#pragma once
#include <time.h>
#include "common.h"
#include "math.h"

typedef struct sInoPad
{
    uint8_t padData[16];
} tInoPad;

typedef struct sInoButtonNames
{
    char str[64];
} tInoButtonNames;

tInoButtonNames buttonNames[18];

typedef struct sJoySimulateCurrentStruct
{
    int controlsMask;
    int buttons;
    float amountXLeft;
    float amountYLeft;
    float amountXRight;
    float amountYRight;
} tJoySimulateCurrentStruct;

/* One slot's line in the index file: enough to draw the load menu
   without opening the save itself, plus the figures the code shown on
   the summary screen is worked out from.

   Everything below the timestamp comes from the script that asks for
   the save. Three of them arrive with the save itself; the rest are
   handed over beforehand and wait in the SAV_idx_* globals until a
   slot is written. */
typedef struct sSavIdxElem
{
    char superiorAtOrdering;
    char sec;
    char min;
    char hour;
    char mday;
    char month;
    int16_t year;
    int16_t worldId;          /* which map, for the name on the menu */
    int16_t worldEntranceId;  /* which way in, for reloading the spot */
    int16_t s3;               /* a script field reads it, nothing writes it */
    int16_t s4;               /* likewise */
    int playTimeSeconds;      /* total elapsed play, shown as h:mm:ss */
    /* Three counts packed a byte apiece: collectibles, photographs
       taken, and a pair of one-off finds. */
    int progressCounts;
    int miniGameBestScore;    /* highest reached, kept as a maximum */
    /* Best lap times, in hundredths. The fourth is never written:
       whatever sets it writes the first a second time instead, so the
       first lap record is lost and the fourth stays zero in every
       save. That is how saves are produced, so it stays. */
    int raceBestTime1;
    int raceBestTime2;
    int raceBestTime3;
    int raceBestTime4;
    int codeChars;            /* four characters, one per byte */
    int deathCount;           /* capped at 255 before it gets here */
    uint8_t gap[12];
} tSavIdxElem;

/* The index file is ten of these and nothing else, so the record
   has to stay exactly this wide: a byte either way and every slot
   after the first is read from the wrong place, with no error to
   say so. */
IRIS_STATIC_ASSERT(sizeof(tSavIdxElem) == 64,
               "the save index record must stay 64 bytes wide");

/* What a script hands over when it asks for a slot to be written.
   The three values go straight into that slot's index line. */
typedef struct sSavSlotDesc
{
    int id;
    int worldId;
    int worldEntranceId;
    int playTimeSeconds;
} tSavSlotDesc;

typedef struct sPhotoElem
{
    int startOffset;
    int photoSize;
} tPhotoElem;

typedef struct sPhotoStruct
{
    tPhotoElem elems[100];
    unsigned int photoBufSize;
    int usedSize;
    char* photoBuf;
} tPhotoStruct;

tPhotoStruct SAV_gst_Photos;
tPhotoStruct SAV_gst_PhotosValidate;
tSavIdxElem SAV_gst_Data[10];
tSavSlotDesc INO_gst_SavSlotDesc;
char* SAV_pc_PhotoValidateBuffer;
int INO_gb_ReadIdxForSaving;

int INO_l_Joystick_Enable;
int INO_gi_CurrentPadId;

tJoySimulateCurrentStruct INO_gst_JoySimulateAsked;
tJoySimulateCurrentStruct INO_gst_JoySimulateCurrent;
tInoPad INO_gst_CurrPad[4];
tInoPad INO_gst_PrevPad[4];

int INO_l_JoystickMask;
int INO_l_JoystickDir;
int INO_l_Joystick_Enable;
float INO_gi_PifDuration;
float INO_gi_PafDuration;
int INO_gi_PafIntensity;
int INO_gi_PifPafEnable;
int INO_gi_DiscError;

int SAVE_gi_DefaultSlot;

int SAV_idx_ProgressCounts;
int SAV_idx_MiniGameBestScore;
int SAV_idx_RaceBestTime1;
int SAV_idx_RaceBestTime2;
int SAV_idx_RaceBestTime3;
int SAV_idx_RaceBestTime4;
int SAV_idx_CodeChars;
int SAV_idx_DeathCount;

int INO_gi_SavButton[8];
extern int INO_ge_SavSetNextState;
extern int INO_ge_SavCommand;
/* Size of the photo buffer, and the free disk space a slot write
   needs. The two are the same figure on purpose: a slot carries the
   photo block, so there is no point starting a write that cannot
   hold one. Changing one without the other either rejects every
   save or writes a block that will not read back. */
#define SAV_PHOTO_BUFFER_SIZE 1638400

int INO_gi_FreeSpace;
char INO_gsz_SavFileName[128];
uint8_t* INO_Keys;
uint8_t* INO_Keys_Prev;
char INO_keybUnkDataProbablyUnused[128];
int Demo_gb_Exit; // TODO FIXME: initelni nem kéne?

int (*INO_gst_MemoryCardManager[43])();

void(*s_JoystickInit)(HWND);
void(*s_JoystickClose)();
void(*s_JoystickUpdate)(struct sInoPad*);
void(*s_JoystickPifPafEnable)(BOOL);
void(*s_JoystickPafIntensity)(int);
void(*s_joyMoveFunction)(Vec3*, int);
int(*s_IsMouseBeingUsedFunction)();
int(*s_SetCurrentPad)(int);

int INO_b_IsKeyPressed(uint8_t key);
BOOL INO_b_IsKeyJustPressed(uint8_t key);
BOOL INO_b_IsKeyJustReleased(uint8_t key);
int INO_i_SetCurrentPad(int pad);
int INO_b_IoButtonPressed(int button);
void INO_Joystick_Move(Vec3* move, int stickID);
BOOL INO_b_Joystick_IsButtonJustDown(int button);
BOOL INO_b_Joystick_IsButtonJustUp(int button);
void INO_JoySimulate(int controlMask, int buttons, Vec3* leftStick, Vec3* rightStick);
void INO_PifSet(int duration);
void INO_PafSet(int intensity, int duration);
int INO_i_PifPafEnable(int enable);
int INO_i_GetDiscError();
void INO_SetControlMode(int mode);
/* Keeps the game's own input away from the pad and the keyboard
   without touching the control mode, and without touching the memory
   card manager, so saving and loading still work. Nests: a second
   suspend does nothing and one release ends it. */
void INO_SuspendInput(int suspend);
int  INO_b_InputSuspended(void);

/* The joystick mask as the game sees it. While the input is
   suspended the live mask belongs to whatever took it, and these go
   to the copy the game will get back -- so a script that saves the
   mask, freezes something and puts the mask back finds what it left
   rather than the suspension. Anything acting for the game uses
   these rather than INO_l_JoystickMask. */
int  INO_i_GameJoystickMask(void);
void INO_GameJoystickMaskSet(int mask);
void INO_SetMouseSensitivity(float sensitivity);
void INO_SetControlSubmode(int submode);
int INO_i_IsMouseBeingUsed();
int INO_i_GetCurrentPad();
int Demo_Geti(int field);
int Demo_Seti(int field);
BOOL INO_b_LanguageIsPresent(int lang);
void SAV_PhotoDel(int photo);
void SAV_PhotoDecal(int photo, int count);
void SAV_LoadData(int forSaving);
void SAV_PhotoValidate();
void SAV_PhotoUnvalidate();
void INO_SavSetCardStatus(int slot, int status);
void SAV_PhotoInit();
int INO_i_SavGetCardStatus(int slot, int* savButtons);
void SAV_PhotoSet(int photo, void* src, size_t size);
void INO_Joystick_Close(void);
void SAV_PhotoLastClose();
void INO_Reinit();
void* SAV_PhotoReinit();
void INO_Update();
void INO_Joystick_Init(HWND hwnd);
char* SAV_PhotoGet(int photo, int32_t* size);
void SAV_PhotoFirstInit();
void INO_InitModule();
void INO_CloseModule();
BOOL INO_b_IsKeyReleased(uint8_t key);
void SAV_PhotoClose();