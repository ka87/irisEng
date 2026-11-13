#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "photo_mocks.h"

/* The frozen copy and the rewrite, in one binary. Only two of the
   eight are in the header, so the rest are declared the way both
   copies define them. */
void BASE_IMG_Log4Debugging(char* msg);
void NEW_IMG_Log4Debugging(char* msg);
void BASE_IMG_LogOutput(char type, tGameObjectMain* gao, float a, float b, float c, float d);
void NEW_IMG_LogOutput(char type, tGameObjectMain* gao, float a, float b, float c, float d);
int BASE_IMG_ModifierObjectAnalyser(tGameObjectMain* gao, tModifier* mdf, tWorld* world,
                                    tGdi_DisplayData* dd, int missionMask, int checkVisibility,
                                    int rawInfo, int ignoreMission, int* code);
int NEW_IMG_ModifierObjectAnalyser(tGameObjectMain* gao, tModifier* mdf, tWorld* world,
                                   tGdi_DisplayData* dd, int missionMask, int checkVisibility,
                                   int rawInfo, int ignoreMission, int* code);
int BASE_IMG_ObjectAnalyser(tGameObjectMain* gao, tWorld* world, tGdi_DisplayData* dd,
                            int missionMask, int checkVisibility, int rawInfo,
                            int ignoreMission, int* code);
int NEW_IMG_ObjectAnalyser(tGameObjectMain* gao, tWorld* world, tGdi_DisplayData* dd,
                           int missionMask, int checkVisibility, int rawInfo,
                           int ignoreMission, int* code);
int BASE_IMG_Analyser(tWorld* world, unsigned int mission);
int NEW_IMG_Analyser(tWorld* world, unsigned int mission);
int BASE_IMG_ModifierObjectAnalyser_Bis(tGameObjectMain* gao, tModifier* mdf, tWorld* world,
                                        tGdi_DisplayData* dd, int checkVisibility,
                                        int ignoreMission, int* code);
int NEW_IMG_ModifierObjectAnalyser_Bis(tGameObjectMain* gao, tModifier* mdf, tWorld* world,
                                       tGdi_DisplayData* dd, int checkVisibility,
                                       int ignoreMission, int* code);
int BASE_IMG_ObjectAnalyser_Bis(tGameObjectMain* gao, tWorld* world, tGdi_DisplayData* dd,
                                int checkVisibility, int ignoreMission, int* code);
int NEW_IMG_ObjectAnalyser_Bis(tGameObjectMain* gao, tWorld* world, tGdi_DisplayData* dd,
                               int checkVisibility, int ignoreMission, int* code);
unsigned int BASE_IMG_Analyser_Bis(tWorld* world, int mission);
unsigned int NEW_IMG_Analyser_Bis(tWorld* world, int mission);

extern int BASE_IMG_Log;
extern int NEW_IMG_Log;
extern int BASE_IMG_PhotoSummary;
extern int NEW_IMG_PhotoSummary;
extern struct sGameObjectMain* BASE_IMG_Gao;
extern struct sGameObjectMain* NEW_IMG_Gao;

static tPhMockCall s_baseLog[PHMOCK_LOG_MAX];
static int s_baseLogLen;
static int s_baseResult;
static unsigned int s_baseResultU;
static int s_baseCode;
static int s_baseSummary;
static void* s_baseGao;
static int s_baseLogFlag;
static Mtx4x4 s_baseViewMtx;
static Mtx4x4 s_newViewMtx;

/* The globals the two copies keep apart. IMG_Log, IMG_PhotoSummary
   and IMG_Gao are defined in photo.h, so each side has its own. */
static void setGlobals(int side)
{
    if (side)
    {
        NEW_IMG_Log = IMG_Log;
        NEW_IMG_PhotoSummary = IMG_PhotoSummary;
        NEW_IMG_Gao = IMG_Gao;
    }
    else
    {
        BASE_IMG_Log = IMG_Log;
        BASE_IMG_PhotoSummary = IMG_PhotoSummary;
        BASE_IMG_Gao = IMG_Gao;
    }
}

static void readGlobals(int side, int* logFlag, int* summary, void** gao)
{
    *logFlag = side ? NEW_IMG_Log : BASE_IMG_Log;
    *summary = side ? NEW_IMG_PhotoSummary : BASE_IMG_PhotoSummary;
    *gao = side ? (void*)NEW_IMG_Gao : (void*)BASE_IMG_Gao;
}

/* ---- the five drivers ------------------------------------------------- */

enum { RUN_MDF, RUN_OBJ, RUN_ANALYSER, RUN_MDF_BIS, RUN_OBJ_BIS, RUN_ANALYSER_BIS };

typedef struct sArgs
{
    int which;
    int obj;
    int missionMask;
    int checkVisibility;
    int rawInfo;
    int ignoreMission;
    int wantCode;
    unsigned int mission;
    int scenePattern;
    unsigned int seed;
} tArgs;

static void runSide(const tArgs* ar, int side, int* result, unsigned int* resultU, int* code)
{
    int local = -77;
    int* codePtr = ar->wantCode ? &local : NULL;

    PHMOCK_BuildScene(ar->seed, ar->scenePattern);
    PHMOCK_Reset();
    setGlobals(side);
    *result = 0;
    *resultU = 0;

    switch (ar->which)
    {
    case RUN_MDF:
        *result = side
            ? NEW_IMG_ModifierObjectAnalyser(&PHMOCK_objects[ar->obj], &PHMOCK_mdfs[ar->obj],
                                             &PHMOCK_world, &PHMOCK_dd, ar->missionMask,
                                             ar->checkVisibility, ar->rawInfo,
                                             ar->ignoreMission, codePtr)
            : BASE_IMG_ModifierObjectAnalyser(&PHMOCK_objects[ar->obj], &PHMOCK_mdfs[ar->obj],
                                              &PHMOCK_world, &PHMOCK_dd, ar->missionMask,
                                              ar->checkVisibility, ar->rawInfo,
                                              ar->ignoreMission, codePtr);
        break;
    case RUN_OBJ:
        *result = side
            ? NEW_IMG_ObjectAnalyser(&PHMOCK_objects[ar->obj], &PHMOCK_world, &PHMOCK_dd,
                                     ar->missionMask, ar->checkVisibility, ar->rawInfo,
                                     ar->ignoreMission, codePtr)
            : BASE_IMG_ObjectAnalyser(&PHMOCK_objects[ar->obj], &PHMOCK_world, &PHMOCK_dd,
                                      ar->missionMask, ar->checkVisibility, ar->rawInfo,
                                      ar->ignoreMission, codePtr);
        break;
    case RUN_ANALYSER:
        *result = side ? NEW_IMG_Analyser(&PHMOCK_world, ar->mission)
                       : BASE_IMG_Analyser(&PHMOCK_world, ar->mission);
        break;
    case RUN_MDF_BIS:
        *result = side
            ? NEW_IMG_ModifierObjectAnalyser_Bis(&PHMOCK_objects[ar->obj], &PHMOCK_mdfs[ar->obj],
                                                 &PHMOCK_world, &PHMOCK_dd, ar->checkVisibility,
                                                 ar->ignoreMission, codePtr)
            : BASE_IMG_ModifierObjectAnalyser_Bis(&PHMOCK_objects[ar->obj], &PHMOCK_mdfs[ar->obj],
                                                  &PHMOCK_world, &PHMOCK_dd, ar->checkVisibility,
                                                  ar->ignoreMission, codePtr);
        break;
    case RUN_OBJ_BIS:
        *result = side
            ? NEW_IMG_ObjectAnalyser_Bis(&PHMOCK_objects[ar->obj], &PHMOCK_world, &PHMOCK_dd,
                                         ar->checkVisibility, ar->ignoreMission, codePtr)
            : BASE_IMG_ObjectAnalyser_Bis(&PHMOCK_objects[ar->obj], &PHMOCK_world, &PHMOCK_dd,
                                          ar->checkVisibility, ar->ignoreMission, codePtr);
        break;
    default:
        *resultU = side ? NEW_IMG_Analyser_Bis(&PHMOCK_world, (int)ar->mission)
                        : BASE_IMG_Analyser_Bis(&PHMOCK_world, (int)ar->mission);
        break;
    }
    *code = local;
}

static void runCase(const char* name, const tArgs* ar)
{
    int result;
    unsigned int resultU;
    int code;
    int logFlag;
    int summary;
    void* gao;

    PHMOCK_SetCase(name);

    runSide(ar, 0, &result, &resultU, &code);
    s_baseResult = result;
    s_baseResultU = resultU;
    s_baseCode = code;
    readGlobals(0, &s_baseLogFlag, &s_baseSummary, &s_baseGao);
    s_baseViewMtx = PHMOCK_viewMtx;
    PHMOCK_SaveLog(s_baseLog, &s_baseLogLen);

    runSide(ar, 1, &result, &resultU, &code);
    readGlobals(1, &logFlag, &summary, &gao);
    s_newViewMtx = PHMOCK_viewMtx;

    PHMOCK_SameInt(s_baseResult, result, "return", name);
    PHMOCK_SameInt((int)s_baseResultU, (int)resultU, "return (Bis)", name);
    PHMOCK_SameInt(s_baseCode, code, "code", name);
    PHMOCK_SameInt(s_baseLogFlag, logFlag, "IMG_Log", name);
    PHMOCK_SameInt(s_baseSummary, summary, "IMG_PhotoSummary", name);
    PHMOCK_SameInt(s_baseGao == gao, 1, "IMG_Gao", name);
    PHMOCK_SameInt(memcmp(&s_baseViewMtx, &s_newViewMtx, sizeof(Mtx4x4)) == 0, 1,
                   "view matrix", name);
    PHMOCK_CompareLog(s_baseLog, s_baseLogLen, PHMOCK_log, PHMOCK_logLen, name);
}

/* ---- the sweeps ------------------------------------------------------- */

static const int missionMasks[] = { 0, -1, 0x01000000, 0x02000000, 0x03000000, 0x04000000 };

static void sweepPerObject(void)
{
    int which;
    int obj;
    unsigned m;
    int vis;
    int raw;
    int ign;
    int boxMask;
    int pattern;
    char name[160];
    tArgs ar;

    for (which = RUN_MDF; which <= RUN_OBJ_BIS; which++)
    {
        if (which == RUN_ANALYSER)
            continue;
        for (obj = 0; obj < PHMOCK_OBJECTS; obj++)
            for (m = 0; m < sizeof(missionMasks) / sizeof(missionMasks[0]); m++)
                for (vis = 0; vis < 2; vis++)
                    for (raw = 0; raw < 2; raw++)
                        for (ign = 0; ign < 2; ign++)
                            for (boxMask = 0; boxMask < 3; boxMask++)
                                for (pattern = 0; pattern < 3; pattern++)
                                {
                                    /* the two modifier entry points
                                       dereference the private data
                                       without a null test, so they
                                       are only driven on the scene
                                       where every object has one */
                                    if ((which == RUN_MDF || which == RUN_MDF_BIS)
                                        && pattern != 1)
                                        continue;
                                    PHMOCK_isAABBoxMask =
                                        boxMask == 0 ? -1 : (boxMask == 1 ? 0 : 0x55555);
                                    PHMOCK_cullPattern = (obj + m) % 6;
                                    PHMOCK_viewMtxIsCamera = (obj % 5) != 4;
                                    PHMOCK_rayHit = (obj % 3 == 0) ? -1
                                                    : ((obj % 3 == 1) ? -2 : obj);
                                    PHMOCK_shiftDown = (obj & 1);
                                    IMG_Log = (obj & 2) ? 1 : 0;
                                    IMG_PhotoSummary = obj * 3;
                                    IMG_Gao = NULL;
                                    memset(&ar, 0, sizeof(ar));
                                    ar.which = which;
                                    ar.obj = obj;
                                    ar.missionMask = missionMasks[m];
                                    ar.checkVisibility = vis;
                                    ar.rawInfo = raw;
                                    ar.ignoreMission = ign;
                                    ar.wantCode = (obj % 4) != 3;
                                    ar.scenePattern = pattern;
                                    ar.seed = 0x1000u + obj;
                                    sprintf(name,
                                            "fn=%d obj=%d mask=%X vis=%d raw=%d ign=%d box=%d pat=%d",
                                            which, obj, missionMasks[m], vis, raw, ign,
                                            boxMask, pattern);
                                    runCase(name, &ar);
                                }
    }
}

static const int walkMissions[] = { -2, -1, 0, 1, 2, 3, 4, 5, 0x101, 0x201, 0x301 };

static void sweepWalks(void)
{
    unsigned mi;
    int mission;
    int boxMask;
    int pattern;
    int shift;
    int ray;
    int cull;
    char name[160];
    tArgs ar;

    for (mi = 0; mi < sizeof(walkMissions) / sizeof(walkMissions[0]); mi++)
        for (boxMask = 0; boxMask < 3; boxMask++)
            for (pattern = 0; pattern < 3; pattern++)
                for (shift = 0; shift < 2; shift++)
                    for (ray = 0; ray < 3; ray++)
                        for (cull = 0; cull < 6; cull++)
                        {
                            mission = walkMissions[mi];
                            PHMOCK_isAABBoxMask =
                                boxMask == 0 ? -1 : (boxMask == 1 ? 0 : 0x55555);
                            PHMOCK_cullPattern = cull;
                            PHMOCK_rayHit = ray == 0 ? -1 : (ray == 1 ? -2 : 3);
                            PHMOCK_shiftDown = shift;
                            IMG_Log = 0;
                            IMG_PhotoSummary = 0;
                            IMG_Gao = NULL;

                            memset(&ar, 0, sizeof(ar));
                            ar.which = RUN_ANALYSER;
                            ar.mission = (unsigned int)mission;
                            ar.scenePattern = pattern;
                            ar.seed = 0x2000u + mission;
                            sprintf(name, "analyser m=%d box=%d pat=%d sh=%d ray=%d cull=%d",
                                    mission, boxMask, pattern, shift, ray, cull);
                            runCase(name, &ar);

                            ar.which = RUN_ANALYSER_BIS;
                            sprintf(name, "analyser_bis m=%d box=%d pat=%d sh=%d ray=%d cull=%d",
                                    mission, boxMask, pattern, shift, ray, cull);
                            runCase(name, &ar);
                        }
}

/* The byte-set walk in IMG_Analyser_Bis keeps three distinct low
   bytes. Driving it needs results whose low byte repeats, differs and
   overflows the three slots, which the design-field pattern below
   produces. */
static void sweepByteSet(void)
{
    int lowByteSpread;
    char name[128];
    tArgs ar;

    for (lowByteSpread = 1; lowByteSpread <= 6; lowByteSpread++)
    {
        PHMOCK_isAABBoxMask = -1;
        PHMOCK_cullPattern = 4;
        PHMOCK_rayHit = -1;
        PHMOCK_shiftDown = 0;
        IMG_Log = 0;
        IMG_PhotoSummary = 0;
        IMG_Gao = NULL;
        PHMOCK_designOverride = lowByteSpread;
        memset(&ar, 0, sizeof(ar));
        ar.which = RUN_ANALYSER_BIS;
        ar.mission = 0;
        ar.scenePattern = 0;
        ar.seed = 0x3000u + lowByteSpread;
        sprintf(name, "byte set spread=%d", lowByteSpread);
        runCase(name, &ar);
    }
    PHMOCK_designOverride = 0;
}

/* IMG_Analyser_Bis stops the walk on a design word whose top byte is
   4, and stops it and records the object when the top byte reaches
   100. Nothing else in the scene produces either. */
static void sweepHighByte(void)
{
    int override;
    int cull;
    char name[128];
    tArgs ar;

    for (override = -1; override >= -2; override--)
        for (cull = 0; cull < 6; cull++)
        {
            PHMOCK_designOverride = override;
            PHMOCK_isAABBoxMask = -1;
            PHMOCK_cullPattern = cull;
            PHMOCK_rayHit = -1;
            PHMOCK_shiftDown = 0;
            IMG_Log = 0;
            IMG_PhotoSummary = 0;
            IMG_Gao = NULL;
            memset(&ar, 0, sizeof(ar));
            ar.which = RUN_ANALYSER_BIS;
            ar.mission = 0;
            ar.scenePattern = 0;
            ar.seed = 0x4000u + cull;
            sprintf(name, "high byte override=%d cull=%d", override, cull);
            runCase(name, &ar);

            ar.which = RUN_ANALYSER;
            ar.mission = 1;
            sprintf(name, "high byte analyser override=%d cull=%d", override, cull);
            runCase(name, &ar);
        }
    PHMOCK_designOverride = 0;
}

int main(void)
{
    PHMOCK_Init();
    sweepPerObject();
    sweepWalks();
    sweepByteSet();
    sweepHighByte();
    PHMOCK_Summary("photo.c");
    if (PHMOCK_logOverflow)
        printf("  WARNING: the call log overflowed\n");
    return PHMOCK_failures ? 1 : 0;
}
