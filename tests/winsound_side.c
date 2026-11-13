/* One side of the win/winsound.c differential test.
 *
 * Compiled twice, once against the rewrite and once against the frozen
 * copy in baseline_winsound.c, with every winsound.c symbol, every global
 * winsound.h and sound.h give it a tentative definition for, this file's
 * two entry points and the engine callbacks below all renamed by -D. Both
 * sides therefore drive their own copy of the platform layer's globals
 * over one shared mock device, and the harness compares the text each side
 * dumps plus the mock call log.
 *
 * The engine callbacks live here rather than in winsound_mocks.c because
 * they are handed pointers into the caller's own globals, which are
 * per-side; a shared mock could not turn those into comparable text.
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include "winsound_mocks.h"
#include "../sound.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../time.h"
#include "../math.h"
#include "../gameobject.h"
#include "../dynamics.h"
#include "../win/wingdi.h"
#include "../win/winsound.h"

#ifndef SIDE_RUN
#define SIDE_RUN wsnd_side_run
#endif
#ifndef SIDE_CASES
#define SIDE_CASES wsnd_side_cases
#endif
#ifndef SIDE_RUN_GUARDS
#define SIDE_RUN_GUARDS wsnd_side_guards
#endif

/* winsound.c defines these and no header declares them. */
void pcSND_AddSoundToThePlayList(BOOL notifyThread);
void pcSND_RemoveSoundFromPlayList(tSndSoundBufferElem* sbe, int fireEvent);
void pcSND_StopSoundAndRemoveFromThePlayList(tSndSoundBufferElem* sbe, int fireEvent);
void pcSND_updateCurrentPlayPositionAndSize(tSndSoundBufferElem* sbe);
unsigned int pcSND_CalcSizeToReadByFormat(unsigned int byteCount, unsigned int channels);
unsigned int pcSND_calcBlockNumForDecompress(int size, int channels);
unsigned int pcSND_calcMemoryNeededForTheSound(unsigned int fullsoundlen, int channels);
void pcSND_ReadAndDecompressSoundData(unsigned int wanted, tSndSoundBufferElem* sbe, int useFirstBuffer,
                                      uint32_t* decodedSize, int16_t* dest);
int pcSND_SoundThread(void* parameter);
int pcSND_CreateSoundBuffer(IDirectSoundBuffer* primary, IDirectSound8* dsndif);
BOOL pcSND_CreateSoundBufferAndPlayTest(tSndHandles* handles);
int pcSND_l_Init(tSndHandles* sndh);
int pcSND_i_ChangeRenderMode(int mode);
int pcSND_SetDefaultSoundPositionAndDistance(tSndSoundBufferElem* sbe);
int pcSND_SNDPropertySetRoomReverb(tSndSoundBufferElem* sbe, int enable);
void pcSND_DisableReverbAndRestartPlaying(tSoundFxParams* params, tSndSoundBufferElem* sbe);
int pcSND_DisableReverbIf_i7_not_match_with_idx(tSoundFxParams* params, tSndSoundBufferElem* sbe);
void pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(tSndSoundBufferElem* sbe);
void pcSND_SB_Stop(tSndSoundBufferElem* sbe);
void pcSND_stopSoundIfReadFinished(tSndSoundBufferElem* sbe);
int pcSND_SB_GetStatus(tSndSoundBufferElem* sbe, int* status);
int pcSND_SB_Play(tSndSoundBufferElem* sbe, int flags);
void pcSND_AddToSNDRaster(IDirectSoundBuffer* buffer, int bytes);
tSndSoundBufferElem* pcSND_pst_SB_Create(tSndHandles* handles, tSndWavHdrStruct* sndHdr, int sndStartPos);
int pcSND_DSoundBuffersRelease(void);
void pcSND_Cleanup(void);
void pcSND_Close(tSndHandles* handles);
void pcSND_SB_Duplicate(tSndHandles* handles, tSndSoundBufferElem* src, tSndSoundBufferElem** dest);
void pcSND_SB_GetFrequency(tSndSoundBufferElem* sbe, int* freq);
void pcSND_SB_GetPan(tSndSoundBufferElem* sbe, int* pan, int* front);
int pcSND_3DSoundBufferSetPos(tSndSoundBufferElem* sbe, int pan);
void pcSND_SB_SetPan(tSndSoundBufferElem* sbe, int pan, int front);
void pcSND_SB_SetCurrPos(tSndSoundBufferElem* sbe, int pos);
void pcSND_SB_SetVolume(tSndSoundBufferElem* sbe, int att);
void pcSND_SNDRasterRelease(IDirectSoundBuffer* buffer);
void pcSND_MiscDataRelease(tSndSoundBufferElem* sbe);
void pcSND_ReleaseSoundBuffer(tSndSoundBufferElem* sbe);
int pcSND_Return160(void);
tSndSoundBufferElem* pcSND_SoundReloadToDSound(tSndHandles* handles, tSndWavHdrStruct* wavHdr, int offset);
tSndSoundBufferElem* pcSND_SB_Reload(tSndHandles* handles, tSndSoundBufferElem* sndBufferElem,
                                     tSndWavHdrStruct* wavHdr, int offset);
void pcSND_StreamPrefetchArray(int* positions, int* sizes);
void pcSND_setCurrentSoundBuffElemAndSetAnEvent(tSndSoundBufferElem* sbe);
void pcSND_SetPositionNotify(tSndSoundBufferElem* sbe);
void pcSND_fillSoundBuffer(int unused, tSbeMiscData* miscArg, tSndSoundBufferElem* sbe);
void pcSND_SB_SetFrequency(tSndSoundBufferElem* sbe, int freq);
void pcSND_UnknownSoundPanAndVolumeSetByUnknown(tSndSoundBufferElem* sbe, int pan);
void pcSND_StreamReinitAndPlay(tSndSoundBufferElem* sbe, int len, int startPos, int from, int to,
                               char flags, int freq, int pan, int att);
tSndSoundBufferElem* pcSND_CreateTrack(tSndHandles* handles, tSndWavHdrStruct* wavHdr);
void pcSND_CreateTrackSB(tSndSoundBufferElem** a, tSndSoundBufferElem** b);
void pcSND_ReleaseTrackSB(tSndSoundBufferElem* a, tSndSoundBufferElem* b);
void pcSND_ShutDownStreams(void);
int pcSND_l_GetGlobalVol(void);
void pcSND_SetGlobalVol(int att);
void SND_ComputeDoppler(Vec3* listenerSpeed, Vec3* listenerPos, tSoundInstanceElem* sie);
float SND_f_Compute3DVol(tSoundInstanceElem* inst);
int pcSND_i_FxInit(void);
void pcSND_FxClose(void);
void pcSND_fxStructSet(EAXLISTENERPROPERTIES* dest, int preset);
int pcSND_EAXListenerPropertiesSet(int preset);
void pcSND_DummyFXFunc(tSoundFxParams* params);
int pcSND_i_FxSetMode(tSoundFxParams* params, int mode);
int pcSND_i_FxGetMode(tSoundFxParams* params);
int pcSND_i_FxSetFeedback(tSoundFxParams* params, int feedback);
int pcSND_i_FxGetFeedback(tSoundFxParams* params);
int pcSND_i_FxSetDelay(tSoundFxParams* params, int delay);
int pcSND_i_FxGetDelay(tSoundFxParams* params);
int pcSND_i_FxSetWetVolume(tSoundFxParams* params, int att);
int pcSND_i_FxGetWetVolume(tSoundFxParams* params);
int pcSND_i_FxSetWetPan(tSoundFxParams* params, int pan);
int pcSND_i_FxGetWetPan(tSoundFxParams* params);
void pcSND_SNDPropertyEnableRoomReverb(tSoundFxParams* params, tSndSoundBufferElem* sbe);
int pcSND_i_FxEnable(tSoundFxParams* params);
int pcSND_i_FxDisable(tSoundFxParams* params);
int pcSND_Stream_GetStatus(tSndSoundBufferElem* h, int* status);
int pcSND_Stream_Play(tSndSoundBufferElem* h, int flags);
void pcSND_Stream_Duplicate(tSndHandles* handles, tSndSoundBufferElem* src, tSndSoundBufferElem** dest);
void pcSND_Stream_GetFrequency(tSndSoundBufferElem* h, int* freq);
void pcSND_Stream_GetPan(tSndSoundBufferElem* h, int* pan, int* front);
void pcSND_Stream_GetCurrPos(tSndSoundBufferElem* h, int* currPos, int* writePos);
void pcSND_Stream_SetVolume(tSndSoundBufferElem* h, int att);
int pcSND_Stream_Release(tSndSoundBufferElem* h);
int pcSND_Stream_SetFrequency(tSndSoundBufferElem* h, int freq);
void pcSND_Stream_SetPan(tSndSoundBufferElem* h, int pan, int front);
void pcSND_Stream_SetCurrPos(tSndSoundBufferElem* h, int pos);
int pcSND_iTrack_GetStatus(tSndSoundBufferElem* key, int* status);
int pcSND_iTrack_Play(tSndSoundBufferElem* key, int flags);
void pcSND_Track_GetFrequency(tSndSoundBufferElem* key, int* freq);
void pcSND_Track_GetPan(tSndSoundBufferElem* key, int* pan, int* front);
void pcSND_Track_GetCurrPos(tSndSoundBufferElem* key, int* currPos, int* writePos);
void pcSND_Track_Pause(tSndSoundBufferElem* key);
void pcSND_Track_Release(tSndSoundBufferElem* key);
void pcSND_Track_SetFrequency(tSndSoundBufferElem* key, int freq);
void pcSND_Track_SetPan(tSndSoundBufferElem* key, int pan, int front);
void pcSND_Track_SetCurrPos(tSndSoundBufferElem* key, int pos);
void pcSND_Track_SetVolume(tSndSoundBufferElem* key, int att);
void pcSND_Track_Stop(tSndSoundBufferElem* key);
void pcSND_SB_GetCurrPos(tSndSoundBufferElem* sbe, int* currPos, int* writePos);
void pcSND_flipPlayStopState(tSndSoundBufferElem* sbe);
void pcSND_SB_Pause(tSndSoundBufferElem* sbe);
void pcSND_D3DSoundBufAAndIKPopertySetRelease(tSndSoundBufferElem* sbe);
void pcSND_SB_Release(tSndSoundBufferElem* sbe);
int pcSND_l_GetGlobalPan(void);
void pcSND_SetGlobalPan(int pan);
void pcSND_Null(Vec3* v);
void pcSND_StreamPrefetch(void);
void pcSND_StreamFlush(void);
void pcSND_StreamGetMonoToStereo(tSndSoundBufferElem** a, tSndSoundBufferElem** b);
void pcSND_StreamGetStereoToMono(void);
void pcSND_Stream_Pause(tSndSoundBufferElem* h);
void pcSND_Stream_Stop(tSndSoundBufferElem* h);

/* ------------------------------------------------- the engine callbacks */

/* This side's fixture, so that a pointer into it can be printed as an
   index rather than an address. */
#define INSTANCES 8
#define STREAMS 6
#define TRACKS 4

static tSoundInstanceElem fx_instances[INSTANCES];
static tSndStreamList fx_streams[STREAMS];
static tTrackListElem fx_tracks[TRACKS];
static tSndSoundBufferElem fx_buffers[8];
static tSbeMiscData fx_misc[8];
static tSndHandles fx_handles;
static Mtx4x4 fx_micro;
static Mtx4x4 fx_ref;
static tGameObjectMain fx_gao;
static tGameObjectExtended fx_ext;
static Mtx4x4 fx_gaoMtx;
static int fx_dyn;
static tSoundFxParams fx_params;
static tSndSoundBufferElem* fx_fxSlots[16];

static int inst_id(const void* p)
{
    const tSoundInstanceElem* q = (const tSoundInstanceElem*)p;
    if (!p) return -1;
    if (q >= fx_instances && q < fx_instances + INSTANCES) return (int)(q - fx_instances);
    return -2;
}

static int sbe_id(const void* p)
{
    const tSndSoundBufferElem* q = (const tSndSoundBufferElem*)p;
    if (!p) return -1;
    if (q == (const tSndSoundBufferElem*)-1) return -3;
    if (q >= fx_buffers && q < fx_buffers + 8) return (int)(q - fx_buffers);
    return (int)(1000 + wsnd_heap_off(p));
}

int SND_l_GetAttFromVol(float vol)
{
    int att;
    if (vol <= 0.0f)
        att = -10000;
    else
        att = (int)(2000.0 * (double)vol) - 2000;
    wsnd_log("GetAttFromVol(%.9g)=%d\n", (double)vol, att);
    return att;
}

float SND_f_GetVolFromAtt(int att)
{
    float vol = (float)((double)att / 20000.0 + 0.5);
    wsnd_log("GetVolFromAtt(%d)=%.9g\n", att, (double)vol);
    return vol;
}

float SND_f_GetGlobalVol(void)
{
    wsnd_log("GetGlobalVol\n");
    return 0.75f;
}

void SND_SetInstVolume(tSoundInstanceElem* inst) { wsnd_log("SetInstVolume(i%d)\n", inst_id(inst)); }
void SND_AutoStop(tSoundInstanceElem* inst) { wsnd_log("AutoStop(i%d)\n", inst_id(inst)); }
void SND_AutoPlay(tSoundInstanceElem* inst) { wsnd_log("AutoPlay(i%d)\n", inst_id(inst)); }

float SND_f_SphericComputing(Vec3* micPos, tSoundInstanceElem* inst, int* state)
{
    *state = inst->flags & 3;
    wsnd_log("SphericComputing(i%d)=%d\n", inst_id(inst), *state);
    return 0.5f + 0.125f * (float)(inst->allocatedSoundsIdx & 3);
}

float SND_f_AxisComputing(Vec3* micPos, tSoundInstanceElem* inst, int* state, int axis)
{
    int before = *state;
    *state = (inst->flags >> (axis * 2)) & 3;
    wsnd_log("AxisComputing(i%d,a%d,%X)=%d\n", inst_id(inst), axis, before, *state);
    return 0.25f * (float)(axis + 1);
}

int SND_i_IsPlaying(int idx) { wsnd_log("IsPlaying(%d)\n", idx); return (idx & 1) == 0; }
int SND_i_StreamAlloc(void) { wsnd_log("StreamAlloc\n"); return 3; }
void SND_TrackDeleteInstanceFromList(int id) { wsnd_log("TrackDeleteInstanceFromList(%d)\n", id); }

struct sDyn* OBJ_pst_GetDynaStruct(tGameObjectMain* gao)
{
    wsnd_log("GetDynaStruct(%d)\n", gao == &fx_gao);
    return fx_dyn ? (struct sDyn*)&fx_dyn : 0;
}

void DYN_SpeedGetVector(struct sDyn* dyn, Vec3* v)
{
    v->x = 1.5f;
    v->y = -0.25f;
    v->z = 0.75f;
    wsnd_log("SpeedGetVector\n");
}

/* --------------------------------------------------------------- dump */

static void d(const char* fmt, ...)
{
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    wsnd_log("%s", buf);
}

static unsigned fbits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

static void dump_globals(void)
{
    int i;

    d("G ev=%d hn=%d cur=%d tni=%d rb1=%d rb2=%d\n",
      sndEventsNum, sndEventHandleNum, sbe_id(sndCurrSoundBufferElem),
      trackEventsArrayNextIdx, dsndReadBuffer1 != 0, dsndReadBuffer2 != 0);
    d("G caps=%u/%u c0=%d c4=%d play=%d peak=%d vol=%d\n",
      gDoSoundGlobal.dsndCaps.dwSize, gDoSoundGlobal.dsndCaps.dwFlags,
      gDoSoundGlobal.dword_B303C0, gDoSoundGlobal.dword_B303C4,
      gDoSoundGlobal.sndConcurrentlyPlayedNum, gDoSoundGlobal.sndConcurrentlyPlayedPeakNum,
      gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0);
    d("G eaxok=%d fxen=%d mode=%d wet=%08X d3d=%d l3d=%d b3d=%d ks=%d\n",
      pcSNDEAXLisenerPropertiesSetIsOk, gFXGlobal.fxEnabled, gFXGlobal.mode, fbits(fxWetVol),
      wsnd_obj_id(d3dSndBuffer), wsnd_obj_id(dsndIDisectSound3DListenerIF),
      wsnd_obj_id(ppdsndDSND3DBuffer), wsnd_obj_id(ppdsndIKSPropertySet));
    d("G list n=%d max=%d\n", dsndSBList.actElemNum, dsndSBList.maxElemNum);
    for (i = 0; i < dsndSBList.actElemNum && i < 16; ++i)
        d("  L%d key=%d val=%d\n", i, dsndSBList.keyValPtr[i].key != 0,
          dsndSBList.keyValPtr[i].val != 0);
    d("G raster %d %d %d %d %d %d %d %d\n",
      sndRasterStat.i1, sndRasterStat.i2, sndRasterStat.i3, sndRasterStat.i4,
      sndRasterStat.i5, sndRasterStat.i6, sndRasterStat.i7, sndRasterStat.i8);
    d("G rb1=%d rb2=%d fileid=%d\n", rasterbas1.actElemNum, rasterbas2.actElemNum,
      SND_gi_RasterCurrentFileId);
    for (i = 0; i < 8 && sndPlayList && i < sndEventsNum; ++i)
        d("  P%d use=%d elem=%d h0=%ld h1=%ld\n", i, sndPlayList[i].playListElemInUse,
          sbe_id(sndPlayList[i].soundElem),
          sndEventHandles ? (long)(size_t)sndEventHandles[2 * i] : -1,
          sndEventHandles ? (long)(size_t)sndEventHandles[2 * i + 1] : -1);
}

static void dump_sbe(const char* tag, tSndSoundBufferElem* sbe)
{
    if (!sbe)
    {
        d("%s NULL\n", tag);
        return;
    }
    d("%s buf=%d fmt=%d bal=%d vol=%d b3d=%d ks=%d\n", tag,
      wsnd_obj_id(sbe->dsndSBIF), sbe->format, sbe->unk2SetTo64InCreate,
      sbe->unknownFiledinsub49db40ToSetVolume,
      wsnd_obj_id(sbe->dsnd3dbufferIF), wsnd_obj_id(sbe->dsndIKPropertySetIF));
    if (sbe->miscData)
    {
        tSbeMiscData* m = sbe->miscData;
        d("%s  misc ntf=%d fh=%ld dtm=%d full=%u sum=%d u6=%d size=%d up=%d\n", tag,
          wsnd_obj_id(m->vmilenEventHandlptr_Sub49e950), (long)(size_t)m->fileHandle,
          m->distanceToMove, m->fullSize, m->sumSize, m->unk6, m->soundBufferSize, m->sizeToUpload);
        d("%s  misc e1=%ld e2=%ld off=%d pos=%d cur=%u rem=%d loop=%d rap=%d stop=%d idx=%d\n", tag,
          (long)(size_t)m->eventHandle1, (long)(size_t)m->eventHandle2,
          m->currOffset, m->currentPlayPosition, m->currSize, m->shouldBeRemovedFromPlayList,
          m->readThroughWithMultipleLoop, m->RemoveAfterPlayFinished, m->Stopped_SetIN49e270,
          m->dsndEventsArrayIdx);
    }
}

static void dump_hdr(const char* tag, tSndWavHdrStruct* h)
{
    d("%s hdr full=%d start=%d tag=%u ch=%u rate=%u avg=%u align=%u bps=%u cb=%u\n", tag,
      h->fullSoundSize, h->dataStartPos, h->wfex.wFormatTag, h->wfex.nChannels,
      h->wfex.nSamplesPerSec, h->wfex.nAvgBytesPerSec, h->wfex.nBlockAlign,
      h->wfex.wBitsPerSample, h->wfex.cbSize);
}

static void dump_fx(const char* tag, tSoundFxParams* p)
{
    int i;
    d("%s fx flags=%X mode=%d delay=%d fb=%d vol=%08X wetpan=%d i7=%d\n", tag,
      p->flags, p->mode, p->delay, p->feedback, fbits(p->vol), p->wetPan, p->i7);
    if (p->i8)
        for (i = 0; i < p->i7 && i < 16; ++i)
            d("%s   slot%d=%d\n", tag, i, sbe_id(p->i8[i]));
}

/* ------------------------------------------------------------ fixture */

/* An orientation matrix with unit rows, so that a dot product with a
   normalised direction lands in [-1, 1] and the clamps and the
   attenuation lookup under them are actually exercised. */
static void mtx_set(Mtx4x4* m, float angle)
{
    float c = (float)cos((double)angle);
    float sn = (float)sin((double)angle);

    m->v1.x = c;    m->v1.y = sn;   m->v1.z = 0.0f;   m->_m14 = 0.0f;
    m->v2.x = -sn;  m->v2.y = c;    m->v2.z = 0.0f;   m->_m24 = 0.0f;
    m->v3.x = 0.0f; m->v3.y = 0.0f; m->v3.z = 1.0f;   m->_m34 = 0.0f;
    m->Pos.x = angle; m->Pos.y = -angle; m->Pos.z = 0.5f * angle;
    m->w = 1.0f;
    m->mtxflag = 0;
    m->unk1 = 0;
    m->unk2 = 0;
    m->unk3 = 0;
}

static void fixture_reset(unsigned seed)
{
    int i;

    memset(fx_instances, 0, sizeof(fx_instances));
    memset(fx_streams, 0, sizeof(fx_streams));
    memset(fx_tracks, 0, sizeof(fx_tracks));
    memset(fx_buffers, 0, sizeof(fx_buffers));
    memset(fx_misc, 0, sizeof(fx_misc));
    memset(&fx_handles, 0, sizeof(fx_handles));
    memset(&fx_params, 0, sizeof(fx_params));
    memset(fx_fxSlots, 0, sizeof(fx_fxSlots));
    memset(&fx_gao, 0, sizeof(fx_gao));
    memset(&fx_ext, 0, sizeof(fx_ext));

    memset(&SND_gst_Params, 0, sizeof(SND_gst_Params));
    memset(&gDoSoundGlobal, 0, sizeof(gDoSoundGlobal));
    memset(&gFXGlobal, 0, sizeof(gFXGlobal));
    memset(&sndRasterStat, 0, sizeof(sndRasterStat));
    memset(&dsndSBList, 0, sizeof(dsndSBList));
    memset(&rasterbas1, 0, sizeof(rasterbas1));
    memset(&rasterbas2, 0, sizeof(rasterbas2));
    memset(&SND_gpst_Interface, 0, sizeof(SND_gpst_Interface));
    memset(&SND_gst_TrackLookUpTable, 0, sizeof(SND_gst_TrackLookUpTable));
    memset(&SND_gst_FxParam, 0, sizeof(SND_gst_FxParam));
    memset(&SND_gst_FxDefaultParam, 0, sizeof(SND_gst_FxDefaultParam));
    memset(dsndEvents, 0, sizeof(dsndEvents));

    sndEventsNum = 0;
    sndEventHandleNum = 0;
    sndEventHandles = 0;
    sndPlayList = 0;
    sndCurrSoundBufferElem = 0;
    trackEventsArrayNextIdx = 0;
    dsndReadBuffer1 = 0;
    dsndReadBuffer2 = 0;
    pcSNDThreadId = 0;
    pcSNDhObject = 0;
    pcSNDEAXLisenerPropertiesSetIsOk = 0;
    pcSNDFXEnabled_dword_B30400 = 1;
    fxWetVol = 0.0f;
    d3dSndBuffer = 0;
    dsndIDisectSound3DListenerIF = 0;
    ppdsndDSND3DBuffer = 0;
    ppdsndIKSPropertySet = 0;
    snd_eaxDLLHandle = 0;
    pEAXDirectSoundCreate8 = 0;
    SND_gc_NoSound = 0;
    LOA_gb_SpeedMode = 0;
    MAI_gh_MainWindow = (HWND)(size_t)0x1234;
    TIM_gf_dt = 0.0166666f;

    SND_gst_Params.soundEnabled = 1;
    SND_gst_Params.soundRenderMode = 2;
    SND_gst_Params.soundInstanceNum = INSTANCES;
    SND_gst_Params.soundInstances = fx_instances;
    SND_gst_Params.handles = &fx_handles;
    SND_gst_Params.f_surround = 1.25f;
    SND_gst_Params.fx_doppler = 0.75f;
    SND_gst_Params.fx_DopplerSpeed = 340.0f;
    mtx_set(&fx_micro, 0.25f);
    mtx_set(&fx_ref, -0.5f);
    mtx_set(&fx_gaoMtx, 2.0f);
    fx_micro.v3.x = 0.6f;
    fx_micro.v3.y = 0.0f;
    fx_micro.v3.z = 0.8f;
    SND_gst_Params.microPosMtxPtr = &fx_micro;
    SND_gst_Params.sndMatrixRef = &fx_ref;

    /* SND_i_SoftInit fills this at startup; several of the functions
       under test call back through it. */
    SND_gpst_Interface.win.GetStatus = pcSND_SB_GetStatus;
    SND_gpst_Interface.win.Play = pcSND_SB_Play;
    SND_gpst_Interface.win.Stop = pcSND_SB_Stop;
    SND_gpst_Interface.win.Pause = pcSND_SB_Pause;
    SND_gpst_Interface.win.Release = pcSND_SB_Release;
    SND_gpst_Interface.win.GetFrequency = pcSND_SB_GetFrequency;
    SND_gpst_Interface.win.GetPan = pcSND_SB_GetPan;
    SND_gpst_Interface.win.GetCurrPos = pcSND_SB_GetCurrPos;
    SND_gpst_Interface.win.SetFrequency = pcSND_SB_SetFrequency;
    SND_gpst_Interface.win.SetPan = pcSND_SB_SetPan;
    SND_gpst_Interface.win.SetCurrPos = pcSND_SB_SetCurrPos;
    SND_gpst_Interface.win.Setvolume = pcSND_SB_SetVolume;

    SND_gast_StreamList = fx_streams;
    SND_gi_StreamListSize = STREAMS;
    SND_gax_TrackPlayingList = fx_tracks;
    SND_gi_RasterCurrentFileId = 0x2000;

    fx_handles.dsndif = wsnd_device();
    fx_handles.MainWindowHandle = (HWND)(size_t)0x1234;

    /* sound.c sizes these at startup; without it basic_array grows a
       zero-element array and writes past it. */
    BAS_binit(&rasterbas1, 200);
    BAS_binit(&rasterbas2, 200);
    BAS_binit(&SND_gst_TrackLookUpTable, 200);
    BAS_binit(&dsndSBList, 48);

    fx_gao.flags = 0x8000000;
    fx_gao.extended = &fx_ext;
    fx_gao.matrices = &fx_gaoMtx;
    fx_ext.snd = (struct sSndBank*)(size_t)0x7777;
    fx_dyn = 1;

    for (i = 0; i < INSTANCES; ++i)
    {
        fx_instances[i].allocatedSoundsIdx = i;
        fx_instances[i].flags = (int)((seed >> i) & 0xFF) | ((i & 3) << 24);
        fx_instances[i].freq = 22050 + 100 * i;
        fx_instances[i].volume = 0.5f + 0.0625f * (float)i;
        fx_instances[i].minPan = 0.125f * (float)(i & 3);
        fx_instances[i].soundIntf = &SND_gpst_Interface.win;
        fx_instances[i].sndInstPosVec.x = 1.0f * (float)i;
        fx_instances[i].sndInstPosVec.y = -2.0f * (float)i;
        fx_instances[i].sndInstPosVec.z = 0.5f * (float)i;
        fx_instances[i].sndInstPosVecPrevious.x = 0.5f * (float)i;
        fx_instances[i].sndInstPosVecPrevious.y = -1.0f * (float)i;
        fx_instances[i].sndInstPosVecPrevious.z = 0.25f * (float)i;
        fx_instances[i].sndPositionSetInSetPosPtr = &fx_instances[i].sndInstPosVec;
    }
    for (i = 0; i < STREAMS; ++i)
    {
        fx_streams[i].num = i;
        fx_streams[i].vol = -100 * i;
        fx_streams[i].pan1 = 10 * i;
        fx_streams[i].pan2 = -10 * i;
        fx_streams[i].freq = 11025 + i;
        fx_streams[i].currPos = 7 * i;
    }
    for (i = 0; i < TRACKS; ++i)
    {
        fx_tracks[i].trackID = i;
        fx_tracks[i].unk1 = i & 1;
        fx_tracks[i].f1 = 0.5f + 0.25f * (float)i;
        fx_tracks[i].sndBufferElem = &fx_buffers[i];
    }
    for (i = 0; i < 8; ++i)
    {
        fx_buffers[i].format = 1 + (i & 1);
        fx_buffers[i].unk2SetTo64InCreate = 64;
    }
    fx_params.i7 = 16;
    fx_params.i8 = fx_fxSlots;
    fx_params.flags = 1;
}

/* Give a buffer a device buffer and, optionally, stream bookkeeping. */
static tSndSoundBufferElem* make_sbe(int idx, int withMisc, int format)
{
    tSndSoundBufferElem* sbe = &fx_buffers[idx];
    DSBUFFERDESC desc;
    WAVEFORMATEX fmt;

    memset(&desc, 0, sizeof(desc));
    memset(&fmt, 0, sizeof(fmt));
    fmt.wFormatTag = 1;
    fmt.nChannels = (WORD)format;
    desc.dwSize = 36;
    desc.lpwfxFormat = &fmt;
    desc.dwBufferBytes = 4096;
    wsnd_device()->lpVtbl->CreateSoundBuffer(wsnd_device(), &desc, &sbe->dsndSBIF, 0);
    sbe->format = format;
    sbe->unk2SetTo64InCreate = 64;
    /* The engine gives every buffer these two in SB_Create, and several
       paths dereference them without checking. */
    sbe->dsndSBIF->lpVtbl->QueryInterface(sbe->dsndSBIF, &IID_IDirectSound3DBuffer,
                                          (void**)&sbe->dsnd3dbufferIF);
    sbe->dsndSBIF->lpVtbl->QueryInterface(sbe->dsndSBIF, &IID_IKsPropertySet,
                                          (void**)&sbe->dsndIKPropertySetIF);
    if (withMisc)
    {
        tSbeMiscData* m = &fx_misc[idx];
        memset(m, 0, sizeof(*m));
        m->fileHandle = (HANDLE)(size_t)(0x9000 + idx);
        m->sizeToUpload = 2048;
        m->soundBufferSize = 8192;
        m->fullSize = 40000;
        m->distanceToMove = 128;
        m->eventHandle1 = (HANDLE)(size_t)(10 + idx);
        m->eventHandle2 = (HANDLE)(size_t)(20 + idx);
        m->dsndEventsArrayIdx = idx & 3;
        sbe->miscData = m;
    }
    else
    {
        sbe->miscData = 0;
    }
    return sbe;
}

/* ------------------------------------------------------------- cases */

#define GROUPS 14
#define SUBCASES 96

int SIDE_CASES(void) { return GROUPS * SUBCASES; }

void SIDE_RUN(int c)
{
    int group = c / SUBCASES;
    int sub = c % SUBCASES;
    unsigned seed = (unsigned)(c * 2654435761u);
    int i;
    int a, b;
    int status;

    fixture_reset(seed);

    switch (group)
    {
    case 0:
    {
        /* the size arithmetic, swept over both channel counts */
        unsigned bytes = (unsigned)(sub * 1237 + 1);
        unsigned ch = (unsigned)(1 + (sub & 1));
        d("calc %u %u -> %u %u %u\n", bytes, ch,
          pcSND_CalcSizeToReadByFormat(bytes, ch),
          pcSND_calcBlockNumForDecompress((int)bytes, (int)ch),
          pcSND_calcMemoryNeededForTheSound(bytes, (int)ch));
        d("calc0 -> %u %u\n", pcSND_CalcSizeToReadByFormat(0, ch),
          pcSND_calcMemoryNeededForTheSound(0, (int)ch));
        d("return160=%d\n", pcSND_Return160());
        break;
    }

    case 1:
    {
        /* the play list: grow it, fill it, take entries out again */
        tSndSoundBufferElem* sbe;
        SND_gc_NoSound = 0;
        pcSND_l_Init(&fx_handles);
        int made = (sub % 7) + 1;
        for (i = 0; i < made; ++i)
        {
            sbe = make_sbe(i & 7, 1, 1 + (i & 1));
            sndCurrSoundBufferElem = sbe;
            pcSND_AddSoundToThePlayList(sub & 1);
        }
        dump_globals();
        if (sub & 2)
            pcSND_RemoveSoundFromPlayList(&fx_buffers[(sub / 4) % made], sub & 4);
        if (sub & 8)
            pcSND_RemoveSoundFromPlayList((tSndSoundBufferElem*)-1, sub & 16);
        if (sub & 32)
            pcSND_StopSoundAndRemoveFromThePlayList(&fx_buffers[0], 1);
        dump_globals();
        break;
    }

    case 2:
    {
        /* the volume/pan ladder, over the whole balance and pan range */
        tSndSoundBufferElem* sbe = make_sbe(0, 0, 1 + (sub & 1));
        int pan = (sub - 48) * 220;
        sbe->unk2SetTo64InCreate = sub + 16;
        SND_gst_Params.soundRenderMode = 1 + (sub % 3);
        gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 = -100 * (sub % 11);
        pcSND_SB_SetPan(sbe, pan, sub & 7);
        dump_sbe("setpan", sbe);
        pcSND_UnknownSoundPanAndVolumeSetByUnknown(sbe, pan);
        dump_sbe("unkpan", sbe);
        pcSND_SB_SetVolume(sbe, -50 * (sub % 23));
        dump_sbe("setvol", sbe);
        pcSND_SB_GetPan(sbe, &a, &b);
        d("getpan %d %d\n", a, b);
        pcSND_SB_SetFrequency(sbe, (sub - 8) * 30000);
        pcSND_SB_SetCurrPos(sbe, sub * 97);
        pcSND_3DSoundBufferSetPos(sbe, (sub - 48) * 240);
        break;
    }

    case 3:
    {
        /* the whole-list render-mode walk */
        for (i = 0; i < 5; ++i)
        {
            tSndSoundBufferElem* sbe = make_sbe(i, 0, 1 + (i & 1));
            sbe->unk2SetTo64InCreate = (sub * 5 + i * 13) & 127;
            BAS_binsert((unsigned int)(size_t)sbe, (int)(size_t)sbe, &dsndSBList);
        }
        pcSND_i_ChangeRenderMode(1 + (sub % 4));
        dump_globals();
        for (i = 0; i < 5; ++i)
            dump_sbe("mode", &fx_buffers[i]);
        break;
    }

    case 4:
    {
        /* the raster accounting, over every file-id arm */
        static const int ids[6] = { 8, 0x2000, 0x4000, 0x80000, 0x40000, 0 };
        tSndSoundBufferElem* sbe = make_sbe(0, 0, 1);
        SND_gst_Params.soundEnabled = (sub & 32) ? 1 : 0;
        for (i = 0; i < 6; ++i)
        {
            SND_gi_RasterCurrentFileId = ids[(sub + i) % 6];
            pcSND_AddToSNDRaster(sbe->dsndSBIF, 100 * (i + 1) + sub);
        }
        dump_globals();
        pcSND_SNDRasterRelease(sbe->dsndSBIF);
        dump_globals();
        pcSND_SNDRasterRelease(sbe->dsndSBIF);
        dump_globals();
        break;
    }

    case 5:
    {
        /* buffer creation from a wav header, down every read path */
        tSndWavHdrStruct hdr;
        tSndSoundBufferElem* made;

        memset(&hdr, 0, sizeof(hdr));
        hdr.fullSoundSize = 72 * (1 + (sub % 9));
        hdr.dataStartPos = 16;
        hdr.wfex.wFormatTag = (sub & 64) ? 1 : 2;
        hdr.wfex.nChannels = (WORD)(1 + (sub & 1));
        hdr.wfex.nSamplesPerSec = 22050;
        hdr.wfex.nBlockAlign = (WORD)(36 * (1 + (sub & 1)));
        hdr.wfex.wBitsPerSample = 4;
        hdr.wfex.cbSize = 32;
        LOA_gb_SpeedMode = (sub & 2) ? 1 : 0;
        wsnd_loa_speedmode = (sub & 4) ? 1 : 0;
        wsnd_create_fail_mod = (sub & 8) ? 1 : 0;
        wsnd_lock_fail_mod = (sub & 16) ? 1 : 0;
        wsnd_lock_tail = (sub & 32) ? 64 : 0;
        made = pcSND_pst_SB_Create(&fx_handles, &hdr, (sub % 5) ? 4096 : 0);
        dump_hdr("create", &hdr);
        dump_sbe("create", made);
        dump_globals();
        break;
    }

    case 6:
    {
        /* the reload twin, driven over the same grid */
        tSndWavHdrStruct hdr;
        tSndSoundBufferElem* made;
        tSndSoundBufferElem* old = make_sbe(0, 0, 1);

        memset(&hdr, 0, sizeof(hdr));
        hdr.fullSoundSize = 72 * (1 + (sub % 9));
        hdr.wfex.wFormatTag = (sub & 64) ? 1 : 2;
        hdr.wfex.nChannels = (WORD)(1 + (sub & 1));
        hdr.wfex.nSamplesPerSec = 44100;
        hdr.wfex.nBlockAlign = (WORD)(36 * (1 + (sub & 1)));
        hdr.wfex.cbSize = 16;
        LOA_gb_SpeedMode = (sub & 2) ? 1 : 0;
        wsnd_loa_speedmode = (sub & 4) ? 1 : 0;
        wsnd_create_fail_mod = (sub & 8) ? 1 : 0;
        wsnd_lock_fail_mod = (sub & 16) ? 1 : 0;
        wsnd_lock_tail = (sub & 32) ? 48 : 0;
        BAS_binit(&dsndSBList, 16);
        made = pcSND_SB_Reload(&fx_handles, old, &hdr, (sub % 5) ? 2048 : 0);
        dump_hdr("reload", &hdr);
        dump_sbe("reload", made);
        dump_globals();
        break;
    }

    case 7:
    {
        /* the streaming path: fill, decode, notify, reinit */
        tSndSoundBufferElem* sbe = make_sbe(0, 1, 1 + (sub & 1));
        uint32_t decoded = 0;
        int16_t scratch[2048];

        SND_gc_NoSound = 0;
        pcSND_l_Init(&fx_handles);
        sbe->miscData->readThroughWithMultipleLoop = (sub & 2) ? 1 : 0;
        sbe->miscData->fullSize = (unsigned)(1200 * (1 + (sub % 5)));
        sbe->miscData->sumSize = 120 * (sub % 7);
        sbe->miscData->unk6 = 120 * (sub % 3);
        SND_gst_Params.soundRenderMode = (sub & 4) ? 1 : 2;
        wsnd_read_bytes = 36 * (1 + (sub % 11));
        /* decoded comes out a multiple of 120, so this is a value the
           short-read test can actually land on. */
        sbe->miscData->sizeToUpload = 120 * (1 + (sub % 11));
        if (sbe->miscData->readThroughWithMultipleLoop)
        {
            /* The refill loop only ends when each pass decodes a whole
               number of ADPCM frames; see docs -- with a short tail it
               spins forever, in both copies. */
            sbe->miscData->soundBufferSize = 1920;
            sbe->miscData->sizeToUpload = 480;
            sbe->miscData->fullSize = (unsigned)(240 * (1 + (sub % 7)));
            sbe->miscData->sumSize = 0;
            sbe->miscData->unk6 = 0;
            wsnd_read_bytes = 1 << 20;
        }
        memset(scratch, 0, sizeof(scratch));
        pcSND_ReadAndDecompressSoundData((unsigned)(720 * (1 + (sub % 4))), sbe,
                                         (sub & 8) ? 1 : 0, &decoded, scratch);
        d("decoded=%u fp=%08X\n", decoded, (unsigned)(scratch[0] + scratch[1] * 3 + scratch[64] * 7));
        dump_sbe("stream", sbe);
        pcSND_fillSoundBuffer(0, sbe->miscData, sbe);
        dump_sbe("fill", sbe);
        wsnd_play_pos = 37 * sub;
        pcSND_updateCurrentPlayPositionAndSize(sbe);
        dump_sbe("upd", sbe);
        pcSND_SB_GetCurrPos(sbe, &a, &b);
        d("currpos %d %d\n", a, b);
        break;
    }

    case 8:
    {
        /* reinit and play, plus the notify table */
        tSndSoundBufferElem* sbe = make_sbe(0, 1, 1 + (sub & 1));

        SND_gc_NoSound = 0;
        pcSND_l_Init(&fx_handles);
        sbe->miscData->vmilenEventHandlptr_Sub49e950 = 0;
        sbe->dsndSBIF->lpVtbl->QueryInterface(sbe->dsndSBIF, &IID_IDirectSoundNotify,
                                              (void**)&sbe->miscData->vmilenEventHandlptr_Sub49e950);
        sbe->miscData->sizeToUpload = 256 * (1 + (sub % 5));
        gDoSoundGlobal.dword_B303C0 = sub % 5;
        wsnd_read_bytes = 36 * (1 + (sub % 9));
        if (sub & 1)
        {
            sbe->miscData->sizeToUpload = 480;
            sbe->miscData->soundBufferSize = 1920;
            wsnd_read_bytes = 1 << 20;
        }
        pcSND_StreamReinitAndPlay(sbe, 720 * (1 + (sub % 6)), 4096, 360 * (sub % 8),
                                  11, (char)(sub & 3), 22050 + sub, (sub - 48) * 200,
                                  -100 * (sub % 9));
        dump_sbe("reinit", sbe);
        dump_globals();
        pcSND_SetPositionNotify(sbe);
        pcSND_SB_Pause(sbe);
        dump_sbe("pause", sbe);
        break;
    }

    case 9:
    {
        /* the FX layer: presets, the mode ladders, enable and disable */
        EAXLISTENERPROPERTIES props;
        int mode;
        static const int modes[16] = {
            -3, 0, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13,
            65536, 65537, 65540, 65545
        };

        memset(&props, 0xCD, sizeof(props));
        pcSND_fxStructSet(&props, sub % 14 - 1);
        d("preset %d env=%u size=%08X dif=%08X room=%ld hf=%ld lf=%ld\n",
          sub % 14 - 1, props.ulEnvironment, fbits(props.flEnvironmentSize),
          fbits(props.flEnvironmentDiffusion), (long)props.lRoom, (long)props.lRoomHF,
          (long)props.lRoomLF);
        d("preset dt=%08X dhf=%08X dlf=%08X refl=%ld rd=%08X rev=%ld vd=%08X\n",
          fbits(props.flDecayTime), fbits(props.flDecayHFRatio), fbits(props.flDecayLFRatio),
          (long)props.lReflections, fbits(props.flReflectionsDelay), (long)props.lReverb,
          fbits(props.flReverbDelay));
        d("preset et=%08X ed=%08X mt=%08X md=%08X ah=%08X hr=%08X lr=%08X rr=%08X fl=%u\n",
          fbits(props.flEchoTime), fbits(props.flEchoDepth), fbits(props.flModulationTime),
          fbits(props.flModulationDepth), fbits(props.flAirAbsorptionHF),
          fbits(props.flHFReference), fbits(props.flLFReference),
          fbits(props.flRoomRolloffFactor), props.ulFlags);
        d("preset pans %08X %08X %08X %08X %08X %08X\n",
          fbits(props.vReverbPan.x), fbits(props.vReverbPan.y), fbits(props.vReverbPan.z),
          fbits(props.vReflectionsPan.x), fbits(props.vReflectionsPan.y),
          fbits(props.vReflectionsPan.z));

        wsnd_reg_fail = (sub & 1) ? 1 : 0;
        wsnd_reg_fx_value = (sub & 2) ? 1 : 2;
        pcSND_i_FxInit();
        dump_globals();
        gFXGlobal.fxEnabled = (sub & 4) ? 1 : 0;
        mode = modes[sub % 16];
        fx_params.mode = mode;
        pcSND_CreateSoundBuffer(make_sbe(5, 0, 1)->dsndSBIF, wsnd_device());
        a = pcSND_i_FxSetMode(&fx_params, sub % 12);
        b = pcSND_i_FxGetMode(&fx_params);
        d("fx mode=%d set=%d get=%d\n", mode, a, b);
        fx_params.mode = mode;
        d("fb set=%d\n", pcSND_i_FxSetFeedback(&fx_params, sub));
        fx_params.mode = mode;
        d("fb get=%d\n", pcSND_i_FxGetFeedback(&fx_params));
        fx_params.mode = mode;
        d("dl set=%d\n", pcSND_i_FxSetDelay(&fx_params, sub));
        fx_params.mode = mode;
        d("dl get=%d\n", pcSND_i_FxGetDelay(&fx_params) != 0);
        fx_params.mode = mode;
        d("wv set=%d\n", pcSND_i_FxSetWetVolume(&fx_params, -100 * (sub % 7)));
        fx_params.mode = mode;
        a = pcSND_i_FxGetWetVolume(&fx_params);
        d("wv get=%d wet=%08X\n", a, fbits(fxWetVol));
        a = pcSND_i_FxSetWetPan(&fx_params, sub);
        b = pcSND_i_FxGetWetPan(&fx_params);
        d("wp set=%d get=%d\n", a, b);
        for (i = 0; i < 4; ++i)
            fx_fxSlots[i * 3] = make_sbe(i, 0, 1);
        pcSND_i_FxEnable(&fx_params);
        dump_fx("en", &fx_params);
        pcSND_i_FxDisable(&fx_params);
        dump_fx("dis", &fx_params);
        pcSND_DisableReverbIf_i7_not_match_with_idx(&fx_params, fx_fxSlots[3]);
        dump_fx("rev", &fx_params);
        pcSND_EAXListenerPropertiesSet(sub % 12);
        pcSND_FxClose();
        dump_globals();
        break;
    }

    case 10:
    {
        /* the stream and track handle APIs */
        int handle = sub % (STREAMS + 2);
        tSndSoundBufferElem* key = (tSndSoundBufferElem*)(size_t)handle;

        status = -1;
        a = pcSND_Stream_GetStatus(key, &status);
        d("sgs=%d st=%d\n", a, status);
        d("spl=%d\n", pcSND_Stream_Play(key, sub));
        pcSND_Stream_SetVolume(key, -10 * sub);
        pcSND_Stream_SetPan(key, sub, -sub);
        pcSND_Stream_SetCurrPos(key, sub * 3);
        d("sfq=%d\n", pcSND_Stream_SetFrequency(key, 1000 + sub));
        a = b = -1;
        pcSND_Stream_GetPan(key, &a, &b);
        d("sgp %d %d\n", a, b);
        a = b = -1;
        pcSND_Stream_GetCurrPos(key, &a, &b);
        d("sgc %d %d\n", a, b);
        a = -1;
        pcSND_Stream_GetFrequency(key, &a);
        d("sgf %d\n", a);
        {
            tSndSoundBufferElem* dup = 0;
            pcSND_Stream_Duplicate(&fx_handles, key, &dup);
            d("sdup %ld\n", (long)(size_t)dup);
        }
        d("srel=%d\n", pcSND_Stream_Release(key));
        for (i = 0; i < STREAMS; ++i)
            d("  S%d %d %d %d %d %d %d\n", i, fx_streams[i].num, fx_streams[i].vol,
              fx_streams[i].pan1, fx_streams[i].pan2, fx_streams[i].freq,
              fx_streams[i].currPos);

        BAS_binit(&SND_gst_TrackLookUpTable, 16);
        for (i = 0; i < TRACKS; ++i)
        {
            make_sbe(i, 0, 1 + (i & 1));
            BAS_binsert((unsigned int)(1000 + i), i, &SND_gst_TrackLookUpTable);
        }
        {
            tSndSoundBufferElem* tk = (tSndSoundBufferElem*)(size_t)(1000 + (sub % (TRACKS + 1)));
            status = -1;
            a = pcSND_iTrack_GetStatus(tk, &status);
            d("tgs=%d st=%d\n", a, status);
            d("tpl=%d\n", pcSND_iTrack_Play(tk, sub & 1));
            a = -1;
            pcSND_Track_GetFrequency(tk, &a);
            d("tgf %d\n", a);
            a = b = -1;
            pcSND_Track_GetPan(tk, &a, &b);
            d("tgp %d %d\n", a, b);
            a = b = -1;
            pcSND_Track_GetCurrPos(tk, &a, &b);
            d("tgc %d %d\n", a, b);
            pcSND_Track_SetFrequency(tk, 30000 + sub);
            pcSND_Track_SetPan(tk, sub - 48, 48 - sub);
            pcSND_Track_SetCurrPos(tk, sub);
            pcSND_Track_SetVolume(tk, -100 * (sub % 13));
            pcSND_Track_Pause(tk);
            pcSND_Track_Stop(tk);
            pcSND_Track_Stop(0);
            pcSND_Track_Release(tk);
        }
        for (i = 0; i < INSTANCES; ++i)
            d("  I%d fl=%X\n", i, fx_instances[i].flags);
        break;
    }

    case 11:
    {
        /* the 3D volume and the doppler shift */
        tSoundInstanceElem* inst = &fx_instances[sub % INSTANCES];
        Vec3 speed;

        inst->soundBufElemFromModel = make_sbe(0, 0, 1);
        inst->flags = (int)seed | ((sub & 1) ? 0x10000000 : 0x1000000);
        if (sub & 2) inst->flags |= 0x8000000;
        else inst->flags &= ~0x8000000;
        if (sub & 4) inst->flags |= 0x2000000;
        if (sub & 8) inst->flags |= 0x4000000;
        SND_gst_Params.soundRenderMode = (sub & 16) ? 8 : ((sub & 32) ? 16 : 2);
        SND_gst_Params.microPosMtxPtr = (sub & 64) ? 0 : &fx_micro;
        SND_gst_Params.f_surround = 0.25f * (float)((sub % 9) - 4);
        inst->minPan = 0.25f * (float)((sub % 7) - 3);
        inst->sndInstPosVec.x = 0.5f * (float)((sub % 11) - 5);
        inst->sndInstPosVec.y = 0.25f * (float)((sub % 13) - 6);
        inst->sndInstPosVec.z = 0.125f * (float)((sub % 17) - 8);
        {
            float computed = SND_f_Compute3DVol(inst);
            d("vol=%08X pan=%d front=%d\n", fbits(computed), inst->pan, inst->front);
        }

        speed.x = 0.5f * (float)((sub % 5) - 2);
        speed.y = 0.25f * (float)((sub % 7) - 3);
        speed.z = 0.125f * (float)((sub % 3) - 1);
        inst->sndPosMtxSetInSetPos = (sub & 1) ? &fx_gaoMtx : 0;
        inst->gao = (sub & 2) ? &fx_gao : 0;
        fx_dyn = (sub & 4) ? 1 : 0;
        fx_ext.snd = (sub & 8) ? (struct sSndBank*)(size_t)0x7777 : 0;
        fx_gao.flags = (sub & 16) ? 0x8000000 : 0;
        SND_ComputeDoppler(&speed, &SND_gst_Params.sndMatrixRef->Pos, inst);
        d("prev %08X %08X %08X\n", fbits(inst->sndInstPosVecPrevious.x),
          fbits(inst->sndInstPosVecPrevious.y), fbits(inst->sndInstPosVecPrevious.z));
        pcSND_ShutDownStreams();
        pcSND_SetGlobalVol((sub - 48) * 400);
        a = pcSND_l_GetGlobalVol();
        b = pcSND_l_GetGlobalPan();
        d("gvol=%d gpan=%d\n", a, b);
        pcSND_SetGlobalPan(sub);
        break;
    }

    case 12:
    {
        /* init, the sound thread's dispatch, tracks and teardown */
        int script[8];
        tSndSoundBufferElem* sbe;

        SND_gc_NoSound = (sub & 64) ? 1 : 0;
        wsnd_reg_fail = (sub & 1) ? 1 : ((sub & 2) ? 2 : 0);
        wsnd_eax_support = (sub & 4) ? 1 : 3;
        d("init=%d\n", pcSND_l_Init(&fx_handles));
        dump_globals();

        sbe = make_sbe(0, 1, 1 + (sub & 1));
        sbe->miscData->fullSize = 2400;
        sbe->miscData->currSize = (unsigned)(600 * (sub % 5));
        sbe->miscData->RemoveAfterPlayFinished = (sub & 16) ? 1 : 0;
        sbe->miscData->readThroughWithMultipleLoop = (sub & 32) ? 1 : 0;
        if (sbe->miscData->readThroughWithMultipleLoop)
        {
            sbe->miscData->sizeToUpload = 1920;
            sbe->miscData->soundBufferSize = 7680;
            sbe->miscData->fullSize = 480;
            sbe->miscData->sumSize = 0;
        }
        sndCurrSoundBufferElem = sbe;
        if (sndEventsNum)
        {
            pcSND_AddSoundToThePlayList(0);
            /* The refill arms first: index 1 and index 3 take the
               sound back out of the play list, and the arm that follows
               would then walk a cleared slot. */
            /* A refill arm can decide the sound is finished and take it
               out of the play list, so the slot is refilled (index 0)
               before the next one walks it; index 1 and index 3, which
               empty it for good, come last. */
            script[0] = 4 + ((sub >> 2) & 1);
            script[1] = (int)0xFFFFFFFF;
            script[2] = 0;
            script[3] = 4 + ((sub >> 2) & 1);
            script[4] = 0;
            script[5] = 4 + ((sub >> 2) & 1);
            script[6] = 1;
            script[7] = 3;
            wsnd_thread_script(script, 8);
            wsnd_read_bytes = 36 * (1 + (sub % 7));
            wsnd_run_thread(pcSND_SoundThread);
            dump_sbe("thread", sbe);
            dump_globals();
        }

        {
            tSndSoundBufferElem* one = 0;
            tSndSoundBufferElem* two = 0;
            pcSND_CreateTrackSB(&one, &two);
            dump_sbe("trackA", one);
            dump_sbe("trackB", two);
            if (one && two)
                pcSND_ReleaseTrackSB(one, two);
        }
        make_sbe(1, 0, 1);
        make_sbe(2, (sub & 2) ? 1 : 0, 2);
        make_sbe(3, 0, 1);
        pcSND_SB_Duplicate(&fx_handles, &fx_buffers[1], &sbe);
        dump_sbe("dup", sbe);
        BAS_binsert((unsigned int)(size_t)&fx_buffers[1], (int)(size_t)&fx_buffers[1], &dsndSBList);
        pcSND_SB_Release(&fx_buffers[1]);
        pcSND_ReleaseSoundBuffer(&fx_buffers[2]);
        pcSND_D3DSoundBufAAndIKPopertySetRelease(&fx_buffers[3]);
        {
            int positions[16];
            int sizes[16];
            for (i = 0; i < 16; ++i) { positions[i] = i + 1; sizes[i] = i; }
            pcSND_StreamPrefetchArray(positions, sizes);
            for (i = 0; i < 16; ++i)
                d("  pf%d=%d\n", i, positions[i]);
            pcSND_StreamPrefetchArray(0, sizes);
            pcSND_StreamPrefetch();
            pcSND_StreamFlush();
            pcSND_StreamGetMonoToStereo(&sbe, &sbe);
            pcSND_StreamGetStereoToMono();
            pcSND_Stream_Pause(0);
            pcSND_Stream_Stop(0);
            pcSND_Null(&SND_gst_Params.sndMatrixRef->Pos);
        }
        pcSND_Cleanup();
        dump_globals();
        if (sndEventsNum == 0 && !SND_gc_NoSound)
            d("cleaned\n");
        break;
    }

    case 13:
    {
        /* The doppler ratio, swept through its clamps on purpose. The
           closing speed passes through zero as fx_DopplerSpeed walks, so
           the ratio spans the whole range either side of both clamps --
           which is the only way the 1000.0 cap is ever reached. */
        tSoundInstanceElem* inst = &fx_instances[sub % INSTANCES];
        Vec3 speed;

        inst->soundBufElemFromModel = make_sbe(0, 0, 1);
        inst->sndPosMtxSetInSetPos = 0;
        inst->gao = 0;
        inst->sndPositionSetInSetPosPtr = &inst->sndInstPosVec;
        inst->sndInstPosVec.x = 3.0f;
        inst->sndInstPosVec.y = 4.0f;
        inst->sndInstPosVec.z = 0.0f;
        inst->sndInstPosVecPrevious.x = 2.5f;
        inst->sndInstPosVecPrevious.y = 3.0f;
        inst->sndInstPosVecPrevious.z = 0.0f;
        inst->freq = (sub < 48) ? 100 : 300000;
        SND_gst_Params.sndMatrixRef->Pos.x = 0.0f;
        SND_gst_Params.sndMatrixRef->Pos.y = 0.0f;
        SND_gst_Params.sndMatrixRef->Pos.z = 0.0f;
        SND_gst_Params.fx_doppler = 1.0f + 0.25f * (float)(sub % 4);
        /* closing = fx_DopplerSpeed * 5 - 11, so this walks it through
           zero and the ratio sweeps both the 1000.0 cap and the -1.0
           clamp. */
        SND_gst_Params.fx_DopplerSpeed = 2.2f + 0.00390625f * (float)(sub - 48);
        TIM_gf_dt = 0.0166666f;
        speed.x = 1.0f;
        speed.y = 2.0f;
        speed.z = 0.0f;
        SND_ComputeDoppler(&speed, &SND_gst_Params.sndMatrixRef->Pos, inst);
        d("dop s=%08X d=%08X\n", fbits(SND_gst_Params.fx_DopplerSpeed),
          fbits(SND_gst_Params.fx_doppler));
        break;
    }
    }

    dump_globals();
    d("heap=%d\n", wsnd_heap_used());
}

/* The guard distribution check: every guard a case derives from its
   sub-index has to be taken both ways somewhere in the sweep, or the
   branch under it never runs and a defect in it scores clean. The
   expressions below are copied from the cases above; sub runs 0..95, so a
   mask above 64 is dead and this is what says so. */
typedef struct sGuard { const char* name; int zero; int one; } tGuard;

#define G(ix, expr) do { if (expr) guards[ix].one++; else guards[ix].zero++; } while (0)

void SIDE_RUN_GUARDS(void)
{
    static tGuard guards[32];
    int sub, i, bad = 0;
    static const char* names[32] = {
        "g1.notify", "g1.remove", "g1.fireEvent", "g1.removeAll", "g1.stopAll",
        "g2.format", "g2.renderMode1", "g2.renderMode2",
        "g4.soundEnabled",
        "g5.speedMode", "g5.binFile", "g5.createFail", "g5.lockFail",
        "g5.lockTail", "g5.formatTag", "g5.zeroOffset",
        "g7.loop", "g7.renderMode1", "g7.firstBuffer",
        "g8.bigRead",
        "g9.regFail", "g9.fxValue", "g9.fxEnabled",
        "g11.spheric", "g11.pan3d", "g11.mode8", "g11.mode16", "g11.noMicro",
        "g11.gaoMtx", "g11.gao", "g11.dyn",
        "g12.noSound"
    };

    for (i = 0; i < 32; ++i)
        guards[i].name = names[i];

    for (sub = 0; sub < SUBCASES; ++sub)
    {
        G(0, sub & 1);
        G(1, sub & 2);
        G(2, sub & 4);
        G(3, sub & 8);
        G(4, sub & 32);
        G(5, sub & 1);
        G(6, (1 + (sub % 3)) == 1);
        G(7, (1 + (sub % 3)) == 2);
        G(8, sub & 32);
        G(9, sub & 2);
        G(10, sub & 4);
        G(11, sub & 8);
        G(12, sub & 16);
        G(13, sub & 32);
        G(14, sub & 64);
        G(15, (sub % 5) == 0);
        G(16, sub & 2);
        G(17, sub & 4);
        G(18, sub & 8);
        G(19, sub & 1);
        G(20, sub & 1);
        G(21, sub & 2);
        G(22, sub & 4);
        G(23, sub & 1);
        G(24, sub & 2);
        G(25, sub & 16);
        G(26, (sub & 16) == 0 && (sub & 32));
        G(27, sub & 64);
        G(28, sub & 1);
        G(29, sub & 2);
        G(30, sub & 4);
        G(31, sub & 64);
    }

    for (i = 0; i < 32; ++i)
    {
        printf("guard %-18s %3d zero %3d one%s\n", guards[i].name,
               guards[i].zero, guards[i].one,
               (guards[i].zero == 0 || guards[i].one == 0) ? "   DEAD" : "");
        if (guards[i].zero == 0 || guards[i].one == 0)
            ++bad;
    }
    printf("%d dead guard(s)\n", bad);
}
