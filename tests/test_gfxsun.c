/* A unit test for the engine's half of gfx/gfxsun.c.
 *
 * gfxsun.c had no harness of any kind. tests/baseline_gfxsun.c is a
 * frozen copy of it that nothing has ever driven, and it was frozen
 * before the renderer split, so it cannot be compared across that
 * change. This is a direct check of the half that stayed behind
 * instead.
 *
 * What is worth checking is the bookkeeping, because it fails
 * silently. alphaThresholdFlags carries this frame's nine answers in
 * its low bits and last frame's in its high half, so that a frame
 * with no fresh answer can show the previous one; getting the shifts
 * or the masks wrong makes the sun flicker rather than crash.
 *
 * The renderer is a set of recording stand-ins, so the cases can say
 * "no answer this frame" without there being hardware to be slow.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gfx/gfxsun.h"
#include "../gdi.h"
#include "../soft.h"

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

/* --------------------------------------------------- the engine around it */

tZlistStruct* SOFT_gpc_BigBufferZlists;

void GFX_NeedGeom(unsigned int a, unsigned int b, unsigned int c, int d)
{
    (void)a; (void)b; (void)c; (void)d;
}

void GFX_RenderGeom(unsigned int mask) { (void)mask; }

unsigned int LIGHT_ul_Interpol2Colors(int color1, int color2, float factor)
{
    (void)color2; (void)factor;
    return (unsigned int)color1;
}

void* MEM_p_Alloc(unsigned int size) { return calloc(1, size); }

/* math.c's own allocator calls, which nothing here reaches but the
   linker still wants. */
void* MEM_p_AllocAligned(int size, int align)
{
    (void)align;
    return calloc(1, (size_t)size);
}

void* MEM_p_ReallocAligned(void* p, unsigned int size, int align)
{
    (void)align;
    return realloc(p, size);
}

void MEM_FreeAlign(void* p) { free(p); }

/* Where the projection says things are. The first call of a frame is
   the sun itself; the nine that follow are the ring walking out from
   it, so the two are answered separately. */
static Vec3 sunScreen;
static Vec3 ringScreen[9];
static int  ringCalls;

void SOFT_TransformAndProject(Vec3* dst, Vec3* src, int num, tDDCamStruct* cam)
{
    (void)src; (void)num; (void)cam;
    *dst = sunScreen;
}

void SOFT_Project(Vec3* dst, Vec3* src, int num, tDDContainerStruct* cam)
{
    (void)src; (void)num; (void)cam;
    if (ringCalls < 9)
        *dst = ringScreen[ringCalls];
    ringCalls++;
}

/* ------------------------------------------------ the renderer, recorded */

static struct {
    int   createCalls;
    int   destroyCalls;
    int   ready;
    int   samplingNow;
    int   queueCalls;
    int   queuedNum;
    int   queuedMask;
    float queuedZ;
    int   queuedX[9];
    int   queuedY[9];
    int   readReturns;
    int   readPassed;
    int   readCalls;
    int   effectOn;
    int   effectCalls;
    void* sampler;
} r;

static void* Fake_SamplerCreate(void)
{
    r.createCalls++;
    r.sampler = (void*)0x51;
    return r.sampler;
}

static void Fake_SamplerDestroy(void* s) { (void)s; r.destroyCalls++; }
static int  Fake_SamplerReady(void* s) { (void)s; return r.ready; }
static int  Fake_SamplingNow(void* s) { (void)s; return r.samplingNow; }

static void Fake_SampleQueue(void* s, const int* x, const int* y, int num,
                             int mask, float z)
{
    int i;

    (void)s;
    r.queueCalls++;
    r.queuedNum = num;
    r.queuedMask = mask;
    r.queuedZ = z;
    for (i = 0; i < num && i < 9; i++)
    {
        r.queuedX[i] = x[i];
        r.queuedY[i] = y[i];
    }
}

static int Fake_SampleRead(void* s, int* outPassed)
{
    (void)s;
    r.readCalls++;
    if (!r.readReturns)
        return 0;
    if (outPassed)
        *outPassed = r.readPassed;
    return 1;
}

static int noFlare;
static int Fake_WantsNoFlare(void) { return noFlare; }

static void Fake_EffectOnOff(void* s, int on)
{
    (void)s;
    r.effectCalls++;
    r.effectOn = on;
}

static tGdi_DisplayData dd;
static Mtx4x4 viewMtx;

#include "../gfx/gfxsun.c"

/* ------------------------------------------------------------- the driver */

static void Reset(void)
{
    memset(&r, 0, sizeof(r));
    memset(&dd, 0, sizeof(dd));
    noFlare = 0;
    ringCalls = 0;
    sunUnkCnt = 0;

    dd.p_SunSamplerCreate = Fake_SamplerCreate;
    dd.SunSamplerDestroy = Fake_SamplerDestroy;
    dd.b_SunSamplerReady = Fake_SamplerReady;
    dd.b_SunSamplingNow = Fake_SamplingNow;
    dd.SunSampleQueue = Fake_SampleQueue;
    dd.b_SunSampleRead = Fake_SampleRead;
    dd.SunEffectOnOff = Fake_EffectOnOff;
    dd.b_WantsNoFlare = Fake_WantsNoFlare;

    /* a viewport the whole ring lands inside unless a case says
       otherwise */
    dd.winWidth = 800;
    dd.winHeight = 600;
    dd.ct.cam.viewPortRealPosX = 0;
    dd.ct.cam.viewPortRealPosY = 0;
    dd.ct.cam.width = 800;
    dd.ct.cam.height = 600;

    /* the engine walks the sun into camera space through this, so it
       has to be a matrix and not a null pointer */
    MATH_SetIdentityMatrix(&viewMtx);
    dd.ct.cam.viewMtxPtr_setInGRORender = &viewMtx;

    gpst_DX_stDD = &dd;

    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));

    sunScreen.x = 400.0f;
    sunScreen.y = 300.0f;
    sunScreen.z = 1.0f;
    {
        int i;

        for (i = 0; i < 9; i++)
        {
            ringScreen[i].x = 400.0f + (float)i;
            ringScreen[i].y = 300.0f;
            ringScreen[i].z = 1.0f;
        }
    }

    r.ready = 1;
    r.samplingNow = 1;
}

static tGfxSunData* MakeSun(void)
{
    tGfxSunData* sun = (tGfxSunData*)GFX_Sun_Create();

    ringCalls = 0;
    return sun;
}

static int Update(tGfxSunData* sun)
{
    ringCalls = 0;
    return GFX_Sun_UpdateRayLum(sun);
}

/* ------------------------------------------------------------- the checks */

static void theSamplerIsTheRenderersToMake(void)
{
    tGfxSunData* sun;

    Reset();
    sun = MakeSun();
    okEq(r.createCalls, 1, "creating a sun asks the renderer for a sampler");
    ok(sun->sampler == r.sampler, "and keeps what it was given");

    GFX_Sun_Destroy(sun);
    okEq(r.destroyCalls, 1, "destroying it hands the sampler back");
    ok(sun->sampler == NULL, "and lets go of it");

    /* and does not hand it back twice */
    GFX_Sun_Destroy(sun);
    okEq(r.destroyCalls, 1, "a second destroy hands nothing back");

    GFX_Sun_Destroy(NULL);
    okEq(r.destroyCalls, 1, "and nor does destroying nothing");
}

/* The hidden path: the sun is not to be drawn, every one of the nine
   counts as blocked, and the effect goes off. */
static void whenTheSunIsHidden(void)
{
    tGfxSunData* sun;

    /* the no-flare tweak */
    Reset();
    sun = MakeSun();
    sun->alphaThresholdFlags = 0;
    sun->flags = 0;
    noFlare = 1;
    Update(sun);
    okEq(sun->alphaThresholdFlags & 0x1FF, 0x1FF, "all nine count as blocked");
    ok((sun->flags & 0x1000000u) != 0, "and the sun is marked hidden");
    okEq(r.effectOn, 0, "with the effect turned off");
    okEq(r.queueCalls, 0, "and nothing sampled");

    /* behind the camera */
    Reset();
    sun = MakeSun();
    sun->alphaThresholdFlags = 0;
    sunScreen.z = -1.0f;
    Update(sun);
    okEq(sun->alphaThresholdFlags & 0x1FF, 0x1FF,
         "a sun behind the camera is hidden too");
    okEq(r.effectOn, 0, "and its effect off");

    /* the sun's own flag */
    Reset();
    sun = MakeSun();
    sun->alphaThresholdFlags = 0;
    sun->b1 = 1;
    Update(sun);
    okEq(sun->alphaThresholdFlags & 0x1FF, 0x1FF, "and so is one switched off");
    okEq(r.effectOn, 0, "with its effect off");
}

/* With no answer to be had, last frame's is shown again: it lives in
   the high half and is copied back down. */
static void lastFramesAnswerIsReused(void)
{
    tGfxSunData* sun;

    /* the renderer has no sampler ready yet */
    Reset();
    sun = MakeSun();
    r.ready = 0;
    sun->alphaThresholdFlags = 0x015 << 16;
    Update(sun);
    okEq(sun->alphaThresholdFlags & 0xFFFF, 0x015,
         "with no sampler ready, last frame's answer is copied down");
    okEq(r.effectCalls, 0, "and nothing is said about the effect");
    okEq(r.queueCalls, 0, "and nothing sampled");

    /* the renderer is not sampling this frame */
    Reset();
    sun = MakeSun();
    r.samplingNow = 0;
    sun->alphaThresholdFlags = 0x0AA;
    Update(sun);
    okEq(sun->alphaThresholdFlags & 0xFFFF, 0x0AA,
         "and when the renderer is not sampling, this frame's is kept");
    okEq(r.effectOn, 1, "though the effect is still turned on");
    ok((sun->flags & 0x1000000u) == 0, "and the sun is not marked hidden");
    okEq(r.queueCalls, 0, "with nothing sampled");

    /* a sample was queued but nothing was ready */
    Reset();
    sun = MakeSun();
    r.readReturns = 0;
    sun->alphaThresholdFlags = 0x100;
    Update(sun);
    okEq(r.queueCalls, 1, "a sample is queued");
    okEq(r.readCalls, 1, "and an answer asked for");
    okEq(sun->alphaThresholdFlags & 0xFFFF, 0x100,
         "and with none ready, last frame's is copied down");
}

static void anAnswerFromTheRenderer(void)
{
    tGfxSunData* sun;

    Reset();
    sun = MakeSun();
    r.readReturns = 1;
    r.readPassed = 0x123;
    sun->alphaThresholdFlags = 0x0AA;        /* last frame's */
    Update(sun);

    okEq(sun->alphaThresholdFlags & 0x1FF, 0x123,
         "the renderer's answer becomes this frame's nine bits");
    okEq((sun->alphaThresholdFlags >> 16) & 0xFFFF, 0x0AA,
         "and the frame before is kept in the high half");
    okEq(r.effectOn, 1, "the effect is on");
    ok((sun->flags & 0x1000000u) == 0, "and the sun is not hidden");

    /* only the nine bits are taken from the answer, whatever else the
       renderer sets in it */
    Reset();
    sun = MakeSun();
    r.readReturns = 1;
    r.readPassed = -1;
    sun->alphaThresholdFlags = 0;
    Update(sun);
    okEq(sun->alphaThresholdFlags & 0xFFFF, 0x1FF,
         "nothing above the nine bits leaks out of the answer");
}

/* The ring of positions is clipped to the viewport: the ones inside
   are handed over packed, with a mask saying which of the nine they
   were. */
static void theRingIsClippedToTheViewport(void)
{
    tGfxSunData* sun;
    int i;

    Reset();
    sun = MakeSun();

    /* positions 3 and 6 fall off the right edge */
    ringScreen[3].x = 10000.0f;
    ringScreen[6].x = 10000.0f;
    Update(sun);

    okEq(r.queuedNum, 7, "seven of the nine were on screen");
    okEq(r.queuedMask, 0x1FF & ~((1 << 3) | (1 << 6)),
         "and the mask says which seven");

    /* above and below count too */
    Reset();
    sun = MakeSun();
    ringScreen[0].y = -5.0f;
    ringScreen[8].y = 10000.0f;
    Update(sun);
    okEq(r.queuedNum, 7, "off the top and the bottom are off screen as well");
    okEq(r.queuedMask, 0x1FF & ~((1 << 0) | (1 << 8)), "and named in the mask");

    /* every one off screen is a sample of nothing, still queued so the
       renderer can answer with all nine unblocked */
    Reset();
    sun = MakeSun();
    for (i = 0; i < 9; i++)
        ringScreen[i].x = 10000.0f;
    Update(sun);
    okEq(r.queuedNum, 0, "none on screen, nothing to sample");
    okEq(r.queuedMask, 0, "and an empty mask");
    okEq(r.queueCalls, 1, "but the frame is still queued");

    /* the viewport, not the window, is what clips */
    Reset();
    sun = MakeSun();
    dd.ct.cam.width = 100;          /* so x beyond 100 is outside */
    for (i = 0; i < 9; i++)
        ringScreen[i].x = 50.0f + (float)(i * 20);
    Update(sun);
    okEq(r.queuedNum, 3, "a narrower viewport clips more of the ring");
}

/* The positions handed over are the ones that were on screen, in the
   order they were found -- which is what the renderer reads back. */
static void thePositionsArePackedInOrder(void)
{
    tGfxSunData* sun;

    Reset();
    sun = MakeSun();
    ringScreen[0].x = 10000.0f;      /* off screen */
    ringScreen[1].x = 111.0f;
    ringScreen[1].y = 222.0f;
    ringScreen[2].x = 333.0f;
    ringScreen[2].y = 444.0f;
    Update(sun);

    okEq(r.queuedX[0], 111, "the first position handed over is the first on screen");
    okEq(r.queuedY[0], 222, "with its y");
    okEq(r.queuedX[1], 333, "and the next is the next");
    okEq(r.queuedY[1], 444, "with its y");
}

int main(void)
{
    printf("=== sun, engine side, unit test ===\n");
    theSamplerIsTheRenderersToMake();
    whenTheSunIsHidden();
    lastFramesAnswerIsReused();
    anAnswerFromTheRenderer();
    theRingIsClippedToTheViewport();
    thePositionsArePackedInOrder();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
