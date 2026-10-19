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

void PRO_RasterAdd(profilerRasterStructElem* a1, char* a2, char* a3, char* a4, int a5, float a6)
{
    memset(a1, 0, sizeof(profilerRasterStructElem));
    a1->unk1 = a5;
    a1->group = a2;
    a1->subgroup = a3;
    a1->module = a4;
    a1->unk2 = a6;
    if (DBG_profiler_first_raster)
    {
        DBG_profiler_first_raster->next = a1;
        a1->prev = DBG_profiler_first_raster;
        a1->next = 0;
        DBG_profiler_first_raster = a1;
    }
    else
    {
        DBG_profiler_first_raster = a1;
        a1->prev = 0;
        a1->next = 0;
    }
}

void PRO_InitModule()
{
    DBG_profiler_first_raster = 0;
}

void profiler_dbg_init()
{
    profilerRasterStructElem* v0;

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
    v0 = &PRO_RasterGroup.PRO_UserProfilerRastersEND;
    do
        PRO_RasterAdd(v0--, "Trame Loop", "User", "User", 0, 0.0);
    while ((int)v0 >= (int)&PRO_RasterGroup);    
}

void ENG_ForceStartRasters()
{
    profilerRasterStructElem* v0;

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
    v0 = &PRO_RasterGroup.PRO_UserProfilerRasters[0];
    do
    {
        v0->insideCounter = 0;
        v0 += sizeof(profilerRasterStructElem);
    } while ((int)v0 < (int)&PRO_RasterGroup.unkRaster1.insideCounter);
}

void PRO_RecordTimeDecRef(profilerRasterStructElem* a1)
{
    __int16 v1;
    __int16 v2;

    v1 = a1->insideCounter;
    if (v1)
    {
        v2 = v1 - 1;
        a1->insideCounter = v2;
        if (!v2)
            a1->sumTime += TIM_GetTicksAsInt32() - a1->startTick;
    }
}

void PRO_PerfCounterStartBlock(profilerRasterStructElem* a1)
{
    if (!a1->insideCounter)
        a1->startTick = TIM_GetTicksAsInt32();
    ++a1->insideCounter;
}

void PRO_PerfCounterEndBlock(profilerRasterStructElem* prse)
{
    if (prse->insideCounter)
    {
        --prse->insideCounter;
        if (!prse->insideCounter)
            prse->sumTime += TIM_GetTicksAsInt32() - prse->startTick;
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

void PRO_ClearOneRaster(profilerRasterStructElem* a1)
{
    profilerRasterStructElem* v1;
    profilerRasterStructElem* v2;

    v1 = a1->next;
    if (v1)
    {
        v1->prev = a1->prev;
    }
    else
    {
        if (!a1->prev)
            return;
        DBG_profiler_first_raster = a1->prev;
    }
    v2 = a1->prev;
    if (v2)
        v2->next = a1->next;
    a1->prev = 0;
    a1->next = 0;
}

void PRO_ClearAllRasters(profilerRasterStructElem* a1)
{
    PRO_ClearOneRaster(a1);
    PRO_ClearOneRaster(a1 + 1);
    PRO_ClearOneRaster(a1 + 2);
    PRO_ClearOneRaster(a1 + 3);
    PRO_ClearOneRaster(a1 + 4);
    PRO_ClearOneRaster(a1 + 5);
    PRO_ClearOneRaster(a1 + 6);
    PRO_ClearOneRaster(a1 + 7);
    PRO_ClearOneRaster(a1 + 8);
    PRO_ClearOneRaster(a1 + 9);
    PRO_ClearOneRaster(a1 + 10);
    PRO_ClearOneRaster(a1 + 11);
    PRO_ClearOneRaster(a1 + 12);
    PRO_ClearOneRaster(a1 + 13);
    PRO_ClearOneRaster(a1 + 14);
    PRO_ClearOneRaster(a1 + 15);
    PRO_ClearOneRaster(a1 + 16);
    PRO_ClearOneRaster(a1 + 17);
    PRO_ClearOneRaster(a1 + 18);
    PRO_ClearOneRaster(a1 + 19);
    PRO_ClearOneRaster(a1 + 20);
    PRO_ClearOneRaster(a1 + 21);
    PRO_ClearOneRaster(a1 + 22);
    PRO_ClearOneRaster(a1 + 23);
    PRO_ClearOneRaster(a1 + 24);
    PRO_ClearOneRaster(a1 + 25);
    PRO_ClearOneRaster(a1 + 26);
    PRO_ClearOneRaster(a1 + 27);
    PRO_ClearOneRaster(a1 + 28);
    PRO_ClearOneRaster(a1 + 29);
    PRO_ClearOneRaster(a1 + 30);
    PRO_ClearOneRaster(a1 + 31);
    PRO_ClearOneRaster(a1 + 32);
    PRO_ClearOneRaster(a1 + 33);
    PRO_ClearOneRaster(a1 + 34);
    PRO_ClearOneRaster(a1 + 35);
    PRO_ClearOneRaster(a1 + 36);
    PRO_ClearOneRaster(a1 + 37);
    PRO_ClearOneRaster(a1 + 38);
    PRO_ClearOneRaster(a1 + 39);
    PRO_ClearOneRaster(a1 + 40);
    PRO_ClearOneRaster(a1 + 41);
    PRO_ClearOneRaster(a1 + 42);
    PRO_ClearOneRaster(a1 + 43);
    PRO_ClearOneRaster(a1 + 44);
    PRO_ClearOneRaster(a1 + 45);
    PRO_ClearOneRaster(a1 + 46);
}

void PRO_InitRasters(profilerRasterStructElem* a1, char* a2)
{
    PRO_RasterAdd(a1 + 9, "Display Bench", a2, "Number of triangles", 1, 0.0);
    PRO_RasterAdd(a1 + 10, "Display Bench", a2, "Number of generated sprites", 1, 0.0);
    PRO_RasterAdd(a1 + 4, "Display Bench", a2, "Number of lights", 1, 0.0);
    PRO_RasterAdd(a1 + 5, "Display Bench", a2, "Light : Nb computed color vertices", 1, 0.0);
    PRO_RasterAdd(a1 + 6, "Display Bench", a2, "Light : Nb vertex lighted by omni", 1, 0.0);
    PRO_RasterAdd(a1 + 7, "Display Bench", a2, "Light : Nb vertex lighted by spot", 1, 0.0);
    PRO_RasterAdd(a1 + 8, "Display Bench", a2, "Light : Nb vertex lighted by direct", 1, 0.0);
    PRO_RasterAdd(a1 + 3, "Display Bench", a2, "Number of objects", 1, 0.0);
    PRO_RasterAdd(a1 + 2, "Display Bench", a2, "Rendering counter", 1, 0.0);
    PRO_RasterAdd(a1 + 1, "Display Bench", a2, "Viewport height", 1, 0.0);
    PRO_RasterAdd(a1, "Display Bench", a2, "Viewport width", 1, 0.0);
    PRO_RasterAdd(a1 + 11, "Display Bench", a2, "Number of sorted objects", 1, 0.0);
    PRO_RasterAdd(a1 + 12, "Display Bench", a2, "Number of sorted triangles", 1, 0.0);
    PRO_RasterAdd(a1 + 13, "Display Bench", a2, "Total Nb of GFX", 1, 0.0);
    PRO_RasterAdd(a1 + 14, "Display Bench", a2, "Nb of GFX displayed", 1, 0.0);
    PRO_RasterAdd(a1 + 41, "Display Rasters", a2, "Temp4", 0, 0.0);
    PRO_RasterAdd(a1 + 40, "Display Rasters", a2, "Temp3", 0, 0.0);
    PRO_RasterAdd(a1 + 39, "Display Rasters", a2, "Temp2", 0, 0.0);
    PRO_RasterAdd(a1 + 38, "Display Rasters", a2, "Temp1", 0, 0.0);
    PRO_RasterAdd(a1 + 35, "Display Rasters", a2, "GL : End", 0, 0.0);
    PRO_RasterAdd(a1 + 34, "Display Rasters", a2, "GL : Begin", 0, 0.0);
    PRO_RasterAdd(a1 + 36, "Display Rasters", a2, "GL : Flip", 0, 0.0);
    PRO_RasterAdd(a1 + 37, "Display Rasters", a2, "GL : Clear", 0, 0.0);
    PRO_RasterAdd(a1 + 33, "Display Rasters", a2, "MatDIT : multi texture UV's", 0, 0.0);
    PRO_RasterAdd(a1 + 32, "Display Rasters", a2, "MatDIT : multi texture", 0, 0.0);
    PRO_RasterAdd(a1 + 31, "Display Rasters", a2, "MatDIT : draw triangles", 0, 0.0);
    PRO_RasterAdd(a1 + 30, "Display Rasters", a2, "MatDIT : set render state", 0, 0.0);
    PRO_RasterAdd(a1 + 29, "Display Rasters", a2, "MatDIT : single material", 0, 0.0);
    PRO_RasterAdd(a1 + 28, "Display Rasters", a2, "MatDIT : Prepare mat", 0, 0.0);
    PRO_RasterAdd(a1 + 42, "Display Rasters", a2, "GeoRender : Compute MRM", 0, 0.0);
    PRO_RasterAdd(a1 + 43, "Display Rasters", a2, "GeoRender : Sprite generation", 0, 0.0);
    PRO_RasterAdd(a1 + 44, "Display Rasters", a2, "GeoRender : Shadow generation", 0, 0.0);
    PRO_RasterAdd(a1 + 27, "Display Rasters", a2, "GeoRender : Draw IT", 0, 0.0);
    PRO_RasterAdd(a1 + 26, "Display Rasters", a2, "GroRender : render", 0, 0.0);
    PRO_RasterAdd(a1 + 25, "Display Rasters", a2, "GroRender : Set view matrix", 0, 0.0);
    PRO_RasterAdd(a1 + 24, "Display Rasters", a2, "GroRender : Push matrix", 0, 0.0);
    PRO_RasterAdd(a1 + 23, "Display Rasters", a2, "GroRender : Look At", 0, 0.0);
    PRO_RasterAdd(a1 + 17, "Display Rasters", a2, "Build light list", 0, 0.0);
    PRO_RasterAdd(a1 + 18, "Display Rasters", a2, "Compute vertex color", 0, 0.0);
    PRO_RasterAdd(a1 + 19, "Display Rasters", a2, "Compute all light", 0, 0.0);
    PRO_RasterAdd(a1 + 20, "Display Rasters", a2, "Compute omni light", 0, 0.0);
    PRO_RasterAdd(a1 + 21, "Display Rasters", a2, "Compute spot light", 0, 0.0);
    PRO_RasterAdd(a1 + 22, "Display Rasters", a2, "Compute direct light", 0, 0.0);
    PRO_RasterAdd(a1 + 16, "Display Rasters", a2, "After display", 0, 0.0);
    PRO_RasterAdd(a1 + 15, "Display Rasters", a2, "Before display", 0, 0.0);
    PRO_RasterAdd(a1 + 45, "Display Rasters", a2, "GFX", 0, 0.0);
    PRO_RasterAdd(a1 + 46, "Display Rasters", a2, "ZList", 0, 0.0);
}

int PRO_DisplayTextsOnRasterWindow(HWND hWnd, HDC hdc, int y, profilerRasterStructElem* a4, int a5)
{
    COLORREF v5;
    UINT v7;
    int v8;
    profilerRasterStructElem* v9;
    UINT v10;
    int v11;
    unsigned int v15;
    double v16;
    double v17;
    int x;
    RECT Rect;
    CHAR String[512];

    x = 10;
    GetClientRect(hWnd, &Rect);
    v5 = GetSysColor(15);
    SetBkColor(hdc, v5);
    if (pointsToUnkRaster_dword_98F944)
    {
        v9 = a4;
        v10 = strlen(a4->module);
        v8 = y;
        ExtTextOutA(hdc, 10, y, 0, 0, a4->module, v10, 0);
    }
    else
    {
        if (a5)
        {
            v7 = strlen(a4->subgroup);
            v8 = y;
            ExtTextOutA(hdc, 10, y, 0, 0, a4->subgroup, v7, 0);
            if (!PRO_CurrRaster)
                v8 = y + 13;
        }
        else
        {
            v8 = y;
        }
        x = 20;
        if (PRO_CurrRaster)
            x = 80;
        ExtTextOutA(hdc, x, v8, 0, 0, a4->module, strlen(a4->module), 0);
        v9 = a4;
    }
    v11 = v9->unk1;
    if (v9->unk1 == 4)
    {
        sprintf(String, "%.5f            ", v9->unk2);
    LABEL_23:
        ExtTextOutA(hdc, x + 150, v8, 0, 0, String, strlen(String), 0);
        v9->unk7 = 0;
        return v8 + 13;
    }
    if (v11 == 1)
    {
        sprintf(String, "%d            ", *(int32_t*)&v9->unk2);
        goto LABEL_23;
    }
    if (v11 != 3)
    {
        if (v11 == 2)
        {
            strcpy(String, (char*)&v9->unk2);
        }
        else
        {
            v15 = v9->sumTime;
            if (v15)
            {
                v16 = (double)v15;
                v17 = (double)TIM_gul_TicksPerSecond;
                v9->unk10 = v15;
                v9->sumTime = 0;
            }
            else
            {
                v16 = (double)(unsigned int)v9->unk10;
                v17 = (double)TIM_gul_TicksPerSecond;
            }
            sprintf(String, "  %.3f                 ", gfDBGRasterSetTo60 * (v16 / v17));
        }
        goto LABEL_23;
    }
    return v8 + 13;
}

void PRO_ProfilerDisplayHandler(HWND hWnd, HDC hdc, int y)
{
    profilerRasterStructElem* v4;
    profilerRasterStructElem* v5;
    int v6;
    profilerRasterStructElem* v7;
    int v8;
    profilerRasterStructElem* v9;

    if (!PRO_FirstRasterPtr)
        return;
    v9 = DBG_profiler_first_raster;
    v8 = 0;
    if (!DBG_profiler_first_raster)
        return;
    while (1)
    {
        v4 = v9;
        if (v9->unk7)
        {
            if (!_stricmp(v9->group, PRO_FirstRasterPtr->group))
                break;
        }
    LABEL_27:
        v7 = v4->prev;
        v4->unk7 = 1;
        v9 = v7;
        if (!v7)
            return;
    }
    while (2)
    {
        if (!v4->unk7
            || _stricmp(v4->group, PRO_FirstRasterPtr->group)
            || pointsToUnkRaster_dword_98F944 && _stricmp(v4->subgroup, pointsToUnkRaster_dword_98F944->subgroup))
        {
            goto LABEL_23;
        }
        v5 = v4;
        do
        {
            if (v5->unk7
                && !_stricmp(v5->group, PRO_FirstRasterPtr->group)
                && !_stricmp(v5->subgroup, v4->subgroup))
            {
                if (PRO_CurrRaster)
                {
                    if (_stricmp(v5->module, PRO_CurrRaster->module))
                        goto LABEL_20;
                    if (PRO_CurrRaster)
                        goto LABEL_18;
                }
                if (v5 == v4)
                    LABEL_18:
                v6 = 1;
                else
                    v6 = 0;
                y = PRO_DisplayTextsOnRasterWindow(hWnd, hdc, y, v5, v6);
                if (PRO_CurrRaster)
                {
                    v8 = 1;
                    goto LABEL_22;
                }
            }
        LABEL_20:
            v5 = v5->prev;
        } while (v5);
        if (!v8)
            goto LABEL_23;
    LABEL_22:
        if (!PRO_FirstRasterPtr)
        {
        LABEL_23:
            v4 = v4->prev;
            if (!v4)
                break;
            continue;
        }
        break;
    }
    if (!v8 || !PRO_FirstRasterPtr)
    {
        v4 = v9;
        goto LABEL_27;
    }
}

void PRO_ProfilerWindowHander()
{
    HWND v0;
    COLORREF v1;
    HMODULE v2;
    HDC v3;
    COLORREF v4;
    RECT rc;
    WNDCLASSA WndClass;
    CHAR chText[52];

    v0 = PRO_rasterHWND;
    if (!PRO_rasterHWND)
    {
        WndClass.style = 0;
        WndClass.lpfnWndProc = DefWindowProcA;
        WndClass.cbClsExtra = 0;
        WndClass.cbWndExtra = 0;
        WndClass.hInstance = GetModuleHandleA(0);
        WndClass.hIcon = 0;
        WndClass.hCursor = 0;
        v1 = GetSysColor(15);
        WndClass.hbrBackground = CreateSolidBrush(v1);
        WndClass.lpszMenuName = 0;
        WndClass.lpszClassName = "Rasters";
        RegisterClassA(&WndClass);
        v2 = GetModuleHandleA(0);
        v0 = CreateWindowExA(0x280u, "Rasters", "Rasters", 0x80C40000, 5, 5, 150, 50, MAI_gh_MainWindow, 0, v2, 0);
        PRO_rasterHWND = v0;
    }
    v3 = GetDC(v0);
    if (PRO_rasterDetailPage)
    {
        PRO_ProfilerDisplayHandler(PRO_rasterHWND, v3, 0);
        ReleaseDC(PRO_rasterHWND, v3);
    }
    else if (PRO_rasterWindowIsOn)
    {
        v4 = GetSysColor(15);
        SetBkColor(v3, v4);
        ShowWindow(PRO_rasterHWND, 5);
        if (gfDBGRasterSetTo60 * TIM_gf_dt > 1.0)
            SetTextColor(v3, 0xFFu);
        sprintf(chText, "%f", gfDBGRasterSetTo60 * TIM_gf_dt);
        rc.bottom = 100;
        rc.right = 100;
        rc.top = 5;
        rc.left = 5;
        DrawTextA(v3, chText, strlen(chText), &rc, 0);
        sprintf(chText, "%f", TIM_dtTicks * gfDBGRasterSetTo60);
        rc.bottom += 20;
        rc.top += 20;
        DrawTextA(v3, chText, strlen(chText), &rc, 0);
        ReleaseDC(PRO_rasterHWND, v3);
    }
    else
    {
        ShowWindow(PRO_rasterHWND, 0);
        ReleaseDC(PRO_rasterHWND, v3);
    }
}