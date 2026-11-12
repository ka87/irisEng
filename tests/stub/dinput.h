#pragma once
#include <windows.h>
typedef struct IDirectInput8 IDirectInput8;
typedef struct IDirectInputDevice8 IDirectInputDevice8;
typedef IDirectInput8* LPDIRECTINPUT8;
typedef IDirectInputDevice8* LPDIRECTINPUTDEVICE8;
typedef struct DIJOYSTATE2 { LONG lX, lY, lZ; LONG rglSlider[2]; DWORD rgdwPOV[4]; uint8_t rgbButtons[128]; } DIJOYSTATE2;
typedef struct DIDEVICEINSTANCE { DWORD dwSize; CHAR tszProductName[MAX_PATH]; } DIDEVICEINSTANCE, *LPCDIDEVICEINSTANCE;
typedef struct DIDEVCAPS { DWORD dwSize, dwFlags, dwDevType, dwAxes, dwButtons, dwPOVs; } DIDEVCAPS;
