#include "debugkeys.h"

#include "common.h"
#include "engine.h"
#include "world.h"
#include "gdi.h"
#include "soft.h"
#include "profiler.h"
#include "platform.h"
#include "platform/overlay.h"
#include "editorcamera.h"
#include "ai.h"
#include "tables.h"
#include "dbg.h"
#include "platform/syscompat.h"

void PC_EngineCheatFinal(void)
{
    tableElemStructPF* currWorld;
    profilerRasterStructElem* currRaster;

    if (PLT_b_KeyHeld(DBK_F7))
    {
        /* Through the interface, so the renderer in use answers: this
           function is in this renderer's file but the engine calls it
           whichever renderer is running, and calling this renderer's
           screenshot directly is why the key did nothing under the
           other one. */
        if (gpst_DX_stDD && gpst_DX_stDD->Request)
            gpst_DX_stDD->Request(26, NULL);
        PLT_WaitKeyRelease(DBK_F7);
    }
    if (PLT_b_KeyHeld(DBK_F6))
    {
        SOFT_ZList_Clear();
        AI_ReinitUniverse();

        for (currWorld = WOR_gst_Universe.univWorldTablePF.firstElemPtr; currWorld <= ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1; currWorld++)
        {
            if (!TAB_TableElemIsInvalid(currWorld->objectPtr)) {
                ENG_ReinitOneWorld((tWorld*)currWorld->objectPtr, 15);
            }
        }

        PLT_WaitKeyRelease(DBK_F6); 
    }
    if (PLT_b_KeyHeld(DBK_F10) && PLT_b_ShiftHeld() && PLT_b_CtrlHeld())
    {
        AI_CompiledAIEnabled = AI_CompiledAIEnabled == 0;
        
        PLT_WaitKeyRelease(DBK_F10);
    }
    if (PLT_b_KeyHeld(DBK_F1))
    {
        // on, to the detail page, and off again
        OVL_RastersCycleView();
        PLT_WaitKeyRelease(DBK_F1);
    }

    if (PLT_b_KeyHeld(DBK_F8))
    {
        // The free camera takes the view's follow parameter and the
        // input, and gives both back on the second press.
        ECAM_Toggle();
        PLT_WaitKeyRelease(DBK_F8);
    }

    if (PLT_b_KeyHeld(DBK_F9))
    {
        // Puts the free camera where the game's camera is looking,
        // once per press.
        ECAM_SyncToGameCamera();
        PLT_WaitKeyRelease(DBK_F9);
    }
    if (PLT_b_KeyHeld(DBK_F2))
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff & 0x800) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff &= ~0x800;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff |= 0x800;
            }
        }
        PLT_WaitKeyRelease(DBK_F2);
    }
    if (PLT_b_KeyHeld(DBK_F3))
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff & 0x10) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff &= ~0x10;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->drawMaskAllowedStuff |= 0x10;
            }
        }
        PLT_WaitKeyRelease(DBK_F3);
    }
    if (PLT_b_KeyHeld(DBK_F5))
    {
        if (MAI_gst_MainHandles.currentWorld)
        {
            if (MAI_gst_MainHandles.currentWorld->viewPtr->displayData)
            {
                if ((MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags & 0x20000000) != 0)
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags &= ~0x20000000;
                else
                    MAI_gst_MainHandles.currentWorld->viewPtr->displayData->presentationFlags |= 0x20000000;
            }
        }
        PLT_WaitKeyRelease(DBK_F5);
    }
    if (PLT_b_KeyHeld(DBK_F4))
    {
        if (PRO_rasterDetailPage)
        {
            currRaster = PRO_CurrRaster;
            if (PRO_CurrRaster)
            {
                while (1)
                {
                    currRaster = currRaster->prev;
                    if (!currRaster)
                        currRaster = DBG_profiler_first_raster;
                    if (currRaster == PRO_CurrRaster || strcasecmp(currRaster->module, PRO_CurrRaster->module))
                        break;
                }
            }
            else
            {
                currRaster = DBG_profiler_first_raster;
            }
            PRO_CurrRaster = currRaster;
        }
        PLT_WaitKeyRelease(DBK_F4);
    }
}
