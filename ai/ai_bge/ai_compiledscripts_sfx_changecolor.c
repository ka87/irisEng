#include "ai_compiledscripts_inc.h"

int SFX_ChangeColor_AfterDisplay()
{
    tScriptStruct_SFXChangeColor* v1;

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXChangeColor*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_OldColor, v1->i_SubMaterial);
    return 1000;
}

int SFX_ChangeColor_BeforeDisplay()
{
    tScriptStruct_SFXChangeColor* v1;

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SFXChangeColor*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    v1->c_OldColor = AI_EvalFunc_MATGetConstant_C(AI_gpst_CurrentGameObject, v1->i_SubMaterial);
    AI_EvalFunc_MATSetConstant_C(AI_gpst_CurrentGameObject, v1->c_Color, v1->i_SubMaterial);
    return 1000;
}