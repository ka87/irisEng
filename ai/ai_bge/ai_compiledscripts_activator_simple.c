#include "ai_compiledscripts_inc.h"

//activator_simple_ac_loop
int activator_simple_ac_loop_univVars;
int activator_simple_ac_loop_univVars_Perso_ID_actor;

int activator_simple_ac_loop()
{
    tScriptStruct_ActivatorSimple* v5; // [esp+0h] [ebp-A8h]
    tGameObjectMain* viewObj; // [esp+84h] [ebp-24h]

    if (AI_gpst_CurrentInstance)
        v5 = (tScriptStruct_ActivatorSimple*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v5 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        activator_simple_ac_loop_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &activator_simple_ac_loop_univVars_Perso_ID_actor,
            (void*)activator_simple_ac_loop_univVars,
            988);
    }
    
    if (!AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext))
    {
        if (AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao(AI_EvalFunc_VIEWGetObject_C(0))))
        {
            if (!v5->i_ON)
            {
                v5->i_ON = 1;
                AI_EvalFunc_GRPSectInvisibleSet_C(AI_gpst_CurrentGameObject, -1, 1);
            }
        }
        else if (v5->i_ON)
        {
            v5->i_ON = 0;
            AI_EvalFunc_GRPSectInvisibleSet_C(AI_gpst_CurrentGameObject, -1, -1);
        }
        return 1000;
    }
    viewObj = AI_EvalFunc_VIEWGetObject_C(0);
    if (v5->des_i_foraircraft)
    {
        if (AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao(viewObj)))
        {
            if (AI2C_CheckActorGao(AI_MainActor[0]) == *(tGameObjectMain**)(activator_simple_ac_loop_univVars_Perso_ID_actor + 44))
            {
                if (!v5->i_ON)
                {
                    v5->i_ON = 1;
                    AI_EvalFunc_GRPSectInvisibleSet_C(AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext), -1, 1);
                }
                return 1000;
            }
        }
        if (!v5->i_ON)
            return 1000;

        v5->i_ON = 0;
        AI_EvalFunc_GRPSectInvisibleSet_C(AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext), -1, -1);
        return 1000;
    }
    if (v5->des_i_both)
    {
        if (AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao(viewObj)))
        {
            if (AI2C_CheckActorGao(AI_MainActor[0]) != *(tGameObjectMain**)(activator_simple_ac_loop_univVars_Perso_ID_actor + 40))
            {
                if (!v5->i_ON)
                {
                    v5->i_ON = 1;
                    AI_EvalFunc_GRPSectInvisibleSet_C(AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext), -1, 1);
                }
                return 1000;
            }
        }
        if (!v5->i_ON)
            return 1000;

        v5->i_ON = 0;
        AI_EvalFunc_GRPSectInvisibleSet_C(AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext), -1, -1);
        return 1000;
    }
    
    if (!AI_EvalFunc_COLBVPivotCollide_C(AI_gpst_CurrentGameObject, AI2C_CheckActorGao(viewObj)) || 
        AI2C_CheckActorGao(AI_MainActor[0]) != *(tGameObjectMain**)(activator_simple_ac_loop_univVars_Perso_ID_actor + 36))
    {
        if (!v5->i_ON)
            return 1000;

        v5->i_ON = 0;
        AI_EvalFunc_GRPSectInvisibleSet_C(AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext), -1, -1);
        return 1000;
    }
    if (!v5->i_ON)
    {
        v5->i_ON = 1;
        AI_EvalFunc_GRPSectInvisibleSet_C(AI2C_CheckActorGao((tGameObjectMain*)v5->des_o_mode_ext), -1, 1);
    }
    return 1000;
}