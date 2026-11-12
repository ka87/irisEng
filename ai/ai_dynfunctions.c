#include "ai_dynfunctions.h"
#include "../gameobject.h"

// Every dynamics builtin starts here: the object's basic block, and
// the dynamics behind it. The decompiler wrote this out as a null test
// and an if/else at each of the fifty-odd sites.
static tDyn* AI_l_GetDyn(struct sGameObjectMain* gao)
{
    if (!gao->basic)
        return NULL;
    return gao->basic->dyn;
}

tAINode* AI_EvalFunc_DYNSpeedReachInTime(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;
    Vec3 target;
    Vec3 reached;
    float speed;
    float time;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    time = AI_GetFloatFromStack();
    AI_GetVecFromStack(&target);

    dyn = AI_l_GetDyn(gaoUltra);
    if (dyn)
        AI_PutIntToStack(DYN_b_SpeedReachInTime(dyn, &speed, &reached, &target, time));
    else
        AI_PutIntToStack(0);
    return node + 1;
}

int AI_EvalFunc_DYNSpeedSetVector_C(struct sGameObjectMain* gao, Vec3* speed)
{
    DYN_SetSpeedVector(AI_l_GetDyn(gao), speed);
    return 1;
}

tAINode* AI_EvalFunc_DYNSpeedSetVector(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 speed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&speed);
    AI_PutIntToStack(AI_EvalFunc_DYNSpeedSetVector_C(gaoUltra, &speed));
    return node + 1;
}

void AI_EvalFunc_DYNSpeedGetVector_C(struct sGameObjectMain* gao, Vec3* speed)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    if (dyn)
        DYN_SpeedGetVector(dyn, speed);
    else
        *speed = MATH_gst_NulVector;
}

void AI_EvalFunc_DYNSpeedGetVector_C_CURRENT(Vec3* speed)
{
    AI_EvalFunc_DYNSpeedGetVector_C(AI_gpst_CurrentGameObject, speed);
}

tAINode* AI_EvalFunc_DYNSpeedGetVector(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 speed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNSpeedGetVector_C(gaoUltra, &speed);
    AI_PutVecToStack(&speed);
    return node + 1;
}

// The engine keeps the reciprocal of the mass, so both of these invert.
int AI_EvalFunc_DYNMassSet_C(tGameObjectMain* gao, float mass)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    if (!dyn)
        return 0;

    dyn->mass = (float)(1.0 / mass);
    return 1;
}

tAINode* AI_EvalFunc_DYNMassSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float mass;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mass = AI_GetFloatFromStack();
    AI_PutIntToStack(AI_EvalFunc_DYNMassSet_C(gaoUltra, mass));
    return node + 1;
}

tAINode* AI_EvalFunc_DYNMaxStepSizeSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;
    float maxStepSize;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    maxStepSize = AI_GetFloatFromStack();

    dyn = AI_l_GetDyn(gaoUltra);
    if (dyn)
        dyn->maxStepSize = maxStepSize;
    return node + 1;
}

float AI_EvalFunc_DYNMassGet_C(tGameObjectMain* gao)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    if (!dyn)
        return 0.0f;
    return (float)(1.0 / dyn->mass);
}

tAINode* AI_EvalFunc_DYNMassGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_DYNMassGet_C(gaoUltra));
    return node + 1;
}

// Adds the object to the world's dynamic table, in place when the AI is
// running inside the engine's own callback and by asking for a rebuild
// otherwise. Reports whether it ended up with dynamics.
//
// The null test the decompiler put on the flags read is dead: gao is
// dereferenced unconditionally below it. It is what an inlined flags
// getter leaves behind, and MSGOn has the same one.
BOOL AI_EvalFunc_DYNOn_C(struct sGameObjectMain* gao)
{
    tWorld* world;
    tableElemStructPF* entry;

    OBJ_ChangeIdentityFlags(gao, gao->flags | 4, -1);

    world = WOR_gpst_CurrentWorld;
    if (AI_C_Callback == 1)
    {
        entry = (tableElemStructPF*)world->eot.dynamicObjectsTablePF.nextElemPtr;
        entry->objectPtr = gao;
        entry->objectFlags = gao->flags;
        ++world->eot.dynamicObjectsTablePF.actElemNum;
        world->eot.dynamicObjectsTablePF.nextElemPtr = entry + 1;
    }
    else
    {
        ENG_gb_MustRebuildDyn = 1;
    }
    return AI_l_GetDyn(gao) != 0;
}

tAINode* AI_EvalFunc_DYNOn(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_DYNOn_C(gaoUltra));
    return node + 1;
}

BOOL AI_EvalFunc_DYNOff_C(struct sGameObjectMain* gao)
{
    OBJ_ChangeIdentityFlags(gao, gao->flags & ~4u, -1);

    if (AI_C_Callback == 1)
        TAB_PFtable_RemoveElemWithPointer(&WOR_gpst_CurrentWorld->eot.dynamicObjectsTablePF, gao);
    else
        ENG_gb_MustRebuildDyn = 1;

    return AI_l_GetDyn(gao) == 0;
}

tAINode* AI_EvalFunc_DYNOff(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_DYNOff_C(gaoUltra));
    return node + 1;
}

// Turns the dynamics on first where the object has none, so a script
// can set flags on an object that was static.
int AI_EvalFunc_DYNFlagsSet_C(struct sGameObjectMain* gao, int toSet, int mask)
{
    tDyn* dyn;
    int flags;

    if (!gao)
        return -1;

    dyn = AI_l_GetDyn(gao);
    if (!dyn)
    {
        OBJ_ChangeIdentityFlags(gao, gao->flags | 4, -1);
        dyn = AI_l_GetDyn(gao);
    }

    flags = toSet | (~mask & dyn->flags);
    DYN_ChangeDynFlags(dyn, flags, dyn->flags);
    return flags;
}

tAINode* AI_EvalFunc_DYNFlagsSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int toSet;
    int mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_DYNFlagsSet_C(gaoUltra, toSet, mask));
    return node + 1;
}

// Note the test: the original takes the address of the flags field and
// then checks THAT for null, so an object whose basic block exists but
// whose dyn is null computes &((tDyn*)0)->flags rather than stopping.
// Preserved as written.
int AI_EvalFunc_DYNFlagsGet_C(struct sGameObjectMain* gao)
{
    int* flags;

    if (!gao)
        return 0;

    if (gao->basic)
        flags = &gao->basic->dyn->flags;
    else
        flags = 0;

    if (!flags)
        return 0;
    return *flags;
}

tAINode* AI_EvalFunc_DYNFlagsGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_DYNFlagsGet_C(gaoUltra));
    return node + 1;
}

Vec3* AI_EvalFunc_DYNGravitySet_C(struct sGameObjectMain* gao, Vec3* gravity)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    dyn->forces->gravityVector = *gravity;
    //dyn->forces->gravityVector.z = 0;
    return gravity;
}

tAINode* AI_EvalFunc_DYNGravitySet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 gravity;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&gravity);
    AI_EvalFunc_DYNGravitySet_C(gaoUltra, &gravity);
    return node + 1;
}

void AI_EvalFunc_DYNTractionSet_C(struct sGameObjectMain* gao, Vec3* traction)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    if (dyn && dyn->forces)
        dyn->forces->tractionVector = *traction;
}

void AI_EvalFunc_DYNTractionSet_C_CURRENT(Vec3* traction)
{
    AI_EvalFunc_DYNTractionSet_C(AI_gpst_CurrentGameObject, traction);
}

tAINode* AI_EvalFunc_DYNTractionSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 traction;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&traction);
    AI_EvalFunc_DYNTractionSet_C(gaoUltra, &traction);
    return node + 1;
}

Vec3* AI_EvalFunc_DYNStreamSet_C(struct sGameObjectMain* gao, Vec3* stream)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    dyn->forces->streamVec = *stream;
    return stream;
}

tAINode* AI_EvalFunc_DYNStreamSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 stream;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&stream);
    AI_EvalFunc_DYNStreamSet_C(gaoUltra, &stream);
    return node + 1;
}

// The scalar friction is the friction vector's x component.
int AI_EvalFunc_DYNFrictionSet_C(struct sGameObjectMain* gao, float friction)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    dyn->forces->frictionVector.x = friction;
    return 1;
}

tAINode* AI_EvalFunc_DYNFrictionSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float friction;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    friction = AI_GetFloatFromStack();
    AI_PutIntToStack(AI_EvalFunc_DYNFrictionSet_C(gaoUltra, friction));
    return node + 1;
}

float AI_EvalFunc_DYNFrictionGet_C(struct sGameObjectMain* gao)
{
    tDyn* dyn;

    if (!gao)
        return 0.0f;

    dyn = AI_l_GetDyn(gao);
    if (!dyn)
        return 0.0f;
    if ((dyn->flags & 4) == 0)
        return 0.0f;
    return dyn->forces->frictionVector.x;
}

tAINode* AI_EvalFunc_DYNFrictionGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_DYNFrictionGet_C(gaoUltra));
    return node + 1;
}

int AI_EvalFunc_DYNFrictionVectorSet_C(struct sGameObjectMain* gao, Vec3* friction)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    if ((dyn->flags & 4) == 0)
        return 0;

    dyn->forces->frictionVector = *friction;
    return 1;
}

tAINode* AI_EvalFunc_DYNFrictionVectorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 friction;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&friction);
    AI_PutIntToStack(AI_EvalFunc_DYNFrictionVectorSet_C(gaoUltra, &friction));
    return node + 1;
}

Vec3* AI_EvalFunc_DYNFrictionVectorGet_C(struct sGameObjectMain* gao, Vec3* friction)
{
    tDynForces* forces;

    forces = AI_l_GetDyn(gao)->forces;
    *friction = forces->frictionVector;
    return &forces->frictionVector;
}

tAINode* AI_EvalFunc_DYNFrictionVectorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 friction;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNFrictionVectorGet_C(gaoUltra, &friction);
    AI_PutVecToStack(&friction);
    return node + 1;
}

struct sDynForces* AI_EvalFunc_DYNGravityVectorGet_C(struct sGameObjectMain* gao, Vec3* gravity)
{
    tDynForces* forces;

    forces = AI_l_GetDyn(gao)->forces;
    *gravity = forces->gravityVector;
    return forces;
}

tAINode* AI_EvalFunc_DYNGravityVectorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 gravity;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNGravityVectorGet_C(gaoUltra, &gravity);
    AI_PutVecToStack(&gravity);
    return node + 1;
}

Vec3* AI_EvalFunc_DYNStreamVectorGet_C(struct sGameObjectMain* gao, Vec3* stream)
{
    tDynForces* forces;

    forces = AI_l_GetDyn(gao)->forces;
    *stream = forces->streamVec;
    return &forces->streamVec;
}

tAINode* AI_EvalFunc_DYNStreamVectorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 stream;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNStreamVectorGet_C(gaoUltra, &stream);
    AI_PutVecToStack(&stream);
    return node + 1;
}

Vec3* AI_EvalFunc_DYNTractionVectorGet_C(struct sGameObjectMain* gao, Vec3* traction)
{
    tDynForces* forces;

    forces = AI_l_GetDyn(gao)->forces;
    *traction = forces->tractionVector;
    return &forces->tractionVector;
}

tAINode* AI_EvalFunc_DYNTractionVectorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 traction;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNTractionVectorGet_C(gaoUltra, &traction);
    AI_PutVecToStack(&traction);
    return node + 1;
}

tAINode* AI_EvalFunc_DYNFrictionSetForMaxSpeed_(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;
    float maxSpeed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    maxSpeed = AI_GetFloatFromStack();

    if (!gaoUltra)
    {
        AI_PutIntToStack(0);
        return node + 1;
    }

    dyn = AI_l_GetDyn(gaoUltra);
    if (!dyn || (dyn->flags & 4) == 0)
    {
        AI_PutIntToStack(0);
        return node + 1;
    }

    dyn->forces->frictionVector.x = DYN_f_FindFrictionForMaxSpeed(dyn, maxSpeed);
    AI_PutIntToStack(1);
    return node + 1;
}

float AI_EvalFunc_DYNSpeedGet_C(struct sGameObjectMain* gao)
{
    tDyn* dyn;
    Vec3 speed;

    dyn = AI_l_GetDyn(gao);
    if (!dyn)
        return 0.0f;

    DYN_SpeedGetVector(dyn, &speed);
    return MATH_VecLen(&speed);
}

tAINode* AI_EvalFunc_DYNSpeedGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_DYNSpeedGet_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_DYNSpeedSet_C(struct sGameObjectMain* gao, float speed)
{
    DYN_ApplyHorizontalSpeed(AI_l_GetDyn(gao), speed);
}

tAINode* AI_EvalFunc_DYNSpeedSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float speed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    speed = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedSet_C(gaoUltra, speed);
    return node + 1;
}

void AI_EvalFunc_DYNElasticSet_C(tGameObjectMain* gao, float elastic)
{
    AI_l_GetDyn(gao)->solid->elastic = elastic;
}

tAINode* AI_EvalFunc_DYNElasticSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float elastic;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    elastic = AI_GetFloatFromStack();
    AI_EvalFunc_DYNElasticSet_C(gaoUltra, elastic);
    return node + 1;
}

float AI_EvalFunc_DYNSpeedLimitGet_C(struct sGameObjectMain* gao)
{
    return AI_l_GetDyn(gao)->speedLimit;
}

tAINode* AI_EvalFunc_DYNSpeedLimitGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_DYNSpeedLimitGet_C(gaoUltra));
    return node + 1;
}

tAINode* AI_EvalFunc_DYNSpeedLimitHorizGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_l_GetDyn(gaoUltra)->speedLimit);
    return node + 1;
}

tAINode* AI_EvalFunc_DYNSpeedLimitVertGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_l_GetDyn(gaoUltra)->speedLimitVertical);
    return node + 1;
}

// The plain setter sets both limits; the Horiz and Vert ones each set
// their own.
struct sDyn* AI_EvalFunc_DYNSpeedLimitSet_C(struct sGameObjectMain* gao, float limit)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    dyn->speedLimit = limit;
    dyn->speedLimitVertical = limit;
    return dyn;
}

tAINode* AI_EvalFunc_DYNSpeedLimitSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float limit;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    limit = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedLimitSet_C(gaoUltra, limit);
    return node + 1;
}

void AI_EvalFunc_DYNSpeedLimitHorizSet_C(tGameObjectMain* gao, float limit)
{
    AI_l_GetDyn(gao)->speedLimit = limit;
}

tAINode* AI_EvalFunc_DYNSpeedLimitHorizSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float limit;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    limit = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedLimitHorizSet_C(gaoUltra, limit);
    return node + 1;
}

void AI_EvalFunc_DYNSpeedLimitVertSet_C(tGameObjectMain* gao, float limit)
{
    AI_l_GetDyn(gao)->speedLimitVertical = limit;
}

tAINode* AI_EvalFunc_DYNSpeedLimitVertSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float limit;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    limit = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSpeedLimitVertSet_C(gaoUltra, limit);
    return node + 1;
}

// The auto-orient inertia is two bits of the dynamics flags. Note the
// += rather than |=, which is the same only because the field was
// cleared on the line above.
tAINode* AI_EvalFunc_DYNAutoOrientInertiaSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;
    int16_t inertia;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    inertia = AI_GetIntFromStack();

    if (!gaoUltra)
        return node + 1;

    dyn = AI_l_GetDyn(gaoUltra);
    if (dyn)
    {
        dyn->flags &= ~0xC00u;
        dyn->flags += inertia & 0xC00;
    }
    return node + 1;
}

tAINode* AI_EvalFunc_DYNAutoOrientInertiaGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    dyn = gaoUltra ? AI_l_GetDyn(gaoUltra) : NULL;
    if (dyn)
        AI_PutIntToStack(dyn->flags & 0xC00);
    else
        AI_PutIntToStack(0);
    return node + 1;
}

void AI_EvalFunc_DYNConstraintObjectToSegment_C(tGameObjectMain* gao, Vec3* from, Vec3* to)
{
    DYN_ConstraintInit(AI_l_GetDyn(gao)->constraint, 1, from, to, &MATH_gst_NulVector, 0.0);
}

tAINode* AI_EvalFunc_DYNConstraintObjectToSegment(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 from;
    Vec3 to;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&to);
    AI_GetVecFromStack(&from);
    AI_EvalFunc_DYNConstraintObjectToSegment_C(gaoUltra, &from, &to);
    return node + 1;
}

void AI_EvalFunc_DYNConstraintObjectToParabol_C(tGameObjectMain* gao, Vec3* from, Vec3* through, Vec3* to, float param)
{
    DYN_ConstraintInit(AI_l_GetDyn(gao)->constraint, 2, from, through, to, param);
}

tAINode* AI_EvalFunc_DYNConstraintObjectToParabol(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 from;
    Vec3 through;
    Vec3 to;
    float param;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    param = AI_GetFloatFromStack();
    AI_GetVecFromStack(&to);
    AI_GetVecFromStack(&through);
    AI_GetVecFromStack(&from);
    AI_EvalFunc_DYNConstraintObjectToParabol_C(gaoUltra, &from, &through, &to, param);
    return node + 1;
}

// Launches the object so that it lands on target having risen height
// above whichever of its start and its target is the higher. Clears
// the traction and friction and lifts both speed limits, since a
// ballistic arc is not to be interfered with.
void AI_EvalFunc_DYNImpulsion_C(struct sGameObjectMain* gao, Vec3* target, float height)
{
    tDynForces* forces;
    tDyn* dyn;
    Mtx4x4 frame;
    Vec3 gravityDir;
    Vec3 up;
    Vec3 fromToTarget;
    Vec3 apex;
    Vec3 apexToFrom;
    Vec3 apexToTarget;
    Vec3 local;
    Vec3 localSpeed;
    Vec3 speed;
    Vec3* from;
    Vec3* base;
    float gravity;
    float dropFrom;
    float dropTarget;
    float flightTime;
    float launchUp;
    float invTime;

    dyn = AI_l_GetDyn(gao);
    MATH_VecSet(&dyn->forces->tractionVector, &MATH_gst_NulVector);
    MATH_VecSet(&dyn->forces->frictionVector, &MATH_gst_NulVector);
    dyn->speedLimit = 3.402822e38f;          // very nearly FLT_MAX, as written
    dyn->speedLimitVertical = 3.402822e38f;

    forces = dyn->forces;
    MATH_NormalizeVector(&gravityDir, &forces->gravityVector);
    MATH_VecInvert(&up, &gravityDir);
    gravity = MATH_VecLen(&forces->gravityVector);

    from = dyn->vptr;
    MATH_SubstractVec(&fromToTarget, target, from);
    if (height < 0.0)
        height = 0.0;

    // the apex sits height above the higher of the two ends
    MATH_VecMultScalar(&apex, &up, height);
    if (MATH_DotProduct(&fromToTarget, &up) <= 0.0)
    {
        apex.x = apex.x + from->x;
        apex.y = apex.y + from->y;
        base = from;
    }
    else
    {
        apex.x = apex.x + target->x;
        apex.y = apex.y + target->y;
        base = target;
    }
    apex.z = apex.z + base->z;

    MATH_SubstractVec(&apexToTarget, target, &apex);
    MATH_SubstractVec(&apexToFrom, from, &apex);
    dropFrom = MATH_DotProduct(&gravityDir, &apexToFrom);
    dropTarget = MATH_DotProduct(&gravityDir, &apexToTarget);

    // time to fall to each end, and the vertical speed that reaches the
    // apex from the start
    flightTime = (float)sqrt(2.0 / gravity)
                 * ((float)sqrt(dropFrom) + (float)sqrt(dropTarget));
    launchUp = (float)sqrt(2.0 * gravity * dropFrom);
    invTime = (float)(1.0 / flightTime);

    if (MATH_b_EqVectorWithEpsilon(&up, &MATH_gst_BaseVectorK, 0.000099999997f))
    {
        // world up, so the horizontal part is just x and y over time
        speed.z = launchUp;
        speed.x = (target->x - from->x) * invTime;
        speed.y = (target->y - from->y) * invTime;
    }
    else
    {
        MATH_MakeRotationMatrix_UsingBanking(&frame, &fromToTarget, &up, 1);
        MATH_VectorGlobalToLocal(&local, &frame, &fromToTarget);
        localSpeed.z = launchUp;
        localSpeed.x = local.x * invTime;
        localSpeed.y = local.y * invTime;
        MATH_TransformVectorNoScale(&speed, &frame, &localSpeed);
    }
    DYN_SetSpeedVector(dyn, &speed);
}

tAINode* AI_EvalFunc_DYNImpulsion(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 target;
    float height;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    height = AI_GetFloatFromStack();
    AI_GetVecFromStack(&target);
    AI_EvalFunc_DYNImpulsion_C(gaoUltra, &target, height);
    return node + 1;
}

// The next four write through a null pointer, or hand one to
// AI_PutVecToStack, when the object has no solid. Preserved as written.
tAINode* AI_EvalFunc_DYNTorqueVectorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynSolid* solid;
    Vec3* slot;
    Vec3 torque;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&torque);

    solid = AI_l_GetDyn(gaoUltra)->solid;
    if (solid)
        slot = &solid->torqueVector;
    else
        slot = 0;
    *slot = torque;
    return node + 1;
}

tAINode* AI_EvalFunc_DYNTorqueVectorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynSolid* solid;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    solid = AI_l_GetDyn(gaoUltra)->solid;
    if (solid)
        AI_PutVecToStack(&solid->torqueVector);
    else
        AI_PutVecToStack(0);
    return node + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedVectorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;
    Vec3* slot;
    Vec3 rotSpeed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&rotSpeed);

    dyn = AI_l_GetDyn(gaoUltra);
    if (dyn->solid)
        slot = &dyn->solid->rotSpeedVector;
    else
        slot = 0;
    *slot = rotSpeed;
    return node + 1;
}

// Here the address is taken before the test, so the test can only fail
// if the field sits at offset zero of a null solid. Preserved.
tAINode* AI_EvalFunc_DYNRotSpeedVectorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3* rotSpeed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    rotSpeed = &AI_l_GetDyn(gaoUltra)->solid->rotSpeedVector;
    if (rotSpeed)
        AI_PutVecToStack(rotSpeed);
    else
        AI_PutVecToStack(0);
    return node + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedLimitGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynSolid* solid;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    solid = AI_l_GetDyn(gaoUltra)->solid;
    if (solid)
        AI_PutFloatToStack(solid->rotSpeedLimit);
    else
        AI_PutFloatToStack(0.0);
    return node + 1;
}

tAINode* AI_EvalFunc_DYNRotSpeedLimitSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynSolid* solid;
    float limit;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    limit = AI_GetFloatFromStack();

    solid = AI_l_GetDyn(gaoUltra)->solid;
    if (solid)
        solid->rotSpeedLimit = limit;
    return node + 1;
}

BOOL AI_EvalFunc_DYNSlideSet_C(tGameObjectMain* gao, float slide)
{
    tDynCol* collision;

    collision = AI_l_GetDyn(gao)->collision;
    if (!collision)
        return FALSE;

    collision->slide = slide;
    return TRUE;
}

tAINode* AI_EvalFunc_DYNSlideSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float slide;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    slide = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSlideSet_C(gaoUltra, slide);
    return node + 1;
}

// With no collision the answer is 1.0, not 0.0 as elsewhere in the
// file: full slide is the neutral value here.
tAINode* AI_EvalFunc_DYNSlideGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynCol* collision;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    collision = AI_l_GetDyn(gaoUltra)->collision;
    if (collision)
        AI_PutFloatToStack(collision->slide);
    else
        AI_PutFloatToStack(1.0);
    return node + 1;
}

BOOL AI_EvalFunc_DYNReboundSet_C(struct sGameObjectMain* gao, float rebound)
{
    tDynCol* collision;

    collision = AI_l_GetDyn(gao)->collision;
    if (!collision)
        return FALSE;

    collision->rebound = rebound;
    return TRUE;
}

tAINode* AI_EvalFunc_DYNReboundSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float rebound;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    rebound = AI_GetFloatFromStack();
    AI_EvalFunc_DYNReboundSet_C(gaoUltra, rebound);
    return node + 1;
}

struct sDynCol* AI_EvalFunc_DYNApplyRectFactorSet_C(struct sGameObjectMain* gao, float rectFactor)
{
    tDynCol* collision;

    collision = AI_l_GetDyn(gao)->collision;
    collision->rectFactor = rectFactor;
    return collision;
}

tAINode* AI_EvalFunc_DYNApplyRectFactorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float rectFactor;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    rectFactor = AI_GetFloatFromStack();
    AI_EvalFunc_DYNApplyRectFactorSet_C(gaoUltra, rectFactor);
    return node + 1;
}

// The address is taken before the test again, so the test cannot fire.
// Preserved as written.
float AI_EvalFunc_DYNReboundGet_C(tGameObjectMain* gao)
{
    float* rebound;

    rebound = &AI_l_GetDyn(gao)->collision->rebound;
    if (!rebound)
        return 0.0f;
    return *rebound;
}

tAINode* AI_EvalFunc_DYNReboundGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_DYNReboundGet_C(gaoUltra));
    return node + 1;
}

tAINode* AI_EvalFunc_DYNReboundHorizCosAngleSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynCol* collision;
    float cosAngle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    cosAngle = AI_GetFloatFromStack();

    collision = AI_l_GetDyn(gaoUltra)->collision;
    if (collision)
        collision->reboundHorizCosAngle = cosAngle;
    return node + 1;
}

tAINode* AI_EvalFunc_DYNReboundHorizCosAngleGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDynCol* collision;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    collision = AI_l_GetDyn(gaoUltra)->collision;
    if (collision)
        AI_PutFloatToStack(collision->reboundHorizCosAngle);
    else
        AI_PutFloatToStack(0.0);
    return node + 1;
}

BOOL AI_EvalFunc_DYNSlideHorizCosAngleSet_C(struct sGameObjectMain* gao, float cosAngle)
{
    tDynCol* collision;

    collision = AI_l_GetDyn(gao)->collision;
    if (!collision)
        return FALSE;

    collision->slideHorizCosAngle = cosAngle;
    return TRUE;
}

tAINode* AI_EvalFunc_DYNSlideHorizCosAngleSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float cosAngle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    cosAngle = AI_GetFloatFromStack();
    AI_EvalFunc_DYNSlideHorizCosAngleSet_C(gaoUltra, cosAngle);
    return node + 1;
}

float AI_EvalFunc_DYNSlideHorizCosAngleGet_C(struct sGameObjectMain* gao)
{
    tDynCol* collision;

    collision = AI_l_GetDyn(gao)->collision;
    if (!collision)
        return 0.0f;
    return collision->slideHorizCosAngle;
}

tAINode* AI_EvalFunc_DYNSlideHorizCosAngleGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_DYNSlideHorizCosAngleGet_C(gaoUltra));
    return node + 1;
}

// The dynamics are read before the vector is popped here, where every
// other builtin in the file pops first; preserved, since the pop can
// run engine code.
tAINode* AI_EvalFunc_DYNSpeedVectorHorizSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tDyn* dyn;
    Vec3 speed;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    dyn = AI_l_GetDyn(gaoUltra);
    AI_GetVecFromStack(&speed);
    DYN_SetSpeedVector_WithoutChangingGravity(dyn, &speed);
    return node + 1;
}

Vec3* AI_EvalFunc_DYNMaxPosSet_C(struct sGameObjectMain* gao, Vec3* maxPos)
{
    tDyn* dyn;

    dyn = AI_l_GetDyn(gao);
    dyn->maxPos = *maxPos;
    return &dyn->maxPos;
}

tAINode* AI_EvalFunc_DYNMaxPosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 maxPos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&maxPos);
    AI_EvalFunc_DYNMaxPosSet_C(gaoUltra, &maxPos);
    return node + 1;
}

Vec3* AI_EvalFunc_DYNConstraintGetTangent_C(tGameObjectMain* gao, Vec3* tangent)
{
    tDynConstraint* constraint;

    constraint = AI_l_GetDyn(gao)->constraint;
    *tangent = constraint->tangentVec;
    return &constraint->tangentVec;
}

tAINode* AI_EvalFunc_DYNConstraintGetTangent(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 tangent;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNConstraintGetTangent_C(gaoUltra, &tangent);
    AI_PutVecToStack(&tangent);
    return node + 1;
}

// Keeps the speed the object has and points it along the object's
// sight.
void AI_EvalFunc_DYNAlignSight_C(tGameObjectMain* gao)
{
    tDyn* dyn;
    Vec3 vec;
    float speed;

    dyn = AI_l_GetDyn(gao);
    DYN_SpeedGetVector(dyn, &vec);
    speed = MATH_VecLen(&vec);

    OBJ_SightGet(gao, &vec);
    MATH_NormalizeVector(&vec, &vec);
    MATH_VecMultScalarWithoutBuffer(&vec, speed);
    DYN_SetSpeedVector(dyn, &vec);
}

tAINode* AI_EvalFunc_DYNAlignSight(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_DYNAlignSight_C(gaoUltra);
    return node + 1;
}
