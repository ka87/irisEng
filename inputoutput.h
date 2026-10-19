#pragma once
#include <time.h>
#include "common.h"
#include "math.h"

typedef struct sInoPad
{
    byte padData[16];
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
    __int16 year;
    __int16 s1;
    __int16 s2;
    __int16 s3;
    __int16 s4;
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    int i6;
    int i7;
    int i8;
    int i9;
    byte gap[12];
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
int INO_gi_FreeSpace;
char INO_gsz_SavFileName[128];
byte* INO_Keys;
byte* INO_Keys_Prev;
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

int INO_b_IsKeyPressed(uint8_t a1);
BOOL INO_b_IsKeyJustPressed(uint8_t a1);
BOOL INO_b_IsKeyJustReleased(uint8_t a1);
int INO_i_SetCurrentPad(int a1);
int INO_i_SetCurrentPad(int a1);
int INO_b_IoButtonPressed(int a1);
void INO_Joystick_Move(Vec3* a1, int a2);
BOOL INO_b_Joystick_IsButtonJustDown(int a1);
BOOL INO_b_Joystick_IsButtonJustUp(int a1);
void INO_JoySimulate(int a1, int a2, Vec3* a3, Vec3* a4);
void INO_PifSet(int a1);
void INO_PafSet(int a1, int a2);
int INO_i_PifPafEnable(int a1);
int INO_i_GetDiscError();
void INO_SetControlMode(int a1);
void INO_SetControlSubmode(int a1);
int INO_i_IsMouseBeingUsed();
int INO_i_GetCurrentPad();
int Demo_Geti(int a1);
int Demo_Seti(int a1);
BOOL INO_b_LanguageIsPresent(int a1);
void SAV_PhotoDel(int a1);
void SAV_PhotoDecal(int a1, int a2);
void SAV_LoadData(int a1);
void SAV_PhotoValidate();
void SAV_PhotoUnvalidate();
void INO_SavSetCardStatus(int a1, int a2);
void SAV_PhotoInit();
int INO_i_SavGetCardStatus(int a1, int* a2);
void SAV_PhotoSet(int a1, void* Src, size_t Size);
void INO_Joystick_Close(void);
void SAV_PhotoLastClose();
void INO_Reinit();
void* SAV_PhotoReinit();
void INO_Update();
void INO_Joystick_Init(HWND a1);
char* SAV_PhotoGet(int a1, int32_t* a2);
void SAV_PhotoFirstInit();
void INO_InitModule();
void INO_CloseModule();
BOOL INO_b_IsKeyReleased(uint8_t a1);
void SAV_PhotoClose();