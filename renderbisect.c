#include "renderbisect.h"
#include "gameobject.h"
#include "dbg.h"

#if RBIS_ENABLED

/* The keys named in renderbisect_keys.h, pulled in twice: once to size
   the table and once to fill it. */
#define RBIS_KEY(k) +1
static const int RBIS_LISTED_COUNT = 0
#include "renderbisect_keys.h"
;
#undef RBIS_KEY

#define RBIS_KEY(k) (unsigned int)(k),
static const unsigned int RBIS_gaul_Listed[] = {
#include "renderbisect_keys.h"
    0u
};
#undef RBIS_KEY

/* Gao pointers whose key was on the list, filled as the world loads.
   Kept sorted so the draw walk can binary search it. */
static struct sGameObjectMain* RBIS_gapst_Matched[1024];
static int RBIS_gi_MatchCount;

static int RBIS_KeyIsListed(unsigned int key)
{
    int i;
    for (i = 0; i < RBIS_LISTED_COUNT; i++)
    {
        if (RBIS_gaul_Listed[i] == key)
            return 1;
    }
    return 0;
}

/* Insert in order, ignoring repeats; a key can be resolved many times. */
static void RBIS_Insert(struct sGameObjectMain* gao)
{
    int lo = 0;
    int hi = RBIS_gi_MatchCount;
    int i;

    while (lo < hi)
    {
        const int mid = (lo + hi) >> 1;
        if (RBIS_gapst_Matched[mid] < gao)
            lo = mid + 1;
        else
            hi = mid;
    }

    if (lo < RBIS_gi_MatchCount && RBIS_gapst_Matched[lo] == gao)
        return;
    if (RBIS_gi_MatchCount >= (int)(sizeof(RBIS_gapst_Matched) / sizeof(RBIS_gapst_Matched[0])))
        return;

    for (i = RBIS_gi_MatchCount; i > lo; i--)
        RBIS_gapst_Matched[i] = RBIS_gapst_Matched[i - 1];
    RBIS_gapst_Matched[lo] = gao;
    ++RBIS_gi_MatchCount;
}

/* Called from the loader once a key has produced a game object. */
void RBIS_NoteGao(unsigned int key, struct sGameObjectMain* gao)
{
    if (gao && RBIS_KeyIsListed(key))
        RBIS_Insert(gao);
}

void RBIS_Report(void)
{
    DBG_OutputDebugString("RBIS: %d keys listed, %d game objects matched, listed are %s\n",
               RBIS_LISTED_COUNT, RBIS_gi_MatchCount,
               RBIS_LISTED_ARE_HIDDEN ? "hidden" : "the only ones shown");
}

int RBIS_IsHidden(struct sGameObjectMain* gao)
{
    int lo = 0;
    int hi = RBIS_gi_MatchCount;
    int matched = 0;

    while (lo < hi)
    {
        const int mid = (lo + hi) >> 1;
        if (RBIS_gapst_Matched[mid] < gao)
            lo = mid + 1;
        else
            hi = mid;
    }
    matched = (lo < RBIS_gi_MatchCount && RBIS_gapst_Matched[lo] == gao);

    return RBIS_LISTED_ARE_HIDDEN ? matched : !matched;
}

#endif
