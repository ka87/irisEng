#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "photo_mocks.h"
#include "../math.h"
#include "../memory.h"
#include "../gameobject.h"
#include "../collision.h"
#include "../animation.h"
#include "../view.h"

tPhMockCall PHMOCK_log[PHMOCK_LOG_MAX];
int PHMOCK_logLen;
int PHMOCK_logOverflow;

int PHMOCK_cases;
int PHMOCK_failures;
long PHMOCK_compares;

static const char* PHMOCK_case = "?";
static int PHMOCK_reported;

tWorld PHMOCK_world;
tGdi_DisplayData PHMOCK_dd;
tGameObjectMain PHMOCK_objects[PHMOCK_OBJECTS];
tGameObjectMain PHMOCK_animObjects[PHMOCK_OBJECTS];
tGameObjectMain PHMOCK_snapObjects[PHMOCK_OBJECTS];
tModifier PHMOCK_mdfs[PHMOCK_OBJECTS];
tPhotoMdfStruct PHMOCK_photos[PHMOCK_OBJECTS];
tableElemStructPF PHMOCK_table[PHMOCK_OBJECTS];

int PHMOCK_shiftDown;
int PHMOCK_rayHit = -1;
float PHMOCK_bvRatio = 0.8f;
float PHMOCK_screenRatio = 0.4f;
int PHMOCK_cullPattern;
int PHMOCK_isAABBoxMask = -1;
int PHMOCK_viewMtxIsCamera = 1;
int PHMOCK_designOverride;
Mtx4x4 PHMOCK_viewMtx;

static tGameObjectBV PHMOCK_bvs[PHMOCK_OBJECTS];
static tGameObjectBV PHMOCK_animBvs[PHMOCK_OBJECTS];
static Mtx4x4 PHMOCK_mtx[PHMOCK_OBJECTS];
static Mtx4x4 PHMOCK_animMtx[PHMOCK_OBJECTS];
static Mtx4x4 PHMOCK_snapMtx[PHMOCK_OBJECTS];
static tGameObjectExtended PHMOCK_ext[PHMOCK_OBJECTS];
static tGameObjectDesignFields PHMOCK_fields[PHMOCK_OBJECTS];

static tViewElem PHMOCK_view;
static tGameObjectMain* PHMOCK_mainActorArr[2];
struct sGameObjectMain** AI_MainActor;
static int PHMOCK_cullCalls;
static long long PHMOCK_misalignBuf[(sizeof(tGameObjectMain) + 16) / 8];
static tGameObjectMain* PHMOCK_misaligned;

void PHMOCK_SetCase(const char* name)
{
    PHMOCK_case = name;
    PHMOCK_reported = 0;
    PHMOCK_cases++;
}

static tPhMockCall* PHMOCK_Slot(const char* fn, int ni, int nf)
{
    tPhMockCall* c;

    if (PHMOCK_logLen >= PHMOCK_LOG_MAX)
    {
        PHMOCK_logOverflow = 1;
        return NULL;
    }
    c = &PHMOCK_log[PHMOCK_logLen++];
    memset(c, 0, sizeof(*c));
    c->fn = fn;
    c->ni = ni;
    c->nf = nf;
    return c;
}

static void PHMOCK_Record(const char* fn, int ni, int nf, ...)
{
    va_list ap;
    int i;
    tPhMockCall* c = PHMOCK_Slot(fn, ni, nf);

    if (!c)
        return;
    va_start(ap, nf);
    for (i = 0; i < ni; i++)
        c->ia[i] = va_arg(ap, int);
    for (i = 0; i < nf; i++)
        c->fa[i] = (float)va_arg(ap, double);
    va_end(ap);
}

/* ---- the log file ---------------------------------------------------- */

static FILE* PHMOCK_fileToken = (FILE*)0x1;

FILE* PHMOCK_fopen(const char* path, const char* mode)
{
    tPhMockCall* c = PHMOCK_Slot("fopen", 0, 0);

    if (c)
    {
        strncpy(c->text, path, PHMOCK_TEXT - 1);
        strncat(c->text, mode, PHMOCK_TEXT - 1 - strlen(c->text));
    }
    return PHMOCK_fileToken;
}

size_t PHMOCK_fwrite(const void* buf, size_t size, size_t n, FILE* fp)
{
    tPhMockCall* c = PHMOCK_Slot("fwrite", 2, 0);

    (void)fp;
    if (c)
    {
        size_t len = size < PHMOCK_TEXT - 1 ? size : PHMOCK_TEXT - 1;

        c->ia[0] = (int)size;
        c->ia[1] = (int)n;
        memcpy(c->text, buf, len);
        c->text[len] = 0;
    }
    return n;
}

int PHMOCK_fclose(FILE* fp)
{
    (void)fp;
    PHMOCK_Record("fclose", 0, 0);
    return 0;
}

/* ---- the engine around the analyser ---------------------------------- */

short GetAsyncKeyState(int key)
{
    PHMOCK_Record("GetAsyncKeyState", 1, 0, key);
    return PHMOCK_shiftDown ? (short)0x8000 : (short)0;
}

struct sModifier* GAO_ModifierPhoto_Get(struct sGameObjectMain* gao, int wantSphere)
{
    int i;

    for (i = 0; i < PHMOCK_OBJECTS; i++)
        if (&PHMOCK_objects[i] == gao)
        {
            PHMOCK_Record("GAO_ModifierPhoto_Get", 2, 0, i, wantSphere);
            return PHMOCK_mdfs[i].mdfPrivateDataPtr ? &PHMOCK_mdfs[i] : NULL;
        }
    PHMOCK_Record("GAO_ModifierPhoto_Get", 2, 0, -1, wantSphere);
    return NULL;
}

struct sGameObjectMain* GAO_ModifierPhoto_SnapGOGet(struct sGameObjectMain* gao,
                                                    struct sPhotoMdfStruct* photo, int useSecond)
{
    int i;

    for (i = 0; i < PHMOCK_OBJECTS; i++)
        if (&PHMOCK_objects[i] == gao)
        {
            PHMOCK_Record("GAO_ModifierPhoto_SnapGOGet", 2, 0, i, useSecond);
            (void)photo;
            return &PHMOCK_snapObjects[i];
        }
    PHMOCK_Record("GAO_ModifierPhoto_SnapGOGet", 2, 0, -1, useSecond);
    return &PHMOCK_snapObjects[0];
}

struct sGameObjectMain* ANI_pst_GetObjectByAICanal(struct sGameObjectMain* gao, uint8_t aiCanal)
{
    int i;

    for (i = 0; i < PHMOCK_OBJECTS; i++)
        if (&PHMOCK_objects[i] == gao)
        {
            PHMOCK_Record("ANI_pst_GetObjectByAICanal", 2, 0, i, aiCanal);
            /* every third object has no animation object at all */
            return (i % 3 == 2) ? NULL : &PHMOCK_animObjects[i];
        }
    PHMOCK_Record("ANI_pst_GetObjectByAICanal", 2, 0, -1, aiCanal);
    return NULL;
}

BOOL OBJ_BV_IsAABBox(tGameObjectBV* bv)
{
    int i;
    int which = -1;

    for (i = 0; i < PHMOCK_OBJECTS; i++)
    {
        if (bv == &PHMOCK_bvs[i])
            which = i;
        else if (bv == &PHMOCK_animBvs[i])
            which = 100 + i;
    }
    PHMOCK_Record("OBJ_BV_IsAABBox", 1, 0, which);
    if (which < 0)
        return 0;
    return (PHMOCK_isAABBoxMask >> (which % 100)) & 1;
}

void OBJ_BV_ComputeCenter(struct sGameObjectMain* gao, Vec3* out)
{
    int i;
    int which = -1;

    for (i = 0; i < PHMOCK_OBJECTS; i++)
    {
        if (gao == &PHMOCK_objects[i])
            which = i;
        else if (gao == &PHMOCK_animObjects[i])
            which = 100 + i;
    }
    PHMOCK_Record("OBJ_BV_ComputeCenter", 1, 0, which);
    out->x = (float)(which % 7) - 2.5f;
    out->y = (float)(which % 5) + 1.25f;
    out->z = (float)(which % 11) - 4.0f;
}

int OBJ_Frame_CullingAABBox(Vec3* bvMin, Vec3* bvMax, struct sDDCamStruct* cam, int width,
                            int height, float* bvRatio, float* screenRatio)
{
    tPhMockCall* c = PHMOCK_Slot("OBJ_Frame_CullingAABBox", 2, 6);
    int step;

    (void)cam;
    if (c)
    {
        c->ia[0] = width;
        c->ia[1] = height;
        c->fa[0] = bvMin->x;
        c->fa[1] = bvMin->y;
        c->fa[2] = bvMin->z;
        c->fa[3] = bvMax->x;
    }
    step = PHMOCK_cullCalls++;
    switch ((step + PHMOCK_cullPattern) % 6)
    {
    case 0:
        *bvRatio = 0.0f;
        *screenRatio = 0.5f;
        break;
    case 1:
        *bvRatio = 0.75f;
        *screenRatio = 0.0099999998f;
        break;
    case 2:
        *bvRatio = 0.25f;
        *screenRatio = 0.3f;
        break;
    case 3:
        /* exactly on both thresholds */
        *bvRatio = 0.5f;
        *screenRatio = 0.025f;
        break;
    case 4:
        /* under the screen threshold, over it as a fraction of the
           bounding volume, and past the bounding-volume threshold so
           that nothing downstream overwrites the code it sets */
        *bvRatio = 0.60000002f;
        *screenRatio = 0.02f;
        break;
    default:
        *bvRatio = PHMOCK_bvRatio;
        *screenRatio = PHMOCK_screenRatio;
        break;
    }
    return 0;
}

struct sGameObjectMain* COL_Visual_RayCast(struct sWorld* world, struct sGameObjectMain* skipGao,
                                           Vec3* source, Vec3* direction, float maxDist,
                                           int flagMustHave, int flagMustNothave, int whatToTest,
                                           int* ignoreResult, int skipRayCast)
{
    tPhMockCall* c = PHMOCK_Slot("COL_Visual_RayCast", 4, 4);

    (void)world;
    (void)skipGao;
    (void)ignoreResult;
    if (c)
    {
        c->ia[0] = flagMustHave;
        c->ia[1] = flagMustNothave;
        c->ia[2] = whatToTest;
        c->ia[3] = skipRayCast;
        c->fa[0] = direction->x;
        c->fa[1] = direction->y;
        c->fa[2] = direction->z;
        c->fa[3] = maxDist;
    }
    (void)source;
    if (PHMOCK_rayHit == -1)
        return NULL;
    if (PHMOCK_rayHit == -2)
        return skipGao;
    return &PHMOCK_objects[PHMOCK_rayHit % PHMOCK_OBJECTS];
}

void* MEM_p_AllocAligned(int size, int alignment) { (void)alignment; return calloc(1, size ? size : 1); }
void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment) { (void)alignment; return realloc(ptr, size ? size : 1); }
void MEM_FreeAlign(void* ptr) { free(ptr); }

/* ---- the scene -------------------------------------------------------- */

void PHMOCK_Init(void)
{
    MATH_InitModule();
    PHMOCK_mainActorArr[0] = &PHMOCK_objects[0];
    AI_MainActor = PHMOCK_mainActorArr;
    PHMOCK_world.viewPtr = &PHMOCK_view;
    PHMOCK_view.displayData = &PHMOCK_dd;
    MATH_SetIdentityMatrix(&PHMOCK_dd.ct.cam.cameraMtx);
    MATH_SetIdentityMatrix(&PHMOCK_dd.ct.cam.objectMtx);
    MATH_SetIdentityMatrix(&PHMOCK_viewMtx);
    PHMOCK_dd.ct.cam.objectMtx.Pos.x = 1.5f;
    PHMOCK_dd.ct.cam.objectMtx.Pos.y = -2.25f;
    PHMOCK_dd.ct.cam.objectMtx.Pos.z = 3.75f;
}

void PHMOCK_Reset(void)
{
    PHMOCK_logLen = 0;
    PHMOCK_logOverflow = 0;
    PHMOCK_cullCalls = 0;
    MATH_SetIdentityMatrix(&PHMOCK_viewMtx);
    PHMOCK_dd.ct.cam.viewMtxPtr_setInGRORender =
        PHMOCK_viewMtxIsCamera ? &PHMOCK_dd.ct.cam.cameraMtx : &PHMOCK_viewMtx;
}

void PHMOCK_BuildScene(unsigned int seed, int pattern)
{
    int i;
    unsigned int r = seed ? seed : 1u;

    for (i = 0; i < PHMOCK_OBJECTS; i++)
    {
        r = r * 1664525u + 1013904223u;

        MATH_SetIdentityMatrix(&PHMOCK_mtx[i]);
        MATH_SetIdentityMatrix(&PHMOCK_animMtx[i]);
        MATH_SetIdentityMatrix(&PHMOCK_snapMtx[i]);
        PHMOCK_mtx[i].Pos.x = (float)(i % 9) - 4.0f;
        PHMOCK_mtx[i].Pos.y = (float)(i % 6) - 2.0f;
        PHMOCK_mtx[i].Pos.z = (float)(i % 4) + 0.5f;
        PHMOCK_animMtx[i].Pos.x = PHMOCK_mtx[i].Pos.x + 0.75f;
        PHMOCK_animMtx[i].Pos.y = PHMOCK_mtx[i].Pos.y - 1.25f;
        PHMOCK_animMtx[i].Pos.z = PHMOCK_mtx[i].Pos.z + 2.0f;
        PHMOCK_snapMtx[i].Pos.x = PHMOCK_mtx[i].Pos.x - 0.5f;
        PHMOCK_snapMtx[i].Pos.y = PHMOCK_mtx[i].Pos.y + 1.5f;
        PHMOCK_snapMtx[i].Pos.z = PHMOCK_mtx[i].Pos.z - 0.25f;

        PHMOCK_bvs[i].BVMin.x = -1.0f - (float)(i % 3);
        PHMOCK_bvs[i].BVMin.y = -1.5f;
        PHMOCK_bvs[i].BVMin.z = -0.5f;
        PHMOCK_bvs[i].BVMax.x = 1.0f + (float)(i % 4);
        PHMOCK_bvs[i].BVMax.y = 2.5f;
        PHMOCK_bvs[i].BVMax.z = 0.75f;
        PHMOCK_animBvs[i] = PHMOCK_bvs[i];
        PHMOCK_animBvs[i].BVMax.y = 3.5f;

        memset(&PHMOCK_objects[i], 0, sizeof(tGameObjectMain));
        memset(&PHMOCK_animObjects[i], 0, sizeof(tGameObjectMain));
        memset(&PHMOCK_snapObjects[i], 0, sizeof(tGameObjectMain));
        PHMOCK_objects[i].matrices = &PHMOCK_mtx[i];
        PHMOCK_objects[i].boundingVolume = &PHMOCK_bvs[i];
        PHMOCK_animObjects[i].matrices = &PHMOCK_animMtx[i];
        PHMOCK_animObjects[i].boundingVolume = &PHMOCK_animBvs[i];
        PHMOCK_snapObjects[i].matrices = &PHMOCK_snapMtx[i];

        /* flags2: bit 1 is "analyse me", bit 2 is "skip me" */
        PHMOCK_objects[i].flags2 = ((i % 5) != 4 ? 2 : 0) | ((i % 7) == 6 ? 4 : 0);
        /* flags: bit 1 is "has an animation object", bit 0x40 gates the design fields */
        PHMOCK_objects[i].flags = ((i % 2) ? 2 : 0) | ((i % 11) != 10 ? 0x40 : 0);

        if (PHMOCK_designOverride == -1)
            PHMOCK_fields[i].designInt3 =
                (int)(((unsigned int)(99 + (i % 3)) << 24) | (unsigned int)(i % 6));
        else if (PHMOCK_designOverride == -2)
            PHMOCK_fields[i].designInt3 =
                (int)(((unsigned int)((i % 2) ? 4 : 100) << 24) | (unsigned int)(i % 6));
        else if (PHMOCK_designOverride > 0)
            PHMOCK_fields[i].designInt3 =
                (int)(0x01000000u | (unsigned int)((i % PHMOCK_designOverride) + 0x81));
        else
            PHMOCK_fields[i].designInt3 = (int)(((unsigned int)((i % 4) + 1) << 24)
                                                | (unsigned int)(i % 6));
        PHMOCK_ext[i].fields = ((i % 13) == 12) ? NULL : &PHMOCK_fields[i];
        PHMOCK_objects[i].extended = ((i % 17) == 16) ? NULL : &PHMOCK_ext[i];

        memset(&PHMOCK_photos[i], 0, sizeof(tPhotoMdfStruct));
        PHMOCK_photos[i].mission = ((i % 8) == 7) ? 0 : ((i % 4) + 1);
        PHMOCK_photos[i].info = i % 6;
        PHMOCK_photos[i].flags = ((i % 9) == 8 ? 1 : 0) | ((i % 10) == 9 ? 2 : 0);
        PHMOCK_photos[i].frame1 = 0.25f;
        PHMOCK_photos[i].frame2 = 0.9f;
        PHMOCK_photos[i].lod1 = 0.1f;
        PHMOCK_photos[i].lod2 = 0.8f;
        /* three values each: below, inside and above the window, plus
           one sitting exactly on an edge */
        switch (i % 6)
        {
        case 0:
            PHMOCK_photos[i].frame0_frameCullingSphereRet1 = 0.1f;
            PHMOCK_photos[i].lod0_frameCullingSphereRet2 = 0.4f;
            break;
        case 1:
            PHMOCK_photos[i].frame0_frameCullingSphereRet1 = 0.5f;
            PHMOCK_photos[i].lod0_frameCullingSphereRet2 = 0.05f;
            break;
        case 2:
            PHMOCK_photos[i].frame0_frameCullingSphereRet1 = 0.95f;
            PHMOCK_photos[i].lod0_frameCullingSphereRet2 = 0.9f;
            break;
        case 3:
            /* exactly on the lower edge of both windows */
            PHMOCK_photos[i].frame0_frameCullingSphereRet1 = 0.25f;
            PHMOCK_photos[i].lod0_frameCullingSphereRet2 = 0.1f;
            break;
        default:
            /* exactly on the upper edge of both windows */
            PHMOCK_photos[i].frame0_frameCullingSphereRet1 = 0.9f;
            PHMOCK_photos[i].lod0_frameCullingSphereRet2 = 0.8f;
            break;
        }
        PHMOCK_photos[i].sphereOffset.x = 0.25f * (float)(i % 5);
        PHMOCK_photos[i].sphereOffset.y = -0.5f;
        PHMOCK_photos[i].sphereOffset.z = 1.0f;

        PHMOCK_mdfs[i].gao = &PHMOCK_objects[i];
        /* pattern 0: no modifier anywhere; 1: every object has one;
           2: every other one. */
        if (pattern == 1 || (pattern == 2 && (i % 2) == 0))
            PHMOCK_mdfs[i].mdfPrivateDataPtr = &PHMOCK_photos[i];
        else
            PHMOCK_mdfs[i].mdfPrivateDataPtr = NULL;

        PHMOCK_table[i].objectPtr = &PHMOCK_objects[i];
        PHMOCK_table[i].objectFlags = 0;
        /* one empty slot and one tagged pointer, which the walk skips */
        if (i == 5)
            PHMOCK_table[i].objectPtr = NULL;
        if (i == 11)
            PHMOCK_table[i].objectPtr = (char*)&PHMOCK_objects[i] + 1;
    }
    PHMOCK_misaligned = (tGameObjectMain*)((char*)PHMOCK_misalignBuf + 2);
    memcpy(PHMOCK_misaligned, &PHMOCK_objects[17], sizeof(tGameObjectMain));
    PHMOCK_table[17].objectPtr = PHMOCK_misaligned;

    PHMOCK_world.visibleGameObjectsPF.firstElemPtr = PHMOCK_table;
    PHMOCK_world.visibleGameObjectsPF.nextElemPtr = &PHMOCK_table[PHMOCK_OBJECTS];
}

/* ---- comparison ------------------------------------------------------- */

void PHMOCK_SaveLog(tPhMockCall* dest, int* destLen)
{
    memcpy(dest, PHMOCK_log, sizeof(tPhMockCall) * PHMOCK_logLen);
    *destLen = PHMOCK_logLen;
}

int PHMOCK_SameInt(int base, int New, const char* what, const char* where)
{
    PHMOCK_compares++;
    if (base == New)
        return 0;
    if (!PHMOCK_reported)
    {
        PHMOCK_reported = 1;
        PHMOCK_failures++;
        printf("  [%s] %s %s: base %d new %d\n", PHMOCK_case, where, what, base, New);
    }
    return 1;
}

int PHMOCK_SameFloat(float base, float New, const char* what, const char* where)
{
    PHMOCK_compares++;
    if (memcmp(&base, &New, sizeof(float)) == 0)
        return 0;
    if (isnan(base) && isnan(New))
        return 0;
    if (!PHMOCK_reported)
    {
        PHMOCK_reported = 1;
        PHMOCK_failures++;
        printf("  [%s] %s %s: base %.9g new %.9g\n", PHMOCK_case, where, what,
               (double)base, (double)New);
    }
    return 1;
}

int PHMOCK_CompareLog(const tPhMockCall* a, int alen, const tPhMockCall* b, int blen,
                      const char* where)
{
    int i;
    int j;

    PHMOCK_compares++;
    if (alen != blen)
    {
        if (!PHMOCK_reported)
        {
            PHMOCK_reported = 1;
            PHMOCK_failures++;
            printf("  [%s] %s call count: base %d new %d\n", PHMOCK_case, where, alen, blen);
            for (i = 0; i < alen && i < blen; i++)
                if (a[i].fn != b[i].fn)
                {
                    printf("      first differing call %d: base %s new %s\n", i,
                           a[i].fn, b[i].fn);
                    break;
                }
        }
        return 1;
    }
    for (i = 0; i < alen; i++)
    {
        if (a[i].fn != b[i].fn)
        {
            if (!PHMOCK_reported)
            {
                PHMOCK_reported = 1;
                PHMOCK_failures++;
                printf("  [%s] %s call %d: base %s new %s\n", PHMOCK_case, where, i,
                       a[i].fn, b[i].fn);
            }
            return 1;
        }
        for (j = 0; j < a[i].ni; j++)
            if (PHMOCK_SameInt(a[i].ia[j], b[i].ia[j], "int arg", where))
                return 1;
        for (j = 0; j < a[i].nf; j++)
            if (PHMOCK_SameFloat(a[i].fa[j], b[i].fa[j], "float arg", where))
                return 1;
        if (memcmp(a[i].text, b[i].text, PHMOCK_TEXT) != 0)
        {
            PHMOCK_compares++;
            if (!PHMOCK_reported)
            {
                PHMOCK_reported = 1;
                PHMOCK_failures++;
                printf("  [%s] %s call %d (%s) text:\n    base |%s|\n    new  |%s|\n",
                       PHMOCK_case, where, i, a[i].fn, a[i].text, b[i].text);
            }
            return 1;
        }
    }
    return 0;
}

void PHMOCK_Summary(const char* title)
{
    printf("%s: %d cases, %ld comparisons, %d failing\n", title, PHMOCK_cases,
           PHMOCK_compares, PHMOCK_failures);
}
