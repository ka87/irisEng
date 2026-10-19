#include "ai_compiledscripts_inc.h"

int activateur_act_ini()
{
    tScriptStruct_activateur* v2;
    tGameObjectMain* v10;
    int flags;

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_activateur*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;

    flags = 0;
   
    if (!AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_actor_who_activate) || 
        !AI_EvalFunc_COLBVObjectObject_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_actor_who_activate)))
    {
        if (!AI_EvalFunc_COLBVGetAllOverlaps_C(AI_gpst_CurrentGameObject, &v10, v2->des_i_CBits_Test, 0, 5))
        {
            if (!v2->des_i_change_hors_BV)
                return 1000;

            flags = 0x400000;
            
            if (v2->des_i_visibilite)
                flags += 0x200000;
            
            if (!v2->des_i_active_desactive)
            {
                AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_actor_to_activate), 0, flags);
                return 1000;
            }
            
            AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_actor_to_activate), flags, 0);
            return 1000;
        }
    }

    flags = 0x400000;
    
    if (v2->des_i_visibilite)
        flags += 0x200000;
    
    if (!v2->des_i_active_desactive)
    {
        AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_actor_to_activate), flags, 0);
        return 1000;
    }

    AI_EvalFunc_OBJ_FlagsControlSet_C(AI2C_CheckActorGao((tGameObjectMain*)v2->des_o_actor_to_activate), 0, flags);
    return 1000;
}