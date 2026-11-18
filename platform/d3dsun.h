#pragma once
#include <d3d9.h>
#include "../common.h"

/*
    The Direct3D side of the sun's occlusion test.

    What the engine wants to know is whether the sun can be seen at a
    handful of screen positions. How that is found out is entirely a
    renderer's business, and in Direct3D 9 it is an asynchronous read
    of the frame that was just drawn: the alpha at each position is
    stretched into a 16x1 render target, copied into a lockable
    surface, and picked up a frame or two later when the hardware has
    finished with it. Three of those surfaces are kept in a ring so
    that a sample can be queued every frame without ever waiting.

    None of which the engine needs to know. It projects the sun,
    clips the positions to the viewport, asks for a sample, and takes
    the answer whenever one is ready.

    The structs stay where d3drender.c can see them, because the device
    reset paths there create and release the texture.
*/

typedef struct sGfxSunD3DSpecificDataInner
{
    IDirect3DSurface9* d3dppSurface;
    int bitMask;
    float alphaThreshold;
} tGfxSunD3DSpecificDataInner;

typedef struct sGfxSunD3DSpecificData
{
    int ptrElemSize;
    int maxElemNum;
    tGfxSunD3DSpecificDataInner* currPtrInner;
    tGfxSunD3DSpecificDataInner* firstPtrInner;
    tGfxSunD3DSpecificDataInner* ptr;
    tGfxSunD3DSpecificDataInner elems[3];
    IDirect3DTexture9* d3dTexture;
    IDirect3DSurface9* d3dRenderTarget;

    /* The slot that was current before this frame's sample was
       queued, which is where the search for a finished one starts. */
    tGfxSunD3DSpecificDataInner* pendingPtrInner;
} tGfxSunD3DSpecificData;

/* Where a sampled position lands in the 16x1 target. */
typedef struct sD3dStrectRectUnkStruct
{
    int x;
    int y;
} tD3dStrectRectUnkStruct;

/* ------------------------------------------- the engine's sun interface */

/* One sampler per sun. NULL if there is no device to make it with. */
void* D3D_p_SunSamplerCreate(void);
void  D3D_SunSamplerDestroy(void* sampler);

/* Whether the sampler has its target yet. Until it has, there is
   nothing to sample and nothing to turn on. */
int D3D_b_SunSamplerReady(void* sampler);

/* Whether the renderer is in a state where it samples occlusion at
   all this frame. Separate from the above because the two mean
   different things and the engine does different things about them. */
int D3D_b_SunSamplingNow(void* sampler);

/* Queue a sample of the frame just drawn. `x` and `y` are the `num`
   positions that were inside the viewport, packed; `pointMask` says
   which of the engine's nine they are. `cameraZ` is the sun's depth
   in camera space, from which the renderer works out what counts as
   occluded. Does nothing when no slot is free. */
void D3D_SunSampleQueue(void* sampler, const int* x, const int* y, int num,
                        int pointMask, float cameraZ);

/* The newest queued sample the renderer has finished with, if any.
   Non-zero when one was ready, with a nine-bit mask of the positions
   that are blocked in *outBlocked -- a set bit means something is in
   front of that position, and a position outside the viewport counts
   as blocked because it cannot light anything. Call after
   queueing: the search starts from what was pending before. */
int D3D_b_SunSampleRead(void* sampler, int* outBlocked);

/* The sun effect on or off for this sampler. The renderer keeps the
   tally across every sun and rebuilds what it needs to when the
   answer changes. */
void D3D_SunEffectOnOff(void* sampler, int on);
