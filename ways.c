#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "ways.h"

void WAY_FreeStruct(struct sWay* a1)
{
    unsigned int v1;
    int v2;

    if (a1)
    {
        if (a1->wayElems)
        {
            v1 = 0;
            if (a1->wayElemNum)
            {
                v2 = 0;
                do
                {
                    if (a1->wayElems[v2].innerElems)
                    {
                        MEM_Free(a1->wayElems[v2].innerElems);
                        a1->wayElems[v2].innerElems = 0;
                    }
                    ++v1;
                    ++v2;
                } while (v1 < a1->wayElemNum);
            }
            MEM_Free(a1->wayElems);
            a1->wayElems = 0;
        }
        LOA_DeleteAddress((int)a1);
        MEM_Free(a1);
    }
}

void WAY_ReinitLinks(struct sWay* a1)
{
    unsigned int v1;
    tWayElem* v2;
    int v3;
    tWayElem* v4;
    int v5;
    int v6;
    tWayElemInnerElem* v7;
    __int16 v8;
    tWayElemInnerElem* v9;

    if (a1)
    {
        v1 = 0;
        if (a1->wayElemNum)
        {
            v2 = a1->wayElems;
            v3 = 0;
            do
            {
                v4 = &v2[v3];
                v5 = 0;
                if (v2[v3].num)
                {
                    v6 = 0;
                    do
                    {
                        v7 = v4->innerElems;
                        v8 = v7[v6].capaBackup;
                        v9 = &v7[v6];
                        v9->capa = v8;
                        v9->design = v9->designBackup;
                        v2 = a1->wayElems;
                        ++v5;
                        v4 = &a1->wayElems[v3];
                        ++v6;
                    } while (v5 < v4->num);
                }
                ++v1;
                ++v3;
            } while (v1 < a1->wayElemNum);
        }
    }
}

void WAY_RemoveAllNetworks(tWayAllNetworks* a1)
{
    int i;

    if (a1)
    {
        for (i = 0; i < a1->networkNum; ++i)
        {
            if (a1->networks[i])
            {
                LOA_DeleteAddress((int)a1->networks[i]);
                MEM_Free(a1->networks[i]);
            }
        }
        if (a1->networks)
            MEM_Free(a1->networks);
        LOA_DeleteAddress((int)a1);
        MEM_Free(a1);
    }
}

tWayNetwork* WAY_ul_NetWorkCallback(uint32_t Offset)
{
    tWayNetwork* v1;
    int v2;
    char* a1;
    int v5;

    v1 = (tWayNetwork*)MEM_p_Alloc(8u);
    v1->gao = 0;
    v1->flags = 0;
    a1 = (char*)BIG_pc_ReadFileTmp(Offset, &v5);
    v2 = LOA_ReadLong(&a1, 0, 1);
    if (!v2 || v2 == -1)
        v1->gao = 0;
    else
        LOA_MakeFileRef(v2, &v1->gao, OBJ_ul_GameObjectCallback, 1);
    v1->flags = LOA_ReadLong(&a1, 0, 1);
    return v1;
}

tWayAllNetworks* WAY_ul_AllNetWorksCallback(uint32_t Offset)
{
    tWayAllNetworks* v1;
    tWayAllNetworks* result;
    int v3;
    int i;
    int v5;
    char* a1;
    int v7;

    v1 = (tWayAllNetworks*)MEM_p_Alloc(8u);
    v1->networks = 0;
    v1->networkNum = 0;
    a1 = (char *)BIG_pc_ReadFileTmp(Offset, &v7);
    result = NULL;
    if (a1)
    {
        v3 = LOA_ReadLong(&a1, 0, 1);
        v1->networkNum = v3;
        if (v3)
        {
            v1->networks = (tWayNetwork**)MEM_p_Alloc(4 * v3);
            for (i = 0; i < v1->networkNum; ++i)
            {
                v5 = LOA_ReadLong(&a1, 0, 1);
                LOA_MakeFileRef(v5, &v1->networks[i], WAY_ul_NetWorkCallback, 1);
            }
        }
        result = v1;
    }
    return result;
}

void WAY_InitModule()
{
    BAS_binit(&WAY_gst_Seen, 1000);
}

void WAY_CloseModule()
{
    BAS_bfree(&WAY_gst_Seen);
}

struct sWayElem* WAY_pst_SearchLinkList(struct sGameObjectMain* a1, struct sWayNetwork* a2)
{
    tGameObjectExtended* v2;
    tWay* v3;
    tWayElem *result = NULL;
    unsigned int idx;
    tWayElem* v5;
    unsigned int v6;
    tWayElem* v7;

    v2 = a1->extended;
    if (!v2)
        return 0;
    v3 = v2->way;
    if (!v3)
        return 0;
    v5 = v3->wayElems;
    idx = 0;
    if (v3->wayElems)
    {
        v6 = v3->wayElemNum;
        if (v6)
        {
            v7 = v3->wayElems;
            while (v7->network != a2)
            {
                ++idx;
                ++v7;
                if (idx >= v6)
                    goto LABEL_9;
            }
            result = &v5[idx];
        }
        else
        {
        LABEL_9:
            result = 0;
        }
    }
    return result;
}

struct sWayElemInnerElem* WAY_pst_SearchOneLinkInList(struct sWayElem* a1, struct sGameObjectMain* a2)
{
    unsigned int v2;
    unsigned int v3;
    tWayElemInnerElem* v4;

    v2 = a1->num;
    v3 = 0;
    if (!v2)
        return 0;
    v4 = a1->innerElems;
    
    while (v4[v3].gao != a2) {
        if (++v3 >= v2)
            return 0;
    }
    return &v4[v3];    
}

void WAY_ComputePoundNetworksRec(tWayNetwork* a1, tGameObjectMain* gao, tGameObjectMain* a3)
{
    int v3;
    tGameObjectExtended* v4;
    tWayElem* v5;
    tWayElem* v6;
    Mtx4x4* v7;
    float v8;
    tWayElemInnerElem* v9;
    tGameObjectMain* v10;
    Mtx4x4* v11;
    double v12;
    double v13;
    double v14;
    tGameObjectMain* v15;
    int v16;
    float v17;
    float v18;

    if (BAS_bsearch((unsigned int)gao, &WAY_gst_Seen) == -1)
    {
        BAS_binsert((unsigned int)gao, (int)gao, &WAY_gst_Seen);
        v3 = 0;
        if (gao)
        {
            v4 = gao->extended;
            if (v4)
            {
                if (v4->way)
                {
                    v5 = WAY_pst_SearchLinkList(gao, a1);
                    v6 = v5;
                    if (v5)
                    {
                        v16 = 0;
                        if (v5->num)
                        {
                            do
                            {
                                v7 = gao->matrices;
                                v8 = v7->Pos.y;
                                v17 = v7->Pos.x;
                                v9 = &v6->innerElems[v3];
                                v18 = v7->Pos.z;
                                v10 = v9->gao;
                                if (v10)
                                {
                                    v11 = v10->matrices;
                                    v12 = v17 - v11->Pos.x;
                                    v13 = v8 - v11->Pos.y;
                                    v14 = v18 - v11->Pos.z;
                                    v9->weight = (float)sqrt(v14 * v14 + v13 * v13 + v12 * v12);
                                    v15 = v6->innerElems[v3].gao;
                                    if (v15 == a3)
                                        return;
                                    WAY_ComputePoundNetworksRec(a1, v15, a3);
                                }
                                ++v3;
                                ++v16;
                            } while (v16 < v6->num);
                        }
                    }
                }
            }
        }
    }
}

void WAY_ComputePoundOneNetworkForObj(tWayNetwork* a1, tGameObjectMain* a2)
{
    WAY_gst_Seen.actElemNum = 0;
    if (a1->gao)
        WAY_ComputePoundNetworksRec(a1, a1->gao, a2);
}

void WAY_ComputePoundOneNetwork(tWayNetwork* net)
{
    WAY_ComputePoundOneNetworkForObj(net, 0);
}

void WAY_ComputePoundNetworks(struct sWorld* a1)
{
    tWayAllNetworks* v1;
    int v2;
    tWayNetwork** v3;

    v1 = a1->wayPtr;
    if (v1)
    {
        v2 = 0;
        if (v1->networkNum)
        {
            do
            {
                gpst_CurrentWorld = a1;
                v3 = a1->wayPtr->networks;
                if (v3[v2])
                    WAY_ComputePoundOneNetwork(v3[v2]);
                gpst_CurrentWorld = 0;
                ++v2;
            } while (v2 < a1->wayPtr->networkNum);
        }
    }
}

struct sWay* WAY_ul_AllLinkListsCallback(uint32_t Offset)
{
    tWay* v1;
    int j;
    int v3;
    BOOL v4;
    int v5;
    int v6;
    int v7;
    int i;
    int v9;
    unsigned int v10;
    char* a1;
    unsigned int v13;
    int a2;

    v1 = (tWay*)MEM_p_Alloc(sizeof(tWay));
    v1->wayElems = 0;
    v1->wayElemNum = 0;
    j = 0;
    a1 = BIG_pc_ReadFileTmp(Offset, &a2);
    v3 = LOA_ReadLong(&a1, 0, 1);
    v1->wayElemNum = v3;
    if (v3)
        v1->wayElems = (tWayElem*)MEM_p_Alloc(sizeof(tWayElem) * v3);
    v4 = v1->wayElemNum == 0;
    v13 = 0;
    if (!v4)
    {
        do
        {
            v5 = 0;
            v6 = LOA_ReadLong(&a1, 0, 1);
            LOA_MakeFileRef(v6, (void**)&(v1->wayElems[j].network), WAY_ul_NetWorkCallback, 1);
            v1->wayElems[j].innerElems = 0;
            v1->wayElems[j].num = LOA_ReadLong(&a1, 0, 1);
            v7 = v1->wayElems[j].num;
            if (v7)
            {
                v1->wayElems[j].innerElems = (tWayElemInnerElem*)MEM_p_Alloc(sizeof(tWayElemInnerElem) * v7);
                for (i = 0; i < v1->wayElems[j].num; ++v5)
                {
                    v1->wayElems[j].innerElems[v5].capa = LOA_ReadShort(&a1, 0, 1);
                    v1->wayElems[j].innerElems[v5].capaBackup = v1->wayElems[j].innerElems[v5].capa;
                    v9 = LOA_ReadLong(&a1, 0, 1);
                    LOA_MakeFileRef(v9, (void**)&(v1->wayElems[j].innerElems[v5].gao), OBJ_ul_GameObjectCallback, 1);
                    v1->wayElems[j].innerElems[v5].design = LOA_ReadChar(&a1, 0, 1);
                    v1->wayElems[j].innerElems[v5].designBackup = v1->wayElems[j].innerElems[v5].design;
                    v1->wayElems[j].innerElems[v5].s3 = LOA_ReadShort(&a1, 0, 1);
                    ++i;
                }
            }
            v10 = v1->wayElemNum;
            ++j;
            ++v13;
        } while (v13 < v10);
    }
    return v1;
}
