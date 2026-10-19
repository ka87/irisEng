#include "memory.h"
#include "loading.h"
#include "gameobject.h"
#include "snp.h"

tSnpAxis* INT_SnP_pstCreateAxisTable()
{
    tSnpAxis* axisTable;
    tSnpAxisElem* axisElem;

    axisTable = (tSnpAxis*)MEM_p_Alloc(sizeof(tSnpAxis));
    LOA_ul_FileTypeSize_38 += sizeof(tSnpAxis);
    
    memset(axisTable, 0, sizeof(tSnpAxis));
    
    axisTable->elems = (tSnpAxisElem*)MEM_p_Alloc(2048 * sizeof(tSnpAxisElem));
    LOA_ul_FileTypeSize_38 += 2048 * sizeof(tSnpAxisElem);
    
    axisElem = axisTable->elems;
    axisElem->gao = 0;
    axisElem->gaoSnPIndex = 0;
    axisElem->flags = 0;
    axisElem->gap = 0;
    axisElem->ptrToBVAxisFloat = 0;
    axisElem->posPlusBVSetIn40DB10 = 0.0f;
    
    return axisTable;
}

void INT_SnP_Alloc(struct sWorld* world)
{
    world->snp = (tSnp*)MEM_p_Alloc(sizeof(tSnp));
    LOA_ul_FileTypeSize_38 += sizeof(tSnp);

    memset(world->snp, 0, sizeof(tSnp));
    
    world->snp->snpObjectsRemoveQueue = (tSnpObjectsToRemoveQueue*)MEM_p_Alloc(sizeof(tSnpObjectsToRemoveQueue));
    LOA_ul_FileTypeSize_38 += sizeof(tSnpObjectsToRemoveQueue);
    
    memset(world->snp->snpObjectsRemoveQueue, 0, sizeof(tSnpObjectsToRemoveQueue));
    
    world->snp->axisPtr[0] = INT_SnP_pstCreateAxisTable();
    world->snp->axisPtr[1] = INT_SnP_pstCreateAxisTable();
    world->snp->axisPtr[2] = INT_SnP_pstCreateAxisTable();
}

void INT_SnP_DesAlloc(struct sWorld* world)
{
    if (world->snp)
    {
        MEM_Free(world->snp->axisPtr[0]->elems);
        MEM_Free(world->snp->axisPtr[0]);
        MEM_Free(world->snp->axisPtr[1]->elems);
        MEM_Free(world->snp->axisPtr[1]);
        MEM_Free(world->snp->axisPtr[2]->elems);
        MEM_Free(world->snp->axisPtr[2]);
        MEM_Free(world->snp->snpObjectsRemoveQueue);
        MEM_Free(world->snp);
    }
}

void INT_SnP_Reinit(struct sWorld* world)
{
    if (world->snp)
    {
        world->snp->currElemIdx = 0;
        memset(world->snp->gaosPtr, 0, sizeof(world->snp->gaosPtr));
        memset(world->snp->axisPtr[0]->flagsArray, 0, sizeof(world->snp->axisPtr[0]->flagsArray));
        memset(world->snp->axisPtr[1]->flagsArray, 0, sizeof(world->snp->axisPtr[1]->flagsArray));
        memset(world->snp->axisPtr[2]->flagsArray, 0, sizeof(world->snp->axisPtr[2]->flagsArray));
        world->SnPRefreshNeeded = TRUE;
    }
}

int INT_SnP_GetFlag(int* flagArr, char offset, int idx)
{
    return flagArr[idx] & (1 << offset);
}

void INT_SnP_SetFlag(int* flagArr, char offset, int idx, int addOrRemove)
{
    int bit, origVal;

    bit = 1 << offset;
    origVal = flagArr[idx];

    if (addOrRemove)
        flagArr[idx] = bit | origVal;
    else
        flagArr[idx] = ~bit & origVal;
}

int INT_SnP_FullGetFlag(int* a1, int a2, int a3, int a4, int* a5, int* a6)
{
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;

    v6 = a2;
    v7 = a2;
    if (a2 < a3)
        v7 = a3;
    if (a2 > a3)
        v6 = a3;
    v8 = v6 * (a4 - 1) - (((v6 + 1) * (v6 + 1) - v6 - 1) >> 1) + v7 - 1;
    v9 = v8 >> 5;
    *a6 = v8 >> 5;
    v10 = v8 - 32 * (v8 >> 5);
    *a5 = v10;
    return (1 << v10) & a1[v9];
}

int INT_SnP_FullSetFlag(int* flagPtr, int idx, int currIdx, int limit, int* a5, int* a6, int a7)
{
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int result;

    v7 = idx;
    v8 = idx;
    if (idx < currIdx)
        v8 = currIdx;
    if (idx > currIdx)
        v7 = currIdx;
    v9 = v7 * (limit - 1) - (((v7 + 1) * (v7 + 1) - v7 - 1) >> 1) + v8 - 1;
    v10 = v9 >> 5;
    *a6 = v9 >> 5;
    v11 = v9 - 32 * (v9 >> 5);
    *a5 = v11;
    v12 = 1 << v11;
    v13 = flagPtr[v10];
    result = (1 << v11) & v13;
    if (a7)
        flagPtr[v10] = v12 | v13;
    else
        flagPtr[v10] = v13 & ~v12;
    return result;
}

void INT_SnP_ResetAllReferenceFlags(tSnp* a1, unsigned __int16 idx)
{
    unsigned __int16 v2;
    int i;
    int a3;
    int a2;

    v2 = 0;
    i = 0;
    do
    {
        if (idx != v2)
        {
            INT_SnP_FullSetFlag(a1->axisPtr[0]->flagsArray, idx, i, 1024, &a2, &a3, 0);
            INT_SnP_SetFlag(a1->axisPtr[1]->flagsArray, a2, a3, 0);
            INT_SnP_SetFlag(a1->axisPtr[2]->flagsArray, a2, a3, 0);
        }
        ++v2;
        ++i;
    } while (v2 < 1024u);
}

// TODO: megnezni a -1 esetet az addNode-dal
int16_t INT_SnP_GetFirstFreeIndex(tSnp* a1)
{
    tGameObjectMain** start;
    tGameObjectMain** curr;
    tGameObjectMain* v3;

    start = a1->gaosPtr;
    curr = a1->gaosPtr;
    if (a1->gaosPtr[0])
    {
        while (curr < &a1->gaosPtr[1023])
        {
            v3 = curr[1];
            ++curr;
            if (!v3)
                return curr - start;
        }
    }
    if (!*curr)
        return curr - start;

    return -1;
}

int INT_SnP_ObjectMustBeRemoved(tSnpObjectsToRemoveQueue* a1, tGameObjectMain* a2)
{
    tGameObjectMain** curr;
    tGameObjectMain** last;

    // andras
    if (a1->num <= 0) 
        return 0;
    // --
    curr = a1->gaos;
    last = &(a1->gaos[a1->num - 1]);
    if (a1->gaos > last)
        return 0;
    while (*curr != a2)
    {
        if (++curr > last)
            return 0;
    }
    return 1;
}

void INT_SnP_AddNode(tSnp* snp, tGameObjectMain* gao, Vec3* bv, char flags)
{
    int16_t v4;
    tSnp* v5;
    int v6;
    tSnpAxisElem* v7;
    uint16_t v8;
    Mtx4x4* v9;
    Vec3* gaoPos;
    double v11;
    double v12;
    byte isSphere;
    float v13;
    int16_t v14;
    Vec3 a1;

    v4 = 0;
    v14 = 0;
    isSphere = flags & 8;                   // sphere

    while (1)
    {
        v5 = snp;
        v6 = v4;
        v7 = &snp->axisPtr[v4]->elems[snp->currElemIdx];
        v7->gao = gao;
        if (!v4 && (flags & 1) != 0)
        {
            v8 = INT_SnP_GetFirstFreeIndex(snp);
            gao->snpIndex = v8;
            INT_SnP_ResetAllReferenceFlags(snp, v8);
            snp->gaosPtr[gao->snpIndex] = gao;
            v5 = snp;
        }
        v7->gaoSnPIndex = gao->snpIndex;
        v7->flags = flags;
        if (isSphere)
        {
            v13 = gao->boundingVolume->BVMax.x;
            v9 = OBJ_pst_GetAbsoluteMatrix(gao);
            MATH_TransformVertexNoScale(&a1, v9, bv);
            v5 = snp;
            v7->ptrToBVAxisFloat = &bv->x;
        }
        gaoPos = &gao->matrices->Pos;
        if (v6)
        {
            if (v6 == 1)
            {
                if (isSphere)
                {
                    v11 = a1.y;
                    if ((flags & 1) == 0) {
                        v12 = v11 + v13;
                        v7->posPlusBVSetIn40DB10 = (float)v12;
                        goto next;
                    }
                    v12 = v11 - v13;
                }
                else
                {
                    v7->ptrToBVAxisFloat = &bv->y;
                    v12 = gaoPos->y + bv->y;
                }
            }
            else
            {
                if (v6 != 2)
                    goto next;
                if (isSphere)
                {
                    v11 = a1.z;
                    if ((flags & 1) == 0) {
                        v12 = v11 + v13;
                        v7->posPlusBVSetIn40DB10 = (float)v12;
                        goto next;
                    }
                    v12 = v11 -v13;
                }
                else
                {
                    v7->ptrToBVAxisFloat = &bv->z;
                    v12 = gaoPos->z + bv->z;
                }
            }
        }
        else
        {
            if (!isSphere)
            {
                v7->ptrToBVAxisFloat = &bv->x;
                v12 = gaoPos->x + bv->x;
                v7->posPlusBVSetIn40DB10 = (float)v12;
                goto next;
            }
            v11 = a1.x;
            if ((flags & 1) == 0)
            {
                v12 = v11 + v13;
                v7->posPlusBVSetIn40DB10 = (float)v12;
                goto next;
            }
            v12 = v11 - v13;
        }
        v7->posPlusBVSetIn40DB10 = (float)v12;
    next:
        if (++v14 > 2)
            break;
        v4 = v14;
    }
    ++v5->currElemIdx;
}

void INT_SnP_AddBox(tSnp* snp, tGameObjectMain* gao, Vec3* bvMin, Vec3* bvMax)
{
    INT_SnP_AddNode(snp, gao, bvMin, 0x11);
    INT_SnP_AddNode(snp, gao, bvMax, 0x12);
}

void INT_SnP_AddSphere(tSnp* snp, tGameObjectMain* gao)
{
    INT_SnP_AddNode(snp, gao, &gao->boundingVolume->BVMin, 0x9);
    INT_SnP_AddNode(snp, gao, &gao->boundingVolume->BVMin, 0xA);
}

void INT_SnP_AttachObject(struct sGameObjectMain* gao, struct sWorld* world)
{
    if (gao->snpIndex == 0xffff && (gao->flags2 & 0x80) != 0 && 
        world->snp->currElemIdx < 1024)
    {
        if (OBJ_BV_IsAABBox(gao->boundingVolume))
            INT_SnP_AddBox(world->snp, gao, &gao->boundingVolume->BVMin, &gao->boundingVolume->BVMax);
        else
            INT_SnP_AddSphere(world->snp, gao);
    }
}

void INT_SnP_AddObject(struct sGameObjectMain* gao, struct sSnp* world)
{    
    if (gao->snpIndex == 0xffff && world->currElemIdx < 1024)
    {
        if (OBJ_BV_IsAABBox(gao->boundingVolume))
            INT_SnP_AddBox(world, gao, &gao->boundingVolume->BVMin, &gao->boundingVolume->BVMax);
        else
            INT_SnP_AddSphere(world, gao);
    }
}

void INT_SnP_Refresh(struct sSnp* snp, tableDescStruct* table)
{
    tableElemStructPF* currElem;
    tGameObjectMain* gao;

    snp->snpObjectsRemoveQueue->num = 0;

    for (currElem = table->firstElemPtr; currElem <= ((tableElemStructPF*)(table->nextElemPtr)) - 1; currElem++)
    {
        gao = (tGameObjectMain*)currElem->objectPtr;
        if (!TAB_TableElemIsInvalid(currElem->objectPtr) && (gao->flags2 & 0x80) != 0)
            INT_SnP_AddObject(gao, snp);
    }
}

void INT_SnP_AddOverlap(tGameObjectMain* gao1, tGameObjectMain* gao2)
{
    tGameObjectMain** currGaoInDetectionList;
    tGameObjectMain** lastGaoInDetectionList;

    if ((gao1->flags2 & 0x20) != 0)
    {
        if (!gao1->extended || !gao1->extended->coll || !gao1->extended->coll->detectionList)
            return;

        currGaoInDetectionList = gao1->extended->coll->detectionList->detectionListGaos;
        if (!gao1->extended->coll->detectionList->overlapNum) {
            gao1->extended->coll->detectionList->detectionListGaos[gao1->extended->coll->detectionList->overlapNum] = gao2;
            ++gao1->extended->coll->detectionList->overlapNum;
            goto PairAdd;
        }

        lastGaoInDetectionList = &currGaoInDetectionList[gao1->extended->coll->detectionList->overlapNum - 1];
        if (*currGaoInDetectionList != gao2)
        {
            while (currGaoInDetectionList != lastGaoInDetectionList)
            {
                ++currGaoInDetectionList;
                if (*currGaoInDetectionList == gao2)
                    goto PairAdd;
            }
            if (*currGaoInDetectionList != gao2)
            {
                if (gao1->extended->coll->detectionList->overlapNum >= 150)
                    gao1->extended->coll->detectionList->detectionListGaos = (tGameObjectMain**)MEM_p_Realloc(gao1->extended->coll->detectionList->detectionListGaos,
                        sizeof(tGameObjectMain**) * (gao1->extended->coll->detectionList->overlapNum + 1));

                gao1->extended->coll->detectionList->detectionListGaos[gao1->extended->coll->detectionList->overlapNum] = gao2;
                ++gao1->extended->coll->detectionList->overlapNum;
                goto PairAdd;
            }
        }
    }

PairAdd:
    if ((gao2->flags2 & 0x20) != 0)
    {
        if (!gao2->extended || !gao2->extended->coll || !gao2->extended->coll->detectionList)
            return;

        currGaoInDetectionList = gao2->extended->coll->detectionList->detectionListGaos;
        if (!gao2->extended->coll->detectionList->overlapNum)
        {
            gao2->extended->coll->detectionList->detectionListGaos[gao2->extended->coll->detectionList->overlapNum] = gao1;
            ++gao2->extended->coll->detectionList->overlapNum;
            return;
        }

        lastGaoInDetectionList = &currGaoInDetectionList[gao2->extended->coll->detectionList->overlapNum - 1];
        if (*currGaoInDetectionList != gao1)
        {
            while (currGaoInDetectionList != lastGaoInDetectionList)
            {
                ++currGaoInDetectionList;
                if (*currGaoInDetectionList == gao1)
                    return;
            }
            if (*currGaoInDetectionList != gao1)
            {
                if (gao2->extended->coll->detectionList->overlapNum >= 150)
                    gao2->extended->coll->detectionList->detectionListGaos = (tGameObjectMain**)MEM_p_Realloc(gao2->extended->coll->detectionList->detectionListGaos,
                        sizeof(tGameObjectMain**) * (gao2->extended->coll->detectionList->overlapNum + 1));

                gao2->extended->coll->detectionList->detectionListGaos[gao2->extended->coll->detectionList->overlapNum] = gao1;
                ++gao2->extended->coll->detectionList->overlapNum;
                return;
            }
        }
    }
}

void INT_SnP_RemoveOverlap(tGameObjectMain* a1, tGameObjectMain* a2)
{
    tGameObjectExtended* v2;
    tGameObjectCollDetectionList** v3;
    tGameObjectCollDetectionList* v4;
    int v5;
    tGameObjectMain** v6;
    tGameObjectMain** v7;
    tGameObjectMain* v8; 
    tGameObjectExtended* v9;
    tGameObjectCollDetectionList** v10;
    tGameObjectCollDetectionList* v11;
    int v12;
    tGameObjectMain** v13;
    tGameObjectMain** v14;
    tGameObjectMain* v15;

    if ((a1->flags2 & 0x20) != 0)
    {
        v2 = a1->extended;
        if (!v2)
            return;
        v3 = &v2->coll->detectionList;
        if (!v3)
            return;
        v4 = *v3;
        if (!v4)
            return;
        v5 = v4->overlapNum;
        v6 = v4->detectionListGaos;
        if (v5)
        {
            v7 = &v6[v5 - 1];
            if (*v6 == a2)
                goto LABEL_11;
            while (v6 != v7)
            {
                v8 = v6[1];
                ++v6;
                if (v8 == a2)
                    goto LABEL_11;
            }
            if (*v6 == a2)
            {
            LABEL_11:
                memcpy(v6, v6 + 1, 4 * ((((char*)v7 - (char*)v6) & 0xFFFFFFFC) >> 2));
                --v4->overlapNum;
            }
        }
    }
    if ((a2->flags2 & 0x20) != 0)
    {
        v9 = a2->extended;
        if (v9)
        {
            v10 = &v9->coll->detectionList;
            if (v10)
            {
                v11 = *v10;
                if (v11)
                {
                    v12 = v11->overlapNum;
                    v13 = v11->detectionListGaos;
                    if (v12)
                    {
                        v14 = &v13[v12 - 1];
                        if (*v13 == a1)
                            goto LABEL_22;
                        while (v13 != v14)
                        {
                            v15 = v13[1];
                            ++v13;
                            if (v15 == a1)
                                goto LABEL_22;
                        }
                        if (*v13 == a1)
                        {
                        LABEL_22:
                            memcpy(v13, v13 + 1, 4 * ((((char*)v14 - (char*)v13) & 0xFFFFFFFC) >> 2));
                            --v11->overlapNum;
                        }
                    }
                }
            }
        }
    }
}

void INT_SnP_UpdateAllDetectionLists(tSnp* a1)
{
    tSnp* v1;
    tSnpObjectsToRemoveQueue* v2;
    tGameObjectMain** v3;
    tGameObjectMain* v4;
    tGameObjectCollDetectionList* v5;
    tGameObjectMain** v6;
    tGameObjectMain** v7;
    unsigned int v8;
    unsigned __int16 v9;
    int v10;
    unsigned __int16 v11;
    int a2;
    tGameObjectMain** v13;
    tGameObjectMain** v14;
    int xx;

    v1 = a1;
    v2 = a1->snpObjectsRemoveQueue;
    v3 = v2->gaos;
    v2->queueNum = 0;
    v14 = &v2->gaos[v2->num];
    if (v2->gaos < v14)
    {
        do
        {
            v4 = *v3;
            if ((*v3)->snpIndex != 0xFFFF)
            {
                if ((v4->flags2 & 0x20) != 0)
                {
                    v5 = v4->extended->coll->detectionList;
                    v6 = v5->detectionListGaos;
                    v7 = &v5->detectionListGaos[v5->overlapNum];
                    if (v6 < v7)
                    {
                        v8 = ((unsigned int)((char*)v7 - (char*)v6 - 1) >> 2) + 1;
                        do
                        {
                            INT_SnP_FullSetFlag(
                                v1->axisPtr[0]->flagsArray,
                                (*v3)->snpIndex,
                                (*v6)->snpIndex,
                                1024,
                                &a2,
                                &xx,
                                0);
                            INT_SnP_SetFlag(v1->axisPtr[1]->flagsArray, a2, xx, 0);
                            INT_SnP_SetFlag(v1->axisPtr[2]->flagsArray, a2, xx, 0);
                            if (*v6)
                                INT_SnP_RemoveOverlap(*v3, *v6);
                            --v8;
                        } while (v8);
                    }
                }
                else
                {
                    v9 = 0;
                    v10 = 0;
                    v13 = v1->gaosPtr;
                    do
                    {
                        v11 = (*v3)->snpIndex;
                        if (v11 != v9)
                        {
                            if (INT_SnP_FullSetFlag(v1->axisPtr[0]->flagsArray, v11, v10, 1024, &a2, &xx, 0)
                                && INT_SnP_GetFlag(v1->axisPtr[1]->flagsArray, a2, xx)
                                && INT_SnP_GetFlag(v1->axisPtr[2]->flagsArray, a2, xx)
                                && *v13)
                            {
                                INT_SnP_RemoveOverlap(*v3, *v13);
                            }
                            INT_SnP_SetFlag(v1->axisPtr[1]->flagsArray, a2, xx, 0);
                            INT_SnP_SetFlag(v1->axisPtr[2]->flagsArray, a2, xx, 0);
                        }
                        ++v9;
                        ++v10;
                        ++v13;
                    } while (v9 < 1024u);
                }
            }
            ++v3;
        } while (v3 < v14);
    }
}

void INT_RegisterObjectForRemoving(tSnpObjectsToRemoveQueue* a1, tGameObjectMain* a2)
{
    a1->gaos[a1->num++] = a2;
}

void INT_SnP_UpdateAxisTable(tSnp* snp, int a2)
{
    tSnp* ssnp;
    tSnpObjectsToRemoveQueue* orq;
    tSnpAxisElem* axisElemStartPtr;
    tSnpAxisElem* axisElemLastPtr;
    tSnpAxisElem* aes;
    uint16_t* currElem;
    uint16_t* lastElem;
    int16_t v9;
    uint16_t i;
    uint16_t* llastElem;

    ssnp = snp;
    orq = snp->snpObjectsRemoveQueue;
    orq->queueNum = 0;
    axisElemStartPtr = snp->axisPtr[a2]->elems;
    axisElemLastPtr = &axisElemStartPtr[snp->currElemIdx - 1];
    aes = axisElemStartPtr;
    i = 0;
    if (axisElemStartPtr <= axisElemLastPtr)
    {
        do
        {
            if (INT_SnP_ObjectMustBeRemoved(orq, aes->gao))
                orq->elemIdxes[orq->queueNum++] = i;
            ++aes;
            ++i;
        } while (aes <= axisElemLastPtr);
        ssnp = snp;
    }
    currElem = orq->elemIdxes;
    lastElem = &(orq->elemIdxes[orq->queueNum - 1]); // fixed, off by one
    orq->destPos = orq->elemIdxes[0];
    llastElem = lastElem;
    if (orq->elemIdxes <= lastElem - 1)
    {
        do
        {
            memcpy(&axisElemStartPtr[orq->destPos], &axisElemStartPtr[*currElem + 1], 16 * (currElem[1] - *currElem - 1));
            v9 = currElem[1] - *currElem;
            ++currElem;
            orq->destPos += v9 - 1;
            lastElem = llastElem;
        } while (currElem <= llastElem - 1);
        ssnp = snp;
    }
    if (orq->queueNum)
        memcpy(&axisElemStartPtr[orq->destPos], &axisElemStartPtr[*lastElem + 1], 16 * (ssnp->currElemIdx - *lastElem - 1));
}

void INT_SnP_DetachObject(struct sGameObjectMain* gao, struct sWorld* world)
{
    int i;

    if (world->snp && world->snp->currElemIdx)
    {
        world->snp->snpObjectsRemoveQueue->num = 0;
        INT_RegisterObjectForRemoving(world->snp->snpObjectsRemoveQueue, gao);
        INT_SnP_UpdateAllDetectionLists(world->snp);
        INT_SnP_UpdateAxisTable(world->snp, 0);
        INT_SnP_UpdateAxisTable(world->snp, 1);
        INT_SnP_UpdateAxisTable(world->snp, 2);

        for (i = 0; i < 1024; i++) {
            if (world->snp->gaosPtr[i] == gao) {
                world->snp->gaosPtr[i] = NULL;
                gao->snpIndex = 0xffff;
                world->snp->currElemIdx -= 2 * world->snp->snpObjectsRemoveQueue->num;
                break;
            }
        }
    }
}

void INT_SnP_UnknownInversionSortInner(tSnpAxisElem* a1, int type)
{
    tGameObjectMain* v3;
    Vec3* v4;
    double v5;
    Mtx4x4* v6;
    double v7;
    double v8;
    Vec3* v9;
    Vec3 v10;
    float a1a;

    v3 = a1->gao;
    if ((a1->flags & 0x10) != 0)
    {
        v4 = &v3->matrices->Pos;
        if (type)
        {
            if (type == 1)
                v5 = v4->y + *a1->ptrToBVAxisFloat;
            else
                v5 = v4->z + *a1->ptrToBVAxisFloat;
            a1->posPlusBVSetIn40DB10 = (float)v5;
        }
        else
        {
            a1->posPlusBVSetIn40DB10 = v4->x + *a1->ptrToBVAxisFloat;
        }
    }
    else
    {
        v9 = (Vec3*)a1->ptrToBVAxisFloat; // ????? TODO FIXME
        a1a = v3->boundingVolume->BVMax.x;
        v6 = OBJ_pst_GetAbsoluteMatrix(v3);
        MATH_TransformVertexNoScale(&v10, v6, v9);
        if (type)
        {
            if (type == 1)
                v7 = v10.y;
            else
                v7 = v10.z;
        }
        else
        {
            v7 = v10.x;
        }
        if ((a1->flags & 1) != 0)
            v8 = v7 - a1a;
        else
            v8 = v7 + a1a;
        a1->posPlusBVSetIn40DB10 = (float)v8;
    }
}

void INT_SnP_ComputeInversion(tSnp* a1, int a2, int a3, int a4, int a5, int a6)
{
    int v6;
    tSnp* v7;
    int v8;
    int v9;
    tSnp* v10;
    int v11;
    int v12;
    int xx;

    if (a6)
    {
        if (!a5)
        {
            v6 = a2;
            v7 = a1;
            v8 = a4;
            v9 = a2 + 2;
            if (!INT_SnP_FullSetFlag(a1->axisPtr[a2]->flagsArray, a3, a4, 1024, &a6, &xx, 1))
            {
                if (INT_SnP_GetFlag(v7->axisPtr[(v6 + 1) % 3]->flagsArray, a6, xx))
                {
                    if (INT_SnP_GetFlag(v7->axisPtr[v9 % 3]->flagsArray, a6, xx))
                        INT_SnP_AddOverlap(v7->gaosPtr[a3], v7->gaosPtr[v8]);
                }
            }
        }
    }
    else if (a5)
    {
        v10 = a1;
        v11 = a4;
        v12 = a3;
        if (INT_SnP_FullSetFlag(a1->axisPtr[a2]->flagsArray, a3, a4, 1024, &a6, &xx, 0))
            INT_SnP_RemoveOverlap(v10->gaosPtr[v12], v10->gaosPtr[v11]);
    }
}
// FIXME: megnézni megy-e, meg rájönni hogy mûködik
void INT_SnP_InsertionSort(struct sSnp* a1, int a2)
{
    int v2;
    uint16_t v3;
    int8_t v3f;
    int8_t v3g;
    tSnpAxisElem* v4;
    tSnpAxisElem* v5;
    tSnpAxisElem* v6;
    tSnpAxisElem* v7;
    tSnpAxisElem* v8;
    double v9;
    BOOL v10;
    int v11;
    tSnpAxisElem* v12;
    int a2a;
    int v14;
    tSnpAxis** v15;
    float v16;
    unsigned int v17;
    int v18;
    float v19;
    tSnpAxisElem* v20;
    tGameObjectMain* v21 = NULL;
    float* v23 = NULL;
    float v24;

    if (!a1->currElemIdx)
        return;
    if (!a2) // 0
    {
        v11 = 2;
        v2 = 0;
    } else if (a2 != 1)
    {
        if (a2 == 2)
        {
            v2 = 2;
            v11 = 2;
        }
        else {
            v11 = 2;
            v2 = 0;
        }
    }
    else {
        v11 = 1;
        v2 = 0;
    }
    a2a = v2;
    if (v2 <= v11)
    {
//        v3 = v22;
        v18 = 16 * a1->currElemIdx - 16;
        v15 = &a1->axisPtr[v2];
        do
        {
            v20 = (*v15)->elems;
            v4 = v20;
            INT_SnP_UnknownInversionSortInner(v20, v2);
            v16 = v20->posPlusBVSetIn40DB10;
            if (v20 < &((*v15)->elems[a1->currElemIdx -1]))
            {
                v5 = v20; // v20->posPlusBVSetIn40DB10; + 12
                v17 = ((unsigned int)(v18 - 1) >> 4) + 1;
                while (1)
                {
                    v12 = &v5[1]; //v12 = v5 + 16;
                    v6 = &v5[1]; // fix ---> it was //v6 = &v12[1]; //(tSnpAxisElem*)(v5 + 4);

                    INT_SnP_UnknownInversionSortInner(v6, a2a);
                    v7 = v12;
                    if (v12->posPlusBVSetIn40DB10 + 0.001 < v16)
                    {
                        v8 = &v12[-1]; //(tSnpAxisElem*)(v12 - 28);
                        v9 = v12->posPlusBVSetIn40DB10 + 0.001;
                        v14 = 1;
                        if (v9 < v12[-1].posPlusBVSetIn40DB10) //*(float*)(v12 - 28 + 12))
                        {
                            do
                            {
                                if (v14)
                                {
                                    v3 = v6->gaoSnPIndex;
                                    v3f = v6->flags;
                                    v3g = v6->gap;
                                    v21 = v6->gao;
                                    v23 = v6->ptrToBVAxisFloat;
                                    v24 = v6->posPlusBVSetIn40DB10;
                                    v14 = 0;
                                }
                                INT_SnP_ComputeInversion(a1, a2a, v8->gaoSnPIndex, v3, v8->flags & 1, v3f & 1);
                                if (--v8 < v4)
                                    break;
                                v19 = (float)v9;
                            } while (v8->posPlusBVSetIn40DB10 > (double)v19);
                            memcpy(&v8[2], &v8[1], 16 * (v6 - v8 - 1));
                            v8[1].gao = v21;
                            v8[1].gaoSnPIndex = v3;
                            v8[1].flags = v3f;
                            v8[1].gap = v3g;
                            v8[1].ptrToBVAxisFloat = v23;
                            v8[1].posPlusBVSetIn40DB10 = v24;
                            v4 = v20;
                            v7 = v12;
                        }
                    }
                    v10 = v17 == 1;
                    v16 = v7->posPlusBVSetIn40DB10; //v16 = *(float*)v7;
                    --v17;
                    if (v10)
                        break;
                    v5 = v12;
                }
                v2 = a2a;
            }
            a2a = ++v2;
            ++v15;
        } while (v2 <= v11);
    }
}