#include "../math.h"
#include "../memory.h"
#include "../gameobject.h"
#include "../ways.h"
#include "ai_gridfunctions.h"
#include "ai_wayfunctions.h"

// Both working sets of the search hold up to this many waypoints.
#define AI_SHORTWAY_MAX 1000

// Waypoints the search may step on, under the flags the script set
// with WAYShortParams: bit 0 asks for the grid to be consulted at all,
// and bit 3 allows the two capabilities 0x40 and 0x80.
static BOOL AI_l_ShortWayAllowsWP(tGameObjectMain* wp)
{
    tWorld* world;
    int capa;

    if ((WAY_gi_ShortParams & 1) == 0)
        return TRUE;

    world = WOR_gpst_CurrentWorld;
    if (!(GRID_gi_Current ? world->grid2Ptr : world->grid1Ptr))
        return TRUE;

    capa = AI_EvalFunc_GRIDGetCapa_C(wp, &wp->matrices->Pos);
    if ((capa & 0x20) != 0)
        return FALSE;
    return (WAY_gi_ShortParams & 8) != 0 || (capa & 0xC0) == 0;
}

// What stepping along a link costs. Bits 1 and 2 of the parameters ask
// for the distance to one of the two main actors instead of the link's
// own weight, so a path can be made to favour or avoid them.
static float AI_l_ShortWayLinkCost(tWayElemInnerElem* link)
{
    tGameObjectMain* actor;
    Vec3 toActor;

    if ((WAY_gi_ShortParams & 6) != 0)
    {
        if ((WAY_gi_ShortParams & 2) != 0)
            actor = AI_MainActor[0];
        else
            actor = AI_MainActor[1];

        if (actor)
        {
            MATH_SubstractVec(&toActor, &actor->matrices->Pos, &link->gao->matrices->Pos);
            return (float)(1.0 / MATH_VecLen(&toActor));
        }
    }
    return link->weight;
}

// Dijkstra over the network from one waypoint to another, writing the
// path into path as a walk back from the target. Returns its length,
// or zero when there is none.
unsigned int AI_EvalFunc_WAYShortWay_C(struct sGameObjectMain* gao, struct sWayNetwork* network, struct sGameObjectMain** path, struct sGameObjectMain* from, struct sGameObjectMain* to)
{
    tGameObjectMain** open;
    float* openCost;
    int* openFrom;
    tGameObjectMain** closed;
    float* closedCost;
    int* closedFrom;
    tWayElem* links;
    tWayElemInnerElem* link;
    float cost;
    float bestCost;
    unsigned int bestIdx;
    int openNum;
    int closedNum;
    int found;
    int i;
    int j;

    bestIdx = 0;
    closedCost = (float*)MEM_p_Alloc(AI_SHORTWAY_MAX * sizeof(float));
    openCost = (float*)MEM_p_Alloc(AI_SHORTWAY_MAX * sizeof(float));
    closedFrom = MEM_p_Alloc(AI_SHORTWAY_MAX * sizeof(int));
    openFrom = MEM_p_Alloc(AI_SHORTWAY_MAX * sizeof(int));
    closed = (tGameObjectMain**)MEM_p_Alloc(AI_SHORTWAY_MAX * sizeof(tGameObjectMain*));
    open = (tGameObjectMain**)MEM_p_Alloc(AI_SHORTWAY_MAX * sizeof(tGameObjectMain*));

    found = 0;
    closedNum = 0;
    openNum = 1;
    open[0] = from;
    openCost[0] = 0.0;
    openFrom[0] = -1;

    while (openNum && !found)
    {
        bestCost = 3.402822e38f; // TODO FIXME? jó ez így?
        for (i = 0; i < openNum; ++i)
        {
            if (openCost[i] < bestCost)
            {
                bestCost = openCost[i];
                bestIdx = i;
            }
        }

        // the cheapest open waypoint closes, and the open set's last
        // entry fills the hole it leaves
        closed[closedNum] = open[bestIdx];
        closedFrom[closedNum] = openFrom[bestIdx];
        closedCost[closedNum++] = bestCost;
        open[bestIdx] = open[--openNum];
        openCost[bestIdx] = openCost[openNum];
        openFrom[bestIdx] = openFrom[openNum];

        if (closed[closedNum - 1] == to)
            found = 1;

        links = WAY_pst_SearchLinkList(closed[closedNum - 1], network);
        if (!links)
            continue;

        for (i = 0; i < links->num; ++i)
        {
            link = &links->innerElems[i];

            // a link with capabilities is only for an object that has
            // them
            if (link->capa && (gao->extended->capa & link->capa) == 0)
                continue;

            for (j = 0; j < closedNum; ++j)
            {
                if (closed[j] == link->gao)
                    break;
            }
            if (j != closedNum)
                continue;

            if (!AI_l_ShortWayAllowsWP(link->gao))
                continue;

            cost = AI_l_ShortWayLinkCost(link) + closedCost[closedNum - 1];

            for (j = 0; j < openNum; ++j)
            {
                if (open[j] == link->gao)
                    break;
            }
            if (j == openNum)
            {
                open[openNum] = link->gao;
                openCost[openNum] = cost;
                openFrom[openNum++] = closedNum - 1;
            }
            else if (cost < openCost[j])
            {
                openCost[j] = cost;
                openFrom[j] = closedNum - 1;
            }
        }
    }

    if (found)
    {
        j = 0;
        for (i = closedNum - 1; i >= 0; i = closedFrom[i])
            path[j++] = closed[i];
        closedNum = j;
    }
    else
    {
        closedNum = 0;
    }

    MEM_Free(closedCost);
    MEM_Free(openCost);
    MEM_Free(closedFrom);
    MEM_Free(openFrom);
    MEM_Free(open);
    MEM_Free(closed);
    return closedNum;
}

// The waypoint a script steps to next. The type picks how: the first
// link, a random one, the lightest, the one most nearly straight ahead
// (3 and 4 take the forward of the object or of the network's root, 5
// its direction of travel), or a link by index.
struct sGameObjectMain* AI_EvalFunc_WAYNetNextWP_C(struct sGameObjectMain* gao, struct sWayNetwork* network, struct sGameObjectMain* wp, int type, int elemID)
{
    tWayElem* elem;
    tGameObjectMain* linked;
    Vec3 forward;
    Vec3 speed;
    Vec3* direction;
    Vec3 toLinked;
    float lowestWeight;
    float bestDot;
    float dot;
    unsigned int i;
    int bestIdx;
    int rndNum;
    int j;

    bestIdx = 0;

    if (!wp)
        return network->gao;

    // the waypoint's entry for this network; one that has none reads a
    // slot past the end, as the original does
    for (i = 0; i < wp->extended->way->wayElemNum && wp->extended->way->wayElems[i].network != network; i++)
        ;
    elem = &wp->extended->way->wayElems[i];

    if (!elem->innerElems)
        return NULL;

    switch (type)
    {
    case 0:
        if (!elem->num)
            return NULL;
        return elem->innerElems[0].gao;

    case 1:
        if (!elem->num)
            return NULL;
        rndNum = elem->num * RND_JADErand() / 0x7fff;
        if (rndNum == elem->num)
            --rndNum;
        return elem->innerElems[rndNum].gao;

    case 2:
        if (!elem->num)
            return NULL;
        lowestWeight = FLT_MAX;
        for (j = 0; j < elem->num; j++)
        {
            if (elem->innerElems[j].weight < lowestWeight)
            {
                lowestWeight = elem->innerElems[j].weight;
                bestIdx = j;
            }
        }
        return elem->innerElems[bestIdx].gao;

    case 3:
        MATH_VecInvert(&forward, &gao->matrices->v2);
        direction = &forward;
        break;

    case 4:
        MATH_VecInvert(&forward, &network->gao->matrices->v2);
        direction = &forward;
        break;

    case 5:
        DYN_SpeedGetVector(gao->basic->dyn, &speed);
        MATH_NormalizeVector(&speed, &speed);
        direction = &speed;
        break;

    case 6:
        if (elemID >= elem->num)
            return NULL;
        return elem->innerElems[elemID].gao;

    default:
        return NULL;
    }

    // the link whose direction from the object is closest to the one
    // the type chose; elemID, where the script gives one, is a mask the
    // link's capabilities have to meet
    bestDot = -2.0f;
    bestIdx = -1;
    for (j = 0; j < elem->num; j++)
    {
        linked = elem->innerElems[j].gao;
        if (!linked)
            break;

        if (elemID && ((uint16_t)elemID & elem->innerElems[j].capa) == 0)
            continue;

        MATH_SubstractVec(&toLinked, &linked->matrices->Pos, &gao->matrices->Pos);
        MATH_NormalizeVector(&toLinked, &toLinked);
        dot = MATH_DotProduct(direction, &toLinked);
        if (dot > bestDot)
        {
            bestDot = dot;
            bestIdx = j;
        }
    }

    if (bestIdx == -1)
        return NULL;
    return elem->innerElems[bestIdx].gao;
}

int AI_EvalFunc_WAYGetNumLinks_C(tWayNetwork* network, tGameObjectMain* wp)
{
    tWay* way;
    unsigned int i;

    way = wp->extended->way;
    for (i = 0; i < way->wayElemNum && way->wayElems[i].network != network; ++i)
        ;
    return way->wayElems[i].num;
}

tAINode* AI_EvalFunc_WAYGetNumLinks(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* wp;

    wp = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_WAYGetNumLinks_C(network, wp));
    return node + 1;
}

tAINode* AI_EvalFunc_WAYNetNextWP(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* next;
    tWayNetwork* network;
    tGameObjectMain* wp;
    int type;
    int elemID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    elemID = AI_GetIntFromStack();
    type = AI_GetIntFromStack();
    wp = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    next = AI_EvalFunc_WAYNetNextWP_C(gaoUltra, network, wp, type, elemID);
    AI_PutObjectToStack(&next, AI_gaw_EnumLink[40]);
    return node + 1;
}

struct sGameObjectMain* AI_EvalFunc_WAYRootGet_C(struct sWayNetwork* network)
{
    return network->gao;
}

tAINode* AI_EvalFunc_WAYRootGet(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* root;

    network = (tWayNetwork*)AI_GetObjectFromStack();
    root = AI_EvalFunc_WAYRootGet_C(network);
    AI_PutObjectToStack(&root, AI_gaw_EnumLink[40]);
    return node + 1;
}

tGameObjectMain** AI_WAYNetFillRec(tWayNetwork* network, tGameObjectMain* gao, tGameObjectMain** gaoList, int flagMustHave, int flagMustNotHave, int whatToTest)
{
    unsigned int i;
    int j;

    if (!gao)
        return gaoList;
    
    if (!gaoList)
        return NULL;

    if (BAS_bsearch((unsigned int)gao, &WAY_gst_Seen) != -1)
        return gaoList;

    BAS_binsert((unsigned int)gao, (int)gao, &WAY_gst_Seen);

    if (OBJ_b_test_flag(gao, flagMustHave, flagMustNotHave, whatToTest))
    {
        ++okok;
        *gaoList++ = gao;
    }

    if (!gao->extended || !gao->extended->way)
        return gaoList;

    for (i = 0; i < gao->extended->way->wayElemNum; i++)
    {
        if (gao->extended->way->wayElems[i].network == network)
        {
            for (j = 0; j < gao->extended->way->wayElems[i].num; j++)
                gaoList = AI_WAYNetFillRec(network, gao->extended->way->wayElems[i].innerElems[j].gao, gaoList, flagMustHave, flagMustNotHave, whatToTest);
        }
    }
    return gaoList;
}

int AI_EvalFunc_WAYNetFill_C(struct sWayNetwork* network, struct sGameObjectMain* gao, struct sGameObjectMain** gaoList)
{
    int actElemNumBak;

    if (!gao)
        gao = network->gao;
    
    WAY_gst_Seen.actElemNum = 0;
    
    AI_WAYNetFillRec(network, gao, gaoList, 0xffffffff, 0, 0);
    
    actElemNumBak = WAY_gst_Seen.actElemNum;
    
    WAY_gst_Seen.actElemNum = 0;
    
    return actElemNumBak;
}

tAINode* AI_EvalFunc_WAYNetFill(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    tWayNetwork* network;
    tGameObjectMain* gao;

    AI_GetVarFromStack(converted, &var);
    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_WAYNetFill_C(network, gao, (tGameObjectMain**)var.ptr));
    return node + 1;
}

int AI_EvalFunc_WAYGet_C(struct sWayNetwork* network, struct sGameObjectMain** gaoList, int flagMustHave, int flagMustNotHave, int whatToTest)
{
    okok = 0;    
    WAY_gst_Seen.actElemNum = 0;
    
    AI_WAYNetFillRec(network, network->gao, gaoList, flagMustHave, flagMustNotHave, whatToTest);
    
    WAY_gst_Seen.actElemNum = 0;
    
    return okok;
}

tAINode* AI_EvalFunc_WAYGet(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    tWayNetwork* network;
    int whatToTest;
    int flagMustHave;
    int flagMustNotHave;

    whatToTest = AI_GetIntFromStack();
    flagMustNotHave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    network = (tWayNetwork*)AI_GetObjectFromStack();

    AI_PutIntToStack(AI_EvalFunc_WAYGet_C(network, (tGameObjectMain**)var.ptr,
                                          flagMustHave, flagMustNotHave, whatToTest));
    return node + 1;
}

void AI_WAYNearestOfPosRec(tWayNetwork* network, tGameObjectMain* gao, Vec3* pos, int flagMustHave, int flagMustNothave, int whatToTest, tGameObjectMain** excludeGaoList, unsigned int excludeGaoListNum)
{
    unsigned int i;
    int j;
    Vec3 posDiff;
    float distance;

    if (BAS_bsearch((unsigned int)gao, &WAY_gst_Seen) == -1)
    {
        BAS_binsert((unsigned int)gao, (int)gao, &WAY_gst_Seen);
        
        for (i = 0; i < excludeGaoListNum && excludeGaoList[i] != gao; i++);

        if (i == excludeGaoListNum && OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
        {
            MATH_SubstractVec(&posDiff, &gao->matrices->Pos, pos);
            distance = MATH_VecLen(&posDiff);
            
            if (AI_WAYgpstBest)
            {
                if (distance < AI_WAYgfBest)
                {
                    AI_WAYgpstBest = gao;
                    AI_WAYgfBest = distance;
                }
            }
            else
            {
                AI_WAYgpstBest = gao;
                AI_WAYgfBest = distance;
            }
        }
        if (gao && gao->extended && gao->extended->way)
        {
            for (i = 0; i < gao->extended->way->wayElemNum; i++)
            {
                if (gao->extended->way->wayElems[i].network == network)
                {
                    for (j = 0; j < gao->extended->way->wayElems[i].num; j++) 
                    {
                        AI_WAYNearestOfPosRec(network, gao->extended->way->wayElems[i].innerElems[j].gao, pos,
                            flagMustHave, flagMustNothave, whatToTest, excludeGaoList, excludeGaoListNum);
                    }
                }
            }
        }
    }
}

struct sGameObjectMain* AI_EvalFunc_WAYNearestWPOfObj_C(struct sGameObjectMain* gao, struct sWayNetwork* network, int flagMustHave, int flagMustNothave, int whatToTest)
{
    AI_WAYgpstBest = 0;
    WAY_gst_Seen.actElemNum = 0;

    AI_WAYNearestOfPosRec(network, network->gao, &gao->matrices->Pos, flagMustHave, flagMustNothave, whatToTest, 0, 0);
    
    return AI_WAYgpstBest;
}

tAINode* AI_EvalFunc_WAYNearestWPOfObj(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* nearest;
    tWayNetwork* network;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    nearest = AI_EvalFunc_WAYNearestWPOfObj_C(gaoUltra, network, flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(&nearest, AI_gaw_EnumLink[40]);
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_WAYNearestWPOfObjExcl_C(tGameObjectMain* gao, tWayNetwork* network, int flagMustHave, int flagMustNothave, int whatToTest, tGameObjectMain** excludeGaoList, unsigned int excludeGaoListNum)
{
    AI_WAYgpstBest = 0;
    WAY_gst_Seen.actElemNum = 0;
    AI_WAYNearestOfPosRec(network, network->gao, &gao->matrices->Pos, flagMustHave, flagMustNothave,
                          whatToTest, excludeGaoList, excludeGaoListNum);
    return AI_WAYgpstBest;
}

tAINode* AI_EvalFunc_WAYNearestWPOfObjExcl(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* nearest;
    char converted[52];
    tVarStackElem var;
    tWayNetwork* network;
    int excludeGaoListNum;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    excludeGaoListNum = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    nearest = AI_EvalFunc_WAYNearestWPOfObjExcl_C(gaoUltra, network, flagMustHave, flagMustNothave,
                                                  whatToTest, (tGameObjectMain**)var.ptr,
                                                  excludeGaoListNum);
    AI_PutObjectToStack(&nearest, AI_gaw_EnumLink[40]);
    return node + 1;
}

struct sWayNetwork* AI_EvalFunc_WAYNetOfObj_C(struct sGameObjectMain* gao)
{
    if (!gao->extended || !gao->extended->way)
        return NULL;
    return gao->extended->way->wayElems->network;
}

tAINode* AI_EvalFunc_WAYNetOfObj(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tWayNetwork* network;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    network = AI_EvalFunc_WAYNetOfObj_C(gaoUltra);
    AI_PutObjectToStack(&network, AI_gaw_EnumLink[43]);
    return node + 1;
}

tAINode* AI_EvalFunc_WAYShortParams(tAINode* node)
{
    WAY_gi_ShortParams = AI_GetIntFromStack();
    return node + 1;
}

tAINode* AI_EvalFunc_WAYShortWay(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char converted[52];
    tVarStackElem var;
    tWayNetwork* network;
    tGameObjectMain* from;
    tGameObjectMain* to;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    to = (tGameObjectMain*)AI_GetObjectFromStack();
    from = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_GetVarFromStack(converted, &var);
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_WAYShortWay_C(gaoUltra, network, (tGameObjectMain**)var.ptr, from, to));
    return node + 1;
}

tWayElemInnerElem* AI_WayGetLink(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2)
{
    tWayElem* linkList = WAY_pst_SearchLinkList(node1, network);

    return WAY_pst_SearchOneLinkInList(linkList, node2);
}

void AI_EvalFunc_WAYCapaSetLink_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2, int16_t capaSet, int16_t capaRemove)
{
    tWayElemInnerElem* link = AI_WayGetLink(network, node1, node2);

    link->capa &= ~capaRemove;
    link->capa |= capaSet;
}

tAINode* AI_EvalFunc_WAYCapaSetLink(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* node1;
    tGameObjectMain* node2;
    int16_t capaSet;
    int16_t capaRemove;

    capaRemove = AI_GetIntFromStack();
    capaSet = AI_GetIntFromStack();
    node2 = (tGameObjectMain*)AI_GetObjectFromStack();
    node1 = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_EvalFunc_WAYCapaSetLink_C(network, node1, node2, capaSet, capaRemove);
    return node + 1;
}

int AI_EvalFunc_WAYCapaGetLink_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2)
{
    return (uint16_t)AI_WayGetLink(network, node1, node2)->capa;
}

tAINode* AI_EvalFunc_WAYCapaGetLink(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* node1;
    tGameObjectMain* node2;

    node2 = (tGameObjectMain*)AI_GetObjectFromStack();
    node1 = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_WAYCapaGetLink_C(network, node1, node2));
    return node + 1;
}

void AI_EvalFunc_WAYDesignSetLink_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2, char designFlagSet, char designFlagRemove)
{
    tWayElemInnerElem* link = AI_WayGetLink(network, node1, node2);

    link->design &= ~designFlagRemove;
    link->design |= designFlagSet;
}

tAINode* AI_EvalFunc_WAYDesignSetLink(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* node1;
    tGameObjectMain* node2;
    char designFlagSet;
    char designFlagRemove;

    designFlagRemove = (char)AI_GetIntFromStack();
    designFlagSet = (char)AI_GetIntFromStack();
    node2 = (tGameObjectMain*)AI_GetObjectFromStack();
    node1 = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_EvalFunc_WAYDesignSetLink_C(network, node1, node2, designFlagSet, designFlagRemove);
    return node + 1;
}

int AI_EvalFunc_WAYDesignGetLink_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2)
{
    return (char)AI_WayGetLink(network, node1, node2)->design;
}

tAINode* AI_EvalFunc_WAYDesignGetLink(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* node1;
    tGameObjectMain* node2;

    node2 = (tGameObjectMain*)AI_GetObjectFromStack();
    node1 = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_PutIntToStack(AI_EvalFunc_WAYDesignGetLink_C(network, node1, node2));
    return node + 1;
}

tWayElemInnerElem* AI_WayGetUnorientedLink(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2)
{
    tWayElem* linkList;
    tWayElemInnerElem* linkElem;

    if (node1->extended && node1->extended->way)
    {
        linkList = WAY_pst_SearchLinkList(node1, network);
        if (linkList)
        {
            linkElem = WAY_pst_SearchOneLinkInList(linkList, node2);
            if (linkElem)
                return linkElem;
        }
    }

    linkList = WAY_pst_SearchLinkList(node2, network);
    
    return WAY_pst_SearchOneLinkInList(linkList, node1);
}

tWayElemInnerElem* AI_EvalFunc_WAYLinkSetWeight_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2, float weight)
{
    tWayElemInnerElem* link;

    link = AI_WayGetUnorientedLink(network, node1, node2);
    link->weight = weight;

    return link;
}

tAINode* AI_EvalFunc_WAYLinkSetWeight(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* node1;
    tGameObjectMain* node2;
    float weight;

    weight = AI_GetFloatFromStack();
    node2 = (tGameObjectMain*)AI_GetObjectFromStack();
    node1 = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_EvalFunc_WAYLinkSetWeight_C(network, node1, node2, weight);
    return node + 1;
}

float AI_EvalFunc_WAYLinkGetWeight_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2)
{
    return AI_WayGetUnorientedLink(network, node1, node2)->weight;
}

tAINode* AI_EvalFunc_WAYLinkGetWeight(tAINode* node)
{
    tWayNetwork* network;
    tGameObjectMain* node1;
    tGameObjectMain* node2;

    node2 = (tGameObjectMain*)AI_GetObjectFromStack();
    node1 = (tGameObjectMain*)AI_GetObjectFromStack();
    network = (tWayNetwork*)AI_GetObjectFromStack();
    AI_PutFloatToStack(AI_EvalFunc_WAYLinkGetWeight_C(network, node1, node2));
    return node + 1;
}
