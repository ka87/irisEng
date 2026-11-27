/* A debugging replacement for the engine's pool allocator, included by
 * memory.c only when IRIS_MEM_DEBUG is defined -- which the CMake option
 * of that name does, together with -fsanitize=address:
 *
 *   cmake -S . -B build-asan --toolchain $PWD/cmake/linux-i386.cmake \
 *         -DIRIS_MEM_DEBUG=ON
 *   ASAN_OPTIONS=detect_leaks=0:halt_on_error=0 ./irisEng
 *
 * Every engine block becomes its own malloc of exactly the requested
 * size, so AddressSanitizer puts a red zone straight after it and keeps
 * freed blocks in quarantine: a write past the requested size or into a
 * freed block is reported with the stacks of the write, the allocation
 * and the free. Inside the pool ASan sees one 40 MB block and nothing
 * else, so this is what makes it useful.
 *
 * The block header the engine reads from outside is kept: [-2] is the
 * magic (0xA0110270 live, 0xA0C0FFEE aligned descriptor), [-1] the size.
 * Liveness is answered from a table, never by reading a block that may
 * have been freed.
 */
/* AddressSanitizer if the build has it, and the same allocator
   without it if not.
 *
 * It used to refuse to compile without it, which kept this file off
 * the one toolchain the game is actually built with: MSVC takes
 * /fsanitize=address, but not alongside the edit-and-continue and
 * runtime checks a Debug configuration turns on by default, so asking
 * for both at once is a configuration change rather than a flag. The
 * separation alone -- one malloc for every engine block -- is most of
 * what this is for: a write past a block lands in the C library's own
 * guard rather than in the next engine object, and a block handed back
 * twice is reported here by name. The sanitizer adds the red zone, the
 * quarantine and the three stacks, and is worth having where it can be
 * had.
 *
 * __SANITIZE_ADDRESS__ is defined by both gcc and MSVC when the
 * sanitizer is on. The stack-trace entry point is in a second header
 * that not every toolchain ships, hence the separate test. */
#if defined(__SANITIZE_ADDRESS__)
#if !defined(__has_include)
#include <sanitizer/asan_interface.h>
#include <sanitizer/common_interface_defs.h>
#define MDBG_HAVE_STACKS 1
#else
#if __has_include(<sanitizer/asan_interface.h>)
#include <sanitizer/asan_interface.h>
#endif
#if __has_include(<sanitizer/common_interface_defs.h>)
#include <sanitizer/common_interface_defs.h>
#define MDBG_HAVE_STACKS 1
#endif
#endif
#endif

#ifndef ASAN_POISON_MEMORY_REGION
#define ASAN_POISON_MEMORY_REGION(p, n)   ((void)(p), (void)(n))
#define ASAN_UNPOISON_MEMORY_REGION(p, n) ((void)(p), (void)(n))
#endif
#ifndef MDBG_HAVE_STACKS
#define __sanitizer_print_stack_trace() ((void)0)
#endif

#define MDBG_KIND_NORMAL  1
#define MDBG_KIND_ALIGNED 2
#define MDBG_KIND_TMP     3

typedef struct sMdbgHdr
{
    struct sMdbgHdr* nextTmp;   /* chain of live temp blocks */
    void*            aligned;   /* aligned pointer carved from this block */
    uint32_t         kind;
    uint32_t         pad;
    uint32_t         magic;     /* user[-2] */
    uint32_t         size;      /* user[-1] */
} tMdbgHdr;

/* The header is as big as it measures, which was written out as 24 --
   its width where a pointer is four bytes. At eight it is 32, so the
   user pointer was handed out eight bytes inside the header and the
   pattern every allocation writes over its own payload landed on
   `magic` and `size` instead. Every block corrupted its own
   bookkeeping, which is a poor start for the thing that is supposed to
   find corruption. */
#define MDBG_HDR ((int)sizeof(tMdbgHdr))

/* Open-addressed table of live user pointers. */
#define MDBG_SLOTS (1u << 21)
#define MDBG_TOMB  ((void*)1)
typedef struct { void* user; void* raw; uint32_t size; uint32_t kind; } tMdbgSlot;
static tMdbgSlot* mdbg_table;
static tMdbgHdr*  mdbg_tmpHead;
static unsigned   mdbg_live;

static unsigned MDBG_l_Hash(void* p)
{
    uint32_t x = (uint32_t)(size_t)p;
    x ^= x >> 16; x *= 0x7feb352d; x ^= x >> 15;
    return x & (MDBG_SLOTS - 1);
}

static tMdbgSlot* MDBG_l_Find(void* user)
{
    unsigned i = MDBG_l_Hash(user);
    if (!mdbg_table)
        return NULL;
    for (;;)
    {
        tMdbgSlot* s = &mdbg_table[i];
        if (!s->user)
            return NULL;
        if (s->user == user)
            return s;
        i = (i + 1) & (MDBG_SLOTS - 1);
    }
}

static void MDBG_l_Insert(void* user, void* raw, uint32_t size, uint32_t kind)
{
    unsigned i;
    if (!mdbg_table)
        mdbg_table = (tMdbgSlot*)calloc(MDBG_SLOTS, sizeof(tMdbgSlot));
    i = MDBG_l_Hash(user);
    while (mdbg_table[i].user && mdbg_table[i].user != MDBG_TOMB)
        i = (i + 1) & (MDBG_SLOTS - 1);
    mdbg_table[i].user = user;
    mdbg_table[i].raw = raw;
    mdbg_table[i].size = size;
    mdbg_table[i].kind = kind;
    if (++mdbg_live > MDBG_SLOTS / 2)
    {
        fprintf(stderr, "MEMDEBUG: table full\n");
        abort();
    }
}

/* How many times this file has had something to say. A run that
   reports nothing and a run whose reports went past unread look the
   same otherwise, and a test cannot ask the difference. */
unsigned MDBG_gul_Reports;

static void MDBG_l_Report(const char* what, void* p)
{
    ++MDBG_gul_Reports;
    fprintf(stderr, "MEMDEBUG: %s %p\n", what, p);
    __sanitizer_print_stack_trace();
}

static void* MDBG_l_Alloc(unsigned int size, uint32_t kind)
{
    char* raw = (char*)malloc(MDBG_HDR + (size ? size : 1));
    tMdbgHdr* h = (tMdbgHdr*)raw;
    void* user = raw + MDBG_HDR;

    if (!raw)
        return NULL;
    h->nextTmp = NULL;
    h->aligned = NULL;
    h->kind = kind;
    h->magic = 0xA0110270;
    h->size = size;
    /* what the pool handed out was not cleared either, but give it a
       pattern rather than whatever malloc had */
    memset(user, 0xCD, size);
    if (kind == MDBG_KIND_TMP)
    {
        h->nextTmp = mdbg_tmpHead;
        mdbg_tmpHead = h;
    }
    MDBG_l_Insert(user, raw, size, kind);
    MEM_gst_MemoryInfo.usedMemoryByEngine += size;
    return user;
}

static void MDBG_l_UnlinkTmp(tMdbgHdr* h)
{
    tMdbgHdr** pp = &mdbg_tmpHead;
    while (*pp && *pp != h)
        pp = &(*pp)->nextTmp;
    if (*pp)
        *pp = h->nextTmp;
}

static int MDBG_l_Free(void* user)
{
    tMdbgSlot* s = MDBG_l_Find(user);
    void* raw;
    uint32_t kind;

    if (!s)
        return 0;
    raw = s->raw;
    kind = s->kind;
    /* An aligned block is a view on the inner block it was carved
       from, and only the inner one was ever counted in -- so counting
       both of them out took the engine's byte total below zero, and
       it is unsigned. */
    if (kind != MDBG_KIND_ALIGNED)
        MEM_gst_MemoryInfo.usedMemoryByEngine -= s->size;
    s->user = MDBG_TOMB;
    mdbg_live--;
    if (kind == MDBG_KIND_TMP)
        MDBG_l_UnlinkTmp((tMdbgHdr*)raw);
    if (kind == MDBG_KIND_ALIGNED)
    {
        /* the descriptor block it lives in */
        void* inner = (void*)(size_t)((MEM_Word*)user)[-1];
        return MDBG_l_Free(inner);
    }
    ((tMdbgHdr*)raw)->magic = 0xB0110270;
    free(raw);
    return 1;
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p;
    MEM_l_TakeLock();
    p = MDBG_l_Alloc(size, MDBG_KIND_NORMAL);
    MEM_l_GiveLock();
    return p;
}

static void* MDBG_l_AllocAligned(int size, int alignment, uint32_t kind)
{
    char* inner;
    char* user;

    if (alignment < 8)
        alignment = 8;
    /* an inner block, and the aligned pointer inside it with the inner
       block's address in the word before it, as the pool does.

       The word is a whole address: written as a uint32_t it kept the
       low half, so every aligned block was freed through a truncated
       pointer that the table did not know, and every one of them
       leaked and was reported as a free of something that is not a
       block. Aligned blocks are most of the geometry, so that was the
       first thing this file would have said on a run. */
    inner = (char*)MDBG_l_Alloc(
        (unsigned)(size + alignment + (int)sizeof(MEM_Word)),
        kind == MDBG_KIND_TMP ? MDBG_KIND_TMP : MDBG_KIND_NORMAL);
    if (!inner)
        return NULL;
    user = (char*)(((size_t)inner + sizeof(MEM_Word) + alignment - 1)
                       & ~(size_t)(alignment - 1));
    ((MEM_Word*)user)[-1] = (MEM_Word)(size_t)inner;
    ((tMdbgHdr*)(inner - MDBG_HDR))->magic = 0xA0C0FFEE;
    ((tMdbgHdr*)(inner - MDBG_HDR))->aligned = user;
    /* the slack after the requested size is not the caller's */
    ASAN_POISON_MEMORY_REGION(
        user + size,
        (inner + size + alignment + (int)sizeof(MEM_Word)) - (user + size));
    MDBG_l_Insert(user, inner, (uint32_t)size, MDBG_KIND_ALIGNED);
    return user;
}

void* MEM_p_AllocAligned(int size, int alignment)
{
    void* p;
    MEM_l_TakeLock();
    p = MDBG_l_AllocAligned(size, alignment, MDBG_KIND_NORMAL);
    MEM_l_GiveLock();
    return p;
}

void MEM_Free(void* ptr)
{
    tMdbgSlot* s;
    if (!ptr)
        return;
    MEM_l_TakeLock();
    s = MDBG_l_Find(ptr);
    if (s && s->kind == MDBG_KIND_ALIGNED)
    {
        void* inner = s->raw;
        size_t total = ((tMdbgHdr*)((char*)inner - MDBG_HDR))->size;
        ASAN_UNPOISON_MEMORY_REGION(inner, total);
    }
    if (!MDBG_l_Free(ptr))
        MDBG_l_Report("free of a pointer that is not a live block:", ptr);
    MEM_l_GiveLock();
}

void MEM_FreeAlign(void* ptr) { MEM_Free(ptr); }
void MEM_FreeTmp(void* ptr)   { MEM_Free(ptr); }

void* MEM_p_AllocTmp(int size)
{
    void* p;
    MEM_l_TakeLock();
    p = MDBG_l_AllocAligned((size + 128) & 0xFFFFFFC0, 64, MDBG_KIND_TMP);
    MEM_l_GiveLock();
    return p;
}

void MEM_ResetTmpMemory()
{
    /* Everything allocated as temporary dies here, as it did in the
       pool; freeing it lets ASan catch whatever still uses it. */
    MEM_l_TakeLock();
    while (mdbg_tmpHead)
    {
        tMdbgHdr* h = mdbg_tmpHead;
        void* user = (char*)h + MDBG_HDR;
        mdbg_tmpHead = h->nextTmp;
        if (h->aligned)
        {
            /* the aligned pointer carved out of it dies with it */
            tMdbgSlot* a = MDBG_l_Find(h->aligned);
            if (a)
            {
                a->user = MDBG_TOMB;
                mdbg_live--;
            }
        }
        {
            tMdbgSlot* s = MDBG_l_Find(user);
            if (s)
            {
                ASAN_UNPOISON_MEMORY_REGION(user, h->size);
                s->user = MDBG_TOMB;
                mdbg_live--;
                MEM_gst_MemoryInfo.usedMemoryByEngine -= h->size;
            }
        }
        h->magic = 0xB0110270;
        free(h);
    }
    MEM_gp_AllocTmpNext = MEM_gp_AllocTmpFirst;
    MEM_gi_AllocTmpNb = 0;
    MEM_l_GiveLock();
    SOFT_ZList_Init();
    SOFT_ZList_Clear();
}

BOOL MEM_Defrag(int onlyOneHole) { (void)onlyOneHole; return TRUE; }

void MEM_GetHolesStat(int* holesNum, int* sizeOfHoles)
{
    if (holesNum) *holesNum = 0;
    if (sizeOfHoles) *sizeOfHoles = 0;
}

void* MEM_MoveToTempMem(void* ptr, size_t Size)
{
    void* newPtr = MEM_p_AllocTmp((int)Size);
    memcpy(newPtr, ptr, Size);
    MEM_Free(ptr);
    return newPtr;
}

void* MEM_GEO_p_Alloc(unsigned int size) { return MEM_p_Alloc(size); }
void* MEM_GEO_p_AllocAlign(int size, int alignment) { return MEM_p_AllocAligned(size, alignment); }

static uint32_t MDBG_l_SizeOf(void* ptr)
{
    tMdbgSlot* s = MDBG_l_Find(ptr);
    if (!s)
    {
        MDBG_l_Report("size asked of a pointer that is not a live block:", ptr);
        return 0;
    }
    return s->size;
}

void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment)
{
    void* newPtr = MEM_p_AllocAligned((int)size, alignment);
    uint32_t old = MDBG_l_SizeOf(ptr);
    memcpy(newPtr, ptr, old < size ? old : size);
    MEM_Free(ptr);
    return newPtr;
}

void* MEM_p_Realloc(void* Src, size_t Size)
{
    tMdbgSlot* s = MDBG_l_Find(Src);
    void* newPtr;
    uint32_t old;

    if (!s)
    {
        MDBG_l_Report("realloc of a pointer that is not a live block:", Src);
        return MEM_p_Alloc((unsigned)Size);
    }
    if (s->kind == MDBG_KIND_ALIGNED)
        return MEM_p_ReallocAligned(Src, (unsigned)Size, 64);
    old = s->size;
    newPtr = MEM_p_Alloc((unsigned)Size);
    memcpy(newPtr, Src, old < Size ? old : Size);
    MEM_Free(Src);
    return newPtr;
}

void* MEM_p_AllocFromEnd(int size) { return MEM_p_Alloc((unsigned)size); }

/* The bin reader grows its buffer as FreeFromEnd(old), AllocFromEnd(new),
   memcpy(new, old): in the pool the new block covers the old one and its
   contents are still there. Keep the last block freed from the end alive
   until the next one is, so that copy reads live memory here too. */
static void* mdbg_pendingFromEnd;
void MEM_FreeFromEnd(void* ptr)
{
    if (mdbg_pendingFromEnd)
        MEM_Free(mdbg_pendingFromEnd);
    mdbg_pendingFromEnd = ptr;
}

int MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(void* ptr) { return (int)MDBG_l_SizeOf(ptr); }
int MEM_MemBlockSizeGetAligned(void* ptr) { return (int)MDBG_l_SizeOf(ptr); }
int MEM_MemBlockSizeGetTmpAddressSpace(void* ptr) { return (int)MDBG_l_SizeOf(ptr); }
int MEM_GetBlockSizeBySize(void* ptr) { return (int)MDBG_l_SizeOf(ptr); }

BOOL MEM_b_CheckBlockHasBeenFreed(void* ptr)
{
    return MDBG_l_Find(ptr) == NULL;
}
