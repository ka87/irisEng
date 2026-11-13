#include "ai_gameobjectfunctions.h"
#include "../win/wingdi.h"
#include "../sound.h"
#include "ai_debugfunctions.h"
#include "ai_inofunctions.h"
#include "../modifiers/xmenmodifier.h"
#include "../modifiers/xmecmodifier.h"
#include "../modifiers/snakemodifier.h"
#include "../modifiers/spritegenmodifier.h"
#include "../modifiers/shadowmodifier.h"
#include "../water.h"
#include "ai_miscfunctions.h"

// A table entry is in use when it has a pointer and its low two bits
// do not carry the free tag 01.
static BOOL AI_l_ObjectInUse(void* objectPtr)
{
    return objectPtr && ((uintptr_t)objectPtr & 3) != 1;
}

// The two group builtins differ only in which per-object flag setter
// they call for each member.
static void AI_l_ApplyToGroupMembers(struct sGameObjectMain* gao, int value,
                                     void (*apply)(struct sGameObjectMain*, int))
{
    tGroup* group;
    tableElemStructPF* entry;
    tableElemStructPF* end;

    if ((gao->flags & 0x800000) == 0 || !gao->extended || !gao->extended->grp)
        return;

    group = gao->extended->grp;
    end = (tableElemStructPF*)(group->tablePF->nextElemPtr);
    for (entry = group->tablePF->firstElemPtr; entry < end; ++entry)
    {
        if (AI_l_ObjectInUse(entry->objectPtr))
            apply((tGameObjectMain*)entry->objectPtr, value);
    }
}

void AI_EvalFunc_GRPInvisibleSet_C(struct sGameObjectMain* gao, int invisible)
{
    AI_l_ApplyToGroupMembers(gao, invisible, AI_EvalFunc_OBJ_FlagInvisibleSet_C);
}

tAINode* AI_EvalFunc_GRPInvisibleSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int invisible;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    invisible = AI_GetIntFromStack();
    AI_EvalFunc_GRPInvisibleSet_C(gaoUltra, invisible);
    return node + 1;
}

void AI_EvalFunc_GRPInactiveSet_C(struct sGameObjectMain* gao, int inactive)
{
    AI_l_ApplyToGroupMembers(gao, inactive, AI_EvalFunc_OBJ_FlagInactiveSet_C);
}

tAINode* AI_EvalFunc_GRPInactiveSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int inactive;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    inactive = AI_GetIntFromStack();
    AI_EvalFunc_GRPInactiveSet_C(gaoUltra, inactive);
    return node + 1;
}

void AI_EvalFunc_Gsp_AE_MASTER_C(int effect, int param, int onOff, float value)
{
    PC_AE_MASTER_CTRL(effect, param, onOff, value);
}

tAINode* AI_EvalFunc_Gsp_AE_MASTER(tAINode* node)
{
    int effect;
    int param;
    int onOff;
    float value;

    value = AI_GetFloatFromStack();
    onOff = AI_GetIntFromStack();
    param = AI_GetIntFromStack();
    effect = AI_GetIntFromStack();
    AI_EvalFunc_Gsp_AE_MASTER_C(effect, param, onOff, value);
    return node + 1;
}

tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETONOFF(tAINode* node)
{
    int effect;

    effect = AI_GetIntFromStack();
    AI_PutIntToStack(PC_AE_MASTER_GET_ONOFF(effect));
    return node + 1;
}

tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETP1(tAINode* node)
{
    int effect;
    int param;

    param = AI_GetIntFromStack();
    effect = AI_GetIntFromStack();
    AI_PutFloatToStack(PC_AE_MASTER_GET_P1(effect, param));
    return node + 1;
}

// Same control call as Gsp_AE_MASTER, with the value taken from an
// object's position rather than off the stack.
tAINode* AI_EvalFunc_Gsp_AE_MASTEREVT(tAINode* node)
{
    tGameObjectMain* gao;
    int effect;
    int param;
    int onOff;

    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    onOff = AI_GetIntFromStack();
    param = AI_GetIntFromStack();
    effect = AI_GetIntFromStack();
    PC_AE_MASTER_CTRL(effect, param, onOff, gao->matrices->Pos.x);
    return node + 1;
}

tAINode* AI_EvalFunc_CIN_OnOff(tAINode* node)
{
    AI_GetIntFromStack();
    AI_EvalFunc_DbgTraceInt_C();
    return node + 1;
}

// Raises or lowers the cinematic bands.
void AI_EvalFunc_CIN_CutsceneMode_C(int onOff)
{
    PC_CIN_CutsceneMode(onOff);
}

tAINode* AI_EvalFunc_CIN_CutsceneMode(tAINode* node)
{
    int onOff;

    onOff = AI_GetIntFromStack();
    AI_EvalFunc_CIN_CutsceneMode_C(onOff);
    return node + 1;
}

tAINode* AI_EvalFunc_DBR_GetN(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_IoConsLangSet_C());
    return node + 1;
}

tAINode* AI_EvalFunc_ConsoleSpecificOption(tAINode* node)
{
    int option;

    option = AI_GetIntFromStack();
    AI_PutIntToStack(ConsoleSpecificOption(option));
    return node + 1;
}

tAINode* AI_EvalFunc_XMEN_OnOff(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int boneNum;
    int onOff;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    onOff = AI_GetIntFromStack();
    boneNum = AI_GetIntFromStack();
    XMEN_OnOff(gaoUltra, boneNum, onOff);
    return node + 1;
}

tAINode* AI_EvalFunc_XMEN_SetSize(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int boneNum;
    int axis;
    float size;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    size = AI_GetFloatFromStack();
    axis = AI_GetIntFromStack();
    boneNum = AI_GetIntFromStack();
    XMEN_SetSize(gaoUltra, boneNum, axis, size);
    return node + 1;
}

tAINode* AI_EvalFunc_XMEC_OnOff(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int boneNum;
    int onOff;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    onOff = AI_GetIntFromStack();
    boneNum = AI_GetIntFromStack();
    XMEC_OnOff(gaoUltra, boneNum, onOff);
    return node + 1;
}

static void AI_l_SetSnakeInertia(tGameObjectMain* gao, float inertia)
{
    tModifier* mdf;

    if (!AI_l_ObjectInUse(gao) || !gao->extended || !gao->extended->mdf)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == 14)
            ((tSnakeMdfStruct*)(mdf->mdfPrivateDataPtr))->inertia = inertia;
    }
}

// The object itself first, then every member of its skeleton group.
void AI_EvalFunc_SNAKE_SetInertia_C(struct sGameObjectMain* gao, float inertia)
{
    tGroup* skeleton;
    tableElemStructPF* entry;
    tableElemStructPF* end;

    if (!gao)
        return;

    AI_l_SetSnakeInertia(gao, inertia);

    if (!gao->basic || !gao->basic->actorObjectOrVisualObject || (gao->flags & 2) == 0)
        return;

    skeleton = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup;
    if (!skeleton)
        return;

    end = (tableElemStructPF*)(skeleton->tablePF->nextElemPtr);
    for (entry = skeleton->tablePF->firstElemPtr; entry < end; ++entry)
        AI_l_SetSnakeInertia((tGameObjectMain*)entry->objectPtr, inertia);
}

tAINode* AI_EvalFunc_SNAKE_SetInertia(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float inertia;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    inertia = AI_GetFloatFromStack();
    AI_EvalFunc_SNAKE_SetInertia_C(gaoUltra, inertia);
    return node + 1;
}

void AI_EvalFunc_SPG_SetNoise_C(struct sGameObjectMain* gao, float noise)
{
    tModifier* mdf;

    if (!gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == 11)
            ((tSpgMdfStruct*)(mdf->mdfPrivateDataPtr))->noise = noise;
    }
}

tAINode* AI_EvalFunc_SPG_SetNoise(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float noise;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    noise = AI_GetFloatFromStack();
    AI_EvalFunc_SPG_SetNoise_C(gaoUltra, noise);
    return node + 1;
}

void AI_EvalFunc_SDW_Activate_C(struct sGameObjectMain* gao, int activate)
{
    tMdfPrivateDataShadow* shadow;
    tModifier* mdf;

    if (!gao->extended)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId != 6)
            continue;

        shadow = (tMdfPrivateDataShadow*)(mdf->mdfPrivateDataPtr);
        if (activate)
            shadow->flags |= 0x40000;
        else
            shadow->flags &= ~0x40000u;
    }
}

tAINode* AI_EvalFunc_SDW_Activate(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int activate;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    activate = AI_GetIntFromStack();
    AI_EvalFunc_SDW_Activate_C(gaoUltra, activate);
    return node + 1;
}

void AI_EvalFunc_WTR_GetDif_C(Vec3* pos, Vec3* dif)
{
    WTR_Get_A_DifVector(pos, dif, 1);
}

tAINode* AI_EvalFunc_WTR_GetDif(tAINode* node)
{
    Vec3 dif;

    // the position is read where the stack keeps it rather than copied out
    AI_EvalFunc_WTR_GetDif_C((Vec3*)AI_PopStackSlot(), &dif);
    AI_PutVecToStack(&dif);
    return node + 1;
}

void AI_EvalFunc_WTR_GetNrm_C(Vec3* pos, Vec3* nrm)
{
    WTR_Get_A_MrmVector(pos, nrm);
}

tAINode* AI_EvalFunc_WTR_GetNrm(tAINode* node)
{
    Vec3 nrm;

    AI_EvalFunc_WTR_GetNrm_C((Vec3*)AI_PopStackSlot(), &nrm);
    AI_PutVecToStack(&nrm);
    return node + 1;
}

// Only a change is exported; setting the state it already has does
// nothing.
void AI_EvalFunc_WTR_Enable_C(int enable)
{
    if ((stExportWaterParrams.waterEnabled != 0) == (enable != 0))
        return;

    stExportWaterParrams.waterEnabled = enable;
    WTR_SetExportParams(&stExportWaterParrams);
}

tAINode* AI_EvalFunc_WTR_Enable(tAINode* node)
{
    int enable;

    enable = AI_GetIntFromStack();
    AI_EvalFunc_WTR_Enable_C(enable);
    return node + 1;
}

// The float parameters are addressed by their position in the export
// block, from waveHeight to waveProp. Only the upper end is checked.
void AI_EvalFunc_WTR_SetF_C(int paramIdx, float value)
{
    float* param;

    param = &stExportWaterParrams.waveHeight + paramIdx;
    if (param > &stExportWaterParrams.waveProp)
        return;

    *param = value;
    WTR_SetExportParams(&stExportWaterParrams);
}

tAINode* AI_EvalFunc_WTR_SetF(tAINode* node)
{
    int paramIdx;
    float value;

    value = AI_GetFloatFromStack();
    paramIdx = AI_GetIntFromStack();
    AI_EvalFunc_WTR_SetF_C(paramIdx, value);
    return node + 1;
}

// The same for the integer parameters, which run from skyColor up to
// where the floats begin.
void AI_EvalFunc_WTR_SetL_C(int paramIdx, int value)
{
    int* param;

    param = &stExportWaterParrams.skyColor + paramIdx;
    if (param >= (int*)&stExportWaterParrams.waveHeight)
        return;

    *param = value;
    WTR_SetExportParams(&stExportWaterParrams);
}

tAINode* AI_EvalFunc_WTR_SetL(tAINode* node)
{
    int paramIdx;
    int value;

    value = AI_GetIntFromStack();
    paramIdx = AI_GetIntFromStack();
    AI_EvalFunc_WTR_SetL_C(paramIdx, value);
    return node + 1;
}

tAINode* AI_EvalFunc_PopArgPutZero(tAINode* node)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(0);
    return node + 1;
}

tAINode* AI_EvalFunc_WTR_PutZero(tAINode* node)
{
    AI_PutIntToStack(0);
    return node + 1;
}

/* PC-version adjustments. Each Compute entry is a hook with nothing in
   it; each Check reports the value the PC build uses. The argument a
   Check takes is popped and discarded except in Price_Check, which
   compares both of its own. */

tAINode* AI_EvalFunc_BGE_PC_Price_Compute(tAINode* node)
{
    return node + 1;
}

int AI_EvalFunc_BGE_PC_Price_Check_C(int item, int kind)
{
    if (item == 6 && kind == 3)
        return 10;

    return 0;
}

tAINode* AI_EvalFunc_BGE_PC_Price_Check(tAINode* node)
{
    int item;
    int kind;

    kind = AI_GetIntFromStack();
    item = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_BGE_PC_Price_Check_C(item, kind));
    return node + 1;
}

tAINode* AI_EvalFunc_BGE_PC_Samsue_Compute(tAINode* node)
{
    return node + 1;
}

int AI_EvalFunc_BGE_PC_Samsue_Check_C()
{
    return 0;
}

tAINode* AI_EvalFunc_BGE_PC_Samsue_Check(tAINode* node)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_BGE_PC_Samsue_Check_C());
    return node + 1;
}

tAINode* AI_EvalFunc_BGE_PC_Radio_Compute(tAINode* node)
{
    return node + 1;
}

int AI_EvalFunc_BGE_PC_Radio_Check_C()
{
    return 90;
}

tAINode* AI_EvalFunc_BGE_PC_Radio_Check(tAINode* node)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_BGE_PC_Radio_Check_C());
    return node + 1;
}

tAINode* AI_EvalFunc_BGE_PC_Robot_Compute(tAINode* node)
{
    return node + 1;
}

int AI_EvalFunc_BGE_PC_Robot_Check_C()
{
    return 1;
}

tAINode* AI_EvalFunc_BGE_PC_Robot_Check(tAINode* node)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_BGE_PC_Robot_Check_C());
    return node + 1;
}
