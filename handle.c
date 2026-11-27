#include "common.h"
#include "handle.h"

#if HDL_TRANSLATES

/* One slot per address that has a handle. An empty slot has no
   address; its generation is kept so that a handle issued before it
   was released never matches again. */
typedef struct HDL_Slot
{
    void* addr;
    uint32_t gen;
    uint32_t nextFree;
} HDL_Slot;

/* The free list's end marker. Index 0 is never a real slot, so it can
   stand for "no next". */
#define HDL_FREE_END 0u

/* How big the two tables start, and by how much they grow. */
#define HDL_SLOTS_INITIAL   256u
#define HDL_BUCKETS_INITIAL 256u
#define HDL_GROW_FACTOR     2u

/* The bucket array is grown before it is ever more than this full, so
   that a probe never has to walk a crowded table: one half. */
#define HDL_BUCKET_LOAD_NUM 1u
#define HDL_BUCKET_LOAD_DEN 2u

static HDL_Slot* hdl_slots;
static uint32_t hdl_slotNum;          /* allocated, including index 0 */
static uint32_t hdl_slotUsed = HDL_FIRST_INDEX;  /* next never-used */
static uint32_t hdl_freeHead = HDL_FREE_END;
static int hdl_live;

/* Address to slot index, so that one address always gets one handle.
   Open addressing: an empty bucket is NULL and a released one holds
   the tombstone, which keeps probe chains intact. The tombstone only
   has to be a value no real address can take, which is what makes one
   of the reserved values the obvious choice. */
#define HDL_TOMBSTONE ((void*)(uintptr_t)HDL_MAIN_ACTOR_FIRST)

typedef struct HDL_Bucket
{
    void* addr;
    uint32_t idx;
} HDL_Bucket;

static HDL_Bucket* hdl_buckets;
static uint32_t hdl_bucketNum;        /* a power of two, or 0 */
static uint32_t hdl_bucketFill;       /* live plus tombstones */

/* Allocated with the C library rather than the engine's own allocator,
   so that the allocator is free to use handles itself. */

/* A standard 64-bit avalanche step: two odd multipliers with a shift
   and an exclusive-or around each. The numbers are the published ones
   and are not a tuning choice -- what they are for is spreading the
   high bits of an address down into the low ones, because the low bits
   of an allocation carry almost nothing and are what picks a bucket. */
#define HDL_MIX_SHIFT 33
#define HDL_MIX_ODD_A 0xff51afd7ed558ccdULL
#define HDL_MIX_ODD_B 0xc4ceb9fe1a85ec53ULL

static uint32_t hdl_hash(void* addr)
{
    uint64_t v = (uint64_t)(uintptr_t)addr;
    v ^= v >> HDL_MIX_SHIFT;
    v *= HDL_MIX_ODD_A;
    v ^= v >> HDL_MIX_SHIFT;
    v *= HDL_MIX_ODD_B;
    v ^= v >> HDL_MIX_SHIFT;
    return (uint32_t)v;
}

static int hdl_bucketsGrow(uint32_t want);

/* Where this address sits, or where it would go. Returns NULL only if
   there is no table. */
static HDL_Bucket* hdl_find(void* addr)
{
    uint32_t mask, i, step;
    HDL_Bucket* firstFree = NULL;

    if (!hdl_bucketNum)
        return NULL;

    mask = hdl_bucketNum - 1u;
    i = hdl_hash(addr) & mask;
    for (step = 0; step <= mask; ++step)
    {
        HDL_Bucket* b = &hdl_buckets[i];
        if (!b->addr)
            return firstFree ? firstFree : b;
        if (b->addr == HDL_TOMBSTONE)
        {
            /* Keep going -- the address may still be further along the
               chain -- but remember this for the insert. */
            if (!firstFree)
                firstFree = b;
        }
        else if (b->addr == addr)
        {
            return b;
        }
        i = (i + 1u) & mask;
    }
    return firstFree;
}

static int hdl_bucketsGrow(uint32_t want)
{
    HDL_Bucket* old = hdl_buckets;
    uint32_t oldNum = hdl_bucketNum;
    uint32_t newNum = hdl_bucketNum ? hdl_bucketNum : HDL_BUCKETS_INITIAL;
    uint32_t i;

    while (newNum * HDL_BUCKET_LOAD_NUM < want * HDL_BUCKET_LOAD_DEN)
        newNum *= HDL_GROW_FACTOR;

    hdl_buckets = (HDL_Bucket*)calloc(newNum, sizeof(HDL_Bucket));
    if (!hdl_buckets)
    {
        hdl_buckets = old;
        return 0;
    }
    hdl_bucketNum = newNum;
    hdl_bucketFill = 0;

    /* Rehashing drops the tombstones, which is the only thing that
       ever shortens a probe chain. */
    for (i = 0; i < oldNum; ++i)
    {
        HDL_Bucket* b;
        if (!old[i].addr || old[i].addr == HDL_TOMBSTONE)
            continue;
        b = hdl_find(old[i].addr);
        b->addr = old[i].addr;
        b->idx = old[i].idx;
        ++hdl_bucketFill;
    }
    free(old);
    return 1;
}

static int hdl_slotsGrow(void)
{
    uint32_t newNum = hdl_slotNum ? hdl_slotNum * HDL_GROW_FACTOR
                                  : HDL_SLOTS_INITIAL;
    HDL_Slot* grown;

    if (newNum > HDL_MAX_SLOTS)
        newNum = HDL_MAX_SLOTS;
    if (newNum == hdl_slotNum)
        return 0;

    grown = (HDL_Slot*)realloc(hdl_slots, newNum * sizeof(HDL_Slot));
    if (!grown)
        return 0;
    memset(&grown[hdl_slotNum], 0, (newNum - hdl_slotNum) * sizeof(HDL_Slot));
    hdl_slots = grown;
    hdl_slotNum = newNum;
    return 1;
}

/* True for the values a slot may hold that are not addresses: the four
   reserved ones, and anything carrying the unresolved-key tag. */
static int hdl_isReserved(uint32_t v)
{
    return v <= HDL_MAIN_ACTOR_LAST
        || (v & HDL_TAG_MASK) != HDL_TAG_RESOLVED;
}

uint32_t HDL_ul_Handle(void* ptr)
{
    HDL_Bucket* b;
    uint32_t idx;
    HDL_Slot* slot;

    if (!ptr)
        return HDL_NO_OBJECT;
    /* The main-actor stand-ins arrive here looking like addresses. */
    if ((uintptr_t)ptr <= HDL_MAIN_ACTOR_LAST)
        return (uint32_t)(uintptr_t)ptr;

    if (!hdl_bucketNum && !hdl_bucketsGrow(1u))
        return HDL_NO_OBJECT;

    b = hdl_find(ptr);
    if (b && b->addr == ptr)
        return (b->idx << HDL_IDX_SHIFT)
             | ((hdl_slots[b->idx].gen & HDL_GEN_MASK) << HDL_GEN_SHIFT);

    /* Grow before inserting, at half full, so that a probe never has
       to walk a nearly full table. */
    if ((hdl_bucketFill + 1u) * HDL_BUCKET_LOAD_DEN
            > hdl_bucketNum * HDL_BUCKET_LOAD_NUM)
    {
        if (!hdl_bucketsGrow(hdl_bucketFill + 1u))
            return HDL_NO_OBJECT;
        b = hdl_find(ptr);
    }
    if (!b)
        return HDL_NO_OBJECT;

    if (hdl_freeHead != HDL_FREE_END)
    {
        idx = hdl_freeHead;
        hdl_freeHead = hdl_slots[idx].nextFree;
    }
    else
    {
        if (hdl_slotUsed >= hdl_slotNum && !hdl_slotsGrow())
            return HDL_NO_OBJECT;
        if (hdl_slotUsed >= HDL_MAX_SLOTS)
            return HDL_NO_OBJECT;
        idx = hdl_slotUsed++;
    }

    slot = &hdl_slots[idx];
    slot->addr = ptr;
    slot->nextFree = HDL_FREE_END;

    if (!b->addr || b->addr == HDL_TOMBSTONE)
    {
        if (!b->addr)
            ++hdl_bucketFill;      /* a tombstone was already counted */
        b->addr = ptr;
    }
    b->idx = idx;
    ++hdl_live;

    return (idx << HDL_IDX_SHIFT)
         | ((slot->gen & HDL_GEN_MASK) << HDL_GEN_SHIFT);
}

/* How many unissued references to report before going quiet. */
#define HDL_UNISSUED_SAY 8
static unsigned int hdl_unissuedSaid;
static HDL_ReportFn hdl_report;

void HDL_SetReporter(HDL_ReportFn fn)
{
    hdl_report = fn;
}

BOOL HDL_b_Issued(uint32_t handle)
{
    uint32_t idx = (handle >> HDL_IDX_SHIFT) & HDL_IDX_MASK;
    uint32_t gen = (handle >> HDL_GEN_SHIFT) & HDL_GEN_MASK;

    if (hdl_isReserved(handle))
        return FALSE;
    if (idx < HDL_FIRST_INDEX || idx >= hdl_slotUsed)
        return FALSE;
    return hdl_slots[idx].addr != NULL && hdl_slots[idx].gen == gen;
}

void* HDL_p_Pointer(uint32_t handle)
{
    uint32_t idx, gen;
    HDL_Slot* slot;

    /* A reserved value or a tagged slot is handed straight back, so
       that the caller's own test of it still sees what it expects. */
    if (hdl_isReserved(handle))
        return (void*)(uintptr_t)handle;

    idx = (handle >> HDL_IDX_SHIFT) & HDL_IDX_MASK;
    gen = (handle >> HDL_GEN_SHIFT) & HDL_GEN_MASK;

    /* Not a handle this table ever issued. The value is handed back
       because a slot may legitimately hold something that is not a
       reference at all -- but a caller that treats it as one is about
       to dereference a number, and the fault will land somewhere with
       nothing to say about where the number came from. So say it here,
       where the value is still in hand.

       The first few only: this is reached by design for a tagged slot
       on a path that does not check the tag first, and a line per call
       would bury the one that matters. */
    if (idx < HDL_FIRST_INDEX || idx >= hdl_slotUsed)
    {
        if (hdl_report && hdl_unissuedSaid < HDL_UNISSUED_SAY)
        {
            char line[192];

            ++hdl_unissuedSaid;
            snprintf(line, sizeof(line),
                     "handle: %#x reads as a reference and was never issued "
                     "(index %u, generation %u, %u issued). Whatever holds "
                     "it is not a reference, or it was encoded twice.\n",
                     handle, idx, gen, hdl_slotUsed);
            hdl_report(line);
        }
        return (void*)(uintptr_t)handle;
    }

    slot = &hdl_slots[idx];
    if (!slot->addr)
        return NULL;                       /* released */
    if ((slot->gen & HDL_GEN_MASK) != gen)
        return NULL;                       /* released and reused */
    return slot->addr;
}

void HDL_Release(void* ptr)
{
    HDL_Bucket* b;
    HDL_Slot* slot;
    uint32_t idx;

    if (!ptr || (uintptr_t)ptr <= HDL_MAIN_ACTOR_LAST)
        return;

    b = hdl_find(ptr);
    if (!b || b->addr != ptr)
        return;

    idx = b->idx;
    b->addr = HDL_TOMBSTONE;               /* still counted in the fill */

    slot = &hdl_slots[idx];
    slot->addr = NULL;
    /* Wrapping is what the generation is for: eight bits means a slot
       has to be reused 256 times before an old handle can match
       again, and the handle is only live while its object is. */
    slot->gen = (slot->gen + 1u) & HDL_GEN_MASK;
    slot->nextFree = hdl_freeHead;
    hdl_freeHead = idx;
    --hdl_live;
}

int HDL_i_LiveCount(void)
{
    return hdl_live;
}

void HDL_CloseModule(void)
{
    free(hdl_slots);
    free(hdl_buckets);
    hdl_slots = NULL;
    hdl_buckets = NULL;
    hdl_slotNum = 0;
    hdl_slotUsed = HDL_FIRST_INDEX;
    hdl_freeHead = HDL_FREE_END;
    hdl_bucketNum = 0;
    hdl_bucketFill = 0;
    hdl_live = 0;
}

#endif /* HDL_TRANSLATES */
