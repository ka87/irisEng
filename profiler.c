#include "profiler.h"
#include "time.h"

int PRO_rasterWindowIsOn = 0; // game doesn't set it explicitly
int PRO_rasterDetailPage = 0; // --||--
profilerRasterStructElem* PRO_FirstRasterPtr = NULL; // --||--
profilerRasterStructElem* pointsToUnkRaster_dword_98F944 = NULL; // --||--
profilerRasterStructElem* PRO_CurrRaster = NULL; // --||--
float gfDBGRasterSetTo60 = 60.0f;

void PRO_RasterAdd(profilerRasterStructElem* raster, char* group, char* subgroup, char* module, int kind, float value)
{
    memset(raster, 0, sizeof(profilerRasterStructElem));
    raster->unk1 = kind;
    raster->group = group;
    raster->subgroup = subgroup;
    raster->module = module;
    raster->unk2 = value;
    if (DBG_profiler_first_raster)
    {
        DBG_profiler_first_raster->next = raster;
        raster->prev = DBG_profiler_first_raster;
        raster->next = 0;
        DBG_profiler_first_raster = raster;
    }
    else
    {
        DBG_profiler_first_raster = raster;
        raster->prev = 0;
        raster->next = 0;
    }
}

void PRO_InitModule()
{
    DBG_profiler_first_raster = 0;
}

void profiler_dbg_init()
{
    profilerRasterStructElem* raster;

    PRO_RasterAdd(&PRO_EngineDt, "Engine Infos", "Various Infos", "Engine Dt", 4, 0.0);
    PRO_RasterAdd(&PRO_EngineRealDT, "Engine Infos", "Various Infos", "Engine Real Dt", 4, 0.0);
    PRO_RasterAdd(&PRO_EngineFrames, "Engine Infos", "Various Infos", "Engine Frames", 4, 0.0);
    PRO_RasterAdd(&PRO_Synchro, "Trame Loop", "Game Engine", "Synchro", 0, 0.0);
    PRO_RasterAdd(&PRO_WinMsg, "Trame Loop", "Game Engine", "Win Msg", 0, 0.0);
    PRO_RasterAdd(&PRO_Input, "Trame Loop", "Game Engine", "Input", 0, 0.0);
    PRO_RasterAdd(&PRO_Display, "Trame Loop", "Game Engine", "Display", 0, 0.0);
    PRO_RasterAdd(&PRO_Activation, "Trame Loop", "Game Engine", "Activation", 0, 0.0);
    PRO_RasterAdd(&PRO_Visibility, "Trame Loop", "Game Engine", "Visibility", 0, 0.0);
    PRO_RasterAdd(&PRO_TablesManager, "Trame Loop", "Game Engine", "Tables MAnager", 0, 0.0);
    PRO_RasterAdd(&PRO_EOTCreation, "Trame Loop", "Game Engine", "EOT Creation", 0, 0.0);
    PRO_RasterAdd(&PRO_AI, "Trame Loop", "Game Engine", "AI", 0, 0.0);
    PRO_RasterAdd(&PRO_SnP, "Trame Loop", "Game Engine", "SnP", 0, 0.0);
    PRO_RasterAdd(&PRO_Events, "Trame Loop", "Game Engine", "Events", 0, 0.0);
    PRO_RasterAdd(&PRO_Animation, "Trame Loop", "Game Engine", "Animation", 0, 0.0);
    PRO_RasterAdd(&PRO_Dynamics, "Trame Loop", "Game Engine", "Dynamic", 0, 0.0);
    PRO_RasterAdd(&PRO_Collision, "Trame Loop", "Game Engine", "Collision", 0, 0.0);
    PRO_RasterAdd(&PRO_Records, "Trame Loop", "Game Engine", "Rec", 0, 0.0);
    PRO_RasterAdd(&PRO_GameEngine, "Trame Loop", "Game Engine", "Game Engine", 0, 0.0);
    PRO_RasterAdd(&PRO_FullLoop, "Trame Loop", "Game Engine", "Full Loop", 0, 0.0);
    for (raster = &PRO_RasterGroup.PRO_UserProfilerRastersEND;
         raster >= &PRO_RasterGroup.PRO_UserProfilerRasters[0];
         raster--)
    {
        PRO_RasterAdd(raster, "Trame Loop", "User", "User", 0, 0.0);
    }
}

void ENG_ForceStartRasters()
{
    profilerRasterStructElem* raster;

    PRO_EngineDt.insideCounter = 0;
    PRO_EngineRealDT.insideCounter = 0;
    PRO_EngineFrames.insideCounter = 0;
    PRO_Synchro.insideCounter = 0;
    PRO_WinMsg.insideCounter = 0;
    PRO_Input.insideCounter = 0;
    PRO_Display.insideCounter = 0;
    PRO_Activation.insideCounter = 0;
    PRO_Visibility.insideCounter = 0;
    PRO_TablesManager.insideCounter = 0;
    PRO_EOTCreation.insideCounter = 0;
    PRO_AI.insideCounter = 0;
    PRO_Events.insideCounter = 0;
    PRO_SnP.insideCounter = 0;
    PRO_Animation.insideCounter = 0;
    PRO_Dynamics.insideCounter = 0;
    PRO_Collision.insideCounter = 0;
    PRO_Records.insideCounter = 0;
    PRO_GameEngine.insideCounter = 0;
    PRO_FullLoop.insideCounter = 0;
    raster = &PRO_RasterGroup.PRO_UserProfilerRasters[0];
    do
    {
        raster->insideCounter = 0;
        // preserved: a byte count stepping a typed pointer, so one raster is cleared and the loop ends
        raster += sizeof(profilerRasterStructElem);
    } while ((int)raster < (int)&PRO_RasterGroup.unkRaster1.insideCounter);
}

void PRO_RecordTimeDecRef(profilerRasterStructElem* raster)
{
    if (raster->insideCounter)
    {
        --raster->insideCounter;
        if (!raster->insideCounter)
            raster->sumTime += TIM_GetTicksAsInt32() - raster->startTick;
    }
}

void PRO_PerfCounterStartBlock(profilerRasterStructElem* raster)
{
    if (!raster->insideCounter)
        raster->startTick = TIM_GetTicksAsInt32();
    ++raster->insideCounter;
}

void PRO_PerfCounterEndBlock(profilerRasterStructElem* raster)
{
    if (raster->insideCounter)
    {
        --raster->insideCounter;
        if (!raster->insideCounter)
            raster->sumTime += TIM_GetTicksAsInt32() - raster->startTick;
    }
}

void PRO_QPC_RecordTimeDecRef(int id)
{
    LARGE_INTEGER pc;

    QueryPerformanceCounter(&pc);
    if (PerfCounterStruct[id].QuadPart != 0)
        PerfCountertimeSumArray[id] += pc.QuadPart - PerfCounterStruct[id].QuadPart;
    PerfCounterStruct[id].QuadPart = 0;
}

void PRO_QPC_StartTime(int id)
{
    LARGE_INTEGER pc;

    QueryPerformanceCounter(&pc);
    PerfCounterStruct[id].QuadPart = pc.QuadPart;
}

void PRO_QPC_ResetSums(void)
{
    memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
}

int64_t PRO_i64_QPC(void)
{
    LARGE_INTEGER pc;

    QueryPerformanceCounter(&pc);
    return pc.QuadPart;
}

int64_t PRO_i64_QPCFrequency(void)
{
    LARGE_INTEGER freq;

    QueryPerformanceFrequency(&freq);
    return freq.QuadPart;
}

void PRO_ClearOneRaster(profilerRasterStructElem* raster)
{
    if (raster->next)
    {
        raster->next->prev = raster->prev;
    }
    else
    {
        if (!raster->prev)
            return;
        DBG_profiler_first_raster = raster->prev;
    }
    if (raster->prev)
        raster->prev->next = raster->next;
    raster->prev = 0;
    raster->next = 0;
}

void PRO_ClearAllRasters(profilerRasterStructElem* rasters)
{
    int i;

    for (i = 0; i < 47; i++)
        PRO_ClearOneRaster(rasters + i);
}

void PRO_InitRasters(profilerRasterStructElem* rasters, char* subgroup)
{
    PRO_RasterAdd(rasters + 9, "Display Bench", subgroup, "Number of triangles", 1, 0.0);
    PRO_RasterAdd(rasters + 10, "Display Bench", subgroup, "Number of generated sprites", 1, 0.0);
    PRO_RasterAdd(rasters + 4, "Display Bench", subgroup, "Number of lights", 1, 0.0);
    PRO_RasterAdd(rasters + 5, "Display Bench", subgroup, "Light : Nb computed color vertices", 1, 0.0);
    PRO_RasterAdd(rasters + 6, "Display Bench", subgroup, "Light : Nb vertex lighted by omni", 1, 0.0);
    PRO_RasterAdd(rasters + 7, "Display Bench", subgroup, "Light : Nb vertex lighted by spot", 1, 0.0);
    PRO_RasterAdd(rasters + 8, "Display Bench", subgroup, "Light : Nb vertex lighted by direct", 1, 0.0);
    PRO_RasterAdd(rasters + 3, "Display Bench", subgroup, "Number of objects", 1, 0.0);
    PRO_RasterAdd(rasters + 2, "Display Bench", subgroup, "Rendering counter", 1, 0.0);
    PRO_RasterAdd(rasters + 1, "Display Bench", subgroup, "Viewport height", 1, 0.0);
    PRO_RasterAdd(rasters, "Display Bench", subgroup, "Viewport width", 1, 0.0);
    PRO_RasterAdd(rasters + 11, "Display Bench", subgroup, "Number of sorted objects", 1, 0.0);
    PRO_RasterAdd(rasters + 12, "Display Bench", subgroup, "Number of sorted triangles", 1, 0.0);
    PRO_RasterAdd(rasters + 13, "Display Bench", subgroup, "Total Nb of GFX", 1, 0.0);
    PRO_RasterAdd(rasters + 14, "Display Bench", subgroup, "Nb of GFX displayed", 1, 0.0);
    PRO_RasterAdd(rasters + 41, "Display Rasters", subgroup, "Temp4", 0, 0.0);
    PRO_RasterAdd(rasters + 40, "Display Rasters", subgroup, "Temp3", 0, 0.0);
    PRO_RasterAdd(rasters + 39, "Display Rasters", subgroup, "Temp2", 0, 0.0);
    PRO_RasterAdd(rasters + 38, "Display Rasters", subgroup, "Temp1", 0, 0.0);
    PRO_RasterAdd(rasters + 35, "Display Rasters", subgroup, "GL : End", 0, 0.0);
    PRO_RasterAdd(rasters + 34, "Display Rasters", subgroup, "GL : Begin", 0, 0.0);
    PRO_RasterAdd(rasters + 36, "Display Rasters", subgroup, "GL : Flip", 0, 0.0);
    PRO_RasterAdd(rasters + 37, "Display Rasters", subgroup, "GL : Clear", 0, 0.0);
    PRO_RasterAdd(rasters + 33, "Display Rasters", subgroup, "MatDIT : multi texture UV's", 0, 0.0);
    PRO_RasterAdd(rasters + 32, "Display Rasters", subgroup, "MatDIT : multi texture", 0, 0.0);
    PRO_RasterAdd(rasters + 31, "Display Rasters", subgroup, "MatDIT : draw triangles", 0, 0.0);
    PRO_RasterAdd(rasters + 30, "Display Rasters", subgroup, "MatDIT : set render state", 0, 0.0);
    PRO_RasterAdd(rasters + 29, "Display Rasters", subgroup, "MatDIT : single material", 0, 0.0);
    PRO_RasterAdd(rasters + 28, "Display Rasters", subgroup, "MatDIT : Prepare mat", 0, 0.0);
    PRO_RasterAdd(rasters + 42, "Display Rasters", subgroup, "GeoRender : Compute MRM", 0, 0.0);
    PRO_RasterAdd(rasters + 43, "Display Rasters", subgroup, "GeoRender : Sprite generation", 0, 0.0);
    PRO_RasterAdd(rasters + 44, "Display Rasters", subgroup, "GeoRender : Shadow generation", 0, 0.0);
    PRO_RasterAdd(rasters + 27, "Display Rasters", subgroup, "GeoRender : Draw IT", 0, 0.0);
    PRO_RasterAdd(rasters + 26, "Display Rasters", subgroup, "GroRender : render", 0, 0.0);
    PRO_RasterAdd(rasters + 25, "Display Rasters", subgroup, "GroRender : Set view matrix", 0, 0.0);
    PRO_RasterAdd(rasters + 24, "Display Rasters", subgroup, "GroRender : Push matrix", 0, 0.0);
    PRO_RasterAdd(rasters + 23, "Display Rasters", subgroup, "GroRender : Look At", 0, 0.0);
    PRO_RasterAdd(rasters + 17, "Display Rasters", subgroup, "Build light list", 0, 0.0);
    PRO_RasterAdd(rasters + 18, "Display Rasters", subgroup, "Compute vertex color", 0, 0.0);
    PRO_RasterAdd(rasters + 19, "Display Rasters", subgroup, "Compute all light", 0, 0.0);
    PRO_RasterAdd(rasters + 20, "Display Rasters", subgroup, "Compute omni light", 0, 0.0);
    PRO_RasterAdd(rasters + 21, "Display Rasters", subgroup, "Compute spot light", 0, 0.0);
    PRO_RasterAdd(rasters + 22, "Display Rasters", subgroup, "Compute direct light", 0, 0.0);
    PRO_RasterAdd(rasters + 16, "Display Rasters", subgroup, "After display", 0, 0.0);
    PRO_RasterAdd(rasters + 15, "Display Rasters", subgroup, "Before display", 0, 0.0);
    PRO_RasterAdd(rasters + 45, "Display Rasters", subgroup, "GFX", 0, 0.0);
    PRO_RasterAdd(rasters + 46, "Display Rasters", subgroup, "ZList", 0, 0.0);
}
