#include "ai_compiledscripts_inc.h"

//SND_Musique_Jour_Nuit_Zone_mjnz_play
int SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars;
int SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars_Planetes_CurHour;

int SND_Musique_Jour_Nuit_Zone_mjnz_play()
{
    int v1; // eax
    tAINode* v2; // [esp+0h] [ebp-2Ch]
    tScriptStruct_SNDMusiqueJourNuitZone* v3; // [esp+4h] [ebp-28h]
    tGameObjectMain* v4; // [esp+Ch] [ebp-20h]
    tGameObjectMain* a2; // [esp+10h] [ebp-1Ch]
    tGameObjectMain* v6; // [esp+14h] [ebp-18h]

    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_SNDMusiqueJourNuitZone*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars_Planetes_CurHour,
            (void*)SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars,
            6116);
    }
    v2 = *AI_gppst_CurrentJumpNode;
    if (!*AI_gppst_CurrentJumpNode)
    {
        v3->i_jour = v3->f_heure_debjour >= (double)*(float*)SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars_Planetes_CurHour
            || v3->f_heure_finjour <= (double)*(float*)SND_Musique_Jour_Nuit_Zone_mjnz_play_univVars_Planetes_CurHour;
        v6 = *AI_MainActor;
        if (*AI_MainActor)
        {
            if ((unsigned int)v6 <= 2)
                v6 = AI_MainActor[(uint32_t)v6 - 1];
            a2 = v6;
        }
        else
        {
            a2 = 0;
        }
        v4 = (tGameObjectMain*)v3->o_zone_ambiancelocale;
        if (v4)
        {
            if ((unsigned int)v4 <= 2)
                v4 = AI_MainActor[(uint32_t)v4 - 1];
            v1 = AI_EvalFunc_COLBVPivotCollide_C(v4, a2);
        }
        else
        {
            v1 = AI_EvalFunc_COLBVPivotCollide_C(0, a2);
        }
        if (v1)
            v3->i_playing = 2;
        else
            v3->i_playing = 0;
        if (v3->i_jour == v3->i_jour_old && v3->i_playing_old == v3->i_playing && SND_i_IsTrackPlaying(v3->i_track))
        {
        LABEL_37:
            v3->i_playing_old = v3->i_playing;
            v3->i_jour_old = v3->i_jour;
            *AI_gppst_CurrentJumpNode = (tAINode*)9;
            TIME_Wait_C(0.5);
            return 0;
        }
        AI_EvalFunc_SNDStopModifier_C(AI_gpst_CurrentGameObject, v3->i_jour_old + v3->i_playing_old);
    LABEL_34:
        if (SND_i_IsTrackPlaying(v3->i_track))
        {
            *AI_gppst_CurrentJumpNode = (tAINode*)8;
            return 0;
        }
        AI_EvalFunc_SNDPlayModifier_C(AI_gpst_CurrentGameObject, v3->i_jour + v3->i_playing);
        goto LABEL_37;
    }
    if (v2 == (tAINode*)8)
        goto LABEL_34;
    if (v2 == (tAINode*)9)
        return 1000;
    *AI_gppst_CurrentJumpNode = 0;
    TIME_Wait_C(0.2f);
    return 0;
}