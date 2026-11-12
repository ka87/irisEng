#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../memory.h"
#include "gfx_mocks.h"
#include "../gfx/gfxprofiler.h"

struct sGfxProfilerData* BASE_GFX_Profiler_Create(void);
struct sGfxProfilerData* NEW_GFX_Profiler_Create(void);
void BASE_GFX_Profiler_Destroy(struct sGfxProfilerData* prof);
void NEW_GFX_Profiler_Destroy(struct sGfxProfilerData* prof);
void BASE_GFX_i_Profiler_StartStop(tGfxProfilerData* prof, unsigned int running);
void NEW_GFX_i_Profiler_StartStop(tGfxProfilerData* prof, unsigned int running);
void BASE_GFX_i_Profiler_UpdateMecanic(tGfxProfilerData* prof, float dt);
void NEW_GFX_i_Profiler_UpdateMecanic(tGfxProfilerData* prof, float dt);
void BASE_GFX_i_Profiler_ComputeMeshState(tGfxProfilerData* prof);
void NEW_GFX_i_Profiler_ComputeMeshState(tGfxProfilerData* prof);
void BASE_GFX_i_Profiler_Update_With_XYT(tGfxProfilerData* prof);
void NEW_GFX_i_Profiler_Update_With_XYT(tGfxProfilerData* prof);
void BASE_GFX_Profiler_Validate(tGfxProfilerData* prof);
void NEW_GFX_Profiler_Validate(tGfxProfilerData* prof);
int BASE_GFX_i_Profiler_Render(struct sGfxProfilerData* prof);
int NEW_GFX_i_Profiler_Render(struct sGfxProfilerData* prof);
void BASE_GFX_SetNumberOfPPP(tGfxProfilerData* prof, int num);
void NEW_GFX_SetNumberOfPPP(tGfxProfilerData* prof, int num);
void BASE_GFX_SetNumberOfLPP(tGfxProfilerData* prof, int num);
void NEW_GFX_SetNumberOfLPP(tGfxProfilerData* prof, int num);
void BASE_GFX_SetNumberOfP(tGfxProfilerData* prof, unsigned int num);
void NEW_GFX_SetNumberOfP(tGfxProfilerData* prof, unsigned int num);
void BASE_GFX_Profiler_Seti(struct sGfxProfilerData* prof, int id, int value);
void NEW_GFX_Profiler_Seti(struct sGfxProfilerData* prof, int id, int value);
void BASE_GFX_Profiler_Setf(struct sGfxProfilerData* prof, int id, float value);
void NEW_GFX_Profiler_Setf(struct sGfxProfilerData* prof, int id, float value);
void BASE_GFX_Profiler_Setv(struct sGfxProfilerData* prof, int id, Vec3* value);
void NEW_GFX_Profiler_Setv(struct sGfxProfilerData* prof, int id, Vec3* value);

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

static Vec3 vec3(float x, float y, float z)
{
    Vec3 v;

    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/* One side of a case: an effect built through the public setters, with
   the same values both times, and its slice flags filled in. */
typedef void (*tSetiFn)(struct sGfxProfilerData*, int, int);
typedef void (*tSetfFn)(struct sGfxProfilerData*, int, float);
typedef void (*tSetvFn)(struct sGfxProfilerData*, int, Vec3*);
typedef struct sGfxProfilerData* (*tCreateFn)(void);

static tGfxProfilerData* profBuild(tCreateFn create, tSetiFn seti, tSetfFn setf,
                                   tSetvFn setv, int ppp, int lpp, int slices,
                                   unsigned seed)
{
    tGfxProfilerData* prof;
    Vec3 v;
    int i;

    rngSeed(seed);
    prof = create();
    seti(prof, 12100, ppp);
    seti(prof, 12101, lpp);
    seti(prof, 12102, slices);
    if (!prof->uu)
        return prof;

    setf(prof, 12000, -9.8f);
    setf(prof, 12004, 0.05f);
    v = vec3(0.05f, 0.1f, 0.02f);
    setv(prof, 12202, &v);
    v = vec3(1.0f, 0.0f, 0.0f);
    setv(prof, 12203, &v);
    v = vec3(0.0f, 1.0f, 0.0f);
    setv(prof, 12204, &v);
    v = vec3(0.0f, 0.0f, 1.0f);
    setv(prof, 12205, &v);
    v = vec3(2.0f, -1.0f, 0.5f);
    setv(prof, 12206, &v);
    seti(prof, 12111, 4);

    for (i = 0; i < lpp; i++)
    {
        seti(prof, 12105, i);
        seti(prof, 12106, i % ppp);
        seti(prof, 12107, (i + 1) % ppp);
        seti(prof, 12108, i);
        setf(prof, 12001, rngFloat(0.0f, 1.0f));
        setf(prof, 12002, rngFloat(0.0f, 1.0f));
    }
    for (i = 0; i < ppp; i++)
    {
        seti(prof, 12104, i);
        seti(prof, 12109, (int)(0xFF000000u | (unsigned)(i * 0x3171)));
        seti(prof, 12110, i);
        setf(prof, 12003, rngFloat(-2.0f, 0.0f));
        v = vec3(rngFloat(-1.0f, 1.0f), rngFloat(-1.0f, 1.0f), rngFloat(-1.0f, 1.0f));
        setv(prof, 12200, &v);
        v = vec3(rngFloat(-3.0f, 3.0f), rngFloat(-3.0f, 3.0f), rngFloat(-3.0f, 3.0f));
        setv(prof, 12201, &v);
        v = vec3(rngFloat(0.1f, 2.0f), rngFloat(0.1f, 2.0f), rngFloat(0.1f, 2.0f));
        setv(prof, 12207, &v);
        v = vec3(rngFloat(0.1f, 2.0f), rngFloat(0.1f, 2.0f), rngFloat(0.1f, 2.0f));
        setv(prof, 12208, &v);
    }
    return prof;
}

/* The points carry a gravity vector in ptr2->v4, which no setter
   reaches, so it is written straight in. */
static void profSeedPoints(tGfxProfilerData* prof, int liveMask, unsigned seed)
{
    int total;
    int i;

    rngSeed(seed);
    total = prof->num * prof->PPPNum;
    for (i = 0; i < prof->PPPNum; i++)
        prof->ptr2[i].v4 = vec3(rngFloat(-1.0f, 1.0f), rngFloat(-1.0f, 1.0f),
                                rngFloat(-1.0f, 1.0f));
    for (i = 0; i < total; i++)
    {
        prof->ptr4[i].v1 = vec3(rngFloat(-5.0f, 5.0f), rngFloat(-5.0f, 5.0f),
                                rngFloat(-5.0f, 5.0f));
        prof->ptr4[i].v2 = vec3(rngFloat(-2.0f, 2.0f), rngFloat(-2.0f, 2.0f),
                                rngFloat(-2.0f, 2.0f));
        prof->ptr4[i].v3 = vec3(0.0f, 0.0f, 1.0f);
        prof->ptr4[i].xx = 0xFF000000u | (unsigned)(i * 0x010203);
        prof->ptr4[i].fff1 = rngFloat(-4.0f, 0.0f);
    }
    for (i = 0; i < prof->num; i++)
        prof->ptr3[i] = ((liveMask >> (i % 31)) & 1) ? 1u : 0u;
}

static int profCompare(tGfxProfilerData* a, tGfxProfilerData* b)
{
    char what[64];
    int ok = 1;
    int total;
    int i;

    ok &= GFXMOCK_SameInt(a->uu, b->uu, "prof.uu", 0);
    ok &= GFXMOCK_SameInt(a->PPPNum, b->PPPNum, "prof.PPPNum", 0);
    ok &= GFXMOCK_SameInt(a->LPPNum, b->LPPNum, "prof.LPPNum", 0);
    ok &= GFXMOCK_SameInt(a->num, b->num, "prof.num", 0);
    ok &= GFXMOCK_SameInt(a->num2, b->num2, "prof.num2", 0);
    ok &= GFXMOCK_SameInt((int)a->xx, (int)b->xx, "prof.xx", 0);
    ok &= GFXMOCK_SameInt(a->currentSlice, b->currentSlice, "prof.currentSlice", 0);
    ok &= GFXMOCK_SameInt(a->xxxxx, b->xxxxx, "prof.xxxxx", 0);
    ok &= GFXMOCK_SameInt(a->triangleNum, b->triangleNum, "prof.triangleNum", 0);
    ok &= GFXMOCK_SameInt(a->pointsNum, b->pointsNum, "prof.pointsNum", 0);
    ok &= GFXMOCK_SameInt(a->UVNum, b->UVNum, "prof.UVNum", 0);
    ok &= GFXMOCK_SameInt(a->ptr1CurrID, b->ptr1CurrID, "prof.ptr1CurrID", 0);
    ok &= GFXMOCK_SameInt(a->ptr2CurrID, b->ptr2CurrID, "prof.ptr2CurrID", 0);
    ok &= GFXMOCK_Close(a->f0, b->f0, "prof.f0", 0);
    ok &= GFXMOCK_Close(a->f1, b->f1, "prof.f1", 0);
    if (!ok || !a->uu || !b->uu)
        return ok;

    for (i = 0; i < a->num; i++)
    {
        snprintf(what, sizeof(what), "prof.slice");
        ok &= GFXMOCK_SameInt((int)a->ptr3[i], (int)b->ptr3[i], what, i);
    }
    for (i = 0; i < a->LPPNum; i++)
    {
        snprintf(what, sizeof(what), "line%d.s", i);
        ok &= GFXMOCK_SameInt(a->ptr1[i].s1, b->ptr1[i].s1, what, 1);
        ok &= GFXMOCK_SameInt(a->ptr1[i].s2, b->ptr1[i].s2, what, 2);
        ok &= GFXMOCK_SameInt(a->ptr1[i].s3, b->ptr1[i].s3, what, 3);
        snprintf(what, sizeof(what), "line%d.f1", i);
        ok &= GFXMOCK_Close(a->ptr1[i].f1, b->ptr1[i].f1, what, 0);
        snprintf(what, sizeof(what), "line%d.f2", i);
        ok &= GFXMOCK_Close(a->ptr1[i].f2, b->ptr1[i].f2, what, 0);
    }
    total = a->num * a->PPPNum;
    for (i = 0; i < total; i++)
    {
        snprintf(what, sizeof(what), "point%d.v1", i);
        ok &= GFXMOCK_CloseVec(&a->ptr4[i].v1, &b->ptr4[i].v1, what);
        snprintf(what, sizeof(what), "point%d.v2", i);
        ok &= GFXMOCK_CloseVec(&a->ptr4[i].v2, &b->ptr4[i].v2, what);
        snprintf(what, sizeof(what), "point%d.v3", i);
        ok &= GFXMOCK_CloseVec(&a->ptr4[i].v3, &b->ptr4[i].v3, what);
        snprintf(what, sizeof(what), "point%d.xx", i);
        ok &= GFXMOCK_SameInt(a->ptr4[i].xx, b->ptr4[i].xx, what, 0);
        snprintf(what, sizeof(what), "point%d.fff1", i);
        ok &= GFXMOCK_Close(a->ptr4[i].fff1, b->ptr4[i].fff1, what, 0);
    }
    return ok;
}

static void validateCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;
    char name[96];
    int ppps[4] = { 0, 1, 3, 6 };
    int lpps[3] = { 0, 2, 5 };
    /* A slice count of zero is left out: the engine's own setter
       divides 255 by it, in both copies, so the case says nothing
       about the rewrite and takes the process down. */
    int slices[3] = { 1, 2, 7 };
    int pi, li, si;

    for (pi = 0; pi < 4; pi++)
        for (li = 0; li < 3; li++)
            for (si = 0; si < 3; si++)
            {
                snprintf(name, sizeof(name), "Validate ppp=%d lpp=%d slices=%d",
                         ppps[pi], lpps[li], slices[si]);
                GFXMOCK_SetCase(name);

                GFXMOCK_Reset();
                a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                              BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                              ppps[pi], lpps[li], slices[si], 0x800u);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                              NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                              ppps[pi], lpps[li], slices[si], 0x800u);
                GFXMOCK_SaveSide(1);

                if (!profCompare(a, b))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* The slice-count setter's own clamping. */
    for (si = 1; si <= 300; si += 37)
    {
        snprintf(name, sizeof(name), "SetNumberOfP %d", si);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        a = BASE_GFX_Profiler_Create();
        BASE_GFX_SetNumberOfP(a, (unsigned)si);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = NEW_GFX_Profiler_Create();
        NEW_GFX_SetNumberOfP(b, (unsigned)si);
        GFXMOCK_SaveSide(1);

        if (!profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void mecanicCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;
    char name[96];
    float dts[4] = { 0.0f, 0.016666668f, 0.05f, 0.3f };
    int masks[4] = { 0x00, 0x55, 0x7F, 0x41 };
    Vec3 damps[3];
    int di, mi, ki, pass;

    damps[0] = vec3(0.0f, 0.0f, 0.0f);
    damps[1] = vec3(0.05f, 0.1f, 0.02f);
    damps[2] = vec3(1.0f, 0.5f, 2.0f);

    for (di = 0; di < 4; di++)
        for (mi = 0; mi < 4; mi++)
            for (ki = 0; ki < 3; ki++)
            {
                snprintf(name, sizeof(name), "UpdateMecanic dt=%g mask=0x%X damp=%d",
                         dts[di], masks[mi], ki);
                GFXMOCK_SetCase(name);

                GFXMOCK_Reset();
                a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                              BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                              3, 2, 7, 0x810u);
                a->v1 = damps[ki];
                profSeedPoints(a, masks[mi], 0x811u);
                BASE_GFX_i_Profiler_UpdateMecanic(a, dts[di]);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                              NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                              3, 2, 7, 0x810u);
                b->v1 = damps[ki];
                profSeedPoints(b, masks[mi], 0x811u);
                NEW_GFX_i_Profiler_UpdateMecanic(b, dts[di]);
                GFXMOCK_SaveSide(1);

                if (!profCompare(a, b))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* Repeated passes, where the damping compounds and the fade field
       walks down. */
    for (pass = 1; pass <= 6; pass++)
    {
        snprintf(name, sizeof(name), "UpdateMecanic x%d", pass);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                      BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                      3, 2, 7, 0x812u);
        profSeedPoints(a, 0x7F, 0x813u);
        for (di = 0; di < pass; di++)
            BASE_GFX_i_Profiler_UpdateMecanic(a, 0.02f);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                      NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                      3, 2, 7, 0x812u);
        profSeedPoints(b, 0x7F, 0x813u);
        for (di = 0; di < pass; di++)
            NEW_GFX_i_Profiler_UpdateMecanic(b, 0.02f);
        GFXMOCK_SaveSide(1);

        if (!profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void meshStateCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;
    char name[96];
    int masks[8] = { 0x00, 0x01, 0x02, 0x55, 0x2A, 0x7F, 0x40, 0x7E };
    int mi;

    for (mi = 0; mi < 8; mi++)
    {
        snprintf(name, sizeof(name), "ComputeMeshState mask=0x%X", masks[mi]);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                      BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                      3, 2, 7, 0x820u);
        profSeedPoints(a, masks[mi], 0x821u);
        BASE_GFX_i_Profiler_ComputeMeshState(a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                      NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                      3, 2, 7, 0x820u);
        profSeedPoints(b, masks[mi], 0x821u);
        NEW_GFX_i_Profiler_ComputeMeshState(b);
        GFXMOCK_SaveSide(1);

        if (!profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void xytCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;
    char name[96];
    int masks[5] = { 0x00, 0x01, 0x03, 0x7F, 0x7E };
    float thresholds[3] = { 0.0f, 0.05f, 1000.0f };
    int starts[3] = { 0, 1, 6 };
    int mi, ti, si, pass;

    for (mi = 0; mi < 5; mi++)
        for (ti = 0; ti < 3; ti++)
            for (si = 0; si < 3; si++)
            {
                snprintf(name, sizeof(name),
                         "UpdateWithXYT mask=0x%X threshold=%g slice=%d",
                         masks[mi], thresholds[ti], starts[si]);
                GFXMOCK_SetCase(name);

                GFXMOCK_Reset();
                a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                              BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                              3, 2, 7, 0x830u);
                profSeedPoints(a, masks[mi], 0x831u);
                a->f1 = thresholds[ti];
                a->currentSlice = starts[si];
                BASE_GFX_i_Profiler_Update_With_XYT(a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                              NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                              3, 2, 7, 0x830u);
                profSeedPoints(b, masks[mi], 0x831u);
                b->f1 = thresholds[ti];
                b->currentSlice = starts[si];
                NEW_GFX_i_Profiler_Update_With_XYT(b);
                GFXMOCK_SaveSide(1);

                if (!profCompare(a, b))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* The restart flag, which shifts the slice cursor back one. */
    GFXMOCK_SetCase("UpdateWithXYT restart flag");
    GFXMOCK_Reset();
    a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                  BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv, 3, 2, 7, 0x832u);
    profSeedPoints(a, 0x7F, 0x833u);
    a->xxxxx = 1;
    BASE_GFX_i_Profiler_Update_With_XYT(a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                  NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv, 3, 2, 7, 0x832u);
    profSeedPoints(b, 0x7F, 0x833u);
    b->xxxxx = 1;
    NEW_GFX_i_Profiler_Update_With_XYT(b);
    GFXMOCK_SaveSide(1);
    if (!profCompare(a, b))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    for (pass = 1; pass <= 8; pass++)
    {
        snprintf(name, sizeof(name), "UpdateWithXYT x%d", pass);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                      BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                      3, 2, 7, 0x834u);
        profSeedPoints(a, 0x7F, 0x835u);
        for (mi = 0; mi < pass; mi++)
            BASE_GFX_i_Profiler_Update_With_XYT(a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                      NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                      3, 2, 7, 0x834u);
        profSeedPoints(b, 0x7F, 0x835u);
        for (mi = 0; mi < pass; mi++)
            NEW_GFX_i_Profiler_Update_With_XYT(b);
        GFXMOCK_SaveSide(1);

        if (!profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void startStopCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;
    char name[96];
    int patterns[4] = { 0x0F, 0x55, 0xAA, 0xF0 };
    int pi, i;

    for (pi = 0; pi < 4; pi++)
    {
        snprintf(name, sizeof(name), "StartStop pattern=0x%X", patterns[pi]);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                      BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                      3, 2, 7, 0x840u);
        for (i = 0; i < 10; i++)
            BASE_GFX_i_Profiler_StartStop(a, (unsigned)((patterns[pi] >> (i % 8)) & 1));
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                      NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                      3, 2, 7, 0x840u);
        for (i = 0; i < 10; i++)
            NEW_GFX_i_Profiler_StartStop(b, (unsigned)((patterns[pi] >> (i % 8)) & 1));
        GFXMOCK_SaveSide(1);

        if (!profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void renderCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;
    char name[96];
    int masks[5] = { 0x00, 0x03, 0x55, 0x7F, 0x7E };
    unsigned int steps[3] = { 1, 4, 8 };
    int mi, xi, dist, frame, i;
    int rbase, rnew;

    for (mi = 0; mi < 5; mi++)
        for (xi = 0; xi < 3; xi++)
        {
            snprintf(name, sizeof(name), "Render mask=0x%X steps=%u",
                     masks[mi], steps[xi]);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.02f;

            GFXMOCK_Reset();
            a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                          BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                          3, 2, 7, 0x850u);
            profSeedPoints(a, masks[mi], 0x851u);
            a->xx = steps[xi];
            rbase = BASE_GFX_i_Profiler_Render(a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                          NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                          3, 2, 7, 0x850u);
            profSeedPoints(b, masks[mi], 0x851u);
            b->xx = steps[xi];
            rnew = NEW_GFX_i_Profiler_Render(b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
                || !profCompare(a, b))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* Nothing validated, and distance zero, which keeps the geometry
       but skips the draw. */
    GFXMOCK_SetCase("Render not validated");
    GFXMOCK_Reset();
    a = BASE_GFX_Profiler_Create();
    rbase = BASE_GFX_i_Profiler_Render(a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    b = NEW_GFX_Profiler_Create();
    rnew = NEW_GFX_i_Profiler_Render(b);
    GFXMOCK_SaveSide(1);
    if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0) || !profCompare(a, b))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    for (dist = 0; dist < 2; dist++)
    {
        snprintf(name, sizeof(name), "Render distance=%d", dist);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.02f;

        GFXMOCK_Reset();
        GFXMOCK_SetDistance(dist);
        a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                      BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                      3, 2, 7, 0x852u);
        profSeedPoints(a, 0x7F, 0x853u);
        rbase = BASE_GFX_i_Profiler_Render(a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        GFXMOCK_SetDistance(dist);
        b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                      NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                      3, 2, 7, 0x852u);
        profSeedPoints(b, 0x7F, 0x853u);
        rnew = NEW_GFX_i_Profiler_Render(b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0) || !profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
    GFXMOCK_SetDistance(1);

    for (frame = 1; frame <= 6; frame++)
    {
        snprintf(name, sizeof(name), "Render frame chain x%d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.02f;

        GFXMOCK_Reset();
        a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                      BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv,
                      3, 2, 7, 0x854u);
        profSeedPoints(a, 0x7F, 0x855u);
        for (i = 0; i < frame; i++)
            BASE_GFX_i_Profiler_Render(a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                      NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv,
                      3, 2, 7, 0x854u);
        profSeedPoints(b, 0x7F, 0x855u);
        for (i = 0; i < frame; i++)
            NEW_GFX_i_Profiler_Render(b);
        GFXMOCK_SaveSide(1);

        if (!profCompare(a, b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void destroyCases(void)
{
    tGfxProfilerData* a;
    tGfxProfilerData* b;

    GFXMOCK_SetCase("Destroy");

    GFXMOCK_Reset();
    a = profBuild(BASE_GFX_Profiler_Create, BASE_GFX_Profiler_Seti,
                  BASE_GFX_Profiler_Setf, BASE_GFX_Profiler_Setv, 3, 2, 7, 0x860u);
    BASE_GFX_Profiler_Destroy(a);
    BASE_GFX_Profiler_Destroy(NULL);
    GFXMOCK_SaveSide(0);

    GFXMOCK_Reset();
    b = profBuild(NEW_GFX_Profiler_Create, NEW_GFX_Profiler_Seti,
                  NEW_GFX_Profiler_Setf, NEW_GFX_Profiler_Setv, 3, 2, 7, 0x860u);
    NEW_GFX_Profiler_Destroy(b);
    NEW_GFX_Profiler_Destroy(NULL);
    GFXMOCK_SaveSide(1);

    GFXMOCK_CompareSides();
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    GFXMOCK_Init();

    validateCases();
    mecanicCases();
    meshStateCases();
    xytCases();
    startStopCases();
    renderCases();
    destroyCases();

    GFXMOCK_Summary("gfxprofiler");
    return GFXMOCK_fails ? 1 : 0;
}
