#include "ai_compiledscripts_inc.h"

//main_menu_mn_init
int main_menu_mn_init_INV_MatFade_gao;

int main_menu_mn_init()
{
    int result; // eax
    tGameObjectMain* v1; // [esp+8h] [ebp-14h]
    int v2; // [esp+10h] [ebp-Ch]
    int a1; // [esp+18h] [ebp-4h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_menu_mn_init_INV_MatFade_gao = AI_ul_RealResolveOneRefObject(0x5E006934u);
    }
    AI_EvalFunc_GFXDel_C_CURRENT(10);
    a1 = AI_EvalFunc_GFXAddSorted_C(AI_gpst_CurrentGameObject, 11, 10);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 0, 1);
    AI_EvalFunc_GFXFlagSet_C_CURRENT(a1, 2, 1);
    v1 = (tGameObjectMain*)main_menu_mn_init_INV_MatFade_gao;
    if (main_menu_mn_init_INV_MatFade_gao)
    {
        if ((unsigned int)main_menu_mn_init_INV_MatFade_gao <= 2)
            v1 = AI_MainActor[main_menu_mn_init_INV_MatFade_gao - 1];
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, v1, 9);
    }
    else
    {
        AI_EvalFunc_GFXMaterialSet_C_CURRENT(a1, 0, 9);
    }
    GFXSetCurGlobal(a1);
    AI_EvalFunc_GFXSeti_C_CURGLOB(11100, 0xFF000000);
    AI_EvalFunc_GFXSeti_C_CURGLOB(11101, 0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(11000, 0.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(11001, 1.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(11002, 2.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(11003, 0.0);
    AI_EvalFunc_GFXSetf_C_CURGLOB(11004, 0.0);
    v2 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
    if (v2 == 1000)
        result = 1000;
    else
        result = v2;
    return result;
}