/* See sound_mocks.h. */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "sound_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../textget.h"
#include "../dbg.h"
#include "../gameobject.h"
#include "../modifiers/gensoundmodifier.h"
#include "../win/winsound.h"

/* ---------------------------------------------------------------- log */

#define LOG_MAX (8 * 1024 * 1024)
static char log_buf[LOG_MAX];
static int log_len;

void snd_log(const char* fmt, ...)
{
    va_list ap;
    int n;

    if (log_len >= LOG_MAX - 512)
        return;
    va_start(ap, fmt);
    n = vsnprintf(log_buf + log_len, LOG_MAX - log_len, fmt, ap);
    va_end(ap);
    if (n > 0)
        log_len += n;
}

const char* snd_log_text(void) { return log_buf; }
int snd_log_len(void) { return log_len; }

/* ---------------------------------------------------------- allocator */

/* Leaks on purpose: the first side's state has to stay readable while the
   second side runs, and both sides must see the same addresses, so nothing
   is ever handed back and the arena is rewound between the two runs. Each
   block sits 64 bytes into a larger one so that a copy that writes a little
   way in front of its block does not corrupt the arena's bookkeeping. */
#define HEAP_MAX (24 * 1024 * 1024)
#define GUARD 64
static unsigned char heap[HEAP_MAX];
static int heap_used;
static int alloc_seq;

typedef struct sBlockHdr { int size; int pad[3]; } tBlockHdr;

const unsigned char* snd_heap_base(void) { return heap; }
int snd_heap_used(void) { return heap_used; }

/* Blocks get twice the room they ask for plus a slack of GUARD bytes.
   Several of the engine's arrays are pointer arrays sized at four bytes a
   pointer, which is right in its own 32-bit build and half of what this
   harness needs; the headroom makes those writes land inside the block
   instead of over the next one, and the realloc below carries the whole
   of it across so a regrown pointer array is not truncated. */
static unsigned int room_for(unsigned int size)
{
    return ((2u * size + GUARD) + 15u) & ~15u;
}

static void* heap_alloc(unsigned int size)
{
    int need = (int)room_for(size) + GUARD + (int)sizeof(tBlockHdr);
    unsigned char* p;

    if (heap_used + need > HEAP_MAX)
    {
        fprintf(stderr, "mock heap exhausted\n");
        exit(2);
    }
    p = heap + heap_used + GUARD;
    ((tBlockHdr*)p)->size = (int)size;
    heap_used += need;
    return p + sizeof(tBlockHdr);
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = heap_alloc(size);
    snd_log("MEM_p_Alloc(%u)=#%d\n", size, ++alloc_seq);
    return p;
}

void* MEM_p_Realloc(void* old, size_t size)
{
    void* p;
    int oldSize;

    snd_log("MEM_p_Realloc(%+d,%u)\n", old ? 1 : 0, size);
    if (!old)
        return heap_alloc(size);
    oldSize = ((tBlockHdr*)((unsigned char*)old - sizeof(tBlockHdr)))->size;
    p = heap_alloc(size);
    {
        unsigned int carry = room_for((unsigned)oldSize);
        unsigned int cap = room_for(size);
        memcpy(p, old, carry < cap ? carry : cap);
    }
    return p;
}

void MEM_Free(void* p)
{
    snd_log("MEM_Free(%d)\n", p ? 1 : 0);
}

BOOL MEM_b_CheckBlockHasBeenFreed(void* p)
{
    return 0;
}

/* math.c's matrix array helpers; nothing in the sound paths reaches them. */
void* MEM_p_AllocAligned(int size, int alignment) { return heap_alloc((unsigned)size); }
void* MEM_p_ReallocAligned(void* p, unsigned int size, int alignment) { return MEM_p_Realloc(p, (size_t)size); }
void MEM_FreeAlign(void* p) { }

/* ------------------------------------------------------------ bigfile */

#define BIG_MAX (1024 * 1024)
static unsigned char big_arena[BIG_MAX];
static int big_used;

#define BIG_RECS 64
static struct { unsigned int key; int pos; int len; } big_recs[BIG_RECS];
static int big_rec_num;

void snd_bigfile_clear(void)
{
    big_used = 0;
    big_rec_num = 0;
    memset(big_arena, 0, sizeof(big_arena));
}

int snd_bigfile_add(unsigned int key, const void* data, int len)
{
    int pos = big_used + 4;      /* the engine always reads from pos + 4 */

    if (big_rec_num >= BIG_RECS || big_used + len + 4 > BIG_MAX)
    {
        fprintf(stderr, "mock bigfile exhausted\n");
        exit(2);
    }
    memcpy(big_arena + pos, data, len);
    big_recs[big_rec_num].key = key;
    big_recs[big_rec_num].pos = big_used;
    big_recs[big_rec_num].len = len;
    ++big_rec_num;
    big_used += len + 4;
    return big_used - len - 4;
}

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    int i;

    for (i = 0; i < big_rec_num; ++i)
    {
        if (big_recs[i].key == key)
            return big_recs[i].pos;
    }
    return -1;
}

char* BIG_pc_ReadFileTmp(int Offset, int* size)
{
    int i;

    for (i = 0; i < big_rec_num; ++i)
    {
        if (big_recs[i].pos == Offset)
        {
            *size = big_recs[i].len;
            snd_log("BIG_pc_ReadFileTmp(%d)=%d\n", Offset, big_recs[i].len);
            return (char*)big_arena + Offset + 4;
        }
    }
    *size = 0;
    snd_log("BIG_pc_ReadFileTmp(%d)=miss\n", Offset);
    return (char*)big_arena;
}

void BIG_ReadNoSeek(int pos, void* dest, unsigned int len)
{
    snd_log("BIG_ReadNoSeek(%d,%u)\n", pos, len);
    if (pos < 0 || pos + (int)len > BIG_MAX)
    {
        memset(dest, 0, len);
        return;
    }
    memcpy(dest, big_arena + pos, len);
}

/* -------------------------------------------------------- loader side */

static const unsigned char* stream_data;
static int stream_len;
static int stream_pos;

void snd_stream_set(const void* data, int len)
{
    stream_data = (const unsigned char*)data;
    stream_len = len;
    stream_pos = 0;
}

char* LOA_FetchBuffer(int size)
{
    char* p;

    snd_log("LOA_FetchBuffer(%d)@%d\n", size, stream_pos);
    if (!stream_data || stream_pos + size > stream_len)
        return (char*)big_arena;
    p = (char*)stream_data + stream_pos;
    stream_pos += size;
    return p;
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int v;

    memcpy(&v, *offset, 4);
    *offset += 4;
    return v;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    uint16_t v;

    memcpy(&v, *offset, 2);
    *offset += 2;
    return v;
}

char** LOA_ReadCharArray(char** offset, char* retVal, unsigned int len, char readMode)
{
    memcpy(retVal, *offset, len);
    *offset += len;
    return offset;
}

void LOA_BeginSpeedMode(int key) { snd_log("LOA_BeginSpeedMode(%d)\n", key); }
void LOA_EndSpeedMode(void) { snd_log("LOA_EndSpeedMode\n"); }
void LOA_DeleteAddress(int pos) { snd_log("LOA_DeleteAddress\n"); }
int LOA_ul_GetCurrentKey(void) { return 0x11223344; }

/* ------------------------------------------------------------- others */

int TEXT_i_GetLang(void) { return 2; }

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine,
                const char* error, const char* extra, int showMsgBox)
{
    snd_log("DBG_assert(%s)\n", error ? error : "");
}

void OBJ_GameObject_CreateExtendedIfNot(tGameObjectMain* gao)
{
    snd_log("OBJ_CreateExtendedIfNot(%d)\n", gao ? 1 : 0);
    if (gao && !gao->extended)
        gao->extended = (tGameObjectExtended*)MEM_p_Alloc(sizeof(tGameObjectExtended));
}

int MDF_SndGetSoundIndexFromBank(tSoundMdfStruct* gsm)
{
    snd_log("MDF_SndGetSoundIndexFromBank\n");
    return 0;
}

int SND_InitMdFModule(void) { snd_log("SND_InitMdFModule\n"); return 0; }
void SND_SModifierInitModule(void) { snd_log("SND_SModifierInitModule\n"); }
int SND_i_SoftInit(void) { snd_log("SND_i_SoftInit\n"); return -1; }

void SND_SModifierDeleteAll(tSndModifierElem** list)
{
    snd_log("SModifierDeleteAll(%d)\n", (list && *list) ? 1 : 0);
    if (list)
        *list = 0;
}

void SND_SModifierDeleteAllLinkedSound(tSndModifierElem* list)
{
    snd_log("SModifierDeleteAllLinkedSound(%d)\n", list ? 1 : 0);
}

void SND_SModifierInstanciate(tSndModifierElem* list, tSoundInstanceElem* inst)
{
    snd_log("SModifierInstanciate\n");
}

static void mk_mdf_apply_sound(tSndModifierElem* m, void* data, int size, tSndSoundElem* s)
{
    snd_log("mdf PlayApplyOnSound(%d)\n", size);
}

static void mk_mdf_apply_inst(tSndModifierElem* m, void* data, int size, tSoundInstanceElem* e)
{
    snd_log("mdf PlayApplyOnInstance(%d,%08X)\n", size, *(unsigned*)&e->volume);
}

static void mk_mdf_notify(tSndModifierElem* m, tSoundInstanceElem* e)
{
    snd_log("mdf notify\n");
}

static void mk_mdf_update(tSndModifierElem* m, tSoundInstanceElem* e, int w)
{
    snd_log("mdf update(%d)\n", w);
}

#define MDFS 32
static tSndModifierElem mdf_pool[MDFS];
static tSoundModifierElemIntf mdf_intf[7];
static int mdf_used;

tSndModifierElem* SND_pst_SModifierInsertOneSModifier(tSndModifierElem** list, int type,
                                                      void* linkedSound, int dataSize)
{
    tSndModifierElem* m;

    snd_log("SModifierInsert(%d,%d)\n", type, dataSize);
    if (type < 0 || type >= 7 || mdf_used >= MDFS)
        return list ? *list : 0;
    m = &mdf_pool[mdf_used++];
    m->sndModifierModelIntf = &mdf_intf[type];
    m->nextModifier = list ? *list : 0;
    m->linkedSoundData = (char*)linkedSound;
    m->linkedSoundDataSize = dataSize;
    if (list)
        *list = m;
    return m;
}

int SND_l_AddSModifierNoLoad(int soundKey, char** cursor)
{
    snd_log("AddSModifierNoLoad(%d)\n", soundKey);
    *cursor += 4;
    return -1;
}

struct sGameObjectMain** AI_MainActor;
static struct sGameObjectMain* ai_main_actor_slot;

/* ---------------------------------------------------- 15-bit rand ---- */
/* MATH_RandFloat and SND_PlayRq both scale rand() by 1/SHRT_MAX, which
   assumes MSVC's 15-bit rand; glibc's 31-bit one puts the values in a
   domain the engine never enters. */
static unsigned int rand_state;
void snd_srand(unsigned int seed) { rand_state = seed; }
int rand(void) { rand_state = rand_state * 214013u + 2531011u; return (rand_state >> 16) & 0x7FFF; }
void srand(unsigned int seed) { rand_state = seed; }

int _strnicmp(const char* a, const char* b, size_t n)
{
    size_t i;

    for (i = 0; i < n; ++i)
    {
        int ca = a[i], cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca += 32;
        if (cb >= 'A' && cb <= 'Z') cb += 32;
        if (ca != cb) return ca - cb;
        if (!ca) return 0;
    }
    return 0;
}

/* ------------------------------------------------- the sound device -- */

#define BUFS 128
static tSndSoundBufferElem bufs[BUFS];
static int buf_used;
static struct { int playing; int att; int freq; int pan; int front; int pos; } buf_state[BUFS];
static int play_seq;
int snd_play_fail_mod;
int snd_global_vol;
int snd_global_pan;

static int buf_index(tSndSoundBufferElem* b)
{
    if (!b) return -1;
    if (b >= bufs && b < bufs + BUFS) return (int)(b - bufs);
    return -2;              /* a stream handle, which is an int cast to a pointer */
}

static int mk_Init(tSndHandles* h) { snd_log("dev Init\n"); return 1; }
static void mk_Close(tSndHandles* h) { snd_log("dev Close\n"); }

static tSndSoundBufferElem* mk_Create(tSndHandles* h, tSndWavHdrStruct* w, int offset)
{
    tSndSoundBufferElem* b;

    if (buf_used >= BUFS) { snd_log("dev Create exhausted\n"); return 0; }
    b = &bufs[buf_used++];
    snd_log("dev Create(fmt=%d,ch=%d,rate=%d,bits=%d,cb=%d,size=%d,start=%d,off=%d)=%d\n",
            w->wfex.wFormatTag, w->wfex.nChannels, w->wfex.nSamplesPerSec,
            w->wfex.wBitsPerSample, w->wfex.cbSize, w->fullSoundSize,
            w->dataStartPos, offset, buf_used - 1);
    return b;
}

static tSndSoundBufferElem* mk_Reload(tSndHandles* h, tSndSoundBufferElem* b,
                                      tSndWavHdrStruct* w, int offset)
{
    snd_log("dev Reload(%d,fmt=%d,size=%d,off=%d)\n", buf_index(b),
            w->wfex.wFormatTag, w->fullSoundSize, offset);
    if (b) return b;
    return mk_Create(h, w, offset);
}

static int mk_Play(tSndSoundBufferElem* b, int flags)
{
    int idx = buf_index(b);

    ++play_seq;
    if (snd_play_fail_mod && (play_seq % snd_play_fail_mod) == 0)
    {
        snd_log("dev Play(%d,%08X)=fail\n", idx, flags);
        return -1;
    }
    if (idx >= 0) buf_state[idx].playing = 1;
    snd_log("dev Play(%d,%08X)=ok\n", idx, flags);
    return 0;
}

static void mk_Stop(tSndSoundBufferElem* b)
{
    int idx = buf_index(b);
    if (idx >= 0) buf_state[idx].playing = 0;
    snd_log("dev Stop(%d)\n", idx);
}

static void mk_Pause(tSndSoundBufferElem* b) { snd_log("dev Pause(%d)\n", buf_index(b)); }
static void mk_Release(tSndSoundBufferElem* b) { snd_log("dev Release(%d)\n", buf_index(b)); }

static void mk_Duplicate(tSndHandles* h, tSndSoundBufferElem* src, tSndSoundBufferElem** dest)
{
    snd_log("dev Duplicate(%d)\n", buf_index(src));
    if (buf_used >= BUFS) { *dest = 0; return; }
    *dest = &bufs[buf_used++];
}

static void mk_SetCurrPos(tSndSoundBufferElem* b, int pos)
{
    int idx = buf_index(b);
    if (idx >= 0) buf_state[idx].pos = pos;
    snd_log("dev SetCurrPos(%d,%d)\n", idx, pos);
}

static void mk_SetVolume(tSndSoundBufferElem* b, int att)
{
    int idx = buf_index(b);
    if (idx >= 0) buf_state[idx].att = att;
    snd_log("dev SetVolume(%d,%d)\n", idx, att);
}

static void mk_SetFrequency(tSndSoundBufferElem* b, int freq)
{
    int idx = buf_index(b);
    if (idx >= 0) buf_state[idx].freq = freq;
    snd_log("dev SetFrequency(%d,%d)\n", idx, freq);
}

static void mk_SetPan(tSndSoundBufferElem* b, int pan, int front)
{
    int idx = buf_index(b);
    if (idx >= 0) { buf_state[idx].pan = pan; buf_state[idx].front = front; }
    snd_log("dev SetPan(%d,%d,%d)\n", idx, pan, front);
}

static int mk_GetStatus(tSndSoundBufferElem* b, int* status)
{
    int idx = buf_index(b);

    if (idx >= 0)
        *status = buf_state[idx].playing | ((idx & 1) << 2) | ((idx & 2) << 3);
    else
        *status = 0;
    snd_log("dev GetStatus(%d)=%d\n", idx, *status);
    return 0;
}

static void mk_GetFrequency(tSndSoundBufferElem* b, int* freq)
{
    int idx = buf_index(b);
    *freq = idx >= 0 ? buf_state[idx].freq : 0;
    snd_log("dev GetFrequency(%d)=%d\n", idx, *freq);
}

static void mk_GetPan(tSndSoundBufferElem* b, int* pan, int* front)
{
    int idx = buf_index(b);
    *pan = idx >= 0 ? buf_state[idx].pan : 0;
    *front = idx >= 0 ? buf_state[idx].front : 0;
    snd_log("dev GetPan(%d)=%d,%d\n", idx, *pan, *front);
}

static void mk_GetCurrPos(tSndSoundBufferElem* b, int* currPos, int* writePos)
{
    int idx = buf_index(b);
    *currPos = idx >= 0 ? 1000 + 37 * idx : 0;
    *writePos = idx >= 0 ? 7 * idx : 0;
    snd_log("dev GetCurrPos(%d)=%d,%d\n", idx, *currPos, *writePos);
}

static void mk_CreateTrack(tSndSoundBufferElem** stereo, tSndSoundBufferElem** mono)
{
    if (buf_used + 1 >= BUFS) { *stereo = 0; *mono = 0; return; }
    *stereo = &bufs[buf_used++];
    *mono = &bufs[buf_used++];
    snd_log("dev CreateTrack(%d,%d)\n", buf_index(*stereo), buf_index(*mono));
}

static void mk_ReleaseTrack(tSndSoundBufferElem* s, tSndSoundBufferElem* m)
{
    snd_log("dev ReleaseTrack(%d,%d)\n", buf_index(s), buf_index(m));
}

static void mk_StreamGetMonoToStereo(tSndSoundBufferElem** a, tSndSoundBufferElem** b)
{
    tSndSoundBufferElem* t = *a;
    snd_log("dev MonoToStereo(%d,%d)\n", buf_index(*a), buf_index(*b));
    *a = *b;
    *b = t;
}

static void mk_StreamGetStereoToMono(tSndSoundBufferElem** a, tSndSoundBufferElem** b)
{
    tSndSoundBufferElem* t = *a;
    snd_log("dev StereoToMono(%d,%d)\n", buf_index(*a), buf_index(*b));
    *a = *b;
    *b = t;
}

static void mk_StreamPrefetch(int pos, int len) { snd_log("dev StreamPrefetch(%d,%d)\n", pos, len); }

static void mk_StreamPrefetchArray(int* positions, int* sizes)
{
    int i;

    snd_log("dev StreamPrefetchArray[");
    for (i = 0; i < 8; ++i)
        snd_log("%d:%d ", positions[i], sizes[i]);
    snd_log("|");
    /* only the owner slots the engine filled: the caller's memset clears the
       first 32 bytes of this array and leaves the owner half uninitialised,
       so the rest is stack garbage and differs between the two frames */
    for (i = 0; i < 8; ++i)
    {
        if (sizes[i])
            snd_log("%d ", positions[i + 8]);
    }
    snd_log("]\n");
    /* fail the odd-numbered slots, which is what drives the recovery walk */
    for (i = 0; i < 8; ++i)
    {
        if (positions[i] && (i & 1) == 0)
            positions[i] = 0;
    }
}

static void mk_StreamFlush(int pos) { snd_log("dev StreamFlush(%d)\n", pos); }

static void mk_StreamReinitAndPlay(tSndSoundBufferElem* b, int len, int startPos,
                                   int from, int to, char flags, int freq, int pan, int att)
{
    snd_log("dev StreamReinitAndPlay(%d,%d,%d,%d,%d,%d,%d,%d,%d)\n",
            buf_index(b), len, startPos, from, to, (int)(unsigned char)flags, freq, pan, att);
}

static void mk_ShutdownStreams(void) { snd_log("dev ShutdownStreams\n"); }
static void mk_nullptr1(void) { snd_log("dev nullptr1\n"); }
static void mk_SetGlobalVol(int att) { snd_global_vol = att; snd_log("dev SetGlobalVol(%d)\n", att); }
static int mk_GetGlobalVol(void) { snd_log("dev GetGlobalVol\n"); return snd_global_vol; }
static void mk_SetGlobalPan(int pan) { snd_global_pan = pan; snd_log("dev SetGlobalPan(%d)\n", pan); }
static int mk_GetGlobalPan(void) { snd_log("dev GetGlobalPan\n"); return snd_global_pan; }

static void mk_ComputeDoppler(Vec3* d, Vec3* p, tSoundInstanceElem* inst)
{
    snd_log("dev ComputeDoppler(%08X,%08X,%08X)\n",
            *(unsigned*)&d->x, *(unsigned*)&p->x, *(unsigned*)&inst->volume);
}

static float mk_Compute3DVol(tSoundInstanceElem* inst)
{
    float v = inst->volume * 0.75f + 0.125f;
    snd_log("dev Compute3DVol=%08X\n", *(unsigned*)&v);
    return v;
}

static void mk_SNDNull(Vec3* v) { snd_log("dev SNDNull(%08X)\n", *(unsigned*)&v->x); }
static int mk_ChangeRenderMode(int mode) { snd_log("dev ChangeRenderMode(%d)\n", mode); return mode; }
static int mk_FxInit(void) { snd_log("dev FxInit\n"); return 0; }
static void mk_FxClose(void) { snd_log("dev FxClose\n"); }
static int mk_FxSetMode(tSoundFxParams* p, int v) { snd_log("fx SetMode(%d)\n", v); return v; }
static int mk_FxGetMode(tSoundFxParams* p) { snd_log("fx GetMode\n"); return p->mode; }
static int mk_FxSetDelay(tSoundFxParams* p, int v) { snd_log("fx SetDelay(%d)\n", v); return v; }
static int mk_FxGetDelay(tSoundFxParams* p) { snd_log("fx GetDelay\n"); return p->delay; }
static int mk_FxSetFeedback(tSoundFxParams* p, int v) { snd_log("fx SetFeedback(%d)\n", v); return v; }
static int mk_FxGetFeedback(tSoundFxParams* p) { snd_log("fx GetFeedback\n"); return p->feedback; }
static int mk_FxSetWetVolume(tSoundFxParams* p, int v) { snd_log("fx SetWetVolume(%d)\n", v); return v; }
static int mk_FxGetWetVolume(tSoundFxParams* p) { snd_log("fx GetWetVolume\n"); return 0; }
static int mk_FxSetWetPan(tSoundFxParams* p, int v) { snd_log("fx SetWetPan(%d)\n", v); return v; }
static int mk_FxGetWetPan(tSoundFxParams* p) { snd_log("fx GetWetPan\n"); return p->wetPan; }
static int mk_FxEnable(tSoundFxParams* p) { snd_log("fx Enable\n"); return 1; }
static int mk_FxDisable(tSoundFxParams* p) { snd_log("fx Disable\n"); return 2; }

tSndInterfaceStruct snd_mock_intf;

static void build_intf(void)
{
    memset(&snd_mock_intf, 0, sizeof(snd_mock_intf));
    snd_mock_intf.Init = mk_Init;
    snd_mock_intf.Close = mk_Close;
    snd_mock_intf.Create = mk_Create;
    snd_mock_intf.Reload = mk_Reload;
    snd_mock_intf.Play = mk_Play;
    snd_mock_intf.Stop = mk_Stop;
    snd_mock_intf.Pause = mk_Pause;
    snd_mock_intf.Release = mk_Release;
    snd_mock_intf.Duplicate = mk_Duplicate;
    snd_mock_intf.SetCurrPos = mk_SetCurrPos;
    snd_mock_intf.Setvolume = mk_SetVolume;
    snd_mock_intf.SetFrequency = mk_SetFrequency;
    snd_mock_intf.SetPan = mk_SetPan;
    snd_mock_intf.GetStatus = mk_GetStatus;
    snd_mock_intf.GetFrequency = mk_GetFrequency;
    snd_mock_intf.GetPan = mk_GetPan;
    snd_mock_intf.GetCurrPos = mk_GetCurrPos;
    snd_mock_intf.CreateTrack = mk_CreateTrack;
    snd_mock_intf.ReleaseTrack = mk_ReleaseTrack;
    snd_mock_intf.StreamGetMonoToStereo = mk_StreamGetMonoToStereo;
    snd_mock_intf.StreamGetStereoToMono = (void(*)())mk_StreamGetStereoToMono;
    snd_mock_intf.StreamPrefetch = (void(*)())mk_StreamPrefetch;
    snd_mock_intf.StreamPrefetchArray = mk_StreamPrefetchArray;
    snd_mock_intf.StreamFlush = (void(*)())mk_StreamFlush;
    snd_mock_intf.StreamReinitAndPlay = mk_StreamReinitAndPlay;
    snd_mock_intf.ShutdownStreams = mk_ShutdownStreams;
    snd_mock_intf.nullptr1 = mk_nullptr1;
    snd_mock_intf.SetGlobalVol = mk_SetGlobalVol;
    snd_mock_intf.GetGlobalVol = mk_GetGlobalVol;
    snd_mock_intf.SetGlobalPan = mk_SetGlobalPan;
    snd_mock_intf.GetGlobalPan = mk_GetGlobalPan;
    snd_mock_intf.ComputeDoppler = mk_ComputeDoppler;
    snd_mock_intf.Compute3DVol = mk_Compute3DVol;
    snd_mock_intf.SNDNull = mk_SNDNull;
    snd_mock_intf.ChangeRenderMode = mk_ChangeRenderMode;
    snd_mock_intf.FxInit = mk_FxInit;
    snd_mock_intf.FxClose = mk_FxClose;
    snd_mock_intf.FxSetMode = mk_FxSetMode;
    snd_mock_intf.FxGetMode = mk_FxGetMode;
    snd_mock_intf.FxSetDelay = mk_FxSetDelay;
    snd_mock_intf.FxGetDelay = mk_FxGetDelay;
    snd_mock_intf.FxSetFeedback = mk_FxSetFeedback;
    snd_mock_intf.FxGetFeedback = mk_FxGetFeedback;
    snd_mock_intf.FxSetWetVolume = mk_FxSetWetVolume;
    snd_mock_intf.FxGetWetVolume = mk_FxGetWetVolume;
    snd_mock_intf.FxSetWetPan = mk_FxSetWetPan;
    snd_mock_intf.FxGetWetPan = mk_FxGetWetPan;
    snd_mock_intf.FxEnable = mk_FxEnable;
    snd_mock_intf.FxDisable = mk_FxDisable;
}

void snd_mock_reset(void)
{
    log_len = 0;
    log_buf[0] = 0;
    memset(heap, 0, heap_used);
    heap_used = 0;
    alloc_seq = 0;
    memset(bufs, 0, sizeof(bufs));
    memset(buf_state, 0, sizeof(buf_state));
    buf_used = 0;
    play_seq = 0;
    snd_global_vol = 0;
    snd_global_pan = 0;
    stream_pos = 0;
    memset(mdf_pool, 0, sizeof(mdf_pool));
    mdf_used = 0;
    {
        int i;
        for (i = 0; i < 7; ++i)
        {
            memset(&mdf_intf[i], 0, sizeof(mdf_intf[i]));
            mdf_intf[i].Type = i;
            mdf_intf[i].PlayApplyOnSound = mk_mdf_apply_sound;
            mdf_intf[i].PlayApplyOnInstance = mk_mdf_apply_inst;
            mdf_intf[i].Update = (i & 1) ? mk_mdf_update : 0;
            mdf_intf[i].NotifyPlay = (i & 2) ? mk_mdf_notify : 0;
            mdf_intf[i].NotifyPause = (i & 1) ? mk_mdf_notify : 0;
            mdf_intf[i].NotifyStop = mk_mdf_notify;
        }
    }
    ai_main_actor_slot = 0;
    AI_MainActor = &ai_main_actor_slot;
    snd_srand(12345);
    build_intf();
}
