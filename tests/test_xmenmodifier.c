#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine_mocks.h"

void BASE_GAO_ModifierXMEN_DRAW(tXmenMdfStruct* xmen);
void NEW_GAO_ModifierXMEN_DRAW(tXmenMdfStruct* xmen);
void BASE_GAO_ModifierXMEN_GetBonePosFromNum(tGameObjectMain* gao, int multiPartDataID,
                                             Vec3* posRet, Vec3* mtxVecRetByType,
                                             unsigned int type, Vec3* offsetVec);
void NEW_GAO_ModifierXMEN_GetBonePosFromNum(tGameObjectMain* gao, int multiPartDataID,
                                            Vec3* posRet, Vec3* mtxVecRetByType,
                                            unsigned int type, Vec3* offsetVec);
void BASE_GAO_ModifierXMEN_SMOOTH_BUFFER(Vec3* buff, unsigned int passNum);
void NEW_GAO_ModifierXMEN_SMOOTH_BUFFER(Vec3* buff, unsigned int passNum);
int BASE_GAO_ModifierXMEN_Load(struct sModifier* mdf, char* data);
int NEW_GAO_ModifierXMEN_Load(struct sModifier* mdf, char* data);
void BASE_GAO_ModifierXMEN_Reinit(tModifier* mdf);
void NEW_GAO_ModifierXMEN_Reinit(tModifier* mdf);
void BASE_XMEN_OnOff(struct sGameObjectMain* gao, int id, int onOff);
void NEW_XMEN_OnOff(struct sGameObjectMain* gao, int id, int onOff);
void BASE_XMEN_SetSize(struct sGameObjectMain* gao, int id, int multipartDataID, float size);
void NEW_XMEN_SetSize(struct sGameObjectMain* gao, int id, int multipartDataID, float size);
extern float BASE_xmenSmoothBuffer_flt_9684BC;
extern float NEW_xmenSmoothBuffer_flt_9684BC;
void BASE_XMEN_FirstInit(void);
void NEW_XMEN_FirstInit(void);

#define MAXENTRY 3
#define MAXPART 4
#define MAXMDF 3

static int g_fails;
static int g_diffCount;
static double g_worstAbs;
static double g_worstRel;
static const char* g_worstWhat = "-";
static char g_worstCase[96] = "-";
static char g_case[96] = "-";

static void setCase(const char* name)
{
    strncpy(g_case, name ? name : "-", sizeof(g_case) - 1);
    g_case[sizeof(g_case) - 1] = 0;
}

static unsigned s_rngState;

static void rngSeed(unsigned seed)
{
    s_rngState = seed ? seed : 1u;
}

static unsigned rngNext(void)
{
    s_rngState = s_rngState * 1664525u + 1013904223u;
    return s_rngState >> 8;
}

static float rngFloat(float lo, float hi)
{
    return lo + (hi - lo) * (float)(rngNext() & 0xFFFF) / 65535.0f;
}

static int closeEnough(float a, float b, const char* what, int idx)
{
    double d, r, m;

    if (memcmp(&a, &b, sizeof(float)) == 0)
        return 1;
    d = fabs((double)a - (double)b);
    m = fabs((double)a) > fabs((double)b) ? fabs((double)a) : fabs((double)b);
    r = m > 0.0 ? d / m : d;
    g_diffCount++;
    if (d > g_worstAbs)
        g_worstAbs = d;
    if (r > g_worstRel)
    {
        g_worstRel = r;
        g_worstWhat = what;
        strcpy(g_worstCase, g_case);
    }
    if (r > 1e-4 && d > 1e-4)
    {
        printf("    DIFF %s[%d]: old %.9g  new %.9g  (rel %.3g)\n",
               what, idx, (double)a, (double)b, r);
        return 0;
    }
    return 1;
}

static int cmpVecs(Vec3* a, Vec3* b, int n, const char* what)
{
    int ok = 1;
    int i;

    for (i = 0; i < n; i++)
    {
        ok &= closeEnough(a[i].x, b[i].x, what, 3 * i);
        ok &= closeEnough(a[i].y, b[i].y, what, 3 * i + 1);
        ok &= closeEnough(a[i].z, b[i].z, what, 3 * i + 2);
    }
    return ok;
}

static int cmpLogs(tMockCall* la, int na, tMockCall* lb, int nb)
{
    int i, j, ok = 1;

    if (na != nb)
    {
        printf("    DIFF call count: old %d new %d\n", na, nb);
        for (i = 0; i < (na > nb ? na : nb); i++)
            printf("      %-36s | %s\n",
                   i < na ? la[i].fn : "-",
                   i < nb ? lb[i].fn : "-");
        return 0;
    }
    for (i = 0; i < na; i++)
    {
        if (strcmp(la[i].fn, lb[i].fn) != 0)
        {
            printf("    DIFF call %d: old %s new %s\n", i, la[i].fn, lb[i].fn);
            ok = 0;
            continue;
        }
        for (j = 0; j < la[i].nargs; j++)
            ok &= closeEnough(la[i].arg[j], lb[i].arg[j], la[i].fn, i);
    }
    return ok;
}

static int cmpCaptures(tXmenCapture* a, int na, tXmenCapture* b, int nb)
{
    int ok = 1;
    int i, t;

    if (na != nb)
    {
        printf("    DIFF draw count: old %d new %d\n", na, nb);
        return 0;
    }
    for (i = 0; i < na; i++)
    {
        if (a[i].vertexNum != b[i].vertexNum || a[i].uvNum != b[i].uvNum
            || a[i].triangleNum != b[i].triangleNum
            || a[i].materialNum != b[i].materialNum
            || a[i].transparent != b[i].transparent)
        {
            printf("    DIFF draw %d counts: old %d/%d/%d/%d/%d new %d/%d/%d/%d/%d\n", i,
                   a[i].vertexNum, a[i].uvNum, a[i].triangleNum, a[i].materialNum,
                   a[i].transparent,
                   b[i].vertexNum, b[i].uvNum, b[i].triangleNum, b[i].materialNum,
                   b[i].transparent);
            ok = 0;
        }
        for (t = 0; t < 64; t++)
        {
            if (memcmp(&a[i].tris[t], &b[i].tris[t], sizeof(tGeoTriangleChunkElem)) != 0)
            {
                printf("    DIFF draw %d tri %d\n", i, t);
                ok = 0;
                break;
            }
        }
        for (t = 0; t < 64; t++)
        {
            ok &= closeEnough(a[i].uvs[t].u, b[i].uvs[t].u, "uv.u", t);
            ok &= closeEnough(a[i].uvs[t].v, b[i].uvs[t].v, "uv.v", t);
        }
        ok &= cmpVecs(a[i].verts, b[i].verts, 64, "drawn verts");
    }
    return ok;
}

static void report(const char* name, int ok)
{
    setCase("-");
    if (ok)
    {
        printf("  ok   %s\n", name);
    }
    else
    {
        printf("  FAIL %s\n", name);
        g_fails++;
    }
}

static void fillMtx(Mtx4x4* m)
{
    m->v1.x = rngFloat(-1.0f, 1.0f);
    m->v1.y = rngFloat(-1.0f, 1.0f);
    m->v1.z = rngFloat(-1.0f, 1.0f);
    m->v2.x = rngFloat(-1.0f, 1.0f);
    m->v2.y = rngFloat(-1.0f, 1.0f);
    m->v2.z = rngFloat(-1.0f, 1.0f);
    m->v3.x = rngFloat(-1.0f, 1.0f);
    m->v3.y = rngFloat(-1.0f, 1.0f);
    m->v3.z = rngFloat(-1.0f, 1.0f);
    m->Pos.x = rngFloat(-30.0f, 30.0f);
    m->Pos.y = rngFloat(-30.0f, 30.0f);
    m->Pos.z = rngFloat(-30.0f, 30.0f);
    m->_m14 = 1.0f;
    m->_m24 = 1.0f;
    m->_m34 = 1.0f;
    m->w = 1.0f;
    m->mtxflag = 2;
}

/* --- the scene --------------------------------------------------- */

typedef struct sScene
{
    tXmenMdfStruct xmen;
    tXmenMdfInner2 trail[MAXENTRY];
    tXmenSize sizes[MAXENTRY];
    int ptr2[8 * MAXENTRY];

    tGameObjectMain gao;
    tGameObjectBasic basic;
    tGameObjectMultiPartData parts;
    tMultiPartGameObjectLinks links[MAXPART];
    Mtx4x4 partMtx[MAXPART];
    tGameObjectMain partGao[MAXPART];
    Mtx4x4 partGaoMtx[MAXPART];
    Mtx4x4 gaoMtx;

    tGameObjectExtended extended;
    tModifier mdf[MAXMDF];
    tModifierInterface intf[MAXMDF];
    tXmenMdfStruct other[MAXMDF];

    tGdi_DisplayData dd;
    Mtx4x4 camMtx;
    Mtx4x4 viewMtx;
} tScene;

static void sceneSetViewMatrix(Mtx4x4* mtx)
{
    MOCK_Record("DD.SetViewMatrix", mtx, 0);
}

static void sceneBuild(tScene* s, unsigned seed, int entries, int flags, int xmenType,
                       int partFlags, int linkMode)
{
    int i, k;

    memset(s, 0, sizeof(*s));
    rngSeed(seed);

    fillMtx(&s->gaoMtx);
    fillMtx(&s->camMtx);
    fillMtx(&s->viewMtx);

    s->gao.flags = partFlags;
    s->gao.basic = &s->basic;
    s->gao.matrices = &s->gaoMtx;
    s->gao.extended = &s->extended;
    s->basic.multiPartData = &s->parts;
    s->parts.elemNum = MAXPART;

    for (i = 0; i < MAXPART; i++)
    {
        fillMtx(&s->partMtx[i]);
        fillMtx(&s->partGaoMtx[i]);
        s->partGao[i].matrices = &s->partGaoMtx[i];
        s->partGao[i].flags = (linkMode == 2) ? 0x1000000 : 0;
        s->links[i].mtxIdx = (linkMode == 0) ? -1 : i;
        s->links[i].mtxPtr = &s->partMtx[i];
        s->links[i].gao = (linkMode == 3) ? NULL : &s->partGao[i];
    }
    s->parts.multipartGaoObjectPartsOrMatrices =
        (partFlags & 0x1000000) ? (void*)s->links : (void*)s->partMtx;

    s->xmen.flags = flags;
    s->xmen.num = entries;
    s->xmen.num2 = 0;
    s->xmen.xmenType = xmenType;
    s->xmen.sizes = s->sizes;
    s->xmen.ptr1 = s->trail;
    s->xmen.ptr2 = s->ptr2;
    s->xmen.gao = &s->gao;
    s->xmen.geomat = 0;
    s->xmen.deathTime = 0.25f;
    s->xmen.time = 0.0f;
    s->xmen.id = 11;
    s->xmen.frameCounter = 0;

    for (i = 0; i < entries; i++)
    {
        s->sizes[i].multipartDataID = i % (MAXPART + 1) - 1;
        s->sizes[i].size = rngFloat(0.2f, 4.0f);
        for (k = 0; k < 16; k++)
        {
            s->trail[i].data[k].x = rngFloat(-20.0f, 20.0f);
            s->trail[i].data[k].y = rngFloat(-20.0f, 20.0f);
            s->trail[i].data[k].z = rngFloat(-20.0f, 20.0f);
        }
    }

    for (i = 0; i < MAXMDF; i++)
    {
        s->intf[i].mdfId = (i == 1) ? 7 : 9;
        s->mdf[i].modifierIntf = &s->intf[i];
        s->mdf[i].mdfPrivateDataPtr = &s->other[i];
        s->mdf[i].modifierFlags = 0x1000000;
        s->mdf[i].nextElem = (i + 1 < MAXMDF) ? &s->mdf[i + 1] : NULL;
        s->other[i].id = (i == 2) ? 12 : 11;
        s->other[i].num = MAXENTRY;
        s->other[i].sizes = s->sizes;
    }
    s->extended.mdf = &s->mdf[0];

    s->dd.SetViewMatrix = sceneSetViewMatrix;
    s->dd.ct.cam.cameraMtx = s->camMtx;
    s->dd.ct.cam.objectMtx = s->viewMtx;
    s->dd.matrixStack.actMatrixElem = &s->viewMtx;
}

/* --- DRAW -------------------------------------------------------- */

static void runDraw(const char* name, unsigned seed, int entries, int flags, int xmenType,
                    int partFlags, int linkMode, int frames, float dt, int breakFrame)
{
    tScene* a = (tScene*)malloc(sizeof(tScene));
    tScene* b = (tScene*)malloc(sizeof(tScene));
    tMockCall logA[MOCK_LOG_MAX];
    tXmenCapture* capA;
    int nA, nCapA, f, ok = 1;

    capA = (tXmenCapture*)malloc(sizeof(MOCK_xmenCaptures));

    sceneBuild(a, seed, entries, flags, xmenType, partFlags, linkMode);
    sceneBuild(b, seed, entries, flags, xmenType, partFlags, linkMode);
    setCase(name);

    TIM_gf_dt = dt;
    gpst_DX_stDD = &a->dd;
    g_ulFrameCounter = 100;
    MOCK_Reset();
    MOCK_xmenCaptureCount = 0;
    for (f = 0; f < frames; f++)
    {
        a->xmen.frameCounter = (f == breakFrame) ? 0 : g_ulFrameCounter - 1;
        BASE_GAO_ModifierXMEN_DRAW(&a->xmen);
        g_ulFrameCounter++;
    }
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);
    nCapA = MOCK_xmenCaptureCount;
    memcpy(capA, MOCK_xmenCaptures, sizeof(MOCK_xmenCaptures));

    gpst_DX_stDD = &b->dd;
    g_ulFrameCounter = 100;
    MOCK_Reset();
    MOCK_xmenCaptureCount = 0;
    for (f = 0; f < frames; f++)
    {
        b->xmen.frameCounter = (f == breakFrame) ? 0 : g_ulFrameCounter - 1;
        NEW_GAO_ModifierXMEN_DRAW(&b->xmen);
        g_ulFrameCounter++;
    }

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpCaptures(capA, nCapA, MOCK_xmenCaptures, MOCK_xmenCaptureCount);
    ok &= cmpVecs(a->trail[0].data, b->trail[0].data, 16 * MAXENTRY, "trail");
    if (a->xmen.num2 != b->xmen.num2)
    {
        printf("    DIFF num2: old %d new %d\n", a->xmen.num2, b->xmen.num2);
        ok = 0;
    }
    ok &= closeEnough(a->xmen.time, b->xmen.time, "time", 0);
    ok &= closeEnough(a->xmen.deathTime, b->xmen.deathTime, "deathTime", 0);

    report(name, ok);
    free(a);
    free(b);
    free(capA);
}

/* --- GetBonePosFromNum ------------------------------------------- */

static void runBonePos(const char* name, unsigned seed, int id, unsigned int type,
                       int partFlags, int linkMode, int noParts, int noBasic, int nullMtxPtr)
{
    tScene* a = (tScene*)malloc(sizeof(tScene));
    tScene* b = (tScene*)malloc(sizeof(tScene));
    tMockCall logA[MOCK_LOG_MAX];
    Vec3 posA, vecA, posB, vecB, offset;
    int nA, i, ok = 1;

    sceneBuild(a, seed, 1, 0, 0, partFlags, linkMode);
    sceneBuild(b, seed, 1, 0, 0, partFlags, linkMode);
    setCase(name);

    if (noParts)
    {
        a->basic.multiPartData = NULL;
        b->basic.multiPartData = NULL;
    }
    if (noBasic)
    {
        a->gao.basic = NULL;
        b->gao.basic = NULL;
    }
    if (nullMtxPtr)
    {
        for (i = 0; i < MAXPART; i++)
        {
            a->links[i].mtxPtr = NULL;
            b->links[i].mtxPtr = NULL;
        }
    }

    rngSeed(seed ^ 0x3333u);
    offset.x = rngFloat(-10.0f, 10.0f);
    offset.y = rngFloat(-10.0f, 10.0f);
    offset.z = rngFloat(-10.0f, 10.0f);

    memset(&posA, 0x5a, sizeof(posA));
    memset(&vecA, 0x5a, sizeof(vecA));
    posB = posA;
    vecB = vecA;

    gpst_DX_stDD = &a->dd;
    MOCK_Reset();
    BASE_GAO_ModifierXMEN_GetBonePosFromNum(&a->gao, id, &posA, &vecA, type, &offset);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    gpst_DX_stDD = &b->dd;
    MOCK_Reset();
    NEW_GAO_ModifierXMEN_GetBonePosFromNum(&b->gao, id, &posB, &vecB, type, &offset);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpVecs(&posA, &posB, 1, "pos");
    ok &= cmpVecs(&vecA, &vecB, 1, "vec");
    report(name, ok);
    free(a);
    free(b);
}

/* --- SMOOTH_BUFFER ----------------------------------------------- */

static void runSmooth(const char* name, unsigned seed, unsigned int passNum, float weight)
{
    Vec3 bufA[64];
    Vec3 bufB[64];
    int i, ok = 1;

    rngSeed(seed);
    for (i = 0; i < 64; i++)
    {
        bufA[i].x = rngFloat(-20.0f, 20.0f);
        bufA[i].y = rngFloat(-20.0f, 20.0f);
        bufA[i].z = rngFloat(-20.0f, 20.0f);
    }
    memcpy(bufB, bufA, sizeof(bufA));
    setCase(name);

    BASE_xmenSmoothBuffer_flt_9684BC = weight;
    NEW_xmenSmoothBuffer_flt_9684BC = weight;
    BASE_GAO_ModifierXMEN_SMOOTH_BUFFER(bufA, passNum);
    NEW_GAO_ModifierXMEN_SMOOTH_BUFFER(bufB, passNum);

    ok &= cmpVecs(bufA, bufB, 64, "smoothed");
    report(name, ok);
}

/* --- Load, Reinit, OnOff, SetSize -------------------------------- */

static void runLoad(const char* name, unsigned version)
{
    tScene* a = (tScene*)malloc(sizeof(tScene));
    tScene* b = (tScene*)malloc(sizeof(tScene));
    tModifier mdfA;
    tModifier mdfB;
    unsigned char stream[160];
    unsigned i;
    int ra, rb, ok = 1;

    sceneBuild(a, 5u, 2, 0, 0, 0, 0);
    sceneBuild(b, 5u, 2, 0, 0, 0, 0);
    setCase(name);

    rngSeed(version + 3u);
    for (i = 0; i < sizeof(stream); i++)
        stream[i] = (unsigned char)(rngNext() & 0x3f);
    memcpy(stream + 8, &version, sizeof(version));
    i = 2;
    memcpy(stream + 16, &i, sizeof(i));         /* entry count */

    memset(&mdfA, 0, sizeof(mdfA));
    memset(&mdfB, 0, sizeof(mdfB));
    mdfA.mdfPrivateDataPtr = &a->xmen;
    mdfB.mdfPrivateDataPtr = &b->xmen;
    mdfA.gao = &a->gao;
    mdfB.gao = &b->gao;

    MOCK_Reset();
    ra = BASE_GAO_ModifierXMEN_Load(&mdfA, (char*)stream);
    rb = NEW_GAO_ModifierXMEN_Load(&mdfB, (char*)stream);

    if (ra != rb)
    {
        printf("    DIFF bytes read: old %d new %d\n", ra, rb);
        ok = 0;
    }
    if (a->xmen.flags != b->xmen.flags || a->xmen.num != b->xmen.num
        || a->xmen.xmenType != b->xmen.xmenType || a->xmen.geoKey != b->xmen.geoKey
        || a->xmen.id != b->xmen.id || a->xmen.num2 != b->xmen.num2)
    {
        printf("    DIFF loaded fields\n");
        ok = 0;
    }
    ok &= closeEnough(a->xmen.deathTime, b->xmen.deathTime, "deathTime", 0);
    for (i = 0; i < (unsigned)a->xmen.num && i < (unsigned)b->xmen.num; i++)
    {
        if (a->xmen.sizes[i].multipartDataID != b->xmen.sizes[i].multipartDataID)
        {
            printf("    DIFF sizes[%u].multipartDataID\n", i);
            ok = 0;
        }
        ok &= closeEnough(a->xmen.sizes[i].size, b->xmen.sizes[i].size, "sizes.size", (int)i);
    }
    report(name, ok);
    free(a);
    free(b);
}

static void runReinit(const char* name, unsigned seed, int flags, int haveGeomat, int entries)
{
    tScene* a = (tScene*)malloc(sizeof(tScene));
    tScene* b = (tScene*)malloc(sizeof(tScene));
    tModifier mdfA;
    tModifier mdfB;
    int ok = 1;

    sceneBuild(a, seed, entries, flags, 0, 0, 0);
    sceneBuild(b, seed, entries, flags, 0, 0, 0);
    setCase(name);

    memset(&mdfA, 0, sizeof(mdfA));
    memset(&mdfB, 0, sizeof(mdfB));
    mdfA.mdfPrivateDataPtr = &a->xmen;
    mdfB.mdfPrivateDataPtr = &b->xmen;
    a->xmen.num = entries;
    b->xmen.num = entries;
    if (!haveGeomat)
    {
        a->xmen.geomat = NULL;
        b->xmen.geomat = NULL;
    }

    MOCK_Reset();
    BASE_GAO_ModifierXMEN_Reinit(&mdfA);
    NEW_GAO_ModifierXMEN_Reinit(&mdfB);

    if (a->xmen.flags != b->xmen.flags || mdfA.modifierFlags != mdfB.modifierFlags
        || (a->xmen.ptr1 == NULL) != (b->xmen.ptr1 == NULL)
        || (a->xmen.ptr2 == NULL) != (b->xmen.ptr2 == NULL))
    {
        printf("    DIFF reinit state\n");
        ok = 0;
    }
    if (entries && a->xmen.ptr1 && b->xmen.ptr1)
        ok &= cmpVecs(a->trail[0].data, b->trail[0].data, 16 * entries, "trail");
    report(name, ok);
    free(a);
    free(b);
}

static void runOnOff(const char* name, unsigned seed, int id, int onOff, int haveSkeleton,
                     int nullGao)
{
    tScene* a = (tScene*)malloc(sizeof(tScene));
    tScene* b = (tScene*)malloc(sizeof(tScene));
    int i, ok = 1;

    sceneBuild(a, seed, 2, 0, 0, 0, 0);
    sceneBuild(b, seed, 2, 0, 0, 0, 0);
    setCase(name);

    /* no actor object, so the scan covers the object itself only */
    if (haveSkeleton)
    {
        a->gao.flags |= 2;
        b->gao.flags |= 2;
    }

    MOCK_Reset();
    BASE_XMEN_OnOff(nullGao ? NULL : &a->gao, id, onOff);
    NEW_XMEN_OnOff(nullGao ? NULL : &b->gao, id, onOff);

    for (i = 0; i < MAXMDF; i++)
    {
        if (a->mdf[i].modifierFlags != b->mdf[i].modifierFlags)
        {
            printf("    DIFF mdf[%d].modifierFlags: old %#x new %#x\n",
                   i, a->mdf[i].modifierFlags, b->mdf[i].modifierFlags);
            ok = 0;
        }
    }
    report(name, ok);
    free(a);
    free(b);
}

static void runSetSize(const char* name, unsigned seed, int id, int partId, float size,
                       int nullGao)
{
    tScene* a = (tScene*)malloc(sizeof(tScene));
    tScene* b = (tScene*)malloc(sizeof(tScene));
    int i, ok = 1;

    sceneBuild(a, seed, MAXENTRY, 0, 0, 0, 0);
    sceneBuild(b, seed, MAXENTRY, 0, 0, 0, 0);
    setCase(name);

    MOCK_Reset();
    BASE_XMEN_SetSize(nullGao ? NULL : &a->gao, id, partId, size);
    NEW_XMEN_SetSize(nullGao ? NULL : &b->gao, id, partId, size);

    for (i = 0; i < MAXENTRY; i++)
        ok &= closeEnough(a->sizes[i].size, b->sizes[i].size, "sizes.size", i);
    report(name, ok);
    free(a);
    free(b);
}

int main(void)
{
    char name[96];
    unsigned int type;
    int mode;

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("xmenmodifier: old against new\n");

    BASE_XMEN_FirstInit();
    NEW_XMEN_FirstInit();

    for (type = 0; type < 8; type++)
    {
        for (mode = 0; mode < 4; mode++)
        {
            sprintf(name, "bonePos, type %u, link mode %d", type, mode);
            runBonePos(name, 11u + type * 4 + (unsigned)mode, 1, type, 0x1000000, mode,
                       0, 0, 0);
        }
        sprintf(name, "bonePos, type %u, plain matrices", type);
        runBonePos(name, 51u + type, 2, type, 0, 0, 0, 0, 0);
    }
    runBonePos("bonePos, id out of range", 71u, 9, 0, 0x1000000, 1, 0, 0, 0);
    runBonePos("bonePos, id -1", 72u, -1, 0, 0x1000000, 1, 0, 0, 0);
    runBonePos("bonePos, no multipart data", 73u, 1, 0, 0x1000000, 1, 1, 0, 0);
    runBonePos("bonePos, no basic", 74u, 1, 0, 0x1000000, 1, 0, 1, 0);
    runBonePos("bonePos, null matrix pointer", 75u, 1, 0, 0x1000000, 1, 0, 0, 1);

    runSmooth("smooth, pass 2", 81u, 2, 0.5f);
    runSmooth("smooth, pass 4", 82u, 4, 0.5f);
    runSmooth("smooth, pass 8", 83u, 8, 0.5f);
    runSmooth("smooth, pass 4, weight 0", 84u, 4, 0.0f);
    runSmooth("smooth, pass 4, weight 1", 85u, 4, 1.0f);
    runSmooth("smooth, pass 4, weight out of range", 86u, 4, 1.7f);
    runSmooth("smooth, pass 16", 87u, 16, 0.5f);

    runDraw("draw, plain trail", 101u, 1, 0, 0, 0x1000000, 1, 1, 0.02f, -1);
    runDraw("draw, plain trail, three entries", 102u, 3, 0, 0, 0x1000000, 1, 1, 0.02f, -1);
    runDraw("draw, ten frames", 103u, 2, 0, 0, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, smoothed", 104u, 2, 1, 0, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, camera space", 105u, 2, 2, 0, 0x1000000, 1, 4, 0.02f, -1);
    runDraw("draw, transparent", 106u, 2, 4, 0, 0x1000000, 1, 4, 0.02f, -1);
    runDraw("draw, centred", 107u, 2, 0x10, 0, 0x1000000, 1, 4, 0.02f, -1);
    runDraw("draw, u along the trail", 108u, 2, 0x20, 0, 0x1000000, 1, 4, 0.02f, -1);
    runDraw("draw, every flag", 109u, 2, 0x37, 0, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, ribbon", 110u, 2, 0, 3, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, ribbon, centred", 111u, 2, 0x10, 3, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, ribbon, smoothed", 112u, 2, 0x11, 3, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, ribbon, every flag", 113u, 3, 0x37, 3, 0x1000000, 1, 10, 0.02f, -1);
    runDraw("draw, trail restart mid-run", 114u, 2, 1, 0, 0x1000000, 1, 10, 0.02f, 4);
    runDraw("draw, long frames expire the trail", 115u, 2, 1, 0, 0x1000000, 1, 10, 0.3f, -1);
    runDraw("draw, zero dt", 116u, 2, 1, 0, 0x1000000, 1, 4, 0.0f, -1);
    runDraw("draw, plain part matrices", 117u, 2, 1, 0, 0, 0, 6, 0.02f, -1);
    runDraw("draw, no entries", 118u, 0, 1, 0, 0x1000000, 1, 2, 0.02f, -1);

    runLoad("load, version 0", 0);
    runLoad("load, version 1", 1);
    runLoad("load, version 2", 2);
    runLoad("load, version 3", 3);

    runReinit("reinit, already referenced", 121u, 8, 1, 2);
    runReinit("reinit, first time with a material", 122u, 0, 1, 2);
    runReinit("reinit, first time without one", 123u, 0, 0, 2);
    runReinit("reinit, no entries", 124u, 0, 1, 0);

    runOnOff("onOff, on, matching id", 131u, 11, 1, 0, 0);
    runOnOff("onOff, off, matching id", 132u, 11, 0, 0, 0);
    runOnOff("onOff, off, every id", 133u, -1, 0, 0, 0);
    runOnOff("onOff, off, no match", 134u, 99, 0, 0, 0);
    runOnOff("onOff, null object", 135u, 11, 0, 0, 1);

    runSetSize("setSize, matching id and part", 141u, 11, -1, 2.5f, 0);
    runSetSize("setSize, every id", 142u, -1, -1, 3.5f, 0);
    runSetSize("setSize, one part", 143u, 11, 0, 4.5f, 0);
    runSetSize("setSize, no match", 144u, 99, -1, 5.5f, 0);
    runSetSize("setSize, null object", 145u, 11, -1, 6.5f, 1);

    printf("%d of the compared floats differed at all; worst abs %.3g, worst rel %.3g at %s (%s)\n",
           g_diffCount, g_worstAbs, g_worstRel, g_worstWhat, g_worstCase);
    if (g_fails)
        printf("%d case(s) differed\n", g_fails);
    else
        printf("all cases equivalent\n");
    return g_fails ? 1 : 0;
}
