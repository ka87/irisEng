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

void COL_FreeDetectionList(struct sGameObjectCollMain* coll)
{
    if (!coll || !coll->detectionList) return;

    MEM_Free(coll->detectionList->detectionListGaos);
    MEM_Free(coll->detectionList);
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


void COL_FreeColMap(struct sGameObjectCollMain* coll)
{
    tColMap* colMap;
    tColCollisionObject** cob;
    tColCollisionObject** end;
    BOOL isMesh;

    if (!coll) return;

    colMap = coll->colMap;
    if (!colMap) return;

    LOA_DeleteAddress((int)colMap);
    if (colMap->colObjectID)
    {
        cob = colMap->collisionObjectPtr;
        end = &cob[colMap->colObjectID];
        for (; cob < end; ++cob)
        {
            isMesh = (*cob)->objectType == 5;
            if (!--(*cob)->usageCount)
            {
                COL_FreeGameMaterial(*cob);
                LOA_DeleteAddress((int)*cob);
                if (isMesh)
                    COL_FreeTrianglesIndexedCob(*cob);
                else
                    COL_FreeMathematicalCob(*cob);
            }
        }
        MEM_Free(colMap->collisionObjectPtr);
    }
    MEM_Free(colMap);
    coll->colMap = 0;
}

void COL_Free(struct sGameObjectMain* gao)
{
    tGameObjectExtended* ext;
    tGameObjectCollMain* coll;

    if (!gao) return;

    ext = gao->extended;
    if (!ext) return;

    coll = ext->coll;
    if (!coll) return;

    if (coll->colInstance)
        COL_FreeInstance(coll);
    if (coll->detectionList)
        COL_FreeDetectionList(coll);
    if (coll->colMap)
        COL_FreeColMap(coll);
    MEM_Free(coll);
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

int INT_PointInTriangle(Vec3* point, Vec3* p1, Vec3* p2, Vec3* p3, Vec3* normal, uint8_t* edgeOut)
{
    float axis;
    double lastA;
    double lastB;
    float e1x;
    float e1y;
    float e1z;
    float e2x;
    float e2y;
    float e2z;
    float e3x;
    float e3y;
    float e3z;
    float cross12;
    float cross23;
    float cross31;

    e1x = p1->x - point->x;
    e1y = p1->y - point->y;
    e1z = p1->z - point->z;
    e2x = p2->x - point->x;
    e2y = p2->y - point->y;
    e2z = p2->z - point->z;
    e3x = p3->x - point->x;
    e3y = p3->y - point->y;
    e3z = p3->z - point->z;

    if (fabs(normal->x) <= 0.57735026) // invsqrt3
    {
        if (fabs(normal->y) <= 0.57735026)
        {
            axis = normal->z;
            cross12 = e1x * e2y - e2x * e1y;
            cross23 = e2x * e3y - e3x * e2y;
            lastA = e3x * e1y;
            lastB = e1x * e3y;
        }
        else
        {
            axis = normal->y;
            cross12 = e2x * e1z - e1x * e2z;
            cross23 = e3x * e2z - e2x * e3z;
            lastA = e1x * e3z;
            lastB = e3x * e1z;
        }
    }
    else
    {
        axis = normal->x;
        cross12 = e2z * e1y - e2y * e1z;
        cross23 = e3z * e2y - e3y * e2z;
        lastA = e3y * e1z;
        lastB = e3z * e1y;
    }

    if (((*(int32_t*)&cross12) ^ (*(int32_t*)&axis)) < 0)
    {
        if (edgeOut)
            *edgeOut = 0;
        return 0;
    }
    if (((*(int32_t*)&cross23) ^ (*(int32_t*)&axis)) < 0)
    {
        if (edgeOut)
            *edgeOut = 1;
        return 0;
    }
    cross31 = (float)(lastA - lastB);
    if (((*(int32_t*)&cross31) ^ (*(int32_t*)&axis)) >= 0)
        return 1;
    if (edgeOut)
        *edgeOut = 2;
    return 0;
}

void COL_ComputeProximity(tColCollisionObject* cob)
{
    tCobMeshObjectMesh* mesh;
    tCobMeshChunkElem* chunk;
    tCobMeshTriangleElem* tri;
    tCobMeshChunkElem* otherChunk;
    tCobMeshTriangleElem* otherTri;
    uint16_t triV3;
    uint16_t otherV1;
    uint16_t otherV2;
    uint16_t otherV3;
    uint16_t neighbour;
    BOOL shares1;
    BOOL shares2;
    BOOL shares3;
    int prox1Set;
    int prox2Set;
    int prox3Set;
    unsigned int chunkID;
    unsigned int elemID;
    unsigned int otherChunkID;
    unsigned int otherElemID;

    mesh = (tCobMeshObjectMesh*)cob->objectPtrByType;

    for (chunkID = 0; chunkID < mesh->meshChunkElemNum; ++chunkID)
    {
        chunk = &mesh->meshChunkElems[chunkID];
        for (elemID = 0; elemID < chunk->elemNum; ++elemID)
        {
            tri = &chunk->triangleElems[elemID];
            triV3 = tri->v3;
            tri->proximity1 = -1;
            tri->proximity2 = -1;
            tri->proximity3 = -1;
            neighbour = 0;
            prox1Set = 0;
            prox2Set = 0;
            prox3Set = 0;

            for (otherChunkID = 0; otherChunkID < mesh->meshChunkElemNum; ++otherChunkID)
            {
                otherChunk = &mesh->meshChunkElems[otherChunkID];
                for (otherElemID = 0; otherElemID < otherChunk->elemNum; ++otherElemID, ++neighbour)
                {
                    otherTri = &otherChunk->triangleElems[otherElemID];
                    if (otherTri == tri)
                        continue;

                    otherV1 = otherTri->v1;
                    otherV2 = otherTri->v2;
                    otherV3 = otherTri->v3;
                    shares1 = tri->v1 == otherV1 || tri->v1 == otherV2 || tri->v1 == otherV3;
                    shares2 = tri->v2 == otherV1 || tri->v2 == otherV2 || tri->v2 == otherV3;
                    shares3 = triV3 == otherV1 || triV3 == otherV2 || triV3 == otherV3;
                    if (shares1 + shares2 + shares3 != 2)
                        continue;

                    if (shares1 && shares2)
                    {
                        tri->proximity1 = neighbour;
                        prox1Set = 1;
                    }
                    else if (shares1 && shares3)
                    {
                        tri->proximity2 = neighbour;
                        prox2Set = 1;
                    }
                    else
                    {
                        tri->proximity3 = neighbour;
                        prox3Set = 1;
                    }

                    if (prox1Set && prox2Set && prox3Set)
                        break;
                }
                if (prox1Set && prox2Set && prox3Set)
                    break;
            }
        }
    }
}

void COL_ColMap_AddSphere(tColCollisionObject* cob, tColCollisionObject* srcCob, Vec3* center)
{
    tColSphere* sphere;
    tColSphere* srcSphere;
    tCobNormalObjectGeneric* shape;

    shape = (tCobNormalObjectGeneric*)cob->objectPtrByType;
    shape->enclosurePtr = MEM_p_Alloc(sizeof(tColSphere));
    sphere = (tColSphere*)shape->enclosurePtr;

    if (srcCob)
    {
        srcSphere = (tColSphere*)((tCobNormalObjectGeneric*)srcCob->objectPtrByType)->enclosurePtr;
        sphere->radius = srcSphere->radius;
        if (center)
            sphere->center = *center;
        else
            sphere->center = srcSphere->center;
        cob->flags = srcCob->flags;
    }
    else
    {
        sphere->radius = 1.0;
        if (center)
        {
            sphere->center = *center;
        }
        else
        {
            sphere->center.x = 0.0;
            sphere->center.y = 0.0;
            sphere->center.z = 0.0;
        }
    }
    cob->objectType = 2;
}

tCobGameMaterialElems* COL_pst_GMat_Get(tColCollisionObject* cob, tCobMeshChunkElem* chunk)
{
    tCobGameMaterial* gameMaterial;
    tCobGameMaterialElems* elems;
    unsigned int wantedID;
    int num;
    int* ids;
    unsigned int mid;
    unsigned int value;
    unsigned int low;
    int index;
    int* id;
    tCobNormalObjectGeneric* shape;

    gameMaterial = cob->gameMaterial;
    if (!gameMaterial)
        return 0;
    elems = gameMaterial->elems;
    if (!elems)
        return 0;

    if (chunk)
    {
        wantedID = chunk->gameMaterialElemID;
    }
    else
    {
        if (cob->objectType == 5)
            return 0;
        shape = (tCobNormalObjectGeneric*)cob->objectPtrByType;
        wantedID = shape->gameMaterialElemID;
    }

    num = gameMaterial->num;
    if (gameMaterial->num > 10u)
    {
        ids = gameMaterial->ptrInts;
        mid = num >> 1;
        value = ids[num >> 1];
        low = 0;
        while (value != wantedID)
        {
            if (low + 1 == num)
                return 0;
            if (value <= wantedID)
                low = mid;
            else
                num = mid;
            mid = (num + low) >> 1;
            value = ids[mid];
        }
        elems[mid].id = wantedID;
        return &cob->gameMaterial->elems[mid];
    }

    index = 0;
    if (!num)
        return 0;
    for (id = gameMaterial->ptrInts; *id != wantedID; ++id)
    {
        if (++index >= num)
            return 0;
    }
    elems[index].id = wantedID;
    return &cob->gameMaterial->elems[index];
}

BOOL COL_GMat_Skip(tCobGameMaterialElems* gameMaterial, tColGlobal* colGlobal)
{
    if (!gameMaterial)
        return FALSE;

    if ((gameMaterial->flags & 0x200000) != 0 && (colGlobal->collInstanceElem->colFlags & 0x10) == 0)
        return TRUE;
    if ((gameMaterial->flags & 0x1000) != 0
        && (colGlobal->collisionObjectPtr->flags & 2) != 0
        && (colGlobal->collInstanceElem->colFlags & 0x80u) == 0)
        return TRUE;
    return (colGlobal->collInstanceElem->colFlags & 0x80u) != 0
        && (gameMaterial->flags & 0x1000) == 0
        && (colGlobal->collisionObjectPtr->flags & 2) != 0;
}

BOOL INT_PointSphere(Vec3* point, float radius, Vec3* center)
{
    float distSquared;

    distSquared = (point->x - center->x) * (point->x - center->x) +
        (point->y - center->y) * (point->y - center->y) +
        (point->z - center->z) * (point->z - center->z);
    if ((uint16_t)((uint16_t)(radius * radius < distSquared) << 8 | // ez milyen egy megoldás float összehasonlításra?????? TODO FIXME
        (uint16_t)(radius * radius == distSquared) << 14) == 0) {
        return 1;
    }
    return 0;
}

BOOL INT_SphereSphere(Vec3* center1, float radius1, Vec3* center2, float radius2)
{
    return INT_PointSphere(center1, radius1 + radius2, center2);
}

BOOL INT_FullSphereSphere(Vec3* center1, float radius1, Vec3* center2, float radius2, Vec3* normalOut, Vec3* pointOut)
{
    double deltaX;
    double deltaY;
    double deltaZ;
    double offsetX;
    double offsetY;
    float invLength;
    float offsetZ;
    Vec3 dir;
    float deltaZf;

    if (!INT_SphereSphere(center1, radius1, center2, radius2))
        return FALSE;

    deltaX = center1->x - center2->x; //TODO FIXME v7 volt az a3, így biztos jó?
    deltaY = center1->y - center2->y;
    deltaZ = center1->z - center2->z;
    deltaZf = (float)deltaZ;
    invLength = (float)(1.0 / sqrt(deltaZ * deltaZf + deltaX * deltaX + deltaY * deltaY));
    dir.x = (float)(deltaX * invLength);
    dir.y = (float)(deltaY * invLength);
    dir.z = deltaZf * invLength;
    offsetX = dir.x * radius2;
    pointOut->x = (float)offsetX;
    offsetY = dir.y * radius2;
    pointOut->y = (float)offsetY;
    offsetZ = dir.z * radius2;
    pointOut->z = offsetZ;
    pointOut->x = (float)(offsetX + center2->x);
    pointOut->y = (float)(offsetY + center2->y);
    pointOut->z = (float)(offsetZ + center2->z);
    *normalOut = dir;
    return TRUE;
}

int INT_FullSegmentSphere(Vec3* segStart, Vec3* segEnd, Vec3* center, float radius, Vec3* hitOut)
{
    double deltaX;
    double deltaZ;
    long double length;
    double invLength;
    double proj;
    double perpSquared;
    float toCenterX;
    float toCenterY;
    float toCenterZ;
    Vec3 dir;
    float startDistSquared;
    float deltaY;
    float projF;
    float radiusSquared;
    float lengthF;

    toCenterX = segStart->x - center->x;
    toCenterY = segStart->y - center->y;
    toCenterZ = segStart->z - center->z;
    deltaX = segEnd->x - segStart->x;
    deltaY = segEnd->y - segStart->y;
    deltaZ = segEnd->z - segStart->z;
    length = sqrt(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX);
    lengthF = (float)length;
    if (length == 0.0f)
        return 0;

    invLength = 1.0 / lengthF;
    dir.x = (float)(invLength * deltaX);
    dir.y = (float)(deltaY * invLength);
    dir.z = (float)(invLength * deltaZ);
    proj = -(toCenterZ * dir.z + toCenterY * dir.y + toCenterX * dir.x);
    projF = (float)proj;
    startDistSquared = toCenterZ * toCenterZ + toCenterX * toCenterX + toCenterY * toCenterY;
    radiusSquared = radius * radius;
    if (projF < 0.0 && startDistSquared > (double)radiusSquared)
        return 0;

    perpSquared = startDistSquared - proj * projF;
    if (perpSquared > radiusSquared || projF - sqrt(radiusSquared - perpSquared) > lengthF)
        return 0;

    if (projF <= (double)lengthF)
    {
        if (startDistSquared <= (double)radiusSquared)
        {
            *hitOut = *segStart;
        }
        else
        {
            MATH_VecMultScalar(hitOut, &dir, projF);
            MATH_VecAddVec(hitOut, segStart, hitOut);
        }
    }
    else
    {
        *hitOut = *segEnd;
    }
    return 1;
}

int INT_FullSegmentTriangle(Vec3* segStart, Vec3* segEnd, Vec3* p1, Vec3* p2, Vec3* p3, Vec3* normal, Vec3* hitOut, uint8_t* edgeOut)
{
    double denom;
    double offsetX;
    double offsetY;
    float deltaX;
    float deltaY;
    float deltaZ;
    float ratio;
    float offsetZ;

    deltaX = segEnd->x - segStart->x;
    deltaY = segEnd->y - segStart->y;
    deltaZ = segEnd->z - segStart->z;
    denom = deltaZ * normal->z + deltaX * normal->x + deltaY * normal->y;
    if (denom < 0.0000099999997f && denom > -0.0000099999997f)
        return 0;

    ratio = (float)(((p1->z - segStart->z) * normal->z
        + (p1->x - segStart->x) * normal->x
        + (p1->y - segStart->y) * normal->y) / denom);
    if (ratio <= 0.0 || ratio >= 1.0)
        return 0;

    offsetX = deltaX * ratio;
    hitOut->x = (float)offsetX;
    offsetY = deltaY * ratio;
    hitOut->y = (float)offsetY;
    offsetZ = deltaZ * ratio;
    hitOut->z = offsetZ;
    hitOut->x = (float)(offsetX + segStart->x);
    hitOut->y = (float)(offsetY + segStart->y);
    hitOut->z = offsetZ + segStart->z;
    return INT_PointInTriangle(hitOut, p1, p2, p3, normal, edgeOut);
}

int INT_FullSphereTriangle(Vec3* centerPos, float radius, Vec3* point1, Vec3* point2, Vec3* point3, Vec3* nomalVec, Vec3* collisionPoint, int* collisionType)
{
    Vec3 segStart;
    Vec3 segEnd;
    float backStep;
    uint8_t edge;

    segStart.z = radius * nomalVec->z;
    segStart.x = radius * nomalVec->x + centerPos->x;
    segStart.y = centerPos->y + radius * nomalVec->y;
    segStart.z = centerPos->z + segStart.z;
    edge = 0;
    backStep = -(radius + radius);
    segEnd.x = backStep * nomalVec->x + segStart.x;
    segEnd.y = backStep * nomalVec->y + segStart.y;
    segEnd.z = backStep * nomalVec->z + segStart.z;

    if (INT_FullSegmentTriangle(&segStart, &segEnd, point1, point2, point3, nomalVec, collisionPoint, &edge))
    {
        *collisionType = 1;
        return 1;
    }

    if (edge == 0)
    {
        if (INT_FullSegmentSphere(point1, point2, centerPos, radius, collisionPoint))
        {
            *collisionType = 2;
            return 1;
        }
        if (INT_FullSegmentSphere(point2, point3, centerPos, radius, collisionPoint))
        {
            *collisionType = 8;
            return 1;
        }
        if (INT_FullSegmentSphere(point3, point1, centerPos, radius, collisionPoint))
        {
            *collisionType = 4;
            return 1;
        }
        return 0;
    }

    if (edge == 1)
    {
        if (INT_FullSegmentSphere(point2, point3, centerPos, radius, collisionPoint))
        {
            *collisionType = 8;
            return 1;
        }
        if (INT_FullSegmentSphere(point1, point2, centerPos, radius, collisionPoint))
        {
            *collisionType = 2;
            return 1;
        }
        if (INT_FullSegmentSphere(point3, point1, centerPos, radius, collisionPoint))
        {
            *collisionType = 4;
            return 1;
        }
        return 0;
    }

    if (edge == 2)
    {
        if (INT_FullSegmentSphere(point3, point1, centerPos, radius, collisionPoint))
        {
            *collisionType = 4;
            return 1;
        }
        if (INT_FullSegmentSphere(point1, point2, centerPos, radius, collisionPoint))
        {
            *collisionType = 2;
            return 1;
        }
        if (INT_FullSegmentSphere(point2, point3, centerPos, radius, collisionPoint))
        {
            *collisionType = 8;
            return 1;
        }
        return 0;
    }
    return 1;
}

void COL_ReportCollision(Vec3* collidedPoint, Vec3* normalVec, Vec3* recalVec, int flags, int triangleChunkElemID, int16_t triangleChunkID, tColGlobal* colGlobal)
{
    tWorld* world;
    tRecordElem* record;
    tColCollisionObject* cob;
    tCobGameMaterialElems* gameMaterial;

    if (colGlobal->num >= 1000u)
        return;

    world = colGlobal->colWorld;
    record = &world->records[colGlobal->num];
    record->collidedPoint = *collidedPoint;
    record->recalVec = *recalVec;
    record->normalVec = *normalVec;
    record->collisionInstanceGameObject = colGlobal->colInstanceGao;
    record->collisionMapGameObject = colGlobal->colMapGao;
    record->flags = flags;
    record->triangleChunkElemID = triangleChunkElemID;
    record->triangleChunkID = triangleChunkID;

    cob = colGlobal->collisionObjectPtr;
    if ((flags & 0xF) != 0)
    {
        gameMaterial = COL_pst_GMat_Get(cob, colGlobal->cobMeshChunkElem);
        record->designVal = colGlobal->cobMeshChunkElem->designVal;
    }
    else
    {
        gameMaterial = COL_pst_GMat_Get(cob, 0);
        record->designVal = 0;
    }
    record->gameMaterial = gameMaterial;
    if (gameMaterial && (colGlobal->colInstance->crossable & gameMaterial->flags) != 0)
        record->flags |= 0x2000u;

    ++colGlobal->num;
    world->recordCount = colGlobal->num;
}

BOOL COL_NeedRecomputation(tColGlobal* colGlobal, tGameObjectMain* gao, int16_t flag)
{
    tColInstance* instance;
    uint8_t count;
    int i;

    if ((gao->flags2 & 0x20) == 0)
        return 1;

    instance = gao->extended->coll->colInstance;
    if (!instance || instance->flag < 0)
        return 1;

    count = colGlobal->gaosNeededToBeRecomputedNum;
    if (count >= 50u)
        return 0;

    for (i = 0; i < count; ++i)
    {
        if (colGlobal->gaosNeededToBeRecomputed[i] == gao)
        {
            instance->flag |= flag;
            return 0;
        }
    }

    colGlobal->gaosNeededToBeRecomputed[count] = gao;
    ++colGlobal->gaosNeededToBeRecomputedNum;
    instance->flag |= flag;
    return 1;
}

int16_t COL_w_TestInstanceFlag(struct sColInstance* instance, int16_t mask)
{
    if (!instance)
        return 0;
    return mask & instance->flag;
}

BOOL COL_b_IsARealTopologicEdge(tColGlobal* colGlobal, Vec3* normal, uint8_t edgeFlags)
{
    BOOL faceIsWall;
    BOOL neighbourIsWall;
    BOOL instanceUpright;
    uint16_t neighbour;
    tCobMeshTriangleElem* tri;
    Vec3* neighbourNormal;
    Vec3* gravityNeg;
    double faceDot;
    double neighbourDot;
    Vec3 faceInMapSpace;
    Vec3 neighbourInMapSpace;

    colGlobal->colMapEdgeNormalVec.x = 0.0;
    colGlobal->colMapEdgeNormalVec.y = 0.0;
    colGlobal->colMapEdgeNormalVec.z = 0.0;
    instanceUpright = fabs(colGlobal->colInstGaoMatrices->v3.z - 1.0) < 0.001;

    tri = colGlobal->cobMeshTriangleElem;
    if ((edgeFlags & 2) != 0)
        neighbour = tri->proximity1;
    else if ((edgeFlags & 4) != 0)
        neighbour = tri->proximity2;
    else
        neighbour = tri->proximity3;
    if (neighbour == 0xFFFF)
        return 1;

    neighbourNormal = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[neighbour]);
    colGlobal->colMapEdgeNormalVec.x = neighbourNormal->x + normal->x;
    colGlobal->colMapEdgeNormalVec.y = neighbourNormal->y + normal->y;
    colGlobal->colMapEdgeNormalVec.z = neighbourNormal->z + normal->z;

    if (instanceUpright)
    {
        gravityNeg = colGlobal->ptrSetToVec6InZDMZDRGravityNeg;
        faceIsWall = gravityNeg->z * normal->z + gravityNeg->y * normal->y + gravityNeg->x * normal->x < colGlobal->wallCosAngle;
        neighbourIsWall = gravityNeg->z * neighbourNormal->z + gravityNeg->y * neighbourNormal->y + neighbourNormal->x * gravityNeg->x
            < colGlobal->wallCosAngle;
    }
    else
    {
        MATH_TransformVectorNoScale(&faceInMapSpace, colGlobal->colMapGaoMatrices, normal);
        MATH_TransformVectorNoScale(&neighbourInMapSpace, colGlobal->colMapGaoMatrices, neighbourNormal);
        gravityNeg = colGlobal->ptrSetToVec6InZDMZDRGravityNeg;
        faceDot = gravityNeg->z * faceInMapSpace.z + gravityNeg->y * faceInMapSpace.y + faceInMapSpace.x * gravityNeg->x;
        if (faceDot >= 0.0f && faceDot < colGlobal->wallCosAngle)
            faceIsWall = 1;
        else
            faceIsWall = faceDot < 0.0 && faceDot > -0.80000001f;
        neighbourDot = gravityNeg->z * neighbourInMapSpace.z + gravityNeg->y * neighbourInMapSpace.y + neighbourInMapSpace.x * gravityNeg->x;
        neighbourIsWall = neighbourDot >= 0.0f && neighbourDot < colGlobal->wallCosAngle
            || neighbourDot < 0.0f && neighbourDot > -0.80000001f;
    }

    colGlobal->colMapEdgeNormalVec.x = colGlobal->colMapEdgeNormalVec.x * 0.5f;
    colGlobal->colMapEdgeNormalVec.y = colGlobal->colMapEdgeNormalVec.y * 0.5f;
    colGlobal->colMapEdgeNormalVec.z = colGlobal->colMapEdgeNormalVec.z * 0.5f;
    return faceIsWall && neighbourIsWall
        && cos(0.3141592741012573) > neighbourNormal->z * normal->z + neighbourNormal->y * normal->y + neighbourNormal->x * normal->x;
}

int COL_ComputeReport(Vec3* localPoint, Vec3* localNormal, Vec3* worldPoint, Vec3* worldNormal, Vec3* recalVec, int* flags, int triangleChunkElemID, int16_t triangleChunkID, tColGlobal* colGlobal)
{
    float alongPush;
    double upDot;
    Vec3* gravityNeg;
    tGameObjectMain* gao;
    tGameObjectExtended* ext;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    tColInstance* reportInstance = NULL;
    tCobGameMaterialElems* gameMaterial;
    tColCollisionObject* cob;
    tCobMeshChunkElem* chunkElem;
    tColInstanceElem* elem;
    tColBox* boxShape;
    tWorld* world;
    tRecordElem* record;
    Mtx4x4* mapMtx;
    Vec3* edgeNormal;
    double prevCenterZ;
    double centerZ;
    double alongPushRaw;
    double toCenterLenSquared;
    double cornerDepth;
    double pushDistanceEps;
    long double pushDistanceRaw;
    long double toCenterLen;
    BOOL movingAway;
    BOOL isTopologicEdge;
    int flagsNow;
    int corner;
    Vec3 pushDir;
    Vec3 toCenter;
    Vec3 pushDirWorld;
    Vec3 scratch;
    Vec3 toPrevCenter;
    Vec3 moveDirUnit;
    Vec3 groundNormal;
    Vec3 scratch2;
    Vec3 pushVec;
    float upDotF;
    float maxPenetration;
    float radicand;
    float pushDistance;

    MATH_TransformVectorNoScale(worldNormal, colGlobal->colMapGaoMatrices, localNormal);
    MATH_TransformVertexNoScale(worldPoint, colGlobal->colMapGaoMatrices, localPoint);
    upDot = colGlobal->ptrSetToVec6InZDMZDRGravityNeg->y * worldNormal->y
        + colGlobal->ptrSetToVec6InZDMZDRGravityNeg->z * worldNormal->z
        + colGlobal->ptrSetToVec6InZDMZDRGravityNeg->x * worldNormal->x;
    upDotF = (float)upDot;

    if (upDot >= 0.0 && upDotF < (double)colGlobal->wallCosAngle
        || upDotF < 0.0 && upDotF > -0.80000001
        || (*flags & 0x400) != 0 && !(worldNormal->x == 0.0f && worldNormal->y == 0.0f))
    {
        *flags |= 0x80u;
        instance = colGlobal->colInstance;
        if (instance && (instance->flag & 4) != 0)
        {
            gameMaterial = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, colGlobal->cobMeshChunkElem);
            if (!gameMaterial || (colGlobal->colInstance->crossable & gameMaterial->flags) == 0)
            {
                COL_NeedRecomputation(colGlobal, colGlobal->colInstanceGao, 16);
                return 0;
            }
        }
    }
    else
    {
        *flags |= 0x40u;
        gravityNeg = colGlobal->ptrSetToVec6InZDMZDRGravityNeg;
        if (upDotF >= 0.0)
        {
            groundNormal = *gravityNeg;
        }
        else
        {
            MATH_VecInvert(&groundNormal, gravityNeg);
            colGlobal->colInstanceGao->extended->coll->colInstance->flag |= 0x4000u;
        }
    }

    toPrevCenter.x = colGlobal->centerPosPrevious.x - localPoint->x;
    toPrevCenter.y = colGlobal->centerPosPrevious.y - localPoint->y;
    prevCenterZ = colGlobal->centerPosPrevious.z - localPoint->z;
    toPrevCenter.z = (float)prevCenterZ;
    if (prevCenterZ * localNormal->z + toPrevCenter.y * localNormal->y + toPrevCenter.x * localNormal->x <= 0.0)
    {
        *flags |= 0x80001000;
        return 1;
    }

    toCenter.x = colGlobal->centerPos.x - localPoint->x;
    toCenter.y = colGlobal->centerPos.y - localPoint->y;
    centerZ = colGlobal->centerPos.z - localPoint->z;
    toCenter.z = (float)centerZ;
    movingAway = centerZ * localNormal->z + toCenter.y * localNormal->y + toCenter.x * localNormal->x < 0.0;

    flagsNow = *flags;
    if ((*flags & 0xE) != 0 && (flagsNow & 0x80u) != 0 && (flagsNow & 0x20) != 0
        && COL_b_IsARealTopologicEdge(colGlobal, localNormal, *flags))
    {
        isTopologicEdge = 1;
        *flags |= 0x800u;
    }
    else
    {
        isTopologicEdge = 0;
    }

    flagsNow = *flags;
    if ((flagsNow & 0x40) == 0 || (colGlobal->collisionObjectPtr->flags & 0x10) != 0)
    {
        if (isTopologicEdge || COL_gst_GlobalVars.COL_gb_SpecialCamera && (flagsNow & 0x20) == 0)
        {
            MATH_NormalizeVector(&moveDirUnit, &toPrevCenter);
            MATH_TransformVectorNoScale(worldNormal, colGlobal->colMapGaoMatrices, &moveDirUnit);
            if (colGlobal->colInstanceGao == WOR_gpst_CurrentWorld->viewPtr->assignedGao)
            {
                pushDir = moveDirUnit;
            }
            else if (fabs(colGlobal->colMapGaoMatrices->v3.z - 1.0) >= 0.001)
            {
                scratch.x = worldNormal->x;
                scratch.y = worldNormal->y;
                scratch.z = 0.0;
                MATH_NormalizeEqualVector(&scratch);
                MATH_TransformVectorNoScale(&pushDir, &colGlobal->colMapGaoInvMatrix, &scratch);
            }
            else
            {
                pushDir = moveDirUnit;
                if (fabs(moveDirUnit.z) >= 0.001)
                {
                    pushDir.z = 0.0;
                    MATH_NormalizeEqualVector(&pushDir);
                }
            }
        }
        else if ((colGlobal->collisionObjectPtr->flags & 0x10) != 0)
        {
            MATH_TransformVectorNoScale(&scratch2, colGlobal->colMapGaoMatrices, localNormal);
            MATH_TransformVectorNoScale(&pushDir, &colGlobal->colMapGaoInvMatrix, &scratch2);
        }
        else if (colGlobal->colInstanceGao == WOR_gpst_CurrentWorld->viewPtr->assignedGao)
        {
            pushDir.x = localNormal->x;
            pushDir.y = localNormal->y;
            pushDir.z = localNormal->z;
        }
        else
        {
            mapMtx = colGlobal->colMapGaoMatrices;
            if (fabs(mapMtx->v3.z - 1.0) >= 0.001)
            {
                MATH_TransformVectorNoScale(&scratch, mapMtx, localNormal);
                scratch.z = 0.0;
                MATH_NormalizeEqualVector(&scratch);
                MATH_TransformVectorNoScale(&pushDir, &colGlobal->colMapGaoInvMatrix, &scratch);
            }
            else
            {
                pushDir.x = localNormal->x;
                pushDir.y = localNormal->y;
                pushDir.z = 0.0;
                MATH_NormalizeEqualVector(&pushDir);
            }
        }
    }
    else
    {
        MATH_TransformVector(&pushDir, &colGlobal->colMapGaoInvMatrix, &groundNormal);
        MATH_NormalizeEqualVector(&pushDir);
    }

    elem = colGlobal->collInstanceElem;
    if (elem->elemType != 2)
    {
        boxShape = (tColBox*)elem->elemShapePtr;
        MATH_TransformVectorNoScale(&pushDirWorld, colGlobal->colMapGaoMatrices, &pushDir);
        maxPenetration = -100000.0f;
        for (corner = 0; corner < 8; ++corner)
        {
            if ((corner & 1) != 0)
                scratch.x = boxShape->pos2.x;
            else
                scratch.x = boxShape->pos1.x;
            if ((corner & 2) != 0)
                scratch.y = boxShape->pos2.y;
            else
                scratch.y = boxShape->pos1.y;
            if ((corner & 4) != 0)
                scratch.z = boxShape->pos2.z;
            else
                scratch.z = boxShape->pos1.z;
            MATH_TransformVertex(&scratch2, colGlobal->colInstGaoMatrices, &scratch);
            cornerDepth = pushDirWorld.z * (worldPoint->z - scratch2.z)
                + pushDirWorld.y * (worldPoint->y - scratch2.y)
                + (worldPoint->x - scratch2.x) * pushDirWorld.x;
            if (cornerDepth > maxPenetration)
                maxPenetration = (float)cornerDepth;
        }
        if (maxPenetration >= 0.001)
        {
            recalVec->x = pushDirWorld.x * maxPenetration;
            recalVec->y = pushDirWorld.y * maxPenetration;
            recalVec->z = pushDirWorld.z * maxPenetration;
            return 1;
        }
        *flags |= 0x40000u;
        recalVec->x = 0.0;
        recalVec->y = 0.0;
        recalVec->z = 0.0;
        return 1;
    }

    flagsNow = *flags;
    if ((*flags & 0x20) == 0)
    {
        pushDistanceRaw = -(toCenter.z * pushDir.z + pushDir.y * toCenter.y + pushDir.x * toCenter.x);
        pushDistance = (float)pushDistanceRaw;
    }
    else if ((flagsNow & 1) == 0 && (flagsNow & 0x700) == 0)
    {
        alongPushRaw = toCenter.z * pushDir.z + pushDir.y * toCenter.y + pushDir.x * toCenter.x;
        alongPush = (float)alongPushRaw;
        radicand = (float)(alongPushRaw * alongPush + colGlobal->radius * colGlobal->radius);
        pushDistanceRaw = sqrt(radicand - MATH_VecLenSquared(&toCenter)) - alongPush;
        pushDistance = (float)pushDistanceRaw;
    }
    else
    {
        toCenterLen = sqrt(toCenter.z * toCenter.z + toCenter.y * toCenter.y + toCenter.x * toCenter.x);
        if (movingAway)
            pushDistance = (float)((toCenterLen + colGlobal->radius)
                / (pushDir.y * localNormal->y + pushDir.x * localNormal->x + pushDir.z * localNormal->z));
        else
            pushDistance = (float)((colGlobal->radius - toCenterLen)
                / (pushDir.y * localNormal->y + pushDir.x * localNormal->x + pushDir.z * localNormal->z));

        if ((flagsNow & 0x40) != 0 && (flagsNow & 1) != 0)
        {
            cob = colGlobal->collisionObjectPtr;
            if ((cob->flags & 0x10) == 0 && worldNormal->z < 0.94999999)
            {
                MATH_VecMultScalar(&scratch, localNormal, colGlobal->radius);
                MATH_VecInvert(&scratch, &scratch);
                MATH_VecAddVecWithoutBuffer(&scratch, &colGlobal->centerPos);
                MATH_VecMultScalar(&scratch2, &pushDir, pushDistance);
                chunkElem = colGlobal->cobMeshChunkElem;
                scratch.x = scratch2.x + scratch.x;
                scratch.y = scratch2.y + scratch.y;
                scratch.z = scratch2.z + scratch.z;
                if (!INT_PointInTriangle(&scratch,
                    &(((tCobMeshObjectMesh*)(cob->objectPtrByType))->vertices[chunkElem->triangleElems[triangleChunkElemID].v1]),
                    &(((tCobMeshObjectMesh*)(cob->objectPtrByType))->vertices[chunkElem->triangleElems[triangleChunkElemID].v2]),
                    &(((tCobMeshObjectMesh*)(cob->objectPtrByType))->vertices[chunkElem->triangleElems[triangleChunkElemID].v3]),
                    localNormal, 0))
                {
                    *flags = *flags & 0xFFFFFFFE | 0xE;
                    alongPushRaw = toCenter.z * pushDir.z + pushDir.y * toCenter.y + pushDir.x * toCenter.x;
                    alongPush = (float)alongPushRaw;
                    radicand = (float)(alongPushRaw * alongPush + colGlobal->radius * colGlobal->radius);
                    toCenterLenSquared = MATH_VecLenSquared(&toCenter);
                    gao = colGlobal->colInstanceGao;
                    pushDistance = (float)(sqrt(radicand - toCenterLenSquared) - alongPush);

                    reportInstance = 0;
                    if (gao)
                    {
                        ext = gao->extended;
                        if (ext)
                        {
                            coll = ext->coll;
                            if (coll)
                                reportInstance = coll->colInstance;
                        }
                    }
                    if (!COL_w_TestInstanceFlag(reportInstance, 1024))
                        COL_NeedRecomputation(colGlobal, gao, 1024);
                }
            }
        }
    }

    if (isTopologicEdge && (colGlobal->colInstance->flag & 1) == 0 && colGlobal->num < 1000u) // reportcollision inline ?
    {
        world = colGlobal->colWorld;
        record = &world->records[colGlobal->num];
        record->collidedPoint = *worldPoint;
        record->recalVec = MATH_gst_NulVector;
        MATH_TransformVectorNoScale(&record->normalVec, colGlobal->colMapGaoMatrices, localNormal);
        record->collisionInstanceGameObject = colGlobal->colInstanceGao;
        record->collisionMapGameObject = colGlobal->colMapGao;
        record->flags = *flags & 0xE | 0x200A0;
        record->triangleChunkElemID = triangleChunkElemID;
        record->triangleChunkID = triangleChunkID;
        gameMaterial = COL_pst_GMat_Get(colGlobal->collisionObjectPtr, colGlobal->cobMeshChunkElem);
        if (gameMaterial && (colGlobal->colInstance->crossable & gameMaterial->flags) != 0)
            record->flags |= 0x2000u;
        record->gameMaterial = gameMaterial;
        ++colGlobal->num;
    }

    if (pushDistance < 0.001)
    {
        *flags |= 0x40000u;
        recalVec->x = 0.0;
        recalVec->y = 0.0;
        recalVec->z = 0.0;
        return 1;
    }

    pushDistanceEps = pushDistance + 0.0000099999997;
    pushVec.x = (float)(pushDir.x * pushDistanceEps);
    pushVec.y = (float)(pushDir.y * pushDistanceEps);
    pushVec.z = (float)(pushDir.z * pushDistanceEps);

    if ((*flags & 0x40) != 0)
    {
        cob = colGlobal->collisionObjectPtr;
        if ((cob->flags & 0x10) == 0)
        {
            gameMaterial = COL_pst_GMat_Get(cob, colGlobal->cobMeshChunkElem);
            if (!gameMaterial || (colGlobal->colInstance->crossable & gameMaterial->flags) == 0)
            {
                instance = colGlobal->colInstance;
                if (instance)
                    instance->flag |= 4u;
                flagsNow = *flags;
                if ((*flags & 0xE) != 0)
                {
                    if ((flagsNow & 2) != 0)
                        edgeNormal = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[colGlobal->cobMeshTriangleElem->proximity1]);
                    else
                        edgeNormal = worldNormal;
                    if ((flagsNow & 4) != 0)
                        edgeNormal = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[colGlobal->cobMeshTriangleElem->proximity2]);
                    if ((flagsNow & 8) != 0)
                        edgeNormal = &(((tCobMeshObjectMesh*)(colGlobal->collisionObjectPtr->objectPtrByType))->normals[colGlobal->cobMeshTriangleElem->proximity3]);
                    if (MATH_DotProduct(edgeNormal, localNormal) < 0.94999999)
                        COL_NeedRecomputation(colGlobal, colGlobal->colInstanceGao, 32);
                }
            }
        }
    }

    MATH_TransformVectorNoScale(recalVec, colGlobal->colMapGaoMatrices, &pushVec);
    return 1;
}

void COL_AllocInstance(struct sGameObjectMain* gao)
{
    tGameObjectCollMain* coll;

    if (!gao->extended->coll)
    {
        gao->extended->coll = (tGameObjectCollMain*)MEM_p_Alloc(sizeof(tGameObjectCollMain));
        LOA_ul_FileTypeSize_38 += sizeof(tGameObjectCollMain);
        coll = gao->extended->coll;
        coll->detectionList = 0;
        coll->colInstance = 0;
        coll->colMap = 0;
    }

    coll = gao->extended->coll;
    coll->colInstance = (tColInstance*)MEM_p_Alloc(sizeof(tColInstance));
    LOA_ul_FileTypeSize_38 += sizeof(tColInstance);
    memset(coll->colInstance, 0, sizeof(tColInstance));
    coll->colInstance->mtx = MATH_Matrix_Alloc();
    LOA_ul_FileTypeSize_38 += sizeof(Mtx4x4);
    MATH_SetIdentityMatrix(coll->colInstance->mtx);
}

void COL_DuplicateInstance(struct sGameObjectMain* dest, struct sGameObjectMain* src)
{
    tColInstance* srcInstance;
    tColInstance* destInstance;
    tColInstSet* colSet;
    tColInstanceElem* srcElem;
    tColInstanceElem* destElem;
    tColInstanceElem* srcEnd;
    tColInstanceElem* searchElem;
    tColInstanceElem* searchEnd;
    tColInstanceElem* table;
    tColSphere* srcSphere;
    tColSphere* destSphere;
    tColBox* srcBox;
    tColBox* destBox;
    tColInstanceElem** destSlot;
    tColInstanceElem** srcSlot;
    tColInstanceElem** slotEnd;
    uint8_t index;
    uint8_t zone;

    srcInstance = src->extended->coll->colInstance;
    COL_AllocInstance(dest);
    destInstance = dest->extended->coll->colInstance;
    colSet = srcInstance->colSet;

    destInstance->colSet = srcInstance->colSet;
    destInstance->zoneNum = srcInstance->zoneNum;
    destInstance->sharedZoneNum = srcInstance->sharedZoneNum;
    destInstance->instElemNum = srcInstance->instElemNum;
    destInstance->instElemBitmask = srcInstance->instElemBitmask;
    destInstance->activationFlags = srcInstance->activationFlags;
    destInstance->minLod = srcInstance->minLod;
    destInstance->maxLod = srcInstance->maxLod;

    destInstance->selectedInstElemPtrs = (tColInstanceElem**)MEM_p_Alloc(4 * colSet->instElemNum);
    LOA_ul_FileTypeSize_38 += 4 * colSet->instElemNum;

    if (srcInstance->instElemNum)
    {
        destInstance->colInstElems = (tColInstanceElem*)MEM_p_Alloc(8 * srcInstance->instElemNum);
        LOA_ul_FileTypeSize_38 += 8 * srcInstance->instElemNum;
        memcpy(destInstance->colInstElems, srcInstance->colInstElems, 8 * srcInstance->instElemNum);

        srcElem = srcInstance->colInstElems;
        srcEnd = &srcElem[srcInstance->instElemNum];
        destElem = destInstance->colInstElems;
        for (; srcElem < srcEnd; ++srcElem, ++destElem)
        {
            if (srcElem->elemType == 1)
            {
                destElem->elemShapePtr = MEM_p_Alloc(sizeof(tColBox));
                LOA_ul_FileTypeSize_38 += sizeof(tColBox);
                srcBox = (tColBox*)srcElem->elemShapePtr;
                destBox = (tColBox*)destElem->elemShapePtr;
                destBox->pos1.x = srcBox->pos1.x;
                destBox->pos1.y = srcBox->pos1.y;
                destBox->pos1.z = srcBox->pos1.z;
                destBox->pos2.x = srcBox->pos2.x;
                destBox->pos2.y = srcBox->pos2.y;
                destBox->pos2.z = srcBox->pos2.z;
            }
            else if (srcElem->elemType == 2)
            {
                destElem->elemShapePtr = MEM_p_Alloc(sizeof(tColSphere));
                LOA_ul_FileTypeSize_38 += sizeof(tColSphere);
                srcSphere = (tColSphere*)srcElem->elemShapePtr;
                destSphere = (tColSphere*)destElem->elemShapePtr;
                destSphere->center.x = srcSphere->center.x;
                destSphere->center.y = srcSphere->center.y;
                destSphere->center.z = srcSphere->center.z;
                destSphere->radius = srcSphere->radius;
            }
        }
    }

    destSlot = destInstance->selectedInstElemPtrs;
    srcSlot = srcInstance->selectedInstElemPtrs;
    slotEnd = &destSlot[destInstance->zoneNum];
    for (zone = 0; destSlot < slotEnd; ++destSlot, ++srcSlot, ++zone)
    {
        if (((1 << zone) & destInstance->instElemBitmask) != 0)
        {
            searchElem = srcInstance->colInstElems;
            searchEnd = &searchElem[srcInstance->instElemNum];
            table = destInstance->colInstElems;
        }
        else
        {
            searchElem = srcInstance->colSet->colInstElems;
            searchEnd = &searchElem[srcInstance->colSet->instElemNum];
            table = destInstance->colSet->colInstElems;
        }

        index = 0;
        if (searchElem >= searchEnd)
        {
            index = -1;
        }
        else
        {
            while (searchElem != *srcSlot)
            {
                ++searchElem;
                ++index;
                if (searchElem >= searchEnd)
                {
                    index = -1;
                    break;
                }
            }
        }
        *destSlot = &table[index];
    }
    ++colSet->instanceNum;
}

void COL_DuplicateColMap(struct sGameObjectMain* dest, struct sGameObjectMain* src)
{
    tColMap* srcColMap;
    tColMap* destColMap;
    tColCollisionObject** cob;
    tColCollisionObject** end;
    uint8_t i;

    srcColMap = src->extended->coll->colMap;
    COL_AllocColMap(dest);
    destColMap = dest->extended->coll->colMap;
    destColMap->activation = srcColMap->activation;
    destColMap->b3 = srcColMap->b3;
    destColMap->b4 = srcColMap->b4;
    destColMap->colObjectID = srcColMap->colObjectID;
    destColMap->collisionObjectPtr = (tColCollisionObject**)MEM_p_Alloc(4 * srcColMap->colObjectID);
    LOA_ul_FileTypeSize_38 += 4 * srcColMap->colObjectID;

    cob = srcColMap->collisionObjectPtr;
    end = &cob[srcColMap->colObjectID];
    for (i = 0; cob < end; ++i)
    {
        destColMap->collisionObjectPtr[i] = *cob;
        ++(*cob++)->usageCount;
    }
}

void COL_ColMap_AddGeometric(struct sColCollisionObject* cob, struct sGEOGRO* geo, tGameObjectMain* gao)
{
    tCobMeshObjectMesh* mesh;
    Mtx4x4* absMtx;
    Vec3* srcVertex;
    Vec3* destVertex;
    Vec3* vertexEnd;
    Vec3* vertices;
    unsigned int byteCount;
    tGeoTriangleChunks* geoChunk;
    tGeoTriangleChunks* geoChunkEnd;
    tCobMeshChunkElem* chunkElem;
    tCobMeshChunkElem* chunkElemEnd;
    tCobMeshTriangleElem* destTri;
    tGeoTriangleChunkElem* srcTri;
    tCobMeshTriangleElem* triEnd;
    int faceIndex;
    Vec3* verts;
    Vec3* vertB;
    Vec3* vertA;
    double abX;
    double abY;
    Vec3* vertC;
    double abZ;
    double acZ;
    Vec3* normal;
    long double invLength;
    int globalFace;
    float normalX;
    float normalY;
    float normalZ;
    Vec3 scale;
    Vec3 ac;

    if (!geo)
    {
        if ((gao->flags & 0x4000) == 0)
            return;
        geo = (tGEOGRO*)(((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
        if (geo->intf->id == 8)                   // lod
            geo = ((tGEOSTATICLOD *)(geo))->geo[0];
        if (!geo)
            return;
    }
    if (!geo->vertexNum || !geo->triangleChunkCount)
        return;

    cob->objectType = 5;
    mesh = (tCobMeshObjectMesh*)MEM_p_Alloc(0x1Cu);
    cob->objectPtrByType = mesh;
    mesh->ok3 = 0;
    GEO_MRM_SetNumberOfPoints_Percent(geo, 2.0);
    mesh->vertexCount = geo->vertexNum;

    if (gao && (OBJ_pst_GetAbsoluteMatrix(gao)->mtxflag & 8) != 0)
    {
        mesh->vertices = (Vec3*)MEM_p_Alloc(12 * mesh->vertexCount);
        absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_GetScale(&scale, absMtx);
        srcVertex = geo->vertices;
        destVertex = mesh->vertices;
        for (vertexEnd = &srcVertex[geo->vertexNum]; srcVertex < vertexEnd; ++srcVertex, ++destVertex)
        {
            destVertex->x = scale.x * srcVertex->x;
            destVertex->y = scale.y * srcVertex->y;
            destVertex->z = scale.z * srcVertex->z;
        }
    }
    else
    {
        vertices = (Vec3*)MEM_p_Alloc(12 * mesh->vertexCount);
        byteCount = 12 * mesh->vertexCount;
        mesh->vertices = vertices;
        memcpy(vertices, geo->vertices, byteCount);
    }

    mesh->meshChunkElemNum = geo->triangleChunkCount;
    mesh->meshChunkElems = (tCobMeshChunkElem*)MEM_p_Alloc(16 * geo->triangleChunkCount);

    geoChunk = geo->triangleChunks;
    geoChunkEnd = &geoChunk[geo->triangleChunkCount];
    mesh->faceCount = 0;
    for (; geoChunk < geoChunkEnd; ++geoChunk)
        mesh->faceCount += geoChunk->triangleNum;
    mesh->normals = (Vec3*)MEM_p_Alloc(12 * mesh->faceCount);

    chunkElem = mesh->meshChunkElems;
    chunkElemEnd = &chunkElem[mesh->meshChunkElemNum];
    geoChunk = geo->triangleChunks;
    globalFace = 0;
    for (; chunkElem < chunkElemEnd; ++chunkElem, ++geoChunk)
    {
        chunkElem->flags = 0;
        chunkElem->gameMaterialElemID = geoChunk->materialNum;
        chunkElem->elemNum = geoChunk->triangleNum;
        if ((geoChunk->triangleNum & 0xffff) == 0)
            continue;

        destTri = (tCobMeshTriangleElem*)MEM_p_Alloc(12 * geoChunk->triangleNum);
        chunkElem->triangleElems = destTri;
        srcTri = geoChunk->triangleChunkElems;
        triEnd = &destTri[chunkElem->elemNum];
        if (destTri >= triEnd)
            continue;

        faceIndex = globalFace;
        globalFace += ((char*)triEnd - (char*)destTri - 1) / 12 + 1; // TODO FIXME, ez undorito, megcsinálni
        do
        {
            destTri->v1 = srcTri->v1;
            destTri->v2 = srcTri->v2;
            destTri->v3 = srcTri->v3;
            verts = mesh->vertices;
            vertB = &verts[srcTri->v2];
            vertA = &verts[srcTri->v1];
            abX = vertB->x - vertA->x;
            abY = vertB->y - vertA->y;
            vertC = &verts[srcTri->v3];
            abZ = vertB->z - vertA->z;
            ac.x = vertC->x - vertA->x;
            ac.y = vertC->y - vertA->y;
            acZ = vertC->z - vertA->z;
            normalX = (float)(acZ * abY - ac.y * abZ);
            normalY = (float)(abZ * ac.x - acZ * abX);
            normalZ = (float)(ac.y * abX - abY * ac.x);
            if (normalX == 0.0f && normalY == 0.0f && normalZ == 0.0f)
            {
                normal = &mesh->normals[faceIndex];
                normal->x = normalX;
                normal->y = normalY;
                normal->z = normalZ;
            }
            else
            {
                normal = &mesh->normals[faceIndex];
                invLength = 1.0 / sqrt(normalZ * normalZ + normalY * normalY + normalX * normalX);
                normal->x = (float)(normalX * invLength);
                normal->y = (float)(normalY * invLength);
                normal->z = (float)(invLength * normalZ);
            }
            ++destTri;
            ++srcTri;
            ++faceIndex;
        } while (destTri < triEnd);
    }
}

void COL_ColMap_AddCob(tGameObjectMain* gao, uint8_t type, void* src, Vec3* center)
{
    tColMap* colMap;
    uint8_t count;
    tColCollisionObject* cob;
    tCobNormalObjectGeneric* shape;

    colMap = gao->extended->coll->colMap;
    count = colMap->colObjectID;
    if (colMap->colObjectID == 8)
        DBG_assert(0,__FILE__, __LINE__, 0, "Sorry, Max number of Cobs reached", 0, 1);

    if (count)
    {
        colMap->collisionObjectPtr = (tColCollisionObject**)MEM_p_Realloc(colMap->collisionObjectPtr, 4 * count + 4);
        colMap->collisionObjectPtr[colMap->colObjectID] = (tColCollisionObject*)MEM_p_Alloc(sizeof(tColCollisionObject));
        cob = colMap->collisionObjectPtr[colMap->colObjectID];
    }
    else
    {
        colMap->collisionObjectPtr = (tColCollisionObject**)MEM_p_Alloc(4u);
        *colMap->collisionObjectPtr = (tColCollisionObject*)MEM_p_Alloc(sizeof(tColCollisionObject));
        cob = *colMap->collisionObjectPtr;
    }
    cob->usageCount = 0;
    cob->gameMaterial = 0;

    if (type == 2)
    {
        shape = (tCobNormalObjectGeneric*)MEM_p_Alloc(8u);
        cob->objectPtrByType = shape;
        shape->enclosurePtr = 0;
        shape->gameMaterialElemID = 0;
        COL_ColMap_AddSphere(cob, (tColCollisionObject*)src, center);
    }
    else if (type == 5)
    {
        COL_ColMap_AddGeometric(cob, (tGEOGRO*)src, gao);
    }
    ++colMap->colObjectID;
    ++cob->usageCount;
}

void COL_ResolveRefs(struct sGameObjectMain* gao)
{
    tColMap* colMap;
    tColCollisionObject** cob;
    tColCollisionObject** cobEnd;
    tCobGameMaterial* gameMaterial;
    tColInstance* instance;
    tColInstanceElem** slots;
    tColInstanceElem** slot;
    tColInstSet* colSet;
    uint8_t elemNum;
    uint8_t index;
    uint8_t zone;
    uint8_t i;

    if ((gao->flags & 0x100) != 0)
    {
        colMap = gao->extended->coll->colMap;
        if (!colMap || !colMap->colObjectID)
        {
            COL_AllocColMap(gao);
            COL_ColMap_AddCob(gao, 5u, 0, 0);
        }
        else
        {
            cob = colMap->collisionObjectPtr;
            cobEnd = &cob[colMap->colObjectID];
            for (; cob < cobEnd; ++cob)
            {
                if (!*cob)
                {
                    gao->flags &= ~0x100u;
                    MEM_Free(colMap->collisionObjectPtr);
                    LOA_DeleteAddress((int)colMap);
                    MEM_Free(colMap);
                    gao->extended->coll->colMap = 0;
                    break;
                }
                gameMaterial = (*cob)->gameMaterial;
                if (gameMaterial && !(*cob)->usageCount)
                    ++gameMaterial->gameMaterialUsageCnt;
                ++(*cob)->usageCount;
                if ((*cob)->objectType == 5)
                {
                    if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
                        COL_ComputeProximity(*cob);
                    COL_OK3_Build(gao, 1, 0);
                }
            }
        }
    }

    if ((gao->flags & 0x600) == 0)
        return;

    instance = gao->extended->coll->colInstance;
    if (!instance || !instance->colSet)
    {
        gao->flags &= ~0x600u;
        return;
    }

    if ((gao->flags & 0x400000) != 0)
        MATH_CopyMatrix(instance->mtx, &gao->basic->mtx->mtx);
    else
        MATH_CopyMatrix(instance->mtx, OBJ_pst_GetAbsoluteMatrix(gao));

    elemNum = instance->colSet->instElemNum;
    if (instance->zoneNum != elemNum)
    {
        instance->zoneNum = elemNum;
        instance->sharedZoneNum = elemNum;
        if (instance->instElemNum)
        {
            for (i = 0; i < instance->instElemNum; ++i)
                MEM_Free((void*)instance->colInstElems[i].elemShapePtr);
            MEM_Free(instance->colInstElems);
        }
        slots = instance->selectedInstElemPtrs;
        instance->instElemNum = 0;
        if (slots)
            MEM_Free(slots);
        instance->selectedInstElemPtrs = (tColInstanceElem**)MEM_p_Alloc(4 * instance->zoneNum);
        memset(instance->selectedInstElemPtrs, 0, 4 * instance->zoneNum);
        for (i = 0; i < instance->zoneNum; ++i)
            instance->selectedInstElemPtrs[i] = (tColInstanceElem*)i;
        instance->instElemBitmask = 0;
    }

    if (instance->zoneNum)
    {
        zone = 0;
        while (1)
        {
            slot = &instance->selectedInstElemPtrs[zone];
            index = (uint8_t)*slot;
            if (index > 16u)
            {
                gao->flags &= ~0x600u;
                return;
            }
            if ((instance->instElemBitmask & (1 << zone)) != 0)
                *slot = &instance->colInstElems[index];
            else
                *slot = &instance->colSet->colInstElems[index];
            if (++zone >= instance->zoneNum)
                break;
        }
    }

    colSet = instance->colSet;
    if (colSet)
        ++colSet->instanceNum;
}

void COL_Reinit(struct sGameObjectMain* gao)
{
    tGameObjectExtended* ext;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    tColMap* colMap;

    if (WOR_gpst_CurrentWorld)
        INT_SnP_DetachObject(gao, WOR_gpst_CurrentWorld);
    else
        gao->snpIndex = -1;

    ext = gao->extended;
    if (!ext)
        return;
    coll = ext->coll;
    if (!coll)
        return;

    if ((gao->flags2 & 0x20) != 0)
        coll->detectionList->overlapNum = 0;

    if ((gao->flags & 0x600) != 0)
    {
        instance = coll->colInstance;
        if (instance)
        {
            instance->activationFlags = -1;
            instance->crossable = 0x589A;
            if ((gao->flags & 0x400000) != 0)
                MATH_CopyMatrix(instance->mtx, &gao->basic->mtx->mtx);
            else
                MATH_CopyMatrix(instance->mtx, OBJ_pst_GetAbsoluteMatrix(gao));
        }
        else
        {
            gao->flags &= ~0x600u;
        }
    }

    if ((gao->flags & 0x100) != 0)
    {
        colMap = coll->colMap;
        if (colMap)
            colMap->activation = -1;
        else
            gao->flags &= ~0x100u;
    }
}

int INT_FullRayTriangle(Vec3* origin, Vec3* dir, Vec3* p1, Vec3* p2, Vec3* p3, float* distance, Vec3* hitOut)
{
    double pvecX;
    double pvecY;
    double pvecZ;
    double det;
    double vRaw;
    double hitDist;
    float weight1;
    Vec3 edge1;
    float edge2X;
    float edge2Y;
    float edge2Z;
    float qvecX;
    float qvecY;
    float qvecZ;
    float v;
    float u;
    float invDet;
    float toOriginX;
    float toOriginY;
    float toOriginZ;
    Vec3 scaled;

    edge1.x = p2->x - p1->x;
    edge1.y = p2->y - p1->y;
    edge1.z = p2->z - p1->z;
    edge2X = p3->x - p1->x;
    edge2Y = p3->y - p1->y;
    edge2Z = p3->z - p1->z;

    pvecX = edge2Z * dir->y - edge2Y * dir->z;
    pvecY = edge2X * dir->z - edge2Z * dir->x;
    pvecZ = edge2Y * dir->x - edge2X * dir->y;
    det = edge1.z * pvecZ + pvecY * edge1.y + pvecX * edge1.x;
    if (det < 0.0000099999997 && det > -0.0000099999997)
        return 0;

    invDet = (float)(1.0 / det);
    toOriginX = origin->x - p1->x;
    toOriginY = origin->y - p1->y;
    toOriginZ = origin->z - p1->z;
    u = (float)(((origin->z - p1->z) * pvecZ + toOriginY * pvecY + toOriginX * pvecX) * invDet);
    if (u < 0.0)
        return 0;
    if (u > 1.0)
        return 0;

    qvecX = toOriginY * edge1.z - toOriginZ * edge1.y;
    qvecY = toOriginZ * edge1.x - edge1.z * toOriginX;
    qvecZ = toOriginX * edge1.y - toOriginY * edge1.x;
    vRaw = (qvecX * dir->x + qvecY * dir->y + qvecZ * dir->z) * invDet;
    v = (float)vRaw;
    if (vRaw < 0.0 || v + u > 1.0)
        return 0;

    if (hitOut)
    {
        MATH_VecMultScalar(&scaled, p3, v);
        MATH_VecMultScalar(hitOut, p2, u);
        MATH_VecAddVecWithoutBuffer(hitOut, &scaled);
        weight1 = (float)(1.0 - u - v);
        MATH_VecMultScalar(&scaled, p1, weight1);
        MATH_VecAddVecWithoutBuffer(hitOut, &scaled);
    }

    hitDist = (qvecZ * edge2Z + qvecY * edge2Y + qvecX * edge2X) * invDet;
    *distance = (float)hitDist;
    if (hitDist < 0.0)
        return 0;
    return 1;
}


// TODO FIXME, ez át lett írva, ill bele lett javítva a generált kódba nem biztos hogy jó
int INT_FullRayAABBox(Vec3* origin, Vec3* dir, tColBox* box, Vec3* hitOut, float maxDist, float* distOut)
{
    int allInside;
    int xSide;
    int ySide;
    int zSide;
    int axis;
    Vec3* hit;
    double candidate;
    double hitX;
    double hitY;
    double hitZ;
    Vec3 boxMax;
    Vec3 localHit;
    float planeX;
    float planeY;
    float planeZ;
    float enter;

    boxMax = box->pos2;
    allInside = 1;
    if (origin->x >= (double)box->pos1.x)
    {
        if (origin->x <= (double)boxMax.x)
        {
            xSide = 2;
        }
        else
        {
            xSide = 0;
            planeX = box->pos2.x;
            allInside = 0;
        }
    }
    else
    {
        xSide = 1;
        planeX = box->pos1.x;
        allInside = 0;
    }

    if (origin->y >= (double)box->pos1.y)
    {
        if (origin->y <= (double)boxMax.y)
        {
            ySide = 2;
        }
        else
        {
            allInside = 0;
            ySide = 0; // fixed v32.y = 0.0 volt
            planeY = boxMax.y;
        }
    }
    else
    {
        ySide = 1;
        planeY = box->pos1.y;
        allInside = 0;
    }

    if (origin->z >= (double)box->pos1.z)
    {
        if (origin->z <= (double)boxMax.z)
        {
            zSide = 2;
        }
        else
        {
            zSide = 0;
            planeZ = boxMax.z;
            allInside = 0;
        }
    }
    else
    {
        planeZ = box->pos1.z;
        zSide = 1;
        allInside = 0;
    }

    hit = hitOut;
    if (!hitOut)
        hit = &localHit;

    if (allInside)
    {
        *hit = *origin;
        if (distOut)
            *distOut = -1.0;
        return 1;
    }

    axis = 0;
    if (xSide == 2 || dir->x == 0.0f)
        enter = -1.0;
    else
        enter = (planeX - origin->x) / dir->x;

    if (ySide != 2 && dir->y != 0.0f)
    {
        candidate = (planeY - origin->y) / dir->y;
        if (candidate > enter)
        {
            enter = (float)candidate;
            axis = 1;
        }
    }
    if (zSide != 2 && dir->z != 0.0f)
    {
        candidate = (planeZ - origin->z) / dir->z;
        if (candidate > enter)
        {
            enter = (float)candidate;
            axis = 2;
        }
    }

    if (enter < 0.0 || enter > (double)maxDist)
        return 0;
    if (distOut)
        *distOut = enter;

    if (axis != 0)
    {
        hitX = enter * dir->x + origin->x;
        hit->x = (float)hitX;
        if (hitX < box->pos1.x)
            return 0;
        if (hitX > boxMax.x)
            return 0;
    }
    else
    {
        hit->x = planeX;
    }

    if (axis != 1)
    {
        hitY = enter * dir->y + origin->y;
        hit->y = (float)hitY;
        if (hitY < box->pos1.y)
            return 0;
        if (hitY > boxMax.y)
            return 0;
    }
    else
    {
        hit->y = planeY;
    }

    if (axis != 2)
    {
        hitZ = enter * dir->z + origin->z;
        hit->z = (float)hitZ;
        if (hitZ >= box->pos1.z)
        {
            if (hitZ > boxMax.z)
                return 0;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        hit->z = planeZ;
    }
    return axis + 1;
}

int INT_FullRaySphere(Vec3* origin, Vec3* dir, tColSphere* sphere, Vec3* hitOut, float maxDist, float* distOut)
{
    double toCenterX;
    double toCenterY;
    double toCenterZ;
    double projection;
    double perpSquared;
    long double hitDist;
    float projSquared;
    float projF;
    float centerDistSquared;
    float radiusSquared;
    float hitDistF;

    toCenterX = sphere->center.x - origin->x;
    toCenterY = sphere->center.y - origin->y;
    toCenterZ = sphere->center.z - origin->z;
    projection = dir->z * toCenterZ + dir->y * toCenterY + toCenterX * dir->x;
    projF = (float)projection;
    centerDistSquared = (float)(toCenterZ * toCenterZ + toCenterX * toCenterX + toCenterY * toCenterY);
    radiusSquared = sphere->radius * sphere->radius;

    if (radiusSquared <= (double)centerDistSquared)
    {
        if (projF < 0.0 && centerDistSquared > (double)radiusSquared)
            return 0;
        projSquared = (float)(projection * projF);
        perpSquared = centerDistSquared - projSquared;
        if (perpSquared > radiusSquared)
            return 0;
        hitDist = projF - sqrt(radiusSquared - perpSquared);
        *distOut = (float)hitDist;
        if (hitDist > maxDist)
            return 0;

        if (hitOut)
        {
            hitDistF = (float)hitDist;
            MATH_VecMultScalar(hitOut, dir, hitDistF);
            MATH_VecAddVec(hitOut, origin, hitOut);
        }
        return 1;
    }

    *distOut = -1.0;
    return 1;
}

int INT_b_FullRayCylinder(Vec3* origin, Vec3* dir, Vec3* center, Vec3* axis, float radius, float halfHeight, float maxDist, float* distOut, Vec3* hitOut)
{
    double axisDotDir;
    double offset;
    double doubleOffset;
    long double discriminant;
    float toOriginX;
    float toOriginY;
    float toOriginZ;
    float axialOffset;
    float sinSquared;
    float hitDist;
    float cosAngle;

    hitOut->x = 0.0;
    hitOut->y = 0.0;
    hitOut->z = 0.0;
    toOriginX = origin->x - center->x;
    toOriginY = origin->y - center->y;
    toOriginZ = origin->z - center->z;
    axialOffset = toOriginZ * axis->z + toOriginX * axis->x + toOriginY * axis->y;
    axisDotDir = axis->x * dir->x + axis->z * dir->z + dir->y * axis->y;
    cosAngle = (float)axisDotDir;
    if (fabs(fabs(axisDotDir) - 1.0) < 0.0000099999997)
        return 0;

    sinSquared = (float)(1.0 - cosAngle * cosAngle);
    offset = toOriginY * dir->y + toOriginZ * dir->z + toOriginX * dir->x - cosAngle * axialOffset;
    doubleOffset = offset + offset;
    discriminant = doubleOffset * doubleOffset
        - -(axialOffset * axialOffset + radius * radius
            - (toOriginZ * toOriginZ + toOriginY * toOriginY + toOriginX * toOriginX)) * sinSquared * 4.0;
    if (discriminant < 0.0)
        return 0;

    hitDist = (float)((-doubleOffset - sqrt(fabs(discriminant))) / (sinSquared + sinSquared));
    if (hitDist < 0.0 || hitDist > (double)maxDist)
        return 0;

    hitOut->x = hitDist * dir->x;
    hitOut->y = hitDist * dir->y;
    hitOut->z = hitDist * dir->z;
    hitOut->x = hitOut->x + origin->x;
    hitOut->y = origin->y + hitOut->y;
    hitOut->z = hitOut->z + origin->z;
    if (distOut)
        *distOut = hitDist;
    if (fabs(hitDist * cosAngle + axialOffset) > halfHeight)
        return 0;
    return 1;
}

static BOOL COL_l_b_TriangleInRayExtent(Vec3* p1, Vec3* p2, Vec3* p3, Vec3* origin, Vec3* dir, float maxDist)
{
    double stepX;
    double stepY;
    double bound;
    float farX;
    float farY;
    float farZ;

    stepX = maxDist * dir->x;
    stepY = maxDist * dir->y;
    farZ = maxDist * dir->z;
    farX = (float)(origin->x + stepX);
    farY = (float)(stepY + origin->y);
    farZ = origin->z + farZ;

    if (dir->x <= 0.0)
    {
        bound = MATH_f_MinOf3(p1->x, p2->x, p3->x);
        if (bound > origin->x)
            return 0;
        bound = MATH_f_MaxOf3(p1->x, p2->x, p3->x);
        if (bound < farX)
            return 0;
    }
    else
    {
        bound = MATH_f_MaxOf3(p1->x, p2->x, p3->x);
        if (bound < origin->x)
            return 0;
        bound = MATH_f_MinOf3(p1->x, p2->x, p3->x);
        if (bound > farX)
            return 0;
    }

    if (dir->y <= 0.0)
    {
        bound = MATH_f_MinOf3(p1->y, p2->y, p3->y);
        if (bound > origin->y)
            return 0;
        bound = MATH_f_MaxOf3(p1->y, p2->y, p3->y);
        if (bound < farY)
            return 0;
    }
    else
    {
        bound = MATH_f_MaxOf3(p1->y, p2->y, p3->y);
        if (bound < origin->y)
            return 0;
        bound = MATH_f_MinOf3(p1->y, p2->y, p3->y);
        if (bound > farY)
            return 0;
    }

    if (dir->z <= 0.0)
    {
        bound = MATH_f_MinOf3(p1->z, p2->z, p3->z);
        if (bound > origin->z)
            return 0;
        bound = MATH_f_MaxOf3(p1->z, p2->z, p3->z);
        if (bound < farZ)
            return 0;
    }
    else
    {
        bound = MATH_f_MaxOf3(p1->z, p2->z, p3->z);
        if (bound < origin->z)
            return 0;
        bound = MATH_f_MinOf3(p1->z, p2->z, p3->z);
        if (bound > farZ)
            return 0;
    }
    return 1;
}

static int COL_l_i_RayCrossable(void)
{
    if (COL_gst_Ray.useUserCrossable)
        return COL_guw_UserCrossable;
    if (COL_gst_Ray.colMapGao && (COL_gst_Ray.colMapGao->flags & 0x200) != 0)
        return COL_gst_Ray.colMapGao->extended->coll->colInstance->crossable;
    return 0x589A;                              // == usercrossable?
}

void COL_ColMap_Triangles_RayCast(tGameObjectMain* gao, tColMap* colMap)
{
    tCobMeshObjectMesh* mesh;
    Mtx4x4* mtx;
    tColCollisionObject* cob;
    tCobGameMaterialElems* gameMaterial;
    tCobMeshChunkElem* chunk;
    tCobMeshChunkElem* chunkEnd;
    tCobMeshTriangleElem* tri;
    tCobMeshTriangleElem* triEnd;
    Vec3* verts;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    Vec3* dir;
    long double invLength;
    int crossable;
    int globalTriangle;
    int chunkID;
    int triangleID;
    Vec3 origin;
    Vec3 localDir;
    Vec3 normal;
    Vec3 hitPos;
    float distance;
    Mtx4x4 noScaleMtx;
    Mtx4x4 invMtx;

    mesh = (tCobMeshObjectMesh*)(*colMap->collisionObjectPtr)->objectPtrByType;
    mtx = gao->matrices;
    if ((mtx->mtxflag & 8) == 0)
    {
        MATH_InvertMatrix(&invMtx, mtx);
    }
    else
    {
        MATH_CopyMatrix(&noScaleMtx, mtx);
        noScaleMtx.mtxflag &= ~8u;
        noScaleMtx._m14 = 0.0;
        noScaleMtx._m24 = 0.0;
        noScaleMtx._m34 = 0.0;
        MATH_InvertMatrix(&invMtx, &noScaleMtx);
    }

    chunk = mesh->meshChunkElems;
    chunkEnd = &chunk[mesh->meshChunkElemNum];
    mtx = gao->matrices;
    if ((mtx->mtxflag & 0x3C) == 0)
    {
        dir = COL_gst_Ray.direction;
        origin.x = COL_gst_Ray.sourcePoint->x - mtx->Pos.x;
        origin.y = COL_gst_Ray.sourcePoint->y - mtx->Pos.y;
        origin.z = COL_gst_Ray.sourcePoint->z - mtx->Pos.z;
    }
    else
    {
        MATH_TransformVertex(&origin, &invMtx, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&localDir, &invMtx, COL_gst_Ray.direction);
        dir = &localDir;
        invLength = 1.0 / sqrt(localDir.z * localDir.z + localDir.y * localDir.y + localDir.x * localDir.x);
        localDir.x = (float)(localDir.x * invLength);
        localDir.y = (float)(localDir.y * invLength);
        localDir.z = (float)(localDir.z * invLength);
    }

    globalTriangle = 0;
    for (chunkID = 0; chunk < chunkEnd; ++chunk, ++chunkID)
    {
        if (COL_gst_Ray.skipFlagX || COL_gst_Ray.skipCrossable || COL_gst_Ray.skipCamera)
        {
            cob = *colMap->collisionObjectPtr;
            if (cob->gameMaterial)
            {
                crossable = COL_l_i_RayCrossable();
                gameMaterial = COL_pst_GMat_Get(cob, chunk);
                if (COL_gst_Ray.skipCrossable && gameMaterial && ((uint16_t)crossable & gameMaterial->flags) != 0
                    || COL_gst_Ray.skipCamera && gameMaterial && (gameMaterial->flags & 0x1000) != 0
                    || COL_gst_Ray.skipFlagX && gameMaterial && (gameMaterial->flags & 0x200000) != 0)
                {
                    globalTriangle += chunk->elemNum;
                    continue;
                }
            }
        }

        tri = chunk->triangleElems;
        triEnd = &tri[chunk->elemNum];
        for (triangleID = 0; tri < triEnd; ++tri, ++triangleID, ++globalTriangle)
        {
            normal = mesh->normals[globalTriangle];
            verts = mesh->vertices;
            p2 = &verts[tri->v2];
            p1 = &verts[tri->v1];
            p3 = &verts[tri->v3];
            if (normal.y * dir->y + normal.z * dir->z + normal.x * dir->x <= 0.0)
            {
                if (!COL_l_b_TriangleInRayExtent(p1, p2, p3, &origin, dir, COL_gst_Ray.distance))
                    continue;
                if (INT_FullRayTriangle(&origin, dir, p1, p2, p3, &distance, &hitPos)
                    && distance < (double)COL_gst_Ray.distance
                    && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
                {
                    if (COL_gst_Ray.ignoreColMapRayCastResult)
                    {
                        *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                        return;
                    }
                    MATH_TransformVertexNoScale(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, gao->matrices, &hitPos);
                    MATH_TransformVectorNoScale(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, gao->matrices, &normal);
                    MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = chunkID;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = triangleID;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = globalTriangle;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = COL_pst_GMat_Get(*colMap->collisionObjectPtr, chunk);
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = chunk->designVal;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
                    COL_gst_Ray.gao = gao;
                    COL_gst_Ray.distance = distance;
                }
            }
        }
    }
}

void COL_ColMap_OK3_RayCast(tGameObjectMain* gao, tColMap* colMap)
{
    Mtx4x4* mtx;
    tCobMeshObjectMesh* mesh;
    tCobOK3* ok3;
    tCobOK3AABBoxElem* aaBox;
    tCobOK3AABBoxElemChunkElem* chunkElem;
    tColCollisionObject* cob;
    tCobGameMaterialElems* gameMaterial;
    tCobMeshTriangleElem* tris;
    Vec3* verts;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    Vec3* dir;
    long double invLength;
    int chunkID;
    int triangleBase;
    int triangleID;
    int crossable;
    unsigned int boxID;
    int chunkElemID;
    int elemID;
    int k;
    Vec3 origin;
    Vec3 localDir;
    Vec3 normal;
    Vec3 hitPos;
    tColBox aabb;
    float distance;
    Mtx4x4 noScaleMtx;
    Mtx4x4 invMtx;

    mtx = gao->matrices;
    mesh = (tCobMeshObjectMesh*)(*colMap->collisionObjectPtr)->objectPtrByType;
    ok3 = mesh->ok3;
    if ((mtx->mtxflag & 8) == 0)
    {
        MATH_InvertMatrix(&invMtx, mtx);
    }
    else
    {
        MATH_CopyMatrix(&noScaleMtx, mtx);
        noScaleMtx._m14 = 0.0;
        noScaleMtx._m24 = 0.0;
        noScaleMtx._m34 = 0.0;
        noScaleMtx.mtxflag &= ~8u;
        MATH_InvertMatrix(&invMtx, &noScaleMtx);
    }

    mtx = gao->matrices;
    if ((mtx->mtxflag & 0x3C) != 0)
    {
        MATH_TransformVertex(&origin, &invMtx, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&localDir, &invMtx, COL_gst_Ray.direction);
        dir = &localDir;
        invLength = 1.0 / sqrt(localDir.z * localDir.z + localDir.y * localDir.y + localDir.x * localDir.x);
        localDir.x = (float)(localDir.x * invLength);
        localDir.y = (float)(localDir.y * invLength);
        localDir.z = (float)(localDir.z * invLength);
    }
    else
    {
        dir = COL_gst_Ray.direction;
        origin.x = COL_gst_Ray.sourcePoint->x - mtx->Pos.x;
        origin.y = COL_gst_Ray.sourcePoint->y - mtx->Pos.y;
        origin.z = COL_gst_Ray.sourcePoint->z - mtx->Pos.z;
    }

    for (boxID = 0; boxID < ok3->num; ++boxID)
    {
        aaBox = &ok3->aaBoxes[boxID];
        aabb.pos1.x = aaBox->vec1.x;
        aabb.pos1.y = aaBox->vec1.y;
        aabb.pos1.z = aaBox->vec1.z;
        aabb.pos2 = aaBox->vec2;
        if (!INT_FullRayAABBox(&origin, dir, &aabb, 0, COL_gst_Ray.distance, 0))
            continue;

        for (chunkElemID = 0; chunkElemID < (int)aaBox->num; ++chunkElemID)
        {
            chunkElem = &aaBox->chunkElems[chunkElemID];
            chunkID = chunkElem->triangleChunkID;
            triangleBase = 0;
            for (k = 0; k < chunkID; ++k)
                triangleBase += mesh->meshChunkElems[k].elemNum;

            gameMaterial = 0;                           // 0x589A == usercrossable?
            if (COL_gst_Ray.skipFlagX || COL_gst_Ray.skipCrossable || COL_gst_Ray.skipCamera)
            {
                cob = *colMap->collisionObjectPtr;
                if (cob->gameMaterial)
                {
                    crossable = COL_l_i_RayCrossable();
                    gameMaterial = COL_pst_GMat_Get(cob, &mesh->meshChunkElems[chunkID]);
                    if (COL_gst_Ray.skipCrossable && gameMaterial && ((uint16_t)crossable & gameMaterial->flags) != 0
                        || COL_gst_Ray.skipCamera && gameMaterial && (gameMaterial->flags & 0x1000) != 0
                        || COL_gst_Ray.skipFlagX && gameMaterial && (gameMaterial->flags & 0x200000) != 0)
                    {
                        continue;
                    }
                }
            }

            for (elemID = 0; elemID < (int)chunkElem->num; ++elemID)
            {
                triangleID = chunkElem->triangleChunkElemIDs[elemID];
                tris = mesh->meshChunkElems[chunkElem->triangleChunkID].triangleElems;
                verts = mesh->vertices;
                p3 = &verts[tris[triangleID].v3];
                p1 = &verts[tris[triangleID].v1];
                p2 = &verts[tris[triangleID].v2];
                normal = mesh->normals[triangleBase + triangleID];
                if (normal.z * dir->z + normal.x * dir->x + normal.y * dir->y > 0.0)
                    continue;
                if (!COL_l_b_TriangleInRayExtent(p1, p2, p3, &origin, dir, COL_gst_Ray.distance))
                    continue;

                if (INT_FullRayTriangle(&origin, dir, p1, p2, p3, &distance, &hitPos)
                    && distance < (double)COL_gst_Ray.distance
                    && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
                {
                    if (COL_gst_Ray.ignoreColMapRayCastResult)
                    {
                        *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                        return;
                    }
                    MATH_TransformVertexNoScale(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, gao->matrices, &hitPos);
                    MATH_TransformVectorNoScale(
                        &COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec,
                        gao->matrices,
                        &normal);
                    MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = chunkElem->triangleChunkID;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = triangleID;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = triangleBase + triangleID;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = gameMaterial;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = mesh->meshChunkElems[chunkElem->triangleChunkID].designVal;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
                    COL_gst_Ray.distance = distance;
                    COL_gst_Ray.gao = gao;
                }
            }
        }
    }
}

void COL_Visual_OK3_RayCast(tGameObjectMain* gao, tGEOGRO* geogro)
{
    tCobOK3* ok3;
    tCobOK3AABBoxElem* aaBox;
    tCobOK3AABBoxElemChunkElem* chunkElem;
    tGeoTriangleChunks* geoChunk;
    tGeoTriangleChunkElem* tri;
    Vec3* pos;
    Mtx4x4* mtx;
    Vec3* rayDir;
    Vec3* dir;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    double sourceX;
    long double invLength;
    double edge1X;
    double edge1Y;
    double edge1Z;
    double edge2Z;
    int chunkID;
    int triangleBase;
    unsigned int boxID;
    int chunkElemID;
    int elemID;
    int k;
    float edge2X;
    float edge2Y;
    float distance;
    float maxDist;
    Vec3 scaled;
    Vec3 scaledDir;
    Vec3 origin;
    Vec3 localDir;
    Vec3 normal;
    Vec3 hitPos;
    tColBox aabb;
    Mtx4x4 invMtx;
    Mtx4x4 oglMtx;

    ok3 = geogro->collisionPtr;
    distance = 0.0;
    MATH_MakeOGLMatrix(&oglMtx, gao->matrices);
    oglMtx._m14 = 0.0;
    oglMtx._m24 = 0.0;
    oglMtx._m34 = 0.0;
    oglMtx.mtxflag &= ~8u;
    MATH_Invert33Matrix(&invMtx, &oglMtx);
    invMtx._m14 = 0.0;
    invMtx._m24 = 0.0;
    invMtx._m34 = 0.0;
    pos = &gao->matrices->Pos;
    invMtx.mtxflag &= ~8u;
    invMtx.Pos.x = -pos->x;
    invMtx.Pos.y = -pos->y;
    invMtx.Pos.z = -pos->z;
    MATH_TransformVector(&invMtx.Pos, &invMtx, &invMtx.Pos);

    mtx = gao->matrices;
    if ((mtx->mtxflag & 0x3C) != 0)
    {
        MATH_TransformVertex(&origin, &invMtx, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&localDir, &invMtx, COL_gst_Ray.direction);
        rayDir = COL_gst_Ray.direction;
        dir = &localDir;
        invLength = 1.0 / sqrt(localDir.z * localDir.z + localDir.y * localDir.y + localDir.x * localDir.x);
        localDir.x = (float)(localDir.x * invLength);
        localDir.y = (float)(localDir.y * invLength);
        localDir.z = (float)(localDir.z * invLength);
    }
    else
    {
        rayDir = COL_gst_Ray.direction;
        sourceX = COL_gst_Ray.sourcePoint->x - mtx->Pos.x;
        dir = COL_gst_Ray.direction;
        origin.x = (float)sourceX;
        origin.y = (float)(COL_gst_Ray.sourcePoint->y - mtx->Pos.y);
        origin.z = (float)(COL_gst_Ray.sourcePoint->z - mtx->Pos.z);
    }

    if ((gao->matrices->mtxflag & 8) != 0)
    {
        scaled.x = COL_gst_Ray.distance * rayDir->x;
        scaled.y = rayDir->y * COL_gst_Ray.distance;
        scaled.z = rayDir->z * COL_gst_Ray.distance;
        MATH_TransformVector(&scaledDir, &invMtx, &scaled);
        maxDist = (float)sqrt(scaledDir.x * scaledDir.x + scaledDir.z * scaledDir.z + scaledDir.y * scaledDir.y);
    }
    else
    {
        maxDist = COL_gst_Ray.distance;
    }

    for (boxID = 0; boxID < ok3->num; ++boxID)
    {
        aaBox = &ok3->aaBoxes[boxID];
        aabb.pos1.x = aaBox->vec1.x;
        aabb.pos1.y = aaBox->vec1.y;
        aabb.pos1.z = aaBox->vec1.z;
        aabb.pos2 = aaBox->vec2;
        if (!INT_FullRayAABBox(&origin, dir, &aabb, 0, maxDist, 0))
            continue;

        // preserved: this scales by the hit distance of the previous triangle,
        // which is zero until one is found, and nothing reads the result
        if ((gao->matrices->mtxflag & 8) != 0)
        {
            scaledDir.x = distance * COL_gst_Ray.direction->x;
            scaledDir.y = COL_gst_Ray.direction->y * distance;
            scaledDir.z = COL_gst_Ray.direction->z * distance;
            MATH_TransformVector(&scaled, &oglMtx, &scaledDir);
            distance = (float)sqrt(scaled.x * scaled.x + scaled.z * scaled.z + scaled.y * scaled.y);
        }

        for (chunkElemID = 0; chunkElemID < (int)aaBox->num; ++chunkElemID)
        {
            chunkElem = &aaBox->chunkElems[chunkElemID];
            chunkID = chunkElem->triangleChunkID;
            triangleBase = 0;
            for (k = 0; k < chunkID; ++k)
                triangleBase += geogro->triangleChunks[k].triangleNum;

            if (!geogro->triangleChunks[chunkID].triangleChunkElems)
                continue;

            for (elemID = 0; elemID < (int)chunkElem->num; ++elemID)
            {
                tri = &geogro->triangleChunks[chunkElem->triangleChunkID].triangleChunkElems[chunkElem->triangleChunkElemIDs[elemID]];
                p1 = &geogro->vertices[tri->v1];
                p2 = &geogro->vertices[tri->v2];
                p3 = &geogro->vertices[tri->v3];
                edge1X = p2->x - p1->x;
                edge1Y = p2->y - p1->y;
                edge1Z = p2->z - p1->z;
                edge2X = p3->x - p1->x;
                edge2Y = p3->y - p1->y;
                edge2Z = p3->z - p1->z;
                normal.x = (float)(edge2Z * edge1Y - edge2Y * edge1Z);
                normal.y = (float)(edge1Z * edge2X - edge2Z * edge1X);
                normal.z = (float)(edge2Y * edge1X - edge1Y * edge2X);
                if (normal.y * dir->y + normal.x * dir->x + normal.z * dir->z > 0.0)
                    continue;
                if (!COL_l_b_TriangleInRayExtent(p1, p2, p3, &origin, dir, maxDist))
                    continue;

                if (INT_FullRayTriangle(&origin, dir, p1, p2, p3, &distance, &hitPos)
                    && distance < (double)maxDist
                    && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
                {
                    if (COL_gst_Ray.ignoreColMapRayCastResult)
                    {
                        *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                        return;
                    }
                    if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                        COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = hitPos;
                    else
                        MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &oglMtx, &hitPos);
                    if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                        COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec = normal;
                    else
                        MATH_TransformVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, &oglMtx, &normal);
                    MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = chunkElem->triangleChunkID;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = chunkElem->triangleChunkElemIDs[elemID];
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = triangleBase
                        + chunkElem->triangleChunkElemIDs[elemID];
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
                    COL_gst_Ray.gao = gao;
                    if ((gao->matrices->mtxflag & 8) != 0)
                    {
                        scaledDir.x = distance * dir->x;
                        scaledDir.y = distance * dir->y;
                        scaledDir.z = distance * dir->z;
                        MATH_TransformVector(&scaled, &oglMtx, &scaledDir);
                        distance = MATH_VecLen(&scaled);
                    }
                    COL_gst_Ray.distance = distance;
                    if ((gao->matrices->mtxflag & 8) != 0)
                    {
                        scaled.x = distance * COL_gst_Ray.direction->x;
                        scaled.y = COL_gst_Ray.direction->y * distance;
                        scaled.z = COL_gst_Ray.direction->z * distance;
                        MATH_TransformVector(&scaledDir, &invMtx, &scaled);
                        maxDist = MATH_VecLen(&scaledDir);
                    }
                    else
                    {
                        maxDist = distance;
                    }
                }
            }
        }
    }
}

void COL_Visual_Triangles_RayCast(tGameObjectMain* gao, tGEOGRO* geogro)
{
    Vec3* pos;
    Mtx4x4* mtx;
    Vec3* rayDir;
    Vec3* dir;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    tGeoTriangleChunks* chunk;
    tGeoTriangleChunks* chunkEnd;
    tGeoTriangleChunkElem* tri;
    tGeoTriangleChunkElem* triEnd;
    double sourceX;
    long double invLength;
    double edge1X;
    double edge1Y;
    double edge1Z;
    double edge2Z;
    int globalTriangle;
    int chunkID;
    int triangleID;
    float edge2X;
    float edge2Y;
    float distance;
    float maxDist;
    Vec3 origin;
    Vec3 localDir;
    Vec3 normal;
    Vec3 hitPos;
    Vec3 scaled;
    Vec3 scaledDir;
    Mtx4x4 invMtx;
    Mtx4x4 oglMtx;

    MATH_MakeOGLMatrix(&oglMtx, gao->matrices);
    oglMtx._m14 = 0.0;
    oglMtx._m24 = 0.0;
    oglMtx._m34 = 0.0;
    oglMtx.mtxflag &= ~8u;
    MATH_Invert33Matrix(&invMtx, &oglMtx);
    invMtx.mtxflag &= ~8u;
    pos = &gao->matrices->Pos;
    invMtx._m14 = 0.0;
    invMtx._m24 = 0.0;
    invMtx._m34 = 0.0;
    invMtx.Pos.x = -pos->x;
    invMtx.Pos.y = -pos->y;
    invMtx.Pos.z = -pos->z;
    MATH_TransformVector(&invMtx.Pos, &invMtx, &invMtx.Pos);

    mtx = gao->matrices;
    if ((mtx->mtxflag & 0x3C) != 0)
    {
        MATH_TransformVertex(&origin, &invMtx, COL_gst_Ray.sourcePoint);
        MATH_TransformVector(&localDir, &invMtx, COL_gst_Ray.direction);
        rayDir = COL_gst_Ray.direction;
        dir = &localDir;
        invLength = 1.0 / sqrt(localDir.z * localDir.z + localDir.y * localDir.y + localDir.x * localDir.x);
        localDir.x = (float)(localDir.x * invLength);
        localDir.y = (float)(localDir.y * invLength);
        localDir.z = (float)(localDir.z * invLength);
    }
    else
    {
        rayDir = COL_gst_Ray.direction;
        sourceX = COL_gst_Ray.sourcePoint->x - mtx->Pos.x;
        dir = COL_gst_Ray.direction;
        origin.x = (float)sourceX;
        origin.y = COL_gst_Ray.sourcePoint->y - mtx->Pos.y;
        origin.z = COL_gst_Ray.sourcePoint->z - mtx->Pos.z;
    }

    if ((gao->matrices->mtxflag & 8) != 0)
    {
        scaled.x = COL_gst_Ray.distance * rayDir->x;
        scaled.y = rayDir->y * COL_gst_Ray.distance;
        scaled.z = rayDir->z * COL_gst_Ray.distance;
        MATH_TransformVector(&scaledDir, &invMtx, &scaled);
        maxDist = (float)sqrt(scaledDir.z * scaledDir.z + scaledDir.y * scaledDir.y + scaledDir.x * scaledDir.x);
    }
    else
    {
        maxDist = COL_gst_Ray.distance;
    }

    chunk = geogro->triangleChunks;
    chunkEnd = &chunk[geogro->triangleChunkCount];
    globalTriangle = 0;
    for (chunkID = 0; chunk < chunkEnd; ++chunk, ++chunkID)
    {
        tri = chunk->triangleChunkElems;
        if (!tri)
            continue;
        triEnd = &tri[chunk->triangleNum];

        for (triangleID = 0; tri < triEnd; ++tri, ++triangleID, ++globalTriangle)
        {
            p1 = &geogro->vertices[tri->v1];
            p2 = &geogro->vertices[tri->v2];
            p3 = &geogro->vertices[tri->v3];
            if (!COL_l_b_TriangleInRayExtent(p1, p2, p3, &origin, dir, maxDist))
                continue;

            edge1X = p2->x - p1->x;
            edge1Y = p2->y - p1->y;
            edge1Z = p2->z - p1->z;
            edge2X = p3->x - p1->x;
            edge2Y = p3->y - p1->y;
            edge2Z = p3->z - p1->z;
            normal.x = (float)(edge2Z * edge1Y - edge2Y * edge1Z);
            normal.y = (float)(edge1Z * edge2X - edge2Z * edge1X);
            normal.z = (float)(edge2Y * edge1X - edge1Y * edge2X);

            if (normal.z * dir->z + normal.y * dir->y + normal.x * dir->x <= 0.0
                && INT_FullRayTriangle(&origin, dir, p1, p2, p3, &distance, &hitPos)
                && distance < (double)maxDist
                && (!COL_gb_UseMinDistance || distance > (double)COL_gf_MinDistance))
            {
                if (COL_gst_Ray.ignoreColMapRayCastResult)
                {
                    *COL_gst_Ray.ignoreColMapRayCastResult = 0;
                    return;
                }
                if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                    COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = hitPos;
                else
                    MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &oglMtx, &hitPos);
                if (COL_gst_Ray.skipVecsTransformationToGaoSpace)
                    COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec = normal;
                else
                    MATH_TransformVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, &oglMtx, &normal);
                MATH_NormalizeEqualVector(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec);
                COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = chunkID;
                COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = triangleID;
                COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = globalTriangle;
                COL_gst_Ray.gao = gao;
                if ((gao->matrices->mtxflag & 8) != 0)
                {
                    MATH_VecMultScalar(&scaledDir, dir, distance);
                    MATH_TransformVector(&scaled, &oglMtx, &scaledDir);
                    distance = MATH_VecLen(&scaled);
                }
                COL_gst_Ray.distance = distance;
                if ((gao->matrices->mtxflag & 8) != 0)
                {
                    MATH_VecMultScalar(&scaled, COL_gst_Ray.direction, distance);
                    MATH_TransformVector(&scaledDir, &invMtx, &scaled);
                    maxDist = MATH_VecLen(&scaledDir);
                }
                else
                {
                    maxDist = distance;
                }
            }
        }
    }
}

void COL_ColMap_Box_RayCast(tGameObjectMain* gao, tCobNormalObjectGeneric * box)
{
    Vec3* pos;
    long double invLength;
    tColBox* shape;
    Mtx4x4* mtx;
    float hitDist;
    Vec3 localDir;
    Vec3 scaledDir;
    Vec3 scratch;
    float maxDist;
    tColBox localBox;
    Vec3 hitPos;
    Mtx4x4 invMtx;
    Vec3 localSource;
    Mtx4x4 oglMtx;

    MATH_MakeOGLMatrix(&oglMtx, gao->matrices);
    oglMtx._m14 = 0.0;
    oglMtx._m24 = 0.0;
    oglMtx._m34 = 0.0;
    oglMtx.mtxflag &= ~8u;
    MATH_Invert33Matrix(&invMtx, &oglMtx);
    pos = &gao->matrices->Pos;
    invMtx.mtxflag &= ~8u;
    invMtx._m14 = 0.0;
    invMtx._m24 = 0.0;
    invMtx._m34 = 0.0;
    invMtx.Pos.x = -pos->x;
    invMtx.Pos.y = -pos->y;
    invMtx.Pos.z = -pos->z;
    MATH_TransformVector(&invMtx.Pos, &invMtx, &invMtx.Pos);
    MATH_TransformVertex(&localSource, &invMtx, COL_gst_Ray.sourcePoint);
    MATH_TransformVector(&localDir, &invMtx, COL_gst_Ray.direction);
    invLength = 1.0 / sqrt(localDir.z * localDir.z + localDir.y * localDir.y + localDir.x * localDir.x);
    localDir.x = (float)(localDir.x * invLength);
    localDir.y = (float)(localDir.y * invLength);
    shape = (tColBox *)(box->enclosurePtr);
    localDir.z = (float)(invLength * localDir.z);
    localBox.pos2 = shape->pos1;
    localBox.pos1.x = shape->pos2.x;
    mtx = gao->matrices;
    localBox.pos1.y = shape->pos2.y;
    localBox.pos1.z = shape->pos2.z;

    if ((mtx->mtxflag & 8) != 0)
    {
        scratch.x = COL_gst_Ray.distance * COL_gst_Ray.direction->x;
        scratch.y = COL_gst_Ray.direction->y * COL_gst_Ray.distance;
        scratch.z = COL_gst_Ray.direction->z * COL_gst_Ray.distance;
        MATH_TransformVector(&scaledDir, &invMtx, &scratch);
        maxDist = (float)sqrt(scaledDir.z * scaledDir.z + scaledDir.y * scaledDir.y + scaledDir.x * scaledDir.x);
    }
    else
    {
        maxDist = COL_gst_Ray.distance;
    }

    if (!INT_FullRayAABBox(&localSource, &localDir, &localBox, &hitPos, maxDist, &hitDist))
        return;

    if ((gao->matrices->mtxflag & 8) != 0)
    {
        scaledDir.x = hitDist * COL_gst_Ray.direction->x;
        scaledDir.y = COL_gst_Ray.direction->y * hitDist;
        scaledDir.z = COL_gst_Ray.direction->z * hitDist;
        MATH_TransformVector(&scratch, &oglMtx, &scaledDir);
        hitDist = (float)sqrt(scratch.z * scratch.z + scratch.y * scratch.y + scratch.x * scratch.x);
    }

    if (!COL_gb_UseMinDistance || hitDist > (double)COL_gf_MinDistance)
    {
        MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &oglMtx, &hitPos);
        MATH_TransformVertex(&COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos, &oglMtx, &hitPos);
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
        COL_gst_Ray.gao = gao;
        COL_gst_Ray.distance = hitDist;
    }
}

void COL_ColMap_Sphere_RayCast(tGameObjectMain* gao, tCobNormalObjectGeneric* shape)
{
    Mtx4x4* mtx;
    double maxScale;
    double radius;
    double offsetX;
    double offsetY;
    Vec3* normal;
    tColSphere* srcSphere;
    Vec3 hitPos;
    Vec3 scale;
    float offsetZ;
    tColSphere sphere;
    float srcRadius;
    float invLength;
    float hitDist;

    srcSphere = shape->enclosurePtr;
    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_TransformVertex(&sphere.center, mtx, &srcSphere->center);

    if ((gao->matrices->mtxflag & 8) != 0)
    {
        MATH_GetScale(&scale, OBJ_pst_GetAbsoluteMatrix(gao));
        srcRadius = ((tColSphere*)(shape->enclosurePtr))->radius;
        maxScale = MATH_f_MaxOf3(scale.x, scale.y, scale.z);
        radius = maxScale * srcRadius;
    }
    else
    {
        radius = ((tColSphere*)(shape->enclosurePtr))->radius;
    }
    sphere.radius = (float)radius;

    if (INT_FullRaySphere(COL_gst_Ray.sourcePoint, COL_gst_Ray.direction, &sphere, &hitPos, COL_gst_Ray.distance, &hitDist)
        && (!COL_gb_UseMinDistance || hitDist > (double)COL_gf_MinDistance))
    {
        COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = hitPos;
        offsetX = hitPos.x - sphere.center.x;
        offsetY = hitPos.y - sphere.center.y;
        normal = &COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec;
        offsetZ = hitPos.z - sphere.center.z;
        invLength = (float)(1.0 / sqrt(offsetZ * offsetZ + offsetY * offsetY + offsetX * offsetX));// normalizevector inline
        COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec.x = (float)(offsetX * invLength);
        normal->y = (float)(offsetY * invLength);
        normal->z = offsetZ * invLength;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
        COL_gst_Ray.gao = gao;
        COL_gst_Ray.distance = hitDist;
    }
}

void COL_ColMap_Cylinder_RayCast(tGameObjectMain* gao, tCobNormalObjectGeneric* shape)
{
    Mtx4x4* mtx;
    double scaleXY;
    tColCylinder* cylinder;
    float scaledHeight;
    float scaledRadius;
    Vec3 scale;
    Vec3 hitPos;
    Vec3 center;
    float hitDist;

    cylinder = (tColCylinder*)(shape->enclosurePtr);
    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_TransformVertex(&center, mtx, &cylinder->center);
    MATH_GetScale(&scale, OBJ_pst_GetAbsoluteMatrix(gao));
    if (scale.x < (double)scale.y)
        scaleXY = scale.y;
    else
        scaleXY = scale.x;
    scaledHeight = (float)(((tColCylinder*)(shape->enclosurePtr))->height * scaleXY);
    scaledRadius = ((tColCylinder*)(shape->enclosurePtr))->radius * scale.z;

    if (INT_b_FullRayCylinder(COL_gst_Ray.sourcePoint, COL_gst_Ray.direction, &center, &MATH_gst_BaseVectorK, scaledHeight, scaledRadius, COL_gst_Ray.distance, &hitDist, &hitPos)
        && (!COL_gb_UseMinDistance || hitDist > (double)COL_gf_MinDistance))
    {
        COL_gst_Ray.worldSetInColMapRayCast->colRayHitPos = hitPos;
        MATH_VecInvert(&COL_gst_Ray.worldSetInColMapRayCast->rayCastNormalVec, COL_gst_Ray.direction);
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastTriangleChunkLastElemID = -1;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobGmat = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastCobMeshChunkElemsDesignVal = 0;
        COL_gst_Ray.worldSetInColMapRayCast->rayCastGao = gao;
        COL_gst_Ray.gao = gao;
        COL_gst_Ray.distance = hitDist;
    }
}

// TODO ÁTNÉZNI!
int COL_RaySphereBV(Vec3* origin, Vec3* dir, tGameObjectMain* gao, float* distOut, float maxDist)
{
    tGameObjectBV* bv;
    Mtx4x4* mtx;
    double centerX;
    tColSphere sphere;
    float x, y, z;
    float radius;

    bv = gao->boundingVolume;
    mtx = gao->matrices;
    centerX = bv->BVMin.x + mtx->Pos.x;
    sphere.center.x = (float)centerX;
    sphere.center.y = bv->BVMin.y + mtx->Pos.y;
    sphere.center.z = bv->BVMin.z + mtx->Pos.z;
    sphere.radius = bv->BVMax.x;


    radius = bv->BVMax.x;
    x = radius;
    if (dir->x < 0.0) { // TODO FIXME assemblyben megnéztem, olyan mint ha el lenne írva, mert mindenhez az x-et nézi, Rendben van mert ez a SUGÁR!!
        x = -radius;
    }
    y = radius;
    if (dir->x < 0.0) {
        y = -radius;
    }
    z = radius;
    if (dir->x < 0.0) {
        z = -radius;
    }

    if (((x + sphere.center.x) - origin->x) * dir->x + dir->y * ((y + sphere.center.y) - origin->y) + dir->z * ((z + sphere.center.z) - origin->z) < 0.0)
        return 0;
    return INT_FullRaySphere(origin, dir, &sphere, 0, maxDist, distOut);
}

int COL_RayAABBox(Vec3* origin, Vec3* dir, tGameObjectMain* gao, float* distOut, float maxDist)
{
    Mtx4x4* mtx;
    tGameObjectBV* bv;
    double minX;
    float farX;
    float farY;
    float stepZ;
    float farZ;
    tColBox box;

    mtx = gao->matrices;
    bv = gao->boundingVolume;
    box.pos2.x = bv->BVMax.x + mtx->Pos.x;
    box.pos2.y = bv->BVMax.y + mtx->Pos.y;
    box.pos2.z = bv->BVMax.z + mtx->Pos.z;
    minX = mtx->Pos.x + bv->BVMin.x;
    box.pos1.x = (float)minX;
    box.pos1.y = mtx->Pos.y + bv->BVMin.y;
    box.pos1.z = mtx->Pos.z + bv->BVMin.z;
    stepZ = maxDist * dir->z;
    farX = maxDist * dir->x + origin->x;
    farY = maxDist * dir->y + origin->y;
    farZ = stepZ + origin->z;

    if (dir->x <= 0.0)
    {
        if (minX > origin->x || farX > (double)box.pos2.x)
            return 0;
    }
    else
    {
        if (origin->x > (double)box.pos2.x)
            return 0;
        if (minX > farX)
            return 0;
    }

    if (dir->y <= 0.0)
    {
        if (box.pos1.y > (double)origin->y || farY > (double)box.pos2.y)
            return 0;
    }
    else
    {
        if (origin->y > (double)box.pos2.y)
            return 0;
        if (box.pos1.y > (double)farY)
            return 0;
    }

    if (dir->z <= 0.0)
    {
        if (box.pos1.z > (double)origin->z || farZ > (double)box.pos2.z)
            return 0;
    }
    else
    {
        if (origin->z > (double)box.pos2.z)
            return 0;
        if (box.pos1.z > (double)farZ)
            return 0;
    }
    return INT_FullRayAABBox(origin, dir, &box, 0, maxDist, distOut);
}

struct sGameObjectMain* COL_Visual_RayCast(struct sWorld* world, struct sGameObjectMain* skipGao, Vec3* source, Vec3* direction, float maxDist, int flagMustHave, int flagMustNothave, int whatToTest, int* ignoreResult, int skipRayCast)
{
    tableElemStructPF* elem;
    tableElemStructPF* end;
    tGameObjectMain* currGao;
    tWorld* rayWorld;
    int gaoFlags;
    tGEOGRO* geo;
    tColMap* colMap;
    tColCollisionObject* firstCob;
    tColCollisionObject* cob;
    tCobNormalObjectGeneric* shape;
    uint8_t i;
    float hitDist;

    COL_gst_Ray.direction = direction;
    COL_gst_Ray.distance = maxDist;
    COL_gst_Ray.worldSetInColMapRayCast = world;
    COL_gst_Ray.sourcePoint = source;
    COL_gst_Ray.gao = 0;
    COL_gst_Ray.ignoreColMapRayCastResult = ignoreResult;
    COL_gst_Ray.skipRayCast = skipRayCast;
    COL_gst_Ray.skipVecsTransformationToGaoSpace = 0;
    if (ignoreResult)
        *ignoreResult = 1;
    world->colCrossObjectNumber = 0;

    elem = world->visibleGameObjectsPF.firstElemPtr;
    end = ((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr)) - 1;
    for (; elem <= end; ++elem)
    {
        currGao = (tGameObjectMain*)elem->objectPtr;
        if (!elem->objectPtr
            || ((uint8_t)currGao & 3) == 1
            || !OBJ_b_test_flag(currGao, flagMustHave, flagMustNothave, whatToTest)
            || (currGao->flags & 0x4000) == 0 && (currGao->flags & 2) == 0
            || currGao == skipGao
            || !(OBJ_BV_IsSphere(currGao->boundingVolume)
                 && COL_RaySphereBV(source, direction, currGao, &hitDist, COL_gst_Ray.distance)
                 || OBJ_BV_IsAABBox(currGao->boundingVolume)
                 && COL_RayAABBox(source, direction, currGao, &hitDist, COL_gst_Ray.distance)))
        {
            continue;
        }

        if (COL_gst_Ray.skipRayCast && (currGao->flags2 & 0x1000000) != 0)
        {
            rayWorld = COL_gst_Ray.worldSetInColMapRayCast;
            if (rayWorld->colCrossObjectNumber == 16)
                rayWorld->colCrossObjectNumber = 15;
            rayWorld->collCrossObjects[rayWorld->colCrossObjectNumber] = currGao;
            ++COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber;
            continue;
        }

        gaoFlags = currGao->flags;
        if ((currGao->flags & 0x4000) != 0)
        {
            if ((((tGameObjectVisualObject*)(currGao->basic->actorObjectOrVisualObject))->objDrawMask & 8) != 0)
            {
                geo = OBJ_p_GetCurrentGeo(currGao);
                if (geo)
                {
                    //nullsub_32();
                    if (geo->collisionPtr)
                        COL_Visual_OK3_RayCast(currGao, geo);
                    else
                        COL_Visual_Triangles_RayCast(currGao, geo);
                    //nullsub_32();
                }
            }
        }
        else if ((gaoFlags & 2) != 0 && (gaoFlags & 0x100) != 0)
        {
            colMap = currGao->extended->coll->colMap;
            firstCob = *colMap->collisionObjectPtr;
            if (firstCob->objectType == 5)
            {
                if ((colMap->activation & 1) != 0)
                {
                    if (((tCobMeshObjectMesh*)(firstCob->objectPtrByType))->ok3)
                        COL_ColMap_OK3_RayCast(currGao, colMap);
                    else
                        COL_ColMap_Triangles_RayCast(currGao, colMap);
                    if (COL_gst_Ray.ignoreColMapRayCastResult && !*COL_gst_Ray.ignoreColMapRayCastResult)
                        return 0;
                }
            }
            else
            {
                for (i = 0; i < colMap->colObjectID; ++i)
                {
                    if (((uint8_t)(1 << i) & colMap->activation) == 0)
                        continue;
                    cob = colMap->collisionObjectPtr[i];
                    shape = (tCobNormalObjectGeneric*)(cob->objectPtrByType);
                    if (cob->objectType == 1)
                        COL_ColMap_Box_RayCast(currGao, shape);
                    else if (cob->objectType == 2)
                        COL_ColMap_Sphere_RayCast(currGao, shape);
                    else if (cob->objectType == 3)
                        COL_ColMap_Cylinder_RayCast(currGao, shape);
                }
            }
        }
    }

    COL_gb_UseMinDistance = 0;
    return COL_gst_Ray.gao;
}

void COL_RayCompute(Vec3* offsetOut, struct sWorld* world, struct sGameObjectMain* gao, Vec3* source, Vec3* direction, int flagMustHave, int flagMustNothave, int whatToTest, int skipRayCast, unsigned int* resultFlags)
{
    float roundedZ;
    float roundedY;
    float roundedX;
    float dirZ;
    float dirY;
    float dirX;
    Vec3* candidate;
    double scaledZ;
    double scaledY;
    double scaledX;
    double sourceZ;
    double sourceY;
    double sourceX;
    Vec3 xAxis;
    int blocked[51]; // TODO FIXME: 17 nem elég?
    Vec3 zAxis;
    float toGaoLen;
    tGameObjectMain* hitGao;
    float length;
    Vec3 toGao;
    int i;
    float invLength;
    Vec3 candidates[17]; // meg ennek is

    length = MATH_VecLen(direction);
    invLength = (float)(1.0 / length);
    direction->x = invLength * direction->x;
    direction->y = direction->y * invLength;
    direction->z = direction->z * invLength;

    zAxis.y = 0.0;
    zAxis.x = 0.0;
    zAxis.z = 1.0;
    xAxis.z = 0.0;
    xAxis.y = 0.0;
    xAxis.x = 1.0;

    candidates[0] = *direction;
    MATH_VecRotate(&candidates[1], &zAxis, direction, 0.78539819f);
    MATH_VecRotate(&candidates[2], &zAxis, direction, -0.78539819f);
    MATH_VecRotate(&candidates[3], &zAxis, direction, 1.5707964f);
    MATH_VecRotate(&candidates[4], &zAxis, direction, -1.5707964f);
    MATH_VecRotate(&candidates[5], &zAxis, direction, 2.3561945f);
    MATH_VecRotate(&candidates[6], &zAxis, direction, -2.3561945f);
    MATH_VecRotate(&candidates[7], &zAxis, direction, 3.1415927f);
    candidates[8].x = direction->x;
    candidates[8].y = direction->y;
    candidates[8].z = 0.0;
    MATH_VecRotate(&candidates[9], &zAxis, &candidates[8], 0.78539819f);
    MATH_VecRotate(&candidates[10], &zAxis, &candidates[8], -0.78539819f);
    MATH_VecRotate(&candidates[11], &zAxis, &candidates[8], 1.5707964f);
    MATH_VecRotate(&candidates[12], &zAxis, &candidates[8], -1.5707964f);
    MATH_VecRotate(&candidates[13], &zAxis, &candidates[8], 2.3561945f);
    MATH_VecRotate(&candidates[14], &zAxis, &candidates[8], -2.3561945f);
    MATH_VecRotate(&candidates[15], &zAxis, &candidates[8], 3.1415927f);
    candidates[16].x = direction->x;
    candidates[16].y = direction->y;
    candidates[16].z = 0.0;
    // preserved: the other fifteen angles here are multiples of pi/4 and this one is not
    MATH_VecRotate(&candidates[16], candidates, &xAxis, 1.4707963f);

    sourceX = 1.0 / 0.1 * source->x + MATH_gd_magicDoubleForRound;
    dirX = (float)(*((int32_t *)(&sourceX)));
    source->x = (float)(dirX * 0.1);
    sourceY = 1.0 / 0.1 * source->y + MATH_gd_magicDoubleForRound;
    dirY = (float)(*((int32_t*)(&sourceY)));
    source->y = (float)(dirY * 0.1);
    sourceZ = 1.0 / 0.1 * source->z + MATH_gd_magicDoubleForRound;
    dirZ = (float)(*((int32_t*)(&sourceZ)));
    source->z = (float)(dirZ * 0.1);

    for (i = 0; i <= 16; ++i)
    {
        MATH_NormalizeVector(&candidates[i], &candidates[i]);
        candidate = &candidates[i];
        *offsetOut = *candidate;
        offsetOut->x = length * offsetOut->x;
        offsetOut->y = offsetOut->y * length;
        offsetOut->z = offsetOut->z * length;

        scaledX = 1.0 / 0.1 * candidates[i].x + MATH_gd_magicDoubleForRound;
        roundedX = (float)(*((int32_t*)(&scaledX)));
        candidates[i].x = (float)(roundedX * 0.1);
        scaledY = 1.0 / 0.1 * candidates[i].y + MATH_gd_magicDoubleForRound;
        roundedY = (float)(*((int32_t*)(&scaledY)));
        candidates[i].y = (float)(roundedY * 0.1);
        scaledZ = 1.0 / 0.1 * candidates[i].z + MATH_gd_magicDoubleForRound;
        roundedZ = (float)(*((int32_t*)(&scaledZ)));
        candidates[i].z = (float)(roundedZ * 0.1);

        hitGao = COL_Visual_RayCast(world, gao, source, &candidates[i], length, flagMustHave, flagMustNothave, whatToTest, 0, skipRayCast);
        blocked[i] = hitGao == 0;
        if (hitGao)
            continue;

        toGao = *source;
        MATH_VecAddVec(&toGao, &toGao, offsetOut);
        MATH_SubstractVec(&toGao, &toGao, &gao->matrices->Pos);
        toGaoLen = MATH_VecLen(&toGao);
        MATH_NormalizeVector(&toGao, &toGao);
        hitGao = COL_Visual_RayCast(world, gao, &gao->matrices->Pos, &toGao, toGaoLen, flagMustHave, flagMustNothave, whatToTest, 0, skipRayCast);
        if (!hitGao)
        {
            *resultFlags |= 0x10u;
            *resultFlags &= ~0x20u;
            return;
        }
    }

    for (i = 0; i <= 16; ++i)
    {
        if (blocked[i])
        {
            *offsetOut = candidates[i];
            offsetOut->x = length * offsetOut->x;
            offsetOut->y = offsetOut->y * length;
            offsetOut->z = offsetOut->z * length;
            *resultFlags |= 0x10u;
            *resultFlags |= 0x20u;
            return;
        }
    }
    *resultFlags &= ~0x10u;
    *resultFlags &= ~0x20u;
}

void COL_UpdateOldMatrix(tWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* end;
    tGameObjectMain* gao;
    tColInstance* instance;
    Mtx4x4* src;

    elem = world->eot.collInstanceZDMGameObjectsPF.firstElemPtr;
    for (end = ((tableElemStructPF*)(world->eot.collInstanceZDMGameObjectsPF.nextElemPtr)) - 1; elem <= end; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (!elem->objectPtr || ((uint8_t)gao & 3) == 1 || (gao->flags & 0x200) == 0)
            continue;

        instance = gao->extended->coll->colInstance;
        instance->backface = 0;
        if ((gao->flags & 0x400000) != 0)
            src = &gao->basic->mtx->mtx;
        else
            src = gao->matrices;
        MATH_CopyMatrix(instance->mtx, src);
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

int COL_i_ZoneActivationGet(tColInstance* instance, char zoneID)
{
    uint8_t i;

    i = 0;
    while (instance->colSet->zoneIDs[i] != zoneID)
    {
        if (++i >= 16u)
            return 0;
    }
    return instance->activationFlags & (1 << i);
}

void COL_A_BCS_ComputeZDxInfo(tColGlobal* colGlobal)
{
    tColInstanceElem* elem;
    tColSphere* sphere;
    tColBox* box;
    tGameObjectMain* gao;
    Mtx4x4* instMtx;
    Vec3* centerPos;
    Vec3* prevCenterPos;
    int gaoFlags;
    double moveX;
    double moveY;
    double moveZ;
    double boxCenterZ;
    double halfX;
    double halfY;
    long double moveLen;
    long double dynRadius;
    float radius;
    float halfDiagonal;
    Vec3 worldCenter;
    Vec3 scale;
    Vec3 halfExtent;
    Vec3 boxCenter;
    Mtx4x4 combined;

    elem = colGlobal->collInstanceElem;
    if (elem->elemType == 2)
    {
        sphere = (tColSphere*)elem->elemShapePtr;
        if ((elem->colFlags & 8) != 0)
        {
            MATH_TransformVertexNoScale(&worldCenter, colGlobal->colInstGaoMatrices, &sphere->center);
            colGlobal->radius = sphere->radius;
        }
        else
        {
            MATH_TransformVertex(&worldCenter, colGlobal->colInstGaoMatrices, &sphere->center);
            if ((colGlobal->colInstGaoMatrices->mtxflag & 8) != 0)
            {
                MATH_GetScale(&scale, colGlobal->colInstGaoMatrices);
                colGlobal->radius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * sphere->radius;
            }
            else
            {
                colGlobal->radius = sphere->radius;
            }
        }

        centerPos = &colGlobal->centerPos;
        MATH_TransformVertexNoScale(&colGlobal->centerPos, &colGlobal->colMapGaoInvMatrix, &worldCenter);
        MATH_TransformVertexNoScale(&colGlobal->centerPos, &colGlobal->colMapGaoInvMatrix, &worldCenter);
        gao = colGlobal->colInstanceGao;
        gaoFlags = gao->flags;
        if ((colGlobal->collInstanceElem->colFlags & 8) != 0)
        {
            if ((gaoFlags & 0x400000) != 0)
            {
                MATH_MulMatrixMatrix(&combined, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
                MATH_TransformVertexNoScale(&worldCenter, &combined, &sphere->center);
            }
            else
            {
                MATH_TransformVertexNoScale(&worldCenter, colGlobal->colInstMatrix, &sphere->center);
            }
        }
        else if ((gaoFlags & 0x400000) != 0)
        {
            MATH_MulMatrixMatrix(&combined, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
            MATH_TransformVertex(&worldCenter, &combined, &sphere->center);
        }
        else
        {
            MATH_TransformVertex(&worldCenter, colGlobal->colInstMatrix, &sphere->center);
        }

        prevCenterPos = &colGlobal->centerPosPrevious;
        MATH_TransformVertexNoScale(&colGlobal->centerPosPrevious, &colGlobal->colMapGaoInvMatrix, &worldCenter);
        colGlobal->moveVec.x = centerPos->x - colGlobal->centerPosPrevious.x;
        colGlobal->moveVec.y = colGlobal->centerPos.y - colGlobal->centerPosPrevious.y;
        colGlobal->moveVec.z = colGlobal->centerPos.z - colGlobal->centerPosPrevious.z;
        if (fabs(colGlobal->moveVec.x) != 0 || fabs(colGlobal->moveVec.y) != 0 || fabs(colGlobal->moveVec.z) != 0)
        {
            MATH_NormalizeVector(&colGlobal->moveVecUnit, &colGlobal->moveVec);
        }
        else
        {
            colGlobal->moveVecUnit.x = 0.0;
            colGlobal->moveVecUnit.y = 0.0;
            colGlobal->moveVecUnit.z = 0.0;
        }

        colGlobal->dynCenterPos.x = centerPos->x - prevCenterPos->x;
        colGlobal->dynCenterPos.y = colGlobal->centerPos.y - colGlobal->centerPosPrevious.y;
        colGlobal->dynCenterPos.z = colGlobal->centerPos.z - colGlobal->centerPosPrevious.z;
        colGlobal->dynCenterPos.x = (float)(colGlobal->dynCenterPos.x * 0.5);
        colGlobal->dynCenterPos.y = (float)(colGlobal->dynCenterPos.y * 0.5);
        colGlobal->dynCenterPos.z = (float)(colGlobal->dynCenterPos.z * 0.5);
        colGlobal->dynCenterPos.x = colGlobal->dynCenterPos.x + prevCenterPos->x;
        colGlobal->dynCenterPos.y = colGlobal->centerPosPrevious.y + colGlobal->dynCenterPos.y;
        colGlobal->dynCenterPos.z = colGlobal->centerPosPrevious.z + colGlobal->dynCenterPos.z;

        instMtx = colGlobal->colInstGaoMatrices;
        moveY = colGlobal->dynCenterPos.y - colGlobal->centerPosPrevious.y;
        moveZ = colGlobal->dynCenterPos.z - colGlobal->centerPosPrevious.z;
        moveX = colGlobal->dynCenterPos.x - prevCenterPos->x;
        moveLen = sqrt(moveX * moveX + moveZ * moveZ + moveY * moveY);
        colGlobal->dynRadius = (float)moveLen;
        dynRadius = moveLen + sphere->radius;
        colGlobal->dynRadius = (float)dynRadius;
        if ((instMtx->mtxflag & 8) == 0 || (colGlobal->collInstanceElem->colFlags & 8) != 0)
        {
            colGlobal->dynRadius = (float)(dynRadius + sphere->radius);
        }
        else
        {
            MATH_GetScale(&scale, instMtx);
            radius = sphere->radius;
            colGlobal->dynRadius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * radius + colGlobal->dynRadius;
        }
        return;
    }

    box = (tColBox*)elem->elemShapePtr;
    boxCenter.x = box->pos1.x + box->pos2.x;
    boxCenter.y = box->pos2.y + box->pos1.y;
    boxCenterZ = box->pos2.z + box->pos1.z;
    boxCenter.x = (float)(boxCenter.x * 0.5);
    boxCenter.y = (float)(boxCenter.y * 0.5);
    boxCenter.z = (float)(boxCenterZ * 0.5);
    halfX = box->pos1.x - box->pos2.x;
    halfY = box->pos1.y - box->pos2.y;
    halfExtent.z = box->pos1.z - box->pos2.z;
    halfExtent.x = (float)(halfX * 0.5);
    halfDiagonal = (float)sqrt(halfExtent.x * halfExtent.x
        + halfExtent.z * 0.5 * (halfExtent.z * 0.5)
        + halfY * 0.5 * (halfY * 0.5));

    if ((elem->colFlags & 8) != 0)
    {
        MATH_TransformVertexNoScale(&worldCenter, colGlobal->colInstGaoMatrices, &boxCenter);
        colGlobal->radius = halfDiagonal;
    }
    else
    {
        MATH_TransformVertex(&worldCenter, colGlobal->colInstGaoMatrices, &boxCenter);
        if ((colGlobal->colInstGaoMatrices->mtxflag & 8) != 0)
        {
            MATH_GetScale(&scale, colGlobal->colInstGaoMatrices);
            colGlobal->radius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * halfDiagonal;
        }
        else
        {
            colGlobal->radius = halfDiagonal;
        }
    }

    centerPos = &colGlobal->centerPos;
    MATH_TransformVertexNoScale(&colGlobal->centerPos, &colGlobal->colMapGaoInvMatrix, &worldCenter);
    MATH_TransformVertexNoScale(&colGlobal->centerPos, &colGlobal->colMapGaoInvMatrix, &worldCenter);
    gao = colGlobal->colInstanceGao;
    gaoFlags = gao->flags;
    if ((colGlobal->collInstanceElem->colFlags & 8) != 0)
    {
        if ((gaoFlags & 0x400000) != 0)
        {
            MATH_MulMatrixMatrix(&combined, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
            MATH_TransformVertexNoScale(&worldCenter, &combined, &boxCenter);
        }
        else
        {
            MATH_TransformVertexNoScale(&worldCenter, colGlobal->colInstMatrix, &boxCenter);
        }
    }
    else if ((gaoFlags & 0x400000) != 0)
    {
        MATH_MulMatrixMatrix(&combined, gao->extended->coll->colInstance->mtx, gao->basic->mtx->refGao2->matrices);
        MATH_TransformVertex(&worldCenter, &combined, &boxCenter);
    }
    else
    {
        MATH_TransformVertex(&worldCenter, colGlobal->colInstMatrix, &boxCenter);
    }

    prevCenterPos = &colGlobal->centerPosPrevious;
    MATH_TransformVertexNoScale(&colGlobal->centerPosPrevious, &colGlobal->colMapGaoInvMatrix, &worldCenter);
    colGlobal->moveVec.x = centerPos->x - colGlobal->centerPosPrevious.x;
    colGlobal->moveVec.y = colGlobal->centerPos.y - colGlobal->centerPosPrevious.y;
    colGlobal->moveVec.z = colGlobal->centerPos.z - colGlobal->centerPosPrevious.z;
    if (fabs(colGlobal->moveVec.x) != 0 || fabs(colGlobal->moveVec.y) != 0 || fabs(colGlobal->moveVec.z) != 0)
    {
        MATH_NormalizeVector(&colGlobal->moveVecUnit, &colGlobal->moveVec);
    }
    else
    {
        colGlobal->moveVecUnit.x = 0.0;
        colGlobal->moveVecUnit.y = 0.0;
        colGlobal->moveVecUnit.z = 0.0;
    }

    colGlobal->dynCenterPos.x = centerPos->x - prevCenterPos->x;
    colGlobal->dynCenterPos.y = colGlobal->centerPos.y - colGlobal->centerPosPrevious.y;
    colGlobal->dynCenterPos.z = colGlobal->centerPos.z - colGlobal->centerPosPrevious.z;
    colGlobal->dynCenterPos.x = (float)(colGlobal->dynCenterPos.x * 0.5);
    colGlobal->dynCenterPos.y = (float)(colGlobal->dynCenterPos.y * 0.5);
    colGlobal->dynCenterPos.z = (float)(colGlobal->dynCenterPos.z * 0.5);
    colGlobal->dynCenterPos.x = prevCenterPos->x + colGlobal->dynCenterPos.x;
    colGlobal->dynCenterPos.y = colGlobal->centerPosPrevious.y + colGlobal->dynCenterPos.y;
    colGlobal->dynCenterPos.z = colGlobal->centerPosPrevious.z + colGlobal->dynCenterPos.z;

    instMtx = colGlobal->colInstGaoMatrices;
    moveY = colGlobal->dynCenterPos.y - colGlobal->centerPosPrevious.y;
    moveZ = colGlobal->dynCenterPos.z - colGlobal->centerPosPrevious.z;
    moveX = colGlobal->dynCenterPos.x - prevCenterPos->x;
    dynRadius = sqrt(moveZ * moveZ + moveY * moveY + moveX * moveX) + halfDiagonal;
    colGlobal->dynRadius = (float)dynRadius;
    if ((instMtx->mtxflag & 8) == 0 || (colGlobal->collInstanceElem->colFlags & 8) != 0)
    {
        colGlobal->dynRadius = (float)(dynRadius + halfDiagonal);
    }
    else
    {
        MATH_GetScale(&scale, instMtx);
        colGlobal->dynRadius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * halfDiagonal + colGlobal->dynRadius;
    }
}

void COL_SetFaceTag(tCobOK3* ok3, unsigned int face)
{
    int* tags;

    if (!ok3)
        return;
    tags = ok3->faceTagInts;
    if (tags)
        tags[face >> 5] |= 1 << (face - 32 * (face >> 5));
}

// the whole word is returned, not a byte: the tested bit is 1 << (face % 32)
int COL_b_TestFaceTag(tCobOK3* ok3, unsigned int face)
{
    int* tags;

    if (!ok3)
        return 0;
    tags = ok3->faceTagInts;
    if (!tags)
        return 0;
    return tags[face >> 5] & (1 << (face - 32 * (face >> 5)));
}

Vec3* INT_PointLineClosestPoint(Vec3* point, Vec3* lineStart, Vec3* lineEnd, Vec3* startToPoint, Vec3* closestOut)
{
    double deltaZ;
    double deltaZSquared;
    double deltaYSquared;
    double deltaXSquared;
    long double invLength;
    double offsetX;
    double offsetY;
    float deltaX;
    float deltaY;
    float deltaZf;
    float unitX;
    float unitY;
    float ratio;
    float offsetZ;

    deltaX = lineEnd->x - lineStart->x;
    deltaY = lineEnd->y - lineStart->y;
    deltaZ = lineEnd->z - lineStart->z;
    deltaZf = (float)deltaZ;
    deltaZSquared = deltaZ * deltaZf;
    deltaYSquared = deltaY * deltaY;
    deltaXSquared = deltaX * deltaX;
    invLength = 1.0 / sqrt(deltaYSquared + deltaZSquared + deltaXSquared);
    unitX = (float)(deltaX * invLength);
    unitY = (float)(deltaY * invLength);
    ratio = (float)(-((startToPoint->z * (invLength * deltaZf) + startToPoint->y * unitY + unitX * startToPoint->x)
        / sqrt(deltaZSquared + deltaYSquared + deltaXSquared)));

    if (ratio >= 0.0)
    {
        if (ratio <= 1.0)
        {
            offsetX = deltaX * ratio;
            closestOut->x = (float)offsetX;
            offsetY = deltaY * ratio;
            closestOut->y = (float)offsetY;
            offsetZ = deltaZf * ratio;
            closestOut->z = offsetZ;
            closestOut->x = (float)(offsetX + lineStart->x);
            closestOut->y = (float)(offsetY + lineStart->y);
            closestOut->z = (float)(offsetZ + lineStart->z);
        }
        else
        {
            *closestOut = *lineEnd;
        }
    }
    else
    {
        *closestOut = *lineStart;
    }
    return closestOut;
}

void INT_PointTriangleClosestPointGet(Vec3* p1, Vec3* p2, Vec3* p3, Vec3* normal, Vec3* point, Vec3* closestOut, uint32_t* edgeOut)
{
    double toP3X;
    double toP3Y;
    double toP3Z;
    double farthest;
    float distToP1Squared;
    float distToP2Squared;
    float distToP3Squared;
    Vec3 toP1;
    Vec3 toP2;

    if (INT_PointInTriangle(point, p1, p2, p3, normal, 0))
    {
        *closestOut = *point;
        *edgeOut = 1;
        return;
    }

    toP1.x = p1->x - point->x;
    toP1.y = p1->y - point->y;
    toP1.z = p1->z - point->z;
    toP2.x = p2->x - point->x;
    toP2.y = p2->y - point->y;
    toP2.z = p2->z - point->z;
    toP3X = p3->x - point->x;
    toP3Y = p3->y - point->y;
    toP3Z = p3->z - point->z;
    distToP1Squared = toP1.z * toP1.z + toP1.x * toP1.x + toP1.y * toP1.y;
    distToP2Squared = toP2.z * toP2.z + toP2.y * toP2.y + toP2.x * toP2.x;
    distToP3Squared = (float)(toP3Z * toP3Z + toP3Y * toP3Y + toP3X * toP3X);
    farthest = MATH_f_MaxOf3(distToP1Squared, distToP2Squared, distToP3Squared);

    if (farthest == distToP1Squared)
    {
        INT_PointLineClosestPoint(point, p2, p3, &toP2, closestOut);
        *edgeOut = 8;
    }
    else if (farthest == distToP2Squared)
    {
        INT_PointLineClosestPoint(point, p1, p3, &toP1, closestOut);
        *edgeOut = 4;
    }
    else if (farthest == distToP3Squared)
    {
        INT_PointLineClosestPoint(point, p1, p2, &toP1, closestOut);
        *edgeOut = 2;
    }
}

static BOOL INT_l_b_ContactWithinRadii(Vec3* center1, float radius1, Vec3* center2, float radius2, Vec3* moveDir, Vec3* closest)
{
    double toClosestX;
    double toClosestY;
    double toClosestZ;
    double along;
    double slidX;
    double slidY;
    double toCenter2X;
    double toCenter2Y;
    double toCenter2Z;
    float slidYf;
    float slidZf;
    float slidZ;
    float toCenter2Xf;

    toClosestY = closest->y - center1->y;
    toClosestZ = closest->z - center1->z;
    toClosestX = closest->x - center1->x;
    if (sqrt(toClosestZ * toClosestZ + toClosestY * toClosestY + toClosestX * toClosestX) > radius1)
        return 0;

    along = (center2->y - closest->y) * moveDir->y
        + (center2->z - closest->z) * moveDir->z
        + (center2->x - closest->x) * moveDir->x;
    slidX = along * moveDir->x;
    slidYf = (float)(along * moveDir->y);
    slidZf = (float)(along * moveDir->z);
    toCenter2X = slidX + closest->x;
    slidY = slidYf + closest->y;
    slidZ = slidZf + closest->z;
    toCenter2Y = slidY;
    toCenter2Xf = (float)(toCenter2X - center2->x);
    toCenter2Z = slidZ - center2->z;
    toCenter2Y = toCenter2Y - center2->y;
    return sqrt(toCenter2Z * toCenter2Z + toCenter2Y * toCenter2Y + toCenter2Xf * toCenter2Xf) <= radius2;
}

BOOL INT_DynamicHit(Vec3* sweptCenter, float sweptRadius, Vec3* center, float radius, Vec3* moveDir, Vec3* p1, Vec3* p2, Vec3* p3, Vec3* normal, Vec3* closestOut, int* edgeOut)
{
    double negRadius;
    double backStepX;
    double startY;
    long double sweepDotNormal;
    long double planeRatio;
    double normalDist;
    double sweepOffsetX;
    double sweepOffsetY;
    BOOL sweepFacesTriangle;
    Vec3 sweep;
    float startX;
    float startZ;
    float forwardX;
    float forwardY;
    float forwardZ;

    negRadius = -sweptRadius;
    backStepX = negRadius * moveDir->x;
    sweep.y = (float)(negRadius * moveDir->y);
    sweepFacesTriangle = 1;
    sweep.z = (float)(negRadius * moveDir->z);
    forwardX = sweptRadius * moveDir->x;
    forwardY = sweptRadius * moveDir->y;
    forwardZ = sweptRadius * moveDir->z;
    startX = (float)(backStepX + sweptCenter->x);
    startY = sweep.y + sweptCenter->y;
    startZ = sweep.z + sweptCenter->z;
    sweepOffsetX = sweptCenter->x - startX;
    sweepOffsetY = sweptCenter->y - startY;
    sweep.z = sweptCenter->z - startZ;
    sweep.x = (float)(sweepOffsetX + forwardX);
    sweep.y = (float)(sweepOffsetY + forwardY);
    sweep.z = (float)(sweep.z + forwardZ);

    sweepDotNormal = sweep.y * normal->y + sweep.x * normal->x + sweep.z * normal->z;
    if (sweepDotNormal > 0.0 || fabs(sweepDotNormal) < 0.0000099999997)
        sweepFacesTriangle = 0;

    planeRatio = ((p1->y - startY) * normal->y + (p1->x - startX) * normal->x + (p1->z - startZ) * normal->z)
        / sweepDotNormal;
    if (planeRatio > 0.0)
    {
        sweep.z = (float)(sweep.z * planeRatio);
        sweep.x = (float)(sweep.x * planeRatio + startX);
        sweep.y = (float)(sweep.y * planeRatio + startY);
        sweep.z = (float)(sweep.z + startZ);
        INT_PointTriangleClosestPointGet(p1, p2, p3, normal, &sweep, closestOut, edgeOut);
        if (INT_l_b_ContactWithinRadii(sweptCenter, sweptRadius, center, radius, moveDir, closestOut)
            && sweepFacesTriangle)
            return 1;
    }

    normalDist = (p1->x - sweptCenter->x) * normal->x
        + (p1->z - sweptCenter->z) * normal->z
        + (p1->y - sweptCenter->y) * normal->y;
    sweep.x = (float)(normalDist * normal->x + sweptCenter->x);
    sweep.y = (float)(normalDist * normal->y) + sweptCenter->y;
    sweep.z = (float)(normalDist * normal->z) + sweptCenter->z;
    INT_PointTriangleClosestPointGet(p1, p2, p3, normal, &sweep, closestOut, edgeOut);
    return INT_l_b_ContactWithinRadii(sweptCenter, sweptRadius, center, radius, moveDir, closestOut);
}

int INT_ul_GetPositionPointWithBox(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    int code;

    code = 0;
    if (point->x >= (double)boxMin->x)
    {
        if (point->x > (double)boxMax->x)
            code = 2;
    }
    else
    {
        code = 1;
    }
    if (point->y >= (double)boxMin->y)
    {
        if (point->y > (double)boxMax->y)
            code |= 8u;
    }
    else
    {
        code |= 4u;
    }
    if (point->z < (double)boxMin->z)
        return code | 0x10;
    if (point->z > (double)boxMax->z)
        code |= 0x20u;
    return code;
}

int INT_ul_GetPositionPointWithEdgeBox(Vec3* point, Vec3* boxMin, Vec3* boxMax, Vec3* extent, Vec3* negExtent)
{
    double pointNegX;
    double maxExtX;
    double pointExtX;
    int code;
    float pointNegY;
    float pointNegZ;
    float maxExtY;
    float maxExtZ;
    float pointExtY;
    float pointExtZ;
    float minNegX;
    float minNegY;
    float minNegZ;

    pointNegX = point->x * negExtent->x;
    code = 0;
    pointNegY = negExtent->y * point->y;
    pointNegZ = negExtent->z * point->z;
    maxExtX = boxMax->x * extent->x;
    maxExtY = boxMax->y * extent->y;
    maxExtZ = boxMax->z * extent->z;
    pointExtX = point->x * extent->x;
    pointExtY = point->y * extent->y;
    pointExtZ = point->z * extent->z;
    minNegX = boxMin->x * negExtent->x;
    minNegY = boxMin->y * negExtent->y;
    minNegZ = boxMin->z * negExtent->z;

    if (maxExtY + maxExtX < pointExtY + pointExtX)
        code = 1;
    if (minNegY + maxExtX < pointNegY + pointExtX)
        code |= 2u;
    if (minNegX + maxExtY < pointExtY + pointNegX)
        code |= 4u;
    if (minNegY + minNegX < pointNegY + pointNegX)
        code |= 8u;
    if (maxExtZ + maxExtX < pointExtZ + pointExtX)
        code |= 0x10u;
    if (minNegZ + maxExtX < pointNegZ + pointExtX)
        code |= 0x20u;
    if (minNegX + maxExtZ < pointExtZ + pointNegX)
        code |= 0x40u;
    if (minNegZ + minNegX < pointNegZ + pointNegX)
        code |= 0x80u;
    if (maxExtZ + maxExtY < pointExtZ + pointExtY)
        code |= 0x100u;
    if (minNegZ + maxExtY < pointExtY + pointNegZ)
        code |= 0x200u;
    if (minNegY + maxExtZ < pointExtZ + pointNegY)
        code |= 0x400u;
    if (minNegZ + minNegY < pointNegZ + pointNegY)
        code |= 0x800u;
    return code;
}

BOOL INT_b_InYZRange(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    return point->y >= (double)boxMin->y && point->y <= (double)boxMax->y
        && point->z >= (double)boxMin->z && point->z <= (double)boxMax->z;
}

BOOL INT_b_InXZRange(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    return point->x >= (double)boxMin->x && point->x <= (double)boxMax->x
        && point->z >= (double)boxMin->z && point->z <= (double)boxMax->z;
}

BOOL INT_b_InXYRange(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    return point->x >= (double)boxMin->x && point->x <= (double)boxMax->x
        && point->y >= (double)boxMin->y && point->y <= (double)boxMax->y;
}

int INT_b_IntersectEdgeWithBoxWithHit(Vec3* edgeStart, Vec3* edgeEnd, Vec3* edgeDir, Vec3* boxMin, Vec3* boxMax, char planeMask, Vec3* hitOut)
{
    double ratio;

    if ((planeMask & 1) != 0)
    {
        ratio = (boxMin->x - edgeStart->x) / edgeDir->x;
        hitOut->x = boxMin->x;
        hitOut->y = (float)(ratio * edgeDir->y + edgeStart->y);
        hitOut->z = (float)(ratio * edgeDir->z + edgeStart->z);
        if (INT_b_InYZRange(hitOut, boxMin, boxMax))
            return 1;
    }
    if ((planeMask & 2) != 0)
    {
        ratio = (boxMax->x - edgeStart->x) / edgeDir->x;
        hitOut->x = boxMax->x;
        hitOut->y = (float)(ratio * edgeDir->y + edgeStart->y);
        hitOut->z = (float)(ratio * edgeDir->z + edgeStart->z);
        if (INT_b_InYZRange(hitOut, boxMin, boxMax))
            return 1;
    }
    if ((planeMask & 4) != 0)
    {
        ratio = (boxMin->y - edgeStart->y) / edgeDir->y;
        hitOut->x = (float)(ratio * edgeDir->x + edgeStart->x);
        hitOut->y = boxMin->y;
        hitOut->z = (float)(ratio * edgeDir->z + edgeStart->z);
        if (INT_b_InXZRange(hitOut, boxMin, boxMax))
            return 1;
    }
    if ((planeMask & 8) != 0)
    {
        ratio = (boxMax->y - edgeStart->y) / edgeDir->y;
        hitOut->x = (float)(ratio * edgeDir->x + edgeStart->x);
        hitOut->y = boxMax->y;
        hitOut->z = (float)(ratio * edgeDir->z + edgeStart->z);
        if (INT_b_InXZRange(hitOut, boxMin, boxMax))
            return 1;
    }
    if ((planeMask & 0x10) != 0)
    {
        ratio = (boxMin->z - edgeStart->z) / edgeDir->z;
        hitOut->x = (float)(ratio * edgeDir->x + edgeStart->x);
        hitOut->y = (float)(ratio * edgeDir->y + edgeStart->y);
        hitOut->z = boxMin->z;
        if (INT_b_InXYRange(hitOut, boxMin, boxMax))
            return 1;
    }
    if ((planeMask & 0x20) != 0)
    {
        ratio = (boxMax->z - edgeStart->z) / edgeDir->z;
        hitOut->x = (float)(ratio * edgeDir->x + edgeStart->x);
        hitOut->y = (float)(ratio * edgeDir->y + edgeStart->y);
        hitOut->z = boxMax->z;
        if (INT_b_InXYRange(hitOut, boxMin, boxMax))
            return 1;
    }
    return 0;
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
            && COL_i_ZoneActivationGet(coll->colInstance, (char)idx))
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
                && COL_i_ZoneActivationGet(colInstance, (char)idx))
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

void REC_MainCall(struct sWorld* world, int firstRecord)
{
    tRecordElem* end;
    tRecordElem* rec;
    tRecordElem* other;
    int flags;
    int otherFlags;
    tGameObjectMain* instGao;
    tGameObjectMain* mapGao;
    tGameObjectBasic* basic;
    tDyn* dyn;
    tGameObjectBasic* mapBasic;
    tDyn* mapDyn;
    Vec3* gravity;
    double upX;
    double upZ;
    long double invLen;
    Vec3 up;
    Vec3 normal;
    BOOL alreadyPaired;

    REC_OrderReports(world);
    end = &world->records[world->recordCount];
    for (rec = &world->records[firstRecord]; rec < end; ++rec)
    {
        flags = rec->flags;
        if ((flags & 0x80043800) != 0)
            continue;
        instGao = rec->collisionInstanceGameObject;
        mapGao = rec->collisionMapGameObject;
        if ((flags & 0x4000) != 0)
        {
            REC_PhysicalCall(instGao, world);
            continue;
        }
        alreadyPaired = FALSE;
        for (other = rec + 1; other < end; ++other)
        {
            otherFlags = other->flags;
            if ((otherFlags & 0x43800) == 0
                && ((flags & 0x80u) != 0 && (otherFlags & 0x80u) != 0
                    || (flags & 0x40) != 0 && (otherFlags & 0x40) != 0)
                && (other->collisionInstanceGameObject == instGao && other->collisionMapGameObject == mapGao
                    || other->collisionInstanceGameObject == mapGao && other->collisionMapGameObject == instGao))
            {
                alreadyPaired = TRUE;
                break;
            }
        }
        if (alreadyPaired)
            continue;
        basic = instGao->basic;
        if (!basic)
            continue;
        dyn = basic->dyn;
        if (!dyn)
            continue;
        if (mapGao
            && (mapBasic = mapGao->basic) != 0
            && (mapDyn = mapBasic->dyn) != 0
            && (mapDyn->flags & 0x2000000) != 0
            && (dyn->flags & 0x2000000) != 0
            && (mapGao->flags & 0x400000) == 0
            && (instGao->flags & 0x400000) == 0)
        {
            normal = rec->normalVec;
            REC_ApplyDyn(instGao, mapGao, &normal);
        }
        else if ((flags & 0x40) != 0 && (dyn->flags & 0x40000000) != 0)
        {
            gravity = &dyn->forces->gravityVector;
            if (gravity
                && (fabs(gravity->x) != 0 || fabs(gravity->y) != 0 || fabs(gravity->z) != 0))
            {
                up.x = -gravity->x;
                upX = up.x;
                up.z = -gravity->z;
                upZ = up.z;
                up.y = -gravity->y;
            }
            else
            {
                upX = 0.0;
                up.y = 0.0;
                upZ = 1.0;
            }
            invLen = 1.0 / sqrt(upZ * upZ + up.y * up.y + upX * upX);
            up.x = (float)(invLen * upX);
            up.y = (float)(up.y * invLen);
            up.z = (float)(upZ * invLen);
            REC_Apply(instGao, mapGao, &up, flags & 0x80);
        }
        else
        {
            REC_Apply(instGao, mapGao, &rec->normalVec, rec->flags & 0x80);
        }
    }
}

float COL_f_SqrLen(Vec3* vec)
{
    return vec->z * vec->z + vec->y * vec->y + vec->x * vec->x;
}

BOOL COL_b_SingleBVIn(struct sGameObjectMain* outerGao, struct sGameObjectMain* innerGao)
{
    tGameObjectBV* outerBV;
    tGameObjectBV* innerBV;
    Mtx4x4* innerMtx;
    Mtx4x4* outerMtx;
    Mtx4x4* boxMtx;
    double outerRadius;
    double halfX;
    double halfY;
    double halfZ;
    float centreY;
    float centreZ;
    float corner;
    float reach;
    Vec3 boxMin;
    Vec3 boxMax;
    Vec3 half;
    float deltaX;
    float deltaY;
    float deltaZ;
    float radius;

    if (outerGao && innerGao)
    {
        outerBV = outerGao->boundingVolume;
        innerBV = innerGao->boundingVolume;
        if (OBJ_BV_IsAABBox(outerBV))
        {
            innerMtx = innerGao->matrices;
            outerMtx = outerGao->matrices;
            deltaX = innerMtx->Pos.x - outerMtx->Pos.x;
            deltaY = innerMtx->Pos.y - outerMtx->Pos.y;
            deltaZ = innerMtx->Pos.z - outerMtx->Pos.z;
            // preserved: re-tests the outer volume the enclosing branch already proved is a box, so the sphere-in-box arm below is unreachable
            if (OBJ_BV_IsAABBox(outerBV))
            {
                boxMin.x = deltaX + innerBV->BVMin.x;
                boxMin.y = innerBV->BVMin.y + deltaY;
                boxMin.z = innerBV->BVMin.z + deltaZ;
                boxMax.x = deltaX + innerBV->BVMax.x;
                boxMax.y = innerBV->BVMax.y + deltaY;
                boxMax.z = innerBV->BVMax.z + deltaZ;
                return boxMin.x >= outerBV->BVMin.x
                    && outerBV->BVMax.x >= boxMax.x
                    && outerBV->BVMin.y <= boxMin.y
                    && outerBV->BVMax.y >= boxMax.y
                    && outerBV->BVMin.z <= boxMin.z
                    && outerBV->BVMax.z >= boxMax.z;
            }
            if (OBJ_BV_IsSphere(innerBV))
            {
                centreY = innerBV->BVMin.y;
                centreZ = innerBV->BVMin.z;
                radius = innerBV->BVMax.x;
                boxMin.x = innerBV->BVMin.x;
                boxMax.x = boxMin.x + deltaX + radius;
                boxMax.y = centreY + deltaY + radius;
                boxMax.z = centreZ + deltaZ + radius;
                reach = radius - deltaY;
                boxMin.x = boxMin.x - reach;
                boxMin.y = centreY - reach;
                boxMin.z = centreZ - (radius - deltaZ);
                return boxMin.x >= outerBV->BVMin.x
                    && outerBV->BVMax.x >= boxMax.x
                    && outerBV->BVMin.y <= boxMin.y
                    && outerBV->BVMax.y >= boxMax.y
                    && outerBV->BVMin.z <= boxMin.z
                    && outerBV->BVMax.z >= boxMax.z;
            }
        }
        else if (OBJ_BV_IsSphere(outerBV))
        {
            // preserved: inside the sphere branch this can never hold, so the box-in-sphere arm and COL_f_SqrLen's only call site are unreachable
            if (OBJ_BV_IsAABBox(outerBV))
            {
                MATH_SubstractVec(&half, &outerBV->BVMax, &outerBV->BVMin);
                boxMtx = innerGao->matrices;
                half.x = (float)(half.x * 0.5);
                half.y = (float)(half.y * 0.5);
                half.z = (float)(half.z * 0.5);
                MATH_VecAddVec(&boxMin, &boxMtx->Pos, &half);
                halfX = half.x;
                if (boxMin.x < 0.0)
                    halfX = -halfX;
                boxMin.x = (float)(halfX + boxMin.x);
                halfY = half.y;
                if (boxMin.y < 0.0)
                    halfY = -halfY;
                boxMin.y = (float)(halfY + boxMin.y);
                halfZ = half.z;
                if (boxMin.z < 0.0)
                    halfZ = -halfZ;
                corner = outerBV->BVMax.x;
                boxMin.z = (float)(halfZ + boxMin.z);
                if (corner * corner > COL_f_SqrLen(&boxMin))
                    return 1;
            }
            else if (OBJ_BV_IsSphere(innerBV)
                && outerBV->BVMax.x >= innerBV->BVMax.x
                && MATH_DistanceOfTwoPoints(&outerBV->BVMin, &innerBV->BVMin) + innerBV->BVMax.x < outerBV->BVMax.x)
            {
                return 1;
            }
        }
    }
    return 0;
}

BOOL COL_b_BVIn(struct sGameObjectMain* outerGao, struct sGameObjectMain* innerGao)
{
    if (outerGao && innerGao)
        return COL_b_SingleBVIn(outerGao, innerGao);
    return 0;
}

BOOL INT_LowLevel_OBBoxOBBox(float aHalfX, float aHalfY, float aHalfZ, float bHalfX, float bHalfY, float bHalfZ, Mtx4x4* rel)
{
    long double absXz;
    BOOL result;
    float absXx;
    float absYz;
    float absYy;
    float absYx;
    float absZz;
    float absZy;
    float absZx;
    float absXy;

    absYx = (float)fabs(rel->v2.x);
    absZx = (float)fabs(rel->v3.x);
    absXx = (float)fabs(rel->v1.x);
    result = 0;
    if (absXx * bHalfX + absZx * bHalfZ + absYx * bHalfY + aHalfX >= fabs(rel->Pos.x))
    {
        absXy = (float)fabs(rel->v1.y);
        absYy = (float)fabs(rel->v2.y);
        absZy = (float)fabs(rel->v3.y);
        if (absZy * bHalfZ + absYy * bHalfY + absXy * bHalfX + aHalfY >= fabs(rel->Pos.y))
        {
            absXz = (float)fabs(rel->v1.z);
            absYz = (float)fabs(rel->v2.z);
            absZz = (float)fabs(rel->v3.z);
            if (absZz * bHalfZ + absYz * bHalfY + absXz * bHalfX + aHalfZ >= fabs(rel->Pos.z)
                && absXz * aHalfZ + absXy * aHalfY + absXx * aHalfX + bHalfX >= fabs(rel->Pos.z * rel->v1.z + rel->v1.y * rel->Pos.y + rel->v1.x * rel->Pos.x)
                && absYz * aHalfZ + absYy * aHalfY + absYx * aHalfX + bHalfY >= fabs(rel->Pos.y * rel->v2.y + rel->Pos.z * rel->v2.z + rel->v2.x * rel->Pos.x)
                && absZz * aHalfZ + absZy * aHalfY + absZx * aHalfX + bHalfZ >= fabs(rel->Pos.y * rel->v3.y + rel->Pos.z * rel->v3.z + rel->v3.x * rel->Pos.x))
            {
                result = 1;
            }
        }
    }
    return result;
}

BOOL INT_HighLevel_OBBoxOBBox(Vec3* aMin, Vec3* aMax, Vec3* bMin, Vec3* bMax, Mtx4x4* rel, Vec3* aScale)
{
    double extentZ;
    float scaledBX;
    float scaledBY;
    float scaledBZ;
    float aHalfY;
    float aHalfX;
    float bHalfX;
    Vec3 scale;
    float bHalfY;
    float bHalfZ;
    float aHalfZ;

    if (aScale)
    {
        aHalfX = (float)((aMax->x - aMin->x) * aScale->x * 0.5);
        aHalfY = (float)((aMax->y - aMin->y) * aScale->y * 0.5);
        extentZ = (aMax->z - aMin->z) * aScale->z;
    }
    else
    {
        aHalfX = (float)((aMax->x - aMin->x) * 0.5);
        aHalfY = (float)((aMax->y - aMin->y) * 0.5);
        extentZ = aMax->z - aMin->z;
    }
    aHalfZ = (float)(extentZ * 0.5);
    bHalfX = (float)((bMax->x - bMin->x) * 0.5);
    bHalfY = (float)((bMax->y - bMin->y) * 0.5);
    bHalfZ = (float)((bMax->z - bMin->z) * 0.5);
    if ((rel->mtxflag & 8) == 0)
        return INT_LowLevel_OBBoxOBBox(aHalfX, aHalfY, aHalfZ, bHalfX, bHalfY, bHalfZ, rel);
    MATH_GetScale(&scale, rel);
    scaledBZ = scale.z * bHalfZ;
    scaledBY = scale.y * bHalfY;
    scaledBX = scale.x * bHalfX;
    return INT_LowLevel_OBBoxOBBox(aHalfX, aHalfY, aHalfZ, scaledBX, scaledBY, scaledBZ, rel);
}

BOOL COL_b_SingleBVOverlap(tGameObjectMain* gao1, tGameObjectMain* gao2)
{
    tGameObjectBV* bv1;
    tGameObjectBV* bv2;
    Mtx4x4* mtx1;
    Mtx4x4* mtx2;
    Vec3* pos;
    Vec3 min1;
    Vec3 max1;
    Vec3 min2;
    Vec3 max2;

    if (gao1 && gao2)
    {
        bv1 = gao1->boundingVolume;
        bv2 = gao2->boundingVolume;
        if (OBJ_BV_IsAABBox(bv1) && OBJ_BV_IsAABBox(bv2))
        {
            mtx1 = gao1->matrices;
            min1.x = bv1->BVMin.x + mtx1->Pos.x;
            min1.y = mtx1->Pos.y + bv1->BVMin.y;
            min1.z = mtx1->Pos.z + bv1->BVMin.z;
            max1.x = mtx1->Pos.x + bv1->BVMax.x;
            max1.y = mtx1->Pos.y + bv1->BVMax.y;
            max1.z = mtx1->Pos.z + bv1->BVMax.z;
            mtx2 = gao2->matrices;
            min2.x = bv2->BVMin.x + mtx2->Pos.x;
            min2.y = mtx2->Pos.y + bv2->BVMin.y;
            min2.z = mtx2->Pos.z + bv2->BVMin.z;
            max2.x = mtx2->Pos.x + bv2->BVMax.x;
            max2.y = mtx2->Pos.y + bv2->BVMax.y;
            max2.z = mtx2->Pos.z + bv2->BVMax.z;
            if (min1.x <= max2.x
                && max1.x >= min2.x
                && min1.y <= max2.y
                && max1.y >= min2.y
                && min1.z <= max2.z
                && max1.z >= min2.z)
            {
                return 1;
            }
        }
        else
        {
            if (OBJ_BV_IsAABBox(bv1) && OBJ_BV_IsSphere(bv2))
            {
                mtx1 = gao1->matrices;
                max1.x = mtx1->Pos.x + bv1->BVMax.x;
                max1.y = bv1->BVMax.y + mtx1->Pos.y;
                max1.z = bv1->BVMax.z + mtx1->Pos.z;
                min1.x = bv1->BVMin.x + mtx1->Pos.x;
                min1.y = mtx1->Pos.y + bv1->BVMin.y;
                min1.z = mtx1->Pos.z + bv1->BVMin.z;
                return INT_SphereAABBox(&bv2->BVMin, bv2->BVMax.x, &min1, &max1);
            }
            if (OBJ_BV_IsSphere(bv1) && OBJ_BV_IsAABBox(bv2))
            {
                pos = &gao2->matrices->Pos;
                MATH_VecAddVec(&max1, &bv2->BVMax, pos);
                MATH_VecAddVec(&min1, &bv2->BVMin, pos);
                return INT_SphereAABBox(&bv1->BVMin, bv1->BVMax.x, &min1, &max1);
            }
            if (OBJ_BV_IsSphere(bv1) && OBJ_BV_IsSphere(bv2))
                return INT_SphereSphere(&bv1->BVMin, bv1->BVMax.x, &bv2->BVMin, bv2->BVMax.x);
        }
    }
    return 0;
}

BOOL COL_b_BVOverlap(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, BOOL OBBoxOfFirstGao, BOOL OBBoxOfSecondGao, BOOL useSnP)
{
    tGameObjectMain* first;
    tGameObjectMain* second;
    BOOL snp;
    int flags1;
    int flags2;
    BOOL swapped;
    tSnp* snpTables;
    tGameObjectBV* firstBV;
    tGameObjectBV* secondBV;
    Vec3* pos;
    Mtx4x4* mtx;
    Vec3* firstMin;
    Vec3* firstMax;
    Vec3* secondMin;
    Vec3* secondMax;
    Vec3 centreB;
    Vec3 centreA;
    Vec3 worldA;
    Vec3 delta;
    Vec3 worldB;
    Vec3 relDelta;
    Vec3 unitScale;
    Mtx4x4 secondMtx;
    Vec3 secondWorldMax;
    Vec3 firstWorldMin;
    Vec3 firstWorldMax;
    Vec3 secondWorldMin;
    Mtx4x4 rel;
    Mtx4x4 noScale;
    Mtx4x4 invFirst;
    int snpIndex;
    int snpSpan;

    first = gao1;
    if (!gao1)
        return FALSE;
    second = gao2;
    if (!gao2)
        return FALSE;
    snp = useSnP;
    if (useSnP)
    {
        flags1 = gao1->flags2;
        if ((flags1 & 0x80u) == 0
            || (flags2 = gao2->flags2, (flags2 & 0x80u) == 0)
            || (flags1 & 0x20) == 0 && (flags2 & 0x20) == 0)
        {
            snp = FALSE;
        }
    }
    if ((gao1->flags & 0x80000) == 0)
        OBBoxOfFirstGao = FALSE;
    if ((gao2->flags & 0x80000) == 0)
        OBBoxOfSecondGao = FALSE;
    if ((gao1->flags2 & 0x20) == 0)
    {
        swapped = OBBoxOfFirstGao;
        OBBoxOfFirstGao = OBBoxOfSecondGao;
        first = gao2;
        OBBoxOfSecondGao = swapped;
        gao2 = gao1;
        second = gao1;
    }
    if (snp)
    {
        snpTables = WOR_gpst_CurrentWorld->snp;
        if (!INT_SnP_FullGetFlag(snpTables->axisPtr[0]->flagsArray, first->snpIndex, second->snpIndex, 1024, &snpIndex, &snpSpan)
            || !INT_SnP_GetFlag(snpTables->axisPtr[1]->flagsArray, snpIndex, snpSpan)
            || !INT_SnP_GetFlag(snpTables->axisPtr[2]->flagsArray, snpIndex, snpSpan))
        {
            return FALSE;
        }
    }
    else if (!COL_b_SingleBVOverlap(first, second))
    {
        return FALSE;
    }
    if (!OBBoxOfFirstGao && !OBBoxOfSecondGao)
        return TRUE;
    if (OBBoxOfFirstGao)
    {
        if ((OBJ_pst_GetAbsoluteMatrix(first)->mtxflag & 8) != 0)
        {
            MATH_CopyMatrix(&noScale, OBJ_pst_GetAbsoluteMatrix(first));
            noScale._m14 = 0.0;
            noScale._m24 = 0.0;
            noScale._m34 = 0.0;
            noScale.mtxflag &= ~8u;
            MATH_InvertMatrix(&invFirst, &noScale);
        }
        else
        {
            MATH_InvertMatrix(&invFirst, OBJ_pst_GetAbsoluteMatrix(first));
        }
        firstBV = first->boundingVolume;
        firstMax = &firstBV->BVMaxLocal;
        firstMin = &firstBV->BVMinLocal;
        if (OBBoxOfSecondGao)
        {
            if ((OBJ_pst_GetAbsoluteMatrix(gao2)->mtxflag & 8) != 0)
            {
                MATH_CopyMatrix(&secondMtx, OBJ_pst_GetAbsoluteMatrix(gao2));
                secondMtx._m14 = 0.0;
                secondMtx._m24 = 0.0;
                secondMtx._m34 = 0.0;
                secondMtx.mtxflag &= ~8u;
            }
            else
            {
                MATH_CopyMatrix(&secondMtx, OBJ_pst_GetAbsoluteMatrix(gao2));
            }
            MATH_MulMatrixMatrix(&rel, &secondMtx, &invFirst);
            secondBV = gao2->boundingVolume;
            secondMax = &secondBV->BVMaxLocal;
            secondMin = &secondBV->BVMinLocal;
        }
        else
        {
            rel = invFirst;
            pos = &gao2->matrices->Pos;
            secondBV = gao2->boundingVolume;
            MATH_VecAddVec(&secondWorldMax, &secondBV->BVMax, pos);
            MATH_VecAddVec(&secondWorldMin, &secondBV->BVMin, pos);
            secondMax = &secondWorldMax;
            secondMin = &secondWorldMin;
        }
    }
    else
    {
        if ((OBJ_pst_GetAbsoluteMatrix(gao2)->mtxflag & 8) != 0)
        {
            MATH_CopyMatrix(&secondMtx, OBJ_pst_GetAbsoluteMatrix(gao2));
            secondMtx._m14 = 0.0;
            secondMtx._m24 = 0.0;
            secondMtx._m34 = 0.0;
            secondMtx.mtxflag &= ~8u;
            rel = secondMtx;
        }
        else
        {
            MATH_CopyMatrix(&rel, OBJ_pst_GetAbsoluteMatrix(gao2));
        }
        firstBV = first->boundingVolume;
        pos = &first->matrices->Pos;
        MATH_VecAddVec(&firstWorldMax, &firstBV->BVMax, pos);
        MATH_VecAddVec(&firstWorldMin, &firstBV->BVMin, pos);
        secondBV = gao2->boundingVolume;
        firstMax = &firstWorldMax;
        firstMin = &firstWorldMin;
        secondMax = &secondBV->BVMaxLocal;
        secondMin = &secondBV->BVMinLocal;
    }
    centreA.x = (float)((firstMax->x - firstMin->x) * 0.5) + firstMin->x;
    centreA.y = (float)((firstMax->y - firstMin->y) * 0.5) + firstMin->y;
    centreA.z = (float)((firstMax->z - firstMin->z) * 0.5) + firstMin->z;
    if (OBBoxOfFirstGao)
    {
        mtx = OBJ_pst_GetAbsoluteMatrix(first);
        MATH_TransformVertexNoScale(&worldA, mtx, &centreA);
    }
    else
    {
        worldA = centreA;
    }
    centreB.x = (float)((secondMax->x - secondMin->x) * 0.5) + secondMin->x;
    centreB.y = (float)((secondMax->y - secondMin->y) * 0.5) + secondMin->y;
    centreB.z = (float)((secondMax->z - secondMin->z) * 0.5) + secondMin->z;
    if (OBBoxOfSecondGao)
    {
        mtx = OBJ_pst_GetAbsoluteMatrix(gao2);
        MATH_TransformVertexNoScale(&worldB, mtx, &centreB);
    }
    else
    {
        worldB = centreB;
    }
    delta.x = worldB.x - worldA.x;
    delta.y = worldB.y - worldA.y;
    delta.z = worldB.z - worldA.z;
    if (OBBoxOfFirstGao)
        MATH_TransformVectorNoScale(&relDelta, &invFirst, &delta);
    else
        relDelta = delta;
    rel.Pos = relDelta;
    unitScale.x = 1.0;
    unitScale.y = 1.0;
    unitScale.z = 1.0;
    return INT_HighLevel_OBBoxOBBox(firstMin, firstMax, secondMin, secondMax, &rel, &unitScale);
}
void COL_InitModule()
{
    COL_gst_GlobalVars.wallCosAngle = 0.5f;
}

tColInstSet* COL_AllocColSet()
{
    tColInstSet* colSet;

    colSet = (tColInstSet*)MEM_p_Alloc(sizeof(tColInstSet));
    LOA_ul_FileTypeSize_38 += sizeof(tColInstSet);
    colSet->instElemNum = 0;
    colSet->unk1 = 0;
    colSet->instanceNum = 0;
    colSet->colInstElems = 0;
    colSet->zoneIDs = 0;
    colSet->unk2 = 0;
    colSet->zoneIDs = (uint8_t*)MEM_p_Alloc(16u);
    LOA_ul_FileTypeSize_38 += 4;
    memset(colSet->zoneIDs, 0xff, 16);

    return colSet;
}

tColInstSet* COL_ul_CallBackLoadColSet(uint32_t Offset)
{
    tColInstSet* colSet;
    int zone;
    tColInstanceElem* elem;
    tColInstanceElem* elemEnd;
    int nameLength;
    char* cursor;
    int size;
    tColBox* box;
    tColSphere* sphere;
    tColCylinder* cylinder;

    cursor = BIG_pc_ReadFileTmp(Offset, &size);
    colSet = COL_AllocColSet();
    colSet->instElemNum = LOA_ReadChar(&cursor, 0, 1);
    colSet->unk1 = LOA_ReadChar(&cursor, 0, 1);
    colSet->instanceNum = 0;
    LOA_ReadShort(&cursor, 0, 2);
    for (zone = 0; zone < 16; ++zone)
        colSet->zoneIDs[zone] = LOA_ReadChar(&cursor, 0, 1);
    if (colSet->instElemNum)
        colSet->colInstElems = (tColInstanceElem*)MEM_p_Alloc(sizeof(tColInstanceElem) * colSet->instElemNum);
    elem = colSet->colInstElems;
    for (elemEnd = &elem[colSet->instElemNum]; elem < elemEnd; ++elem)
    {
        elem->colFlags = LOA_ReadChar(&cursor, 0, 1);
        elem->elemType = LOA_ReadChar(&cursor, 0, 1);
        elem->unk1 = LOA_ReadChar(&cursor, 0, 1);
        elem->design = LOA_ReadChar(&cursor, 0, 1);
        nameLength = LOA_ReadLong(&cursor, 0, 1);
        if (nameLength)
            LOA_ReadString(&cursor, 0, nameLength, 2);
        LOA_ReadChar(&cursor, 0, 2);
        switch (elem->elemType)
        {
        case 1u:
            box = (tColBox*)MEM_p_Alloc(sizeof(tColBox));
            elem->elemShapePtr = (void*)box;
            LOA_ReadVector(&cursor, &(box->pos1), 1);
            LOA_ReadVector(&cursor, &(box->pos2), 1);
            break;
        case 2u:
            sphere = (tColSphere*)MEM_p_Alloc(sizeof(tColSphere));
            elem->elemShapePtr = (void*)sphere;
            LOA_ReadVector(&cursor, &(sphere->center), 1);
            sphere->radius = LOA_ReadFloat(&cursor, 0, 1);
            break;
        case 3u:
            cylinder = (tColCylinder*)MEM_p_Alloc(sizeof(tColCylinder));
            elem->elemShapePtr = (void*)cylinder;
            LOA_ReadVector(&cursor, &(cylinder->center), 1);
            cylinder->height = LOA_ReadFloat(&cursor, 0, 1);
            cylinder->radius = LOA_ReadFloat(&cursor, 0, 1);
            break;
        }
    }
    return colSet;
}

struct sColInstance* COL_ul_CallBackLoadInstance(uint32_t Offset)
{
    tColInstance* instance;
    int colSetRef;
    uint8_t ownElemNum;
    tColInstanceElem** zoneSlots;
    unsigned int zoneSlotBytes;
    uint8_t zoneNum;
    uint8_t zone;
    tColInstanceElem* elem;
    tColInstanceElem* elemEnd;
    int nameLength;
    char* cursor;
    int size;
    tColBox* box;
    tColSphere* sphere;
    tColCylinder* cylinder;

    instance = (tColInstance*)MEM_p_Alloc(sizeof(tColInstance));
    memset(instance, 0, sizeof(tColInstance));
    instance->activationFlags = -1;
    instance->mtx = MATH_Matrix_Alloc();
    cursor = BIG_pc_ReadFileTmp(Offset, &size);
    colSetRef = LOA_ReadLong(&cursor, 0, 1);
    if (colSetRef != -1)
        LOA_MakeFileRef(colSetRef, &instance->colSet, COL_ul_CallBackLoadColSet, 1);
    if (size != 4)
    {
        instance->zoneNum = LOA_ReadChar(&cursor, 0, 1);
        instance->sharedZoneNum = LOA_ReadChar(&cursor, 0, 1);
        ownElemNum = LOA_ReadChar(&cursor, 0, 1);
        instance->instElemNum = ownElemNum;
        if (instance->sharedZoneNum + ownElemNum != instance->zoneNum)
        {
            instance->zoneNum = 0;
            instance->sharedZoneNum = 0;
            instance->instElemNum = 0;
            return instance;
        }
        instance->priority = LOA_ReadChar(&cursor, 0, 1);
        zoneNum = instance->zoneNum;
        if (zoneNum)
        {
            zoneSlots = (tColInstanceElem**)MEM_p_Alloc(4 * zoneNum);
            zoneSlotBytes = 4 * instance->zoneNum;
            instance->selectedInstElemPtrs = zoneSlots;
            memset(zoneSlots, 0, zoneSlotBytes);
        }
        for (zone = 0; zone < instance->zoneNum; ++zone)
            instance->selectedInstElemPtrs[zone] = (tColInstanceElem*)(uint8_t)LOA_ReadChar(&cursor, 0, 1);
        if (instance->instElemNum)
        {
            elem = (tColInstanceElem*)MEM_p_Alloc(sizeof(tColInstanceElem) * instance->instElemNum);
            elemEnd = &elem[instance->instElemNum];
            for (instance->colInstElems = elem; elem < elemEnd; ++elem)
            {
                elem->colFlags = LOA_ReadChar(&cursor, 0, 1);
                elem->elemType = LOA_ReadChar(&cursor, 0, 1);
                elem->unk1 = LOA_ReadChar(&cursor, 0, 1);
                elem->design = LOA_ReadChar(&cursor, 0, 1);
                LOA_ReadChar(&cursor, 0, 2);
                nameLength = LOA_ReadLong(&cursor, 0, 1);
                if (nameLength)
                    LOA_ReadString(&cursor, 0, nameLength, 2);
                LOA_ReadChar(&cursor, 0, 2);
                switch (elem->elemType)
                {
                case 1u:
                    box = (tColBox*)MEM_p_Alloc(sizeof(tColBox));
                    elem->elemShapePtr = (void *)box;
                    LOA_ReadVector(&cursor, &(box->pos1), 1);
                    LOA_ReadVector(&cursor, &(box->pos2), 1);
                    break;
                case 2u:
                    sphere = (tColSphere*)MEM_p_Alloc(sizeof(tColSphere));
                    elem->elemShapePtr = (void *)sphere;
                    LOA_ReadVector(&cursor, &(sphere->center), 1);
                    sphere->radius = LOA_ReadFloat(&cursor, 0, 1);
                    break;
                case 3u:
                    cylinder = (tColCylinder*)MEM_p_Alloc(sizeof(tColCylinder));
                    elem->elemShapePtr = (void *)cylinder;
                    LOA_ReadVector(&cursor, &(cylinder->center), 1);
                    cylinder->height = LOA_ReadFloat(&cursor, 0, 1);
                    cylinder->radius = LOA_ReadFloat(&cursor, 0, 1);
                    break;
                }
            }
        }
        instance->activationFlags = LOA_ReadShort(&cursor, 0, 1);
        instance->instElemBitmask = LOA_ReadShort(&cursor, 0, 1);
        LOA_ReadShort(&cursor, 0, 2);
        LOA_ReadShort(&cursor, 0, 1);
        instance->crossable = 0x589A;
        instance->flag = LOA_ReadShort(&cursor, 0, 1);
        instance->minLod = LOA_ReadChar(&cursor, 0, 1);
        instance->maxLod = LOA_ReadChar(&cursor, 0, 1);
        instance->wallPush = 1;
        instance->backface = 0;
    }
    return instance;
}

struct sCobGameMaterial* COL_ul_CallBackLoadGameMaterial(uint32_t Offset)
{
    tCobGameMaterial* material;
    int count;
    tCobGameMaterialElems* elems;
    int* ref;
    int i;
    tCobGameMaterialElems* elem;
    char* cursor;
    int size;

    cursor = BIG_pc_ReadFileTmp(Offset, &size);
    material = (tCobGameMaterial*)MEM_p_Alloc(sizeof(tCobGameMaterial));
    material->num = 0;
    material->ptrInts = 0;
    material->elems = 0;
    material->gameMaterialUsageCnt = 0;
    count = LOA_ReadLong(&cursor, 0, 1);
    material->num = count;
    material->ptrInts = (int*)MEM_p_Alloc(4 * count);
    elems = (tCobGameMaterialElems*)MEM_p_Alloc(sizeof(tCobGameMaterialElems) * material->num);
    ref = material->ptrInts;
    material->elems = elems;
    for (i = 0; i < material->num; ++i)
        *ref++ = LOA_ReadLong(&cursor, 0, 1);
    elem = material->elems;
    for (i = 0; i < material->num; ++i)
    {
        elem->flags = LOA_ReadLong(&cursor, 0, 1);
        elem->slide = LOA_ReadFloat(&cursor, 0, 1);
        elem->rebound = LOA_ReadFloat(&cursor, 0, 1);
        elem->sound = LOA_ReadChar(&cursor, 0, 1);
        elem->b2 = LOA_ReadChar(&cursor, 0, 1);
        elem->id = LOA_ReadShort(&cursor, 0, 1);
        ++elem;
    }
    return material;
}

tColCollisionObject* COL_ul_CallBackLoadCob(uint32_t Offset)
{
    tColCollisionObject* cob;
    int vertexIndex;
    int materialRef;
    uint8_t objType;
    tCobNormalObjectGeneric* generic;
    tColSphere* sphere;
    tColBox* box;
    tColCylinder* cylinder;
    tCobMeshObjectMesh* cobMesh;
    unsigned int count;
    Vec3* vertex;
    Vec3* normal;
    unsigned int i;
    tCobMeshChunkElem* chunk;
    tCobMeshChunkElem* chunkEnd;
    uint16_t triangleCount;
    int pointTotal;
    tCobMeshTriangleElem* triangles;
    int triangleIndex;
    tCobMeshChunkElem* proxChunk;
    tCobMeshChunkElem* proxEnd;
    tCobOK3* ok3;
    int boxIndex;
    unsigned int chunkIndex;
    int idIndex;
    unsigned int boxesRead;
    char* cursor;
    tCobMeshChunkElem* savedChunkEnd;
    unsigned int withProximity;
    unsigned int tagBytes;
    int size;

    cursor = BIG_pc_ReadFileTmp(Offset, &size);
    cob = (tColCollisionObject*)MEM_p_Alloc(sizeof(tColCollisionObject));
    cob->objectType = 0;
    cob->flags = 0;
    cob->usageCount = 0;
    vertexIndex = 0;
    cob->gameMaterial = NULL;
    cob->objectPtrByType = NULL;
    materialRef = LOA_ReadLong(&cursor, 0, 1);
    cob->objectType = LOA_ReadChar(&cursor, 0, 1);
    cob->flags = LOA_ReadChar(&cursor, 0, 1) & 0xFE;
    objType = cob->objectType;
    if (cob->objectType != 1 && objType != 2 && objType != 3 && objType != 5)
        return 0;
    if (materialRef != -1)
    {
        LOA_MakeFileRef(materialRef, &cob->gameMaterial, COL_ul_CallBackLoadGameMaterial, 1);
        cob->flags |= 1u;
    }
    if (cob->objectType && cob->objectType <= 3u)
    {
        generic = (tCobNormalObjectGeneric*)MEM_p_Alloc(sizeof(tCobNormalObjectGeneric));
        cob->objectPtrByType = (void*)generic;
        generic->enclosurePtr = 0;
        generic->gameMaterialElemID = 0;
        ((tCobNormalObjectGeneric*)(cob->objectPtrByType))->gameMaterialElemID = LOA_ReadLong(&cursor, 0, 1);
    }
    switch (cob->objectType)
    {
    case 1u:
        ((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr = (void *)MEM_p_Alloc(sizeof(tColBox));
        box = (tColBox*)((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr;
        LOA_ReadVector(&cursor, &box->pos1, 1);
        LOA_ReadVector(&cursor, &box->pos2, 1);
        break;
    case 2u:
        ((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr = (void *)MEM_p_Alloc(sizeof(tColSphere));
        sphere = (tColSphere*)((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr;
        LOA_ReadVector(&cursor, &sphere->center, 1);
        sphere->radius = LOA_ReadFloat(&cursor, 0, 1);
        break;
    case 3u:
        ((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr = (void *)MEM_p_Alloc(sizeof(tColCylinder));
        cylinder = (tColCylinder*)((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr;
        LOA_ReadVector(&cursor, &cylinder->center, 1);
        cylinder->height = LOA_ReadFloat(&cursor, 0, 1);
        cylinder->radius = LOA_ReadFloat(&cursor, 0, 1);
        break;
    case 5u:
        cobMesh = (tCobMeshObjectMesh*)MEM_p_Alloc(sizeof(tCobMeshObjectMesh));
        cob->objectPtrByType = cobMesh;
        cobMesh->ok3 = NULL;
        count = LOA_ReadLong(&cursor, 0, 1);
        cobMesh->vertexCount = count;
        vertex = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * count);
        cobMesh->vertices = vertex;
        if (cobMesh->vertexCount)
        {
            do
            {
                LOA_ReadVector(&cursor, vertex, 1);
                ++vertexIndex;
                ++vertex;
            } while ((unsigned int)vertexIndex < cobMesh->vertexCount);
        }
        count = LOA_ReadLong(&cursor, 0, 1);
        cobMesh->faceCount = count;
        normal = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * count);
        cobMesh->normals = normal;
        for (i = 0; i < cobMesh->faceCount; ++i)
        {
            LOA_ReadVector(&cursor, normal, 1);
            ++normal;
        }
        count = LOA_ReadLong(&cursor, 0, 1);
        cobMesh->meshChunkElemNum = count;
        chunk = (tCobMeshChunkElem*)MEM_p_Alloc(16 * count);
        chunkEnd = &chunk[cobMesh->meshChunkElemNum];
        cobMesh->meshChunkElems = chunk;
        for (savedChunkEnd = chunkEnd; chunk < chunkEnd; ++chunk)
        {
            triangleCount = LOA_ReadShort(&cursor, 0, 1);
            pointTotal = ColMapPoints;
            chunk->elemNum = triangleCount;
            ColMapPoints = triangleCount + pointTotal;
            chunk->designVal = LOA_ReadChar(&cursor, 0, 1);
            chunk->flags = (uint8_t)LOA_ReadChar(&cursor, 0, 1);
            chunk->gameMaterialElemID = LOA_ReadLong(&cursor, 0, 1);
            if (chunk->elemNum)
            {
                triangles = (tCobMeshTriangleElem*)MEM_p_Alloc(12 * chunk->elemNum);
                chunk->triangleElems = triangles;
                for (triangleIndex = 0; triangleIndex < chunk->elemNum; ++triangleIndex)
                {
                    chunk->triangleElems[triangleIndex].v1 = LOA_ReadShort(&cursor, 0, 1);
                    chunk->triangleElems[triangleIndex].v2 = LOA_ReadShort(&cursor, 0, 1);
                    chunk->triangleElems[triangleIndex].v3 = LOA_ReadShort(&cursor, 0, 1);
                }
                chunkEnd = savedChunkEnd;
            }
            else
            {
                chunk->triangleElems = 0;
            }
        }
        if (!LOA_gb_SpeedMode || (withProximity = 1, BIG_gi_ReadMode != 2))
            withProximity = 0;
        if ((cob->flags & 0x80u) != 0)
        {
            if (withProximity)
            {
                proxChunk = cobMesh->meshChunkElems;
                proxEnd = &proxChunk[cobMesh->meshChunkElemNum];
                for (; proxChunk < proxEnd; ++proxChunk)
                {
                    if (proxChunk->elemNum)
                    {
                        for (triangleIndex = 0; triangleIndex < proxChunk->elemNum; ++triangleIndex)
                        {
                            proxChunk->triangleElems[triangleIndex].proximity1 = LOA_ReadShort(&cursor, 0, 1);
                            proxChunk->triangleElems[triangleIndex].proximity2 = LOA_ReadShort(&cursor, 0, 1);
                            proxChunk->triangleElems[triangleIndex].proximity3 = LOA_ReadShort(&cursor, 0, 1);
                        }
                    }
                }
            }
        }
        if ((cob->flags & 0x88) == 0x88 && withProximity)
        {
            ok3 = (tCobOK3*)MEM_p_Alloc(sizeof(tCobOK3));
            tagBytes = ((cobMesh->faceCount >> 3) & 0x1FFFFFFC) + 4;
            cobMesh->ok3 = ok3;
            cobMesh->ok3->faceTagInts = (int*)MEM_p_Alloc(tagBytes);
            memset(cobMesh->ok3->faceTagInts, 0, 4 * (cobMesh->faceCount >> 5) + 4);
            cobMesh->ok3->num = LOA_ReadLong(&cursor, 0, 1);
            cobMesh->ok3->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(sizeof(tCobOK3AABBoxElem) * cobMesh->ok3->num);
            boxesRead = 0;
            if (cobMesh->ok3->num > 0)
            {
                boxIndex = 0;
                do
                {
                    cobMesh->ok3->aaBoxes[boxIndex].num = LOA_ReadLong(&cursor, 0, 1);
                    LOA_ReadVector(&cursor, &cobMesh->ok3->aaBoxes[boxIndex].vec2, 1);
                    LOA_ReadVector(&cursor, &cobMesh->ok3->aaBoxes[boxIndex].vec1, 1);
                    cobMesh->ok3->aaBoxes[boxIndex].chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(sizeof(tCobOK3AABBoxElemChunkElem) * cobMesh->ok3->aaBoxes[boxIndex].num);
                    for (chunkIndex = 0; chunkIndex < cobMesh->ok3->aaBoxes[boxIndex].num; ++chunkIndex)
                    {
                        cobMesh->ok3->aaBoxes[boxIndex].chunkElems[chunkIndex].triangleChunkID = LOA_ReadShort(&cursor, 0, 1);
                        cobMesh->ok3->aaBoxes[boxIndex].chunkElems[chunkIndex].num = LOA_ReadShort(&cursor, 0, 1);
                        cobMesh->ok3->aaBoxes[boxIndex].chunkElems[chunkIndex].triangleChunkElemIDs = (uint16_t*)MEM_p_Alloc(2 * cobMesh->ok3->aaBoxes[boxIndex].chunkElems[chunkIndex].num);
                        for (idIndex = 0; idIndex < cobMesh->ok3->aaBoxes[boxIndex].chunkElems[chunkIndex].num; ++idIndex)
                            cobMesh->ok3->aaBoxes[boxIndex].chunkElems[chunkIndex].triangleChunkElemIDs[idIndex] = LOA_ReadShort(&cursor, 0, 1);
                    }
                    ++boxIndex;
                } while (++boxesRead < cobMesh->ok3->num);
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
    cob->flags &= ~0x80u;
    cob->usageCount = 0;
    return cob;
}

struct sColMap* COL_ul_CallBackLoadColMap(uint32_t Offset)
{
    tColMap* colMap;
    int cobRef;
    tColCollisionObject** cobs;
    tColCollisionObject** cob;
    tColCollisionObject** cobEnd;
    unsigned int cobBytes;
    char* cursor;
    int size;

    cursor = BIG_pc_ReadFileTmp(Offset, &size);
    colMap = (tColMap*)MEM_p_Alloc(sizeof(tColMap));
    colMap->colObjectID = 0;
    colMap->activation = 0;
    colMap->b3 = 0;
    colMap->b4 = 0;
    colMap->collisionObjectPtr = 0;
    colMap->activation = -1;
    if (size == 4)
    {
        cobRef = LOA_ReadLong(&cursor, 0, 1);
        colMap->colObjectID = 1;
        colMap->activation = -1;
        colMap->b3 = 0;
        colMap->b4 = 0;
        cobs = (tColCollisionObject**)MEM_p_Alloc(4u);
        colMap->collisionObjectPtr = cobs;
        LOA_MakeFileRef(cobRef, cobs, COL_ul_CallBackLoadCob, 1);
    }
    else
    {
        colMap->colObjectID = LOA_ReadChar(&cursor, 0, 1);
        colMap->activation = LOA_ReadChar(&cursor, 0, 1);
        colMap->b3 = LOA_ReadChar(&cursor, 0, 1);
        colMap->b4 = LOA_ReadChar(&cursor, 0, 1);
        cobs = (tColCollisionObject**)MEM_p_Alloc(4 * colMap->colObjectID);
        cobBytes = 4 * colMap->colObjectID;
        colMap->collisionObjectPtr = cobs;
        memset(cobs, 0, cobBytes);
        cob = colMap->collisionObjectPtr;
        for (cobEnd = &cob[colMap->colObjectID]; cob < cobEnd; ++cob)
        {
            cobRef = LOA_ReadLong(&cursor, 0, 1);
            if (cobRef != -1)
                LOA_MakeFileRef(cobRef, cob, COL_ul_CallBackLoadCob, 1);
        }
    }
    return colMap;
}

struct sGameObjectMain* COL_RayBV(struct tableDescStruct* table, Vec3* rayOrigin, Vec3* rayDir, float rayDistance, int flagMustHave, int flagMustNothave, int whatToTest, int skipCrossObjects)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGameObjectMain* hit;
    float bestDistance;

    bestDistance = rayDistance;
    last = ((tableElemStructPF*)(table->nextElemPtr)) - 1;
    hit = 0;
    for (elem = table->firstElemPtr; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (!elem->objectPtr
            || ((uint8_t)gao & 3) == 1
            || skipCrossObjects && (gao->flags2 & 0x1000000) != 0
            || !OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
        {
            continue;
        }
        if (OBJ_BV_IsSphere(gao->boundingVolume))
        {
            if (!COL_RaySphereBV(rayOrigin, rayDir, gao, &rayDistance, bestDistance) || rayDistance == -1.0f)
                continue;
        }
        else
        {
            if (!COL_RayAABBox(rayOrigin, rayDir, gao, &rayDistance, bestDistance) || rayDistance == -1.0f)
                continue;
        }
        bestDistance = rayDistance;
        hit = gao;
    }
    return hit;
}

int COL_Report_CollideType(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if ((reportFilter & 0x10000) != 0)
    {
        if (gao && (extended = gao->extended) != 0 && (coll = extended->coll) != 0 && (instance = coll->colInstance) != 0)
            return (uint16_t)COL_w_TestInstanceFlag(instance, 2);
        return 0;
    }
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if ((rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x3000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter)
                return 1;
        }
    }
    return 0;
}

void COL_Report_GetNormal(struct sGameObjectMain* gao, struct sWorld* world, Vec3* outNormal, int reportFilter)
{
    int skipZoneReports;
    int skipSlippery;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!outNormal)
        return;
    outNormal->x = 0.0;
    outNormal->y = 0.0;
    outNormal->z = 0.0;
    if (reportFilter < 0)
    {
        *outNormal = world->records[(uint16_t)reportFilter].normalVec;
        return;
    }
    skipZoneReports = 1;
    skipSlippery = 0;
    if ((reportFilter & 0x2000) != 0)
        skipZoneReports = 0;
    if ((reportFilter & 0x40000000) != 0)
        skipSlippery = 1;
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if ((!skipZoneReports || (rec->flags & 0x1000) == 0)
            && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && (!skipZoneReports || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0
                && (!skipSlippery || (flags & 0x800) == 0)
                && (reportFilter & 0xBFFFFFFF & flags) == (reportFilter & 0xBFFFFFFF))
            {
                if (rec->collisionInstanceGameObject == gao)
                    *outNormal = rec->normalVec;
                else
                    MATH_VecInvert(outNormal, &rec->normalVec);
                return;
            }
        }
    }
}

struct sGameObjectMain* COL_ColMap_RayCast(struct sWorld* world, struct sGameObjectMain* excludeGao, Vec3* rayOrigin, Vec3* rayDir, float rayDistance, int flagMustHave, int flagMustNothave, int whatToTest, int* outClearPath, int skipRayCast, int skipCrossable, int skipCamera, int skipFlagX, int useUserCrossable)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tWorld* rayWorld;
    tColMap* colMap;
    tColCollisionObject** cobs;
    tColCollisionObject* cob;
    tCobNormalObjectGeneric* shape;
    tCobGameMaterialElems* gameMaterial;
    uint16_t crossable;
    uint8_t cobIndex;
    float bvDistance;

    COL_gst_Ray.skipCrossable = skipCrossable;
    COL_gst_Ray.skipCamera = skipCamera;
    COL_gst_Ray.useUserCrossable = useUserCrossable;
    COL_gst_Ray.skipRayCast = skipRayCast;
    COL_gst_Ray.direction = rayDir;
    COL_gst_Ray.distance = rayDistance;
    COL_gst_Ray.skipFlagX = skipFlagX;
    COL_gst_Ray.worldSetInColMapRayCast = world;
    COL_gst_Ray.sourcePoint = rayOrigin;
    COL_gst_Ray.ignoreColMapRayCastResult = outClearPath;
    COL_gst_Ray.gao = 0;
    COL_gst_Ray.colMapGao = excludeGao;
    COL_gst_Ray.skipVecsTransformationToGaoSpace = 0;
    if (outClearPath)
        *outClearPath = 1;
    world->colCrossObjectNumber = 0;
    last = ((tableElemStructPF*)(world->eot.collMapObjectsTablePF.nextElemPtr)) - 1;
    for (elem = world->eot.collMapObjectsTablePF.firstElemPtr; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (!elem->objectPtr
            || ((uint8_t)gao & 3) == 1
            || (gao->flags & 0x100) == 0
            || (gao->flags2 & 1) == 0
            || !OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest)
            || gao == excludeGao
            || (!OBJ_BV_IsAABBox(gao->boundingVolume) || !COL_RayAABBox(rayOrigin, rayDir, gao, &bvDistance, COL_gst_Ray.distance))
            && (!OBJ_BV_IsSphere(gao->boundingVolume) || !COL_RaySphereBV(rayOrigin, rayDir, gao, &bvDistance, COL_gst_Ray.distance)))
        {
            continue;
        }
        if (COL_gst_Ray.skipRayCast && (gao->flags2 & 0x1000000) != 0)
        {
            rayWorld = COL_gst_Ray.worldSetInColMapRayCast;
            if (rayWorld->colCrossObjectNumber == 16)
                rayWorld->colCrossObjectNumber = 15;
            rayWorld->collCrossObjects[rayWorld->colCrossObjectNumber] = gao;
            ++COL_gst_Ray.worldSetInColMapRayCast->colCrossObjectNumber;
            continue;
        }
        if ((gao->flags & 0x100) == 0)
            continue;
        colMap = gao->extended->coll->colMap;
        cobs = colMap->collisionObjectPtr;
        if ((*cobs)->objectType != 5)
        {
            for (cobIndex = 0; cobIndex < colMap->colObjectID; ++cobIndex)
            {
                if (((uint8_t)(1 << cobIndex) & colMap->activation) == 0)
                    continue;
                cob = colMap->collisionObjectPtr[cobIndex];
                shape = (tCobNormalObjectGeneric*)cob->objectPtrByType;
                if (skipCrossable)
                {
                    if (useUserCrossable)
                    {
                        gameMaterial = COL_pst_GMat_Get(cob, 0);
                        if (gameMaterial && (COL_guw_UserCrossable & gameMaterial->flags) != 0)
                            continue;
                    }
                    else if (cob->gameMaterial)
                    {
                        if (excludeGao && (excludeGao->flags & 0x200) != 0)
                            crossable = excludeGao->extended->coll->colInstance->crossable;
                        else
                            crossable = 0x589A;
                        gameMaterial = COL_pst_GMat_Get(cob, 0);
                        if (gameMaterial && (crossable & gameMaterial->flags) != 0)
                            continue;
                    }
                }
                switch (colMap->collisionObjectPtr[cobIndex]->objectType)
                {
                case 1u:
                    COL_ColMap_Box_RayCast(gao, shape);
                    break;
                case 2u:
                    COL_ColMap_Sphere_RayCast(gao, shape);
                    break;
                case 3u:
                    COL_ColMap_Cylinder_RayCast(gao, shape);
                    break;
                }
            }
            continue;
        }
        if ((colMap->activation & 1) != 0)
        {
            if (((tCobMeshObjectMesh*)((*cobs)->objectPtrByType))->ok3)
                COL_ColMap_OK3_RayCast(gao, gao->extended->coll->colMap);
            else
                COL_ColMap_Triangles_RayCast(gao, gao->extended->coll->colMap);
            if (COL_gst_Ray.ignoreColMapRayCastResult && !*COL_gst_Ray.ignoreColMapRayCastResult)
                return 0;
        }
    }
    COL_gb_UseMinDistance = 0;
    return COL_gst_Ray.gao;
}

struct sGameObjectMain* COL_Report_GetCOBGameObject(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    int rank;
    tGameObjectMain* result;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;
    BOOL skipZoneReports;

    if (reportFilter < 0)
    {
        rank = (uint16_t)reportFilter;
        result = world->records[rank].collisionMapGameObject;
        if (result == gao)
            result = world->records[rank].collisionInstanceGameObject;
        return result;
    }
    skipZoneReports = (reportFilter & 0x2000) == 0;
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if (!skipZoneReports || (rec->flags & 0x3000) == 0)
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0)
            {
                if (rec->collisionInstanceGameObject == gao && (reportFilter & flags) == reportFilter && rec->collisionMapGameObject)
                    return rec->collisionMapGameObject;
                if (rec->collisionMapGameObject == gao && (reportFilter & flags) == reportFilter && rec->collisionInstanceGameObject)
                    return rec->collisionInstanceGameObject;
            }
        }
    }
    return 0;
}

int COL_GameMaterial_GetFlags(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    tCobGameMaterialElems* gameMaterial;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!world)
        return 0;
    if (reportFilter == 0xFFFF)
    {
        gameMaterial = world->rayCastCobGmat;
        if (gameMaterial)
            return gameMaterial->flags;
        return 0;
    }
    if (reportFilter < 0)
    {
        gameMaterial = world->records[(uint16_t)reportFilter].gameMaterial;
        if (!gameMaterial)
            return 0;
        return gameMaterial->flags;
    }
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if (((reportFilter & 0x2000) != 0 || (rec->flags & 0x1000) == 0)
            && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter)
            {
                if (rec->gameMaterial)
                    return rec->gameMaterial->flags;
                return 0;
            }
        }
    }
    return 0;
}

float COL_GameMaterial_GetSlide(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    tCobGameMaterialElems* gameMaterial;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!world)
        return 0.0f;
    if (reportFilter == 0xFFFF)
    {
        gameMaterial = world->rayCastCobGmat;
        if (gameMaterial)
            return gameMaterial->slide;
        return 0.0;
    }
    if (reportFilter < 0)
    {
        gameMaterial = world->records[(uint16_t)reportFilter].gameMaterial;
        if (!gameMaterial)
            return 0.0;
        return (float)gameMaterial->slide;
    }
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if (((reportFilter & 0x2000) != 0 || (rec->flags & 0x1000) == 0)
            && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter && rec->gameMaterial)
                return (float)rec->gameMaterial->slide;
        }
    }
    return 0.0f;
}

float COL_GameMaterial_GetRebound(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    tCobGameMaterialElems* gameMaterial;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!world)
        return 0.0;
    if (reportFilter == 0xFFFF)
    {
        gameMaterial = world->rayCastCobGmat;
        if (gameMaterial)
            return gameMaterial->rebound;
        return 0.0;
    }
    if (reportFilter < 0)
    {
        // preserved: the rank is read without the (uint16_t) narrowing its three siblings use, so a tagged filter indexes far in front of the record array
        gameMaterial = world->records[reportFilter].gameMaterial;
        if (!gameMaterial)
            return 0.0;
        return (float)gameMaterial->rebound;
    }
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if (((reportFilter & 0x2000) != 0 || (rec->flags & 0x1000) == 0)
            && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter && rec->gameMaterial)
                return (float)rec->gameMaterial->rebound;
        }
    }
    return 0.0;
}

uint8_t COL_GameMaterial_GetSound(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    tCobGameMaterialElems* gameMaterial;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!world)
        return 0;
    if (reportFilter == 0xFFFF)
    {
        gameMaterial = world->rayCastCobGmat;
        if (gameMaterial)
            return gameMaterial->sound;
        return 0;
    }
    if (reportFilter < 0)
    {
        gameMaterial = world->records[(uint16_t)reportFilter].gameMaterial;
        if (!gameMaterial)
            return 0;
        return gameMaterial->sound;
    }
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if (((reportFilter & 0x2000) != 0 || (rec->flags & 0x1000) == 0)
            && rec->collisionInstanceGameObject == gao
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter && rec->gameMaterial)
                return rec->gameMaterial->sound;
        }
    }
    return 0;
}
void COL_FindEdgeInFollowDirection(Vec3* hitPoint, Vec3** point1, Vec3** point2, Vec3** point3, Vec3* right, Vec3* normal, int down)
{
    Vec3* first;
    Vec3* second;
    Vec3* third;
    Vec3* extreme;
    Vec3* chosen;
    float side;
    BOOL behind;

    first = *point1;
    second = *point2;
    third = *point3;
    if (down)
    {
        if (first->z > (double)second->z || first->z > (double)third->z)
        {
            if (second->z > (double)first->z || second->z > (double)third->z)
                extreme = third;
            else
                extreme = second;
        }
        else
        {
            extreme = first;
        }
    }
    else
    {
        if (first->z < (double)second->z || first->z < (double)third->z)
        {
            if (second->z < (double)first->z || second->z < (double)third->z)
                extreme = third;
            else
                extreme = second;
        }
        else
        {
            extreme = first;
        }
    }
    side = right->z * (extreme->z - hitPoint->z) + right->y * (extreme->y - hitPoint->y)
        + (extreme->x - hitPoint->x) * right->x; // dotproduct
    behind = side <= 0.0f;
    if (down)
    {
        if (extreme == second && side <= 0.0f || extreme == third && side > 0.0f)
            chosen = third;
        else
            chosen = first;
        *point1 = chosen;
        if (third != chosen && (extreme == chosen && !behind || extreme == third && behind))
            *point2 = third;
    }
    else
    {
        if (extreme == second && side > 0.0f || extreme == third && side <= 0.0f)
            chosen = third;
        else
            chosen = first;
        *point1 = chosen;
        if (third != chosen && (extreme == chosen && behind || extreme == third && !behind))
            *point2 = third;
    }
}

tCobMeshTriangleElem* COL_GetZConnected(Vec3* point1, Vec3* point2, tCobMeshObjectMesh* mesh, tCobMeshChunkElem* chunk, tCobMeshTriangleElem* skip)
{
    tCobMeshTriangleElem* tri;
    Vec3* vertices;
    Vec3* corner1;
    Vec3* corner2;
    Vec3* corner3;
    int i;

    tri = chunk->triangleElems;
    for (i = 0; i < chunk->elemNum; ++i)
    {
        if (tri != skip)
        {
            vertices = mesh->vertices;
            corner1 = &vertices[tri->v1];
            corner2 = &vertices[tri->v2];
            corner3 = &vertices[tri->v3];
            if ((point1 == corner1 || point1 == corner2 || point1 == corner3)
                && (point2 == corner1 || point2 == corner2 || point2 == corner3))
            {
                return tri;
            }
        }
        ++tri;
    }
    return 0;
}

static int COL_l_TakeMoreHorizontalEdge(Vec3* edgeFrom, Vec3* edgeTo, int fromIsShared, float* best, Vec3* outShared, Vec3* outOther)
{
    double deltaX;
    double deltaY;
    double deltaZ;
    float deltaZf;
    long double invLength;
    float invLengthf;
    float unitX;
    long double verticality;

    deltaX = edgeFrom->x - edgeTo->x;
    deltaY = edgeFrom->y - edgeTo->y;
    deltaZ = edgeFrom->z - edgeTo->z;
    deltaZf = (float)deltaZ;
    invLength = 1.0 / sqrt(deltaZ * deltaZf + deltaY * deltaY + deltaX * deltaX);
    invLengthf = (float)invLength;
    unitX = (float)(invLength * deltaX);
    verticality = fabs(deltaZf * invLengthf * MATH_gst_BaseVectorK.z
        + deltaY * invLengthf * MATH_gst_BaseVectorK.y
        + MATH_gst_BaseVectorK.x * unitX);
    if (verticality < *best)
    {
        *best = (float)verticality;
        if (fromIsShared)
        {
            *outShared = *edgeFrom;
            *outOther = *edgeTo;
        }
        else
        {
            *outShared = *edgeTo;
            *outOther = *edgeFrom;
        }
        return 1;
    }
    return 0;
}

tCobMeshTriangleElem* COL_GetPointConnectedTriangleWithMuchHorizontalEdge(Vec3* sharedPoint, Vec3* outShared, Vec3* outOther, tCobMeshObjectMesh* mesh, Mtx4x4* mtx, tCobMeshChunkElem* chunk)
{
    tCobMeshTriangleElem* tri;
    tCobMeshTriangleElem* bestTri;
    Vec3* vertices;
    Vec3* corner1;
    Vec3* corner2;
    Vec3* corner3;
    float best;
    int i;
    Vec3 world1;
    Vec3 world2;
    Vec3 world3;

    bestTri = 0;
    best = 100000.0f;
    for (i = 0; i < chunk->elemNum; ++i)
    {
        tri = &chunk->triangleElems[i];
        vertices = mesh->vertices;
        corner1 = &vertices[tri->v1];
        corner2 = &vertices[tri->v2];
        corner3 = &vertices[tri->v3];
        if (sharedPoint != corner1 && sharedPoint != corner2 && sharedPoint != corner3)
            continue;
        MATH_TransformVertexNoScale(&world1, mtx, corner1);
        MATH_TransformVertexNoScale(&world2, mtx, corner2);
        MATH_TransformVertexNoScale(&world3, mtx, corner3);
        if (sharedPoint == corner1 || sharedPoint == corner2)
        {
            if (COL_l_TakeMoreHorizontalEdge(&world1, &world2, sharedPoint == corner1, &best, outShared, outOther))
            {
                bestTri = tri;
                if (fabs(best) < 0.001)
                    return bestTri;
            }
        }
        if (sharedPoint == corner1 || sharedPoint == corner3)
        {
            if (COL_l_TakeMoreHorizontalEdge(&world1, &world3, sharedPoint == corner1, &best, outShared, outOther))
            {
                bestTri = tri;
                if (fabs(best) < 0.001)
                    return bestTri;
            }
        }
        if (sharedPoint == corner2 || sharedPoint == corner3)
        {
            if (COL_l_TakeMoreHorizontalEdge(&world2, &world3, sharedPoint == corner2, &best, outShared, outOther))
            {
                bestTri = tri;
                if (fabs(best) < 0.001)
                    return bestTri;
            }
        }
    }
    return bestTri;
}

// TODO FIXME átnézni, fõleg a &vec local változókat
void COL_ComputeJumpPoint(struct sGameObjectMain* gao, Vec3* outPoint, float margin, int down, int useLastRayHit)
{
    tWorld* world;
    int elemIndex;
    uint16_t chunkID;
    tGameObjectMain* hitGao;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColMap* colMap;
    tColCollisionObject* cob;
    tCobMeshObjectMesh* mesh;
    tCobMeshTriangleElem* tri;
    tCobMeshTriangleElem* neighbour;
    tCobMeshTriangleElem* normalTri;
    Mtx4x4* mtx;
    Vec3* chosenA;
    Vec3* vertices;
    Vec3* edgeA;
    Vec3* edgeB;
    Vec3* corner1;
    Vec3* corner2;
    Vec3* corner3;
    tCobMeshChunkElem* chunk;
    double along;
    double offsetZ;
    double unitZ;
    double deltaX;
    double deltaY;
    double deltaZ;
    long double invLength;
    long double borderLength;
    uint8_t step;
    BOOL lower;
    BOOL equal;
    int sharedIndex;
    int otherIndex;
    int walk;
    float twiceMargin;
    tWorld* hitWorld = NULL; // TODO FIXME: game doesn't set it to anything, should chekc it. Game calls this function with a5 being set to true, so this shouldn1t happen.
    Vec3* worldC;
    float edgeLength;
    float offsetX;
    float offsetY;
    float invLengthf;
    float borderFactor;
    Vec3 edgeVec;
    Vec3 world1;
    Vec3 world2;
    Vec3 world3;
    Vec3 right;
    Vec3 normal;
    Vec3 toPoint;
    Vec3 side;
    Vec3* worldA;
    Vec3* worldB;

    *outPoint = MATH_gst_NulVector;
    world = WOR_gpst_CurrentWorld;
    if (!WOR_gpst_CurrentWorld)
        return;
    if (useLastRayHit)
    {
        outPoint->x = world->colRayHitPos.x;
        outPoint->y = world->colRayHitPos.y;
        outPoint->z = world->colRayHitPos.z;
        elemIndex = world->rayCastTriangleChunkElemID;
        chunkID = world->rayCastTriangleChunkID;
        hitGao = world->rayCastGao;
        hitWorld = world;
    }
    else
    {
        rec = world->records;
        end = &world->records[world->recordCount];
        if (rec < end)
        {
            do
            {
                if (rec->collisionInstanceGameObject == gao)
                {
                    flags = rec->flags;
                    if ((flags & 0x80u) != 0 && (flags & 0xF) != 0)
                        break;
                }
                ++rec;
            } while (rec < end);
        }
        // preserved: with no matching record the cursor is one past the end and this reads past it
        flags = rec->flags;
        if ((flags & 0x80u) == 0 || (flags & 0xF) == 0)
            return;
        *outPoint = rec->collidedPoint;
        elemIndex = rec->triangleChunkElemID;
        chunkID = rec->triangleChunkID;
        hitGao = rec->collisionMapGameObject;
    }
    if (!hitGao)
        return;
    if ((hitGao->flags & 0x100) == 0)
        return;
    extended = hitGao->extended;
    if (!extended)
        return;
    coll = extended->coll;
    if (!coll)
        return;
    colMap = coll->colMap;
    if (!colMap)
        return;
    cob = *colMap->collisionObjectPtr;
    if (!cob || cob->objectType != 5)
        return;
    mesh = (tCobMeshObjectMesh*)(cob->objectPtrByType);
    twiceMargin = margin + margin;
    tri = &mesh->meshChunkElems[chunkID].triangleElems[elemIndex];
    walk = 0;
    for (;;)
    {
        MATH_TransformVertexNoScale(&world1, OBJ_pst_GetAbsoluteMatrix(hitGao), &mesh->vertices[tri->v1]);
        MATH_TransformVertexNoScale(&world2, OBJ_pst_GetAbsoluteMatrix(hitGao), &mesh->vertices[tri->v2]);
        MATH_TransformVertexNoScale(&world3, OBJ_pst_GetAbsoluteMatrix(hitGao), &mesh->vertices[tri->v3]);
        worldA = &world1;
        worldB = &world2;
        worldC = &world3;
        MATH_SubstractVec(&edgeVec, &world2, &world1);
        MATH_SubstractVec(&side, &world3, &world1);
        MATH_CrossProduct(&normal, &edgeVec, &side);
        MATH_NormalizeEqualVector(&normal);
        MATH_CrossProduct(&right, &MATH_gst_BaseVectorK, &normal);
        COL_FindEdgeInFollowDirection(outPoint, &worldA, &worldB, &worldC, &right, &normal, down);
        chosenA = worldA;
        MATH_SubstractVec(&edgeVec, worldB, worldA);
        edgeLength = MATH_DistanceOfTwoPoints(chosenA, worldB);
        MATH_SubstractVec(&toPoint, chosenA, outPoint);
        along = -(right.z * toPoint.z + right.y * toPoint.y + right.x * toPoint.x)
            / (edgeVec.z * right.z + edgeVec.y * right.y + edgeVec.x * right.x);
        if (edgeLength <= (double)twiceMargin)
        {
            along = edgeLength * 0.5;
        }
        else if (edgeLength * along < margin || (1.0 - along) * edgeLength < margin)
        {
            along = margin / edgeLength;
            if (edgeLength > twiceMargin)
                along = 1.0 - along;
        }
        offsetX = (float)(edgeVec.x * along);
        outPoint->x = offsetX;
        offsetY = (float)(edgeVec.y * along);
        outPoint->y = offsetY;
        offsetZ = along * edgeVec.z;
        outPoint->z = (float)offsetZ;
        outPoint->x = offsetX + chosenA->x;
        normalTri = tri;
        outPoint->y = (float)(offsetY + chosenA->y);
        outPoint->z = (float)(offsetZ + chosenA->z);
        vertices = mesh->vertices;
        if (&world3 == chosenA)
            sharedIndex = tri->v3;
        else
            sharedIndex = tri->v1;
        edgeA = &vertices[sharedIndex];
        if (&world3 == worldB)
            otherIndex = tri->v3;
        else
            otherIndex = tri->v2;
        edgeB = &vertices[otherIndex];
        neighbour = COL_GetZConnected(edgeA, edgeB, mesh, &mesh->meshChunkElems[chunkID], tri);
        for (;;)
        {
            if (!neighbour)
            {
                MATH_TransformVertexNoScale(&world1, OBJ_pst_GetAbsoluteMatrix(hitGao), edgeA);
                MATH_TransformVertexNoScale(&world2, OBJ_pst_GetAbsoluteMatrix(hitGao), edgeB);
                deltaX = world1.x - world2.x;
                deltaY = world1.y - world2.y;
                deltaZ = world1.z - world2.z;
                edgeVec.z = (float)deltaZ;
                invLength = 1.0 / sqrt(deltaZ * edgeVec.z + deltaY * deltaY + deltaX * deltaX);
                invLengthf = (float)invLength;
                edgeVec.x = (float)(invLength * deltaX);
                edgeVec.y = (float)(deltaY * invLengthf);
                unitZ = edgeVec.z * invLengthf;
                edgeVec.z = (float)unitZ;
                if (fabs(unitZ * MATH_gst_BaseVectorK.z + edgeVec.y * MATH_gst_BaseVectorK.y + edgeVec.x * MATH_gst_BaseVectorK.x) > gcjpUnkFloatflt_968524)
                {
                    lower = world1.z < (double)world2.z;
                    equal = world1.z == world2.z;
                    if (down)
                    {
                        if (!lower && !equal)
                            edgeA = edgeB;
                    }
                    else if (lower || equal)
                    {
                        edgeA = edgeB;
                    }
                    chunk = &mesh->meshChunkElems[chunkID];
                    mtx = OBJ_pst_GetAbsoluteMatrix(hitGao);
                    COL_GetPointConnectedTriangleWithMuchHorizontalEdge(edgeA, &world1, &world2, mesh, mtx, chunk);
                    edgeVec.x = world2.x - world1.x;
                    edgeVec.y = world2.y - world1.y;
                    edgeVec.z = world2.z - world1.z;
                    borderLength = MATH_DistanceOfTwoPoints(&world2, &world1);
                    if (borderLength <= twiceMargin)
                        borderFactor = (float)(borderLength * 0.5);
                    else
                        borderFactor = (float)(margin / borderLength);
                    MATH_VecMultScalar(outPoint, &edgeVec, borderFactor);
                    MATH_VecAddVecWithoutBuffer(outPoint, &world1);
                    break;
                }
            }
            step = ++walk;
            if (walk != 14)
                break;
            neighbour = 0;
        }
        if (!neighbour)
        {
            vertices = mesh->vertices;
            corner1 = &vertices[normalTri->v1];
            corner2 = &vertices[normalTri->v2];
            corner3 = &vertices[normalTri->v3];
            MATH_SubstractVec(&normal, corner2, corner1);
            MATH_SubstractVec(&side, corner3, corner1);
            MATH_CrossProduct(&toPoint, &normal, &side);
            MATH_NormalizeEqualVector(&toPoint);
            MATH_TransformVector(&(hitWorld->rayCastNormalVec), hitGao->matrices, &toPoint);
            return;
        }
        if (step >= 15u)
            break;
        tri = neighbour;
    }
    // preserved: unreachable -- the walk counter only reaches 15 on a step that leaves no neighbour, and that path has already returned above
    hitWorld->rayCastNormalVec.x = 0;
    hitWorld->rayCastNormalVec.y = 0;
    hitWorld->rayCastNormalVec.z = 0;
}
int COL_Report_GetZDMGameObjectList(struct sGameObjectMain* gao, struct sWorld* world, struct sGameObjectMain** outList, Vec3* direction, int reportFilter)
{
    int count;
    int index;
    tRecordElem* rec;

    count = 0;
    if ((gao->flags & 0x100) == 0)
        return 0;
    rec = world->records;
    for (index = 0; index < world->recordCount; ++index)
    {
        if (rec->collisionMapGameObject == gao
            && (reportFilter & rec->flags) == reportFilter
            && (fabs(direction->x) == 0.0f // todo inline
                && fabs(direction->y) == 0.0f
                && fabs(direction->z) == 0.0f
                || rec->normalVec.z * direction->z + direction->y * rec->normalVec.y + rec->normalVec.x * direction->x > 0.0f))
        {
            outList[count] = rec->collisionInstanceGameObject;
            ++count;
        }
        ++rec;
    }
    return count;
}

struct sGameObjectMain* COL_Report_GetBestWallGOInDirection(struct sGameObjectMain* gao, struct sWorld* world, Vec3* direction, float minCos, int* outRank)
{
    tRecordElem* rec;
    tRecordElem* end;
    int index;
    int flags;
    double facing;
    tGameObjectMain* other;
    tGameObjectMain* best;
    Vec3 wanted;
    float bestFacing;

    if (minCos == 0.0)
        bestFacing = 0.0;
    else
        bestFacing = (float)(0.001 - minCos);
    rec = world->records;
    index = 0;
    end = &world->records[world->recordCount];
    best = 0;
    if (outRank)
        *outRank = 0;
    if (rec >= end)
        return 0;

    do
    {
        if (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
        {
            flags = rec->flags;
            if ((flags & 0x80u) != 0 && (flags & 0x80052800) == 0)
            {
                if (rec->collisionInstanceGameObject == gao)
                    wanted = *direction;
                else
                    MATH_VecInvert(&wanted, direction);
                facing = rec->normalVec.z * wanted.z + rec->normalVec.y * wanted.y + wanted.x * rec->normalVec.x;
                if (facing < bestFacing)
                {
                    if (rec->collisionMapGameObject)
                    {
                        // preserved: the rank is written back without the null test the initialisation uses
                        *outRank = index;
                        bestFacing = (float)facing;
                        other = rec->collisionInstanceGameObject;
                        if (rec->collisionInstanceGameObject == gao)
                            other = rec->collisionMapGameObject;
                        best = other;
                    }
                }
            }
        }
        ++rec;
        ++index;
    } while (rec < end);
    return best;
}

Vec3* COL_Report_GetCollidedPoint(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    int skipZoneReports;
    int skipSlippery;
    tRecordElem* rec;
    tRecordElem* end;
    unsigned int mask;
    int flags;

    if (reportFilter < 0)
        return &world->records[(uint16_t)reportFilter].collidedPoint;
    skipZoneReports = 1;
    skipSlippery = 0;
    if ((reportFilter & 0x2000) != 0)
        skipZoneReports = 0;
    if ((reportFilter & 0x40000000) != 0)
        skipSlippery = 1;
    end = &world->records[world->recordCount];
    mask = reportFilter & 0xBFFFFFFF;
    for (rec = world->records; rec < end; ++rec)
    {
        if ((!skipZoneReports || (rec->flags & 0x1000) == 0)
            && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && (!skipZoneReports || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0
                && (!skipSlippery || (flags & 0x800) == 0)
                && (mask & flags) == mask
                && !MATH_b_NulVector(&rec->collidedPoint))
            {
                return &rec->collidedPoint;
            }
        }
    }
    return 0;
}

Vec3* COL_Report_GMat_GetCollidedPoint(struct sGameObjectMain* gao, struct sWorld* world, int gameMaterialMask, int* outRank)
{
    tRecordElem* rec;
    tRecordElem* end;
    int index;
    tCobGameMaterialElems* gameMaterial;

    rec = world->records;
    index = 0;
    end = &world->records[world->recordCount];
    if (outRank)
        *outRank = 0;
    for (; rec < end; ++rec, ++index)
    {
        if ((rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && (rec->flags & 0xF0000) == 0)
        {
            gameMaterial = rec->gameMaterial;
            if (gameMaterial && (gameMaterialMask & gameMaterial->flags) != 0)
            {
                if (outRank)
                    *outRank = index;
                return &rec->collidedPoint;
            }
        }
    }
    return &MATH_gst_NulVector;
}

int COL_Report_GetAllReportsNormal(struct sGameObjectMain* gao, struct sWorld* world, Vec3* outNormals, int reportFilter)
{
    int index;
    int count;
    Vec3* out;
    tRecordElem* rec;
    int flags;

    count = 0;
    out = outNormals;
    rec = world->records;
    for (index = 0; index < world->recordCount; ++index)
    {
        if (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
        {
            flags = rec->flags;
            if ((rec->flags & 0x80043000) == 0
                && ((reportFilter & 0x40000000) == 0 || (flags & 0x800) == 0)
                && (reportFilter & 0xBFFFFFFF & flags) == (reportFilter & 0xBFFFFFFF))
            {
                out->x = rec->normalVec.x;
                out->y = rec->normalVec.y;
                out->z = rec->normalVec.z;
                ++count;
                ++out;
            }
        }
        ++rec;
    }
    return count;
}

int COL_VisibleObjectListCreate(struct sWorld* world, int flagMustHave, int flagMustNothave, int whatToTest, struct sGameObjectMain** outList)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    int count;
    tGameObjectMain* gao;

    last = ((tableElemStructPF*)(world->visibleGameObjectsPF.nextElemPtr)) - 1;
    for (count = 0, elem = world->visibleGameObjectsPF.firstElemPtr; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr
            && ((uint8_t)gao & 3) != 1
            && gao->distance
            && OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
        {
            outList[count++] = gao;
        }
    }
    return count;
}

int COL_Report_GetNumberOfReports(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    int count;
    tRecordElem* rec;
    int remaining;
    int flags;

    count = 0;
    if (world->recordCount)
    {
        rec = world->records;
        remaining = world->recordCount;
        do
        {
            if (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            {
                flags = rec->flags;
                if ((rec->flags & 0x80043000) == 0
                    && ((reportFilter & 0x40000000) == 0 || (flags & 0x800) == 0)
                    && (reportFilter & 0xBFFFFFFF & flags) == (reportFilter & 0xBFFFFFFF)
                    && ((reportFilter & 0x40) == 0 || rec->normalVec.z >= 0.0))
                {
                    ++count;
                }
            }
            ++rec;
            --remaining;
        } while (remaining);
    }
    return count;
}

void COL_Report_ComputeResultingNormal(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter, Vec3* outNormal)
{
    int index;
    tRecordElem* rec;
    int flags;
    long double length;

    index = 0;
    if (!outNormal)
        return;
    outNormal->x = 0.0;
    outNormal->y = 0.0;
    outNormal->z = 0.0;
    if (world->recordCount)
    {
        rec = world->records;
        do
        {
            if (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            {
                flags = rec->flags;
                if ((rec->flags & 0x80043000) == 0
                    && ((reportFilter & 0x40000000) == 0 || (flags & 0x800) == 0)
                    && (reportFilter & 0xBFFFFFFF & flags) == (reportFilter & 0xBFFFFFFF)
                    && ((reportFilter & 0x40) == 0 || rec->normalVec.z >= 0.0))
                {
                    MATH_VecAddVecWithoutBuffer(outNormal, &rec->normalVec);
                }
            }
            ++index;
            ++rec;
        } while (index < world->recordCount);
    }
    if (fabs(outNormal->x) != 0.0 || fabs(outNormal->y) != 0.0 || fabs(outNormal->z) != 0.0)
    {
        length = sqrt(outNormal->x * outNormal->x + outNormal->y * outNormal->y + outNormal->z * outNormal->z);
        outNormal->x = (float)(1.0 / length * outNormal->x);
        outNormal->y = (float)(1.0 / length * outNormal->y);
        outNormal->z = (float)(1.0 / length * outNormal->z);
    }
}

struct sGameObjectMain* COL_Report_GetBestWallGOInDirectionSpecial(struct sGameObjectMain* gao, struct sWorld* world, Vec3* direction, float minCos, int* outRank)
{
    tRecordElem* rec;
    tRecordElem* end;
    int index;
    int flags;
    double facing;
    tGameObjectMain* other;
    tGameObjectMain* best;
    Vec3 wanted;
    float bestFacing;

    if (minCos == 0.0)
        bestFacing = 0.0;
    else
        bestFacing = (float)(0.001 - minCos);
    rec = world->records;
    index = 0;
    end = &world->records[world->recordCount];
    best = 0;
    if (outRank)
        *outRank = 0;
    if (rec >= end)
        return 0;
    do
    {
        if (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
        {
            flags = rec->flags;
            if ((flags & 0x80u) != 0 && (flags & 0x80072000) == 0)
            {
                if (rec->collisionInstanceGameObject == gao)
                    wanted = *direction;
                else
                    MATH_VecInvert(&wanted, direction);
                facing = rec->normalVec.z * wanted.z + rec->normalVec.y * wanted.y + wanted.x * rec->normalVec.x;
                if (facing < bestFacing)
                {
                    if (rec->collisionMapGameObject)
                    {
                        // preserved: the rank is written back without the null test the initialisation uses
                        *outRank = index;
                        bestFacing = (float)facing;
                        other = rec->collisionInstanceGameObject;
                        if (rec->collisionInstanceGameObject == gao)
                            other = rec->collisionMapGameObject;
                        best = other;
                    }
                }
            }
        }
        ++rec;
        ++index;
    } while (rec < end);
    return best;
}

int16_t COL_GameMaterial_GetID(struct sGameObjectMain* gao, struct sWorld* world, uint16_t reportFilter)
{
    tCobGameMaterialElems* gameMaterial;
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!world)
        return -1;
    if (reportFilter == 0xFFFF)
    {
        gameMaterial = world->rayCastCobGmat;
        if (gameMaterial)
            return gameMaterial->id;
        return -1;
    }
    // preserved: the filter is a uint16_t, so this is always taken and the rank arm below is dead
    if ((int)reportFilter >= 0)
    {
        end = &world->records[world->recordCount];
        for (rec = world->records; rec < end; ++rec)
        {
            if (((reportFilter & 0x2000) != 0 || (rec->flags & 0x1000) == 0)
                && rec->collisionInstanceGameObject == gao
                && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x2000) == 0))
            {
                flags = rec->flags;
                if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter && rec->gameMaterial)
                    return rec->gameMaterial->id;
            }
        }
        return -1;
    }
    gameMaterial = world->records[reportFilter].gameMaterial;
    if (!gameMaterial)
        return -1;
    return gameMaterial->id;
}

int COL_Report_GetNumberOfReportsAndRanks(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter, unsigned int maxRanks, int* outRanks)
{
    int index;
    unsigned int count;
    tRecordElem* rec;
    int flags;
    unsigned int seen;
    BOOL skipSlippery;
    unsigned int mask;

    index = 0;
    count = 0;
    skipSlippery = (reportFilter & 0x40000000) != 0;
    mask = reportFilter & ~0x40000000u;
    if (world->recordCount)
    {
        rec = world->records;
        do
        {
            if (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            {
                flags = rec->flags;
                if ((rec->flags & 0x80043000) == 0 && (!skipSlippery || (flags & 0x800) == 0) && (mask & flags) == mask)
                {
                    for (seen = 0; seen < count; ++seen)
                    {
                        if (world->records[outRanks[seen]].triangleChunkID == rec->triangleChunkID
                            && world->records[outRanks[seen]].triangleChunkElemID == rec->triangleChunkElemID)
                        {
                            break;
                        }
                    }
                    if (seen == count && ((mask & 0x40) == 0 || rec->normalVec.z >= 0.0))
                    {
                        if (count < maxRanks)
                            outRanks[count] = index;
                        ++count;
                    }
                }
            }
            ++index;
            ++rec;
        } while (index < world->recordCount);
    }
    return count;
}

int COL_Report_b_ObjectsAreColliding(struct sGameObjectMain* gao1, struct sGameObjectMain* gao2, struct sWorld* world)
{
    int recordCount;
    int index;
    tRecordElem* rec;
    tGameObjectMain* inst;

    recordCount = world->recordCount;
    if (!recordCount)
        return 0;
    rec = world->records;
    for (index = 0; index < recordCount; ++index)
    {
        if ((rec->flags & 0x80043000) == 0)
        {
            inst = rec->collisionInstanceGameObject;
            if (inst == gao1 && rec->collisionMapGameObject == gao2)
                return 1;
            if (inst == gao2 && rec->collisionMapGameObject == gao1)
                return 1;
        }
        ++rec;
    }
    return 0;
}

struct sGameObjectMain* COL_Visual_RayCast_OneObject(struct sGameObjectMain* gao, Vec3* rayOrigin, Vec3* rayDir, float rayDistance, int flagMustHave, int flagMustNothave, int whatToTest, int* outClearPath, int skipRayCast, int skipBVTest, int skipTransform)
{
    int flags;
    tGEOGRO* geo;
    tColMap* colMap;
    tColCollisionObject* firstCob;
    uint8_t cobIndex;
    tColCollisionObject* cob;
    tCobNormalObjectGeneric* shape;
    float bvDistance;

    COL_gst_Ray.ignoreColMapRayCastResult = outClearPath;
    COL_gst_Ray.sourcePoint = rayOrigin;
    COL_gst_Ray.direction = rayDir;
    COL_gst_Ray.gao = 0;
    COL_gst_Ray.distance = rayDistance;
    COL_gst_Ray.skipRayCast = skipRayCast;
    COL_gst_Ray.skipVecsTransformationToGaoSpace = skipTransform;
    if (!gao
        || ((uint8_t)gao & 3) == 1
        || !OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest)
        || (gao->flags & 0x4000) == 0 && (gao->flags & 2) == 0)
    {
        return 0;
    }
    if (!skipBVTest
        && (!OBJ_BV_IsSphere(gao->boundingVolume) || !COL_RaySphereBV(rayOrigin, rayDir, gao, &bvDistance, COL_gst_Ray.distance))
        && (!OBJ_BV_IsAABBox(gao->boundingVolume) || !COL_RayAABBox(rayOrigin, rayDir, gao, &bvDistance, COL_gst_Ray.distance)))
    {
        COL_gb_UseMinDistance = 0;
        return COL_gst_Ray.gao;
    }
    flags = gao->flags;
    if ((gao->flags & 0x4000) == 0)
    {
        if ((flags & 2) != 0 && (flags & 0x100) != 0)
        {
            colMap = gao->extended->coll->colMap;
            firstCob = *colMap->collisionObjectPtr;
            if (firstCob->objectType == 5)
            {
                if ((colMap->activation & 1) == 0)
                    return 0;
                if (((tCobMeshObjectMesh*)(firstCob->objectPtrByType))->ok3)
                    COL_ColMap_OK3_RayCast(gao, colMap);
                else
                    COL_ColMap_Triangles_RayCast(gao, colMap);
                if (COL_gst_Ray.ignoreColMapRayCastResult && !*COL_gst_Ray.ignoreColMapRayCastResult)
                    return 0;
            }
            else
            {
                for (cobIndex = 0; cobIndex < colMap->colObjectID; ++cobIndex)
                {
                    if (((uint8_t)(1 << cobIndex) & colMap->activation) != 0)
                    {
                        cob = colMap->collisionObjectPtr[cobIndex];
                        shape = (tCobNormalObjectGeneric*)(cob->objectPtrByType);
                        switch (cob->objectType)
                        {
                        case 1:
                            COL_ColMap_Box_RayCast(gao, shape);
                            break;
                        case 2:
                            COL_ColMap_Sphere_RayCast(gao, shape);
                            break;
                        case 3:
                            COL_ColMap_Cylinder_RayCast(gao, shape);
                            break;
                        }
                    }
                }
            }
        }
        COL_gb_UseMinDistance = 0;
        return COL_gst_Ray.gao;
    }
    if ((((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->objDrawMask & 8) == 0)
        return 0;
    geo = OBJ_p_GetCurrentGeo(gao);
    if (!geo)
        return 0;
    //nullsub_32();
    if (geo->collisionPtr)
        COL_Visual_OK3_RayCast(gao, geo);
    else
        COL_Visual_Triangles_RayCast(gao, geo);
    //nullsub_32();
    COL_gb_UseMinDistance = 0;
    return COL_gst_Ray.gao;
}

int COL_Report_ul_BestReportGet(struct sGameObjectMain* gao, struct sWorld* world, int flagsWanted, int flagsRejected)
{
    unsigned int recordCount;
    int bestRank;
    unsigned int index;
    tRecordElem* rec;
    int flags;
    double recalLength;
    float bestLength;

    recordCount = world->recordCount;
    bestRank = -1;
    index = 0;
    bestLength = -1.0;
    if (recordCount)
    {
        rec = world->records;
        do
        {
            flags = rec->flags;
            if ((rec->flags & 0x80001000) == 0
                && (flags & flagsWanted) != 0
                && (flags & flagsRejected) == 0
                && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao))
            {
                recalLength = rec->recalVec.z * rec->recalVec.z + rec->recalVec.x * rec->recalVec.x + rec->recalVec.y * rec->recalVec.y;
                if (recalLength > bestLength)
                {
                    bestLength = (float)recalLength;
                    bestRank = index;
                }
            }
            ++index;
            ++rec;
        } while (index < recordCount);
    }
    return bestRank;
}

BOOL COL_Report_HasGMat(struct sGameObjectMain* gao, struct sWorld* world, int reportFilter)
{
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (reportFilter < 0)
        return world->records[(uint16_t)reportFilter].gameMaterial != 0;
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if ((rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao)
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x3000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter)
                return rec->gameMaterial != 0;
        }
    }
    return 0;
}

uint8_t COL_GetDesign(struct sGameObjectMain* gao, struct sWorld* world, int32_t reportFilter)
{
    tRecordElem* rec;
    tRecordElem* end;
    int flags;

    if (!world)
        return 0;
    if (reportFilter == 0xFFFF)
        return world->rayCastCobMeshChunkElemsDesignVal;
    if ((reportFilter & 0x80000000) != 0)
        return world->records[(uint16_t)reportFilter].designVal;
    end = &world->records[world->recordCount];
    for (rec = world->records; rec < end; ++rec)
    {
        if (((reportFilter & 0x2000) != 0 || (rec->flags & 0x1000) == 0)
            && rec->collisionInstanceGameObject == gao
            && ((reportFilter & 0x2000) != 0 || (rec->flags & 0x2000) == 0))
        {
            flags = rec->flags;
            if ((flags & 0x80040000) == 0 && (reportFilter & flags) == reportFilter)
                return rec->designVal;
        }
    }
    return 0;
}
int COL_ZDE_PivotCollide(struct sGameObjectMain* gao1, uint8_t id, struct sGameObjectMain* gao2, int direction)
{
    tGameObjectMain* zoneGao;
    tGameObjectMain* pivotGao;
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    uint8_t zoneSlot;
    tColInstanceElem* elem;
    tColBox* box;
    tColSphere* sphere;
    Mtx4x4* mtx;
    double radius;
    float radiusf;
    Vec3 local;
    Vec3 scale;
    Vec3 pivot;
    Vec3 centre;
    Mtx4x4 noScale;
    Mtx4x4 inverse;

    if (!gao1 || !gao2 || (gao1->flags2 & 1) == 0 || (gao2->flags2 & 1) == 0)
        return 0;
    if (direction)
    {
        zoneGao = gao2;
        pivotGao = gao1;
    }
    else
    {
        zoneGao = gao1;
        pivotGao = gao2;
    }
    if ((zoneGao->flags & 0x600) == 0)
        return 0;
    extended = zoneGao->extended;
    if (!extended)
        return 0;
    coll = extended->coll;
    if (!coll)
        return 0;
    instance = coll->colInstance;
    if (!instance)
        return 0;
    if (!instance->colSet)
        return 0;
    zoneSlot = instance->colSet->zoneIDs[id]; // byte cast
    if (zoneSlot >= instance->zoneNum)
        return 0;
    if (!COL_i_ZoneActivationGet(instance, zoneSlot))
        return 0;
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if ((elem->colFlags & 4) == 0)
        return 0;
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        if (box->pos2.x != box->pos1.x && box->pos2.y != box->pos1.y && box->pos2.z != box->pos1.z)
        {
            mtx = OBJ_pst_GetAbsoluteMatrix(pivotGao);
            pivot.x = mtx->Pos.x;
            pivot.y = mtx->Pos.y;
            pivot.z = mtx->Pos.z;
            if ((OBJ_pst_GetAbsoluteMatrix(zoneGao)->mtxflag & 8) != 0 && (elem->colFlags & 8) != 0)
            {
                MATH_CopyMatrix(&noScale, OBJ_pst_GetAbsoluteMatrix(zoneGao));
                noScale.mtxflag &= ~8u;
                noScale._m14 = 0.0;
                noScale._m24 = 0.0;
                noScale._m34 = 0.0;
                MATH_InvertMatrix(&inverse, &noScale);
            }
            else
            {
                MATH_InvertMatrix(&inverse, OBJ_pst_GetAbsoluteMatrix(zoneGao));
            }
            MATH_TransformVertex(&local, &inverse, &pivot);
            if (local.x > (double)box->pos2.x
                && local.x < (double)box->pos1.x
                && local.y >(double)box->pos2.y
                && local.y < (double)box->pos1.y
                && local.z >(double)box->pos2.z
                && local.z < (double)box->pos1.z)
            {
                return 1;
            }
        }
    }
    else if (elem->elemType == 2 && ((tColSphere*)(elem->elemShapePtr))->radius != 0.0)
    {
        mtx = OBJ_pst_GetAbsoluteMatrix(pivotGao);
        pivot.x = mtx->Pos.x;
        pivot.y = mtx->Pos.y;
        pivot.z = mtx->Pos.z;
        if ((OBJ_pst_GetAbsoluteMatrix(zoneGao)->mtxflag & 8) == 0 || (elem->colFlags & 8) != 0)
        {
            sphere = (tColSphere*)(elem->elemShapePtr);
            MATH_TransformVertexNoScale(&centre, OBJ_pst_GetAbsoluteMatrix(zoneGao), &sphere->center);
            radius = ((tColSphere*)(elem->elemShapePtr))->radius;
        }
        else
        {
            MATH_GetScale(&scale, OBJ_pst_GetAbsoluteMatrix(zoneGao));
            MATH_TransformVertex(&centre, OBJ_pst_GetAbsoluteMatrix(zoneGao), &(((tColSphere*)(elem->elemShapePtr))->center));
            radius = MATH_f_MaxOf3(scale.x, scale.y, scale.z) * ((tColSphere*)(elem->elemShapePtr))->radius;
        }
        radiusf = (float)radius;
        local.x = pivot.x - centre.x;
        local.y = pivot.y - centre.y;
        local.z = pivot.z - centre.z;
        if (radiusf * radiusf > MATH_VecLenSquared(&local))
            return 1;
    }
    return 0;
}

void COL_Zone_Transform(tColInstanceElem* elem, tGameObjectMain* gao, Vec3* dest, Vec3* outMax)
{
    Mtx4x4* mtx;
    int hasScale;
    tColBox* box;
    double centreZ;
    long double absX;
    long double absY;
    float absZ;
    Vec3 centre;
    Vec3 worldCentre;
    Vec3 src;
    Vec3 corner;

    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    hasScale = mtx->mtxflag & 8;
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        centre.x = box->pos1.x + box->pos2.x;
        centre.y = box->pos2.y + box->pos1.y;
        centreZ = box->pos2.z + box->pos1.z;
        centre.x = (float)(centre.x * 0.5);
        centre.y = (float)(centre.y * 0.5);
        centre.z = (float)(centreZ * 0.5);
        src.x = box->pos1.x - centre.x;
        src.y = box->pos1.y - centre.y;
        src.z = box->pos1.z - centre.z;
        if (!hasScale || (elem->colFlags & 8) != 0)
        {
            MATH_TransformVertexNoScale(&worldCentre, mtx, &centre);
            MATH_TransformVectorNoScale(&corner, mtx, &src);
        }
        else
        {
            MATH_TransformVertex(&worldCentre, mtx, &centre);
            MATH_TransformVector(&corner, mtx, &src);
        }
        absX = fabs(corner.x);
        dest->x = (float)(worldCentre.x - absX);
        absY = fabs(corner.y);
        dest->y = (float)(worldCentre.y - absY);
        absZ = (float)(fabs(corner.z));
        dest->z = worldCentre.z - absZ;
        outMax->x = (float)(absX + worldCentre.x);
        outMax->y = (float)(absY + worldCentre.y);
        outMax->z = absZ + worldCentre.z;
    }
    else if (elem->elemType == 2)
    {
        if ((mtx->mtxflag & 8) == 0 || (elem->colFlags & 8) != 0)
        {
            MATH_TransformVertexNoScale(dest, mtx, &(((tColSphere*)(elem->elemShapePtr))->center));
            outMax->x = ((tColSphere*)(elem->elemShapePtr))->radius;
        }
        else
        {
            MATH_GetScale(&corner, mtx);
            MATH_TransformVertex(dest, mtx, &(((tColSphere*)(elem->elemShapePtr))->center));
            outMax->x = MATH_f_MaxOf3(corner.x, corner.y, corner.z) * ((tColSphere*)(elem->elemShapePtr))->radius;
        }
    }
}

BOOL COL_ZDE_BVCollide(struct sGameObjectMain* gao1, uint8_t id, struct sGameObjectMain* gao2, int direction)
{
    tGameObjectMain* bvGao;
    tGameObjectMain* zoneGao;
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    tColInstanceElem* elem;
    BOOL bvIsBox;
    int elemType;
    tGameObjectBV* bv;
    Vec3* pos;
    Mtx4x4* mtx;
    tColBox* box;
    uint8_t zoneSlot;
    Vec3 zoneSphereMax;
    Vec3 zoneCentre;
    Vec3 bvCentre;
    Vec3 worldZoneCentre;
    Vec3 delta;
    Vec3 worldBvCentre;
    Vec3 zoneSphereCentre;
    Vec3 zoneMax;
    Vec3 bvWorldMin;
    Vec3 bvWorldMax;
    Mtx4x4 noScale;
    Vec3 zoneMin;
    Mtx4x4 rel;
    Vec3 zoneScale;

    bvGao = gao1;
    if (!gao1 || !gao2 || (gao1->flags2 & 1) == 0 || (gao2->flags2 & 1) == 0)
        return 0;
    if (direction)
    {
        zoneGao = gao2;
    }
    else
    {
        zoneGao = gao1;
        bvGao = gao2;
    }
    if ((zoneGao->flags & 0x600) == 0)
        return 0;
    extended = zoneGao->extended;
    if (!extended)
        return 0;
    coll = extended->coll;
    if (!coll)
        return 0;
    instance = coll->colInstance;
    if (!instance)
        return 0;
    if (!instance->colSet)
        return 0;
    zoneSlot = instance->colSet->zoneIDs[id];
    if (zoneSlot >= instance->zoneNum)
        return 0;
    if (!COL_i_ZoneActivationGet(instance, zoneSlot))
        return 0;
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if ((elem->colFlags & 4) == 0)
        return 0;
    if (elem->elemType == 1)
        COL_Zone_Transform(elem, zoneGao, &zoneMin, &zoneMax);
    else if (elem->elemType == 2)
        COL_Zone_Transform(elem, zoneGao, &zoneSphereCentre, &zoneSphereMax);
    mtx = OBJ_pst_GetAbsoluteMatrix(bvGao);
    bvIsBox = !OBJ_BV_IsSphere(bvGao->boundingVolume);
    elemType = elem->elemType;
    if (!bvIsBox)
    {
        if (elemType == 2)
        {
            MATH_TransformVertexNoScale(&worldZoneCentre, mtx, &bvGao->boundingVolume->BVMin);
            return INT_SphereSphere(&zoneSphereCentre, zoneSphereMax.x, &worldZoneCentre, bvGao->boundingVolume->BVMax.x);
        }
        return 0;
    }
    if (elemType != 1)
    {
        if (elemType == 2) // elemtype == 2 ez is
        {
            mtx = bvGao->matrices;
            bv = bvGao->boundingVolume;
            pos = &mtx->Pos;
            MATH_VecAddVec(&bvWorldMax, &bv->BVMax, &mtx->Pos);
            MATH_VecAddVec(&bvWorldMin, &bv->BVMin, pos);
            return INT_SphereAABBox(&zoneSphereCentre, zoneSphereMax.x, &bvWorldMin, &bvWorldMax);
        }
        return 0;
    }
    if ((OBJ_pst_GetAbsoluteMatrix(zoneGao)->mtxflag & 8) != 0)
    {
        MATH_CopyMatrix(&noScale, OBJ_pst_GetAbsoluteMatrix(zoneGao));
        noScale._m14 = 0.0;
        noScale._m24 = 0.0;
        noScale._m34 = 0.0;
        noScale.mtxflag &= ~8u;
        MATH_InvertMatrix(&rel, &noScale);
    }
    else
    {
        MATH_InvertMatrix(&rel, OBJ_pst_GetAbsoluteMatrix(zoneGao));
    }
    box = (tColBox*)elem->elemShapePtr;
    bv = bvGao->boundingVolume;
    MATH_VecAddVec(&zoneCentre, &box->pos1, &box->pos2);
    zoneCentre.x = (float)(zoneCentre.x * 0.5);
    zoneCentre.y = (float)(zoneCentre.y * 0.5);
    zoneCentre.z = (float)(zoneCentre.z * 0.5);
    MATH_TransformVertex(&worldZoneCentre, OBJ_pst_GetAbsoluteMatrix(zoneGao), &zoneCentre);
    MATH_VecAddVec(&bvCentre, &bv->BVMax, &bv->BVMin);
    bvCentre.x = (float)(bvCentre.x * 0.5);
    bvCentre.y = (float)(bvCentre.y * 0.5);
    bvCentre.z = (float)(bvCentre.z * 0.5);
    MATH_TransformVertex(&worldBvCentre, OBJ_pst_GetAbsoluteMatrix(bvGao), &bvCentre);
    delta.x = worldBvCentre.x - worldZoneCentre.x;
    delta.y = worldBvCentre.y - worldZoneCentre.y;
    delta.z = worldBvCentre.z - worldZoneCentre.z;
    rel.Pos = delta;
    MATH_GetScale(&zoneScale, OBJ_pst_GetAbsoluteMatrix(zoneGao));
    return INT_HighLevel_OBBoxOBBox(&box->pos2, &box->pos1, &bv->BVMin, &bv->BVMax, &rel, &zoneScale);
}

int COL_ZDE_BVCenterCollide(struct sGameObjectMain* gao1, uint8_t id, struct sGameObjectMain* gao2, int direction)
{
    tGameObjectMain* zoneGao;
    tGameObjectMain* otherGao;
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    uint8_t zoneSlot;
    tColInstanceElem* elem;
    tColBox* box;
    double radius;
    Mtx4x4* mtx;
    float radiusf;
    Vec3 local;
    Vec3 scale;
    Vec3 bvCentre;
    Vec3 sphereCentre;
    Mtx4x4 noScale;
    Mtx4x4 inverse;

    if (!gao1 || !gao2 || (gao1->flags2 & 1) == 0 || (gao2->flags2 & 1) == 0)
        return 0;
    if (direction)
    {
        zoneGao = gao2;
        otherGao = gao1;
    }
    else
    {
        zoneGao = gao1;
        otherGao = gao2;
    }
    if ((zoneGao->flags & 0x600) == 0)
        return 0;
    extended = zoneGao->extended;
    if (!extended)
        return 0;
    coll = extended->coll;
    if (!coll)
        return 0;
    instance = coll->colInstance;
    if (!instance)
        return 0;
    if (!instance->colSet)
        return 0;
    zoneSlot = instance->colSet->zoneIDs[id]; // byte assign
    if (zoneSlot >= instance->zoneNum)
        return 0;
    if (!COL_i_ZoneActivationGet(instance, zoneSlot))
        return 0;
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if ((elem->colFlags & 4) == 0)
        return 0;
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        if (box->pos2.x != box->pos1.x && box->pos2.y != box->pos1.y && box->pos2.z != box->pos1.z)
        {
            OBJ_BV_ComputeCenter(otherGao, &bvCentre);
            if ((OBJ_pst_GetAbsoluteMatrix(zoneGao)->mtxflag & 8) == 0 || (elem->colFlags & 8) != 0)
            {
                MATH_InvertMatrix(&inverse, OBJ_pst_GetAbsoluteMatrix(zoneGao));
            }
            else
            {
                MATH_CopyMatrix(&noScale, OBJ_pst_GetAbsoluteMatrix(zoneGao));
                noScale.mtxflag &= ~8u;
                noScale._m14 = 0.0;
                noScale._m24 = 0.0;
                noScale._m34 = 0.0;
                MATH_InvertMatrix(&inverse, &noScale);
            }
            MATH_TransformVertex(&local, &inverse, &bvCentre);
            if (local.x > (double)box->pos2.x
                && local.x < (double)box->pos1.x
                && local.y >(double)box->pos2.y
                && local.y < (double)box->pos1.y
                && local.z >(double)box->pos2.z
                && local.z < (double)box->pos1.z)
            {
                return 1;
            }
        }
    }
    else if (elem->elemType == 2 && ((tColSphere*)(elem->elemShapePtr))->radius != 0.0)
    {
        OBJ_BV_ComputeCenter(otherGao, &bvCentre);
        if ((OBJ_pst_GetAbsoluteMatrix(zoneGao)->mtxflag & 8) == 0 || (elem->colFlags & 8) != 0)
        {
            mtx = OBJ_pst_GetAbsoluteMatrix(zoneGao);
            MATH_TransformVertexNoScale(&sphereCentre, mtx, &(((tColSphere*)(elem->elemShapePtr))->center));
            radius = ((tColSphere*)(elem->elemShapePtr))->radius;
        }
        else
        {
            MATH_GetScale(&scale, OBJ_pst_GetAbsoluteMatrix(zoneGao));
            mtx = OBJ_pst_GetAbsoluteMatrix(zoneGao);
            MATH_TransformVertex(&sphereCentre, mtx, &(((tColSphere*)(elem->elemShapePtr))->center));
            radius = MATH_f_MaxOf3(scale.x, scale.y, scale.z)
                * ((tColSphere*)(elem->elemShapePtr))->radius;
        }
        radiusf = (float)radius;
        local.x = bvCentre.x - sphereCentre.x;
        local.y = bvCentre.y - sphereCentre.y;
        local.z = bvCentre.z - sphereCentre.z;
        if (radiusf * radiusf > MATH_VecLenSquared(&local))
            return 1;
    }
    return 0;
}

BOOL COL_ZDE_ZDECollide(struct sGameObjectMain* gao1, uint8_t id1, uint8_t id2, struct sGameObjectMain* gao2)
{
    tGameObjectExtended* extended1;
    tGameObjectCollMain* coll1;
    tColInstance* instance1;
    tGameObjectExtended* extended2;
    tGameObjectCollMain* coll2;
    tColInstance* instance2;
    tColInstanceElem* elem1;
    tColInstanceElem* elem2;
    BOOL result;
    BOOL unscaled;
    tColBox* box1;
    tColBox* box2;
    Mtx4x4* mtx;
    Vec3* sphereCentre;
    float radius1;
    float radius2;
    uint8_t zoneSlot;
    Vec3 min1;
    Vec3 max1;
    Vec3 delta;
    Vec3 worldCentre2;
    Vec3 centre2;
    Vec3 centre1;
    Mtx4x4 work;
    Vec3 scale;
    Mtx4x4 rel;
    Mtx4x4 inverse1;
    Vec3 radiusVec1;
    Vec3 radiusVec2;

    if (!gao1)
        return 0;
    if (!gao2)
        return 0;
    if ((gao1->flags2 & 1) == 0)
        return 0;
    if ((gao2->flags2 & 1) == 0)
        return 0;
    if ((gao1->flags & 0x600) == 0)
        return 0;
    extended1 = gao1->extended;
    if (!extended1)
        return 0;
    coll1 = extended1->coll;
    if (!coll1)
        return 0;
    instance1 = coll1->colInstance;
    if (!instance1)
        return 0;
    if ((gao2->flags & 0x600) == 0)
        return 0;
    extended2 = gao2->extended;
    if (!extended2)
        return 0;
    coll2 = extended2->coll;
    if (!coll2)
        return 0;
    instance2 = coll2->colInstance;
    if (!instance2)
        return 0;
    if (!instance1->colSet)
        return 0;
    zoneSlot = instance1->colSet->zoneIDs[id1];
    if (zoneSlot >= instance1->zoneNum)
        return 0;
    if (!COL_i_ZoneActivationGet(instance1, zoneSlot))
        return 0;
    elem1 = instance1->selectedInstElemPtrs[zoneSlot];
    if ((elem1->colFlags & 4) == 0)
        return 0;
    zoneSlot = instance2->colSet->zoneIDs[id2];
    if (zoneSlot > instance2->zoneNum - 1)
        return 0;
    if (!COL_i_ZoneActivationGet(instance2, zoneSlot))
        return 0;
    elem2 = instance2->selectedInstElemPtrs[zoneSlot];
    if ((elem2->colFlags & 4) == 0)
        return 0;
    switch (elem2->elemType + 16 * elem1->elemType)
    {
    case 17:
        if ((OBJ_pst_GetAbsoluteMatrix(gao1)->mtxflag & 8) != 0)
        {
            MATH_CopyMatrix(&work, OBJ_pst_GetAbsoluteMatrix(gao1));
            work._m14 = 0.0;
            work._m24 = 0.0;
            work._m34 = 0.0;
            work.mtxflag &= ~8u;
            MATH_InvertMatrix(&inverse1, &work);
        }
        else
        {
            MATH_InvertMatrix(&inverse1, OBJ_pst_GetAbsoluteMatrix(gao1));
        }
        MATH_MulMatrixMatrix(&rel, OBJ_pst_GetAbsoluteMatrix(gao2), &inverse1);
        box1 = (tColBox*)elem1->elemShapePtr;
        box2 = (tColBox*)elem2->elemShapePtr;
        MATH_VecAddVec(&min1, &box1->pos1, &box1->pos2);
        min1.x = (float)(min1.x * 0.5);
        min1.y = (float)(min1.y * 0.5);
        min1.z = (float)(min1.z * 0.5);
        MATH_TransformVertex(&centre1, OBJ_pst_GetAbsoluteMatrix(gao1), &min1);
        MATH_VecAddVec(&max1, &box2->pos1, &box2->pos2);
        max1.x = (float)(max1.x * 0.5);
        max1.y = (float)(max1.y * 0.5);
        max1.z = (float)(max1.z * 0.5);
        MATH_TransformVertex(&worldCentre2, OBJ_pst_GetAbsoluteMatrix(gao2), &max1);
        centre2.x = worldCentre2.x - centre1.x;
        centre2.y = worldCentre2.y - centre1.y;
        centre2.z = worldCentre2.z - centre1.z;
        MATH_TransformVector(&delta, &inverse1, &centre2);
        rel.Pos = delta;
        MATH_GetScale(&scale, OBJ_pst_GetAbsoluteMatrix(gao1));
        result = INT_HighLevel_OBBoxOBBox(&box1->pos2, &box1->pos1, &box2->pos2, &box2->pos1, &rel, &scale);
        break;
    case 18: // v12 = 1, //v14 = 2
        if ((OBJ_pst_GetAbsoluteMatrix(gao1)->mtxflag & 8) != 0)
        {
            unscaled = (elem1->colFlags & 8) == 0;
            box1 = (tColBox*)elem1->elemShapePtr;
            if (unscaled)
            {
                MATH_GetScale(&delta, OBJ_pst_GetAbsoluteMatrix(gao1));
                min1.x = delta.x * box1->pos2.x;
                min1.y = delta.y * box1->pos2.y;
                min1.z = delta.z * box1->pos2.z;
                max1.x = delta.x * box1->pos1.x;
                max1.y = delta.y * box1->pos1.y;
                max1.z = delta.z * box1->pos1.z;
            }
            else
            {
                min1 = box1->pos2;
                max1 = box1->pos1;
            }
            mtx = OBJ_pst_GetAbsoluteMatrix(gao1);
            MATH_Transp33MatrixWithoutBuffer(&work, mtx);
            work.mtxflag = mtx->mtxflag;
            work._m14 = (float)(1.0 / mtx->_m14);
            work._m24 = (float)(1.0 / mtx->_m24);
            work._m34 = (float)(1.0 / mtx->_m34);
            MATH_VecInvert(&work.Pos, &mtx->Pos);
            MATH_TransformVectorNoScale(&work.Pos, &work, &work.Pos);
            sphereCentre = (Vec3*)elem2->elemShapePtr;
            if ((elem2->colFlags & 8) != 0)
                MATH_TransformVertexNoScale(&centre2, OBJ_pst_GetAbsoluteMatrix(gao2), sphereCentre);
            else
                MATH_TransformVertex(&centre2, OBJ_pst_GetAbsoluteMatrix(gao2), sphereCentre);
            MATH_TransformVertexNoScale(&centre1, &work, &centre2);
            unscaled = (elem2->colFlags & 8) == 0;
            radius2 = ((tColSphere*)(elem2->elemShapePtr))->radius;
            if (unscaled)
            {
                MATH_GetScale(&worldCentre2, OBJ_pst_GetAbsoluteMatrix(gao2));
                radius2 = MATH_f_MaxOf3(worldCentre2.x, worldCentre2.y, worldCentre2.z) * radius2;
            }
            result = INT_SphereOBBox(&centre1, radius2, &min1, &max1, 0);
        }
        else
        {
            MATH_InvertMatrix(&work, OBJ_pst_GetAbsoluteMatrix(gao1));
            MATH_TransformVertex(&centre2, OBJ_pst_GetAbsoluteMatrix(gao2), &(((tColSphere*)(elem2->elemShapePtr))->center));
            MATH_TransformVertex(&centre1, &work, &centre2); // v12 = 1, //v14 = 2
            result = INT_SphereOBBox(
                &centre1,
                ((tColSphere*)(elem2->elemShapePtr))->radius,
                &(((tColBox*)(elem1->elemShapePtr))->pos2),
                &(((tColBox*)(elem1->elemShapePtr))->pos1),
                0);
        }
        break;
    case 33: // v12 = 2, //v14 = 1
        if ((OBJ_pst_GetAbsoluteMatrix(gao2)->mtxflag & 8) != 0)
        {
            unscaled = (elem2->colFlags & 8) == 0;
            box2 = (tColBox*)elem2->elemShapePtr;
            if (unscaled)
            {
                MATH_GetScale(&max1, OBJ_pst_GetAbsoluteMatrix(gao2));
                delta.x = max1.x * box2->pos2.x;
                delta.y = max1.y * box2->pos2.y;
                delta.z = max1.z * box2->pos2.z;
                min1.x = max1.x * box2->pos1.x;
                min1.y = max1.y * box2->pos1.y;
                min1.z = max1.z * box2->pos1.z;
            }
            else
            {
                delta = box2->pos2;
                min1 = box2->pos1;
            }
            mtx = OBJ_pst_GetAbsoluteMatrix(gao2);
            MATH_Transp33MatrixWithoutBuffer(&work, mtx);
            work.mtxflag = mtx->mtxflag;
            work._m14 = (float)(1.0 / mtx->_m14);
            work._m24 = (float)(1.0 / mtx->_m24);
            work._m34 = (float)(1.0 / mtx->_m34);
            MATH_VecInvert(&work.Pos, &mtx->Pos);
            MATH_TransformVectorNoScale(&work.Pos, &work, &work.Pos);
            sphereCentre = &(((tColSphere*)(elem1->elemShapePtr))->center); // v12 = 2, //v14 = 1
            if ((elem1->colFlags & 8) != 0)
                MATH_TransformVertexNoScale(&centre2, OBJ_pst_GetAbsoluteMatrix(gao1), sphereCentre);
            else
                MATH_TransformVertex(&centre2, OBJ_pst_GetAbsoluteMatrix(gao1), sphereCentre);
            MATH_TransformVertexNoScale(&worldCentre2, &work, &centre2);
            unscaled = (elem1->colFlags & 8) == 0;
            radius1 = ((tColSphere*)(elem1->elemShapePtr))->radius;
            if (unscaled)
            {
                MATH_GetScale(&centre1, OBJ_pst_GetAbsoluteMatrix(gao1));
                radius1 = MATH_f_MaxOf3(centre1.x, centre1.y, centre1.z) * radius1;
            }
            result = INT_SphereOBBox(&worldCentre2, radius1, &delta, &min1, 0);
        }
        else
        {
            MATH_InvertMatrix(&work, OBJ_pst_GetAbsoluteMatrix(gao2));
            sphereCentre = &(((tColSphere*)(elem1->elemShapePtr))->center);
            if ((elem1->colFlags & 8) != 0)
                MATH_TransformVertexNoScale(&centre2, OBJ_pst_GetAbsoluteMatrix(gao1), sphereCentre);
            else
                MATH_TransformVertex(&centre2, OBJ_pst_GetAbsoluteMatrix(gao1), sphereCentre);
            MATH_TransformVertex(&worldCentre2, &work, &centre2);
            result = INT_SphereOBBox(
                &worldCentre2,
                ((tColSphere*)(elem1->elemShapePtr))->radius, // v12 = 2, //v14 = 1
                &(((tColBox*)(elem2->elemShapePtr))->pos2),
                &(((tColBox*)(elem2->elemShapePtr))->pos1),
                0);
        }
        break;
    case 34:
        COL_Zone_Transform(elem1, gao1, &centre1, &radiusVec1);
        COL_Zone_Transform(elem2, gao2, &worldCentre2, &radiusVec2);
        result = INT_SphereSphere(&centre1, radiusVec1.x, &worldCentre2, radiusVec2.x);
        break;
    default:
        return 0;
    }
    return result;
}

int COL_ListCreate(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest, char id, struct sWorld* world, struct sGameObjectMain** outList, char mode)
{
    uint8_t count;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* other;
    int hit;

    if (!gao)
        return 0;
    count = 0;
    if (mode == -1 || mode == -2 || mode == -3 || mode == -4 || mode == -5 || mode == -6 || mode == -17 || mode == -18)
    {
        last = ((tableElemStructPF*)(world->activatedObjectsTable4PF.nextElemPtr)) - 1;
        elem = world->activatedObjectsTable4PF.firstElemPtr;
    }
    else
    {
        if ((gao->flags & 0x400) == 0)
            return 0;
        last = ((tableElemStructPF*)(world->eot.collInstanceZDEGameObjectsPF.nextElemPtr)) - 1;
        elem = world->eot.collInstanceZDEGameObjectsPF.firstElemPtr;
    }
    for (; elem <= last; ++elem)
    {
        other = (tGameObjectMain*)elem->objectPtr;
        if (!elem->objectPtr
            || ((uint8_t)other & 3) == 1
            || !OBJ_b_test_flag(other, flagMustHave, flagMustNothave, whatToTest)
            || other == gao)
        {
            continue;
        }
        switch (mode)
        {
        case -1:
            hit = COL_ZDE_BVCollide(gao, id, other, 0);
            break;
        case -2:
            hit = COL_ZDE_BVCollide(gao, id, other, 1);
            break;
        case -3:
            hit = COL_BV_PivotCollide(gao, other, FALSE);
            break;
        case -4:
            hit = COL_BV_PivotCollide(gao, other, TRUE);
            break;
        case -5:
            hit = COL_ZDE_PivotCollide(gao, id, other, 0);
            break;
        case -6:
            hit = COL_ZDE_PivotCollide(gao, id, other, 1);
            break;
        case -17:
            hit = COL_ZDE_BVCenterCollide(gao, id, other, 0);
            break;
        case -18:
            hit = COL_ZDE_BVCenterCollide(gao, id, other, 1);
            break;
        default:
            hit = COL_ZDE_ZDECollide(gao, id, mode, other);
            break;
        }
        if (hit)
        {
            outList[count] = other;
            ++count;
        }
    }
    return count;
}

struct sGameObjectMain* COL_GetFirstObjectInRange(struct sWorld* world, Vec3* pos, float range, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    float dx, dy, dz;

    if (!world)
        return 0;
    last = ((tableElemStructPF*)(world->activatedObjectsTable4PF.nextElemPtr)) - 1;
    for (elem = world->activatedObjectsTable4PF.firstElemPtr; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr)
        {
            if (((uint8_t)gao & 3) != 1 && OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest)) {
                dx = pos->x - gao->matrices->Pos.x;
                dy = pos->y - gao->matrices->Pos.y;
                dz = pos->z - gao->matrices->Pos.z;
                if ((dx * dx + dy * dy + dz * dz) < (range * range)) return gao;
            }
        }
    }
    return 0;
}
int INT_FullTriangleTriangle(Vec3* t1a, Vec3* t1b, Vec3* t1c, Vec3* t2a, Vec3* t2b, Vec3* t2c, Vec3* outHit)
{
    double e1x;
    double e1y;
    double e1z;
    double e2z;
    double normalZ;
    int mask;
    double rayX;
    double rayY;
    double rayZ;
    double invLength;
    Vec3 dir;
    float e2x;
    float e2y;
    float side1;
    float side2;
    float side3;
    float length1;
    float length2;
    float length3;
    float edgeY;
    float edgeZ;
    float invLength1;
    float invLength2;
    float hitDistance;

    e1x = t1b->x - t1a->x;
    e1y = t1b->y - t1a->y;
    e1z = t1b->z - t1a->z;
    e2x = t1c->x - t1a->x;
    e2y = t1c->y - t1a->y;
    e2z = t1c->z - t1a->z;
    dir.x = (float)(e2z * e1y - e2y * e1z);
    dir.y = (float)(e1z * e2x - e2z * e1x);
    normalZ = e1x * e2y - e1y * e2x;
    side1 = (float)((t2a->z - t1a->z) * normalZ + (t2a->y - t1a->y) * dir.y + (t2a->x - t1a->x) * dir.x);
    mask = 1 << (side1 >= 0.0);
    side2 = (float)((t2b->z - t1a->z) * normalZ + (t2b->y - t1a->y) * dir.y + (t2b->x - t1a->x) * dir.x);
    mask = mask << (side2 >= 0.0);
    side3 = (float)((t2c->z - t1a->z) * normalZ + (t2c->y - t1a->y) * dir.y + (t2c->x - t1a->x) * dir.x);
    if (((mask << (side3 >= 0.0)) & 0xE) == 0)
        return 0;
    e1x = t2b->x - t2a->x;
    e1y = t2b->y - t2a->y;
    edgeY = (float)e1y;
    e1z = t2b->z - t2a->z;
    edgeZ = (float)e1z;
    e2x = t2c->x - t2a->x;
    e2y = t2c->y - t2a->y;
    e2z = t2c->z - t2a->z;
    dir.x = (float)(e2z * e1y - e2y * e1z);
    dir.y = (float)(e1z * e2x - e2z * e1x);
    normalZ = e1x * e2y - e1y * e2x;
    side1 = (float)((t1a->z - t2a->z) * normalZ + (t1a->y - t2a->y) * dir.y + (t1a->x - t2a->x) * dir.x);
    mask = 1 << (side1 >= 0.0);
    side2 = (float)((t1b->z - t2a->z) * normalZ + (t1b->y - t2a->y) * dir.y + (t1b->x - t2a->x) * dir.x);
    mask = mask << (side2 >= 0.0);
    side3 = (float)((t1c->z - t2a->z) * normalZ + (t1c->y - t2a->y) * dir.y + (t1c->x - t2a->x) * dir.x);
    if (((mask << (side3 >= 0.0)) & 0xE) == 0)
        return 0;
    rayY = edgeY;
    length1 = (float)sqrt(edgeZ * edgeZ + rayY * rayY + e1x * e1x);
    invLength = 1.0 / length1;
    invLength1 = (float)invLength;
    dir.x = (float)(invLength * e1x);
    dir.y = (float)(rayY * invLength1);
    dir.z = edgeZ * invLength1;
    if (INT_FullRayTriangle(t2a, &dir, t1a, t1b, t1c, &hitDistance, outHit))
    {
        if (hitDistance < (double)length1)
            return 1;
    }
    rayX = t2c->x - t2b->x;
    rayY = t2c->y - t2b->y;
    rayZ = t2c->z - t2b->z;
    dir.z = (float)rayZ;
    length2 = (float)sqrt(rayZ * dir.z + rayY * rayY + rayX * rayX);
    invLength = 1.0 / length2;
    invLength2 = (float)invLength;
    dir.x = (float)(invLength * rayX);
    dir.y = (float)(rayY * invLength2);
    dir.z = dir.z * invLength2;
    if (INT_FullRayTriangle(t2b, &dir, t1a, t1b, t1c, &hitDistance, outHit))
    {
        if (hitDistance < (double)length2)
            return 1;
    }
    rayX = t2c->x - t2a->x;
    rayY = t2c->y - t2a->y;
    rayZ = t2c->z - t2a->z;
    dir.z = (float)rayZ;
    length3 = (float)sqrt(rayZ * dir.z + rayY * rayY + rayX * rayX);
    invLength = 1.0 / length3;
    invLength2 = (float)invLength;
    dir.x = (float)(invLength * rayX);
    dir.y = (float)(rayY * invLength2);
    dir.z = dir.z * invLength2;
    if (!INT_FullRayTriangle(t2a, &dir, t1a, t1b, t1c, &hitDistance, outHit))
        return 0;
    if (hitDistance < (double)length3)
        return 1;
    return 0;
}

int COL_TriangleCollision(Vec3* corner1, Vec3* corner2, Vec3* corner3, Vec3* outPoint, struct sWorld* world, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    Mtx4x4* mtx;
    tGameObjectBV* bv;
    double worldMinX;
    tColMap* colMap;
    tColCollisionObject** cob;
    tColCollisionObject** cobEnd;
    tCobMeshObjectMesh* mesh;
    tCobMeshChunkElem* chunk;
    tCobMeshChunkElem* chunkEnd;
    tCobGameMaterialElems* gameMaterial;
    tCobMeshTriangleElem* tri;
    tCobMeshTriangleElem* triEnd;
    double hitX;
    double hitY;
    double hitZ;
    long double invLength;
    double facing;
    int found;
    float invLengthf;
    float bestFacing;
    Vec3 local1;
    Vec3 direction;
    float boundsMinX;
    float boundsMinY;
    float boundsMinZ;
    float hitZf;
    float hitXf;
    float worldMaxX;
    float worldMaxY;
    float worldMaxZ;
    float boundsMaxX;
    float boundsMaxY;
    float boundsMaxZ;
    float worldMinY;
    float worldMinZ;
    Vec3 local2;
    Vec3 hit;
    Vec3 local3;
    Mtx4x4 inverse;

    if (outPoint)
    {
        outPoint->x = 0.0;
        outPoint->y = 0.0;
        outPoint->z = 0.0;
    }
    found = 0;
    bestFacing = -100000.0f;
    boundsMinX = MATH_f_MinOf3(corner1->x, corner2->x, corner3->x);
    boundsMinY = MATH_f_MinOf3(corner1->y, corner2->y, corner3->y);
    boundsMinZ = MATH_f_MinOf3(corner1->z, corner2->z, corner3->z);
    boundsMaxX = MATH_f_MaxOf3(corner1->x, corner2->x, corner3->x);
    boundsMaxY = MATH_f_MaxOf3(corner1->y, corner2->y, corner3->y);
    boundsMaxZ = MATH_f_MaxOf3(corner1->z, corner2->z, corner3->z);
    last = ((tableElemStructPF*)(world->eot.collMapObjectsTablePF.nextElemPtr)) - 1;
    for (elem = world->eot.collMapObjectsTablePF.firstElemPtr; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (!OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
            continue;
        if ((gao->flags2 & 1) == 0)
            continue;
        mtx = gao->matrices;
        bv = gao->boundingVolume;
        worldMinX = mtx->Pos.x + bv->BVMin.x;
        worldMinY = mtx->Pos.y + bv->BVMin.y;
        worldMinZ = mtx->Pos.z + bv->BVMin.z;
        worldMaxX = mtx->Pos.x + bv->BVMax.x;
        worldMaxY = mtx->Pos.y + bv->BVMax.y;
        worldMaxZ = mtx->Pos.z + bv->BVMax.z;
        if (!(worldMinX <= boundsMaxX
            && worldMaxX >= (double)boundsMinX
            && worldMinY <= (double)boundsMaxY
            && worldMaxY >= (double)boundsMinY
            && worldMinZ <= (double)boundsMaxZ
            && worldMaxZ >= (double)boundsMinZ))
        {
            continue;
        }
        MATH_InvertMatrix(&inverse, mtx);
        MATH_TransformVertexNoScale(&local1, &inverse, corner1);
        MATH_TransformVertexNoScale(&local2, &inverse, corner2);
        MATH_TransformVertexNoScale(&local3, &inverse, corner3);
        direction.x = local2.x - local1.x;
        direction.y = local2.y - local1.y;
        direction.z = local2.z - local1.z;
        MATH_NormalizeEqualVector(&direction);
        colMap = gao->extended->coll->colMap;
        cob = colMap->collisionObjectPtr;
        cobEnd = &cob[colMap->colObjectID];
        for (; cob < cobEnd; ++cob)
        {
            if ((*cob)->objectType != 5)
                continue;
            mesh = (tCobMeshObjectMesh*)(*cob)->objectPtrByType;
            chunk = mesh->meshChunkElems;
            chunkEnd = &chunk[mesh->meshChunkElemNum];
            for (; chunk < chunkEnd; ++chunk)
            {
                gameMaterial = COL_pst_GMat_Get(*cob, chunk);
                if (gameMaterial && (gameMaterial->flags & 0x201000) != 0)
                    continue;
                tri = chunk->triangleElems;
                triEnd = &tri[chunk->elemNum];
                for (; tri < triEnd; ++tri)
                {
                    if (INT_FullTriangleTriangle(
                        &local1,
                        &local2,
                        &local3,
                        &mesh->vertices[tri->v1],
                        &mesh->vertices[tri->v2],
                        &mesh->vertices[tri->v3],
                        &hit))
                    {
                        hitX = hit.x - local1.x;
                        hitY = hit.y - local1.y;
                        hitZ = hit.z - local1.z;
                        hitZf = (float)hitZ;
                        invLength = 1.0 / sqrt(hitZ * hitZf + hitY * hitY + hitX * hitX);
                        invLengthf = (float)invLength;
                        hitXf = (float)(invLength * hitX);
                        facing = hitZf * invLengthf * direction.z + hitY * invLengthf * direction.y + hitXf * direction.x;
                        if (facing > bestFacing)
                        {
                            bestFacing = (float)facing;
                            found = 1;
                            if (outPoint)
                                MATH_TransformVertexNoScale(outPoint, gao->matrices, &hit);
                        }
                    }
                }
            }
        }
    }
    return found;
}

void COL_SwapToSpecific(struct sGameObjectMain* gao, uint8_t id)
{
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColInstance* instance;
    uint8_t zoneSlot;
    tColInstanceElem* shared;
    uint8_t ownElemNum;
    tColInstanceElem* oldElems;
    tColInstanceElem* newElems;
    int zoneNum;
    tColInstanceElem** slot;
    tColInstanceElem** slotEnd;
    char index;
    tColInstanceElem* own;
    void* shape;
    uint8_t design;
    int moved;

    if (id == 0xFF)
        return;
    if (!gao)
        return;
    extended = gao->extended;
    if (!extended)
        return;
    coll = extended->coll;
    if (!coll)
        return;
    instance = coll->colInstance;
    if (!instance)
        return;
    if (!instance->colSet)
        return;
    zoneSlot = instance->colSet->zoneIDs[id];
    // preserved: the bound admits a slot one past the end of selectedInstElemPtrs
    if (zoneSlot > instance->zoneNum)
        return;
    if (((1 << zoneSlot) & instance->instElemBitmask) != 0)
        return;
    shared = instance->selectedInstElemPtrs[zoneSlot];
    ownElemNum = instance->instElemNum;
    if (ownElemNum)
    {
        oldElems = instance->colInstElems;
        newElems = (tColInstanceElem*)MEM_p_Realloc(oldElems, sizeof(tColInstanceElem) * (ownElemNum + 1));
        zoneNum = instance->zoneNum;
        instance->colInstElems = newElems;
        moved = (char*)newElems - (char*)oldElems;
        slot = instance->selectedInstElemPtrs;
        slotEnd = &slot[zoneNum];
        for (index = 0; slot < slotEnd; ++index)
        {
            if (((1 << index) & instance->instElemBitmask) != 0)
                *slot = (tColInstanceElem*)((char*)*slot + moved); // TODO FIXME: ez egy igen nagy tárgya, hogy a régi + új offset különbsége alapjkán frissíti a selectedInstElemPtrs-t, megoldani másképp
            ++slot;
        }
    }
    else
    {
        instance->colInstElems = (tColInstanceElem*)MEM_p_Alloc(sizeof(tColInstanceElem));
    }
    own = &instance->colInstElems[instance->instElemNum];
    *own = *shared;
    if (shared->elemType == 1)      // aabbox
    {
        shape = MEM_p_Alloc(sizeof(tColBox));
        own->elemShapePtr = shape;
        memcpy(shape, shared->elemShapePtr, sizeof(tColBox));
        own->elemType = shared->elemType;
        own->colFlags = shared->colFlags;
        own->unk1 = shared->unk1;
        design = shared->design;
        own->design = design;
    }
    else if (shared->elemType == 2)
    {
        shape = MEM_p_Alloc(sizeof(tColSphere));     // sphere
        own->elemShapePtr = shape;
        memcpy(shape, shared->elemShapePtr, sizeof(tColSphere));
        own->elemType = shared->elemType;
        own->colFlags = shared->colFlags;
        own->unk1 = shared->unk1;
        design = shared->design;
        own->design = design;
    }
    instance->instElemBitmask |= 1 << zoneSlot;
    own->colFlags |= 0x40u;
    instance->selectedInstElemPtrs[zoneSlot] = own;
    instance->sharedZoneNum = instance->sharedZoneNum - 1;
    ++instance->instElemNum;
}

int COL_ListCreateDistance(int flagMustHave, int flagMustNothave, int whatToTest, struct sWorld* world, struct sGameObjectMain** outList, Vec3* pos, float range, char maxCount)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    uint8_t count;
    tGameObjectMain* gao;
    float dx, dy, dz;

    last = ((tableElemStructPF*)(world->activatedObjectsTable4PF.nextElemPtr)) - 1;
    for (count = 0, elem = world->activatedObjectsTable4PF.firstElemPtr; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr)
        {
            if (((uint8_t)gao & 3) != 1)
            {
                if (OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
                {
                    dx = pos->x - gao->matrices->Pos.x;
                    dy = pos->y - gao->matrices->Pos.y;
                    dz = pos->z - gao->matrices->Pos.z;

                    if ((dx * dx + dy * dy + dz * dz) < (range * range))
                    {
                        outList[count] = gao;
                        ++count;
                        if (count >= (uint8_t)maxCount)
                            count = maxCount - 1;
                    }
                }
            }
        }
    }
    return count;
}

void COL_ZDx_SpecificPosSet(struct sGameObjectMain* gao, uint8_t id, Vec3* pos)
{
    tColInstance* instance;
    int zoneSlot;
    tColInstanceElem* elem;
    tColBox* box;
    double shiftX;
    float halfX;
    float centreX;
    float centreY;
    float shiftY;
    float extentZ;
    float shiftZ;

    if ((gao->flags & 0x600) == 0)
        return;
    instance = gao->extended->coll->colInstance;
    zoneSlot = instance->colSet->zoneIDs[id];
    if ((instance->instElemBitmask & (1 << zoneSlot)) == 0)
        return;
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        extentZ = box->pos1.z - box->pos2.z;
        halfX = (float)((box->pos1.x - box->pos2.x) * 0.5);
        centreX = halfX + box->pos2.x;
        centreY = (float)((box->pos1.y - box->pos2.y) * 0.5 + box->pos2.y);
        shiftX = pos->x - centreX;
        shiftY = pos->y - centreY;
        shiftZ = (float)(pos->z - (extentZ * 0.5 + box->pos2.z));
        box->pos1.x = (float)(shiftX + box->pos1.x);
        box->pos1.y = shiftY + box->pos1.y;
        box->pos1.z = shiftZ + box->pos1.z;
        box->pos2.x = (float)(shiftX + box->pos2.x);
        box->pos2.y = shiftY + box->pos2.y;
        box->pos2.z = shiftZ + box->pos2.z;
    }
    else if (elem->elemType == 2)
    {
        ((tColSphere*)elem->elemShapePtr)->center = *pos;
    }
}

void COL_ZDx_SpecificSizeSet(struct sGameObjectMain* gao, uint8_t id, Vec3* size)
{
    tColInstance* instance;
    int zoneSlot;
    tColInstanceElem* elem;
    tColBox* box;
    double centreZ;
    float halfX;
    float centreX;
    float centreY;
    float extentZ;

    if ((gao->flags & 0x600) == 0)
        return;
    instance = gao->extended->coll->colInstance;
    zoneSlot = instance->colSet->zoneIDs[id];
    if ((instance->instElemBitmask & (1 << zoneSlot)) == 0)
        return;
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        extentZ = box->pos1.z - box->pos2.z;
        halfX = (float)((box->pos1.x - box->pos2.x) * 0.5);
        centreX = halfX + box->pos2.x;
        centreY = (float)((box->pos1.y - box->pos2.y) * 0.5 + box->pos2.y);
        centreZ = extentZ * 0.5 + box->pos2.z;
        box->pos2.x = (float)(centreX - size->x * 0.5);
        box->pos2.y = (float)(centreY - size->y * 0.5);
        box->pos2.z = (float)(centreZ - size->z * 0.5);
        box->pos1.x = (float)(size->x * 0.5 + centreX);
        box->pos1.y = (float)(size->y * 0.5 + centreY);
        box->pos1.z = (float)(size->z * 0.5 + centreZ);
    }
    else if (elem->elemType == 2)
    {
        ((tColSphere*)(elem->elemShapePtr))->radius = size->x;
    }
}

float COL_ZDx_SizeGet(struct sGameObjectMain* gao, uint8_t id)
{
    double size;
    tColInstance* instance;
    uint8_t zoneSlot;
    tColInstanceElem* elem;
    tColBox* box;
    float extentX;
    float extentY;
    float extentZ;

    size = 0.0;
    if ((gao->flags & 0x600) == 0)
        return 0.0;
    instance = gao->extended->coll->colInstance;
    zoneSlot = instance->colSet->zoneIDs[id];
    if (zoneSlot == 0xFF)
        return 0.0;
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        extentZ = box->pos1.z - box->pos2.z;
        extentY = box->pos1.y - box->pos2.y;
        extentX = box->pos1.x - box->pos2.x;
        size = MATH_f_MaxOf3(extentX, extentY, extentZ);
    }
    else if (elem->elemType == 2)
    {
        size = ((tColSphere*)(elem->elemShapePtr))->radius;
    }
    return (float)size;
}

void COL_ZDx_PosGet(struct sGameObjectMain* gao, uint8_t id, Vec3* outPos)
{
    tColInstance* instance;
    uint8_t zoneSlot;
    tColInstanceElem* elem;
    tColBox* box;

    if ((gao->flags & 0x600) == 0)
        return;
    instance = gao->extended->coll->colInstance;
    zoneSlot = instance->colSet->zoneIDs[id];
    // preserved: the unset-zone arm zeroes the result and then falls through into the lookup
    if (zoneSlot == 0xFF)
    {
        outPos->x = 0.0;
        outPos->y = 0.0;
        outPos->z = 0.0;
    }
    elem = instance->selectedInstElemPtrs[zoneSlot];
    if (elem->elemType == 1)
    {
        box = (tColBox*)elem->elemShapePtr;
        outPos->x = box->pos1.x + box->pos2.x;
        outPos->y = box->pos1.y + box->pos2.y;
        outPos->z = box->pos1.z + box->pos2.z;
        outPos->x = (float)(outPos->x * 0.5);
        outPos->y = (float)(outPos->y * 0.5);
        outPos->z = (float)(outPos->z * 0.5);
    }
    else
    {
        if (elem->elemType != 2)
            return;
        *outPos = ((tColSphere*)(elem->elemShapePtr))->center;
    }
    if ((elem->colFlags & 8) != 0)
        MATH_TransformVertexNoScale(outPos, gao->matrices, outPos);
    else
        MATH_TransformVertex(outPos, gao->matrices, outPos);
}

void COL_GeoColMap_PosSet(struct sGameObjectMain* gao, uint8_t cobIndex, Vec3* pos)
{
    tColMap* colMap;
    tColCollisionObject* cob;
    Vec3* centre;

    if (!gao)
        return;
    if ((gao->flags & 0x100) == 0)
        return;
    colMap = gao->extended->coll->colMap;
    if (colMap->colObjectID <= cobIndex)
        return;
    cob = colMap->collisionObjectPtr[cobIndex];
    if (cob->objectType == 2)
    {
        centre = &(((tColSphere*)(((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr))->center);
    }
    else
    {
        if (cob->objectType != 3)
            return;
        centre = &(((tColCylinder*)(((tCobNormalObjectGeneric*)(cob->objectPtrByType))->enclosurePtr))->center);
    }
    *centre = *pos;
}

int COL_Report_ul_GMatReportGet(struct sGameObjectMain* gao, struct sWorld* world, int gameMaterialMask)
{
    int recordCount;
    int rank;
    tRecordElem* rec;
    tCobGameMaterialElems* gameMaterial;

    recordCount = world->recordCount;
    if (!recordCount)
        return -1;
    rec = world->records;
    for (rank = 0; rank < recordCount; ++rank)
    {
        if ((rec->flags & 0x40000) == 0
            && (rec->collisionInstanceGameObject == gao || rec->collisionMapGameObject == gao))
        {
            gameMaterial = rec->gameMaterial;
            if (gameMaterial && (gameMaterialMask & gameMaterial->flags) != 0)
                return rank;
        }
        ++rec;
    }
    return -1;
}
