/* One side of the sound.c differential test.
 *
 * This file is compiled twice, once against the rewrite and once against
 * the frozen copy in baseline_sound.c, with every sound.c symbol and this
 * file's two entry points renamed by -D. Both sides therefore drive their
 * own copy of the engine's globals over the same mock device, and the
 * harness compares the text each side dumps plus the mock call log.
 *
 * Pointers into the mock heap are directly comparable because the arena is
 * rewound between the two runs; pointers into a side's own globals are not,
 * so they are normalised to an index before being printed.
 */

#include <string.h>
#include <stdio.h>

#include "sound_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../time.h"
#include "../math.h"
#include "../gameobject.h"
#include "../modifiers/gensoundmodifier.h"
#include "../win/winsound.h"

/* sound.c defines these and no header declares them; without the
   prototypes the side file would take the pointer-returning ones as
   returning int and truncate the result. */
extern int i_init_521;

void SND_CloseMdFModule(void);
void SND_DeleteOldLoadingSound(void);
void SND_DetachLoadingInstance(void);
void SND_DetachSound(int soundIdx);
void SND_FadeUpdate(void);
void SND_FamillyCloseModule(void);
void SND_FamillyUpdate(void);
void SND_FxCloseModule(void);
void SND_LoadingSoundCloseModule(void);
void SND_LoadingSoundInitModule(void);
void SND_LoadingSoundNotifyBeginLoading(void);
void SND_LoadingSoundNotifyEndLoading(void);
void SND_LoadingSoundUpdate(void);
void SND_MergeBank(tGameObjectMain* dest, tGameObjectMain* src, int firstIdx,
                   int lastIdx, int overwrite);
void SND_RasterCloseModule(void);
void SND_ReadRIFFHeader(char** cursor, void* hdrDest);
int SND_ReadSNDHeaderElem(char** cursor, tSoundHeaderGeneric* header);
int SND_SignalChangeLanguageToMdF(void);
int SND_SoundListMake(tSndList** list, int* count);
void SND_TrackDestroy(unsigned int trackKey);
void SND_TrackForceFreeResources(int keepIdx);
void SND_TrackFreeList(void);
int SND_TrackInitModule(void);
int SND_StreamInitModule(void);
BOOL SND_b_IsLoadingInstance(int instanceIdx);
int SND_b_SndIsInActiveVolume(int instanceIdx);
float SND_f_GetInstVolume(tSoundInstanceElem* inst);
float SND_f_MuteComputeFactor(tSoundInstanceElem* inst);
int SND_i_ChangeRenderMode(int mode);
int SND_i_FxInitModule(void);
int SND_l_GetUnusedInstance(void);
tSoundFamilyStruct* SND_pst_GetOrArray(int mask, int bits);
tSndBank* SND_ul_CallbackLoadMetaBank(int Offset);

#ifndef SIDE_RUN
#define SIDE_RUN snd_side_run
#endif
#ifndef SIDE_CASES
#define SIDE_CASES snd_side_cases
#endif

#define GROUPS 18
#define SUBCASES 96

int SIDE_CASES(void) { return GROUPS * SUBCASES; }

/* ------------------------------------------------------------ dumping */

static void d(const char* fmt, ...);

#include <stdarg.h>
static void d(const char* fmt, ...)
{
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    snd_log("%s", buf);
}

static unsigned fbits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

static int intf_id(const void* p)
{
    const tSndInterfaceStruct* base = &SND_gpst_Interface.win;
    const tSndInterfaceStruct* q = (const tSndInterfaceStruct*)p;
    if (!p) return -1;
    if (q >= base && q < base + 6) return (int)(q - base);
    if (p == (const void*)&SND_gpst_Interface) return 100;
    return -2;
}

static int fader_id(const void* p)
{
    const tMutesSetupStruct* q = (const tMutesSetupStruct*)p;
    if (!p) return -1;
    if (q >= SND_Mutes_B30600_fadergroups && q < SND_Mutes_B30600_fadergroups + 12)
        return (int)(q - SND_Mutes_B30600_fadergroups);
    return -2;
}

static long heap_off(const void* p)
{
    const unsigned char* b = snd_heap_base();
    if (!p) return -1;
    if ((const unsigned char*)p < b || (const unsigned char*)p >= b + 24 * 1024 * 1024)
        return -2;
    return (long)((const unsigned char*)p - b);
}

static void dump_faders(void)
{
    int i;
    for (i = 0; i < 12; ++i)
    {
        tMutesSetupStruct* f = &SND_Mutes_B30600_fadergroups[i];
        d("F%d en=%d s1=%08X s2=%08X fl=%d el=%08X i6=%08X i7=%08X t=%08X\n",
          i, f->muteEnabled, fbits(f->muteSetUp1), fbits(f->muteSetUp2), f->flags,
          fbits(f->elapsedTime), fbits(f->i6), fbits(f->i7),
          fbits(f->muteSetUp2ToSetWithThis));
    }
}

static void dump_sounds(void)
{
    int i;
    d("sounds max=%d ptr=%ld\n", SND_gst_Params.allocatedSoundsMaxNum,
      heap_off(SND_gst_Params.allocatedSounds));
    for (i = 0; i < SND_gst_Params.allocatedSoundsMaxNum && i < 32; ++i)
    {
        tSndSoundElem* s = &SND_gst_Params.allocatedSounds[i];
        d("S%d k=%d fl=%08X mf=%08X use=%d buf=%ld freq=%d ch=%d bits=%d intf=%d "
          "mdf=%ld start=%d len=%u rate=%u align=%d extra=%d fmt=%d\n",
          i, s->soundKey, s->flags, s->muteFacorFlags, s->usageCnt,
          heap_off(s->sndSndBufElem), s->freq, s->numChannels, s->bitsPerSample,
          intf_id(s->sndInterface), heap_off(s->soundModifier), s->startDataPos,
          s->fullsoundLen, s->byteRate, s->blockAlign, s->extraDataSize, s->format);
    }
}

static void dump_instances(void)
{
    int i;
    d("inst num=%d max=%d ptr=%ld\n", SND_gst_Params.soundInstanceNum,
      SND_gst_Params.soundInstanceMaxNum, heap_off(SND_gst_Params.soundInstances));
    for (i = 0; i < SND_gst_Params.soundInstanceNum && i < 32; ++i)
    {
        tSoundInstanceElem* e = &SND_gst_Params.soundInstances[i];
        d("I%d s=%d fl=%08X mf=%08X fam=%08X trk=%d freq=%d vol=%08X front=%d pan=%d "
          "minPan=%08X n=%08X,%08X,%08X f=%08X,%08X,%08X dt=%08X c=%08X,%08X,%08X "
          "fc=%08X mc=%08X mtx=%ld pos=%ld ts=%08X gao=%ld intf=%d mute=%d mdf=%ld "
          "rem=%08X def=%08X\n",
          i, e->allocatedSoundsIdx, e->flags, e->muteFactorFlags, e->familyBitmask,
          e->trackID, e->freq, fbits(e->volume), e->front, e->pan, fbits(e->minPan),
          fbits(e->nearArr[0]), fbits(e->nearArr[1]), fbits(e->nearArr[2]),
          fbits(e->farArr[0]), fbits(e->farArr[1]), fbits(e->farArr[2]),
          fbits(e->dtFar), fbits(e->centerArr[0]), fbits(e->centerArr[1]),
          fbits(e->centerArr[2]), fbits(e->farCoefficient), fbits(e->middleCoefficient),
          heap_off(e->sndPosMtxSetInSetPos), heap_off(e->sndPositionSetInSetPosPtr),
          fbits(e->requestTimeStamp), heap_off(e->gao), intf_id(e->soundIntf),
          fader_id(e->unkMuteSetupSeeInitVolTable), heap_off(e->modifierInstances),
          fbits(e->timeRemainTillStart), fbits(e->timeFromDefault));
    }
}

static void dump_tracks(void)
{
    int i;
    d("tracks nb=%d list=%ld lastVirtual=%d ids=%d lut=%d\n", SND_gi_TrackNb,
      heap_off(SND_gax_TrackPlayingList), SND_gi_TrackLastVirtualId,
      SND_gst_TrackId.actElemNum, SND_gst_TrackLookUpTable.actElemNum);
    for (i = 0; i < SND_gi_TrackNb && i < 48; ++i)
    {
        tTrackListElem* t = &SND_gax_TrackPlayingList[i];
        d("T%d fl=%08X id=%d buf=%ld res=%ld intf=%d num=%d s2m=%d unk=%d "
          "f1=%08X f2=%08X\n",
          i, t->flags, t->trackID, heap_off(t->sndBufferElem),
          heap_off(t->trackResource), intf_id(t->soundIntf), t->trackResourceNum,
          t->stereoToMono_sub499C20, t->unk1, fbits(t->f1), fbits(t->f2));
    }
    for (i = 0; i < 4; ++i)
        d("R%d busy=%d s=%ld m=%ld\n", i, SND_gast_TrackResources[i].isBusy,
          heap_off(SND_gast_TrackResources[i].stereoSoundBuffer),
          heap_off(SND_gast_TrackResources[i].monoSoundBuffer));
    for (i = 0; i < SND_gst_TrackId.actElemNum && i < 48; ++i)
        d("TK%d %u->%d\n", i, SND_gst_TrackId.keyValPtr[i].key,
          SND_gst_TrackId.keyValPtr[i].val);
}

static void dump_prefetch(void)
{
    int i;
    d("pf num=%d sum=%d ptr=%ld\n", SND_gst_PrefetchManager.num,
      SND_gst_PrefetchManager.sumChannelNums, heap_off(SND_gst_PrefetchManager.ptr));
    for (i = 0; i < SND_gst_PrefetchManager.num && i < 32; ++i)
    {
        tSndPrefetchElem* e = &SND_gst_PrefetchManager.ptr[i];
        d("P%d s=%d t=%d a=%d b=%d\n", i, e->sndElemIdx, e->type,
          e->sndIntanceIdx, e->sndInstanceIdx2);
    }
    d("streams size=%d ptr=%ld\n", SND_gi_StreamListSize, heap_off(SND_gast_StreamList));
    for (i = 0; i < SND_gi_StreamListSize && i < 16; ++i)
        d("ST%d n=%d\n", i, SND_gast_StreamList[i].num);
}

static void dump_families(void)
{
    int i, j;
    d("fam nb=%d size=%d ptr=%ld\n", SND_gi_AndFamillyNb, SND_gi_AndFamillyListSize,
      heap_off(SND_gast_AndFamillyList));
    for (i = 0; i < SND_gi_AndFamillyNb && i < 16; ++i)
    {
        tSoundFamilyStruct* f = &SND_gast_AndFamillyList[i];
        d("A%d i1=%08X i2=%08X n=%d p=%ld\n", i, f->i1, f->i2, f->innerNum,
          heap_off(f->unkIntArrPtr));
        for (j = 0; j < f->innerNum && j < 8; ++j)
        {
            tSoundFamilyInnerStruct* in = f->unkIntArrPtr ? f->unkIntArrPtr[j] : 0;
            d("  B%d bits=%08X p=%ld", j, f->i4[j], heap_off(in));
            if (heap_off(in) >= 0)
                d(" rc=%d m1=%d,%08X,%08X m2=%d,%08X,%08X t1=%08X t2=%08X",
                  in->i1, in->muteSetup1.muteEnabled, fbits(in->muteSetup1.muteSetUp1),
                  fbits(in->muteSetup1.muteSetUp2), in->muteSetup2.muteEnabled,
                  fbits(in->muteSetup2.muteSetUp1), fbits(in->muteSetup2.muteSetUp2),
                  fbits(in->muteSetup1TimeMultiplier), fbits(in->muteSetup2TimeMultiplier));
            d("\n");
        }
    }
}

static void dump_globals(void)
{
    d("params fl=%08X en=%d handles=%ld micPtr=%ld ref=%ld rolloff=%08X dop=%08X "
      "sur=%08X dopSpd=%08X mode=%d mask=%d\n",
      SND_gst_Params.flags, SND_gst_Params.soundEnabled,
      heap_off(SND_gst_Params.handles), heap_off(SND_gst_Params.microPosMtxPtr),
      heap_off(SND_gst_Params.sndMatrixRef), fbits(SND_gst_Params.f_rolloff),
      fbits(SND_gst_Params.fx_doppler), fbits(SND_gst_Params.f_surround),
      fbits(SND_gst_Params.fx_DopplerSpeed), SND_gst_Params.soundRenderMode,
      SND_gst_Params.soundRenderModeMask);
    d("ls new=%d inst=%d elem=%d buf=%ld cb=%d used=%d\n",
      SND_gst_LSGlobal.isNewLoading, SND_gst_LSGlobal.soundInstanceIndex,
      SND_gst_LSGlobal.soundElemIndex, heap_off(SND_gst_LSGlobal.soundElemElem),
      SND_gst_LSGlobal.userCallBackFunction ? 1 : 0,
      SND_gst_LSGlobal.isSoundStillUsedElsewhere);
    d("le mode=%d out=%08X in=%08X i4=%08X i5=%08X i6=%08X\n",
      SND_gst_LoadingEffect.mode, fbits(SND_gst_LoadingEffect.f_fadeOutTime),
      fbits(SND_gst_LoadingEffect.f_fadeInTime), fbits(SND_gst_LoadingEffect.i4),
      fbits(SND_gst_LoadingEffect.i5), fbits(SND_gst_LoadingEffect.i6));
    d("fx a=%08X,%d,%d,%d,%08X,%d b=%08X,%d,%d,%d,%08X,%d\n",
      SND_gst_FxParam.flags, SND_gst_FxParam.mode, SND_gst_FxParam.delay,
      SND_gst_FxParam.feedback, fbits(SND_gst_FxParam.vol), SND_gst_FxParam.wetPan,
      SND_gst_FxDefaultParam.flags, SND_gst_FxDefaultParam.mode,
      SND_gst_FxDefaultParam.delay, SND_gst_FxDefaultParam.feedback,
      fbits(SND_gst_FxDefaultParam.vol), SND_gst_FxDefaultParam.wetPan);
    d("counters %d/%d %d/%d unloaded=%d all=%d seek=%d raster=%d meta=%u/%ld\n",
      SND_AllocatedSoundsNumWithhoutFlag80000000,
      SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage,
      SND_AllocatedSoundsNumWithFlag80000000,
      SND_AllocatedSoundsNumWithFlag80000000PeakUsage,
      SND_gb_UnloadedFile, SND_gb_AllSoundsLoaded, SND_gul_WavBinSeek,
      SND_gi_RasterCurrentFileId, SND_gul_MetaBankUserNb,
      heap_off(SND_gax_MetaBankUserList));
    d("mic %08X,%08X,%08X init=%d hdr=%d\n", fbits(t_last_mic_pos_522.x),
      fbits(t_last_mic_pos_522.y), fbits(t_last_mic_pos_522.z), i_init_521,
      SND_gst_HeaderArray.actElemNum);
}

static void dump_all(void)
{
    dump_globals();
    dump_faders();
    dump_sounds();
    dump_instances();
    dump_tracks();
    dump_prefetch();
    dump_families();
}

/* ------------------------------------------------------------- set-up */

static Mtx4x4 mtx_pool[8];
static tGameObjectMain gao_pool[4];
static tGameObjectExtended ext_pool[4];
static tSndBank* bank_pool[4];

static void reset_globals(void)
{
    memset(&SND_gst_Params, 0, sizeof(SND_gst_Params));
    memset(&SND_gst_LSGlobal, 0, sizeof(SND_gst_LSGlobal));
    memset(&SND_gpst_Interface, 0, sizeof(SND_gpst_Interface));
    memset(SND_gast_TrackResources, 0, sizeof(SND_gast_TrackResources));
    memset(&SND_gst_TrackId, 0, sizeof(SND_gst_TrackId));
    memset(&SND_gst_TrackLookUpTable, 0, sizeof(SND_gst_TrackLookUpTable));
    memset(&SND_gst_HeaderArray, 0, sizeof(SND_gst_HeaderArray));
    memset(&SND_gst_PrefetchManager, 0, sizeof(SND_gst_PrefetchManager));
    memset(SND_Mutes_B30600_fadergroups, 0, sizeof(SND_Mutes_B30600_fadergroups));
    memset(SND_gs_SoundModifiers, 0, sizeof(SND_gs_SoundModifiers));
    memset(&SND_gst_LoadingEffect, 0, sizeof(SND_gst_LoadingEffect));
    memset(&SND_gst_FxParam, 0, sizeof(SND_gst_FxParam));
    memset(&SND_gst_FxDefaultParam, 0, sizeof(SND_gst_FxDefaultParam));
    memset(&rasterbas1, 0, sizeof(rasterbas1));
    memset(&rasterbas2, 0, sizeof(rasterbas2));
    memset(&sndRasterStat, 0, sizeof(sndRasterStat));
    memset(&t_last_mic_pos_522, 0, sizeof(t_last_mic_pos_522));
    memset(mtx_pool, 0, sizeof(mtx_pool));
    memset(gao_pool, 0, sizeof(gao_pool));
    memset(ext_pool, 0, sizeof(ext_pool));
    memset(bank_pool, 0, sizeof(bank_pool));
    SND_gax_TrackPlayingList = 0;
    SND_gi_TrackNb = 0;
    SND_gi_TrackLastVirtualId = 0;
    SND_gul_MetaBankUserNb = 0;
    SND_gax_MetaBankUserList = 0;
    SND_gb_AllSoundsLoaded = 0;
    SND_gb_UnloadedFile = 0;
    SND_gul_WavBinSeek = 0;
    SND_gi_RasterCurrentFileId = 0;
    SND_gast_StreamList = 0;
    SND_gi_StreamListSize = 0;
    SND_gast_AndFamillyList = 0;
    SND_gi_AndFamillyNb = 0;
    SND_gi_AndFamillyListSize = 0;
    SND_gpst_RqMicroPosFromAI = 0;
    SND_gc_NoSound = 0;
    SND_gf_AutoVolumeOff = 1.0f;
    SND_AllocatedSoundsNumWithhoutFlag80000000 = 0;
    SND_AllocatedSoundsNumWithFlag80000000 = 0;
    SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage = 0;
    SND_AllocatedSoundsNumWithFlag80000000PeakUsage = 0;
    i_init_521 = 0;
    sndMdfBankUser = 0;
    SND_gst_BankMdFUserList = 0;
    LOA_gb_SpeedMode = 0;
    BIG_gi_ReadMode = 0;
    BIG_gul_ReadPos = 0;
    BIG_gi_ReadSeek = 0;
    LOA_ul_BinKey = 0x00300000;
    TIM_gf_dt = 0.03125f;
    TIM_gf_MainClock = 10.5f;
}

static const unsigned int sound_flag_bank[8] = {
    0x400u, 0x401u, 0x80020400u, 0x80024400u,
    0x80022400u, 0x80080400u, 0x10400u, 0x200401u
};

static void build_world(int k)
{
    int i;
    tSndWavHdrStruct w;

    SND_gpst_Interface.win = snd_mock_intf;
    SND_gpst_Interface.music = snd_mock_intf;
    SND_gpst_Interface.dialog = snd_mock_intf;
    SND_gpst_Interface.ambience = snd_mock_intf;
    SND_gpst_Interface.track = snd_mock_intf;
    SND_gpst_Interface.stream = snd_mock_intf;

    SND_gst_Params.soundEnabled = 1;
    SND_gst_Params.handles = (tSndHandles*)MEM_p_Alloc(sizeof(tSndHandles));
    memset(SND_gst_Params.handles, 0, sizeof(tSndHandles));
    SND_gst_Params.f_rolloff = 1.0f + 0.25f * (k & 3);
    SND_gst_Params.fx_doppler = 1.0f;
    SND_gst_Params.f_surround = 1.2f;
    SND_gst_Params.fx_DopplerSpeed = 300.0f;
    SND_gst_Params.flags = (k & 4) ? 0x500 : 0x100;

    SND_l_InitVolumeTable();

    SND_gst_Params.allocatedSoundsMaxNum = 16;
    SND_gst_Params.allocatedSounds =
        (tSndSoundElem*)MEM_p_Alloc(16 * sizeof(tSndSoundElem));
    memset(SND_gst_Params.allocatedSounds, 0, 16 * sizeof(tSndSoundElem));

    memset(&w, 0, sizeof(w));
    for (i = 0; i < 8; ++i)
    {
        tSndSoundElem* s = &SND_gst_Params.allocatedSounds[i];
        s->soundKey = 0x1000 + i;
        /* bit 0 (loaded) and 0x200000 (header read) are forced on every sound:
           the load paths reach SND_i_GetWaveInfo, which holds a pointer in an
           int and so cannot run in this 64-bit harness. */
        s->flags = sound_flag_bank[(i + k) & 7] | 1u | 0x200000u;
        s->muteFacorFlags = 0x40u << ((i + k) % 3);
        s->usageCnt = 1 + ((i + k) & 1);
        s->freq = 22050 + 1000 * i;
        s->numChannels = (short)(1 + ((i + k) & 1));
        s->bitsPerSample = 16;
        s->format = (short)((i & 1) ? 2 : 1);
        s->byteRate = 44100u + 100u * i;
        s->blockAlign = 4;
        s->extraDataSize = (short)(2 * (i & 3));
        s->startDataPos = 64 + 16 * i;
        s->fullsoundLen = 100000u + 1000u * i;
        s->sndInterface = &SND_gpst_Interface.win;
        w.wfex.wFormatTag = s->format;
        w.wfex.nChannels = s->numChannels;
        w.wfex.nSamplesPerSec = s->freq;
        w.fullSoundSize = s->fullsoundLen;
        w.dataStartPos = s->startDataPos;
        s->sndSndBufElem = SND_gpst_Interface.win.Create(SND_gst_Params.handles, &w, 0);
    }

    SND_gst_Params.soundInstanceMaxNum = 12;
    SND_gst_Params.soundInstanceNum = 8;
    SND_gst_Params.soundInstances =
        (tSoundInstanceElem*)MEM_p_Alloc(12 * sizeof(tSoundInstanceElem));
    memset(SND_gst_Params.soundInstances, 0, 12 * sizeof(tSoundInstanceElem));

    for (i = 0; i < 4; ++i)
    {
        mtx_pool[i].v1.x = 1.0f; mtx_pool[i].v2.y = 1.0f; mtx_pool[i].v3.z = 1.0f;
        mtx_pool[i].Pos.x = (float)(10 * i - 15 + k);
        mtx_pool[i].Pos.y = (float)(3 * i - 4);
        mtx_pool[i].Pos.z = (float)(7 - 2 * i + (k & 7));
    }
    mtx_pool[4].v1.x = 1.0f; mtx_pool[4].v2.y = 1.0f; mtx_pool[4].v3.z = 1.0f;
    mtx_pool[4].Pos.x = (float)(k & 15);
    mtx_pool[4].Pos.y = 1.0f;
    mtx_pool[4].Pos.z = -2.0f;

    for (i = 0; i < 4; ++i)
    {
        gao_pool[i].flags = 0x8000000u;
        gao_pool[i].flags2 = (i & 1) ? 0x2u : 0x400006u;
        gao_pool[i].extended = &ext_pool[i];
        gao_pool[i].matrices = &mtx_pool[i];
        bank_pool[i] = (tSndBank*)MEM_p_Alloc(sizeof(tSndBank));
        memset(bank_pool[i], 0, sizeof(tSndBank));
        bank_pool[i]->usageCount = 1;
        bank_pool[i]->elemNum = 4;
        bank_pool[i]->bankKey = 0x7000 + i;
        bank_pool[i]->soundIndices = (int*)MEM_p_Alloc(4 * sizeof(int));
        bank_pool[i]->soundIndices[0] = (i + 0) & 7;
        bank_pool[i]->soundIndices[1] = (i + 2) & 7;
        bank_pool[i]->soundIndices[2] = ((i + k) & 1) ? -1 : ((i + 4) & 7);
        bank_pool[i]->soundIndices[3] = (i + 6) & 7;
        ext_pool[i].snd = bank_pool[i];
    }

    for (i = 0; i < 8; ++i)
    {
        tSoundInstanceElem* e = &SND_gst_Params.soundInstances[i];
        e->allocatedSoundsIdx = i & 7;
        e->flags = 0x400u | ((i & 1) ? 0x8000u : 0u) | ((i & 2) ? 0x1000u : 0u)
                 | (((i + k) & 3) == 0 ? 0x4000u : 0u)
                 | (((i + k) & 7) == 2 ? 0x17000000u : 0u);
        if ((i & 3) == 3)
            e->flags |= 0x86000u;
        e->muteFactorFlags = (0x40u << ((i + k) % 3)) | (((i + k) & 1) ? 0x200u : 0u);
        e->familyBitmask = (1 << (i & 3)) | (1 << (4 + ((i + k) & 3)));
        e->trackID = ((i & 3) == 3) ? 1000 + i : -1;
        e->freq = 22050 + 500 * i;
        e->volume = 0.125f * (float)(1 + (i & 7));
        e->front = i & 1;
        e->pan = 100 * i - 300;
        e->minPan = 0.25f;
        e->nearArr[0] = 10.0f; e->nearArr[1] = 12.0f; e->nearArr[2] = 8.0f;
        e->farArr[0] = 80.0f; e->farArr[1] = 60.0f; e->farArr[2] = 100.0f;
        e->dtFar = 2.0f;
        e->centerArr[0] = 45.0f; e->centerArr[1] = 45.0f; e->centerArr[2] = 45.0f;
        e->farCoefficient = 0.0f;
        e->middleCoefficient = 0.5f;
        e->sndPosMtxSetInSetPos = &mtx_pool[i & 3];
        e->sndPositionSetInSetPosPtr = &mtx_pool[i & 3].Pos;
        e->soundBufElemFromModel = SND_gst_Params.allocatedSounds[i & 7].sndSndBufElem;
        e->requestTimeStamp = 1.0f * (float)i;
        e->gao = ((i + k) & 3) ? &gao_pool[i & 3] : 0;
        e->soundIntf = &SND_gpst_Interface.win;
        e->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[(i + k) % 12];
        e->timeRemainTillStart = ((i + k) % 5 == 0) ? 0.05f : 0.0f;
        e->timeFromDefault = ((i + k) % 7 == 0) ? 0.5f : 0.0f;
    }

    SND_gst_Params.sndMatrixRef = &mtx_pool[4];
    SND_gst_Params.microPosMtxPtr = &mtx_pool[5];

    SND_gi_TrackLastVirtualId = 10000;
    BAS_binit(&SND_gst_TrackId, 16);
    BAS_binit(&SND_gst_TrackLookUpTable, 16);
    SND_TrackInitModule();

    /* SND_StreamInitModule cannot be called here: it subscripts
       SND_gast_StreamList with the result of SND_i_StreamAlloc, which is
       what assigns SND_gast_StreamList, and gcc loads the base first and
       faults on the null. The rest of what it does is set up by hand. */
    SND_gst_PrefetchManager.ptr =
        (tSndPrefetchElem*)MEM_p_Alloc(8 * sizeof(tSndPrefetchElem));
    memset(SND_gst_PrefetchManager.ptr, 0, 8 * sizeof(tSndPrefetchElem));
    SND_gst_PrefetchManager.num = 8;
    SND_gst_PrefetchManager.sumChannelNums = 0;
    SND_gast_StreamList = 0;
    SND_gi_StreamListSize = 0;
    SND_i_StreamAlloc();
}

/* ----------------------------------------------------------- the runs */

void SIDE_RUN(int c)
{
    int g = c % GROUPS;
    int k = c / GROUPS;
    int i, r;
    float f;

    reset_globals();
    build_world(k);

    switch (g)
    {
    case 0:                       /* the volume/attenuation tables */
        for (i = -9000; i <= 1000; i += 37)
            d("v2a %d -> %08X\n", i, fbits(SND_f_GetVolFromAtt(i)));
        for (i = 0; i <= 128; ++i)
        {
            f = (float)i / 128.0f + 0.0001f * (float)k;
            d("a2v %08X -> %d\n", fbits(f), SND_l_GetAttFromVol(f));
        }
        break;

    case 1:                       /* the mute factor over every fader */
        for (i = 0; i < 12; ++i)
        {
            SND_MuteEnable(i, ((k >> (i & 7)) & 1));
            SND_MuteSetUp(i, 0.125f * (float)((i + k) & 7), 0.25f * (float)((i * 3 + k) & 3));
        }
        for (i = 0; i < 8; ++i)
            d("mcf %d -> %08X %08X\n", i,
              fbits(SND_f_MuteComputeFactor(&SND_gst_Params.soundInstances[i])),
              fbits(SND_f_GetInstVolume(&SND_gst_Params.soundInstances[i])));
        for (i = 0; i < 12; ++i)
            d("mg %d %08X %08X %d\n", i, fbits(SND_f_MuteGet(i, 0)),
              fbits(SND_f_MuteGet(i, 1)), SND_i_MuteIsEnable(i));
        break;

    case 2:                       /* the fade engine */
        for (i = 0; i < 12; ++i)
            SND_Fade(i, 0.125f * (float)(i & 7), 0.125f * (float)((i + k) & 7),
                     0.25f * (float)((i + k) % 5));
        for (i = 0; i < 24; ++i)
        {
            TIM_gf_dt = 0.0625f * (float)(1 + (i & 3));
            SND_FadeUpdate();
            dump_faders();
        }
        SND_ResetAllMute();

        /* The fade's terminal step, which the sweep above cannot reach.
           SND_Fade sets i6 = 256/duration, so the step index is
           elapsed*256/duration -- and step 255 needs elapsed/duration
           to be exactly 255/256, which no sum of the dt values used
           above can produce. It matters because SND_gf_FadeOut[255] is
           0.0, so the level lands exactly on the target: the > 255 cap
           and the level >= target terminator are each one boundary
           away from behaving differently, and both scored clean until
           this case existed. 256 is driven too, for the cap itself. */
        for (i = 0; i < 4; ++i)
        {
            int grp = (i & 1) ? 5 : 3;          /* 5 is the gap in the switch */
            SND_Fade(grp, 0.25f, 0.75f, 1.0f);
            TIM_gf_dt = (i < 2) ? (255.0f / 256.0f) : 1.0f;
            SND_FadeUpdate();
            dump_faders();
            SND_Fade(grp, 0.75f, 0.25f, 1.0f);   /* the other flag arm */
            TIM_gf_dt = (i < 2) ? (255.0f / 256.0f) : 1.0f;
            SND_FadeUpdate();
            dump_faders();
        }
        SND_ResetAllMute();
        break;

    case 3:                       /* the spherical and per-axis attenuation */
        for (i = 0; i < 32; ++i)
        {
            Vec3 mic;
            int state = 0;
            tSoundInstanceElem* e = &SND_gst_Params.soundInstances[i & 7];
            mic.x = (float)(i * 7 - 100) + 0.5f * (float)k;
            mic.y = (float)(i * 3 - 40);
            mic.z = (float)(60 - i * 5);
            e->muteFactorFlags = (e->muteFactorFlags & ~7u) | (unsigned)((i + k) & 7);
            d("sph %d -> %08X st=%d\n", i, fbits(SND_f_SphericComputing(&mic, e, &state)), state);
            state = 0;
            d("ax0 %d -> %08X st=%d\n", i, fbits(SND_f_AxisComputing(&mic, e, &state, 0)), state);
            state = 0;
            d("ax1 %d -> %08X st=%d\n", i, fbits(SND_f_AxisComputing(&mic, e, &state, 1)), state);
            state = 0;
            d("ax2 %d -> %08X st=%d\n", i, fbits(SND_f_AxisComputing(&mic, e, &state, 2)), state);
        }
        break;

    case 4:                       /* the active-volume test */
        for (i = 0; i < 8; ++i)
        {
            tSoundInstanceElem* e = &SND_gst_Params.soundInstances[i];
            e->flags = (e->flags & ~0x1F000000u)
                     | ((unsigned)((i + k) & 0x1F) << 24);
            d("av %d -> %d\n", i, SND_b_SndIsInActiveVolume(i));
        }
        SND_gst_Params.sndMatrixRef = 0;
        d("av-null -> %d\n", SND_b_SndIsInActiveVolume(0));
        break;

    case 5:                       /* the sound element table */
        for (i = 0; i < 12; ++i)
        {
            d("add %d\n", SND_l_AddSound(0x1000 + ((i + k) & 15)));
            d("addL %d\n", SND_l_AddLoadingSound(0x2000 + ((i + k) & 7)));
            d("addD %d\n", SND_l_AddDialogNoLoad(0x3000 + ((i + k) & 7)));
            d("addM %d\n", SND_l_AddMusicNoLoad(0x4000 + ((i + k) & 7)));
            d("addA %d\n", SND_l_AddAmbienceNoLoad(0x5000 + ((i + k) & 7)));
            d("idx %d\n", SND_l_GetSoundIndex(0x1000 + ((i + k) & 15)));
        }
        {
            tSndList* list = 0;
            int count = 0;
            d("make %d count=%d\n", SND_SoundListMake(&list, &count), count);
            for (i = 0; i < count && i < 32; ++i)
                d("L%d %d %08X\n", i, list[i].sndkey, list[i].flags);
        }
        for (i = 0; i < 20; ++i)
            SND_DeleteByIndex((i + k) % 20 - 2);
        SND_DetachSound((k & 7));
        break;

    case 6:                       /* instance allocation and release */
        for (i = 0; i < 10; ++i)
            d("unused %d\n", SND_l_GetUnusedInstance());
        for (i = 0; i < 8; ++i)
            d("req %d\n", SND_l_Request(0, (i + k) & 7));
        for (i = 0; i < 4; ++i)
            d("reqg %d\n", SND_l_Request(&gao_pool[i], (i + k) & 7));
        for (i = 0; i < 6; ++i)
            SND_Release((i + k) % 10 - 1);
        break;

    case 7:                       /* the instance setters and getters */
        for (i = 0; i < 8; ++i)
        {
            SND_SetVol(i, 0.125f * (float)((i + k) & 15));
            SND_SetPan(i, 100 * ((i + k) & 7) - 300, (i + k) & 1);
            SND_SetFreq(i, 11025 + 1000 * ((i + k) & 7));
            SND_SetNearFar(i, 4096 + ((i + k) % 3), (float)(5 + i), (float)(50 + 3 * i));
            SND_SetPos(0, i, &mtx_pool[(i + k) & 3]);
            SND_SetFlags(0, i, (int)(0x40000000u | (unsigned)((i + k) & 0xFCB)));
            SND_SetFlags(0, i, (int)((unsigned)((i + k) & 0xFF)));
            SND_ResetFlags(0, i, (int)(0x40000000u | (unsigned)((i * 3 + k) & 0xFCB)));
            SND_SetDoppler(i, (i + k) & 1);
            SND_LinkToFamilly(1 << ((i + k) & 5), i);
            SND_UnlinkFromFamilly(1 << ((i + k) & 3), i);
            SND_i_InstanceSetf(i, 4099 + ((i + k) % 20), 0.5f * (float)(i + 1));
        }
        for (i = -1; i < 9; ++i)
        {
            d("get %d pan=%d front=%d freq=%d vol=%08X ratio=%08X trk=%d play=%d "
              "snd=%d gi4112=%d gi4113=%d\n",
              i, SND_l_GetPan(i), SND_l_GetFront(i), SND_l_GetFreq(i),
              fbits(SND_f_GetVol(i)), fbits(SND_f_CurrentPosRatio(i)),
              SND_i_GetTrack(i), SND_i_IsPlaying(i), SND_l_GetSoundFromInstance(i),
              SND_i_InstanceGeti(i, 4112), SND_i_InstanceGeti(i, 4113));
            for (r = 4099; r <= 4118; ++r)
                d("  gf %d %08X\n", r, fbits(SND_f_InstanceGetf(i, r)));
        }
        break;

    case 8:                       /* tracks */
        for (i = 0; i < 6; ++i)
            d("tnew %d\n", SND_i_TrackNew());
        for (i = 0; i < 8; ++i)
            SND_SetTrack(i, (unsigned)(2000 + ((i + k) & 3)));
        for (i = 0; i < 8; ++i)
            SND_PlaySoundOnTrack(i);
        for (i = 0; i < 4; ++i)
        {
            d("tseti %d\n", SND_i_TrackSeti((unsigned)(2000 + i), 1 + ((i + k) % 5), (i + k) & 1));
            d("tgeti %d\n", SND_i_TrackGeti((unsigned)(2000 + i), 1 + ((i + k) % 6)));
            d("tsetf %d\n", SND_i_TrackSetf(2000 + i, 1 + ((i + k) % 7), 0.25f * (float)i));
            d("tgetf %08X\n", fbits(SND_f_TrackGetf((unsigned)(2000 + i), 1 + ((i + k) % 4))));
        }
        SND_TrackForceFreeResources((k & 3));
        for (i = 0; i < 4; ++i)
            SND_TrackDestroy((unsigned)(2000 + ((i + k) & 3)));
        SND_StreamStopAll();
        SND_TrackFreeList();
        break;

    case 9:                       /* the prefetch manager */
        for (i = 0; i < 3; ++i)
            d("salloc %d\n", SND_i_StreamAlloc());
        for (i = 0; i < 12; ++i)
            SND_StreamPrefetchOneSound((i + k) % 10 - 1, (i + k) % 3);
        dump_prefetch();
        SND_StreamResolveSyncPrefetch();
        dump_prefetch();
        for (i = 0; i < 6; ++i)
            SND_StreamFlushOneSound_0((i + k) % 10 - 1, (i + k) & 1);
        dump_prefetch();
        for (i = 0; i < 8; ++i)
            SND_StreamPrefetch((i + k) % 10 - 1);
        for (i = 0; i < 8; ++i)
            SND_StreamFlush((i + k) % 10 - 1);
        SND_StreamPrefetchFlushAll();
        break;

    case 10:                      /* play requests and the victim search */
        snd_play_fail_mod = 1 + (k % 5);
        for (i = 0; i < 8; ++i)
        {
            SND_PlaySound(i);
            SND_PlaySoundLooping((i + 1) & 7);
            SND_PlaySoundOnTrack((i + 2) & 7);
            SND_PlaySoundLoopingOnTrack((i + 3) & 7);
            SND_PlaySoundOnTrackFromTo((i + 4) & 7, 0.125f * (float)i, 0.5f * (float)i);
            SND_PlaySoundLoopingOnTrackFrom((i + 5) & 7, 0.25f * (float)i);
        }
        snd_play_fail_mod = 0;
        for (i = 0; i < 8; ++i)
        {
            SND_StopRq(i);
            SND_AutoStop(&SND_gst_Params.soundInstances[i]);
            SND_AutoPlay(&SND_gst_Params.soundInstances[i]);
            SND_Pause(i);
            SND_Stop(i);
        }
        break;

    case 11:                      /* the per-frame update */
        for (i = 0; i < 12; ++i)
        {
            TIM_gf_dt = 0.0078125f * (float)(1 + ((i + k) & 7));
            TIM_gf_MainClock += TIM_gf_dt;
            mtx_pool[4].Pos.x += 1.5f;
            mtx_pool[4].Pos.z -= 0.75f;
            if ((i & 3) == 2)
                SND_gpst_RqMicroPosFromAI = &gao_pool[(i + k) & 3];
            SND_Update(&mtx_pool[6]);
            dump_instances();
        }
        SND_NotifyNewLoading();
        SND_DeleteOldLoadingSound();
        SND_RegisterInactiveGAO(&gao_pool[k & 3]);
        break;

    case 12:                      /* the families */
        /* Every request here either creates a family entry or is fully
           consumed by the ones already there. A request that is only
           partly covered reaches SND_pst_GetOrArray's tail, which stores
           the new inner one past the array it just reallocated and leaves
           the slot it filled i4[] for uninitialised, so the walks below
           would then dereference whatever realloc left behind. */
        for (i = 0; i < 8; ++i)
        {
            int mask = 1 << ((i + k) & 3);
            int bits = 1 << ((i * 3 + k) & 7);
            SND_pst_GetOrArray(mask, bits);     /* create */
            SND_pst_GetOrArray(mask, bits);     /* exact match, early return */
            SND_MuteOnFamilly(mask, bits);
            SND_MuteOffFamilly(mask, bits);
            SND_FadeInFamilly(mask, bits);
            SND_FadeOutFamilly(mask, bits);
            SND_SetfFamilly(mask, bits, 3 + ((i + k) % 5), 0.25f * (float)(i - 3));
            d("gff %08X\n", fbits(SND_f_GetfFamilly(mask, bits, 3 + ((i + k) % 5))));
            d("sfam %d\n", SND_i_GetStatusFromFamilly(mask, bits));
            dump_families();
        }
        /* the split-and-share walk, over a family built by hand so that the
           request is fully consumed and the tail is never reached */
        {
            int fam = SND_gi_AndFamillyNb;
            tSoundFamilyStruct* f;
            tSoundFamilyInnerStruct* first;

            SND_pst_GetOrArray(0x4000, 3);
            f = &SND_gast_AndFamillyList[fam];
            /* the engine sizes this array at 4 bytes per pointer, which is
               right in its own 32-bit build and half of what this harness
               needs, so the first entry is saved across the regrow */
            first = f->unkIntArrPtr[0];
            f->unkIntArrPtr = (tSoundFamilyInnerStruct**)
                MEM_p_Realloc(f->unkIntArrPtr, 3 * sizeof(void*));
            f->unkIntArrPtr[0] = first;
            for (i = 1; i < 3; ++i)
            {
                tSoundFamilyInnerStruct* in =
                    (tSoundFamilyInnerStruct*)MEM_p_Alloc(sizeof(tSoundFamilyInnerStruct));
                memset(in, 0, sizeof(tSoundFamilyInnerStruct));
                in->i1 = 1 + (i & 1);
                in->muteSetup1.muteSetUp1 = 1.0f;
                in->muteSetup1.muteSetUp2 = 0.5f;
                in->muteSetup2.muteSetUp1 = 1.0f;
                in->muteSetup2.muteSetUp2 = 1.0f;
                in->muteSetup1TimeMultiplier = 10000.0f;
                in->muteSetup2TimeMultiplier = -10000.0f;
                f->unkIntArrPtr[i] = in;
            }
            f->i4[1] = 1;
            f->i4[2] = 12;
            f->innerNum = 3;
            f->i2 = 15;
            dump_families();
            SND_pst_GetOrArray(0x4000, 1);
            dump_families();
            SND_pst_GetOrArray(0x4000, 15);
            dump_families();
            SND_MuteOnFamilly(0x4000, 1);
            SND_FadeOutFamilly(0x4000, 12);
            SND_SetfFamilly(0x4000, 15, 3 + (k % 4), 0.5f * (float)(k & 7));
            dump_families();
        }
        for (i = 0; i < 8; ++i)
        {
            SND_PlayFamilly(1 << (i & 3), 1 << (4 + (i & 3)));
            SND_StopFamilly(1 << (i & 3), 1 << (4 + (i & 3)));
        }
        SND_FamillyUpdate();
        SND_FamillyUpdate();
        SND_FamillyCloseModule();
        break;

    case 13:                      /* the fx block and the global params */
        for (i = 8193; i <= 8205; ++i)
        {
            d("fxs %d -> %d\n", i, SND_i_FxSeti(i, (i + k) % 9));
            d("fxg %d -> %d\n", i, SND_i_FxGeti(i));
            d("fxsf %d -> %d\n", i, SND_i_FxSetf(i, 0.125f * (float)((i + k) & 7)));
            d("fxgf %d -> %08X\n", i, fbits(SND_f_FxGetf(i)));
        }
        SND_i_FxInitModule();
        for (i = -3; i <= 16; ++i)
        {
            d("seti %d -> %d\n", i, SND_i_Seti(i, (i + k) & 1));
            d("geti %d -> %d\n", i, SND_i_Geti(i));
            d("setf %d -> %d\n", i, SND_i_Setf(i, 0.25f * (float)(i + k)));
            d("getf %d -> %08X\n", i, fbits(SND_f_Getf(i)));
            d("crm %d -> %d\n", i, SND_i_ChangeRenderMode(i));
            d("cso %d -> %d\n", i, ConsoleSpecificOption(i));
        }
        SND_SetGlobalVol(0.125f * (float)(k & 7));
        SND_SetGlobalPan(100 * (k & 7));
        d("gv %08X gp %d\n", fbits(SND_f_GetGlobalVol()), SND_l_GetGlobalPan());
        SND_FxCloseModule();
        break;

    case 14:                      /* banks, merges and object attach/detach */
        for (i = 0; i < 4; ++i)
        {
            SND_ResolveRefs(&gao_pool[i]);
            SND_i_CheckOrInitGAO(&gao_pool[i]);
            d("dur %08X\n", fbits(SND_SoundDurationGet(&gao_pool[i], (i + k) % 6 - 1)));
        }
        SND_Duplicate(&gao_pool[0], &gao_pool[1]);
        SND_MergeBankOver(&gao_pool[(k + 1) & 3], &gao_pool[k & 3]);
        SND_MergeBankNoOver(&gao_pool[(k + 2) & 3], &gao_pool[k & 3]);
        SND_MergeBank(&gao_pool[(k + 3) & 3], &gao_pool[k & 3],
                      (k % 3) - 1, (k % 5) - 1, k & 1);
        for (i = 0; i < 4; ++i)
            SND_RestoreBank(&gao_pool[i]);
        for (i = 0; i < 4; ++i)
            SND_DetachObject(&gao_pool[i]);
        SND_DestroyInstanceOfGao(&gao_pool[k & 3]);
        for (i = 0; i < 4; ++i)
            SND_FreeMainStruct(&gao_pool[i]);
        break;

    case 15:                      /* the loading-sound state machine */
        SND_LoadingSoundInitModule();
        SND_gst_LSGlobal.soundInstanceIndex = (k & 7);
        SND_gst_LSGlobal.soundElemIndex = ((k >> 1) & 7);
        SND_gst_LoadingEffect.mode = (k % 4);
        SND_gst_LoadingEffect.i5 = 0.25f * (float)(k & 3);
        d("isLI %d isLS %d\n", SND_b_IsLoadingInstance(k & 7), SND_b_IsLoadingSound(k & 7));
        SND_LoadingSoundNotifyBeginLoading();
        for (i = 0; i < 8; ++i)
        {
            SND_LoadingSoundUpdate();
            dump_globals();
        }
        SND_LoadingSoundNotifyEndLoading();
        for (i = 0; i < 8; ++i)
        {
            SND_LoadingSoundUpdate();
            dump_globals();
        }
        SND_LoadingSoundRegisterInstanceUserCallback((k & 7), 0, 0);
        SND_DetachLoadingInstance();
        SND_ReinitLoadingInstance();
        SND_LoadingSoundCloseModule();
        SND_RasterCloseModule();
        break;

    case 16:                      /* the RIFF header reader */
        for (i = 0; i < 24; ++i)
        {
            unsigned char rec[80];
            char hdr[64];
            char* cur = (char*)rec;
            int fmtSize = (i % 4 == 0) ? 14 : (i % 4 == 1) ? 16
                        : (i % 4 == 2) ? 18 : 20;
            memset(rec, 0, sizeof(rec));
            memset(hdr, 0, sizeof(hdr));
            memcpy(rec + 0, "RIFF", 4);
            *(int*)(rec + 4) = 1000 + i + k;
            memcpy(rec + 8, "WAVE", 4);
            memcpy(rec + 12, "fmt ", 4);
            *(int*)(rec + 16) = fmtSize;
            *(short*)(rec + 20) = (short)(1 + (i & 1));
            *(short*)(rec + 22) = (short)(1 + ((i + k) & 1));
            *(int*)(rec + 24) = 22050 + 100 * i;
            *(int*)(rec + 28) = 44100 + 100 * i;
            *(short*)(rec + 32) = (short)(2 + (i & 3));
            *(short*)(rec + 34) = 16;
            *(short*)(rec + 36) = (short)(i & 7);
            memcpy(rec + 38, "data", 4);
            *(int*)(rec + 42) = 5000 + i;
            SND_ReadRIFFHeader(&cur, hdr);
            d("riff %d size=%d adv=%d [", i, fmtSize, (int)(cur - (char*)rec));
            for (r = 0; r < 54; ++r)
                d("%02X", (unsigned char)hdr[r]);
            d("]\n");
        }
        for (i = 0; i < 8; ++i)
        {
            unsigned char rec[80];
            tSoundHeaderGeneric g;
            char* cur = (char*)rec;
            memset(rec, 0, sizeof(rec));
            memset(&g, 0, sizeof(g));
            *(int*)(rec + 0) = 0x5000 + i;
            memcpy(rec + 4, "RIFF", 4);
            memcpy(rec + 12, "WAVE", 4);
            memcpy(rec + 16, "fmt ", 4);
            *(int*)(rec + 20) = 14 + 2 * (i & 2);
            *(int*)(rec + 50) = 0x9000 + i;
            d("sndhdr %d -> %d adv=%d key=%d key1=%d\n", i,
              SND_ReadSNDHeaderElem(&cur, &g), (int)(cur - (char*)rec),
              g.soundKey, g.soundKey1);
        }
        break;

    case 17:                      /* the bank loader callbacks */
        {
            unsigned char bank[8 * 8];
            unsigned char meta[6 * 8];
            static const char* ext[6] = { ".wad", ".wam", ".waa", ".smd", ".wac", ".wav" };
            tSndBank* b1;
            tSndBank* b2;

            snd_bigfile_clear();
            for (r = 0; r < 8; ++r)
            {
                *(int*)(bank + 8 * r) = (r == (k % 9)) ? 0 : (0x6000 + r);
                memcpy(bank + 8 * r + 4, ext[(r + k) % 6], 4);
            }
            for (r = 0; r < 6; ++r)
            {
                *(int*)(meta + 8 * r) = (r == (k % 7)) ? -1 : (0x8100 + r);
                *(int*)(meta + 8 * r + 4) = r;
            }
            d("bankpos %d\n", snd_bigfile_add(0x8100 + (k % 6), bank, sizeof(bank)));
            for (r = 0; r < 6; ++r)
            {
                if (r != (k % 6))
                    snd_bigfile_add(0x8100 + r, bank, sizeof(bank));
            }
            d("metapos %d\n", snd_bigfile_add(0x9000, meta, sizeof(meta)));

            b1 = SND_ul_CallbackLoadBank(BIG_ul_SearchKeyToPos(0x8100 + (k % 6)));
            d("bank1 %ld\n", heap_off(b1));
            b2 = SND_ul_CallbackLoadMetaBank(BIG_ul_SearchKeyToPos(0x9000));
            d("bank2 %ld\n", heap_off(b2));
            d("bank3 %ld\n", heap_off(SND_ul_CallbackLoad(BIG_ul_SearchKeyToPos(0x9000))));
            for (i = 0; i < 4; ++i)
            {
                tSoundMdfStruct* m = (tSoundMdfStruct*)MEM_p_Alloc(sizeof(tSoundMdfStruct));
                memset(m, 0, sizeof(tSoundMdfStruct));
                m->flags = (i & 1) ? 0x1800 : 0x800;
                m->idx = i;
                SND_AddMdFBankUser(m);
                if (i & 1)
                    SND_DelMdFBankUser(m);
            }
            SND_ResolveStaticPrefetchForSpecialMdF();
            SND_SignalChangeLanguageToMdF();
            if (b1) SND_FreeSndBank(&b1);
            if (b2) SND_FreeSndBank(&b2);
            SND_MetaBankFreeList();
            SND_CloseMdFModule();
            dump_sounds();
        }
        break;

    default:
        break;
    }

    dump_all();
}
