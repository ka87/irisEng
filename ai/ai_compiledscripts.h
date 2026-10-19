#pragma once
#include "../ai.h"

typedef struct sStruct_funclist
{
    int key;
    int(*func)();
} tStruct_funclist;

//extern tStruct_funclist AI2C_gat_fctdefs[136];
extern tStruct_funclist AI2C_gat_fctdefs[2530];

unsigned int AI2C_i_SizeofArray();


// hack
typedef struct sScript_mlStruct {
    float f_Chrono;
    int i_ModePause;
    int i_PauseState;
    float f_Time; //12
    int sound; //16 ////////
    int texte; //20 ////
    int o_Logo2Langue; //24 ////
    int o_Langue2Start; //28 /////
    int o_End2Start; //32
    int o_Logo2Start; //36
    int o_CurEvent; // 40
    int o_EventCamera; // 44
    int o_LangueTitle; // 48
    int o_LangueSel; // 52
    int o_MenuSel; // 56
    Vec3 v_LangueTitle_Pos; // 60
    Vec3 v_LangueTitle_Offset; // 72
    int __v_LanguePos_Dimension; // 84
    Vec3 v_LanguePos[6]; // 86
    int i_fade; // 160
    float f_focal; // 164
    int des_i_lang; // 168 //////
    tTextGet txt_lang; // 172
    int i_langcur; // 180
    int i_langnb; // 184
    int __i_langval_Dimension; // 188
    int i_langval[10]; // 192
    int i_NoPad; //232
    int i_CurrentState; // 236
    int i_CurrentLine; // 240
    int i_WaitZeroJoy; // 244
    tTextGet txt_MemCard; // 248
    int MC_slot; // 256
    int __MC_button_Dimension; // 260
    int MC_button[8]; // 264
    int MC_result; // 296
    int MC_action; // 300
} tScript_mlStruct;

typedef struct sScript_E3Struct
{
    int mode_main;
    int i_map;
    int o_capa;
    float time;
    int o_camera;
    int i_effect;
    int __o_pad_Dimension;
    int o_pad[3];
    int __o_PadButtons_Dimension;
    int o_PadButtons[3];
    tTextGet www;
} tScript_E3Struct;

typedef struct sScriptStruct_INVManager {
    int __ao_Camera_Dimension1;  // 0
    int ao_Camera[9];  // 4
    int o_Camera;  // 40
    int o_Photo;  // 44
    int o_PhotoText;  // 48
    int o_AnimauxIcone;  // 52
    int o_AnimauxSelecteur;  // 56
    int o_CadrePhotoSolo;  // 60
    int o_LightForObject;  // 64
    int o_CadrePognes;  // 68
    int o_CadreFile;  // 72
    int o_CadreSound;  // 76
    int o_CadreMap;  // 80
    int o_CadreMission;  // 84
    int o_CadreObject;  // 88
    int o_CadreMails;  // 92
    int o_CadreAlbum;  // 96
    int i_OpeningMode;  // 100
    int i_CineMode;  // 104
    int i_SaveDisable;  // 108
    int i_Reinit;  // 112
    int i_SelObject;  // 116
    int i_SelPerso;  // 120
    int i_SelPersoOld;  // 124
    int i_SelPearl;  // 128
    int i_SelMission;  // 132
    int i_SelPhoto;  // 136
    int i_LastAlbumPhoto;  // 140
    int i_LastMissionPhoto;  // 144
    int i_Exit;  // 148
    int i_AskExit;  // 152
    int i_Allocated;  // 156
    int i_PhotoRefresh;  // 160
    float f_RepeatTime;  // 164
    int i_ForceRefresh;  // 168
    int i_Locked;  // 172
    float f_Timer;  // 176
    int i_Mode;  // 180
    int i_SubMode;  // 184
    int i_ModeValue;  // 188
    int i_JoyBlocage;  // 192
    int i_Technology;  // 196
    int i_Validate;  // 200
    int i_Effect;  // 204
    int i_AreneRestart;  // 208
    int i_OpeningParamInt;  // 212
    float i_OpeningParamFloat;  // 216
    int __ai_MapData_Dimension1;  // 220
    int __ai_MapData_Dimension2;  // 224
    int ai_MapData[4600];  // 228
    int __ai_DonjData_Dimension1;  // 18628
    int __ai_DonjData_Dimension2;  // 18632
    int ai_DonjData[280];  // 18636
    int o_SelCursor;  // 19756
    int o_ButtonNext;  // 19760
    int o_ButtonPrevious;  // 19764
    int o_ButtonNextPerso;  // 19768
    int o_Cursor;  // 19772
    int i_CurPos;  // 19776
    int i_CurCase;  // 19780
    int __ai_Slice2Case_Dimension1;  // 19784
    int ai_Slice2Case[16];  // 19788
    int fct_MoveCursor;  // 19852
    int i_CanSel;  // 19856
    int i_CanSelJoy;  // 19860
    int __ao_Case_Dimension1;  // 19864
    int ao_Case[16];  // 19868
    int __ac_Color_Dimension1;  // 19932
    int ac_Color[7];  // 19936
    int o_Case;  // 19964
    int i_Case;  // 19968
    Vec3 v_Case;  // 19972
    int c_CaseColor;  // 19984
    int i_HasIcon;  // 19988
    int i_Number;  // 19992
    int i_MenuFirst;  // 19996
    int i_MenuSel;  // 20000
    int i_MenuButton;  // 20004
    int i_MenuAction;  // 20008
    int __txt_Menu_Dimension1;  // 20012
    tTextGet txt_Menu[30];  // 20016
    int __i_Menu_Dimension1;  // 20256
    int i_Menu[30];  // 20260
    int __i_MenuNote_Dimension1;  // 20380
    int i_MenuNote[30];  // 20384
    int fct_Menu;  // 20504
    int fct_Buttons;  // 20508
    int i_MenuHistNb;  // 20512
    int __ai_MenuHist_Dimension1;  // 20516
    int ai_MenuHist[10];  // 20520
    int i_UpdateMenu;  // 20560
    int i_UpdateMenu_CurCase;  // 20564
    int i_Back;  // 20568
    float f_ContextTime;  // 20572
    float f_ContextOldTime;  // 20576
    int i_ContextOldCase;  // 20580
    int i_ContextCase;  // 20584
    int i_MailBlinkEffect;  // 20588
    int i_SetLangage;  // 20592
    int i_SetLangageState;  // 20596
    int i_ObjectGivenCase;  // 20600
    int i_ObjectGivenMenu;  // 20604
    int i_ObjectGivenPerso;  // 20608
    int __av_ObjectGivenPos_Dimension1;  // 20612
    Vec3 av_ObjectGivenPos[2];  // 20616
    int i_ObjectGivenResult;  // 20640
    int __ai_ObjectOrder_Dimension1;  // 20644
    int ai_ObjectOrder[50];  // 20648
    int i_PersoOffset;  // 20848
    int i_PersoNumber;  // 20852
    int i_PersoNextBlink;  // 20856
    int __i_PersoId_Dimension1;  // 20860
    int i_PersoId[12];  // 20864
    int __i_PersoAnim_Dimension1;  // 20912
    int i_PersoAnim[12];  // 20916
    int __i_PersoLife_Dimension1;  // 20964
    int i_PersoLife[12];  // 20968
    int __i_PersoLifeMax_Dimension1;  // 21016
    int i_PersoLifeMax[12];  // 21020
    int __ao_RewardObject_Dimension1;  // 21068
    int ao_RewardObject[8];  // 21072
    int o_RewardCurObject;  // 21104
    Vec3 v_RewardRotateAxis;  // 21108
    float f_RewardRotateSpeed;  // 21120
    int __av_PosPhotos_Dimension1;  // 21124
    Vec3 av_PosPhotos[9];  // 21128
    int __av_PosAnimaux_Dimension1;  // 21236
    Vec3 av_PosAnimaux[10];  // 21240
    int i_SelAnimal;  // 21360
    int i_SelAnimalCollect;  // 21364
    int i_SelAnimalRecompense;  // 21368
    int i_AnimalReward;  // 21372
    int i_NoteCur;  // 21376
    int i_NoteGen;  // 21380
    int i_NoteLock;  // 21384
    int i_NoteModif;  // 21388
    float f_NoteTime;  // 21392
    int i_NotePrev;  // 21396
    int i_NoteNext;  // 21400
    int i_NoteLocked;  // 21404
    float f_NoteTimeLeft;  // 21408
    int o_Jauge;  // 21412
    Vec3 v_Jauge_SavePos;  // 21416
    int __af_Jauge_SaveProg_Dimension1;  // 21428
    float af_Jauge_SaveProg[4];  // 21432
    int __af_Jauge_SaveFactor_Dimension1;  // 21448
    float af_Jauge_SaveFactor[4];  // 21452
    float f_Jauge_OldValue;  // 21468
    float f_Jauge_OldMax;  // 21472
    int i_Fric2Add;  // 21476
    int i_Fric2AddInit;  // 21480
    int i_FricWithPerso;  // 21484
    int i_SmallTransfert;  // 21488
    float f_PogneAppearTime;  // 21492
    float f_PogneAppearTimeFixed;  // 21496
    float f_PogneDelay;  // 21500
    float f_PogneAppearTimeInit;  // 21504
    int __i_Bt_Dimension1;  // 21508
    int i_Bt[4];  // 21512
    tTextGet txt_Gen;  // 21528
    tTextGet txt_Object;  // 21536
    tTextGet txt_ObjectInfo;  // 21544
    tTextGet txt_ObjectInfoExt;  // 21552
    tTextGet txt_ObjectSubTitle;  // 21560
    tTextGet txt_MapName;  // 21568
    tTextGet txt_ReportAnimals;  // 21576
    tTextGet txt_ReportGen;  // 21584
    tTextGet txt_Mail;  // 21592
    tTextGet txt_GameNotes;  // 21600
    tTextGet txt_RealMapName;  // 21608
    tTextGet txt_MemCard;  // 21616
    int __o_Object_Dimension1;  // 21624
    int o_Object[50];  // 21628
    int o_Perso;  // 21828
    int o_CurObject;  // 21832
    int i_NoUpdateObject;  // 21836
    int i_ObjMode;  // 21840
    int i_Obj3DType;  // 21844
    int i_Obj3DIndex;  // 21848
    float f_Zoom;  // 21852
    float f_BVSize;  // 21856
    Vec3 v_BVCenter;  // 21860
    Vec3 v_ZOffset;  // 21872
    Vec3 v_ObjectPos;  // 21884
    int i_ObjectVisible;  // 21896
    float f_ObjInfo_Wait;  // 21900
    int i_ObjInfo_PageState;  // 21904
    int i_ObjInfo_Page;  // 21908
    int i_ObjInfo_NextPage;  // 21912
    int i_ObjInfo_PreviousPage;  // 21916
    int __ai_ObjectAppear_Value_Dimension1;  // 21920
    int ai_ObjectAppear_Value[40];  // 21924
    int i_ObjectAppear_CurLine;  // 22084
    int i_ObjectAppear_PrevLine;  // 22088
    float f_ObjectAppear_YUp;  // 22092
    float f_ObjectAppear_YDown;  // 22096
    float f_ObjectAppear_YSpeed;  // 22100
    int i_ObjectAppear_Laser;  // 22104
    int o_ObjectAppear;  // 22108
    int __ai_MDisk_SpecialFlag_Dimension1;  // 22112
    int ai_MDisk_SpecialFlag[14];  // 22116
    int __ai_MDisk_Order_Dimension1;  // 22172
    int ai_MDisk_Order[14];  // 22176
    int i_MDisk_Number;  // 22232
    int i_Obj_Cur;  // 22236
    int i_Obj_Next;  // 22240
    int i_Obj_Prev;  // 22244
    int i_Obj_First;  // 22248
    int i_Obj_Valide;  // 22252
    int i_ObjSwap_State;  // 22256
    float f_ObjSwap_Timer;  // 22260
    int i_ObjSwap_Dir;  // 22264
    int i_ObjSwap_LastDir;  // 22268
    int i_Pearl_First;  // 22272
    int i_Pearl_2Add;  // 22276
    int i_Pearl_Added;  // 22280
    int i_Pearl_Reward;  // 22284
    float f_Var0;  // 22288
    float f_Res0;  // 22292
    int i_NoEffect;  // 22296
    int i_EffectStep;  // 22300
    float f_EffectValue;  // 22304
    int __o_SpiralObj_Dimension1;  // 22308
    int o_SpiralObj[56];  // 22312
    float f_RotateTime;  // 22536
    int i_CurLetter;  // 22540
    int i_NbLetter;  // 22544
    int i_PrevLetter;  // 22548
    int i_SpiralState;  // 22552
    float f_RotateDepth;  // 22556
    int i_SpiralMode;  // 22560
    int i_CodeUsableFirst;  // 22564
    int i_CodeUsableLast;  // 22568
    int __ai_CodeNum_Dimension1;  // 22572
    int ai_CodeNum[30];  // 22576
    int __ai_CodeSF_Dimension1;  // 22696
    int ai_CodeSF[30];  // 22700
    int i_Code;  // 22820
    int i_Rotate0;  // 22824
    int i_Rotate1;  // 22828
    int i_Cheat;  // 22832
    int i_PhotoType;  // 22836
    int i_PhotoFirst;  // 22840
    int i_PhotoNumber;  // 22844
    int i_PhotoMaxNumber;  // 22848
    int i_Send_State;  // 22852
    int i_PhotoChange;  // 22856
    int i_Next_State;  // 22860
    int i_Photo_Cheat;  // 22864
    int i_MailOffset;  // 22868
    int i_MailCur;  // 22872
    int i_MailNext;  // 22876
    int i_MailMore;  // 22880
    int i_MailDir;  // 22884
    int i_CarteCur;  // 22888
    int i_NbNotes;  // 22892
    int __ai_Note_Dimension1;  // 22896
    int ai_Note[10];  // 22900
    int i_IntNotesNb;  // 22940
    int __ai_IntNotes_Dimension1;  // 22944
    int ai_IntNotes[15];  // 22948
    int i_AideDisplayed;  // 23008
    int i_must_zero_joy;  // 23012
    float f_mail_time;  // 23016
    float f_arrow_time;  // 23020
    float f_EtageAppear_Dest;  // 23024
    float f_MapScale;  // 23028
    float f_EtageAppear_Time;  // 23032
    int __af_EtageAppear_Y_Dimension1;  // 23036
    float af_EtageAppear_Y[6];  // 23040
    int i_SndPognes;  // 23064
    int i_SndPognePanelPlayed;  // 23068
    int i_SndProgress;  // 23072
    int i_SndPhoto1;  // 23076
    int i_SndPhoto2;  // 23080
    int i_Table_NbCol;  // 23084
    int i_Table_CurCol;  // 23088
    int i_MsgModeBefore;  // 23092
    int f_MsgFunctionBefore;  // 23096
    int i_MsgPhotoTextStatus;  // 23100
    int i_MsgFlags;  // 23104
    int i_MsgResult;  // 23108
    int i_MsgType;  // 23112
    int i_MsgNote;  // 23116
    int i_MsgWait;  // 23120
    int __av_PosFile_Dimension1;  // 23124
    Vec3 av_PosFile[13];  // 23128
    int i_CurSlot;  // 23284
    int i_Aide_Page;  // 23288
    int i_Aide_LockJoyX;  // 23292
    float f_Aide_LockJoyXTime;  // 23296
    int i_Cheat_Page;  // 23300
    int i_Cheat_GameStatus;  // 23304
    int i_SecundoVoice;  // 23308
    int __ao_GameText_Dimension1;  // 23312
    int ao_GameText[8];  // 23316
    int i_ExternalText_Nb;  // 23348
    int o_ModeEmpty_Obj;  // 23352
    int i_ModeEmpty_Flags;  // 23356
    int fct_Track4;  // 23360
    int i_Track4;  // 23364
    int __ao_Animals_Dimension1;  // 23368
    int ao_Animals[64];  // 23372
    int __ai_AnimalsData_Dimension1;  // 23628
    int ai_AnimalsData[64];  // 23632
    int i_Animals_Nb;  // 23888
    int i_CheckState;  // 23892
    int i_CheckResult;  // 23896
    int i_CheckNextState;  // 23900
    int i_CheckState_OnYes;  // 23904
    int i_CheckState_OnNo;  // 23908
    int i_StateOnCancel;  // 23912
    int i_StateOnRetry;  // 23916
    int MC_slot;  // 23920
    int __MC_button_Dimension1;  // 23924
    int MC_button[8];  // 23928
    int MC_result;  // 23960
    int MC_action;  // 23964
    int MC_Overwrite;  // 23968
    int __ai_Options_Dimension1;  // 23972
    int ai_Options[10];  // 23976
    int i_OptionsCur;  // 24016
    int i_OptionsFirst;  // 24020
    int i_OptionsSave;  // 24024
    float f_OptionsSaveVol;  // 24028
    int i_OptionsSaveModeAudio;  // 24032
    int i_OptionsSaveModeVideo;  // 24036
} tScriptStruct_INVManager;

typedef struct sScriptStruct_INVPhoto {
    int i_Mode;  // 0
    int i_Status;  // 4
    float des_f_width;  // 8
    int o_StringObj;  // 12
    int i_String;  // 16
    int i_StringEfface;  // 20
    int i_EffacePhoto;  // 24
    float f_offset;  // 28
    float f_time;  // 32
} tScriptStruct_INVPhoto;

typedef struct sScriptStruct_Letters {
    int des_i_Mode;  // 0
    int des_txt_File;  // 4
    int des_i_IndexOrCode;  // 8
    int des_i_Wait;  // 12
    int des_i_Restart;  // 16
} tScriptStruct_Letters;

typedef struct sScriptStruct_SFXDisappear2 {
    int i_NbObject;  // 0
    int i_CurObj;  // 4
    int __ao_Object_Dimension1;  // 8
    int ao_Object[8];  // 12
    int __ai_Flags_Dimension1;  // 44
    int ai_Flags[8];  // 48
    int __af_Min_Dimension1;  // 80
    float af_Min[8];  // 84
    int __af_Max_Dimension1;  // 116
    float af_Max[8];  // 120
    int __af_Time1_Dimension1;  // 152
    float af_Time1[8];  // 156
    int __af_Time2_Dimension1;  // 188
    float af_Time2[8];  // 192
    int __af_TimeCur_Dimension1;  // 224
    float af_TimeCur[8];  // 228
    int __af_ZCur_Dimension1;  // 260
    float af_ZCur[8];  // 264
    int __af_Scale_Dimension1;  // 296
    float af_Scale[8];  // 300
    int obj;  // 332
} tScriptStruct_SFXDisappear2;

typedef struct sScriptStruct_SFXFlashTexture {
    int des_i_submat;  // 0
    int des_c_couleur;  // 4
    float des_f_temps_deb;  // 8
    float des_f_temps_fin;  // 12
    float des_f_opacite;  // 16
    int des_i_type_effet;  // 20
    int o_objet_cible;  // 24
    int i_flash_phase1;  // 28
    int i_flash_encours;  // 32
    float f_coeff_courant;  // 36
    int c_couleur_courante;  // 40
    int c_constant_backup;  // 44
} tScriptStruct_SFXFlashTexture;

typedef struct sScriptStruct_SFXXMEN {
    int des_start_actor;  // 0
    int des_start_bone_num;  // 4
    Vec3 des_start_bone_offset;  // 8
    int des_dest_actor;  // 20
    int des_dest_bone_num;  // 24
    Vec3 des_dest_bone_offset;  // 28
    Vec3 des_dest_bone_dir;  // 40
    float des_f_move_delay;  // 52
    float des_f_death_delay;  // 56
    float des_XMEN_width;  // 60
    int des_accelere;  // 64
    float des_delay_min;  // 68
    float des_delay_max;  // 72
    int des_rand_BV_start_pos;  // 76
    float des_digitalisation_delay;  // 80
    float f_time_gen;  // 84
    float f_delay_gen;  // 88
    float f_Z_coef;  // 92
    int i_flag_move_done;  // 96
    int GFX_Halo;  // 100
    float f_time_start;  // 104
    float f_time_death;  // 108
    int o_afterblend_gao;  // 112
    int o_afterrec_gao;  // 116
    int o_bone;  // 120
    int des_sfx_halo;  // 124
} tScriptStruct_SFXXMEN;

typedef struct sScriptStruct_SFXChangeColor {
    int c_OldColor;  // 0
    int c_Color;  // 4
    int i_SubMaterial;  // 8
} tScriptStruct_SFXChangeColor;


typedef struct sScriptStruct_GSTGlobal {
    int o_alarme_perso_detecte;  // 0
    float f_alarme_timer_autooff;  // 4
    float f_alarme_timer_start;  // 8
    float f_reinit_time;  // 12
    float f_time_active_avancement_jeu;  // 16
    Vec3 v_joy_camera;  // 20
    Vec3 v_joy_camera_old;  // 32
    Vec3 v_joy_camera_startblend;  // 44
    Vec3 v_joy_camera_normalized;  // 56
    Vec3 v_joy_camera_normalized_old;  // 68
    Vec3 v_joy_camera_normalized_start;  // 80
    Vec3 v_joy_cam_sight;  // 92
    Vec3 v_joy_cam_sight_old;  // 104
    float f_joy_norm;  // 116
    float f_joy_norm_startblend;  // 120
    float f_joy_norm_old;  // 124
    int i_joy_transition_en_cours;  // 128
    float f_joy_timeref;  // 132
    int o_joy_acteur;  // 136
    int __i_button_SpecialJustPressed_Dimension1;  // 140
    int i_button_SpecialJustPressed[6];  // 144
    int __i_button_SpecialLongPress_Dimension1;  // 168
    int i_button_SpecialLongPress[6];  // 172
    int __f_button_time_Dimension1;  // 196
    float f_button_time[6];  // 200
    int o_look_acteur;  // 224
    int o_pupille_acteur;  // 228
    int o_pupille_acteur_bis;  // 232
    float f_vitesse_suiviregard;  // 236
    int i_tempint;  // 240
    int o_checkpoint;  // 244
    int i_blabla_priorite;  // 248
    int o_acteur_blabla_actif;  // 252
    int i_HlpTxt_Interdit;  // 256
    int i_HlpTxt_Interdit_old;  // 260
    int o_acteur_cine_actif;  // 264
    int i_flash_on;  // 268
    float f_flash_time;  // 272
    int o_objet_flash;  // 276
    int __o_objet_FX_Dimension1;  // 280
    int o_objet_FX[20];  // 284
    int o_board_panel;  // 364
    int i_snd_global_priority;  // 368
    int o_snd_global_priority_object;  // 372
    int __v_event_sound_pos_Dimension1;  // 376
    Vec3 v_event_sound_pos[20];  // 380
    int __f_event_sound_size_Dimension1;  // 620
    float f_event_sound_size[20];  // 624
    int __f_event_sound_time_Dimension1;  // 704
    float f_event_sound_time[20];  // 708
    int __o_event_sound_actor_Dimension1;  // 788
    int o_event_sound_actor[20];  // 792
    int i_event_sound_nbr;  // 872
    int __i_event_sound_id_Dimension1;  // 876
    int i_event_sound_id[20];  // 880
    int i_event_sound_next_id;  // 960
    int __o_targets_Dimension1;  // 964
    int o_targets[100];  // 968
    int __v_targets_offset_Dimension1;  // 1368
    Vec3 v_targets_offset[100];  // 1372
    int i_targets_nb;  // 2572
    int o_locking_target;  // 2576
    int __o_selected_target_Dimension1;  // 2580
    int o_selected_target[10];  // 2584
    int __v_selected_target_offset_Dimension1;  // 2624
    Vec3 v_selected_target_offset[10];  // 2628
    int i_selected_target_nb;  // 2748
    Vec3 v_reticule_visee;  // 2752
    int i_reticule_on;  // 2764
    int o_eau_acteur;  // 2768
    float f_eau_hauteurZ;  // 2772
    int o_ciel_acteur;  // 2776
    float f_ciel_hauteurZ;  // 2780
    int __Planetes_Rotations_Dimension1;  // 2784
    float Planetes_Rotations[50];  // 2788
    float f_Hour_Set;  // 2988
    int i_mise_a_jour_des_SF;  // 2992
    int i_compagnon_qui_parle;  // 2996
    int __f_mat_resistance_Dimension1;  // 3000
    float f_mat_resistance[10];  // 3004
    int o_cur_activator;  // 3044
    int i_optim_nb_dialogue;  // 3048
    int i_optim_nb_avancement;  // 3052
    int __ao_optim_dialogue_Dimension1;  // 3056
    int ao_optim_dialogue[50];  // 3060
    float f_fade_start_time;  // 3260
    float f_fade_delay;  // 3264
    int i_fade_priority;  // 3268
    int i_mort_en_cours;  // 3272
    int i_mort_en_cours_old;  // 3276
    int i_CheckpointDesable;  // 3280
    float f_vol_mort;  // 3284
    int i_Perso_ID_a_checker;  // 3288
    int i_map_virtuelle;  // 3292
    int des_i_numplanete;  // 3296
    int i_map_just_reset;  // 3300
    int i_nb_compagnon_en_suivi;  // 3304
    int i_nb_compagnon_en_suivi_old;  // 3308
    int i_id_compagnon_en_suivi;  // 3312
    int i_id_compagnon_en_suivi_old;  // 3316
    int __i_snd_mm_instance_Dimension1;  // 3320
    int i_snd_mm_instance[4];  // 3324
    int __f_snd_mm_volume_ref_Dimension1;  // 3340
    float f_snd_mm_volume_ref[4];  // 3344
    int __f_snd_mm_volume_tgt_Dimension1;  // 3360
    float f_snd_mm_volume_tgt[4];  // 3364
    int __f_snd_mm_volume_cur_Dimension1;  // 3380
    float f_snd_mm_volume_cur[4];  // 3384
    int __f_snd_mm_vitessefade_Dimension1;  // 3400
    float f_snd_mm_vitessefade[4];  // 3404
    int i_ambiance_courante;  // 3420
    int o_ambiance_acteur;  // 3424
    int i_nb_sfx_disp_addmat_users;  // 3428
    int i_nb_sfx_disp_revelateur_user;  // 3432
    int c_fade_color_1;  // 3436
    int c_fade_color_2;  // 3440
    float f_enterprise_speed_max;  // 3444
    Vec3 v_alarme_pos;  // 3448
    int __o_musique_fight_list_Dimension1;  // 3460
    int o_musique_fight_list[20];  // 3464
    int i_fight_action;  // 3544
    float f_game_speed;  // 3548
    float f_cheat_speed;  // 3552
    float f_old_game_speed;  // 3556
    float f_old_old_game_speed;  // 3560
    int i_projectile_ami_nbr;  // 3564
    int __oa_projectile_ami_Dimension1;  // 3568
    int oa_projectile_ami[10];  // 3572
    float f_dist_mine;  // 3612
    int __o_actor_texte_Dimension1;  // 3616
    int o_actor_texte[5];  // 3620
    int __o_tesla_recepteur_Dimension1;  // 3640
    int o_tesla_recepteur[20];  // 3644
    int i_nb_tesla_recepteurs;  // 3724
    tMessage m_msg;  // 3728
    float f_time_reinit;  // 3780
    int __ao_list_of_modelooktarget_Dimension1;  // 3784
    int ao_list_of_modelooktarget[50];  // 3788
    int i_nbr_of_modelooktarget;  // 3988
    int __v_Perso_ID_actor_stockage_pos_Dimension1;  // 3992
    Vec3 v_Perso_ID_actor_stockage_pos[12];  // 3996
    int __v_Perso_ID_actor_stockage_sig_Dimension1;  // 4140
    Vec3 v_Perso_ID_actor_stockage_sig[12];  // 4144
    int i_nb_materia_around_sally;  // 4288
    float f_last_materia;  // 4292
    int __ao_actors_pos_Dimension1;  // 4296
    Vec3 ao_actors_pos[12];  // 4300
    int __ao_actors_sight_Dimension1;  // 4444
    Vec3 ao_actors_sight[12];  // 4448
    int i_mode_matrix;  // 4592
    int i_mode_matrix_old;  // 4596
    int o_mode_matrix_acteur;  // 4600
    int o_mode_uppercuted_acteur;  // 4604
    int i_mode_matrix_aborted;  // 4608
    int o_dernier_garde_en_alarme;  // 4612
    int o_dernier_garde_en_recherche;  // 4616
    float f_sally_dernier_paf;  // 4620
    int o_sally_baton_intro;  // 4624
    Vec3 v_joy_camera_pour_blend;  // 4628
    int i_nb_SFX_Explosion_Fire_SND;  // 4640
    int i_Domz_Attack_Boat_Shoot_NB;  // 4644
    int o_micro;  // 4648
    int i_sens;  // 4652
    int __ao_vaisseau_pote_Dimension1;  // 4656
    int ao_vaisseau_pote[50];  // 4660
    int i_nb_vaisseau_pote;  // 4860
    int i_freeze_joy;  // 4864
    float f_crochax_DT;  // 4868
    int i_exit_the_map;  // 4872
    int o_compagnon_talk;  // 4876
    int o_aircraft_col;  // 4880
    int i_nb_materia_on;  // 4884
    float f_joy_min;  // 4888
    int i_disable_hysteresis;  // 4892
    float time_screen_saver;  // 4896
} tScriptStruct_GSTGlobal;

typedef struct sScriptStruct_GSTBoardPanel {
    int __ao_listofobjects_Dimension1;  // 0
    int ao_listofobjects[10];  // 4
    int __ai_listofbutton_Dimension1;  // 44
    int ai_listofbutton[10];  // 48
    int __ai_listofpriority_Dimension1;  // 88
    int ai_listofpriority[10];  // 92
    int __ai_action_Dimension1;  // 132
    int ai_action[10];  // 136
    int __ao_listoftargets_Dimension1;  // 176
    int ao_listoftargets[10];  // 180
    int __ao_targets_Dimension1;  // 220
    int ao_targets[20];  // 224
    int __af_targetsoffsets_Dimension1;  // 304
    float af_targetsoffsets[20];  // 308
    int o_sally_contextuel;  // 388
    int o_sally_contextuel_old;  // 392
    float i_sally_priority;  // 396
    Vec3 v_sally_offset;  // 400
    int i_sally_action;  // 412
    int i_sally_priority_old;  // 416
    int i_current_nb_of_object;  // 420
    int i_current_nb_of_target;  // 424
    int i_old_nb_of_object;  // 428
    int o_saut_object;  // 432
    int o_saut_object_old;  // 436
    int i_saut_prio;  // 440
    int i_saut_action;  // 444
    int i_objet_inventaire;  // 448
    int i_objet_inventaire_prec;  // 452
    int i_objet_inventaire_suiv;  // 456
    int i_objet_inventaire_prec_old;  // 460
    int i_objet_inventaire_suiv_old;  // 464
    int i_objet_inventaire_old;  // 468
    int o_principal_object;  // 472
    int o_contextuel_object;  // 476
    int o_contextuel_target;  // 480
    int i_contextuel_prio;  // 484
    int o_use_object;  // 488
    int o_use_object_old;  // 492
    int i_use_prio;  // 496
    int i_use_action;  // 500
    float f_use_offset;  // 504
    int i_action_selected;  // 508
    int o_contextuel_saut_object;  // 512
    int i_current_mode;  // 516
    int i_old_mode;  // 520
    int i_init_look;  // 524
    int i_init_inventaire;  // 528
    int i_init_enterprise;  // 532
    int i_init_aircraft;  // 536
    int f_old_mode;  // 540
    int __ai_ind_dialog_phr_Dimension1;  // 544
    int ai_ind_dialog_phr[5];  // 548
    int i_ind_pointeur;  // 568
    Vec3 v_fight_target_vision;  // 572
    int i_fight_index_NMI;  // 584
    float f_fight_time_jauge_NMI;  // 588
    float f_fight_time_jauge_NMI_old;  // 592
    int o_evil_jauge;  // 596
    int o_look_object;  // 600
    int o_look_object_old;  // 604
    int __ao_look_object_mission_Dimension1;  // 608
    int ao_look_object_mission[10];  // 612
    int __ai_look_validite_mission_Dimension1;  // 652
    int ai_look_validite_mission[20];  // 656
    int i_look_object_nb;  // 736
    Vec3 v_look_jeter_destination;  // 740
    Vec3 v_look_pos_center;  // 752
    int i_look_aircraft;  // 764
    int i_look_enterprise;  // 768
    int i_look_aircraft_photo;  // 772
    int i_look_enterprise_photo;  // 776
    int i_look_info_on;  // 780
    int i_look_PhotoIndex;  // 784
    int i_look_photo_border;  // 788
    float f_look_time_flash;  // 792
    float f_look_time_info;  // 796
    int i_look_loading;  // 800
    Vec3 v_look_before_inv;  // 804
    int i_look_jauge_load_on;  // 816
    float f_time_en_mode_look;  // 820
    float f_time_hors_mode_look;  // 824
    int i_exit_look;  // 828
    float f_time_entre_look;  // 832
    int i_look_tir_freesby;  // 836
    int i_look_objet_mission;  // 840
    int i_look_photo_affiche_info;  // 844
    float f_time_photo_prise;  // 848
    int i_look_start_fade_on;  // 852
    int i_look_start_fade_off;  // 856
    int i_look_object_valide;  // 860
    int i_look_photo_text_affiche;  // 864
    int i_look_mode_toggle;  // 868
    float f_look_prog_viseur;  // 872
    float f_look_prog_cadrage;  // 876
    float f_look_u_jauge;  // 880
    float f_look_time_change_target;  // 884
    float f_look_morph_apparition;  // 888
    int i_look_photo_target;  // 892
    int __objet_liste_gao_Dimension1;  // 896
    int objet_liste_gao[50];  // 900
    int o_dialogue_actor;  // 1100
    int __at_dialog_questions_Dimension1;  // 1104
    tTextGet at_dialog_questions[20];  // 1108
    int __at_dialog_reponses_Dimension1;  // 1268
    tTextGet at_dialog_reponses[20];  // 1272
    int __ai_dialog_consequences_Dimension1;  // 1432
    int ai_dialog_consequences[20];  // 1436
    int __ai_dialog_grisage_Dimension1;  // 1516
    int ai_dialog_grisage[20];  // 1520
    int i_dialog_act_pos;  // 1600
    int i_dialog_phase;  // 1604
    int i_dialog_init_choix;  // 1608
    int i_dialog_ind_questions_max;  // 1612
    int i_dialog_page_actu;  // 1616
    int i_dialog_page_nb;  // 1620
    int i_dialog_1_question;  // 1624
    int o_acteur_pointe;  // 1628
    int i_acteur_pointe_prio;  // 1632
    float f_acteur_pointe_offset;  // 1636
    int i_acteur_pointe_button;  // 1640
    int i_acteur_pointe_text;  // 1644
    int o_acteur_pointe_target;  // 1648
    int i_acteur_pointe_visible;  // 1652
    int i_acteur_pointe_new;  // 1656
    int i_swap_button_sens;  // 1660
    int i_swap_button_used;  // 1664
    int i_swap_button_1tramebis;  // 1668
    float f_swap_button_time;  // 1672
    float f_swap_button_DT;  // 1676
    int i_swap_button_1trame;  // 1680
    float f_swap_button_time_press;  // 1684
    int i_swap_button_pos;  // 1688
    int i_swap_nb_obj_hide;  // 1692
    float Interface_Time_fade_OFF_C;  // 1696
    float Interface_Time_fade_OFF_T;  // 1700
    float Interface_Time_fade_OFF_O;  // 1704
    float Interface_Time_fade_OFF_X;  // 1708
    int Interface_Afficher_C;  // 1712
    int Interface_Afficher_T;  // 1716
    int Interface_Afficher_O;  // 1720
    int Interface_Afficher_X;  // 1724
    int i_button_swap_fadeX;  // 1728
    int i_button_swap_fadeO;  // 1732
    int i_button_swap_fadeC;  // 1736
    int i_button_swap_fadeT;  // 1740
    int i_button_swap_alphaX;  // 1744
    int i_button_swap_alphaO;  // 1748
    int i_button_swap_alphaC;  // 1752
    int i_button_swap_alphaT;  // 1756
    int Boutonces_CurInv;  // 1760
    int Boutonces_NbInv;  // 1764
    int __Boutonces_Inv2Char_Dimension1;  // 1768
    int Boutonces_Inv2Char[50];  // 1772
    int Boutonces_ActionNeeded;  // 1972
    int Boutonces_CurAction;  // 1976
    int Boutonces_OldAction;  // 1980
    int Boutonces_ActionBeforeEffect;  // 1984
    int Boutonces_FriendActionNeeded;  // 1988
    int Boutonces_FriendCurAction;  // 1992
    int Boutonces_FriendBeforeEffect;  // 1996
    int Boutonces_UseActionNeeded;  // 2000
    int Boutonces_UseCurAction;  // 2004
    int Boutonces_UseBeforeEffect;  // 2008
    int Boutonces_JumpCurAction;  // 2012
    int Boutonces_JumpOldAction;  // 2016
    int Boutonces_JumpBeforeEffect;  // 2020
    int __i_joje_fade_Dimension1;  // 2024
    int i_joje_fade[6];  // 2028
    int __f_joje_fade_time_Dimension1;  // 2052
    float f_joje_fade_time[6];  // 2056
    int i_new_action;  // 2080
    int i_new_action_use;  // 2084
    int __i_time_refresh_color_Dimension1;  // 2088
    int i_time_refresh_color[3];  // 2092
    int __f_time_chiant_Dimension1;  // 2104
    float f_time_chiant[3];  // 2108
    int __f_time_refresh_color_Dimension1;  // 2120
    float f_time_refresh_color[3];  // 2124
    float f_time_init;  // 2136
    float f_time_on_sort_de_linventaire;  // 2140
    float f_time_INTF_on_sort_de_linven;  // 2144
    int i_MainActor_ID;  // 2148
    int i_MainActor_ID_old;  // 2152
    int i_CompActor_ID;  // 2156
    int __Perso_ID_Sprite_Dimension1;  // 2160
    int Perso_ID_Sprite[12];  // 2164
    int i_commerce_tune;  // 2212
    int i_commerce_perle;  // 2216
    int i_commerce_tune_old;  // 2220
    int i_nb_perle;  // 2224
    float f_time_perle;  // 2228
    float f_time_tune;  // 2232
    int i_commercer_tune_deja_affiche;  // 2236
    int i_commerce_tune_change;  // 2240
    float f_time_commerce;  // 2244
    int i_commerce_non_vendeur;  // 2248
    int i_commercer_perle_deja_affich;  // 2252
    int i_commerce_perle_change;  // 2256
    int Gen_Show;  // 2260
    int Gen_OldShow;  // 2264
    int Gen_SaveShow;  // 2268
    float Gen_Time;  // 2272
    int Gen_ForceShow;  // 2276
    int i_inv_in;  // 2280
    int i_Obj_On_Radar_Nb;  // 2284
    int __ao_Obj_On_Radar_Dimension1;  // 2288
    int ao_Obj_On_Radar[20];  // 2292
    int __at_Obj_On_Radar_Desc_Dimension1;  // 2372
    tTextGet at_Obj_On_Radar_Desc[20];  // 2376
    int i_Obj_On_Radar_Display;  // 2536
    float f_angle_Radar_Display;  // 2540
    Vec3 v_sight_RadarDisplay;  // 2544
    int __at_actions_Dimension1;  // 2556
    tTextGet at_actions[38];  // 2560
    int __at_photo_Dimension1;  // 2864
    tTextGet at_photo[3];  // 2868
    float f_time_superaction;  // 2892
    int I_delai_superaction;  // 2896
    int i_done_superaction;  // 2900
    int i_load_init;  // 2904
    int i_indice_texte_synchro;  // 2908
    Vec3 v_pos_viseur;  // 2912
    float f_alfa_viseur;  // 2924
    int __ao_selected_target_Dimension1;  // 2928
    int ao_selected_target[10];  // 2932
    int i_selected_target_nb;  // 2972
    int i_fast_look;  // 2976
    int i_swap_auto;  // 2980
    float f_time_swap_auto;  // 2984
    int i_compte_a_rebourd;  // 2988
    int i_PhotoCheat_CurLine;  // 2992
    int i_alpha_bad_photoi_mana_add;  // 2996
    float f_alpha_bad_photo;  // 3000
    int i_photo_blok_sorti_look;  // 3004
    int i_texte_vala;  // 3008
    int i_texte_synchro_SAFight;  // 3012
    int mode_matrix_on;  // 3016
    float mode_matrix_f_speed;  // 3020
    float mode_matrix_f_start;  // 3024
    Vec3 mode_matrix_v_pos_viseur;  // 3028
    Vec3 mode_matrix_v_sight_viseur;  // 3040
    float f_matrix_duree;  // 3052
    float f_matrix_joy;  // 3056
    int i_cpt;  // 3060
    int o_map_manager;  // 3064
    int i_ambiant;  // 3068
    int i_ambiant2;  // 3072
    float f_life_main;  // 3076
    float f_life_main_old;  // 3080
    float f_life_main_max;  // 3084
    float f_life_main_alfa;  // 3088
    float f_life_main_time_cran_change;  // 3092
    float f_life_main_time_cran_on;  // 3096
    float f_life_main_life_blend;  // 3100
    int i_life_main_effect_finish;  // 3104
    float f_life_main_life_blend_delai;  // 3108
    float f_life_main_delai_vis;  // 3112
    float f_life_main_color;  // 3116
    int i_life_main;  // 3120
    float f_life_main_delai_ondule_vie;  // 3124
    float f_life_comp;  // 3128
    float f_life_comp_old;  // 3132
    float f_life_comp_max;  // 3136
    float f_life_comp_alfa;  // 3140
    float f_life_comp_time_cran_change;  // 3144
    float f_life_comp_time_cran_on;  // 3148
    float f_life_comp_life_blend;  // 3152
    int i_life_comp_effect_finish;  // 3156
    float f_life_comp_life_blend_delai;  // 3160
    float f_life_comp_delai_vis;  // 3164
    float f_life_comp_color;  // 3168
    int i_life_comp;  // 3172
    float f_life_comp_delai_ondule_vie;  // 3176
    int i_life_min_nb_bip;  // 3180
    float f_life_time_bip;  // 3184
    int i_life_min_nb_bip2;  // 3188
    float f_life_time_bip2;  // 3192
    Vec3 v_size_effet_matrix;  // 3196
    int i_var_sorti_inv_de_merde_pour;  // 3208
    int i_snd_matrix;  // 3212
    int i_bip_off;  // 3216
    int __ao_INTF_lock_target_Dimension1;  // 3220
    int ao_INTF_lock_target[10];  // 3224
    int i_INTF_lock_target_nb;  // 3264
    int i_affiche_encart_photo;  // 3268
    float f_color_blend_viseur;  // 3272
    float f_color_blend_reticule;  // 3276
    float f_color_blend_photo_0;  // 3280
    float f_INTF_alfa;  // 3284
    float f_INTF_time_08;  // 3288
    int i_force_interface;  // 3292
    float f_time_chrono_before_effect;  // 3296
    Vec3 v_pos_chrono;  // 3300
    int i_snd_loopchrono1;  // 3312
    int i_snd_loopchrono2;  // 3316
    int i_old_sec;  // 3320
    float f_snd_loopchrono_vol;  // 3324
    float f_time_txt_on;  // 3328
    float f_time_mode_look_end;  // 3332
    int i_TXT_info_ON;  // 3336
    tTextGet txt_ReportAnimals;  // 3340
    int i_fuck_goyo_blindage;  // 3348
    float f_time_life_max_main;  // 3352
    int i_snd_vendeur_loop;  // 3356
    Vec3 v_pos_locking_target;  // 3360
    float sfx_MBlur;  // 3372
    float sfx_punch;  // 3376
    float sfx_ZSmooth;  // 3380
    float sfx_REM_Intensity;  // 3384
} tScriptStruct_GSTBoardPanel;

typedef struct sScriptStruct_GSTMapManager {
    int i_Ambiant;  // 0
    int i_Ambiant2;  // 4
    int des_numero_de_l_entree;  // 8
    int des_triche_peyj_status;  // 12
    int des_triche_peyj_position;  // 16
    int des_triche_mili_status;  // 20
    int des_triche_mili_position;  // 24
    int des_triche_domz_status;  // 28
    int des_triche_domz_position;  // 32
    int des_triche_xb25_status;  // 36
    int des_triche_xb25_position;  // 40
    int des_triche_xb26_status;  // 44
    int des_triche_xb26_position;  // 48
    int des_triche_xb27_status;  // 52
    int des_triche_xb27_position;  // 56
    int des_triche_ent_status;  // 60
    int des_triche_ent_passengers;  // 64
    int des_triche_ent_dock;  // 68
    int des_triche_aircraft_status;  // 72
    int des_triche_aircraft_passenger;  // 76
    int des_triche_aircraft_dock;  // 80
    int SL_Use_SL;  // 84
    int SL_triche_peyj_status;  // 88
    int SL_triche_peyj_position;  // 92
    int SL_triche_mili_status;  // 96
    int SL_triche_mili_position;  // 100
    int SL_triche_domz_status;  // 104
    int SL_triche_domz_position;  // 108
    int SL_triche_xb25_status;  // 112
    int SL_triche_xb25_position;  // 116
    int SL_triche_xb26_status;  // 120
    int SL_triche_xb26_position;  // 124
    int SL_triche_xb27_status;  // 128
    int SL_triche_xb27_position;  // 132
    int SL_triche_ent_status;  // 136
    int SL_triche_ent_passengers;  // 140
    int SL_triche_ent_dock;  // 144
    int SL_triche_aircraft_status;  // 148
    int SL_triche_aircraft_passengers;  // 152
    int SL_triche_aircraft_dock;  // 156
    int des_perso_qui_centre_la_grill;  // 160
    int des_acteur_eau;  // 164
    int des_acteur_ciel;  // 168
    float des_f_enterprise_speed_max;  // 172
    float f_time_reinit;  // 176
    float f_engine_speed;  // 180
    int __des_SF_a_mettre_a_0_Dimension1;  // 184
    int des_SF_a_mettre_a_0[10];  // 188
    int __des_SF_a_mettre_a_1_Dimension1;  // 228
    int des_SF_a_mettre_a_1[10];  // 232
    int __des_SF_a_mettre_a_0r_Dimension1;  // 272
    int des_SF_a_mettre_a_0r[4];  // 276
    int __des_SF_a_mettre_a_1r_Dimension1;  // 292
    int des_SF_a_mettre_a_1r[4];  // 296
    int __o_PLAN_liste_des_zones_Dimension1;  // 312
    int o_PLAN_liste_des_zones[31];  // 316
    int __i_PLAN_liste_des_SF_Dimension1;  // 440
    int i_PLAN_liste_des_SF[31];  // 444
    int __i_PLAN_liste_des_SF_sup_Dimension1;  // 568
    int i_PLAN_liste_des_SF_sup[31];  // 572
    float sfx_brightness;  // 696
    float sfx_contrast;  // 700
    int sfx_DB;  // 704
    int sfx_DB_ZNear;  // 708
    int sfx_DB_ZFar;  // 712
    float sfx_DB_Intensity;  // 716
    int sfx_SC;  // 720
    float sfx_SC_Intensity;  // 724
    float sfx_SC_spectre;  // 728
    int sfx_R;  // 732
    float sfx_R_Intensity;  // 736
    int sfx_F;  // 740
    int sfx_F_mode;  // 744
    float sfx_Fog_near;  // 748
    float sfx_Fog_far;  // 752
    int sfx_WR;  // 756
    float sfx_WR_ZHeight;  // 760
    float sfx_WR_ZPlane;  // 764
    int i_use_music_fight_manager;  // 768
    int o_musique_fight;  // 772
    int __o_objet_colmap_Dimension1;  // 776
    int o_objet_colmap[15];  // 780
    int __i_sound_index_Dimension1;  // 840
    int i_sound_index[15];  // 844
    int SND_FX_Mode;  // 904
    float SND_FX_Pourcentage;  // 908
    int SND_FX_Delay;  // 912
    int SND_FX_Feedback;  // 916
    float SND_FX_Doppler;  // 920
    int SND_stereo_souffle;  // 924
    int WTR_activation;  // 928
    int __WTR_Zone_Dimension1;  // 932
    int WTR_Zone[5];  // 936
    int WTR_sky_color;  // 956
    int WTR_bottom_color;  // 960
    int WTR_water_color;  // 964
    float WTR_Waves_Height;  // 968
    float WTR_Choppy;  // 972
    float WTR_Tiling;  // 976
    float WTR_Speed;  // 980
    float WTR_Water_Height;  // 984
    float WTR_Wave_dir;  // 988
    float WTR_Wave_Prop;  // 992
    int o_sally_baton_intro;  // 996
    int i_WTR_nb_zones;  // 1000
    float f_WTR_Waves_Height;  // 1004
    int __wp_gridpos_Dimension1;  // 1008
    int __wp_gridpos_Dimension2;  // 1012
    int wp_gridpos[20];  // 1016
} tScriptStruct_GSTMapManager;

typedef struct sScriptStruct_ESAuSol {
    int SF_validation;  // 0
    int SF_destruction_apres_ES;  // 4
    int des_mode_checkpoint;  // 8
    int i_destroy_after_checkpoint;  // 12
    int des_mode_mdisk;  // 16
    int i_auto_reactivate_checkpoint_;  // 20
    int i_lance_checkpoint_meme_en_fi;  // 24
    int i_SF_a_valider_apres_CP;  // 28
    int des_mode_external_validation;  // 32
    int des_mode_mdisk_put_here;  // 36
    int des_mode_mdisk_regarde_la;  // 40
    int des_i_interdire_en_fight;  // 44
    int des_mode_game;  // 48
    float f_mode_game_distance_active;  // 52
    int i_force_change_status;  // 56
    int i_Collision_Pivot;  // 60
    int des_o_perso_detect;  // 64
    int o_mdf_son1;  // 68
    int o_mdf_son2;  // 72
    int o_mdf_son3;  // 76
    int des_je_fais_entree_de_map;  // 80
    int des_numero_de_l_entree;  // 84
    int des_Carte_Etage;  // 88
    int des_ES_HH_malade;  // 92
    int des_sally;  // 96
    int des_pos_sally;  // 100
    int des_vers_gao;  // 104
    int des_pos_camera;  // 108
    int des_pos_target;  // 112
    int des_militaire;  // 116
    Vec3 des_offset_militaire;  // 120
    int des_pos_militaire;  // 132
    int des_peyj;  // 136
    Vec3 des_offset_peyj;  // 140
    int des_pos_peyj;  // 152
    int des_domz;  // 156
    Vec3 des_offset_domz;  // 160
    int des_pos_domz;  // 172
    int des_xb25;  // 176
    Vec3 des_offset_xb25;  // 180
    int des_pos_xb25;  // 192
    int des_xb26;  // 196
    Vec3 des_offset_xb26;  // 200
    int des_pos_xb26;  // 212
    int des_xb27;  // 216
    Vec3 des_offset_xb27;  // 220
    int des_pos_xb27;  // 232
    int des_enterprise;  // 236
    Vec3 des_offset_enterprise;  // 240
    int des_pos_enterprise;  // 252
    int des_aircraft;  // 256
    Vec3 des_offset_aircraft;  // 260
    int des_pos_aircraft;  // 272
    int __i_SF_a_Changer_Dimension1;  // 276
    int i_SF_a_Changer[20];  // 280
    float f_force_hour;  // 360
    float f_force_hour_release;  // 364
    int des_entree_cine_auto;  // 368
    int des_targetdeplacement_entree_;  // 372
    float des_distance_je_rend_le_contr;  // 376
    float des_force_du_joy_init;  // 380
    int des_je_fais_aussi_sortie_de_m;  // 384
    float des_distance_je_change_de_map;  // 388
    int des_vers_map_numero;  // 392
    int des_vers_entree_numero;  // 396
    float des_taille_plaque_sortie;  // 400
    float des_f_fade_length;  // 404
    int des_i_fade_submaterial;  // 408
    float des_f_fade_start_alpha;  // 412
    float des_f_fade_end_alpha;  // 416
    int des_i_fade_displayname;  // 420
    int des_i_SF_sortie_bis;  // 424
    int des_vers_map_numero_bis;  // 428
    int des_i_SF_sortie_ter;  // 432
    int des_vers_map_numero_ter;  // 436
    int __des_i_SF_a_Changer_Dimension1;  // 440
    int des_i_SF_a_Changer[20];  // 444
    int des_sortie_cine_auto;  // 524
    int des_targetdeplacement_sortie_;  // 528
    int des_o_course_manager;  // 532
    float f_old_y;  // 536
    int i_fin;  // 540
    int i_objet_selectionne;  // 544
    int i_objet_mdisk_map;  // 548
    int i_reflex_prio_use;  // 552
    int i_Sally_status;  // 556
    int i_Sally_Monte_dans_quel_vehic;  // 560
    int i_Peyj_status;  // 564
    int i_Peyj_map;  // 568
    Vec3 v_Peyj_pos;  // 572
    Vec3 v_Peyj_sight;  // 584
    int i_Milit_status;  // 596
    int i_Milit_map;  // 600
    Vec3 v_Milit_pos;  // 604
    Vec3 v_Milit_sight;  // 616
    int i_Aircraft_status;  // 628
    int i_Aircraft_map;  // 632
    Vec3 v_Aircraft_pos;  // 636
    Vec3 v_Aircraft_sight;  // 648
    int i_Enterprise_status;  // 660
    int i_Enterprise_map;  // 664
    Vec3 v_Enterprise_pos;  // 668
    Vec3 v_Enterprise_sight;  // 680
    int Je_Suis_1_Entree_Radar;  // 692
    int SF_Entree_Connue;  // 696
    tTextGet Nom_De_L_Entree_Radar;  // 700
    int i_reflex_prio_contextuel;  // 708
    int i_map_id;  // 712
    int i_get_map_id;  // 716
    int i_must_checkpoint;  // 720
    int i_comp_loop;  // 724
    float f_time_blindage;  // 728
    int i_play_snk_recup_disk;  // 732
    int i_play_snk_insert_disk;  // 736
    int i_flag_dont_do_all_stuff;  // 740
    int i_change_item;  // 744
} tScriptStruct_ESAuSol;

typedef struct sScriptStruct_PJCamera {
    float des_f_distance;  // 0
    float des_f_angle;  // 4
    float f_dist_enterprise;  // 8
    float f_dist_enterprise_max;  // 12
    int des_o_followed_actor;  // 16
    float f_coef_translation_speed;  // 20
    float f_cam_inertie;  // 24
    float cam_i_priority;  // 28
    float cam_i_old_priority;  // 32
    float f_focal_curr;  // 36
    float f_focal_gc;  // 40
    float f_focal_old;  // 44
    float f_focal_diff;  // 48
    int o_gencam_mario_courant;  // 52
    int o_gencam_mario;  // 56
    Vec3 v_sfx_vec1;  // 60
    Vec3 v_sfx_vec2;  // 72
    int i_sfx_type;  // 84
    float f_sfx_flo1;  // 88
    int current_camera_mode;  // 92
    int old_camera_mode;  // 96
    int End_mode;  // 100
    float f_distance_cam;  // 104
    float coef_inclinaison_blend;  // 108
    Vec3 v_ideal_axis;  // 112
    float f_ideal_Z;  // 124
    float f_AngleZ;  // 128
    float f_ideal_dist;  // 132
    int i_proj_togen;  // 136
    float f_speed_trans;  // 140
    int i_must_cut;  // 144
    int o_followed_actor;  // 148
    int i_test_vis;  // 152
    float f_inertie;  // 156
    float f_translation_speed;  // 160
    Vec3 v_pos_normale;  // 164
    int o_perso_fight;  // 176
    int o_cine_objet_dest;  // 180
    int o_cine_objet_cible;  // 184
    Vec3 v_cine_pos_dest;  // 188
    Vec3 v_cine_offset_cible;  // 200
    Vec3 v_speed_old;  // 212
    int i_cine_mode;  // 224
    float f_focal_cine;  // 228
    float f_cine_delai_trans;  // 232
    float f_cine_delai_focal;  // 236
    float f_temps;  // 240
    float f_coeff1;  // 244
    int i_canuserctrl;  // 248
    int i_canusercut;  // 252
    int i_usertomario;  // 256
    int o_lockedgen;  // 260
    int i_lockedtotal;  // 264
    int i_lockedaxis;  // 268
    int i_cutextern;  // 272
    float f_dist_cut;  // 276
    int o_speed_obj;  // 280
    float f_factor_sight;  // 284
    float f_coeff;  // 288
    int i_notest_dodge;  // 292
    float f_time_vis;  // 296
    int i_echec_vis;  // 300
    int o_last_mur_devant;  // 304
    Vec3 v_last_move;  // 308
    float f_time_turn;  // 320
    Vec3 v_last_normal_sol;  // 324
    Vec3 v_last_pos;  // 336
    Vec3 v_cur_dir;  // 348
    float des_i_spycam;  // 360
    float des_f_spy_left;  // 364
    float des_f_spy_right;  // 368
    float des_f_spy_up;  // 372
    float des_f_spy_down;  // 376
    int des_i_spy_autocut;  // 380
    float f_time_spy;  // 384
    Vec3 v_last_joy;  // 388
    int i_can_cut_spy;  // 400
    int i_has_cut_extern;  // 404
    float f_add_z;  // 408
    int i_from_look;  // 412
    Vec3 v_pad1;  // 416
    int i_corner;  // 428
    int i_test_ground;  // 432
    int forcelock;  // 436
    int des_i_dynplan;  // 440
    float des_f_timedynplan;  // 444
    Vec3 des_v_offsetviseedynplan;  // 448
    float des_f_focaldynplan;  // 460
    int des_i_contrechampdynplan;  // 464
    int des_o_viseedynplan;  // 468
    float des_f_zdynplan;  // 472
    float des_f_distdynplan;  // 476
    Vec3 des_v_sightdynplan;  // 480
    int des_i_dynplanclickafter;  // 492
    int des_i_dynplanclickaftersight;  // 496
    Vec3 des_v_dynplanvisee;  // 500
    int des_i_dynplanviseedyn;  // 512
    int des_i_can_dynplan;  // 516
    Vec3 des_v_dynplanclickafter;  // 520
    int i_lastdynplan;  // 532
    float f_timedynplan;  // 536
    Vec3 v_posdynplan;  // 540
    int i_zoom_morfle_allowed;  // 552
    int i_zoom_talk_allowed;  // 556
    int i_flag_joy_cam_X_released;  // 560
    int i_flag_joy_cam_Y_released;  // 564
    int i_flag_joy_cam;  // 568
    int i_flag_cam_initialized;  // 572
    int i_flag_virtual_Stick2_click;  // 576
    int i_flag_forbid_free_cam;  // 580
    int i_flag_ignore_gencam;  // 584
    float f_cam_coef_lacet;  // 588
    float f_cam_coef_tangage;  // 592
    float f_cam_blend_speed;  // 596
    float f_cam_target_dist;  // 600
    float f_cam_dist;  // 604
    float f_cam_joy_lacet;  // 608
    float f_cam_joy_tangage;  // 612
    float f_last_cam_coef_lacet;  // 616
    float f_last_cam_coef_tangage;  // 620
    float f_enterprise_speed_min;  // 624
    float f_enterprise_speed_max;  // 628
    float f_enterprise_dist_max;  // 632
    float f_time_ent_cam;  // 636
    float f_time_joy_cam;  // 640
    float f_time_joy_cam_released;  // 644
    float f_time_ground;  // 648
    float f_time_cam_start;  // 652
    Vec3 v_cam_banking;  // 656
    Vec3 v_cam_ideal_axe;  // 668
    Vec3 v_cam_init_pos;  // 680
    Vec3 v_target_init_pos;  // 692
    Vec3 v_last_pos_sally;  // 704
    Vec3 v_pos_target_talk;  // 716
    int i_desactive_zdm;  // 728
    int i_jai_pris_la_main;  // 732
    float f_mode_min_dist;  // 736
    Vec3 v_joy_1;  // 740
    Vec3 v_last;  // 752
    float f_time_blend;  // 764
    Vec3 v_force_pos;  // 768
    int i_peut_prendre_la_vue;  // 780
    int i_must_leave_dyn_plan;  // 784
    float f_shift_angle;  // 788
    int i_turn_around_tgt;  // 792
    Vec3 v_last_axis;  // 796
    int i_mode_mgs;  // 808
    float f_blend_eviting;  // 812
    float f_blend_mgs;  // 816
    float f_blend_z;  // 820
    float f_blend_tz;  // 824
    float f_blend_dist;  // 828
    float f_z_mgs;  // 832
    int i_mgs_ks_bas;  // 836
    float f_addz_air;  // 840
    int i_ctrl_cam;  // 844
    int i_aircraft_course;  // 848
    int i_last_sens;  // 852
    int i_interdire_sens;  // 856
    float f_dist_rot_air;  // 860
    Vec3 v_axis_air;  // 864
    float f_time_air;  // 876
    float f_min_mgs;  // 880
    float f_max_mgs;  // 884
    float f_last_dz;  // 888
    float f_last_dtz;  // 892
    float f_dist_mgsmgs;  // 896
    float f_last_add_z_air;  // 900
    int i_mgs_chien;  // 904
    Vec3 v_last_pos_normale;  // 908
    float f_cut_to_ctrl_timer;  // 920
    Vec3 v_last_target_pos;  // 924
    float f_cut_to_wall_timer;  // 936
    float f_last_distance;  // 940
} tScriptStruct_PJCamera;

typedef struct sScriptStruct_PJCameraTarget {
    Vec3 v_offset_sight;  // 0
    int o_followed_actor1;  // 12
    int o_gencamera;  // 16
    int o_gencamera_old;  // 20
    Vec3 v_move_old;  // 24
    Vec3 v_cur_sight;  // 36
    int o_actor1_old;  // 48
    Vec3 v_default_sight;  // 52
    int i_force_global_pos;  // 64
    Vec3 v_offset_sight_mario;  // 68
    float f_speed_trans;  // 80
    Vec3 v_offset_screen;  // 84
    int i_must_cut;  // 96
    tMessage m_MSG_in;  // 100
    int o_target_cine;  // 152
    int i_cine_mode;  // 156
    Vec3 v_sfx_vec1;  // 160
    Vec3 v_sfx_vec2;  // 172
    int i_sfx_type;  // 184
    float f_sfx_flo1;  // 188
    float f_sfx_flo2;  // 192
    float f_coeff;  // 196
    float f_coeff2;  // 200
    Vec3 v_global_pos;  // 204
    int o_lockedgen;  // 216
    int i_lockedtotal;  // 220
    int i_cut_to_mario;  // 224
    Vec3 v_last_global_pos;  // 228
    int i_last_cinemode;  // 240
    float f_rot_local_y;  // 244
    float f_time_blend;  // 248
    Vec3 v_cam_banking;  // 252
    float f_speed_trans_z;  // 264
    float f_pc_last_cut_time;  // 268
    Vec3 v_pc_last_pos;  // 272
    Vec3 v_pc_last_cam_pos;  // 284
    Vec3 v_pc_last_cam_sight;  // 296
    int i_must_blend_cut;  // 308
} tScriptStruct_PJCameraTarget;

typedef struct sScriptStruct_PJCameraDerriere {
    Vec3 v_sight;  // 0
    float f_dist;  // 12
    float f_AngleZ;  // 16
    float f_HauteurZ;  // 20
    int i_cancut;  // 24
    int i_mustcut;  // 28
    int i_external_cancut;  // 32
    int i_was_external_cancut;  // 36
    int i_sim_click;  // 40
    Vec3 v_use_sight;  // 44
    int i_cut_by_time;  // 56
    float f_speed_move;  // 60
    int i_move_en_cours;  // 64
    Vec3 v_pos_move_smooth;  // 68
    int i_en_echec_vis;  // 80
    Vec3 v_force_sight;  // 84
    int o_followed;  // 96
} tScriptStruct_PJCameraDerriere;

typedef struct sScriptStruct_GenCamMario {
    int des_o_actor_detected;  // 0
    int des_o_actor_followed;  // 4
    int des_o_actor_sight;  // 8
    int des_i_actor_followed_is_fight;  // 12
    int des_i_actor_sight_is_fight;  // 16
    int des_o_actor_CapaSwitch;  // 20
    int __des_ai_special_flags_Dimension1;  // 24
    int des_ai_special_flags[5];  // 28
    int __des_ai_special_flags_off_Dimension1;  // 48
    int des_ai_special_flags_off[5];  // 52
    int des_i_fight;  // 72
    int des_i_enterprise;  // 76
    float des_f_dist_max;  // 80
    int des_i_mode;  // 84
    int des_i_cut_enter;  // 88
    int des_i_cut_leave;  // 92
    int des_i_cut_always;  // 96
    int des_i_auto_desactive;  // 100
    float des_f_trans_factor;  // 104
    float des_f_idealdist;  // 108
    float des_f_idealdist_factor;  // 112
    float des_f_distfight;  // 116
    float des_f_idealzangle;  // 120
    float des_f_entering_angle;  // 124
    int des_i_angle_front;  // 128
    int des_i_entering_onetime;  // 132
    int des_i_test_angle_one;  // 136
    int des_o_entering_reference;  // 140
    float des_f_coef_speed;  // 144
    float des_f_coef_inertie;  // 148
    int __des_o_actor_activate_Dimension1;  // 152
    int des_o_actor_activate[5];  // 156
    int __des_o_actor_disactivate_Dimension1;  // 176
    int des_o_actor_disactivate[5];  // 180
    int des_i_user_cut;  // 200
    int des_i_user_to_mario;  // 204
    int des_i_can_look;  // 208
    float des_f_factor_sight;  // 212
    int des_i_test_ground;  // 216
    int des_i_test_dodge;  // 220
    float des_f_time_disactivate;  // 224
    float f_time_disactivate;  // 228
    int des_i_zoom_morfle_allowed;  // 232
    int des_i_zoom_talk_allowed;  // 236
    float des_f_fondu_enchaine_enter;  // 240
    float des_f_shift_angle;  // 244
    float des_f_factor_dist_sight;  // 248
    int des_i_axis_sens;  // 252
    int des_i_control_axis;  // 256
    int des_i_control_Z;  // 260
    int des_i_proj_gen;  // 264
    int des_i_calc_vis;  // 268
    int des_i_user_ctrl;  // 272
    int des_i_const_dist;  // 276
    float des_f_max_angle;  // 280
    int des_i_around_fight;  // 284
    Vec3 des_v_sight;  // 288
    Vec3 des_v_sight_screen;  // 300
    float des_f_inertie_tgt;  // 312
    int des_i_global_pos;  // 316
    int des_i_turn_around_tgt;  // 320
    float des_f_focale;  // 324
    int i_first_in;  // 328
    int i_ready;  // 332
    float des_i_spycam;  // 336
    float des_f_spy_left;  // 340
    float des_f_spy_right;  // 344
    float des_f_spy_up;  // 348
    float des_f_spy_down;  // 352
    float f_last_speed_trans;  // 356
    int o_last_fight;  // 360
    int des_i_spycam_autocut;  // 364
    int i_cancel_entering;  // 368
    float f_blend_mgs;  // 372
    float des_f_mgs_hauteur;  // 376
    int des_i_keep_sight_bas;  // 380
    float des_f_min_mgs;  // 384
    float des_f_max_mgs;  // 388
    float des_f_dist_mgs;  // 392
    int des_i_mgs_chien;  // 396
    float f_time_chg_tgt;  // 400
    float f_dist_tgt_fight;  // 404
    float f_time_no_tgt;  // 408
    float des_f_dist_fightfight;  // 412
    float des_f_angle_fightfight;  // 416
} tScriptStruct_GenCamMario;

typedef struct sScriptStruct_GenCamES {
    int des_o_actor_detected;  // 0
    int des_o_enterobj;  // 4
    int des_o_visee_fromme;  // 8
    int des_o_visee_fromlautre;  // 12
    float des_f_idealdist;  // 16
    float des_f_focale;  // 20
    int des_i_sortie_sur_place;  // 24
    int i_persoin;  // 28
    int i_camin;  // 32
    int i_actif;  // 36
    Vec3 v_togopos;  // 40
    Vec3 v_vistogo;  // 52
    int i_can_cut;  // 64
    int i_cut_first;  // 68
} tScriptStruct_GenCamES;

typedef struct sScriptStruct_CameraSpeciale {
    int des_o_followed_actor;  // 0
    int End_mode;  // 4
    float f_focal_curr;  // 8
    float f_focal;  // 12
    float f_Look_focal_init;  // 16
    float f_focal_normal;  // 20
    float f_focal_target;  // 24
    float f_focal_speed;  // 28
    int i_sens;  // 32
    int init_look;  // 36
    Vec3 v_old_joyvector;  // 40
    Vec3 v_lim_left;  // 52
    Vec3 v_lim_right;  // 64
    float tf_angle_side_min;  // 76
    int i_special_exit;  // 80
    Vec3 v_banking;  // 84
    float f_angle_cumul;  // 96
    float f_angle_dt;  // 100
    float f_time_start_look;  // 104
    int dist;  // 108
    Vec3 v_offset_vertical;  // 112
    Vec3 v_offset_horizontal;  // 124
    float f_offset_z;  // 136
    float f_look_disthoriz_tete;  // 140
    float f_alfa_incline;  // 144
    float f_joy_incline;  // 148
    float f_joy_incline_old;  // 152
    float f_angle_limite;  // 156
    int i_lock_ZDM;  // 160
    Vec3 v_ZDM_init;  // 164
    int i_cam_check_pos;  // 176
    Vec3 v_horizon_init;  // 180
    Vec3 v_sight_init;  // 192
    float f_offset_horizontal_coef;  // 204
    float f_offset_horizontal_sens;  // 208
    float f_offset_horizontal_sens_old;  // 212
    float f_dist_max;  // 216
    float f_dist_max_old;  // 220
    int o_tete;  // 224
    int o_bassin;  // 228
    int o_torse;  // 232
    int i_init_stealth;  // 236
    int i_can_smooth;  // 240
    Vec3 v_pos_dest;  // 244
    Vec3 v_wanted_sight;  // 256
    float f_init_dist;  // 268
    int i_effect_on;  // 272
    Vec3 v_pos_init_tete;  // 276
    Vec3 v_pos_init_torse;  // 288
    Vec3 v_pos_dest_camera;  // 300
    int i_snd_zoom;  // 312
    float f_snd_freq_zoom;  // 316
    int i_must_zero;  // 320
} tScriptStruct_CameraSpeciale;

typedef struct sScriptStruct_CameraNew {
    int o_request_cible;  // 0
    Vec3 v_request_axis;  // 4
    float f_request_dist;  // 16
    float f_request_z;  // 20
    Vec3 v_request_tgt;  // 24
    float f_request_blend_tgtposXY;  // 36
    float f_request_blend_tgtposZ;  // 40
    float f_request_blend_dist;  // 44
    float f_request_blend_z;  // 48
    int i_request_cut;  // 52
    float f_request_hauteur_cible;  // 56
    int i_disable;  // 60
    int i_request_ray_tgt;  // 64
    int o_plugto;  // 68
    int i_can_assign;  // 72
    Vec3 v_request_pos_tgt;  // 76
    int f_request_big_blend;  // 88
    int f_request_big_blend_until_rea;  // 92
    Vec3 v_request_force_sight;  // 96
    Vec3 v_request_cut_axis;  // 108
    Vec3 v_request_pos_cible;  // 120
    Vec3 v_joy_1;  // 132
    Vec3 v_wantedpos_tgt;  // 144
    Vec3 v_curpos_tgt;  // 156
    Vec3 v_curaxis;  // 168
    float f_curdist;  // 180
    float f_cur_z;  // 184
    int o_last_mur_courant;  // 188
    float f_add_z_pente;  // 192
    int o_last_fight;  // 196
    float f_time_chg_tgt;  // 200
    int i_was_cine;  // 204
    int i_was_look;  // 208
    Vec3 v_gencam_axis;  // 212
    float f_gencam_dist;  // 224
    float f_gencam_z_pos;  // 228
    Vec3 v_gencam_tgtpos;  // 232
    int i_gencam_notestray;  // 244
    float f_last_z_aircraft;  // 248
} tScriptStruct_CameraNew;

typedef struct sScriptStruct_tGSTFightManager {
    int __ao_Good_Actor_Dimension1;  // 0
    int ao_Good_Actor[12];  // 4
    int __ao_Good_Target_Dimension1;  // 52
    int ao_Good_Target[12];  // 56
    int __af_Good_NRJ_Courante_Dimension1;  // 104
    float af_Good_NRJ_Courante[12];  // 108
    int __af_Good_Last_Hit_Dimension1;  // 156
    float af_Good_Last_Hit[12];  // 160
    int __ao_Good_Last_Evil_Dimension1;  // 208
    int ao_Good_Last_Evil[12];  // 212
    int __af_Good_Last_Dmg_Dimension1;  // 260
    float af_Good_Last_Dmg[12];  // 264
    int __af_Good_Last_Paf_Dimension1;  // 312
    float af_Good_Last_Paf[12];  // 316
    int __af_Good_Cumul_Dmg_Dimension1;  // 364
    float af_Good_Cumul_Dmg[12];  // 368
    int __ai_Good_Targeted_Dimension1;  // 416
    int ai_Good_Targeted[12];  // 420
    int __ai_Good_NbHit_Dimension1;  // 468
    int ai_Good_NbHit[12];  // 472
    int __ao_Evil_Actor_Dimension1;  // 520
    int ao_Evil_Actor[30];  // 524
    int __ao_Evil_Target_Dimension1;  // 644
    int ao_Evil_Target[30];  // 648
    int __af_Evil_NRJShieldMax_Dimension1;  // 768
    float af_Evil_NRJShieldMax[30];  // 772
    int __af_Evil_NRJ_Courante_Dimension1;  // 892
    float af_Evil_NRJ_Courante[30];  // 896
    int __af_Evil_Last_Hit_Dimension1;  // 1016
    float af_Evil_Last_Hit[30];  // 1020
    int __ao_Evil_Last_Good_Dimension1;  // 1140
    int ao_Evil_Last_Good[30];  // 1144
    int __af_Evil_Last_Dmg_Dimension1;  // 1264
    float af_Evil_Last_Dmg[30];  // 1268
    int __af_Evil_Last_Eff_Dimension1;  // 1388
    float af_Evil_Last_Eff[30];  // 1392
    int __af_Evil_Cumul_Dmg_Dimension1;  // 1512
    float af_Evil_Cumul_Dmg[30];  // 1516
    int __ai_Evil_Targeted_Dimension1;  // 1636
    int ai_Evil_Targeted[30];  // 1640
    int __ai_Evil_Icone_Dimension1;  // 1760
    int ai_Evil_Icone[30];  // 1764
    int __af_Evil_Dmin_Dimension1;  // 1884
    float af_Evil_Dmin[30];  // 1888
    int __af_Evil_DMax_Dimension1;  // 2008
    float af_Evil_DMax[30];  // 2012
    int __af_Evil_Angle_Dimension1;  // 2132
    float af_Evil_Angle[30];  // 2136
    int __ai_Evil_AngleSight_Dimension1;  // 2256
    Vec3 ai_Evil_AngleSight[30];  // 2260
    int i_Good_NbActor;  // 2620
    int i_Evil_NbActor;  // 2624
    int i_Evil_NbActor_old;  // 2628
    int i_Evil_NbActorAlive;  // 2632
    float f_dist_attaque_comp;  // 2636
    int i_fight_frappe;  // 2640
    float f_fight_time_frappe;  // 2644
    float Cf_frappe_duree_SF;  // 2648
    int i_cine_fin_fight_id;  // 2652
    int i_use_music_fight_manager;  // 2656
    int o_musique_fight;  // 2660
    int o_musique_fight_play;  // 2664
    int i_must_wait_for_music;  // 2668
    float f_time;  // 2672
    int SF_Ennemi1;  // 2676
    int SF_Ennemi2;  // 2680
    int SF_Ennemi3;  // 2684
    int SF_Ennemi4;  // 2688
    int SF_Ennemi5;  // 2692
    int SF_Ennemi6;  // 2696
    int SF_Ennemi7;  // 2700
    int SF_Ennemi8;  // 2704
    float f_time_start_fight_A;  // 2708
    float f_time_start_fight_B;  // 2712
    int i_a_terre_old;  // 2716
    int i_fight_jusqua_mort;  // 2720
    int __am_msg_Dimension1;  // 2724
    tMessage am_msg[20];  // 2728
    int i_msg_number;  // 3768
    int o_last_paf;  // 3772
    float f_last_paf_time;  // 3776
    int i_combo_avancement;  // 3780
    float f_time_change;  // 3784
    int o_last_mort;  // 3788
    int f_delai_comp_paf;  // 3792
    int __i_attaque_compagnon_Dimension1;  // 3796
    int i_attaque_compagnon[12];  // 3800
    float f_time_play;  // 3848
} tScriptStruct_tGSTFightManager;

typedef struct sScriptStruct_CineTextDisplay {
    tMessage message_a_traiter;  // 0
    tTextGet texte_message_a_traiter;  // 52
    int prio_message_a_traiter;  // 60
    int i_Sound_ID;  // 64
    int o_Sound_Localisation3D;  // 68
    float f_Sound_Volume;  // 72
    int i_encart;  // 76
    tMessage message_a_traiter_display;  // 80
    tTextGet texte_message_a_traiter_displ;  // 132
    int prio_message_a_traiter_displa;  // 140
    int i_Sound_ID_display;  // 144
    int o_Sound_Localisation3D_displa;  // 148
    float f_Sound_Volume_display;  // 152
    int i_Sound_Localisation_type;  // 156
    int i_encart_display;  // 160
    int o_dernier_cinetxt;  // 164
    int o_dernier_snd_txt;  // 168
    int o_dernier_visual_txt;  // 172
    Vec3 v_pos_text;  // 176
    Vec3 v_taille_ligne;  // 188
    int i_set_pos;  // 200
    int i_affiche;  // 204
    float f_duree_affichage;  // 208
    float f_time_debut_affichage;  // 212
    int i_txt;  // 216
    float f_near;  // 220
    float f_far;  // 224
    int i_Effect;  // 228
} tScriptStruct_CineTextDisplay;

// FIXME: bele van javítva, mert ugyanolyan nevû vûltozók is vannak bent többször
typedef struct sScriptStruct_PJSally
{
    int __i_joy_action_list_Dimension1;
    int i_joy_action_list[12];
    int i_joy_action_kit;
    int i_joy_action_kit_old;
    int i_joy_external_action_kit;
    int i_joy_external_attente_action;
    int i_joy_internal_attente_action;
    float f_joy_norm;
    float f_joy_timming_no_down;
    Vec3 v_joy_vector;
    float f_time;
    int __ao_overlap_actor_Dimension1;
    int ao_overlap_actor[10];
    Vec3 v_jump_pos;
    int o_jump_wp;
    float f_jump_amplitude;
    float f_jump_timming1;
    float f_jump_timming2;
    int i_jump_reception_type;
    int i_jump_up;
    Vec3 v_jump_offset_position;
    int i_ray_gmat_desactive;
    int i_ray_gamemat1;
    Vec3 v_jump_start_pos;
    int i_jump_special_zdm_managment;
    int i_jump_point_known;
    int o_ray_wall;
    int i_rayjump_down_point;
    int i_jump_petitbond;
    int i_rayjump_force;
    Vec3 v_rayjump_force_raydestpos;
    int i_jump_from_message;
    int i_jump_dontplay_anticipation;
    int i_donot_change_amplitude;
    Vec3 v_jump_ray1posfin;
    Vec3 v_jump_ray1posdeb;
    Vec3 v_jump_zdm_reactivation_pos;
    int i_jump_do_not_change_speed;
    int i_jump_accrochage_rate;
    int i_jump_accrochage_interdire_c;
    int i_jump_accrochage_interdire_m;
    int i_rayjump_anim_chute;
    Vec3 v_jump_chute_sight;
    int i_jump_raccrochage;
    int i_rayjump_impulsion;
    int o_jump_on_actor;
    Vec3 v_jump_sight;
    int i_jump_cam_cut;
    Vec3 v_grabwall_sight;
    Vec3 sight_checker_new_sight;
    Vec3 sight_checker_old_sight;
    int sight_checker_counter;
    float f_blink_progress;
    float f_blink_time;
    float f_blink_delai;
    int i_blink_double_allowed;
    int i_expression_kit;
    int i_expression_oldkit;
    int i_blink_now;
    int i_expression_de_cinematique;
    tMessage m_expression_message;
    int i_d2_use_joy_sight;
    float f_coef_sight_blend_blend;
    Vec3 v_joy_vector_blend;
    Vec3 v_2d_bong_normale;
    int i_d2_marche_contre_le_mur;
    float f_time_last_2Dinit;
    float f_time_last_wait;
    Vec3 v_plaque_normale;
    float f_plaque_time_arret;
    float f_time_plaquecontre;
    int i_plaque_gmat;
    float f_kickassi_time;
    Vec3 v_kick_oriente;
    int o_magnet;
    float f_dot_magnet_sight;
    float f_dot_magnet_pos;
    int current_pm_mode;
    int current_pm_priority;
    int f_cur_root_function;
    int old_pm_mode;
    int old_sally_status;
    float i_same_msg_received_during_ac;
    int i_cinematique_mode;
    int f_cinematique_return_function;
    tMessage msg_cinematique;
    int i_cinematique_colset;
    tMessage message_action;
    float i_paf_force_du_coup;
    float f_paf_shield_time;
    float f_paf_time;
    int i_paf_en_cours;
    int o_last_paffed;
    int message_contextuel_recu;
    tMessage message_contextuel;
    tMessage msg_fight;
    tMessage msg_super_prio;
    int i_message;
    int i_echec;
    int o_push_object;
    int i_push_ok;
    int i_je_pousse_seule;
    int i_push_turn;
    Vec3 v_push_axe;
    int i_PP_Action1;
    int i_PP_Action2;
    int i_PP_Action3;
    int i_PP_Action4;
    int i_takeput;
    int i_takeput_objet;
    int i_kick_init;
    int i_kick_hit;
    Vec3 v_kick_ZDF_Pos;
    int o_action_collide;
    int __am_recieved_Dimension1;
    tMessage am_recieved[4];
    int o_jeter_surki;
    int o_jeter_koi;
    int i_jeter_id;
    Vec3 v_jeter_offset;
    int o_jeter_master;
    int i_fight_on;
    int i_fight_on_old;
    int i_fight_zone;
    int i_fight_flags;
    int o_fight_look_and_cam;
    float f_fight_aie;
    int i_fight_action;
    float f_time_fight;
    float f_time_protect;
    int i_fight_cancel;
    int i_fight_old_ani_CBits;
    float f_fight_kick_time;
    int i_fight_boutonX_valide;
    int i_fight_boutonR_valide;
    int i_fight_boutonC_valide;
    int i_fight_paf_type;
    float f_fight_paf_energie;
    int i_fight_paffed;
    int i_fight_must_sight;
    Vec3 v_fight_zdef_pos;
    Vec3 v_fight_zdef_size;
    Vec3 v_fight_roulade_vector;
    Vec3 v_fight_morfle;
    float f_fight_morfle_energie;
    int o_fight_morfle;
    float f_fight_morfle_time;
    float f_fight_morfle_defense_nbr;
    int i_fight_morfle_defense_flag;
    int i_morfle_action_item;
    int i_fight_morfle_anim_defense;
    int i_speak_fini;
    Vec3 tag_vectorpos;
    Vec3 tag_vectorsight;
    int tag_flag;
    int tag_value;
    int tag_change_value;
    int tag_type;
    int tag_change_type;
    int tag_mode;
    int tag_change_mode;
    int tag_etage;
    int tag_etage_force;
    int __tag_2x2dyn_pos_Dimension1;
    Vec3 tag_2x2dyn_pos[4];
    float tag_size;
    float tag_change_size;
    Vec3 tag_change_free_pos1;
    Vec3 tag_free_pos1;
    Vec3 tag_change_free_pos2;
    Vec3 tag_free_pos2;
    int des_i_type_acteur;
    int des_o_acteur_suividesyeux;
    Vec3 des_v_hauteur_cible;
    int des_i_RT_flags;
    float des_f_RT_vitesse_de_rotation;
    Vec3 v_head_joy_vector_blend;
    int __ai_expr_bouche_blabla_liste_Dimension1;
    int ai_expr_bouche_blabla_liste[32];
    int i_expr_bouche_blabla_id;
    int i_expr_bouche_auto;
    int __f_expr_factor_target_Dimension1;
    float f_expr_factor_target[7];
    int __f_expr_factor_current_Dimension1;
    float f_expr_factor_current[7];
    int __f_expr_prog_current_Dimension1;
    float f_expr_prog_current[7];
    int __f_expr_speed_Dimension1;
    float f_expr_speed[7];
    int __f_expr_forme_Dimension1;
    float f_expr_forme[7];
    int __f_expr_flag_Dimension1;
    float f_expr_flag[7];
    int o_action_kit_owner;
    int o_action_kit_owner_old;
    int ti_action2;
    Vec3 macro_regard_aleatoire_v_curr;
    Vec3 macro_regard_aleatoire_v_targ;
    Vec3 macro_regard_aleatoire_v_inte;
    int macro_regard_aleatoire_i_cpt;
    int i_anim_attente;
    float f_attente_delai;
    float f_attente_course;
    float f_time_before_anim_attente;
    float f_time_before_regard_aleatoir;
    int i_attente_force;
    int i_jump_action_cloche;
    int o_take_put_on;
    int o_objet_en_main;
    Vec3 v_grabwall_pos_zone;
    float f_takeput_scale;
    float f_takeput_scale_target;
    int i_takeput_blendversactor_mode;
    float f_takeput_meta_time;
    float f_takeput_tempo;
    int o_take_put_oriente;
    int i_take_put_posset;
    int o_module_tete;
    int o_module_cheuveux;
    int o_module_pupD;
    int o_module_pupG;
    int o_module_PlugMainG;
    int o_module_shield;
    int o_module_mainG;
    int o_module_mainD;
    Vec3 v_reflex_sight;
    float f_time_cogne_comp;
    float f_t;
    Vec3 v_fight_axe_perso_sally;
    float f_fight_dist_perso_sally;
    float f_fight_coef_blendrotate;
    Vec3 v_axe;
    float f_time_jump;
    int i_mana_message;
    float f_fight_time_roulade;
    float f_roulade_oriente_speed;
    int i_sfx_halo;
    float f_sfx_halo_time;
    int i_sfx_halo_color;
    float f_time_last_cine;
    Vec3 stress_sequence_vec_target;
    Vec3 stress_sequence_vec_source;
    float stress_sequence_time;
    float stress_sequence_curdelay;
    int stress_sequence_id;
    int __stress_sequence_vec_Dimension1;
    Vec3 stress_sequence_vec[20];
    int __stress_sequence_delay_Dimension1;
    float stress_sequence_delay[20];
    int stress_sequence_sym;
    int i_shape_gant;
    int i_shape_haute_def;
    int i_cheat_status;
    int i_cheat_compagnon_nbr;
    int i_cheat_compagnon_ID;
    int o_objet_en_main_AP;
    int i_fight_morfle_nbr;
    int i_app_photo_must_desapear;
    float f_time_exit_mode_look;
    float f_time_lance_un_frisbee;
    float f_time_coef;
    float f_time_coef_ref;
    int i_fight_axe_paf;
    int i_shield_target_color;
    int i_shield_on;
    float f_time_sortie_shield;
    Vec3 v_shield_axe;
    Vec3 v_shield_torce;
    int o_module_torce;
    float f_time_R2;
    int i_bond_blend;
    float f_time_d;
    float f_time_g;
    float f_time_m;
    float f_time_mg;
    Vec3 v_axe_sightjoy;
    int haloG;
    float haloG_size;
    int haloD;
    float haloD_size;
    int haloM;
    float haloM_size;
    int haloMg;
    float haloMg_size;
    int i_SFX_Ano;
    int i_SFX_Ano_old;
    int __f_SFX_Ano_size_Dimension1;
    float f_SFX_Ano_size[5];
    int i_SFX_Ano_status;
    float f_zz;
    int GFX;
    int __v_axez_Dimension1;
    Vec3 v_axez[5];
    int __v_table_around_Dimension1;
    Vec3 v_table_around[36];
    int colora;
    int colorb;
    int __f_posz_Dimension1;
    float f_posz[5];
    float f_fight_blend_coef;
    float f_time_ano;
    int o_fight_who_old;
    float f_sfx_noise_big;
    float f_sfx_noise_total;
    float f_kick_time_ref;
    float f_time_last_ano;
    int i_fight_cam_cumul_kick;
    int i_already_hit;
    float f_fight_time_salto;
    float f_le_plus_loin;
    float f_time_repoussing;
    Vec3 v_pos_after_recal_old;
    Vec3 v_pos_after_recal;
    int __tai_report_rank_Dimension1;
    int tai_report_rank[30];
    int i_do_not_corner;
    float f_time_gaz;
    int o_flash_gaz;
    float f_fight_time_cumul_kick;
    int i_fight_must_release_R2;
    int GFX_Smoke;
    float f_gfx_rate;
    int i_do_smoke;
    float f_time_smoke;
    float f_time_gaz_paf;
    float f_fight_time;
    float f_fight_time_action;
    int __ao_Fight_list_of_actors_Dimension1;
    int ao_Fight_list_of_actors[30];
    int __af_Fight_list_of_distances_Dimension1;
    float af_Fight_list_of_distances[30];
    int __ai_Fight_list_of_bits_Dimension1;
    int ai_Fight_list_of_bits[30];
    int i_Fight_list_nbr;
    int __o_fightSE_who_old_Dimension1;
    int o_fightSE_who_old;
    int o_fightSE_who;
    int i_fightSE_zone;
    int i_fightSE_bit;
    float f_fight_blend_speed;
    int i_fight_id_FM;
    int i_power_on;
    float f_power_time;
    int i_SFX_baton_Halo1;
    int i_SFX_baton_Halo2;
    float f_fight_time_button;
    Vec3 v_fight_kick_axis;
    Vec3 v_fight_kick_axis_non_tourne;
    float f_fight_ratio_distance;
    int i_fight_kick_enchainement_id;
    int i_fight_action_courante_cbits;
    int i_fight_action_courante_cbits1;
    float f_fight_kick_time_cb1;
    Vec3 v_fight_memo_axis;
    Vec3 v_fight_kickselector_axis;
    int i_fight_zdef_nbr_paf_max;
    Vec3 v_fight_att_sight;
    Vec3 v_fight_att_horiz;
    int i_fight_kickselector_mode;
    float f_time_memo_sighthoriz;
    int i_fight_must_staff;
    float f_fight_duree_fenetre_1;
    int i_fight_fenetre2;
    int i_SF_baton;
    float f_baton_XMen_blend;
    float f_baton_XMen_blend_target;
    int i_baton_XMen_color;
    Vec3 v_fight_kick_axis_force;
    int i_fight_distance;
    int GFX_Particules;
    int i_fight_kick_fenetre2;
    float f_time_last_mvt;
    int i_fight_shield_sight_and_snap;
    int i_fight_must_salto;
    float f_toutchy_norm;
    Vec3 v_mur_axis_old;
    float f_look_hauteur_tete;
    float f_look_disthoriz_tete;
    int i_sally_est_force_accroupi;
    Vec3 v_fight_pafkick_axis_force;
    int i_animchange_anim;
    Vec3 v_animchange_orientation;
    Vec3 v_paf_lastvector;
    float f_fight_kick_dist;
    int o_fight_perso_kicke;
    int i_fight_devant;
    Vec3 v_pos_h1;
    Vec3 v_pos_h2;
    int o_fight_last_sighted_perso;
    int i_fight_big_SA;
    float f_tt;
    int o_fight_perso_esquive;
    float f_fight_look_time;
    float f_fight_esquive_time;
    int i_stealth_accroupi;
    int GFX_Flash;
    float f_zz_blend_coef;
    float f_sin;
    int i_Fight_dymmy;
    int i_flag_paf_abborted;
    int i_jump_buttonjump_test;
    int i_joy_external_attente_action1;
    float f_lightUP_current;
    float f_lightUP_target;
    float f_lightUP_old;
    float f_lightDN_current;
    float f_lightDN_target;
    float f_lightDN_old;
    int o_module_bassin;
    int o_module_ventre;
    int o_module_sac;
    int i_reflex_action_courante;
    int i_stealth_mode;
    float f_time_wait_plaque_mur;
    float f_sight_speedup;
    float f_reflex_speed_courant;
    int f_jump_function;
    int i_gmat_sol;
    int o_baton_en_main;
    int i_sally_arret;
    float f_sally_arret_speed;
    int i_reflex_actionitem_courante;
    int i_reflex_action_courante_old;
    int i_reflex_actionitem_courante_;
    int f_anim_freq_ref;
    int i_sally_ombre;
    int i_fight_coup_long_interdit;
    int i_must_stop_joy;
    int i_snd;
    int i_rnd_snd;
    int i_rnd_esquive_snd;
    int i_attente_a_interrompre;
    int i_reflex_action_desflags_cour;
    int i_reflex_action_desflags_old;
    float f_time_before_anim_attente_ch;
    int i_joy_internal_attente_action1;
    int i_baton_visible;
    float f_baton_progression;
    int i_do_uppercut;
    int i_snd_cri_SA;
    float f_time_GrabWall;
    Vec3 v_balayage_axis;
    float f_paf_time_force;
    int o_go;
    int i_regard_mode_regard;
    int i_force_shadow;
    int i_st_on_corner;
    Vec3 v_var_axis;
    int i_fight_freqmanager_raz;
    int i_kick_demande_old;
    float f_fight_freqmanager_speedcoef;
    Vec3 v_new_axis;
    Vec3 v_old_axis;
    float tmp_float;
    float f_jump_amplitudax;
    int i_force_attenuation;
    float f_rot_coef;
    int i_demi_tour_anim;
    int i_freeze;
    Vec3 v_SA_start_pos;
    Vec3 v_demi_tour_axis;
    float f_demi_tour_angle;
    Vec3 v_demi_tour_startaxis;
    int i_joy_must_be_null;
    int i_abort_eat;
    int i_abort_mort;
    int i_abort_mdisk;
    int i_abort_mdisk_flag;
    float f_meta_wait;
    int i_sally_est_force_accroupi_ol;
} tScriptStruct_PJSally;

typedef struct sScriptStruct_GSTSoundManager {
    float f_last_pas_sally;  // 0
    int i_last_snd_id_sally;  // 4
    int i_sally_nbr_avant_snd;  // 8
} tScriptStruct_GSTSoundManager;

typedef struct sScriptStruct_SNDMerge {
    int i_mode;  // 0
    int o_dest_gao;  // 4
    int SF_actif;  // 8
    int o_gao_merge;  // 12
    int o_gao_unmerge;  // 16
    int i_flag_merged;  // 20
} tScriptStruct_SNDMerge;

typedef struct sScriptStruct_CineTextes {
    int __des_at_lignes_texte_Dimension1;  // 0
    tTextGet des_at_lignes_texte[100];  // 4
    int __des_ai_son_associe_Dimension1;  // 804
    int des_ai_son_associe[100];  // 808
    int __ai_son_differents_Dimension1;  // 1208
    int ai_son_differents[100];  // 1212
    int __des_af_volume_associe_Dimension1;  // 1612
    float des_af_volume_associe[100];  // 1616
    int __des_af_near_associe_Dimension1;  // 2016
    float des_af_near_associe[100];  // 2020
    int __des_af_far_associe_Dimension1;  // 2420
    float des_af_far_associe[100];  // 2424
    int __des_ai_encart_Dimension1;  // 2824
    float des_ai_encart[100];  // 2828
    int des_ai_perso_localisation_par;  // 3228
    int des_i_priorite_par_defaut;  // 3232
    int i_index_ligne;  // 3236
    int i_Indice_compagnon;  // 3240
} tScriptStruct_CineTextes;

typedef struct sScriptStruct_TXTEncart {
    int SF_activation;  // 0
    int SF_activation2;  // 4
    int o_zone;  // 8
    int o_perso;  // 12
    int i_pivotBV;  // 16
    int o_capa_activ;  // 20
    int i_capa_activ;  // 24
    int i_capa_active_vrai;  // 28
    int o_main_actor;  // 32
    int i_main_actor_id;  // 36
    int nbr_evilmin_dans_fight;  // 40
    int nbr_evilmax_dans_fight;  // 44
    int i_test_si_joy_bloque;  // 48
    int i_Sally_MainActor;  // 52
    int i_valide_meme_en_cine;  // 56
    int o_perso_visible;  // 60
    int i_text_ID_X;  // 64
    int i_text_ID_Carre;  // 68
    int i_mode_de_Sally;  // 72
    int i_test_special_mur;  // 76
    int SF_destruction;  // 80
    int o_capa;  // 84
    int i_capa;  // 88
    int i_test_touche;  // 92
    int i_key;  // 96
    int i_test_touche2;  // 100
    int i_key2;  // 104
    int i_lock;  // 108
    int i_joyD_lateral;  // 112
    int i_joyL_lateral;  // 116
    int SF_encart_valide;  // 120
    tTextGet TXT_encart;  // 124
    float f_duree_affichage_texte;  // 132
    float f_delai_attente_mon_txt;  // 136
    float f_delai_attente_autre_txt;  // 140
    float f_delai;  // 144
    float f_time_texte;  // 148
    int i_must_destroy;  // 152
    float f_delai_b4_efface;  // 156
    float f_time_valide;  // 160
} tScriptStruct_TXTEncart;

typedef struct sScriptStruct_MergeActionKit {
    int des_o_actionkit;  // 0
    int des_o_perso;  // 4
    int des_i_act_debut;  // 8
    int des_i_act_fin;  // 12
} tScriptStruct_MergeActionKit;

typedef struct sScriptStruct_CineEvents {
    int des_i_cine_independante;  // 0
    int des_i_cine_type;  // 4
    int des_i_actor_type1;  // 8
    int des_o_actor_detec1;  // 12
    int des_o_actor_zone1;  // 16
    int des_i_collision_type2;  // 20
    int des_o_actor_detec2;  // 24
    int des_o_actor_zone2;  // 28
    int des_o_actor_status1;  // 32
    int des_i_activity_test1;  // 36
    int des_o_actor_capa1;  // 40
    int des_i_capa_test1;  // 44
    int des_i_capa_etat1;  // 48
    int des_o_actor_cbit1;  // 52
    int des_i_cbit_test1;  // 56
    int des_i_cbit_etat1;  // 60
    int des_o_actor_visible1;  // 64
    float des_f_aire_visibilite;  // 68
    int des_i_specialFlag_test1;  // 72
    int des_i_specialFlag_test2;  // 76
    int des_o_acteur_msg;  // 80
    int des_i_msg_ID;  // 84
    int des_i_msg_INT1;  // 88
    int des_o_actor_desstruct;  // 92
    int des_i_desstruct_flag;  // 96
    int des_i_desstruct_num;  // 100
    int des_i_desstruct_int;  // 104
    float des_f_tempo;  // 108
    int des_i_SPFlag_associe_vrai;  // 112
    int des_i_SPFlag_associe_faux;  // 116
    int des_i_ripley;  // 120
    int des_i_blocage_joy;  // 124
    int des_i_look_interdit;  // 128
    int des_i_inventaire_interdit;  // 132
    int __ao_objets_collisions_Dimension1;  // 136
    int ao_objets_collisions[10];  // 140
    int i_ready;  // 180
    int i_tests_a_reussir;  // 184
    float f_temps_ecoule;  // 188
    int i_playing;  // 192
    int f_track_init;  // 196
    float des_f_duree_totale;  // 200
    int des_i_SPFlag_GoPhase2;  // 204
    int des_i_SPFlag_GoPhase3;  // 208
    int des_i_SPFlag_TestPhase2;  // 212
    int des_i_SPFlag_TestPhase3;  // 216
    int des_i_look_EJECT;  // 220
    float f_appui_skip;  // 224
    int des_o_actor_detec_exit1;  // 228
    int des_o_actor_zone_exit1;  // 232
    int des_i_collision_type_exit1;  // 236
    int des_o_actor_status_exit1;  // 240
    int des_i_activity_exit1;  // 244
    int des_o_actor_capa_exit1;  // 248
    int des_i_capa_exit1;  // 252
    int des_i_SPFlag_test_exit1;  // 256
    int i_tests_a_reussir_exit;  // 260
    int des_o_snd_micro;  // 264
    int des_i_HlpTxt_Interdit;  // 268
    int des_i_SndMdf_Musique;  // 272
    int des_i_SndMdf_Ambiance;  // 276
    int des_i_SndMdf_Voix;  // 280
    float f_temps_fondu_1010;  // 284
    float f_temps_fondu_1020;  // 288
    float f_temps_fondu_1040;  // 292
    float des_f_Fadergroup;  // 296
    float f_temps_Fadergroup;  // 300
    int i_enLecture;  // 304
    float f_timecode;  // 308
    int i_sf_fade;  // 312
    float f_first_delay;  // 316
} tScriptStruct_CineEvents;

typedef struct sScriptStruct_ActivatorDoor {
    int des_i_visible_init;  // 0
    int __des_ao_doors_Dimension1;  // 4
    int des_ao_doors[20];  // 8
    int des_i_num_doors;  // 88
    int des_o_forcevisibleinit;  // 92
    int des_o_forcevisibleinit1;  // 96
    int des_o_forcevisibleinit2;  // 100
    int des_o_forceinvisibleinit;  // 104
    int des_o_forceinvisibleinit_not;  // 108
    int des_i_water_onoff;  // 112
    int des_o_mm;  // 116
    int des_i_wait_a_little;  // 120
    int inbv;  // 124
    int lastdoor;  // 128
    int i_ID;  // 132
    int i_washide;  // 136
    int i_totaff;  // 140
} tScriptStruct_ActivatorDoor;

typedef struct sScriptStruct_Activator {
    int des_o_detect;  // 0
    int __des_ao_portes_Dimension1;  // 4
    int des_ao_portes[20];  // 8
    int __des_ao_visible_portes_Dimension1;  // 88
    int des_ao_visible_portes[20];  // 92
    int __des_ao_virer_les_secteurs_Dimension1;  // 172
    int des_ao_virer_les_secteurs[40];  // 176
    int __des_ao_montrer_les_secteurs_Dimension1;  // 336
    int des_ao_montrer_les_secteurs[40];  // 340
    int des_o_cacher_groupe_de;  // 500
    int des_i_culldoor;  // 504
    float des_f_distcut;  // 508
    int des_i_auto_hide;  // 512
    int des_i_register;  // 516
    int i_cptact;  // 520
    int i_forceshow;  // 524
    int myID;  // 528
    int i_ON;  // 532
} tScriptStruct_Activator;

typedef struct sScriptStruct_Mirror {
    int des_i_type;  // 0
    int des_i_decors;  // 4
    int des_i_generator;  // 8
    int des_i_specialflag;  // 12
    Vec3 des_v_offset_rayon;  // 16
    int des_o_Wall_GAO;  // 28
    int des_o_follow_dest;  // 32
    int des_i_orientation;  // 36
    int des_i_sf_peut_exploser;  // 40
    float des_f_sf_time;  // 44
    int des_puissance;  // 48
    int des_i_forcelum;  // 52
    int des_i_forcechaleur;  // 56
    int des_i_forceelec;  // 60
    int des_i_forceinerte;  // 64
    int des_i_force_axe_recul;  // 68
    float des_f_epaisseur_faisceau;  // 72
    float des_f_maxlen;  // 76
    float des_f_radius;  // 80
    float des_i_samples;  // 84
    int des_o_gaomat;  // 88
    int des_i_submat;  // 92
    int des_i_submat_halo;  // 96
    int des_i_detect_all;  // 100
    int des_i_detect_ai;  // 104
    int des_i_detect_meca;  // 108
    int des_i_detect_anim;  // 112
    float f_first_wait;  // 116
    int des_i_move;  // 120
    float des_f_angletoler;  // 124
    float des_f_step;  // 128
    int des_o_linked;  // 132
    int des_i_backface;  // 136
    int des_i_transfert_auto;  // 140
    int des_i_numdest;  // 144
    int __des_ao_dest_Dimension1;  // 148
    int des_ao_dest[20];  // 152
    int __des_af_angletoler_Dimension1;  // 232
    float des_af_angletoler[20];  // 236
    int des_i_canreflect;  // 316
    int des_o_const_gao;  // 320
    int des_o_const_gao1;  // 324
    int des_c_const_base;  // 328
    int des_c_const_allum;  // 332
    int des_i_halo;  // 336
    int des_i_spark;  // 340
    int des_i_alone;  // 344
    Vec3 des_v_offset;  // 348
    int des_color_1;  // 360
    int des_color_2;  // 364
    int des_mouvement;  // 368
    float des_f_amplitude;  // 372
    Vec3 v_init_pos;  // 376
    int c_current_color;  // 388
    int i_GFX;  // 392
    int i_oldGFX;  // 396
    int i_baseGFX;  // 400
    int i_islighted;  // 404
    int i_ideclaire;  // 408
    Vec3 v_entering;  // 412
    tMessage msg;  // 424
    int __i_idspark_Dimension1;  // 476
    int i_idspark[2];  // 480
    int __i_idhalo_Dimension1;  // 488
    int i_idhalo[2];  // 492
    Vec3 v_destpos;  // 500
    Vec3 v_dir;  // 512
    int detruit_moi;  // 524
    int i_request_play;  // 528
    int o_snd_cur_target;  // 532
    int o_snd_old_target;  // 536
    int i_snd_cur_mat;  // 540
    float f_paf_time;  // 544
    int i_do_not_destroy_sound;  // 548
    float f_snd_dist_nosound;  // 552
    int f_freq_additionnelle_pour_dep;  // 556
    int i_snd_flag;  // 560
    int i_snd_lumiere_loop;  // 564
    int i_snd_prisme_loop;  // 568
    int i_flag_ON;  // 572
    int i_flag_1st_trame;  // 576
    int i_sndGFX;  // 580
    float i_coef_de_best_sight;  // 584
    int i_paf_one_time;  // 588
    float f_ray_lenght;  // 592
    int GFX_Halo;  // 596
    float GFX_Halo_Size;  // 600
    float t_time_spark;  // 604
    float t_delay_spark;  // 608
    int spark;  // 612
    int i_je_paf_le_milit;  // 616
} tScriptStruct_Mirror;

typedef struct sScriptStruct_Porte {
    int SF_qui_me_commande;  // 0
    int Capa_qui_me_commande1;  // 4
    int type_de_fonctionnement;  // 8
    int etat_de_declanchement;  // 12
    float attente_sup_apres_mvt;  // 16
    int play_close_on_init;  // 20
    int i_must_open;  // 24
    int o_open_max;  // 28
    float f_open_speed;  // 32
    float f_gravity;  // 36
    float f_gravity_close_max;  // 40
    int o_close_max;  // 44
    float f_close_speed;  // 48
    float f_grav;  // 52
    int i_indy_bvout;  // 56
    Vec3 v_recal_pos;  // 60
    Vec3 v_recal_axis;  // 72
    float f_epaisseur_porte;  // 84
    int o_halo_simple;  // 88
    int __o_halo_simple_supl_Dimension1;  // 92
    int o_halo_simple_supl[3];  // 96
    int interdire_separation_equipe;  // 108
    int o_BV_test_separation;  // 112
    int paf_pendant_fermeture;  // 116
    int ne_touche_pas_aux_colmaps;  // 120
    int pos_basse;  // 124
    int pos_haute;  // 128
    int i_bit;  // 132
    int i_instantane;  // 136
    int i_separation;  // 140
    float f_last_separation_time;  // 144
    int i_nbr_track_ouverture;  // 148
    int snd_le_son_douverture_boucle;  // 152
    int i_sound_BR;  // 156
    int i_snd_instance;  // 160
    float f_coef_volume;  // 164
    int TAG_etage;  // 168
    int TAG_WP_extremite_1;  // 172
    int TAG_WP_extremite_2;  // 176
    int des_sound_event;  // 180
    float des_sound_event_portee;  // 184
    float des_sound_event_duree;  // 188
    int des_sound_event_open_pos;  // 192
    int des_sound_event_close_pos;  // 196
    Vec3 v_memo_indy_sens;  // 200
    int o_active_a_louverture0;  // 212
    int o_active_a_louverture1;  // 216
    int o_active_a_la_fermeture0;  // 220
    int o_active_a_la_fermeture1;  // 224
    int o_desactive_a_louverture0;  // 228
    int o_desactive_a_louverture1;  // 232
    int o_desactive_a_la_fermeture0;  // 236
    int o_desactive_a_la_fermeture1;  // 240
    int o_evt_ouverture;  // 244
    int o_evt_fermeture;  // 248
} tScriptStruct_Porte;

typedef struct sScriptStruct_SFXSallySA {
    float f_size_current;  // 0
    float f_size_target;  // 4
    float f_size_coef;  // 8
    float f_transparency_coef;  // 12
    float f_transparancy_speed;  // 16
    float f_transparancy_no_more_col;  // 20
    int i_after;  // 24
    int i_nbr_of_paffed;  // 28
    float f_force_du_coup;  // 32
    int __a_qui_est_deja_paffed_Dimension1;  // 36
    int a_qui_est_deja_paffed[50];  // 40
    float old_factor;  // 240
    int old_color;  // 244
    float f_time_paf;  // 248
    int i_paf_flag;  // 252
    int halo;  // 256
} tScriptStruct_SFXSallySA;

typedef struct sScriptStruct_LightPAF {
    float f_near;  // 0
    float f_far;  // 4
    int i_color;  // 8
    float f_blend;  // 12
    float f_blend2;  // 16
    float f_near_current;  // 20
    float f_far_current;  // 24
    int i_init_done;  // 28
    int i_type;  // 32
} tScriptStruct_LightPAF;

typedef struct sScriptStruct_SFXComete {
    int des_c_halo;  // 0
    int des_c_particules;  // 4
    int des_i_autokill;  // 8
    float des_f_altitude_sol;  // 12
    float des_f_generationrate;  // 16
    int des_o_ondedechoc;  // 20
    int des_o_explosion;  // 24
    int o_ondedechoc;  // 28
    int o_explosion;  // 32
    int des_o_anim_explosion;  // 36
    int des_o_gerbe;  // 40
    int o_animexplosion;  // 44
    int o_gerbe;  // 48
    int GFX_Particules;  // 52
    int GFX_Halo_Burn_1;  // 56
    int GFX_Flash;  // 60
    int i_Tempo_mort;  // 64
    float f_time_GFX_Particules;  // 68
    float f_speed;  // 72
    Vec3 v_last_pos;  // 76
    int c_constant_old;  // 88
    float old_factor;  // 92
    int old_color;  // 96
    int old_constantcolor0;  // 100
    int old_constantcolor1;  // 104
    int i_after;  // 108
    float f_transparency_coef;  // 112
    int i_meta;  // 116
    float f_size;  // 120
    int i_SFX_baton_Halo1;  // 124
    Vec3 v_pos;  // 128
    int c_constant_old_2;  // 140
    int c_constante_new_2;  // 144
} tScriptStruct_SFXComete;

typedef struct sScriptStruct_Companion {
    int i_cheat_mode_on;  // 0
    int des_o_leader;  // 4
    int des_i_friend;  // 8
    float des_dist_optim;  // 12
    float des_dist_toler;  // 16
    int des_n_walk_network;  // 20
    float des_f_speedmax;  // 24
    int des_o_acteur_text;  // 28
    int i_way_use_network;  // 32
    int n_way_cur_network;  // 36
    int i_way_indice_wp;  // 40
    int __ao_way_wp_list_Dimension1;  // 44
    int ao_way_wp_list[20];  // 48
    int o_way_wp_current;  // 128
    int o_way_wp_target;  // 132
    Vec3 v_way_current_pos;  // 136
    Vec3 v_way_next_pos;  // 148
    int i_slave_mode;  // 160
    int i_slave_type;  // 164
    int i_slave_order;  // 168
    int i_slave_talk;  // 172
    int o_slave_destination;  // 176
    int o_slave_destination_old;  // 180
    int o_slave_next_wp;  // 184
    int o_slave_current_wp;  // 188
    int o_slave_actor_action;  // 192
    int o_slave_actor_action_old;  // 196
    Vec3 v_slave_destination;  // 200
    Vec3 v_slave_destination_old;  // 212
    float f_slave_angle;  // 224
    float f_slave_dist_min;  // 228
    int i_slave_run;  // 232
    int i_slave_wait;  // 236
    int o_slave_ObjInHand;  // 240
    int i_slave_blendversactor_mode;  // 244
    float f_slave_scale_target;  // 248
    float f_slave_scale;  // 252
    float f_slave_meta_time;  // 256
    float f_slave_time_wait;  // 260
    tMessage m_slave_last_order;  // 264
    int i_slave_action_bidouille;  // 316
    int i_slave_autocancel;  // 320
    int i_slave_use_reseau;  // 324
    int i_slave_paf_cancel_vala;  // 328
    int i_slave_pousse_toi;  // 332
    int o_slave_capa_etage;  // 336
    float f_distance_blend;  // 340
    Vec3 v_jump_start_pos;  // 344
    float f_jump_timming1;  // 356
    float f_jump_timming2;  // 360
    Vec3 v_jump_pos;  // 364
    float f_jump_amplitude;  // 376
    Vec3 v_jump_offset_position;  // 380
    int i_jump_reception_type;  // 392
    int i_jump_special_zdm_managment;  // 396
    int i_jump_point_known;  // 400
    int i_jump_do_not_change_speed;  // 404
    int i_jump_down;  // 408
    int i_rayjump_down_point;  // 412
    Vec3 v_jump_zdm_reactivation_pos;  // 416
    Vec3 v_jump_ray1posdeb;  // 428
    Vec3 v_jump_ray1posfin;  // 440
    int i_jump_no_eject;  // 452
    int i_ray_gamemat1;  // 456
    int i_ray_gmat_desactive;  // 460
    int i_rayjump_force;  // 464
    int i_grabwall_ini;  // 468
    Vec3 v_grabwall_sight;  // 472
    Vec3 v_grabwall_sight_up;  // 484
    Vec3 v_grabwall_pos_zone;  // 496
    int o_grabwall;  // 508
    int o_push_object;  // 512
    int o_push_anck;  // 516
    Vec3 v_push_normal;  // 520
    float f_push_timePush;  // 532
    float f_push_timeText;  // 536
    int i_push_ok;  // 540
    int i_push_action_item;  // 544
    int i_fight_action;  // 548
    int o_fight_enemy;  // 552
    float f_fight_change_pos_time;  // 556
    float f_fight_distance;  // 560
    float f_fight_angle_cos;  // 564
    float f_fight_angle_sin;  // 568
    int o_fight_hideout;  // 572
    int i_fight_pos;  // 576
    int i_fight_pos_calc;  // 580
    int i_fight_action_var;  // 584
    int i_fight_pos_ok;  // 588
    int i_fight_action_ok;  // 592
    float f_fight_time_action;  // 596
    float f_fight_time_pos;  // 600
    float f_fight_dist_min;  // 604
    float f_fight_dist_max;  // 608
    float f_fight_dist_cur_square;  // 612
    int f_fight_toler_min;  // 616
    int f_fight_toler_max;  // 620
    float f_fight_angle_max;  // 624
    int __o_fight_hide_Dimension1;  // 628
    int o_fight_hide[2];  // 632
    int i_fight_go_hide;  // 640
    float f_fight_time_baffe;  // 644
    float f_fight_time_tappe;  // 648
    float f_fight_time_check;  // 652
    float f_fight_time_parle_Jade;  // 656
    float f_fight_time_parle_assist;  // 660
    int i_fight_force_du_coup;  // 664
    float f_fight_dist_proche;  // 668
    int i_fight_contourne;  // 672
    Vec3 v_fight_morfle;  // 676
    float f_fight_puissance_coup;  // 688
    Vec3 v_fight_CoupCul_offset;  // 692
    int i_fight_CoupCul_impulse;  // 704
    Vec3 v_fight_esquive;  // 708
    float f_delai_esquive;  // 720
    int i_fight_ami;  // 724
    int i_fight_help_tabasse;  // 728
    float f_fight_Cf_AttTappe;  // 732
    float i_fight_retour_vers_Sally;  // 736
    int i_fight_esquive_laterale;  // 740
    int i_fight_au_sol_fin;  // 744
    float f_fight_delai_parle_Jade;  // 748
    float f_blend_coef;  // 752
    int __ao_fight_hit_Dimension1;  // 756
    int ao_fight_hit[20];  // 760
    float f_fight_time_shield;  // 840
    int i_fight_shield;  // 844
    float f_fight_delai_baffe;  // 848
    float f_fight_delai_shield_immobile;  // 852
    int i_fight_shield_action;  // 856
    int i_fight_shield_action_old;  // 860
    int i_fight_shield_action_nb;  // 864
    int i_CoupCul_retour;  // 868
    int i_CoupCul_meta;  // 872
    int o_CoupCul_target;  // 876
    int i_bidouille_fonctionne;  // 880
    tMessage m_paf_message;  // 884
    int i_paf_tousse;  // 936
    float f_paf_time;  // 940
    Vec3 v_dash_traction;  // 944
    Vec3 v_dash_dest_pos;  // 956
    int i_dash_init;  // 968
    Vec3 v_dash_BV_Min;  // 972
    Vec3 v_dash_BV_Max;  // 984
    Vec3 v_dash_good_sight;  // 996
    float f_dash_good_angle;  // 1008
    float f_dash_start;  // 1012
    Vec3 v_dash_zdm_pos_init;  // 1016
    int i_OT_action;  // 1028
    int i_OT_nbWps;  // 1032
    int __o_OT_tabWps_Dimension1;  // 1036
    int o_OT_tabWps[6];  // 1040
    int i_OT_force_walk;  // 1064
    int i_OT_Att;  // 1068
    int o_blink_module_tete;  // 1072
    tMessage m_blink;  // 1076
    float f_blink_progress;  // 1128
    float f_blink_time;  // 1132
    float f_blink_delai;  // 1136
    int i_blink_double_allowed;  // 1140
    int i_blink_now;  // 1144
    tMessage m_cine_message;  // 1148
    float f_cine_vitesse_rotation_tete;  // 1200
    Vec3 tag_vectorpos;  // 1204
    Vec3 tag_vectorsight;  // 1216
    int tag_flag;  // 1228
    int tag_value;  // 1232
    int tag_change_value;  // 1236
    int tag_type;  // 1240
    int tag_change_type;  // 1244
    int tag_mode;  // 1248
    int tag_change_mode;  // 1252
    int tag_etage;  // 1256
    int tag_etage_force;  // 1260
    int __tag_2x2dyn_pos_Dimension1;  // 1264
    Vec3 tag_2x2dyn_pos[4];  // 1268
    float tag_size;  // 1316
    float tag_change_size;  // 1320
    Vec3 tag_change_free_pos1;  // 1324
    Vec3 tag_free_pos1;  // 1336
    Vec3 tag_change_free_pos2;  // 1348
    Vec3 tag_free_pos2;  // 1360
    int i_way_force_recompute;  // 1372
    int i_way_moving;  // 1376
    Vec3 v_way_currentpos;  // 1380
    Vec3 v_way_destpos;  // 1392
    Vec3 v_way_case_next;  // 1404
    Vec3 v_way_case_dest;  // 1416
    Vec3 v_way_speed;  // 1428
    Vec3 v_way_sight;  // 1440
    float f_way_blendrotate;  // 1452
    int i_way_algo_limitdestpos;  // 1456
    int i_way_etage_dest;  // 1460
    Vec3 v_way_old_case;  // 1464
    int i_way_search_around_destpos;  // 1476
    int i_way_destpos_error;  // 1480
    float f_way_len_big;  // 1484
    float f_way_test_born;  // 1488
    int i_way_case_nbr;  // 1492
    float f_way_time_newcell;  // 1496
    int i_stop;  // 1500
    Vec3 v_actu_dest_pos;  // 1504
    float f_time;  // 1516
    float f_time_delai;  // 1520
    float f_time_OT_wait;  // 1524
    float f_time_regardeVide;  // 1528
    float f_time_regardeSally;  // 1532
    float f_time_regarde_delai;  // 1536
    float f_time_attent_qd_oqp;  // 1540
    float f_time_last_wait;  // 1544
    float f_time_recup_NRJ;  // 1548
    float f_time_se_plaint;  // 1552
    float f_time_shield;  // 1556
    float f_time_morfle;  // 1560
    float f_time_start_run;  // 1564
    float f_time_move_attend_give_to;  // 1568
    float f_time_regard_vague;  // 1572
    float f_time_jump;  // 1576
    float f_time_init;  // 1580
    float f_time_esquive;  // 1584
    float f_time_start_OT;  // 1588
    float f_time_debut_baffe;  // 1592
    float f_time_start_meta;  // 1596
    float f_time_echec;  // 1600
    float f_time_paf_by_sally;  // 1604
    float f_time_nouvelle_case;  // 1608
    int co_current_mode;  // 1612
    int co_old_mode;  // 1616
    int f_gotrack;  // 1620
    int f_speak_back;  // 1624
    int old_companion_status;  // 1628
    int co_current_function;  // 1632
    int i_interdit_action;  // 1636
    int i_interdit_action_old;  // 1640
    int __am_recieved_Dimension1;  // 1644
    tMessage am_recieved[5];  // 1648
    int i_msg_nb;  // 1908
    int des_i_type_acteur;  // 1912
    int des_o_acteur_suividesyeux;  // 1916
    Vec3 des_v_hauteur_cible;  // 1920
    int des_i_RT_flags;  // 1932
    float des_f_RT_vitesse_de_rotation;  // 1936
    tMessage m_dial_recieved_msg;  // 1940
    int i_crypt_action;  // 1992
    int i_crypt_val1;  // 1996
    int i_crypt_val2;  // 2000
    int i_crypt_val3;  // 2004
    int i_dialog_mode_ini;  // 2008
    int __ai_conditions_Dimension1;  // 2012
    int ai_conditions[21];  // 2016
    int __at_questions_Dimension1;  // 2100
    tTextGet at_questions[21];  // 2104
    int __at_answers_Dimension1;  // 2272
    tTextGet at_answers[21];  // 2276
    int __ai_consequences_Dimension1;  // 2444
    int ai_consequences[21];  // 2448
    int __ai_grisage_Dimension1;  // 2532
    int ai_grisage[21];  // 2536
    int i_dialog_consequence;  // 2620
    int i_dialog_position;  // 2624
    int __ai_current_positions_Dimension1;  // 2628
    int ai_current_positions[5];  // 2632
    int __ai_start_positions_Dimension1;  // 2652
    int ai_start_positions[5];  // 2656
    float f_dialog_time_start;  // 2676
    int i_attente_action;  // 2680
    float f_rayjump_raydistmarche;  // 2684
    float f_rayjump_raydisttrot;  // 2688
    float f_rayjump_raydistcourse;  // 2692
    float f_rayjump_raydist2marche;  // 2696
    float f_rayjump_raydist2trot;  // 2700
    float f_rayjump_raydist2course;  // 2704
    float f_amplitude_saut_accrochage_d;  // 2708
    float f_amplitude_saut_accrochage_c;  // 2712
    float f_Jump_hauteur_saut_pied;  // 2716
    float f_Jump_hauteur_saut_coude;  // 2720
    float f_Jump_hauteur_saut_rate;  // 2724
    Vec3 v_offset_accrochage;  // 2728
    Vec3 v_offset_accrochage_coudes;  // 2740
    int __ai_expr_bouche_blabla_liste_Dimension1;  // 2752
    int ai_expr_bouche_blabla_liste[32];  // 2756
    int i_expr_bouche_blabla_id;  // 2884
    int i_expr_bouche_auto;  // 2888
    int __f_expr_factor_target_Dimension1;  // 2892
    float f_expr_factor_target[5];  // 2896
    int __f_expr_factor_current_Dimension1;  // 2916
    float f_expr_factor_current[5];  // 2920
    int __f_expr_prog_current_Dimension1;  // 2940
    float f_expr_prog_current[5];  // 2944
    int __f_expr_speed_Dimension1;  // 2964
    float f_expr_speed[5];  // 2968
    int __f_expr_forme_Dimension1;  // 2988
    float f_expr_forme[5];  // 2992
    int __f_expr_flag_Dimension1;  // 3012
    float f_expr_flag[5];  // 3016
    Vec3 macro_regard_aleatoire_v_curr;  // 3036
    Vec3 macro_regard_aleatoire_v_targ;  // 3048
    Vec3 macro_regard_aleatoire_v_inte;  // 3060
    int macro_regard_aleatoire_i_cpt;  // 3072
    int o_module_PlugMainG;  // 3076
    int o_module_mainG;  // 3080
    int o_module_mainD;  // 3084
    int o_module_piedD;  // 3088
    int o_module_piedG;  // 3092
    int o_module_tete;  // 3096
    int o_module_bassin;  // 3100
    int o_module_sac;  // 3104
    int i_giveto_object_id;  // 3108
    int GFX_smoke;  // 3112
    int i_sfx_halo;  // 3116
    float f_sfx_halo_time;  // 3120
    Vec3 v_xb_tete_sight;  // 3124
    int o_xb;  // 3136
    int o_xb_dupl;  // 3140
    float f_sight_lim_down;  // 3144
    float f_sight_lim_up;  // 3148
    float f_sight_lim_angle;  // 3152
    float f_way_time_force_recompute;  // 3156
    int i_snd_decoupe;  // 3160
    float f_bidouille_vol;  // 3164
    int i_fight_fin;  // 3168
    float f_scale_cle;  // 3172
    float f_scale_lim;  // 3176
    int o_cle;  // 3180
    int i_cle_must_desapear;  // 3184
    int i_cle_must_apear;  // 3188
    int o_La_cle;  // 3192
    int o_le_shield;  // 3196
    int i_baffe_frappe;  // 3200
    int o_fragment_ecrou;  // 3204
    int o_fragment_cable;  // 3208
    int o_fragment_carlingue;  // 3212
    int o_fragment_boitier;  // 3216
    int o_fragment_ressort;  // 3220
    int i_rayonshield_on;  // 3224
    float f_rayonshield_time;  // 3228
    float f_rayonshield_delai;  // 3232
    int o_rayonshield_objet_flash;  // 3236
    int o_rayonshield_objet_flash_sal;  // 3240
    int i_sfx_halo_rayonshield_deb;  // 3244
    int i_sfx_halo_rayonshield_fin;  // 3248
    int i_tentacule;  // 3252
    int i_texte_dit;  // 3256
    int i_texte_blesse;  // 3260
    int i_texte_paferreur;  // 3264
    int o_combo_colmap;  // 3268
    int haloG;  // 3272
    float haloG_size;  // 3276
    int haloD;  // 3280
    float haloD_size;  // 3284
    int haloYeuxD;  // 3288
    float haloYeuxD_size;  // 3292
    int haloYeuxG;  // 3296
    float haloYeuxG_size;  // 3300
    float f_time_tir_XB;  // 3304
    int o_tir_XB;  // 3308
    int o_tir_dup_XB;  // 3312
    int o_tir_dup2_XB;  // 3316
    int o_SFX_gaomat;  // 3320
    int i_tir_Yeux;  // 3324
    int GFX_Particules;  // 3328
    int GFX_Halo_PiedG;  // 3332
    int GFX_Halo_PiedD;  // 3336
    int i_flag_SFX_coup_de_cul;  // 3340
    float f_time_SFX_coup_de_cul;  // 3344
    float f_time_SFX_Halo;  // 3348
    float f_delai_roquettes;  // 3352
    int i_HDef;  // 3356
    int i_shape_trame_attente;  // 3360
    int i_talk_propose;  // 3364
    int i_texte_synchro_SAFight;  // 3368
    int i_texte_vienspas;  // 3372
    int i_texte_attendsmoi;  // 3376
    Vec3 v_pos_after_recal;  // 3380
    Vec3 v_pos_after_recal_old;  // 3392
    float f_time_last_fixe;  // 3404
    int o_repousse_moi;  // 3408
    float f_time_attends_moi;  // 3412
    float f_delai_attends_moi;  // 3416
    int i_do_smoke;  // 3420
    float f_time_gaz;  // 3424
    float f_time_gaz_paf;  // 3428
    int o_flash_gaz;  // 3432
    float f_time_smoke;  // 3436
    int GFX_Smoke;  // 3440
    float f_gfx_rate;  // 3444
    int o_SFX_special;  // 3448
    int o_SFX_specialHammer;  // 3452
    int o_shield;  // 3456
    int i_init_frappe;  // 3460
    int __ai_plug_indice_Dimension1;  // 3464
    int ai_plug_indice[5];  // 3468
    int i_gmat_sol;  // 3488
    float f_lightUP_current;  // 3492
    float f_lightUP_target;  // 3496
    float f_lightUP_old;  // 3500
    float f_lightDN_current;  // 3504
    float f_lightDN_target;  // 3508
    float f_lightDN_old;  // 3512
    int i_talk_Karlson;  // 3516
    Vec3 v_fight_pos_offset;  // 3520
    int i_txt_shield_low;  // 3532
    int i_etat_colmap;  // 3536
    int i_etat_marto;  // 3540
    Vec3 v_blend_Hammer;  // 3544
    float f_time_col_bullrush;  // 3556
    int i_auto_libere_crochax;  // 3560
    int __ai_repartition_Dimension1;  // 3564
    int ai_repartition[7];  // 3568
    float f_time_blindage_give;  // 3596
} tScriptStruct_Companion;

typedef struct sScriptStruct_GetObject {
    int des_i_object;  // 0
    int des_o_valide_take;  // 4
    int i_valide_take_alim;  // 8
    float des_f_dist_target;  // 12
    float des_f_dist_action;  // 16
    float des_f_dist_action2;  // 20
    int des_i_sf_valide_take;  // 24
    int des_destroy_after_put;  // 28
    int des_i_saute_dans_linventaire;  // 32
    tTextGet des_t_take;  // 36
    tTextGet des_t_put;  // 44
    int des_i_choix;  // 52
    int des_i_auto_selection;  // 56
    int des_autorise_optimisation_AI;  // 60
    float f_offset_z_max;  // 64
    int i_prio_add;  // 68
    int i_orientation_du_take;  // 72
    float des_f_angle_action;  // 76
    float f_rotation_speed;  // 80
    int des_i_interdire_en_fight;  // 84
    int des_halo;  // 88
    int des_halo_extrude_axe;  // 92
    int des_halo_extrude_sens;  // 96
    float des_halo_radius;  // 100
    float des_halo_extrude;  // 104
    float des_halo_angle;  // 108
    int des_halo_number;  // 112
    int des_halo_couleur;  // 116
    Vec3 des_v_halo_offset;  // 120
    int des_SF_destroy_on_load;  // 132
    int des_SF_inverse_status;  // 136
    int des_SF_active_on_destroy;  // 140
    int des_generer_un_magnet;  // 144
    int des_extra_magnet;  // 148
    float des_f_life_time;  // 152
    int i_bit;  // 156
    int i_activation;  // 160
    float f_time_start;  // 164
    int i_use_event_track;  // 168
    int i_use_event_on_reset;  // 172
    int i_force_use;  // 176
    int i_plus_de_place;  // 180
    float f_time_place;  // 184
    int i_swap_force;  // 188
    int GFX_Halo;  // 192
    float f_angle_halo;  // 196
    int i_idhalo;  // 200
    Vec3 des_v_halo_pos;  // 204
    int i_colhalo;  // 216
    float f_timehalo;  // 220
    int i_senshalo;  // 224
    int i_gen_halo_simple;  // 228
    int spark;  // 232
    int halo;  // 236
    int halob;  // 240
    int haloc;  // 244
    float t_time_spark;  // 248
    float t_delay_spark;  // 252
} tScriptStruct_GetObject;

typedef struct sScriptStruct_MDiskMap {
    int des_i_type;  // 0
    int SF_Mdisk_lu;  // 4
    tTextGet txt_Bt0;  // 8
    tTextGet txt_Bt1;  // 16
    tTextGet txt_Bt2;  // 24
    tTextGet txt_Bt3;  // 32
    tTextGet txt_Bt4;  // 40
    int SF_Context_Lu;  // 48
    int __SF_ai_Context_Dimension1;  // 52
    int SF_ai_Context[4];  // 56
    float des_f_maxdist_droite;  // 72
    float des_f_maxdist_gauche;  // 76
    float des_f_maxdist_haut;  // 80
    float des_f_maxdist_bas;  // 84
    float des_f_focal_min;  // 88
    int des_i_can_change_focal;  // 92
    int des_i_can_pane;  // 96
    int des_i_sfx_level0;  // 100
    int des_i_sfx_level1;  // 104
    int des_i_sfx_level2;  // 108
    int des_i_interface;  // 112
    int des_i_interface_bottom;  // 116
    int des_i_sfx_scanlines;  // 120
    int des_o_camera_AVI;  // 124
    float des_f_duree_avi;  // 128
    Vec3 des_v_position_text;  // 132
    int i_inventaire;  // 144
    int i_forceinventaire;  // 148
    float f_cur_h;  // 152
    float f_cur_v;  // 156
    float f_focal_cur;  // 160
    float f_abstime;  // 164
    int inpause;  // 168
    int des_o_camera_PHOTO;  // 172
    int des_i_max_photos;  // 176
    int __des_ao_text_Dimension1;  // 180
    tTextGet des_ao_text[50];  // 184
    int __des_av_pos_Dimension1;  // 584
    Vec3 des_av_pos[50];  // 588
    int des_i_single_pos;  // 1188
    float des_f_offset;  // 1192
    int des_i_move_smooth;  // 1196
    int des_i_speed_smooth;  // 1200
    int i_cur_photo;  // 1204
    int i_can_smooth;  // 1208
    int sfx_mdisk;  // 1212
    int i_mask_photo;  // 1216
    int i_can_exit;  // 1220
    int i_force_exit;  // 1224
    int i_gfx_ID_01;  // 1228
    float f_gfx_size;  // 1232
    int i_gfx_switch;  // 1236
    Vec3 v_gfx_01;  // 1240
    int photomode;  // 1252
    int autoplay;  // 1256
    Vec3 vmovejoy;  // 1260
    float f_focalinitcamera;  // 1272
    float f_total_time_read;  // 1276
    float f_waitjoyzero;  // 1280
    int i_instance_blabla;  // 1284
    int i_mdisk_deja_lu;  // 1288
    int des_o_phototexture;  // 1292
    int __des_ai_photovalue_Dimension1;  // 1296
    int des_ai_photovalue[50];  // 1300
    int i_curphotovalue;  // 1500
    int des_o_Board;  // 1504
    int des_o_Interface;  // 1508
    int des_o_micro;  // 1512
    int des_o_snd_mdf;  // 1516
    int i_snd_instance;  // 1520
    int i_snd_freq;  // 1524
    float f_snd_volume;  // 1528
    int i_snd_mode;  // 1532
} tScriptStruct_MDiskMap;

typedef struct sScriptStruct_MDiskDrone {
    int wp_of_network;  // 0
    int des_o_plug_wp;  // 4
    int des_o_drone_vert;  // 8
    int des_o_ES_target;  // 12
    int SF_deplacement_done;  // 16
    int i_interdit_pendant_mili_malad;  // 20
    float des_f_distance_talk;  // 24
    float des_f_angle_talk;  // 28
    int __ai_conditions_Dimension1;  // 32
    int __ai_conditions_Dimension2;  // 36
    int ai_conditions[80];  // 40
    int __at_questionsReponses_Dimension1;  // 360
    int __at_questionsReponses_Dimension2;  // 364
    tTextGet at_questionsReponses[40];  // 368
    int __ai_consequences_Dimension1;  // 688
    int __ai_consequences_Dimension2;  // 692
    int ai_consequences[60];  // 696
    int __ai_consequences_plus_Dimension1;  // 936
    int __ai_consequences_plus_Dimension2;  // 940
    int ai_consequences_plus[140];  // 944
    int __ai_grisage_Dimension1;  // 1504
    int ai_grisage[20];  // 1508
    int __at_question_reponse_OK_Dimension1;  // 1588
    tTextGet at_question_reponse_OK[2];  // 1592
    int i_dialogue_on;  // 1608
    tMessage m_dialogue;  // 1612
    int Valide_Effet_1_Question;  // 1664
    int o_papa;  // 1668
    int i_talk_propose;  // 1672
    float f_time_fin_talk;  // 1676
    int i_current_mode;  // 1680
    int i_flag_mdisk_pris;  // 1684
    int i_flag_force_halo;  // 1688
    int GFX_Plasma;  // 1692
    int GFX_Halo_1;  // 1696
    int GFX_Halo_2;  // 1700
    int GFX_Halo_3;  // 1704
    int GFX_Halo_1bis;  // 1708
    int GFX_Halo_2bis;  // 1712
    int GFX_Halo_3bis;  // 1716
    int i_halo_flash_index;  // 1720
    float f_angle;  // 1724
    float f_speed;  // 1728
    float f_blend_speed;  // 1732
    float f_rotation_speed;  // 1736
    Vec3 v_init_pos;  // 1740
    Vec3 v_last_pos;  // 1752
    Vec3 v_virtual_sight;  // 1764
    Vec3 v_last_wp_pos;  // 1776
    int o_next_wp;  // 1788
    int n_net;  // 1792
    int i_snd_loop;  // 1796
    float f_snd_loop_volref;  // 1800
    float f_snd_loop_freqref;  // 1804
    tMessage m_msg;  // 1808
} tScriptStruct_MDiskDrone;

typedef struct sScriptStruct_SFXHaloSimple {
    int des_o_gaomat;  // 0
    float des_f_Radius;  // 4
    float des_f_Offset;  // 8
    int des_c_color;  // 12
    int des_i_canmove;  // 16
    int des_i_Type;  // 20
    float des_f_Extrude;  // 24
    float des_f_Angle;  // 28
    int des_i_Number;  // 32
    int des_i_clignote;  // 36
    float des_f_time_init;  // 40
    float des_f_freq;  // 44
    float des_f_speed_clignote;  // 48
    float f_time_cur;  // 52
    int i_idhalo;  // 56
    int i_mode;  // 60
    int c_color;  // 64
    int i_flag_play_snd;  // 68
} tScriptStruct_SFXHaloSimple;

typedef struct sScriptStruct_SFXSecundo {
    int des_secundo;  // 0
    int des_SF;  // 4
    int des_ES;  // 8
    int des_BV;  // 12
    int i_flag_ON;  // 16
    int GFX_Halo;  // 20
    int i_flag_move_done;  // 24
    int i_flag_manager_done;  // 28
    int i_test_dist;  // 32
    int c_ambiant_2;  // 36
    float f_time_start;  // 40
    float f_time_gen;  // 44
    float f_time_death;  // 48
    float f_delay_gen;  // 52
    float f_secundo_dist;  // 56
    float f_rotation_speed;  // 60
    float f_ext_light_factor;  // 64
    float f_light_factor;  // 68
    float f_ext_local_alpha;  // 72
    float f_global_alpha;  // 76
    float f_local_alpha;  // 80
    float f_MAT_MT_U_pos;  // 84
    float f_MAT_MT_V_pos;  // 88
    Vec3 v_final_axe;  // 92
    int o_bassin;  // 104
    int o_secundo_gmat;  // 108
    int __ao_me_Dimension1;  // 112
    int ao_me[20];  // 116
    int o_dest_pos;  // 196
    int one_shot;  // 200
    int i_done;  // 204
} tScriptStruct_SFXSecundo;

typedef struct sScriptStruct_Switch {
    int des_i_on;  // 0
    float des_f_time_on;  // 4
    int des_i_carte_d_access;  // 8
    int des_SF_on_off;  // 12
    int des_o_target;  // 16
    int des_generer_un_magnet;  // 20
    int i_change_item;  // 24
    int o_acteur_capa;  // 28
    int i_capa_a_changer;  // 32
    int des_o_actor_who_allow_it;  // 36
    int i_valide_courant_aussi;  // 40
    int des_i_SF_mecanisme_en_fonctio;  // 44
    int des_i_interdire_en_fight;  // 48
    int i_interdire_si_comp_KO;  // 52
    int des_i_SF_acces_a_l_inter;  // 56
    int des_o_ascenseur;  // 60
    int des_i_etage;  // 64
    int des_i_vala_allowed;  // 68
    int o_vala_visee;  // 72
    int des_i_acteur_choix;  // 76
    float des_f_vala_angle;  // 80
    float des_f_vala_distmax;  // 84
    float des_f_agit_distmax;  // 88
    float des_f_distmax_sally;  // 92
    Vec3 v_coup_cul_offset_sol;  // 96
    int Interdiction_pendant_fight;  // 108
    int des_i_auto;  // 112
    int des_i_action_type;  // 116
    int des_i_action_type_peyj;  // 120
    int des_i_action_type_mili;  // 124
    int des_i_action_type_domz;  // 128
    int des_i_action_type_XB;  // 132
    float f_delai_attente_apres_action;  // 136
    float des_f_vala_offset_cible;  // 140
    float des_f_vala_angle_belier;  // 144
    float des_f_vala_dist_min;  // 148
    tMessage m_vala;  // 152
    int i_vala_fini;  // 204
    int i_vala_action_sent;  // 208
    Vec3 v_vala_pos;  // 212
    float f_vala_prio;  // 224
    int __ai_action_Dimension1;  // 228
    int ai_action[5];  // 232
    int o_compagnon_qui_agit;  // 252
    int i_trigger_on;  // 256
    float f_start_action;  // 260
    float des_f_offset_vala;  // 264
    int i_prio_action;  // 268
    int o_magnet;  // 272
    int i_actif;  // 276
    float f_time_actif;  // 280
    float des_f_flash_time;  // 284
    int des_i_flash_nb;  // 288
    int des_enterprise_target;  // 292
    int c_base_constante;  // 296
    int c_off_constante;  // 300
    float f_time_flash;  // 304
    int des_sound_mode;  // 308
    float des_delay_sans_son;  // 312
    float des_sound_frequence_countdown;  // 316
    float f_time_sound;  // 320
    float f_sound_delay;  // 324
    int i_sound_pulse;  // 328
    int des_i_SF_allow_special_text;  // 332
    int des_i_sally_text_index;  // 336
    int des_i_peyj_text_index;  // 340
    int des_i_militaire_text_index;  // 344
    float f_time_last_info;  // 348
    int TXT_X2Inter_QdCompagnonAbsent;  // 352
    tMessage m_msg;  // 356
    int i_index_message;  // 408
    float offset_z_regard;  // 412
    int i_flag_bip;  // 416
} tScriptStruct_Switch;

typedef struct sScriptStruct_ZoneDetectActors {
    int i_type_de_test;  // 0
    int i_pivot;  // 4
    int i_test_hierarchy;  // 8
    int des_o_actor_1;  // 12
    int des_o_actor_2;  // 16
    int des_o_actor_3;  // 20
    int des_o_actor_activate;  // 24
    int i_desactive_after;  // 28
    float des_f_deplacement_dist;  // 32
    float des_f_speed;  // 36
} tScriptStruct_ZoneDetectActors;

typedef struct sScriptStruct_SFXOndeDeChocCine {
    int des_c_color;  // 0
    float des_f_delai;  // 4
    float des_f_rayon_max;  // 8
    float des_f_hauteur;  // 12
    float f_speed_coef;  // 16
    float f_blend;  // 20
    float old_factor;  // 24
    int old_color;  // 28
    int old_constantcolor0;  // 32
    int old_constantcolor1;  // 36
    int i_after;  // 40
    float f_transparency_coef;  // 44
} tScriptStruct_SFXOndeDeChocCine;

typedef struct sScriptStruct_SFXOndeDeChoc { // hyllis planetesnél ez van, domz Sangue is ezt használja
    float des_f_rayon_max;  // 0
    float des_f_hauteur;  // 4
    float des_f_speed;  // 8
    int o_sound_bank;  // 12
    int i_snd_loop;  // 16
    float f_snd_freqref;  // 20
    float f_snd_volref;  // 24
    float f_blend;  // 28
    int i_canpaf;  // 32
    float old_factor;  // 36
    int old_color;  // 40
    int old_constantcolor0;  // 44
    int old_constantcolor1;  // 48
    int i_after;  // 52
    float f_transparency_coef;  // 56
} tScriptStruct_SFXOndeDeChoc;

typedef struct sScriptStruct_SFXSpiraleDomz {
    float f_prog;  // 0
    int o_soleil;  // 4
    int o_lune;  // 8
    int o_plafond;  // 12
    int o_trou;  // 16
    int o_lum_moi;  // 20
    int o_eclaire;  // 24
    int __o_position_spirale_Dimension1;  // 28
    int o_position_spirale[10];  // 32
    float old_factor;  // 72
    int old_color;  // 76
    int i_after;  // 80
    float f_transparency_coef;  // 84
    float f_transparency_coef_trou;  // 88
    int GFX_Halo;  // 92
    int i_SFX_baton_Halo1;  // 96
    float f_x;  // 100
    float f_y;  // 104
    float f_z;  // 108
    float f_angle;  // 112
    float f_rad;  // 116
    float f_dist;  // 120
    int i_color;  // 124
    int i_color2;  // 128
    int c_col_soleil;  // 132
    int c_col_lune;  // 136
    float f_time_eclaire;  // 140
    float f_delay_eclaire;  // 144
    int c_col_spiral;  // 148
    float f_speed_apparition;  // 152
    float f_time_attaque;  // 156
    int o_faisceau;  // 160
    float f_dist1;  // 164
    float f_dist2;  // 168
    int i_jingle;  // 172
} tScriptStruct_SFXSpiraleDomz;

typedef struct sScriptStruct_RLIVariator {
    int des_stealth_mode;  // 0
    int des_global_RLI;  // 4
    int des_paf_sensitive;  // 8
    float des_f_transition_delay;  // 12
    float des_f_light_ON_vib;  // 16
    float des_f_light_trans_vib;  // 20
    float des_f_OFF_delay;  // 24
    int des_RLI_ON;  // 28
    int des_RLI_OFF;  // 32
    int __des_o_halo_simple_Dimension1;  // 36
    int des_o_halo_simple[10];  // 40
    int __des_o_dyn_lights_Dimension1;  // 80
    int des_o_dyn_lights[20];  // 84
    int __des_c_dyn_lights_Dimension1;  // 164
    int des_c_dyn_lights[20];  // 168
    int __des_o_capa_switch_Dimension1;  // 248
    int des_o_capa_switch[20];  // 252
    int c_ambiant_on;  // 332
    int c_ambiant2_on;  // 336
    int des_sound_location;  // 340
    float f_angle;  // 344
    float f_time_activation;  // 348
    float f_time_flash;  // 352
    int __ai_backup_color_Dimension1;  // 356
    int ai_backup_color[10];  // 360
    int i_nb_halo_simple;  // 400
    int i_flag_ON;  // 404
    int i_flag_set_light;  // 408
    int i_flash_nb;  // 412
    tMessage m_msg;  // 416
    int c_ambiant_off;  // 468
    int c_ambiant2_off;  // 472
    int i_backup_ambiant;  // 476
    int i_backup_ambiant2;  // 480
    float f_backup_RLI_Factor;  // 484
    int c_backup_RLI_DestColor;  // 488
} tScriptStruct_RLIVariator;

typedef struct sScriptStruct_SFXSun {
    int i_GFX;  // 0
    int i_Compute;  // 4
    int i_Number;  // 8
    int c_Color;  // 12
    int i_Hide;  // 16
    float f_Interval;  // 20
    float f_Time;  // 24
    int des_o_source_mat;  // 28
    int des_i_NoMat;  // 32
    float Balpha;  // 36
    float BLMin;  // 40
    float BLMax;  // 44
    float BWMin;  // 48
    float BWMax;  // 52
    float Salpha;  // 56
    float SLMin;  // 60
    float SLMax;  // 64
    float SWMin;  // 68
    float SWMax;  // 72
    float LRatio;  // 76
    float Distance;  // 80
} tScriptStruct_SFXSun;

typedef struct sScriptStruct_ENVWorldCycle {
    int des_i_numplanete;  // 0
    int des_o_teleport;  // 4
    int des_o_env;  // 8
    int des_o_sun;  // 12
    int des_o_moon;  // 16
    int des_o_moon_light;  // 20
    int des_o_moon_rayon;  // 24
    int des_o_fog;  // 28
    int des_o_sky;  // 32
    Vec3 des_v_center_world;  // 36
    float des_f_distance;  // 48
    int des_o_desactivate;  // 52
    float des_f_Z_jour;  // 56
    float des_f_Z_lever;  // 60
    float des_f_Z_aurore;  // 64
    float des_f_Z_nuit;  // 68
    float des_f_fog_near;  // 72
    float des_f_fog_far;  // 76
    float des_f_fog_near_attaque;  // 80
    float des_f_fog_far_attaque;  // 84
    int __des_ac_jour_Dimension1;  // 88
    int des_ac_jour[10];  // 92
    int __des_ac_coucher_Dimension1;  // 132
    int des_ac_coucher[10];  // 136
    int __des_ac_aurore_Dimension1;  // 176
    int des_ac_aurore[10];  // 180
    int __des_ac_nuit_Dimension1;  // 220
    int des_ac_nuit[10];  // 224
    int __des_ac_jour_attaque_Dimension1;  // 264
    int des_ac_jour_attaque[10];  // 268
    int __des_ac_coucher_attaque_Dimension1;  // 308
    int des_ac_coucher_attaque[10];  // 312
    int __des_ac_aurore_attaque_Dimension1;  // 352
    int des_ac_aurore_attaque[10];  // 356
    int __des_ac_nuit_attaque_Dimension1;  // 396
    int des_ac_nuit_attaque[10];  // 400
    int des_c_moon_light;  // 440
    float des_f_RLI_jour;  // 444
    float des_f_RLI_coucher;  // 448
    float des_f_RLI_aurore;  // 452
    float des_f_RLI_nuit;  // 456
    int des_standard_world_cycle;  // 460
    int des_copy_all_but_colors;  // 464
    int __ac_color_Dimension1;  // 468
    int ac_color[10];  // 472
    int __ac_color_attaque_Dimension1;  // 512
    int ac_color_attaque[10];  // 516
    int __ac_color_final_Dimension1;  // 556
    int ac_color_final[10];  // 560
    int c_moon_light;  // 600
    float f_firstnorm;  // 604
    Vec3 v_normvais;  // 608
    float f_time;  // 620
    float f_vitesse_transition_vers_att;  // 624
    float f_vitesse_transition_retour_v;  // 628
    float f_coef_attaque;  // 632
    Vec3 v_rel_lune_lgt;  // 636
    float f_time_moon;  // 648
    int i_soleil_ok;  // 652
    Vec3 pos_air;  // 656
    Vec3 pos_cam;  // 668
    Vec3 des_v_center_to_repos;  // 680
} tScriptStruct_ENVWorldCycle;

typedef struct sScriptStruct_ActorMouette {
    int des_network;  // 0
    float des_f_speed;  // 4
    int des_o_follow;  // 8
    int des_o_coucher;  // 12
    float des_f_maxfollow;  // 16
    int des_i_always_visible;  // 20
    float des_f_zoom;  // 24
    float des_f_friction;  // 28
    int o_follow;  // 32
    int i_attwp;  // 36
    float f_delayattwp;  // 40
    float f_sinfactor;  // 44
    float f_speedfactor;  // 48
    int i_state;  // 52
    float f_speed;  // 56
    int cur_wp;  // 60
    Vec3 v_postogo;  // 64
    float f_delaybeg;  // 76
    int __af_angleailes_Dimension1;  // 80
    float af_angleailes[4];  // 84
    float f_delay;  // 100
    int i_forcehide;  // 104
    Vec3 v_posfollow;  // 108
    int i_ishere;  // 120
    int i_washere;  // 124
    int i_MapEtage;  // 128
    float f_snd_delay;  // 132
    float f_snd_time;  // 136
    float f_time_leve;  // 140
    float f_time_couche;  // 144
} tScriptStruct_ActorMouette;

typedef struct sScriptStruct_SFXFunPoleine {
    float des_f_freq;  // 0
    float des_f_sqrdist;  // 4
} tScriptStruct_SFXFunPoleine;

typedef struct sScriptStruct_WorldCycleInit {
    int des_i_numplanete;  // 0
    int des_i_specialflag1;  // 4
    int des_i_specialflag2;  // 8
    int des_i_specialflag3;  // 12
    int des_i_specialflag4;  // 16
    float des_f_specialtime1;  // 20
    float des_f_specialtime2;  // 24
    float des_f_specialtime3;  // 28
    float des_f_specialtime4;  // 32
    Vec3 des_v_center_world;  // 36
    int des_o_cycle;  // 48
} tScriptStruct_WorldCycleInit;

typedef struct sScriptStruct_SFXFlammes {
    int des_o_BV;  // 0
    int des_o_snap_actor;  // 4
    int des_activation;  // 8
    int des_actif_si;  // 12
    float des_f_detail;  // 16
    float des_f_gravity;  // 20
    Vec3 des_v_friction;  // 24
    float des_f_wind;  // 36
    float des_f_wind_current_speed_coef;  // 40
    int des_rand_BV;  // 44
    int des_volume;  // 48
    float des_X;  // 52
    float des_Y;  // 56
    float des_Z;  // 60
    float des_f_ON_size_min;  // 64
    float des_f_ON_size_max;  // 68
    float des_f_ON_speed_min;  // 72
    float des_f_ON_speed_max;  // 76
    float f_time_phase1;  // 80
    float f_time_phase2;  // 84
    int des_c_ON_flamme_1;  // 88
    int des_c_ON_flamme_2;  // 92
    float des_f_OFF_size_min;  // 96
    float des_f_OFF_size_max;  // 100
    float des_f_OFF_speed_min;  // 104
    float des_f_OFF_speed_max;  // 108
    int des_c_OFF_flamme_1;  // 112
    int des_c_OFF_flamme_2;  // 116
    int des_smoke;  // 120
    float des_f_smoke_gravity;  // 124
    int des_paf;  // 128
    float des_f_traction;  // 132
    int des_i_degats;  // 136
    float des_f_delai_entre_paf_degats;  // 140
    float des_dist_activation;  // 144
    int GFX;  // 148
    int GFX_Smoke;  // 152
    int i_flag_Burn;  // 156
    float f_time_start;  // 160
    Vec3 v_offset;  // 164
    int o_me;  // 176
    int i_snd_flamme;  // 180
    int i_snd_0;  // 184
    int i_snd_1;  // 188
    float f_time_ref_paf;  // 192
} tScriptStruct_SFXFlammes;

typedef struct sScriptStruct_SFXBouclierPhare {
    int __des_ai_eclaire_Dimension1;  // 0
    int des_ai_eclaire[10];  // 4
    int __i_idhalo_Dimension1;  // 44
    int i_idhalo[10];  // 48
    int __i_idhalo1_Dimension1;  // 88
    int i_idhalo1[10];  // 92
    int __des_v_pos_Dimension1;  // 132
    Vec3 des_v_pos[10];  // 136
    int c_col_plaques;  // 256
    int c_col_plaques1;  // 260
    float f_time;  // 264
    int i_cine;  // 268
    int i_after;  // 272
    int des_o_colmap_fermee;  // 276
    int des_o_colmap_ouverte;  // 280
    int des_o_haut_phare;  // 284
    int des_o_jauge;  // 288
    int des_o_texte;  // 292
    float des_f_time0;  // 296
    float des_f_time1;  // 300
    float des_f_time2;  // 304
    float des_f_time3;  // 308
    float des_f_timed;  // 312
    int des_o_dome;  // 316
    int old_factor;  // 320
    int old_color;  // 324
    float f_transparency_coef;  // 328
    float m_time0;  // 332
    float m_time1;  // 336
    float m_time2;  // 340
    float m_time3;  // 344
} tScriptStruct_SFXBouclierPhare;

typedef struct sScriptStruct_SnapOrienteOnBone { // "combat stick" (twig) at the intro
    int des_perso_a_suivre;  // 0
    int des_bone_a_suivre;  // 4
    int i_transit;  // 8
    float des_f_vitesse;  // 12
    int des_suivre_taille;  // 16
    Vec3 v_pos_start;  // 20
    Vec3 v_sight_start;  // 32
    Vec3 v_bank_start;  // 44
    float f_temps;  // 56
} tScriptStruct_SnapOrienteOnBone;

typedef struct sScriptStruct_MultiSnapOnBone { // Jade's hands at the intro 
    int des_i_objet_anime;  // 0
    int des_i_max;  // 4
    int __des_ao_objets_source_Dimension1;  // 8
    int des_ao_objets_source[16];  // 12
    int __des_ai_canaux_source_Dimension1;  // 76
    int des_ai_canaux_source[16];  // 80
    int des_ao_objet_dest;  // 144
    int __des_ai_canaux_dest_Dimension1;  // 148
    int des_ai_canaux_dest[16];  // 152
} tScriptStruct_MultiSnapOnBone;

typedef struct sScriptStruct_GSTCombat {
    int o_ZONE_Fight;  // 0
    int SF_INIT;  // 4
    int SF_END;  // 8
    float f_time_delai_avant_debut;  // 12
    int o_chain_script;  // 16
    int i_nb_NMI_restant_pour_activer;  // 20
    int __Actors_Dimension1;  // 24
    int Actors[30];  // 28
    int i_active_actor_on_init;  // 148
    int i_invisible_actor_on_init;  // 152
    int i_script_indice;  // 156
    int Script_nb_de_refresh;  // 160
    float Script_4_delai_entre_attaque;  // 164
    float Script_4_distance_a_Sally;  // 168
    float Script_4_vitesse_de_deplaceme;  // 172
    float Script_4_angle_entre_Worms;  // 176
    float Script_9_delai_entre_attaque;  // 180
    float Script_10_delai_entre_attaque;  // 184
    float Script_11_delai_entre_attaque;  // 188
    int Scr_Actor_ID;  // 192
    int Scr_Actor_Nbr;  // 196
    int __Scr_Slot_Dimension1;  // 200
    int Scr_Slot[10];  // 204
    int __Scr_Conditions_Dimension1;  // 244
    int Scr_Conditions[10];  // 248
    int __Scr_ConditionParam_Dimension1;  // 288
    int __Scr_ConditionParam_Dimension2;  // 292
    float Scr_ConditionParam[30];  // 296
    int __Scr_ConditionOn_Dimension1;  // 416
    int Scr_ConditionOn[10];  // 420
    int __Scr_ScriptFunction_Dimension1;  // 460
    int Scr_ScriptFunction[50];  // 464
    int __Scr_ScriptParam_Dimension1;  // 664
    int __Scr_ScriptParam_Dimension2;  // 668
    float Scr_ScriptParam[150];  // 672
    int __Scr_Macro_Dimension1;  // 1272
    int __Scr_Macro_Dimension2;  // 1276
    int Scr_Macro[20];  // 1280
    int Scr_Indice;  // 1360
    int Scr_Indice_Old;  // 1364
    int Scr_Label_Indice;  // 1368
    int Scr_Condition_Indice;  // 1372
    int Scr_Condition_Nb;  // 1376
    int __Scr_Labels_Dimension1;  // 1380
    int Scr_Labels[20];  // 1384
    int __Scr_Function_Dimension1;  // 1464
    int Scr_Function[20];  // 1468
    int __Scr_FunctionCondition_Dimension1;  // 1548
    int Scr_FunctionCondition[5];  // 1552
    int __Scr_TimeWait_Dimension1;  // 1572
    int Scr_TimeWait[30];  // 1576
    int AA_Indice;  // 1696
    int AA_ActionPrecedente;  // 1700
    int __AE_Action1_effectue_Dimension1;  // 1704
    int AE_Action1_effectue[10];  // 1708
    int AE_nb;  // 1748
    int __i_Macro_Ind_Dimension1;  // 1752
    int i_Macro_Ind[10];  // 1756
    int EMA_indice;  // 1796
    float tf_time_spe_1;  // 1800
    int o_CS_actor;  // 1804
    int i_CS_phase;  // 1808
    int i_label;  // 1812
    int i_test;  // 1816
    int i_choix_perso_done;  // 1820
    float f_time_wait;  // 1824
    int Scr_nb_refresh;  // 1828
    int Scr_nb_mort;  // 1832
} tScriptStruct_GSTCombat;

typedef struct sScriptStruct_PNJSpectre {
    int des_bras_droit;  // 0
    int des_bras_gauche;  // 4
    int WP_pos_combat;  // 8
    int WP_init_combat;  // 12
    int o_Monstro;  // 16
    float f_deplacement_speed_max;  // 20
    int i_depart;  // 24
    int i_position_start;  // 28
    int i_ZDM_ON_OFF;  // 32
    int SF_ON;  // 36
    int SF_OFF;  // 40
    int SF_MORT;  // 44
    float f_zoom;  // 48
    float f_zoom_min;  // 52
    float f_zoom_max;  // 56
    int se_reactive_tant_que_pas_capa;  // 60
    int o_actor_capaswitch;  // 64
    int o_actor_a_reactiver_a_sa_mort;  // 68
    int i_NMI;  // 72
    float f_Compagnon_delai_attaque;  // 76
    float f_Compagnon_delai_touche;  // 80
    float i_Compagnon_attaque;  // 84
    int i_mode;  // 88
    int i_paf_nb_coup;  // 92
    int i_paf_type;  // 96
    int Nb_Coup;  // 100
    int i_fight_on;  // 104
    int i_flag_order_old;  // 108
    int i_flag_order;  // 112
    int i_mode_avt_paf;  // 116
    int i_apparition_commence;  // 120
    int i_apparition_init;  // 124
    float f_z_apparition;  // 128
    float f_altitude_offset;  // 132
    float f_distance_actuelle;  // 136
    float f_time_last_attak;  // 140
    float f_time_paf_sally;  // 144
    int o_gamin_snap;  // 148
    int o_debrit;  // 152
    int o_NMI;  // 156
    int o_me;  // 160
    Vec3 v_paf_sens_coup;  // 164
    Vec3 v_paf_speed;  // 176
    tMessage t_mes;  // 188
    int acteur_musique_fight;  // 240
    int des_i_etage;  // 244
    int o_body;  // 248
    int o_snap;  // 252
    Vec3 v_last_pos;  // 256
    Vec3 v_grav_pos;  // 268
    Vec3 v_grav_speed;  // 280
    Vec3 v_speed;  // 292
    int i_snd_deplace_id;  // 304
    float f_snd_deplace_freq;  // 308
    int halo;  // 312
    float f_pos_dist;  // 316
    float f_pos_dt;  // 320
    float f_time;  // 324
    float f_retard;  // 328
    float f_blend_current;  // 332
    float f_frappe_dernier;  // 336
    int i_key_brasG;  // 340
    int i_key_brasD;  // 344
    int i_key_mask;  // 348
    int Partie_perdue;  // 352
    int i_brasG_on;  // 356
    int i_brasD_on;  // 360
    int i_color_membrane;  // 364
    int i_apparition_etape;  // 368
    int i_sens_paf;  // 372
    int i_attaque_on;  // 376
    float f_time_debut_attaque;  // 380
    float f_dt;  // 384
    float f_time_wait;  // 388
    float f_vitre_local_alpha;  // 392
    float f_local_tresh_alpha;  // 396
    int i_snap_le_gamin;  // 400
    float f_time_gen_XMEN;  // 404
    int des_fuite_WP;  // 408
    float f_color_duration;  // 412
    int des_helper_libe;  // 416
    int i_replique;  // 420
    float f_speed_rot;  // 424
    float f_blend_color_membrane;  // 428
    float f_dist_max;  // 432
    int i_jattaque_le_dernier_qui_mef;  // 436
    int i_non_selectionnable;  // 440
    Vec3 tag_vectorpos;  // 444
    Vec3 tag_vectorsight;  // 456
    int tag_flag;  // 468
    int tag_value;  // 472
    int tag_change_value;  // 476
    int tag_type;  // 480
    int tag_change_type;  // 484
    int tag_mode;  // 488
    int tag_change_mode;  // 492
    int tag_etage;  // 496
    int tag_etage_force;  // 500
    int __tag_2x2dyn_pos_Dimension1;  // 504
    Vec3 tag_2x2dyn_pos[4];  // 508
    float tag_size;  // 556
    float tag_change_size;  // 560
    Vec3 tag_change_free_pos1;  // 564
    Vec3 tag_free_pos1;  // 576
    Vec3 tag_change_free_pos2;  // 588
    Vec3 tag_free_pos2;  // 600
    int GFX_Halo;  // 612
    float f_xmen_time;  // 616
    float f_xmen_delay;  // 620
    float f_halo_size;  // 624
    float f_halo_first_delay;  // 628
    float f_halo_time;  // 632
    int i_xmen_nbr;  // 636
    int f_halo_size_up;  // 640
    float f_time_tresh_start;  // 644
    int i_auto_color_membrane;  // 648
    int o_xmen_father;  // 652
    int i_xmen_father_bone_num;  // 656
} tScriptStruct_PNJSpectre;

typedef struct sScriptStruct_PNJSectreBras {
    float des_bras;  // 0
    int des_papa;  // 4
    int __GFX_Halo_Dimension1;  // 8
    int GFX_Halo[5];  // 12
    int GFX_Halo_Bras;  // 32
    int GFX_Part;  // 36
    int i_nb_rebond;  // 40
    int i_global_tresh_alpha;  // 44
    int i_flag_dyn_on;  // 48
    float f_time_start;  // 52
    float f_local_tresh_alpha;  // 56
    int o_bras;  // 60
    int o_avant_bras;  // 64
    int o_main;  // 68
    int o_gmat_gao;  // 72
    int des_i_numbones;  // 76
    int __ao_obj_Dimension1;  // 80
    int ao_obj[3];  // 84
    int __af_lenos_Dimension1;  // 96
    float af_lenos[3];  // 100
} tScriptStruct_PNJSectreBras;

typedef struct sScriptStruct_SFXColPagCam {
    int des_o_pag1;  // 0
    int des_o_pag2;  // 4
    int des_o_brume;  // 8
    float f_time;  // 12
    float f_rand;  // 16
    int i_mode;  // 20
} tScriptStruct_SFXColPagCam;

typedef struct sScriptStruct_MonstroplanteGamin {
    int o_z_ground;  // 0
    int o_target;  // 4
    int i_freq;  // 8
} tScriptStruct_MonstroplanteGamin;

typedef struct sScriptStruct_DetectOTime {
    float f_Heure_Depart;  // 0
    float f_Heure_Fin;  // 4
    int SF_Fuseau_Horaire_Valide;  // 8
    int i_fuseau_courant_valide;  // 12
} tScriptStruct_DetectOTime;

typedef struct sScriptStruct_DetectO4SF {
    int SF_1;  // 0
    int SF_2;  // 4
    int SF_3;  // 8
    int SF_4;  // 12
    int des_i_only_vrai;  // 16
    int SF_result_a_1;  // 20
    int SF_result_a_0;  // 24
    float f_every;  // 28
    float f_time;  // 32
} tScriptStruct_DetectO4SF;

typedef struct sScriptStruct_MoveWithMe {
    int des_o_followed;  // 0
    int des_o_sky;  // 4
    Vec3 v_lastpos;  // 8
} tScriptStruct_MoveWithMe;

typedef struct sScriptStruct_PlaneteRotation {
    int des_i_numplanete;  // 0
    int des_o_env;  // 4
} tScriptStruct_PlaneteRotation;

typedef struct sScriptStruct_IndiceJeuLocal {
    int des_i_companion;  // 0
    int des_i_niveau;  // 4
    int __ai_conditions_Dimension1;  // 8
    int __ai_conditions_Dimension2;  // 12
    int ai_conditions[20];  // 16
    int __at_questionsReponses_Dimension1;  // 96
    int __at_questionsReponses_Dimension2;  // 100
    tTextGet at_questionsReponses[10];  // 104
    int __ai_grisage_Dimension1;  // 184
    int ai_grisage[5];  // 188
} tScriptStruct_IndiceJeuLocal;

typedef struct sScriptStruct_SFX_HaloDinamique {
    int des_mode;  // 0
    int des_o_actor_capa;  // 4
    int des_i_capa_test;  // 8
    int des_i_SF_je_suis_la;  // 12
    float des_f_delai_ON_min;  // 16
    float des_f_delai_ON_max;  // 20
    float des_f_delai_OFF_min;  // 24
    float des_f_delai_OFF_max;  // 28
    float des_f_delai_fade_in;  // 32
    float des_f_delai_fade_out;  // 36
    int des_halo_mode;  // 40
    int des_o_gmat_gao;  // 44
    int des_i_gmat_nb;  // 48
    int des_i_halo_type;  // 52
    int des_c_color_OFF;  // 56
    int des_c_color_ON_1;  // 60
    int des_c_color_ON_2;  // 64
    float des_f_freq;  // 68
    float des_f_size;  // 72
    float des_f_extraction;  // 76
    float des_f_size_init;  // 80
    float des_f_size_speed;  // 84
    float des_f_longueur_rays;  // 88
    float des_f_angle_rays;  // 92
    int des_i_nombre_rays;  // 96
    int des_perso_a_suivre;  // 100
    int des_bone_a_suivre;  // 104
    float f_time;  // 108
    float f_delai;  // 112
    float f_angle;  // 116
    float des_f_light_near;  // 120
    float des_f_light_far;  // 124
    float f_light_near_c;  // 128
    float f_light_far_c;  // 132
    int i_flag_ON;  // 136
    int GFX_Halo;  // 140
    int c_current_color;  // 144
    float f_size;  // 148
} tScriptStruct_SFX_HaloDinamique;

typedef struct sScriptStruct_MaterialModificator {
    int des_o_actor_material;  // 0
    int des_i_method;  // 4
    int des_i_typeeffet;  // 8
    int des_i_start_alphavalue;  // 12
    int des_i_end_alphavalue;  // 16
    int des_c_start_color;  // 20
    int des_c_end_color;  // 24
    int des_i_No_Mat;  // 28
    int des_i_No_Layer;  // 32
    float des_f_duree;  // 36
    float des_f_duree_Off;  // 40
    int des_i_effetgeneral;  // 44
    int des_i_jemetue;  // 48
    int i_valeur;  // 52
    int i_sauve_valeur;  // 56
    float f_coeff;  // 60
    float f_temps;  // 64
    float f_temps_Off;  // 68
    int c_temp_color;  // 72
    int c_sauve_color;  // 76
} tScriptStruct_MaterialModificator;

typedef struct sScriptStruct_Ondulator {
    Vec3 des_v_amplitude;  // 0
    float des_f_periode;  // 12
    int des_i_type;  // 16
    Vec3 v_last_amp;  // 20
    Vec3 v_prev_pos;  // 32
    float f_angle;  // 44
} tScriptStruct_Ondulator;

typedef struct sScriptStruct_PNJMonstroPlante {
    int SF_debut;  // 0
    int i;  // 4
    int o_tentacule;  // 8
    int __o_low_Dimension1;  // 12
    int o_low[20];  // 16
    int i_low_indice;  // 96
    int i_low_indice_max;  // 100
    int i_phase;  // 104
    float f_prog;  // 108
    int i_start;  // 112
    int o_objet_network;  // 116
    int o_objet_network_bas;  // 120
    int n_list_of_pos;  // 124
    int n_list_of_pos_bas;  // 128
    int __o_tentacules_Dimension1;  // 132
    int o_tentacules[10];  // 136
    float f_time_generate;  // 176
    int i_nbr_de_tentacule_simultane;  // 180
    int i_nbr_de_tentacule_killed;  // 184
    int i_nbr_de_tentacule_generated;  // 188
    int i_tentacule_paffed;  // 192
    int i_generate;  // 196
    int i_compteur;  // 200
    float f_time_sally;  // 204
    int o_gros_pere;  // 208
    int __o_gamin_Dimension1;  // 212
    int o_gamin[15];  // 216
    int o_perle;  // 276
    int o_perle_dist;  // 280
    int i_vulnerable;  // 284
    float f_energie;  // 288
    float f_offset_z;  // 292
    int c_saverli;  // 296
    float f_savefactor;  // 300
    float f_factor;  // 304
    int i_FM;  // 308
    int i_FM_enemy_index;  // 312
    int i_FM_je_suis_deja_dans_le_FM;  // 316
    int i_FM_flag_dangereux;  // 320
    int i_FM_flag_invulnerable;  // 324
    int i_FM_flag_non_selectionnable;  // 328
    int o_FM_mon_ennemi;  // 332
    int i_FM_des_flags;  // 336
    int i_nbr_opposant_max;  // 340
    int i_attaque_aussi_compagnon_iso;  // 344
    int o_qui_je_detecte;  // 348
    int o_zone_de_combat_limite;  // 352
    float f_fight_distance;  // 356
    float f_distance_react;  // 360
    float f_Energie_courante;  // 364
    float f_Energie_max;  // 368
    float f_Shield_max;  // 372
    int i_SallyFight_ActionDisable;  // 376
    float f_delay_generate;  // 380
    int SF_je_suis_mort;  // 384
    int o_cine_2blob;  // 388
    int o_gamin_target;  // 392
    int o_gamin_target2;  // 396
    int o_gamin_wait;  // 400
    int o_socle_bas;  // 404
    int i_peyj_pris;  // 408
    int des_o_arrivee_peyj;  // 412
    int i_nb_sequence;  // 416
    int o_musique_phase_1;  // 420
    int o_script_test;  // 424
    int i_plan_cut;  // 428
    int i_last_fm;  // 432
    int o_txt_cine;  // 436
    int i_cpt_peyjpeyj;  // 440
    int o_last_mort;  // 444
    int i_FX_mode;  // 448
    float i_FX_vol;  // 452
    int i_FX_delay;  // 456
    int i_FX_feed;  // 460
    float f_SFX_Vol;  // 464
    float f_SFX_Vol_Ref;  // 468
    int i_sf;  // 472
    int i_old_fight_nbr;  // 476
    int i_nb_spectre;  // 480
    int i_step;  // 484
    int i_counter;  // 488
    float f_memo_contrast;  // 492
    float f_memo_bright;  // 496
    float f_cur_contrast;  // 500
    float f_cur_bright;  // 504
} tScriptStruct_PNJMonstroPlante;

typedef struct sScriptStruct_PNJMonstroPlanteSocle {
    int des_i_type;  // 0
    int o_perle;  // 4
    int o_perle_dist;  // 8
    int o_monstro_plante;  // 12
    int o_prison_1;  // 16
    int o_tige;  // 20
    int o_ref;  // 24
    int o_pag;  // 28
    int o_faisceau;  // 32
    int o_pag_tir;  // 36
    int o_vala;  // 40
    int o_txt_peyj;  // 44
    int o_musique_fight;  // 48
    int o_checkpoint_bas;  // 52
    int o_zde_bas;  // 56
    int o_text_intro;  // 60
    int o_texte_daijo;  // 64
    int i_nb_paf;  // 68
    int i_gen_mat;  // 72
    float f_morph;  // 76
    float f_dest_morph;  // 80
    float f_time_debut_libe;  // 84
    int i_prison_x_sortie;  // 88
    int i_paf;  // 92
    float f_time_paf;  // 96
    int i_num_materia;  // 100
    int i_meurt;  // 104
    int i_sandwich;  // 108
    float f_time_wait_prison;  // 112
    int i_plan_mamama;  // 116
    float f_time_sa;  // 120
    int i_shoot;  // 124
    float f_time_shoot;  // 128
    Vec3 v_shoot_end;  // 132
    Vec3 v_shoot_dir;  // 144
    float f_delay_rayon_sans_paf;  // 156
    int o_main_suivi;  // 160
    int i_shoot_peyj;  // 164
    int i_plan_first_sw;  // 168
    float f_debut_sand;  // 172
    int i_force_shoot_peyj;  // 176
    int i_plan_cut;  // 180
    int i_plan_peyj;  // 184
    float f_time_vala;  // 188
    int __o_net_Dimension1;  // 192
    int o_net[100];  // 196
    int i_net;  // 596
    float f_time_cam_peyj;  // 600
    float f_z_move;  // 604
    float f_dist_move;  // 608
    int i_gfx_halo_tete;  // 612
    float f_time_mort;  // 616
    int f_time_text_prison;  // 620
    int i_play_sound;  // 624
    int i_fight_on;  // 628
    float f_time_mini_paf;  // 632
    int i_mini_paf;  // 636
    int i_count_mini;  // 640
    int o_flash;  // 644
    int i_count_tot;  // 648
    int i_paf_type;  // 652
    int i_snd_prepa_tir;  // 656
    float f_volume_prepa;  // 660
    int i_snd_tir;  // 664
    float f_snd_vol_tir;  // 668
    int i_mode_txt_paf;  // 672
    float f_time_sp;  // 676
    int i_sound_descend;  // 680
    float f_morph_tete;  // 684
    int i_accessoir;  // 688
    int i_first_socle;  // 692
    float f_time_socle;  // 696
    int i_paf_ext;  // 700
    int i_mini_paf1;  // 704
    float f_time_mini_paf1;  // 708
    float f_first_wait;  // 712
    float f_duree_paf;  // 716
    float f_morph_tete_target;  // 720
    int o_prefetch;  // 724
    int halo;  // 728
    int halo_tete;  // 732
} tScriptStruct_PNJMonstroPlanteSocle;

typedef struct sScriptStruct_PNJMonstroPlanteTentacule {
    int i_sally;  // 0
    int i_peyj;  // 4
    int i_gamin;  // 8
    int o_pag;  // 12
    int i_phase;  // 16
    float f_offset_z;  // 20
    int i_SallyFight_ActionDisable;  // 24
    float f_grav;  // 28
    int o_papa;  // 32
    int haloG;  // 36
    float haloG_size;  // 40
    float i_not_dead;  // 44
    float f_time;  // 48
    int i_paf;  // 52
    float t_time_paf;  // 56
    float f_time_rentre;  // 60
    int o_gros_pere;  // 64
    int o_camera;  // 68
    float f_speed_angle;  // 72
    int __ao_overlap_actor_Dimension1;  // 76
    int ao_overlap_actor[10];  // 80
    int des_i_numbones;  // 120
    float des_f_length_last_bone;  // 124
    float f_tentacule_length;  // 128
    float f_length_coef;  // 132
    Vec3 v_scale;  // 136
    int i_flag_snap;  // 148
    int o_queue;  // 152
    float f_time_snap;  // 156
    int o_snap_actor;  // 160
    Vec3 v_snap_actor_offset;  // 164
    float f_angle_max;  // 176
    int i_kick;  // 180
    float f_scale;  // 184
    int __af_lenos_Dimension1;  // 188
    float af_lenos[20];  // 192
    float f_sight_inertie;  // 272
    float f_banking_inertie;  // 276
    float f_angle_sight;  // 280
    float f_angle_banking;  // 284
    float f_angle_sight_speed;  // 288
    float f_angle_banking_speed;  // 292
    float f_sin_angle_sight;  // 296
    float f_sin_angle_banking;  // 300
    float f_sight_amplitude;  // 304
    float f_banking_amplitude;  // 308
    int __ao_obj_Dimension1;  // 312
    int ao_obj[20];  // 316
    int __av_bone_pos_Dimension1;  // 396
    Vec3 av_bone_pos[20];  // 400
    int i_kick_nbr;  // 640
    float f_delta;  // 644
    int i_sally_chope;  // 648
    float f_time_debut;  // 652
    int i_gen_materia;  // 656
    int o_socle_bas;  // 660
    float f_time_app;  // 664
    int i_idhalo;  // 668
    float f_rayon_halo;  // 672
    float f_time_pag;  // 676
    float f_time_piques;  // 680
    Vec3 v_paf_dir;  // 684
    int i_snd_peyj_chope;  // 696
    int o_flash;  // 700
    float f_time_touche;  // 704
    int i_ok_2;  // 708
    int i_anim_att;  // 712
} tScriptStruct_PNJMonstroPlanteTentacule;

typedef struct sScriptStruct_DetectOEmails {
    int __i_SF_activation_de_lEmail_Dimension1;  // 0
    int __i_SF_activation_de_lEmail_Dimension2;  // 4
    int i_SF_activation_de_lEmail[240];  // 8
    int __i_SF_activation_delay_Dimension1;  // 968
    float i_SF_activation_delay[60];  // 972
    int ti_loop;  // 1212
    int ti_loop2;  // 1216
    int ti_NbMails;  // 1220
    int ti_SF;  // 1224
    int ti_note;  // 1228
    int ti_note_array_id;  // 1232
} tScriptStruct_DetectOEmails;

typedef struct sScriptStruct_SFXSAllyAddAno {
    float f_coef_opacite;  // 0
    float f_noise;  // 4
    float old_factor;  // 8
    int old_color;  // 12
    int i_after;  // 16
    int haloG;  // 20
    float haloG_size;  // 24
} tScriptStruct_SFXSAllyAddAno;

typedef struct sScriptStruct_CinePerleDansLeSac {
    int i_State;  // 0
    float f_Timer;  // 4
    int o_Cine;  // 8
    int o_Main;  // 12
} tScriptStruct_CinePerleDansLeSac;

typedef struct sScriptStruct_ZoneOccupationTerrain {
    int SF_Activation;  // 0
    int SF_Desactivation;  // 4
    int Mode;  // 8
    int des_i_qui;  // 12
    int des_i_nbWps;  // 16
    int __des_o_tabWps_Dimension1;  // 20
    int des_o_tabWps[5];  // 24
    float des_f_delai;  // 44
    float f_time;  // 48
    int i_capa_out;  // 52
    int o_Actor_Test;  // 56
    int i_Actor_Test;  // 60
    int i_transfered;  // 64
} tScriptStruct_ZoneOccupationTerrain;

typedef struct sScriptStruct_ObjetPoussable {
    int des_i_type_magnet;  // 0
    int des_i_VertPasVert;  // 4
    int des_i_RougePasRouge;  // 8
    int des_i_TailleX;  // 12
    int des_i_TailleY;  // 16
    int __ao_magnets_Dimension1;  // 20
    int ao_magnets[30];  // 24
    int i_index;  // 144
    int i_nombre_magnets;  // 148
    Vec3 tag_vectorpos;  // 152
    Vec3 tag_vectorsight;  // 164
    int tag_flag;  // 176
    int tag_value;  // 180
    int tag_change_value;  // 184
    int tag_type;  // 188
    int tag_change_type;  // 192
    int tag_mode;  // 196
    int tag_change_mode;  // 200
    int tag_etage;  // 204
    int tag_etage_force;  // 208
    int __tag_2x2dyn_pos_Dimension1;  // 212
    Vec3 tag_2x2dyn_pos[4];  // 216
    float tag_size;  // 264
    float tag_change_size;  // 268
    Vec3 tag_change_free_pos1;  // 272
    Vec3 tag_free_pos1;  // 284
    Vec3 tag_change_free_pos2;  // 296
    Vec3 tag_free_pos2;  // 308
    int i_SF_detruit;  // 320
    int des_i_bouffable;  // 324
    int des_i_poussable;  // 328
    int des_i_deplacable;  // 332
    int des_alignement_axe_monde;  // 336
    int des_i_force_necessaire;  // 340
    int des_i_mur_case_jaune;  // 344
    int des_special_tag_value;  // 348
    int des_tag_mode_special;  // 352
    int SV_objet_position_ID;  // 356
    int SF_tentative_de_push;  // 360
    int des_objet_a_detruire1;  // 364
    int des_objet_a_detruire2;  // 368
    int des_objet_a_detruire3;  // 372
    int des_objet_a_detruire4;  // 376
    int des_tagoff_si_capaswich;  // 380
    int i_default_tag_type;  // 384
    Vec3 v_offset_pos;  // 388
    tMessage m_push;  // 400
    int i_force_exercee;  // 452
    int __ao_pousseurs_Dimension1;  // 456
    int ao_pousseurs[5];  // 460
    int o_disappear;  // 480
    int des_prio_de_collision;  // 484
    int i_interdire_push;  // 488
    int i_allow_message;  // 492
    int i_instance;  // 496
    int i_instance1;  // 500
    int i_snd_flag;  // 504
    int des_i_type;  // 508
    float f_temps;  // 512
    int i_has_been_moved;  // 516
    int i_sound_instance;  // 520
    int i_snd_freq_solo;  // 524
    int i_snd_freq1;  // 528
    int i_snd_freq2;  // 532
    int i_blocked_by_grid;  // 536
    int i_force_exercee_old;  // 540
    float f_speed;  // 544
    Vec3 v_tag_free_offset;  // 548
    Vec3 v_paffed;  // 560
    int i_paffed;  // 572
    int o_wp_max;  // 576
    int o_prefetch;  // 580
    int i_prefetch;  // 584
} tScriptStruct_ObjetPoussable;

typedef struct sScriptStruct_PJAircraft {
    int des_i_mode;  // 0
    int des_visuel;  // 4
    float des_f_speed_medium;  // 8
    float des_f_speed_max;  // 12
    float des_f_dash_speed_max;  // 16
    float des_f_dash_speed_min;  // 20
    float des_f_dash_blend_speed;  // 24
    float des_f_dash_delay;  // 28
    float des_f_poids;  // 32
    float f_coef_braquage;  // 36
    float f_friction_laterale_eau;  // 40
    float f_friction_laterale_sol;  // 44
    float f_acceleration_eau;  // 48
    float f_acceleration_sol;  // 52
    int des_n_network;  // 56
    int des_map_visible;  // 60
    float des_f_duree_fonce;  // 64
    int des_o_aircraft;  // 68
    float des_f_hit_multiplifier;  // 72
    float des_f_dist_vision;  // 76
    float des_f_shoot_min_dist;  // 80
    float des_f_shoot_max_dist;  // 84
    float des_f_same_speed_dist;  // 88
    float des_f_shoot_delay_min;  // 92
    float des_f_shoot_delay_max;  // 96
    float des_f_shoot_freq;  // 100
    int des_i_shoot_nb;  // 104
    float des_f_shoot_angle;  // 108
    float des_f_shoot_speed;  // 112
    float des_f_depassement_speed;  // 116
    int des_o_cinetexte;  // 120
    int des_i_replique_nargue;  // 124
    int des_i_nb_repliques_nargue;  // 128
    int des_i_replique_abandonne;  // 132
    int des_i_nb_repliques_abandonne;  // 136
    int des_mode_attaque;  // 140
    int des_o_torpille;  // 144
    int des_o_tourelle_laser;  // 148
    int i_num_entree;  // 152
    int __ao_kit_entree_Dimension1;  // 156
    int ao_kit_entree[10];  // 160
    int des_c_halo_avant;  // 200
    int des_c_halo_arriere;  // 204
    int des_c_etincelles;  // 208
    Vec3 v_speed;  // 212
    Vec3 v_normalized_speed;  // 224
    Vec3 v_hor_speed;  // 236
    Vec3 v_hor_normalized_speed;  // 248
    float f_speed;  // 260
    float f_hor_speed;  // 264
    float f_water_Z;  // 268
    float f_last_valid_water_Z;  // 272
    float f_friction_globale_z;  // 276
    float f_engine_rotspeed;  // 280
    float f_paf_rotspeed;  // 284
    float f_sol_rotspeed;  // 288
    float f_col_rotspeed;  // 292
    float f_helice_G_rot_speed;  // 296
    float f_helice_D_rot_speed;  // 300
    float f_vitesse_max;  // 304
    float f_external_speed;  // 308
    float pri_f_joy_norm;  // 312
    float f_min_rotation_factor;  // 316
    float f_sol_resistance;  // 320
    float f_collision_speed;  // 324
    float f_angle;  // 328
    float f_sin;  // 332
    float f_oscillation_speed;  // 336
    float f_limit_speed;  // 340
    float f_next_shoot_delay;  // 344
    float f_houle_coef;  // 348
    float f_trappe_angle;  // 352
    float f_plug_malus;  // 356
    float f_shoot_delay;  // 360
    float f_accostage_blend_coef;  // 364
    float f_blend_change_normale_sol;  // 368
    float f_moteur_angle;  // 372
    float f_throttle;  // 376
    float f_throttle_duree;  // 380
    float f_base_gravity;  // 384
    float f_slide_coef;  // 388
    float f_profondeur;  // 392
    float f_courant_off_friction_coef;  // 396
    float f_DT_modifier;  // 400
    float f_jump_Z_add_speed;  // 404
    float f_time_shoot;  // 408
    float f_time_ray;  // 412
    float f_time_target_seen;  // 416
    float f_time_air;  // 420
    float f_time_water;  // 424
    float f_time_last_jump;  // 428
    float f_time_paf;  // 432
    float f_time_ground_col;  // 436
    float f_time_water_col;  // 440
    float f_time_aircraft_vide;  // 444
    float f_time_culled;  // 448
    float f_time_speed_loss;  // 452
    float f_time_speak;  // 456
    float f_time_speak_delay;  // 460
    float f_time_slow_speed;  // 464
    float f_time_send_paf;  // 468
    float f_time_aircraft_proche;  // 472
    float f_time_bouton_gaz_released;  // 476
    float f_time_dock_out;  // 480
    float f_time_bip;  // 484
    float f_time_start_mode;  // 488
    float f_time_start_course;  // 492
    float f_time_gonfle_jupe;  // 496
    float f_time_drop_drone;  // 500
    float f_time_drop_pod;  // 504
    float f_time_aircraft_col;  // 508
    Vec3 v_last_speed;  // 512
    Vec3 v_engine_force;  // 524
    Vec3 v_water_force;  // 536
    Vec3 v_slide_force;  // 548
    Vec3 v_normale_sol;  // 560
    Vec3 v_new_normale_sol;  // 572
    Vec3 v_normale_eau;  // 584
    Vec3 pri_v_joy_dir;  // 596
    Vec3 v_dest_pos;  // 608
    Vec3 v_friction;  // 620
    Vec3 v_joy_last_dir;  // 632
    Vec3 v_gravity;  // 644
    Vec3 v_poursuite_degagement_pos;  // 656
    Vec3 v_BV_size;  // 668
    Vec3 v_accostage_sight;  // 680
    Vec3 v_accostage_horizon;  // 692
    Vec3 v_accostage_pos;  // 704
    int des_bouton_gaz;  // 716
    int des_bouton_saut;  // 720
    int des_bouton_use;  // 724
    int old_aircraft_status;  // 728
    int i_reflex_prio_contextuel;  // 732
    int i_current_mode;  // 736
    int i_last_mode;  // 740
    int i_poursuite_mode;  // 744
    int i_shoot_nb;  // 748
    int i_nb_lock;  // 752
    int i_snd_water;  // 756
    float f_snd_water_volref;  // 760
    float f_snd_water_freqref;  // 764
    int i_snd_water_play;  // 768
    float f_snd_profondeur;  // 772
    int i_snd_engine;  // 776
    float f_freq_engine;  // 780
    float f_snd_engine_volref;  // 784
    int i_snd_reactor;  // 788
    int i_snd_reactor_freqref;  // 792
    float f_snd_reactor_volref;  // 796
    float f_time_sound_choc;  // 800
    int i_snd_ground;  // 804
    float f_snd_ground_volref;  // 808
    int i_snd_ground_freqref;  // 812
    int i_snd_ground_matiere_old;  // 816
    int i_snd_ground_matiere;  // 820
    int i_snd_frottement;  // 824
    int i_snd_frottement_freqref;  // 828
    float f_ratio_speed;  // 832
    float des_f_freq_max;  // 836
    float des_f_freq_min;  // 840
    int i_freq;  // 844
    float des_f_temps_accel_max;  // 848
    float des_frotte_coef;  // 852
    float f_compteur_air;  // 856
    float f_sound_course_coef;  // 860
    int i_snd_flamme;  // 864
    float f_shoot_vol;  // 868
    int GFX_Halo_Laser;  // 872
    int __GFX_Halo_Tab_Dimension1;  // 876
    int GFX_Halo_Tab[6];  // 880
    Vec3 v_GFX_wind;  // 904
    int i_flag_in_water;  // 916
    float i_flag_in_water_old;  // 920
    int i_flag_in_air;  // 924
    int i_flag_on_ground;  // 928
    int i_flag_accostage_done;  // 932
    int i_flag_debarquement_done;  // 936
    int i_flag_ES;  // 940
    int i_flag_shoot;  // 944
    int i_flag_jump_embeded;  // 948
    int i_flag_lock_embeded;  // 952
    int i_flag_engine_upgraded;  // 956
    int i_flag_jump_allowed;  // 960
    int i_flag_jump_ready;  // 964
    int i_flag_gen_cam_allowed;  // 968
    int i_flag_en_deplacement;  // 972
    int i_flag_perso_on_aircraft;  // 976
    int i_flag_paf;  // 980
    int i_flag_paf_aircraft;  // 984
    int i_flag_vue_interne;  // 988
    int i_flag_abandon;  // 992
    int i_flag_phare_allume;  // 996
    int i_flag_courant_ok;  // 1000
    int i_flag_engine_on;  // 1004
    int i_flag_force_phares;  // 1008
    int i_flag_accostage_zone;  // 1012
    int i_flag_largue_mine;  // 1016
    int i_flag_external_orientation;  // 1020
    int i_flag_full_throttle;  // 1024
    int i_flag_jump;  // 1028
    int i_flag_in_water_jump;  // 1032
    int i_flag_presque_mort;  // 1036
    int i_flag_force_speed_cam;  // 1040
    int i_flag_real_water;  // 1044
    int i_flag_freine;  // 1048
    int i_flag_slide;  // 1052
    int i_flag_dont_turn;  // 1056
    int i_flag_dash_allowed;  // 1060
    int i_flag_shoot_allowed;  // 1064
    int i_flag_paf_this_trame;  // 1068
    int i_flag_water_secto;  // 1072
    int i_interdire_look;  // 1076
    int i_camera_mode;  // 1080
    int i_last_camera_mode;  // 1084
    int i_cam_course_flag_cut;  // 1088
    float f_cam_course_rot_sign;  // 1092
    float f_cam_courrse_start_time;  // 1096
    float f_time_paf_cam;  // 1100
    float f_time_big_impact;  // 1104
    Vec3 v_cam_paf_axis;  // 1108
    int o_seen_object;  // 1120
    int o_enterprise;  // 1124
    int o_paf_flash_effect;  // 1128
    int o_trappe;  // 1132
    int o_visual;  // 1136
    int o_helice_G;  // 1140
    int o_helice_D;  // 1144
    int o_interieur;  // 1148
    int o_trappe_interieur;  // 1152
    int o_jupe;  // 1156
    int o_moteur_G;  // 1160
    int o_moteur_D;  // 1164
    int o_wp_target;  // 1168
    int o_first_wp;  // 1172
    int o_prev_wp;  // 1176
    int o_next_wp;  // 1180
    int o_rescue_wp;  // 1184
    int o_course_manager;  // 1188
    int i_nbr_de_tour;  // 1192
    int i_nbr_de_checkpoint;  // 1196
    int i_nbr_de_checkpoint_save;  // 1200
    float f_dist_to_next_wp;  // 1204
    tMessage m_mes;  // 1208
    tMessage m_shoot_msg;  // 1260
    int o_perso_accostage;  // 1312
    int o_colmap_sup;  // 1316
    int o_companion;  // 1320
    int i_phase_companion;  // 1324
    int i_companion_id;  // 1328
    int nbr_of_actor_on_air;  // 1332
    float f_time_send_vala;  // 1336
    int i_nb_companion_out;  // 1340
    int i_cheat_status;  // 1344
    int __ao_pod_Dimension1;  // 1348
    int ao_pod[5];  // 1352
    int i_pod_nb;  // 1372
    int i_gen_pod;  // 1376
    int o_materia;  // 1380
    int i_pod_type;  // 1384
    int f_pod_energie_a_reparer;  // 1388
    float f_time_clignoting;  // 1392
    float f_delai_clignoting;  // 1396
    int i_nb_clignoting;  // 1400
    float f_scale_clignoting;  // 1404
    int GFX_Halo_collision;  // 1408
    Vec3 v_collide_point;  // 1412
    Vec3 v_normale_wall;  // 1424
    float f_time_wall_col;  // 1436
    Vec3 v_grav_pos;  // 1440
    Vec3 v_grav_speed;  // 1452
    float f_jupe_angle;  // 1464
    float f_visual_Z;  // 1468
    int GFX_Airtrace;  // 1472
    int i_flag_trace_on;  // 1476
    float LForce_Jet_UP;  // 1480
    float RForce_Jet_UP;  // 1484
    float LForce_Jet_SIDE;  // 1488
    float RForce_Jet_SIDE;  // 1492
    int c_water_color;  // 1496
    float f_profondeur_coef;  // 1500
    float WaterZ;  // 1504
    int __ao_plug_wp_Dimension1;  // 1508
    int ao_plug_wp[5];  // 1512
    int __ai_GFX_flamme_Dimension1;  // 1532
    int ai_GFX_flamme[5];  // 1536
    int __ai_GFX_fumee_Dimension1;  // 1556
    int ai_GFX_fumee[5];  // 1560
    int __af_time_smoke_Dimension1;  // 1580
    float af_time_smoke[5];  // 1584
    int __af_time_water_smoke_Dimension1;  // 1604
    float af_time_water_smoke[5];  // 1608
    int __f_time_change_dir_Dimension1;  // 1628
    float f_time_change_dir[5];  // 1632
    int __f_delai_change_dir_Dimension1;  // 1652
    float f_delai_change_dir[5];  // 1656
    int __v_GFX_pos_Dimension1;  // 1676
    Vec3 v_GFX_pos[5];  // 1680
    float f_GFX_speed_coef;  // 1740
    float f_accelerateur;  // 1744
    float f_time_vroom;  // 1748
    float f_delai_vroom;  // 1752
    int i_flag_vroom;  // 1756
    float f_time_loop;  // 1760
    float f_X_angle;  // 1764
    float f_Y_angle;  // 1768
    float f_Z_angle;  // 1772
    int i_X_angle_sign;  // 1776
    int i_Y_angle_sign;  // 1780
    float f_sign_malus_dir;  // 1784
    float f_time_malus_dir;  // 1788
    float f_delay_malus_dir;  // 1792
    float f_malus_dir;  // 1796
    float f_malus_speed;  // 1800
    int i_flag_teuf_teuf;  // 1804
    int i_flag_force_flash;  // 1808
    int i_flag_en_panne;  // 1812
    int GFX_Engine_Smoke;  // 1816
    int GFX_Flash;  // 1820
    int GFX_Flash_2;  // 1824
    float f_time_GFX_Engine_Smoke;  // 1828
    float f_time_GFX_Flash;  // 1832
    int i_flag_dash;  // 1836
    int i_flag_virtual_dash;  // 1840
    int GFX_Boost_G;  // 1844
    int GFX_Boost_D;  // 1848
    float f_time_dash;  // 1852
    float f_dash_speed;  // 1856
    float f_dash_speed_min;  // 1860
    float f_corde_length;  // 1864
    float f_corde_tension;  // 1868
    float f_time_contact_visuel;  // 1872
    float f_time_release;  // 1876
    float f_time_corde_cam;  // 1880
    Vec3 v_corde_force;  // 1884
    Vec3 v_corde_pos;  // 1896
    Vec3 v_attache_pos;  // 1908
    Vec3 v_axe_corde;  // 1920
    int o_perso_ventoused;  // 1932
    Vec3 v_corde_offset;  // 1936
    float f_cabrage;  // 1948
    float f_axe_speed;  // 1952
    float f_last_axe_speed;  // 1956
    int i_flag_mode_cine;  // 1960
    Vec3 v_cine_friction_water;  // 1964
    Vec3 v_cine_friction_ground;  // 1976
    float f_cine_throttle;  // 1988
    float f_cine_gaz;  // 1992
    float f_time_lock;  // 1996
    int i_target_max_nb;  // 2000
    int i_missile_nb;  // 2004
    int __ai_free_space_index_Dimension1;  // 2008
    int ai_free_space_index[20];  // 2012
    int o_locking_target;  // 2092
    int __ao_missiles_Dimension1;  // 2096
    int ao_missiles[20];  // 2100
    int des_o_SimpleShoot;  // 2180
    int i_target_index;  // 2184
    int __af_time_contact_visuel_Dimension1;  // 2188
    float af_time_contact_visuel[20];  // 2192
    float f_time_contextuel_release;  // 2272
    int GFX_Poussiere;  // 2276
    int i_mode_saut;  // 2280
    float f_z_tgt;  // 2284
    float f_z_air;  // 2288
    float f_time_saut;  // 2292
    float f_time_derniere_perte_denergi;  // 2296
    int i_flag_init_visual_rot;  // 2300
    float f_paf_rot_side;  // 2304
    float f_paf_rot_speed;  // 2308
    float f_paf_rot_angle;  // 2312
    float f_paf_rot_dec_angle;  // 2316
    float f_paf_rot_dec_delay;  // 2320
    float f_paf_rot_dec_time;  // 2324
    float f_paf_rot_dec_speed;  // 2328
    float f_paf_rot_dec_angle_compl;  // 2332
    float f_time_display_text;  // 2336
    float f_delay_display_text;  // 2340
    int i_next_txt_index;  // 2344
    float f_real_profondeur;  // 2348
    float f_visual_Z_offset_min;  // 2352
    Vec3 v_last_water_collide_pos;  // 2356
    Vec3 v_last_water_real_pos;  // 2368
    int i_flag_dash_gravity;  // 2380
    int i_block_accel;  // 2384
    Vec3 v_orientation_dir;  // 2388
    int i_flag_course_rewarder;  // 2400
    int i_disable_save_ON;  // 2404
} tScriptStruct_PJAircraft;

typedef struct sScriptStruct_DUPPodRepare {
    int des_o_aircraft;  // 0
    float des_f_energie;  // 4
    int des_o_caisse;  // 8
    int i_current_mode;  // 12
    int GFX_spark;  // 16
    int i_nb_explosion;  // 20
    int i_pod_index;  // 24
    int i_mode_phase;  // 28
    int i_pod_type;  // 32
    int i_flag_ES;  // 36
    float f_time_change_mode;  // 40
    float f_time_spark;  // 44
    float f_time_explosion;  // 48
    float f_delay;  // 52
    float f_energie;  // 56
    float f_time_phase;  // 60
    float f_snd_freq;  // 64
    float f_snd_vol;  // 68
    Vec3 v_materia_pos;  // 72
    int o_plug;  // 84
    int o_aircraft_visual;  // 88
    int o_materia;  // 92
} tScriptStruct_DUPPodRepare;

typedef struct sScriptStruct_DUPHoverCraftShoot {
    int o_Target;  // 0
    float f_Distance;  // 4
    int i_Halo;  // 8
    Vec3 v_Sight;  // 12
    int i_flag_1st_trame;  // 24
    int i_flag_target;  // 28
    int i_flag_init_done;  // 32
    int GFX_Line;  // 36
    int GFX_Halo;  // 40
    int GFX_Halo_2;  // 44
    int GFX_Flash;  // 48
    float f_speed;  // 52
    float f_line_length;  // 56
    float f_time_impact;  // 60
    float f_time_start;  // 64
    float f_XMEN_width;  // 68
    float f_delay_inertie;  // 72
    Vec3 v_rand_bv_pos;  // 76
    Vec3 v_last_pos;  // 88
    Vec3 v_aircraft_start_speed;  // 100
    int o_main_actor;  // 112
    int o_aircraft;  // 116
    float f_shoot_vol;  // 120
    int i_flag_perfect_shoot;  // 124
    int i_flag_sight_set;  // 128
} tScriptStruct_DUPHoverCraftShoot;

typedef struct sScriptStruct_DetectOZone {
    int des_acteur_detecte1;  // 0
    int des_i_SPFlag_set01;  // 4
    int des_i_SPFlag_reset01;  // 8
    int des_i_SPFlag_invert01;  // 12
    float des_f_minuterie;  // 16
    float f_temps;  // 20
    int i_IN;  // 24
} tScriptStruct_DetectOZone;

typedef struct sScriptStruct_ZoneChangeStatusMultiCompanion {
    int i_Peyj_change_status;  // 0
    int i_Mili_change_status;  // 4
    int i_Domz_change_status;  // 8
    int i_XB25_change_status;  // 12
    int i_XB26_change_status;  // 16
    int __ai_compagnon_qui_change_de_st_Dimension1;  // 20
    int ai_compagnon_qui_change_de_st[5];  // 24
    int i_compagnon_status_objectif;  // 44
    int __OT_a_activer_en_attente_Dimension1;  // 48
    int OT_a_activer_en_attente[5];  // 52
    int __des_o_pos_reinit_perso_Dimension1;  // 72
    int des_o_pos_reinit_perso[5];  // 76
} tScriptStruct_ZoneChangeStatusMultiCompanion;

typedef struct sScriptStruct_SFXExplosion {
    int des_play_sound;  // 0
    float f_delai_init;  // 4
    int des_moving_sfx;  // 8
    int des_standard_color;  // 12
    int des_fire_smoke;  // 16
    int des_fire_rotation;  // 20
    int i_effect_nbr;  // 24
    float f_size_coef;  // 28
    float f_speed_coef;  // 32
    float f_particules_Z;  // 36
    float Detail;  // 40
    int i_sorted;  // 44
    int SND_explode_sound0;  // 48
    int SND_explode_sound0b;  // 52
    float SND_random_sound;  // 56
    int SND_Perso_a_detruire;  // 60
    int des_perso_a_suivre;  // 64
    int des_bone_a_suivre;  // 68
    Vec3 des_v_offset;  // 72
    int __table_color_Dimension1;  // 84
    int table_color[5];  // 88
    int __table_fonction_Dimension1;  // 108
    int table_fonction[50];  // 112
    int o_sfx_explode;  // 312
    Vec3 FFR;  // 316
    Vec3 pos_alea;  // 328
    Vec3 v_vecteur;  // 340
    float BV;  // 352
    float OoSpeedcoef;  // 356
    float OoSizecoef;  // 360
    float FX4_Ht;  // 364
    float FX4_Rd;  // 368
    int GFX;  // 372
    int i_GFX;  // 376
    float Npa;  // 380
    int GFX_Spark_1;  // 384
    int GFX_2;  // 388
    int GFX_3;  // 392
    int GFX_4;  // 396
    int GFX_Lightning_1;  // 400
    int GFX_Lightning_2;  // 404
    int GFX_Lightning_3;  // 408
    int GFX_Flash;  // 412
    float f_time_start;  // 416
    float f_time_change_dir;  // 420
    float f_delai_change_dir;  // 424
    float f_time_change_size;  // 428
    float f_delai_change_size;  // 432
    Vec3 v_init_pos;  // 436
    int i_int;  // 448
    int i_flag_snap_on_actor;  // 452
    Vec3 v_last_pos;  // 456
    int i_snd_fire_loop;  // 468
    int i_flag_volume_3D;  // 472
} tScriptStruct_SFXExplosion;

typedef struct sScriptStruct_SFXBoneFire {
    int des_o_snap_actor;  // 0
    int des_i_bone_num;  // 4
    Vec3 des_v_bone_offset;  // 8
    float des_f_detail;  // 20
    float des_f_size_coef;  // 24
    float des_f_life_time;  // 28
    int des_move;  // 32
    int o_materiaux_fire;  // 36
    int GFX_Fire;  // 40
    int GFX_Halo;  // 44
    int __GFX_Fire2_Dimension1;  // 48
    int GFX_Fire2[20];  // 52
    int i_flag_fire_done;  // 132
    float f_time_start;  // 136
    float f_time_spark;  // 140
    float f_delay_spark;  // 144
    Vec3 v_last_pos;  // 148
    Vec3 v_last_last_pos;  // 160
    int o_bone;  // 172
    int o_callback_gao;  // 176
    tMessage m_msg;  // 180
} tScriptStruct_SFXBoneFire;

typedef struct sScriptStruct_OrageMeteores {
    int des_o_meteore_base;  // 0
    int des_o_rayon;  // 4
    int des_o_cible;  // 8
    Vec3 des_v_zone_depart;  // 12
    float des_f_intervalle;  // 24
    float des_f_vitesse;  // 28
    float f_temps;  // 32
    float f_delai;  // 36
} tScriptStruct_OrageMeteores;

typedef struct sScriptStruct_CineFigeleMonde {
    float des_f_temps_iris_1;  // 0
    float des_f_temps_obturation;  // 4
    float des_f_temps_iris_2;  // 8
    Vec3 des_v_centre_iris;  // 12
} tScriptStruct_CineFigeleMonde;

typedef struct sScriptStruct_TestAppuiBouton {
    int des_i_bouton_teste;  // 0
    int __des_ai_sequence_boutons_Dimension1;  // 4
    int des_ai_sequence_boutons[16];  // 8
    float des_f_temps_appui;  // 72
    int des_o_acteur_capa;  // 76
    int des_i_capa_VRAI;  // 80
    int des_i_capa_FAUX;  // 84
    float f_temps_ecoule;  // 88
    int i_bouton_seq;  // 92
} tScriptStruct_TestAppuiBouton;

typedef struct sScriptStruct_DonneDesPognes {
    int des_i_fric_a_donner;  // 0
    float des_f_temps;  // 4
    int des_o_pause_snd;  // 8
    int i_fric;  // 12
    float f_fric_coeff;  // 16
    float f_temps;  // 20
    float f_time;  // 24
    int i_instance;  // 28
    int i_freq;  // 32
    float f_vol;  // 36
} tScriptStruct_DonneDesPognes;

typedef struct sScriptStruct_INVSecundoEtLaFacture {
    int o_Text;  // 0
    int o_Secundo;  // 4
    int o_SecundoFX;  // 8
    float f_Timer;  // 12
    int i_State;  // 16
} tScriptStruct_INVSecundoEtLaFacture;

typedef struct sScriptStruct_ZoneChangeStatus {
    int SF_activation;  // 0
    int SF_destruction;  // 4
    int des_companion_ID;  // 8
    int des_companion_status_objectif;  // 12
    int des_OT_a_activer_en_attente;  // 16
    int des_i_affiche_texte;  // 20
    tTextGet des_t_texte_attente;  // 24
    tTextGet des_t_texte_attente_mili;  // 32
    int des_o_pos_reinit_perso;  // 40
    int i_ind_compagnon;  // 44
    int o_compagnon;  // 48
} tScriptStruct_ZoneChangeStatus;

typedef struct sScriptStruct_CapaSFActivationDeactivationDetect {
    int acteur_a_detecter;  // 0
    int SF_activation;  // 4
    int fait_l_inverse_aussi;  // 8
    int i_active_ou_desactive;  // 12
    int __object_a_activer_Dimension1;  // 16
    int object_a_activer[5];  // 20
    int rend_invisible_aussi;  // 40
    int i_done_off;  // 44
    int i_done_on;  // 48
} tScriptStruct_CapaSFActivationDeactivationDetect;

typedef struct sScriptStruct_MpegPlayer {
    int des_i_SubMat;  // 0
    int des_i_SubMatLevel;  // 4
    int des_i_StopAtEnd;  // 8
    int des_o_Interrupteur;  // 12
    int des_o_stopsound;  // 16
    int i_Play;  // 20
    int des_i_sf_jesuisla;  // 24
    int des_i_sf_modifier_vrai;  // 28
    int des_i_sf_on_off;  // 32
    int des_i_mat;  // 36
    int des_o_snd_stop;  // 40
    float des_f_duree_here;  // 44
    int des_f_init_la;  // 48
    int des_i_SND_modifier;  // 52
} tScriptStruct_MpegPlayer;

typedef struct sScriptStruct_GSTCarte3D {
    int des_i_CarteNum;  // 0
    int des_i_SFPiece;  // 4
    int des_i_Close;  // 8
    int des_i_ForceTaken;  // 12
    int des_i_InvertZoom;  // 16
    Vec3 des_v_Pos;  // 20
    Vec3 v_SavePos;  // 32
    float des_f_Z;  // 44
    int des_i_NbEtages;  // 48
    int __des_ao_Etage_Dimension1;  // 52
    int des_ao_Etage[8];  // 56
    int des_i_DefaultEtage;  // 88
    int i_EtageRequired;  // 92
    int i_EtageCur;  // 96
    int i_EtageTransition;  // 100
    int __f_EtageState_Dimension1;  // 104
    float f_EtageState[8];  // 108
    int __f_EtageStateNext_Dimension1;  // 140
    float f_EtageStateNext[8];  // 144
    int __des_af_EtageZ_Dimension1;  // 176
    float des_af_EtageZ[8];  // 180
    float des_f_EtageEcart;  // 212
    int des_i_EtageInInv;  // 216
    int des_i_NbFleches;  // 220
    int __des_i_FlecheEtage_Dimension1;  // 224
    int des_i_FlecheEtage[32];  // 228
    int __des_i_FlecheGroup_Dimension1;  // 356
    int des_i_FlecheGroup[32];  // 360
    int __des_i_FlechePiece_Dimension1;  // 488
    int des_i_FlechePiece[32];  // 492
    float f_FlecheTimer;  // 620
    int des_i_NbPieces;  // 624
    int __des_ai_PieceState_Dimension1;  // 628
    int des_ai_PieceState[64];  // 632
    int __des_ai_PieceData_Dimension1;  // 888
    int des_ai_PieceData[64];  // 892
    int __des_ai_PieceColor_Dimension1;  // 1148
    int des_ai_PieceColor[64];  // 1152
    int des_i_MapNb;  // 1408
    int __des_ai_MapIndex_Dimension1;  // 1412
    int des_ai_MapIndex[16];  // 1416
    int __des_av_MapOffset_Dimension1;  // 1480
    Vec3 des_av_MapOffset[16];  // 1484
    int __des_ai_MapRotation_Dimension1;  // 1676
    int des_ai_MapRotation[16];  // 1680
    int __des_ai_FixedSpot_Dimension1;  // 1744
    int des_ai_FixedSpot[16];  // 1748
    Vec3 v_Offset;  // 1812
    int i_Rotation;  // 1824
    int i_CurMap;  // 1828
    int i_FixedSpot;  // 1832
    int des_o_Spot;  // 1836
    int i_SpotType;  // 1840
    int i_SpotIndex;  // 1844
    int i_SpotOnMap;  // 1848
    int i_SpotOnFloor;  // 1852
    int __des_c_SpotColor_Dimension1;  // 1856
    int des_c_SpotColor[5];  // 1860
    int __ai_CompState_Dimension1;  // 1880
    int ai_CompState[12];  // 1884
    int des_i_NbNotes;  // 1932
    int __des_ai_Note_Dimension1;  // 1936
    int des_ai_Note[32];  // 1940
    int __des_ai_NoteSF_Dimension1;  // 2068
    int des_ai_NoteSF[32];  // 2072
    int __des_av_NotePos_Dimension1;  // 2200
    Vec3 des_av_NotePos[32];  // 2204
    int des_i_NbAnimals;  // 2588
    int __des_ai_Animal_Dimension1;  // 2592
    int des_ai_Animal[80];  // 2596
    int __des_ai_AnimalEtage_Dimension1;  // 2916
    int des_ai_AnimalEtage[80];  // 2920
    int __des_ai_AnimalMap_Dimension1;  // 3240
    int des_ai_AnimalMap[80];  // 3244
    int __des_ai_AnimalState_Dimension1;  // 3564
    int des_ai_AnimalState[80];  // 3568
    int __des_av_AnimalPos_Dimension1;  // 3888
    Vec3 des_av_AnimalPos[80];  // 3892
    int __des_ai_AnimalSF_Dimension1;  // 4852
    int des_ai_AnimalSF[80];  // 4856
    int i_NbAnimalsVisible;  // 5176
    int i_AnimalDynNb;  // 5180
    int __av_AnimalDynPos_Dimension1;  // 5184
    Vec3 av_AnimalDynPos[32];  // 5188
    int __ai_AnimalDynData_Dimension1;  // 5572
    int ai_AnimalDynData[32];  // 5576
    int des_i_NbPearls;  // 5704
    int __des_ai_Pearl_Dimension1;  // 5708
    int des_ai_Pearl[40];  // 5712
    int __des_ai_PearlSF_Dimension1;  // 5872
    int des_ai_PearlSF[40];  // 5876
    int __des_ai_PearlSF2_Dimension1;  // 6036
    int des_ai_PearlSF2[40];  // 6040
    int __des_ai_PearlMap_Dimension1;  // 6200
    int des_ai_PearlMap[40];  // 6204
    int __des_ai_PearlEtage_Dimension1;  // 6364
    int des_ai_PearlEtage[40];  // 6368
    int __des_av_PearlPos_Dimension1;  // 6528
    Vec3 des_av_PearlPos[40];  // 6532
    int des_i_NbPearlVisible;  // 7012
    int des_i_PearlNbGroup;  // 7016
    int __des_i_PearlGroupFirst_Dimension1;  // 7020
    int des_i_PearlGroupFirst[16];  // 7024
    int __des_i_PearlGroupNb_Dimension1;  // 7088
    int des_i_PearlGroupNb[16];  // 7092
    int des_i_NbConsole;  // 7156
    int __des_av_ConsolePos_Dimension1;  // 7160
    Vec3 des_av_ConsolePos[10];  // 7164
    int des_i_CanZoom;  // 7284
    int des_i_CanPane;  // 7288
    float des_f_Zoom;  // 7292
    float f_ZoomScale;  // 7296
    Vec3 v_ZoomPos;  // 7300
    Vec3 v_ZoomPosInit;  // 7312
    float f_ZoomMinX;  // 7324
    float f_ZoomMaxX;  // 7328
    float f_ZoomMinZ;  // 7332
    float f_ZoomMaxZ;  // 7336
    int i_ZoomWait1;  // 7340
    float f_ZoomMinTime;  // 7344
    float des_f_Alpha;  // 7348
    float des_f_Beta;  // 7352
    int i_Appear;  // 7356
    int i_EffectRot;  // 7360
    int i_exec_State;  // 7364
    Vec3 v_exec_3DPos;  // 7368
    Vec3 v_exec_2DPos;  // 7380
    int i_exec_Icone;  // 7392
    int i_exec_Resize;  // 7396
    int i_exec_Rotate;  // 7400
    int c_exec_Color;  // 7404
    int i_Edit_Clean;  // 7408
    int i_Edit_CurPage;  // 7412
    int i_Edit_Page0_CurLine;  // 7416
    int i_Edit_Page0_Offset;  // 7420
    int i_Edit_Page1_CurLine;  // 7424
    int i_Edit_Page1_Offset;  // 7428
    int i_Edit_Page2_CurLine;  // 7432
    int i_Edit_Page2_Offset;  // 7436
    int i_Edit_Page3_CurLine;  // 7440
    int i_Edit_Page3_Offset;  // 7444
    int i_Edit_Page4_CurLine;  // 7448
    int i_Edit_ShowAnimalsStatic;  // 7452
    int i_Edit_ShowAnimalsDyn;  // 7456
    float f_Edit_Delay;  // 7460
    float f_Edit_CurDelay;  // 7464
    float f_EtageProg;  // 7468
} tScriptStruct_GSTCarte3D;

typedef struct sScriptStruct_SFXLightning {
    int des_mode;  // 0
    int des_o_actor_capa;  // 4
    int des_i_capa_test;  // 8
    float des_f_delai_ON_min;  // 12
    float des_f_delai_ON_max;  // 16
    float des_f_delai_OFF_min;  // 20
    float des_f_delai_OFF_max;  // 24
    float des_f_delai_fade_in;  // 28
    float des_f_delai_fade_out;  // 32
    int des_o_gmat_gao;  // 36
    int des_i_gmat_nb;  // 40
    int des_o_wp_dest;  // 44
    float des_f_epaisseur;  // 48
    float des_f_rayon;  // 52
    int des_i_decoupage;  // 56
    int des_lightning_mode;  // 60
    int des_c_color_OFF;  // 64
    int des_c_color_ON_1;  // 68
    int des_c_color_ON_2;  // 72
    float des_f_freq;  // 76
    float f_time;  // 80
    float f_delai;  // 84
    float f_angle;  // 88
    int i_flag_ON;  // 92
    int GFX_lightning;  // 96
    int c_current_color;  // 100
} tScriptStruct_SFXLightning;

typedef struct sScriptStruct_SFXAfterFX {
    int i_AFX_type;  // 0
    float f_temps_debut;  // 4
    float f_temps_fixe;  // 8
    float f_temps_fin;  // 12
    float f_p0_val_dest;  // 16
    float f_p0_val_fin;  // 20
    float f_p0_val_cour;  // 24
    float f_p1_val_cour;  // 28
    float f_p2_val_cour;  // 32
    float f_p0_val_coeff;  // 36
    float f_p0_val_coeff2;  // 40
} tScriptStruct_SFXAfterFX;

typedef struct sScriptStruct_SFXEtincelles {
    int des_mode;  // 0
    int des_o_actor_capa;  // 4
    int des_i_capa_test;  // 8
    float des_f_delai_ON_min;  // 12
    float des_f_delai_ON_max;  // 16
    float des_f_delai_OFF_min;  // 20
    float des_f_delai_OFF_max;  // 24
    int des_o_gmat_gao;  // 28
    int des_i_gmat_nb;  // 32
    int des_o_object_pos;  // 36
    int des_i_object_canal;  // 40
    int des_axe;  // 44
    int des_sens;  // 48
    float des_f_angle;  // 52
    float des_f_speed_min;  // 56
    float des_f_speed_max;  // 60
    float des_f_friction;  // 64
    float des_f_epaisseur;  // 68
    int des_c_color_1;  // 72
    int des_c_color_2;  // 76
    float des_f_life_time_min;  // 80
    float des_f_life_time_max;  // 84
    float des_f_length_multiplifier;  // 88
    float des_f_generation_period;  // 92
    int des_i_nb;  // 96
    Vec3 des_v_gravity;  // 100
    Vec3 des_v_pos_alea_min;  // 112
    int des_halo;  // 124
    float des_f_halo_radius;  // 128
    float des_f_halo_extrude;  // 132
    float f_time;  // 136
    float f_delai;  // 140
    int i_flag_ON;  // 144
    int GFX_spark;  // 148
    int GFX_Halo;  // 152
    int o_object_pos;  // 156
} tScriptStruct_SFXEtincelles;

typedef struct sScriptStruct_ActorGamEmploi {
    float f_scale_coef;  // 0
    float f_speed_coef;  // 4
    int I_Speak;  // 8
    int SF_plage_reserve;  // 12
    int n_occupation_network;  // 16
    int __f_occupation_hour_Dimension1;  // 20
    float f_occupation_hour[30];  // 24
    int __i_occupation_type_Dimension1;  // 144
    int i_occupation_type[30];  // 148
    int __o_occupation_pos_Dimension1;  // 268
    int o_occupation_pos[30];  // 272
    int i_action_trot;  // 392
    int i_occupation_indice;  // 396
    int i_occupation_courante;  // 400
    int i_occupation_courante_old;  // 404
    int o_nearest;  // 408
    int o_nearest_old;  // 412
    int o_occupation_dest;  // 416
    int o_occupation_source;  // 420
    int i_occupation_cherche_nouvelle;  // 424
    int __o_occupation_network_list_Dimension1;  // 428
    int o_occupation_network_list[50];  // 432
    int i_occupation_nbr_de_wp;  // 632
    int o_occupation_wp_intermediaire;  // 636
    int i_occupation_je_suis_sur_un_w;  // 640
    float f_edt_hour;  // 644
    float f_old_curent_hour;  // 648
    int i_force_next_edt;  // 652
    Vec3 v_memo_axe;  // 656
    float tf_size;  // 668
    Vec3 tag_vectorpos;  // 672
    Vec3 tag_vectorsight;  // 684
    int tag_flag;  // 696
    int tag_value;  // 700
    int tag_change_value;  // 704
    int tag_type;  // 708
    int tag_change_type;  // 712
    int tag_mode;  // 716
    int tag_change_mode;  // 720
    int tag_etage;  // 724
    int tag_etage_force;  // 728
    int __tag_2x2dyn_pos_Dimension1;  // 732
    Vec3 tag_2x2dyn_pos[4];  // 736
    float tag_size;  // 784
    float tag_change_size;  // 788
    Vec3 tag_change_free_pos1;  // 792
    Vec3 tag_free_pos1;  // 804
    Vec3 tag_change_free_pos2;  // 816
    Vec3 tag_free_pos2;  // 828
    int i_way_force_recompute;  // 840
    int i_way_moving;  // 844
    Vec3 v_way_currentpos;  // 848
    Vec3 v_way_destpos;  // 860
    Vec3 v_way_case_next;  // 872
    Vec3 v_way_case_dest;  // 884
    Vec3 v_way_speed;  // 896
    Vec3 v_way_sight;  // 908
    float f_way_blendrotate;  // 920
    int i_way_algo_limitdestpos;  // 924
    int i_way_etage_dest;  // 928
    Vec3 v_way_old_case;  // 932
    int i_way_search_around_destpos;  // 944
    int i_way_destpos_error;  // 948
    float f_way_len_big;  // 952
    float f_way_test_born;  // 956
    int i_way_case_nbr;  // 960
    float f_way_time_newcell;  // 964
    int des_i_type_acteur;  // 968
    int des_o_acteur_suividesyeux;  // 972
    Vec3 des_v_hauteur_cible;  // 976
    int des_i_RT_flags;  // 988
    float des_f_RT_vitesse_de_rotation;  // 992
    int o_manager;  // 996
    int o_le_loup;  // 1000
    int o_perso_a_attraper;  // 1004
    int __o_safe_list_Dimension1;  // 1008
    int o_safe_list[20];  // 1012
    int i_safe_nbr;  // 1092
    int __o_attente_list_Dimension1;  // 1096
    int o_attente_list[10];  // 1100
    int i_attente_nbr;  // 1140
    int o_safe;  // 1144
    float f_time_changetarget;  // 1148
    float f_time_changetarget_delay;  // 1152
    int i_fuite_frequency;  // 1156
    Vec3 v_gogol;  // 1160
    float f_distance_trop_proche;  // 1172
    float f_distance_trop_loin;  // 1176
    float speed;  // 1180
    int o_old_capa_wp;  // 1184
    int i_old_capa;  // 1188
    int o_safe_backup;  // 1192
    float f_time_safe;  // 1196
    float f_duree_safe;  // 1200
    float f_time_loup_init;  // 1204
    int i_zoom;  // 1208
    float f_time_keep_anim;  // 1212
    int i_interdit_pendant_mili_malad;  // 1216
    float des_f_distance_talk;  // 1220
    float des_f_angle_talk;  // 1224
    int __ai_conditions_Dimension1;  // 1228
    int __ai_conditions_Dimension2;  // 1232
    int ai_conditions[80];  // 1236
    int __at_questionsReponses_Dimension1;  // 1556
    int __at_questionsReponses_Dimension2;  // 1560
    tTextGet at_questionsReponses[40];  // 1564
    int __ai_consequences_Dimension1;  // 1884
    int __ai_consequences_Dimension2;  // 1888
    int ai_consequences[60];  // 1892
    int __ai_consequences_plus_Dimension1;  // 2132
    int __ai_consequences_plus_Dimension2;  // 2136
    int ai_consequences_plus[140];  // 2140
    int __ai_grisage_Dimension1;  // 2700
    int ai_grisage[20];  // 2704
    int __at_question_reponse_OK_Dimension1;  // 2784
    tTextGet at_question_reponse_OK[2];  // 2788
    int i_dialogue_on;  // 2804
    tMessage m_dialogue;  // 2808
    int Valide_Effet_1_Question;  // 2860
    int o_papa;  // 2864
    int i_talk_propose;  // 2868
    float f_time_fin_talk;  // 2872
    int i_snd_dodo;  // 2876
    int i_snd_depl;  // 2880
    int i_flag;  // 2884
    int i_flag1;  // 2888
    int i_SF_snap_on_wp;  // 2892
    int i_SF_snap_on_wp1;  // 2896
    int o_snap_on_wp;  // 2900
    int i_snap_action;  // 2904
    int des_i_animaux;  // 2908
    int des_i_etage;  // 2912
    int i_test_map_reset;  // 2916
} tScriptStruct_ActorGamEmploi;

typedef struct sScriptStruct_ActorRatBleu {
    float time;  // 0
    float time_anim;  // 4
    float alea;  // 8
    int i_distance;  // 12
    int i_MapEtage;  // 16
} tScriptStruct_ActorRatBleu;

typedef struct sScriptStruct_ActorCrochax {
    int des_i_type;  // 0
    int des_surprise_mode;  // 4
    int des_fast_1st_attack;  // 8
    float des_i_comportement_simplifie;  // 12
    float des_f_dist_propose_fight;  // 16
    int des_mode;  // 20
    int des_difficulte;  // 24
    int des_o_wp_of_net;  // 28
    int des_o_BV_zone;  // 32
    int des_SF_mort;  // 36
    int des_o_BV_detect_zone;  // 40
    int des_o_detected_actor;  // 44
    float des_angle_plug_axis;  // 48
    float des_f_taille;  // 52
    float des_f_base_speed;  // 56
    float des_f_attack_speed;  // 60
    float des_f_max_speed;  // 64
    float des_f_Sally_Y_offset;  // 68
    float des_f_Sally_Z_offset;  // 72
    float des_f_Sally_banking_orient;  // 76
    float des_f_Peyj_Y_offset;  // 80
    float des_f_Peyj_Z_offset;  // 84
    float des_f_Peyj_banking_orient;  // 88
    float des_f_Milit_Y_offset;  // 92
    float des_f_Milit_Z_offset;  // 96
    float des_f_Milit_banking_orient;  // 100
    int des_o_INFO_PHOTO;  // 104
    int des_o_COL_MODEL;  // 108
    int des_i_sf_present;  // 112
    int des_perle;  // 116
    int __des_points_de_chute_Dimension1;  // 120
    int des_points_de_chute[10];  // 124
    int des_o_renfort;  // 164
    float des_renfort_delay;  // 168
    int i_cine_fin_fight_id;  // 172
    int des_i_mapetage;  // 176
    int des_i_numbones;  // 180
    int __ao_obj_Dimension1;  // 184
    int ao_obj[11];  // 188
    int __af_lenos_Dimension1;  // 232
    float af_lenos[11];  // 236
    int __ao_patte_Dimension1;  // 280
    int ao_patte[12];  // 284
    int i_current_mode;  // 332
    int i_last_mode;  // 336
    int i_flag_ground_col;  // 340
    int i_flag_devant_aircraft;  // 344
    int i_flag_do_snake;  // 348
    int i_flag_sorti;  // 352
    int i_flag_force_sorti;  // 356
    int i_flag_dont_use_normal_mat;  // 360
    int i_flag_paf;  // 364
    int i_flag_enemy_non_culled;  // 368
    int i_flag_snake_done;  // 372
    int i_flag_esquive_done;  // 376
    int i_flag_cri_done;  // 380
    int i_cine_colset;  // 384
    int i_mode_phase;  // 388
    int i_plante;  // 392
    float f_angle_sight;  // 396
    float f_angle_banking;  // 400
    float f_angle_ailes;  // 404
    float f_angle_sight_speed;  // 408
    float f_angle_banking_speed;  // 412
    float f_angle_ailes_speed;  // 416
    float f_angle_ailes_wait_speed;  // 420
    float f_angle_pattes_G;  // 424
    float f_sin_angle_sight;  // 428
    float f_sin_angle_ailes;  // 432
    float f_time_change_mode;  // 436
    float f_scale;  // 440
    float f_delai;  // 444
    float f_time_ground_col;  // 448
    float f_time_wall_col;  // 452
    float f_time_demi_tour;  // 456
    float f_cam_horizon_offset;  // 460
    float f_dist_me_to_target;  // 464
    float f_vert_offset;  // 468
    float f_sight_blend_speed;  // 472
    float f_time_sort;  // 476
    float f_time_GFX_Part;  // 480
    float f_death_rot_speed;  // 484
    float f_time_phase;  // 488
    float f_angle_axis;  // 492
    float f_paf_sight_side;  // 496
    float f_plug_axis_dist;  // 500
    float f_time_activation;  // 504
    float f_tail_length;  // 508
    Vec3 v_me_to_target;  // 512
    Vec3 v_hor_me_to_target;  // 524
    Vec3 v_init_offset;  // 536
    Vec3 v_init_offset_normalize;  // 548
    Vec3 v_init_banking;  // 560
    Vec3 v_normale_sol;  // 572
    Vec3 v_normale_wall;  // 584
    Vec3 v_collide_point;  // 596
    Vec3 v_paf_dir;  // 608
    Vec3 v_valid_sight;  // 620
    Vec3 v_dont_move_pos;  // 632
    Vec3 v_plug_base_pos;  // 644
    Vec3 v_paf_sight;  // 656
    Vec3 v_plug_axis;  // 668
    int o_me;  // 680
    int o_current_wp;  // 684
    int o_next_wp;  // 688
    int o_last_last_wp;  // 692
    int o_paf_sender;  // 696
    int o_snaped_actor;  // 700
    int o_locked_actor;  // 704
    int o_colmap;  // 708
    int o_aile_av_g;  // 712
    int o_aile_ar_g;  // 716
    int o_aile_av_d;  // 720
    int o_aile_ar_d;  // 724
    int o_plug;  // 728
    int o_queue;  // 732
    int n_net;  // 736
    tMessage m_msg;  // 740
    int o_FM_enemy;  // 792
    int o_last_FM_enemy;  // 796
    int i_FM_enemy_index;  // 800
    int i_FM_already_in;  // 804
    float f_energie_courante;  // 808
    float f_energie_max;  // 812
    float f_fight_force_du_coup;  // 816
    int __i_flammes_on_Dimension1;  // 820
    int i_flammes_on[26];  // 824
    int i_faire_1ereCapture_dmg;  // 928
    int i_mort_plug_sol;  // 932
    int acteur_musique_fight;  // 936
    float f_vol_freq;  // 940
    int i_sound_vol;  // 944
    int i_sound_suce;  // 948
    int i_sound_vol_freq;  // 952
    int i_bone_index;  // 956
    Vec3 v_zdm_pos;  // 960
    float f_zdm_blend_speed;  // 972
    float f_time_ailes_bzzz;  // 976
    float f_delai_ailes_bzzz;  // 980
    int i_flag_ailes_bzzz;  // 984
    int GFX_Flash;  // 988
    int GFX_Particules;  // 992
    int GFX_Halo;  // 996
    int GFX_Crache;  // 1000
    float f_snd_vol_ref;  // 1004
    float f_time_halo;  // 1008
    int i_nb_materia;  // 1012
    int i_num_next_bone;  // 1016
    float f_time_gen_materia;  // 1020
    float f_time_cine;  // 1024
    float f_save_zoom;  // 1028
    float f_morph_progression;  // 1032
    int i_last_paf_type;  // 1036
    float f_time_paf;  // 1040
    Vec3 v_impulsion_pos;  // 1044
    Vec3 v_last_speed;  // 1056
    int i_global_tresh_alpha_0_0;  // 1068
    int i_global_tresh_alpha_0_1;  // 1072
    float f_local_tresh_alpha_0_0;  // 1076
    float f_local_tresh_alpha_0_1;  // 1080
    int i_gen_materia;  // 1084
    int i_sf_jamais_la;  // 1088
    int o_fast_plug_actor;  // 1092
    float f_time_in_bv_fight;  // 1096
} tScriptStruct_ActorCrochax;

typedef struct sScriptStruct_ActorWafwaf {
    int SF_joue;  // 0
    int SF_attrape;  // 4
    int SF_app_photo;  // 8
    int SF_substitution;  // 12
    int o_point_attente;  // 16
    float f_dist_propose_attrape;  // 20
    float f_distance_fight;  // 24
    float f_mon_rayon_daction;  // 28
    int o_app_photo;  // 32
    int o_app_photo_debut;  // 36
    int o_center;  // 40
    int o_perso_a_activer_si_jai_lapp;  // 44
    Vec3 v_fuite_axe1;  // 48
    Vec3 v_fuite_axe2;  // 60
    int o_le_loup;  // 72
    Vec3 v_way_sight_force;  // 76
    float f_time_longe_mur;  // 88
    float f_longe_mur;  // 92
    int o_bones_app_photo;  // 96
    int i_joujou;  // 100
    Vec3 tag_vectorpos;  // 104
    Vec3 tag_vectorsight;  // 116
    int tag_flag;  // 128
    int tag_value;  // 132
    int tag_change_value;  // 136
    int tag_type;  // 140
    int tag_change_type;  // 144
    int tag_mode;  // 148
    int tag_change_mode;  // 152
    int tag_etage;  // 156
    int tag_etage_force;  // 160
    int __tag_2x2dyn_pos_Dimension1;  // 164
    Vec3 tag_2x2dyn_pos[4];  // 168
    float tag_size;  // 216
    float tag_change_size;  // 220
    Vec3 tag_change_free_pos1;  // 224
    Vec3 tag_free_pos1;  // 236
    Vec3 tag_change_free_pos2;  // 248
    Vec3 tag_free_pos2;  // 260
    int i_way_force_recompute;  // 272
    int i_way_moving;  // 276
    Vec3 v_way_currentpos;  // 280
    Vec3 v_way_destpos;  // 292
    Vec3 v_way_case_next;  // 304
    Vec3 v_way_case_dest;  // 316
    Vec3 v_way_speed;  // 328
    Vec3 v_way_sight;  // 340
    float f_way_blendrotate;  // 352
    int i_way_algo_limitdestpos;  // 356
    int i_way_etage_dest;  // 360
    Vec3 v_way_old_case;  // 364
    int i_way_search_around_destpos;  // 376
    int i_way_destpos_error;  // 380
    float f_way_len_big;  // 384
    float f_way_test_born;  // 388
    int i_way_case_nbr;  // 392
    float f_way_time_newcell;  // 396
    tMessage m_msg;  // 400
    float f_touching_countor;  // 452
    float f_time_attrape;  // 456
    int i_flag;  // 460
    int i_flag_sally_vers_moi;  // 464
    int i_r2_pressed;  // 468
    int i_r2_out;  // 472
    float f_time_message_r2;  // 476
    int i_snd_dodo;  // 480
} tScriptStruct_ActorWafwaf;

typedef struct sScriptStruct_ActorLuciole {
    int wp_current;  // 0
    int wp_next;  // 4
    int canmove;  // 8
    int i_cur_wp;  // 12
    int i_idhalo;  // 16
    float delay;  // 20
    float time_att;  // 24
    Vec3 add_vitesse;  // 28
    Vec3 add_vitesse1;  // 40
    Vec3 v_rand_pos;  // 52
    int i_forcehide;  // 64
    int i_ishere;  // 68
    int des_i_first_wp;  // 72
    int __des_ao_allpoints_Dimension1;  // 76
    int des_ao_allpoints[10];  // 80
    int des_i_num_points;  // 120
    int des_c_couleur;  // 124
    int des_i_test_cycle;  // 128
} tScriptStruct_ActorLuciole;

typedef struct sScriptStruct_ActorScarabeeCine {
    int des_i_morphing;  // 0
    Vec3 v_old_pos;  // 4
    int i_prog_ailes;  // 16
} tScriptStruct_ActorScarabeeCine;

typedef struct sScriptStruct_ActorCharlie {
    int i_mode_papillon;  // 0
    float f_prog;  // 4
    int des_o_wp;  // 8
    int des_o_wp1;  // 12
    float f_freq_pitch;  // 16
    float f_dist_snd;  // 20
    int i_snd_bzzz;  // 24
    int i_posing;  // 28
    float f_dist_pos;  // 32
    int ti_pose_en_cours;  // 36
    float f_time_poser;  // 40
    float f_delay_poser;  // 44
    float f_time_vole;  // 48
    float f_delay_vole;  // 52
    float i_mode_move;  // 56
    float f_time_move;  // 60
    float f_rand_move;  // 64
    Vec3 v_pos_sur_place;  // 68
    Vec3 v_last_move;  // 80
    int des_o_pos_dep;  // 92
    int i_first;  // 96
    float f_first_wait;  // 100
    float f_sf120;  // 104
    int des_i_Etage;  // 108
} tScriptStruct_ActorCharlie;

typedef struct sScriptStruct_ActorVaisseauHyllis {
    int i_lock_aircraft;  // 0
    Vec3 v_postogo;  // 4
    float f_angleh;  // 16
    float f_anglev;  // 20
    int o_next_wp;  // 24
    int i_idhaloL;  // 28
    int i_idhaloR;  // 32
    float f_alea_halo;  // 36
    float f_alea_speed;  // 40
    int i_ok_halo;  // 44
    float f_rank_z;  // 48
    int o_leader;  // 52
    Vec3 v_blend_eviting;  // 56
    Vec3 v_cur_eviting;  // 68
    int i_force_leader;  // 80
    Vec3 v_cur_sight;  // 84
    Vec3 v_forcepostogo;  // 96
    float f_forcepostogo;  // 108
    float f_speed;  // 112
    int GFX_Smoke;  // 116
    int GFX_Flamme;  // 120
    float f_time_smoke;  // 124
    int i_nb_paf;  // 128
    float f_blend_paf;  // 132
    int i_cur_replique;  // 136
    float f_time_gen;  // 140
    float des_f_dechalo;  // 144
    int des_c_colorhalo;  // 148
    float des_f_nervosite;  // 152
    float des_f_max_speed;  // 156
    float des_f_blend_speed;  // 160
    float des_f_blend_sight;  // 164
    float des_f_blend_angleh;  // 168
    float des_f_blend_anglev;  // 172
    float f_speed_pal;  // 176
    int des_o_network;  // 180
    int des_o_beg;  // 184
    int des_o_init;  // 188
    int des_i_id;  // 192
    int des_i_canmovez;  // 196
    int des_o_text;  // 200
    int des_i_replique;  // 204
    int des_i_can_wait;  // 208
    int des_o_flic;  // 212
    int i_snd_indice_loop;  // 216
    int des_i_sf_je_suis_la;  // 220
    float f_freq;  // 224
    float f_rot_blend_speed;  // 228
    float f_banking_coef;  // 232
    int i_flag_gameover;  // 236
    float f_time_crash;  // 240
    int o_gameover_explosion;  // 244
} tScriptStruct_ActorVaisseauHyllis;

typedef struct sScriptStruct_INVCacheObjet {
    int des_o_obj;  // 0
    int deja;  // 4
} tScriptStruct_INVCacheObjet;

typedef struct sScriptStruct_HelperText {
    int des_i_actor_type1;  // 0
    int des_o_actor_detec1;  // 4
    int des_o_actor_zone1;  // 8
    int des_i_collision_type2;  // 12
    int des_o_actor_detec2;  // 16
    int des_o_actor_zone2;  // 20
    int des_o_actor_status1;  // 24
    int des_i_activity_test1;  // 28
    int des_o_actor_capa1;  // 32
    int des_i_capa_test1;  // 36
    int des_i_capa_etat1;  // 40
    int des_o_actor_cbit1;  // 44
    int des_i_cbit_test1;  // 48
    int des_i_cbit_etat1;  // 52
    int des_o_actor_visible1;  // 56
    float des_f_aire_visibilite;  // 60
    int des_i_specialFlag_test1;  // 64
    int des_i_specialFlag_test2;  // 68
    int des_o_acteur_msg;  // 72
    int des_i_msg_ID;  // 76
    int des_i_msg_INT1;  // 80
    int des_o_actor_desstruct;  // 84
    int des_i_desstruct_flag;  // 88
    int des_i_desstruct_num;  // 92
    int des_i_desstruct_int;  // 96
    float des_f_tempo;  // 100
    int des_i_SPFlag_associe_vrai;  // 104
    int des_i_SPFlag_associe_faux;  // 108
    int des_i_ripley;  // 112
    int des_i_blocage_joy;  // 116
    int des_i_look_interdit;  // 120
    int des_i_inventaire_interdit;  // 124
    int __ao_objets_collisions_Dimension1;  // 128
    int ao_objets_collisions[10];  // 132
    int i_ready;  // 172
    int i_tests_a_reussir;  // 176
    float f_temps_ecoule;  // 180
    int i_playing;  // 184
    int f_track_init;  // 188
    int i_replay_loop;  // 192
    int des_i_prio_des_textes;  // 196
    int des_i_choix_aleatoire;  // 200
    int i_Sound_Localisation_type;  // 204
    int __des_at_lignes_texte_Dimension1;  // 208
    tTextGet des_at_lignes_texte[5];  // 212
    int __des_ai_encart_Dimension1;  // 252
    int des_ai_encart[5];  // 256
    int __des_ai_son_associe_Dimension1;  // 276
    int des_ai_son_associe[5];  // 280
    int __des_af_volume_associe_Dimension1;  // 300
    float des_af_volume_associe[5];  // 304
    int __des_af_near_associe_Dimension1;  // 324
    float des_af_near_associe[5];  // 328
    int __des_af_far_associe_Dimension1;  // 348
    float des_af_far_associe[5];  // 352
    int __des_ai_perso_localisation_Dimension1;  // 372
    int des_ai_perso_localisation[5];  // 376
    int __des_ao_acteur_dialog_Dimension1;  // 396
    int des_ao_acteur_dialog[5];  // 400
    float des_i_auto_delay;  // 420
    int __des_f_duree_dialogues_Dimension1;  // 424
    float des_f_duree_dialogues[5];  // 428
    int des_i_cine_temporaire;  // 448
    int des_i_Flag_debut;  // 452
    int i_index_ligne;  // 456
    float f_tempo;  // 460
    float f_temps;  // 464
    int i_display_once;  // 468
    int i_sfx_temp;  // 472
    float f_dist_abort;  // 476
    int i_was_ready;  // 480
} tScriptStruct_HelperText;

typedef struct sScriptStruct_HoverCraftShoot {
    int o_Target;  // 0
    float f_Distance;  // 4
    int i_Halo;  // 8
    Vec3 v_Sight;  // 12
    int i_flag_1st_trame;  // 24
    int i_flag_target;  // 28
    int i_flag_init_done;  // 32
    int GFX_Line;  // 36
    int GFX_Halo;  // 40
    int GFX_Halo_2;  // 44
    int GFX_Flash;  // 48
    float f_speed;  // 52
    float f_line_length;  // 56
    float f_time_impact;  // 60
    float f_time_start;  // 64
    float f_XMEN_width;  // 68
    float f_delay_inertie;  // 72
    Vec3 v_rand_bv_pos;  // 76
    Vec3 v_last_pos;  // 88
    Vec3 v_aircraft_start_speed;  // 100
    int o_main_actor;  // 112
    int o_aircraft;  // 116
    float f_shoot_vol;  // 120
    int i_flag_perfect_shoot;  // 124
    int i_flag_sight_set;  // 128
} tScriptStruct_HoverCraftShoot;

typedef struct sScriptStruct_GSTHyllisManager {
    int __des_ao_allzones_Dimension1;  // 0
    int des_ao_allzones[50];  // 4
    int des_i_numzones;  // 204
    int __des_ao_allgp_Dimension1;  // 208
    int des_ao_allgp[50];  // 212
    int des_i_numgp;  // 412
    float des_f_freq;  // 416
    int i_last_gp;  // 420
    int i_wait_sf;  // 424
    float f_time;  // 428
    int i_with_time;  // 432
} tScriptStruct_GSTHyllisManager;

typedef struct sScriptStruct_Detect1SpecialFlag {
    int SF_a_tester;  // 0
    int __SF_a_VRAI_Dimension1;  // 4
    int SF_a_VRAI[10];  // 8
    int __SF_a_FAUX_Dimension1;  // 48
    int SF_a_FAUX[10];  // 52
    int SF_Destroy_on_init;  // 92
    int SF_Destroy_if_test_ok;  // 96
    int delay_not_first_trame;  // 100
    float f_wait_before_destroy;  // 104
    int first_trame;  // 108
} tScriptStruct_Detect1SpecialFlag;

typedef struct sScriptStruct_MoveGrid {
    float des_f_distance;  // 0
    int des_i_grid1;  // 4
} tScriptStruct_MoveGrid;

typedef struct sScriptStruct_ObjetFlottant {
    int des_i_settings;  // 0
    int des_mobilite;  // 4
    int des_rotation;  // 8
    int __des_o_obj_list_Dimension1;  // 12
    int des_o_obj_list[5];  // 16
    float des_f_dist_max;  // 36
    int i_optim_VF;  // 40
    int des_i_sf_je_suis_la;  // 44
    int des_wait_until_sf;  // 48
    int des_SF_destruction;  // 52
    Vec3 des_v_init_speed;  // 56
    Vec3 des_v_grav_init_speed;  // 68
    float des_f_base_gravity;  // 80
    float des_f_perso_gravity;  // 84
    float des_f_aircraft_gravity;  // 88
    Vec3 des_v_water_friction;  // 92
    Vec3 des_v_ground_friction;  // 104
    float des_f_ApplyRecFactor;  // 116
    float des_f_rebound_coef;  // 120
    float des_f_rot_speed_ground;  // 124
    float des_f_rot_speed_water;  // 128
    float des_f_rot_friction_ground;  // 132
    float des_f_rot_friction_water;  // 136
    float des_f_archimede;  // 140
    float des_f_archimede_offset;  // 144
    float des_f_dist_centre_grav;  // 148
    float des_f_water_gravity;  // 152
    float des_f_water_friction;  // 156
    float des_f_air_gravity;  // 160
    float des_f_air_friction;  // 164
    float des_f_coef_banking_perso;  // 168
    float des_f_coef_banking_aircraft;  // 172
    int des_mode;  // 176
    int des_resistance;  // 180
    int des_energie;  // 184
    int des_i_explosion_type;  // 188
    int des_i_explosion_size;  // 192
    int des_i_reward_force;  // 196
    int des_i_GFX_Smoke;  // 200
    int des_Perle_ID;  // 204
    Vec3 des_v_offset_reward;  // 208
    int __o_explode_me_Dimension1;  // 220
    int o_explode_me[5];  // 224
    int o_shoot_hover;  // 244
    int des_SF_mort_ID;  // 248
    int SV_objet_position_ID;  // 252
    int i_flag_in_water;  // 256
    int i_flag_ignore_hierachy;  // 260
    int i_nb_fils;  // 264
    int GFX_Airtrace;  // 268
    int i_obj_sup_nb;  // 272
    int i_flag_LOD;  // 276
    int i_flag_real_water;  // 280
    int i_id_reward;  // 284
    int GFX_Halo;  // 288
    int i_flag_1st_ground_col;  // 292
    int i_flag_ejection_allowed;  // 296
    float f_water_Z;  // 300
    float f_last_valid_water_Z;  // 304
    float f_rotation_speed;  // 308
    float f_last_speed;  // 312
    float f_time_ground_col;  // 316
    float f_time_air;  // 320
    float f_time_paf;  // 324
    float f_time_ignore_hier;  // 328
    float f_time_dyn_ground_col;  // 332
    int __f_time_dyn_hier_Dimension1;  // 336
    float f_time_dyn_hier[10];  // 340
    float f_time_water_col;  // 380
    float f_zdm_rayon;  // 384
    float f_sqr_dist_max;  // 388
    float f_time_main_col;  // 392
    float f_time_ref;  // 396
    Vec3 v_water_force;  // 400
    Vec3 v_grav_pos;  // 412
    Vec3 v_grav_speed;  // 424
    Vec3 v_normale_sol;  // 436
    Vec3 v_last_pos;  // 448
    Vec3 v_last_collide_pos;  // 460
    Vec3 v_last_speed;  // 472
    Vec3 v_reward_ref_pos;  // 484
    Vec3 v_offset_water;  // 496
    int __ao_hierarchy_object_Dimension1;  // 508
    int ao_hierarchy_object[10];  // 512
    int o_reward_dup_gao;  // 552
    float f_snd_coef_vol_choc;  // 556
    float f_snd_time_choc;  // 560
    int GFX_Fire;  // 564
    int GFX_Smoke;  // 568
    int i_expulse_special;  // 572
    int i_max_gen_materia;  // 576
    int i_flag_msg_send;  // 580
    int o_reward_dup_gao_extra;  // 584
} tScriptStruct_ObjetFlottant;

typedef struct sScriptStruct_caisse {
    int des_o_caisse;  // 0
    int des_i_num;  // 4
    int __ao_all_caisses_Dimension1;  // 8
    int ao_all_caisses[50];  // 12
    int hide;  // 212
    float des_f_dist_hide;  // 216
    int des_i_sf_jesuisla;  // 220
} tScriptStruct_caisse;

typedef struct sScriptStruct_DEMOESSelect {
    int __ao_ES_Dimension1;  // 0
    int ao_ES[5];  // 4
    tTextGet Nom_De_L_Entree_Radar;  // 24
    int i_ES_valide;  // 32
} tScriptStruct_DEMOESSelect;

typedef struct sScriptStruct_ActorDepanneuse {
    int des_o_net;  // 0
    int des_o_bv_panne;  // 4
    int des_o_bv_panne1;  // 8
    int des_o_es_wp;  // 12
    int des_o_text;  // 16
    int des_o_cam_arrivee;  // 20
    int des_o_first_wp;  // 24
    int des_ventouse;  // 28
    Vec3 v_destpos;  // 32
    int o_next_wp;  // 44
    int o_end_wp;  // 48
    float f_dist_aircraft_debut;  // 52
    float f_inertie_speed;  // 56
    float f_cur_speed;  // 60
    float f_dest_speed;  // 64
    int i_grappin_donne;  // 68
    int o_aircraft;  // 72
    int i_replique;  // 76
    float f_time_grap;  // 80
    int i_idhaloL;  // 84
    int i_idhaloR;  // 88
    float f_last_z;  // 92
    float f_time_retour;  // 96
    float f_delay_retour;  // 100
    int GFX_Line;  // 104
    int o_ventouse;  // 108
    tMessage m_msg;  // 112
    float f_dist_cam;  // 164
    int i_peut_y_aller;  // 168
    int i_je_monte;  // 172
    int i_retour;  // 176
    float f_surround;  // 180
    float f_Z_cam;  // 184
    Vec3 v_cam_axis;  // 188
    int i_sound;  // 200
    int en_route;  // 204
} tScriptStruct_ActorDepanneuse;

typedef struct sScriptStruct_AirCraftVentouse {
    float des_f_life_time;  // 0
    float des_f_traction;  // 4
    float des_f_init_speed;  // 8
    float des_f_rot_coef_blend_speed;  // 12
    float des_f_rotation_speed;  // 16
    float des_f_rotation_speed_max;  // 20
    float des_f_friction;  // 24
    float des_f_friction_max;  // 28
    float des_f_friction_blend_speed;  // 32
    Vec3 des_v_gravity;  // 36
    int GFX_Halo;  // 48
    int c_color;  // 52
    float f_time_start;  // 56
    float f_time_start_fade;  // 60
    float f_largeur_X_men;  // 64
    Vec3 v_offset;  // 68
    int o_sender;  // 80
    tMessage m_msg;  // 84
    float f_time_disp;  // 136
    float f_size_halo;  // 140
    int i_snd_loop;  // 144
    float f_snd_vol;  // 148
    float f_snd_freq;  // 152
    int i_snd_col;  // 156
} tScriptStruct_AirCraftVentouse;

typedef struct sScriptStruct_VehiculeMaxSpeedTest {
    int des_actor;  // 0
    float des_speed_min;  // 4
    float des_speed_max;  // 8
    int des_saut_autorise;  // 12
    int des_boost_autorise;  // 16
    int des_shoot_autorise;  // 20
} tScriptStruct_VehiculeMaxSpeedTest;

typedef struct sScriptStruct_SFXPasPoussiere {
    int des_c_color;  // 0
    float des_f_alpha;  // 4
    int des_i_on_bv;  // 8
    int des_i_on_cbc;  // 12
    int des_i_on_terre;  // 16
    int i_generated;  // 20
    float f_time_gen;  // 24
    int done;  // 28
    int i_actor;  // 32
    int o_test_obj;  // 36
    int i_ready;  // 40
} tScriptStruct_SFXPasPoussiere;

typedef struct sScriptStruct_SFXArrose {
    int des_SF_desactivation;  // 0
    int des_snap_actor;  // 4
    int des_bone_index;  // 8
    int des_o_BV;  // 12
    int des_mat_gao;  // 16
    int des_mat_1;  // 20
    int des_mat_2;  // 24
    int des_mat_3;  // 28
    int des_mat_4;  // 32
    int des_particules_buffer;  // 36
    float des_f_time_phase_1;  // 40
    float des_f_time_phase_2;  // 44
    Vec3 des_v_friction;  // 48
    float des_f_gravity;  // 60
    float des_f_wind;  // 64
    int des_c_1st_color;  // 68
    int des_c_2nd_color;  // 72
    int des_c_3rd_color;  // 76
    float des_f_growing_speed_min;  // 80
    float des_f_growing_speed_max;  // 84
    float des_f_growing_speed_friction;  // 88
    float des_f_time_random;  // 92
    float des_f_gen_rate_ON;  // 96
    float des_f_ON_size_min;  // 100
    float des_f_ON_size_max;  // 104
    float des_f_ON_speed_min;  // 108
    float des_f_ON_speed_max;  // 112
    int GFX;  // 116
    int o_bone;  // 120
} tScriptStruct_SFXArrose;

typedef struct sScriptStruct_ActorJeTeParle {
    int i_compotement_specifique;  // 0
    int i_interdit_pendant_mili_malad;  // 4
    float des_f_distance_talk;  // 8
    float des_f_angle_talk;  // 12
    int __ai_conditions_Dimension1;  // 16
    int __ai_conditions_Dimension2;  // 20
    int ai_conditions[80];  // 24
    int __at_questionsReponses_Dimension1;  // 344
    int __at_questionsReponses_Dimension2;  // 348
    tTextGet at_questionsReponses[40];  // 352
    int __ai_consequences_Dimension1;  // 672
    int __ai_consequences_Dimension2;  // 676
    int ai_consequences[60];  // 680
    int __ai_consequences_plus_Dimension1;  // 920
    int __ai_consequences_plus_Dimension2;  // 924
    int ai_consequences_plus[140];  // 928
    int __ai_grisage_Dimension1;  // 1488
    int ai_grisage[20];  // 1492
    int __at_question_reponse_OK_Dimension1;  // 1572
    tTextGet at_question_reponse_OK[2];  // 1576
    int i_dialogue_on;  // 1592
    tMessage m_dialogue;  // 1596
    int Valide_Effet_1_Question;  // 1648
    int o_papa;  // 1652
    int i_talk_propose;  // 1656
    float f_time_fin_talk;  // 1660
    float f_dist_regard;  // 1664
    int o_regard_capa;  // 1668
    int des_i_type_acteur;  // 1672
    int des_o_acteur_suividesyeux;  // 1676
    Vec3 des_v_hauteur_cible;  // 1680
    int des_i_RT_flags;  // 1692
    float des_f_RT_vitesse_de_rotation;  // 1696
    Vec3 macro_regard_aleatoire_v_curr;  // 1700
    Vec3 macro_regard_aleatoire_v_targ;  // 1712
    Vec3 macro_regard_aleatoire_v_inte;  // 1724
    int macro_regard_aleatoire_i_cpt;  // 1736
    float f_vitesse_je_regarde_Sally;  // 1740
    int i_ne_tourne_que_la_tete;  // 1744
    int i_regarde_si_talk;  // 1748
    int i_custom_limit;  // 1752
    int i_regarder_dans_le_dos;  // 1756
    int o_perso_a_regarder;  // 1760
    int __des_ai_sf_pos_Dimension1;  // 1764
    int des_ai_sf_pos[10];  // 1768
    int __des_ao_sf_pos_Dimension1;  // 1808
    int des_ao_sf_pos[10];  // 1812
    float f_angle;  // 1852
    float f_angle_target;  // 1856
    int i_rot_mamago;  // 1860
    int des_i_animaux;  // 1864
    int des_i_etage;  // 1868
    int des_i_sf_jesuis_la;  // 1872
    int des_o_jesuisla_desactive;  // 1876
    int des_o_jesuisla_active;  // 1880
    float f_time_revol;  // 1884
    int des_o_panneau;  // 1888
    float f_rand_revol;  // 1892
    int i_shape_main_revol;  // 1896
    float f_offset_pivot;  // 1900
    int i_act_revol;  // 1904
} tScriptStruct_ActorJeTeParle;

typedef struct sScriptStruct_ActorVendeur {
    int des_i_vendeur_ID;  // 0
    int des_i_SF_show_lecteur_code;  // 4
    int des_i_SF_pay_x_pognes;  // 8
    int i_nb_pognes;  // 12
    int des_monaie;  // 16
    int des_i_type;  // 20
    int des_o_ma_BV;  // 24
    int des_o_acteur_txt;  // 28
    int i_pas_la_peine_de_carte;  // 32
    int des_o_perle;  // 36
    int des_o_mamago_repar;  // 40
    int des_o_mamago_decor;  // 44
    int des_o_mamago_decor1;  // 48
    int des_o_mamago_decor_f;  // 52
    int des_o_sound_mamago;  // 56
    int correspondance_valeur_objet;  // 60
    int __des_i_art_type_Dimension1;  // 64
    int des_i_art_type[10];  // 68
    int __des_o_art_wp_Dimension1;  // 108
    int des_o_art_wp[10];  // 112
    int __des_i_art_qte_Dimension1;  // 152
    int des_i_art_qte[10];  // 156
    int __des_i_art_prix_vente_Dimension1;  // 196
    int des_i_art_prix_vente[10];  // 200
    int __des_i_art_prix_achat_Dimension1;  // 240
    int des_i_art_prix_achat[10];  // 244
    int __des_i_art_SF_Dimension1;  // 284
    int des_i_art_SF[10];  // 288
    int __des_i_art_if_SF_Dimension1;  // 328
    int des_i_art_if_SF[10];  // 332
    int __des_i_destinataire_Dimension1;  // 372
    int des_i_destinataire[10];  // 376
    int __des_i_art_SELL_if_SF_Dimension1;  // 416
    int des_i_art_SELL_if_SF[10];  // 420
    int __des_i_TXT_index_Dimension1;  // 460
    int des_i_TXT_index[10];  // 464
    int __des_i_TXT_index_vente_Dimension1;  // 504
    int des_i_TXT_index_vente[10];  // 508
    int __des_o_art_cacher1_Dimension1;  // 548
    int des_o_art_cacher1[10];  // 552
    int __des_o_art_cacher2_Dimension1;  // 592
    int des_o_art_cacher2[10];  // 596
    int __des_o_art_cacher3_Dimension1;  // 636
    int des_o_art_cacher3[10];  // 640
    int __des_o_art_cacher4_Dimension1;  // 680
    int des_o_art_cacher4[10];  // 684
    int __des_o_art_cacher5_Dimension1;  // 724
    int des_o_art_cacher5[10];  // 728
    int __des_o_art_Prefetch_Dimension1;  // 768
    int des_o_art_Prefetch[10];  // 772
    float des_f_dist_buy;  // 812
    float des_f_delai_remplacement;  // 816
    int des_o_lecteur_code;  // 820
    int i_interdit_pendant_mili_malad;  // 824
    float des_f_distance_talk;  // 828
    float des_f_angle_talk;  // 832
    int __ai_conditions_Dimension1;  // 836
    int __ai_conditions_Dimension2;  // 840
    int ai_conditions[80];  // 844
    int __at_questionsReponses_Dimension1;  // 1164
    int __at_questionsReponses_Dimension2;  // 1168
    tTextGet at_questionsReponses[40];  // 1172
    int __ai_consequences_Dimension1;  // 1492
    int __ai_consequences_Dimension2;  // 1496
    int ai_consequences[60];  // 1500
    int __ai_consequences_plus_Dimension1;  // 1740
    int __ai_consequences_plus_Dimension2;  // 1744
    int ai_consequences_plus[140];  // 1748
    int __ai_grisage_Dimension1;  // 2308
    int ai_grisage[20];  // 2312
    int __at_question_reponse_OK_Dimension1;  // 2392
    tTextGet at_question_reponse_OK[2];  // 2396
    int i_dialogue_on;  // 2412
    tMessage m_dialogue;  // 2416
    int Valide_Effet_1_Question;  // 2468
    int o_papa;  // 2472
    int i_talk_propose;  // 2476
    float f_time_fin_talk;  // 2480
    int des_o_cam_wp;  // 2484
    int i_SF_gestion_pognes;  // 2488
    int i_nb_articles;  // 2492
    int i_last_art_index;  // 2496
    int i_flag_vente;  // 2500
    int i_dialog_index;  // 2504
    int i_flag_talk;  // 2508
    int i_last_thune;  // 2512
    int i_transaction_done;  // 2516
    int i_reset_to_do;  // 2520
    int __i_show_art_Dimension1;  // 2524
    int i_show_art[10];  // 2528
    int i_flag_je_desactive_la_camera;  // 2568
    float f_time_decompte;  // 2572
    float f_time_transaction_done;  // 2576
    tMessage m_msg;  // 2580
    int i_comportement;  // 2632
    int __o_pos_acheteur_Dimension1;  // 2636
    int o_pos_acheteur[5];  // 2640
    int i_status1;  // 2660
    int i_status2;  // 2664
    float f_counter;  // 2668
    int i_active_la_rotationtete;  // 2672
    float f_dist_regarde_sally;  // 2676
    int i_ne_tourne_que_la_tete;  // 2680
    int i_regarde_si_talk;  // 2684
    int des_i_type_acteur;  // 2688
    int des_o_acteur_suividesyeux;  // 2692
    Vec3 des_v_hauteur_cible;  // 2696
    int des_i_RT_flags;  // 2708
    float des_f_RT_vitesse_de_rotation;  // 2712
    int i_mamago_replique;  // 2716
    int i_swap_force;  // 2720
    int i_snd_loop;  // 2724
    int des_i_animaux;  // 2728
    int des_i_etage;  // 2732
    float f_time_stop_mamago;  // 2736
} tScriptStruct_ActorVendeur;

typedef struct sScriptStruct_DistribCrunsh {
    float des_f_distance;  // 0
    float angle_de_proposition;  // 4
    float f_offset_Z_regard;  // 8
    int des_o_distrib;  // 12
    int __des_ao_contenu_Dimension1;  // 16
    int des_ao_contenu[6];  // 20
    int __des_ai_prix_Dimension1;  // 44
    int des_ai_prix[6];  // 48
    int __des_at_prix_Dimension1;  // 72
    tTextGet des_at_prix[6];  // 76
    int __des_at_noms_Dimension1;  // 124
    tTextGet des_at_noms[6];  // 128
    int __des_at_explication_Dimension1;  // 176
    tTextGet des_at_explication[6];  // 180
    int __des_ai_son_Dimension1;  // 228
    int des_ai_son[6];  // 232
    int __des_f_offset_Z_Dimension1;  // 256
    float des_f_offset_Z[6];  // 260
    int i_current;  // 284
    int i_bouge_sens;  // 288
    int i_init_bouge;  // 292
    Vec3 v_centre_disque;  // 296
    float f_hauteur_disque;  // 308
    float f_rayon_disque;  // 312
    int __av_dest_Dimension1;  // 316
    Vec3 av_dest[10];  // 320
    Vec3 v_pos_cam;  // 440
    Vec3 v_last_pos_cam;  // 452
    Vec3 v_last_sight_cam;  // 464
    Vec3 v_rot_plateau;  // 476
    int i_swap_force;  // 488
    int i_mode_texte;  // 492
    float f_time_texte;  // 496
    int i_force_expl;  // 500
    float f_time_debut;  // 504
    int i_last_snd;  // 508
    int i_snd;  // 512
} tScriptStruct_DistribCrunsh;

typedef struct sScriptStruct_IndiceAvancementJeu
{
    int des_i_companion;
    int des_i_niveau;
    int __ai_conditions_Dimension1;
    int ai_conditions[20];
    int __at_questionsReponses_Dimension1;
    int __at_questionsReponses_Dimension2;
    tTextGet at_questionsReponses[40];
    int __at_question_reponse_OK_Dimension1;
    tTextGet at_question_reponse_OK[2];
    int __ai_grisage_Dimension1;
    int ai_grisage[20];
} tScriptStruct_IndiceAvancementJeu;

typedef struct sScriptStruct_DialogueResetAllSF {
    int __des_ai_SF_to_reset_Dimension1;  // 0
    int des_ai_SF_to_reset[30];  // 4
} tScriptStruct_DialogueResetAllSF;

typedef struct sScriptStruct_LipsyncAuto {
    int des_o_acteur;  // 0
    int des_i_type_acteur;  // 4
    int i_shape_canal_tete;  // 8
    int i_canal_tete_ouverture_bouche;  // 12
    int i_expr_bouche_auto;  // 16
    int i_expr_bouche_blabla_id;  // 20
    int __f_expr_factor_target_Dimension1;  // 24
    float f_expr_factor_target[5];  // 28
    int __f_expr_prog_current_Dimension1;  // 48
    float f_expr_prog_current[5];  // 52
    int __f_expr_factor_current_Dimension1;  // 72
    float f_expr_factor_current[5];  // 76
    int __f_expr_forme_Dimension1;  // 96
    float f_expr_forme[5];  // 100
    int __f_expr_flag_Dimension1;  // 120
    float f_expr_flag[5];  // 124
    int __f_expr_speed_Dimension1;  // 144
    float f_expr_speed[5];  // 148
    tMessage m_blink;  // 168
    int __ai_expr_bouche_blabla_liste_Dimension1;  // 220
    int ai_expr_bouche_blabla_liste[32];  // 224
} tScriptStruct_LipsyncAuto;

typedef struct sScriptStruct_CinePerleTransformation {
    int o_Perle;  // 0
    int o_Halo;  // 4
    int o_Morph;  // 8
    float f_Timer;  // 12
    float f_RLIDestFactor;  // 16
    float f_speed;  // 20
    int GFX_Flash;  // 24
} tScriptStruct_CinePerleTransformation;

typedef struct sScriptStruct_PerleBase {
    int des_perso_a_suivre;  // 0
    int des_bone_a_suivre;  // 4
    int des_haloperie;  // 8
    int i_transit;  // 12
    float des_f_vitesse;  // 16
    float des_f_amplitude;  // 20
    int des_suivre_taille;  // 24
    Vec3 v_pos_start;  // 28
    Vec3 v_sight_start;  // 40
    Vec3 v_bank_start;  // 52
    float f_zoom_start;  // 64
    float f_zoom_end;  // 68
    float f_prev_amplitude;  // 72
    float f_temps;  // 76
    float f_angle;  // 80
    float des_f_dist_sound;  // 84
    int i_SND_loop;  // 88
} tScriptStruct_PerleBase;

typedef struct sScriptStruct_Perle {
    int des_mode;  // 0
    int des_Perle_ID;  // 4
    int des_i_prix_vente;  // 8
    int des_o_crochax;  // 12
    int des_i_special_flag;  // 16
    int des_i_SF_jesuisla;  // 20
    int des_o_plug_to;  // 24
    Vec3 des_v_offset_plug;  // 28
    int peut_prendre;  // 40
    int des_i_interdire_en_fight;  // 44
    float des_f_dist_attrape;  // 48
    float des_f_dist_prendre;  // 52
    float des_f_dist_z;  // 56
    float des_f_size;  // 60
    float des_f_mode_z;  // 64
    int peut_prendre_si_capa;  // 68
    float des_f_zoom_normal;  // 72
    float des_f_angle_prendre;  // 76
    float f_dist_sound;  // 80
    int i_SND_loop;  // 84
    int des_i_dummyspoon;  // 88
    float f_time_capture;  // 92
    int o_afterrec_gao;  // 96
    int i_idhalo;  // 100
    int i_mort;  // 104
    int o_plug_to;  // 108
    int i_plug_to;  // 112
    Vec3 v_plug_offset;  // 116
    int GFX_Flash;  // 128
    int gotoz;  // 132
    int i_gen_halos;  // 136
    int i_inventaire_done;  // 140
    int i_capa_passe;  // 144
    float f_GS_indice_blend;  // 148
    int i_GS_init;  // 152
    Vec3 v_GS_axis_D;  // 156
    int i_last_thune;  // 168
    int i_bip;  // 172
    float f_time_decompte;  // 176
    int i_snd_loop;  // 180
} tScriptStruct_Perle;

typedef struct sScriptStruct_Armoire {
    int switch_SF_reste_ouverte;  // 0
    int switch_condition_ouverture;  // 4
    int switch_etat_init;  // 8
    int a_activer_a_louverture1;  // 12
    int a_activer_a_louverture2;  // 16
    int a_activer_a_louverture3;  // 20
    int o_activer_si_ouvert;  // 24
} tScriptStruct_Armoire;

typedef struct sScriptStruct_Detect_o_Capa {
    int __des_o_actor_capa_Dimension1;  // 0
    int des_o_actor_capa[10];  // 4
    int des_i_capa_test;  // 44
    int des_i_type_test;  // 48
    int des_i_SFlag;  // 52
} tScriptStruct_Detect_o_Capa;

typedef struct sScriptStruct_LecteurCode {
    int des_SF_Indice_1er_Journal;  // 0
    int prix_abonnement;  // 4
    int des_i_code;  // 8
    float des_i_distance;  // 12
    int des_SF_valide;  // 16
    float angle_de_proposition;  // 20
    float f_offset_Z_regard;  // 24
    int o_other_capa;  // 28
    int i_pas_tout_le_temps;  // 32
} tScriptStruct_LecteurCode;

typedef struct sScriptStruct_PorteArmoire {
    int des_test;  // 0
    int des_propose_close;  // 4
    float des_f_dist_open;  // 8
    float des_f_porte_largeur;  // 12
    float des_f_cone;  // 16
    int des_axe;  // 20
    int des_angle_sign;  // 24
    float des_f_angle_min;  // 28
    float des_f_angle_max;  // 32
    float des_f_angle_acceleration;  // 36
    float des_f_open_speed;  // 40
    float des_f_close_speed;  // 44
    float des_f_rebound_speed;  // 48
    float des_f_rebound_min_friction;  // 52
    float des_f_rebound_max_friction;  // 56
    int des_SF_porte_ouverte;  // 60
    int des_o_battant;  // 64
    int des_o_contenu;  // 68
    int des_o_contenu1;  // 72
    int des_o_contenu2;  // 76
    int des_o_contenu3;  // 80
    float des_f_time_active_contenu;  // 84
    int i_desactive_qd_ferme_contenu;  // 88
    int i_desactive_qd_ferme_contenu1;  // 92
    int i_desactive_qd_ferme_contenu2;  // 96
    int i_desactive_qd_ferme_contenu3;  // 100
    int o_regarde_la;  // 104
    int des_sound_event;  // 108
    float des_sound_event_portee;  // 112
    float des_sound_event_duree;  // 116
    int des_sound_event_open_pos;  // 120
    int des_sound_event_close_pos;  // 124
    float f_angle;  // 128
    float f_angle_speed;  // 132
    float f_time_open;  // 136
    int i_current_mode;  // 140
    int i_flag_porte_ouverte;  // 144
    int i_flag_2nd_trame;  // 148
    int i_open_once;  // 152
    tMessage m_msg;  // 156
} tScriptStruct_PorteArmoire;

typedef struct sScriptStruct_SNDSfnDelay {
    int o_BV;  // 0
    int i_inout;  // 4
    int i_SF;  // 8
    int i_SF_Stop;  // 12
    float f_delay_min;  // 16
    float f_delay_max;  // 20
    float f_delay;  // 24
    float f_delay_init_min;  // 28
    float f_delay_init_max;  // 32
    float f_delay_add_min;  // 36
    float f_delay_add_max;  // 40
    int o_capa;  // 44
    int i_rand_play;  // 48
    float f_rand_pos;  // 52
    float f_time_start;  // 56
} tScriptStruct_SNDSfnDelay;

typedef struct sScriptStruct_SFXPhare {
    int des_SF_activation;  // 0
    int des_SF_desactivation;  // 4
    int des_SFX_Sun;  // 8
    int __des_o_wp_Dimension1;  // 12
    int des_o_wp[5];  // 16
    float des_f_rot_speed;  // 36
    float des_f_angle_obscure;  // 40
    float des_Z_offset;  // 44
    int des_halo_2;  // 48
    int des_halo_3;  // 52
    float des_f_flash_extrude;  // 56
    float des_f_light_length;  // 60
    int des_halo_detail;  // 64
    int des_c_halo;  // 68
    float des_f_halo_angle;  // 72
    int o_son_lumiere;  // 76
    int i_nb_wp;  // 80
    int i_wp_index;  // 84
    int i_flag_en_mode_look;  // 88
    float f_angle;  // 92
    int __af_angle_Dimension1;  // 96
    float af_angle[5];  // 100
    float f_dist_to_cam;  // 120
    float f_Halo_color;  // 124
    float f_time_light_ON;  // 128
    int o_camera;  // 132
    int c_flash_color;  // 136
    int GFX_Halo;  // 140
    int GFX_Halo_2;  // 144
    int GFX_Halo_3;  // 148
    int GFX_Flash;  // 152
    int i_snd_loop;  // 156
    int i_snd_loop2;  // 160
    int i_snd_loop3;  // 164
} tScriptStruct_SFXPhare;

typedef struct sScriptStruct_SNDMusiqueJourNuitZone {
    int i_track;  // 0
    float f_heure_debjour;  // 4
    float f_heure_finjour;  // 8
    int o_zone_ambiancelocale;  // 12
    int i_playing;  // 16
    int i_playing_old;  // 20
    int i_jour;  // 24
    int i_jour_old;  // 28
} tScriptStruct_SNDMusiqueJourNuitZone;

typedef struct sScriptStruct_Mobile {
    int i_flag_sally_out_BV;  // 0
    int i_flag_reinit_done;  // 4
    float f_speed_rotation;  // 8
    float f_time_culled;  // 12
    Vec3 v_grav_pos;  // 16
    Vec3 v_grav_speed;  // 28
} tScriptStruct_Mobile;

typedef struct sScriptStruct_RandomTrackPlayer {
    int i_nb_track;  // 0
    float f_temps_pause_min;  // 4
    float f_temps_pause_Max;  // 8
    float f_heure_deb;  // 12
    float f_heure_fin;  // 16
    float f_time_pause;  // 20
    float f_time_fin;  // 24
    int i_track_crt;  // 28
    int i_track_last;  // 32
} tScriptStruct_RandomTrackPlayer;

typedef struct sScriptStruct_GrillonCigales {
    int __af_heure_deb_Dimension1;  // 0
    float af_heure_deb[4];  // 4
    int __af_heure_fin_Dimension1;  // 20
    float af_heure_fin[4];  // 24
    int __ai_indice_Dimension1;  // 40
    int ai_indice[4];  // 44
    int __af_duree_Dimension1;  // 60
    float af_duree[4];  // 64
    int __ai_nb_min_Dimension1;  // 80
    int ai_nb_min[4];  // 84
    int __ai_nb_max_Dimension1;  // 100
    int ai_nb_max[4];  // 104
    int __af_pause_min_Dimension1;  // 120
    float af_pause_min[4];  // 124
    int __af_pause_max_Dimension1;  // 140
    float af_pause_max[4];  // 144
    int __ai_nb_bis_min_Dimension1;  // 160
    int ai_nb_bis_min[4];  // 164
    int __ai_nb_bis_max_Dimension1;  // 180
    int ai_nb_bis_max[4];  // 184
    int __af_pause_bis_min_Dimension1;  // 200
    float af_pause_bis_min[4];  // 204
    int __af_pause_bis_max_Dimension1;  // 220
    float af_pause_bis_max[4];  // 224
    int i_ind_snd;  // 240
    float f_time_crt;  // 244
    float f_time_boucle_1;  // 248
    float f_time_pause_1;  // 252
    float f_time_boucle_2;  // 256
    float f_time_pause_2;  // 260
    float f_time_total;  // 264
    int i_SND;  // 268
} tScriptStruct_GrillonCigales;

typedef struct sScriptStruct_SNDMDFPeriode {
    float f_heure_debjour;  // 0
    float f_heure_finjour;  // 4
    int i_inveser_nuitjour;  // 8
    float f_far;  // 12
    int i_playing;  // 16
} tScriptStruct_SNDMDFPeriode;

typedef struct sScriptStruct_SND_MusicFight {
    int i_type_de_perso;  // 0
} tScriptStruct_SND_MusicFight;

typedef struct sScriptStruct_SNDMDFBVFrontiere {
    float f_taille_hysteresis;  // 0
    int i_track;  // 4
    int o_mdf_son1_V;  // 8
    int o_mdf_son2_V;  // 12
    int o_mdf_son3_V;  // 16
    int i_famille_V;  // 20
    int o_mdf_son1;  // 24
    int o_mdf_son2;  // 28
    int o_mdf_son3;  // 32
    int i_famille;  // 36
    int i_play;  // 40
} tScriptStruct_SNDMDFBVFrontiere;

typedef struct sScriptStruct_SFXDomeHyllis {
    float f_transparency;  // 0
    int i_after;  // 4
    float old_factor;  // 8
    int old_color;  // 12
    int i_wait;  // 16
    float f_wait_app;  // 20
    int des_i_sf_jesuis_plus_la;  // 24
} tScriptStruct_SFXDomeHyllis;

typedef struct sScriptStruct_PNJDomzSansue {
    float des_duree_attaque;  // 0
    int __des_aircraft_Dimension1;  // 4
    int des_aircraft[20];  // 8
    int __des_iles_Dimension1;  // 88
    int des_iles[5];  // 92
    int des_o_plouf;  // 112
    int des_o_projectile;  // 116
    int des_o_laser;  // 120
    int des_spirale;  // 124
    int n_net;  // 128
    int des_perle;  // 132
    int n_jump_net;  // 136
    int des_jump_perle;  // 140
    int n_beluga_net;  // 144
    int des_beluga_perle;  // 148
    int des_ring;  // 152
    int o_beluga_wp;  // 156
    int o_projectiles;  // 160
    int i_network3_wp_nbr;  // 164
    int __o_network3_Dimension1;  // 168
    int o_network3[50];  // 172
    int i_network3d_wp_nbr;  // 372
    int __o_network3d_Dimension1;  // 376
    int o_network3d[50];  // 380
    float f_time_shootx;  // 580
    float f_time_shoot_delay;  // 584
    float f_scale;  // 588
    int des_o_module_model;  // 592
    float des_f_dist;  // 596
    int des_i_numbones;  // 600
    float des_f_zoom_coef;  // 604
    int des_i_zoom_1st_bone;  // 608
    int i_flag_largue_module;  // 612
    int i_initial_bone_nb;  // 616
    int i_sfx_bone_index;  // 620
    int __af_lenos_Dimension1;  // 624
    float af_lenos[20];  // 628
    int __af_zoom_Dimension1;  // 708
    float af_zoom[20];  // 712
    float f_time_morph;  // 792
    float f_time_separation;  // 796
    int __v_bone_last_pos_Dimension1;  // 800
    Vec3 v_bone_last_pos[20];  // 804
    int __ao_obj_Dimension1;  // 1044
    int ao_obj[20];  // 1048
    float f_time_start_mode;  // 1128
    float f_time_start_attaque;  // 1132
    float f_time_death;  // 1136
    float f_energie_courante;  // 1140
    float f_gen_side;  // 1144
    float f_time_gen;  // 1148
    float f_time_paf;  // 1152
    float f_dest_Z;  // 1156
    float f_time_spark;  // 1160
    float f_banking_coef;  // 1164
    float f_angle_sin;  // 1168
    float f_angle_cos;  // 1172
    float f_dest_offset;  // 1176
    float f_time_ray_cast;  // 1180
    float f_time_lock_paf;  // 1184
    int i_current_mode;  // 1188
    int GFX_Halo;  // 1192
    int GFX_Halo_2;  // 1196
    int i_flag_Z_ok;  // 1200
    int i_wp_nb;  // 1204
    int i_pos_index;  // 1208
    int i_eject_pos_index;  // 1212
    int i_flag_jump_mode;  // 1216
    int i_flag_beluga_mode;  // 1220
    int i_flag_kaboom;  // 1224
    int c_color;  // 1228
    int o_flash_object;  // 1232
    int __ao_flash_object_Dimension1;  // 1236
    int ao_flash_object[10];  // 1240
    int o_main_actor;  // 1280
    int o_aircraft;  // 1284
    int o_next_wp;  // 1288
    int __ao_wp_Dimension1;  // 1292
    int ao_wp[20];  // 1296
    int __ao_projectiles_Dimension1;  // 1376
    int ao_projectiles[10];  // 1380
    int o_main_aircraft;  // 1420
    Vec3 v_last_wp_pos;  // 1424
    Vec3 v_real_pos;  // 1436
    Vec3 v_real_sight;  // 1448
    Vec3 v_dest_pos;  // 1460
    Vec3 v_eject_start_pos;  // 1472
    Vec3 v_eject_dest_pos;  // 1484
    tMessage m_msg;  // 1496
    float f_speed;  // 1548
    float f_rot_blend_speed;  // 1552
    int GFX_Airtrace_G;  // 1556
    int GFX_Airtrace_D;  // 1560
    float f_rotation_coef;  // 1564
    float f_profondeur_coef_G;  // 1568
    float f_profondeur_coef_D;  // 1572
    Vec3 v_grav_pos;  // 1576
    Vec3 v_grav_speed;  // 1588
    float f_time_shoot;  // 1600
    float f_delay_shoot;  // 1604
    int __ao_laser_Dimension1;  // 1608
    int ao_laser[5];  // 1612
    int GFX_Flash;  // 1632
    int i_flag_play_sound;  // 1636
    int i_snd_loop;  // 1640
    int i_snd_loop2;  // 1644
    float f_snd_loop2_volref;  // 1648
    int i_flag_spash_done;  // 1652
    float f_snd_paf_time;  // 1656
    int __GFX_AirSmoke_Dimension1;  // 1660
    int GFX_AirSmoke[10];  // 1664
    int __av_pos_Dimension1;  // 1704
    Vec3 av_pos[10];  // 1708
    Vec3 v_last_pos;  // 1828
    Vec3 v_last_last_pos;  // 1840
    int des_o_acteur_texte;  // 1852
    int des_sally_perle;  // 1856
    int des_peyj_perle;  // 1860
    int des_attack_start_index;  // 1864
    int des_attack_start_nb;  // 1868
    int des_paf_peyj_index;  // 1872
    int des_paf_peyj_nb;  // 1876
    int des_paf_sally_index;  // 1880
    int des_paf_sally_nb;  // 1884
    int des_fin_combat_index;  // 1888
    int des_fin_combat_nb;  // 1892
    int des_explosion_index;  // 1896
    int des_eloigne_index;  // 1900
    int des_eloigne_nb;  // 1904
    int des_reward_index;  // 1908
    int des_fuite_index;  // 1912
    int des_fuite_nb;  // 1916
    int des_appuie_sur_R2;  // 1920
    int i_paf_peyj_text_index;  // 1924
    int i_paf_sally_text_index;  // 1928
    int i_eloigne_text_index;  // 1932
    int i_fuite_text_index;  // 1936
    float f_time_affiche_texte;  // 1940
    float f_time_paf_texte;  // 1944
    float f_time_fuite_texte;  // 1948
    float f_time_eloigne_texte;  // 1952
    int des_o_meteor;  // 1956
    int des_o_paf;  // 1960
    int des_snd_bank;  // 1964
    int __o_lst_meteor_Dimension1;  // 1968
    int o_lst_meteor[20];  // 1972
    int i_nummeteor;  // 2052
    int __i_snd_loop_meteor_Dimension1;  // 2056
    int i_snd_loop_meteor[20];  // 2060
    int i_salve_nb;  // 2140
    float f_time_gener;  // 2144
    float f_rand_gener;  // 2148
    float f_freq_loop;  // 2152
    int __v_speed_Dimension1;  // 2156
    Vec3 v_speed[20];  // 2160
    float f_perle_dist;  // 2400
    int i_halo_boom;  // 2404
    int i_halo_boom2;  // 2408
    int i_mode_flotte;  // 2412
    int i_flag_provocation;  // 2416
    float f_time_culled;  // 2420
    int GFX_Sphere;  // 2424
    float f_link_coef;  // 2428
    float f_link_length;  // 2432
    int o_last_wp;  // 2436
} tScriptStruct_PNJDomzSansue;

typedef struct sScriptStruct_Laser {
    int des_visuel;  // 0
    int des_paf_chaleur;  // 4
    int des_i_halo_au_bout;  // 8
    int des_f_halo_size;  // 12
    int des_i_halo_submat;  // 16
    float des_f_laser_length;  // 20
    float des_f_laser_width;  // 24
    float des_f_laser_speed;  // 28
    float des_f_laser_life_time;  // 32
    int des_i_rebound_nb;  // 36
    int des_shield_rebound;  // 40
    int des_water_rebound;  // 44
    int des_i_submat_beam;  // 48
    int des_dont_destroy;  // 52
    int des_target;  // 56
    int des_residu;  // 60
    int des_residu_mode;  // 64
    int des_test_flag_X;  // 68
    int des_test_water;  // 72
    float des_f_sizemin;  // 76
    float des_f_sizemax;  // 80
    int des_i_submat_ripple;  // 84
    int des_i_max_sprites;  // 88
    float des_f_life;  // 92
    int des_c_color_ripple;  // 96
    int des_puissance;  // 100
    int des_f_point_de_vie;  // 104
    int des_o_sight;  // 108
    int des_c_color;  // 112
    int des_mode;  // 116
    int i_type_vrai;  // 120
    int i_type_faux;  // 124
    int i_flag_touche;  // 128
    int i_rebound_nb;  // 132
    int i_flag_1st_trame;  // 136
    int i_take_speed_vector;  // 140
    int i_flag_factice;  // 144
    float f_time_last_shoot;  // 148
    int i_gfx_lightning;  // 152
    int i_gfx_ripple;  // 156
    int i_gfx_idhalo;  // 160
    Vec3 v_last_beam_pos;  // 164
    Vec3 v_beam_sight;  // 176
    int spark;  // 188
    float f_time_spark;  // 192
    int o_acteur_a_informer;  // 196
    int objet_touche;  // 200
    tMessage m_msg;  // 204
    int i_snd_req;  // 256
    int i_flag_play_loop;  // 260
    float f_blend_sight;  // 264
    float f_alea_sight;  // 268
    float f_freq_min;  // 272
    float f_freq_max;  // 276
    float snd_volmin;  // 280
    float snd_volmax;  // 284
    int __ao_traverse_Dimension1;  // 288
    int ao_traverse[10];  // 292
    int i_flag_special_init;  // 332
    int i_start_bone;  // 336
    int i_start_sight;  // 340
    int o_start_gao_pos;  // 344
    Vec3 v_start_offset;  // 348
    int o_gao_axis_paf;  // 360
    float f_SND_play_dist;  // 364
    float des_f_realhalo_size;  // 368
} tScriptStruct_Laser;

typedef struct sScriptStruct_CVAirCraft {
    int des_o_bison_fute;  // 0
    int des_o_wp_of_net;  // 4
    int n_fuite;  // 8
    int des_rand_bv_pos;  // 12
    int des_type;  // 16
    int des_type_speed;  // 20
    int des_mode;  // 24
    float des_f_speed;  // 28
    Vec3 des_v_friction;  // 32
    int des_water_trace;  // 44
    int des_ignore_player_aircraft;  // 48
    int des_SF_desactivation;  // 52
    float des_f_rot_speed;  // 56
    int des_i_temps_echec;  // 60
    float des_f_break_dist_factor;  // 64
    int des_bateau;  // 68
    int des_i_quartier_pietons;  // 72
    Vec3 v_lock_offset;  // 76
    float des_f_Y_min_offset;  // 88
    float des_f_Y_max_offset;  // 92
    float des_f_Z_water_offset;  // 96
    int des_o_acteur_texte;  // 100
    int des_peyj_col_index;  // 104
    int des_peyj_col_nb;  // 108
    int des_sally_col_index;  // 112
    int des_sally_col_nb;  // 116
    int des_tir_index;  // 120
    int des_tir_nb;  // 124
    int des_peyj_tir_index;  // 128
    int des_peyj_tir_nb;  // 132
    int des_SOS_index;  // 136
    int des_SOS_nb;  // 140
    float f_time_affiche_texte;  // 144
    int i_current_mode;  // 148
    int GFX_Airtrace;  // 152
    int i_flag_trace_on;  // 156
    int i_flag_fuite;  // 160
    int i_flag_water_picking_side;  // 164
    float f_water_Z;  // 168
    float f_virage_coef;  // 172
    float f_vitesse;  // 176
    float f_speed_target;  // 180
    float f_time;  // 184
    float f_rot_speed;  // 188
    float f_time_gfx_trace;  // 192
    float f_blocking_time;  // 196
    float f_brake_dist;  // 200
    float f_time_ground_col;  // 204
    float f_aircraft_dist;  // 208
    float f_camera_dist;  // 212
    float f_largeur_test;  // 216
    float f_banking_coef;  // 220
    int o_current_wp;  // 224
    int o_next_wp;  // 228
    int o_monstros;  // 232
    int o_aircraft;  // 236
    Vec3 v_water_force;  // 240
    Vec3 v_engine_force;  // 252
    Vec3 v_new_sight;  // 264
    Vec3 v_last_speed;  // 276
    Vec3 v_ground_normal;  // 288
    Vec3 v_water_normal;  // 300
    Vec3 v_dest_pos;  // 312
    Vec3 v_water_last_diff;  // 324
    int n_net;  // 336
    tMessage m_mes;  // 340
    int GFX_Halo_G;  // 392
    int GFX_Halo_D;  // 396
    int c_phares;  // 400
    int GFX_Smoke;  // 404
    Vec3 des_v_pos_fumee;  // 408
    int GFX_Flammes;  // 420
    int GFX_Fire_Smoke;  // 424
    float f_time_black_smoke;  // 428
    int i_num_mat;  // 432
    int i_flag_low_res;  // 436
    int o_fuite_wp;  // 440
    int i_snd_loop_motor;  // 444
    int i_snd_loop_water;  // 448
    float f_snd_time_bip;  // 452
    float f_snd_delai_bip;  // 456
    int i_snd_klaxon;  // 460
    float f_snd_freq_bip;  // 464
    int i_snd_mode;  // 468
    int i_snd_tut;  // 472
    float f_snd_tut;  // 476
    float f_snd_freq_tut;  // 480
    tMessage m_msg;  // 484
    float f_time_no_fire;  // 536
    int i_flag_on_coule;  // 540
    int o_sfx_explosion;  // 544
    int i_limo_categ;  // 548
    int i_water_z_categ0;  // 552
    int i_water_z_categ1;  // 556
    int des_police;  // 560
    int des_vaisseau_police;  // 564
    int i_nb_laser_pris;  // 568
    float f_time_collision;  // 572
    float f_collision_rot_speed;  // 576
    int i_last_check;  // 580
    int i_last_txt_is_comp;  // 584
    int des_i_sf_jesuisla;  // 588
    int des_i_v_hyllis;  // 592
} tScriptStruct_CVAirCraft;

typedef struct sScriptStruct_SFXPlouf {
    int des_spark;  // 0
    float f_prog;  // 4
    int old_factor;  // 8
    int old_color;  // 12
    int i_after;  // 16
    float f_transparency_coef;  // 20
    float f_transparency_coef1;  // 24
    int col;  // 28
    int i_rand;  // 32
    float f_speed;  // 36
    int i_Gfx;  // 40
    float f_scale_spark;  // 44
    int c_col;  // 48
    int ii;  // 52
} tScriptStruct_SFXPlouf;

typedef struct sScriptStruct_PNJPodDeReparation {
    int des_o_aircraft;  // 0
    float des_f_energie;  // 4
    int des_o_caisse;  // 8
    int i_current_mode;  // 12
    int GFX_spark;  // 16
    int i_nb_explosion;  // 20
    int i_pod_index;  // 24
    int i_mode_phase;  // 28
    int i_pod_type;  // 32
    int i_flag_ES;  // 36
    float f_time_change_mode;  // 40
    float f_time_spark;  // 44
    float f_time_explosion;  // 48
    float f_delay;  // 52
    float f_energie;  // 56
    float f_time_phase;  // 60
    float f_snd_freq;  // 64
    float f_snd_vol;  // 68
    Vec3 v_materia_pos;  // 72
    int o_plug;  // 84
    int o_aircraft_visual;  // 88
    int o_materia;  // 92
} tScriptStruct_PNJPodDeReparation;

typedef struct sScriptStruct_BisonFute {
    int i_nb_aircraft;  // 0
    float f_water_Z;  // 4
    int __ao_aircraft_Dimension1;  // 8
    int ao_aircraft[30];  // 12
    int o_monstros;  // 132
    int i_last_txt_comp;  // 136
    int i_last_txt_sally;  // 140
    int i_last_paf_txt_comp;  // 144
    int i_last_paf_txt_sally;  // 148
    int i_last_sos_txt;  // 152
} tScriptStruct_BisonFute;

typedef struct sScriptStruct_PNJPompier {
    int des_type;  // 0
    int des_mode;  // 4
    int __des_aircraft_Dimension1;  // 8
    int des_aircraft[20];  // 12
    int n_net;  // 92
    int des_msk_gao;  // 96
    int i_current_mode;  // 100
    int i_wp_nb;  // 104
    int i_flag_snap;  // 108
    int i_flag_compute_best_way;  // 112
    int i_flag_reset_rot_coef;  // 116
    int i_flag_smoke;  // 120
    int i_flag_wp_a_arroser;  // 124
    float f_time_phase;  // 128
    float f_rot_blend_speed;  // 132
    float f_speed;  // 136
    float f_dist_to_dest;  // 140
    float f_banking_coef;  // 144
    float f_time_fine;  // 148
    float f_cam_blend_speed;  // 152
    float f_time_domz_attack;  // 156
    Vec3 v_grav_pos;  // 160
    Vec3 v_grav_speed;  // 172
    Vec3 v_last_pos;  // 184
    Vec3 v_start_pos;  // 196
    Vec3 v_dest_pos;  // 208
    int o_aircraft;  // 220
    int o_next_wp;  // 224
    int __ao_wp_Dimension1;  // 228
    int ao_wp[20];  // 232
    int o_start_pos_wp;  // 312
    int GFX_Halo_1;  // 316
    int GFX_Halo_2;  // 320
    int GFX_Halo_3;  // 324
    int GFX_Halo_4;  // 328
    int GFX_Halo_5;  // 332
    int GFX_Halo_6;  // 336
    int GFX_Halo_7;  // 340
    float f_angle_halo;  // 344
    int GFX_Flash;  // 348
    int i_snd_motor_loop;  // 352
    int i_snd_sirene_loop;  // 356
    int i_snd_psss_loop;  // 360
    int i_flag_stream_played;  // 364
    int GFX_Smoke;  // 368
} tScriptStruct_PNJPompier;

typedef struct sScriptStruct_PorteAntiDomz {
    int des_SF_supplementaire;  // 0
    int des_wp_porte_ferme;  // 4
    float des_delai_av_fermeture;  // 8
    float des_delai_ouverture;  // 12
    float des_delai_fermeture;  // 16
    float f_time_changement_etat;  // 20
    int i_snd_instance;  // 24
} tScriptStruct_PorteAntiDomz;

typedef struct sScriptStruct_HyllisMeteor {
    int des_o_meteor;  // 0
    int des_o_paf;  // 4
    int des_o_plouf;  // 8
    int des_o_env;  // 12
    int __o_lst_meteor_Dimension1;  // 16
    int o_lst_meteor[20];  // 20
    int i_nummeteor;  // 100
    float f_time_gener;  // 104
    int i_max_meteor;  // 108
    float f_rand_gener;  // 112
    int i_jai_le_saut;  // 116
    int __at_domz_Dimension1;  // 120
    tTextGet at_domz[6];  // 124
    int i_text_deja_dit;  // 172
    int __i_snd_loop_Dimension1;  // 176
    int i_snd_loop[20];  // 180
    int __v_speed_Dimension1;  // 260
    Vec3 v_speed[20];  // 264
    float f_freq_loop;  // 504
} tScriptStruct_HyllisMeteor;

typedef struct sScriptStruct_SFXEclaire {
    int time;  // 0
    int i_meurt;  // 4
    float f_transparency_coef;  // 8
    float old_factor;  // 12
    int old_color;  // 16
    float angle;  // 20
} tScriptStruct_SFXEclaire;

typedef struct sScriptStruct_MonstroplanteDeath {
    float f_Time;  // 0
    int __af_MorphStartTime_Dimension1;  // 4
    float af_MorphStartTime[7];  // 8
    int __av_ExplodePos_Dimension1;  // 36
    Vec3 av_ExplodePos[20];  // 40
    int __af_ExplodeTime_Dimension1;  // 280
    float af_ExplodeTime[20];  // 284
    int o_ExplodeCircleDup;  // 364
    int __ao_ExplodeCircle_Dimension1;  // 368
    int ao_ExplodeCircle[20];  // 372
    int __af_ExplodeCircleTime_Dimension1;  // 452
    float af_ExplodeCircleTime[20];  // 456
    int i_CutCam;  // 536
    int o_Particles;  // 540
    float f_ParticleZMin;  // 544
    float f_ParticleZMaxLow;  // 548
    float f_ParticleZMaxHigh;  // 552
    float f_ParticleZMaxStart;  // 556
    float f_ParticleZMaxLength;  // 560
    float f_ParticleStop;  // 564
} tScriptStruct_MonstroplanteDeath;

typedef struct sScriptStruct_SFXSallyShield {
    float f_coef_opacite;  // 0
    float f_coef_noiseB;  // 4
    float f_coef_noiseL;  // 8
    float old_factor;  // 12
    int old_color;  // 16
    int i_after;  // 20
    float f_time_noise;  // 24
    float f_delay_noise;  // 28
} tScriptStruct_SFXSallyShield;

typedef struct sScriptStruct_Detect_o_Notes {
    int __i_numero_note_a_gerer_Dimension1;  // 0
    int i_numero_note_a_gerer[50];  // 4
    int __i_SF_activation_de_la_note_Dimension1;  // 204
    int __i_SF_activation_de_la_note_Dimension2;  // 208
    int i_SF_activation_de_la_note[200];  // 212
    int __i_SF_desactivation_de_la_note_Dimension1;  // 1012
    int __i_SF_desactivation_de_la_note_Dimension2;  // 1016
    int i_SF_desactivation_de_la_note[200];  // 1020
} tScriptStruct_Detect_o_Notes;

typedef struct sScriptStruct_JaugeEnnemi {
    int i_life;  // 0
    float f_time;  // 4
} tScriptStruct_JaugeEnnemi;

typedef struct sScriptStruct_Detect_o_SFAvancement {
    int des_i_force_indice;  // 0
    int __des_SF_tab_Dimension1;  // 4
    int des_SF_tab[100];  // 8
    int __des_f_time_Dimension1;  // 408
    int des_f_time[100];  // 412
    int des_SF_Help;  // 812
    float f_time;  // 816
    int i_indice;  // 820
    int i_flag_set_not_done;  // 824
} tScriptStruct_Detect_o_SFAvancement;

typedef struct sScriptStruct_SallyTeteChercheuses {
    int o_target;  // 0
    int o_FM;  // 4
    int haloG;  // 8
    float haloG_size;  // 12
    Vec3 v_normal;  // 16
    int o_perso;  // 28
    int GFX_spark;  // 32
    float f_time_colset;  // 36
    Vec3 v_rot_vector;  // 40
    float f_rot_speed;  // 52
    float f_rot_speedREF;  // 56
    int i_rot_sens;  // 60
    int i_rot_sensREF;  // 64
    float f_rayon_length;  // 68
    float f_rayon_length_max;  // 72
    float f_angle_cercle;  // 76
    Vec3 v_rot_vectorRef;  // 80
    int i_stop_orientation;  // 92
    int i_init;  // 96
} tScriptStruct_SallyTeteChercheuses;

typedef struct sScriptStruct_Detect_o_16_SF {
    int __SF_Dimension1;  // 0
    int SF[16];  // 4
    int SF_result_a_1;  // 68
    int SF_result_a_0;  // 72
    float f_every;  // 76
    float f_time;  // 80
} tScriptStruct_Detect_o_16_SF;

typedef struct sScriptStruct_Detect_o_SuperSpecialFlags {
    int Mode_ET;  // 0
    int __SF_Et_1_Dimension1;  // 4
    int SF_Et_1[5];  // 8
    int __Et_1_etatSF_Dimension1;  // 28
    int Et_1_etatSF[5];  // 32
    int __SF_Et_2_Dimension1;  // 52
    int SF_Et_2[5];  // 56
    int __Et_2_etatSF_Dimension1;  // 76
    int Et_2_etatSF[5];  // 80
    int __SF_Et_3_Dimension1;  // 100
    int SF_Et_3[5];  // 104
    int __Et_3_etatSF_Dimension1;  // 124
    int Et_3_etatSF[5];  // 128
    int SF_result_a_1;  // 148
    int SF_result_a_0;  // 152
    float f_every;  // 156
    float f_time;  // 160
} tScriptStruct_Detect_o_SuperSpecialFlags;

typedef struct sScriptStruct_SFXFragments {
    int i_life_time;  // 0
    int des_paf_sensitive;  // 4
    int des_i_oriente_a_linit;  // 8
    float f_delai_init;  // 12
    Vec3 v_friction;  // 16
    Vec3 v_gravity;  // 28
    int direction_mode;  // 40
    Vec3 v_direction;  // 44
    float f_angle;  // 56
    float f_speed_min;  // 60
    float f_speed_max;  // 64
    int i_nb_max_rebonds;  // 68
    float f_rebound;  // 72
    float f_init_rot_speed_min;  // 76
    float f_init_rot_speed_max;  // 80
    int i_rotate_axis;  // 84
    int des_detect_water;  // 88
    int des_water_smoke;  // 92
    float des_f_smoke_size;  // 96
    float des_f_smoke_speed;  // 100
    float des_f_smoke_detail;  // 104
    int i_nb_rebonds;  // 108
    int i_flag_play_collision;  // 112
    float f_time_start;  // 116
    float f_rotation_speed;  // 120
    float f_init_rotation_speed;  // 124
    float f_time_play_collision;  // 128
    int o_MDF;  // 132
    float f_zoom_init;  // 136
    float f_speed_zoom;  // 140
    Vec3 v_axe;  // 144
} tScriptStruct_SFXFragments;

typedef struct sScriptStruct_Projectile {
    int i_type_projectile;  // 0
    Vec3 v_param_speed;  // 4
    float f_time;  // 16
    int o_sender;  // 20
} tScriptStruct_Projectile;

typedef struct sScriptStruct_Frisbee {
    int o_jeter_surki;  // 0
    float f_time;  // 4
    float f_time_life;  // 8
    Vec3 v_axe;  // 12
    Vec3 v_sens;  // 24
    int i_sound;  // 36
    Vec3 v_direction;  // 40
    int halo;  // 52
    int GFX_Lightning_1;  // 56
    int GFX_Lightning_2;  // 60
    float i_sin;  // 64
    Vec3 v_impact;  // 68
    float f_coef;  // 80
    Vec3 v_start_pos;  // 84
    int i_start_etage;  // 96
    int o_PAF;  // 100
    int GFX_Halo;  // 104
    float f_size;  // 108
} tScriptStruct_Frisbee;

typedef struct sScriptStruct_ObjectScalator {
    int des_o_objet_scale;  // 0
    Vec3 v_cur_scale;  // 4
    Vec3 v_next_scale;  // 16
    Vec3 v_old_scale;  // 28
    Vec3 v_delta_scale;  // 40
    float f_delai;  // 52
    float f_squash;  // 56
    float f_cur_coeff;  // 60
    tMessage m_msg;  // 64
} tScriptStruct_ObjectScalator;

typedef struct sScriptStruct_GaoShaker {
    float des_X_min;  // 0
    float des_X_max;  // 4
    float des_Y_min;  // 8
    float des_Y_max;  // 12
    float des_Z_min;  // 16
    float des_Z_max;  // 20
    float des_move_delai_min;  // 24
    float des_move_delai_max;  // 28
    float des_stop_delai_min;  // 32
    float des_stop_delai_max;  // 36
    int des_coord;  // 40
    int des_amorti;  // 44
    int des_je_suis_une_light;  // 48
    float des_light_near_min;  // 52
    float des_light_near_max;  // 56
    float des_light_far_min;  // 60
    float des_light_far_max;  // 64
    int i_mode_phase;  // 68
    int i_nb_phases;  // 72
    int i_flag_activated;  // 76
    float f_time_phase;  // 80
    float f_duree_phase;  // 84
    Vec3 v_init_pos;  // 88
    Vec3 v_start_pos;  // 100
    Vec3 v_dest_pos;  // 112
} tScriptStruct_GaoShaker;

typedef struct sScriptStruct_AscenseurNazh {
    float des_f_time_transfert;  // 0
    int des_WP_1_entree;  // 4
    int o_wp_1;  // 8
    int o_wp_1_colmap;  // 12
    int des_wp_1_mode_sortie;  // 16
    int o_wp_1_sortie_sally;  // 20
    int des_wp_1_sortie_auto;  // 24
    int __o_wp_1_attente_Dimension1;  // 28
    int o_wp_1_attente[5];  // 32
    int __o_wp_1_sortie_Dimension1;  // 52
    int o_wp_1_sortie[5];  // 56
    int des_WP_2_entree;  // 76
    int o_wp_2;  // 80
    int o_wp_2_colmap;  // 84
    int des_wp_2_mode_sortie;  // 88
    int o_wp_2_sortie_sally;  // 92
    int des_wp_2_sortie_auto;  // 96
    int __o_wp_2_attente_Dimension1;  // 100
    int o_wp_2_attente[5];  // 104
    int __o_wp_2_sortie_Dimension1;  // 124
    int o_wp_2_sortie[5];  // 128
    int des_GFX;  // 148
    float des_dist_sound_off;  // 152
    int i_current_mode;  // 156
    int i_companion_id;  // 160
    int i_mode_phase;  // 164
    int i_flag_force_sortie;  // 168
    int i_mode_test_sortie;  // 172
    int __ai_companion_status_Dimension1;  // 176
    int ai_companion_status[5];  // 180
    float f_time_phase;  // 200
    Vec3 v_transfert_init_pos;  // 204
    int o_companion;  // 216
    int o_start_wp;  // 220
    int o_dest_wp;  // 224
    int o_colmap_wp;  // 228
    int o_wp_sortie;  // 232
    tMessage m_msg;  // 236
    int GFX_Particules;  // 288
    float f_time_GFX;  // 292
    int o_down_wp;  // 296
    int o_up_wp;  // 300
    float f_GFX_wind_Z_speed;  // 304
    float f_GFX_life_time;  // 308
    int i_flag_particules_down;  // 312
    int i_flag_particules_up;  // 316
    int i_snd_loop;  // 320
    int i_snd_loop2;  // 324
} tScriptStruct_AscenseurNazh;

typedef struct sScriptStruct_PNJSectionAlphaTransport {
    int i_current_mode;  // 0
    int i_flag_pied_au_sol;  // 4
    int i_mode_phase;  // 8
    float f_ground_Z;  // 12
    float f_zoom;  // 16
    float f_fire_coef;  // 20
    float f_speed;  // 24
    Vec3 v_tuyere_pos;  // 28
    Vec3 v_speed;  // 40
    Vec3 v_last_pos;  // 52
    Vec3 v_ground_pos;  // 64
    int o_fuselage;  // 76
    int GFX_Smoke;  // 80
    int GFX_Fire;  // 84
    int GFX_Glow;  // 88
    Vec3 v_pivot_pos;  // 92
    Vec3 v_grav_pos;  // 104
    Vec3 v_grav_speed;  // 116
    float f_Y_val;  // 128
    float f_Z_val;  // 132
    float f_levier_length;  // 136
    int des_i_sf_jesuisla;  // 140
    int des_i_sf_jesuisplusla;  // 144
} tScriptStruct_PNJSectionAlphaTransport;

typedef struct sScriptStruct_AFXManager {
    float f_remanence_cur;  // 0
    int i_remanence_on;  // 4
    float f_remanence_deb;  // 8
    float f_remanence_fin;  // 12
    int i_remanence_delay;  // 16
    float f_Motion_blur_cur;  // 20
    int i_Motion_blur_on;  // 24
    float f_Motion_blur_deb;  // 28
    float f_Motion_blur_fin;  // 32
    int i_Motion_blur_delay;  // 36
    int i_Depth_Blur_on;  // 40
    float f_Depth_Blur_cur_near;  // 44
    float f_Depth_Blur_cur_far;  // 48
    int i_Depth_Blur_type;  // 52
    float f_Screen_Smooth_cur;  // 56
    int i_Screen_Smooth_on;  // 60
    float f_Screen_Smooth_deb;  // 64
    float f_Screen_Smooth_fin;  // 68
    int i_Screen_Smooth_delay;  // 72
} tScriptStruct_AFXManager;

typedef struct sScriptStruct_SFXRedFlash {
    int des_o_object;  // 0
    int des_i_capa;  // 4
    float f_time_start_fade;  // 8
    int i_mdf_play;  // 12
    int o_alarme_distante;  // 16
    float f_time_alarme;  // 20
} tScriptStruct_SFXRedFlash;

typedef struct sScriptStruct_PJEnterprise {
    int des_o_laser;  // 0
    int des_o_missile;  // 4
    tTextGet Nom_De_L_Entree_Radar;  // 8
    int des_reacteurs_vitesse;  // 16
    int des_reacteurs_lateraux;  // 20
    int des_reacteurs_spaciaux;  // 24
    int des_nageoires_laterales;  // 28
    int des_heat_fan;  // 32
    int des_c_halo;  // 36
    int des_c_etincelles;  // 40
    int des_c_laser;  // 44
    int i_current_mode;  // 48
    int i_last_mode;  // 52
    int i_target_max_nb;  // 56
    int i_missile_nb;  // 60
    int i_reflex_prio_contextuel;  // 64
    int i_old_enterprise_status;  // 68
    int i_phase_dock;  // 72
    int i_phase_companion;  // 76
    int __ai_free_space_index_Dimension1;  // 80
    int ai_free_space_index[10];  // 84
    int i_laser_color;  // 124
    int i_joy_mode;  // 128
    int i_joy_strafe_mode;  // 132
    int i_stab_mode;  // 136
    int i_mode_phase;  // 140
    int i_companion_id;  // 144
    int i_colset_state;  // 148
    int i_target_index;  // 152
    int i_flag_vrille_on;  // 156
    int i_flag_strafe_on;  // 160
    int i_flag_force_strafe;  // 164
    int i_flag_can_break_strafe;  // 168
    int i_flag_dock_in;  // 172
    int i_flag_dock_allowed;  // 176
    int i_flag_disable_cam;  // 180
    int i_flag_inverse_boost;  // 184
    int i_flag_in_water;  // 188
    int i_flag_trace_on;  // 192
    int i_flag_in_radar;  // 196
    int i_flag_dyn_X_cam;  // 200
    int i_flag_dyn_Y_cam;  // 204
    int i_flag_cam_X_raz;  // 208
    int i_flag_cam_Y_raz;  // 212
    int i_flag_debarquement_done;  // 216
    int i_flag_hyperdrive_allowed;  // 220
    int i_flag_valid_hyperdrive;  // 224
    int i_flag_landing_effect_done;  // 228
    int i_flag_analog_throttle;  // 232
    int i_flag_in_space;  // 236
    int i_flag_cancel_take_off;  // 240
    int GFX_Halo_G;  // 244
    int GFX_Halo_D;  // 248
    int GFX_Airtrace;  // 252
    int GFX_Halo_collision;  // 256
    int GFX_smoke;  // 260
    int GFX_Halo_Trappe;  // 264
    float f_external_speed_min;  // 268
    float f_external_speed_max;  // 272
    float f_coef_lacet;  // 276
    float f_coef_tangage;  // 280
    float f_coef_roulis;  // 284
    float f_angle_roulis;  // 288
    float f_coef_tangage_visuel;  // 292
    float f_coef_roulis_visuel;  // 296
    float f_coef_lateral_strafe;  // 300
    float f_coef_vertical_strafe;  // 304
    float f_engine_traction;  // 308
    float f_vertical_strafe_traction;  // 312
    float f_lateral_strafe_traction;  // 316
    float f_lacet_speed;  // 320
    float f_tangage_speed;  // 324
    float f_roulis_blend_speed;  // 328
    float f_Z_min;  // 332
    float f_Z_max;  // 336
    float f_Z_Offset;  // 340
    float f_friction;  // 344
    float f_Z_ground;  // 348
    float f_paf_roulis_sens;  // 352
    float f_X_blend_coef;  // 356
    float f_Y_blend_coef;  // 360
    float f_engine_angle;  // 364
    float f_water_Z;  // 368
    float f_last_water_Z;  // 372
    float f_last_valid_water_Z;  // 376
    float f_angle_stab;  // 380
    float f_volet_coef;  // 384
    float f_reticule_dist;  // 388
    float f_sight_blend_speed;  // 392
    float f_angle_trappe;  // 396
    float f_gfx_smoke_delay;  // 400
    float f_roulis_new_val;  // 404
    float f_tangage_new_val;  // 408
    float f_sign_X_rot;  // 412
    float f_sign_Y_rot;  // 416
    float f_sign_Z_rot;  // 420
    float f_sign_cam_X_rot;  // 424
    float f_sign_cam_Y_rot;  // 428
    float f_cam_X_composante;  // 432
    float f_cam_banking_blend_speed;  // 436
    float f_speed_selector_speed_max;  // 440
    float f_speed_selector_dist_min;  // 444
    float f_speed_selector_dist_max;  // 448
    float f_speed_max;  // 452
    float f_energie_perdue;  // 456
    float f_time_lock;  // 460
    float f_time_shoot;  // 464
    float f_time_released;  // 468
    float f_time_dock;  // 472
    float f_time_take_off;  // 476
    float f_time_ground_col;  // 480
    float f_time_water_col;  // 484
    float f_time_paf;  // 488
    float f_time_violent_paf;  // 492
    float f_time_stop_engine;  // 496
    float f_time_wall_col;  // 500
    float f_time_vrille;  // 504
    float f_time_gfx_trace;  // 508
    float f_time_valide_hyper_drive;  // 512
    float f_time_sfx_gaz;  // 516
    float f_time_roulis_set;  // 520
    float f_delay_roulis_set;  // 524
    float f_time_spark_col;  // 528
    float f_time_back_col;  // 532
    float f_time_cam_offset;  // 536
    float f_time_pod_activation;  // 540
    float f_time_start_mode;  // 544
    int __af_time_contact_visuel_Dimension1;  // 548
    float af_time_contact_visuel[10];  // 552
    float f_time_send_vala;  // 592
    float f_time_retournement;  // 596
    Vec3 v_joy;  // 600
    Vec3 v_new_sight;  // 612
    Vec3 v_new_banking;  // 624
    Vec3 v_vert_banking;  // 636
    Vec3 v_hor_sight;  // 648
    Vec3 v_hor_horizon;  // 660
    Vec3 v_shoot_pos;  // 672
    Vec3 v_dock_init_pos;  // 684
    Vec3 v_dock_init_sight;  // 696
    Vec3 v_dock_init_banking;  // 708
    Vec3 v_engine_traction;  // 720
    Vec3 v_strafe_traction;  // 732
    Vec3 v_total_traction;  // 744
    Vec3 v_normale_sol;  // 756
    Vec3 v_normale_wall;  // 768
    Vec3 v_aircraft_offset;  // 780
    Vec3 v_water_force;  // 792
    Vec3 v_collide_point;  // 804
    Vec3 v_last_speed;  // 816
    Vec3 v_last_pos;  // 828
    Vec3 v_macro;  // 840
    Vec3 v_collide_offset;  // 852
    Vec3 v_beam;  // 864
    int o_me;  // 876
    int o_aircraft;  // 880
    int o_visual;  // 884
    int __ao_missiles_Dimension1;  // 888
    int ao_missiles[10];  // 892
    int o_macro_object;  // 932
    int o_wp_atterrissage;  // 936
    int o_wp_decollage;  // 940
    int o_sfx_explode;  // 944
    int o_sight_object;  // 948
    int o_wp_base;  // 952
    int __o_vala_wp_Dimension1;  // 956
    int o_vala_wp[2];  // 960
    int o_companion;  // 968
    int o_speed_selector;  // 972
    int o_locking_target;  // 976
    tMessage m_mes;  // 980
    Vec3 v_pivot_pos;  // 1032
    Vec3 v_grav_pos;  // 1044
    Vec3 v_grav_speed;  // 1056
    Vec3 v_contact_grav_pos;  // 1068
    float f_pivot_grav_dist;  // 1080
    float f_OH;  // 1084
    float f_HB;  // 1088
    float f_amortisseurs_range;  // 1092
    float f_amortisseur_Z;  // 1096
    int i_flag_init_landing_effect;  // 1100
    int i_flag_amorto_on_ground;  // 1104
    int i_flag_set_amorto_pos;  // 1108
    Vec3 v_amorto_G_pos;  // 1112
    Vec3 v_amorto_D_pos;  // 1124
    int i_cheat_status;  // 1136
    int o_paf_flash_effect;  // 1140
    int i_snd_motor_loop;  // 1144
    float f_snd_motor_freq;  // 1148
    float f_snd_motor_freqref;  // 1152
    float f_snd_motor_vol;  // 1156
    float f_snd_motor_volref;  // 1160
    int i_snd_reactor_loop;  // 1164
    float f_snd_reactor_freq;  // 1168
    float f_snd_reactor_freqref;  // 1172
    float f_snd_reactor_vol;  // 1176
    float f_snd_reactor_volref;  // 1180
    int i_snd_frottement;  // 1184
    float f_snd_frottement_freqref;  // 1188
    float f_time_snd_boost;  // 1192
    float f_time_snd_start_boost;  // 1196
    int GFX_Particules;  // 1200
    float f_time_GFX_Particules;  // 1204
    int GFX_Halo_Burn_1;  // 1208
    int GFX_Halo_Burn_2;  // 1212
    int GFX_Phare_G;  // 1216
    int GFX_Phare_D;  // 1220
    int GFX_Flash;  // 1224
    int o_aircraft_flash_effect;  // 1228
    int GFX_Part_Mot_G;  // 1232
    int GFX_Part_Mot_D;  // 1236
    int c_GFX_Reacteurs;  // 1240
    float f_time_GFX_Mot_G;  // 1244
    float f_time_GFX_Mot_D;  // 1248
    float f_gen_rate_mot_G;  // 1252
    float f_gen_rate_mot_D;  // 1256
    float f_time_engine_hs;  // 1260
    float f_delay_engine_hs;  // 1264
    int i_SF_cant_landing;  // 1268
    int i_flag_abort_landing;  // 1272
    float f_time_L_pressed;  // 1276
    float f_time_bip;  // 1280
    float f_time_display_text;  // 1284
    float f_delay_display_text;  // 1288
    int i_next_txt_index;  // 1292
    int GFX_Flash_1;  // 1296
    int GFX_Flash_2;  // 1300
    float f_time_GFX_Flash;  // 1304
    int i_snd_embarquement;  // 1308
    int i_snd_motor_stoped;  // 1312
    float f_snap_X_speed;  // 1316
    int i_flag_reoriente;  // 1320
    float f_reoriente_blend_speed;  // 1324
    Vec3 v_reoriente_dir;  // 1328
    int test;  // 1340
} tScriptStruct_PJEnterprise;

typedef struct sScriptStruct_NRJPure {
    int SF_Materia_prise;  // 0
    int des_type;  // 4
    float des_f_quantite_energie;  // 8
    float f_temps;  // 12
    int o_sally_sac;  // 16
    float f_time_au_sol;  // 20
    float f_time_disp;  // 24
    int GFX_Halo;  // 28
    float f_size;  // 32
    float f_dist_recupere;  // 36
    int i_mode;  // 40
    float f_time_init_tourne;  // 44
    float f_rayon_tourne;  // 48
    float f_rayon_tourne_init;  // 52
    int i_face_de_mort;  // 56
    float f_rot_speed;  // 60
    Vec3 v_rot_vector;  // 64
    int i_rot_sens;  // 76
    int i_tourne;  // 80
    float f_blend_tourne;  // 84
    int i_nb_materia;  // 88
    int c_color;  // 92
    int GFX;  // 96
    float f_duree_add;  // 100
    float f_extern_duree_add;  // 104
    Vec3 v_dest_pos;  // 108
    Vec3 v_pos_sol;  // 120
    float f_rotosol_speed;  // 132
    float f_oscosol_speed;  // 136
    float f_oscosol_angle;  // 140
    float f_time_activation;  // 144
    float f_delay_before_activation;  // 148
    int c_base_constant_1;  // 152
    int o_afterrec_gao;  // 156
    float f_rayon_reduction_speed;  // 160
    int i_sound_init;  // 164
} tScriptStruct_NRJPure;

typedef struct sScriptStruct_EntShootSimple {
    int __o_list_Dimension1;  // 0
    int o_list[15];  // 4
    float f_time;  // 64
    int o_perso_cible;  // 68
    float f_speed;  // 72
    int o_locked_perso;  // 76
} tScriptStruct_EntShootSimple;

typedef struct sScriptStruct_EntShootFollower {
    float f_mvt_proj_speed;  // 0
    Vec3 v_mvt_target_pos;  // 4
    int des_o_lockedtarget;  // 16
    Vec3 des_v_speed;  // 20
    float f_time;  // 32
} tScriptStruct_EntShootFollower;

typedef struct sScriptStruct_EntMissile {
    int des_o_target;  // 0
    float des_f_life_time;  // 4
    int des_puissance;  // 8
    int des_i_point_de_vie;  // 12
    float des_f_traction;  // 16
    float des_f_init_speed;  // 20
    float des_f_rot_coef_blend_speed;  // 24
    float des_f_rotation_speed;  // 28
    float des_f_rotation_speed_max;  // 32
    float des_f_friction;  // 36
    float des_f_friction_max;  // 40
    float des_f_friction_blend_speed;  // 44
    Vec3 des_v_gravity;  // 48
    int des_explosion;  // 60
    int des_i_num_explosion;  // 64
    float des_f_explosion_size;  // 68
    float des_f_explosion_size_rand;  // 72
    float des_f_explosion_speed;  // 76
    float des_f_explosion_detail;  // 80
    int GFX_Halo;  // 84
    int c_color;  // 88
    float f_time_start;  // 92
    float f_time_start_fade;  // 96
    float f_largeur_X_men;  // 100
    Vec3 v_target_pos;  // 104
    int o_collide_object;  // 116
    tMessage m_msg;  // 120
    float f_time_disp;  // 172
    float f_size_halo;  // 176
} tScriptStruct_EntMissile;

typedef struct sScriptStruct_CaisseManMissile {
    int des_o_target;  // 0
    float des_f_life_time;  // 4
    int des_puissance;  // 8
    int des_i_point_de_vie;  // 12
    int des_drop;  // 16
    float des_f_drop_nrj;  // 20
    float des_f_traction;  // 24
    float des_f_init_speed;  // 28
    float des_f_rotation_speed;  // 32
    Vec3 des_v_gravity;  // 36
    Vec3 des_v_friction;  // 48
    int des_i_num_explosion;  // 60
    float des_f_explosion_size;  // 64
    float des_f_explosion_size_rand;  // 68
    float des_f_explosion_detail;  // 72
    int GFX_Halo;  // 76
    int GFX_Lightning_1;  // 80
    int GFX_Lightning_2;  // 84
    int GFX_Lightning_3;  // 88
    int i_plantage_missile;  // 92
    int i_flag_preset;  // 96
    int i_mode_suivi;  // 100
    int c_color;  // 104
    float f_time_start;  // 108
    float f_amplitude_min;  // 112
    float f_amplitude_max;  // 116
    Vec3 v_dest_pos;  // 120
    tMessage m_msg;  // 132
    int o_mon_papa;  // 184
    int i_sound;  // 188
    int i_flag_play_generation_snd;  // 192
    float f_time_disp;  // 196
    float f_size;  // 200
    float f_size_halo;  // 204
    int o_target;  // 208
    int i_mode_test_PAF;  // 212
    int i_beluga;  // 216
    int o_paf;  // 220
} tScriptStruct_CaisseManMissile;

typedef struct sScriptStruct_PNJGardeBarriereEntrepot {
    int des_SF_inactif;  // 0
    int des_o_wp;  // 4
    int des_mode;  // 8
    float des_f_dist_min;  // 12
    float des_f_dist_max;  // 16
    float des_f_time;  // 20
    int des_wp_cone_1;  // 24
    int des_wp_cone_2;  // 28
    int __des_wp_repoussing_Dimension1;  // 32
    int des_wp_repoussing[10];  // 36
    int des_c_color_1;  // 76
    int des_c_color_2;  // 80
    int des_c_color_3;  // 84
    float des_f_base_gravity;  // 88
    Vec3 des_v_water_friction;  // 92
    Vec3 des_v_ground_friction;  // 104
    float des_f_archimede;  // 116
    float des_f_dist_centre_grav;  // 120
    float des_f_water_gravity;  // 124
    float des_f_water_friction;  // 128
    float des_f_air_gravity;  // 132
    float des_f_air_friction;  // 136
    int des_o_acteur_texte;  // 140
    int des_i_num_1st_phrase;  // 144
    int des_i_nb_phrases;  // 148
    float f_time_affiche_texte;  // 152
    int i_flag_in_water;  // 156
    int i_nb_wp;  // 160
    int i_last_mode;  // 164
    int i_flag_real_water;  // 168
    float f_water_Z;  // 172
    float f_last_water_Z;  // 176
    float f_last_valid_water_Z;  // 180
    float f_rotation_speed;  // 184
    float f_last_speed;  // 188
    float f_time_ground_col;  // 192
    float f_time_air;  // 196
    float f_time_paf;  // 200
    float f_zdm_rayon;  // 204
    float f_air_friction;  // 208
    float f_time_gfx_trace;  // 212
    float f_time_main_actor;  // 216
    float f_choppe_blend_speed;  // 220
    Vec3 v_water_force;  // 224
    Vec3 v_grav_pos;  // 236
    Vec3 v_grav_speed;  // 248
    Vec3 v_normale_sol;  // 260
    Vec3 v_last_pos;  // 272
    Vec3 v_last_collide_pos;  // 284
    Vec3 v_dest_pos;  // 296
    Vec3 v_last_grav_speed;  // 308
    int i_current_mode;  // 320
    int i_flag_vert_align_ok;  // 324
    int i_flag_trace_on;  // 328
    int i_flag_capture_done;  // 332
    float f_time_change_mode;  // 336
    float f_time_keep_camera;  // 340
    float f_time_text;  // 344
    float f_dist;  // 348
    float f_dist_range;  // 352
    float f_time_flotte;  // 356
    Vec3 v_my_pos;  // 360
    Vec3 v_aircraft_pos;  // 372
    Vec3 v_aircraft_sight;  // 384
    int o_main_actor;  // 396
    int o_aircraft;  // 400
    int o_aircraft_visuel;  // 404
    tMessage m_msg;  // 408
    int GFX_Smoke;  // 460
    int GFX_Airtrace;  // 464
    int __GFX_Lightning_Dimension1;  // 468
    int GFX_Lightning[3];  // 472
    int __GFX_Halo_Dimension1;  // 484
    int GFX_Halo[3];  // 488
    int i_flag_lightning;  // 500
    float f_time_gfx_lighning;  // 504
    int c_lighning_color;  // 508
    float f_time_GFX_anneaux;  // 512
    int i_snd_ID;  // 516
    int i_snd_play;  // 520
    float f_snd_vol;  // 524
    int i_flag_stay_in_cone;  // 528
    float f_cone_angle;  // 532
    float f_cone_cos;  // 536
    Vec3 v_cone_axe;  // 540
    float f_energie_courante;  // 552
    int i_flag_helper;  // 556
    float f_time_helper;  // 560
    int i_flag_play_plouf;  // 564
} tScriptStruct_PNJGardeBarriereEntrepot;

typedef struct sScriptStruct_SFXGoutteO {
    int Sale_petit_roumain_de_CB;  // 0
    float des_f_freq_min;  // 4
    float des_f_freq_max;  // 8
    int des_c_ripple;  // 12
    int des_i_color_spark;  // 16
    int des_c_spark;  // 20
    float des_f_gravity_min;  // 24
    float des_f_gravity_max;  // 28
    float des_f_zsol;  // 32
    float des_f_largeur_g;  // 36
    float des_f_longueur_g;  // 40
    int des_c_goutte;  // 44
    int des_i_ripple;  // 48
    float des_f_minlife;  // 52
    float des_f_maxlife;  // 56
    float f_next_freq;  // 60
    int i_cur_gouttes;  // 64
    float f_last_gen;  // 68
    int __av_pos_gouttes_Dimension1;  // 72
    Vec3 av_pos_gouttes[50];  // 76
    int __af_speed_gouttes_Dimension1;  // 676
    float af_speed_gouttes[50];  // 680
    int __af_vit_gouttes_Dimension1;  // 880
    float af_vit_gouttes[50];  // 884
    int __ai_id_gouttes_Dimension1;  // 1084
    int ai_id_gouttes[50];  // 1088
    float f_coef_volume;  // 1288
} tScriptStruct_SFXGoutteO;
 

typedef struct sScriptStruct_PNJRaskass {
    int des_i_mode;  // 0
    int des_o_followed;  // 4
    int __des_o_allzones_Dimension1;  // 8
    int des_o_allzones[20];  // 12
    int des_i_numzones;  // 92
    int des_i_constraint;  // 96
    int des_i_etage;  // 100
    int des_i_animal;  // 104
    int GFX_Airtrace;  // 108
    float f_time_jump;  // 112
    float f_time_inside_etat;  // 116
    float f_time_alea_etat;  // 120
    float f_time_change_etat;  // 124
    float f_time_alea_init;  // 128
    Vec3 v_offset_pos;  // 132
    int i_etat;  // 144
    float f_factor_sinus;  // 148
    float f_inertie_sinus;  // 152
    float f_inertie_rot;  // 156
    int f_vitesse;  // 160
    Vec3 v_pos_to_go;  // 164
    Vec3 v_last_offset_pos;  // 176
    int i_dans_la_place;  // 188
    int o_cur_zone;  // 192
    Vec3 v_pos_trace;  // 196
    int i_can_send_paf;  // 208
    float f_time_attak;  // 212
    Vec3 v_speed_saut;  // 216
    int i_can_jump;  // 228
    int i_can_jump_cote;  // 232
    float f_water_Z;  // 236
    float WaterZDelta;  // 240
    float f_last_water_Z;  // 244
    float f_max_z;  // 248
    float f_gravity;  // 252
    float f_friction;  // 256
    float f_time_hs;  // 260
    float f_lime_va_vite;  // 264
    float des_f_hauteur_peut_saut;  // 268
    float des_f_hauteur;  // 272
    float des_f_dist_aileron;  // 276
    float des_f_factor_gravity;  // 280
    float des_f_min_speed;  // 284
    float des_f_max_speed;  // 288
    int o_test_gen;  // 292
    int i_sound_nage;  // 296
    int i_nage_init;  // 300
} tScriptStruct_PNJRaskass;

typedef struct sScriptStruct_PNJTwinIonEngine {
    int des_mode;  // 0
    int des_type;  // 4
    int des_o_dogfight_manager;  // 8
    int des_o_zone;  // 12
    float des_f_zone_max_dist;  // 16
    int des_i_energie;  // 20
    float des_f_speed_min;  // 24
    float des_f_speed_max;  // 28
    float des_f_speed_retour;  // 32
    float des_f_Z_min;  // 36
    float des_f_Z_max;  // 40
    int des_o_laser;  // 44
    int des_i_laser_max_nb;  // 48
    float des_f_laser_delay_min;  // 52
    float des_f_laser_delay_max;  // 56
    int des_o_missile;  // 60
    int des_i_missile_max_nb;  // 64
    int des_tourbillon;  // 68
    int i_current_mode;  // 72
    int i_flag_esquive;  // 76
    int i_flag_retour;  // 80
    int i_flag_je_suis_un_ennemi;  // 84
    int i_energie;  // 88
    int i_flag_near_death;  // 92
    int i_flag_lock_altitude;  // 96
    int i_laser_color;  // 100
    float f_dist_to_target;  // 104
    float f_engine_traction;  // 108
    float f_enfine_traction_cible;  // 112
    float f_target_speed;  // 116
    float f_coef_lacet;  // 120
    float f_coef_tangage;  // 124
    float f_coef_lacet_cible;  // 128
    float f_coef_tangage_cible;  // 132
    float f_Z_min;  // 136
    float f_Z_max;  // 140
    float f_Z_Offset;  // 144
    float f_coef_vert;  // 148
    float f_coef_vert_sign;  // 152
    float f_coef_roulis;  // 156
    float f_missile_life_time;  // 160
    float f_friction;  // 164
    float f_paf_roulis_sens;  // 168
    float f_zone_sqr_dist;  // 172
    float f_target_size;  // 176
    float f_time_shoot;  // 180
    float f_time_esquive;  // 184
    float f_time_lock;  // 188
    float f_time_target_seen;  // 192
    float f_time_choose_rot;  // 196
    float f_time_ground_col;  // 200
    float f_time_wall_col;  // 204
    float f_time_paf;  // 208
    float f_time_start_mode;  // 212
    float f_time_target_locked;  // 216
    float f_time_missile_shoot;  // 220
    float f_time_sfx_gaz;  // 224
    float f_delay_sfx_gaz;  // 228
    float f_delay_shoot;  // 232
    float f_delay_choose_rot;  // 236
    Vec3 v_target_pos;  // 240
    Vec3 v_me_to_target;  // 252
    Vec3 v_hor_me_to_target;  // 264
    Vec3 v_me_to_retour;  // 276
    Vec3 v_hor_me_to_retour;  // 288
    Vec3 v_new_sight;  // 300
    Vec3 v_new_banking;  // 312
    Vec3 v_vert_banking;  // 324
    Vec3 v_hor_sight;  // 336
    Vec3 v_hor_horizon;  // 348
    Vec3 v_normale_sol;  // 360
    Vec3 v_normale_wall;  // 372
    Vec3 v_retour_pos;  // 384
    int o_target;  // 396
    int o_sfx_explode;  // 400
    int o_missile_lock_on_me;  // 404
    int o_ennemy_lock_on_me;  // 408
    int __ao_laser_Dimension1;  // 412
    int ao_laser[10];  // 416
    int __ao_missile_Dimension1;  // 456
    int ao_missile[5];  // 460
    tMessage m_msg;  // 480
    int GFX_Halo_G;  // 532
    int GFX_Halo_D;  // 536
    int GFX_smoke;  // 540
    int GFX_smoke2;  // 544
} tScriptStruct_PNJTwinIonEngine;

typedef struct sScriptStruct_DogFightManager {
    int des_gameplay_manager;  // 0
    int __des_o_ennemi_Dimension1;  // 4
    int des_o_ennemi[10];  // 8
    int __des_o_ami_Dimension1;  // 48
    int des_o_ami[30];  // 52
    int i_ennemi_nb;  // 172
    int i_ami_nb;  // 176
    int i_next_enemy_index;  // 180
    int i_next_friend_index;  // 184
    int __ao_ami_Dimension1;  // 188
    int ao_ami[31];  // 192
    int __ao_ami_cible_Dimension1;  // 316
    int ao_ami_cible[31];  // 320
    int __ao_ennemi_Dimension1;  // 444
    int ao_ennemi[10];  // 448
    int __ao_ennemi_cible_Dimension1;  // 488
    int ao_ennemi_cible[10];  // 492
    tMessage m_msg;  // 532
} tScriptStruct_DogFightManager;

typedef struct sScriptStruct_PNJBaleine {
    int des_o_network;  // 0
    float des_f_water_Z;  // 4
    int o_curwp;  // 8
    int i_etat;  // 12
    float f_speed;  // 16
    float f_time_etat;  // 20
    float z_pos;  // 24
    float f_time_jet;  // 28
    int i_Gfx;  // 32
    Vec3 v_speed_saut;  // 36
    float f_alea_etat;  // 48
    float f_time_saut;  // 52
    float f_time_plonge;  // 56
    Vec3 v_queue;  // 60
    int i_can_queue;  // 72
    int i_can_plonge;  // 76
    float f_time_ripple;  // 80
    float f_inertia;  // 84
    float f_reqinertia;  // 88
    float f_dist_to_air;  // 92
    int i_count_plouf;  // 96
    int o_explo_com;  // 100
    int i_snd_loop;  // 104
} tScriptStruct_PNJBaleine;

typedef struct sScriptStruct_PNJJumpingSnake {
    float des_f_friction;  // 0
    float des_f_sight_blend_speed;  // 4
    float des_f_banking_blend_speed;  // 8
    int des_o_wp_of_net;  // 12
    int des_o_BV_detection;  // 16
    int i_current_mode;  // 20
    float f_time_start_mode;  // 24
    float f_sight_blend_coef;  // 28
    float f_banking_blend_coef;  // 32
    float f_angle;  // 36
    float f_angle_2;  // 40
    float f_oscillation_speed;  // 44
    int o_current_wp;  // 48
    int o_next_wp;  // 52
    int n_net;  // 56
    int des_i_etage;  // 60
} tScriptStruct_PNJJumpingSnake;

typedef struct sScriptStruct_HyllisMeteorManager {
    int __des_aircraft_Dimension1;  // 0
    int des_aircraft[20];  // 4
    int __des_vaisseau_Dimension1;  // 84
    int des_vaisseau[20];  // 88
    int des_meteor;  // 168
    int des_gameplay_manager;  // 172
    int i_current_mode;  // 176
    int i_flag_explode;  // 180
    int i_nb_meteor_aircraft;  // 184
    int __f_Z_hors_champ_Dimension1;  // 188
    float f_Z_hors_champ[5];  // 192
    int __f_Z_hors_champ_save_Dimension1;  // 212
    float f_Z_hors_champ_save[5];  // 216
    int __f_speed_Dimension1;  // 236
    float f_speed[5];  // 240
    float f_time_start_wait;  // 260
    float f_delai_before_reinit;  // 264
    int o_main_actor;  // 268
    int o_aircraft;  // 272
    int __o_meteor_Dimension1;  // 276
    int o_meteor[5];  // 280
    int __v_offset_Dimension1;  // 300
    Vec3 v_offset[5];  // 304
} tScriptStruct_HyllisMeteorManager;

typedef struct sScriptStruct_PilotWingRing {
    int i_flag_paf;  // 0
    float f_side;  // 4
    int old_color;  // 8
    int old_constantcolor0;  // 12
    int old_constantcolor1;  // 16
    float f_zoom;  // 20
    float old_factor;  // 24
    float f_transparency_coef;  // 28
} tScriptStruct_PilotWingRing;

typedef struct sScriptStruct_PNJVaisseauTransportMilitaire {
    int des_patrouilleur;  // 0
    int des_o_start_wp;  // 4
    float des_f_wait_delay;  // 8
    float des_f_traction;  // 12
    int n_net;  // 16
    int GFX_Halo_bleu;  // 20
    int GFX_Halo_rouge;  // 24
    int GFX_smoke;  // 28
    int i_flag_on_ground;  // 32
    int i_flag_pod_activation;  // 36
    int i_flag_pod_out;  // 40
    int i_flag_position_ok;  // 44
    int i_flag_decollage;  // 48
    int i_flag_trappe_sortie;  // 52
    float f_speed;  // 56
    float f_ground_Z;  // 60
    float f_rotation_coef;  // 64
    float f_time_ground_col;  // 68
    float f_time_pod_activation;  // 72
    float f_time_sfx_gaz;  // 76
    float f_pod_1_range;  // 80
    float f_pod_2_range;  // 84
    float f_pod_3_range;  // 88
    float f_pod_1_Z;  // 92
    float f_pod_2_Z;  // 96
    float f_pod_3_Z;  // 100
    float f_collision_speed_Z;  // 104
    float f_gfx_smoke_delay;  // 108
    float f_arceau_angle;  // 112
    float f_vert_coef;  // 116
    int o_next_wp;  // 120
    int o_sfx_reference_object;  // 124
    int o_corps;  // 128
    int o_arceau_1;  // 132
    int o_arceau_2;  // 136
    int o_arceau_3;  // 140
    int o_tete;  // 144
    int o_gyro;  // 148
    int o_motor_1;  // 152
    int o_motor_2;  // 156
    int o_motor_3;  // 160
    int o_plaque_1;  // 164
    int o_plaque_2;  // 168
    int o_plaque_3;  // 172
    int o_pod_1;  // 176
    int o_pod_2;  // 180
    int o_pod_3;  // 184
    int o_light_red;  // 188
    int o_light_blue;  // 192
    int o_trappe;  // 196
    Vec3 v_grav_pos;  // 200
    Vec3 v_grav_speed;  // 212
    Vec3 v_speed;  // 224
    Vec3 v_last_speed;  // 236
    Vec3 v_normalized_speed;  // 248
    Vec3 v_hor_speed;  // 260
    Vec3 v_normalized_hor_speed;  // 272
    Vec3 v_ground_col_pos;  // 284
    int i_snd_moteur_vol;  // 296
    int i_snd_moteur_vol_freq;  // 300
    int i_snd_moteur_arret;  // 304
    int GFX_Reacteur_1;  // 308
    int GFX_Reacteur_2;  // 312
    int GFX_Reacteur_3;  // 316
    float f_reacteur_coef;  // 320
} tScriptStruct_PNJVaisseauTransportMilitaire;

typedef struct sScriptStruct_BoueeAntiAircraft {
    int des_comportement;  // 0
    int des_permanent;  // 4
    int des_SF_inactif;  // 8
    float des_f_dist_max;  // 12
    int des_o_suivant;  // 16
    int des_o_precedent;  // 20
    float des_f_coef_distance_detect;  // 24
    int des_o_acteur_texte;  // 28
    float f_time_affiche_texte;  // 32
    int i_text_index;  // 36
    float des_f_Z_min;  // 40
    float des_f_Z_max;  // 44
    float des_f_halo_extrude;  // 48
    int des_mat_num;  // 52
    float des_bouee_rayon;  // 56
    float des_line_width;  // 60
    int des_line_color;  // 64
    float des_f_base_gravity;  // 68
    Vec3 des_v_water_friction;  // 72
    Vec3 des_v_ground_friction;  // 84
    float des_f_archimede;  // 96
    float des_f_dist_centre_grav;  // 100
    float des_f_water_gravity;  // 104
    float des_f_water_friction;  // 108
    float des_f_air_gravity;  // 112
    float des_f_air_friction;  // 116
    int i_flag_in_water;  // 120
    int i_nb_wp;  // 124
    int i_actif;  // 128
    float f_water_Z;  // 132
    float f_last_water_Z;  // 136
    float f_last_valid_water_Z;  // 140
    float f_rotation_speed;  // 144
    float f_last_speed;  // 148
    float f_time_ground_col;  // 152
    float f_time_air;  // 156
    float f_time_paf;  // 160
    float f_zdm_rayon;  // 164
    float f_air_friction;  // 168
    float f_time_gfx_trace;  // 172
    float f_sqr_dist_aircraft;  // 176
    float f_sqr_dist_suivant;  // 180
    float f_sqr_dist_precedent;  // 184
    Vec3 v_water_force;  // 188
    Vec3 v_grav_pos;  // 200
    Vec3 v_grav_speed;  // 212
    Vec3 v_normale_sol;  // 224
    Vec3 v_last_pos;  // 236
    Vec3 v_last_collide_pos;  // 248
    Vec3 v_dest_pos;  // 260
    int i_current_mode;  // 272
    int i_flag_vert_align_ok;  // 276
    int i_flag_mode_1st_trame;  // 280
    int i_flag_trace_on;  // 284
    int i_flag_capture_done;  // 288
    int c_suivant;  // 292
    int c_precedent;  // 296
    int c_halo;  // 300
    float f_sqr_dist_max;  // 304
    float f_time_change_mode;  // 308
    float f_air_prec_last_side;  // 312
    float f_air_suiv_last_side;  // 316
    float f_time_GFX_suivant;  // 320
    float f_time_GFX_precedent;  // 324
    Vec3 v_my_pos;  // 328
    Vec3 v_aircraft_pos;  // 340
    int o_main_actor;  // 352
    tMessage m_msg;  // 356
    int GFX_Airtrace;  // 408
    int GFX_Line_1;  // 412
    int GFX_Line_2;  // 416
    int GFX_Halo;  // 420
    float f_line_width;  // 424
    float f_line_Z_offset;  // 428
    int o_sfx_flash;  // 432
    int i_snd_laser_1;  // 436
    int i_snd_laser_pres_1;  // 440
    int i_snd_laser_2;  // 444
    int i_snd_laser_pres_2;  // 448
} tScriptStruct_BoueeAntiAircraft;

typedef struct sScriptStruct_PNJGardeBarriereMine {
    int des_o_mine_base;  // 0
    int des_o_wp_de_base;  // 4
    float des_f_dist_att;  // 8
    float des_f_dist_retour;  // 12
    int des_i_special_flag;  // 16
    float des_f_hour_min;  // 20
    float des_f_hour_max;  // 24
    float des_f_dist_centre_grav;  // 28
    float des_f_air_gravity;  // 32
    float des_f_air_friction;  // 36
    int des_i_sf_mort;  // 40
    int des_i_life;  // 44
    int i_mode_arrdep;  // 48
    int i_num_mine;  // 52
    float f_time_mine;  // 56
    float f_time_mine1;  // 60
    float f_alea_mine;  // 64
    float f_alea_mine1;  // 68
    Vec3 v_grav_pos;  // 72
    Vec3 v_grav_speed;  // 84
    Vec3 v_pos_togo;  // 96
    float f_time_pos;  // 108
    Vec3 v_add_pos;  // 112
    Vec3 v_add_pos_blend;  // 124
    int i_attaque;  // 136
    int o_corps;  // 140
    int o_motor_1;  // 144
    int o_motor_2;  // 148
    int o_motor_3;  // 152
    int o_gyro;  // 156
    int GFX_Reacteur_1;  // 160
    int GFX_Reacteur_2;  // 164
    int GFX_Reacteur_3;  // 168
} tScriptStruct_PNJGardeBarriereMine;

typedef struct sScriptStruct_PNJTorpille {
    int des_seb_mode;  // 0
    int des_mode;  // 4
    int des_mine_mode;  // 8
    int des_propulsion;  // 12
    int des_water_type;  // 16
    int des_mine_infinie;  // 20
    int des_puissance;  // 24
    int des_i_point_de_vie;  // 28
    float des_f_mine_time;  // 32
    float des_f_min_attraction;  // 36
    float des_f_stream_force;  // 40
    int des_i_force_suicide;  // 44
    int des_i_ripple;  // 48
    int des_visuel;  // 52
    float des_f_visuel_rotation_speed;  // 56
    float des_f_gravity;  // 60
    float des_f_archimede;  // 64
    int des_wall_collision;  // 68
    int des_ground_collision;  // 72
    int des_torpille_collision;  // 76
    int des_i_sound_id_nb;  // 80
    int __des_i_sound_id_Dimension1;  // 84
    int des_i_sound_id[10];  // 88
    int des_paf_resitance;  // 128
    int des_o_target;  // 132
    int des_lock_target_mode;  // 136
    float des_f_speed;  // 140
    float des_f_rotation_speed;  // 144
    Vec3 des_v_water_friction;  // 148
    Vec3 des_v_air_friction;  // 160
    int __des_o_target_lock_Dimension1;  // 172
    int des_o_target_lock[50];  // 176
    float des_f_target_lock_dist;  // 376
    float des_angle_lock;  // 380
    int des_SF_ID;  // 384
    int i_flag_in_water;  // 388
    int i_flag_reaction_en_chaine;  // 392
    int i_current_mode;  // 396
    int i_flag_paf_send;  // 400
    int i_flag_engine_on;  // 404
    int i_flag_mode_mer;  // 408
    int i_flag_mode_air;  // 412
    int i_flag_mode_explosion;  // 416
    int i_flag_activation;  // 420
    float f_sight_rotspeed;  // 424
    float f_time_start;  // 428
    float f_time_change_mode;  // 432
    float f_water_Z;  // 436
    float f_angle;  // 440
    float f_rotation_speed;  // 444
    float f_oscillation_speed;  // 448
    float f_main_actor_dist;  // 452
    float f_radar_activation_coef;  // 456
    float f_radar_rot_angle;  // 460
    float f_time_send_paf;  // 464
    float f_target_lock_sqr_dist;  // 468
    Vec3 v_init_speed;  // 472
    Vec3 v_water_force;  // 484
    Vec3 v_last_pos;  // 496
    Vec3 v_last_speed;  // 508
    int o_me;  // 520
    int o_collide_object;  // 524
    int __ao_obj_Dimension1;  // 528
    int ao_obj[20];  // 532
    int o_sfx_explosion;  // 612
    int o_visuel;  // 616
    int o_radar;  // 620
    int o_last_target;  // 624
    tMessage m_msg;  // 628
    float f_time_explosion;  // 680
    int GFX_Airtrace;  // 684
    int i_flag_trace_on;  // 688
    float f_time_gfx_trace;  // 692
    float f_last_valid_water_Z;  // 696
    int i_sound_loop;  // 700
    int i_sound_loop_bis;  // 704
    float f_sound_freq_coef;  // 708
    int i_snd_rotor;  // 712
    float f_snd_rotor_volref;  // 716
    float f_snd_rotor_vol;  // 720
    int i_snd_rotor_freqref;  // 724
    float f_ripple_sign;  // 728
    float f_color_angle;  // 732
    float f_color_sin_angle;  // 736
    int c_color_pointe;  // 740
    int c_color_oeil;  // 744
    int halo;  // 748
    float halo_size;  // 752
    int i_flag_real_water;  // 756
    int GFX_Halo;  // 760
} tScriptStruct_PNJTorpille;

typedef struct sScriptStruct_SFDestroy {
    int i_sf;  // 0
} tScriptStruct_SFDestroy;

typedef struct sScriptStruct_TirBlokHyllis {
    int o_obj_detect;  // 0
    float des_f_dist;  // 4
    float des_f_dist_text2;  // 8
    float des_f_dist_shoot;  // 12
    float des_f_factor_sight;  // 16
    int GFX_Halo;  // 20
    float f_morph;  // 24
    int o_laser;  // 28
    float f_time_tir;  // 32
    int o_obj1;  // 36
    int o_obj2;  // 40
    int o_gao_axis_paf;  // 44
} tScriptStruct_TirBlokHyllis;

typedef struct sScriptStruct_DCA {
    float des_f_Radius;  // 0
    float des_f_Offset;  // 4
    float des_f_Extrude;  // 8
    float des_f_Angle;  // 12
    int des_c_color;  // 16
    float des_i_Number;  // 20
    int i_idhalo;  // 24
    float f_rand1;  // 28
    float f_rand2;  // 32
} tScriptStruct_DCA;

typedef struct sScriptStruct_VaisseauHyllisFlic {
    float des_f_dist_centre_grav;  // 0
    float des_f_air_gravity;  // 4
    float des_f_air_friction;  // 8
    int des_o_missile;  // 12
    int i_numpaf;  // 16
    int i_can_move;  // 20
    float f_time_change_pos;  // 24
    Vec3 v_pos_togo;  // 28
    Vec3 v_grav_pos;  // 40
    Vec3 v_grav_speed;  // 52
    Vec3 v_pos_decal;  // 64
    float f_speed;  // 76
    float f_time_shoot;  // 80
    int i_etat_change;  // 84
    int i_mode_attaque;  // 88
    int i_nb_vu;  // 92
    float f_time_vu;  // 96
    int i_devant;  // 100
    int i_je_me_barre;  // 104
    float f_temps_devant;  // 108
    float f_temps_part;  // 112
    float f_time_last_paf;  // 116
} tScriptStruct_VaisseauHyllisFlic;

typedef struct sScriptStruct_FlicTir {
    float f_time_live;  // 0
    float f_time_max;  // 4
    int GFX_Halo;  // 8
    int o_send_paf;  // 12
    Vec3 v_speed;  // 16
    int i_niveau_mechant;  // 28
    int o_owner;  // 32
} tScriptStruct_FlicTir;

typedef struct sScriptStruct_ESCiel {
    float des_diametre_surface;  // 0
    int des_numero_de_l_entree;  // 4
    int des_perso_enterprise;  // 8
    int des_vers_map_numero;  // 12
    int des_vers_entree_numero;  // 16
    int world_ID;  // 20
    int world_key;  // 24
    int i_get_map_id;  // 28
    int i_must_freeze_joy;  // 32
} tScriptStruct_ESCiel;

typedef struct sScriptStruct_DesTourelleLaser {
    int des_SF_mort;  // 0
    int des_o_laser_model;  // 4
    int des_i_laser_max_nb;  // 8
    float des_f_laser_delay_min;  // 12
    float des_f_laser_delay_max;  // 16
    float des_f_dist_vision;  // 20
    float des_f_angle_vision;  // 24
    int des_i_resistance;  // 28
    int des_o_actor_transfert;  // 32
    float des_f_laser_life_time;  // 36
    float des_f_point_de_vie;  // 40
    int i_current_mode;  // 44
    int i_target_type;  // 48
    int i_flag_shoot;  // 52
    float f_time_change_mode;  // 56
    float f_time_laser_shoot;  // 60
    float f_delay_laser_shoot;  // 64
    Vec3 v_me_to_target;  // 68
    int o_target;  // 80
    int __ao_laser_Dimension1;  // 84
    int ao_laser[10];  // 88
    tMessage m_msg;  // 128
} tScriptStruct_DesTourelleLaser;

typedef struct sScriptStruct_PNJGardeAntiVaisseau {
    float des_f_altitude_stop;  // 0
    float des_f_altitude_test;  // 4
    float des_f_rotation;  // 8
    float des_f_vert_offset;  // 12
    int des_o_lecteur_code;  // 16
    int des_i_test_SF;  // 20
    int des_i_texte_demande_code;  // 24
    int des_i_texte_echec_code;  // 28
    int i_ok;  // 32
    int i_flag_show_lecteur_code;  // 36
    int o_beluga;  // 40
    int o_ciel_actor;  // 44
    int o_wp_de_base;  // 48
    float f_time_ok;  // 52
    float f_time_demande_code;  // 56
} tScriptStruct_PNJGardeAntiVaisseau;

typedef struct sScriptStruct_DetectAlarmGlobal {
    int o_musique_normale;  // 0
    int o_musique_alarme;  // 4
    int i_change;  // 8
} tScriptStruct_DetectAlarmGlobal;

typedef struct sScriptStruct_PNJARTicpoil {
    float f_morph;  // 0
    float f_dist;  // 4
    int o_fuite;  // 8
    Vec3 v_dest_pos;  // 12
    int i_fuite;  // 24
    float f_arrivee;  // 28
    int i_cpt_bouge;  // 32
    float f_rand_bouge;  // 36
    float f_max_debat;  // 40
    float f_speed;  // 44
    int ti_GFX_Boom;  // 48
    int i_MapEtage;  // 52
    int i_snd_marche;  // 56
} tScriptStruct_PNJARTicpoil;

typedef struct sScriptStruct_FunEtoileFilante {
    float time;  // 0
    float size;  // 4
    float alea;  // 8
    Vec3 speed;  // 12
    float des_f_z;  // 24
    int espace;  // 28
} tScriptStruct_FunEtoileFilante;

typedef struct sScriptStruct_VideoPlayer {
    int v_video_file;  // 0
    float f_delai;  // 4
    int o_camera;  // 8
    float f_time_wait;  // 12
    int VideoIsPlaying;  // 16
} tScriptStruct_VideoPlayer;

typedef struct sScriptStruct_ZoneCompanionNetwork {
    int des_n_network_order;  // 0
    int des_i_companion;  // 4
    int __i_annuler_Dimension1;  // 8
    int i_annuler[5];  // 12
} tScriptStruct_ZoneCompanionNetwork;

typedef struct sScriptStruct_Detect_o_SpecialFlags {
    int des_i_specialFlag_vrai1;  // 0
    int des_i_specialFlag_vrai2;  // 4
    int des_i_specialFlag_faux1;  // 8
    int des_i_specialFlag_faux2;  // 12
    int des_i_nbre_conditions;  // 16
    float f_frequence_test;  // 20
    float f_temps;  // 24
} tScriptStruct_Detect_o_SpecialFlags;

typedef struct sScriptStruct_BVActiveDesactive {
    int i_detection_type;  // 0
    int i_bv_pivot;  // 4
    int o_BV_a_qui;  // 8
    int acteur_a_detecter;  // 12
    int acteur_avec_CBit;  // 16
    int ne_fait_pas_l_activation;  // 20
    int active_dans_bv;  // 24
    int ia_reinit1;  // 28
    int object_a_activer1;  // 32
    int ia_reinit2;  // 36
    int object_a_activer2;  // 40
    int ia_reinit3;  // 44
    int object_a_activer3;  // 48
    int rend_invisible_aussi;  // 52
    int ne_fait_qu_activer;  // 56
    int se_desactive_apres;  // 60
    int a_active;  // 64
} tScriptStruct_BVActiveDesactive;

typedef struct sScriptStruct_Detect_o_BVCapaSet {
    int o_perso;  // 0
    int o_perso_capa;  // 4
    int i_capa;  // 8
    int des_i_test_si_actif;  // 12
    int des_i_met_SF;  // 16
    int des_capa_a_faux;  // 20
} tScriptStruct_Detect_o_BVCapaSet;

typedef struct sScriptStruct_ZoneOrdreVala {
    int o_actor_a_detecter;  // 0
    int i_Detect;  // 4
    int i_cancel_vala;  // 8
    int i_Peyj_vala;  // 12
    int i_Mili_vala;  // 16
    int i_Domz_vala;  // 20
    int i_XB25_vala;  // 24
    int i_XB26_vala;  // 28
    int __ai_compagnon_qui_agit_Dimension1;  // 32
    int ai_compagnon_qui_agit[5];  // 36
    int __o_destinationVala_Dimension1;  // 56
    int o_destinationVala[5];  // 60
    float f_delai_avant_envoi_vala;  // 80
    float f_time_attente;  // 84
    int i_mode_de_deplacement;  // 88
    int i_done;  // 92
    int i_sedesactive_apres_ordre;  // 96
    int i_replique;  // 100
    int i_Perso_ID_replique_acteur;  // 104
    int i_replique_limite;  // 108
} tScriptStruct_ZoneOrdreVala;

typedef struct sScriptStruct_SnapOnActor {
    int des_use_initial_offset;  // 0
    int des_acteur;  // 4
    Vec3 v_offset;  // 8
} tScriptStruct_SnapOnActor;

typedef struct sScriptStruct_activateur {
    int des_o_actor_to_activate;  // 0
    int des_o_actor_who_activate;  // 4
    int des_i_CBits_Test;  // 8
    int des_i_active_desactive;  // 12
    int des_i_visibilite;  // 16
    int des_i_change_hors_BV;  // 20
} tScriptStruct_activateur;

typedef struct sScriptStruct_ActivateurAvecSF {
    int des_i_SpFlag;  // 0
    int des_i_SpFlag_etat;  // 4
    int des_o_acteur;  // 8
    int des_i_control_flags;  // 12
    float des_f_frequence_test;  // 16
    float f_temps;  // 20
} tScriptStruct_ActivateurAvecSF;

typedef struct sScriptStruct_SNDMDFSF {
    int SF_a_tester;  // 0
    int SF_destruction;  // 4
    int i_son_qd_SF_a_vrai;  // 8
} tScriptStruct_SNDMDFSF;


typedef struct sScriptStruct_SNDLoopVolFreqCapa {
    int SF_ON_OFF;  // 0
    float f_vol_min;  // 4
    float f_vol_max;  // 8
    float f_freq_min;  // 12
    float f_freq_max;  // 16
    int i_nb_son_simul;  // 20
    int __i_snd_id_Dimension1;  // 24
    int i_snd_id[5];  // 28
    int __f_snd_freq_Dimension1;  // 48
    float f_snd_freq[5];  // 52
    int __f_snd_vol_Dimension1;  // 72
    float f_snd_vol[5];  // 76
    float tf_time_change;  // 96
} tScriptStruct_SNDLoopVolFreqCapa;

typedef struct sScriptStruct_SNDCapaWatchRandomPlay {
    int i_watch_capa;  // 0
    int o_watch_actor;  // 4
    int o_cine_actor;  // 8
    int i_snd_nb_smd;  // 12
    float f_snd_min_vol;  // 16
    float f_snd_max_vol;  // 20
    float f_snd_min_freq;  // 24
    float f_snd_max_freq;  // 28
} tScriptStruct_SNDCapaWatchRandomPlay;

typedef struct sScriptStruct_ActionReaction {
    int o_activateur;  // 0
    float des_i_distance_action;  // 4
    int des_o_transfert_PafUniv;  // 8
    int des_transfer_all_paf;  // 12
    int i_insensible_a_la_SA;  // 16
    int des_i_interdire_en_fight;  // 20
    int des_i_action;  // 24
    int des_i_evt_cycle;  // 28
    int des_i_unbreakable;  // 32
    int i_nb_kick;  // 36
    int des_i_nb_kick_for_break;  // 40
    int des_SF_je_suis_active;  // 44
    int des_SF_dois_je_m_activer;  // 48
    int des_i_priority_jump;  // 52
    int des_i_type_of_botton;  // 56
    tTextGet des_t_action;  // 60
    int des_f_offsetZ_regard_sally;  // 68
    float des_f_haut_regard;  // 72
    int des_mode_info;  // 76
    int des_i_sally_text_index;  // 80
    float des_f_duree_du_text;  // 84
    float des_f_coef_volume;  // 88
    tMessage m_msg;  // 92
} tScriptStruct_ActionReaction;

typedef struct sScriptStruct_GetObjectOnGround {
    int Indice_de_lobjet;  // 0
    float f_distance_propose_action;  // 4
    int SF_objet_deja_recupere;  // 8
    int SF_objet_recuperable;  // 12
    int SV_objet_recuperable_position;  // 16
    float des_f_OffsetZ;  // 20
    int des_i_saute_dans_linventaire;  // 24
    float des_f_life_time;  // 28
    int des_i_interdire_en_fight;  // 32
    int des_i_prio_action;  // 36
    int i_chute_avec_rebond;  // 40
    int des_halo;  // 44
    int des_halo_extrude_axe;  // 48
    int des_halo_extrude_sens;  // 52
    float des_halo_radius;  // 56
    float des_halo_extrude;  // 60
    float des_halo_angle;  // 64
    int des_halo_number;  // 68
    int des_halo_couleur;  // 72
    Vec3 des_v_halo_offset;  // 76
    int i_chute_termine;  // 88
    int i_reflex_prio_contextuel;  // 92
    int i_nbr_dobjets;  // 96
    float f_time_start;  // 100
    int i_snd_pas_nutrille;  // 104
    int i_snd_play;  // 108
    float f_snd_vol;  // 112
    Vec3 v_pos_init;  // 116
    float px;  // 128
    float py;  // 132
    float f_time;  // 136
    float f_alfa;  // 140
    int i_rebond;  // 144
    Vec3 v_axe;  // 148
    int i_plus_de_place;  // 160
    float f_time_place;  // 164
    int GFX_Halo;  // 168
    float f_angle_halo;  // 172
} tScriptStruct_GetObjectOnGround;

typedef struct sScriptStruct_LecteurCarte {
    int SF_Mode_Retrait_Pognes;  // 0
    int objet_lecteur;  // 4
    int des_i_vendeur_ID;  // 8
    int i_afficher_tune;  // 12
} tScriptStruct_LecteurCarte;

typedef struct sScriptStruct_Barrique {
    float des_f_dist_propose_kick;  // 0
    float f_init_speed;  // 4
    float f_rotation_speed;  // 8
    float f_rotation_friction;  // 12
    float f_norm_OG;  // 16
    float f_norm_OH;  // 20
    float f_norm_HI;  // 24
    int i_current_mode;  // 28
    int i_flag_stop_dyn;  // 32
    Vec3 v_init_pos;  // 36
    Vec3 v_grav_pos;  // 48
    Vec3 v_grav_speed;  // 60
    Vec3 v_IO;  // 72
    Vec3 v_paf_dir;  // 84
    float f_time_bing;  // 96
    tMessage m_msg;  // 100
    int des_o_active;  // 152
} tScriptStruct_Barrique;

typedef struct sScriptStruct_GenerateurLanceur {
    int __des_o_objet_lance_Dimension1;  // 0
    int des_o_objet_lance[5];  // 4
    float des_f_frequence_min;  // 24
    float des_f_frequence_max;  // 28
    float des_f_angle_max;  // 32
    float f_delai;  // 36
    float f_temps;  // 40
    int i_indice;  // 44
    Vec3 v_direction;  // 48
} tScriptStruct_GenerateurLanceur;

typedef struct sScriptStruct_ObjetGenere {
    float f_duree_vie;  // 0
    float f_vitesse_dep;  // 4
    float f_snd_blend;  // 8
    int i_snd;  // 12
    float f_snd_vol;  // 16
} tScriptStruct_ObjetGenere;

typedef struct sScriptStruct_SFXDisapear {
    int des_o_target;  // 0
    float des_f_hauteur;  // 4
    int des_i_emetteur_fixe;  // 8
    int des_i_mat_id;  // 12
    int des_i_particle_nbr;  // 16
    float des_f_emetteur_diametre;  // 20
    float des_f_particle_lifetimeMin;  // 24
    float des_f_particle_lifetimeMax;  // 28
    Vec3 v_pos_target;  // 32
    float f_Light_Near;  // 44
    float f_Light_Far;  // 48
    float f_time_phase0;  // 52
    float f_time_phase1;  // 56
    float f_time1;  // 60
    float f_time2;  // 64
    int i_on_mat_flags;  // 68
    int i_off_mat_flags;  // 72
    int i_old_flag_value;  // 76
    int i_phase;  // 80
    float f_LightStart;  // 84
    float f_LightEnd;  // 88
    float f_GenStart;  // 92
    float f_GenEnd;  // 96
} tScriptStruct_SFXDisapear;

typedef struct sScriptStruct_SFXSmoke {
    int des_o_BV;  // 0
    int des_mat_gao;  // 4
    int des_mat_1;  // 8
    int des_mat_2;  // 12
    int des_mat_3;  // 16
    int des_mat_4;  // 20
    int des_particules_buffer;  // 24
    float des_f_time_phase_1;  // 28
    float des_f_time_phase_2;  // 32
    Vec3 des_v_friction;  // 36
    float des_f_gravity;  // 48
    float des_f_wind;  // 52
    int des_c_1st_color;  // 56
    int des_c_2nd_color;  // 60
    int des_c_3rd_color;  // 64
    float des_f_growing_speed_min;  // 68
    float des_f_growing_speed_max;  // 72
    float des_f_growing_speed_friction;  // 76
    float des_f_time_random;  // 80
    float des_f_gen_rate_ON;  // 84
    float des_f_ON_size_min;  // 88
    float des_f_ON_size_max;  // 92
    float des_f_ON_speed_min;  // 96
    float des_f_ON_speed_max;  // 100
    float des_f_gen_rate_OFF;  // 104
    float des_f_OFF_size_min;  // 108
    float des_f_OFF_size_max;  // 112
    float des_f_OFF_speed_min;  // 116
    float des_f_OFF_speed_max;  // 120
    int __table_color_Dimension1;  // 124
    int table_color[5];  // 128
    int GFX;  // 148
    float f_time_start;  // 152
    int o_me;  // 156
    int i_snd_flamme;  // 160
    int des_i_sf_jesuisla;  // 164
} tScriptStruct_SFXSmoke;

typedef struct sScriptStruct_SFXFlash {
    int des_o_snap_actor;  // 0
    Vec3 des_v_offset;  // 4
    float des_f_delai_apparition;  // 16
    float des_f_delai_disparition;  // 20
    float des_f_extrude_dist;  // 24
    int des_c_color;  // 28
    float des_f_param1;  // 32
    float des_f_param2;  // 36
    int des_i_nb_rayons;  // 40
    float des_f_big_alpha;  // 44
    float des_f_big_length_min;  // 48
    float des_f_big_length_max;  // 52
    float des_f_big_width_min;  // 56
    float des_f_big_width_max;  // 60
    float des_f_small_alpha;  // 64
    float des_f_small_length_min;  // 68
    float des_f_small_length_max;  // 72
    float des_f_small_width_min;  // 76
    float des_f_small_width_max;  // 80
    int GFX_Flash;  // 84
    float f_time_start_flash;  // 88
    float f_time_stop_flash;  // 92
} tScriptStruct_SFXFlash;

typedef struct sScriptStruct_ActivatorSimple {
    int des_o_mode_ext;  // 0
    int des_i_foraircraft;  // 4
    int des_i_both;  // 8
    int i_ON;  // 12
} tScriptStruct_ActivatorSimple;

typedef struct sScriptStruct_InfoAnimaux {
    int des_i_sf_jesuisla;  // 0
    int des_o_jesuisla_desactive;  // 4
    int des_o_jesuisla_active;  // 8
    int des_i_animaux;  // 12
    int des_i_MapEtage;  // 16
    float des_f_sphere_radius;  // 20
    float des_f_frame_min;  // 24
} tScriptStruct_InfoAnimaux;

int Global_glob_init();
int Global_glob_go();
int Global_glob_exec_buttons();
int Global_glob_exec_alarme();
int Global_glob_exec_volumes_manager();
int Global_glob_exec_music_machine();
int Global_glob_exec_world_cycle();
int Global_glob_mort();
int Global_glob_exec_game_speed();
int Global_glob_exec_event_sound();
int Global_glob_raz_univers();
int Global_glob_reflex();
int Global_glob_exec_joy();
int Global_glob_init_object();
int Global_glob_exec_flash();
int Global_glob_exec_hauteur_eau();

int board_panel_bp_init_trame1();
int board_panel_bp_init_trame2();
int board_panel_bp_exec_reflex();
int board_panel_bp_mode_look();
int board_panel_bp_inventaire_mode();
int board_panel_bp_exec_choix_perso_pointe();
int board_panel_bp_exec_select_button_enterprise();
int board_panel_bp_INTF_exec_target();
int board_panel_bp_INTF_update();
int board_panel_bp_exec_button_valide_nextfastitem();
int board_panel_bp_exec_menu_swap();
int board_panel_bp_updatedisplay();
int board_panel_bp_INTF_boutonces_update_new();
int board_panel_bp_exec_look_photoborder_off();
int board_panel_bp_exec_look_photo_select_info();
int board_panel_bp_exec_button_valide_look();
int board_panel_bp_exec_affiche_contextuel();
int board_panel_bp_exec_button_valide_action_jump();
int board_panel_bp_mode_aircraft();
int board_panel_bp_INTF_exec_photo();
int board_panel_bp_exec_select_button_mode_test();
int board_panel_bp_exec_sprite_boutons_on();
int board_panel_bp_mode_dialogue();
int board_panel_bp_exec_look_simple();
int board_panel_bp_INTF_exec_radar();
int board_panel_bp_mode_normal();
int board_panel_bp_exec_clear();
int board_panel_bp_exec_look_reset();
int board_panel_bp_exec_button_valide_friend();
int board_panel_bp_mode_enterprise();
int board_panel_bp_exec_button_valide_action_sally();
int board_panel_bp_beforedisplay();
int board_panel_bp_dialogue_check();
int board_panel_bp_exec_joje_set();
int board_panel_bp_inventaire_in();
int board_panel_bp_exec_look_phototraitement();
int board_panel_bp_exec_look_photoborder_on();
int board_panel_bp_exec_button_valide_use();
int board_panel_bp_inventaire_out();
int board_panel_bp_exec_look_jeter();
int board_panel_bp_exec_compte_a_rebour();
int board_panel_bp_exec_look_init();
int board_panel_bp_exec_button_valide_suite_dialogue();
int board_panel_bp_exec_select_button_dialog_mode();
int board_panel_bp_exec_select_button_look_mode_shift();
int board_panel_bp_afterrec();
int board_panel_bp_exec_select_button_normal_mode();
int board_panel_bp_exec_button_valide_next_companion();
int board_panel_bp_boutonces_init();
int board_panel_bp_exec_look_photo();
int board_panel_bp_exec_button_valide_exit_dialogue();
int Global_Library_photo_add();
int Global_Library_photo_take();

int map_manager_mm_exec_TM();
int map_manager_mm_init_trame1();
int map_manager_mm_reflex();
int map_manager_mm_init_trame2();
int map_manager_mm_exec_carte_manager();
int map_manager_mm_exec_choc_sur_mat_speciale();
int map_manager_mm_exec_TM_SpecialLoad();
int map_manager_mm_exec_TM_Editeur();

int ES_au_sol_esas_enter_the_map();
int ES_au_sol_esas_mdisk();
int ES_au_sol_esas_reflex();
int ES_au_sol_esas_decode_world_key();
int ES_au_sol_esas_exit_cinematique();
int ES_au_sol_esas_game();
int ES_au_sol_esas_exit_the_map();
int ES_au_sol_esas_play_events();
int ES_au_sol_esas_checkpoint();
int ES_au_sol_esas_force_change_status();
int ES_au_sol_esas_external();

int camera_cam_dynplaninit();
int camera_cam_afterrec();
int camera_cam_exec_mario();
int camera_cam_exec_visibility();
int camera_cam_exec_cine();
int camera_cam_exec_change_mode();
int camera_cam_dynplan();
int camera_cam_1_mainloop();
int camera_cam_exec_enterprise();
int camera_cam_1_init();

int camera_target_tgt_afterrec();
int camera_target_tgt_follow1actor();

int camera_derriere_cd_init();
int camera_derriere_cd_execcut();
int camera_derriere_cd_loop();
int camera_derriere_cd_movecam();

int gencam_mario_gcm_first();
int gencam_mario_gcm_exec_activation();
int gencam_mario_gcm_normal();

int gencam_es_ges_init();
int gencam_es_ges_loop();

int camera_speciale_cs_init();
int camera_speciale_cs_wait();
int camera_speciale_cs_after_rec();
int camera_speciale_cs_reflex();
int camera_speciale_cs_mode_look();
int camera_speciale_cs_mode_inventaire();

int camera_new_cn_mode_sally();
int camera_new_cn_extern();
int camera_new_cn_mode_aircraft();
int camera_new_cn_process();
int camera_new_cn_init();
int camera_new_cn_select_mode();

int Fight_Manager_fm_init();
int Fight_Manager_fm_mise_a_jour();
int Fight_Manager_fm_exec_music();
int Fight_Manager_fm_exec_get_msg();
int Fight_Manager_fm_reflex();

int cine_text_display_ctd_init();
int cine_text_display_ctd_display();
int cine_text_display_ctd_manager();

int GST_Sound_Manager_gs_play_sound();

int SND_MergeKit_SND_Merge();
int SND_MergeKit_SND_MergeUnmerge();

int Cine_Textes_CiTxt_reception_messages();
int Cine_Textes_CiTxt_init();

int TXT_Encart_TXT_go();
int TXT_Encart_TXT_init();

int Merge_ActionKit_mak_init();

int Cine_Events_cine_firstinit();
int Cine_Events_cine_init();
int Cine_Events_cine_tests_depart();
int Cine_Events_cine_tests_interruption();
int Cine_Events_cine_exec_test_priorite();
int Cine_Events_Cine_exec_InitialiseCine();
int Cine_Events_Cine_exec_GoToEvents();
int Cine_Events_cine_play_eventtrack();
int Cine_Events_cine_test_fin_sons();
int Cine_Events_Cine_exec_EndOfEvents();
int Cine_Events_Cine_exec_soundtracks_Fade();

int activator_door_ad_init();
int activator_door_ad_loop();

int Activator_act_init();
int Activator_act_loop();
int Activator_act_afterrec();
int Activator_act_cutter();

int mirror_mirror_init();
int mirror_mirror_loop();
int mirror_mirror_lightning_update();
int mirror_mirror_lightning_on();
int mirror_mirror_lightning_off();
int mirror_mirror_beforedisplay();
int mirror_mirror_afterdisplay();
int mirror_mirror_exec_set_snd_pos();

int porte_po_init();
int porte_po_reflex();
int porte_po_ferme_toi();
int porte_po_mode_indy_ARec();
int porte_po_paf();
int porte_po_etat_ouverte();
int porte_po_check_separation();
int porte_po_etat_ferme();
int porte_po_mode_indy();
int porte_po_ouvre_toi();

int Light_PAF_lp_init();
int Light_PAF_lp_FarUp_FarDown();
int Light_PAF_lp_FarUp_ColorDown();
int Light_PAF_C_Light_mode_FarDown();

int SFX_Sally_SA_Ano_circle_Jade_Uppercut();
int SFX_Sally_SA_Ano_circle_Jade_SuperAction();
int SFX_Sally_SA_Ano_circle_adisplay();
int SFX_Sally_SA_Ano_circle_garde();
int SFX_Sally_SA_Ano_circle_init();
int SFX_Sally_SA_Ano_circle_bdisplay();
int SFX_Sally_SA_Ano_circle_HH_BullRush();
int SFX_Sally_SA_Ano_circle_Jade_CoupFinal();

int SFX_Comete_SFX_Comete_comp();
int SFX_Comete_SFX_Comete_init();
int SFX_Comete_SFX_AfterD();
int SFX_Comete_SFX_Comete_dead();
int SFX_Comete_SFX_Comete_mort();
int SFX_Comete_SFX_BeforeD();

int Sally_sa_init();
int Sally_sa_exec_get_modules();
int Sally_sa_after_blend();
int Sally_sa_init_select_mode();
int Sally_sa_exec_init_normale();
int Sally_sa_2D();
int Sally_sa_exec_joynorm_blend();
int Sally_sa_after_rec();
int Sally_sa_exec_tag_off();
int Sally_sa_exec_tag_on();
int Sally_sa_exec_actionkitselector();
int Sally_sa_action_animchange();
int Sally_sa_action_capture();
int Sally_sa_action_eat();
int Sally_sa_action_entercode();
int Sally_sa_exec_chocmur();
int Sally_sa_reflex();
int Sally_sa_exec_message_read();
int Sally_sa_exec_message_select_action();
int Sally_sa_exec_stop_on_corner();
int Sally_sa_cinematique();
int Sally_sa_exec_update_shield_energie();
int Sally_sa_action_interupteur();
int Sally_sa_exec_animkitselector();
int Sally_sa_exec_demi_tour();
int Sally_sa_exec_demi_tour_track();
int Sally_sa_exec_magnet();
int Sally_sa_exec_testpush();
int Sally_sa_exec_animselector();
int Sally_sa_exec_same_sight_checker();
int Sally_sa_action_gaz();
int Sally_sa_exec_sight_banking();
int Sally_sa_exec_suivi_des_yeux();
int Sally_sa_exec_test_roulade();
int Sally_sa_action_roulade();
int Sally_sa_exec_expressionkitselector_main();
int Sally_sa_exec_expressionkitselector_blink();
int Sally_sa_exec_expressionkitselector_visage();
int Sally_sa_exec_fight_esquiveselector();
int Sally_sa_exec_fight_on();
int Sally_sa_exec_fight_get_kick_sight();
int Sally_sa_exec_fight_kickselector();
int Sally_sa_SFX_Shadows();
int Sally_sa_reflex_exec_chaque_trame();
int Sally_sa_action_grabwall();
int Sally_sa_action_paf();
int Sally_sa_action_kickassis();
int Sally_sa_action_push();
int Sally_sa_SFX_Baton();
int Sally_sa_SFX_XMEN_Ano();
int Sally_sa_exec_jump_frequency_get();
int Sally_sa_SFX_XMEN();
int Sally_sa_action_tourne();
int Sally_sa_action_lance();
int Sally_sa_SFX_Gaz();
int Sally_sa_action_kicklow();
int Sally_sa_action_morfle();
int Sally_sa_action_speak();
int Sally_sa_action_pass();
int Sally_sa_exec_testrayjump();
int Sally_sa_rayjump_air();
int Sally_sa_rayjump_initsaut();
int Sally_sa_mort();
int Sally_sa_rayjump_initchute();
int Sally_sa_exec_rayjump();
int Sally_sa_exec_lance();
int Sally_sa_action_kickfight();
int Sally_sa_exec_testplaquagemur();
int Sally_sa_action_plaquage_mur();
int Sally_sa_exec_paf_decode_universel();
int Sally_sa_exec_paf_recul();
int Sally_sa_exec_paf_zdef_zdec();
int Sally_sa_action_takeput_virtuel();
int Sally_sa_exec_paf_zdes_zdes();
int Sally_sa_action_takeput();

int get_object_go_init();
int get_object_go_get_i_bit();
int get_object_go_SFX();
int get_object_go_present();
int get_object_go_absent();
int get_object_go_exec_gene_magnet();

int MDisk_MD_init();
int MDisk_MD_reflex();
int MDisk_MD_photos();
int MDisk_MD_Photos_SetNewPage();
int MDisk_MD_exec_sfx();
int MDisk_MD_Camera();
int MDisk_MD_avi();

int Drone_Mdisk_Drone_Mdisk_init();
int Drone_Mdisk_Drone_Mdisk_mode_wait();
int Drone_Mdisk_Drone_Mdisk_exec_check_dialog();
int Drone_Mdisk_Drone_Mdisk_mode_deplacement();
int Drone_Mdisk_Drone_Mdisk_mode_plug();
int Drone_Mdisk_Drone_Mdisk_exec_GFX_Halo();
int Drone_Mdisk_Drone_Mdisk_exec_GFX_Plasma();

int SFX_HaloSimple_init();
int SFX_HaloSimple_move();

int SFX_Secundo_SFX_Secundo_init();
int SFX_Secundo_SFX_Secundo_Reflex();
int SFX_Secundo_SFX_Secundo_Manager();
int SFX_Secundo_SFX_Secundo_Apparition();
int SFX_Secundo_SFX_Secundo_exec_afterdisplay();
int SFX_Secundo_SFX_Secundo_exec_afterblend();
int SFX_Secundo_SFX_Secundo_Disparition();
int SFX_Secundo_SFX_Secundo_exec_beforedisplay();
int SFX_Secundo_SFX_Secundo_exec_afterrec();
int SFX_Secundo_SFX_Secundo_Destroy();

int Switch_s1_init();
int Switch_s1_wait();
int Switch_s1_exec_reset_constante();
int Switch_s1_exec_set_action();
int Switch_s1_exec_set_constante();
int Switch_s1_exec_on_off();
int Switch_s1_exec_magnet_gene();
int Switch_s1_set_action();
int Switch_s1_exec_send_info();
int Switch_s1_exec_get_message();

int zone_detect_actors_zda_init();
int zone_detect_actors_zda_check_ou();
int zone_detect_actors_zda_check_et();

int Onde_de_choc_cine_ODCc_comportement();
int Onde_de_choc_cine_ODCc_init();
int Onde_de_choc_cine_ODCc_adisplay();
int Onde_de_choc_cine_ODCc_bdisplay();

int Spirale_Domz_spd_init();
int Spirale_Domz_spd_centre();
int Spirale_Domz_spd_wait();
int Spirale_Domz_spd_apparition();
int Spirale_Domz_spd_trou();
int Spirale_Domz_spd_disparition();
int Spirale_Domz_spirale_adisplay();
int Spirale_Domz_spirale_bdisplay();

int RLI_Variator_RLI_Variator_init();
int RLI_Variator_RLI_Variator_comportement();
int RLI_Variator_RLI_Variator_exec_after_display();
int RLI_Variator_RLI_Variator_exec_mode_stealth();
int RLI_Variator_RLI_Variator_exec_mode_standard();
int RLI_Variator_RLI_Variator_exec_before_display();

int SFX_Sun_loop();

int world_cycle_world_init();
int world_cycle_world_cycle();
int world_cycle_world_planetes();

int Fun_Mouette_mouette_init();
int Fun_Mouette_mouette_move();
int Fun_Mouette_mouette_follow();
int Fun_Mouette_mouette_gotonext();
int Fun_Mouette_mouette_reflexe();

int Fun_Poleine_pol_loop();

int world_cycle_init_wsi_init();

int SFX_Flammes_SFX_Flammes_comportement();
int SFX_Flammes_SFX_Flammes_init();

int sfx_bouclier_phare_sbp_firstinit();
int sfx_bouclier_phare_sbp_init();
int sfx_bouclier_phare_abp_apparition2();
int sfx_bouclier_phare_sbp_disparition();
int sfx_bouclier_phare_sbp_apparition1();
int sfx_bouclier_phare_sbp_wait_eteind();
int sfx_bouclier_phare_sbp_adisplay();
int sfx_bouclier_phare_sbp_msg();
int sfx_bouclier_phare_sbp_apparition0();
int sfx_bouclier_phare_sbp_bdisplay();
int sfx_bouclier_phare_sbp_exec_pos();
int sfx_bouclier_phare_sbp_apparition();

int snap_oriente_on_bone_soob_init();
int snap_oriente_on_bone_soob_transit();
int snap_oriente_on_bone_soob_scale();
int snap_oriente_on_bone_soob_msg();
int snap_oriente_on_bone_soob_snap();

int multi_snape_on_bone_msnb_init();
int multi_snape_on_bone_msnb_snap_objets();
int multi_snape_on_bone_msnb_snap_anim();

int GST_Script_Combat_GST_Init();
int GST_Script_Combat_GST_main();
int GST_Script_Combat_GST_ConditionPermanente();
int GST_Script_Combat_GST_Macro_Pause();
int GST_Script_Combat_GST_Macro_Label();
int GST_Script_Combat_GST_Script_Order_Sauterelle();
int GST_Script_Combat_GST_Script_Order_Spectre();
int GST_Script_Combat_GST_Script_Order_SpectrePlus();
int GST_Script_Combat_GST_ComportementSpeciaux();
int GST_Script_Combat_GST_Script_Order_Crevette();
int GST_Script_Combat_GST_Script_Order_SpectreBas();
int GST_Script_Combat_GST_CP_RefreshActors();
int GST_Script_Combat_GST_Macro_AForceComportement();
int GST_Script_Combat_GST_Macro_DesactiveCP();
int GST_Script_Combat_GST_Script_Order_CaveCrawler();
int GST_Script_Combat_GST_Macro_ActionAlternee();
int GST_Script_Combat_GST_Macro_ExecMacroAlterne();
int GST_Script_Combat_GST_Macro_AChoixPerso();
int GST_Script_Combat_GST_Macro_TestContinue();
int GST_Script_Combat_GST_CP_Minumum();
int GST_Script_Combat_GST_Script_Order_DireWorm();
int GST_Script_Combat_GST_CP_FightSallyOn();
int GST_Script_Combat_GST_Macro_SendMessage();
int GST_Script_Combat_GST_Macro_ActiveCP();
int GST_Script_Combat_GST_Macro_ActionEnchaine();
int GST_Script_Combat_GST_Script_Order_Tikha();
int GST_Script_Combat_GST_CP_AttendPaf();
int GST_Script_Combat_GST_Script_Order_Sarko();
int GST_Script_Combat_GST_Script_Order_Memorial();
int GST_Script_Combat_GST_Script_Order_SpectreContinu();
int GST_Script_Combat_GST_Macro_Goto();
int GST_Script_Combat_GST_Macro_ExecMacroSimul();
int GST_Script_Combat_GST_Script_Order_SpectreProche();

int PNJ_Spectre_sp_init();
int PNJ_Spectre_sp_mode_combat();
int PNJ_Spectre_sp_mode_apparition_anim();
int PNJ_Spectre_sp_reflex();
int PNJ_Spectre_sp_mode_mort();
int PNJ_Spectre_sp_tag_off();
int PNJ_Spectre_sp_bef_disp();
int PNJ_Spectre_sp_after_disp();
int PNJ_Spectre_sp_after_blend();
int PNJ_Spectre_sp_tag_on();
int PNJ_Spectre_sp_exec_decode_paf_univ();
int PNJ_Spectre_sp_mode_paf();
int PNJ_Spectre_sp_mode_apparition_xmen();

int SFX_colpag_cam_scp_loop();

int Monstroplante_Gamin_MPG_run();
int Monstroplante_Gamin_MPG_init();
int Monstroplante_Gamin_MPG_Jump();

int PNJ_Spectre_bras_PNJ_Spectre_Bras_init();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_do_snake();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_move();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_after_display();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_before_display();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_reflex();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterblend_init();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_afterrec_move();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_GFX_Part();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_exec_init_snake();
int PNJ_Spectre_bras_PNJ_Spectre_Bras_Destroy();

int Detect_o_Time_dot_init();
int Detect_o_Time_dot_off();
int Detect_o_Time_dot_on();
int Detect_o_Time_dot_wait_chgt();

int Detect_o_4SpecialFlags_D4sf_init();
int Detect_o_4SpecialFlags_D4sf_go();
int Detect_o_4SpecialFlags_D4sf_wait();

int move_with_me_move_with_me_init();
int move_with_me_move_with_me_loop();

int planete_rotation_pr_init();
int planete_rotation_pr_loop();

int SFX_Halo_Dynamique_SFXHD_init();
int SFX_Halo_Dynamique_SFXHD_halo_simple();
int SFX_Halo_Dynamique_SFXHD_halo_rays();
int SFX_Halo_Dynamique_SFXHD_exec_looptest();
int SFX_Halo_Dynamique_SFXHD_afterrec();

int Material_modificator_Matmod_init();
int Material_modificator_Matmod_Fade_BD();
int Material_modificator_Matmod_Flash();
int Material_modificator_Matmod_Fade_wait();
int Material_modificator_Matmod_Fade();
int Material_modificator_Matmod_Flash_wait();
int Material_modificator_Matmod_Fade_AD();
int Material_modificator_Matmod_Fade_CB();

int Ondulator_ond_init();
int Ondulator_ond_rumble();
int Ondulator_ond_sinus();

int Monstroplante_mon_init();
int Monstroplante_mon_play();
int Monstroplante_mon_reflex();
int Monstroplante_mon_BDisplay();
int Monstroplante_mon_ADisplay();

int Detect_o_Email_doe_go();

int SFX_Sally_AddAno_SFX_go();
int SFX_Sally_AddAno_SFX_ADisplay();
int SFX_Sally_AddAno_SFX_delete();
int SFX_Sally_AddAno_SFX_BDisplay();

int Monstroplante_socle_mp_tige();
int Monstroplante_socle_mp_init();
int Monstroplante_socle_mp_reflex();
int Monstroplante_socle_mp_exec_materia();
int Monstroplante_socle_mp_donne_perle();
int Monstroplante_socle_mp_sfx_eclat();
int Monstroplante_socle_mp_morph();
int Monstroplante_socle_mp_attend_paradie();
int Monstroplante_socle_mp_meurt();

int Monstroplante_Tentacule_MT_First_Init();
int Monstroplante_Tentacule_MT_Paf();
int Monstroplante_Tentacule_MT_init();
int Monstroplante_Tentacule_MT_reflex();
int Monstroplante_Tentacule_MT_go();
int Monstroplante_Tentacule_MT_do_snake();

int Zone_OccupationTerrain_zo_init();
int Zone_OccupationTerrain_zo_dehors();
int Zone_OccupationTerrain_zo_exec_transfert();
int Zone_OccupationTerrain_zo_dans_la_place();
int Zone_OccupationTerrain_Zo_reflex();
int Zone_OccupationTerrain_zo_exec_clear();

int companion_co_init();
int companion_co_exec_set_friend_type();
int companion_co_init_select_mode();
int companion_co_action_cine();
int companion_co_action_normal();
int companion_co_action_grab_wall();
int companion_co_action_push_init();
int companion_co_tag_on();
int companion_co_exec_choix_acteur_suivi_regard();
int companion_co_slave_action();
int companion_co_exec_2__calcul_next_wp();
int companion_co_exec_get_message();
int companion_co_action_CoupCul();
int companion_co_exec_normal_arret();
int companion_co_exec_rayjump();
int companion_co_rayjump_initsaut();
int companion_co_exec_reflex();
int companion_co_exec_magnet();
int companion_co_exec_fight_action_observe();
int companion_co_exec_after_rec();
int companion_co_exec_after_blend();
int companion_co_tag_off();
int companion_co_action_attente();
int companion_co_go_nextwp_walk();
int companion_co_exec_expressionkitselector_blink();
int companion_co_action_baffe();
int companion_co_exec_fight_AI();
int companion_co_exec_fight_action_baffe();
int companion_co_slave_move_choise();
int companion_co_go_wp_push();
int companion_co_action_bidouille();
int companion_co_exec_expressionkitselector_visage();
int companion_co_exec_dialogue();
int companion_co_exec_test_push();
int companion_co_action_push();
int companion_co_exec_deplacements();
int companion_co_exec_anim_deplace();
int companion_co_action_occupationTerrain();
int companion_co_action_HammerTime();
int companion_co_exec_anim_deplace_slave();
int companion_co_exec_paf_zdes_zdes();
int companion_co_SFX_coup_de_cul();
int companion_co_exec_anim_arret();
int companion_co_exec_fight_action_dash();
int companion_co_exec_get_message_action();
int companion_co_slave_move_between_wps();
int companion_co_slave_finish();
int companion_co_exec_calcul_virtualCoefSens();
int companion_co_slave_wait();
int companion_co_action_mort();
int companion_co_slave_mode_dispatch();
int companion_co_action_BullRush();
int companion_co_exec_suivi_des_yeux();
int companion_co_action_parle();
int companion_co_action_a_terre();
int companion_co_exec_fight_pos_proche();
int companion_co_exec_jump_frequency_get();
int companion_co_action_joy_cheat();
int companion_co_exec_normal_reseau_ou_pas();
int companion_co_exec_normal_choix_anim();
int companion_co_exec_normal_deplacement();
int companion_co_SFX_pied();
int companion_co_exec_detect_col_NMI();
int companion_co_exec_test_rayjump();
int companion_co_rayjump_air();
int companion_co_rayjump_initchute();
int companion_co_action_blesse(); // hurt
int companion_co_exec_paf_decode_universel();
int companion_co_action_fight();
int companion_co_exec_paf_recul();
int companion_co_action_paf();
int companion_co_exec_fight_pos_autour_enemy();
int companion_co_slave_push_take_eat();
int companion_co_exec_fight_pos_surplace();
int companion_co_exec_fight_pos_proche_sally();
int companion_co_exec_fight_action_hide();
int companion_co_exec_fight_pos_hide();
int companion_co_action_explosion();
int companion_co_action_mort_XB();

int Objet_Poussable_obp_init();
int Objet_Poussable_obp_destroy();
int Objet_Poussable_obp_tag_off();
int Objet_Poussable_obp_reflex();
int Objet_Poussable_obp_exec_check_obstacle();
int Objet_Poussable_obp_tag_on();
int Objet_Poussable_obp_go();
int Global_Library_exec_magnets_generation();
int Global_Library_exec_magnets_destruction();

int aircraft_air_init();
int aircraft_air_mode_deplacement_init();
int aircraft_air_mode_dock_out_init();
int aircraft_air_init_select_mode();
int aircraft_air_mode_docked_init();
int aircraft_air_mode_off_init();
int aircraft_air_exec_set_afterrec_mode();
int aircraft_air_mode_docked();
int aircraft_air_exec_camera();
int aircraft_air_reflex();
int aircraft_air_exec_s_b();
int aircraft_air_exec_normales_get();
int aircraft_air_exec_test_jump();
int aircraft_air_exec_waterforce();
int aircraft_air_exec_frictions();
int aircraft_air_exec_GFX_trace();
int aircraft_air_mode_a_quai_init();
int aircraft_air_exec_test_accostage();
int aircraft_air_mode_accostage_init();
int aircraft_air_mode_embarquement_init();
int aircraft_air_exec_pilote();
int aircraft_air_exec_pilote_auto();
int aircraft_air_exec_pilote_course();
int aircraft_air_mode_deplacement();
int aircraft_air_exec_landing_effect();
int aircraft_air_mode_debarquement_init();
int aircraft_air_exec_debarquement_autres();
int aircraft_air_exec_gestion_paf();
int aircraft_air_exec_pilote_poursuite();
int aircraft_air_exec_GFX_Halos();
int aircraft_air_exec_GFX_Morph();
int aircraft_air_exec_GFX_tirs();
int aircraft_air_exec_pilote_poursuite2();
int aircraft_air_mode_mort_init();
int aircraft_air_exec_GFX_Helices();
int aircraft_air_exec_afterrec();
int aircraft_air_mode_dock_in_init();
int aircraft_air_exec_zone_set();
int aircraft_air_exec_joy_mario();
int aircraft_air_exec_joy_local();
int aircraft_air_exec_check_plug();
int aircraft_air_exec_gravity();
int aircraft_air_exec_GFX_Flammes();
int aircraft_air_mode_a_quai();
int aircraft_air_mode_accostage();
int aircraft_air_mode_off();
int aircraft_air_mode_debarquement();
int aircraft_air_mode_dock_in();
int aircraft_air_mode_dock_out();
int aircraft_air_mode_embarquement();
int aircraft_air_mode_mort();
int aircraft_air_exec_GFX_Etincelles();
int aircraft_air_exec_GFX_Modules();
int aircraft_air_exec_GFX_global();
int aircraft_air_exec_GFX_Particules();
int aircraft_air_exec_embarquement_autres();
int aircraft_air_exec_GFX_ripple();
int aircraft_air_exec_GFX_Flash();
int aircraft_air_exec_corde_force();
int aircraft_air_exec_shoot();
int aircraft_air_exec_visual_rotation();
int aircraft_air_exec_update_water_info();
int aircraft_air_exec_anti_traversing();
int aircraft_air_exec_GFX_Boost();
int aircraft_air_exec_GFX_Poussiere();
int aircraft_air_exec_dont_sink();

int Colmap_Accostage_Colmap_Accostage_comportement();

int Detect_o_Zone_doz_init();
int Detect_o_Zone_doz_detec_o_BV();
int Detect_o_Zone_doz_detec_o_ZDE();

int Zone_ChangeStatus_MultiCompagnon_zcsmc_init();
int Zone_ChangeStatus_MultiCompagnon_zcsmc_wait();

int SFX_explosion_sfx_effect1();
int SFX_explosion_sfx_combo4();
int SFX_explosion_sfx_combo15();
int SFX_explosion_sfx_effect9();
int SFX_explosion_sfx_expl_init();
int SFX_explosion_sfx_AfterRec_snap();
int SFX_explosion_sfx_combo2();
int SFX_explosion_sfx_combo13();
int SFX_explosion_sfx_effect10();
int SFX_explosion_sfx_combo22();
int SFX_explosion_sfx_effect8();
int SFX_explosion_sfx_combo1();
int SFX_explosion_sfx_combo12();
int SFX_explosion_sfx_combo21();
int SFX_explosion_sfx_effect6();
int SFX_explosion_sfx_combo9();
int SFX_explosion_sfx_combo10();
int SFX_explosion_sfx_effect3();
int SFX_explosion_sfx_combo6();
int SFX_explosion_sfx_combo17();
int SFX_explosion_sfx_effect5();
int SFX_explosion_sfx_combo8();
int SFX_explosion_sfx_combo19();
int SFX_explosion_sfx_effect4();
int SFX_explosion_sfx_combo7();
int SFX_explosion_sfx_combo18();
int SFX_explosion_sfx_effect2();
int SFX_explosion_sfx_combo5();
int SFX_explosion_sfx_combo16();
int SFX_explosion_sfx_effect7();
int SFX_explosion_sfx_combo0();
int SFX_explosion_sfx_combo11();
int SFX_explosion_sfx_combo20();
int SFX_explosion_sfx_effect0();
int SFX_explosion_sfx_combo3();
int SFX_explosion_sfx_combo14();
int SFX_explosion_sfx_combo_explo_rocs();

int SFX_Bone_Fire_SFX_Bone_Fire_init();
int SFX_Bone_Fire_SFX_Bone_Fire_After_Blend();
int SFX_Bone_Fire_SFX_Bone_Fire_exec_fire();
int SFX_Bone_Fire_SFX_Bone_Fire_After_Rec();
int SFX_Bone_Fire_SFX_Bone_Fire_exec_smooth_fire();
int SFX_Bone_Fire_SFX_Bone_Fire_Destroy();

int Cine_orage_Meteores_met_bombarde();
int Cine_orage_Meteores_met_init();

int Cine_FigeleMonde_flm_fige();
int Cine_FigeleMonde_flm_defige();

int Test_Appui_Bouton_Tab_init();
int Test_Appui_Bouton_Tab_loop_sequence();
int Test_Appui_Bouton_Tab_loop();
int Test_Appui_Bouton_Tab_init_sequence();

int Donne_des_Pognes_ddp_wait();
int Donne_des_Pognes_ddp_prend();
int Donne_des_Pognes_ddp_donne();

int Zone_ChangeStatus_zcs_init();
int Zone_ChangeStatus_zcs_wait();
int Zone_ChangeStatus_zcs_exec_texte();

int Capa_SF_Active_Desactive_csad_detect();

int mpegplayer_init();
int mpegplayer_loop();

int GST_Carte3D_Init();
int GST_Carte3D_Close();
int GST_Carte3D_Open();
int GST_Carte3D_exec_UpdatePiece();
int GST_Carte3D_Loop();
int GST_Carte3D_exec_Spot();
int GST_Carte3D_exec_SpotInsert();

int SFX_Lightning_SFX_Lightning_afterrec();
int SFX_Lightning_SFX_Lightning_init();
int SFX_Lightning_SFX_Lightning_reflex();
int SFX_Lightning_SFX_Lightning_comportement();

int SFX_AfterFX_AFX_Loop();

int SFX_Etincelles_SFX_Etincelles_afterrec();
int SFX_Etincelles_SFX_Etincelles_init();
int SFX_Etincelles_SFX_Etincelles_reflex();
int SFX_Etincelles_SFX_Etincelles_comportement();

int Gam_emploi_du_temps_edt_init();
int Gam_emploi_du_temps_edt_reflex();
int Gam_emploi_du_temps_edt_tag_off();
int Gam_emploi_du_temps_edt_tag_on();
int Gam_emploi_du_temps_edt_exec_track_occupation();
int Gam_emploi_du_temps_edt_occupation_loup_init();
int Gam_emploi_du_temps_edt_occupation_loup_attentejoueur();
int Gam_emploi_du_temps_edt_exec_recherche_chemin();
int Gam_emploi_du_temps_edt_occupation_jump();
int Gam_emploi_du_temps_edt_occupation_loup_poursuite();
int Gam_emploi_du_temps_edt_exec_next_edt();
int Gam_emploi_du_temps_edt_exec_cherche_dest();
int Gam_emploi_du_temps_edt_deplacement();
int Gam_emploi_du_temps_edt_deplacement_direct();
int Gam_emploi_du_temps_edt_occupation_dodo();
int Gam_emploi_du_temps_edt_occupation_regarde();
int Gam_emploi_du_temps_edt_occupation_mange();
int Gam_emploi_du_temps_edt_after_blend();
int Gam_emploi_du_temps_edt_occupation_loup_nargue();
int Gam_emploi_du_temps_edt_exec_get_free_player();
int Gam_emploi_du_temps_edt_dialogue_check();
int Gam_emploi_du_temps_edt_action_attente();
int Gam_emploi_du_temps_edt_occupation_loup_safe();
int Gam_emploi_du_temps_edt_exec_suivi_regard();

int AR_RatBleu_rb_init();
int AR_RatBleu_rb_loop();

int Crochax_3_Cro3_mode_wait();
int Crochax_3_Cro3_exec_pattes();
int Crochax_3_Cro3_mode_meurt();
int Crochax_3_Cro3_exec_do_snake();
int Crochax_3_Cro3_exec_ailes();
int Crochax_3_Cro3_mode_sort();
int Crochax_3_Cro3_mode_plug_on_ground();
int Crochax_3_Cro3_exec_zdm_posset();
int Crochax_3_Cro3_exec_GFX_Crache();
int Crochax_3_Cro3_mode_snap();
int Crochax_3_Cro3_exec_init_snake();
int Crochax_3_Cro3_mode_balade();
int Crochax_3_Cro3_mode_paf();
int Crochax_3_Cro3_mode_eject();
int Crochax_3_Cro3_exec_GFX_Paf();
int Crochax_3_Cro3_exec_after_display();
int Crochax_3_Cro3_reflex();
int Crochax_3_Cro3_init();
int Crochax_3_Cro3_mode_fight();
int Crochax_3_Cro3_mode_plug();
int Crochax_3_Cro3_mode_cine();
int Crochax_3_Cro3_exec_before_display();
int Crochax_3_Cro3_exec_push_enemy();
int Crochax_3_Cro3_exec_free_enemy();
int Crochax_3_Cro3_exec_s_b();

int Waf_Appareil_Photo_gam_init();
int Waf_Appareil_Photo_gam_reflex();
int Waf_Appareil_Photo_gam_comportement_deplacements();
int Waf_Appareil_Photo_gam_tag_on();
int Waf_Appareil_Photo_gam_comportement_attrape();
int Waf_Appareil_Photo_gam_comportement_attente_SF();
int Waf_Appareil_Photo_gam_after_rec();
int Waf_Appareil_Photo_gam_tag_off();
int Waf_Appareil_Photo_gam_exec_recherche_chemin();
int Waf_Appareil_Photo__gam_comportement_coince();

int AR_luciole_luciole_init();
int AR_luciole_luciole_move();
int AR_luciole_luciole_reflexe();
int AR_luciole_luciole_loop();

int Scarabee_cine_sca_init();
int Scarabee_cine_sca_oriente();

int charlie_ch_init();
int charlie_ch_poser();
int charlie_ch_reflexe();
int charlie_ch_move();

int vaisseau_hyllis_vh_init();
int vaisseau_hyllis_vh_go();
int vaisseau_hyllis_vh_go_vrille();
int vaisseau_hyllis_vh_explosion();
int vaisseau_hyllis_vh_go_move();
int vaisseau_hyllis_vh_reflexe();

int helper_text_hlptxt_init();
int helper_text_hlptxt_exec_fin_ou_abort();
int helper_text_hlptxt_exec_tests_interruption();
int helper_text_hlptxt_tests_wait();
int helper_text_hlptxt_tests_depart();
int helper_text_hlptxt_exec_ask_display();

int Hovercraft_shoot_HoverShoot_init();
int Hovercraft_shoot_HoverShoot_move();
int Hovercraft_shoot_HoverShoot_reflex();
int Hovercraft_shoot_HoverShoot_exec_impact();
int Hovercraft_shoot_HoverShoot_exec_GFX_Flash();
int Hovercraft_shoot_HoverShoot_death();

int hyllis_gp_manager_hgm_init();
int hyllis_gp_manager_hgm_loop();
int hyllis_gp_manager_hgm_attend();

int Detect_o_1SpecialFlag_dos_init();

int Move_Grid_Move_Grid_init();

int Objet_Flottant_OF_destroy();
int Objet_Flottant_OF_init();
int Objet_Flottant_OF_Reward();
int Objet_Flottant_OF_exec_FX_Smoke();
int Objet_Flottant_OF_exec_GFX_Fire();
int Objet_Flottant_OF_NoReward();
int Objet_Flottant_OF_exec_GFX_Smoke();
int Objet_Flottant_OF_mode_ground();
int Objet_Flottant_OF_comportement();
int Objet_Flottant_OF_afterrec();
int Objet_Flottant_OF_Message_Read();

int caisse_gene_init();
int caisse_gene_loop();

int Unknown_DEMO_ES_select();

int depaneuse_dp_exec_afterrec();
int depaneuse_dp_toaircraftreel();
int depaneuse_dp_texte();
int depaneuse_dp_donnegrapin();
int depaneuse_dp_retour();
int depaneuse_dp_toaircraft();
int depaneuse_dp_init();
int depaneuse_dp_reflex();

int Ventouse_Ventouse_init();
int Ventouse_Ventouse_afterrec();
int Ventouse_Ventouse_explosion();
int Ventouse_Ventouse_avec_cible();

int vehicule_speed_max_vsm_test();

int sfx_pas_poussiere_spp_bv();
int sfx_pas_poussiere_spp_ar();
int sfx_pas_poussiere_spp_go();
int sfx_pas_poussiere_spp_exec_test();

int SFX_Arrose_SFX_Arrose_comportement();
int SFX_Arrose_SFX_Arrose_init();
int SFX_Arrose_SFX_Arrose_reflex();

int JeTeParle_jtp_revol();
int JeTeParle_jtp_mamago_bricole();
int JeTeParle_jtp_palet();
int JeTeParle_jtp_reflex();
int JeTeParle_jtp_exec_regarde();
int JeTeParle_jtp_init();
int JeTeParle__jtp_check();
int JeTeParle_jtp_rottete();
int JeTeParle_jtp_mamago_presente_objets();
int JeTeParle_jtp_mamago_bricole_AB();
int JeTeParle_jtp_palet_posperle();
int JeTeParle_jtp_revol_AB();
int Dialogue_dialogue_check();

int Vendeur_Vendeur_init();
int Vendeur_Vendeur_reflex();
int Vendeur_Vendeur_Comp_Bio();
int Vendeur_Vendeur_mamago();
int Vendeur_Vendeur_AfterBlend();
int Vendeur_Vendeur_exec_dialog();
int Vendeur_Vendeur_Comportements();
int Vendeur_Vendeur_Comp_Simple();
int Vendeur_Vendeur_Comp_Journaux();

int distrib_crunsh_dc_init();
int distrib_crunsh_dc_bouge();
int distrib_crunsh_dc_deroule();
int distrib_crunsh_dc_texte();
int distrib_crunsh_dc_exec_cam();
int distrib_crunsh_dc_reflex();
int distrib_crunsh_dc_tourne();

int Indice_Avancement_Jeu_iaj_init();
int Indice_Avancement_Jeu_iaj_change();

int Dialogue_Reset_All_SF_sfr_init();
int Dialogue_Reset_All_SF_sfr_reset();

int Cine_lipsynch_auto_Lips_init();
int Cine_lipsynch_auto_Lips_morphing();
int Cine_lipsynch_auto_Lips_lecture_messages();

int cine_perletransformation_init();
int cine_perletransformation_BeforeDisplay();
int cine_perletransformation_params();
int cine_perletransformation_wait();

int Perle_base_perl_cycle();
int Perle_base_perl_init();
int Perle_base_perl_transit();
int Perle_base_perl_zoom();
int Perle_base_perl_snap();

int perle_perle_afterrec();
int perle_perle_loop();
int perle_perle_init();
int perle_perle_dummy();
int perle_perle_meurt();
int perle_perle_meurtpied();
int perle_perle_goto_sally();

int main_fix_mf_init();

int SFX_XMEN_SFX_XMEN_init();
int SFX_XMEN_SFX_XMEN_afterrec_init();
int SFX_XMEN_SFX_XMEN_afterblend_init();
int SFX_XMEN_SFX_XMEN_set_moving_function();
int SFX_XMEN_SFX_XMEN_afterrec_move();
int SFX_XMEN_SFX_XMEN_afterblend_move();
int SFX_XMEN_SFX_XMEN_reflex();
int SFX_XMEN_SFX_XMEN_exec_GFX_Halo();
int SFX_XMEN_SFX_XMEN_Destroy();

int SFX_ChangeColor_AfterDisplay();
int SFX_ChangeColor_BeforeDisplay();

int INV_manager_INV_Init();
int INV_manager_Options_Exit();
int INV_manager_Buttons_Select();
int INV_manager_Loop();
int INV_manager_Update_Buttons();
int INV_manager_Select_Menu_Main();
int INV_manager_Update_Menu();
int INV_manager_Select_Menu_Options();
int INV_manager_Aide_Init();
int INV_manager_Select_Back();
int INV_manager_Move_3DObject();
int INV_manager_Move_Speed();
int INV_manager_ComputeModuloAngle();
int INV_manager_INV_InitFirstTrame();
int INV_manager_INV_InitStartGame();
int INV_manager_Menu_Object();
int INV_manager_Options_Loop();
int INV_manager_Update_Object();
int INV_manager_Select_Menu_Object();
int INV_manager_Options_Init();
int INV_manager_Update_PrevNextCase();
int INV_manager_Effect_MenuDisapear();
int INV_manager_Effect_MenuAppear();
int INV_manager_Update_MenuEnd();
int INV_manager_Update_MenuStart();
int INV_manager_Effect_MenuAppear2();
int INV_manager_Pognes_Close();
int INV_manager_Buttons_Next();
int INV_manager_Spiral_Init();
int INV_manager_Spiral_DisplayCase();
int INV_manager_Spiral_Loop();
int INV_manager_Spiral_Exit();
int INV_manager_Buttons_Mission();
int INV_manager_Mission_Params();
int INV_manager_Buttons_Object();
int INV_manager_Select_Menu_Mission();
int INV_manager_Perso_Eat();
int INV_manager_Update_Note();
int INV_manager_Pognes_Display();
int INV_manager_INV_Exit();
int INV_manager_Pognes_Init();
int INV_manager_Update_UpLeft();
int INV_manager_Loop2();
int INV_manager_Cheat_Loop();
int INV_manager_Loop_Cheat();
int INV_manager_Mission_Init();
int INV_manager_Mission_Exit();
int INV_manager_Mission_Loop();
int INV_manager_Loop_NewGame();
int INV_manager_INV_InitSpecialMode();
int INV_manager_Object_Init();
int INV_manager_Object_Loop();
int INV_manager_Object_Exit();
int Global_Library_photo_del();
int Global_Library_Perso_TreatEquipement();
int INV_manager_Object_SetPage();
int INV_manager_Perso_FillMenu();
int INV_manager_Perso_DisplayMenu();
int INV_manager_Perso_Select();
int INV_manager_Perso_GiveTo();
int INV_manager_Aide_Loop();
int INV_manager_Aide_Exit();
int INV_manager_Aide_MailDisplay();
int INV_manager_Aide_NoteDisplay();
int INV_manager_Update_Mail();
int INV_manager_Menu_Equip();
int INV_manager_Select_Menu_Equip();
int INV_manager_Buttons_Equip();
int INV_manager_Menu_Mission();
int INV_manager_Cheat_Exit();
int INV_manager_Buttons_Give();
int INV_manager_Perso_GiveToNote();
int INV_manager_Perso_CanUnequip();
int INV_manager_Object_ChargeurMDisk();
int INV_manager_Cheat_Init();
int INV_manager_Aide_DisplayPage();
int INV_manager_Perso_DropObject();
int INV_manager_MissionPhoto_Album();
int INV_manager_Buttons_Open();
int INV_manager_Perso_Unequip();
int INV_manager_Perso_Equip();
int INV_manager_Options_Display();
int INV_manager_Aide_DisplayCarte();
int INV_manager_Message();
int INV_manager_Message_Loop();
int INV_manager_MissionPhoto_Init();
int INV_manager_MissionPhoto_Loop();
int INV_manager_MissionPhoto_Exit();
int INV_manager_Mission_DisplayAnimalCollect();
int INV_manager_Mission_DisplayAnimalName();
int INV_manager_Message_Close();
int INV_manager_File_Init();
int INV_manager_File_Loop();
int INV_manager_File_Exit();
int INV_manager_File_DisplayDesc();
int INV_manager_Pognes_Loop();
int INV_manager_Mission_DisplayAnimalReward();
int INV_manager_File_CheckMC();
int INV_manager_Object_Appear();
int INV_manager_Object_Perle();

int INV_Photo_Loop();
int INV_Photo_LoopMode1();

int Letters_init();
int Letters_WaitRestart();

int SFX_Disapear2_init();
int SFX_Disapear2_loop();
int SFX_Disapear2_ComputeMatParams();

int SFX_FlashTexture_Fla_BeDisplay();
int SFX_FlashTexture_Fla_AfDisplay();
int SFX_FlashTexture_Fla_Flash_constant();

int main_logo_ml_init();
int main_logo_ml_load();
int main_logo_ml_init1();
int main_logo_ml_init2();
int main_logo_ml_trailer();

int main_E3_loadmap_me3_init();
int main_E3_loadmap_me3_finit();
int main_E3_loadmap_me3_waitload();

int INV_cacheobjet_loop();

int INV_SecundoEtLaFacture_init();
int INV_SecundoEtLaFacture_loop();
int INV_SecundoEtLaFacture_firstinit();

int cine_perledanslesac_init();
int cine_perledanslesac_loop();
int cine_perledanslesac_fct_pour_inventaire();

int armoire_arm_init();
int armoire_arm_go();

int Detect_o_Capa_doc_init();
int Detect_o_Capa_doc_une_a_faux();
int Detect_o_Capa_doc_une_vrai_une_faux();
int Detect_o_Capa_doc_une_a_vrai();
int Detect_o_Capa_doc_toutes_a_faux();
int Detect_o_Capa_doc_toutes_a_vrai();

int Indice_Jeu_Local_ijl_init();
int Indice_Jeu_Local_iaj_change();
int Indice_Jeu_Local_ijl_wait();

int Lecteur_Code_lc_init();
int Lecteur_Code_lc_reflex();
int Lecteur_Code_lc_attente_code();

int Porte_Armoire_Porte_Armoire_init();
int Porte_Armoire_Porte_Armoire_mode_wait();
int Porte_Armoire_Porte_Armoire_mode_open();
int Porte_Armoire_Porte_Armoire_mode_close();

int SND_SFnDELAY_sfd_init();
int SND_SFnDELAY_sfd_go();

int SFX_Phare_SFX_Phare_init();
int SFX_Phare_SFX_Phare_comportement();

int SND_Musique_Jour_Nuit_Zone_mjnz_play();

int Mobile_Mobile_comportement();
int Mobile_Mobile_init();

int Random_Track_Player_rtp_init();
int Random_Track_Player_trp_play();

int Grillon_Cigales_gc_init();
int Grillon_Cigales_gc_play();

int SND_MDF_periode_snd_init();
int SND_MDF_periode_snd_mp();

int SND_MusicFight_Init_mf_init();

int Snd_mdf_BVFrontiere_BVF_init();
int Snd_mdf_BVFrontiere_BVF_in_BV_cotevert();
int Snd_mdf_BVFrontiere_BVF_in_BV_pascotevert();
int Snd_mdf_BVFrontiere_BVF_out_BV();

int SFX_Dome_Hyllis_sdh_init();
int SFX_Dome_Hyllis_sdh_adisplay();
int SFX_Dome_Hyllis_sdh_bdisplay();
int SFX_Dome_Hyllis_sdh_reflex();

int Domz_Sansue_Domz_Sansue_init();
int Domz_Sansue_Domz_Sansue_mode_fight();
int Domz_Sansue_Domz_Sansue_exec_GFX_trace();
int Domz_Sansue_Domz_Sansue_exec_afterrec();
int Domz_Sansue_Domz_Sansue_Manager();
int Domz_Sansue_Domz_Sansue_mode_reward();
int Domz_Sansue_Domz_Sansue_exec_camera();
int Domz_Sansue_Domz_Sansue_mode_projectile();
int Domz_Sansue_Domz_Sansue_exec_get_target();
int Domz_Sansue_Domz_Sansue_exec_GFX_Flash();
int Domz_Sansue_Domz_Sansue_exec_init_snake();
int Domz_Sansue_Domz_Sansue_exec_do_snake();
int Domz_Sansue_Domz_Sansue_exec_gestion_paf();
int Domz_Sansue_Domz_Sansue_exec_GFX_Spark();
int Domz_Sansue_Domz_Sansue_exec_GFX_Boom();
int Domz_Sansue_Domz_Sansue_mode_apparition();
int Domz_Sansue_Domz_Sansue_mode_meurt();
int Domz_Sansue_Domz_Sansue_exec_GFX_AirSmoke();
int Domz_Sansue_Domz_Sansue_exec_meteor();
int Domz_Sansue_Domz_Sansue_mode_beluga();
int Domz_Sansue_Domz_Sansue_exec_compute_position();
int Domz_Sansue_Domz_Sansue_exec_compute_wp();

int Onde_de_Choc_ODC_init();
int Onde_de_Choc_ODC_comportement();
int Onde_de_Choc_ODC_bdisplay();
int Onde_de_Choc_ODC_adisplay();
int Onde_de_Choc_ODC_destroy();

int sfx_plouf_sfx_loop();
int sfx_plouf_sp_bdis();
int sfx_plouf_sp_adis();
int sfx_plouf_sp_init();

int Pod_De_Reparation_PDR_init();
int Pod_De_Reparation_PDR_mode_wait();
int Pod_De_Reparation_PDR_mode_plug();
int Pod_De_Reparation_PDR_mode_fix();
int Pod_De_Reparation_PDR_mode_unplug();
int Pod_De_Reparation_PDR_mode_reach();
int Pod_De_Reparation_PDR_mode_get_materia();
int Pod_De_Reparation_PDR_mode_back();

int Bison_Fute_Bison_Fute_init();
int Bison_Fute_Bison_Fute_comportement();

int CV_aircraft_CV_init();
int CV_aircraft_CV_mode_deplacement();
int CV_aircraft_CV_exec_waterforce();
int CV_aircraft_CV_exec_s_b();
int CV_aircraft_CV_exec_check_pos();
int CV_aircraft_CV_exec_test_collision();
int CV_aircraft_CV_exec_GFX_Halo();
int CV_aircraft_CV_exec_afterrec();
int CV_aircraft_CV_exec_GFX_Flammes();
int CV_aircraft_CV_exec_GFX_Smoke();
int CV_aircraft_CV_exec_before_display();
int CV_aircraft_CV_exec_after_display();
int CV_aircraft_CV_bdisplay();

int Laser_Laser_exec_shot();
int Laser_Laser_comportement();
int Laser_Laser_init();
int Laser_Laser_tempo_destroy();

int Pompier_Pompier_init();
int Pompier_Pompier_mode_wait();
int Pompier_Pompier_mode_hyllis();
int Pompier_Pompier_exec_GFX_Flash();
int Pompier_Police_mode_hyllis();
int Pompier_Police_mode_canaux();
int Pompier_Pompier_exec_get_aircraft();
int Pompier_Pompier_mode_canaux();
int Pompier_Pompier_exec_GFX_Smoke();
int Pompier_Pompier_exec_GFX_Halo();
int Pompier_Police_mode_wait();

int Porte_Anti_Domz_PAD_init();
int Porte_Anti_Domz_PAD_mode_fermee();
int Porte_Anti_Domz_PAD_mode_ouverte();
int Porte_Anti_Domz_PAD_mode_ouverture();
int Porte_Anti_Domz_PAD_mode_fermeture();

int hyllis_meteor_hm_init();
int hyllis_meteor_hm_loop();

int SFX_eclaire_se_go();
int SFX_eclaire_se_bdisplay();
int SFX_eclaire_se_adisplay();
int SFX_eclaire_se_meurt();
int SFX_eclaire_se_orient();

int monstroplante_death_mpd_beforedisplay();
int monstroplante_death_mpd_afterdisplay();
int monstroplante_death_mpd_loop();
int monstroplante_death_mpd_init();
int monstroplante_death_mpd_waitfordeath();

int SFX_Sally_Shield_SS_BDisplay();
int SFX_Sally_Shield_SS_ADisplay();
int SFX_Sally_Shield_SS_C_Apparition();
int SFX_Sally_Shield_SS_C_Disparition();
int SFX_Sally_Shield_SS_C_Wait();
int SFX_Sally_Shield_SS_C_init();

int detect_o_notes_don_test();

int SFX_JaugeEnnemi_jauge_init();
int SFX_JaugeEnnemi_jauge_comportement();

int Detect_o_SF_Avancement_DSFA_init();
int Detect_o_SF_Avancement_DSFA_comportement();

int Sally_TeteChercheuses_stc_init();
int Sally_TeteChercheuses_stc_go();
int Sally_TeteChercheuses_stc_end();
int Sally_TeteChercheuses_stc_loop();
int Sally_TeteChercheuses_stc_loop_end();

int Detect_o_16_SF_D16sf_init();
int Detect_o_16_SF_D16sf_go();

int Detect_o_SuperSpecialFlags_ssf_init();
int Detect_o_SuperSpecialFlags_ssf_ET_test();
int Detect_o_SuperSpecialFlags_ssf_ET_dialogue();
int Detect_o_SuperSpecialFlags_ssf_OU_test();
int Detect_o_SuperSpecialFlags_ssf_OU_dialogue();

int SFX_Fragments_SFX_frag_init();
int SFX_Fragments_SFX_frag_comportement();
int SFX_Fragments_SFX_frag_disparition();

int projectile_pr_init();
int projectile_pr_go();

int frisbee_fr_init();
int frisbee_fr_jet();
int frisbee_fr_destroy();
int frisbee_fr_disparition();
int frisbee_fr_rebound();
int frisbee_fr_impact();
int frisbee_fr_exec_GFX_Plouf();
int frisbee_fr_before_display();

int Object_Scalator_osc_init();
int Object_Scalator_osc_wait();
int Object_Scalator_osc_scale();
int Object_Scalator_osc_scale_objet();

int GAO_Shaker_GAOS_comportement();
int GAO_Shaker_GAOS_init();

int Ascenseur_Nazh_Asc_Nazh_init();
int Ascenseur_Nazh_Asc_Nazh_mode_wait();
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_companion();
int Ascenseur_Nazh_Asc_Nazh_mode_transfert_sally();
int Ascenseur_Nazh_Asc_Nazh_exec_GFX();
int Ascenseur_Nazh_Asc_Nazh_exec_comp_status();
int Ascenseur_Nazh_Asc_Nazh_exec_sound();

int Section_Alpha_Transport_SAT_init();
int Section_Alpha_Transport_SAT_exec_GFX_smoke();
int Section_Alpha_Transport_SAT_exec_GFX_Fire();
int Section_Alpha_Transport_SAT_exec_GFX_Glow();
int Section_Alpha_Transport_SAT_exec_afterrec();

int afx_manager_afx_msg();
int afx_manager_afx_loop();

int SFX_RedFlash_RF_comportement();
int SFX_RedFlash_RF_init();

int Enterprise_Ent_init();
int Enterprise_Ent_mode_flying();
int Enterprise_Ent_exec_shoot();
int Enterprise_Ent_exec_camera();
int Enterprise_Ent_reflex();
int Enterprise_Ent_exec_s_b();
int Enterprise_Ent_exec_constraint_Z();
int Enterprise_Ent_exec_constraint_Z_with_dist();
int Enterprise_Ent_exec_read_joy();
int Enterprise_Ent_exec_companions_leave_enterprise();
int Enterprise_Ent_exec_GFX_Burning();
int Enterprise_Ent_exec_snd_allume_les_moteurs();
int Enterprise_Ent_exec_select_init_mode();
int Enterprise_Ent_exec_test_atterrissage();
int Enterprise_Ent_mode_cine();
int Enterprise_Ent_unknown_demo_only();
int Enterprise_Ent_mode_landing();
int Enterprise_Ent_mode_on_ground();
int Enterprise_Ent_mode_takeoff();
int Enterprise_Ent_mode_wait_aircraft();
int Enterprise_Ent_exec_gestion_paf();
int Enterprise_Ent_mode_meurt();
int Enterprise_Ent_exec_constraint_collision();
int Enterprise_Ent_exec_afterrec();
int Enterprise_Ent_exec_change_mode();
int Enterprise_Ent_exec_water_force();
int Enterprise_Ent_exec_water_trace();
int Enterprise_Ent_exec_landing_effect();
int Enterprise_Ent_exec_s_b_space();
int Enterprise_Ent_mode_flying_no_gyro();
int Enterprise_Ent_exec_GFX_Reacteurs();
int Enterprise_Ent_exec_GFX_Transfert();
int Enterprise_Ent_exec_snd_stop_les_moteurs();
int Enterprise_Ent_exec_GFX_Flash();

int nrj_pure_np_sol();
int nrj_pure_np_timeout();
int nrj_pure_np_init();
int nrj_pure_np_afterrec();
int nrj_pure_np_air();
int nrj_pure_np_reviens();
int nrj_pure_np_exec_before_display();
int nrj_pure_np_exec_after_display();

int dead_if_invisible_dii_go();

int ent_shoot_simple_ess_init();
int ent_shoot_simple_ess_go();
int ent_shoot_simple_ess_tete_chercheuse();

int ent_shoot_follower_esf_init();
int ent_shoot_follower_est_exec_mvt();
int ent_shoot_follower_esf_follow();

int Ent_missile_EM_afterrec();
int Ent_missile_EM_comportement();
int Ent_missile_EM_disparition();
int Ent_missile_EM_explosion();
int Ent_missile_EM_init();
int Ent_missile_EM_reflex();

int Caisse_Man_Missile_CMM_init();
int Caisse_Man_Missile_CMM_comportement();
int Caisse_Man_Missile_CMM_reflex();
int Caisse_Man_Missile_CMM_explosion();
int Caisse_Man_Missile_CMM_afterrec();
int Caisse_Man_Missile_CMM_disparition();

int Garde_Barriere_Entrepot_GBE_init();
int Garde_Barriere_Entrepot_GBE_mode_attente();
int Garde_Barriere_Entrepot_GBE_mode_repousse();
int Garde_Barriere_Entrepot_GBE_mode_flotte();
int Garde_Barriere_Entrepot_GBE_exec_trace();
int Garde_Barriere_Entrepot_GBE_reflex();
int Garde_Barriere_Entrepot_GBE_mode_choppe();
int Garde_Barriere_Entrepot_GBE_exec_camera();
int Garde_Barriere_Entrepot_GBE_exec_GFX_anneaux();
int Garde_Barriere_Entrepot_GBE_exec_GFX_Lightning();
int Garde_Barriere_Entrepot_GBE_exec_GFX_Smoke();
int Garde_Barriere_Entrepot_GBE_unknown_demo();

int SFX_GoutteO_g_init();
int SFX_GoutteO_g_gener();
int SFX_GoutteO_g_move();
int SFX_GoutteO_g_ripple();

int Raskass_r_init();
int Raskass_r_attend_zone();
int Raskass_r_enter_zone();
int Raskass_r_move();
int Raskass_r_reflexe();
int Raskass_r_afterrec();
int Raskass_r_tracepos();
int Raskass_r_saute();
int Raskass_r_plouf();
int Raskass_r_ripple();
int Raskass_r_sortie_zone();
int Raskass_r_exec_testgens();

int Twin_Ion_Engine_TIE_init();
int Twin_Ion_Engine_TIE_exec_s_b();
int Twin_Ion_Engine_TIE_mode_flying();
int Twin_Ion_Engine_TIE_reflex();
int Twin_Ion_Engine_TIE_exec_gestion_paf();
int Twin_Ion_Engine_TIE_exec_constraint_Z();
int Twin_Ion_Engine_TIE_exec_afterrec();
int Twin_Ion_Engine_TIE_exec_constraint_collision();
int Twin_Ion_Engine_TIE_mode_meurt();
int Twin_Ion_Engine_TIE_mode_wait_for_target();

int Dog_Fight_Manager_DFM_init();
int Dog_Fight_Manager_DFM_comportement();

int Fun_baleine_bal_init();
int Fun_baleine_bal_plonge();
int Fun_baleine_bal_move();
int Fun_baleine_bal_saute();
int Fun_baleine_bal_afterrec();
int Fun_baleine_bal_queue();
int Fun_baleine_bal_plouf();
int Fun_baleine_bal_exec_snd();

int Jumping_Snake_JS_init();
int Jumping_Snake_JS_comportement();
int Jumping_Snake_JS_firstinit();

int Hyllis_Meteor_Manager_HMM_init();
int Hyllis_Meteor_Manager_HMM_mode_wait();
int Hyllis_Meteor_Manager_HMM_mode_aircraft();
int Hyllis_Meteor_Manager_HMM_mode_vaisseau();
int Hyllis_Meteor_Manager_HMM_mode_reinit();

int Pilot_Wing_Ring_PWR_init();
int Pilot_Wing_Ring_PWR_comportement();
int Pilot_Wing_Ring_PWR_exec_before_display();
int Pilot_Wing_Ring_PWR_exec_after_display();

int Vaisseau_Transport_Militaire_VTM_init();
int Vaisseau_Transport_Militaire_VTM_exec_s_b();
int Vaisseau_Transport_Militaire_VTM_mode_deplacement();
int Vaisseau_Transport_Militaire_VTM_exec_afterrec();
int Vaisseau_Transport_Militaire_VTM_reflex();
int Vaisseau_Transport_Militaire_VTM_exec_GFX();

int Bouee_Anti_Aircraft_BAA_init();
int Bouee_Anti_Aircraft_BAA_mode_dynamique();
int Bouee_Anti_Aircraft_BAA_exec_GFX_trace();
int Bouee_Anti_Aircraft_BAA_reflex();
int Bouee_Anti_Aircraft_BAA_exec_afterrec();
int Bouee_Anti_Aircraft_BAA_mode_statique();
int Bouee_Anti_Aircraft_BAA_exec_interception();

int Garde_Barriere_Mine_GBM_init();
int Garde_Barriere_Mine_GBM_attend();
int Garde_Barriere_Mine_GBM_attaque();
int Garde_Barriere_Mine_GBM_reflex();
int Garde_Barriere_Mine_GBM_depart();
int Garde_Barriere_Mine_GBM_retour();
int Garde_Barriere_Mine_GBM_afterrec();
int Garde_Barriere_Mine_GBM_exec_GFX();

int torpille_tor_exec_trace();
int torpille_tor_init();
int torpille_tor_mode_mer();
int torpille_tor_mode_explosion();
int torpille_tor_exec_visuel();
int torpille_tor_reflex();
int torpille_tor_mode_air();
int torpille_tor_exec_send_paf();
int torpille_tor_exec_test_collision();
int torpille_tor_exec_before_display();
int torpille_tor_exec_after_display();

int SF_Destroy_sfd_loop();

int tir_blok_hyllis_tbh_init();
int tir_blok_hyllis_tbh_attend();
int tir_blok_hyllis_tbh_shoot();
int tir_blok_hyllis_tbh_sort();
int tir_blok_hyllis_tbh_entre();

int dca_dca_loop();

int vaisseau_hyllis_flic_vhf_init();
int vaisseau_hyllis_flic_vhf_move();
int vaisseau_hyllis_flic_vhf_reflexe();
int vaisseau_hyllis_flic_vhf_shoot();
int vaisseau_hyllis_flic_vhf_part();

int Flic_Tir_ft_init();
int Flic_Tir_ft_afterr();
int Flic_Tir_ft_explode();
int Flic_Tir_ft_loop();

int ES_ciel_esc_enter_the_map();
int ES_ciel_esc_exit_the_map();
int ES_ciel_esc_joy();

int Des_Tourelle_laser_Des_TL_init();
int Des_Tourelle_laser_Des_TL_mode_attaque();
int Des_Tourelle_laser_Des_TL_reflex();
int Des_Tourelle_laser_Des_TL_exec_afterrec();
int Des_Tourelle_laser_Des_TL_mode_explosion();

int Garde_anti_vaisseau_gav_test();
int Garde_anti_vaisseau_gav_init();
int Garde_anti_vaisseau_gav_interception();

int AR_tickpoil_art_init();
int AR_tickpoil_art_marche();
int AR_tickpoil_art_att();
int AR_tickpoil_art_reflex();
int AR_tickpoil_art_firstinit();
int AR_tickpoil_art_meurt();

int Fun_EtoileFilante_ef_loop();
int Fun_EtoileFilante_ef_init();
int Fun_EtoileFilante_ef_move();

int Detect_Alarm_Global_dag_init();

int Empty_IA_Empty_IA_init();

int Video_Player_VP_int();
int Video_Player_VP_go();

int Zone_CompanionNetwork_nz_init();
int Zone_CompanionNetwork_nz_set();

int Detect_o_SpecialFlags_dosf_init();
int Detect_o_SpecialFlags_dosf_tests();

int BV_Active_Desactive_BV_In();
int BV_Active_Desactive_BV_Out();
int BV_Active_Desactive_BV_CBits_Out();
int BV_Active_Desactive_BV_CBits_In();
int BV_Active_Desactive_BV_AD();

int Detect_o_BV_CapaSet_detect_go();

int Zone_OrdreVala_zov_init();
int Zone_OrdreVala_zov_wait();
int Zone_OrdreVala_zov_ini();

int snap_on_actor_soa_init();
int snap_on_actor_soa_go();

int activateur_act_ini();

int Activateur_avec_SF_Acs_init();
int Activateur_avec_SF_Acs_loop();

int Snd_mdf_SF_snd_mdf_play();
int Snd_mdf_SF_snd_mdf_stop();
int Snd_mdf_SF_snd_mdf_init();

int SND_LoopVolFreqCapa_lvfc_watch();

int SND_CapaWatch_RandomPlay_cwrp_watch();

int action_reaction_ar_init();
int action_reaction_ar_check();
int action_reaction_ar_action();
int action_reaction_ar_exec_send_info();

int get_object_on_ground_goog_reflex();
int get_object_on_ground_goog_init();
int get_object_on_ground_goog_go();
int get_object_on_ground_goog_chute();

int LecteurCarte_lcrt_wait();
int LecteurCarte_lcrt_init();

int Barrique_Barrique_init();
int Barrique_Barrique_mode_balance();
int Barrique_Barrique_reflex();

int Generateur_lanceur_gela_normal();
int Generateur_lanceur_gela_init();

int Objet_genere_obge_init();
int Objet_genere_obge_normal();

int SFX_Disapear_dis_init();
int SFX_Disapear_dis_phases();
int SFX_Disapear_dis_before_display();
int SFX_Disapear_dis_after_display();

int SFX_Smoke_SFX_Smoke_init();
int SFX_Smoke_SFX_Smoke_comportement();

int SFX_Flash_SFX_Flash_init();
int SFX_Flash_SFX_Flash_comportement();

int activator_simple_ac_loop();

int InfoAnimaux_ia_init();

int main_newgame_mng_init();

int main_menu_mn_init();