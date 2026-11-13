#include "memory.h"
#include "gameobject.h"
#include "animation.h"
#include "events.h"
#include "world.h"
#include "inputoutput.h"
#include "modifiers.h"
#include "ai.h"
#include "ai/ai_worldfunctions.h"
#include "ai/ai_debugfunctions.h"
#include "dynamics.h"
#include "collision.h"
#include "time.h"
#include "snp.h"
#include "sectors.h"
#include "tables.h"
#include "textures.h"
#include "strings.h"
#include "bigfiles.h"
#include "win/wingdi.h"
#include "win/wininputoutput.h"
#include "textget.h"
#include "loading.h"
#include "gfx.h"
#include "engine.h"
#include "dbg.h"

int ENG_gb_ActiveSectorization = 1;
int ENG_gb_NeedToReinit = 0; // game doesn't set it explicitly
int ENG_PlatformID = 0; // game doesn't set it explicitly
int ENG_WindowedMode = 0; // game doesn't have it

void ENG_ReinitOneObject(struct sGameObjectMain* gao, int reinitType)
{
    tGameObjectBasic* basic;
    tGaoMatrix* gaoMtx;
    tGameObjectMain* refGao;
    char process;
    tGameObjectExtended* ext;
    int flags2Cleared;
    BOOL wasNotFlagged;
    int flags2Invis;
    int flags2Hie;
    tGameObjectExtended* ext2;

    basic = gao->basic;
    if (basic)
    {
        gaoMtx = basic->mtx;
        if (gaoMtx)
        {
            refGao = gaoMtx->refGao;
            if (refGao)
            {
                gaoMtx->refGao2 = refGao;
            }
            else
            {
                gao->flags &= ~0x400000u;
                MEM_Free(basic->mtx);
                gao->basic->mtx = 0;
            }
        }
    }
    if (reinitType != 13)
        OBJ_Reinit(gao);
    OBJ_ResetProcess(gao, WOR_gpst_CurrentWorld->aiCallCount, 32);
    OBJ_ResetProcess(gao, WOR_gpst_CurrentWorld->dynCallCount, 64);
    OBJ_ResetProcess(gao, WOR_gpst_CurrentWorld->hierarchyCallCount, 128);
    if ((gao->flags & 0x20) != 0)
        AI_ReinitInstance(gao->extended->AIInstance);
    if ((gao->flags & 0x40) != 0)
        gao->extended->fields->designDesFlags = gao->extended->fields->u13;
    if ((gao->flags & 0x2000000) != 0)
        EVE_ReinitData(gao->extended->eve);
    if ((gao->flags & 0x40000) != 0)
        WAY_ReinitLinks(gao->extended->way);
    if ((gao->flags & 0x8000) != 0)
        gao->extended->messages->count = 0;
    OBJ_ComputeBV(gao, 1);
    COL_Reinit(gao);
    if ((gao->flags & 2) != 0)
    {
        ANI_Reinit(gao);
        if (((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->actionKit)
            ACT_SetAction(gao, 0, 0);
    }
    OBJ_ChangeIdentityFlags(gao, gao->flags & ~4u, -1);
    process = gao->process;
    gao->flags2 &= ~0x4000000u;
    ext = gao->extended;
    gao->process = process & 0xFC | ((char)(process & 0xFC) >> 2) & 1 | ((char)(process & 0xFC | ((char)(process & 0xFC) >> 2) & 1) >> 2) & 2;
    if (ext)
    {
        ext->extraFlag &= 0xFFFFFFF7u; // LOBYTE, v. ~0x8
        gao->extended->sectorID = -1;
        gao->extended->sectorElemIdx = -1;
    }
    flags2Cleared = gao->flags2 & 0x7FFFFFFF;
    wasNotFlagged = (gao->flags2 & 0x20000000) == 0;
    gao->if_this_not0_wont_render_gao = 0;
    gao->flags2 = flags2Cleared;
    if (wasNotFlagged)
        flags2Invis = flags2Cleared & ~0x200000;
    else
        flags2Invis = flags2Cleared | 0x200000;
    gao->flags2 = flags2Invis;
    if ((flags2Invis & 0x40000000) != 0)
        flags2Hie = flags2Invis | 0x400000;
    else
        flags2Hie = flags2Invis & ~0x400000u;
    gao->flags2 = flags2Hie;
    ext2 = gao->extended;
    if (ext2)
        ext2->capa = ext2->capaBackup;
    SND_DetachObject(gao);
}

void ENG_CloseEngine()
{
    INO_Joystick_Close();
    SND_CloseModule();
    SAV_PhotoLastClose();
}

void ENG_OneWorldEngineCall(struct sWorld* world)
{
    tViewElem* currView;
    LARGE_INTEGER perfCounter;

    QueryPerformanceCounter(PerfCounterStruct);
    
    WOR_gpst_CurrentWorld = world;
    AI_EvalFunc_WORPreLoadTex_C();
    ENG_gb_MustRebuildDyn = 0;

    if (world->tablesManagerAndEOTNeeded)
    {
        QueryPerformanceCounter(&PerfCounterStruct[28]);
    
        WOR_World_ActivateObjects(world);
        
        PRO_QPC_RecordTimeDecRef(28);
        QueryPerformanceCounter(&PerfCounterStruct[29]);
        
        EOT_SetOfEOT_Build(&world->eot, &world->activatedObjectsTable4PF);
        
        PRO_QPC_RecordTimeDecRef(29);
        world->tablesManagerAndEOTNeeded = 0;
    }
    QueryPerformanceCounter(&PerfCounterStruct[30]);
    
    if (ENG_gb_FirstFrame)
        INT_SnP_InsertionSort(world->snp, 0);
    
    PRO_QPC_RecordTimeDecRef(30);
    if (!PRO_SnP.insideCounter)
        PRO_SnP.startTick = TIM_GetTicksAsInt32();
    ++PRO_SnP.insideCounter;
    QueryPerformanceCounter(&PerfCounterStruct[31]);
    
    MDF_MainApplyGen(&world->activatedObjectsTable4PF);
    
    PRO_QPC_RecordTimeDecRef(31);
    QueryPerformanceCounter(&PerfCounterStruct[30]);

    if (world->SnPRefreshNeeded)
    {
        INT_SnP_Refresh(world->snp, &world->activatedObjectsTable4PF);
        world->SnPRefreshNeeded = FALSE;
    }

    PRO_QPC_RecordTimeDecRef(30);
    if (PRO_SnP.insideCounter)
    {
        if (!--PRO_SnP.insideCounter)
            PRO_SnP.sumTime += TIM_GetTicksAsInt32() - PRO_SnP.startTick;
    }
    QueryPerformanceCounter(&PerfCounterStruct[32]);
    if (!PRO_Events.insideCounter)
        PRO_Events.startTick = TIM_GetTicksAsInt32();
    ++PRO_Events.insideCounter;

    ENG_gb_EVERunning = 1;
    EVE_MainCall(world);
    ENG_gb_EVERunning = 0;

    if (PRO_Events.insideCounter)
    {
        if (!--PRO_Events.insideCounter)
            PRO_Events.sumTime += TIM_GetTicksAsInt32() - PRO_Events.startTick;
    }
    PRO_QPC_RecordTimeDecRef(32);
    PRO_QPC_RecordTimeDecRef(0);
    QueryPerformanceCounter(&PerfCounterStruct[11]);
    if (!PRO_Animation.insideCounter)
        PRO_Animation.startTick = TIM_GetTicksAsInt32();
    ++PRO_Animation.insideCounter;

    ANI_MainCall(world);

    if (PRO_Animation.insideCounter)
    {
        if (!--PRO_Animation.insideCounter)
            PRO_Animation.sumTime += TIM_GetTicksAsInt32() - PRO_Animation.startTick;
    }
    PRO_QPC_RecordTimeDecRef(11);
    QueryPerformanceCounter(&PerfCounterStruct[14]);
    if (!PRO_AI.insideCounter)
        PRO_AI.startTick = TIM_GetTicksAsInt32();
    ++PRO_AI.insideCounter;

    ENG_gb_AIRunning = TRUE;
    AI_MainCall(world);
    ENG_gb_AIRunning = FALSE;

    if (PRO_AI.insideCounter)
    {
        if (!--PRO_AI.insideCounter)
            PRO_AI.sumTime += TIM_GetTicksAsInt32() - PRO_AI.startTick;
    }
    PRO_QPC_RecordTimeDecRef(14);
    QueryPerformanceCounter(PerfCounterStruct);
    QueryPerformanceCounter(&PerfCounterStruct[33]);

    OBJ_HierarchyMainCall(world);

    PRO_QPC_RecordTimeDecRef(33);
    QueryPerformanceCounter(&PerfCounterStruct[29]);

    if (ENG_gb_MustRebuildDyn)
    {
        EOT_SetOfEOT_OneBuild(&world->eot.dynamicObjectsTablePF, 4, &world->activatedObjectsTable4PF);
        ENG_gb_MustRebuildDyn = 0;
    }
    
    PRO_QPC_RecordTimeDecRef(29);
    QueryPerformanceCounter(&PerfCounterStruct[34]);
    if (!PRO_Dynamics.insideCounter)
        PRO_Dynamics.startTick = TIM_GetTicksAsInt32();
    ++PRO_Dynamics.insideCounter;
    
    DYN_MainCall(world);    
    
    if (!PRO_Dynamics.insideCounter)
        PRO_Dynamics.startTick = TIM_GetTicksAsInt32();
    ++PRO_Dynamics.insideCounter;
    PRO_QPC_RecordTimeDecRef(34);
    PRO_QPC_RecordTimeDecRef(0);
    QueryPerformanceCounter(&PerfCounterStruct[16]);
    if (!PRO_Collision.insideCounter)
        PRO_Collision.startTick = TIM_GetTicksAsInt32();
    ++PRO_Collision.insideCounter;
    
    COL_MainCall(world);
    
    if (PRO_Collision.insideCounter)
    {
        if (!--PRO_Collision.insideCounter)
            PRO_Collision.sumTime += TIM_GetTicksAsInt32() - PRO_Collision.startTick;
    }
    PRO_QPC_RecordTimeDecRef(16);
    QueryPerformanceCounter(&PerfCounterStruct[17]);
    if (!PRO_Records.insideCounter)
        PRO_Records.startTick = TIM_GetTicksAsInt32();
    ++PRO_Records.insideCounter;
        
    REC_MainCall(world, 0);
    
    if (PRO_Records.insideCounter)
    {
        if (!--PRO_Records.insideCounter)
            PRO_Records.sumTime += TIM_GetTicksAsInt32() - PRO_Records.startTick;
    }
    PRO_QPC_RecordTimeDecRef(17);
    QueryPerformanceCounter(PerfCounterStruct);
    QueryPerformanceCounter(&PerfCounterStruct[33]);
    
    OBJ_HierarchyMainCall(world);
    
    PRO_QPC_RecordTimeDecRef(33);
    PRO_QPC_RecordTimeDecRef(0);
    QueryPerformanceCounter(&PerfCounterStruct[14]);
    if (!PRO_AI.insideCounter)
        PRO_AI.startTick = TIM_GetTicksAsInt32();
    ++PRO_AI.insideCounter;
    
    AI_ExecCallbackAll(world, 2);
    
    if (PRO_AI.insideCounter)
    {
        if (!--PRO_AI.insideCounter)
            PRO_AI.sumTime += TIM_GetTicksAsInt32() - PRO_AI.startTick;
    }
    PRO_QPC_RecordTimeDecRef(14);
    QueryPerformanceCounter(&PerfCounterStruct[29]);
    
    if (ENG_gb_MustRebuildDyn)
    {
        EOT_SetOfEOT_OneBuild(&world->eot.dynamicObjectsTablePF, 4, &world->activatedObjectsTable4PF);
        ENG_gb_MustRebuildDyn = 0;
    }
    
    PRO_QPC_RecordTimeDecRef(29);
    QueryPerformanceCounter(PerfCounterStruct);
    QueryPerformanceCounter(&PerfCounterStruct[33]);
    
    OBJ_HierarchyMainCall(world);
    
    PRO_QPC_RecordTimeDecRef(33);
    QueryPerformanceCounter(&PerfCounterStruct[35]);
    
    for (currView = world->viewPtr; currView < &world->viewPtr[world->viewNum]; currView++)
    {
        if (currView->displayData && currView->viewFunctionPtr)
            currView->viewFunctionPtr(currView);
    }

    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[35].QuadPart)
        PerfCountertimeSumArray[35] += perfCounter.QuadPart - PerfCounterStruct[35].QuadPart;
    PerfCounterStruct[35].QuadPart = 0;
    QueryPerformanceCounter(&PerfCounterStruct[31]);
    
    MDF_MainUnApplyGen(&world->activatedObjectsTable4PF);
    
    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[31].QuadPart)
        PerfCountertimeSumArray[31] += perfCounter.QuadPart - PerfCounterStruct[31].QuadPart;
    PerfCounterStruct[31].QuadPart = 0;
    QueryPerformanceCounter(&PerfCounterStruct[36]);
    
    if (world->visibilityCallNeeded)
    {
        WOR_World_MakeObjectsVisible(world);
        world->visibilityCallNeeded = 0;
    }
    
    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[36].QuadPart)
        PerfCountertimeSumArray[36] += perfCounter.QuadPart - PerfCounterStruct[36].QuadPart;
    PerfCounterStruct[36].QuadPart = 0;
    
    ANI_BeforeRendering(world);
    ++ENG_gul_Loop;

    // inline
    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[0].QuadPart)
    {
        PerfCountertimeSumArray[0] += perfCounter.QuadPart - PerfCounterStruct[0].QuadPart;
    }
    PerfCounterStruct[0].QuadPart = 0;
}

void ENG_ReinitOneWorld(struct sWorld* world, int reinitType)
{
    int vn;
    tableElemStructPF* currElem;
    tGameObjectMain* currGao;
    BOOL hasFlashMatrix;

    if (world)
    {
        SND_StreamPrefetchFlushAll();

        TIM_gf_SpeedFactor = 1.0f;
        world->hierarchyCallCount = 0;
        world->aiCallCount = 0;
        world->dynCallCount = 0;
        ENG_gb_FirstFrame = 1;
        GFX_gpst_Current = 0;
        GRID_gi_Current = 0;
        GRID_gi_Dest = 0;
        *AI_MainActor = 0;
        
        AI_Reset();
        GRPSectClearAll();
        ResetPreloadTexAll();
        
        world->recordCount = 0;
        world->colCrossObjectNumber = 0;

        if (world->viewNum > 1)
        {
            for (vn = 1; vn < world->viewNum; vn++)
            {
                WOR_View_Close(&world->viewPtr[vn]);
                world->viewPtr[vn].flags &= ~1;
                world->viewPtr[vn].viewFunctionPtr = NULL;
            }
        }

        world->viewNum = 1;
        world->viewPtr->assignedGao = NULL;

        for (currElem = world->objectTablePF.firstElemPtr; currElem <= ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1; currElem++)
        {
            currGao = (tGameObjectMain*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(currGao) && (currGao->flags & 0x20000) == 0)
            {
                hasFlashMatrix = (currGao->flags2 & 0x4000000) != 0;
                if (reinitType != 20 || (currGao->flags2 & 0x4000000) != 0)
                {
                    ENG_ReinitOneObject(currGao, reinitType);
                    if (reinitType == 20 && hasFlashMatrix)
                        currGao->flags2 |= 0x4000000;
                }
            }
        }

        for (currElem = world->objectTablePF.firstElemPtr; currElem <= ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1; currElem++)
        {
            currGao = (tGameObjectMain*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(currGao) && (currGao->flags & 0x20000) != 0)
            {
                WOR_World_DetachObject(world, (tGameObjectMain*)currElem->objectPtr);
                INT_SnP_DetachObject(currGao, world);
                OBJ_GameObject_Remove(currGao, TRUE);
            }
        }

        WAY_ComputePoundNetworks(world);

        if (world->grid1Ptr)
            GRI_Reinit(world->grid1Ptr);
        if (world->grid2Ptr)
            GRI_Reinit(world->grid2Ptr);

        GFX_Free(&world->gfxCurrentList, reinitType);
        GFX_Free(&world->gfxCurrentListSortedAfterAE, reinitType);

        INT_SnP_Reinit(world);

        world->deletedObjNum = 0;
        AI_gf_Epsilon = 0.000099999997f;

        INO_Reinit();

        if (ENG_gf_SpecialTime == 0.0f)
            SAV_PhotoReinit();

        STR_3DStringList_Clear();

        if (gpst_DX_stDD)
            gpst_DX_stDD->fog.flags = 0;

        WOR_World_ActivateObjects(world);
        EOT_SetOfEOT_Build(&world->eot, &world->activatedObjectsTable4PF);

        for (vn = 0; vn < world->viewNum; vn++)
        {
            world->viewPtr[vn].flags |= 0x2;
            ++vn;
        }

        WOR_World_MakeObjectsVisible(world);
        SND_StopAll(reinitType);
        SND_StreamResolveSyncPrefetch();
        MEM_Defrag(0);
    }
}

void ENG_EngineCall()
{
    tableElemStructPF* elem;
    tableElemStructPF* lastElem;
    tWorld* world;
    int anyRan;
    tableElemStructPF* elem2;
    tableElemStructPF* elemSaved;
    tableElemStructPF* lastElem2;
    char firstPass;
    float special;
    float specialSaved;
    LARGE_INTEGER PerformanceCount;

    if (!PRO_TablesManager.insideCounter)
        PRO_TablesManager.startTick = TIM_GetTicksAsInt32();
    ++PRO_TablesManager.insideCounter;
    QueryPerformanceCounter(PerfCounterStruct);
    QueryPerformanceCounter(&PerfCounterStruct[26]);
    
    TAB_TablesManager_Call();
    
    PRO_QPC_RecordTimeDecRef(26);
    TIM_MeasureTicksPerSecond();
    if (PRO_TablesManager.insideCounter)
    {
        if (!--PRO_TablesManager.insideCounter)
            PRO_TablesManager.sumTime += TIM_GetTicksAsInt32() - PRO_TablesManager.startTick;
    }
    QueryPerformanceCounter(&PerfCounterStruct[27]);
    
    specialSaved = ENG_gf_SpecialTime;
    special = ENG_gf_SpecialTime;
    if (ENG_gf_SpecialTime == 0.0f
        || (elem = WOR_gst_Universe.univWorldTablePF.firstElemPtr,
            lastElem = ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1,
            ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr)) > ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1))
    {
        world = NULL;
    }
    else
    {
        do
        {
            world = (tWorld*)elem->objectPtr;
            if (!TAB_TableElemIsInvalid(world))
                ENG_ReinitOneWorld(world, 20);
            ++elem;
        } while (elem <= lastElem);
    }
    
    QueryPerformanceCounter(&PerformanceCount);
    if (PerfCounterStruct[27].QuadPart)
        PerfCountertimeSumArray[27] += PerformanceCount.QuadPart - PerfCounterStruct[27].QuadPart;
    
    anyRan = 0;
    
    PerfCounterStruct[27].QuadPart = 0;
    QueryPerformanceCounter(&PerformanceCount);
    if (PerfCounterStruct[0].QuadPart)
        PerfCountertimeSumArray[0] += PerformanceCount.QuadPart - PerfCounterStruct[0].QuadPart;
    PerfCounterStruct[0].QuadPart = 0;
    QueryPerformanceCounter(&PerfCounterStruct[14]);
    
    if (AIUnivIsRunning && WOR_gst_Universe.univAIInstance)
    {
        AI_gpst_CurrentInstanceUltra = WOR_gst_Universe.univAIInstance;
        AI_gpst_CurrentInstance = WOR_gst_Universe.univAIInstance;
        ENG_gb_AIRunning = TRUE;
        AI_RunInstance(WOR_gst_Universe.univAIInstance);
        ENG_gb_AIRunning = FALSE;
    }
    
    QueryPerformanceCounter(&PerformanceCount);
    if (PerfCounterStruct[14].QuadPart)
        PerfCountertimeSumArray[14] += PerformanceCount.QuadPart - PerfCounterStruct[14].QuadPart;
    elem2 = WOR_gst_Universe.univWorldTablePF.firstElemPtr;
    PerfCounterStruct[14].QuadPart = 0;
    
    firstPass = 1;
    do
    {
        if (special > 0.0f)
        {
            if (special >= ENG_gf_SpecialDT)
            {
                TIM_gf_dt = ENG_gf_SpecialDT;
                ENG_gb_LastSpecialTime = 0.0f;
            }
            else
            {
                ENG_gb_LastSpecialTime = 1.0f;
                TIM_gf_dt = special;
            }
        }

        elemSaved = elem2;
        lastElem2 = ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1;
        if (elem2 <= &(((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr))[-1]))
        {
            do
            {
                world = (tWorld*)elemSaved->objectPtr;
                if (elemSaved->objectPtr && ((uint8_t)world & 3) != 1 && (elem2[anyRan].objectFlags & 1) != 0)
                {
                    if (special != 0.0f)
                    {
                        ENG_gb_FirstFrame = firstPass;
                        world->tablesManagerAndEOTNeeded = 0;
                        world->visibilityCallNeeded = 0;
                        if (!firstPass)
                            world->SnPRefreshNeeded = FALSE;
                    }
                    ENG_OneWorldEngineCall(world);
                    elem2 = WOR_gst_Universe.univWorldTablePF.firstElemPtr;
                }
                ++elemSaved;
                ++anyRan;
            } while (elemSaved <= lastElem2);
            anyRan = 0;
        }
        firstPass = 0;
        special = special - ENG_gf_SpecialDT;
    } while (special > 0.0f);
    
    if (specialSaved != 0.0f)
    {
        world->tablesManagerAndEOTNeeded = 1;
        world->visibilityCallNeeded = 1;
        ENG_gf_SpecialTime = 0.0f;
        ENG_gf_SpecialDT = 0.0f;
    }
    
    WOR_ResetKillGO();
    // the load ran inside this frame, so the next frame's measured
    // time would include it. account it the same way the message
    // pump is accounted, so it is not played as game time.
    if (WOR_gb_FirstTrameAfterLoad)
    {
        float loadSeconds = TIM_f_Clock_TrueRead() - WOR_gf_LoadStartTime;
        // how long the load actually took, against the frame-time ceiling
        // of 0.26666668 that would otherwise be charged for it
        DBG_OutputDebugString("===== LOAD END    WOL 0x%08X  %f s (ceiling 0.266667) =====\n",
            WOR_gul_WorldKey, loadSeconds);
        TIM_SpentTimeWithHandlingWinMessages = loadSeconds
            + TIM_SpentTimeWithHandlingWinMessages;
    }
    WOR_gb_FirstTrameAfterLoad = FALSE;
    ENG_gb_FirstFrame = 0;
}

void ENG_GetStatsText(char* Buffer)
{
    unsigned int sizeOfHoles;
    int holesNum;

    MEM_GetHolesStat(&holesNum, &sizeOfHoles);
    sprintf(Buffer,
        "Engine %u ko -- Textures %u ko -- WH %u ko -- Gran %u ko -- Holes % u -- Size Holes %u ko",
        MEM_gst_MemoryInfo.usedMemoryByEngine >> 10,
        MEM_gst_MemoryInfo.usedMemoryByTextures >> 10,
        (MEM_gst_MemoryInfo.nextFreeMemoryPointer - MEM_gst_MemoryInfo.memoryStartPointer) / 1024,
        MEM_gst_MemoryInfo.totalUsedMemory >> 10,
        holesNum, sizeOfHoles >> 10);
}

void s_InitBeforeTrame()
{
    ENG_gp_Display = s_Display;
    ENG_gp_Input = INO_Update;
    if (WOR_gst_Universe.unknownReloadFlag != (uint8_t)-1)
    {
        if (WOR_gst_Universe.unknownReloadFlag == (uint8_t)-2)
        {
            AI_Reset();
            WOR_Universe_Open(BIG_gst.hdr.startKey);
        }
        else
        {
            AI_ReinitUniverse();
            ENG_ReinitOneWorld(MAI_gst_MainHandles.currentWorld, WOR_gst_Universe.unknownReloadFlag);
        }
        WOR_gst_Universe.unknownReloadFlag = -1;
    }
}

void s_EngineCheatFinal()
{
    PC_EngineCheatFinal();
}

void s_OneTrame()
{ 
    tableElemStructPF* currElem;
    tWorld* currWorld;
    LARGE_INTEGER perfCounter;

    if (!PRO_FullLoop.insideCounter)
        PRO_FullLoop.startTick = TIM_GetTicksAsInt32();
    ++PRO_FullLoop.insideCounter;

    s_HandleWinMessages();
    TIM_Clock_Update();

    if (ENG_gb_NeedToReinit)
    {
        SOFT_ZList_Clear();
        AI_ReinitUniverse();

        for (currElem = WOR_gst_Universe.univWorldTablePF.firstElemPtr; currElem <= ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1; currElem++)
        {
            currWorld = (tWorld*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(currWorld))
                ENG_ReinitOneWorld(currWorld, 15);
        }
    }

    ENG_gb_NeedToReinit = 0;
    s_EngineCheatFinal();
    
    QueryPerformanceCounter(&PerfCounterStruct[15]);
    
    if (ENG_gp_Input)
    {
        if (!PRO_Input.insideCounter)
            PRO_Input.startTick = TIM_GetTicksAsInt32();
        ++PRO_Input.insideCounter;

        ENG_gp_Input();

        if (PRO_Input.insideCounter)
        {
            if (!--PRO_Input.insideCounter)
                PRO_Input.sumTime += TIM_GetTicksAsInt32() - PRO_Input.startTick;
        }
    }

    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[15].QuadPart)
        PerfCountertimeSumArray[15] += perfCounter.QuadPart - PerfCounterStruct[15].QuadPart;
    PerfCounterStruct[15].QuadPart = 0;

    QueryPerformanceCounter(&PerfCounterStruct[18]);
    
    if (ENG_gp_Engine && !ENG_gb_ForcePauseEngine)
    {
        if (!PRO_GameEngine.insideCounter)
            PRO_GameEngine.startTick = TIM_GetTicksAsInt32();
        ++PRO_GameEngine.insideCounter;

        ENG_gp_Engine();

        if (PRO_GameEngine.insideCounter)
        {
            if (!--PRO_GameEngine.insideCounter)
                PRO_GameEngine.sumTime += TIM_GetTicksAsInt32() - PRO_GameEngine.startTick;
        }
    }

    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[18].QuadPart)
        PerfCountertimeSumArray[18] += perfCounter.QuadPart - PerfCounterStruct[18].QuadPart;
    PerfCounterStruct[18].QuadPart = 0;

    QueryPerformanceCounter(&PerfCounterStruct[19]);

    if (ENG_gp_Display && MAI_gst_MainHandles.displayData)
    {
        if (!PRO_Display.insideCounter)
            PRO_Display.startTick = TIM_GetTicksAsInt32();
        ++PRO_Display.insideCounter;

        ENG_gp_Display(MAI_gst_MainHandles.hwnd, MAI_gst_MainHandles.displayData);

        if (PRO_Display.insideCounter)
        {
            if (!--PRO_Display.insideCounter)
                PRO_Display.sumTime += TIM_GetTicksAsInt32() - PRO_Display.startTick;
        }
    }

    QueryPerformanceCounter(&perfCounter);
    if (PerfCounterStruct[19].QuadPart)
        PerfCountertimeSumArray[19] += perfCounter.QuadPart - PerfCounterStruct[19].QuadPart;
    PerfCounterStruct[19].QuadPart = 0;

    if (MAI_gst_MainHandles.displayData)
    {
        QueryPerformanceCounter(&PerfCounterStruct[25]);
        SND_Update(&MAI_gst_MainHandles.displayData->ct.cam.objectMtx);
        PRO_QPC_RecordTimeDecRef(25);

        QueryPerformanceCounter(&PerfCounterStruct[20]);
        TEX_Procedural_Update(MAI_gst_MainHandles.displayData);
        PRO_QPC_RecordTimeDecRef(20);
        
        TEX_Anim_Update();
    }

    if (!PRO_RasterGroup.unkRaster1.insideCounter)
        PRO_RasterGroup.unkRaster1.startTick = TIM_GetTicksAsInt32();
    ++PRO_RasterGroup.unkRaster1.insideCounter;

    PRO_ProfilerWindowHandler();
    
    if (PRO_RasterGroup.unkRaster1.insideCounter)
    {
        if (!--PRO_RasterGroup.unkRaster1.insideCounter)
            PRO_RasterGroup.unkRaster1.sumTime += TIM_GetTicksAsInt32() - PRO_RasterGroup.unkRaster1.startTick;
    }
    
    MEM_Defrag(1);
    
    if (PRO_FullLoop.insideCounter)
    {
        if (!--PRO_FullLoop.insideCounter)
            PRO_FullLoop.sumTime += TIM_GetTicksAsInt32() - PRO_FullLoop.startTick;
    }
}

BOOL sfnb_EndGame()
{
    if (ENG_gb_ExitApplication)
        return TRUE;
    else
        return (ENG_gb_ForceEndEngine != 0);
}

void ENG_InitEngine()
{
    ENG_gp_Engine = ENG_EngineCall;
    ENG_gb_FirstFrame = 1;
    TIM_Clock_Reset();
    SND_InitModule();
    TIM_InitModule();
    INO_Joystick_Init(MAI_gh_MainWindow);
    profiler_dbg_init();
    WOR_gst_Universe.unknownReloadFlag = -2;
}

void ENG_Loop()
{
    ENG_gb_EngineRunning = 1;
    ENG_gb_ExitApplication = 0;
    ENG_gb_ForceEndEngine = 0;
    ENG_gb_ForcePauseEngine = 0;
    ENG_ForceStartRasters();
    pcINO_dinput_mouse_and_keyb_acquire();
    while (!sfnb_EndGame())
    {
        s_InitBeforeTrame();
        s_OneTrame();
    }
    SND_StreamPrefetchFlushAll();
    SND_StopAll(0);
    ENG_gb_EngineRunning = 0;
}

void ENG_InitApplication()
{
    memset(MAI_gst_BIGFileName, 0, 260);
    MAI_gst_MainHandles.hwndSetAfterCreateWindow = 0;
    MAI_gst_MainHandles.hwnd = 0;
    MAI_gst_MainHandles.currentWorld = 0;
    BIG_gst.CLibFileHandle = 0;
    MAI_gst_MainHandles.displayData = 0;
    MEM_InitModule();
    PRO_InitModule();
    BAS_InitModule();
    BIG_InitModule();
    INO_InitModule();
    AI_InitModule();
    WAY_InitModule();
    MATH_InitModule();
    TAB_InitModule();
    GDI_InitModule();
    MDF_Modifier_Init();
    DYN_InitModule();
    TEXT_InitModule();
    COL_InitModule();
    // ANI_InitModule
    LOA_LoadSpecialArray();
    WOR_InitModule();    
    SAV_PhotoFirstInit();
}

void ENG_ParseCommandLine(const char* cmd)
{
    char next;
    char i;

    while (1)
    {
        if (*cmd == ' ')
        {
            do
                next = *++cmd;
            while (next == ' ');
        }
        if (*cmd != '/')
            break;
        if (!_strnicmp(++cmd, CMD_NOSOUND, strlen(CMD_NOSOUND)))
        {
            SND_gc_NoSound = 1;
            cmd += strlen(CMD_NOSOUND);
        }
        else
        {
            if (!_strnicmp(cmd, CMD_BIN, strlen(CMD_BIN)))
            {
                LOA_gb_SpeedMode = 1;
                cmd += strlen(CMD_BIN);
            }
        }
        if (!_strnicmp(cmd, CMD_DBLBF, strlen(CMD_DBLBF)))
        {
            ENG_IsDLBFEnabled = 1;
            cmd += strlen(CMD_DBLBF);
        }
        if (!_strnicmp(cmd, CMD_WINDOWED, strlen(CMD_WINDOWED)))
        {
            ENG_WindowedMode = 1;
            cmd += strlen(CMD_WINDOWED);
        }
        if (!_strnicmp(cmd, CMD_SINGLESTREAM, strlen(CMD_SINGLESTREAM)))
        {
            ENG_IsSingleStream = 1;
            ENG_ifSingleStreamThenEq2Otherwise8 = 2;
            cmd += strlen(CMD_SINGLESTREAM);
        }
        else
        {
            for (i = *cmd; i; i = *++cmd)
            {
                if (i == ' ')
                    break;
            }
        }
    }
    sscanf(cmd, "%s", MAI_gst_BIGFileName);
}

void ENG_CloseApplication()
{
    //MTX_CloseModule(); //leftover mpeg2 functions
    DYN_CloseModule();
    WOR_CloseModule();
    TEXT_CloseModule();
    BAS_CloseModule();
    BIG_CloseModule();
    INO_CloseModule();
    BIG_FreeGlobalBuffer();
    AI_EvalFunc_DbgTraceInt_C();
    TAB_CloseModule();
    GDI_CloseModule();
    WAY_CloseModule();
    LOA_KillSpecialArray();
    SAV_PhotoLastClose();
    MEM_CloseModule();
    CloseHandle(memMngEvent);
    memMngEvent = NULL;
}