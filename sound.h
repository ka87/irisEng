#pragma once
/* DirectSound's header is wanted by the frozen copies in tests/,
   which still call it, and by nothing in the game. The test build
   defines WINSOUND_FROZEN for every one of its translation units and
   gets the four interface fields marked below; the game build gets
   pointers of the same size in their place, so that both see one
   layout for a struct they hand to each other. */
#ifdef WINSOUND_FROZEN
#include <dsound.h>
#endif
#include "common.h"
#include "basic_array.h"
#include "math.h"
#include "gameobject.h"
#include "handle.h"

/* Named below in a parameter list and nowhere else, so without this
   each prototype would declare a type of its own, scoped to itself:
   two functions taking "the same" pointer would not agree, and
   assigning one to the other is a type error rather than the
   intended opaque handle. */
struct sSndModifierElem;
struct sSndWavHdrStruct;
struct sSoundFxParams;
struct sSoundInstanceElem;

struct sModifier;
struct sLoadingSoundMdfStruct;

typedef struct sSbeMiscData
{
    /* The notify interface the refill positions were armed on. The
       rewrite arms nothing and only ever clears this; the frozen copy
       queries it and calls it, so it keeps the type. */
#ifdef WINSOUND_FROZEN
    IDirectSoundNotify* vmilenEventHandlptr_Sub49e950;
#else
    void* vmilenEventHandlptr_Sub49e950;
#endif
    /* The big file this sound streams out of. A Windows handle for
       the frozen copy and the platform layer's own for the rewrite;
       both are a pointer, so the struct the two hand each other is
       the same shape either way. */
#ifdef WINSOUND_FROZEN
    HANDLE fileHandle;
#else
    struct sSplFile* fileHandle;
#endif
    int distanceToMove;
    unsigned int fullSize;
    int sumSize;
    int startDecodedOffset;
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
    /* Which block of the buffer the play cursor was last seen in.
       The device used to say when the cursor entered each block, by
       firing an event set at the block's own offset; nothing says so
       now, so the thread looks and this is what it compares against.
       One refill per block, which is what the four events gave. */
    int lastPlayedBlock;
} tSbeMiscData;


/* One compressed block, per channel. It is a property of the sample
   format and nothing else; it happens to match the size of the
   structure below in a 32-bit build, which is where that size used
   to be read from. */
#define SND_ADPCM_BLOCK_BYTES 36

typedef struct sSndSoundBufferElem
{
    /* The sound itself: the buffer and the cursor over it. Three
       interfaces used to live here -- the buffer, a 3D buffer parked
       at a fixed position, and the property set the reverb went
       through. sndplatform.h says what replaced the first; the other
       two have no successor. */
    struct sSplVoice* voice;
    struct sSbeMiscData* miscData; // track-nál van ilyen elem
    int format;
    int unk2SetTo64InCreate; // panning, set: pcSND_SB_SetPan
    uint8_t gap[4];
    int unknownFiledinsub49db40ToSetVolume; // set: pcSND_SB_SetPan
    /* Three interfaces the engine no longer uses: the buffer, the 3D
       buffer parked at a fixed position, and the property set the
       reverb went through. The voice above replaced the first and the
       other two have no successor -- but the frozen copy in tests/
       still uses all three, and a baseline is not edited to follow a
       rewrite. Both copies have to agree on this struct's shape, so
       the space stays until the frozen copy is retired. */
#ifdef WINSOUND_FROZEN
    IDirectSoundBuffer* dsndSBIF;
    IDirectSound3DBuffer* dsnd3dbufferIF;
    IKsPropertySet* dsndIKPropertySetIF;
#else
    void* frozenOnly[3];
#endif
    uint8_t gap1[4];
}  tSndSoundBufferElem;

typedef struct sSndHandles
{
    HWND MainWindowHandle;
    /* Whether the device is open. It used to be the DirectSound object
       and the primary buffer, and the only thing the engine ever asked
       either of them was whether it had one. The two below are the
       frozen copy's, for the reason given in sSndSoundBufferElem. */
    int deviceOpen;
#ifdef WINSOUND_FROZEN
    IDirectSound8* dsndif;
    IDirectSoundBuffer* SoundBuffer;
#else
    void* frozenOnlyHandles[2];
#endif
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
    float fadeRate;
    float fadeLevel;
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
    float startVolume;
    float targetVolume;
    float duration;
    float rate;
    int flags;
    float elapsedTime;
} tSoundModifierDataFade;

typedef struct sSoundModifierDataPlayer
{
    tSoundModifierDataGeneric gen;
    int freq;
    float vol;
    float fnear;
    float ffar;
    int flags;
    int dataVersion;
    float timeRemainTillStart;
} tSoundModifierDataPlayer;

typedef struct sSoundModifierDataExtPlayer
{
    tSoundModifierDataGeneric gen;
    int flags;
    int idx;
    int flags2;
    float vol;
    float vol2;
    float fadeInDuration;
    float fadeOutDuration;
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
    int dataVersion;
    float timeRemainTillStart;
} tSoundModifierDataExtPlayer;

typedef struct sSoundModifierData3DSettings
{
    tSoundModifierDataGeneric gen;
    float interpolationFactor;
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
    float fadeCurveRate;
    float fadeFrom;
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
    /* The same slot as the sound's own: see tSndRef above. */
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


/* What a sound's buffer slot holds, and how to read it.
 *
 * sndSndBufElem and the instance's soundBufElemFromModel are the same
 * slot, and it does not hold one kind of thing. Which kind depends on
 * which of the three interfaces below the sound is using:
 *
 *   the plain one   a sound buffer the platform layer made
 *   the stream one  a one-based index into the stream list
 *   the track one   whichever of those the track was built from, used
 *                   as a key into the track lookup table
 *
 * and nought or -1 for no sound at all. The slot is read back as a
 * 32-bit key in that lookup table, so a buffer goes in it as a handle
 * rather than as its address, and an index goes in tagged -- so the two
 * can be told apart by whoever reads the slot without already knowing
 * which it was given.
 *
 * The slot keeps its pointer declaration because the frozen copies in
 * tests/ share these structs and have to agree with the rewrite about
 * their layout. Nothing dereferences it.
 */
typedef struct sSndSoundBufferElem* tSndRef;

/* Whether the slot is encoded at all.
 *
 * It follows the translation layer, and has to: a build where a handle
 * is the address it stands for is a build where the slot holds raw
 * values, and tagging an index there would encode half of the slot and
 * not the other half. The frozen copies in tests/ know nothing about
 * either, so they are built that way and the rewrite agrees with them
 * about every slot. */
#ifndef SND_REF_TAGS
#define SND_REF_TAGS HDL_TRANSLATES
#endif

/* A buffer the platform layer made, as a slot value. */
static __inline tSndRef SND_ref_FromBuffer(void* buffer)
{
    return (tSndRef)(uintptr_t)HDL_ul_Handle(buffer);
}

/* A one-based index into a list, as a slot value: tagged, so it is not
   mistaken for a reference. Nought stays nought, because that is no
   sound rather than index nought. */
static __inline tSndRef SND_ref_FromIndex(int index)
{
#if SND_REF_TAGS
    if (!index)
        return (tSndRef)0;
    return (tSndRef)(uintptr_t)(((uint32_t)index << HDL_TAG_BITS)
                                | HDL_TAG_INDEX);
#else
    return (tSndRef)(uintptr_t)(uint32_t)index;
#endif
}

static __inline uint32_t SND_ul_RefBits(tSndRef ref)
{
    return (uint32_t)(uintptr_t)ref;
}

static __inline BOOL SND_b_RefIsIndex(tSndRef ref)
{
#if SND_REF_TAGS
    return (SND_ul_RefBits(ref) & HDL_TAG_MASK) == HDL_TAG_INDEX;
#else
    (void)ref;
    return 0;        /* nothing is tagged, so nothing can be told apart */
#endif
}

/* The index a slot holds, or the value itself where that is no sound. */
static __inline int SND_i_IndexFromRef(tSndRef ref)
{
    uint32_t bits = SND_ul_RefBits(ref);

#if SND_REF_TAGS
    if (!bits || bits == HDL_NOT_FOUND)
        return (int)bits;
    return (int)(bits >> HDL_TAG_BITS);
#else
    return (int)bits;
#endif
}

/* The buffer a slot refers to. A slot holding an index or one of the
   reserved values comes back as the value it holds, which is what the
   translation layer does with anything it did not issue. */
static __inline struct sSndSoundBufferElem* SND_p_BufferFromRef(tSndRef ref)
{
    return (struct sSndSoundBufferElem*)HDL_p_Pointer(SND_ul_RefBits(ref));
}

typedef struct sSndSoundElem
{
    int soundKey;
    unsigned int flags;
    int muteFacorFlags;
    int usageCnt;
    /* A slot, not a pointer: see tSndRef above. */
    struct sSndSoundBufferElem* sndSndBufElem;
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
    int attenuated;
    float attenuationFactor;
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
/* The sound file's own header, read in whole. Which of the three
   shapes applies is decided by fmtChunkSize: 14 has no bit depth and
   no extension size, 16 adds the bit depth, 18 adds both. The header
   is 28 bytes plus that size, so 42, 44 or 46. */
typedef struct sSoundHeaderDataCase0
{
    int riffId;
    int riffChunkSize;
    int waveId;
    int fmtId;
    int fmtChunkSize;
    int16_t wFormatTag;
    int16_t nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    int16_t nBlockAlign;
    int dataId;
    int dataChunkSize;
} tSoundHeaderDataCase0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct sSoundHeaderDataCase1
{
    int riffId;
    int riffChunkSize;
    int waveId;
    int fmtId;
    int fmtChunkSize;
    int16_t wFormatTag;
    int16_t nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    int16_t nBlockAlign;
    int16_t wBitsPerSample;
    int dataId;
    int dataChunkSize;
} tSoundHeaderDataCase1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct sSoundHeaderDataCase2
{
    int riffId;
    int riffChunkSize;
    int waveId;
    int fmtId;
    int fmtChunkSize;
    int16_t wFormatTag;
    int16_t nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    int16_t nBlockAlign;
    int16_t wBitsPerSample;
    int16_t cbSize;
    int dataId;
    int dataChunkSize;
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
/* The format of a sound, as its wav header gives it.

   Field for field and width for width what WAVEFORMATEX is, and
   named the same, because every reader of it says wfex.nChannels and
   the like. It is the engine's own type rather than Win32's for one
   reason: nothing else in the sound needs windows.h now, and a
   header that cannot be compiled off Windows is the whole of what
   this move is against.

   Nothing reads the bytes of this straight out of a file -- the wav
   header is parsed into tSoundHeaderDataCase* and then assigned
   across field by field -- so its layout answers to nobody. */
typedef struct sSndWaveFormat
{
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    uint16_t cbSize;
} tSndWaveFormat;

typedef struct sSndWavHdrStruct
{
    int32_t fullSoundSize;
    int32_t dataStartPos;
    tSndWaveFormat wfex;
} tSndWavHdrStruct;

/* Bytes handed out per category, kept for inspection only: the engine
   writes these and reads none of them back, and the three fields
   between them are written by the initial clear and never again. What
   they were for is not recoverable, so they keep their placeholders
   rather than a name that would only look informative. */
typedef struct sSndRasterStatStruct
{
    int trackBytes;
    int i2;
    int loadingSoundBytes;
    int i4;
    int soundBytes;
    int i6;
    int totalBytes;
    int budgetLeft;
} tSndRasterStatStruct;

typedef struct sSoundFxParams
{
    int flags;
    int mode;
    int delay;
    int feedback;
    float vol;
    int wetPan;
    int voiceListCount;
    struct sSndSoundBufferElem** voiceList;
} tSoundFxParams;

typedef struct sSoundFamilyInnerStruct
{
    int refCount;
    tMutesSetupStruct muteSetup1;
    tMutesSetupStruct muteSetup2;
    float muteSetup1TimeMultiplier;
    float muteSetup2TimeMultiplier;
} tSoundFamilyInnerStruct;


typedef struct sSoundFamilyStruct
{
    int requiredMask;
    int anyMaskUnion;
    int innerNum;
    int anyMask[32];
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
    /* Which revision of this record the data was written at. The
       loader sets it from the record's own length, fills in whatever
       that older shape lacked, then stamps the current value. */
    int dataVersion;
    int flags;
    int modifierID;
    int sndKey;
    float volume;
    float volume2;
    float fadeInTime;
    float distPrefetchVolume;
    float fadeOutTime;
    int familyMask;
    int freq2;
    int pan;
    int ii8;
    int ii9;
    int flags2;
    int muteFactorFlags;
    int idx;
    int sndInstanceID;
    float near1;
    float near2;
    float near3;
    float far1;
    float far2;
    float far3;
    float dtFar;
    int trackID;
    float nearFarFactor;
    float nearFar2factor;
    float nearFar3factor;
    float farCoefficient;
    float middleCoefficient;
    float minPan;
    int soundIndex;
    tSndBank* soundBank;
    float timeRemainTillStart;
    int b3;
    int b4;
    float freq;
    char gap[40];
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

/* How much room the AND-family list takes for a given capacity.
 *
 * The frozen copies write this folded, as
 * 16 * (9 * size + sizeof(tSoundFamilyStruct)), where 9 * 16 is the
 * struct's own size on a 32-bit target -- so the two forms agree there
 * and the folded one comes out a whole element short per element once
 * the struct is wider, which the list then indexes past. The harness
 * builds both copies with SND_FAMILY_FROZEN so they stay comparable;
 * ./tests/run_sndfamily.sh --asan is the check on the real one.
 */
#ifndef SND_FAMILY_FROZEN
#define SND_FAMILY_FROZEN 0
#endif

#if SND_FAMILY_FROZEN
#define SND_FAMILY_LIST_BYTES(cap) \
    (16 * (9 * (cap) + sizeof(tSoundFamilyStruct)))
#define SND_FAMILY_GROWN_CLEAR(cap) SND_FAMILY_LIST_BYTES(cap)
#else
#define SND_FAMILY_LIST_BYTES(cap) \
    (sizeof(tSoundFamilyStruct) * ((unsigned int)(cap) + 16u))
/* Only the sixteen the list just grew by. Clearing the whole figure
   above from the first new element runs off the end by every element
   the list already had. */
#define SND_FAMILY_GROWN_CLEAR(cap) (sizeof(tSoundFamilyStruct) * 16u)
#endif

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
/* Offset is a position in the big file for read modes 1 and 5, and the
   address of a header already in memory for mode 4, so it is as wide as
   a pointer. The frozen copy in tests/ still holds that address in an
   int and is compiled with SND_WAVEINFO_FROZEN, which keeps its own
   signature; nothing in the game defines it. */
#ifdef SND_WAVEINFO_FROZEN
int SND_i_GetWaveInfo(int Offset, tSndWavHdrStruct* wavHdrStructDest, int readMode);
#else
int SND_i_GetWaveInfo(intptr_t Offset, tSndWavHdrStruct* wavHdrStructDest, int readMode);
#endif
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