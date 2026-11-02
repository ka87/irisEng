#include "ai_compiledscripts.h"
#include "../ai.h"
#include "../time.h"
#include "../win/wingdi.h"
#include "ai_debugfunctions.h"
#include "ai_aifunctions.h"
#include "ai_mathfunctions.h"
#include "ai_inofunctions.h"
#include "ai_timefunctions.h"
#include "ai_worldfunctions.h"
#include "ai_messagefunctions.h"
#include "ai_gameobjectfunctions.h"
#include "ai_pagfunctions.h"
#include "ai_lightfunctions.h"
#include "ai_wayfunctions.h"
#include "ai_dynfunctions.h"
#include "ai_arrfunctions.h"
#include "ai_colfunctions.h"
#include "ai_soundfunctions.h"
#include "ai_viewfunctions.h"
#include "ai_gridfunctions.h"
#include "ai_animationfunctions.h"
#include "ai_actionfunctions.h"
#include "ai_netfunctions.h"
#include "ai_materialandcolorfunctions.h"
#include "ai_eventtrackfunctions.h"
#include "ai_sectorfunctions.h"
#include "ai_miscfunctions.h"
#include "ai_gfxfunctions.h"
#include "../strings.h"
#include "../inputoutput.h"
#include "../textures.h"
#include "../modifiers/xmenmodifier.h"
#include "../dbg.h"

int sc_dummy() {   
    return 1000;
}

int hack() {        
        int x = AI_ul_RealResolveOneRefObject(0x620031e5);
        AI_EvalFunc_VIEWAssignObject_C((tGameObjectMain*)x, 0);
        return 1000;
}

int hackViewActorSelector = 0;

int levelHack() {
    int _pj_cam = AI_ul_RealResolveOneRefObject(0x71003F56u);
    int INV_reference_WP = AI_ul_RealResolveOneRefObject(0x620013B8u); // INV WP (group), board panel calls it in original game
    AI_EvalFunc_GRPInactiveSet_C((tGameObjectMain*)INV_reference_WP, 1); // disable inventory menu
    Vec3 v;

    AI_EvalFunc_VIEWAssignObject_C((tGameObjectMain * )_pj_cam, 0);

    //DBG_OutputDebugString("WK: %x\n", AI_EvalFunc_WORGetKey_C());

    //Vec3* v2 = OBJ_pst_GetPos((tGameObjectMain*)_pj_cam);

    if (AI_EvalFunc_WORGetKey_C() == 0xd9005833) {
        Vec3 v2;

        if (AI_EvalFunc_IoButtonJustPressed_C(0)) hackViewActorSelector = !hackViewActorSelector;

        if (hackViewActorSelector) {
            // peyj
            v2.x = -1.5f;
            v2.y = 1.5f;
            v2.z = 0.25f;
        }
        else {
            // sally
            v2.x = -1.05f;
            v2.y = -0.57f; // ez a z igazabol
            v2.z = 7.0;
        }
        OBJ_SetAbsolutePosition((tGameObjectMain*)_pj_cam, &v2);
        // eddig
    }
    else {

        //v2->z = (TIM_gf_dt / 6.0f) + v2->z;
        //OBJ_SetAbsolutePosition((tGameObjectMain*)_pj_cam, &v2);

        v.x = 74.52093f;
        v.y = 20.953468f;
        v.z = 61.516752f;
        OBJ_SetAbsolutePosition((tGameObjectMain*)_pj_cam, &v);
        AI_EvalFunc_OBJRotateGlobalZ_C((tGameObjectMain*)_pj_cam, TIM_gf_dt/4.0f);
    }
    //AI_EvalFunc_WORColorAmbiantSet_C(0, 0x7b723c);
    //AI_EvalFunc_WORColorAmbiant2Set_C(0, 0x908854);
    return 1000;
}

int script_obj_destroy()
{
    return AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
}

#define SC_MISSING(hexkey)                                                 \
    static int sc_seen_##hexkey = 0;                                       \
    static int sc_missing_##hexkey(void)                                   \
    {                                                                      \
        if (!sc_seen_##hexkey)                                             \
        {                                                                  \
            sc_seen_##hexkey = 1;                                          \
            DBG_OutputDebugString("AI2C: behaviour 0x%08x has no "         \
                                  "implementation\n", 0x##hexkey);         \
        }                                                                  \
        return 1000;                                                       \
    }

tStruct_funclist AI2C_gat_fctdefs[] = { 0 };

unsigned int AI2C_i_SizeofArray()
{
    return sizeof(AI2C_gat_fctdefs)/sizeof(AI2C_gat_fctdefs[0]);
}