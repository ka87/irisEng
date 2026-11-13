#include "memory.h"
#include "gameobject.h"
#include "graphicsobjects/geo.h"
#include "collision.h" // there is no ok3.h yet


int Face_Plane(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    int outcode;

    outcode = point->x > (double)boxMax->x;
    if (point->x < (double)boxMin->x)
        outcode |= 2u;
    if (point->y > (double)boxMax->y)
        outcode |= 4u;
    if (point->y < (double)boxMin->y)
        outcode |= 8u;
    if (point->z > (double)boxMax->z)
        outcode |= 0x10u;
    if (point->z < (double)boxMin->z)
        outcode |= 0x20u;
    return outcode;
}


int bevel_2D(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    int outcode;
    double centerZ;
    double halfZ;
    double negX;
    double limitXZ;
    double limitYZ;
    float extentZ;
    float extentHalfX;
    float centerX;
    float centerY;
    float px;
    float py;
    float pz;
    float halfX;
    float halfY;
    float limitXY;

    extentZ = boxMax->z - boxMin->z;
    extentHalfX = (float)((boxMax->x - boxMin->x) * 0.5);
    centerX = extentHalfX + boxMin->x;
    centerY = (float)((boxMax->y - boxMin->y) * 0.5 + boxMin->y);
    centerZ = extentZ * 0.5 + boxMin->z;
    px = point->x - centerX;
    py = point->y - centerY;
    outcode = 0;
    pz = (float)(point->z - centerZ);
    halfX = boxMax->x - centerX;
    halfY = boxMax->y - centerY;
    halfZ = boxMax->z - centerZ;
    limitXY = halfY + halfX;
    if (py + px > limitXY)
        outcode = 1;
    if (px - py > limitXY)
        outcode |= 2u;
    if (py - px > limitXY)
        outcode |= 4u;
    negX = -px;
    if (negX - py > limitXY)
        outcode |= 8u;
    limitXZ = halfZ + halfX;
    if (pz + px > limitXZ)
        outcode |= 0x10u;
    if (px - pz > limitXZ)
        outcode |= 0x20u;
    if (pz - px > limitXZ)
        outcode |= 0x40u;
    if (negX - pz > limitXZ)
        outcode |= 0x80u;
    limitYZ = halfZ + halfY;
    if (pz + py > limitYZ)
        outcode |= 0x100u;
    if (py - pz > limitYZ)
        outcode |= 0x200u;
    if (pz - py > limitYZ)
        outcode |= 0x400u;
    if (-py - pz > limitYZ)
        outcode |= 0x800u;
    return outcode;
}


int bevel_3D(Vec3* point, Vec3* boxMin, Vec3* boxMax)
{
    int outcode;
    double centerZ;
    double limit;
    double pxy;
    double pxMinusY;
    float extentZ;
    float extentHalfX;
    float centerX;
    float centerY;
    float px;
    float py;
    float pz;

    extentZ = boxMax->z - boxMin->z;
    extentHalfX = (float)((boxMax->x - boxMin->x) * 0.5);
    centerX = extentHalfX + boxMin->x;
    centerY = (float)((boxMax->y - boxMin->y) * 0.5 + boxMin->y);
    centerZ = extentZ * 0.5 + boxMin->z;
    px = point->x - centerX;
    py = point->y - centerY;
    outcode = 0;
    pz = (float)(point->z - centerZ);
    limit = boxMax->z - centerZ + boxMax->y - centerY + boxMax->x - centerX;
    if (pz + py + px > limit)
        outcode = 1;
    pxy = py + px;
    if (pxy - pz > limit)
        outcode |= 2u;
    pxMinusY = px - py;
    if (pz + pxMinusY > limit)
        outcode |= 4u;
    if (pxMinusY - pz > limit)
        outcode |= 8u;
    if (pz - px + py > limit)
        outcode |= 0x10u;
    if (py - px - pz > limit)
        outcode |= 0x20u;
    if (pz - pxy > limit)
        outcode |= 0x40u;
    if (-px - py - pz > limit)
        outcode |= 0x80u;
    return outcode;
}


int check_point(Vec3* from, Vec3* to, float t, int mask, Vec3* boxMin, Vec3* boxMax)
{
    Vec3 at;

    at.x = (to->x - from->x) * t + from->x;
    at.y = (to->y - from->y) * t + from->y;
    at.z = (to->z - from->z) * t + from->z;
    return mask & Face_Plane(&at, boxMin, boxMax);
}


BOOL check_line(Vec3* from, Vec3* to, char mask, Vec3* boxMin, Vec3* boxMax)
{
    float t;

    if ((mask & 1) != 0)
    {
        t = (from->x - boxMax->x) / (from->x - to->x);
        if (!check_point(from, to, t, 62, boxMin, boxMax))
            return 1;
    }
    if ((mask & 2) != 0)
    {
        t = (from->x - boxMin->x) / (from->x - to->x);
        if (!check_point(from, to, t, 61, boxMin, boxMax))
            return 1;
    }
    if ((mask & 4) != 0)
    {
        t = (from->y - boxMax->y) / (from->y - to->y);
        if (!check_point(from, to, t, 59, boxMin, boxMax))
            return 1;
    }
    if ((mask & 8) != 0)
    {
        t = (from->y - boxMin->y) / (from->y - to->y);
        if (!check_point(from, to, t, 55, boxMin, boxMax))
            return 1;
    }
    if ((mask & 0x10) != 0)
    {
        t = (from->z - boxMax->z) / (from->z - to->z);
        if (!check_point(from, to, t, 47, boxMin, boxMax))
            return 1;
    }
    if ((mask & 0x20) != 0)
    {
        t = (from->z - boxMin->z) / (from->z - to->z);
        if (!check_point(from, to, t, 31, boxMin, boxMax))
            return 1;
    }
    return 0;
}


unsigned int COL_OK3_GetNumberOfFacesInBox(tCobOK3AABBoxElem* box)
{
    tCobOK3AABBoxElemChunkElem* chunks;
    unsigned int total;
    int count;
    int i;

    total = 0;
    count = box->num;
    if (count > 0)
    {
        chunks = box->chunkElems;
        for (i = 0; i < count; ++i)
            total += chunks[i].num;
    }
    return total;
}


void COL_pst_OK3_GetTriangle(void* object, uint16_t chunkIdx, uint16_t triangleIdx, Vec3** p1, Vec3** p2, Vec3** p3, int type)
{
    if (type)
    {
        tCobMeshObjectMesh* mesh = (tCobMeshObjectMesh*)object;
        *p1 = &mesh->vertices[mesh->meshChunkElems[chunkIdx].triangleElems[triangleIdx].v1];
        *p2 = &mesh->vertices[mesh->meshChunkElems[chunkIdx].triangleElems[triangleIdx].v2];
        *p3 = &mesh->vertices[mesh->meshChunkElems[chunkIdx].triangleElems[triangleIdx].v3];
    }
    else
    {
        tGEOGRO* geo = (tGEOGRO*)object;
        *p1 = &geo->vertices[geo->triangleChunks[chunkIdx].triangleChunkElems[triangleIdx].v1];
        *p2 = &geo->vertices[geo->triangleChunks[chunkIdx].triangleChunkElems[triangleIdx].v2];
        *p3 = &geo->vertices[geo->triangleChunks[chunkIdx].triangleChunkElems[triangleIdx].v3];
    }
}


void COL_OK3_OptimizeBoxBV(tCobOK3AABBoxElem* box, void* object, int type)
{
    tCobOK3AABBoxElemChunkElem* chunk;
    Vec3* corner[3];
    Vec3* p;
    double minZ;
    float minX;
    float minY;
    float maxX;
    float maxY;
    float maxZ;
    unsigned int i;
    int j;
    int n;

    if (!box || !object)
        return;

    minZ = 1000000.0;
    minX = 1000000.0;
    minY = 1000000.0;
    maxX = -1000000.0;
    maxY = -1000000.0;
    maxZ = -1000000.0;
    if ((int)box->num > 0)
    {
        for (i = 0; i < box->num; ++i)
        {
            chunk = &box->chunkElems[i];
            for (j = 0; j < chunk->num; ++j)
            {
                COL_pst_OK3_GetTriangle(object, chunk->triangleChunkID, chunk->triangleChunkElemIDs[j],
                                        &corner[0], &corner[1], &corner[2], type);
                for (n = 0; n < 3; ++n)
                {
                    p = corner[n];
                    if (p->x > (double)maxX)
                        maxX = p->x;
                    if (p->x < (double)minX)
                        minX = p->x;
                    if (p->y > (double)maxY)
                        maxY = p->y;
                    if (p->y < (double)minY)
                        minY = p->y;
                    if (p->z > (double)maxZ)
                        maxZ = p->z;
                    if (minZ > p->z)
                        minZ = p->z;
                }
            }
        }
    }
    if (box->vec1.x < (double)minX)
        box->vec1.x = minX;
    if (box->vec2.x > (double)maxX)
        box->vec2.x = maxX;
    if (box->vec1.y < (double)minY)
        box->vec1.y = minY;
    if (box->vec2.y > (double)maxY)
        box->vec2.y = maxY;
    if (minZ > box->vec1.z)
        box->vec1.z = (float)minZ;
    if (box->vec2.z > (double)maxZ)
        box->vec2.z = maxZ;
}


BOOL COL_OK3_TriangleTouchBox(Vec3* p1, Vec3* p2, Vec3* p3, Vec3* boxMin, Vec3* boxMax)
{
    static const int atMaxX[4] = { 0, 0, 1, 1 };
    static const int atMaxY[4] = { 0, 1, 1, 0 };
    int face1;
    int face2;
    int face3;
    int out1;
    int out2;
    int out3;
    int d;
    float t;
    Vec3 edge1;
    Vec3 edge2;
    Vec3 normal;
    Vec3 corner;
    Vec3 opposite;
    Vec3 toP1;
    Vec3 diagonal;
    Vec3 hit;

    face1 = Face_Plane(p1, boxMin, boxMax);
    if (!face1)
        return 1;
    face2 = Face_Plane(p2, boxMin, boxMax);
    if (!face2)
        return 1;
    face3 = Face_Plane(p3, boxMin, boxMax);
    if (!face3)
        return 1;
    if ((face2 & face3 & face1) != 0)
        return 0;

    out1 = (bevel_2D(p1, boxMin, boxMax) << 8) | face1;
    out2 = (bevel_2D(p2, boxMin, boxMax) << 8) | face2;
    out3 = (bevel_2D(p3, boxMin, boxMax) << 8) | face3;
    if ((out2 & out3 & out1) != 0)
        return 0;

    out1 = (bevel_3D(p1, boxMin, boxMax) << 24) | out1;
    out2 = (bevel_3D(p2, boxMin, boxMax) << 24) | out2;
    out3 = (bevel_3D(p3, boxMin, boxMax) << 24) | out3;
    if ((out2 & out3 & out1) != 0)
        return 0;

    if ((out2 & out1) == 0 && check_line(p1, p2, out1 | out2, boxMin, boxMax))
        return 1;
    if ((out3 & out1) == 0 && check_line(p1, p3, out1 | out3, boxMin, boxMax))
        return 1;
    if ((out2 & out3) == 0 && check_line(p2, p3, out2 | out3, boxMin, boxMax))
        return 1;

    MATH_SubstractVec(&edge1, p2, p1);
    MATH_SubstractVec(&edge2, p3, p1);
    MATH_CrossProduct(&normal, &edge1, &edge2);
    for (d = 0; d < 4; ++d)
    {
        corner.x = atMaxX[d] ? boxMax->x : boxMin->x;
        corner.y = atMaxY[d] ? boxMax->y : boxMin->y;
        corner.z = boxMin->z;
        opposite.x = atMaxX[d] ? boxMin->x : boxMax->x;
        opposite.y = atMaxY[d] ? boxMin->y : boxMax->y;
        opposite.z = boxMax->z;
        MATH_SubstractVec(&toP1, p1, &corner);
        diagonal.x = opposite.x - corner.x;
        diagonal.y = opposite.y - corner.y;
        diagonal.z = opposite.z - corner.z;
        t = (normal.z * toP1.z + normal.y * toP1.y + normal.x * toP1.x)
            / (normal.z * diagonal.z + normal.x * diagonal.x + normal.y * diagonal.y);
        MATH_VecMultScalar(&hit, &diagonal, t);
        hit.x = hit.x + corner.x;
        hit.y = hit.y + corner.y;
        hit.z = hit.z + corner.z;
        if (INT_PointInTriangle(&hit, p1, p2, p3, &normal, 0))
            return 1;
    }
    return 0;
}


void COL_OK3_Optimize(tCobOK3* ok3, void* object, int type)
{
    tCobOK3AABBoxElem* box;
    tCobOK3AABBoxElemChunkElem* chunk;
    unsigned int i;
    unsigned int j;

    i = 0;
    while (i < ok3->num)
    {
        box = &ok3->aaBoxes[i];
        j = 0;
        while (j < box->num)
        {
            chunk = &box->chunkElems[j];
            if (chunk->num)
            {
                ++j;
                continue;
            }
            if (box->num == 1)
            {
                MEM_Free(box->chunkElems);
                box->chunkElems = 0;
                box->num = 0;
                break;
            }
            memcpy(chunk, &chunk[1], 8 * (box->num - j) - 8);
            --box->num;
        }
        if (box->chunkElems)
        {
            ++i;
        }
        else
        {
            memcpy(&ok3->aaBoxes[i], &ok3->aaBoxes[i + 1], 32 * (ok3->num - i - 1));
            --ok3->num;
        }
    }
    for (i = 0; i < ok3->num; ++i)
        COL_OK3_OptimizeBoxBV(&ok3->aaBoxes[i], object, type);
}


int COL_OK3_SubdivideBox(void* object, unsigned int boxIdx, int* forcedAxes, int type)
{
    tCobOK3* ok3;
    tCobOK3AABBoxElem* boxes;
    tCobOK3AABBoxElem* box;
    tCobOK3AABBoxElem* sub;
    tCobOK3AABBoxElemChunkElem* srcChunk;
    tCobOK3AABBoxElemChunkElem* dstChunk;
    tCobOK3AABBoxElemChunkElem* chunks;
    Vec3* p1;
    Vec3* p2;
    Vec3* p3;
    Vec3 lo;
    Vec3 hi;
    char* saved;
    int16_t* ids;
    double widthx1_5;
    float width;
    float depth;
    float height;
    float halfWidth;
    float halfDepth;
    float halfHeight;
    int axes;
    int subCount;
    int newNum;
    int k;
    int c;
    int t;
    int kept;
    int srcCount;
    int bytes;
    unsigned int i;
    int16_t scratch[10000];

    if (!object)
        return 0;
    ok3 = type ? ((tCobMeshObjectMesh*)object)->ok3 : ((tGEOGRO*)object)->collisionPtr;
    // preserved: the bound admits boxIdx == num, one past the end of the box array
    if (boxIdx > ok3->num)
        return 0;
    boxes = ok3->aaBoxes;
    width = boxes[boxIdx].vec2.x - boxes[boxIdx].vec1.x;
    depth = boxes[boxIdx].vec2.y - boxes[boxIdx].vec1.y;
    height = boxes[boxIdx].vec2.z - boxes[boxIdx].vec1.z;

    if (forcedAxes)
    {
        axes = *forcedAxes;
    }
    else
    {
        axes = 0;
        widthx1_5 = width * 1.5;
        if (height > widthx1_5 || depth * 1.5 < height)
            axes = height > 2.0;
        if (depth > widthx1_5 || height * 1.5 < depth)
            axes |= depth > 1.0 ? 2 : 0;
        if (depth * 1.5 < width || height * 1.5 < width)
            axes |= width > 1.0 ? 4 : 0;
        if (!axes)
            axes = (width > 1.0 ? 4 : 0) | (depth > 1.0 ? 2 : 0) | (height > 2.0);
    }

    switch (axes)
    {
    case 0:
        return 0;
    case 1:
    case 2:
    case 4:
        subCount = 2;
        break;
    case 3:
    case 5:
    case 6:
        subCount = 4;
        break;
    case 7:
        subCount = 8;
        break;
    default:
        subCount = 0;
        break;
    }

    newNum = subCount + ok3->num;
    ok3->num = newNum;
    if (newNum % 0x32 <= subCount)
    {
        ok3->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Realloc(boxes, sizeof(tCobOK3AABBoxElem) * (newNum + 50));
        ++COL_OK3_Realloc;
    }
    box = &ok3->aaBoxes[boxIdx];
    halfWidth = (float)(width * 0.5);
    halfDepth = (float)(depth * 0.5);
    halfHeight = (float)(height * 0.5);

    for (k = 0; k < subCount; ++k)
    {
        switch (axes)
        {
        case 1:
            lo.z = (k & 1) != 0 ? box->vec1.z : halfHeight + box->vec1.z;
            hi.z = (k & 1) != 0 ? box->vec2.z - halfHeight : box->vec2.z;
            lo.y = box->vec1.y;
            hi.y = box->vec2.y;
            lo.x = box->vec1.x;
            hi.x = box->vec2.x;
            break;
        case 2:
            lo.z = box->vec1.z;
            hi.z = box->vec2.z;
            lo.y = (k & 1) != 0 ? box->vec1.y : halfDepth + box->vec1.y;
            hi.y = (k & 1) != 0 ? box->vec2.y - halfDepth : box->vec2.y;
            lo.x = box->vec1.x;
            hi.x = box->vec2.x;
            break;
        case 3:
            lo.z = (k & 1) != 0 ? box->vec1.z : halfHeight + box->vec1.z;
            hi.z = (k & 1) != 0 ? box->vec2.z - halfHeight : box->vec2.z;
            lo.y = (k & 2) != 0 ? box->vec1.y : halfDepth + box->vec1.y;
            hi.y = (k & 2) != 0 ? box->vec2.y - halfDepth : box->vec2.y;
            lo.x = box->vec1.x;
            hi.x = box->vec2.x;
            break;
        case 4:
            lo.z = box->vec1.z;
            hi.z = box->vec2.z;
            lo.y = box->vec1.y;
            hi.y = box->vec2.y;
            lo.x = (k & 1) != 0 ? box->vec1.x : halfWidth + box->vec1.x;
            hi.x = (k & 1) != 0 ? box->vec2.x - halfWidth : box->vec2.x;
            break;
        case 5:
            lo.z = (k & 2) != 0 ? box->vec1.z : halfHeight + box->vec1.z;
            hi.z = (k & 2) != 0 ? box->vec2.z - halfHeight : box->vec2.z;
            lo.y = box->vec1.y;
            hi.y = box->vec2.y;
            lo.x = (k & 1) != 0 ? box->vec1.x : halfWidth + box->vec1.x;
            hi.x = (k & 1) != 0 ? box->vec2.x - halfWidth : box->vec2.x;
            break;
        case 6:
            lo.z = box->vec1.z;
            hi.z = box->vec2.z;
            lo.y = (k & 1) != 0 ? box->vec1.y : halfDepth + box->vec1.y;
            hi.y = (k & 1) != 0 ? box->vec2.y - halfDepth : box->vec2.y;
            lo.x = (k & 2) != 0 ? box->vec1.x : halfWidth + box->vec1.x;
            hi.x = (k & 2) != 0 ? box->vec2.x - halfWidth : box->vec2.x;
            break;
        case 7:
            lo.z = (k & 4) != 0 ? box->vec1.z : halfHeight + box->vec1.z;
            hi.z = (k & 4) != 0 ? box->vec2.z - halfHeight : box->vec2.z;
            lo.y = (k & 2) != 0 ? box->vec1.y : halfDepth + box->vec1.y;
            hi.y = (k & 2) != 0 ? box->vec2.y - halfDepth : box->vec2.y;
            lo.x = (k & 1) != 0 ? box->vec1.x : halfWidth + box->vec1.x;
            hi.x = (k & 1) != 0 ? box->vec2.x - halfWidth : box->vec2.x;
            break;
        default:
            break;
        }

        sub = &ok3->aaBoxes[k + ok3->num - subCount];
        sub->vec1 = lo;
        sub->vec2 = hi;
        sub->num = box->num;
        sub->chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(8 * sub->num);
        memset(sub->chunkElems, 0, 8 * sub->num);

        if ((int)box->num > 0)
        {
            for (c = 0; c < (int)box->num; ++c)
            {
                dstChunk = &sub->chunkElems[c];
                dstChunk->triangleChunkID = box->chunkElems[c].triangleChunkID;
                dstChunk->num = 0;
                srcChunk = &box->chunkElems[c];
                kept = 0;
                t = 0;
                if (srcChunk->num == 0)
                    continue;
                do
                {
                    COL_pst_OK3_GetTriangle(object, dstChunk->triangleChunkID,
                                            srcChunk->triangleChunkElemIDs[t], &p1, &p2, &p3, type);
                    if (COL_OK3_TriangleTouchBox(p1, p2, p3, &lo, &hi))
                        scratch[(uint16_t)kept++] = box->chunkElems[c].triangleChunkElemIDs[t];
                    srcChunk = &box->chunkElems[c];
                    srcCount = box->chunkElems[c].num;
                    ++t;
                } while (t < srcCount);
                if ((int16_t)kept)
                {
                    bytes = 2 * (uint16_t)kept;
                    dstChunk->num = kept;
                    ids = (int16_t*)MEM_p_Alloc(bytes);
                    dstChunk->triangleChunkElemIDs = (uint16_t*)ids;
                    memcpy(ids, scratch, bytes);
                }
            }
        }
    }

    saved = (char*)MEM_p_Alloc(32 * (ok3->num - subCount - boxIdx));
    for (i = 0; i < ok3->aaBoxes[boxIdx].num; ++i)
    {
        chunks = ok3->aaBoxes[boxIdx].chunkElems;
        if (chunks[i].triangleChunkElemIDs)
            MEM_Free(chunks[i].triangleChunkElemIDs);
    }
    MEM_Free(ok3->aaBoxes[boxIdx].chunkElems);
    memcpy(saved, &ok3->aaBoxes[boxIdx + 1], 32 * (ok3->num - subCount - boxIdx));
    memcpy(&ok3->aaBoxes[boxIdx], &ok3->aaBoxes[ok3->num - subCount], 32 * subCount);
    memcpy(&ok3->aaBoxes[subCount + boxIdx], saved, 32 * (ok3->num - subCount - boxIdx));
    MEM_Free(saved);
    // preserved: the first copy takes one box too many, and clearing the last
    // slot is what makes the duplicate it leaves there harmless
    ok3->aaBoxes[ok3->num - 1].num = 0;
    ok3->aaBoxes[ok3->num - 1].chunkElems = 0;

    return 1;
}


void COL_OK3_Build(void* object, int type, int rebuild)
{
    tGameObjectMain* gao;
    tGameObjectExtended* extended;
    tGameObjectCollMain* coll;
    tColMap* colMap;
    tColCollisionObject** collisionObject;
    tCobMeshObjectMesh* mesh;
    tCobMeshChunkElem* meshChunk;
    tGEOGRO* geo;
    tGeoTriangleChunks* geoChunk;
    tCobOK3* ok3;
    tCobOK3AABBoxElem* box;
    tCobOK3AABBoxElemChunkElem* chunk;
    tCobOK3AABBoxElemChunkElem* chunks;
    Vec3* vertex;
    Vec3 min;
    Vec3 max;
    unsigned int i;
    unsigned int j;
    unsigned int tagBytes;
    int e;
    int remaining;

    if (!object)
        return;

    if (type)
    {
        gao = (tGameObjectMain*)object;
        if ((gao->flags & 0x100) == 0)
            return;
        extended = gao->extended;
        if (!extended)
            return;
        coll = extended->coll;
        if (!coll)
            return;
        colMap = coll->colMap;
        if (!colMap)
            return;
        collisionObject = colMap->collisionObjectPtr;
        if (!collisionObject)
            return;
        if ((*collisionObject)->objectType != 5)
            return;
        (*collisionObject)->flags &= ~8u;
        mesh = (tCobMeshObjectMesh*)(*collisionObject)->objectPtrByType;

        if (rebuild && mesh->ok3)
        {
            for (i = 0; i < mesh->ok3->num; ++i)
            {
                box = &mesh->ok3->aaBoxes[i];
                if ((int)box->num > 0)
                {
                    for (j = 0; j < box->num; ++j)
                    {
                        chunks = box->chunkElems;
                        if (chunks[j].triangleChunkElemIDs)
                            MEM_Free(chunks[j].triangleChunkElemIDs);
                    }
                }
                MEM_Free(box->chunkElems);
            }
            MEM_Free(mesh->ok3->aaBoxes);
            MEM_Free(mesh->ok3->faceTagInts);
            MEM_Free(mesh->ok3);
            mesh->ok3 = 0;
        }

        if (mesh->ok3)
        {
            (*collisionObject)->flags |= 8u;
            return;
        }
        if (mesh->faceCount < 75u)
            return;

        vertex = mesh->vertices;
        min.x = vertex->x;
        min.y = vertex->y;
        min.z = vertex->z;
        max = min;
        if (mesh->vertexCount > 0)
        {
            remaining = mesh->vertexCount;
            vertex = mesh->vertices;
            do
            {
                if (vertex->x < (double)min.x)
                    min.x = vertex->x;
                if (vertex->y < (double)min.y)
                    min.y = vertex->y;
                if (vertex->z < (double)min.z)
                    min.z = vertex->z;
                if (vertex->x > (double)max.x)
                    max.x = vertex->x;
                if (vertex->y > (double)max.y)
                    max.y = vertex->y;
                if (vertex->z > (double)max.z)
                    max.z = vertex->z;
                ++vertex;
                --remaining;
            } while (remaining);
        }
        if (!(max.x - min.x >= 2.0 || max.y - min.y >= 2.0 || max.z - min.z >= 2.0))
            return;

        ok3 = (tCobOK3*)MEM_p_Alloc(0xCu);
        tagBytes = (((unsigned int)mesh->faceCount >> 3) & 0x1FFFFFFC) + 4;
        mesh->ok3 = ok3;
        mesh->ok3->faceTagInts = (int*)MEM_p_Alloc(tagBytes);
        memset(mesh->ok3->faceTagInts, 0, 4 * ((unsigned int)mesh->faceCount >> 5) + 4);
        mesh->ok3->num = 1;
        mesh->ok3->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(0x640u);
        mesh->ok3->aaBoxes->vec1 = min;
        mesh->ok3->aaBoxes->vec2 = max;
        mesh->ok3->aaBoxes->num = mesh->meshChunkElemNum;
        mesh->ok3->aaBoxes->chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(8 * mesh->meshChunkElemNum);
        meshChunk = mesh->meshChunkElems;
        chunk = mesh->ok3->aaBoxes->chunkElems;
        for (i = 0; i < mesh->meshChunkElemNum; ++i)
        {
            chunk->triangleChunkID = i;
            chunk->num = meshChunk->elemNum;
            if (meshChunk->elemNum)
            {
                chunk->triangleChunkElemIDs = (uint16_t*)MEM_p_Alloc(2 * meshChunk->elemNum);
                for (e = 0; e < meshChunk->elemNum; ++e)
                    chunk->triangleChunkElemIDs[e] = e;
            }
            else
            {
                chunk->triangleChunkElemIDs = 0;
            }
            ++meshChunk;
            ++chunk;
        }
        COL_OK3_SubdivideBox(mesh, 0, 0, 1);
        for (i = 0; i < mesh->ok3->num; ++i)
        {
            if (COL_OK3_GetNumberOfFacesInBox(&mesh->ok3->aaBoxes[i]) > 75
                && COL_OK3_SubdivideBox(mesh, i, 0, 1))
                --i;
        }
        (*collisionObject)->flags |= 8u;
        COL_OK3_Optimize(mesh->ok3, mesh, type);
        return;
    }

    geo = (tGEOGRO*)object;
    if (rebuild && geo->collisionPtr)
    {
        for (i = 0; i < geo->collisionPtr->num; ++i)
        {
            box = &geo->collisionPtr->aaBoxes[i];
            if ((int)box->num > 0)
            {
                for (j = 0; j < box->num; ++j)
                {
                    chunks = box->chunkElems;
                    if (chunks[j].triangleChunkElemIDs)
                        MEM_Free(chunks[j].triangleChunkElemIDs);
                }
            }
            MEM_Free(box->chunkElems);
        }
        MEM_Free(geo->collisionPtr->aaBoxes);
        if (geo->collisionPtr->faceTagInts)
            MEM_Free((void*)geo->collisionPtr->faceTagInts);
        MEM_Free(geo->collisionPtr);
        geo->collisionPtr = 0;
    }

    if (geo->collisionPtr || geo->vertexNum < 100)
        return;

    vertex = geo->vertices;
    min = *vertex;
    max.x = vertex->x;
    max.y = min.y;
    max.z = min.z;
    remaining = geo->vertexNum;
    vertex = geo->vertices;
    do
    {
        if (vertex->x < (double)min.x)
            min.x = vertex->x;
        if (vertex->y < (double)min.y)
            min.y = vertex->y;
        if (vertex->z < (double)min.z)
            min.z = vertex->z;
        if (vertex->x > (double)max.x)
            max.x = vertex->x;
        if (vertex->y > (double)max.y)
            max.y = vertex->y;
        if (vertex->z > (double)max.z)
            max.z = vertex->z;
        ++vertex;
        --remaining;
    } while (remaining);
    if (!(max.x - min.x >= 2.0 || max.y - min.y >= 2.0 || max.z - min.z >= 2.0))
        return;

    ok3 = (tCobOK3*)MEM_p_Alloc(0xCu);
    geo->collisionPtr = ok3;
    ok3->faceTagInts = 0;
    geo->collisionPtr->num = 1;
    geo->collisionPtr->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(0x640u);
    geo->collisionPtr->aaBoxes->vec1 = min;
    geo->collisionPtr->aaBoxes->vec2 = max;
    geo->collisionPtr->aaBoxes->num = geo->triangleChunkCount;
    geo->collisionPtr->aaBoxes->chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(8 * geo->triangleChunkCount);
    geoChunk = geo->triangleChunks;
    chunk = geo->collisionPtr->aaBoxes->chunkElems;
    for (i = 0; i < geo->triangleChunkCount; ++i)
    {
        chunk->triangleChunkID = i;
        chunk->num = geoChunk->triangleNum;
        if (geoChunk->triangleNum)
        {
            chunk->triangleChunkElemIDs = (uint16_t*)MEM_p_Alloc(2 * geoChunk->triangleNum);
            for (j = 0; j < geoChunk->triangleNum; ++j)
                chunk->triangleChunkElemIDs[j] = j;
        }
        else
        {
            chunk->triangleChunkElemIDs = 0;
        }
        ++geoChunk;
        ++chunk;
    }
    COL_OK3_SubdivideBox(geo, 0, 0, 0);
    for (i = 0; i < geo->collisionPtr->num; ++i)
    {
        if (COL_OK3_GetNumberOfFacesInBox(&geo->collisionPtr->aaBoxes[i]) > 0x64
            && COL_OK3_SubdivideBox(geo, i, 0, 0))
            --i;
    }
    COL_OK3_Optimize(geo->collisionPtr, geo, 0);
}