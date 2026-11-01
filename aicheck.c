#include "aicheck.h"

#if AICHK_ENABLED

#include "common.h"
#include "ai.h"
#include "dbg.h"

/* The pairing the loader knows and the dispatcher does not. Only
   watched keys are stored, so this stays small however many
   behaviours a world carries. */
#define AICHK_SLOTS 64

static struct
{
    struct sAIFunction* fn;
    unsigned int        key;
    unsigned int        runs;
} AICHK_gast_Watched[AICHK_SLOTS];

static int          AICHK_gi_Used;
static int          AICHK_gi_Announced;

static int AICHK_IsWatched(unsigned int key)
{
    static const unsigned int watch[] = AICHK_WATCH;
    int i;

    for (i = 0; watch[i]; i++)
        if (watch[i] == key)
            return 1;
    return 0;
}

/* Called once per function as the loader builds it, with the key it
   was loaded under. */
void AICHK_NoteFunction(struct sAIFunction* fn, unsigned int key)
{
    if (!fn || !AICHK_IsWatched(key))
        return;

    if (AICHK_gi_Used >= AICHK_SLOTS)
    {
        DBG_OutputDebugString("AICHK: no room to watch %08x\n", key);
        return;
    }

    AICHK_gast_Watched[AICHK_gi_Used].fn   = fn;
    AICHK_gast_Watched[AICHK_gi_Used].key  = key;
    AICHK_gast_Watched[AICHK_gi_Used].runs = 0;
    ++AICHK_gi_Used;

    DBG_OutputDebugString("AICHK: watching %08x, loaded as function %p"
                          " (nodes %p, compiled %p)\n",
                          key, (void*)fn,
                          (void*)((tAIFunction*)fn)->nodes,
                          (void*)((tAIFunction*)fn)->funcPtr);
}

/* Called from the dispatcher. Reports the first few runs of each
   watched behaviour, then goes quiet and says so. */
void AICHK_Running(struct sAIFunction* fn)
{
    int i;

    if (!fn)
        return;

    if (!AICHK_gi_Announced)
    {
        AICHK_gi_Announced = 1;
        if (!AICHK_gi_Used)
            DBG_OutputDebugString("AICHK: none of the watched keys was"
                                  " loaded in this world -- the object"
                                  " carrying it is not here, or the key"
                                  " is wrong\n");
    }

    for (i = 0; i < AICHK_gi_Used; i++)
    {
        if (AICHK_gast_Watched[i].fn != fn)
            continue;

        ++AICHK_gast_Watched[i].runs;

        if (AICHK_MAX_PER_KEY
            && AICHK_gast_Watched[i].runs > AICHK_MAX_PER_KEY)
            return;

        DBG_OutputDebugString("AICHK: %08x is running (run %u)%s\n",
                              AICHK_gast_Watched[i].key,
                              AICHK_gast_Watched[i].runs,
                              (AICHK_MAX_PER_KEY
                               && AICHK_gast_Watched[i].runs
                                  == AICHK_MAX_PER_KEY)
                                ? " -- last report for this key" : "");
        return;
    }
}

#endif  /* AICHK_ENABLED */
