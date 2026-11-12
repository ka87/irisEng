#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gfx_mocks.h"
#include "../gfx/gfxwater2.h"
#include "../gfx/gfxripple.h"
#include "../gfx/gfxsmoke.h"

struct sGfxWater2Data* BASE_GFX_Water2_Create(void);
struct sGfxWater2Data* NEW_GFX_Water2_Create(void);
void BASE_GFX_Water2_Destroy(struct sGfxWater2Data* water);
void NEW_GFX_Water2_Destroy(struct sGfxWater2Data* water);
int BASE_GFX_i_Water2_Render(struct sGfxWater2Data* water);
int NEW_GFX_i_Water2_Render(struct sGfxWater2Data* water);
void BASE_GFX_Water2_Seti(struct sGfxWater2Data* water, int id, int offset, int value);
void NEW_GFX_Water2_Seti(struct sGfxWater2Data* water, int id, int offset, int value);
void BASE_GFX_Water2_Setf(struct sGfxWater2Data* water, int id, int offset, float value);
void NEW_GFX_Water2_Setf(struct sGfxWater2Data* water, int id, int offset, float value);
void BASE_GFX_Water2_Setv(struct sGfxWater2Data* water, int id, int offset, Vec3* value);
void NEW_GFX_Water2_Setv(struct sGfxWater2Data* water, int id, int offset, Vec3* value);

void BASE_GFX_Ripple_Generate(tGfxRippleData* ripple, float dt);
void NEW_GFX_Ripple_Generate(tGfxRippleData* ripple, float dt);
void BASE_GFX_Ripple_Update(tGfxRippleData* ripple, float dt);
void NEW_GFX_Ripple_Update(tGfxRippleData* ripple, float dt);
void BASE_GFX_Ripple_Alloc(tGfxRippleData* ripple, int num);
void NEW_GFX_Ripple_Alloc(tGfxRippleData* ripple, int num);
void BASE_GFX_Ripple_Seti(struct sGfxRippleData* ripple, int id, int value);
void NEW_GFX_Ripple_Seti(struct sGfxRippleData* ripple, int id, int value);
void BASE_GFX_Ripple_Setf(struct sGfxRippleData* ripple, int id, float value);
void NEW_GFX_Ripple_Setf(struct sGfxRippleData* ripple, int id, float value);
BOOL BASE_GFX_i_Ripple_Render(struct sGfxRippleData* ripple);
BOOL NEW_GFX_i_Ripple_Render(struct sGfxRippleData* ripple);
struct sGfxRippleData* BASE_GFX_Ripple_Create(struct sGameObjectMain* gao);
struct sGfxRippleData* NEW_GFX_Ripple_Create(struct sGameObjectMain* gao);

void BASE_GFX_Smoke_Update(tGfxSmokeData* smoke, float dt, int steps);
void NEW_GFX_Smoke_Update(tGfxSmokeData* smoke, float dt, int steps);
int BASE_GFX_i_Smoke_Render(struct sGfxSmokeData* smoke);
int NEW_GFX_i_Smoke_Render(struct sGfxSmokeData* smoke);
struct sGfxSmokeData* BASE_GFX_Smoke_Create(void);
struct sGfxSmokeData* NEW_GFX_Smoke_Create(void);
void BASE_GFX_Smoke_Alloc(struct sGfxSmokeData* smoke, int num);
void NEW_GFX_Smoke_Alloc(struct sGfxSmokeData* smoke, int num);
void BASE_GFX_Smoke_AddOne(tGfxSmokeData* smoke);
void NEW_GFX_Smoke_AddOne(tGfxSmokeData* smoke);
void BASE_GFX_Smoke_Seti(struct sGfxSmokeData* smoke, int id, int num);
void NEW_GFX_Smoke_Seti(struct sGfxSmokeData* smoke, int id, int num);

#define DROPS 24
#define RINGS 16
#define PUFFS 20

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

/* ------------------------------------------------------------------ */
/* water2: a ring of drops whose lifetimes are arranged so that some   */
/* expire on each render, which is what drives the compaction loop     */
/* the rewrite restructured.                                          */
/* ------------------------------------------------------------------ */

static void water2Fill(tGfxWater2Data* water, tGfxWater2InnerData* drops,
                       int num, unsigned seed)
{
    int i;

    rngSeed(seed);
    water->numToAlloc = num;
    water->i2 = 0;
    water->num = num;
    water->ptr = drops;
    for (i = 0; i < num; i++)
    {
        drops[i].f1 = rngFloat(-20.0f, 20.0f);
        drops[i].f2 = rngFloat(-20.0f, 20.0f);
        drops[i].f3 = rngFloat(-5.0f, 5.0f);
        drops[i].v3.x = rngFloat(-3.0f, 3.0f);
        drops[i].v3.y = rngFloat(-3.0f, 3.0f);
        drops[i].v3.z = rngFloat(0.5f, 4.0f);
        drops[i].ff1 = rngFloat(0.05f, 1.5f);
        drops[i].ff2 = rngFloat(0.1f, 2.0f);
        drops[i].ff3 = rngFloat(0.1f, 3.0f);
        drops[i].ff4 = rngFloat(0.05f, 0.8f);
        drops[i].ff5 = rngFloat(0.0f, 1.2f);
        drops[i].color = (int)(0xFF000000u | (unsigned)(i * 0x010307));
    }
}

static int water2Compare(tGfxWater2Data* a, tGfxWater2Data* b,
                         tGfxWater2InnerData* da, tGfxWater2InnerData* db, int num)
{
    char what[64];
    int ok = 1;
    int i;

    ok &= GFXMOCK_SameInt(a->num, b->num, "water.num", 0);
    ok &= GFXMOCK_SameInt(a->numToAlloc, b->numToAlloc, "water.numToAlloc", 0);
    ok &= GFXMOCK_SameInt(a->i2, b->i2, "water.i2", 0);
    for (i = 0; i < num; i++)
    {
        snprintf(what, sizeof(what), "drop%d.f1", i);
        ok &= GFXMOCK_Close(da[i].f1, db[i].f1, what, 0);
        snprintf(what, sizeof(what), "drop%d.f2", i);
        ok &= GFXMOCK_Close(da[i].f2, db[i].f2, what, 0);
        snprintf(what, sizeof(what), "drop%d.f3", i);
        ok &= GFXMOCK_Close(da[i].f3, db[i].f3, what, 0);
        snprintf(what, sizeof(what), "drop%d.v3", i);
        ok &= GFXMOCK_CloseVec(&da[i].v3, &db[i].v3, what);
        snprintf(what, sizeof(what), "drop%d.ff5", i);
        ok &= GFXMOCK_Close(da[i].ff5, db[i].ff5, what, 0);
        snprintf(what, sizeof(what), "drop%d.color", i);
        ok &= GFXMOCK_SameInt(da[i].color, db[i].color, what, 0);
    }
    return ok;
}

static void water2Cases(void)
{
    static tGfxWater2InnerData da[DROPS];
    static tGfxWater2InnerData db[DROPS];
    tGfxWater2Data a;
    tGfxWater2Data b;
    char name[96];
    float dts[4] = { 0.0f, 0.016666668f, 0.1f, 0.75f };
    int counts[4] = { 1, 3, 12, DROPS };
    int ci, di, frame;
    int rbase, rnew;

    for (ci = 0; ci < 4; ci++)
        for (di = 0; di < 4; di++)
        {
            snprintf(name, sizeof(name), "water2 drops=%d dt=%g", counts[ci], dts[di]);
            GFXMOCK_SetCase(name);

            TIM_gf_dt = dts[di];

            GFXMOCK_Reset();
            water2Fill(&a, da, counts[ci], 0x51EDu + (unsigned)(ci * 16 + di));
            rbase = BASE_GFX_i_Water2_Render(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            water2Fill(&b, db, counts[ci], 0x51EDu + (unsigned)(ci * 16 + di));
            rnew = NEW_GFX_i_Water2_Render(&b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "water2 return", 0)
                || !water2Compare(&a, &b, da, db, counts[ci]))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* Several frames in a row, so a drop that dies in frame n changes
       what frame n+1 sees. */
    for (frame = 0; frame < 6; frame++)
    {
        snprintf(name, sizeof(name), "water2 frame chain step %d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.2f;

        GFXMOCK_Reset();
        water2Fill(&a, da, DROPS, 0xABCDu);
        water2Fill(&b, db, DROPS, 0xABCDu);
        for (di = 0; di <= frame; di++)
            BASE_GFX_i_Water2_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        for (di = 0; di <= frame; di++)
            NEW_GFX_i_Water2_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!water2Compare(&a, &b, da, db, DROPS))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* The setters, including the 10102 add that grows the list. */
    GFXMOCK_SetCase("water2 setters");
    GFXMOCK_Reset();
    {
        tGfxWater2Data* wa;
        tGfxWater2Data* wb;
        Vec3 v;

        wa = BASE_GFX_Water2_Create();
        wa->numToAlloc = 4;
        BASE_GFX_Water2_Seti(wa, 10102, 0, 0);
        BASE_GFX_Water2_Setf(wa, 0, (int)((char*)&wa->ptr[0].ff1 - (char*)&wa->ptr[0]), 2.5f);
        v.x = 1.0f; v.y = -2.0f; v.z = 3.0f;
        BASE_GFX_Water2_Setv(wa, 0, (int)((char*)&wa->ptr[0].v3 - (char*)&wa->ptr[0]), &v);
        BASE_GFX_Water2_Seti(wa, 0, (int)((char*)&wa->ptr[0].color - (char*)&wa->ptr[0]), 0x7F3311);
        BASE_GFX_Water2_Seti(wa, 10102, 0, 0);
        BASE_GFX_Water2_Destroy(wa);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        wb = NEW_GFX_Water2_Create();
        wb->numToAlloc = 4;
        NEW_GFX_Water2_Seti(wb, 10102, 0, 0);
        NEW_GFX_Water2_Setf(wb, 0, (int)((char*)&wb->ptr[0].ff1 - (char*)&wb->ptr[0]), 2.5f);
        v.x = 1.0f; v.y = -2.0f; v.z = 3.0f;
        NEW_GFX_Water2_Setv(wb, 0, (int)((char*)&wb->ptr[0].v3 - (char*)&wb->ptr[0]), &v);
        NEW_GFX_Water2_Seti(wb, 0, (int)((char*)&wb->ptr[0].color - (char*)&wb->ptr[0]), 0x7F3311);
        NEW_GFX_Water2_Seti(wb, 10102, 0, 0);
        NEW_GFX_Water2_Destroy(wb);
        GFXMOCK_SaveSide(1);

        GFXMOCK_CompareSides();
    }
}

/* ------------------------------------------------------------------ */
/* ripple                                                             */
/* ------------------------------------------------------------------ */

static void rippleFill(tGfxRippleData* ripple, tGfxRippleInnerData* rings,
                       int num, int live, int flags, float spacing, unsigned seed)
{
    int i;

    rngSeed(seed);
    memset(ripple, 0, sizeof(*ripple));
    ripple->pos.x = 3.0f;
    ripple->pos.y = -1.5f;
    ripple->pos.z = 0.25f;
    ripple->v1.x = 0.0f;
    ripple->v1.y = 0.0f;
    ripple->v1.z = -1.0f;
    ripple->v2.x = 1.0f;
    ripple->v2.y = 0.0f;
    ripple->v2.z = 0.0f;
    ripple->f1 = 5.0f;
    ripple->f2 = 1.25f;
    ripple->f3 = spacing;
    ripple->f4 = 0.8f;
    ripple->f555 = 0.3f;
    ripple->num = num;
    ripple->color1 = 0x203040;
    ripple->color2 = (int)0xFFC0A080u;
    ripple->rippleMaxElemID = live;
    ripple->flags = flags;
    ripple->numUnk = live;
    ripple->ptr = rings;
    ripple->fff = 0.0f;

    memset(rings, 0, sizeof(tGfxRippleInnerData) * num);
    for (i = 0; i < live; i++)
    {
        rings[i].pos.x = rngFloat(-10.0f, 10.0f);
        rings[i].pos.y = rngFloat(-10.0f, 10.0f);
        rings[i].pos.z = rngFloat(-2.0f, 2.0f);
        rings[i].v1Inv.x = rngFloat(-1.0f, 1.0f);
        rings[i].v1Inv.y = rngFloat(-1.0f, 1.0f);
        rings[i].v1Inv.z = rngFloat(-1.0f, 1.0f);
        rings[i].v2.x = rngFloat(-1.0f, 1.0f);
        rings[i].v2.y = rngFloat(-1.0f, 1.0f);
        rings[i].v2.z = rngFloat(-1.0f, 1.0f);
        rings[i].f1 = rngFloat(-0.1f, 5.0f);
    }
}

static int rippleCompare(tGfxRippleData* a, tGfxRippleData* b,
                         tGfxRippleInnerData* ra, tGfxRippleInnerData* rb, int num)
{
    char what[64];
    int ok = 1;
    int i;

    ok &= GFXMOCK_SameInt(a->rippleMaxElemID, b->rippleMaxElemID, "ripple.maxElemID", 0);
    ok &= GFXMOCK_SameInt(a->numUnk, b->numUnk, "ripple.numUnk", 0);
    ok &= GFXMOCK_SameInt(a->num, b->num, "ripple.num", 0);
    ok &= GFXMOCK_SameInt(a->flags, b->flags, "ripple.flags", 0);
    ok &= GFXMOCK_Close(a->f3, b->f3, "ripple.f3", 0);
    ok &= GFXMOCK_Close(a->fff, b->fff, "ripple.fff", 0);
    for (i = 0; i < num; i++)
    {
        snprintf(what, sizeof(what), "ring%d.pos", i);
        ok &= GFXMOCK_CloseVec(&ra[i].pos, &rb[i].pos, what);
        snprintf(what, sizeof(what), "ring%d.v1Inv", i);
        ok &= GFXMOCK_CloseVec(&ra[i].v1Inv, &rb[i].v1Inv, what);
        snprintf(what, sizeof(what), "ring%d.v2", i);
        ok &= GFXMOCK_CloseVec(&ra[i].v2, &rb[i].v2, what);
        snprintf(what, sizeof(what), "ring%d.f1", i);
        ok &= GFXMOCK_Close(ra[i].f1, rb[i].f1, what, 0);
    }
    return ok;
}

static void rippleCases(void)
{
    static tGfxRippleInnerData ra[RINGS];
    static tGfxRippleInnerData rb[RINGS];
    tGfxRippleData a;
    tGfxRippleData b;
    char name[96];
    float spacings[4] = { -1.0f, 0.0f, 0.05f, 0.4f };
    int lives[4] = { 0, 1, 8, RINGS };
    int flagSet[3] = { 0, 1, 2 };
    int li, si, fi, frame, dist;
    int rbase, rnew;

    for (li = 0; li < 4; li++)
        for (si = 0; si < 4; si++)
            for (fi = 0; fi < 3; fi++)
            {
                snprintf(name, sizeof(name), "ripple live=%d spacing=%g flags=%d",
                         lives[li], spacings[si], flagSet[fi]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = 0.1f;

                GFXMOCK_Reset();
                rippleFill(&a, ra, RINGS, lives[li], flagSet[fi], spacings[si], 0x3C1u);
                rbase = BASE_GFX_i_Ripple_Render(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                rippleFill(&b, rb, RINGS, lives[li], flagSet[fi], spacings[si], 0x3C1u);
                rnew = NEW_GFX_i_Ripple_Render(&b);
                GFXMOCK_SaveSide(1);

                if (!GFXMOCK_SameInt(rbase, rnew, "ripple return", 0)
                    || !rippleCompare(&a, &b, ra, rb, RINGS))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* distance 0 is the early return the render shares with smoke. */
    for (dist = 0; dist < 2; dist++)
    {
        snprintf(name, sizeof(name), "ripple distance=%d", dist);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.05f;

        GFXMOCK_Reset();
        GFXMOCK_SetDistance(dist);
        rippleFill(&a, ra, RINGS, 6, 0, 0.2f, 0x9A1u);
        rbase = BASE_GFX_i_Ripple_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        GFXMOCK_SetDistance(dist);
        rippleFill(&b, rb, RINGS, 6, 0, 0.2f, 0x9A1u);
        rnew = NEW_GFX_i_Ripple_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(rbase, rnew, "ripple return", 0)
            || !rippleCompare(&a, &b, ra, rb, RINGS))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
    GFXMOCK_SetDistance(1);

    /* Generate and Update on their own, over a chain of frames, so the
       ring list grows and shrinks across calls. */
    for (frame = 1; frame <= 8; frame++)
    {
        snprintf(name, sizeof(name), "ripple generate/update x%d", frame);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        rippleFill(&a, ra, RINGS, 2, 2, 0.15f, 0x7771u);
        for (si = 0; si < frame; si++)
        {
            BASE_GFX_Ripple_Generate(&a, 0.1f);
            BASE_GFX_Ripple_Update(&a, 0.6f);
        }
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        rippleFill(&b, rb, RINGS, 2, 2, 0.15f, 0x7771u);
        for (si = 0; si < frame; si++)
        {
            NEW_GFX_Ripple_Generate(&b, 0.1f);
            NEW_GFX_Ripple_Update(&b, 0.6f);
        }
        GFXMOCK_SaveSide(1);

        if (!rippleCompare(&a, &b, ra, rb, RINGS))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* Alloc, the setters and Create, which all touch the allocator. */
    GFXMOCK_SetCase("ripple alloc and setters");
    {
        tGfxRippleData* pa;
        tGfxRippleData* pb;

        GFXMOCK_Reset();
        pa = BASE_GFX_Ripple_Create(GFXMOCK_elem->aiGameObject);
        BASE_GFX_Ripple_Alloc(pa, 8);
        BASE_GFX_Ripple_Alloc(pa, 8);
        BASE_GFX_Ripple_Alloc(pa, 12);
        BASE_GFX_Ripple_Setf(pa, 20002, 0.25f);
        BASE_GFX_Ripple_Seti(pa, 20100, -1);
        BASE_GFX_Ripple_Seti(pa, 20100, -2);
        BASE_GFX_Ripple_Seti(pa, 20100, 4);
        BASE_GFX_Ripple_Alloc(pa, 0);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        pb = NEW_GFX_Ripple_Create(GFXMOCK_elem->aiGameObject);
        NEW_GFX_Ripple_Alloc(pb, 8);
        NEW_GFX_Ripple_Alloc(pb, 8);
        NEW_GFX_Ripple_Alloc(pb, 12);
        NEW_GFX_Ripple_Setf(pb, 20002, 0.25f);
        NEW_GFX_Ripple_Seti(pb, 20100, -1);
        NEW_GFX_Ripple_Seti(pb, 20100, -2);
        NEW_GFX_Ripple_Seti(pb, 20100, 4);
        NEW_GFX_Ripple_Alloc(pb, 0);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(pa->flags, pb->flags, "ripple.flags", 0)
            || !GFXMOCK_SameInt(pa->num, pb->num, "ripple.num", 0)
            || !GFXMOCK_Close(pa->f3, pb->f3, "ripple.f3", 0)
            || !GFXMOCK_Close(pa->fff, pb->fff, "ripple.fff", 0))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

/* ------------------------------------------------------------------ */
/* smoke                                                              */
/* ------------------------------------------------------------------ */

static void smokeFill(tGfxSmokeData* smoke, tGfxSmokeInnerData* puffs,
                      int num, int live, float life, float elapsed, unsigned seed)
{
    int i;

    rngSeed(seed);
    memset(smoke, 0, sizeof(*smoke));
    smoke->innerData1 = 1.0f;
    smoke->innerData2 = 2.0f;
    smoke->innerData3 = 3.0f;
    smoke->innerData4 = 0.5f;
    smoke->innerData5 = -0.25f;
    smoke->innerData6 = 0.75f;
    smoke->i1 = life;
    smoke->i2 = 0.89999998f;
    smoke->i3 = 0.5f;
    smoke->i4 = 2.0f;
    smoke->color1 = (int)0xFFFFFFFFu;
    smoke->color2 = 0x112233;
    smoke->innerDataNum = num;
    smoke->num = live;
    smoke->ptr = puffs;
    smoke->timeElapsed = elapsed;

    memset(puffs, 0, sizeof(tGfxSmokeInnerData) * num);
    for (i = 0; i < live; i++)
    {
        puffs[i].i1 = rngFloat(-8.0f, 8.0f);
        puffs[i].i2 = rngFloat(-8.0f, 8.0f);
        puffs[i].i3 = rngFloat(-3.0f, 3.0f);
        puffs[i].i4 = rngFloat(-2.0f, 2.0f);
        puffs[i].i5 = rngFloat(-2.0f, 2.0f);
        puffs[i].i6 = rngFloat(-2.0f, 2.0f);
        puffs[i].i7 = rngFloat(0.0f, 2.5f);
    }
}

static int smokeCompare(tGfxSmokeData* a, tGfxSmokeData* b,
                        tGfxSmokeInnerData* pa, tGfxSmokeInnerData* pb, int num)
{
    char what[64];
    int ok = 1;
    int i;

    ok &= GFXMOCK_SameInt(a->num, b->num, "smoke.num", 0);
    ok &= GFXMOCK_SameInt(a->innerDataNum, b->innerDataNum, "smoke.innerDataNum", 0);
    ok &= GFXMOCK_Close(a->timeElapsed, b->timeElapsed, "smoke.timeElapsed", 0);
    for (i = 0; i < num; i++)
    {
        snprintf(what, sizeof(what), "puff%d.i1", i);
        ok &= GFXMOCK_Close(pa[i].i1, pb[i].i1, what, 0);
        snprintf(what, sizeof(what), "puff%d.i2", i);
        ok &= GFXMOCK_Close(pa[i].i2, pb[i].i2, what, 0);
        snprintf(what, sizeof(what), "puff%d.i3", i);
        ok &= GFXMOCK_Close(pa[i].i3, pb[i].i3, what, 0);
        snprintf(what, sizeof(what), "puff%d.i4", i);
        ok &= GFXMOCK_Close(pa[i].i4, pb[i].i4, what, 0);
        snprintf(what, sizeof(what), "puff%d.i5", i);
        ok &= GFXMOCK_Close(pa[i].i5, pb[i].i5, what, 0);
        snprintf(what, sizeof(what), "puff%d.i6", i);
        ok &= GFXMOCK_Close(pa[i].i6, pb[i].i6, what, 0);
        snprintf(what, sizeof(what), "puff%d.i7", i);
        ok &= GFXMOCK_Close(pa[i].i7, pb[i].i7, what, 0);
    }
    return ok;
}

static void smokeCases(void)
{
    static tGfxSmokeInnerData pa[PUFFS];
    static tGfxSmokeInnerData pb[PUFFS];
    tGfxSmokeData a;
    tGfxSmokeData b;
    char name[96];
    float lives[3] = { 0.0f, 1.0f, 2.5f };
    float dts[4] = { 0.0f, 0.008f, 0.033333f, 0.5f };
    int lives2[3] = { 1, 7, PUFFS };
    int li, di, ni, steps, frame;
    int rbase, rnew;

    for (li = 0; li < 3; li++)
        for (di = 0; di < 4; di++)
            for (ni = 0; ni < 3; ni++)
            {
                snprintf(name, sizeof(name), "smoke live=%d life=%g dt=%g",
                         lives2[ni], lives[li], dts[di]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = dts[di];

                GFXMOCK_Reset();
                smokeFill(&a, pa, PUFFS, lives2[ni], lives[li], 0.004f, 0x2BEEu);
                rbase = BASE_GFX_i_Smoke_Render(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                smokeFill(&b, pb, PUFFS, lives2[ni], lives[li], 0.004f, 0x2BEEu);
                rnew = NEW_GFX_i_Smoke_Render(&b);
                GFXMOCK_SaveSide(1);

                if (!GFXMOCK_SameInt(rbase, rnew, "smoke return", 0)
                    || !smokeCompare(&a, &b, pa, pb, PUFFS))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* Update on its own with a step count that exercises the inner
       loop the rewrite turned into a for. */
    for (steps = 0; steps <= 4; steps++)
    {
        snprintf(name, sizeof(name), "smoke update steps=%d", steps);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        smokeFill(&a, pa, PUFFS, PUFFS, 2.0f, 0.0f, 0x5151u);
        BASE_GFX_Smoke_Update(&a, 0.25f, steps);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        smokeFill(&b, pb, PUFFS, PUFFS, 2.0f, 0.0f, 0x5151u);
        NEW_GFX_Smoke_Update(&b, 0.25f, steps);
        GFXMOCK_SaveSide(1);

        if (!smokeCompare(&a, &b, pa, pb, PUFFS))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* A chain of renders, so puffs expire between frames. */
    for (frame = 1; frame <= 6; frame++)
    {
        snprintf(name, sizeof(name), "smoke frame chain step %d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.2f;

        GFXMOCK_Reset();
        smokeFill(&a, pa, PUFFS, PUFFS, 1.5f, 0.0f, 0x6262u);
        for (di = 0; di < frame; di++)
            BASE_GFX_i_Smoke_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        smokeFill(&b, pb, PUFFS, PUFFS, 1.5f, 0.0f, 0x6262u);
        for (di = 0; di < frame; di++)
            NEW_GFX_i_Smoke_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!smokeCompare(&a, &b, pa, pb, PUFFS))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* Create, Alloc, AddOne and the Seti dispatch. */
    GFXMOCK_SetCase("smoke alloc and setters");
    {
        tGfxSmokeData* sa;
        tGfxSmokeData* sb;

        GFXMOCK_Reset();
        sa = BASE_GFX_Smoke_Create();
        BASE_GFX_Smoke_Seti(sa, 6100, 4);
        BASE_GFX_Smoke_Seti(sa, 6100, 4);
        BASE_GFX_Smoke_Seti(sa, 6100, 6);
        sa->innerData1 = 1.5f;
        sa->innerData4 = -0.5f;
        for (di = 0; di < 8; di++)
            BASE_GFX_Smoke_Seti(sa, 6103, 0);
        BASE_GFX_Smoke_Seti(sa, 6100, 0);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        sb = NEW_GFX_Smoke_Create();
        NEW_GFX_Smoke_Seti(sb, 6100, 4);
        NEW_GFX_Smoke_Seti(sb, 6100, 4);
        NEW_GFX_Smoke_Seti(sb, 6100, 6);
        sb->innerData1 = 1.5f;
        sb->innerData4 = -0.5f;
        for (di = 0; di < 8; di++)
            NEW_GFX_Smoke_Seti(sb, 6103, 0);
        NEW_GFX_Smoke_Seti(sb, 6100, 0);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(sa->num, sb->num, "smoke.num", 0)
            || !GFXMOCK_SameInt(sa->innerDataNum, sb->innerDataNum, "smoke.innerDataNum", 0))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    GFXMOCK_Init();

    water2Cases();
    rippleCases();
    smokeCases();

    GFXMOCK_Summary("gfxwater2 / gfxripple / gfxsmoke");
    return GFXMOCK_fails ? 1 : 0;
}
