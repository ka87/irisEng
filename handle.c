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

/* Index 0 is never issued, so that no handle is 0. */
#define HDL_FREE_END 0u

static HDL_Slot* hdl_slots;
static uint32_t hdl_slotNum;          /* allocated, including index 0 */
static uint32_t hdl_slotUsed = 1;     /* next never-used index */
static uint32_t hdl_freeHead = HDL_FREE_END;
static int hdl_live;

/* Address to slot index, so that one address always gets one handle.
   Open addressing: an empty bucket is 0 and a released one is the
   tombstone, which keeps probe chains intact. */
#define HDL_TOMBSTONE ((void*)(uintptr_t)1)

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

static uint32_t hdl_hash(void* addr)
{
    /* The low bits of an address carry little, so they are mixed up
       before the bucket is chosen. */
    uint64_t v = (uint64_t)(uintptr_t)addr;
    v ^= v >> 33;
    v *= 0xff51afd7ed558ccdULL;
    v ^= v >> 33;
    v *= 0xc4ceb9fe1a85ec53ULL;
    v ^= v >> 33;
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
    uint32_t newNum = hdl_bucketNum ? hdl_bucketNum : 256u;
    uint32_t i;

    while (newNum < want * 2u)
        newNum *= 2u;

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
    uint32_t newNum = hdl_slotNum ? hdl_slotNum * 2u : 256u;
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
    return v <= 2u || (v & 3u) != 0u;
}

uint32_t HDL_ul_Handle(void* ptr)
{
    HDL_Bucket* b;
    uint32_t idx;
    HDL_Slot* slot;

    if (!ptr)
        return 0;
    /* The main-actor stand-ins arrive here looking like addresses. */
    if ((uintptr_t)ptr <= 2u)
        return (uint32_t)(uintptr_t)ptr;

    if (!hdl_bucketNum && !hdl_bucketsGrow(1))
        return 0;

    b = hdl_find(ptr);
    if (b && b->addr == ptr)
        return (b->idx << HDL_IDX_SHIFT)
             | ((hdl_slots[b->idx].gen & HDL_GEN_MASK) << HDL_GEN_SHIFT);

    /* Grow before inserting, at half full, so that a probe never has
       to walk a nearly full table. */
    if ((hdl_bucketFill + 1u) * 2u > hdl_bucketNum)
    {
        if (!hdl_bucketsGrow(hdl_bucketFill + 1u))
            return 0;
        b = hdl_find(ptr);
    }
    if (!b)
        return 0;

    if (hdl_freeHead != HDL_FREE_END)
    {
        idx = hdl_freeHead;
        hdl_freeHead = hdl_slots[idx].nextFree;
    }
    else
    {
        if (hdl_slotUsed >= hdl_slotNum && !hdl_slotsGrow())
            return 0;
        if (hdl_slotUsed >= HDL_MAX_SLOTS)
            return 0;
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

    /* Not a handle this table ever issued. */
    if (idx == 0u || idx >= hdl_slotUsed)
        return (void*)(uintptr_t)handle;

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

    if (!ptr || (uintptr_t)ptr <= 2u)
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
    hdl_slotUsed = 1;
    hdl_freeHead = HDL_FREE_END;
    hdl_bucketNum = 0;
    hdl_bucketFill = 0;
    hdl_live = 0;
}

#endif /* HDL_TRANSLATES */
