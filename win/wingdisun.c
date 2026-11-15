#include "wingdisun.h"
#include "wingdi.h"
#include "../memory.h"

#include <string.h>

/* Where each of the nine positions lands in the 16x1 target. */
static const tD3dStrectRectUnkStruct d3d_sunDestPos[9] = {
    { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 },
    { 5, 0 }, { 6, 0 }, { 7, 0 }, { 8, 0 }
};

/* Which suns exist and which of them want the effect. Parallel
   sixteen-entry tables, so both are walked by index. gD3dSunSurfaces
   is wingdi.c's, because its device reset paths walk it too. */
static int d3d_sunEffectOn[16];

/* ------------------------------------------------------------- the ring */

static tGfxSunD3DSpecificDataInner* D3D_l_pst_SlotNext(
    tGfxSunD3DSpecificData* s, tGfxSunD3DSpecificDataInner* elem)
{
    unsigned int index;

    index = (unsigned int)((char*)elem - (char*)s->ptr) / s->ptrElemSize;
    return (tGfxSunD3DSpecificDataInner*)
           ((char*)s->ptr + s->ptrElemSize * ((index + 1) % s->maxElemNum));
}

static tGfxSunD3DSpecificDataInner* D3D_l_pst_SlotPrev(
    tGfxSunD3DSpecificData* s, tGfxSunD3DSpecificDataInner* elem)
{
    unsigned int index;

    index = (unsigned int)((char*)elem - (char*)s->ptr) / s->ptrElemSize;
    return (tGfxSunD3DSpecificDataInner*)
           ((char*)s->ptr + s->ptrElemSize * ((index - 1) % s->maxElemNum));
}

/* Takes the next slot, or nothing when the ring has caught up with
   its own oldest unread sample. */
static tGfxSunD3DSpecificDataInner* D3D_l_pst_SlotTake(tGfxSunD3DSpecificData* s)
{
    tGfxSunD3DSpecificDataInner* next;

    if (!s->currPtrInner)
    {
        s->firstPtrInner = s->ptr;
        s->currPtrInner = s->ptr;
        return s->ptr;
    }

    next = D3D_l_pst_SlotNext(s, s->currPtrInner);
    if (next == s->firstPtrInner)
        return NULL;

    s->currPtrInner = next;
    return next;
}

/* Everything up to and including `elem` has been read, so the ring
   is free again from the one after it. */
static void D3D_l_SlotsDone(tGfxSunD3DSpecificData* s,
                            tGfxSunD3DSpecificDataInner* elem)
{
    if (elem == s->currPtrInner)
    {
        s->currPtrInner = NULL;
        s->firstPtrInner = NULL;
    }
    else
    {
        s->firstPtrInner = D3D_l_pst_SlotNext(s, elem);
    }
}

/* Has the hardware finished with this slot? A lock that would have
   to wait says no rather than blocking, which is the whole point of
   there being three of them. */
static int D3D_l_b_SlotFinished(tGfxSunD3DSpecificDataInner* slot)
{
    D3DLOCKED_RECT locked;

    if (!slot->d3dppSurface)
        return 0;
    if (slot->d3dppSurface->lpVtbl->LockRect(slot->d3dppSurface, &locked, 0,
                                             0xC010) == D3DERR_WASSTILLDRAWING)
        return 0;
    slot->d3dppSurface->lpVtbl->UnlockRect(slot->d3dppSurface);
    return 1;
}

/* ------------------------------------------------------- making and ending */

static void D3D_l_SunRegister(tGfxSunD3DSpecificData* s)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (!gD3dSunSurfaces[i])
        {
            gD3dSunSurfaces[i] = s;
            return;
        }
    }
}

void D3D_UnsetSunSurface(tGfxSunD3DSpecificData* sun3d)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i] == sun3d)
        {
            gD3dSunSurfaces[i] = NULL;
            return;
        }
    }
}

void* D3D_p_SunSamplerCreate(void)
{
    tGfxSunD3DSpecificData* s;
    int i;

    if (!D3DRenderParams.d3dDev)
        return NULL;

    s = (tGfxSunD3DSpecificData*)MEM_p_Alloc(sizeof(*s));
    if (!s)
        return NULL;
    memset(s, 0, sizeof(*s));

    s->ptr = s->elems;
    s->maxElemNum = 3;
    s->ptrElemSize = sizeof(s->elems) / 3;
    s->currPtrInner = NULL;
    s->firstPtrInner = NULL;

    for (i = 0; i < 3; i++)
    {
        s->elems[i].bitMask = 0;
        D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(
            D3DRenderParams.d3dDev, 16, 1, 21, 2, &s->elems[i].d3dppSurface, 0);
    }

    D3D_l_SunRegister(s);
    D3D_CreateSunTextures(s);
    D3D_SunEffectOnOff(s, 0);
    return s;
}

void D3D_SunSamplerDestroy(void* sampler)
{
    tGfxSunD3DSpecificData* s = (tGfxSunD3DSpecificData*)sampler;
    int i;

    if (!s)
        return;

    D3D_SunEffectOnOff(s, 0);
    D3D_ReleaseSunRenderTargetAndTexture(s);
    D3D_UnsetSunSurface(s);
    for (i = 0; i < 3; i++)
    {
        if (s->elems[i].d3dppSurface)
        {
            s->elems[i].d3dppSurface->lpVtbl->Release(s->elems[i].d3dppSurface);
            s->elems[i].d3dppSurface = NULL;
        }
    }
    MEM_Free(s);
}

/* ------------------------------------------------------------- the effect */

void D3D_SunEffectOnOff(void* sampler, int on)
{
    tGfxSunD3DSpecificData* s = (tGfxSunD3DSpecificData*)sampler;
    int anyOn;
    int i;

    anyOn = 0;
    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i] == s)
            d3d_sunEffectOn[i] = on;
        if (gD3dSunSurfaces[i])
            anyOn |= d3d_sunEffectOn[i];
    }

    if ((anyOn != 0) == (D3DRenderParams.sunEffectIsOn != 0))
        return;

    D3DRenderParams.sunEffectIsOn = anyOn;
    D3D_UpdateBlurTexture();
}

/* ------------------------------------------------------------ sampling */

int D3D_b_SunSamplerReady(void* sampler)
{
    tGfxSunD3DSpecificData* s = (tGfxSunD3DSpecificData*)sampler;

    return s && s->d3dRenderTarget != NULL;
}

int D3D_b_SunSamplingNow(void* sampler)
{
    (void)sampler;
    /* The depth blur writes the alpha this test reads, so there is
       only one of its modes in which there is anything to read. */
    return depthblur_Type == 4;
}

/* One source pixel to one destination pixel, which is a stretch of
   nothing but is how a single pixel is moved between surfaces. */
static int D3D_l_SunCopyPixel(IDirect3DSurface9* src, int srcX, int srcY,
                              IDirect3DSurface9* dest, int destX, int destY)
{
    RECT srcRect;
    RECT destRect;

    srcRect.left = srcX;
    srcRect.top = srcY;
    srcRect.right = srcX + 1;
    srcRect.bottom = srcY + 1;
    destRect.left = destX;
    destRect.top = destY;
    destRect.right = destX + 1;
    destRect.bottom = destY + 1;
    return D3DRenderParams.d3dDev->lpVtbl->StretchRect(D3DRenderParams.d3dDev,
                                                       src, &srcRect,
                                                       dest, &destRect, 0);
}

void D3D_SunSampleQueue(void* sampler, const int* x, const int* y, int num,
                        int pointMask, float cameraZ)
{
    tGfxSunD3DSpecificData* s = (tGfxSunD3DSpecificData*)sampler;
    IDirect3DSurface9* renderTarget;
    tGfxSunD3DSpecificDataInner* slot;
    int i;

    if (!s || !s->d3dRenderTarget || !D3DRenderParams.d3dDev)
        return;
    if (num < 0 || num > 9)
        return;

    /* Where the search for a finished sample will start, taken before
       the ring moves on. */
    s->pendingPtrInner = s->currPtrInner;

    slot = D3D_l_pst_SlotTake(s);
    if (!slot)
        return;        /* nothing read yet; this frame goes unsampled */

    slot->bitMask = pointMask;
    slot->alphaThreshold = (float)(uint8_t)D3D_DepthBlurBlurFuncCalc2(cameraZ);

    if (FAILED(D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(
            D3DRenderParams.d3dDev, 0, &renderTarget)))
        return;

    for (i = 0; i < num; i++)
    {
        D3D_l_SunCopyPixel(renderTarget, x[i], y[i], s->d3dRenderTarget,
                           d3d_sunDestPos[i].x, d3d_sunDestPos[i].y);
    }
    renderTarget->lpVtbl->Release(renderTarget);

    D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev,
                                                        s->d3dRenderTarget,
                                                        slot->d3dppSurface);
}

/* The alpha at each sampled position, against what counts as
   occluded for the slot. A position that was outside the viewport
   was never sampled and counts as not occluded. */
static int D3D_l_i_SlotResolve(tGfxSunD3DSpecificDataInner* slot)
{
    D3DLOCKED_RECT locked;
    D3DSURFACE_DESC desc;
    const uint32_t* pixels;
    int passed = 0;
    int sampled = 0;
    int i;

    if (!slot->d3dppSurface)
        return 0x1FF;

    slot->d3dppSurface->lpVtbl->GetDesc(slot->d3dppSurface, &desc);
    if (slot->d3dppSurface->lpVtbl->LockRect(slot->d3dppSurface, &locked, 0,
                                             0xC010) == D3DERR_WASSTILLDRAWING)
        return 0x1FF;

    pixels = (const uint32_t*)locked.pBits;
    for (i = 0; i < 9; i++)
    {
        uint32_t pixel;

        if ((slot->bitMask & (1 << i)) == 0)
        {
            passed |= 1 << i;      /* never sampled, never occluded */
            continue;
        }

        pixel = pixels[d3d_sunDestPos[sampled].x
                       + desc.Width * d3d_sunDestPos[sampled].y];
        if ((double)((pixel >> 24) & 0xff) > slot->alphaThreshold)
            passed |= 1 << i;
        sampled++;
    }

    slot->d3dppSurface->lpVtbl->UnlockRect(slot->d3dppSurface);
    return passed;
}

int D3D_b_SunSampleRead(void* sampler, int* outPassed)
{
    tGfxSunD3DSpecificData* s = (tGfxSunD3DSpecificData*)sampler;
    tGfxSunD3DSpecificDataInner* walk;
    tGfxSunD3DSpecificDataInner* ready = NULL;

    if (!s)
        return 0;

    walk = s->pendingPtrInner;
    if (!walk)
        return 0;

    /* The newest the hardware has finished with, walking back from
       the one that was pending towards the oldest unread. */
    if (D3D_l_b_SlotFinished(walk))
    {
        ready = walk;
    }
    else
    {
        while (walk != s->firstPtrInner)
        {
            walk = D3D_l_pst_SlotPrev(s, walk);
            if (D3D_l_b_SlotFinished(walk))
            {
                ready = walk;
                break;
            }
        }
    }

    if (!ready)
        return 0;

    if (outPassed)
        *outPassed = D3D_l_i_SlotResolve(ready);
    D3D_l_SlotsDone(s, ready);
    return 1;
}
