/* Differential test for events.c: the frozen pre-rewrite copy against
   the rewritten one, over identical state.

   Both copies run over one scene buffer at one address, one after the
   other, with the mock allocator rewound between them, so every
   pointer either copy writes into the scene can be compared directly
   rather than being normalised. A case is equivalent when the scene
   bytes, the engine globals, the return values and the sequence of
   mocked engine calls all match.

   What it is for: the rewrite replaced nine gotos and three nested
   loops in EVE_RunOneTrack and EVE_b_RunAnimation with structured
   control flow, turned the key-type offset walks into one
   multiplication, turned the interpolation loader's fall-through
   vector reads into a loop, and restored two inlined math helpers.
   Those are the changes a reading cannot settle. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../events.h"
#include "../gameobject.h"
#include "../animation.h"
#include "../time.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../ai.h"
#include "../world.h"
#include "events_mocks.h"

/* ------------------------------------------- the two copies' exports */

#define DECL2(ret, name, args) ret BASE_##name args; ret NEW_##name args;

DECL2(void, EVE_InitData, (tEventData*, tGameObjectEventsList*))
DECL2(tEventData*, EVE_pst_DuplicateData, (tEventData*))
DECL2(BOOL, EVE_b_Track_UseFlash, (tGameObjectEventsListElem*, tGameObjectMain*, tGameObjectMain*, tGameObjectEventsList*))
DECL2(int, EVE_i_SearchLabel, (tGameObjectEventsListElem*, int, float*))
DECL2(int, EVE_i_SearchTime, (tGameObjectEventsListElem*, float*))
DECL2(void, EVE_SetAllTimesTo, (float))
DECL2(void, EVE_RunningTracksAtStart, (tEventData*))
DECL2(void, EVE_SetRunningTrack, (tAnimTracks*, int))
DECL2(void, EVE_SetRunningTracks, (tEventData*, int))
DECL2(void, EVE_SetPauseTrack, (tAnimTracks*, int))
DECL2(void, EVE_SetPauseTracks, (tEventData*, int))
DECL2(void, EVE_FlashMatrix, (void))
DECL2(void, EVE_GetTracksTime, (tEventData*, int, float*, float*))
DECL2(void, EVE_SetTracksTime, (tEventData*, float))
DECL2(void, EVE_FreeTrack, (tGameObjectEventsListElem*))
DECL2(uint8_t*, EVE_p_GetPdata, (tGameObjectEventElem*))
DECL2(uint16_t, EVE_w_Event_InterpolationKey_GetType, (tGameObjectEventElem*))
DECL2(Quat4*, EVE_pst_Event_InterpolationKey_GetQuaternion, (tGameObjectEventElem*))
DECL2(int, EVE_Anim_InterpolationKey_CopyMatrix, (Mtx4x4*, tGameObjectEventElem*))
DECL2(uint8_t*, EVE_pst_Event_InterpolationKey_GetNextValue, (tGameObjectEventElem*))
DECL2(Mtx4x4*, EVE_pst_Event_InterpolationKey_GetRotation, (tGameObjectEventElem*))
DECL2(void, EVE_Event_InterpolationKey_CopyMatrix, (Mtx4x4*, tGameObjectEventElem*, int))
DECL2(uint8_t*, EVE_pf_Event_InterpolationKey_GetTime, (tGameObjectEventElem*))
DECL2(float, EVE_f_Event_InterpolationKey_InterpolateTime, (tGameObjectEventElem*, float))
DECL2(void, EVE_Anim_ComputeInterpoledMatrix, (tGameObjectMain*, Mtx4x4*, tGameObjectEventElem*, tGameObjectEventElem*, float))
DECL2(void, EVE_Event_ComputeInterpoledMatrix, (tGameObjectMain*, Mtx4x4*, tGameObjectEventElem*, tGameObjectEventElem*, float))
DECL2(Vec3*, EVE_pst_Event_InterpolationKey_GetPos, (tGameObjectEventElem*))
DECL2(void, EVE_Anim_TranslationKey_Play, (tGameObjectEventElem*))
DECL2(void, EVE_Anim_QuaternionKey_Play, (tGameObjectEventElem*))
DECL2(int, EVE_RunAnimTrack, (tGameObjectEventsListElem*, tAnimTracks*, unsigned int, char, float*))
DECL2(uint8_t, EVE_c_UpdateLOD, (tGameObjectMain*, int))
DECL2(tGameObjectEventElem*, EVE_Event_InterpolationKey_Play, (tGameObjectEventElem*))
DECL2(int, EVE_RunOneTrack, (tGameObjectEventsListElem*, tAnimTracks*, int, float*))
DECL2(int, EVE_b_RunAnimation, (tEventData*, float*))
DECL2(int, EVE_b_RunAllTracks, (tEventData*, float*))
DECL2(void, EVE_ResolveTrackRef, (tGameObjectEventsList*, tGameObjectEventsListElem*))
DECL2(void, EVE_ResolveListTrackRef, (tGameObjectEventsList*))
DECL2(void, EVE_ReinitTrack, (tEventData*, int))
DECL2(void, EVE_ReinitData, (tEventData*))
DECL2(int, EVE_Event_InterpolationKey_Load, (tGameObjectEventElem*, tGameObjectEventElem*, char*, void*, tGameObjectEventsListElem*, int16_t))
DECL2(void, EVE_LoadListEvents, (tGameObjectEventsListElem*, char*, uint32_t*, int16_t))
DECL2(tGameObjectEventsList*, EVE_ul_CallbackLoadListTracks, (uint32_t))
DECL2(void, EVE_MainCall, (struct sWorld*))
DECL2(void, EVE_Duplicate, (struct sGameObjectMain*, struct sGameObjectMain*, int))
DECL2(void, EVE_ResolveRefs, (struct sGameObjectMain*, int))
DECL2(void, EVE_SynchronizeTracks, (tGameObjectActorObject*))
DECL2(void, EVE_DeleteListTracks, (tGameObjectEventsList*))
DECL2(void, EVE_FreeMainStruct, (tGameObjectMain*))
DECL2(struct sEventData*, EVE_AllocateMainStruct, (struct sGameObjectMain*))

extern int BASE_EVE_gb_CanFlash;
extern int NEW_EVE_gb_CanFlash;

/* ---------------------------------------------------------- the scene */

#define NT 6     /* tracks */
#define NK 6     /* key frames per track */
#define NG 4     /* game objects */
#define NB 4     /* skeleton bones */
#define DATA 256 /* per-track data block */

typedef struct
{
    tGameObjectEventsList list;
    tGameObjectEventsListElem tracks[NT];
    tGameObjectEventElem keys[NT][NK];
    tAnimTracks states[NT];
    tEventData eve;
    uint8_t data[NT][DATA];

    tGameObjectMain gao[NG];
    tGameObjectBasic basic[NG];
    tGameObjectExtended ext[NG];
    tGaoMatrix gaoMtx[NG];
    Mtx4x4 matrices[NG][3];
    struct sDyn dyn[NG];
    tGameObjectCollMain coll;
    struct sColInstance colInst;
    Mtx4x4 collMtx;

    tGameObjectActorObject actor;
    tGroup skeleton;
    tableDescStruct skelTable;
    tableElemStructPF skelElems[NB];
    struct sAnim anim;

    Mtx4x4 refMtx;
    Mtx4x4 out[4];
    float outF[4];
    int outI[4];
    void* outP[4];
    uint8_t keyData[128];
} tScene;

static tScene scene;
static tScene pristine;
static tScene snapBase;
static tScene snapNew;

#define HEAPSNAP (192 * 1024)
static uint8_t heapBase[HEAPSNAP];
static uint8_t heapNew[HEAPSNAP];
static unsigned int heapUsedBase;
static unsigned int heapUsedNew;

/* Engine globals both copies share, captured after each run. */
typedef struct
{
    void* curTrack;
    void* curEvent;
    void* curParam;
    void* curGao;
    void* curRefMtx;
    void* savedMtx;
    void* curData;
    void* curList;
    void* curResolve;
    void* ownerGao;
    float dt;
    float elapsedDt;
    int needReparse;
    int animSize;
    int numTracks;
    int everunning;
    int readMode;
    unsigned long fts[4];
    void* aiFunc;
    void* aiInst;
    void* aiInstUltra;
    void* aiUltra;
    void* aiGao;
    int aiStack;
    int aiVarStack;
} tGlobals;

static tGlobals gBase;
static tGlobals gNew;

static void capture(tGlobals* g)
{
    memset(g, 0, sizeof(*g));
    g->curTrack = EVE_gpst_CurrentTrack;
    g->curEvent = EVE_gpst_CurrentEvent;
    g->curParam = EVE_gpst_CurrentParam;
    g->curGao = EVE_gpst_CurrentGAO;
    g->curRefMtx = EVE_gpst_CurrentRefMatrix;
    g->savedMtx = EVE_gpst_SavedMatrix;
    g->curData = EVE_gpst_CurrentData;
    g->curList = EVE_gpst_CurrentListTracks;
    g->curResolve = EVE_gpst_CurrentReolve;
    g->ownerGao = EVE_gpst_OwnerGAO;
    g->dt = EVE_gf_DT;
    g->elapsedDt = EVE_gf_ElapsedDT;
    g->needReparse = EVE_gb_NeedToReparse;
    g->animSize = AnimSizeOnDisk;
    g->numTracks = NumTracks;
    g->everunning = ENG_gb_EVERunning;
    g->readMode = BIG_gi_ReadMode;
    g->fts[0] = LOA_ul_FileTypeSize_30;
    g->fts[1] = LOA_ul_FileTypeSize_31;
    g->fts[2] = LOA_ul_FileTypeSize_32;
    g->fts[3] = LOA_ul_FileTypeSize_33;
    g->aiFunc = AI_gpst_CurrentFunction;
    g->aiInst = AI_gpst_CurrentInstance;
    g->aiInstUltra = AI_gpst_CurrentInstanceUltra;
    g->aiUltra = AI_gpst_CurrentUltra;
    g->aiGao = AI_gpst_CurrentGameObject;
    g->aiStack = AI_gul_SizeGlobalStack;
    g->aiVarStack = AI_gul_SizeGlobalVarStack;
}

/* ------------------------------------------------------- scene set-up */

static float g_dt;
static int g_playOnlyAnim;
static int g_forcePlayer;
static int g_everunning;
static int g_canFlash;
static int g_exiting;

static void resetEngine(void)
{
    int i;

    MOCK_HeapReset();
    MOCK_LogReset();
    MOCK_FramesReset();
    MOCK_RegisterScene(&scene, (unsigned int)sizeof(scene));

    EVE_gpst_CurrentTrack = NULL;
    EVE_gpst_CurrentEvent = NULL;
    EVE_gpst_CurrentParam = NULL;
    EVE_gpst_CurrentGAO = NULL;
    EVE_gpst_CurrentRefMatrix = NULL;
    EVE_gpst_SavedMatrix = NULL;
    EVE_gpst_CurrentData = NULL;
    EVE_gpst_CurrentListTracks = NULL;
    EVE_gpst_CurrentReolve = NULL;
    EVE_gpst_OwnerGAO = NULL;
    EVE_gf_DT = 0.0f;
    EVE_gf_ElapsedDT = 0.0f;
    EVE_gb_NeedToReparse = 0;
    EVE_gb_PlayOnlyAnimTracks = g_playOnlyAnim;
    EVE_gb_ForcePlayerOnAllTracks = g_forcePlayer;
    BASE_EVE_gb_CanFlash = g_canFlash;
    NEW_EVE_gb_CanFlash = g_canFlash;
    ENG_gb_EVERunning = g_everunning;
    ENG_gb_ExitApplication = g_exiting;
    AnimSizeOnDisk = 0;
    NumTracks = 0;
    TIM_gf_dt = g_dt;
    BIG_gi_ReadMode = 1;
    BIG_gi_SpecialRead = 0;
    OBJ_gb_DuplicateAI = 1;
    LOA_ul_FileTypeSize_30 = 0;
    LOA_ul_FileTypeSize_31 = 0;
    LOA_ul_FileTypeSize_32 = 0;
    LOA_ul_FileTypeSize_33 = 0;
    AI_gul_SizeGlobalStack = 0;
    AI_gul_SizeGlobalVarStack = 0;
    AI_gpst_CurrentFunction = NULL;
    AI_gpst_CurrentInstance = NULL;
    AI_gpst_CurrentInstanceUltra = NULL;
    AI_gpst_CurrentUltra = NULL;
    AI_gpst_CurrentGameObject = NULL;
    memset(AI_gac_GlobalStack, 0, 64);
    for (i = 0; i < 64; ++i)
        AI_gaw_EnumLink[i] = (int16_t)(i * 3 + 1);
}

static unsigned s_rng;

static void rngSeed(unsigned seed)
{
    s_rng = seed ? seed : 1u;
}

static unsigned rngNext(void)
{
    s_rng = s_rng * 1664525u + 1013904223u;
    return s_rng >> 8;
}

static float rngFloat(float lo, float hi)
{
    return lo + (hi - lo) * (float)(rngNext() & 0xFFFF) / 65535.0f;
}

static void setIdentity(Mtx4x4* m, float bias)
{
    memset(m, 0, sizeof(*m));
    m->v1.x = 1.0f;
    m->v2.y = 1.0f;
    m->v3.z = 1.0f;
    m->Pos.x = bias;
    m->Pos.y = bias + 0.5f;
    m->Pos.z = bias - 0.25f;
    m->w = 1.0f;
    m->mtxflag = 7;
}

/* One deterministic scene. `shape` varies the parts a case wants to
   reach; everything else is the same every time. */
static void buildScene(unsigned seed, int shape)
{
    int t, k, g;

    memset(&scene, 0, sizeof(scene));
    rngSeed(seed);

    for (g = 0; g < NG; ++g)
    {
        scene.gao[g].basic = &scene.basic[g];
        scene.gao[g].extended = &scene.ext[g];
        scene.gao[g].matrices = &scene.matrices[g][0];
        scene.basic[g].mtx = &scene.gaoMtx[g];
        scene.basic[g].dyn = &scene.dyn[g];
        scene.basic[g].actorObjectOrVisualObject = &scene.actor;
        setIdentity(&scene.matrices[g][0], (float)g);
        setIdentity(&scene.matrices[g][1], (float)g + 10.0f);
        setIdentity(&scene.matrices[g][2], (float)g + 20.0f);
        setIdentity(&scene.gaoMtx[g].mtx, (float)g + 30.0f);
        scene.gao[g].distance = (uint8_t)(g * 11);
        scene.ext[g].extraFlag = (int16_t)((shape & 1) ? 0 : 0x80);
    }
    /* object 0 is the owner, 1..3 are bones of its skeleton */
    scene.gao[0].flags = 2 | 0x200;
    scene.gao[0].flags2 = 1;
    scene.gaoMtx[1].refGao = &scene.gao[0];
    scene.gaoMtx[1].refGao2 = &scene.gao[0];
    scene.gaoMtx[2].refGao = &scene.gao[1];
    scene.gaoMtx[2].refGao2 = &scene.gao[1];
    scene.gaoMtx[3].refGao = &scene.gao[2];
    scene.gaoMtx[3].refGao2 = &scene.gao[2];

    scene.ext[0].coll = &scene.coll;
    scene.coll.colInstance = &scene.colInst;
    scene.colInst.mtx = &scene.collMtx;
    setIdentity(&scene.collMtx, 99.0f);

    scene.actor.skeletonGroup = &scene.skeleton;
    scene.actor.animPtr[0] = &scene.anim;
    scene.anim.ptr = &scene.eve;
    scene.actor.lod = (uint8_t)((shape & 2) ? 0x9F : 0x00);
    scene.actor.ANILOD = (uint8_t)((shape & 4) ? 0x05 : 0x00);
    scene.actor.aniBitFieldLOD = 0x8F;
    scene.actor.aniPelvisOffset = 0.75f;
    scene.skeleton.tablePF = &scene.skelTable;
    scene.skelTable.actElemNum = NB;
    scene.skelTable.firstElemPtr = &scene.skelElems[0];
    scene.skelTable.nextElemPtr = &scene.skelElems[NB];
    for (g = 0; g < NB; ++g)
    {
        scene.skelElems[g].objectPtr = &scene.gao[g];
        scene.skelElems[g].objectFlags = scene.gao[g].flags;
    }

    scene.list.listElems = &scene.tracks[0];
    scene.list.num = NT;
    scene.list.flags = (int16_t)((shape & 8) ? 1 : 0);
    scene.list.usageCount = 3;
    scene.eve.eventListStruct = &scene.list;
    scene.eve.tracks = &scene.states[0];

    for (t = 0; t < NT; ++t)
    {
        scene.tracks[t].trackElems = &scene.keys[t][0];
        scene.tracks[t].elemNum = NK;
        scene.tracks[t].dataPtr = &scene.data[t][0];
        scene.tracks[t].size = DATA;
        scene.tracks[t].boneID = (uint16_t)(t == 0 ? 0xFFFF : (t % NB));
        scene.tracks[t].flags = (int16_t)((t & 1) ? 0x8002 : 0x000A);
        scene.tracks[t].gao = (t == 2) ? &scene.gao[1] : NULL;
        scene.tracks[t].s3 = (int16_t)t;
        scene.states[t].currTime = rngFloat(0.0f, 0.05f);
        scene.states[t].currElemKeyFrame = (uint16_t)(t % NK);
        scene.states[t].flag = (int16_t)(1 | ((t & 2) ? 4 : 0));
        scene.states[t].privateData = t * 7;

        for (k = 0; k < NK; ++k)
        {
            static const int kinds[6] = { 0x80, 0x40, 0x200, 0x400, 0x80, 0x00 };
            int kind = kinds[(t + k) % 6];
            scene.keys[t][k].duration = (uint16_t)(1 + ((t + k) % 5));
            /* A 0x400 key whose label search succeeds does not
               terminate: the reparse restarts the track walk from the
               key the label picked and reaches this one again. These
               carry a label nothing matches. */
            scene.keys[t][k].flags2 = (int16_t)(kind
                                                | ((k == 1) ? 4 : 0)
                                                | ((kind == 0x400) ? 0xF000 : ((k == 3) ? 0x1000 : 0))
                                                | ((shape & 16) ? 0x20 : 0));
            /* inline key data: size odd, so EVE_p_GetPdata walks dataPtr */
            scene.keys[t][k].eptrdu.size = (uint16_t)((20 << 1) | 1);
            scene.keys[t][k].eptrdu.type = (uint16_t)(0x0013 | ((k & 1) ? 0x04 : 0x00));
        }
        /* the data block holds plausible vectors and quaternions */
        for (k = 0; k < DATA / 4; ++k)
        {
            float f = rngFloat(-4.0f, 4.0f);
            memcpy(&scene.data[t][k * 4], &f, sizeof(float));
        }
    }
    setIdentity(&scene.refMtx, 5.0f);
    for (k = 0; k < 4; ++k)
        setIdentity(&scene.out[k], 40.0f + k);
    for (k = 0; k < 128; ++k)
        scene.keyData[k] = (uint8_t)(k * 7 + 3);

    memcpy(&pristine, &scene, sizeof(scene));
}

/* -------------------------------------------------------- comparison */

static int g_fails;
static int g_cases;
static int g_floatDiffs;
static double g_worstAbs;
static double g_worstRel;
static char g_case[160];

static void fail(const char* what)
{
    if (g_fails < 40)
        printf("    DIFF %s | %s\n", g_case, what);
    ++g_fails;
}

/* A byte-for-byte difference in the scene is reported as the field
   offset, and then interpreted as a float pair so a last-bit
   difference from a restored math call is told apart from a real one. */
static void compareScene(void)
{
    unsigned int i;
    const uint8_t* a = (const uint8_t*)&snapBase;
    const uint8_t* b = (const uint8_t*)&snapNew;
    int reported = 0;

    for (i = 0; i < sizeof(tScene); ++i)
    {
        if (a[i] == b[i])
            continue;
        if (reported < 4)
        {
            unsigned int w = i & ~3u;
            float fa, fb;
            int ia, ib;
            memcpy(&fa, a + w, 4);
            memcpy(&fb, b + w, 4);
            memcpy(&ia, a + w, 4);
            memcpy(&ib, b + w, 4);
            printf("    DIFF %s | scene+%u: old %d/%.9g new %d/%.9g\n",
                   g_case, w, ia, (double)fa, ib, (double)fb);
            if (fa == fa && fb == fb && fa != 0.0f && fb != 0.0f)
            {
                double d = fabs((double)fa - (double)fb);
                double m = fabs((double)fa) > fabs((double)fb) ? fabs((double)fa) : fabs((double)fb);
                double r = m > 0.0 ? d / m : d;
                ++g_floatDiffs;
                if (d > g_worstAbs)
                    g_worstAbs = d;
                if (r > g_worstRel)
                    g_worstRel = r;
            }
        }
        ++reported;
    }
    if (reported)
    {
        ++g_fails;
        printf("    DIFF %s | %d scene bytes differ\n", g_case, reported);
    }
}

/* What the loaders and the duplication wrote into the mock heap. Both
   runs get the same blocks at the same addresses, so this is a plain
   memcmp of the used prefix. */
static void compareHeap(void)
{
    unsigned int i, n;

    if (heapUsedBase != heapUsedNew)
    {
        char buf[96];
        sprintf(buf, "heap used: old %u new %u", heapUsedBase, heapUsedNew);
        fail(buf);
    }
    n = heapUsedBase < heapUsedNew ? heapUsedBase : heapUsedNew;
    if (n > HEAPSNAP)
        n = HEAPSNAP;
    for (i = 0; i < n; ++i)
        if (heapBase[i] != heapNew[i])
        {
            char buf[96];
            sprintf(buf, "heap byte +%u: old %02x new %02x", i, heapBase[i], heapNew[i]);
            fail(buf);
            return;
        }
}

static void compareGlobals(void)
{
    if (memcmp(&gBase, &gNew, sizeof(tGlobals)) != 0)
    {
        unsigned int i;
        const uint8_t* a = (const uint8_t*)&gBase;
        const uint8_t* b = (const uint8_t*)&gNew;
        for (i = 0; i < sizeof(tGlobals); ++i)
            if (a[i] != b[i])
            {
                char buf[96];
                sprintf(buf, "engine global at +%u differs", i);
                fail(buf);
                return;
            }
    }
}

static int g_logBaseNum;
static char g_logBase[MOCK_LOG_MAX][MOCK_LOG_LINE];

static void snapLog(void)
{
    int i;

    g_logBaseNum = MOCK_LogNum();
    for (i = 0; i < g_logBaseNum; ++i)
        strcpy(g_logBase[i], MOCK_LogLine(i));
}

static void compareLog(void)
{
    int i;
    int n = MOCK_LogNum();

    if (MOCK_LogOverflowed())
    {
        fail("mock call log overflowed");
        return;
    }
    if (n != g_logBaseNum)
    {
        char buf[96];
        sprintf(buf, "call count: old %d new %d", g_logBaseNum, n);
        fail(buf);
    }
    for (i = 0; i < (n < g_logBaseNum ? n : g_logBaseNum); ++i)
    {
        if (strcmp(g_logBase[i], MOCK_LogLine(i)) != 0)
        {
            char buf[256];
            sprintf(buf, "call %d: old %.90s new %.90s", i, g_logBase[i], MOCK_LogLine(i));
            fail(buf);
            return;
        }
    }
}

typedef void (*tCase)(int side);

static void runBoth(tCase fn)
{
    ++g_cases;

    memcpy(&scene, &pristine, sizeof(scene));
    resetEngine();
    fn(0);
    memcpy(&snapBase, &scene, sizeof(scene));
    heapUsedBase = MOCK_HeapUsed();
    memcpy(heapBase, MOCK_HeapBase(), heapUsedBase < HEAPSNAP ? heapUsedBase : HEAPSNAP);
    capture(&gBase);
    snapLog();

    memcpy(&scene, &pristine, sizeof(scene));
    resetEngine();
    fn(1);
    memcpy(&snapNew, &scene, sizeof(scene));
    heapUsedNew = MOCK_HeapUsed();
    memcpy(heapNew, MOCK_HeapBase(), heapUsedNew < HEAPSNAP ? heapUsedNew : HEAPSNAP);
    capture(&gNew);

    compareScene();
    compareHeap();
    compareGlobals();
    compareLog();
}

/* ------------------------------------------------------------- cases */

static int c_t, c_k, c_i, c_j;
static float c_f;
static int16_t c_type;

/* 1. the key-type offset walks, over every type word */

static void case_accessors(int side)
{
    tGameObjectEventElem* key = &scene.keys[0][0];
    uint8_t* p;

    EVE_gpst_CurrentTrack = &scene.tracks[0];
    key->eptrdu.size = (uint16_t)((20 << 1) | (c_j & 1));
    if (c_j & 1)
        key->eptrdu.type = (uint16_t)c_type;
    else
    {
        key->eptrdu.data = &scene.keyData[0];
        memcpy(&scene.keyData[2], &c_type, 2);
    }

    if (side == 0)
    {
        scene.outI[0] = BASE_EVE_w_Event_InterpolationKey_GetType(key);
        scene.outP[0] = BASE_EVE_pst_Event_InterpolationKey_GetQuaternion(key);
        scene.outP[1] = BASE_EVE_pst_Event_InterpolationKey_GetNextValue(key);
        scene.outP[2] = BASE_EVE_pst_Event_InterpolationKey_GetRotation(key);
        scene.outP[3] = BASE_EVE_pf_Event_InterpolationKey_GetTime(key);
        p = (uint8_t*)BASE_EVE_pst_Event_InterpolationKey_GetPos(key);
        scene.outI[1] = (int)(p ? p - (uint8_t*)&scene : -1);
        p = BASE_EVE_p_GetPdata(key);
        scene.outI[2] = (int)(p ? p - (uint8_t*)&scene : -1);
    }
    else
    {
        scene.outI[0] = NEW_EVE_w_Event_InterpolationKey_GetType(key);
        scene.outP[0] = NEW_EVE_pst_Event_InterpolationKey_GetQuaternion(key);
        scene.outP[1] = NEW_EVE_pst_Event_InterpolationKey_GetNextValue(key);
        scene.outP[2] = NEW_EVE_pst_Event_InterpolationKey_GetRotation(key);
        scene.outP[3] = NEW_EVE_pf_Event_InterpolationKey_GetTime(key);
        p = (uint8_t*)NEW_EVE_pst_Event_InterpolationKey_GetPos(key);
        scene.outI[1] = (int)(p ? p - (uint8_t*)&scene : -1);
        p = NEW_EVE_p_GetPdata(key);
        scene.outI[2] = (int)(p ? p - (uint8_t*)&scene : -1);
    }
}

/* 2. the time and label searches */

static void case_searchTime(int side)
{
    scene.outF[0] = c_f;
    if (side == 0)
        scene.outI[0] = BASE_EVE_i_SearchTime(&scene.tracks[c_t], &scene.outF[0]);
    else
        scene.outI[0] = NEW_EVE_i_SearchTime(&scene.tracks[c_t], &scene.outF[0]);
}

static void case_searchLabel(int side)
{
    if (side == 0)
        scene.outI[0] = BASE_EVE_i_SearchLabel(&scene.tracks[c_t], c_i, &scene.outF[0]);
    else
        scene.outI[0] = NEW_EVE_i_SearchLabel(&scene.tracks[c_t], c_i, &scene.outF[0]);
}

static void case_tracksTime(int side)
{
    EVE_gpst_OwnerGAO = &scene.gao[0];
    scene.ext[0].eve = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    if (side == 0)
    {
        BASE_EVE_GetTracksTime(&scene.eve, c_t, &scene.outF[0], &scene.outF[1]);
        BASE_EVE_GetTracksTime(&scene.eve, c_t, &scene.outF[2], NULL);
        BASE_EVE_GetTracksTime(&scene.eve, c_t, NULL, &scene.outF[3]);
        BASE_EVE_SetTracksTime(&scene.eve, c_f);
        BASE_EVE_SetAllTimesTo(c_f);
    }
    else
    {
        NEW_EVE_GetTracksTime(&scene.eve, c_t, &scene.outF[0], &scene.outF[1]);
        NEW_EVE_GetTracksTime(&scene.eve, c_t, &scene.outF[2], NULL);
        NEW_EVE_GetTracksTime(&scene.eve, c_t, NULL, &scene.outF[3]);
        NEW_EVE_SetTracksTime(&scene.eve, c_f);
        NEW_EVE_SetAllTimesTo(c_f);
    }
}

/* 3. the per-track state helpers */

static void case_state(int side)
{
    if (side == 0)
    {
        BASE_EVE_RunningTracksAtStart(&scene.eve);
        BASE_EVE_SetRunningTrack(&scene.states[c_t], c_i);
        BASE_EVE_SetRunningTracks(&scene.eve, c_i);
        BASE_EVE_SetPauseTrack(&scene.states[c_t], c_i);
        BASE_EVE_SetPauseTracks(&scene.eve, c_i);
        BASE_EVE_ReinitTrack(&scene.eve, c_t);
        BASE_EVE_ReinitData(&scene.eve);
    }
    else
    {
        NEW_EVE_RunningTracksAtStart(&scene.eve);
        NEW_EVE_SetRunningTrack(&scene.states[c_t], c_i);
        NEW_EVE_SetRunningTracks(&scene.eve, c_i);
        NEW_EVE_SetPauseTrack(&scene.states[c_t], c_i);
        NEW_EVE_SetPauseTracks(&scene.eve, c_i);
        NEW_EVE_ReinitTrack(&scene.eve, c_t);
        NEW_EVE_ReinitData(&scene.eve);
    }
}

/* 4. the flash test and the matrix flash */

static void case_flash(int side)
{
    EVE_gpst_CurrentGAO = &scene.gao[c_i & 1];
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = (c_i & 2) ? &scene.refMtx : NULL;
    if (side == 0)
    {
        scene.outI[0] = BASE_EVE_b_Track_UseFlash(&scene.tracks[c_t], NULL, NULL, NULL);
        scene.outI[1] = BASE_EVE_b_Track_UseFlash(&scene.tracks[c_t], &scene.gao[1], &scene.gao[1], &scene.list);
        BASE_EVE_FlashMatrix();
    }
    else
    {
        scene.outI[0] = NEW_EVE_b_Track_UseFlash(&scene.tracks[c_t], NULL, NULL, NULL);
        scene.outI[1] = NEW_EVE_b_Track_UseFlash(&scene.tracks[c_t], &scene.gao[1], &scene.gao[1], &scene.list);
        NEW_EVE_FlashMatrix();
    }
}

/* 5. the interpolated matrices and the key players */

static void case_interp(int side)
{
    tGameObjectEventElem* key = &scene.keys[c_t][c_k];
    tGameObjectEventElem* next = &scene.keys[c_t][(c_k + 1) % NK];

    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentTrack = &scene.tracks[c_t];
    EVE_gpst_CurrentParam = &scene.states[c_t];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = &scene.refMtx;
    key->eptrdu.type = (uint16_t)c_type;

    if (side == 0)
    {
        BASE_EVE_Anim_ComputeInterpoledMatrix(&scene.gao[0], &scene.out[0], key, next, c_f);
        BASE_EVE_Event_ComputeInterpoledMatrix(&scene.gao[0], &scene.out[1], key, next, c_f);
        BASE_EVE_Event_InterpolationKey_CopyMatrix(&scene.out[2], key, c_i & 2);
        scene.outI[0] = BASE_EVE_Anim_InterpolationKey_CopyMatrix(&scene.out[3], key);
        BASE_EVE_Anim_TranslationKey_Play(key);
        BASE_EVE_Anim_QuaternionKey_Play(key);
        scene.outP[0] = BASE_EVE_Event_InterpolationKey_Play(key);
    }
    else
    {
        NEW_EVE_Anim_ComputeInterpoledMatrix(&scene.gao[0], &scene.out[0], key, next, c_f);
        NEW_EVE_Event_ComputeInterpoledMatrix(&scene.gao[0], &scene.out[1], key, next, c_f);
        NEW_EVE_Event_InterpolationKey_CopyMatrix(&scene.out[2], key, c_i & 2);
        scene.outI[0] = NEW_EVE_Anim_InterpolationKey_CopyMatrix(&scene.out[3], key);
        NEW_EVE_Anim_TranslationKey_Play(key);
        NEW_EVE_Anim_QuaternionKey_Play(key);
        scene.outP[0] = NEW_EVE_Event_InterpolationKey_Play(key);
    }
}

/* 6. the key-frame walkers: the whole point of the test */

static void case_runOneTrack(int side)
{
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[c_j & 1];
    EVE_gpst_CurrentTrack = &scene.tracks[c_t];
    EVE_gpst_CurrentParam = &scene.states[c_t];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = (c_j & 2) ? &scene.refMtx : NULL;
    scene.ext[0].eve = &scene.eve;
    scene.states[c_t].currElemKeyFrame = (uint16_t)c_k;
    scene.outF[0] = -1.0f;
    if (side == 0)
        scene.outI[0] = BASE_EVE_RunOneTrack(&scene.tracks[c_t], &scene.states[c_t], c_i, &scene.outF[0]);
    else
        scene.outI[0] = NEW_EVE_RunOneTrack(&scene.tracks[c_t], &scene.states[c_t], c_i, &scene.outF[0]);
}

static void case_runAnimTrack(int side)
{
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentTrack = &scene.tracks[c_t];
    EVE_gpst_CurrentParam = &scene.states[c_t];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = &scene.refMtx;
    scene.ext[0].eve = &scene.eve;
    scene.states[c_t].currElemKeyFrame = (uint16_t)c_k;
    scene.tracks[c_t].boneID = (uint16_t)((c_i & 1) ? 255 : scene.tracks[c_t].boneID);
    {
        int n;
        for (n = 0; n < NK; ++n)
            scene.keys[c_t][n].eptrdu.type = (uint16_t)((c_i & 2) ? 0x03 : 0x13);
    }
    scene.outF[0] = -1.0f;
    if (side == 0)
        scene.outI[0] = BASE_EVE_RunAnimTrack(&scene.tracks[c_t], &scene.states[c_t], c_i, (char)c_j, &scene.outF[0]);
    else
        scene.outI[0] = NEW_EVE_RunAnimTrack(&scene.tracks[c_t], &scene.states[c_t], c_i, (char)c_j, &scene.outF[0]);
}

static void case_runAll(int side)
{
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentRefMatrix = (c_j & 2) ? &scene.refMtx : NULL;
    scene.ext[0].eve = &scene.eve;
    scene.outF[0] = -1.0f;
    scene.outF[1] = -1.0f;
    if (side == 0)
    {
        scene.outI[0] = BASE_EVE_b_RunAnimation(&scene.eve, &scene.outF[0]);
        scene.outI[1] = BASE_EVE_b_RunAllTracks(&scene.eve, &scene.outF[1]);
        scene.outI[2] = BASE_EVE_c_UpdateLOD(&scene.gao[0], c_i & 1);
    }
    else
    {
        scene.outI[0] = NEW_EVE_b_RunAnimation(&scene.eve, &scene.outF[0]);
        scene.outI[1] = NEW_EVE_b_RunAllTracks(&scene.eve, &scene.outF[1]);
        scene.outI[2] = NEW_EVE_c_UpdateLOD(&scene.gao[0], c_i & 1);
    }
}

/* 7. the loaders */

static unsigned char lbuf[3072];
static int lp;

static void pb(int v) { lbuf[lp++] = (unsigned char)v; }
static void ps(int v) { pb(v & 0xFF); pb((v >> 8) & 0xFF); }
static void pl(unsigned int v) { ps((int)(v & 0xFFFF)); ps((int)(v >> 16)); }
static void pf(float f) { memcpy(lbuf + lp, &f, 4); lp += 4; }

/* The payload one interpolation key reads, for the types the cases
   use. `kf` is the flag word the loader passes down per key. */
static void putKeyPayload(int kf, int type)
{
    int i;

    if (!(kf & 0x2000))
        ps(40);
    if (!(kf & 0x1000))
        ps(type);
    if (type & 0x780)
    {
        for (i = 0; i < 8; ++i)
            pb(0x20 + i);
        return;
    }
    if (type & 0xF800)
    {
        for (i = 0; i < 8; ++i)
            pb(0x30 + i);
        return;
    }
    if ((type & 3) != 0)
    {
        for (i = 0; i < (type & 3); ++i)
        {
            pf(1.0f + i);
            pf(2.0f + i);
            pf(3.0f + i);
        }
        if (type & 8)
        {
            pl(2);                  /* the time block's count */
            pf(0.01f);
            pf(2.0f);
            pf(3.0f);
        }
        return;
    }
    if ((type & 0x14) == 4)
    {
        for (i = 0; i < 20; ++i)
            pf(0.25f * i);
    }
    else if ((type & 0x14) == 16)
    {
        pf(0.5f);
        pf(0.25f);
        pf(0.125f);
        pf(1.0f);
    }
}

static void case_keyLoad(int side)
{
    tGameObjectEventElem* key = &scene.keys[0][2];
    tGameObjectEventElem* prev = &scene.keys[0][1];

    lp = 0;
    memset(lbuf, 0xA5, sizeof(lbuf));
    putKeyPayload(c_i, (int)(uint16_t)c_type);
    MOCK_FileSet(lbuf, (int)sizeof(lbuf));

    EVE_gpst_CurrentTrack = &scene.tracks[0];
    scene.tracks[0].trackElems[0].eptrdu.size = (uint16_t)((20 << 1) | 1);
    scene.tracks[0].trackElems[0].eptrdu.type = (uint16_t)c_type;

    if (side == 0)
        scene.outI[0] = BASE_EVE_Event_InterpolationKey_Load(key, prev, MOCK_FileBase(),
                                                            &scene.data[1][0], &scene.tracks[0], (int16_t)c_i);
    else
        scene.outI[0] = NEW_EVE_Event_InterpolationKey_Load(key, prev, MOCK_FileBase(),
                                                           &scene.data[1][0], &scene.tracks[0], (int16_t)c_i);
}

/* The whole track-list stream: two tracks, each with two keys. The
   sizes and counts are small on purpose; everything else varies. */
static void putTrackList(int trackFlags, int keyType)
{
    int t, k, i, kf;

    lp = 0;
    memset(lbuf, 0xA5, sizeof(lbuf));
    ps(2);                      /* track count */
    ps(0);                      /* list flags */
    for (t = 0; t < 2; ++t)
    {
        int tf = trackFlags | ((t == 1) ? 0x8000 : 0);
        ps(tf);
        ps(1);                  /* boneID */
        pl(192);                /* track data size */
        if (tf & 0x8000)
        {
            if (((tf & 0xFF00) & ~0xC000) == 0)
                pl(7);          /* s3 */
        }
        else
        {
            pl(0x1234);         /* gao key */
            for (i = 0; i < 16; ++i)
                pb(0x40 + i);
        }
        pl(2);                  /* elemNum */
        for (k = 0; k < 2; ++k)
        {
            if (tf & 0x200)
            {
                /* 0x96 is 150 frames. The compact duration is a single
                   byte read with LOA_ReadChar, which returns a signed
                   char, so anything from 0x80 up sign-extends into the
                   uint16_t field unless it is taken as a byte. The
                   fixture used to write 3 at every key and could not
                   see that. */
                pb(k ? 0x96 : 3);
            }
            else
            {
                ps(3);          /* duration */
            }
            if (!(tf & 0x800) || k == 0)
                ps(0x80);       /* flags2: an interpolation key */
            kf = (k ? (tf & 0xFF00) : 0);
            if (k == 0 && ((tf & 0xFF00) & 0x2000))
                kf |= 0x100;
            putKeyPayload(kf, keyType);
        }
    }
}

static void case_listLoad(int side)
{
    uint32_t read;

    putTrackList((int)(uint16_t)c_type, 1 + (c_k % 3));
    MOCK_FileSet(lbuf, (int)sizeof(lbuf));
    MOCK_isSpecial = c_i & 1;
    BIG_gi_ReadMode = (c_i & 2) ? 1 : 0;

    if (side == 0)
        scene.outP[0] = BASE_EVE_ul_CallbackLoadListTracks(0);
    else
        scene.outP[0] = NEW_EVE_ul_CallbackLoadListTracks(0);

    /* and the per-track loader on its own, over the same stream */
    lp = 0;
    memset(lbuf, 0xA5, sizeof(lbuf));
    pl(2);
    putKeyPayload(0, 1 + (c_k % 3));
    MOCK_FileSet(lbuf, (int)sizeof(lbuf));
    scene.tracks[3].size = (int16_t)((c_j & 1) ? 0 : 192);
    EVE_gpst_CurrentTrack = &scene.tracks[3];
    read = 0;
    if (side == 0)
        BASE_EVE_LoadListEvents(&scene.tracks[3], MOCK_FileBase(), &read, (int16_t)c_type);
    else
        NEW_EVE_LoadListEvents(&scene.tracks[3], MOCK_FileBase(), &read, (int16_t)c_type);
    scene.outI[1] = (int)read;

    /* Hash the durations the list loader actually produced. Without
       this the case compared only the byte count and the call log, so
       a compact duration byte that sign-extended into the uint16_t
       field was invisible. */
    scene.outI[2] = 0;
    if (scene.outP[0])
    {
        tGameObjectEventsList* L = (tGameObjectEventsList*)scene.outP[0];
        unsigned t2, k2;
        for (t2 = 0; t2 < L->num && t2 < 4u; ++t2)
            if (L->listElems[t2].trackElems)
                for (k2 = 0; k2 < L->listElems[t2].elemNum && k2 < 4u; ++k2)
                    scene.outI[2] = scene.outI[2] * 131
                                  + (int)L->listElems[t2].trackElems[k2].duration;
    }
}

/* 8. allocation, duplication, the ref resolve and the main call */

static void case_structs(int side)
{
    int t, k;

    /* the duplication copies a key's data block through eptrdu.data,
       which only means anything for the pointer form of a key */
    for (t = 0; t < NT; ++t)
        for (k = 0; k < NK; ++k)
            scene.keys[t][k].eptrdu.data = NULL;
    scene.ext[0].eve = &scene.eve;
    scene.ext[1].eve = NULL;
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    if (side == 0)
    {
        scene.outP[0] = BASE_EVE_AllocateMainStruct(&scene.gao[1]);
        BASE_EVE_InitData(&scene.eve, &scene.list);
        scene.outP[1] = BASE_EVE_pst_DuplicateData(&scene.eve);
        BASE_EVE_ResolveListTrackRef(&scene.list);
        BASE_EVE_FreeTrack(&scene.tracks[c_t]);
    }
    else
    {
        scene.outP[0] = NEW_EVE_AllocateMainStruct(&scene.gao[1]);
        NEW_EVE_InitData(&scene.eve, &scene.list);
        scene.outP[1] = NEW_EVE_pst_DuplicateData(&scene.eve);
        NEW_EVE_ResolveListTrackRef(&scene.list);
        NEW_EVE_FreeTrack(&scene.tracks[c_t]);
    }
}

static void case_aiResolve(int side)
{
    tAINode* nodes = (tAINode*)&scene.data[4][8];
    int i;

    for (i = 0; i < 6; ++i)
    {
        nodes[i].val = i * 5 + 1;
        nodes[i].val2 = (int16_t)(i % 4);
        nodes[i].flags = 0;
        nodes[i].category = (char)((i == 5) ? 31 : (i % 3));
    }
    scene.tracks[4].flags |= 0x10;
    for (i = 0; i < NK; ++i)
    {
        scene.keys[4][i].flags2 = (int16_t)0x40;
        scene.keys[4][i].eptrdu.data = &scene.data[4][4];
    }
    if (side == 0)
        BASE_EVE_ResolveTrackRef(&scene.list, &scene.tracks[4]);
    else
        NEW_EVE_ResolveTrackRef(&scene.list, &scene.tracks[4]);
}

static void case_interpolateTime(int side)
{
    tGameObjectEventElem* key = &scene.keys[0][0];
    float* times = (float*)&scene.data[0][0];
    int n = 1 + (c_k % 4);
    int i;

    /* the first element's start field carries the count in its bits */
    memcpy(&times[0], &n, sizeof(int));
    for (i = 1; i < n; ++i)
    {
        times[i * 2] = 0.01f * i;
        times[i * 2 + 1] = 2.0f + i;
    }
    times[1] = 1.5f;
    key->eptrdu.size = (uint16_t)((20 << 1) | 1);
    key->eptrdu.type = 8;
    EVE_gpst_CurrentTrack = &scene.tracks[0];

    if (side == 0)
        scene.outF[0] = BASE_EVE_f_Event_InterpolationKey_InterpolateTime(key, c_f);
    else
        scene.outF[0] = NEW_EVE_f_Event_InterpolationKey_InterpolateTime(key, c_f);
}


/* ---- a second round, aimed at the paths the first round misses ---- */

/* Every key an interpolation key of the same type, so the whole
   position-interpolation block runs: the three Bezier orders, the
   mirror, and the two restored math calls.

   The mirror is only combined with a single control point on purpose:
   with two or three, the original reads keyPos[1] and keyPos[2] past
   the one-vector mirror local, so the result depends on the frame
   layout and the two copies legitimately differ. That is the defect
   this pass preserved, and this is where it shows. */
static void case_interpPlay(int side)
{
    int k;
    int mirror = (c_i & 1) && (c_type & 3) == 1;
    tGameObjectEventElem* key = &scene.keys[c_t][c_k];

    for (k = 0; k < NK; ++k)
    {
        scene.keys[c_t][k].flags2 = (int16_t)(0x80
                                              | (mirror ? 0x20 : 0)
                                              | (((c_i & 2) && k == 2) ? 0x800 : 0));
        scene.keys[c_t][k].eptrdu.size = (uint16_t)((20 << 1) | 1);
        scene.keys[c_t][k].eptrdu.type = (uint16_t)c_type;
        scene.keys[c_t][k].duration = (uint16_t)(1 + ((k + c_j) % 4));
    }
    if (c_i & 64)
    {
        /* the next key decides early: another kind, or no type at all */
        scene.keys[c_t][(c_k + 1) % NK].flags2 = (int16_t)((c_i & 128) ? 0x200 : 0x80);
        if (!(c_i & 128))
            scene.keys[c_t][(c_k + 1) % NK].eptrdu.type = 0xFFFF;
    }
    for (k = 0; k < NG; ++k)
        if (c_i & 256)
            scene.gao[k].flags |= 0x10000;
    if (c_i & 4)
    {
        /* the local-matrix path: the target's matrix is its own, and the
           reference object decides whether the ref matrix is applied */
        for (k = 0; k < NG; ++k)
            scene.gao[k].flags |= 0x400000;
    }
    if (c_i & 8)
    {
        /* a flash key, which is what EVE_b_Track_UseFlash looks for */
        for (k = 0; k < NK; ++k)
            scene.keys[c_t][k].flags2 |= 4;
    }
    scene.ext[1].extraFlag = (int16_t)((c_i & 16) ? 0x200 : 0);
    scene.ext[2].extraFlag = (int16_t)((c_i & 16) ? 0x400 : 0);
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentTrack = &scene.tracks[c_t];
    EVE_gpst_CurrentParam = &scene.states[c_t];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = &scene.refMtx;
    EVE_gpst_CurrentParam->currTime = c_f;
    EVE_gpst_CurrentParam->currElemKeyFrame = (uint16_t)c_k;
    if (c_i & 512)
        scene.gaoMtx[3].refGao = &scene.gao[1];   /* so the bone's two refs differ */
    if (c_i & 1024)
        EVE_gpst_CurrentTrack = NULL;

    if (side == 0)
        scene.outP[0] = BASE_EVE_Event_InterpolationKey_Play(key);
    else
        scene.outP[0] = NEW_EVE_Event_InterpolationKey_Play(key);
}

/* A key whose type carries the time block, with a well-formed one, so
   the interpolated-time path inside the player runs too. */
static void case_interpPlayTime(int side)
{
    float* times = (float*)&scene.data[c_t][48];
    int n = 1 + (c_k % 3);
    int i;

    memcpy(&times[0], &n, sizeof(int));
    for (i = 1; i < n; ++i)
    {
        times[i * 2] = 0.005f * i;
        times[i * 2 + 1] = 1.0f + i;
    }
    times[1] = 1.25f;
    c_type = (int16_t)(0x1B);   /* one control point, quaternion, time */
    case_interpPlay(side);
}

/* The key-frame walker's ends: the restart at the last frame, the
   bone-255 reset, the two AI-function exits and the no-loop tail. */
static void case_runEnds(int side)
{
    int k;
    tGameObjectEventsListElem* track = &scene.tracks[c_t];
    tAnimTracks* state = &scene.states[c_t];

    track->flags = (int16_t)((c_i & 1) ? 0x0002 : 0x0004);
    if (c_i & 32)
        track->trackElems = NULL;
    track->boneID = (uint16_t)((c_i & 2) ? 255 : 1);
    for (k = 0; k < NK; ++k)
    {
        /* 30 frames is half a second, which is exact in a float, so the
           restart head's frameTime == currTime can actually hold */
        scene.keys[c_t][k].duration = (uint16_t)((c_i & 4) ? 30 : 2);
        scene.keys[c_t][k].flags2 = (int16_t)((c_j & 1) ? 0x40 : 0x80);
        if (c_i & 8)
            scene.keys[c_t][k].flags2 |= 1;
        if (c_j & 2)
            scene.keys[c_t][k].flags2 |= (int16_t)((c_j & 4) ? 8 : 0x10);
        scene.keys[c_t][k].eptrdu.size = (uint16_t)((20 << 1) | 1);
        scene.keys[c_t][k].eptrdu.type = 0x13;
        if (c_j & 1)
            scene.keys[c_t][k].eptrdu.data = &scene.data[c_t][16];
    }
    if (c_j & 32)
    {
        /* the AI key's bone path: the track names a game object */
        track->gao = &scene.gao[1];
        scene.gao[1].flags |= 2;
        track->boneID = (uint16_t)((c_j & 64) ? 0xFFFF : 2);
    }
    state->currElemKeyFrame = (uint16_t)(NK - 1);
    state->currTime = (c_i & 4) ? 0.5f : (float)(2 * 1.0 / 60.0);
    state->flag = (int16_t)(1 | ((c_i & 8) ? 4 : 0));
    MOCK_aiStackValue = (c_j & 8) ? 1 : 0;
    if (c_i & 16)
    {
        /* A cursor one past the last key frame, which EVE_SetAllTimesTo
           can leave behind: EVE_i_SearchTime returns elemNum when the
           time lands at the end of the last key, and nothing clamps it.
           The walk then reads one key past the array. The step is kept
           small so it reads exactly one. */
        track->elemNum = 3;
        state->currElemKeyFrame = 3;
        TIM_gf_dt = 0.016666667f;
    }
    if (c_i & 64)
    {
        /* a label-jump-all key whose label resolves. Safe here because
           nothing restarts this walk; through EVE_b_RunAllTracks the
           same key does not terminate. */
        scene.keys[c_t][2].flags2 = (int16_t)0x400;
        scene.keys[c_t][0].flags2 = (int16_t)0x80;
        state->currElemKeyFrame = 2;
    }

    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentTrack = track;
    EVE_gpst_CurrentParam = state;
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = &scene.refMtx;
    scene.ext[0].eve = &scene.eve;
    scene.outF[0] = -1.0f;
    if (side == 0)
        scene.outI[0] = BASE_EVE_RunOneTrack(track, state, 1, (c_j & 16) ? NULL : &scene.outF[0]);
    else
        scene.outI[0] = NEW_EVE_RunOneTrack(track, state, 1, (c_j & 16) ? NULL : &scene.outF[0]);
}

/* The two track walkers' own ends: the LOD skips, the break that
   leaves the first loop, the backward second loop, and the null
   arguments. */
static void case_runWalkers(int side)
{
    scene.gao[0].if_this_not0_wont_render_gao = (uint8_t)((c_i & 1) ? 1 : 0);
    /* flags2 bit 1 set and bit 2 clear is the one combination that does
       not skip the animated tracks */
    scene.gao[0].flags2 = (c_i & 16) ? 2 : 1;
    if (c_i & 32)
    {
        scene.states[1].flag = 0;
        scene.states[2].flag = (int16_t)(1 | 2);
    }
    scene.gao[0].flags |= 0x4000000 | 4;
    scene.ext[1].extraFlag = (int16_t)((c_i & 2) ? 0x20 : 0);
    scene.actor.aniBitFieldLOD = (uint8_t)((c_i & 4) ? 0x8F : 0x80);
    scene.actor.ANILOD = (uint8_t)((c_j & 16) ? 0x08 : 0x00);
    scene.ext[0].extraFlag = (int16_t)((c_j & 32) ? 0x80 : 0x00);
    scene.tracks[1].boneID = (uint16_t)((c_i & 8) ? 0 : 1);
    scene.tracks[NT - 1].flags = (int16_t)((c_j & 1) ? 0x000A : 0x8002);
    scene.tracks[2].flags = (int16_t)((c_j & 2) ? 0x8002 : 0x000A);
    scene.ext[0].eve = &scene.eve;
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentRefMatrix = &scene.refMtx;
    scene.outF[0] = -1.0f;
    scene.outF[1] = -1.0f;

    scene.eve.tracks = (c_j & 4) ? NULL : &scene.states[0];
    scene.eve.eventListStruct = (c_j & 8) ? NULL : &scene.list;

    if (side == 0)
    {
        scene.outI[0] = BASE_EVE_b_RunAnimation(&scene.eve, &scene.outF[0]);
        scene.outI[1] = BASE_EVE_b_RunAllTracks(&scene.eve, &scene.outF[1]);
        scene.outI[2] = BASE_EVE_b_RunAnimation(NULL, &scene.outF[0]);
        scene.outI[3] = BASE_EVE_b_RunAllTracks(NULL, NULL);
    }
    else
    {
        scene.outI[0] = NEW_EVE_b_RunAnimation(&scene.eve, &scene.outF[0]);
        scene.outI[1] = NEW_EVE_b_RunAllTracks(&scene.eve, &scene.outF[1]);
        scene.outI[2] = NEW_EVE_b_RunAnimation(NULL, &scene.outF[0]);
        scene.outI[3] = NEW_EVE_b_RunAllTracks(NULL, NULL);
    }
}

/* Allocation, duplication, the free path and the main call. */
static void case_lifecycle(int side)
{
    static struct sWorld world;
    static tableElemStructPF eot[3];
    int t, k;

    /* The duplication takes a key's data length from eptrdu.size, which
       for the pointer form is the low half of the pointer itself, so a
       key with real data asks for a megabyte. Left null here; see the
       note in the pass record. */
    for (t = 0; t < NT; ++t)
        for (k = 0; k < NK; ++k)
            scene.keys[t][k].eptrdu.data = NULL;
    memset(&world, 0, sizeof(world));
    memset(eot, 0, sizeof(eot));
    eot[0].objectPtr = &scene.gao[0];
    eot[1].objectPtr = (c_i & 2) ? NULL : (void*)((uintptr_t)&scene.gao[1] | 1u);
    eot[2].objectPtr = &scene.gao[2];
    world.eot.eventGameObjectsTablePF.firstElemPtr = &eot[0];
    world.eot.eventGameObjectsTablePF.nextElemPtr = &eot[3];
    scene.ext[0].eve = &scene.eve;
    scene.ext[2].eve = &scene.eve;
    scene.gao[0].flags |= 0x4000000;
    scene.gao[0].distance = 1;
    scene.list.usageCount = (c_j & 1) ? 1 : 1000001;
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];

    if (side == 0)
    {
        BASE_EVE_Duplicate(&scene.gao[0], &scene.gao[3], c_i & 4);
        BASE_EVE_ResolveRefs(&scene.gao[2], c_i & 8);
        BASE_EVE_SynchronizeTracks(&scene.actor);
        BASE_EVE_MainCall(&world);
        BASE_EVE_FreeMainStruct(&scene.gao[3]);
        BASE_EVE_DeleteListTracks(&scene.list);
    }
    else
    {
        NEW_EVE_Duplicate(&scene.gao[0], &scene.gao[3], c_i & 4);
        NEW_EVE_ResolveRefs(&scene.gao[2], c_i & 8);
        NEW_EVE_SynchronizeTracks(&scene.actor);
        NEW_EVE_MainCall(&world);
        NEW_EVE_FreeMainStruct(&scene.gao[3]);
        NEW_EVE_DeleteListTracks(&scene.list);
    }
}

/* The loader's remaining key kinds: the AI-function key, the link key,
   and the delta-quaternion and time-block types. */
static void case_loadKinds(int side)
{
    uint32_t read;
    int kind = (c_i & 1) ? 0x40 : 0x80;
    int link = (c_i & 2) ? 0x20 : 0;

    lp = 0;
    memset(lbuf, 0xA5, sizeof(lbuf));
    pl(2);                      /* elemNum */
    if (kind == 0x40)
    {
        int k;
        for (k = 0; k < 2; ++k)
        {
            ps(3);                      /* duration */
            ps(kind | link);            /* flags2 */
            pl(2);                      /* node count */
            pb(1); pb(0); pb(0); pb(0); pb(2); pb(0); pb(0); pb(3);
            pb(5); pb(0); pb(0); pb(0); pb(1); pb(0); pb(0); pb(31);
        }
    }
    else
    {
        int k;
        for (k = 0; k < 2; ++k)
        {
            ps(3);
            ps(kind | link);
            if (link)
            {
                pl(0x00020001);         /* the packed track/key pair */
            }
            else
            {
                /* the delta-from-previous types read the previous key's
                   quaternion, so the first key always carries one */
                putKeyPayload(0, k ? (int)(uint16_t)c_type : 0x11);
            }
        }
    }
    MOCK_FileSet(lbuf, (int)sizeof(lbuf));

    scene.tracks[3].size = 192;
    scene.tracks[3].flags = 0;
    EVE_gpst_CurrentTrack = &scene.tracks[3];
    read = 0;
    if (side == 0)
        BASE_EVE_LoadListEvents(&scene.tracks[3], MOCK_FileBase(), &read, 0);
    else
        NEW_EVE_LoadListEvents(&scene.tracks[3], MOCK_FileBase(), &read, 0);
    scene.outI[0] = (int)read;
}

/* The two whole-track key players, with the knobs that pick their
   early returns and their reference-matrix path. */
static void case_animPlays(int side)
{
    int k;
    tGameObjectEventElem* key = &scene.keys[c_t][c_k];

    for (k = 0; k < NG; ++k)
        if (c_i & 1)
            scene.gao[k].flags |= 0x400000;
    scene.ext[1].extraFlag = (int16_t)((c_i & 2) ? 0x200 : 0);
    scene.ext[2].extraFlag = (int16_t)((c_i & 4) ? 0x400 : 0);
    scene.ext[3].extraFlag = (int16_t)((c_i & 8) ? 0x20 : 0);
    scene.gao[3].flags2 = (c_i & 16) ? -1 : 1;
    scene.actor.lod = (uint8_t)((c_i & 16) ? 0x08 : 0x00);
    scene.tracks[c_t].boneID = (uint16_t)((c_j & 1) ? 0xFFFF : ((c_j & 2) ? 99 : 3));
    if (c_i & 32)
        scene.gaoMtx[3].refGao2 = &scene.gao[0];   /* so the bone's ref is the current object */
    for (k = 0; k < NK; ++k)
    {
        scene.keys[c_t][k].duration = (uint16_t)((c_j & 4) ? 0 : (1 + k));
        scene.keys[c_t][k].flags2 = 0x80;
        scene.keys[c_t][k].eptrdu.size = (uint16_t)((20 << 1) | 1);
        scene.keys[c_t][k].eptrdu.type = (uint16_t)((c_j & 8) ? 0x03 : 0x13);
    }
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentTrack = &scene.tracks[c_t];
    EVE_gpst_CurrentParam = &scene.states[c_t];
    EVE_gpst_CurrentData = &scene.eve;
    EVE_gpst_CurrentListTracks = &scene.list;
    EVE_gpst_CurrentRefMatrix = &scene.refMtx;
    EVE_gpst_CurrentParam->currElemKeyFrame = (uint16_t)c_k;
    EVE_gpst_CurrentParam->currTime = c_f;
    /* the quaternion player blends the key's and the next key's
       quaternions without checking for them, so it is only called with
       a type that carries one */
    if (side == 0)
    {
        BASE_EVE_Anim_TranslationKey_Play(key);
        if (!(c_j & 8))
            BASE_EVE_Anim_QuaternionKey_Play(key);
    }
    else
    {
        NEW_EVE_Anim_TranslationKey_Play(key);
        if (!(c_j & 8))
            NEW_EVE_Anim_QuaternionKey_Play(key);
    }
}

/* The small branches the shaped scene never reaches: an event block
   with no tracks allocated yet, a paused or stopped track, a track with
   no keys at all, and the second of the two reference matrices. */
static void case_edges(int side)
{
    scene.eve.tracks = (c_i & 1) ? NULL : &scene.states[0];
    scene.eve.eventListStruct = (c_i & 2) ? NULL : &scene.list;
    scene.list.num = (uint16_t)((c_i & 4) ? NT : NT - 2);
    scene.states[0].flag = (int16_t)((c_i & 8) ? 0 : 1);
    /* a keyless track: the search's tail is only reachable with a zero
       count, and with a non-null block it then reads element -1 */
    scene.tracks[0].elemNum = (uint16_t)((c_j & 1) ? 0 : NK);
    if ((c_j & 1) && (c_j & 2))
        scene.tracks[0].trackElems = NULL;
    scene.gao[1].flags |= (c_j & 4) ? (0x4000000 | 0x10000) : 0;
    scene.ext[0].eve = &scene.eve;
    EVE_gpst_OwnerGAO = &scene.gao[0];
    EVE_gpst_CurrentGAO = &scene.gao[0];
    EVE_gpst_CurrentListTracks = &scene.list;
    scene.outF[0] = 0.125f;

    if (side == 0)
    {
        BASE_EVE_InitData(&scene.eve, &scene.list);
        BASE_EVE_SetRunningTrack(&scene.states[0], 1);
        BASE_EVE_SetRunningTrack(&scene.states[0], 0);
        scene.outI[0] = BASE_EVE_i_SearchTime(&scene.tracks[0], &scene.outF[0]);
        BASE_EVE_ReinitTrack(&scene.eve, 1);
        BASE_EVE_ReinitTrack(NULL, 0);
        BASE_EVE_b_RunAllTracks(&scene.eve, &scene.outF[1]);
    }
    else
    {
        NEW_EVE_InitData(&scene.eve, &scene.list);
        NEW_EVE_SetRunningTrack(&scene.states[0], 1);
        NEW_EVE_SetRunningTrack(&scene.states[0], 0);
        scene.outI[0] = NEW_EVE_i_SearchTime(&scene.tracks[0], &scene.outF[0]);
        NEW_EVE_ReinitTrack(&scene.eve, 1);
        NEW_EVE_ReinitTrack(NULL, 0);
        NEW_EVE_b_RunAllTracks(&scene.eve, &scene.outF[1]);
    }
}

/* -------------------------------------------------------------- main */

int main(void)
{
    static const float dts[4] = { 0.0f, 0.016666667f, 0.05f, 0.2f };
    static const float times[5] = { 0.0f, 0.004f, 0.0333f, 0.1f, 2.5f };
    int shape, d, t, k, i, j;
    unsigned seed = 1u;

    printf("events.c: frozen copy against the rewrite\n");

    /* the key-type offset walks, over every type word, both key forms */
    g_dt = 0.016666667f;
    g_playOnlyAnim = 0;
    g_forcePlayer = 0;
    g_everunning = 0;
    g_canFlash = 1;
    g_exiting = 0;
    buildScene(7u, 0);
    for (j = 0; j < 2; ++j)
    {
        c_j = j;
        for (i = 0; i < 65536; i += 1)
        {
            c_type = (int16_t)i;
            sprintf(g_case, "accessors form=%d type=%04x", j, (unsigned)(uint16_t)c_type);
            runBoth(case_accessors);
            if (g_fails > 20)
                break;
        }
    }

    for (shape = 0; shape < 32; ++shape)
    {
        for (d = 0; d < 4; ++d)
        {
            g_dt = dts[d];
            g_playOnlyAnim = (shape >> 1) & 1;
            g_forcePlayer = (shape >> 2) & 1;
            g_everunning = (shape >> 3) & 1;
            g_canFlash = (shape >> 4) & 1;
            g_exiting = shape & 1;
            buildScene(seed++, shape);

            for (t = 0; t < NT; ++t)
            {
                c_t = t;
                for (k = 0; k < 5; ++k)
                {
                    c_f = times[k];
                    sprintf(g_case, "searchTime shape=%d dt=%d t=%d f=%g", shape, d, t, (double)c_f);
                    runBoth(case_searchTime);
                    sprintf(g_case, "tracksTime shape=%d dt=%d t=%d f=%g", shape, d, t, (double)c_f);
                    runBoth(case_tracksTime);
                }
                for (i = 0; i < 6; ++i)
                {
                    c_i = i << 12;
                    sprintf(g_case, "searchLabel shape=%d t=%d label=%x", shape, t, (unsigned)c_i);
                    runBoth(case_searchLabel);
                }
                for (i = 0; i < 2; ++i)
                {
                    c_i = i;
                    sprintf(g_case, "state shape=%d t=%d on=%d", shape, t, i);
                    runBoth(case_state);
                }
                for (i = 0; i < 4; ++i)
                {
                    c_i = i;
                    sprintf(g_case, "flash shape=%d t=%d i=%d", shape, t, i);
                    runBoth(case_flash);
                }
                sprintf(g_case, "structs shape=%d t=%d", shape, t);
                runBoth(case_structs);
                sprintf(g_case, "aiResolve shape=%d t=%d", shape, t);
                runBoth(case_aiResolve);

                for (k = 0; k < NK; ++k)
                {
                    c_k = k;
                    for (j = 0; j < 4; ++j)
                    {
                        c_j = j;
                        for (i = 0; i < 2; ++i)
                        {
                            c_i = i;
                            sprintf(g_case, "runOneTrack shape=%d dt=%d t=%d k=%d j=%d end=%d",
                                    shape, d, t, k, j, i);
                            runBoth(case_runOneTrack);
                        }
                    }
                    for (j = 0; j < 4; ++j)
                    {
                        static const int lods[4] = { 0x00, 0x12, 0x16, 0x92 };
                        c_j = lods[j];
                        c_i = j;
                        sprintf(g_case, "runAnimTrack shape=%d dt=%d t=%d k=%d lod=%x",
                                shape, d, t, k, (unsigned)c_j);
                        runBoth(case_runAnimTrack);
                    }
                }
            }

            for (j = 0; j < 4; ++j)
            {
                c_j = j;
                for (i = 0; i < 2; ++i)
                {
                    c_i = i;
                    sprintf(g_case, "runAll shape=%d dt=%d j=%d i=%d", shape, d, j, i);
                    runBoth(case_runAll);
                }
            }

            /* the interpolated matrices and the players, over the type
               bits that pick a path through them */
            for (t = 0; t < NT; ++t)
            {
                c_t = t;
                for (k = 0; k < NK; ++k)
                {
                    c_k = k;
                    for (i = 0; i < 4; ++i)
                    {
                        c_i = i;
                        c_f = times[k % 5];
                        /* 0x10 always, so the quaternion getters do not
                           hand the blend a null; 0x8 has its own case,
                           because a random time block is not one */
                        c_type = (int16_t)(0x11 | (i & 1 ? 0x4 : 0) | (i & 2 ? 0x40 : 0)
                                           | ((k & 1) ? 0x20 : 0));
                        sprintf(g_case, "interp shape=%d t=%d k=%d i=%d type=%x",
                                shape, t, k, i, (unsigned)(uint16_t)c_type);
                        runBoth(case_interp);
                    }
                }
            }

            for (k = 0; k < 5; ++k)
            {
                c_f = times[k];
                c_k = k;
                sprintf(g_case, "interpolateTime shape=%d k=%d f=%g", shape, k, (double)c_f);
                runBoth(case_interpolateTime);
            }

            if (g_fails > 20)
                break;
        }
        if (g_fails > 20)
            break;
    }

    /* the second round */
    for (shape = 0; shape < 4; ++shape)
    {
        static const int ptypes[12] = { 0x11, 0x12, 0x13, 0x51, 0x52, 0x53,
                                        0x15, 0x16, 0x17, 0x05, 0x06, 0x07 };
        g_dt = dts[shape & 3];
        g_playOnlyAnim = (shape >> 1) & 1;
        g_forcePlayer = 0;
        g_everunning = shape & 1;
        g_canFlash = 1;
        g_exiting = shape & 1;
        buildScene(500u + shape, shape);

        for (t = 0; t < NT; ++t)
        {
            c_t = t;
            for (k = 0; k < 3; ++k)
            {
                c_k = k;
                for (i = 0; i < 2048; i += 61)
                {
                    c_i = i;
                    for (j = 0; j < 12; ++j)
                    {
                        c_type = (int16_t)(ptypes[j] | ((i & 2) ? 0x20 : 0));
                        c_j = j;
                        c_f = times[j % 5];
                        sprintf(g_case, "interpPlay shape=%d t=%d k=%d i=%d type=%02x",
                                shape, t, k, i, (unsigned)(uint16_t)c_type);
                        runBoth(case_interpPlay);
                    }
                    c_j = i;
                    c_f = times[k % 5];
                    sprintf(g_case, "interpPlayTime shape=%d t=%d k=%d i=%d", shape, t, k, i);
                    runBoth(case_interpPlayTime);
                }
            }
            for (i = 0; i < 128; ++i)
            {
                c_i = i;
                for (j = 0; j < 128; j += 3)
                {
                    c_j = j;
                    sprintf(g_case, "runEnds shape=%d t=%d i=%d j=%d", shape, t, i, j);
                    runBoth(case_runEnds);
                }
            }
        }
        for (i = 0; i < 64; ++i)
        {
            c_i = i;
            for (j = 0; j < 64; j += 3)
            {
                c_j = j;
                sprintf(g_case, "runWalkers shape=%d i=%d j=%d", shape, i, j);
                runBoth(case_runWalkers);
            }
        }
        for (i = 0; i < 16; ++i)
        {
            c_i = i;
            for (j = 0; j < 2; ++j)
            {
                c_j = j;
                sprintf(g_case, "lifecycle shape=%d i=%d j=%d", shape, i, j);
                runBoth(case_lifecycle);
            }
        }
        for (t = 0; t < NT; ++t)
        {
            c_t = t;
            for (k = 0; k < 3; ++k)
            {
                c_k = k;
                c_f = times[k];
                for (i = 0; i < 64; ++i)
                {
                    c_i = i;
                    for (j = 0; j < 16; ++j)
                    {
                        c_j = j;
                        sprintf(g_case, "animPlays shape=%d t=%d k=%d i=%d j=%d",
                                shape, t, k, i, j);
                        runBoth(case_animPlays);
                    }
                }
            }
        }
        for (i = 0; i < 16; ++i)
        {
            c_i = i;
            for (j = 0; j < 8; ++j)
            {
                c_j = j;
                sprintf(g_case, "edges shape=%d i=%d j=%d", shape, i, j);
                runBoth(case_edges);
            }
        }
        for (i = 0; i < 4; ++i)
        {
            c_i = i;
            for (j = 0; j < 8; ++j)
            {
                static const int ltypes[8] = { 0x0001, 0x0009, 0x0004, 0x0010,
                                               0x0100, 0x0200, 0x2000, 0x8000 };
                c_type = (int16_t)ltypes[j];
                sprintf(g_case, "loadKinds shape=%d i=%d type=%04x", shape, i, (unsigned)(uint16_t)c_type);
                runBoth(case_loadKinds);
            }
        }
        if (g_fails > 20)
            break;
    }

    /* the loaders, over the flag words that pick a path */
    for (shape = 0; shape < 4; ++shape)
    {
        buildScene(99u + shape, shape);
        for (i = 0; i < 16; ++i)
        {
            c_i = ((i & 1) ? 0x1000 : 0) | ((i & 2) ? 0x2000 : 0)
                | ((i & 4) ? 0x100 : 0) | ((i & 8) ? 0x200 : 0);
            for (j = 0; j < 18; ++j)
            {
                static const int types[18] = { 0x0000, 0x0001, 0x0003, 0x0004, 0x0010,
                                               0x0014, 0x0080, 0x0100, 0x0200, 0x0400,
                                               0x0800, 0x1000, 0x2000, 0x4000, 0x8000,
                                               0x0009, 0x000B, 0x0019 };
                c_type = (int16_t)types[j];
                sprintf(g_case, "keyLoad shape=%d flags=%x type=%x", shape, (unsigned)c_i, (unsigned)(uint16_t)c_type);
                runBoth(case_keyLoad);
            }
        }
        for (i = 0; i < 4; ++i)
        {
            c_i = i;
            for (j = 0; j < 4; ++j)
            {
                c_j = j;
                for (k = 0; k < 4; ++k)
                {
                    c_k = k;
                    static const int lflags[4] = { 0x0000, 0x0200, 0x0800, 0x2200 };
                    c_type = (int16_t)(lflags[j] | ((i & 2) ? 0x1000 : 0));
                    sprintf(g_case, "listLoad shape=%d i=%d j=%d k=%d flags=%x",
                            shape, i, j, k, (unsigned)(uint16_t)c_type);
                    runBoth(case_listLoad);
                }
            }
        }
        if (g_fails > 20)
            break;
    }

    printf("%d cases\n", g_cases);
    if (g_floatDiffs)
        printf("%d float words differed, worst abs %.3g, worst rel %.3g\n",
               g_floatDiffs, g_worstAbs, g_worstRel);
    if (g_fails)
    {
        printf("%d differences\n", g_fails);
        return 1;
    }
    printf("equivalent\n");
    return 0;
}
