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
        for (i = boneNum; i > 0; i--) {
            gao2Global = OBJ_pst_GetAbsoluteMatrix(xmec->gao2);
            if ((xmec->flags & 1) != 0) {
                memcpy(&col, &gao2Global->v2, sizeof(Vec3));
            }
            else {
                memcpy(&col, &gao2Global->v3, sizeof(Vec3));
            }
            MATH_AddScaleVector(&xmec->speedVecs[i - 1], &xmec->speedVecs[i - 1], &col, calculatedFactor);
            calculatedFactor = calculatedFactor - calculatedFactorPerBone;
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
        for (i = 0; i < boneNum; i++) {
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
            currGaoGlobal = currMatrixPtr;
            gao2GlobalPos = currMatrixPtr->Pos;
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

void GAO_ModifierXMEC_UpdatePointers(tModifier* mdf)
{
    tXmecMdfStruct* xmec;
    tGroup* grp;
    tGameObjectMain* bone;
    int i;

    xmec = (tXmecMdfStruct*)mdf->mdfPrivateDataPtr;
    grp = MDF_Modifier_GetCurGroup(mdf);
    if (!grp)
        return;

    for (i = 0; i < xmec->boneNum; i++)
    {
        xmec->gaos[i] = NULL;
        xmec->matrices[i] = NULL;
        bone = OBJ_pst_GroupGetByRank(grp, xmec->indices[i]);
        if (bone)
        {
            xmec->gaos[i] = bone;
            xmec->matrices[i] = OBJ_pst_GetAbsoluteMatrix(bone);
        }
    }

    if (xmec->rank != -1)
    {
        bone = OBJ_pst_GroupGetByRank(grp, xmec->rank);
        if (bone)
            xmec->gao = bone;
    }
}

void GAO_ModifierXMEC_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tXmecMdfStruct* xmec;
    int i;

    XMEC_FirstInit();
    xmec = (tXmecMdfStruct*)MEM_p_Alloc(sizeof(tXmecMdfStruct));
    mdf->mdfPrivateDataPtr = (void*)xmec;
    mdf->modifierFlags = 0x1000000;
    if (initData)
    {
        memcpy(xmec, initData, sizeof(tXmecMdfStruct));
    }
    else
    {
        memset(xmec, 0, sizeof(tXmecMdfStruct));
        xmec->i1 = -1;
        xmec->boneNum = 0;
        xmec->rank = -1;
        for (i = 0; i < 50; i++)
            xmec->indices[i] = i;
        GAO_ModifierXMEC_UpdatePointers(mdf);
    }
    xmec->gao = gao;
    xmec->gao2 = gao;
}

void GAO_ModifierXMEC_Destroy(struct sModifier* mdf)
{
    if (mdf->mdfPrivateDataPtr)
        MEM_Free(mdf->mdfPrivateDataPtr);
}

int GAO_ModifierXMEC_Load(struct sModifier* mdf, char* readPtr)
{
    char* startOffset;
    tXmecMdfStruct* xmec;
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
        for (idx = 0; idx < 50; idx++)
            xmec->indices[idx] = LOA_ReadLong(&readPtr, 0, 1);
    }
    xmec->gao = mdf->gao;
    xmec->gao2 = mdf->gao;
    return readPtr - startOffset;
}

void GAO_ModifierXMEC_Reinit(struct sModifier* mdf)
{
    tXmecMdfStruct* xmec;
    int i;

    xmec = (tXmecMdfStruct*)mdf->mdfPrivateDataPtr;
    if (xmec->boneNum > 32)
        xmec->boneNum = 32;
    for (i = 0; i < 50; i++)
    {
        xmec->vvv[i].x = 0.0;
        xmec->vvv[i].y = 0.0;
        xmec->vvv[i].z = 0.0;
        xmec->speedVecs[i].x = 0.0;
        xmec->speedVecs[i].y = 0.0;
        xmec->speedVecs[i].z = 0.0;
    }
    GAO_ModifierXMEC_UpdatePointers(mdf);
}

void GAO_ModifierXMEC_Unapply(struct sModifier* mdf, void* unk1)
{
}

static void XMEC_l_SetModifierFlags(struct sGameObjectMain* gao, int flagToAdd)
{
    tModifier* mdf;

    if (!gao->extended || !gao->extended->mdf)
        return;

    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == 10)
        {
            mdf->modifierFlags &= ~0x2000000u;
            mdf->modifierFlags |= flagToAdd;
        }
    }
}

void XMEC_OnOff(struct sGameObjectMain* gao, int a2, int onOff)
{
    tGameObjectActorObject* actor;
    tGroup* skeleton;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* bone;
    int flagToAdd;

    if (onOff)
        flagToAdd = 0;
    else
        flagToAdd = 0x2000000;

    XMEC_l_SetModifierFlags(gao, flagToAdd);

    if (!gao->basic || (gao->flags & 2) == 0)
        return;
    actor = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
    if (!actor || !actor->skeletonGroup)
        return;

    skeleton = actor->skeletonGroup;
    last = ((tableElemStructPF*)(skeleton->tablePF->nextElemPtr)) - 1;
    for (elem = (tableElemStructPF*)skeleton->tablePF->firstElemPtr; elem <= last; elem++)
    {
        bone = (tGameObjectMain*)elem->objectPtr;
        if (bone && ((uintptr_t)bone & 3) != 1)
            XMEC_l_SetModifierFlags(bone, flagToAdd);
    }
}
