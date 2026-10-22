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
    SIDE: 54 key(s) overridden from ...\sideload\

With no folder there, nothing changes: the engine behaves exactly as it
did before.

## What is here

Every behaviour recovered from the retail 2003 game so far: the five
`main_logo` ones, the `INV_manager` inventory and pause screen, and
the small generic objects of the same world — the colour effects, the
letters screen, the X-MEN rig, `Global_Library`.

Each was written as source, compiled, and scored against the chunk in
the user's binarized world: local pool, node count and string block,
all three. "exact" below means all three matched. The four marked
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
| `2600345E.ofc` | `SFX_XMEN_SFX_XMEN_Destroy` | 2 | exact |
| `35009407.ofc` | `SFX_FlashTexture_Fla_BeDisplay` | 8 | exact |
| `35009408.ofc` | `SFX_FlashTexture_Fla_AfDisplay` | 4 | exact |
| `3D003882.ofc` | `main_logo_ml_load` | 34 | exact |
| `3D003A94.ofc` | `main_logo_ml_init1` | 926 | not exact |
| `4900FDCD.ofc` | `INV_manager_Update_Buttons` | 147 | 5 node(s) short |
| `4900FDD3.ofc` | `INV_manager_Update_Menu` | 42 | exact |
| `4900FDF4.ofc` | `INV_manager_ComputeModuloAngle` | 29 | exact |
| `4900FE08.ofc` | `SFX_ChangeColor_AfterDisplay` | 4 | exact |
| `4900FE09.ofc` | `SFX_ChangeColor_BeforeDisplay` | 8 | exact |
| `4900FE1A.ofc` | `INV_manager_Buttons_Select` | 55 | exact |
| `4900FE1B.ofc` | `(unnamed)` | 1 | exact |
| `4900FE21.ofc` | `INV_manager_INV_InitStartGame` | 86 | exact |
| `4900FE47.ofc` | `INV_manager_Select_Menu_Object` | 78 | exact |
| `4900FE54.ofc` | `INV_manager_Effect_MenuDisapear` | 16 | exact |
| `4900FE57.ofc` | `INV_manager_Update_MenuStart` | 72 | exact |
| `4900FE59.ofc` | `INV_manager_Effect_MenuAppear2` | 40 | exact |
| `4900FE5F.ofc` | `INV_manager_Select_Back` | 133 | exact |
| `4900FECE.ofc` | `INV_manager_Buttons_Object` | 104 | exact |
| `4900FF1D.ofc` | `INV_manager_Update_Note` | 122 | exact |
| `4900FFF6.ofc` | `INV_manager_Buttons_Next` | 37 | exact |
| `490186B4.ofc` | `INV_manager_Loop2` | 100 | exact |
| `4901871E.ofc` | `(unnamed)` | 1 | exact |
| `49018728.ofc` | `INV_manager_Loop_Cheat` | 107 | exact |
| `49018734.ofc` | `INV_Photo_Loop` | 39 | exact |
| `49018A17.ofc` | `Global_Library_photo_del` | 47 | 1 node(s) short |
| `49018B57.ofc` | `Letters_WaitRestart` | 5 | exact |
| `49018B67.ofc` | `INV_manager_Perso_Eat` | 121 | exact |
| `49018B76.ofc` | `INV_manager_Select_Menu_Mission` | 49 | exact |
| `49018B8D.ofc` | `Global_Library_Perso_TreatEquipement` | 94 | exact |
| `49019640.ofc` | `INV_manager_Update_Mail` | 56 | exact |
| `49019BBF.ofc` | `INV_manager_Buttons_Equip` | 96 | exact |
| `49019BC9.ofc` | `INV_manager_Buttons_Give` | 68 | exact |
| `49019BCB.ofc` | `INV_manager_Perso_GiveToNote` | 147 | 1 node(s) short |
| `49019C6C.ofc` | `INV_manager_Perso_CanUnequip` | 77 | exact |
| `4901A410.ofc` | `INV_manager_Buttons_Open` | 53 | exact |
| `4901A4EB.ofc` | `INV_manager_Perso_Unequip` | 29 | exact |
| `4901A4EC.ofc` | `INV_manager_Perso_Equip` | 77 | exact |
| `4901AD1B.ofc` | `INV_manager_Message_Loop` | 133 | exact |
| `4901B107.ofc` | `(unnamed)` | 1 | exact |
| `4901BA51.ofc` | `INV_manager_Message_Close` | 21 | exact |
| `4901C07B.ofc` | `INV_manager_Perso_DropObject` | 52 | exact |
| `4901C084.ofc` | `INV_manager_Aide_DisplayPage` | 60 | exact |
| `4901C089.ofc` | `(unnamed)` | 1 | exact |
| `4901C08A.ofc` | `INV_manager_Cheat_Loop` | 55 | exact |
| `4902121B.ofc` | `SFX_Disapear2_init` | 6 | exact |
| `49027635.ofc` | `main_logo_ml_init2` | 2612 | not exact |
| `490289B1.ofc` | `main_logo_ml_initnopad` | 1 | exact |
| `490294BF.ofc` | `main_logo_ml_trailer` | 137 | exact |
| `5E006324.ofc` | `main_logo_ml_init` | 244 | exact |
| `5E0084E3.ofc` | `main_fix_mf_init` | 19 | exact |
| `7200555A.ofc` | `SFX_XMEN_SFX_XMEN_set_moving_function` | 55 | exact |
| `7200555B.ofc` | `SFX_XMEN_SFX_XMEN_afterrec_move` | 17 | 1 node(s) short |
| `72005563.ofc` | `SFX_XMEN_SFX_XMEN_reflex` | 52 | exact |

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
