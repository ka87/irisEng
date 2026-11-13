#include "tables.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "view.h"
#include "snp.h"
#include "profiler.h"
#include "gameobject.h"
#include "time.h"
#include "graphicsobjects/geo.h"
#include "graphicsobjects/geomaterial.h"
#include "modifiers/xmenmodifier.h"
#include "animation.h"
#include "engine.h"
#include "water.h"
#include "ai/ai_worldfunctions.h"
#include "ai/ai_viewfunctions.h"
#include "ai/ai_gfxfunctions.h"
#include "grid.h"
#include "ways.h"
#include "strings.h"
#include "graphicsobjects/staticlod.h"
#include "textget.h"
#include "cameraengine.h"
#include "gfx.h"
#include "modifiers/lazymodifier.h"
#include "modifiers/protexmodifier.h"
#include "gdi.h"
#include "win/wingdi.h"
#include "soft.h"
#include "inputoutput.h"
#include "textures.h"
#include "sound.h"
#include "sectors.h"
#include "win/wininputoutput.h"
#include "ai/ai_debugfunctions.h"
#include "dbg.h"
#include "world.h"

int WOR_gb_RealAllocWorld = 1;
int WOR_gb_FinalWorld = 1;
int WOR_gul_WorldText = 0xffffffff;
int WOR_gul_WorldKey = 0xffffffff;

void WOR_ActivateObjectIfFlagsAllow(tGameObjectMain* gao, tWorld* world)
{
    if ((gao->flags2 & 0x1) == 0)
    {
        if (ENG_gf_SpecialTime == 0.0f)
        {
            if ((gao->flags2 & 0x4400000) == 0 && (gao->flags2 & 0x10) == 0)
            {
                TAB_AddElemToTablePF(&world->activatedObjectsTable4PF, gao, gao->flags);
                gao->flags2 |= 0x1;
            }
        }
        else if ((gao->flags2 & 0x4000000) != 0)
        {
            TAB_AddElemToTablePF(&world->activatedObjectsTable4PF, gao, gao->flags);
            gao->flags2 |= 0x1;
        }
    }
}

void WOR_World_DetachObject(struct sWorld* world, struct sGameObjectMain* gao)
{
    tableElemStructPF* currElem;
    tGameObjectMain* currGao;

    if ((gao->flags2 & 0x40) != 0)
    {
        for (currElem = world->objectTablePF.firstElemPtr; currElem < (tableElemStructPF*) world->objectTablePF.nextElemPtr; currElem++)
        {
            currGao = (tGameObjectMain*)currElem->objectPtr;
            if (currElem->objectPtr)
            {
                if (((uint8_t)currGao & 3) != 1 && (currGao->flags & 0x400000) != 0)
                {
                    if (currGao->basic && currGao->basic->mtx && currGao->basic->mtx->refGao2 == gao)
                    {
                        currGao->basic->mtx->refGao2 = NULL;
                        currGao->basic->mtx->refGao = NULL;
                        currGao->flags &= ~0x400000;
                        MEM_Free(currGao->basic->mtx);
                        currGao->basic->mtx = NULL;
                    }
                }
            }
        }
    }
    TAB_PFtable_RemoveElemWithPointer(&world->objectTablePF, gao);
    TAB_PFtable_RemoveElemWithPointer(&world->activatedObjectsTable4PF, gao);
    TAB_PFtable_RemoveElemWithPointer(&world->visibleGameObjectsPF, gao);
    if ((gao->flags & 0x10) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->lightObjectsTablePF, gao);
    if ((gao->flags & 0x40000000) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->objectsWithBVMaskTablePF, gao);
    if ((gao->flags & 2) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.animatedObjectsTablePF, gao);
    if ((gao->flags & 4) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.dynamicObjectsTablePF, gao);
    if ((gao->flags & 0x20) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.AICallbacksTablePF, gao);
    if ((gao->flags & 0x100) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.collMapObjectsTablePF, gao);
    if ((gao->flags & 0x200) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.collInstanceZDMGameObjectsPF, gao);
    if ((gao->flags & 0x400) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.collInstanceZDEGameObjectsPF, gao);
    if ((gao->flags & 0x2000000) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.eventGameObjectsTablePF, gao);
    if ((gao->flags & 0x400000) != 0)
        TAB_PFtable_RemoveElemWithPointer(&world->eot.objectsWithHiearchyTablePF, gao);
}

void WOR_ResetKillGO()
{
    WOR_gi_NumKilledGO = 0;
}

void WOR_DelObj(struct sWorld* world, struct sGameObjectMain* gao)
{
    int i;

    for (i = 0; i < world->deletedObjNum; i++) {
        if (world->deletedObjs[i].gao == gao)
        {
            world->deletedObjs[i].gao = NULL;
            world->deletedObjs[i].flash[0] = 0;
        }
    }

    // preserved: the loop has no break, so i is deletedObjNum here and this
    // test can never hold -- the list never shrinks.
    if (i == world->deletedObjNum - 1)
        world->deletedObjNum = world->deletedObjNum - 1;
}

void WOR_AddDelObj(struct sWorld* world, struct sGameObjectMain* gao, int* data)
{
    int i;

    for (i = 0; i < world->deletedObjNum; i++) {
        if (!world->deletedObjs[i].gao)
        {
            break;
        }
    }

    if (i == world->deletedObjNum)
        world->deletedObjNum++;
    world->deletedObjs[i].gao = gao;
    world->deletedObjs[i].flash = data;
}

void WOR_MakeObjectVisibleIfFlagsAllow(tGameObjectMain* gao, tWorld* world)
{
    if ((gao->flags2 & 2) == 0)
    {
        if ((gao->flags2 & 0x200000) != 0)
        {
            gao->distance = 0;
            gao->flags2 |= 0x4;
        }
        else
        {
            TAB_AddElemToTablePF(&world->visibleGameObjectsPF, gao, gao->flags);
            gao->flags2 |= 0x2;
            if ((gao->flags & 0x10) != 0)
                TAB_PFTable_AddElemToTable(&world->lightObjectsTablePF, gao, gao->flags);
            if ((gao->flags & 0x40000000) != 0)
                TAB_PFTable_AddElemToTable(&world->objectsWithBVMaskTablePF, gao, gao->flags);
        }
    }
}

void WOR_World_ActivateObjects(struct sWorld* world)
{
    tableElemStructPF* currElem;
    tGameObjectMain* currGao;

    TAB_PFtable_Clear(&world->activatedObjectsTable4PF);
    for (currElem = world->objectTablePF.firstElemPtr; currElem < (tableElemStructPF*)world->objectTablePF.nextElemPtr; currElem++)
    {
        currGao = (tGameObjectMain*)currElem->objectPtr;
        if (!TAB_TableElemIsInvalid(currGao))
        {
            if ((currGao->flags2 & 1) != 0)
                currGao->flags2 &= ~1;
            if ((currGao->flags & 1) == 0)
                WOR_ActivateObjectIfFlagsAllow(currGao, world);
        }
    }
}

void WOR_World_AddLoadedObject(tWorld* world, tGameObjectMain* gao, int flags)
{
    if (TAB_ul_PFtable_GetElemIndexWithPointer(&world->objectTablePF, gao) == -1)
        TAB_PFTable_AddElemToTable(&world->objectTablePF, gao, flags);
    if ((gao->flags & 1) == 0)
    {
        WOR_ActivateObjectIfFlagsAllow(gao, world);
        WOR_MakeObjectVisibleIfFlagsAllow(gao, world);
    }
}

tWorld* WOR_pst_World_Create()
{
    tWorld* ret;

    if (WOR_gb_RealAllocWorld)
        ret = (tWorld*)MEM_p_Alloc(sizeof(tWorld));
    else
        ret = (tWorld*)MEM_p_AllocTmp(sizeof(tWorld));
    memset(ret, 0, sizeof(tWorld));
    return ret;
}

void EOT_SetOfEOT_Init(tEOTStructs* eot)
{
    TAB_PFtable_Init(&eot->animatedObjectsTablePF,       500, 1.0f);
    TAB_PFtable_Init(&eot->dynamicObjectsTablePF,        500, 1.0f);
    TAB_PFtable_Init(&eot->AICallbacksTablePF,           500, 1.0f);
    TAB_PFtable_Init(&eot->collMapObjectsTablePF,        500, 1.0f);
    TAB_PFtable_Init(&eot->collInstanceZDMGameObjectsPF, 500, 1.0f);
    TAB_PFtable_Init(&eot->collInstanceZDEGameObjectsPF, 500, 1.0f);
    TAB_PFtable_Init(&eot->eventGameObjectsTablePF,      500, 1.0f);
    TAB_PFtable_Init(&eot->objectsWithHiearchyTablePF,   500, 1.0f);
}

void WOR_World_Init(tWorld* world, unsigned int elemNum)
{
    world->gaoTableMaxElemNum = elemNum;
    TAB_PFtable_Init(&world->objectTablePF,          elemNum, 1.0f);
    TAB_Ptable_Init(&world->worldGROTable,                 0, 0.1f);
    TAB_Ptable_Init(&world->worldMaterialTable,            0, 0.1f);
    TAB_PFtable_Init(&world->activatedObjectsTable4PF,  1500, 1.0f);
    TAB_PFtable_Init(&world->visibleGameObjectsPF,      2000, 1.0f);
    TAB_PFtable_Init(&world->lightObjectsTablePF,        100, 1.0f);
    TAB_PFtable_Init(&world->objectsWithBVMaskTablePF,   100, 1.0f);
    world->viewNum = 0;
    world->viewPtr = NULL;
    WOR_View_Create(world);
    EOT_SetOfEOT_Init(&world->eot);
    world->tablesManagerAndEOTNeeded = 1;
    world->visibilityCallNeeded = 1;
    world->SnPRefreshNeeded = TRUE;
    world->wayPtr = NULL;
    if (WOR_gb_FinalWorld)
        INT_SnP_Alloc(world);
}

void WOR_Universe_Init()
{
    memset(&WOR_gst_Universe, 0, sizeof(WOR_gst_Universe));
    TAB_PFtable_Init(&WOR_gst_Universe.univWorldTablePF, 4, 1.0f);
}

void WOR_InitModule()
{
    WOR_Universe_Init();
}

int WOR_World_Close(tWorld* world)
{
    tableElemStructPF* currElem;
    tGameObjectMain* currGao;
    int vn;

    if (world)
    {
        GFX_Free(&world->gfxCurrentList, 0);
        GFX_Free(&world->gfxCurrentListSortedAfterAE, 0);
        for (currElem = world->objectTablePF.firstElemPtr; currElem < (tableElemStructPF*)world->objectTablePF.nextElemPtr; currElem++)
        {
            currGao = (tGameObjectMain*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(currGao))
            {
                AI_EvalFunc_WORPreLoadTex_C();
                if (((uint8_t)currGao & 1) == 0 && (currGao->flags & 1) == 0) // valid ptr and it doesn't have flag 1
                {
                    WOR_World_DetachObject(world, currGao);
                    OBJ_GameObject_Remove(currGao, TRUE);
                }
            }
        }
        GRO_Struct_DestroyTable(&world->worldGROTable);
        GRO_Struct_DestroyTable(&world->worldMaterialTable);

        vn = world->viewNum;
        if (vn)
        {
            while (vn-- > 0) {
                WOR_View_Close(&world->viewPtr[vn]);
            }
        }
        if (world->viewPtr)
            MEM_Free(world->viewPtr);

        TAB_PFtable_Close(&world->objectTablePF);
        TAB_PFtable_Close(&world->activatedObjectsTable4PF);
        TAB_PFtable_Close(&world->visibleGameObjectsPF);
        TAB_PFtable_Close(&world->lightObjectsTablePF);
        TAB_PFtable_Close(&world->objectsWithBVMaskTablePF);
        EOT_SetOfEOT_Close(&world->eot);
        if (world->wayPtr)
        {
            WAY_RemoveAllNetworks(world->wayPtr);
            world->wayPtr = NULL;
        }
        if (world->grid1Ptr)
            GRI_Destroy(world->grid1Ptr);
        if (world->grid2Ptr)
            GRI_Destroy(world->grid2Ptr);
        INT_SnP_DesAlloc(world);
        if (MAI_gst_MainHandles.currentWorld == world)
            MAI_gst_MainHandles.currentWorld = NULL;
    }
    return 0;
}

int WOR_World_Destroy(tWorld* world)
{
    if (world)
    {
        STR_3DStringList_Clear();
        WTR_BeginLoadWorldHook();
        if (WOR_World_Close(world))
            return 1;
        LOA_DeleteAddress((int)world);
        MEM_Free(world);
        if (MAI_gst_MainHandles.currentWorld == world)
        {
            MAI_gst_MainHandles.currentWorld = NULL;
            if (MAI_gst_MainHandles.displayData)
                MAI_gst_MainHandles.displayData->CurrentWorld = NULL;
        }
    }
    return 0;
}

void WOR_Universe_DeleteWorld(struct sWorld* world)
{
    if (world
        && TAB_ul_PFtable_GetElemIndexWithPointer(&WOR_gst_Universe.univWorldTablePF, world) != -1
        && !WOR_World_Destroy(world))
    {
        TAB_PFtable_RemoveElemWithPointer(&WOR_gst_Universe.univWorldTablePF, world);
    }
}

int WOR_l_Universe_GetWorldIndex(struct sWorld* world)
{
    return TAB_ul_PFtable_GetElemIndexWithPointer(&WOR_gst_Universe.univWorldTablePF, world);
}

struct sWorld* WOR_pst_Universe_GetActiveWorld(int idx)
{
    tableElemStructPF* currElem;
    tableElemStructPF* lastElem = ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1;
    int currIdx = idx;

    for (currElem = &((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[idx]; currElem <= lastElem; currElem++, currIdx++) {
        if (!TAB_TableElemIsInvalid(currElem->objectPtr) && (currElem->objectFlags & 1) != 0)
            return (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[currIdx].objectPtr;
    }

    return NULL;
}

int WOR_i_GetAllChildrenOf(struct sWorld* world, struct sGameObjectMain* parent, struct sGameObjectMain** children)
{
    int idx;
    tableElemStructPF* currElem;
    tGameObjectMain* currGao;

    idx = 0;
    if (parent)
    {
        for (currElem = world->objectTablePF.firstElemPtr; currElem < (tableElemStructPF*)world->objectTablePF.nextElemPtr; currElem++)
        {
            currGao = (tGameObjectMain*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(currGao))
            {
                if ((currGao->flags2 & 0x1) != 0 && (currGao->flags2 & 0x400000) == 0 && 
                    (currGao->flags & 0x400000) != 0 && currGao->basic->mtx->refGao2 == parent)
                {
                    if (children)
                        children[idx++] = currGao;
                }
            }
        }
    }
    return idx;
}

tGameObjectMain* WOR_GetLastChildObj(tGameObjectMain* gao)
{
    tWorld* world;
    tGameObjectBasic* basic;
    tGaoMatrix* mtx;
    tGameObjectMain* parent;
    tGameObjectMain* child;
    tGameObjectMain* result;
    int depth;
    int step;
    // preserved: one slot for a list WOR_i_GetAllChildrenOf writes without a bound,
    // so a parent with more than one child writes past it.
    tGameObjectMain* onlyChild;

    if (!gao)
        return 0;
    world = WOR_gpst_CurrentWorld;
    if (!world)
        return 0;
    basic = gao->basic;
    if (!basic)
        return 0;
    mtx = basic->mtx;
    if (!mtx)
        return 0;
    parent = mtx->refGao2;
    if (!parent)
        return 0;
    depth = 0;
    child = gao;
    if (WOR_i_GetAllChildrenOf(world, parent, &onlyChild) == 1)
    {
        do
        {
            child = parent;
            basic = parent->basic;
            if (!basic)
                return 0;
            mtx = basic->mtx;
            if (!mtx)
                return 0;
            parent = mtx->refGao2;
            if (!parent)
                return 0;
            ++depth;
        } while (WOR_i_GetAllChildrenOf(world, parent, &onlyChild) == 1);
    }
    result = onlyChild;
    // preserved: child cannot be NULL here, so the second arm always returns and the
    // first leaves result NULL -- every path out of this function yields NULL.
    if (onlyChild == child)
        result = NULL;
    else if (NULL != child)
        return NULL;
    step = 0;
    if (depth > 0)
    {
        while (WOR_i_GetAllChildrenOf(world, result, &onlyChild) == 1)
        {
            result = onlyChild;
            if (++step >= depth)
                return result;
        }
        return NULL;
    }
    return result;
}

void EOT_RemoveObjectFromTableIfBadFlag(tableDescStruct* table, int flagMask)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;

    elem = table->firstElemPtr;
    for (last = ((tableElemStructPF*)(table->nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr && ((uint8_t)gao & 3) != 1 && (flagMask & elem->objectFlags) != (flagMask & gao->flags))
            TAB_PFtable_RemoveElem(table, elem);
    }
}

void EOT_UpdateIdentityFlagsOfTable(tableDescStruct* table)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;

    elem = table->firstElemPtr;
    for (last = ((tableElemStructPF*)(table->nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr)
        {
            if (((uint8_t)gao & 3) != 1)
                elem->objectFlags = gao->flags;
        }
    }
}

void EOT_SetOfEOT_Close(tEOTStructs* eot)
{
    TAB_PFtable_Close(&eot->animatedObjectsTablePF);
    TAB_PFtable_Close(&eot->dynamicObjectsTablePF);
    TAB_PFtable_Close(&eot->AICallbacksTablePF);
    TAB_PFtable_Close(&eot->collMapObjectsTablePF);
    TAB_PFtable_Close(&eot->collInstanceZDMGameObjectsPF);
    TAB_PFtable_Close(&eot->collInstanceZDEGameObjectsPF);
    TAB_PFtable_Close(&eot->eventGameObjectsTablePF);
    TAB_PFtable_Close(&eot->objectsWithHiearchyTablePF);
}

void EOT_SetOfEOT_Clear(tEOTStructs* eot)
{
    TAB_PFtable_Clear(&eot->animatedObjectsTablePF);
    TAB_PFtable_Clear(&eot->dynamicObjectsTablePF);
    TAB_PFtable_Clear(&eot->AICallbacksTablePF);
    TAB_PFtable_Clear(&eot->collMapObjectsTablePF);
    TAB_PFtable_Clear(&eot->collInstanceZDMGameObjectsPF);
    TAB_PFtable_Clear(&eot->collInstanceZDEGameObjectsPF);
    TAB_PFtable_Clear(&eot->eventGameObjectsTablePF);
    TAB_PFtable_Clear(&eot->objectsWithHiearchyTablePF);
}

void EOT_AISortWithPriority(tableDescStruct* table)
{
    tableElemStructPF* base;
    tableElemStructPF* last;
    tableElemStructPF* cursor;
    tableElemStructPF* elem;
    tableElemStructPF* slot;
    tGameObjectMain* gao;
    tGameObjectMain* held;
    tGameObjectMain* displaced;
    int heldFlags;
    int displacedFlags;
    int prio;
    int holeFound;
    int i;
    int start[256];
    int count[256];

    do
    {
        holeFound = 0;
        memset(count, 0, sizeof(count));
        base = table->firstElemPtr;
        last = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
        for (elem = base; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (!elem->objectPtr || ((uint8_t)gao & 3) == 1)
            {
                TAB_PFtable_RemoveHoles(table);
                holeFound = 1;
                break;
            }
            ++count[gao->extended->AIPriority];
        }
    } while (holeFound);

    start[0] = 0;
    for (i = 0; i < 255; ++i)
        start[i + 1] = start[i] + count[i];

    held = (tGameObjectMain*)-1;
    heldFlags = 0;
    cursor = base;
    while (cursor <= last)
    {
        if ((cursor->objectFlags & 0x40000000) == 0)
        {
            prio = ((tGameObjectMain*)(cursor->objectPtr))->extended->AIPriority;
            slot = &base[start[prio]];
            if (slot != cursor && slot <= last)
            {
                if (held == (tGameObjectMain*)-1)
                {
                    held = (tGameObjectMain*)slot->objectPtr;
                    heldFlags = slot->objectFlags;
                    slot->objectPtr = cursor->objectPtr;
                    slot->objectFlags = cursor->objectFlags | 0x40000000;
                    cursor->objectPtr = (void*)-1;
                    ++start[prio];
                }
                while (held != (tGameObjectMain*)-1)
                {
                    slot = &base[start[held->extended->AIPriority]];
                    if (slot > last)
                        break;
                    displaced = (tGameObjectMain*)slot->objectPtr;
                    displacedFlags = slot->objectFlags;
                    slot->objectPtr = held;
                    slot->objectFlags = heldFlags | 0x40000000;
                    if (slot == cursor)
                        ++cursor;
                    ++start[held->extended->AIPriority];
                    heldFlags = displacedFlags;
                    held = displaced;
                }
                continue;
            }
            ++start[prio];
        }
        ++cursor;
    }
}

void EOT_SetOfEOT_Build(tEOTStructs* eot, tableDescStruct* src)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;

    if (!PRO_EOTCreation.insideCounter)
        PRO_EOTCreation.startTick = TIM_GetTicksAsInt32();
    ++PRO_EOTCreation.insideCounter;
    EOT_SetOfEOT_Clear(eot);
    elem = src->firstElemPtr;
    for (last = ((tableElemStructPF*)(src->nextElemPtr)) - 1; elem <= last; ++elem)
    {
        if ((elem->objectFlags & 2) != 0)
            TAB_AddElemToTablePF(&eot->animatedObjectsTablePF, elem->objectPtr, elem->objectFlags);
        if ((elem->objectFlags & 4) != 0)
        {
            OBJ_ResetProcess((tGameObjectMain*)elem->objectPtr, WOR_gpst_CurrentWorld->dynCallCount, 0x40);
            TAB_AddElemToTablePF(&eot->dynamicObjectsTablePF, elem->objectPtr, elem->objectFlags);
        }
        if ((elem->objectFlags & 0x20) != 0)
        {
            OBJ_ResetProcess((tGameObjectMain*)elem->objectPtr, WOR_gpst_CurrentWorld->aiCallCount, 0x20);
            TAB_AddElemToTablePF(&eot->AICallbacksTablePF, elem->objectPtr, elem->objectFlags);
        }
        if ((elem->objectFlags & 0x100) != 0)
            TAB_AddElemToTablePF(&eot->collMapObjectsTablePF, elem->objectPtr, elem->objectFlags);
        if ((elem->objectFlags & 0x200) != 0)
            TAB_AddElemToTablePF(&eot->collInstanceZDMGameObjectsPF, elem->objectPtr, elem->objectFlags);
        if ((elem->objectFlags & 0x400) != 0)
            TAB_AddElemToTablePF(&eot->collInstanceZDEGameObjectsPF, elem->objectPtr, elem->objectFlags);
        if ((elem->objectFlags & 0x2000000) != 0)
            TAB_AddElemToTablePF(&eot->eventGameObjectsTablePF, elem->objectPtr, elem->objectFlags);
        if ((elem->objectFlags & 0x400000) != 0)
        {
            OBJ_ResetProcess((tGameObjectMain*)elem->objectPtr, WOR_gpst_CurrentWorld->hierarchyCallCount, 0x80);
            TAB_AddElemToTablePF(&eot->objectsWithHiearchyTablePF, elem->objectPtr, elem->objectFlags);
        }
    }

    EOT_AISortWithPriority(&eot->AICallbacksTablePF);

    if (PRO_EOTCreation.insideCounter)
    {
        if (!--PRO_EOTCreation.insideCounter)
            PRO_EOTCreation.sumTime += TIM_GetTicksAsInt32() - PRO_EOTCreation.startTick;
    }
}

void EOT_SetOfEOT_OneBuild(tableDescStruct* dest, int flagMask, tableDescStruct* src)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    int* gaoFlagsPtr;
    int gaoFlags;

    if (!PRO_EOTCreation.insideCounter)
        PRO_EOTCreation.startTick = TIM_GetTicksAsInt32();
    ++PRO_EOTCreation.insideCounter;
    TAB_PFtable_Clear(dest);
    elem = src->firstElemPtr;
    for (last = ((tableElemStructPF*)(src->nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gaoFlagsPtr = (int*)elem->objectPtr;
        if (elem->objectPtr)
        {
            if (((uint8_t)gaoFlagsPtr & 3) != 1)
            {
                gaoFlags = *gaoFlagsPtr;
                elem->objectFlags = gaoFlags;
                if ((gaoFlags & flagMask) != 0)
                    TAB_AddElemToTablePF(dest, gaoFlagsPtr, gaoFlags);
            }
        }
    }
    if (PRO_EOTCreation.insideCounter)
    {
        if (!--PRO_EOTCreation.insideCounter)
            PRO_EOTCreation.sumTime += TIM_GetTicksAsInt32() - PRO_EOTCreation.startTick;
    }
}

void WOR_World_ResolveAIRef(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGameObjectDesignFields* fields;
    unsigned int pos;

    elem = world->objectTablePF.firstElemPtr;
    for (last = ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr)
        {
            if (((uint8_t)gao & 3) != 1)
            {
                OBJ_ResolveAIRef(gao);
                if ((gao->flags & 0x40) != 0)
                {
                    fields = gao->extended->fields;
                    if (fields->designNet1)
                    {
                        pos = BIG_ul_SearchKeyToPos(fields->designNet1);
                        if (pos != -1)
                            fields->designNet1 = LOA_ul_SearchAddress(pos);
                    }
                    if (fields->designNet2)
                    {
                        pos = BIG_ul_SearchKeyToPos(fields->designNet2);
                        if (pos != -1)
                            fields->designNet2 = LOA_ul_SearchAddress(pos);
                    }
                    if (fields->designGO1)
                    {
                        pos = BIG_ul_SearchKeyToPos(fields->designGO1);
                        if (pos != -1)
                            fields->designGO1 = LOA_ul_SearchAddress(pos);
                    }
                    if (fields->designGO2)
                    {
                        pos = BIG_ul_SearchKeyToPos(fields->designGO2);
                        if (pos != -1)
                            fields->designGO2 = LOA_ul_SearchAddress(pos);
                    }
                }
            }
        }
    }
}

void WOR_World_AttachGameObject(struct sWorld* world, struct sGameObjectMain* gao)
{
    tGameObjectExtended* ext;
    tGroup* grp;
    tLIGHT* light;
    tGameObjectMain* parent;
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    tGEOGRO* gro;
    int* rliColors;
    int vertexNum;
    int* newColors;
    tGEOMAT* mat;
    tableDescStruct* matTable;
    int subMatNum;
    tGEOMAT** subMat;
    tGEOMAT** subMatEnd;
    tGEOMAT* sub;
    int subId;
    tModifier* mdf;
    tXmenMdfStruct* xmen;
    tGEOMAT* xmenMat;
    //tableElemStructPF* v25;
    tGameObjectActorObject* actor;
    tAnim* anim;

    WOR_World_AddLoadedObject(world, gao, 0);
    if ((gao->flags & 0x800000) != 0)
    {
        ext = gao->extended;
        if (ext && (grp = ext->grp) != 0)
            ++grp->groupElemNum;
        else
            OBJ_ChangeIdentityFlags(gao, gao->flags & ~0x800000u, -1);
    }
    if ((gao->flags & 0x10) != 0)
    {
        light = (tLIGHT*)(gao->extended->light);
        if (light)
        {
            light->intf->AddRef((tGroGeneric*)light, 1);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(&world->worldGROTable, light) == -1)
            {
                TAB_PTable_AddElemToTable(&world->worldGROTable, light);
                light->intf->AddRef((tGroGeneric*)light, 1);
            }
        }
    }
    if ((gao->flags & 0x400000) != 0)
    {
        parent = gao->basic->mtx->refGao2;
        if (parent)
            parent->flags2 |= 0x40u;
    }
    if ((gao->flags & 0x4000) != 0)
    {
        basic = gao->basic;
        visual = basic->actorObjectOrVisualObject;         // vigyázz mire van állítva, itt Visual kell
        gro = (tGEOGRO*)(((tGameObjectVisualObject*)(basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
        if (gro)
        {
            gro->intf->AddRef((tGroGeneric*)gro, 1);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(&world->worldGROTable, gro) == -1)
            {
                TAB_PTable_AddElemToTable(&world->worldGROTable, gro);
                gro->intf->AddRef((tGroGeneric*)gro, 1);
            }
            rliColors = visual->RLIVertexColors;
            if (rliColors)
            {
                if (gro->intf->id == 1)
                {
                    vertexNum = gro->vertexNum;
                    if (*rliColors != vertexNum)
                    {
                        newColors = (int*)MEM_p_Realloc(rliColors, 4 * vertexNum + 4);
                        visual->RLIVertexColors = newColors;
                        *newColors = gro->vertexNum;
                    }
                }
            }
        }
        else
        {
            if (visual->RLIVertexColors)
                MEM_Free(visual->RLIVertexColors);
            visual->RLIVertexColors = 0;
        }
        mat = (tGEOMAT*)(visual->GROMAT);
        if (mat)
        {
            matTable = &world->worldMaterialTable;
            if (!mat->usageCount && mat->intf->id == 4)
            {
                subMatNum = mat->multiMaterialNumIfType4ThenAmbiantColor;
                if (subMatNum)
                {
                    subMat = mat->multiMaterialPtrIfType4ThenDiffuseColor;
                    for (subMatEnd = &subMat[subMatNum]; subMat < subMatEnd; ++subMat)
                    {
                        sub = *subMat;
                        if (*subMat)
                        {
                            subId = sub->intf->id;
                            if (subId == 3 || subId == 5)
                            {
                                sub->intf->AddRef((tGroGeneric*)sub, 1);
                                if (TAB_ul_Ptable_GetElemIndexWithPointer(matTable, *subMat) == -1)
                                {
                                    TAB_PTable_AddElemToTable(matTable, *subMat);
                                    (*subMat)->intf->AddRef((tGroGeneric*)*subMat, 1);
                                    WOR_gb_HasAddedMaterial = TRUE;
                                }
                            }
                            else
                            {
                                *subMat = 0;
                            }
                        }
                    }
                }
            }
            mat->intf->AddRef((tGroGeneric*)mat, 1);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(matTable, mat) == -1)
            {
                TAB_PTable_AddElemToTable(matTable, mat);
                mat->intf->AddRef((tGroGeneric*)mat, 1);
                WOR_gb_HasAddedMaterial = TRUE;
            }
        }
    }
    ext = gao->extended;
    if (ext)
    {
        for (mdf = ext->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId == 9)        // xmen
            {
                xmen = (tXmenMdfStruct*)(mdf->mdfPrivateDataPtr);
                xmenMat = xmen->geomat;
                if (xmenMat)
                {
                    if (TAB_ul_Ptable_GetElemIndexWithPointer(&world->worldMaterialTable, xmen->geomat) == -1)
                    {
                        /* 
                        // AddElemP inline
                        if (a1->worldMaterialTable.nextElemPtr > a1->worldMaterialTable.lastElemPtr)
                            TAB_Ptable_Resize(&a1->worldMaterialTable, 2 * (a1->worldMaterialTable.holeNum + a1->worldMaterialTable.actElemNum));
                        ((tableElemStructP*)(a1->worldMaterialTable.nextElemPtr))->objectPtr = v24;
                        v25 = a1->worldMaterialTable.nextElemPtr;
                        ++a1->worldMaterialTable.actElemNum;
                        a1->worldMaterialTable.nextElemPtr = v25 + 1;
                        */
                        // Crash fix, a típus rossz volt (v25 (tableElemStructPF) ezért átlépkedett elemeket és az inicializálatlan memória maradt egy-két helyen,
                        // amin a MAT_pc_GetAllUsedTextureIndex-bõl hívott MAT_TestUsedForMat elszállt)
                        TAB_PTable_AddElemToTable(&world->worldMaterialTable, xmenMat);
                        xmenMat->intf->AddRef((tGroGeneric*)xmenMat, 1);
                        WOR_gb_HasAddedMaterial = TRUE;
                    }
                }
            }
        }
    }
    if ((gao->flags & 2) != 0)
    {
        actor = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
        if (actor->actionKit)
        {
            ACT_SetUsageCounts(actor->actionKit);
            EVE_gb_PlayOnlyAnimTracks = 1;
            ACT_SetAction(gao, 0, 0);
            EVE_gb_PlayOnlyAnimTracks = 0;
        }
        else
        {
            anim = actor->animPtr[0];
            if (anim && anim != (tAnim*)-1)
            {
                ++anim->ptr->eventListStruct->usageCount;
                actor->animPtr[0]->frequency = 60;
            }
            if (actor->flagsEgBlendIw2)
            {
                EVE_gb_PlayOnlyAnimTracks = 1;
                ANI_PlayGameObjectAnimation(gao, actor, 0.0f, 0);
                EVE_gb_PlayOnlyAnimTracks = 0;
            }
        }
    }
}

void WOR_World_JustAfterLoadObject(struct sWorld* world, struct sGameObjectMain* gao, int resolveAI, int attach)
{
    tGameObjectActorObject* actor;
    tAnim* anim;

    OBJ_Reinit(gao);
    OBJ_Gizmo_UpdateGizmoPtr(gao);
    if ((gao->flags & 0x2000) != 0 && gao->extended->mdf)
        SND_ResolveRefsForMdF(gao->extended->mdf);
    if ((gao->flags & 0x8000000) != 0)
        SND_ResolveRefs(gao);
    if ((gao->flags & 0x700) != 0)
        COL_ResolveRefs(gao);
    if ((gao->flags & 2) != 0)
    {
        ANI_ResolveRefs(gao);
        if ((gao->flags & 2) != 0)
        {
            actor = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
            if (!actor->actionKit)
            {
                anim = actor->animPtr[0];
                if (anim)
                    EVE_ResolveListTrackRef(anim->ptr->eventListStruct);
            }
        }
    }
    if (attach)
        WOR_World_AttachGameObject(world, gao);
    if (resolveAI)
        OBJ_ResolveAIRef(gao);
}

void WOR_World_CheckFather(tGameObjectMain* gao, tWorld* world)
{
    tGameObjectBasic* basic;
    tGaoMatrix* mtx;
    tGameObjectMain* parent;

    if ((gao->flags & 0x400000) != 0)
    {
        basic = gao->basic;
        if (basic && (mtx = basic->mtx) != 0 && (parent = mtx->refGao2) != 0)
        {
            if (TAB_ul_PFtable_GetElemIndexWithPointer(&world->objectTablePF, parent) == -1)
            {
                parent = gao->basic->mtx->refGao2;
                WOR_World_DetachObject(world, parent);
                OBJ_GameObject_Remove(parent, FALSE);
            }
        }
        else
        {
            gao->flags &= ~0x400000u;
            mtx = basic->mtx;
            if (mtx)
                MEM_Free(mtx);
            gao->basic->mtx = 0;
        }
    }
}

void WOR_World_CheckGroup(tGameObjectMain* gao, tWorld* world)
{
    tGameObjectExtended* ext;
    tGroup* grp;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* member;
    tableDescStruct* grpTable;

    if ((gao->flags & 0x800000) != 0)
    {
        ext = gao->extended;
        if (ext)
        {
            grp = ext->grp;
            if (grp)
            {
                elem = grp->tablePF->firstElemPtr;
                last = ((tableElemStructPF*)(grp->tablePF->nextElemPtr)) - 1;
                for (; elem <= last; ++elem)
                {
                    member = (tGameObjectMain*)elem->objectPtr;
                    if (elem->objectPtr && ((uint8_t)member & 3) != 1)
                    {
                        if (TAB_ul_PFtable_GetElemIndexWithPointer(&world->objectTablePF, elem->objectPtr) == -1)
                        {
                            WOR_World_DetachObject(world, member);
                            OBJ_GameObject_Remove(member, TRUE);
                            grpTable = grp->tablePF;
                            if (elem->objectPtr)
                            {
                                if (((int)elem->objectPtr & 3) != 1)
                                {
                                    elem->objectPtr = (void*)1;
                                    --grpTable->actElemNum;
                                    ++grpTable->holeNum;
                                }
                            }
                            elem->objectFlags = 0;
                        }
                    }
                }
            }
        }
    }
}

void * WOR_ul_GameObjectGroupCallback(uint32_t Offset)
{
    char* data;
    char* cursor;
    unsigned int size;
    int key;

    data = BIG_pc_ReadFileTmp(Offset, (int*)&size);
    cursor = data;
    TAB_PFtable_Resize(&WOR_gpst_WorldToLoadIn->objectTablePF, size);
    if (data < &data[size])
    {
        do
        {
            key = LOA_ReadLong(&cursor, 0, 1);
            LOA_ReadLong(&cursor, 0, 2);
            if (key)
            {
                TAB_AddElemToTablePF(&WOR_gpst_WorldToLoadIn->objectTablePF, NULL, 0);// pftable inline sztem
                LOA_MakeFileRef(key, &(((tableElemStructPF*)(WOR_gpst_WorldToLoadIn->objectTablePF.nextElemPtr))[-1].objectPtr), OBJ_ul_GameObjectCallback, 1);
            }
        } while (cursor < &data[size]);
    }
    return NULL;
}

tWorld* WOR_ul_World_LoadCallback(uint32_t Offset)
{
    tWorld* world;
    char* cursor;
    char magic[4];
    int size;
    int version;
    int elemNum;
    int key;

    world = WOR_pst_World_Create();
    world->worldKey = LOA_ul_GetCurrentKey();
    cursor = BIG_pc_ReadFileTmp(Offset, &size);
    LOA_ReadCharArray(&cursor, magic, 4u, 1);
    if (strncmp(magic, ".wow", 4u))
        return 0;
    version = LOA_ReadLong(&cursor, 0, 1);
    if (version != 3)
        return 0;
    world->deletedObjNum = 0;
    elemNum = LOA_ReadLong(&cursor, 0, 1);
    world->gaoTableMaxElemNum = elemNum;
    WOR_World_Init(world, elemNum);
    world->ambientColor1 = LOA_ReadLong(&cursor, 0, 1);
    LOA_ReadString(&cursor, world->worldName, 60, 1);
    DBG_OutputDebugString("Loading world: %s\n", world->worldName);
    LOA_ReadLong(&cursor, 0, 2);
    LOA_ReadMatrix(&cursor, &world->unkMatrix, 1);
    if (!MATH_b_TypeIsCorrect(&world->unkMatrix))
    {
        MATH_SetIdentityMatrix(&world->unkMatrix);
        world->unkMatrix.Pos.x = 0.0;
        world->unkMatrix.Pos.y = 0.0;
        world->unkMatrix.Pos.z = 0.0;
    }
    WOR_gpst_WorldToLoadIn = world;
    world->viewPtr->focale = LOA_ReadFloat(&cursor, 0, 1);
    world->clearColor = LOA_ReadLong(&cursor, 0, 1);
    world->ambientColor2 = LOA_ReadLong(&cursor, 0, 1);
    if (world->ambientColor2 == -1)
        world->ambientColor2 = world->ambientColor1;
    world->distanceToHide = LOA_ReadLong(&cursor, 0, 1);
    if (world->distanceToHide < 0 || world->distanceToHide > 255)
        world->distanceToHide = 0;
    LOA_ReadCharArray(&cursor, 0, 44u, 2);
    key = LOA_ReadLong(&cursor, 0, 1);
    if (key && key != -1)
        LOA_MakeFileRef(key, &world->grid2Ptr, GRID_ul_CallbackWorldGrid, 1);
    key = LOA_ReadLong(&cursor, 0, 1);
    if (key && key != -1)
        LOA_MakeFileRef(key, &world->grid1Ptr, GRID_ul_CallbackWorldGrid, 1);
    key = LOA_ReadLong(&cursor, 0, 1);
    LOA_MakeFileRef(key, 0, WOR_ul_GameObjectGroupCallback, 5);
    key = LOA_ReadLong(&cursor, 0, 1);
    if (key && key != -1)
        LOA_MakeFileRef(key, &world->wayPtr, WAY_ul_AllNetWorksCallback, 1);
    TEXT_FreeAll();
    WOR_gul_WorldText = -1;
    key = LOA_ReadLong(&cursor, 0, 1);
    if (key && key != -1 && BIG_ul_SearchKeyToPos(key) != -1)
        WOR_gul_WorldText = key;
    return world;
}

tWorld* WOR_pst_World_Load(tWorld* dest, int worldKey, int resolveAI)
{
    tWorld* loaded;
    tWorld* target;
    tWorld* result;
    tableDescStruct* groTable;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tWayAllNetworks* ways;
    tWayNetwork* net;
    tGEOSTATICLOD* lod;
    tGroGeneric* lodGeo;
    tableElemStructP* groCursor;
    tableElemStructP* groLast;
    unsigned int pos;
    unsigned int groBase;
    int total;
    int i;
    int oldNetworkNum;
    int src;
    int dst;
    int groCursorIdx;
    int lodIdx;
    tableElemStructP* groStart = NULL; // TODO FIXME, ez nem állítódik null-ra a játékban

    groBase = 0;
    if (worldKey == -1)
        return 0;
    pos = BIG_ul_SearchKeyToPos(worldKey);
    if (pos != -1)
    {
        result = (tWorld*)LOA_ul_SearchAddress(pos);
        if (result != (tWorld*)-1)
            return result;
    }

    loaded = NULL;
    LOA_MakeFileRef(worldKey, &loaded, WOR_ul_World_LoadCallback, 1);
    LOA_Resolve();
    if (!loaded)
        return 0;
    target = dest;
    if (!dest)
        target = loaded;
    groTable = &target->worldGROTable;
    if (loaded != target)
    {
        TAB_Ptable_RemoveHoles(&target->worldMaterialTable);
        TAB_Ptable_RemoveHoles(&target->worldGROTable);
        groBase = target->worldGROTable.actElemNum;
        elem = target->objectTablePF.firstElemPtr;
        for (last = ((tableElemStructPF*)(target->objectTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)(elem->objectPtr);
            if (elem->objectPtr && ((uint8_t)gao & 3) != 1)
                gao->process |= 0x10u;
        }
    }

    total = loaded->objectTablePF.holeNum + loaded->objectTablePF.actElemNum;
    for (i = 0; i < total; ++i)
    {
        gao = (tGameObjectMain*)(((tableElemStructPF*)(loaded->objectTablePF.firstElemPtr))[i].objectPtr);
        if (gao && ((uint8_t)gao & 3) != 1 && (gao->process & 0x10) == 0)
        {
            WOR_gpst_CurrentWorld = target;
            WOR_World_JustAfterLoadObject(target, gao, resolveAI, 1);
        }
    }

    if (loaded != target)
    {
        elem = loaded->objectTablePF.firstElemPtr;
        for (last = ((tableElemStructPF*)(loaded->objectTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (elem->objectPtr && ((uint8_t)gao & 3) != 1)
            {
                if ((gao->process & 0x10) != 0)
                    ENG_ReinitOneObject(gao, 14);
                gao->process &= ~0x10u;
                if (loaded != target && TAB_ul_PFtable_GetElemIndexWithPointer(&target->objectTablePF, gao) == -1)
                {
                    gao->process &= ~0x10u;
                    TAB_PFTable_AddElemToTable(&target->objectTablePF, gao, 0);
                }
            }
        }
    }

    elem = target->objectTablePF.firstElemPtr;
    for (last = ((tableElemStructPF*)(target->objectTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr && ((uint8_t)gao & 3) != 1)
        {
            WOR_World_CheckFather(gao, target);
            WOR_World_CheckGroup(gao, target);
        }
    }

    if (loaded == target)
    {
        groStart = target->worldGROTable.firstElemPtr;
    }
    else
    {
        memcpy(&target->unkMatrix, &loaded->unkMatrix, sizeof(target->unkMatrix));
        target->ambientColor1 = loaded->ambientColor1;
        target->ambientColor2 = loaded->ambientColor2;
        target->clearColor = loaded->clearColor;
        target->distanceToHide = loaded->distanceToHide;
        ways = target->wayPtr;
        if (ways)
        {
            for (src = 0; src < target->wayPtr->networkNum; ++src)
            {
                net = target->wayPtr->networks[src];
                if (net)
                    net->flags |= 4u;
            }
            if (loaded->wayPtr)
            {
                oldNetworkNum = target->wayPtr->networkNum;
                target->wayPtr->networkNum = oldNetworkNum + loaded->wayPtr->networkNum;
                target->wayPtr->networks = (tWayNetwork**)MEM_p_Realloc(target->wayPtr->networks, 4 * target->wayPtr->networkNum);
                dst = oldNetworkNum;
                for (src = 0; src < loaded->wayPtr->networkNum; ++src, ++dst)
                    target->wayPtr->networks[dst] = loaded->wayPtr->networks[src];
                MEM_Free(loaded->wayPtr->networks);
                loaded->wayPtr->networks = target->wayPtr->networks;
                loaded->wayPtr->networkNum = target->wayPtr->networkNum;
                LOA_DeleteAddress((int)target->wayPtr);
                MEM_Free(target->wayPtr);
                target->wayPtr = loaded->wayPtr;
                loaded->wayPtr = 0;
            }
        }
        else
        {
            target->wayPtr = loaded->wayPtr;
            loaded->wayPtr = 0;
        }
        if (target->grid1Ptr)
            GRI_Destroy(target->grid1Ptr);
        if (target->grid2Ptr)
            GRI_Destroy(target->grid2Ptr);
        target->grid1Ptr = loaded->grid1Ptr;
        target->grid2Ptr = loaded->grid2Ptr;
        loaded->grid1Ptr = 0;
        loaded->grid2Ptr = 0;
        groStart = &((tableElemStructP*)(target->worldGROTable.firstElemPtr))[groBase];
    }

    groCursor = groStart;
    // preserved: a hole in the GRO table is dereferenced here without the tag test
    // every other walk in this file makes.
    for (groLast = ((tableElemStructP*)(target->worldGROTable.nextElemPtr)) - 1; groCursor <= groLast; ++groCursor)
    {
        if (((tGroGeneric*)(groCursor->objectPtr))->intf->id == 8)      // staticlod
        {
            lod = (tGEOSTATICLOD*)(groCursor->objectPtr);
            if (lod->lodNum)
            {
                lodIdx = 0;
                do
                {
                    lodGeo = (tGroGeneric*)(lod->geo[lodIdx]); // fixed: v35 = (tGroGeneric*)&(v33->geo[v43]);
                    if (lodGeo)
                    {
                        if (lodGeo->intf->id == 1)
                        {
                            lodGeo->intf->AddRef(lodGeo, 1);
                            if (TAB_ul_Ptable_GetElemIndexWithPointer(&target->worldGROTable, lodGeo) == -1)
                            {
                                groCursorIdx = -1;
                                if ((tableElemStructP*)(target->worldGROTable.nextElemPtr) > (tableElemStructP*)(groTable->lastElemPtr))
                                    groCursorIdx = groCursor - (tableElemStructP*)(target->worldGROTable.firstElemPtr);
                                TAB_PTable_AddElemToTable(&target->worldGROTable, lodGeo);
                                lodGeo->intf->AddRef(lodGeo, 1);
                                if (groCursorIdx != -1)
                                {
                                    groCursor = &(((tableElemStructP*)(target->worldGROTable.firstElemPtr))[groCursorIdx]);
                                    groLast = ((tableElemStructP*)(target->worldGROTable.nextElemPtr)) - 1;
                                }
                            }
                        }
                        else
                        {
                            lod->geo[lodIdx] = NULL;
                        }
                    }
                    lod = (tGEOSTATICLOD*)groCursor->objectPtr;
                    ++lodIdx;
                } while (lodIdx < lod->lodNum);
            }
        }
    }

    if (target != loaded)
    {
        target->snp = loaded->snp;
        loaded->snp = 0;
        strcpy(target->worldName, loaded->worldName);
        TAB_PFtable_Close(&loaded->objectTablePF);
        WOR_World_Destroy(loaded);
        LOA_DeleteAddress((int)target);
        pos = BIG_ul_SearchKeyToPos(worldKey);
        LOA_AddPosAddress(pos, (unsigned int)target);
    }
    return target;
}

static float WOR_l_SignOf(float f) {
    int bits = (*((uint32_t*)(&f))) & 0xbf800000 | 0x3f800000;
    return *((float *)(&bits));
}

int WOR_BVMaskComputeInside(tGameObjectMain* gao)
{
    tBVMasks* mask;
    tGameObjectBV* bv;
    Mtx4x4* mtx;
    Vec3 bvMax;
    Vec3 bvMin;
    Vec3 corners[8];
    double e1x;
    double e1y;
    double e1z;
    float e2x;
    float e2y;
    double e2z;
    double pool2y;
    float nx;
    float ny;
    float nz;
    float sign;
    double dx;
    double dz;
    float dzNarrow;
    float dy;
    float dot;
    int flatInZ;
    int idx;
    int corner;
    int inside;

    if (!BVMask_gi_Num)
        return 0;
    bv = gao->boundingVolume;
    bvMax = bv->BVMax;
    bvMin = bv->BVMin;
    mtx = gao->matrices;
    corners[0].x = bvMax.x + mtx->Pos.x;
    corners[0].y = bvMax.y + mtx->Pos.y;
    corners[0].z = bvMax.z + mtx->Pos.z;
    corners[1].x = bvMin.x + mtx->Pos.x;
    corners[1].y = bvMin.y + mtx->Pos.y;
    corners[1].z = bvMin.z + mtx->Pos.z;
    corners[2].x = bvMax.x + mtx->Pos.x;
    corners[2].y = bvMin.y + mtx->Pos.y;
    corners[2].z = bvMax.z + mtx->Pos.z;
    corners[3].x = bvMin.x + mtx->Pos.x;
    corners[3].y = bvMin.y + mtx->Pos.y;
    corners[3].z = bvMax.z + mtx->Pos.z;
    corners[4].x = bvMin.x + mtx->Pos.x;
    corners[4].y = bvMax.y + mtx->Pos.y;
    corners[4].z = bvMax.z + mtx->Pos.z;
    corners[5].x = bvMax.x + mtx->Pos.x;
    corners[5].y = bvMax.y + mtx->Pos.y;
    corners[5].z = bvMin.z + mtx->Pos.z;
    corners[6].x = bvMax.x + mtx->Pos.x;
    corners[6].y = bvMin.y + mtx->Pos.y;
    corners[6].z = bvMin.z + mtx->Pos.z;
    corners[7].x = bvMin.x + mtx->Pos.x;
    corners[7].y = bvMax.y + mtx->Pos.y;
    corners[7].z = bvMin.z + mtx->Pos.z;

    for (idx = 0; idx < BVMask_gi_Num; ++idx)
    {
        mask = &BVMask_ga_Masks[idx];
        if (mask->gao == gao)
            break;
        flatInZ = mask->is14;
        sign = WOR_l_SignOf(mask->is15);             //LODWORD(v60) = LODWORD(v9) & 0xBF800000 | 0x3F800000;
        e1x = mask->pool[1].x - mask->pool[0].x;
        e1y = mask->pool[1].y - mask->pool[0].y;
        e1z = mask->pool[1].z - mask->pool[0].z;
        e2x = mask->pool[2].x - mask->pool[1].x;
        pool2y = mask->pool[2].y;
        e2y = (float)(pool2y - mask->pool[1].y);
        e2z = mask->pool[2].z - mask->pool[1].z;
        nx = (float)(e2z * e1y - e2y * e1z);
        ny = (float)(e2x * e1z - e2z * e1x);
        nz = (float)(e2y * e1x - e1y * e2x);
        for (corner = 0; corner < 8; ++corner)
        {
            dx = mask->pool[0].x - corners[corner].x;
            if (flatInZ)
            {
                dz = 0.0;
            }
            else
            {
                dx = 0.0;
                dzNarrow = mask->pool[0].z - corners[corner].z;
                dz = dzNarrow;
            }
            dy = mask->pool[0].y - corners[corner].y;
            dot = (float)(ny * dy + nz * dz + nx * dx);
            if (sign == WOR_l_SignOf(dot)) /*if (v60 == COERCE_FLOAT(LODWORD(v36) & 0xBF800000 | 0x3F800000))*/
                break;
        }
        if (corner == 8)
        {
            for (inside = 0; inside < 8; ++inside)
            {
                if (!OBJ_CullingPointInside(
                    &corners[inside],
                    &(mask->planeLeft),
                    &(mask->planeUp),
                    &(mask->planeRight),
                    &(mask->planeDown),
                    (mask->flags)))
                    break;
            }
            if (inside == 8)
                return 1;
        }
    }
    return 0;
}

static void WOR_l_InvertMaskPlanes(tBVMasks* mask)
{
    mask->planeLeft.x = -mask->planeLeft.x;
    mask->planeLeft.y = -mask->planeLeft.y;
    mask->planeLeft.z = -mask->planeLeft.z;
    mask->planeUp.x = -mask->planeUp.x;
    mask->planeUp.y = -mask->planeUp.y;
    mask->planeUp.z = -mask->planeUp.z;
    mask->planeRight.x = -mask->planeRight.x;
    mask->planeRight.y = -mask->planeRight.y;
    mask->planeRight.z = -mask->planeRight.z;
    mask->planeDown.x = -mask->planeDown.x;
    mask->planeDown.y = -mask->planeDown.y;
    mask->planeDown.z = -mask->planeDown.z;
}

int WOR_BVMaskPreCompute(tGameObjectMain* gao, tDDCamStruct* cam, int invert)
{
    tBVMasks* mask;
    tGameObjectBV* bv;
    Mtx4x4* mtx;
    Vec3* edgePlanes[4];
    Vec3* cullPlanes[4];
    Vec3* norms[4];
    Vec3* plane;
    Vec3* best;
    Vec3* edgeA;
    Vec3* edgeB;
    long double sizeX;
    long double sizeY;
    float sizeZ;
    double ax;
    double ay;
    float az;
    long double aLen;
    float aLenNarrow;
    float anx;
    double any;
    double anz;
    double bx;
    double by;
    float bz;
    long double bLen;
    float bLenNarrow;
    float bnx;
    float bny;
    double bnz;
    float cx;
    double cy;
    double cz;
    float czNarrow;
    long double cLen;
    float cLenNarrow;
    double e1x;
    double e1y;
    double e1z;
    float e2x;
    float e2y;
    double e2z;
    float nx;
    float ny;
    double nz;
    double dx;
    double dz;
    float dzNarrow;
    float dy;
    double facing;
    float bestDot;
    double dot;
    int edge;
    int i;
    int n;

    // preserved: BVMask_ga_Masks holds 200 entries and neither this function nor
    // WOR_BVMaskPreComputeAll bounds BVMask_gi_Num against that.
    mask = &BVMask_ga_Masks[BVMask_gi_Num];
    mask->gao = gao;
    mask->flags = 15;
    mask->pool[0] = gao->boundingVolume->BVMax;
    bv = gao->boundingVolume;
    mask->is11 = bv->BVMin.x;
    mask->is12 = bv->BVMin.y;
    mask->is13 = bv->BVMin.z;
    sizeX = fabs(mask->pool[0].x - mask->is11);
    sizeY = fabs(mask->pool[0].y - mask->is12);
    sizeZ = (float)fabs(mask->pool[0].z - mask->is13);
    if (sizeY >= sizeX || sizeY >= sizeZ)
    {
        if (sizeX >= sizeY || sizeX >= sizeZ)
        {
            mask->pool[1].x = mask->is11;
            mask->pool[1].y = mask->pool[0].y;
            mask->pool[1].z = mask->pool[0].z;
            mask->pool[2].x = mask->is11;
            mask->pool[2].y = mask->is12;
            mask->pool[2].z = mask->pool[0].z;
            mask->pool[3].x = mask->pool[0].x;
            mask->pool[3].y = mask->is12;
            mask->pool[3].z = mask->pool[0].z;
            mask->is14 = 0;
        }
        else
        {
            mask->pool[1].x = mask->pool[0].x;
            mask->pool[1].y = mask->is12;
            mask->pool[1].z = mask->pool[0].z;
            mask->pool[2].x = mask->pool[0].x;
            mask->pool[2].y = mask->is12;
            mask->pool[2].z = mask->is13;
            mask->pool[3].x = mask->pool[0].x;
            mask->pool[3].y = mask->pool[0].y;
            mask->pool[3].z = mask->is13;
            mask->is14 = 1;
        }
    }
    else
    {
        mask->pool[1].x = mask->is11;
        mask->pool[1].y = mask->pool[0].y;
        mask->pool[1].z = mask->pool[0].z;
        mask->pool[2].x = mask->is11;
        mask->pool[2].y = mask->pool[0].y;
        mask->pool[2].z = mask->is13;
        mask->pool[3].x = mask->pool[0].x;
        mask->pool[3].y = mask->pool[0].y;
        mask->pool[3].z = mask->is13;
        mask->is14 = 1;
    }

    mtx = gao->matrices;
    mask->pool[0].x = mtx->Pos.x + mask->pool[0].x;
    mask->pool[0].y = mtx->Pos.y + mask->pool[0].y;
    mask->pool[0].z = mtx->Pos.z + mask->pool[0].z;
    mask->pool[1].x = mtx->Pos.x + mask->pool[1].x;
    mask->pool[1].y = mtx->Pos.y + mask->pool[1].y;
    mask->pool[1].z = mtx->Pos.z + mask->pool[1].z;
    mask->pool[2].x = mtx->Pos.x + mask->pool[2].x;
    mask->pool[2].y = mtx->Pos.y + mask->pool[2].y;
    mask->pool[2].z = mtx->Pos.z + mask->pool[2].z;
    mask->pool[3].x = mtx->Pos.x + mask->pool[3].x;
    mask->pool[3].y = mtx->Pos.y + mask->pool[3].y;
    mask->pool[3].z = mtx->Pos.z + mask->pool[3].z;

    edgePlanes[0] = &mask->planeUp;
    edgePlanes[1] = &mask->planeRight;
    edgePlanes[2] = &mask->planeDown;
    edgePlanes[3] = &mask->planeLeft;
    for (edge = 0; edge < 4; ++edge)
    {
        plane = edgePlanes[edge];
        edgeB = &mask->pool[(edge + 1) % 4];
        edgeA = &mask->pool[edge];
        ax = edgeB->x - cam->objectMtx.Pos.x;
        ay = edgeB->y - cam->objectMtx.Pos.y;
        az = edgeB->z - cam->objectMtx.Pos.z;
        aLen = 1.0 / sqrt(ay * ay + ax * ax + az * az);
        aLenNarrow = (float)aLen;
        anx = (float)(aLen * ax);
        any = ay * aLenNarrow;
        anz = aLenNarrow * az;
        bx = edgeA->x - cam->objectMtx.Pos.x;
        by = edgeA->y - cam->objectMtx.Pos.y;
        bz = edgeA->z - cam->objectMtx.Pos.z;
        bLen = 1.0 / sqrt(bx * bx + bz * bz + by * by);
        bLenNarrow = (float)bLen;
        bnx = (float)(bLen * bx);
        bny = (float)(by * bLenNarrow);
        bnz = bz * bLenNarrow;
        cx = (float)(bnz * any - bny * anz);
        cy = anz * bnx - bnz * anx;
        cz = bny * anx - any * bnx;
        czNarrow = (float)cz;
        cLen = 1.0 / sqrt(cz * czNarrow + cy * cy + cx * cx);
        cLenNarrow = (float)cLen;
        plane->x = (float)(cLen * cx);
        plane->y = (float)(cLenNarrow * cy);
        plane->z = czNarrow * cLenNarrow;
    }

    e1x = mask->pool[1].x - mask->pool[0].x;
    e1y = mask->pool[1].y - mask->pool[0].y;
    e1z = mask->pool[1].z - mask->pool[0].z;
    e2x = mask->pool[2].x - mask->pool[1].x;
    e2y = mask->pool[2].y - mask->pool[1].y;
    e2z = mask->pool[2].z - mask->pool[1].z;
    nx = (float)(e2z * e1y - e2y * e1z);
    ny = (float)(e2x * e1z - e2z * e1x);
    nz = e1x * e2y - e1y * e2x;
    dx = mask->pool[0].x - cam->objectMtx.Pos.x;
    if (mask->is14)
    {
        dz = 0.0;
    }
    else
    {
        dx = 0.0;
        dzNarrow = mask->pool[0].z - cam->objectMtx.Pos.z;
        dz = dzNarrow;
    }
    dy = mask->pool[0].y - cam->objectMtx.Pos.y;
    facing = nz * dz + ny * dy + nx * dx;
    mask->is15 = (float)facing;
    if (facing < 0.0)
        WOR_l_InvertMaskPlanes(mask);
    if (invert)
        WOR_l_InvertMaskPlanes(mask);

    cullPlanes[0] = &mask->planeLeft;
    cullPlanes[1] = &mask->planeUp;
    cullPlanes[2] = &mask->planeRight;
    cullPlanes[3] = &mask->planeDown;
    norms[0] = &g_stGlobalNormPlaneLeft;
    norms[1] = &g_stGlobalNormPlaneUp;
    norms[2] = &g_stGlobalNormPlaneRight;
    norms[3] = &g_stGlobalNormPlaneDown;
    for (i = 0; i < 4; ++i)
    {
        plane = cullPlanes[i];
        best = norms[0];
        bestDot = norms[0]->y * plane->y + norms[0]->x * plane->x + norms[0]->z * plane->z;
        for (n = 1; n < 4; ++n)
        {
            dot = (float)(norms[n]->y * plane->y + norms[n]->z * plane->z + plane->x * norms[n]->x);
            if (dot < bestDot)
            {
                bestDot = (float)dot;
                best = norms[n];
            }
        }
        if ((cam->objectMtx.v3.z * best->z + cam->objectMtx.v3.y * best->y + best->x * cam->objectMtx.v3.x) <
            ((-plane->x) * cam->objectMtx.v3.x + (-plane->z) * cam->objectMtx.v3.z + (-plane->y)  * cam->objectMtx.v3.y))
            mask->flags &= ~(1 << i);
    }
    return 4;
}

void WOR_BVMaskPreComputeAll(tWorld* world, tDDContainerStruct* dd)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;

    BVMask_gi_Num = 0;
    if (ENG_gb_ActiveSectorization)
    {
        elem = world->objectsWithBVMaskTablePF.firstElemPtr;
        for (last = ((tableElemStructPF*)(world->objectsWithBVMaskTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (elem->objectPtr && ((uint8_t)gao & 3) != 1 && !OBJ_CullingObject(gao))
            {
                WOR_BVMaskPreCompute(gao, &dd->cam, 0);
                ++BVMask_gi_Num;
            }
        }
    }
}

void ENG_InitOneWorld(tWorld* world)
{
    tViewElem* view;
    tWorldGridStruct* grid;

    ENG_gb_FirstFrame = 1;
    view = world->viewPtr;
    world->recordCount = 0;
    world->colCrossObjectNumber = 0;
    if (view)
        view->assignedGao = 0;
    WAY_ComputePoundNetworks(world);
    if (world->grid1Ptr)
        GRI_Reinit(world->grid1Ptr);
    grid = world->grid2Ptr;
    if (grid)
        GRI_Reinit(grid);
    AI_gf_Epsilon = 0.000099999997f;
}

struct sWorld* WOR_pst_Universe_AddWorld(int worldKey, int unused1, int resolveAI)
{
    tWorld* world;

    world = WOR_pst_World_Load(NULL, worldKey, resolveAI);
    if (world)
    {
        ENG_InitOneWorld(world);
        if (resolveAI)
        {
            WOR_World_ActivateObjects(world);
            EOT_SetOfEOT_Build(&world->eot, &world->activatedObjectsTable4PF);
        }
        if (TAB_ul_PFtable_GetElemIndexWithPointer(&WOR_gst_Universe.univWorldTablePF, world) == -1)
            TAB_PFTable_AddElemToTable(&WOR_gst_Universe.univWorldTablePF, world, 0);
    }
    return world;
}

void WOR_World_MakeObjectsVisible(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    int flags2;

    if (!PRO_Visibility.insideCounter)
        PRO_Visibility.startTick = TIM_GetTicksAsInt32();
    ++PRO_Visibility.insideCounter;
    TAB_PFtable_Clear(&world->visibleGameObjectsPF);
    TAB_PFtable_Clear(&world->lightObjectsTablePF);
    TAB_PFtable_Clear(&world->objectsWithBVMaskTablePF);
    elem = world->objectTablePF.firstElemPtr;
    for (last = ((tableElemStructPF*)(world->objectTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr && ((uint8_t)gao & 3) != 1)
        {
            flags2 = gao->flags2;
            if ((flags2 & 2) != 0)
                gao->flags2 = flags2 & ~2u;
            if ((gao->flags & 1) == 0)
                WOR_MakeObjectVisibleIfFlagsAllow(gao, world);
        }
    }
    if (PRO_Visibility.insideCounter)
    {
        if (!--PRO_Visibility.insideCounter)
            PRO_Visibility.sumTime += TIM_GetTicksAsInt32() - PRO_Visibility.startTick;
    }
}

void WOR_SetCam(tViewElem* view)
{
    CAM_SetCameraMatrix(&gpst_DX_stDD->ct, &view->cameraMtx);
    gpst_DX_stDD->ct.cam.focale = view->focale;
    gpst_DX_stDD->ct.cam.viewPortPosX = view->viewPortPosX;
    gpst_DX_stDD->ct.cam.viewPortPosY = view->viewPortPosY;
    gpst_DX_stDD->ct.cam.viewPortSizeWidth = view->viewPortSizeWidth;
    gpst_DX_stDD->ct.cam.viewPortSizeHeight = view->viewPortSizeHeight;
    gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    CAM_Inverse(&gpst_DX_stDD->ct);
    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
}

static void WOR_l_ResetRasterStats(profilerRasterStructElem* rasters)
{
    rasters[3].unk2 = 0;
    rasters[4].unk2 = 0;
    rasters[5].unk2 = 0;
    rasters[6].unk2 = 0;
    rasters[7].unk2 = 0;
    rasters[8].unk2 = 0;
    rasters[9].unk2 = 0;
    rasters[10].unk2 = 0;
    rasters[11].unk2 = 0;
    rasters[12].unk2 = 0;
    rasters[13].unk2 = 0;
    rasters[14].unk2 = 0;
}

void WOR_Render_All_GO(struct sWorld* world, struct sGdi_DisplayData* dxdd)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGameObjectExtended* ext;
    tGameObjectBasic* basic;
    tViewElem* view;
    Mtx4x4* mtx;
    Vec3* pos;
    int flags2;
    int presFlags;
    double dxCam;
    double dyCam;
    double dzCam;
    float distSq;
    char tooFar;
    BOOL culled;
    int distanceToHide;
    LARGE_INTEGER PerformanceCount;

    QueryPerformanceCounter(&PerfCounterStruct[41]);
    if ((dxdd->flags & 0x80000) == 0)
    {
        elem = world->visibleGameObjectsPF.firstElemPtr;
        for (last = &((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr))[-1]; elem <= last; ++elem)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            if (!elem->objectPtr || ((uint8_t)gao & 3) == 1)
                continue;
            flags2 = gao->flags2 & ~0x20000u | 4;
            gao->flags2 = flags2;
            if (ENG_gb_ActiveSectorization && gao->if_this_not0_wont_render_gao)
            {
                gao->distance = 0;
                gao->flags2 = flags2 | 0x20000;
                continue;
            }
            mtx = gao->matrices;
            view = world->viewPtr;
            dxCam = mtx->Pos.x - view->cameraMtx.Pos.x;
            pos = &mtx->Pos;
            ext = gao->extended;
            tooFar = 0;
            if (!ext
                || ext->renderMinReqDistSq < 1.0
                || ext->renderMinReqDistSq == - 1.0f
                || (dzCam = pos->z - view->cameraMtx.Pos.z,
                    dyCam = pos->y - view->cameraMtx.Pos.y,
                    distSq = (float)(dzCam * dzCam + dyCam * dyCam + dxCam * dxCam),
                    ext->renderMinReqDistSq >= (double)distSq))
            {
                distanceToHide = world->distanceToHide;
                if (distanceToHide != -1 && gao->distance < distanceToHide)
                {
                    gao->distance = OBJ_uc_CalcLODVis(gao, view);
                    continue;
                }
            }
            else
            {
                tooFar = 1;
            }
            culled = OBJ_CullingObject(gao);
            if ((gao->flags2 & 0x10000000) != 0 || !tooFar && !culled && !WOR_BVMaskComputeInside(gao))
            {
                gao->distance = OBJ_uc_CalcLODVis(gao, world->viewPtr);
                if ((gao->flags & 0x400000) != 0 && (gao->flags & 8) == 0)
                    OBJ_ComputeGlobalWhenHie(gao);
                gao->flags2 = gao->flags2 & ~4u;
                continue;
            }
            gao->distance = 0;
        }
    }
    QueryPerformanceCounter(&PerformanceCount);
    if (PerfCounterStruct[41].QuadPart)
        PerfCountertimeSumArray[41] += PerformanceCount.QuadPart - PerfCounterStruct[41].QuadPart;
    PerfCounterStruct[41].QuadPart = 0LL;
    elem = world->visibleGameObjectsPF.firstElemPtr;
    if (elem <= &((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr))[-1])
    {
        while (1)
        {
            gao = (tGameObjectMain*)elem->objectPtr;
            last = ((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr)) - 1;
            if (elem->objectPtr && ((uint8_t)gao & 3) != 1)
            {
                if ((gao->flags2 & 4) == 0)
                {
                    MDF_ApplyAllGao((tGameObjectMain*)elem->objectPtr);
                    if ((gao->flags & 0x400000) != 0 && (gao->flags & 8) == 0)
                        OBJ_ComputeGlobalWhenHie(gao);
                    if ((gao->flags & 0x4000) != 0)
                    {
                        basic = gao->basic;
                        if (basic && basic->actorObjectOrVisualObject && !((tGameObjectVisualObject*)(basic->actorObjectOrVisualObject))->renderOnlyWithGFXRender)
                        {
                            dxdd->extraMaterialToGeo = basic->extraMaterial;
                            GRO_Render(gao);
                        }
                    }
                    else if ((gao->flags & 2) != 0)
                    {
                        basic = gao->basic;
                        if (basic)
                        {
                            if (basic->actorObjectOrVisualObject)
                            {
                                dxdd->extraMaterialToGeo = basic->extraMaterial;
                                ANI_Render(gao);
                            }
                        }
                    }
                    MDF_UnApplyAllGao(gao);
                    presFlags = dxdd->presentationFlags & ~0x20100u;
                    dxdd->extraMaterialToGeo = 0;
                    dxdd->presentationFlags = presFlags;
                }
                else if ((dxdd->flags & 0x80000) == 0)
                {
                    AI_ExecCallback(gao, 3);
                }
            }
            if (++elem > last)
                return;
        }
    }
}

void WOR_Universe_Close()
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tWorld* world;

    elem = WOR_gst_Universe.univWorldTablePF.firstElemPtr;
    last = ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1;
    if (WOR_gst_Universe.univWorldTablePF.firstElemPtr)
    {
        for (; elem <= last; ++elem)
        {
            world = (tWorld*)elem->objectPtr;
            if (elem->objectPtr && ((uint8_t)world & 3) != 1)
                WOR_Universe_DeleteWorld(world);
        }
        if (WOR_gst_Universe.univAIInstance)
        {
            AI_FreeInstance(WOR_gst_Universe.univAIInstance);
            WOR_gst_Universe.univAIInstance = 0;
        }
        if (AI_gpc_SaveBuffer)
        {
            MEM_Free(AI_gpc_SaveBuffer);
            AI_gpc_SaveBuffer = 0;
            AI_gi_SaveBufferSize = 0;
        }
        MAI_gst_MainHandles.currentWorld = 0;
        WOR_gst_Universe.unknownReloadFlag = -2;
    }
}

void WOR_Universe_Open(int aiInstanceKey)
{
    WOR_Universe_Close();
    if (aiInstanceKey == -1)
        WOR_gst_Universe.univAIInstance = 0;
    else
        WOR_gst_Universe.univAIInstance = AI_pst_LoadInstance(aiInstanceKey);
}

void WOR_World_Attached(tWorld* world)
{
    if (world)
    {
        GFX_Attached(world->gfxCurrentList);
        GFX_Attached(world->gfxCurrentListSortedAfterAE);
    }
}

void WOR_Render(struct sWorld* world, struct sGdi_DisplayData* dxdd)
{
    tViewElem* view;
    tViewElem* viewEnd;
    profilerRasterStructElem* raster;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tLIGHT* light;
    int lightFlags;
    int16_t insideCounter;
    int16_t newCounter;
    tFogParams fog;
    int viewIdx;

    if (GFX_gl_FS_VideoPlaying)
    {
        GFX_FS_VideoUpdate();
        return;
    }

    MAI_gst_MainHandles.displayData = dxdd;
    if (dxdd)
    {
        WOR_l_ResetRasterStats(dxdd->rasters);
        gpst_DX_stDD = dxdd;
        viewEnd = &world->viewPtr[world->viewNum];
        viewIdx = 0;
        for (view = world->viewPtr; view < viewEnd; ++view, ++viewIdx)
        {
            if ((view->flags & 1) != 0 && view->displayData == dxdd)
            {
                world->currViewElem = view;
                WOR_SetCam(view);
                CAM_Engine_ComputePlans(&dxdd->ct);
                OBJ_UpdateCullingVars(&dxdd->ct.cam);
                if (viewIdx)
                    dxdd->Clear(2, 0);
                raster = &dxdd->rasters[17];
                if (!dxdd->rasters[17].insideCounter)
                    raster->startTick = TIM_GetTicksAsInt32();
                ++raster->insideCounter;
                LIGHT_List_Reset(&dxdd->lightList);
                elem = world->lightObjectsTablePF.firstElemPtr;
                last = ((tableElemStructPF*)(world->lightObjectsTablePF.nextElemPtr)) - 1;
                for (; elem <= last; ++elem)
                {
                    gao = (tGameObjectMain*)(elem->objectPtr);
                    if (elem->objectPtr)
                    {
                        if (((uint8_t)gao & 3) != 1)
                        {
                            light = (tLIGHT*)(gao->extended->light);
                            if (light)
                            {
                                lightFlags = light->flags;
                                if ((lightFlags & 8) != 0)
                                {
                                    if ((lightFlags & 7) == 3)
                                    {
                                        if ((dxdd->fog.flags & 4) == 0)
                                        {
                                            fog.fogVertexMode = 0;
                                            fog.fogStart = light->fnear;
                                            fog.fogEnd = light->ffar;
                                            fog.fogColor = light->color;
                                            fog.density = light->density;
                                            fog.flags = 1;
                                            dxdd->Request(1, (void*)&fog);// fogparams
                                        }
                                    }
                                    else if ((!gao->if_this_not0_wont_render_gao || !ENG_gb_ActiveSectorization)
                                        && (lightFlags & 0x40000000) == 0)
                                    {
                                        LIGHT_List_AddLight(&dxdd->lightList, gao);
                                    }
                                }
                            }
                        }
                    }
                }
                raster = &dxdd->rasters[17];
                insideCounter = dxdd->rasters[17].insideCounter;
                if (insideCounter)
                {
                    newCounter = insideCounter - 1;
                    dxdd->rasters[17].insideCounter = newCounter;
                    if (!newCounter)
                        raster->sumTime += TIM_GetTicksAsInt32() - raster->startTick;
                }
                dxdd->rasters[4].unk2 = (float) dxdd->lightList.actualLightNum; // FIXME: ennek mi a típusa? float?
                WOR_SetCam(view);
                D3D_PrepareWaterRendering();
                SDW_UpdatePositions();
                PROTEX_BeforeDraw();
                WOR_BVMaskPreComputeAll(world, &dxdd->ct);
                WOR_Render_All_GO(world, dxdd);
                GFX_Render(&world->gfxCurrentList, 0);
                SOFT_ZList_Send();
                GAO_ModifierLazy_UnapplyAll();
                D3D_PC_AE(world);
                AI_EvalFunc_VIEWSnapshot_DoIt();
                if ((dxdd->presentationFlags & 0x4000000) == 0 && world->gfxCurrentListSortedAfterAE)
                {
                    dxdd->Request(31, NULL);
                    if ((dxdd->presentationFlags & 0x20000000) == 0)
                        dxdd->Clear(2, 0);
                    memcpy(&gpst_DX_stDD->ct.camBackup, &gpst_DX_stDD->ct, sizeof(gpst_DX_stDD->ct.camBackup));
                    CAM_SetCameraMatrix(&gpst_DX_stDD->ct, &MATH_gst_IdentityMatrix);
                    gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
                    CAM_Inverse(&gpst_DX_stDD->ct);
                    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
                    dxdd->flags |= 0x800u;
                    GFX_Render(&world->gfxCurrentListSortedAfterAE, 1);
                    dxdd->flags &= ~0x800u;
                    memcpy(&gpst_DX_stDD->ct, &gpst_DX_stDD->ct.camBackup, 400u);
                    dxdd->Request(32, NULL);
                }
            }
        }
        WOR_SetCam(world->viewPtr);
        gpst_DX_stDD = 0;
    }
}

void WOR_Universe_Destroy()
{
    WOR_Universe_Close();
    TAB_PFtable_Close(&WOR_gst_Universe.univWorldTablePF);
}

void WOR_CloseModule()
{
    WOR_Universe_Destroy();
}

tWorld* WOR_pst_Universe_MergeWorld(tWorld* wow, int worldKey, int unused1, int resolveAI)
{
    tWorld* result;

    if (!wow)
        return WOR_pst_Universe_AddWorld(worldKey, unused1, resolveAI);
    result = WOR_pst_World_Load(wow, worldKey, resolveAI);
    if (result)
    {
        if (resolveAI && WOR_gb_HasAddedMaterial)
        {
            if (wow->viewPtr)
                GDI_l_AttachWorld(wow->viewPtr->displayData, wow);
            WOR_gb_HasAddedMaterial = FALSE;
        }
        ENG_InitOneWorld(wow);
        if (resolveAI)
        {
            WOR_World_ActivateObjects(wow);
            EOT_SetOfEOT_Build(&wow->eot, &wow->activatedObjectsTable4PF);
        }
        result = wow;
    }
    return result;
}

struct sWorld* WOR_pst_Universe_AddWorldList(int unused0, int worldKey, int unused1)
{
    tableElemStructPF* curWorld;
    tWorld* world;
    tWorld* mergedWorld;
    int bigFileOffset;
    unsigned int pairNum;
    unsigned int i;
    int firstIdx;
    int lastIdx;
    int key;
    unsigned int preloadTexRet;
    char* cursor;
    int size;
    BASKeyValStruct worldKeyOffsetPairs[400];
    uint32_t binKey;

    memset(worldKeyOffsetPairs, 0, sizeof(worldKeyOffsetPairs)); // FIXME: a játék ezt nem csinálja, supresses a compiler warning

    WOR_gul_WorldKey = worldKey;
    WTR_BeginLoadWorldHook();
    WOR_gb_FirstTrameAfterLoad = TRUE;
    // a load takes far longer than a frame, and the frame after it
    // would otherwise be charged the whole of it. mark the start so
    // the time can be accounted rather than played.
    WOR_gf_LoadStartTime = TIM_f_Clock_TrueRead();
    DBG_OutputDebugString("===== LOAD BEGIN  WOL 0x%08X =====\n", worldKey);
    GRPSectClearAll();
    world = NULL;

    if (WOR_gst_Universe.univWorldTablePF.firstElemPtr) {
        for (curWorld = WOR_gst_Universe.univWorldTablePF.firstElemPtr; curWorld <= ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1; curWorld++) {
            if (!TAB_TableElemIsInvalid(curWorld->objectPtr)) {
                WOR_Universe_DeleteWorld((tWorld*)curWorld->objectPtr);
            }
        }
    }

    if (MAI_gst_MainHandles.displayData)
    {
        GDI_l_DetachWorld(MAI_gst_MainHandles.displayData);
        MAI_gst_MainHandles.displayData->RLIFactor = 1.0;
        MAI_gst_MainHandles.displayData->RLIDestColor = 0;
    }
    GFX_End();
    PC_AE_MASTER &= ~0x10008;
    SDW_Destroy();
    MAT_SpriteGen_Close();
    SND_FreeHeaderArray(&SND_gst_HeaderArray);
    STR_3DStringList_Clear();
    SOFT_ZList_Clear();
    MSG_gpo_Owner = NULL;
    TEXT_FreeAll();
    SND_MetaBankFreeList();
    SND_ResetAllMute();
    SND_StreamPrefetchFlushAll();
    TEX_List_Free(&TEX_gst_GlobalList);
    STR_ReinitAll();
    INO_Reinit();
    SAV_PhotoClose();
    MEM_Defrag(0);
    GFX_NeedGeomEx(1500, 1500, 4, 300, 1);
    GFX_NeedGeomExElem(0, 300);
    GFX_NeedGeomExElem(1, 300);
    GFX_NeedGeomExElem(2, 300);
    GFX_NeedGeomExElem(3, 300);
    SAV_PhotoInit();
    preloadTexRet = 0;
    PreLoadTex_Finished(&preloadTexRet);
    if (!preloadTexRet)
        AI_EvalFunc_DbgTraceInt_C();
    PreLoadMoveBlocks(worldKey);
    LOA_BeginSpeedMode(worldKey);
    MEM_gb_EnableTmp = 1;
    BIG_FreeGlobalBuffer();
    MEM_ResetTmpMemory();

    if (worldKey == -1)
        return NULL;

    bigFileOffset = BIG_ul_SearchKeyToPos(worldKey);

    // fix a crash when we try to load a map that demo bf doesn't contain.
    if (BIG_gi_ReadMode == 2) {
        binKey = BIG_ul_SearchKeyToPos(worldKey | 0xff000000);
        if (binKey == -1 || binKey == (worldKey | 0xff000000))
        {
            return NULL;
        }
    }
    // ---

    cursor = BIG_pc_ReadFileTmp(bigFileOffset, &size);
    for (i = 0; i < (unsigned int)size >> 3; ++i)
    {
        worldKeyOffsetPairs[i].key = LOA_ReadLong(&cursor, 0, 1);
        worldKeyOffsetPairs[i].val = LOA_ReadLong(&cursor, 0, 1);
    }
    pairNum = (unsigned int)size >> 3;
    lastIdx = -1;
    firstIdx = -1;
    for (i = 0; i < pairNum; ++i)
    {
        key = worldKeyOffsetPairs[i].key;
        if (key && key != -1)
        {
            if (firstIdx == -1)
                firstIdx = i;
            lastIdx = i;
        }
    }
    for (i = 0; i < pairNum; ++i)
    {
        key = worldKeyOffsetPairs[i].key;
        if (key && key != -1)
        {
            WOR_gb_RealAllocWorld = i == firstIdx;
            WOR_gb_FinalWorld = i == lastIdx;
            mergedWorld = WOR_pst_Universe_MergeWorld(world, key, unused1, 0);
            world = mergedWorld;
        }
    }
    WOR_gb_RealAllocWorld = 1;
    WOR_gb_FinalWorld = 1;
    LOA_EndSpeedMode();
    if (!world)
    {
        MEM_gb_EnableTmp = 0;
        return 0;
    }
    world->worldKey = worldKey;
    TEXT_LoadWorldText(WOR_gul_WorldText);
    
    if (WOR_gb_HasAddedMaterial)
        WOR_gb_HasAddedMaterial = FALSE;

    if (world->viewPtr->displayData)
        world->viewPtr->displayData->Request(35, 0);

    ENG_gb_ForceAttach = TRUE;
    WOR_World_ResolveAIRef(world);

    *AI_MainActor = NULL;

    WOR_World_ActivateObjects(world);
    EOT_SetOfEOT_Build(&world->eot, &world->activatedObjectsTable4PF);
    BIG_FreeGlobalBuffer();
    MAI_gst_MainHandles.currentWorld = world;
    DisplayAttach(MAI_gst_MainHandles.displayData);
    MEM_gb_EnableTmp = 0;
    MEM_ResetTmpMemory();
    ResetPreloadTexAll();
    D3D_CreateGameObjectsD3DStuffForMultiStream(world);
    DINPUT_InitVariables(0);

    return world;
}
