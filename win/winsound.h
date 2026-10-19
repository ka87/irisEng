#pragma once
#include <dsound.h>
#include "eax/eax.h"
#include "../sound.h"
#include "../common.h"

typedef struct sDSoundGlobalStruct
{
    DSCAPS dsndCaps;
    int dword_B303C0;
    int dword_B303C4;
    int sndConcurrentlyPlayedNum;
    int sndConcurrentlyPlayedPeakNum;
    int pcSNDGlobalVol_dword_B303D0;
} tDSoundGlobalStruct;

typedef struct sPcSndEventRelated
{
    tSndSoundBufferElem* soundElem;
    int playListElemInUse;
} tPcSndEventRelated;

typedef struct sFXGlobalStruct
{
    int fxEnabled;
    byte gap1[8];
    int mode;
    byte gap[24];
} tFXGlobalStruct;



HMODULE snd_eaxDLLHandle;

int sndEventsNum;
tPcSndEventRelated* sndPlayList;
HANDLE* sndEventHandles;
HANDLE dsndEvents[23];
tSndSoundBufferElem* sndCurrSoundBufferElem;
int sndEventHandleNum;
void* dsndReadBuffer2;
void* dsndReadBuffer1;

typedef HRESULT (WINAPI * PEAXDirectSoundCreate8)(GUID*, LPDIRECTSOUND8*, IUnknown FAR*);

PEAXDirectSoundCreate8 pEAXDirectSoundCreate8;
tDSoundGlobalStruct gDoSoundGlobal;
int pcSNDThreadId;
HANDLE pcSNDhObject;
int trackEventsArrayNextIdx;
BASStruct dsndSBList;
IDirectSoundBuffer* d3dSndBuffer;
IDirectSound3DListener* dsndIDisectSound3DListenerIF;
IDirectSound3DBuffer* ppdsndDSND3DBuffer;
IKsPropertySet* ppdsndIKSPropertySet;
int pcSNDEAXLisenerPropertiesSetIsOk;
int pcSNDFXEnabled_dword_B30400;
float fxWetVol;
tFXGlobalStruct gFXGlobal;

int SND_i_SoftInit();