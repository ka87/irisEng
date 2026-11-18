/* A unit test for platform/d3dsun.c, the Direct3D side of the sun's
 * occlusion test.
 *
 * New code with no earlier copy, so this is a direct check rather
 * than a differential one.
 *
 * What is worth testing here is the asynchrony. The point of three
 * surfaces in a ring is that a sample can be queued every frame
 * without ever waiting for one, so the cases drive the device saying
 * "still drawing" for a while and then finishing, in orders the real
 * hardware would produce, and check that:
 *
 *   - a sample is never waited for;
 *   - the newest finished one is the one read;
 *   - the ring stops taking samples when nothing has been read, and
 *     starts again once one has;
 *   - a position that was outside the viewport counts as not
 *     occluded, which is what it always did.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The engine's own headers first: a fake below that does not match
   what the engine declares is then a compile error rather than a
   mystery at link time. */
#include "../platform/d3drender.h"
#include "../platform/d3dsun.h"
#include "../memory.h"

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
        printf("  FAIL %s: got %ld (0x%lX) want %ld (0x%lX)\n", what,
               got, (unsigned long)got, want, (unsigned long)want);
        fails++;
    }
}

/* --------------------------------------------------------------- the fakes */

#define NSLOT 3

static int stretchCalls;
static int readbackCalls;
static int blurTextureCalls;
static int releaseCalls;

/* Per slot: whether the hardware has finished, and the alpha values
   the surface would hand back. */
static struct {
    int finished;
    uint32_t pixels[16];
} slotState[NSLOT];

static IDirect3DSurface9     slotSurf[NSLOT];
static IDirect3DSurface9     targetSurf;
static IDirect3DSurface9     frameSurf;
static IDirect3DSurface9Vtbl surfVtbl;
static IDirect3DTexture9     targetTex;
static IDirect3DTexture9Vtbl texVtbl;
static IDirect3DDevice9      dev;
static IDirect3DDevice9Vtbl  devVtbl;

static int SlotOf(IDirect3DSurface9* s)
{
    int i;

    for (i = 0; i < NSLOT; i++)
        if (s == &slotSurf[i])
            return i;
    return -1;
}

static HRESULT Vt_LockRect(IDirect3DSurface9* s, D3DLOCKED_RECT* lr,
                           const RECT* r, DWORD flags)
{
    int slot = SlotOf(s);

    (void)r; (void)flags;
    if (slot < 0)
        return -1;
    if (!slotState[slot].finished)
        return D3DERR_WASSTILLDRAWING;
    lr->pBits = slotState[slot].pixels;
    lr->Pitch = sizeof(slotState[slot].pixels);
    return 0;
}

static HRESULT Vt_UnlockRect(IDirect3DSurface9* s) { (void)s; return 0; }

static HRESULT Vt_GetDesc(IDirect3DSurface9* s, D3DSURFACE_DESC* d)
{
    (void)s;
    memset(d, 0, sizeof(*d));
    d->Width = 16;
    d->Height = 1;
    return 0;
}

static ULONG Vt_SurfRelease(IDirect3DSurface9* s) { (void)s; releaseCalls++; return 0; }
static ULONG Vt_TexRelease(IDirect3DTexture9* t) { (void)t; return 0; }

static int nextSurface;

static HRESULT Vt_CreateOffscreenPlainSurface(IDirect3DDevice9* d, UINT w, UINT h,
                                              D3DFORMAT f, D3DPOOL pool,
                                              IDirect3DSurface9** out, HANDLE* sh)
{
    (void)d; (void)w; (void)h; (void)f; (void)pool; (void)sh;
    if (nextSurface >= NSLOT)
        return -1;
    *out = &slotSurf[nextSurface++];
    return 0;
}

static HRESULT Vt_CreateTexture(IDirect3DDevice9* d, UINT w, UINT h, UINT lv,
                                DWORD usage, D3DFORMAT f, D3DPOOL pool,
                                IDirect3DTexture9** out, HANDLE* sh)
{
    (void)d; (void)w; (void)h; (void)lv; (void)usage; (void)f; (void)pool; (void)sh;
    *out = &targetTex;
    return 0;
}

static HRESULT Vt_GetSurfaceLevel(IDirect3DTexture9* t, UINT lv,
                                  IDirect3DSurface9** out)
{
    (void)t; (void)lv;
    *out = &targetSurf;
    return 0;
}

static HRESULT Vt_GetRenderTarget(IDirect3DDevice9* d, DWORD idx,
                                  IDirect3DSurface9** out)
{
    (void)d; (void)idx;
    *out = &frameSurf;
    return 0;
}

static HRESULT Vt_StretchRect(IDirect3DDevice9* d, IDirect3DSurface9* src,
                              const RECT* sr, IDirect3DSurface9* dst,
                              const RECT* dr, D3DTEXTUREFILTERTYPE f)
{
    (void)d; (void)src; (void)sr; (void)dst; (void)dr; (void)f;
    stretchCalls++;
    return 0;
}

static HRESULT Vt_GetRenderTargetData(IDirect3DDevice9* d, IDirect3DSurface9* rt,
                                      IDirect3DSurface9* dest)
{
    (void)d; (void)rt; (void)dest;
    readbackCalls++;
    return 0;
}

/* ------------------------------------------------- the engine around it */

/* d3drender.c's, which the sampler leans on rather than reimplementing */
void D3D_CreateSunTextures(tGfxSunD3DSpecificData* sun)
{
    if (!sun->d3dTexture)
    {
        D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
            16, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
            &sun->d3dTexture, NULL);
        sun->d3dTexture->lpVtbl->GetSurfaceLevel(sun->d3dTexture, 0,
                                                 &sun->d3dRenderTarget);
    }
}

void D3D_ReleaseSunRenderTargetAndTexture(tGfxSunD3DSpecificData* sun)
{
    sun->d3dRenderTarget = NULL;
    sun->d3dTexture = NULL;
}

int D3D_UpdateBlurTexture(void) { blurTextureCalls++; return 0; }

/* the depth-blur curve the threshold comes from; a straight pass so a
   case can say what the threshold is */
static float curveIn;
uint32_t D3D_DepthBlurBlurFuncCalc2(float depth)
{
    curveIn = depth;
    return (uint32_t)depth;
}

static void* allocated[8];
static int   allocCount;
static int   freeCount;

void* MEM_p_Alloc(unsigned int size)
{
    void* p = calloc(1, (size_t)size);

    if (allocCount < 8)
        allocated[allocCount++] = p;
    return p;
}

void MEM_Free(void* p) { freeCount++; free(p); }

#include "../platform/d3dsun.c"

/* ------------------------------------------------------------- the driver */

static void Reset(void)
{
    int i;

    stretchCalls = 0;
    readbackCalls = 0;
    blurTextureCalls = 0;
    releaseCalls = 0;
    nextSurface = 0;
    allocCount = 0;
    freeCount = 0;
    curveIn = -1.0f;
    depthblur_Type = 4;

    memset(slotState, 0, sizeof(slotState));
    memset(gD3dSunSurfaces, 0, sizeof(gD3dSunSurfaces));
    memset(d3d_sunEffectOn, 0, sizeof(d3d_sunEffectOn));

    surfVtbl.Release = Vt_SurfRelease;
    surfVtbl.GetDesc = Vt_GetDesc;
    surfVtbl.LockRect = Vt_LockRect;
    surfVtbl.UnlockRect = Vt_UnlockRect;
    for (i = 0; i < NSLOT; i++)
        slotSurf[i].lpVtbl = &surfVtbl;
    targetSurf.lpVtbl = &surfVtbl;
    frameSurf.lpVtbl = &surfVtbl;

    texVtbl.Release = Vt_TexRelease;
    texVtbl.GetSurfaceLevel = Vt_GetSurfaceLevel;
    targetTex.lpVtbl = &texVtbl;

    devVtbl.CreateOffscreenPlainSurface = Vt_CreateOffscreenPlainSurface;
    devVtbl.CreateTexture = Vt_CreateTexture;
    devVtbl.GetRenderTarget = Vt_GetRenderTarget;
    devVtbl.StretchRect = Vt_StretchRect;
    devVtbl.GetRenderTargetData = Vt_GetRenderTargetData;
    dev.lpVtbl = &devVtbl;

    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    D3DRenderParams.d3dDev = &dev;
}

/* nine positions, all inside the viewport */
static const int allX[9] = { 10, 11, 12, 13, 14, 15, 16, 17, 18 };
static const int allY[9] = { 20, 21, 22, 23, 24, 25, 26, 27, 28 };

static void Queue(void* s, int num, int mask, float z)
{
    D3D_SunSampleQueue(s, allX, allY, num, mask, z);
}

/* alpha for every one of the nine positions in a slot */
static void SetSlotAlpha(int slot, int alpha)
{
    int i;

    for (i = 0; i < 16; i++)
        slotState[slot].pixels[i] = ((uint32_t)alpha << 24) | 0x00112233u;
}

/* A sample queued this frame is never read this frame -- it has only
   just been handed to the hardware. The search for a finished one
   starts from whatever was pending before this frame's queue, so a
   sample is read a frame or more after it was asked for. That is the
   whole point of the ring, and it means every case below has to
   drive frames rather than a single call. */
static void aLatentFrame(void* s)
{
    Queue(s, 9, 0x1FF, 1.0f);
}

/* ------------------------------------------------------------- the checks */

static void theOneFrameLatency(void)
{
    void* s;
    int passed;

    Reset();
    s = D3D_p_SunSamplerCreate();

    Queue(s, 9, 0x1FF, 1.0f);
    slotState[0].finished = 1;
    SetSlotAlpha(0, 255);
    okEq(D3D_b_SunSampleRead(s, &passed), 0,
         "a sample queued this frame is not read this frame, finished or not");

    /* the next frame's queue is what makes it readable */
    Queue(s, 9, 0x1FF, 1.0f);
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "the next frame reads it");
    okEq(passed, 0x1FF, "with its answer");

    D3D_SunSamplerDestroy(s);
}

static void makingAndEnding(void)
{
    void* s;

    Reset();
    s = D3D_p_SunSamplerCreate();
    ok(s != NULL, "a sampler is made");
    ok(gD3dSunSurfaces[0] == s, "and registered among the suns");
    okEq(allocCount, 1, "with one allocation");
    ok(D3D_b_SunSamplerReady(s) == 1, "and it has its target");

    D3D_SunSamplerDestroy(s);
    okEq(releaseCalls, NSLOT, "destroying releases every slot surface");
    okEq(freeCount, 1, "and frees the sampler");
    ok(gD3dSunSurfaces[0] == NULL, "and takes it out of the suns");

    /* no device, no sampler */
    Reset();
    D3DRenderParams.d3dDev = NULL;
    ok(D3D_p_SunSamplerCreate() == NULL, "no device makes no sampler");
    okEq(allocCount, 0, "and allocates nothing");

    /* and destroying nothing is not a crash */
    D3D_SunSamplerDestroy(NULL);
    okEq(freeCount, 0, "destroying nothing frees nothing");
}

static void theEffectTally(void)
{
    void* a;
    void* b;

    Reset();
    a = D3D_p_SunSamplerCreate();
    b = D3D_p_SunSamplerCreate();
    ok(a && b, "two suns");

    blurTextureCalls = 0;
    D3D_SunEffectOnOff(a, 1);
    okEq(D3DRenderParams.sunEffectIsOn, 1, "one sun on turns the effect on");
    okEq(blurTextureCalls, 1, "and rebuilds the blur texture once");

    D3D_SunEffectOnOff(b, 1);
    okEq(blurTextureCalls, 1, "a second sun on changes nothing");

    D3D_SunEffectOnOff(a, 0);
    okEq(D3DRenderParams.sunEffectIsOn, 1,
         "and one of two off leaves the effect on");
    okEq(blurTextureCalls, 1, "with nothing rebuilt");

    D3D_SunEffectOnOff(b, 0);
    okEq(D3DRenderParams.sunEffectIsOn, 0, "both off turns it off");
    okEq(blurTextureCalls, 2, "and rebuilds once more");

    D3D_SunSamplerDestroy(a);
    D3D_SunSamplerDestroy(b);
}

static void samplingWhenNotSampling(void)
{
    void* s;

    Reset();
    s = D3D_p_SunSamplerCreate();

    depthblur_Type = 4;
    ok(D3D_b_SunSamplingNow(s) == 1, "the depth blur mode that writes alpha samples");
    depthblur_Type = 3;
    ok(D3D_b_SunSamplingNow(s) == 0, "and the others do not");
    depthblur_Type = 0;
    ok(D3D_b_SunSamplingNow(s) == 0, "including none at all");

    /* with no target there is nothing to sample */
    D3D_ReleaseSunRenderTargetAndTexture((tGfxSunD3DSpecificData*)s);
    ok(D3D_b_SunSamplerReady(s) == 0, "no target, not ready");
    stretchCalls = 0;
    Queue(s, 9, 0x1FF, 1.0f);
    okEq(stretchCalls, 0, "and queueing does nothing");

    ok(D3D_b_SunSamplerReady(NULL) == 0, "no sampler is never ready");
    D3D_SunSamplerDestroy(s);
}

static void aSampleQueuedAndRead(void)
{
    void* s;
    int passed;

    Reset();
    s = D3D_p_SunSamplerCreate();

    /* nothing has been queued, so there is nothing to read */
    passed = -1;
    okEq(D3D_b_SunSampleRead(s, &passed), 0, "nothing queued, nothing to read");

    Queue(s, 9, 0x1FF, 12.5f);
    okEq(stretchCalls, 9, "nine positions, nine pixels moved");
    okEq(readbackCalls, 1, "and one readback queued");
    ok(curveIn == 12.5f, "the threshold comes from the depth handed in");

    /* the hardware has not finished: no answer, and no waiting */
    aLatentFrame(s);
    slotState[0].finished = 0;
    okEq(D3D_b_SunSampleRead(s, &passed), 0, "an unfinished sample is not read");

    /* it finishes, with alpha above the threshold everywhere */
    slotState[0].finished = 1;
    SetSlotAlpha(0, 200);
    passed = 0;
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "a finished sample is read");
    okEq(passed, 0x1FF, "alpha above the threshold is not occluded");

    /* and one whose alpha is below its threshold. Slot 1 holds the
       frame the latent call queued, whose threshold came from a depth
       of 1.0, so zero alpha is below it. */
    aLatentFrame(s);
    slotState[1].finished = 1;
    SetSlotAlpha(1, 0);
    slotState[2].finished = 0;
    passed = 0x1FF;
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "the next one too");
    okEq(passed, 0, "alpha below the threshold is occluded");

    D3D_SunSamplerDestroy(s);
}

/* A position outside the viewport was never sampled, and counts as
   not occluded -- which is what the engine did with it before. */
static void positionsOutsideTheViewport(void)
{
    void* s;
    int passed;

    Reset();
    s = D3D_p_SunSamplerCreate();

    /* only positions 0, 2 and 4 were on screen, and all three are
       occluded; the other six were never looked at */
    Queue(s, 3, 0x015, 100.0f);
    okEq(stretchCalls, 3, "only the positions on screen are moved");
    aLatentFrame(s);
    slotState[0].finished = 1;
    SetSlotAlpha(0, 1);
    passed = 0;
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "and read a frame later");
    okEq(passed, 0x1FF & ~0x015,
         "the six off screen count as not occluded, the three on screen as occluded");

    D3D_SunSamplerDestroy(s);
}

/* The ring is the whole point: three samples can be in flight, and
   once they are, no more are taken until one has been read. */
static void theRingFillsAndDrains(void)
{
    void* s;
    int passed;
    int i;

    Reset();
    s = D3D_p_SunSamplerCreate();

    for (i = 0; i < NSLOT; i++)
    {
        stretchCalls = 0;
        Queue(s, 9, 0x1FF, 1.0f);
        okEq(stretchCalls, 9, "a slot is free, so the sample is taken");
    }

    /* the ring is full and nothing has finished */
    stretchCalls = 0;
    Queue(s, 9, 0x1FF, 1.0f);
    okEq(stretchCalls, 0, "a full ring takes no more samples");
    okEq(D3D_b_SunSampleRead(s, &passed), 0, "and has nothing finished to read");

    /* the oldest finishes; reading it frees the ring again */
    slotState[0].finished = 1;
    SetSlotAlpha(0, 255);
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "the finished one is read");

    stretchCalls = 0;
    Queue(s, 9, 0x1FF, 1.0f);
    okEq(stretchCalls, 9, "and the ring takes samples again");

    D3D_SunSamplerDestroy(s);
}

/* When more than one has finished, the newest is the one that
   matters -- an older one would show the sun where it was two frames
   ago. */
static void theNewestFinishedIsRead(void)
{
    void* s;
    int passed;

    Reset();
    s = D3D_p_SunSamplerCreate();

    Queue(s, 9, 0x1FF, 1.0f);      /* slot 0 */
    Queue(s, 9, 0x1FF, 1.0f);      /* slot 1 */
    Queue(s, 9, 0x1FF, 1.0f);      /* slot 2, so slot 1 is the pending one */

    /* slots 0 and 1 have both finished, with different answers so
       they can be told apart; slot 2 was only just queued */
    slotState[0].finished = 1;
    SetSlotAlpha(0, 255);          /* not occluded */
    slotState[1].finished = 1;
    SetSlotAlpha(1, 0);            /* occluded */

    passed = -1;
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "a sample is read");
    okEq(passed, 0, "and it is the newer of the two that finished");

    D3D_SunSamplerDestroy(s);
}

/* The newest may not be finished while an older one is, which is the
   case the walk back exists for. */
static void anOlderOneIsReadWhenTheNewestIsNot(void)
{
    void* s;
    int passed;

    Reset();
    s = D3D_p_SunSamplerCreate();

    Queue(s, 9, 0x1FF, 1.0f);      /* slot 0 */
    Queue(s, 9, 0x1FF, 1.0f);      /* slot 1 */
    Queue(s, 9, 0x1FF, 1.0f);      /* slot 2 */

    slotState[2].finished = 0;
    slotState[1].finished = 0;
    slotState[0].finished = 1;
    SetSlotAlpha(0, 255);

    passed = 0;
    okEq(D3D_b_SunSampleRead(s, &passed), 1,
         "the oldest finished one is found by walking back");
    okEq(passed, 0x1FF, "with its own answer");

    D3D_SunSamplerDestroy(s);
}

/* The sampled positions are packed: only the ones on screen were
   moved, and they went into the target one after another. So the
   pixels are read back by the packed number and not by which of the
   nine they were -- a distinction nothing can see while every
   position carries the same alpha, which is why this case gives each
   one its own. */
static void thePackedPositionsAreReadInOrder(void)
{
    void* s;
    int passed;
    int i;

    Reset();
    s = D3D_p_SunSamplerCreate();

    /* positions 5, 6 and 7 were on screen, so they were moved into
       target pixels 0, 1 and 2 */
    Queue(s, 3, 0x0E0, 100.0f);
    aLatentFrame(s);

    /* target pixel 1 is occluded, 0 and 2 are not; everything past
       the three that were moved is left dark, so reading by the
       unpacked number would find darkness and call it occluded */
    for (i = 0; i < 16; i++)
        slotState[0].pixels[i] = 0x00000000u;
    slotState[0].pixels[0] = 0xFF000000u;
    slotState[0].pixels[1] = 0x01000000u;
    slotState[0].pixels[2] = 0xFF000000u;
    slotState[0].finished = 1;

    passed = 0;
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "the sample is read");

    /* 5 and 7 visible, 6 occluded, and the six off screen not occluded */
    okEq(passed, (0x1FF & ~0x0E0) | (1 << 5) | (1 << 7),
         "the packed pixels line up with the positions that were moved");

    D3D_SunSamplerDestroy(s);
}

/* Reading the only sample in flight empties the ring rather than
   leaving it pointing at itself -- which would refuse every sample
   from then on. */
static void readingTheRingEmpty(void)
{
    void* s;
    int passed;
    int i;

    Reset();
    s = D3D_p_SunSamplerCreate();

    for (i = 0; i < NSLOT; i++)
        Queue(s, 9, 0x1FF, 1.0f);

    /* the ring is full, so this frame's sample is refused and the
       newest in flight is the one pending */
    Queue(s, 9, 0x1FF, 1.0f);

    /* only the newest has finished, so reading it reads the slot the
       ring is currently on */
    for (i = 0; i < NSLOT; i++)
        slotState[i].finished = 0;
    slotState[NSLOT - 1].finished = 1;
    SetSlotAlpha(NSLOT - 1, 255);
    okEq(D3D_b_SunSampleRead(s, &passed), 1, "the newest in flight is read");

    /* and with nothing left in flight the ring is free again */
    for (i = 0; i < NSLOT; i++)
    {
        stretchCalls = 0;
        Queue(s, 9, 0x1FF, 1.0f);
        okEq(stretchCalls, 9, "the emptied ring takes samples again");
    }

    D3D_SunSamplerDestroy(s);
}

static void refusingNonsense(void)
{
    void* s;

    Reset();
    s = D3D_p_SunSamplerCreate();

    stretchCalls = 0;
    Queue(s, -1, 0x1FF, 1.0f);
    okEq(stretchCalls, 0, "a negative count samples nothing");
    Queue(s, 10, 0x1FF, 1.0f);
    okEq(stretchCalls, 0, "and so does one past the nine positions");

    D3D_SunSampleQueue(NULL, allX, allY, 9, 0x1FF, 1.0f);
    okEq(stretchCalls, 0, "and no sampler samples nothing");
    okEq(D3D_b_SunSampleRead(NULL, NULL), 0, "nor reads anything");

    /* a read with nowhere to put the answer is still a read */
    Queue(s, 9, 0x1FF, 1.0f);
    aLatentFrame(s);
    slotState[0].finished = 1;
    SetSlotAlpha(0, 255);
    okEq(D3D_b_SunSampleRead(s, NULL), 1, "a read with no out parameter works");

    D3D_SunSamplerDestroy(s);
}

int main(void)
{
    printf("=== sun occlusion sampler unit test ===\n");
    makingAndEnding();
    theOneFrameLatency();
    theEffectTally();
    samplingWhenNotSampling();
    aSampleQueuedAndRead();
    positionsOutsideTheViewport();
    theRingFillsAndDrains();
    theNewestFinishedIsRead();
    anOlderOneIsReadWhenTheNewestIsNot();
    thePackedPositionsAreReadInOrder();
    readingTheRingEmpty();
    refusingNonsense();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
