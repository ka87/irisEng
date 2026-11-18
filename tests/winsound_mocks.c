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

/* The device's own ceremony -- opening a named driver, reading the
   registry for its GUID and the effects setting, loading the EAX
   export, asking for capabilities, setting a cooperative level -- is
   silent for the same reason the rest of it is: the rewrite does none
   of it, so there is nothing on the other side to line it up with. */

/* ----------------------------------------------- the shared vocabulary */

/* The frozen copy reaches a sound through DirectSound and the rewrite
   through the platform layer, so the two cannot be compared on which
   calls they made. What they can be compared on is the events those
   calls stand for, and these are what both sides log for them: one
   line an event, in the same words, whichever interface delivered it.
   Everything that is one interface's ceremony and not an event --
   querying an interface, setting a data format or a cooperative
   level, arming notification positions, probing EAX -- is silent,
   because there is nothing on the other side to line it up with.
   
   The getters are silent too, for a different reason: what the engine
   does with what they return lands in state, and state is compared in
   full. */

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

static void wsnd_ev_voice(int id, unsigned ch, unsigned rate, unsigned bytes)
{
    wsnd_log("voice(o%d,ch=%u,rate=%u,bytes=%u)\n", id, ch, rate, bytes);
}
static void wsnd_ev_drop(int id) { wsnd_log("drop(o%d)\n", id); }
static void wsnd_ev_play(int id, int loop) { wsnd_log("play(o%d,%d)\n", id, loop); }
static void wsnd_ev_stop(int id) { wsnd_log("stop(o%d)\n", id); }
static void wsnd_ev_att(int id, long v) { wsnd_log("att(o%d,%ld)\n", id, v); }
static void wsnd_ev_pan(int id, long v) { wsnd_log("pan(o%d,%ld)\n", id, v); }
static void wsnd_ev_freq(int id, unsigned v) { wsnd_log("freq(o%d,%u)\n", id, v); }
static void wsnd_ev_cursor(int id, unsigned v) { wsnd_log("cursor(o%d,%u)\n", id, v); }
static void wsnd_ev_write(int id, unsigned off, unsigned bytes, unsigned fp)
{
    wsnd_log("wr(o%d,%u,%u,fp=%08X)\n", id, off, bytes, fp);
}

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
    /* Which object this one was queried off, for the 3D buffer: what
       it does belongs to the buffer it hangs from, and that is the
       object the rewrite's log names. */
    int parent;
} tMockObj;

#define MOCK_MAX 256
static tMockObj mock_objs[MOCK_MAX];
static int mock_count;

/* Set while the module's own init runs. The objects it makes are not
   numbered: the frozen copy's init makes four of them and the
   rewrite's makes none, and numbering them would put every object
   made afterwards four apart on the two sides. They print negative
   instead, which says they are the init's, and the numbering carries
   on from where it was. */
int wsnd_init_quiet;
static int quiet_count;
static int id_next;

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
#define WSND_LOCK_MAX (1024 * 1024)
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
    o->id = wsnd_init_quiet ? -1 - quiet_count++ : id_next++;
    o->kind = kind;
    o->refs = 1;
    o->parent = -1;
    ++mock_count;
    return o;
}

/* --- shared IUnknown --- */

static DWORD m_AddRef(void* self) { ((tMockObj*)self)->refs++; return 1; }

static DWORD m_Release(void* self)
{
    /* Only a sound buffer going away is an event. The rest -- the
       device, the listener, the 3D buffer, the property set, the
       notify interface -- are this interface's own objects and the
       rewrite has none of them. */
    if (((tMockObj*)self)->kind == 1)
        wsnd_ev_drop(wsnd_obj_id(self));
    return --((tMockObj*)self)->refs;
}

static HRESULT m_QueryInterface(void* self, const GUID* iid, void** out)
{
    int kind = 0;
    void* vtbl = 0;

    ++qi_calls;
    if (wsnd_qi_fail_mod && (qi_calls % wsnd_qi_fail_mod) == 0)
    {

        *out = 0;
        return (HRESULT)0x80004002L;
    }
    if (iid == &IID_IDirectSound3DBuffer) { kind = 2; vtbl = &buffer3d_vtbl; }
    else if (iid == &IID_IDirectSound3DListener) { kind = 3; vtbl = &listener_vtbl; }
    else if (iid == &IID_IDirectSoundNotify) { kind = 4; vtbl = &notify_vtbl; }
    else if (iid == &IID_IKsPropertySet) { kind = 5; vtbl = &propset_vtbl; }
    else { *out = 0; return (HRESULT)0x80004002L; }
    /* Unnumbered, all four of them. Querying an interface off a
       buffer is this interface's own way of reaching a feature -- 3D
       position, a listener, refill notifications, the reverb property
       set -- and the rewrite queries nothing, so none of these has a
       counterpart. Numbering them would put every object made after
       one of them out of step on the two sides, which is what the
       notify interface queried per track was doing. The parent is
       still recorded, because a 3D buffer speaks for its buffer. */
    {
        int was = wsnd_init_quiet;
        wsnd_init_quiet = 1;
        *out = mock_new(kind, vtbl);
        wsnd_init_quiet = was;
    }
    ((tMockObj*)*out)->parent = wsnd_obj_id(self);
    return 0;
}

/* --- IDirectSoundBuffer --- */

static HRESULT b_GetCaps(IDirectSoundBuffer* s, DSBCAPS* c) { return 0; }

static HRESULT b_GetCurrentPosition(IDirectSoundBuffer* s, DWORD* play, DWORD* write)
{

    if (play) *(int*)play = wsnd_play_pos;
    if (write) *(int*)write = wsnd_play_pos + 64;
    return 0;
}

static HRESULT b_GetFormat(IDirectSoundBuffer* s, LPWAVEFORMATEX f, DWORD n, DWORD* w) { return 0; }

static HRESULT b_GetVolume(IDirectSoundBuffer* s, LONG* v)
{

    *(int*)v = -1234;
    return 0;
}

static HRESULT b_GetPan(IDirectSoundBuffer* s, LONG* v) { *(int*)v = 0; return 0; }

static HRESULT b_GetFrequency(IDirectSoundBuffer* s, DWORD* v)
{

    *(int*)v = 22050;
    return 0;
}

static HRESULT b_GetStatus(IDirectSoundBuffer* s, DWORD* v)
{

    *(int*)v = wsnd_status;
    return 0;
}

static HRESULT b_Initialize(IDirectSoundBuffer* s, void* d, const DSBUFFERDESC* x) { return 0; }

/* The offset the last lock was taken at, so that unlocking can say
   where the bytes went. The engine locks and unlocks in one breath at
   every site, so one is enough. */
static unsigned lock_off;
static unsigned lock_bytes;

static HRESULT b_Lock(IDirectSoundBuffer* s, DWORD off, DWORD bytes,
                      void** p1, DWORD* n1, void** p2, DWORD* n2, DWORD flags)
{
    ++lock_calls;
    lock_off = off;
    /* A megabyte, where this was 64 KB. A real streamed sound locks
       its whole ring at the prefetch and that is 307200 bytes, so the
       old cap meant the frozen copy could not be asked for one either
       -- and a bug in the rewrite that stopped at exactly 64 KB had
       nothing to disagree with. AUDIO_MAX is two megabytes and one
       lock is live at a time. */
    lock_bytes = bytes > WSND_LOCK_MAX ? WSND_LOCK_MAX : bytes;
    if (wsnd_lock_fail_mod && (lock_calls % wsnd_lock_fail_mod) == 0)
    {
        wsnd_log("wr(o%d,%u,%u,FAIL)\n", wsnd_obj_id(s), off, bytes);
        return (HRESULT)0x88780096L;
    }
    if (bytes > WSND_LOCK_MAX) bytes = WSND_LOCK_MAX;
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
    /* Silent: a lock is half an event. The write is logged when it
       is unlocked, which is when the bytes exist. */
    return 0;
}


static HRESULT b_Play(IDirectSoundBuffer* s, DWORD r1, DWORD r2, DWORD flags)
{
    wsnd_ev_play(wsnd_obj_id(s), (int)(flags & 1));
    wsnd_status |= 1;
    return 0;
}

static HRESULT b_SetCurrentPosition(IDirectSoundBuffer* s, DWORD p) { wsnd_ev_cursor(wsnd_obj_id(s), p); return 0; }
static HRESULT b_SetFormat(IDirectSoundBuffer* s, const WAVEFORMATEX* f)
{
    return 0;
}
static HRESULT b_SetVolume(IDirectSoundBuffer* s, LONG v) { wsnd_ev_att(wsnd_obj_id(s), (long)v); return 0; }
static HRESULT b_SetPan(IDirectSoundBuffer* s, LONG v) { wsnd_ev_pan(wsnd_obj_id(s), (long)v); return 0; }
static HRESULT b_SetFrequency(IDirectSoundBuffer* s, DWORD v) { wsnd_ev_freq(wsnd_obj_id(s), v); return 0; }
static HRESULT b_Stop(IDirectSoundBuffer* s)
{
    wsnd_ev_stop(wsnd_obj_id(s));
    /* The device really does stop: pcSND_flipPlayStopState spins on
       GetStatus until the playing bit clears. */
    wsnd_status &= ~1;
    return 0;
}

static HRESULT b_Unlock(IDirectSoundBuffer* s, void* p1, DWORD n1, void* p2, DWORD n2)
{
    /* What was asked for rather than what the lock handed back. A real
       lock splits a write at the end of the buffer and hands back two
       stretches; the voice wraps inside itself and takes one call. The
       bytes are the same bytes either way, so the event is the offset,
       the length and a fingerprint of them. */
    wsnd_ev_write(wsnd_obj_id(s), lock_off, lock_bytes,
                  p1 ? fingerprint((unsigned char*)p1,
                                   (int)(lock_bytes > 4096 ? 4096 : lock_bytes)) : 0);
    return 0;
}

static HRESULT b_Restore(IDirectSoundBuffer* s) { return 0; }

/* --- IDirectSound3DBuffer --- */

static HRESULT d3_GetAllParameters(IDirectSound3DBuffer* s, DS3DBUFFER* b) { return 0; }
static HRESULT d3_SetMaxDistance(IDirectSound3DBuffer* s, float v, DWORD a) { return 0; }
static HRESULT d3_SetMinDistance(IDirectSound3DBuffer* s, float v, DWORD a) { return 0; }
static HRESULT d3_SetMode(IDirectSound3DBuffer* s, DWORD m, DWORD a) { return 0; }
static HRESULT d3_SetPosition(IDirectSound3DBuffer* s, float x, float y, float z, DWORD a)
{
    /* Decoded back into the pan it was made from, because that is
       what it was carrying. The forward map is the engine's own, in
       pcSND_SoundBufferSetPan's note: the pan became an angle, hard
       right at 0 degrees through straight ahead at 90 to hard left at
       180. Inverting it gives the integer back exactly over the whole
       range, and the pan is the thing the rewrite sets. The id is the
       buffer's, not this object's: a 3D buffer speaks for the buffer
       it hangs from. */
    if (z != 0.0f)
    {
        /* Not a pan. This is the parking position the engine gave
           every buffer at creation -- straight ahead at (0,0,1), with
           fixed distances -- so that the per-buffer reverb had a 3D
           buffer to hang off. Nothing parks anything now. */
        return 0;
    }
    {
        const tMockObj* o = (const tMockObj*)s;
        double degrees = atan2((double)y, (double)x) * 57.29577951308232;
        long pan;

        /* Not normalised into a full turn: atan2 answers in
           (-180,180], which is the range the forward map used, and
           wrapping a negative angle up by 360 would read a pan just
           past hard right as one far past hard left. */
        pan = (long)floor((10000.0 - degrees / 0.0089999996) + 0.5);
        wsnd_ev_pan(o->parent >= 0 ? o->parent : wsnd_obj_id(s), pan);
    }
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
    /* Silent. Arming the refill positions is the only way this
       interface had of asking to be told when the cursor moved, and
       the rewrite has no equivalent to ask with -- it looks instead.
       What the positions were for is compared where it shows: the
       refills themselves are writes, and writes are events. */
    return 0;
}

/* --- IKsPropertySet --- */

static HRESULT ks_Get(IKsPropertySet* s, const GUID* g, DWORD id, void* a, DWORD an, void* b, DWORD bn, DWORD* r) { return 0; }

static unsigned fb(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

static HRESULT ks_Set(IKsPropertySet* s, const GUID* g, DWORD id, void* a, DWORD an, void* b, DWORD bn)
{
    /* Silent, listener and buffer alike. The rewrite has no property
       set and no EAX: it answers that the reverb cannot be set, which
       is what a driver without EAX answered, and nothing it does
       corresponds to one of these. */
    return 0;
}

static HRESULT ks_QuerySupport(IKsPropertySet* s, const GUID* g, DWORD id, DWORD* support)
{

    if (wsnd_eax_support < 0)
        return (HRESULT)0x80004005L;
    *(int*)support = wsnd_eax_support;
    return 0;
}

/* --- IDirectSound --- */

static HRESULT ds_CreateSoundBuffer(IDirectSound* s, const DSBUFFERDESC* d, IDirectSoundBuffer** out, IUnknown* unk)
{
    ++create_calls;
    /* The description carried a size, capability flags, a 3D
       algorithm and a whole WAVEFORMATEX; three of those are the
       event -- the channels, the rate and the length -- and the rest
       is this interface asking for a buffer in its own words. The
       primary buffer, which has no format and no length, is not a
       sound and says nothing. */
    if (wsnd_create_fail_mod && (create_calls % wsnd_create_fail_mod) == 0)
    {
        if (d->lpwfxFormat)
            wsnd_log("voice(FAIL,ch=%u,rate=%u,bytes=%u)\n",
                     d->lpwfxFormat->nChannels, d->lpwfxFormat->nSamplesPerSec,
                     d->dwBufferBytes);
        *out = 0;
        return (HRESULT)0x88780032L;
    }
    *out = (IDirectSoundBuffer*)mock_new(1, &buffer_vtbl);
    if (d->lpwfxFormat)
        wsnd_ev_voice(wsnd_obj_id(*out), d->lpwfxFormat->nChannels,
                      d->lpwfxFormat->nSamplesPerSec, d->dwBufferBytes);
    return 0;
}

static HRESULT ds_GetCaps(IDirectSound* s, DSCAPS* c)
{

    c->dwFlags = 0x55;
    c->dwMaxHwMixingAllBuffers = 32;
    return 0;
}

static HRESULT ds_DuplicateSoundBuffer(IDirectSound* s, IDirectSoundBuffer* src, IDirectSoundBuffer** out)
{
    *out = (IDirectSoundBuffer*)mock_new(1, &buffer_vtbl);
    wsnd_log("dup(o%d)=o%d\n", wsnd_obj_id(src), wsnd_obj_id(*out));
    return 0;
}

static HRESULT ds_SetCooperativeLevel(IDirectSound* s, HWND w, DWORD level)
{

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
/* Files number themselves, because events do not exist on the
   rewrite's side and a shared counter would hand the two copies
   different file handles for the same file. */
static int file_seq;

LONG RegOpenKeyExA(HKEY root, LPCSTR path, DWORD opts, DWORD access, PHKEY out)
{

    *out = (HKEY)(size_t)0x1000;
    return 0;
}

LONG RegQueryValueExA(HKEY key, LPCSTR name, DWORD* res, DWORD* type, LPBYTE data, DWORD* size)
{
    int isFx = name[0] == 'S';
    int fail = isFx ? (wsnd_reg_fail & 1) : (wsnd_reg_fail & 2);


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

LONG RegCloseKey(HKEY key) { return 0; }

DWORD GetLastError(void) { return 5; }

DWORD FormatMessageA(DWORD flags, const void* src, DWORD id, DWORD lang, LPSTR buf, DWORD size, va_list* args)
{

    if (size) buf[0] = 0;
    return 0;
}

HMODULE LoadLibraryA(LPCSTR name) { return (HMODULE)(size_t)0x2000; }

static HRESULT WINAPI mock_eax_create(GUID* g, LPDIRECTSOUND8* out, IUnknown* outer)
{

    *out = (IDirectSound8*)the_device;
    return 0;
}

FARPROC GetProcAddress(HMODULE m, LPCSTR name)
{

    return (FARPROC)mock_eax_create;
}

/* The twenty-three events, and the four of them that ever mattered.
 *
 * The frozen copy reaches the sound thread through Windows events: it
 * sets one of four to say what it wants, waits on a fifth for the
 * answer, and the thread waits on the four. The rewrite has a mailbox
 * instead -- one command outstanding, one answer -- so neither side
 * can be compared by the API it uses, and both say the same four
 * things in the same words:
 *
 *     cmd(n)      a command handed to the thread
 *     cmdwait(ms) the poster waiting for the answer
 *     cmddone     the thread giving it
 *     wake=n      the thread's own wait coming back
 *
 * Making the events themselves is ceremony the rewrite has no
 * counterpart for, so it is silent -- but the handles are still
 * numbered 1..23 in creation order, because the rewrite's own tokens
 * are i+1 for the same i and the two are compared where a sound
 * records them. */
HANDLE CreateEventA(void* attrs, BOOL manual, BOOL initial, LPCSTR name)
{
    return (HANDLE)(size_t)(++handle_seq);
}

/* Which of dsndEvents a handle is, or -1. They are made first and in
   order, so the handle is the index plus one. */
static int wsnd_event_index(HANDLE h)
{
    long n = (long)(size_t)h;
    return (n >= 1 && n <= 23) ? (int)(n - 1) : -1;
}

BOOL SetEvent(HANDLE h)
{
    int idx = wsnd_event_index(h);

    if (idx >= 0 && idx <= 3)
        wsnd_log("cmd(%d)\n", idx);
    else if (idx == 5)
        wsnd_log("cmddone\n");
    else
        wsnd_log("SetEvent(%ld)\n", (long)(size_t)h);
    return 1;
}

/* Silent: the rewrite neither makes these nor resets them. */
BOOL ResetEvent(HANDLE h) { (void)h; return 1; }
BOOL CloseHandle(HANDLE h)
{
    /* Events and the thread handle are both silent: the rewrite makes
       neither, so neither has anything to close. A file is the one
       handle both copies really hold, and that one is said. */
    if (wsnd_event_index(h) >= 0 || (long)(size_t)h == 0x3000)
        return 1;
    /* The only handle left that both copies really hold is the big
       file, and the rewrite gives it back through SPL_FileClose. */
    wsnd_log("fclose(%ld)\n", (long)(size_t)h);
    return 1;
}

DWORD WaitForSingleObject(HANDLE h, DWORD ms)
{
    if (wsnd_event_index(h) == 5)
        wsnd_log("cmdwait(%u)\n", ms);
    else if ((long)(size_t)h == 0x3000)
        wsnd_log("threadwait(%u)\n", ms);
    else
        wsnd_log("WaitForSingleObject(%ld,%u)\n", (long)(size_t)h, ms);
    return 0;
}

HANDLE CreateThread(void* attrs, SIZE_T stack, LPTHREAD_START_ROUTINE start, LPVOID param, DWORD flags, LPDWORD id)
{
    wsnd_log("threadstart\n");
    *(int*)id = 4242;
    return (HANDLE)(size_t)0x3000;
}

/* The big file, in the words both copies say it in.
 *
 * The frozen copy opens, reads and seeks it through Windows and the
 * rewrite through the platform layer, so neither can be compared by
 * the call it makes. What both of them do is the same four things:
 *
 *     fopen(path)      the big file opened
 *     fread(h,n)=got   a read, and how much came back
 *     fseek(h,off)     an absolute seek from the start
 *     fclose(h)        the file given back
 *
 * The seek's method is not said, because every one of them is from
 * the start -- that is the only kind the sound does, and it is why
 * the platform layer takes an offset and nothing else. */
static int wsnd_file_read(long id, void* buf, int count)
{
    int n = wsnd_read_bytes;
    int i;
    unsigned char* p = (unsigned char*)buf;

    if (n > count) n = count;
    for (i = 0; i < n; ++i)
        p[i] = adpcm_byte(i, 3);
    wsnd_log("fread(%ld,%u)=%u\n", id, (unsigned)count, (unsigned)n);
    return n;
}

HANDLE CreateFileA(LPCSTR path, DWORD access, DWORD share, void* sec, DWORD disp, DWORD attrs, HANDLE templ)
{
    wsnd_log("fopen(%s)\n", path);
    return (HANDLE)(size_t)(0x5000 + (++file_seq));
}

BOOL ReadFile(HANDLE h, LPVOID buf, DWORD count, LPDWORD read, void* ov)
{
    *(int*)read = wsnd_file_read((long)(size_t)h, buf, (int)count);
    return 1;
}

DWORD SetFilePointer(HANDLE h, LONG move, LONG* high, DWORD method)
{
    wsnd_log("fseek(%ld,%ld)\n", (long)(size_t)h, (long)move);
    return (DWORD)move;
}

struct sSplFile;
struct sSplFile* SPL_p_FileOpen(const char* path)
{
    wsnd_log("fopen(%s)\n", path);
    return (struct sSplFile*)(size_t)(0x5000 + (++file_seq));
}

int SPL_i_FileRead(struct sSplFile* file, void* dest, int bytes)
{
    return wsnd_file_read((long)(size_t)file, dest, bytes);
}

int SPL_b_FileSeek(struct sSplFile* file, long long offset)
{
    wsnd_log("fseek(%ld,%ld)\n", (long)(size_t)file, (long)offset);
    return 1;
}

void SPL_FileClose(struct sSplFile* file)
{
    wsnd_log("fclose(%ld)\n", (long)(size_t)file);
}

void OutputDebugStringA(LPCSTR s) { wsnd_log("OutputDebugStringA(%s)\n", s); }
/* Silent, where OutputDebugStringA above is logged. The rewrite says
   something when the sound thread fails to answer in five seconds, and
   the frozen copy has no counterpart to say it with -- it waited the
   same five seconds and said nothing. The wait itself is logged either
   way, with its handle and its timeout, so the behaviour is compared
   and only the diagnostic is one-sided. A one-sided diagnostic that
   logged would be a difference at every site rather than a check. */
void DBG_OutputDebugString(char* format, ...) { (void)format; }
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

/* How far the play cursor moves when the device reaches a refill
   point. Set by the driver to the block size of the sound it is
   driving; see the note below. */
int wsnd_play_pos_step;

/* Set by the rewrite's copy of the driver. A wake-up the frozen copy
   reads as "the device reached a refill position" is, to the rewrite,
   nothing but a tick: it is told no positions and looks at the cursor
   instead. So the same scripted event is handed over as the index to
   one and as a timeout to the other, and the cursor moves either way.
*/
int wsnd_notify_is_tick;

DWORD WaitForMultipleObjects(DWORD count, const HANDLE* handles, BOOL all, DWORD ms)
{
    DWORD r;
    ++wsnd_waits;
    if (thread_step < thread_script_len)
        r = (DWORD)thread_script[thread_step];
    else
        r = 2;                   /* the thread's own exit case */
    ++thread_step;

    /* The count is not logged. The frozen copy waits on four control
       events and two more for every streaming sound; the rewrite
       waits on the four and looks at the sounds itself, so the two
       numbers are the shape of the wait and not an event.
       
       A scripted wake-up of 4 or more was the device saying the play
       cursor had reached a position the refill was armed at. The
       rewrite is told no such thing -- it compares the block the
       cursor is in against the block it last refilled for -- so the
       same event has to be expressed the way it reads it, and that is
       the cursor having moved a block. Even values were the refill
       arm and odd ones the stop arm, which is the same split the
       frozen copy makes on (signalled & 1). */
    /* As a signed value: the script's own sentinel for a wait that
       failed is -1, which as an unsigned is larger than every real
       index and was being read as one. */
    if ((int)r >= 4)
    {
        /* The cursor moves for both copies, because the accounting it
           feeds -- pcSND_updateCurrentPlayPositionAndSize, and the
           decision to take a finished sound off the list -- is shared
           code that both of them run. Moving it for only one makes
           their bookkeeping drift apart. */
        if ((r & 1) == 0)
        {
            /* A buffer that reached a refill position is playing --
               that is how it got there -- and the cursor has moved a
               block, which is the same event said the way the rewrite
               reads it. */
            wsnd_status |= 1;
            wsnd_play_pos += wsnd_play_pos_step;
        }
        else
        {
            wsnd_status &= ~1;   /* and the other arm is it stopping */
        }
        /* The rewrite is handed it as a tick: it is told no refill
           positions and looks at the cursor instead. */
        if (wsnd_notify_is_tick)
        {
            ++wsnd_ticks;
            wsnd_log("wake=%u\n", r);
            return WAIT_TIMEOUT;
        }
    }
    if (r == (DWORD)-1)
        ++wsnd_errors;
    else if (r == WAIT_TIMEOUT)
        ++wsnd_ticks;
    wsnd_log("wake=%u\n", r);
    return r;
}

/* The mailbox the rewrite waits on, driven by the same script and
   saying the same words. SPL_CMD_NONE is the timeout and SPL_CMD_ERROR
   the wait that failed, which are WAIT_TIMEOUT and -1 above. */
int SPL_i_TakeCommand(int timeoutMs)
{
    DWORD r = WaitForMultipleObjects(4, 0, 0, (DWORD)timeoutMs);

    if (r == WAIT_TIMEOUT)
        return -1;                     /* SPL_CMD_NONE */
    if (r == (DWORD)-1)
        return -2;                     /* SPL_CMD_ERROR */
    return (int)r;
}

int SPL_b_Command(int command, int timeoutMs)
{
    wsnd_log("cmd(%d)\n", command);
    wsnd_log("cmdwait(%u)\n", (unsigned)timeoutMs);
    return 1;
}

void SPL_CommandDone(void) { wsnd_log("cmddone\n"); }

struct sSplThread;
struct sSplThread* SPL_p_ThreadStart(int (*body)(void*), void* arg, const char* name)
{
    (void)body; (void)arg; (void)name;
    wsnd_log("threadstart\n");
    return (struct sSplThread*)(size_t)0x3000;
}

int SPL_b_ThreadWait(struct sSplThread* thread, int timeoutMs)
{
    (void)thread;
    wsnd_log("threadwait(%u)\n", (unsigned)timeoutMs);
    return 1;
}

void ExitThread(DWORD code)
{
    /* Silent: the rewrite returns from the body instead, and
       wsnd_run_thread says so for both. */
    (void)code;
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
    /* Said here rather than in ExitThread, because the rewrite leaves
       the loop by returning and there is nothing to intercept. */
    wsnd_log("threadexit\n");
}

/* ------------------------------------------------------------- reset */

/* Everything up to here is forgotten, and the object ids start again.
   One caller: the module's own init, which is the one entry point the
   two copies cannot be compared on. The frozen copy opens a named
   driver, makes a primary buffer, queries a 3D listener off it and
   makes a second buffer for no purpose but asking EAX whether it is
   there; the rewrite opens a device. Four objects and five calls with
   nothing on the other side to line them up with, and every object
   made afterwards numbered five apart. So the init is not compared,
   and the comparison starts from the state it left behind. */
/* What the init left in the log is dropped, for the same reason its
   objects are not numbered: there is nothing on the other side to
   compare it against. The table itself is left alone -- those objects
   are still live and still held. */
void wsnd_log_restart(void)
{
    /* The log and nothing else. The call counters are what the
       failure knobs count against -- every nth create, every nth
       lock -- so zeroing them here would re-script which calls fail
       from the middle of a case, which is not what dropping the log
       is for. */
    log_len = 0;
}

void wsnd_mock_reset(void)
{
    log_len = 0;
    wsnd_init_quiet = 0;
    wsnd_play_pos_step = 0;
    quiet_count = 0;
    id_next = 0;
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
    file_seq = 0;
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

/* ------------------------------------------- the mock platform layer */

/* The rewrite's side of the shared vocabulary. Each of these logs the
   event its DirectSound counterpart logs, in the same words, so the
   two sides can still be compared on what they did rather than on how
   they said it. The voices come out of the same object table as the
   buffers, so the Nth voice and the Nth buffer have the same id and
   the lines line up.
   
   The getters answer what the buffer mock answered, for the same
   reason: the engine does the same arithmetic on either, and that
   arithmetic is what is being compared. */

int wsnd_open_fails;

static struct
{
    int bytes, channels, freq;
    int att, pan, cursor;
} voice_state[MOCK_MAX];

static int wsnd_voice_id(const void* v) { return wsnd_obj_id(v); }

int SPL_b_Open(void) { return !wsnd_open_fails; }
void SPL_Close(void) { }
int SPL_b_IsOpen(void) { return !wsnd_open_fails; }
/* Counted rather than logged. A feed is one-sided -- the frozen copy
   has a device that pulls from a buffer and never feeds anything -- so
   it cannot go in the shared log. The count is what the one-sided
   check in winsound_side.c reads. */
int wsnd_feeds;
int wsnd_waits;
int wsnd_ticks;
int wsnd_errors;
void SPL_Feed(void) { ++wsnd_feeds; }
/* Only ever referenced by the trace, which is off here. Defined so
   that turning the trace on in this build links. */
int SPL_i_VoiceQueued(const struct sSplVoice* v) { (void)v; return 0; }
int SPL_b_FeedWanted(void) { return 0; }
void SPL_SetAttenuation(int v) { wsnd_log("devatt(%d)\n", v); }
int SPL_i_Attenuation(void) { return 0; }
void SPL_SetPan(int v) { wsnd_log("devpan(%d)\n", v); }
int SPL_i_Pan(void) { return 0; }

tSplVoice* SPL_p_VoiceCreate(int channels, int freq, int bytes)
{
    void* v;
    int id;

    ++create_calls;
    if (wsnd_create_fail_mod && (create_calls % wsnd_create_fail_mod) == 0)
    {
        wsnd_log("voice(FAIL,ch=%u,rate=%u,bytes=%u)\n",
                 (unsigned)channels, (unsigned)freq, (unsigned)bytes);
        return 0;
    }
    v = mock_new(1, &buffer_vtbl);
    id = wsnd_voice_id(v);
    if (id < 0 || id >= MOCK_MAX)
        return (tSplVoice*)v;
    voice_state[id].bytes = bytes;
    voice_state[id].channels = channels;
    voice_state[id].freq = freq;
    wsnd_ev_voice(id, (unsigned)channels, (unsigned)freq, (unsigned)bytes);
    return (tSplVoice*)v;
}

tSplVoice* SPL_p_VoiceDuplicate(const tSplVoice* src)
{
    void* v = mock_new(1, &buffer_vtbl);
    wsnd_log("dup(o%d)=o%d\n", wsnd_voice_id(src), wsnd_voice_id(v));
    return (tSplVoice*)v;
}

void SPL_VoiceDestroy(tSplVoice* v) { if (v) wsnd_ev_drop(wsnd_voice_id(v)); }

/* An id from before the log was restarted prints negative, so every
   read of the side table is bounded. */
static int voice_slot(const tSplVoice* v)
{
    int id = v ? wsnd_voice_id(v) : -1;
    return (id >= 0 && id < MOCK_MAX) ? id : -1;
}

int SPL_i_VoiceBytes(const tSplVoice* v)
{
    int i = voice_slot(v);
    return i < 0 ? 0 : voice_state[i].bytes;
}

int SPL_i_VoiceChannels(const tSplVoice* v)
{
    int i = voice_slot(v);
    return i < 0 ? 0 : voice_state[i].channels;
}
int SPL_i_ChunkBytes(const tSplVoice* v) { return 480 * 2 * SPL_i_VoiceChannels(v); }

int SPL_b_VoiceWrite(tSplVoice* v, int offset, const void* data, int bytes)
{
    unsigned n;

    ++lock_calls;
    if (wsnd_lock_fail_mod && (lock_calls % wsnd_lock_fail_mod) == 0)
    {
        wsnd_log("wr(o%d,%u,%u,FAIL)\n", wsnd_voice_id(v), (unsigned)offset, (unsigned)bytes);
        return 0;
    }
    /* The same bound as the lock above, and for the same reason: a
       real streamed sound writes its whole ring at the prefetch, and
       a mock that quietly logged 64 KB of it made a rewrite that
       quietly wrote 64 KB of it look right. */
    n = (unsigned)bytes > (unsigned)WSND_LOCK_MAX ? (unsigned)WSND_LOCK_MAX : (unsigned)bytes;
    wsnd_ev_write(wsnd_voice_id(v), (unsigned)offset, n,
                  data ? fingerprint((const unsigned char*)data,
                                     (int)(n > 4096 ? 4096 : n)) : 0);
    return 1;
}

int SPL_b_VoiceSilence(tSplVoice* v, int offset, int bytes)
{
    static const unsigned char zeros[4096];
    unsigned n;

    /* Not forwarded to the write with a clamped length: that reported
       a shorter write than was asked for, and a silence that wrote
       sound instead then looked the same. The length is the length
       asked for, and the fingerprint is of zeros. */
    ++lock_calls;
    if (wsnd_lock_fail_mod && (lock_calls % wsnd_lock_fail_mod) == 0)
    {
        wsnd_log("wr(o%d,%u,%u,FAIL)\n", wsnd_voice_id(v), (unsigned)offset, (unsigned)bytes);
        return 0;
    }
    n = (unsigned)bytes > (unsigned)WSND_LOCK_MAX ? (unsigned)WSND_LOCK_MAX : (unsigned)bytes;
    wsnd_ev_write(wsnd_voice_id(v), (unsigned)offset, n,
                  fingerprint(zeros, (int)(n > 4096 ? 4096 : n)));
    return 1;
}

void SPL_VoicePlay(tSplVoice* v, int loop)
{
    wsnd_ev_play(wsnd_voice_id(v), loop ? 1 : 0);
    wsnd_status |= 1;
}

void SPL_VoiceStop(tSplVoice* v)
{
    wsnd_ev_stop(wsnd_voice_id(v));
    wsnd_status &= ~1;
}

void SPL_VoicePause(tSplVoice* v)
{
    wsnd_ev_stop(wsnd_voice_id(v));
    wsnd_status &= ~1;
}

int SPL_i_VoiceStatus(const tSplVoice* v)
{
    int st = 0;
    (void)v;
    if (wsnd_status & 1)
        st |= SPL_PLAYING;
    if (wsnd_status & 4)
        st |= SPL_LOOPING;
    return st;
}

void SPL_VoiceSetCursor(tSplVoice* v, int off) { wsnd_ev_cursor(wsnd_voice_id(v), (unsigned)off); }
int  SPL_i_VoiceCursor(const tSplVoice* v) { (void)v; return wsnd_play_pos; }
/* 64 ahead of the play cursor, which is what the buffer mock answers
   for the write cursor: the device holds a little of what it has not
   played yet, and the engine's arithmetic on the gap is the thing
   being compared. */
int  SPL_i_VoiceWriteCursor(const tSplVoice* v) { (void)v; return wsnd_play_pos + 64; }

void SPL_VoiceSetAttenuation(tSplVoice* v, int db) { wsnd_ev_att(wsnd_voice_id(v), db); }
int  SPL_i_VoiceAttenuation(const tSplVoice* v) { (void)v; return -1234; }
void SPL_VoiceSetPan(tSplVoice* v, int db) { wsnd_ev_pan(wsnd_voice_id(v), db); }
int  SPL_i_VoicePan(const tSplVoice* v) { (void)v; return 0; }
void SPL_VoiceSetFrequency(tSplVoice* v, int hz) { wsnd_ev_freq(wsnd_voice_id(v), (unsigned)hz); }
int  SPL_i_VoiceFrequency(const tSplVoice* v) { (void)v; return 22050; }
