#pragma once
#include "../common.h"
#include "../inputoutput.h"
#include "../math.h"

#define DIRECTINPUT_VERSION  0x0800

#include <dinput.h>

typedef struct sSaveFileDesc
{
    FILE* fp;
    char* saveBuffer;
    int size;
} tSaveFileDesc;

tSaveFileDesc INO_gst_SavFileDesc;

HWND pcINO_Hwnd;

IDirectInput8A* dinput_if;
IDirectInputDevice8A* mouse_if_dword_98F248;
IDirectInputDevice8A* keyb_if_dword_98F238;
int mouseGranularity;
Vec3 pcINO_joystickPosition[2];

int pcINO_stateMachineFlags;
//float pcINO_case1MouseAmountX;
//float pcINO_case1MouseAmountY;
float pcINO_case1DividedTime2;
float pcINO_case1DividedTime;
float pcINO_case2DividedTime;
int pcINO_case2_mouseScroll;
float pcINO_case4_mouseMoveX;
float pcINO_case4_mouseMoveY;
float pcINO_case4_probablyUnunsedUnk1;
int pcINO_case64StateMachineFlags;
float pcINO_case64ElapsedTime;

char pcINO_keyboardDataBackup[264];
char pcINO_mouseTwoBytesDataBackup[2];

void (*sJoystickHandlerFunction)(struct sInoPad*, int*, byte*);
char* pcINO_keyMaps;
short pcINO_unkKeyStateTest;

int pcINO_gi_LastError;

char* pc_Buffer_304;
int pcINO_SAVPhotosSize;
char pcINO_KeyNamesToRet[256];
extern int pcINO_SelectedControlMode;
extern int pcINO_SelectedControlSubmode;
extern char prefixOneSpace[8];

void pcINO_Joystick_Init(HWND hwnd);
void pcINO_Joystick_Close();
void pcINO_Joystick_Move(Vec3* a1, int stickID);
int pcINO_i_IsMouseBeingUsed();
int pcINO_i_SetCurrentPad(int a1);
void pcINO_JoystickPifPafEnable(BOOL b);
void pcINO_JoystickPafIntensity(int i1);
void pcINO_Joystick_Update(struct sInoPad* padData);
void pcINO_dinput_mouse_and_keyb_acquire();
void pcINO_dinput_mouse_and_keyb_unacquire();
char* pcINO_GetFourKeyNamesCattedTogether(int a1, int a2, int a3, int a4);
void INO_SetControlSubmode_inner_sub_48A650(int a1);
void INO_SetControlMode_inner_sub_48ACB0(int a1);

int pcINO_SavModuleInit();
int pcINO_SavModuleClose();
int pcINO_b_AsyncInProgress();
int pcINO_i_TestPresence();
int pcINO_i_MountCard();
int pcINO_i_UnmountCard();
int pcINO_i_CheckCard();
int pcINO_i_FormatCard();
int pcINO_i_TestFreeSpace();
int pcINO_i_Retry();
int pcINO_i_Continue();
int pcINO_i_ActiveConsole();
int pcINO_i_Open();
int pcINO_i_Close();
int pcINO_i_ReadIndex_1();
int pcINO_i_WriteIndex();
int pcINO_i_ReadSlot_1();
int pcINO_i_ReadSlot_2();
int pcINO_i_ReadSlot_3();
int pcINO_i_WriteSlot_C();
int pcINO_i_WriteSlot_E();
int pcINO_i_CheckFreeSpace();
int pcINO_i_TestCard();

void DINPUT_InitVariables(int a1);
