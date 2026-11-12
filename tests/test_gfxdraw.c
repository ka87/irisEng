#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../memory.h"
#include "gfx_mocks.h"
#include "../gfx/gfxdiaphragm.h"
#include "../gfx/gfxfade.h"
#include "../gfx/gfxline.h"
#include "../gfx/gfxtable.h"
#include "../gfx/gfxspeed.h"
#include "../gfx/gfxflare.h"

struct sGfxDiaphragmData* BASE_GFX_Diaphragm_Create(void);
struct sGfxDiaphragmData* NEW_GFX_Diaphragm_Create(void);
int BASE_GFX_i_Diaphragm_Render(struct sGfxDiaphragmData* iris);
int NEW_GFX_i_Diaphragm_Render(struct sGfxDiaphragmData* iris);

int BASE_GFX_i_Fade_Render(struct sGfxFadeData* fade);
int NEW_GFX_i_Fade_Render(struct sGfxFadeData* fade);
struct sGfxFadeData* BASE_GFX_Fade_Create(void);
struct sGfxFadeData* NEW_GFX_Fade_Create(void);

void BASE_GFX_Line_Render(struct sGfxLineData* line);
void NEW_GFX_Line_Render(struct sGfxLineData* line);
struct sGfxLineData* BASE_GFX_Line_Create(void);
struct sGfxLineData* NEW_GFX_Line_Create(void);

int BASE_GFX_i_Table_Render(struct sGfxTableData* table);
int NEW_GFX_i_Table_Render(struct sGfxTableData* table);
void BASE_GFX_Table_Setv(struct sGfxTableData* table, int id, Vec3* value);
void NEW_GFX_Table_Setv(struct sGfxTableData* table, int id, Vec3* value);
struct sGfxTableData* BASE_GFX_Table_Create(void);
struct sGfxTableData* NEW_GFX_Table_Create(void);

int BASE_GFX_i_Speed_Render(struct sGfxSpeedData* speed);
int NEW_GFX_i_Speed_Render(struct sGfxSpeedData* speed);
struct sGfxSpeedData* BASE_GFX_Speed_Create(void);
struct sGfxSpeedData* NEW_GFX_Speed_Create(void);

struct sGfxFlareData* BASE_GFX_Flare_Create(int withSubFlares);
struct sGfxFlareData* NEW_GFX_Flare_Create(int withSubFlares);
int BASE_GFX_i_Flare_TestDepth(tGfxFlareData* flare);
int NEW_GFX_i_Flare_TestDepth(tGfxFlareData* flare);
void BASE_GFX_Flare_Test(tGfxFlareData* flare);
void NEW_GFX_Flare_Test(tGfxFlareData* flare);
void BASE_GFX_Flare_Render(struct sGfxFlareData* flare);
void NEW_GFX_Flare_Render(struct sGfxFlareData* flare);

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

/* ------------------------------------------------------------------ */
/* diaphragm: the stage walk, the blade ring and the corner stitching  */
/* ------------------------------------------------------------------ */

static void irisFill(tGfxDiaphragmData* iris, float f0, float f1, float f2,
                     float f3, float elapsed, float cx, float cy)
{
    memset(iris, 0, sizeof(*iris));
    iris->color = (int)0xFF40A0C0u;
    iris->f01[0] = f0;
    iris->f01[1] = f1;
    iris->f01[2] = f2;
    iris->f01[3] = f3;
    iris->elapsedTime = elapsed;
    iris->v1 = vec3(cx, cy, 0.0f);
}

static int irisCompare(tGfxDiaphragmData* a, tGfxDiaphragmData* b)
{
    int ok = 1;

    ok &= GFXMOCK_SameInt(a->color, b->color, "iris.color", 0);
    ok &= GFXMOCK_Close(a->elapsedTime, b->elapsedTime, "iris.elapsedTime", 0);
    ok &= GFXMOCK_CloseVec(&a->v1, &b->v1, "iris.v1");
    return ok;
}

static void diaphragmCases(void)
{
    tGfxDiaphragmData a;
    tGfxDiaphragmData b;
    char name[96];
    float elapsed[9] = { 0.0f, 0.05f, 0.2f, 0.5f, 0.75f, 1.0f, 1.4f, 1.9f, 3.0f };
    float centres[4][2] = { { 0.5f, 0.5f }, { 0.0f, 0.0f }, { 1.0f, 1.0f },
                            { 0.25f, 0.8f } };
    int ei, ci, frame;
    int rbase, rnew;

    for (ei = 0; ei < 9; ei++)
        for (ci = 0; ci < 4; ci++)
        {
            snprintf(name, sizeof(name), "diaphragm elapsed=%g centre=%g,%g",
                     elapsed[ei], centres[ci][0], centres[ci][1]);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.05f;

            GFXMOCK_Reset();
            irisFill(&a, 0.1f, 0.4f, 0.5f, 0.4f, elapsed[ei],
                     centres[ci][0], centres[ci][1]);
            rbase = BASE_GFX_i_Diaphragm_Render(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            irisFill(&b, 0.1f, 0.4f, 0.5f, 0.4f, elapsed[ei],
                     centres[ci][0], centres[ci][1]);
            rnew = NEW_GFX_i_Diaphragm_Render(&b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "diaphragm return", 0)
                || !irisCompare(&a, &b))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* Zero-length stages, which is where the stage walk can run off the
       end of the four. */
    {
        float zeroes[4][4] = { { 0.0f, 0.0f, 0.0f, 0.0f },
                               { 0.0f, 1.0f, 0.0f, 1.0f },
                               { 1.0f, 0.0f, 1.0f, 0.0f },
                               { 0.2f, 0.0f, 0.2f, 0.2f } };
        int zi;

        for (zi = 0; zi < 4; zi++)
            for (ei = 0; ei < 9; ei++)
            {
                snprintf(name, sizeof(name), "diaphragm stages %d elapsed=%g",
                         zi, elapsed[ei]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = 0.05f;

                GFXMOCK_Reset();
                irisFill(&a, zeroes[zi][0], zeroes[zi][1], zeroes[zi][2],
                         zeroes[zi][3], elapsed[ei], 0.5f, 0.5f);
                rbase = BASE_GFX_i_Diaphragm_Render(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                irisFill(&b, zeroes[zi][0], zeroes[zi][1], zeroes[zi][2],
                         zeroes[zi][3], elapsed[ei], 0.5f, 0.5f);
                rnew = NEW_GFX_i_Diaphragm_Render(&b);
                GFXMOCK_SaveSide(1);

                if (!GFXMOCK_SameInt(rbase, rnew, "diaphragm return", 0)
                    || !irisCompare(&a, &b))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }
    }

    /* A viewport that is not at the origin, which moves every corner. */
    GFXMOCK_SetCase("diaphragm offset viewport");
    TIM_gf_dt = 0.05f;
    GFXMOCK_Reset();
    gpst_DX_stDD->ct.cam.viewPortRealPosX = 32;
    gpst_DX_stDD->ct.cam.viewPortRealPosY = 24;
    irisFill(&a, 0.1f, 0.4f, 0.5f, 0.4f, 0.3f, 0.5f, 0.5f);
    rbase = BASE_GFX_i_Diaphragm_Render(&a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    gpst_DX_stDD->ct.cam.viewPortRealPosX = 32;
    gpst_DX_stDD->ct.cam.viewPortRealPosY = 24;
    irisFill(&b, 0.1f, 0.4f, 0.5f, 0.4f, 0.3f, 0.5f, 0.5f);
    rnew = NEW_GFX_i_Diaphragm_Render(&b);
    GFXMOCK_SaveSide(1);
    if (!GFXMOCK_SameInt(rbase, rnew, "diaphragm return", 0) || !irisCompare(&a, &b))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    /* Several frames, so the stage walk advances on its own clock. */
    for (frame = 1; frame <= 8; frame++)
    {
        snprintf(name, sizeof(name), "diaphragm frame chain x%d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.2f;

        GFXMOCK_Reset();
        irisFill(&a, 0.1f, 0.4f, 0.5f, 0.4f, 0.0f, 0.5f, 0.5f);
        for (ei = 0; ei < frame; ei++)
            BASE_GFX_i_Diaphragm_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        irisFill(&b, 0.1f, 0.4f, 0.5f, 0.4f, 0.0f, 0.5f, 0.5f);
        for (ei = 0; ei < frame; ei++)
            NEW_GFX_i_Diaphragm_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!irisCompare(&a, &b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

/* ------------------------------------------------------------------ */
/* fade: the five-stage walk, including a NaN that falls through all    */
/* five, and the colour blend                                          */
/* ------------------------------------------------------------------ */

static void fadeCases(void)
{
    tGfxFadeData a;
    tGfxFadeData b;
    char name[96];
    float elapsed[11] = { 0.0f, 0.05f, 0.2f, 0.45f, 0.6f, 0.9f, 1.1f, 1.6f,
                          2.5f, -1.0f, 0.0f };
    int ei, frame;
    int rbase, rnew;

    for (ei = 0; ei < 11; ei++)
    {
        snprintf(name, sizeof(name), "fade elapsed=%g", elapsed[ei]);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.05f;

        GFXMOCK_Reset();
        memset(&a, 0, sizeof(a));
        a.color1 = 0x102030;
        a.color2 = (int)0xFFE0C0A0u;
        a.factors[0] = 0.1f;
        a.factors[1] = 0.3f;
        a.factors[2] = 0.4f;
        a.factors[3] = 0.3f;
        a.factors[4] = 0.2f;
        a.elapsedTime = elapsed[ei];
        a.f = 0.055f;
        if (ei == 10)
            a.elapsedTime = (float)(0.0 / 0.0);
        b = a;
        rbase = BASE_GFX_i_Fade_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        rnew = NEW_GFX_i_Fade_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(rbase, rnew, "fade return", 0)
            || !GFXMOCK_Close(a.elapsedTime, b.elapsedTime, "fade.elapsedTime", 0))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* Zero-length stages. */
    GFXMOCK_SetCase("fade zero stages");
    TIM_gf_dt = 0.05f;
    GFXMOCK_Reset();
    memset(&a, 0, sizeof(a));
    a.color1 = 0x112233;
    a.color2 = 0x445566;
    a.f = 0.055f;
    b = a;
    rbase = BASE_GFX_i_Fade_Render(&a);
    GFXMOCK_SaveSide(0);
    GFXMOCK_Reset();
    rnew = NEW_GFX_i_Fade_Render(&b);
    GFXMOCK_SaveSide(1);
    if (!GFXMOCK_SameInt(rbase, rnew, "fade return", 0))
        GFXMOCK_fails++;
    GFXMOCK_CompareSides();

    GFXMOCK_SetCase("fade create");
    GFXMOCK_Reset();
    {
        tGfxFadeData* pa;
        tGfxFadeData* pb;

        pa = BASE_GFX_Fade_Create();
        GFXMOCK_SaveSide(0);
        GFXMOCK_Reset();
        pb = NEW_GFX_Fade_Create();
        GFXMOCK_SaveSide(1);
        if (memcmp(pa, pb, sizeof(tGfxFadeData)) != 0)
        {
            printf("    DIFF fade create: the two effects differ\n");
            GFXMOCK_fails++;
        }
        GFXMOCK_CompareSides();
    }

    for (frame = 1; frame <= 8; frame++)
    {
        snprintf(name, sizeof(name), "fade frame chain x%d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.2f;

        GFXMOCK_Reset();
        memset(&a, 0, sizeof(a));
        a.color1 = 0;
        a.color2 = (int)0xFFFFFFFFu;
        a.factors[0] = 0.1f;
        a.factors[1] = 0.3f;
        a.factors[2] = 0.4f;
        a.factors[3] = 0.3f;
        a.factors[4] = 0.2f;
        a.f = 0.055f;
        b = a;
        for (ei = 0; ei < frame; ei++)
            BASE_GFX_i_Fade_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        for (ei = 0; ei < frame; ei++)
            NEW_GFX_i_Fade_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_Close(a.elapsedTime, b.elapsedTime, "fade.elapsedTime", 0))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }
}

/* ------------------------------------------------------------------ */
/* line: the nearest-point clamp, including the NaN that must fall to   */
/* the first arm, and the degenerate normal                            */
/* ------------------------------------------------------------------ */

static void lineCases(void)
{
    tGfxLineData a;
    tGfxLineData b;
    char name[96];
    Vec3 camPos[6];
    int ci, di;

    camPos[0] = vec3(0.0f, 0.0f, 0.0f);
    camPos[1] = vec3(-50.0f, -50.0f, 0.0f);
    camPos[2] = vec3(50.0f, 50.0f, 0.0f);
    camPos[3] = vec3(0.0f, 5.0f, 10.0f);
    camPos[4] = vec3(-10.0f, 0.0f, 0.0f);
    camPos[5] = vec3(10.0f, 10.0f, 0.0f);

    for (ci = 0; ci < 6; ci++)
        for (di = 0; di < 3; di++)
        {
            snprintf(name, sizeof(name), "line cam=%d shape=%d", ci, di);
            GFXMOCK_SetCase(name);

            GFXMOCK_Reset();
            GFXMOCK_SetCameraPos(camPos[ci].x, camPos[ci].y, camPos[ci].z);
            gpst_DX_stDD->ct.cam.objectMtx.Pos = camPos[ci];
            memset(&a, 0, sizeof(a));
            a.v1 = vec3(-10.0f, 0.0f, 0.0f);
            a.v2 = vec3(10.0f, 10.0f, 0.0f);
            a.v3 = vec3(0.0f, 0.0f, 1.0f);
            a.f = 0.1f;
            a.vertexColor = (int)0xFF00FF00u;
            if (di == 1)
                a.v2 = a.v1;                  /* zero length: NaN along */
            else if (di == 2)
                a.v2 = vec3(10.0f, 0.0f, 0.0f); /* in the camera plane */
            b = a;
            BASE_GFX_Line_Render(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            GFXMOCK_SetCameraPos(camPos[ci].x, camPos[ci].y, camPos[ci].z);
            gpst_DX_stDD->ct.cam.objectMtx.Pos = camPos[ci];
            NEW_GFX_Line_Render(&b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_CloseVec(&a.v1, &b.v1, "line.v1")
                || !GFXMOCK_CloseVec(&a.v2, &b.v2, "line.v2"))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    GFXMOCK_SetCase("line create");
    GFXMOCK_Reset();
    {
        tGfxLineData* pa;
        tGfxLineData* pb;

        pa = BASE_GFX_Line_Create();
        GFXMOCK_SaveSide(0);
        GFXMOCK_Reset();
        pb = NEW_GFX_Line_Create();
        GFXMOCK_SaveSide(1);
        if (memcmp(pa, pb, sizeof(tGfxLineData)) != 0)
        {
            printf("    DIFF line create: the two effects differ\n");
            GFXMOCK_fails++;
        }
        GFXMOCK_CompareSides();
    }
    gpst_DX_stDD->ct.cam.objectMtx.Pos = vec3(0.0f, 0.0f, 0.0f);
}

/* ------------------------------------------------------------------ */
/* table: the row and column walk, the colour index lookup and the      */
/* setters with their off-by-one bound                                 */
/* ------------------------------------------------------------------ */

static void tableFill(tGfxTableData* table, int rows, int cols, unsigned seed)
{
    int i;

    rngSeed(seed);
    memset(table, 0, sizeof(*table));
    table->num1 = (char)rows;
    table->num2 = (char)cols;
    table->ff1 = 0.1f;
    table->ff2 = 0.15f;
    table->ff3 = 0.06f;
    for (i = 0; i < 10; i++)
    {
        table->colors[i] = (int)(0xFF000000u | (unsigned)(i * 0x112233));
        table->f[i] = rngFloat(0.02f, 0.2f);
        table->f2[i] = rngFloat(0.02f, 0.2f);
    }
    for (i = 0; i < 100; i++)
        table->vertexColorIndexes[i] = (char)(i % 10);
}

/* The frozen copy writes each cell's four colours at the indices
   4n+1, 4n, 4n-1 and 4n-2 -- backwards from its cursor -- so cell zero
   writes two ints in front of the colour array and every later cell
   overwrites its neighbour's. The rewrite writes 4n+1 to 4n+4, which is
   where the four vertices of cell n keep their colours, so the two
   arrays cannot be compared. Instead the rewritten side is checked
   against what the geometry asks for, directly. */
static int tableColoursAsGeometryWants(tGfxTableData* table)
{
    unsigned int* colors;
    int cells;
    int expected;
    int rows;
    int cols;
    int row;
    int col;
    int n;
    int k;
    int ok = 1;

    rows = table->num1;
    cols = table->num2;
    cells = rows * cols;
    if (!cells)
        return 1;

    colors = GFX_gpst_Geo->vertexColors;
    if (!GFXMOCK_SameInt((int)colors[0], 4 * cells, "table colour count", 0))
        ok = 0;
    for (row = 0; row < rows; row++)
        for (col = 0; col < cols; col++)
        {
            n = row * cols + col;
            expected = table->colors[(int)table->vertexColorIndexes[10 * row + col]];
            for (k = 0; k < 4; k++)
                if (!GFXMOCK_SameInt(expected, (int)colors[4 * n + 1 + k],
                                     "table cell colour", 4 * n + 1 + k))
                    ok = 0;
        }
    return ok;
}

static void tableCases(void)
{
    tGfxTableData a;
    tGfxTableData b;
    char name[96];
    int shapes[6][2] = { { 0, 0 }, { 1, 1 }, { 1, 10 }, { 10, 1 }, { 4, 7 },
                         { 10, 10 } };
    int si, i;
    int rbase, rnew;

    GFXMOCK_ignoreColors = 1;
    for (si = 0; si < 6; si++)
    {
        snprintf(name, sizeof(name), "table %dx%d", shapes[si][0], shapes[si][1]);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        tableFill(&a, shapes[si][0], shapes[si][1], 0x900u);
        rbase = BASE_GFX_i_Table_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        tableFill(&b, shapes[si][0], shapes[si][1], 0x900u);
        rnew = NEW_GFX_i_Table_Render(&b);
        if (!tableColoursAsGeometryWants(&b))
            GFXMOCK_fails++;
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_SameInt(rbase, rnew, "table return", 0)
            || memcmp(&a, &b, sizeof(tGfxTableData)) != 0)
        {
            printf("    DIFF %s: the two tables differ\n", name);
            GFXMOCK_fails++;
        }
        GFXMOCK_CompareSides();
    }

    /* The three Setv cases, including the index of ten that the bound
       lets through and the sign checks. */
    {
        float idx[7] = { -1.0f, 0.0f, 3.0f, 9.0f, 10.0f, 11.0f, -5.0f };
        int ii, jj;
        Vec3 v;

        for (ii = 0; ii < 7; ii++)
        {
            snprintf(name, sizeof(name), "table setv index=%g", idx[ii]);
            GFXMOCK_SetCase(name);

            GFXMOCK_Reset();
            tableFill(&a, 10, 10, 0x901u);
            v = vec3(idx[ii], 0.33f, 0.0f);
            BASE_GFX_Table_Setv(&a, 7201, &v);
            BASE_GFX_Table_Setv(&a, 7202, &v);
            for (jj = 0; jj < 7; jj++)
            {
                v = vec3(idx[ii], idx[jj], (float)(jj * 40));
                BASE_GFX_Table_Setv(&a, 7203, &v);
            }
            v = vec3(1.0f, 1.0f, 1.0f);
            BASE_GFX_Table_Setv(&a, 7204, &v);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            tableFill(&b, 10, 10, 0x901u);
            v = vec3(idx[ii], 0.33f, 0.0f);
            NEW_GFX_Table_Setv(&b, 7201, &v);
            NEW_GFX_Table_Setv(&b, 7202, &v);
            for (jj = 0; jj < 7; jj++)
            {
                v = vec3(idx[ii], idx[jj], (float)(jj * 40));
                NEW_GFX_Table_Setv(&b, 7203, &v);
            }
            v = vec3(1.0f, 1.0f, 1.0f);
            NEW_GFX_Table_Setv(&b, 7204, &v);
            GFXMOCK_SaveSide(1);

            if (memcmp(&a, &b, sizeof(tGfxTableData)) != 0)
            {
                printf("    DIFF %s: the two tables differ\n", name);
                for (i = 0; i < 10; i++)
                {
                    GFXMOCK_Close(a.f[i], b.f[i], "table.f", i);
                    GFXMOCK_Close(a.f2[i], b.f2[i], "table.f2", i);
                }
                GFXMOCK_fails++;
            }
            GFXMOCK_CompareSides();
        }
    }

    GFXMOCK_ignoreColors = 0;

    GFXMOCK_SetCase("table create");
    GFXMOCK_Reset();
    {
        tGfxTableData* pa;
        tGfxTableData* pb;

        pa = BASE_GFX_Table_Create();
        GFXMOCK_SaveSide(0);
        GFXMOCK_Reset();
        pb = NEW_GFX_Table_Create();
        GFXMOCK_SaveSide(1);
        if (memcmp(pa, pb, sizeof(tGfxTableData)) != 0)
        {
            printf("    DIFF table create: the two effects differ\n");
            GFXMOCK_fails++;
        }
        GFXMOCK_CompareSides();
    }
}

/* ------------------------------------------------------------------ */
/* speed: the first-frame sentinel, the behind-camera exit and the      */
/* trail quad                                                          */
/* ------------------------------------------------------------------ */

static void speedCases(void)
{
    tGfxSpeedData a;
    tGfxSpeedData b;
    char name[96];
    float remaining[4] = { 1.0f, 0.05f, 0.0f, -0.1f };
    float prevZ[4] = { 1.0f, 0.0f, -1.0f, 0.0f };
    int ri, zi, sentinel, frame;
    int rbase, rnew;

    for (ri = 0; ri < 4; ri++)
        for (zi = 0; zi < 4; zi++)
            for (sentinel = 0; sentinel < 2; sentinel++)
            {
                snprintf(name, sizeof(name), "speed life=%g prevZ=%g sentinel=%d",
                         remaining[ri], prevZ[zi], sentinel);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = 0.05f;

                GFXMOCK_Reset();
                memset(&a, 0, sizeof(a));
                a.v1 = vec3(3.0f, -1.0f, 2.0f);
                a.timeRemaining = remaining[ri];
                a.i2 = 1.5f;
                a.i3 = (int)0xFF806040u;
                if (sentinel)
                    *(int32_t*)&a.v2.x = -1;
                else
                    a.v2 = vec3(1.0f, 1.0f, prevZ[zi]);
                if (zi == 3 && !sentinel)
                    a.v2.z = (float)(0.0 / 0.0);
                b = a;
                rbase = BASE_GFX_i_Speed_Render(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                rnew = NEW_GFX_i_Speed_Render(&b);
                GFXMOCK_SaveSide(1);

                if (!GFXMOCK_SameInt(rbase, rnew, "speed return", 0)
                    || !GFXMOCK_Close(a.timeRemaining, b.timeRemaining,
                                      "speed.timeRemaining", 0)
                    || !GFXMOCK_CloseVec(&a.v2, &b.v2, "speed.v2"))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    for (frame = 1; frame <= 5; frame++)
    {
        snprintf(name, sizeof(name), "speed frame chain x%d", frame);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.05f;

        GFXMOCK_Reset();
        memset(&a, 0, sizeof(a));
        a.v1 = vec3(3.0f, -1.0f, 2.0f);
        a.timeRemaining = 1.0f;
        a.i2 = 1.5f;
        a.i3 = -1;
        *(int32_t*)&a.v2.x = -1;
        b = a;
        for (ri = 0; ri < frame; ri++)
        {
            a.v1.x = a.v1.x + 0.5f;
            BASE_GFX_i_Speed_Render(&a);
        }
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        for (ri = 0; ri < frame; ri++)
        {
            b.v1.x = b.v1.x + 0.5f;
            NEW_GFX_i_Speed_Render(&b);
        }
        GFXMOCK_SaveSide(1);

        if (!GFXMOCK_Close(a.timeRemaining, b.timeRemaining, "speed.timeRemaining", 0)
            || !GFXMOCK_CloseVec(&a.v1, &b.v1, "speed.v1")
            || !GFXMOCK_CloseVec(&a.v2, &b.v2, "speed.v2"))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    GFXMOCK_SetCase("speed create");
    GFXMOCK_Reset();
    {
        tGfxSpeedData* pa;
        tGfxSpeedData* pb;

        pa = BASE_GFX_Speed_Create();
        GFXMOCK_SaveSide(0);
        GFXMOCK_Reset();
        pb = NEW_GFX_Speed_Create();
        GFXMOCK_SaveSide(1);
        if (memcmp(pa, pb, sizeof(tGfxSpeedData)) != 0)
        {
            printf("    DIFF speed create: the two effects differ\n");
            GFXMOCK_fails++;
        }
        GFXMOCK_CompareSides();
    }
}

/* ------------------------------------------------------------------ */
/* flare: the depth test and the sub-flare chain.                      */
/*                                                                     */
/* The nine-sample window of the depth test is left out, and that is    */
/* not a gap in the rewrite. It reads one float in front of its own     */
/* table, so each of the two copies in the binary reads whatever its    */
/* own linker put there; the values are not the same memory and a       */
/* comparison of them would mean nothing. Every other path is here.     */
/* ------------------------------------------------------------------ */

static void flareFill(tGfxFlareData* flare, int flags, float factor,
                      float divider, unsigned seed)
{
    rngSeed(seed);
    memset(flare, 0, sizeof(*flare));
    flare->basic.v1 = vec3(4.0f, 2.0f, 20.0f);
    flare->basic.v2 = vec3(0.5f, -0.25f, 0.0f);
    flare->basic.v3 = vec3(4.0f, 2.0f, 20.0f);
    flare->basic.unkFactor = 1.0f;
    flare->basic.flags = flags;
    flare->basic.colorFactor = factor;
    flare->basic.timeColorFactorDivider = divider;
    flare->basic.i11 = 3.0f;
    flare->basic.i12 = 0.0f;
    flare->basic.materialNum2 = 2;
    flare->basic.color1 = 0x204080;
    flare->basic.color2 = (int)0xFFC0B0A0u;

    flare->i22 = rngFloat(0.1f, 1.0f);
    flare->i23 = rngFloat(0.1f, 1.0f);
    flare->i24 = rngFloat(0.1f, 1.0f);
    flare->i25 = rngFloat(0.1f, 1.0f);
    flare->i26 = rngFloat(0.1f, 1.0f);
    flare->i27 = rngFloat(0.1f, 1.0f);
    flare->i28 = rngFloat(0.1f, 1.0f);
    flare->i29 = rngFloat(0.1f, 1.0f);
    flare->materialNum1 = 3;
    flare->i39 = 4;
    flare->i40 = 5;
    flare->i41 = 6;
    flare->i42 = 7;
    flare->i43 = 8;
    flare->i44 = 9;
    flare->i45 = 10;
    flare->i46 = 11;
    flare->color3 = (int)0xFF112233u;
    flare->i48 = (int)0xFF223344u;
    flare->i49 = (int)0xFF334455u;
    flare->i50 = (int)0xFF445566u;
    flare->i51 = (int)0xFF556677u;
    flare->i52 = (int)0xFF667788u;
    flare->i53 = (int)0xFF778899u;
    flare->i54 = (int)0xFF8899AAu;
    flare->i55 = (int)0xFF99AABBu;
}

static int flareCompare(tGfxFlareData* a, tGfxFlareData* b)
{
    int ok = 1;

    ok &= GFXMOCK_SameInt(a->basic.flags, b->basic.flags, "flare.flags", 0);
    ok &= GFXMOCK_Close(a->basic.colorFactor, b->basic.colorFactor, "flare.colorFactor", 0);
    ok &= GFXMOCK_Close(a->basic.i12, b->basic.i12, "flare.i12", 0);
    ok &= GFXMOCK_CloseVec(&a->basic.v1, &b->basic.v1, "flare.v1");
    ok &= GFXMOCK_CloseVec(&a->basic.v2, &b->basic.v2, "flare.v2");
    ok &= GFXMOCK_CloseVec(&a->basic.v3, &b->basic.v3, "flare.v3");
    return ok;
}

static void flareCases(void)
{
    tGfxFlareData a;
    tGfxFlareData b;
    char name[96];
    /* 0x200 and 0x300 pick the nine-sample window, so they are absent
       for the reason given above. */
    int testFlags[6] = { 0x00, 0x10, 0x100, 0x110, 0x18, 0x118 };
    /* The sub-flare flag is absent here and has cases of its own: the
       frozen copy reads the nine sub-flares by stepping a cursor of the
       whole effect's type, so eight of the nine come out of memory the
       effect does not own and there is nothing to compare against. */
    int renderFlags[7] = { 0x00, 0x01, 0x04, 0x05, 0x08, 0x09, 0x0A };
    float factors[3] = { 0.0f, 0.5f, 1.0f };
    float divs[2] = { 0.0f, 0.5f };
    Vec3 moved[3];
    int fi, ci, di, mi, dist, pass;
    int rbase, rnew;

    moved[0] = vec3(4.0f, 2.0f, 20.0f);
    moved[1] = vec3(4.1f, 2.0f, 20.0f);
    moved[2] = vec3(9.0f, 2.0f, 20.0f);

    for (fi = 0; fi < 6; fi++)
        for (mi = 0; mi < 3; mi++)
        {
            snprintf(name, sizeof(name), "flare depth flags=0x%X moved=%d",
                     testFlags[fi], mi);
            GFXMOCK_SetCase(name);

            GFXMOCK_Reset();
            flareFill(&a, testFlags[fi], 1.0f, 0.5f, 0xA00u);
            a.basic.v3 = moved[mi];
            rbase = BASE_GFX_i_Flare_TestDepth(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            flareFill(&b, testFlags[fi], 1.0f, 0.5f, 0xA00u);
            b.basic.v3 = moved[mi];
            rnew = NEW_GFX_i_Flare_TestDepth(&b);
            GFXMOCK_SaveSide(1);

            if (!GFXMOCK_SameInt(rbase, rnew, "flare depth return", 0)
                || !flareCompare(&a, &b))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }

    /* A flare behind the camera, and one off the side of the viewport. */
    {
        Vec3 places[4];
        int pi;

        places[0] = vec3(4.0f, 2.0f, 20.0f);
        places[1] = vec3(4.0f, 2.0f, -20.0f);
        places[2] = vec3(4000.0f, 2.0f, 20.0f);
        places[3] = vec3(4.0f, -4000.0f, 20.0f);

        for (pi = 0; pi < 4; pi++)
            for (fi = 0; fi < 6; fi++)
            {
                snprintf(name, sizeof(name), "flare depth place=%d flags=0x%X",
                         pi, testFlags[fi]);
                GFXMOCK_SetCase(name);

                GFXMOCK_Reset();
                flareFill(&a, testFlags[fi], 1.0f, 0.5f, 0xA01u);
                a.basic.v1 = places[pi];
                a.basic.v3 = places[pi];
                rbase = BASE_GFX_i_Flare_TestDepth(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                flareFill(&b, testFlags[fi], 1.0f, 0.5f, 0xA01u);
                b.basic.v1 = places[pi];
                b.basic.v3 = places[pi];
                rnew = NEW_GFX_i_Flare_TestDepth(&b);
                GFXMOCK_SaveSide(1);

                if (!GFXMOCK_SameInt(rbase, rnew, "flare depth return", 0)
                    || !flareCompare(&a, &b))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }
    }

    /* Flare_Test, which is the depth test behind a distance check. */
    for (dist = 0; dist < 2; dist++)
        for (fi = 0; fi < 6; fi++)
        {
            snprintf(name, sizeof(name), "flare test distance=%d flags=0x%X",
                     dist, testFlags[fi]);
            GFXMOCK_SetCase(name);

            GFXMOCK_Reset();
            GFXMOCK_SetDistance(dist);
            flareFill(&a, testFlags[fi] | 8, 1.0f, 0.5f, 0xA02u);
            BASE_GFX_Flare_Test(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            GFXMOCK_SetDistance(dist);
            flareFill(&b, testFlags[fi] | 8, 1.0f, 0.5f, 0xA02u);
            NEW_GFX_Flare_Test(&b);
            GFXMOCK_SaveSide(1);

            if (!flareCompare(&a, &b))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }
    GFXMOCK_SetDistance(1);

    /* The render: the single quad, the nine sub-flares, the fade-out
       and the deletion when it runs out. */
    for (fi = 0; fi < 7; fi++)
        for (ci = 0; ci < 3; ci++)
            for (di = 0; di < 2; di++)
            {
                snprintf(name, sizeof(name),
                         "flare render flags=0x%X factor=%g div=%g",
                         renderFlags[fi], factors[ci], divs[di]);
                GFXMOCK_SetCase(name);
                TIM_gf_dt = 0.05f;

                GFXMOCK_Reset();
                flareFill(&a, renderFlags[fi], factors[ci], divs[di], 0xA10u);
                BASE_GFX_Flare_Render(&a);
                GFXMOCK_SaveSide(0);

                GFXMOCK_Reset();
                flareFill(&b, renderFlags[fi], factors[ci], divs[di], 0xA10u);
                NEW_GFX_Flare_Render(&b);
                GFXMOCK_SaveSide(1);

                if (!flareCompare(&a, &b))
                    GFXMOCK_fails++;
                GFXMOCK_CompareSides();
            }

    /* Distance zero, and the presentation flag that reads the cached
       occlusion bit instead of testing again. */
    for (dist = 0; dist < 2; dist++)
        for (fi = 0; fi < 7; fi++)
        {
            snprintf(name, sizeof(name), "flare render distance=%d flags=0x%X",
                     dist, renderFlags[fi]);
            GFXMOCK_SetCase(name);
            TIM_gf_dt = 0.05f;

            GFXMOCK_Reset();
            GFXMOCK_SetDistance(dist);
            gpst_DX_stDD->presentationFlags = 0x20000000;
            flareFill(&a, renderFlags[fi] | 8, 1.0f, 0.5f, 0xA11u);
            BASE_GFX_Flare_Render(&a);
            GFXMOCK_SaveSide(0);

            GFXMOCK_Reset();
            GFXMOCK_SetDistance(dist);
            gpst_DX_stDD->presentationFlags = 0x20000000;
            flareFill(&b, renderFlags[fi] | 8, 1.0f, 0.5f, 0xA11u);
            NEW_GFX_Flare_Render(&b);
            GFXMOCK_SaveSide(1);

            if (!flareCompare(&a, &b))
                GFXMOCK_fails++;
            GFXMOCK_CompareSides();
        }
    GFXMOCK_SetDistance(1);
    gpst_DX_stDD->presentationFlags = 0;

    /* The fade-out over several frames, which is where the factor walks
       down and the flare finally deletes itself. */
    for (pass = 1; pass <= 8; pass++)
    {
        snprintf(name, sizeof(name), "flare fade out x%d", pass);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.1f;

        GFXMOCK_Reset();
        flareFill(&a, 0x4000009, 1.0f, 0.3f, 0xA12u);
        a.basic.v3 = vec3(100.0f, 100.0f, 100.0f);
        for (fi = 0; fi < pass; fi++)
            BASE_GFX_Flare_Render(&a);
        GFXMOCK_SaveSide(0);

        GFXMOCK_Reset();
        flareFill(&b, 0x4000009, 1.0f, 0.3f, 0xA12u);
        b.basic.v3 = vec3(100.0f, 100.0f, 100.0f);
        for (fi = 0; fi < pass; fi++)
            NEW_GFX_Flare_Render(&b);
        GFXMOCK_SaveSide(1);

        if (!flareCompare(&a, &b))
            GFXMOCK_fails++;
        GFXMOCK_CompareSides();
    }

    /* The sub-flare chain, checked against what the four runs of nine
       fields hold rather than against the frozen copy. */
    for (ci = 0; ci < 3; ci++)
    {
        const int* subColor;
        const int* subMaterial;
        int expected;
        int ok;
        int k;

        snprintf(name, sizeof(name), "flare sub-flares factor=%g", factors[ci]);
        GFXMOCK_SetCase(name);
        TIM_gf_dt = 0.05f;

        GFXMOCK_Reset();
        flareFill(&b, 0x2000002, factors[ci], 0.5f, 0xA20u);
        NEW_GFX_Flare_Render(&b);

        ok = GFXMOCK_SameInt(GFXMOCK_capCount, 9, "flare sub-flare renders", 0);
        subColor = &b.color3;
        subMaterial = &b.materialNum1;
        for (k = 0; k < 9 && k < GFXMOCK_capCount; k++)
        {
            if (!GFXMOCK_SameInt(subMaterial[k], GFXMOCK_caps[k].materialNum[0],
                                 "flare sub-flare material", k))
                ok = 0;
            if (b.basic.colorFactor == 1.0f || (b.basic.flags & 2) == 0)
                expected = subColor[k];
            else
                expected = (int)LIGHT_ul_Interpol2Colors(0, subColor[k],
                                                         b.basic.colorFactor);
            if (GFXMOCK_caps[k].colors
                && !GFXMOCK_SameInt(expected, (int)GFXMOCK_caps[k].colors[1],
                                    "flare sub-flare colour", k))
                ok = 0;
        }
        if (!ok)
        {
            printf("  FAIL %s\n", name);
            GFXMOCK_fails++;
        }
    }

    /* Create, with and without the sub-flare block. */
    for (fi = 0; fi < 2; fi++)
    {
        tGfxFlareData* pa;
        tGfxFlareData* pb;

        snprintf(name, sizeof(name), "flare create sub=%d", fi);
        GFXMOCK_SetCase(name);

        GFXMOCK_Reset();
        pa = BASE_GFX_Flare_Create(fi);
        GFXMOCK_SaveSide(0);
        GFXMOCK_Reset();
        pb = NEW_GFX_Flare_Create(fi);
        GFXMOCK_SaveSide(1);
        if (memcmp(pa, pb, fi ? sizeof(tGfxFlareData) : sizeof(tGfxFlareDataBasic)) != 0)
        {
            printf("    DIFF %s: the two effects differ\n", name);
            GFXMOCK_fails++;
        }
        GFXMOCK_CompareSides();
    }
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    GFXMOCK_Init();

    diaphragmCases();
    fadeCases();
    lineCases();
    tableCases();
    speedCases();
    flareCases();

    GFXMOCK_Summary("gfxdiaphragm / gfxfade / gfxline / gfxtable / gfxspeed / gfxflare");
    return GFXMOCK_fails ? 1 : 0;
}
