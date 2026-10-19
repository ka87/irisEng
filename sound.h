#pragma once
#include <dsound.h>
#include "common.h"
#include "basic_array.h"
#include "math.h"
#include "gameobject.h"

typedef struct sSbeMiscData
{
    IDirectSoundNotify* vmilenEventHandlptr_Sub49e950;
    HANDLE fileHandle;
    int distanceToMove;
    unsigned int fullSize;
    int sumSize;
    int unk6;
    int soundBufferSize;
    int sizeToUpload;
    HANDLE eventHandle2;
    HANDLE eventHandle1;
    int currOffset;
    int currentPlayPosition;
    unsigned int currSize;
    int shouldBeRemovedFromPlayList;
    int readThroughWithMultipleLoop;
    int RemoveAfterPlayFinished;
    int Stopped_SetIN49e270;
    int dsndEventsArrayIdx;
} tSbeMiscData;


typedef struct sSndSoundBufferElem
{
    IDirectSoundBuffer* dsndSBIF;
    struct sSbeMiscData* miscData; // track-nál van ilyen elem
    int format;
    int unk2SetTo64InCreate; // panning, set: pcSND_SB_SetPan
    byte gap[4];
    int unknownFiledinsub49db40ToSetVolume; // set: pcSND_SB_SetPan
    IDirectSound3DBuffer* dsnd3dbufferIF;
    IKsPropertySet* dsndIKPropertySetIF;
    byte gap1[4];
}  tSndSoundBufferElem;

typedef struct sSndHandles
{
    HWND MainWindowHandle;
    IDirectSound8* dsndif;
    IDirectSoundBuffer* SoundBuffer;
    unsigned int soundElemFlagsOffset20isHas1;
    struct sSndSoundElem* soundElem;
} tSndHandles;

typedef struct sSndStreamList
{
    int num;
    int vol;
    int pan1;
    int pan2;
    int freq;
    int currPos;
} tSndStreamList;

typedef struct sLoadingSoundEffectStruct
{
    int mode;
    float f_fadeOutTime;
    float f_fadeInTime;
    float i4;
    float i5;
    float i6;
} tLoadingSoundEffectStruct;


typedef struct sSndInterfaceStruct
{
    char tag[4];
    int(* Init)(struct sSndHandles* sndh);
    void(* Close)(tSndHandles* a1);
    tSndSoundBufferElem* (* Create)(struct sSndHandles* a2, struct sSndWavHdrStruct* a3, int offset);
    tSndSoundBufferElem* (* Reload)(struct sSndHandles* a1, struct sSndSoundBufferElem* a2, struct sSndWavHdrStruct* a3, int offset);
    int(* Play)(tSndSoundBufferElem* a1, int flags);
    void(* Stop)(tSndSoundBufferElem* sndBufferElem);
    void(* Pause)(tSndSoundBufferElem* a1);
    void(* Release)(tSndSoundBufferElem* a1);
    void(* Duplicate)(tSndHandles* a1, tSndSoundBufferElem* a2, tSndSoundBufferElem** a3);
    void(* SetCurrPos)(tSndSoundBufferElem* a1, int a2);
    void(* Setvolume)(tSndSoundBufferElem* a1, int a2);
    void(* SetFrequency)(tSndSoundBufferElem* a1, int a2);
    void(* SetPan)(tSndSoundBufferElem* a1, int a2, int a3);
    int(* GetStatus)(tSndSoundBufferElem* sndBufferElem, int *status);
    void(* GetFrequency)(tSndSoundBufferElem* sndBufferElem, int *a2);
    void(* GetPan)(tSndSoundBufferElem* sndBufferElem, int *a2, int *a3);
    void(* GetCurrPos)(tSndSoundBufferElem* a1, int* a2, int* a3);
    void(*CreateTrack)(tSndSoundBufferElem** a1, tSndSoundBufferElem** a2);
    void(* ReleaseTrack)(tSndSoundBufferElem* a1, tSndSoundBufferElem* a2);
    void(* StreamGetMonoToStereo)(tSndSoundBufferElem** x, tSndSoundBufferElem** xx);
    void(* StreamGetStereoToMono)();
    void(* StreamPrefetch)();
    void(* StreamPrefetchArray)(int* a1, int *a2);
    void(* StreamFlush)();
    void(* StreamReinitAndPlay)(tSndSoundBufferElem* a1, int a2, int a3, int a4, int a5, char a6, int a7, int a8, int a9);
    void(* ShutdownStreams)();
    void(* nullptr1)();
    void(* SetGlobalVol)(int a1);
    int(* GetGlobalVol)();
    void(* SetGlobalPan)(int a1);
    int(* GetGlobalPan)();
    void(* ComputeDoppler)(Vec3*, Vec3*, struct sSoundInstanceElem*);
    float(* Compute3DVol)(struct sSoundInstanceElem* a1);
    void(* SNDNull)(Vec3 *v);
    int(* ChangeRenderMode)(int a1);
    int(* FxInit)();
    void(* FxClose)();
    int(* FxSetMode)(struct sSoundFxParams* a1, int a2);
    int(* FxGetMode)(struct sSoundFxParams* a1);
    int(* FxSetDelay)(struct sSoundFxParams *a1, int a2);
    int(* FxGetDelay)(struct sSoundFxParams* a1);
    int(* FxSetFeedback)(struct sSoundFxParams* a1, int a2);
    int(* FxGetFeedback)(struct sSoundFxParams* a1);
    int(* FxSetWetVolume)(struct sSoundFxParams* a1, int a2);
    int(* FxGetWetVolume)(struct sSoundFxParams* a1);
    int(* FxSetWetPan)(struct sSoundFxParams* a1, int a2);
    int(* FxGetWetPan)(struct sSoundFxParams* a1);
    int(* FxEnable)(struct sSoundFxParams* a1);
    int(* FxDisable)(struct sSoundFxParams* a1);
} tSndInterfaceStruct;

typedef struct sSoundModifierElemIntf
{
    int Type;
    void (*Init)(struct sSndModifierElem* a1, void* data, int dataSize);
    void (*InitChainedElemVersion)(struct sSndModifierElem* a1, void *data, int dataSize);
    void (*Update)(struct sSndModifierElem*, struct sSoundInstanceElem*, int);
    void (*Destroy)(struct sSndModifierElem* a1, int unk1, int unk2);
    void (*PlayApplyOnSound)(struct sSndModifierElem* a1, void* a2, int a3, struct sSndSoundElem* a4);
    void (*PlayApplyOnInstance)(struct sSndModifierElem* a1, void *linkedsound, int dataSize, struct sSoundInstanceElem* a2);
    void (*NotifyPlay)(struct sSndModifierElem*, struct sSoundInstanceElem*);
    void (*NotifyPause)(struct sSndModifierElem*, struct sSoundInstanceElem*);
    void (*NotifyStop)(struct sSndModifierElem* a1, struct sSoundInstanceElem* a2);
} tSoundModifierElemIntf;

typedef struct sSoundModifierDataGeneric {
    int sndKey;
    int flags;
    int sndIndexInAllocatedSounds;
    //int size; // ezt majd egy másik struktúrában
} tSoundModifierDataGeneric;

typedef struct sSndModifierElem
{
    struct sSoundModifierElemIntf* sndModifierModelIntf;
    struct sSndModifierElem* nextModifier;
    int linkedSoundDataSize;
    char* linkedSoundData; //struct sSoundModifierDataGeneric* 
} tSndModifierElem;

typedef struct sSoundModifierDataEmpty
{
    tSoundModifierDataGeneric gen;
} tSoundModifierDataEmpty;

typedef struct sSoundModifierDataFade
{
    tSoundModifierDataGeneric gen;
    float i1;
    float i2;
    float i3;
    float i4;
    int i5;
    float i6;
} tSoundModifierDataFade;

typedef struct sSoundModifierDataPlayer
{
    tSoundModifierDataGeneric gen;
    int freq;
    float vol;
    float fnear;
    float ffar;
    int flags;
    int i6;
    float timeRemainTillStart;
} tSoundModifierDataPlayer;

typedef struct sSoundModifierDataExtPlayer
{
    tSoundModifierDataGeneric gen;
    int i1;
    int idx;
    int i3;
    float vol;
    float vol2;
    float i6;
    float i7;
    int freq;
    int pan;
    float fnear;
    float fnear2;
    float fnear3;
    float ffar;
    float ffar2;
    float ffar3;
    float unkIsInActiveVolume;
    int familyID;
    int trackID;
    float interpolationFactor;
    float interpolationFactor2;
    float interpolationFactor3;
    float farCoefficient;
    float middleCoefficient;
    float nearfar;
    int i25;
    float timeRemainTillStart;
} tSoundModifierDataExtPlayer;

typedef struct sSoundModifierData3DSettings
{
    tSoundModifierDataGeneric gen;
    float i1;
    float i2;
    float i3;
    float farCoefficient;
    float middleCoefficient;
    float nearfar;
} tSoundModifierData3DSettings;

typedef struct sMutesSetupStruct
{
    int muteEnabled;
    float muteSetUp2;
    float muteSetUp1;
    int flags;
    float elapsedTime;
    float i6;
    float i7;
    float muteSetUp2ToSetWithThis;
} tMutesSetupStruct;

typedef struct sSoundInstanceElem
{
    int allocatedSoundsIdx;
    int flags;
    int muteFactorFlags;
    int familyBitmask;
    int trackID;
    int freq;
    float volume;
    int front;
    int pan;
    float minPan;
    float nearArr[3];
    float farArr[3];
    float dtFar;
    float centerArr[3];
    float farCoefficient;
    float middleCoefficient;
    Mtx4x4* sndPosMtxSetInSetPos;
    Vec3* sndPositionSetInSetPosPtr;
    Vec3 sndInstPosVec;
    Vec3 sndInstPosVecPrevious;
    struct sSndSoundBufferElem* soundBufElemFromModel;
    float requestTimeStamp;
    struct sGameObjectMain* gao;
    struct sSndInterfaceStruct* soundIntf;
    struct sMutesSetupStruct* unkMuteSetupSeeInitVolTable;
    struct sSndModifierElem* modifierInstances;
    float timeRemainTillStart;
    float timeFromDefault;
} tSoundInstanceElem;

typedef struct sLSGlobalStruct
{
    int isNewLoading;
    int soundInstanceIndex;
    int soundElemIndex;
    struct sSndSoundBufferElem* soundElemElem;
    void (*userCallBackFunction)(void* a1);
    void* userCallBackFunctionParam;
    int isSoundStillUsedElsewhere;
} tLSGlobalStruct;

typedef struct sSndSoundElem
{
    int soundKey;
    unsigned int flags;
    int muteFacorFlags;
    int usageCnt;
    struct sSndSoundBufferElem* sndSndBufElem; // // TODO FIXME ez jó típus itt?? (@SND_l_AddStreamLoad_inner_sub_496680, @SND_l_Request)
    int freq;
    __int16 numChannels;
    __int16 bitsPerSample;
    struct sSndInterfaceStruct* sndInterface; // // TODO FIXME ez jó itt?
    struct sSndModifierElem* soundModifier;
    int startDataPos;
    unsigned int fullsoundLen;
    unsigned int byteRate;
    __int16 blockAlign;
    __int16 extraDataSize;
    __int16 format;
    byte gap2b[2];
} tSndSoundElem;

typedef struct sSNDgstParams
{
    int flags;
    int soundEnabled;
    struct sSndHandles* handles;
    int allocatedSoundsMaxNum;
    struct sSndSoundElem* allocatedSounds;
    int soundInstanceNum;
    int soundInstanceMaxNum;
    struct sSoundInstanceElem* soundInstances;
    Mtx4x4* microPosMtxPtr;
    Mtx4x4* sndMatrixRef;
    Mtx4x4* microPosMtxPtrBackup;
    Mtx4x4* sndMatrixRefBackup;
    Mtx4x4 microPosMtx;
    Mtx4x4 sndMatrix2;
    float f_rolloff;
    float fx_doppler;
    float f_surround;
    float fx_DopplerSpeed;
    int soundRenderModeMask;
    int soundRenderMode;
    byte gap2[8];
} tSNDgstParams;

typedef struct sTrackListElem
{
    int flags;
    int trackID;
    tSndSoundBufferElem* sndBufferElem;
    tSndSoundBufferElem* trackResource;
    tSndInterfaceStruct* soundIntf;
    __int16 trackResourceNum;
    __int16 stereoToMono_sub499C20;
    int unk1;
    float f1;
    float f2;
    byte gap[20];
} tTrackListElem;

typedef struct sTrackResourceElem
{
    struct sSndSoundBufferElem* stereoSoundBuffer;
    struct sSndSoundBufferElem* monoSoundBuffer;
    int isBusy;
} tTrackResourceElem;

typedef struct sSndInterfaceFullStruct
{
    struct sSndInterfaceStruct win;
    struct sSndInterfaceStruct music;
    struct sSndInterfaceStruct dialog;
    struct sSndInterfaceStruct ambience;
    struct sSndInterfaceStruct track;
    struct sSndInterfaceStruct stream;
} tSndInterfaceFullStruct;

typedef struct sSndBank
{
    int usageCount;
    int elemNum;
    int bankKey;
    int* soundIndices;
    int* soundIndicesV2;
} tSndBank;

typedef struct sSndMetaBank
{
    tSndBank* bank;
    int offset;
} tSndMetaBank;

typedef struct sSndPrefetchElem
{
    int sndElemIdx;
    int type;
    int sndIntanceIdx;
    int sndInstanceIdx2;
} tSndPrefetchElem;

typedef struct sSndPrefetchManager
{
    tSndPrefetchElem* ptr;
    int num;
    int sumChannelNums;
} tSndPrefetchManager;

typedef struct sSndList
{
    int sndkey;
    int flags;
} tSndList;

#pragma pack(push, 1)
typedef struct sSoundHeaderDataCase0
{
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    __int16 is1;
    __int16 is2;
    int ii1;
    int ii2;
    __int16 is3;
    int ii3;
    int ii4;
} tSoundHeaderDataCase0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct sSoundHeaderDataCase1
{
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    __int16 is1;
    __int16 is2;
    int ii1;
    int ii2;
    __int16 is3;
    __int16 is4;
    int ii3;
    int ii4;
} tSoundHeaderDataCase1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct sSoundHeaderDataCase2
{
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    __int16 is1;
    __int16 is2;
    int ii1;
    int ii2;
    __int16 is3;
    __int16 is4;
    __int16 is5;
    int ii3;
    int ii4;
} tSoundHeaderDataCase2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct sSoundHeaderGeneric
{
    int soundKey;
    char data[46];
    int soundKey1;
} tSoundHeaderGeneric;
#pragma pack(pop)

/*
typedef struct sSndWavHdrStruct
{
    int32_t fullSoundSize;
    int32_t dataStartPos;
    __int16 format;
    __int16 numChannels;
    int32_t sampleRate;
    int32_t byteRate;
    __int16 blockAlign;
    __int16 bitsPerSample;
    __int16 extraDataSize;
} tSndWavHdrStruct;
*/
typedef struct sSndWavHdrStruct
{
    int32_t fullSoundSize;
    int32_t dataStartPos;
    WAVEFORMATEX wfex;
} tSndWavHdrStruct;

typedef struct sSndRasterStatStruct
{
    int i1;
    int i2;
    int i3;
    int i4;
    int i5;
    int i6;
    int i7;
    int i8;
} tSndRasterStatStruct;

typedef struct sSoundFxParams
{
    int flags;
    int mode;
    int delay;
    int feedback;
    float vol;
    int wetPan;
    int i7;
    struct sSndSoundBufferElem** i8;
} tSoundFxParams;

typedef struct sSoundFamilyInnerStruct
{
    int i1;
    tMutesSetupStruct muteSetup1;
    tMutesSetupStruct muteSetup2;
    float muteSetup1TimeMultiplier;
    float muteSetup2TimeMultiplier;
} tSoundFamilyInnerStruct;


typedef struct sSoundFamilyStruct
{
    int i1;
    int i2;
    int innerNum;
    int i4[32];
    struct sSoundFamilyInnerStruct** unkIntArrPtr;
} tSoundFamilyStruct;

typedef struct sLinkedSoundFade
{
    int sndKey;
    int flags;
    int sndIndexInAllocatedSounds;
    float minVolume;
    float maxVolume;
    float maxTimeToLast;
    float timeMultiplier;
    int fadeFlags;
    float elapsedTime;
} tLinkedSoundFade;

typedef struct sSoundMdfStruct
{
    int i1;
    int flags;
    int modifierID;
    int i4;
    float volume;
    float volume2;
    float ii2;
    float distPrefetchVolume;
    float ii4;
    int ii5;
    int freq2;
    int pan;
    int ii8;
    int ii9;
    int flags2;
    int ii11;
    int idx;
    int sndInstanceID;
    float near1;
    float near2;
    float near3;
    float far1;
    float far2;
    float far3;
    float unkIsInActiveVolume;
    int trackID;
    float nearFarFactor;
    float nearFar2factor;
    float nearFar3factor;
    float xx;
    float f4;
    float b0;
    int soundIndex;
    tSndBank* soundBank;
    float timeRemainTillStart;
    int b3;
    int b4;
    float freq;
    char gap[38];
} tSoundMdfStruct;

tSNDgstParams SND_gst_Params;
BOOL SND_gc_NoSound;
tLSGlobalStruct SND_gst_LSGlobal;
tTrackResourceElem SND_gast_TrackResources[4];
tTrackListElem* SND_gax_TrackPlayingList;
tSndInterfaceFullStruct SND_gpst_Interface;
BASStruct SND_gst_TrackId;
BASStruct SND_gst_TrackLookUpTable;
int SND_gi_TrackNb;
unsigned int SND_gul_MetaBankUserNb;
tSndMetaBank* SND_gax_MetaBankUserList;
tMutesSetupStruct SND_Mutes_B30600_fadergroups[12];
tSndPrefetchManager SND_gst_PrefetchManager;
int SND_gb_AllSoundsLoaded;
int SND_gb_UnloadedFile;
BASStruct SND_gst_HeaderArray;
int SND_gul_WavBinSeek;
int SND_gi_RasterCurrentFileId;
tSndStreamList* SND_gast_StreamList;
int SND_gi_StreamListSize;
tSndModifierElem SND_gs_SoundModifiers[7];
int SND_gi_TrackLastVirtualId;
tLoadingSoundEffectStruct SND_gst_LoadingEffect;
struct sGameObjectMain* SND_gpst_RqMicroPosFromAI;
Vec3 t_last_mic_pos_522;

int SND_AllocatedSoundsNumWithhoutFlag80000000;
int SND_AllocatedSoundsNumWithFlag80000000;
int SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage;
int SND_AllocatedSoundsNumWithFlag80000000PeakUsage;

extern float SND_gf_AutoVolumeOff;
float SND_af_VolTable[256];
int32_t SND_al_AttTable[256];

BASStruct rasterbas1;
BASStruct rasterbas2;
tSndRasterStatStruct sndRasterStat;

tSoundFxParams SND_gst_FxDefaultParam;
tSoundFxParams SND_gst_FxParam;

tSoundFamilyStruct* SND_gast_AndFamillyList;
int SND_gi_AndFamillyNb;
int SND_gi_AndFamillyListSize;

int ConsoleSpecificOption(int a1);

void SND_DeleteByIndex(int a1);
void SND_FreeMainStruct(struct sGameObjectMain* a1);
void SND_Duplicate(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void SND_ResolveRefsForMdF(struct sModifier* a1);
void SND_ResolveRefs(struct sGameObjectMain* a1);
void SND_DetachObject(struct sGameObjectMain* a1);
void SND_DestroyInstanceOfGao(struct sGameObjectMain* a1);
void SND_RegisterInactiveGAO(struct sGameObjectMain* a1);
void SND_l_InitVolumeTable();
void SND_StreamStopAll();
void SND_NotifyNewLoading();
void SND_StreamResolveSyncPrefetch();
int SND_ResolveStaticPrefetchForSpecialMdF();
void SND_HeaderLoadArray(BASStruct* a1, int a2);
void SND_HeaderMakeArray(BASStruct* a1);
void SND_LoadAllSounds();
unsigned int SND_UpdateLang();
int SND_l_AddStreamLoad(int a1);
int SND_i_GetWaveInfo(int Offset, tSndWavHdrStruct* wavHdrStructDest, int readMode);
tSndBank* SND_ul_CallbackLoadBank(int Offset);
int SND_l_AddSModifierNoLoad(int a1, char** a2);
int SND_l_GetSoundIndex(int soundKEy);
int SND_i_AllocSound();
int SND_l_AddDialogNoLoad(int a1);
int SND_l_AddMusicNoLoad(int a1);
int SND_l_AddAmbienceNoLoad(int a1);
int SND_l_AddLoadingSound(int a1);
int SND_l_AddSound(int soundKey);
void SND_Update(Mtx4x4* mtx);
int SND_l_GetAttFromVol(float a1);
float SND_f_GetVolFromAtt(int a1);
void SND_SetInstVolume(tSoundInstanceElem* a1);

void SND_StreamPrefetchFlushAll();
void SND_StreamFlushOneSound_0(int a1, int a2);
void SND_FreeSndBank(struct sSndBank** a1);
void SND_DelMdFBankUser(struct sSoundMdfStruct* a1);
void SND_CloseModule();
void SND_PlayMdF(struct sSoundMdfStruct* a1);
void SND_StreamPrefetchOneSound(int a1, int a2);
void SND_TrackDeleteInstanceFromList(int id);
int SND_i_TrackNew();
int SND_l_Request(struct sGameObjectMain* gao, int a2);
void SND_SetVol(int a1, float a2);
void SND_SetFlags(struct sGameObjectMain* a1, int idx, int a3);
int SND_i_CheckOrInitGAO(struct sGameObjectMain* a1);
void SND_SetPos(struct sGameObjectMain* a1, int a2, Mtx4x4* a3);
void SND_SetFreq(int a1, int a2);
void SND_SetPan(int a1, int a2, int a3);
void SND_SetNearFar(int idx, int type, float near1, float far1);
int SND_i_InstanceSetf(int a1, int a2, float a3);
void SND_LinkToFamilly(int a1, int a2);
int SND_i_IsTrackPlaying(unsigned int a1);
int SND_i_IsPlaying(int a1);
void SND_Stop(int a1);
void SND_StopAll(int a1);
void SND_SetTrack(int a1, unsigned int a2);
void MDF_SndCreateInstance(struct sSoundMdfStruct* a1, struct sGameObjectMain* a2);
int MDF_b_SndIsInActiveVolume(struct sSoundMdfStruct* a1);
struct sSndBank* SND_ul_CallbackLoad(uint32_t Offset);
void SND_Release(int soundInstanceID);
void SND_LoadingSoundRegisterInstanceUserCallback(int index, void (*a2)(void*), struct sLoadingSoundMdfStruct* a3);
int SND_l_GetLoadingInstance();
BOOL SND_b_IsLoadingSound(int a1);
void SND_ReinitLoadingInstance();
void SND_AutoStop(struct sSoundInstanceElem* a1);
float SND_f_SphericComputing(Vec3* a1, struct sSoundInstanceElem* a2, int* a3);
void SND_AutoPlay(struct sSoundInstanceElem* a1);
float SND_f_AxisComputing(Vec3* a1, struct sSoundInstanceElem* a2, int* a3, int a4);
float SND_f_GetGlobalVol();
int SND_i_StreamAlloc();
void SND_PlaySoundLoopingOnTrack(int a1);
void SND_PlaySoundOnTrack(int a1);
void SND_PlaySoundLooping(int a1);
void SND_PlaySound(int a1);
void SND_InitModule();
void SND_SetGlobalPan(int a1);
int SND_l_GetGlobalPan();
void SND_SetGlobalVol(float a1);
float SND_f_GetGlobalVol();
int SND_l_GetPan(int a1);
int SND_l_GetFront(int a1);
int SND_l_GetFreq(int a1);
float SND_f_GetVol(int a1);
void SND_SetDoppler(int a1, int a2);
void SND_MuteEnable(int a1, int a2);
int SND_i_MuteIsEnable(int a1);
int SND_MuteSetUp(int a1, float a2, float a3);
float SND_f_MuteGet(int a1, int a2);
void SND_StopRq(int a1);
void SND_Pause(int a1);
float SND_f_CurrentPosRatio(int a1);
int SND_i_GetTrack(int a1);
int SND_i_Seti(int a1, int a2);
int SND_i_Geti(int a1);
int SND_l_GetSoundFromInstance(int a1);
int SND_i_Setf(int a1, float a2);
float SND_f_Getf(int a1);
void SND_ResetFlags(struct sGameObjectMain* a1, int a2, int a3);
void SND_MergeBankOver(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void SND_MergeBankNoOver(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void SND_RestoreBank(struct sGameObjectMain* a1);
void SND_StreamPrefetch(int a1);
void SND_StreamFlush(int instanceIdx);
int SND_i_InstanceSeti_return_negative_1(int i1, int i2, int i3);
int SND_i_InstanceGeti(int a1, int a2);
int SND_i_InstanceSetf(int a1, int a2, float a3);
float SND_f_InstanceGetf(int a1, int a2);
void SND_Fade(int a1, float a2, float a3, float a4);
void SND_PlaySoundOnTrackFromTo(int a1, float a2, float a3);
void SND_PlaySoundLoopingOnTrackFrom(int a1, float a2);
float SND_SoundDurationGet(struct sGameObjectMain* a1, int sndIdx);

void SND_SModifierDeleteAll(tSndModifierElem** a1);
void SND_SModifierDeleteOneSModifier(tSndModifierElem** a1, int type);
void SND_SModifierDeleteAllLinkedSound(tSndModifierElem* a1);
struct sSndModifierElem* SND_pst_SModifierInsertOneSModifier(struct sSndModifierElem** a1, int type, void* linkedSound, int a4);
void SND_SModifierInstanciate(struct sSndModifierElem* a1, struct sSoundInstanceElem* a2);
void SND_SModifierAddInInstance(int soundInstanceID, int sndModifierID, void* a3, int a4);
struct sSndModifierElem* SND_pst_SModifierGet(struct sSndModifierElem* a1, int type);
BOOL MDF_b_SndIsInPrefetchVolume(struct sSoundMdfStruct* a1);
void SND_AddMdFBankUser(struct sSoundMdfStruct* a1);
int SND_InitMdFModule();
void SND_SModifierInitModule();
void SND_PauseMdF(struct sSoundMdfStruct* a1);
void SND_StopMdF(struct sSoundMdfStruct* a1);
void SND_PrefetchMdF(struct sSoundMdfStruct* a1);
void SND_FlushMdF(tSoundMdfStruct* a1);
void SND_FreeHeaderArray(BASStruct* a1);
void SND_MetaBankFreeList();
void SND_ResetAllMute();

void SND_PlayFamilly(int a1, int a2);
void SND_StopFamilly(int a1, int a2);
void SND_MuteOnFamilly(int a2, int x);
void SND_MuteOffFamilly(int a1, int a2);
void SND_SetfFamilly(int a1, int a2, int a3, float a4);
float SND_f_GetfFamilly(int a1, int a2, int a3);
int SND_i_GetStatusFromFamilly(int a1, int a2);
void SND_FadeInFamilly(int a1, int a2);
void SND_FadeOutFamilly(int a1, int a2);
void SND_LinkToFamilly(int a1, int a2);
void SND_UnlinkFromFamilly(int a1, int a2);

int SND_i_TrackSeti(unsigned int a1, int a2, int a3);
int SND_i_TrackGeti(unsigned int a1, int a2);
int SND_i_TrackSetf(int a1, int a2, float a3);
float SND_f_TrackGetf(unsigned int a1, int a2);

int SND_i_FxSeti(int a1, int a2);
int SND_i_FxSetf(int a1, float a2);
float SND_f_FxGetf(int a1);
int SND_i_FxGeti(int a1);