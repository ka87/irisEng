#include "ai_gameobjectfunctions.h"
#include "../engine.h"
#include "../memory.h"
#include "../math.h"
#include "../snp.h"
#include "../profiler.h"
#include "../animation.h"
#include "../events.h"
#include "../world.h"
#include "../modifiers/morphmodifier.h"
#include "../modifiers/rlicartemodifier.h"
#include "../modifiers/leglinkmodifier.h"
#include "../photo.h"
#include "../modifiers/photomodifier.h"

// A game object's visual, which it only has while it carries the
// visual flag.
static tGameObjectVisualObject* AI_l_Visual(struct sGameObjectMain* gao)
{
    if (!gao->basic || !gao->basic->actorObjectOrVisualObject || (gao->flags & 0x4000) == 0)
        return NULL;
    return (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
}

// The actor behind a game object.
static tGameObjectActorObject* AI_l_Actor(struct sGameObjectMain* gao)
{
    return (tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject);
}

// The matrix an animation flash keeps beside the object's own: the
// third where the object also has an init matrix, the second
// otherwise. Only called where the object is known to be flashing.
static Mtx4x4* AI_l_FlashMatrix(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x10000) != 0)
        return gao->matrices + 2;
    return gao->matrices + 1;
}

// While an event is driving the behaviour, a per-call delta is scaled
// by the frame time over the event's own duration in seconds.
static float AI_l_EventScale(void)
{
    float duration;

    duration = (float)EVE_gpst_CurrentEvent->duration;
    return (float)(EVE_gf_DT / (duration * 1.0 / 60.0));
}

/* flags2 holds two things: the upper sixteen bits are the control
   flags a script sets and the lower sixteen the status bits the engine
   keeps. Every write to the upper half came out as three statements --
   build the new half, truncate the word, OR the half back -- which
   comes to a plain set or clear of those bits, and is written that way
   throughout. */

int AI_EvalFunc_OBJ_HasBeenKilled_C(struct sGameObjectMain* gao)
{
    int i;

    for (i = 0; i < WOR_gi_NumKilledGO; ++i)
    {
        if (WOR_AllKilledGO[i] == gao)
            return 1;
    }
    return 0;
}

tAINode* AI_EvalFunc_OBJ_HasBeenKilled(tAINode* node)
{
    tGameObjectMain* gao;

    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_HasBeenKilled_C(gao));
    return node + 1;
}

// Writes the progress array out beside the executable, behind its own
// four-character magic.
void AI_EvalFunc_OutputProgress_C(int num, void* values)
{
    FILE* stream;
    int magic;

    stream = fopen("SFTime.sav", "wb");
    if (stream)
    {
        magic = '.TFS';
        fwrite(&magic, 4u, 1u, stream);
        fwrite(&num, 4u, 1u, stream);
        fwrite(values, 4 * num, 1u, stream);
        fclose(stream);
    }
}

tAINode* AI_EvalFunc_OutputProgress(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int num;

    AI_GetVarFromStack(converted, &var);
    num = AI_GetIntFromStack();
    AI_EvalFunc_OutputProgress_C(num, var.ptr);
    return node + 1;
}

void AI_EvalFunc_OBJDrawMaskSet_C(struct sGameObjectMain* gao, int toSet, int toClear)
{
    tGameObjectVisualObject* visual;

    visual = AI_l_Visual(gao);
    if (visual)
    {
        visual->objDrawMask |= toSet;
        visual->objDrawMask &= ~toClear;
    }
}

tAINode* AI_EvalFunc_OBJDrawMaskSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int toSet;
    int toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_OBJDrawMaskSet_C(gaoUltra, toSet, toClear);
    return node + 1;
}

int AI_EvalFunc_OBJDrawMaskGet_C(struct sGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;

    visual = AI_l_Visual(gao);
    if (!visual)
        return 0;
    return visual->objDrawMask;
}

tAINode* AI_EvalFunc_OBJDrawMaskGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJDrawMaskGet_C(gaoUltra));
    return node + 1;
}

tAINode* AI_EvalFunc_OBJMe(tAINode* node)
{
    tGameObjectMain* me;

    me = AI_gpst_CurrentGameObject;
    AI_PutObjectToStack(&me, AI_gaw_EnumLink[40]);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_OBJDuplicate_C(struct sGameObjectMain* srcGao, Vec3* pos)
{
    tGameObjectMain* copy;

    OBJ_gb_DuplicateAI = 1;
    copy = OBJ_GameObject_Duplicate(WOR_gpst_CurrentWorld, srcGao, 1, 0, pos);
    OBJ_SetInitialAbsolutePosition(copy, pos);
    OBJ_RestoreInitialPos(copy);
    ENG_ReinitOneObject(copy, 13);
    INT_SnP_AttachObject(copy, WOR_gpst_CurrentWorld);
    OBJ_gb_DuplicateAI = 0;
    return copy;
}

tAINode* AI_EvalFunc_OBJDuplicate(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* copy;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the position is read where the stack holds it rather than copied out
    copy = AI_EvalFunc_OBJDuplicate_C(gaoUltra, (Vec3*)AI_PopStackSlot());
    AI_PutObjectToStack(&copy, AI_gaw_EnumLink[40]);
    return node + 1;
}

// Removes a duplicate outright, and otherwise marks the object
// invisible and inactive and leaves the engine to tidy up. Returns an
// interpreter control code when the object destroying itself is the
// one running the behaviour: 6 for a removal, 1 for a deactivation.
int AI_EvalFunc_OBJDestroy_C(struct sGameObjectMain* gao)
{
    tWorld* world;

    if (WOR_gi_NumKilledGO < 200)
        WOR_AllKilledGO[WOR_gi_NumKilledGO++] = gao;

    if ((gao->flags & 0x20000) != 0)
    {
        world = WOR_gpst_CurrentWorld;
        if (world)
        {
            WOR_DelObj(world, gao);
            WOR_World_DetachObject(world, gao);
            INT_SnP_DetachObject(gao, world);
            SND_DetachObject(gao);
        }
        OBJ_GameObject_Remove(gao, 1);
        if (gao == AI_gpst_CurrentGameObject)
            return 6;
    }
    else
    {
        gao->flags2 |= 0x200000;
        gao->flags2 |= 0x400000;

        world = WOR_gpst_CurrentWorld;
        if (world)
        {
            world->tablesManagerAndEOTNeeded = 1;
            world->visibilityCallNeeded = 1;
        }

        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_DestroyInstanceOfGao(gao);
        PRO_QPC_RecordTimeDecRef(39);

        // a hierarchy root with nothing above it loses its local matrix
        if ((gao->flags & 0x400000) != 0 && gao->basic && gao->basic->mtx && !gao->basic->mtx->refGao)
        {
            OBJ_ComputeGlobalWhenHie(gao);
            if ((gao->flags & 0x4000000) != 0)
                ANI_RemoveHierarchyOnFlash(gao);
            gao->flags &= ~0x400000u;
            MEM_Free(gao->basic->mtx);
            gao->basic->mtx = 0;
        }
    }

    if (gao == AI_gpst_CurrentGameObject)
        return 1;
    return 1000;
}

tAINode* AI_EvalFunc_OBJDestroy(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int code;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    code = AI_EvalFunc_OBJDestroy_C(gaoUltra);
    if (code == 1)
        return (tAINode*)1;
    if (code == 6)
        return (tAINode*)6;
    return node + 1;
}

// Note that the scaling is applied to the caller's vector in place, so
// a script variable passed here is changed while an event runs.
void AI_EvalFunc_OBJMove_C(struct sGameObjectMain* gao, Vec3* move)
{
    float scale;

    if (ENG_gb_EVERunning)
    {
        scale = AI_l_EventScale();
        move->x = scale * move->x;
        move->y = move->y * scale;
        move->z = move->z * scale;
    }

    MATH_VecAddVecWithoutBuffer(&gao->matrices->Pos, move);

    if ((gao->flags & 4) != 0)
        gao->basic->dyn->flags |= 0x800000u;
}

tAINode* AI_EvalFunc_OBJMove(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJMove_C(gaoUltra, (Vec3*)AI_PopStackSlot());
    return node + 1;
}

void AI_EvalFunc_OBJRotateAround_C(struct sGameObjectMain* gao, Vec3* axis, float angle)
{
    Mtx4x4 rotation;
    Mtx4x4 before;
    Mtx4x4* flash;
    int hierarchical;
    int flashing;

    hierarchical = gao->flags & 0x400000;
    flashing = gao->flags & 0x4000000;
    if (hierarchical)
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);

    MATH_MakeRotationMatrix_AxisAngle(&rotation, axis, angle, 0, 1);
    before = *gao->matrices;
    MATH_MulMatrixMatrix(gao->matrices, &rotation, &before);

    if (flashing)
    {
        flash = AI_l_FlashMatrix(gao);
        before = *flash;
        MATH_MulMatrixMatrix(flash, &rotation, &before);
    }

    if (hierarchical)
        OBJ_ComputeLocalWhenHie(gao);
}

tAINode* AI_EvalFunc_OBJRotateAround(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 axis;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_GetVecFromStack(&axis);
    AI_EvalFunc_OBJRotateAround_C(gaoUltra, &axis, angle);
    return node + 1;
}
// A hierarchical object is lifted into world space, rotated, and put
// back with the position its local matrix held. The flash matrix, when
// there is one, is turned by the same angle.
void AI_EvalFunc_OBJRotateLocal_C(struct sGameObjectMain* gao, Vec3* angles)
{
    float scale;
    int hierarchical;
    int flashing;
    Vec3 localPos;

    if (ENG_gb_EVERunning)
    {
        scale = AI_l_EventScale();
        angles->x = scale * angles->x;
        angles->y = scale * angles->y;
        angles->z = scale * angles->z;
    }

    hierarchical = gao->flags & 0x400000;
    flashing = gao->flags & 0x4000000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angles->x != 0.0)
    {
        MATH_RotateMatrix_AroundLocalXAxis(gao->matrices, angles->x);
        if (flashing)
            MATH_RotateMatrix_AroundLocalXAxis(AI_l_FlashMatrix(gao), angles->x);
    }
    if (angles->y != 0.0)
    {
        MATH_RotateMatrix_AroundLocalYAxis(gao->matrices, angles->y);
        if (flashing)
            MATH_RotateMatrix_AroundLocalYAxis(AI_l_FlashMatrix(gao), angles->y);
    }
    if (angles->z != 0.0)
    {
        MATH_RotateMatrix_AroundLocalZAxis(gao->matrices, angles->z);
        if (flashing)
            MATH_RotateMatrix_AroundLocalZAxis(AI_l_FlashMatrix(gao), angles->z);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateLocal(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 angles;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&angles);
    AI_EvalFunc_OBJRotateLocal_C(gaoUltra, &angles);
    return node + 1;
}

// The single-axis forms also turn the blend matrix of an animation in
// transition, which the three-axis form above leaves alone. The second
// test of the angle is not redundant: the event scale can bring it to
// zero.
void AI_EvalFunc_OBJRotateLocalX_C(struct sGameObjectMain* gao, float angle)
{
    int hierarchical;
    Vec3 localPos;

    if (angle == 0.0)
        return;

    if (ENG_gb_EVERunning)
        angle = (float)(AI_l_EventScale() * angle);

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angle != 0.0)
    {
        MATH_RotateMatrix_AroundLocalXAxis(gao->matrices, angle);
        if ((gao->flags & 0x4000000) != 0)
            MATH_RotateMatrix_AroundLocalXAxis(AI_l_FlashMatrix(gao), angle);
        if (ANI_GaoAnimationIsTransitioning(gao))
            MATH_RotateMatrix_AroundLocalXAxis(&AI_l_Actor(gao)->animPtr[1]->flashMtx, angle);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateLocalX(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateLocalX_C(gaoUltra, angle);
    return node + 1;
}

void AI_EvalFunc_OBJRotateLocalY_C(struct sGameObjectMain* gao, float angle)
{
    int hierarchical;
    Vec3 localPos;

    if (angle == 0.0)
        return;

    if (ENG_gb_EVERunning)
        angle = (float)(AI_l_EventScale() * angle);

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angle != 0.0)
    {
        MATH_RotateMatrix_AroundLocalYAxis(gao->matrices, angle);
        if ((gao->flags & 0x4000000) != 0)
            MATH_RotateMatrix_AroundLocalYAxis(AI_l_FlashMatrix(gao), angle);
        if (ANI_GaoAnimationIsTransitioning(gao))
            MATH_RotateMatrix_AroundLocalYAxis(&AI_l_Actor(gao)->animPtr[1]->flashMtx, angle);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateLocalY(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateLocalY_C(gaoUltra, angle);
    return node + 1;
}

void AI_EvalFunc_OBJRotateLocalZ_c(struct sGameObjectMain* gao, float angle)
{
    int hierarchical;
    Vec3 localPos;

    if (angle == 0.0)
        return;

    if (ENG_gb_EVERunning)
        angle = (float)(AI_l_EventScale() * angle);

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angle != 0.0)
    {
        MATH_RotateMatrix_AroundLocalZAxis(gao->matrices, angle);
        if ((gao->flags & 0x4000000) != 0)
            MATH_RotateMatrix_AroundLocalZAxis(AI_l_FlashMatrix(gao), angle);
        if (ANI_GaoAnimationIsTransitioning(gao))
            MATH_RotateMatrix_AroundLocalZAxis(&AI_l_Actor(gao)->animPtr[1]->flashMtx, angle);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateLocalZ(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateLocalZ_c(gaoUltra, angle);
    return node + 1;
}

// The global-axis three-axis form does not restore the hierarchy
// position afterwards, unlike its local counterpart.
void AI_EvalFunc_OBJRotateGlobal_C(struct sGameObjectMain* gao, Vec3* angles)
{
    float scale;
    int hierarchical;
    int flashing;

    if (ENG_gb_EVERunning)
    {
        scale = AI_l_EventScale();
        angles->x = scale * angles->x;
        angles->y = scale * angles->y;
        angles->z = scale * angles->z;
    }

    hierarchical = gao->flags & 0x400000;
    flashing = gao->flags & 0x4000000;
    if (hierarchical)
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);

    if (angles->x != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalXAxis(gao->matrices, angles->x);
        if (flashing)
            MATH_RotateMatrix_AroundGlobalXAxis(AI_l_FlashMatrix(gao), angles->x);
    }
    if (angles->y != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalYAxis(gao->matrices, angles->y);
        if (flashing)
            MATH_RotateMatrix_AroundGlobalYAxis(AI_l_FlashMatrix(gao), angles->y);
    }
    if (angles->z != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalZAxis(gao->matrices, angles->z);
        if (flashing)
            MATH_RotateMatrix_AroundGlobalZAxis(AI_l_FlashMatrix(gao), angles->z);
    }

    if (hierarchical)
        OBJ_ComputeLocalWhenHie(gao);
}

tAINode* AI_EvalFunc_OBJRotateGlobal(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 angles;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&angles);
    AI_EvalFunc_OBJRotateGlobal_C(gaoUltra, &angles);
    return node + 1;
}

// The single-axis global forms do restore it, and unlike the local ones
// they leave a transitioning animation alone.
void AI_EvalFunc_OBJRotateGlobalX_C(struct sGameObjectMain* gao, float angle)
{
    int hierarchical;
    Vec3 localPos;

    if (angle == 0.0)
        return;

    if (ENG_gb_EVERunning)
        angle = (float)(AI_l_EventScale() * angle);

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angle != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalXAxis(gao->matrices, angle);
        if ((gao->flags & 0x4000000) != 0)
            MATH_RotateMatrix_AroundGlobalXAxis(AI_l_FlashMatrix(gao), angle);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateGlobalX(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateGlobalX_C(gaoUltra, angle);
    return node + 1;
}

void AI_EvalFunc_OBJRotateGlobalY_C(struct sGameObjectMain* gao, float angle)
{
    int hierarchical;
    Vec3 localPos;

    if (angle == 0.0)
        return;

    if (ENG_gb_EVERunning)
        angle = (float)(AI_l_EventScale() * angle);

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angle != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalYAxis(gao->matrices, angle);
        if ((gao->flags & 0x4000000) != 0)
            MATH_RotateMatrix_AroundGlobalYAxis(AI_l_FlashMatrix(gao), angle);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateGlobalY(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateGlobalY_C(gaoUltra, angle);
    return node + 1;
}

void AI_EvalFunc_OBJRotateGlobalZ_C(struct sGameObjectMain* gao, float angle)
{
    int hierarchical;
    Vec3 localPos;

    if (angle == 0.0)
        return;

    if (ENG_gb_EVERunning)
        angle = (float)(AI_l_EventScale() * angle);

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    if (angle != 0.0)
    {
        MATH_RotateMatrix_AroundGlobalZAxis(gao->matrices, angle);
        if ((gao->flags & 0x4000000) != 0)
            MATH_RotateMatrix_AroundGlobalZAxis(AI_l_FlashMatrix(gao), angle);
    }

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
}

tAINode* AI_EvalFunc_OBJRotateGlobalZ(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float angle;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    angle = AI_GetFloatFromStack();
    AI_EvalFunc_OBJRotateGlobalZ_C(gaoUltra, angle);
    return node + 1;
}

// A bone's position is the product of its own local matrix with every
// local matrix up the chain to its animation reference, and then the
// reference's own matrix. Anything else just reads its position.
void AI_EvalFunc_OBJGetPos_C(struct sGameObjectMain* gao, Vec3* pos)
{
    tGameObjectMain* reference;
    tGameObjectMain* walk;
    tGaoMatrix* local;
    Mtx4x4 chain;
    Mtx4x4 product;

    if ((gao->flags & 1) == 0)
    {
        *pos = gao->matrices->Pos;
        return;
    }

    reference = ANI_pst_GetReference(gao);
    MATH_SetIdentityMatrix(&chain);
    walk = gao;
    while (walk != reference)
    {
        local = OBJ_pst_GetLocalMatrix(walk);
        MATH_MulMatrixMatrix(&product, &chain, &local->mtx);
        chain = product;
        if (walk && walk->basic && walk->basic->mtx)
            walk = walk->basic->mtx->refGao2;
        else
            walk = 0;
    }
    MATH_MulMatrixMatrix(&product, &chain, reference->matrices);
    *pos = product.Pos;
}

void AI_EvalFunc_OBJGetPos_C_CURRENT(Vec3* pos)
{
    AI_EvalFunc_OBJGetPos_C(AI_gpst_CurrentGameObject, pos);
}

tAINode* AI_EvalFunc_OBJGetPos(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJGetPos_C(gaoUltra, &pos);
    AI_PutVecToStack(&pos);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJRestoreInitMatrix(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    OBJ_RestoreInitialPos(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_OBJGetPosInit_C(struct sGameObjectMain* gao, Vec3* pos)
{
    if ((gao->flags & 0x10000) != 0)
        *pos = gao->matrices[1].Pos;
    else
        *pos = gao->matrices->Pos;
}

tAINode* AI_EvalFunc_OBJGetPosInit(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJGetPosInit_C(gaoUltra, &pos);
    AI_PutVecToStack(&pos);
    return node + 1;
}

void AI_EvalFunc_OBJSetPos_C(struct sGameObjectMain* gao, Vec3* pos)
{
    int hierarchical;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);

    OBJ_SetAbsolutePosition(gao, pos);

    if (hierarchical)
        OBJ_ComputeLocalWhenHie(gao);
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
    if ((gao->flags & 4) != 0)
        gao->basic->dyn->flags |= 0x800000u;
}

void AI_EvalFunc_OBJSetPos_C_CURRENT(Vec3* pos)
{
    AI_EvalFunc_OBJSetPos_C(AI_gpst_CurrentGameObject, pos);
}

tAINode* AI_EvalFunc_OBJSetPos(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJSetPos_C(gaoUltra, (Vec3*)AI_PopStackSlot());
    return node + 1;
}

void AI_EvalFunc_OBJSetLocalPos_C(struct sGameObjectMain* gao, Vec3* pos)
{
    OBJ_SetLocalPosition(gao, pos);
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
    if ((gao->flags & 4) != 0)
        gao->basic->dyn->flags |= 0x800000u;
}

tAINode* AI_EvalFunc_OBJSetLocalPos(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJSetLocalPos_C(gaoUltra, (Vec3*)AI_PopStackSlot());
    return node + 1;
}

// Note that it is the moving object's own hierarchy flag that decides
// whether the TARGET is lifted into world space first.
void AI_EvalFunc_OBJ_Rejoin_C(tGameObjectMain* gao, tGameObjectMain* target)
{
    int hierarchical;

    if (!target)
        return;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
        OBJ_ComputeGlobalWhenHie(target);

    OBJ_SetAbsolutePosition(gao, &target->matrices->Pos);

    if (hierarchical)
        OBJ_ComputeLocalWhenHie(gao);
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

tAINode* AI_EvalFunc_OBJ_Rejoin(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_Rejoin_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

void AI_EvalFunc_OBJ_RejoinAndOrient_C(struct sGameObjectMain* gao, struct sGameObjectMain* target)
{
    int hierarchical;

    if (!target)
        return;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
        OBJ_ComputeGlobalWhenHie(target);

    OBJ_SetAbsoluteMatrix(gao, OBJ_pst_GetAbsoluteMatrix(target));

    if (hierarchical)
        OBJ_ComputeLocalWhenHie(gao);
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
    if ((gao->flags & 4) != 0)
        gao->basic->dyn->flags |= 0x800000u;
}

tAINode* AI_EvalFunc_OBJ_RejoinAndOrient(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_RejoinAndOrient_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

// Reinit kind 1000 keeps the invisibility count the object already had.
void AI_EvalFunc_OBJReinit_C(struct sGameObjectMain* gao, int kind)
{
    uint8_t invisible;

    invisible = gao->if_this_not0_wont_render_gao;
    ENG_ReinitOneObject(gao, kind);
    if (kind == 1000)
        gao->if_this_not0_wont_render_gao = invisible;
    INT_SnP_AttachObject(gao, WOR_gpst_CurrentWorld);
}

tAINode* AI_EvalFunc_OBJReinit(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int kind;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    kind = AI_GetIntFromStack();
    AI_EvalFunc_OBJReinit_C(gaoUltra, kind);
    return node + 1;
}

void AI_EvalFunc_OBJ_ExtraFlagsSet_C(struct sGameObjectMain* gao, int16_t toSet, int16_t mask)
{
    if (gao->extended)
        gao->extended->extraFlag = toSet | (~mask & gao->extended->extraFlag);
}

tAINode* AI_EvalFunc_OBJ_ExtraFlagsSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t toSet;
    int16_t mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_ExtraFlagsSet_C(gaoUltra, toSet, mask);
    return node + 1;
}

void AI_EvalFunc_OBJ_AILodSet_C(struct sGameObjectMain* gao, float aiLod)
{
    if (gao->extended)
        gao->extended->aiLod = aiLod;
}

tAINode* AI_EvalFunc_OBJ_AILodSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float aiLod;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    aiLod = AI_GetFloatFromStack();
    AI_EvalFunc_OBJ_AILodSet_C(gaoUltra, aiLod);
    return node + 1;
}

// Whichever of two box corners sits further from the origin on one
// axis. Each fabs is recomputed rather than reused, as it was.
static float AI_l_MaxAbs(float first, float second)
{
    if ((float)fabs(second) >= (double)(float)fabs(first))
        return (float)fabs(second);
    return (float)fabs(first);
}

// Sets the object's AI level of detail from the size of its bounding
// box: the half-diagonal of the box squared, plus eight.
void AI_EvalFunc_OBJ_AILodBV_C(tGameObjectMain* gao)
{
    Vec3 boxMin;
    Vec3 boxMax;
    Vec3 extent;
    float aiLod;

    if (!gao->extended)
        return;

    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        boxMin = gao->boundingVolume->BVMinOriginal;
    else
        boxMin = MATH_gst_NulVector;

    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        boxMax = gao->boundingVolume->BVMaxOriginal;
    else
        boxMax = MATH_gst_NulVector;

    extent.x = AI_l_MaxAbs(boxMin.x, boxMax.x);
    extent.y = AI_l_MaxAbs(boxMin.y, boxMax.y);
    extent.z = AI_l_MaxAbs(boxMin.z, boxMax.z);

    aiLod = MATH_VecLen(&extent);
    aiLod = aiLod * aiLod;
    aiLod = (float)(aiLod + 8.0);

    gao->extended->extraFlag |= 8u;
    gao->extended->aiLod = aiLod;
}

tAINode* AI_EvalFunc_OBJ_AILodBV(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_AILodBV_C(gaoUltra);
    return node + 1;
}

float AI_EvalFunc_OBJ_SqrDist_C(struct sGameObjectMain* gao, struct sGameObjectMain* other)
{
    Vec3 between;

    MATH_SubstractVec(&between, &other->matrices->Pos, &gao->matrices->Pos);
    return between.z * between.z + between.y * between.y + between.x * between.x;
}

tAINode* AI_EvalFunc_OBJ_SqrDist(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    other = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_PutFloatToStack(AI_EvalFunc_OBJ_SqrDist_C(gaoUltra, other));
    return node + 1;
}

// The same ignoring height. The height term was a folded zero.
float AI_EvalFunc_OBJ_SqrDistHorz_C(struct sGameObjectMain* gao, struct sGameObjectMain* other)
{
    Vec3 between;

    MATH_SubstractVec(&between, &other->matrices->Pos, &gao->matrices->Pos);
    return between.y * between.y + between.x * between.x;
}

tAINode* AI_EvalFunc_OBJ_SqrDistHorz(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* other;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    other = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_PutFloatToStack(AI_EvalFunc_OBJ_SqrDistHorz_C(gaoUltra, other));
    return node + 1;
}

// Sets and clears control flags in one go, and tells the world which
// of its tables that invalidates. Returns the new control half.
unsigned int AI_EvalFunc_OBJ_FlagsControlSet_C(struct sGameObjectMain* gao, int toSet, int toClear)
{
    unsigned int control;
    tWorld* world;

    control = (toSet | (~toClear & gao->flags2)) & 0xFFFF0000;
    gao->flags2 = (uint16_t)gao->flags2 | control;

    if ((toSet & 0x400000) != 0)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_RegisterInactiveGAO(gao);
        PRO_QPC_RecordTimeDecRef(39);
    }
    if ((toSet & 0x400000) != 0 || (toClear & 0x400000) != 0)
    {
        world = WOR_gpst_CurrentWorld;
        world->tablesManagerAndEOTNeeded = 1;
        world->visibilityCallNeeded = 1;
        world->SnPRefreshNeeded = TRUE;
    }
    if ((toSet & 0x200000) != 0 || (toClear & 0x200000) != 0)
        WOR_gpst_CurrentWorld->visibilityCallNeeded = 1;

    return control;
}

tAINode* AI_EvalFunc_OBJ_FlagsControlSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int toSet;
    int toClear;

    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsControlSet_C(gaoUltra, toSet, toClear));
    return node + 1;
}

unsigned int AI_EvalFunc_OBJ_FlagsControlGet_C(struct sGameObjectMain* gao)
{
    if (!gao)
        return 0;
    return gao->flags2 & 0xFFFF0000;
}

tAINode* AI_EvalFunc_OBJ_FlagsControlGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsControlGet_C(gaoUltra));
    return node + 1;
}

// The world is only told to recompute visibility when the flag
// actually changed.
void AI_EvalFunc_OBJ_FlagInvisibleSet_C(struct sGameObjectMain* gao, int invisible)
{
    if (invisible)
    {
        if ((gao->flags2 & 0x200000) != 0)
            return;
        gao->flags2 |= 0x200000;
    }
    else
    {
        if ((gao->flags2 & 0x200000) == 0)
            return;
        gao->flags2 &= ~0x200000u;
    }
    WOR_gpst_CurrentWorld->visibilityCallNeeded = 1;
}

tAINode* AI_EvalFunc_OBJ_FlagInvisibleSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int invisible;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    invisible = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagInvisibleSet_C(gaoUltra, invisible);
    return node + 1;
}

// Going inactive also drops the local matrix of a hierarchy root that
// has nothing above it, and hands the object to the sound engine.
void AI_EvalFunc_OBJ_FlagInactiveSet_C(struct sGameObjectMain* gao, int inactive)
{
    tWorld* world;

    if (inactive)
    {
        if ((gao->flags2 & 0x400000) != 0)
            return;
        gao->flags2 |= 0x400000;
    }
    else
    {
        if ((gao->flags2 & 0x400000) == 0)
            return;
        gao->flags2 &= ~0x400000u;
    }

    world = WOR_gpst_CurrentWorld;
    world->tablesManagerAndEOTNeeded = 1;
    world->visibilityCallNeeded = 1;
    world->SnPRefreshNeeded = TRUE;

    if (inactive && (gao->flags & 0x400000) != 0 && gao->basic && gao->basic->mtx && !gao->basic->mtx->refGao)
    {
        OBJ_ComputeGlobalWhenHie(gao);
        if ((gao->flags & 0x4000000) != 0)
            ANI_RemoveHierarchyOnFlash(gao);
        gao->flags &= ~0x400000u;
        MEM_Free(gao->basic->mtx);
        gao->basic->mtx = 0;
    }

    if (inactive)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_RegisterInactiveGAO(gao);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_OBJ_FlagInactiveSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int inactive;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    inactive = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagInactiveSet_C(gaoUltra, inactive);
    return node + 1;
}

int AI_EvalFunc_OBJ_FlagsStatusGet_C(tGameObjectMain* gao)
{
    if (!gao)
        return 0;
    return (uint16_t)gao->flags2;
}

tAINode* AI_EvalFunc_OBJ_FlagsStatusGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsStatusGet_C(gaoUltra));
    return node + 1;
}

// The custom bits a script owns are the top byte of the status half.
void AI_EvalFunc_OBJ_CustomBitsSet_C(struct sGameObjectMain* gao, int16_t toSet, int16_t toClear)
{
    gao->flags2 &= ~(toClear & 0xFF00);
    gao->flags2 |= toSet & 0xFF00;
}

void AI_EvalFunc_OBJ_CustomBitsSet_C_CURRENT(int16_t toSet, int16_t toClear)
{
    AI_EvalFunc_OBJ_CustomBitsSet_C(AI_gpst_CurrentGameObject, toSet, toClear);
}

tAINode* AI_EvalFunc_OBJ_CustomBitsSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t toSet;
    int16_t toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_CustomBitsSet_C(gaoUltra, toSet, toClear);
    return node + 1;
}

int AI_EvalFunc_OBJ_CustomBitsGet_C(struct sGameObjectMain* gao)
{
    return gao->flags2 & 0xFF00;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_CustomBitsGet_C(gaoUltra));
    return node + 1;
}

BOOL AI_EvalFunc_OBJ_CustomBitsTest_C(struct sGameObjectMain* gao, int16_t mask)
{
    return (mask & gao->flags2 & 0xFF00) != 0;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsTest(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    if (AI_EvalFunc_OBJ_CustomBitsTest_C(gaoUltra, mask))
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return node + 1;
}

int AI_EvalFunc_OBJ_CustomBitsNot_C(tGameObjectMain* gao, int16_t mask)
{
    gao->flags2 ^= mask & 0xFF00;
    return gao->flags2;
}

tAINode* AI_EvalFunc_OBJ_CustomBitsNot(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_CustomBitsNot_C(gaoUltra, mask));
    return node + 1;
}

void AI_EvalFunc_OBJ_CapaSet_C(struct sGameObjectMain* gao, int16_t set, int16_t remove)
{
    gao->extended->capa &= ~remove;
    gao->extended->capa |= set;
}

void AI_EvalFunc_OBJ_CapaSet_C_CURRENT(int16_t set, int16_t remove)
{
    AI_EvalFunc_OBJ_CapaSet_C(AI_gpst_CurrentGameObject, set, remove);
}

tAINode* AI_EvalFunc_OBJ_CapaSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t set;
    int16_t remove;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    remove = AI_GetIntFromStack();
    set = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_CapaSet_C(gaoUltra, set, remove);
    return node + 1;
}

int AI_EvalFunc_OBJ_CapaGet_C(struct sGameObjectMain* gao)
{
    return gao->extended->capa;
}

tAINode* AI_EvalFunc_OBJ_CapaGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_CapaGet_C(gaoUltra));
    return node + 1;
}

BOOL AI_EvalFunc_OBJ_CapaTest_C(struct sGameObjectMain* gao, int16_t mask)
{
    if (!gao->extended)
        return 0;
    return (uint16_t)(mask & gao->extended->capa) != 0;
}

BOOL AI_EvalFunc_OBJ_CapaTest_C_CURRENT(int16_t mask)
{
    return AI_EvalFunc_OBJ_CapaTest_C(AI_gpst_CurrentGameObject, mask);
}

// Note that the argument is only popped when the object has an
// extended block, so a capability test on an object without one
// leaves the argument on the stack.
tAINode* AI_EvalFunc_OBJ_CapaTest(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (gaoUltra->extended)
    {
        mask = AI_GetIntFromStack();
        if (AI_EvalFunc_OBJ_CapaTest_C(gaoUltra, mask))
            AI_PutIntToStack(1);
        else
            AI_PutIntToStack(0);
    }
    else
    {
        AI_PutIntToStack(0);
    }
    return node + 1;
}

// Note that this toggles capa and then returns flags2.
int AI_EvalFunc_OBJ_CapaNot_C(struct sGameObjectMain* gao, int16_t mask)
{
    gao->extended->capa ^= mask;
    return gao->flags2;
}

tAINode* AI_EvalFunc_OBJ_CapaNot(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_CapaNot_C(gaoUltra, mask));
    return node + 1;
}

void AI_EvalFunc_OBJ_SightGeneralSet_C(struct sGameObjectMain* gao, Vec3* sight, Vec3* up)
{
    int hierarchical;
    Vec3 localPos;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    OBJ_SightGeneralSet(gao, sight, up);

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

tAINode* AI_EvalFunc_OBJ_SightGeneralSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 sight;
    Vec3 up;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&up);
    AI_GetVecFromStack(&sight);
    AI_EvalFunc_OBJ_SightGeneralSet_C(gaoUltra, &sight, &up);
    return node + 1;
}

void AI_EvalFunc_OBJ_SightSet_C(struct sGameObjectMain* gao, Vec3* sight)
{
    int hierarchical;
    Vec3 localPos;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    OBJ_SightSet(gao, sight);

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

tAINode* AI_EvalFunc_OBJ_SightSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 sight;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&sight);
    AI_EvalFunc_OBJ_SightSet_C(gaoUltra, &sight);
    return node + 1;
}

// An AI canal of -1 means the object itself rather than one of its
// bones.
void AI_EvalFunc_OBJ_BoneDisplayOrder_C(struct sGameObjectMain* gao, int aiCanal, char order)
{
    tGameObjectVisualObject* visual;

    if (aiCanal != -1)
        gao = ANI_pst_GetObjectByAICanal(gao, aiCanal);
    if (!gao)
        return;

    visual = AI_l_Visual(gao);
    if (visual)
        visual->boneDisplayOrder_zListBufferNum = order;
}

tAINode* AI_EvalFunc_OBJ_BoneDisplayOrder(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int aiCanal;
    char order;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    order = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_BoneDisplayOrder_C(gaoUltra, aiCanal, order);
    return node + 1;
}

// Orients the local matrix instead of the global one, which only means
// anything for an object in a hierarchy.
void AI_EvalFunc_OBJ_LocalSightSet_C(tGameObjectMain* gao, Vec3* sight, Vec3* up)
{
    if ((gao->flags & 0x400000) == 0)
        return;

    OBJ_SightGeneralSet(gao, sight, up);
    MATH_Copy33Matrix(&gao->basic->mtx->mtx, gao->matrices);
    OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

tAINode* AI_EvalFunc_OBJ_LocalSightSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 sight;
    Vec3 up;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&up);
    AI_GetVecFromStack(&sight);
    AI_EvalFunc_OBJ_LocalSightSet_C(gaoUltra, &sight, &up);
    return node + 1;
}

// Turns the object to face another, keeping world Z as up.
void AI_EvalFunc_OBJ_OrientTo_C(tGameObjectMain* gao, tGameObjectMain* target)
{
    int hierarchical;
    Vec3 sight;
    Vec3 up;
    Vec3 localPos;

    if (!target)
        return;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWhenHie(gao);
    }
    if ((target->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWhenHie(target);

    sight = target->matrices->Pos;
    MATH_SubstractVec(&sight, &sight, &gao->matrices->Pos);
    up.x = 0.0;
    up.y = 0.0;
    up.z = 1.0;
    OBJ_SightGeneralSet(gao, &sight, &up);

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

tAINode* AI_EvalFunc_OBJ_OrientTo(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_OrientTo_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

// The sight direction is the second row of the matrix, negated. An
// object in a hierarchy is composed into a scratch matrix rather than
// having its own touched.
void AI_EvalFunc_OBJ_SightGet_C(struct sGameObjectMain* gao, Vec3* sight)
{
    Vec3* row;
    Mtx4x4 global;

    if ((gao->flags & 0x400000) != 0)
    {
        OBJ_ComputeGlobalWithLocal(gao, &global, 1);
        row = &global.v2;
    }
    else
    {
        row = &gao->matrices->v2;
    }
    sight->x = -row->x;
    sight->y = -row->y;
    sight->z = -row->z;
}

void AI_EvalFunc_OBJ_SightGet_C_CURRENT(Vec3* sight)
{
    AI_EvalFunc_OBJ_SightGet_C(AI_gpst_CurrentGameObject, sight);
}

tAINode* AI_EvalFunc_OBJ_SightGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 sight;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_SightGet_C(gaoUltra, &sight);
    AI_PutVecToStack(&sight);
    return node + 1;
}

void AI_EvalFunc_OBJ_SightInitGet_C(struct sGameObjectMain* gao, Vec3* sight)
{
    if ((gao->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWhenHie(gao);
    OBJ_SightInitGet(gao, sight);
}

tAINode* AI_EvalFunc_OBJ_SightInitGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 sight;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_SightInitGet_C(gaoUltra, &sight);
    AI_PutVecToStack(&sight);
    return node + 1;
}

void AI_EvalFunc_OBJ_BankingGeneralSet_C(struct sGameObjectMain* gao, Vec3* banking, Vec3* up)
{
    int hierarchical;
    Vec3 localPos;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    OBJ_BankingGeneralSet(gao, banking, up);

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

void AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(Vec3* banking, Vec3* up)
{
    AI_EvalFunc_OBJ_BankingGeneralSet_C(AI_gpst_CurrentGameObject, banking, up);
}

tAINode* AI_EvalFunc_OBJ_BankingGeneralSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 banking;
    Vec3 up;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&up);
    AI_GetVecFromStack(&banking);
    AI_EvalFunc_OBJ_BankingGeneralSet_C(gaoUltra, &banking, &up);
    return node + 1;
}

void AI_EvalFunc_OBJ_BankingSet_C(struct sGameObjectMain* gao, Vec3* banking)
{
    int hierarchical;
    Vec3 localPos;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
    {
        localPos = gao->basic->mtx->mtx.Pos;
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    }

    OBJ_BankingSet(gao, banking);

    if (hierarchical)
    {
        OBJ_ComputeLocalWhenHie(gao);
        gao->basic->mtx->mtx.Pos = localPos;
    }
    if ((gao->flags & 0x4000000) != 0)
        ANI_UpdateFlash(gao, 3);
}

tAINode* AI_EvalFunc_OBJ_BankingSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 banking;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&banking);
    AI_EvalFunc_OBJ_BankingSet_C(gaoUltra, &banking);
    return node + 1;
}

// Banking is the third row of the matrix.
void AI_EvalFunc_OBJ_BankingGet_C(struct sGameObjectMain* gao, Vec3* banking)
{
    Mtx4x4 global;

    if ((gao->flags & 0x400000) != 0)
    {
        OBJ_ComputeGlobalWithLocal(gao, &global, 1);
        *banking = global.v3;
    }
    else
    {
        *banking = gao->matrices->v3;
    }
}

void AI_EvalFunc_OBJ_BankingGet_C_CURRENT(Vec3* banking)
{
    AI_EvalFunc_OBJ_BankingGet_C(AI_gpst_CurrentGameObject, banking);
}

tAINode* AI_EvalFunc_OBJ_BankingGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 banking;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_BankingGet_C(gaoUltra, &banking);
    AI_PutVecToStack(&banking);
    return node + 1;
}

// Note that this composes into the object's own matrix and does not
// put it back, unlike the other getters here.
void AI_EvalFunc_OBJ_HorizonGet_C(struct sGameObjectMain* gao, Vec3* horizon)
{
    if ((gao->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    OBJ_HorizonGet(gao, horizon);
}

void AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(Vec3* horizon)
{
    AI_EvalFunc_OBJ_HorizonGet_C(AI_gpst_CurrentGameObject, horizon);
}

tAINode* AI_EvalFunc_OBJ_HorizonGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 horizon;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_HorizonGet_C(gaoUltra, &horizon);
    AI_PutVecToStack(&horizon);
    return node + 1;
}

// Note that the scale is popped before the current object is saved,
// unlike every other wrapper in this file.
tAINode* AI_EvalFunc_OBJ_ScaleSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 scale;

    AI_GetVecFromStack(&scale);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    OBJ_ScaleSet(gaoUltra, &scale);
    return node + 1;
}

void AI_EvalFunc_OBJ_ScaleGet_C(struct sGameObjectMain* gao, Vec3* scale)
{
    MATH_GetScale(scale, gao->matrices);
}

tAINode* AI_EvalFunc_OBJ_ScaleGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 scale;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_ScaleGet_C(gaoUltra, &scale);
    AI_PutVecToStack(&scale);
    return node + 1;
}

void AI_EvalFunc_OBJ_ZoomSet_C(struct sGameObjectMain* gao, float zoom)
{
    int hierarchical;

    hierarchical = gao->flags & 0x400000;
    if (hierarchical)
        OBJ_ComputeGlobalWithLocal(gao, gao->matrices, 1);
    OBJ_ZoomSet(gao, zoom);
    if (hierarchical)
        OBJ_ComputeLocalWhenHie(gao);
}

tAINode* AI_EvalFunc_OBJ_ZoomSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float zoom;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    zoom = AI_GetFloatFromStack();
    AI_EvalFunc_OBJ_ZoomSet_C(gaoUltra, zoom);
    return node + 1;
}

// The zoom is the largest of the three scale components.
float AI_EvalFunc_OBJ_ZoomGet_C(struct sGameObjectMain* gao)
{
    Vec3 scale;

    MATH_GetScale(&scale, gao->matrices);
    return float_get_max_of_3vals_sub_48BBB0(scale.x, scale.y, scale.z);
}

tAINode* AI_EvalFunc_OBJ_ZoomGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_OBJ_ZoomGet_C(gaoUltra));
    return node + 1;
}

int AI_EvalFunc_OBJ_FlagsIdentityGet_C(struct sGameObjectMain* gao)
{
    if (!gao)
        return 0;
    return gao->flags;
}

tAINode* AI_EvalFunc_OBJ_FlagsIdentityGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsIdentityGet_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_OBJ_FlagsIdentitySet_C(struct sGameObjectMain* gao, int toSet, int toClear)
{
    int newFlags;

    if (!gao)
    {
        OBJ_ChangeIdentityFlags(0, toSet, 0);
        return;
    }
    newFlags = toSet | (~toClear & gao->flags);
    OBJ_ChangeIdentityFlags(gao, newFlags, gao->flags);
}

tAINode* AI_EvalFunc_OBJ_FlagsIdentitySet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int toSet;
    int toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagsIdentitySet_C(gaoUltra, toSet, toClear);
    return node + 1;
}

BOOL AI_EvalFunc_OBJ_FlagsIdentityTest_C(struct sGameObjectMain* gao, int mask)
{
    int flags;

    if (gao)
        flags = gao->flags;
    else
        flags = 0;
    return (mask & flags) != 0;
}

tAINode* AI_EvalFunc_OBJ_FlagsIdentityTest(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsIdentityTest_C(gaoUltra, mask));
    return node + 1;
}

int AI_EvalFunc_OBJ_FlagsDesignGet_C(struct sGameObjectMain* gao)
{
    if (!gao)
        return 0;
    return gao->designFlags;
}

tAINode* AI_EvalFunc_OBJ_FlagsDesignGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsDesignGet_C(gaoUltra));
    return node + 1;
}

int AI_EvalFunc_OBJ_FlagsDesignSet_C(struct sGameObjectMain* gao, char toSet, char toClear)
{
    uint8_t newFlags;

    if (!gao)
        return -1;
    newFlags = toSet | (~toClear & gao->designFlags);
    gao->designFlags = newFlags;
    return newFlags;
}

tAINode* AI_EvalFunc_OBJ_FlagsDesignSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char toSet;
    char toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_FlagsDesignSet_C(gaoUltra, toSet, toClear));
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_OBJ_FlagsFixSet_C(tGameObjectMain* gao, char toSet, char toClear)
{
    gao->process &= ~toClear;
    gao->process |= toSet;
    return gao;
}

tAINode* AI_EvalFunc_OBJ_FlagsFixSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char toSet;
    char toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_FlagsFixSet_C(gaoUltra, toSet, toClear);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_FlagsFixGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(gaoUltra->process);
    return node + 1;
}

// Only the two init bits of the process flags can be set this way.
tAINode* AI_EvalFunc_OBJ_FlagsFixSetInit(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    gaoUltra->process |= AI_GetIntFromStack() & 0xC;
    return node + 1;
}

// Copies the morph channels of one object onto another, channel by
// channel, as far as the shorter of the two reaches. A data index the
// destination cannot hold becomes -1.
void AI_EvalFunc_OBJ_MorphCopy_C(struct sGameObjectMain* gao, struct sGameObjectMain* src)
{
    tMorphMdfStruct* dstMorph;
    tMorphMdfStruct* srcMorph;
    tMorphMdfInner2Struct* dstChannel;
    tMorphMdfInner2Struct* srcChannel;
    int channels;
    int slots;
    int channel;
    int slot;

    dstMorph = GEO_pst_ModifierMorphing_Get(gao);
    if (!dstMorph)
        return;
    srcMorph = GEO_pst_ModifierMorphing_Get(src);
    if (!srcMorph)
        return;

    if (dstMorph->num2 <= srcMorph->num2)
        channels = dstMorph->num2;
    else
        channels = srcMorph->num2;

    for (channel = 0; channel < channels; ++channel)
    {
        dstChannel = &dstMorph->ptr2[channel];
        srcChannel = &srcMorph->ptr2[channel];

        dstChannel->Factor = srcChannel->Factor;
        dstChannel->Prog = srcChannel->Prog;

        if (dstChannel->num >= srcChannel->num)
            slots = srcChannel->num;
        else
            slots = dstChannel->num;

        for (slot = 0; slot < slots; ++slot)
        {
            dstChannel->ptr[slot] = srcChannel->ptr[slot];
            if (dstChannel->ptr[slot] >= dstMorph->num)
                dstChannel->ptr[slot] = -1;
        }
    }
}

tAINode* AI_EvalFunc_OBJ_MorphCopy(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_MorphCopy_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

int AI_EvalFunc_OBJ_NumberOfData_C(tGameObjectMain* gao)
{
    tMorphMdfStruct* morph;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return 0;
    return morph->num;
}

tAINode* AI_EvalFunc_OBJ_NumberOfData(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_NumberOfData_C(gaoUltra));
    return node + 1;
}

int AI_EvalFunc_OBJ_NumberOfChannel_C(tGameObjectMain* gao)
{
    tMorphMdfStruct* morph;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return 0;
    return morph->num2;
}

tAINode* AI_EvalFunc_OBJ_NumberOfChannel(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_NumberOfChannel_C(gaoUltra));
    return node + 1;
}

float AI_EvalFunc_OBJ_GetChannelFactor_C(struct sGameObjectMain* gao, int channel)
{
    tMorphMdfStruct* morph;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return 0.0;
    if (channel >= 0 && channel < morph->num2)
        return morph->ptr2[channel].Factor;
    return 0.0;
}

tAINode* AI_EvalFunc_OBJ_GetChannelFactor(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    channel = AI_GetIntFromStack();
    AI_PutFloatToStack(AI_EvalFunc_OBJ_GetChannelFactor_C(gaoUltra, channel));
    return node + 1;
}

void AI_EvalFunc_OBJ_SetChannelFactor_C(struct sGameObjectMain* gao, int channel, float factor)
{
    tMorphMdfStruct* morph;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (morph && channel >= 0 && channel < morph->num2)
        morph->ptr2[channel].Factor = factor;
}

tAINode* AI_EvalFunc_OBJ_SetChannelFactor(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;
    float factor;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    factor = AI_GetFloatFromStack();
    channel = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetChannelFactor_C(gaoUltra, channel, factor);
    return node + 1;
}

float AI_EvalFunc_OBJ_GetChannelProg_C(struct sGameObjectMain* gao, int channel)
{
    tMorphMdfStruct* morph;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return 0.0;
    if (channel >= 0 && channel < morph->num2)
        return morph->ptr2[channel].Prog;
    return 0.0;
}

tAINode* AI_EvalFunc_OBJ_GetChannelProg(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    channel = AI_GetIntFromStack();
    AI_PutFloatToStack(AI_EvalFunc_OBJ_GetChannelProg_C(gaoUltra, channel));
    return node + 1;
}

void AI_EvalFunc_OBJ_SetChannelProg_C(struct sGameObjectMain* gao, int channel, float prog)
{
    tMorphMdfStruct* morph;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (morph && channel >= 0 && channel < morph->num2)
        morph->ptr2[channel].Prog = prog;
}

tAINode* AI_EvalFunc_OBJ_SetChannelProg(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;
    float prog;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    prog = AI_GetFloatFromStack();
    channel = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetChannelProg_C(gaoUltra, channel, prog);
    return node + 1;
}

// Note that an out-of-range slot reads back the slot count rather
// than failing.
int AI_EvalFunc_OBJ_GetChannelData_C(struct sGameObjectMain* gao, int channel, int slot)
{
    tMorphMdfStruct* morph;
    tMorphMdfInner2Struct* morphChannel;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return 0;
    if (channel < 0 || channel >= morph->num2)
        return 0;

    morphChannel = &morph->ptr2[channel];
    if (slot >= 0 && slot < morphChannel->num)
        return morphChannel->ptr[slot];
    return morphChannel->num;
}

tAINode* AI_EvalFunc_OBJ_GetChannelData(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;
    int slot;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    slot = AI_GetIntFromStack();
    channel = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_GetChannelData_C(gaoUltra, channel, slot));
    return node + 1;
}

void AI_EvalFunc_OBJ_SetChannelData_C(struct sGameObjectMain* gao, int channel, int dataIdx, int slot)
{
    tMorphMdfStruct* morph;
    tMorphMdfInner2Struct* morphChannel;

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return;
    if (channel < 0 || channel >= morph->num2)
        return;

    morphChannel = &morph->ptr2[channel];
    if (slot >= 0 && slot < morphChannel->num && dataIdx < morph->num)
        morphChannel->ptr[slot] = dataIdx;
}

tAINode* AI_EvalFunc_OBJ_SetChannelData(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;
    int dataIdx;
    int slot;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    slot = AI_GetIntFromStack();
    dataIdx = AI_GetIntFromStack();
    channel = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetChannelData_C(gaoUltra, channel, dataIdx, slot);
    return node + 1;
}

// The same for one bone of the object, named by its AI canal, where
// -1 means the object itself.
void AI_EvalFunc_OBJ_SetModuleChannelData_C(tGameObjectMain* gao, int aiCanal, int channel, int slot, int dataIdx)
{
    tMorphMdfStruct* morph;
    tMorphMdfInner2Struct* morphChannel;

    if (aiCanal != -1)
    {
        gao = ANI_pst_GetObjectByAICanal(gao, aiCanal);
        if (!gao)
            return;
    }

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph)
        return;
    if (channel < 0 || channel >= morph->num2)
        return;

    morphChannel = &morph->ptr2[channel];
    if (slot >= 0 && slot < morphChannel->num && dataIdx < morph->num)
        morphChannel->ptr[slot] = dataIdx;
}

tAINode* AI_EvalFunc_OBJ_SetModuleChannelData(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int aiCanal;
    int channel;
    int slot;
    int dataIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    dataIdx = AI_GetIntFromStack();
    slot = AI_GetIntFromStack();
    channel = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_SetModuleChannelData_C(gaoUltra, aiCanal, channel, slot, dataIdx);
    return node + 1;
}

void AI_EvalFunc_OBJ_ResetModuleChannelData_C(tGameObjectMain* gao, int aiCanal, int channel)
{
    tMorphMdfStruct* morph;
    tMorphMdfInner2Struct* morphChannel;
    int slot;

    if (aiCanal != -1)
    {
        gao = ANI_pst_GetObjectByAICanal(gao, aiCanal);
        if (!gao)
            return;
    }

    morph = GEO_pst_ModifierMorphing_Get(gao);
    if (!morph || channel < 0 || channel >= morph->num2)
        return;

    morphChannel = &morph->ptr2[channel];
    for (slot = 0; slot < morphChannel->num; ++slot)
        morphChannel->ptr[slot] = -1;
}

tAINode* AI_EvalFunc_OBJ_ResetModuleChannelData(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int aiCanal;
    int channel;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    channel = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_ResetModuleChannelData_C(gaoUltra, aiCanal, channel);
    return node + 1;
}

// Drives a morph channel from the position of a second object: its x
// becomes the blend factor and its y the progress, so an animation
// moving that object animates the morph.
void AI_EvalFunc_OBJ_AnimMorphFactor_C(tGameObjectMain* driver, tGameObjectMain* gao, int aiCanal, int channel, int dataIdxAt0, int dataIdxAt1)
{
    if (aiCanal != -1)
    {
        gao = ANI_pst_GetObjectByAICanal(gao, aiCanal);
        if (!gao)
            return;
    }

    AI_EvalFunc_OBJ_SetChannelData_C(gao, channel, dataIdxAt0, 0);
    AI_EvalFunc_OBJ_SetChannelData_C(gao, channel, dataIdxAt1, 1);
    AI_EvalFunc_OBJ_SetChannelFactor_C(gao, channel, driver->matrices->Pos.x);
    AI_EvalFunc_OBJ_SetChannelProg_C(gao, channel, driver->matrices->Pos.y);
}

tAINode* AI_EvalFunc_OBJ_AnimMorphFactor(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int aiCanal;
    int channel;
    int dataIdxAt0;
    int dataIdxAt1;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    dataIdxAt1 = AI_GetIntFromStack();
    dataIdxAt0 = AI_GetIntFromStack();
    channel = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_AnimMorphFactor_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack(), aiCanal, channel, dataIdxAt0, dataIdxAt1);
    return node + 1;
}

// The same without touching the channel's data slots, and with the
// progress offset by one.
void AI_EvalFunc_OBJ_AnimMorphFactorNoKey_C(tGameObjectMain* driver, tGameObjectMain* gao, int aiCanal, int channel)
{
    float prog;

    if (aiCanal != -1)
    {
        gao = ANI_pst_GetObjectByAICanal(gao, aiCanal);
        if (!gao)
            return;
    }

    AI_EvalFunc_OBJ_SetChannelFactor_C(gao, channel, driver->matrices->Pos.x);
    prog = (float)(driver->matrices->Pos.y + 1.0);
    AI_EvalFunc_OBJ_SetChannelProg_C(gao, channel, prog);
}

tAINode* AI_EvalFunc_OBJ_AnimMorphFactorNoKey(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* gao;
    int aiCanal;
    int channel;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    channel = AI_GetIntFromStack();
    aiCanal = AI_GetIntFromStack();
    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    if (gao)
        AI_EvalFunc_OBJ_AnimMorphFactorNoKey_C(gaoUltra, gao, aiCanal, channel);
    return node + 1;
}

// Channel -1 and -2 address the two scalar fields of the RLI card
// modifier; anything else is one of its sixty-four colours.
void AI_EvalFunc_OBJ_RLICarteSet_C(struct sGameObjectMain* gao, unsigned int channel, int value)
{
    tRliCarteMdfStruct* rliCarte;
    tModifier* modifier;

    modifier = MDF_pst_GetByType(gao, 23);
    if (!modifier || !modifier->mdfPrivateDataPtr)
        return;

    rliCarte = (tRliCarteMdfStruct*)modifier->mdfPrivateDataPtr;
    if (channel == -1)
        rliCarte->b1 = value;
    else if (channel == -2)
        rliCarte->b2Max3 = value;
    else if (channel < 64)
        rliCarte->colors[channel] = value;
}

tAINode* AI_EvalFunc_OBJ_RLICarteSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int channel;
    int value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    value = AI_GetIntFromStack();
    channel = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_RLICarteSet_C(gaoUltra, channel, value);
    return node + 1;
}

// The visibility distance is kept as a byte over the range 0..1.
float AI_EvalFunc_OBJ_LodVisGet_C(struct sGameObjectMain* gao)
{
    float distance;

    distance = (float)gao->distance;
    return (float)(distance * 0.0039215689);   // 1/255
}

tAINode* AI_EvalFunc_OBJ_LodVisGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_OBJ_LodVisGet_C(gaoUltra));
    return node + 1;
}

// The level-of-detail distance is a byte on two scales: eight units a
// step up to 1024, and sixty-four a step above it.
float AI_EvalFunc_OBJ_LodDistGet_c_inner(uint8_t lodDist)
{
    if (lodDist <= 127u)
        return (float)(8 * lodDist);
    return (float)(((lodDist - 128) << 6) + 1024);
}

float AI_EvalFunc_OBJ_LodDistGet_C(tGameObjectMain* gao)
{
    return AI_EvalFunc_OBJ_LodDistGet_c_inner(gao->LODDist);
}

tAINode* AI_EvalFunc_OBJ_LodDistGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_OBJ_LodDistGet_C(gaoUltra));
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_OBJ_HierarchyGet_C(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x400000) == 0)
        return 0;
    return gao->basic->mtx->refGao2;
}

tAINode* AI_EvalFunc_OBJ_HierarchyGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* father;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    father = AI_EvalFunc_OBJ_HierarchyGet_C(gaoUltra);
    AI_PutObjectToStack(&father, AI_gaw_EnumLink[40]);
    return node + 1;
}

int AI_EvalFunc_OBJ_HierarchyGetChildren_C(tGameObjectMain* gao, tGameObjectMain** children)
{
    return WOR_i_GetAllChildrenOf(WOR_gpst_CurrentWorld, gao, children);
}

tAINode* AI_EvalFunc_OBJ_HierarchyGetChilds(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_OBJ_HierarchyGetChildren_C(gaoUltra, (tGameObjectMain**)var.ptr));
    return node + 1;
}

// Takes the object out of its hierarchy, putting its collision matrix
// back into world space on the way.
void AI_EvalFunc_OBJ_HierarchyReset_C(struct sGameObjectMain* gao)
{
    Mtx4x4 global;

    if (!gao->basic || !gao->basic->mtx)
        return;

    if ((gao->flags & 0x4000000) != 0)
        ANI_RemoveHierarchyOnFlash(gao);

    if ((gao->flags & 0x600) != 0)
    {
        MATH_MulMatrixMatrix(&global, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
        *gao->extended->coll->colInstance->mtx = global;
    }

    OBJ_ChangeIdentityFlags(gao, gao->flags & ~0x400000u, gao->flags);
    TAB_PFtable_RemoveElemWithPointer(&WOR_gpst_CurrentWorld->eot.objectsWithHiearchyTablePF, gao);
}

tAINode* AI_EvalFunc_OBJ_HierarchyReset(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_HierarchyReset_C(gaoUltra);
    return node + 1;
}

// Hangs the object under a new father and returns the old one. The
// collision matrix goes the other way here, into the father's space.
struct sGameObjectMain* AI_EvalFunc_OBJ_HierarchySet_C(struct sGameObjectMain* gao, struct sGameObjectMain* father)
{
    Mtx4x4 fatherInverse;
    Mtx4x4 local;
    tGameObjectMain* oldFather;
    tWorld* world;

    if (!father || father == gao)
        return 0;

    world = WOR_gpst_CurrentWorld;
    AI_EvalFunc_OBJ_HierarchyReset_C(gao);
    OBJ_ChangeIdentityFlags(gao, gao->flags | 0x400000, gao->flags);

    oldFather = gao->basic->mtx->refGao2;
    gao->basic->mtx->refGao2 = father;
    OBJ_ComputeLocalWhenHie(gao);

    if ((gao->flags & 0x600) != 0)
    {
        MATH_InvertMatrix(&fatherInverse, father->matrices);
        MATH_MulMatrixMatrix(&local, gao->extended->coll->colInstance->mtx, &fatherInverse);
        *gao->extended->coll->colInstance->mtx = local;
    }

    if ((gao->flags & 0x4000000) != 0)
        ANI_ApplyHierarchyOnFlash(gao);
    if (world)
        world->tablesManagerAndEOTNeeded = 1;

    return oldFather;
}

// Note that the hierarchy is reset here and again inside the call.
tAINode* AI_EvalFunc_OBJ_HierarchySet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* father;
    tGameObjectMain* oldFather;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    father = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalFunc_OBJ_HierarchyReset_C(gaoUltra);
    oldFather = AI_EvalFunc_OBJ_HierarchySet_C(gaoUltra, father);
    AI_PutObjectToStack(&oldFather, AI_gaw_EnumLink[40]);
    return node + 1;
}

// Swaps the father without touching the collision matrix or the
// world's tables, for an object already in a hierarchy.
struct sGameObjectMain* AI_EvalFunc_OBJ_HierarchySetFather_C(struct sGameObjectMain* gao, struct sGameObjectMain* father)
{
    tGameObjectMain* oldFather;

    if (father == gao)
        return 0;
    if ((gao->flags & 0x400000) == 0)
        return 0;

    oldFather = gao->basic->mtx->refGao2;
    gao->basic->mtx->refGao2 = father;
    OBJ_ComputeLocalWhenHie(gao);
    return oldFather;
}

tAINode* AI_EvalFunc_OBJ_HierarchySetFather(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* father;
    tGameObjectMain* oldFather;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    father = (tGameObjectMain*)AI_GetObjectFromStack();
    oldFather = AI_EvalFunc_OBJ_HierarchySetFather_C(gaoUltra, father);
    AI_PutObjectToStack(&oldFather, AI_gaw_EnumLink[40]);
    return node + 1;
}

// Moves the father so that this object lands on the given position,
// then recomputes every one of the father's children.
void AI_EvalFunc_OBJ_HierarchyMoveFather_C(tGameObjectMain* gao, Vec3* pos)
{
    tGameObjectMain* father;
    tGameObjectMain* child;
    Vec3 fatherPos;
    tableElemStructPF* elem;
    tableElemStructPF* end;

    if ((gao->flags & 0x400000) == 0)
        return;

    father = gao->basic->mtx->refGao2;
    if (!father)
        return;

    MATH_TransformVector(&fatherPos, OBJ_pst_GetAbsoluteMatrix(father), &gao->basic->mtx->mtx.Pos);
    MATH_SubstractVec(&fatherPos, pos, &fatherPos);
    OBJ_SetAbsolutePosition(father, &fatherPos);

    elem = WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr;
    end = WOR_gpst_CurrentWorld->objectTablePF.nextElemPtr;
    while (elem < end)
    {
        child = (tGameObjectMain*)elem->objectPtr;
        if (child && ((uintptr_t)elem->objectPtr & 3) != 1 &&
            (child->flags & 0x400000) != 0 && child->basic && child->basic->mtx &&
            child->basic->mtx->refGao2 == father)
        {
            if (child == gao)
                fatherPos.x = 0.0;   // dead store, kept as found
            OBJ_ComputeGlobalWhenHie(child);
        }
        ++elem;
    }
}

tAINode* AI_EvalFunc_OBJ_HierarchyMoveFather(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_HierarchyMoveFather_C(gaoUltra, (Vec3*)AI_PopStackSlot());
    return node + 1;
}

// True while the object is still in the world's object table.
tAINode* AI_EvalFunc_OBJ_IsValidGAO(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tableElemStructPF* elem;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    for (elem = WOR_gpst_CurrentWorld->objectTablePF.firstElemPtr;
         elem < (tableElemStructPF*)(WOR_gpst_CurrentWorld->objectTablePF.nextElemPtr);
         ++elem)
    {
        if (elem->objectPtr == gaoUltra)
        {
            AI_PutIntToStack(1);
            return node + 1;
        }
    }
    AI_PutIntToStack(0);
    return node + 1;
}

// Lends this object the material of another's visual. A multi-material
// is indexed by the slot, wrapping round.
void AI_EvalFunc_OBJ_AddMaterial_C(struct sGameObjectMain* gao, struct sGameObjectMain* from, int slot)
{
    tGEOMAT* material;

    if ((gao->flags & 0x1000) == 0 || (from->flags & 0x4000) == 0)
        return;

    if (slot == -1)
    {
        gao->basic->extraMaterial = 0;
        return;
    }

    material = (tGEOMAT*)(((tGameObjectVisualObject*)(from->basic->actorObjectOrVisualObject))->GROMAT);
    if (material && material->intf->id == 4)
        material = material->multiMaterialPtrIfType4ThenDiffuseColor[slot % material->multiMaterialNumIfType4ThenAmbiantColor];
    gao->basic->extraMaterial = material;
}

tAINode* AI_EvalFunc_OBJ_AddMaterial(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int slot;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    slot = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_AddMaterial_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack(), slot);
    return node + 1;
}

// Note that the object the builtin was called on plays no part.
void AI_EvalFunc_OBJ_RegisterKill_C(struct sGameObjectMain* gao, struct sGameObjectMain* toKill, int* flag)
{
    WOR_AddDelObj(WOR_gpst_CurrentWorld, toKill, flag);
}

// Note that both arguments are popped before the current object is
// saved, unlike most wrappers here.
tAINode* AI_EvalFunc_OBJ_RegisterKill(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* toKill;
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    toKill = (tGameObjectMain*)AI_GetObjectFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_RegisterKill_C(gaoUltra, toKill, (int*)var.ptr);
    return node + 1;
}

// Returns the distance to the position when it lies inside the cone of
// the object's sight, and zero when it does not. The flat flag drops
// the height from both the offset and the sight direction.
float AI_EvalFunc_OBJIsInCone_C(struct sGameObjectMain* gao, Vec3* pos, float cosHalfAngle, float range, int flat)
{
    Vec3 sight;
    Vec3 offset;
    float distance;
    float alignment;

    MATH_SubstractVec(&offset, pos, &gao->matrices->Pos);
    if (flat)
        offset.z = 0.0;

    distance = MATH_VecLen(&offset);
    if (distance > (double)range)
        return 0.0;

    MATH_NormalizeVector(&offset, &offset);
    OBJ_SightGet(gao, &sight);
    if (flat)
    {
        sight.z = 0.0;
        MATH_NormalizeVector(&sight, &sight);
    }

    alignment = sight.z * offset.z + sight.y * offset.y + offset.x * sight.x;
    if (alignment <= (double)cosHalfAngle)
        return 0.0;
    return distance;
}

tAINode* AI_EvalFunc_OBJIsInCone(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pos;
    float cosHalfAngle;
    float range;
    int flat;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    flat = AI_GetIntFromStack();
    range = AI_GetFloatFromStack();
    cosHalfAngle = AI_GetFloatFromStack();
    AI_GetVecFromStack(&pos);
    AI_PutFloatToStack(AI_EvalFunc_OBJIsInCone_C(gaoUltra, &pos, cosHalfAngle, range, flat));
    return node + 1;
}

// Solves a two-bone limb for the hip, knee and foot the caller names,
// then writes each joint back: into its local matrix where it hangs
// under a father, and into its own otherwise. A segment length of zero
// is taken from the joint's init position.
void AI_OBJIKFatherPosSet(tGameObjectMain* foot, tGameObjectMain* hip, tGameObjectMain* knee, float upperLen, float lowerLen)
{
    Mtx4x4 hipGlobal;
    Mtx4x4 kneeGlobal;
    Mtx4x4 footGlobal;
    Mtx4x4 fatherGlobal;
    Mtx4x4 fatherInverse;
    Mtx4x4* initMtx;
    tGameObjectMain* father;

    OBJ_ComputeGlobalWithLocal(hip, &hipGlobal, 1);
    OBJ_ComputeGlobalWithLocal(knee, &kneeGlobal, 1);
    OBJ_ComputeGlobalWithLocal(foot, &footGlobal, 1);

    if (upperLen == 0.0)
    {
        if ((knee->flags & 0x10000) != 0)
            initMtx = knee->matrices + 1;
        else
            initMtx = knee->matrices;
        upperLen = MATH_VecLen(&initMtx->Pos);
    }
    if (lowerLen == 0.0)
    {
        if ((foot->flags & 0x10000) != 0)
            initMtx = foot->matrices + 1;
        else
            initMtx = foot->matrices;
        lowerLen = MATH_VecLen(&initMtx->Pos);
    }

    GAO_ModifierLegLink_Compute(&hipGlobal, &kneeGlobal, &footGlobal, upperLen, lowerLen);

    if (hip && hip->basic && hip->basic->mtx)
        father = hip->basic->mtx->refGao2;
    else
        father = 0;
    if (father)
    {
        OBJ_ComputeGlobalWithLocal(father, &fatherGlobal, 1);
        MATH_InvertMatrix(&fatherInverse, &fatherGlobal);
        MATH_MulMatrixMatrix(&hip->basic->mtx->mtx, &hipGlobal, &fatherInverse);
        MATH_Orthonormalize(&hip->basic->mtx->mtx);
    }
    else
    {
        *hip->matrices = hipGlobal;
    }

    if (knee && knee->basic && knee->basic->mtx)
        father = knee->basic->mtx->refGao2;
    else
        father = 0;
    if (father)
    {
        // the knee hangs under the hip, so the hip's own global matrix
        // is what it is put back into
        MATH_InvertMatrix(&fatherInverse, &hipGlobal);
        MATH_MulMatrixMatrix(&knee->basic->mtx->mtx, &kneeGlobal, &fatherInverse);
        MATH_Orthonormalize(&knee->basic->mtx->mtx);
    }
    else
    {
        *knee->matrices = kneeGlobal;
    }

    if (foot && foot->basic && foot->basic->mtx)
        father = foot->basic->mtx->refGao2;
    else
        father = 0;
    if (father)
    {
        OBJ_ComputeGlobalWithLocal(father, &fatherGlobal, 1);
        MATH_InvertMatrix(&fatherInverse, &fatherGlobal);
        MATH_MulMatrixMatrix(&foot->basic->mtx->mtx, &footGlobal, &fatherInverse);
        MATH_Orthonormalize(&foot->basic->mtx->mtx);
    }
}

// Note that the two joints and the two lengths both arrive swapped.
void AI_EvalFunc_OBJIKFatherPosSetExt_C(tGameObjectMain* gao, tGameObjectMain* knee, tGameObjectMain* hip, float lowerLen, float upperLen)
{
    AI_OBJIKFatherPosSet(gao, hip, knee, upperLen, lowerLen);
}

tAINode* AI_EvalFunc_OBJIKFatherPosSetExt(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* hip;
    float lowerLen;
    float upperLen;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    upperLen = AI_GetFloatFromStack();
    lowerLen = AI_GetFloatFromStack();
    hip = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalFunc_OBJIKFatherPosSetExt_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack(), hip, lowerLen, upperLen);
    return node + 1;
}

// Takes the knee and the hip to be the object's father and
// grandfather, and both lengths from the init positions.
void AI_EvalFunc_OBJIKFatherPosSet_C(tGameObjectMain* gao)
{
    tGameObjectMain* knee;
    tGameObjectMain* hip;

    if (gao && gao->basic && gao->basic->mtx)
        knee = gao->basic->mtx->refGao2;
    else
        knee = 0;
    if (!knee)
        return;

    if (knee->basic && knee->basic->mtx)
        hip = knee->basic->mtx->refGao2;
    else
        hip = 0;
    if (hip)
        AI_OBJIKFatherPosSet(gao, hip, knee, 0.0, 0.0);
}

tAINode* AI_EvalFunc_OBJIKFatherPosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJIKFatherPosSet_C(gaoUltra);
    return node + 1;
}

// The limits within which a character's head and torso turn to follow
// what its eyes are on. These are engine-wide, not per object.
void AI_EvalFunc_OBJ_EyeFollowParamSet_C(float headZMax, float headZMin, float headXMax, float headXMin, float torsoZMax, float torsoZMin, float headXOffset)
{
    OBJ_gf_EYEFOLLOW_HeadZMax = headZMax;
    OBJ_gf_EYEFOLLOW_HeadZMin = headZMin;
    OBJ_gf_EYEFOLLOW_HeadXMax = headXMax;
    OBJ_gf_EYEFOLLOW_HeadXMin = headXMin;
    OBJ_gf_EYEFOLLOW_TorsoZMax = torsoZMax;
    OBJ_gf_EYEFOLLOW_TorsoZMin = torsoZMin;
    OBJ_gf_EYEFOLLOW_HeadXOffset = headXOffset;
}

tAINode* AI_EvalFunc_OBJ_EyeFollowParamSet(tAINode* node)
{
    float headZMax;
    float headZMin;
    float headXMax;
    float headXMin;
    float torsoZMax;
    float torsoZMin;
    float headXOffset;

    headXOffset = AI_GetFloatFromStack();
    torsoZMin = AI_GetFloatFromStack();
    torsoZMax = AI_GetFloatFromStack();
    headXMin = AI_GetFloatFromStack();
    headXMax = AI_GetFloatFromStack();
    headZMin = AI_GetFloatFromStack();
    headZMax = AI_GetFloatFromStack();
    AI_EvalFunc_OBJ_EyeFollowParamSet_C(headZMax, headZMin, headXMax, headXMin, torsoZMax, torsoZMin, headXOffset);
    return node + 1;
}

// The script passes one int holding seven switches in its top bits and
// a count in the low twenty-six.
void AI_EvalFunc_OBJEyeFollow_C(struct sGameObjectMain* gao, Vec3* target, float weight, int flags)
{
    OBJ_EyeFollow(
        gao,
        target,
        weight,
        flags & 0x3FFFFFF,
        flags & 0x80000000,
        flags & 0x40000000,
        flags & 0x20000000,
        flags & 0x10000000,
        flags & 0x8000000,
        flags & 0x4000000);
}

tAINode* AI_EvalFunc_OBJEyeFollow(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 target;
    float weight;
    int flags;

    flags = AI_GetIntFromStack();
    weight = AI_GetFloatFromStack();
    AI_GetVecFromStack(&target);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJEyeFollow_C(gaoUltra, &target, weight, flags);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJFreezeModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int mdfId;
    int setOrReset;

    setOrReset = AI_GetIntFromStack();
    mdfId = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    OBJ_FreezeModifier_C(gaoUltra, mdfId, setOrReset);
    return node + 1;
}

// Note that an object with no modifier list pushes a result and then
// falls through and pushes a second one.
tAINode* AI_EvalFunc_OBJModifierType(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int mdfId;

    mdfId = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (!gaoUltra->extended || !gaoUltra->extended->mdf)
        AI_PutIntToStack(0);
    AI_PutIntToStack(OBJ_ModifierType_C(gaoUltra, mdfId));
    return node + 1;
}

int AI_EvalFunc_OBJ_ColMapOptimize_C(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x100) == 0
        || !gao->extended
        || !gao->extended->coll->colMap
        || !gao->extended->coll->colMap->collisionObjectPtr)
    {
        return 0;
    }
    return (*gao->extended->coll->colMap->collisionObjectPtr)->flags & 2;
}

tAINode* AI_EvalFunc_OBJ_ColMapOptimize(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_ColMapOptimize_C(gaoUltra));
    return node + 1;
}

// Runs this object's dynamics, collision and recording for one frame
// out of turn, from the record mark the world is at.
void AI_EvalFunc_OBJEngineCall_C(struct sGameObjectMain* gao)
{
    tWorld* world;
    int recordMark;

    world = WOR_gpst_CurrentWorld;
    if (!world)
        return;

    recordMark = world->recordCount;
    DYN_OneCall(gao);
    COL_OneCall(gao, recordMark);
    REC_MainCall(world, recordMark);
}

tAINode* AI_EvalFunc_OBJEngineCall(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJEngineCall_C(gaoUltra);
    return node + 1;
}

// Asks the image analyser whether the object is in shot. A photo value
// of -1 means any, otherwise it is the top byte of the mask.
BOOL AI_EvalFunc_OBJHasPhotoValue_C(struct sGameObjectMain* gao, int photoValue, int* result)
{
    tGdi_DisplayData* displayData;
    int mask;

    if (!WOR_gpst_CurrentWorld)
        return 0;

    displayData = WOR_gpst_CurrentWorld->viewPtr->displayData;
    if (photoValue == -1)
        mask = -1;
    else
        mask = photoValue << 24;
    return IMG_ObjectAnalyser(gao, WOR_gpst_CurrentWorld, displayData, mask, 1, 0, 0, result) != 0;
}

tAINode* AI_EvalFunc_OBJHasPhotoValue(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tVarStackElem var;
    char converted[52];
    int photoValue;

    AI_GetVarFromStack(converted, &var);
    photoValue = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (AI_EvalFunc_OBJHasPhotoValue_C(gaoUltra, photoValue, (int*)var.ptr))
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return node + 1;
}

// The same analyser asked whether the object is centred in shot.
BOOL AI_EvalFunc_OBJCentred_C(tGameObjectMain* gao, int* result)
{
    if (!WOR_gpst_CurrentWorld)
        return 0;
    return IMG_ObjectAnalyser(gao, WOR_gpst_CurrentWorld, WOR_gpst_CurrentWorld->viewPtr->displayData, -1, 1, 0, 1, result) != 0;
}

tAINode* AI_EvalFunc_OBJCentred(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tVarStackElem var;
    char converted[52];

    AI_GetVarFromStack(converted, &var);
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (AI_EvalFunc_OBJCentred_C(gaoUltra, (int*)var.ptr))
        AI_PutIntToStack(1);
    else
        AI_PutIntToStack(0);
    return node + 1;
}

// The point a photo of this object is aimed at: the offset the photo
// modifier carries, or failing that the centre of the bounding volume
// of its first bone, or of the object itself.
void AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(struct sGameObjectMain* gao, Vec3* pivot)
{
    tModifier* photo;
    tGameObjectMain* snapGao;
    tGameObjectMain* bone;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (photo)
    {
        snapGao = GAO_ModifierPhoto_SnapGOGet(gao, (tPhotoMdfStruct*)photo->mdfPrivateDataPtr, 1);
        MATH_TransformVertex(pivot, snapGao->matrices, &(((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->infoOffset));
        return;
    }

    bone = 0;
    if ((gao->flags & 2) != 0)
        bone = ANI_pst_GetObjectByAICanal(gao, 0);
    if (bone)
        OBJ_BV_ComputeCenter(bone, pivot);
    else
        OBJ_BV_ComputeCenter(gao, pivot);
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoPivotGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pivot;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(gaoUltra, &pivot);
    AI_PutVecToStack(&pivot);
    return node + 1;
}

int AI_EvalFunc_OBJ_InfoPhotoMissionGet_C(struct sGameObjectMain* gao)
{
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (!photo)
        return -1;
    return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->mission;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoMissionGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_InfoPhotoMissionGet_C(gaoUltra));
    return node + 1;
}

int AI_EvalFunc_OBJ_InfoPhotoInfoGet_C(struct sGameObjectMain* gao)
{
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (!photo)
        return -1;
    return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->info;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoInfoGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_OBJ_InfoPhotoInfoGet_C(gaoUltra));
    return node + 1;
}

float AI_EvalFunc_OBJ_InfoPhotoLODGet_C(struct sGameObjectMain* gao, int which)
{
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (!photo)
        return -1.0f;

    switch (which)
    {
    case 0:
        return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->lod0_frameCullingSphereRet2;
    case 1:
        return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->lod1;
    case 2:
        return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->lod2;
    }
    return -1.0f;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoLODGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int which;

    which = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_OBJ_InfoPhotoLODGet_C(gaoUltra, which));
    return node + 1;
}

float AI_EvalFunc_OBJ_InfoPhotoFrameGet_C(struct sGameObjectMain* gao, int which)
{
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (!photo)
        return -1.0f;

    switch (which)
    {
    case 0:
        return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->frame0_frameCullingSphereRet1;
    case 1:
        return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->frame1;
    case 2:
        return ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->frame2;
    }
    return -1.0f;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoFrameGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int which;

    which = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_OBJ_InfoPhotoFrameGet_C(gaoUltra, which));
    return node + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet_C(struct sGameObjectMain* gao, Vec3* offset)
{
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (photo)
        ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->sphereOffset = *offset;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 offset;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&offset);
    AI_EvalFunc_OBJ_InfoPhotoSphereOffsetSet_C(gaoUltra, &offset);
    return node + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet_C(struct sGameObjectMain* gao, Vec3* offset)
{
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (photo)
        ((tPhotoMdfStruct*)(photo->mdfPrivateDataPtr))->infoOffset = *offset;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 offset;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&offset);
    AI_EvalFunc_OBJ_InfoPhotoInfoOffsetSet_C(gaoUltra, &offset);
    return node + 1;
}

// Creates the photo modifier if the object has none, appending it to
// the end of the modifier list. Note that the far frame is fixed.
void AI_EvalFunc_OBJ_InfoPhotoParamSet_C(struct sGameObjectMain* gao, int mission, int info, int aiCanal1, int aiCanal2, float lod1, float lod2, float frame1, float radius)
{
    tModifier** tail;
    tModifier* photo;
    tPhotoMdfStruct* params;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (!photo)
    {
        OBJ_GameObject_CreateExtendedIfNot(gao);
        for (tail = &gao->extended->mdf; *tail; tail = &(*tail)->nextElem)
            ;
        *tail = MDF_pst_Modifier_Create(gao, 20, 0);
        photo = *tail;
        photo->modifierFlags |= 0x1000000u;
        photo->gao = gao;
    }

    params = (tPhotoMdfStruct*)photo->mdfPrivateDataPtr;
    params->radius = radius;
    params->frame1 = frame1;
    params->frame2 = 1.001f;
    params->lod1 = lod1;
    params->lod2 = lod2;
    params->aiCanal1 = aiCanal1;
    params->aiCanal2 = aiCanal2;
    params->info = info;
    params->mission = mission;
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoParamSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int mission;
    int info;
    int aiCanal1;
    int aiCanal2;
    float lod1;
    float lod2;
    float frame1;
    float radius;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    radius = AI_GetFloatFromStack();
    frame1 = AI_GetFloatFromStack();
    lod2 = AI_GetFloatFromStack();
    lod1 = AI_GetFloatFromStack();
    aiCanal2 = AI_GetIntFromStack();
    aiCanal1 = AI_GetIntFromStack();
    info = AI_GetIntFromStack();
    mission = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_InfoPhotoParamSet_C(gaoUltra, mission, info, aiCanal1, aiCanal2, lod1, lod2, frame1, radius);
    return node + 1;
}

void AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(struct sGameObjectMain* gao, int toSet, int toClear)
{
    tPhotoMdfStruct* params;
    tModifier* photo;

    photo = GAO_ModifierPhoto_Get(gao, 0);
    if (photo)
    {
        params = (tPhotoMdfStruct*)photo->mdfPrivateDataPtr;
        params->flags |= toSet;
        params->flags &= ~toClear;
    }
}

tAINode* AI_EvalFunc_OBJ_InfoPhotoFlagsSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int toSet;
    int toClear;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    toClear = AI_GetIntFromStack();
    toSet = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_InfoPhotoFlagsSet_C(gaoUltra, toSet, toClear);
    return node + 1;
}

