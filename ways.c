#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "ways.h"

void WAY_FreeStruct(struct sWay* way)
{
    unsigned int i;

    if (!way)
        return;

    if (way->wayElems)
    {
        for (i = 0; i < way->wayElemNum; ++i)
        {
            if (way->wayElems[i].innerElems)
            {
                MEM_Free(way->wayElems[i].innerElems);
                way->wayElems[i].innerElems = 0;
            }
        }
        MEM_Free(way->wayElems);
        way->wayElems = 0;
    }
    LOA_DeleteAddress((int)way);
    MEM_Free(way);
}

void WAY_ReinitLinks(struct sWay* way)
{
    tWayElem* elem;
    tWayElemInnerElem* link;
    unsigned int i;
    int j;

    if (!way)
        return;

    for (i = 0; i < way->wayElemNum; ++i)
    {
        elem = &way->wayElems[i];
        for (j = 0; j < elem->num; ++j)
        {
            link = &elem->innerElems[j];
            link->capa = link->capaBackup;
            link->design = link->designBackup;
        }
    }
}

void WAY_RemoveAllNetworks(tWayAllNetworks* allNetworks)
{
    int i;

    if (allNetworks)
    {
        // preserved: networks is indexed here and only null-tested after the loop
        for (i = 0; i < allNetworks->networkNum; ++i)
        {
            if (allNetworks->networks[i])
            {
                LOA_DeleteAddress((int)allNetworks->networks[i]);
                MEM_Free(allNetworks->networks[i]);
            }
        }
        if (allNetworks->networks)
            MEM_Free(allNetworks->networks);
        LOA_DeleteAddress((int)allNetworks);
        MEM_Free(allNetworks);
    }
}

tWayNetwork* WAY_ul_NetWorkCallback(uint32_t offset)
{
    tWayNetwork* network;
    char* data;
    int rootKey;
    int size;

    network = (tWayNetwork*)MEM_p_Alloc(8u);
    network->gao = 0;
    network->flags = 0;
    data = (char*)BIG_pc_ReadFileTmp(offset, &size);
    rootKey = LOA_ReadLong(&data, 0, 1);
    if (!rootKey || rootKey == -1)
        network->gao = 0;
    else
        LOA_MakeFileRef(rootKey, &network->gao, OBJ_ul_GameObjectCallback, 1);
    network->flags = LOA_ReadLong(&data, 0, 1);
    return network;
}

tWayAllNetworks* WAY_ul_AllNetWorksCallback(uint32_t offset)
{
    tWayAllNetworks* allNetworks;
    char* data;
    int networkKey;
    int num;
    int size;
    int i;

    allNetworks = (tWayAllNetworks*)MEM_p_Alloc(8u);
    allNetworks->networks = 0;
    allNetworks->networkNum = 0;
    data = (char*)BIG_pc_ReadFileTmp(offset, &size);
    if (!data)
        return NULL;

    num = LOA_ReadLong(&data, 0, 1);
    allNetworks->networkNum = num;
    if (num)
    {
        allNetworks->networks = (tWayNetwork**)MEM_p_Alloc(4 * num);
        for (i = 0; i < allNetworks->networkNum; ++i)
        {
            networkKey = LOA_ReadLong(&data, 0, 1);
            LOA_MakeFileRef(networkKey, &allNetworks->networks[i], WAY_ul_NetWorkCallback, 1);
        }
    }
    return allNetworks;
}

void WAY_InitModule()
{
    BAS_binit(&WAY_gst_Seen, 1000);
}

void WAY_CloseModule()
{
    BAS_bfree(&WAY_gst_Seen);
}

struct sWayElem* WAY_pst_SearchLinkList(struct sGameObjectMain* wp, struct sWayNetwork* network)
{
    tGameObjectExtended* extended;
    tWay* way;
    unsigned int i;

    extended = wp->extended;
    if (!extended)
        return 0;

    way = extended->way;
    if (!way)
        return 0;

    if (!way->wayElems)
        return 0;

    for (i = 0; i < way->wayElemNum; ++i)
    {
        if (way->wayElems[i].network == network)
            return &way->wayElems[i];
    }
    return 0;
}

struct sWayElemInnerElem* WAY_pst_SearchOneLinkInList(struct sWayElem* linkList, struct sGameObjectMain* wp)
{
    tWayElemInnerElem* links;
    unsigned int num;
    unsigned int i;

    num = linkList->num;
    if (!num)
        return 0;

    links = linkList->innerElems;
    for (i = 0; i < num; ++i)
    {
        if (links[i].gao == wp)
            return &links[i];
    }
    return 0;
}

void WAY_ComputePoundNetworksRec(tWayNetwork* network, tGameObjectMain* gao, tGameObjectMain* stopAt)
{
    tWayElem* linkList;
    tWayElemInnerElem* link;
    tGameObjectMain* linked;
    Vec3 pos;
    double dx;
    double dy;
    double dz;
    int i;

    if (BAS_bsearch((unsigned int)gao, &WAY_gst_Seen) != -1)
        return;

    BAS_binsert((unsigned int)gao, (int)gao, &WAY_gst_Seen);

    if (!gao || !gao->extended || !gao->extended->way)
        return;

    linkList = WAY_pst_SearchLinkList(gao, network);
    if (!linkList)
        return;

    for (i = 0; i < linkList->num; ++i)
    {
        pos = gao->matrices->Pos;
        link = &linkList->innerElems[i];
        linked = link->gao;
        if (!linked)
            continue;

        dx = pos.x - linked->matrices->Pos.x;
        dy = pos.y - linked->matrices->Pos.y;
        dz = pos.z - linked->matrices->Pos.z;
        link->weight = (float)sqrt(dz * dz + dy * dy + dx * dx);

        // preserved: this returns from the whole call, so the waypoint's remaining links go unweighted
        if (link->gao == stopAt)
            return;
        WAY_ComputePoundNetworksRec(network, link->gao, stopAt);
    }
}

void WAY_ComputePoundOneNetworkForObj(tWayNetwork* network, tGameObjectMain* stopAt)
{
    WAY_gst_Seen.actElemNum = 0;
    if (network->gao)
        WAY_ComputePoundNetworksRec(network, network->gao, stopAt);
}

void WAY_ComputePoundOneNetwork(tWayNetwork* net)
{
    WAY_ComputePoundOneNetworkForObj(net, 0);
}

void WAY_ComputePoundNetworks(struct sWorld* world)
{
    tWayNetwork** networks;
    int i;

    if (!world->wayPtr)
        return;

    for (i = 0; i < world->wayPtr->networkNum; ++i)
    {
        gpst_CurrentWorld = world;
        networks = world->wayPtr->networks;
        if (networks[i])
            WAY_ComputePoundOneNetwork(networks[i]);
        gpst_CurrentWorld = 0;
    }
}

struct sWay* WAY_ul_AllLinkListsCallback(uint32_t offset)
{
    tWay* way;
    tWayElem* elem;
    tWayElemInnerElem* link;
    char* data;
    int networkKey;
    int gaoKey;
    int num;
    unsigned int i;
    int j;
    int size;

    way = (tWay*)MEM_p_Alloc(sizeof(tWay));
    way->wayElems = 0;
    way->wayElemNum = 0;
    data = BIG_pc_ReadFileTmp(offset, &size);

    num = LOA_ReadLong(&data, 0, 1);
    way->wayElemNum = num;
    if (num)
        way->wayElems = (tWayElem*)MEM_p_Alloc(sizeof(tWayElem) * num);

    for (i = 0; i < way->wayElemNum; ++i)
    {
        elem = &way->wayElems[i];
        networkKey = LOA_ReadLong(&data, 0, 1);
        LOA_MakeFileRef(networkKey, (void**)&elem->network, WAY_ul_NetWorkCallback, 1);
        elem->innerElems = 0;
        elem->num = LOA_ReadLong(&data, 0, 1);
        if (elem->num)
        {
            elem->innerElems = (tWayElemInnerElem*)MEM_p_Alloc(sizeof(tWayElemInnerElem) * elem->num);
            for (j = 0; j < elem->num; ++j)
            {
                link = &elem->innerElems[j];
                link->capa = LOA_ReadShort(&data, 0, 1);
                link->capaBackup = link->capa;
                gaoKey = LOA_ReadLong(&data, 0, 1);
                LOA_MakeFileRef(gaoKey, (void**)&link->gao, OBJ_ul_GameObjectCallback, 1);
                link->design = LOA_ReadChar(&data, 0, 1);
                link->designBackup = link->design;
                link->s3 = LOA_ReadShort(&data, 0, 1);
            }
        }
    }
    return way;
}
