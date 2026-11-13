#pragma once
#include <dsound.h>
#include "common.h"
#include "basic_array.h"
#include "math.h"
#include "gameobject.h"

struct sModifier;
struct sLoadingSoundMdfStruct;

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
    uint8_t gap[4];
    int unknownFiledinsub49db40ToSetVolume; // set: pcSND_SB_SetPan
    IDirectSound3DBuffer* dsnd3dbufferIF;
    IKsPropertySet* dsndIKPropertySetIF;
    uint8_t gap1[4];
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
    void(* Close)(tSndHandles* handles);
    tSndSoundBufferElem* (* Create)(struct sSndHandles* handles, struct sSndWavHdrStruct* wavHdr, int offset);
    tSndSoundBufferElem* (* Reload)(struct sSndHandles* handles, struct sSndSoundBufferElem* sndBufferElem, struct sSndWavHdrStruct* wavHdr, int offset);
    int(* Play)(tSndSoundBufferElem* sndBufferElem, int flags);
    void(* Stop)(tSndSoundBufferElem* sndBufferElem);
    void(* Pause)(tSndSoundBufferElem* sndBufferElem);
    void(* Release)(tSndSoundBufferElem* sndBufferElem);
    void(* Duplicate)(tSndHandles* handles, tSndSoundBufferElem* src, tSndSoundBufferElem** dest);
    void(* SetCurrPos)(tSndSoundBufferElem* sndBufferElem, int pos);
    void(* Setvolume)(tSndSoundBufferElem* sndBufferElem, int att);
    void(* SetFrequency)(tSndSoundBufferElem* sndBufferElem, int freq);
    void(* SetPan)(tSndSoundBufferElem* sndBufferElem, int pan, int front);
    int(* GetStatus)(tSndSoundBufferElem* sndBufferElem, int *status);
    void(* GetFrequency)(tSndSoundBufferElem* sndBufferElem, int *freq);
    void(* GetPan)(tSndSoundBufferElem* sndBufferElem, int *pan, int *front);
    void(* GetCurrPos)(tSndSoundBufferElem* sndBufferElem, int* currPos, int* writePos);
    void(*CreateTrack)(tSndSoundBufferElem** stereo, tSndSoundBufferElem** mono);
    void(* ReleaseTrack)(tSndSoundBufferElem* stereo, tSndSoundBufferElem* mono);
    void(* StreamGetMonoToStereo)(tSndSoundBufferElem** x, tSndSoundBufferElem** xx);
    void(* StreamGetStereoToMono)();
    void(* StreamPrefetch)();
    void(* StreamPrefetchArray)(int* positions, int *sizes);
    void(* StreamFlush)();
    void(* StreamReinitAndPlay)(tSndSoundBufferElem* sndBufferElem, int len, int startPos, int from, int to, char flags, int freq, int pan, int att);
    void(* ShutdownStreams)();
    void(* nullptr1)();
    void(* SetGlobalVol)(int att);
    int(* GetGlobalVol)();
    void(* SetGlobalPan)(int pan);
    int(* GetGlobalPan)();
    void(* ComputeDoppler)(Vec3*, Vec3*, struct sSoundInstanceElem*);
    float(* Compute3DVol)(struct sSoundInstanceElem* inst);
    void(* SNDNull)(Vec3 *v);
    int(* ChangeRenderMode)(int mode);
    int(* FxInit)();
    void(* FxClose)();
    int(* FxSetMode)(struct sSoundFxParams* params, int mode);
    int(* FxGetMode)(struct sSoundFxParams* params);
    int(* FxSetDelay)(struct sSoundFxParams *params, int delay);
    int(* FxGetDelay)(struct sSoundFxParams* params);
    int(* FxSetFeedback)(struct sSoundFxParams* params, int feedback);
    int(* FxGetFeedback)(struct sSoundFxParams* params);
    int(* FxSetWetVolume)(struct sSoundFxParams* params, int att);
    int(* FxGetWetVolume)(struct sSoundFxParams* params);
    int(* FxSetWetPan)(struct sSoundFxParams* params, int pan);
    int(* FxGetWetPan)(struct sSoundFxParams* params);
    int(* FxEnable)(struct sSoundFxParams* params);
    int(* FxDisable)(struct sSoundFxParams* params);
} tSndInterfaceStruct;

typedef struct sSoundModifierElemIntf
{
    int Type;
    void (*Init)(struct sSndModifierElem* modifier, void* data, int dataSize);
    void (*InitChainedElemVersion)(struct sSndModifierElem* modifier, void *data, int dataSize);
    void (*Update)(struct sSndModifierElem*, struct sSoundInstanceElem*, int);
    void (*Destroy)(struct sSndModifierElem* modifier, int unk1, int unk2);
    void (*PlayApplyOnSound)(struct sSndModifierElem* modifier, void* linkedSound, int dataSize, struct sSndSoundElem* sound);
    void (*PlayApplyOnInstance)(struct sSndModifierElem* modifier, void *linkedsound, int dataSize, struct sSoundInstanceElem* inst);
    void (*NotifyPlay)(struct sSndModifierElem*, struct sSoundInstanceElem*);
    void (*NotifyPause)(struct sSndModifierElem*, struct sSoundInstanceElem*);
    void (*NotifyStop)(struct sSndModifierElem* modifier, struct sSoundInstanceElem* inst);
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
    void (*userCallBackFunction)(void* param);
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
    int16_t numChannels;
    int16_t bitsPerSample;
    struct sSndInterfaceStruct* sndInterface; // // TODO FIXME ez jó itt?
    struct sSndModifierElem* soundModifier;
    int startDataPos;
    unsigned int fullsoundLen;
    unsigned int byteRate;
    int16_t blockAlign;
    int16_t extraDataSize;
    int16_t format;
    uint8_t gap2b[2];
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
    uint8_t gap2[8];
} tSNDgstParams;

typedef struct sTrackListElem
{
    int flags;
    int trackID;
    tSndSoundBufferElem* sndBufferElem;
    tSndSoundBufferElem* trackResource;
    tSndInterfaceStruct* soundIntf;
    int16_t trackResourceNum;
    int16_t stereoToMono_sub499C20;
    int unk1;
    float f1;
    float f2;
    uint8_t gap[20];
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
    int16_t is1;
    int16_t is2;
    int ii1;
    int ii2;
    int16_t is3;
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
    int16_t is1;
    int16_t is2;
    int ii1;
    int ii2;
    int16_t is3;
    int16_t is4;
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
    int16_t is1;
    int16_t is2;
    int ii1;
    int ii2;
    int16_t is3;
    int16_t is4;
    int16_t is5;
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

int ConsoleSpecificOption(int option);

void SND_DeleteByIndex(int soundIdx);
void SND_FreeMainStruct(struct sGameObjectMain* gao);
void SND_Duplicate(struct sGameObjectMain* src, struct sGameObjectMain* dest);
void SND_ResolveRefsForMdF(struct sModifier* mdf);
void SND_ResolveRefs(struct sGameObjectMain* gao);
void SND_DetachObject(struct sGameObjectMain* gao);
void SND_DestroyInstanceOfGao(struct sGameObjectMain* gao);
void SND_RegisterInactiveGAO(struct sGameObjectMain* gao);
void SND_l_InitVolumeTable();
void SND_StreamStopAll();
void SND_NotifyNewLoading();
void SND_StreamResolveSyncPrefetch();
int SND_ResolveStaticPrefetchForSpecialMdF();
void SND_HeaderLoadArray(BASStruct* array, int binKey);
void SND_HeaderMakeArray(BASStruct* array);
void SND_LoadAllSounds();
unsigned int SND_UpdateLang();
int SND_l_AddStreamLoad(int soundKey);
int SND_i_GetWaveInfo(int Offset, tSndWavHdrStruct* wavHdrStructDest, int readMode);
tSndBank* SND_ul_CallbackLoadBank(int Offset);
int SND_l_AddSModifierNoLoad(int soundKey, char** cursor);
int SND_l_GetSoundIndex(int soundKEy);
int SND_i_AllocSound();
int SND_l_AddDialogNoLoad(int soundKey);
int SND_l_AddMusicNoLoad(int soundKey);
int SND_l_AddAmbienceNoLoad(int soundKey);
int SND_l_AddLoadingSound(int soundKey);
int SND_l_AddSound(int soundKey);
void SND_Update(Mtx4x4* mtx);
int SND_l_GetAttFromVol(float vol);
float SND_f_GetVolFromAtt(int att);
void SND_SetInstVolume(tSoundInstanceElem* inst);

void SND_StreamPrefetchFlushAll();
void SND_StreamFlushOneSound_0(int soundIdx, int doFlush);
void SND_FreeSndBank(struct sSndBank** bankPtr);
void SND_DelMdFBankUser(struct sSoundMdfStruct* sndMdf);
void SND_CloseModule();
void SND_PlayMdF(struct sSoundMdfStruct* sndMdf);
void SND_StreamPrefetchOneSound(int soundIdx, int mode);
void SND_TrackDeleteInstanceFromList(int id);
int SND_i_TrackNew();
int SND_l_Request(struct sGameObjectMain* gao, int soundIdx);
void SND_SetVol(int instanceIdx, float vol);
void SND_SetFlags(struct sGameObjectMain* gao, int idx, int flags);
int SND_i_CheckOrInitGAO(struct sGameObjectMain* gao);
void SND_SetPos(struct sGameObjectMain* gao, int instanceIdx, Mtx4x4* mtx);
void SND_SetFreq(int instanceIdx, int freq);
void SND_SetPan(int idx, int pan, int front);
void SND_SetNearFar(int idx, int type, float near1, float far1);
int SND_i_InstanceSetf(int instanceIdx, int type, float value);
void SND_LinkToFamilly(int mask, int instanceIdx);
int SND_i_IsTrackPlaying(unsigned int trackKey);
int SND_i_IsPlaying(int instanceIdx);
void SND_Stop(int instanceIdx);
void SND_StopAll(int mode);
void SND_SetTrack(int instanceIdx, unsigned int trackID);
void MDF_SndCreateInstance(struct sSoundMdfStruct* sndMdf, struct sGameObjectMain* gao);
int MDF_b_SndIsInActiveVolume(struct sSoundMdfStruct* sndMdf);
struct sSndBank* SND_ul_CallbackLoad(uint32_t Offset);
void SND_Release(int soundInstanceID);
void SND_LoadingSoundRegisterInstanceUserCallback(int index, void (*callback)(void*), struct sLoadingSoundMdfStruct* param);
int SND_l_GetLoadingInstance();
BOOL SND_b_IsLoadingSound(int soundIdx);
void SND_ReinitLoadingInstance();
void SND_AutoStop(struct sSoundInstanceElem* inst);
float SND_f_SphericComputing(Vec3* micPos, struct sSoundInstanceElem* inst, int* state);
void SND_AutoPlay(struct sSoundInstanceElem* inst);
float SND_f_AxisComputing(Vec3* micPos, struct sSoundInstanceElem* inst, int* state, int axis);
float SND_f_GetGlobalVol();
int SND_i_StreamAlloc();
void SND_PlaySoundLoopingOnTrack(int instanceIdx);
void SND_PlaySoundOnTrack(int instanceIdx);
void SND_PlaySoundLooping(int instanceIdx);
void SND_PlaySound(int instanceIdx);
void SND_InitModule();
void SND_SetGlobalPan(int pan);
int SND_l_GetGlobalPan();
void SND_SetGlobalVol(float vol);
float SND_f_GetGlobalVol();
int SND_l_GetPan(int instanceIdx);
int SND_l_GetFront(int instanceIdx);
int SND_l_GetFreq(int instanceIdx);
float SND_f_GetVol(int instanceIdx);
void SND_SetDoppler(int instanceIdx, int enabled);
void SND_MuteEnable(int group, int enabled);
int SND_i_MuteIsEnable(int group);
int SND_MuteSetUp(int group, float setUp2, float setUp1);
float SND_f_MuteGet(int group, int which);
void SND_StopRq(int instanceIdx);
void SND_Pause(int instanceIdx);
float SND_f_CurrentPosRatio(int instanceIdx);
int SND_i_GetTrack(int instanceIdx);
int SND_i_Seti(int type, int value);
int SND_i_Geti(int type);
int SND_l_GetSoundFromInstance(int instanceIdx);
int SND_i_Setf(int id, float val);
float SND_f_Getf(int type);
void SND_ResetFlags(struct sGameObjectMain* gao, int instanceIdx, int flags);
void SND_MergeBankOver(struct sGameObjectMain* src, struct sGameObjectMain* dest);
void SND_MergeBankNoOver(struct sGameObjectMain* src, struct sGameObjectMain* dest);
void SND_RestoreBank(struct sGameObjectMain* gao);
void SND_StreamPrefetch(int instanceIdx);
void SND_StreamFlush(int instanceIdx);
int SND_i_InstanceSeti_return_negative_1(int i1, int i2, int i3);
int SND_i_InstanceGeti(int instanceIdx, int type);
int SND_i_InstanceSetf(int instanceIdx, int type, float value);
float SND_f_InstanceGetf(int instanceIdx, int type);
void SND_Fade(int group, float from, float to, float duration);
void SND_PlaySoundOnTrackFromTo(int instanceIdx, float fromTime, float toTime);
void SND_PlaySoundLoopingOnTrackFrom(int instanceIdx, float fromTime);
float SND_SoundDurationGet(struct sGameObjectMain* gao, int sndIdx);

void SND_SModifierDeleteAll(tSndModifierElem** list);
void SND_SModifierDeleteOneSModifier(tSndModifierElem** list, int type);
void SND_SModifierDeleteAllLinkedSound(tSndModifierElem* list);
struct sSndModifierElem* SND_pst_SModifierInsertOneSModifier(struct sSndModifierElem** list, int type, void* linkedSound, int dataSize);
void SND_SModifierInstanciate(struct sSndModifierElem* list, struct sSoundInstanceElem* inst);
void SND_SModifierAddInInstance(int soundInstanceID, int sndModifierID, void* linkedSound, int dataSize);
struct sSndModifierElem* SND_pst_SModifierGet(struct sSndModifierElem* list, int type);
BOOL MDF_b_SndIsInPrefetchVolume(struct sSoundMdfStruct* sndMdf);
void SND_AddMdFBankUser(struct sSoundMdfStruct* sndMdf);
int SND_InitMdFModule();
void SND_SModifierInitModule();
void SND_PauseMdF(struct sSoundMdfStruct* sndMdf);
void SND_StopMdF(struct sSoundMdfStruct* sndMdf);
void SND_PrefetchMdF(struct sSoundMdfStruct* sndMdf);
void SND_FlushMdF(tSoundMdfStruct* sndMdf);
void SND_FreeHeaderArray(BASStruct* array);
void SND_MetaBankFreeList();
void SND_ResetAllMute();

void SND_PlayFamilly(int mask, int bits);
void SND_StopFamilly(int mask, int bits);
void SND_MuteOnFamilly(int mask, int x);
void SND_MuteOffFamilly(int mask, int bits);
void SND_SetfFamilly(int mask, int bits, int type, float value);
float SND_f_GetfFamilly(int mask, int bits, int type);
int SND_i_GetStatusFromFamilly(int mask, int bits);
void SND_FadeInFamilly(int mask, int bits);
void SND_FadeOutFamilly(int mask, int bits);
void SND_LinkToFamilly(int mask, int instanceIdx);
void SND_UnlinkFromFamilly(int mask, int instanceIdx);

int SND_i_TrackSeti(unsigned int trackKey, int type, int value);
int SND_i_TrackGeti(unsigned int trackKey, int type);
int SND_i_TrackSetf(int trackKey, int type, float value);
float SND_f_TrackGetf(unsigned int trackKey, int type);

int SND_i_FxSeti(int type, int value);
int SND_i_FxSetf(int type, float value);
float SND_f_FxGetf(int type);
int SND_i_FxGeti(int type);