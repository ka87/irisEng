#include "../math.h"
#include "../memory.h"
#include "../gameobject.h"
#include "../ways.h"
#include "ai_gridfunctions.h"
#include "ai_wayfunctions.h"

unsigned int AI_EvalFunc_WAYShortWay_C(struct sGameObjectMain* a1, struct sWayNetwork* a2, struct sGameObjectMain** a3, struct sGameObjectMain* a4, struct sGameObjectMain* a5)
{
    int v6;
    int* v7;
    float* v8;
    int v9;
    float v10;
    Vec3 dest;
    int* v12;
    tGameObjectMain* v13;
    tWayElemInnerElem* v14;
    float* v15;
    int v16;
    tWayElem* v17;
    float v18;
    int v19;
    int i;
    int j;
    int v22;
    int v23;
    tWorld* v24;
    tGameObjectMain** v25;
    unsigned int v26;
    tGameObjectMain** v27;

    v26 = 0;
    v8 = (float*)MEM_p_Alloc(4000u);
    v15 = (float*)MEM_p_Alloc(4000u);
    v7 = MEM_p_Alloc(4000u);
    v12 = MEM_p_Alloc(4000u);
    v25 = (tGameObjectMain**)MEM_p_Alloc(4000u);
    v27 = (tGameObjectMain**)MEM_p_Alloc(4000u);
    v22 = 0;
    v19 = 0;
    v9 = 1;
    *v27 = a4;
    *v15 = 0.0;
    *v12 = -1;
    while (v9 && !v22)
    {
        v18 = 3.402822e38f; // TODO FIXME? jó ez így?
        for (i = 0; i < v9; ++i)
        {
            if (v15[i] < (double)v18)
            {
                v18 = v15[i];
                v26 = i;
            }
        }
        v25[v19] = v27[v26];
        v7[v19] = v12[v26];
        v8[v19++] = v18;
        v27[v26] = v27[--v9];
        v15[v26] = v15[v9];
        v12[v26] = v12[v9];
        if (v25[v19 - 1] == a5)
            v22 = 1;
        v17 = WAY_pst_SearchLinkList(v25[v19 - 1], a2);
        if (v17)
        {
            for (i = 0; i < v17->num; ++i)
            {
                v14 = &v17->innerElems[i];
                if (!v14->capa || (a1->extended->capa & v14->capa) != 0)
                {
                    for (j = 0; j < v19; ++j)
                    {
                        if (v25[j] == v14->gao)
                            goto LABEL_13;
                    }
                    if ((WAY_gi_ShortParams & 1) == 0
                        || ((v24 = WOR_gpst_CurrentWorld, GRID_gi_Current) ? (v6 = (int)v24->grid2Ptr) : (v6 = (int)v24->grid1Ptr),
                            (v16 = v6) == 0
                            || (v23 = AI_EvalFunc_GRIDGetCapa_C(v14->gao, &v14->gao->matrices->Pos), (v23 & 0x20) == 0)
                            && ((WAY_gi_ShortParams & 8) != 0 || (v23 & 0xC0) == 0)))
                    {
                        if ((WAY_gi_ShortParams & 6) != 0)
                        {
                            if ((WAY_gi_ShortParams & 2) != 0)
                                v13 = *AI_MainActor;
                            else
                                v13 = AI_MainActor[1];
                            if (v13)
                            {
                                MATH_SubstractVec(&dest, &v13->matrices->Pos, &v14->gao->matrices->Pos);
                                v10 = (float)(1.0 / MATH_VecLen(&dest));
                            }
                            else
                            {
                                v10 = v14->weight;
                            }
                        }
                        else
                        {
                            v10 = v14->weight;
                        }
                        for (j = 0; ; ++j)
                        {
                            if (j >= v9)
                            {
                                v27[v9] = v14->gao;
                                v15[v9] = v10 + v8[v19 - 1];
                                v12[v9++] = v19 - 1;
                                goto LABEL_13;
                            }
                            if (v27[j] == v14->gao)
                                break;
                        }
                        if (v10 + v8[v19 - 1] < v15[j])
                        {
                            v15[j] = v10 + v8[v19 - 1];
                            v12[j] = v19 - 1;
                        }
                    }
                }
            LABEL_13:
                ;
            }
        }
    }
    if (v22)
    {
        i = v19 - 1;
        j = 0;
        while ((i & 0x80000000) == 0)
        {
            a3[j] = v25[i];
            i = v7[i];
            ++j;
        }
        v19 = j;
    }
    else
    {
        v19 = 0;
    }
    MEM_Free(v8);
    MEM_Free(v15);
    MEM_Free(v7);
    MEM_Free(v12);
    MEM_Free(v27);
    MEM_Free(v25);
    return v19;
}

struct sGameObjectMain* AI_EvalFunc_WAYNetNextWP_C(struct sGameObjectMain* a1, struct sWayNetwork* a2, struct sGameObjectMain* a3, int type, int elemID)
{
    int rndNum;
    int j;
    Vec3 dest;
    tGameObjectMain* v13;
    float lowestWeight;
    Vec3 a2a;
    Vec3 v16;
    Vec3* v18;
    float v19;
    unsigned int i;
    unsigned int lowestWeightIdx;
    float v23;
    Vec3* v24;
    tGameObjectMain* retWPGao;

    lowestWeightIdx = 0;
    retWPGao = 0;

    if (!a3)
        return a2->gao;
    
    for (i = 0; i < a3->extended->way->wayElemNum && a3->extended->way->wayElems[i].network != a2; i++);

    if (!a3->extended->way->wayElems[i].innerElems)
        return NULL;

    switch (type)
    {
    case 0:
        if (a3->extended->way->wayElems[i].num)
            retWPGao = a3->extended->way->wayElems[i].innerElems[0].gao;
        else
            retWPGao = NULL;
        break;
    case 1:
        if (a3->extended->way->wayElems[i].num)
        {
            rndNum = a3->extended->way->wayElems[i].num * RND_JADErand() / 0x7fff;
            if (rndNum == a3->extended->way->wayElems[i].num) --rndNum;

            retWPGao = a3->extended->way->wayElems[i].innerElems[rndNum].gao;
        }
        else
        {
            retWPGao = NULL;
        }
        break;
    case 2:
        if (a3->extended->way->wayElems[i].num)
        {
            lowestWeight = FLT_MAX;
            for (j = 0; j < a3->extended->way->wayElems[i].num; j++)
            {
                if (a3->extended->way->wayElems[i].innerElems[j].weight < lowestWeight)
                {
                    lowestWeight = a3->extended->way->wayElems[i].innerElems[j].weight;
                    lowestWeightIdx = j;
                }
            }
            retWPGao = a3->extended->way->wayElems[i].innerElems[lowestWeightIdx].gao;
        }
        else
        {
            retWPGao = NULL;
        }
        break;
    case 3:
    case 4:
    case 5:
        if (type == 3)
        {
            v18 = &a1->matrices->v2;
            v16 = *v18;
            v18 = &v16;
            v16.x = -v16.x;
            v16.y = -v16.y;
            v16.z = -v16.z;
        }
        else if (type == 4)
        {
            v18 = &a2->gao->matrices->v2;
            v16 = *v18;
            v18 = &v16;
            v16.x = -v16.x;
            v16.y = -v16.y;
            v16.z = -v16.z;
        }
        else
        {
            DYN_SpeedGetVector(a1->basic->dyn, &a2a);
            MATH_NormalizeVector(&a2a, &a2a);
            v18 = &a2a;
        }
        v23 = -2.0f;
        lowestWeightIdx = -1;
        for (j = 0; j < a3->extended->way->wayElems[i].num; j++)
        {
            v13 = a3->extended->way->wayElems[i].innerElems[j].gao;
            if (!v13)
                break;
            if (!elemID || ((uint16_t)elemID & a3->extended->way->wayElems[i].innerElems[j].capa) != 0)
            {
                v24 = &v13->matrices->Pos;
                MATH_SubstractVec(&dest, v24, &a1->matrices->Pos);
                MATH_NormalizeVector(&dest, &dest);
                v19 = v18->z * dest.z + v18->y * dest.y + dest.x * v18->x;
                if (v19 > v23)
                {
                    v23 = v19;
                    lowestWeightIdx = j;
                }
            }
        }
        retWPGao = NULL;
        if (lowestWeightIdx != -1)
            retWPGao = a3->extended->way->wayElems[i].innerElems[lowestWeightIdx].gao;
        break;
    case 6:
        if (elemID < a3->extended->way->wayElems[i].num)
            retWPGao = a3->extended->way->wayElems[i].innerElems[elemID].gao;
        else
            retWPGao = NULL;
        break;
    default:
        return retWPGao;
    }
    return retWPGao;
}

int AI_EvalFunc_WAYGetNumLinks_C(tWayNetwork* a1, tGameObjectMain* a2)
{
    tWay* v3;
    unsigned int i;

    v3 = a2->extended->way;
    for (i = 0; i < v3->wayElemNum && v3->wayElems[i].network != a1; ++i)
        ;
    return v3->wayElems[i].num;
}

tAINode* AI_EvalFunc_WAYGetNumLinks(tAINode* a1)
{
    int v1;
    tGameObjectMain* a2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_WAYGetNumLinks_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_WAYNetNextWP(tAINode* a1)
{
    int v2[7];
    tGameObjectMain* v3;
    tGameObjectMain* a3;
    tWayNetwork* a2;
    int type;
    int elemID;
    tGameObjectMain* v8;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v8 = v3;
    elemID = AI_GetIntFromStack();
    type = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2[0] = (int)AI_EvalFunc_WAYNetNextWP_C(v3, a2, a3, type, elemID);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

struct sGameObjectMain* AI_EvalFunc_WAYRootGet_C(struct sWayNetwork* a1)
{
    return a1->gao;
}

tAINode* AI_EvalFunc_WAYRootGet(tAINode* a1)
{
    int v2[3];
    tWayNetwork* a1a;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a1a = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2[0] = (int)AI_EvalFunc_WAYRootGet_C(a1a);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
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

tAINode* AI_EvalFunc_WAYNetFill(tAINode* a1)
{
    int v1;
    char dest[52];
    tWayNetwork* v4;
    tVarStackElem a2;
    tGameObjectMain* gao;

    AI_GetVarFromStack(dest, &a2);
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    gao = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v4 = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = AI_EvalFunc_WAYNetFill_C(v4, gao, (tGameObjectMain**)a2.ptr);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_WAYGet_C(struct sWayNetwork* network, struct sGameObjectMain** gaoList, int flagMustHave, int flagMustNotHave, int whatToTest)
{
    okok = 0;    
    WAY_gst_Seen.actElemNum = 0;
    
    AI_WAYNetFillRec(network, network->gao, gaoList, flagMustHave, flagMustNotHave, whatToTest);
    
    WAY_gst_Seen.actElemNum = 0;
    
    return okok;
}

tAINode* AI_EvalFunc_WAYGet(tAINode* aiNode)
{
    char dest[52];
    int whatToTest;
    int elemNum;
    tWayNetwork* network;
    int flagMustHave;
    int flagMustNotHave;
    tVarStackElem vse;

    whatToTest = AI_GetIntFromStack();
    flagMustNotHave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &vse);
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    network = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];

    elemNum = AI_EvalFunc_WAYGet_C(network, (tGameObjectMain**)vse.ptr, flagMustHave, flagMustNotHave, whatToTest);
    AI_PutIntToStack(elemNum);
    return aiNode + 1;
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

struct sGameObjectMain* AI_EvalFunc_WAYNearestWPOfObj_C(struct sGameObjectMain* a1, struct sWayNetwork* network, int flagMustHave, int flagMustNothave, int whatToTest)
{
    AI_WAYgpstBest = 0;
    WAY_gst_Seen.actElemNum = 0;

    AI_WAYNearestOfPosRec(network, network->gao, &a1->matrices->Pos, flagMustHave, flagMustNothave, whatToTest, 0, 0);
    
    return AI_WAYgpstBest;
}

tAINode* AI_EvalFunc_WAYNearestWPOfObj(tAINode* a1)
{
    int v2[9];
    tGameObjectMain* v3;
    tWayNetwork* a2;
    int flagMustHave;
    int flagMustNothave;
    tGameObjectMain* v7;
    int whatToTest;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v3;
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2[0] = (int)AI_EvalFunc_WAYNearestWPOfObj_C(v3, a2, flagMustHave, flagMustNothave, whatToTest);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

tGameObjectMain* AI_EvalFunc_WAYNearestWPOfObjExcl_C(tGameObjectMain* a1, tWayNetwork* a2, int flagMustHave, int flagMustNothave, int whatToTest, tGameObjectMain** a6, unsigned int a8)
{
    AI_WAYgpstBest = 0;
    WAY_gst_Seen.actElemNum = 0;
    AI_WAYNearestOfPosRec(a2, a2->gao, &a1->matrices->Pos, flagMustHave, flagMustNothave, whatToTest, a6, a8);
    return AI_WAYgpstBest;
}

tAINode* AI_EvalFunc_WAYNearestWPOfObjExcl(tAINode* a1)
{
    int v2[13];
    tGameObjectMain* v3;
    char dest[52];
    tWayNetwork* v5;
    int a8;
    int flagMustHave;
    int flagMustNothave;
    tVarStackElem a2;
    tGameObjectMain* v10;
    int whatToTest;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v10 = v3;
    a8 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &a2);
    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v5 = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v2[0] = (int)AI_EvalFunc_WAYNearestWPOfObjExcl_C(
        v10,
        v5,
        flagMustHave,
        flagMustNothave,
        whatToTest,
        (tGameObjectMain**)a2.ptr,
        a8);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

struct sWayNetwork* AI_EvalFunc_WAYNetOfObj_C(struct sGameObjectMain* a1)
{
    tWayNetwork* result;

    if (a1->extended && a1->extended->way)
        result = a1->extended->way->wayElems->network;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_WAYNetOfObj(tAINode* a1)
{
    int v2[3];
    tGameObjectMain* v3;
    tGameObjectMain* v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v3;
    v2[0] = (int)AI_EvalFunc_WAYNetOfObj_C(v3);
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[43]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_WAYShortParams(tAINode* a1)
{
    WAY_gi_ShortParams = AI_GetIntFromStack();
    return a1 + 1;
}

tAINode* AI_EvalFunc_WAYShortWay(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    tWayNetwork* v5;
    tGameObjectMain* a4;
    tGameObjectMain* a5;
    tVarStackElem a2;
    tGameObjectMain* v9;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v9 = v3;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a5 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a4 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_GetVarFromStack(dest, &a2);
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v5 = *(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = AI_EvalFunc_WAYShortWay_C(v9, v5, (tGameObjectMain**)a2.ptr, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
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

tAINode* AI_EvalFunc_WAYCapaSetLink(tAINode* a1)
{
    tGameObjectMain* a2;
    tGameObjectMain* a3;
    int16_t a5;
    int16_t a4;

    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_WAYCapaSetLink_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3, a4, a5);
    return a1 + 1;
}

int AI_EvalFunc_WAYCapaGetLink_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2)
{
    return (uint16_t)AI_WayGetLink(network, node1, node2)->capa;
}

tAINode* AI_EvalFunc_WAYCapaGetLink(tAINode* a1)
{
    int v1;
    tGameObjectMain* a2;
    tGameObjectMain* a3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_WAYCapaGetLink_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_WAYDesignSetLink_C(tWayNetwork* network, tGameObjectMain* node1, tGameObjectMain* node2, char designFlagSet, char designFlagRemove)
{
    tWayElemInnerElem* link = AI_WayGetLink(network, node1, node2);

    link->design &= ~designFlagRemove;
    link->design |= designFlagSet;
}

tAINode* AI_EvalFunc_WAYDesignSetLink(tAINode* a1)
{
    tGameObjectMain* a2;
    tGameObjectMain* a3;
    char a5;
    char a4;

    a5 = (char)AI_GetIntFromStack();
    a4 = (char)AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_WAYDesignSetLink_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3, a4, a5);
    return a1 + 1;
}

int AI_EvalFunc_WAYDesignGetLink_C(tWayNetwork* a1, tGameObjectMain* a2, tGameObjectMain* a3)
{
    return (char)AI_WayGetLink(a1, a2, a3)->design;
}

tAINode* AI_EvalFunc_WAYDesignGetLink(tAINode* a1)
{
    int v1;
    tGameObjectMain* a2;
    tGameObjectMain* a3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_WAYDesignGetLink_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
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

tAINode* AI_EvalFunc_WAYLinkSetWeight(tAINode* a1)
{
    tGameObjectMain* a2;
    tGameObjectMain* a3;
    float a4;

    a4 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_WAYLinkSetWeight_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3, a4);
    return a1 + 1;
}

float AI_EvalFunc_WAYLinkGetWeight_C(tWayNetwork* a1, tGameObjectMain* a2, tGameObjectMain* a3)
{
    return AI_WayGetUnorientedLink(a1, a2, a3)->weight;
}

tAINode* AI_EvalFunc_WAYLinkGetWeight(tAINode* a1)
{
    float v2;
    tGameObjectMain* a2;
    tGameObjectMain* a3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = AI_EvalFunc_WAYLinkGetWeight_C(*(tWayNetwork**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, a3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}