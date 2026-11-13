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

// preserved: BOOL is int8_t, so the returned bit is truncated away for every face whose index mod 32 is 8 or more
BOOL COL_b_TestFaceTag(tCobOK3* ok3, unsigned int face)
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
            result = (uint16_t)COL_w_TestInstanceFlag(v5, 2);
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
                                if (COL_i_ZoneActivationGet(v7, v29))
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
    if (!COL_i_ZoneActivationGet(v8, v27a))
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
                                if (COL_i_ZoneActivationGet(v7, v25))
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
    if (!COL_i_ZoneActivationGet(v6, a4ab))
        return 0;
    v12 = v6->selectedInstElemPtrs[a4ab];
    if ((v12->colFlags & 4) == 0)
        return 0;
    v13 = v9->zoneNum;
    a4ab = v9->colSet->zoneIDs[a3];
    if (a4ab > v13 - 1)
        return 0;
    if (!COL_i_ZoneActivationGet(v9, a4ab))
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
