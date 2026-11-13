#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ways_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../ways.h"

/* The frozen copy and the rewrite in one binary. Four of ways.c's
   functions are not in its header, so they are declared here the way
   both copies define them. */
void BASE_WAY_FreeStruct(struct sWay* way);
void NEW_WAY_FreeStruct(struct sWay* way);
void BASE_WAY_ReinitLinks(struct sWay* way);
void NEW_WAY_ReinitLinks(struct sWay* way);
void BASE_WAY_RemoveAllNetworks(tWayAllNetworks* all);
void NEW_WAY_RemoveAllNetworks(tWayAllNetworks* all);
tWayAllNetworks* BASE_WAY_ul_AllNetWorksCallback(uint32_t offset);
tWayAllNetworks* NEW_WAY_ul_AllNetWorksCallback(uint32_t offset);
tWayNetwork* BASE_WAY_ul_NetWorkCallback(uint32_t offset);
tWayNetwork* NEW_WAY_ul_NetWorkCallback(uint32_t offset);
struct sWay* BASE_WAY_ul_AllLinkListsCallback(uint32_t offset);
struct sWay* NEW_WAY_ul_AllLinkListsCallback(uint32_t offset);
struct sWayElem* BASE_WAY_pst_SearchLinkList(struct sGameObjectMain* wp, struct sWayNetwork* net);
struct sWayElem* NEW_WAY_pst_SearchLinkList(struct sGameObjectMain* wp, struct sWayNetwork* net);
struct sWayElemInnerElem* BASE_WAY_pst_SearchOneLinkInList(struct sWayElem* l, struct sGameObjectMain* wp);
struct sWayElemInnerElem* NEW_WAY_pst_SearchOneLinkInList(struct sWayElem* l, struct sGameObjectMain* wp);
void BASE_WAY_ComputePoundNetworksRec(tWayNetwork* n, tGameObjectMain* g, tGameObjectMain* s);
void NEW_WAY_ComputePoundNetworksRec(tWayNetwork* n, tGameObjectMain* g, tGameObjectMain* s);
void BASE_WAY_ComputePoundOneNetworkForObj(tWayNetwork* n, tGameObjectMain* s);
void NEW_WAY_ComputePoundOneNetworkForObj(tWayNetwork* n, tGameObjectMain* s);
void BASE_WAY_ComputePoundNetworks(struct sWorld* w);
void NEW_WAY_ComputePoundNetworks(struct sWorld* w);
void BASE_WAY_InitModule(void);
void NEW_WAY_InitModule(void);
void BASE_WAY_CloseModule(void);
void NEW_WAY_CloseModule(void);

extern BASStruct BASE_WAY_gst_Seen;
extern BASStruct NEW_WAY_gst_Seen;
extern struct sWorld* BASE_gpst_CurrentWorld;
extern struct sWorld* NEW_gpst_CurrentWorld;

/* ------------------------------------------------------------------ */

static int s_cases;
static int s_fails;

#define CAPTURE_MAX (1 << 20)

typedef struct
{
    long ret;
    int logLen;
    tWMockCall log[WMOCK_LOG_MAX];
    size_t used;
    unsigned char bytes[CAPTURE_MAX];
    int seenNum;
    int seenMax;
    long seenKeys[2048];
    long seenVals[2048];
    long currentWorld;
    int sceneLen;
    unsigned char scene[CAPTURE_MAX];
} tCapture;

static tCapture s_base;
static tCapture s_new;

/* The scene the by-hand cases are built into. Both sides run over the
   same addresses with it rebuilt in between, so a memcmp of it is a
   comparison of every field either copy could have touched. */
#define SCENE_SIZE (1 << 18)
static unsigned char s_scene[SCENE_SIZE];
static size_t s_sceneUsed;

static void* sceneAlloc(size_t n)
{
    void* p;

    s_sceneUsed = (s_sceneUsed + 15) & ~(size_t)15;
    if (s_sceneUsed + n > SCENE_SIZE)
    {
        fprintf(stderr, "scene exhausted\n");
        exit(2);
    }
    p = s_scene + s_sceneUsed;
    s_sceneUsed += n;
    return p;
}

static void sceneReset(void)
{
    memset(s_scene, 0, sizeof(s_scene));
    s_sceneUsed = 0;
}

static void capture(tCapture* c, long ret, int side)
{
    BASStruct* bas = side ? &NEW_WAY_gst_Seen : &BASE_WAY_gst_Seen;
    int i;

    c->ret = ret;
    c->logLen = WMOCK_logLen;
    memcpy(c->log, WMOCK_log, sizeof(c->log));
    c->used = WMOCK_used;
    memcpy(c->bytes, WMOCK_arena, WMOCK_used < CAPTURE_MAX ? WMOCK_used : CAPTURE_MAX);
    c->seenNum = bas->actElemNum;
    c->seenMax = bas->maxElemNum;
    for (i = 0; i < bas->actElemNum && i < 2048; ++i)
    {
        c->seenKeys[i] = bas->keyValPtr[i].key;
        c->seenVals[i] = bas->keyValPtr[i].val;
    }
    c->currentWorld = side ? (long)(intptr_t)NEW_gpst_CurrentWorld
                           : (long)(intptr_t)BASE_gpst_CurrentWorld;
    c->sceneLen = (int)s_sceneUsed;
    memcpy(c->scene, s_scene, s_sceneUsed);
}

static void compare(const char* what, int k)
{
    int i;
    int bad = 0;

    ++s_cases;
    if (s_base.ret != s_new.ret)
    {
        printf("  %s[%d]: return %ld vs %ld\n", what, k, s_base.ret, s_new.ret);
        bad = 1;
    }
    if (s_base.logLen != s_new.logLen)
    {
        printf("  %s[%d]: call count %d vs %d\n", what, k, s_base.logLen, s_new.logLen);
        bad = 1;
    }
    else
    {
        for (i = 0; i < s_base.logLen && i < WMOCK_LOG_MAX; ++i)
        {
            if (strcmp(s_base.log[i].name, s_new.log[i].name) != 0
                || s_base.log[i].a != s_new.log[i].a
                || s_base.log[i].b != s_new.log[i].b
                || s_base.log[i].c != s_new.log[i].c)
            {
                printf("  %s[%d]: call %d  %s(%ld,%ld,%ld) vs %s(%ld,%ld,%ld)\n",
                       what, k, i,
                       s_base.log[i].name, s_base.log[i].a, s_base.log[i].b, s_base.log[i].c,
                       s_new.log[i].name, s_new.log[i].a, s_new.log[i].b, s_new.log[i].c);
                bad = 1;
                break;
            }
        }
    }
    if (s_base.used != s_new.used)
    {
        printf("  %s[%d]: arena used %lu vs %lu\n", what, k,
               (unsigned long)s_base.used, (unsigned long)s_new.used);
        bad = 1;
    }
    else if (memcmp(s_base.bytes, s_new.bytes,
                    s_base.used < CAPTURE_MAX ? s_base.used : CAPTURE_MAX) != 0)
    {
        printf("  %s[%d]: arena bytes differ\n", what, k);
        bad = 1;
    }
    if (s_base.sceneLen != s_new.sceneLen
        || memcmp(s_base.scene, s_new.scene, s_base.sceneLen) != 0)
    {
        printf("  %s[%d]: scene bytes differ\n", what, k);
        bad = 1;
    }
    if (s_base.seenNum != s_new.seenNum)
    {
        printf("  %s[%d]: seen count %d vs %d\n", what, k, s_base.seenNum, s_new.seenNum);
        bad = 1;
    }
    else
    {
        for (i = 0; i < s_base.seenNum && i < 2048; ++i)
        {
            if (s_base.seenKeys[i] != s_new.seenKeys[i] || s_base.seenVals[i] != s_new.seenVals[i])
            {
                printf("  %s[%d]: seen entry %d differs\n", what, k, i);
                bad = 1;
                break;
            }
        }
    }
    if (s_base.currentWorld != s_new.currentWorld)
    {
        printf("  %s[%d]: gpst_CurrentWorld %ld vs %ld\n", what, k,
               s_base.currentWorld, s_new.currentWorld);
        bad = 1;
    }
    if (bad)
        ++s_fails;
}

/* ------------------------------------------------------------------ */
/* The byte stream the loader callbacks read.                          */

static int s_put;

static void putLong(int v)
{
    memcpy(WMOCK_stream + s_put, &v, 4);
    s_put += 4;
}

static void putShort(int v)
{
    short s = (short)v;
    memcpy(WMOCK_stream + s_put, &s, 2);
    s_put += 2;
}

static void putChar(int v)
{
    WMOCK_stream[s_put++] = (unsigned char)v;
}

/* A network record: the root object's key, then the flag word. */
static void buildNetworkStream(int rootKey, int flags)
{
    s_put = 0;
    putLong(rootKey);
    putLong(flags);
    WMOCK_streamSize = s_put;
}

static void buildAllNetworksStream(int num, int seed)
{
    int i;

    s_put = 0;
    putLong(num);
    for (i = 0; i < num; ++i)
        putLong(seed * 100 + i);
    WMOCK_streamSize = s_put;
}

/* elemNum records, each a network key, a link count and that many
   link records of { capa, gao key, design, s3 }. */
static void buildLinkListStream(int elemNum, int linksPerElem, int seed)
{
    int i;
    int j;

    s_put = 0;
    putLong(elemNum);
    for (i = 0; i < elemNum; ++i)
    {
        putLong(seed * 1000 + i);
        putLong(linksPerElem + (i & 1));
        for (j = 0; j < linksPerElem + (i & 1); ++j)
        {
            putShort(((seed + i + j) * 37) & 0xFFFF);
            putLong(seed * 10000 + i * 100 + j);
            putChar((seed + i * 3 + j) & 0xFF);
            putShort((seed * 7 + i + j) & 0x7FFF);
        }
    }
    WMOCK_streamSize = s_put;
}

/* ------------------------------------------------------------------ */
/* Hand-built waypoint graphs.                                         */

typedef struct
{
    tGameObjectMain* gao[8];
    tWayNetwork* net[2];
    tWay* way[8];
    struct sWorld* world;
    tWayAllNetworks* all;
} tGraph;

static float fseed(int k)
{
    /* A spread of values with bits in the low mantissa, so a change in
       the summation order would show. */
    return (float)((k * 7919 % 2003) - 1000) * 0.3137f + 0.125f;
}

/* nodes waypoints in a ring with one chord, every link pointing at the
   next; linkNull leaves one link's object NULL, netSplit puts half the
   waypoints on the second network. */
static void buildGraph(tGraph* g, int nodes, int linkNull, int netSplit, int seed)
{
    int i;
    int j;
    int n;

    memset(g, 0, sizeof(*g));

    for (i = 0; i < 2; ++i)
    {
        g->net[i] = (tWayNetwork*)sceneAlloc(sizeof(tWayNetwork));
        g->net[i]->flags = i;
    }

    for (i = 0; i < nodes; ++i)
    {
        g->gao[i] = (tGameObjectMain*)sceneAlloc(sizeof(tGameObjectMain));
        g->gao[i]->matrices = (Mtx4x4*)sceneAlloc(sizeof(Mtx4x4));
        g->gao[i]->matrices->Pos.x = fseed(seed * 31 + i * 3 + 0);
        g->gao[i]->matrices->Pos.y = fseed(seed * 31 + i * 3 + 1);
        g->gao[i]->matrices->Pos.z = fseed(seed * 31 + i * 3 + 2);
        g->gao[i]->extended = (tGameObjectExtended*)sceneAlloc(sizeof(tGameObjectExtended));
    }
    g->net[0]->gao = g->gao[0];
    g->net[1]->gao = nodes > 1 ? g->gao[1] : g->gao[0];

    for (i = 0; i < nodes; ++i)
    {
        g->way[i] = (tWay*)sceneAlloc(sizeof(tWay));
        g->way[i]->wayElemNum = 2;
        g->way[i]->wayElems = (tWayElem*)sceneAlloc(2 * sizeof(tWayElem));
        for (j = 0; j < 2; ++j)
        {
            n = (netSplit && (i & 1)) ? 1 - j : j;
            g->way[i]->wayElems[j].network = g->net[n];
            g->way[i]->wayElems[j].num = (j == 0) ? 2 : 1;
            g->way[i]->wayElems[j].innerElems =
                (tWayElemInnerElem*)sceneAlloc(2 * sizeof(tWayElemInnerElem));
            g->way[i]->wayElems[j].innerElems[0].gao = g->gao[(i + 1) % nodes];
            g->way[i]->wayElems[j].innerElems[0].capa = (int16_t)(i + 1);
            g->way[i]->wayElems[j].innerElems[0].capaBackup = (int16_t)(100 + i);
            g->way[i]->wayElems[j].innerElems[0].design = (uint8_t)(i + 7);
            g->way[i]->wayElems[j].innerElems[0].designBackup = (uint8_t)(i + 200);
            g->way[i]->wayElems[j].innerElems[0].weight = -1.0f;
            g->way[i]->wayElems[j].innerElems[1].gao =
                (linkNull && i == 1) ? 0 : g->gao[(i + 2) % nodes];
            g->way[i]->wayElems[j].innerElems[1].capa = (int16_t)(i + 50);
            g->way[i]->wayElems[j].innerElems[1].capaBackup = (int16_t)(300 + i);
            g->way[i]->wayElems[j].innerElems[1].design = (uint8_t)(i + 9);
            g->way[i]->wayElems[j].innerElems[1].designBackup = (uint8_t)(i + 100);
            g->way[i]->wayElems[j].innerElems[1].weight = -2.0f;
        }
        g->gao[i]->extended->way = g->way[i];
    }

    g->all = (tWayAllNetworks*)sceneAlloc(sizeof(tWayAllNetworks));
    g->all->networkNum = 2;
    g->all->networks = (tWayNetwork**)sceneAlloc(2 * sizeof(tWayNetwork*));
    g->all->networks[0] = g->net[0];
    g->all->networks[1] = g->net[1];
    g->world = (struct sWorld*)sceneAlloc(sizeof(struct sWorld));
    g->world->wayPtr = g->all;
}

/* ------------------------------------------------------------------ */

static void runLoaderCases(void)
{
    int k;
    int side;
    long ret;

    printf("loader callbacks\n");

    /* WAY_ul_NetWorkCallback: no root, the -1 sentinel, and real keys. */
    for (k = 0; k < 6; ++k)
    {
        static const int roots[6] = { 0, -1, 1, 7, 0x7FFFFFFF, 12345 };
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            WMOCK_returnNullStream = 0;
            buildNetworkStream(roots[k], k * 11 + 3);
            ret = WMOCK_Offset(side ? (void*)NEW_WAY_ul_NetWorkCallback(0)
                                    : (void*)BASE_WAY_ul_NetWorkCallback(0));
            capture(side ? &s_new : &s_base, ret, side);
        }
        compare("NetWorkCallback", k);
    }

    /* WAY_ul_AllNetWorksCallback, including the empty list and the
       null stream that makes it return NULL. */
    for (k = 0; k < 6; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            WMOCK_returnNullStream = (k == 5);
            buildAllNetworksStream(k == 5 ? 0 : k, k + 1);
            ret = WMOCK_Offset(side ? (void*)NEW_WAY_ul_AllNetWorksCallback(0)
                                    : (void*)BASE_WAY_ul_AllNetWorksCallback(0));
            capture(side ? &s_new : &s_base, ret, side);
        }
        compare("AllNetWorksCallback", k);
        WMOCK_returnNullStream = 0;
    }

    /* WAY_ul_AllLinkListsCallback over element and link counts,
       including the two zero cases. */
    for (k = 0; k < 20; ++k)
    {
        int elems = k % 5;
        int links = k / 5;
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            buildLinkListStream(elems, links, k + 2);
            ret = WMOCK_Offset(side ? (void*)NEW_WAY_ul_AllLinkListsCallback(0)
                                    : (void*)BASE_WAY_ul_AllLinkListsCallback(0));
            capture(side ? &s_new : &s_base, ret, side);
        }
        compare("AllLinkListsCallback", k);
    }
}

static void runFreeAndReinitCases(void)
{
    int k;
    int side;
    tWay* way;

    printf("free and reinit\n");

    /* Load a way through the real callback and free it again, so the
       free order is measured against a structure the file built. */
    for (k = 0; k < 20; ++k)
    {
        int elems = k % 5;
        int links = k / 5;
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            buildLinkListStream(elems, links, k + 3);
            way = side ? NEW_WAY_ul_AllLinkListsCallback(0) : BASE_WAY_ul_AllLinkListsCallback(0);
            if (side)
                NEW_WAY_FreeStruct(way);
            else
                BASE_WAY_FreeStruct(way);
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("FreeStruct", k);
    }

    /* And the null pointer, which must do nothing at all. */
    for (side = 0; side < 2; ++side)
    {
        WMOCK_Reset();
        sceneReset();
        if (side)
            NEW_WAY_FreeStruct(0);
        else
            BASE_WAY_FreeStruct(0);
        capture(side ? &s_new : &s_base, 0, side);
    }
    compare("FreeStruct", 100);

    /* ReinitLinks over hand-built graphs, where every link's capa and
       design must come back from its backup. */
    for (k = 0; k < 12; ++k)
    {
        tGraph g;
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            buildGraph(&g, 2 + k % 5, k & 1, (k >> 1) & 1, k + 4);
            if (side)
                NEW_WAY_ReinitLinks(g.way[0]);
            else
                BASE_WAY_ReinitLinks(g.way[0]);
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("ReinitLinks", k);
    }
    for (side = 0; side < 2; ++side)
    {
        WMOCK_Reset();
        sceneReset();
        if (side)
            NEW_WAY_ReinitLinks(0);
        else
            BASE_WAY_ReinitLinks(0);
        capture(side ? &s_new : &s_base, 0, side);
    }
    compare("ReinitLinks", 100);

    /* A way whose element list is empty, and one whose link list is. */
    for (k = 0; k < 2; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            way = (tWay*)sceneAlloc(sizeof(tWay));
            way->wayElemNum = k;
            way->wayElems = (tWayElem*)sceneAlloc(sizeof(tWayElem));
            way->wayElems[0].num = 0;
            way->wayElems[0].innerElems = 0;
            if (side)
                NEW_WAY_ReinitLinks(way);
            else
                BASE_WAY_ReinitLinks(way);
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("ReinitLinksEmpty", k);
    }
}

static void runRemoveAllCases(void)
{
    int k;
    int side;
    tWayAllNetworks* all;

    printf("remove all networks\n");

    for (k = 0; k < 8; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            int i;
            WMOCK_Reset();
            sceneReset();
            all = (tWayAllNetworks*)MEM_p_Alloc(sizeof(tWayAllNetworks));
            /* k == 7 has no network array at all. Its count has to be
               zero with it: the walk below indexes the array before the
               null test that follows it, so a count with no array
               faults in both copies. */
            all->networkNum = (k == 7) ? 0 : k % 4;
            all->networks = (k == 7) ? 0 : (tWayNetwork**)MEM_p_Alloc(8 * (unsigned)(k % 4 + 1));
            for (i = 0; i < all->networkNum && all->networks; ++i)
                all->networks[i] = (i == 1) ? 0 : (tWayNetwork*)MEM_p_Alloc(8u);
            WMOCK_logLen = 0;
            if (side)
                NEW_WAY_RemoveAllNetworks(all);
            else
                BASE_WAY_RemoveAllNetworks(all);
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("RemoveAllNetworks", k);
    }
    for (side = 0; side < 2; ++side)
    {
        WMOCK_Reset();
        sceneReset();
        if (side)
            NEW_WAY_RemoveAllNetworks(0);
        else
            BASE_WAY_RemoveAllNetworks(0);
        capture(side ? &s_new : &s_base, 0, side);
    }
    compare("RemoveAllNetworks", 100);
}

static void runSearchCases(void)
{
    int k;
    int side;
    long ret;
    tGraph g;

    printf("search\n");

    /* The link list of every waypoint against both networks, plus a
       network that is in neither. */
    for (k = 0; k < 40; ++k)
    {
        int nodes = 2 + k % 5;
        int node = k % nodes;
        int which = (k / 5) % 3;
        for (side = 0; side < 2; ++side)
        {
            tWayNetwork* net;
            tWayNetwork other;
            WMOCK_Reset();
            sceneReset();
            buildGraph(&g, nodes, k & 1, (k >> 2) & 1, k + 5);
            memset(&other, 0, sizeof(other));
            net = which == 2 ? &other : g.net[which];
            if (k % 7 == 3)
                g.gao[node]->extended->way = 0;
            if (k % 11 == 5)
                g.gao[node]->extended = 0;
            if (k % 13 == 7 && g.gao[node]->extended && g.gao[node]->extended->way)
                g.gao[node]->extended->way->wayElems = 0;
            if (k % 17 == 9 && g.gao[node]->extended && g.gao[node]->extended->way)
                g.gao[node]->extended->way->wayElemNum = 0;
            ret = (long)(intptr_t)(side ? NEW_WAY_pst_SearchLinkList(g.gao[node], net)
                                        : BASE_WAY_pst_SearchLinkList(g.gao[node], net));
            ret = ret ? ret - (long)(intptr_t)s_scene : -1;
            capture(side ? &s_new : &s_base, ret, side);
        }
        compare("SearchLinkList", k);
    }

    /* And one link in a list, found and not found, over list lengths
       that include zero. */
    for (k = 0; k < 30; ++k)
    {
        int nodes = 2 + k % 5;
        int target = k % (nodes + 1);
        for (side = 0; side < 2; ++side)
        {
            tWayElem* elem;
            tGameObjectMain miss;
            WMOCK_Reset();
            sceneReset();
            buildGraph(&g, nodes, 0, 0, k + 6);
            memset(&miss, 0, sizeof(miss));
            elem = &g.way[0]->wayElems[0];
            if (k % 6 == 4)
                elem->num = 0;
            ret = (long)(intptr_t)(side
                ? NEW_WAY_pst_SearchOneLinkInList(elem, target == nodes ? &miss : g.gao[target])
                : BASE_WAY_pst_SearchOneLinkInList(elem, target == nodes ? &miss : g.gao[target]));
            ret = ret ? ret - (long)(intptr_t)s_scene : -1;
            capture(side ? &s_new : &s_base, ret, side);
        }
        compare("SearchOneLinkInList", k);
    }
}

static void runPoundCases(void)
{
    int k;
    int side;
    tGraph g;

    printf("pound networks\n");

    /* The recursion on its own, with and without a stop object, over
       graphs that contain a cycle and a null link. */
    for (k = 0; k < 40; ++k)
    {
        int nodes = 2 + k % 5;
        int stop = k % (nodes + 1);
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            buildGraph(&g, nodes, (k >> 1) & 1, (k >> 2) & 1, k + 7);
            if (side)
            {
                NEW_WAY_gst_Seen.actElemNum = 0;
                NEW_WAY_ComputePoundNetworksRec(g.net[0], g.gao[0],
                                                stop == nodes ? 0 : g.gao[stop]);
            }
            else
            {
                BASE_WAY_gst_Seen.actElemNum = 0;
                BASE_WAY_ComputePoundNetworksRec(g.net[0], g.gao[0],
                                                 stop == nodes ? 0 : g.gao[stop]);
            }
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("ComputePoundNetworksRec", k);
    }

    /* A network whose root is NULL, which must do nothing but clear
       the seen set. */
    for (k = 0; k < 4; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            buildGraph(&g, 2 + k, 0, 0, k + 8);
            g.net[0]->gao = 0;
            if (side)
                NEW_WAY_ComputePoundOneNetworkForObj(g.net[0], 0);
            else
                BASE_WAY_ComputePoundOneNetworkForObj(g.net[0], 0);
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("ComputePoundOneNetworkForObj", k);
    }

    /* And the whole-world entry point, including a world with no way
       table and one with a NULL network in the list. */
    for (k = 0; k < 24; ++k)
    {
        int nodes = 2 + k % 5;
        for (side = 0; side < 2; ++side)
        {
            WMOCK_Reset();
            sceneReset();
            buildGraph(&g, nodes, (k >> 1) & 1, (k >> 2) & 1, k + 9);
            if (k % 6 == 3)
                g.world->wayPtr = 0;
            else if (k % 6 == 4)
                g.all->networks[0] = 0;
            else if (k % 6 == 5)
                g.all->networkNum = 0;
            if (side)
                NEW_WAY_ComputePoundNetworks(g.world);
            else
                BASE_WAY_ComputePoundNetworks(g.world);
            capture(side ? &s_new : &s_base, 0, side);
        }
        compare("ComputePoundNetworks", k);
    }
}

/* Two cases driven on purpose rather than by the sweep, each closing
   something the random graphs could not reach. They take a seed base
   above everything else in the file so an inserted case cannot shift
   them off the branch they exist for. */
static void runTargetedCases(void)
{
    int side;
    long ret;
    tGraph g;

    printf("targeted\n");

    /* A link list whose count stops one short of a real entry, with
       that entry's object as the thing being looked for. The walk must
       not see it; a bound that reads one past the count does. */
    for (side = 0; side < 2; ++side)
    {
        tWayElem* elem;
        WMOCK_Reset();
        sceneReset();
        buildGraph(&g, 3, 0, 0, 901);
        elem = &g.way[0]->wayElems[0];
        elem->num = 1;
        ret = (long)(intptr_t)(side ? NEW_WAY_pst_SearchOneLinkInList(elem, elem->innerElems[1].gao)
                                    : BASE_WAY_pst_SearchOneLinkInList(elem, elem->innerElems[1].gao));
        ret = ret ? ret - (long)(intptr_t)s_scene : -1;
        capture(side ? &s_new : &s_base, ret, side);
    }
    compare("SearchPastTheCount", 0);

    /* A pair of waypoints whose separation is small enough that the
       three squares do not sum to the same float in both orders. The
       random graphs never produce one: 140 million random triples were
       searched and this class of difference needs components whose
       exponents are tens apart. */
    for (side = 0; side < 2; ++side)
    {
        WMOCK_Reset();
        sceneReset();
        buildGraph(&g, 2, 0, 0, 902);
        g.gao[0]->matrices->Pos.x = 0.0f;
        g.gao[0]->matrices->Pos.y = 0.0f;
        g.gao[0]->matrices->Pos.z = 0.0f;
        g.gao[1]->matrices->Pos.x = -0x1.48625cp-28f;
        g.gao[1]->matrices->Pos.y = 0x1.569506p-29f;
        g.gao[1]->matrices->Pos.z = 0x1.ca934cp-22f;
        if (side)
        {
            NEW_WAY_gst_Seen.actElemNum = 0;
            NEW_WAY_ComputePoundNetworksRec(g.net[0], g.gao[0], 0);
        }
        else
        {
            BASE_WAY_gst_Seen.actElemNum = 0;
            BASE_WAY_ComputePoundNetworksRec(g.net[0], g.gao[0], 0);
        }
        capture(side ? &s_new : &s_base, 0, side);
    }
    compare("PoundSummationOrder", 0);
}

int main(void)
{
    printf("=== ways differential test ===\n");

    /* One seen set per side, built by the file's own init. */
    WMOCK_Reset();
    BASE_WAY_InitModule();
    NEW_WAY_InitModule();

    runLoaderCases();
    runFreeAndReinitCases();
    runRemoveAllCases();
    runSearchCases();
    runPoundCases();
    runTargetedCases();

    printf("\n%d cases, %d differing\n", s_cases, s_fails);
    if (s_fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
