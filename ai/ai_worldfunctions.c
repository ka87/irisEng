#include "../memory.h"
#include "../ai.h"
#include "../world.h"
#include "../engine.h"
#include "../textures.h"
#include "ai_worldfunctions.h"

int AI_EvalFunc_WORLoaded_C()
{
    return 1;
}

tAINode* AI_EvalFunc_WORLoaded(tAINode* a1)
{
    AI_PutIntToStack(1);
    return a1 + 1;
}

// FIXME: decompile, de van a függvény elején egy jump ami a végére ugrik, szóval mégis jó a kimenet
void AI_EvalFunc_WORPreLoadTex_C()
{
}

int AI_EvalFunc_WORUniverseAdd_C(int worldKey)
{
    tWorld* world;

    world = WOR_pst_Universe_AddWorld(worldKey, 0, 1);
    return WOR_l_Universe_GetWorldIndex(world);
}

tAINode* AI_EvalFunc_WORUniverseAdd(tAINode* a1)
{
    int worldKey;
    int worldIndex;

    worldKey = AI_GetIntFromStack();
    worldIndex = AI_EvalFunc_WORUniverseAdd_C(worldKey);
    AI_PutIntToStack(worldIndex);
    return a1 + 1;
}

void AI_EvalFunc_WORActive_C(int a1)
{
    ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectFlags |= 1u;
    MAI_gst_MainHandles.currentWorld = WOR_pst_Universe_GetActiveWorld(a1);
}

tAINode* AI_EvalFunc_WORActive(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORActive_C(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_WORDesactive(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    ((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[v2].objectFlags &= ~1u;
    return a1 + 1;
}

tGameObjectMain* AI_EvalFunc_WORGetObject_C(int a1, int a2)
{
    return (tGameObjectMain*)((tableElemStructPF*)(((tWorld*)(((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr))->objectTablePF.firstElemPtr))[a2].objectPtr;
}

tAINode* AI_EvalFunc_WORGetObject(tAINode* a1)
{
    int v1;
    tGameObjectMain* v2;
    int v4;

    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_WORGetObject_C(v1, v4);
    AI_PutIntToStack((int)v2);
    return a1 + 1;
}

void AI_EvalFunc_WORUniverseDestroy_C(int a1)
{
    WOR_Universe_DeleteWorld((tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr);
}

tAINode* AI_EvalFunc_WORUniverseDestroy(tAINode* a1)
{
    int v1;

    AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORUniverseDestroy_C(v1);
    return a1 + 1;
}

int AI_EvalFunc_WORLoadList_C(int worldKey)
{
    tWorld* world;

    WOR_gi_IsLoading = 1;
    world = WOR_pst_Universe_AddWorldList(0, worldKey, 0);
    WOR_l_Universe_GetWorldIndex(world);
    WOR_gi_IsLoading = 0;
    return WOR_l_Universe_GetWorldIndex(world);
}

tAINode* AI_EvalFunc_WORLoadList(tAINode* a1)
{
    int worldKey;
    int worldIndex;

    worldKey = AI_GetIntFromStack();
    worldIndex = AI_EvalFunc_WORLoadList_C(worldKey);
    AI_PutIntToStack(worldIndex);
    return a1 + 1;
}

void AI_EvalFunc_WORRefreshVisible_C(int arg0)
{
    tWorld* a1;
    int i;

    a1 = (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[arg0].objectPtr;
    for (i = 0; i < a1->viewNum; ++i)
        a1->viewPtr[i].flags |= 2u;
    WOR_World_MakeObjectsVisible(a1);
}

tAINode* AI_EvalFunc_WORRefreshVisible(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORRefreshVisible_C(v1);
    return a1 + 1;
}

int AI_EvalFunc_WORReinit_C()
{
    ENG_gb_NeedToReinit = 1;
    return 3;
}

tAINode* AI_EvalFunc_WORReinit(tAINode* param1)
{
    return (tAINode*)AI_EvalFunc_WORReinit_C();
}

void AI_EvalFunc_WORPause_C(int a1, int a2)
{
    BOOL v2;
    tableElemStructPF* v3;
    tWorld* v4;
    tableElemStructPF* v5;

    v4 = (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr;
    v5 = v4->activatedObjectsTable4PF.firstElemPtr;
    v3 = v4->activatedObjectsTable4PF.nextElemPtr;
    while (v5 < v3)
    {
        v2 = !v5->objectPtr || ((int)v5->objectPtr & 3) == 1;
        if (!v2 && (a2 & ((tGameObjectMain*)(v5->objectPtr))->flags) != 0 && (((tGameObjectMain*)(v5->objectPtr))->flags2 & 0x8000000) == 0)
            ((tGameObjectMain*)(v5->objectPtr))->flags2 |= 0x4000000u;
        ++v5;
    }
    v4->tablesManagerAndEOTNeeded = 1;
    v4->visibilityCallNeeded = 1;
    v4->SnPRefreshNeeded = TRUE;
}

tAINode* AI_EvalFunc_WORPause(tAINode* a1)
{
    int v1;
    int a2;

    a2 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORPause_C(v1, a2);
    return a1 + 1;
}

void AI_EvalFunc_WORUnPause_C(int a1, int a2)
{
    tableElemStructPF* v3;
    tWorld* v4;
    tableElemStructPF* v5;
        
    v4 = (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr;
    v5 = v4->objectTablePF.firstElemPtr;
    v3 = v4->objectTablePF.nextElemPtr;
    while (v5 < v3)
    {
        if (v5->objectPtr && ((int)v5->objectPtr & 3) != 1)
        {
            if ((a2 & ((tGameObjectMain*)(v5->objectPtr))->flags) != 0)
                ((tGameObjectMain*)(v5->objectPtr))->flags2 &= ~0x4000000u;
            if ((((tGameObjectMain*)(v5->objectPtr))->flags2 & 0x400000) == 0)
                ((tGameObjectMain*)(v5->objectPtr))->flags2 |= 1u;
        }
        ++v5;
    }
    v4->tablesManagerAndEOTNeeded = 1;
    v4->visibilityCallNeeded = 1;
    v4->SnPRefreshNeeded = TRUE;
}

tAINode* AI_EvalFunc_WORUnPause(tAINode* a1)
{
    int v1;
    int a2;

    a2 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORUnPause_C(v1, a2);
    return a1 + 1;
}

int AI_EvalFunc_WORCurIndexGet_C()
{
    int v1;
    tableElemStructPF* v2;

    v2 = WOR_gst_Universe.univWorldTablePF.firstElemPtr;
    v1 = 0;
    while ((void*)v2 < WOR_gst_Universe.univWorldTablePF.nextElemPtr && (tWorld*)(v2->objectPtr) != WOR_gpst_CurrentWorld)
    {
        ++v1;
        ++v2;
    }
    return v1;
}

tAINode* AI_EvalFunc_WORCurIndexGet(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_WORCurIndexGet_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_WORColorAmbiantSet_C(int a1, int a2)
{
    tWorld* result;

    result = (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr;
    result->ambientColor1 = a2;
}

tAINode* AI_EvalFunc_WORColorAmbiantSet(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORColorAmbiantSet_C(v1, v3);
    return a1 + 1;
}

int AI_EvalFunc_WORColorAmbiantGet_C(int a1)
{
    return ((tWorld*)(((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr))->ambientColor1;
}

tAINode* AI_EvalFunc_WORColorAmbiantGet(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_WORColorAmbiantGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_WORColorBackSet_C(int a1, int a2)
{
    tWorld* result;

    result = (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr;
    result->clearColor = a2;
}

tAINode* AI_EvalFunc_WORColorBackSet(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORColorBackSet_C(v1, v3);
    return a1 + 1;
}

int AI_EvalFunc_WORColorBackGet_C(int a1)
{
    return ((tWorld*)(((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr))->clearColor;
}

tAINode* AI_EvalFunc_WORColorBackGet(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_WORColorBackGet_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_WORRefreshVisibleOfActor_C()
{
    tWorld* a1;
    int i;

    a1 = WOR_gpst_CurrentWorld;
    for (i = 0; i < a1->viewNum; ++i)
        a1->viewPtr[i].flags |= 2u;
    WOR_World_MakeObjectsVisible(a1);
}

tAINode* AI_EvalFunc_WORRefreshVisibleOfActor(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_WORRefreshVisibleOfActor_C(v2);
    return a1 + 1;
}

int AI_EvalFunc_WORPlayAbsTime_C(float specTime, float specDT)
{
    ENG_gf_SpecialTime = specTime;
    ENG_gf_SpecialDT = specDT;
    return 1;
}

tAINode* AI_EvalFunc_WORPlayAbsTime(tAINode* param1)
{
    ENG_gf_SpecialDT = AI_GetFloatFromStack();
    ENG_gf_SpecialTime = AI_GetFloatFromStack();
    return (tAINode*)1;
}

tAINode* AI_EvalFunc_WORIsPlayAbs(tAINode* a1)
{
    if (ENG_gf_SpecialTime == 0.0f)
        AI_PutIntToStack(0);
    else
        AI_PutIntToStack(1);
    return a1 + 1;
}

void AI_EvalFunc_WORColorAmbiant2Set_C(int a1, int a2)
{
    tWorld* result;

    result = (tWorld*)((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr;
    result->ambientColor2 = a2;
}

tAINode* AI_EvalFunc_WORColorAmbiant2Set(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WORColorAmbiant2Set_C(v1, v3);
    return a1 + 1;
}

int AI_EvalFunc_WORColorAmbiant2Get_C(int a1)
{
    return ((tWorld*)(((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[a1].objectPtr))->ambientColor2;
}

tAINode* AI_EvalFunc_WORColorAmbiant2Get(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_WORColorAmbiant2Get_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_WORRegisterES_C(int a1, struct sGameObjectMain* a2)
{
    if (gi_NumES < 100)
    {
        gap_ES[gi_NumES] = a2;
        gau_ES[gi_NumES++] = a1;
    }
}

tAINode* AI_EvalFunc_WORRegisterES(tAINode* a1)
{
    int v2;
    tGameObjectMain* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2 = AI_GetIntFromStack();
    AI_EvalFunc_WORRegisterES_C(v2, a2);
    return a1 + 1;
}

int AI_EvalFunc_WORGetKey_C()
{
    return WOR_gpst_CurrentWorld->worldKey;
}

tAINode* AI_EvalFunc_WORGetKey(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_WORGetKey_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_WORCheckSetOrder_C(tWorld* a1, tWorld* a2)
{
    int v3;
    int v4;
    int v5;
    int v6;
    tableElemStructPF* i;
    tWorld* v8;
    tWorld* v9;

    if (a1 && a2 && a1 != a2)
    {
        v8 = WOR_gpst_CurrentWorld;
        v3 = -1;
        v6 = -1;
        v5 = 0;
        for (i = WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr;
            i <= &(((tableElemStructPF*)(WOR_gpst_CurrentWorld->objectTablePF.nextElemPtr))[-1]);
            ++i)
        {
            v9 = (tWorld*)(i->objectPtr);
            if (i->objectPtr && ((int)i->objectPtr & 3) != 1)
            {
                if (v9 == a1)
                {
                    if (v6 == -1)
                        return;
                    v3 = v5;
                }
                if (v9 == a2)
                    v6 = v5;
                ++v5;
            }
        }
        if (v3 != -1 && v6 != -1)
        {
            v4 = ((tableElemStructPF*)(WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr))[v3].objectFlags;
            ((tableElemStructPF*)(WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr))[v3] = ((tableElemStructPF*)(WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr))[v6];
            ((tableElemStructPF*)(v8->objectTablePF.firstElemPtr))[v6].objectPtr = (void*)a1;
            ((tableElemStructPF*)(v8->objectTablePF.firstElemPtr))[v6].objectFlags = v4;
        }
        v8->visibilityCallNeeded = 1;
    }
}

tAINode* AI_EvalFunc_WORCheckSetOrder(tAINode* a1)
{
    tWorld* a2;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tWorld**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_WORCheckSetOrder_C(*(tWorld**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2);
    return a1 + 1;
}