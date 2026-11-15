/* The two functions that replaced this renderer's last uses of D3DX.
 *
 * These are the one part of the renderer the differential comparison
 * cannot check. It works by running the frozen pre-rewrite copy beside
 * the rewrite over identical state; the frozen copy calls
 * D3DXLoadSurfaceFromMemory and D3DXFilterTexture, which are the
 * things being replaced and are not available here, so
 * tests/wingdi_mocks.c stands both sides in with the same recorded
 * stub. The two sides therefore agree by construction and say nothing
 * about whether the replacement behaves like the original.
 *
 * So this file is the only thing standing behind them, and it is only
 * as good as the reading of what D3DX did. That is not hypothetical:
 * the surface load was written as a row copy, which is right only when
 * the destination surface is the same size as the rectangle handed
 * over. D3DXLoadSurfaceFromMemory with no destination rectangle
 * stretches to fill the surface, the engine does hand it a surface of
 * a different shape, and the in-game video screens broke. The cases
 * below did not catch it because the surface mock reported a zeroed
 * descriptor, so the destination had no size to disagree with.
 *
 * When adding to this file, prefer a case where the inputs disagree
 * with each other over one where they match.
 */

/* A unit test for win/wingditex.c.
 *
 * New code replacing two D3DX calls, and a direct check rather than a
 * differential one -- deliberately. The D3DX mocks in the renderer's
 * own harness only ever logged that they had been called; they moved
 * no pixels, so there is nothing there to compare a real
 * implementation against. What matters about a box filter is the
 * pixels it produces, and that is what this checks.
 *
 * The textures and surfaces are built here, with real buffers behind
 * them, so a case can set level 0 and read back what the filter put
 * in the levels below it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../win/wingditex.h"

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

/* ------------------------------------------------ a texture, for real */

#define WT_MAXLEVELS 12

typedef struct sWtLevel
{
    int            width;
    int            height;
    int            pitch;      /* deliberately more than width * 4 */
    unsigned char* bits;
} tWtLevel;

typedef struct sWtTexture
{
    IDirect3DTexture9Vtbl* lpVtbl;
    int                    levels;
    D3DFORMAT              format;
    tWtLevel               level[WT_MAXLEVELS];
    int                    locked[WT_MAXLEVELS];
    int                    lockCalls;
    int                    unlockCalls;
    int                    failLockOfLevel;   /* -1 for none */
    int                    failDescOfLevel;
} tWtTexture;

static ULONG WtRelease(IDirect3DTexture9* t) { (void)t; return 0; }

static DWORD WtGetLevelCount(IDirect3DTexture9* t)
{
    return (DWORD)((tWtTexture*)t)->levels;
}

static HRESULT WtGetLevelDesc(IDirect3DTexture9* t, UINT l, D3DSURFACE_DESC* d)
{
    tWtTexture* tex = (tWtTexture*)t;

    if ((int)l >= tex->levels || (int)l == tex->failDescOfLevel)
        return -1;
    memset(d, 0, sizeof(*d));
    d->Width = (UINT)tex->level[l].width;
    d->Height = (UINT)tex->level[l].height;
    d->Format = tex->format;
    return 0;
}

static HRESULT WtLockRect(IDirect3DTexture9* t, UINT l, D3DLOCKED_RECT* r,
                          const RECT* rect, DWORD flags)
{
    tWtTexture* tex = (tWtTexture*)t;

    (void)rect;
    (void)flags;
    tex->lockCalls++;
    if ((int)l >= tex->levels || (int)l == tex->failLockOfLevel)
        return -1;
    tex->locked[l]++;
    r->pBits = tex->level[l].bits;
    r->Pitch = tex->level[l].pitch;
    return 0;
}

static HRESULT WtUnlockRect(IDirect3DTexture9* t, UINT l)
{
    tWtTexture* tex = (tWtTexture*)t;

    tex->unlockCalls++;
    if ((int)l < tex->levels)
        tex->locked[l]--;
    return 0;
}

static HRESULT WtGetSurfaceLevel(IDirect3DTexture9* t, UINT l, IDirect3DSurface9** s)
{
    (void)t; (void)l; (void)s;
    return -1;
}

static HRESULT WtAddDirtyRect(IDirect3DTexture9* t, const RECT* r)
{
    (void)t; (void)r;
    return 0;
}

static IDirect3DTexture9Vtbl wtVtbl = {
    .Release = WtRelease,
    .GetSurfaceLevel = WtGetSurfaceLevel,
    .AddDirtyRect = WtAddDirtyRect,
    .GetLevelCount = WtGetLevelCount,
    .GetLevelDesc = WtGetLevelDesc,
    .LockRect = WtLockRect,
    .UnlockRect = WtUnlockRect
};

/* A full mip chain down to 1x1, every level's pitch padded so that a
   filter which assumes pitch == width * 4 is caught. */
static void WT_Build(tWtTexture* tex, int w, int h)
{
    int i;

    memset(tex, 0, sizeof(*tex));
    tex->lpVtbl = &wtVtbl;
    tex->format = D3DFMT_A8R8G8B8;
    tex->failLockOfLevel = -1;
    tex->failDescOfLevel = -1;
    tex->levels = 0;
    for (i = 0; i < WT_MAXLEVELS; i++)
    {
        tex->level[i].width = w;
        tex->level[i].height = h;
        tex->level[i].pitch = w * 4 + 16;
        tex->level[i].bits = (unsigned char*)calloc((size_t)tex->level[i].pitch
                                                    * (size_t)h, 1);
        tex->levels = i + 1;
        if (w == 1 && h == 1)
            break;
        if (w > 1) w /= 2;
        if (h > 1) h /= 2;
    }
}

static void WT_Free(tWtTexture* tex)
{
    int i;

    for (i = 0; i < tex->levels; i++)
        free(tex->level[i].bits);
}

static unsigned char* WT_Pixel(tWtTexture* tex, int level, int x, int y)
{
    return tex->level[level].bits + (size_t)y * tex->level[level].pitch
           + (size_t)x * 4;
}

static void WT_Set(tWtTexture* tex, int level, int x, int y,
                   int b, int g, int r, int a)
{
    unsigned char* p = WT_Pixel(tex, level, x, y);

    p[0] = (unsigned char)b;
    p[1] = (unsigned char)g;
    p[2] = (unsigned char)r;
    p[3] = (unsigned char)a;
}

/* ---------------------------------------------- a surface, for real */

typedef struct sWtSurface
{
    IDirect3DSurface9Vtbl* lpVtbl;
    unsigned char*         bits;
    int                    pitch;
    /* The size the surface says it is, which is the thing the load
       has to fit itself to. The mock used to report a zeroed
       descriptor, so a load that ignored the destination size looked
       correct here -- and ignoring it is exactly how the in-game
       video screens broke. */
    int                    width;
    int                    height;
    int                    lockCalls;
    int                    unlockCalls;
    int                    failLock;
    int                    failDesc;
} tWtSurface;

static ULONG WsRelease(IDirect3DSurface9* s) { (void)s; return 0; }

static HRESULT WsGetDesc(IDirect3DSurface9* s, D3DSURFACE_DESC* d)
{
    tWtSurface* surf = (tWtSurface*)s;

    /* Rubbish rather than zeroes when it fails, which is what a
       caller that ignores the return value would then act on. Zeroing
       it made ignoring the return value harmless and therefore
       untestable. */
    memset(d, 0, sizeof(*d));
    if (surf->failDesc)
    {
        memset(d, 0x7F, sizeof(*d));
        return -1;
    }
    d->Width = (UINT)surf->width;
    d->Height = (UINT)surf->height;
    d->Format = D3DFMT_X8R8G8B8;
    return 0;
}

static HRESULT WsLockRect(IDirect3DSurface9* s, D3DLOCKED_RECT* r,
                          const RECT* rect, DWORD flags)
{
    tWtSurface* surf = (tWtSurface*)s;

    (void)rect;
    (void)flags;
    surf->lockCalls++;
    if (surf->failLock)
        return -1;
    r->pBits = surf->bits;
    r->Pitch = surf->pitch;
    return 0;
}

static HRESULT WsUnlockRect(IDirect3DSurface9* s)
{
    ((tWtSurface*)s)->unlockCalls++;
    return 0;
}

static IDirect3DSurface9Vtbl wsVtbl = {
    .Release = WsRelease,
    .GetDesc = WsGetDesc,
    .LockRect = WsLockRect,
    .UnlockRect = WsUnlockRect
};

/* ---------------------------------------------------------- the cases */

/* A surface whose size differs from the rectangle handed over.
 *
 * This is the case the game hits and the one that was missing: the
 * engine keeps a texture's width in its height field, the two
 * creation paths undo that differently, and so the one caller can
 * hand a width-by-height rectangle to a surface that was made
 * height-by-width. The original stretched to fit. A row copy writes
 * past the end of every row.
 */
static void caseSurfaceLoadStretch(void)
{
    /* 4 wide, 2 tall, each pixel's red channel saying which column it
       is and its green which row, so where a pixel came from is
       readable off the result. */
    unsigned char src[4 * 2 * 4];
    unsigned char dest[64 * 64 * 4 + 256];
    tWtSurface    surf;
    int           x, y;
    int           bad;

    printf("-- a surface that is not the size of the rectangle\n");

    for (y = 0; y < 2; y++)
        for (x = 0; x < 4; x++)
        {
            unsigned char* p = &src[(y * 4 + x) * 4];

            p[0] = 0;                        /* blue */
            p[1] = (unsigned char)(y + 1);   /* green: the row */
            p[2] = (unsigned char)(x + 1);   /* red: the column */
            p[3] = 0xFF;
        }

    /* The transposed case: a 4 by 2 rectangle into a 2 by 4 surface.
       Nothing may be written past the end of a row, and nothing past
       the end of the surface -- the sentinel after it says so. */
    memset(dest, 0xCD, sizeof(dest));
    memset(&surf, 0, sizeof(surf));
    surf.lpVtbl = &wsVtbl;
    surf.bits = dest;
    surf.width = 2;
    surf.height = 4;
    surf.pitch = 2 * 4;          /* tight, so an overrun lands in the next row */

    ok(D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 4 * 4, 4, 2),
       "a transposed surface still loads");

    /* Which source pixel each destination pixel came from, not merely
       that it came from somewhere: a stretch that took its rows from
       the width and its columns from the height would land inside the
       source everywhere and be wrong everywhere. */
    bad = 0;
    for (y = 0; y < 4; y++)
        for (x = 0; x < 2; x++)
        {
            const unsigned char* p = &dest[(y * 2 + x) * 4];
            int wantCol = ((x * 2 + 1) * 4) / (2 * 2) + 1;
            int wantRow = ((y * 2 + 1) * 2) / (4 * 2) + 1;

            if (p[2] != wantCol || p[1] != wantRow || p[3] != 0xFF)
                bad++;
        }
    okEq(bad, 0, "and each pixel comes from the source pixel it should");

    bad = 0;
    for (x = 2 * 4 * 4; x < (int)sizeof(dest); x++)
        if (dest[x] != 0xCD)
            bad++;
    okEq(bad, 0, "with nothing written past the end of the surface");

    /* Scaled up: a 4 by 2 rectangle into an 8 by 4 surface, which
       doubles each pixel. Point sampled, so the corners are the
       source's corners exactly. */
    memset(dest, 0xCD, sizeof(dest));
    surf.width = 8;
    surf.height = 4;
    surf.pitch = 8 * 4;
    ok(D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 4 * 4, 4, 2),
       "a larger surface loads");
    okEq(dest[0 * 4 + 2], 1, "the top left is the source's first column");
    okEq(dest[0 * 4 + 1], 1, "and its first row");
    okEq(dest[7 * 4 + 2], 4, "the top right is the source's last column");
    okEq(dest[(3 * 8 + 7) * 4 + 1], 2,
         "and the bottom right is its last row");
    /* doubled rather than smeared: the second destination column is
       still the first source column */
    okEq(dest[1 * 4 + 2], 1, "each source pixel covers two across");
    okEq(dest[2 * 4 + 2], 2, "before the next one starts");

    /* Scaled down: a 4 by 2 rectangle into a 2 by 1 surface. */
    memset(dest, 0xCD, sizeof(dest));
    surf.width = 2;
    surf.height = 1;
    surf.pitch = 2 * 4;
    ok(D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 4 * 4, 4, 2),
       "a smaller surface loads");
    bad = 0;
    for (x = 0; x < 2; x++)
        if (dest[x * 4 + 2] < 1 || dest[x * 4 + 2] > 4)
            bad++;
    okEq(bad, 0, "and takes its pixels from inside the source");
    okEq(dest[2 * 4], 0xCD, "writing no more than the surface holds");

    /* A surface that will not say how big it is cannot be loaded into
       at all, which is better than guessing. */
    memset(&surf, 0, sizeof(surf));
    surf.lpVtbl = &wsVtbl;
    surf.bits = dest;
    surf.width = 4;
    surf.height = 2;
    surf.pitch = 4 * 4;
    surf.failDesc = 1;
    ok(!D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 16, 4, 2),
       "a surface with no description is refused");
    okEq(surf.lockCalls, 0, "and is not even locked");

    /* A surface that says it has no size is refused too, rather than
       loaded with nothing and reported as a success. */
    memset(&surf, 0, sizeof(surf));
    surf.lpVtbl = &wsVtbl;
    surf.bits = dest;
    surf.pitch = 4 * 4;
    ok(!D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 16, 4, 2),
       "a surface of no size is refused");
    okEq(surf.lockCalls, 0, "and is not locked either");

    /* And the mapping stays inside the source for every shape, which
       is the property the two clamps in there are a backstop for.
       They cannot fire with this mapping -- the arithmetic says so,
       and removing either one changes nothing -- so the property is
       asserted here rather than left to a branch no test can reach. */
    {
        int dw, dh, sw, sh;

        bad = 0;
        for (sw = 1; sw <= 9; sw++)
            for (sh = 1; sh <= 9; sh++)
                for (dw = 1; dw <= 9; dw++)
                    for (dh = 1; dh <= 9; dh++)
                    {
                        int sx = ((dw - 1) * 2 + 1) * sw / (dw * 2);
                        int sy = ((dh - 1) * 2 + 1) * sh / (dh * 2);

                        if (sx < 0 || sx >= sw || sy < 0 || sy >= sh)
                            bad++;
                    }
        okEq(bad, 0, "the stretch's last pixel is inside the source for"
                     " every shape up to nine by nine");
    }
}

static void caseSurfaceLoad(void)
{
    tWtSurface     surf;
    unsigned char  src[4 * 4 * 3];       /* 4 wide, 3 tall, tight */
    unsigned char  dest[(4 * 4 + 20) * 3];
    int            x, y, c;
    int            bad = 0;

    printf("-- loading a surface from memory\n");

    for (y = 0; y < 3; y++)
        for (x = 0; x < 4; x++)
            for (c = 0; c < 4; c++)
                src[(y * 4 + x) * 4 + c] = (unsigned char)(y * 64 + x * 16 + c);

    memset(dest, 0xCD, sizeof(dest));
    memset(&surf, 0, sizeof(surf));
    surf.lpVtbl = &wsVtbl;
    surf.bits = dest;
    surf.pitch = 4 * 4 + 20;     /* padded, so a pitch-blind copy shows */
    surf.width = 4;
    surf.height = 3;

    ok(D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 4 * 4, 4, 3),
       "it reports success");
    okEq(surf.lockCalls, 1, "locked once");
    okEq(surf.unlockCalls, 1, "and unlocked once");

    for (y = 0; y < 3; y++)
        for (x = 0; x < 4; x++)
            for (c = 0; c < 4; c++)
                if (dest[y * surf.pitch + x * 4 + c] != src[(y * 4 + x) * 4 + c])
                    bad++;
    okEq(bad, 0, "every pixel arrived, every channel");

    /* the padding between rows is not written over */
    bad = 0;
    for (y = 0; y < 3; y++)
        for (x = 4 * 4; x < surf.pitch; x++)
            if (dest[y * surf.pitch + x] != 0xCD)
                bad++;
    okEq(bad, 0, "and the pitch padding is left alone");

    /* refusals */
    surf.failLock = 1;
    ok(!D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 16, 4, 3),
       "a surface that will not lock is a failure");
    okEq(surf.unlockCalls, 1, "and is not unlocked");
    surf.failLock = 0;
    ok(!D3D_b_SurfaceLoadFromMemory(NULL, src, 16, 4, 3), "no surface, no copy");
    ok(!D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, NULL, 16, 4, 3),
       "no source, no copy");
    ok(!D3D_b_SurfaceLoadFromMemory((IDirect3DSurface9*)&surf, src, 16, 0, 3),
       "nothing to copy is not a copy");
}

static void caseUniform(void)
{
    tWtTexture tex;
    int        level;
    int        bad = 0;

    printf("-- a uniform texture stays uniform\n");
    WT_Build(&tex, 8, 8);
    okEq(tex.levels, 4, "8x8 is four levels");

    {
        int x, y;

        for (y = 0; y < 8; y++)
            for (x = 0; x < 8; x++)
                WT_Set(&tex, 0, x, y, 10, 20, 30, 40);
    }

    ok(D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex), "it reports success");

    for (level = 1; level < tex.levels; level++)
    {
        int x, y;

        for (y = 0; y < tex.level[level].height; y++)
            for (x = 0; x < tex.level[level].width; x++)
            {
                unsigned char* p = WT_Pixel(&tex, level, x, y);

                if (p[0] != 10 || p[1] != 20 || p[2] != 30 || p[3] != 40)
                    bad++;
            }
    }
    okEq(bad, 0, "every level is the same colour, every channel");

    /* nothing is left locked, and the locks are balanced */
    bad = 0;
    for (level = 0; level < tex.levels; level++)
        if (tex.locked[level] != 0)
            bad++;
    okEq(bad, 0, "no level is left locked");
    okEq(tex.lockCalls, tex.unlockCalls, "every lock has its unlock");
    WT_Free(&tex);
}

static void caseAverages(void)
{
    tWtTexture     tex;
    unsigned char* p;

    printf("-- the average of a 2x2 block\n");
    WT_Build(&tex, 2, 2);
    okEq(tex.levels, 2, "2x2 is two levels");

    /* 0, 1, 2, 3 averages to 1.5, which rounds to 2 */
    WT_Set(&tex, 0, 0, 0, 0, 0, 0, 0);
    WT_Set(&tex, 0, 1, 0, 1, 1, 1, 1);
    WT_Set(&tex, 0, 0, 1, 2, 2, 2, 2);
    WT_Set(&tex, 0, 1, 1, 3, 3, 3, 3);
    D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex);
    p = WT_Pixel(&tex, 1, 0, 0);
    okEq(p[0], 2, "0 1 2 3 rounds to 2 rather than truncating to 1");

    /* a channel at a time, so a filter that averages the wrong bytes
       together shows: each channel has its own values */
    WT_Set(&tex, 0, 0, 0, 0,   10,  100, 200);
    WT_Set(&tex, 0, 1, 0, 4,   14,  104, 204);
    WT_Set(&tex, 0, 0, 1, 8,   18,  108, 208);
    WT_Set(&tex, 0, 1, 1, 12,  22,  112, 212);
    D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex);
    p = WT_Pixel(&tex, 1, 0, 0);
    okEq(p[0], 6, "blue averages on its own");
    okEq(p[1], 16, "green too");
    okEq(p[2], 106, "and red");
    okEq(p[3], 206, "and alpha");

    /* the extremes do not wrap: 255s stay 255 */
    WT_Set(&tex, 0, 0, 0, 255, 255, 255, 255);
    WT_Set(&tex, 0, 1, 0, 255, 255, 255, 255);
    WT_Set(&tex, 0, 0, 1, 255, 255, 255, 255);
    WT_Set(&tex, 0, 1, 1, 255, 255, 255, 255);
    D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex);
    p = WT_Pixel(&tex, 1, 0, 0);
    okEq(p[0], 255, "four 255s average to 255, not to zero");
    okEq(p[3], 255, "in alpha as well");
    WT_Free(&tex);
}

static void casePosition(void)
{
    tWtTexture tex;
    int        x, y;
    int        bad = 0;

    printf("-- each destination pixel comes from its own block\n");
    WT_Build(&tex, 4, 4);

    /* a gradient whose every pixel differs, so a filter reading the
       wrong block, or transposing x and y, cannot score */
    for (y = 0; y < 4; y++)
        for (x = 0; x < 4; x++)
            WT_Set(&tex, 0, x, y, y * 4 + x, 0, 0, 255);

    D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex);

    for (y = 0; y < 2; y++)
        for (x = 0; x < 2; x++)
        {
            int a = (2 * y) * 4 + 2 * x;
            int b = (2 * y) * 4 + 2 * x + 1;
            int c = (2 * y + 1) * 4 + 2 * x;
            int d = (2 * y + 1) * 4 + 2 * x + 1;
            int want = (a + b + c + d + 2) / 4;

            if (WT_Pixel(&tex, 1, x, y)[0] != want)
            {
                printf("    at %d,%d got %d want %d\n", x, y,
                       WT_Pixel(&tex, 1, x, y)[0], want);
                bad++;
            }
        }
    okEq(bad, 0, "all four of level 1's pixels average their own block");

    /* and it is not symmetric: a transposed read would give the same
       answer on a symmetric gradient, so check the gradient is not */
    ok(WT_Pixel(&tex, 1, 0, 1)[0] != WT_Pixel(&tex, 1, 1, 0)[0],
       "the two off-diagonal pixels differ, so x and y are told apart");
    WT_Free(&tex);
}

static void caseOdd(void)
{
    tWtTexture     tex;
    unsigned char* p;

    printf("-- an odd dimension\n");

    /* 3x1 halves to 1x1: the last column has no partner, and must be
       averaged with itself rather than read past the end */
    memset(&tex, 0, sizeof(tex));
    tex.lpVtbl = &wtVtbl;
    tex.format = D3DFMT_A8R8G8B8;
    tex.failLockOfLevel = -1;
    tex.failDescOfLevel = -1;
    tex.levels = 2;
    tex.level[0].width = 3; tex.level[0].height = 1;
    tex.level[0].pitch = 3 * 4 + 8;
    tex.level[0].bits = (unsigned char*)calloc((size_t)tex.level[0].pitch, 1);
    tex.level[1].width = 1; tex.level[1].height = 1;
    tex.level[1].pitch = 4 + 8;
    tex.level[1].bits = (unsigned char*)calloc((size_t)tex.level[1].pitch, 1);

    WT_Set(&tex, 0, 0, 0, 100, 0, 0, 0);
    WT_Set(&tex, 0, 1, 0, 200, 0, 0, 0);
    WT_Set(&tex, 0, 2, 0, 1, 0, 0, 0);

    ok(D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex), "it succeeds");
    p = WT_Pixel(&tex, 1, 0, 0);
    /* the 1x1 destination reads columns 0 and 1, both rows clamped to
       row 0: (100 + 200 + 100 + 200 + 2) / 4 = 150 */
    okEq(p[0], 150, "the surviving pixel averages the block it covers");
    WT_Free(&tex);
}

static void caseNarrow(void)
{
    tWtTexture     tex;
    unsigned char* p;
    int            y;

    printf("-- a texture one pixel wide\n");

    /* 1x4 halves to 1x2 and then 1x1. The width is already 1, so
       every destination pixel's second column would be off the end of
       its row -- which is the only shape where the clamp on x
       matters, because a dimension that halves to the floor never
       otherwise leaves a column without a partner. Removing that
       clamp scored clean against a 3x1 case, which is why this one
       exists.

       The row padding is filled, so reading past the row gives a
       visibly wrong average rather than averaging with zeroes that
       might pass for an answer. */
    WT_Build(&tex, 1, 4);
    okEq(tex.levels, 3, "1x4 is three levels");
    for (y = 0; y < 4; y++)
    {
        memset(tex.level[0].bits + (size_t)y * tex.level[0].pitch, 0xFF,
               (size_t)tex.level[0].pitch);
        WT_Set(&tex, 0, 0, y, 40, 40, 40, 40);
    }

    ok(D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex), "it succeeds");
    p = WT_Pixel(&tex, 1, 0, 0);
    okEq(p[0], 40, "level 1 stays 40 rather than drifting towards the padding");
    p = WT_Pixel(&tex, 2, 0, 0);
    okEq(p[0], 40, "and so does level 2");
    WT_Free(&tex);

    /* and the same the other way round, for the clamp on y */
    WT_Build(&tex, 4, 1);
    okEq(tex.levels, 3, "4x1 is three levels");
    memset(tex.level[0].bits, 0xFF, (size_t)tex.level[0].pitch);
    {
        int x;

        for (x = 0; x < 4; x++)
            WT_Set(&tex, 0, x, 0, 40, 40, 40, 40);
    }
    ok(D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex), "it succeeds");
    okEq(WT_Pixel(&tex, 1, 0, 0)[0], 40, "level 1 stays 40 here too");
    okEq(WT_Pixel(&tex, 2, 0, 0)[0], 40, "and level 2");
    WT_Free(&tex);
}

static void caseRefusals(void)
{
    tWtTexture tex;

    printf("-- what it declines to do\n");

    ok(!D3D_b_TextureBuildMipLevels(NULL), "no texture is a failure");

    /* one level: nothing to do, and that is success, not failure */
    WT_Build(&tex, 1, 1);
    okEq(tex.levels, 1, "1x1 is one level");
    ok(D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex),
       "a single level is success with nothing done");
    okEq(tex.lockCalls, 0, "and nothing was locked");
    WT_Free(&tex);

    /* a format this file cannot average */
    WT_Build(&tex, 4, 4);
    tex.format = D3DFMT_R5G6B5;
    ok(!D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex),
       "a 16-bit format is declined");
    okEq(tex.lockCalls, 0, "without touching it");
    WT_Free(&tex);

    /* a lock that fails partway leaves nothing locked behind */
    WT_Build(&tex, 8, 8);
    tex.failLockOfLevel = 2;
    ok(!D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex),
       "a level that will not lock is a failure");
    {
        int level, bad = 0;

        for (level = 0; level < tex.levels; level++)
            if (tex.locked[level] != 0)
                bad++;
        okEq(bad, 0, "and nothing is left locked");
    }
    WT_Free(&tex);

    /* and a description that fails */
    WT_Build(&tex, 8, 8);
    tex.failDescOfLevel = 1;
    ok(!D3D_b_TextureBuildMipLevels((IDirect3DTexture9*)&tex),
       "a level it cannot describe is a failure");
    WT_Free(&tex);
}

int main(void)
{
    printf("=== renderer texture unit test ===\n");
    caseSurfaceLoad();
    caseSurfaceLoadStretch();
    caseUniform();
    caseAverages();
    casePosition();
    caseOdd();
    caseNarrow();
    caseRefusals();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
