#include "../memory.h"
#include "../loading.h"
#include "xmecmodifier.h"

int bFirst_286_0 = 0; // game doesn't set it explicitly

void GAO_ModifierXMEC_Normalize(Vec3* mainGaoGlobalPos, Mtx4x4 **mtxTmpPtrsForGlobal, int num, float substractFromLen, float pos3MinLimit) {
    int i;
    Vec3 *prevVector;
    Vec3 tmpPositions[50];
    Vec3 tmpPos;
    int iterationCounter;
    float calcdLen;

    for (iterationCounter = 0; iterationCounter < 10; iterationCounter++) {
        
        for (i = 0; i < num; i++) {
            tmpPositions[i].x = 0.0f;
            tmpPositions[i].y = 0.0f;
            tmpPositions[i].z = 0.0f;
        }
        
        prevVector = mainGaoGlobalPos;
        for (i = 0; i < num; i++) {
            MATH_SubstractVec(&tmpPos, prevVector, &mtxTmpPtrsForGlobal[i]->Pos);
            calcdLen = MATH_VecLen(&tmpPos) - substractFromLen;
            MATH_NormalizeVector(&tmpPos, &tmpPos);
            if (i > 0) {
                MATH_AddScaleVector(
                    &tmpPositions[i - 1],
                    &tmpPositions[i - 1],
                    &tmpPos,
                    (calcdLen * -0.5f));
                MATH_AddScaleVector(
                    &tmpPositions[i],
                    &tmpPositions[i],
                    &tmpPos,
                    (calcdLen * 0.5f));
            }
            else {
                MATH_AddScaleVector(&tmpPositions[0], &tmpPositions[0], &tmpPos, calcdLen);
            }
            prevVector = &mtxTmpPtrsForGlobal[i]->Pos;
        }
        for (i = 0; i < num; i++) {
            MATH_VecAddVec(&mtxTmpPtrsForGlobal[i]->Pos, &mtxTmpPtrsForGlobal[i]->Pos, &tmpPositions[i]);
            if (mtxTmpPtrsForGlobal[i]->Pos.z < pos3MinLimit) {
                mtxTmpPtrsForGlobal[i]->Pos.z = pos3MinLimit;
            }
        }
    }
}


void GAO_ModifierXMEC_Apply(struct sModifier* mdf, void* unk1) {
    int boneNum;
    int i;
    Vec3 col, temp, temp2;
    Vec3 gao2GlobalPos;
    Mtx4x4* currMatrixPtr;
    Mtx4x4* currGaoGlobal;
    Mtx4x4* gao2Global;
    Mtx4x4 tmpMatricesForGlobal[50];
    Mtx4x4 *mtxTmpPtrsForGlobal[50];
    Vec3 tmpVecStoreForPos[50];
    float calculatedFactor;

    tXmecMdfStruct* xmec = (tXmecMdfStruct*)mdf->mdfPrivateDataPtr;
    OBJ_ComputeGlobalWithLocal(xmec->gao2, OBJ_pst_GetAbsoluteMatrix(xmec->gao2), 1);
    if (xmec->gao2 != xmec->gao)
    {
        OBJ_ComputeGlobalWithLocal(xmec->gao, OBJ_pst_GetAbsoluteMatrix(xmec->gao), 1);
    }

    boneNum = xmec->boneNum;

    for (i = 0; i < boneNum; i++) {
        mtxTmpPtrsForGlobal[i] = &tmpMatricesForGlobal[i];
        MATH_SetIdentityMatrix(&tmpMatricesForGlobal[i]);
        if (xmec->matrices[i] != NULL) {
            memcpy(&tmpMatricesForGlobal[i], xmec->matrices[i], sizeof(Mtx4x4));
        }
        else {
            boneNum = i;
        }
    }

    if (boneNum != 0) {
        for (i = 0; i < boneNum; i++) {
            xmec->speedVecs[i].z = xmec->gravity * 0.050000001f + xmec->speedVecs[i].z;
            memcpy(&tmpVecStoreForPos[i], &mtxTmpPtrsForGlobal[i]->Pos, sizeof(Vec3));
        }

        calculatedFactor = xmec->tension * 0.050000001f;
        float calculatedFactorPerBone = calculatedFactor * xmec->elasticity / boneNum;
        if (boneNum > 0) {
            i = boneNum;
            do {
                gao2Global = OBJ_pst_GetAbsoluteMatrix(xmec->gao2);
                if ((xmec->flags & 1) != 0) {
                    memcpy(&col, &gao2Global->v2, sizeof(Vec3));
                }
                else {
                    memcpy(&col, &gao2Global->v3, sizeof(Vec3));
                }
                MATH_AddScaleVector(&xmec->speedVecs[i - 1], &xmec->speedVecs[i - 1], &col, calculatedFactor);
                calculatedFactor = calculatedFactor - calculatedFactorPerBone;
            } while (!(i-- == 1));
        }

        for (i = 0; i < boneNum; i++) {
            if (mtxTmpPtrsForGlobal[i]->Pos.z <= xmec->ground) {
                mtxTmpPtrsForGlobal[i]->Pos.z = xmec->ground;
                xmec->speedVecs[i].x = xmec->speedVecs[i].x * xmec->groundFriction;
                xmec->speedVecs[i].y = xmec->speedVecs[i].y * xmec->groundFriction;
            }
            MATH_AddScaleVector(&mtxTmpPtrsForGlobal[i]->Pos, &mtxTmpPtrsForGlobal[i]->Pos, &xmec->speedVecs[i], 0.050000001f);
        }

        GAO_ModifierXMEC_Normalize(&(OBJ_pst_GetAbsoluteMatrix(xmec->gao2)->Pos), mtxTmpPtrsForGlobal, boneNum, xmec->distance, xmec->ground);

        for (i = 0; i < boneNum; i++) {
            MATH_SubstractVec(&xmec->speedVecs[i], &mtxTmpPtrsForGlobal[i]->Pos, &tmpVecStoreForPos[i]);
            MATH_VecMultScalar(&xmec->speedVecs[i], &xmec->speedVecs[i], 20.0f);
        }

        gao2GlobalPos = OBJ_pst_GetAbsoluteMatrix(xmec->gao2)->Pos;
        currGaoGlobal = OBJ_pst_GetAbsoluteMatrix(xmec->gao2);
        i = 0;
        if (boneNum > 0) {
            while (TRUE) {
                currMatrixPtr = mtxTmpPtrsForGlobal[i];
                if ((xmec->flags & 1) != 0) {
                    MATH_SubstractVec(&temp, &gao2GlobalPos, &currMatrixPtr->Pos);
                    MATH_NormalizeVector(&temp, &temp);
                    memcpy(&currMatrixPtr->v2, &temp, sizeof(Vec3));
                    MATH_CrossProduct(&temp2, &currGaoGlobal->v1, &temp);
                    MATH_NormalizeVector(&temp2, &temp2);
                    memcpy(&currMatrixPtr->v3, &temp2, sizeof(Vec3));
                    MATH_CrossProduct(&temp, &currMatrixPtr->v2, &currMatrixPtr->v3);
                    memcpy(&currMatrixPtr->v1, &temp, sizeof(Vec3));
                }
                else {
                    MATH_SubstractVec(&temp, &gao2GlobalPos, &currMatrixPtr->Pos);
                    MATH_NormalizeVector(&temp, &temp);
                    memcpy(&currMatrixPtr->v3, &temp, sizeof(Vec3));
                    MATH_CrossProduct(&temp2, &temp, &currGaoGlobal->v1);
                    MATH_NormalizeVector(&temp2, &temp2);
                    memcpy(&currMatrixPtr->v2, &temp2, sizeof(Vec3));
                    MATH_CrossProduct(&temp, &currMatrixPtr->v2, &currMatrixPtr->v3);
                    memcpy(&currMatrixPtr->v1, &temp, sizeof(Vec3));
                }
                ++i;
                currGaoGlobal = currMatrixPtr;
                gao2GlobalPos = currMatrixPtr->Pos;
                if (i >= boneNum) {
                    break;
                }
            }
        }

        for (i = 0; i < boneNum; i++) {
            memcpy(xmec->matrices[i], mtxTmpPtrsForGlobal[i], sizeof(Mtx4x4));
            OBJ_ComputeLocalWhenHie(xmec->gaos[i]);
        }
    }
}

void XMEC_FirstInit()
{
    if (!bFirst_286_0)
        bFirst_286_0 = 1;
}

void GAO_ModifierXMEC_UpdatePointers(tModifier* a1)
{
    tXmecMdfStruct* v1;
    tGroup* v2;
    int v3;
    int* v4;
    tGameObjectMain** v4a;
    tGameObjectMain* v5;
    tGameObjectMain* v6;
    int v7;
    Mtx4x4** v4b;

    v1 = (tXmecMdfStruct*)a1->mdfPrivateDataPtr;
    v2 = MDF_Modifier_GetCurGroup(a1);
    if (v2)
    {
        v3 = 0;
        if (v1->boneNum > 0)
        {
            v4 = v1->indices;
            v4a = v1->gaos;
            v4b = v1->matrices;
            do
            {
                v7 = *v4;
                *v4a = NULL;
                *v4b = NULL;                    
                v5 = OBJ_pst_GroupGetByRank(v2, v7);
                if (v5)
                {
                    *v4a = v5;
                    *v4b = OBJ_pst_GetAbsoluteMatrix(v5);
                }
                ++v3;
                ++v4;
                ++v4a;
                ++v4b;
            } while (v3 < v1->boneNum);
        }
        if (v1->rank != -1)
        {
            v6 = OBJ_pst_GroupGetByRank(v2, v1->rank);
            if (v6)
                v1->gao = v6;
        }
    }
}

void GAO_ModifierXMEC_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tXmecMdfStruct* v3;
    tXmecMdfStruct* v4;
    tXmecMdfStruct* v5;
    int v6;
    int* v7;

    XMEC_FirstInit();
    v3 = (tXmecMdfStruct*)MEM_p_Alloc(sizeof(tXmecMdfStruct));
    v4 = v3;
    a2->mdfPrivateDataPtr = (void *)v3;
    v5 = v3;
    if (a3)
    {
        a2->modifierFlags = 0x1000000;
        memcpy(v3, a3, sizeof(tXmecMdfStruct));
        v3->gao = a1;
        v3->gao2 = a1;
    }
    else
    {
        v6 = 0;
        memset(v5, 0, sizeof(tXmecMdfStruct));
        v4->i1 = -1;
        a2->modifierFlags = 0x1000000;
        v4->boneNum = 0;
        v4->rank = -1;
        v7 = v4->indices;
        do
            *v7++ = v6++;
        while (v6 < 50);
        GAO_ModifierXMEC_UpdatePointers(a2);
        v4->gao = a1;
        v4->gao2 = a1;
    }
}

void GAO_ModifierXMEC_Destroy(struct sModifier* a1)
{
    if (a1->mdfPrivateDataPtr)
        MEM_Free(a1->mdfPrivateDataPtr);
}

int GAO_ModifierXMEC_Load(struct sModifier* mdf, char* readPtr)
{
    char* startOffset;
    tXmecMdfStruct* xmec;
    int* indices;
    int idx;

    XMEC_FirstInit();
    startOffset = readPtr;
    xmec = (tXmecMdfStruct*)mdf->mdfPrivateDataPtr;
    LOA_ReadLong(&readPtr, 0, 2);
    if (LOA_ReadLong(&readPtr, 0, 1))
    {
        xmec->distance = LOA_ReadFloat(&readPtr, 0, 1);
        xmec->gravity = LOA_ReadFloat(&readPtr, 0, 1);
        xmec->elasticity = LOA_ReadFloat(&readPtr, 0, 1);
        xmec->ground = LOA_ReadFloat(&readPtr, 0, 1);
        xmec->groundFriction = LOA_ReadFloat(&readPtr, 0, 1);
        xmec->tension = LOA_ReadFloat(&readPtr, 0, 1);
        xmec->flags = LOA_ReadLong(&readPtr, 0, 1);
        xmec->boneNum = LOA_ReadLong(&readPtr, 0, 1);
        xmec->rank = LOA_ReadLong(&readPtr, 0, 1);
        indices = xmec->indices;
        idx = 50;
        do
        {
            *indices++ = LOA_ReadLong(&readPtr, 0, 1);
            --idx;
        } while (idx);
    }
    xmec->gao = mdf->gao;
    xmec->gao2 = mdf->gao;
    return readPtr - startOffset;
}

void GAO_ModifierXMEC_Reinit(struct sModifier* a1)
{
    tXmecMdfStruct* xmec;
    Vec3* vvv;
    Vec3* speedVecs;
    int idx;

    xmec = (tXmecMdfStruct*)a1->mdfPrivateDataPtr;
    if (xmec->boneNum > 32)
        xmec->boneNum = 32;
    vvv = xmec->vvv;
    speedVecs = xmec->speedVecs;
    idx = 50;
    do
    {
        vvv->x = 0.0;
        vvv->y = 0.0;
        vvv->z = 0.0;
        speedVecs->x = 0.0;
        speedVecs->y = 0.0;
        speedVecs->z = 0.0;
        vvv++;
        speedVecs++;
        --idx;
    } while (idx);
    GAO_ModifierXMEC_UpdatePointers(a1);
}

void GAO_ModifierXMEC_Unapply(struct sModifier* a1, void* unk1) {
}

void XMEC_OnOff(struct sGameObjectMain* gao, int a2, int onOff)
{
    BOOL v3;
    tModifier* j;
    tModifier* i;
    tableElemStructPF* v6;
    tableElemStructPF* v7;
    tGroup* v8;
    tGameObjectMain* v9;
    int flagToAdd;

    if (onOff)
        flagToAdd = 0;
    else
        flagToAdd = 0x2000000;
    if (gao->extended && gao->extended->mdf)
    {
        for (i = gao->extended->mdf; i; i = i->nextElem)
        {
            if (i->modifierIntf->mdfId == 10)
            {
                i->modifierFlags &= ~0x2000000u;
                i->modifierFlags |= flagToAdd;
            }
        }
    }
    v8 = 0;
    v6 = 0;
    v7 = 0;
    if (gao)
    {
        if (gao->basic
            && (gao->flags & 2) != 0
            && gao->basic->actorObjectOrVisualObject   // actorObject
            && ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup)// skeletongroup
        {
            v8 = ((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup;
            v7 = ((tableElemStructPF*)(v8->tablePF->nextElemPtr)) - 1;
        }
        while (v6 <= v7)
        {
            if (v6)
            {
                v9 = (tGameObjectMain*)v6->objectPtr;
            }
            else
            {
                v9 = gao;
                if (v7)
                    v6 = ((tableElemStructPF*)(v8->tablePF->firstElemPtr)) - 1;
            }
            v3 = !v9 || ((uint8_t)v9 & 3) == 1;
            if (!v3 && v9->extended && v9->extended->mdf)
            {
                for (j = v9->extended->mdf; j; j = j->nextElem)
                {
                    if (j->modifierIntf->mdfId == 10)
                    {
                        j->modifierFlags &= ~0x2000000u;
                        j->modifierFlags |= flagToAdd;
                    }
                }
            }
            ++v6;
        }
    }
}