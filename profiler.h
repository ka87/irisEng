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

// profiler window
extern int PRO_rasterWindowIsOn;
extern int PRO_rasterDetailPage;
extern profilerRasterStructElem* PRO_FirstRasterPtr;
extern profilerRasterStructElem* pointsToUnkRaster_dword_98F944;
extern profilerRasterStructElem* PRO_CurrRaster;
extern HWND PRO_rasterHWND;

void PRO_RasterAdd(profilerRasterStructElem* raster, char* group, char* subgroup, char* module, int kind, float value);
void PRO_RecordTimeDecRef(profilerRasterStructElem* raster);
void PRO_PerfCounterStartBlock(profilerRasterStructElem* raster);
void PRO_PerfCounterEndBlock(profilerRasterStructElem* raster);
void PRO_QPC_RecordTimeDecRef(int id);
void PRO_ClearAllRasters(profilerRasterStructElem* rasters);
void PRO_InitRasters(profilerRasterStructElem* rasters, char* subgroup);
void ENG_ForceStartRasters();
void profiler_dbg_init();
void PRO_InitModule();

// windows specific
void PRO_ProfilerWindowHander();
