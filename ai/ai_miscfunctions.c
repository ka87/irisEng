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

void AI_EvalFunc_GRPInvisibleSet_C(struct sGameObjectMain* a1, int a2)
{
    tGroup* v3; 
    tableElemStructPF* v4;
    tableElemStructPF* v5; 

    if ((a1->flags & 0x800000) != 0 && a1->extended && a1->extended->grp)
    {
        v3 = a1->extended->grp;
        v4 = v3->tablePF->firstElemPtr;
        v5 = ((tableElemStructPF*)(v3->tablePF->nextElemPtr)) - 1;
        while (v4 <= v5)
        {
            if (v4->objectPtr && ((int)v4->objectPtr & 3) != 1)
                AI_EvalFunc_OBJ_FlagInvisibleSet_C((tGameObjectMain*)v4->objectPtr, a2);
            ++v4;
        }
    }
}

tAINode* AI_EvalFunc_GRPInvisibleSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GRPInvisibleSet_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_GRPInactiveSet_C(struct sGameObjectMain* a1, int a2)
{
    tGroup* v3;
    tableElemStructPF* v4;
    tableElemStructPF* v5;

    if ((a1->flags & 0x800000) != 0 && a1->extended && a1->extended->grp)
    {
        v3 = a1->extended->grp;
        v4 = v3->tablePF->firstElemPtr;
        v5 = ((tableElemStructPF*)(v3->tablePF->nextElemPtr)) - 1;
        while (v4 <= v5)
        {
            if (v4->objectPtr && ((int)v4->objectPtr & 3) != 1)
                AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)v4->objectPtr, a2);
            ++v4;
        }
    }
}

tAINode* AI_EvalFunc_GRPInactiveSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GRPInactiveSet_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_Gsp_AE_MASTER_C(int a1, int a2, int a3, float a4)
{
    PC_AE_MASTER_CTRL(a1, a2, a3, a4);
}

tAINode* AI_EvalFunc_Gsp_AE_MASTER(tAINode* a1)
{
    int v2;
    float v3;
    int v4;
    int v5;

    v3 = AI_GetFloatFromStack();
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v2 = AI_GetIntFromStack();
    AI_EvalFunc_Gsp_AE_MASTER_C(v2, v4, v5, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETONOFF(tAINode* a1)
{
    int v1; 
    int v2; 

    v1 = AI_GetIntFromStack();
    v2 = PC_AE_MASTER_GET_ONOFF_sub_4A37D0(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_Gsp_AE_MASTER_GETP1(tAINode* a1)
{
    int v1;
    int v3;
    float v4;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v4 = PC_AE_MASTER_GET_P1(v1, v3);
    AI_PutFloatToStack(v4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_Gsp_AE_MASTEREVT(tAINode* a1)
{
    int v2;
    int v3;
    int v4;
    tGameObjectMain* v5;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v5 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v2 = AI_GetIntFromStack();
    PC_AE_MASTER_CTRL(v2, v3, v4, v5->matrices->Pos.x);
    return a1 + 1;
}

tAINode* AI_EvalFunc_CIN_OnOff(tAINode* a1)
{
    AI_GetIntFromStack();
    AI_EvalFunc_DbgTraceInt_C();
    return a1 + 1;
}

// Raises or lowers the cinematic bands. Called by the cutscene
// behaviours at the two ends of a cine. See
// ai/notes/cin-cutscene-mode.md.
void AI_EvalFunc_CIN_CutsceneMode_C(int a1)
{
    PC_CIN_CutsceneMode(a1);
}

tAINode* AI_EvalFunc_CIN_CutsceneMode(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    AI_EvalFunc_CIN_CutsceneMode_C(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_DBR_GetN(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_IoConsLangSet_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_ConsoleSpecificOption(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = ConsoleSpecificOption(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_XMEN_OnOff(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    XMEN_OnOff(v2, a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_XMEN_SetSize(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    float a4;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetFloatFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    XMEN_SetSize(v2, a2, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_XMEC_OnOff(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    XMEC_OnOff(v2, a2, a3);
    return a1 + 1;
}

void AI_EvalFunc_SNAKE_SetInertia_C(struct sGameObjectMain* a1, float a2)
{
    BOOL v2;
    tModifier* i;
    tableElemStructPF* v4;
    tableElemStructPF* v5;
    tGroup* v6;
    tGameObjectMain* v7;

    v6 = 0;
    v4 = 0;
    v5 = 0;
    if (a1)
    {
        if (a1->basic
            && a1->basic->actorObjectOrVisualObject
            && (a1->flags & 2) != 0
            && ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            v6 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup;
            v5 = ((tableElemStructPF*)(v6->tablePF->nextElemPtr)) - 1;
        }
        while (v4 <= v5)
        {
            if (v4)
            {
                v7 = (tGameObjectMain*)v4->objectPtr;
            }
            else
            {
                v7 = a1;
                if (v5)
                    v4 = ((tableElemStructPF*)(v6->tablePF->firstElemPtr)) - 1;
            }
            v2 = !v7 || ((uint8_t)v7 & 3) == 1;
            if (!v2 && v7->extended && v7->extended->mdf)
            {
                for (i = v7->extended->mdf; i; i = i->nextElem)
                {
                    if (i->modifierIntf->mdfId == 14)
                        ((tSnakeMdfStruct*)(i->mdfPrivateDataPtr))->inertia = a2;
                }
            }
            ++v4;
        }
    }
}

tAINode* AI_EvalFunc_SNAKE_SetInertia(tAINode* a1)
{
    tGameObjectMain* v2; 
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_SNAKE_SetInertia_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_SPG_SetNoise_C(struct sGameObjectMain* a1, float a2)
{
    tModifier* i;

    if (a1->extended)
    {
        for (i = a1->extended->mdf; i; i = i->nextElem)
        {
            if (i->modifierIntf->mdfId == 11)
                ((tSpgMdfStruct*)(i->mdfPrivateDataPtr))->noise = a2;
        }
    }
}

tAINode* AI_EvalFunc_SPG_SetNoise(tAINode* a1)
{
    tGameObjectMain* v2;
    float a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_SPG_SetNoise_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_SDW_Activate_C(struct sGameObjectMain* a1, int a2)
{
    unsigned int v2;
    tMdfPrivateDataShadow* v3;
    tModifier* i;

    if (a1->extended)
    {
        for (i = a1->extended->mdf; i; i = i->nextElem)
        {
            if (i->modifierIntf->mdfId == 6)
            {
                v3 = (tMdfPrivateDataShadow*)(i->mdfPrivateDataPtr);
                if (a2)
                    v2 = v3->flags | 0x40000;
                else
                    v2 = v3->flags & ~0x40000u;
                v3->flags = v2;
            }
        }
    }
}

tAINode* AI_EvalFunc_SDW_Activate(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_SDW_Activate_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_WTR_GetDif_C(Vec3* a1, Vec3* a2)
{
    WTR_Get_A_DifVector(a1, a2, 1);
}

tAINode* AI_EvalFunc_WTR_GetDif(tAINode* a1)
{
    Vec3 a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_WTR_GetDif_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], &a2);
    AI_PutVecToStack(&a2);
    return a1 + 1;
}

void AI_EvalFunc_WTR_GetNrm_C(Vec3* a1, Vec3* a2)
{
    WTR_Get_A_MrmVector(a1, a2);
}

tAINode* AI_EvalFunc_WTR_GetNrm(tAINode* arg0)
{
    Vec3 a1;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_WTR_GetNrm_C((Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], &a1);
    AI_PutVecToStack(&a1);
    return arg0 + 1;
}

void AI_EvalFunc_WTR_Enable_C(int a1)
{
    if ((!stExportWaterParrams.waterEnabled || !a1) && (stExportWaterParrams.waterEnabled || a1))
    {
        stExportWaterParrams.waterEnabled = a1;
        WTR_SetExportParams(&stExportWaterParrams);
    }
}

tAINode* AI_EvalFunc_WTR_Enable(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WTR_Enable_C(v1);
    return a1 + 1;
}

void AI_EvalFunc_WTR_SetF_C(int a1, float a2)
{
    float* v2;

    v2 = &stExportWaterParrams.waveHeight + a1;
    if (v2 <= &stExportWaterParrams.waveProp)
    {
        *v2 = a2;
        WTR_SetExportParams(&stExportWaterParrams);
    }
}

tAINode* AI_EvalFunc_WTR_SetF(tAINode* a1)
{
    int v1;
    float a2; 

    a2 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WTR_SetF_C(v1, a2);
    return a1 + 1;
}

void AI_EvalFunc_WTR_SetL_C(int a1, int a2)
{
    int* v2;

    v2 = &stExportWaterParrams.skyColor + a1;
    if (v2 < (int*)&stExportWaterParrams.waveHeight)
    {
        *v2 = a2;
        WTR_SetExportParams(&stExportWaterParrams);
    }
}

tAINode* AI_EvalFunc_WTR_SetL(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_WTR_SetL_C(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_pop_one_elem_put_zero_sub_4B4280(tAINode* a1)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(0);
    return a1 + 1;
}

tAINode* AI_EvalFunc_WTR_put_zero_no_pop_sub_4B42C0(tAINode* a1)
{
    AI_PutIntToStack(0);
    return a1 + 1;
}