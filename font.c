#include "font.h"
#include "gdi.h"
#include "datapath.h"
#include "dbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H

/* The largest atlas worth building. Printable ASCII at the overlay's
   biggest size fits in 512; the loop below stops here rather than
   asking a renderer for something it may refuse. */
#define FNT_ATLASMAX 2048

/* One glyph's place in the atlas and its place on the line. All in
   pixels, all at the size the atlas was built at. */
typedef struct sFntGlyph
{
    short x, y;          /* where it sits in the atlas */
    short w, h;          /* how big its bitmap is; 0 for a space */
    short bearingX;      /* from the pen to the left of the bitmap */
    short bearingY;      /* from the baseline up to its top */
    short advance;       /* how far the pen then moves */
} tFntGlyph;

typedef struct sFntFace
{
    const char* file;
    int         wantedHeight;  /* 0 means: follow the viewport */
    FT_Face     ft;
    void*       atlas;         /* the renderer's texture */
    int         atlasSize;
    int         builtAt;       /* the pixel height the atlas holds */
    int         lineHeight;
    int         ascender;
    tFntGlyph   glyphs[FNT_GLYPHCOUNT];
    int         ready;
    int         failed;        /* tried once, could not; do not retry */
} tFntFace;

static FT_Library fnt_lib;
static int        fnt_libFailed;
static tFntFace   fnt_faces[FNT_COUNT];

/* Quads for one call. A line longer than this is drawn as far as it
   fits rather than overrunning: the overlay's own text buffer is
   shorter than this, so it is a backstop and not a limit anyone
   meets. */
#define FNT_MAXQUADS 512
static tGdi_TexturedVertex fnt_verts[FNT_MAXQUADS * 6];

/* ------------------------------------------------------- the library */

static int FNT_l_b_Library(void)
{
    if (fnt_lib)
        return 1;
    if (fnt_libFailed)
        return 0;
    if (FT_Init_FreeType(&fnt_lib))
    {
        fnt_lib = NULL;
        fnt_libFailed = 1;
        DBG_OutputDebugString("FNT: FreeType would not start; no text\n");
        return 0;
    }
    return 1;
}

/* --------------------------------------------------------- the atlas */

/* The smallest power of two square that holds the glyphs in a grid of
   equal cells. Equal cells waste a little area and make the placement
   arithmetic something a reader can check. */
static int FNT_l_i_AtlasSize(int cellW, int cellH)
{
    int size;
    int cols;
    int rows;

    for (size = 64; size <= FNT_ATLASMAX; size <<= 1)
    {
        cols = size / cellW;
        if (cols < 1)
            continue;
        rows = (FNT_GLYPHCOUNT + cols - 1) / cols;
        if (rows * cellH <= size)
            return size;
    }
    return 0;
}

/* Rasterizes the printable range into one coverage map and hands it
   to the renderer. Loads every glyph twice: once to find the largest
   bitmap, once to copy it in, because FreeType keeps one glyph slot
   and the atlas cannot be sized before the first pass has finished.
   It happens on a size change, not on a frame. */
static int FNT_l_b_BuildAtlas(tFntFace* f, int pixelHeight)
{
    unsigned char* cover;
    int            cellW = 1, cellH = 1;
    int            size, cols;
    int            i, row;

    if (FT_Set_Pixel_Sizes(f->ft, 0, (FT_UInt)pixelHeight))
    {
        DBG_OutputDebugString("FNT: %s will not size to %d\n", f->file, pixelHeight);
        return 0;
    }

    for (i = FNT_FIRSTCHAR; i <= FNT_LASTCHAR; i++)
    {
        if (FT_Load_Char(f->ft, (FT_ULong)i, FT_LOAD_RENDER))
            continue;
        if ((int)f->ft->glyph->bitmap.width > cellW)
            cellW = (int)f->ft->glyph->bitmap.width;
        if ((int)f->ft->glyph->bitmap.rows > cellH)
            cellH = (int)f->ft->glyph->bitmap.rows;
    }

    size = FNT_l_i_AtlasSize(cellW, cellH);
    if (!size)
    {
        DBG_OutputDebugString("FNT: %s at %d needs an atlas past %d\n",
                              f->file, pixelHeight, FNT_ATLASMAX);
        return 0;
    }
    cols = size / cellW;

    cover = (unsigned char*)calloc((size_t)size * (size_t)size, 1);
    if (!cover)
        return 0;

    for (i = 0; i < FNT_GLYPHCOUNT; i++)
    {
        tFntGlyph*  g = &f->glyphs[i];
        FT_Bitmap*  bm;
        int         cx = (i % cols) * cellW;
        int         cy = (i / cols) * cellH;

        memset(g, 0, sizeof(*g));
        if (FT_Load_Char(f->ft, (FT_ULong)(FNT_FIRSTCHAR + i), FT_LOAD_RENDER))
            continue;

        bm = &f->ft->glyph->bitmap;
        g->advance  = (short)(f->ft->glyph->advance.x >> 6);
        g->bearingX = (short)f->ft->glyph->bitmap_left;
        g->bearingY = (short)f->ft->glyph->bitmap_top;
        g->x = (short)cx;
        g->y = (short)cy;

        /* The smooth rasterizer gives 8-bit coverage. Anything else
           would need converting, and rather than convert it wrongly
           the glyph is left blank and still advances. */
        if (bm->pixel_mode != FT_PIXEL_MODE_GRAY)
            continue;

        g->w = (short)bm->width;
        g->h = (short)bm->rows;
        for (row = 0; row < (int)bm->rows; row++)
            memcpy(cover + (size_t)(cy + row) * size + cx,
                   bm->buffer + (size_t)row * bm->pitch, bm->width);
    }

    if (f->atlas)
    {
        gpst_DX_stDD->DestroyAlphaTexture(f->atlas);
        f->atlas = NULL;
    }
    f->atlas = gpst_DX_stDD->p_CreateAlphaTexture(size, size, cover);
    free(cover);
    if (!f->atlas)
    {
        DBG_OutputDebugString("FNT: renderer would not take a %dx%d atlas\n",
                              size, size);
        return 0;
    }

    f->atlasSize  = size;
    f->builtAt    = pixelHeight;
    f->lineHeight = (int)(f->ft->size->metrics.height >> 6);
    f->ascender   = (int)(f->ft->size->metrics.ascender >> 6);
    f->ready      = 1;
    return 1;
}

/* ---------------------------------------------------------- the size */

/* The overlay's height follows the viewport: the raster font was
   right for a small window and is unreadable as a command line. The
   clamp keeps it legible on a small surface and stops it swallowing a
   large one. */
static int FNT_l_i_OverlayHeight(void)
{
    int w = 0, h = 0;
    int height;

    gpst_DX_stDD->GetDrawingSize(&w, &h);
    height = h / 30;
    if (height < FNT_OVERLAY_MINHEIGHT)
        height = FNT_OVERLAY_MINHEIGHT;
    if (height > FNT_OVERLAY_MAXHEIGHT)
        height = FNT_OVERLAY_MAXHEIGHT;
    return height;
}

/* The face, built at the size it should be drawn at, or NULL. One
   failure is remembered: a missing file would otherwise be opened,
   missed and logged on every frame. */
static tFntFace* FNT_l_p_Ready(int which)
{
    tFntFace* f;
    char      path[MAX_PATH];
    int       height;

    if (which < 0 || which >= FNT_COUNT)
        return NULL;
    f = &fnt_faces[which];
    if (f->failed)
        return NULL;
    if (!gpst_DX_stDD)
        return NULL;
    if (!FNT_l_b_Library())
        return NULL;

    if (!f->ft)
    {
        if (!DPT_b_DataPath("fonts", f->file, path, sizeof(path)))
        {
            f->failed = 1;
            DBG_OutputDebugString("FNT: cannot work out where %s should be\n",
                                  f->file);
            return NULL;
        }
        if (FT_New_Face(fnt_lib, path, 0, &f->ft))
        {
            f->ft = NULL;
            f->failed = 1;
            DBG_OutputDebugString("FNT: no %s; text is off\n", path);
            return NULL;
        }
    }

    height = f->wantedHeight ? f->wantedHeight : FNT_l_i_OverlayHeight();
    if (f->ready && f->builtAt == height)
        return f;
    if (!FNT_l_b_BuildAtlas(f, height))
    {
        f->failed = 1;
        return NULL;
    }
    return f;
}

/* ---------------------------------------------------------- the glyph */

static const tFntGlyph* FNT_l_pc_Glyph(const tFntFace* f, unsigned char c)
{
    if (c < FNT_FIRSTCHAR || c > FNT_LASTCHAR)
        c = FNT_FALLBACKCHAR;
    return &f->glyphs[c - FNT_FIRSTCHAR];
}

/* ------------------------------------------------------------- public */

void FNT_InitModule(void)
{
    memset(fnt_faces, 0, sizeof(fnt_faces));
    fnt_faces[FNT_RASTERS].file = "LiberationSans-Regular.ttf";
    fnt_faces[FNT_OVERLAY].file = "LiberationMono-Regular.ttf";

    /* The raster readouts were drawn at ten pixels and the layout
       around them assumes it. The overlay takes its size from the
       viewport instead, which is what the zero means. */
    fnt_faces[FNT_RASTERS].wantedHeight = 10;
    fnt_faces[FNT_OVERLAY].wantedHeight = 0;
}

void FNT_CloseModule(void)
{
    int i;

    for (i = 0; i < FNT_COUNT; i++)
    {
        tFntFace* f = &fnt_faces[i];

        if (f->atlas && gpst_DX_stDD)
            gpst_DX_stDD->DestroyAlphaTexture(f->atlas);
        f->atlas = NULL;
        if (f->ft)
            FT_Done_Face(f->ft);
        f->ft = NULL;
        f->ready = 0;
        f->builtAt = 0;
    }
    if (fnt_lib)
        FT_Done_FreeType(fnt_lib);
    fnt_lib = NULL;
    fnt_libFailed = 0;
}

int FNT_i_Height(int face)
{
    tFntFace* f = FNT_l_p_Ready(face);

    return f ? f->lineHeight : 0;
}

int FNT_i_TextWidth(int face, const char* text)
{
    tFntFace* f;
    int       w = 0;

    if (!text)
        return 0;
    f = FNT_l_p_Ready(face);
    if (!f)
        return 0;
    for (; *text; text++)
        w += FNT_l_pc_Glyph(f, (unsigned char)*text)->advance;
    return w;
}

void FNT_DrawText(int face, const char* text, int x, int y, uint32_t color)
{
    tFntFace* f;
    int       quads = 0;
    float     baseline;
    float     scale;

    if (!text || !*text)
        return;
    f = FNT_l_p_Ready(face);
    if (!f)
        return;

    /* (x, y) is the top of the line, which is where a rectangle's top
       used to put the text. */
    baseline = (float)y + (float)f->ascender;
    scale = 1.0f / (float)f->atlasSize;

    for (; *text; text++)
    {
        const tFntGlyph*     g = FNT_l_pc_Glyph(f, (unsigned char)*text);
        tGdi_TexturedVertex* v;
        float                x0, y0, x1, y1;
        float                u0, v0, u1, v1;
        int                  k;

        if (g->w && g->h)
        {
            if (quads >= FNT_MAXQUADS)
                break;

            x0 = (float)x + (float)g->bearingX;
            y0 = baseline - (float)g->bearingY;
            x1 = x0 + (float)g->w;
            y1 = y0 + (float)g->h;
            u0 = (float)g->x * scale;
            v0 = (float)g->y * scale;
            u1 = (float)(g->x + g->w) * scale;
            v1 = (float)(g->y + g->h) * scale;

            v = &fnt_verts[quads * 6];
            v[0].pos.x = x0; v[0].pos.y = y0; v[0].u = u0; v[0].v = v0;
            v[1].pos.x = x1; v[1].pos.y = y0; v[1].u = u1; v[1].v = v0;
            v[2].pos.x = x1; v[2].pos.y = y1; v[2].u = u1; v[2].v = v1;
            v[3].pos.x = x0; v[3].pos.y = y0; v[3].u = u0; v[3].v = v0;
            v[4].pos.x = x1; v[4].pos.y = y1; v[4].u = u1; v[4].v = v1;
            v[5].pos.x = x0; v[5].pos.y = y1; v[5].u = u0; v[5].v = v1;
            for (k = 0; k < 6; k++)
            {
                v[k].pos.z = 0.0f;
                v[k].color = color;
            }
            quads++;
        }
        x += g->advance;
    }

    if (quads)
        gpst_DX_stDD->OverlayDrawTexturedQuads(f->atlas, fnt_verts, quads * 2);
}
