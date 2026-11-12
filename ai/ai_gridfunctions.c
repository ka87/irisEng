#include "../math.h"
#include "../grid.h"
#include "../gameobject.h"
#include "../time.h"
#include "../profiler.h"
#include "ai_gridfunctions.h"

// The world has two grids and GRID_gi_Current picks between them.
static tWorldGridStruct* AI_l_CurrentGrid(void)
{
    if (GRID_gi_Current)
        return WOR_gpst_CurrentWorld->grid2Ptr;
    return WOR_gpst_CurrentWorld->grid1Ptr;
}

// The same for the far end of a path, which a script may put in the
// other grid.
static tWorldGridStruct* AI_l_DestGrid(void)
{
    if (GRID_gi_Dest)
        return WOR_gpst_CurrentWorld->grid2Ptr;
    return WOR_gpst_CurrentWorld->grid1Ptr;
}

int AI_EvalFunc_GRIDGetCapa_C(tGameObjectMain* gao, Vec3* pos)
{
    return GRI_i_GetCapa(AI_l_CurrentGrid(), pos);
}

int AI_EvalFunc_GRIDSetWall_C(int wallMask)
{
    GRID_gi_TestWall = wallMask;
    return wallMask;
};

tAINode* AI_EvalFunc_GRIDSetWall(tAINode* node)
{
    int wallMask;

    wallMask = AI_GetIntFromStack();
    AI_EvalFunc_GRIDSetWall_C(wallMask);
    return node + 1;
}

int AI_EvalFunc_GRIDModeSet_C(int mode)
{
    GRID_gi_Mode = mode;
    return mode;
}

tAINode* AI_EvalFunc_GRIDModeSet(tAINode* node)
{
    int mode;

    mode = AI_GetIntFromStack();
    AI_EvalFunc_GRIDModeSet_C(mode);
    return node + 1;
}

int AI_EvalFunc_GRIDCurrentSet_C(int which)
{
    GRID_gi_Current = which;
    return which;
}

tAINode* AI_EvalFunc_GRIDCurrentSet(tAINode* node)
{
    int which;

    which = AI_GetIntFromStack();
    AI_EvalFunc_GRIDCurrentSet_C(which);
    return node + 1;
}

int AI_EvalFunc_GRIDDestSet_C(int which)
{
    GRID_gi_Dest = which;
    return which;
}

tAINode* AI_EvalFunc_GRIDDestSet(tAINode* node)
{
    int which;

    which = AI_GetIntFromStack();
    AI_EvalFunc_GRIDDestSet_C(which);
    return node + 1;
}

void AI_EvalFunc_GRIDSetCapa_C(struct sGameObjectMain* gao, Vec3* pos, char capability)
{
    GRI_SetCapa(AI_l_CurrentGrid(), pos, capability);
}

tAINode* AI_EvalFunc_GRIDSetCapa(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    char capability;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    capability = AI_GetIntFromStack();
    AI_GetVecFromStack(&pos);
    AI_EvalFunc_GRIDSetCapa_C(gaoUltra, &pos, capability);
    return node + 1;
}

// Moves a grid's origin, naming the grid directly rather than through
// GRID_gi_Current, which it saves and puts back.
void AI_EvalFunc_GRIDSetPosAll_C(struct sGameObjectMain* gao, int which, Vec3* origin)
{
    tWorldGridStruct* grid;
    int previous;

    previous = GRID_gi_Current;
    GRID_gi_Current = which;
    if (which)
        grid = WOR_gpst_CurrentWorld->grid2Ptr;
    else
        grid = WOR_gpst_CurrentWorld->grid1Ptr;
    grid->startX = origin->x;
    grid->startY = origin->y;
    GRID_gi_Current = previous;
}

tAINode* AI_EvalFunc_GRIDSetPosAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 origin;
    int which;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&origin);
    which = AI_GetIntFromStack();
    AI_EvalFunc_GRIDSetPosAll_C(gaoUltra, which, &origin);
    return node + 1;
}

int AI_EvalFunc_GRIDAddCapa_C(struct sGameObjectMain* gao, Vec3* pos, char capability)
{
    return GRI_AddCapa(AI_l_CurrentGrid(), pos, capability);
}

tAINode* AI_EvalFunc_GRIDAddCapa(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    char capability;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    capability = AI_GetIntFromStack();
    AI_GetVecFromStack(&pos);
    AI_EvalFunc_GRIDAddCapa_C(gaoUltra, &pos, capability);
    return node + 1;
}

tAINode* AI_EvalFunc_GRIDGetCapa(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&pos);
    AI_PutIntToStack(AI_EvalFunc_GRIDGetCapa_C(gaoUltra, &pos));
    return node + 1;
}

/* A* from the object's position to a target, timed on its own profiler
   raster. The grid the path is built in is the current one and the one
   its walkability is read from is the dest one, which the call resets
   afterwards so a script has to set it each time.

   GRID_ShortestPath ignores its seventh argument, which the original
   builds here as maxCells with its low byte replaced. */
int AI_EvalFunc_GRIDShortWay_C(struct sGameObjectMain* gao, Vec3* pathOut, Vec3* start, Vec3* target, char lowByte, int nearLimit, float maxDist, int maxCells, int maxPoints)
{
    int unused;
    int written;

    if (!i_Raster_339)
    {
        PRO_RasterAdd(&PRO_ShortWay, "Trame Loop", "Game Engine", "GRID Short Way", 0, 0.0);
        i_Raster_339 = 1;
    }
    if (!PRO_ShortWay.insideCounter)
        PRO_ShortWay.startTick = TIM_GetTicksAsInt32();
    ++PRO_ShortWay.insideCounter;

    unused = (maxCells & 0xffffff00) | (lowByte & 0xff); // unused = maxCells; LOBYTE(unused) = lowByte;
    written = GRID_ShortestPath(gao, AI_l_CurrentGrid(), AI_l_DestGrid(),
                                start, target, pathOut,
                                unused, nearLimit, maxDist, maxCells, maxPoints);

    PRO_RecordTimeDecRef(&PRO_ShortWay);
    GRID_gi_Dest = 0;
    return written;
}

tAINode* AI_EvalFunc_GRIDShortWay(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    Vec3 start;
    Vec3 target;
    char lowByte;
    int nearLimit;
    float maxDist;
    int maxCells;
    int maxPoints;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    maxPoints = AI_GetIntFromStack();
    maxCells = AI_GetIntFromStack();
    maxDist = AI_GetFloatFromStack();
    nearLimit = AI_GetIntFromStack();
    lowByte = AI_GetIntFromStack();
    AI_GetVecFromStack(&target);
    AI_GetVecFromStack(&start);
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_GRIDShortWay_C(gaoUltra, (Vec3*)var.ptr, &start, &target,
                                                lowByte, nearLimit, maxDist,
                                                maxCells, maxPoints));
    return node + 1;
}

// The centre of the cell a position is in.
Vec3* AI_EvalFunc_GRIDPosGet_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* cellCentre)
{
    tWorldGridStruct* grid;
    int cellX;
    int cellY;

    grid = AI_l_CurrentGrid();
    *cellCentre = *pos;
    GRI_3Dto2D(grid, cellCentre, &cellX, &cellY);
    cellCentre->x = (float)((float)cellX + 0.5 + grid->startX);
    cellCentre->y = (float)((float)cellY + 0.5 + grid->startY);
    cellCentre->z = 0.0;
    return cellCentre;
}

tAINode* AI_EvalFunc_GRIDPosGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    Vec3 cellCentre;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&pos);
    AI_EvalFunc_GRIDPosGet_C(gaoUltra, &pos, &cellCentre);
    AI_PutVecToStack(&cellCentre);
    return node + 1;
}

// Centres both of the world's grids on a position, not just the
// current one.
void AI_EvalFunc_GRIDSetCenter_C(struct sGameObjectMain* gao, Vec3* pos)
{
    tWorld* world;
    int cellX;
    int cellY;

    world = WOR_gpst_CurrentWorld;
    if (world->grid1Ptr)
    {
        GRI_3Dto2D(world->grid1Ptr, pos, &cellX, &cellY);
        GRID_SetRealCenter(world->grid1Ptr, cellX, cellY);
    }
    if (world->grid2Ptr)
    {
        GRI_3Dto2D(world->grid2Ptr, pos, &cellX, &cellY);
        GRID_SetRealCenter(world->grid2Ptr, cellX, cellY);
    }
}

tAINode* AI_EvalFunc_GRIDSetCenter(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&pos);
    AI_EvalFunc_GRIDSetCenter_C(gaoUltra, &pos);
    return node + 1;
}

BOOL AI_EvalFunc_GRIDIsIn_C(struct sGameObjectMain* gao, Vec3* pos)
{
    int cellX;
    int cellY;

    return GRID_3Dto2D(AI_l_CurrentGrid(), pos, &cellX, &cellY) != 0;
}

tAINode* AI_EvalFunc_GRIDIsIn(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&pos);
    AI_PutIntToStack(AI_EvalFunc_GRIDIsIn_C(gaoUltra, &pos));
    return node + 1;
}

int AI_EvalFunc_GRIDTag_C(struct sGameObjectMain* gao, Vec3* pos, int shape, int capability)
{
    return GRI_Tag(AI_l_CurrentGrid(), pos, shape, capability);
}

tAINode* AI_EvalFunc_GRIDTag(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    int shape;
    int capability;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    capability = AI_GetIntFromStack();
    shape = AI_GetIntFromStack();
    AI_GetVecFromStack(&pos);
    AI_PutIntToStack(AI_EvalFunc_GRIDTag_C(gaoUltra, &pos, shape, capability));
    return node + 1;
}

int AI_EvalFunc_GRIDTagBox_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* to, int capability)
{
    return GRI_TagBox(AI_l_CurrentGrid(), pos, to, capability);
}

tAINode* AI_EvalFunc_GRIDTagBox(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    Vec3 to;
    int capability;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    capability = AI_GetIntFromStack();
    AI_GetVecFromStack(&to);
    AI_GetVecFromStack(&pos);
    AI_PutIntToStack(AI_EvalFunc_GRIDTagBox_C(gaoUltra, &pos, &to, capability));
    return node + 1;
}

// How much of a path is still walkable, counted back from its far end.
int AI_EvalFunc_GRIDSmoothWay_C(struct sGameObjectMain* gao, Vec3* path, int num)
{
    return GRI_SmoothWay(AI_l_DestGrid(), AI_l_CurrentGrid(), path, num);
}

tAINode* AI_EvalFunc_GRIDSmoothWay(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    int num;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_GRIDSmoothWay_C(gaoUltra, (Vec3*)var.ptr, num));
    return node + 1;
}

int AI_EvalFunc_GRIDTagVec_C(struct sGameObjectMain* gao, Vec3* pos, int shape, int capability, Vec3* dir)
{
    return GRI_TagVec(AI_l_CurrentGrid(), pos, shape, capability, dir);
}

tAINode* AI_EvalFunc_GRIDTagVec(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    Vec3 dir;
    int shape;
    int capability;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&dir);
    capability = AI_GetIntFromStack();
    shape = AI_GetIntFromStack();
    AI_GetVecFromStack(&pos);
    AI_PutIntToStack(AI_EvalFunc_GRIDTagVec_C(gaoUltra, &pos, shape, capability, &dir));
    return node + 1;
}

/* Steps the object's position towards a target by at most one step, and
   reports the cell centre it lands on. A step that leaves the grid
   reports the zero vector. A zero-length move reports nothing, since
   the output is only written inside the guard. */
void AI_EvalFunc_GRIDCptDest_C(struct sGameObjectMain* gao, Vec3* target, float step, Vec3* dest)
{
    tWorldGridStruct* grid;
    float distance;

    grid = AI_l_CurrentGrid();
    MATH_SubstractVec(dest, target, &gao->matrices->Pos);
    distance = MATH_VecLen(dest);
    if (distance == 0.0)
        return;

    if (distance < (double)step)
        step = distance;
    if (step == 0.0)
        step = 1.0;

    MATH_NormalizeVector(dest, dest);
    MATH_VecMultScalarWithoutBuffer(dest, step);
    MATH_VecAddVec(dest, dest, &gao->matrices->Pos);
    if (!GRI_i_ComputeDest(grid, dest))
    {
        dest->x = 0.0;
        dest->y = 0.0;
        dest->z = 0.0;
    }
}

tAINode* AI_EvalFunc_GRIDCptDest(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 target;
    Vec3 dest;
    float step;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    step = AI_GetFloatFromStack();
    AI_GetVecFromStack(&target);
    AI_EvalFunc_GRIDCptDest_C(gaoUltra, &target, step, &dest);
    AI_PutVecToStack(&dest);
    return node + 1;
}

int AI_EvalFunc_GRIDTagDyn_C(struct sGameObjectMain* gao, Vec3* pos, Vec3* cells, float distLimit, char capability)
{
    return GRI_TagDyn(AI_l_CurrentGrid(), pos, distLimit, cells, capability);
}

tAINode* AI_EvalFunc_GRIDTagDyn(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    Vec3 pos;
    float distLimit;
    char capability;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    capability = AI_GetIntFromStack();
    distLimit = AI_GetFloatFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_GetVecFromStack(&pos);
    AI_PutIntToStack(AI_EvalFunc_GRIDTagDyn_C(gaoUltra, &pos, (Vec3*)var.ptr,
                                              distLimit, capability));
    return node + 1;
}

// Whether the object is hemmed in along its own line of sight.
BOOL AI_EvalFunc_GRIDSurround_C(tGameObjectMain* gao)
{
    Vec3 sight;

    OBJ_SightGet(gao, &sight);
    return GRI_Surround(AI_l_CurrentGrid(), &gao->matrices->Pos, &sight);
}

tAINode* AI_EvalFunc_GRIDSurround(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_GRIDSurround_C(gaoUltra));
    return node + 1;
}
