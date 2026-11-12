#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../memory.h"
#include "gfx_mocks.h"
#include "../gfx/gfxlightning.h"

void BASE_GFX_Lightning_SampleZero(tGfxLightningData* bolt);
void NEW_GFX_Lightning_SampleZero(tGfxLightningData* bolt);
struct sGfxLightningData* BASE_GFX_Lightning_Create(void);
struct sGfxLightningData* NEW_GFX_Lightning_Create(void);
void BASE_GFX_Lightning_Destroy(struct sGfxLightningData* bolt);
void NEW_GFX_Lightning_Destroy(struct sGfxLightningData* bolt);
void BASE_GFX_AllocPattern(tGfxLightningData* bolt);
void NEW_GFX_AllocPattern(tGfxLightningData* bolt);
void BASE_GFX_Lightning_SampleFractal(struct sGfxLightningData* bolt);
void NEW_GFX_Lightning_SampleFractal(struct sGfxLightningData* bolt);
int BASE_GFX_i_Lightning_Render(struct sGfxLightningData* bolt);
int NEW_GFX_i_Lightning_Render(struct sGfxLightningData* bolt);

#define MAXPOINTS 64

static unsigned s_rngState;

static void rngSeed(unsigned seed)
{
    s_rngState = seed ? seed : 1u;
}

static float rngFloat(float lo, float hi)
{
    s_rngState = s_rngState * 1664525u + 1013904223u;
    return lo + (hi - lo) * (float)((s_rngState >> 8) & 0xFFFF) / 65535.0f;
}

static void boltFill(tGfxLightningData* bolt, Vec3* points, int num, int allocd,
                     int flags, float life, float timeStart, float deathTime,
                     unsigned seed)
{
    int i;

    rngSeed(seed);
    memset(bolt, 0, sizeof(*bolt));
    bolt->v1.x = -10.0f;
    bolt->v1.y = -8.0f;
    bolt->v1.z = 3.0f;
    bolt->v2.x = 12.0f;
    bolt->v2.y = 9.0f;
    bolt->v2.z = 4.0f;
    bolt->v3.x = 0.0f;
    bolt->v3.y = 0.0f;
    bolt->v3.z = 1.0f;
    bolt->f = 0.35f;
    bolt->sampleFractalFloat = 2.0f;
    bolt->timeElapsed = 0.0f;
    bolt->f3 = life;
    bolt->timeStart = timeStart;
    bolt->deathTime = deathTime;
    bolt->pointsNum = num;
    bolt->flags = flags;
    bolt->pointsNumAllocd = allocd;
    bolt->vertexColor = (int)0xFF88CCEEu;
    bolt->points = points;

    memset(points, 0, sizeof(Vec3) * MAXPOINTS);
    for (i = 0; i < allocd && i < MAXPOINTS; i++)
    {
        points[i].x = rngFloat(-3.2f, 3.2f);
        points[i].y = rngFloat(-1.0f, 1.0f);
        points[i].z = (float)(i + 1) / (float)(allocd + 1);
    }
}

static int boltCompare(tGfxLightningData* a, tGfxLightningData* b,
                       Vec3* pa, Vec3* pb)
{
    char what[64];
    int ok = 1;
    int i;

    ok &= GFXMOCK_SameInt(a->pointsNum, b->pointsNum, "bolt.pointsNum", 0);
    ok &= GFXMOCK_SameInt(a->pointsNumAllocd, b->pointsNumAllocd, "bolt.pointsNumAllocd", 0);
    ok &= GFXMOCK_SameInt(a->flags, b->flags, "bolt.flags", 0);
    ok &= GFXMOCK_SameInt(a->vertexColor, b->vertexColor, "bolt.vertexColor", 0);
    ok &= GFXMOCK_Close(a->timeElapsed, b->timeElapsed, "bolt.timeElapsed", 0);
    ok &= GFXMOCK_Close(a->f, b->f, "bolt.f", 0);
    ok &= GFXMOCK_Close(a->f3, b->f3, "bolt.f3", 0);
    ok &= GFXMOCK_CloseVec(&a->v1, &b->v1, "bolt.v1");
    ok &= GFXMOCK_CloseVec(&a->v2, &b->v2, "bolt.v2");
    ok &= GFXMOCK_CloseVec(&a->v3, &b->v3, "bolt.v3");
    for (i = 0; i < MAXPOINTS; i++)
    {
        snprintf(what, sizeof(what), "point%d", i);
        ok &= GFXMOCK_CloseVec(&pa[i], &pb[i], what);
    }
    return ok;
}

/* The render decrements a raster counter on its two culled exits, so
   that counter is part of what a case compares. */
static float g_rasterBase;
static float g_rasterNew;

static void sampleZeroCases(void)
{
    static Vec3 pa[MAXPOINTS];
    static Vec3 pb[MAXPOINTS];
    tGfxLightningData a;
    tGfxLightningData b;
    char name[96];
    int nums[7] = { 0, 1, 3, 7, 15, 31, 63 };
    int ni;

    for (ni = 0; ni < 7; ni++)
    {
        snprintf(name, sizeof(name), "SampleZero num=%d", nums[ni]);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        boltFill(&a, pa, nums[ni], nums[ni], 0, 1.0f, 1e30f, 1e30f, 0x100u + (unsigned)ni);
        BASE_GFX_Lightning_SampleZero(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        boltFill(&b, pb, nums[ni], nums[ni], 0, 1.0f, 1e30f, 1e30f, 0x100u + (unsigned)ni);
        NEW_GFX_Lightning_SampleZero(&b);
        GFXMOCK_SaveSide(1);

        if (!boltCompare(&a, &b, pa, pb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* pointsNumAllocd zero is the early return. */
    GFXMOCK_SetCase("SampleZero unallocated");
    GFXMOCK_Reset();
    boltFill(&a, pa, 15, 0, 0, 1.0f, 1e30f, 1e30f, 0x111u);
    BASE_GFX_Lightning_SampleZero(&a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    boltFill(&b, pb, 15, 0, 0, 1.0f, 1e30f, 1e30f, 0x111u);
    NEW_GFX_Lightning_SampleZero(&b);
    GFXMOCK_SaveSide(1);
    if (!boltCompare(&a, &b, pa, pb))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();
}

/* Every interesting path through the count snap and the two spreads:
   growing, shrinking, first allocation and the zero case. */
static void allocPatternCases(void)
{
    static Vec3 pa[MAXPOINTS];
    static Vec3 pb[MAXPOINTS];
    tGfxLightningData a;
    tGfxLightningData b;
    char name[96];
    int wanted[12] = { 0, 1, 2, 3, 4, 5, 9, 11, 15, 20, 31, 48 };
    int have[5] = { 0, 1, 3, 7, 15 };
    int wi, hi;

    for (wi = 0; wi < 12; wi++)
        for (hi = 0; hi < 5; hi++)
        {
            snprintf(name, sizeof(name), "AllocPattern want=%d have=%d",
                     wanted[wi], have[hi]);
            GFXMOCK_SetCase(name);

            GFXMOCK_Reset();
            boltFill(&a, pa, wanted[wi], have[hi], 0, 1.0f, 1e30f, 1e30f, 0x200u);
            /* The real caller only reaches AllocPattern through the
               render, which keeps the buffer the effect owns; here the
               buffer is the driver's, so the allocating paths get a
               copy they may hand back to the allocator. */
            if (have[hi])
            {
                a.points = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * MAXPOINTS);
                memcpy(a.points, pa, sizeof(Vec3) * MAXPOINTS);
            }
            else
                a.points = NULL;
            BASE_GFX_AllocPattern(&a);
            if (a.pointsNumAllocd > 0)
                memcpy(pa, a.points, sizeof(Vec3) * (a.pointsNumAllocd < MAXPOINTS
                                                     ? a.pointsNumAllocd : MAXPOINTS));
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            boltFill(&b, pb, wanted[wi], have[hi], 0, 1.0f, 1e30f, 1e30f, 0x200u);
            if (have[hi])
            {
                b.points = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * MAXPOINTS);
                memcpy(b.points, pb, sizeof(Vec3) * MAXPOINTS);
            }
            else
                b.points = NULL;
            NEW_GFX_AllocPattern(&b);
            if (b.pointsNumAllocd > 0)
                memcpy(pb, b.points, sizeof(Vec3) * (b.pointsNumAllocd < MAXPOINTS
                                                     ? b.pointsNumAllocd : MAXPOINTS));
            GFXMOCK_SaveSide(1);

            a.points = pa;
            b.points = pb;
            if (!boltCompare(&a, &b, pa, pb))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }
}

static void sampleFractalCases(void)
{
    static Vec3 pa[MAXPOINTS];
    static Vec3 pb[MAXPOINTS];
    tGfxLightningData a;
    tGfxLightningData b;
    char name[96];
    int nums[5] = { 1, 3, 7, 15, 31 };
    float amps[3] = { 0.0f, 2.0f, 7.5f };
    int ni, ai, pass;

    for (ni = 0; ni < 5; ni++)
        for (ai = 0; ai < 3; ai++)
        {
            snprintf(name, sizeof(name), "SampleFractal num=%d amp=%g",
                     nums[ni], amps[ai]);
            GFXMOCK_SetCase(name);

            GFXMOCK_Reset();
            boltFill(&a, pa, nums[ni], nums[ni], 0, 1.0f, 1e30f, 1e30f, 0x300u);
            a.sampleFractalFloat = amps[ai];
            BASE_GFX_Lightning_SampleFractal(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            boltFill(&b, pb, nums[ni], nums[ni], 0, 1.0f, 1e30f, 1e30f, 0x300u);
            b.sampleFractalFloat = amps[ai];
            NEW_GFX_Lightning_SampleFractal(&b);
            GFXMOCK_SaveSide(1);

            if (!boltCompare(&a, &b, pa, pb))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* Repeated passes, because each one reads back what the last wrote. */
    for (pass = 1; pass <= 5; pass++)
    {
        snprintf(name, sizeof(name), "SampleFractal x%d", pass);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        boltFill(&a, pa, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x301u);
        for (ni = 0; ni < pass; ni++)
            BASE_GFX_Lightning_SampleFractal(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        boltFill(&b, pb, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x301u);
        for (ni = 0; ni < pass; ni++)
            NEW_GFX_Lightning_SampleFractal(&b);
        GFXMOCK_SaveSide(1);

        if (!boltCompare(&a, &b, pa, pb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void renderCases(void)
{
    static Vec3 pa[MAXPOINTS];
    static Vec3 pb[MAXPOINTS];
    tGfxLightningData a;
    tGfxLightningData b;
    char name[96];
    int nums[4] = { 1, 3, 15, 31 };
    int flagSet[2] = { 0, 2 };
    float lives[4] = { 0.0f, 0.3f, 1.0f, 100.0f };
    int ni, fi, li, ri, running, dist, frame;
    int rbase, rnew;

    for (ni = 0; ni < 4; ni++)
        for (fi = 0; fi < 2; fi++)
            for (li = 0; li < 4; li++)
            {
                snprintf(name, sizeof(name), "Render num=%d flags=%d life=%g",
                         nums[ni], flagSet[fi], lives[li]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = 0.1f;
                ENG_gb_EngineRunning = 1;

                GFXMOCK_Reset();
                boltFill(&a, pa, nums[ni], nums[ni], flagSet[fi], lives[li],
                         1e30f, 1e30f, 0x400u);
                rbase = BASE_GFX_i_Lightning_Render(&a);
                g_rasterBase = gpst_DX_stDD->rasters[14].unk2;
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                boltFill(&b, pb, nums[ni], nums[ni], flagSet[fi], lives[li],
                         1e30f, 1e30f, 0x400u);
                rnew = NEW_GFX_i_Lightning_Render(&b);
                g_rasterNew = gpst_DX_stDD->rasters[14].unk2;
                GFXMOCK_SaveSide(1);

                if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
                    || !GFXMOCK_Close(g_rasterBase, g_rasterNew, "raster14.unk2", 0)
                    || !boltCompare(&a, &b, pa, pb))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* The fade-in and fade-out windows, which pick a sample along the
       pattern and blend one quad. */
    for (ri = 0; ri < 6; ri++)
    {
        float starts[6] = { 0.0f, 0.05f, 0.2f, 0.5f, 0.9f, 1.5f };

        snprintf(name, sizeof(name), "Render window start=%g", starts[ri]);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.12f;
        ENG_gb_EngineRunning = 1;

        GFXMOCK_Reset();
        boltFill(&a, pa, 15, 15, 0, 1.0f, starts[ri], 2.0f, 0x401u);
        rbase = BASE_GFX_i_Lightning_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        boltFill(&b, pb, 15, 15, 0, 1.0f, starts[ri], 2.0f, 0x401u);
        rnew = NEW_GFX_i_Lightning_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
            || !boltCompare(&a, &b, pa, pb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* Past the death time, the engine stopped, and distance zero: the
       three early returns. */
    for (running = 0; running < 2; running++)
        for (dist = 0; dist < 2; dist++)
        {
            snprintf(name, sizeof(name), "Render running=%d distance=%d",
                     running, dist);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.1f;

            GFXMOCK_Reset();
            ENG_gb_EngineRunning = running;
            GFXMOCK_SetDistance(dist);
            boltFill(&a, pa, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x402u);
            rbase = BASE_GFX_i_Lightning_Render(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            ENG_gb_EngineRunning = running;
            GFXMOCK_SetDistance(dist);
            boltFill(&b, pb, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x402u);
            rnew = NEW_GFX_i_Lightning_Render(&b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
                || !boltCompare(&a, &b, pa, pb))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }
    GFXMOCK_SetDistance(1);
    ENG_gb_EngineRunning = 1;

    GFXMOCK_SetCase("Render past death");
    TIM_gf_dt = 1.0f;
    GFXMOCK_Reset();
    boltFill(&a, pa, 15, 15, 0, 1.0f, 0.1f, 0.5f, 0x403u);
    rbase = BASE_GFX_i_Lightning_Render(&a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    boltFill(&b, pb, 15, 15, 0, 1.0f, 0.1f, 0.5f, 0x403u);
    rnew = NEW_GFX_i_Lightning_Render(&b);
    GFXMOCK_SaveSide(1);
    if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
        || !boltCompare(&a, &b, pa, pb))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    /* Behind the camera and off to one side: the two culled exits that
       touch the raster counter. */
    {
        float zs[3] = { -50.0f, -1.0f, 5.0f };
        int zi;

        for (zi = 0; zi < 3; zi++)
        {
            snprintf(name, sizeof(name), "Render camera z=%g", zs[zi]);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.1f;

            GFXMOCK_Reset();
            boltFill(&a, pa, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x404u);
            a.v1.z = zs[zi];
            a.v2.z = zs[zi];
            rbase = BASE_GFX_i_Lightning_Render(&a);
            g_rasterBase = gpst_DX_stDD->rasters[14].unk2;
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            boltFill(&b, pb, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x404u);
            b.v1.z = zs[zi];
            b.v2.z = zs[zi];
            rnew = NEW_GFX_i_Lightning_Render(&b);
            g_rasterNew = gpst_DX_stDD->rasters[14].unk2;
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
                || !GFXMOCK_Close(g_rasterBase, g_rasterNew, "raster14.unk2", 0)
                || !boltCompare(&a, &b, pa, pb))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }
    }

    /* A degenerate bolt: start and end in the same place, so the width
       cross product is null and the fallback runs. */
    GFXMOCK_SetCase("Render degenerate axis");
    TIM_gf_dt = 0.1f;
    GFXMOCK_Reset();
    boltFill(&a, pa, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x405u);
    a.v2 = a.v1;
    rbase = BASE_GFX_i_Lightning_Render(&a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    boltFill(&b, pb, 15, 15, 0, 1.0f, 1e30f, 1e30f, 0x405u);
    b.v2 = b.v1;
    rnew = NEW_GFX_i_Lightning_Render(&b);
    GFXMOCK_SaveSide(1);
    if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
        || !boltCompare(&a, &b, pa, pb))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    /* Several frames in a row, which is what moves the pattern. */
    for (frame = 1; frame <= 5; frame++)
    {
        snprintf(name, sizeof(name), "Render frame chain x%d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.08f;
        ENG_gb_EngineRunning = 1;

        GFXMOCK_Reset();
        boltFill(&a, pa, 15, 15, 2, 2.0f, 1.0f, 4.0f, 0x406u);
        for (ni = 0; ni < frame; ni++)
            BASE_GFX_i_Lightning_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        boltFill(&b, pb, 15, 15, 2, 2.0f, 1.0f, 4.0f, 0x406u);
        for (ni = 0; ni < frame; ni++)
            NEW_GFX_i_Lightning_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!boltCompare(&a, &b, pa, pb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void createDestroyCases(void)
{
    tGfxLightningData* a;
    tGfxLightningData* b;

    GFXMOCK_SetCase("Create and Destroy");

    GFXMOCK_Reset();
    a = BASE_GFX_Lightning_Create();
    BASE_GFX_AllocPattern(a);
    BASE_GFX_Lightning_Destroy(a);
    BASE_GFX_Lightning_Destroy(a);
    GFXMOCK_SaveSide(0);

    GFXMOCK_Reset();
    b = NEW_GFX_Lightning_Create();
    NEW_GFX_AllocPattern(b);
    NEW_GFX_Lightning_Destroy(b);
    NEW_GFX_Lightning_Destroy(b);
    GFXMOCK_SaveSide(1);

    if (!GFXMOCK_SameInt(a->pointsNum, b->pointsNum, "bolt.pointsNum", 0)
        || !GFXMOCK_SameInt(a->pointsNumAllocd, b->pointsNumAllocd, "bolt.pointsNumAllocd", 0)
        || !GFXMOCK_Close(a->f, b->f, "bolt.f", 0)
        || !GFXMOCK_Close(a->timeStart, b->timeStart, "bolt.timeStart", 0)
        || !GFXMOCK_Close(a->deathTime, b->deathTime, "bolt.deathTime", 0))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    GFXMOCK_Init();

    sampleZeroCases();
    allocPatternCases();
    sampleFractalCases();
    renderCases();
    createDestroyCases();

    GFXMOCK_Summary("gfxlightning");
    return GFXMOCK_fails ? 1 : 0;
}
