#include "../memory.h"
#include "../ai.h"
#include "../world.h"
#include "../engine.h"
#include "../textures.h"
#include "ai_worldfunctions.h"

// Scripts name a world by its index in the universe table.
static tWorld* AI_l_UniverseWorld(int worldIndex)
{
    tableElemStructPF* table;

    table = (tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr);
    return (tWorld*)table[worldIndex].objectPtr;
}

static tableElemStructPF* AI_l_UniverseEntry(int worldIndex)
{
    return &((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[worldIndex];
}

// A table entry is in use when it has a pointer and its low two bits
// do not carry the free tag 01.
static BOOL AI_l_EntryInUse(tableElemStructPF* entry)
{
    return entry->objectPtr && ((uintptr_t)entry->objectPtr & 3) != 1;
}

static void AI_l_MakeWorldObjectsVisible(tWorld* world)
{
    int i;

    for (i = 0; i < world->viewNum; ++i)
        world->viewPtr[i].flags |= 2u;
    WOR_World_MakeObjectsVisible(world);
}

int AI_EvalFunc_WORLoaded_C()
{
    return 1;
}

tAINode* AI_EvalFunc_WORLoaded(tAINode* node)
{
    AI_PutIntToStack(1);
    return node + 1;
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

tAINode* AI_EvalFunc_WORUniverseAdd(tAINode* node)
{
    int worldKey;

    worldKey = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_WORUniverseAdd_C(worldKey));
    return node + 1;
}

void AI_EvalFunc_WORActive_C(int worldIndex)
{
    AI_l_UniverseEntry(worldIndex)->objectFlags |= 1u;
    MAI_gst_MainHandles.currentWorld = WOR_pst_Universe_GetActiveWorld(worldIndex);
}

tAINode* AI_EvalFunc_WORActive(tAINode* node)
{
    int worldIndex;

    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORActive_C(worldIndex);
    return node + 1;
}

tAINode* AI_EvalFunc_WORDesactive(tAINode* node)
{
    int worldIndex;

    worldIndex = AI_GetIntFromStack();
    AI_l_UniverseEntry(worldIndex)->objectFlags &= ~1u;
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_WORGetObject_C(int worldIndex, int objectIndex)
{
    tableElemStructPF* objects;

    objects = (tableElemStructPF*)(AI_l_UniverseWorld(worldIndex)->objectTablePF.firstElemPtr);
    return (tGameObjectMain*)objects[objectIndex].objectPtr;
}

tAINode* AI_EvalFunc_WORGetObject(tAINode* node)
{
    int worldIndex;
    int objectIndex;

    objectIndex = AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_PutIntToStack((int)AI_EvalFunc_WORGetObject_C(worldIndex, objectIndex));
    return node + 1;
}

void AI_EvalFunc_WORUniverseDestroy_C(int worldIndex)
{
    WOR_Universe_DeleteWorld(AI_l_UniverseWorld(worldIndex));
}

tAINode* AI_EvalFunc_WORUniverseDestroy(tAINode* node)
{
    int worldIndex;

    // the builtin takes two arguments and uses only the second
    AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORUniverseDestroy_C(worldIndex);
    return node + 1;
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

tAINode* AI_EvalFunc_WORLoadList(tAINode* node)
{
    int worldKey;

    worldKey = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_WORLoadList_C(worldKey));
    return node + 1;
}

void AI_EvalFunc_WORRefreshVisible_C(int worldIndex)
{
    AI_l_MakeWorldObjectsVisible(AI_l_UniverseWorld(worldIndex));
}

tAINode* AI_EvalFunc_WORRefreshVisible(tAINode* node)
{
    int worldIndex;

    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORRefreshVisible_C(worldIndex);
    return node + 1;
}

int AI_EvalFunc_WORReinit_C()
{
    ENG_gb_NeedToReinit = 1;
    return 3;
}

// Three and one are interpreter control codes, not nodes: these two
// builtins end the current evaluation rather than carrying on.
tAINode* AI_EvalFunc_WORReinit(tAINode* node)
{
    return (tAINode*)AI_EvalFunc_WORReinit_C();
}

void AI_EvalFunc_WORPause_C(int worldIndex, int flagMask)
{
    tWorld* world;
    tableElemStructPF* entry;
    tableElemStructPF* end;
    tGameObjectMain* gao;

    world = AI_l_UniverseWorld(worldIndex);
    end = world->activatedObjectsTable4PF.nextElemPtr;
    for (entry = world->activatedObjectsTable4PF.firstElemPtr; entry < end; ++entry)
    {
        if (!AI_l_EntryInUse(entry))
            continue;

        gao = (tGameObjectMain*)(entry->objectPtr);
        if ((flagMask & gao->flags) != 0 && (gao->flags2 & 0x8000000) == 0)
            gao->flags2 |= 0x4000000u;
    }
    world->tablesManagerAndEOTNeeded = 1;
    world->visibilityCallNeeded = 1;
    world->SnPRefreshNeeded = TRUE;
}

tAINode* AI_EvalFunc_WORPause(tAINode* node)
{
    int worldIndex;
    int flagMask;

    flagMask = AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORPause_C(worldIndex, flagMask);
    return node + 1;
}

void AI_EvalFunc_WORUnPause_C(int worldIndex, int flagMask)
{
    tWorld* world;
    tableElemStructPF* entry;
    tableElemStructPF* end;
    tGameObjectMain* gao;

    world = AI_l_UniverseWorld(worldIndex);
    end = world->objectTablePF.nextElemPtr;
    for (entry = world->objectTablePF.firstElemPtr; entry < end; ++entry)
    {
        if (!AI_l_EntryInUse(entry))
            continue;

        gao = (tGameObjectMain*)(entry->objectPtr);
        if ((flagMask & gao->flags) != 0)
            gao->flags2 &= ~0x4000000u;
        if ((gao->flags2 & 0x400000) == 0)
            gao->flags2 |= 1u;
    }
    world->tablesManagerAndEOTNeeded = 1;
    world->visibilityCallNeeded = 1;
    world->SnPRefreshNeeded = TRUE;
}

tAINode* AI_EvalFunc_WORUnPause(tAINode* node)
{
    int worldIndex;
    int flagMask;

    flagMask = AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORUnPause_C(worldIndex, flagMask);
    return node + 1;
}

// The index of the current world, or the table's length when it is not
// in the table at all.
int AI_EvalFunc_WORCurIndexGet_C()
{
    tableElemStructPF* entry;
    int worldIndex;

    entry = WOR_gst_Universe.univWorldTablePF.firstElemPtr;
    worldIndex = 0;
    while ((void*)entry < WOR_gst_Universe.univWorldTablePF.nextElemPtr
           && (tWorld*)(entry->objectPtr) != WOR_gpst_CurrentWorld)
    {
        ++worldIndex;
        ++entry;
    }
    return worldIndex;
}

tAINode* AI_EvalFunc_WORCurIndexGet(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_WORCurIndexGet_C());
    return node + 1;
}

void AI_EvalFunc_WORColorAmbiantSet_C(int worldIndex, int color)
{
    AI_l_UniverseWorld(worldIndex)->ambientColor1 = color;
}

tAINode* AI_EvalFunc_WORColorAmbiantSet(tAINode* node)
{
    int worldIndex;
    int color;

    color = AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORColorAmbiantSet_C(worldIndex, color);
    return node + 1;
}

int AI_EvalFunc_WORColorAmbiantGet_C(int worldIndex)
{
    return AI_l_UniverseWorld(worldIndex)->ambientColor1;
}

tAINode* AI_EvalFunc_WORColorAmbiantGet(tAINode* node)
{
    int worldIndex;

    worldIndex = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_WORColorAmbiantGet_C(worldIndex));
    return node + 1;
}

void AI_EvalFunc_WORColorBackSet_C(int worldIndex, int color)
{
    AI_l_UniverseWorld(worldIndex)->clearColor = color;
}

tAINode* AI_EvalFunc_WORColorBackSet(tAINode* node)
{
    int worldIndex;
    int color;

    color = AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORColorBackSet_C(worldIndex, color);
    return node + 1;
}

int AI_EvalFunc_WORColorBackGet_C(int worldIndex)
{
    return AI_l_UniverseWorld(worldIndex)->clearColor;
}

tAINode* AI_EvalFunc_WORColorBackGet(tAINode* node)
{
    int worldIndex;

    worldIndex = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_WORColorBackGet_C(worldIndex));
    return node + 1;
}

void AI_EvalFunc_WORRefreshVisibleOfActor_C()
{
    AI_l_MakeWorldObjectsVisible(WOR_gpst_CurrentWorld);
}

tAINode* AI_EvalFunc_WORRefreshVisibleOfActor(tAINode* node)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_WORRefreshVisibleOfActor_C();
    return node + 1;
}

int AI_EvalFunc_WORPlayAbsTime_C(float specTime, float specDT)
{
    ENG_gf_SpecialTime = specTime;
    ENG_gf_SpecialDT = specDT;
    return 1;
}

tAINode* AI_EvalFunc_WORPlayAbsTime(tAINode* node)
{
    float specTime;
    float specDT;

    specDT = AI_GetFloatFromStack();
    specTime = AI_GetFloatFromStack();
    return (tAINode*)AI_EvalFunc_WORPlayAbsTime_C(specTime, specDT);
}

tAINode* AI_EvalFunc_WORIsPlayAbs(tAINode* node)
{
    AI_PutIntToStack(ENG_gf_SpecialTime != 0.0f);
    return node + 1;
}

void AI_EvalFunc_WORColorAmbiant2Set_C(int worldIndex, int color)
{
    AI_l_UniverseWorld(worldIndex)->ambientColor2 = color;
}

tAINode* AI_EvalFunc_WORColorAmbiant2Set(tAINode* node)
{
    int worldIndex;
    int color;

    color = AI_GetIntFromStack();
    worldIndex = AI_GetIntFromStack();
    AI_EvalFunc_WORColorAmbiant2Set_C(worldIndex, color);
    return node + 1;
}

int AI_EvalFunc_WORColorAmbiant2Get_C(int worldIndex)
{
    return AI_l_UniverseWorld(worldIndex)->ambientColor2;
}

tAINode* AI_EvalFunc_WORColorAmbiant2Get(tAINode* node)
{
    int worldIndex;

    worldIndex = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_WORColorAmbiant2Get_C(worldIndex));
    return node + 1;
}

void AI_EvalFunc_WORRegisterES_C(int id, struct sGameObjectMain* gao)
{
    if (gi_NumES < 100)
    {
        gap_ES[gi_NumES] = gao;
        gau_ES[gi_NumES++] = id;
    }
}

tAINode* AI_EvalFunc_WORRegisterES(tAINode* node)
{
    tGameObjectMain* gao;
    int id;

    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    id = AI_GetIntFromStack();
    AI_EvalFunc_WORRegisterES_C(id, gao);
    return node + 1;
}

int AI_EvalFunc_WORGetKey_C()
{
    return WOR_gpst_CurrentWorld->worldKey;
}

tAINode* AI_EvalFunc_WORGetKey(tAINode* node)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_WORGetKey_C());
    return node + 1;
}

// Moves first in front of second in the current world's object table,
// which is the order they are drawn in. Nothing happens when first is
// already the earlier of the two.
void AI_EvalFunc_WORCheckSetOrder_C(tWorld* first, tWorld* second)
{
    tWorld* world;
    tableElemStructPF* table;
    tableElemStructPF* entry;
    tableElemStructPF* end;
    tableElemStructPF* moved;
    int firstIndex;
    int secondIndex;
    int flags;
    int index;

    if (!first || !second || first == second)
        return;

    world = WOR_gpst_CurrentWorld;
    table = (tableElemStructPF*)(world->objectTablePF.firstElemPtr);
    end = (tableElemStructPF*)(world->objectTablePF.nextElemPtr);

    firstIndex = -1;
    secondIndex = -1;
    index = 0;
    for (entry = world->objectTablePF.firstElemPtr; entry < end; ++entry)
    {
        if (!AI_l_EntryInUse(entry))
            continue;

        if ((tWorld*)(entry->objectPtr) == first)
        {
            if (secondIndex == -1)
                return;
            firstIndex = index;
        }
        if ((tWorld*)(entry->objectPtr) == second)
            secondIndex = index;
        ++index;
    }

    if (firstIndex != -1 && secondIndex != -1)
    {
        moved = &table[secondIndex];
        flags = table[firstIndex].objectFlags;
        table[firstIndex] = *moved;
        moved->objectPtr = (void*)first;
        moved->objectFlags = flags;
    }
    world->visibilityCallNeeded = 1;
}

tAINode* AI_EvalFunc_WORCheckSetOrder(tAINode* node)
{
    tWorld* first;
    tWorld* second;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    second = (tWorld*)AI_GetObjectFromStack();
    first = (tWorld*)AI_GetObjectFromStack();
    AI_EvalFunc_WORCheckSetOrder_C(first, second);
    return node + 1;
}
