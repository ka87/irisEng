#pragma once
#include <windows.h>

#define DI_OK                 0
#define DIERR_INPUTLOST       0x8007001E
#define DIERR_NOTACQUIRED     0x8007000C
#define DISCL_EXCLUSIVE       0x00000001
#define DISCL_NONEXCLUSIVE    0x00000002
#define DISCL_FOREGROUND      0x00000004
#define DISCL_BACKGROUND      0x00000008
#define DISCL_NOWINKEY        0x00000010
#define DIPH_DEVICE           0
#define DIPH_BYOFFSET         1
#define DIPH_BYID             2
#define DIPH_BYUSAGE          3
#define DIPROP_GRANULARITY    ((const GUID*)3)

typedef struct DIPROPHEADER { DWORD dwSize, dwHeaderSize, dwObj, dwHow; } DIPROPHEADER, *LPDIPROPHEADER;
typedef struct DIPROPDWORD { DIPROPHEADER diph; DWORD dwData; } DIPROPDWORD;

typedef struct DIJOYSTATE2 { LONG lX, lY, lZ; LONG rglSlider[2]; DWORD rgdwPOV[4]; uint8_t rgbButtons[128]; } DIJOYSTATE2;
typedef struct DIMOUSESTATE2 { LONG lX, lY, lZ; uint8_t rgbButtons[8]; } DIMOUSESTATE2;
typedef struct DIDEVICEINSTANCE { DWORD dwSize; CHAR tszProductName[MAX_PATH]; } DIDEVICEINSTANCE, *LPCDIDEVICEINSTANCE;
typedef struct DIDEVICEOBJECTINSTANCEA { DWORD dwSize; GUID guidType; DWORD dwOfs, dwType, dwFlags; CHAR tszName[MAX_PATH]; } DIDEVICEOBJECTINSTANCEA;
typedef struct DIDEVCAPS { DWORD dwSize, dwFlags, dwDevType, dwAxes, dwButtons, dwPOVs; } DIDEVCAPS;
typedef struct DIDATAFORMAT { DWORD dwSize; } DIDATAFORMAT;

typedef struct IDirectInputDevice8A IDirectInputDevice8A;
typedef struct IDirectInputDevice8AVtbl {
    HRESULT (*Release)(IDirectInputDevice8A*);
    HRESULT (*GetProperty)(IDirectInputDevice8A*, const GUID*, LPDIPROPHEADER);
    HRESULT (*Acquire)(IDirectInputDevice8A*);
    HRESULT (*Unacquire)(IDirectInputDevice8A*);
    HRESULT (*GetDeviceState)(IDirectInputDevice8A*, DWORD, void*);
    HRESULT (*SetDataFormat)(IDirectInputDevice8A*, const DIDATAFORMAT*);
    HRESULT (*SetCooperativeLevel)(IDirectInputDevice8A*, HWND, DWORD);
    HRESULT (*GetObjectInfo)(IDirectInputDevice8A*, DIDEVICEOBJECTINSTANCEA*, DWORD, DWORD);
    HRESULT (*GetCapabilities)(IDirectInputDevice8A*, DIDEVCAPS*);
} IDirectInputDevice8AVtbl;
struct IDirectInputDevice8A { IDirectInputDevice8AVtbl* lpVtbl; };

typedef struct IDirectInput8A IDirectInput8A;
typedef struct IDirectInput8AVtbl {
    HRESULT (*Release)(IDirectInput8A*);
    HRESULT (*CreateDevice)(IDirectInput8A*, const GUID*, IDirectInputDevice8A**, void*);
} IDirectInput8AVtbl;
struct IDirectInput8A { IDirectInput8AVtbl* lpVtbl; };

typedef IDirectInput8A IDirectInput8;
typedef IDirectInputDevice8A IDirectInputDevice8;
typedef IDirectInput8A* LPDIRECTINPUT8;
typedef IDirectInputDevice8A* LPDIRECTINPUTDEVICE8;

extern const GUID IID_IDirectInput8A;
extern const GUID GUID_SysMouse;
extern const GUID GUID_SysKeyboard;
extern const DIDATAFORMAT c_dfDIMouse2;
extern const DIDATAFORMAT c_dfDIKeyboard;
extern const DIDATAFORMAT c_dfDIJoystick2;

HRESULT DirectInput8Create(HINSTANCE, DWORD, const GUID*, void**, void*);
