#pragma once
/* DirectSound's header and the EAX one are wanted by the frozen copy in
   tests/, which still calls through both, and by nothing in the game.
   The test build defines WINSOUND_FROZEN for every one of its
   translation units; the game build gets neither. */
#ifdef WINSOUND_FROZEN
#include <dsound.h>
#include "eax/eax.h"
#endif
#include "../sound.h"
#include "../common.h"

/* The room the fx layer works out, in the shape the reverb wanted it:
   levels in hundredths of a decibel, times and delays in seconds. The
   field names and widths are the ones the preset table was written
   against, so the computation reads the same either way. Nothing
   consumes it -- see pcSND_EAXListenerPropertiesSet -- but it is what
   a reverb would be handed, so it is kept whole. */
typedef struct sSndRoomVector
{
    float x;
    float y;
    float z;
} tSndRoomVector;

typedef struct sSndRoomProperties
{
    uint32_t ulEnvironment;
    float    flEnvironmentSize;
    float    flEnvironmentDiffusion;
    int32_t  lRoom;
    int32_t  lRoomHF;
    int32_t  lRoomLF;
    float    flDecayTime;
    float    flDecayHFRatio;
    float    flDecayLFRatio;
    int32_t  lReflections;
    float    flReflectionsDelay;
    tSndRoomVector vReflectionsPan;
    int32_t  lReverb;
    float    flReverbDelay;
    tSndRoomVector vReverbPan;
    float    flEchoTime;
    float    flEchoDepth;
    float    flModulationTime;
    float    flModulationDepth;
    float    flAirAbsorptionHF;
    float    flHFReference;
    float    flLFReference;
    float    flRoomRolloffFactor;
    uint32_t ulFlags;
} tSndRoomProperties;

typedef struct sDSoundGlobalStruct
{
    /* The device capabilities the old init read. Nothing ever read them
       back, so only the frozen copy has the field. */
#ifdef WINSOUND_FROZEN
    DSCAPS dsndCaps;
#endif
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
    uint8_t gap1[8];
    int mode;
    uint8_t gap[24];
} tFXGlobalStruct;



/* The EAX library the old init looked for. Nothing looks for it now. */
#ifdef WINSOUND_FROZEN
HMODULE snd_eaxDLLHandle;
#endif

int sndEventsNum;
tPcSndEventRelated* sndPlayList;
HANDLE* sndEventHandles;
HANDLE dsndEvents[23];
tSndSoundBufferElem* sndCurrSoundBufferElem;
int sndEventHandleNum;
void* dsndReadBuffer2;
void* dsndReadBuffer1;
/* Where a chunk is decoded before it is handed to the voice. The
   decode used to go straight into a locked stretch of the sound
   buffer; nothing hands out a pointer into the buffer now, so it goes
   through here instead. Big enough for the largest sizeToUpload the
   engine asks for, and the thread is the only user. */
uint8_t dsndUploadBuffer[65536];

/* The entry point the EAX library was asked for. */
#ifdef WINSOUND_FROZEN
typedef HRESULT (WINAPI * PEAXDirectSoundCreate8)(GUID*, LPDIRECTSOUND8*, IUnknown FAR*);

PEAXDirectSoundCreate8 pEAXDirectSoundCreate8;
#endif
tDSoundGlobalStruct gDoSoundGlobal;
int pcSNDThreadId;
HANDLE pcSNDhObject;
int trackEventsArrayNextIdx;
BASStruct dsndSBList;
/* The four interfaces the old init made to probe for 3D and for EAX.
   None of them is made or asked for now. */
#ifdef WINSOUND_FROZEN
IDirectSoundBuffer* d3dSndBuffer;
IDirectSound3DListener* dsndIDisectSound3DListenerIF;
IDirectSound3DBuffer* ppdsndDSND3DBuffer;
IKsPropertySet* ppdsndIKSPropertySet;
#endif
int pcSNDEAXLisenerPropertiesSetIsOk;
int pcSNDFXEnabled_dword_B30400;
float fxWetVol;
tFXGlobalStruct gFXGlobal;

int SND_i_SoftInit();