/* See soundmod_mocks.h. */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "soundmod_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../dbg.h"
#include "../gameobject.h"

/* ---------------------------------------------------------------- log */

#define LOG_MAX (16 * 1024 * 1024)
static char log_buf[LOG_MAX];
static int log_len;

void sm_log(const char* fmt, ...)
{
    va_list ap;
    int n;

    if (log_len >= LOG_MAX - 1024)
        return;
    va_start(ap, fmt);
    n = vsnprintf(log_buf + log_len, LOG_MAX - log_len, fmt, ap);
    va_end(ap);
    if (n > 0)
        log_len += n;
}

const char* sm_log_text(void) { return log_buf; }
int sm_log_len(void) { return log_len; }

/* ---------------------------------------------------- the bump arena */

/* Nothing is ever handed back: the first side's state has to stay
   readable while the second side runs, and the call log carries the
   allocation sequence instead. Every block sits 64 bytes into a larger
   one, so a copy that writes a little way outside its block does not
   make the process die where the real engine would run on. */

#define HEAP_MAX (16 * 1024 * 1024)
#define GUARD 64
static unsigned char heap[HEAP_MAX];
static int heap_used;

const unsigned char* sm_heap_base(void) { return heap; }

long sm_heap_off(const void* p)
{
    if (!p) return -1;
    if ((const unsigned char*)p < heap || (const unsigned char*)p >= heap + HEAP_MAX)
        return -2;
    return (long)((const unsigned char*)p - heap);
}

static void* heap_alloc(unsigned int size)
{
    unsigned char* p;
    unsigned int room = (size + 15u) & ~15u;

    if (heap_used + (int)room + 2 * GUARD > HEAP_MAX)
    {
        sm_log("HEAP EXHAUSTED\n");
        abort();
    }
    heap_used += GUARD;
    p = heap + heap_used;
    heap_used += (int)room + GUARD;
    memset(p - GUARD, 0xAB, room + 2 * GUARD);
    memset(p, 0, room);
    return p;
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = heap_alloc(size);
    sm_log("MEM_p_Alloc(%u)=%ld\n", size, sm_heap_off(p));
    return p;
}

void* MEM_p_AllocAligned(int size, int alignment) { return heap_alloc((unsigned)size); }
void* MEM_p_Realloc(void* old, size_t size)
{
    void* p = heap_alloc((unsigned)size);
    sm_log("MEM_p_Realloc(%ld,%u)=%ld\n", sm_heap_off(old), (unsigned)size, sm_heap_off(p));
    return p;
}
void* MEM_p_ReallocAligned(void* p, unsigned int size, int alignment) { return MEM_p_Realloc(p, (size_t)size); }
void MEM_Free(void* p) { sm_log("MEM_Free(%ld)\n", sm_heap_off(p)); }
void MEM_FreeAlign(void* p) { }
BOOL MEM_b_CheckBlockHasBeenFreed(void* p) { return 0; }

/* ------------------------------------------------------ the big file */

#define BIG_MAX 16
static struct { unsigned int key; const void* data; int len; } big[BIG_MAX];
static int big_n;

void sm_bigfile_clear(void) { big_n = 0; }

int sm_bigfile_add(unsigned int key, const void* data, int len)
{
    if (big_n >= BIG_MAX) abort();
    big[big_n].key = key;
    big[big_n].data = data;
    big[big_n].len = len;
    return big_n++;
}

/* A key's position is its slot times 1024, so the caller's Offset and
   Offset+4 both decode back to the same slot. */
#define BIG_STRIDE 1024

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    int i;
    for (i = 0; i < big_n; ++i)
        if (big[i].key == key)
            return i * BIG_STRIDE;
    return -1;
}

void BIG_ReadNoSeek(int pos, void* dest, unsigned int len)
{
    int slot = pos / BIG_STRIDE;
    int off = pos % BIG_STRIDE;

    sm_log("BIG_ReadNoSeek(%d,%u)\n", pos, len);
    if (len == 0)
        return;
    if (slot < 0 || slot >= big_n || off + (int)len > big[slot].len)
    {
        memset(dest, 0, len);
        return;
    }
    memcpy(dest, (const unsigned char*)big[slot].data + off, len);
}

char* BIG_pc_ReadFileTmp(int Offset, int* size) { return 0; }

/* -------------------------------------------------- the loader stream */

static const unsigned char* stream_data;
static int stream_len;
static char* stream_cur;

void sm_stream_set(const void* data, int len)
{
    stream_data = (const unsigned char*)data;
    stream_len = len;
    stream_cur = (char*)data;
}

char** sm_stream_cursor(void) { return &stream_cur; }

int sm_speed_mode;

BOOL isSpeedModeAndBinFile(void) { return sm_speed_mode; }

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    uint32_t v = 0;
    memcpy(&v, *offset, 4);
    *offset += 4;
    if (retVal) *retVal = v;
    sm_log("LOA_ReadLong=%u\n", v);
    return (int)v;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    uint16_t v = 0;
    memcpy(&v, *offset, 2);
    *offset += 2;
    if (retVal) *retVal = v;
    return v;
}

char** LOA_ReadCharArray(char** offset, char* retVal, unsigned int len, char readMode)
{
    sm_log("LOA_ReadCharArray(%u)\n", len);
    memcpy(retVal, *offset, len);
    *offset += len;
    return offset;
}

char* LOA_FetchBuffer(int size) { return (char*)heap_alloc((unsigned)size); }
void LOA_BeginSpeedMode(int key) { }
void LOA_EndSpeedMode(void) { }
void LOA_DeleteAddress(int pos) { }
int LOA_ul_GetCurrentKey(void) { return 0x11223344; }

/* ------------------------------------------------------------- debug */

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine,
                const char* error, const char* extra, int showMsgBox)
{
    sm_log("DBG_assert(%d,\"%s\")\n", isError, error ? error : "");
}

void OBJ_GameObject_CreateExtendedIfNot(tGameObjectMain* gao) { }

/* --------------------------------------------- the attenuation curve
 *
 * sound.c's SND_f_GetVolFromAtt interpolates a 256-entry table that is
 * an exponential curve in hundredths of a decibel. The real table is a
 * file-scope object in sound.c, which is not linked here, so the curve
 * is computed instead. It is pure, so both sides share this one copy.
 */
float SND_f_GetVolFromAtt(int att)
{
    double v;
    if (att >= 0) return 1.0f;
    if (att <= -10000) return 0.0f;
    v = pow(10.0, (double)att / 2000.0);
    return (float)v;
}

/* --------------------------------------------------------- the reset */

void sm_reset(void)
{
    log_len = 0;
    heap_used = 0;
    memset(heap, 0, HEAP_MAX);
    big_n = 0;
    stream_data = 0;
    stream_len = 0;
    stream_cur = 0;
    sm_speed_mode = 0;
}
