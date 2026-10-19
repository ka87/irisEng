#include "ai_compiledscripts_inc.h"

//INV_cacheobjet_loop
int INV_cacheobjet_loop_INV_manager_gao;
int INV_cacheobjet_loop_INV_manager_gao_ao_GameText;

int INV_cacheobjet_loop()
{
    tScriptStruct_INVCacheObjet* v1; // [esp+0h] [ebp-20h]
    tGameObjectMain* v2; // [esp+4h] [ebp-1Ch]
    tGameObjectMain* v3; // [esp+8h] [ebp-18h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-14h]
    tGameObjectMain* v5; // [esp+10h] [ebp-10h]
    int v6; // [esp+14h] [ebp-Ch]
    int i; // [esp+1Ch] [ebp-4h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_INVCacheObjet*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        INV_cacheobjet_loop_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &INV_cacheobjet_loop_INV_manager_gao_ao_GameText,
            (void*)INV_cacheobjet_loop_INV_manager_gao,
            23312);
    }
    v6 = -1;
    for (i = 0; i < 8; ++i)
    {
        v5 = *(tGameObjectMain**)(INV_cacheobjet_loop_INV_manager_gao_ao_GameText + 4 * i + 4);
        if (v5)
        {
            if ((unsigned int)v5 <= 2)
                v5 = AI_MainActor[(uint32_t)v5 - 1];
            v4 = v5;
        }
        else
        {
            v4 = 0;
        }
        if (!v4)
        {
            v6 = i;
            break;
        }
        v3 = (tGameObjectMain*)v1->des_o_obj;
        if (v1->des_o_obj)
        {
            if ((unsigned int)v3 <= 2)
                v3 = AI_MainActor[(uint32_t)v3 - 1];
            v2 = v3;
        }
        else
        {
            v2 = 0;
        }
        if (*(tGameObjectMain**)(INV_cacheobjet_loop_INV_manager_gao_ao_GameText + 4 * i + 4) == v2)
        {
            v6 = i;
            break;
        }
    }
    if (v6 != -1)
        *(uint32_t*)(INV_cacheobjet_loop_INV_manager_gao_ao_GameText + 4 * v6 + 4) = v1->des_o_obj;
    AI_EvalFunc_OBJ_FlagInactiveSet_C(AI_gpst_CurrentGameObject, 1);
    return 1000;
}