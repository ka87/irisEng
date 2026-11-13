#include "profiler.h"
#include "time.h"
#include "win/wingdi.h"

int PRO_rasterWindowIsOn = 0; // game doesn't set it explicitly
int PRO_rasterDetailPage = 0; // --||--
profilerRasterStructElem* PRO_FirstRasterPtr = NULL; // --||--
profilerRasterStructElem* pointsToUnkRaster_dword_98F944 = NULL; // --||--
profilerRasterStructElem* PRO_CurrRaster = NULL; // --||--
HWND PRO_rasterHWND = NULL; // --||--
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

int PRO_DisplayTextsOnRasterWindow(HWND hWnd, HDC hdc, int y, profilerRasterStructElem* raster, int withSubgroup)
{
    unsigned int ticks;
    int x;
    int lineY;
    RECT rect;
    CHAR text[512];

    x = 10;
    GetClientRect(hWnd, &rect);
    SetBkColor(hdc, GetSysColor(15));
    lineY = y;
    if (pointsToUnkRaster_dword_98F944)
    {
        ExtTextOutA(hdc, 10, y, 0, 0, raster->module, strlen(raster->module), 0);
    }
    else
    {
        if (withSubgroup)
        {
            ExtTextOutA(hdc, 10, y, 0, 0, raster->subgroup, strlen(raster->subgroup), 0);
            if (!PRO_CurrRaster)
                lineY = y + 13;
        }
        x = 20;
        if (PRO_CurrRaster)
            x = 80;
        ExtTextOutA(hdc, x, lineY, 0, 0, raster->module, strlen(raster->module), 0);
    }
    switch (raster->unk1)
    {
    case 1:
        sprintf(text, "%d            ", *(int32_t*)&raster->unk2);
        break;
    case 2:
        strcpy(text, (char*)&raster->unk2);
        break;
    case 3:
        return lineY + 13;
    case 4:
        sprintf(text, "%.5f            ", raster->unk2);
        break;
    default:
        ticks = raster->sumTime;
        if (ticks)
        {
            raster->unk10 = ticks;
            raster->sumTime = 0;
        }
        else
        {
            ticks = raster->unk10;
        }
        sprintf(text, "  %.3f                 ",
                gfDBGRasterSetTo60 * ((double)ticks / (double)TIM_gul_TicksPerSecond));
        break;
    }
    ExtTextOutA(hdc, x + 150, lineY, 0, 0, text, strlen(text), 0);
    raster->unk7 = 0;
    return lineY + 13;
}

void PRO_ProfilerDisplayHandler(HWND hWnd, HDC hdc, int y)
{
    profilerRasterStructElem* start;
    profilerRasterStructElem* head;
    profilerRasterStructElem* scan;
    int displayed;
    int isSubgroupHead;

    if (!PRO_FirstRasterPtr)
        return;
    displayed = 0;
    for (start = DBG_profiler_first_raster; start; start = start->prev)
    {
        if (start->unk7 && !_stricmp(start->group, PRO_FirstRasterPtr->group))
        {
            for (head = start; head; head = head->prev)
            {
                if (!head->unk7
                    || _stricmp(head->group, PRO_FirstRasterPtr->group)
                    || pointsToUnkRaster_dword_98F944
                       && _stricmp(head->subgroup, pointsToUnkRaster_dword_98F944->subgroup))
                {
                    continue;
                }
                for (scan = head; scan; scan = scan->prev)
                {
                    if (!scan->unk7
                        || _stricmp(scan->group, PRO_FirstRasterPtr->group)
                        || _stricmp(scan->subgroup, head->subgroup))
                    {
                        continue;
                    }
                    if (PRO_CurrRaster && _stricmp(scan->module, PRO_CurrRaster->module))
                        continue;
                    isSubgroupHead = PRO_CurrRaster != NULL || scan == head;
                    y = PRO_DisplayTextsOnRasterWindow(hWnd, hdc, y, scan, isSubgroupHead);
                    if (PRO_CurrRaster)
                    {
                        displayed = 1;
                        break;
                    }
                }
                if (displayed)
                    break;
            }
            if (displayed)
                return;
        }
        start->unk7 = 1;
    }
}

void PRO_ProfilerWindowHander()
{
    HWND hwnd;
    HMODULE instance;
    HDC hdc;
    RECT rc;
    WNDCLASSA wndClass;
    CHAR chText[52];

    hwnd = PRO_rasterHWND;
    if (!PRO_rasterHWND)
    {
        wndClass.style = 0;
        wndClass.lpfnWndProc = DefWindowProcA;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = GetModuleHandleA(0);
        wndClass.hIcon = 0;
        wndClass.hCursor = 0;
        wndClass.hbrBackground = CreateSolidBrush(GetSysColor(15));
        wndClass.lpszMenuName = 0;
        wndClass.lpszClassName = "Rasters";
        RegisterClassA(&wndClass);
        instance = GetModuleHandleA(0);
        hwnd = CreateWindowExA(0x280u, "Rasters", "Rasters", 0x80C40000, 5, 5, 150, 50, MAI_gh_MainWindow, 0, instance, 0);
        PRO_rasterHWND = hwnd;
    }
    hdc = GetDC(hwnd);
    if (PRO_rasterDetailPage)
    {
        PRO_ProfilerDisplayHandler(PRO_rasterHWND, hdc, 0);
        ReleaseDC(PRO_rasterHWND, hdc);
    }
    else if (PRO_rasterWindowIsOn)
    {
        SetBkColor(hdc, GetSysColor(15));
        ShowWindow(PRO_rasterHWND, 5);
        if (gfDBGRasterSetTo60 * TIM_gf_dt > 1.0)
            SetTextColor(hdc, 0xFFu);
        sprintf(chText, "%f", gfDBGRasterSetTo60 * TIM_gf_dt);
        rc.bottom = 100;
        rc.right = 100;
        rc.top = 5;
        rc.left = 5;
        DrawTextA(hdc, chText, strlen(chText), &rc, 0);
        sprintf(chText, "%f", TIM_dtTicks * gfDBGRasterSetTo60);
        rc.bottom += 20;
        rc.top += 20;
        DrawTextA(hdc, chText, strlen(chText), &rc, 0);
        ReleaseDC(PRO_rasterHWND, hdc);
    }
    else
    {
        ShowWindow(PRO_rasterHWND, 0);
        ReleaseDC(PRO_rasterHWND, hdc);
    }
}
