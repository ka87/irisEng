/* One side of the soundmodifier.c differential test.
 *
 * This file is compiled twice, once against the rewrite and once against
 * the frozen copy in baseline_soundmodifier.c, with every soundmodifier.c
 * symbol, every sound.h global and this file's two entry points renamed
 * by -D. Both sides therefore drive their own copy of SND_gst_Params and
 * SND_gs_SoundModifiers, and the harness compares the text each side
 * dumps plus the shared mock call log.
 *
 * sound.c is not linked. The entry points soundmodifier.c reaches into it
 * through are stood up here: the ones that only have to be observable log
 * and return a fixed answer, and the four that soundmodifier.c depends on
 * for its own behaviour -- SND_SModifierAddInInstance, SND_pst_SModifierGet,
 * SND_i_AllocSound and SND_l_Request -- reproduce what sound.c does,
 * reading sound.c's own bodies. They are renamed with the rest, so each
 * side has its own.
 *
 * Pointers into the mock heap are directly comparable because the arena is
 * rewound between the two runs; pointers into a side's own globals are not,
 * so they are normalised to an index before being printed.
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "soundmod_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../time.h"
#include "../math.h"
#include "../gameobject.h"
#include "../modifiers/gensoundmodifier.h"

#ifndef SIDE_RUN
#define SIDE_RUN sm_side_run
#endif
#ifndef SIDE_CASES
#define SIDE_CASES sm_side_cases
#endif

/* ------------------------------------------------ the engine's tables */

#define SOUNDS 12
#define INSTANCES 10

static tGameObjectMain gao_store[2];
static Mtx4x4 mtx_store[2];
static Vec3 pos_store[INSTANCES];
static Mtx4x4 inst_mtx_store[INSTANCES];
static Mtx4x4 ref_mtx;
static tSndBank bank_store;
static int bank_indices[8];

static int obj_id(const void* p)
{
    if (!p) return -1;
    if (p == &gao_store[0]) return 0;
    if (p == &gao_store[1]) return 1;
    if (p == &mtx_store[0]) return 10;
    if (p == &mtx_store[1]) return 11;
    if (p == &ref_mtx) return 12;
    if (p == &bank_store) return 13;
    if (p == (const void*)bank_indices) return 14;
    if (p == (const void*)&SND_gpst_Interface) return 20;
    if ((const Vec3*)p >= pos_store && (const Vec3*)p < pos_store + INSTANCES)
        return 30 + (int)((const Vec3*)p - pos_store);
    if ((const Mtx4x4*)p >= inst_mtx_store && (const Mtx4x4*)p < inst_mtx_store + INSTANCES)
        return 50 + (int)((const Mtx4x4*)p - inst_mtx_store);
    if ((const tSndModifierElem*)p >= SND_gs_SoundModifiers
        && (const tSndModifierElem*)p < SND_gs_SoundModifiers + 7)
        return 70 + (int)((const tSndModifierElem*)p - SND_gs_SoundModifiers);
    return (int)sm_heap_off(p);
}

/* ------------------------------------------------------------ dumping */

static void d(const char* fmt, ...)
{
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    sm_log("%s", buf);
}

static unsigned fbits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

/* MDF_SndCreateInstance and SND_SModifierExtPlayerApplyOnInstance both
   build a fade record on the stack and leave timeMultiplier and
   elapsedTime unwritten, and FadeInit copies the whole record into the
   heap. The two sides lay their frames out differently, so the garbage
   differs and nothing else would be comparable. Zeroing the region the
   call is about to use makes both sides read 0 there, and still catches a
   dropped assignment, which would leave one side at 0 and the other not. */
static void scrub_stack(void)
{
    volatile char pad[8192];
    memset((void*)pad, 0, sizeof(pad));
}

/* --------------------------------------- the sound.c entry points
 *
 * SND_i_AllocSound, SND_l_Request, SND_pst_SModifierGet and
 * SND_SModifierAddInInstance are the four whose behaviour soundmodifier.c
 * actually depends on, so they follow sound.c's own bodies. The rest only
 * have to be observable and are logged.
 */

int SND_i_AllocSound(void)
{
    int i;
    for (i = 0; i < SND_gst_Params.allocatedSoundsMaxNum; ++i)
        if (!SND_gst_Params.allocatedSounds[i].flags)
        {
            d("SND_i_AllocSound=%d\n", i);
            return i;
        }
    d("SND_i_AllocSound=0 (full)\n");
    return 0;
}

int SND_l_GetSoundIndex(int soundKey)
{
    int i;
    for (i = 0; i < SND_gst_Params.allocatedSoundsMaxNum; ++i)
        if (SND_gst_Params.allocatedSounds[i].flags
            && SND_gst_Params.allocatedSounds[i].soundKey == soundKey)
        {
            d("SND_l_GetSoundIndex(%d)=%d\n", soundKey, i);
            return i;
        }
    d("SND_l_GetSoundIndex(%d)=-1\n", soundKey);
    return -1;
}

static int add_stub(const char* who, int soundKey)
{
    int idx = SND_i_AllocSound();
    tSndSoundElem* s = &SND_gst_Params.allocatedSounds[idx];
    s->soundKey = soundKey;
    s->flags = 0x10400;
    s->usageCnt = 1;
    d("%s(%d)=%d\n", who, soundKey, idx);
    return idx;
}

int SND_l_AddDialogNoLoad(int soundKey)  { return add_stub("AddDialog", soundKey); }
int SND_l_AddMusicNoLoad(int soundKey)   { return add_stub("AddMusic", soundKey); }
int SND_l_AddAmbienceNoLoad(int soundKey){ return add_stub("AddAmbience", soundKey); }
int SND_l_AddLoadingSound(int soundKey)  { return add_stub("AddLoading", soundKey); }
int SND_l_AddSound(int soundKey)         { return add_stub("AddSound", soundKey); }

void SND_DeleteByIndex(int soundIdx) { d("SND_DeleteByIndex(%d)\n", soundIdx); }

int SND_l_Request(struct sGameObjectMain* gao, int soundIdx)
{
    int i;
    d("SND_l_Request(gao=%d,%d)", obj_id(gao), soundIdx);
    if (soundIdx < 0 || soundIdx >= SND_gst_Params.allocatedSoundsMaxNum)
    {
        d("=-1\n");
        return -1;
    }
    for (i = 0; i < SND_gst_Params.soundInstanceNum; ++i)
        if (!(SND_gst_Params.soundInstances[i].flags & 0x400))
        {
            tSoundInstanceElem* e = &SND_gst_Params.soundInstances[i];
            memset(e, 0, sizeof(*e));
            e->allocatedSoundsIdx = soundIdx;
            e->flags = 0x400;
            e->volume = 1.0f;
            e->trackID = -1;
            e->sndPositionSetInSetPosPtr = &pos_store[i];
            e->sndPosMtxSetInSetPos = &inst_mtx_store[i];
            e->farArr[0] = 10.0f;
            e->farArr[1] = 20.0f;
            e->farArr[2] = 30.0f;
            e->nearArr[0] = 1.0f;
            e->nearArr[1] = 2.0f;
            e->nearArr[2] = 3.0f;
            d("=%d\n", i);
            return i;
        }
    d("=-1\n");
    return -1;
}

struct sSndModifierElem* SND_pst_SModifierGet(struct sSndModifierElem* list, int type)
{
    tSndModifierElem* result;

    if (type >= 7 || type < 0)
        return NULL;
    for (result = list; result; result = result->nextModifier)
    {
        if (!result->sndModifierModelIntf)
            break;
        if (result->sndModifierModelIntf->Type == type)
            return result;
    }
    return NULL;
}

void SND_SModifierAddInInstance(int soundInstanceID, int sndModifierID, void* linkedSound, int dataSize)
{
    tSoundInstanceElem* inst;
    tSndModifierElem* modifier;

    d("SND_SModifierAddInInstance(%d,%d,%d)\n", soundInstanceID, sndModifierID, dataSize);
    if (soundInstanceID >= 0 && soundInstanceID < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[soundInstanceID];
        if ((inst->flags & 0x400) != 0)
        {
            modifier = SND_pst_SModifierInsertOneSModifier(&inst->modifierInstances, sndModifierID, linkedSound, dataSize);
            modifier->sndModifierModelIntf->PlayApplyOnInstance(modifier, modifier->linkedSoundData, modifier->linkedSoundDataSize, inst);
        }
    }
}

void SND_SetVol(int instanceIdx, float vol)
{
    d("SND_SetVol(%d,%08X)\n", instanceIdx, fbits(vol));
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
        SND_gst_Params.soundInstances[instanceIdx].volume = vol;
}

void SND_SetFlags(struct sGameObjectMain* gao, int idx, int flags)
{
    d("SND_SetFlags(gao=%d,%d,%08X)\n", obj_id(gao), idx, flags);
    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
        SND_gst_Params.soundInstances[idx].flags |= flags & 0xFCB;
}

int SND_i_CheckOrInitGAO(struct sGameObjectMain* gao)
{
    d("SND_i_CheckOrInitGAO(gao=%d)\n", obj_id(gao));
    return 1;
}

void SND_SetPos(struct sGameObjectMain* gao, int instanceIdx, Mtx4x4* mtx)
{
    d("SND_SetPos(gao=%d,%d,mtx=%d)\n", obj_id(gao), instanceIdx, obj_id(mtx));
}

void SND_SetFreq(int instanceIdx, int freq)
{
    d("SND_SetFreq(%d,%d)\n", instanceIdx, freq);
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
        SND_gst_Params.soundInstances[instanceIdx].freq = freq;
}

void SND_SetPan(int idx, int pan, int front)
{
    d("SND_SetPan(%d,%d,%d)\n", idx, pan, front);
    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
        SND_gst_Params.soundInstances[idx].pan = pan;
}

void SND_SetNearFar(int idx, int type, float near1, float far1)
{
    d("SND_SetNearFar(%d,%d,%08X,%08X)\n", idx, type, fbits(near1), fbits(far1));
    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum && type >= 4096 && type <= 4098)
    {
        SND_gst_Params.soundInstances[idx].nearArr[type - 4096] = near1;
        SND_gst_Params.soundInstances[idx].farArr[type - 4096] = far1;
    }
}

int SND_i_InstanceSetf(int instanceIdx, int type, float value)
{
    d("SND_i_InstanceSetf(%d,%d,%08X)\n", instanceIdx, type, fbits(value));
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        tSoundInstanceElem* e = &SND_gst_Params.soundInstances[instanceIdx];
        if (type == 4102) e->farCoefficient = value;
        if (type == 4103) e->middleCoefficient = value;
        if (type == 4104) e->dtFar = value;
    }
    return 1;
}

void SND_LinkToFamilly(int mask, int instanceIdx)
{
    d("SND_LinkToFamilly(%08X,%d)\n", mask, instanceIdx);
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
        SND_gst_Params.soundInstances[instanceIdx].familyBitmask |= mask;
}

int sm_track_playing;
int SND_i_IsTrackPlaying(unsigned int trackKey)
{
    d("SND_i_IsTrackPlaying(%u)=%d\n", trackKey, sm_track_playing);
    return sm_track_playing;
}

void SND_Stop(int instanceIdx) { d("SND_Stop(%d)\n", instanceIdx); }

void SND_SetTrack(int instanceIdx, unsigned int trackID)
{
    d("SND_SetTrack(%d,%u)\n", instanceIdx, trackID);
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
        SND_gst_Params.soundInstances[instanceIdx].trackID = (int)trackID;
}

void SND_PlayMdF(struct sSoundMdfStruct* sndMdf) { d("SND_PlayMdF(inst=%d)\n", sndMdf->sndInstanceID); }
void SND_PlaySound(int instanceIdx) { d("SND_PlaySound(%d)\n", instanceIdx); }
void SND_PlaySoundLooping(int instanceIdx) { d("SND_PlaySoundLooping(%d)\n", instanceIdx); }
void SND_PlaySoundOnTrack(int instanceIdx) { d("SND_PlaySoundOnTrack(%d)\n", instanceIdx); }
void SND_PlaySoundLoopingOnTrack(int instanceIdx) { d("SND_PlaySoundLoopingOnTrack(%d)\n", instanceIdx); }

/* ------------------------------------------------------------- state */

static unsigned rng;
static unsigned r32(void) { rng = rng * 1664525u + 1013904223u; return rng >> 8; }
static int ri(int n) { return (int)(r32() % (unsigned)n); }
static float rf(float lo, float hi) { return lo + (hi - lo) * (float)(r32() % 10001u) / 10000.0f; }

static void table_init(void)
{
    SND_gst_Params.soundEnabled = 1;
    SND_SModifierInitModule();
    SND_InitMdFModule();
}

static void state_init(unsigned seed)
{
    int i;

    rng = seed * 2654435761u + 12345u;
    memset(&SND_gst_Params, 0, sizeof(SND_gst_Params));
    memset(gao_store, 0, sizeof(gao_store));
    memset(mtx_store, 0, sizeof(mtx_store));
    memset(pos_store, 0, sizeof(pos_store));
    memset(inst_mtx_store, 0, sizeof(inst_mtx_store));
    memset(&ref_mtx, 0, sizeof(ref_mtx));
    memset(&bank_store, 0, sizeof(bank_store));
    memset(bank_indices, 0, sizeof(bank_indices));
    sm_track_playing = 0;

    SND_gst_Params.soundEnabled = 1;
    SND_gst_Params.allocatedSoundsMaxNum = SOUNDS;
    SND_gst_Params.allocatedSounds = (tSndSoundElem*)MEM_p_Alloc(SOUNDS * sizeof(tSndSoundElem));
    SND_gst_Params.soundInstanceNum = INSTANCES;
    SND_gst_Params.soundInstances = (tSoundInstanceElem*)MEM_p_Alloc(INSTANCES * sizeof(tSoundInstanceElem));
    SND_gst_Params.sndMatrixRef = &ref_mtx;

    ref_mtx.Pos.x = rf(-20.0f, 20.0f);
    ref_mtx.Pos.y = rf(-20.0f, 20.0f);
    ref_mtx.Pos.z = rf(-20.0f, 20.0f);

    for (i = 0; i < 2; ++i)
        gao_store[i].matrices = &mtx_store[i];

    for (i = 0; i < INSTANCES; ++i)
    {
        pos_store[i].x = rf(-20.0f, 20.0f);
        pos_store[i].y = rf(-20.0f, 20.0f);
        pos_store[i].z = rf(-20.0f, 20.0f);
        inst_mtx_store[i].v1.x = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v1.y = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v1.z = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v2.x = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v2.y = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v2.z = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v3.x = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v3.y = rf(-1.0f, 1.0f);
        inst_mtx_store[i].v3.z = rf(-1.0f, 1.0f);
    }

    table_init();
}

static void dump_chain(const char* tag, tSndModifierElem* list)
{
    int n = 0;
    tSndModifierElem* m;
    for (m = list; m && n < 16; m = m->nextModifier, ++n)
    {
        d("  %s[%d] at=%d intf=%d type=%d size=%d data=%d\n", tag, n, obj_id(m),
          obj_id(m->sndModifierModelIntf),
          m->sndModifierModelIntf ? m->sndModifierModelIntf->Type : -1,
          m->linkedSoundDataSize, obj_id(m->linkedSoundData));
        if (m->linkedSoundData && m->linkedSoundDataSize > 0 && m->linkedSoundDataSize <= 128)
        {
            int i;
            d("    ");
            for (i = 0; i < m->linkedSoundDataSize; i += 4)
            {
                unsigned u;
                memcpy(&u, m->linkedSoundData + i, 4);
                d("%08X ", u);
            }
            d("\n");
        }
    }
}

static void dump_instances(void)
{
    int i;
    for (i = 0; i < SND_gst_Params.soundInstanceNum; ++i)
    {
        tSoundInstanceElem* e = &SND_gst_Params.soundInstances[i];
        d("I%d s=%d fl=%08X fam=%08X trk=%d freq=%d vol=%08X pan=%d "
          "n=%08X,%08X,%08X f=%08X,%08X,%08X dt=%08X fc=%08X mc=%08X "
          "mtx=%d pos=%d gao=%d mdf=%d rem=%08X\n",
          i, e->allocatedSoundsIdx, e->flags, e->familyBitmask, e->trackID, e->freq,
          fbits(e->volume), e->pan,
          fbits(e->nearArr[0]), fbits(e->nearArr[1]), fbits(e->nearArr[2]),
          fbits(e->farArr[0]), fbits(e->farArr[1]), fbits(e->farArr[2]),
          fbits(e->dtFar), fbits(e->farCoefficient), fbits(e->middleCoefficient),
          obj_id(e->sndPosMtxSetInSetPos), obj_id(e->sndPositionSetInSetPosPtr),
          obj_id(e->gao), obj_id(e->modifierInstances), fbits(e->timeRemainTillStart));
        dump_chain("inst", e->modifierInstances);
    }
}

static void dump_sounds(void)
{
    int i;
    for (i = 0; i < SND_gst_Params.allocatedSoundsMaxNum; ++i)
    {
        tSndSoundElem* s = &SND_gst_Params.allocatedSounds[i];
        d("S%d k=%d fl=%08X mf=%08X use=%d freq=%d buf=%d intf=%d mdf=%d start=%d len=%u\n",
          i, s->soundKey, s->flags, s->muteFacorFlags, s->usageCnt, s->freq,
          (int)sm_heap_off(s->sndSndBufElem), obj_id(s->sndInterface),
          obj_id(s->soundModifier), s->startDataPos, s->fullsoundLen);
        dump_chain("snd", s->soundModifier);
    }
}

static void dump_templates(void)
{
    int i;
    d("BankList=%u users=%d\n", SND_gst_BankMdFUserList, (int)sm_heap_off(sndMdfBankUser));
    for (i = 0; i < 7; ++i)
    {
        tSndModifierElem* m = &SND_gs_SoundModifiers[i];
        tSoundModifierElemIntf* f = m->sndModifierModelIntf;
        d("M%d intf=%d type=%d next=%d size=%d data=%d\n", i, obj_id(f),
          f ? f->Type : -1, obj_id(m->nextModifier), m->linkedSoundDataSize,
          obj_id(m->linkedSoundData));
        if (f)
            d("   init=%d chain=%d upd=%d dtor=%d aos=%d aoi=%d np=%d npa=%d ns=%d\n",
              f->Init != 0, f->InitChainedElemVersion != 0, f->Update != 0,
              f->Destroy != 0, f->PlayApplyOnSound != 0, f->PlayApplyOnInstance != 0,
              f->NotifyPlay != 0, f->NotifyPause != 0, f->NotifyStop != 0);
        if (m->linkedSoundData && m->linkedSoundDataSize > 0 && m->linkedSoundDataSize <= 128)
        {
            int k;
            d("   ");
            for (k = 0; k < m->linkedSoundDataSize; k += 4)
            {
                unsigned u;
                memcpy(&u, m->linkedSoundData + k, 4);
                d("%08X ", u);
            }
            d("\n");
        }
    }
}

static void dump_mdf(tSoundMdfStruct* g)
{
    d("G i1=%d fl=%08X mid=%d i4=%d vol=%08X vol2=%08X ii2=%08X pre=%08X ii4=%08X "
      "ii5=%d freq2=%d pan=%d fl2=%08X ii11=%08X idx=%d inst=%d "
      "n=%08X,%08X,%08X f=%08X,%08X,%08X act=%08X trk=%d nf=%08X,%08X,%08X "
      "xx=%08X f4=%08X b0=%08X si=%d bank=%d rem=%08X\n",
      g->i1, g->flags, g->modifierID, g->i4, fbits(g->volume), fbits(g->volume2),
      fbits(g->ii2), fbits(g->distPrefetchVolume), fbits(g->ii4), g->ii5, g->freq2,
      g->pan, g->flags2, g->ii11, g->idx, g->sndInstanceID,
      fbits(g->near1), fbits(g->near2), fbits(g->near3),
      fbits(g->far1), fbits(g->far2), fbits(g->far3),
      fbits(g->unkIsInActiveVolume), g->trackID,
      fbits(g->nearFarFactor), fbits(g->nearFar2factor), fbits(g->nearFar3factor),
      fbits(g->xx), fbits(g->f4), fbits(g->b0), g->soundIndex, obj_id(g->soundBank),
      fbits(g->timeRemainTillStart));
}

/* ------------------------------------------------------------ groups */

/* 1. the module tables */
static void g_tables(int sub)
{
    state_init(1000u + sub);
    if (sub & 1)
    {
        SND_gst_Params.soundEnabled = 0;
        d("InitMdF=%d\n", SND_InitMdFModule());
    }
    SND_SModifierInitModule();
    dump_templates();
}

/* 2. chain insert, get, delete */
static void g_chain(int sub)
{
    tSndModifierElem* list = 0;
    tSoundModifierDataFade fade;
    tSoundModifierDataPlayer player;
    tSoundModifierDataExtPlayer ext;
    tSoundModifierData3DSettings set3d;
    tSoundModifierDataEmpty empty;
    int i, n;

    state_init(2000u + sub);

    memset(&fade, 0, sizeof(fade));
    memset(&player, 0, sizeof(player));
    memset(&ext, 0, sizeof(ext));
    memset(&set3d, 0, sizeof(set3d));
    memset(&empty, 0, sizeof(empty));

    fade.i1 = rf(0.0f, 1.0f); fade.i2 = rf(0.0f, 1.0f);
    fade.i3 = rf(0.0f, 4.0f); fade.i5 = (int[]){1, 2, 0x80, 0x90}[ri(4)];
    player.vol = rf(-0.5f, 1.5f); player.freq = ri(80000) - 20000;
    player.fnear = rf(-5.0f, 5.0f); player.ffar = rf(-50.0f, 50.0f);
    player.i6 = ri(4); player.timeRemainTillStart = rf(-1.0f, 2000000.0f);
    ext.i25 = ri(5); ext.trackID = ri(3) - 1; ext.gen.flags = (int)r32();
    ext.i6 = rf(0.0f, 1.0f); ext.i7 = rf(0.0f, 1.0f); ext.i1 = (int)r32();
    set3d.i1 = rf(-0.5f, 1.5f); set3d.farCoefficient = rf(0.0f, 1.0f);
    set3d.middleCoefficient = rf(0.0f, 1.0f); set3d.nearfar = rf(0.0f, 10.0f);
    set3d.gen.flags = (int)r32();

    n = 2 + ri(6);
    for (i = 0; i < n; ++i)
    {
        int type = ri(9) - 1;   /* -1 and 7, 8 exercise the range guard */
        void* data = 0;
        int size = 0;
        switch (type)
        {
        case 0: data = &empty;  size = sizeof(empty); break;
        case 1: case 3: case 4: data = &fade; size = 36; break;
        case 2: data = &player; size = (int[]){28, 36, 40}[ri(3)]; break;
        case 5: data = &ext;    size = (int[]){84, 108, 112, 116}[ri(4)]; break;
        case 6: data = &set3d;  size = sizeof(set3d); break;
        default: data = &empty; size = sizeof(empty); break;
        }
        d("insert type=%d size=%d -> %d\n", type, size,
          obj_id(SND_pst_SModifierInsertOneSModifier(&list, type, data, size)));
    }
    dump_chain("A", list);

    SND_SModifierDeleteAllLinkedSound(list);
    if (sub & 1)
    {
        int type = ri(7);
        d("delete one type=%d\n", type);
        SND_SModifierDeleteOneSModifier(&list, type);
        dump_chain("B", list);
    }
    SND_SModifierDeleteAll(&list);
    d("after all list=%d\n", obj_id(list));
}

/* 3. the .smd reader
 *
 * A record is {type, len} followed by len bytes of that modifier's own
 * data struct, whose first three words are the generic header. A record
 * with len 0 hands the modifier's Init a null pointer and every one of
 * the seven dereferences it, so the fixture always sizes the payload to
 * the type.
 */
static unsigned char smd_blob[768];
static char smd_file[1024];

static int smd_size_for(int type)
{
    switch (type)
    {
    case 0: return (int)sizeof(tSoundModifierDataEmpty);
    case 1: case 3: case 4: return (int)sizeof(tSoundModifierDataFade);
    case 2: return (int)sizeof(tSoundModifierDataPlayer);
    case 5: return (int)sizeof(tSoundModifierDataExtPlayer);
    default: return (int)sizeof(tSoundModifierData3DSettings);
    }
}

static void g_addsmd(int sub)
{
    int n, i, w, size, key;
    char* cur;

    state_init(3000u + sub);
    sm_bigfile_clear();
    memset(smd_blob, 0, sizeof(smd_blob));
    memset(smd_file, 0, sizeof(smd_file));

    *(int*)(smd_blob + 0) = 1;      /* version word */
    *(int*)(smd_blob + 4) = 0;
    w = 8;
    n = 1 + ri(3);
    for (i = 0; i < n; ++i)
    {
        int type = ri(7);
        int len = smd_size_for(type);
        int flags = (int[]){0, 8, 0x2000, 0x4000, 0x80000}[ri(5)];
        if (w + 8 + len + 8 > (int)sizeof(smd_blob))
            break;
        *(int*)(smd_blob + w) = type;
        *(int*)(smd_blob + w + 4) = len;
        w += 8;
        /* the payload is the modifier's own data struct, generic header
           first: sndKey, flags, sndIndexInAllocatedSounds */
        *(int*)(smd_blob + w + 0) = 0x1000 + ri(4);
        *(int*)(smd_blob + w + 4) = flags;
        *(int*)(smd_blob + w + 8) = 0;
        if (type == 1 || type == 3 || type == 4)
        {
            tSoundModifierDataFade f;
            memset(&f, 0, sizeof(f));
            f.gen.sndKey = *(int*)(smd_blob + w);
            f.gen.flags = flags;
            f.i1 = rf(0.0f, 1.0f);
            f.i2 = rf(0.0f, 1.0f);
            f.i3 = rf(0.1f, 2.0f);
            f.i5 = (int[]){1, 2, 0x80}[ri(3)] | 0x10;
            memcpy(smd_blob + w, &f, sizeof(f));
        }
        else if (type == 2)
        {
            tSoundModifierDataPlayer pl;
            memset(&pl, 0, sizeof(pl));
            pl.gen.sndKey = *(int*)(smd_blob + w);
            pl.gen.flags = flags;
            pl.freq = 44100;
            pl.vol = rf(0.0f, 1.0f);
            pl.fnear = rf(-4.0f, 4.0f);
            pl.ffar = rf(-40.0f, 40.0f);
            pl.i6 = ri(4);
            memcpy(smd_blob + w, &pl, sizeof(pl));
        }
        else if (type == 5)
        {
            tSoundModifierDataExtPlayer ex;
            memset(&ex, 0, sizeof(ex));
            ex.gen.sndKey = *(int*)(smd_blob + w);
            ex.gen.flags = flags;
            ex.i25 = ri(5);
            ex.trackID = ri(3) - 1;
            ex.vol = rf(0.0f, 1.0f);
            ex.i1 = (int)(r32() & 0xFFFu);
            ex.i3 = (int)(r32() & 0x3Fu);
            memcpy(smd_blob + w, &ex, sizeof(ex));
        }
        else if (type == 6)
        {
            tSoundModifierData3DSettings st;
            memset(&st, 0, sizeof(st));
            st.gen.sndKey = *(int*)(smd_blob + w);
            st.gen.flags = flags;
            st.i1 = rf(-0.2f, 1.2f);
            st.farCoefficient = rf(0.0f, 1.0f);
            st.middleCoefficient = rf(0.0f, 1.0f);
            st.nearfar = rf(0.0f, 9.0f);
            memcpy(smd_blob + w, &st, sizeof(st));
        }
        w += len;
    }
    *(int*)(smd_blob + w) = -1;     /* terminator type */
    *(int*)(smd_blob + w + 4) = 0;
    w += 8;
    size = w;
    key = 0x900 + (sub / 2);

    if (sub & 1)
    {
        sm_speed_mode = 1;
        memcpy(smd_file, &size, 4);
        memcpy(smd_file + 4, smd_blob, (size_t)size);
        cur = smd_file;
        sm_bigfile_add((unsigned)key, smd_file, size + 4);
        d("Add(speed) = %d\n", SND_l_AddSModifierNoLoad(key, (sub & 2) ? &cur : 0));
    }
    else
    {
        sm_speed_mode = 0;
        memcpy(smd_file, &size, 4);
        memcpy(smd_file + 4, smd_blob, (size_t)size);
        sm_bigfile_add((unsigned)key, smd_file, size + 4);
        d("Add(big) = %d\n", SND_l_AddSModifierNoLoad(key, 0));
        d("Add again = %d\n", SND_l_AddSModifierNoLoad(key, 0));
    }
    d("missing key = %d\n", SND_l_AddSModifierNoLoad(0x1234567, 0));
    d("zero key = %d\n", SND_l_AddSModifierNoLoad(0, 0));
    d("minus one = %d\n", SND_l_AddSModifierNoLoad(-1, 0));
    sm_speed_mode = 0;
    dump_sounds();
}

/* 3b. the 8-byte file the reader rejects */
static void g_addsmd_bad(int sub)
{
    static char filebuf[64];
    int size = (sub & 1) ? 8 : 12;
    int body[3];

    state_init(3500u + sub);
    sm_bigfile_clear();
    body[0] = (sub & 2) ? 1 : 7;       /* a wrong version word on half of them */
    body[1] = 0;
    body[2] = -1;
    memcpy(filebuf, &size, 4);
    memcpy(filebuf + 4, body, sizeof(body));
    sm_speed_mode = 0;
    sm_bigfile_add(0x700u, filebuf, size + 4);
    d("Add = %d\n", SND_l_AddSModifierNoLoad(0x700, 0));
    dump_sounds();
}

/* 4. MDF_SndCreateInstance */
static void g_create(int sub)
{
    tSoundMdfStruct g;
    int i;

    state_init(4000u + sub);
    memset(&g, 0, sizeof(g));

    /* the first eight subcases drive one flags2 bit each on purpose */
    if (sub < 8)
        g.flags2 = (int)(0x1000000u << (sub % 5)) | ((sub & 8) ? 0 : 0);
    else
        g.flags2 = (int)(r32() & 0x1F000000u);
    if (sub < 8)
        g.flags2 = (int[]){0x10000000, 0x1000000, 0x2000000, 0x4000000,
                           0x8000000, 0x11000000, 0x1F000000, 0}[sub];

    g.idx = (sub % 13 == 12) ? -1 : ri(SOUNDS);
    g.volume = rf(0.0f, 1.0f);
    g.volume2 = rf(0.0f, 1.0f);
    g.ii2 = (sub % 3) ? rf(0.0f, 3.0f) : 0.0f;
    g.ii4 = (sub % 4) ? rf(0.0f, 3.0f) : 0.0f;
    g.ii5 = ri(4) ? (int)(r32() & 0xFFu) : 0;
    g.freq2 = ri(2) ? 22050 : 0;
    g.pan = ri(2) ? ri(200) - 100 : 0;
    g.ii11 = (int)(r32() & 0xFFFFu);
    g.flags = (int)(r32() & 0x7FFu);
    g.trackID = (int[]){-1, 0, 7, 1040}[ri(4)];
    g.near1 = rf(0.0f, 5.0f);  g.far1 = rf(5.0f, 50.0f);
    g.near2 = rf(0.0f, 5.0f);  g.far2 = rf(5.0f, 50.0f);
    g.near3 = rf(0.0f, 5.0f);  g.far3 = rf(5.0f, 50.0f);
    g.nearFarFactor = rf(0.0f, 1.0f);
    g.nearFar2factor = rf(0.0f, 1.0f);
    g.nearFar3factor = rf(0.0f, 1.0f);
    g.unkIsInActiveVolume = rf(0.0f, 4.0f);
    g.xx = rf(0.0f, 1.0f);
    g.f4 = rf(0.0f, 1.0f);
    g.b0 = rf(0.0f, 9.0f);
    g.i4 = 0x5000 + ri(8);
    g.sndInstanceID = -1;
    sm_track_playing = ri(2);

    for (i = 0; i < SOUNDS; ++i)
    {
        SND_gst_Params.allocatedSounds[i].flags = 0x10400;
        SND_gst_Params.allocatedSounds[i].soundKey = 0x4000 + i;
    }

    scrub_stack();
    MDF_SndCreateInstance(&g, &gao_store[sub & 1]);
    dump_mdf(&g);
    dump_instances();
}

/* 5. the active and prefetch volume tests */
static void g_volume(int sub)
{
    tSoundMdfStruct g;
    tSoundInstanceElem* e;
    int idx;

    state_init(5000u + sub);
    memset(&g, 0, sizeof(g));
    idx = ri(INSTANCES);
    g.sndInstanceID = (sub % 11 == 10) ? INSTANCES + 3 : idx;
    if (sub % 17 == 16) g.sndInstanceID = -1;
    g.far1 = rf(0.0f, 40.0f);
    g.distPrefetchVolume = rf(0.0f, 60.0f);

    e = &SND_gst_Params.soundInstances[idx];
    e->flags = 0x400;
    /* the first eight subcases drive one axis-flag combination each */
    if (sub < 8)
        e->flags |= (int[]){0x10000000, 0x1000000, 0x2000000, 0x4000000,
                            0x3000000, 0x5000000, 0x7000000, 0}[sub];
    else
        e->flags |= (int)(r32() & 0x17000000u);
    e->sndPositionSetInSetPosPtr = (sub % 13 == 12) ? 0 : &pos_store[idx];
    e->sndPosMtxSetInSetPos = (sub % 7 == 6) ? 0 : &inst_mtx_store[idx];
    e->farArr[0] = rf(0.0f, 20.0f);
    e->farArr[1] = rf(0.0f, 20.0f);
    e->farArr[2] = rf(0.0f, 20.0f);
    e->dtFar = rf(0.0f, 5.0f);
    if (sub % 19 == 18) SND_gst_Params.sndMatrixRef = 0;

    d("active=%d prefetch=%d\n", MDF_b_SndIsInActiveVolume(&g),
      MDF_b_SndIsInPrefetchVolume(&g));
}

/* 6. the bank lookup */
static void g_bank(int sub)
{
    tSoundMdfStruct g;
    int i;

    state_init(6000u + sub);
    memset(&g, 0, sizeof(g));
    for (i = 0; i < 8; ++i)
        bank_indices[i] = (sub % 5 == 4) ? SOUNDS + i : ri(SOUNDS + 2) - 1;
    bank_store.elemNum = 8;
    bank_store.soundIndices = (sub % 7 == 6) ? 0 : bank_indices;
    for (i = 0; i < SOUNDS; ++i)
    {
        SND_gst_Params.allocatedSounds[i].flags = (unsigned)r32() & 0xFFFFFu;
        SND_gst_Params.allocatedSounds[i].soundKey = 0x4000 + i;
    }
    g.flags = (sub & 1) ? 0x800 : 0x400;
    g.idx = (sub & 2) ? -1 : 3;
    g.soundIndex = (sub % 11 == 10) ? 20 : ((sub % 5 == 3) ? bank_store.elemNum : ri(8));
    g.soundBank = (sub % 9 == 8) ? 0 : &bank_store;
    g.flags2 = (int)(r32() & 0xFFu);
    d("bank=%d\n", MDF_SndGetSoundIndexFromBank(&g));
    dump_mdf(&g);
}

/* 7. the fade modifier, initialised then stepped */
static void g_fade(int sub)
{
    tSndModifierElem elem;
    tSoundModifierDataFade data;
    tSoundInstanceElem* inst;
    int step, type;
    int mode = (int[]){1, 2, 0x80, 0x83}[sub & 3];

    state_init(7000u + sub);
    memset(&elem, 0, sizeof(elem));
    memset(&data, 0, sizeof(data));
    data.gen.sndKey = 0x77;
    data.gen.sndIndexInAllocatedSounds = ri(SOUNDS);
    data.i1 = (sub % 7 == 6) ? 0.0f : rf(-0.2f, 1.2f);
    data.i2 = (sub % 11 == 10) ? data.i1 : rf(-0.2f, 1.2f);
    data.i3 = (sub % 13 == 12) ? 0.0f : rf(0.05f, 2.0f);
    if (sub & 4) data.i3 = -data.i3;
    data.i5 = mode | 0x10 | ((sub & 8) ? 0x20 : 0) | ((sub & 16) ? 8 : 0);
    data.i6 = 0.0f;

    type = (int[]){1, 3, 4}[(sub / 32) % 3];
    elem.sndModifierModelIntf = SND_gs_SoundModifiers[type].sndModifierModelIntf;
    elem.sndModifierModelIntf->Init(&elem, &data, 36);
    d("after init: ");
    dump_chain("F", &elem);

    SND_l_Request(0, 0);
    inst = &SND_gst_Params.soundInstances[0];
    inst->volume = rf(0.0f, 1.0f);

    if (elem.sndModifierModelIntf->NotifyPlay)
        elem.sndModifierModelIntf->NotifyPlay(&elem, inst);

    TIM_gf_dt = (sub % 5 == 4) ? 0.5f : 0.03f;
    for (step = 0; step < 90; ++step)
    {
        if (step == 40 && (sub & 32) && elem.sndModifierModelIntf->NotifyPause)
            elem.sndModifierModelIntf->NotifyPause(&elem, inst);
        if (step == 60 && (sub & 64) && elem.sndModifierModelIntf->NotifyStop)
            elem.sndModifierModelIntf->NotifyStop(&elem, inst);
        if (elem.sndModifierModelIntf->Update)
            elem.sndModifierModelIntf->Update(&elem, inst, 0);
        d("s%02d vol=%08X i5=%08X i6=%08X\n", step, fbits(inst->volume),
          ((tSoundModifierDataFade*)elem.linkedSoundData)->i5,
          fbits(((tSoundModifierDataFade*)elem.linkedSoundData)->i6));
    }
    TIM_gf_dt = 0.0f;
    dump_instances();
}

/* 7b. the table arm driven past its last step on purpose */
static void g_fade_overrun(int sub)
{
    tSndModifierElem elem;
    tSoundModifierDataFade data;
    tSoundInstanceElem* inst;
    int step;
    int type = (sub & 1) ? 3 : 4;

    state_init(7500u + sub);
    memset(&elem, 0, sizeof(elem));
    memset(&data, 0, sizeof(data));
    data.i1 = 0.0f;
    data.i2 = 1.0f;
    data.i3 = 1.0f;
    data.i5 = 0x80 | 0x10;
    elem.sndModifierModelIntf = SND_gs_SoundModifiers[type].sndModifierModelIntf;
    elem.sndModifierModelIntf->Init(&elem, &data, 36);

    SND_l_Request(0, 0);
    inst = &SND_gst_Params.soundInstances[0];
    inst->volume = 0.5f;

    /* dt chosen so the step index lands exactly on 0, on 255, and one
       step past 255. The arm clears its own enable bit on the step after
       the duration runs out, so the run stops itself there; forcing the
       bit back on would walk the index far below zero and read a long way
       in front of the table, which is the defect the file carries a line
       about rather than something to drive. The restart below exercises
       the index from the top again. */
    TIM_gf_dt = (float[]){1.0f / 255.0f, 0.5f, 1.0f, 0.1f, 0.004f}[sub % 5];
    for (step = 0; step < 300; ++step)
    {
        if (step == 150 && elem.sndModifierModelIntf->NotifyPlay)
            elem.sndModifierModelIntf->NotifyPlay(&elem, inst);
        elem.sndModifierModelIntf->Update(&elem, inst, 0);
        d("o%03d vol=%08X i5=%08X i6=%08X\n", step, fbits(inst->volume),
          ((tSoundModifierDataFade*)elem.linkedSoundData)->i5,
          fbits(((tSoundModifierDataFade*)elem.linkedSoundData)->i6));
    }
    TIM_gf_dt = 0.0f;
}

/* 8. the player and extended player */
static void g_player(int sub)
{
    tSndModifierElem elem;
    tSoundModifierDataPlayer player;
    tSoundModifierDataExtPlayer ext;
    tSoundInstanceElem* inst;
    tSndSoundElem* snd;
    int size;

    state_init(8000u + sub);
    memset(&elem, 0, sizeof(elem));
    memset(&player, 0, sizeof(player));
    memset(&ext, 0, sizeof(ext));

    SND_l_Request(0, 0);
    inst = &SND_gst_Params.soundInstances[0];
    inst->gao = (sub & 1) ? &gao_store[0] : 0;
    snd = &SND_gst_Params.allocatedSounds[1];
    snd->flags = 0x10400;

    if (sub & 2)
    {
        player.gen.sndKey = 0x81;
        player.gen.flags = (int)(r32() & 0x1FFFFFu);
        player.gen.sndIndexInAllocatedSounds = ri(SOUNDS);
        player.freq = ri(90000) - 30000;
        player.vol = rf(-0.3f, 1.3f);
        player.fnear = rf(-9.0f, 9.0f);
        player.ffar = rf(-60.0f, 60.0f);
        player.flags = (int)(r32() & 0xFFFFu);
        player.i6 = (sub / 2) % 4;
        player.timeRemainTillStart = (sub & 4) ? 2000000.0f : rf(-2.0f, 5.0f);
        size = (int[]){28, 36, 40}[(sub / 8) % 3];
        elem.sndModifierModelIntf = SND_gs_SoundModifiers[2].sndModifierModelIntf;
        elem.sndModifierModelIntf->Init(&elem, &player, size);
        d("player init size=%d\n", size);
        dump_chain("P", &elem);
        elem.sndModifierModelIntf->PlayApplyOnInstance(&elem, elem.linkedSoundData, elem.linkedSoundDataSize, inst);
        elem.sndModifierModelIntf->PlayApplyOnSound(&elem, elem.linkedSoundData, elem.linkedSoundDataSize, snd);
        if (elem.sndModifierModelIntf->NotifyPlay)
            elem.sndModifierModelIntf->NotifyPlay(&elem, inst);
    }
    else
    {
        ext.gen.sndKey = 0x91;
        ext.gen.flags = (int)r32();
        ext.gen.sndIndexInAllocatedSounds = ri(SOUNDS);
        ext.i1 = (int)(r32() & 0xFFFFu);
        ext.i3 = (int)(r32() & 0x3Fu);
        ext.vol = rf(0.0f, 1.0f);
        ext.vol2 = rf(0.0f, 1.0f);
        ext.i6 = (sub & 4) ? rf(0.0f, 2.0f) : 0.0f;
        ext.i7 = (sub & 8) ? rf(0.0f, 2.0f) : 0.0f;
        ext.freq = (sub & 16) ? 32000 : 0;
        ext.pan = (sub & 32) ? 55 : 0;
        ext.fnear = rf(0.0f, 4.0f);  ext.ffar = rf(4.0f, 40.0f);
        ext.fnear2 = rf(0.0f, 4.0f); ext.ffar2 = rf(4.0f, 40.0f);
        ext.fnear3 = rf(0.0f, 4.0f); ext.ffar3 = rf(4.0f, 40.0f);
        ext.unkIsInActiveVolume = rf(0.0f, 4.0f);
        ext.familyID = (sub & 64) ? 0x33 : 0;
        ext.trackID = (int[]){-1, 0, 9}[(sub / 2) % 3];
        ext.interpolationFactor = (sub & 128) ? 0.5f : rf(0.0f, 1.0f);
        ext.interpolationFactor2 = rf(0.0f, 1.0f);
        ext.interpolationFactor3 = 0.5f;
        ext.farCoefficient = rf(0.0f, 1.0f);
        ext.middleCoefficient = rf(0.0f, 1.0f);
        ext.nearfar = rf(0.0f, 9.0f);
        ext.i25 = (sub / 4) % 5;
        ext.timeRemainTillStart = rf(0.0f, 3.0f);
        size = (int[]){84, 108, 112, 116}[(sub / 16) % 4];
        elem.sndModifierModelIntf = SND_gs_SoundModifiers[5].sndModifierModelIntf;
        elem.sndModifierModelIntf->Init(&elem, &ext, size);
        d("ext init size=%d\n", size);
        dump_chain("E", &elem);
        scrub_stack();
        elem.sndModifierModelIntf->PlayApplyOnInstance(&elem, elem.linkedSoundData, elem.linkedSoundDataSize, inst);
        elem.sndModifierModelIntf->PlayApplyOnSound(&elem, elem.linkedSoundData, elem.linkedSoundDataSize, snd);
        if (elem.sndModifierModelIntf->NotifyPlay)
            elem.sndModifierModelIntf->NotifyPlay(&elem, inst);
    }
    dump_instances();
    dump_sounds();
}

/* 9. the 3D settings modifier */
static void g_3d(int sub)
{
    tSndModifierElem elem;
    tSoundModifierData3DSettings set3d;
    tSoundInstanceElem* inst;

    state_init(9000u + sub);
    memset(&elem, 0, sizeof(elem));
    memset(&set3d, 0, sizeof(set3d));
    set3d.gen.sndKey = 0xA1;
    set3d.gen.flags = (int)r32();
    set3d.gen.sndIndexInAllocatedSounds = ri(SOUNDS);
    set3d.i1 = (int[]){-1, 0, 1, 2}[sub & 3] + rf(-0.2f, 0.2f);
    set3d.i2 = rf(0.0f, 1.0f);
    set3d.i3 = rf(0.0f, 1.0f);
    set3d.farCoefficient = rf(0.0f, 1.0f);
    set3d.middleCoefficient = rf(0.0f, 1.0f);
    set3d.nearfar = rf(0.0f, 9.0f);

    SND_l_Request(0, 0);
    inst = &SND_gst_Params.soundInstances[0];
    inst->nearArr[0] = rf(0.0f, 5.0f);
    inst->nearArr[1] = rf(0.0f, 5.0f);
    inst->nearArr[2] = rf(0.0f, 5.0f);
    inst->farArr[0] = rf(5.0f, 50.0f);
    inst->farArr[1] = rf(5.0f, 50.0f);
    inst->farArr[2] = rf(5.0f, 50.0f);

    elem.sndModifierModelIntf = SND_gs_SoundModifiers[6].sndModifierModelIntf;
    elem.sndModifierModelIntf->Init(&elem, &set3d, sizeof(set3d));
    dump_chain("D", &elem);
    elem.sndModifierModelIntf->PlayApplyOnInstance(&elem, elem.linkedSoundData, elem.linkedSoundDataSize, inst);
    dump_instances();
}

/* 10. a sound's modifier list instanced onto an instance */
static void g_instanciate(int sub)
{
    tSndModifierElem* list = 0;
    tSoundModifierDataFade fade;
    tSoundModifierDataPlayer player;
    tSoundModifierDataExtPlayer ext;
    tSoundInstanceElem* inst;
    int i, n;

    state_init(10000u + sub);
    memset(&fade, 0, sizeof(fade));
    memset(&player, 0, sizeof(player));
    memset(&ext, 0, sizeof(ext));

    fade.i1 = rf(0.0f, 1.0f); fade.i2 = rf(0.0f, 1.0f);
    fade.i3 = rf(0.1f, 2.0f); fade.i5 = (int[]){1, 2, 0x80}[ri(3)] | 0x10;
    player.vol = rf(0.0f, 1.0f); player.freq = 44100; player.i6 = ri(3);
    player.fnear = rf(0.0f, 4.0f); player.ffar = rf(4.0f, 40.0f);
    ext.i25 = 3; ext.trackID = -1; ext.vol = rf(0.0f, 1.0f);
    ext.gen.flags = (int)(r32() & 0x1F10000Fu);
    ext.i1 = (int)(r32() & 0xFFFu); ext.i3 = (int)(r32() & 0x3Fu);
    ext.ffar = 20.0f; ext.ffar2 = 20.0f; ext.ffar3 = 20.0f;
    ext.interpolationFactor = 0.5f;
    ext.interpolationFactor2 = 0.5f;
    ext.interpolationFactor3 = 0.5f;

    n = 1 + ri(4);
    for (i = 0; i < n; ++i)
    {
        int type = (int[]){1, 2, 3, 4, 5, 6, 0}[ri(7)];
        switch (type)
        {
        case 1: case 3: case 4:
            SND_pst_SModifierInsertOneSModifier(&list, type, &fade, 36); break;
        case 2:
            SND_pst_SModifierInsertOneSModifier(&list, type, &player, 40); break;
        case 5:
            SND_pst_SModifierInsertOneSModifier(&list, type, &ext, 116); break;
        default:
            SND_pst_SModifierInsertOneSModifier(&list, type, &fade, 12); break;
        }
    }
    dump_chain("src", list);

    SND_l_Request(0, 0);
    inst = &SND_gst_Params.soundInstances[0];
    inst->gao = &gao_store[0];
    scrub_stack();
    SND_SModifierInstanciate(list, inst);
    dump_instances();
}

/* 11. the per-axis active-volume test, driven on purpose
 *
 * The random geometry in group 5 almost always fails at least one
 * enabled axis, so the answer is 0 whichever axis the loop reads and a
 * wrong row or a wrong bound is invisible. Here the instance matrix is
 * the identity and the three far bounds are far apart, so the projection
 * on axis k is exactly |delta[k]| and each axis's decision shows on its
 * own. Several of the deltas land exactly on a bound, which is what
 * makes the >= visible, and the prefetch radius is set to the exact
 * distance on every third subcase for the same reason. The seed base
 * sits above every other group's so that inserting a case here cannot
 * shift another group's random stream.
 */
static void g_axis(int sub)
{
    tSoundMdfStruct g;
    tSoundInstanceElem* e;
    static const float delta[6] = { 2.0f, 7.0f, 12.0f, 10.0f, 5.0f, 15.0f };
    int idx = 0;
    int combo = (sub % 7) + 1;
    int pick = (sub / 7) % 6;
    int farTest = (sub >= 42);

    state_init(20000u + sub);
    memset(&g, 0, sizeof(g));
    g.sndInstanceID = idx;

    ref_mtx.Pos.x = 0.0f;
    ref_mtx.Pos.y = 0.0f;
    ref_mtx.Pos.z = 0.0f;
    if (farTest)
    {
        pos_store[idx].x = -3.0f;
        pos_store[idx].y = -4.0f;
        pos_store[idx].z = 0.0f;
    }
    else
    {
        pos_store[idx].x = -delta[pick];
        pos_store[idx].y = -delta[(pick + 1) % 6];
        pos_store[idx].z = -delta[(pick + 2) % 6];
    }

    memset(&inst_mtx_store[idx], 0, sizeof(Mtx4x4));
    inst_mtx_store[idx].v1.x = 1.0f;
    inst_mtx_store[idx].v2.y = 1.0f;
    inst_mtx_store[idx].v3.z = 1.0f;

    e = &SND_gst_Params.soundInstances[idx];
    e->flags = 0x400 | (farTest ? 0x10000000 : (combo << 24));
    e->sndPositionSetInSetPosPtr = &pos_store[idx];
    e->sndPosMtxSetInSetPos = &inst_mtx_store[idx];
    e->farArr[0] = 5.0f;
    e->farArr[1] = 10.0f;
    e->farArr[2] = 15.0f;
    e->dtFar = 0.0f;
    g.far1 = farTest ? 5.0f : 11.0f;
    g.distPrefetchVolume = (sub % 3 == 0)
        ? MATH_DistanceOfTwoPoints(&ref_mtx.Pos, &pos_store[idx])
        : 13.0f;

    d("combo=%d far=%d pos=(%08X,%08X,%08X) rad=%08X active=%d",
      combo, farTest, fbits(pos_store[idx].x), fbits(pos_store[idx].y),
      fbits(pos_store[idx].z), fbits(g.distPrefetchVolume),
      MDF_b_SndIsInActiveVolume(&g));
    d(" prefetch=%d\n", MDF_b_SndIsInPrefetchVolume(&g));
}

/* 12. the fade-out modifier, with its endpoints the right way round
 *
 * For a fade-out i1 is the level it starts from and i2 the floor it
 * stops at, and the arm's terminal test is volume <= i2 where the
 * fade-in's is volume >= i2. Group 7b ran type 4 with i1 = 0 and
 * i2 = 1, so the terminal test fired on the first update and the table
 * arm's own arithmetic was never compared -- a swap of its two
 * endpoints scored clean. The seed base sits above every other group's.
 */
static void g_fadeout(int sub)
{
    tSndModifierElem elem;
    tSoundModifierDataFade data;
    tSoundInstanceElem* inst;
    int step;
    int mode = (int[]){1, 2, 0x80}[sub % 3];

    state_init(21000u + sub);
    memset(&elem, 0, sizeof(elem));
    memset(&data, 0, sizeof(data));
    data.gen.sndKey = 0x44;
    data.gen.sndIndexInAllocatedSounds = 2;
    data.i1 = 1.0f;
    data.i2 = (sub % 5 == 4) ? 0.25f : 0.0f;
    data.i3 = (float[]){1.0f, 0.5f, 2.0f, 0.3f}[(sub / 3) % 4];
    data.i5 = mode | 0x10 | ((sub & 16) ? 0x20 : 0) | ((sub & 32) ? 8 : 0);
    data.i6 = 0.0f;

    elem.sndModifierModelIntf = SND_gs_SoundModifiers[4].sndModifierModelIntf;
    elem.sndModifierModelIntf->Init(&elem, &data, 36);
    dump_chain("O", &elem);

    SND_l_Request(0, 0);
    inst = &SND_gst_Params.soundInstances[0];
    inst->volume = 1.0f;
    if (elem.sndModifierModelIntf->NotifyPlay)
        elem.sndModifierModelIntf->NotifyPlay(&elem, inst);
    ((tSoundModifierDataFade*)elem.linkedSoundData)->i5 |= 0x10;

    TIM_gf_dt = (float[]){1.0f / 255.0f, 0.01f, 0.1f, 0.25f}[(sub / 12) % 4];
    for (step = 0; step < 300; ++step)
    {
        if (step == 200 && elem.sndModifierModelIntf->NotifyPause)
            elem.sndModifierModelIntf->NotifyPause(&elem, inst);
        elem.sndModifierModelIntf->Update(&elem, inst, 0);
        d("q%03d vol=%08X i5=%08X i6=%08X\n", step, fbits(inst->volume),
          ((tSoundModifierDataFade*)elem.linkedSoundData)->i5,
          fbits(((tSoundModifierDataFade*)elem.linkedSoundData)->i6));
    }
    TIM_gf_dt = 0.0f;
    dump_instances();
}

/* ------------------------------------------------------------ driver */

#define SUB 48

static void (*const groups[])(int) = {
    g_tables, g_chain, g_addsmd, g_addsmd_bad, g_create,
    g_volume, g_bank, g_fade, g_fade_overrun, g_player, g_3d, g_instanciate,
    g_axis, g_fadeout
};

#define NGROUPS ((int)(sizeof(groups) / sizeof(groups[0])))

int SIDE_CASES(void) { return NGROUPS * SUB; }

void SIDE_RUN(int c)
{
    int g = c / SUB;
    int sub = c % SUB;
    TIM_gf_dt = 0.0f;
    groups[g](sub);
}
