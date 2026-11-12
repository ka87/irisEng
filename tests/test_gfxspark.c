#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../memory.h"
#include "gfx_mocks.h"
#include "../gfx/gfxspark.h"

struct sGfxSparkData* BASE_GFX_Spark_Create(void);
struct sGfxSparkData* NEW_GFX_Spark_Create(void);
void BASE_GFX_Spark_Destroy(struct sGfxSparkData* spark);
void NEW_GFX_Spark_Destroy(struct sGfxSparkData* spark);
void BASE_GFX_Spark_Generate(struct sGfxSparkData* spark);
void NEW_GFX_Spark_Generate(struct sGfxSparkData* spark);
void BASE_GFX_Spark_Update(tGfxSparkData* spark, Vec3* head, Vec3* tail, int stride);
void NEW_GFX_Spark_Update(tGfxSparkData* spark, Vec3* head, Vec3* tail, int stride);
void BASE_GFX_AllocSpark(tGfxSparkData* spark);
void NEW_GFX_AllocSpark(tGfxSparkData* spark);
int BASE_GFX_i_Spark_Render(struct sGfxSparkData* spark);
int NEW_GFX_i_Spark_Render(struct sGfxSparkData* spark);

#define SPARKS 24
#define STRIP (4 * SPARKS)

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

static void sparkFill(tGfxSparkData* spark, tGfxSparkElemData* elems,
                      int num, int active, int flags, unsigned seed)
{
    int i;

    rngSeed(seed);
    memset(spark, 0, sizeof(*spark));
    spark->v0.x = 1.0f;
    spark->v0.y = -2.0f;
    spark->v0.z = 0.5f;
    spark->v1.x = 0.0f;
    spark->v1.y = 0.0f;
    spark->v1.z = 1.0f;
    spark->i4 = 0.3f;
    spark->i5 = -0.2f;
    spark->i6 = -2.0f;
    spark->i7 = 0.6f;
    spark->rand2Max = 2.0f;
    spark->rand2Min = 4.0f;
    spark->i10 = 0.89999998f;
    spark->ii1 = 2.0f;
    spark->rand3Max = 1.0f;
    spark->rand3Min = 2.0f;
    spark->ii4 = 1.0f;
    spark->ii5 = 0.1f;
    spark->randMin = 0.2f;
    spark->randMax = 0.8f;
    spark->num = num;
    spark->iii1 = (int)0xFFCC9966u;
    spark->flags = flags;
    spark->allocdNum = num;
    spark->actNum = active;
    spark->timeRemaining = 0.05f;
    spark->ptr = elems;

    memset(elems, 0, sizeof(tGfxSparkElemData) * SPARKS);
    for (i = 0; i < active && i < SPARKS; i++)
    {
        elems[i].i1 = (i & 1);
        elems[i].v1.x = rngFloat(-5.0f, 5.0f);
        elems[i].v1.y = rngFloat(-5.0f, 5.0f);
        elems[i].v1.z = rngFloat(-2.0f, 2.0f);
        elems[i].i5 = rngFloat(-3.0f, 3.0f);
        elems[i].i6 = rngFloat(-3.0f, 3.0f);
        elems[i].i7 = rngFloat(-3.0f, 3.0f);
        elems[i].i8 = rngFloat(-0.05f, 1.2f);
        elems[i].i9 = rngFloat(0.1f, 4.0f);
    }
}

static int sparkCompare(tGfxSparkData* a, tGfxSparkData* b,
                        tGfxSparkElemData* ea, tGfxSparkElemData* eb)
{
    char what[64];
    int ok = 1;
    int i;

    ok &= GFXMOCK_SameInt(a->actNum, b->actNum, "spark.actNum", 0);
    ok &= GFXMOCK_SameInt(a->num, b->num, "spark.num", 0);
    ok &= GFXMOCK_SameInt(a->allocdNum, b->allocdNum, "spark.allocdNum", 0);
    ok &= GFXMOCK_SameInt(a->flags, b->flags, "spark.flags", 0);
    ok &= GFXMOCK_SameInt(a->iii1, b->iii1, "spark.iii1", 0);
    ok &= GFXMOCK_Close(a->timeRemaining, b->timeRemaining, "spark.timeRemaining", 0);
    for (i = 0; i < SPARKS; i++)
    {
        snprintf(what, sizeof(what), "elem%d.i1", i);
        ok &= GFXMOCK_SameInt(ea[i].i1, eb[i].i1, what, 0);
        snprintf(what, sizeof(what), "elem%d.v1", i);
        ok &= GFXMOCK_CloseVec(&ea[i].v1, &eb[i].v1, what);
        snprintf(what, sizeof(what), "elem%d.i5", i);
        ok &= GFXMOCK_Close(ea[i].i5, eb[i].i5, what, 0);
        snprintf(what, sizeof(what), "elem%d.i6", i);
        ok &= GFXMOCK_Close(ea[i].i6, eb[i].i6, what, 0);
        snprintf(what, sizeof(what), "elem%d.i7", i);
        ok &= GFXMOCK_Close(ea[i].i7, eb[i].i7, what, 0);
        snprintf(what, sizeof(what), "elem%d.i8", i);
        ok &= GFXMOCK_Close(ea[i].i8, eb[i].i8, what, 0);
        snprintf(what, sizeof(what), "elem%d.i9", i);
        ok &= GFXMOCK_Close(ea[i].i9, eb[i].i9, what, 0);
    }
    return ok;
}

static int stripCompare(Vec3* a, Vec3* b, int n)
{
    char what[64];
    int ok = 1;
    int i;

    for (i = 0; i < n; i++)
    {
        /* Slot 1 of each triple carries the strip width in x and a
           colour, as raw bits, in y, so y is compared as an int. */
        snprintf(what, sizeof(what), "strip%d.x", i);
        ok &= GFXMOCK_Close(a[i].x, b[i].x, what, 0);
        snprintf(what, sizeof(what), "strip%d.y", i);
        if (memcmp(&a[i].y, &b[i].y, sizeof(float)) != 0)
            ok &= GFXMOCK_Close(a[i].y, b[i].y, what, 0);
        snprintf(what, sizeof(what), "strip%d.z", i);
        ok &= GFXMOCK_Close(a[i].z, b[i].z, what, 0);
    }
    return ok;
}

static void generateCases(void)
{
    static tGfxSparkElemData ea[SPARKS];
    static tGfxSparkElemData eb[SPARKS];
    tGfxSparkData a;
    tGfxSparkData b;
    char name[96];
    int actives[4] = { 0, 1, 12, SPARKS };
    float spacings[3] = { 0.1f, 0.0f, 0.4f };
    float cones[3] = { 0.0f, 0.6f, 3.0f };
    int ai, si, ci, pass;

    for (ai = 0; ai < 4; ai++)
        for (si = 0; si < 3; si++)
            for (ci = 0; ci < 3; ci++)
            {
                snprintf(name, sizeof(name), "Generate active=%d spacing=%g cone=%g",
                         actives[ai], spacings[si], cones[ci]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = 0.1f;

                GFXMOCK_Reset();
                sparkFill(&a, ea, SPARKS, actives[ai], 0x20, 0x500u);
                a.ii5 = spacings[si];
                a.i7 = cones[ci];
                BASE_GFX_Spark_Generate(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                sparkFill(&b, eb, SPARKS, actives[ai], 0x20, 0x500u);
                b.ii5 = spacings[si];
                b.i7 = cones[ci];
                NEW_GFX_Spark_Generate(&b);
                GFXMOCK_SaveSide(1);

                if (!sparkCompare(&a, &b, ea, eb))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* An emission axis parallel to the world up, which is the branch
       that reaches for the other base vector. */
    GFXMOCK_SetCase("Generate axis along K");
    TIM_gf_dt = 0.2f;
    GFXMOCK_Reset();
    sparkFill(&a, ea, SPARKS, 0, 0x20, 0x501u);
    a.v1 = MATH_gst_BaseVectorK;
    BASE_GFX_Spark_Generate(&a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    sparkFill(&b, eb, SPARKS, 0, 0x20, 0x501u);
    b.v1 = MATH_gst_BaseVectorK;
    NEW_GFX_Spark_Generate(&b);
    GFXMOCK_SaveSide(1);
    if (!sparkCompare(&a, &b, ea, eb))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    /* Several passes, so the emission clock carries over. */
    for (pass = 1; pass <= 5; pass++)
    {
        snprintf(name, sizeof(name), "Generate x%d", pass);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.07f;

        GFXMOCK_Reset();
        sparkFill(&a, ea, SPARKS, 0, 0x20, 0x502u);
        for (ai = 0; ai < pass; ai++)
            BASE_GFX_Spark_Generate(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        sparkFill(&b, eb, SPARKS, 0, 0x20, 0x502u);
        for (ai = 0; ai < pass; ai++)
            NEW_GFX_Spark_Generate(&b);
        GFXMOCK_SaveSide(1);

        if (!sparkCompare(&a, &b, ea, eb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void updateCases(void)
{
    static tGfxSparkElemData ea[SPARKS];
    static tGfxSparkElemData eb[SPARKS];
    static Vec3 stripA[STRIP];
    static Vec3 stripB[STRIP];
    tGfxSparkData a;
    tGfxSparkData b;
    char name[96];
    int modes[4] = { 0, 1, 2, 3 };
    float dts[4] = { 0.0f, 0.016666668f, 0.05f, 0.4f };
    float stretch[3] = { 0.0f, 1.0f, 2.5f };
    int mi, di, si, strides[2] = { 3, 4 };
    int sti, respawn, pass;

    for (mi = 0; mi < 4; mi++)
        for (di = 0; di < 4; di++)
            for (si = 0; si < 3; si++)
            {
                snprintf(name, sizeof(name), "Update mode=%d dt=%g stretch=%g",
                         modes[mi], dts[di], stretch[si]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = dts[di];

                GFXMOCK_Reset();
                memset(stripA, 0, sizeof(stripA));
                sparkFill(&a, ea, SPARKS, SPARKS, 0x20 | modes[mi], 0x600u);
                a.ii4 = stretch[si];
                BASE_GFX_Spark_Update(&a, stripA, stripA + 2, 3);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                memset(stripB, 0, sizeof(stripB));
                sparkFill(&b, eb, SPARKS, SPARKS, 0x20 | modes[mi], 0x600u);
                b.ii4 = stretch[si];
                NEW_GFX_Spark_Update(&b, stripB, stripB + 2, 3);
                GFXMOCK_SaveSide(1);

                if (!sparkCompare(&a, &b, ea, eb)
                    || !stripCompare(stripA, stripB, STRIP))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* Both strides, and the second-life path that 0x10 opens. */
    for (sti = 0; sti < 2; sti++)
        for (respawn = 0; respawn < 2; respawn++)
        {
            snprintf(name, sizeof(name), "Update stride=%d respawn=%d",
                     strides[sti], respawn);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.1f;

            GFXMOCK_Reset();
            memset(stripA, 0, sizeof(stripA));
            sparkFill(&a, ea, SPARKS, SPARKS, respawn ? 0x30 : 0x20, 0x601u);
            BASE_GFX_Spark_Update(&a, stripA, stripA + 2, strides[sti]);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            memset(stripB, 0, sizeof(stripB));
            sparkFill(&b, eb, SPARKS, SPARKS, respawn ? 0x30 : 0x20, 0x601u);
            NEW_GFX_Spark_Update(&b, stripB, stripB + 2, strides[sti]);
            GFXMOCK_SaveSide(1);

            if (!sparkCompare(&a, &b, ea, eb)
                || !stripCompare(stripA, stripB, STRIP))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* A respawn window that can hand back a non-positive extra life, so
       the drop inside the respawn branch runs too. */
    GFXMOCK_SetCase("Update respawn with no life left");
    TIM_gf_dt = 0.5f;
    GFXMOCK_Reset();
    memset(stripA, 0, sizeof(stripA));
    sparkFill(&a, ea, SPARKS, SPARKS, 0x30, 0x602u);
    a.randMin = -1.0f;
    a.randMax = 0.0f;
    BASE_GFX_Spark_Update(&a, stripA, stripA + 2, 3);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    memset(stripB, 0, sizeof(stripB));
    sparkFill(&b, eb, SPARKS, SPARKS, 0x30, 0x602u);
    b.randMin = -1.0f;
    b.randMax = 0.0f;
    NEW_GFX_Spark_Update(&b, stripB, stripB + 2, 3);
    GFXMOCK_SaveSide(1);
    if (!sparkCompare(&a, &b, ea, eb)
        || !stripCompare(stripA, stripB, STRIP))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    /* Nothing active: the early return. */
    GFXMOCK_SetCase("Update nothing active");
    GFXMOCK_Reset();
    memset(stripA, 0, sizeof(stripA));
    sparkFill(&a, ea, SPARKS, 0, 0x20, 0x603u);
    BASE_GFX_Spark_Update(&a, stripA, stripA + 2, 3);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    memset(stripB, 0, sizeof(stripB));
    sparkFill(&b, eb, SPARKS, 0, 0x20, 0x603u);
    NEW_GFX_Spark_Update(&b, stripB, stripB + 2, 3);
    GFXMOCK_SaveSide(1);
    if (!sparkCompare(&a, &b, ea, eb)
        || !stripCompare(stripA, stripB, STRIP))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    /* Repeated passes, which is where the sub-stepping accumulates. */
    for (pass = 1; pass <= 4; pass++)
    {
        snprintf(name, sizeof(name), "Update x%d", pass);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.12f;

        GFXMOCK_Reset();
        memset(stripA, 0, sizeof(stripA));
        sparkFill(&a, ea, SPARKS, SPARKS, 0x20, 0x604u);
        for (mi = 0; mi < pass; mi++)
            BASE_GFX_Spark_Update(&a, stripA, stripA + 2, 3);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        memset(stripB, 0, sizeof(stripB));
        sparkFill(&b, eb, SPARKS, SPARKS, 0x20, 0x604u);
        for (mi = 0; mi < pass; mi++)
            NEW_GFX_Spark_Update(&b, stripB, stripB + 2, 3);
        GFXMOCK_SaveSide(1);

        if (!sparkCompare(&a, &b, ea, eb)
            || !stripCompare(stripA, stripB, STRIP))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void renderCases(void)
{
    static tGfxSparkElemData ea[SPARKS];
    static tGfxSparkElemData eb[SPARKS];
    tGfxSparkData a;
    tGfxSparkData b;
    char name[96];
    int flagSet[6] = { 0x20, 0x24, 0x28, 0x2C, 0x30, 0x21 };
    int actives[3] = { 0, 5, SPARKS };
    int fi, ai, dist, frame, i;
    int rbase, rnew;

    for (fi = 0; fi < 6; fi++)
        for (ai = 0; ai < 3; ai++)
        {
            snprintf(name, sizeof(name), "Render flags=0x%X active=%d",
                     flagSet[fi], actives[ai]);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.1f;

            GFXMOCK_Reset();
            sparkFill(&a, ea, SPARKS, actives[ai], flagSet[fi], 0x700u);
            rbase = BASE_GFX_i_Spark_Render(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            sparkFill(&b, eb, SPARKS, actives[ai], flagSet[fi], 0x700u);
            rnew = NEW_GFX_i_Spark_Render(&b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
                || !sparkCompare(&a, &b, ea, eb))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* A null effect and distance zero, the two cheap exits. */
    GFXMOCK_SetCase("Render null");
    GFXMOCK_Reset();
    rbase = BASE_GFX_i_Spark_Render(NULL);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    rnew = NEW_GFX_i_Spark_Render(NULL);
    GFXMOCK_SaveSide(1);
    if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    for (dist = 0; dist < 2; dist++)
    {
        snprintf(name, sizeof(name), "Render distance=%d", dist);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.1f;

        GFXMOCK_Reset();
        GFXMOCK_SetDistance(dist);
        sparkFill(&a, ea, SPARKS, SPARKS, 0x20, 0x701u);
        rbase = BASE_GFX_i_Spark_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        GFXMOCK_SetDistance(dist);
        sparkFill(&b, eb, SPARKS, SPARKS, 0x20, 0x701u);
        rnew = NEW_GFX_i_Spark_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(rbase, rnew, "Render return", 0)
            || !sparkCompare(&a, &b, ea, eb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
    GFXMOCK_SetDistance(1);

    /* A chain of frames, which emits, ages and drops. */
    for (frame = 1; frame <= 6; frame++)
    {
        snprintf(name, sizeof(name), "Render frame chain x%d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.09f;

        GFXMOCK_Reset();
        sparkFill(&a, ea, SPARKS, 0, 0x28, 0x702u);
        for (i = 0; i < frame; i++)
            BASE_GFX_i_Spark_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        sparkFill(&b, eb, SPARKS, 0, 0x28, 0x702u);
        for (i = 0; i < frame; i++)
            NEW_GFX_i_Spark_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!sparkCompare(&a, &b, ea, eb))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

static void allocCases(void)
{
    tGfxSparkData* a;
    tGfxSparkData* b;
    char name[96];
    int wanted[5] = { 0, 1, 8, 24, 3 };
    int wi;

    GFXMOCK_SetCase("Create");
    GFXMOCK_Reset();
    a = BASE_GFX_Spark_Create();
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    b = NEW_GFX_Spark_Create();
    GFXMOCK_SaveSide(1);
    if (memcmp(a, b, sizeof(tGfxSparkData)) != 0)
    {
        printf("    DIFF Create: the two effects differ\n");
        GFXMOCK_fails++;
    }
    GFXMOCK_CompareSides();

    for (wi = 0; wi < 5; wi++)
    {
        snprintf(name, sizeof(name), "AllocSpark want=%d", wanted[wi]);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        a = BASE_GFX_Spark_Create();
        a->num = 12;
        BASE_GFX_AllocSpark(a);
        a->actNum = 10;
        a->num = wanted[wi];
        BASE_GFX_AllocSpark(a);
        BASE_GFX_Spark_Destroy(a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        b = NEW_GFX_Spark_Create();
        b->num = 12;
        NEW_GFX_AllocSpark(b);
        b->actNum = 10;
        b->num = wanted[wi];
        NEW_GFX_AllocSpark(b);
        NEW_GFX_Spark_Destroy(b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(a->actNum, b->actNum, "spark.actNum", 0)
            || !GFXMOCK_SameInt(a->allocdNum, b->allocdNum, "spark.allocdNum", 0)
            || !GFXMOCK_SameInt(a->num, b->num, "spark.num", 0))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    GFXMOCK_Init();

    generateCases();
    updateCases();
    renderCases();
    allocCases();

    GFXMOCK_Summary("gfxspark");
    return GFXMOCK_fails ? 1 : 0;
}
