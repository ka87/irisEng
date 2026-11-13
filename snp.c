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

int INT_SnP_GetFlag(int* flagArr, char bit, int word)
{
    return flagArr[word] & (1 << bit);
}

void INT_SnP_SetFlag(int* flagArr, char bit, int word, int addOrRemove)
{
    int mask, origVal;

    mask = 1 << bit;
    origVal = flagArr[word];

    if (addOrRemove)
        flagArr[word] = mask | origVal;
    else
        flagArr[word] = ~mask & origVal;
}

static int INT_SnP_l_PairBitIndex(int snpIndexA, int snpIndexB, int count)
{
    int lo;
    int hi;

    lo = snpIndexA;
    hi = snpIndexA;
    if (snpIndexA < snpIndexB)
        hi = snpIndexB;
    if (snpIndexA > snpIndexB)
        lo = snpIndexB;

    return lo * (count - 1) - (((lo + 1) * (lo + 1) - lo - 1) >> 1) + hi - 1;
}

int INT_SnP_FullGetFlag(int* flagArr, int snpIndexA, int snpIndexB, int count, int* bitOut, int* wordOut)
{
    int bitIndex;
    int bit;
    int word;

    bitIndex = INT_SnP_l_PairBitIndex(snpIndexA, snpIndexB, count);
    word = bitIndex >> 5;
    *wordOut = word;
    bit = bitIndex & 31;
    *bitOut = bit;

    return (1 << bit) & flagArr[word];
}

int INT_SnP_FullSetFlag(int* flagArr, int snpIndexA, int snpIndexB, int count, int* bitOut, int* wordOut, int addOrRemove)
{
    int bitIndex;
    int bit;
    int word;
    int mask;
    int origVal;
    int result;

    bitIndex = INT_SnP_l_PairBitIndex(snpIndexA, snpIndexB, count);
    word = bitIndex >> 5;
    *wordOut = word;
    bit = bitIndex & 31;
    *bitOut = bit;

    mask = 1 << bit;
    origVal = flagArr[word];
    result = mask & origVal;
    if (addOrRemove)
        flagArr[word] = mask | origVal;
    else
        flagArr[word] = origVal & ~mask;

    return result;
}

void INT_SnP_ResetAllReferenceFlags(tSnp* snp, uint16_t snpIndex)
{
    int other;
    int bit;
    int word;

    for (other = 0; other < 1024; other++)
    {
        if (snpIndex != other)
        {
            INT_SnP_FullSetFlag(snp->axisPtr[0]->flagsArray, snpIndex, other, 1024, &bit, &word, 0);
            INT_SnP_SetFlag(snp->axisPtr[1]->flagsArray, bit, word, 0);
            INT_SnP_SetFlag(snp->axisPtr[2]->flagsArray, bit, word, 0);
        }
    }
}

// TODO: megnezni a -1 esetet az addNode-dal
int16_t INT_SnP_GetFirstFreeIndex(tSnp* snp)
{
    int i;

    for (i = 0; i < 1024; i++)
    {
        if (!snp->gaosPtr[i])
            return i;
    }

    return -1;
}

int INT_SnP_ObjectMustBeRemoved(tSnpObjectsToRemoveQueue* queue, tGameObjectMain* gao)
{
    int i;

    // andras
    if (queue->num <= 0) 
        return 0;
    // --
    for (i = 0; i < queue->num; i++)
    {
        if (queue->gaos[i] == gao)
            return 1;
    }

    return 0;
}

void INT_SnP_AddNode(tSnp* snp, tGameObjectMain* gao, Vec3* bv, char flags)
{
    tSnpAxisElem* elem;
    Mtx4x4* absMtx;
    Vec3* gaoPos;
    Vec3 bvAbs;
    double axisPos;
    float radius;
    uint16_t freeIndex;
    uint8_t isSphere;
    int axis;

    radius = 0.0f;
    isSphere = flags & 8;                   // sphere

    for (axis = 0; axis <= 2; axis++)
    {
        elem = &snp->axisPtr[axis]->elems[snp->currElemIdx];
        elem->gao = gao;
        if (axis == 0 && (flags & 1) != 0)
        {
            freeIndex = INT_SnP_GetFirstFreeIndex(snp);
            gao->snpIndex = freeIndex;
            INT_SnP_ResetAllReferenceFlags(snp, freeIndex);
            snp->gaosPtr[gao->snpIndex] = gao;
        }
        elem->gaoSnPIndex = gao->snpIndex;
        elem->flags = flags;

        if (isSphere)
        {
            radius = gao->boundingVolume->BVMax.x;
            absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
            MATH_TransformVertexNoScale(&bvAbs, absMtx, bv);
            elem->ptrToBVAxisFloat = &bv->x;

            if (axis == 0)
                axisPos = bvAbs.x;
            else if (axis == 1)
                axisPos = bvAbs.y;
            else
                axisPos = bvAbs.z;

            if ((flags & 1) != 0)
                axisPos = axisPos - radius;
            else
                axisPos = axisPos + radius;
        }
        else
        {
            gaoPos = &gao->matrices->Pos;
            if (axis == 0)
            {
                elem->ptrToBVAxisFloat = &bv->x;
                axisPos = gaoPos->x + bv->x;
            }
            else if (axis == 1)
            {
                elem->ptrToBVAxisFloat = &bv->y;
                axisPos = gaoPos->y + bv->y;
            }
            else
            {
                elem->ptrToBVAxisFloat = &bv->z;
                axisPos = gaoPos->z + bv->z;
            }
        }
        elem->posPlusBVSetIn40DB10 = (float)axisPos;
    }
    ++snp->currElemIdx;
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

void INT_SnP_AddObject(struct sGameObjectMain* gao, struct sSnp* snp)
{    
    if (gao->snpIndex == 0xffff && snp->currElemIdx < 1024)
    {
        if (OBJ_BV_IsAABBox(gao->boundingVolume))
            INT_SnP_AddBox(snp, gao, &gao->boundingVolume->BVMin, &gao->boundingVolume->BVMax);
        else
            INT_SnP_AddSphere(snp, gao);
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

static BOOL INT_SnP_l_AddToDetectionList(tGameObjectMain* gao, tGameObjectMain* other)
{
    tGameObjectCollDetectionList* list;
    int i;

    if ((gao->flags2 & 0x20) == 0)
        return TRUE;

    if (!gao->extended || !gao->extended->coll || !gao->extended->coll->detectionList)
        return FALSE;

    list = gao->extended->coll->detectionList;
    for (i = 0; i < list->overlapNum; i++)
    {
        if (list->detectionListGaos[i] == other)
            return TRUE;
    }

    if (list->overlapNum >= 150)
        list->detectionListGaos = (tGameObjectMain**)MEM_p_Realloc(list->detectionListGaos,
            sizeof(tGameObjectMain**) * (list->overlapNum + 1));

    list->detectionListGaos[list->overlapNum] = other;
    ++list->overlapNum;

    return TRUE;
}

void INT_SnP_AddOverlap(tGameObjectMain* gao1, tGameObjectMain* gao2)
{
    if (!INT_SnP_l_AddToDetectionList(gao1, gao2))
        return;

    INT_SnP_l_AddToDetectionList(gao2, gao1);
}

static BOOL INT_SnP_l_RemoveFromDetectionList(tGameObjectMain* gao, tGameObjectMain* other)
{
    tGameObjectCollDetectionList* list;
    int i;

    if ((gao->flags2 & 0x20) == 0)
        return TRUE;

    if (!gao->extended || !gao->extended->coll || !gao->extended->coll->detectionList)
        return FALSE;

    list = gao->extended->coll->detectionList;
    for (i = 0; i < list->overlapNum; i++)
    {
        if (list->detectionListGaos[i] == other)
        {
            memcpy(&list->detectionListGaos[i], &list->detectionListGaos[i + 1],
                sizeof(tGameObjectMain*) * (list->overlapNum - 1 - i));
            --list->overlapNum;
            return TRUE;
        }
    }

    return TRUE;
}

void INT_SnP_RemoveOverlap(tGameObjectMain* gao1, tGameObjectMain* gao2)
{
    if (!INT_SnP_l_RemoveFromDetectionList(gao1, gao2))
        return;

    INT_SnP_l_RemoveFromDetectionList(gao2, gao1);
}

void INT_SnP_UpdateAllDetectionLists(tSnp* snp)
{
    tSnpObjectsToRemoveQueue* queue;
    tGameObjectCollDetectionList* list;
    tGameObjectMain** overlaps;
    tGameObjectMain* gao;
    int queued;
    int remaining;
    int other;
    int bit;
    int word;

    queue = snp->snpObjectsRemoveQueue;
    queue->queueNum = 0;

    for (queued = 0; queued < queue->num; queued++)
    {
        gao = queue->gaos[queued];
        if (gao->snpIndex == 0xFFFF)
            continue;

        if ((gao->flags2 & 0x20) != 0)
        {
            list = gao->extended->coll->detectionList;
            overlaps = list->detectionListGaos;
            for (remaining = list->overlapNum; remaining > 0; remaining--)
            {
                INT_SnP_FullSetFlag(
                    snp->axisPtr[0]->flagsArray,
                    gao->snpIndex,
                    overlaps[0]->snpIndex,
                    1024,
                    &bit,
                    &word,
                    0);
                INT_SnP_SetFlag(snp->axisPtr[1]->flagsArray, bit, word, 0);
                INT_SnP_SetFlag(snp->axisPtr[2]->flagsArray, bit, word, 0);
                if (overlaps[0])
                    INT_SnP_RemoveOverlap(gao, overlaps[0]);
            }
        }
        else
        {
            for (other = 0; other < 1024; other++)
            {
                if (gao->snpIndex == other)
                    continue;

                if (INT_SnP_FullSetFlag(snp->axisPtr[0]->flagsArray, gao->snpIndex, other, 1024, &bit, &word, 0)
                    && INT_SnP_GetFlag(snp->axisPtr[1]->flagsArray, bit, word)
                    && INT_SnP_GetFlag(snp->axisPtr[2]->flagsArray, bit, word)
                    && snp->gaosPtr[other])
                {
                    INT_SnP_RemoveOverlap(gao, snp->gaosPtr[other]);
                }
                INT_SnP_SetFlag(snp->axisPtr[1]->flagsArray, bit, word, 0);
                INT_SnP_SetFlag(snp->axisPtr[2]->flagsArray, bit, word, 0);
            }
        }
    }
}

void INT_RegisterObjectForRemoving(tSnpObjectsToRemoveQueue* queue, tGameObjectMain* gao)
{
    queue->gaos[queue->num++] = gao;
}

void INT_SnP_UpdateAxisTable(tSnp* snp, int axis)
{
    tSnpObjectsToRemoveQueue* queue;
    tSnpAxisElem* elems;
    int lastQueued;
    uint16_t i;
    int q;

    queue = snp->snpObjectsRemoveQueue;
    queue->queueNum = 0;
    elems = snp->axisPtr[axis]->elems;

    for (i = 0; i < snp->currElemIdx; i++)
    {
        if (INT_SnP_ObjectMustBeRemoved(queue, elems[i].gao))
            queue->elemIdxes[queue->queueNum++] = i;
    }

    lastQueued = queue->queueNum - 1; // fixed, off by one
    queue->destPos = queue->elemIdxes[0];

    for (q = 0; q < lastQueued; q++)
    {
        memcpy(&elems[queue->destPos], &elems[queue->elemIdxes[q] + 1],
            16 * (queue->elemIdxes[q + 1] - queue->elemIdxes[q] - 1));
        queue->destPos += queue->elemIdxes[q + 1] - queue->elemIdxes[q] - 1;
    }

    if (queue->queueNum)
        memcpy(&elems[queue->destPos], &elems[queue->elemIdxes[lastQueued] + 1],
            16 * (snp->currElemIdx - queue->elemIdxes[lastQueued] - 1));
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

void INT_SnP_UnknownInversionSortInner(tSnpAxisElem* elem, int axis)
{
    tGameObjectMain* gao;
    Vec3* gaoPos;
    Mtx4x4* absMtx;
    Vec3* bv;
    Vec3 bvAbs;
    double axisPos;
    float radius;

    gao = elem->gao;
    if ((elem->flags & 0x10) != 0)
    {
        gaoPos = &gao->matrices->Pos;
        if (axis == 0)
            axisPos = gaoPos->x + *elem->ptrToBVAxisFloat;
        else if (axis == 1)
            axisPos = gaoPos->y + *elem->ptrToBVAxisFloat;
        else
            axisPos = gaoPos->z + *elem->ptrToBVAxisFloat;
    }
    else
    {
        bv = (Vec3*)elem->ptrToBVAxisFloat; // ????? TODO FIXME
        radius = gao->boundingVolume->BVMax.x;
        absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_TransformVertexNoScale(&bvAbs, absMtx, bv);

        if (axis == 0)
            axisPos = bvAbs.x;
        else if (axis == 1)
            axisPos = bvAbs.y;
        else
            axisPos = bvAbs.z;

        if ((elem->flags & 1) != 0)
            axisPos = axisPos - radius;
        else
            axisPos = axisPos + radius;
    }
    elem->posPlusBVSetIn40DB10 = (float)axisPos;
}

void INT_SnP_ComputeInversion(tSnp* snp, int axis, int passedIndex, int movedIndex, int passedIsStart, int movedIsStart)
{
    int bit;
    int word;

    if (movedIsStart)
    {
        if (!passedIsStart)
        {
            if (!INT_SnP_FullSetFlag(snp->axisPtr[axis]->flagsArray, passedIndex, movedIndex, 1024, &bit, &word, 1)
                && INT_SnP_GetFlag(snp->axisPtr[(axis + 1) % 3]->flagsArray, bit, word)
                && INT_SnP_GetFlag(snp->axisPtr[(axis + 2) % 3]->flagsArray, bit, word))
            {
                INT_SnP_AddOverlap(snp->gaosPtr[passedIndex], snp->gaosPtr[movedIndex]);
            }
        }
    }
    else if (passedIsStart)
    {
        if (INT_SnP_FullSetFlag(snp->axisPtr[axis]->flagsArray, passedIndex, movedIndex, 1024, &bit, &word, 0))
            INT_SnP_RemoveOverlap(snp->gaosPtr[passedIndex], snp->gaosPtr[movedIndex]);
    }
}
// FIXME: megnézni megy-e, meg rájönni hogy mûködik
void INT_SnP_InsertionSort(struct sSnp* snp, int axisMode)
{
    tSnpAxisElem* elems;
    tGameObjectMain* movedGao;
    float* movedBVAxis;
    uint16_t movedIndex;
    int8_t movedFlags;
    int8_t movedGap;
    float movedKey;
    float prevKey;
    double keyPlusEps;
    float threshold;
    int firstAxis;
    int lastAxis;
    int axis;
    int i;
    int pos;

    if (!snp->currElemIdx)
        return;
    if (!axisMode) // 0
    {
        firstAxis = 0;
        lastAxis = 2;
    }
    else if (axisMode == 1)
    {
        firstAxis = 0;
        lastAxis = 1;
    }
    else if (axisMode == 2)
    {
        firstAxis = 2;
        lastAxis = 2;
    }
    else
    {
        firstAxis = 0;
        lastAxis = 2;
    }

//        v3 = v22;
    for (axis = firstAxis; axis <= lastAxis; axis++)
    {
        elems = snp->axisPtr[axis]->elems;
        INT_SnP_UnknownInversionSortInner(&elems[0], axis);
        prevKey = elems[0].posPlusBVSetIn40DB10; // v20->posPlusBVSetIn40DB10; + 12

        // fix ---> it was //v6 = &v12[1]; //(tSnpAxisElem*)(v5 + 4);
        for (i = 1; i < snp->currElemIdx; i++)
        {
            INT_SnP_UnknownInversionSortInner(&elems[i], axis); //v12 = v5 + 16;
            keyPlusEps = elems[i].posPlusBVSetIn40DB10 + 0.001;
            if (keyPlusEps < prevKey) //*(float*)(v12 - 28 + 12))
            {
                movedGao = elems[i].gao;
                movedIndex = elems[i].gaoSnPIndex;
                movedFlags = elems[i].flags;
                movedGap = elems[i].gap;
                movedBVAxis = elems[i].ptrToBVAxisFloat;
                movedKey = elems[i].posPlusBVSetIn40DB10;

                threshold = (float)keyPlusEps;
                pos = i - 1; //(tSnpAxisElem*)(v12 - 28);
                while (1)
                {
                    INT_SnP_ComputeInversion(snp, axis, elems[pos].gaoSnPIndex, movedIndex,
                        elems[pos].flags & 1, movedFlags & 1);
                    if (--pos < 0)
                        break;
                    if (!(elems[pos].posPlusBVSetIn40DB10 > (double)threshold))
                        break;
                }

                memcpy(&elems[pos + 2], &elems[pos + 1], 16 * (i - pos - 1));
                elems[pos + 1].gao = movedGao;
                elems[pos + 1].gaoSnPIndex = movedIndex;
                elems[pos + 1].flags = movedFlags;
                elems[pos + 1].gap = movedGap;
                elems[pos + 1].ptrToBVAxisFloat = movedBVAxis;
                elems[pos + 1].posPlusBVSetIn40DB10 = movedKey;
            }
            prevKey = elems[i].posPlusBVSetIn40DB10; //v16 = *(float*)v7;
        }
    }
}
