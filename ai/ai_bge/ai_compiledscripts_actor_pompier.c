#include "ai_compiledscripts_inc.h"

//Pompier_Police_mode_hyllis
int Pompier_Police_mode_hyllis_univVars;
int Pompier_Police_mode_hyllis_univVars_Flag_speciaux_sauve;
int Pompier_Police_mode_hyllis__PJ_Aircraft_gao;
int Pompier_Police_mode_hyllis_GST_Global_gao;
int Pompier_Police_mode_hyllis_GST_Global_gao_o_joy_acteur;
int Pompier_Police_mode_hyllis__PJ_Camera_gao;
int Pompier_Police_mode_hyllis_univVars_f_sally_fric;
int Pompier_Police_mode_hyllis__PJ_Camera_Speciale_gao;
int Pompier_Police_mode_hyllis_GST_Board_panel_gao;
int Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao;
int Pompier_Police_mode_hyllis_GST_Board_panel_gao_Gen_Show;
int fct_Pompier_Police_mode_hyllis_Pompier_Police_mode_wait;
int Pompier_Police_mode_hyllis_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour;
int Pompier_Police_mode_INV_manager_gao;
int Pompier_Police_mode_INV_manager_gao_i_Fric2Add;
int Pompier_Police_mode_INV_manager_gao_i_FricWithPerso;
int Pompier_Police_mode_INV_manager_gao_f_PogneAppearTimeInit;
int Pompier_Police_mode_INV_manager_gao_f_PogneAppearTimeFixed;
int Pompier_Police_mode_INV_manager_gao_f_PogneDelay;
int Pompier_Police_mode_INV_manager_gao_i_OpeningMode;
int fct_Pompier_Police_mode_Pompier_Pompier_exec_GFX_Halo;

//Pompier_Pompier_mode_hyllis
int Pompier_Pompier_mode_hyllis__PJ_Aircraft_gao;
int fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_get_aircraft;
int fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_mode_wait;
int fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_GFX_Halo;
int fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_GFX_Smoke;

//Pompier_Pompier_exec_GFX_Flash
int Pompier_Pompier_exec_GFX_Flash_SFX_light_smoke_gao;

//Pompier_Pompier_init
int fct_Pompier_Pompier_init_Pompier_Pompier_mode_canaux;
int Pompier_Pompier_init_DUP_WP_simple_de_base_gao;
int fct_Pompier_Pompier_init_Pompier_Pompier_mode_wait;
int fct_Pompier_Pompier_init_Pompier_Police_mode_canaux;
int fct_Pompier_Pompier_init_Pompier_Police_mode_wait;

//Pompier_Pompier_mode_canaux
int Pompier_Pompier_mode_canaux__PJ_Aircraft_gao;
int Pompier_Pompier_mode_canaux_univVars;
int Pompier_Pompier_mode_canaux_univVars_Flag_speciaux_sauve;
int fct_Pompier_Pompier_mode_canaux_Pompier_Pompier_exec_GFX_Halo;
int fct_Pompier_Pompier_mode_canaux_Pompier_Pompier_exec_GFX_Smoke;

//Pompier_Police_mode_wait
int Pompier_Police_mode_wait_univVars;
int Pompier_Police_mode_wait_univVars_Flag_speciaux_sauve;
int Pompier_Police_mode_wait_univVars_f_sally_fric;
int fct_Pompier_Police_mode_wait_Pompier_Police_mode_hyllis;

//Pompier_Pompier_mode_wait
int fct_Pompier_Pompier_mode_wait_Pompier_Pompier_exec_get_aircraft;
int fct_Pompier_Pompier_mode_wait_Pompier_Pompier_mode_hyllis;

//Pompier_Police_mode_canaux
int Pompier_Police_mode_canaux__PJ_Aircraft_gao;
int Pompier_Police_mode_canaux_univVars;
int Pompier_Police_mode_canaux_univVars_f_sally_fric;
int Pompier_Police_mode_canaux_GST_Global_gao;
int Pompier_Police_mode_canaux_GST_Global_gao_o_joy_acteur;
int Pompier_Police_mode_canaux__PJ_Camera_Speciale_gao;
int Pompier_Police_mode_canaux_GST_Board_panel_gao;
int Pompier_Police_mode_canaux__PJ_Camera_gao;
int Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao;
int Pompier_Police_mode_canaux_GST_Board_panel_gao_Gen_Show;
int Pompier_Police_mode_canaux_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour;
int Pompier_Police_mode_canaux_INV_manager_gao;
int Pompier_Police_mode_canaux_INV_manager_gao_i_Fric2Add;
int Pompier_Police_mode_canaux_INV_manager_gao_i_FricWithPerso;
int Pompier_Police_mode_canaux_INV_manager_gao_f_PogneAppearTimeInit;
int Pompier_Police_mode_canaux_INV_manager_gao_f_PogneAppearTimeFixed;
int Pompier_Police_mode_canaux_INV_manager_gao_f_PogneDelay;
int Pompier_Police_mode_canaux_INV_manager_gao_i_OpeningMode;
int fct_Pompier_Police_mode_canaux_Pompier_Pompier_exec_GFX_Halo;
int fct_Pompier_Police_mode_canaux_Pompier_Pompier_exec_GFX_Flash;

//Pompier_Pompier_exec_GFX_Halo
int fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;

//Pompier_Pompier_exec_GFX_Smoke
int Pompier_Pompier_exec_GFX_Smoke_SFX_explosion_gao;

int Pompier_Police_mode_hyllis()
{
    double v1; // st7
    int v2; // eax
    int v3; // eax
    int v4; // eax
    tGameObjectMain* v5; // eax
    tGameObjectMain* v6; // eax
    unsigned int v7; // eax
    double v8; // st7
    double v9; // st7
    int v10; // eax
    double v11; // st7
    double v12; // st7
    double v13; // st7
    float a4; // [esp+0h] [ebp-968h]
    float a4a; // [esp+0h] [ebp-968h]
    float a4b; // [esp+0h] [ebp-968h]
    float a4c; // [esp+0h] [ebp-968h]
    float a4d; // [esp+0h] [ebp-968h]
    float a4e; // [esp+0h] [ebp-968h]
    float a4f; // [esp+0h] [ebp-968h]
    float a4g; // [esp+0h] [ebp-968h]
    float a4h; // [esp+0h] [ebp-968h]
    float a4i; // [esp+0h] [ebp-968h]
    float v24; // [esp+8h] [ebp-960h]
    float v25; // [esp+Ch] [ebp-95Ch]
    float v26; // [esp+10h] [ebp-958h]
    float v27; // [esp+14h] [ebp-954h]
    float v28; // [esp+18h] [ebp-950h]
    tScriptStruct_PNJPompier* v29; // [esp+1Ch] [ebp-94Ch]
    Vec3* v30; // [esp+2Ch] [ebp-93Ch]
    Vec3* v31; // [esp+30h] [ebp-938h]
    Vec3* v32; // [esp+3Ch] [ebp-92Ch]
    float v33; // [esp+40h] [ebp-928h]
    Vec3* v34; // [esp+48h] [ebp-920h]
    Vec3* v35; // [esp+4Ch] [ebp-91Ch]
    Vec3* v36; // [esp+64h] [ebp-904h]
    double v37; // [esp+98h] [ebp-8D0h]
    float v38; // [esp+A4h] [ebp-8C4h]
    float v39; // [esp+B0h] [ebp-8B8h]
    float v40; // [esp+BCh] [ebp-8ACh]
    tGameObjectMain* v41; // [esp+D0h] [ebp-898h]
    float v42; // [esp+D4h] [ebp-894h]
    Vec3* v43; // [esp+E4h] [ebp-884h]
    float v44; // [esp+E8h] [ebp-880h]
    float v45; // [esp+ECh] [ebp-87Ch]
    tGameObjectMain* v46; // [esp+120h] [ebp-848h]
    tGameObjectMain* v47; // [esp+124h] [ebp-844h]
    float v48; // [esp+128h] [ebp-840h]
    float v49; // [esp+130h] [ebp-838h]
    float v50; // [esp+134h] [ebp-834h]
    float v51; // [esp+138h] [ebp-830h]
    float v52; // [esp+13Ch] [ebp-82Ch]
    tGameObjectMain* v53; // [esp+150h] [ebp-818h]
    tGameObjectMain* v54; // [esp+164h] [ebp-804h]
    tGameObjectMain* v55; // [esp+168h] [ebp-800h]
    tGameObjectMain* v56; // [esp+16Ch] [ebp-7FCh]
    tGameObjectMain* v57; // [esp+170h] [ebp-7F8h]
    tGameObjectMain* v58; // [esp+174h] [ebp-7F4h]
    tGameObjectMain* v59; // [esp+178h] [ebp-7F0h]
    tGameObjectMain* v60; // [esp+17Ch] [ebp-7ECh]
    tGameObjectMain* v61; // [esp+180h] [ebp-7E8h]
    tGameObjectMain* v62; // [esp+184h] [ebp-7E4h]
    tGameObjectMain* v63; // [esp+18Ch] [ebp-7DCh]
    tGameObjectMain* v64; // [esp+194h] [ebp-7D4h]
    tGameObjectMain* v65; // [esp+19Ch] [ebp-7CCh]
    tGameObjectMain* v66; // [esp+1A4h] [ebp-7C4h]
    tGameObjectMain* v67; // [esp+1ACh] [ebp-7BCh]
    tGameObjectMain* v68; // [esp+1B0h] [ebp-7B8h]
    tGameObjectMain* v69; // [esp+1B4h] [ebp-7B4h]
    tGameObjectMain* v70; // [esp+1B8h] [ebp-7B0h]
    tGameObjectMain* v71; // [esp+1BCh] [ebp-7ACh]
    tGameObjectMain* v72; // [esp+1C4h] [ebp-7A4h]
    tGameObjectMain* v73; // [esp+1CCh] [ebp-79Ch]
    tGameObjectMain* v74; // [esp+1D4h] [ebp-794h]
    tGameObjectMain* v75; // [esp+1D8h] [ebp-790h]
    tGameObjectMain* v76; // [esp+1DCh] [ebp-78Ch]
    tGameObjectMain* v77; // [esp+1E0h] [ebp-788h]
    tGameObjectMain* v78; // [esp+1E8h] [ebp-780h]
    tGameObjectMain* v79; // [esp+1F0h] [ebp-778h]
    tGameObjectMain* v80; // [esp+1F4h] [ebp-774h]
    tGameObjectMain* v81; // [esp+1F8h] [ebp-770h]
    float v82; // [esp+238h] [ebp-730h]
    float v83; // [esp+244h] [ebp-724h]
    tGameObjectMain* v84; // [esp+254h] [ebp-714h]
    tGameObjectMain* v85; // [esp+258h] [ebp-710h]
    tGameObjectMain* v86; // [esp+25Ch] [ebp-70Ch]
    tGameObjectMain* v87; // [esp+260h] [ebp-708h]
    tGameObjectMain* v88; // [esp+274h] [ebp-6F4h]
    tGameObjectMain* v89; // [esp+278h] [ebp-6F0h]
    tGameObjectMain* v90; // [esp+27Ch] [ebp-6ECh]
    tGameObjectMain* v91; // [esp+284h] [ebp-6E4h]
    Vec3* v92; // [esp+288h] [ebp-6E0h]
    Vec3* v93; // [esp+28Ch] [ebp-6DCh]
    Vec3 v94; // [esp+294h] [ebp-6D4h] BYREF
    Vec3 v95; // [esp+2A0h] [ebp-6C8h] BYREF
    Vec3 v96; // [esp+2ACh] [ebp-6BCh] BYREF
    Vec3 v97; // [esp+2C4h] [ebp-6A4h] BYREF
    Vec3 v98; // [esp+2D0h] [ebp-698h] BYREF
    Vec3 v99; // [esp+2DCh] [ebp-68Ch] BYREF
    float v101; // [esp+2F4h] [ebp-674h]
    float v102; // [esp+2FCh] [ebp-66Ch]
    float v103; // [esp+300h] [ebp-668h]
    float v104; // [esp+304h] [ebp-664h]
    float v105; // [esp+308h] [ebp-660h]
    float v106; // [esp+30Ch] [ebp-65Ch]
    float v107; // [esp+310h] [ebp-658h]
    Vec3 v108; // [esp+320h] [ebp-648h] BYREF
    Vec3 v109; // [esp+32Ch] [ebp-63Ch] BYREF
    Vec3 v110; // [esp+338h] [ebp-630h] BYREF
    Vec3 v111; // [esp+344h] [ebp-624h] BYREF
    Vec3 v112; // [esp+350h] [ebp-618h] BYREF
    Vec3 v113; // [esp+35Ch] [ebp-60Ch] BYREF
    Vec3 v114; // [esp+368h] [ebp-600h] BYREF
    Vec3 v115; // [esp+374h] [ebp-5F4h] BYREF
    Vec3 v116; // [esp+38Ch] [ebp-5DCh] BYREF
    Vec3 v117; // [esp+3A4h] [ebp-5C4h] BYREF
    Vec3 v119; // [esp+3C0h] [ebp-5A8h] BYREF
    Vec3 v120; // [esp+3CCh] [ebp-59Ch] BYREF
    Vec3 v121; // [esp+3D8h] [ebp-590h] BYREF
    Vec3 dest; // [esp+3E4h] [ebp-584h] BYREF
    float v123; // [esp+3F4h] [ebp-574h]
    Vec3 a1; // [esp+3F8h] [ebp-570h] BYREF
    Vec3 v125; // [esp+404h] [ebp-564h] BYREF
    Vec3 v126; // [esp+410h] [ebp-558h] BYREF
    Vec3 v127; // [esp+41Ch] [ebp-54Ch] BYREF
    Vec3 v128; // [esp+428h] [ebp-540h] BYREF
    float v129; // [esp+434h] [ebp-534h]
    float v130; // [esp+43Ch] [ebp-52Ch]
    Vec3 v131; // [esp+440h] [ebp-528h] BYREF
    Vec3 v132; // [esp+44Ch] [ebp-51Ch] BYREF
    Vec3 v133; // [esp+458h] [ebp-510h] BYREF
    Vec3 v134; // [esp+464h] [ebp-504h] BYREF
    Vec3 v135; // [esp+470h] [ebp-4F8h] BYREF
    Vec3 v136; // [esp+47Ch] [ebp-4ECh] BYREF
    Vec3 v137; // [esp+488h] [ebp-4E0h] BYREF
    Vec3 v138; // [esp+494h] [ebp-4D4h] BYREF
    float v139; // [esp+4A8h] [ebp-4C0h]
    float v140; // [esp+4B0h] [ebp-4B8h]
    Vec3 v141; // [esp+4B8h] [ebp-4B0h]
    Vec3 v142; // [esp+4C4h] [ebp-4A4h] BYREF
    Vec3 v143; // [esp+4DCh] [ebp-48Ch] BYREF
    Vec3 v144; // [esp+4E8h] [ebp-480h] BYREF
    Vec3 v145; // [esp+500h] [ebp-468h] BYREF
    Vec3 v146; // [esp+50Ch] [ebp-45Ch] BYREF
    Vec3 v147; // [esp+518h] [ebp-450h] BYREF
    tMessage Src; // [esp+524h] [ebp-444h] BYREF
    float v149; // [esp+558h] [ebp-410h]
    float v150; // [esp+560h] [ebp-408h]
    Vec3 v151; // [esp+56Ch] [ebp-3FCh] BYREF
    Vec3 v152; // [esp+578h] [ebp-3F0h] BYREF
    Vec3 v153; // [esp+584h] [ebp-3E4h] BYREF
    Vec3 v154; // [esp+590h] [ebp-3D8h] BYREF
    Vec3 v155; // [esp+5A8h] [ebp-3C0h] BYREF
    unsigned int v156; // [esp+5B4h] [ebp-3B4h]
    float v157; // [esp+5B8h] [ebp-3B0h]
    Vec3 v158; // [esp+5BCh] [ebp-3ACh] BYREF
    Vec3 v159; // [esp+5C8h] [ebp-3A0h] BYREF
    Vec3 v160; // [esp+5D4h] [ebp-394h] BYREF
    Vec3 v161; // [esp+5E0h] [ebp-388h] BYREF
    Vec3 a2; // [esp+5ECh] [ebp-37Ch] BYREF
    Vec3 v163; // [esp+5F8h] [ebp-370h] BYREF
    Vec3 v164; // [esp+604h] [ebp-364h] BYREF
    Vec3 v165; // [esp+610h] [ebp-358h] BYREF
    BOOL v166; // [esp+61Ch] [ebp-34Ch]
    float v167; // [esp+620h] [ebp-348h]
    float v168; // [esp+624h] [ebp-344h]
    Vec3 v169; // [esp+628h] [ebp-340h] BYREF
    Vec3 v170; // [esp+634h] [ebp-334h]
    float v171; // [esp+640h] [ebp-328h]
    float v172; // [esp+644h] [ebp-324h]
    float v173; // [esp+648h] [ebp-320h]
    Vec3 v174; // [esp+64Ch] [ebp-31Ch]
    Vec3 v175; // [esp+658h] [ebp-310h] BYREF
    float v176; // [esp+664h] [ebp-304h]
    BOOL v177; // [esp+66Ch] [ebp-2FCh]
    float v178; // [esp+674h] [ebp-2F4h]
    float v179; // [esp+678h] [ebp-2F0h]
    float v180; // [esp+684h] [ebp-2E4h]
    Vec3 v181; // [esp+688h] [ebp-2E0h] BYREF
    Vec3 v182; // [esp+6A0h] [ebp-2C8h] BYREF
    Vec3 v183; // [esp+6ACh] [ebp-2BCh] BYREF
    Vec3 v184; // [esp+6B8h] [ebp-2B0h] BYREF
    Vec3 v185; // [esp+6C4h] [ebp-2A4h] BYREF
    Vec3 v186; // [esp+6D0h] [ebp-298h] BYREF
    Vec3 v187; // [esp+6DCh] [ebp-28Ch] BYREF
    int v188; // [esp+6E8h] [ebp-280h]
    float v189; // [esp+6ECh] [ebp-27Ch]
    float v190; // [esp+6F0h] [ebp-278h]
    Vec3 v191; // [esp+6F4h] [ebp-274h] BYREF
    Vec3 a3; // [esp+700h] [ebp-268h] BYREF
    float v193; // [esp+70Ch] [ebp-25Ch]
    float v194; // [esp+710h] [ebp-258h]
    float v195; // [esp+714h] [ebp-254h]
    float v196; // [esp+718h] [ebp-250h]
    float v197; // [esp+71Ch] [ebp-24Ch]
    Vec3 v199; // [esp+734h] [ebp-234h] BYREF
    Vec3 v200; // [esp+740h] [ebp-228h] BYREF
    float v201; // [esp+758h] [ebp-210h]
    float v202; // [esp+75Ch] [ebp-20Ch]
    float v203; // [esp+760h] [ebp-208h]
    int v204; // [esp+768h] [ebp-200h]
    float v205; // [esp+76Ch] [ebp-1FCh]
    int v206; // [esp+770h] [ebp-1F8h]
    float v207; // [esp+774h] [ebp-1F4h]
    float v208; // [esp+778h] [ebp-1F0h]
    Vec3 v209; // [esp+77Ch] [ebp-1ECh] BYREF
    Vec3 v210; // [esp+788h] [ebp-1E0h] BYREF
    Vec3 v211; // [esp+794h] [ebp-1D4h] BYREF
    Vec3 v212; // [esp+7A0h] [ebp-1C8h] BYREF
    Vec3 v213; // [esp+7ACh] [ebp-1BCh] BYREF
    Vec3 v214; // [esp+7B8h] [ebp-1B0h] BYREF
    Vec3 v215; // [esp+7C4h] [ebp-1A4h] BYREF
    Vec3 v216; // [esp+7D0h] [ebp-198h] BYREF
    Vec3 v217; // [esp+7DCh] [ebp-18Ch] BYREF
    int v218; // [esp+7E8h] [ebp-180h]
    float v219; // [esp+7ECh] [ebp-17Ch]
    float v220; // [esp+7F0h] [ebp-178h]
    float v221; // [esp+7F4h] [ebp-174h]
    Vec3 v222; // [esp+7F8h] [ebp-170h] BYREF
    Vec3 v223; // [esp+804h] [ebp-164h] BYREF
    Vec3 v224; // [esp+810h] [ebp-158h] BYREF
    Vec3 a5; // [esp+81Ch] [ebp-14Ch] BYREF
    Vec3 v226; // [esp+828h] [ebp-140h] BYREF
    float v227; // [esp+834h] [ebp-134h]
    float v228; // [esp+838h] [ebp-130h]
    float v229; // [esp+83Ch] [ebp-12Ch]
    float v230; // [esp+850h] [ebp-118h]
    int v231; // [esp+854h] [ebp-114h]
    float v232; // [esp+858h] [ebp-110h]
    float v233; // [esp+85Ch] [ebp-10Ch]
    float v234; // [esp+860h] [ebp-108h]
    float v235; // [esp+864h] [ebp-104h]
    Vec3 v236; // [esp+868h] [ebp-100h] BYREF
    float v237; // [esp+874h] [ebp-F4h]
    float v238; // [esp+878h] [ebp-F0h]
    float v239; // [esp+87Ch] [ebp-ECh]
    Vec3 v240; // [esp+880h] [ebp-E8h]
    Vec3 v241; // [esp+88Ch] [ebp-DCh] BYREF
    Vec3 v242; // [esp+898h] [ebp-D0h] BYREF
    Vec3 v243; // [esp+8A4h] [ebp-C4h] BYREF
    Vec3 v244; // [esp+8B0h] [ebp-B8h] BYREF
    tScriptStruct_PNJPompier* v245; // [esp+8BCh] [ebp-ACh]
    float v246; // [esp+8C0h] [ebp-A8h]
    float v247; // [esp+8C4h] [ebp-A4h]
    float v248; // [esp+8C8h] [ebp-A0h]
    float v249; // [esp+8CCh] [ebp-9Ch]
    float v250; // [esp+8D0h] [ebp-98h]
    float v251; // [esp+8D4h] [ebp-94h]
    BOOL v252; // [esp+8D8h] [ebp-90h]
    float v253; // [esp+8DCh] [ebp-8Ch]
    Vec3 v254; // [esp+8E0h] [ebp-88h] BYREF
    Vec3 v255; // [esp+8F8h] [ebp-70h] BYREF
    Vec3 v256; // [esp+904h] [ebp-64h] BYREF
    float v257; // [esp+914h] [ebp-54h]
    tGameObjectMain* v258; // [esp+91Ch] [ebp-4Ch]
    float v259; // [esp+920h] [ebp-48h]
    Vec3 v260; // [esp+928h] [ebp-40h] BYREF
    Vec3 v261; // [esp+934h] [ebp-34h] BYREF
    Vec3 v262; // [esp+940h] [ebp-28h] BYREF
    tGameObjectMain* v263; // [esp+94Ch] [ebp-1Ch]
    float v264; // [esp+950h] [ebp-18h]
    float v265; // [esp+954h] [ebp-14h]
    int v266; // [esp+95Ch] [ebp-Ch]

    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    v212.x = 1.0;
    v212.y = 0.0;
    v212.z = 0.0;
    v213.x = 1.0;
    v213.y = 0.0;
    v213.z = 0.0;
    v171 = 1.0;
    v172 = 0.0;
    v173 = 0.0;
    v146.x = 1.0;
    v146.y = 0.0;
    v146.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v29 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v29 = 0;
    v245 = v29;
    v263 = 0;
    v218 = 0;
    v177 = 0;
    v156 = 0;
    v248 = 0.0;
    v205 = 0.0;
    v251 = 0.0;
    v208 = 0.0;
    v252 = 0;
    v253 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;        
        Pompier_Police_mode_hyllis_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_hyllis_univVars_Flag_speciaux_sauve,
            (void*)Pompier_Police_mode_hyllis_univVars,
            4488);
        Pompier_Police_mode_hyllis__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Pompier_Police_mode_hyllis_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_hyllis_GST_Global_gao_o_joy_acteur,
            (void*)Pompier_Police_mode_hyllis_GST_Global_gao,
            136);
        Pompier_Police_mode_hyllis__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_hyllis_univVars_f_sally_fric,
            (void*)Pompier_Police_mode_hyllis_univVars,
            1200);
        Pompier_Police_mode_hyllis__PJ_Camera_Speciale_gao = AI_ul_RealResolveOneRefObject(0x9E0007E2);
        Pompier_Police_mode_hyllis_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao = AI_ul_RealResolveOneRefObject(0x5E005FA2u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_hyllis_GST_Board_panel_gao_Gen_Show,
            (void*)Pompier_Police_mode_hyllis_GST_Board_panel_gao,
            2260);
        fct_Pompier_Police_mode_hyllis_Pompier_Police_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72005407u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_hyllis_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour,
            (void*)Pompier_Police_mode_hyllis_GST_Board_panel_gao,
            3208);
        Pompier_Police_mode_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_INV_manager_gao_i_Fric2Add,
            (void*)Pompier_Police_mode_INV_manager_gao,
            21476);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_INV_manager_gao_i_FricWithPerso,
            (void*)Pompier_Police_mode_INV_manager_gao,
            21484);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_INV_manager_gao_f_PogneAppearTimeInit,
            (void*)Pompier_Police_mode_INV_manager_gao,
            21504);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_INV_manager_gao_f_PogneAppearTimeFixed,
            (void*)Pompier_Police_mode_INV_manager_gao,
            21496);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_INV_manager_gao_f_PogneDelay,
            (void*)Pompier_Police_mode_INV_manager_gao,
            21500);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_INV_manager_gao_i_OpeningMode,
            (void*)Pompier_Police_mode_INV_manager_gao,
            100);
        fct_Pompier_Police_mode_Pompier_Pompier_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x72005349u);
    }
    v177 = AI_EvalFunc_ARRBitTest_C((int*)(Pompier_Police_mode_hyllis_univVars_Flag_speciaux_sauve + 4), 2302);
    if (v177)
    {
        v231 = AI_EvalFunc_OBJDestroy_C(AI_gpst_CurrentGameObject);
        if (v231 != 1000)
            return v231;
    }
    if (v245->i_current_mode != 3)
    {
        v245->i_current_mode = 3;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v245->v_start_pos);
        v92 = &v245->v_last_pos;
        v93 = &v245->v_start_pos;
        v245->v_last_pos.x = v245->v_start_pos.x;
        v92->y = v93->y;
        v92->z = v93->z;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v245->v_start_pos);
        AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v121);
        v117.x = 0.0;
        v117.y = 0.0;
        v117.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v121, &v117);
        v245->f_speed = 600.0;
        v245->i_flag_snap = 0;
        v245->o_next_wp = 0;
        v245->i_flag_stream_played = 0;
    }
    v91 = (tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao;
    if (Pompier_Police_mode_hyllis__PJ_Aircraft_gao)
    {
        if ((unsigned int)Pompier_Police_mode_hyllis__PJ_Aircraft_gao <= 2)
            v91 = AI_MainActor[Pompier_Police_mode_hyllis__PJ_Aircraft_gao - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v91);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v179 = (float)v1;
    if (v179 >= 90000.0)
    {
        if (v245->i_snd_motor_loop != -1)
        {
            SND_Release(v245->i_snd_motor_loop);
            v245->i_snd_motor_loop = -1;
        }
        if (v245->i_snd_sirene_loop != -1)
        {
            SND_Release(v245->i_snd_sirene_loop);
            v245->i_snd_sirene_loop = -1;
        }
    }
    else
    {
        if (v245->i_snd_motor_loop == -1)
        {
            v2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
            v245->i_snd_motor_loop = v2;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v245->i_snd_motor_loop);
        }
        if (v245->i_snd_sirene_loop == -1)
        {
            v3 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 4, 402653184);
            v245->i_snd_sirene_loop = v3;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v245->i_snd_sirene_loop);
        }
        v90 = (tGameObjectMain*)v245->des_msk_gao;
        if (v90)
        {
            if ((unsigned int)v90 <= 2)
                v90 = AI_MainActor[(uint32_t)v90 - 1];
            v89 = v90;
        }
        else
        {
            v89 = 0;
        }
        if (v89 && !v245->i_flag_stream_played)
        {
            v245->i_flag_stream_played = 1;
            v88 = (tGameObjectMain*)v245->des_msk_gao;
            if (v88)
            {
                if ((unsigned int)v88 <= 2)
                    v88 = AI_MainActor[(uint32_t)v88 - 1];
                v4 = AI_EvalFunc_SNDRequest_C(v88, 0, 0x8000080);
            }
            else
            {
                v4 = AI_EvalFunc_SNDRequest_C(0, 0, 0x8000080);
            }
            v218 = v4;
            if (v4 != -1)
                AI_EvalFunc_SNDPlaySoundOnTrack_C(AI_gpst_CurrentGameObject, v218);
        }
    }
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v254);
    a1 = v254;
    v252 = AI_EvalFunc_ARRBitTest_C((int*)(Pompier_Police_mode_hyllis_univVars_Flag_speciaux_sauve + 4), 2718);
    if (v252)
        goto LABEL_47;
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(256))
    {
        v87 = *(tGameObjectMain**)Pompier_Police_mode_hyllis_GST_Global_gao_o_joy_acteur;
        if (*(uint32_t*)Pompier_Police_mode_hyllis_GST_Global_gao_o_joy_acteur)
        {
            if ((unsigned int)v87 <= 2)
                v87 = AI_MainActor[(uint32_t)v87 - 1];
            v86 = v87;
        }
        else
        {
            v86 = 0;
        }
        if (v86)
        {
        LABEL_47:
            v245->i_flag_compute_best_way = 1;
            v245->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
            v245->o_aircraft = 0;
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
            AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_gao, 0);
            AI_EvalFunc_MSGClear_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao);
            Src.msgID = 32;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 1;
            Src.Int2 = 1;
            Src.f2 = 0.0;
            Src.f1 = 0.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao, &Src);
        }
    }
    v208 = (float)*(int*)Pompier_Police_mode_hyllis_univVars_f_sally_fric;
    v166 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
    if (v166 && (v208 > 0.0 || v245->i_flag_snap))
    {
        v85 = (tGameObjectMain*)v245->o_aircraft;
        if (v85)
        {
            if ((unsigned int)v85 <= 2)
                v85 = AI_MainActor[(uint32_t)v85 - 1];
            v84 = v85;
        }
        else
        {
            v84 = 0;
        }
        if (!v84)
        {
            if (AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Speciale_gao, 0x100))
                AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis_GST_Board_panel_gao, 0x400, 0);
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_gao, &v163);
            AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v163);
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_gao, &v211);
            v187.x = 0.0;
            v187.y = 0.0;
            v187.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(
                (tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao,
                &v211,
                &v187);
            v245->o_aircraft = Pompier_Police_mode_hyllis__PJ_Aircraft_gao;
            v245->i_flag_compute_best_way = 1;
            v245->f_time_fine = 0.0;
            Src.msgID = 32;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 1;
            Src.Int2 = 0;
            Src.f2 = 0.0;
            Src.f1 = 0.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao, &Src);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v152);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao, &v215);
        MATH_SubstractVec(&dest, &v152, &v215);
        a1.x = dest.x;
        a1.y = dest.y;
        a1.z = 0.0;
        v248 = MATH_VecLen(&a1);
        if (v248 != 0.0f)
        {
            v83 = 1.0f / v248;
            a1.x = a1.x * v83;
            a1.y = a1.y * v83;
            a1.z = a1.z * v83;
        }
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao, &v169);
        v233 = a1.x * 10.0f;
        v234 = a1.y * 10.0f;
        v235 = a1.z * 10.0f;
        v146.x = v169.x - v233;
        v146.y = v169.y - v234;
        v146.z = v169.z - v235;
        v174.x = 0.0;
        v174.y = 0.0;
        v174.z = 5.0;
        v146.x = v146.x + 0.0f;
        v146.y = v146.y + 0.0f;
        v146.z = v146.z + 5.0f;
        v193 = TIM_gf_dt;
        v82 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v82 = 0.0;
        if (v82 > 1.0)
            v82 = 1.0;
        v257 = 6.0f * v82 + v245->f_cam_blend_speed * (1.0f - v82);
        v245->f_cam_blend_speed = v257;
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v133);
        v265 = TIM_gf_dt;
        a4 = v245->f_cam_blend_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v133, &v146, a4, &v209);
        v146 = v209;
        AI_EvalFunc_DYNSpeedGetVector_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao, &v114);
        v140 = TIM_gf_dt;
        v227 = v114.x * TIM_gf_dt;
        v228 = v114.y * TIM_gf_dt;
        v229 = v114.z * TIM_gf_dt;
        v146.x = v146.x + v227;
        v146.y = v146.y + v228;
        v146.z = v146.z + v229;
        *(uint32_t*)Pompier_Police_mode_hyllis_GST_Board_panel_gao_Gen_Show = 0;
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis_GST_Board_panel_gao, 0x200, 0);
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis_GST_Board_panel_gao, 0x100, 0);
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_gao, 1);
        v258 = AI_EvalFunc_VIEWAssignObject_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, 0);
        AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v146);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v181);
        MATH_SubstractVec(&v158, &v181, &v146);
        a3 = v158;
        MATH_NormalizeEqualVector(&a3);
        AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v216);
        v139 = a3.z * v216.z + a3.y * v216.y + v216.x * a3.x;
        if (v139 <= 0.0)
        {
            AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v127);
            v230 = a3.z * v127.z + a3.y * v127.y + v127.x * a3.x;
            if (v230 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v160);
                AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v94);
                v222.x = -v94.x;
                v222.y = -v94.y;
                v222.z = -v94.z;
                v101 = TIM_gf_dt;
                a4c = v245->f_cam_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v160, &v222, a4c, &v184);
                a3 = v184;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v262);
                AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &v135);
                v176 = TIM_gf_dt;
                a4b = v245->f_cam_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v262, &v135, a4b, &v132);
                a3 = v132;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &a2);
            v264 = TIM_gf_dt;
            a4a = v245->f_cam_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4a, &a5);
            a3 = a5;
        }
        v128.x = 0.0;
        v128.y = 0.0;
        v128.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_Derriere_gao, &a3, &v128);
    }
    if (!v245->i_flag_snap)
    {
        if (v245->i_flag_compute_best_way)
        {
            v245->i_flag_compute_best_way = 0;
            v263 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v245->n_net, -1, 0, 0);
            v81 = (tGameObjectMain*)v245->o_aircraft;
            if (v81)
            {
                if ((unsigned int)v81 <= 2)
                    v81 = AI_MainActor[(uint32_t)v81 - 1];
                v80 = v81;
            }
            else
            {
                v80 = 0;
            }
            if (v80)
            {
                v79 = (tGameObjectMain*)v245->o_aircraft;
                if (v79)
                {
                    if ((unsigned int)v79 <= 2)
                        v79 = AI_MainActor[(uint32_t)v79 - 1];
                    v5 = AI_EvalFunc_WAYNearestWPOfObj_C(v79, (tWayNetwork*)v245->n_net, -1, 1, 3);
                }
                else
                {
                    v5 = AI_EvalFunc_WAYNearestWPOfObj_C(0, (tWayNetwork*)v245->n_net, -1, 1, 3);
                }
                v156 = (unsigned int)v5;
            }
            else
            {
                v78 = (tGameObjectMain*)v245->o_start_pos_wp;
                if (v78)
                {
                    if ((unsigned int)v78 <= 2)
                        v78 = AI_MainActor[(uint32_t)v78 - 1];
                    v6 = AI_EvalFunc_WAYNearestWPOfObj_C(v78, (tWayNetwork*)v245->n_net, -1, 0, 0);
                }
                else
                {
                    v6 = AI_EvalFunc_WAYNearestWPOfObj_C(0, (tWayNetwork*)v245->n_net, -1, 0, 0);
                }
                v156 = (unsigned int)v6;
            }
            v245->o_next_wp = (int)v263;
            v77 = (tGameObjectMain*)v156;
            if (v156)
            {
                if (v156 <= 2)
                    v77 = AI_MainActor[v156 - 1];
                v76 = v77;
            }
            else
            {
                v76 = 0;
            }
            v75 = v263;
            if (v263)
            {
                if ((unsigned int)v263 <= 2)
                    v75 = AI_MainActor[(uint32_t)v263 - 1];
                v74 = v75;
            }
            else
            {
                v74 = 0;
            }
            v7 = AI_EvalFunc_WAYShortWay_C(
                AI_gpst_CurrentGameObject,
                (tWayNetwork*)v245->n_net,
                (tGameObjectMain**)v245->ao_wp,
                v74,
                v76);
            v245->i_wp_nb = v7;
            if (v245->i_wp_nb > 1)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v245->v_start_pos);
                v245->f_rot_blend_speed = 0.0;
                v73 = (tGameObjectMain*)v245->ao_wp[v245->i_wp_nb - 1]; // FIXME
                if (v73)
                {
                    if ((unsigned int)v73 <= 2)
                        v73 = AI_MainActor[(uint32_t)v73 - 1];
                    AI_EvalFunc_OBJGetPos_C(v73, &v110);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v110);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v98);
                MATH_SubstractVec(&v161, &v110, &v98);
                v72 = (tGameObjectMain*)v245->ao_wp[v245->i_wp_nb - 2]; // FIXME
                if (v72)
                {
                    if ((unsigned int)v72 <= 2)
                        v72 = AI_MainActor[(uint32_t)v72 - 1];
                    AI_EvalFunc_OBJGetPos_C(v72, &v97);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v97);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v217);
                MATH_SubstractVec(&v165, &v97, &v217);
                v207 = v165.z * v161.z + v165.y * v161.y + v161.x * v165.x;
                if (v207 < 0.0) {
                    v245->ao_wp[--v245->i_wp_nb] = 0;
                }
            }
        }
        v71 = (tGameObjectMain*)v245->o_next_wp;
        if (v71)
        {
            if ((unsigned int)v71 <= 2)
                v71 = AI_MainActor[(uint32_t)v71 - 1];
            v70 = v71;
        }
        else
        {
            v70 = 0;
        }
        if (v70)
        {
            if (v245->i_wp_nb != 1)
                goto LABEL_140;
            v69 = (tGameObjectMain*)v245->o_aircraft;
            if (v69)
            {
                if ((unsigned int)v69 <= 2)
                    v69 = AI_MainActor[(uint32_t)v69 - 1];
                v68 = v69;
            }
            else
            {
                v68 = 0;
            }
            if (!v68)
                goto LABEL_140;
            v67 = (tGameObjectMain*)v245->o_aircraft;
            if (v67)
            {
                if ((unsigned int)v67 <= 2)
                    v67 = AI_MainActor[(uint32_t)v67 - 1];
                v8 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v67);
            }
            else
            {
                v8 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v221 = (float)v8;
            v66 = (tGameObjectMain*)v245->o_next_wp;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v66 = AI_MainActor[(uint32_t)v66 - 1];
                v9 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v66);
            }
            else
            {
                v9 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v232 = (float)v9;
            if (v221 < (double)v232)
            {
                v245->i_wp_nb = 0;
                v245->f_rot_blend_speed = 0.0;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v245->v_start_pos);
            }
            else
            {
            LABEL_140:
                v65 = (tGameObjectMain*)v245->o_next_wp;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v65 = AI_MainActor[(uint32_t)v65 - 1];
                    v10 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v65);
                }
                else
                {
                    v10 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
                }
                v206 = v10;
                if (v10)
                    goto LABEL_157;
                v64 = (tGameObjectMain*)v245->o_next_wp;
                if (v64)
                {
                    if ((unsigned int)v64 <= 2)
                        v64 = AI_MainActor[(uint32_t)v64 - 1];
                    AI_EvalFunc_OBJGetPos_C(v64, &v175);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v175);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v224);
                MATH_SubstractVec(&v185, &v175, &v224);
                v63 = (tGameObjectMain*)v245->o_next_wp;
                if (v63)
                {
                    if ((unsigned int)v63 <= 2)
                        v63 = AI_MainActor[(uint32_t)v63 - 1];
                    AI_EvalFunc_OBJGetPos_C(v63, &v119);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v119);
                }
                MATH_SubstractVec(&v96, &v119, &v245->v_start_pos);
                v103 = v96.z * v185.z + v96.y * v185.y + v185.x * v96.x;
                if (v103 < 0.0)
                {
                LABEL_157:
                    v62 = (tGameObjectMain*)v245->o_next_wp;
                    if (v62)
                    {
                        if ((unsigned int)v62 <= 2)
                            v62 = AI_MainActor[(uint32_t)v62 - 1];
                        v61 = v62;
                    }
                    else
                    {
                        v61 = 0;
                    }
                    v60 = (tGameObjectMain*)v245->o_start_pos_wp;
                    if (v60)
                    {
                        if ((unsigned int)v60 <= 2)
                            v60 = AI_MainActor[(uint32_t)v60 - 1];
                        v59 = v60;
                    }
                    else
                    {
                        v59 = 0;
                    }
                    if (v61 == v59)
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                        v266 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pompier_Police_mode_hyllis_Pompier_Police_mode_wait);
                        if (v266 != 1000)
                            return v266;
                    }
                    v245->f_rot_blend_speed = 0.0;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v245->v_start_pos);
                    --v245->i_wp_nb;
                }
            }
        }
        if (v245->i_wp_nb)
        {
            v245->o_next_wp = *(&v245->__ao_wp_Dimension1 + v245->i_wp_nb);
        }
        else
        {
            v58 = (tGameObjectMain*)v245->o_aircraft;
            if (v58)
            {
                if ((unsigned int)v58 <= 2)
                    v58 = AI_MainActor[(uint32_t)v58 - 1];
                v57 = v58;
            }
            else
            {
                v57 = 0;
            }
            if (v57)
                v245->o_next_wp = 0;
            else
                v245->o_next_wp = v245->o_start_pos_wp;
        }
        v56 = (tGameObjectMain*)v245->o_next_wp;
        if (v56)
        {
            if ((unsigned int)v56 <= 2)
                v56 = AI_MainActor[(uint32_t)v56 - 1];
            v55 = v56;
        }
        else
        {
            v55 = 0;
        }
        if (v55)
        {
            v54 = (tGameObjectMain*)v245->o_next_wp;
            if (v54)
            {
                if ((unsigned int)v54 <= 2)
                    v54 = AI_MainActor[(uint32_t)v54 - 1];
                AI_EvalFunc_OBJGetPos_C(v54, &v200);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v200);
            }
            v213.x = v200.x;
            v213.y = v200.y;
            v213.z = 40.0;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v214);
            v213.x = v213.x - v214.x;
            v213.y = v213.y - v214.y;
            v213.z = v213.z - v214.z;
            v11 = MATH_VecLen(&v213);
            v245->f_dist_to_dest = (float)v11;
        }
        else
        {
            v53 = (tGameObjectMain*)v245->o_aircraft;
            if (v53)
            {
                if ((unsigned int)v53 <= 2)
                    v53 = AI_MainActor[(uint32_t)v53 - 1];
                AI_EvalFunc_OBJGetPos_C(v53, &v120);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v120);
            }
            v213.x = v120.x;
            v213.y = v120.y;
            v213.z = v120.z + 20.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v155);
            v213.x = v213.x - v155.x;
            v213.y = v213.y - v155.y;
            v213.z = v213.z - v155.z;
            v12 = MATH_VecLen(&v213);
            v245->f_dist_to_dest = (float)v12;
            if (v245->f_dist_to_dest < 100.0)
            {
                v245->i_wp_nb = 0;
                v245->f_rot_blend_speed = 0.0;
                v245->o_next_wp = 0;
                v245->i_flag_snap = 1;
            }
        }
    }
    if (v245->i_flag_snap)
    {
        v245->f_banking_coef = 0.0;
        v195 = TIM_gf_dt;
        v42 = 0.5f * TIM_gf_dt;
        if (v42 < 0.0)
            v42 = 0.0;
        if (v42 > 1.0)
            v42 = 1.0;
        v149 = 3.0f * v42 + v245->f_rot_blend_speed * (1.0f - v42);
        v245->f_rot_blend_speed = v149;
        v41 = (tGameObjectMain*)v245->o_aircraft;
        if (v41)
        {
            if ((unsigned int)v41 <= 2)
                v41 = AI_MainActor[(uint32_t)v41 - 1];
            AI_EvalFunc_OBJGetPos_C(v41, &v113);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v113);
        }
        v237 = a1.x * 20.0f;
        v238 = a1.y * 20.0f;
        v239 = a1.z * 20.0f;
        v213.x = v113.x + v237;
        v213.y = v113.y + v238;
        v213.z = 8.0;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v183);
        v213.x = v213.x - v183.x;
        v213.y = v213.y - v183.y;
        v213.z = v213.z - v183.z;
        v13 = MATH_VecLen(&v213);
        v245->f_dist_to_dest = (float)v13;
        v40 = 1.0f / v245->f_dist_to_dest;
        v213.x = v213.x * v40;
        v213.y = v213.y * v40;
        v213.z = v213.z * v40;
        if (v245->f_dist_to_dest > 600.0)
            v25 = 600.0;
        else
            v25 = v245->f_dist_to_dest;
        v245->f_speed = v25;
        v250 = TIM_gf_dt;
        v39 = v245->f_speed * TIM_gf_dt;
        v182.x = v213.x * v39;
        v182.y = v213.y * v39;
        v182.z = v213.z * v39;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v182);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v136);
        v170.x = -a1.x;
        v170.y = -a1.y;
        v170.z = -a1.z;
        v205 = -a1.z * v136.z
            + -a1.y * v136.y
            + v136.x * -a1.x;
        if (v245->f_time_fine == 0.0)
        {
            if (v245->f_dist_to_dest < 2.0 && v205 > 0.99619502)
            {
                v245->f_time_fine = TIM_gf_MainClock - TIM_gf_LockTime;
                *(uint32_t*)Pompier_Police_mode_hyllis_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour = 0;
                v38 = (float)*(int*)Pompier_Police_mode_hyllis_univVars_f_sally_fric;
                if (v38 < 250.0)
                    v24 = (float)*(int*)Pompier_Police_mode_hyllis_univVars_f_sally_fric;
                else
                    v24 = 250.0;
                v248 = v24;
                v37 = MATH_gd_magicDoubleForRound - v24;
                *(uint32_t*)Pompier_Police_mode_INV_manager_gao_i_Fric2Add = (*(uint32_t *)&v37);
                *(uint32_t*)Pompier_Police_mode_INV_manager_gao_i_FricWithPerso = 12;
                *(uint32_t*)Pompier_Police_mode_INV_manager_gao_f_PogneAppearTimeInit = 0x3F800000;
                *(uint32_t*)Pompier_Police_mode_INV_manager_gao_f_PogneAppearTimeFixed = 0x3E4CCCCD;
                *(uint32_t*)Pompier_Police_mode_INV_manager_gao_f_PogneDelay = 0x3E4CCCCD;
                *(uint32_t*)Pompier_Police_mode_INV_manager_gao_i_OpeningMode = 19;
                AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis_GST_Board_panel_gao, 0, 0x100);
                AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis_GST_Board_panel_gao, 0x1000, 0);
            }
        }
        else if (*(uint32_t*)Pompier_Police_mode_hyllis_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour)
        {
            if (!AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Pompier_Police_mode_hyllis_GST_Board_panel_gao, 0x1000))
            {
                *(uint32_t*)Pompier_Police_mode_hyllis_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour = 0;
                *(uint32_t*)Pompier_Police_mode_hyllis_GST_Board_panel_gao_Gen_Show = 1;
                v245->f_speed = 0.0;
                v245->f_rot_blend_speed = 0.0;
                v245->i_flag_snap = 0;
                v245->i_flag_compute_best_way = 1;
                v245->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
                v245->o_aircraft = 0;
                AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
                AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Camera_gao, 0);
                AI_EvalFunc_MSGClear_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao);
                Src.msgID = 32;
                Src.gao = AI_gpst_CurrentGameObject;
                Src.gao2 = 0;
                Src.Int1 = 1;
                Src.Int2 = 1;
                Src.f2 = 0.0;
                Src.f1 = 0.0;
                Src.v1.x = 0.0;
                Src.v1.y = 0.0;
                Src.v1.z = 0.0;
                Src.v2.x = 0.0;
                Src.v2.y = 0.0;
                Src.v2.z = 0.0;
                AI_EvalFunc_MSGSend_C((tGameObjectMain*)Pompier_Police_mode_hyllis__PJ_Aircraft_gao, &Src);
            }
        }
        if (v205 <= 0.0)
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v111);
            v141.x = -a1.x;
            v141.y = -a1.y;
            v141.z = -a1.z;
            v197 = -a1.z * v111.z
                + -a1.y * v111.y
                + v111.x * -a1.x;
            if (v197 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v109);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v145);
                v186.x = -v145.x;
                v186.y = -v145.y;
                v186.z = -v145.z;
                v246 = TIM_gf_dt;
                a4i = v245->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v109, &v186, a4i, &v116);
                a3 = v116;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v242);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v108);
                v150 = TIM_gf_dt;
                a4h = v245->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v242, &v108, a4h, &v243);
                a3 = v243;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v115);
            v143.x = -a1.x;
            v143.y = -a1.y;
            v143.z = -a1.z;
            v194 = TIM_gf_dt;
            a4g = v245->f_rot_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v115, &v143, a4g, &v244);
            a3 = v244;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v144);
        v188 = 0;
        v189 = 0.0;
        v190 = v144.z - v245->v_last_pos.z;
        v36 = &v245->v_grav_pos;
        v245->v_grav_pos.x = v245->v_grav_pos.x + 0.0f;
        v36->y = v189 + v36->y;
        v36->z = v190 + v36->z;
        v212 = v245->v_grav_pos;
        v240.x = 0.0;
        v240.y = 0.0;
        v240.z = -40.0;
        v171 = 0.0;
        v172 = 0.0;
        v173 = -40.0;
        v105 = v245->v_grav_speed.x * 0.94999999f;
        v106 = v245->v_grav_speed.y * 0.94999999f;
        v107 = v245->v_grav_speed.z * 0.94999999f;
        v171 = v105 + 0.0f;
        v172 = v106 + 0.0f;
        v173 = v107 + -40.0f;
        v157 = TIM_gf_dt;
        v201 = v171 * TIM_gf_dt;
        v202 = v172 * TIM_gf_dt;
        v203 = v173 * TIM_gf_dt;
        v35 = &v245->v_grav_pos;
        v245->v_grav_pos.x = v201 + v245->v_grav_pos.x;
        v35->y = v202 + v35->y;
        v35->z = v203 + v35->z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v255);
        v34 = &v245->v_grav_pos;
        v245->v_grav_pos.x = v245->v_grav_pos.x - v255.x;
        v34->y = v34->y - v255.y;
        v34->z = v34->z - v255.z;
        v248 = MATH_VecLen(&v245->v_grav_pos);
        if (v248 != 0.0f)
        {
            v32 = &v245->v_grav_pos;
            v33 = 1.0f / v248;
            v245->v_grav_pos.x = v33 * v245->v_grav_pos.x;
            v32->y = v33 * v32->y;
            v32->z = v33 * v32->z;
            v151.x = -v245->v_grav_pos.x;
            v151.y = -v245->v_grav_pos.y;
            v151.z = -v245->v_grav_pos.z;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a3, &v151);
            v31 = &v245->v_grav_pos;
            v245->v_grav_pos.x = v245->v_grav_pos.x * 50.0f;
            v31->y = v31->y * 50.0f;
            v31->z = v31->z * 50.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v199);
            v30 = &v245->v_grav_pos;
            v245->v_grav_pos.x = v199.x + v245->v_grav_pos.x;
            v30->y = v199.y + v30->y;
            v30->z = v199.z + v30->z;
        }
        MATH_SubstractVec(&v256, &v245->v_grav_pos, &v212);
        v123 = TIM_gf_dt;
        MATH_VecMultNormScalar(&v154, &v256, TIM_gf_dt);
        v245->v_grav_speed = v154;
    }
    else
    {
        v245->f_time_fine = 0.0;
        v245->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
        v52 = v245->f_dist_to_dest / 200.0f;
        if (v52 > 1.0)
            v28 = 1.0;
        else
            v28 = v245->f_dist_to_dest / 200.0f;
        v102 = v28;
        v253 = 1.0f - v28;
        v180 = TIM_gf_dt;
        v51 = 2.0f * TIM_gf_dt;
        if (v253 < (double)v51)
            v27 = 2.0f * TIM_gf_dt;
        else
            v27 = v253;
        v168 = v27;
        v253 = v27;
        v50 = v27;
        if (v27 < 0.0)
            v50 = 0.0;
        if (v50 > 1.0)
            v50 = 1.0;
        v245->f_rot_blend_speed = 12.0f * v50 + v245->f_rot_blend_speed * (1.0f - v50);
        if (v245->f_dist_to_dest != 0.0f)
        {
            v49 = 1.0f / v245->f_dist_to_dest;
            v213.x = v213.x * v49;
            v213.y = v213.y * v49;
            v213.z = v213.z * v49;
            v259 = TIM_gf_dt;
            v48 = 0.5f * TIM_gf_dt;
            if (v48 < 0.0)
                v48 = 0.0;
            if (v48 > 1.0)
                v48 = 1.0;
            v129 = 600.0f * v48 + v245->f_speed * (1.0f - v48);
            v245->f_speed = v129;
            v47 = (tGameObjectMain*)v245->o_next_wp;
            if (v47)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                v46 = v47;
            }
            else
            {
                v46 = 0;
            }
            if (!v46)
            {
                if (v245->f_speed > (double)v245->f_dist_to_dest)
                    v26 = v245->f_dist_to_dest;
                else
                    v26 = v245->f_speed;
                v245->f_speed = v26;
            }
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v134);
            v153.x = 0.0;
            v153.y = 0.0;
            v153.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v134, &v153);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v126);
            v219 = v213.z * v126.z + v213.y * v126.y + v126.x * v213.x;
            if (v219 <= 0.0)
            {
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v210);
                v167 = v213.z * v210.z + v213.y * v210.y + v210.x * v213.x;
                if (v167 <= 0.0)
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v138);
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v95);
                    v226.x = -v95.x;
                    v226.y = -v95.y;
                    v226.z = -v95.z;
                    v178 = TIM_gf_dt;
                    a4f = v245->f_rot_blend_speed * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v138, &v226, a4f, &v137);
                    a3 = v137;
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v125);
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v260);
                    v220 = TIM_gf_dt;
                    a4e = v245->f_rot_blend_speed * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v125, &v260, a4e, &v159);
                    a3 = v159;
                }
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v164);
                v130 = TIM_gf_dt;
                a4d = v245->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v164, &v213, a4d, &v261);
                a3 = v261;
            }
            v99.x = 0.0;
            v99.y = 0.0;
            v99.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v99);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v223);
            v251 = v213.z * v223.z + v213.y * v223.y + v223.x * v213.x;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v131);
            v104 = v213.z * v131.z + v213.y * v131.y + v131.x * v213.x;
            if (v104 < 0.0)
                v251 = AI_EvalFunc_MTHFloatSign_C(v251);
            v247 = TIM_gf_dt;
            v45 = TIM_gf_dt;
            if (TIM_gf_dt < 0.0)
                v45 = 0.0;
            if (v45 > 1.0)
                v45 = 1.0;
            v196 = v251 * 1.570796f * v45 + v245->f_banking_coef * (1.0f - v45);
            v245->f_banking_coef = v196;
            AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, v245->f_banking_coef);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v142);
            v44 = v245->f_speed;
            v43 = &v245->v_grav_speed;
            v245->v_grav_speed.x = v142.x * v44;
            v43->y = v142.y * v44;
            v43->z = v142.z * v44;
            v249 = TIM_gf_dt;
            v191.x = TIM_gf_dt * v245->v_grav_speed.x;
            v191.y = v245->v_grav_speed.y * TIM_gf_dt;
            v191.z = v245->v_grav_speed.z * TIM_gf_dt;
            AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v191);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v147);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v112);
            v236.x = v112.x * 50.0f;
            v236.y = v112.y * 50.0f;
            v236.z = v112.z * 50.0f;
            MATH_SubstractVec(&v241, &v147, &v236);
            v245->v_grav_pos = v241;
        }
    }
    v204 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Police_mode_Pompier_Pompier_exec_GFX_Halo);
    if (v204 != 1000)
        return v204;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v245->v_last_pos);
    return 1000;
}

int Pompier_Pompier_mode_hyllis()
{
    double v0; // st7
    int v1; // eax
    int v2; // eax
    int v3; // eax
    unsigned int v4; // eax
    BOOL v5; // eax
    double v7; // st7
    double v8; // st7
    int v9; // eax
    double v10; // st7
    double v11; // st7
    double v12; // st7
    float a4; // [esp+0h] [ebp-60Ch]
    float a4a; // [esp+0h] [ebp-60Ch]
    float a4b; // [esp+0h] [ebp-60Ch]
    float a4c; // [esp+0h] [ebp-60Ch]
    float a4d; // [esp+0h] [ebp-60Ch]
    float a4e; // [esp+0h] [ebp-60Ch]
    float v19; // [esp+8h] [ebp-604h]
    float v20; // [esp+Ch] [ebp-600h]
    float v21; // [esp+10h] [ebp-5FCh]
    tScriptStruct_PNJPompier* v22; // [esp+14h] [ebp-5F8h]
    Vec3* v23; // [esp+24h] [ebp-5E8h]
    Vec3* v24; // [esp+28h] [ebp-5E4h]
    Vec3* v25; // [esp+34h] [ebp-5D8h]
    float v26; // [esp+38h] [ebp-5D4h]
    Vec3* v27; // [esp+40h] [ebp-5CCh]
    Vec3* v28; // [esp+44h] [ebp-5C8h]
    Vec3* v29; // [esp+5Ch] [ebp-5B0h]
    tGameObjectMain* v30; // [esp+7Ch] [ebp-590h]
    tGameObjectMain* v31; // [esp+8Ch] [ebp-580h]
    tGameObjectMain* v32; // [esp+94h] [ebp-578h]
    float v33; // [esp+98h] [ebp-574h]
    float v34; // [esp+A4h] [ebp-568h]
    tGameObjectMain* v35; // [esp+B0h] [ebp-55Ch]
    tGameObjectMain* v36; // [esp+C0h] [ebp-54Ch]
    float v37; // [esp+C4h] [ebp-548h]
    Vec3* v38; // [esp+D4h] [ebp-538h]
    float v39; // [esp+D8h] [ebp-534h]
    float v40; // [esp+DCh] [ebp-530h]
    float v41; // [esp+E0h] [ebp-52Ch]
    float v42; // [esp+108h] [ebp-504h]
    float v43; // [esp+10Ch] [ebp-500h]
    float v44; // [esp+110h] [ebp-4FCh]
    float v45; // [esp+114h] [ebp-4F8h]
    tGameObjectMain* v46; // [esp+128h] [ebp-4E4h]
    tGameObjectMain* v47; // [esp+13Ch] [ebp-4D0h]
    tGameObjectMain* v48; // [esp+140h] [ebp-4CCh]
    tGameObjectMain* v49; // [esp+144h] [ebp-4C8h]
    tGameObjectMain* v50; // [esp+148h] [ebp-4C4h]
    tGameObjectMain* v51; // [esp+14Ch] [ebp-4C0h]
    tGameObjectMain* v52; // [esp+150h] [ebp-4BCh]
    tGameObjectMain* v53; // [esp+154h] [ebp-4B8h]
    tGameObjectMain* v54; // [esp+158h] [ebp-4B4h]
    tGameObjectMain* v55; // [esp+15Ch] [ebp-4B0h]
    tGameObjectMain* v56; // [esp+164h] [ebp-4A8h]
    tGameObjectMain* v57; // [esp+16Ch] [ebp-4A0h]
    tGameObjectMain* v58; // [esp+174h] [ebp-498h]
    tGameObjectMain* v59; // [esp+17Ch] [ebp-490h]
    tGameObjectMain* v60; // [esp+184h] [ebp-488h]
    tGameObjectMain* v61; // [esp+188h] [ebp-484h]
    tGameObjectMain* v62; // [esp+18Ch] [ebp-480h]
    tGameObjectMain* v63; // [esp+190h] [ebp-47Ch]
    tGameObjectMain* v64; // [esp+194h] [ebp-478h]
    tGameObjectMain* v65; // [esp+19Ch] [ebp-470h]
    tGameObjectMain* v66; // [esp+1A4h] [ebp-468h]
    tGameObjectMain* v67; // [esp+1ACh] [ebp-460h]
    tGameObjectMain* v68; // [esp+1B4h] [ebp-458h]
    tGameObjectMain* v69; // [esp+1C0h] [ebp-44Ch]
    tGameObjectMain* v70; // [esp+1C4h] [ebp-448h]
    tGameObjectMain* v71; // [esp+1CCh] [ebp-440h]
    Vec3* v72; // [esp+1D0h] [ebp-43Ch]
    Vec3* v73; // [esp+1D4h] [ebp-438h]
    Vec3 v74; // [esp+1D8h] [ebp-434h] BYREF
    Vec3 v75; // [esp+1F0h] [ebp-41Ch] BYREF
    Vec3 v76; // [esp+1FCh] [ebp-410h] BYREF
    Vec3 v77; // [esp+208h] [ebp-404h] BYREF
    Vec3 v78; // [esp+214h] [ebp-3F8h] BYREF
    float v79; // [esp+220h] [ebp-3ECh]
    float v80; // [esp+224h] [ebp-3E8h]
    float v81; // [esp+228h] [ebp-3E4h]
    float v82; // [esp+22Ch] [ebp-3E0h]
    float v83; // [esp+234h] [ebp-3D8h]
    Vec3 v84; // [esp+238h] [ebp-3D4h] BYREF
    Vec3 v85; // [esp+250h] [ebp-3BCh] BYREF
    float v86; // [esp+25Ch] [ebp-3B0h]
    float v87; // [esp+260h] [ebp-3ACh]
    float v88; // [esp+268h] [ebp-3A4h]
    float v89; // [esp+278h] [ebp-394h]
    float v90; // [esp+27Ch] [ebp-390h]
    float v91; // [esp+280h] [ebp-38Ch]
    Vec3 a3; // [esp+284h] [ebp-388h] BYREF
    float v93; // [esp+290h] [ebp-37Ch]
    float v94; // [esp+294h] [ebp-378h]
    Vec3 v95; // [esp+298h] [ebp-374h] BYREF
    Vec3 v96; // [esp+2A4h] [ebp-368h] BYREF
    Vec3 v97; // [esp+2B0h] [ebp-35Ch] BYREF
    float v98; // [esp+2BCh] [ebp-350h]
    float v99; // [esp+2C0h] [ebp-34Ch]
    float v100; // [esp+2C4h] [ebp-348h]
    Vec3 v101; // [esp+2C8h] [ebp-344h] BYREF
    Vec3 v102; // [esp+2E8h] [ebp-324h] BYREF
    Vec3 v103; // [esp+2F4h] [ebp-318h] BYREF
    Vec3 v104; // [esp+300h] [ebp-30Ch] BYREF
    unsigned int v105; // [esp+30Ch] [ebp-300h]
    float v106; // [esp+310h] [ebp-2FCh]
    float v107; // [esp+320h] [ebp-2ECh]
    float v108; // [esp+334h] [ebp-2D8h]
    float v109; // [esp+338h] [ebp-2D4h]
    float v110; // [esp+33Ch] [ebp-2D0h]
    int v111; // [esp+340h] [ebp-2CCh]
    Vec3 v112; // [esp+344h] [ebp-2C8h] BYREF
    Vec3 v113; // [esp+350h] [ebp-2BCh] BYREF
    Vec3 v114; // [esp+35Ch] [ebp-2B0h] BYREF
    Vec3 v115; // [esp+368h] [ebp-2A4h] BYREF
    float v116; // [esp+374h] [ebp-298h]
    Vec3 v117; // [esp+378h] [ebp-294h] BYREF
    Vec3 v118; // [esp+384h] [ebp-288h] BYREF
    Vec3 v119; // [esp+390h] [ebp-27Ch] BYREF
    Vec3 v120; // [esp+39Ch] [ebp-270h] BYREF
    Vec3 v121; // [esp+3A8h] [ebp-264h]
    Vec3 a5; // [esp+3B4h] [ebp-258h] BYREF
    Vec3 v123; // [esp+3C0h] [ebp-24Ch] BYREF
    float v124; // [esp+3CCh] [ebp-240h]
    int v125; // [esp+3D0h] [ebp-23Ch]
    float v126; // [esp+3D4h] [ebp-238h]
    float v127; // [esp+3D8h] [ebp-234h]
    Vec3 v128; // [esp+3DCh] [ebp-230h] BYREF
    Vec3 v129; // [esp+3E8h] [ebp-224h] BYREF
    Vec3 v130; // [esp+3F4h] [ebp-218h] BYREF
    Vec3 v131; // [esp+400h] [ebp-20Ch] BYREF
    Vec3 v132; // [esp+40Ch] [ebp-200h] BYREF
    Vec3 v133; // [esp+418h] [ebp-1F4h] BYREF
    Vec3 v134; // [esp+424h] [ebp-1E8h] BYREF
    float v135; // [esp+430h] [ebp-1DCh]
    int v136; // [esp+434h] [ebp-1D8h]
    Vec3 v137; // [esp+43Ch] [ebp-1D0h] BYREF
    Vec3 v138; // [esp+448h] [ebp-1C4h] BYREF
    float v139; // [esp+454h] [ebp-1B8h]
    float v140; // [esp+458h] [ebp-1B4h]
    float v141; // [esp+45Ch] [ebp-1B0h]
    Vec3 v142; // [esp+460h] [ebp-1ACh] BYREF
    Vec3 v143; // [esp+46Ch] [ebp-1A0h] BYREF
    Vec3 v144; // [esp+478h] [ebp-194h] BYREF
    Vec3 v145; // [esp+484h] [ebp-188h] BYREF
    Vec3 v148; // [esp+498h] [ebp-174h] BYREF
    Vec3 v149; // [esp+4A4h] [ebp-168h] BYREF
    Vec3 v150; // [esp+4B0h] [ebp-15Ch] BYREF
    Vec3 v151; // [esp+4BCh] [ebp-150h] BYREF
    float v152; // [esp+4C8h] [ebp-144h]
    BOOL v153; // [esp+4CCh] [ebp-140h]
    int v154; // [esp+4D0h] [ebp-13Ch]
    float v155; // [esp+4D4h] [ebp-138h]
    float v156; // [esp+4D8h] [ebp-134h]
    Vec3 v157; // [esp+4DCh] [ebp-130h] BYREF
    float v158; // [esp+4E8h] [ebp-124h]
    float v159; // [esp+4ECh] [ebp-120h]
    float v160; // [esp+4F0h] [ebp-11Ch]
    Vec3 v161; // [esp+4F4h] [ebp-118h] BYREF
    Vec3 v162; // [esp+500h] [ebp-10Ch] BYREF
    Vec3 v163; // [esp+50Ch] [ebp-100h] BYREF
    Vec3 a2; // [esp+518h] [ebp-F4h] BYREF
    int v165; // [esp+524h] [ebp-E8h]
    float v166; // [esp+528h] [ebp-E4h]
    float v167; // [esp+52Ch] [ebp-E0h]
    Vec3 v168; // [esp+530h] [ebp-DCh] BYREF
    Vec3 v169; // [esp+53Ch] [ebp-D0h] BYREF
    Vec3 v170; // [esp+548h] [ebp-C4h] BYREF
    Vec3 v171; // [esp+554h] [ebp-B8h] BYREF
    Vec3 v172; // [esp+560h] [ebp-ACh] BYREF
    Vec3 v173; // [esp+56Ch] [ebp-A0h] BYREF
    Vec3 v174; // [esp+578h] [ebp-94h] BYREF
    float v175; // [esp+584h] [ebp-88h]
    int v176; // [esp+588h] [ebp-84h]
    tScriptStruct_PNJPompier* v177; // [esp+58Ch] [ebp-80h]
    Vec3 v178; // [esp+594h] [ebp-78h] BYREF
    Vec3 v179; // [esp+5A0h] [ebp-6Ch] BYREF
    Vec3 v180; // [esp+5ACh] [ebp-60h] BYREF
    float v181; // [esp+5B8h] [ebp-54h]
    float v182; // [esp+5BCh] [ebp-50h]
    Vec3 v183; // [esp+5CCh] [ebp-40h] BYREF
    Vec3 dest; // [esp+5D8h] [ebp-34h] BYREF
    Vec3 v185; // [esp+5E4h] [ebp-28h] BYREF
    int v186; // [esp+5F0h] [ebp-1Ch]
    float v187; // [esp+5F4h] [ebp-18h]
    float v188; // [esp+5F8h] [ebp-14h]
    float v189; // [esp+5FCh] [ebp-10h]
    float v190; // [esp+600h] [ebp-Ch]
    float v191; // [esp+608h] [ebp-4h]

    v139 = 1.0;
    v140 = 0.0;
    v141 = 0.0;
    v113.x = 1.0;
    v113.y = 0.0;
    v113.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v134.x = 1.0;
    v134.y = 0.0;
    v134.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v22 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v22 = 0;
    v177 = v22;
    v108 = 0.0;
    v189 = 0.0;
    v110 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Pompier_Pompier_mode_hyllis__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_get_aircraft = (int)AI_ul_RealResolveOneRefFunction(0x72004DE4u);
        fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004DD6u);
        fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x72005349u);
        fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_GFX_Smoke = (int)AI_ul_RealResolveOneRefFunction(0x72005347u);
    }
    if (v177->i_current_mode != 1)
    {
        v177->i_current_mode = 1;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 0);
        AI_EvalFunc_OBJGetPosInit_C(AI_gpst_CurrentGameObject, &v177->v_start_pos);
        v72 = &v177->v_last_pos;
        v73 = &v177->v_start_pos;
        v177->v_last_pos.x = v177->v_start_pos.x;
        v72->y = v73->y;
        v72->z = v73->z;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v177->v_start_pos);
        AI_EvalFunc_OBJ_SightInitGet_C(AI_gpst_CurrentGameObject, &v174);
        v148.x = 0.0;
        v148.y = 0.0;
        v148.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v174, &v148);
        v177->f_speed = 300.0;
        v177->i_flag_snap = 0;
        v177->o_next_wp = 0;
    }
    v177->i_flag_smoke = 0;
    v71 = (tGameObjectMain*)Pompier_Pompier_mode_hyllis__PJ_Aircraft_gao;
    if (Pompier_Pompier_mode_hyllis__PJ_Aircraft_gao)
    {
        if ((unsigned int)Pompier_Pompier_mode_hyllis__PJ_Aircraft_gao <= 2)
            v71 = AI_MainActor[Pompier_Pompier_mode_hyllis__PJ_Aircraft_gao - 1];
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v71);
    }
    else
    {
        v0 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v94 = (float)v0;
    if (v94 >= 90000.0)
    {
        if (v177->i_snd_motor_loop != -1)
        {
            SND_Release(v177->i_snd_motor_loop);
            v177->i_snd_motor_loop = -1;
        }
        if (v177->i_snd_sirene_loop != -1)
        {
            SND_Release(v177->i_snd_sirene_loop);
            v177->i_snd_sirene_loop = -1;
        }
        if (v177->i_snd_psss_loop != -1)
        {
            SND_Release(v177->i_snd_psss_loop);
            v177->i_snd_psss_loop = -1;
        }
    }
    else
    {
        if (v177->i_snd_motor_loop == -1)
        {
            v1 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
            v177->i_snd_motor_loop = v1;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v177->i_snd_motor_loop);
        }
        if (v177->i_snd_sirene_loop == -1)
        {
            v2 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 4, 0x18000000);
            v177->i_snd_sirene_loop = v2;
            AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v177->i_snd_sirene_loop);
        }
        if (v177->i_snd_psss_loop == -1)
        {
            if (v177->i_flag_snap)
            {
                v3 = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 5, 0x18000000);
                v177->i_snd_psss_loop = v3;
                AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v177->i_snd_psss_loop);
            }
        }
        else if (!v177->i_flag_snap)
        {
            SND_StopRq(v177->i_snd_psss_loop);
        }
    }
    v70 = (tGameObjectMain*)v177->o_aircraft;
    if (v70)
    {
        if ((unsigned int)v70 <= 2)
            v70 = AI_MainActor[(uint32_t)v70 - 1];
        v69 = v70;
    }
    else
    {
        v69 = 0;
    }
    if (!v69)
        goto LABEL_226;
    v68 = (tGameObjectMain*)v177->o_aircraft;
    if (v68)
    {
        if ((unsigned int)v68 <= 2)
            v68 = AI_MainActor[(uint32_t)v68 - 1];
        v4 = AI_EvalFunc_OBJ_FlagsControlGet_C(v68);
    }
    else
    {
        v4 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
    }
    v105 = v4;
    if ((v4 & 0x200000) != 0)
        goto LABEL_226;
    v67 = (tGameObjectMain*)v177->o_aircraft;
    if (v67)
    {
        if ((unsigned int)v67 <= 2)
            v67 = AI_MainActor[(uint32_t)v67 - 1];
        v5 = AI_EvalFunc_OBJ_CapaTest_C(v67, 0x1000);
    }
    else
    {
        v5 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x1000);
    }
    v153 = v5;
    if (v5)
    {
    LABEL_226:
        v177->i_flag_snap = 0;
        v177->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
        v111 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_get_aircraft);
        if (v111 != 1000)
            return v111;
    }
    if (v177->i_flag_snap)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v177->f_time_phase, 8.0))
        {
            v177->i_flag_snap = 0;
            v177->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
            v66 = (tGameObjectMain*)v177->o_aircraft;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v66 = AI_MainActor[(uint32_t)v66 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v66, 0, 0xC00);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0xC00);
            }
            v186 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_get_aircraft);
            if (v186 != 1000)
                return v186;
        }
        else if (AI_EvalFunc_TIMEElapsed_C(v177->f_time_phase, 2.0))
        {
            v65 = (tGameObjectMain*)v177->o_aircraft;
            if (v65)
            {
                if ((unsigned int)v65 <= 2)
                    v65 = AI_MainActor[(uint32_t)v65 - 1];
                AI_EvalFunc_OBJ_CapaSet_C(v65, 0, 0x800);
            }
            else
            {
                AI_EvalFunc_OBJ_CapaSet_C(0, 0, 0x800);
            }
        }
    }
    if (!v177->i_flag_snap)
    {
        v64 = (tGameObjectMain*)v177->o_next_wp;
        if (v64)
        {
            if ((unsigned int)v64 <= 2)
                v64 = AI_MainActor[(uint32_t)v64 - 1];
            v63 = v64;
        }
        else
        {
            v63 = 0;
        }
        if (v63)
        {
            if (v177->i_wp_nb != 1)
                goto LABEL_91;
            v62 = (tGameObjectMain*)v177->o_aircraft;
            if (v62)
            {
                if ((unsigned int)v62 <= 2)
                    v62 = AI_MainActor[(uint32_t)v62 - 1];
                v61 = v62;
            }
            else
            {
                v61 = 0;
            }
            if (!v61)
                goto LABEL_91;
            v60 = (tGameObjectMain*)v177->o_aircraft;
            if (v60)
            {
                if ((unsigned int)v60 <= 2)
                    v60 = AI_MainActor[(uint32_t)v60 - 1];
                v7 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v60);
            }
            else
            {
                v7 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v109 = (float)v7;
            v59 = (tGameObjectMain*)v177->o_next_wp;
            if (v59)
            {
                if ((unsigned int)v59 <= 2)
                    v59 = AI_MainActor[(uint32_t)v59 - 1];
                v8 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v59);
            }
            else
            {
                v8 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v127 = (float)v8;
            if (v109 < (double)v127)
            {
                v177->i_wp_nb = 0;
                v177->f_rot_blend_speed = 0.0;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v177->v_start_pos);
            }
            else
            {
            LABEL_91:
                v58 = (tGameObjectMain*)v177->o_next_wp;
                if (v58)
                {
                    if ((unsigned int)v58 <= 2)
                        v58 = AI_MainActor[(uint32_t)v58 - 1];
                    v9 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v58);
                }
                else
                {
                    v9 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
                }
                v125 = v9;
                if (v9)
                    goto LABEL_108;
                v57 = (tGameObjectMain*)v177->o_next_wp;
                if (v57)
                {
                    if ((unsigned int)v57 <= 2)
                        v57 = AI_MainActor[(uint32_t)v57 - 1];
                    AI_EvalFunc_OBJGetPos_C(v57, &v161);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v161);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v144);
                MATH_SubstractVec(&dest, &v161, &v144);
                v56 = (tGameObjectMain*)v177->o_next_wp;
                if (v56)
                {
                    if ((unsigned int)v56 <= 2)
                        v56 = AI_MainActor[(uint32_t)v56 - 1];
                    AI_EvalFunc_OBJGetPos_C(v56, &v149);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v149);
                }
                MATH_SubstractVec(&v112, &v149, &v177->v_start_pos);
                v116 = v112.z * dest.z + v112.y * dest.y + dest.x * v112.x;
                if (v116 < 0.0)
                {
                LABEL_108:
                    v55 = (tGameObjectMain*)v177->o_next_wp;
                    if (v55)
                    {
                        if ((unsigned int)v55 <= 2)
                            v55 = AI_MainActor[(uint32_t)v55 - 1];
                        v54 = v55;
                    }
                    else
                    {
                        v54 = 0;
                    }
                    v53 = (tGameObjectMain*)v177->o_start_pos_wp;
                    if (v53)
                    {
                        if ((unsigned int)v53 <= 2)
                            v53 = AI_MainActor[(uint32_t)v53 - 1];
                        v52 = v53;
                    }
                    else
                    {
                        v52 = 0;
                    }
                    if (v54 == v52)
                    {
                        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
                        v165 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_mode_wait);
                        if (v165 != 1000)
                            return v165;
                    }
                    v177->f_rot_blend_speed = 0.0;
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v177->v_start_pos);
                    --v177->i_wp_nb;
                }
            }
        }
        if (v177->i_wp_nb)
        {
            v177->o_next_wp = *(&v177->__ao_wp_Dimension1 + v177->i_wp_nb);
        }
        else
        {
            v51 = (tGameObjectMain*)v177->o_aircraft;
            if (v51)
            {
                if ((unsigned int)v51 <= 2)
                    v51 = AI_MainActor[(uint32_t)v51 - 1];
                v50 = v51;
            }
            else
            {
                v50 = 0;
            }
            if (v50)
                v177->o_next_wp = 0;
            else
                v177->o_next_wp = v177->o_start_pos_wp;
        }
        v49 = (tGameObjectMain*)v177->o_next_wp;
        if (v49)
        {
            if ((unsigned int)v49 <= 2)
                v49 = AI_MainActor[(uint32_t)v49 - 1];
            v48 = v49;
        }
        else
        {
            v48 = 0;
        }
        if (v48)
        {
            v47 = (tGameObjectMain*)v177->o_next_wp;
            if (v47)
            {
                if ((unsigned int)v47 <= 2)
                    v47 = AI_MainActor[(uint32_t)v47 - 1];
                AI_EvalFunc_OBJGetPos_C(v47, &v178);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v178);
            }
            a3.x = v178.x;
            a3.y = v178.y;
            a3.z = 40.0;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v128);
            a3.x = a3.x - v128.x;
            a3.y = a3.y - v128.y;
            a3.z = a3.z - v128.z;
            v10 = MATH_VecLen(&a3);
            v177->f_dist_to_dest = (float)v10;
        }
        else
        {
            v46 = (tGameObjectMain*)v177->o_aircraft;
            if (v46)
            {
                if ((unsigned int)v46 <= 2)
                    v46 = AI_MainActor[(uint32_t)v46 - 1];
                AI_EvalFunc_OBJGetPos_C(v46, &v133);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v133);
            }
            a3.x = v133.x;
            a3.y = v133.y;
            a3.z = v133.z + 20.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v142);
            a3.x = a3.x - v142.x;
            a3.y = a3.y - v142.y;
            a3.z = a3.z - v142.z;
            v11 = MATH_VecLen(&a3);
            v177->f_dist_to_dest = (float)v11;
            if (v177->f_dist_to_dest < 100.0)
            {
                v177->i_wp_nb = 0;
                v177->f_rot_blend_speed = 0.0;
                v177->o_next_wp = 0;
                v177->i_flag_snap = 1;
            }
        }
    }
    if (v177->i_flag_snap)
    {
        v177->f_banking_coef = 0.0;
        v152 = TIM_gf_dt;
        v37 = 0.5f * TIM_gf_dt;
        if (v37 < 0.0)
            v37 = 0.0;
        if (v37 > 1.0)
            v37 = 1.0;
        v135 = 3.0f * v37 + v177->f_rot_blend_speed * (1.0f - v37);
        v177->f_rot_blend_speed = v135;
        v36 = (tGameObjectMain*)v177->o_aircraft;
        if (v36)
        {
            if ((unsigned int)v36 <= 2)
                v36 = AI_MainActor[(uint32_t)v36 - 1];
            AI_EvalFunc_OBJGetPos_C(v36, &v169);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v169);
        }
        a3 = v169;
        v35 = (tGameObjectMain*)v177->o_aircraft;
        if (v35)
        {
            if ((unsigned int)v35 <= 2)
                v35 = AI_MainActor[(uint32_t)v35 - 1];
            AI_EvalFunc_DYNSpeedGetVector_C(v35, &v145);
        }
        else
        {
            AI_EvalFunc_DYNSpeedGetVector_C(0, &v145);
        }
        v89 = v145.x * 1.5f;
        v90 = v145.y * 1.5f;
        v91 = v145.z * 1.5f;
        a3.x = a3.x + v89;
        a3.y = a3.y + v90;
        a3.z = 30.0;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v77);
        a3.x = a3.x - v77.x;
        a3.y = a3.y - v77.y;
        a3.z = a3.z - v77.z;
        v12 = MATH_VecLen(&a3);
        v177->f_dist_to_dest = (float)v12;
        v34 = 1.0f / v177->f_dist_to_dest;
        a3.x = a3.x * v34;
        a3.y = a3.y * v34;
        a3.z = a3.z * v34;
        if (v177->f_dist_to_dest > 300.0)
            v19 = 300.0;
        else
            v19 = v177->f_dist_to_dest;
        v177->f_speed = v19;
        v88 = TIM_gf_dt;
        v33 = v177->f_speed * TIM_gf_dt;
        v151.x = a3.x * v33;
        v151.y = a3.y * v33;
        v151.z = a3.z * v33;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v151);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v162);
        v32 = (tGameObjectMain*)v177->o_aircraft;
        if (v32)
        {
            if ((unsigned int)v32 <= 2)
                v32 = AI_MainActor[(uint32_t)v32 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v32, &v84);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &v84);
        }
        v93 = v84.z * v162.z + v84.y * v162.y + v162.x * v84.x;
        if (v93 <= 0.0)
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v131);
            v30 = (tGameObjectMain*)v177->o_aircraft;
            if (v30)
            {
                if ((unsigned int)v30 <= 2)
                    v30 = AI_MainActor[(uint32_t)v30 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v30, &v117);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v117);
            }
            v191 = v117.z * v131.z + v117.y * v131.y + v131.x * v117.x;
            if (v191 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v114);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v95);
                v163.x = -v95.x;
                v163.y = -v95.y;
                v163.z = -v95.z;
                v126 = TIM_gf_dt;
                a4e = v177->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v114, &v163, a4e, &v97);
                v113 = v97;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v180);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v101);
                v181 = TIM_gf_dt;
                a4d = v177->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v180, &v101, a4d, &v103);
                v113 = v103;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v115);
            v31 = (tGameObjectMain*)v177->o_aircraft;
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v31, &v130);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v130);
            }
            v82 = TIM_gf_dt;
            a4c = v177->f_rot_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v115, &v130, a4c, &v75);
            v113 = v75;
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v185);
        v154 = 0;
        v155 = 0.0;
        v156 = v185.z - v177->v_last_pos.z;
        v29 = &v177->v_grav_pos;
        v177->v_grav_pos.x = v177->v_grav_pos.x + 0.0f;
        v29->y = v155 + v29->y;
        v29->z = v156 + v29->z;
        v134 = v177->v_grav_pos;
        v121.x = 0.0;
        v121.y = 0.0;
        v121.z = -40.0;
        v139 = 0.0;
        v140 = 0.0;
        v141 = -40.0;
        v98 = v177->v_grav_speed.x * 0.94999999f;
        v99 = v177->v_grav_speed.y * 0.94999999f;
        v100 = v177->v_grav_speed.z * 0.94999999f;
        v139 = v98 + 0.0f;
        v140 = v99 + 0.0f;
        v141 = v100 + -40.0f;
        v190 = TIM_gf_dt;
        v158 = v139 * TIM_gf_dt;
        v159 = v140 * TIM_gf_dt;
        v160 = v141 * TIM_gf_dt;
        v28 = &v177->v_grav_pos;
        v177->v_grav_pos.x = v158 + v177->v_grav_pos.x;
        v28->y = v159 + v28->y;
        v28->z = v160 + v28->z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v120);
        v27 = &v177->v_grav_pos;
        v177->v_grav_pos.x = v177->v_grav_pos.x - v120.x;
        v27->y = v27->y - v120.y;
        v27->z = v27->z - v120.z;
        v108 = MATH_VecLen(&v177->v_grav_pos);
        if (v108 != 0.0f)
        {
            v25 = &v177->v_grav_pos;
            v26 = 1.0f / v108;
            v177->v_grav_pos.x = v26 * v177->v_grav_pos.x;
            v25->y = v26 * v25->y;
            v25->z = v26 * v25->z;
            v76.x = -v177->v_grav_pos.x;
            v76.y = -v177->v_grav_pos.y;
            v76.z = -v177->v_grav_pos.z;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v113, &v76);
            v24 = &v177->v_grav_pos;
            v177->v_grav_pos.x = v177->v_grav_pos.x * 50.0f;
            v24->y = v24->y * 50.0f;
            v24->z = v24->z * 50.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v173);
            v23 = &v177->v_grav_pos;
            v177->v_grav_pos.x = v173.x + v177->v_grav_pos.x;
            v23->y = v173.y + v23->y;
            v23->z = v173.z + v23->z;
        }
        MATH_SubstractVec(&v129, &v177->v_grav_pos, &v134);
        v107 = TIM_gf_dt;
        MATH_VecMultNormScalar(&v74, &v129, TIM_gf_dt);
        v177->v_grav_speed = v74;
        v177->i_flag_smoke = 1;
    }
    else
    {
        v177->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
        v45 = v177->f_dist_to_dest / 60.0f;
        if (v45 > 1.0)
            v21 = 1.0;
        else
            v21 = v177->f_dist_to_dest / 60.0f;
        v187 = v21;
        v189 = 1.0f - v21;
        v167 = TIM_gf_dt;
        v44 = 1.5f * TIM_gf_dt;
        if (v189 < (double)v44)
            v20 = 1.5f * TIM_gf_dt;
        else
            v20 = v189;
        v87 = v20;
        v189 = v20;
        v43 = v20;
        if (v20 < 0.0)
            v43 = 0.0;
        if (v43 > 1.0)
            v43 = 1.0;
        v177->f_rot_blend_speed = 8.0f * v43 + v177->f_rot_blend_speed * (1.0f - v43);
        if (v177->f_dist_to_dest != 0.0f)
        {
            v42 = 1.0f / v177->f_dist_to_dest;
            a3.x = a3.x * v42;
            a3.y = a3.y * v42;
            a3.z = a3.z * v42;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v170);
            v179.x = 0.0;
            v179.y = 0.0;
            v179.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v170, &v179);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v137);
            v124 = a3.z * v137.z + a3.y * v137.y + v137.x * a3.x;
            if (v124 <= 0.0)
            {
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v183);
                v175 = a3.z * v183.z + a3.y * v183.y + v183.x * a3.x;
                if (v175 <= 0.0)
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v132);
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v102);
                    v119.x = -v102.x;
                    v119.y = -v102.y;
                    v119.z = -v102.z;
                    v81 = TIM_gf_dt;
                    a4b = v177->f_rot_blend_speed * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v132, &v119, a4b, &v171);
                    v113 = v171;
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v96);
                    AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v172);
                    v188 = TIM_gf_dt;
                    a4a = v177->f_rot_blend_speed * TIM_gf_dt;
                    AI_EvalFunc_MATHVecBlendRotate_C(&v96, &v172, a4a, &v104);
                    v113 = v104;
                }
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
                v106 = TIM_gf_dt;
                a4 = v177->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
                v113 = a5;
            }
            v78.x = 0.0;
            v78.y = 0.0;
            v78.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v113, &v78);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v143);
            v110 = a3.z * v143.z + a3.y * v143.y + v143.x * a3.x;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v138);
            v83 = a3.z * v138.z + a3.y * v138.y + v138.x * a3.x;
            if (v83 < 0.0)
                v110 = AI_EvalFunc_MTHFloatSign_C(v110);
            v79 = TIM_gf_dt;
            v41 = TIM_gf_dt;
            if (TIM_gf_dt < 0.0)
                v41 = 0.0;
            if (v41 > 1.0)
                v41 = 1.0;
            v182 = v110 * 1.570796f * v41 + v177->f_banking_coef * (1.0f - v41);
            v177->f_banking_coef = v182;
            AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, v177->f_banking_coef);
            v86 = TIM_gf_dt;
            v40 = 2.0f * TIM_gf_dt;
            if (v40 < 0.0)
                v40 = 0.0;
            if (v40 > 1.0)
                v40 = 1.0;
            v80 = 300.0f * v40 + v177->f_speed * (1.0f - v40);
            v177->f_speed = v80;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v118);
            v39 = v177->f_speed;
            v38 = &v177->v_grav_speed;
            v177->v_grav_speed.x = v118.x * v39;
            v38->y = v118.y * v39;
            v38->z = v118.z * v39;
            v166 = TIM_gf_dt;
            v123.x = TIM_gf_dt * v177->v_grav_speed.x;
            v123.y = v177->v_grav_speed.y * TIM_gf_dt;
            v123.z = v177->v_grav_speed.z * TIM_gf_dt;
            AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v123);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v150);
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v157);
            v168.x = v157.x * 50.0f;
            v168.y = v157.y * 50.0f;
            v168.z = v157.z * 50.0f;
            MATH_SubstractVec(&v85, &v150, &v168);
            v177->v_grav_pos = v85;
        }
    }
    v176 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_GFX_Halo);
    if (v176 != 1000)
        return v176;
    v136 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_hyllis_Pompier_Pompier_exec_GFX_Smoke);
    if (v136 != 1000)
        return v136;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v177->v_last_pos);
    return 1000;
}

int Pompier_Pompier_exec_GFX_Flash()
{
    float v1; // [esp+8h] [ebp-198h]
    float v2; // [esp+Ch] [ebp-194h]
    tScriptStruct_PNJPompier* v3; // [esp+10h] [ebp-190h]
    tGameObjectMain* v4; // [esp+20h] [ebp-180h]
    tGameObjectMain* v5; // [esp+38h] [ebp-168h]
    tGameObjectMain* v6; // [esp+48h] [ebp-158h]
    Vec3 v7; // [esp+4Ch] [ebp-154h] BYREF
    int v8; // [esp+58h] [ebp-148h]
    Vec3 v9; // [esp+5Ch] [ebp-144h] BYREF
    Vec3 a1; // [esp+68h] [ebp-138h] BYREF
    tGameObjectMain* v11; // [esp+74h] [ebp-12Ch]
    float v12; // [esp+78h] [ebp-128h]
    Vec3 v13; // [esp+7Ch] [ebp-124h] BYREF
    float v14; // [esp+88h] [ebp-118h]
    float v15; // [esp+8Ch] [ebp-114h]
    float v16; // [esp+90h] [ebp-110h]
    int v17; // [esp+94h] [ebp-10Ch]
    Vec3 v18; // [esp+98h] [ebp-108h] BYREF
    Vec3 v20; // [esp+ACh] [ebp-F4h] BYREF
    float v21; // [esp+B8h] [ebp-E8h]
    float v22; // [esp+BCh] [ebp-E4h]
    float v23; // [esp+C0h] [ebp-E0h]
    Vec3 v24; // [esp+C4h] [ebp-DCh] BYREF
    Vec3 v26; // [esp+D8h] [ebp-C8h] BYREF
    Vec3 v27; // [esp+E4h] [ebp-BCh]
    float v28; // [esp+F0h] [ebp-B0h]
    float v29; // [esp+F4h] [ebp-ACh]
    float v30; // [esp+F8h] [ebp-A8h]
    float v31; // [esp+FCh] [ebp-A4h]
    float v32; // [esp+100h] [ebp-A0h]
    float v33; // [esp+104h] [ebp-9Ch]
    Vec3 v34; // [esp+108h] [ebp-98h]
    Vec3 v35; // [esp+11Ch] [ebp-84h] BYREF
    Vec3 v36; // [esp+128h] [ebp-78h] BYREF
    float v37; // [esp+138h] [ebp-68h]
    float v38; // [esp+13Ch] [ebp-64h]
    float v39; // [esp+140h] [ebp-60h]
    float v40; // [esp+144h] [ebp-5Ch]
    float v41; // [esp+148h] [ebp-58h]
    Vec3 v42; // [esp+150h] [ebp-50h] BYREF
    tScriptStruct_PNJPompier* v43; // [esp+15Ch] [ebp-44h]
    float v44; // [esp+160h] [ebp-40h]
    float v45; // [esp+164h] [ebp-3Ch]
    float v46; // [esp+168h] [ebp-38h]
    float v47; // [esp+16Ch] [ebp-34h]
    float v48; // [esp+170h] [ebp-30h]
    float v49; // [esp+174h] [ebp-2Ch]
    Vec3 v50; // [esp+178h] [ebp-28h] BYREF
    Vec3 v51; // [esp+188h] [ebp-18h] BYREF
    Vec3 a3; // [esp+194h] [ebp-Ch] BYREF

    v24.x = 1.0;
    v24.y = 0.0;
    v24.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v3 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v3 = 0;
    v43 = v3;
    v11 = 0;
    v12 = 0.0;
    v37 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Pompier_Pompier_exec_GFX_Flash_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        if (v43->GFX_Flash == -1)
        {
            v43->GFX_Flash = AI_EvalFunc_GFXAdd_C_CURRENT(18);
            v6 = (tGameObjectMain*)Pompier_Pompier_exec_GFX_Flash_SFX_light_smoke_gao;
            if (Pompier_Pompier_exec_GFX_Flash_SFX_light_smoke_gao)
            {
                if ((unsigned int)Pompier_Pompier_exec_GFX_Flash_SFX_light_smoke_gao <= 2)
                    v6 = AI_MainActor[Pompier_Pompier_exec_GFX_Flash_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v43->GFX_Flash, v6, 11);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v43->GFX_Flash, 0, 11);
            }
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v43->GFX_Flash, 2, 1);
            GFXSetCurGlobal(v43->GFX_Flash);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18010, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18012, 10000.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18000, 0.1f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18001, 8.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18002, 16.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18003, 2.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18004, 4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18005, 0.75);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18006, 6.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18007, 12.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18008, 1.5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18009, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18011, 5.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18101, 16);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18103, 8);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18100, 1);
        }
        v11 = AI_EvalFunc_VIEWGetObject_C(0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        v24 = a1;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v7);
        v28 = v7.x * 2.3f;
        v29 = v7.y * 2.3f;
        v30 = v7.z * 2.3f;
        v24.x = v24.x + v28;
        v24.y = v24.y + v29;
        v24.z = v24.z + v30;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v50);
        v14 = v50.x * 2.5f;
        v15 = v50.y * 2.5f;
        v16 = v50.z * 2.5f;
        v24.x = v24.x + v14;
        v24.y = v24.y + v15;
        v24.z = v24.z + v16;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v36);
        v47 = v36.x * 2.5f;
        v48 = v36.y * 2.5f;
        v49 = v36.z * 2.5f;
        v24.x = v24.x + v47;
        v24.y = v24.y + v48;
        v24.z = v24.z + v49;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v18);
        v31 = v18.x * 5.0f;
        v32 = v18.y * 5.0f;
        v33 = v18.z * 5.0f;
        GFXSetCurGlobal(v43->GFX_Flash);
        AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v24);
        AI_EvalFunc_GFXSetf_C_CURGLOB(18013, 30.0);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v13);
        v5 = v11;
        if (v11)
        {
            if ((unsigned int)v11 <= 2)
                v5 = AI_MainActor[(uint32_t)v11 - 1];
            AI_EvalFunc_OBJ_SightGet_C(v5, &a3);
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C(0, &a3);
        }
        v34.x = -a3.x;
        v34.y = -a3.y;
        v34.z = -a3.z;
        v37 = -a3.z * v13.z
            + -a3.y * v13.y
            + v13.x * -a3.x;
        if (v37 < 0.0)
            v2 = 0.0;
        else
            v2 = v37;
        v37 = v2;
        v12 = v2;
        v8 = AI_EvalFunc_COLORBlend_C(0, 2113600, v2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(18102, v8);
    }
    else if (v43->GFX_Flash != -1)
    {
        if (AI_EvalFunc_TIMEElapsed_C(v43->f_time_phase, 0.5))
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v43->GFX_Flash);
            v43->GFX_Flash = -1;
        }
        else
        {
            v41 = TIM_gf_MainClock - TIM_gf_LockTime;
            v12 = (v41 - v43->f_time_phase) / 0.5f;
            v11 = AI_EvalFunc_VIEWGetObject_C(0);
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v51);
            v24 = v51;
            AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v35);
            v21 = v35.x * 3.9000001f;
            v22 = v35.y * 3.9000001f;
            v23 = v35.z * 3.9000001f;
            v24.x = v24.x - v21;
            v24.y = v24.y - v22;
            v24.z = v24.z - v23;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v20);
            v38 = v20.x * 8.0f;
            v39 = v20.y * 8.0f;
            v40 = v20.z * 8.0f;
            v24.x = v24.x + v38;
            v24.y = v24.y + v39;
            v24.z = v24.z + v40;
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v9);
            v44 = v9.x * 0.89999998f;
            v45 = v9.y * 0.89999998f;
            v46 = v9.z * 0.89999998f;
            v24.x = v24.x + v44;
            v24.y = v24.y + v45;
            v24.z = v24.z + v46;
            GFXSetCurGlobal(v43->GFX_Flash);
            AI_EvalFunc_GFXSetv_C_CURGLOB(18200, &v24);
            AI_EvalFunc_GFXSetf_C_CURGLOB(18013, 30.0);
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v26);
            v4 = v11;
            if (v11)
            {
                if ((unsigned int)v11 <= 2)
                    v4 = AI_MainActor[(uint32_t)v11 - 1];
                AI_EvalFunc_OBJ_SightGet_C(v4, &v42);
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C(0, &v42);
            }
            v27.x = -v42.x;
            v27.y = -v42.y;
            v27.z = -v42.z;
            v37 = -v42.z * v26.z
                + -v42.y * v26.y
                + v26.x * -v42.x;
            if (v37 < 0.0)
                v1 = 0.0;
            else
                v1 = v37;
            v37 = v1;
            v12 = (1.0f - v1) * v12;
            v17 = AI_EvalFunc_COLORBlend_C(0x404040, 0, v12);
            AI_EvalFunc_GFXSeti_C_CURGLOB(18102, v17);
        }
    }
    return 1000;
}

int Pompier_Pompier_init()
{
    tGameObjectMain* v0; // eax
    tScriptStruct_PNJPompier* v2; // [esp+0h] [ebp-3Ch]
    Vec3 a1; // [esp+8h] [ebp-34h] BYREF
    int v4; // [esp+1Ch] [ebp-20h]
    int v5; // [esp+20h] [ebp-1Ch]
    tScriptStruct_PNJPompier* v6; // [esp+2Ch] [ebp-10h]
    Vec3 pos; // [esp+30h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v2 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v2 = 0;
    v6 = v2;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Pompier_Pompier_init_Pompier_Pompier_mode_canaux = (int)AI_ul_RealResolveOneRefFunction(0x7200533Au);
        Pompier_Pompier_init_DUP_WP_simple_de_base_gao = AI_ul_RealResolveOneRefObject(0x7100BEC4u);
        fct_Pompier_Pompier_init_Pompier_Pompier_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72004DD6u);
        fct_Pompier_Pompier_init_Pompier_Police_mode_canaux = (int)AI_ul_RealResolveOneRefFunction(0x720053D4u);
        fct_Pompier_Pompier_init_Pompier_Police_mode_wait = (int)AI_ul_RealResolveOneRefFunction(0x72005407u);
    }
    if (!v6->des_mode)
    {
        v6->i_snd_motor_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 0, 0x18000000);
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v6->i_snd_motor_loop);
        v6->i_snd_sirene_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 4, 0x18000000);
    }
    if (v6->des_type)
    {
        if (v6->des_mode)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&pos);
            v6->o_start_pos_wp = (int)AI_EvalFunc_OBJDuplicate_C(
                (tGameObjectMain*)Pompier_Pompier_init_DUP_WP_simple_de_base_gao,
                &pos);
            AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
            v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pompier_Pompier_init_Pompier_Police_mode_wait);
            if (v5 != 1000)
                return v5;
        }
        else
        {
            AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Pompier_Pompier_init_Pompier_Police_mode_canaux);
        }
    }
    else if (v6->des_mode)
    {
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        v0 = AI_EvalFunc_OBJDuplicate_C((tGameObjectMain*)Pompier_Pompier_init_DUP_WP_simple_de_base_gao, &a1);
        v6->o_start_pos_wp = (int)v0;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        v4 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pompier_Pompier_init_Pompier_Pompier_mode_wait);
        if (v4 != 1000)
            return v4;
    }
    else
    {
        v6->i_snd_psss_loop = AI_EvalFunc_SNDRequest_C(AI_gpst_CurrentGameObject, 5, 0x18000000);
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v6->i_snd_sirene_loop);
        AI_EvalFunc_AITrackCurChange_C((tAIFunction*)fct_Pompier_Pompier_init_Pompier_Pompier_mode_canaux);
    }
    return 1000;
}

int Pompier_Pompier_mode_canaux()
{
    tGameObjectMain* v0; // eax
    BOOL v1; // eax
    BOOL v2; // eax
    BOOL v3; // eax
    tGameObjectMain* v4; // eax
    BOOL v5; // eax
    tGameObjectMain* v6; // eax
    double v7; // st7
    tGameObjectMain* v8; // eax
    float a4; // [esp+0h] [ebp-4E0h]
    float a4a; // [esp+0h] [ebp-4E0h]
    float a4b; // [esp+0h] [ebp-4E0h]
    float v13; // [esp+8h] [ebp-4D8h]
    float v14; // [esp+Ch] [ebp-4D4h]
    float v15; // [esp+10h] [ebp-4D0h]
    tScriptStruct_PNJPompier* v16; // [esp+14h] [ebp-4CCh]
    Vec3* v17; // [esp+24h] [ebp-4BCh]
    Vec3* v18; // [esp+28h] [ebp-4B8h]
    Vec3* v19; // [esp+34h] [ebp-4ACh]
    float v20; // [esp+38h] [ebp-4A8h]
    Vec3* v21; // [esp+40h] [ebp-4A0h]
    Vec3* v22; // [esp+44h] [ebp-49Ch]
    Vec3* v23; // [esp+5Ch] [ebp-484h]
    float v24; // [esp+60h] [ebp-480h]
    float v25; // [esp+68h] [ebp-478h]
    tGameObjectMain* v26; // [esp+78h] [ebp-468h]
    tGameObjectMain* v27; // [esp+7Ch] [ebp-464h]
    tGameObjectMain* v28; // [esp+80h] [ebp-460h]
    Vec3* v29; // [esp+90h] [ebp-450h]
    float v30; // [esp+94h] [ebp-44Ch]
    float v31; // [esp+98h] [ebp-448h]
    float v32; // [esp+9Ch] [ebp-444h]
    float v33; // [esp+C4h] [ebp-41Ch]
    float v34; // [esp+C8h] [ebp-418h]
    float v35; // [esp+CCh] [ebp-414h]
    float v36; // [esp+D0h] [ebp-410h]
    tGameObjectMain* v37; // [esp+E4h] [ebp-3FCh]
    tGameObjectMain* v38; // [esp+ECh] [ebp-3F4h]
    tGameObjectMain* v39; // [esp+F4h] [ebp-3ECh]
    tGameObjectMain* v40; // [esp+F8h] [ebp-3E8h]
    tGameObjectMain* v41; // [esp+FCh] [ebp-3E4h]
    tGameObjectMain* v42; // [esp+104h] [ebp-3DCh]
    tGameObjectMain* v43; // [esp+10Ch] [ebp-3D4h]
    tGameObjectMain* v44; // [esp+114h] [ebp-3CCh]
    tGameObjectMain* v45; // [esp+118h] [ebp-3C8h]
    tGameObjectMain* v46; // [esp+11Ch] [ebp-3C4h]
    tGameObjectMain* v47; // [esp+124h] [ebp-3BCh]
    tGameObjectMain* v48; // [esp+12Ch] [ebp-3B4h]
    tGameObjectMain* v49; // [esp+134h] [ebp-3ACh]
    tGameObjectMain* v50; // [esp+138h] [ebp-3A8h]
    tGameObjectMain* v51; // [esp+13Ch] [ebp-3A4h]
    tGameObjectMain* v52; // [esp+140h] [ebp-3A0h]
    tGameObjectMain* v53; // [esp+148h] [ebp-398h]
    tGameObjectMain* v54; // [esp+150h] [ebp-390h]
    tGameObjectMain* v55; // [esp+158h] [ebp-388h]
    tGameObjectMain* v56; // [esp+160h] [ebp-380h]
    tGameObjectMain* v57; // [esp+164h] [ebp-37Ch]
    tGameObjectMain* v58; // [esp+168h] [ebp-378h]
    Vec3* v59; // [esp+16Ch] [ebp-374h]
    Vec3* v60; // [esp+170h] [ebp-370h]
    tGameObjectMain* v61; // [esp+178h] [ebp-368h]
    Vec3 v62; // [esp+17Ch] [ebp-364h] BYREF
    Vec3 v63; // [esp+188h] [ebp-358h] BYREF
    float v64; // [esp+194h] [ebp-34Ch]
    float v65; // [esp+198h] [ebp-348h]
    float v66; // [esp+19Ch] [ebp-344h]
    Vec3 v67; // [esp+1A0h] [ebp-340h] BYREF
    int v68; // [esp+1BCh] [ebp-324h]
    float v69; // [esp+1C0h] [ebp-320h]
    float v70; // [esp+1C4h] [ebp-31Ch]
    int v71; // [esp+1C8h] [ebp-318h]
    Vec3 v72; // [esp+1CCh] [ebp-314h] BYREF
    Vec3 v73; // [esp+1D8h] [ebp-308h] BYREF
    Vec3 v74; // [esp+1E4h] [ebp-2FCh] BYREF
    float v75; // [esp+1F0h] [ebp-2F0h]
    float v76; // [esp+1F4h] [ebp-2ECh]
    Vec3 v77; // [esp+200h] [ebp-2E0h] BYREF
    float v78; // [esp+20Ch] [ebp-2D4h]
    float v79; // [esp+210h] [ebp-2D0h]
    float v80; // [esp+214h] [ebp-2CCh]
    Vec3 v81; // [esp+218h] [ebp-2C8h] BYREF
    Vec3 a3; // [esp+224h] [ebp-2BCh] BYREF
    Vec3 v83; // [esp+234h] [ebp-2ACh] BYREF
    Vec3 v84; // [esp+240h] [ebp-2A0h] BYREF
    float v85; // [esp+24Ch] [ebp-294h]
    Vec3 v86; // [esp+250h] [ebp-290h] BYREF
    Vec3 v87; // [esp+25Ch] [ebp-284h] BYREF
    Vec3 v88; // [esp+268h] [ebp-278h] BYREF
    Vec3 v89; // [esp+274h] [ebp-26Ch] BYREF
    Vec3 a2; // [esp+280h] [ebp-260h] BYREF
    Vec3 v91; // [esp+28Ch] [ebp-254h] BYREF
    BOOL v92; // [esp+298h] [ebp-248h]
    Vec3 v93; // [esp+29Ch] [ebp-244h] BYREF
    float v94; // [esp+2A8h] [ebp-238h]
    float v95; // [esp+2B0h] [ebp-230h]
    float v96; // [esp+2B4h] [ebp-22Ch]
    Vec3 v97; // [esp+2B8h] [ebp-228h] BYREF
    Vec3 v98; // [esp+2C4h] [ebp-21Ch] BYREF
    Vec3 v99; // [esp+2D0h] [ebp-210h] BYREF
    float v100; // [esp+2DCh] [ebp-204h]
    Vec3 v101; // [esp+2E0h] [ebp-200h] BYREF
    float v103; // [esp+2FCh] [ebp-1E4h]
    float v104; // [esp+304h] [ebp-1DCh]
    BOOL v105; // [esp+308h] [ebp-1D8h]
    Vec3 a1; // [esp+30Ch] [ebp-1D4h] BYREF
    Vec3 v107; // [esp+324h] [ebp-1BCh] BYREF
    Vec3 v108; // [esp+330h] [ebp-1B0h] BYREF
    Vec3 v109; // [esp+33Ch] [ebp-1A4h] BYREF
    Vec3 dest; // [esp+348h] [ebp-198h] BYREF
    float v111; // [esp+354h] [ebp-18Ch]
    float v112; // [esp+358h] [ebp-188h]
    Vec3 v113; // [esp+35Ch] [ebp-184h] BYREF
    float v114; // [esp+368h] [ebp-178h]
    float v115; // [esp+36Ch] [ebp-174h]
    float v116; // [esp+370h] [ebp-170h]
    Vec3 v117; // [esp+374h] [ebp-16Ch] BYREF
    Vec3 v118; // [esp+380h] [ebp-160h]
    Vec3 v119; // [esp+38Ch] [ebp-154h] BYREF
    float v120; // [esp+3A0h] [ebp-140h]
    float v121; // [esp+3A4h] [ebp-13Ch]
    Vec3 v122; // [esp+3A8h] [ebp-138h] BYREF
    Vec3 v123; // [esp+3B4h] [ebp-12Ch] BYREF
    Vec3 v124; // [esp+3CCh] [ebp-114h] BYREF
    BOOL v125; // [esp+3E4h] [ebp-FCh]
    Vec3 v126; // [esp+3E8h] [ebp-F8h] BYREF
    Vec3 v127; // [esp+3F4h] [ebp-ECh] BYREF
    Vec3 v128; // [esp+40Ch] [ebp-D4h] BYREF
    Vec3 v129; // [esp+418h] [ebp-C8h] BYREF
    Vec3 v130; // [esp+424h] [ebp-BCh] BYREF
    Vec3 v131; // [esp+434h] [ebp-ACh] BYREF
    int v132; // [esp+440h] [ebp-A0h]
    float v133; // [esp+444h] [ebp-9Ch]
    float v134; // [esp+448h] [ebp-98h]
    Vec3 v135; // [esp+44Ch] [ebp-94h] BYREF
    float v136; // [esp+458h] [ebp-88h]
    float v137; // [esp+460h] [ebp-80h]
    tScriptStruct_PNJPompier* v138; // [esp+468h] [ebp-78h]
    float v139; // [esp+46Ch] [ebp-74h]
    Vec3 a5; // [esp+470h] [ebp-70h] BYREF
    Vec3 v141; // [esp+47Ch] [ebp-64h] BYREF
    float v142; // [esp+488h] [ebp-58h]
    tGameObjectMain* v143; // [esp+494h] [ebp-4Ch]
    Vec3 v144; // [esp+49Ch] [ebp-44h] BYREF
    Vec3 v145; // [esp+4A8h] [ebp-38h] BYREF
    Vec3 v146; // [esp+4B4h] [ebp-2Ch] BYREF
    Vec3 v147; // [esp+4C0h] [ebp-20h] BYREF
    Vec3 v148; // [esp+4CCh] [ebp-14h] BYREF
    float v149; // [esp+4D8h] [ebp-8h]
    float v150; // [esp+4DCh] [ebp-4h]

    v114 = 1.0;
    v115 = 0.0;
    v116 = 0.0;
    v98.x = 1.0;
    v98.y = 0.0;
    v98.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    v109.x = 1.0;
    v109.y = 0.0;
    v109.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v16 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v16 = 0;
    v138 = v16;
    v94 = 0.0;
    v143 = 0;
    v150 = 0.0;
    v96 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Pompier_Pompier_mode_canaux__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Pompier_Pompier_mode_canaux_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Pompier_Pompier_mode_canaux_univVars_Flag_speciaux_sauve,
            (void*)Pompier_Pompier_mode_canaux_univVars,
            4488);
        fct_Pompier_Pompier_mode_canaux_Pompier_Pompier_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x72005349u);
        fct_Pompier_Pompier_mode_canaux_Pompier_Pompier_exec_GFX_Smoke = (int)AI_ul_RealResolveOneRefFunction(0x72005347u);
    }
    v143 = AI_EvalFunc_VIEWGetObject_C(0);
    if (v138->i_current_mode != 2)
    {
        v138->i_current_mode = 2;
        v138->o_next_wp = (int)AI_EvalFunc_WAYNearestWPOfObj_C(
            (tGameObjectMain*)Pompier_Pompier_mode_canaux__PJ_Aircraft_gao,
            (tWayNetwork*)v138->n_net,
            -1,
            0,
            0);
        v61 = (tGameObjectMain*)v138->o_next_wp;
        if (v61)
        {
            if ((unsigned int)v61 <= 2)
                v61 = AI_MainActor[(uint32_t)v61 - 1];
            AI_EvalFunc_OBJGetPos_C(v61, &v138->v_start_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v138->v_start_pos);
        }
        v59 = &v138->v_last_pos;
        v60 = &v138->v_start_pos;
        v138->v_last_pos.x = v138->v_start_pos.x;
        v59->y = v60->y;
        v59->z = v60->z;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v138->v_start_pos);
        v58 = (tGameObjectMain*)v138->o_next_wp;
        if (v58)
        {
            if ((unsigned int)v58 <= 2)
                v58 = AI_MainActor[(uint32_t)v58 - 1];
            v57 = v58;
        }
        else
        {
            v57 = 0;
        }
        v56 = v143;
        if (v143)
        {
            if ((unsigned int)v143 <= 2)
                v56 = AI_MainActor[(uint32_t)v143 - 1];
            v0 = AI_EvalFunc_WAYNetNextWP_C(v56, (tWayNetwork*)v138->n_net, v57, 3, 0);
        }
        else
        {
            v0 = AI_EvalFunc_WAYNetNextWP_C(0, (tWayNetwork*)v138->n_net, v57, 3, 0);
        }
        v138->o_next_wp = (int)v0;
        v55 = (tGameObjectMain*)v138->o_next_wp;
        if (v55)
        {
            if ((unsigned int)v55 <= 2)
                v55 = AI_MainActor[(uint32_t)v55 - 1];
            AI_EvalFunc_OBJGetPos_C(v55, &v74);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v74);
        }
        MATH_SubstractVec(&dest, &v74, &v138->v_start_pos);
        v141.x = 0.0;
        v141.y = 0.0;
        v141.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&dest, &v141);
    }
    v54 = (tGameObjectMain*)v138->o_next_wp;
    if (v54)
    {
        if ((unsigned int)v54 <= 2)
            v54 = AI_MainActor[(uint32_t)v54 - 1];
        v1 = AI_EvalFunc_OBJ_CapaTest_C(v54, 1);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_CapaTest_C(0, 1);
    }
    v92 = v1;
    if (v1)
    {
        v53 = (tGameObjectMain*)v138->o_next_wp;
        if (v53)
        {
            if ((unsigned int)v53 <= 2)
                v53 = AI_MainActor[(uint32_t)v53 - 1];
            v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(v53, 0x40);
        }
        else
        {
            v2 = AI_EvalFunc_OBJ_FlagsIdentityTest_C(0, 0x40);
        }
        v125 = v2;
        if (v2)
        {
            v52 = (tGameObjectMain*)v138->o_next_wp;
            if (v52)
            {
                if ((unsigned int)v52 <= 2)
                    v52 = AI_MainActor[(uint32_t)v52 - 1];
                v51 = v52;
            }
            else
            {
                v51 = 0;
            }
            if (v51->extended->fields->designInt1 <= 0)
            {
                v138->i_flag_wp_a_arroser = 0;
            }
            else
            {
                v50 = (tGameObjectMain*)v138->o_next_wp;
                if (v50)
                {
                    if ((unsigned int)v50 <= 2)
                        v50 = AI_MainActor[(uint32_t)v50 - 1];
                    v49 = v50;
                }
                else
                {
                    v49 = 0;
                }
                v3 = AI_EvalFunc_ARRBitTest_C(
                    (int*)(Pompier_Pompier_mode_canaux_univVars_Flag_speciaux_sauve + 4),
                    v49->extended->fields->designInt1);
                v138->i_flag_wp_a_arroser = v3;
            }
        }
        else
        {
            v138->i_flag_wp_a_arroser = 1;
        }
    }
    else
    {
        v138->i_flag_wp_a_arroser = 0;
    }
    if (!v138->i_flag_wp_a_arroser)
    {
        v48 = (tGameObjectMain*)v138->o_next_wp;
        if (v48)
        {
            if ((unsigned int)v48 <= 2)
                v48 = AI_MainActor[(uint32_t)v48 - 1];
            AI_EvalFunc_OBJGetPos_C(v48, &v93);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v93);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v119);
        MATH_SubstractVec(&v84, &v93, &v119);
        v47 = (tGameObjectMain*)v138->o_next_wp;
        if (v47)
        {
            if ((unsigned int)v47 <= 2)
                v47 = AI_MainActor[(uint32_t)v47 - 1];
            AI_EvalFunc_OBJGetPos_C(v47, &v148);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v148);
        }
        MATH_SubstractVec(&v117, &v148, &v138->v_start_pos);
        v95 = v117.z * v84.z + v117.y * v84.y + v84.x * v117.x;
        if (v95 < 0.0)
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v138->v_start_pos);
            v46 = (tGameObjectMain*)v138->o_next_wp;
            if (v46)
            {
                if ((unsigned int)v46 <= 2)
                    v46 = AI_MainActor[(uint32_t)v46 - 1];
                v45 = v46;
            }
            else
            {
                v45 = 0;
            }
            v44 = v143;
            if (v143)
            {
                if ((unsigned int)v143 <= 2)
                    v44 = AI_MainActor[(uint32_t)v143 - 1];
                v4 = AI_EvalFunc_WAYNetNextWP_C(v44, (tWayNetwork*)v138->n_net, v45, 3, 0);
            }
            else
            {
                v4 = AI_EvalFunc_WAYNetNextWP_C(0, (tWayNetwork*)v138->n_net, v45, 3, 0);
            }
            v138->o_next_wp = (int)v4;
            v138->f_rot_blend_speed = 0.0;
            v43 = (tGameObjectMain*)v138->o_next_wp;
            if (v43)
            {
                if ((unsigned int)v43 <= 2)
                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                v5 = AI_EvalFunc_OBJ_CapaTest_C(v43, 2);
            }
            else
            {
                v5 = AI_EvalFunc_OBJ_CapaTest_C(0, 2);
            }
            v105 = v5;
            if (v5)
            {
                v42 = (tGameObjectMain*)v138->o_next_wp;
                if (v42)
                {
                    if ((unsigned int)v42 <= 2)
                        v42 = AI_MainActor[(uint32_t)v42 - 1];
                    AI_EvalFunc_OBJGetPos_C(v42, &v138->v_start_pos);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v138->v_start_pos);
                }
                AI_EvalFunc_OBJSetPos_C_CURRENT(&v138->v_start_pos);
                v41 = (tGameObjectMain*)v138->o_next_wp;
                if (v41)
                {
                    if ((unsigned int)v41 <= 2)
                        v41 = AI_MainActor[(uint32_t)v41 - 1];
                    v40 = v41;
                }
                else
                {
                    v40 = 0;
                }
                v39 = v143;
                if (v143)
                {
                    if ((unsigned int)v143 <= 2)
                        v39 = AI_MainActor[(uint32_t)v143 - 1];
                    v6 = AI_EvalFunc_WAYNetNextWP_C(v39, (tWayNetwork*)v138->n_net, v40, 3, 0);
                }
                else
                {
                    v6 = AI_EvalFunc_WAYNetNextWP_C(0, (tWayNetwork*)v138->n_net, v40, 3, 0);
                }
                v138->o_next_wp = (int)v6;
                v38 = (tGameObjectMain*)v138->o_next_wp;
                if (v38)
                {
                    if ((unsigned int)v38 <= 2)
                        v38 = AI_MainActor[(uint32_t)v38 - 1];
                    AI_EvalFunc_OBJGetPos_C(v38, &v144);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v144);
                }
                MATH_SubstractVec(&v123, &v144, &v138->v_start_pos);
                v99.x = 0.0;
                v99.y = 0.0;
                v99.z = 1.0;
                AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v123, &v99);
                v138->f_rot_blend_speed = 6.0;
                v138->f_speed = 0.0;
                v138->f_banking_coef = 0.0;
            }
        }
    }
    v37 = (tGameObjectMain*)v138->o_next_wp;
    if (v37)
    {
        if ((unsigned int)v37 <= 2)
            v37 = AI_MainActor[(uint32_t)v37 - 1];
        AI_EvalFunc_OBJGetPos_C(v37, &v97);
    }
    else
    {
        AI_EvalFunc_OBJGetPos_C(0, &v97);
    }
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v129);
    MATH_SubstractVec(&v81, &v97, &v129);
    a3 = v81;
    v7 = MATH_VecLen(&a3);
    v138->f_dist_to_dest = (float)v7;
    v36 = v138->f_dist_to_dest / 60.0f;
    if (v36 > 1.0)
        v15 = 1.0;
    else
        v15 = v138->f_dist_to_dest / 60.0f;
    v112 = v15;
    v150 = 1.0f - v15;
    v139 = TIM_gf_dt;
    v35 = 1.5f * TIM_gf_dt;
    if (v150 < (double)v35)
        v14 = 1.5f * TIM_gf_dt;
    else
        v14 = v150;
    v111 = v14;
    v150 = v14;
    v34 = v14;
    if (v14 < 0.0)
        v34 = 0.0;
    if (v34 > 1.0)
        v34 = 1.0;
    v138->f_rot_blend_speed = 6.0f * v34 + v138->f_rot_blend_speed * (1.0f - v34);
    if (v138->f_dist_to_dest == 0.0f || v138->f_dist_to_dest <= 40.0 && v138->i_flag_wp_a_arroser)
    {
        v138->i_flag_smoke = 1;
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v138->i_snd_psss_loop);
        if (AI_EvalFunc_TIMEElapsed_C(v138->f_time_phase, 10.0))
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v138->v_start_pos);
            v28 = (tGameObjectMain*)v138->o_next_wp;
            if (v28)
            {
                if ((unsigned int)v28 <= 2)
                    v28 = AI_MainActor[(uint32_t)v28 - 1];
                v27 = v28;
            }
            else
            {
                v27 = 0;
            }
            v26 = v143;
            if (v143)
            {
                if ((unsigned int)v143 <= 2)
                    v26 = AI_MainActor[(uint32_t)v143 - 1];
                v8 = AI_EvalFunc_WAYNetNextWP_C(v26, (tWayNetwork*)v138->n_net, v27, 3, 0);
            }
            else
            {
                v8 = AI_EvalFunc_WAYNetNextWP_C(0, (tWayNetwork*)v138->n_net, v27, 3, 0);
            }
            v138->o_next_wp = (int)v8;
            v138->f_rot_blend_speed = 0.0;
        }
        if (v138->f_speed > (double)v138->f_dist_to_dest)
            v13 = v138->f_dist_to_dest;
        else
            v13 = v138->f_speed;
        v138->f_speed = v13;
        v138->f_banking_coef = 0.0;
        v25 = 1.0f / v138->f_dist_to_dest;
        a3.x = a3.x * v25;
        a3.y = a3.y * v25;
        a3.z = a3.z * v25;
        v75 = TIM_gf_dt;
        v24 = v138->f_speed * TIM_gf_dt;
        v122.x = a3.x * v24;
        v122.y = a3.y * v24;
        v122.z = a3.z * v24;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v122);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v108);
        v132 = 0;
        v133 = 0.0;
        v134 = v108.z - v138->v_last_pos.z;
        v23 = &v138->v_grav_pos;
        v138->v_grav_pos.x = v138->v_grav_pos.x + 0.0f;
        v23->y = v133 + v23->y;
        v23->z = v134 + v23->z;
        v109 = v138->v_grav_pos;
        v118.x = 0.0;
        v118.y = 0.0;
        v118.z = -15.0;
        v114 = 0.0;
        v115 = 0.0;
        v116 = -15.0;
        v78 = v138->v_grav_speed.x * 0.98000002f;
        v79 = v138->v_grav_speed.y * 0.98000002f;
        v80 = v138->v_grav_speed.z * 0.98000002f;
        v114 = v78 + 0.0f;
        v115 = v79 + 0.0f;
        v116 = v80 + -15.0f;
        v69 = TIM_gf_dt;
        v64 = v114 * TIM_gf_dt;
        v65 = v115 * TIM_gf_dt;
        v66 = v116 * TIM_gf_dt;
        v22 = &v138->v_grav_pos;
        v138->v_grav_pos.x = v64 + v138->v_grav_pos.x;
        v22->y = v65 + v22->y;
        v22->z = v66 + v22->z;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v146);
        v21 = &v138->v_grav_pos;
        v138->v_grav_pos.x = v138->v_grav_pos.x - v146.x;
        v21->y = v21->y - v146.y;
        v21->z = v21->z - v146.z;
        v94 = MATH_VecLen(&v138->v_grav_pos);
        if (v94 != 0.0f)
        {
            v19 = &v138->v_grav_pos;
            v20 = 1.0f / v94;
            v138->v_grav_pos.x = v20 * v138->v_grav_pos.x;
            v19->y = v20 * v19->y;
            v19->z = v20 * v19->z;
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v124);
            v127.x = -v138->v_grav_pos.x;
            v127.y = -v138->v_grav_pos.y;
            v127.z = -v138->v_grav_pos.z;
            AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&v124, &v127);
            v18 = &v138->v_grav_pos;
            v138->v_grav_pos.x = v138->v_grav_pos.x * 20.0f;
            v18->y = v18->y * 20.0f;
            v18->z = v18->z * 20.0f;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v72);
            v17 = &v138->v_grav_pos;
            v138->v_grav_pos.x = v72.x + v138->v_grav_pos.x;
            v17->y = v72.y + v17->y;
            v17->z = v72.z + v17->z;
        }
        MATH_SubstractVec(&v77, &v138->v_grav_pos, &v109);
        v100 = TIM_gf_dt;
        MATH_VecMultNormScalar(&a1, &v77, TIM_gf_dt);
        v138->v_grav_speed = a1;
    }
    else
    {
        v138->i_flag_smoke = 0;
        SND_StopRq(v138->i_snd_psss_loop);
        v138->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
        v33 = 1.0f / v138->f_dist_to_dest;
        a3.x = a3.x * v33;
        a3.y = a3.y * v33;
        a3.z = a3.z * v33;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v87);
        v147.x = 0.0;
        v147.y = 0.0;
        v147.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v87, &v147);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v126);
        v76 = a3.z * v126.z + a3.y * v126.y + v126.x * a3.x;
        if (v76 <= 0.0)
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v113);
            v103 = a3.z * v113.z + a3.y * v113.y + v113.x * a3.x;
            if (v103 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v135);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v83);
                v131.x = -v83.x;
                v131.y = -v83.y;
                v131.z = -v83.z;
                v149 = TIM_gf_dt;
                a4b = v138->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v135, &v131, a4b, &v88);
                v98 = v88;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v130);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v89);
                v104 = TIM_gf_dt;
                a4a = v138->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v130, &v89, a4a, &v145);
                v98 = v145;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&a2);
            v137 = TIM_gf_dt;
            a4 = v138->f_rot_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
            v98 = a5;
        }
        v107.x = 0.0;
        v107.y = 0.0;
        v107.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v98, &v107);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v86);
        v96 = a3.z * v86.z + a3.y * v86.y + v86.x * a3.x;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v62);
        v136 = a3.z * v62.z + a3.y * v62.y + v62.x * a3.x;
        if (v136 < 0.0)
            v96 = AI_EvalFunc_MTHFloatSign_C(v96);
        v120 = TIM_gf_dt;
        v32 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v32 = 0.0;
        if (v32 > 1.0)
            v32 = 1.0;
        v85 = v96 * 1.570796f * v32 + v138->f_banking_coef * (1.0f - v32);
        v138->f_banking_coef = v85;
        AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, v138->f_banking_coef);
        v121 = TIM_gf_dt;
        v31 = 2.0f * TIM_gf_dt;
        if (v31 < 0.0)
            v31 = 0.0;
        if (v31 > 1.0)
            v31 = 1.0;
        v70 = 100.0f * v31 + v138->f_speed * (1.0f - v31);
        v138->f_speed = v70;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v91);
        v30 = v138->f_speed;
        v29 = &v138->v_grav_speed;
        v138->v_grav_speed.x = v91.x * v30;
        v29->y = v91.y * v30;
        v29->z = v91.z * v30;
        v142 = TIM_gf_dt;
        v73.x = TIM_gf_dt * v138->v_grav_speed.x;
        v73.y = v138->v_grav_speed.y * TIM_gf_dt;
        v73.z = v138->v_grav_speed.z * TIM_gf_dt;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v73);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v67);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v101);
        v63.x = v101.x * 20.0f;
        v63.y = v101.y * 20.0f;
        v63.z = v101.z * 20.0f;
        MATH_SubstractVec(&v128, &v67, &v63);
        v138->v_grav_pos = v128;
    }
    v71 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_canaux_Pompier_Pompier_exec_GFX_Halo);
    if (v71 != 1000)
        return v71;
    v68 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_canaux_Pompier_Pompier_exec_GFX_Smoke);
    if (v68 != 1000)
        return v68;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v138->v_last_pos);
    return 1000;
}

int Pompier_Pompier_exec_get_aircraft()
{
    int result; // eax
    unsigned int v1; // eax
    int v2; // eax
    BOOL v3; // eax
    double v4; // st7
    tGameObjectMain* v5; // eax
    tGameObjectMain* v6; // eax
    tScriptStruct_PNJPompier* v7; // [esp+0h] [ebp-1A0h]
    tGameObjectMain* v8; // [esp+8h] [ebp-198h]
    tGameObjectMain* v9; // [esp+10h] [ebp-190h]
    tGameObjectMain* v10; // [esp+18h] [ebp-188h]
    tGameObjectMain* v11; // [esp+1Ch] [ebp-184h]
    tGameObjectMain* v12; // [esp+20h] [ebp-180h]
    tGameObjectMain* v13; // [esp+24h] [ebp-17Ch]
    tGameObjectMain* v14; // [esp+2Ch] [ebp-174h]
    tGameObjectMain* v15; // [esp+38h] [ebp-168h]
    tGameObjectMain* v16; // [esp+3Ch] [ebp-164h]
    tGameObjectMain* v17; // [esp+44h] [ebp-15Ch]
    tGameObjectMain* v18; // [esp+4Ch] [ebp-154h]
    tGameObjectMain* a4; // [esp+54h] [ebp-14Ch]
    tGameObjectMain* v20; // [esp+58h] [ebp-148h]
    tGameObjectMain* a5; // [esp+5Ch] [ebp-144h]
    tGameObjectMain* v22; // [esp+60h] [ebp-140h]
    tGameObjectMain* v23; // [esp+68h] [ebp-138h]
    tGameObjectMain* v24; // [esp+70h] [ebp-130h]
    tGameObjectMain* v25; // [esp+74h] [ebp-12Ch]
    tGameObjectMain* v26; // [esp+78h] [ebp-128h]
    tGameObjectMain* v27; // [esp+80h] [ebp-120h]
    tGameObjectMain* v28; // [esp+88h] [ebp-118h]
    tGameObjectMain* v29; // [esp+90h] [ebp-110h]
    tGameObjectMain* v30; // [esp+94h] [ebp-10Ch]
    tGameObjectMain* v31; // [esp+98h] [ebp-108h]
    tGameObjectMain* v32; // [esp+9Ch] [ebp-104h]
    tGameObjectMain* v33; // [esp+A0h] [ebp-100h]
    float v34; // [esp+ACh] [ebp-F4h]
    Vec3 v35; // [esp+B4h] [ebp-ECh] BYREF
    Vec3 v36; // [esp+C4h] [ebp-DCh] BYREF
    float v37; // [esp+D4h] [ebp-CCh]
    Vec3 v38; // [esp+D8h] [ebp-C8h] BYREF
    Vec3 v39; // [esp+E4h] [ebp-BCh] BYREF
    tGameObjectMain* v40; // [esp+F0h] [ebp-B0h]
    tGameObjectMain* v41; // [esp+F4h] [ebp-ACh]
    Vec3 dest; // [esp+10Ch] [ebp-94h] BYREF
    Vec3 v43; // [esp+118h] [ebp-88h] BYREF
    Vec3 v44; // [esp+124h] [ebp-7Ch] BYREF
    Vec3 v45; // [esp+130h] [ebp-70h] BYREF
    tGameObjectMain* v46; // [esp+140h] [ebp-60h]
    int v47; // [esp+144h] [ebp-5Ch]
    Vec3 v48; // [esp+154h] [ebp-4Ch] BYREF
    int v49; // [esp+160h] [ebp-40h]
    unsigned int v50; // [esp+164h] [ebp-3Ch]
    tScriptStruct_PNJPompier* v51; // [esp+168h] [ebp-38h]
    float v52; // [esp+16Ch] [ebp-34h]
    float v53; // [esp+170h] [ebp-30h]
    int i; // [esp+174h] [ebp-2Ch]
    Vec3 a3; // [esp+178h] [ebp-28h] BYREF
    Vec3 v56; // [esp+188h] [ebp-18h] BYREF
    Vec3 v57; // [esp+194h] [ebp-Ch] BYREF

    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v51 = v7;
    v49 = 0;
    v52 = 0.0;
    v53 = 0.0;
    v46 = 0;
    v40 = 0;
    i = 0;
    v41 = 0;
    v46 = *AI_MainActor;
    v33 = v46;
    if (v46)
    {
        if ((unsigned int)v46 <= 2)
            v33 = AI_MainActor[(uint32_t)v46 - 1];
        v32 = v33;
    }
    else
    {
        v32 = 0;
    }
    if (v32)
    {
        v51->o_aircraft = 0;
        v53 = 1.0e14f;
        v49 = -1;
        for (i = 0; i < 20; ++i)
        {
            v31 = (tGameObjectMain*)v51->des_aircraft[i];
            if (v31)
            {
                if ((unsigned int)v31 <= 2)
                    v31 = AI_MainActor[(uint32_t)v31 - 1];
                v30 = v31;
            }
            else
            {
                v30 = 0;
            }
            if (!v30)
                break;
            v29 = (tGameObjectMain*)v51->des_aircraft[i];
            if (v29)
            {
                if ((unsigned int)v29 <= 2)
                    v29 = AI_MainActor[(uint32_t)v29 - 1];
                v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(v29);
            }
            else
            {
                v1 = AI_EvalFunc_OBJ_FlagsControlGet_C(0);
            }
            v50 = v1;
            if ((v1 & 0x200000) == 0)
            {
                v28 = (tGameObjectMain*)v51->des_aircraft[i];
                if (v28)
                {
                    if ((unsigned int)v28 <= 2)
                        v28 = AI_MainActor[(uint32_t)v28 - 1];
                    v2 = AI_EvalFunc_OBJ_CapaGet_C(v28);
                }
                else
                {
                    v2 = AI_EvalFunc_OBJ_CapaGet_C(0);
                }
                v47 = v2;
                if ((v2 & 0xC00) != 0)
                {
                    v27 = (tGameObjectMain*)v51->des_aircraft[i];
                    if (v27)
                    {
                        if ((unsigned int)v27 <= 2)
                            v27 = AI_MainActor[(uint32_t)v27 - 1];
                        v3 = AI_EvalFunc_OBJ_CapaTest_C(v27, 0x1000);
                    }
                    else
                    {
                        v3 = AI_EvalFunc_OBJ_CapaTest_C(0, 0x1000);
                    }
                    if (!v3)
                    {
                        v26 = (tGameObjectMain*)v51->des_aircraft[i];
                        if (v26)
                        {
                            if ((unsigned int)v26 <= 2)
                                v26 = AI_MainActor[(uint32_t)v26 - 1];
                            v25 = v26;
                        }
                        else
                        {
                            v25 = 0;
                        }
                        v24 = v46;
                        if (v46)
                        {
                            if ((unsigned int)v46 <= 2)
                                v24 = AI_MainActor[(uint32_t)v46 - 1];
                            v4 = AI_EvalFunc_OBJ_SqrDist_C(v24, v25);
                        }
                        else
                        {
                            v4 = AI_EvalFunc_OBJ_SqrDist_C(0, v25);
                        }
                        v52 = (float)v4;
                        if (v52 < (double)v53)
                        {
                            v49 = i;
                            v53 = v52;
                        }
                    }
                }
            }
        }
        if (v49 == -1)
        {
            v16 = (tGameObjectMain*)v51->o_next_wp;
            if (v16)
            {
                if ((unsigned int)v16 <= 2)
                    v16 = AI_MainActor[(uint32_t)v16 - 1];
                v15 = v16;
            }
            else
            {
                v15 = 0;
            }
            if (!v15 && v51->i_current_mode)
            {
                v40 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v51->n_net, -1, 0, 0);
                v14 = (tGameObjectMain*)v51->o_start_pos_wp;
                if (v14)
                {
                    if ((unsigned int)v14 <= 2)
                        v14 = AI_MainActor[(uint32_t)v14 - 1];
                    v6 = AI_EvalFunc_WAYNearestWPOfObj_C(v14, (tWayNetwork*)v51->n_net, -1, 0, 0);
                }
                else
                {
                    v6 = AI_EvalFunc_WAYNearestWPOfObj_C(0, (tWayNetwork*)v51->n_net, -1, 0, 0);
                }
                v41 = v6;
                v13 = v6;
                if (v6)
                {
                    if ((unsigned int)v6 <= 2)
                        v13 = AI_MainActor[(uint32_t)v6 - 1];
                    v12 = v13;
                }
                else
                {
                    v12 = 0;
                }
                v11 = v40;
                if (v40)
                {
                    if ((unsigned int)v40 <= 2)
                        v11 = AI_MainActor[(uint32_t)v40 - 1];
                    v10 = v11;
                }
                else
                {
                    v10 = 0;
                }
                v51->i_wp_nb = AI_EvalFunc_WAYShortWay_C(
                    AI_gpst_CurrentGameObject,
                    (tWayNetwork*)v51->n_net,
                    (tGameObjectMain**)v51->ao_wp,
                    v10,
                    v12);
                if (v51->i_wp_nb > 1)
                {
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v51->v_start_pos);
                    v51->f_rot_blend_speed = 0.0;
                    v9 = (tGameObjectMain*)v51->ao_wp[v51->i_wp_nb - 1];
                    if (v9)
                    {
                        if ((unsigned int)v9 <= 2)
                            v9 = AI_MainActor[(uint32_t)v9 - 1];
                        AI_EvalFunc_OBJGetPos_C(v9, &v43);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v43);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v56);
                    MATH_SubstractVec(&v45, &v43, &v56);
                    v8 = (tGameObjectMain*)v51->ao_wp[v51->i_wp_nb - 2];
                    if (v8)
                    {
                        if ((unsigned int)v8 <= 2)
                            v8 = AI_MainActor[(uint32_t)v8 - 1];
                        AI_EvalFunc_OBJGetPos_C(v8, &v39);
                    }
                    else
                    {
                        AI_EvalFunc_OBJGetPos_C(0, &v39);
                    }
                    AI_EvalFunc_OBJGetPos_C_CURRENT(&v38);
                    MATH_SubstractVec(&v48, &v39, &v38);
                    v34 = v48.z * v45.z + v48.y * v45.y + v45.x * v48.x;
                    if (v34 < 0.0)
                        v51->ao_wp[--v51->i_wp_nb] = 0;
                }
            }
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v51->v_start_pos);
            v51->f_rot_blend_speed = 0.0;
            v51->o_aircraft = v51->des_aircraft[v49];
            v40 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v51->n_net, -1, 0, 0);
            v23 = (tGameObjectMain*)v51->o_aircraft;
            if (v23)
            {
                if ((unsigned int)v23 <= 2)
                    v23 = AI_MainActor[(uint32_t)v23 - 1];
                v5 = AI_EvalFunc_WAYNearestWPOfObj_C(v23, (tWayNetwork*)v51->n_net, -1, 0, 0);
            }
            else
            {
                v5 = AI_EvalFunc_WAYNearestWPOfObj_C(0, (tWayNetwork*)v51->n_net, -1, 0, 0);
            }
            v41 = v5;
            v22 = v5;
            if (v5)
            {
                if ((unsigned int)v5 <= 2)
                    v22 = AI_MainActor[(uint32_t)v5 - 1];
                a5 = v22;
            }
            else
            {
                a5 = 0;
            }
            v20 = v40;
            if (v40)
            {
                if ((unsigned int)v40 <= 2)
                    v20 = AI_MainActor[(uint32_t)v40 - 1];
                a4 = v20;
            }
            else
            {
                a4 = 0;
            }
            v51->i_wp_nb = AI_EvalFunc_WAYShortWay_C(
                AI_gpst_CurrentGameObject,
                (tWayNetwork*)v51->n_net,
                (tGameObjectMain**)v51->ao_wp,
                a4,
                a5);
            if (v51->i_wp_nb > 1)
            {
                v18 = (tGameObjectMain*)v51->ao_wp[v51->i_wp_nb - 1];
                if (v18)
                {
                    if ((unsigned int)v18 <= 2)
                        v18 = AI_MainActor[(uint32_t)v18 - 1];
                    AI_EvalFunc_OBJGetPos_C(v18, &a3);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &a3);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v36);
                MATH_SubstractVec(&dest, &a3, &v36);
                v17 = (tGameObjectMain*)v51->ao_wp[v51->i_wp_nb - 2];
                if (v17)
                {
                    if ((unsigned int)v17 <= 2)
                        v17 = AI_MainActor[(uint32_t)v17 - 1];
                    AI_EvalFunc_OBJGetPos_C(v17, &v35);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v35);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v57);
                MATH_SubstractVec(&v44, &v35, &v57);
                v37 = v44.z * dest.z + v44.y * dest.y + dest.x * v44.x;
                if (v37 < 0.0)
                    v51->ao_wp[--v51->i_wp_nb] = 0;
            }
        }
        result = 1000;
    }
    else
    {
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 1000;
    }
    return result;
}

int Pompier_Police_mode_wait()
{
    int result; // eax
    tScriptStruct_PNJPompier* v1; // [esp+8h] [ebp-28h]
    int v2; // [esp+10h] [ebp-20h]
    float v3; // [esp+14h] [ebp-1Ch]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Pompier_Police_mode_wait_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_wait_univVars_Flag_speciaux_sauve,
            (void*)Pompier_Police_mode_wait_univVars,
            4488);
        AI2C_ResolveBuffer(&Pompier_Police_mode_wait_univVars_f_sally_fric, (void*)Pompier_Police_mode_wait_univVars, 1200);
        fct_Pompier_Police_mode_wait_Pompier_Police_mode_hyllis = (int)AI_ul_RealResolveOneRefFunction(0x720053D3u);
    }
    if (v1->i_current_mode != 5)
    {
        v1->i_current_mode = 5;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        if (v1->i_snd_motor_loop != -1)
        {
            SND_Release(v1->i_snd_motor_loop);
            v1->i_snd_motor_loop = -1;
        }
        if (v1->i_snd_sirene_loop != -1)
        {
            SND_Release(v1->i_snd_sirene_loop);
            v1->i_snd_sirene_loop = -1;
        }
        if (v1->GFX_Halo_1 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo_1);
            v1->GFX_Halo_1 = -1;
        }
        if (v1->GFX_Halo_2 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo_2);
            v1->GFX_Halo_2 = -1;
        }
        if (v1->GFX_Halo_3 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo_3);
            v1->GFX_Halo_3 = -1;
        }
        if (v1->GFX_Halo_4 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo_4);
            v1->GFX_Halo_4 = -1;
        }
        if (v1->GFX_Halo_5 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo_5);
            v1->GFX_Halo_5 = -1;
        }
        if (v1->GFX_Halo_6 != -1)
        {
            AI_EvalFunc_GFXDel_C_CURRENT(v1->GFX_Halo_6);
            v1->GFX_Halo_6 = -1;
        }
    }
    if (AI_EvalFunc_ARRBitTest_C((int*)(Pompier_Police_mode_wait_univVars_Flag_speciaux_sauve + 4), 2718))
        v1->f_time_domz_attack = TIM_gf_MainClock - TIM_gf_LockTime;
    v3 = (float)*(int*)Pompier_Police_mode_wait_univVars_f_sally_fric;
    if (AI_EvalFunc_TIMEElapsed_C(v1->f_time_domz_attack, 5.0) && v3 != 0.0f)
    {
        if (!AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100)
            || (v2 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pompier_Police_mode_wait_Pompier_Police_mode_hyllis),
                v2 == 1000))
        {
            result = 1000;
        }
        else
        {
            result = v2;
        }
    }
    else
    {
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
        *AI_gppst_CurrentJumpNode = (tAINode*)-1;
        result = 4;
    }
    return result;
}

int Pompier_Pompier_mode_wait()
{
    int result; // eax
    tScriptStruct_PNJPompier* v1; // [esp+0h] [ebp-18h]
    tGameObjectMain* v2; // [esp+4h] [ebp-14h]
    tGameObjectMain* v3; // [esp+8h] [ebp-10h]
    int v4; // [esp+Ch] [ebp-Ch]
    int v5; // [esp+10h] [ebp-8h]

    if (AI_gpst_CurrentInstance)
        v1 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v1 = 0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Pompier_Pompier_mode_wait_Pompier_Pompier_exec_get_aircraft = (int)AI_ul_RealResolveOneRefFunction(0x72004DE4u);
        fct_Pompier_Pompier_mode_wait_Pompier_Pompier_mode_hyllis = (int)AI_ul_RealResolveOneRefFunction(0x72004DDEu);
    }
    if (v1->i_current_mode)
    {
        v1->i_current_mode = 0;
        AI_EvalFunc_OBJ_FlagInvisibleSet_C(AI_gpst_CurrentGameObject, 1);
        if (v1->i_snd_motor_loop != -1)
        {
            SND_Release(v1->i_snd_motor_loop);
            v1->i_snd_motor_loop = -1;
        }
        if (v1->i_snd_sirene_loop != -1)
        {
            SND_Release(v1->i_snd_sirene_loop);
            v1->i_snd_sirene_loop = -1;
        }
        if (v1->i_snd_psss_loop != -1)
        {
            SND_Release(v1->i_snd_psss_loop);
            v1->i_snd_psss_loop = -1;
        }
    }
    v4 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Pompier_mode_wait_Pompier_Pompier_exec_get_aircraft);
    if (v4 != 1000)
        return v4;
    v3 = (tGameObjectMain*)v1->o_aircraft;
    if (v3)
    {
        if ((unsigned int)v3 <= 2)
            v3 = AI_MainActor[(uint32_t)v3 - 1];
        v2 = v3;
    }
    else
    {
        v2 = 0;
    }
    if (!v2
        || (v5 = AI_EvalFunc_AITrackCurChangeNow_C((tAIFunction*)fct_Pompier_Pompier_mode_wait_Pompier_Pompier_mode_hyllis),
            v5 == 1000))
    {
        result = 1000;
    }
    else
    {
        result = v5;
    }
    return result;
}

int Pompier_Police_mode_canaux()
{
    tGameObjectMain* v0; // eax
    double v1; // st7
    int v2; // eax
    unsigned int v3; // eax
    int v4; // eax
    double v5; // st7
    double v6; // st7
    double v7; // st7
    double v8; // st7
    int v9; // eax
    double v10; // st7
    tGameObjectMain* v11; // eax
    tGameObjectMain* v12; // eax
    double v13; // st7
    tGameObjectMain* v14; // eax
    tGameObjectMain* v15; // eax
    float a4; // [esp+0h] [ebp-A7Ch]
    float a4a; // [esp+0h] [ebp-A7Ch]
    float a4b; // [esp+0h] [ebp-A7Ch]
    float a4c; // [esp+0h] [ebp-A7Ch]
    float a4d; // [esp+0h] [ebp-A7Ch]
    float a4e; // [esp+0h] [ebp-A7Ch]
    float a4f; // [esp+0h] [ebp-A7Ch]
    float a4g; // [esp+0h] [ebp-A7Ch]
    float a4h; // [esp+0h] [ebp-A7Ch]
    float a4i; // [esp+0h] [ebp-A7Ch]
    float v27; // [esp+8h] [ebp-A74h]
    float v28; // [esp+Ch] [ebp-A70h]
    float v29; // [esp+10h] [ebp-A6Ch]
    float v30; // [esp+14h] [ebp-A68h]
    float v31; // [esp+18h] [ebp-A64h]
    tScriptStruct_PNJPompier* v32; // [esp+1Ch] [ebp-A60h]
    Vec3* v33; // [esp+2Ch] [ebp-A50h]
    Vec3* v34; // [esp+30h] [ebp-A4Ch]
    Vec3* v35; // [esp+3Ch] [ebp-A40h]
    float v36; // [esp+40h] [ebp-A3Ch]
    Vec3* v37; // [esp+48h] [ebp-A34h]
    Vec3* v38; // [esp+4Ch] [ebp-A30h]
    Vec3* v39; // [esp+64h] [ebp-A18h]
    float v40; // [esp+68h] [ebp-A14h]
    double v41; // [esp+8Ch] [ebp-9F0h]
    float v42; // [esp+94h] [ebp-9E8h]
    tGameObjectMain* v43; // [esp+9Ch] [ebp-9E0h]
    float v44; // [esp+A8h] [ebp-9D4h]
    float v45; // [esp+B8h] [ebp-9C4h]
    float v46; // [esp+CCh] [ebp-9B0h]
    Vec3* v47; // [esp+DCh] [ebp-9A0h]
    float v48; // [esp+E0h] [ebp-99Ch]
    float v49; // [esp+E4h] [ebp-998h]
    float v50; // [esp+F0h] [ebp-98Ch]
    float v51; // [esp+F4h] [ebp-988h]
    float v52; // [esp+124h] [ebp-958h]
    float v53; // [esp+128h] [ebp-954h]
    float v54; // [esp+12Ch] [ebp-950h]
    float v55; // [esp+130h] [ebp-94Ch]
    tGameObjectMain* v56; // [esp+144h] [ebp-938h]
    tGameObjectMain* v57; // [esp+14Ch] [ebp-930h]
    tGameObjectMain* v58; // [esp+154h] [ebp-928h]
    tGameObjectMain* v59; // [esp+15Ch] [ebp-920h]
    tGameObjectMain* v60; // [esp+174h] [ebp-908h]
    tGameObjectMain* v61; // [esp+17Ch] [ebp-900h]
    tGameObjectMain* v62; // [esp+184h] [ebp-8F8h]
    tGameObjectMain* v63; // [esp+18Ch] [ebp-8F0h]
    tGameObjectMain* v64; // [esp+1A0h] [ebp-8DCh]
    tGameObjectMain* v65; // [esp+1B4h] [ebp-8C8h]
    tGameObjectMain* v66; // [esp+1BCh] [ebp-8C0h]
    tGameObjectMain* v67; // [esp+1D0h] [ebp-8ACh]
    float v68; // [esp+210h] [ebp-86Ch]
    float v69; // [esp+21Ch] [ebp-860h]
    tGameObjectMain* v70; // [esp+230h] [ebp-84Ch]
    tGameObjectMain* v71; // [esp+238h] [ebp-844h]
    tGameObjectMain* v72; // [esp+240h] [ebp-83Ch]
    tGameObjectMain* v73; // [esp+248h] [ebp-834h]
    tGameObjectMain* v74; // [esp+250h] [ebp-82Ch]
    tGameObjectMain* v75; // [esp+258h] [ebp-824h]
    tGameObjectMain* v76; // [esp+25Ch] [ebp-820h]
    tGameObjectMain* v77; // [esp+260h] [ebp-81Ch]
    tGameObjectMain* v78; // [esp+264h] [ebp-818h]
    tGameObjectMain* v79; // [esp+268h] [ebp-814h]
    tGameObjectMain* v80; // [esp+26Ch] [ebp-810h]
    tGameObjectMain* v81; // [esp+274h] [ebp-808h]
    tGameObjectMain* v82; // [esp+278h] [ebp-804h]
    tGameObjectMain* v83; // [esp+27Ch] [ebp-800h]
    tGameObjectMain* v84; // [esp+284h] [ebp-7F8h]
    tGameObjectMain* v85; // [esp+28Ch] [ebp-7F0h]
    tGameObjectMain* v86; // [esp+294h] [ebp-7E8h]
    tGameObjectMain* v87; // [esp+298h] [ebp-7E4h]
    tGameObjectMain* v88; // [esp+29Ch] [ebp-7E0h]
    Vec3* v89; // [esp+2A0h] [ebp-7DCh]
    Vec3* v90; // [esp+2A4h] [ebp-7D8h]
    tGameObjectMain* v91; // [esp+2ACh] [ebp-7D0h]
    Vec3 v92; // [esp+2B0h] [ebp-7CCh] BYREF
    Vec3 v93; // [esp+2C8h] [ebp-7B4h] BYREF
    Vec3 v94; // [esp+2D4h] [ebp-7A8h] BYREF
    Vec3 v95; // [esp+2E0h] [ebp-79Ch] BYREF
    Vec3 v96; // [esp+2ECh] [ebp-790h] BYREF
    Vec3 v97; // [esp+2F8h] [ebp-784h] BYREF
    Vec3 v98; // [esp+304h] [ebp-778h] BYREF
    Vec3 v99; // [esp+310h] [ebp-76Ch] BYREF
    Vec3 v100; // [esp+31Ch] [ebp-760h] BYREF
    float v101; // [esp+328h] [ebp-754h]
    float v102; // [esp+32Ch] [ebp-750h]
    float v103; // [esp+330h] [ebp-74Ch]
    float v104; // [esp+338h] [ebp-744h]
    Vec3 v105; // [esp+33Ch] [ebp-740h] BYREF
    Vec3 v106; // [esp+348h] [ebp-734h] BYREF
    Vec3 v107; // [esp+354h] [ebp-728h] BYREF
    Vec3 v108; // [esp+360h] [ebp-71Ch] BYREF
    Vec3 v109; // [esp+36Ch] [ebp-710h] BYREF
    Vec3 v110; // [esp+378h] [ebp-704h] BYREF
    Vec3 v111; // [esp+39Ch] [ebp-6E0h] BYREF
    Vec3 v112; // [esp+3A8h] [ebp-6D4h] BYREF
    Vec3 v113; // [esp+3B4h] [ebp-6C8h] BYREF
    Vec3 v114; // [esp+3CCh] [ebp-6B0h] BYREF
    float v115; // [esp+3D8h] [ebp-6A4h]
    float v116; // [esp+3DCh] [ebp-6A0h]
    float v117; // [esp+3E0h] [ebp-69Ch]
    float v118; // [esp+3E8h] [ebp-694h]
    float v119; // [esp+3ECh] [ebp-690h]
    float v120; // [esp+3F0h] [ebp-68Ch]
    Vec3 v121; // [esp+3F8h] [ebp-684h] BYREF
    Vec3 v122; // [esp+404h] [ebp-678h] BYREF
    Vec3 v123; // [esp+410h] [ebp-66Ch] BYREF
    Vec3 v124; // [esp+41Ch] [ebp-660h] BYREF
    Vec3 v125; // [esp+428h] [ebp-654h] BYREF
    int v126; // [esp+434h] [ebp-648h]
    int v127; // [esp+438h] [ebp-644h]
    float v128; // [esp+43Ch] [ebp-640h]
    Vec3 v129; // [esp+448h] [ebp-634h] BYREF
    Vec3 v130; // [esp+454h] [ebp-628h] BYREF
    Vec3 v131; // [esp+460h] [ebp-61Ch] BYREF
    Vec3 v132; // [esp+46Ch] [ebp-610h] BYREF
    Vec3 v133; // [esp+478h] [ebp-604h] BYREF
    float v134; // [esp+488h] [ebp-5F4h]
    float v135; // [esp+48Ch] [ebp-5F0h]
    tMessage Src; // [esp+494h] [ebp-5E8h] BYREF
    Vec3 v137; // [esp+4C8h] [ebp-5B4h] BYREF
    Vec3 v138; // [esp+4D4h] [ebp-5A8h] BYREF
    Vec3 v139; // [esp+4E0h] [ebp-59Ch] BYREF
    Vec3 v140; // [esp+4ECh] [ebp-590h] BYREF
    Vec3 v141; // [esp+4F8h] [ebp-584h]
    Vec3 v142; // [esp+504h] [ebp-578h] BYREF
    Vec3 v143; // [esp+510h] [ebp-56Ch] BYREF
    Vec3 v144; // [esp+51Ch] [ebp-560h] BYREF
    float v145; // [esp+528h] [ebp-554h]
    tGameObjectMain* v146; // [esp+52Ch] [ebp-550h]
    float v147; // [esp+534h] [ebp-548h]
    float v148; // [esp+538h] [ebp-544h]
    float v149; // [esp+53Ch] [ebp-540h]
    Vec3 v150; // [esp+540h] [ebp-53Ch] BYREF
    Vec3 v151; // [esp+54Ch] [ebp-530h] BYREF
    Vec3 v152; // [esp+558h] [ebp-524h] BYREF
    Vec3 v153; // [esp+564h] [ebp-518h] BYREF
    Vec3 v154; // [esp+570h] [ebp-50Ch] BYREF
    Vec3 v155; // [esp+57Ch] [ebp-500h] BYREF
    Vec3 a1; // [esp+588h] [ebp-4F4h] BYREF
    float v158; // [esp+598h] [ebp-4E4h]
    float v159; // [esp+5A4h] [ebp-4D8h]
    float v160; // [esp+5A8h] [ebp-4D4h]
    float v161; // [esp+5ACh] [ebp-4D0h]
    BOOL v162; // [esp+5B4h] [ebp-4C8h]
    Vec3 v163; // [esp+5C4h] [ebp-4B8h] BYREF
    float v164; // [esp+5D0h] [ebp-4ACh]
    float v165; // [esp+5D4h] [ebp-4A8h]
    float v166; // [esp+5D8h] [ebp-4A4h]
    int v167; // [esp+5DCh] [ebp-4A0h]
    float v168; // [esp+5E0h] [ebp-49Ch]
    float v169; // [esp+5E4h] [ebp-498h]
    Vec3 v170; // [esp+5E8h] [ebp-494h] BYREF
    Vec3 v171; // [esp+5F4h] [ebp-488h] BYREF
    Vec3 v172; // [esp+600h] [ebp-47Ch] BYREF
    Vec3 v173; // [esp+60Ch] [ebp-470h] BYREF
    tGameObjectMain* v174; // [esp+61Ch] [ebp-460h]
    Vec3 v175; // [esp+620h] [ebp-45Ch] BYREF
    Vec3 v176; // [esp+638h] [ebp-444h] BYREF
    Vec3 v177; // [esp+644h] [ebp-438h] BYREF
    Vec3 v178; // [esp+650h] [ebp-42Ch] BYREF
    Vec3 v179; // [esp+65Ch] [ebp-420h] BYREF
    float v180; // [esp+674h] [ebp-408h]
    float v181; // [esp+678h] [ebp-404h]
    float v182; // [esp+67Ch] [ebp-400h]
    float v183; // [esp+680h] [ebp-3FCh]
    float v184; // [esp+688h] [ebp-3F4h]
    Vec3 v185; // [esp+68Ch] [ebp-3F0h] BYREF
    Vec3 v186; // [esp+698h] [ebp-3E4h] BYREF
    float v187; // [esp+6A4h] [ebp-3D8h]
    float v188; // [esp+6A8h] [ebp-3D4h]
    float v189; // [esp+6ACh] [ebp-3D0h]
    float v190; // [esp+6B0h] [ebp-3CCh]
    float v191; // [esp+6B4h] [ebp-3C8h]
    float v192; // [esp+6B8h] [ebp-3C4h]
    Vec3 v193; // [esp+6C8h] [ebp-3B4h] BYREF
    Vec3 dest; // [esp+6D4h] [ebp-3A8h] BYREF
    BOOL v195; // [esp+6E8h] [ebp-394h]
    int v196; // [esp+6ECh] [ebp-390h]
    float v197; // [esp+6F0h] [ebp-38Ch]
    float v198; // [esp+6F4h] [ebp-388h]
    Vec3 v199; // [esp+6F8h] [ebp-384h] BYREF
    Vec3 a5; // [esp+704h] [ebp-378h] BYREF
    Vec3 v201; // [esp+710h] [ebp-36Ch] BYREF
    Vec3 a2; // [esp+71Ch] [ebp-360h] BYREF
    Vec3 v203; // [esp+728h] [ebp-354h] BYREF
    Vec3 v204; // [esp+734h] [ebp-348h] BYREF
    Vec3 v205; // [esp+740h] [ebp-33Ch] BYREF
    Vec3 v206; // [esp+74Ch] [ebp-330h] BYREF
    Vec3 v207; // [esp+758h] [ebp-324h] BYREF
    Vec3 v208; // [esp+764h] [ebp-318h] BYREF
    Vec3 v209; // [esp+770h] [ebp-30Ch] BYREF
    Vec3 a3; // [esp+77Ch] [ebp-300h] BYREF
    float v211; // [esp+790h] [ebp-2ECh]
    float v212; // [esp+79Ch] [ebp-2E0h]
    Vec3 v214; // [esp+7A8h] [ebp-2D4h] BYREF
    Vec3 v215; // [esp+7C0h] [ebp-2BCh] BYREF
    Vec3 v216; // [esp+7CCh] [ebp-2B0h] BYREF
    Vec3 v217; // [esp+7D8h] [ebp-2A4h] BYREF
    Vec3 v218; // [esp+7E4h] [ebp-298h] BYREF
    Vec3 v219; // [esp+7F0h] [ebp-28Ch] BYREF
    float v220; // [esp+800h] [ebp-27Ch]
    float v221; // [esp+804h] [ebp-278h]
    float v222; // [esp+808h] [ebp-274h]
    int v223; // [esp+80Ch] [ebp-270h]
    float v224; // [esp+814h] [ebp-268h]
    Vec3 v225; // [esp+818h] [ebp-264h] BYREF
    Vec3 v226; // [esp+824h] [ebp-258h] BYREF
    Vec3 v227; // [esp+830h] [ebp-24Ch] BYREF
    Vec3 v228; // [esp+83Ch] [ebp-240h] BYREF
    float v229; // [esp+848h] [ebp-234h]
    float v230; // [esp+84Ch] [ebp-230h]
    float v231; // [esp+850h] [ebp-22Ch]
    Vec3 v232; // [esp+854h] [ebp-228h] BYREF
    Vec3 v233; // [esp+860h] [ebp-21Ch] BYREF
    Vec3 v234; // [esp+86Ch] [ebp-210h] BYREF
    Vec3 v235; // [esp+878h] [ebp-204h] BYREF
    int v236; // [esp+884h] [ebp-1F8h]
    float v237; // [esp+88Ch] [ebp-1F0h]
    int v238; // [esp+890h] [ebp-1ECh]
    float v239; // [esp+894h] [ebp-1E8h]
    Vec3 v240; // [esp+89Ch] [ebp-1E0h] BYREF
    Vec3 v241; // [esp+8A8h] [ebp-1D4h] BYREF
    Vec3 v242; // [esp+8B4h] [ebp-1C8h] BYREF
    Vec3 v243; // [esp+8C0h] [ebp-1BCh] BYREF
    Vec3 v244; // [esp+8CCh] [ebp-1B0h] BYREF
    Vec3 v245; // [esp+8D8h] [ebp-1A4h] BYREF
    Vec3 v246; // [esp+8E4h] [ebp-198h] BYREF
    float v247; // [esp+8F0h] [ebp-18Ch]
    float v248; // [esp+8F4h] [ebp-188h]
    float v249; // [esp+8FCh] [ebp-180h]
    tGameObjectMain* v250; // [esp+900h] [ebp-17Ch]
    Vec3 v251; // [esp+904h] [ebp-178h] BYREF
    Vec3 v252; // [esp+910h] [ebp-16Ch] BYREF
    Vec3 v253; // [esp+91Ch] [ebp-160h] BYREF
    Vec3 v254; // [esp+934h] [ebp-148h] BYREF
    Vec3 v255; // [esp+940h] [ebp-13Ch] BYREF
    Vec3 v256; // [esp+94Ch] [ebp-130h] BYREF
    Vec3 v257; // [esp+958h] [ebp-124h] BYREF
    tScriptStruct_PNJPompier* v258; // [esp+964h] [ebp-118h]
    float v259; // [esp+968h] [ebp-114h]
    float v260; // [esp+96Ch] [ebp-110h]
    float v261; // [esp+970h] [ebp-10Ch]
    int v262; // [esp+974h] [ebp-108h]
    float v264; // [esp+97Ch] [ebp-100h]
    float v265; // [esp+984h] [ebp-F8h]
    int v266; // [esp+988h] [ebp-F4h]
    float v267; // [esp+98Ch] [ebp-F0h]
    Vec3 v268; // [esp+990h] [ebp-ECh] BYREF
    Vec3 v269; // [esp+99Ch] [ebp-E0h] BYREF
    Vec3 v270; // [esp+9A8h] [ebp-D4h] BYREF
    float v271; // [esp+9B4h] [ebp-C8h]
    float v272; // [esp+9B8h] [ebp-C4h]
    float v273; // [esp+9BCh] [ebp-C0h]
    float v274; // [esp+9C0h] [ebp-BCh]
    float v275; // [esp+9C4h] [ebp-B8h]
    float v276; // [esp+9C8h] [ebp-B4h]
    Vec3 v277; // [esp+9CCh] [ebp-B0h] BYREF
    Vec3 v278; // [esp+9D8h] [ebp-A4h] BYREF
    Vec3 v279; // [esp+9E4h] [ebp-98h] BYREF
    Vec3 v280; // [esp+9F0h] [ebp-8Ch] BYREF
    float v282; // [esp+A04h] [ebp-78h]
    Vec3 v283; // [esp+A18h] [ebp-64h] BYREF
    Vec3 v284; // [esp+A24h] [ebp-58h] BYREF
    Vec3 v285; // [esp+A30h] [ebp-4Ch] BYREF
    Vec3 v286; // [esp+A3Ch] [ebp-40h] BYREF
    Vec3 v287; // [esp+A48h] [ebp-34h] BYREF
    Vec3 v288; // [esp+A54h] [ebp-28h] BYREF
    Vec3 v289; // [esp+A60h] [ebp-1Ch] BYREF
    tGameObjectMain* v290; // [esp+A6Ch] [ebp-10h]
    float v291; // [esp+A70h] [ebp-Ch]
    int v292; // [esp+A74h] [ebp-8h]
    float v293; // [esp+A78h] [ebp-4h]

    v129.x = 1.0;
    v129.y = 0.0;
    v129.z = 0.0;
    v227.x = 1.0;
    v227.y = 0.0;
    v227.z = 0.0;
    v228.x = 1.0;
    v228.y = 0.0;
    v228.z = 0.0;
    v190 = 1.0;
    v191 = 0.0;
    v192 = 0.0;
    a1.x = 1.0;
    a1.y = 0.0;
    a1.z = 0.0;
    a3.x = 1.0;
    a3.y = 0.0;
    a3.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v32 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v32 = 0;
    v258 = v32;
    v236 = 0;
    v290 = 0;
    v261 = 0.0;
    v174 = 0;
    v221 = 0.0;
    v250 = 0;
    v265 = 0.0;
    v224 = 0.0;
    v266 = 0;
    v267 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Pompier_Police_mode_canaux__PJ_Aircraft_gao = AI_ul_RealResolveOneRefObject(0x7100BBDCu);
        Pompier_Police_mode_canaux_univVars = AI_ul_RealResolveOneRefObject(0x71003FF9u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_univVars_f_sally_fric,
            (void*)Pompier_Police_mode_canaux_univVars,
            1200);
        Pompier_Police_mode_canaux_GST_Global_gao = AI_ul_RealResolveOneRefObject(0x710047BCu);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_GST_Global_gao_o_joy_acteur,
            (void*)Pompier_Police_mode_canaux_GST_Global_gao,
            136);
        Pompier_Police_mode_canaux__PJ_Camera_Speciale_gao = AI_ul_RealResolveOneRefObject(0x9E0007E2);
        Pompier_Police_mode_canaux_GST_Board_panel_gao = AI_ul_RealResolveOneRefObject(0x6C00AD97u);
        Pompier_Police_mode_canaux__PJ_Camera_gao = AI_ul_RealResolveOneRefObject(0x71003F56u);
        Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao = AI_ul_RealResolveOneRefObject(0x5E005FA2u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_GST_Board_panel_gao_Gen_Show,
            (void*)Pompier_Police_mode_canaux_GST_Board_panel_gao,
            2260);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour,
            (void*)Pompier_Police_mode_canaux_GST_Board_panel_gao,
            3208);
        Pompier_Police_mode_canaux_INV_manager_gao = AI_ul_RealResolveOneRefObject(0x4900FDB4u);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_INV_manager_gao_i_Fric2Add,
            (void*)Pompier_Police_mode_canaux_INV_manager_gao,
            21476);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_INV_manager_gao_i_FricWithPerso,
            (void*)Pompier_Police_mode_canaux_INV_manager_gao,
            21484);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_INV_manager_gao_f_PogneAppearTimeInit,
            (void*)Pompier_Police_mode_canaux_INV_manager_gao,
            21504);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_INV_manager_gao_f_PogneAppearTimeFixed,
            (void*)Pompier_Police_mode_canaux_INV_manager_gao,
            21496);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_INV_manager_gao_f_PogneDelay,
            (void*)Pompier_Police_mode_canaux_INV_manager_gao,
            21500);
        AI2C_ResolveBuffer(
            &Pompier_Police_mode_canaux_INV_manager_gao_i_OpeningMode,
            (void*)Pompier_Police_mode_canaux_INV_manager_gao,
            100);
        fct_Pompier_Police_mode_canaux_Pompier_Pompier_exec_GFX_Halo = (int)AI_ul_RealResolveOneRefFunction(0x72005349u);
        fct_Pompier_Police_mode_canaux_Pompier_Pompier_exec_GFX_Flash = (int)AI_ul_RealResolveOneRefFunction(0x720053D8u);
    }
    v290 = AI_EvalFunc_VIEWGetObject_C(0);
    if (v258->i_current_mode != 2)
    {
        v258->i_current_mode = 2;
        v258->i_flag_snap = 0;
        v258->o_next_wp = (int)AI_EvalFunc_WAYNearestWPOfObj_C(
            (tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao,
            (tWayNetwork*)v258->n_net,
            -1,
            0,
            0);
        v91 = (tGameObjectMain*)v258->o_next_wp;
        if (v91)
        {
            if ((unsigned int)v91 <= 2)
                v91 = AI_MainActor[(uint32_t)v91 - 1];
            AI_EvalFunc_OBJGetPos_C(v91, &v258->v_start_pos);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v258->v_start_pos);
        }
        v89 = &v258->v_last_pos;
        v90 = &v258->v_start_pos;
        v258->v_last_pos.x = v258->v_start_pos.x;
        v89->y = v90->y;
        v89->z = v90->z;
        AI_EvalFunc_OBJSetPos_C_CURRENT(&v258->v_start_pos);
        v88 = (tGameObjectMain*)v258->o_next_wp;
        if (v88)
        {
            if ((unsigned int)v88 <= 2)
                v88 = AI_MainActor[(uint32_t)v88 - 1];
            v87 = v88;
        }
        else
        {
            v87 = 0;
        }
        v86 = v290;
        if (v290)
        {
            if ((unsigned int)v290 <= 2)
                v86 = AI_MainActor[(uint32_t)v290 - 1];
            v0 = AI_EvalFunc_WAYNetNextWP_C(v86, (tWayNetwork*)v258->n_net, v87, 3, 0);
        }
        else
        {
            v0 = AI_EvalFunc_WAYNetNextWP_C(0, (tWayNetwork*)v258->n_net, v87, 3, 0);
        }
        v258->o_next_wp = (int)v0;
        v85 = (tGameObjectMain*)v258->o_next_wp;
        if (v85)
        {
            if ((unsigned int)v85 <= 2)
                v85 = AI_MainActor[(uint32_t)v85 - 1];
            AI_EvalFunc_OBJGetPos_C(v85, &v114);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v114);
        }
        MATH_SubstractVec(&dest, &v114, &v258->v_start_pos);
        v279.x = 0.0;
        v279.y = 0.0;
        v279.z = 1.0;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&dest, &v279);
        v258->i_flag_stream_played = 1;
    }
    v266 = 0;
    v267 = 0.0;
    v224 = (float)*(int*)Pompier_Police_mode_canaux_univVars_f_sally_fric;
    if (v224 == 0.0 && !v258->i_flag_snap)
        AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
    v84 = (tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao;
    if (Pompier_Police_mode_canaux__PJ_Aircraft_gao)
    {
        if ((unsigned int)Pompier_Police_mode_canaux__PJ_Aircraft_gao <= 2)
            v84 = AI_MainActor[Pompier_Police_mode_canaux__PJ_Aircraft_gao - 1];
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, v84);
    }
    else
    {
        v1 = AI_EvalFunc_OBJ_SqrDist_C(AI_gpst_CurrentGameObject, 0);
    }
    v147 = (float)v1;
    if (v147 < 90000.0)
    {
        v83 = (tGameObjectMain*)v258->des_msk_gao;
        if (v83)
        {
            if ((unsigned int)v83 <= 2)
                v83 = AI_MainActor[(uint32_t)v83 - 1];
            v82 = v83;
        }
        else
        {
            v82 = 0;
        }
        if (v82 && !v258->i_flag_stream_played)
        {
            v258->i_flag_stream_played = 1;
            v81 = (tGameObjectMain*)v258->des_msk_gao;
            if (v81)
            {
                if ((unsigned int)v81 <= 2)
                    v81 = AI_MainActor[(uint32_t)v81 - 1];
                v2 = AI_EvalFunc_SNDRequest_C(v81, 0, 0x8000080);
            }
            else
            {
                v2 = AI_EvalFunc_SNDRequest_C(0, 0, 0x8000080);
            }
            v236 = v2;
            if (v2 != -1)
                AI_EvalFunc_SNDPlaySoundOnTrack_C(AI_gpst_CurrentGameObject, v236);
        }
    }
    if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
    {
        v80 = *(tGameObjectMain**)Pompier_Police_mode_canaux_GST_Global_gao_o_joy_acteur;
        if (*(uint32_t*)Pompier_Police_mode_canaux_GST_Global_gao_o_joy_acteur)
        {
            if ((unsigned int)v80 <= 2)
                v80 = AI_MainActor[(uint32_t)v80 - 1];
            v79 = v80;
        }
        else
        {
            v79 = 0;
        }
        if (v79)
            goto LABEL_254;
    }
    v162 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
    if (v162 && (v224 > 0.0 || v258->i_flag_snap))
    {
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(AI_gpst_CurrentGameObject, v258->i_snd_sirene_loop);
        if (v258->i_flag_compute_best_way)
        {
            v258->i_flag_compute_best_way = 0;
            v258->i_flag_stream_played = 0;
            v258->f_cam_blend_speed = 0.0;
            if (AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Speciale_gao, 0x100))
                AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Pompier_Police_mode_canaux_GST_Board_panel_gao, 0x400, 0);
            AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_gao, &v131);
            AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v131);
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_gao, &v286);
            v199.x = 0.0;
            v199.y = 0.0;
            v199.z = 1.0;
            AI_EvalFunc_OBJ_SightGeneralSet_C(
                (tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao,
                &v286,
                &v199);
            Src.msgID = 32;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 1;
            Src.Int2 = 0;
            Src.f2 = 0.0;
            Src.f1 = 0.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &Src);
            v174 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, -1, 0, 0);
            v250 = AI_EvalFunc_WAYNearestWPOfObj_C(
                (tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao,
                (tWayNetwork*)v258->n_net,
                -1,
                1,
                3);
            v258->o_next_wp = (int)v174;
            v78 = v250;
            if (v250)
            {
                if ((unsigned int)v250 <= 2)
                    v78 = AI_MainActor[(uint32_t)v250 - 1];
                v77 = v78;
            }
            else
            {
                v77 = 0;
            }
            v76 = v174;
            if (v174)
            {
                if ((unsigned int)v174 <= 2)
                    v76 = AI_MainActor[(uint32_t)v174 - 1];
                v75 = v76;
            }
            else
            {
                v75 = 0;
            }
            v3 = AI_EvalFunc_WAYShortWay_C(
                AI_gpst_CurrentGameObject,
                (tWayNetwork*)v258->n_net,
                (tGameObjectMain**)v258->ao_wp,
                v75,
                v77);
            v258->i_wp_nb = v3;
            if (v258->i_wp_nb > 1)
            {
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v258->v_start_pos);
                v258->f_rot_blend_speed = 0.0;
                v74 = (tGameObjectMain*)v258->ao_wp[v258->i_wp_nb - 1];
                if (v74)
                {
                    if ((unsigned int)v74 <= 2)
                        v74 = AI_MainActor[(uint32_t)v74 - 1];
                    AI_EvalFunc_OBJGetPos_C(v74, &v216);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v216);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v172);
                MATH_SubstractVec(&v171, &v216, &v172);
                v73 = (tGameObjectMain*)v258->ao_wp[v258->i_wp_nb - 2];
                if (v73)
                {
                    if ((unsigned int)v73 <= 2)
                        v73 = AI_MainActor[(uint32_t)v73 - 1];
                    AI_EvalFunc_OBJGetPos_C(v73, &v235);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v235);
                }
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v124);
                MATH_SubstractVec(&v268, &v235, &v124);
                v197 = v268.z * v171.z + v268.y * v171.y + v171.x * v268.x;
                if (v197 < 0.0)
                    v258->ao_wp[--v258->i_wp_nb] = 0;
            }
        }
        if (v258->i_wp_nb)
        {
            v72 = (tGameObjectMain*)v258->ao_wp[v258->i_wp_nb - 1];
            if (v72)
            {
                if ((unsigned int)v72 <= 2)
                    v72 = AI_MainActor[(uint32_t)v72 - 1];
                v4 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v72);
            }
            else
            {
                v4 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
            }
            v262 = v4;
            if (v4)
                goto LABEL_105;
            v71 = (tGameObjectMain*)v258->ao_wp[v258->i_wp_nb - 1];
            if (v71)
            {
                if ((unsigned int)v71 <= 2)
                    v71 = AI_MainActor[(uint32_t)v71 - 1];
                AI_EvalFunc_OBJGetPos_C(v71, &v193);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v193);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v201);
            MATH_SubstractVec(&v270, &v193, &v201);
            v70 = (tGameObjectMain*)v258->ao_wp[v258->i_wp_nb - 1];
            if (v70)
            {
                if ((unsigned int)v70 <= 2)
                    v70 = AI_MainActor[(uint32_t)v70 - 1];
                AI_EvalFunc_OBJGetPos_C(v70, &v139);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v139);
            }
            MATH_SubstractVec(&v283, &v139, &v258->v_start_pos);
            v237 = v283.z * v270.z + v283.y * v270.y + v270.x * v283.x;
            if (v237 < 0.0)
            {
            LABEL_105:
                v258->f_rot_blend_speed = 0.0;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v258->v_start_pos);
                --v258->i_wp_nb;
            }
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v144);
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &v241);
        MATH_SubstractVec(&v269, &v144, &v241);
        a1.x = v269.x;
        a1.y = v269.y;
        a1.z = 0.0;
        v265 = MATH_VecLen(&a1);
        if (v265 != 0.0f)
        {
            v69 = 1.0f / v265;
            a1.x = a1.x * v69;
            a1.y = a1.y * v69;
            a1.z = a1.z * v69;
        }
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &v176);
        v229 = a1.x * 8.0f;
        v230 = a1.y * 8.0f;
        v231 = a1.z * 8.0f;
        v129.x = v176.x - v229;
        v129.y = v176.y - v230;
        v129.z = v176.z - v231;
        v141.x = 0.0;
        v141.y = 0.0;
        v141.z = 4.0;
        v129.x = v129.x + 0.0f;
        v129.y = v129.y + 0.0f;
        v129.z = v129.z + 4.0f;
        v180 = TIM_gf_dt;
        v68 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v68 = 0.0;
        if (v68 > 1.0)
            v68 = 1.0;
        v291 = 6.0f * v68 + v258->f_cam_blend_speed * (1.0f - v68);
        v258->f_cam_blend_speed = v291;
        AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v246);
        v135 = TIM_gf_dt;
        a4 = v258->f_cam_blend_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlend_C(&v246, &v129, a4, &v243);
        v129 = v243;
        AI_EvalFunc_DYNSpeedGetVector_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &v289);
        v149 = TIM_gf_dt;
        v187 = v289.x * TIM_gf_dt;
        v188 = v289.y * TIM_gf_dt;
        v189 = v289.z * TIM_gf_dt;
        v129.x = v129.x + v187;
        v129.y = v129.y + v188;
        v129.z = v129.z + v189;
        *(uint32_t*)Pompier_Police_mode_canaux_GST_Board_panel_gao_Gen_Show = 0;
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Pompier_Police_mode_canaux_GST_Board_panel_gao, 0x200, 0);
        AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Pompier_Police_mode_canaux_GST_Board_panel_gao, 0x100, 0);
        AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_gao, 1);
        v146 = AI_EvalFunc_VIEWAssignObject_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, 0);
        AI_EvalFunc_OBJSetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v129);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v177);
        MATH_SubstractVec(&v97, &v177, &v129);
        a3 = v97;
        MATH_NormalizeEqualVector(&a3);
        AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v242);
        v103 = a3.z * v242.z + a3.y * v242.y + v242.x * a3.x;
        if (v103 <= 0.0)
        {
            AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v94);
            v148 = a3.z * v94.z + a3.y * v94.y + v94.x * a3.x;
            if (v148 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v175);
                AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v95);
                v232.x = -v95.x;
                v232.y = -v95.y;
                v232.z = -v95.z;
                v181 = TIM_gf_dt;
                a4c = v258->f_cam_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v175, &v232, a4c, &v218);
                a3 = v218;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v93);
                AI_EvalFunc_OBJ_HorizonGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &v278);
                v116 = TIM_gf_dt;
                a4b = v258->f_cam_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v93, &v278, a4b, &v98);
                a3 = v98;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &a2);
            v134 = TIM_gf_dt;
            a4a = v258->f_cam_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4a, &a5);
            a3 = a5;
        }
        v226.x = 0.0;
        v226.y = 0.0;
        v226.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_Derriere_gao, &a3, &v226);
        if (v258->i_wp_nb)
        {
            v266 = 1;
            v258->o_next_wp = *(&v258->__ao_wp_Dimension1 + v258->i_wp_nb);
            v67 = (tGameObjectMain*)v258->o_next_wp;
            if (v67)
            {
                if ((unsigned int)v67 <= 2)
                    v67 = AI_MainActor[(uint32_t)v67 - 1];
                AI_EvalFunc_OBJGetPos_C(v67, &v245);
            }
            else
            {
                AI_EvalFunc_OBJGetPos_C(0, &v245);
            }
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v105);
            MATH_SubstractVec(&v214, &v245, &v105);
            v228 = v214;
            v5 = MATH_VecLen(&v228);
            v258->f_dist_to_dest = (float)v5;
        }
        else
        {
            v66 = (tGameObjectMain*)v258->o_next_wp;
            if (v66)
            {
                if ((unsigned int)v66 <= 2)
                    v66 = AI_MainActor[(uint32_t)v66 - 1];
                v6 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v66);
            }
            else
            {
                v6 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v247 = (float)v6;
            if (v247 <= 100.0)
            {
                v64 = (tGameObjectMain*)v258->o_next_wp;
                if (v64)
                {
                    if ((unsigned int)v64 <= 2)
                        v64 = AI_MainActor[(uint32_t)v64 - 1];
                    AI_EvalFunc_OBJGetPos_C(v64, &v100);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v100);
                }
                v228.x = v100.x;
                v228.y = v100.y;
                v228.z = 10.0;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v284);
                v228.x = v228.x - v284.x;
                v228.y = v228.y - v284.y;
                v228.z = v228.z - v284.z;
                v8 = MATH_VecLen(&v228);
                v258->f_dist_to_dest = (float)v8;
            }
            else
            {
                v266 = 1;
                v65 = (tGameObjectMain*)v258->o_next_wp;
                if (v65)
                {
                    if ((unsigned int)v65 <= 2)
                        v65 = AI_MainActor[(uint32_t)v65 - 1];
                    AI_EvalFunc_OBJGetPos_C(v65, &v209);
                }
                else
                {
                    AI_EvalFunc_OBJGetPos_C(0, &v209);
                }
                v228 = v209;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v123);
                v228.x = v228.x - v123.x;
                v228.y = v228.y - v123.y;
                v228.z = v228.z - v123.z;
                v7 = MATH_VecLen(&v228);
                v258->f_dist_to_dest = (float)v7;
            }
        }
    }
    else
    {
        SND_StopRq(v258->i_snd_sirene_loop);
        v258->i_snd_sirene_loop = -1;
        v258->i_flag_compute_best_way = 1;
        v266 = 1;
        v63 = (tGameObjectMain*)v258->o_next_wp;
        if (v63)
        {
            if ((unsigned int)v63 <= 2)
                v63 = AI_MainActor[(uint32_t)v63 - 1];
            v9 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, v63);
        }
        else
        {
            v9 = AI_EvalFunc_COLPivotBVCollide_C(AI_gpst_CurrentGameObject, 0);
        }
        v223 = v9;
        if (v9)
            goto LABEL_159;
        v62 = (tGameObjectMain*)v258->o_next_wp;
        if (v62)
        {
            if ((unsigned int)v62 <= 2)
                v62 = AI_MainActor[(uint32_t)v62 - 1];
            AI_EvalFunc_OBJGetPos_C(v62, &v234);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v234);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v107);
        MATH_SubstractVec(&v122, &v234, &v107);
        v61 = (tGameObjectMain*)v258->o_next_wp;
        if (v61)
        {
            if ((unsigned int)v61 <= 2)
                v61 = AI_MainActor[(uint32_t)v61 - 1];
            AI_EvalFunc_OBJGetPos_C(v61, &v173);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v173);
        }
        MATH_SubstractVec(&v186, &v173, &v258->v_start_pos);
        v102 = v186.z * v122.z + v186.y * v122.y + v122.x * v186.x;
        if (v102 < 0.0)
        {
        LABEL_159:
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v258->v_start_pos);
            v196 = (unsigned __int16)AI_gpst_CurrentGameObject->flags2;
            if ((v196 & 4) != 0)
                goto LABEL_166;
            v60 = (tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao;
            if (Pompier_Police_mode_canaux__PJ_Aircraft_gao)
            {
                if ((unsigned int)Pompier_Police_mode_canaux__PJ_Aircraft_gao <= 2)
                    v60 = AI_MainActor[Pompier_Police_mode_canaux__PJ_Aircraft_gao - 1];
                v10 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, v60);
            }
            else
            {
                v10 = AI_EvalFunc_OBJ_SqrDistHorz_C(AI_gpst_CurrentGameObject, 0);
            }
            v183 = (float)v10;
            if (v183 > 40000.0)
            {
            LABEL_166:
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v285);
                a1 = v285;
                AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &v280);
                v129 = v280;
                v59 = v290;
                if (v290)
                {
                    if ((unsigned int)v290 <= 2)
                        v59 = AI_MainActor[(uint32_t)v290 - 1];
                    AI_EvalFunc_OBJ_SightGet_C(v59, &v132);
                }
                else
                {
                    AI_EvalFunc_OBJ_SightGet_C(0, &v132);
                }
                v271 = v132.x * 100.0f;
                v272 = v132.y * 100.0f;
                v273 = v132.z * 100.0f;
                v129.x = v129.x + v271;
                v129.y = v129.y + v272;
                v129.z = v129.z + v273;
                AI_EvalFunc_OBJGetPos_C_CURRENT(&v140);
                MATH_SubstractVec(&v163, &v129, &v140);
                v130.x = 0.0;
                v130.y = 0.0;
                v130.z = 1.0;
                AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v163, &v130);
                v58 = (tGameObjectMain*)v258->o_next_wp;
                if (v58)
                {
                    if ((unsigned int)v58 <= 2)
                        v58 = AI_MainActor[(uint32_t)v58 - 1];
                    v11 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, v58, 3, 0);
                }
                else
                {
                    v11 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, 0, 3, 0);
                }
                v258->o_next_wp = (int)v11;
                v178.x = 0.0;
                v178.y = 0.0;
                v178.z = 1.0;
                AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a1, &v178);
            }
            else
            {
                v57 = (tGameObjectMain*)v258->o_next_wp;
                if (v57)
                {
                    if ((unsigned int)v57 <= 2)
                        v57 = AI_MainActor[(uint32_t)v57 - 1];
                    v12 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, v57, 3, 0);
                }
                else
                {
                    v12 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, 0, 3, 0);
                }
                v258->o_next_wp = (int)v12;
            }
            v258->f_rot_blend_speed = 0.0;
        }
        v56 = (tGameObjectMain*)v258->o_next_wp;
        if (v56)
        {
            if ((unsigned int)v56 <= 2)
                v56 = AI_MainActor[(uint32_t)v56 - 1];
            AI_EvalFunc_OBJGetPos_C(v56, &v287);
        }
        else
        {
            AI_EvalFunc_OBJGetPos_C(0, &v287);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v225);
        MATH_SubstractVec(&v179, &v287, &v225);
        v228 = v179;
        v13 = MATH_VecLen(&v228);
        v258->f_dist_to_dest = (float)v13;
    }
    if (v266)
    {
        v258->i_flag_reset_rot_coef = 1;
        v55 = v258->f_dist_to_dest / 60.0f;
        if (v55 > 1.0)
            v31 = 1.0;
        else
            v31 = v258->f_dist_to_dest / 60.0f;
        v293 = v31;
        v261 = 1.0f - v31;
        v239 = TIM_gf_dt;
        v54 = 1.5f * TIM_gf_dt;
        if (v261 < (double)v54)
            v30 = 1.5f * TIM_gf_dt;
        else
            v30 = v261;
        v184 = v30;
        v261 = v30;
        v53 = v30;
        if (v30 < 0.0)
            v53 = 0.0;
        if (v53 > 1.0)
            v53 = 1.0;
        v258->f_rot_blend_speed = 12.0f * v53 + v258->f_rot_blend_speed * (1.0f - v53);
        if (v258->f_dist_to_dest == 0.0f)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v92);
            v228 = v92;
        }
        else
        {
            v52 = 1.0f / v258->f_dist_to_dest;
            v228.x = v228.x * v52;
            v228.y = v228.y * v52;
            v228.z = v228.z * v52;
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v244);
        v185.x = 0.0;
        v185.y = 0.0;
        v185.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &v244, &v185);
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v142);
        v104 = v228.z * v142.z + v228.y * v142.y + v142.x * v228.x;
        if (v104 <= 0.0)
        {
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v96);
            v117 = v228.z * v96.z + v228.y * v96.y + v96.x * v228.x;
            if (v117 <= 0.0)
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v251);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v205);
                v153.x = -v205.x;
                v153.y = -v205.y;
                v153.z = -v205.z;
                v120 = TIM_gf_dt;
                a4f = v258->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v251, &v153, a4f, &v256);
                a3 = v256;
            }
            else
            {
                AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v254);
                AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v208);
                v159 = TIM_gf_dt;
                a4e = v258->f_rot_blend_speed * TIM_gf_dt;
                AI_EvalFunc_MATHVecBlendRotate_C(&v254, &v208, a4e, &v112);
                a3 = v112;
            }
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v240);
            v198 = TIM_gf_dt;
            a4d = v258->f_rot_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v240, &v228, a4d, &v138);
            a3 = v138;
        }
        v252.x = 0.0;
        v252.y = 0.0;
        v252.z = 1.0;
        AI_EvalFunc_OBJ_SightGeneralSet_C(AI_gpst_CurrentGameObject, &a3, &v252);
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v207);
        v221 = v228.z * v207.z + v228.y * v207.y + v207.x * v228.x;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v110);
        v264 = v228.z * v110.z + v228.y * v110.y + v110.x * v228.x;
        if (v264 < 0.0)
            v221 = AI_EvalFunc_MTHFloatSign_C(v221);
        v160 = TIM_gf_dt;
        v51 = TIM_gf_dt;
        if (TIM_gf_dt < 0.0)
            v51 = 0.0;
        if (v51 > 1.0)
            v51 = 1.0;
        v119 = v221 * 1.570796f * v51 + v258->f_banking_coef * (1.0f - v51);
        v258->f_banking_coef = v119;
        AI_EvalFunc_OBJRotateLocalY_C(AI_gpst_CurrentGameObject, v258->f_banking_coef);
        if (AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100))
        {
            if (v258->i_wp_nb <= 1)
            {
                if (v258->f_speed > (double)v258->f_dist_to_dest)
                    v29 = v258->f_dist_to_dest;
                else
                    v29 = v258->f_speed;
                v258->f_speed = v29;
            }
            else
            {
                v212 = TIM_gf_dt;
                v50 = 2.0f * TIM_gf_dt;
                if (v50 < 0.0)
                    v50 = 0.0;
                if (v50 > 1.0)
                    v50 = 1.0;
                v145 = 100.0f * v50 + v258->f_speed * (1.0f - v50);
                v258->f_speed = v145;
            }
        }
        else
        {
            v248 = TIM_gf_dt;
            v49 = 2.0f * TIM_gf_dt;
            if (v49 < 0.0)
                v49 = 0.0;
            if (v49 > 1.0)
                v49 = 1.0;
            v101 = 40.0f * v49 + v258->f_speed * (1.0f - v49);
            v258->f_speed = v101;
        }
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v137);
        v48 = v258->f_speed;
        v47 = &v258->v_grav_speed;
        v258->v_grav_speed.x = v137.x * v48;
        v47->y = v137.y * v48;
        v47->z = v137.z * v48;
        v249 = TIM_gf_dt;
        v99.x = TIM_gf_dt * v258->v_grav_speed.x;
        v99.y = v258->v_grav_speed.y * TIM_gf_dt;
        v99.z = v258->v_grav_speed.z * TIM_gf_dt;
        AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v99);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v143);
        a1 = v143;
        v195 = AI_EvalFunc_OBJ_CapaTest_C_CURRENT(0x100);
        if (!v195 && a1.z < 40.0)
        {
            v115 = TIM_gf_dt;
            v46 = 2.0f * TIM_gf_dt;
            if (v46 < 0.0)
                v46 = 0.0;
            if (v46 > 1.0)
                v46 = 1.0;
            v158 = 40.0f * v46 + (1.0f - v46) * a1.z;
            a1.z = v158;
            AI_EvalFunc_OBJSetPos_C_CURRENT(&a1);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v204);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v257);
        v111.x = v257.x * 20.0f;
        v111.y = v257.y * 20.0f;
        v111.z = v257.z * 20.0f;
        MATH_SubstractVec(&v151, &v204, &v111);
        v258->v_grav_pos = v151;
        goto LABEL_274;
    }
    v258->f_time_phase = TIM_gf_MainClock - TIM_gf_LockTime;
    if (v258->i_flag_reset_rot_coef)
    {
        v258->i_flag_reset_rot_coef = 0;
        v258->f_rot_blend_speed = 0.0;
    }
    if (v258->f_speed > (double)v258->f_dist_to_dest)
        v28 = v258->f_dist_to_dest;
    else
        v28 = v258->f_speed;
    v258->f_speed = v28;
    v258->f_banking_coef = 0.0;
    if (v258->f_dist_to_dest == 0.0)
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v106);
        v228 = v106;
    }
    else
    {
        v45 = 1.0f / v258->f_dist_to_dest;
        v228.x = v228.x * v45;
        v228.y = v228.y * v45;
        v228.z = v228.z * v45;
    }
    v161 = TIM_gf_dt;
    v44 = TIM_gf_dt;
    if (TIM_gf_dt < 0.0)
        v44 = 0.0;
    if (v44 > 1.0)
        v44 = 1.0;
    v260 = 2.0f * v44 + v258->f_rot_blend_speed * (1.0f - v44);
    v258->f_rot_blend_speed = v260;
    AI_EvalFunc_OBJGetPos_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &v108);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v154);
    MATH_SubstractVec(&v203, &v108, &v154);
    a3 = v203;
    AI_EvalFunc_MATHVecSetHorzNormalize_C(&a3);
    AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v255);
    v267 = a3.z * v255.z + a3.y * v255.y + v255.x * a3.x;
    if (!v258->i_flag_snap)
    {
        if (v258->f_dist_to_dest < 2.0 && v267 > 0.99619502)
        {
            v258->i_flag_snap = 1;
            *(uint32_t*)Pompier_Police_mode_canaux_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour = 0;
            v258->f_time_fine = TIM_gf_MainClock - TIM_gf_LockTime;
            v42 = (float)*(int*)Pompier_Police_mode_canaux_univVars_f_sally_fric;
            if (v42 < 250.0)
                v27 = (float)*(int*)Pompier_Police_mode_canaux_univVars_f_sally_fric;
            else
                v27 = 250.0;
            v265 = v27;
            v41 = MATH_gd_magicDoubleForRound - v27;
            *(uint32_t*)Pompier_Police_mode_canaux_INV_manager_gao_i_Fric2Add = (*(uint32_t *)&v41);
            *(uint32_t*)Pompier_Police_mode_canaux_INV_manager_gao_i_FricWithPerso = 12;
            *(uint32_t*)Pompier_Police_mode_canaux_INV_manager_gao_f_PogneAppearTimeInit = 0x3F800000;
            *(uint32_t*)Pompier_Police_mode_canaux_INV_manager_gao_f_PogneAppearTimeFixed = 0x3E4CCCCD;
            *(uint32_t*)Pompier_Police_mode_canaux_INV_manager_gao_f_PogneDelay = 0x3E4CCCCD;
            *(uint32_t*)Pompier_Police_mode_canaux_INV_manager_gao_i_OpeningMode = 19;
            AI_EvalFunc_OBJ_CustomBitsSet_C((tGameObjectMain*)Pompier_Police_mode_canaux_GST_Board_panel_gao, 0, 0x100);
            AI_EvalFunc_OBJ_CapaSet_C((tGameObjectMain*)Pompier_Police_mode_canaux_GST_Board_panel_gao, 0x1000, 0);
        }
        goto LABEL_266;
    }
    if (*(uint32_t*)Pompier_Police_mode_canaux_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour)
    {
        if (!AI_EvalFunc_OBJ_CapaTest_C((tGameObjectMain*)Pompier_Police_mode_canaux_GST_Board_panel_gao, 0x1000))
        {
        LABEL_254:
            *(uint32_t*)Pompier_Police_mode_canaux_GST_Board_panel_gao_i_var_sorti_inv_de_merde_pour = 0;
            *(uint32_t*)Pompier_Police_mode_canaux_GST_Board_panel_gao_Gen_Show = 1;
            v258->i_flag_snap = 0;
            AI_EvalFunc_OBJ_FlagInactiveSet_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Camera_gao, 0);
            Src.msgID = 32;
            Src.gao = AI_gpst_CurrentGameObject;
            Src.gao2 = 0;
            Src.Int1 = 1;
            Src.Int2 = 1;
            Src.f2 = 0.0;
            Src.f1 = 0.0;
            Src.v1.x = 0.0;
            Src.v1.y = 0.0;
            Src.v1.z = 0.0;
            Src.v2.x = 0.0;
            Src.v2.y = 0.0;
            Src.v2.z = 0.0;
            AI_EvalFunc_MSGSend_C((tGameObjectMain*)Pompier_Police_mode_canaux__PJ_Aircraft_gao, &Src);
            AI_EvalFunc_OBJ_CapaSet_C_CURRENT(0, 0x100);
            v258->f_speed = 0.0;
            v258->f_rot_blend_speed = 0.0;
            AI_EvalFunc_OBJGetPos_C_CURRENT(&v258->v_last_pos);
            v14 = AI_EvalFunc_WAYNearestWPOfObj_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, -1, 0, 0);
            v258->o_next_wp = (int)v14;
            v43 = (tGameObjectMain*)v258->o_next_wp;
            if (v43)
            {
                if ((unsigned int)v43 <= 2)
                    v43 = AI_MainActor[(uint32_t)v43 - 1];
                v15 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, v43, 3, 0);
            }
            else
            {
                v15 = AI_EvalFunc_WAYNetNextWP_C(AI_gpst_CurrentGameObject, (tWayNetwork*)v258->n_net, 0, 3, 0);
            }
            v258->o_next_wp = (int)v15;
        }
    }
LABEL_266:
    if (v267 <= 0.0)
    {
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v170);
        v222 = a3.z * v170.z + a3.y * v170.y + v170.x * a3.x;
        if (v222 <= 0.0)
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v150);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v109);
            v253.x = -v109.x;
            v253.y = -v109.y;
            v253.z = -v109.z;
            v211 = TIM_gf_dt;
            a4i = v258->f_rot_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v150, &v253, a4i, &v155);
            a3 = v155;
        }
        else
        {
            AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v152);
            AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v113);
            v259 = TIM_gf_dt;
            a4h = v258->f_rot_blend_speed * TIM_gf_dt;
            AI_EvalFunc_MATHVecBlendRotate_C(&v152, &v113, a4h, &v206);
            a3 = v206;
        }
    }
    else
    {
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v215);
        v282 = TIM_gf_dt;
        a4g = v258->f_rot_blend_speed * TIM_gf_dt;
        AI_EvalFunc_MATHVecBlendRotate_C(&v215, &a3, a4g, &v121);
        a3 = v121;
    }
    v118 = TIM_gf_dt;
    v40 = v258->f_speed * TIM_gf_dt;
    v277.x = v228.x * v40;
    v277.y = v228.y * v40;
    v277.z = v228.z * v40;
    AI_EvalFunc_OBJMove_C(AI_gpst_CurrentGameObject, &v277);
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v219);
    v167 = 0;
    v168 = 0.0;
    v169 = v219.z - v258->v_last_pos.z;
    v39 = &v258->v_grav_pos;
    v258->v_grav_pos.x = v258->v_grav_pos.x + 0.0f;
    v39->y = v168 + v39->y;
    v39->z = v169 + v39->z;
    v227 = v258->v_grav_pos;
    v126 = 0;
    v127 = 0;
    v128 = -15.0;
    v190 = 0.0;
    v191 = 0.0;
    v192 = -15.0;
    v274 = v258->v_grav_speed.x * 0.98000002f;
    v275 = v258->v_grav_speed.y * 0.98000002f;
    v276 = v258->v_grav_speed.z * 0.98000002f;
    v190 = v274 + 0.0f;
    v191 = v275 + 0.0f;
    v192 = v276 + -15.0f;
    v220 = TIM_gf_dt;
    v164 = v190 * TIM_gf_dt;
    v165 = v191 * TIM_gf_dt;
    v166 = v192 * TIM_gf_dt;
    v38 = &v258->v_grav_pos;
    v258->v_grav_pos.x = v164 + v258->v_grav_pos.x;
    v38->y = v165 + v38->y;
    v38->z = v166 + v38->z;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v125);
    v37 = &v258->v_grav_pos;
    v258->v_grav_pos.x = v258->v_grav_pos.x - v125.x;
    v37->y = v37->y - v125.y;
    v37->z = v37->z - v125.z;
    v265 = MATH_VecLen(&v258->v_grav_pos);
    if (v265 != 0.0f)
    {
        v35 = &v258->v_grav_pos;
        v36 = 1.0f / v265;
        v258->v_grav_pos.x = v36 * v258->v_grav_pos.x;
        v35->y = v36 * v35->y;
        v35->z = v36 * v35->z;
        v217.x = -v258->v_grav_pos.x;
        v217.y = -v258->v_grav_pos.y;
        v217.z = -v258->v_grav_pos.z;
        AI_EvalFunc_OBJ_BankingGeneralSet_C_CURRENT(&a3, &v217);
        v34 = &v258->v_grav_pos;
        v258->v_grav_pos.x = v258->v_grav_pos.x * 20.0f;
        v34->y = v34->y * 20.0f;
        v34->z = v34->z * 20.0f;
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v288);
        v33 = &v258->v_grav_pos;
        v258->v_grav_pos.x = v288.x + v258->v_grav_pos.x;
        v33->y = v288.y + v33->y;
        v33->z = v288.z + v33->z;
    }
    MATH_SubstractVec(&v233, &v258->v_grav_pos, &v227);
    v182 = TIM_gf_dt;
    MATH_VecMultNormScalar(&v133, &v233, TIM_gf_dt);
    v258->v_grav_speed = v133;
LABEL_274:
    v292 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Police_mode_canaux_Pompier_Pompier_exec_GFX_Halo);
    if (v292 != 1000)
        return v292;
    v238 = AI_EvalFunc_AIExecute_C((tAIFunction*)fct_Pompier_Police_mode_canaux_Pompier_Pompier_exec_GFX_Flash);
    if (v238 != 1000)
        return v238;
    AI_EvalFunc_OBJGetPos_C_CURRENT(&v258->v_last_pos);
    return 1000;
}

int Pompier_Pompier_exec_GFX_Halo()
{
    float X; // [esp+0h] [ebp-528h]
    float Xa; // [esp+0h] [ebp-528h]
    float Xb; // [esp+0h] [ebp-528h]
    float Xc; // [esp+0h] [ebp-528h]
    float Xd; // [esp+0h] [ebp-528h]
    float Xe; // [esp+0h] [ebp-528h]
    float Xf; // [esp+0h] [ebp-528h]
    tScriptStruct_PNJPompier* v8; // [esp+4h] [ebp-524h]
    tGameObjectMain* v9; // [esp+1Ch] [ebp-50Ch]
    tGameObjectMain* v10; // [esp+2Ch] [ebp-4FCh]
    tGameObjectMain* v11; // [esp+3Ch] [ebp-4ECh]
    tGameObjectMain* v12; // [esp+4Ch] [ebp-4DCh]
    tGameObjectMain* v13; // [esp+5Ch] [ebp-4CCh]
    tGameObjectMain* v14; // [esp+74h] [ebp-4B4h]
    tGameObjectMain* v15; // [esp+8Ch] [ebp-49Ch]
    tGameObjectMain* v16; // [esp+A4h] [ebp-484h]
    tGameObjectMain* v17; // [esp+B4h] [ebp-474h]
    float v18; // [esp+B8h] [ebp-470h]
    tGameObjectMain* v19; // [esp+C8h] [ebp-460h]
    float v20; // [esp+CCh] [ebp-45Ch]
    float v21; // [esp+D0h] [ebp-458h]
    float v22; // [esp+D4h] [ebp-454h]
    Vec3 v23; // [esp+D8h] [ebp-450h] BYREF
    Vec3 v24; // [esp+E4h] [ebp-444h] BYREF
    float v25; // [esp+F0h] [ebp-438h]
    float v26; // [esp+F4h] [ebp-434h]
    float v27; // [esp+F8h] [ebp-430h]
    Vec3 v28; // [esp+FCh] [ebp-42Ch] BYREF
    float v29; // [esp+108h] [ebp-420h]
    float v30; // [esp+10Ch] [ebp-41Ch]
    float v31; // [esp+110h] [ebp-418h]
    Vec3 v32; // [esp+114h] [ebp-414h] BYREF
    Vec3 v33; // [esp+120h] [ebp-408h]
    Vec3 v34; // [esp+12Ch] [ebp-3FCh] BYREF
    float v35; // [esp+13Ch] [ebp-3ECh]
    Vec3 v36; // [esp+140h] [ebp-3E8h]
    Vec3 v37; // [esp+14Ch] [ebp-3DCh] BYREF
    float v38; // [esp+158h] [ebp-3D0h]
    float v39; // [esp+15Ch] [ebp-3CCh]
    float v40; // [esp+160h] [ebp-3C8h]
    Vec3 v41; // [esp+164h] [ebp-3C4h] BYREF
    Vec3 a1; // [esp+170h] [ebp-3B8h] BYREF
    Vec3 v43; // [esp+184h] [ebp-3A4h] BYREF
    float v44; // [esp+190h] [ebp-398h]
    float v45; // [esp+194h] [ebp-394h]
    float v46; // [esp+198h] [ebp-390h]
    int v47; // [esp+1A0h] [ebp-388h]
    Vec3 v48; // [esp+1A4h] [ebp-384h] BYREF
    float v49; // [esp+1B0h] [ebp-378h]
    float v50; // [esp+1B4h] [ebp-374h]
    float v51; // [esp+1B8h] [ebp-370h]
    Vec3 v52; // [esp+1BCh] [ebp-36Ch] BYREF
    float v53; // [esp+1C8h] [ebp-360h]
    float v54; // [esp+1CCh] [ebp-35Ch]
    float v55; // [esp+1D0h] [ebp-358h]
    int v56; // [esp+1D8h] [ebp-350h]
    Vec3 v57; // [esp+1DCh] [ebp-34Ch] BYREF
    Vec3 v58; // [esp+1E8h] [ebp-340h] BYREF
    Vec3 v59; // [esp+1F4h] [ebp-334h] BYREF
    Vec3 v60; // [esp+200h] [ebp-328h]
    float v61; // [esp+20Ch] [ebp-31Ch]
    float v62; // [esp+210h] [ebp-318h]
    float v63; // [esp+214h] [ebp-314h]
    float v64; // [esp+218h] [ebp-310h]
    float v65; // [esp+21Ch] [ebp-30Ch]
    float v66; // [esp+220h] [ebp-308h]
    float v67; // [esp+224h] [ebp-304h]
    float v68; // [esp+228h] [ebp-300h]
    float v69; // [esp+22Ch] [ebp-2FCh]
    float v70; // [esp+230h] [ebp-2F8h]
    float v71; // [esp+234h] [ebp-2F4h]
    float v72; // [esp+238h] [ebp-2F0h]
    Vec3 v73; // [esp+23Ch] [ebp-2ECh] BYREF
    float v74; // [esp+24Ch] [ebp-2DCh]
    float v75; // [esp+250h] [ebp-2D8h]
    float v76; // [esp+254h] [ebp-2D4h]
    float v77; // [esp+258h] [ebp-2D0h]
    float v78; // [esp+25Ch] [ebp-2CCh]
    Vec3 v79; // [esp+260h] [ebp-2C8h] BYREF
    float v80; // [esp+26Ch] [ebp-2BCh]
    float v81; // [esp+270h] [ebp-2B8h]
    float v82; // [esp+274h] [ebp-2B4h]
    float v83; // [esp+278h] [ebp-2B0h]
    float v84; // [esp+27Ch] [ebp-2ACh]
    float v85; // [esp+280h] [ebp-2A8h]
    Vec3 v86; // [esp+284h] [ebp-2A4h] BYREF
    Vec3 v87; // [esp+290h] [ebp-298h] BYREF
    float v88; // [esp+29Ch] [ebp-28Ch]
    float v89; // [esp+2A0h] [ebp-288h]
    float v90; // [esp+2A4h] [ebp-284h]
    Vec3 v91; // [esp+2A8h] [ebp-280h] BYREF
    Vec3 v92; // [esp+2B4h] [ebp-274h] BYREF
    Vec3 v93; // [esp+2C0h] [ebp-268h] BYREF
    Vec3 v94; // [esp+2CCh] [ebp-25Ch] BYREF
    Vec3 v95; // [esp+2D8h] [ebp-250h]
    float v96; // [esp+2E4h] [ebp-244h]
    float v97; // [esp+2E8h] [ebp-240h]
    float v98; // [esp+2ECh] [ebp-23Ch]
    int v99; // [esp+2F0h] [ebp-238h]
    Vec3 v100; // [esp+2F4h] [ebp-234h] BYREF
    float v101; // [esp+300h] [ebp-228h]
    float v102; // [esp+304h] [ebp-224h]
    float v103; // [esp+308h] [ebp-220h]
    Vec3 v104; // [esp+30Ch] [ebp-21Ch] BYREF
    float v105; // [esp+318h] [ebp-210h]
    float v106; // [esp+31Ch] [ebp-20Ch]
    float v107; // [esp+320h] [ebp-208h]
    Vec3 v108; // [esp+324h] [ebp-204h] BYREF
    Vec3 v109; // [esp+330h] [ebp-1F8h] BYREF
    int v110; // [esp+33Ch] [ebp-1ECh]
    Vec3 v111; // [esp+340h] [ebp-1E8h] BYREF
    Vec3 v112; // [esp+34Ch] [ebp-1DCh] BYREF
    Vec3 v113; // [esp+358h] [ebp-1D0h] BYREF
    Vec3 v114; // [esp+364h] [ebp-1C4h] BYREF
    float v115; // [esp+370h] [ebp-1B8h]
    float v116; // [esp+374h] [ebp-1B4h]
    float v117; // [esp+378h] [ebp-1B0h]
    Vec3 v118; // [esp+37Ch] [ebp-1ACh] BYREF
    float v119; // [esp+388h] [ebp-1A0h]
    float v120; // [esp+38Ch] [ebp-19Ch]
    float v121; // [esp+390h] [ebp-198h]
    float v122; // [esp+394h] [ebp-194h]
    float v123; // [esp+398h] [ebp-190h]
    float v124; // [esp+39Ch] [ebp-18Ch]
    float v125; // [esp+3A0h] [ebp-188h]
    Vec3 v126; // [esp+3A4h] [ebp-184h] BYREF
    Vec3 v127; // [esp+3B0h] [ebp-178h] BYREF
    Vec3 v128; // [esp+3C8h] [ebp-160h] BYREF
    float v129; // [esp+3D4h] [ebp-154h]
    float v130; // [esp+3D8h] [ebp-150h]
    float v131; // [esp+3DCh] [ebp-14Ch]
    Vec3 v132; // [esp+3E0h] [ebp-148h]
    float v133; // [esp+3ECh] [ebp-13Ch]
    float v134; // [esp+3F0h] [ebp-138h]
    float v135; // [esp+3F4h] [ebp-134h]
    float v136; // [esp+3F8h] [ebp-130h]
    float v137; // [esp+3FCh] [ebp-12Ch]
    float v138; // [esp+400h] [ebp-128h]
    float v139; // [esp+404h] [ebp-124h]
    float v140; // [esp+408h] [ebp-120h]
    float v141; // [esp+40Ch] [ebp-11Ch]
    Vec3 v142; // [esp+410h] [ebp-118h] BYREF
    Vec3 v143; // [esp+41Ch] [ebp-10Ch] BYREF
    int v144; // [esp+428h] [ebp-100h]
    Vec3 v145; // [esp+42Ch] [ebp-FCh]
    Vec3 v146; // [esp+438h] [ebp-F0h]
    Vec3 v147; // [esp+444h] [ebp-E4h] BYREF
    Vec3 v148; // [esp+450h] [ebp-D8h] BYREF
    float v149; // [esp+464h] [ebp-C4h]
    Vec3 v150; // [esp+468h] [ebp-C0h] BYREF
    tScriptStruct_PNJPompier* v151; // [esp+474h] [ebp-B4h]
    Vec3 a2; // [esp+478h] [ebp-B0h] BYREF
    float v153; // [esp+484h] [ebp-A4h]
    float v154; // [esp+488h] [ebp-A0h]
    float v155; // [esp+48Ch] [ebp-9Ch]
    float v156; // [esp+490h] [ebp-98h]
    float v157; // [esp+494h] [ebp-94h]
    float v158; // [esp+498h] [ebp-90h]
    float v159; // [esp+49Ch] [ebp-8Ch]
    float v160; // [esp+4A0h] [ebp-88h]
    float v161; // [esp+4A4h] [ebp-84h]
    Vec3 v162; // [esp+4A8h] [ebp-80h] BYREF
    Vec3 v163; // [esp+4B4h] [ebp-74h] BYREF
    int v164; // [esp+4C0h] [ebp-68h]
    float v165; // [esp+4C8h] [ebp-60h]
    float v166; // [esp+4CCh] [ebp-5Ch]
    float v167; // [esp+4D0h] [ebp-58h]
    float v168; // [esp+4D4h] [ebp-54h]
    float v169; // [esp+4D8h] [ebp-50h]
    float v170; // [esp+4DCh] [ebp-4Ch]
    Vec3 v171; // [esp+4E0h] [ebp-48h] BYREF
    float v172; // [esp+4ECh] [ebp-3Ch]
    float v173; // [esp+4F0h] [ebp-38h]
    float v174; // [esp+4F4h] [ebp-34h]
    Vec3 v175; // [esp+4F8h] [ebp-30h] BYREF
    Vec3 v176; // [esp+504h] [ebp-24h] BYREF
    Vec3 v177; // [esp+510h] [ebp-18h]
    Vec3 v178; // [esp+51Ch] [ebp-Ch] BYREF

    v73.x = 1.0;
    v73.y = 0.0;
    v73.z = 0.0;
    if (AI_gpst_CurrentInstance)
        v8 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v8 = 0;
    v151 = v8;
    v35 = 0.0;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao = AI_ul_RealResolveOneRefObject(0x62000F8Fu);
    }
    if (v151->des_type)
    {
        v75 = TIM_gf_dt;
        v151->f_angle_halo = TIM_gf_dt * 6.283185f + v151->f_angle_halo;
        if (v151->f_angle_halo >= 6.283185f)
            v151->f_angle_halo = v151->f_angle_halo - 6.283185f;
        v35 = sinf(v151->f_angle_halo);
        if (v151->GFX_Halo_1 == -1)
        {
            v151->GFX_Halo_1 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_1, 4, 1);
            v19 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v19 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_1, v19, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_1, 0, 29);
            }
            GFXSetCurGlobal(v151->GFX_Halo_1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 2.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        v47 = AI_EvalFunc_COLORBlend_C(16512, 255, v35);
        GFXSetCurGlobal(v151->GFX_Halo_1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v47);
        X = v35 + 2.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, X);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a1);
        v73 = a1;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v108);
        v159 = v108.x * 2.8f;
        v160 = v108.y * 2.8f;
        v161 = v108.z * 2.8f;
        v73.x = v73.x + v159;
        v73.y = v73.y + v160;
        v73.z = v73.z + v161;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v59);
        v123 = v59.x * 0.60000002f;
        v124 = v59.y * 0.60000002f;
        v125 = v59.z * 0.60000002f;
        v73.x = v73.x + v123;
        v73.y = v73.y + v124;
        v73.z = v73.z + v125;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&a2);
        v74 = TIM_gf_MainClock - TIM_gf_LockTime;
        Xa = v74 * 10.0f;
        v122 = sinf(Xa);
        v18 = v122 + v122;
        v53 = a2.x * v18;
        v54 = a2.y * v18;
        v55 = a2.z * v18;
        v73.x = v73.x + v53;
        v73.y = v73.y + v54;
        v73.z = v73.z + v55;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v175);
        v115 = v175.x * 5.0f;
        v116 = v175.y * 5.0f;
        v117 = v175.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_2 == -1)
        {
            v151->GFX_Halo_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_2, 4, 1);
            v17 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v17 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_2, v17, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_2, 0, 29);
            }
            GFXSetCurGlobal(v151->GFX_Halo_2);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        v99 = AI_EvalFunc_COLORBlend_C(32, 0x80C0C0, v35);
        GFXSetCurGlobal(v151->GFX_Halo_2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v99);
        Xb = v35 + 2.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xb);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v41);
        v73 = v41;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v93);
        v139 = v93.x * 1.0f;
        v140 = v93.y * 1.0f;
        v141 = v93.z * 1.0f;
        v73.x = v73.x - v139;
        v73.y = v73.y - v140;
        v73.z = v73.z - v141;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v114);
        v172 = v114.x * 3.7f;
        v173 = v114.y * 3.7f;
        v174 = v114.z * 3.7f;
        v73.x = v73.x - v172;
        v73.y = v73.y - v173;
        v73.z = v73.z - v174;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v126);
        v80 = v126.x * 3.5f;
        v81 = v126.y * 3.5f;
        v82 = v126.z * 3.5f;
        v73.x = v73.x + v80;
        v73.y = v73.y + v81;
        v73.z = v73.z + v82;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v79);
        v129 = v79.x * 5.0f;
        v130 = v79.y * 5.0f;
        v131 = v79.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_3 == -1)
        {
            v151->GFX_Halo_3 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_3, 4, 1);
            v16 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v16 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_3, v16, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_3, 0, 29);
            }
            GFXSetCurGlobal(v151->GFX_Halo_3);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        v164 = AI_EvalFunc_COLORBlend_C(0x80C0C0, 32, v35);
        GFXSetCurGlobal(v151->GFX_Halo_3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v164);
        Xc = 2.0f - v35;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xc);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v100);
        v73 = v100;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v150);
        v105 = v150.x * 1.0f;
        v106 = v150.y * 1.0f;
        v107 = v150.z * 1.0f;
        v73.x = v73.x - v105;
        v73.y = v73.y - v106;
        v73.z = v73.z - v107;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v112);
        v153 = v112.x * 3.7f;
        v154 = v112.y * 3.7f;
        v155 = v112.z * 3.7f;
        v73.x = v73.x - v153;
        v73.y = v73.y - v154;
        v73.z = v73.z - v155;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v57);
        v168 = v57.x * 3.5f;
        v169 = v57.y * 3.5f;
        v170 = v57.z * 3.5f;
        v73.x = v73.x - v168;
        v73.y = v73.y - v169;
        v73.z = v73.z - v170;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v128);
        v36.x = -v128.x;
        v36.y = -v128.y;
        v36.z = -v128.z;
        v60.x = -v128.x * 5.0f;
        v60.y = -v128.y * 5.0f;
        v60.z = -v128.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_4 == -1)
        {
            v151->GFX_Halo_4 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_4, 4, 1);
            v15 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v15 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_4, v15, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_4, 0, 0);
            }
            GFXSetCurGlobal(v151->GFX_Halo_4);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
        }
        GFXSetCurGlobal(v151->GFX_Halo_4);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 255);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v162);
        v73 = v162;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v111);
        v88 = v111.x * 6.5f;
        v89 = v111.y * 6.5f;
        v90 = v111.z * 6.5f;
        v73.x = v73.x - v88;
        v73.y = v73.y - v89;
        v73.z = v73.z - v90;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v142);
        v70 = v142.x * 1.5f;
        v71 = v142.y * 1.5f;
        v72 = v142.z * 1.5f;
        v73.x = v73.x + v70;
        v73.y = v73.y + v71;
        v73.z = v73.z + v72;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v92);
        v177.x = -v92.x;
        v177.y = -v92.y;
        v177.z = -v92.z;
        v146.x = -v92.x * 5.0f;
        v146.y = -v92.y * 5.0f;
        v146.z = -v92.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_5 == -1)
        {
            v151->GFX_Halo_5 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_5, 4, 1);
            v14 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v14 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_5, v14, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_5, 0, 0);
            }
            GFXSetCurGlobal(v151->GFX_Halo_5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
        }
        GFXSetCurGlobal(v151->GFX_Halo_5);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 255);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v148);
        v73 = v148;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v178);
        v67 = v178.x * 6.5f;
        v68 = v178.y * 6.5f;
        v69 = v178.z * 6.5f;
        v73.x = v73.x - v67;
        v73.y = v73.y - v68;
        v73.z = v73.z - v69;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v109);
        v64 = v109.x * 1.5f;
        v65 = v109.y * 1.5f;
        v66 = v109.z * 1.5f;
        v73.x = v73.x - v64;
        v73.y = v73.y - v65;
        v73.z = v73.z - v66;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v91);
        v33.x = -v91.x;
        v33.y = -v91.y;
        v33.z = -v91.z;
        v145.x = -v91.x * 5.0f;
        v145.y = -v91.y * 5.0f;
        v145.z = -v91.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_6 == -1)
        {
            v151->GFX_Halo_6 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_6, 4, 1);
            v13 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v13 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_6, v13, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_6, 0, 0);
            }
            GFXSetCurGlobal(v151->GFX_Halo_6);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
        }
        GFXSetCurGlobal(v151->GFX_Halo_6);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFFFFC0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v113);
        v73 = v113;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v52);
        v119 = v52.x * 6.3000002f;
        v120 = v52.y * 6.3000002f;
        v121 = v52.z * 6.3000002f;
        v73.x = v73.x + v119;
        v73.y = v73.y + v120;
        v73.z = v73.z + v121;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v28);
        v61 = v28.x * 1.7f;
        v62 = v28.y * 1.7f;
        v63 = v28.z * 1.7f;
        v73.x = v73.x + v61;
        v73.y = v73.y + v62;
        v73.z = v73.z + v63;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v24);
        v156 = v24.x * 5.0f;
        v157 = v24.y * 5.0f;
        v158 = v24.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_7 == -1)
        {
            v151->GFX_Halo_7 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_7, 4, 1);
            v12 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v12 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_7, v12, 0);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_7, 0, 0);
            }
            GFXSetCurGlobal(v151->GFX_Halo_7);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 0.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21000, 5.0);
        }
        GFXSetCurGlobal(v151->GFX_Halo_7);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, 0xFFFFC0);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v34);
        v73 = v34;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v87);
        v20 = v87.x * 6.3000002f;
        v21 = v87.y * 6.3000002f;
        v22 = v87.z * 6.3000002f;
        v73.x = v73.x + v20;
        v73.y = v73.y + v21;
        v73.z = v73.z + v22;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v143);
        v96 = v143.x * 1.7f;
        v97 = v143.y * 1.7f;
        v98 = v143.z * 1.7f;
        v73.x = v73.x - v96;
        v73.y = v73.y - v97;
        v73.z = v73.z - v98;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v127);
        v136 = v127.x * 5.0f;
        v137 = v127.y * 5.0f;
        v138 = v127.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
    }
    else
    {
        v149 = TIM_gf_dt;
        v151->f_angle_halo = TIM_gf_dt * 12.56637f + v151->f_angle_halo;
        if (v151->f_angle_halo >= 6.283185f)
            v151->f_angle_halo = v151->f_angle_halo - 6.283185f;
        v35 = sinf(v151->f_angle_halo);
        if (v151->GFX_Halo_1 == -1)
        {
            v151->GFX_Halo_1 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_1, 4, 1);
            v11 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v11 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_1, v11, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_1, 0, 29);
            }
            GFXSetCurGlobal(v151->GFX_Halo_1);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 10.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        v110 = AI_EvalFunc_COLORBlend_C(32, 255, v35);
        GFXSetCurGlobal(v151->GFX_Halo_1);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v110);
        Xd = v35 + 2.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xd);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v147);
        v73 = v147;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v118);
        v44 = v118.x * 3.8f;
        v45 = v118.y * 3.8f;
        v46 = v118.z * 3.8f;
        v73.x = v73.x + v44;
        v73.y = v73.y + v45;
        v73.z = v73.z + v46;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v32);
        v29 = v32.x * 1.6f;
        v30 = v32.y * 1.6f;
        v31 = v32.z * 1.6f;
        v73.x = v73.x + v29;
        v73.y = v73.y + v30;
        v73.z = v73.z + v31;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v171);
        v38 = v171.x * 0.40000001f;
        v39 = v171.y * 0.40000001f;
        v40 = v171.z * 0.40000001f;
        v73.x = v73.x - v38;
        v73.y = v73.y - v39;
        v73.z = v73.z - v40;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_2 == -1)
        {
            v151->GFX_Halo_2 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_2, 4, 1);
            v10 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v10 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_2, v10, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_2, 0, 29);
            }
            GFXSetCurGlobal(v151->GFX_Halo_2);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 1.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        v144 = AI_EvalFunc_COLORBlend_C(4128, 33023, v35);
        GFXSetCurGlobal(v151->GFX_Halo_2);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v144);
        Xe = v35 + 2.0f;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xe);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v37);
        v73 = v37;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v43);
        v83 = v43.x * 3.8f;
        v84 = v43.y * 3.8f;
        v85 = v43.z * 3.8f;
        v73.x = v73.x - v83;
        v73.y = v73.y - v84;
        v73.z = v73.z - v85;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v104);
        v25 = v104.x * 0.80000001f;
        v26 = v104.y * 0.80000001f;
        v27 = v104.z * 0.80000001f;
        v73.x = v73.x - v25;
        v73.y = v73.y - v26;
        v73.z = v73.z - v27;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v23);
        v101 = v23.x * 6.1999998f;
        v102 = v23.y * 6.1999998f;
        v103 = v23.z * 6.1999998f;
        v73.x = v73.x + v101;
        v73.y = v73.y + v102;
        v73.z = v73.z + v103;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v86);
        v165 = v86.x * 5.0f;
        v166 = v86.y * 5.0f;
        v167 = v86.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
        if (v151->GFX_Halo_3 == -1)
        {
            v151->GFX_Halo_3 = AI_EvalFunc_GFXAdd_C_CURRENT(21);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v151->GFX_Halo_3, 4, 1);
            v9 = (tGameObjectMain*)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao;
            if (fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao)
            {
                if ((unsigned int)fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao <= 2)
                    v9 = AI_MainActor[fct_Pompier_Pompier_exec_GFX_Halo_SFX_light_smoke_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_3, v9, 29);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v151->GFX_Halo_3, 0, 29);
            }
            GFXSetCurGlobal(v151->GFX_Halo_3);
            AI_EvalFunc_GFXSetf_C_CURGLOB(21002, 1.0);
            AI_EvalFunc_GFXSeti_C_CURGLOB(21103, 1);
        }
        v56 = AI_EvalFunc_COLORBlend_C(33023, 4128, v35);
        GFXSetCurGlobal(v151->GFX_Halo_3);
        AI_EvalFunc_GFXSeti_C_CURGLOB(21101, v56);
        Xf = 2.0f - v35;
        AI_EvalFunc_GFXSetf_C_CURGLOB(21000, Xf);
        AI_EvalFunc_OBJGetPos_C_CURRENT(&v163);
        v73 = v163;
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v58);
        v76 = v58.x * 3.8f;
        v77 = v58.y * 3.8f;
        v78 = v58.z * 3.8f;
        v73.x = v73.x - v76;
        v73.y = v73.y - v77;
        v73.z = v73.z - v78;
        AI_EvalFunc_OBJ_SightGet_C_CURRENT(&v48);
        v133 = v48.x * 0.80000001f;
        v134 = v48.y * 0.80000001f;
        v135 = v48.z * 0.80000001f;
        v73.x = v73.x - v133;
        v73.y = v73.y - v134;
        v73.z = v73.z - v135;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v94);
        v49 = v94.x * 7.1999998f;
        v50 = v94.y * 7.1999998f;
        v51 = v94.z * 7.1999998f;
        v73.x = v73.x - v49;
        v73.y = v73.y - v50;
        v73.z = v73.z - v51;
        AI_EvalFunc_OBJ_HorizonGet_C_CURRENT(&v176);
        v132.x = -v176.x;
        v132.y = -v176.y;
        v132.z = -v176.z;
        v95.x = -v176.x * 5.0f;
        v95.y = -v176.y * 5.0f;
        v95.z = -v176.z * 5.0f;
        AI_EvalFunc_GFXSetv_C_CURGLOB(21200, &v73);
    }
    return 1000;
}

int Pompier_Pompier_exec_GFX_Smoke()
{
    float v1; // [esp+0h] [ebp-150h]
    float v2; // [esp+4h] [ebp-14Ch]
    float v3; // [esp+8h] [ebp-148h]
    float v4; // [esp+Ch] [ebp-144h]
    float v5; // [esp+10h] [ebp-140h]
    float v6; // [esp+14h] [ebp-13Ch]
    tScriptStruct_PNJPompier* v7; // [esp+18h] [ebp-138h]
    tGameObjectMain* v8; // [esp+48h] [ebp-108h]
    Vec3 v9; // [esp+58h] [ebp-F8h] BYREF
    Vec3 v10; // [esp+64h] [ebp-ECh] BYREF
    Vec3 v11; // [esp+70h] [ebp-E0h]
    Vec3 a3; // [esp+7Ch] [ebp-D4h] BYREF
    float v13; // [esp+88h] [ebp-C8h]
    float v14; // [esp+8Ch] [ebp-C4h]
    float v15; // [esp+90h] [ebp-C0h]
    Vec3 v16; // [esp+94h] [ebp-BCh]
    float v17; // [esp+A0h] [ebp-B0h]
    float v18; // [esp+A8h] [ebp-A8h]
    float v19; // [esp+ACh] [ebp-A4h]
    float v20; // [esp+B0h] [ebp-A0h]
    float v21; // [esp+B4h] [ebp-9Ch]
    float v22; // [esp+B8h] [ebp-98h]
    Vec3 a2; // [esp+BCh] [ebp-94h] BYREF
    Vec3 v24; // [esp+C8h] [ebp-88h]
    Vec3 v25; // [esp+D4h] [ebp-7Ch] BYREF
    float v26; // [esp+E0h] [ebp-70h]
    float v27; // [esp+E4h] [ebp-6Ch]
    Vec3 v28; // [esp+E8h] [ebp-68h] BYREF
    Vec3 dest; // [esp+F4h] [ebp-5Ch] BYREF
    Vec3 v30; // [esp+100h] [ebp-50h] BYREF
    float v31; // [esp+10Ch] [ebp-44h]
    Vec3 v32; // [esp+110h] [ebp-40h] BYREF
    tScriptStruct_PNJPompier* v33; // [esp+11Ch] [ebp-34h]
    Vec3 v34; // [esp+120h] [ebp-30h] BYREF
    Vec3 a1; // [esp+12Ch] [ebp-24h] BYREF
    float v36; // [esp+138h] [ebp-18h]
    float v37; // [esp+13Ch] [ebp-14h]
    float v38; // [esp+140h] [ebp-10h]
    Vec3 v39; // [esp+144h] [ebp-Ch]

    v30.x = 1.0;
    v30.y = 0.0;
    v30.z = 0.0;
    v13 = 1.0;
    v14 = 0.0;
    v15 = 0.0;
    v25.x = 1.0;
    v25.y = 0.0;
    v25.z = 0.0;
    v18 = 1.0;
    v19 = 0.0;
    v20 = 0.0;
    if (AI_gpst_CurrentInstance)
        v7 = (tScriptStruct_PNJPompier*)AI_gpst_CurrentInstance->instanceVarDataCurrent;
    else
        v7 = 0;
    v33 = v7;
    if (!AI_gpst_CurrentFunction->varsResolved)
    {
        AI_gpst_CurrentFunction->varsResolved = 1;
        Pompier_Pompier_exec_GFX_Smoke_SFX_explosion_gao = AI_ul_RealResolveOneRefObject(0x9E0031D5);
    }
    if (v33->i_flag_smoke)
    {
        if (v33->GFX_Smoke == -1)
        {
            v33->GFX_Smoke = AI_EvalFunc_GFXAdd_C_CURRENT(13);
            GFXSetCurGlobal(v33->GFX_Smoke);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13100, 150);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13106, 0xD4A50FFF);
            v8 = (tGameObjectMain*)Pompier_Pompier_exec_GFX_Smoke_SFX_explosion_gao;
            if (Pompier_Pompier_exec_GFX_Smoke_SFX_explosion_gao)
            {
                if ((unsigned int)Pompier_Pompier_exec_GFX_Smoke_SFX_explosion_gao <= 2)
                    v8 = AI_MainActor[Pompier_Pompier_exec_GFX_Smoke_SFX_explosion_gao - 1];
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Smoke, v8, -1);
            }
            else
            {
                AI_EvalFunc_GFXMaterialSet_C_CURRENT(v33->GFX_Smoke, 0, -1);
            }
            GFXSetCurGlobal(v33->GFX_Smoke);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13101, 7);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13103, 0x60FFFFFF);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13104, 0x30FFFFFF);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13105, 0xFFFFFF);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13005, 0.40000001f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13006, 0.80000001f);
            AI_EvalFunc_GFXSeti_C_CURGLOB(13107, 0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13000, 3.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13001, 6.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13002, 0.000099999997f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13003, 1.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13004, 0.5);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13007, -4.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13008, 0.0099999998f);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13009, 15.0);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13010, 25.0);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Smoke, 0, 1);
            AI_EvalFunc_GFXFlagSet_C_CURRENT(v33->GFX_Smoke, 2, 1);
            GFXSetCurGlobal(v33->GFX_Smoke);
            AI_EvalFunc_GFXSetf_C_CURGLOB(13012, 0.75);
            v32.x = 0.0099999998f;
            v32.y = 0.0099999998f;
            v32.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13203, &v32);
            v28.x = 0.0;
            v28.y = 0.0;
            v28.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13204, &v28);
            v10.x = 0.0;
            v10.y = 0.0;
            v10.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13205, &v10);
            v9.x = 0.0;
            v9.y = 0.0;
            v9.z = 0.0;
            AI_EvalFunc_GFXSetv_C_CURGLOB(13206, &v9);
        }
        AI_EvalFunc_OBJGetPos_C_CURRENT(&a2);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&a1);
        a3.x = a1.x * 3.0f;
        a3.y = a1.y * 3.0f;
        a3.z = a1.z * 3.0f;
        MATH_SubstractVec(&dest, &a2, &a3);
        GFXSetCurGlobal(v33->GFX_Smoke);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13200, &dest);
        AI_EvalFunc_OBJ_BankingGet_C_CURRENT(&v34);
        v16.x = -v34.x;
        v16.y = -v34.y;
        v16.z = -v34.z;
        v24.x = -v34.x * 5.0f;
        v24.y = -v34.y * 5.0f;
        v24.z = -v34.z * 5.0f;
        v11.x = 5.0;
        v11.y = 5.0;
        v11.z = 0.0;
        v13 = v24.x - 5.0f;
        v14 = v24.y - 5.0f;
        v15 = v24.z - 0.0f;
        v39.x = 5.0;
        v39.y = 5.0;
        v39.z = 0.0;
        v18 = v24.x + 5.0f;
        v19 = v24.y + 5.0f;
        v20 = v24.z + 0.0f;
        if (v13 > (double)v18)
            v6 = v18;
        else
            v6 = v13;
        v27 = v6;
        if (v14 > (double)v19)
            v5 = v19;
        else
            v5 = v14;
        v17 = v5;
        if (v15 > (double)v20)
            v4 = v20;
        else
            v4 = v15;
        v21 = v4;
        v25.x = v27;
        v25.y = v17;
        v25.z = v4;
        if (v13 < (double)v18)
            v3 = v18;
        else
            v3 = v13;
        v22 = v3;
        if (v14 < (double)v19)
            v2 = v19;
        else
            v2 = v14;
        v31 = v2;
        if (v15 < (double)v20)
            v1 = v20;
        else
            v1 = v15;
        v26 = v1;
        v36 = v22;
        v37 = v31;
        v38 = v1;
        v30.x = v22;
        v30.y = v31;
        v30.z = v1;
        AI_EvalFunc_GFXSetv_C_CURGLOB(13201, &v25);
        AI_EvalFunc_GFXSetv_C_CURGLOB(13202, &v30);
    }
    else if (v33->GFX_Smoke != -1)
    {
        AI_EvalFunc_GFXSeti_C_CURRENT(v33->GFX_Smoke, 13106, 0);
        v33->GFX_Smoke = -1;
    }
    return 1000;
}