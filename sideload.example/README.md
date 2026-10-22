# Sideloading a recovered script

Copy this folder next to the built exe and rename it `sideload`:

    <build output>\irisEng.exe
    <build output>\sideload\5E006324.ofc
    ...

On startup `SIDE_InitModule` lists the folder and notes every file whose
name is eight hex digits. From then on, loading one of those keys reads
the file instead of the `.bf` member, and runs it through the
interpreter even where the behaviour has been transcribed into C. The
debug output says what it picked up:

    SIDE: 0x5e006324 <- 5E006324.ofc
    SIDE: 60 key(s) overridden from ...\sideload\

With no folder there, nothing changes: the engine behaves exactly as it
did before.

## What is here

Every behaviour recovered from the retail 2003 game so far: the five
`main_logo` ones, the `INV_manager` inventory and pause screen, the
small generic objects of the same world — the colour effects, the
letters screen, the X-MEN rig, `Global_Library` — `main_fix`, which
starts everything, and the first of the event machinery from
`ff0045a2`: the cinematic hooks, the activator and the detectors.

Each was written as source, compiled, and scored against the chunk in
the user's binarized world: local pool, node count and string block,
all three. "exact" below means all three matched. The five marked
short read cleanly and agree on everything else, but miss the node
count by a little and are flagged rather than padded; the two marked
"not exact" are works in progress and are the two largest.

`main_fix_mf_init` is the one that starts everything: it sets the
language and two sound parameters and then asks for world 1000. Its
chunk lives in `ff0084df`, which turns out to hold that one behaviour
and nothing else.

Four behaviours are empty in retail and compile to a single node.
They are worth having here anyway: dropping one in takes the
transcribed C out of the path and lets the engine do what the 2003
game does, which is nothing.

| file | behaviour | nodes | |
|---|---|---|---|
0x2600345E      1 nodes      52 bytes  <- 0x2600345E_SFX_XMEN_SFX_XMEN_Destroy.fct
| `2600345E.ofc` | `SFX_XMEN_SFX_XMEN_Destroy` | 2 | exact |
0x26004C6C      6 nodes     132 bytes  <- 0x26004C6C_Cine_Events_cine_firstinit.fct
| `26004C6C.ofc` | `Cine_Events_cine_firstinit` | 7 | exact |
0x350091AD     18 nodes     415 bytes  <- 0x350091AD_Cine_Textes_CiTxt_init.fct
| `350091AD.ofc` | `Cine_Textes_CiTxt_init` | 19 | exact |
0x35009407      7 nodes     148 bytes  <- 0x35009407_SFX_FlashTexture_Fla_BeDisplay.fct
| `35009407.ofc` | `SFX_FlashTexture_Fla_BeDisplay` | 8 | exact |
0x35009408      3 nodes      84 bytes  <- 0x35009408_SFX_FlashTexture_Fla_AfDisplay.fct
| `35009408.ofc` | `SFX_FlashTexture_Fla_AfDisplay` | 4 | exact |
0x3D003882     33 nodes     604 bytes  <- 0x3D003882_main_logo_ml_load.fct
| `3D003882.ofc` | `main_logo_ml_load` | 34 | exact |
0x3D003A94    925 nodes   15150 bytes  <- 0x3D003A94_main_logo_ml_init1.fct
| `3D003A94.ofc` | `main_logo_ml_init1` | 926 | not exact |
0x4900FDCD    146 nodes    2412 bytes  <- 0x4900FDCD_INV_manager_Update_Buttons.fct
| `4900FDCD.ofc` | `INV_manager_Update_Buttons` | 147 | 5 node(s) short |
0x4900FDD3     41 nodes     694 bytes  <- 0x4900FDD3_INV_manager_Update_Menu.fct
| `4900FDD3.ofc` | `INV_manager_Update_Menu` | 42 | exact |
0x4900FDF4     28 nodes     484 bytes  <- 0x4900FDF4_INV_manager_ComputeModuloAngle.fct
| `4900FDF4.ofc` | `INV_manager_ComputeModuloAngle` | 29 | exact |
0x4900FE08      3 nodes      84 bytes  <- 0x4900FE08_SFX_ChangeColor_AfterDisplay.fct
| `4900FE08.ofc` | `SFX_ChangeColor_AfterDisplay` | 4 | exact |
0x4900FE09      7 nodes     148 bytes  <- 0x4900FE09_SFX_ChangeColor_BeforeDisplay.fct
| `4900FE09.ofc` | `SFX_ChangeColor_BeforeDisplay` | 8 | exact |
0x4900FE1A     54 nodes     940 bytes  <- 0x4900FE1A_INV_manager_Buttons_Select.fct
| `4900FE1A.ofc` | `INV_manager_Buttons_Select` | 55 | exact |
0x4900FE1B      0 nodes      36 bytes  <- 0x4900FE1B.fct
| `4900FE1B.ofc` | `(unnamed)` | 1 | exact |
0x4900FE21     85 nodes    1396 bytes  <- 0x4900FE21_INV_manager_INV_InitStartGame.fct
| `4900FE21.ofc` | `INV_manager_INV_InitStartGame` | 86 | exact |
0x4900FE47     77 nodes    1348 bytes  <- 0x4900FE47_INV_manager_Select_Menu_Object.fct
| `4900FE47.ofc` | `INV_manager_Select_Menu_Object` | 78 | exact |
0x4900FE54     15 nodes     316 bytes  <- 0x4900FE54_INV_manager_Effect_MenuDisapear.fct
| `4900FE54.ofc` | `INV_manager_Effect_MenuDisapear` | 16 | exact |
0x4900FE57     71 nodes    1172 bytes  <- 0x4900FE57_INV_manager_Update_MenuStart.fct
| `4900FE57.ofc` | `INV_manager_Update_MenuStart` | 72 | exact |
0x4900FE59     39 nodes     820 bytes  <- 0x4900FE59_INV_manager_Effect_MenuAppear2.fct
| `4900FE59.ofc` | `INV_manager_Effect_MenuAppear2` | 40 | exact |
0x4900FE5F    132 nodes    2308 bytes  <- 0x4900FE5F_INV_manager_Select_Back.fct
| `4900FE5F.ofc` | `INV_manager_Select_Back` | 133 | exact |
0x4900FECE    103 nodes    1764 bytes  <- 0x4900FECE_INV_manager_Buttons_Object.fct
| `4900FECE.ofc` | `INV_manager_Buttons_Object` | 104 | exact |
0x4900FF1D    121 nodes    2013 bytes  <- 0x4900FF1D_INV_manager_Update_Note.fct
| `4900FF1D.ofc` | `INV_manager_Update_Note` | 122 | exact |
0x4900FFF6     36 nodes     612 bytes  <- 0x4900FFF6_INV_manager_Buttons_Next.fct
| `4900FFF6.ofc` | `INV_manager_Buttons_Next` | 37 | exact |
0x490186B4     99 nodes    1660 bytes  <- 0x490186B4_INV_manager_Loop2.fct
| `490186B4.ofc` | `INV_manager_Loop2` | 100 | exact |
0x4901871E      0 nodes      36 bytes  <- 0x4901871E.fct
| `4901871E.ofc` | `(unnamed)` | 1 | exact |
0x49018728    106 nodes    1852 bytes  <- 0x49018728_INV_manager_Loop_Cheat.fct
| `49018728.ofc` | `INV_manager_Loop_Cheat` | 107 | exact |
0x49018734     38 nodes     644 bytes  <- 0x49018734_INV_Photo_Loop.fct
| `49018734.ofc` | `INV_Photo_Loop` | 39 | exact |
0x49018A17     46 nodes     852 bytes  <- 0x49018A17_Global_Library_photo_del.fct
| `49018A17.ofc` | `Global_Library_photo_del` | 47 | 1 node(s) short |
0x49018B57      4 nodes     140 bytes  <- 0x49018B57_Letters_WaitRestart.fct
| `49018B57.ofc` | `Letters_WaitRestart` | 5 | exact |
0x49018B67    120 nodes    2076 bytes  <- 0x49018B67_INV_manager_Perso_Eat.fct
| `49018B67.ofc` | `INV_manager_Perso_Eat` | 121 | exact |
0x49018B76     48 nodes     804 bytes  <- 0x49018B76_INV_manager_Select_Menu_Mission.fct
| `49018B76.ofc` | `INV_manager_Select_Menu_Mission` | 49 | exact |
0x49018B8D     93 nodes    1564 bytes  <- 0x49018B8D_Global_Library_Perso_TreatEquipement.fct
| `49018B8D.ofc` | `Global_Library_Perso_TreatEquipement` | 94 | exact |
0x49019640     55 nodes     916 bytes  <- 0x49019640_INV_manager_Update_Mail.fct
| `49019640.ofc` | `INV_manager_Update_Mail` | 56 | exact |
0x49019BBF     95 nodes    1636 bytes  <- 0x49019BBF_INV_manager_Buttons_Equip.fct
| `49019BBF.ofc` | `INV_manager_Buttons_Equip` | 96 | exact |
0x49019BC9     67 nodes    1148 bytes  <- 0x49019BC9_INV_manager_Buttons_Give.fct
| `49019BC9.ofc` | `INV_manager_Buttons_Give` | 68 | exact |
0x49019BCB    146 nodes    2453 bytes  <- 0x49019BCB_INV_manager_Perso_GiveToNote.fct
| `49019BCB.ofc` | `INV_manager_Perso_GiveToNote` | 147 | 1 node(s) short |
0x49019C6C     76 nodes    1412 bytes  <- 0x49019C6C_INV_manager_Perso_CanUnequip.fct
| `49019C6C.ofc` | `INV_manager_Perso_CanUnequip` | 77 | exact |
0x4901A410     52 nodes     868 bytes  <- 0x4901A410_INV_manager_Buttons_Open.fct
| `4901A410.ofc` | `INV_manager_Buttons_Open` | 53 | exact |
0x4901A4EB     28 nodes     564 bytes  <- 0x4901A4EB_INV_manager_Perso_Unequip.fct
| `4901A4EB.ofc` | `INV_manager_Perso_Unequip` | 29 | exact |
0x4901A4EC     76 nodes    1372 bytes  <- 0x4901A4EC_INV_manager_Perso_Equip.fct
| `4901A4EC.ofc` | `INV_manager_Perso_Equip` | 77 | exact |
0x4901AD1B    132 nodes    2310 bytes  <- 0x4901AD1B_INV_manager_Message_Loop.fct
| `4901AD1B.ofc` | `INV_manager_Message_Loop` | 133 | exact |
0x4901B107      0 nodes      36 bytes  <- 0x4901B107.fct
| `4901B107.ofc` | `(unnamed)` | 1 | exact |
0x4901BA51     20 nodes     358 bytes  <- 0x4901BA51_INV_manager_Message_Close.fct
| `4901BA51.ofc` | `INV_manager_Message_Close` | 21 | exact |
0x4901C07B     51 nodes    1052 bytes  <- 0x4901C07B_INV_manager_Perso_DropObject.fct
| `4901C07B.ofc` | `INV_manager_Perso_DropObject` | 52 | exact |
0x4901C084     59 nodes    1301 bytes  <- 0x4901C084_INV_manager_Aide_DisplayPage.fct
| `4901C084.ofc` | `INV_manager_Aide_DisplayPage` | 60 | exact |
0x4901C089      0 nodes      36 bytes  <- 0x4901C089.fct
| `4901C089.ofc` | `(unnamed)` | 1 | exact |
0x4901C08A     54 nodes    1140 bytes  <- 0x4901C08A_INV_manager_Cheat_Loop.fct
| `4901C08A.ofc` | `INV_manager_Cheat_Loop` | 55 | exact |
0x4902121B      5 nodes     116 bytes  <- 0x4902121B_SFX_Disapear2_init.fct
| `4902121B.ofc` | `SFX_Disapear2_init` | 6 | exact |
0x49027635   2611 nodes   42184 bytes  <- 0x49027635_main_logo_ml_init2.fct
| `49027635.ofc` | `main_logo_ml_init2` | 2612 | not exact |
0x490289B1      0 nodes      36 bytes  <- 0x490289B1_main_logo_ml_initnopad.fct
| `490289B1.ofc` | `main_logo_ml_initnopad` | 1 | exact |
0x490294BF    136 nodes    2292 bytes  <- 0x490294BF_main_logo_ml_trailer.fct
| `490294BF.ofc` | `main_logo_ml_trailer` | 137 | exact |
0x5E006324    243 nodes    4004 bytes  <- 0x5E006324_main_logo_ml_init.fct
| `5E006324.ofc` | `main_logo_ml_init` | 244 | exact |
0x5E0084E3     18 nodes     324 bytes  <- 0x5E0084E3_main_fix_mf_init.fct
| `5E0084E3.ofc` | `main_fix_mf_init` | 19 | exact |
0x5E008A18     13 nodes     244 bytes  <- 0x5E008A18_Activator_act_init.fct
| `5E008A18.ofc` | `Activator_act_init` | 14 | exact |
0x7100E394     36 nodes     652 bytes  <- 0x7100E394_Detect_o_4SpecialFlags_D4sf_init.fct
| `7100E394.ofc` | `Detect_o_4SpecialFlags_D4sf_init` | 37 | 1 node(s) short |
0x7200555A     54 nodes     900 bytes  <- 0x7200555A_SFX_XMEN_SFX_XMEN_set_moving_function.fct
| `7200555A.ofc` | `SFX_XMEN_SFX_XMEN_set_moving_function` | 55 | exact |
0x7200555B     16 nodes     292 bytes  <- 0x7200555B_SFX_XMEN_SFX_XMEN_afterrec_move.fct
| `7200555B.ofc` | `SFX_XMEN_SFX_XMEN_afterrec_move` | 17 | 1 node(s) short |
0x72005563     51 nodes     852 bytes  <- 0x72005563_SFX_XMEN_SFX_XMEN_reflex.fct
| `72005563.ofc` | `SFX_XMEN_SFX_XMEN_reflex` | 52 | exact |
0x9E003B9B     22 nodes     388 bytes  <- 0x9E003B9B_Detect_o_Time_dot_init.fct
| `9E003B9B.ofc` | `Detect_o_Time_dot_init` | 23 | exact |
0x9E004478      9 nodes     180 bytes  <- 0x9E004478_Detect_o_16_SF_D16sf_init.fct
| `9E004478.ofc` | `Detect_o_16_SF_D16sf_init` | 10 | exact |

The sources these were built from are in the decompiler's
`retail/fct/`, and what was found while recovering each one — and
which source form was chosen where more than one was possible — is in
`retail/notes/`.

`extra/5E006325.ova` is the matching variable file for the `main_logo`
model, rebuilt and byte-identical to the one in the binarized world.
Subfolders are not scanned, so it does nothing where it sits; move it up
a level to sideload the model's variables too. The full game already has
this layout, so it is only worth doing to check the rebuild itself.

## Naming

The part before the first dot is the key, eight hex digits, optionally
`0x`-prefixed. The extension is ignored. A name that is not a key is
skipped, so notes and spare copies are safe to leave in the folder.

## interpret.txt

Keys listed there are interpreted from the archive's own nodes, with no
replacement file. Use it to take a transcribed behaviour out of the C
path and watch the original script run.

## A caveat

A sideloaded file is a standalone member, not a binarized one, so every
field is present. The loader turns `LOA_gb_SpeedMode` off while it
parses one and puts it back afterwards. Dropping in a chunk carved out
of a binarized world file will not work; it has to be the standalone
form.
