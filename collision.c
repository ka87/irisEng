#include "gameobject.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "graphicsobjects/geo.h"
#include "graphicsobjects/staticlod.h"
#include "dbg.h"
#include "snp.h"
#include "animation.h"
#include "time.h"
#include "modifiers/otcmodifier.h"
#include "collision.h"

int COL_ul_SnP_RefreshType = 1;

float gcjpUnkFloatflt_968524 = 0.5f;

void COL_AllocDetectionList(struct sGameObjectMain* gao)
{
    if (!gao->extended)
    {
        OBJ_GameObject_CreateExtended(gao);
        gao->flags |= 0x2000;
    }
    if (!gao->extended->coll)
    {
        gao->extended->coll = (tGameObjectCollMain*)MEM_p_Alloc(sizeof(tGameObjectCollMain));
        LOA_ul_FileTypeSize_38 += sizeof(tGameObjectCollMain);
        gao->extended->coll->detectionList = NULL;
        gao->extended->coll->colInstance = NULL;
        gao->extended->coll->colMap = NULL;
    }
    
    gao->extended->coll->detectionList = (tGameObjectCollDetectionList*)MEM_p_Alloc(sizeof(tGameObjectCollDetectionList));
    LOA_ul_FileTypeSize_38 += sizeof(tGameObjectCollDetectionList);
    gao->extended->coll->detectionList->overlapNum = 0;
    
    gao->extended->coll->detectionList->detectionListGaos = (tGameObjectMain**)MEM_p_Alloc(150 * sizeof(tGameObjectMain*));
    LOA_ul_FileTypeSize_38 += 150 * sizeof(tGameObjectMain*);
    memset(gao->extended->coll->detectionList->detectionListGaos, 0, (150 * sizeof(tGameObjectMain*)));
}

void COL_FreeDetectionList(struct sGameObjectCollMain* gao)
{
    if (!gao || !gao->detectionList) return;

    MEM_Free(gao->detectionList->detectionListGaos);
    MEM_Free(gao->detectionList);
}

BOOL COL_BV_PivotCollide(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL reverse)
{
    tGameObjectMain* firstGao;
    tGameObjectMain* secondGao;
    Mtx4x4* absMtx;
    Vec3 realMinPos;
    Vec3 realMaxPos;
    Vec3 distance;

    if (!gao1 || !gao2 || (gao1->flags2 & 1) == 0 || (gao2->flags2 & 1) == 0) return FALSE;
    
    if (reverse)
    {
        firstGao = gao1;
        secondGao = gao2;
    }
    else
    {
        firstGao = gao2;
        secondGao = gao1;
    }
    if (OBJ_BV_IsSphere(secondGao->boundingVolume))
    {
        if (secondGao->boundingVolume->BVMax.x != 0.0f)
        {
            absMtx = OBJ_pst_GetAbsoluteMatrix(firstGao);
            MATH_SubstractVec(&distance, &absMtx->Pos, &secondGao->boundingVolume->BVMin);

            if ((secondGao->boundingVolume->BVMax.x * secondGao->boundingVolume->BVMax.x) > MATH_VecLenSquared(&distance))
                return TRUE;
        }
    }
    else
    {
        MATH_VecAddVec(&realMinPos, &secondGao->boundingVolume->BVMin, &secondGao->matrices->Pos);
        MATH_VecAddVec(&realMaxPos, &secondGao->boundingVolume->BVMax, &secondGao->matrices->Pos);

        if (realMinPos.x != realMaxPos.x && realMinPos.y != realMaxPos.y && realMinPos.z != realMaxPos.z)
        {
            absMtx = OBJ_pst_GetAbsoluteMatrix(firstGao); 

            if (absMtx->Pos.x > realMinPos.x &&
                absMtx->Pos.x < realMaxPos.x &&
                absMtx->Pos.y > realMinPos.y &&
                absMtx->Pos.y < realMaxPos.y &&
                absMtx->Pos.z > realMinPos.z &&
                absMtx->Pos.z < realMaxPos.z)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void COL_FreeColSet(tColInstSet* instSet)
{
    int i;

    if (!instSet) return;

    LOA_DeleteAddress((int)instSet);
    if (instSet->instElemNum)
    {
        for (i = 0; i < instSet->instElemNum; i++) {
            MEM_Free((void*)instSet->colInstElems[i].elemShapePtr);
        }

        MEM_Free(instSet->colInstElems);
    }
    MEM_Free(instSet->zoneIDs);
    MEM_Free(instSet);
}

void COL_FreeInstance(struct sGameObjectCollMain* coll)
{
    int i;

    if (!coll || !coll->colInstance) return;

    if (coll->colInstance->colSet)
    {
        if (!--coll->colInstance->colSet->instanceNum)
            COL_FreeColSet(coll->colInstance->colSet);
    }
    LOA_DeleteAddress((int)coll->colInstance);
    MATH_Matrix_Free(coll->colInstance->mtx);

    if (coll->colInstance->instElemNum)
    {
        for (i = 0; i < coll->colInstance->instElemNum; i++) {
            MEM_Free((void*)coll->colInstance->colInstElems[i].elemShapePtr);
        }

        MEM_Free(coll->colInstance->colInstElems);
    }
    if (coll->colInstance->selectedInstElemPtrs)
        MEM_Free(coll->colInstance->selectedInstElemPtrs);

    MEM_Free(coll->colInstance);
    coll->colInstance = NULL;
}

void COL_FreeGameMaterial(tColCollisionObject* cob)
{
    if (!cob || !cob->gameMaterial) return;

    if (!--cob->gameMaterial->gameMaterialUsageCnt)
    {
        MEM_Free(cob->gameMaterial->ptrInts);
        MEM_Free(cob->gameMaterial->elems);
        MEM_Free(cob->gameMaterial);
        LOA_DeleteAddress((int)cob->gameMaterial);
        cob->gameMaterial = NULL;
        cob->flags |= 0xfe;
    }
}

void COL_FreeTrianglesIndexedCob(tColCollisionObject* cob)
{
    tCobMeshObjectMesh* cobMesh;
    tCobOK3AABBoxElem* currAABox;
    unsigned int i, j;

    cobMesh = (tCobMeshObjectMesh*)cob->objectPtrByType;
    
    for (i = 0; i < cobMesh->meshChunkElemNum; i++) {
        if (cobMesh->meshChunkElems[i].elemNum && cobMesh->meshChunkElems[i].triangleElems)
            MEM_Free(cobMesh->meshChunkElems[i].triangleElems);
    }
    
    MEM_Free(cobMesh->meshChunkElems);
    MEM_Free(cobMesh->normals);
    MEM_Free(cobMesh->vertices);

    if (cobMesh->ok3)
    {
        for (i = 0; i < cobMesh->ok3->num; i++) {
            currAABox = &cobMesh->ok3->aaBoxes[i];

            for (j = 0; j < currAABox->num; j++) {
                if (currAABox->chunkElems[j].triangleChunkElemIDs)
                    MEM_Free(currAABox->chunkElems[j].triangleChunkElemIDs);
            }

            MEM_Free(currAABox->chunkElems);
        }
        
        MEM_Free(cobMesh->ok3->aaBoxes);
        MEM_Free(cobMesh->ok3->faceTagInts);
        MEM_Free(cobMesh->ok3);
    }
    MEM_Free((void*)cob->objectPtrByType);
    MEM_Free(cob);
}

void COL_FreeMathematicalCob(tColCollisionObject* cob)
{
    MEM_Free( ((tCobNormalObjectGeneric *)(cob->objectPtrByType))->enclosurePtr);
    MEM_Free(cob->objectPtrByType);
    MEM_Free(cob);
}


void COL_FreeColMap(struct sGameObjectCollMain* a1)
{
    tGameObjectCollMain* v1;
    tColMap* v2;
    tColCollisionObject** v3;
    tColCollisionObject** v4;
    BOOL v5;
    BOOL v6;

    v1 = a1;
    if (a1)
    {
        v2 = a1->colMap;
        if (v2)
        {
            LOA_DeleteAddress((int)a1->colMap);
            if (v2->colObjectID)
            {
                v3 = v2->collisionObjectPtr;
                v4 = &v3[v2->colObjectID];
                if (v3 < v4)
                {
                    do
                    {
                        v5 = (*v3)->objectType == 5;
                        --(*v3)->usageCount;
                        v6 = v5;
                        if (!(*v3)->usageCount)
                        {
                            COL_FreeGameMaterial(*v3);
                            LOA_DeleteAddress((int)*v3);
                            if (v6)
                                COL_FreeTrianglesIndexedCob(*v3);
                            else
                                COL_FreeMathematicalCob(*v3);
                        }
                        ++v3;
                    } while (v3 < v4);
                    v1 = a1;
                }
                MEM_Free(v2->collisionObjectPtr);
            }
            MEM_Free(v2);
            v1->colMap = 0;
        }
    }
}

void COL_Free(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tGameObjectCollMain* v2;

    if (a1)
    {
        v1 = a1->extended;
        if (v1)
        {
            v2 = v1->coll;
            if (v1->coll)
            {
                if (v2->colInstance)
                    COL_FreeInstance(v2);
                if (v2->detectionList)
                    COL_FreeDetectionList(v2);
                if (v2->colMap)
                    COL_FreeColMap(v2);
                MEM_Free(v2);
            }
        }
    }
}

int COL_AllocColMap(struct sGameObjectMain* gao)
{
    tGameObjectCollMain* coll;
    tColMap* colMap;

    if (!gao->extended->coll)
    {
        gao->extended->coll = (tGameObjectCollMain*)MEM_p_Alloc(sizeof(tGameObjectCollMain));
        LOA_ul_FileTypeSize_38 += sizeof(tGameObjectCollMain);
        
        coll = gao->extended->coll;
        coll->detectionList = NULL;
        coll->colInstance = NULL;
        coll->colMap = NULL;
    }
    
    gao->extended->coll->colMap = (tColMap*)MEM_p_Alloc(sizeof(tColMap));
    LOA_ul_FileTypeSize_38 += sizeof(tColMap);

    colMap = gao->extended->coll->colMap;
    colMap->colObjectID = 0;
    colMap->activation = 0;
    colMap->b3 = 0;
    colMap->b4 = 0;
    colMap->collisionObjectPtr = 0;
    colMap->activation = -1;

    return 0;
}

int INT_PointInTriangle(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, uint8_t* a6)
{
    float v6; // eax
    double v7; // st7
    double v8; // st6
    float v10; // [esp+0h] [ebp-24h]
    float v11; // [esp+4h] [ebp-20h]
    float v12; // [esp+8h] [ebp-1Ch]
    float v13; // [esp+Ch] [ebp-18h]
    float v14; // [esp+10h] [ebp-14h]
    float v15; // [esp+14h] [ebp-10h]
    float v16; // [esp+18h] [ebp-Ch]
    float v17; // [esp+1Ch] [ebp-8h]
    float v18; // [esp+20h] [ebp-4h]
    float v19; // [esp+2Ch] [ebp+8h]
    float v20; // [esp+2Ch] [ebp+8h]
    float v21; // [esp+2Ch] [ebp+8h]
    float v22; // [esp+2Ch] [ebp+8h]
    float v23; // [esp+2Ch] [ebp+8h]
    float v24; // [esp+2Ch] [ebp+8h]
    float v25; // [esp+2Ch] [ebp+8h]

    v13 = a2->x - a1->x;
    v14 = a2->y - a1->y;
    v15 = a2->z - a1->z;
    v10 = a3->x - a1->x;
    v11 = a3->y - a1->y;
    v12 = a3->z - a1->z;
    v16 = a4->x - a1->x;
    v17 = a4->y - a1->y;
    v18 = a4->z - a1->z;
    if (fabs(a5->x) <= 0.57735026) // invsqrt3
    {
        if (fabs(a5->y) <= 0.57735026)
        {
            v6 = a5->z;
            v23 = v13 * v11 - v10 * v14;
            if (( (*(int32_t *)&v23) ^ (*(int32_t*)&v6)) >= 0)
            {
                v24 = v10 * v17 - v16 * v11;
                if ( ((*(int32_t*)&v24) ^ (*(int32_t*)&v6)) >= 0)
                {
                    v7 = v16 * v14;
                    v8 = v13 * v17;
                    goto LABEL_16;
                }
                goto LABEL_13;
            }
        }
        else
        {
            v6 = a5->y;
            v21 = v10 * v15 - v13 * v12;
            if (((*(int32_t*)&v21) ^ (*(int32_t*)&v6)) >= 0)
            {
                v22 = v16 * v12 - v10 * v18;
                if (((*(int32_t*)&v22) ^ (*(int32_t*)&v6)) >= 0)
                {
                    v7 = v13 * v18;
                    v8 = v16 * v15;
                    goto LABEL_16;
                }
            LABEL_13:
                if (a6)
                {
                    *a6 = 1;
                    return 0;
                }
                return 0;
            }
        }
    LABEL_10:
        if (a6)
        {
            *a6 = 0;
            return 0;
        }
        return 0;
    }
    v6 = a5->x;
    v19 = v12 * v14 - v11 * v15;
    if (((*(int32_t*)&v19) ^ (*(int32_t*)&a5->x)) < 0)
        goto LABEL_10;
    v20 = v18 * v11 - v17 * v12;
    if (((*(int32_t*)&v20) ^ (*(int32_t*)&v6)) < 0)
        goto LABEL_13;
    v7 = v17 * v15;
    v8 = v18 * v14;
LABEL_16:
    v25 = (float)(v7 - v8);
    if (((*(int32_t*)&v25) ^ (*(int32_t*)&v6)) >= 0)
        return 1;
    if (a6)
        *a6 = 2;
    return 0;
}

void COL_ComputeProximity(tColCollisionObject* a1)
{
    tCobMeshObjectMesh* v1;
    int v2;
    tCobMeshChunkElem* v3;
    tCobMeshTriangleElem* v4;
    uint16_t v5;
    tCobMeshTriangleElem* v6;
    uint16_t v7;
    tCobMeshChunkElem* meshinner1;
    tCobMeshTriangleElem* v9;
    uint16_t vertid1;
    uint16_t vvertid1;
    BOOL v12;
    uint16_t v13;
    uint16_t v14;
    uint16_t v15;
    BOOL v16;
    BOOL v17;
    BOOL v18;
    int prox1Set;
    int prox2Set;
    int prox3Set;
    int currInner1ElemID;
    int j;
    int i;
    uint16_t v25;
    int v26;
    tCobMeshObjectMesh* v27;
    unsigned int currInner1ElemNumInside;
    unsigned int currelemId;
    unsigned int currInner1ElemNum;
    tCobMeshChunkElem* v31;
    tCobMeshChunkElem* v32;
    int a1a;

    v1 = (tCobMeshObjectMesh*)a1->objectPtrByType;
    v2 = 0;
    v27 = v1;
    currInner1ElemNum = 0;
    if (v1->meshChunkElemNum)
    {
        i = 0;
        do
        {
            v3 = &v1->meshChunkElems[i];
            v32 = v3;
            currelemId = 0;
            if (!v3->elemNum)
                goto LABEL_44;
            j = 0;
            do
            {
                v4 = v3->triangleElems;
                v5 = v4[j].v3;
                v6 = &v4[j];
                v25 = v5;
                v6->proximity1 = -1;
                v6->proximity2 = -1;
                v6->proximity3 = -1;
                v7 = 0;
                prox1Set = 0;
                prox2Set = 0;
                prox3Set = 0;
                currInner1ElemNumInside = 0;
                if (!v1->meshChunkElemNum)
                    goto LABEL_43;
                currInner1ElemID = 0;
                do
                {
                    meshinner1 = &v1->meshChunkElems[currInner1ElemID];
                    v31 = meshinner1;
                    v26 = 0;
                    if (!meshinner1->elemNum)
                        goto LABEL_38;
                    a1a = 0;
                    while (1)
                    {
                        v9 = &meshinner1->triangleElems[a1a];
                        if (v9 != v6)
                            break;
                    LABEL_35:
                        ++a1a;
                        ++v7;
                        if (++v26 >= meshinner1->elemNum)
                            goto LABEL_38;
                    }
                    vertid1 = v9->v1;
                    vvertid1 = v6->v1;
                    v12 = v6->v1 == v9->v1;
                    v13 = v9->v2;
                    v14 = v9->v3;
                    if (v12 || vvertid1 == v13 || vvertid1 == v14)
                        v2 = 1;
                    v15 = v6->v2;
                    v16 = v15 == vertid1 || v15 == v13 || v15 == v14;
                    v17 = v25 == vertid1 || v25 == v13 || v25 == v14;
                    if (v2 + v17 + v16 != 2)
                    {
                    LABEL_34:
                        v1 = v27;
                        meshinner1 = v31;
                        v2 = 0;
                        goto LABEL_35;
                    }
                    if (!v2)
                        goto LABEL_30;
                    if (v16)
                    {
                        v6->proximity1 = v7;
                        prox1Set = 1;
                        goto LABEL_32;
                    }
                    if (v17)
                    {
                        v6->proximity2 = v7;
                        prox2Set = 1;
                    }
                    else
                    {
                    LABEL_30:
                        v6->proximity3 = v7;
                        prox3Set = 1;
                    }
                    if (!prox1Set)
                        goto LABEL_34;
                LABEL_32:
                    if (!prox2Set || !prox3Set)
                        goto LABEL_34;
                    v1 = v27;
                    v2 = 0;
                LABEL_38:
                    if (prox1Set && prox2Set && prox3Set)
                        break;
                    v18 = ++currInner1ElemNumInside < v1->meshChunkElemNum;
                    ++currInner1ElemID;
                } while (v18);
                v3 = v32;
            LABEL_43:
                ++j;
                ++currelemId;
            } while (currelemId < v3->elemNum);
        LABEL_44:
            v18 = ++currInner1ElemNum < v1->meshChunkElemNum;
            ++i;
        } while (v18);
    }
}

void COL_ColMap_AddSphere(tColCollisionObject* a1, tColCollisionObject* a2, Vec3* a3)
{
    tColSphere* v3;
    tColSphere* v4;

    tCobNormalObjectGeneric* cob1 = (tCobNormalObjectGeneric*)a1->objectPtrByType;
    tCobNormalObjectGeneric* cob2 = (tCobNormalObjectGeneric*)a2->objectPtrByType;

    cob1->enclosurePtr = MEM_p_Alloc(16u);
    v3 = (tColSphere*)(cob1->enclosurePtr);
    if (a2)
    {
        v4 = (tColSphere*)(cob2->enclosurePtr);
        v3->radius = v4->radius;
        if (a3)
        {
            v3->center = *a3;
        }
        else
        {
            v3->center.x = v4->center.x;
            v3->center.y = v4->center.y;
            v3->center.z = v4->center.z;
        }
        a1->flags = a2->flags;
        a1->objectType = 2;
    }
    else
    {
        v3->radius = 1.0;
        if (a3)
        {
            v3->center = *a3;
        }
        else
        {
            v3->center.x = 0.0;
            v3->center.y = 0.0;
            v3->center.z = 0.0;
        }
        a1->objectType = 2;
    }
}

tCobGameMaterialElems* COL_pst_GMat_Get(tColCollisionObject* a1, tCobMeshChunkElem* a2)
{
    tCobGameMaterial* v2;
    tCobGameMaterialElems* gmatelem;
    unsigned int v4;
    int num;
    int* v6;
    unsigned int v7;
    unsigned int v8;
    unsigned int v9;
    int v11;
    int* i;
    tCobNormalObjectGeneric* cob;

    v2 = a1->gameMaterial;
    if (!v2)
        return 0;
    gmatelem = v2->elems;
    if (!gmatelem)
        return 0;
    if (a2)
    {
        v4 = a2->gameMaterialElemID;
    }
    else
    {
        if (a1->objectType == 5)
            return 0;
        cob = (tCobNormalObjectGeneric*)a1->objectPtrByType;
        v4 = cob->gameMaterialElemID;
    }
    num = v2->num;
    if (v2->num > 10u)
    {
        v6 = v2->ptrInts;
        v7 = num >> 1;
        v8 = v6[num >> 1];
        v9 = 0;
        if (v8 == v4)
        {
        LABEL_14:
            gmatelem[v7].id = v4;
            return &a1->gameMaterial->elems[v7];
        }
        while (v9 + 1 != num)
        {
            if (v8 <= v4)
                v9 = v7;
            else
                num = v7;
            v7 = (num + v9) >> 1;
            v8 = v6[v7];
            if (v8 == v4)
                goto LABEL_14;
        }
        return 0;
    }
    v11 = 0;
    if (!num)
        return 0;
    for (i = v2->ptrInts; *i != v4; ++i)
    {
        if (++v11 >= num)
            return 0;
    }
    gmatelem[v11].id = v4;
    return &a1->gameMaterial->elems[v11];
}

BOOL COL_GMat_Skip(tCobGameMaterialElems* a1, tColGlobal* a2)
{
    return a1
        && ((a1->flags & 0x200000) != 0 && (a2->collInstanceElem->colFlags & 0x10) == 0
            || (a1->flags & 0x1000) != 0
            && (a2->collisionObjectPtr->flags & 2) != 0
            && (a2->collInstanceElem->colFlags & 0x80u) == 0)
        || (a2->collInstanceElem->colFlags & 0x80u) != 0
        && a1
        && (a1->flags & 0x1000) == 0
        && (a2->collisionObjectPtr->flags & 2) != 0;
}

BOOL INT_PointSphere(Vec3* param_1, float param_2, Vec3* param_3)

{
    float fVar1;

    fVar1 = (param_1->x - param_3->x) * (param_1->x - param_3->x) +
        (param_1->y - param_3->y) * (param_1->y - param_3->y) +
        (param_1->z - param_3->z) * (param_1->z - param_3->z);
    if ((uint16_t)((uint16_t)(param_2 * param_2 < fVar1) << 8 | // ez milyen egy megoldás float összehasonlításra?????? TODO FIXME
        (uint16_t)(param_2 * param_2 == fVar1) << 14) == 0) {
        return 1;
    }
    return 0;
}

BOOL INT_SphereSphere(Vec3* param_1, float a2, Vec3* param_3, float a4)
{
    float param_2; // [esp+0h] [ebp-8h]

    param_2 = a2 + a4;
    return INT_PointSphere(param_1, param_2, param_3);
}

BOOL INT_FullSphereSphere(Vec3* a1, float a2, Vec3* a3, float a4, Vec3* a5, Vec3* a6)
{
    BOOL result; 
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    float v13;
    float v14;
    Vec3 v15;
    float v16;

    result = INT_SphereSphere(a1, a2, a3, a4);
    if (result)
    {
        v8 = a1->x - a3->x; //TODO FIXME v7 volt az a3, így biztos jó?
        v9 = a1->y - a3->y;
        v10 = a1->z - a3->z;
        v16 = (float)v10;
        v13 = (float)(1.0 / sqrt(v10 * v16 + v8 * v8 + v9 * v9));
        v15.x = (float)(v8 * v13);
        v15.y = (float)(v9 * v13);
        v15.z = v16 * v13;
        v11 = v15.x * a4;
        a6->x = (float)v11;
        v12 = v15.y * a4;
        a6->y = (float)v12;
        v14 = v15.z * a4;
        a6->z = (float)v14;
        a6->x = (float)(v11 + a3->x);
        a6->y = (float)(v12 + a3->y);
        a6->z = (float)(v14 + a3->z);
        *a5 = v15;
        result = 1;
    }
    return result;
}

int INT_FullSegmentSphere(Vec3* segmentPoint1, Vec3* segmentPoint2, Vec3* a3, float a4, Vec3* a1)
{
    double v7; // st7
    double v8; // st6
    long double v9; // st5
    int result; // eax
    double v11; // st5
    double v12; // st7
    double v13; // st7
    double v14; // st7
    float v15; // [esp+4h] [ebp-28h]
    float v16; // [esp+8h] [ebp-24h]
    float v17; // [esp+Ch] [ebp-20h]
    float v18; // [esp+10h] [ebp-1Ch]
    Vec3 v19; // [esp+14h] [ebp-18h] BYREF
    float v20; // [esp+24h] [ebp-8h]
    float a2a; // [esp+30h] [ebp+4h]
    float v22; // [esp+34h] [ebp+8h]
    float v23; // [esp+38h] [ebp+Ch]

    v16 = segmentPoint1->x - a3->x;
    v17 = segmentPoint1->y - a3->y;
    v18 = segmentPoint1->z - a3->z;
    v7 = segmentPoint2->x - segmentPoint1->x;
    v20 = segmentPoint2->y - segmentPoint1->y;
    v8 = segmentPoint2->z - segmentPoint1->z;
    v9 = sqrt(v8 * v8 + v20 * v20 + v7 * v7);
    v23 = (float)v9;
    if (v9 == 0.0f)
        return 0;
    v11 = 1.0 / v23;
    v19.x = (float)(v11 * v7);
    v19.y = (float)(v20 * v11);
    v19.z = (float)(v11 * v8);
    v12 = -(v18 * v19.z + v17 * v19.y + v16 * v19.x);
    a2a = (float)v12;
    v13 = v12 * a2a;
    v15 = v18 * v18 + v16 * v16 + v17 * v17;
    v22 = a4 * a4;
    if (a2a < 0.0 && v15 >(double)v22)
        return 0;
    v14 = v15 - v13;
    if (v14 > v22 || a2a - sqrt(v22 - v14) > v23)
        return 0;
    if (a2a <= (double)v23)
    {
        if (v15 <= (double)v22)
        {
            *a1 = *segmentPoint1;
        }
        else
        {
            MATH_VecMultScalar(a1, &v19, a2a);
            MATH_VecAddVec(a1, segmentPoint1, a1);
        }
        result = 1;
    }
    else
    {
        *a1 = *segmentPoint2;
        result = 1;
    }
    return result;
}

int INT_FullSegmentTriangle(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, Vec3* a6, Vec3* a7, uint8_t* a8)
{
    double v9; // st6
    double v11; // st7
    double v12; // st6
    float v13; // [esp+4h] [ebp-Ch]
    float v14; // [esp+8h] [ebp-8h]
    float v15; // [esp+Ch] [ebp-4h]
    float a2a; // [esp+1Ch] [ebp+Ch]
    float a2b; // [esp+1Ch] [ebp+Ch]

    v13 = a2->x - a1->x;
    v14 = a2->y - a1->y;
    v15 = a2->z - a1->z;
    v9 = v15 * a6->z + v13 * a6->x + v14 * a6->y;
    if (v9 < 0.0000099999997f && v9 > -0.0000099999997f)
        return 0;
    a2a = (float)(((a3->z - a1->z) * a6->z + (a3->x - a1->x) * a6->x + (a3->y - a1->y) * a6->y) / v9);
    if (a2a <= 0.0 || a2a >= 1.0)
        return 0;
    v11 = v13 * a2a;
    a7->x = (float)v11;
    v12 = v14 * a2a;
    a7->y = (float)v12;
    a2b = v15 * a2a;
    a7->z = a2b;
    a7->x = (float)(v11 + a1->x);
    a7->y = (float)(v12 + a1->y);
    a7->z = a2b + a1->z;
    return INT_PointInTriangle(a7, a3, a4, a5, a6, a8);
}

int INT_FullSphereTriangle(Vec3* centerPos, float radius, Vec3* point1, Vec3* point2, Vec3* point3, Vec3* nomalVec, Vec3* collisionPoint, int* collisionType)
{
    float v8; // st7
    float v9; // st6
    Vec3* v10; // edi
    float v11; // st7
    float v12; // st6
    Vec3* v14; // [esp-1Ch] [ebp-40h]
    Vec3 a1; // [esp+0h] [ebp-24h] BYREF
    float v16; // [esp+10h] [ebp-14h]
    float v17; // [esp+14h] [ebp-10h]
    Vec3 a2; // [esp+18h] [ebp-Ch] BYREF
    uint8_t ct;

    v8 = radius * nomalVec->x;
    v9 = radius * nomalVec->y;
    a1.z = radius * nomalVec->z;
    v10 = collisionPoint;
    a1.x = v8 + centerPos->x;
    v14 = nomalVec;
    a1.y = centerPos->y + v9;
    a1.z = centerPos->z + a1.z;
    ct = 0;
    v11 = -(radius + radius);
    v12 = v11 * v14->x;
    v16 = v11 * v14->y;
    v17 = v11 * v14->z;
    a2.x = v12 + a1.x;
    a2.y = v16 + a1.y;
    a2.z = v17 + a1.z;
    if (INT_FullSegmentTriangle(&a1, &a2, point1, point2, point3, v14, collisionPoint, &ct))
    {
        *collisionType = 1;
        return 1;
    }
    if (!ct)
    {
        if (INT_FullSegmentSphere(point1, point2, centerPos, radius, v10))
        {
            *collisionType = 2;
            return 1;
        }
        if (INT_FullSegmentSphere(point2, point3, centerPos, radius, v10))
        {
            *collisionType = 8;
            return 1;
        }
        if (INT_FullSegmentSphere(point3, point1, centerPos, radius, v10))
            goto LABEL_21;
        return 0;
    }
    if (ct == 1)
    {
        if (INT_FullSegmentSphere(point2, point3, centerPos, radius, v10))
        {
        LABEL_11:
            *collisionType = 8;
            return 1;
        }
        if (INT_FullSegmentSphere(point1, point2, centerPos, radius, v10))
        {
            *collisionType = 2;
            return 1;
        }
        if (INT_FullSegmentSphere(point3, point1, centerPos, radius, v10))
        {
            *collisionType = 4;
            return 1;
        }
        return 0;
    }
    if (ct == 2)
    {
        if (!INT_FullSegmentSphere(point3, point1, centerPos, radius, v10))
        {
            if (INT_FullSegmentSphere(point1, point2, centerPos, radius, v10))
            {
                *collisionType = 2;
                return 1;
            }
            if (!INT_FullSegmentSphere(point2, point3, centerPos, radius, v10))
                return 0;
            goto LABEL_11;
        }
    LABEL_21:
        *collisionType = 4;
        return 1;
    }
    return 1;
}

void COL_ReportCollision(Vec3* a1, Vec3* a2, Vec3* a3, int a4, int a5, int16_t a6, tColGlobal* a7)
{
    tWorld* v7;
    tColCollisionObject* v9;
    tCobGameMaterialElems* v10;
    int v11;

    if (a7->num < 1000u)
    {
        v7 = a7->colWorld;
        v7->records[a7->num].collidedPoint = *a1;
        v7->records[a7->num].recalVec = *a3;
        v7->records[a7->num].normalVec = *a2;
        v7->records[a7->num].collisionInstanceGameObject = a7->colInstanceGao;
        v7->records[a7->num].collisionMapGameObject = a7->colMapGao;
        v7->records[a7->num].flags = a4;
        v7->records[a7->num].triangleChunkElemID = a5;
        v7->records[a7->num].triangleChunkID = a6;
        v9 = a7->collisionObjectPtr;
        if ((a4 & 0xF) != 0)
        {
            v10 = COL_pst_GMat_Get(v9, a7->cobMeshChunkElem);
            v7->records[a7->num].designVal = a7->cobMeshChunkElem->designVal;
        }
        else
        {
            v10 = COL_pst_GMat_Get(v9, 0);
            v7->records[a7->num].designVal = 0;
        }
        v7->records[a7->num].gameMaterial = v10;
        if (v10)
        {
            if ((a7->colInstance->crossable & v10->flags) != 0)
                v7->records[a7->num].flags |= 0x2000u;
        }
        v11 = a7->num + 1;
        a7->num = v11;
        v7->recordCount = v11;
    }
}

BOOL COL_NeedRecomputation(tColGlobal* a1, tGameObjectMain* a2, int16_t a3)
{
    tGameObjectMain* v3;
    tGameObjectExtended* v4;
    tColInstance* v5;
    int v6;
    uint8_t v7;
    int v8;
    tGameObjectMain** v9;
    tGameObjectExtended* v10;
    tGameObjectCollMain* v11;
    tColInstance* v12;

    v3 = a2;
    v4 = a2->extended;
    v5 = v4->coll->colInstance;
    if ((a2->flags2 & 0x20) != 0)
    {
        v6 = 0;
        if (v5)
        {
            if (v5->flag >= 0)
            {
                v7 = a1->gaosNeededToBeRecomputedNum;
                if (v7 < 50u)
                {
                    v8 = v7;
                    a2 = 0;
                    if (v7)
                    {
                        v9 = a1->gaosNeededToBeRecomputed;
                        while (*v9 != v3)
                        {
                            ++v6;
                            ++v9;
                            if (v6 >= a1->gaosNeededToBeRecomputedNum)
                                goto LABEL_10;
                            v5 = v4->coll->colInstance;
                        }
                        a2 = (tGameObjectMain*)1;
                        if (v4 && v4->coll && v5)
                        {
                            v5->flag |= a3;
                            return 0;
                        }
                    }
                    else
                    {
                    LABEL_10:
                        a1->gaosNeededToBeRecomputed[v8] = v3;
                        ++a1->gaosNeededToBeRecomputedNum;
                        v10 = v3->extended;
                        if (v10)
                        {
                            v11 = v10->coll;
                            if (v11)
                            {
                                v12 = v11->colInstance;
                                if (v12)
                                    v12->flag |= a3;
                            }
                        }
                    }
                }
                return a2 == 0;
            }
        }
    }
    return 1;
}

int16_t COL_test_flag_sub_436970(struct sColInstance* a1, int16_t a2)
{
    int16_t result;

    if (a1)
        result = a2 & a1->flag;
    else
        result = 0;
    return result;
}

BOOL COL_b_IsARealTopologicEdge(tColGlobal* a1, Vec3* a2, uint8_t a3)
{
    BOOL v3;
    uint16_t v4;
    tCobMeshTriangleElem* v6;
    Vec3* v7;
    Vec3* v8;
    BOOL v9;
    int v10;
    Vec3* v11;
    double v12;
    double v13;
    Vec3 a1a;
    Vec3 v15;

    a1->colMapEdgeNormalVec.x = 0.0;
    a1->colMapEdgeNormalVec.y = 0.0;
    a1->colMapEdgeNormalVec.z = 0.0;
    v3 = fabs(a1->colInstGaoMatrices->v3.z - 1.0) < 0.001;
    if ((a3 & 2) != 0)
    {
        v4 = a1->cobMeshTriangleElem->proximity1;
    }
    else
    {
        v6 = a1->cobMeshTriangleElem;
        if ((a3 & 4) == 0)
        {
            v4 = v6->proximity3;
            if (v4 == 0xFFFF)
                return 1;
            goto LABEL_8;
        }
        v4 = v6->proximity2;
    }
    if (v4 == 0xFFFF)
        return 1;
LABEL_8:    
    v7 = &(((tCobMeshObjectMesh*)(a1->collisionObjectPtr->objectPtrByType))->normals[v4]);
    a1->colMapEdgeNormalVec.x = v7->x + a2->x;
    a1->colMapEdgeNormalVec.y = v7->y + a2->y;
    a1->colMapEdgeNormalVec.z = v7->z + a2->z;
    if (!v3)
    {
        MATH_TransformVectorNoScale(&a1a, a1->colMapGaoMatrices, a2);
        MATH_TransformVectorNoScale(&v15, a1->colMapGaoMatrices, v7);
        v11 = a1->ptrSetToVec6InZDMZDRGravityNeg;
        v12 = v11->z * a1a.z + v11->y * a1a.y + a1a.x * v11->x;
        if (v12 >= 0.0f && v12 < a1->wallCosAngle)
            v9 = 1;
        else
            v9 = v12 < 0.0 && v12 > -0.80000001f;
        v13 = v11->z * v15.z + v11->y * v15.y + v15.x * v11->x;
        if (v13 >= 0.0f && v13 < a1->wallCosAngle)
        {
            v10 = 1;
            goto LABEL_25;
        }
        if (v13 < 0.0f && v13 > -0.80000001f)
        {
            v10 = 1;
            goto LABEL_25;
        }
    LABEL_24:
        v10 = 0;
        goto LABEL_25;
    }
    v8 = a1->ptrSetToVec6InZDMZDRGravityNeg;
    v9 = v8->z * a2->z + v8->y * a2->y + v8->x * a2->x < a1->wallCosAngle;
    if (v8->z * v7->z + v8->y * v7->y + v7->x * v8->x >= a1->wallCosAngle)
        goto LABEL_24;
    v10 = 1;
LABEL_25:
    a1->colMapEdgeNormalVec.x = a1->colMapEdgeNormalVec.x * 0.5f;
    a1->colMapEdgeNormalVec.y = a1->colMapEdgeNormalVec.y * 0.5f;
    a1->colMapEdgeNormalVec.z = a1->colMapEdgeNormalVec.z * 0.5f;
    return v9 && v10 && cos(0.3141592741012573) > v7->z * a2->z + v7->y * a2->y + v7->x * a2->x;
}

int COL_ComputeReport(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, int* a6, int a7, int16_t a8, tColGlobal* colGlobal)
{
    float a4a;
    double v10; // st7
    int* v11; // edx
    Vec3* v12; // eax
    tGameObjectMain* v13; // ecx
    tColInstance* v14; // eax
    tCobGameMaterialElems* v15; // eax
    double v17; // st7
    double v18; // st7
    BOOL v19; // edi
    int v20; // eax
    int* v21; // eax
    int v22; // ecx
    int v23; // eax
    float v24; // edx
    float v25; // eax
    Mtx4x4* v26; // ecx
    float v27; // edx
    float v28; // ecx
    tColInstanceElem* v29; // eax
    int v30; // eax
    double v31; // st6
    long double v32; // st7
    long double v33; // st7
    tColCollisionObject* v34; // edi
    tCobMeshChunkElem* v35; // eax
    double v36; // st6
    double v37; // st7
    tGameObjectMain* v38; // edi
    tGameObjectExtended* v39; // eax
    tGameObjectCollMain* v40; // eax
    tColInstance* v41 = NULL; // eax
    tWorld* v42; // edi
    tCobGameMaterialElems* gmat; // eax
    double v44; // st7
    BOOL v45; // zf
    tColCollisionObject* v46; // eax
    tCobGameMaterialElems* v47; // eax
    tColInstance* v48; // eax
    int v49; // ecx
    Vec3* v50; // eax
    tColBox* v51; // ebp
    int i; // ebx
    double v53; // st7
    int v54; // [esp+10h] [ebp-58h]
    Vec3 a1a; // [esp+14h] [ebp-54h] BYREF
    Vec3 v56; // [esp+20h] [ebp-48h] BYREF
    Vec3 a2a; // [esp+2Ch] [ebp-3Ch] BYREF
    Vec3 v58; // [esp+38h] [ebp-30h] BYREF
    Vec3 a3a; // [esp+44h] [ebp-24h] BYREF
    Vec3 v60; // [esp+50h] [ebp-18h] BYREF
    Vec3 v61; // [esp+5Ch] [ebp-Ch] BYREF
    float v62; // [esp+78h] [ebp+10h]
    float colGlobala; // [esp+8Ch] [ebp+24h]
    float colGlobalc; // [esp+8Ch] [ebp+24h]
    float colGlobalb; // [esp+8Ch] [ebp+24h]
    float colGlobald; // [esp+8Ch] [ebp+24h]

    MATH_TransformVectorNoScale(a4, colGlobal->colMapGaoMatrices, a2);
    MATH_TransformVertexNoScale(a3, colGlobal->colMapGaoMatrices, a1);
    v10 = colGlobal->ptrSetToVec6InZDMZDRGravityNeg->y * a4->y
        + colGlobal->ptrSetToVec6InZDMZDRGravityNeg->z * a4->z
        + colGlobal->ptrSetToVec6InZDMZDRGravityNeg->x * a4->x;
    colGlobala = (float)v10;
    if (v10 >= 0.0 && colGlobala < (double)colGlobal->wallCosAngle || colGlobala < 0.0 && colGlobala > -0.80000001)
    {
        v11 = a6;
    }
    else
    {
        v11 = a6;
        if ((*a6 & 0x400) == 0 || a4->x == 0.0f && a4->y == 0.0f)
        {
            *a6 |= 0x40u;
            v12 = colGlobal->ptrSetToVec6InZDMZDRGravityNeg;
            if (colGlobala >= 0.0)
            {
                a3a = *v12;
            }
            else
            {
                a3a.x = -v12->x;
                v13 = colGlobal->colInstanceGao;
                a3a.y = -v12->y;
                a3a.z = -v12->z;
                v13->extended->coll->colInstance->flag |= 0x4000u;
            }
            goto LABEL_17;
        }
    }
    *v11 |= 0x80u;
    v14 = colGlobal->colInstance;
    if (v14)
    {
        if ((v14->flag & 4) != 0)
        {
            v15 = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, colGlobal->cobMeshChunkElem);
            if (!v15 || (colGlobal->colInstance->crossable & v15->flags) == 0)
            {
                COL_NeedRecomputation(colGlobal, colGlobal->colInstanceGao, 16);
                return 0;
            }
        }
    }
LABEL_17:
    a2a.x = colGlobal->centerPosPrevious.x - a1->x;
    a2a.y = colGlobal->centerPosPrevious.y - a1->y;
    v17 = colGlobal->centerPosPrevious.z - a1->z;
    a2a.z = (float)v17;
    if (v17 * a2->z + a2a.y * a2->y + a2a.x * a2->x <= 0.0)
    {
        *a6 |= 0x80001000;
        return 1;
    }
    v56.x = colGlobal->centerPos.x - a1->x;
    v56.y = colGlobal->centerPos.y - a1->y;
    v18 = colGlobal->centerPos.z - a1->z;
    v56.z = (float)v18;
    v19 = v18 * a2->z + v56.y * a2->y + v56.x * a2->x < 0.0;
    v20 = *a6;
    if ((*a6 & 0xE) != 0 && (v20 & 0x80u) != 0 && (v20 & 0x20) != 0 && COL_b_IsARealTopologicEdge(colGlobal, a2, *a6))
    {
        v21 = a6;
        v22 = 1;
        v54 = 1;
        *a6 |= 0x800u;
    }
    else
    {
        v21 = a6;
        v22 = 0;
        v54 = 0;
    }
    v23 = *v21;
    if ((v23 & 0x40) == 0 || (colGlobal->collisionObjectPtr->flags & 0x10) != 0)
    {
        if (v22 || COL_gst_GlobalVars.COL_gb_SpecialCamera && (v23 & 0x20) == 0)
        {
            MATH_NormalizeVector(&v58, &a2a);
            MATH_TransformVectorNoScale(a4, colGlobal->colMapGaoMatrices, &v58);
            if (colGlobal->colInstanceGao == WOR_gpst_CurrentWorld->viewPtr->assignedGao)
            {
                a1a = v58;
            }
            else if (fabs(colGlobal->colMapGaoMatrices->v3.z - 1.0) >= 0.001)
            {
                v28 = a4->y;
                a2a.x = a4->x;
                a2a.y = v28;
                a2a.z = 0.0;
                MATH_NormalizeEqualVector(&a2a);
                MATH_TransformVectorNoScale(&a1a, &colGlobal->colMapGaoInvMatrix, &a2a);
            }
            else
            {
                a1a = v58;
                if (fabs(v58.z) >= 0.001)
                {
                    a1a.z = 0.0;
                    MATH_NormalizeEqualVector(&a1a);
                }
            }
        }
        else if ((colGlobal->collisionObjectPtr->flags & 0x10) != 0)
        {
            MATH_TransformVectorNoScale(&v60, colGlobal->colMapGaoMatrices, a2);
            MATH_TransformVectorNoScale(&a1a, &colGlobal->colMapGaoInvMatrix, &v60);
        }
        else if (colGlobal->colInstanceGao == WOR_gpst_CurrentWorld->viewPtr->assignedGao)
        {
            v24 = a2->y;
            v25 = a2->z;
            a1a.x = a2->x;
            a1a.y = v24;
            a1a.z = v25;
        }
        else
        {
            v26 = colGlobal->colMapGaoMatrices;
            if (fabs(v26->v3.z - 1.0) >= 0.001)
            {
                MATH_TransformVectorNoScale(&a2a, v26, a2);
                a2a.z = 0.0;
                MATH_NormalizeEqualVector(&a2a);
                MATH_TransformVectorNoScale(&a1a, &colGlobal->colMapGaoInvMatrix, &a2a);
            }
            else
            {
                v27 = a2->y;
                a1a.x = a2->x;
                a1a.y = v27;
                a1a.z = 0.0;
                MATH_NormalizeEqualVector(&a1a);
            }
        }
    }
    else
    {
        MATH_TransformVector(&a1a, &colGlobal->colMapGaoInvMatrix, &a3a);
        MATH_NormalizeEqualVector(&a1a);
    }
    v29 = colGlobal->collInstanceElem;
    if (v29->elemType != 2)
    {
        v51 = (tColBox*)v29->elemShapePtr;
        MATH_TransformVectorNoScale(&v56, colGlobal->colMapGaoMatrices, &a1a);
        v62 = -100000.0f;
        for (i = 0; i < 8; ++i)
        {
            if ((i & 1) != 0)
                a2a.x = v51->pos2.x;
            else
                a2a.x = v51->pos1.x;
            if ((i & 2) != 0)
                a2a.y = v51->pos2.y;
            else
                a2a.y = v51->pos1.y;
            if ((i & 4) != 0)
                a2a.z = v51->pos2.z;
            else
                a2a.z = v51->pos1.z;
            MATH_TransformVertex(&v60, colGlobal->colInstGaoMatrices, &a2a);
            v53 = v56.z * (a3->z - v60.z) + v56.y * (a3->y - v60.y) + (a3->x - v60.x) * v56.x;
            if (v53 > v62)
                v62 = (float)v53;
        }
        if (v62 >= 0.001)
        {
            a5->x = v56.x * v62;
            a5->y = v56.y * v62;
            a5->z = v56.z * v62;
            return 1;
        }
    LABEL_105:
        *a6 |= 0x40000u;
        a5->x = 0.0;
        a5->y = 0.0;
        a5->z = 0.0;
        return 1;
    }
    v30 = *a6;
    if ((*a6 & 0x20) == 0)
    {
        v32 = -(v56.z * a1a.z + a1a.y * v56.y + a1a.x * v56.x);
        goto LABEL_65;
    }
    if ((v30 & 1) == 0 && (v30 & 0x700) == 0)
    {
        v31 = v56.z * a1a.z + a1a.y * v56.y + a1a.x * v56.x;
        a4a = (float)v31;
        colGlobalc = (float)(v31 * a4a + colGlobal->radius * colGlobal->radius);
        v32 = sqrt(colGlobalc - MATH_VecLenSquared(&v56)) - a4a;
    LABEL_65:
        colGlobalb = (float)v32;
        goto LABEL_66;
    }
    v33 = sqrt(v56.z * v56.z + v56.y * v56.y + v56.x * v56.x);
    if (v19)
        colGlobalb = (float)((v33 + colGlobal->radius) / (a1a.y * a2->y + a1a.x * a2->x + a1a.z * a2->z));
    else
        colGlobalb = (float)((colGlobal->radius - v33) / (a1a.y * a2->y + a1a.x * a2->x + a1a.z * a2->z));
    if ((v30 & 0x40) != 0 && (v30 & 1) != 0)
    {
        v34 = colGlobal->collisionObjectPtr;
        if ((v34->flags & 0x10) == 0 && a4->z < 0.94999999)
        {
            MATH_VecMultScalar(&a2a, a2, colGlobal->radius);
            a2a.x = -a2a.x;
            a2a.y = -a2a.y;
            a2a.z = -a2a.z;
            MATH_VecAddVecWithoutBuffer(&a2a, &colGlobal->centerPos);
            MATH_VecMultScalar(&v60, &a1a, colGlobalb);
            v35 = colGlobal->cobMeshChunkElem;
            a2a.x = v60.x + a2a.x;
            a2a.y = v60.y + a2a.y;
            a2a.z = v60.z + a2a.z;
            if (!INT_PointInTriangle(&a2a, 
                &(((tCobMeshObjectMesh*)(v34->objectPtrByType))->vertices[v35->triangleElems[a7].v1]), 
                &(((tCobMeshObjectMesh*)(v34->objectPtrByType))->vertices[v35->triangleElems[a7].v2]),
                &(((tCobMeshObjectMesh*)(v34->objectPtrByType))->vertices[v35->triangleElems[a7].v3]), a2, 0))
            {
                *a6 = *a6 & 0xFFFFFFFE | 0xE;
                v36 = v56.z * a1a.z + a1a.y * v56.y + a1a.x * v56.x;
                a4a = (float)v36;
                colGlobald = (float)(v36 * a4a + colGlobal->radius * colGlobal->radius);
                v37 = MATH_VecLenSquared(&v56);
                v38 = colGlobal->colInstanceGao;
                colGlobalb = (float)(sqrt(colGlobald - v37) - a4a);
                if (!v38)
                    goto LABEL_61;
                v39 = v38->extended;
                if (v39 && (v40 = v39->coll) != 0)
                    v41 = v40->colInstance;
                else
                    LABEL_61:
                v41 = 0;
                if (!COL_test_flag_sub_436970(v41, 1024))
                    COL_NeedRecomputation(colGlobal, v38, 1024);
            }
        }
    }
LABEL_66:
    if (v54 && (colGlobal->colInstance->flag & 1) == 0 && colGlobal->num < 1000u) // reportcollision inline ?
    {
        v42 = colGlobal->colWorld;
        v42->records[colGlobal->num].collidedPoint = *a3;
        v42->records[colGlobal->num].recalVec = MATH_gst_NulVector;
        MATH_TransformVectorNoScale(&v42->records[colGlobal->num].normalVec, colGlobal->colMapGaoMatrices, a2);
        v42->records[colGlobal->num].collisionInstanceGameObject = colGlobal->colInstanceGao;
        v42->records[colGlobal->num].collisionMapGameObject = colGlobal->colMapGao;
        v42->records[colGlobal->num].flags = *a6 & 0xE | 0x200A0;
        v42->records[colGlobal->num].triangleChunkElemID = a7;
        v42->records[colGlobal->num].triangleChunkID = a8;
        gmat = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, colGlobal->cobMeshChunkElem);
        v42->records[colGlobal->num].gameMaterial = gmat;
        if (gmat && (colGlobal->colInstance->crossable & gmat->flags) != 0)
            v42->records[colGlobal->num].flags |= 0x2000u;
        v42->records[colGlobal->num].gameMaterial = gmat;
        ++colGlobal->num;
    }
    if (colGlobalb < 0.001)
        goto LABEL_105;
    v44 = colGlobalb + 0.0000099999997;
    v45 = (*a6 & 0x40) == 0;
    v61.x = (float)(a1a.x * v44);
    v61.y = (float)(a1a.y * v44);
    v61.z = (float)(a1a.z * v44);
    if (!v45)
    {
        v46 = colGlobal->collisionObjectPtr;
        if ((v46->flags & 0x10) == 0)
        {
            v47 = COL_pst_GMat_Get(v46, colGlobal->cobMeshChunkElem);
            if (!v47 || (colGlobal->colInstance->crossable & v47->flags) == 0)
            {
                v48 = colGlobal->colInstance;
                if (v48)
                    v48->flag |= 4u;
                v49 = *a6;
                if ((*a6 & 0xE) != 0)
                {
                    if ((v49 & 2) != 0)
                        v50 = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[colGlobal->cobMeshTriangleElem->proximity1]);
                    else
                        v50 = a4;
                    if ((v49 & 4) != 0)
                        v50 = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[colGlobal->cobMeshTriangleElem->proximity2]);
                    if ((v49 & 8) != 0)
                        v50 = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[colGlobal->cobMeshTriangleElem->proximity3]);
                    if (MATH_DotProduct(v50, a2) < 0.94999999)
                        COL_NeedRecomputation(colGlobal, colGlobal->colInstanceGao, 32);
                }
            }
        }
    }
    MATH_TransformVectorNoScale(a5, colGlobal->colMapGaoMatrices, &v61);
    return 1;
}

void COL_AllocInstance(struct sGameObjectMain* a1)
{
    tGameObjectCollMain* v1;
    tGameObjectCollMain* v2;
    Mtx4x4* v3;
    int v4;

    if (!a1->extended->coll)
    {
        a1->extended->coll = (tGameObjectCollMain*)MEM_p_Alloc(sizeof(tGameObjectCollMain));
        LOA_ul_FileTypeSize_38 += sizeof(tGameObjectCollMain);
        v1 = a1->extended->coll;
        v1->detectionList = 0;
        v1->colInstance = 0;
        v1->colMap = 0;
    }
    v2 = a1->extended->coll;
    v2->colInstance = (tColInstance*)MEM_p_Alloc(sizeof(tColInstance));
    LOA_ul_FileTypeSize_38 += sizeof(tColInstance);
    memset(v2->colInstance, 0, sizeof(tColInstance));
    v2->colInstance->mtx = MATH_Matrix_Alloc();
    LOA_ul_FileTypeSize_38 += sizeof(Mtx4x4);
    v3 = v2->colInstance->mtx;
    v3->v1.x = 1.0;
    v3->v2.y = 1.0;
    v3->v3.z = 1.0;
    v3->w = 1.0;
    v4 = v3->mtxflag;
    v3->v1.y = 0.0;
    v3->v1.z = 0.0;
    v3->_m14 = 0.0;
    v3->v2.x = 0.0;
    v3->v2.z = 0.0;
    v3->_m24 = 0.0;
    v3->v3.x = 0.0;
    v3->v3.y = 0.0;
    v3->_m34 = 0.0;
    v3->Pos.x = 0.0;
    v3->Pos.y = 0.0;
    v3->Pos.z = 0.0;
    v3->mtxflag = v4 & 2;
}

void COL_DuplicateInstance(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tColInstance* v2;
    tColInstance* v3;
    tColInstSet* v4; 
    uint8_t v5;
    tColInstanceElem* v6;
    tColInstanceElem* v7;
    tColSphere* v8;
    tColSphere* v9;
    tColBox* v10;
    tColBox* v11;
    tColInstanceElem** unkPtrCurr;
    char v15;
    tColInstanceElem* v16;
    tColInstanceElem* v17;
    uint8_t v18;
    tColInstanceElem* v19;
    tColInstanceElem* v20;
    tColInstanceElem* v21;
    tColInstSet* v22;
    tColInstanceElem* v23;
    tColInstanceElem* v24;
    tColInstanceElem** v25;
    tColInstanceElem** a1a;
    tColInstanceElem* a2a;
    char a2b;

    v2 = a2->extended->coll->colInstance;
    COL_AllocInstance(a1);
    v3 = a1->extended->coll->colInstance;
    v4 = v2->colSet;
    v3->colSet = v2->colSet;
    v3->zoneNum = v2->zoneNum;
    v3->sharedZoneNum = v2->sharedZoneNum;
    v3->instElemNum = v2->instElemNum;
    v3->instElemBitmask = v2->instElemBitmask;
    v3->activationFlags = v2->activationFlags;
    v3->minLod = v2->minLod;
    v3->maxLod = v2->maxLod;
    v22 = v4;
    v3->selectedInstElemPtrs = (tColInstanceElem**)MEM_p_Alloc(4 * v4->instElemNum);
    LOA_ul_FileTypeSize_38 += 4 * v4->instElemNum;
    v5 = v2->instElemNum;
    if (v5)
    {
        v3->colInstElems = (tColInstanceElem*)MEM_p_Alloc(8 * v5);
        LOA_ul_FileTypeSize_38 += 8 * v2->instElemNum;
        memcpy(v3->colInstElems, v2->colInstElems, 8 * v2->instElemNum);
        v6 = v2->colInstElems;
        a2a = &v6[v2->instElemNum];
        if (v6 < a2a)
        {
            v7 = v3->colInstElems;
            do
            {
                if (v6->elemType == 1)
                {
                    v7->elemShapePtr = MEM_p_Alloc(sizeof(tColBox));
                    LOA_ul_FileTypeSize_38 += sizeof(tColBox);
                    v10 = (tColBox*)v6->elemShapePtr;
                    v11 = (tColBox*)v7->elemShapePtr;
                    v11->pos1.x = v10->pos1.x;
                    v11->pos1.y = v10->pos1.y;
                    v11->pos1.z = v10->pos1.z;
                    v11->pos2.x = v10->pos2.x;
                    v11->pos2.y = v10->pos2.y;
                    v11->pos2.z = v10->pos2.z;
                }
                else if (v6->elemType == 2)
                {
                    v7->elemShapePtr = MEM_p_Alloc(sizeof(tColSphere));
                    LOA_ul_FileTypeSize_38 += sizeof(tColSphere);
                    v8 = (tColSphere*)v6->elemShapePtr;
                    v9 = (tColSphere*)v7->elemShapePtr;
                    v9->center.x = v8->center.x;
                    v9->center.y = v8->center.y;
                    v9->center.z = v8->center.z;
                    v9->radius = v8->radius;
                }
                ++v6;
                ++v7;
            } while (v6 < a2a);
        }
        v4 = v22;
    }
    unkPtrCurr = v3->selectedInstElemPtrs;
    a1a = v2->selectedInstElemPtrs;
    v23 = v3->colInstElems;
    v15 = 0;
    v25 = &unkPtrCurr[v3->zoneNum];
    v24 = v3->colSet->colInstElems;
    a2b = 0;
    if (unkPtrCurr >= v25)
    {
        ++v4->instanceNum;
    }
    else
    {
        do
        {
            if (((1 << v15) & v3->instElemBitmask) != 0)
            {
                v16 = v2->colInstElems;
                v17 = &v16[v2->instElemNum];
                v18 = 0;
                if (v16 >= v17)
                {
                LABEL_15:
                    v18 = -1;
                }
                else
                {
                    while (v16 != *a1a)
                    {
                        ++v16;
                        ++v18;
                        if (v16 >= v17)
                            goto LABEL_15;
                    }
                }
                v19 = v23;
            }
            else
            {
                v20 = v2->colSet->colInstElems;
                v21 = &v20[v2->colSet->instElemNum];
                v18 = 0;
                if (v20 >= v21)
                {
                LABEL_20:
                    v18 = -1;
                }
                else
                {
                    while (v20 != *a1a)
                    {
                        ++v20;
                        ++v18;
                        if (v20 >= v21)
                            goto LABEL_20;
                    }
                }
                v19 = v24;
            }
            *unkPtrCurr = &v19[v18];
            ++a1a;
            ++unkPtrCurr;
            v15 = ++a2b;
        } while (unkPtrCurr < v25);
        ++v22->instanceNum;
    }
}

void COL_DuplicateColMap(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tColMap* v2;
    tColMap* v3;
    tColCollisionObject** v4;
    tColCollisionObject** v5;
    uint8_t i;

    v2 = a2->extended->coll->colMap;
    COL_AllocColMap(a1);
    v3 = a1->extended->coll->colMap;
    v3->activation = v2->activation;
    v3->b3 = v2->b3;
    v3->b4 = v2->b4;
    v3->colObjectID = v2->colObjectID;
    v3->collisionObjectPtr = (tColCollisionObject**)MEM_p_Alloc(4 * v2->colObjectID);
    LOA_ul_FileTypeSize_38 += 4 * v2->colObjectID;
    v4 = v2->collisionObjectPtr;
    v5 = &v4[v2->colObjectID];
    for (i = 0; v4 < v5; ++i)
    {
        v3->collisionObjectPtr[i] = *v4;
        ++(*v4++)->usageCount;
    }
}

void COL_ColMap_AddGeometric(struct sColCollisionObject* a1, struct sGEOGRO* a3_1, tGameObjectMain* a3)
{
    tGEOGRO* v3;
    tCobMeshObjectMesh* v4;
    Mtx4x4* v5;
    Vec3* v6;
    Vec3* v7;
    Vec3* i; 
    Vec3* v9;
    unsigned int v10;
    tGeoTriangleChunks* v11;
    tGeoTriangleChunks* v12;
    int v13;
    Vec3* v14;
    tCobMeshChunkElem* v15;
    tGeoTriangleChunks* v16;
    tCobMeshChunkElem* v17;
    BOOL v18;
    tCobMeshTriangleElem* v19;
    tGeoTriangleChunkElem* v20;
    tCobMeshTriangleElem* v21;
    tCobMeshTriangleElem* v22;
    int v23;
    Vec3* v24;
    Vec3* v25;
    Vec3* v26;
    double v27;
    double v28;
    Vec3* v29; 
    double v30;
    double v31;
    Vec3* v32;
    Vec3* v33; 
    long double v34;
    tCobMeshChunkElem* v35;
    tCobMeshChunkElem* v36;
    tGeoTriangleChunks* j;
    tCobMeshTriangleElem* v38;
    float v39;
    float v40;
    float v41;
    Vec3 a1a; 
    int geogroa; 

    v3 = a3_1;
    if (a3_1)
        goto LABEL_31;
    if ((a3->flags & 0x4000) != 0)
    {
        v3 = (tGEOGRO*)(((tGameObjectVisualObject*)(a3->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
        if (v3->intf->id == 8)                    // lod
            v3 = ((tGEOSTATICLOD *)(v3))->geo[0];
        if (v3)
        {
        LABEL_31:
            if (v3->vertexNum && v3->triangleChunkCount)
            {
                a1->objectType = 5;
                v4 = (tCobMeshObjectMesh*)MEM_p_Alloc(0x1Cu);
                a1->objectPtrByType = v4;
                v4->ok3 = 0;
                GEO_MRM_SetNumberOfPoints_Percent(v3, 2.0);
                v4->vertexCount = v3->vertexNum;
                if (a3 && (OBJ_pst_GetAbsoluteMatrix(a3)->mtxflag & 8) != 0)
                {
                    v4->vertices = (Vec3*)MEM_p_Alloc(12 * v4->vertexCount);
                    v5 = OBJ_pst_GetAbsoluteMatrix(a3);
                    MATH_GetScale(&a1a, v5);
                    v6 = v3->vertices;
                    v7 = v4->vertices;
                    for (i = &v6[v3->vertexNum]; v6 < i; )
                    {
                        ++v6;
                        ++v7;
                        v7[-1].x = a1a.x * v6[-1].x;
                        v7[-1].y = a1a.y * v6[-1].y;
                        v7[-1].z = a1a.z * v6[-1].z;
                    }
                }
                else
                {
                    v9 = (Vec3*)MEM_p_Alloc(12 * v4->vertexCount);
                    v10 = 12 * v4->vertexCount;
                    v4->vertices = v9;
                    memcpy(v9, v3->vertices, v10);
                }
                v4->meshChunkElemNum = v3->triangleChunkCount;
                v4->meshChunkElems = (tCobMeshChunkElem*)MEM_p_Alloc(16 * v3->triangleChunkCount);
                v11 = v3->triangleChunks;
                v12 = &v11[v3->triangleChunkCount];
                for (v4->faceCount = 0; v11 < v12; v4->faceCount = v13)
                {
                    v13 = v11->triangleNum + v4->faceCount;
                    ++v11;
                }
                v14 = (Vec3*)MEM_p_Alloc(12 * v4->faceCount);
                v15 = v4->meshChunkElems;
                v4->normals = v14;
                v16 = v3->triangleChunks;
                v17 = &v15[v4->meshChunkElemNum];
                geogroa = 0;
                v36 = v15;
                v35 = v17;
                for (j = v16; v15 < v17; j = v16)
                {
                    v15->flags = 0;
                    v15->gameMaterialElemID = v16->materialNum;
                    v18 = ((v16->triangleNum & 0xffff) == 0);
                    v15->elemNum = v16->triangleNum;
                    if (!v18)
                    {
                        v19 = (tCobMeshTriangleElem*)MEM_p_Alloc(12 * v16->triangleNum);
                        v15->triangleElems = v19;
                        v20 = v16->triangleChunkElems;
                        v21 = v19;
                        v22 = &v19[v15->elemNum];
                        v38 = v22;
                        if (v21 < v22)
                        {
                            v23 = geogroa;
                            geogroa += ((char*)v22 - (char*)v21 - 1) / 12 + 1; // TODO FIXME, ez undorito, megcsinálni
                            do
                            {
                                v21->v1 = v20->v1;
                                v21->v2 = v20->v2;
                                v21->v3 = v20->v3;
                                v24 = v4->vertices;
                                v25 = &v24[v20->v2];
                                v26 = &v24[v20->v1];
                                v27 = v25->x - v26->x;
                                v28 = v25->y - v26->y;
                                v29 = &v24[v20->v3];
                                v30 = v25->z - v26->z;
                                a1a.x = v29->x - v26->x;
                                a1a.y = v29->y - v26->y;
                                v31 = v29->z - v26->z;
                                v39 = (float)(v31 * v28 - a1a.y * v30);
                                v40 = (float)(v30 * a1a.x - v31 * v27);
                                v41 = (float)(a1a.y * v27 - v28 * a1a.x);
                                if (v39 == 0.0f && v40 == 0.0f && v41 == 0.0f)
                                {
                                    v32 = &v4->normals[v23];
                                    v32->x = v39;
                                    v32->y = v40;
                                    v32->z = v41;
                                }
                                else
                                {
                                    v33 = &v4->normals[v23];
                                    v34 = 1.0 / sqrt(v41 * v41 + v40 * v40 + v39 * v39);
                                    v33->x = (float)(v39 * v34);
                                    v33->y = (float)(v40 * v34);
                                    v33->z = (float)(v34 * v41);
                                }
                                ++v21;
                                ++v20;
                                ++v23;
                            } while (v21 < v38);
                            v15 = v36;
                            v16 = j;
                        }
                        v17 = v35;
                    }
                    ++v15;
                    ++v16;
                    v36 = v15;
                }
            }
        }
    }
}

void COL_ColMap_AddCob(tGameObjectMain* a1, uint8_t type, void* a3, Vec3* a4)
{
    tColMap* v4;
    uint8_t v5;
    tColCollisionObject* v6;
    tCobNormalObjectGeneric* v7;

    v4 = a1->extended->coll->colMap;
    v5 = v4->colObjectID;
    if (v4->colObjectID == 8)
        DBG_assert(0,__FILE__, __LINE__, 0, "Sorry, Max number of Cobs reached", 0, 1);
    if (v5)
    {
        v4->collisionObjectPtr = (tColCollisionObject**)MEM_p_Realloc(v4->collisionObjectPtr, 4 * v5 + 4);
        v4->collisionObjectPtr[v4->colObjectID] = (tColCollisionObject*)MEM_p_Alloc(sizeof(tColCollisionObject));
        v6 = v4->collisionObjectPtr[v4->colObjectID];
    }
    else
    {
        v4->collisionObjectPtr = (tColCollisionObject**)MEM_p_Alloc(4u);
        *v4->collisionObjectPtr = (tColCollisionObject*)MEM_p_Alloc(sizeof(tColCollisionObject));
        v6 = *v4->collisionObjectPtr;
    }
    v6->usageCount = 0;
    v6->gameMaterial = 0;
    if (type == 2)
    {
        v7 = (tCobNormalObjectGeneric*)MEM_p_Alloc(8u);
        v6->objectPtrByType = v7;
        v7->enclosurePtr = 0;
        v7->gameMaterialElemID = 0;
        COL_ColMap_AddSphere(v6, (tColCollisionObject*)a3, a4);
    }
    else if (type == 5)
    {
        COL_ColMap_AddGeometric(v6, (tGEOGRO*)a3, a1);
        ++v4->colObjectID;
        ++v6->usageCount;
        return;
    }
    ++v4->colObjectID;
    ++v6->usageCount;
}

void COL_ResolveRefs(struct sGameObjectMain* a1)
{
    tColMap* colMap; // ebx
    tColCollisionObject** v2; // esi
    tColCollisionObject** v3; // ebp
    tColCollisionObject* v4; // eax
    tCobGameMaterial* v5; // ecx
    int v6; // eax
    tColInstance* v7; // ebp
    tGaoMatrix* v8; // esi
    tGaoMatrix* v9; // edi
    Mtx4x4* v10; // eax
    uint8_t v11; // al
    uint8_t v12; // bl
    tColInstanceElem** v13; // eax
    tColInstanceElem** v14; // eax
    unsigned int v15; // edx
    uint8_t i; // al
    uint8_t v17; // bl
    tColInstanceElem** v18; // eax
    uint8_t v19; // dl
    tColInstanceElem** v20; // eax
    tColInstSet* v21; // ebp

    if ((a1->flags & 0x100) != 0)
    {
        colMap = a1->extended->coll->colMap;
        if (colMap && colMap->colObjectID)
        {
            v2 = colMap->collisionObjectPtr;
            v3 = &v2[colMap->colObjectID];
            if (v2 < v3)
            {
                while (1)
                {
                    v4 = *v2;
                    if (!*v2)
                        break;
                    v5 = v4->gameMaterial;
                    if (v5 && !v4->usageCount)
                        ++v5->gameMaterialUsageCnt;
                    ++(*v2)->usageCount;
                    if ((*v2)->objectType == 5)
                    {
                        if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
                            COL_ComputeProximity(*v2);
                        COL_OK3_Build(a1, 1, 0);
                    }
                    if (++v2 >= v3)
                        goto LABEL_18;
                }
                a1->flags &= ~0x100u;
                MEM_Free(colMap->collisionObjectPtr);
                LOA_DeleteAddress((int)colMap);
                MEM_Free(colMap);
                a1->extended->coll->colMap = 0;
            }
        }
        else
        {
            COL_AllocColMap(a1);
            COL_ColMap_AddCob(a1, 5u, 0, 0);
        }
    }
LABEL_18:
    v6 = a1->flags;
    if ((a1->flags & 0x600) != 0)
    {
        v7 = a1->extended->coll->colInstance;
        if (!v7 || !v7->colSet)
        {
            a1->flags = v6 & ~0x600u;
            return;
        }
        if ((v6 & 0x400000) != 0)
        {
            v8 = a1->basic->mtx;
            v9 = (tGaoMatrix*)v7->mtx;
            if (v9 == v8)
                goto LABEL_27;
        }
        else
        {
            v10 = OBJ_pst_GetAbsoluteMatrix(a1);
            v9 = (tGaoMatrix*)v7->mtx;
            if (v9 == (tGaoMatrix*)v10)
                goto LABEL_27;
            v8 = (tGaoMatrix*)v10;
        }
        memcpy(v9, v8, sizeof(Mtx4x4));
    LABEL_27:
        v11 = v7->colSet->instElemNum;
        if (v7->zoneNum != v11)
        {
            v7->zoneNum = v11;
            v7->sharedZoneNum = v11;
            if (v7->instElemNum)
            {
                v12 = 0;
                do
                    MEM_Free((void*)v7->colInstElems[v12++].elemShapePtr);
                while (v12 < v7->instElemNum);
                MEM_Free(v7->colInstElems);
            }
            v13 = v7->selectedInstElemPtrs;
            v7->instElemNum = 0;
            if (v13)
                MEM_Free(v13);
            v14 = (tColInstanceElem**)MEM_p_Alloc(4 * v7->zoneNum);
            v15 = 4 * v7->zoneNum;
            v7->selectedInstElemPtrs = v14;
            memset(v14, 0, v15);
            for (i = 0; i < v7->zoneNum; ++i)
                v7->selectedInstElemPtrs[i] = (tColInstanceElem*)i;
            v7->instElemBitmask = 0;
        }
        v17 = 0;
        if (v7->zoneNum)
        {
            while (1)
            {
                v18 = v7->selectedInstElemPtrs;
                v19 = (uint8_t)v18[v17];
                v20 = &v18[v17];
                if (v19 > 16u)
                    break;
                if ((v7->instElemBitmask & (1 << v17)) != 0)
                    *v20 = &v7->colInstElems[v19];
                else
                    *v20 = &v7->colSet->colInstElems[v19];
                if (++v17 >= v7->zoneNum)
                    goto LABEL_43;
            }
            a1->flags &= ~0x600u;
        }
        else
        {
        LABEL_43:
            v21 = v7->colSet;
            if (v21)
                ++v21->instanceNum;
        }
    }
}

void COL_Reinit(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tGameObjectCollMain* v2;
    tColInstance* v3;
    Mtx4x4* v4;
    tColMap* v5;

    if (WOR_gpst_CurrentWorld)
        INT_SnP_DetachObject(a1, WOR_gpst_CurrentWorld);
    else
        a1->snpIndex = -1;
    v1 = a1->extended;
    if (v1)
    {
        v2 = v1->coll;
        if (v2)
        {
            if ((a1->flags2 & 0x20) != 0)
                v2->detectionList->overlapNum = 0;
            if ((a1->flags & 0x600) != 0)
            {
                v3 = a1->extended->coll->colInstance;
                if (v3)
                {
                    v3->activationFlags = -1;
                    a1->extended->coll->colInstance->crossable = 0x589A;
                    if ((a1->flags & 0x400000) != 0)
                    {
                        MATH_CopyMatrix(a1->extended->coll->colInstance->mtx, &a1->basic->mtx->mtx);
                    }
                    else
                    {
                        v4 = OBJ_pst_GetAbsoluteMatrix(a1);
                        MATH_CopyMatrix(a1->extended->coll->colInstance->mtx, v4);
                    }
                }
                else
                {
                    a1->flags &= ~0x600u;
                }
            }
            if ((a1->flags & 0x100) != 0)
            {
                v5 = a1->extended->coll->colMap;
                if (v5)
                    v5->activation = -1;
                else
                    a1->flags &= ~0x100u;
            }
        }
    }
}

int INT_FullRayTriangle(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, float* distance, Vec3* a7)
{
    double v10;
    double v11;
    double v12;
    double v13;
    int result;
    double v15;
    double v16;
    double v17;
    float v18;
    float v19;
    float v20;
    float v21;
    Vec3 a1a; 
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float a3a;
    float v30;
    float a2a;

    a1a.x = a4->x - a3->x;
    a1a.y = a4->y - a3->y;
    a1a.z = a4->z - a3->z;
    v23 = a5->x - a3->x;
    v24 = a5->y - a3->y;
    v25 = a5->z - a3->z;
    v10 = v25 * a2->y - v24 * a2->z;
    v11 = v23 * a2->z - v25 * a2->x;
    v12 = v24 * a2->x - v23 * a2->y;
    v13 = a1a.z * v12 + v11 * a1a.y + v10 * a1a.x;
    if (v13 < 0.0000099999997 && v13 > -0.0000099999997)
        goto LABEL_3;
    a2a = (float)(1.0 / v13);
    v19 = a1->x - a3->x;
    v20 = a1->y - a3->y;
    v15 = a1->z - a3->z;
    v21 = (float)v15;
    v30 = (float)((v15 * v12 + v20 * v11 + v19 * v10) * a2a);
    if (v30 < 0.0)
        goto LABEL_3;
    if (v30 > 1.0)
        goto LABEL_3;
    v26 = v20 * a1a.z - v21 * a1a.y;
    v27 = v21 * a1a.x - a1a.z * v19;
    v28 = v19 * a1a.y - v20 * a1a.x;
    v16 = (v26 * a2->x + v27 * a2->y + v28 * a2->z) * a2a;
    a3a = (float)v16;
    if (v16 < 0.0 || a3a + v30 > 1.0)
        goto LABEL_3;
    if (a7)
    {
        MATH_VecMultScalar(&a1a, a5, a3a);
        MATH_VecMultScalar(a7, a4, v30);
        MATH_VecAddVecWithoutBuffer(a7, &a1a);
        v18 = (float)(1.0 - v30 - a3a);
        MATH_VecMultScalar(&a1a, a3, v18);
        MATH_VecAddVecWithoutBuffer(a7, &a1a);
    }
    v17 = (v28 * v25 + v27 * v24 + v26 * v23) * a2a;
    *distance = (float)v17;
    if (v17 < 0.0)
        LABEL_3:
    result = 0;
    else
        result = 1;
    return result;
}


// TODO FIXME, ez át lett írva, ill bele lett javítva a generált kódba nem biztos hogy jó
int INT_FullRayAABBox(Vec3* a1, Vec3* a2, tColBox* a3, Vec3* a4, float a5, float* a6)
{
    int v6;
    int v7;
    int v8;
    Vec3* v9;
    int v11;
    double v12;
    double v13;
    double v14;
    double v18;
    double v22;
    float v26;
    float v27;
    Vec3 v28; 
    float v29;
    float v30;
    float v31;
    Vec3 v32;
    int v32a;
    float v33;

    v26 = a3->pos1.y;
    v27 = a3->pos1.z;
    v28 = a3->pos2;
    v6 = 1;
    if (a1->x >= (double)a3->pos1.x)
    {
        if (a1->x <= (double)v28.x)
        {
            v7 = 2;
        }
        else
        {
            v7 = 0;
            v29 = a3->pos2.x;
            v6 = 0;
        }
    }
    else
    {
        v7 = 1;
        v29 = a3->pos1.x;
        v6 = 0;
    }
    if (a1->y >= (double)v26)
    {
        if (a1->y <= (double)v28.y)
        {
            v32a = 2;
        }
        else
        {
            v6 = 0;
            v32a = 0; // fixed v32.y = 0.0 volt
            v30 = v28.y;
        }
    }
    else
    {
        v32a = 1;
        v30 = v26;
        v6 = 0;
    }
    if (a1->z >= (double)v27)
    {
        if (a1->z <= (double)v28.z)
        {
            v8 = 2;
        }
        else
        {
            v8 = 0;
            v31 = v28.z;
            v6 = 0;
        }
    }
    else
    {
        v31 = v27;
        v8 = 1;
        v6 = 0;
    }
    v9 = a4;
    if (!a4)
        v9 = &v32;
    if (!v6)
    {
        v11 = 0;
        if (v7 == 2 || a2->x == 0.0f)
            v33 = -1.0;
        else
            v33 = (v29 - a1->x) / a2->x;
        if (v32a != 2 && a2->y != 0.0f)
        {
            v12 = (v30 - a1->y) / a2->y;
            if (v12 > v33)
            {
                v33 = (float)v12;
                v11 = 1;
            }
        }
        if (v8 != 2 && a2->z != 0.0f)
        {
            v13 = (v31 - a1->z) / a2->z;
            if (v13 > v33)
            {
                v33 = (float)v13;
                v11 = 2;
            }
        }
        if (v33 < 0.0 || v33 >(double)a5)
            return 0;
        if (a6)
            *a6 = v33;
        if (v11)
        {
            v14 = v33 * a2->x + a1->x;
            v9->x = (float)v14;
            if (v14 < a3->pos1.x)
                return 0;
            if (v14 > v28.x)
                return 0;
            if (v11 == 1)
            {
                v9->y = v30;
            LABEL_47:
                v22 = v33 * a2->z + a1->z;
                v9->z = (float)v22;
                if (v22 >= a3->pos1.z) 
                {
                    if (v22 > v28.z)
                        return 0;
                    return v11 + 1;
                }
                return 0;
            }
        }
        else
        {
            v9->x = v29;
        }
        v18 = v33 * a2->y + a1->y;
        v9->y = (float)v18;
        if (v18 < a3->pos1.y) 
            return 0;
        if (v18 > v28.y)
            return 0;
        if (v11 == 2)
        {
            v9->z = v31;
            return v11 + 1;
        }
        goto LABEL_47;
    }
    *v9 = *a1;
    if (a6)
        *a6 = -1.0;
    return 1;
}

int INT_FullRaySphere(Vec3* a1, Vec3* a2, tColSphere* a3, Vec3* talanHitPos, float a5, float* a6)
{
    double v8;
    double v9;
    double v10;
    double v11;
    int result;
    double v13;
    long double v14;
    float v15;
    float v16;
    float a2a;
    float a3a;
    float a3b;

    v8 = a3->center.x - a1->x;
    v9 = a3->center.y - a1->y;
    v10 = a3->center.z - a1->z;
    v11 = a2->z * v10 + a2->y * v9 + v8 * a2->x;
    v16 = (float)v11;
    a2a = (float)(v10 * v10 + v8 * v8 + v9 * v9);
    a3a = a3->radius * a3->radius;
    if (a3a <= (double)a2a)
    {
        if (v16 < 0.0 && a2a >(double)a3a
            || (v15 = (float)(v11 * v16), v13 = a2a - v15, v13 > a3a)
            || (v14 = v16 - sqrt(a3a - v13), *a6 = (float)v14, v14 > a5))
        {
            result = 0;
        }
        else
        {
            if (talanHitPos)
            {
                a3b = (float)v14;
                MATH_VecMultScalar(talanHitPos, a2, a3b);
                MATH_VecAddVec(talanHitPos, a1, talanHitPos);
            }
            result = 1;
        }
    }
    else
    {
        *a6 = -1.0;
        result = 1;
    }
    return result;
}

int INT_b_FullRayCylinder(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, float a5, float a6, float a7, float* dist, Vec3* hitPosTalan)
{
    double v11;
    double v12;
    double v13;
    long double v14;
    int result;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float hitPosTalana;

    hitPosTalan->x = 0.0;
    hitPosTalan->y = 0.0;
    hitPosTalan->z = 0.0;
    v16 = a1->x - a3->x;
    v17 = a1->y - a3->y;
    v18 = a1->z - a3->z;
    v19 = v18 * a4->z + v16 * a4->x + v17 * a4->y;
    v11 = a4->x * a2->x + a4->z * a2->z + a2->y * a4->y;
    hitPosTalana = (float)v11;
    if (fabs(fabs(v11) - 1.0) < 0.0000099999997)
        goto LABEL_3;
    v20 = (float)(1.0 - hitPosTalana * hitPosTalana);
    v12 = v17 * a2->y + v18 * a2->z + v16 * a2->x - hitPosTalana * v19;
    v13 = v12 + v12;
    v14 = v13 * v13 - -(v19 * v19 + a5 * a5 - (v18 * v18 + v17 * v17 + v16 * v16)) * v20 * 4.0;
    if (v14 < 0.0)
        goto LABEL_3;
    v21 = (float)((-v13 - sqrt(fabs(v14))) / (v20 + v20));
    if (v21 < 0.0 || v21 >(double)a7)
        goto LABEL_3;
    hitPosTalan->x = v21 * a2->x;
    hitPosTalan->y = v21 * a2->y;
    hitPosTalan->z = v21 * a2->z;
    hitPosTalan->x = hitPosTalan->x + a1->x;
    hitPosTalan->y = a1->y + hitPosTalan->y;
    hitPosTalan->z = hitPosTalan->z + a1->z;
    if (dist)
        *dist = v21;
    if (fabs(v21 * hitPosTalana + v19) > a6)
        LABEL_3:
    result = 0;
    else
        result = 1;
    return result;
}

void COL_ColMap_Triangles_RayCast(tGameObjectMain* gao, tColMap* colMap)
{
    tCobMeshObjectMesh* v2;
    Mtx4x4* v3;
    BOOL v4;
    int v5;
    tCobMeshChunkElem* v6;
    Mtx4x4* v7;
    long double v8;
    tColCollisionObject* v9;
    tCobGameMaterialElems* v10;
    tCobMeshTriangleElem* v11;
    tCobMeshTriangleElem* v12;
    int v13;
    Vec3* v14;
    Vec3* v15;
    Vec3* v17;
    Vec3* v18;
    Vec3* v19;
    Vec3* v20;
    double v21;
    double v22;
    double v23;
    double v24;
    double v25;
    double v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    double v33;
    double v34;
    tCobMeshChunkElem* v35;
    BOOL v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    int v46;
    Vec3* v47;
    Vec3 dest;
    Vec3 v49;
    int v50;
    int v51;
    tCobMeshChunkElem* v52;
    float distance;
    int v54;
    float v55;
    float v56;
    float v57;
    tCobMeshObjectMesh* v58;
    tCobMeshChunkElem* v59;
    int v60;
    tCobMeshTriangleElem* v61;
    Vec3 a3;
    tCobMeshTriangleElem* v63;
    Vec3 a7;
    Mtx4x4 a2a;
    Mtx4x4 a1a;

    v2 = (tCobMeshObjectMesh*)(*colMap->collisionObjectPtr)->objectPtrByType;
    v3 = gao->matrices;
    v4 = (v3->mtxflag & 8) == 0;
    v58 = v2;
    if (v4)
    {
        MATH_InvertMatrix(&a1a, v3);
        v5 = 0;
    }
    else
    {
        if (&a2a != v3)
            memcpy(&a2a, v3, sizeof(a2a));
        a2a.mtxflag &= ~8u;
        v5 = 0;
        a2a._m14 = 0.0;
        a2a._m24 = 0.0;
        a2a._m34 = 0.0;
        MATH_InvertMatrix(&a1a, &a2a);
    }
    v6 = v2->meshChunkElems;
    v59 = &v6[v2->meshChunkElemNum];
    v7 = gao->matrices;
    v4 = (v7->mtxflag & 0x3C) == 0;
    v52 = v6;
    if (v4)
    {
        v47 = COL_gst_Ray.direction;
        dest.x = COL_gst_Ray.sourcePoint->x - v7->Pos.x;
        dest.y = COL_gst_Ray.sourcePoint->y - v7->Pos.y;
        dest.z = COL_gst_Ray.sourcePoint->z - v7->Pos.z;
    }
    else
    {
        MATH_TransformVertex(&dest, &a1a, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&v49, &a1a, COL_gst_Ray.direction);
        v47 = &v49;
        v8 = 1.0 / sqrt(v49.z * v49.z + v49.y * v49.y + v49.x * v49.x);
        v49.x = (float)(v49.x * v8);
        v49.y = (float)(v49.y * v8);
        v49.z = (float)(v49.z * v8);
    }
    v46 = 0;
    v54 = 0;
    if (v6 < v59)
    {
        while (COL_gst_Ray.skipFlagX || COL_gst_Ray.skipCrossable || COL_gst_Ray.skipCamera)
        {
            v9 = *colMap->collisionObjectPtr;
            if (!v9->gameMaterial)
                break;
            if (COL_gst_Ray.useUserCrossable)
            {
                v51 = COL_guw_UserCrossable;
            }
            else if (COL_gst_Ray.colMapGao && (COL_gst_Ray.colMapGao->flags & 0x200) != 0)
            {
                v51 = COL_gst_Ray.colMapGao->extended->coll->colInstance->crossable;
            }
            else
            {
                v51 = 0x589A;                           // == usercrossable?
            }
            v10 = COL_pst_GMat_Get(v9, v6);
            if (COL_gst_Ray.skipCrossable && v10 && ((uint16_t)v51 & v10->flags) != 0)
            {
                v5 += v6->elemNum;
                v46 = v5;
            }
            else if (COL_gst_Ray.skipCamera && v10 && (v10->flags & 0x1000) != 0)
            {
                v5 += v6->elemNum;
                v46 = v5;
            }
            else
            {
                if (!COL_gst_Ray.skipFlagX || !v10 || (v10->flags & 0x200000) == 0)
                    break;
                v5 += v6->elemNum;
                v46 = v5;
            }
        LABEL_61:
            v52 = ++v6;
            ++v54;
            if (v6 >= v59)
                return;
        }
        v11 = v6->triangleElems;
        v12 = &v11[v6->elemNum];
        v61 = v11;
        v63 = v12;
        v50 = 0;
        if (v11 >= v12)
            goto LABEL_61;
        v13 = v5;
        v60 = v13;
        while (1)
        {
            v14 = v2->normals;
            v15 = &v14[v13];
            a3.x = v15->x;
            a3.y = v15->y;
            a3.z = v15->z;
            v17 = v2->vertices;
            v18 = &v17[v11->v2];
            v19 = &v17[v11->v1];
            v20 = &v17[v11->v3];
            if (a3.y * v47->y + a3.z * v47->z + a3.x * v47->x <= 0.0)
            {
                v21 = COL_gst_Ray.distance * v47->x;
                v43 = v20->x;
                v22 = COL_gst_Ray.distance * v47->y;
                v57 = COL_gst_Ray.distance * v47->z;
                v55 = (float)(dest.x + v21);
                v56 = (float)(v22 + dest.y);
                v57 = dest.z + v57;
                v40 = v18->x;
                v37 = v19->x;
                if (v47->x <= 0.0)
                {
                    v25 = MATH_f_MinOf3(v37, v40, v43);
                    if (v25 > dest.x)
                        goto LABEL_59;
                    v26 = MATH_f_MaxOf3(v19->x, v18->x, v20->x);
                    if (v26 < v55)
                        goto LABEL_59;
                }
                else
                {
                    v23 = MATH_f_MaxOf3(v37, v40, v43);
                    if (v23 < dest.x)
                        goto LABEL_59;
                    v24 = MATH_f_MinOf3(v19->x, v18->x, v20->x);
                    if (v24 > v55)
                        goto LABEL_59;
                }
                v44 = v20->y;
                v41 = v18->y;
                v38 = v19->y;
                if (v47->y <= 0.0)
                {
                    v29 = MATH_f_MinOf3(v38, v41, v44);
                    if (v29 > dest.y)
                        goto LABEL_59;
                    v30 = MATH_f_MaxOf3(v19->y, v18->y, v20->y);
                    if (v30 < v56)
                        goto LABEL_59;
                }
                else
                {
                    v27 = MATH_f_MaxOf3(v38, v41, v44);
                    if (v27 < dest.y)
                        goto LABEL_59;
                    v28 = MATH_f_MinOf3(v19->y, v18->y, v20->y);
                    if (v28 > v56)
                        goto LABEL_59;
                }
                v45 = v20->z;
                v42 = v18->z;
                v39 = v19->z;
                if (v47->z <= 0.0)
                {
                    v33 = MATH_f_MinOf3(v39, v42, v45);
                    if (v33 > dest.z)
                        goto LABEL_59;
                    v34 = MATH_f_MaxOf3(v19->z, v18->z, v20->z);
                    if (v34 < v57)
                        goto LABEL_59;
                }
                else
                {
                    v31 = MATH_f_MaxOf3(v39, v42, v45);
                    if (v31 < dest.z)
                        goto LABEL_59;
                    v32 = MATH_f_MinOf3(v19->z, v18->z, v20->z);
                    if (v32 > v57)
                        goto LABEL_59;
                }
                if (INT_FullRayTriangle(&dest, v47, v19, v18, v20, &distance, &a7)
                    && distance < (double)COL_gst_Ray.distance
                    && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
                {
                    if (COL_gst_Ray.ignoreColMapRayCastResult)
                    {
                        *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                        return;
                    }
                    MATH_TransformVertexNoScale(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, gao->matrices, &a7);
                    MATH_TransformVectorNoScale(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, gao->matrices, &a3);
                    MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                    v35 = v52;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = v54;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = v50;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = v46;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = COL_pst_GMat_Get(*colMap->collisionObjectPtr, v35);
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = v35->designVal;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
                    COL_gst_Ray.gao = gao;
                    COL_gst_Ray.distance = distance;
                }
            }
        LABEL_59:
            v2 = v58;
            v11 = v61 + 1;
            ++v46;
            v13 = v60 + 1;
            v36 = &v61[1] < v63;
            ++v61;
            ++v60;
            ++v50;
            if (!v36)
            {
                v6 = v52;
                v5 = v46;
                goto LABEL_61;
            }
        }
    }
}

void COL_ColMap_OK3_RayCast(tGameObjectMain* gao, tColMap* colMap)
{
    Mtx4x4* v2;
    BOOL v3;
    tCobMeshObjectMesh* v4;
    tCobOK3* v5;
    Mtx4x4* v6;
    long double v7;
    BOOL v8;
    tCobOK3AABBoxElem* v9;
    int v10;
    int v11;
    tCobOK3AABBoxElemChunkElem* v12;
    int v13;
    int v14;
    tCobMeshChunkElem* v15;
    tColCollisionObject* v16;
    tCobGameMaterialElems* v17;
    int v18;
    tCobMeshTriangleElem* v19;
    Vec3* v20;
    int v21;
    tCobMeshTriangleElem* v22;
    Vec3* v23;
    Vec3* v24;
    Vec3* v25;
    Vec3* v26;
    int v27;
    float v28;
    Vec3* v29;
    float v30;
    double v31;
    double v32;
    double v33;
    double v34;
    double v35;
    double v36;
    double v37;
    double v38;
    double v39;
    double v40;
    double v41;
    double v42;
    double v43;
    double v44;
    tCobOK3AABBoxElemChunkElem* v45;
    int v46;
    int v47;
    int v48;
    float v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    Vec3* v57;
    int v58;
    int v59;
    tCobMeshObjectMesh* v60;
    Vec3 dest;
    int v62;
    Vec3 v63;
    float distance;
    int v65;
    tCobOK3AABBoxElemChunkElem* v66;
    unsigned int v67;
    float v68;
    float v69;
    float v70;
    int v71;
    tCobOK3AABBoxElem* v72;
    tCobGameMaterialElems* v73;
    unsigned int v74;
    tCobOK3* v75;
    Vec3 src;
    tColBox a3;
    Vec3 a7;
    Mtx4x4 a2a;
    Mtx4x4 a1a;

    v2 = gao->matrices;
    v3 = (v2->mtxflag & 8) == 0;
    v4 = (tCobMeshObjectMesh*)(*colMap->collisionObjectPtr)->objectPtrByType;
    v5 = v4->ok3;
    v60 = v4;
    v75 = v5;
    if (v3)
    {
        MATH_InvertMatrix(&a1a, v2);
    }
    else
    {
        if (&a2a != v2)
        {
            memcpy(&a2a, v2, sizeof(a2a));
            v5 = v75;
        }
        a2a._m14 = 0.0;
        a2a._m24 = 0.0;
        a2a._m34 = 0.0;
        a2a.mtxflag &= ~8u;
        MATH_InvertMatrix(&a1a, &a2a);
    }
    v6 = gao->matrices;
    if ((v6->mtxflag & 0x3C) != 0)
    {
        MATH_TransformVertex(&dest, &a1a, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&v63, &a1a, COL_gst_Ray.direction);
        v57 = &v63;
        v7 = 1.0 / sqrt(v63.z * v63.z + v63.y * v63.y + v63.x * v63.x);
        v63.x = (float)(v63.x * v7);
        v63.y = (float)(v63.y * v7);
        v63.z = (float)(v63.z * v7);
    }
    else
    {
        v57 = COL_gst_Ray.direction;
        dest.x = COL_gst_Ray.sourcePoint->x - v6->Pos.x;
        dest.y = COL_gst_Ray.sourcePoint->y - v6->Pos.y;
        dest.z = COL_gst_Ray.sourcePoint->z - v6->Pos.z;
    }
    v8 = v5->num <= 0;
    v74 = 0;
    if (!v8)
    {
        v67 = 0;
        do
        {
            v9 = &v5->aaBoxes[v67];
            a3.pos1.x = v9->vec1.x;
            a3.pos1.y = v9->vec1.y;
            a3.pos1.z = v9->vec1.z;
            a3.pos2 = v9->vec2;
            v72 = v9;
            if (INT_FullRayAABBox(&dest, v57, &a3, 0, COL_gst_Ray.distance, 0))
            {
                v10 = v9->num;
                v11 = 0;
                v71 = 0;
                if (v10 > 0)
                {
                    do
                    {
                        v12 = &v9->chunkElems[v11];
                        v13 = v12->triangleChunkID;
                        v14 = 0;
                        v66 = v12;
                        v58 = 0;
                        if (v13)
                        {
                            v15 = v4->meshChunkElems;
                            v59 = v13;
                            do
                            {
                                v14 += v15->elemNum;
                                ++v15;
                                --v59;
                            } while (v59);
                            v9 = v72;
                            v58 = v14;
                        }
                        v73 = 0;                            // 0x589A == usercrossable?
                        if (!COL_gst_Ray.skipFlagX && !COL_gst_Ray.skipCrossable && !COL_gst_Ray.skipCamera
                            || (v16 = *colMap->collisionObjectPtr, !v16->gameMaterial)
                            || (!COL_gst_Ray.useUserCrossable ? (!COL_gst_Ray.colMapGao || (COL_gst_Ray.colMapGao->flags & 0x200) == 0 ? (v65 = 0x589A) : (v65 = COL_gst_Ray.colMapGao->extended->coll->colInstance->crossable)) : (v65 = COL_guw_UserCrossable),
                                ((v17 = COL_pst_GMat_Get(v16, &v4->meshChunkElems[v13]), v73 = v17, !COL_gst_Ray.skipCrossable)
                                    || !v17
                                    || ((uint16_t)v65 & v17->flags) == 0)
                                && (!COL_gst_Ray.skipCamera || !v17 || (v17->flags & 0x1000) == 0)
                                && (!COL_gst_Ray.skipFlagX || !v17 || (v17->flags & 0x200000) == 0)))
                        {
                            v3 = v12->num == 0;
                            v62 = 0;
                            if (!v3)
                            {
                                while (1)
                                {
                                    v18 = v12->triangleChunkElemIDs[v62];
                                    v19 = v4->meshChunkElems[v12->triangleChunkID].triangleElems;
                                    v20 = v4->vertices;
                                    v21 = v19[v18].v2;
                                    v22 = &v19[v18];
                                    v23 = &v20[v22->v3];
                                    v24 = &v20[v22->v1];
                                    v25 = &v20[v21];
                                    v26 = v60->normals;
                                    v27 = v58 + v18;
                                    v28 = v26[v27].x;
                                    v29 = &v26[v27];
                                    src.x = v28;
                                    src.y = v29->y;
                                    src.z = v29->z;
                                    if (src.z * v57->z + v28 * v57->x + src.y * v57->y > 0.0)
                                        goto LABEL_64;
                                    v30 = v25->x;
                                    v31 = COL_gst_Ray.distance * v57->x;
                                    v32 = COL_gst_Ray.distance * v57->y;
                                    v70 = COL_gst_Ray.distance * v57->z;
                                    v68 = (float)(dest.x + v31);
                                    v69 = (float)(v32 + dest.y);
                                    v70 = dest.z + v70;
                                    v54 = v23->x;
                                    v49 = v24->x;
                                    if (v57->x <= 0.0)
                                    {
                                        v35 = MATH_f_MinOf3(v49, v30, v54);
                                        if (v35 > dest.x)
                                            goto LABEL_64;
                                        v36 = MATH_f_MaxOf3(v24->x, v25->x, v23->x);
                                        if (v36 < v68)
                                            goto LABEL_64;
                                    }
                                    else
                                    {
                                        v33 = MATH_f_MaxOf3(v49, v30, v54);
                                        if (v33 < dest.x)
                                            goto LABEL_64;
                                        v34 = MATH_f_MinOf3(v24->x, v25->x, v23->x);
                                        if (v34 > v68)
                                            goto LABEL_64;
                                    }
                                    v55 = v23->y;
                                    v52 = v25->y;
                                    v50 = v24->y;
                                    if (v57->y <= 0.0)
                                    {
                                        v39 = MATH_f_MinOf3(v50, v52, v55);
                                        if (v39 > dest.y)
                                            goto LABEL_64;
                                        v40 = MATH_f_MaxOf3(v24->y, v25->y, v23->y);
                                        if (v40 < v69)
                                            goto LABEL_64;
                                    }
                                    else
                                    {
                                        v37 = MATH_f_MaxOf3(v50, v52, v55);
                                        if (v37 < dest.y)
                                            goto LABEL_64;
                                        v38 = MATH_f_MinOf3(v24->y, v25->y, v23->y);
                                        if (v38 > v69)
                                            goto LABEL_64;
                                    }
                                    v56 = v23->z;
                                    v53 = v25->z;
                                    v51 = v24->z;
                                    if (v57->z <= 0.0)
                                    {
                                        v43 = MATH_f_MinOf3(v51, v53, v56);
                                        if (v43 > dest.z)
                                            goto LABEL_64;
                                        v44 = MATH_f_MaxOf3(v24->z, v25->z, v23->z);
                                        if (v44 < v70)
                                            goto LABEL_64;
                                    }
                                    else
                                    {
                                        v41 = MATH_f_MaxOf3(v51, v53, v56);
                                        if (v41 < dest.z)
                                            goto LABEL_64;
                                        v42 = MATH_f_MinOf3(v24->z, v25->z, v23->z);
                                        if (v42 > v70)
                                            goto LABEL_64;
                                    }
                                    if (INT_FullRayTriangle(&dest, v57, v24, v25, v23, &distance, &a7)
                                        && distance < (double)COL_gst_Ray.distance
                                        && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
                                    {
                                        if (COL_gst_Ray.ignoreColMapRayCastResult)
                                        {
                                            *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                                            return;
                                        }
                                        MATH_TransformVertexNoScale(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, gao->matrices, &a7);
                                        MATH_TransformVectorNoScale(
                                            &COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec,
                                            gao->matrices,
                                            &src);
                                        MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                                        v45 = v66;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = v66->triangleChunkID;
                                        v46 = v62;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = v45->triangleChunkElemIDs[v62];
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = v58
                                            + v45->triangleChunkElemIDs[v46];
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = v73;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = v60->meshChunkElems[v45->triangleChunkID].designVal;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
                                        COL_gst_Ray.distance = distance;
                                        COL_gst_Ray.gao = gao;
                                    }
                                LABEL_64:
                                    v47 = v66->num;
                                    v4 = v60;
                                    if (++v62 >= v47)
                                    {
                                        v9 = v72;
                                        break;
                                    }
                                    v12 = v66;
                                }
                            }
                        }
                        v48 = v9->num;
                        v11 = ++v71;
                    } while (v71 < v48);
                    v5 = v75;
                }
            }
            v8 = ++v74 < v5->num;
            ++v67;
        } while (v8);
    }
}

void COL_Visual_OK3_RayCast(tGameObjectMain* gao, tGEOGRO* geogro)
{
    tGEOGRO* v2;
    tCobOK3* v3;
    Vec3* v4;
    Mtx4x4* v5;
    Vec3* v6;
    double v7;
    long double v8;
    BOOL v9;
    tCobOK3AABBoxElem* v10;
    int v11;
    int v12;
    tCobOK3AABBoxElemChunkElem* v13;
    int v14;
    int triangleNumSum;
    tGeoTriangleChunks* v16;
    int v17;
    int ii;
    BOOL numEq0;
    Vec3* v20;
    tGeoTriangleChunkElem* v21;
    Vec3* v22; 
    Vec3* v23; 
    Vec3* v24; 
    double v25;
    double v26;
    double v27;
    double v28;
    float v29;
    double v30;
    double v31;
    double v32;
    double v33;
    double v34;
    double v35;
    double v36;
    double v37;
    double v38;
    double v39;
    double v40;
    double v41;
    double v42;
    double v43;
    Vec3* v44;
    tCobOK3AABBoxElemChunkElem* v45;
    int iii;
    int nnum;
    int v48;
    Vec3* v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    Mtx4x4* v55;
    float v56;
    float v57;
    float v58;
    float distance;
    float a5;
    Vec3* v61;
    Vec3 src;
    Vec3 a3;
    tCobOK3AABBoxElemChunkElem* v64;
    Vec3 dest;
    Vec3 v66;
    int v67;
    int i;
    float v69;
    float v70;
    float v71;
    Vec3 v72;
    int v73;
    int ttriangleNumSum;
    unsigned int j;
    tCobOK3* v76;
    tCobOK3AABBoxElem* v77;
    float v78;
    float v79;
    Vec3 a7;
    tColBox v81;
    Mtx4x4 a2a;
    Mtx4x4 a1a;

    v2 = geogro;
    v3 = geogro->collisionPtr;
    v55 = gao->matrices;
    distance = 0.0;
    v76 = v3;
    MATH_MakeOGLMatrix(&a1a, v55);
    a1a._m14 = 0.0;
    a1a._m24 = 0.0;
    a1a._m34 = 0.0;
    a1a.mtxflag &= ~8u;
    MATH_Invert33Matrix(&a2a, &a1a);
    a2a._m14 = 0.0;
    a2a._m24 = 0.0;
    a2a._m34 = 0.0;
    v4 = &gao->matrices->Pos;
    a2a.mtxflag &= ~8u;
    a2a.Pos.x = -v4->x;
    a2a.Pos.y = -v4->y;
    a2a.Pos.z = -v4->z;
    MATH_TransformVector(&a2a.Pos, &a2a, &a2a.Pos);
    v5 = gao->matrices;
    if ((v5->mtxflag & 0x3C) != 0)
    {
        MATH_TransformVertex(&dest, &a2a, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&v66, &a2a, COL_gst_Ray.direction);
        v6 = COL_gst_Ray.direction;
        v61 = &v66;
        v8 = 1.0 / sqrt(v66.z * v66.z + v66.y * v66.y + v66.x * v66.x);
        v66.x = (float)(v66.x * v8);
        v66.y = (float)(v66.y * v8);
        v66.z = (float)(v66.z * v8);
    }
    else
    {
        v6 = COL_gst_Ray.direction;
        v7 = COL_gst_Ray.sourcePoint->x - v5->Pos.x;
        v61 = COL_gst_Ray.direction;
        dest.x = (float)v7;
        dest.y = (float)(COL_gst_Ray.sourcePoint->y - v5->Pos.y);
        dest.z = (float)(COL_gst_Ray.sourcePoint->z - v5->Pos.z);
    }
    if ((gao->matrices->mtxflag & 8) != 0)
    {
        a3.x = COL_gst_Ray.distance * v6->x;
        a3.y = v6->y * COL_gst_Ray.distance;
        a3.z = v6->z * COL_gst_Ray.distance;
        MATH_TransformVector(&src, &a2a, &a3);
        a5 = (float)sqrt(src.x * src.x + src.z * src.z + src.y * src.y);
    }
    else
    {
        a5 = COL_gst_Ray.distance;
    }
    v9 = v3->num <= 0;
    j = 0;
    if (!v9)
    {
        v67 = 0;
        do
        {
            v10 = &v3->aaBoxes[v67];
            v81.pos1.x = v10->vec1.x;
            v81.pos1.y = v10->vec1.y;
            v81.pos1.z = v10->vec1.z;
            v81.pos2 = v10->vec2;
            v77 = v10;
            if (INT_FullRayAABBox(&dest, v61, &v81, 0, a5, 0))
            {
                if ((gao->matrices->mtxflag & 8) != 0)
                {
                    src.x = distance * COL_gst_Ray.direction->x;
                    src.y = COL_gst_Ray.direction->y * distance;
                    src.z = COL_gst_Ray.direction->z * distance;
                    MATH_TransformVector(&a3, &a1a, &src);
                    distance = (float)sqrt(a3.x * a3.x + a3.z * a3.z + a3.y * a3.y);
                }
                v11 = v10->num;
                v12 = 0;
                v73 = 0;
                if (v11 > 0)
                {
                    do
                    {
                        v13 = &v10->chunkElems[v12];
                        v14 = v13->triangleChunkID;
                        triangleNumSum = 0;
                        v64 = v13;
                        ttriangleNumSum = 0;
                        if (v14)
                        {
                            v16 = v2->triangleChunks;
                            v17 = v14;
                            do
                            {
                                triangleNumSum += v16->triangleNum;
                                ++v16;
                                --v17;
                            } while (v17);
                            v13 = v64;
                            ttriangleNumSum = triangleNumSum;
                        }
                        if (v2->triangleChunks[v14].triangleChunkElems)
                        {
                            ii = 0;
                            numEq0 = v13->num == 0;
                            i = 0;
                            if (!numEq0)
                            {
                                while (1)
                                {
                                    v20 = v2->vertices;
                                    v21 = &v2->triangleChunks[v13->triangleChunkID].triangleChunkElems[v13->triangleChunkElemIDs[ii]];
                                    v22 = &v20[v21->v1];
                                    v23 = &v20[v21->v2];
                                    v24 = &v20[v21->v3];
                                    v25 = v23->x - v22->x;
                                    v26 = v23->y - v22->y;
                                    v27 = v23->z - v22->z;
                                    v78 = v24->x - v22->x;
                                    v79 = v24->y - v22->y;
                                    v28 = v24->z - v22->z;
                                    v72.x = (float)(v28 * v26 - v79 * v27);
                                    v72.y = (float)(v27 * v78 - v28 * v25);
                                    v72.z = (float)(v79 * v25 - v26 * v78);
                                    if (v72.y * v61->y + v72.x * v61->x + v72.z * v61->z > 0.0)
                                        goto LABEL_56;
                                    v29 = v22->x;
                                    v30 = a5 * v61->x;
                                    v31 = a5 * v61->y;
                                    v71 = a5 * v61->z;
                                    v69 = (float)(dest.x + v30);
                                    v70 = (float)(v31 + dest.y);
                                    v71 = dest.z + v71;
                                    v56 = v24->x;
                                    v52 = v23->x;
                                    if (v61->x <= 0.0)
                                    {
                                        v34 = MATH_f_MinOf3(v29, v52, v56);
                                        if (v34 > dest.x)
                                            goto LABEL_56;
                                        v35 = MATH_f_MaxOf3(v22->x, v23->x, v24->x);
                                        if (v35 < v69)
                                            goto LABEL_56;
                                    }
                                    else
                                    {
                                        v32 = MATH_f_MaxOf3(v29, v52, v56);
                                        if (v32 < dest.x)
                                            goto LABEL_56;
                                        v33 = MATH_f_MinOf3(v22->x, v23->x, v24->x);
                                        if (v33 > v69)
                                            goto LABEL_56;
                                    }
                                    v57 = v24->y;
                                    v53 = v23->y;
                                    v50 = v22->y;
                                    if (v61->y <= 0.0)
                                    {
                                        v38 = MATH_f_MinOf3(v50, v53, v57);
                                        if (v38 > dest.y)
                                            goto LABEL_56;
                                        v39 = MATH_f_MaxOf3(v22->y, v23->y, v24->y);
                                        if (v39 < v70)
                                            goto LABEL_56;
                                    }
                                    else
                                    {
                                        v36 = MATH_f_MaxOf3(v50, v53, v57);
                                        if (v36 < dest.y)
                                            goto LABEL_56;
                                        v37 = MATH_f_MinOf3(v22->y, v23->y, v24->y);
                                        if (v37 > v70)
                                            goto LABEL_56;
                                    }
                                    v58 = v24->z;
                                    v54 = v23->z;
                                    v51 = v22->z;
                                    if (v61->z <= 0.0)
                                    {
                                        v42 = MATH_f_MinOf3(v51, v54, v58);
                                        if (v42 > dest.z)
                                            goto LABEL_56;
                                        v43 = MATH_f_MaxOf3(v22->z, v23->z, v24->z);
                                        if (v43 < v71)
                                            goto LABEL_56;
                                    }
                                    else
                                    {
                                        v40 = MATH_f_MaxOf3(v51, v54, v58);
                                        if (v40 < dest.z)
                                            goto LABEL_56;
                                        v41 = MATH_f_MinOf3(v22->z, v23->z, v24->z);
                                        if (v41 > v71)
                                            goto LABEL_56;
                                    }
                                    v49 = v23;
                                    v44 = v61;
                                    if (INT_FullRayTriangle(&dest, v61, v22, v49, v24, &distance, &a7)
                                        && distance < (double)a5
                                        && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
                                    {
                                        if (COL_gst_Ray.ignoreColMapRayCastResult)
                                        {
                                            *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                                            return;
                                        }
                                        if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                                            COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = a7;
                                        else
                                            MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &a1a, &a7);
                                        if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                                            COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec = v72;
                                        else
                                            MATH_TransformVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, &a1a, &v72);
                                        MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                                        v45 = v64;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = v64->triangleChunkID;
                                        iii = i;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = v45->triangleChunkElemIDs[i];
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = ttriangleNumSum
                                            + v45->triangleChunkElemIDs[iii];
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
                                        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
                                        COL_gst_Ray.gao = gao;
                                        if ((gao->matrices->mtxflag & 8) != 0)
                                        {
                                            src.x = distance * v44->x;
                                            src.y = distance * v44->y;
                                            src.z = distance * v44->z;
                                            MATH_TransformVector(&a3, &a1a, &src);
                                            distance = MATH_VecLen(&a3);
                                        }
                                        COL_gst_Ray.distance = distance;
                                        if ((gao->matrices->mtxflag & 8) != 0)
                                        {
                                            a3.x = distance * COL_gst_Ray.direction->x;
                                            a3.y = COL_gst_Ray.direction->y * distance;
                                            a3.z = COL_gst_Ray.direction->z * distance;
                                            MATH_TransformVector(&src, &a2a, &a3);
                                            a5 = MATH_VecLen(&src);
                                        }
                                        else
                                        {
                                            a5 = distance;
                                        }
                                    }
                                LABEL_56:
                                    nnum = v64->num;
                                    v2 = geogro;
                                    ii = ++i;
                                    if (i >= nnum)
                                    {
                                        v10 = v77;
                                        break;
                                    }
                                    v13 = v64;
                                }
                            }
                        }
                        v48 = v10->num;
                        v12 = ++v73;
                    } while (v73 < v48);
                    v3 = v76;
                }
            }
            v9 = ++j < v3->num;
            ++v67;
        } while (v9);
    }
}

void COL_Visual_Triangles_RayCast(tGameObjectMain* gao, tGEOGRO* geogro)
{
    Vec3* v2; // eax
    Mtx4x4* v3; // eax
    Vec3* v4; // edx
    double v5; // st7
    long double v6; // st7
    tGeoTriangleChunks* v7; // ecx
    tGeoTriangleChunks* v8; // edi
    tGeoTriangleChunkElem* v9; // eax
    tGeoTriangleChunkElem* v10; // edx
    Vec3* v11; // ebx
    Vec3* v12; // esi
    Vec3* v13; // edi
    double v14; // st7
    float v15; // ecx
    Vec3* v16; // ebx
    double v17; // st6
    float v18; // edx
    double v19; // st7
    double v20; // st7
    double v21; // st7
    double v22; // st7
    double v23; // st7
    double v24; // st7
    double v25; // st7
    double v26; // st7
    double v27; // st7
    double v28; // st7
    double v29; // st7
    double v30; // st7
    double v31; // st7
    double v32; // st6
    double v33; // st5
    double v34; // st4
    BOOL v35; // cf
    float v36; // [esp-Ch] [ebp-14Ch]
    float v37; // [esp-Ch] [ebp-14Ch]
    float v38; // [esp-8h] [ebp-148h]
    float v39; // [esp-8h] [ebp-148h]
    float v40; // [esp-4h] [ebp-144h]
    float v41; // [esp-4h] [ebp-144h]
    float v42; // [esp-4h] [ebp-144h]
    float distance; // [esp+18h] [ebp-128h] BYREF
    float v44; // [esp+1Ch] [ebp-124h]
    Vec3* v45; // [esp+20h] [ebp-120h]
    Vec3 v46; // [esp+24h] [ebp-11Ch] BYREF
    Vec3 v47; // [esp+30h] [ebp-110h] BYREF
    int v48; // [esp+3Ch] [ebp-104h]
    int v49; // [esp+40h] [ebp-100h]
    int v50; // [esp+44h] [ebp-FCh]
    tGeoTriangleChunkElem* v51; // [esp+48h] [ebp-F8h]
    float v52; // [esp+4Ch] [ebp-F4h]
    float v53; // [esp+50h] [ebp-F0h]
    float v54; // [esp+54h] [ebp-ECh]
    Vec3 v55; // [esp+58h] [ebp-E8h] BYREF
    Vec3 v56; // [esp+64h] [ebp-DCh] BYREF
    tGeoTriangleChunks* v57; // [esp+70h] [ebp-D0h]
    Vec3 a3; // [esp+74h] [ebp-CCh] BYREF
    tGeoTriangleChunks* v59; // [esp+80h] [ebp-C0h]
    tGeoTriangleChunkElem* v60; // [esp+84h] [ebp-BCh]
    float v61; // [esp+88h] [ebp-B8h]
    float v62; // [esp+8Ch] [ebp-B4h]
    Vec3 a7; // [esp+94h] [ebp-ACh] BYREF
    Mtx4x4 a2a; // [esp+A0h] [ebp-A0h] BYREF
    Mtx4x4 a1a; // [esp+F0h] [ebp-50h] BYREF

    MATH_MakeOGLMatrix(&a1a, gao->matrices);
    a1a._m14 = 0.0;
    a1a._m24 = 0.0;
    a1a._m34 = 0.0;
    a1a.mtxflag &= ~8u;
    MATH_Invert33Matrix(&a2a, &a1a);
    a2a.mtxflag &= ~8u;
    v2 = &gao->matrices->Pos;
    a2a._m14 = 0.0;
    a2a._m24 = 0.0;
    a2a._m34 = 0.0;
    a2a.Pos.x = -v2->x;
    a2a.Pos.y = -v2->y;
    a2a.Pos.z = -v2->z;
    MATH_TransformVector(&a2a.Pos, &a2a, &a2a.Pos);
    v3 = gao->matrices;
    if ((v3->mtxflag & 0x3C) != 0)
    {
        MATH_TransformVertex(&v46, &a2a, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&v47, &a2a, COL_gst_Ray.direction);
        v4 = COL_gst_Ray.direction;
        v45 = &v47;
        v6 = 1.0 / sqrt(v47.z * v47.z + v47.y * v47.y + v47.x * v47.x);
        v47.x = (float)(v47.x * v6);
        v47.y = (float)(v47.y * v6);
        v47.z = (float)(v47.z * v6);
    }
    else
    {
        v4 = COL_gst_Ray.direction;
        v5 = COL_gst_Ray.sourcePoint->x - v3->Pos.x;
        v45 = COL_gst_Ray.direction;
        v46.x = (float)v5;
        v46.y = COL_gst_Ray.sourcePoint->y - v3->Pos.y;
        v46.z = COL_gst_Ray.sourcePoint->z - v3->Pos.z;
    }
    if ((gao->matrices->mtxflag & 8) != 0)
    {
        a3.x = COL_gst_Ray.distance * v4->x;
        a3.y = v4->y * COL_gst_Ray.distance;
        a3.z = v4->z * COL_gst_Ray.distance;
        MATH_TransformVector(&v55, &a2a, &a3);
        v44 = (float)sqrt(v55.z * v55.z + v55.y * v55.y + v55.x * v55.x);
    }
    else
    {
        v44 = COL_gst_Ray.distance;
    }
    v7 = geogro->triangleChunks;
    v8 = &v7[geogro->triangleChunkCount];
    v57 = v7;
    v59 = v8;
    v49 = 0;
    v48 = 0;
    if (v7 < v8)
    {
        while (1)
        {
            v9 = v7->triangleChunkElems;
            if (v9)
            {
                v10 = &v9[v7->triangleNum];
                v51 = v7->triangleChunkElems;
                v60 = v10;
                v50 = 0;
                if (v9 < v10)
                    break;
            }
        LABEL_49:
            v57 = ++v7;
            ++v48;
            if (v7 >= v8)
                return;
        }
        while (1)
        {
            v11 = geogro->vertices;
            v12 = &v11[v9->v1];
            v13 = &v11[v9->v2];
            v14 = v44 * v45->x;
            v15 = v13->x;
            v16 = &v11[v9->v3];
            v17 = v44 * v45->y;
            v18 = v12->x;
            v54 = v44 * v45->z;
            v52 = (float)(v46.x + v14);
            v53 = (float)(v17 + v46.y);
            v54 = v46.z + v54;
            v40 = v16->x;
            if (v45->x <= 0.0)
            {
                v21 = MATH_f_MinOf3(v18, v15, v40);
                if (v21 > v46.x)
                    goto LABEL_47;
                v22 = MATH_f_MaxOf3(v12->x, v13->x, v16->x);
                if (v22 < v52)
                    goto LABEL_47;
            }
            else
            {
                v19 = MATH_f_MaxOf3(v18, v15, v40);
                if (v19 < v46.x)
                    goto LABEL_47;
                v20 = MATH_f_MinOf3(v12->x, v13->x, v16->x);
                if (v20 > v52)
                    goto LABEL_47;
            }
            v41 = v16->y;
            v38 = v13->y;
            v36 = v12->y;
            if (v45->y <= 0.0)
            {
                v25 = MATH_f_MinOf3(v36, v38, v41);
                if (v25 > v46.y)
                    goto LABEL_47;
                v26 = MATH_f_MaxOf3(v12->y, v13->y, v16->y);
                if (v26 < v53)
                    goto LABEL_47;
            }
            else
            {
                v23 = MATH_f_MaxOf3(v36, v38, v41);
                if (v23 < v46.y)
                    goto LABEL_47;
                v24 = MATH_f_MinOf3(v12->y, v13->y, v16->y);
                if (v24 > v53)
                    goto LABEL_47;
            }
            v42 = v16->z;
            v39 = v13->z;
            v37 = v12->z;
            if (v45->z <= 0.0)
            {
                v29 = MATH_f_MinOf3(v37, v39, v42);
                if (v29 > v46.z)
                    goto LABEL_47;
                v30 = MATH_f_MaxOf3(v12->z, v13->z, v16->z);
                if (v30 < v54)
                    goto LABEL_47;
            }
            else
            {
                v27 = MATH_f_MaxOf3(v37, v39, v42);
                if (v27 < v46.z)
                    goto LABEL_47;
                v28 = MATH_f_MinOf3(v12->z, v13->z, v16->z);
                if (v28 > v54)
                    goto LABEL_47;
            }
            v31 = v13->x - v12->x;
            v32 = v13->y - v12->y;
            v33 = v13->z - v12->z;
            v61 = v16->x - v12->x;
            v62 = v16->y - v12->y;
            v34 = v16->z - v12->z;
            v56.x = (float)(v34 * v32 - v62 * v33);
            v56.y = (float)(v33 * v61 - v34 * v31);
            v56.z = (float)(v62 * v31 - v32 * v61);
            if (v56.z * v45->z + v56.y * v45->y + v56.x * v45->x <= 0.0
                && INT_FullRayTriangle(&v46, v45, v12, v13, v16, &distance, &a7)
                && distance < (double)v44
                && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
            {
                if (COL_gst_Ray.ignoreColMapRayCastResult)
                {
                    *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                    return;
                }
                if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                    COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = a7;
                else
                    MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &a1a, &a7);
                if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec = v56;
                else
                    MATH_TransformVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, &a1a, &v56);
                MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = v48;
                COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = v50;
                COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = v49;
                COL_gst_Ray.gao = gao;
                if ((gao->matrices->mtxflag & 8) != 0)
                {
                    MATH_VecMultScalar(&v55, v45, distance);
                    MATH_TransformVector(&a3, &a1a, &v55);
                    distance = MATH_VecLen(&a3);
                }
                COL_gst_Ray.distance = distance;
                if ((gao->matrices->mtxflag & 8) != 0)
                {
                    MATH_VecMultScalar(&a3, COL_gst_Ray.direction, distance);
                    MATH_TransformVector(&v55, &a2a, &a3);
                    v44 = MATH_VecLen(&v55);
                }
                else
                {
                    v44 = distance;
                }
            }
        LABEL_47:
            v35 = &v51[1] < v60;
            ++v51;
            ++v49;
            ++v50;
            if (!v35)
            {
                v7 = v57;
                v8 = v59;
                goto LABEL_49;
            }
            v9 = v51;
        }
    }
}

void COL_ColMap_Box_RayCast(tGameObjectMain* gao, tCobNormalObjectGeneric * box)
{
    Vec3* v2;
    long double v3;
    tColBox* v4;
    Mtx4x4* v5;
    float a6;
    Vec3 v7;
    Vec3 a3;
    Vec3 v9;
    float a5;
    tColBox colBox;
    Vec3 a4;
    Mtx4x4 a2a;
    Vec3 v14;
    Mtx4x4 a1a;

    MATH_MakeOGLMatrix(&a1a, gao->matrices);
    a1a._m14 = 0.0;
    a1a._m24 = 0.0;
    a1a._m34 = 0.0;
    a1a.mtxflag &= ~8u;
    MATH_Invert33Matrix(&a2a, &a1a);
    v2 = &gao->matrices->Pos;
    a2a.mtxflag &= ~8u;
    a2a._m14 = 0.0;
    a2a._m24 = 0.0;
    a2a._m34 = 0.0;
    a2a.Pos.x = -v2->x;
    a2a.Pos.y = -v2->y;
    a2a.Pos.z = -v2->z;
    MATH_TransformVector(&a2a.Pos, &a2a, &a2a.Pos);
    MATH_TransformVertex(&v14, &a2a, COL_gst_Ray.sourcePoint);
    MATH_TransformVector(&v7, &a2a, COL_gst_Ray.direction);
    v3 = 1.0 / sqrt(v7.z * v7.z + v7.y * v7.y + v7.x * v7.x);
    v7.x = (float)(v7.x * v3);
    v7.y = (float)(v7.y * v3);
    v4 = (tColBox *)(box->enclosurePtr);
    v7.z = (float)(v3 * v7.z);
    colBox.pos2 = v4->pos1;
    colBox.pos1.x = v4->pos2.x;
    v5 = gao->matrices;
    colBox.pos1.y = v4->pos2.y;
    colBox.pos1.z = v4->pos2.z;
    if ((v5->mtxflag & 8) != 0)
    {
        a3.x = COL_gst_Ray.distance * COL_gst_Ray.direction->x;
        a3.y = COL_gst_Ray.direction->y * COL_gst_Ray.distance;
        a3.z = COL_gst_Ray.direction->z * COL_gst_Ray.distance;
        MATH_TransformVector(&v9, &a2a, &a3);
        a5 = (float)sqrt(v9.z * v9.z + v9.y * v9.y + v9.x * v9.x);
    }
    else
    {
        a5 = COL_gst_Ray.distance;
    }
    if (INT_FullRayAABBox(&v14, &v7, &colBox, &a4, a5, &a6))
    {
        if ((gao->matrices->mtxflag & 8) != 0)
        {
            v9.x = a6 * COL_gst_Ray.direction->x;
            v9.y = COL_gst_Ray.direction->y * a6;
            v9.z = COL_gst_Ray.direction->z * a6;
            MATH_TransformVector(&a3, &a1a, &v9);
            a6 = (float)sqrt(a3.z * a3.z + a3.y * a3.y + a3.x * a3.x);
        }
        if (!COL_gb_UseMinDistance || a6 > (double)COL_gf_MinDistance)
        {
            MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &a1a, &a4);
            MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &a1a, &a4);
            COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = -1;
            COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = -1;
            COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = -1;
            COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
            COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
            COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
            COL_gst_Ray.gao = gao;
            COL_gst_Ray.distance = a6;
        }
    }
}

void COL_ColMap_Sphere_RayCast(tGameObjectMain* a1, tCobNormalObjectGeneric* a2)
{
    tGameObjectMain* v2;
    tCobNormalObjectGeneric* v3;
    Mtx4x4* v4;
    Mtx4x4* v5;
    double v6;
    double v7;
    double v8;
    double v9;
    Vec3* v10;
    tColSphere* v11;
    Vec3 a4;
    Vec3 v13;
    float v14;
    tColSphere a3;
    float v16;
    float a1a;

    v2 = a1;
    v3 = a2;
    v11 = a2->enclosurePtr;
    v4 = OBJ_pst_GetAbsoluteMatrix(a1);
    MATH_TransformVertex(&a3.center, v4, &v11->center);
    if ((v2->matrices->mtxflag & 8) != 0)
    {
        v5 = OBJ_pst_GetAbsoluteMatrix(v2);
        MATH_GetScale(&v13, v5);
        v16 = ((tColSphere*)(v3->enclosurePtr))->radius;
        v6 = MATH_f_MaxOf3(v13.x, v13.y, v13.z);
        v7 = v6 * v16;
    }
    else
    {
        v7 = ((tColSphere*)(v3->enclosurePtr))->radius;
    }
    a3.radius = (float)v7;
    if (INT_FullRaySphere(COL_gst_Ray.sourcePoint, COL_gst_Ray.direction, &a3, &a4, COL_gst_Ray.distance, &a1a)
        && (!COL_gb_UseMinDistance || a1a > (double)COL_gf_MinDistance))
    {
        COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = a4;
        v8 = a4.x - a3.center.x;
        v9 = a4.y - a3.center.y;
        v10 = &COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec;
        v14 = a4.z - a3.center.z;
        v16 = (float)(1.0 / sqrt(v14 * v14 + v9 * v9 + v8 * v8));// normalizevector inline
        COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec.x = (float)(v8 * v16);
        v10->y = (float)(v9 * v16);
        v10->z = v14 * v16;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = v2;
        COL_gst_Ray.gao = v2;
        COL_gst_Ray.distance = a1a;
    }
}

void COL_ColMap_Cylinder_RayCast(tGameObjectMain* a1, tCobNormalObjectGeneric* a2)
{
    tCobNormalObjectGeneric* v2;
    Mtx4x4* v3;
    Mtx4x4* v4;
    double v5;
    Vec3* v6;
    Vec3* v7;
    tColCylinder* v8;
    float v9;
    float v10;
    Vec3 v11;
    Vec3 v12;
    Vec3 v13;
    float a2a;

    v2 = a2;
    v8 = (tColCylinder*)(a2->enclosurePtr);
    v3 = OBJ_pst_GetAbsoluteMatrix(a1);
    MATH_TransformVertex(&v13, v3, &v8->center);
    v4 = OBJ_pst_GetAbsoluteMatrix(a1);
    MATH_GetScale(&v11, v4);
    if (v11.x < (double)v11.y)
        v5 = v11.y;
    else
        v5 = v11.x;
    v10 = (float)(((tColCylinder*)(v2->enclosurePtr))->height * v5);
    v9 = ((tColCylinder*)(v2->enclosurePtr))->radius * v11.z;
    if (INT_b_FullRayCylinder(COL_gst_Ray.sourcePoint, COL_gst_Ray.direction, &v13, &MATH_gst_BaseVectorK, v10, v9, COL_gst_Ray.distance, &a2a, &v12)
        && (!COL_gb_UseMinDistance || a2a > (double)COL_gf_MinDistance))
    {
        COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = v12;
        v6 = COL_gst_Ray.direction;
        v7 = &COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec.x = -COL_gst_Ray.direction->x;
        v7->y = -v6->y;
        v7->z = -v6->z;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = a1;
        COL_gst_Ray.gao = a1;
        COL_gst_Ray.distance = a2a;
    }
}

// TODO ÁTNÉZNI!
int COL_RaySphereBV(Vec3* a1, Vec3* a2, tGameObjectMain* a3, float* a6, float a5)
{
    tGameObjectBV* v5;
    Mtx4x4* v6;
    double v7;
    int result; 
    tColSphere a3a;
    float x, y, z;
    float tmp;

    v5 = a3->boundingVolume;
    v6 = a3->matrices;
    v7 = v5->BVMin.x + v6->Pos.x;
    a3a.center.x = (float)v7;
    a3a.center.y = v5->BVMin.y + v6->Pos.y;
    a3a.center.z = v5->BVMin.z + v6->Pos.z;
    a3a.radius = v5->BVMax.x;


    tmp = v5->BVMax.x;   
    x = tmp;
    if (a2->x < 0.0) { // TODO FIXME assemblyben megnéztem, olyan mint ha el lenne írva, mert mindenhez az x-et nézi, Rendben van mert ez a SUGÁR!!
        x = -tmp;
    }
    y = tmp;
    if (a2->x < 0.0) {
        y = -tmp;
    }
    z = tmp;
    if (a2->x < 0.0) {
        z = -tmp;
    }

    if (((x + a3a.center.x) - a1->x) * a2->x +  a2->y * ((y + a3a.center.y) - a1->y) + a2->z * ((z + a3a.center.z) - a1->z) < 0.0)     
        result = 0;
    else
        result = INT_FullRaySphere(a1, a2, &a3a, 0, a5, a6);
    return result;
}

int COL_RayAABBox(Vec3* a1, Vec3* a2, tGameObjectMain* a3, float* a6, float a5)
{
    Mtx4x4* v5;
    tGameObjectBV* v6;
    double v7;
    float v9;
    float v10;
    float v11;
    float v12;
    tColBox colBox;

    v5 = a3->matrices;
    v6 = a3->boundingVolume;
    colBox.pos2.x = v6->BVMax.x + v5->Pos.x;
    colBox.pos2.y = v6->BVMax.y + v5->Pos.y;
    colBox.pos2.z = v6->BVMax.z + v5->Pos.z;
    v7 = v5->Pos.x + v6->BVMin.x;
    colBox.pos1.x = (float)v7;
    colBox.pos1.y = v5->Pos.y + v6->BVMin.y;
    colBox.pos1.z = v5->Pos.z + v6->BVMin.z;
    v11 = a5 * a2->z;
    v9 = a5 * a2->x + a1->x;
    v10 = a5 * a2->y + a1->y;
    v12 = v11 + a1->z;
    if (a2->x <= 0.0)
    {
        if (v7 > a1->x || v9 > (double)colBox.pos2.x)
            return 0;
    }
    else
    {
        if (a1->x > (double)colBox.pos2.x)
            return 0;
        if (v7 > v9)
            return 0;
    }
    if (a2->y <= 0.0)
    {
        if (colBox.pos1.y > (double)a1->y || v10 > (double)colBox.pos2.y)
            return 0;
        goto LABEL_14;
    }
    if (a1->y > (double)colBox.pos2.y)
        return 0;
    if (colBox.pos1.y > (double)v10)
        return 0;
LABEL_14:
    if (a2->z <= 0.0)
    {
        if (colBox.pos1.z > (double)a1->z || v12 > (double)colBox.pos2.z)
            return 0;
    }
    else
    {
        if (a1->z > (double)colBox.pos2.z)
            return 0;
        if (colBox.pos1.z > (double)v12)
            return 0;
    }
    return INT_FullRayAABBox(a1, a2, &colBox, 0, a5, a6);
}

struct sGameObjectMain* COL_Visual_RayCast(struct sWorld* a1, struct sGameObjectMain* a2, Vec3* a3, Vec3* a4, float a5, int flagMustHave, int flagMustNothave, int whatToTest, int* a9, int a10)
{
    Vec3* v11;
    tableElemStructPF* v12;
    tGameObjectMain* result;
    tGameObjectMain* currGao;
    tWorld* v15;
    int v16;
    tGEOGRO* v17;
    tColMap* v18;
    tColCollisionObject* v19;
    uint8_t i;
    tColCollisionObject* v21;
    tCobNormalObjectGeneric* v22;
    int v23;
    int v24;
    tableElemStructPF* v25;
    tableElemStructPF* v26;
    float a9a;

    COL_gst_Ray.direction = a4;
    v11 = a3;
    COL_gst_Ray.distance = a5;
    COL_gst_Ray.worldSetInColMapRayCast = a1;
    COL_gst_Ray.sourcePoint = a3;
    COL_gst_Ray.gao = 0;
    COL_gst_Ray.ignoreColMapRayCastResult = a9;
    COL_gst_Ray.skipRayCast = a10;
    COL_gst_Ray.skipVecsTransformationToGaoSpace = 0;
    if (a9)
        *a9 = 1;
    a1->colCrossObjectNumber = 0;
    v12 = a1->visibleGameObjectsPF.firstElemPtr;
    v25 = v12;
    v26 = ((tableElemStructPF*)(a1->visibleGameObjectsPF.nextElemPtr)) - 1;
    if (v12 <= v26)
    {
        while (1)
        {
            currGao = (tGameObjectMain*)v12->objectPtr;
            if (v12->objectPtr
                && ((uint8_t)currGao & 3) != 1
                && OBJ_b_test_flag(currGao, flagMustHave, flagMustNothave, whatToTest)
                && ((currGao->flags & 0x4000) != 0 || (currGao->flags & 2) != 0)
                && currGao != a2
                && (OBJ_BV_IsSphere(currGao->boundingVolume)
                    && COL_RaySphereBV(v11, a4, currGao, &a9a, COL_gst_Ray.distance)
                    || OBJ_BV_IsAABBox(currGao->boundingVolume)
                    && COL_RayAABBox(v11, a4, currGao, &a9a, COL_gst_Ray.distance)))
            {
                if (COL_gst_Ray.skipRayCast && (currGao->flags2 & 0x1000000) != 0)
                {
                    v15 = COL_gst_Ray.worldSetInColMapRayCast;
                    if (COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber == 16)
                    {
                        COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber = 15;
                        v15 = COL_gst_Ray.worldSetInColMapRayCast;
                    }
                    v15->collCrossObjects[v15->colCrossObjectNumber] = currGao;
                    ++COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber;
                }
                else
                {
                    v16 = currGao->flags;
                    if ((currGao->flags & 0x4000) != 0)
                    {
                        if ((((tGameObjectVisualObject*)(currGao->basic->actorObjectOrVisualObject))->objDrawMask & 8) != 0)
                        {
                            v17 = OBJ_p_GetCurrentGeo(currGao);
                            if (v17)
                            {
                                //nullsub_32();
                                if (v17->collisionPtr)
                                    COL_Visual_OK3_RayCast(currGao, v17);
                                else
                                    COL_Visual_Triangles_RayCast(currGao, v17);
                                //nullsub_32();
                            }
                        }
                    }
                    else if ((v16 & 2) != 0 && (v16 & 0x100) != 0)
                    {
                        v18 = currGao->extended->coll->colMap;
                        v19 = *v18->collisionObjectPtr;
                        if (v19->objectType == 5)
                        {
                            if ((v18->activation & 1) != 0)
                            {
                                if (((tCobMeshObjectMesh*)(v19->objectPtrByType))->ok3)
                                    COL_ColMap_OK3_RayCast(currGao, v18);
                                else
                                    COL_ColMap_Triangles_RayCast(currGao, v18);
                                if (COL_gst_Ray.ignoreColMapRayCastResult && !*COL_gst_Ray.ignoreColMapRayCastResult)
                                    return 0;
                            }
                        }
                        else
                        {
                            for (i = 0; i < v18->colObjectID; ++i)
                            {
                                if (((uint8_t)(1 << i) & v18->activation) != 0)
                                {
                                    v21 = v18->collisionObjectPtr[i];
                                    v22 = (tCobNormalObjectGeneric*)(v21->objectPtrByType);
                                    v23 = v21->objectType - 1;
                                    if (v23)
                                    {
                                        v24 = v23 - 1;
                                        if (v24)
                                        {
                                            if (v24 == 1)
                                                COL_ColMap_Cylinder_RayCast(currGao, v22);
                                        }
                                        else
                                        {
                                            COL_ColMap_Sphere_RayCast(currGao, v22);
                                        }
                                    }
                                    else
                                    {
                                        COL_ColMap_Box_RayCast(currGao, v22);
                                    }
                                }
                            }
                            v12 = v25;
                        }
                    }
                }
            }
            v25 = ++v12;
            if (v12 > v26)
                break;
            v11 = a3;
        }
        result = COL_gst_Ray.gao;
        COL_gb_UseMinDistance = 0;
    }
    else
    {
        result = COL_gst_Ray.gao;
        COL_gb_UseMinDistance = 0;
    }
    return result;
}

void COL_RayCompute(Vec3* a1, struct sWorld* a2, struct sGameObjectMain* a3, Vec3* a4, Vec3* a5, int a6, int a7, int a8, int a9, unsigned int* a10)
{
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    Vec3* v16;
    double v17;
    double v18;
    double v19;
    Vec3* v20; 
    double v21;
    double v22;
    double v23;
    Vec3 v24;
    int v25[51]; // TODO FIXME: 17 nem elég?
    Vec3 v26;
    float v27;
    tGameObjectMain* v28;
    float a5a;
    Vec3 a2a;
    int i;
    float v32;
    Vec3 a1a[17]; // meg ennek is

    a5a = MATH_VecLen(a5);
    v32 = (float)(1.0 / a5a);
    a5->x = v32 * a5->x;
    a5->y = a5->y * v32;
    a5->z = a5->z * v32;
    v26.y = 0.0;
    v26.x = 0.0;
    v26.z = 1.0;
    v24.z = 0.0;
    v24.y = 0.0;
    v24.x = 1.0;
    a1a[0] = *a5;
    MATH_VecRotate(&a1a[1], &v26, a5, 0.78539819f);
    MATH_VecRotate(&a1a[2], &v26, a5, -0.78539819f);
    MATH_VecRotate(&a1a[3], &v26, a5, 1.5707964f);
    MATH_VecRotate(&a1a[4], &v26, a5, -1.5707964f);
    MATH_VecRotate(&a1a[5], &v26, a5, 2.3561945f);
    MATH_VecRotate(&a1a[6], &v26, a5, -2.3561945f);
    MATH_VecRotate(&a1a[7], &v26, a5, 3.1415927f);
    a1a[8].x = a5->x;
    a1a[8].y = a5->y;
    a1a[8].z = 0.0;
    MATH_VecRotate(&a1a[9], &v26, &a1a[8], 0.78539819f);
    MATH_VecRotate(&a1a[10], &v26, &a1a[8], -0.78539819f);
    MATH_VecRotate(&a1a[11], &v26, &a1a[8], 1.5707964f);
    MATH_VecRotate(&a1a[12], &v26, &a1a[8], -1.5707964f);
    MATH_VecRotate(&a1a[13], &v26, &a1a[8], 2.3561945f);
    MATH_VecRotate(&a1a[14], &v26, &a1a[8], -2.3561945f);
    MATH_VecRotate(&a1a[15], &v26, &a1a[8], 3.1415927f);
    a1a[16].x = a5->x;
    a1a[16].y = a5->y;
    a1a[16].z = 0.0;
    MATH_VecRotate(&a1a[16], a1a, &v24, 1.4707963f);
    v23 = 1.0 / 0.1 * a4->x + MATH_gd_magicDoubleForRound;
    v15 = (float)(*((int32_t *)(&v23)));
    a4->x = (float)(v15 * 0.1);
    v22 = 1.0 / 0.1 * a4->y + MATH_gd_magicDoubleForRound;
    v14 = (float)(*((int32_t*)(&v22)));
    a4->y = (float)(v14 * 0.1);
    v21 = 1.0 / 0.1 * a4->z + MATH_gd_magicDoubleForRound;
    v13 = (float)(*((int32_t*)(&v21)));
    a4->z = (float)(v13 * 0.1);
    for (i = 0; i <= 16; ++i)
    {
        MATH_NormalizeVector(&a1a[i], &a1a[i]);
        v20 = &a1a[i];
        *a1 = *v20;
        a1->x = a5a * a1->x;
        a1->y = a1->y * a5a;
        a1->z = a1->z * a5a;
        v19 = 1.0 / 0.1 * a1a[i].x + MATH_gd_magicDoubleForRound;
        v12 = (float)(*((int32_t*)(&v19)));
        a1a[i].x = (float)(v12 * 0.1);
        v18 = 1.0 / 0.1 * a1a[i].y + MATH_gd_magicDoubleForRound;
        v11 = (float)(*((int32_t*)(&v18)));
        a1a[i].y = (float)(v11 * 0.1);
        v17 = 1.0 / 0.1 * a1a[i].z + MATH_gd_magicDoubleForRound;
        v10 = (float)(*((int32_t*)(&v17)));
        a1a[i].z = (float)(v10 * 0.1);
        v28 = COL_Visual_RayCast(a2, a3, a4, &a1a[i], a5a, a6, a7, a8, 0, a9);
        v25[i] = v28 == 0;
        if (!v28)
        {
            a2a = *a4;
            MATH_VecAddVec(&a2a, &a2a, a1);
            MATH_SubstractVec(&a2a, &a2a, &a3->matrices->Pos);
            v27 = MATH_VecLen(&a2a);
            MATH_NormalizeVector(&a2a, &a2a);
            v28 = COL_Visual_RayCast(a2, a3, &a3->matrices->Pos, &a2a, v27, a6, a7, a8, 0, a9);
            if (!v28)
            {
                *a10 |= 0x10u;
                *a10 &= ~0x20u;
                return;
            }
        }
    }
    for (i = 0; i <= 16; ++i)
    {
        if (v25[i])
        {
            v16 = &a1a[i];
            *a1 = *v16;
            a1->x = a5a * a1->x;
            a1->y = a1->y * a5a;
            a1->z = a1->z * a5a;
            *a10 |= 0x10u;
            *a10 |= 0x20u;
            return;
        }
    }
    *a10 &= ~0x10u;
    *a10 &= ~0x20u;
}

void COL_UpdateOldMatrix(tWorld* a1)
{
    tableElemStructPF* v1;
    tableElemStructPF* i;
    tGameObjectMain* v3;
    tColInstance* v4;
    tGaoMatrix* v5;
    tGaoMatrix* v6;
    tGaoMatrix* v7;

    v1 = a1->eot.collInstanceZDMGameObjectsPF.firstElemPtr;
    for (i = ((tableElemStructPF*)(a1->eot.collInstanceZDMGameObjectsPF.nextElemPtr)) - 1; v1 <= i; ++v1)
    {
        v3 = (tGameObjectMain*)v1->objectPtr;
        if (!v1->objectPtr || ((uint8_t)v3 & 3) == 1 || (v3->flags & 0x200) == 0)
            continue;

        v4 = v3->extended->coll->colInstance;
        v5 = (tGaoMatrix*)v4->mtx;
        v4->backface = 0;
        if ((v3->flags & 0x400000) != 0)
        {
            v6 = v3->basic->mtx;
            if (v5 != v6)
                goto LABEL_10;
        }
        else
        {
            v7 = (tGaoMatrix*)v3->matrices;
            if (v5 != v7)
            {
                v6 = v7;
            LABEL_10:
                memcpy(v5, v6, sizeof(Mtx4x4));
                continue;
            }
        }
    }
}

void COL_InitGlobalVars(tGameObjectMain* inst, tGameObjectMain* map)
{
    COL_gst_GlobalVars.colInstanceGao = inst;
    COL_gst_GlobalVars.colInstMatrix = inst->extended->coll->colInstance->mtx;
    COL_gst_GlobalVars.colInstGaoMatrices = inst->matrices;
    COL_gst_GlobalVars.colMapGao = map;
    COL_gst_GlobalVars.colMapGaoMatrices = map->matrices;
}

int COL_get_activation_flag_for_id__max_16_sub_4132C0(tColInstance* a1, char a2)
{
    uint8_t v2;

    v2 = 0;
    while (a1->colSet->zoneIDs[v2] != a2)
    {
        if (++v2 >= 16u)
            return 0;
    }
    return a1->activationFlags & (1 << v2);
}

void COL_A_BCS_ComputeZDxInfo(tColGlobal* a1)
{
    tColInstanceElem* v1;
    tColSphere* v2;
    Vec3* v3;
    tGameObjectMain* v4;
    int v5;
    Vec3* v6;
    Mtx4x4* v7;
    double v8;
    double v9;
    double v10;
    long double v11;
    long double v12;
    tColBox* v13;
    double v14;
    double v15;
    double v16;
    Vec3* v17;
    tGameObjectMain* v18;
    int v19;
    Vec3* v20;
    Mtx4x4* v21;
    double v22;
    double v23;
    double v24;
    long double v25;
    tColSphere* v26;
    float v27;
    float v28;
    Vec3 dest;
    Vec3 scale;
    Vec3 a3;
    Mtx4x4 a1a;

    v1 = a1->collInstanceElem;
    if (v1->elemType == 2)
    {
        v2 = (tColSphere*)v1->elemShapePtr;
        v26 = v2;
        if ((v1->colFlags & 8) != 0)
        {
            MATH_TransformVertexNoScale(&dest, a1->colInstGaoMatrices, &v2->center);
        }
        else
        {
            MATH_TransformVertex(&dest, a1->colInstGaoMatrices, &v2->center);
            if ((a1->colInstGaoMatrices->mtxflag & 8) != 0)
            {
                MATH_GetScale(&scale, a1->colInstGaoMatrices);
                a1->radius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * v2->radius;
            LABEL_7:
                v3 = &a1->centerPos;
                MATH_TransformVertexNoScale(&a1->centerPos, &a1->colMapGaoInvMatrix, &dest);
                MATH_TransformVertexNoScale(&a1->centerPos, &a1->colMapGaoInvMatrix, &dest);
                v4 = a1->colInstanceGao;
                v5 = v4->flags;
                if ((a1->collInstanceElem->colFlags & 8) != 0)
                {
                    if ((v5 & 0x400000) != 0)
                    {
                        MATH_MulMatrixMatrix(&a1a, v4->extended->coll->colInstance->mtx, v4->basic->mtx->refGao2->matrices);
                        MATH_TransformVertexNoScale(&dest, &a1a, &v2->center);
                    }
                    else
                    {
                        MATH_TransformVertexNoScale(&dest, a1->colInstMatrix, &v2->center);
                    }
                }
                else if ((v5 & 0x400000) != 0)
                {
                    MATH_MulMatrixMatrix(&a1a, v4->extended->coll->colInstance->mtx, v4->basic->mtx->refGao2->matrices);
                    MATH_TransformVertex(&dest, &a1a, &v2->center);
                }
                else
                {
                    MATH_TransformVertex(&dest, a1->colInstMatrix, &v2->center);
                }
                v6 = &a1->centerPosPrevious;
                MATH_TransformVertexNoScale(&a1->centerPosPrevious, &a1->colMapGaoInvMatrix, &dest);
                a1->moveVec.x = v3->x - a1->centerPosPrevious.x;
                a1->moveVec.y = a1->centerPos.y - a1->centerPosPrevious.y;
                a1->moveVec.z = a1->centerPos.z - a1->centerPosPrevious.z;
                if (fabs(a1->moveVec.x) != 0 || fabs(a1->moveVec.y) != 0 || fabs(a1->moveVec.z) != 0)
                {
                    MATH_NormalizeVector(&a1->moveVecUnit, &a1->moveVec);
                }
                else
                {
                    a1->moveVecUnit.x = 0.0;
                    a1->moveVecUnit.y = 0.0;
                    a1->moveVecUnit.z = 0.0;
                }
                a1->dynCenterPos.x = v3->x - v6->x;
                a1->dynCenterPos.y = a1->centerPos.y - a1->centerPosPrevious.y;
                a1->dynCenterPos.z = a1->centerPos.z - a1->centerPosPrevious.z;
                a1->dynCenterPos.x = (float)(a1->dynCenterPos.x * 0.5);
                a1->dynCenterPos.y = (float)(a1->dynCenterPos.y * 0.5);
                a1->dynCenterPos.z = (float)(a1->dynCenterPos.z * 0.5);
                a1->dynCenterPos.x = a1->dynCenterPos.x + v6->x;
                a1->dynCenterPos.y = a1->centerPosPrevious.y + a1->dynCenterPos.y;
                a1->dynCenterPos.z = a1->centerPosPrevious.z + a1->dynCenterPos.z;
                v7 = a1->colInstGaoMatrices;
                v8 = a1->dynCenterPos.y - a1->centerPosPrevious.y;
                v9 = a1->dynCenterPos.z - a1->centerPosPrevious.z;
                v10 = a1->dynCenterPos.x - v6->x;
                v11 = sqrt(v10 * v10 + v9 * v9 + v8 * v8);
                a1->dynRadius = (float)v11;
                v12 = v11 + v26->radius;
                a1->dynRadius = (float)v12;
                if ((v7->mtxflag & 8) == 0 || (a1->collInstanceElem->colFlags & 8) != 0)
                {
                    a1->dynRadius = (float)(v12 + v26->radius);
                }
                else
                {
                    MATH_GetScale(&scale, v7);
                    v27 = v26->radius;
                    a1->dynRadius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * v27 + a1->dynRadius;
                }
                return;
            }
        }
        a1->radius = v2->radius;
        goto LABEL_7;
    }
    v13 = (tColBox*)v1->elemShapePtr;
    a3.x = v13->pos1.x + v13->pos2.x;
    a3.y = v13->pos2.y + v13->pos1.y;
    v14 = v13->pos2.z + v13->pos1.z;
    a3.x = (float)(a3.x * 0.5);
    a3.y = (float)(a3.y * 0.5);
    a3.z = (float)(v14 * 0.5);
    v15 = v13->pos1.x - v13->pos2.x;
    v16 = v13->pos1.y - v13->pos2.y;
    scale.z = v13->pos1.z - v13->pos2.z;
    scale.x = (float)(v15 * 0.5);
    v28 = (float)sqrt(scale.x * scale.x + scale.z * 0.5 * (scale.z * 0.5) + v16 * 0.5 * (v16 * 0.5));
    if ((v1->colFlags & 8) != 0)
    {
        MATH_TransformVertexNoScale(&dest, a1->colInstGaoMatrices, &a3);
        a1->radius = v28;
    }
    else
    {
        MATH_TransformVertex(&dest, a1->colInstGaoMatrices, &a3);
        if ((a1->colInstGaoMatrices->mtxflag & 8) != 0)
        {
            MATH_GetScale(&scale, a1->colInstGaoMatrices);
            a1->radius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * v28;
        }
        else
        {
            a1->radius = v28;
        }
    }
    v17 = &a1->centerPos;
    MATH_TransformVertexNoScale(&a1->centerPos, &a1->colMapGaoInvMatrix, &dest);
    MATH_TransformVertexNoScale(&a1->centerPos, &a1->colMapGaoInvMatrix, &dest);
    v18 = a1->colInstanceGao;
    v19 = v18->flags;
    if ((a1->collInstanceElem->colFlags & 8) != 0)
    {
        if ((v19 & 0x400000) != 0)
        {
            MATH_MulMatrixMatrix(&a1a, v18->extended->coll->colInstance->mtx, v18->basic->mtx->refGao2->matrices);
            MATH_TransformVertexNoScale(&dest, &a1a, &a3);
        }
        else
        {
            MATH_TransformVertexNoScale(&dest, a1->colInstMatrix, &a3);
        }
    }
    else if ((v19 & 0x400000) != 0)
    {
        MATH_MulMatrixMatrix(&a1a, v18->extended->coll->colInstance->mtx, v18->basic->mtx->refGao2->matrices);
        MATH_TransformVertex(&dest, &a1a, &a3);
    }
    else
    {
        MATH_TransformVertex(&dest, a1->colInstMatrix, &a3);
    }
    v20 = &a1->centerPosPrevious;
    MATH_TransformVertexNoScale(&a1->centerPosPrevious, &a1->colMapGaoInvMatrix, &dest);
    a1->moveVec.x = v17->x - a1->centerPosPrevious.x;
    a1->moveVec.y = a1->centerPos.y - a1->centerPosPrevious.y;
    a1->moveVec.z = a1->centerPos.z - a1->centerPosPrevious.z;
    if (fabs(a1->moveVec.x) != 0 || fabs(a1->moveVec.y) != 0 || fabs(a1->moveVec.z) != 0)
    {
        MATH_NormalizeVector(&a1->moveVecUnit, &a1->moveVec);
    }
    else
    {
        a1->moveVecUnit.x = 0.0;
        a1->moveVecUnit.y = 0.0;
        a1->moveVecUnit.z = 0.0;
    }
    a1->dynCenterPos.x = v17->x - v20->x;
    a1->dynCenterPos.y = a1->centerPos.y - a1->centerPosPrevious.y;
    a1->dynCenterPos.z = a1->centerPos.z - a1->centerPosPrevious.z;
    a1->dynCenterPos.x = (float)(a1->dynCenterPos.x * 0.5);
    a1->dynCenterPos.y = (float)(a1->dynCenterPos.y * 0.5);
    a1->dynCenterPos.z = (float)(a1->dynCenterPos.z * 0.5);
    a1->dynCenterPos.x = v20->x + a1->dynCenterPos.x;
    a1->dynCenterPos.y = a1->centerPosPrevious.y + a1->dynCenterPos.y;
    a1->dynCenterPos.z = a1->centerPosPrevious.z + a1->dynCenterPos.z;
    v21 = a1->colInstGaoMatrices;
    v22 = a1->dynCenterPos.y - a1->centerPosPrevious.y;
    v23 = a1->dynCenterPos.z - a1->centerPosPrevious.z;
    v24 = a1->dynCenterPos.x - v20->x;
    v25 = sqrt(v23 * v23 + v22 * v22 + v24 * v24) + v28;
    a1->dynRadius = (float)v25;
    if ((v21->mtxflag & 8) == 0 || (a1->collInstanceElem->colFlags & 8) != 0)
    {
        a1->dynRadius = (float)(v25 + v28);
    }
    else
    {
        MATH_GetScale(&scale, v21);
        a1->dynRadius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * v28 + a1->dynRadius;
    }
}

void COL_SetFaceTag(tCobOK3* a1, unsigned int a2)
{
    int* v2;

    if (a1)
    {
        v2 = a1->faceTagInts;
        if (v2)
            v2[a2 >> 5] |= 1 << (a2 - 32 * (a2 >> 5));
    }
}

BOOL COL_b_TestFaceTag(tCobOK3* a1, unsigned int a2)
{
    int* v2;
    int result;

    if (a1 && (v2 = a1->faceTagInts) != 0)
        result = v2[a2 >> 5] & (1 << (a2 - 32 * (a2 >> 5)));
    else
        result = 0;
    return result;
}

Vec3* INT_PointLineClosestPoint(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5)
{
    double v6;
    double v7;
    double v8;
    double v9;
    long double v10;
    Vec3* result;
    double v12;
    double v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;

    v14 = a3->x - a2->x;
    v15 = a3->y - a2->y;
    v6 = a3->z - a2->z;
    v16 = (float)v6;
    v7 = v6 * v16;
    v8 = v15 * v15;
    v9 = v14 * v14;
    v10 = 1.0 / sqrt(v8 + v7 + v9);
    v17 = (float)(v14 * v10);
    v18 = (float)(v15 * v10);
    v19 = (float)(-((a4->z * (v10 * v16) + a4->y * v18 + v17 * a4->x) / sqrt(v7 + v8 + v9)));
    if (v19 >= 0.0)
    {
        result = a5;
        if (v19 <= 1.0)
        {
            v12 = v14 * v19;
            a5->x = (float)v12;
            v13 = v15 * v19;
            a5->y = (float)v13;
            v20 = v16 * v19;
            a5->z = v20;
            a5->x = (float)(v12 + a2->x);
            a5->y = (float)(v13 + a2->y);
            a5->z = (float)(v20 + a2->z);
        }
        else
        {
            *a5 = *a3;
        }
    }
    else
    {
        result = a5;
        *a5 = *a2;
    }
    return result;
}

void INT_PointTriangleClosestPointGet(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, Vec3* a6, uint32_t* a7)
{
    double v7;
    double v8;
    double v9;
    double v10;
    float v11;
    float v12;
    float v13;
    Vec3 v14;
    Vec3 a4a;

    if (INT_PointInTriangle(a5, a1, a2, a3, a4, 0))
    {
        *a6 = *a5;
        *a7 = 1;
    }
    else
    {
        v14.x = a1->x - a5->x;
        v14.y = a1->y - a5->y;
        v14.z = a1->z - a5->z;
        a4a.x = a2->x - a5->x;
        a4a.y = a2->y - a5->y;
        a4a.z = a2->z - a5->z;
        v7 = a3->x - a5->x;
        v8 = a3->y - a5->y;
        v9 = a3->z - a5->z;
        v11 = v14.z * v14.z + v14.x * v14.x + v14.y * v14.y;
        v12 = a4a.z * a4a.z + a4a.y * a4a.y + a4a.x * a4a.x;
        v13 = (float)(v9 * v9 + v8 * v8 + v7 * v7);
        v10 = MATH_f_MaxOf3(v11, v12, v13);
        if (v10 == v11)
        {
            INT_PointLineClosestPoint(a5, a2, a3, &a4a, a6);
            *a7 = 8;
        }
        else if (v10 == v12)
        {
            INT_PointLineClosestPoint(a5, a1, a3, &v14, a6);
            *a7 = 4;
        }
        else if (v10 == v13)
        {
            INT_PointLineClosestPoint(a5, a1, a2, &v14, a6);
            *a7 = 2;
        }
    }
}

BOOL INT_DynamicHit(Vec3* a1, float a2, Vec3* a3, float a4, Vec3* a5, Vec3* a6, Vec3* a7, Vec3* a8, Vec3* a9, Vec3* a10, int* a11)
{
    double v11;
    double v12;
    Vec3* v13; 
    double v14;
    double v15;
    double v16;
    long double v17;
    long double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    double v24;
    double v25;
    double v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    double v33;
    double v34;
    double v35;
    double v36;
    double v37;
    double v38;
    double v39;
    double v40;
    int v42;
    Vec3 a5a;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    float v49;

    v11 = -a2;
    v12 = v11 * a5->x;
    v13 = a6;
    a5a.y = (float)(v11 * a5->y);
    v42 = 1;
    a5a.z = (float)(v11 * a5->z);
    v47 = a2 * a5->x;
    v48 = a2 * a5->y;
    v49 = a2 * a5->z;
    v44 = (float)(v12 + a1->x);
    v14 = a5a.y + a1->y;
    v46 = a5a.z + a1->z;
    v15 = a1->x - v44;
    v16 = a1->y - v14;
    a5a.z = a1->z - v46;
    a5a.x = (float)(v15 + v47);
    a5a.y = (float)(v16 + v48);
    a5a.z = (float)(a5a.z + v49);
    v17 = a5a.y * a9->y + a5a.x * a9->x + a5a.z * a9->z;
    if (v17 > 0.0 || fabs(v17) < 0.0000099999997)
        v42 = 0;
    v18 = ((a6->y - v14) * a9->y + (a6->x - v44) * a9->x + (a6->z - v46) * a9->z) / v17;
    if (v18 > 0.0)
    {
        a5a.z = (float)(a5a.z * v18);
        a5a.x = (float)(a5a.x * v18 + v44);
        a5a.y = (float)(a5a.y * v18 + v14);
        a5a.z = (float)(a5a.z + v46);
        INT_PointTriangleClosestPointGet(a6, a7, a8, a9, &a5a, a10, a11);
        v19 = a10->y - a1->y;
        v20 = a10->z - a1->z;
        v21 = a10->x - a1->x;
        if (sqrt(v20 * v20 + v19 * v19 + v21 * v21) <= a2)
        {
            v22 = (a3->y - a10->y) * a5->y + (a3->z - a10->z) * a5->z + (a3->x - a10->x) * a5->x;
            v23 = v22 * a5->x;
            v45 = (float)(v22 * a5->y);
            v46 = (float)(v22 * a5->z);
            v24 = v23 + a10->x;
            v25 = v45 + a10->y;
            a5a.z = v46 + a10->z;
            v26 = v25;
            v47 = (float)(v24 - a3->x);
            v27 = a5a.z - a3->z;
            v28 = v26 - a3->y;
            if (sqrt(v27 * v27 + v28 * v28 + v47 * v47) <= a4)
            {
                if (v42)
                    return 1;
            }
        }
        v13 = a6;
    }
    v29 = (v13->x - a1->x) * a9->x + (v13->z - a1->z) * a9->z + (v13->y - a1->y) * a9->y;
    v30 = v29 * a9->x;
    v48 = (float)(v29 * a9->y);
    v49 = (float)(v29 * a9->z);
    a5a.x = (float)(v30 + a1->x);
    a5a.y = v48 + a1->y;
    a5a.z = v49 + a1->z;
    INT_PointTriangleClosestPointGet(v13, a7, a8, a9, &a5a, a10, a11);
    v31 = a10->y - a1->y;
    v32 = a10->z - a1->z;
    v33 = a10->x - a1->x;
    if (sqrt(v32 * v32 + v31 * v31 + v33 * v33) > a2)
        return 0;
    v34 = (a3->y - a10->y) * a5->y + (a3->z - a10->z) * a5->z + (a3->x - a10->x) * a5->x;
    v35 = v34 * a5->x;
    v45 = (float)(v34 * a5->y);
    v46 = (float)(v34 * a5->z);
    v36 = v35 + a10->x;
    v37 = v45 + a10->y;
    a5a.z = v46 + a10->z;
    v38 = v37;
    v47 = (float)(v36 - a3->x);
    v39 = a5a.z - a3->z;
    v40 = v38 - a3->y;
    return sqrt(v39 * v39 + v40 * v40 + v47 * v47) <= a4;
}

int INT_ul_GetPositionPointWithBox(Vec3* a1, Vec3* a2, Vec3* a3)
{
    int v3; // edx

    v3 = 0;
    if (a1->x >= (double)a2->x)
    {
        if (a1->x > (double)a3->x)
            v3 = 2;
    }
    else
    {
        v3 = 1;
    }
    if (a1->y >= (double)a2->y)
    {
        if (a1->y > (double)a3->y)
            v3 |= 8u;
    }
    else
    {
        v3 |= 4u;
    }
    if (a1->z < (double)a2->z)
        return v3 | 0x10;
    if (a1->z > (double)a3->z)
        v3 |= 0x20u;
    return v3;
}

int INT_ul_GetPositionPointWithEdgeBox(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5)
{
    double v5;
    int v6; 
    double v7;
    double v8;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;

    v5 = a1->x * a5->x;
    v6 = 0;
    v17 = a5->y * a1->y;
    v18 = a5->z * a1->z;
    v7 = a3->x * a4->x;
    v15 = a3->y * a4->y;
    v16 = a3->z * a4->z;
    v8 = a1->x * a4->x;
    v13 = a1->y * a4->y;
    v14 = a1->z * a4->z;
    v10 = a2->x * a5->x;
    v11 = a2->y * a5->y;
    v12 = a2->z * a5->z;
    if (v15 + v7 < v13 + v8)
        v6 = 1;
    if (v11 + v7 < v17 + v8)
        v6 |= 2u;
    if (v10 + v15 < v13 + v5)
        v6 |= 4u;
    if (v11 + v10 < v17 + v5)
        v6 |= 8u;
    if (v16 + v7 < v14 + v8)
        v6 |= 0x10u;
    if (v12 + v7 < v18 + v8)
        v6 |= 0x20u;
    if (v10 + v16 < v14 + v5)
        v6 |= 0x40u;
    if (v12 + v10 < v18 + v5)
        v6 |= 0x80u;
    if (v16 + v15 < v14 + v13)
        v6 |= 0x100u;
    if (v12 + v15 < v13 + v18)
        v6 |= 0x200u;
    if (v11 + v16 < v14 + v17)
        v6 |= 0x400u;
    if (v12 + v11 < v18 + v17)
        v6 |= 0x800u;
    return v6;
}

BOOL INT_b_InYZRange(Vec3* a1, Vec3* a2, Vec3* a3)
{
    return a1->y >= (double)a2->y && a1->y <= (double)a3->y && a1->z >= (double)a2->z && a1->z <= (double)a3->z;
}

BOOL INT_b_InXZRange(Vec3* a1, Vec3* a2, Vec3* a3)
{
    return a1->x >= (double)a2->x && a1->x <= (double)a3->x && a1->z >= (double)a2->z && a1->z <= (double)a3->z;
}

BOOL INT_b_InXYRange(Vec3* a1, Vec3* a2, Vec3* a3)
{
    return a1->x >= (double)a2->x && a1->x <= (double)a3->x && a1->y >= (double)a2->y && a1->y <= (double)a3->y;
}

int INT_b_IntersectEdgeWithBoxWithHit(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, char a6, Vec3* a7)
{
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    int result;

    if ((a6 & 1) != 0)
    {
        v7 = (a4->x - a1->x) / a3->x;
        a7->x = a4->x;
        a7->y = (float)(v7 * a3->y + a1->y);
        a7->z = (float)(v7 * a3->z + a1->z);
        if (INT_b_InYZRange(a7, a4, a5))
            goto LABEL_13;
    }
    if ((a6 & 2) != 0)
    {
        v8 = (a5->x - a1->x) / a3->x;
        a7->x = a5->x;
        a7->y = (float)(v8 * a3->y + a1->y);
        a7->z = (float)(v8 * a3->z + a1->z);
        if (INT_b_InYZRange(a7, a4, a5))
            goto LABEL_13;
    }
    if ((a6 & 4) != 0)
    {
        v9 = (a4->y - a1->y) / a3->y;
        a7->x = (float)(v9 * a3->x + a1->x);
        a7->y = a4->y;
        a7->z = (float)(v9 * a3->z + a1->z);
        if (INT_b_InXZRange(a7, a4, a5))
            goto LABEL_13;
    }
    if ((a6 & 8) != 0)
    {
        v10 = (a5->y - a1->y) / a3->y;
        a7->x = (float)(v10 * a3->x + a1->x);
        a7->y = a5->y;
        a7->z = (float)(v10 * a3->z + a1->z);
        if (INT_b_InXZRange(a7, a4, a5))
            goto LABEL_13;
    }
    if ((a6 & 0x10) != 0)
    {
        v11 = (a4->z - a1->z) / a3->z;
        a7->x = (float)(v11 * a3->x + a1->x);
        a7->y = (float)(v11 * a3->y + a1->y);
        a7->z = a4->z;
        if (INT_b_InXYRange(a7, a4, a5))
            goto LABEL_13;
    }
    if ((a6 & 0x20) != 0
        && (v12 = (a5->z - a1->z) / a3->z,
            a7->x = (float)(v12 * a3->x + a1->x),
            a7->y = (float)(v12 * a3->y + a1->y),
            a7->z = a5->z,
            INT_b_InXYRange(a7, a4, a5)))
    {
    LABEL_13:
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

static void COL_l_SwapRecords(tRecordElem* a, tRecordElem* b)
{
    tRecordElem tmp;

    memcpy(&tmp, a, sizeof(tmp));
    memcpy(a, b, sizeof(tRecordElem));
    memcpy(b, &tmp, sizeof(tRecordElem));
}

static void COL_l_ExpandBVByColInstanceOffset(tGameObjectMain* gao)
{
    Mtx4x4* colInstMtx;
    Mtx4x4* gaoMtx;
    tGameObjectBV* bv;
    float dx;
    float dy;
    float dz;
    Mtx4x4 hieMtx;

    colInstMtx = gao->extended->coll->colInstance->mtx;
    if ((gao->flags & 0x400000) != 0)
    {
        MATH_MulMatrixMatrix(&hieMtx, colInstMtx, gao->basic->mtx->refGao2->matrices);
        gaoMtx = gao->matrices;
        dx = gaoMtx->Pos.x - hieMtx.Pos.x;
        dy = gaoMtx->Pos.y - hieMtx.Pos.y;
        dz = gaoMtx->Pos.z - hieMtx.Pos.z;
    }
    else
    {
        gaoMtx = gao->matrices;
        dx = gaoMtx->Pos.x - colInstMtx->Pos.x;
        dy = gaoMtx->Pos.y - colInstMtx->Pos.y;
        dz = gaoMtx->Pos.z - colInstMtx->Pos.z;
    }
    bv = gao->boundingVolume;
    bv->BVMin = bv->BVMinOriginal;
    bv->BVMax.x = bv->BVMaxOriginal.x;
    bv->BVMax.y = bv->BVMaxOriginal.y;
    bv->BVMax.z = bv->BVMaxOriginal.z;
    if (dx <= 0.0f)
        bv->BVMax.x = bv->BVMax.x - dx;
    else
        bv->BVMin.x = bv->BVMin.x - dx;
    if (dy <= 0.0f)
        bv->BVMax.y = bv->BVMax.y - dy;
    else
        bv->BVMin.y = bv->BVMin.y - dy;
    if (dz <= 0.0f)
        bv->BVMax.z = bv->BVMax.z - dz;
    else
        bv->BVMin.z = bv->BVMin.z - dz;
}

static void COL_l_AddHitPoint(Vec3* hits, uint32_t* hitCount, Vec3* point)
{
    hits[(*hitCount)++] = *point;
}

int INT_FullAABBoxTriangle(Vec3* p0, Vec3* p1, Vec3* p2, Vec3* normal, Vec3* boxMin, Vec3* boxMax, Vec3* hits, uint32_t* hitCount, uint32_t* flags)
{
    int hit;
    double denom;
    int out0;
    int out1;
    int out2;
    int side0;
    int side1;
    int side2;
    float t;
    float planeD;
    Vec3 point;
    Vec3 diag;
    Vec3 diagFlip;

    *hitCount = 0;
    hit = 0;
    out0 = INT_ul_GetPositionPointWithBox(p0, boxMin, boxMax);
    out1 = INT_ul_GetPositionPointWithBox(p1, boxMin, boxMax);
    out2 = INT_ul_GetPositionPointWithBox(p2, boxMin, boxMax);
    if ((out1 & out2 & out0) != 0)
        return 0;
    diag.x = boxMax->x - boxMin->x;
    diag.y = (float)(boxMax->y - boxMin->y);
    diag.z = (float)(boxMax->z - boxMin->z);
    diagFlip.x = -diag.x;
    diagFlip.y = -diag.y;
    diagFlip.z = -diag.z;
    side0 = (INT_ul_GetPositionPointWithEdgeBox(p0, boxMin, boxMax, &diag, &diagFlip) << 8) | out0;
    side1 = (INT_ul_GetPositionPointWithEdgeBox(p1, boxMin, boxMax, &diag, &diagFlip) << 8) | out1;
    side2 = (INT_ul_GetPositionPointWithEdgeBox(p2, boxMin, boxMax, &diag, &diagFlip) << 8) | out2;
    if ((side1 & side2 & side0) != 0)
        return 0;
    if (flags)
        *flags |= 1u;
    planeD = -(p0->y * normal->y + normal->x * p0->x + p0->z * normal->z);
    denom = diag.x * normal->x + diag.y * normal->y + diag.z * normal->z;
    if (denom != 0.0f)
    {
        t = (float)(-((normal->z * boxMin->z + boxMin->y * normal->y + normal->x * boxMin->x + planeD) / denom));
        if (t > 0.0 && t < 1.0)
        {
            point.x = t * diag.x + boxMin->x;
            point.y = t * diag.y + boxMin->y;
            point.z = t * diag.z + boxMin->z;
            if (INT_PointInTriangle(&point, p0, p1, p2, normal, 0))
            {
                COL_l_AddHitPoint(hits, hitCount, &point);
                hit = 1;
            }
        }
    }
    denom = diag.x * normal->x + diagFlip.y * normal->y + diag.z * normal->z;
    if (denom != 0.0f)
    {
        t = (float)(-((normal->z * boxMin->z + boxMax->y * normal->y + normal->x * boxMin->x + planeD) / denom));
        if (t > 0.0 && t < 1.0)
        {
            point.x = t * diag.x + boxMin->x;
            point.y = diagFlip.y * t + boxMax->y;
            point.z = t * diag.z + boxMin->z;
            if (INT_PointInTriangle(&point, p0, p1, p2, normal, 0))
            {
                COL_l_AddHitPoint(hits, hitCount, &point);
                hit = 1;
            }
        }
    }
    denom = diag.x * normal->x + diag.y * normal->y + diagFlip.z * normal->z;
    if (denom != 0.0f)
    {
        t = (float)(-((boxMin->y * normal->y + normal->z * boxMax->z + normal->x * boxMin->x + planeD) / denom));
        if (t > 0.0 && t < 1.0)
        {
            point.x = t * diag.x + boxMin->x;
            point.y = t * diag.y + boxMin->y;
            point.z = diagFlip.z * t + boxMax->z;
            if (INT_PointInTriangle(&point, p0, p1, p2, normal, 0))
            {
                COL_l_AddHitPoint(hits, hitCount, &point);
                hit = 1;
            }
        }
    }
    denom = diag.x * normal->x + diagFlip.y * normal->y + diagFlip.z * normal->z;
    if (denom != 0.0f)
    {
        t = (float)(-((normal->z * boxMax->z + boxMax->y * normal->y + normal->x * boxMin->x + planeD) / denom));
        if (t > 0.0f && t < 1.0f)
        {
            point.x = t * diag.x + boxMin->x;
            point.y = diagFlip.y * t + boxMax->y;
            point.z = diagFlip.z * t + boxMax->z;
            if (INT_PointInTriangle(&point, p0, p1, p2, normal, 0))
            {
                COL_l_AddHitPoint(hits, hitCount, &point);
                return 1;
            }
        }
    }
    if (hit)
        return hit;
    if ((side1 & side0) == 0)
    {
        diagFlip.x = p1->x - p0->x;
        diagFlip.y = p1->y - p0->y;
        diagFlip.z = p1->z - p0->z;
        if (INT_b_IntersectEdgeWithBoxWithHit(p0, p1, &diagFlip, boxMin, boxMax, side0 | side1, &point))
        {
            COL_l_AddHitPoint(hits, hitCount, &point);
            hit = 1;
        }
    }
    if ((side2 & side0) == 0)
    {
        diagFlip.x = p2->x - p0->x;
        diagFlip.y = p2->y - p0->y;
        diagFlip.z = p2->z - p0->z;
        if (INT_b_IntersectEdgeWithBoxWithHit(p0, p2, &diagFlip, boxMin, boxMax, side0 | side2, &point))
        {
            COL_l_AddHitPoint(hits, hitCount, &point);
            hit = 1;
        }
    }
    if ((side1 & side2) != 0)
        return hit;
    diagFlip.x = p2->x - p1->x;
    diagFlip.y = p2->y - p1->y;
    diagFlip.z = p2->z - p1->z;
    if (!INT_b_IntersectEdgeWithBoxWithHit(p1, p2, &diagFlip, boxMin, boxMax, side1 | side2, &point))
        return hit;
    COL_l_AddHitPoint(hits, hitCount, &point);
    return 1;
}

int COL_Box_Triangle(tColGlobal* colGlobal, tCobMeshTriangleElem* tri, int16_t i0, int16_t i1, int16_t i2, Vec3* normal, int reportA, int16_t reportB)
{
    tColCollisionObject* colObj;
    Vec3* verts;
    Vec3* cursor;
    long double len;
    unsigned int hitCount;
    unsigned int n;
    int flags;
    float bestLen;
    Vec3 bestRecal;
    Vec3 worldP0;
    Vec3 worldP1;
    Vec3 worldP2;
    Vec3 worldNormal;
    Vec3 localP0;
    Vec3 localP1;
    Vec3 localP2;
    Vec3 localNormal;
    Vec3 hitWorld;
    Vec3 hitLocal;
    Vec3 recalVec;
    Vec3 reportNormal;
    Mtx4x4 instInvMtx;
    Vec3 hits[6];

    colObj = colGlobal->collisionObjectPtr;
    flags = 0x4000;
    verts = ((tCobMeshObjectMesh*)(colObj->objectPtrByType))->vertices;
    bestLen = -100000.0f;
    MATH_TransformVertexNoScale(&worldP0, colGlobal->colMapGaoMatrices, &verts[(uint16_t)i0]);
    MATH_TransformVertexNoScale(&worldP1, colGlobal->colMapGaoMatrices, &verts[(uint16_t)i1]);
    MATH_TransformVertexNoScale(&worldP2, colGlobal->colMapGaoMatrices, &verts[(uint16_t)i2]);
    MATH_TransformVectorNoScale(&worldNormal, colGlobal->colMapGaoMatrices, normal);
    MATH_InvertMatrix(&instInvMtx, colGlobal->colInstGaoMatrices);
    MATH_TransformVertexNoScale(&localP0, &instInvMtx, &worldP0);
    MATH_TransformVertexNoScale(&localP1, &instInvMtx, &worldP1);
    MATH_TransformVertexNoScale(&localP2, &instInvMtx, &worldP2);
    MATH_TransformVectorNoScale(&localNormal, &instInvMtx, &worldNormal);
    memset(&hits, 0, 32u); // FIXME: miert csak ennyit?
    if (!INT_FullAABBoxTriangle(
        &localP0,
        &localP1,
        &localP2,
        &localNormal,
        &(((tColBox*)(colGlobal->collInstanceElem->elemShapePtr))->pos2),
        &(((tColBox*)(colGlobal->collInstanceElem->elemShapePtr))->pos1),
        hits,
        &hitCount,
        &flags))
        return 0;
    n = 0;
    if (hitCount)
    {
        cursor = hits;
        do
        {
            MATH_TransformVertex(&hitWorld, colGlobal->colInstGaoMatrices, cursor);
            MATH_TransformVertex(&hitLocal, &colGlobal->colMapGaoInvMatrix, &hitWorld);
            flags |= 0x20u;
            if (COL_ComputeReport(&hitLocal, normal, &hitWorld, &reportNormal, &recalVec, &flags, reportA, reportB, colGlobal))
            {
                len = sqrt(recalVec.z * recalVec.z + recalVec.x * recalVec.x + recalVec.y * recalVec.y);
                if (len > bestLen)
                {
                    bestLen = (float)len;
                    bestRecal = recalVec;
                }
                recalVec.x = 0.0;
                recalVec.y = 0.0;
                recalVec.z = 0.0;
            }
            ++cursor;
            ++n;
        } while (n < hitCount);
        if (bestLen != -100000.0f)
            COL_ReportCollision(&hitWorld, &reportNormal, &bestRecal, flags & ~0x1000u, reportA, reportB, colGlobal);
    }
    return 1;
}

int COL_Sphere_Triangle(tColGlobal* colGlobal, tCobMeshTriangleElem* tri, int16_t i0, int16_t i1, int16_t i2, Vec3* normalVec, int reportA, int16_t reportB, int testDynamic)
{
    tColCollisionObject* colObj;
    Vec3* verts;
    Vec3* p1;
    Vec3* p0;
    Vec3* p2;
    Vec3* contact;
    int reported;
    int flags;
    Vec3 staticHit;
    Vec3 recalVec;
    Vec3 reportNormal;
    Vec3 collidedPoint;
    Vec3 dynHit;

    colObj = colGlobal->collisionObjectPtr;
    flags = 0;
    verts = ((tCobMeshObjectMesh*)(colObj->objectPtrByType))->vertices;
    p1 = &verts[(uint16_t)i1];
    p0 = &verts[(uint16_t)i0];
    p2 = &verts[(uint16_t)i2];
    if (testDynamic
        && INT_DynamicHit(
            &colGlobal->dynCenterPos,
            colGlobal->dynRadius,
            &colGlobal->centerPos,
            colGlobal->radius,
            &colGlobal->moveVecUnit,
            p0,
            p1,
            p2,
            normalVec,
            &dynHit,
            &flags))
    {
        if (INT_FullSphereTriangle(&colGlobal->centerPos, colGlobal->radius, p0, p1, p2, normalVec, &staticHit, &flags))
        {
            flags |= 0x20u;
            contact = &staticHit;
        }
        else
        {
            flags |= 0x10u;
            contact = &dynHit;
        }
        reported = COL_ComputeReport(contact, normalVec, &collidedPoint, &reportNormal, &recalVec, &flags, reportA, reportB, colGlobal);
    }
    else
    {
        // TODO CHECK: triangle collision happens here (mammago edge collision bug)
        if (!INT_FullSphereTriangle(&colGlobal->centerPos, colGlobal->radius, p0, p1, p2, normalVec, &staticHit, &flags))
            return 0;
        flags |= 0x20u;
        reported = COL_ComputeReport(&staticHit, normalVec, &collidedPoint, &reportNormal, &recalVec, &flags, reportA, reportB, colGlobal);
    }
    //a6a &= ~0x40000;
    //a6a &= ~0x80;
    //a6a &= ~0x8; ?? ez javit rajta
    if (reported) {
        //DBG_OutputDebugString("a1: ");
        //DBG_DumpVec(&a1);
        COL_ReportCollision(&collidedPoint, &reportNormal, &recalVec, flags, reportA, reportB, colGlobal);
    }
    //if (HACCCCCK) DBG_OutputDebugString("a6a: %x\n", a6a);
    return 1;
}

void COL_TestAndCollideTriangleIfNeeded(tColGlobal* colglobal, tCobMeshTriangleElem* tri, Vec3* normal, int16_t chunkId, uint16_t triIndex, Vec3* rejectMin, Vec3* rejectMax, int testDynamic)
{
    long double absY;
    long double absZ;
    tColInstance* colInstance;
    int instFlags;
    Mtx4x4* mapMtx;
    double xPart;
    int wantsAccelTest;
    uint16_t i2;
    Vec3* verts;
    Vec3* p0;
    Vec3* p1;
    Vec3* p2;
    float p2x;
    float ny;
    int i0;
    float nx;
    int i1;
    float nz;
    int16_t i2s;
    Vec3 step;

    if (fabs(normal->x) >= 0.001)
        nx = normal->x;
    else
        nx = 0.0;
    absY = fabs(normal->y);
    normal->x = nx;
    if (absY >= 0.001)
        ny = normal->y;
    else
        ny = 0.0;
    absZ = fabs(normal->z);
    normal->y = ny;
    if (absZ >= 0.001)
        nz = normal->z;
    else
        nz = 0.0;
    normal->z = nz;
    if (MATH_b_NulVector(normal))     // MATH_b_NulVector
        return;
    colInstance = colglobal->colInstance;
    if (colInstance->backface)
    {
        MATH_TransformVectorNoScale(&step, &colglobal->colMapGaoInvMatrix, &colInstance->backFaceVec);
        if (step.y * normal->y + step.x * normal->x + step.z * normal->z > 0.0)
            return;
    }
    else
    {
        instFlags = colglobal->colInstanceGao->flags;
        if ((instFlags & 4) != 0 && (colglobal->collisionObjectPtr->flags & 4) == 0)
        {
            wantsAccelTest = 0;
            if ((instFlags & 0x400000) == 0 || colglobal->recomputeNeeded)
            {
                mapMtx = colglobal->colMapGaoMatrices;
                step.x = colglobal->centerPos.x - colglobal->centerPosPrevious.x;
                step.y = colglobal->centerPos.y - colglobal->centerPosPrevious.y;
                step.z = colglobal->centerPos.z - colglobal->centerPosPrevious.z;
                if (fabs(mapMtx->v3.z - 1.0) >= 0.001
                    || fabs(MATH_DotProduct(colglobal->ptrSetToVec6InZDMZDRGravityNeg, normal)) >= colglobal->wallCosAngle)
                {
                    if (step.z * nz + ny * step.y + nx * step.x > 0.0)
                        wantsAccelTest = 1;
                }
                else
                {
                    xPart = nx * step.x;
                    if (ny * step.y + xPart > 0.0 && step.z * nz + ny * step.y + xPart > 0.0)
                        wantsAccelTest = 1;
                }
            }
            else if (fabs(colglobal->colMapGaoMatrices->v3.z - 1.0) >= 0.001
                || fabs(MATH_DotProduct(colglobal->ptrSetToVec6InZDMZDRGravityNeg, normal)) >= colglobal->wallCosAngle
                || ny * colglobal->accelVecInZDMZDR.y + nx * colglobal->accelVecInZDMZDR.x > 0.0)
            {
                wantsAccelTest = 1;
            }
            if (wantsAccelTest && MATH_DotProduct(&colglobal->accelVecInZDMZDR, normal) > 0.0)
                return;
        }
    }
    i2 = tri->v3;
    verts = ((tCobMeshObjectMesh *)(colglobal->collisionObjectPtr->objectPtrByType))->vertices;
    i0 = tri->v1;
    p0 = &verts[i0];
    i1 = tri->v2;
    p1 = &verts[i1];
    p2x = verts[i2].x;
    i2s = i2;
    p2 = &verts[i2];
    if (MATH_f_MaxOf3(p0->x, p1->x, p2x) >= (double)rejectMin->x
        && MATH_f_MinOf3(p0->x, p1->x, p2->x) <= (double)rejectMax->x
        && MATH_f_MaxOf3(p0->y, p1->y, p2->y) >= (double)rejectMin->y
        && MATH_f_MinOf3(p0->y, p1->y, p2->y) <= (double)rejectMax->y
        && MATH_f_MaxOf3(p0->z, p1->z, p2->z) >= (double)rejectMin->z
        && MATH_f_MinOf3(p0->z, p1->z, p2->z) <= (double)rejectMax->z)
    {
        if (colglobal->collInstanceElem->elemType == 2)
            COL_Sphere_Triangle(colglobal, tri, i0, i1, i2s, normal, triIndex, chunkId, testDynamic);
        else
            COL_Box_Triangle(colglobal, tri, i0, i1, i2s, normal, triIndex, chunkId);
    }
}

BOOL INT_SphereAABBox(Vec3* center, float radius, Vec3* boxMin, Vec3* boxMax)
{
    double cx;
    double cy;
    double cz;
    double dist;

    cx = center->x;
    if (center->x > boxMax->x)
    {
        if (cx < boxMin->x && boxMin->x - center->x > radius)
            return 0;
    }
    else if (cx - boxMax->x > radius)
    {
        return 0;
    }
    cy = center->y;
    if (center->y > boxMax->y) // TODO FIXME lehet nem jó
    {
        if (cy < boxMin->y && boxMin->y - center->y > radius)
            return 0;
    }
    else if (cy - boxMax->y > radius)
    {
        return 0;
    }
    cz = center->z;
    if (center->z > boxMax->z)
    {
        if (cz >= boxMin->z)
            return 1;
        dist = boxMin->z - center->z;
    }
    else
    {
        dist = cz - boxMax->z;
    }
    return dist <= radius;
}

BOOL INT_SphereOBBox(Vec3* center, float radius, Vec3* boxMin, Vec3* boxMax, Vec3* closest)
{
    float cx;
    float cy;
    float cz;
    float distSq;

    if (center->x >= (double)boxMin->x)
    {
        if (center->x <= (double)boxMax->x)
            cx = center->x;
        else
            cx = boxMax->x;
    }
    else
    {
        cx = boxMin->x;
    }
    if (center->y >= (double)boxMin->y)
    {
        if (center->y <= (double)boxMax->y)
            cy = center->y;
        else
            cy = boxMax->y;
    }
    else
    {
        cy = boxMin->y;
    }
    if (center->z >= (double)boxMin->z)
    {
        if (center->z > (double)boxMax->z)
            cz = boxMax->z;
        else
            cz = center->z;
    }
    else
    {
        cz = boxMin->z;
    }
    if (closest)
    {
        closest->x = cx;
        closest->y = cy;
        closest->z = cz;
    }
    distSq = (center->y - cy) * (center->y - cy) + (center->x - cx) * (center->x - cx) + (center->z - cz) * (center->z - cz);

    if (radius * radius <= distSq) {
        return FALSE;
    }
    return TRUE;
}

int INT_FullSphereOBBox(Vec3* center, float radius, Vec3* boxMin, Vec3* boxMax, Vec3* rayOrigin, Vec3* rayDir, Vec3* outNormal, Vec3* outHit, int* outFlags)
{
    double dx;
    double dy;
    double dz;
    int result;
    tColBox box;
    float maxDist;

    dx = center->x - rayOrigin->x;
    dy = center->y - rayOrigin->y;
    dz = center->z - rayOrigin->z;
    box.pos1 = *boxMin;
    box.pos2 = *boxMax;
    maxDist = (float)(sqrt(dz * dz + dy * dy + dx * dx) + radius);
    if ((fabs(rayDir->x) != 0 || fabs(rayDir->y) != 0 || fabs(rayDir->z) != 0)
        && INT_FullRayAABBox(rayOrigin, rayDir, &box, outHit, maxDist, 0))
    {
        if (outFlags)
            *outFlags = 544;
        if (fabs(outHit->x - boxMax->x) < 0.0000099999997)
        {
            outNormal->x = 1.0;
            outNormal->y = 0.0;
            outNormal->z = 0.0;
        }
        if (fabs(outHit->y - boxMax->y) < 0.0000099999997)
        {
            outNormal->x = 0.0;
            outNormal->y = 1.0;
            outNormal->z = 0.0;
        }
        if (fabs(outHit->z - boxMax->z) < 0.0000099999997)
        {
            outNormal->x = 0.0;
            outNormal->y = 0.0;
            outNormal->z = 1.0;
        }
        if (fabs(outHit->x - boxMin->x) < 0.0000099999997)
        {
            outNormal->x = -1.0;
            outNormal->y = 0.0;
            outNormal->z = 0.0;
        }
        if (fabs(outHit->y - boxMin->y) < 0.0000099999997)
        {
            outNormal->x = 0.0;
            outNormal->y = -1.0;
            outNormal->z = 0.0;
        }
        if (fabs(outHit->z - boxMin->z) < 0.0000099999997)
        {
            outNormal->x = 0.0;
            outNormal->y = 0.0;
            outNormal->z = -1.0;
        }
        result = 1;
    }
    else
    {
        if (!INT_SphereOBBox(center, radius, boxMin, boxMax, outHit))
            return 0;
        if (outFlags)
            *outFlags = 544;
        outNormal->x = center->x - outHit->x;
        outNormal->y = center->y - outHit->y;
        outNormal->z = center->z - outHit->z;
        if (MATH_b_NulVector(outNormal))
            return 0;
        MATH_NormalizeEqualVector(outNormal);
        result = 1;
    }
    return result;
}

int INT_FullSphereCylinder(Vec3* center, float radius, Vec3* cylCenter, float cylRadius, float cylHalfHeight, Vec3* rayOrigin, Vec3* rayDir, Vec3* outNormal, Vec3* hitPosTalan, int* outFlags)
{
    double dx;
    double dy;
    double dz;
    double radialX;
    long double invLen;
    double zeroZ;
    Vec3* normal;
    double centerZ;
    float maxDist;
    Vec3 radial;
    float cylX;
    float cylY;
    float hitZ;

    dx = center->x - rayOrigin->x;
    dy = center->y - rayOrigin->y;
    dz = center->z - rayOrigin->z;
    cylX = cylCenter->x;
    cylY = cylCenter->y;
    maxDist = (float)sqrt(dz * dz + dy * dy + dx * dx);
    if (INT_b_FullRayCylinder(rayOrigin, rayDir, cylCenter, &MATH_gst_BaseVectorK, cylRadius, cylHalfHeight, maxDist, 0, hitPosTalan))
    {
        radial.x = (float)(hitPosTalan->x - cylX);
        radial.y = (float)(hitPosTalan->y - cylY);
        // preserved: the contact normal's z is always zero -- the subtraction here is hitPosTalan->z minus itself
        radial.z = (float)(hitPosTalan->z - hitPosTalan->z);
        if (fabs(radial.x) != 0 || fabs(radial.y) != 0 || fabs(radial.z) != 0)
        {
            radialX = radial.x;
        }
        else
        {
            radialX = 0.0;
            radial.y = 0.0;
            radial.z = 1.0;
        }
        invLen = 1.0 / sqrt(radial.z * radial.z + radial.y * radial.y + radialX * radialX);
        outNormal->x = (float)(invLen * radialX);
        outNormal->y = (float)(radial.y * invLen);
        outNormal->z = (float)(radial.z * invLen);
    }
    else
    {
        hitZ = center->z;
        if (radius + cylHalfHeight < fabs(hitZ - cylCenter->z))
            return 0;
        radial.x = center->x - cylX;
        radial.y = center->y - cylY;
        // preserved: as above -- the radial offset's z is center->z minus itself
        zeroZ = center->z - hitZ;
        radial.z = (float)zeroZ;
        if (radius + cylRadius < sqrt(zeroZ * radial.z + radial.y * radial.y + radial.x * radial.x))
            return 0;
        if (fabs(radial.x) != 0 || fabs(radial.y) != 0 || fabs(radial.z) != 0)
        {
            normal = outNormal;
            MATH_NormalizeVector(outNormal, &radial);
        }
        else
        {
            centerZ = center->z;
            normal = outNormal;
            outNormal->y = 0.0;
            outNormal->x = 0.0;
            if (centerZ < cylCenter->z || centerZ == cylCenter->z)
                outNormal->z = -1.0;
            else
                outNormal->z = 1.0;
        }
        radial.z = normal->z * cylRadius;
        hitPosTalan->x = (float)(cylRadius * normal->x + cylX);
        hitPosTalan->y = (float)(normal->y * cylRadius + cylY);
        hitPosTalan->z = (float)(radial.z + hitZ);
    }
    if (outFlags)
        *outFlags = 0x420;
    return 1;
}

int COL_ZDM_Box(tColGlobal* colGlobal)
{
    tCobGameMaterialElems* gmat;
    tColCollisionObject* colObj;
    tColBox* box;
    Mtx4x4* mapMtx;
    int flags;
    Vec3 scale;
    Vec3 boxMax;
    Vec3 boxMin;
    Vec3 normal;
    Vec3 hit;
    Vec3 recalVec;
    Vec3 reportNormal;
    Vec3 collidedPoint;

    gmat = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, 0);
    if (COL_GMat_Skip(gmat, colGlobal))
        return 0;
    colObj = colGlobal->collisionObjectPtr;
    box = (tColBox*)(((tCobNormalObjectGeneric*)(colObj->objectPtrByType))->enclosurePtr);
    boxMin.x = box->pos2.x;
    boxMin.y = box->pos2.y;
    boxMin.z = box->pos2.z;
    box = (tColBox*)(((tCobNormalObjectGeneric*)(colObj->objectPtrByType))->enclosurePtr);
    boxMax.x = box->pos1.x;
    boxMax.y = box->pos1.y;
    boxMax.z = box->pos1.z;
    mapMtx = colGlobal->colMapGaoMatrices;
    if ((mapMtx->mtxflag & 8) != 0)
    {
        MATH_GetScale(&scale, mapMtx);
        boxMin.x = boxMin.x * scale.x;
        boxMin.y = boxMin.y * scale.y;
        boxMin.z = boxMin.z * scale.z;
        boxMax.x = boxMax.x * scale.x;
        boxMax.y = boxMax.y * scale.y;
        boxMax.z = boxMax.z * scale.z;
    }
    if (!INT_FullSphereOBBox(&colGlobal->centerPos, colGlobal->radius, &boxMin, &boxMax, &colGlobal->centerPosPrevious, &colGlobal->moveVecUnit, &normal, &hit, &flags))
        return 0;
    if (COL_ComputeReport(&hit, &normal, &collidedPoint, &reportNormal, &recalVec, &flags, -1, -1, colGlobal))
        COL_ReportCollision(&collidedPoint, &reportNormal, &recalVec, flags, -1, -1, colGlobal);
    return 1;
}

int COL_ZDM_Sphere(tColGlobal* colGlobal)
{
    tCobGameMaterialElems* gmat;
    tColCollisionObject* colObj;
    tColSphere* sphere;
    Mtx4x4* mapMtx;
    BOOL intersects;
    float scaledRadius;
    int flags;
    Vec3 center;
    Vec3 scale;
    Vec3 normal;
    Vec3 hit;
    Vec3 recalVec;
    Vec3 reportNormal;
    Vec3 collidedPoint;

    gmat = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, 0);
    if (COL_GMat_Skip(gmat, colGlobal))
        return 0;
    colObj = colGlobal->collisionObjectPtr;
    sphere = (tColSphere*)(((tCobNormalObjectGeneric *)(colObj->objectPtrByType))->enclosurePtr);
    center.x = sphere->center.x;
    center.y = sphere->center.y;
    center.z = sphere->center.z;
    mapMtx = colGlobal->colMapGaoMatrices;
    if ((mapMtx->mtxflag & 8) != 0)
    {
        MATH_GetScale(&scale, mapMtx);
        scaledRadius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * ((tColSphere*)(((tCobNormalObjectGeneric*)(colObj->objectPtrByType))->enclosurePtr))->radius;
        center.x = center.x * scale.x;
        center.y = center.y * scale.y;
        center.z = center.z * scale.z;
        intersects = INT_FullSphereSphere(&colGlobal->centerPos, colGlobal->radius, &center, scaledRadius, &normal, &hit);
    }
    else
    {
        intersects = INT_FullSphereSphere(&colGlobal->centerPos, colGlobal->radius, &center, ((tColSphere*)(((tCobNormalObjectGeneric*)(colObj->objectPtrByType))->enclosurePtr))->radius, &normal, &hit);
    }
    if (!intersects)
        return 0;
    flags = 0x120;
    if (COL_ComputeReport(&hit, &normal, &collidedPoint, &reportNormal, &recalVec, &flags, -1, -1, colGlobal))
        COL_ReportCollision(&collidedPoint, &reportNormal, &recalVec, flags, -1, -1, colGlobal);
    return 1;
}

int COL_ZDM_Cylinder(tColGlobal* colGlobal)
{
    tCobGameMaterialElems* gmat;
    tColCollisionObject* colObj;
    tColCylinder* cylinder;
    Mtx4x4* mapMtx;
    double maxXYScale;
    tCobNormalObjectGeneric* cob;
    double scaledHalfHeight;
    float cylRadius;
    float cylHalfHeight;
    int flags;
    Vec3 center;
    Vec3 scale;
    Vec3 normal;
    Vec3 hit;
    Vec3 recalVec;
    Vec3 reportNormal;
    Vec3 collidedPoint;

    gmat = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, 0);
    if (COL_GMat_Skip(gmat, colGlobal))
        return 0;
    colObj = colGlobal->collisionObjectPtr;
    cylinder = (tColCylinder*)(((tCobNormalObjectGeneric*)(colObj->objectPtrByType))->enclosurePtr);
    center.x = cylinder->center.x;
    center.y = cylinder->center.y;
    center.z = cylinder->center.z;
    mapMtx = colGlobal->colMapGaoMatrices;
    if ((mapMtx->mtxflag & 8) != 0)
    {
        MATH_GetScale(&scale, mapMtx);
        center.x = center.x * scale.x;
        center.y = center.y * scale.y;
        center.z = center.z * scale.z;
        maxXYScale = scale.x < (double)scale.y ? scale.y : scale.x;
        cob = colObj->objectPtrByType;
        cylRadius = (float)(((tColCylinder*)(cob->enclosurePtr))->height * maxXYScale);
        scaledHalfHeight = ((tColCylinder*)(cob->enclosurePtr))->radius * scale.z;
    }
    else
    {
        cob = colObj->objectPtrByType;
        cylRadius = ((tColCylinder*)(cob->enclosurePtr))->height;
        scaledHalfHeight = ((tColCylinder*)(cob->enclosurePtr))->radius;
    }
    cylHalfHeight = (float)scaledHalfHeight;
    if (!INT_FullSphereCylinder(
        &colGlobal->centerPos,
        colGlobal->radius,
        &center,
        cylRadius,
        cylHalfHeight,
        &colGlobal->centerPosPrevious,
        &colGlobal->moveVecUnit,
        &normal,
        &hit,
        &flags))
        return 0;
    if (COL_ComputeReport(&hit, &normal, &collidedPoint, &reportNormal, &recalVec, &flags, -1, -1, colGlobal))
        COL_ReportCollision(&collidedPoint, &reportNormal, &recalVec, flags, -1, -1, colGlobal);
    return 1;
}

void COL_ComputeRejection(tColGlobal* colGlobal, Vec3* outMin, Vec3* outMax, int useDynamic)
{
    double radius;
    Vec3* center;

    if (useDynamic)
    {
        radius = colGlobal->dynRadius;
        center = &colGlobal->dynCenterPos;
    }
    else
    {
        radius = colGlobal->radius;
        center = &colGlobal->centerPos;
    }
    outMax->x = (float)(radius + center->x);
    outMax->y = (float)(radius + center->y);
    outMax->z = (float)(radius + center->z);
    outMin->x = (float)(center->x - radius);
    outMin->y = (float)(center->y - radius);
    outMin->z = (float)(center->z - radius);
}

void COL_ZDM_ZDR(tColGlobal* colGlobal)
{
    tColCollisionObject* colObj;
    tGameObjectMain* colInstGao;
    Vec3* accelField;
    tGameObjectBasic* basic;
    tDyn* dyn;
    double mass;
    Vec3* gravity;
    float negY;
    float negZ;
    double clampedX;
    double clampedY;
    double clampedZ;
    tColInstance* colInstance;
    tCobMeshObjectMesh* mesh;
    tCobMeshChunkElem* chunk;
    Vec3* prevPos;
    tCobOK3* ok3;
    tCobOK3AABBoxElem* aabox;
    tCobOK3AABBoxElem* aaboxEnd;
    tCobOK3AABBoxElemChunkElem* chunkRef;
    tCobGameMaterialElems* gmat;
    int faceBase;
    tCobMeshChunkElem* countChunk;
    tCobMeshTriangleElem* tri;
    unsigned int faceIdx;
    Vec3* normal;
    tCobMeshTriangleElem* triEnd;
    int face;
    float dt;
    int k;
    int faceAcc;
    int remaining;
    int16_t chunkId;
    int useDynamic;
    tCobMeshChunkElem* chunkEnd;
    tCobOK3AABBoxElemChunkElem* chunkRefEnd;
    uint16_t triIdx;
    Vec3 rejectMin;
    Vec3 accelVec;
    Vec3 rejectMax;
    Mtx4x4 hieMtx;

    colObj = colGlobal->collisionObjectPtr;
    colInstGao = colGlobal->colInstanceGao;
    colGlobal->ptrSetToVec6InZDMZDRGravityNeg = &MATH_gst_BaseVectorK;
    useDynamic = 0;
    accelField = &colGlobal->accelVecInZDMZDR;
    if ((colInstGao->flags & 4) != 0)
    {
        basic = colInstGao->basic;                  // OBJ_pst_GetDynaStruct
        if (basic)
            dyn = basic->dyn;
        else
            dyn = 0;
        mass = dyn->mass;
        accelVec.x = (float)(mass * dyn->forceVector.x);        // DYN_GetSpeedVector + MATH_ScaleVector
        accelVec.y = (float)(dyn->forceVector.y * mass);
        accelVec.z = (float)(dyn->forceVector.z * mass);
        if ((dyn->flags & 4) != 0)
        {
            gravity = &dyn->forces->gravityVector;
            if (!MATH_b_NulVector(gravity))
            {
                if ((colInstGao->flags & 0x400000) != 0)// OBJ_b_TestIdentityFlag
                {
                    MATH_MulMatrixMatrix(
                        colInstGao->matrices,
                        &colInstGao->basic->mtx->mtx,
                        colInstGao->basic->mtx->refGao2->matrices);
                    useDynamic = 1;
                }
                if ((dyn->flags & 0x40000) == 0)       // DYN_ul_TestDynFlags inine
                {
                    MATH_NormalizeVector(&colGlobal->vec6InZDMZDRGravityNeg, &dyn->forces->gravityVector);// DYN_pst_GetGravity
                    negY = colGlobal->vec6InZDMZDRGravityNeg.y;
                    colGlobal->vec6InZDMZDRGravityNeg.x = -colGlobal->vec6InZDMZDRGravityNeg.x; // MATH_NegEqualVector
                    negZ = colGlobal->vec6InZDMZDRGravityNeg.z;
                    colGlobal->vec6InZDMZDRGravityNeg.y = -negY;
                    colGlobal->vec6InZDMZDRGravityNeg.z = -negZ;
                    colGlobal->ptrSetToVec6InZDMZDRGravityNeg = &colGlobal->vec6InZDMZDRGravityNeg;
                }
            }
        }
        MATH_TransformVector(&colGlobal->accelVecInZDMZDR, &colGlobal->colMapGaoInvMatrix, &accelVec);
    }
    else
    {
        accelField->x = 0.0;
        colGlobal->accelVecInZDMZDR.y = 0.0;
        colGlobal->accelVecInZDMZDR.z = 0.0;
    }
    if (fabs(accelField->x) >= 0.001)
        clampedX = accelField->x;
    else
        clampedX = 0.0;
    accelField->x = (float)clampedX;
    if (fabs(accelField->y) >= 0.001)
        clampedY = accelField->y;
    else
        clampedY = 0.0;
    accelField->y = (float)clampedY;
    if (fabs(accelField->z) >= 0.001)
        clampedZ = accelField->z;
    else
        clampedZ = 0.0;
    accelField->z = (float)clampedZ;
    switch (colObj->objectType)
    {
    case 1u:
        if (colGlobal->collInstanceElem->elemType == 2)// COL_b_Zone_TestType inline
            COL_ZDM_Box(colGlobal);
        break;
    case 2u:
        if (colGlobal->collInstanceElem->elemType == 2)
            COL_ZDM_Sphere(colGlobal);
        break;
    case 3u:
        if (colGlobal->collInstanceElem->elemType == 2)
            COL_ZDM_Cylinder(colGlobal);
        break;
    case 5u:
        if ((colObj->flags & 4) != 0)
        {
            colInstance = colInstGao->extended->coll->colInstance;
            colInstance->flag &= ~0x8000u; //HIBYTE(v13->flag) &= ~0x80u;
        }
        mesh = colObj->objectPtrByType;
        chunk = mesh->meshChunkElems;
        chunkEnd = &chunk[mesh->meshChunkElemNum];
        colGlobal->recordCountBackup = colGlobal->colWorld->recordCount;
        if ((colInstGao->flags & 4) != 0)       // OBJ_b_TestIdentityFlag inline
        {
            dt = TIM_gf_dt;
            if ((colInstGao->basic->dyn->flags & 0x4000000) != 0)
                dt = TIM_gf_dt / TIM_gf_SpeedFactor;
            if (colGlobal->radius + colGlobal->radius < MATH_VecLen(&colGlobal->accelVecInZDMZDR) * dt)
            {
                useDynamic = 1;
            }
            else if (!useDynamic)
            {
                if ((colInstGao->flags & 0x400000) == 0)
                {
                    prevPos = &colInstGao->extended->coll->colInstance->mtx->Pos;
                }
                else
                {
                    MATH_MulMatrixMatrix(&hieMtx, colInstGao->extended->coll->colInstance->mtx, colInstGao->basic->mtx->refGao2->matrices);
                    prevPos = &hieMtx.Pos;
                }
                MATH_SubstractVec(&rejectMin, &colInstGao->matrices->Pos, prevPos);
                if (colGlobal->radius + colGlobal->radius < MATH_VecLen(&rejectMin))
                    useDynamic = 1;
            }
        }
        COL_ComputeRejection(colGlobal, &rejectMin, &rejectMax, useDynamic);
        if (((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->ok3)
        {
            ok3 = mesh->ok3;
            memset(ok3->faceTagInts, 0, 4 * (mesh->faceCount >> 5) + 4);
            aabox = ok3->aaBoxes;
            aaboxEnd = &aabox[ok3->num];
            for (; aabox < aaboxEnd; ++aabox)
            {
                if (!INT_SphereAABBox(&colGlobal->dynCenterPos, colGlobal->dynRadius, &aabox->vec1, &aabox->vec2))
                    continue;
                chunkRef = aabox->chunkElems;
                chunkRefEnd = &chunkRef[aabox->num];
                for (; chunkRef < chunkRefEnd; ++chunkRef)
                {
                    chunk = &mesh->meshChunkElems[chunkRef->triangleChunkID];
                    gmat = COL_pst_GMat_Get(colObj, chunk);
                    if (COL_GMat_Skip(gmat, colGlobal))
                        continue;
                    colGlobal->cobMeshChunkElem = chunk;
                    faceBase = 0;
                    if (chunkRef->triangleChunkID)
                    {
                        countChunk = mesh->meshChunkElems;
                        remaining = chunkRef->triangleChunkID;
                        faceAcc = 0;
                        do
                        {
                            faceAcc += countChunk->elemNum;
                            ++countChunk;
                            --remaining;
                        } while (remaining);
                        faceBase = faceAcc;
                    }
                    for (k = 0; k < chunkRef->num; k++)
                    {
                        triIdx = chunkRef->triangleChunkElemIDs[k];
                        tri = &chunk->triangleElems[triIdx];
                        faceIdx = triIdx + faceBase;
                        if (!COL_b_TestFaceTag(ok3, faceIdx))
                        {
                            normal = &mesh->normals[faceIdx];
                            colGlobal->cobMeshTriangleElem = tri;
                            COL_TestAndCollideTriangleIfNeeded(colGlobal, tri, normal, chunkRef->triangleChunkID, triIdx, &rejectMin, &rejectMax, useDynamic);
                            COL_SetFaceTag(ok3, faceIdx);
                        }
                    }
                }
            }
        }
        else
        {
            faceBase = 0;
            chunkId = 0;
            for (; chunk < chunkEnd; ++chunk, ++chunkId)
            {
                gmat = COL_pst_GMat_Get(colObj, chunk);
                if (COL_GMat_Skip(gmat, colGlobal))
                {
                    faceBase += chunk->elemNum;
                    continue;
                }
                colGlobal->cobMeshChunkElem = chunk;
                tri = chunk->triangleElems;
                triEnd = &tri[chunk->elemNum];
                if (tri >= triEnd)
                    continue;
                face = faceBase;
                faceBase += chunk->elemNum;
                k = 0;
                do
                {
                    normal = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[face]);
                    colGlobal->cobMeshTriangleElem = tri;
                    COL_TestAndCollideTriangleIfNeeded(colGlobal, tri, normal, chunkId, k, &rejectMin, &rejectMax, useDynamic);
                    ++tri;
                    ++face;
                    ++k;
                } while (tri < triEnd);
            }
        }
        break;
    default:
        return;
    }
}

int COL_Actor_GameObject(tColGlobal* colGlobal)
{
    tColInstance* colInstance;
    tColMap* colMap;
    tColInstanceElem** elem;
    tColCollisionObject** colObj;
    uint8_t zone;
    Mtx4x4* mapMtx;
    int invMatrixCalculated;
    tColCollisionObject** colObjEnd;
    tColInstanceElem** elemEnd;
    uint8_t elemType;
    int elemIdx;
    int objIdx;

    colInstance = colGlobal->colInstanceGao->extended->coll->colInstance;
    colMap = colGlobal->colMapGao->extended->coll->colMap;
    invMatrixCalculated = 0;
    if (!colMap)
        return 1;
    colGlobal->colInstance = colInstance;
    colGlobal->colMap = colMap;
    elem = colInstance->selectedInstElemPtrs;
    elemEnd = &elem[colInstance->zoneNum];
    colGlobal->colInstanceElemPtrs = elem;
    for (elemIdx = 0; elem < elemEnd; ++elem, ++elemIdx)
    {
        colObj = colMap->collisionObjectPtr;
        colObjEnd = &colObj[colMap->colObjectID];
        zone = 0;
        while (colInstance->colSet->zoneIDs[zone] != (char)elemIdx)
        {
            if (++zone >= 16u)
                break;
        }
        if (zone >= 16u)
            continue;
        if ((colInstance->activationFlags & (1 << zone)) == 0 || ((*elem)->colFlags & 1) == 0)
            continue;
        if (!invMatrixCalculated)
        {
            mapMtx = colGlobal->colMapGao->matrices;
            invMatrixCalculated = 1;
            if ((mapMtx->mtxflag & 8) != 0)
            {
                MATH_Transp33MatrixWithoutBuffer(&colGlobal->colMapGaoInvMatrix, mapMtx);
                colGlobal->colMapGaoInvMatrix.mtxflag = mapMtx->mtxflag;
                if ((mapMtx->mtxflag & 8) != 0)
                {
                    colGlobal->colMapGaoInvMatrix._m14 = (float)(1.0 / mapMtx->_m14);
                    colGlobal->colMapGaoInvMatrix._m24 = (float)(1.0 / mapMtx->_m24);
                    colGlobal->colMapGaoInvMatrix._m34 = (float)(1.0 / mapMtx->_m34);
                }
                colGlobal->colMapGaoInvMatrix.Pos.x = -mapMtx->Pos.x;
                colGlobal->colMapGaoInvMatrix.Pos.y = -mapMtx->Pos.y;
                colGlobal->colMapGaoInvMatrix.Pos.z = -mapMtx->Pos.z;
                MATH_TransformVectorNoScale(
                    &colGlobal->colMapGaoInvMatrix.Pos,
                    &colGlobal->colMapGaoInvMatrix,
                    &colGlobal->colMapGaoInvMatrix.Pos);
            }
            else
            {
                MATH_InvertMatrix(&colGlobal->colMapGaoInvMatrix, mapMtx);
            }
        }
        elemType = (*elem)->elemType;
        colGlobal->collInstanceElem = *elem;
        COL_A_BCS_ComputeZDxInfo(colGlobal);
        if (elemType == 1)
        {
            for (objIdx = 0; colObj < colObjEnd; ++colObj, ++objIdx)
            {
                if (((uint8_t)(1 << objIdx) & colMap->activation) != 0)
                {
                    colGlobal->collisionObjectPtr = *colObj;
                                /* TEST code, to be removed
                                tGameObjectMain* sally = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x71003f5a);
                                tGameObjectMain* mammagodecal = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0xf3000404);

                                if (sally != (tGameObjectMain*)-1 && mammagodecal != (tGameObjectMain*)-1) {
                                   // DBG_OutputDebugString("(1) Mammago level mesh and sally coll %d\n", v10);
                                }
                                */
                    COL_ZDM_ZDR(colGlobal);
                }
            }
        }
        else if (elemType == 2)
        {
            for (objIdx = 0; colObj < colObjEnd; ++colObj, ++objIdx)
            {
                if (((uint8_t)(1 << objIdx) & colMap->activation) != 0)
                {
                    colGlobal->collisionObjectPtr = *colObj;
                                /* TEST code, to be removed
                                tGameObjectMain* sally = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x71003f5a);
                                tGameObjectMain* mammagodecal = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0xf3000404);

                                if (sally != (tGameObjectMain*)-1 && mammagodecal != (tGameObjectMain*)-1) {
                                   // DBG_OutputDebugString("(2) Mammago level mesh and sally coll %d\n", v9);
                                }
                                */
                    COL_ZDM_ZDR(colGlobal);
                }
            }
        }
    }
    return 1;
}

void COL_UpdateDynamicHierarchyLink(tGameObjectMain* gao, tGameObjectMain* refGao)
{
    tGaoMatrix* gaoMtx;
    Mtx4x4* colInstMtx;
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tGameObjectMain* walk;
    int flags;
    int attach;
    Mtx4x4 refInvMtx;
    Mtx4x4 refAbsMtx;
    Mtx4x4 localMtx;

    if ((gao->flags & 0x400000) != 0)
    {
        gaoMtx = gao->basic->mtx;
        if (!gaoMtx->refGao && ((refGao->flags & 4) == 0 || refGao != gaoMtx->refGao2))
        {
            OBJ_ComputeGlobalWhenHie(gao);
            MATH_MulMatrixMatrix(&refInvMtx, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
            colInstMtx = gao->extended->coll->colInstance->mtx;
            if (colInstMtx != &refInvMtx)
                memcpy(colInstMtx, &refInvMtx, sizeof(Mtx4x4));
            if ((gao->flags & 0x4000000) != 0)
                ANI_RemoveHierarchyOnFlash(gao);
            gao->flags &= ~0x400000u;
            MEM_Free(gao->basic->mtx);
            gao->basic->mtx = 0;
        }
    }
    flags = gao->flags;
    if ((gao->flags & 0x400000) != 0 || (gao->flags & 4) == 0 || (gao->basic->dyn->flags & 0x1000000) != 0)
        return;
    extended = refGao->extended;
    if (!extended)
        return;
    coll = extended->coll;
    if (!coll)
        return;
    if (!coll->colMap || (refGao->flags & 4) == 0 || (refGao->basic->dyn->flags & 0x8000u) != 0) // BYTE1(v2->basic->dyn->flags) & 0x80u
        return;
    attach = 1;
    walk = refGao;
    if (get_reference_point_gao_2_from_gao_sub_40BEA0(refGao))
    {
        attach = 0;
        while (get_reference_point_gao_2_from_gao_sub_40BEA0(walk) != gao)
        {
            walk = get_reference_point_gao_2_from_gao_sub_40BEA0(walk);
            if (!get_reference_point_gao_2_from_gao_sub_40BEA0(walk))
            {
                attach = 1;
                break;
            }
        }
    }
    if (!attach)
        return;
    OBJ_ChangeIdentityFlags(gao, flags | 0x400000, flags);
    if ((refGao->flags & 0x600) != 0)
    {
        if ((refGao->flags & 0x400000) != 0)
        {
            MATH_MulMatrixMatrix(&refAbsMtx, refGao->extended->coll->colInstance->mtx, refGao->basic->mtx->refGao2->matrices);
            MATH_InvertMatrix(&refInvMtx, &refAbsMtx);
        }
        else
        {
            MATH_InvertMatrix(&refInvMtx, refGao->extended->coll->colInstance->mtx);
        }
    }
    else
    {
        MATH_InvertMatrix(&refInvMtx, refGao->matrices);
    }
    MATH_MulMatrixMatrix(&gao->basic->mtx->mtx, gao->matrices, &refInvMtx);
    MATH_MulMatrixMatrix(&localMtx, gao->extended->coll->colInstance->mtx, &refInvMtx);
    MATH_CopyMatrix(gao->extended->coll->colInstance->mtx, &localMtx);
    gao->basic->mtx->refGao = 0;
    gao->basic->mtx->refGao2 = refGao;
    if ((gao->flags & 0x4000000) != 0)
        ANI_ApplyHierarchyOnFlash(gao);
}

int COL_b_SlowActorVsFastActor(tRecordElem* recElem, tColGlobal* colGlobal)
{
    tGameObjectMain* instGao;
    tGameObjectExtended* instExt;
    int instIsFast;
    tGameObjectMain* mapGao;
    tGameObjectBasic* instBasic;
    tDyn* instDyn;
    tGameObjectBasic* mapBasic;
    tDyn* mapDyn;
    tColInstance* instColInstance;
    tGameObjectMain* otherGao;
    tGameObjectExtended* otherExt;
    tGameObjectCollMain* otherColl;
    tColInstance* otherColInstance;
    tColInstance* myColInstance;
    int higherPriority;
    int8_t myPriority;
    int8_t otherPriority;
    tDyn* myDyn;
    long double myForceY;
    long double myForceX;
    tDyn* otherDyn;
    int slot;
    float recalX;
    tGameObjectExtended* mapExt;
    tGameObjectCollMain* mapColl;
    tColMap* mapColMap;
    tColCollisionObject* mapColObj;
    int newCount;
    tWorld* world;
    tRecordElem* scan;
    tRecordElem* scanEnd;
    Vec3* recalVec;
    Vec3 negNormal;
    tWorld* recWorld;
    float otherLenSq;
    float negRecalY;
    float negRecalZ;

    if (!recElem)
        return 0;
    instGao = recElem->collisionInstanceGameObject;
    instExt = recElem->collisionInstanceGameObject->extended;
    if (instExt && (instExt->extraFlag & 0x100) != 0)
        return 0;
    instIsFast = 0;
    if (instExt && (instExt->extraFlag & 0x10) != 0)
        instIsFast = 1;
    if ((instGao->flags & 0x600) != 0
        && (mapGao = recElem->collisionMapGameObject, (mapGao->flags & 0x600) != 0)
        && (instBasic = instGao->basic) != 0
        && (instDyn = instBasic->dyn) != 0
        && (instDyn->flags & 0x2000000) != 0
        && (mapBasic = mapGao->basic) != 0
        && (mapDyn = mapBasic->dyn) != 0
        && (mapDyn->flags & 0x2000000) != 0
        && (instColInstance = instExt->coll->colInstance, instColInstance->priority == mapGao->extended->coll->colInstance->priority))
    {
        if ((recElem->flags & 0x80) == 0 || (instColInstance->flag & 0x100) != 0)
            return 0;
    }
    else if (!instIsFast)
    {
        higherPriority = 0;
        if (instGao)
        {
            otherGao = recElem->collisionMapGameObject;
            if (otherGao
                && (otherExt = otherGao->extended) != 0
                && (otherColl = otherExt->coll) != 0
                && (otherColInstance = otherColl->colInstance) != 0
                && instExt
                && instExt->coll
                && (myColInstance = instExt->coll->colInstance) != 0)
            {
                myPriority = myColInstance->priority;
                otherPriority = otherColInstance->priority;
                if (myPriority > otherPriority)
                    higherPriority = 1;
                else if (myPriority < otherPriority)
                    return 0;
            }
        }
        if (!higherPriority)
        {
            myDyn = instGao->basic->dyn;
            myForceY = fabs(myDyn->forceVector.y);
            myForceX = myDyn->forceVector.x;
            otherDyn = recElem->collisionMapGameObject->basic->dyn;
            if (myForceY + fabs(myForceX) >= fabs(otherDyn->forceVector.y) + fabs(otherDyn->forceVector.x))
                return 0;
        }
    }
    slot = colGlobal->num;
    if (colGlobal->num >= 1000u)
        return 0;
    recalX = recElem->recalVec.x;
    recalVec = &recElem->recalVec;
    negRecalZ = -recElem->recalVec.z;
    negNormal.x = -recElem->normalVec.x;
    negNormal.y = -recElem->normalVec.y;
    negNormal.z = -recElem->normalVec.z;
    negRecalY = -recElem->recalVec.y;
    colGlobal->colInstanceGao = recElem->collisionMapGameObject;
    colGlobal->colMapGao = recElem->collisionInstanceGameObject;
    recWorld = colGlobal->colWorld;
    colGlobal->colWorld->records[slot].collidedPoint = recElem->collidedPoint;
    colGlobal->colWorld->records[slot].recalVec.x = -recalX;
    colGlobal->colWorld->records[slot].recalVec.y = negRecalY;
    colGlobal->colWorld->records[slot].recalVec.z = negRecalZ;
    colGlobal->colWorld->records[slot].normalVec = negNormal;
    colGlobal->colWorld->records[slot].collisionInstanceGameObject = recElem->collisionMapGameObject;
    colGlobal->colWorld->records[slot].collisionMapGameObject = recElem->collisionInstanceGameObject;
    colGlobal->colWorld->records[slot].flags = recElem->flags;
    colGlobal->colWorld->records[slot].triangleChunkElemID = recElem->triangleChunkElemID;
    colGlobal->colWorld->records[slot].triangleChunkID = recElem->triangleChunkID;
    if (recElem->collisionInstanceGameObject
        && (mapExt = recElem->collisionInstanceGameObject->extended) != 0
        && (mapColl = mapExt->coll) != 0
        && (mapColMap = mapColl->colMap) != 0
        && (mapColObj = *mapColMap->collisionObjectPtr) != 0
        && mapColObj->gameMaterial
        && mapColObj->objectType != 5)
    {
        colGlobal->colWorld->records[slot].gameMaterial = COL_pst_GMat_Get(mapColObj, 0);
    }
    else
    {
        colGlobal->colWorld->records[slot].gameMaterial = 0;
    }
    newCount = colGlobal->num + 1;
    colGlobal->num = newCount;
    recWorld->recordCount = newCount;
    recElem->flags |= 0x1000u;
    world = colGlobal->colWorld;
    if (colGlobal->recomputeNeeded)
        scan = &world->records[colGlobal->firstRecordToRecompute];
    else
        scan = world->records;
    scanEnd = &world->records[colGlobal->num - 1];
    if (scan < scanEnd)
    {
        while ((scan->flags & 0x800F3000) != 0
            || scan->collisionInstanceGameObject != recElem->collisionMapGameObject
            || scan->collisionMapGameObject != recElem->collisionInstanceGameObject)
        {
            if (++scan >= scanEnd)
                return 1;
        }
        otherLenSq = MATH_VecLenSquared(&scan->recalVec);
        if (MATH_VecLenSquared(recalVec) < (double)otherLenSq)
        {
            world->records[colGlobal->num - 1].flags = world->records[colGlobal->num - 1].flags | 0x1000;
            return 1;
        }
        scan->flags |= 0x1000u;
    }
    return 1;
}

static void COL_l_CollideWithDetectionList(tGameObjectMain* gao)
{
    tGameObjectCollDetectionList* list;
    tGameObjectMain** entry;
    tGameObjectMain** last;
    tGameObjectMain* other;
    int otherFlags;

    list = gao->extended->coll->detectionList;
    entry = list->detectionListGaos;
    for (last = &entry[list->overlapNum - 1]; entry <= last; ++entry)
    {
        other = *entry;
        otherFlags = (*entry)->flags;
        if ((otherFlags & 0x100) != 0
            && (other->flags2 & 1) != 0
            && ((otherFlags & 0x400000) == 0 || other->basic->mtx->refGao2 != gao))
        {
            COL_InitGlobalVars(gao, *entry);
            COL_Actor_GameObject(&COL_gst_GlobalVars);
        }
    }
}

static void COL_l_ApplyRecomputedMoves(uint8_t count)
{
    int n;
    tGameObjectMain* gao;
    Mtx4x4** gaoMtx;
    Mtx4x4* colInstMtx;
    Vec3* pos;
    tColInstance* colInstance;
    Vec3 move;
    Mtx4x4 hieMtx;

    if (!count)
        return;
    n = 0;
    do
    {
        gao = COL_gst_GlobalVars.gaosNeededToBeRecomputed[n];
        if ((gao->flags & 0x400000) != 0)
        {
            MATH_MulMatrixMatrix(&hieMtx, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
            gaoMtx = &COL_gst_GlobalVars.gaosNeededToBeRecomputed[n]->matrices;
            move.x = hieMtx.Pos.x - (*gaoMtx)->Pos.x;
            move.y = hieMtx.Pos.y - (*gaoMtx)->Pos.y;
            move.z = hieMtx.Pos.z - (*gaoMtx)->Pos.z;
        }
        else
        {
            gaoMtx = &gao->matrices;
            colInstMtx = gao->extended->coll->colInstance->mtx;
            move.x = colInstMtx->Pos.x - (*gaoMtx)->Pos.x;
            move.y = colInstMtx->Pos.y - (*gaoMtx)->Pos.y;
            move.z = colInstMtx->Pos.z - (*gaoMtx)->Pos.z;
        }
        pos = &(*gaoMtx)->Pos;
        // !!!a Collision itt is pozíciót állít, ha ütközik valami valamivel és a ComputeReports fvben is!!!
        pos->x = move.x + pos->x;
        pos->y = move.y + pos->y;
        pos->z = move.z + pos->z;

        // TODO FIXME: ez így jó?
        // TODO FIXME ez biztos jó?
        ANI_UpdateFlashAfterMagicBoxTranslation(gao, &move, 3);
        colInstance = COL_gst_GlobalVars.gaosNeededToBeRecomputed[n]->extended->coll->colInstance;
        if (colInstance)
            colInstance->flag |= 2u;
        ++n;
    } while (n < COL_gst_GlobalVars.gaosNeededToBeRecomputedNum);
}

void COL_PreComputeReports(tColGlobal* colglobal)
{
    tWorld* world;
    tRecordElem* rec;
    tRecordElem* end;
    tRecordElem* endLimit;
    tRecordElem* scanStart;
    tRecordElem* scan;
    tGameObjectMain* instGao;
    tGameObjectMain* mapGao;
    tGameObjectExtended* instExt;
    tGameObjectCollMain* instColl;
    tColInstance* instColInstance;
    int anyDropped;
    int scanFlags;
    int anyNoNormal;
    int noNormal;
    tGameObjectExtended* ext;
    tGameObjectCollMain* coll;
    tColInstance* colInstance;
    long double invLen;
    long double invLen2D;
    int newCount;
    tRecordElem* cand;
    double bestLen;
    int candFlags;
    char count;
    float candLen;
    int slippery;
    float bestLenF;
    float nx;
    float ny;
    float nz;
    Vec3 avgNormal;

    if (colglobal->recomputeNeeded)
    {
        world = colglobal->colWorld;
        rec = &world->records[colglobal->firstRecordToRecompute];
        end = &world->records[colglobal->num];
    }
    else
    {
        rec = colglobal->colWorld->records;
        end = &rec[colglobal->num];
    }
    scanStart = rec;
    endLimit = end;
    if (rec < end)
    {
        do
        {
            if ((rec->flags & 0x800F3080) == 0)
            {
                instGao = rec->collisionInstanceGameObject;
                mapGao = rec->collisionMapGameObject;
                if (!instGao
                    || (instExt = instGao->extended) == 0
                    || (instColl = instExt->coll) == 0
                    || (instColInstance = instColl->colInstance) == 0
                    || (instColInstance->flag & 0x8000) == 0)
                {
                    if (mapGao
                        && ((ext = instGao->extended) != 0 && (ext->extraFlag & 0x10) != 0 && (mapGao->flags & 4) != 0
                            || (instGao->flags & 4) != 0 && (mapGao->flags & 4) != 0)
                        && COL_b_SlowActorVsFastActor(rec, colglobal))
                    {
                        COL_NeedRecomputation(colglobal, mapGao, 256);
                        ++end;
                    }
                    else
                    {
                        if (instGao == WOR_gpst_CurrentWorld->viewPtr->assignedGao)
                        {
                            colInstance = instGao->extended->coll->colInstance;
                            //HIBYTE(v11->flag) |= 0x40u;
                            colInstance->flag |= 0x4000u;
                        }
                        anyDropped = 0;
                        if (scanStart < end)
                        {
                            scan = scanStart;
                            do
                            {
                                if (scan != rec && scan->collisionInstanceGameObject == instGao)
                                {
                                    scanFlags = scan->flags;
                                    if ((scan->flags & 0x80u) != 0 && (scanFlags & 0xF0000) == 0)
                                    {
                                        scan->flags = scanFlags | 0x1000;
                                        anyDropped = 1;
                                    }
                                }
                                ++scan;
                            } while (scan < end);
                            if (anyDropped)
                                COL_NeedRecomputation(colglobal, instGao, 0x10);
                        }
                        anyNoNormal = (rec->flags & 0x20) == 0;
                        for (scan = rec + 1; scan < end; ++scan) // next
                        {
                            if (scan->collisionInstanceGameObject == instGao && (scan->flags & 0x3080) == 0)
                            {
                                if (!anyNoNormal)
                                    anyNoNormal = (scan->flags & 0x20) == 0;
                                if (scan->recalVec.z > (double)rec->recalVec.z)
                                {
                                    rec->recalVec.x = scan->recalVec.x;
                                    rec->recalVec.y = scan->recalVec.y;
                                    rec->recalVec.z = scan->recalVec.z;
                                    rec->normalVec.x = scan->normalVec.x;
                                    rec->normalVec.y = scan->normalVec.y;
                                    rec->normalVec.z = scan->normalVec.z;
                                    rec->collidedPoint.x = scan->collidedPoint.x;
                                    rec->collidedPoint.y = scan->collidedPoint.y;
                                    rec->collidedPoint.z = scan->collidedPoint.z;
                                    rec->collisionMapGameObject = scan->collisionMapGameObject;
                                    rec->flags = scan->flags;
                                    rec->gameMaterial = scan->gameMaterial;
                                    mapGao = scan->collisionMapGameObject;
                                }
                                scan->flags |= 0x1000u;
                            }
                        }
                        COL_UpdateDynamicHierarchyLink(instGao, mapGao);
                        if (instGao
                            && (ext = instGao->extended) != 0
                            && (coll = ext->coll) != 0
                            && (colInstance = coll->colInstance) != 0
                            && (colInstance->flag & 0x4000) != 0)
                        {
                            COL_NeedRecomputation(colglobal, instGao, 0x200);
                        }
                        else if (anyNoNormal)
                        {
                            COL_NeedRecomputation(colglobal, instGao, 0x80);
                        }
                    }
                }
            }
            ++rec;
        } while (rec < end);
        endLimit = end;
    }
    rec = colglobal->colWorld->records;
    if (colglobal->recomputeNeeded)
        rec += colglobal->firstRecordToRecompute;
    for (; rec < endLimit; ++rec)
    {
        if ((rec->flags & 0x800F3040) != 0)
            continue;
        instGao = rec->collisionInstanceGameObject;
        mapGao = rec->collisionMapGameObject;
        if (instGao
            && (ext = instGao->extended) != 0
            && (coll = ext->coll) != 0
            && (colInstance = coll->colInstance) != 0
            && (colInstance->flag & 0x8000) != 0)
        {
            continue;
        }
        if ((instGao->flags & 0x600) != 0)
        {
            colInstance = instGao->extended->coll->colInstance;
            colInstance->flag |= 0x4000u; //HIBYTE(v28->flag) |= 0x40u;
        }
        if (mapGao
            && ((ext = instGao->extended) != 0 && (ext->extraFlag & 0x10) != 0 && (mapGao->flags & 4) != 0
                || (instGao->flags & 4) != 0 && (mapGao->flags & 4) != 0)
            && COL_b_SlowActorVsFastActor(rec, colglobal))
        {
            COL_NeedRecomputation(colglobal, mapGao, 0x100);
            ++endLimit;
            continue;
        }
        ext = instGao->extended;
        if (ext
            && (coll = ext->coll) != 0
            && (colInstance = coll->colInstance) != 0
            && (colInstance->flag & 1) == 0)
        {
            if ((rec->flags & 0x800) != 0)
            {
                nx = 0.0;
                ny = 0.0;
                nz = 0.0;
                count = 0;
            }
            else
            {
                nx = rec->normalVec.x;
                ny = rec->normalVec.y;
                nz = rec->normalVec.z;
                count = 1;
            }
            for (scan = rec + 1; scan < endLimit; ++scan) // next
            {
                if (scan->collisionInstanceGameObject == instGao
                    && (scan->flags & 0x80003840) == 0
                    && nz * scan->normalVec.z + ny * scan->normalVec.y + nx * scan->normalVec.x < 0.60000002)
                {
                    ++count;
                    nx = nx + scan->normalVec.x;
                    ny = ny + scan->normalVec.y;
                    nz = nz + scan->normalVec.z;
                    if (fabs(nx) != 0 || fabs(ny) != 0 || fabs(nz) != 0)
                    {
                        invLen = 1.0 / sqrt(ny * ny + nx * nx + nz * nz);
                        nx = (float)(nx * invLen);
                        ny = (float)(ny * invLen);
                        nz = (float)(invLen * nz);
                    }
                }
            }
            if ((uint8_t)count >= 2u
                && (fabs(nx) != 0 || fabs(ny) != 0 || fabs(nz) != 0) && colglobal->num < 1000u)
            {
                world = colglobal->colWorld;
                world->records[colglobal->num].collidedPoint = MATH_gst_NulVector;
                world->records[colglobal->num].recalVec = MATH_gst_NulVector;
                world->records[colglobal->num].collisionInstanceGameObject = instGao;
                invLen2D = 1.0 / sqrt(ny * ny + nx * nx);
                avgNormal.x = (float)(nx * invLen2D);
                avgNormal.y = (float)(ny * invLen2D);
                avgNormal.z = (float)(invLen2D * 0.0);
                world->records[colglobal->num].normalVec = avgNormal;
                world->records[colglobal->num].collisionMapGameObject = rec->collisionMapGameObject;
                world->records[colglobal->num].triangleChunkElemID = 0;
                world->records[colglobal->num].triangleChunkID = 0;
                world->records[colglobal->num].gameMaterial = 0;
                world->records[colglobal->num].flags = 0x100A1;
                colInstance = instGao->extended->coll->colInstance;
                colInstance->flag |= 2u; //LOBYTE(v39->flag) |= 2u;
                newCount = colglobal->num + 1;
                colglobal->num = newCount;
                endLimit = &colglobal->colWorld->records[newCount];
                world->recordCount = newCount;
            }
            colInstance = instGao->extended->coll->colInstance;
            colInstance->flag |= 1u; //LOBYTE(v41->flag) |= 1u;
        }
        if ((rec->flags & 0x20000) != 0)
            continue;
        slippery = rec->flags & 0x800;
        noNormal = (rec->flags & 0x20) == 0;
        bestLen = rec->recalVec.y * rec->recalVec.y
            + rec->recalVec.z * rec->recalVec.z
            + rec->recalVec.x * rec->recalVec.x;
        bestLenF = (float)bestLen;
        for (cand = rec + 1; cand < endLimit; ++cand) // NEXT
        {
            if (cand->collisionInstanceGameObject != instGao)
                continue;
            candFlags = cand->flags;
            if ((candFlags & 0x800F3040) != 0)
                continue;
            candLen = cand->recalVec.x * cand->recalVec.x + cand->recalVec.y * cand->recalVec.y + cand->recalVec.z * cand->recalVec.z;
            if (noNormal)
            {
                if ((candFlags & 0x20) == 0 && candLen > bestLen)
                {
                    bestLen = candLen;
                    COL_l_SwapRecords(rec, cand);
                }
            }
            else if (slippery)
            {
                if ((candFlags & 0x800) != 0 && candLen > bestLen)
                {
                    bestLen = candLen;
                    COL_l_SwapRecords(rec, cand);
                }
            }
            else if ((candFlags & 0x20) == 0)
            {
                noNormal = 1;
                bestLen = candLen;
                COL_l_SwapRecords(rec, cand);
            }
            else if ((candFlags & 0x800) != 0)
            {
                slippery = 1;
                bestLen = candLen;
                COL_l_SwapRecords(rec, cand);
            }
            else if (candLen > (double)bestLenF)
            {
                bestLenF = candLen;
                COL_l_SwapRecords(rec, cand);
            }
            cand->flags |= 0x1000u;
        }
        COL_NeedRecomputation(colglobal, instGao, 64);
    }
}

void COL_ComputeReports(tColGlobal* colglobal, int firstRecord)
{
    tWorld* world;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;
    tGameObjectMain* gao;
    tColInstance* colInstance;
    int gaoFlags;
    Mtx4x4* gaoMtx;
    tGameObjectBasic* basic;
    tGaoMatrix* gaoHieMtx;
    tGameObjectMain* refGao;
    Mtx4x4* refAbsMtx;
    int16_t colFlag;
    tGaoMatrix* posMtx;
    Vec3* pos;
    Mtx4x4* colInstMtx;
    int anyDeferred;
    int sawPending;
    Vec3 move;
    Mtx4x4 refInvMtx;

    // Collision records dump testcode - to be removed
    /*
    tGameObjectMain* sally = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0x71003f5a);
    tGameObjectMain* mammagodecal = (tGameObjectMain*)AI_ul_RealResolveOneRefObject(0xf3000404);
    tRecordElem* re;
    int i;

    if (sally != (tGameObjectMain*)-1) {
        DBG_OutputDebugString("record cnt: %d\n", colglobal->colWorld->recordCount);
        for (i = 0; i < colglobal->colWorld->recordCount; i++) {
            re = &colglobal->colWorld->records[i];
            if (re->collisionInstanceGameObject == sally) {
                DBG_OutputDebugString("%04d. coll flags %08x, %08p %s - ", i, re->flags, re->collisionMapGameObject, (re->collisionMapGameObject == mammagodecal ? "mammagodecal" : "non-mammagodecal"));
                DBG_DumpVec(&re->collidedPoint);
            }
        }
        DBG_OutputDebugString("--\n");
    }
    */
    // hack

    //return;

    sawPending = 0;
    if (colglobal->num == colglobal->firstRecordToRecompute)
    {
        end = &colglobal->colWorld->records[colglobal->colWorld->recordCount];
    }
    else
    {
        anyDeferred = 0;
        COL_PreComputeReports(colglobal);
        world = colglobal->colWorld;
        if (colglobal->recomputeNeeded)
        {
            rec = &world->records[colglobal->firstRecordToRecompute];
            end = &world->records[world->recordCount];
        }
        else
        {
            rec = &world->records[firstRecord];
            end = &rec[world->recordCount];
        }
        if (rec < end)
        {
            do
            {
                flags = rec->flags;
                if ((flags & 0x800F3000) == 0)
                {
                    gao = rec->collisionInstanceGameObject;
                    colInstance = rec->collisionInstanceGameObject->extended->coll->colInstance;
                    gaoFlags = rec->collisionInstanceGameObject->flags;
                    if ((gaoFlags & 0x400000) != 0)
                    {
                        anyDeferred = 1;
                    }
                    else
                    {
                        if ((flags & 0x40) != 0 && (gaoFlags & 4) != 0)
                            gao->basic->dyn->flags |= 0x400000u;
                        if (!colInstance || colInstance->flag >= 0)
                        {
                            gaoMtx = gao->matrices;
                            gaoMtx->Pos.x = rec->recalVec.x + gaoMtx->Pos.x;
                            gaoMtx->Pos.y = rec->recalVec.y + gaoMtx->Pos.y;
                            gaoMtx->Pos.z = rec->recalVec.z + gaoMtx->Pos.z;
                            ANI_UpdateFlashAfterMagicBoxTranslation(gao, &rec->recalVec, 3);
                        }
                    }
                }
                ++rec;
            } while (rec < end);
            if (anyDeferred)
            {
                if (colglobal->recomputeNeeded)
                    rec = &colglobal->colWorld->records[colglobal->firstRecordToRecompute];
                else
                    rec = colglobal->colWorld->records;
                for (; rec < end; ++rec)
                {
                    flags = rec->flags;
                    if ((flags & 0x800F3000) == 0)
                    {
                        gao = rec->collisionInstanceGameObject;
                        colInstance = rec->collisionInstanceGameObject->extended->coll->colInstance;
                        gaoFlags = rec->collisionInstanceGameObject->flags;
                        if ((gaoFlags & 4) != 0 && (!colInstance || (colInstance->flag & 0x8000) == 0) && (gaoFlags & 0x400000) != 0)
                        {
                            if ((flags & 0x40) != 0)
                                gao->basic->dyn->flags |= 0x400000u;
                            basic = gao->basic;
                            gaoHieMtx = basic->mtx;
                            if (basic && gaoHieMtx)
                                refGao = gaoHieMtx->refGao2;
                            else
                                refGao = 0;
                            refAbsMtx = OBJ_pst_GetAbsoluteMatrix(refGao);
                            MATH_InvertMatrix(&refInvMtx, refAbsMtx);
                            MATH_TransformVector(&move, &refInvMtx, &rec->recalVec);
                            gaoHieMtx->mtx.Pos.x = move.x + gaoHieMtx->mtx.Pos.x;
                            gaoHieMtx->mtx.Pos.y = move.y + gaoHieMtx->mtx.Pos.y;
                            gaoHieMtx->mtx.Pos.z = move.z + gaoHieMtx->mtx.Pos.z;
                            OBJ_ComputeGlobalWhenHie(gao);
                            ANI_UpdateFlashAfterMagicBoxTranslation(gao, &move, 3);
                        }
                    }
                }
                        }
        }
    }
    if (colglobal->gaosNeededToBeRecomputedNum && !COL_gb_LastLoop)
        return;
    rec = &colglobal->colWorld->records[firstRecord];
    if (rec >= end)
        return;
    for (; rec < end; ++rec)
    {
        flags = rec->flags;
        if ((flags & 0x80043000) != 0)
            continue;
        gao = rec->collisionInstanceGameObject;
        colInstance = rec->collisionInstanceGameObject->extended->coll->colInstance;
        if (colInstance && colInstance->flag < 0)
            sawPending = 1;
        if ((flags & 0x850) != 0)
            continue;
        if (gao == WOR_gpst_CurrentWorld->viewPtr->assignedGao)
            continue;
        if (!colInstance)
            continue;
        if (!colInstance->wallPush)
            continue;
        colFlag = colInstance->flag;
        if ((colFlag & 0x8000) != 0)
            continue;
        if ((gao->flags & 0x400000) != 0)
            posMtx = gao->basic->mtx;
        else
            posMtx = (tGaoMatrix*)gao->matrices;
        pos = &posMtx->mtx.Pos;
        if ((colFlag & 2) != 0)
        {
            if ((flags & 0x10000) == 0)
                continue;
            move.x = rec->normalVec.x * -0.0099999998f;
            move.y = rec->normalVec.y * -0.0099999998f;
        }
        else
        {
            if ((flags & 0x800) != 0)
                continue;
            move.x = rec->normalVec.x * -0.0020000001f;
            move.y = rec->normalVec.y * -0.0020000001f;
        }
        pos->x = move.x + pos->x;
        pos->y = move.y + pos->y;
        move.z = 0.0;
        ANI_UpdateFlashAfterMagicBoxTranslation(gao, &move, 3);
    }
    if (!sawPending)
        return;
    rec = &colglobal->colWorld->records[firstRecord];
    if (rec >= end)
        return;
    do
    {
        if ((rec->flags & 0x80043000) == 0)
        {
            gao = rec->collisionInstanceGameObject;
            gaoFlags = rec->collisionInstanceGameObject->flags;
            colInstance = rec->collisionInstanceGameObject->extended->coll->colInstance;
            if ((gaoFlags & 4) != 0 && colInstance && colInstance->flag < 0)
            {
                if ((gaoFlags & 0x400000) != 0)
                    posMtx = gao->basic->mtx;
                else
                    posMtx = (tGaoMatrix*)gao->matrices;
                colInstMtx = colInstance->mtx;
                pos = &posMtx->mtx.Pos;
                move.x = colInstMtx->Pos.x - pos->x;
                move.y = colInstMtx->Pos.y - pos->y;
                move.z = colInstMtx->Pos.z - pos->z;
                pos->x = move.x + pos->x;
                pos->y = move.y + pos->y;
                pos->z = move.z + pos->z;
                if ((gao->flags & 0x400000) != 0)
                    OBJ_ComputeGlobalWhenHie(gao);
                ANI_UpdateFlashAfterMagicBoxTranslation(gao, &move, 3);
                colInstance->flag &= ~0x8000u; //HIBYTE(v32->flag) &= ~0x80u;
            }
        }
        ++rec;
    } while (rec < end);
}

void COL_OneCall(struct sGameObjectMain* gao, int firstRecord)
{
    int flags2;
    tGameObjectCollMain* coll;
    tColInstance* colInstance;
    uint8_t zoneNum;
    tGameObjectBasic* basic;
    tDyn* dyn;
    Mtx4x4* colInstMtx;
    long double deltaX;
    double deltaY;
    tColInstance* instance;
    tGameObjectMain* recomputed;
    tGameObjectBasic* recBasic;
    tDyn* recDyn;
    int pass;
    uint8_t pending;
    int reachedLimit;
    int n;
    int idx;
    tColInstanceElem** elems;
    int passSaved;
    int pendingSaved;
    tWorld* world;
    float forceX;
    float forceY;
    float deltaYf;
    Mtx4x4 hieMtx;

    COL_gst_GlobalVars.COL_gb_SpecialCamera = 1;
    if (OBJ_BV_IsAABBox(gao->boundingVolume))
        COL_l_ExpandBVByColInstanceOffset(gao);
    COL_gb_LastLoop = 0;
    COL_gst_GlobalVars.COL_uc_RecomputingLoop = 0;
    COL_gst_GlobalVars.firstRecordToRecompute = COL_gst_GlobalVars.num;
    COL_gst_GlobalVars.recomputeNeeded = 1;
    flags2 = gao->flags2;
    if ((flags2 & 1) == 0)
        return;
    if ((flags2 & 0x400000) != 0)
        return;
    COL_gst_GlobalVars.COL_gb_AfterEngine = 1;
    world = WOR_gpst_CurrentWorld;
    INT_SnP_InsertionSort(WOR_gpst_CurrentWorld->snp, 0);
    if ((gao->flags2 & 0x20) == 0)
        return;
    coll = gao->extended->coll;
    colInstance = coll->colInstance;
    zoneNum = colInstance->zoneNum;
    if (!zoneNum)
        return;
    elems = colInstance->selectedInstElemPtrs;
    for (idx = 0; idx < zoneNum; ++idx)
    {
        if ((elems[idx]->colFlags & 1) != 0
            && COL_get_activation_flag_for_id__max_16_sub_4132C0(coll->colInstance, (char)idx))
            break;
    }
    if (idx >= zoneNum)
        return;
    if (!coll->detectionList->overlapNum)
        return;
    coll->colInstance->flag = 0;
    basic = gao->basic;
    if (basic && (dyn = basic->dyn) != 0)
        COL_gst_GlobalVars.maxStepSize = dyn->maxStepSize;
    else
        COL_gst_GlobalVars.maxStepSize = -1.0;
    if ((gao->flags & 4) != 0)
    {
        basic = gao->basic;
        if (basic)
            dyn = basic->dyn;
        else
            dyn = 0;
        forceX = dyn->mass * dyn->forceVector.x;
        forceY = dyn->forceVector.y * dyn->mass;
        if ((gao->flags & 0x400000) != 0)
        {
            MATH_MulMatrixMatrix(&hieMtx, coll->colInstance->mtx, basic->mtx->refGao2->matrices);
            colInstMtx = gao->matrices;
            deltaX = hieMtx.Pos.x - colInstMtx->Pos.x;
            deltaY = hieMtx.Pos.y - colInstMtx->Pos.y;
        }
        else
        {
            colInstMtx = coll->colInstance->mtx;
            deltaX = colInstMtx->Pos.x - gao->matrices->Pos.x;
            deltaY = colInstMtx->Pos.y - gao->matrices->Pos.y;
        }
        if ((gao->basic->dyn->flags & 0x400000) != 0
            && fabs(forceX) < 0.001
            && fabs(forceY) < 0.001
            && fabs(deltaX) < 0.001
            && (deltaYf = (float)deltaY, fabs(deltaYf) < 0.001))
        {
            instance = gao->extended->coll->colInstance;
            instance->flag |= 0x8000u; //HIBYTE(v25->flag) |= 0x80;
        }
        else
        {
            instance = gao->extended->coll->colInstance;
            instance->flag &= ~0x8000u; //HIBYTE(v25->flag) &= ~0x80u;
        }
        gao->basic->dyn->flags &= ~0x800000u;
        gao->basic->dyn->flags &= ~0x400000u;
    }
    if ((gao->flags & 0x400000) != 0)
        OBJ_ComputeGlobalWhenHie(gao);
    COL_l_CollideWithDetectionList(gao);
    COL_ComputeReports(&COL_gst_GlobalVars, firstRecord);
    pending = COL_gst_GlobalVars.gaosNeededToBeRecomputedNum;
    pass = 0;
    if (COL_gst_GlobalVars.gaosNeededToBeRecomputedNum)
    {
        while (1)
        {
            reachedLimit = pass == 6;
            if (pass >= 6)
                break;
            passSaved = ++pass;
            COL_gst_GlobalVars.COL_uc_RecomputingLoop = pass;
            COL_gb_LastLoop = pass == 6;
            COL_gst_GlobalVars.firstRecordToRecompute = COL_gst_GlobalVars.num;
            pendingSaved = pending;
            COL_gst_GlobalVars.gaosNeededToBeRecomputedNum = 0;
            n = 0;
            if (pending)
            {
                do
                {
                    recomputed = COL_gst_GlobalVars.gaosNeededToBeRecomputed[n];
                    recomputed->extended->coll->colInstance->flag &= ~4u; //LOBYTE(v34->extended->coll->colInstance->flag) &= ~4u;
                    recomputed->extended->coll->colInstance->flag &= ~0x3F0u;
                    recBasic = recomputed->basic;
                    if (recBasic && (recDyn = recBasic->dyn) != 0)
                        COL_gst_GlobalVars.maxStepSize = recDyn->maxStepSize;
                    else
                        COL_gst_GlobalVars.maxStepSize = -1.0;
                    COL_l_CollideWithDetectionList(recomputed);
                    ++n;
                } while (n < pendingSaved);
                pass = passSaved;
            }
            COL_ComputeReports(&COL_gst_GlobalVars, firstRecord);
            pending = COL_gst_GlobalVars.gaosNeededToBeRecomputedNum;
            if (!COL_gst_GlobalVars.gaosNeededToBeRecomputedNum)
            {
                reachedLimit = pass == 6;
                break;
            }
        }
        if (reachedLimit)
            COL_l_ApplyRecomputedMoves(pending);
    }
    COL_UpdateOldMatrix(world);
    COL_gst_GlobalVars.COL_gb_SpecialCamera = 0;
}

void COL_ComputeMove(tGameObjectMain* gao, Vec3* move)
{
    Mtx4x4* gaoMtx;
    Mtx4x4* colInstMtx;
    Mtx4x4 hieMtx;

    if ((gao->flags & 0x400000) != 0)
    {
        MATH_MulMatrixMatrix(&hieMtx, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
        gaoMtx = gao->matrices;
        move->x = hieMtx.Pos.x - gaoMtx->Pos.x;
        move->y = hieMtx.Pos.y - gaoMtx->Pos.y;
        move->z = hieMtx.Pos.z - gaoMtx->Pos.z;
    }
    else
    {
        gaoMtx = gao->matrices;
        colInstMtx = gao->extended->coll->colInstance->mtx;
        move->x = colInstMtx->Pos.x - gaoMtx->Pos.x;
        move->y = colInstMtx->Pos.y - gaoMtx->Pos.y;
        move->z = colInstMtx->Pos.z - gaoMtx->Pos.z;
    }
}

void COL_DynamicHierarchisation(tGameObjectMain* gao, tGameObjectMain* refGao)
{
    int flags;
    Mtx4x4* colInstMtx;
    Mtx4x4 refInvMtx;
    Mtx4x4 localMtx;

    if (gao)
        flags = gao->flags;
    else
        flags = 0;
    OBJ_ChangeIdentityFlags(gao, flags | 0x400000, flags);
    MATH_InvertMatrix(&refInvMtx, refGao->matrices);
    MATH_MulMatrixMatrix(&gao->basic->mtx->mtx, gao->matrices, &refInvMtx);
    MATH_MulMatrixMatrix(&localMtx, gao->extended->coll->colInstance->mtx, &refInvMtx);
    colInstMtx = gao->extended->coll->colInstance->mtx;
    if (colInstMtx != &localMtx)
        memcpy(colInstMtx, &localMtx, sizeof(Mtx4x4));
    gao->basic->mtx->refGao = 0;
    gao->basic->mtx->refGao2 = refGao;
    if ((gao->flags & 0x4000000) != 0)
        ANI_ApplyHierarchyOnFlash(gao);
}

int COL_b_MoveReports(tGameObjectMain* gao, unsigned int* cursor)
{
    unsigned int startIdx;
    tRecordElem* rec;
    int hierarchised;
    int flags;
    tColInstance* colInstance;
    tGameObjectMain* mapGao;
    tGameObjectExtended* mapExt;
    tGameObjectCollMain* mapColl;
    tColMap* mapColMap;
    tColCollisionObject** mapObjs;
    tGameObjectMain* refGao;
    tGameObjectExtended* refExt;
    tGameObjectCollMain* refColl;
    tColInstance* refColInstance;
    tRecordElem* dst;
    Vec3 delta;
    tRecordElem* end;
    int moved;
    unsigned int idx;

    startIdx = *cursor;
    rec = &COL_gst_GlobalVars.colWorld->records[*cursor];
    hierarchised = gao->flags & 0x400000;
    moved = 0;
    end = &COL_gst_GlobalVars.colWorld->records[COL_gst_GlobalVars.num];
    idx = startIdx;
    for (; rec < end; ++rec, ++idx)
    {
        if (rec->collisionInstanceGameObject != gao
            || rec->collisionMapGameObject && MEM_b_CheckBlockHasBeenFreed(rec->collisionMapGameObject))
        {
            continue;
        }
        flags = rec->flags;
        if ((flags & 0x1000) != 0)
            continue;
        if ((flags & 0x10000) != 0)
        {
            colInstance = gao->extended->coll->colInstance;
            if (colInstance)
                colInstance->flag |= 2u; //LOBYTE(v8->flag) |= 2u;
        }
        mapGao = rec->collisionMapGameObject;
        if (mapGao
            && (mapGao->flags & 0x100) != 0
            && (mapExt = mapGao->extended) != 0
            && (mapColl = mapExt->coll) != 0
            && (mapColMap = mapColl->colMap) != 0)
        {
            mapObjs = mapColMap->collisionObjectPtr;
            if ((*mapObjs)->objectType == 5 && ((*mapObjs)->flags & 0x20) != 0)
            {
                *cursor = startIdx;
                return 0;
            }
        }
        if (!hierarchised && (rec->flags & 0x40) != 0 && mapGao && (mapGao->flags & 4) != 0)
        {
            COL_DynamicHierarchisation(gao, rec->collisionMapGameObject);
            hierarchised = 1;
        }
        refGao = rec->collisionMapGameObject;
        if (!refGao
            || (refExt = refGao->extended) == 0
            || (refColl = refExt->coll) == 0
            || (refColInstance = refColl->colInstance) == 0
            || get_reference_point_gao_2_from_gao_sub_40BEA0(rec->collisionInstanceGameObject) == refGao
            || (MATH_SubstractVec(&delta, &refColInstance->mtx->Pos, &refGao->matrices->Pos), fabs(delta.x) < 0.0000099999997)
            && fabs(delta.y) < 0.0000099999997)
        {
            if (idx > *cursor)
            {
                dst = &COL_gst_GlobalVars.colWorld->records[*cursor];
                COL_l_SwapRecords(dst, rec);
            }
            ++*cursor;
            moved = 1;
        }
    }
    return moved;
}

void COL_UpdateBV(tableDescStruct* table)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;

    elem = table->firstElemPtr;
    for (last = ((tableElemStructPF*)(table->nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr
            && ((uint8_t)gao & 3) != 1
            && (gao->flags & 0x200) != 0
            && !OBJ_BV_IsSphere(gao->boundingVolume))
        {
            COL_l_ExpandBVByColInstanceOffset(gao);
        }
    }
}

static float COL_l_CobWaveAmplitude(tOtcMdfStruct* otc, const uint8_t* weight, int invertWeight)
{
    if (!weight)
        return otc->intensity;
    if (invertWeight)
        return (float)((1.0 - (double)*weight * 0.00390625) * otc->intensity);
    return (float)((double)*weight * otc->intensity * 0.00390625);
}

static void COL_l_CobWave(tOtcMdfStruct* otc, Vec3* verts, unsigned int num,
                          const uint8_t* weight, int invertWeight,
                          int dstA, int srcA, int dstB, int srcB,
                          int secondIsCos)
{
    unsigned int i;
    float amp;
    float* v;

    for (i = 0; i < num; i++)
    {
        amp = COL_l_CobWaveAmplitude(otc, weight, invertWeight);
        v = (float*)&verts[i];
        v[dstA] = (float)(MATH_GetSin(v[srcA] * otc->speed + otc->offset) * amp + v[dstA]);
        if (secondIsCos)
            v[dstB] = MATH_GetCos(v[srcB] * otc->speed + otc->offset) * amp + v[dstB];
        else
            v[dstB] = (float)(MATH_GetSin(v[srcB] * otc->speed + otc->offset) * amp + v[dstB]);
        if (weight)
            weight += 4;
    }
}

static void COL_l_CobWaveZFromXY(tOtcMdfStruct* otc, Vec3* verts, unsigned int num)
{
    unsigned int i;
    double atOffset;
    float amp;

    atOffset = MATH_GetSin(otc->offset);
    amp = (float)(atOffset * otc->intensity + atOffset * otc->intensity);
    for (i = 0; i < num; i++)
    {
        verts[i].z = (float)((MATH_GetSin(otc->speed * verts[i].x)
                              + MATH_GetSin(verts[i].y * otc->speed)) * amp + verts[i].z);
    }
}

void COL_UpdateCobFromVisualRealTime(tGameObjectMain* gao, tColCollisionObject* colObj, tGEOGRO* geogro)
{
    tCobMeshObjectMesh* mesh;
    tModifier* mdf;
    tOtcMdfStruct* otc;
    Vec3* verts;
    unsigned int num;
    int flags;
    int* rliColors;
    const uint8_t* weight;
    int invertWeight;
    tCobMeshChunkElem* chunk;
    tCobMeshChunkElem* chunkEnd;
    int faceBase;
    tCobMeshTriangleElem* tri;
    tCobMeshTriangleElem* triEnd;
    int face;
    Vec3* p0;
    Vec3* p1;
    Vec3* p2;
    double e1x;
    double e1y;
    double e1z;
    double e2z;
    Vec3* dst;
    long double invLen;
    float e2x;
    float e2y;
    Vec3 normal;

    if (!colObj || !geogro || colObj->objectType != 5)
        return;
    mesh = colObj->objectPtrByType;
    if (mesh->vertexCount != geogro->vertexNum)
    {
        colObj->flags &= ~0x40u;
        return;
    }
    if ((gao->flags & 0x2000) == 0)
        return;
    if (!gao->extended)
        return;
    PRO_PerfCounterStartBlock(&PRO_RasterGroup.PRO_COBFromVisual);
    mdf = gao->extended->mdf;
    if (!mdf)
        return;
    while ((mdf->modifierFlags & 0xF000000) != 0 || mdf->modifierIntf->mdfId != 1)// GEO_ModifierOnduleTonCorps
    {
        mdf = mdf->nextElem;
        if (!mdf)
            return;
    }
    memcpy(mesh->vertices, geogro->vertices, 12 * geogro->vertexNum);
    otc = (tOtcMdfStruct*)mdf->mdfPrivateDataPtr;
    verts = mesh->vertices;
    num = mesh->vertexCount;
    flags = otc->flags;
    rliColors = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors;
    if ((flags & 0x10) != 0 && rliColors)
    {
        weight = (const uint8_t*)(rliColors + 1) + 3;
        invertWeight = (flags & 0x20) != 0;
    }
    else
    {
        weight = 0;
        invertWeight = 0;
    }
    if ((flags & 1) != 0)
    {
        if ((flags & 8) != 0)
            COL_l_CobWave(otc, verts, num, weight, invertWeight, 0, 1, 0, 2, 0);
        else
            COL_l_CobWave(otc, verts, num, weight, invertWeight, 1, 0, 2, 0, 1);
    }
    else if ((flags & 2) != 0)
    {
        if ((flags & 8) != 0)
            COL_l_CobWave(otc, verts, num, weight, invertWeight, 1, 0, 1, 2, 0);
        else
            COL_l_CobWave(otc, verts, num, weight, invertWeight, 0, 1, 2, 1, 1);
    }
    else if ((flags & 4) != 0)
    {
        if ((flags & 8) != 0)
        {
            if (weight)
                COL_l_CobWave(otc, verts, num, weight, invertWeight, 2, 0, 2, 1, 0);
            else
                COL_l_CobWaveZFromXY(otc, verts, num);
        }
        else
        {
            COL_l_CobWave(otc, verts, num, weight, invertWeight, 0, 2, 1, 2, 1);
        }
    }
    chunk = mesh->meshChunkElems;
    chunkEnd = &chunk[mesh->meshChunkElemNum];
    faceBase = 0;
    for (; chunk < chunkEnd; ++chunk)
    {
        if (!chunk->elemNum)
            continue;
        tri = chunk->triangleElems;
        triEnd = &tri[chunk->elemNum];
        if (tri >= triEnd)
            continue;
        face = faceBase;
        faceBase += chunk->elemNum;
        do
        {
            p1 = &mesh->vertices[tri->v2];
            p0 = &mesh->vertices[tri->v1];
            p2 = &mesh->vertices[tri->v3];
            e1x = p1->x - p0->x;
            e1y = p1->y - p0->y;
            e1z = p1->z - p0->z;
            e2x = p2->x - p0->x;
            e2y = p2->y - p0->y;
            e2z = p2->z - p0->z;
            normal.x = (float)(e2z * e1y - e2y * e1z);
            normal.y = (float)(e1z * e2x - e2z * e1x);
            normal.z = (float)(e2y * e1x - e1y * e2x);
            if (fabs(normal.x) != 0 || fabs(e1z * e2x - e2z * e1x) || fabs(e2y * e1x - e1y * e2x))
            {
                dst = &mesh->normals[face];
                invLen = 1.0 / sqrt(normal.z * normal.z + normal.y * normal.y + normal.x * normal.x);
                dst->x = (float)(normal.x * invLen);
                dst->y = (float)(normal.y * invLen);
                dst->z = (float)(normal.z * invLen);
            }
            else
            {
                mesh->normals[face] = normal;
            }
            ++tri;
            ++face;
        } while (tri < triEnd);
    }
    if (PRO_RasterGroup.PRO_COBFromVisual.insideCounter)
    {
        if (!--PRO_RasterGroup.PRO_COBFromVisual.insideCounter)
            PRO_RasterGroup.PRO_COBFromVisual.sumTime += TIM_GetTicksAsInt32() - PRO_RasterGroup.PRO_COBFromVisual.startTick;
    }
}

void COL_UpdateRealTimeColMaps(tWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    int flags2;
    tColMap* colMap;
    tColCollisionObject* colObj;
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    tGEOGRO* geogro;

    elem = world->eot.collMapObjectsTablePF.firstElemPtr;
    for (last = ((tableElemStructPF*)(world->eot.collMapObjectsTablePF.nextElemPtr)) - 1; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (!elem->objectPtr)
            continue;
        if (((uint8_t)gao & 3) == 1)
            continue;
        flags2 = gao->flags2;
        if ((flags2 & 1) == 0 || (flags2 & 0x400000) != 0)
            continue;
        colMap = gao->extended->coll->colMap;
        if (!colMap || !colMap->colObjectID)
            continue;
        colObj = *colMap->collisionObjectPtr;
        if (colObj->objectType != 5 || (colObj->flags & 0x40) == 0)
            continue;
        basic = gao->basic;
        if (!basic)
            continue;
        visual = basic->actorObjectOrVisualObject;
        if (!visual)
            continue;
        geogro = (tGEOGRO*)(visual->GROGEOOrSomethingElse);
        if (geogro)
            COL_UpdateCobFromVisualRealTime(gao, colObj, geogro);
    }
}

void COL_Compute(tWorld* world)
{
    unsigned int recordCount;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* colInstGao;
    int flags2;
    tGameObjectCollMain* coll;
    tColInstance* colInstance;
    uint8_t zoneNum;
    int isDynamic;
    tGameObjectBasic* basic;
    tDyn* dyn;
    double mass;
    int dynFlags;
    int16_t colFlag;
    tGameObjectMain** mapGao;
    tGameObjectMain** mapGaoEnd;
    tGameObjectMain* other;
    int otherFlags;
    int otherFlags2;
    int idx;
    Vec3 hierMove;
    Vec3 move;
    float forceX;
    float forceY;
    float forceZ;

    COL_gst_GlobalVars.gaosNeededToBeRecomputedNum = 0;
    COL_gst_GlobalVars.firstRecordToRecompute = 0;
    COL_gst_GlobalVars.recomputeNeeded = 0;
    COL_gst_GlobalVars.colWorld = world;
    elem = world->eot.collInstanceZDMGameObjectsPF.firstElemPtr;
    last = ((tableElemStructPF*)(world->eot.collInstanceZDMGameObjectsPF.nextElemPtr)) - 1;
    recordCount = 0;
    for (; elem <= last; ++elem)
    {
        colInstGao = (tGameObjectMain*)(elem->objectPtr);
        if (!elem->objectPtr)
            continue;
        if (((uint8_t)colInstGao & 3) == 1)
            continue;
        flags2 = colInstGao->flags2;
        if ((flags2 & 1) == 0
            || (flags2 & 0x400000) != 0
            || (flags2 & 0x20) == 0
            || (colInstGao->flags & 4) != 0 && (colInstGao->basic->dyn->flags & 0x20000000) != 0)
        {
            continue;
        }
        coll = colInstGao->extended->coll;
        if (!coll->detectionList->overlapNum)
            continue;
        coll->colInstance->flag = 0;
        colInstance = coll->colInstance;
        zoneNum = colInstance->zoneNum;
        if (!zoneNum)
            continue;
        for (idx = 0; idx < zoneNum; ++idx)
        {
            if ((colInstance->selectedInstElemPtrs[idx]->colFlags & 1) != 0
                && COL_get_activation_flag_for_id__max_16_sub_4132C0(colInstance, (char)idx))
                break;
        }
        if (idx >= zoneNum)
            continue;
        colInstGao->extended->coll->colInstance->flag = 1;
        COL_ComputeMove(colInstGao, &move);
        isDynamic = colInstGao->flags & 4;
        if (!isDynamic || (colInstGao->basic->dyn->flags & 0x800000) == 0)
        {
            if (fabs(move.x) < 0.0000099999997 && fabs(move.y) < 0.0000099999997 && fabs(move.z) < 0.0000099999997)
            {
                colInstance = colInstGao->extended->coll->colInstance;
                colInstance->flag |= 4u; //LOBYTE(v12->flag) |= 4u;
                continue;
            }
            if (!isDynamic)
                continue;
        }
        basic = colInstGao->basic;
        if (basic)
            dyn = basic->dyn;
        else
            dyn = 0;
        mass = dyn->mass;
        forceX = (float)(mass * dyn->forceVector.x);
        forceY = (float)(dyn->forceVector.y * mass);
        forceZ = (float)(dyn->forceVector.z * mass);
        COL_ComputeMove(colInstGao, &hierMove);
        dynFlags = colInstGao->basic->dyn->flags;
        if ((dynFlags & 0x800000) == 0
            && (dynFlags & 0x400000) != 0
            && fabs(forceX) < 0.0000099999997
            && fabs(forceY) < 0.0000099999997
            && forceZ < 0.0)
        {
            if (COL_b_MoveReports(colInstGao, &recordCount))
                colInstGao->extended->coll->colInstance->flag |= 0x8000;
            else
                colInstGao->extended->coll->colInstance->flag &= ~0x8000;
        }
    }
    COL_gst_GlobalVars.num = recordCount;
    COL_gst_GlobalVars.colWorld->recordCount = recordCount;
    elem = world->eot.collInstanceZDMGameObjectsPF.firstElemPtr;
    last = ((tableElemStructPF*)(world->eot.collInstanceZDMGameObjectsPF.nextElemPtr)) - 1;
    for (; elem <= last; ++elem)
    {
        colInstGao = (tGameObjectMain*)(elem->objectPtr);
        if (!elem->objectPtr)
            continue;
        if (((uint8_t)colInstGao & 3) == 1)
            continue;
        flags2 = colInstGao->flags2;
        if ((flags2 & 1) == 0
            || (flags2 & 0x400000) != 0
            || (flags2 & 0x20) == 0
            || (colInstGao->flags & 4) != 0 && (colInstGao->basic->dyn->flags & 0x20000000) != 0)
        {
            continue;
        }
        coll = colInstGao->extended->coll;
        if (!coll->detectionList->overlapNum)
            continue;
        colInstance = coll->colInstance;
        colFlag = colInstance->flag;
        if ((colFlag & 1) == 0)
            continue;
        colInstance->flag = colFlag & 0xFFFE;
        colFlag = colInstGao->extended->coll->colInstance->flag;
        if ((colFlag & 4) != 0 || colFlag < 0)
            continue;
        basic = colInstGao->basic;
        if (basic && (dyn = basic->dyn) != 0)
            COL_gst_GlobalVars.maxStepSize = dyn->maxStepSize;
        else
            COL_gst_GlobalVars.maxStepSize = -1.0;
        if ((colInstGao->flags & 0x400000) != 0)
            OBJ_ComputeGlobalWhenHie(colInstGao);
        if ((colInstGao->flags & 4) != 0)
        {
            colInstGao->basic->dyn->flags &= ~0x800000u;
            colInstGao->basic->dyn->flags &= ~0x400000u;
        }
        mapGao = coll->detectionList->detectionListGaos;
        for (mapGaoEnd = &mapGao[coll->detectionList->overlapNum - 1]; mapGao <= mapGaoEnd; ++mapGao)
        {
            other = *mapGao;
            otherFlags = (*mapGao)->flags;
            if ((otherFlags & 0x100) != 0)
            {
                otherFlags2 = other->flags2;
                if ((otherFlags2 & 1) != 0
                    && (otherFlags2 & 0x400000) == 0
                    && ((otherFlags & 0x400000) == 0 || other->basic->mtx->refGao2 != colInstGao))
                {
                    COL_InitGlobalVars(colInstGao, *mapGao);
                    COL_Actor_GameObject(&COL_gst_GlobalVars);
                }
            }
        }
    }
}

void COL_MainCall(struct sWorld* world)
{
    uint8_t pending;
    int pass;
    int reachedLimit;
    int n;
    tGameObjectMain* recomputed;
    tColInstance* colInstance;
    tGameObjectBasic* basic;
    tDyn* dyn;

    COL_gb_LastLoop = 0;
    COL_gst_GlobalVars.COL_uc_RecomputingLoop = 0;
    COL_gst_GlobalVars.COL_gb_AfterEngine = 0;
    COL_UpdateBV(&world->eot.collInstanceZDMGameObjectsPF);
    INT_SnP_InsertionSort(world->snp, 0);
    COL_ul_SnP_RefreshType = (COL_ul_SnP_RefreshType == 1) + 1;
    COL_UpdateRealTimeColMaps(world);
    COL_Compute(world);
    if (COL_gst_GlobalVars.num)
        COL_ComputeReports(&COL_gst_GlobalVars, 0);
    pending = COL_gst_GlobalVars.gaosNeededToBeRecomputedNum;
    pass = 0;
    if (COL_gst_GlobalVars.gaosNeededToBeRecomputedNum)
    {
        while (1)
        {
            reachedLimit = pass == 6;
            if (pass >= 6)
                break;
            ++pass;
            COL_gst_GlobalVars.firstRecordToRecompute = COL_gst_GlobalVars.num;
            COL_gst_GlobalVars.COL_uc_RecomputingLoop = pass;
            COL_gb_LastLoop = pass == 6;
            COL_gst_GlobalVars.recomputeNeeded = 1;
            COL_gst_GlobalVars.gaosNeededToBeRecomputedNum = 0;
            for (n = 0; n < pending; ++n)
            {
                recomputed = COL_gst_GlobalVars.gaosNeededToBeRecomputed[n];
                colInstance = recomputed->extended->coll->colInstance;
                colInstance->flag &= ~4u; //LOBYTE(v8->flag) &= ~4u;
                colInstance = recomputed->extended->coll->colInstance;
                colInstance->flag &= ~0x3F0u;
                basic = recomputed->basic;
                if (basic && (dyn = basic->dyn) != 0)
                    COL_gst_GlobalVars.maxStepSize = dyn->maxStepSize;
                else
                    COL_gst_GlobalVars.maxStepSize = -1.0;
                COL_l_CollideWithDetectionList(recomputed);
            }
            COL_ComputeReports(&COL_gst_GlobalVars, 0);
            pending = COL_gst_GlobalVars.gaosNeededToBeRecomputedNum;
            if (!COL_gst_GlobalVars.gaosNeededToBeRecomputedNum)
            {
                reachedLimit = pass == 6;
                break;
            }
        }
        if (reachedLimit)
            COL_l_ApplyRecomputedMoves(pending);
    }
    COL_UpdateOldMatrix(world);
}

void REC_OrderReports(tWorld* world)
{
    tRecordElem* rec;
    tRecordElem* end;
    tGameObjectBasic* basic;
    tDyn* dyn;
    double mass;
    tRecordElem* cand;
    double bestDot;
    double dot;
    tRecordElem* best;
    float dotF;
    float speedX;
    float speedY;
    float speedZ;

    rec = world->records;
    end = &world->records[world->recordCount];
    for (; rec < end; ++rec)
    {
        if ((rec->flags & 0x80003000) != 0)
            continue;
        basic = rec->collisionInstanceGameObject->basic;
        if (!basic)
            continue;
        dyn = basic->dyn;
        if (!dyn)
            continue;
        mass = dyn->mass;
        best = 0;
        speedX = (float)(mass * dyn->forceVector.x);
        speedY = (float)(dyn->forceVector.y * mass);
        speedZ = (float)(dyn->forceVector.z * mass);
        bestDot = rec->normalVec.z * speedZ + speedX * rec->normalVec.x + speedY * rec->normalVec.y - 0.001;
        for (cand = rec + 1; cand < end; ++cand)
        {
            if ((cand->flags & 0x80003000) == 0
                && cand->collisionInstanceGameObject == rec->collisionInstanceGameObject)
            {
                dot = cand->normalVec.z * speedZ + speedX * cand->normalVec.x + speedY * cand->normalVec.y;
                if (dot < bestDot)
                {
                    best = cand;
                    dotF = (float)dot;
                    bestDot = dotF - 0.001;
                }
            }
        }
        if (best)
        {
            COL_l_SwapRecords(rec, best);
        }
    }
}

void REC_PhysicalCall(tGameObjectMain* gao, tWorld* world)
{
    tDyn* dyn;
    tDynSolid* solid;
    double mass;
    tColBox* box;
    double sumX;
    double sumY;
    tRecordElem* rec;
    tRecordElem* end;
    double armX;
    double spinX;
    double spinY;
    double torqueY;
    double torqueZ;
    double impulse;
    Vec3* spin;
    double impTorqueY;
    double impTorqueZ;
    Mtx4x4* gaoMtx;
    Vec3* oldPos;
    float armY;
    float armZ;
    Vec3 localNormal;
    float spinZ;
    float velX;
    Vec3 impulseVec;
    float massA;
    float massB;
    float centerZ;
    float impCrossZ;
    float impTorqueX;
    float torqueCrossX;
    float torqueCrossY;
    float torqueX;
    Vec3 speedVec;
    Vec3 worldImpulse;
    Vec3 localSpeed;
    Vec3 localPoint;
    float crossZ;
    float centerX;
    float centerY;
    Mtx4x4 gaoInvMtx;

    dyn = gao->basic->dyn;
    if (!dyn || !dyn->solid)
        return;
    MATH_InvertMatrix(&gaoInvMtx, gao->matrices);
    solid = dyn->solid;
    solid->torqueVector.x = 0.0;
    solid->torqueVector.y = 0.0;
    solid->torqueVector.z = 0.0;
    dyn->solid->rotSpeedVector.x = (float)(dyn->solid->rotSpeedVector.x * 0.94999999);
    dyn->solid->rotSpeedVector.y = (float)(dyn->solid->rotSpeedVector.y * 0.94999999);
    dyn->solid->rotSpeedVector.z = (float)(dyn->solid->rotSpeedVector.z * 0.94999999);
    mass = dyn->mass;
    speedVec.x = (float)(mass * dyn->forceVector.x);
    speedVec.y = (float)(dyn->forceVector.y * mass);
    speedVec.z = (float)(dyn->forceVector.z * mass);
    MATH_TransformVector(&localSpeed, &gaoInvMtx, &speedVec);
    box = (tColBox*)((*gao->extended->coll->colInstance->selectedInstElemPtrs)->elemShapePtr); // TODO ez jó így?
    sumX = box->pos2.x + box->pos1.x;
    sumY = box->pos2.y + box->pos1.y;
    end = &world->records[world->recordCount];
    centerZ = box->pos2.z + box->pos1.z;
    centerX = (float)(sumX * 0.5);
    rec = world->records;
    centerY = (float)(sumY * 0.5);
    centerZ = (float)(centerZ * 0.5);
    for (; rec < end; ++rec)
    {
        if ((rec->flags & 0x80043800) == 0
            && rec->collisionInstanceGameObject == gao
            && (rec->recalVec.x != 0.0 || rec->recalVec.y != 0.0 || rec->recalVec.z != 0.0))
        {
            MATH_TransformVertex(&localPoint, &gaoInvMtx, &rec->collidedPoint);
            MATH_TransformVector(&localNormal, &gaoInvMtx, &rec->normalVec);
            armX = localPoint.x - centerX;
            solid = dyn->solid;
            armY = localPoint.y - centerY;
            armZ = localPoint.z - centerZ;
            spinX = (float)(armZ * solid->rotSpeedVector.y - armY * solid->rotSpeedVector.z);
            spinY = (float)(armX * solid->rotSpeedVector.z - armZ * solid->rotSpeedVector.x);
            spinZ = (float)(armY * solid->rotSpeedVector.x - armX * solid->rotSpeedVector.y);
            velX = (float)(localSpeed.x + spinX);
            massA = dyn->mass;
            crossZ = (float)(localNormal.y * armX - armY * localNormal.x);
            torqueX = (localNormal.z * armY - localNormal.y * armZ) * massA;
            torqueY = (armZ * localNormal.x - localNormal.z * armX) * massA;
            torqueZ = crossZ * massA;
            torqueCrossX = (float)(torqueY * armZ - torqueZ * armY);
            torqueCrossY = (float)(torqueZ * armX - armZ * torqueX);
            impulse = -((localNormal.z * (localSpeed.z + spinZ) + localNormal.y * (localSpeed.y + spinY) + velX * localNormal.x) * (solid->elastic + 1.0))
                / ((armY * torqueX - torqueY * armX) * localNormal.z + torqueCrossY * localNormal.y + torqueCrossX * localNormal.x + dyn->mass);
            impulseVec.x = (float)(localNormal.x * impulse);
            impulseVec.y = (float)(localNormal.y * impulse);
            impulseVec.z = (float)(localNormal.z * impulse);
            spin = &dyn->solid->rotSpeedVector;
            massB = dyn->mass;
            impCrossZ = (float)(impulseVec.y * armX - armY * impulseVec.x);
            impTorqueX = (impulseVec.z * armY - impulseVec.y * armZ) * massB;
            impTorqueY = (armZ * impulseVec.x - impulseVec.z * armX) * massB;
            impTorqueZ = impCrossZ * massB;
            spin->x = impTorqueX + spin->x;
            spin->y = (float)(impTorqueY + spin->y);
            spin->z = (float)(impTorqueZ + spin->z);
            MATH_TransformVector(&worldImpulse, gao->matrices, &impulseVec);
            dyn->forceVector.x = worldImpulse.x + dyn->forceVector.x;
            dyn->forceVector.y = worldImpulse.y + dyn->forceVector.y;
            dyn->forceVector.z = worldImpulse.z + dyn->forceVector.z;
        }
    }
    gaoMtx = gao->matrices;
    oldPos = &dyn->solid->oldPos;
    oldPos->x = gaoMtx->Pos.x;
    oldPos->y = gaoMtx->Pos.y;
    oldPos->z = gaoMtx->Pos.z;
}

void REC_ApplyDynObj(Vec3* speed1, Vec3* speed2, Vec3* out1, Vec3* out2, Vec3* normal)
{
    double len1;
    double len2;
    double keep1;
    double give2;
    double out1Z;
    float nx;
    float ny;
    float nz;
    float dot1;
    float dot2;
    float normal1X;
    float normal1Y;
    float normal1Z;
    float normal2X;
    float normal2Y;
    float normal2Z;
    float tangent1X;
    float tangent1Y;
    float tangent1Z;
    float tangent2X;
    float tangent2Y;
    float tangent2Z;
    float kept1X;
    float kept1Y;
    float kept1Z;
    float given2X;
    float given2Y;
    float out1X;
    float out1Y;
    float out2X;
    float out2Y;
    float out2Z;
    float kept2Z;
    float given1Y;
    float given1Z;
    float give1;
    float keep2;

    nx = -normal->x;
    ny = -normal->y;
    nz = -normal->z;
    len1 = sqrt(speed1->x * speed1->x + speed1->y * speed1->y + speed1->z * speed1->z);
    len2 = sqrt(speed2->x * speed2->x + speed2->y * speed2->y + speed2->z * speed2->z);
    dot1 = nx * speed1->x + nz * speed1->z + ny * speed1->y;
    dot2 = nx * speed2->x + nz * speed2->z + ny * speed2->y;
    normal1X = nx * dot1;
    normal1Y = ny * dot1;
    normal1Z = nz * dot1;
    normal2X = nx * dot2;
    normal2Y = ny * dot2;
    normal2Z = nz * dot2;
    tangent1X = speed1->x - normal1X;
    tangent1Y = speed1->y - normal1Y;
    tangent1Z = speed1->z - normal1Z;
    tangent2X = speed2->x - normal2X;
    tangent2Y = speed2->y - normal2Y;
    tangent2Z = speed2->z - normal2Z;
    if (len1 <= 5.0 * len2)
    {
        if (len2 <= len1 * 5.0)
        {
            keep1 = 0.2f;
            keep2 = 0.2f;
            give2 = 0.80000001f;
            give1 = 0.80000001f;
        }
        else
        {
            keep1 = 0.1f;
            keep2 = 0.80000001f;
            give2 = 1.5f;
            give1 = 0.2f;
        }
    }
    else
    {
        keep2 = 0.1f;
        give1 = 1.5;
        keep1 = 0.80000001f;
        give2 = 0.2f;
    }
    kept1X = (float)(normal1X * keep1);
    kept1Y = (float)(normal1Y * keep1);
    kept1Z = (float)(normal1Z * keep1);
    given2X = (float)(normal2X * give2);
    given2Y = (float)(normal2Y * give2);
    out1X = given2X + kept1X;
    out1Y = given2Y + kept1Y;
    out1Z = normal2Z * give2 + kept1Z;
    kept2Z = normal2Z * keep2;
    given1Y = normal1Y * give1;
    given1Z = normal1Z * give1;
    out2X = normal1X * give1 + normal2X * keep2;
    out2Y = given1Y + normal2Y * keep2;
    out2Z = given1Z + kept2Z;
    if (dot2 * dot1 > 0.0 && dot1 <= (double)dot2)
    {
        *out1 = *speed1;
        *out2 = *speed2;
        return;
    }
    out1->x = out1X + tangent1X;
    out1->y = out1Y + tangent1Y;
    out1->z = (float)(out1Z + tangent1Z);
    out2->x = tangent2X + out2X;
    out2->y = out2Y + tangent2Y;
    out2->z = out2Z + tangent2Z;
}

void REC_ApplyDyn(tGameObjectMain* gao1, tGameObjectMain* gao2, Vec3* normal)
{
    tGameObjectBasic* basic1;
    tDyn* dyn1;
    tGameObjectBasic* basic2;
    tDyn* dyn2;
    double mass1;
    double mass2;
    double invMass1;
    double invMass2;
    Vec3 speed2;
    Vec3 speed1;
    Vec3 out1;
    Vec3 out2;

    basic1 = gao1->basic;
    if (basic1)
        dyn1 = basic1->dyn;
    else
        dyn1 = 0;
    basic2 = gao2->basic;
    if (basic2)
        dyn2 = basic2->dyn;
    else
        dyn2 = 0;
    mass1 = dyn1->mass;
    speed1.x = (float)(mass1 * dyn1->forceVector.x);
    speed1.y = (float)(dyn1->forceVector.y * mass1);
    speed1.z = (float)(dyn1->forceVector.z * mass1);
    mass2 = dyn2->mass;
    speed2.x = (float)(mass2 * dyn2->forceVector.x);
    speed2.y = (float)(dyn2->forceVector.y * mass2);
    speed2.z = (float)(dyn2->forceVector.z * mass2);
    REC_ApplyDynObj(&speed1, &speed2, &out1, &out2, normal);
    invMass1 = 1.0 / dyn1->mass;
    dyn1->forceVector.x = (float)(out1.x * invMass1);
    dyn1->forceVector.y = (float)(out1.y * invMass1);
    dyn1->forceVector.z = (float)(out1.z * invMass1);
    DYN_ApplySpeedLimit(dyn1);
    invMass2 = 1.0 / dyn2->mass;
    dyn2->forceVector.x = (float)(out2.x * invMass2);
    dyn2->forceVector.y = (float)(out2.y * invMass2);
    dyn2->forceVector.z = (float)(out2.z * invMass2);
    DYN_ApplySpeedLimit(dyn2);
}

void REC_CalcNormalForImpulse(tDyn* dyn, Vec3* outNormal, Vec3* upVec, Vec3* hitNormal, int useRebound)
{
    tDynCol* col;
    double cosLimit;
    double halfLimit;
    float blend;
    float cosAngle;

    col = dyn->collision;
    cosAngle = -(upVec->x * hitNormal->x + upVec->y * hitNormal->y + upVec->z * hitNormal->z);
    if (!col)
        cosLimit = 0.0;
    else if (useRebound)
        cosLimit = col->reboundHorizCosAngle;
    else
        cosLimit = col->slideHorizCosAngle;
    outNormal->x = -hitNormal->x;
    outNormal->y = -hitNormal->y;
    outNormal->z = -hitNormal->z;
    if (cosAngle < cosLimit)
    {
        halfLimit = (3.0 * cosLimit - 1.0) * 0.5;
        if (cosAngle >= halfLimit)
        {
            blend = (float)((cosAngle - halfLimit) / (cosLimit - halfLimit));
            MATH_BlendVector(outNormal, upVec, outNormal, blend);
        }
        else
        {
            *outNormal = *upVec;
        }
    }
}

void REC_CalcImpulse(tGameObjectMain* gao, tDyn* dyn, Vec3* outSpeed, Vec3* hitNormal, Vec3* unused, char mode, tGameObjectMain* otherGao, int forceRebound)
{
    double mass;
    tDynForces* forces;
    long double gravityLen;
    tDynCol* col;
    Vec3* slideNormal;
    Vec3* reboundNormal;
    Vec3 upVec;
    Vec3 speedVec;
    Vec3 slideVec;
    Vec3 slideBase;
    Vec3 reboundBase;
    float slide;
    float rebound;

    mass = dyn->mass;
    speedVec.x = (float)(mass * dyn->forceVector.x);
    speedVec.y = (float)(dyn->forceVector.y * mass);
    speedVec.z = (float)(dyn->forceVector.z * mass);
    if ((dyn->flags & 0x20000) == 0)
    {
        slideNormal = hitNormal;
        reboundNormal = hitNormal;
        slide = 1.0;
        rebound = 0.0;
    }
    else
    {
        forces = dyn->forces;
        if (!MATH_b_NulVector(&forces->gravityVector))
        {
            gravityLen = sqrt(
                forces->gravityVector.z * forces->gravityVector.z
                + forces->gravityVector.y * forces->gravityVector.y
                + forces->gravityVector.x * forces->gravityVector.x);
            upVec.x = (float)(1.0 / gravityLen * forces->gravityVector.x);
            upVec.y = (float)(1.0 / gravityLen * forces->gravityVector.y);
            upVec.z = (float)(1.0 / gravityLen * forces->gravityVector.z);
        }
        else
        {
            upVec.x = 0.0;
            upVec.y = 0.0;
            upVec.z = -1.0;
        }
        // preserved: both calls ask for the rebound angle, so the slide normal is computed with it too
        REC_CalcNormalForImpulse(dyn, &slideBase, hitNormal, &upVec, 1);
        REC_CalcNormalForImpulse(dyn, &reboundBase, hitNormal, &upVec, 1);
        col = dyn->collision;
        slideNormal = &slideBase;
        reboundNormal = &reboundBase;
        if (col)
            slide = col->slide;
        else
            slide = 1.0;
        if (col)
            rebound = col->rebound;
        else
            rebound = 0.0;
    }
    if (fabs(speedVec.x) <= 0.0000001//  MATH_b_NulVectorWithEpsilon
        && fabs(speedVec.y) <= 0.0000001
        && fabs(speedVec.z) <= 0.0000001)
    {
        outSpeed->x = 0.0;
        outSpeed->y = 0.0;
        outSpeed->z = 0.0;
    }
    else if (mode == 2)
    {
        DYN_CalcSlideVector(&slideVec, slideNormal, &speedVec, slide);
        if (forceRebound || dyn->flags >= 0)
        {
            DYN_CalcReboundVector(&upVec, reboundNormal, &speedVec, rebound);
        }
        else
        {
            upVec.x = 0.0;
            upVec.y = 0.0;
            upVec.z = 0.0;
        }
        if (sqrt(upVec.z * upVec.z + upVec.y * upVec.y + upVec.x * upVec.x) <= 0.0099999998)
        {
            outSpeed->x = slideVec.x;
            outSpeed->y = slideVec.y;
            outSpeed->z = slideVec.z;
        }
        else
        {
            outSpeed->x = slideVec.x + upVec.x;
            outSpeed->y = slideVec.y + upVec.y;
            outSpeed->z = slideVec.z + upVec.z;
        }
    }
    else
    {
        outSpeed->z = 0.0;
    }
}

void REC_Apply(tGameObjectMain* gao, tGameObjectMain* otherGao, Vec3* hitNormal, int forceRebound)
{
    tGameObjectBasic* basic;
    tDyn* dyn;
    long double invLen;
    double invMass;
    Vec3 normal;
    Vec3 newSpeed;

    basic = gao->basic;
    if (!basic)
        return;
    dyn = basic->dyn;
    if (!dyn)
        return;
    normal.x = hitNormal->x;
    normal.y = hitNormal->y;
    normal.z = hitNormal->z;
    if (forceRebound)
    {
        invLen = 1.0 / sqrt(normal.y * normal.y + normal.x * normal.x);// MATH_NormalizeEqualVector
        normal.x = (float)(normal.x * invLen);
        normal.y = (float)(normal.y * invLen);
        normal.z = (float)(invLen * 0.0);
    }
    REC_CalcImpulse(gao, dyn, &newSpeed, &normal, &newSpeed, 2, otherGao, forceRebound);
    invMass = 1.0 / dyn->mass;
    dyn->forceVector.x = (float)(newSpeed.x * invMass);
    dyn->forceVector.y = (float)(newSpeed.y * invMass);
    dyn->forceVector.z = (float)(newSpeed.z * invMass);
    DYN_ApplySpeedLimit(dyn);
}
void REC_MainCall(struct sWorld* a1, int a2)
{
    tRecordElem* v2;
    tRecordElem* v3;
    int v4;
    tGameObjectMain* v5;
    tGameObjectMain* v6;
    tRecordElem* v7;
    int v8;
    tGameObjectBasic* v9;
    tDyn* v10;
    tGameObjectBasic* v11;
    tDyn* v12;
    float v13;
    float v14;
    Vec3* v15;
    double v16;
    float v17;
    double v18;
    long double v19;
    Vec3 v20;
    Vec3 a3;
    tRecordElem* i;

    REC_OrderReports(a1);
    v2 = &a1->records[a1->recordCount];
    v3 = &a1->records[a2];
    for (i = v2; v3 < v2; ++v3)
    {
        v4 = v3->flags;
        if ((v4 & 0x80043800) == 0)
        {
            v5 = v3->collisionInstanceGameObject;
            v6 = v3->collisionMapGameObject;
            if ((v4 & 0x4000) != 0)
            {
                REC_PhysicalCall(v5, a1);
            }
            else
            {
                v2 = i;
                v7 = v3 + 1;
                if (&v3[1] >= i)
                {
                LABEL_16:
                    v9 = v5->basic;
                    if (v9)
                    {
                        v10 = v9->dyn;
                        if (v10)
                        {
                            if (v6
                                && (v11 = v6->basic) != 0
                                && (v12 = v11->dyn) != 0
                                && (v12->flags & 0x2000000) != 0
                                && (v10->flags & 0x2000000) != 0
                                && (v6->flags & 0x400000) == 0
                                && (v5->flags & 0x400000) == 0)
                            {
                                v13 = v3->normalVec.y;
                                a3.x = v3->normalVec.x;
                                v14 = v3->normalVec.z;
                                a3.y = v13;
                                a3.z = v14;
                                REC_ApplyDyn(v5, v6, &a3);
                            }
                            else if ((v4 & 0x40) != 0 && (v10->flags & 0x40000000) != 0)
                            {
                                v15 = &v10->forces->gravityVector;
                                if (v15
                                    && (fabs(v15->x) != 0 || fabs(v15->y) != 0  || fabs(v15->z) != 0))
                                {
                                    v20.x = -v15->x;
                                    v16 = v20.x;
                                    v17 = v15->y;
                                    v20.z = -v15->z;
                                    v18 = v20.z;
                                    v20.y = -v17;
                                }
                                else
                                {
                                    v16 = 0.0;
                                    v20.y = 0.0;
                                    v18 = 1.0;
                                }
                                v19 = 1.0 / sqrt(v18 * v18 + v20.y * v20.y + v16 * v16);
                                v20.x = (float)(v19 * v16);
                                v20.y = (float)(v20.y * v19);
                                v20.z = (float)(v18 * v19);
                                REC_Apply(v5, v6, &v20, v4 & 0x80);
                            }
                            else
                            {
                                REC_Apply(v5, v6, &v3->normalVec, v3->flags & 0x80);
                            }
                        }
                    }
                }
                else
                {
                    while (1)
                    {
                        v8 = v7->flags;
                        if ((v8 & 0x43800) == 0
                            && ((v4 & 0x80u) != 0 && (v8 & 0x80u) != 0 || (v4 & 0x40) != 0 && (v8 & 0x40) != 0)
                            && (v7->collisionInstanceGameObject == v5 && v7->collisionMapGameObject == v6
                                || v7->collisionInstanceGameObject == v6 && v7->collisionMapGameObject == v5))
                        {
                            break;
                        }
                        if (++v7 >= i)
                            goto LABEL_16;
                    }
                }
            }
        }
    }
}

float bvin_sqrlen_sub_412FD0(Vec3* a1)
{
    return a1->z * a1->z + a1->y * a1->y + a1->x * a1->x;
}

BOOL COL_b_SingleBVIn(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tGameObjectBV* v2;
    tGameObjectBV* v3;
    Mtx4x4* v4;
    Mtx4x4* v5;
    double v6;
    double v7;
    float v8;
    float v9;
    Mtx4x4* v11;
    double v13;
    double v17;
    double v21;
    float v24; 
    float v25;
    Vec3 a1a; 
    Vec3 dest;
    float v28;
    float v29;
    float v30;
    float v31;

    if (a1 && a2)
    {
        v2 = a1->boundingVolume;
        v3 = a2->boundingVolume;
        if (OBJ_BV_IsAABBox(v2))
        {
            v4 = a2->matrices;
            v5 = a1->matrices;
            v6 = v4->Pos.x - v5->Pos.x;
            v4 = (Mtx4x4*)((char*)v4 + 48);
            v5 = (Mtx4x4*)((char*)v5 + 48);
            v28 = (float)v6;
            v29 = v4->v1.y - v5->v1.y;
            v30 = v4->v1.z - v5->v1.z;
            if (OBJ_BV_IsAABBox(v2))
            {
                v7 = v28 + v3->BVMin.x;
                a1a.y = v3->BVMin.y + v29;
                a1a.z = v3->BVMin.z + v30;
                dest.x = v28 + v3->BVMax.x;
                dest.y = v3->BVMax.y + v29;
                dest.z = v3->BVMax.z + v30;
                return v7 >= v2->BVMin.x
                    && v2->BVMax.x >= (double)dest.x
                    && v2->BVMin.y <= (double)a1a.y
                    && v2->BVMax.y >= (double)dest.y
                    && v2->BVMin.z <= (double)a1a.z
                    && v2->BVMax.z >= (double)dest.z;
            }
            if (OBJ_BV_IsSphere(v3))
            {
                v8 = v3->BVMin.y;
                v9 = v3->BVMin.z;
                v31 = v3->BVMax.x;
                a1a.x = v3->BVMin.x;
                dest.x = a1a.x + v28 + v31;
                dest.y = v8 + v29 + v31;
                dest.z = v9 + v30 + v31;
                v25 = v31 - v29;
                v7 = a1a.x - v25;
                a1a.y = v8 - v25;
                a1a.z = v9 - (v31 - v30);
                return v7 >= v2->BVMin.x
                    && v2->BVMax.x >= (double)dest.x
                    && v2->BVMin.y <= (double)a1a.y
                    && v2->BVMax.y >= (double)dest.y
                    && v2->BVMin.z <= (double)a1a.z
                    && v2->BVMax.z >= (double)dest.z;
            }
        }
        else if (OBJ_BV_IsSphere(v2))
        {
            if (OBJ_BV_IsAABBox(v2))
            {
                MATH_SubstractVec(&dest, &v2->BVMax, &v2->BVMin);
                v11 = a2->matrices;
                dest.x = (float)(dest.x * 0.5);
                dest.y = (float)(dest.y * 0.5);
                dest.z = (float)(dest.z * 0.5);
                MATH_VecAddVec(&a1a, &v11->Pos, &dest);
                v13 = dest.x;
                if (a1a.x < 0.0)
                    v13 = -v13;
                a1a.x = (float)(v13 + a1a.x);
                v17 = dest.y;
                if (a1a.y < 0.0)
                    v17 = -v17;
                a1a.y = (float)(v17 + a1a.y);
                v21 = dest.z;
                if (a1a.z < 0.0)
                    v21 = -v21;
                v24 = v2->BVMax.x;
                a1a.z = (float)(v21 + a1a.z);
                if (v24 * v24 > bvin_sqrlen_sub_412FD0(&a1a))
                    return 1;
            }
            else if (OBJ_BV_IsSphere(v3)
                && v2->BVMax.x >= (double)v3->BVMax.x
                && MATH_DistanceOfTwoPoints(&v2->BVMin, &v3->BVMin) + v3->BVMax.x < v2->BVMax.x)
            {
                return 1;
            }
        }
    }
    return 0;
}

BOOL COL_b_BVIn(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    BOOL result;

    if (a1 && a2)
        result = COL_b_SingleBVIn(a1, a2);
    else
        result = 0;
    return result;
}

BOOL INT_LowLevel_OBBoxOBBox(float a1, float a2, float a3, float a4, float a5, float a6, Mtx4x4* a7)
{
    long double v8; // st7
    BOOL result; // eax
    float v10; // [esp+0h] [ebp-1Ch]
    float v11; // [esp+4h] [ebp-18h]
    float v12; // [esp+8h] [ebp-14h]
    float v13; // [esp+Ch] [ebp-10h]
    float v14; // [esp+10h] [ebp-Ch]
    float v15; // [esp+14h] [ebp-8h]
    float v16; // [esp+18h] [ebp-4h]
    float v17; // [esp+38h] [ebp+1Ch]

    v13 = (float)fabs(a7->v2.x);
    v16 = (float)fabs(a7->v3.x);
    v10 = (float)fabs(a7->v1.x);
    result = 0;
    if (v10 * a4 + v16 * a6 + v13 * a5 + a1 >= fabs(a7->Pos.x))
    {
        v17 = (float)fabs(a7->v1.y);
        v12 = (float)fabs(a7->v2.y);
        v15 = (float)fabs(a7->v3.y);
        if (v15 * a6 + v12 * a5 + v17 * a4 + a2 >= fabs(a7->Pos.y))
        {
            v8 = (float)fabs(a7->v1.z);
            v11 = (float)fabs(a7->v2.z);
            v14 = (float)fabs(a7->v3.z);
            if (v14 * a6 + v11 * a5 + v8 * a4 + a3 >= fabs(a7->Pos.z)
                && v8 * a3 + v17 * a2 + v10 * a1 + a4 >= fabs(a7->Pos.z * a7->v1.z + a7->v1.y * a7->Pos.y + a7->v1.x * a7->Pos.x)
                && v11 * a3 + v12 * a2 + v13 * a1 + a5 >= fabs(a7->Pos.y * a7->v2.y + a7->Pos.z * a7->v2.z + a7->v2.x * a7->Pos.x)
                && v14 * a3 + v15 * a2 + v16 * a1 + a6 >= fabs(a7->Pos.y * a7->v3.y + a7->Pos.z * a7->v3.z + a7->v3.x * a7->Pos.x))
            {
                result = 1;
            }
        }
    }
    return result;
}

BOOL INT_HighLevel_OBBoxOBBox(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Mtx4x4* a5, Vec3* a6)
{
    double v6; // st7
    float v9; // [esp+0h] [ebp-2Ch]
    float v10; // [esp+4h] [ebp-28h]
    float v11; // [esp+8h] [ebp-24h]
    float v12; // [esp+14h] [ebp-18h]
    float v13; // [esp+18h] [ebp-14h]
    float v14; // [esp+1Ch] [ebp-10h]
    Vec3 a1a; // [esp+20h] [ebp-Ch] BYREF
    float v16; // [esp+38h] [ebp+Ch]
    float v17; // [esp+3Ch] [ebp+10h]
    float v18; // [esp+44h] [ebp+18h]

    if (a6)
    {
        v13 = (float)((a2->x - a1->x) * a6->x * 0.5);
        v12 = (float)((a2->y - a1->y) * a6->y * 0.5);
        v6 = (a2->z - a1->z) * a6->z;
    }
    else
    {
        v13 = (float)((a2->x - a1->x) * 0.5);
        v12 = (float)((a2->y - a1->y) * 0.5);
        v6 = a2->z - a1->z;
    }
    v18 = (float)(v6 * 0.5);
    v14 = (float)((a4->x - a3->x) * 0.5);
    v16 = (float)((a4->y - a3->y) * 0.5);
    v17 = (float)((a4->z - a3->z) * 0.5);
    if ((a5->mtxflag & 8) == 0)
        return INT_LowLevel_OBBoxOBBox(v13, v12, v18, v14, v16, v17, a5);
    MATH_GetScale(&a1a, a5);
    v11 = a1a.z * v17;
    v10 = a1a.y * v16;
    v9 = a1a.x * v14;
    return INT_LowLevel_OBBoxOBBox(v13, v12, v18, v9, v10, v11, a5);
}

BOOL COL_b_SingleBVOverlap(tGameObjectMain* a1, tGameObjectMain* a2)
{
    tGameObjectBV* v2; // esi
    tGameObjectBV* v3; // edi
    Mtx4x4* v4; // eax
    double v5; // st7
    double v6; // st6
    Mtx4x4* v7; // eax
    double v8; // st6
    Mtx4x4* v10; // eax
    Vec3* v11; // ebx
    float v12; // [esp+1Ch] [ebp-30h]
    float v13; // [esp+20h] [ebp-2Ch]
    float v14; // [esp+24h] [ebp-28h]
    float v15; // [esp+28h] [ebp-24h]
    float v16; // [esp+2Ch] [ebp-20h]
    float v17; // [esp+30h] [ebp-1Ch]
    Vec3 a3; // [esp+34h] [ebp-18h] BYREF
    Vec3 a4; // [esp+40h] [ebp-Ch] BYREF

    if (a1 && a2)
    {
        v2 = a1->boundingVolume;
        v3 = a2->boundingVolume;
        if (OBJ_BV_IsAABBox(v2) && OBJ_BV_IsAABBox(v3))
        {
            v4 = a1->matrices;
            v5 = v2->BVMin.x + v4->Pos.x;
            a3.y = v4->Pos.y + v2->BVMin.y;
            a3.z = v4->Pos.z + v2->BVMin.z;
            v12 = v4->Pos.x + v2->BVMax.x;
            v13 = v4->Pos.y + v2->BVMax.y;
            v6 = v4->Pos.z;
            v7 = a2->matrices;
            v14 = (float)(v6 + v2->BVMax.z);
            v15 = v3->BVMin.x + v7->Pos.x;
            v16 = v7->Pos.y + v3->BVMin.y;
            v17 = v7->Pos.z + v3->BVMin.z;
            v8 = v7->Pos.x + v3->BVMax.x;
            a4.y = v7->Pos.y + v3->BVMax.y;
            a4.z = v7->Pos.z + v3->BVMax.z;
            if (v5 <= v8
                && v12 >= (double)v15
                && a3.y <= (double)a4.y
                && v13 >= (double)v16
                && a3.z <= (double)a4.z
                && v14 >= (double)v17)
            {
                return 1;
            }
        }
        else
        {
            if (OBJ_BV_IsAABBox(v2) && OBJ_BV_IsSphere(v3))
            {
                v10 = a1->matrices;
                a4.x = v10->Pos.x + v2->BVMax.x;
                a4.y = v2->BVMax.y + v10->Pos.y;
                a4.z = v2->BVMax.z + v10->Pos.z;
                a3.x = v2->BVMin.x + v10->Pos.x;
                a3.y = v10->Pos.y + v2->BVMin.y;
                a3.z = v10->Pos.z + v2->BVMin.z;
                return INT_SphereAABBox(&v3->BVMin, v3->BVMax.x, &a3, &a4);
            }
            if (OBJ_BV_IsSphere(v2) && OBJ_BV_IsAABBox(v3))
            {
                v11 = &a2->matrices->Pos;
                MATH_VecAddVec(&a4, &v3->BVMax, v11);
                MATH_VecAddVec(&a3, &v3->BVMin, v11);
                return INT_SphereAABBox(&v2->BVMin, v2->BVMax.x, &a3, &a4);
            }
            if (OBJ_BV_IsSphere(v2) && OBJ_BV_IsSphere(v3))
                return INT_SphereSphere(&v2->BVMin, v2->BVMax.x, &v3->BVMin, v3->BVMax.x);
        }
    }
    return 0;
}

BOOL COL_b_BVOverlap(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL OBBoxOfFirstGao, BOOL OBBoxOfSecondGao, BOOL useSnP)
{
    tGameObjectMain* v5; // ebx
    Vec3* v6; // edi
    tGameObjectMain* v7; // edx
    BOOL v8; // esi
    int v9; // ecx
    int v10; // eax
    int v11; // eax
    tSnp* v12; // esi
    Vec3* v14; // esi
    Mtx4x4* v15; // eax
    Mtx4x4* v16; // eax
    Mtx4x4* v17; // eax
    Mtx4x4* v18; // eax
    tGameObjectBV* v19; // esi
    tGameObjectBV* v20; // edi
    Mtx4x4* v21; // eax
    Mtx4x4* v22; // eax
    tGameObjectBV* v23; // esi
    Vec3* v24; // edi
    Mtx4x4* v25; // eax
    Mtx4x4* v26; // eax
    tGameObjectBV* v27; // edi
    Vec3* v28; // esi
    double v29; // st7
    Mtx4x4* v30; // eax
    double v31; // st7
    Mtx4x4* v32; // eax
    Vec3* v33; // [esp+14h] [ebp-21Ch] BYREF
    Vec3* a6; // [esp+18h] [ebp-218h] BYREF
    Vec3 v35; // [esp+1Ch] [ebp-214h] BYREF
    Vec3 src; // [esp+28h] [ebp-208h] BYREF
    Vec3 v37; // [esp+34h] [ebp-1FCh] BYREF
    Vec3 v38; // [esp+40h] [ebp-1F0h] BYREF
    Vec3 v39; // [esp+4Ch] [ebp-1E4h] BYREF
    Vec3 v40; // [esp+58h] [ebp-1D8h] BYREF
    Vec3 v41; // [esp+64h] [ebp-1CCh] BYREF
    Mtx4x4 dest; // [esp+70h] [ebp-1C0h] BYREF
    Vec3 v43; // [esp+C0h] [ebp-170h] BYREF
    Vec3 v44; // [esp+CCh] [ebp-164h] BYREF
    Vec3 v45; // [esp+D8h] [ebp-158h] BYREF
    Vec3 v46; // [esp+E4h] [ebp-14Ch] BYREF
    Mtx4x4 v47; // [esp+F0h] [ebp-140h] BYREF
    Mtx4x4 v48; // [esp+140h] [ebp-F0h] BYREF
    Mtx4x4 a2a; // [esp+190h] [ebp-A0h] BYREF
    Mtx4x4 a3a; // [esp+1E0h] [ebp-50h] BYREF
    int v33a;
    int a6a;

    v5 = gao1;
    v6 = 0;
    if (!gao1)
        return FALSE;
    v7 = gao2;
    if (!gao2)
        return FALSE;
    v8 = useSnP;
    if (useSnP)
    {
        v9 = gao1->flags2;
        if ((v9 & 0x80u) == 0 || (v10 = gao2->flags2, (v10 & 0x80u) == 0) || (v9 & 0x20) == 0 && (v10 & 0x20) == 0)
            v8 = FALSE;
    }
    if ((gao1->flags & 0x80000) == 0)
        OBBoxOfFirstGao = FALSE;
    if ((gao2->flags & 0x80000) == 0)
        OBBoxOfSecondGao = FALSE;
    if ((gao1->flags2 & 0x20) == 0)
    {
        v11 = OBBoxOfFirstGao;
        OBBoxOfFirstGao = OBBoxOfSecondGao;
        v5 = gao2;
        OBBoxOfSecondGao = v11;
        gao2 = gao1;
        v7 = gao1;
    }
    if (v8)
    {
        v12 = WOR_gpst_CurrentWorld->snp;
        if (!INT_SnP_FullGetFlag(v12->axisPtr[0]->flagsArray, v5->snpIndex, v7->snpIndex, 1024, &v33a, &a6a)
            || !INT_SnP_GetFlag(v12->axisPtr[1]->flagsArray, v33a, a6a)
            || !INT_SnP_GetFlag(v12->axisPtr[2]->flagsArray, v33a, a6a))
        {
            return FALSE;
        }
    }
    else if (!COL_b_SingleBVOverlap(v5, v7))
    {
        return FALSE;
    }
    if (!OBBoxOfFirstGao && !OBBoxOfSecondGao)
        return TRUE;
    v14 = 0;
    a6 = 0;
    v33 = 0;
    if (OBBoxOfFirstGao)
    {
        if (!OBBoxOfSecondGao)
        {
            if ((OBJ_pst_GetAbsoluteMatrix(v5)->mtxflag & 8) != 0)
            {
                v21 = OBJ_pst_GetAbsoluteMatrix(v5);
                MATH_CopyMatrix(&v48, v21);
                v48._m14 = 0.0;
                v48._m24 = 0.0;
                v48._m34 = 0.0;
                v48.mtxflag &= ~8u;
                MATH_InvertMatrix(&a3a, &v48);
            }
            else
            {
                v22 = OBJ_pst_GetAbsoluteMatrix(v5);
                MATH_InvertMatrix(&a3a, v22);
            }
            memcpy(&v47, &a3a, sizeof(v47));
            v23 = v5->boundingVolume;
            v24 = &gao2->matrices->Pos;
            v33 = &gao2->boundingVolume->BVMin;
            a6 = &v23->BVMaxLocal;
            v14 = &v23->BVMinLocal;
            MATH_VecAddVec(&v43, v33 + 1, v24);
            MATH_VecAddVec(&v46, v33, v24);
            v33 = &v43;
            v6 = &v46;
            goto LABEL_43;
        }
        if ((OBJ_pst_GetAbsoluteMatrix(v5)->mtxflag & 8) != 0)
        {
            v15 = OBJ_pst_GetAbsoluteMatrix(v5);
            MATH_CopyMatrix(&a2a, v15);
            a2a.mtxflag &= ~8u;
            a2a._m14 = 0.0;
            a2a._m24 = 0.0;
            a2a._m34 = 0.0;
            MATH_InvertMatrix(&a3a, &a2a);
        }
        else
        {
            v16 = OBJ_pst_GetAbsoluteMatrix(v5);
            MATH_InvertMatrix(&a3a, v16);
        }
        if ((OBJ_pst_GetAbsoluteMatrix(gao2)->mtxflag & 8) != 0)
        {
            v17 = OBJ_pst_GetAbsoluteMatrix(gao2);
            MATH_CopyMatrix(&dest, v17);
            dest._m14 = 0.0;
            dest._m24 = 0.0;
            dest._m34 = 0.0;
            dest.mtxflag &= ~8u;
        }
        else
        {
            v18 = OBJ_pst_GetAbsoluteMatrix(gao2);
            MATH_CopyMatrix(&dest, v18);
        }
        MATH_MulMatrixMatrix(&v47, &dest, &a3a);
        v19 = v5->boundingVolume;
        v20 = gao2->boundingVolume;
        a6 = &v19->BVMaxLocal;
        v14 = &v19->BVMinLocal;
        v33 = &v20->BVMaxLocal;
        goto LABEL_42;
    }
    if (OBBoxOfSecondGao)
    {
        if ((OBJ_pst_GetAbsoluteMatrix(gao2)->mtxflag & 8) != 0)
        {
            v25 = OBJ_pst_GetAbsoluteMatrix(gao2);
            MATH_CopyMatrix(&dest, v25);
            dest._m14 = 0.0;
            dest._m24 = 0.0;
            dest._m34 = 0.0;
            dest.mtxflag &= ~8u;
            memcpy(&v47, &dest, sizeof(v47));
        }
        else
        {
            v26 = OBJ_pst_GetAbsoluteMatrix(gao2);
            MATH_CopyMatrix(&v47, v26);
        }
        v27 = v5->boundingVolume;
        v28 = &v5->matrices->Pos;
        MATH_VecAddVec(&v45, &v27->BVMax, v28);
        MATH_VecAddVec(&v44, &v27->BVMin, v28);
        v20 = gao2->boundingVolume;
        a6 = &v45;
        v14 = &v44;
        v33 = &v20->BVMaxLocal;
    LABEL_42:
        v6 = &v20->BVMinLocal;
    }
LABEL_43:
    src.x = a6->x - v14->x;
    src.y = a6->y - v14->y;
    v29 = a6->z - v14->z;
    src.x = (float)(src.x * 0.5);
    src.y = (float)(src.y * 0.5);
    src.z = (float)(v29 * 0.5);
    src.x = src.x + v14->x;
    src.y = src.y + v14->y;
    src.z = src.z + v14->z;
    if (OBBoxOfFirstGao)
    {
        v30 = OBJ_pst_GetAbsoluteMatrix(v5);
        MATH_TransformVertexNoScale(&v37, v30, &src);
    }
    else
    {
        v37 = src;
    }
    v35.x = v33->x - v6->x;
    v35.y = v33->y - v6->y;
    v31 = v33->z - v6->z;
    v35.x = (float)(v35.x * 0.5);
    v35.y = (float)(v35.y * 0.5);
    v35.z = (float)(v31 * 0.5);
    v35.x = v35.x + v6->x;
    v35.y = v35.y + v6->y;
    v35.z = v35.z + v6->z;
    if (OBBoxOfSecondGao)
    {
        v32 = OBJ_pst_GetAbsoluteMatrix(gao2);
        MATH_TransformVertexNoScale(&v39, v32, &v35);
    }
    else
    {
        v39 = v35;
    }
    v38.x = v39.x - v37.x;
    v38.y = v39.y - v37.y;
    v38.z = v39.z - v37.z;
    if (OBBoxOfFirstGao)
        MATH_TransformVectorNoScale(&v40, &a3a, &v38);
    else
        v40 = v38;
    v47.Pos = v40;
    v41.x = 1.0;
    v41.y = 1.0;
    v41.z = 1.0;
    return INT_HighLevel_OBBoxOBBox(v14, a6, v6, v33, &v47, &v41);
}

void COL_InitModule()
{
    COL_gst_GlobalVars.wallCosAngle = 0.5f;
}

tColInstSet* COL_AllocColSet()
{
    tColInstSet* v0;
    uint8_t* v1;

    v0 = (tColInstSet*)MEM_p_Alloc(sizeof(tColInstSet));
    LOA_ul_FileTypeSize_38 += sizeof(tColInstSet);
    v0->instElemNum = 0;
    v0->unk1 = 0;
    v0->instanceNum = 0;
    v0->colInstElems = 0;
    v0->zoneIDs = 0;
    v0->unk2 = 0;
    v0->zoneIDs = (uint8_t*)MEM_p_Alloc(16u);
    LOA_ul_FileTypeSize_38 += 4;
    v1 = v0->zoneIDs;
    memset(v1, 0xff, 16);

    return v0;
}

tColInstSet* COL_ul_CallBackLoadColSet(uint32_t Offset)
{
    tColInstSet* v1;
    int v2;
    int v3;
    tColInstanceElem* v4;
    tColInstanceElem* i;
    int v6;
    char* a1;
    int size;
    tColBox* cb;
    tColSphere* cs;
    tColCylinder* cc;

    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    v1 = COL_AllocColSet();
    v1->instElemNum = LOA_ReadChar(&a1, 0, 1);
    v1->unk1 = LOA_ReadChar(&a1, 0, 1);
    v1->instanceNum = 0;
    LOA_ReadShort(&a1, 0, 2);
    v2 = 0;
    v3 = 16;
    do
    {
        v1->zoneIDs[v2++] = LOA_ReadChar(&a1, 0, 1);
        --v3;
    } while (v3);
    if (v1->instElemNum)
        v1->colInstElems = (tColInstanceElem*)MEM_p_Alloc(sizeof(tColInstanceElem) * v1->instElemNum);
    v4 = v1->colInstElems;
    for (i = &v4[v1->instElemNum]; v4 < i; ++v4)
    {
        v4->colFlags = LOA_ReadChar(&a1, 0, 1);
        v4->elemType = LOA_ReadChar(&a1, 0, 1);
        v4->unk1 = LOA_ReadChar(&a1, 0, 1);
        v4->design = LOA_ReadChar(&a1, 0, 1);
        v6 = LOA_ReadLong(&a1, 0, 1);
        if (v6)
            LOA_ReadString(&a1, 0, v6, 2);
        LOA_ReadChar(&a1, 0, 2);
        switch (v4->elemType)
        {
        case 1u:
            cb = (tColBox*)MEM_p_Alloc(sizeof(tColBox));
            v4->elemShapePtr = (void*)cb;
            LOA_ReadVector(&a1, &(cb->pos1), 1);
            LOA_ReadVector(&a1, &(cb->pos2), 1);
            break;
        case 2u:
            cs = (tColSphere*)MEM_p_Alloc(sizeof(tColSphere));
            v4->elemShapePtr = (void*)cs;
            LOA_ReadVector(&a1, &(cs->center), 1);
            cs->radius = LOA_ReadFloat(&a1, 0, 1);
            break;
        case 3u:
            cc = (tColCylinder*)MEM_p_Alloc(sizeof(tColCylinder));
            v4->elemShapePtr = (void*)cc;
            LOA_ReadVector(&a1, &(cc->center), 1);
            cc->height = LOA_ReadFloat(&a1, 0, 1);
            cc->radius = LOA_ReadFloat(&a1, 0, 1);
            break;
        }
    }
    return v1;
}

struct sColInstance* COL_ul_CallBackLoadInstance(uint32_t Offset)
{
    tColInstance* v1;
    int v2;
    uint8_t v3;
    int v4;
    int v5;
    uint8_t v7;
    tColInstanceElem** v8;
    unsigned int v9;
    uint8_t i;
    uint8_t v11;
    tColInstanceElem* v12;
    tColInstanceElem* v13;
    int v14;
    char* a1; 
    int size; 
    tColBox* cb;
    tColSphere* cs;
    tColCylinder* cc;

    v1 = (tColInstance*)MEM_p_Alloc(sizeof(tColInstance));
    memset(v1, 0, sizeof(tColInstance));
    v1->activationFlags = -1;
    v1->mtx = MATH_Matrix_Alloc();
    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    v2 = LOA_ReadLong(&a1, 0, 1);
    if (v2 != -1)
        LOA_MakeFileRef(v2, &v1->colSet, COL_ul_CallBackLoadColSet, 1);
    if (size != 4)
    {
        v1->zoneNum = LOA_ReadChar(&a1, 0, 1);
        v1->sharedZoneNum = LOA_ReadChar(&a1, 0, 1);
        v3 = LOA_ReadChar(&a1, 0, 1);
        v4 = v1->sharedZoneNum;
        v5 = v1->zoneNum;
        v1->instElemNum = v3;
        if (v4 + v3 != v5)
        {
            v1->zoneNum = 0;
            v1->sharedZoneNum = 0;
            v1->instElemNum = 0;
            return v1;
        }
        v1->priority = LOA_ReadChar(&a1, 0, 1);
        v7 = v1->zoneNum;
        if (v7)
        {
            v8 = (tColInstanceElem**)MEM_p_Alloc(4 * v7);
            v9 = 4 * v1->zoneNum;
            v1->selectedInstElemPtrs = v8;
            memset(v8, 0, v9);
        }
        for (i = 0; i < v1->zoneNum; ++i)
            v1->selectedInstElemPtrs[i] = (tColInstanceElem*)(uint8_t)LOA_ReadChar(&a1, 0, 1);
        v11 = v1->instElemNum;
        if (v11)
        {
            v12 = (tColInstanceElem*)MEM_p_Alloc(sizeof(tColInstanceElem) * v11);
            v13 = &v12[v1->instElemNum];
            for (v1->colInstElems = v12; v12 < v13; ++v12)
            {
                v12->colFlags = LOA_ReadChar(&a1, 0, 1);
                v12->elemType = LOA_ReadChar(&a1, 0, 1);
                v12->unk1 = LOA_ReadChar(&a1, 0, 1);
                v12->design = LOA_ReadChar(&a1, 0, 1);
                LOA_ReadChar(&a1, 0, 2);
                v14 = LOA_ReadLong(&a1, 0, 1);
                if (v14)
                    LOA_ReadString(&a1, 0, v14, 2);
                LOA_ReadChar(&a1, 0, 2);
                switch (v12->elemType)
                {
                case 1u:
                    cb = (tColBox*)MEM_p_Alloc(sizeof(tColBox));
                    v12->elemShapePtr = (void *)cb;
                    LOA_ReadVector(&a1, &(cb->pos1), 1);
                    LOA_ReadVector(&a1, &(cb->pos2), 1);
                    break;
                case 2u:
                    cs = (tColSphere*)MEM_p_Alloc(sizeof(tColSphere));
                    v12->elemShapePtr = (void *)cs;
                    LOA_ReadVector(&a1, &(cs->center), 1);
                    cs->radius = LOA_ReadFloat(&a1, 0, 1);
                    break;
                case 3u:
                    cc = (tColCylinder*)MEM_p_Alloc(sizeof(tColCylinder));
                    v12->elemShapePtr = (void *)cc;
                    LOA_ReadVector(&a1, &(cc->center), 1);
                    cc->height = LOA_ReadFloat(&a1, 0, 1);
                    cc->radius = LOA_ReadFloat(&a1, 0, 1);
                    break;
                }
            }
        }
        v1->activationFlags = LOA_ReadShort(&a1, 0, 1);
        v1->instElemBitmask = LOA_ReadShort(&a1, 0, 1);
        LOA_ReadShort(&a1, 0, 2);
        LOA_ReadShort(&a1, 0, 1);
        v1->crossable = 0x589A;
        v1->flag = LOA_ReadShort(&a1, 0, 1);
        v1->minLod = LOA_ReadChar(&a1, 0, 1);
        v1->maxLod = LOA_ReadChar(&a1, 0, 1);
        v1->wallPush = 1;
        v1->backface = 0;
    }
    return v1;
}

struct sCobGameMaterial* COL_ul_CallBackLoadGameMaterial(uint32_t Offset)
{
    tCobGameMaterial* v1;
    int v2;
    tCobGameMaterialElems* v3;
    int* v4;
    int i;
    tCobGameMaterialElems* v6;
    int j;
    char* a1;
    int size;

    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    v1 = (tCobGameMaterial*)MEM_p_Alloc(sizeof(tCobGameMaterial));
    v1->num = 0;
    v1->ptrInts = 0;
    v1->elems = 0;
    v1->gameMaterialUsageCnt = 0;
    v2 = LOA_ReadLong(&a1, 0, 1);
    v1->num = v2;
    v1->ptrInts = (int*)MEM_p_Alloc(4 * v2);
    v3 = (tCobGameMaterialElems*)MEM_p_Alloc(sizeof(tCobGameMaterialElems) * v1->num);
    v4 = v1->ptrInts;
    v1->elems = v3;
    for (i = 0; i < v1->num; ++v4)
    {
        *v4 = LOA_ReadLong(&a1, 0, 1);
        ++i;
    }
    v6 = v1->elems;
    for (j = 0; j < v1->num; ++v6)
    {
        v6->flags = LOA_ReadLong(&a1, 0, 1);
        v6->slide = LOA_ReadFloat(&a1, 0, 1);
        v6->rebound = LOA_ReadFloat(&a1, 0, 1);
        v6->sound = LOA_ReadChar(&a1, 0, 1);
        v6->b2 = LOA_ReadChar(&a1, 0, 1);
        v6->id = LOA_ReadShort(&a1, 0, 1);
        ++j;
    }
    return v1;
}

tColCollisionObject* COL_ul_CallBackLoadCob(uint32_t Offset)
{
    tColCollisionObject* v1;
    int v2;
    int v3;
    uint8_t objType;
    tCobNormalObjectGeneric* v6;
    tColSphere* v7;
    tColBox* v8;
    tColCylinder* v9;
    tCobMeshObjectMesh* cobMesh;
    int v11;
    Vec3* v12;
    Vec3* v13;
    int v14;
    Vec3* v15;
    Vec3* v16;
    unsigned int i;
    int v18;
    tCobMeshChunkElem* v19;
    tCobMeshChunkElem* v20; 
    uint16_t v21;
    int v22;
    tCobMeshTriangleElem* v23;
    int v24;
    BOOL v25;
    int v26;
    tCobMeshChunkElem* v27;
    tCobMeshChunkElem* k;
    int v29;
    int v30;
    tCobOK3* v31;
    BOOL v32;
    int v33;
    unsigned int l;
    int v35; 
    unsigned int v36;
    char* a1;
    tCobMeshChunkElem* j;
    unsigned int v39; 
    tColCollisionObject* v40;
    int size;

    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    v1 = (tColCollisionObject*)MEM_p_Alloc(sizeof(tColCollisionObject));
    v1->objectType = 0;
    v1->flags = 0;
    v1->usageCount = 0;
    v2 = 0;
    v1->gameMaterial = NULL;
    v40 = v1;
    v1->objectPtrByType = NULL;
    v3 = LOA_ReadLong(&a1, 0, 1);
    v1->objectType = LOA_ReadChar(&a1, 0, 1);
    v1->flags = LOA_ReadChar(&a1, 0, 1) & 0xFE;
    objType = v1->objectType;
    if (v1->objectType != 1 && objType != 2 && objType != 3 && objType != 5)
        return 0;
    if (v3 != -1)
    {
        LOA_MakeFileRef(v3, &v1->gameMaterial, COL_ul_CallBackLoadGameMaterial, 1);
        v1->flags |= 1u;
    }
    if (v1->objectType && v1->objectType <= 3u)
    {
        v6 = (tCobNormalObjectGeneric*)MEM_p_Alloc(sizeof(tCobNormalObjectGeneric));
        v1->objectPtrByType = (void*)v6;
        v6->enclosurePtr = 0;
        v6->gameMaterialElemID = 0;
        ((tCobNormalObjectGeneric*)(v1->objectPtrByType))->gameMaterialElemID = LOA_ReadLong(&a1, 0, 1);
    }
    switch (v1->objectType)
    {
    case 1u:
        ((tCobNormalObjectGeneric*)(v1->objectPtrByType))->enclosurePtr = (void *)MEM_p_Alloc(sizeof(tColBox));
        v8 = (tColBox*)((tCobNormalObjectGeneric*)(v1->objectPtrByType))->enclosurePtr;
        LOA_ReadVector(&a1, &v8->pos1, 1);
        LOA_ReadVector(&a1, &v8->pos2, 1);
        break;
    case 2u:
        ((tCobNormalObjectGeneric*)(v1->objectPtrByType))->enclosurePtr = (void *)MEM_p_Alloc(sizeof(tColSphere));
        v7 = (tColSphere*)((tCobNormalObjectGeneric*)(v1->objectPtrByType))->enclosurePtr;
        LOA_ReadVector(&a1, &v7->center, 1);
        v7->radius = LOA_ReadFloat(&a1, 0, 1);
        break;
    case 3u:
        ((tCobNormalObjectGeneric*)(v1->objectPtrByType))->enclosurePtr = (void *)MEM_p_Alloc(sizeof(tColCylinder));
        v9 = (tColCylinder*)((tCobNormalObjectGeneric*)(v1->objectPtrByType))->enclosurePtr;
        LOA_ReadVector(&a1, &v9->center, 1);
        v9->height = LOA_ReadFloat(&a1, 0, 1);
        v9->radius = LOA_ReadFloat(&a1, 0, 1);
        break;
    case 5u:
        cobMesh = (tCobMeshObjectMesh*)MEM_p_Alloc(sizeof(tCobMeshObjectMesh));
        v1->objectPtrByType = cobMesh;
        cobMesh->ok3 = NULL;
        v11 = LOA_ReadLong(&a1, 0, 1);
        cobMesh->vertexCount = v11;
        v12 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v11);
        cobMesh->vertices = v12;
        v13 = v12;
        if (cobMesh->vertexCount)
        {
            do
            {
                LOA_ReadVector(&a1, v13, 1);
                ++v2;
                ++v13;
            } while ((unsigned int)v2 < cobMesh->vertexCount);
        }
        v14 = LOA_ReadLong(&a1, 0, 1);
        cobMesh->faceCount = v14;
        v15 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v14);
        cobMesh->normals = v15;
        v16 = v15;
        for (i = 0; i < cobMesh->faceCount; ++v16)
        {
            LOA_ReadVector(&a1, v16, 1);
            ++i;
        }
        v18 = LOA_ReadLong(&a1, 0, 1);
        cobMesh->meshChunkElemNum = v18;
        v19 = (tCobMeshChunkElem*)MEM_p_Alloc(16 * v18);
        v20 = &v19[cobMesh->meshChunkElemNum];
        cobMesh->meshChunkElems = v19;
        for (j = v20; v19 < v20; ++v19)
        {
            v21 = LOA_ReadShort(&a1, 0, 1);
            v22 = ColMapPoints;
            v19->elemNum = v21;
            ColMapPoints = v21 + v22;
            v19->designVal = LOA_ReadChar(&a1, 0, 1);
            v19->flags = (uint8_t)LOA_ReadChar(&a1, 0, 1);
            v19->gameMaterialElemID = LOA_ReadLong(&a1, 0, 1);
            if (v19->elemNum)
            {
                v23 = (tCobMeshTriangleElem*)MEM_p_Alloc(12 * v19->elemNum);
                v24 = 0;
                v25 = v19->elemNum == 0;
                v19->triangleElems = v23;
                if (!v25)
                {
                    v26 = 0;
                    do
                    {
                        v19->triangleElems[v26].v1 = LOA_ReadShort(&a1, 0, 1);
                        v19->triangleElems[v26].v2 = LOA_ReadShort(&a1, 0, 1);
                        v19->triangleElems[v26].v3 = LOA_ReadShort(&a1, 0, 1);
                        ++v24;
                        ++v26;
                    } while (v24 < v19->elemNum);
                    v1 = v40;
                }
                v20 = j;
            }
            else
            {
                v19->triangleElems = 0;
            }
        }
        if (!LOA_gb_SpeedMode || (v39 = 1, BIG_gi_ReadMode != 2))
            v39 = 0;
        if ((v1->flags & 0x80u) != 0)
        {
            if (v39)
            {
                v27 = cobMesh->meshChunkElems;
                j = &v27[cobMesh->meshChunkElemNum];
                for (k = j; v27 < k; ++v27)
                {
                    if (v27->elemNum)
                    {
                        v29 = 0;
                        if (v27->elemNum)
                        {
                            v30 = 0;
                            do
                            {
                                v27->triangleElems[v30].proximity1 = LOA_ReadShort(&a1, 0, 1);
                                v27->triangleElems[v30].proximity2 = LOA_ReadShort(&a1, 0, 1);
                                v27->triangleElems[v30].proximity3 = LOA_ReadShort(&a1, 0, 1);
                                ++v29;
                                ++v30;
                            } while (v29 < v27->elemNum);
                            v1 = v40;
                            k = j;
                        }
                    }
                }
            }
        }
        if ((v1->flags & 0x88) == 0x88 && v39)
        {
            v31 = (tCobOK3*)MEM_p_Alloc(sizeof(tCobOK3));
            v36 = ((cobMesh->faceCount >> 3) & 0x1FFFFFFC) + 4;
            cobMesh->ok3 = v31;
            cobMesh->ok3->faceTagInts = (int*)MEM_p_Alloc(v36);
            memset(cobMesh->ok3->faceTagInts, 0, 4 * (cobMesh->faceCount >> 5) + 4);
            cobMesh->ok3->num = LOA_ReadLong(&a1, 0, 1);
            cobMesh->ok3->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(sizeof(tCobOK3AABBoxElem) * cobMesh->ok3->num);
            v32 = cobMesh->ok3->num <= 0;
            v39 = 0;
            if (!v32)
            {
                v33 = 0;
                do
                {
                    cobMesh->ok3->aaBoxes[v33].num = LOA_ReadLong(&a1, 0, 1);
                    LOA_ReadVector(&a1, &cobMesh->ok3->aaBoxes[v33].vec2, 1);
                    LOA_ReadVector(&a1, &cobMesh->ok3->aaBoxes[v33].vec1, 1);
                    cobMesh->ok3->aaBoxes[v33].chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(sizeof(tCobOK3AABBoxElemChunkElem) * cobMesh->ok3->aaBoxes[v33].num);
                    for (l = 0; l < cobMesh->ok3->aaBoxes[v33].num; ++l)
                    {
                        cobMesh->ok3->aaBoxes[v33].chunkElems[l].triangleChunkID = LOA_ReadShort(&a1, 0, 1);
                        cobMesh->ok3->aaBoxes[v33].chunkElems[l].num = LOA_ReadShort(&a1, 0, 1);
                        cobMesh->ok3->aaBoxes[v33].chunkElems[l].triangleChunkElemIDs = (uint16_t*)MEM_p_Alloc(2 * cobMesh->ok3->aaBoxes[v33].chunkElems[l].num);
                        v35 = 0;
                        if (cobMesh->ok3->aaBoxes[v33].chunkElems[l].num)
                        {
                            do
                                cobMesh->ok3->aaBoxes[v33].chunkElems[l].triangleChunkElemIDs[v35++] = LOA_ReadShort(&a1, 0, 1);
                            while (v35 < cobMesh->ok3->aaBoxes[v33].chunkElems[l].num);
                        }
                    }
                    ++v33;
                    v32 = ++v39 < cobMesh->ok3->num;
                } while (v32);
                v1 = v40;
            }
        }
        else
        {
            cobMesh->ok3 = NULL;
        }
        break;
    default:
        break;
    }
    v1->flags &= ~0x80u;
    v1->usageCount = 0;
    return v1;
}

struct sColMap* COL_ul_CallBackLoadColMap(uint32_t Offset)
{
    tColMap* colMap;
    int v2;
    tColCollisionObject** v3;
    tColMap* result;
    tColCollisionObject** v5;
    unsigned int v6;
    tColCollisionObject** v7;
    tColCollisionObject** i;
    int v9;
    char* a1;
    int size;

    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    colMap = (tColMap*)MEM_p_Alloc(sizeof(tColMap));
    colMap->colObjectID = 0;
    colMap->activation = 0;
    colMap->b3 = 0;
    colMap->b4 = 0;
    colMap->collisionObjectPtr = 0;
    colMap->activation = -1;
    if (size == 4)
    {
        v2 = LOA_ReadLong(&a1, 0, 1);
        colMap->colObjectID = 1;
        colMap->activation = -1;
        colMap->b3 = 0;
        colMap->b4 = 0;
        v3 = (tColCollisionObject**)MEM_p_Alloc(4u);
        colMap->collisionObjectPtr = v3;
        LOA_MakeFileRef(v2, v3, COL_ul_CallBackLoadCob, 1);
        result = colMap;
    }
    else
    {
        colMap->colObjectID = LOA_ReadChar(&a1, 0, 1);
        colMap->activation = LOA_ReadChar(&a1, 0, 1);
        colMap->b3 = LOA_ReadChar(&a1, 0, 1);
        colMap->b4 = LOA_ReadChar(&a1, 0, 1);
        v5 = (tColCollisionObject**)MEM_p_Alloc(4 * colMap->colObjectID);
        v6 = 4 * colMap->colObjectID;
        colMap->collisionObjectPtr = v5;
        memset(v5, 0, v6);
        v7 = colMap->collisionObjectPtr;
        for (i = &v7[colMap->colObjectID]; v7 < i; ++v7)
        {
            v9 = LOA_ReadLong(&a1, 0, 1);
            if (v9 != -1)
                LOA_MakeFileRef(v9, v7, COL_ul_CallBackLoadCob, 1);
        }
        result = colMap;
    }
    return result;
}

struct sGameObjectMain* COL_RayBV(struct tableDescStruct* a1, Vec3* a2, Vec3* a3, float a4, int a5, int a6, int whatToTest, int a8)
{
    tableElemStructPF* v8;
    tableElemStructPF* v9;
    tGameObjectMain* gao;
    float a5a;
    tGameObjectMain* v13;

    a5a = a4;
    v8 = a1->firstElemPtr;
    v9 = ((tableElemStructPF*)(a1->nextElemPtr)) - 1;
    v13 = 0;
    if (v8 <= v9)
    {
        while (1)
        {
            gao = (tGameObjectMain*)v8->objectPtr;
            if (!v8->objectPtr
                || ((uint8_t)gao & 3) == 1
                || a8 && (gao->flags2 & 0x1000000) != 0
                || !OBJ_b_test_flag(gao, a5, a6, whatToTest))
            {
                goto LABEL_15;
            }
            if (OBJ_BV_IsSphere(gao->boundingVolume))
            {
                if (!COL_RaySphereBV(a2, a3, gao, &a4, a5a) || a4 == -1.0f)
                    goto LABEL_15;
                a5a = a4;
            }
            else
            {
                if (!COL_RayAABBox(a2, a3, gao, &a4, a5a) || a4 == -1.0f)
                    goto LABEL_15;
                a5a = a4;
            }
            v13 = gao;
        LABEL_15:
            if (++v8 > v9)
                return v13;
        }
    }
    return 0;
}

int COL_Report_CollideType(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    tGameObjectExtended* v3;
    tGameObjectCollMain* v4;
    tColInstance* v5;
    int result;
    tRecordElem* v7;
    tRecordElem* v8;
    int v9;

    if ((a3 & 0x10000) != 0)
    {
        if (a1 && (v3 = a1->extended) != 0 && (v4 = v3->coll) != 0 && (v5 = v4->colInstance) != 0)
            result = (uint16_t)COL_test_flag_sub_436970(v5, 2);
        else
            result = 0;
    }
    else
    {
        v7 = a2->records;
        v8 = &a2->records[a2->recordCount];
        if (a2->records >= v8)
        {
        LABEL_16:
            result = 0;
        }
        else
        {
            while (1)
            {
                if ((v7->collisionInstanceGameObject == a1 || v7->collisionMapGameObject == a1)
                    && ((a3 & 0x2000) != 0 || (v7->flags & 0x3000) == 0))
                {
                    v9 = v7->flags;
                    if ((v9 & 0x80040000) == 0 && (a3 & v9) == a3)
                        break;
                }
                if (++v7 >= v8)
                    goto LABEL_16;
            }
            result = 1;
        }
    }
    return result;
}

void COL_Report_GetNormal(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, int a4)
{
    int v5;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;
    int a3a;

    if (a3)
    {
        a3->x = 0.0;
        a3->y = 0.0;
        a3->z = 0.0;
        if (a4 >= 0)
        {
            v5 = 1;
            a3a = 0;
            if ((a4 & 0x2000) != 0)
                v5 = 0;
            if ((a4 & 0x40000000) != 0)
                a3a = 1;
            v6 = a2->records;
            v7 = &a2->records[a2->recordCount];
            if (a2->records < v7)
            {
                while (1)
                {
                    if ((!v5 || (v6->flags & 0x1000) == 0)
                        && (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
                        && (!v5 || (v6->flags & 0x2000) == 0))
                    {
                        v8 = v6->flags;
                        if ((v8 & 0x80040000) == 0 && (!a3a || (v8 & 0x800) == 0) && (a4 & 0xBFFFFFFF & v8) == (a4 & 0xBFFFFFFF))
                            break;
                    }
                    if (++v6 >= v7)
                        return;
                }
                if (v6->collisionInstanceGameObject == a1)
                    *a3 = v6->normalVec;
                else
                    MATH_VecInvert(a3, &v6->normalVec);
            }
        }
        else
        {
            *a3 = a2->records[(uint16_t)a4].normalVec;
        }
    }
}

struct sGameObjectMain* COL_ColMap_RayCast(struct sWorld* a1, struct sGameObjectMain* a2, Vec3* a3, Vec3* a4, float a5, int a6, int a7, int a8, int* a9, int a10, int a11, int a12, int a13, int a14)
{
    Vec3* v14;
    tableElemStructPF* v15;
    tableElemStructPF* v16;
    tGameObjectMain* result;
    tGameObjectMain* v18;
    tWorld* v19;
    tColMap* v20;
    tColCollisionObject** v21;
    uint8_t v22;
    int v23;
    tColCollisionObject* v24;
    tCobNormalObjectGeneric* v25;
    tCobGameMaterialElems* v26;
    uint16_t v27;
    tCobGameMaterialElems* v28;
    tColCollisionObject** v29;
    tColMap* v30;
    tCobNormalObjectGeneric* v31;
    tableElemStructPF* v32;
    uint8_t v33;
    tableElemStructPF* v34;
    float a13a;

    COL_gst_Ray.skipCrossable = a11;
    COL_gst_Ray.skipCamera = a12;
    COL_gst_Ray.useUserCrossable = a14;
    COL_gst_Ray.skipRayCast = a10;
    v14 = a3;
    COL_gst_Ray.direction = a4;
    COL_gst_Ray.distance = a5;
    COL_gst_Ray.skipFlagX = a13;
    COL_gst_Ray.worldSetInColMapRayCast = a1;
    COL_gst_Ray.sourcePoint = a3;
    COL_gst_Ray.ignoreColMapRayCastResult = a9;
    COL_gst_Ray.gao = 0;
    COL_gst_Ray.colMapGao = a2;
    COL_gst_Ray.skipVecsTransformationToGaoSpace = 0;
    if (a9)
        *a9 = 1;
    a1->colCrossObjectNumber = 0;
    v15 = a1->eot.collMapObjectsTablePF.firstElemPtr;
    v16 = ((tableElemStructPF*)(a1->eot.collMapObjectsTablePF.nextElemPtr)) - 1;
    v34 = v15;
    v32 = v16;
    if (v15 > v16)
    {
        result = COL_gst_Ray.gao;
        COL_gb_UseMinDistance = 0;
        return result;
    }
    while (1)
    {
        v18 = (tGameObjectMain*)v15->objectPtr;
        if (!v15->objectPtr
            || ((uint8_t)v18 & 3) == 1
            || (v18->flags & 0x100) == 0
            || (v18->flags2 & 1) == 0
            || !OBJ_b_test_flag(v18, a6, a7, a8)
            || v18 == a2
            || (!OBJ_BV_IsAABBox(v18->boundingVolume) || !COL_RayAABBox(v14, a4, v18, &a13a, COL_gst_Ray.distance))
            && (!OBJ_BV_IsSphere(v18->boundingVolume) || !COL_RaySphereBV(v14, a4, v18, &a13a, COL_gst_Ray.distance)))
        {
            goto LABEL_53;
        }
        if (COL_gst_Ray.skipRayCast && (v18->flags2 & 0x1000000) != 0)
        {
            v19 = COL_gst_Ray.worldSetInColMapRayCast;
            if (COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber == 16)
            {
                COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber = 15;
                v19 = COL_gst_Ray.worldSetInColMapRayCast;
            }
            v19->collCrossObjects[v19->colCrossObjectNumber] = v18;
            ++COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber;
            goto LABEL_53;
        }
        if ((v18->flags & 0x100) == 0)
            goto LABEL_53;
        v20 = v18->extended->coll->colMap;
        v21 = v20->collisionObjectPtr;
        if ((*v21)->objectType != 5)
        {
            v22 = 0;
            v33 = 0;
            if (!v20->colObjectID)
                goto LABEL_53;
            while (1)
            {
                v23 = v22;
                if (((uint8_t)(1 << v22) & v20->activation) != 0)
                {
                    v24 = v20->collisionObjectPtr[v22];
                    v25 = (tCobNormalObjectGeneric*)v24->objectPtrByType;
                    v31 = v25;
                    if (!a11)
                        goto LABEL_45;
                    if (!a14)
                    {
                        if (v24->gameMaterial)
                        {
                            if (a2 && (a2->flags & 0x200) != 0)
                                v27 = a2->extended->coll->colInstance->crossable;
                            else
                                v27 = 0x589A;
                            v28 = COL_pst_GMat_Get(v24, 0);
                            if (v28 && (v27 & v28->flags) != 0)
                                goto LABEL_51;
                            v25 = v31;
                        }
                    LABEL_45:
                        v29 = v20->collisionObjectPtr;
                        switch (v29[v23]->objectType)
                        {
                        case 1u:
                            COL_ColMap_Box_RayCast(v18, v25);
                            break;
                        case 2u:
                            COL_ColMap_Sphere_RayCast(v18, v25);
                            break;
                        case 3u:
                            COL_ColMap_Cylinder_RayCast(v18, v25);
                            break;
                        }
                        goto LABEL_51;
                    }
                    v26 = COL_pst_GMat_Get(v24, 0);
                    if (!v26 || (COL_guw_UserCrossable & v26->flags) == 0)
                        goto LABEL_45;
                }
            LABEL_51:
                v22 = ++v33;
                if (v33 >= v20->colObjectID)
                {
                    v16 = v32;
                    v15 = v34;
                    goto LABEL_53;
                }
            }
        }
        if ((v20->activation & 1) != 0)
        {
            v30 = v18->extended->coll->colMap;
            if (((tCobMeshObjectMesh*)((*v21)->objectPtrByType))->ok3)
                COL_ColMap_OK3_RayCast(v18, v30);
            else
                COL_ColMap_Triangles_RayCast(v18, v30);
            if (COL_gst_Ray.ignoreColMapRayCastResult && !*COL_gst_Ray.ignoreColMapRayCastResult)
                return 0;
        }
    LABEL_53:
        v34 = ++v15;
        if (v15 > v16)
            break;
        v14 = a3;
    }
    result = COL_gst_Ray.gao;
    COL_gb_UseMinDistance = 0;
    return result;
}

struct sGameObjectMain* COL_Report_GetCOBGameObject(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    int v4;
    tGameObjectMain* result;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;
    BOOL v9;

    if (a3 >= 0)
    {
        v9 = (a3 & 0x2000) == 0;
        v6 = a2->records;
        v7 = &a2->records[a2->recordCount];
        if (a2->records >= v7)
        {
        LABEL_15:
            result = 0;
        }
        else
        {
            while (1)
            {
                if (!v9 || (v6->flags & 0x3000) == 0)
                {
                    v8 = v6->flags;
                    if ((v8 & 0x80040000) == 0)
                    {
                        if (v6->collisionInstanceGameObject == a1 && (a3 & v8) == a3 && v6->collisionMapGameObject)
                            return v6->collisionMapGameObject;
                        if (v6->collisionMapGameObject == a1 && (a3 & v8) == a3 && v6->collisionInstanceGameObject)
                            break;
                    }
                }
                if (++v6 >= v7)
                    goto LABEL_15;
            }
            result = v6->collisionInstanceGameObject;
        }
    }
    else
    {
        v4 = (uint16_t)a3;
        result = a2->records[v4].collisionMapGameObject;
        if (result == a1)
            result = a2->records[v4].collisionInstanceGameObject;
    }
    return result;
}

int COL_GameMaterial_GetFlags(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    int result;
    tCobGameMaterialElems* v4;
    tCobGameMaterialElems* v5;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;
    int* v9;

    if (!a2)
        return 0;
    if (a3 == 0xFFFF)
    {
        v4 = a2->rayCastCobGmat;
        if (v4)
            return v4->flags;
        return 0;
    }
    if (a3 >= 0)
    {
        v6 = a2->records;
        v7 = &a2->records[a2->recordCount];
        if (a2->records >= v7)
            goto LABEL_19;
        while (1)
        {
            if (((a3 & 0x2000) != 0 || (v6->flags & 0x1000) == 0)
                && (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
                && ((a3 & 0x2000) != 0 || (v6->flags & 0x2000) == 0))
            {
                v8 = v6->flags;
                if ((v8 & 0x80040000) == 0 && (a3 & v8) == a3)
                    break;
            }
            if (++v6 >= v7)
                goto LABEL_19;
        }
        v9 = &v6->gameMaterial->flags;
        if (v9)
            result = *v9;
        else
            LABEL_19:
        result = 0;
    }
    else
    {
        v5 = a2->records[(uint16_t)a3].gameMaterial;
        if (!v5)
            return 0;
        result = v5->flags;
    }
    return result;
}

float COL_GameMaterial_GetSlide(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    double result;
    tCobGameMaterialElems* v4;
    tCobGameMaterialElems* v5;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;

    if (!a2)
        return 0.0f;
    if (a3 == 0xFFFF)
    {
        v4 = a2->rayCastCobGmat;
        if (v4)
            return v4->slide;
        return 0.0;
    }
    if (a3 >= 0)
    {
        v6 = a2->records;
        v7 = &a2->records[a2->recordCount];
        if (a2->records >= v7)
        {
        LABEL_20:
            result = 0.0f;
        }
        else
        {
            while (1)
            {
                if (((a3 & 0x2000) != 0 || (v6->flags & 0x1000) == 0)
                    && (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
                    && ((a3 & 0x2000) != 0 || (v6->flags & 0x2000) == 0))
                {
                    v8 = v6->flags;
                    if ((v8 & 0x80040000) == 0 && (a3 & v8) == a3 && v6->gameMaterial)
                        break;
                }
                if (++v6 >= v7)
                    goto LABEL_20;
            }
            result = v6->gameMaterial->slide;
        }
    }
    else
    {
        v5 = a2->records[(uint16_t)a3].gameMaterial;
        if (!v5)
            return 0.0;
        result = v5->slide;
    }
    return (float)result;
}

float COL_GameMaterial_GetRebound(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    double result;
    tCobGameMaterialElems* v4;
    tCobGameMaterialElems* v5;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;

    if (!a2)
        return 0.0;
    if (a3 == 0xFFFF)
    {
        v4 = a2->rayCastCobGmat;
        if (v4)
            return v4->rebound;
        return 0.0;
    }
    if ((int)a3 >= 0)
    {
        v6 = a2->records;
        v7 = &a2->records[a2->recordCount];
        if (a2->records >= v7)
        {
        LABEL_20:
            result = 0.0;
        }
        else
        {
            while (1)
            {
                if (((a3 & 0x2000) != 0 || (v6->flags & 0x1000) == 0)
                    && (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
                    && ((a3 & 0x2000) != 0 || (v6->flags & 0x2000) == 0))
                {
                    v8 = v6->flags;
                    if ((v8 & 0x80040000) == 0 && (a3 & v8) == a3 && v6->gameMaterial)
                        break;
                }
                if (++v6 >= v7)
                    goto LABEL_20;
            }
            result = v6->gameMaterial->rebound;
        }
    }
    else
    {
        v5 = a2->records[a3].gameMaterial;
        if (!v5)
            return 0.0;
        result = v5->rebound;
    }
    return (float)result;
}

uint8_t COL_GameMaterial_GetSound(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    uint8_t result;
    tCobGameMaterialElems* v4;
    tCobGameMaterialElems* v5;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;

    if (!a2)
        return 0;
    if (a3 == 0xFFFF)
    {
        v4 = a2->rayCastCobGmat;
        if (v4)
            return v4->sound;
        return 0;
    }
    if (a3 >= 0)
    {
        v6 = a2->records;
        v7 = &a2->records[a2->recordCount];
        if (a2->records >= v7)
        {
        LABEL_19:
            result = 0;
        }
        else
        {
            while (1)
            {
                if (((a3 & 0x2000) != 0 || (v6->flags & 0x1000) == 0)
                    && v6->collisionInstanceGameObject == a1
                    && ((a3 & 0x2000) != 0 || (v6->flags & 0x2000) == 0))
                {
                    v8 = v6->flags;
                    if ((v8 & 0x80040000) == 0 && (a3 & v8) == a3 && v6->gameMaterial)
                        break;
                }
                if (++v6 >= v7)
                    goto LABEL_19;
            }
            result = v6->gameMaterial->sound;
        }
    }
    else
    {
        v5 = a2->records[(uint16_t)a3].gameMaterial;
        if (!v5)
            return 0;
        result = v5->sound;
    }
    return result;
}

void COL_FindEdgeInFollowDirection(Vec3* hitPoint, Vec3** point1, Vec3** point2, Vec3** point3, Vec3* right, Vec3* normal, int down)
{
    Vec3* v7;
    Vec3* v8;
    Vec3* v9;
    Vec3* v10;
    BOOL v11; 
    Vec3* v12;
    Vec3* v13;
    Vec3* v14;
    Vec3* v15;
    Vec3* v16;
    Vec3* v17;
    BOOL v18; 
    Vec3* v19;

    if (down)
    {
        v14 = *point1;
        v15 = *point2;
        if ((*point1)->z > (double)(*point2)->z || (v16 = *point3, v14->z > (double)(*point3)->z))
        {
            if (v15->z > (double)v14->z || (v16 = *point3, v15->z > (double)(*point3)->z))
            {
                v16 = *point3;
                v17 = *point3;
            }
            else
            {
                v17 = *point2;
            }
        }
        else
        {
            v17 = *point1;
        }
        v18 = right->z * (v17->z - hitPoint->z) + right->y * (v17->y - hitPoint->y) + (v17->x - hitPoint->x) * right->x <= 0.0f; // dotproduct
        if (v17 == v15 && right->z * (v17->z - hitPoint->z) + right->y * (v17->y - hitPoint->y) + (v17->x - hitPoint->x) * right->x <= 0.0f
            || v17 == v16 && right->z * (v17->z - hitPoint->z) + right->y * (v17->y - hitPoint->y) + (v17->x - hitPoint->x) * right->x > 0.0f)
        {
            v19 = v16;
        }
        else
        {
            v19 = *point1;
        }
        *point1 = v19;
        v13 = *point3;
        if (*point3 != v19 && (v17 == v19 && !v18 || v17 == v13 && v18))
            goto LABEL_40;
    LABEL_39:
        v13 = *point2;
        goto LABEL_40;
    }
    v7 = *point1;
    v8 = *point2;
    if ((*point1)->z < (double)(*point2)->z || (v9 = *point3, v7->z < (double)(*point3)->z))
    {
        if (v8->z < (double)v7->z || (v9 = *point3, v8->z < (double)(*point3)->z))
        {
            v9 = *point3;
            v10 = *point3;
        }
        else
        {
            v10 = *point2;
        }
    }
    else
    {
        v10 = *point1;
    }
    v11 = right->z * (v10->z - hitPoint->z) + right->y * (v10->y - hitPoint->y) + (v10->x - hitPoint->x) * right->x <= 0.0f;
    if (v10 == v8 && right->z * (v10->z - hitPoint->z) + right->y * (v10->y - hitPoint->y) + (v10->x - hitPoint->x) * right->x > 0.0f
        || v10 == v9 && right->z * (v10->z - hitPoint->z) + right->y * (v10->y - hitPoint->y) + (v10->x - hitPoint->x) * right->x <= 0.0f)
    {
        v12 = v9;
    }
    else
    {
        v12 = *point1;
    }
    *point1 = v12;
    v13 = *point3;
    if (*point3 == v12)
        goto LABEL_39;
    if (v10 == v12 && v11)
        goto LABEL_40;
    if (v10 != v13)
        goto LABEL_39;
    if (v11)
    {
        *point2 = *point2;
        return;
    }
LABEL_40:
    *point2 = v13;
}

tCobMeshTriangleElem* COL_GetZConnected(Vec3* a1, Vec3* a2, tCobMeshObjectMesh* a3, tCobMeshChunkElem* a4, tCobMeshTriangleElem* a5)
{
    int v6; 
    tCobMeshTriangleElem* result;
    tCobMeshTriangleElem* i;
    Vec3* v9;
    Vec3* v1;
    Vec3* v2;
    Vec3* v3;
    int v13; 
    int v14; 

    v6 = a4->elemNum;
    result = 0;
    v13 = 0;
    v14 = v6;
    if (v6)
    {
        for (i = a4->triangleElems; ; ++i)
        {
            if (i != a5)
            {
                v9 = a3->vertices;
                v1 = &v9[i->v1];
                v2 = &v9[i->v2];
                v3 = &v9[i->v3];
                if ((a1 == v1 || a1 == v2 || a1 == v3) && (a2 == v1 || a2 == v2 || a2 == v3))
                    return i;
                v6 = v14;
            }
            if (++v13 >= v6)
                return 0;
        }
    }
    return result;
}

tCobMeshTriangleElem* COL_GetPointConnectedTriangleWithMuchHorizontalEdge(Vec3* a1, Vec3* a2, Vec3* a3, tCobMeshObjectMesh* a4, Mtx4x4* a5, tCobMeshChunkElem* a6)
{
    tCobMeshChunkElem* v6;
    tCobMeshTriangleElem* result;
    tCobMeshTriangleElem* v8;
    int v9; 
    Vec3* v10;
    tCobMeshTriangleElem* v11;
    Vec3* v12;
    Vec3* v13;
    Vec3* v14;
    double v15;
    double v16;
    double v17;
    long double v18;
    long double v19;
    Vec3* v20;
    Vec3* v21;
    float v22;
    Vec3* v23;
    Vec3* v24;
    double v25;
    double v26;
    double v27;
    long double v28;
    long double v29;
    Vec3* v30;
    float v31;
    Vec3* v32;
    float v33;
    double v34;
    double v35;
    double v36;
    long double v37;
    long double v38;
    Vec3* v39;
    float v40;
    Vec3* v41;
    float v42;
    float v44;
    tCobMeshTriangleElem* v45;
    float v46;
    float v47;
    Vec3* src;
    float srca;
    int i;
    int v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    Vec3 dest;
    Vec3 v59; 
    Vec3 v60; 

    v6 = a6;
    result = 0;
    v45 = 0;
    v44 = 100000.0f;
    v51 = 0;
    if (a6->elemNum)
    {
        for (i = 0; ; ++i)
        {
            v8 = v6->triangleElems;
            v9 = v8[i].v1;
            v10 = a4->vertices;
            v11 = &v8[i];
            v12 = &v10[v9];
            v13 = &v10[v11->v2];
            v14 = &v10[v11->v3];
            src = v14;
            if (a1 == v12 || a1 == v13 || a1 == v14)
            {
                MATH_TransformVertexNoScale(&dest, a5, v12);
                MATH_TransformVertexNoScale(&v60, a5, v13);
                MATH_TransformVertexNoScale(&v59, a5, src);
                if (a1 != v12 && a1 != v13)
                    goto LABEL_15;
                v15 = dest.x - v60.x;
                v16 = dest.y - v60.y;
                v17 = dest.z - v60.z;
                v55 = (float)v17;
                v18 = 1.0 / sqrt(v17 * v55 + v16 * v16 + v15 * v15);
                v46 = (float)v18;
                v52 = (float)(v18 * v15);
                v19 = fabs(v55 * v46 * MATH_gst_BaseVectorK.z + v16 * v46 * MATH_gst_BaseVectorK.y + MATH_gst_BaseVectorK.x
                    * v52);
                if (v19 < v44)
                {
                    v44 = (float)v19;
                    v45 = v11;
                    v20 = &dest;
                    if (a1 != v12)
                        v20 = &v60;
                    v21 = a2;
                    a2->x = v20->x;
                    v22 = v20->y;
                    a2->z = v20->z;
                    a2->y = v22;
                    v23 = &v60;
                    if (a1 != v12)
                        v23 = &dest;
                    v24 = a3;
                    *a3 = *v23;
                    if (fabs(v44) < 0.001)
                        return v45;
                }
                else
                {
                LABEL_15:
                    v21 = a2;
                    v24 = a3;
                }
                if (a1 == v12 || a1 == src)
                {
                    v25 = dest.x - v59.x;
                    v26 = dest.y - v59.y;
                    v27 = dest.z - v59.z;
                    v56 = (float)v27;
                    v28 = 1.0 / sqrt(v27 * v56 + v26 * v26 + v25 * v25);
                    v47 = (float)v28;
                    v53 = (float)(v28 * v25);
                    v29 = fabs(v56 * v47 * MATH_gst_BaseVectorK.z + v26 * v47 * MATH_gst_BaseVectorK.y + MATH_gst_BaseVectorK.x
                        * v53);
                    if (v29 < v44)
                    {
                        v44 = (float)v29;
                        v45 = v11;
                        v30 = &dest;
                        if (a1 != v12)
                            v30 = &v59;
                        v21->x = v30->x;
                        v31 = v30->y;
                        v21->z = v30->z;
                        v21->y = v31;
                        v32 = &v59;
                        if (a1 != v12)
                            v32 = &dest;
                        v24 = a3;
                        a3->x = v32->x;
                        v33 = v32->y;
                        a3->z = v32->z;
                        a3->y = v33;
                        if (fabs(v44) < 0.001)
                            return v45;
                    }
                }
                if (a1 == v13 || a1 == src)
                {
                    v34 = v60.x - v59.x;
                    v35 = v60.y - v59.y;
                    v36 = v60.z - v59.z;
                    v57 = (float)v36;
                    v37 = 1.0 / sqrt(v36 * v57 + v35 * v35 + v34 * v34);
                    srca = (float)v37;
                    v54 = (float)(v37 * v34);
                    v38 = fabs(v57 * srca * MATH_gst_BaseVectorK.z + v35 * srca * MATH_gst_BaseVectorK.y  + MATH_gst_BaseVectorK.x * v54);
                    if (v38 < v44)
                    {
                        v44 = (float)v38;
                        v45 = v11;
                        v39 = &v60;
                        if (a1 != v13)
                            v39 = &v59;
                        v21->x = v39->x;
                        v40 = v39->y;
                        v21->z = v39->z;
                        v21->y = v40;
                        v41 = &v59;
                        if (a1 != v13)
                            v41 = &v60;
                        v24->x = v41->x;
                        v42 = v41->y;
                        v24->z = v41->z;
                        v24->y = v42;
                        if (fabs(v44) < 0.001)
                            return v45;
                    }
                }
            }
            v6 = a6;
            if (++v51 >= a6->elemNum)
                return v45;
        }
    }
    return result;
}

// TODO FIXME átnézni, fõleg a &vec local változókat
void COL_ComputeJumpPoint(struct sGameObjectMain* a1, Vec3* a2, float a3, int a4, int a5)
{
    Vec3* v5;
    tWorld* v6;
    tWorld* v7;
    int idx;
    uint16_t rayCastTriangleChunkID;
    tGameObjectMain* rayCastGao;
    tRecordElem* v11;
    tRecordElem* v12;
    int v13;
    int v14;
    tGameObjectExtended* v15;
    tGameObjectCollMain* v16;
    tColMap* v17;
    tColCollisionObject* v18;
    tCobMeshObjectMesh* v19;
    int TCHid;
    tCobMeshTriangleElem* v21;
    tCobMeshTriangleElem* i;
    Mtx4x4* v23;
    Mtx4x4* v24;
    Mtx4x4* v25;
    Vec3* v26;
    double v27;
    double v31;
    double v32;
    Vec3* v33;
    int v34; 
    Vec3* v35;
    Mtx4x4* v36;
    Mtx4x4* v37;
    double v38; 
    double v39; 
    double v40; 
    long double v41;
    double v42;
    uint8_t v43;
    BOOL v44;
    BOOL v45;
    Mtx4x4* v46;
    long double v47;
    Vec3* v48;
    Vec3* v49;
    Vec3* v50;
    Vec3* v51;
    tWorld* v52;
    Vec3* v53;
    Vec3* v54;
    Vec3* v55;
    Vec3* v56;
    tCobMeshChunkElem* v57;
    char v58;
    Vec3* tobbertekevanMEgnezni;
    Vec3* v2; 
    int v61; 
    float v62;
    tWorld* v63 = NULL; // TODO FIXME: game doesn't set it to anything, should chekc it. Game calls this function with a5 being set to true, so this shouldn1t happen.
    Vec3* a4a;
    tCobMeshTriangleElem* v65; 
    Vec3 dest; 
    Vec3 a1a;
    Vec3 a2a;
    Vec3 a5a;
    Vec3 v70;
    Vec3 a6; 
    Vec3 v72;
    Vec3 a3a;
    float tobbertekevanMEgneznia;
    float v65a;
    float v2a;

    v5 = a2;
    *a2 = MATH_gst_NulVector;
    v6 = WOR_gpst_CurrentWorld;
    if (!WOR_gpst_CurrentWorld)
        return;
    if (a5)
    {
        v7 = WOR_gpst_CurrentWorld;
        a2->x = WOR_gpst_CurrentWorld->colRayHitPos.x;
        a2->y = v6->colRayHitPos.y;
        a2->z = v6->colRayHitPos.z;
        idx = v6->rayCastTriangleChunkElemID;
        rayCastTriangleChunkID = v7->rayCastTriangleChunkID;
        rayCastGao = v7->rayCastGao;
        v63 = v7;
    }
    else
    {
        v11 = WOR_gpst_CurrentWorld->records;
        v12 = &WOR_gpst_CurrentWorld->records[WOR_gpst_CurrentWorld->recordCount];
        if (WOR_gpst_CurrentWorld->records < v12)
        {
            do
            {
                if (v11->collisionInstanceGameObject == a1)
                {
                    v13 = v11->flags;
                    if ((v13 & 0x80u) != 0 && (v13 & 0xF) != 0)
                        break;
                }
                ++v11;
            } while (v11 < v12);
        }
        v14 = v11->flags;
        if ((v14 & 0x80u) == 0 || (v14 & 0xF) == 0)
            return;
        *a2 = v11->collidedPoint;
        idx = v11->triangleChunkElemID;
        rayCastTriangleChunkID = v11->triangleChunkID;
        rayCastGao = v11->collisionMapGameObject;
    }
    if (!rayCastGao)
        return;
    if ((rayCastGao->flags & 0x100) == 0)
        return;
    v15 = rayCastGao->extended;
    if (!v15)
        return;
    v16 = v15->coll;
    if (!v16)
        return;
    v17 = v16->colMap;
    if (!v17)
        return;
    v18 = *v17->collisionObjectPtr;
    if (!v18 || v18->objectType != 5)
        return;
    v19 = (tCobMeshObjectMesh*)(v18->objectPtrByType);
    TCHid = rayCastTriangleChunkID;
    v21 = v19->meshChunkElems[TCHid].triangleElems;
    v62 = a3 + a3;
    v61 = TCHid;
    i = &v21[idx];
    v58 = 0;
    a4a = &v70;
    while (2)
    {
        v53 = &v19->vertices[i->v1];
        v23 = (Mtx4x4*)OBJ_pst_GetAbsoluteMatrix(rayCastGao);
        MATH_TransformVertexNoScale(&a1a, v23, v53);
        v54 = &v19->vertices[i->v2];
        v24 = (Mtx4x4*)OBJ_pst_GetAbsoluteMatrix(rayCastGao);
        MATH_TransformVertexNoScale(&a2a, v24, v54);
        v55 = &v19->vertices[i->v3];
        v25 = (Mtx4x4*)OBJ_pst_GetAbsoluteMatrix(rayCastGao);
        MATH_TransformVertexNoScale(&v70, v25, v55);
        tobbertekevanMEgnezni = &a1a;
        v2 = &a2a;
        MATH_SubstractVec(&dest, &a2a, &a1a);
        MATH_SubstractVec(&a3a, &v70, &a1a);
        MATH_CrossProduct(&a6, &dest, &a3a);
        MATH_NormalizeEqualVector(&a6);
        MATH_CrossProduct(&a5a, &MATH_gst_BaseVectorK, &a6);
        COL_FindEdgeInFollowDirection(v5, &tobbertekevanMEgnezni, &v2, &a4a, &a5a, &a6, a4);
        v26 = tobbertekevanMEgnezni;
        MATH_SubstractVec(&dest, v2, tobbertekevanMEgnezni);
        tobbertekevanMEgneznia = MATH_DistanceOfTwoPoints(v26, v2);
        MATH_SubstractVec(&v72, v26, a2);
        v27 = -(a5a.z * v72.z + a5a.y * v72.y + a5a.x * v72.x) / (dest.z * a5a.z + dest.y * a5a.y + dest.x * a5a.x);
        if (tobbertekevanMEgneznia <= (double)v62)
        {
            v27 = tobbertekevanMEgneznia * 0.5;
        }
        else if (tobbertekevanMEgneznia * v27 < a3 || (1.0 - v27) * tobbertekevanMEgneznia < a3)
        {
            v27 = a3 / tobbertekevanMEgneznia;
            if (tobbertekevanMEgneznia > v62)
                v27 = 1.0 - v27;
        }
        tobbertekevanMEgneznia = (float)(dest.x * v27);
        a2->x =  tobbertekevanMEgneznia;
        v65a = (float)(dest.y * v27);
        a2->y = v65a;
        v31 = v27 * dest.z;
        a2->z = (float)v31;
        a2->x = tobbertekevanMEgneznia + v26->x;
        v32 = v65a;
        v65 = i;
        a2->y = (float)(v32 + v26->y);
        a2->z = (float)(v31 + v26->z);
        v33 = v19->vertices;
        if (&v70 == v26)
            v34 = i->v3;
        else
            v34 = i->v1;
        tobbertekevanMEgnezni = &v33[v34];
        if (&v70 == v2)
            v35 = &v33[i->v3];
        else
            v35 = &v33[i->v2];
        for (i = COL_GetZConnected(tobbertekevanMEgnezni, v35, v19, &v19->meshChunkElems[v61], i);
            ;
            i = 0)
        {
            if (!i)
            {
                v56 = tobbertekevanMEgnezni;
                v36 = (Mtx4x4*)OBJ_pst_GetAbsoluteMatrix(rayCastGao);
                MATH_TransformVertexNoScale(&a1a, v36, v56);
                v37 = (Mtx4x4*)OBJ_pst_GetAbsoluteMatrix(rayCastGao);
                MATH_TransformVertexNoScale(&a2a, v37, v35);
                v38 = a1a.x - a2a.x;
                v39 = a1a.y - a2a.y;
                v40 = a1a.z - a2a.z;
                dest.z = (float)v40;
                v41 = 1.0 / sqrt(v40 * dest.z + v39 * v39 + v38 * v38);
                v2a = (float)v41;
                dest.x = (float)(v41 * v38);
                dest.y = (float)(v39 * v2a);
                v42 = dest.z * v2a;
                dest.z = (float)v42;
                if (fabs(v42 * MATH_gst_BaseVectorK.z + dest.y * MATH_gst_BaseVectorK.y + dest.x * MATH_gst_BaseVectorK.x) > gcjpUnkFloatflt_968524)
                {
                    v44 = a1a.z < (double)a2a.z;
                    v45 = a1a.z == a2a.z;
                    if (a4)
                    {
                        if (!v44 && !v45)
                            goto LABEL_44;
                    }
                    else if (v44 || v45)
                    {
                    LABEL_44:
                        tobbertekevanMEgnezni = v35;
                        goto LABEL_45;
                    }
                LABEL_45:
                    v57 = &(v19->meshChunkElems[v61]);
                    v46 = OBJ_pst_GetAbsoluteMatrix(rayCastGao);
                    COL_GetPointConnectedTriangleWithMuchHorizontalEdge(tobbertekevanMEgnezni, &a1a, &a2a, v19, v46, v57);
                    dest.x = a2a.x - a1a.x;
                    dest.y = a2a.y - a1a.y;
                    dest.z = a2a.z - a1a.z;
                    v47 = MATH_DistanceOfTwoPoints(&a2a, &a1a);
                    if (v47 <= v62)
                        v62 = (float)(v47 * 0.5);
                    else
                        v62 = (float)(a3 / v47);
                    MATH_VecMultScalar(a2, &dest, v62);
                    MATH_VecAddVecWithoutBuffer(a2, &a1a);
                    goto LABEL_49;
                }
            }
            v43 = ++v58;
            if (v58 != 14)
                break;
        }
        if (!i)
        {
        LABEL_49:
            v48 = v19->vertices;
            v49 = &v48[v65->v1];
            v50 = &v48[v65->v2];
            v51 = &v48[v65->v3];
            MATH_SubstractVec(&a6, v50, v49);
            MATH_SubstractVec(&a3a, v51, v49);
            MATH_CrossProduct(&v72, &a6, &a3a);
            MATH_NormalizeEqualVector(&v72);
            MATH_TransformVector(&(v63->rayCastNormalVec), rayCastGao->matrices, &v72);
            return;
        }
        if (v43 < 15u)
        {
            v5 = a2;
            continue;
        }
        break;
    }
    v52 = v63;
    v63->rayCastNormalVec.x = 0;
    v52->rayCastNormalVec.y = 0;
    v52->rayCastNormalVec.z = 0;
}

int COL_Report_GetZDMGameObjectList(struct sGameObjectMain* a1, struct sWorld* a2, struct sGameObjectMain** a3, Vec3* a4, int a5)
{
    int v5;
    int v7;
    int v8;
    tRecordElem* v9;

    v5 = 0;
    if ((a1->flags & 0x100) == 0)
        return 0;
    v7 = 0;
    if (a2->recordCount)
    {
        v8 = a5;
        v9 = a2->records;
        do
        {
            if (v9->collisionMapGameObject == a1
                && (v8 & v9->flags) == v8
                && (fabs(a4->x) == 0.0f // todo inline
                    && fabs(a4->y) == 0.0f
                    && fabs(a4->z) == 0.0f
                    || v9->normalVec.z * a4->z + a4->y * v9->normalVec.y + v9->normalVec.x * a4->x > 0.0f))
            {
                a3[v5] = v9->collisionInstanceGameObject;
                v8 = a5;
                ++v5;
            }
            ++v7;
            ++v9;
        } while (v7 < a2->recordCount);
    }
    return v5;
}

struct sGameObjectMain* COL_Report_GetBestWallGOInDirection(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, float a4, int* a5)
{
    int* v5;
    tRecordElem* v6;
    int v7;
    tRecordElem* v8;
    int v9;
    float v10;
    double v11; 
    tGameObjectMain* v12;
    tGameObjectMain* v14;
    Vec3 a1a;
    float v16;

    if (a4 == 0.0)
        v16 = 0.0;
    else
        v16 = (float)(0.001 - a4);
    v5 = a5;
    v6 = a2->records;
    v7 = 0;
    v8 = &a2->records[a2->recordCount];
    v14 = 0;
    if (a5)
        *a5 = 0;
    if (v6 >= v8)
        return 0;

    do
    {
        if (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
        {
            v9 = v6->flags;
            if ((v9 & 0x80u) != 0 && (v9 & 0x80052800) == 0)
            {
                if (v6->collisionInstanceGameObject == a1)
                {
                    a1a.x = a3->x;
                    v10 = a3->z;
                    a1a.y = a3->y;
                    a1a.z = v10;
                }
                else
                {
                    MATH_VecInvert(&a1a, a3);
                    v5 = a5;
                }
                v11 = v6->normalVec.z * a1a.z + v6->normalVec.y * a1a.y + a1a.x * v6->normalVec.x;
                if (v11 < v16)
                {
                    if (v6->collisionMapGameObject)
                    {
                        *v5 = v7;
                        v16 = (float)v11;
                        v12 = v6->collisionInstanceGameObject;
                        if (v6->collisionInstanceGameObject == a1)
                            v12 = v6->collisionMapGameObject;
                        v14 = v12;
                    }
                }
            }
        }
        ++v6;
        ++v7;
    } while (v6 < v8);
    return v14;
}

Vec3* COL_Report_GetCollidedPoint(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    int v5;
    tRecordElem* v6;
    tRecordElem* v7;
    unsigned int v8;
    int v9;
    int v10;

    if (a3 < 0)
        return &a2->records[(uint16_t)a3].collidedPoint;
    v5 = 1;
    v10 = 0;
    if ((a3 & 0x2000) != 0)
        v5 = 0;
    if ((a3 & 0x40000000) != 0)
        v10 = 1;
    v6 = a2->records;
    v7 = &a2->records[a2->recordCount];
    v8 = a3 & 0xBFFFFFFF;
    if (a2->records >= v7)
        return 0;
    while (1)
    {
        if ((!v5 || (v6->flags & 0x1000) == 0)
            && (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
            && (!v5 || (v6->flags & 0x2000) == 0))
        {
            v9 = v6->flags;
            if ((v9 & 0x80040000) == 0
                && (!v10 || (v9 & 0x800) == 0)
                && (v8 & v9) == v8
                && !MATH_b_NulVector(&v6->collidedPoint))
            {
                break;
            }
        }
        if (++v6 >= v7)
            return 0;
    }
    return &v6->collidedPoint;
}

Vec3* COL_Report_GMat_GetCollidedPoint(struct sGameObjectMain* a1, struct sWorld* a2, int a3, int* a4)
{
    tRecordElem* v4;
    int v5;
    tRecordElem* v6;
    tCobGameMaterialElems* v7;

    v4 = a2->records;
    v5 = 0;
    v6 = &a2->records[a2->recordCount];
    if (a4)
        *a4 = 0;
    if (v4 >= v6)
        return &MATH_gst_NulVector;
    while (1)
    {
        if ((v4->collisionInstanceGameObject == a1 || v4->collisionMapGameObject == a1) && (v4->flags & 0xF0000) == 0)
        {
            v7 = v4->gameMaterial;
            if (v7)
            {
                if ((a3 & v7->flags) != 0)
                    break;
            }
        }
        ++v4;
        ++v5;
        if (v4 >= v6)
            return &MATH_gst_NulVector;
    }
    if (a4)
        *a4 = v5;
    return &v4->collidedPoint;
}

int COL_Report_GetAllReportsNormal(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, int a4)
{
    int v4;
    int result;
    Vec3* v6;
    tRecordElem* v7;
    int v8;
    int v9;

    v4 = 0;
    result = 0;
    v9 = 0;
    if (a2->recordCount)
    {
        v6 = a3;
        v7 = a2->records;
        do
        {
            if (v7->collisionInstanceGameObject == a1 || v7->collisionMapGameObject == a1)
            {
                v8 = v7->flags;
                if ((v7->flags & 0x80043000) == 0
                    && ((a4 & 0x40000000) == 0 || (v8 & 0x800) == 0)
                    && (a4 & 0xBFFFFFFF & v8) == (a4 & 0xBFFFFFFF))
                {
                    v6->x = v7->normalVec.x;
                    v6->y = v7->normalVec.y;
                    v6->z = v7->normalVec.z;
                    ++v9;
                    ++v6;
                }
            }
            ++v4;
            ++v7;
        } while (v4 < a2->recordCount);
        result = v9;
    }
    return result;
}

int COL_VisibleObjectListCreate(struct sWorld* a1, int flagMustHave, int flagMustNothave, int whatToTest, struct sGameObjectMain** a5)
{
    tableElemStructPF* v5;
    tableElemStructPF* v6;
    int i;
    tGameObjectMain* v8;

    v5 = a1->visibleGameObjectsPF.firstElemPtr;
    v6 = ((tableElemStructPF*)(a1->visibleGameObjectsPF.nextElemPtr)) - 1;
    for (i = 0; v5 <= v6; ++v5)
    {
        v8 = (tGameObjectMain*)v5->objectPtr;
        if (v5->objectPtr
            && ((uint8_t)v8 & 3) != 1
            && v8->distance
            && OBJ_b_test_flag(v8, flagMustHave, flagMustNothave, whatToTest))
        {
            a5[i++] = v8;
        }
    }
    return i;
}

int COL_Report_GetNumberOfReports(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    int v3;
    tRecordElem *v4;
    int v5;
    int v6;

    v3 = 0;
    if (a2->recordCount)
    {
        v4 = a2->records;
        v5 = a2->recordCount;
        do
        {
            if (v4->collisionInstanceGameObject == a1 || v4->collisionMapGameObject == a1)
            {
                v6 = v4->flags;
                if ((v4->flags & 0x80043000) == 0
                    && ((a3 & 0x40000000) == 0 || (v6 & 0x800) == 0)
                    && (a3 & 0xBFFFFFFF & v6) == (a3 & 0xBFFFFFFF)
                    && ((a3 & 0x40) == 0 || v4->normalVec.z >= 0.0))
                {
                    ++v3;
                }
            }
            ++v4;
            --v5;
        } while (v5);
    }
    return v3;
}

void COL_Report_ComputeResultingNormal(struct sGameObjectMain* a1, struct sWorld* a2, int a3, Vec3* a4)
{
    int v4;
    tWorld* v5;
    tRecordElem* v6;
    int v7;
    long double v8;

    v4 = 0;
    if (a4)
    {
        v5 = a2;
        a4->x = 0.0;
        a4->y = 0.0;
        a4->z = 0.0;
        if (a2->recordCount)
        {
            v6 = a2->records;
            do
            {
                if (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
                {
                    v7 = v6->flags;
                    if ((v6->flags & 0x80043000) == 0
                        && ((a3 & 0x40000000) == 0 || (v7 & 0x800) == 0)
                        && (a3 & 0xBFFFFFFF & v7) == (a3 & 0xBFFFFFFF)
                        && ((a3 & 0x40) == 0 || v6->normalVec.z >= 0.0))
                    {
                        MATH_VecAddVecWithoutBuffer(a4, &v6->normalVec);
                        v5 = a2;
                    }
                }
                ++v4;
                ++v6;
            } while (v4 < v5->recordCount);
        }
        if (fabs(a4->x) != 0.0 || fabs(a4->y) != 0.0 || fabs(a4->z) != 0.0)
        {
            v8 = sqrt(a4->x * a4->x + a4->y * a4->y + a4->z * a4->z);
            a4->x = (float)(1.0 / v8 * a4->x);
            a4->y = (float)(1.0 / v8 * a4->y);
            a4->z = (float)(1.0 / v8 * a4->z);
        }
    }
}

struct sGameObjectMain* COL_Report_GetBestWallGOInDirectionSpecial(struct sGameObjectMain* a1, struct sWorld* a2, Vec3* a3, float a4, int* a5)
{
    int* v5;
    tRecordElem* v6;
    int v7;
    tRecordElem* v8;
    int v9;
    float v10;
    double v11;
    tGameObjectMain* v12;
    tGameObjectMain* v14;
    Vec3 a1a;
    float v16;

    if (a4 == 0.0)
        v16 = 0.0;
    else
        v16 = (float)(0.001 - a4);
    v5 = a5;
    v6 = a2->records;
    v7 = 0;
    v8 = &a2->records[a2->recordCount];
    v14 = 0;
    if (a5)
        *a5 = 0;
    if (v6 >= v8)
        return 0;
    do
    {
        if (v6->collisionInstanceGameObject == a1 || v6->collisionMapGameObject == a1)
        {
            v9 = v6->flags;
            if ((v9 & 0x80u) != 0 && (v9 & 0x80072000) == 0)
            {
                if (v6->collisionInstanceGameObject == a1)
                {
                    a1a.x = a3->x;
                    v10 = a3->z;
                    a1a.y = a3->y;
                    a1a.z = v10;
                }
                else
                {
                    MATH_VecInvert(&a1a, a3);
                    v5 = a5;
                }
                v11 = v6->normalVec.z * a1a.z + v6->normalVec.y * a1a.y + a1a.x * v6->normalVec.x;
                if (v11 < v16)
                {
                    if (v6->collisionMapGameObject)
                    {
                        *v5 = v7;
                        v16 = (float)v11;
                        v12 = v6->collisionInstanceGameObject;
                        if (v6->collisionInstanceGameObject == a1)
                            v12 = v6->collisionMapGameObject;
                        v14 = v12;
                    }
                }
            }
        }
        ++v6;
        ++v7;
    } while (v6 < v8);
    return v14;
}

int16_t COL_GameMaterial_GetID(struct sGameObjectMain* a1, struct sWorld* a2, uint16_t a3)
{
    int16_t result;
    tCobGameMaterialElems* v4;
    tCobGameMaterialElems* v5;
    tRecordElem* v6;
    tRecordElem* v7;
    int v8;

    if (!a2)
        return -1;
    if (a3 == 0xFFFF)
    {
        v4 = a2->rayCastCobGmat;
        if (v4)
            return v4->id;
        return -1;
    }
    if ((int)a3 >= 0)
    {
        v6 = a2->records;
        v7 = &a2->records[a2->recordCount];
        if (a2->records >= v7)
        {
        LABEL_19:
            result = -1;
        }
        else
        {
            while (1)
            {
                if (((a3 & 0x2000) != 0 || (v6->flags & 0x1000) == 0)
                    && v6->collisionInstanceGameObject == a1
                    && ((a3 & 0x2000) != 0 || (v6->flags & 0x2000) == 0))
                {
                    v8 = v6->flags;
                    if ((v8 & 0x80040000) == 0 && (a3 & v8) == a3 && v6->gameMaterial)
                        break;
                }
                if (++v6 >= v7)
                    goto LABEL_19;
            }
            result = v6->gameMaterial->id;
        }
    }
    else
    {
        v5 = a2->records[a3].gameMaterial;
        if (!v5)
            return -1;
        result = v5->id;
    }
    return result;
}

int COL_Report_GetNumberOfReportsAndRanks(struct sGameObjectMain* a1, struct sWorld* a2, int a3, unsigned int a4, int* a5)
{
    int v5;
    unsigned int v6;
    tRecordElem* v7;
    int v8;
    unsigned int v9;
    BOOL v12;
    unsigned int v13;
    unsigned int v14;

    v5 = 0;
    v6 = 0;
    v12 = (a3 & 0x40000000) != 0;
    v14 = a3 & ~0x40000000u;
    v13 = 0;
    if (a2->recordCount)
    {
        v7 = a2->records;
        do
        {
            if (v7->collisionInstanceGameObject == a1 || v7->collisionMapGameObject == a1)
            {
                v8 = v7->flags;
                if ((v7->flags & 0x80043000) == 0 && (!v12 || (v8 & 0x800) == 0) && (v14 & v8) == v14)
                {
                    v9 = 0;
                    if (v6)
                    {
                        do
                        {
                            if (a2->records[a5[v9]].triangleChunkID == v7->triangleChunkID
                                && a2->records[a5[v9]].triangleChunkElemID == v7->triangleChunkElemID)
                            {
                                break;
                            }
                            ++v9;
                        } while (v9 < v6);
                        v5 = v13;
                    }
                    if (v9 == v6 && ((v14 & 0x40) == 0 || v7->normalVec.z >= 0.0))
                    {
                        if (v6 < a4)
                            a5[v6] = v5;
                        ++v6;
                    }
                }
            }
            ++v5;
            ++v7;
            v13 = v5;
        } while (v5 < a2->recordCount);
    }
    return v6;
}

int COL_Report_b_ObjectsAreColliding(struct sGameObjectMain* a1, struct sGameObjectMain* a2, struct sWorld* a3)
{
    int v3;
    int v4;
    tRecordElem* i;
    tGameObjectMain* v6;

    v3 = a3->recordCount;
    v4 = 0;
    if (!v3)
        return 0;
    for (i = a3->records; ; ++i)
    {
        if ((i->flags & 0x80043000) == 0)
        {
            v6 = i->collisionInstanceGameObject;
            if (v6 == a1 && i->collisionMapGameObject == a2)
                break;
            if (v6 == a2 && i->collisionMapGameObject == a1)
                break;
        }
        if (++v4 >= v3)
            return 0;
    }
    return 1;
}

struct sGameObjectMain* COL_Visual_RayCast_OneObject(struct sGameObjectMain* a1, Vec3* rayV1, Vec3* rayV2, float rayF1, int flagMustHave, int flagMustNothave, int whatToTest, int* type7, int type1, int a10, int type6)
{
    Vec3* v11;
    Vec3* v12;
    int v13;
    tGEOGRO* v14;
    tGameObjectMain* result;
    tColMap* v16;
    tColCollisionObject* v17;
    uint8_t i;
    tColCollisionObject* v19;
    tCobNormalObjectGeneric* v20;
    int v21;
    int v22;
    float rayv1a;

    v11 = rayV2;
    v12 = rayV1;
    COL_gst_Ray.ignoreColMapRayCastResult = type7;
    COL_gst_Ray.sourcePoint = rayV1;
    COL_gst_Ray.direction = rayV2;
    COL_gst_Ray.gao = 0;
    COL_gst_Ray.distance = rayF1;
    COL_gst_Ray.skipRayCast = type1;
    COL_gst_Ray.skipVecsTransformationToGaoSpace = type6;
    if (!a1
        || ((uint8_t)a1 & 3) == 1
        || !OBJ_b_test_flag(a1, flagMustHave, flagMustNothave, whatToTest)
        || (a1->flags & 0x4000) == 0 && (a1->flags & 2) == 0)
    {
        return 0;
    }
    if (!a10
        && (!OBJ_BV_IsSphere(a1->boundingVolume) || !COL_RaySphereBV(v12, v11, a1, &rayv1a, COL_gst_Ray.distance))
        && (!OBJ_BV_IsAABBox(a1->boundingVolume) || !COL_RayAABBox(v12, v11, a1, &rayv1a, COL_gst_Ray.distance)))
    {
    LABEL_37:
        result = COL_gst_Ray.gao;
        COL_gb_UseMinDistance = 0;
        return result;
    }
    v13 = a1->flags;
    if ((a1->flags & 0x4000) == 0)
    {
        if ((v13 & 2) != 0 && (v13 & 0x100) != 0)
        {
            v16 = a1->extended->coll->colMap;
            v17 = *v16->collisionObjectPtr;
            if (v17->objectType == 5)
            {
                if ((v16->activation & 1) == 0)
                    return 0;
                if (((tCobMeshObjectMesh*)(v17->objectPtrByType))->ok3)
                    COL_ColMap_OK3_RayCast(a1, v16);
                else
                    COL_ColMap_Triangles_RayCast(a1, v16);
                if (COL_gst_Ray.ignoreColMapRayCastResult && !*COL_gst_Ray.ignoreColMapRayCastResult)
                    return 0;
            }
            else
            {
                for (i = 0; i < v16->colObjectID; ++i)
                {
                    if (((uint8_t)(1 << i) & v16->activation) != 0)
                    {
                        v19 = v16->collisionObjectPtr[i];
                        v20 = (tCobNormalObjectGeneric*)(v19->objectPtrByType);
                        v21 = v19->objectType - 1;
                        if (v21)
                        {
                            v22 = v21 - 1;
                            if (v22)
                            {
                                if (v22 == 1)
                                    COL_ColMap_Cylinder_RayCast(a1, v20);
                            }
                            else
                            {
                                COL_ColMap_Sphere_RayCast(a1, v20);
                            }
                        }
                        else
                        {
                            COL_ColMap_Box_RayCast(a1, v20);
                        }
                    }
                }
            }
        }
        goto LABEL_37;
    }
    if ((((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask & 8) == 0)
        return 0;
    v14 = OBJ_p_GetCurrentGeo(a1);
    if (!v14)
        return 0;
    //nullsub_32();
    if (v14->collisionPtr)
        COL_Visual_OK3_RayCast(a1, v14);
    else
        COL_Visual_Triangles_RayCast(a1, v14);
    //nullsub_32();
    result = COL_gst_Ray.gao;
    COL_gb_UseMinDistance = 0;
    return result;
}

int COL_Report_ul_BestReportGet(struct sGameObjectMain* a1, struct sWorld* a2, int a3, int a4)
{
    unsigned int v4;
    int v5;
    unsigned int v6;
    tRecordElem* v7;
    int v8;
    double v9;
    float v11;

    v4 = a2->recordCount;
    v5 = -1;
    v6 = 0;
    v11 = -1.0;
    if (v4)
    {
        v7 = a2->records;
        do
        {
            v8 = v7->flags;
            if ((v7->flags & 0x80001000) == 0
                && (v8 & a3) != 0
                && (v8 & a4) == 0
                && (v7->collisionInstanceGameObject == a1 || v7->collisionMapGameObject == a1))
            {
                v9 = v7->recalVec.z * v7->recalVec.z + v7->recalVec.x * v7->recalVec.x + v7->recalVec.y * v7->recalVec.y;
                if (v9 > v11)
                {
                    v11 = (float)v9;
                    v5 = v6;
                }
            }
            ++v6;
            ++v7;
        } while (v6 < v4);
    }
    return v5;
}

BOOL COL_Report_HasGMat(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    tRecordElem* v4;
    tRecordElem* v5;
    int v6;

    if (a3 < 0)
        return a2->records[(uint16_t)a3].gameMaterial != 0;
    v4 = a2->records;
    v5 = &a2->records[a2->recordCount];
    if (a2->records >= v5)
        return 0;
    while (1)
    {
        if ((v4->collisionInstanceGameObject == a1 || v4->collisionMapGameObject == a1)
            && ((a3 & 0x2000) != 0 || (v4->flags & 0x3000) == 0))
        {
            v6 = v4->flags;
            if ((v6 & 0x80040000) == 0 && (a3 & v6) == a3)
                break;
        }
        if (++v4 >= v5)
            return 0;
    }
    return v4->gameMaterial != 0;
}

uint8_t COL_GetDesign(struct sGameObjectMain* a1, struct sWorld* a2, int32_t a3)
{
    tRecordElem* v4;
    tRecordElem* v5;
    int v6;

    if (!a2)
        return 0;
    if (a3 == 0xFFFF)
        return a2->rayCastCobMeshChunkElemsDesignVal;
    if ((a3 & 0x80000000) != 0)
        return a2->records[(uint16_t)a3].designVal;
    v4 = a2->records;
    v5 = &a2->records[a2->recordCount];
    if (a2->records >= v5)
        return 0;
    while (1)
    {
        if (((a3 & 0x2000) != 0 || (v4->flags & 0x1000) == 0)
            && v4->collisionInstanceGameObject == a1
            && ((a3 & 0x2000) != 0 || (v4->flags & 0x2000) == 0))
        {
            v6 = v4->flags;
            if ((v6 & 0x80040000) == 0 && (a3 & v6) == a3)
                break;
        }
        if (++v4 >= v5)
            return 0;
    }
    return v4->designVal;
}

int COL_ZDE_PivotCollide(struct sGameObjectMain* a1, uint8_t id, struct sGameObjectMain* a3, int direction)
{
    tGameObjectMain* v4;
    tGameObjectExtended* v5;
    tGameObjectCollMain* v6;
    tColInstance* v7;
    uint8_t v8;
    BOOL v9;
    tColInstanceElem* v10;
    Mtx4x4* v11;
    float v12;
    Mtx4x4* v13;
    Mtx4x4* v14;
    double v15; 
    Mtx4x4* v16;
    tColBox* v18;
    Mtx4x4* v19; 
    float v20; 
    Mtx4x4* v21;
    Mtx4x4* v22;
    Vec3* v23;
    tColSphere* v24;
    tGameObjectMain* a1a; 
    float a1b;
    Vec3 v27; 
    Vec3 a3a; 
    int v29; 
    Vec3 v30;
    Mtx4x4 a2a; 
    Mtx4x4 v32; 

    if (a1 && a3 && (a1->flags2 & 1) != 0 && (a3->flags2 & 1) != 0)
    {
        if (direction)
        {
            v4 = a3;
            a1a = a1;
        }
        else
        {
            v4 = a1;
            a1a = a3;
        }
        if ((v4->flags & 0x600) != 0)
        {
            v5 = v4->extended;
            if (v5)
            {
                v6 = v5->coll;
                if (v6)
                {
                    v7 = v6->colInstance;
                    if (v7)
                    {
                        if (v7->colSet)
                        {
                            v8 = v7->colSet->zoneIDs[id];
                            v9 = v8 < v7->zoneNum;
                            v29 = v8; // byte cast
                            if (v9)
                            {
                                if (COL_get_activation_flag_for_id__max_16_sub_4132C0(v7, v29))
                                {
                                    v10 = v7->selectedInstElemPtrs[v8];
                                    if ((v10->colFlags & 4) != 0)
                                    {
                                        if (v10->elemType == 1)
                                        {
                                            v18 = (tColBox*)v10->elemShapePtr;
                                            if (v18->pos2.x != v18->pos1.x && v18->pos2.y != v18->pos1.y && v18->pos2.z != v18->pos1.z)
                                            {
                                                v19 = OBJ_pst_GetAbsoluteMatrix(a1a);
                                                v20 = v19->Pos.x;
                                                a3a.x = v20;
                                                a3a.y = v19->Pos.y;
                                                a3a.z = v19->Pos.z;
                                                if ((OBJ_pst_GetAbsoluteMatrix(v4)->mtxflag & 8) != 0 && (v10->colFlags & 8) != 0)
                                                {
                                                    v21 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                    MATH_CopyMatrix(&a2a, v21);
                                                    a2a.mtxflag &= ~8u;
                                                    a2a._m14 = 0.0;
                                                    a2a._m24 = 0.0;
                                                    a2a._m34 = 0.0;
                                                    MATH_InvertMatrix(&v32, &a2a);
                                                }
                                                else
                                                {
                                                    v22 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                    MATH_InvertMatrix(&v32, v22);
                                                }
                                                MATH_TransformVertex(&v27, &v32, &a3a);
                                                if (v27.x > (double)v18->pos2.x
                                                    && v27.x < (double)v18->pos1.x
                                                    && v27.y >(double)v18->pos2.y
                                                    && v27.y < (double)v18->pos1.y
                                                    && v27.z >(double)v18->pos2.z
                                                    && v27.z < (double)v18->pos1.z)
                                                {
                                                    return 1;
                                                }
                                            }
                                        }
                                        else if (v10->elemType == 2 && ((tColSphere*)(v10->elemShapePtr))->radius != 0.0)
                                        {
                                            v11 = (Mtx4x4*)OBJ_pst_GetAbsoluteMatrix(a1a);
                                            v12 = v11->Pos.x;
                                            a3a.x = v12;
                                            a3a.y = v11->Pos.y;
                                            a3a.z = v11->Pos.z;
                                            if ((OBJ_pst_GetAbsoluteMatrix(v4)->mtxflag & 8) == 0 || (v10->colFlags & 8) != 0)
                                            {
                                                v24 = (tColSphere*)(v10->elemShapePtr);
                                                v16 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                MATH_TransformVertexNoScale(&v30, v16, &v24->center);
                                                v15 = ((tColSphere*)(v10->elemShapePtr))->radius;
                                            }
                                            else
                                            {
                                                v13 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                MATH_GetScale(&v27, v13);
                                                v23 = &(((tColSphere*)(v10->elemShapePtr))->center);
                                                v14 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                MATH_TransformVertex(&v30, v14, v23);
                                                v15 = MATH_f_MaxOf3(v27.x, v27.y, v27.z) * ((tColSphere*)(v10->elemShapePtr))->radius;
                                            }
                                            a1b = (float)v15;
                                            v27.x = a3a.x - v30.x;
                                            v27.y = a3a.y - v30.y;
                                            v27.z = a3a.z - v30.z;
                                            if (a1b * a1b > MATH_VecLenSquared(&v27))
                                                return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void COL_Zone_Transform(tColInstanceElem* a1, tGameObjectMain* a2, Vec3* dest, Vec3* a4)
{
    Mtx4x4* v4;
    int v5; 
    tColBox* v6;
    double v7;
    long double v8;
    long double v9;
    float v10;
    Vec3 a3a; 
    Vec3 v12; 
    Vec3 src; 
    Vec3 scale; 

    v4 = OBJ_pst_GetAbsoluteMatrix(a2);
    v5 = v4->mtxflag & 8;
    if (a1->elemType == 1)
    {
        v6 = (tColBox*)a1->elemShapePtr;
        a3a.x = v6->pos1.x + v6->pos2.x;
        a3a.y = v6->pos2.y + v6->pos1.y;
        v7 = v6->pos2.z + v6->pos1.z;
        a3a.x = (float)(a3a.x * 0.5);
        a3a.y = (float)(a3a.y * 0.5);
        a3a.z = (float)(v7 * 0.5);
        src.x = v6->pos1.x - a3a.x;
        src.y = v6->pos1.y - a3a.y;
        src.z = v6->pos1.z - a3a.z;
        if (!v5 || (a1->colFlags & 8) != 0)
        {
            MATH_TransformVertexNoScale(&v12, v4, &a3a);
            MATH_TransformVectorNoScale(&scale, v4, &src);
        }
        else
        {
            MATH_TransformVertex(&v12, v4, &a3a);
            MATH_TransformVector(&scale, v4, &src);
        }
        v8 = fabs(scale.x);
        dest->x = (float)(v12.x - v8);
        v9 = fabs(scale.y);
        dest->y = (float)(v12.y - v9);
        v10 = (float)(fabs(scale.z));
        dest->z = v12.z - v10;
        a4->x = (float)(v8 + v12.x);
        a4->y = (float)(v9 + v12.y);
        a4->z = v10 + v12.z;
    }
    else if (a1->elemType == 2)
    {
        if ((v4->mtxflag & 8) == 0 || (a1->colFlags & 8) != 0)
        {
            MATH_TransformVertexNoScale(dest, v4, &(((tColSphere*)(a1->elemShapePtr))->center));
            a4->x = ((tColSphere*)(a1->elemShapePtr))->radius;
        }
        else
        {
            MATH_GetScale(&scale, v4);
            MATH_TransformVertex(dest, v4, &(((tColSphere*)(a1->elemShapePtr))->center));
            a4->x = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * ((tColSphere*)(a1->elemShapePtr))->radius;
        }
    }
}

BOOL COL_ZDE_BVCollide(struct sGameObjectMain* a1, uint8_t a2, struct sGameObjectMain* a3, int a4)
{
    tGameObjectMain* v4;
    tGameObjectMain* v5;
    tGameObjectExtended* v6;
    tGameObjectCollMain* v7;
    tColInstance* v8;
    uint8_t* v9;
    BOOL v10;
    tColInstanceElem* v11;
    BOOL v12;
    int v13;
    int v15;
    Mtx4x4* v16;
    tGameObjectBV* v17;
    Vec3* v18;
    Mtx4x4* v19;
    Mtx4x4* v20;
    Vec3* v21;
    tGameObjectBV* v22;
    Mtx4x4* v23;
    Mtx4x4* v24;
    Mtx4x4* v25;
    Mtx4x4* mtx;
    uint8_t v27a;
    Vec3 v27b;
    tGameObjectMain* v28;
    Vec3 a3a;
    Vec3 src;
    Vec3 v31;
    Vec3 v32;
    Vec3 v33;
    Vec3 v34;
    Vec3 a4a;
    Vec3 v36;
    Vec3 v37;
    Mtx4x4 v38;
    Vec3 dest;
    Mtx4x4 a5;

    v4 = a1;
    if (!a1 || !a3 || (a1->flags2 & 1) == 0 || (a3->flags2 & 1) == 0)
        return 0;
    if (a4)
    {
        v5 = a3;
        v28 = a1;
    }
    else
    {
        v5 = a1;
        v28 = a3;
        v4 = a3;
    }
    if ((v5->flags & 0x600) == 0)
        return 0;
    v6 = v5->extended;
    if (!v6)
        return 0;
    v7 = v6->coll;
    if (!v7)
        return 0;
    v8 = v7->colInstance;
    if (!v8)
        return 0;
    if (!v8->colSet)
        return 0;
    v9 = v8->colSet->zoneIDs;
    v10 = v9[a2] < v8->zoneNum;
    v27a = v9[a2];
    if (!v10)
        return 0;
    if (!COL_get_activation_flag_for_id__max_16_sub_4132C0(v8, v27a))
        return 0;
    v11 = v8->selectedInstElemPtrs[v27a];
    if ((v11->colFlags & 4) == 0)
        return 0;
    if (v11->elemType == 1)
    {
        COL_Zone_Transform(v11, v5, &dest, &a4a);
    }
    else if (v11->elemType == 2)
    {
        COL_Zone_Transform(v11, v5, &v34, &v27b);
    }
    mtx = OBJ_pst_GetAbsoluteMatrix(v4);
    v12 = !OBJ_BV_IsSphere(v4->boundingVolume);
    v13 = v11->elemType;
    if (!v12)
    {
        if (v13 == 2)
        {
            MATH_TransformVertexNoScale(&v31, mtx, &v4->boundingVolume->BVMin);
            return INT_SphereSphere(&v34, v27b.x, &v31, v4->boundingVolume->BVMax.x);
        }
        return 0;
    }
    v15 = v13 - 1;
    if (v15)
    {
        if (v15 == 1) // elemtype == 2 ez is
        {
            v16 = v4->matrices;
            v17 = v4->boundingVolume;
            v18 = &v16->Pos;
            MATH_VecAddVec(&v37, &v17->BVMax, &v16->Pos);
            MATH_VecAddVec(&v36, &v17->BVMin, v18);
            return INT_SphereAABBox(&v34, v27b.x, &v36, &v37);
        }
        return 0;
    }
    if ((OBJ_pst_GetAbsoluteMatrix(v5)->mtxflag & 8) != 0)
    {
        v19 = OBJ_pst_GetAbsoluteMatrix(v5);
        MATH_CopyMatrix(&v38, v19);
        v38._m14 = 0.0;
        v38._m24 = 0.0;
        v38._m34 = 0.0;
        v38.mtxflag &= ~8u;
        MATH_InvertMatrix(&a5, &v38);
    }
    else
    {
        v20 = OBJ_pst_GetAbsoluteMatrix(v5);
        MATH_InvertMatrix(&a5, v20);
    }
    v21 = (Vec3*)v11->elemShapePtr;
    v22 = v4->boundingVolume;
    MATH_VecAddVec(&a3a, v21, v21 + 1);
    a3a.x = (float)(a3a.x * 0.5);
    a3a.y = (float)(a3a.y * 0.5);
    a3a.z = (float)(a3a.z * 0.5);
    v23 = OBJ_pst_GetAbsoluteMatrix(v5);
    MATH_TransformVertex(&v31, v23, &a3a);
    MATH_VecAddVec(&src, &v22->BVMax, &v22->BVMin);
    src.x = (float)(src.x * 0.5);
    src.y = (float)(src.y * 0.5);
    src.z = (float)(src.z * 0.5);
    v24 = OBJ_pst_GetAbsoluteMatrix(v28);
    MATH_TransformVertex(&v33, v24, &src);
    v32.x = v33.x - v31.x;
    v32.y = v33.y - v31.y;
    v32.z = v33.z - v31.z;
    a5.Pos = v32;
    v25 = OBJ_pst_GetAbsoluteMatrix(v5);
    MATH_GetScale(&a4a, v25);
    return INT_HighLevel_OBBoxOBBox(v21 + 1, v21, &v22->BVMin, &v22->BVMax, &a5, &a4a);
}

int COL_ZDE_BVCenterCollide(struct sGameObjectMain* a1, uint8_t a2, struct sGameObjectMain* a3, int a4)
{
    tGameObjectMain* v4;
    tGameObjectExtended* v5;
    tGameObjectCollMain* v6;
    tColInstance* v7;
    uint8_t v8; 
    BOOL v9; 
    tColInstanceElem* v10;
    Mtx4x4* v11;
    Mtx4x4* v12;
    double v13; 
    Mtx4x4* v14; 
    tColBox* v16;
    Mtx4x4* v17; 
    Mtx4x4* v18; 
    Vec3* v19; 
    Vec3* v20; 
    tGameObjectMain* a1a;
    float a1b;
    Vec3 v23; 
    Vec3 a2a; 
    int v25; 
    Vec3 v26;
    Mtx4x4 v27;
    Mtx4x4 v28;

    if (a1 && a3 && (a1->flags2 & 1) != 0 && (a3->flags2 & 1) != 0)
    {
        if (a4)
        {
            v4 = a3;
            a1a = a1;
        }
        else
        {
            v4 = a1;
            a1a = a3;
        }
        if ((v4->flags & 0x600) != 0)
        {
            v5 = v4->extended;
            if (v5)
            {
                v6 = v5->coll;
                if (v6)
                {
                    v7 = v6->colInstance;
                    if (v7)
                    {
                        if (v7->colSet)
                        {
                            v8 = v7->colSet->zoneIDs[a2];
                            v9 = v8 < v7->zoneNum;
                            v25 = v8; // byte assign
                            if (v9)
                            {
                                if (COL_get_activation_flag_for_id__max_16_sub_4132C0(v7, v25))
                                {
                                    v10 = v7->selectedInstElemPtrs[v8];
                                    if ((v10->colFlags & 4) != 0)
                                    {
                                        if (v10->elemType == 1)
                                        {
                                            v16 = (tColBox*)v10->elemShapePtr;
                                            if (v16->pos2.x != v16->pos1.x && v16->pos2.y != v16->pos1.y && v16->pos2.z != v16->pos1.z)
                                            {
                                                OBJ_BV_ComputeCenter(a1a, &a2a);
                                                if ((OBJ_pst_GetAbsoluteMatrix(v4)->mtxflag & 8) == 0 || (v10->colFlags & 8) != 0)
                                                {
                                                    v18 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                    MATH_InvertMatrix(&v28, v18);
                                                }
                                                else
                                                {
                                                    v17 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                    MATH_CopyMatrix(&v27, v17);
                                                    v27.mtxflag &= ~8u;
                                                    v27._m14 = 0.0;
                                                    v27._m24 = 0.0;
                                                    v27._m34 = 0.0;
                                                    MATH_InvertMatrix(&v28, &v27);
                                                }
                                                MATH_TransformVertex(&v23, &v28, &a2a);
                                                if (v23.x > (double)v16->pos2.x
                                                    && v23.x < (double)v16->pos1.x
                                                    && v23.y >(double)v16->pos2.y
                                                    && v23.y < (double)v16->pos1.y
                                                    && v23.z >(double)v16->pos2.z
                                                    && v23.z < (double)v16->pos1.z)
                                                {
                                                    return 1;
                                                }
                                            }
                                        }
                                        else if (v10->elemType == 2 && ((tColSphere*)(v10->elemShapePtr))->radius != 0.0)
                                        {
                                            OBJ_BV_ComputeCenter(a1a, &a2a);
                                            if ((OBJ_pst_GetAbsoluteMatrix(v4)->mtxflag & 8) == 0 || (v10->colFlags & 8) != 0)
                                            {
                                                v20 = &(((tColSphere*)(v10->elemShapePtr))->center);
                                                v14 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                MATH_TransformVertexNoScale(&v26, v14, v20);
                                                v13 = ((tColSphere*)(v10->elemShapePtr))->radius;
                                            }
                                            else
                                            {
                                                v11 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                MATH_GetScale(&v23, v11);
                                                v19 = &(((tColSphere*)(v10->elemShapePtr))->center);
                                                v12 = OBJ_pst_GetAbsoluteMatrix(v4);
                                                MATH_TransformVertex(&v26, v12, v19);
                                                v13 = MATH_f_MaxOf3(v23.x, v23.y, v23.z)
                                                    * ((tColSphere*)(v10->elemShapePtr))->radius;
                                            }
                                            a1b = (float)v13;
                                            v23.x = a2a.x - v26.x;
                                            v23.y = a2a.y - v26.y;
                                            v23.z = a2a.z - v26.z;
                                            if (a1b * a1b > MATH_VecLenSquared(&v23))
                                                return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

BOOL COL_ZDE_ZDECollide(struct sGameObjectMain* a1, uint8_t a2, uint8_t a3, struct sGameObjectMain* a4)
{
    tGameObjectExtended* v4;
    tGameObjectCollMain* v5;
    tColInstance* v6;
    tGameObjectExtended* v7;
    tGameObjectCollMain* v8;
    tColInstance* v9;
    uint8_t* v10;
    BOOL v11;
    tColInstanceElem* v12;
    int v13;
    tColInstanceElem* v14;
    BOOL result;
    BOOL v16; 
    tColBox* v17;
    Mtx4x4* v18; 
    Mtx4x4* v19; 
    Mtx4x4* v20; 
    Mtx4x4* v21; 
    Mtx4x4* v22; 
    Mtx4x4* v23; 
    Mtx4x4* v24; 
    Mtx4x4* v25; 
    tColBox* v26; 
    Mtx4x4* v27; 
    Mtx4x4* v28; 
    Mtx4x4* v29; 
    Mtx4x4* v30; 
    Mtx4x4* v31; 
    Mtx4x4* v32; 
    Mtx4x4* v33; 
    Mtx4x4* v34; 
    Mtx4x4* v35; 
    Mtx4x4* v36; 
    tColBox* v37;
    tColBox* v38;
    Mtx4x4* v39; 
    Mtx4x4* v40; 
    Mtx4x4* v41; 
    Vec3* v42; 
    Vec3* v43; 
    Vec3* v44; 
    Vec3* v45; 
    float a4a; 
    float v47; 
    tColBox* v48;
    Vec3 v49; 
    Vec3 v50; 
    Vec3 v51; 
    Vec3 v52; 
    Vec3 dest;
    Vec3 a3a; 
    Mtx4x4 v55; 
    Vec3 a6; 
    Mtx4x4 a5;
    Mtx4x4 v58;
    uint8_t a4ab;
    Vec3 a4aa;
    Vec3 v48a;

    if (!a1)
        return 0;
    if (!a4)
        return 0;
    if ((a1->flags2 & 1) == 0)
        return 0;
    if ((a4->flags2 & 1) == 0)
        return 0;
    if ((a1->flags & 0x600) == 0)
        return 0;
    v4 = a1->extended;
    if (!v4)
        return 0;
    v5 = v4->coll;
    if (!v5)
        return 0;
    v6 = v5->colInstance;
    if (!v6)
        return 0;
    if ((a4->flags & 0x600) == 0)
        return 0;
    v7 = a4->extended;
    if (!v7)
        return 0;
    v8 = v7->coll;
    if (!v8)
        return 0;
    v9 = v8->colInstance;
    if (!v9)
        return 0;
    if (!v6->colSet)
        return 0;
    v10 = v6->colSet->zoneIDs;
    v11 = v10[a2] < v6->zoneNum;
    a4ab = v10[a2];
    if (!v11)
        return 0;
    if (!COL_get_activation_flag_for_id__max_16_sub_4132C0(v6, a4ab))
        return 0;
    v12 = v6->selectedInstElemPtrs[a4ab];
    if ((v12->colFlags & 4) == 0)
        return 0;
    v13 = v9->zoneNum;
    a4ab = v9->colSet->zoneIDs[a3];
    if (a4ab > v13 - 1)
        return 0;
    if (!COL_get_activation_flag_for_id__max_16_sub_4132C0(v9, a4ab))
        return 0;
    v14 = v9->selectedInstElemPtrs[a4ab];
    if ((v14->colFlags & 4) == 0)
        return 0;
    switch (v14->elemType + 16 * v12->elemType)
    {
    case 17:
        if ((OBJ_pst_GetAbsoluteMatrix(a1)->mtxflag & 8) != 0)
        {
            v34 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_CopyMatrix(&v55, v34);
            v55._m14 = 0.0;
            v55._m24 = 0.0;
            v55._m34 = 0.0;
            v55.mtxflag &= ~8u;
            MATH_InvertMatrix(&v58, &v55);
        }
        else
        {
            v35 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_InvertMatrix(&v58, v35);
        }
        v36 = OBJ_pst_GetAbsoluteMatrix(a4);
        v36 = OBJ_pst_GetAbsoluteMatrix(a4);
        MATH_MulMatrixMatrix(&a5, v36, &v58);
        v37 = (tColBox*)v12->elemShapePtr;
        v38 = (tColBox*)v14->elemShapePtr;
        MATH_VecAddVec(&v49, &v37->pos1, &v37->pos2);
        v49.x = (float)(v49.x * 0.5);
        v49.y = (float)(v49.y * 0.5);
        v49.z = (float)(v49.z * 0.5);
        v39 = OBJ_pst_GetAbsoluteMatrix(a1);
        MATH_TransformVertex(&a3a, v39, &v49);
        MATH_VecAddVec(&v50, &v38->pos1, &v38->pos2);
        v50.x = (float)(v50.x * 0.5);
        v50.y = (float)(v50.y * 0.5);
        v50.z = (float)(v50.z * 0.5);
        v40 = OBJ_pst_GetAbsoluteMatrix(a4);
        MATH_TransformVertex(&dest, v40, &v50);
        v52.x = dest.x - a3a.x;
        v52.y = dest.y - a3a.y;
        v52.z = dest.z - a3a.z;
        MATH_TransformVector(&v51, &v58, &v52);
        a5.Pos = v51;
        v41 = OBJ_pst_GetAbsoluteMatrix(a1);
        MATH_GetScale(&a6, v41);
        result = INT_HighLevel_OBBoxOBBox(&v37->pos2, &v37->pos1, &v38->pos2, &v38->pos1, &a5, &a6);
        break;
    case 18: // v12 = 1, //v14 = 2
        if ((OBJ_pst_GetAbsoluteMatrix(a1)->mtxflag & 8) != 0)
        {
            v16 = (v12->colFlags & 8) == 0;
            v26 = (tColBox*)v12->elemShapePtr;
            v48 = v26;
            if (v16)
            {
                v27 = OBJ_pst_GetAbsoluteMatrix(a1);
                MATH_GetScale(&v51, v27);
                v49.x = v51.x * v48->pos2.x;
                v49.y = v51.y * v48->pos2.y;
                v49.z = v51.z * v48->pos2.z;
                v50.x = v51.x * v48->pos1.x;
                v50.y = v51.y * v48->pos1.y;
                v50.z = v51.z * v48->pos1.z;
            }
            else
            {
                v49 = v26->pos2;
                v50 = v26->pos1;
            }
            v28 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_Transp33MatrixWithoutBuffer(&v55, v28);
            v55.mtxflag = v28->mtxflag;
            v55._m14 = (float)(1.0 / v28->_m14);
            v55._m24 = (float)(1.0 / v28->_m24);
            v55._m34 = (float)(1.0 / v28->_m34);
            MATH_VecInvert(&v55.Pos, &v28->Pos);
            MATH_TransformVectorNoScale(&v55.Pos, &v55, &v55.Pos);
            v44 = (Vec3*)v14->elemShapePtr;
            if ((v14->colFlags & 8) != 0)
            {
                v30 = OBJ_pst_GetAbsoluteMatrix(a4);
                MATH_TransformVertexNoScale(&v52, v30, v44);
            }
            else
            {
                v29 = OBJ_pst_GetAbsoluteMatrix(a4);
                MATH_TransformVertex(&v52, v29, v44);
            }
            MATH_TransformVertexNoScale(&a3a, &v55, &v52);
            v16 = (v14->colFlags & 8) == 0;
            v47 = ((tColSphere*)(v14->elemShapePtr))->radius;
            if (v16)
            {
                v31 = OBJ_pst_GetAbsoluteMatrix(a4);
                MATH_GetScale(&dest, v31);
                v47 = MATH_f_MaxOf3(dest.x, dest.y, dest.z) * v47;
            }
            result = INT_SphereOBBox(&a3a, v47, &v49, &v50, 0);
        }
        else
        {
            v32 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_InvertMatrix(&v55, v32);
            v45 = &(((tColSphere*)(v14->elemShapePtr))->center);
            v33 = OBJ_pst_GetAbsoluteMatrix(a4);
            MATH_TransformVertex(&v52, v33, v45);
            MATH_TransformVertex(&a3a, &v55, &v52); // v12 = 1, //v14 = 2
            result = INT_SphereOBBox(
                &a3a,
                ((tColSphere*)(v14->elemShapePtr))->radius,
                &(((tColBox*)(v12->elemShapePtr))->pos2),
                &(((tColBox*)(v12->elemShapePtr))->pos1),
                0);
        }
        break;
    case 33: // v12 = 2, //v14 = 1
        if ((OBJ_pst_GetAbsoluteMatrix(a4)->mtxflag & 8) != 0)
        {
            v16 = (v14->colFlags & 8) == 0;
            v17 = (tColBox*)v14->elemShapePtr;
            v48 = v17;
            if (v16)
            {
                v18 = OBJ_pst_GetAbsoluteMatrix(a4);
                MATH_GetScale(&v50, v18);
                v51.x = v50.x * v48->pos2.x;
                v51.y = v50.y * v48->pos2.y;
                v51.z = v50.z * v48->pos2.z;
                v49.x = v50.x * v48->pos1.x;
                v49.y = v50.y * v48->pos1.y;
                v49.z = v50.z * v48->pos1.z;
            }
            else
            {
                v51 = v17->pos2;
                v49 = v17->pos1;
            }
            v19 = OBJ_pst_GetAbsoluteMatrix(a4);
            MATH_Transp33MatrixWithoutBuffer(&v55, v19);
            v55.mtxflag = v19->mtxflag;
            v55._m14 = (float)(1.0 / v19->_m14);
            v55._m24 = (float)(1.0 / v19->_m24);
            v55._m34 = (float)(1.0 / v19->_m34);
            MATH_VecInvert(&v55.Pos, &v19->Pos);
            MATH_TransformVectorNoScale(&v55.Pos, &v55, &v55.Pos);
            v42 = &(((tColSphere*)(v12->elemShapePtr))->center); // v12 = 2, //v14 = 1
            if ((v12->colFlags & 8) != 0)
            {
                v21 = OBJ_pst_GetAbsoluteMatrix(a1);
                MATH_TransformVertexNoScale(&v52, v21, v42);
            }
            else
            {
                v20 = OBJ_pst_GetAbsoluteMatrix(a1);
                MATH_TransformVertex(&v52, v20, v42);
            }
            MATH_TransformVertexNoScale(&dest, &v55, &v52);
            v16 = (v12->colFlags & 8) == 0;
            a4a = ((tColSphere*)(v12->elemShapePtr))->radius;
            if (v16)
            {
                v22 = OBJ_pst_GetAbsoluteMatrix(a1);
                MATH_GetScale(&a3a, v22);
                a4a = MATH_f_MaxOf3(a3a.x, a3a.y, a3a.z) * a4a;
            }
            result = INT_SphereOBBox(&dest, a4a, &v51, &v49, 0);
        }
        else
        {
            v23 = OBJ_pst_GetAbsoluteMatrix(a4);
            MATH_InvertMatrix(&v55, v23);
            v43 = &(((tColSphere*)(v12->elemShapePtr))->center);
            if ((v12->colFlags & 8) != 0)
            {
                v25 = OBJ_pst_GetAbsoluteMatrix(a1);
                MATH_TransformVertexNoScale(&v52, v25, v43);
            }
            else
            {
                v24 = OBJ_pst_GetAbsoluteMatrix(a1);
                MATH_TransformVertex(&v52, v24, v43);
            }
            MATH_TransformVertex(&dest, &v55, &v52);
            result = INT_SphereOBBox(
                &dest,
                ((tColSphere*)(v12->elemShapePtr))->radius, // v12 = 2, //v14 = 1
                &(((tColBox*)(v14->elemShapePtr))->pos2),
                &(((tColBox*)(v14->elemShapePtr))->pos1),
                0);
        }
        break;
    case 34:
        COL_Zone_Transform(v12, a1, &a3a, &a4aa);
        COL_Zone_Transform(v14, a4, &dest, &v48a);
        result = INT_SphereSphere(&a3a, a4aa.x, &dest, v48a.x);
        break;
    default:
        return 0;
    }
    return result;
}

int COL_ListCreate(struct sGameObjectMain* a1, int flagMustHave, int flagMustNothave, int whatToTest, char id, struct sWorld* a6, struct sGameObjectMain** a7, char a3)
{
    uint8_t v10;
    tableElemStructPF* v11;
    tableElemStructPF* v12;
    tGameObjectMain* v13;
    int v14;
    int v15;
    int v16;
    tableElemStructPF* v17;
    tableElemStructPF* a1a;

    if (!a1)
        return 0;
    v10 = 0;
    if (a3 == -1 || a3 == -2 || a3 == -3 || a3 == -4 || a3 == -5 || a3 == -6 || a3 == -17 || a3 == -18)
    {
        v11 = a6->activatedObjectsTable4PF.nextElemPtr;
        a1a = a6->activatedObjectsTable4PF.firstElemPtr;
    }
    else
    {
        if ((a1->flags & 0x400) == 0)
            return 0;
        v11 = a6->eot.collInstanceZDEGameObjectsPF.nextElemPtr;
        a1a = a6->eot.collInstanceZDEGameObjectsPF.firstElemPtr;
    }
    v17 = v11 - 1;
    v12 = a1a;
    if (a1a <= v17)
    {
        while (1)
        {
            v13 = (tGameObjectMain*)v12->objectPtr;
            if (!v12->objectPtr
                || ((uint8_t)v13 & 3) == 1
                || !OBJ_b_test_flag(v13, flagMustHave, flagMustNothave, whatToTest)
                || v13 == a1)
            {
                goto LABEL_44;
            }
            if (a3 == -5)
                break;
            if (a3 == -6)
            {
                v15 = COL_ZDE_PivotCollide(a1, id, v13, 1);
                goto LABEL_41;
            }
            if (a3 == -1)
            {
                v16 = COL_ZDE_BVCollide(a1, id, v13, 0);
            }
            else
            {
                if (a3 == -2)
                {
                    v14 = COL_ZDE_BVCollide(a1, id, v13, 1);
                LABEL_36:
                    if (v14)
                    {
                        a7[v10] = v13;
                        goto LABEL_43;
                    }
                    goto LABEL_44;
                }
                if (a3 == -3)
                {
                    v15 = COL_BV_PivotCollide(a1, v13, FALSE);
                    goto LABEL_41;
                }
                if (a3 != -4)
                {
                    if (a3 != -17)
                    {
                        if (a3 == -18)
                            v15 = COL_ZDE_BVCenterCollide(a1, id, v13, 1);
                        else
                            v15 = COL_ZDE_ZDECollide(a1, id, a3, v13);
                    LABEL_41:
                        if (v15)
                        {
                            a7[v10] = v13;
                            goto LABEL_43;
                        }
                        goto LABEL_44;
                    }
                    v14 = COL_ZDE_BVCenterCollide(a1, id, v13, 0);
                    goto LABEL_36;
                }
                v16 = COL_BV_PivotCollide(a1, v13, TRUE);
            }
            if (v16)
            {
                a7[v10] = v13;
            LABEL_43:
                ++v10;
            }
        LABEL_44:
            v12 = ++a1a;
            if (a1a > v17)
                return v10;
        }
        v14 = COL_ZDE_PivotCollide(a1, id, v13, 0);
        goto LABEL_36;
    }
    return v10;
}

struct sGameObjectMain* COL_GetFirstObjectInRange(struct sWorld* a1, Vec3* a2, float a3, int a4, int a5, int a6)
{
    tGameObjectMain* result;
    tableElemStructPF* v7; 
    tableElemStructPF* v8;
    tGameObjectMain* v9;
    float f1, f2, f3;

    result = (tGameObjectMain*)a1;
    if (a1)
    {
        v7 = a1->activatedObjectsTable4PF.firstElemPtr;
        v8 = ((tableElemStructPF*)(a1->activatedObjectsTable4PF.nextElemPtr)) - 1;
        if (v7 > v8)
        {
        LABEL_8:
            result = 0;
        }
        else
        {
            while (1)
            {
                v9 = (tGameObjectMain*)v7->objectPtr;
                if (v7->objectPtr)
                {
                    if (((uint8_t)v9 & 3) != 1 && OBJ_b_test_flag(v9, a4, a5, a6)) {
                        f1 = a2->x - v9->matrices->Pos.x;
                        f2 = a2->y - v9->matrices->Pos.y;
                        f3 = a2->z - v9->matrices->Pos.z;
                        if ((f1 * f1 + f2 * f2 + f3 * f3) < (a3 * a3)) break;                        
                    }
                }
                if (++v7 > v8)
                    goto LABEL_8;
            }
            result = v9;
        }
    }
    return result;
}

int INT_FullTriangleTriangle(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, Vec3* a6, Vec3* a7)
{
    double v8;
    Vec3* v9; 
    Vec3* v10;
    double v11;
    double v12;
    double v13;
    double v14;
    int v15; 
    Vec3* v16; 
    int v17; 
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    int v23; 
    int v24; 
    int result;
    double v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    double v33;
    double v34;
    double v35;
    Vec3 a2a; 
    float v37;
    float v38;
    float a3d;
    float a3e;
    float a3f;
    float a3g;
    float a3h;
    float a3i;
    float a3a;
    float a3b;
    float a3c;
    float a4a;
    float a5a;

    v8 = a2->x - a1->x;
    v9 = a4;
    v10 = a5;
    v11 = a2->y - a1->y;
    v12 = a2->z - a1->z;
    v37 = a3->x - a1->x;
    v38 = a3->y - a1->y;
    v13 = a3->z - a1->z;
    a2a.x = (float)(v13 * v11 - v38 * v12);
    a2a.y = (float)(v12 * v37 - v13 * v8);
    v14 = v8 * v38 - v11 * v37;
    a3d = (float)((a4->z - a1->z) * v14 + (a4->y - a1->y) * a2a.y + (a4->x - a1->x) * a2a.x);
    v15 = 1 << (a3d >= 0.0);
    v16 = a6;
    a3e = (float)((a5->z - a1->z) * v14 + (a5->y - a1->y) * a2a.y + (a5->x - a1->x) * a2a.x);
    v17 = v15 << (a3e >= 0.0);
    a3f = (float)((a6->z - a1->z) * v14 + (a6->y - a1->y) * a2a.y + (a6->x - a1->x) * a2a.x);
    if (((v17 << (a3f >= 0.0)) & 0xE) == 0)
        goto LABEL_3;
    v18 = a5->x - a4->x;
    v19 = a5->y - a4->y;
    a4a = (float)v19;
    v20 = a5->z - v9->z;
    a5a = (float)v20;
    v37 = a6->x - v9->x;
    v38 = a6->y - v9->y;
    v21 = a6->z - v9->z;
    a2a.x = (float)(v21 * v19 - v38 * v20);
    a2a.y = (float)(v20 * v37 - v21 * v18);
    v22 = v18 * v38 - v19 * v37;
    a3g = (float)((a1->z - v9->z) * v22 + (a1->y - v9->y) * a2a.y + (a1->x - v9->x) * a2a.x);
    v23 = 1 << (a3g >= 0.0);
    a3h = (float)((a2->z - v9->z) * v22 + (a2->y - v9->y) * a2a.y + (a2->x - v9->x) * a2a.x);
    v24 = v23 << (a3h >= 0.0);
    a3i = (float)((a3->z - v9->z) * v22 + (a3->y - v9->y) * a2a.y + (a3->x - v9->x) * a2a.x);
    if (((v24 << (a3i >= 0.0)) & 0xE) == 0)
        goto LABEL_3;
    v26 = a4a;
    a3a = (float)sqrt(a5a * a5a + v26 * v26 + v18 * v18);
    v27 = 1.0 / a3a;
    a4a = (float)v27;
    a2a.x = (float)(v27 * v18);
    a2a.y = (float)(v26 * a4a);
    a2a.z = a5a * a4a;
    if (INT_FullRayTriangle(v9, &a2a, a1, a2, a3, &a4a, a7))
    {
        if (a4a < (double)a3a)
            goto LABEL_10;
    }
    v28 = v16->x - v10->x;
    v29 = v16->y - v10->y;
    v30 = v16->z - v10->z;
    a2a.z = (float)v30;
    a3b = (float)sqrt(v30 * a2a.z + v29 * v29 + v28 * v28);
    v31 = 1.0 / a3b;
    a5a = (float)v31;
    a2a.x = (float)(v31 * v28);
    a2a.y = (float)(v29 * a5a);
    a2a.z = a2a.z * a5a;
    if (INT_FullRayTriangle(v10, &a2a, a1, a2, a3, &a4a, a7))
    {
        if (a4a < (double)a3b)
            goto LABEL_10;
    }
    v32 = v16->x - v9->x;
    v33 = v16->y - v9->y;
    v34 = v16->z - v9->z;
    a2a.z = (float)v34;
    a3c = (float)sqrt(v34 * a2a.z + v33 * v33 + v32 * v32);
    v35 = 1.0 / a3c;
    a5a = (float)v35;
    a2a.x = (float)(v35 * v32);
    a2a.y = (float)(v33 * a5a);
    a2a.z = a2a.z * a5a;
    if (!INT_FullRayTriangle(v9, &a2a, a1, a2, a3, &a4a, a7))
        goto LABEL_3;
    if (a4a < (double)a3c)
        LABEL_10:
    result = 1;
    else
        LABEL_3:
    result = 0;
    return result;
}

int COL_TriangleCollision(Vec3* a1, Vec3* src, Vec3* a3, Vec3* a4, struct sWorld* a5, int flagMustHave, int flagMustNothave, int whatToTest)
{
    Vec3* v8;
    Vec3* v9;
    tableElemStructPF* v10;
    tGameObjectMain* v11;
    Mtx4x4* v12;
    tGameObjectBV* v13;
    double v14; 
    tColMap* v15; 
    tColCollisionObject** v16;
    tColCollisionObject** v17;
    tCobMeshObjectMesh* v18; 
    tCobMeshChunkElem* v19; 
    tCobGameMaterialElems* v20;
    tCobMeshTriangleElem* v21;
    double v22; 
    double v23; 
    double v24; 
    long double v25; 
    double v26; 
    int v28; 
    tableElemStructPF* v29;
    float v30;
    tColCollisionObject** v31;
    float v32;
    tCobMeshChunkElem* v33;
    Vec3 dest;
    Vec3 v;
    tableElemStructPF* v36;
    float v37;
    float v38;
    float v39;
    tColCollisionObject** v40;
    float v41;
    float v42;
    tCobMeshTriangleElem* v43;
    float v44;
    float v45;
    float v46;
    tGameObjectMain* v47;
    float v48;
    float v49;
    float v50;
    tCobMeshChunkElem* v51;
    Vec3 a2a; 
    Vec3 a7; 
    float v54; 
    float v55; 
    Vec3 v56; 
    Mtx4x4 a1a; 

    if (a4)
    {
        a4->x = 0.0;
        a4->y = 0.0;
        a4->z = 0.0;
    }
    v8 = a3;
    v9 = src;
    v28 = 0;
    v32 = -100000.0f;
    v37 = MATH_f_MinOf3(a1->x, src->x, a3->x);
    v38 = MATH_f_MinOf3(a1->y, src->y, a3->y);
    v39 = MATH_f_MinOf3(a1->z, src->z, a3->z);
    v48 = MATH_f_MaxOf3(a1->x, src->x, a3->x);
    v49 = MATH_f_MaxOf3(a1->y, src->y, a3->y);
    v50 = MATH_f_MaxOf3(a1->z, src->z, a3->z);
    v10 = a5->eot.collMapObjectsTablePF.firstElemPtr;
    v29 = v10;
    v36 = ((tableElemStructPF*)(a5->eot.collMapObjectsTablePF.nextElemPtr)) - 1;
    if (v10 <= v36)
    {
        while (1)
        {
            v11 = (tGameObjectMain*)v10->objectPtr;
            v47 = v11;
            if (OBJ_b_test_flag(v11, flagMustHave, flagMustNothave, whatToTest))
            {
                if ((v11->flags2 & 1) != 0)
                {
                    v12 = v11->matrices;
                    v13 = v11->boundingVolume;
                    v14 = v12->Pos.x + v13->BVMin.x;
                    v54 = v12->Pos.y + v13->BVMin.y;
                    v55 = v12->Pos.z + v13->BVMin.z;
                    v44 = v12->Pos.x + v13->BVMax.x;
                    v45 = v12->Pos.y + v13->BVMax.y;
                    v46 = v12->Pos.z + v13->BVMax.z;
                    if (v14 <= v48
                        && v44 >= (double)v37
                        && v54 <= (double)v49
                        && v45 >= (double)v38
                        && v55 <= (double)v50
                        && v46 >= (double)v39)
                    {
                        MATH_InvertMatrix(&a1a, v12);
                        MATH_TransformVertexNoScale(&dest, &a1a, a1);
                        MATH_TransformVertexNoScale(&a2a, &a1a, v9);
                        MATH_TransformVertexNoScale(&v56, &a1a, v8);
                        v.x = a2a.x - dest.x;
                        v.y = a2a.y - dest.y;
                        v.z = a2a.z - dest.z;
                        MATH_NormalizeEqualVector(&v);
                        v15 = v11->extended->coll->colMap;
                        v16 = v15->collisionObjectPtr;
                        v17 = &v16[v15->colObjectID];
                        v31 = v16;
                        v40 = v17;
                        if (v16 < v17)
                        {
                            do
                            {
                                if ((*v16)->objectType == 5)
                                {
                                    v18 = (tCobMeshObjectMesh*)(*v16)->objectPtrByType;
                                    v19 = v18->meshChunkElems;
                                    v33 = v19;
                                    v51 = &v19[v18->meshChunkElemNum];
                                    if (v19 < v51)
                                    {
                                        do
                                        {
                                            v20 = COL_pst_GMat_Get(*v16, v19);
                                            if (!v20 || (v20->flags & 0x201000) == 0)
                                            {
                                                v21 = v19->triangleElems;
                                                v43 = &v21[v19->elemNum];
                                                if (v21 < v43)
                                                {
                                                    do
                                                    {
                                                        if (INT_FullTriangleTriangle(
                                                            &dest,
                                                            &a2a,
                                                            &v56,
                                                            &v18->vertices[v21->v1],
                                                            &v18->vertices[v21->v2],
                                                            &v18->vertices[v21->v3],
                                                            &a7))
                                                        {
                                                            v22 = a7.x - dest.x;
                                                            v23 = a7.y - dest.y;
                                                            v24 = a7.z - dest.z;
                                                            v42 = (float)v24;
                                                            v25 = 1.0 / sqrt(v24 * v42 + v23 * v23 + v22 * v22);
                                                            v30 = (float)v25;
                                                            v41 = (float)(v25 * v22);
                                                            v26 = v42 * v30 * v.z + v23 * v30 * v.y + v41 * v.x;
                                                            if (v26 > v32)
                                                            {
                                                                v32 = (float)v26;
                                                                v28 = 1;
                                                                if (a4)
                                                                    MATH_TransformVertexNoScale(a4, v47->matrices, &a7);
                                                            }
                                                        }
                                                        ++v21;
                                                    } while (v21 < v43);
                                                    v19 = v33;
                                                }
                                                v16 = v31;
                                            }
                                            v33 = ++v19;
                                        } while (v19 < v51);
                                        v17 = v40;
                                    }
                                }
                                v31 = ++v16;
                            } while (v16 < v17);
                            v9 = src;
                            v8 = a3;
                        }
                    }
                }
            }
            if (++v29 > v36)
                break;
            v10 = v29;
        }
    }
    return v28;
}

void COL_SwapToSpecific(struct sGameObjectMain* a1, uint8_t a2)
{
    tGameObjectExtended* v2;
    tGameObjectCollMain* v3;
    tColInstance* v4;
    uint8_t v5;
    int v6;
    tColInstanceElem* v7;
    uint8_t v8;
    tColInstanceElem* instElemOrigPtr;
    tColInstanceElem* instElemNewPtr;
    int v11;
    tColInstanceElem** v12;
    tColInstanceElem** v13;
    char i; 
    tColInstanceElem* v15;
    void* v16;
    uint8_t v18;
    void* v19;
    uint8_t v20;
    tColInstanceElem* v21;
    int v22;
    int origNewDistance;

    if (a2 != 0xFF)
    {
        if (a1)
        {
            v2 = a1->extended;
            if (v2)
            {
                v3 = v2->coll;
                if (v3)
                {
                    v4 = v3->colInstance;
                    if (v4)
                    {
                        if (v4->colSet)
                        {
                            v5 = v4->colSet->zoneIDs[a2];
                            if (v5 <= v4->zoneNum)
                            {
                                v6 = v5;
                                v22 = v5;
                                if (((1 << v5) & v4->instElemBitmask) == 0)
                                {
                                    v7 = v4->selectedInstElemPtrs[v5];
                                    v8 = v4->instElemNum;
                                    v21 = v4->selectedInstElemPtrs[v6];
                                    if (v8)
                                    {
                                        instElemOrigPtr = v4->colInstElems;
                                        instElemNewPtr = (tColInstanceElem*)MEM_p_Realloc(instElemOrigPtr, sizeof(tColInstanceElem) * (v8 + 1));
                                        v11 = v4->zoneNum;
                                        v4->colInstElems = instElemNewPtr;
                                        origNewDistance = (char*)instElemNewPtr - (char*)instElemOrigPtr;
                                        v12 = v4->selectedInstElemPtrs;
                                        v13 = &v12[v11];
                                        for (i = 0; v12 < v13; ++i)
                                        {
                                            if (((1 << i) & v4->instElemBitmask) != 0)
                                                *v12 = (tColInstanceElem*)((char*)*v12 + origNewDistance); // TODO FIXME: ez egy igen nagy tárgya, hogy a régi + új offset különbsége alapjkán frissíti a selectedInstElemPtrs-t, megoldani másképp
                                            ++v12;
                                        }
                                    }
                                    else
                                    {
                                        v4->colInstElems = (tColInstanceElem*)MEM_p_Alloc(sizeof(tColInstanceElem));
                                    }
                                    v15 = &v4->colInstElems[v4->instElemNum];
                                    *v15 = *v7;
                                    if (v7->elemType == 1)      // aabbox
                                    {
                                        v19 = MEM_p_Alloc(sizeof(tColBox));
                                        v15->elemShapePtr = v19;
                                        memcpy(v19, v7->elemShapePtr, sizeof(tColBox));
                                        v15->elemType = v21->elemType;
                                        v15->colFlags = v21->colFlags;
                                        v15->unk1 = v21->unk1;
                                        v18 = v21->design;
                                    }
                                    else
                                    {
                                        if (v7->elemType != 2)
                                        {
                                        LABEL_20:
                                            v4->instElemBitmask |= 1 << v22;
                                            v15->colFlags |= 0x40u;
                                            v4->selectedInstElemPtrs[v22] = v15;
                                            v20 = v4->sharedZoneNum - 1;
                                            ++v4->instElemNum;
                                            v4->sharedZoneNum = v20;
                                            return;
                                        }
                                        v16 = MEM_p_Alloc(sizeof(tColSphere));     // sphere
                                        v15->elemShapePtr = v16;
                                        memcpy(v16, v7->elemShapePtr, sizeof(tColSphere));
                                        v15->elemType = v7->elemType;
                                        v15->colFlags = v7->colFlags;
                                        v15->unk1 = v7->unk1;
                                        v18 = v7->design;
                                    }
                                    v15->design = v18;
                                    goto LABEL_20;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int COL_ListCreateDistance(int flagMustHave, int flagMustNothave, int whatToTest, struct sWorld* a4, struct sGameObjectMain** a5, Vec3* a6, float a7, char a8)
{
    tableElemStructPF* v8;
    tableElemStructPF* v9;
    uint8_t i;
    tGameObjectMain* v11;
    int v15;
    float f1, f2, f3;

    v8 = a4->activatedObjectsTable4PF.firstElemPtr;
    v9 = ((tableElemStructPF*)(a4->activatedObjectsTable4PF.nextElemPtr)) - 1;
    for (i = 0; v8 <= v9; ++v8)
    {
        v11 = (tGameObjectMain*)v8->objectPtr;
        if (v8->objectPtr)
        {
            if (((uint8_t)v11 & 3) != 1)
            {
                if (OBJ_b_test_flag(v11, flagMustHave, flagMustNothave, whatToTest))
                {
                    f1 = a6->x - v11->matrices->Pos.x;
                    f2 = a6->y - v11->matrices->Pos.y;
                    f3 = a6->z - v11->matrices->Pos.z;
                    
                    if ((f1 * f1 + f2 * f2 + f3 * f3) < (a7 * a7))       
                    {      
                        v15 = i++;
                        a5[v15] = v11;
                        if (i >= (uint8_t)a8)
                            i = a8 - 1;
                    }
                }
            }
        }
    }
    return i;
}

void COL_ZDx_SpecificPosSet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3)
{
    tColInstance* v3;
    int v4;
    tColInstanceElem* v5; 
    tColBox* v6;
    double v7; 
    float v8; 
    float v9; 
    float v10;
    float v11;
    float v12;
    float v13;

    if ((a1->flags & 0x600) != 0)
    {
        v3 = a1->extended->coll->colInstance;
        v4 = v3->colSet->zoneIDs[a2];
        if ((v3->instElemBitmask & (1 << v4)) != 0)
        {
            v5 = v3->selectedInstElemPtrs[v4];
            if (v5->elemType == 1)
            {
                v6 = (tColBox*)v5->elemShapePtr;
                v12 = v6->pos1.z - v6->pos2.z;
                v8 = (float)((v6->pos1.x - v6->pos2.x) * 0.5);
                v9 = v8 + v6->pos2.x;
                v10 = (float)((v6->pos1.y - v6->pos2.y) * 0.5 + v6->pos2.y);
                v7 = a3->x - v9;
                v11 = a3->y - v10;
                v13 = (float)(a3->z - (v12 * 0.5 + v6->pos2.z));
                v6->pos1.x = (float)(v7 + v6->pos1.x);
                v6->pos1.y = v11 + v6->pos1.y;
                v6->pos1.z = v13 + v6->pos1.z;
                v6->pos2.x = (float)(v7 + v6->pos2.x);
                v6->pos2.y = v11 + v6->pos2.y;
                v6->pos2.z = v13 + v6->pos2.z;
            }
            else if (v5->elemType == 2)
            {
                ((tColSphere*)v5->elemShapePtr)->center = *a3;
            }
        }
    }
}

void COL_ZDx_SpecificSizeSet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3)
{
    tColInstance* v3;
    int v4;
    tColInstanceElem* v5;
    tColBox* v6;
    double v7;
    float v8; 
    float v9; 
    float v10;
    float v11;

    if ((a1->flags & 0x600) != 0)
    {
        v3 = a1->extended->coll->colInstance;
        v4 = v3->colSet->zoneIDs[a2];
        if ((v3->instElemBitmask & (1 << v4)) != 0)
        {
            v5 = v3->selectedInstElemPtrs[v4];
            if (v5->elemType == 1)
            {
                v6 = (tColBox*)v5->elemShapePtr;
                v11 = v6->pos1.z - v6->pos2.z;
                v8 = (float)((v6->pos1.x - v6->pos2.x) * 0.5);
                v9 = v8 + v6->pos2.x;
                v10 = (float)((v6->pos1.y - v6->pos2.y) * 0.5 + v6->pos2.y);
                v7 = v11 * 0.5 + v6->pos2.z;
                v6->pos2.x = (float)(v9 - a3->x * 0.5);
                v6->pos2.y = (float)(v10 - a3->y * 0.5);
                v6->pos2.z = (float)(v7 - a3->z * 0.5);
                v6->pos1.x = (float)(a3->x * 0.5 + v9);
                v6->pos1.y = (float)(a3->y * 0.5 + v10);
                v6->pos1.z = (float)(a3->z * 0.5 + v7);
            }
            else if (v5->elemType == 2)
            {
                ((tColSphere*)(v5->elemShapePtr))->radius = a3->x;
            }
        }
    }
}

float COL_ZDx_SizeGet(struct sGameObjectMain* a1, uint8_t a2)
{
    double result;
    tColInstance* v3;
    uint8_t v4; 
    tColInstanceElem* v5;
    tColBox* v6;
    float a1a;
    float a2a;
    float a3; 

    result = 0.0;
    if ((a1->flags & 0x600) == 0)
        return 0.0;
    v3 = a1->extended->coll->colInstance;
    v4 = v3->colSet->zoneIDs[a2];
    if (v4 == 0xFF)
        return 0.0;
    v5 = v3->selectedInstElemPtrs[v4];
    if (v5->elemType == 1)
    {
        v6 = (tColBox*)v5->elemShapePtr;
        a3 = v6->pos1.z - v6->pos2.z;
        a2a = v6->pos1.y - v6->pos2.y;
        a1a = v6->pos1.x - v6->pos2.x;
        result = MATH_f_MaxOf3(a1a, a2a, a3);
    }
    else if (v5->elemType == 2)
    {
        result = ((tColSphere*)(v5->elemShapePtr))->radius;
    }
    return (float)result;
}

void COL_ZDx_PosGet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3)
{
    tColInstance* v3;
    uint8_t v4;
    tColInstanceElem* v5;
    tColBox* v6;

    if ((a1->flags & 0x600) != 0)
    {
        v3 = a1->extended->coll->colInstance;
        v4 = v3->colSet->zoneIDs[a2];
        if (v4 == 0xFF)
        {
            a3->x = 0.0;
            a3->y = 0.0;
            a3->z = 0.0;
        }
        v5 = v3->selectedInstElemPtrs[v4];
        if (v5->elemType == 1)
        {
            v6 = (tColBox*)v5->elemShapePtr;
            a3->x = v6->pos1.x + v6->pos2.x;
            a3->y = v6->pos1.y + v6->pos2.y;
            a3->z = v6->pos1.z + v6->pos2.z;
            a3->x = (float)(a3->x * 0.5);
            a3->y = (float)(a3->y * 0.5);
            a3->z = (float)(a3->z * 0.5);
        }
        else
        {
            if (v5->elemType != 2)
                return;
            *a3 = ((tColSphere*)(v5->elemShapePtr))->center;
        }
        if ((v5->colFlags & 8) != 0)
            MATH_TransformVertexNoScale(a3, a1->matrices, a3);
        else
            MATH_TransformVertex(a3, a1->matrices, a3);
    }
}

void COL_GeoColMap_PosSet(struct sGameObjectMain* a1, uint8_t a2, Vec3* a3)
{
    tColMap* v3;
    tColCollisionObject* v4;
    Vec3* v5;

    if (a1)
    {
        if ((a1->flags & 0x100) != 0)
        {
            v3 = a1->extended->coll->colMap;
            if (v3->colObjectID > a2)
            {
                v4 = v3->collisionObjectPtr[a2];
                if (v4->objectType == 2)
                {                    
                    v5 = &(((tColSphere*)(((tCobNormalObjectGeneric*)(v4->objectPtrByType))->enclosurePtr))->center);
                }
                else
                {
                    if (v4->objectType != 3)
                        return;
                    v5 = &(((tColCylinder*)(((tCobNormalObjectGeneric*)(v4->objectPtrByType))->enclosurePtr))->center);
                }
                *v5 = *a3;
            }
        }
    }
}

int COL_Report_ul_GMatReportGet(struct sGameObjectMain* a1, struct sWorld* a2, int a3)
{
    int v3;
    int result;
    tRecordElem* i;
    tCobGameMaterialElems* v6;

    v3 = a2->recordCount;
    result = 0;
    if (!v3)
        return -1;
    for (i = a2->records; ; ++i)
    {
        if ((i->flags & 0x40000) == 0 && (i->collisionInstanceGameObject == a1 || i->collisionMapGameObject == a1))
        {
            v6 = i->gameMaterial;
            if (v6)
            {
                if ((a3 & v6->flags) != 0)
                    break;
            }
        }
        if (++result >= v3)
            return -1;
    }
    return result;
}
