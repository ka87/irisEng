#pragma once
#include "common.h"

typedef struct profilerRasterStructElem
{
    int unk1;
    float unk2;
    int16_t insideCounter;
    int16_t unk3s2;
    char* group;
    char* subgroup;
    char* module;
    int unk7;
    int startTick;
    int sumTime;
    int unk10;
    struct profilerRasterStructElem* prev;
    struct profilerRasterStructElem* next;
} profilerRasterStructElem;

typedef struct stRastersGroupStruct
{
    profilerRasterStructElem PRO_UserProfilerRasters[6];
    profilerRasterStructElem PRO_COBFromVisual;
    profilerRasterStructElem unknownRaster[2];
    profilerRasterStructElem PRO_UserProfilerRastersEND;
    profilerRasterStructElem unkRaster1;
} tRastersGroupStruct;


profilerRasterStructElem* DBG_profiler_first_raster;

profilerRasterStructElem PRO_EngineDt;
profilerRasterStructElem PRO_EngineRealDT;
profilerRasterStructElem PRO_EngineFrames;
profilerRasterStructElem PRO_Synchro;
profilerRasterStructElem PRO_WinMsg;
profilerRasterStructElem PRO_Input;
profilerRasterStructElem PRO_Display;
profilerRasterStructElem PRO_Activation;
profilerRasterStructElem PRO_Visibility;
profilerRasterStructElem PRO_TablesManager;
profilerRasterStructElem PRO_EOTCreation;
profilerRasterStructElem PRO_AI;
profilerRasterStructElem PRO_SnP;
profilerRasterStructElem PRO_Events;
profilerRasterStructElem PRO_Animation;
profilerRasterStructElem PRO_Dynamics;
profilerRasterStructElem PRO_Collision;
profilerRasterStructElem PRO_Records;
profilerRasterStructElem PRO_GameEngine;
profilerRasterStructElem PRO_FullLoop;
profilerRasterStructElem PRO_ShortWay;
profilerRasterStructElem PRO_RayObjectDist;
profilerRasterStructElem PRO_RayObjectVector;

//profilerRasterStructElem PRO_UserProfilerRasters[6];
//profilerRasterStructElem PRO_COBFromVisual;
tRastersGroupStruct PRO_RasterGroup;

// windows
LARGE_INTEGER PerfCounterStruct[54];
int64_t PerfCountertimeSumArray[54];

/* What each of those 54 rasters is called on a performance display,
 * and its kind -- nought for a raster that is not shown at all.
 *
 * The table was in the Direct3D renderer's own file, which was its
 * home only because that renderer's display was the only reader. It
 * names the profiler's rasters, so it lives here and both displays
 * read it. The definition is still beside that renderer's display. */
typedef struct sRasterNames
{
    char name[32];
    int  unk1;
} tRasterNames;

extern tRasterNames rasterNames[];

// profiler window
extern int PRO_rasterWindowIsOn;
extern int PRO_rasterDetailPage;
extern profilerRasterStructElem* PRO_FirstRasterPtr;
extern profilerRasterStructElem* pointsToUnkRaster_dword_98F944;
extern profilerRasterStructElem* PRO_CurrRaster;
extern float gfDBGRasterSetTo60;

void PRO_RasterAdd(profilerRasterStructElem* raster, char* group, char* subgroup, char* module, int kind, float value);
void PRO_RecordTimeDecRef(profilerRasterStructElem* raster);
void PRO_PerfCounterStartBlock(profilerRasterStructElem* raster);
void PRO_PerfCounterEndBlock(profilerRasterStructElem* raster);
void PRO_QPC_RecordTimeDecRef(int id);
/* The other half of the one above: the clock read into a raster's
   start, so a block's cost can be taken without the caller touching
   the platform's counter. */
void PRO_QPC_StartTime(int id);
/* The per-frame sums, cleared. The performance display does this once
   it has shown them, which is what makes them a frame's worth rather
   than a run's. */
void PRO_QPC_ResetSums(void);
/* The platform's counter and its frequency, so a display can measure
   a frame without a platform call of its own. */
int64_t PRO_i64_QPC(void);
int64_t PRO_i64_QPCFrequency(void);
void PRO_ClearAllRasters(profilerRasterStructElem* rasters);
void PRO_InitRasters(profilerRasterStructElem* rasters, char* subgroup);
void ENG_ForceStartRasters();
void profiler_dbg_init();
void PRO_InitModule();
