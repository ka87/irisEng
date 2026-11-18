/* A unit test for font.c.
 *
 * New code with no earlier copy -- the D3DX font it replaces had no
 * harness and could not have had one, because every answer came from
 * inside d3dx9 -- so this is a direct check rather than a
 * differential one.
 *
 * It uses the real FreeType, vendored in thirdparty/freetype, and the
 * real faces in data/fonts. Nothing about glyph shapes is mocked:
 * GetModuleFileNameA is pointed at the repository so the module finds
 * the same two .ttf files the game would, and what comes back is what
 * the game would rasterize. That is deliberate -- a mocked rasterizer
 * would only check the module's arithmetic against itself.
 *
 * The renderer is mocked, so a case can inspect the coverage map the
 * module built and every vertex it asked to have drawn.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gdi.h"
#include "../font.h"

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

static void okNear(float got, float want, const char* what)
{
    checks++;
    if (got < want - 0.001f || got > want + 0.001f)
    {
        printf("  FAIL %s: got %f want %f\n", what, (double)got, (double)want);
        fails++;
    }
}

/* ------------------------------------------------------- the log, mocked */

static char fmLog[64][256];
static int  fmLogLen;

void DBG_OutputDebugString(char* format, ...)
{
    va_list ap;

    if (fmLogLen >= 64)
        return;
    va_start(ap, format);
    vsnprintf(fmLog[fmLogLen], sizeof(fmLog[0]), format, ap);
    va_end(ap);
    fmLogLen++;
}

static int FM_i_Logged(const char* needle)
{
    int i, n = 0;

    for (i = 0; i < fmLogLen; i++)
        if (strstr(fmLog[i], needle))
            n++;
    return n;
}

/* ---------------------------------------------------- the exe, mocked */

/* Where the module will think the executable is; data/fonts
   resolves against it, so pointing it at the repository root makes
   the module read the real committed faces. */
#include "sdlpath.h"

/* --------------------------------------------- the renderer, mocked */

#define FM_MAXVERTS 8192
#define FM_MAXCOVER (1024 * 1024)

static int fm_viewW, fm_viewH;

static int           fm_createCalls;
static int           fm_destroyCalls;
static int           fm_createFails;
static int           fm_lastW, fm_lastH;
static unsigned char fm_cover[FM_MAXCOVER];
static int           fm_coverSize;

static int                 fm_drawCalls;
static void*               fm_drawTexture;
static int                 fm_drawTriangles;
static tGdi_TexturedVertex fm_verts[FM_MAXVERTS];

static void FM_GetDrawingSize(int* w, int* h)
{
    *w = fm_viewW;
    *h = fm_viewH;
}

static void* FM_p_CreateAlphaTexture(int width, int height, const uint8_t* coverage)
{
    fm_createCalls++;
    fm_lastW = width;
    fm_lastH = height;
    fm_coverSize = width;
    if (width * height > FM_MAXCOVER)
    {
        printf("  FAIL coverage %dx%d past the mock's buffer\n", width, height);
        fails++;
        return NULL;
    }
    memcpy(fm_cover, coverage, (size_t)width * (size_t)height);
    if (fm_createFails)
        return NULL;
    /* a distinct handle per call, so a stale one is visible */
    return (void*)(size_t)(0x1000 + fm_createCalls);
}

static void FM_DestroyAlphaTexture(void* texture)
{
    fm_destroyCalls++;
    ok(texture != NULL, "destroy was given a real handle");
}

static void FM_OverlayDrawTexturedQuads(void* texture, const void* vertices,
                                        int triangleCount)
{
    fm_drawCalls++;
    fm_drawTexture = texture;
    fm_drawTriangles = triangleCount;
    if (triangleCount > 0 && triangleCount * 3 <= FM_MAXVERTS)
        memcpy(fm_verts, vertices,
               sizeof(tGdi_TexturedVertex) * (size_t)triangleCount * 3);
}

static tGdi_DisplayData fm_dd;

/* The module under test. */
#include "../font.c"

/* ------------------------------------------------------------- the rig */

static void FM_Reset(void)
{
    memset(&fm_dd, 0, sizeof(fm_dd));
    fm_dd.GetDrawingSize = FM_GetDrawingSize;
    fm_dd.p_CreateAlphaTexture = FM_p_CreateAlphaTexture;
    fm_dd.DestroyAlphaTexture = FM_DestroyAlphaTexture;
    fm_dd.OverlayDrawTexturedQuads = FM_OverlayDrawTexturedQuads;
    gpst_DX_stDD = &fm_dd;

    fmLogLen = 0;
    dpt_baseCalls = 0;
    dpt_baseOk = 0;
    fm_createCalls = 0;
    fm_destroyCalls = 0;
    fm_createFails = 0;
    fm_lastW = 0;
    fm_lastH = 0;
    fm_coverSize = 0;
    fm_drawCalls = 0;
    fm_drawTriangles = 0;
    fm_drawTexture = NULL;
    fm_viewW = 1024;
    fm_viewH = 600;
    strcpy(dpt_basePath, "./");

    FNT_CloseModule();
    FNT_InitModule();
    fmLogLen = 0;
    fm_destroyCalls = 0;
    dpt_baseCalls = 0;
    dpt_baseOk = 0;
}

/* How many inked bytes a glyph's cell holds in the coverage map. */
static int FM_i_Ink(const tFntGlyph* g)
{
    int x, y, n = 0;

    for (y = 0; y < g->h; y++)
        for (x = 0; x < g->w; x++)
            if (fm_cover[(size_t)(g->y + y) * fm_coverSize + g->x + x])
                n++;
    return n;
}

static const tFntGlyph* FM_pc_Glyph(int face, char c)
{
    return &fnt_faces[face].glyphs[(unsigned char)c - FNT_FIRSTCHAR];
}

/* ----------------------------------------------------------- the cases */

static void FM_CaseLoads(void)
{
    printf("-- the faces load\n");
    FM_Reset();

    ok(FNT_i_Height(FNT_RASTERS) > 0, "the raster face loads from data/fonts");
    okEq(fm_createCalls, 1, "one atlas built for it");
    okEq(fm_lastW, fm_lastH, "the atlas is square");
    ok(fm_lastW > 0 && (fm_lastW & (fm_lastW - 1)) == 0,
       "the atlas side is a power of two");
    okEq(fnt_faces[FNT_RASTERS].builtAt, 10, "the raster face is ten pixels");
    ok(fnt_faces[FNT_RASTERS].ascender > 0, "it has an ascender");
    okEq(fmLogLen, 0, "nothing was logged");

    ok(FNT_i_Height(FNT_OVERLAY) > 0, "the overlay face loads too");
    okEq(fm_createCalls, 2, "and builds an atlas of its own");

    /* Liberation Mono is what the overlay face is, and it is
       monospaced; every advance being equal is the thing that makes a
       command line's columns line up. */
    ok(FT_IS_FIXED_WIDTH(fnt_faces[FNT_OVERLAY].ft) != 0,
       "the overlay face is monospaced");
    okEq(FM_pc_Glyph(FNT_OVERLAY, 'i')->advance,
         FM_pc_Glyph(FNT_OVERLAY, 'W')->advance,
         "so i and W advance alike");

    /* the raster face is not, which is what makes measuring matter */
    ok(FM_pc_Glyph(FNT_RASTERS, 'i')->advance
       != FM_pc_Glyph(FNT_RASTERS, 'W')->advance,
       "the raster face is proportional");
}

static void FM_CaseOverlayHeight(void)
{
    printf("-- the overlay's size follows the viewport\n");

    /* one thirtieth of the viewport, clamped at both ends */
    FM_Reset();
    fm_viewH = 600;
    okEq(fnt_faces[FNT_OVERLAY].builtAt, 0, "nothing built before it is asked");
    FNT_i_Height(FNT_OVERLAY);
    okEq(fnt_faces[FNT_OVERLAY].builtAt, 20, "600 gives twenty");

    FM_Reset();
    fm_viewH = 100;
    FNT_i_Height(FNT_OVERLAY);
    okEq(fnt_faces[FNT_OVERLAY].builtAt, FNT_OVERLAY_MINHEIGHT,
         "a small viewport clamps up");

    FM_Reset();
    fm_viewH = FNT_OVERLAY_MINHEIGHT * 30;
    FNT_i_Height(FNT_OVERLAY);
    okEq(fnt_faces[FNT_OVERLAY].builtAt, FNT_OVERLAY_MINHEIGHT,
         "and the clamp meets the ratio exactly there");

    /* One pixel the other side of it, which is the only viewport
       where an off-by-one in the comparison shows: the ratio asks
       for a size just under the floor rather than far under it. */
    FM_Reset();
    fm_viewH = (FNT_OVERLAY_MINHEIGHT - 1) * 30;
    FNT_i_Height(FNT_OVERLAY);
    okEq(fnt_faces[FNT_OVERLAY].builtAt, FNT_OVERLAY_MINHEIGHT,
         "a ratio one under the floor still clamps up");

    FM_Reset();
    fm_viewH = (FNT_OVERLAY_MAXHEIGHT + 1) * 30;
    FNT_i_Height(FNT_OVERLAY);
    okEq(fnt_faces[FNT_OVERLAY].builtAt, FNT_OVERLAY_MAXHEIGHT,
         "and a ratio one over the ceiling clamps down");

    FM_Reset();
    fm_viewH = 3000;
    FNT_i_Height(FNT_OVERLAY);
    okEq(fnt_faces[FNT_OVERLAY].builtAt, FNT_OVERLAY_MAXHEIGHT,
         "a large viewport clamps down");

    /* and the raster face does not move with it */
    okEq(fnt_faces[FNT_RASTERS].builtAt, 0, "the raster face was not touched");
    FNT_i_Height(FNT_RASTERS);
    okEq(fnt_faces[FNT_RASTERS].builtAt, 10, "and stays at ten");
}

static void FM_CaseRebuild(void)
{
    int height;

    printf("-- a resize rebuilds, an unchanged size does not\n");
    FM_Reset();

    fm_viewH = 600;
    /* in two steps: the call is what builds the face, and reading
       lineHeight as the other argument would read it beforehand */
    height = FNT_i_Height(FNT_OVERLAY);
    okEq(height, fnt_faces[FNT_OVERLAY].lineHeight,
         "the height is the face's line height");
    okEq(fm_createCalls, 1, "built once");

    FNT_i_Height(FNT_OVERLAY);
    FNT_i_TextWidth(FNT_OVERLAY, "hello");
    FNT_DrawText(FNT_OVERLAY, "hello", 0, 0, 0xFFFFFFFF);
    okEq(fm_createCalls, 1, "and not again at the same size");
    okEq(fm_destroyCalls, 0, "with nothing released");

    fm_viewH = 1200;
    FNT_i_Height(FNT_OVERLAY);
    okEq(fm_createCalls, 2, "a new size rebuilds");
    okEq(fm_destroyCalls, 1, "and gives the old atlas back");
    okEq(fnt_faces[FNT_OVERLAY].builtAt, 40, "at the size the viewport asks");
}

static void FM_CaseWidth(void)
{
    int advance;
    int i;
    int sum;

    printf("-- measuring\n");
    FM_Reset();
    FNT_i_Height(FNT_OVERLAY);

    advance = FM_pc_Glyph(FNT_OVERLAY, 'A')->advance;
    ok(advance > 0, "a glyph advances");
    okEq(FNT_i_TextWidth(FNT_OVERLAY, "AAAA"), advance * 4,
         "four of them are four advances wide");
    okEq(FNT_i_TextWidth(FNT_OVERLAY, ""), 0, "an empty string is nothing");
    okEq(FNT_i_TextWidth(FNT_OVERLAY, NULL), 0, "and so is no string");

    /* the proportional face: the sum of the parts, not a multiple */
    FNT_i_Height(FNT_RASTERS);
    sum = 0;
    for (i = 0; i < 5; i++)
        sum += FM_pc_Glyph(FNT_RASTERS, "Wimpy"[i])->advance;
    okEq(FNT_i_TextWidth(FNT_RASTERS, "Wimpy"), sum,
         "a proportional string is its advances summed");
    ok(FNT_i_TextWidth(FNT_RASTERS, "WWWWW")
       > FNT_i_TextWidth(FNT_RASTERS, "iiiii"),
       "and W is wider than i");

    /* a character the atlas does not hold measures as the fallback,
       so a string containing one measures as wide as it draws */
    okEq(FNT_i_TextWidth(FNT_RASTERS, "\x01\x02"),
         FNT_i_TextWidth(FNT_RASTERS, "??"),
         "an unprintable pair measures as two fallbacks");
    okEq(FNT_i_TextWidth(FNT_RASTERS, "\xFF"),
         FNT_i_TextWidth(FNT_RASTERS, "?"),
         "and so does a high byte");
}

static void FM_CaseQuads(void)
{
    const tFntGlyph* g;
    float            baseline;
    float            scale;
    int              i;

    printf("-- the quads\n");
    FM_Reset();
    FNT_i_Height(FNT_OVERLAY);

    FNT_DrawText(FNT_OVERLAY, "A", 100, 50, 0xFF336699);
    okEq(fm_drawCalls, 1, "one draw call");
    okEq(fm_drawTriangles, 2, "one glyph is two triangles");
    ok(fm_drawTexture != NULL, "drawn against the atlas");

    g = FM_pc_Glyph(FNT_OVERLAY, 'A');
    baseline = 50.0f + (float)fnt_faces[FNT_OVERLAY].ascender;
    scale = 1.0f / (float)fnt_faces[FNT_OVERLAY].atlasSize;

    /* (x, y) is the top of the line; the glyph hangs from the
       baseline by its own bearing */
    okNear(fm_verts[0].pos.x, 100.0f + (float)g->bearingX, "left edge");
    okNear(fm_verts[0].pos.y, baseline - (float)g->bearingY, "top edge");
    okNear(fm_verts[1].pos.x, 100.0f + (float)g->bearingX + (float)g->w,
           "right edge");
    okNear(fm_verts[2].pos.y, baseline - (float)g->bearingY + (float)g->h,
           "bottom edge");

    okNear(fm_verts[0].u, (float)g->x * scale, "u at the cell");
    okNear(fm_verts[0].v, (float)g->y * scale, "v at the cell");
    okNear(fm_verts[1].u, (float)(g->x + g->w) * scale, "u across the glyph");
    okNear(fm_verts[2].v, (float)(g->y + g->h) * scale, "v down the glyph");

    for (i = 0; i < 6; i++)
    {
        okEq((long)fm_verts[i].color, 0xFF336699, "the colour on every vertex");
        okNear(fm_verts[i].pos.z, 0.0f, "flat in z");
        ok(fm_verts[i].u >= 0.0f && fm_verts[i].u <= 1.0f, "u is in range");
        ok(fm_verts[i].v >= 0.0f && fm_verts[i].v <= 1.0f, "v is in range");
    }

    /* a space has no bitmap, so it emits no quad but still moves the
       pen -- which is what keeps measuring and drawing agreed */
    FM_Reset();
    FNT_i_Height(FNT_OVERLAY);
    FNT_DrawText(FNT_OVERLAY, " ", 0, 0, 0xFFFFFFFF);
    okEq(fm_drawCalls, 0, "a space draws nothing at all");

    FNT_DrawText(FNT_OVERLAY, "A A", 0, 0, 0xFFFFFFFF);
    okEq(fm_drawTriangles, 4, "two glyphs across a space are four triangles");
    g = FM_pc_Glyph(FNT_OVERLAY, 'A');
    okNear(fm_verts[6].pos.x, (float)(g->advance * 2) + (float)g->bearingX,
           "the second A sits two advances along");

    FNT_DrawText(FNT_OVERLAY, "", 0, 0, 0xFFFFFFFF);
    okEq(fm_drawTriangles, 4, "an empty string draws nothing new");
    FNT_DrawText(FNT_OVERLAY, NULL, 0, 0, 0xFFFFFFFF);
    okEq(fm_drawTriangles, 4, "and neither does no string");

    /* The left bearing, checked on a glyph that actually has one.
       Taking whichever character A happens to be would make this
       vacuous the moment its bearing were zero -- which at this size
       it is -- so the glyph is picked for the property being
       tested, and the search failing is itself a failure. */
    {
        int  i;
        char with = 0;

        for (i = FNT_FIRSTCHAR; i <= FNT_LASTCHAR && !with; i++)
            if (FM_pc_Glyph(FNT_OVERLAY, (char)i)->bearingX > 0
                && FM_pc_Glyph(FNT_OVERLAY, (char)i)->w > 0)
                with = (char)i;
        ok(with != 0, "the face has a glyph with a left bearing");
        if (with)
        {
            char one[2];

            one[0] = with;
            one[1] = 0;
            g = FM_pc_Glyph(FNT_OVERLAY, with);
            FNT_DrawText(FNT_OVERLAY, one, 200, 0, 0xFFFFFFFF);
            okNear(fm_verts[0].pos.x, 200.0f + (float)g->bearingX,
                   "which the quad is inset by");
        }
    }
}

static void FM_CaseCoverage(void)
{
    printf("-- the coverage map\n");
    FM_Reset();
    FNT_i_Height(FNT_OVERLAY);

    ok(FM_i_Ink(FM_pc_Glyph(FNT_OVERLAY, 'A')) > 0, "A has ink");
    ok(FM_i_Ink(FM_pc_Glyph(FNT_OVERLAY, 'W')) > 0, "W has ink");
    okEq(FM_pc_Glyph(FNT_OVERLAY, ' ')->w, 0, "a space has no bitmap");
    ok(FM_i_Ink(FM_pc_Glyph(FNT_OVERLAY, '.'))
       < FM_i_Ink(FM_pc_Glyph(FNT_OVERLAY, 'W')),
       "a full stop has less of it than a W");

    /* every cell inside the atlas, which is what stops a packing
       error from being a read past the texture */
    {
        int i;

        for (i = 0; i < FNT_GLYPHCOUNT; i++)
        {
            const tFntGlyph* g = &fnt_faces[FNT_OVERLAY].glyphs[i];

            ok(g->x + g->w <= fnt_faces[FNT_OVERLAY].atlasSize
               && g->y + g->h <= fnt_faces[FNT_OVERLAY].atlasSize,
               "the glyph's cell is inside the atlas");
        }
    }
}

static void FM_CaseNoFont(void)
{
    printf("-- a missing face\n");
    FM_Reset();
    strcpy(dpt_basePath, "/nowhere/at/all/");

    okEq(FNT_i_Height(FNT_OVERLAY), 0, "no height without a face");
    okEq(fm_createCalls, 0, "no atlas built");
    okEq(FM_i_Logged("no "), 1, "and it said so once");

    FNT_i_Height(FNT_OVERLAY);
    FNT_i_TextWidth(FNT_OVERLAY, "hello");
    FNT_DrawText(FNT_OVERLAY, "hello", 0, 0, 0xFFFFFFFF);
    okEq(fmLogLen, 1, "and does not say it again every frame");
    okEq(fm_drawCalls, 0, "nothing is drawn");
    okEq(FNT_i_TextWidth(FNT_OVERLAY, "hello"), 0, "and nothing measures");

    /* the other face is unaffected: one missing file is not all of
       them */
    strcpy(dpt_basePath, "./");
    ok(FNT_i_Height(FNT_RASTERS) > 0, "the other face still loads");
}

static void FM_CaseNoPath(void)
{
    printf("-- nowhere to look\n");
    FM_Reset();
    dpt_basePath[0] = 0;

    okEq(FNT_i_Height(FNT_RASTERS), 0, "no height without a path");
    okEq(FM_i_Logged("where"), 1, "and it said why");
    FNT_DrawText(FNT_RASTERS, "x", 0, 0, 0xFFFFFFFF);
    okEq(fm_drawCalls, 0, "nothing drawn");
}

static void FM_CaseLongPath(void)
{
    int i;

    printf("-- a path too long to hold\n");
    FM_Reset();

    /* An executable deep enough that its own path fits but the path
       plus data/fonts and the file name does not. Not a case worth
       handling beyond declining to look -- but it has to decline,
       rather than build a truncated path and report success.

       The directory has to land in a window: short enough that the
       mock will hand it over at all, long enough that appending
       data/fonts and a 26-character file name overruns MAX_PATH.
       Getting that wrong is how the first version of this case
       failed inside the mock instead, where it proved nothing, so
       the case asserts that the mock answered. */
    for (i = 0; i < 229; i++)
        dpt_basePath[i] = 'd';
    dpt_basePath[229] = '/';

    okEq(FNT_i_Height(FNT_RASTERS), 0, "no height");
    okEq(dpt_baseOk, 1, "and the path of the executable itself did arrive");
    okEq(fm_createCalls, 0, "no atlas");
    okEq(FM_i_Logged("where"), 1, "and it said it could not place the file");
}

static void FM_CaseRendererRefuses(void)
{
    printf("-- the renderer will not take the atlas\n");
    FM_Reset();
    fm_createFails = 1;

    okEq(FNT_i_Height(FNT_RASTERS), 0, "no height");
    okEq(fm_createCalls, 1, "it was offered once");
    okEq(FM_i_Logged("atlas"), 1, "and the refusal was logged");
    FNT_DrawText(FNT_RASTERS, "x", 0, 0, 0xFFFFFFFF);
    okEq(fm_drawCalls, 0, "nothing drawn");
    okEq(fm_createCalls, 1, "and it is not offered again");
}

static void FM_CaseNoRenderer(void)
{
    printf("-- no renderer at all\n");
    FM_Reset();
    gpst_DX_stDD = NULL;

    okEq(FNT_i_Height(FNT_OVERLAY), 0, "no height");
    okEq(FNT_i_TextWidth(FNT_OVERLAY, "hello"), 0, "no width");
    FNT_DrawText(FNT_OVERLAY, "hello", 0, 0, 0xFFFFFFFF);
    okEq(fm_drawCalls, 0, "and no draw");
    gpst_DX_stDD = &fm_dd;
}

static void FM_CaseClose(void)
{
    printf("-- closing\n");
    FM_Reset();

    FNT_i_Height(FNT_RASTERS);
    FNT_i_Height(FNT_OVERLAY);
    okEq(fm_createCalls, 2, "two atlases live");

    fm_destroyCalls = 0;
    FNT_CloseModule();
    okEq(fm_destroyCalls, 2, "both are given back");
    ok(fnt_lib == NULL, "and FreeType is shut down");

    FNT_CloseModule();
    okEq(fm_destroyCalls, 2, "closing twice releases nothing twice");

    /* and the module comes back afterwards */
    FNT_InitModule();
    ok(FNT_i_Height(FNT_RASTERS) > 0, "it can be started again");
}

int main(void)
{
    printf("=== font unit test ===\n");
    FM_CaseLoads();
    FM_CaseOverlayHeight();
    FM_CaseRebuild();
    FM_CaseWidth();
    FM_CaseQuads();
    FM_CaseCoverage();
    FM_CaseNoFont();
    FM_CaseNoPath();
    FM_CaseLongPath();
    FM_CaseRendererRefuses();
    FM_CaseNoRenderer();
    FM_CaseClose();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
