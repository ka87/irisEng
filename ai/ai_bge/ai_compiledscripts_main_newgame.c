#include "ai_compiledscripts_inc.h"

// IMHO it isn't used.

//main_newgame_mng_init
int main_newgame_mng_init_univVars;
int main_newgame_mng_init_univVars_load_world;
int main_newgame_mng_init_univVars_i_force_newgame_reinit;
int main_newgame_mng_init_univVars_i_INV_OptionsFlags;

int main_newgame_mng_init()
{
    int v1; // [esp+8h] [ebp-4h]

    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        main_newgame_mng_init_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(&main_newgame_mng_init_univVars_load_world, (void*)main_newgame_mng_init_univVars, 0);
        AI2C_ResolveBuffer(
            &main_newgame_mng_init_univVars_i_force_newgame_reinit,
            (void*)main_newgame_mng_init_univVars,
            32);
        AI2C_ResolveBuffer(&main_newgame_mng_init_univVars_i_INV_OptionsFlags, (void*)main_newgame_mng_init_univVars, 7656);
    }
    *(uint32_t*)main_newgame_mng_init_univVars_load_world = 1;
    *(uint32_t*)main_newgame_mng_init_univVars_i_force_newgame_reinit = 1;
    v1 = *(uint32_t*)main_newgame_mng_init_univVars_i_INV_OptionsFlags;
    AI_ReinitUniverse();
    *(uint32_t*)main_newgame_mng_init_univVars_i_INV_OptionsFlags = v1;
    return 1000;
}