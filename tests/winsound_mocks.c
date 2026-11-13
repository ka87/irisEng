/* See winsound_mocks.h. */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "winsound_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"

/* ---------------------------------------------------------------- log */

#define LOG_MAX (8 * 1024 * 1024)
static char log_buf[LOG_MAX];
static int log_len;

void wsnd_log(const char* fmt, ...)
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

const char* wsnd_log_text(void) { return log_buf; }
int wsnd_log_len(void) { return log_len; }

/* ---------------------------------------------------------- allocator */

/* Leaks on purpose and is rewound between the two sides, so both see the
   same addresses. Blocks get twice the room they ask for plus a slack:
   several of the engine's arrays are sized at four bytes a pointer, which
   is right in its own 32-bit build and half of what this harness needs --
   pcSND_AddSoundToThePlayList's 8 * (n + 200) is exactly that shape. */
#define HEAP_MAX (48 * 1024 * 1024)
#define GUARD 64
static unsigned char heap[HEAP_MAX];
static int heap_used;
static int heap_high;
static int alloc_seq;

typedef struct sBlockHdr { int size; int pad[3]; } tBlockHdr;

const unsigned char* wsnd_heap_base(void) { return heap; }
int wsnd_heap_used(void) { return heap_used; }

long wsnd_heap_off(const void* p)
{
    if (!p)
        return -1;
    if ((const unsigned char*)p < heap || (const unsigned char*)p >= heap + HEAP_MAX)
        return -2;
    return (long)((const unsigned char*)p - heap);
}

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
    if (heap_used > heap_high)
        heap_high = heap_used;
    return p + sizeof(tBlockHdr);
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = heap_alloc(size);
    wsnd_log("MEM_p_Alloc(%u)=#%d@%ld\n", size, ++alloc_seq, wsnd_heap_off(p));
    return p;
}

void* MEM_p_Realloc(void* old, size_t size)
{
    void* p;
    int oldSize;

    wsnd_log("MEM_p_Realloc(%ld,%u)\n", wsnd_heap_off(old), (unsigned)size);
    if (!old)
        return heap_alloc((unsigned)size);
    oldSize = ((tBlockHdr*)((unsigned char*)old - sizeof(tBlockHdr)))->size;
    p = heap_alloc((unsigned)size);
    {
        unsigned int carry = room_for((unsigned)oldSize);
        unsigned int cap = room_for((unsigned)size);
        memcpy(p, old, carry < cap ? carry : cap);
    }
    return p;
}

void MEM_Free(void* p) { wsnd_log("MEM_Free(%ld)\n", wsnd_heap_off(p)); }
void* MEM_p_AllocFromEnd(int size) { return heap_alloc((unsigned)size); }
void MEM_FreeFromEnd(void* p) { }
void* MEM_p_AllocTmp(int size) { return heap_alloc((unsigned)size); }
void* MEM_p_AllocAligned(int size, int alignment) { return heap_alloc((unsigned)size); }
void* MEM_p_ReallocAligned(void* p, unsigned int size, int alignment) { return MEM_p_Realloc(p, (size_t)size); }
void MEM_FreeAlign(void* p) { }

/* ------------------------------------------------ the mock COM objects */

/* Every mock interface is one of these. The vtable pointer is first so
   that a cast to any of the DirectSound interface types works, and the id
   is what the log prints instead of an address. */
typedef struct sMockObj
{
    void* lpVtbl;
    int id;
    int kind;
    int refs;
} tMockObj;

#define MOCK_MAX 256
static tMockObj mock_objs[MOCK_MAX];
static int mock_count;

int wsnd_obj_id(const void* p)
{
    const tMockObj* o = (const tMockObj*)p;
    if (!p)
        return -1;
    if (o >= mock_objs && o < mock_objs + MOCK_MAX)
        return o->id;
    return -2;
}

int wsnd_create_fail_mod;
int wsnd_lock_fail_mod;
int wsnd_status = 1;
int wsnd_play_pos;
int wsnd_lock_tail;
int wsnd_eax_support = 3;
int wsnd_qi_fail_mod;

static int create_calls;
static int lock_calls;
static int qi_calls;

static IDirectSoundBufferVtbl buffer_vtbl;
static IDirectSound3DBufferVtbl buffer3d_vtbl;
static IDirectSound3DListenerVtbl listener_vtbl;
static IDirectSoundNotifyVtbl notify_vtbl;
static IKsPropertySetVtbl propset_vtbl;
static IDirectSoundVtbl device_vtbl;

/* Lock hands out slices of this, so the bytes a side writes into a sound
   buffer are comparable between the two runs. */
#define AUDIO_MAX (2 * 1024 * 1024)
static unsigned char audio[AUDIO_MAX];
static int audio_used;

static void* mock_new(int kind, void* vtbl)
{
    tMockObj* o;
    if (mock_count >= MOCK_MAX)
    {
        fprintf(stderr, "mock object table exhausted\n");
        exit(2);
    }
    o = &mock_objs[mock_count];
    o->lpVtbl = vtbl;
    o->id = mock_count;
    o->kind = kind;
    o->refs = 1;
    ++mock_count;
    return o;
}

/* --- shared IUnknown --- */

static DWORD m_AddRef(void* self) { ((tMockObj*)self)->refs++; return 1; }

static DWORD m_Release(void* self)
{
    wsnd_log("Release(o%d)\n", wsnd_obj_id(self));
    return --((tMockObj*)self)->refs;
}

static HRESULT m_QueryInterface(void* self, const GUID* iid, void** out)
{
    int kind = 0;
    void* vtbl = 0;

    ++qi_calls;
    if (wsnd_qi_fail_mod && (qi_calls % wsnd_qi_fail_mod) == 0)
    {
        wsnd_log("QI(o%d)=FAIL\n", wsnd_obj_id(self));
        *out = 0;
        return (HRESULT)0x80004002L;
    }
    if (iid == &IID_IDirectSound3DBuffer) { kind = 2; vtbl = &buffer3d_vtbl; }
    else if (iid == &IID_IDirectSound3DListener) { kind = 3; vtbl = &listener_vtbl; }
    else if (iid == &IID_IDirectSoundNotify) { kind = 4; vtbl = &notify_vtbl; }
    else if (iid == &IID_IKsPropertySet) { kind = 5; vtbl = &propset_vtbl; }
    else { wsnd_log("QI(o%d)=UNKNOWN\n", wsnd_obj_id(self)); *out = 0; return (HRESULT)0x80004002L; }
    *out = mock_new(kind, vtbl);
    wsnd_log("QI(o%d,k%d)=o%d\n", wsnd_obj_id(self), kind, wsnd_obj_id(*out));
    return 0;
}

/* --- IDirectSoundBuffer --- */

static HRESULT b_GetCaps(IDirectSoundBuffer* s, DSBCAPS* c) { wsnd_log("GetCaps(o%d)\n", wsnd_obj_id(s)); return 0; }

static HRESULT b_GetCurrentPosition(IDirectSoundBuffer* s, DWORD* play, DWORD* write)
{
    wsnd_log("GetCurrentPosition(o%d)=%d\n", wsnd_obj_id(s), wsnd_play_pos);
    if (play) *(int*)play = wsnd_play_pos;
    if (write) *(int*)write = wsnd_play_pos + 64;
    return 0;
}

static HRESULT b_GetFormat(IDirectSoundBuffer* s, LPWAVEFORMATEX f, DWORD n, DWORD* w) { return 0; }

static HRESULT b_GetVolume(IDirectSoundBuffer* s, LONG* v)
{
    wsnd_log("GetVolume(o%d)=-1234\n", wsnd_obj_id(s));
    *(int*)v = -1234;
    return 0;
}

static HRESULT b_GetPan(IDirectSoundBuffer* s, LONG* v) { *(int*)v = 0; return 0; }

static HRESULT b_GetFrequency(IDirectSoundBuffer* s, DWORD* v)
{
    wsnd_log("GetFrequency(o%d)=22050\n", wsnd_obj_id(s));
    *(int*)v = 22050;
    return 0;
}

static HRESULT b_GetStatus(IDirectSoundBuffer* s, DWORD* v)
{
    wsnd_log("GetStatus(o%d)=%d\n", wsnd_obj_id(s), wsnd_status);
    *(int*)v = wsnd_status;
    return 0;
}

static HRESULT b_Initialize(IDirectSoundBuffer* s, void* d, const DSBUFFERDESC* x) { return 0; }

static HRESULT b_Lock(IDirectSoundBuffer* s, DWORD off, DWORD bytes,
                      void** p1, DWORD* n1, void** p2, DWORD* n2, DWORD flags)
{
    ++lock_calls;
    if (wsnd_lock_fail_mod && (lock_calls % wsnd_lock_fail_mod) == 0)
    {
        wsnd_log("Lock(o%d,%u,%u,f%u)=FAIL\n", wsnd_obj_id(s), off, bytes, flags);
        return (HRESULT)0x88780096L;
    }
    if (bytes > 65536) bytes = 65536;
    if (audio_used + (int)bytes + wsnd_lock_tail + 64 > AUDIO_MAX)
        audio_used = 0;
    *p1 = audio + audio_used;
    *(int*)n1 = (int)bytes;
    audio_used += (int)bytes + 16;
    /* The engine passes NULL for the second pointer pair at several
       sites, which the real Lock accepts. */
    if (wsnd_lock_tail && p2)
    {
        *p2 = audio + audio_used;
        if (n2) *(int*)n2 = wsnd_lock_tail;
        audio_used += wsnd_lock_tail + 16;
    }
    else
    {
        if (p2) *p2 = 0;
        if (n2) *(int*)n2 = 0;
    }
    wsnd_log("Lock(o%d,%u,%u,f%u)=p1@%d,n1=%u,p2@%d,n2=%d\n",
             wsnd_obj_id(s), off, bytes, flags,
             (int)((unsigned char*)*p1 - audio), bytes,
             (p2 && *p2) ? (int)((unsigned char*)*p2 - audio) : -1,
             (p2 && *p2) ? wsnd_lock_tail : 0);
    return 0;
}

/* A fingerprint of a byte range, so that what a side wrote into a locked
   region is compared without printing kilobytes of it. */
static unsigned fingerprint(const unsigned char* p, int n)
{
    unsigned h = 2166136261u;
    int i;
    for (i = 0; i < n; ++i)
    {
        h ^= p[i];
        h *= 16777619u;
    }
    return h;
}

static HRESULT b_Play(IDirectSoundBuffer* s, DWORD r1, DWORD r2, DWORD flags)
{
    wsnd_log("Play(o%d,%u)\n", wsnd_obj_id(s), flags);
    wsnd_status |= 1;
    return 0;
}

static HRESULT b_SetCurrentPosition(IDirectSoundBuffer* s, DWORD p) { wsnd_log("SetCurrentPosition(o%d,%u)\n", wsnd_obj_id(s), p); return 0; }
static HRESULT b_SetFormat(IDirectSoundBuffer* s, const WAVEFORMATEX* f)
{
    wsnd_log("SetFormat(o%d,tag=%u,ch=%u,rate=%u,bps=%u,align=%u,avg=%u,cb=%u)\n",
             wsnd_obj_id(s), f->wFormatTag, f->nChannels, f->nSamplesPerSec,
             f->wBitsPerSample, f->nBlockAlign, f->nAvgBytesPerSec, f->cbSize);
    return 0;
}
static HRESULT b_SetVolume(IDirectSoundBuffer* s, LONG v) { wsnd_log("SetVolume(o%d,%ld)\n", wsnd_obj_id(s), (long)v); return 0; }
static HRESULT b_SetPan(IDirectSoundBuffer* s, LONG v) { wsnd_log("SetPan(o%d,%ld)\n", wsnd_obj_id(s), (long)v); return 0; }
static HRESULT b_SetFrequency(IDirectSoundBuffer* s, DWORD v) { wsnd_log("SetFrequency(o%d,%u)\n", wsnd_obj_id(s), v); return 0; }
static HRESULT b_Stop(IDirectSoundBuffer* s)
{
    wsnd_log("Stop(o%d)\n", wsnd_obj_id(s));
    /* The device really does stop: pcSND_flipPlayStopState spins on
       GetStatus until the playing bit clears. */
    wsnd_status &= ~1;
    return 0;
}

static HRESULT b_Unlock(IDirectSoundBuffer* s, void* p1, DWORD n1, void* p2, DWORD n2)
{
    wsnd_log("Unlock(o%d,p1@%d,n1=%u,p2@%d,n2=%u,fp=%08X)\n",
             wsnd_obj_id(s), p1 ? (int)((unsigned char*)p1 - audio) : -1, n1,
             p2 ? (int)((unsigned char*)p2 - audio) : -1, n2,
             p1 ? fingerprint((unsigned char*)p1, (int)(n1 > 4096 ? 4096 : n1)) : 0);
    return 0;
}

static HRESULT b_Restore(IDirectSoundBuffer* s) { return 0; }

/* --- IDirectSound3DBuffer --- */

static HRESULT d3_GetAllParameters(IDirectSound3DBuffer* s, DS3DBUFFER* b) { return 0; }
static HRESULT d3_SetMaxDistance(IDirectSound3DBuffer* s, float v, DWORD a) { wsnd_log("SetMaxDistance(o%d,%.9g)\n", wsnd_obj_id(s), (double)v); return 0; }
static HRESULT d3_SetMinDistance(IDirectSound3DBuffer* s, float v, DWORD a) { wsnd_log("SetMinDistance(o%d,%.9g)\n", wsnd_obj_id(s), (double)v); return 0; }
static HRESULT d3_SetMode(IDirectSound3DBuffer* s, DWORD m, DWORD a) { return 0; }
static HRESULT d3_SetPosition(IDirectSound3DBuffer* s, float x, float y, float z, DWORD a)
{
    wsnd_log("3DSetPosition(o%d,%.9g,%.9g,%.9g)\n", wsnd_obj_id(s), (double)x, (double)y, (double)z);
    return 0;
}
static HRESULT d3_SetVelocity(IDirectSound3DBuffer* s, float x, float y, float z, DWORD a) { return 0; }

/* --- IDirectSound3DListener --- */

static HRESULT l_GetAllParameters(IDirectSound3DListener* s, DS3DLISTENER* d) { return 0; }
static HRESULT l_SetAllParameters(IDirectSound3DListener* s, const DS3DLISTENER* d, DWORD a) { return 0; }
static HRESULT l_SetOrientation(IDirectSound3DListener* s, float a, float b, float c, float d, float e, float f, DWORD g) { return 0; }
static HRESULT l_SetPosition(IDirectSound3DListener* s, float x, float y, float z, DWORD a) { return 0; }
static HRESULT l_SetVelocity(IDirectSound3DListener* s, float x, float y, float z, DWORD a) { return 0; }
static HRESULT l_Commit(IDirectSound3DListener* s) { return 0; }

/* --- IDirectSoundNotify --- */

static HRESULT n_SetNotificationPositions(IDirectSoundNotify* s, DWORD n, LPDSBPOSITIONNOTIFY p)
{
    DWORD i;
    wsnd_log("SetNotificationPositions(o%d,%u)\n", wsnd_obj_id(s), n);
    for (i = 0; i < n; ++i)
        wsnd_log("  notify[%u] off=%u ev=%ld\n", i, p[i].dwOffset, (long)(size_t)p[i].hEventNotify);
    return 0;
}

/* --- IKsPropertySet --- */

static HRESULT ks_Get(IKsPropertySet* s, const GUID* g, DWORD id, void* a, DWORD an, void* b, DWORD bn, DWORD* r) { return 0; }

static unsigned fb(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

static HRESULT ks_Set(IKsPropertySet* s, const GUID* g, DWORD id, void* a, DWORD an, void* b, DWORD bn)
{
    /* The listener block is printed field by field rather than
       fingerprinted: EAXLISTENERPROPERTIES has no padding in the engine's
       own 32-bit build but does here, and the holes a memcpy carries
       across are stack residue rather than anything the code decided. */
    if (g == &DSPROPSETID_EAX30_ListenerProperties && bn >= sizeof(EAXLISTENERPROPERTIES))
    {
        const EAXLISTENERPROPERTIES* p = (const EAXLISTENERPROPERTIES*)b;
        wsnd_log("KsSet(o%d,listener,%u) env=%lu size=%08X dif=%08X room=%ld hf=%ld lf=%ld\n",
                 wsnd_obj_id(s), id, p->ulEnvironment, fb(p->flEnvironmentSize),
                 fb(p->flEnvironmentDiffusion), p->lRoom, p->lRoomHF, p->lRoomLF);
        wsnd_log("  dt=%08X dhf=%08X dlf=%08X refl=%ld rd=%08X rev=%ld vd=%08X\n",
                 fb(p->flDecayTime), fb(p->flDecayHFRatio), fb(p->flDecayLFRatio),
                 p->lReflections, fb(p->flReflectionsDelay), p->lReverb, fb(p->flReverbDelay));
        wsnd_log("  et=%08X ed=%08X mt=%08X md=%08X ah=%08X hr=%08X lr=%08X rr=%08X fl=%lu\n",
                 fb(p->flEchoTime), fb(p->flEchoDepth), fb(p->flModulationTime),
                 fb(p->flModulationDepth), fb(p->flAirAbsorptionHF), fb(p->flHFReference),
                 fb(p->flLFReference), fb(p->flRoomRolloffFactor), p->ulFlags);
        wsnd_log("  pans %08X %08X %08X %08X %08X %08X\n",
                 fb(p->vReverbPan.x), fb(p->vReverbPan.y), fb(p->vReverbPan.z),
                 fb(p->vReflectionsPan.x), fb(p->vReflectionsPan.y), fb(p->vReflectionsPan.z));
        return 0;
    }
    wsnd_log("KsSet(o%d,%s,%u,%u,v=%d)\n", wsnd_obj_id(s),
             g == &DSPROPSETID_EAX30_BufferProperties ? "buffer" : "?",
             id, bn, bn == 4 ? *(const int*)b : 0);
    return 0;
}

static HRESULT ks_QuerySupport(IKsPropertySet* s, const GUID* g, DWORD id, DWORD* support)
{
    wsnd_log("KsQuerySupport(o%d,%u)=%d\n", wsnd_obj_id(s), id, wsnd_eax_support);
    if (wsnd_eax_support < 0)
        return (HRESULT)0x80004005L;
    *(int*)support = wsnd_eax_support;
    return 0;
}

/* --- IDirectSound --- */

static HRESULT ds_CreateSoundBuffer(IDirectSound* s, const DSBUFFERDESC* d, IDirectSoundBuffer** out, IUnknown* unk)
{
    ++create_calls;
    wsnd_log("CreateSoundBuffer(size=%u,flags=%X,bytes=%u,fmt=%s",
             d->dwSize, d->dwFlags, d->dwBufferBytes, d->lpwfxFormat ? "y" : "n");
    if (d->lpwfxFormat)
        wsnd_log(",tag=%u,ch=%u,rate=%u,bps=%u,align=%u,avg=%u,cb=%u",
                 d->lpwfxFormat->wFormatTag, d->lpwfxFormat->nChannels,
                 d->lpwfxFormat->nSamplesPerSec, d->lpwfxFormat->wBitsPerSample,
                 d->lpwfxFormat->nBlockAlign, d->lpwfxFormat->nAvgBytesPerSec,
                 d->lpwfxFormat->cbSize);
    if (wsnd_create_fail_mod && (create_calls % wsnd_create_fail_mod) == 0)
    {
        wsnd_log(")=FAIL\n");
        *out = 0;
        return (HRESULT)0x88780032L;
    }
    *out = (IDirectSoundBuffer*)mock_new(1, &buffer_vtbl);
    wsnd_log(")=o%d\n", wsnd_obj_id(*out));
    return 0;
}

static HRESULT ds_GetCaps(IDirectSound* s, DSCAPS* c)
{
    wsnd_log("dsGetCaps(size=%u)\n", c->dwSize);
    c->dwFlags = 0x55;
    c->dwMaxHwMixingAllBuffers = 32;
    return 0;
}

static HRESULT ds_DuplicateSoundBuffer(IDirectSound* s, IDirectSoundBuffer* src, IDirectSoundBuffer** out)
{
    *out = (IDirectSoundBuffer*)mock_new(1, &buffer_vtbl);
    wsnd_log("DuplicateSoundBuffer(o%d)=o%d\n", wsnd_obj_id(src), wsnd_obj_id(*out));
    return 0;
}

static HRESULT ds_SetCooperativeLevel(IDirectSound* s, HWND w, DWORD level)
{
    wsnd_log("SetCooperativeLevel(%ld,%u)\n", (long)(size_t)w, level);
    return 0;
}

static HRESULT ds_Compact(IDirectSound* s) { return 0; }
static HRESULT ds_GetSpeakerConfig(IDirectSound* s, DWORD* c) { *c = 0; return 0; }
static HRESULT ds_SetSpeakerConfig(IDirectSound* s, DWORD c) { return 0; }
static HRESULT ds_Initialize(IDirectSound* s, const GUID* g) { return 0; }

static tMockObj* the_device;

static void build_vtbls(void)
{
    buffer_vtbl.QueryInterface = (HRESULT (*)(IDirectSoundBuffer*, const GUID*, void**))m_QueryInterface;
    buffer_vtbl.AddRef = (DWORD (*)(IDirectSoundBuffer*))m_AddRef;
    buffer_vtbl.Release = (DWORD (*)(IDirectSoundBuffer*))m_Release;
    buffer_vtbl.GetCaps = b_GetCaps;
    buffer_vtbl.GetCurrentPosition = b_GetCurrentPosition;
    buffer_vtbl.GetFormat = b_GetFormat;
    buffer_vtbl.GetVolume = b_GetVolume;
    buffer_vtbl.GetPan = b_GetPan;
    buffer_vtbl.GetFrequency = b_GetFrequency;
    buffer_vtbl.GetStatus = b_GetStatus;
    buffer_vtbl.Initialize = b_Initialize;
    buffer_vtbl.Lock = b_Lock;
    buffer_vtbl.Play = b_Play;
    buffer_vtbl.SetCurrentPosition = b_SetCurrentPosition;
    buffer_vtbl.SetFormat = b_SetFormat;
    buffer_vtbl.SetVolume = b_SetVolume;
    buffer_vtbl.SetPan = b_SetPan;
    buffer_vtbl.SetFrequency = b_SetFrequency;
    buffer_vtbl.Stop = b_Stop;
    buffer_vtbl.Unlock = b_Unlock;
    buffer_vtbl.Restore = b_Restore;

    buffer3d_vtbl.QueryInterface = (HRESULT (*)(IDirectSound3DBuffer*, const GUID*, void**))m_QueryInterface;
    buffer3d_vtbl.AddRef = (DWORD (*)(IDirectSound3DBuffer*))m_AddRef;
    buffer3d_vtbl.Release = (DWORD (*)(IDirectSound3DBuffer*))m_Release;
    buffer3d_vtbl.GetAllParameters = d3_GetAllParameters;
    buffer3d_vtbl.SetMaxDistance = d3_SetMaxDistance;
    buffer3d_vtbl.SetMinDistance = d3_SetMinDistance;
    buffer3d_vtbl.SetMode = d3_SetMode;
    buffer3d_vtbl.SetPosition = d3_SetPosition;
    buffer3d_vtbl.SetVelocity = d3_SetVelocity;

    listener_vtbl.QueryInterface = (HRESULT (*)(IDirectSound3DListener*, const GUID*, void**))m_QueryInterface;
    listener_vtbl.AddRef = (DWORD (*)(IDirectSound3DListener*))m_AddRef;
    listener_vtbl.Release = (DWORD (*)(IDirectSound3DListener*))m_Release;
    listener_vtbl.GetAllParameters = l_GetAllParameters;
    listener_vtbl.SetAllParameters = l_SetAllParameters;
    listener_vtbl.SetOrientation = l_SetOrientation;
    listener_vtbl.SetPosition = l_SetPosition;
    listener_vtbl.SetVelocity = l_SetVelocity;
    listener_vtbl.CommitDeferredSettings = l_Commit;

    notify_vtbl.QueryInterface = (HRESULT (*)(IDirectSoundNotify*, const GUID*, void**))m_QueryInterface;
    notify_vtbl.AddRef = (DWORD (*)(IDirectSoundNotify*))m_AddRef;
    notify_vtbl.Release = (DWORD (*)(IDirectSoundNotify*))m_Release;
    notify_vtbl.SetNotificationPositions = n_SetNotificationPositions;

    propset_vtbl.QueryInterface = (HRESULT (*)(IKsPropertySet*, const GUID*, void**))m_QueryInterface;
    propset_vtbl.AddRef = (DWORD (*)(IKsPropertySet*))m_AddRef;
    propset_vtbl.Release = (DWORD (*)(IKsPropertySet*))m_Release;
    propset_vtbl.Get = ks_Get;
    propset_vtbl.Set = ks_Set;
    propset_vtbl.QuerySupport = ks_QuerySupport;

    device_vtbl.QueryInterface = (HRESULT (*)(IDirectSound*, const GUID*, void**))m_QueryInterface;
    device_vtbl.AddRef = (DWORD (*)(IDirectSound*))m_AddRef;
    device_vtbl.Release = (DWORD (*)(IDirectSound*))m_Release;
    device_vtbl.CreateSoundBuffer = ds_CreateSoundBuffer;
    device_vtbl.GetCaps = ds_GetCaps;
    device_vtbl.DuplicateSoundBuffer = ds_DuplicateSoundBuffer;
    device_vtbl.SetCooperativeLevel = ds_SetCooperativeLevel;
    device_vtbl.Compact = ds_Compact;
    device_vtbl.GetSpeakerConfig = ds_GetSpeakerConfig;
    device_vtbl.SetSpeakerConfig = ds_SetSpeakerConfig;
    device_vtbl.Initialize = ds_Initialize;
}

IDirectSound8* wsnd_device(void) { return (IDirectSound8*)the_device; }

/* ---- the GUIDs the engine names ------------------------------------- */

const GUID IID_IDirectSound3DBuffer = { 1, 0, 0, { 0 } };
const GUID IID_IDirectSound3DListener = { 2, 0, 0, { 0 } };
const GUID IID_IDirectSoundNotify = { 3, 0, 0, { 0 } };
const GUID IID_IKsPropertySet = { 4, 0, 0, { 0 } };
const GUID DSPROPSETID_EAX30_ListenerProperties = { 5, 0, 0, { 0 } };
const GUID DSPROPSETID_EAX30_BufferProperties = { 6, 0, 0, { 0 } };
const GUID IID_IDirectSoundBuffer8 = { 7, 0, 0, { 0 } };

/* ------------------------------------------------------------- bigfile */

BIGGSTStruct BIG_gst;

int wsnd_read_bytes = 4096;
int wsnd_loa_speedmode;

/* ADPCM source bytes. pcSND_ADPCMToPCM_ConvertOneBlock indexes its
   seven-entry coefficient table with the block's predictor byte and does
   not range-check it, so a random byte there reads off the end of the
   caller's frame and the two sides decode different samples from the same
   input. Every block header byte is therefore kept inside 0..6; the rest
   is an arbitrary but fixed pattern. */
static unsigned char adpcm_byte(int i, int salt)
{
    if ((i % 36) < 2)
        return (unsigned char)((i / 36 + salt) % 7);
    return (unsigned char)(i * 13 + (i >> 5) + salt);
}

void BIG_ReadNoSeek(int pos, void* dest, unsigned int len)
{
    unsigned int i;
    unsigned char* p = (unsigned char*)dest;
    for (i = 0; i < len; ++i)
        p[i] = adpcm_byte((int)i, pos * 7);
    wsnd_log("BIG_ReadNoSeek(%d,%u)fp=%08X\n", pos, len, fingerprint(p, (int)(len > 2048 ? 2048 : len)));
}

/* ------------------------------------------------------------- loader */

static char loa_buffer[256 * 1024];

char* LOA_FetchBuffer(int size)
{
    wsnd_log("LOA_FetchBuffer(%d)\n", size);
    return loa_buffer;
}

char** LOA_ReadCharArray(char** offset, char* dest, unsigned int len, char readMode)
{
    unsigned int i;
    for (i = 0; i < len; ++i)
        dest[i] = (char)adpcm_byte((int)i, (int)len);
    wsnd_log("LOA_ReadCharArray(%u,%d)fp=%08X\n", len, readMode,
             fingerprint((unsigned char*)dest, (int)(len > 2048 ? 2048 : len)));
    *offset += len;
    return offset;
}

BOOL isSpeedModeAndBinFile(void)
{
    wsnd_log("isSpeedModeAndBinFile()=%d\n", wsnd_loa_speedmode);
    return wsnd_loa_speedmode;
}

/* ------------------------------------------------------------- Win32 */

int wsnd_reg_fail;
int wsnd_reg_fx_value = 2;

static int handle_seq;

LONG RegOpenKeyExA(HKEY root, LPCSTR path, DWORD opts, DWORD access, PHKEY out)
{
    wsnd_log("RegOpenKeyExA(%s,%u)\n", path, access);
    *out = (HKEY)(size_t)0x1000;
    return 0;
}

LONG RegQueryValueExA(HKEY key, LPCSTR name, DWORD* res, DWORD* type, LPBYTE data, DWORD* size)
{
    int isFx = name[0] == 'S';
    int fail = isFx ? (wsnd_reg_fail & 1) : (wsnd_reg_fail & 2);

    wsnd_log("RegQueryValueExA(%s,%u)=%d\n", name, *size, fail ? 1 : 0);
    if (fail)
        return 1;
    if (isFx)
    {
        *type = 4;
        *size = 4;
        memcpy(data, &wsnd_reg_fx_value, 4);
    }
    else
    {
        int i;
        *type = 3;
        *size = 16;
        for (i = 0; i < 16; ++i)
            data[i] = (unsigned char)(0x40 + i);
    }
    return 0;
}

LONG RegCloseKey(HKEY key) { wsnd_log("RegCloseKey\n"); return 0; }

DWORD GetLastError(void) { return 5; }

DWORD FormatMessageA(DWORD flags, const void* src, DWORD id, DWORD lang, LPSTR buf, DWORD size, va_list* args)
{
    wsnd_log("FormatMessageA(%u)\n", id);
    if (size) buf[0] = 0;
    return 0;
}

HMODULE LoadLibraryA(LPCSTR name) { wsnd_log("LoadLibraryA(%s)\n", name); return (HMODULE)(size_t)0x2000; }

static HRESULT WINAPI mock_eax_create(GUID* g, LPDIRECTSOUND8* out, IUnknown* outer)
{
    wsnd_log("EAXDirectSoundCreate8(guid0=%02X)\n", ((unsigned char*)g)[0]);
    *out = (IDirectSound8*)the_device;
    return 0;
}

FARPROC GetProcAddress(HMODULE m, LPCSTR name)
{
    wsnd_log("GetProcAddress(%s)\n", name);
    return (FARPROC)mock_eax_create;
}

HANDLE CreateEventA(void* attrs, BOOL manual, BOOL initial, LPCSTR name)
{
    return (HANDLE)(size_t)(++handle_seq);
}

BOOL SetEvent(HANDLE h) { wsnd_log("SetEvent(%ld)\n", (long)(size_t)h); return 1; }
BOOL ResetEvent(HANDLE h) { wsnd_log("ResetEvent(%ld)\n", (long)(size_t)h); return 1; }
BOOL CloseHandle(HANDLE h) { wsnd_log("CloseHandle(%ld)\n", (long)(size_t)h); return 1; }
DWORD WaitForSingleObject(HANDLE h, DWORD ms) { wsnd_log("WaitForSingleObject(%ld,%u)\n", (long)(size_t)h, ms); return 0; }

HANDLE CreateThread(void* attrs, SIZE_T stack, LPTHREAD_START_ROUTINE start, LPVOID param, DWORD flags, LPDWORD id)
{
    wsnd_log("CreateThread\n");
    *(int*)id = 4242;
    return (HANDLE)(size_t)0x3000;
}

HANDLE CreateFileA(LPCSTR path, DWORD access, DWORD share, void* sec, DWORD disp, DWORD attrs, HANDLE templ)
{
    wsnd_log("CreateFileA(%s,%X)\n", path, access);
    return (HANDLE)(size_t)(0x5000 + (++handle_seq));
}

BOOL ReadFile(HANDLE h, LPVOID buf, DWORD count, LPDWORD read, void* ov)
{
    DWORD n = (DWORD)wsnd_read_bytes;
    DWORD i;
    unsigned char* p = (unsigned char*)buf;

    if (n > count) n = count;
    for (i = 0; i < n; ++i)
        p[i] = adpcm_byte((int)i, 3);
    *(int*)read = (int)n;
    wsnd_log("ReadFile(%ld,%u)=%u\n", (long)(size_t)h, count, n);
    return 1;
}

DWORD SetFilePointer(HANDLE h, LONG move, LONG* high, DWORD method)
{
    wsnd_log("SetFilePointer(%ld,%ld,%u)\n", (long)(size_t)h, (long)move, method);
    return (DWORD)move;
}

void OutputDebugStringA(LPCSTR s) { wsnd_log("OutputDebugStringA(%s)\n", s); }
HRESULT CoInitialize(LPVOID r) { wsnd_log("CoInitialize\n"); return 0; }
void CoUninitialize(void) { wsnd_log("CoUninitialize\n"); }

/* ---- the scripted sound thread -------------------------------------- */

static int thread_script[64];
static int thread_script_len;
static int thread_step;
static jmp_buf thread_exit;
static int thread_running;

void wsnd_thread_script(const int* indices, int count)
{
    int i;
    if (count > 64) count = 64;
    for (i = 0; i < count; ++i)
        thread_script[i] = indices[i];
    thread_script_len = count;
    thread_step = 0;
}

DWORD WaitForMultipleObjects(DWORD count, const HANDLE* handles, BOOL all, DWORD ms)
{
    DWORD r;
    if (thread_step < thread_script_len)
        r = (DWORD)thread_script[thread_step];
    else
        r = 2;                   /* the thread's own exit case */
    ++thread_step;
    wsnd_log("WaitForMultipleObjects(n=%u)=%u\n", count, r);
    return r;
}

void ExitThread(DWORD code)
{
    wsnd_log("ExitThread(%u)\n", code);
    if (thread_running)
        longjmp(thread_exit, 1);
    exit(3);
}

void wsnd_run_thread(int (*body)(void*))
{
    thread_running = 1;
    if (!setjmp(thread_exit))
        body(0);
    thread_running = 0;
}

/* ------------------------------------------------------------- reset */

void wsnd_mock_reset(void)
{
    log_len = 0;
    if (heap_used > heap_high)
        heap_high = heap_used;
    heap_used = 0;
    alloc_seq = 0;
    mock_count = 0;
    create_calls = 0;
    lock_calls = 0;
    qi_calls = 0;
    audio_used = 0;
    handle_seq = 0;
    thread_step = 0;
    thread_script_len = 0;
    thread_running = 0;
    wsnd_create_fail_mod = 0;
    wsnd_lock_fail_mod = 0;
    wsnd_qi_fail_mod = 0;
    wsnd_status = 1;
    wsnd_play_pos = 0;
    wsnd_lock_tail = 0;
    wsnd_eax_support = 3;
    wsnd_reg_fail = 0;
    wsnd_reg_fx_value = 2;
    wsnd_read_bytes = 4096;
    wsnd_loa_speedmode = 0;
    /* Clear everything a previous case touched, so that a block a side
       writes past -- the engine sizes several pointer arrays at four
       bytes a pointer -- cannot read a previous case's bytes back. */
    if (heap_high)
        memset(heap, 0, (size_t)heap_high);
    heap_high = 0;
    memset(audio, 0, sizeof(audio));
    memset(mock_objs, 0, sizeof(mock_objs));
    build_vtbls();
    the_device = (tMockObj*)mock_new(0, &device_vtbl);
    strcpy(BIG_gst.filePath, "mock.bf");
}
