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

typedef struct sSavIdxElem
{
    char superiorAtOrdering;
    char sec;
    char min;
    char hour;
    char mday;
    char month;
    int16_t year;
    int16_t s1;
    int16_t s2;
    int16_t s3;
    int16_t s4;
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    int i6;
    int i7;
    int i8;
    int i9;
    uint8_t gap[12];
} tSavIdxElem;

typedef struct sSavSlotDesc
{
    int id;
    int i2;
    int i3;
    int i4;
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

int SAV_idx_Field_i2;
int SAV_idx_Field_i3;
int SAV_idx_Field_i4;
int SAV_idx_Field_i5;
int SAV_idx_Field_i6;
int SAV_idx_Field_i7;
int SAV_idx_Field_i8;
int SAV_idx_Field_i9;

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