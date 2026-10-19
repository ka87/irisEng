#include "ai_compiledscripts_inc.h"

//SND_MusicFight_Init_mf_init
int SND_MusicFight_Init_mf_init_GST_Global_gao;
int SND_MusicFight_Init_mf_init_GST_Global_gao_o_musique_fight_list;

int SND_MusicFight_Init_mf_init()
{
    tScriptStruct_SND_MusicFight* v1; // [esp+0h] [ebp-Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_SND_MusicFight*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_MusicFight_Init_mf_init_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &SND_MusicFight_Init_mf_init_GST_Global_gao_o_musique_fight_list,
            (void*)SND_MusicFight_Init_mf_init_GST_Global_gao,
            3460);
    }
    *(uint32_t*)(SND_MusicFight_Init_mf_init_GST_Global_gao_o_musique_fight_list + 4 * v1->i_type_de_perso + 4) = (uint32_t)AI_gpst_CurrentGameObject;
    AI_EvalFunc_AITrackCurStop_C();
    return 1000;
}