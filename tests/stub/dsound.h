#pragma once
#include <windows.h>

#define S_OK                 0
#define S_FALSE              1
#define DS_OK                0
#define DSERR_BUFFERLOST     0x88780096
#define FAR
#define NEAR
#define PASCAL
#define CDECL
#define STDAPI               HRESULT
#define DSSCL_NORMAL         1
#define DSSCL_PRIORITY       2
#define DSSCL_EXCLUSIVE      3
#define DSSCL_WRITEPRIMARY   4
#define DSBPLAY_LOOPING      0x00000001

/* from mmreg.h, which the real windows.h pulls in */
#define WAVE_FORMAT_PCM      1
#define DSBSTATUS_PLAYING    0x00000001
#define DSBSTATUS_BUFFERLOST 0x00000002
#define DSBSTATUS_LOOPING    0x00000004
#define DSBCAPS_PRIMARYBUFFER     0x00000001
#define DSBCAPS_STATIC            0x00000002
#define DSBCAPS_LOCHARDWARE       0x00000004
#define DSBCAPS_LOCSOFTWARE       0x00000008
#define DSBCAPS_CTRL3D            0x00000010
#define DSBCAPS_CTRLFREQUENCY     0x00000020
#define DSBCAPS_CTRLPAN           0x00000040
#define DSBCAPS_CTRLVOLUME        0x00000080
#define DSBCAPS_CTRLPOSITIONNOTIFY 0x00000100
#define DSBCAPS_STICKYFOCUS       0x00004000
#define DSBCAPS_GLOBALFOCUS       0x00008000
#define DSBCAPS_GETCURRENTPOSITION2 0x00010000
#define DSBVOLUME_MIN        (-10000)
#define DSBVOLUME_MAX        0
#define DSBPAN_LEFT          (-10000)
#define DSBPAN_CENTER        0
#define DSBPAN_RIGHT         10000
#define DSBFREQUENCY_MIN     100
#define DSBFREQUENCY_MAX     200000
#define DSBPN_OFFSETSTOP     0xFFFFFFFF
#define KSPROPERTY_SUPPORT_GET 1
#define KSPROPERTY_SUPPORT_SET 2

typedef struct IUnknown IUnknown;
typedef struct IUnknownVtbl {
    HRESULT (*QueryInterface)(IUnknown*, const GUID*, void**);
    DWORD   (*AddRef)(IUnknown*);
    DWORD   (*Release)(IUnknown*);
} IUnknownVtbl;
struct IUnknown { IUnknownVtbl* lpVtbl; };

typedef struct WAVEFORMATEX {
    WORD  wFormatTag;
    WORD  nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
    WORD  cbSize;
} WAVEFORMATEX, *LPWAVEFORMATEX;

typedef struct DSCAPS {
    DWORD dwSize, dwFlags, dwMinSecondarySampleRate, dwMaxSecondarySampleRate;
    DWORD dwPrimaryBuffers;
    DWORD dwMaxHwMixingAllBuffers, dwMaxHwMixingStaticBuffers, dwMaxHwMixingStreamingBuffers;
    DWORD dwFreeHwMixingAllBuffers, dwFreeHwMixingStaticBuffers, dwFreeHwMixingStreamingBuffers;
    DWORD dwMaxHw3DAllBuffers, dwMaxHw3DStaticBuffers, dwMaxHw3DStreamingBuffers;
    DWORD dwFreeHw3DAllBuffers, dwFreeHw3DStaticBuffers, dwFreeHw3DStreamingBuffers;
    DWORD dwTotalHwMemBytes, dwFreeHwMemBytes, dwMaxContigFreeHwMemBytes;
    DWORD dwUnlockTransferRateHwBuffers, dwPlayCpuOverheadSwBuffers;
    DWORD dwReserved1, dwReserved2;
} DSCAPS;

typedef struct DSBCAPS { DWORD dwSize, dwFlags, dwBufferBytes, dwUnlockTransferRate, dwPlayCpuOverhead; } DSBCAPS;
typedef struct DSBUFFERDESC { DWORD dwSize, dwFlags, dwBufferBytes, dwReserved; LPWAVEFORMATEX lpwfxFormat; GUID guid3DAlgorithm; } DSBUFFERDESC;
typedef struct DSBPOSITIONNOTIFY { DWORD dwOffset; HANDLE hEventNotify; } DSBPOSITIONNOTIFY, *LPDSBPOSITIONNOTIFY;
typedef struct DS3DBUFFER { DWORD dwSize; } DS3DBUFFER;
typedef struct DS3DLISTENER { DWORD dwSize; } DS3DLISTENER;

typedef struct IDirectSoundBuffer IDirectSoundBuffer;
typedef struct IDirectSoundBufferVtbl {
    HRESULT (*QueryInterface)(IDirectSoundBuffer*, const GUID*, void**);
    DWORD   (*AddRef)(IDirectSoundBuffer*);
    DWORD   (*Release)(IDirectSoundBuffer*);
    HRESULT (*GetCaps)(IDirectSoundBuffer*, DSBCAPS*);
    HRESULT (*GetCurrentPosition)(IDirectSoundBuffer*, DWORD*, DWORD*);
    HRESULT (*GetFormat)(IDirectSoundBuffer*, LPWAVEFORMATEX, DWORD, DWORD*);
    HRESULT (*GetVolume)(IDirectSoundBuffer*, LONG*);
    HRESULT (*GetPan)(IDirectSoundBuffer*, LONG*);
    HRESULT (*GetFrequency)(IDirectSoundBuffer*, DWORD*);
    HRESULT (*GetStatus)(IDirectSoundBuffer*, DWORD*);
    HRESULT (*Initialize)(IDirectSoundBuffer*, void*, const DSBUFFERDESC*);
    HRESULT (*Lock)(IDirectSoundBuffer*, DWORD, DWORD, void**, DWORD*, void**, DWORD*, DWORD);
    HRESULT (*Play)(IDirectSoundBuffer*, DWORD, DWORD, DWORD);
    HRESULT (*SetCurrentPosition)(IDirectSoundBuffer*, DWORD);
    HRESULT (*SetFormat)(IDirectSoundBuffer*, const WAVEFORMATEX*);
    HRESULT (*SetVolume)(IDirectSoundBuffer*, LONG);
    HRESULT (*SetPan)(IDirectSoundBuffer*, LONG);
    HRESULT (*SetFrequency)(IDirectSoundBuffer*, DWORD);
    HRESULT (*Stop)(IDirectSoundBuffer*);
    HRESULT (*Unlock)(IDirectSoundBuffer*, void*, DWORD, void*, DWORD);
    HRESULT (*Restore)(IDirectSoundBuffer*);
} IDirectSoundBufferVtbl;
struct IDirectSoundBuffer { IDirectSoundBufferVtbl* lpVtbl; };
typedef IDirectSoundBuffer IDirectSoundBuffer8;

typedef struct IDirectSound IDirectSound;
typedef struct IDirectSoundVtbl {
    HRESULT (*QueryInterface)(IDirectSound*, const GUID*, void**);
    DWORD   (*AddRef)(IDirectSound*);
    DWORD   (*Release)(IDirectSound*);
    HRESULT (*CreateSoundBuffer)(IDirectSound*, const DSBUFFERDESC*, IDirectSoundBuffer**, IUnknown*);
    HRESULT (*GetCaps)(IDirectSound*, DSCAPS*);
    HRESULT (*DuplicateSoundBuffer)(IDirectSound*, IDirectSoundBuffer*, IDirectSoundBuffer**);
    HRESULT (*SetCooperativeLevel)(IDirectSound*, HWND, DWORD);
    HRESULT (*Compact)(IDirectSound*);
    HRESULT (*GetSpeakerConfig)(IDirectSound*, DWORD*);
    HRESULT (*SetSpeakerConfig)(IDirectSound*, DWORD);
    HRESULT (*Initialize)(IDirectSound*, const GUID*);
} IDirectSoundVtbl;
struct IDirectSound { IDirectSoundVtbl* lpVtbl; };
typedef IDirectSound IDirectSound8;

typedef struct IDirectSound3DBuffer IDirectSound3DBuffer;
typedef struct IDirectSound3DBufferVtbl {
    HRESULT (*QueryInterface)(IDirectSound3DBuffer*, const GUID*, void**);
    DWORD   (*AddRef)(IDirectSound3DBuffer*);
    DWORD   (*Release)(IDirectSound3DBuffer*);
    HRESULT (*GetAllParameters)(IDirectSound3DBuffer*, DS3DBUFFER*);
    HRESULT (*SetMaxDistance)(IDirectSound3DBuffer*, float, DWORD);
    HRESULT (*SetMinDistance)(IDirectSound3DBuffer*, float, DWORD);
    HRESULT (*SetMode)(IDirectSound3DBuffer*, DWORD, DWORD);
    HRESULT (*SetPosition)(IDirectSound3DBuffer*, float, float, float, DWORD);
    HRESULT (*SetVelocity)(IDirectSound3DBuffer*, float, float, float, DWORD);
} IDirectSound3DBufferVtbl;
struct IDirectSound3DBuffer { IDirectSound3DBufferVtbl* lpVtbl; };
typedef IDirectSound3DBuffer IDirectSound3DBuffer8;

typedef struct IDirectSound3DListener IDirectSound3DListener;
typedef struct IDirectSound3DListenerVtbl {
    HRESULT (*QueryInterface)(IDirectSound3DListener*, const GUID*, void**);
    DWORD   (*AddRef)(IDirectSound3DListener*);
    DWORD   (*Release)(IDirectSound3DListener*);
    HRESULT (*GetAllParameters)(IDirectSound3DListener*, DS3DLISTENER*);
    HRESULT (*SetAllParameters)(IDirectSound3DListener*, const DS3DLISTENER*, DWORD);
    HRESULT (*SetOrientation)(IDirectSound3DListener*, float, float, float, float, float, float, DWORD);
    HRESULT (*SetPosition)(IDirectSound3DListener*, float, float, float, DWORD);
    HRESULT (*SetVelocity)(IDirectSound3DListener*, float, float, float, DWORD);
    HRESULT (*CommitDeferredSettings)(IDirectSound3DListener*);
} IDirectSound3DListenerVtbl;
struct IDirectSound3DListener { IDirectSound3DListenerVtbl* lpVtbl; };
typedef IDirectSound3DListener IDirectSound3DListener8;

typedef struct IDirectSoundNotify IDirectSoundNotify;
typedef struct IDirectSoundNotifyVtbl {
    HRESULT (*QueryInterface)(IDirectSoundNotify*, const GUID*, void**);
    DWORD   (*AddRef)(IDirectSoundNotify*);
    DWORD   (*Release)(IDirectSoundNotify*);
    HRESULT (*SetNotificationPositions)(IDirectSoundNotify*, DWORD, LPDSBPOSITIONNOTIFY);
} IDirectSoundNotifyVtbl;
struct IDirectSoundNotify { IDirectSoundNotifyVtbl* lpVtbl; };

typedef struct IKsPropertySet IKsPropertySet;
typedef struct IKsPropertySetVtbl {
    HRESULT (*QueryInterface)(IKsPropertySet*, const GUID*, void**);
    DWORD   (*AddRef)(IKsPropertySet*);
    DWORD   (*Release)(IKsPropertySet*);
    HRESULT (*Get)(IKsPropertySet*, const GUID*, DWORD, void*, DWORD, void*, DWORD, DWORD*);
    HRESULT (*Set)(IKsPropertySet*, const GUID*, DWORD, void*, DWORD, void*, DWORD);
    HRESULT (*QuerySupport)(IKsPropertySet*, const GUID*, DWORD, DWORD*);
} IKsPropertySetVtbl;
struct IKsPropertySet { IKsPropertySetVtbl* lpVtbl; };

typedef IDirectSound*             LPDIRECTSOUND;
typedef IDirectSound8*            LPDIRECTSOUND8;
typedef IDirectSoundBuffer*       LPDIRECTSOUNDBUFFER;
typedef IDirectSoundBuffer8*      LPDIRECTSOUNDBUFFER8;
typedef IDirectSound3DBuffer*     LPDIRECTSOUND3DBUFFER;
typedef IDirectSound3DListener*   LPDIRECTSOUND3DLISTENER;
typedef IDirectSoundNotify*       LPDIRECTSOUNDNOTIFY;
typedef IKsPropertySet*           LPKSPROPERTYSET;

extern const GUID IID_IDirectSound3DBuffer;
extern const GUID IID_IDirectSound3DListener;
extern const GUID IID_IDirectSoundNotify;
extern const GUID IID_IKsPropertySet;
extern const GUID DSBCAPS_CTRL3D_GUID;
extern const GUID DS3DALG_DEFAULT;

HRESULT DirectSoundCreate(const GUID*, LPDIRECTSOUND*, IUnknown*);
HRESULT DirectSoundCreate8(const GUID*, LPDIRECTSOUND8*, IUnknown*);
