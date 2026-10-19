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

void EOT_SetOfEOT_Init(tEOTStructs* a1)
{
    TAB_PFtable_Init(&a1->animatedObjectsTablePF,       500, 1.0f);
    TAB_PFtable_Init(&a1->dynamicObjectsTablePF,        500, 1.0f);
    TAB_PFtable_Init(&a1->AICallbacksTablePF,           500, 1.0f);
    TAB_PFtable_Init(&a1->collMapObjectsTablePF,        500, 1.0f);
    TAB_PFtable_Init(&a1->collInstanceZDMGameObjectsPF, 500, 1.0f);
    TAB_PFtable_Init(&a1->collInstanceZDEGameObjectsPF, 500, 1.0f);
    TAB_PFtable_Init(&a1->eventGameObjectsTablePF,      500, 1.0f);
    TAB_PFtable_Init(&a1->objectsWithHiearchyTablePF,   500, 1.0f);
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

tGameObjectMain* WOR_GetLastChildObj(tGameObjectMain* a1)
{
    tGameObjectMain* result;
    tWorld* v2;
    tGameObjectBasic* v3;
    tGaoMatrix* v4;
    tGameObjectMain* v5;
    int v6;
    tGameObjectMain* v7;
    tGameObjectBasic* v8;
    tGaoMatrix* v9;
    int v10;
    tGameObjectMain* ret;

    if (!a1)
        return 0;
    v2 = WOR_gpst_CurrentWorld;
    if (!WOR_gpst_CurrentWorld)
        return 0;
    v3 = a1->basic;
    if (!v3)
        return 0;
    v4 = v3->mtx;
    if (!v4)
        return 0;
    v5 = v4->refGao2;
    if (!v5)
        return 0;
    v6 = 0;
    v7 = a1;
    if (WOR_i_GetAllChildrenOf(WOR_gpst_CurrentWorld, v5, &ret) == 1)
    {
        do
        {
            v7 = v5;
            v8 = v5->basic;
            if (!v8)
                return 0;
            v9 = v8->mtx;
            if (!v9)
                return 0;
            v5 = v9->refGao2;
            if (!v5)
                return 0;
            ++v6;
        } while (WOR_i_GetAllChildrenOf(v2, v5, &ret) == 1);
    }
    result = ret;
    if (ret == v7)
    {
        result = NULL;
    }
    else if (NULL != v7)
    {
        return NULL;
    }
    v10 = 0;
    if (v6 > 0)
    {
        while (WOR_i_GetAllChildrenOf(v2, result, &ret) == 1)
        {
            result = ret;
            if (++v10 >= v6)
                return result;
        }
        return NULL;
    }
    return result;
}

void EOT_RemoveObjectFromTableIfBadFlag(tableDescStruct* a1, int a2)
{
    tableElemStructPF* v2;
    tableElemStructPF* i;
    tGameObjectMain* v4;

    v2 = a1->firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->nextElemPtr)) - 1; v2 <= i; ++v2)
    {
        v4 = (tGameObjectMain*)v2->objectPtr;
        if (v2->objectPtr && ((uint8_t)v4 & 3) != 1 && (a2 & v2->objectFlags) != (a2 & v4->flags))
            TAB_PFtable_RemoveElem(a1, v2);
    }
}

void EOT_UpdateIdentityFlagsOfTable(tableDescStruct* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;
    tGameObjectMain* v3;

    v1 = a1->firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        v3 = (tGameObjectMain*)v1->objectPtr;
        if (v1->objectPtr)
        {
            if (((uint8_t)v3 & 3) != 1)
                v1->objectFlags = v3->flags;
        }
    }
}

void EOT_SetOfEOT_Close(tEOTStructs* a1)
{
    TAB_PFtable_Close(&a1->animatedObjectsTablePF);
    TAB_PFtable_Close(&a1->dynamicObjectsTablePF);
    TAB_PFtable_Close(&a1->AICallbacksTablePF);
    TAB_PFtable_Close(&a1->collMapObjectsTablePF);
    TAB_PFtable_Close(&a1->collInstanceZDMGameObjectsPF);
    TAB_PFtable_Close(&a1->collInstanceZDEGameObjectsPF);
    TAB_PFtable_Close(&a1->eventGameObjectsTablePF);
    TAB_PFtable_Close(&a1->objectsWithHiearchyTablePF);
}

void EOT_SetOfEOT_Clear(tEOTStructs* a1)
{
    TAB_PFtable_Clear(&a1->animatedObjectsTablePF);
    TAB_PFtable_Clear(&a1->dynamicObjectsTablePF);
    TAB_PFtable_Clear(&a1->AICallbacksTablePF);
    TAB_PFtable_Clear(&a1->collMapObjectsTablePF);
    TAB_PFtable_Clear(&a1->collInstanceZDMGameObjectsPF);
    TAB_PFtable_Clear(&a1->collInstanceZDEGameObjectsPF);
    TAB_PFtable_Clear(&a1->eventGameObjectsTablePF);
    TAB_PFtable_Clear(&a1->objectsWithHiearchyTablePF);
}

void EOT_AISortWithPriority(tableDescStruct* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    tGameObjectMain* v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    tableElemStructPF* v15;
    tableElemStructPF* v16;
    tGameObjectMain* v17;
    int v18;
    int v19;
    int* v20;
    tableElemStructPF* v21;
    int v22;
    int v23;
    int v24;
    int* v25;
    tableElemStructPF* v26;
    tGameObjectMain* v27;
    int v28;
    tableElemStructPF* v29;
    tableElemStructPF* v30;
    int v31;
    int v32[256];
    int v33[256];

restart:
    v1 = a1->nextElemPtr;
    memset(v33, 0, sizeof(v33));
    v2 = a1->firstElemPtr;
    v3 = v1 - 1;
    while (v2 <= v3)
    {
        v4 = (tGameObjectMain*)v2->objectPtr;
        if (!v2->objectPtr || ((uint8_t)v4 & 3) == 1)
        {
            TAB_PFtable_RemoveHoles(a1);
            goto restart;
        }
        v5 = v4->extended->AIPriority;
        ++v33[v5];
        ++v2;
    }
    v6 = 0;
    v32[0] = 0;
    do
    {
        v7 = v33[v6] + v32[v6];
        v8 = v33[v6 + 1];
        v32[v6 + 1] = v7;
        v9 = v8 + v7;
        v10 = v33[v6 + 2];
        v32[v6 + 2] = v9;
        v11 = v10 + v9;
        v12 = v33[v6 + 3];
        v32[v6 + 3] = v11;
        v13 = v12 + v11;
        v14 = v13 + v33[v6 + 4];
        v32[v6 + 4] = v13;
        v32[v6 + 5] = v14;
        v6 += 5;
    } while (v6 < 255);
    v15 = a1->firstElemPtr;
    v16 = ((tableElemStructPF *)(a1->nextElemPtr)) - 1;
    v17 = (tGameObjectMain*)-1;
    v30 = v15;
    v29 = v16;
    while (v15 <= v16)
    {
        if ((v15->objectFlags & 0x40000000) == 0)
        {
            v18 = ((tGameObjectMain *)(v15->objectPtr))->extended->AIPriority;
            v19 = v32[v18];
            v20 = &v32[v18];
            v21 = &v30[v19];
            if (v21 != v15 && v21 <= v16)
            {
                if (v17 != (tGameObjectMain*)-1) {
                    do
                    {
                        v23 = v17->extended->AIPriority;
                        v24 = v32[v23];
                        v25 = &v32[v23];
                        v26 = &v30[v24];
                        if (v26 > v16)
                            break;
                        v27 = (tGameObjectMain*)v26->objectPtr;
                        v28 = v26->objectFlags;
                        v26->objectPtr = v17;
                        v26->objectFlags = v31;
                        v26->objectFlags = v31 | 0x40000000;
                        if (v26 == v15)
                            ++v15;
                        v31 = v28;
                        v16 = v29;
                        v17 = v27;
                        ++* v25;
                    } while (v27 != (tGameObjectMain*)-1);
                    continue;
                }
                v17 = (tGameObjectMain*)v21->objectPtr;
                v31 = v21->objectFlags;
                v21->objectPtr = v15->objectPtr;
                v22 = v15->objectFlags;
                v21->objectFlags = v22;
                v21->objectFlags = v22 | 0x40000000;
                v16 = v29;
                v15->objectPtr = (void*)-1;
                *v20 = v19 + 1;
                if (v17 != (tGameObjectMain*)-1)
                {
                    do
                    {
                        v23 = v17->extended->AIPriority;
                        v24 = v32[v23];
                        v25 = &v32[v23];
                        v26 = &v30[v24];
                        if (v26 > v16)
                            break;
                        v27 = (tGameObjectMain*)v26->objectPtr;
                        v28 = v26->objectFlags;
                        v26->objectPtr = v17;
                        v26->objectFlags = v31;
                        v26->objectFlags = v31 | 0x40000000;
                        if (v26 == v15)
                            ++v15;
                        v31 = v28;
                        v16 = v29;
                        v17 = v27;
                        ++* v25;
                    } while (v27 != (tGameObjectMain*)-1);
                }
                continue;
            }
            *v20 = v19 + 1;
        }
        ++v15;
    }
}

void EOT_SetOfEOT_Build(tEOTStructs* a1, tableDescStruct* a2)
{
    tableElemStructPF* v2;
    tableElemStructPF* i;
    tableElemStructPF* v4;
    tableElemStructPF* v5;
    tableElemStructPF* v6;
    tableElemStructPF* v7;
    tableElemStructPF* v8;
    tableElemStructPF* v9;
    tableElemStructPF* v10;
    tableElemStructPF* v11;

    if (!PRO_EOTCreation.insideCounter)
        PRO_EOTCreation.startTick = TIM_GetTicksAsInt32();
    ++PRO_EOTCreation.insideCounter;
    EOT_SetOfEOT_Clear(a1);
    v2 = a2->firstElemPtr;
    for (i = ((tableElemStructPF*)(a2->nextElemPtr)) - 1; v2 <= i; ++v2)
    {
        if ((v2->objectFlags & 2) != 0)
        {
            ((tableElemStructPF*)(a1->animatedObjectsTablePF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->animatedObjectsTablePF.nextElemPtr))->objectFlags = v2->objectFlags;
            v4 = a1->animatedObjectsTablePF.nextElemPtr;
            ++a1->animatedObjectsTablePF.actElemNum;
            a1->animatedObjectsTablePF.nextElemPtr = v4 + 1;
        }
        if ((v2->objectFlags & 4) != 0)
        {
            OBJ_ResetProcess((tGameObjectMain*)v2->objectPtr, WOR_gpst_CurrentWorld->dynCallCount, 0x40);
            ((tableElemStructPF *)(a1->dynamicObjectsTablePF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->dynamicObjectsTablePF.nextElemPtr))->objectFlags = v2->objectFlags;
            v5 = a1->dynamicObjectsTablePF.nextElemPtr;
            ++a1->dynamicObjectsTablePF.actElemNum;
            a1->dynamicObjectsTablePF.nextElemPtr = v5 + 1;
        }
        if ((v2->objectFlags & 0x20) != 0)
        {
            OBJ_ResetProcess((tGameObjectMain*)v2->objectPtr, WOR_gpst_CurrentWorld->aiCallCount, 0x20);
            ((tableElemStructPF *)(a1->AICallbacksTablePF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->AICallbacksTablePF.nextElemPtr))->objectFlags = v2->objectFlags;
            v6 = a1->AICallbacksTablePF.nextElemPtr;
            ++a1->AICallbacksTablePF.actElemNum;
            a1->AICallbacksTablePF.nextElemPtr = v6 + 1;
        }
        if ((v2->objectFlags & 0x100) != 0)
        {
            ((tableElemStructPF *)(a1->collMapObjectsTablePF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->collMapObjectsTablePF.nextElemPtr))->objectFlags = v2->objectFlags;
            v7 = a1->collMapObjectsTablePF.nextElemPtr;
            ++a1->collMapObjectsTablePF.actElemNum;
            a1->collMapObjectsTablePF.nextElemPtr = v7 + 1;
        }
        if ((v2->objectFlags & 0x200) != 0)
        {
            ((tableElemStructPF *)(a1->collInstanceZDMGameObjectsPF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->collInstanceZDMGameObjectsPF.nextElemPtr))->objectFlags = v2->objectFlags;
            v8 = a1->collInstanceZDMGameObjectsPF.nextElemPtr;
            ++a1->collInstanceZDMGameObjectsPF.actElemNum;
            a1->collInstanceZDMGameObjectsPF.nextElemPtr = v8 + 1;
        }
        if ((v2->objectFlags & 0x400) != 0)
        {
            ((tableElemStructPF *)(a1->collInstanceZDEGameObjectsPF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->collInstanceZDEGameObjectsPF.nextElemPtr))->objectFlags = v2->objectFlags;
            v9 = a1->collInstanceZDEGameObjectsPF.nextElemPtr;
            ++a1->collInstanceZDEGameObjectsPF.actElemNum;
            a1->collInstanceZDEGameObjectsPF.nextElemPtr = v9 + 1;
        }
        if ((v2->objectFlags & 0x2000000) != 0)
        {
            ((tableElemStructPF *)(a1->eventGameObjectsTablePF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->eventGameObjectsTablePF.nextElemPtr))->objectFlags = v2->objectFlags;
            v10 = a1->eventGameObjectsTablePF.nextElemPtr;
            ++a1->eventGameObjectsTablePF.actElemNum;
            a1->eventGameObjectsTablePF.nextElemPtr = v10 + 1;
        }
        if ((v2->objectFlags & 0x400000) != 0)
        {
            OBJ_ResetProcess((tGameObjectMain*)v2->objectPtr, WOR_gpst_CurrentWorld->hierarchyCallCount, 0x80);
            ((tableElemStructPF *)(a1->objectsWithHiearchyTablePF.nextElemPtr))->objectPtr = v2->objectPtr;
            ((tableElemStructPF *)(a1->objectsWithHiearchyTablePF.nextElemPtr))->objectFlags = v2->objectFlags;
            v11 = a1->objectsWithHiearchyTablePF.nextElemPtr;
            ++a1->objectsWithHiearchyTablePF.actElemNum;
            a1->objectsWithHiearchyTablePF.nextElemPtr = v11 + 1;
        }
    }

    EOT_AISortWithPriority(&a1->AICallbacksTablePF);

    if (PRO_EOTCreation.insideCounter)
    {
        if (!--PRO_EOTCreation.insideCounter)
            PRO_EOTCreation.sumTime += TIM_GetTicksAsInt32() - PRO_EOTCreation.startTick;
    }
}

void EOT_SetOfEOT_OneBuild(tableDescStruct* a1, int a2, tableDescStruct* a3)
{
    tableElemStructPF* v3;
    tableElemStructPF* i;
    int* v5;
    BOOL v6;
    tableElemStructPF* v7;

    if (!PRO_EOTCreation.insideCounter)
        PRO_EOTCreation.startTick = TIM_GetTicksAsInt32();
    ++PRO_EOTCreation.insideCounter;
    TAB_PFtable_Clear(a1);
    v3 = a3->firstElemPtr;
    for (i = ((tableElemStructPF*)(a3->nextElemPtr)) - 1; v3 <= i; ++v3)
    {
        v5 = (int*)v3->objectPtr;
        if (v3->objectPtr)
        {
            if (((uint8_t)v5 & 3) != 1)
            {
                v6 = (*v5 & a2) == 0;
                v3->objectFlags = *v5;
                if (!v6)
                {
                    ((tableElemStructPF*)(a1->nextElemPtr))->objectPtr = v5;
                    ((tableElemStructPF*)(a1->nextElemPtr))->objectFlags = v3->objectFlags;
                    v7 = a1->nextElemPtr;
                    ++a1->actElemNum;
                    a1->nextElemPtr = v7 + 1;
                }
            }
        }
    }
    if (PRO_EOTCreation.insideCounter)
    {
        if (!--PRO_EOTCreation.insideCounter)
            PRO_EOTCreation.sumTime += TIM_GetTicksAsInt32() - PRO_EOTCreation.startTick;
    }
}

void WOR_World_ResolveAIRef(struct sWorld* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;
    tGameObjectMain* v3;
    tGameObjectDesignFields* v4;
    unsigned int v5;
    unsigned int v6;
    unsigned int v7;
    unsigned int v8;

    v1 = a1->objectTablePF.firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->objectTablePF.nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        v3 = (tGameObjectMain*)v1->objectPtr;
        if (v1->objectPtr)
        {
            if (((unsigned __int8)v3 & 3) != 1)
            {
                OBJ_ResolveAIRef((tGameObjectMain*)v1->objectPtr);
                if ((v3->flags & 0x40) != 0)
                {
                    v4 = v3->extended->fields;
                    if (v4->designNet1)
                    {
                        v5 = BIG_ul_SearchKeyToPos(v4->designNet1);
                        if (v5 != -1)
                            v4->designNet1 = LOA_ul_SearchAddress(v5);
                    }
                    if (v4->designNet2)
                    {
                        v6 = BIG_ul_SearchKeyToPos(v4->designNet2);
                        if (v6 != -1)
                            v4->designNet2 = LOA_ul_SearchAddress(v6);
                    }
                    if (v4->designGO1)
                    {
                        v7 = BIG_ul_SearchKeyToPos(v4->designGO1);
                        if (v7 != -1)
                            v4->designGO1 = LOA_ul_SearchAddress(v7);
                    }
                    if (v4->designGO2)
                    {
                        v8 = BIG_ul_SearchKeyToPos(v4->designGO2);
                        if (v8 != -1)
                            v4->designGO2 = LOA_ul_SearchAddress(v8);
                    }
                }
            }
        }
    }
}

void WOR_World_AttachGameObject(struct sWorld* a1, struct sGameObjectMain* a2)
{
    tGameObjectMain* v2;
    tWorld* v3;
    tGameObjectExtended* v4;
    tGroup* v5;
    tLIGHT* v6;
    tGameObjectMain* v7;
    tGameObjectBasic* v8;
    tGameObjectVisualObject* v9;
    tGEOGRO* v10;
    int* v11;
    int v12;
    int* v13;
    tGEOMAT* v14;
    tableDescStruct* v15;
    int v16;
    tGEOMAT** v17; 
    tGEOMAT** i;
    tGEOMAT* v19;
    int v20; 
    tGameObjectExtended* v21;
    tModifier* j;
    tXmenMdfStruct* v23;
    tGEOMAT* v24;
    //tableElemStructPF* v25;
    tGameObjectActorObject* v26;
    tAnim* v27;

    v2 = a2;
    v3 = a1;
    WOR_World_AddLoadedObject(a1, a2, 0);
    if ((a2->flags & 0x800000) != 0)
    {
        v4 = a2->extended;
        if (v4 && (v5 = v4->grp) != 0)
            ++v5->groupElemNum;
        else
            OBJ_ChangeIdentityFlags(a2, a2->flags & ~0x800000u, -1);
    }
    if ((a2->flags & 0x10) != 0)
    {
        v6 = (tLIGHT*)(a2->extended->light);
        if (v6)
        {
            v6->intf->AddRef((tGroGeneric*)v6, 1);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(&a1->worldGROTable, v6) == -1)
            {
                TAB_PTable_AddElemToTable(&a1->worldGROTable, v6);
                v6->intf->AddRef((tGroGeneric*)v6, 1);
            }
        }
    }
    if ((a2->flags & 0x400000) != 0)
    {
        v7 = a2->basic->mtx->refGao2;
        if (v7)
            v7->flags2 |= 0x40u;
    }
    if ((a2->flags & 0x4000) != 0)
    {
        v8 = a2->basic;
        v9 = v8->actorObjectOrVisualObject;         // vigyázz mire van állítva, itt Visual kell
        v10 = (tGEOGRO*)(((tGameObjectVisualObject*)(v8->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
        if (v10)
        {
            v10->intf->AddRef((tGroGeneric*)v10, 1);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(&a1->worldGROTable, v10) == -1)
            {
                TAB_PTable_AddElemToTable(&a1->worldGROTable, v10);
                v10->intf->AddRef((tGroGeneric*)v10, 1);
            }
            v11 = v9->RLIVertexColors;
            if (v11)
            {
                if (v10->intf->id == 1)
                {
                    v12 = v10->vertexNum;
                    if (*v11 != v12)
                    {
                        v13 = (int*)MEM_p_Realloc(v11, 4 * v12 + 4);
                        v9->RLIVertexColors = v13;
                        *v13 = v10->vertexNum;
                    }
                }
            }
            v3 = a1;
        }
        else
        {
            if (v9->RLIVertexColors)
                MEM_Free(v9->RLIVertexColors);
            v9->RLIVertexColors = 0;
        }
        v14 = (tGEOMAT*)(v9->GROMAT);
        if (v14)
        {
            v15 = &v3->worldMaterialTable;
            if (!v14->usageCount && v14->intf->id == 4)
            {
                v16 = v14->multiMaterialNumIfType4ThenAmbiantColor;
                if (v16)
                {
                    v17 = v14->multiMaterialPtrIfType4ThenDiffuseColor;
                    for (i = &v17[v16]; v17 < i; ++v17)
                    {
                        v19 = *v17;
                        if (*v17)
                        {
                            v20 = v19->intf->id;
                            if (v20 == 3 || v20 == 5)
                            {
                                v19->intf->AddRef((tGroGeneric*)v19, 1);
                                if (TAB_ul_Ptable_GetElemIndexWithPointer(v15, *v17) == -1)
                                {
                                    TAB_PTable_AddElemToTable(v15, *v17);
                                    (*v17)->intf->AddRef((tGroGeneric*)*v17, 1);
                                    WOR_gb_HasAddedMaterial = TRUE;
                                }
                            }
                            else
                            {
                                *v17 = 0;
                            }
                        }
                    }
                    v2 = a2;
                }
            }
            v14->intf->AddRef((tGroGeneric*)v14, 1);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(v15, v14) == -1)
            {
                TAB_PTable_AddElemToTable(v15, v14);
                v14->intf->AddRef((tGroGeneric*)v14, 1);
                WOR_gb_HasAddedMaterial = TRUE;
            }
        }
    }
    v21 = v2->extended;
    if (v21)
    {
        for (j = v21->mdf; j; j = j->nextElem)
        {
            if (j->modifierIntf->mdfId == 9)        // xmen
            {
                v23 = (tXmenMdfStruct*)(j->mdfPrivateDataPtr);
                v24 = v23->geomat;
                if (v24)
                {
                    if (TAB_ul_Ptable_GetElemIndexWithPointer(&a1->worldMaterialTable, v23->geomat) == -1) 
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
                        TAB_PTable_AddElemToTable(&a1->worldMaterialTable, v24);
                        v24->intf->AddRef((tGroGeneric*)v24, 1);
                        WOR_gb_HasAddedMaterial = TRUE;
                    }
                }
            }
        }
    }
    if ((v2->flags & 2) != 0)
    {
        v26 = (tGameObjectActorObject*)v2->basic->actorObjectOrVisualObject;
        if (v26->actionKit)
        {
            ACT_SetUsageCounts(v26->actionKit);
            EVE_gb_PlayOnlyAnimTracks = 1;
            ACT_SetAction(v2, 0, 0);
            EVE_gb_PlayOnlyAnimTracks = 0;
        }
        else
        {
            v27 = v26->animPtr[0];
            if (v27 && v27 != (tAnim*)-1)
            {
                ++v27->ptr->eventListStruct->usageCount;
                v26->animPtr[0]->frequency = 60;
            }
            if (v26->flagsEgBlendIw2)
            {
                EVE_gb_PlayOnlyAnimTracks = 1;
                ANI_PlayGameObjectAnimation(v2, v26, 0.0f, 0);
                EVE_gb_PlayOnlyAnimTracks = 0;
            }
        }
    }
}

void WOR_World_JustAfterLoadObject(struct sWorld* a1, struct sGameObjectMain* a2, int resolveAI, int attach)
{
    tGameObjectActorObject* v4;
    tAnim* v5;

    OBJ_Reinit(a2);
    OBJ_Gizmo_UpdateGizmoPtr(a2);
    if ((a2->flags & 0x2000) != 0 && a2->extended->mdf)
        SND_ResolveRefsForMdF(a2->extended->mdf);
    if ((a2->flags & 0x8000000) != 0)
        SND_ResolveRefs(a2);
    if ((a2->flags & 0x700) != 0)
        COL_ResolveRefs(a2);
    if ((a2->flags & 2) != 0)
    {
        ANI_ResolveRefs(a2);
        if ((a2->flags & 2) != 0)
        {
            v4 = (tGameObjectActorObject*)a2->basic->actorObjectOrVisualObject;
            if (!v4->actionKit)
            {
                v5 = v4->animPtr[0];
                if (v5)
                    EVE_ResolveListTrackRef(v5->ptr->eventListStruct);
            }
        }
    }
    if (attach)
        WOR_World_AttachGameObject(a1, a2);
    if (resolveAI)
        OBJ_ResolveAIRef(a2);
}

void WOR_World_CheckFather(tGameObjectMain* a1, tWorld* a2)
{
    tGameObjectBasic* v2;
    tGaoMatrix* v3;
    tGameObjectMain* v4;
    tGameObjectMain* v5;
    tGaoMatrix* v6;

    if ((a1->flags & 0x400000) != 0)
    {
        v2 = a1->basic;
        if (v2 && (v3 = v2->mtx) != 0 && (v4 = v3->refGao2) != 0)
        {
            if (TAB_ul_PFtable_GetElemIndexWithPointer(&a2->objectTablePF, v4) == -1)
            {
                v5 = a1->basic->mtx->refGao2;
                WOR_World_DetachObject(a2, v5);
                OBJ_GameObject_Remove(v5, FALSE);
            }
        }
        else
        {
            a1->flags &= ~0x400000u;
            v6 = v2->mtx;
            if (v6)
                MEM_Free(v6);
            a1->basic->mtx = 0;
        }
    }
}

void WOR_World_CheckGroup(tGameObjectMain* a1, tWorld* a2)
{
    tGameObjectExtended* v2;
    tGroup* v3;
    tableElemStructPF* v4;
    tableElemStructPF* v5;
    tGameObjectMain* v6;
    tableDescStruct* v7;
    int v8;
    tableElemStructPF* i;

    if ((a1->flags & 0x800000) != 0)
    {
        v2 = a1->extended;
        if (v2)
        {
            v3 = v2->grp;
            if (v3)
            {
                v4 = v3->tablePF->firstElemPtr;
                v5 = ((tableElemStructPF*)(v3->tablePF->nextElemPtr)) - 1;
                for (i = v5; v4 <= v5; ++v4)
                {
                    v6 = (tGameObjectMain*)v4->objectPtr;
                    if (v4->objectPtr && ((uint8_t)v6 & 3) != 1)
                    {
                        if (TAB_ul_PFtable_GetElemIndexWithPointer(&a2->objectTablePF, v4->objectPtr) == -1)
                        {
                            WOR_World_DetachObject(a2, v6);
                            OBJ_GameObject_Remove(v6, TRUE);
                            v7 = v3->tablePF;
                            if (v4->objectPtr)
                            {
                                if (((int)v4->objectPtr & 3) != 1)
                                {
                                    v4->objectPtr = (void*)1;
                                    v8 = v7->actElemNum - 1;
                                    ++v7->holeNum;
                                    v7->actElemNum = v8;
                                }
                            }
                            v4->objectFlags = 0;
                        }
                        v5 = i;
                    }
                }
            }
        }
    }
}

void * WOR_ul_GameObjectGroupCallback(uint32_t Offset)
{
    char* v1;
    int v2;
    tableDescStruct* v3;
    int v4;
    char* a1; 
    unsigned int size;

    v1 = BIG_pc_ReadFileTmp(Offset, (int*)&size);
    a1 = v1;
    TAB_PFtable_Resize(&WOR_gpst_WorldToLoadIn->objectTablePF, size);
    if (v1 < &v1[size])
    {
        do
        {
            v2 = LOA_ReadLong(&a1, 0, 1);
            LOA_ReadLong(&a1, 0, 2);
            if (v2)
            {
                v3 = &WOR_gpst_WorldToLoadIn->objectTablePF;// pftable inline sztem
                ((tableElemStructPF*)(WOR_gpst_WorldToLoadIn->objectTablePF.nextElemPtr))->objectPtr = NULL;
                ((tableElemStructPF*)(v3->nextElemPtr))->objectFlags = 0;
                v4 = v3->actElemNum + 1;
                ++((tableElemStructPF*)(v3->nextElemPtr));
                v3->actElemNum = v4;
                LOA_MakeFileRef(v2, &(((tableElemStructPF*)(WOR_gpst_WorldToLoadIn->objectTablePF.nextElemPtr))[-1].objectPtr), OBJ_ul_GameObjectCallback, 1);
            }
        } while (a1 < &v1[size]);
    }
    return NULL;
}

tWorld* WOR_ul_World_LoadCallback(uint32_t Offset)
{
    tWorld* v1;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    char* a1;
    char Str1[4];
    int v12;
    int a2;

    v1 = WOR_pst_World_Create();
    v1->worldKey = LOA_ul_GetCurrentKey();
    a1 = BIG_pc_ReadFileTmp(Offset, &a2);
    LOA_ReadCharArray(&a1, Str1, 4u, 1);
    if (strncmp(Str1, ".wow", 4u))
        return 0;
    a2 = LOA_ReadLong(&a1, 0, 1);
    if (a2 != 3)
        return 0;
    v1->deletedObjNum = 0;
    v3 = LOA_ReadLong(&a1, 0, 1);
    v1->gaoTableMaxElemNum = v3;
    WOR_World_Init(v1, v3);
    v1->ambientColor1 = LOA_ReadLong(&a1, 0, 1);
    LOA_ReadString(&a1, v1->worldName, 60, 1);
    DBG_OutputDebugString("Loading world: %s\n", v1->worldName);
    LOA_ReadLong(&a1, 0, 2);
    LOA_ReadMatrix(&a1, &v1->unkMatrix, 1);
    if (!MATH_b_TypeIsCorrect(&v1->unkMatrix))
    {
        MATH_SetIdentityMatrix(&v1->unkMatrix);
        v1->unkMatrix.Pos.x = 0.0;
        v1->unkMatrix.Pos.y = 0.0;
        v1->unkMatrix.Pos.z = 0.0;
    }
    WOR_gpst_WorldToLoadIn = v1;
    v1->viewPtr->focale = LOA_ReadFloat(&a1, 0, 1);
    v1->clearColor = LOA_ReadLong(&a1, 0, 1);
    v4 = LOA_ReadLong(&a1, 0, 1);
    v1->ambientColor2 = v4;
    if (v4 == -1)
        v1->ambientColor2 = v1->ambientColor1;
    v5 = LOA_ReadLong(&a1, 0, 1);
    v1->distanceToHide = v5;
    if (v5 < 0 || v5 > 255)
        v1->distanceToHide = 0;
    LOA_ReadCharArray(&a1, 0, 44u, 2);
    v6 = LOA_ReadLong(&a1, 0, 1);
    v12 = v6;
    if (v6 && v6 != -1)
        LOA_MakeFileRef(v6, &v1->grid2Ptr, GRID_ul_CallbackWorldGrid, 1);
    v7 = LOA_ReadLong(&a1, 0, 1);
    v12 = v7;
    if (v7 && v7 != -1)
        LOA_MakeFileRef(v7, &v1->grid1Ptr, GRID_ul_CallbackWorldGrid, 1);
    v12 = LOA_ReadLong(&a1, 0, 1);
    LOA_MakeFileRef(v12, 0, WOR_ul_GameObjectGroupCallback, 5);
    v8 = LOA_ReadLong(&a1, 0, 1);
    v12 = v8;
    if (v8 && v8 != -1)
        LOA_MakeFileRef(v8, &v1->wayPtr, WAY_ul_AllNetWorksCallback, 1);
    TEXT_FreeAll();
    WOR_gul_WorldText = -1;
    v9 = LOA_ReadLong(&a1, 0, 1);
    v12 = v9;
    if (v9 && v9 != -1 && BIG_ul_SearchKeyToPos(v9) != -1)
        WOR_gul_WorldText = v12;
    return v1;
}

tWorld* WOR_pst_World_Load(tWorld* a2, int worldKey, int resolveAI)
{
    unsigned int v3;
    tWorld* result;
    tWorld* v5;
    tableDescStruct* v6;
    tableElemStructPF* v7;
    tableElemStructPF* i;
    tGameObjectMain* v9;
    tWorld* v10;
    signed int v11;
    signed int j;
    tGameObjectMain* v13;
    tableElemStructPF* v14;
    tGameObjectMain* v15;
    tableElemStructPF* v16;
    tableElemStructPF* v17;
    tGameObjectMain* v18;
    tWayAllNetworks* v19;
    int v20;
    int v21;
    tWayNetwork* v22;
    tWayAllNetworks* v23;
    int v24;
    tWorld* v25;
    tWayAllNetworks* v26;
    int v27;
    int v28;
    int v29;
    tWayNetwork* v30;
    tableElemStructP* v31 = NULL; // TODO FIXME, ez nem állítódik null-ra a játékban
    tableElemStructP* v32;
    tGEOSTATICLOD* v33;
    char v34;
    tGroGeneric* v35;
    tableElemStructP* v36;
    signed int v37;
    tableElemStructP* v38;
    BOOL v39;
    unsigned int v40;
    tWorld* world;
    tableElemStructPF* v42;
    tableElemStructP* v42a;
    int v43;
    tableElemStructPF* v43a;
    unsigned int v44;
    tableElemStructP* k;

    v44 = 0;
    if (worldKey == -1)
        return 0;
    v3 = BIG_ul_SearchKeyToPos(worldKey);
    if (v3 == -1 || (result = (tWorld*)LOA_ul_SearchAddress(v3), world = result, result == (tWorld*)-1))
    {
        world = NULL;
        LOA_MakeFileRef(worldKey, &world, WOR_ul_World_LoadCallback, 1);
        LOA_Resolve();
        if (!world)
            return 0;
        v5 = a2;
        if (!a2)
            v5 = world;
        v6 = &v5->worldGROTable;
        if (world != v5)
        {
            TAB_Ptable_RemoveHoles(&v5->worldMaterialTable);
            TAB_Ptable_RemoveHoles(&v5->worldGROTable);
            v44 = v5->worldGROTable.actElemNum;
            if (world != v5)
            {
                v7 = v5->objectTablePF.firstElemPtr;
                for (i = ((tableElemStructPF*)(v5->objectTablePF.nextElemPtr)) - 1; v7 <= i; ++v7)
                {
                    v9 = (tGameObjectMain*)(v7->objectPtr);
                    if (v7->objectPtr && ((uint8_t)v9 & 3) != 1)
                        v9->process |= 0x10u;
                }
            }
        }
        v10 = world;
        v11 = world->objectTablePF.holeNum + world->objectTablePF.actElemNum;
        for (j = 0; j < v11; ++j)
        {
            v13 = (tGameObjectMain*)(((tableElemStructPF*)(v10->objectTablePF.firstElemPtr))[j].objectPtr);
            if (v13 && ((uint8_t)v13 & 3) != 1 && (v13->process & 0x10) == 0)
            {
                WOR_gpst_CurrentWorld = v5;
                WOR_World_JustAfterLoadObject(v5, v13, resolveAI, 1);
            }
            v10 = world;
        }
        if (v10 != v5)
        {
            v14 = ((tableElemStructPF*)(v10->objectTablePF.nextElemPtr)) - 1;
            v43a = v10->objectTablePF.firstElemPtr;
            v42 = v14;
            if (v43a <= v14)
            {
                do
                {
                    v15 = (tGameObjectMain*)v43a->objectPtr;
                    if (v43a->objectPtr && ((uint8_t)v15 & 3) != 1)
                    {
                        if ((v15->process & 0x10) != 0)
                            ENG_ReinitOneObject(v15, 14);
                        v15->process &= ~0x10u;
                        if (world != v5 && TAB_ul_PFtable_GetElemIndexWithPointer(&v5->objectTablePF, v15) == -1)
                        {
                            v15->process &= ~0x10u;
                            TAB_PFTable_AddElemToTable(&v5->objectTablePF, v15, 0);
                        }
                    }
                    ++v43a;
                } while (v43a <= v42);
                v10 = world;
            }
        }
        v16 = v5->objectTablePF.firstElemPtr;
        v42 = ((tableElemStructPF*)(v5->objectTablePF.nextElemPtr)) - 1;
        v17 = v42;
        if (v16 <= v42)
        {
            do
            {
                v18 = (tGameObjectMain*)v16->objectPtr;
                if (v16->objectPtr && ((uint8_t)v18 & 3) != 1)
                {
                    WOR_World_CheckFather(v18, v5);
                    WOR_World_CheckGroup(v18, v5);
                    v17 = v42;
                }
                ++v16;
            } while (v16 <= v17);
            v10 = world;
        }
        if (v10 == v5)
            goto LABEL_64;
        memcpy(&v5->unkMatrix, &v10->unkMatrix, sizeof(v5->unkMatrix));
        v10 = world;
        if (world == v5)
            goto LABEL_64;
        v5->ambientColor1 = world->ambientColor1;
        v5->ambientColor2 = world->ambientColor2;
        v5->clearColor = world->clearColor;
        v5->distanceToHide = world->distanceToHide;
        v10 = world;
        if (world == v5)
            goto LABEL_64;
        v19 = v5->wayPtr;
        if (v19)
        {
            v20 = v19->networkNum;
            if (world->wayPtr)
            {
                v21 = 0;
                if (v20 > 0)
                {
                    do
                    {
                        v22 = v5->wayPtr->networks[v21];
                        if (v22)
                            v22->flags |= 4u;
                        ++v21;
                    } while (v21 < v5->wayPtr->networkNum);
                }
                v23 = v5->wayPtr;
                v24 = v23->networkNum;
                v23->networkNum = v24 + world->wayPtr->networkNum;
                v5->wayPtr->networks = (tWayNetwork**)MEM_p_Realloc(v5->wayPtr->networks, 4 * v5->wayPtr->networkNum);
                v25 = world;
                v26 = world->wayPtr;
                v27 = 0;
                if (v26->networkNum > 0)
                {
                    v28 = v24;
                    do
                    {
                        v5->wayPtr->networks[v28] = v26->networks[v27];
                        v25 = world;
                        v26 = world->wayPtr;
                        ++v27;
                        ++v28;
                    } while (v27 < v26->networkNum);
                }
                MEM_Free(v25->wayPtr->networks);
                world->wayPtr->networks = v5->wayPtr->networks;
                world->wayPtr->networkNum = v5->wayPtr->networkNum;
                LOA_DeleteAddress((int)v5->wayPtr);
                MEM_Free(v5->wayPtr);
                v5->wayPtr = world->wayPtr;
                world->wayPtr = 0;
            }
            else
            {
                v29 = 0;
                if (v20 > 0)
                {
                    do
                    {
                        v30 = v5->wayPtr->networks[v29];
                        if (v30)
                            v30->flags |= 4u;
                        ++v29;
                    } while (v29 < v5->wayPtr->networkNum);
                }
            }
        }
        else
        {
            v5->wayPtr = world->wayPtr;
            world->wayPtr = 0;
        }
        v10 = world;
        if (world == v5)
            goto LABEL_64;
        if (v5->grid1Ptr)
            GRI_Destroy(v5->grid1Ptr);
        if (v5->grid2Ptr)
            GRI_Destroy(v5->grid2Ptr);
        v5->grid1Ptr = world->grid1Ptr;
        v5->grid2Ptr = world->grid2Ptr;
        world->grid1Ptr = 0;
        world->grid2Ptr = 0;
        v10 = world;
        if (world == v5)
            LABEL_64:
        v31 = v5->worldGROTable.firstElemPtr;
        else
            v31 = &((tableElemStructP*)(v5->worldGROTable.firstElemPtr))[v44];
        v32 = ((tableElemStructP*)(v5->worldGROTable.nextElemPtr)) - 1;
        v42a = v31;
        for (k = v32; v42a <= k; ++v42a)
        {
            if (((tGroGeneric*)(v42a->objectPtr))->intf->id == 8)      // staticlod
            {
                v33 = (tGEOSTATICLOD*)(v42a->objectPtr);
                v34 = v33->lodNum;
                v44 = 0;
                if (v34)
                {
                    v43 = 0;
                    do
                    {
                        v35 = (tGroGeneric*)(v33->geo[v43]); // fixed: v35 = (tGroGeneric*)&(v33->geo[v43]);
                        if (v35)
                        {
                            if (v35->intf->id == 1)
                            {
                                v35->intf->AddRef(v35, 1);
                                if (TAB_ul_Ptable_GetElemIndexWithPointer(&v5->worldGROTable, v35) == -1)
                                {
                                    v36 = (tableElemStructP*)v5->worldGROTable.nextElemPtr;
                                    if (v36 <= (tableElemStructP*)(v6->lastElemPtr))
                                        v37 = -1;
                                    else
                                        v37 = v42a - (tableElemStructP*)(v5->worldGROTable.firstElemPtr);
                                    if (v36 > (tableElemStructP*)(v6->lastElemPtr))
                                        TAB_Ptable_Resize(&v5->worldGROTable, 2 * (v5->worldGROTable.holeNum + v5->worldGROTable.actElemNum));
                                    ((tableElemStructP*)(v5->worldGROTable.nextElemPtr))->objectPtr = v35;
                                    v38 = v5->worldGROTable.nextElemPtr;
                                    ++v5->worldGROTable.actElemNum;
                                    v5->worldGROTable.nextElemPtr = v38 + 1;
                                    v35->intf->AddRef(v35, 1);
                                    if (v37 != -1)
                                    {
                                        v42a = &(((tableElemStructP*)(v5->worldGROTable.firstElemPtr))[v37]);
                                        k = ((tableElemStructP*)(v5->worldGROTable.nextElemPtr)) - 1;
                                    }
                                }
                            }
                            else
                            {
                                v33->geo[v43] = NULL;
                            }
                        }
                        v33 = v42a->objectPtr;
                        v39 = ++v44 < ((tGEOSTATICLOD*)(v42a->objectPtr))->lodNum;
                        ++v43;
                    } while (v39);
                    v10 = world;
                }
            }
        }
        if (v5 != v10)
        {
            v5->snp = v10->snp;
            world->snp = 0;
            strcpy(v5->worldName, world->worldName);
            TAB_PFtable_Close(&world->objectTablePF);
            WOR_World_Destroy(world);
            LOA_DeleteAddress((int)v5);
            v40 = BIG_ul_SearchKeyToPos(worldKey);
            LOA_AddPosAddress(v40, (unsigned int)v5);
        }
        result = v5;
    }
    return result;
}

float _floatIsNegative(float f) {
    int x = (*((uint32_t*)(&f))) & 0xbf800000 | 0x3f800000;
    return *((float *)(&x));
}

int WOR_BVMaskComputeInside(tGameObjectMain* a1)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectBV* v4;
    float v5;
    Mtx4x4* v6;
    double v7;
    tBVMasks *i;
    float v9;
    double v10;
    int v11;
    double v12;
    int v13;
    double v14;
    double v15;
    Vec3* v16;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    double v24;
    double v25;
    double v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    double v33;
    int v34;
    Vec3* v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    float v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    float v60;
    float v61;
    float v62;
    float v63;
    Vec3 a1a[8];
    int v65;
    float v66;
    float v67;
    float v68;

    v1 = BVMask_gi_Num;
    if (!BVMask_gi_Num)
        return 0;
    v3 = a1;
    v4 = a1->boundingVolume;
    a1a[0] = v4->BVMax;
    a1a[1].x = v4->BVMin.x;
    a1a[1].y = v4->BVMin.y;
    v5 = v4->BVMin.z;
    a1a[2].x = a1a[0].x;
    a1a[2].y = a1a[1].y;
    a1a[3].y = a1a[1].y;
    a1a[3].x = a1a[1].x;
    a1a[4].x = a1a[1].x;
    a1a[1].z = v5;
    a1a[4].y = a1a[0].y;
    a1a[5].y = a1a[0].y;
    a1a[5].x = a1a[0].x;
    a1a[6].x = a1a[0].x;
    a1a[2].z = a1a[0].z;
    a1a[3].z = a1a[0].z;
    a1a[4].z = a1a[0].z;
    a1a[6].y = a1a[1].y;
    a1a[7].x = a1a[1].x;
    v6 = a1->matrices;
    a1a[5].z = v5;
    a1a[6].z = v5;
    a1a[7].y = a1a[0].y;
    a1a[7].z = v5;
    a1a[0].x = a1a[0].x + v6->Pos.x;
    a1a[0].y = a1a[0].y + v6->Pos.y;
    a1a[0].z = a1a[0].z + v6->Pos.z;
    a1a[1].x = a1a[1].x + v6->Pos.x;
    a1a[1].y = a1a[1].y + v6->Pos.y;
    a1a[1].z = v5 + v6->Pos.z;
    a1a[2].x = a1a[2].x + v6->Pos.x;
    a1a[2].y = a1a[2].y + v6->Pos.y;
    a1a[2].z = a1a[2].z + v6->Pos.z;
    a1a[3].x = a1a[3].x + v6->Pos.x;
    a1a[3].y = a1a[3].y + v6->Pos.y;
    a1a[3].z = a1a[3].z + v6->Pos.z;
    v7 = a1a[4].x + v6->Pos.x;
    v65 = 0;
    a1a[4].x = (float)v7;
    a1a[4].y = a1a[4].y + v6->Pos.y;
    a1a[4].z = a1a[4].z + v6->Pos.z;
    a1a[5].x = a1a[5].x + v6->Pos.x;
    a1a[5].y = a1a[5].y + v6->Pos.y;
    a1a[5].z = v5 + v6->Pos.z;
    a1a[6].x = a1a[6].x + v6->Pos.x;
    a1a[6].y = a1a[6].y + v6->Pos.y;
    a1a[6].z = v5 + v6->Pos.z;
    a1a[7].x = a1a[7].x + v6->Pos.x;
    a1a[7].y = a1a[7].y + v6->Pos.y;
    a1a[7].z = v5 + v6->Pos.z;
    if (BVMask_gi_Num > 0)
    {
        for (i = &BVMask_ga_Masks[0]; v3 != i->gao; i++)
        {
            v9 = i->is15;
            v10 = i->pool[1].x - i->pool[0].x;
            v11 = i->is14;
            v12 = i->pool[1].y - i->pool[0].y;
            v13 = 0;
            v14 = i->pool[1].z - i->pool[0].z;
            v60 = _floatIsNegative(v9);             //LODWORD(v60) = LODWORD(v9) & 0xBF800000 | 0x3F800000;
            v67 = i->pool[2].x - i->pool[1].x;
            v15 = i->pool[2].y;
            v66 = v9;
            v16 = &a1a[0];
            v68 = (float)(v15 - i->pool[1].y);
            v17 = i->pool[2].z - i->pool[1].z;
            v61 = (float)(v17 * v12 - v68 * v14);
            v62 = (float)(v67 * v14 - v17 * v10);
            v63 = (float)(v68 * v10 - v12 * v67);
            do
            {
                v18 = i->pool[0].x - v16->x;
                if (v11)
                {
                    v19 = 0.0;
                }
                else
                {
                    v18 = 0.0;
                    v52 = i->pool[0].z - v16->z;
                    v19 = v52;
                }
                v44 = i->pool[0].y - v16->y;
                v36 = (float)(v62 * v44 + v63 * v19 + v61 * v18);
                if (v60 == _floatIsNegative(v36)) /*if (v60 == COERCE_FLOAT(LODWORD(v36) & 0xBF800000 | 0x3F800000))*/
                    break;
                v20 = i->pool[0].x - v16[1].x;
                if (v11)
                {
                    v21 = 0.0;
                }
                else
                {
                    v20 = 0.0;
                    v53 = i->pool[0].z - v16[1].z;
                    v21 = v53;
                }
                v45 = i->pool[0].y - v16[1].y;
                v37 = (float)(v62 * v45 + v63 * v21 + v61 * v20);
                if (v60 == _floatIsNegative(v37)) //if (v60 == COERCE_FLOAT(LODWORD(v37) & 0xBF800000 | 0x3F800000))
                {
                    ++v13;
                    break;
                }
                v22 = i->pool[0].x - v16[2].x;
                if (v11)
                {
                    v23 = 0.0;
                }
                else
                {
                    v22 = 0.0;
                    v54 = i->pool[0].z - v16[2].z;
                    v23 = v54;
                }
                v46 = i->pool[0].y - v16[2].y;
                v38 = (float)(v62 * v46 + v63 * v23 + v61 * v22);
                if (v60 == _floatIsNegative(v38)) //if (v60 == COERCE_FLOAT(LODWORD(v38) & 0xBF800000 | 0x3F800000))
                {
                    v13 += 2;
                    break;
                }
                v24 = i->pool[0].x - v16[3].x;
                if (v11)
                {
                    v25 = 0.0;
                }
                else
                {
                    v24 = 0.0;
                    v55 = i->pool[0].z - v16[3].z;
                    v25 = v55;
                }
                v47 = i->pool[0].y - v16[3].y;
                v39 = (float)(v62 * v47 + v63 * v25 + v61 * v24);
                if (v60 == _floatIsNegative(v39)) //if (v60 == COERCE_FLOAT(LODWORD(v39) & 0xBF800000 | 0x3F800000))
                {
                    v13 += 3;
                    break;
                }
                v26 = i->pool[0].x - v16[4].x;
                if (v11)
                {
                    v27 = 0.0;
                }
                else
                {
                    v26 = 0.0;
                    v56 = i->pool[0].z - v16[4].z;
                    v27 = v56;
                }
                v48 = i->pool[0].y - v16[4].y;
                v40 = (float)(v62 * v48 + v63 * v27 + v61 * v26);
                if (v60 == _floatIsNegative(v40)) //if (v60 == COERCE_FLOAT(LODWORD(v40) & 0xBF800000 | 0x3F800000))
                {
                    v13 += 4;
                    break;
                }
                v28 = i->pool[0].x - v16[5].x;
                if (v11)
                {
                    v29 = 0.0;
                }
                else
                {
                    v28 = 0.0;
                    v57 = i->pool[0].z - v16[5].z;
                    v29 = v57;
                }
                v49 = i->pool[0].y - v16[5].y;
                v41 = (float)(v62 * v49 + v63 * v29 + v61 * v28);
                if (v60 == _floatIsNegative(v41)) //if (v60 == COERCE_FLOAT(LODWORD(v41) & 0xBF800000 | 0x3F800000))
                {
                    v13 += 5;
                    break;
                }
                v30 = i->pool[0].x - v16[6].x;
                if (v11)
                {
                    v31 = 0.0;
                }
                else
                {
                    v30 = 0.0;
                    v58 = i->pool[0].z - v16[6].z;
                    v31 = v58;
                }
                v50 = i->pool[0].y - v16[6].y;
                v42 = (float)(v62 * v50 + v63 * v31 + v61 * v30);
                if (v60 == _floatIsNegative(v42)) //if (v60 == COERCE_FLOAT(LODWORD(v42) & 0xBF800000 | 0x3F800000))
                {
                    v13 += 6;
                    break;
                }
                v32 = i->pool[0].x - v16[7].x;
                if (v11)
                {
                    v33 = 0.0;
                }
                else
                {
                    v32 = 0.0;
                    v59 = i->pool[0].z - v16[7].z;
                    v33 = v59;
                }
                v51 = i->pool[0].y - v16[7].y;
                v43 = (float)(v62 * v51 + v63 * v33 + v61 * v32);
                if (v60 == _floatIsNegative(v43)) //if (v60 == COERCE_FLOAT(LODWORD(v43) & 0xBF800000 | 0x3F800000))
                {
                    v13 += 7;
                    break;
                }
                v13 += 8;
                v16 += 2;
            } while (v13 < 8);
            if (v13 == 8)
            {
                v34 = 0;
                v35 = a1a;
                do
                {
                    if (!OBJ_CullingPointInside(
                        v35,
                        &(i->planeLeft),
                        &(i->planeUp),
                        &(i->planeRight),
                        &(i->planeDown),
                        (i->flags)))
                        break;
                    ++v34;
                    ++v35;
                } while (v34 < 8);
                if (v34 == 8)
                    return 1;
                v1 = BVMask_gi_Num;
                v3 = a1;
            }
            if (++v65 >= v1)
                return 0;
        }
    }
    return 0;
}

int WOR_BVMaskPreCompute(tGameObjectMain* a1, tDDCamStruct* a2, int a3)
{
    tBVMasks* v4;
    tGameObjectBV* v5;
    long double v6;
    long double v7;
    float v8;
    float v9;
    Mtx4x4* v10;
    Mtx4x4* v13;
    Mtx4x4* v15;
    Mtx4x4* v17;
    double v18;
    int v20;
    Vec3* v21;
    int v22;
    Vec3* v23;
    double v24;
    Vec3* v25;
    double v26;
    long double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    long double v32;
    double v33;
    double v34;
    double v35;
    long double v36;
    double v37;
    double v38;
    double v39;
    double v40;
    double v41;
    double v42;
    double v43;
    double v44;
    int v45;
    Vec3* v46;
    Vec3* v47;
    int v48;
    Vec3* v49;
    double v50;
    int result;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    float v60;
    float v61;
    float v62;
    float v63;
    float v64;
    float v65;
    float v66;
    float v67;
    int i;
    float v69;
    float v70;

    v4 = &BVMask_ga_Masks[BVMask_gi_Num];
    v4->gao = a1;
    v4->flags = 15;
    v4->pool[0] = a1->boundingVolume->BVMax;
    v5 = a1->boundingVolume;
    v4->is11 = v5->BVMin.x;
    v4->is12 = v5->BVMin.y;
    v4->is13 = v5->BVMin.z;
    v6 = fabs(v4->pool[0].x - v4->is11);
    v7 = fabs(v4->pool[0].y - v4->is12);
    v64 = (float)fabs(v4->pool[0].z - v4->is13);
    if (v7 >= v6 || v7 >= v64)
    {
        if (v6 >= v7 || v6 >= v64)
        {
            v4->pool[1].x = v4->is11;
            v4->pool[1].y = v4->pool[0].y;
            v4->pool[1].z = v4->pool[0].z;
            v4->pool[2].x = v4->is11;
            v4->pool[2].y = v4->is12;
            v4->pool[2].z = v4->pool[0].z;
            v4->pool[3].x = v4->pool[0].x;
            v4->pool[3].y = v4->is12;
            v9 = v4->pool[0].z;
            v4->is14 = 0;
            goto LABEL_9;
        }
        v4->pool[1].x = v4->pool[0].x;
        v4->pool[1].y = v4->is12;
        v4->pool[1].z = v4->pool[0].z;
        v4->pool[2].x = v4->pool[0].x;
        v8 = v4->is12;
    }
    else
    {
        v4->pool[1].x = v4->is11;
        v4->pool[1].y = v4->pool[0].y;
        v4->pool[1].z = v4->pool[0].z;
        v4->pool[2].x = v4->is11;
        v8 = v4->pool[0].y;
    }
    v4->pool[2].y = v8;
    v4->pool[2].z = v4->is13;
    v4->pool[3].x = v4->pool[0].x;
    v4->pool[3].y = v4->pool[0].y;
    v9 = v4->is13;
    v4->is14 = 1;
LABEL_9:
    v4->pool[3].z = v9;
    v10 = a1->matrices;
    v4->pool[0].x = v10->Pos.x + v4->pool[0].x;
    v4->pool[0].y = v10->Pos.y + v4->pool[0].y;
    v4->pool[0].z = v10->Pos.z + v4->pool[0].z;
    v13 = a1->matrices;
    v4->pool[1].x = v13->Pos.x + v4->pool[1].x;
    v4->pool[1].y = v13->Pos.y + v4->pool[1].y;
    v4->pool[1].z = v13->Pos.z + v4->pool[1].z;
    v15 = a1->matrices;
    v4->pool[2].x = v15->Pos.x + v4->pool[2].x;
    v4->pool[2].y = v15->Pos.y + v4->pool[2].y;
    v4->pool[2].z = v15->Pos.z + v4->pool[2].z;
    v17 = a1->matrices;
    v18 = v17->Pos.x;
    v4->pool[3].x = v17->Pos.x + v4->pool[3].x;
    v4->pool[3].y = v17->Pos.y + v4->pool[3].y;
    v20 = 0;
    v4->pool[3].z = v17->Pos.z + v4->pool[3].z;
    do
    {
        switch (v20)
        {
        case 0:
            v21 = &v4->planeUp;
            break;
        case 1:
            v21 = &v4->planeRight;
            break;
        case 2:
            v21 = &v4->planeDown;
            break;
        case 3:
            v21 = &v4->planeLeft;
            break;
        default:
            v21 = 0;
            break;
        }
        v22 = v20 + 1;
        v23 = &v4->pool[(v20 + 1) % 4];
        v24 = v23->x - a2->objectMtx.Pos.x;
        v25 = &v4->pool[v20];
        v26 = v23->y - a2->objectMtx.Pos.y;
        v55 = v23->z - a2->objectMtx.Pos.z;
        v27 = 1.0 / sqrt(v26 * v26 + v24 * v24 + v55 * v55);
        v65 = (float)v27;
        v53 = (float)(v27 * v24);
        v28 = v26 * v65;
        v29 = v65 * v55;
        v30 = v25->x - a2->objectMtx.Pos.x;
        v31 = v25->y - a2->objectMtx.Pos.y;
        v61 = v25->z - a2->objectMtx.Pos.z;
        v32 = 1.0 / sqrt(v30 * v30 + v61 * v61 + v31 * v31);
        v66 = (float)v32;
        v57 = (float)(v32 * v30);
        v59 = (float)(v31 * v66);
        v33 = v61 * v66;
        v20 = v22;
        v67 = (float)(v33 * v28 - v59 * v29);
        v34 = v29 * v57 - v33 * v53;
        v35 = v59 * v53 - v28 * v57;
        v69 = (float)v35;
        v36 = 1.0 / sqrt(v35 * v69 + v34 * v34 + v67 * v67);
        v52 = (float)v36;
        v21->x = (float)(v36 * v67);
        v21->y = (float)(v52 * v34);
        v21->z = v69 * v52;
    } while (v22 < 4);
    v37 = v4->pool[1].x - v4->pool[0].x;
    v38 = v4->pool[1].y - v4->pool[0].y;
    v39 = v4->pool[1].z - v4->pool[0].z;
    v58 = v4->pool[2].x - v4->pool[1].x;
    v60 = v4->pool[2].y - v4->pool[1].y;
    v40 = v4->pool[2].z - v4->pool[1].z;
    v62 = (float)(v40 * v38 - v60 * v39);
    v63 = (float)(v58 * v39 - v40 * v37);
    v41 = v37 * v60 - v38 * v58;
    v42 = v4->pool[0].x - a2->objectMtx.Pos.x;
    if (v4->is14)
    {
        v43 = 0.0;
    }
    else
    {
        v42 = 0.0;
        v56 = v4->pool[0].z - a2->objectMtx.Pos.z;
        v43 = v56;
    }
    v54 = v4->pool[0].y - a2->objectMtx.Pos.y;
    v44 = v41 * v43 + v63 * v54 + v62 * v42;
    v4->is15 = (float)v44;
    if (v44 < 0.0)
    {
        v4->planeLeft.x =- v4->planeLeft.x;
        v4->planeLeft.y = -v4->planeLeft.y;
        v4->planeLeft.z = -v4->planeLeft.z;
        v4->planeUp.x = -v4->planeUp.x;
        v4->planeUp.y = -v4->planeUp.y;
        v4->planeUp.z = -v4->planeUp.z;
        v4->planeRight.x = -v4->planeRight.x;
        v4->planeRight.y = -v4->planeRight.y;
        v4->planeRight.z = -v4->planeRight.z;
        v4->planeDown.x = -v4->planeDown.x;
        v4->planeDown.y = -v4->planeDown.y;
        v4->planeDown.z = -v4->planeDown.z;
    }
    if (a3)
    {
        v4->planeLeft.x = -v4->planeLeft.x;
        v4->planeLeft.y = -v4->planeLeft.y;
        v4->planeLeft.z = -v4->planeLeft.z;
        v4->planeUp.x = -v4->planeUp.x;
        v4->planeUp.y = -v4->planeUp.y;
        v4->planeUp.z = -v4->planeUp.z;
        v4->planeRight.x = -v4->planeRight.x;
        v4->planeRight.y = -v4->planeRight.y;
        v4->planeRight.z = -v4->planeRight.z;
        v4->planeDown.x = -v4->planeDown.x;
        v4->planeDown.y = -v4->planeDown.y;
        v4->planeDown.z = -v4->planeDown.z;
    }
    v45 = 0;
    for (i = 0; ; v45 = i)
    {
        switch (v45)
        {
        case 0:
            v46 = &v4->planeLeft;
            break;
        case 1:
            v46 = &v4->planeUp;
            break;
        case 2:
            v46 = &v4->planeRight;
            break;
        case 3:
            v46 = &v4->planeDown;
            break;
        default:
            v46 = 0;
            break;
        }
        v47 = &g_stGlobalNormPlaneLeft;
        v48 = 1;
        v70 = g_stGlobalNormPlaneLeft.y * v46->y + g_stGlobalNormPlaneLeft.x * v46->x + g_stGlobalNormPlaneLeft.z * v46->z;
        do
        {
            switch (v48)
            {
            case 1:
                v49 = &g_stGlobalNormPlaneUp;
                break;
            case 2:
                v49 = &g_stGlobalNormPlaneRight;
                break;
            case 3:
                v49 = &g_stGlobalNormPlaneDown;
                break;
            default:
                v49 = 0;
                break;
            }
            v50 = (float)(v49->y * v46->y + v49->z * v46->z + v46->x * v49->x);
            if (v50 < v70)
            {
                v70 = (float)v50;
                v47 = v49;
            }
            ++v48;
        } while (v48 < 4);
        if ((a2->objectMtx.v3.z * v47->z + a2->objectMtx.v3.y * v47->y + v47->x * a2->objectMtx.v3.x) < 
            ((-v46->x) * a2->objectMtx.v3.x + (-v46->z) * a2->objectMtx.v3.z + (-v46->y)  * a2->objectMtx.v3.y))
            v4->flags &= ~(1 << i);
        result = ++i;
        if (i >= 4)
            break;
    }
    return result;
}

void WOR_BVMaskPreComputeAll(tWorld* a1, tDDContainerStruct* a2)
{
    tableElemStructPF* v2;
    tableElemStructPF* i;
    tGameObjectMain* v4;

    BVMask_gi_Num = 0;
    if (ENG_gb_ActiveSectorization)
    {
        v2 = a1->objectsWithBVMaskTablePF.firstElemPtr;
        for (i = ((tableElemStructPF*)(a1->objectsWithBVMaskTablePF.nextElemPtr)) - 1; v2 <= i; ++v2)
        {
            v4 = (tGameObjectMain*)v2->objectPtr;
            if (v2->objectPtr && ((uint8_t)v4 & 3) != 1 && !OBJ_CullingObject(v4))
            {
                WOR_BVMaskPreCompute(v4, &a2->cam, 0);
                ++BVMask_gi_Num;
            }
        }
    }
}

void ENG_InitOneWorld(tWorld* a1)
{
    tViewElem* v1;
    tWorldGridStruct* v2;

    ENG_gb_FirstFrame = 1;
    v1 = a1->viewPtr;
    a1->recordCount = 0;
    a1->colCrossObjectNumber = 0;
    if (v1)
        v1->assignedGao = 0;
    WAY_ComputePoundNetworks(a1);
    if (a1->grid1Ptr)
        GRI_Reinit(a1->grid1Ptr);
    v2 = a1->grid2Ptr;
    if (v2)
        GRI_Reinit(v2);
    AI_gf_Epsilon = 0.000099999997f;
}

struct sWorld* WOR_pst_Universe_AddWorld(int worldKey, int unused1, int resolveAI)
{
    tWorld* result;
    tWorld* v4;

    result = WOR_pst_World_Load(NULL, worldKey, resolveAI);
    v4 = result;
    if (result)
    {
        ENG_InitOneWorld(result);
        if (resolveAI)
        {
            WOR_World_ActivateObjects(v4);
            EOT_SetOfEOT_Build(&v4->eot, &v4->activatedObjectsTable4PF);
        }
        if (TAB_ul_PFtable_GetElemIndexWithPointer(&WOR_gst_Universe.univWorldTablePF, v4) == -1)
            TAB_PFTable_AddElemToTable(&WOR_gst_Universe.univWorldTablePF, v4, 0);
        result = v4;
    }
    return result;
}

void WOR_World_MakeObjectsVisible(struct sWorld* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;
    tGameObjectMain* v3;
    int v4;

    if (!PRO_Visibility.insideCounter)
        PRO_Visibility.startTick = TIM_GetTicksAsInt32();
    ++PRO_Visibility.insideCounter;
    TAB_PFtable_Clear(&a1->visibleGameObjectsPF);
    TAB_PFtable_Clear(&a1->lightObjectsTablePF);
    TAB_PFtable_Clear(&a1->objectsWithBVMaskTablePF);
    v1 = a1->objectTablePF.firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->objectTablePF.nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        v3 = (tGameObjectMain*)v1->objectPtr;
        if (v1->objectPtr && ((uint8_t)v3 & 3) != 1)
        {
            v4 = v3->flags2;
            if ((v4 & 2) != 0)
                v3->flags2 = v4 & ~2u;
            if ((v3->flags & 1) == 0)
                WOR_MakeObjectVisibleIfFlagsAllow(v3, a1);
        }
    }
    if (PRO_Visibility.insideCounter)
    {
        if (!--PRO_Visibility.insideCounter)
            PRO_Visibility.sumTime += TIM_GetTicksAsInt32() - PRO_Visibility.startTick;
    }
}

void WOR_SetCam(tViewElem* a2)
{
    CAM_SetCameraMatrix(&gpst_DX_stDD->ct, &a2->cameraMtx);
    gpst_DX_stDD->ct.cam.focale = a2->focale;
    gpst_DX_stDD->ct.cam.viewPortPosX = a2->viewPortPosX;
    gpst_DX_stDD->ct.cam.viewPortPosY = a2->viewPortPosY;
    gpst_DX_stDD->ct.cam.viewPortSizeWidth = a2->viewPortSizeWidth;
    gpst_DX_stDD->ct.cam.viewPortSizeHeight = a2->viewPortSizeHeight;
    gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    CAM_Inverse(&gpst_DX_stDD->ct);
    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
}

void zero_out_param1_values_statistics_sub_43EBF0(profilerRasterStructElem* a1)
{
    a1[3].unk2 = 0;
    a1[4].unk2 = 0;
    a1[5].unk2 = 0;
    a1[6].unk2 = 0;
    a1[7].unk2 = 0;
    a1[8].unk2 = 0;
    a1[9].unk2 = 0;
    a1[10].unk2 = 0;
    a1[11].unk2 = 0;
    a1[12].unk2 = 0;
    a1[13].unk2 = 0;
    a1[14].unk2 = 0;
}

void WOR_Render_All_GO(struct sWorld* a1, struct sGdi_DisplayData* dxdd)
{
    tWorld* v2;
    tableElemStructPF* v3;
    tGameObjectMain* v4;
    int v5;
    int v6;
    Mtx4x4* v7;
    tViewElem* v8;
    double v9;
    Vec3* v10;
    tGameObjectExtended* v11;
    double v12;
    char v13;
    double v14;
    BOOL v15;
    int v16;
    tableElemStructPF* v17;
    tGameObjectMain* v18;
    tableElemStructPF* v19;
    tGameObjectBasic* v20; 
    tGdi_DisplayData* v21; 
    tGameObjectBasic* v22; 
    unsigned int v23; 
    float v24;
    LARGE_INTEGER PerformanceCount;
    tableElemStructPF* a1a;


    QueryPerformanceCounter(&PerfCounterStruct[41]);
    v2 = a1;
    if ((dxdd->flags & 0x80000) == 0)
    {
        v3 = a1->visibleGameObjectsPF.firstElemPtr;
        for (a1a = &((tableElemStructPF*)(a1->visibleGameObjectsPF.nextElemPtr))[-1]; v3 <= a1a; ++v3)
        {
            v4 = (tGameObjectMain*)v3->objectPtr;
            if (!v3->objectPtr || ((uint8_t)v4 & 3) == 1)
                continue;
            v5 = v4->flags2 & ~0x20000u | 4;
            v4->flags2 = v5;
            if (ENG_gb_ActiveSectorization && v4->if_this_not0_wont_render_gao)
            {
                v6 = v5 | 0x20000;
                v4->distance = 0;
            LABEL_25:
                v4->flags2 = v6;
                continue;
            }
            v7 = v4->matrices;
            v8 = v2->viewPtr;
            v9 = v7->Pos.x - v8->cameraMtx.Pos.x;
            v10 = &v7->Pos;
            v11 = v4->extended;
            v13 = 0;
            if (!v11
                || v11->renderMinReqDistSq < 1.0
                || v11->renderMinReqDistSq == - 1.0f
                || (v14 = v10->z - v8->cameraMtx.Pos.z,
                    v12 = v10->y - v8->cameraMtx.Pos.y,
                    v24 = (float)(v14 * v14 + v12 * v12 + v9 * v9),
                    v11->renderMinReqDistSq >= (double)v24))
            {
                v16 = v2->distanceToHide;
                if (v16 != -1 && v4->distance < v16)
                {
                    v4->distance = OBJ_uc_CalcLODVis(v4, v8);
                    continue;
                }
            }
            else
            {
                v13 = 1;
            }
            v15 = OBJ_CullingObject(v4);
            if ((v4->flags2 & 0x10000000) != 0 || !v13 && !v15 && !WOR_BVMaskComputeInside(v4))
            {
                v4->distance = OBJ_uc_CalcLODVis(v4, a1->viewPtr);
                if ((v4->flags & 0x400000) != 0 && (v4->flags & 8) == 0)
                    OBJ_ComputeGlobalWhenHie(v4);
                v2 = a1;
                v6 = v4->flags2 & ~4u;
                goto LABEL_25;
            }
            v2 = a1;
            v4->distance = 0;
        }
    }
    QueryPerformanceCounter(&PerformanceCount);
    if (PerfCounterStruct[41].QuadPart)
        PerfCountertimeSumArray[41] += PerformanceCount.QuadPart - PerfCounterStruct[41].QuadPart;
    PerfCounterStruct[41].QuadPart = 0i64;
    v17 = v2->visibleGameObjectsPF.firstElemPtr;
    if (v17 <= &((tableElemStructPF*)(v2->visibleGameObjectsPF.nextElemPtr))[-1])
    {
        while (1)
        {
            v18 = (tGameObjectMain*)v17->objectPtr;
            v19 = ((tableElemStructPF*)(v2->visibleGameObjectsPF.nextElemPtr)) - 1;
            if (v17->objectPtr && ((uint8_t)v18 & 3) != 1)
            {
                if ((v18->flags2 & 4) == 0)
                {
                    MDF_ApplyAllGao((tGameObjectMain*)v17->objectPtr);
                    if ((v18->flags & 0x400000) != 0 && (v18->flags & 8) == 0)
                        OBJ_ComputeGlobalWhenHie(v18);
                    if ((v18->flags & 0x4000) != 0)
                    {
                        v20 = v18->basic;
                        if (v20 && v20->actorObjectOrVisualObject && !((tGameObjectVisualObject*)(v20->actorObjectOrVisualObject))->renderOnlyWithGFXRender)
                        {
                            v21 = dxdd;
                            dxdd->extraMaterialToGeo = v20->extraMaterial;
                            GRO_Render(v18);
                        LABEL_50:
                            MDF_UnApplyAllGao(v18);
                            v23 = v21->presentationFlags & ~0x20100u;
                            v21->extraMaterialToGeo = 0;
                            v21->presentationFlags = v23;
                            goto LABEL_51;
                        }
                    }
                    else if ((v18->flags & 2) != 0)
                    {
                        v22 = v18->basic;
                        if (v22)
                        {
                            if (v22->actorObjectOrVisualObject)
                            {
                                dxdd->extraMaterialToGeo = v22->extraMaterial;
                                ANI_Render(v18);
                            }
                        }
                    }
                    v21 = dxdd;
                    goto LABEL_50;
                }
                if ((dxdd->flags & 0x80000) == 0)
                    AI_ExecCallback(v18, 3);
            }
        LABEL_51:
            if (++v17 > v19)
                return;
            v2 = a1;
        }
    }
}

void WOR_Universe_Close()
{
    tableElemStructPF* v0;
    tableElemStructPF* v1;
    tWorld* v2;

    v0 = WOR_gst_Universe.univWorldTablePF.firstElemPtr;
    v1 = ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.nextElemPtr)) - 1;
    if (WOR_gst_Universe.univWorldTablePF.firstElemPtr)
    {
        if (((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr)) <= v1)
        {
            do
            {
                v2 = (tWorld*)v0->objectPtr;
                if (v0->objectPtr && ((uint8_t)v2 & 3) != 1)
                    WOR_Universe_DeleteWorld(v2);
                ++v0;
            } while (v0 <= v1);
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

void WOR_Universe_Open(int a1)
{
    WOR_Universe_Close();
    if (a1 == -1)
        WOR_gst_Universe.univAIInstance = 0;
    else
        WOR_gst_Universe.univAIInstance = AI_pst_LoadInstance(a1);
}

void WOR_World_Attached(tWorld* a1)
{
    if (a1)
    {
        GFX_Attached(a1->gfxCurrentList);
        GFX_Attached(a1->gfxCurrentListSortedAfterAE);
    }
}

void WOR_Render(struct sWorld* world, struct sGdi_DisplayData* dxdd)
{
    tWorld* wworld;
    tViewElem* view;
    tViewElem* v5;
    profilerRasterStructElem* v6;
    tableElemStructPF* v7;
    tableElemStructPF* v8;
    tGameObjectMain* v9;
    tLIGHT* v10;
    int v11;
    profilerRasterStructElem* v12;
    __int16 v13;
    __int16 v14;
    tFogParams v15;
    tableElemStructPF* v16;
    tViewElem* i;
    tableElemStructPF* v18;
    int v19;
    tViewElem* ves;

    MAI_gst_MainHandles.displayData = dxdd;
    if (dxdd)
    {
        zero_out_param1_values_statistics_sub_43EBF0(dxdd->rasters);
        wworld = world;
        gpst_DX_stDD = dxdd;
        view = world->viewPtr;
        v5 = &view[world->viewNum];
        v19 = 0;
        ves = view;
        for (i = v5; view < v5; ves = view)
        {
            if ((view->flags & 1) != 0 && view->displayData == dxdd)
            {
                wworld->currViewElem = view;
                WOR_SetCam(view);
                CAM_Engine_ComputePlans(&dxdd->ct);
                OBJ_UpdateCullingVars(&dxdd->ct.cam);
                if (v19)
                    dxdd->Clear(2, 0);
                v6 = &dxdd->rasters[17];
                if (!dxdd->rasters[17].insideCounter)
                    v6->startTick = TIM_GetTicksAsInt32();
                ++v6->insideCounter;
                LIGHT_List_Reset(&dxdd->lightList);
                v8 = ((tableElemStructPF*)(wworld->lightObjectsTablePF.nextElemPtr)) - 1;
                v18 = wworld->lightObjectsTablePF.firstElemPtr;
                v7 = v18;
                v16 = v8;
                if (v18 <= v8)
                {
                    do
                    {
                        v9 = (tGameObjectMain*)(v7->objectPtr);
                        if (v7->objectPtr)
                        {
                            if (((uint8_t)v9 & 3) != 1)
                            {
                                v10 = (tLIGHT*)(v9->extended->light);
                                if (v10)
                                {
                                    v11 = v10->flags;
                                    if ((v11 & 8) != 0)
                                    {
                                        if ((v11 & 7) == 3)
                                        {
                                            if ((dxdd->fog.flags & 4) == 0)
                                            {
                                                v15.fogVertexMode = 0;
                                                v15.fogStart = v10->fnear;
                                                v15.fogEnd = v10->ffar;
                                                v15.fogColor = v10->color;
                                                v15.density = v10->density;
                                                v15.flags = 1;
                                                dxdd->Request(1, (void*)&v15);// fogparams
                                            }
                                        }
                                        else if ((!v9->if_this_not0_wont_render_gao || !ENG_gb_ActiveSectorization)
                                            && (v11 & 0x40000000) == 0)
                                        {
                                            LIGHT_List_AddLight(&dxdd->lightList, v9);
                                        }
                                    }
                                }
                            }
                        }
                        v7 = ++v18;
                    } while (v18 <= v16);
                }
                v12 = &dxdd->rasters[17];
                v13 = dxdd->rasters[17].insideCounter;
                if (v13)
                {
                    v14 = v13 - 1;
                    dxdd->rasters[17].insideCounter = v14;
                    if (!v14)
                        v12->sumTime += TIM_GetTicksAsInt32() - v12->startTick;
                }
                dxdd->rasters[4].unk2 = (float) dxdd->lightList.actualLightNum; // FIXME: ennek mi a típusa? float?
                WOR_SetCam(ves);
                D3D_PrepareWaterRendering();
                SDW_UpdatePositions();
                PROTEX_BeforeDraw();
                WOR_BVMaskPreComputeAll(wworld, &dxdd->ct);
                WOR_Render_All_GO(wworld, dxdd);
                GFX_Render(&wworld->gfxCurrentList, 0);
                SOFT_ZList_Send();
                GAO_ModifierLazy_UnapplyAll();
                D3D_PC_AE(wworld);
                AI_EvalFunc_VIEWSnapshot_DoIt();
                if ((dxdd->presentationFlags & 0x4000000) == 0 && wworld->gfxCurrentListSortedAfterAE)
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
                    wworld = world;
                }
                v5 = i;
                view = ves;
            }
            ++view;
            ++v19;
        }
        WOR_SetCam(wworld->viewPtr);
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

tWorld* WOR_pst_Universe_MergeWorld(tWorld* wow, int worldKey, int unused1, int a4)
{
    tWorld* result;

    if (!wow)
        return WOR_pst_Universe_AddWorld(worldKey, unused1, a4);
    result = WOR_pst_World_Load(wow, worldKey, a4);
    if (result)
    {
        if (a4 && WOR_gb_HasAddedMaterial)
        {
            if (wow->viewPtr)
                GDI_l_AttachWorld(wow->viewPtr->displayData, wow);
            WOR_gb_HasAddedMaterial = FALSE;
        }
        ENG_InitOneWorld(wow);
        if (a4)
        {
            WOR_World_ActivateObjects(wow);
            EOT_SetOfEOT_Build(&wow->eot, &wow->activatedObjectsTable4PF);
        }
        result = wow;
    }
    return result;
}

struct sWorld* WOR_pst_Universe_AddWorldList(int a1, int worldKey, int unused1)
{
    tableElemStructPF* curWorld;
    tWorld* world;
    int bigFileOffset;
    unsigned int v8;
    unsigned int v9;
    unsigned int v10;
    int v11;
    int v12;
    unsigned int v13;
    int v14;
    unsigned int j;
    int v16;
    tWorld* mergedWorld;
    unsigned int preloadTexRet;
    char* a1a;
    int size;
    BASKeyValStruct worldKeyOffsetPairs[400];
    uint32_t binKey;

    memset(worldKeyOffsetPairs, 0, sizeof(worldKeyOffsetPairs)); // FIXME: a játék ezt nem csinálja, supresses a compiler warning

    WOR_gul_WorldKey = worldKey;
    WTR_BeginLoadWorldHook();
    WOR_gb_FirstTrameAfterLoad = TRUE;
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

    a1a = BIG_pc_ReadFileTmp(bigFileOffset, &size);
    v8 = size;
    v9 = 0;
    if ((size & 0xFFFFFFF8) != 0)
    {
        do
        {
            worldKeyOffsetPairs[v9].key = LOA_ReadLong(&a1a, 0, 1);
            worldKeyOffsetPairs[v9].val = LOA_ReadLong(&a1a, 0, 1);
            v8 = size;
            ++v9;
        } while (v9 < (unsigned int)size >> 3);
    }
    v10 = v8 >> 3;
    v11 = -1;
    v12 = -1;
    v13 = 0;
    for (preloadTexRet = v10; v13 < v10; ++v13)
    {
        v14 = worldKeyOffsetPairs[v13].key;
        if (v14 && v14 != -1)
        {
            if (v12 == -1)
                v12 = v13;
            v11 = v13;
        }
    }
    for (j = 0; j < v10; ++j)
    {
        v16 = worldKeyOffsetPairs[j].key;
        if (v16 && v16 != -1)
        {
            WOR_gb_RealAllocWorld = j == v12;
            WOR_gb_FinalWorld = j == v11;
            mergedWorld = WOR_pst_Universe_MergeWorld(world, v16, unused1, 0);
            v10 = preloadTexRet;
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
