#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "particle_mocks.h"
#include "../common.h"

/* The frozen copy and the rewrite, in one binary. PAG_Generate is not
   in the header, so it is declared here the way both copies define
   it. */
void BASE_PAG_Update(struct sGameObjectMain* gao, struct sPAGStruct* pag, float dt);
void NEW_PAG_Update(struct sGameObjectMain* gao, struct sPAGStruct* pag, float dt);
void BASE_PAG_Generate(tGameObjectMain* gao, tPAGStruct* pag, float dt);
void NEW_PAG_Generate(tGameObjectMain* gao, tPAGStruct* pag, float dt);
void BASE_PAG_RenderCommon(struct sGameObjectMain* gao, struct sPAGStruct* pag);
void NEW_PAG_RenderCommon(struct sGameObjectMain* gao, struct sPAGStruct* pag);
extern int BASE_PAGRenderingEnabled;
extern int NEW_PAGRenderingEnabled;

#define POOL 48

typedef struct sScene
{
    tPAGStruct pag;
    tPAGElemStruct elems[POOL];
} tScene;

static tScene s_template;
static tScene s_side;
static tScene s_baseState;
static tPMockCall s_baseLog[PMOCK_LOG_MAX];
static int s_baseLogLen;
static tPMockCapture s_baseCapture;

static unsigned s_fill = 1;

static float fillFloat(float lo, float hi)
{
    s_fill = s_fill * 1664525u + 1013904223u;
    return lo + (hi - lo) * (float)((s_fill >> 8) & 0xFFFF) / 65535.0f;
}

static void sceneBase(tScene* sc, unsigned seed)
{
    int i;

    s_fill = seed ? seed : 1u;
    memset(sc, 0, sizeof(*sc));
    sc->pag.pagElems = sc->elems;
    sc->pag.triangleNum = POOL;
    sc->pag.triangleNum2 = 0;
    sc->pag.color = (int)0xFF80C040u;
    sc->pag.num = 4;
    sc->pag.gao1 = &PMOCK_shapeGao;
    sc->pag.gaoForMatrixRef = NULL;

    sc->pag.ffb1 = 0.25f;
    sc->pag.ffb2 = 2.0f;
    sc->pag.ffb3 = 1.5f;
    sc->pag.ffb4 = 0.75f;
    sc->pag.ffb5 = 1.0f;
    sc->pag.ffb6 = 3.0f;
    sc->pag.ffb7 = 0.4f;
    sc->pag.ffb8 = 0.9f;
    sc->pag.ffb9 = 0.5f;
    sc->pag.ffb10 = 1.25f;
    sc->pag.ffb11 = 0.3f;
    sc->pag.ffb12 = 0.8f;
    sc->pag.ffb13 = 0.2f;
    sc->pag.ffb14 = 1.1f;
    sc->pag.ffb15 = 0.1f;
    sc->pag.ffb16 = 0.4f;
    sc->pag.ffb17 = 0.05f;
    sc->pag.fd21 = 0.3f;
    sc->pag.fd22 = 1.75f;

    sc->pag.v1.x = 0.1f;
    sc->pag.v1.y = -0.2f;
    sc->pag.v1.z = -9.81f;
    sc->pag.f1 = 0.96999997f;
    sc->pag.i3 = -1.0f;
    sc->pag.ff = 0.60000002f;
    sc->pag.ffa1 = 4.0f;
    sc->pag.ffa2 = 0.40000001f;
    sc->pag.ffa3 = 1.0f;
    sc->pag.ffa4 = 2.0f;
    sc->pag.ffa5 = -3.0f;
    sc->pag.ffa6 = 3.0f;
    sc->pag.ffa7 = 0.0f;
    sc->pag.ffa8 = 0.0f;
    sc->pag.ffa9 = 0.0f;

    for (i = 0; i < 16; i++)
    {
        sc->pag.generators[i].generatorID = i;
        sc->pag.generators[i].gao = NULL;
        sc->pag.generators[i].generatorFreq = 0.0f;
        sc->pag.generators[i].fff21 = 0.0f;
        sc->pag.generators[i].fff22 = 0.0f;
    }
}

static void fillPool(tScene* sc, int live)
{
    int i;

    for (i = 0; i < POOL; i++)
    {
        tPAGElemStruct* e = &sc->elems[i];

        e->particleState = (i % 3 == 0) ? 1 : (i % 3 == 1 ? 0 : 2);
        e->particlePos.x = fillFloat(-4.0f, 4.0f);
        e->particlePos.y = fillFloat(-4.0f, 4.0f);
        e->particlePos.z = fillFloat(-2.0f, 6.0f);
        e->v1.x = fillFloat(-2.0f, 2.0f);
        e->v1.y = fillFloat(-2.0f, 2.0f);
        e->v1.z = fillFloat(-3.0f, 3.0f);
        e->ff2 = fillFloat(0.0f, 0.3f);
        /* ff is one over the whole phase, not over what is left of
           it, so the fade factor ff2 * ff lands inside (0, 1). With
           ff the exact reciprocal of ff2 every factor is 1.0 to
           within a rounding step and scaling by it is invisible. */
        e->ff = 1.0f / (e->ff2 + fillFloat(0.05f, 0.5f));
        e->fff = fillFloat(0.0f, 2.0f);
        e->pagPosAndVertexIndexFloatBase = fillFloat(0.2f, 1.5f);
        e->ffff2 = fillFloat(0.2f, 1.0f);
        e->f5 = (unsigned int)(i * 0x08000000u) | (unsigned int)(i * 311u % 0x8000u)
                | ((i & 1) ? 0x8000u : 0u);
    }
    sc->pag.triangleNum2 = live;
    if (live > 3)
    {
        /* Two phase times sitting exactly on a step boundary, so that
           a < written as <= in the phase loop is visible. */
        sc->elems[1].ff2 = 0.0f;
        sc->elems[2].ff2 = 0.016666999f;
    }
}

/* A particle standing still exactly on the edge of the hole in the
   floor, so that the > in the hole test is not free to be a >=. */
static void poolHoleBoundary(tScene* sc)
{
    int i;

    for (i = 0; i < POOL; i++)
    {
        tPAGElemStruct* e = &sc->elems[i];

        e->particleState = 0;
        e->particlePos.x = (i & 1) ? 3.0f : 3.0001221f;
        e->particlePos.y = 4.0f;
        e->particlePos.z = -5.0f;
        e->v1.x = 0.0f;
        e->v1.y = 0.0f;
        e->v1.z = 0.0f;
        e->ff2 = 10.0f;
        e->ff = 0.1f;
        e->fff = 0.0f;
        e->pagPosAndVertexIndexFloatBase = 1.0f;
        e->ffff2 = 0.5f;
        e->f5 = 0;
    }
    sc->pag.v1.x = 0.0f;
    sc->pag.v1.y = 0.0f;
    sc->pag.v1.z = 0.0f;
    sc->pag.ffa9 = 25.0f;
    sc->pag.i3 = -1.0f;
    sc->pag.triangleNum2 = POOL;
}

/* ---- comparison ------------------------------------------------------ */

static void compareElem(const tPAGElemStruct* b, const tPAGElemStruct* n, const char* where)
{
    PMOCK_SameInt(b->particleState, n->particleState, "particleState", where);
    PMOCK_CloseFloat(b->particlePos.x, n->particlePos.x, "pos.x", where);
    PMOCK_CloseFloat(b->particlePos.y, n->particlePos.y, "pos.y", where);
    PMOCK_CloseFloat(b->particlePos.z, n->particlePos.z, "pos.z", where);
    PMOCK_CloseFloat(b->v1.x, n->v1.x, "vel.x", where);
    PMOCK_CloseFloat(b->v1.y, n->v1.y, "vel.y", where);
    PMOCK_CloseFloat(b->v1.z, n->v1.z, "vel.z", where);
    PMOCK_CloseFloat(b->ff, n->ff, "ff", where);
    PMOCK_CloseFloat(b->ff2, n->ff2, "ff2", where);
    PMOCK_CloseFloat(b->fff, n->fff, "age", where);
    PMOCK_CloseFloat(b->pagPosAndVertexIndexFloatBase, n->pagPosAndVertexIndexFloatBase,
                     "size", where);
    PMOCK_CloseFloat(b->ffff2, n->ffff2, "height", where);
    PMOCK_SameInt((int)b->f5, (int)n->f5, "f5", where);
}

static void compareScene(const tScene* b, const tScene* n, const char* where)
{
    int i;

    PMOCK_SameInt(b->pag.triangleNum2, n->pag.triangleNum2, "triangleNum2", where);
    PMOCK_SameInt(b->pag.renderCnt, n->pag.renderCnt, "renderCnt", where);
    PMOCK_CloseFloat(b->pag.timeElapsed, n->pag.timeElapsed, "timeElapsed", where);
    for (i = 0; i < 16; i++)
        PMOCK_CloseFloat(b->pag.generators[i].fff22, n->pag.generators[i].fff22,
                         "generator accumulator", where);
    for (i = 0; i < POOL; i++)
        compareElem(&b->elems[i], &n->elems[i], where);
}

static void compareCapture(const tPMockCapture* b, const tPMockCapture* n, const char* where)
{
    unsigned int i;

    if (PMOCK_SameInt(b->taken, n->taken, "render reached", where))
        return;
    if (!b->taken)
        return;
    PMOCK_SameInt((int)b->vertexNum, (int)n->vertexNum, "vertexNum", where);
    PMOCK_SameInt((int)b->uvNum, (int)n->uvNum, "uvNum", where);
    PMOCK_SameInt((int)b->spritesNum, (int)n->spritesNum, "spritesNum", where);
    PMOCK_SameInt((int)b->drawMask, (int)n->drawMask, "drawMask", where);
    PMOCK_SameInt(b->materialNum, n->materialNum, "materialNum", where);
    PMOCK_SameInt((int)b->triangleNum, (int)n->triangleNum, "triangleNum", where);
    if (b->vertexNum != n->vertexNum)
        return;
    for (i = 0; i < b->vertexNum && i < PMOCK_VERTS; i++)
    {
        PMOCK_CloseFloat(b->verts[i].x, n->verts[i].x, "vertex.x", where);
        PMOCK_CloseFloat(b->verts[i].y, n->verts[i].y, "vertex.y", where);
        PMOCK_CloseFloat(b->verts[i].z, n->verts[i].z, "vertex.z", where);
    }
    for (i = 0; i <= b->vertexNum && i < PMOCK_VERTS; i++)
        PMOCK_SameInt(b->colors[i], n->colors[i], "vertex colour", where);
    for (i = 0; i < b->uvNum && i < PMOCK_UVS; i++)
    {
        PMOCK_CloseFloat(b->uvs[i].u, n->uvs[i].u, "uv.u", where);
        PMOCK_CloseFloat(b->uvs[i].v, n->uvs[i].v, "uv.v", where);
    }
    for (i = 0; i < b->triangleNum && i < PMOCK_TRIS; i++)
    {
        PMOCK_SameInt(b->tris[i].v1, n->tris[i].v1, "tri.v1", where);
        PMOCK_SameInt(b->tris[i].v2, n->tris[i].v2, "tri.v2", where);
        PMOCK_SameInt(b->tris[i].v3, n->tris[i].v3, "tri.v3", where);
        PMOCK_SameInt(b->tris[i].uv1, n->tris[i].uv1, "tri.uv1", where);
        PMOCK_SameInt(b->tris[i].uv2, n->tris[i].uv2, "tri.uv2", where);
        PMOCK_SameInt(b->tris[i].uv3, n->tris[i].uv3, "tri.uv3", where);
    }
    for (i = 0; i < b->spritesNum && i < PMOCK_SPRITES; i++)
        PMOCK_SameInt((int)b->sprites[i].idx, (int)n->sprites[i].idx, "sprite word", where);
}

/* ---- the three entry points ------------------------------------------ */

enum { CALL_UPDATE, CALL_GENERATE, CALL_RENDER };

static void runSide(int which, int side, float dt, int steps)
{
    int i;

    s_side = s_template;
    s_side.pag.pagElems = s_side.elems;
    PMOCK_Reset();
    PMOCK_Seed(0x12345u);
    for (i = 0; i < steps; i++)
    {
        if (which == CALL_UPDATE)
        {
            if (side)
                NEW_PAG_Update(&PMOCK_gao, &s_side.pag, dt);
            else
                BASE_PAG_Update(&PMOCK_gao, &s_side.pag, dt);
        }
        else if (which == CALL_GENERATE)
        {
            if (side)
                NEW_PAG_Generate(&PMOCK_gao, &s_side.pag, dt);
            else
                BASE_PAG_Generate(&PMOCK_gao, &s_side.pag, dt);
        }
        else
        {
            if (side)
                NEW_PAG_RenderCommon(&PMOCK_gao, &s_side.pag);
            else
                BASE_PAG_RenderCommon(&PMOCK_gao, &s_side.pag);
        }
    }
}

static void runCase(const char* name, int which, float dt, int steps)
{
    PMOCK_SetCase(name);

    runSide(which, 0, dt, steps);
    s_baseState = s_side;
    PMOCK_SaveLog(s_baseLog, &s_baseLogLen);
    PMOCK_SaveCapture(&s_baseCapture);

    runSide(which, 1, dt, steps);

    compareScene(&s_baseState, &s_side, name);
    PMOCK_CompareLog(s_baseLog, s_baseLogLen, PMOCK_log, PMOCK_logLen, name);
    if (which == CALL_RENDER)
        compareCapture(&s_baseCapture, &PMOCK_capture, name);
}

/* ---- the sweeps ------------------------------------------------------- */

static const int updateFlags[] = {
    0, 0x400, 0x800, 0x1000, 0x2000, 0x8000,
    0x400 | 0x2000, 0x800 | 0x2000, 0x400 | 0x800,
    0x1000 | 0x2000 | 0x8000, 0x400 | 0x800 | 0x1000 | 0x2000 | 0x8000
};

static const float updateDts[] = { 0.0f, 0.0083330004f, 0.016666999f, 0.033329997f, 0.1f, 0.25f };

static void sweepUpdate(void)
{
    unsigned f;
    unsigned d;
    int live;
    int hole;
    int wiggle;
    char name[128];

    for (f = 0; f < sizeof(updateFlags) / sizeof(updateFlags[0]); f++)
        for (d = 0; d < sizeof(updateDts) / sizeof(updateDts[0]); d++)
            for (live = 0; live <= POOL; live += 16)
                for (hole = 0; hole < 2; hole++)
                    for (wiggle = 0; wiggle < 2; wiggle++)
                    {
                        sceneBase(&s_template, 0x2000u + f * 97u + d * 13u + live + hole);
                        fillPool(&s_template, live);
                        s_template.pag.flags = updateFlags[f];
                        s_template.pag.ffa9 = hole ? 2.0f : 0.0f;
                        s_template.pag.ffa7 = wiggle ? 3.0f : 0.0f;
                        s_template.pag.ffa8 = wiggle ? 1.5f : 0.0f;
                        sprintf(name, "update f=%X dt=%g live=%d hole=%d wiggle=%d",
                                updateFlags[f], (double)updateDts[d], live, hole, wiggle);
                        runCase(name, CALL_UPDATE, updateDts[d], 1);
                    }
}

static void sweepHoleBoundary(void)
{
    unsigned d;
    char name[128];

    for (d = 0; d < sizeof(updateDts) / sizeof(updateDts[0]); d++)
    {
        sceneBase(&s_template, 0x8000u + d);
        poolHoleBoundary(&s_template);
        s_template.pag.flags = 0x400;
        sprintf(name, "update hole boundary dt=%g", (double)updateDts[d]);
        runCase(name, CALL_UPDATE, updateDts[d], 1);
    }
}

static void sweepUpdateMulti(void)
{
    unsigned f;
    char name[128];

    for (f = 0; f < sizeof(updateFlags) / sizeof(updateFlags[0]); f++)
    {
        sceneBase(&s_template, 0x7000u + f);
        fillPool(&s_template, POOL);
        s_template.pag.flags = updateFlags[f];
        sprintf(name, "update x30 f=%X", updateFlags[f]);
        runCase(name, CALL_UPDATE, 0.016666999f, 30);
    }
}

static void sweepGenerate(void)
{
    int mode;
    int subMode;
    int withGro;
    int withStp;
    int rot;
    int already;
    char name[160];

    for (mode = 0; mode <= 7; mode++)
        for (subMode = 0; subMode <= 6; subMode++)
            for (withGro = 0; withGro < 2; withGro++)
                for (withStp = 0; withStp < 2; withStp++)
                    for (rot = 0; rot < 2; rot++)
                        for (already = 0; already < 2; already++)
                        {
                            if ((mode != 6 && mode != 4) && withStp)
                                continue;
                            /* subMode 4 reads the mesh normals at an
                               index only mode 6 sets, and through a
                               pointer only mode 6 guarantees. Left
                               out because the frozen copy faults. */
                            if (subMode == 4 && (mode != 6 || !withGro))
                                continue;
                            sceneBase(&s_template, 0x3000u + mode * 211u + subMode * 37u
                                      + withGro * 7u + withStp * 3u + rot);
                            fillPool(&s_template, 0);
                            s_template.pag.triangleNum2 = already ? POOL - 3 : 0;
                            s_template.pag.mode = (uint8_t)mode;
                            s_template.pag.subMode = (uint8_t)subMode;
                            s_template.pag.flags = rot ? 0x1000 : 4;
                            s_template.pag.generators[0].generatorFreq = 20.0f;
                            s_template.pag.generators[1].generatorFreq = 0.0f;
                            s_template.pag.generators[2].generatorFreq = 7.0f;
                            s_template.pag.generators[2].gao = &PMOCK_genGao;
                            s_template.pag.generators[3].generatorFreq = 1000.0f;
                            PMOCK_groForGao1 = withGro ? &PMOCK_gro : NULL;
                            PMOCK_groId = 1;
                            PMOCK_haveStp = withStp;
                            PMOCK_stpHasStp = withStp;
                            sprintf(name,
                                    "generate mode=%d sub=%d gro=%d stp=%d rot=%d used=%d",
                                    mode, subMode, withGro, withStp, rot, already);
                            runCase(name, CALL_GENERATE, 0.1f, 1);
                        }
    PMOCK_groForGao1 = &PMOCK_gro;
    PMOCK_haveStp = 0;
}

static void sweepGenerateExtras(void)
{
    int fadeIn;
    int zeroLife;
    int matrixRef;
    int badGro;
    char name[160];

    for (fadeIn = 0; fadeIn < 2; fadeIn++)
        for (zeroLife = 0; zeroLife < 2; zeroLife++)
            for (matrixRef = 0; matrixRef < 2; matrixRef++)
                for (badGro = 0; badGro < 2; badGro++)
                {
                    sceneBase(&s_template, 0x4000u + fadeIn * 31u + zeroLife * 11u
                              + matrixRef * 5u + badGro);
                    fillPool(&s_template, 0);
                    s_template.pag.triangleNum2 = 0;
                    s_template.pag.mode = 6;
                    s_template.pag.subMode = 4;
                    s_template.pag.flags = 0x1000;
                    s_template.pag.fd21 = fadeIn ? 0.3f : 0.0f;
                    if (zeroLife)
                    {
                        s_template.pag.ffb13 = 0.0f;
                        s_template.pag.ffb14 = 0.0f;
                    }
                    s_template.pag.gaoForMatrixRef = matrixRef ? &PMOCK_genGao : NULL;
                    s_template.pag.generators[0].generatorFreq = 30.0f;
                    PMOCK_groForGao1 = &PMOCK_gro;
                    PMOCK_groId = badGro ? 4 : 1;
                    sprintf(name, "generate extras fade=%d zerolife=%d mref=%d badgro=%d",
                            fadeIn, zeroLife, matrixRef, badGro);
                    runCase(name, CALL_GENERATE, 0.1f, 1);
                }
    PMOCK_groId = 1;
}

static const int renderFlags[] = {
    0x40,
    0x40 | 0x100,
    0x40 | 0x100 | 2,
    0x40 | 0x200,
    0x40 | 0x100 | 0x200,
    0x40 | 8,
    0x40 | 8 | 2,
    0x40 | 0x10,
    0x40 | 0x20,
    0x40 | 8 | 0x20,
    0x40 | 0x10000,
    0x40 | 0x10000 | 1,
    0x40 | 1,
    0x40 | 4,
    0x40 | 0x80,
    0x40 | 0x100 | 0x200 | 8 | 0x10 | 2,
    0,
};

static void sweepRender(void)
{
    unsigned f;
    int sprites;
    int uniform;
    int spin;
    int live;
    char name[160];

    for (f = 0; f < sizeof(renderFlags) / sizeof(renderFlags[0]); f++)
        for (sprites = 0; sprites < 2; sprites++)
            for (uniform = 0; uniform < 2; uniform++)
                for (spin = 0; spin < 2; spin++)
                    for (live = 1; live <= 9; live += 8)
                    {
                        sceneBase(&s_template, 0x5000u + f * 83u + sprites * 17u
                                  + uniform * 5u + spin + live);
                        fillPool(&s_template, live);
                        s_template.pag.flags = renderFlags[f];
                        if (uniform)
                        {
                            s_template.pag.ffb10 = s_template.pag.ffb9;
                            s_template.pag.ffb12 = s_template.pag.ffb11;
                        }
                        if (!spin)
                        {
                            int i;

                            for (i = 0; i < POOL; i++)
                                s_template.elems[i].f5 = 0;
                        }
                        s_template.pag.renderCnt = 0;
                        BASE_PAGRenderingEnabled = sprites ? 0 : 1;
                        NEW_PAGRenderingEnabled = sprites ? 0 : 1;
                        ENG_gb_EngineRunning = 0;
                        sprintf(name, "render f=%X sprites=%d uniform=%d spin=%d live=%d",
                                renderFlags[f], sprites, uniform, spin, live);
                        runCase(name, CALL_RENDER, 0.0f, 1);
                    }
}

static void sweepRenderDriving(void)
{
    int sprites;
    int running;
    char name[128];

    for (sprites = 0; sprites < 2; sprites++)
        for (running = 0; running < 2; running++)
        {
            sceneBase(&s_template, 0x6000u + sprites * 3u + running);
            fillPool(&s_template, 6);
            s_template.pag.flags = 0x40 | 0x100 | 0x1000;
            s_template.pag.mode = 2;
            s_template.pag.subMode = 1;
            s_template.pag.generators[0].generatorFreq = 25.0f;
            s_template.pag.renderCnt = 0;
            BASE_PAGRenderingEnabled = sprites ? 0 : 1;
            NEW_PAGRenderingEnabled = sprites ? 0 : 1;
            ENG_gb_EngineRunning = running;
            TIM_gf_dt = 0.05f;
            sprintf(name, "render driving sprites=%d running=%d", sprites, running);
            runCase(name, CALL_RENDER, 0.0f, 1);
        }
    ENG_gb_EngineRunning = 0;
    TIM_gf_dt = 0.0f;
}

int main(void)
{
    PMOCK_Init();
    PMOCK_groForGao1 = &PMOCK_gro;

    /* Restoring MATH_AddScaleVector, MATH_VecMultScalar and
       MATH_RandFloat moves the last bits: the frozen copy keeps the
       inlined arithmetic in the decompiler's doubles where the
       helpers round to float at every step. Integer state, the call
       sequence and every index are still compared exactly. */
    PMOCK_tolAbs = 1e-5f;
    PMOCK_tolRel = 5e-5f;

    sweepUpdate();
    sweepHoleBoundary();
    sweepUpdateMulti();
    sweepGenerate();
    sweepGenerateExtras();
    sweepRender();
    sweepRenderDriving();

    PMOCK_Summary("particlegenerator.c");
    if (PMOCK_logOverflow)
        printf("  WARNING: the call log overflowed\n");
    return PMOCK_failures ? 1 : 0;
}
