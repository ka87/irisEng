#include "wingditex.h"

#include <string.h>

/* Loading a surface from a block of 32-bit pixels.
 *
 * What this replaced took a source rectangle and no destination
 * rectangle, which means the whole of the destination surface -- so
 * when the two differ in size it stretches the source to fill it,
 * point sampled. It is not a row copy, and the difference is not
 * academic: the one caller hands over a rectangle of width by height
 * and the surface it hands over may have been created the other way
 * round, because the engine stores a texture's width in its height
 * field and the two creation paths undo that differently.
 *
 * Written as a plain row copy, that wrote width pixels into rows only
 * height pixels wide and ran off the end of every one of them. On a
 * square texture it was right; on anything else it was not, which is
 * why the in-game video screens broke and the fullscreen ones, which
 * do not come through here, did not.
 *
 * The equal-size case is kept as the straight copy it was, so where
 * this was already right it is unchanged. The stretch uses the
 * half-pixel-centre mapping, which is the conventional one; the exact
 * rounding of the original's point filter is not documented and is
 * not something this can check, so a destination pixel that lands on
 * a source boundary may pick the other neighbour. Nothing here
 * depends on which.
 */
int D3D_b_SurfaceLoadFromMemory(IDirect3DSurface9* surface, const void* src,
                                int srcPitch, int width, int height)
{
    D3DSURFACE_DESC desc;
    D3DLOCKED_RECT  locked;
    int             dstW;
    int             dstH;
    int             x;
    int             y;

    if (!surface || !src || width <= 0 || height <= 0)
        return 0;
    if (FAILED(surface->lpVtbl->GetDesc(surface, &desc)))
        return 0;

    dstW = (int)desc.Width;
    dstH = (int)desc.Height;
    if (dstW <= 0 || dstH <= 0)
        return 0;

    if (FAILED(surface->lpVtbl->LockRect(surface, &locked, NULL, 0)))
        return 0;

    if (dstW == width && dstH == height)
    {
        for (y = 0; y < height; y++)
            memcpy((char*)locked.pBits + (size_t)y * locked.Pitch,
                   (const char*)src + (size_t)y * srcPitch,
                   (size_t)width * 4);
    }
    else
    {
        for (y = 0; y < dstH; y++)
        {
            const char* srow;
            char*       drow;
            int         sy;

            sy = ((y * 2 + 1) * height) / (dstH * 2);
            if (sy >= height)
                sy = height - 1;
            srow = (const char*)src + (size_t)sy * srcPitch;
            drow = (char*)locked.pBits + (size_t)y * locked.Pitch;

            for (x = 0; x < dstW; x++)
            {
                int sx;

                sx = ((x * 2 + 1) * width) / (dstW * 2);
                if (sx >= width)
                    sx = width - 1;
                memcpy(drow + (size_t)x * 4, srow + (size_t)sx * 4, 4);
            }
        }
    }

    surface->lpVtbl->UnlockRect(surface);
    return 1;
}

/* Is this a format whose pixels are four bytes the way this file
   assumes? Only the two the engine's upload path produces. */
static int D3D_l_b_Is32Bit(D3DFORMAT f)
{
    return f == D3DFMT_A8R8G8B8 || f == D3DFMT_X8R8G8B8;
}

/* One level from the one above it. The source co-ordinates are
   doubled and clamped, so an odd source dimension averages its last
   row or column with itself instead of reading past the end. */
static void D3D_l_BoxFilter(unsigned char* dst, int dstPitch, int dstW, int dstH,
                            const unsigned char* src, int srcPitch,
                            int srcW, int srcH)
{
    int x, y, c;

    for (y = 0; y < dstH; y++)
    {
        int y0 = y * 2;
        int y1 = y0 + 1 < srcH ? y0 + 1 : y0;

        for (x = 0; x < dstW; x++)
        {
            int x0 = x * 2;
            int x1 = x0 + 1 < srcW ? x0 + 1 : x0;

            const unsigned char* a = src + (size_t)y0 * srcPitch + (size_t)x0 * 4;
            const unsigned char* b = src + (size_t)y0 * srcPitch + (size_t)x1 * 4;
            const unsigned char* d = src + (size_t)y1 * srcPitch + (size_t)x0 * 4;
            const unsigned char* e = src + (size_t)y1 * srcPitch + (size_t)x1 * 4;
            unsigned char*       o = dst + (size_t)y * dstPitch + (size_t)x * 4;

            for (c = 0; c < 4; c++)
                o[c] = (unsigned char)(((unsigned)a[c] + b[c] + d[c] + e[c] + 2) / 4);
        }
    }
}

int D3D_b_TextureBuildMipLevels(IDirect3DTexture9* texture)
{
    D3DSURFACE_DESC up;
    D3DSURFACE_DESC down;
    D3DLOCKED_RECT  lockedUp;
    D3DLOCKED_RECT  lockedDown;
    DWORD           levels;
    DWORD           level;

    if (!texture)
        return 0;
    levels = texture->lpVtbl->GetLevelCount(texture);
    if (levels < 2)
        return 1;   /* nothing to fill in, and not a failure */

    if (FAILED(texture->lpVtbl->GetLevelDesc(texture, 0, &up))
        || !D3D_l_b_Is32Bit(up.Format))
        return 0;

    for (level = 1; level < levels; level++)
    {
        if (FAILED(texture->lpVtbl->GetLevelDesc(texture, level - 1, &up))
            || FAILED(texture->lpVtbl->GetLevelDesc(texture, level, &down)))
            return 0;
        if (FAILED(texture->lpVtbl->LockRect(texture, level - 1, &lockedUp, NULL, 0)))
            return 0;
        if (FAILED(texture->lpVtbl->LockRect(texture, level, &lockedDown, NULL, 0)))
        {
            texture->lpVtbl->UnlockRect(texture, level - 1);
            return 0;
        }

        D3D_l_BoxFilter((unsigned char*)lockedDown.pBits, lockedDown.Pitch,
                        (int)down.Width, (int)down.Height,
                        (const unsigned char*)lockedUp.pBits, lockedUp.Pitch,
                        (int)up.Width, (int)up.Height);

        texture->lpVtbl->UnlockRect(texture, level);
        texture->lpVtbl->UnlockRect(texture, level - 1);
    }
    return 1;
}
