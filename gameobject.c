#include "math.h"
#include "memory.h"
#include "tables.h"
#include "loading.h"
#include "dynamics.h"
#include "animation.h"
#include "modifiers.h"
#include "gameobject.h"
#include "sectors.h"
#include "groups.h"
#include "world.h"
#include "sound.h"
#include "bigfiles.h"
#include "modifiers.h"
#include "time.h"
#include "graphicsobjects/geo.h"
#include "graphicsobjects/staticlod.h"
#include "graphicsobjects/gro_common.h"
#include "soft.h"
#include "dbg.h"

struct sGameObjectMain** AI_MainActor = AI_MainActorArr;

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    return gao->matrices;
}

void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* gao)
{
    tGaoMatrix* local;
    Mtx4x4* global;
    Mtx4x4* parent;

    if ((gao->flags & 0x400000) != 0)
    {
        local = gao->basic->mtx;
        if (local->refGao2)
        {
            parent = OBJ_pst_GetAbsoluteMatrix(local->refGao2);
            global = OBJ_pst_GetAbsoluteMatrix(gao);
            MATH_MulMatrixMatrix(global, (Mtx4x4*)local, parent);
        }
    }
}

void OBJ_UpdateStatusFlagRTL(tGameObjectMain* gao)
{
    int flags;
    int flags2;
    int newFlags2;

    if (gao)
        flags = gao->flags;
    else
        flags = 0; // LOWORD
    if ((flags & 0x4000) != 0 || (flags & 2) != 0)
    {
        if ((flags & 0x26) != 0 || (flags2 = gao->flags2, (flags2 & 0x40000) != 0))
            newFlags2 = gao->flags2 | 8;
        else
            newFlags2 = flags2 & ~8u;
        gao->flags2 = newFlags2;
        if ((newFlags2 & 0x80000) != 0)
            gao->flags2 = newFlags2 & ~8u;
    }
    else
    {
        gao->flags2 &= ~8u;
    }
}

int OBJ_b_HasBeenProcess(tGameObjectMain* gao, char callCount, int mask)
{
    int result;

    if ((callCount & 1) != 0)
        result = (mask & gao->process) == 0;
    else
        result = mask & gao->process;
    return result;
}

void OBJ_ResetProcess(tGameObjectMain* gao, char callCount, char mask)
{
    if ((callCount & 1) != 0)
        gao->process |= mask;
    else
        gao->process &= ~mask;
}

BOOL OBJ_BV_IsSphere(tGameObjectBV* bv)
{
    return (*((uint32_t *)&(bv->BVMax.z))) == 0xffffffff;
}

BOOL OBJ_BV_IsAABBox(tGameObjectBV* bv)
{
    return (*((uint32_t*)&(bv->BVMax.z))) != 0xffffffff;
}

void OBJ_BV_SetSphereFlag(tGameObjectBV* bv)
{
    (*((uint32_t*)&(bv->BVMax.z))) = 0xffffffff;
}

BOOL OBJ_HasNoBV(tGameObjectMain* gao)
{
    tGameObjectBV* bv;
    BOOL isBox;
    double extent;

    bv = gao->boundingVolume;
    isBox = OBJ_BV_IsAABBox(bv);
    extent = bv->BVMax.x;
    if (isBox)
    {
        if (extent != 0.0 || bv->BVMax.y != 0.0)
            return 0;
        extent = bv->BVMax.z;
    }
    return extent == 0.0;
}

void OBJ_BV_InitAABBox(tGameObjectBV* bv, Mtx4x4* mtx, Vec3* point)
{
    float z;
    float x;
    float y;
    Vec3 p;

    MATH_TransformVector(&p, mtx, point);
    z = p.z;
    x = p.x;
    y = p.y;
    bv->BVMin.z = p.z;
    bv->BVMin.x = x;
    bv->BVMin.y = y;
    bv->BVMax.z = z;
    bv->BVMax.x = x;
    bv->BVMax.y = y;
}

void OBJ_BV_InitSphereBV(tGameObjectBV* bv, Vec3* center, float radius, Vec3* scale)
{
    float z;
    float x;
    float y;

    if (scale)
    {
        x = center->x * scale->x;
        y = center->y * scale->y;
        z = center->z * scale->z;
        bv->BVMin.x = x;
        bv->BVMin.y = y;
        bv->BVMin.z = z;
    }
    else
    {
        bv->BVMin = *center;
    }
    bv->BVMax.x = radius;
    OBJ_BV_SetSphereFlag(bv);
}

void OBJ_BV_AddPointToSphereBV(tGameObjectBV* bv, Vec3* point, Vec3* scale)
{
    float dx;
    float dy;
    float pz;
    float dz;
    float dist;
    float step;
    float shift;
    float radius;

    if (scale)
    {
        dx = point->x * scale->x - bv->BVMin.x;
        dy = point->y * scale->y - bv->BVMin.y;
        pz = point->z * scale->z;
    }
    else
    {
        dx = point->x - bv->BVMin.x;
        dy = point->y - bv->BVMin.y;
        pz = point->z;
    }
    radius = bv->BVMax.x;
    dz = pz - bv->BVMin.z;
    dist = (float)sqrt(dz * dz + dx * dx + dy * dy);
    if (dist > radius)
    {
        shift = (1.0f - radius / dist) * 0.5f;
        step = dz * shift;
        bv->BVMin.x = dx * shift + bv->BVMin.x;
        bv->BVMin.y = dy * shift + bv->BVMin.y;
        bv->BVMin.z = step + bv->BVMin.z;
        bv->BVMax.x = (radius + dist) * 0.5f;
    }
}

void OBJ_BV_AddPointToAABBox(tGameObjectBV* bv, Mtx4x4* mtx, Vec3* point, int noScale)
{
    Vec3 p;

    if (noScale)
        MATH_TransformVectorNoScale(&p, mtx, point);
    else
        MATH_TransformVector(&p, mtx, point);
    if (p.x < (double)bv->BVMin.x)
        bv->BVMin.x = p.x;
    if (p.y < (double)bv->BVMin.y)
        bv->BVMin.y = p.y;
    if (p.z < (double)bv->BVMin.z)
        bv->BVMin.z = p.z;
    if (p.x > (double)bv->BVMax.x)
        bv->BVMax.x = p.x;
    if (p.y > (double)bv->BVMax.y)
        bv->BVMax.y = p.y;
    if (p.z > (double)bv->BVMax.z)
        bv->BVMax.z = p.z;
}

void OBJ_ComputeCameraBV(tGameObjectMain* gao, int asSphere)
{
    tGameObjectBV* bv;

    bv = gao->boundingVolume;
    if (asSphere)
    {
        bv->BVMin.x = 0.0;
        bv->BVMin.y = 0.0;
        bv->BVMin.z = 0.0;
        bv->BVMax.x = 0.5;
        OBJ_BV_SetSphereFlag(gao->boundingVolume);
    }
    else
    {
        bv->BVMin.x = -0.5;
        bv->BVMin.y = -0.5;
        bv->BVMin.z = -0.5;
        bv->BVMax.x = 0.5;
        bv->BVMax.y = 0.5;
        bv->BVMax.z = 0.5;
        gao->boundingVolume->BVMinOriginal = gao->boundingVolume->BVMin;
        gao->boundingVolume->BVMaxOriginal = gao->boundingVolume->BVMax;
    }
    if ((gao->flags & 0x80000) != 0)
    {
        bv->BVMinLocal.x = -0.5;
        bv->BVMinLocal.y = -0.5;
        bv->BVMinLocal.z = -0.5;
        bv->BVMaxLocal.x = 0.5;
        bv->BVMaxLocal.y = 0.5;
        bv->BVMaxLocal.z = 0.5;
    }
}

void OBJ_ComputeWaypointBV(tGameObjectMain* gao, int asSphere)
{
    tGameObjectBV* bv;

    bv = gao->boundingVolume;
    if ((gao->flags & 0x80000) != 0)
    {
        bv->BVMinLocal.x = -1.0;
        bv->BVMinLocal.y = -1.0;
        bv->BVMinLocal.z = -1.0;
        bv->BVMaxLocal.x = 1.0;
        bv->BVMaxLocal.y = 1.0;
        bv->BVMaxLocal.z = 1.0;
    }
    else if (asSphere)
    {
        bv->BVMin.x = 0.0;
        bv->BVMin.y = 0.0;
        bv->BVMin.z = 0.0;
        bv->BVMax.x = 1.0;
        OBJ_BV_SetSphereFlag(gao->boundingVolume);
    }
    else
    {
        bv->BVMin.y = -1.0;
        bv->BVMin.x = -1.0;
        bv->BVMin.z = -1.0;
        bv->BVMax.x = 1.0;
        bv->BVMax.y = 1.0;
        bv->BVMax.z = 1.0;
        bv->BVMinOriginal.x = bv->BVMin.x;
        bv->BVMinOriginal.y = bv->BVMin.y;
        bv->BVMinOriginal.z = bv->BVMin.z;
        bv->BVMaxOriginal.x = bv->BVMax.x;
        bv->BVMaxOriginal.y = bv->BVMax.y;
        bv->BVMaxOriginal.z = bv->BVMax.z;
    }
}

void OBJ_ComputeLightBV(tGameObjectMain* gao)
{
    tGameObjectBV* bv;
    tLIGHT* light;

    bv = gao->boundingVolume;
    light = (tLIGHT*)(gao->extended->light);
    if ((light->flags & 7) != 0)
    {
        if ((light->flags & 7u) <= 3)
        {
            bv->BVMin.x = 0.0;
            bv->BVMin.y = 0.0;
            bv->BVMin.z = 0.0;
            bv->BVMax.x = 0.0;
            OBJ_BV_SetSphereFlag(gao->boundingVolume);
        }
    }
    else
    {
        bv->BVMin.x = 0.0;
        bv->BVMin.y = 0.0;
        bv->BVMin.z = 0.0;
        bv->BVMax.x = light->ffar;
        OBJ_BV_SetSphereFlag(gao->boundingVolume);
    }
}

void OBJ_ComputeAABBoxFromOBBox(tGameObjectMain* gao)
{
    tGameObjectBV* bv;
    unsigned int i;
    Mtx4x4* mtx;
    Vec3 p;
    Vec3 corner;

    bv = gao->boundingVolume;
    bv->BVMin.x = 3.402822e38f;
    bv->BVMin.y = 3.402822e38f;
    bv->BVMin.z = 3.402822e38f;
    bv->BVMax.x = -3.402822e38f;
    bv->BVMax.y = -3.402822e38f;
    bv->BVMax.z = -3.402822e38f;
    for (i = 0; i < 8; ++i)
    {
        if ((i & 1) != 0)
            corner.x = bv->BVMinLocal.x;
        else
            corner.x = bv->BVMaxLocal.x;
        if ((i & 2) != 0)
            corner.y = bv->BVMinLocal.y;
        else
            corner.y = bv->BVMaxLocal.y;
        if ((i & 4) != 0)
            corner.z = bv->BVMinLocal.z;
        else
            corner.z = bv->BVMaxLocal.z;
        mtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_TransformVectorNoScale(&p, mtx, &corner);
        if (p.x < (double)bv->BVMin.x)
            bv->BVMin.x = p.x;
        if (p.y < (double)bv->BVMin.y)
            bv->BVMin.y = p.y;
        if (p.z < (double)bv->BVMin.z)
            bv->BVMin.z = p.z;
        if (p.x > (double)bv->BVMax.x)
            bv->BVMax.x = p.x;
        if (p.y > (double)bv->BVMax.y)
            bv->BVMax.y = p.y;
        if (p.z > (double)bv->BVMax.z)
            bv->BVMax.z = p.z;
    }
}

void OBJ_DuplicateBV(tGameObjectMain* src, tGameObjectMain* dest)
{
    tGameObjectBV* bv;

    bv = src->boundingVolume;
    if ((src->flags & 0x80000) != 0)
        memcpy(dest->boundingVolume, bv, sizeof(tGameObjectBV));
    else
        memcpy(dest->boundingVolume, bv, sizeof(tGameObjectBV) - (2 * sizeof(Vec3))); // kivonjuk belõle a végén lévõ két local méretét
}

void OBJ_BV_ComputeCenter(struct sGameObjectMain* gao, Vec3* center)
{
    tGameObjectBV* bv;
    float cx;
    float cy;
    Mtx4x4* mtx;
    float cz;

    bv = gao->boundingVolume;
    if (OBJ_BV_IsAABBox(bv))
    {
        center->x = bv->BVMax.x + bv->BVMin.x;
        center->y = bv->BVMax.y + bv->BVMin.y;
        center->z = bv->BVMax.z + bv->BVMin.z;
        cx = center->x * 0.5f;
        center->x = cx;
        cy = center->y * 0.5f;
        center->y = cy;
        cz = center->z * 0.5f;
        center->z = cz;
        center->x = cx + gao->matrices->Pos.x;
        center->y = cy + gao->matrices->Pos.y;
        center->z = cz + gao->matrices->Pos.z;
    }
    else
    {
        mtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_TransformVertexNoScale(center, mtx, &bv->BVMin);
    }
}

void OBJ_BV_CalcBVForTwoOBJs(tGameObjectMain* dest, tGameObjectMain* src)
{
    Mtx4x4* srcMtx;
    tGameObjectBV* srcBv;
    Mtx4x4* destMtx;
    tGameObjectBV* destBv;
    float maxX;
    float maxY;
    float maxZ;
    float minX;
    float minY;
    float minZ;

    srcMtx = src->matrices;
    srcBv = src->boundingVolume;
    maxX = srcBv->BVMax.x + srcMtx->Pos.x;
    maxY = srcBv->BVMax.y + srcMtx->Pos.y;
    maxZ = srcBv->BVMax.z + srcMtx->Pos.z;
    minX = srcBv->BVMin.x + srcMtx->Pos.x;
    minY = srcMtx->Pos.y + srcBv->BVMin.y;
    minZ = srcMtx->Pos.z + srcBv->BVMin.z;
    destMtx = dest->matrices;
    destBv = dest->boundingVolume;
    destBv->BVMax.x = maxX - destMtx->Pos.x;
    destBv->BVMax.y = maxY - destMtx->Pos.y;
    destBv->BVMax.z = maxZ - destMtx->Pos.z;
    destBv->BVMin.x = minX - destMtx->Pos.x;
    destBv->BVMin.y = minY - destMtx->Pos.y;
    destBv->BVMin.z = minZ - destMtx->Pos.z;
}

void OBJ_MergeBVs(tGameObjectMain* dest, tGameObjectMain* src)
{
    tGameObjectBV* destBv;
    tGameObjectBV* srcBv;
    Mtx4x4* srcMtx;
    Mtx4x4* destMtx;
    float maxX;
    float maxY;
    float maxZ;
    float minX;
    float minY;
    float minZ;
    float relMaxX;
    float relMaxY;
    float relMaxZ;
    float relMinX;
    float relMinY;
    float relMinZ;

    destBv = dest->boundingVolume;
    if (OBJ_BV_IsAABBox(destBv))
    {
        srcBv = src->boundingVolume;
        if (OBJ_BV_IsAABBox(srcBv))
        {
            srcMtx = src->matrices;
            maxX = srcMtx->Pos.x + srcBv->BVMax.x;
            maxY = srcBv->BVMax.y + srcMtx->Pos.y;
            maxZ = srcBv->BVMax.z + srcMtx->Pos.z;
            minX = srcMtx->Pos.x + srcBv->BVMin.x;
            minY = srcMtx->Pos.y + srcBv->BVMin.y;
            minZ = srcMtx->Pos.z + srcBv->BVMin.z;
            destMtx = dest->matrices;
            relMaxX = maxX - destMtx->Pos.x;
            relMaxY = maxY - destMtx->Pos.y;
            relMaxZ = maxZ - destMtx->Pos.z;
            relMinX = minX - destMtx->Pos.x;
            relMinY = minY - destMtx->Pos.y;
            relMinZ = minZ - destMtx->Pos.z;
            if (relMinX >= destBv->BVMin.x)
                relMinX = destBv->BVMin.x;
            destBv->BVMin.x = relMinX;
            if (relMinY >= (double)destBv->BVMin.y)
                relMinY = destBv->BVMin.y;
            destBv->BVMin.y = relMinY;
            if (relMinZ >= (double)destBv->BVMin.z)
                relMinZ = destBv->BVMin.z;
            destBv->BVMin.z = relMinZ;
            if (relMaxX <= (double)destBv->BVMax.x)
                relMaxX = destBv->BVMax.x;
            destBv->BVMax.x = relMaxX;
            if (relMaxY <= destBv->BVMax.y)
                relMaxY = destBv->BVMax.y;
            destBv->BVMax.y = relMaxY;
            if (!(relMaxZ <= (double)destBv->BVMax.z))
                destBv->BVMax.z = relMaxZ;
        }
    }
}

int GEO_MRM_GetRealNumberOfPoints(tGEOGRO* geo)
{
    tGeoMRM* mrm; // eax
    int result; // eax

    if (geo && geo->intf->id == 1 && (mrm = geo->mrm) != 0)
        result = mrm->endNum;
    else
        result = geo->vertexNum;
    return result;
}

void OBJ_ComputeGeometricObjectBV(tGameObjectMain* gao, int asSphere)
{
    tGameObjectVisualObject* visual;
    tGEOGRO* geo;
    Mtx4x4* absMtx;
    Vec3* first;
    Vec3* vertex;
    Vec3* end;
    tColMap* colMap;
    tColCollisionObject* colObject;
    tCobMeshObjectMesh* mesh;
    unsigned int vertexIdx;
    int objectIdx;
    Mtx4x4* mtx;
    tGameObjectBV* bv;
    double minX;
    double minY;
    double minZ;
    Vec3 scale;

    visual = gao->basic->actorObjectOrVisualObject;
    geo = (tGEOGRO*)visual->GROGEOOrSomethingElse;
    if (!geo->vertices)
        return;
    absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
    first = geo->vertices;
    vertex = first + 1;
    end = &first[GEO_MRM_GetRealNumberOfPoints(geo)];
    if (asSphere != 1)
    {
        mtx = OBJ_pst_GetAbsoluteMatrix(gao);
        OBJ_BV_InitAABBox(gao->boundingVolume, mtx, first);
        for (; vertex < end; ++vertex)
        {
            mtx = OBJ_pst_GetAbsoluteMatrix(gao);
            OBJ_BV_AddPointToAABBox(gao->boundingVolume, mtx, vertex, 0);
        }
        if ((gao->flags & 0x100) != 0)
        {
            colMap = gao->extended->coll->colMap;
            for (objectIdx = 0; objectIdx < colMap->colObjectID; ++objectIdx)
            {
                colObject = colMap->collisionObjectPtr[objectIdx];
                if (colObject->objectType != 5)
                    continue;
                mesh = (tCobMeshObjectMesh*)colObject->objectPtrByType;
                if (!mesh)
                    continue;
                for (vertexIdx = 0; vertexIdx < mesh->vertexCount; ++vertexIdx)
                {
                    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
                    OBJ_BV_AddPointToAABBox(gao->boundingVolume, mtx, &mesh->vertices[vertexIdx], 1);
                }
            }
        }
        bv = gao->boundingVolume;
        minX = bv->BVMin.x;
        if (bv->BVMin.x == bv->BVMax.x)
            minX = minX - 0.0099999998;
        bv->BVMin.x = (float)minX;
        minY = bv->BVMin.y;
        if (bv->BVMin.y == bv->BVMax.y)
            minY = minY - 0.0099999998;
        bv->BVMin.y = (float)minY;
        minZ = bv->BVMin.z;
        if (bv->BVMin.z == bv->BVMax.z)
            minZ = minZ - 0.0099999998;
        bv->BVMin.z = (float)minZ;
        gao->boundingVolume->BVMinOriginal = gao->boundingVolume->BVMin;
        gao->boundingVolume->BVMaxOriginal = gao->boundingVolume->BVMax;
    }
    else if ((absMtx->mtxflag & 8) != 0)
    {
        MATH_GetScale(&scale, absMtx);
        OBJ_BV_InitSphereBV(gao->boundingVolume, geo->vertices, 0.0, &scale);
        for (; vertex < end; ++vertex)
            OBJ_BV_AddPointToSphereBV(gao->boundingVolume, vertex, &scale);
    }
    else
    {
        OBJ_BV_InitSphereBV(gao->boundingVolume, first, 0.0, 0);
        for (; vertex < end; ++vertex)
            OBJ_BV_AddPointToSphereBV(gao->boundingVolume, vertex, 0);
    }
    geo->bv = gao->boundingVolume;
}

void OBJ_ComputeStaticLODObjectBV(tGameObjectMain* gao, int asSphere)
{
    tGameObjectVisualObject* visual;
    tGroGeneric* staticLod;
    tGEOGRO* geo;

    visual = gao->basic->actorObjectOrVisualObject;
    staticLod = visual->GROGEOOrSomethingElse;
    geo = ((tGEOSTATICLOD*)(visual->GROGEOOrSomethingElse))->geo[0];
    if (geo)
    {
        visual->GROGEOOrSomethingElse = (tGroGeneric*)geo;
        OBJ_ComputeGeometricObjectBV(gao, asSphere);
        ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = staticLod;
    }
}

void OBJ_ComputeAnimBV(tGameObjectMain* gao)
{
    tGameObjectBV* bv;
    Vec3* max;
    tGameObjectBasic* basic;
    tGameObjectActorObject* actor;
    tGroup* skeleton;
    tableDescStruct* table;
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* bone;

    bv = gao->boundingVolume;
    bv->BVMin.x = 0.0;
    bv->BVMin.y = 0.0;
    bv->BVMin.z = 0.0;
    max = &gao->boundingVolume->BVMax;
    max->x = 0.0;
    max->y = 0.0;
    max->z = 0.0;
    if ((gao->flags & 2) != 0)
    {
        basic = gao->basic;
        if (basic)
        {
            actor = (tGameObjectActorObject*)(basic->actorObjectOrVisualObject);
            if (actor)
            {
                skeleton = actor->skeletonGroup;
                if (skeleton)
                {
                    table = skeleton->tablePF;
                    elem = table->firstElemPtr;
                    for (last = ((tableElemStructPF*)(table->nextElemPtr)) - 1; elem <= last; ++elem)
                    {
                        bone = (tGameObjectMain*)elem->objectPtr;
                        if ((bone->flags & 0x80000) != 0 && (bone->flags2 & 0x2000000) == 0)
                            OBJ_ComputeAABBoxFromOBBox(bone);
                        OBJ_ComputeGlobalWhenHie(bone);
                        OBJ_MergeBVs(gao, bone);
                    }
                    gao->boundingVolume->BVMinOriginal = gao->boundingVolume->BVMin;
                    gao->boundingVolume->BVMaxOriginal = gao->boundingVolume->BVMax;
                }
            }
        }
    }
}

void OBJ_ComputeBV(tGameObjectMain* gao, int onlyIfEmpty)
{
    tGameObjectExtended* extended;
    tGroGeneric* gro;

    if (onlyIfEmpty == 1 || (gao->flags2 & 0x2000000) == 0)
    {
        extended = gao->extended;
        if (extended && extended->light)
        {
            if (OBJ_HasNoBV(gao) || !onlyIfEmpty)
                OBJ_ComputeLightBV(gao);
        }
        else if ((gao->flags & 2) != 0)
        {
            if (OBJ_HasNoBV(gao) || !onlyIfEmpty)
                OBJ_ComputeAnimBV(gao);
        }
        else
        {
            if ((gao->flags & 0x4000) != 0)
            {
                gro = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
                if (!gro)
                    return;
                if (!gro->intf)
                    return;
                switch (gro->intf->id)
                {
                case 1:
                    if (OBJ_HasNoBV(gao) || !onlyIfEmpty)
                        OBJ_ComputeGeometricObjectBV(gao, 0);
                    break;
                case 6:
                    if (OBJ_HasNoBV(gao) || !onlyIfEmpty)
                        OBJ_ComputeCameraBV(gao, 0);
                    break;
                case 8:
                    if (OBJ_HasNoBV(gao) || !onlyIfEmpty)
                        OBJ_ComputeStaticLODObjectBV(gao, 0);
                    break;
                }
            }
            if ((gao->flags & 0x80) != 0 && (OBJ_HasNoBV(gao) || !onlyIfEmpty))
                OBJ_ComputeWaypointBV(gao, 0);
        }
    }
}

void OBJ_GameObject_CreateExtended(tGameObjectMain* gao)
{
    gao->extended = (tGameObjectExtended*)MEM_p_Alloc(sizeof(tGameObjectExtended));
    memset(gao->extended, 0, sizeof(tGameObjectExtended));
    gao->extended->AIPriority = 127;
    gao->extended->sectorID = 0xffff;
    gao->extended->sectorElemIdx = 0xffff;
}

void OBJ_GameObject_CreateExtendedIfNot(struct sGameObjectMain* gao)
{
    if ((gao->flags & 0x2000) == 0)
    {
        OBJ_GameObject_CreateExtended(gao);
        gao->flags |= 0x2000u;
    }
}

tGaoMatrix* OBJ_pst_GetLocalMatrix(tGameObjectMain* gao)
{
    tGaoMatrix* result;

    if ((gao->flags & 0x400000) != 0)
        result = gao->basic->mtx;
    else
        result = 0;
    return result;
}

void OBJ_SightSet(struct sGameObjectMain* gao, Vec3* sight)
{
    MATH_OrientMatrix_UsingSight(gao->matrices, sight, 0);
}

void OBJ_BankingSet(struct sGameObjectMain* gao, Vec3* banking)
{
    MATH_OrientMatrix_UsingBanking(gao->matrices, 0, banking);
}

void OBJ_SightGeneralSet(struct sGameObjectMain* gao, Vec3* sight, Vec3* banking)
{
    MATH_OrientMatrix_UsingSight(gao->matrices, sight, banking);
}

void OBJ_BankingGeneralSet(struct sGameObjectMain* gao, Vec3* sight, Vec3* banking)
{
    MATH_OrientMatrix_UsingBanking(gao->matrices, sight, banking);
}

void OBJ_ComputeRecursivScale(tGameObjectMain* gao, Vec3* scale)
{
    tGameObjectMain* walk;
    tGameObjectBasic* basic;
    tGaoMatrix* local;
    Mtx4x4* absMtx;
    Vec3 step;

    walk = gao;
    scale->x = 1.0;
    scale->y = 1.0;
    scale->z = 1.0;
    if ((gao->flags & 0x400000) != 0 && (basic = gao->basic) != 0 && (local = basic->mtx) != 0 && local->refGao2)
    {
        do
        {
            local = OBJ_pst_GetLocalMatrix(walk);
            MATH_GetScale(&step, &local->mtx);
            scale->x = step.x * scale->x;
            scale->y = step.y * scale->y;
            scale->z = step.z * scale->z;
            if (walk && (basic = walk->basic) != 0 && (local = basic->mtx) != 0)
                walk = local->refGao2;
            else
                walk = 0;
            // preserved: the three guards above can leave walk null and the test below dereferences it
        } while ((walk->flags & 0x400000) != 0);
        MATH_GetScale(&step, walk->matrices);
        scale->x = step.x * scale->x;
        scale->y = step.y * scale->y;
        scale->z = step.z * scale->z;
    }
    else
    {
        absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_GetScale(scale, absMtx);
    }
}

void OBJ_SetAbsoluteMatrix(struct sGameObjectMain* gao, Mtx4x4* mtx)
{
    MATH_CopyMatrix(gao->matrices, mtx);
}

void OBJ_MakeAbsoluteMatrix(tGameObjectMain* gao, Mtx4x4* dest)
{
    MATH_CopyMatrix(dest, gao->matrices);
}

void OBJ_SightGet(tGameObjectMain* gao, Vec3* sight)
{
    Mtx4x4* mtx;

    mtx = gao->matrices;
    sight->x = -mtx->v2.x;
    sight->y = -mtx->v2.y;
    sight->z = -mtx->v2.z;
}

void OBJ_SightInitGet(struct sGameObjectMain* gao, Vec3* sight)
{
    Mtx4x4* mtx;

    if ((gao->flags & 0x10000) != 0)
    {
        mtx = gao->matrices;
        sight->x = -mtx[1].v2.x;
        sight->y = -mtx[1].v2.y;
        sight->z = -mtx[1].v2.z;
    }
}

void OBJ_BankingGet(tGameObjectMain* gao, Vec3* banking)
{
    Mtx4x4* mtx; //MATH_NormalizeVector inline
    double len;

    mtx = gao->matrices;
    len = sqrt(mtx->v3.z * mtx->v3.z + mtx->v3.y * mtx->v3.y + mtx->v3.x * mtx->v3.x);
    banking->x = (float)(1.0 / len * mtx->v3.x);
    banking->y = (float)(1.0 / len * mtx->v3.y);
    banking->z = (float)(1.0 / len * mtx->v3.z);
}

void OBJ_HorizonGet(struct sGameObjectMain* gao, Vec3* horizon)
{
    Mtx4x4* mtx;
    double len;

    mtx = gao->matrices;
    len = sqrt(mtx->v1.z * mtx->v1.z + mtx->v1.y * mtx->v1.y + mtx->v1.x * mtx->v1.x);
    horizon->x = (float)(1.0 / len * mtx->v1.x);
    horizon->y = (float)(1.0 / len * mtx->v1.y);
    horizon->z = (float)(1.0 / len * mtx->v1.z);
}

void OBJ_SetLocalPosition(struct sGameObjectMain* gao, Vec3* pos)
{
    tGaoMatrix* local;

    if ((gao->flags & 0x400000) != 0)
    {
        local = gao->basic->mtx;
        local->mtx.Pos = *pos;
        local->mtx.mtxflag |= 2u;
        OBJ_ComputeGlobalWhenHie(gao);
    }
}

void OBJ_SetAbsolutePosition(struct sGameObjectMain* gao, Vec3* pos)
{
    Mtx4x4* mtx;

    mtx = gao->matrices;
    mtx->Pos = *pos;
    mtx->mtxflag |= 2u;
}

void OBJ_SetInitialAbsolutePosition(struct sGameObjectMain* gao, Vec3* pos)
{
    Mtx4x4* mtx;
    tGameObjectBasic* basic;
    Mtx4x4* parentMtx;
    Mtx4x4 local;
    Mtx4x4 invParent;

    mtx = gao->matrices;
    if ((gao->flags & 0x10000) != 0)
        ++mtx;
    mtx->Pos = *pos;
    mtx->mtxflag |= 2u;
    if ((gao->flags & 0x400000) != 0)
    {
        basic = gao->basic;
        if (basic->mtx->refGao2)
        {
            if ((gao->flags & 0x10000) != 0)
            {
                parentMtx = OBJ_pst_GetAbsoluteMatrix(basic->mtx->refGao2);
                MATH_InvertMatrix(&invParent, parentMtx);
                MATH_MulMatrixMatrix(&local, gao->matrices + 1, &invParent);
                MATH_CopyMatrix(gao->matrices + 1, &local);
            }
        }
    }
}

void OBJ_SetInitialAbsoluteMatrix(tGameObjectMain* gao, Mtx4x4* mtx)
{
    Mtx4x4* flashMtx;
    tGaoMatrix* gaoMtx;
    Mtx4x4* parentMtx;
    Mtx4x4 local;
    Mtx4x4 invParent;

    if ((gao->flags & 0x10000) != 0)
    {
        MATH_CopyMatrix(gao->matrices + 1, mtx);
        if ((gao->flags & 0x4000000) != 0)
        {
            flashMtx = (gao->flags & 0x10000) != 0 ? &gao->matrices[2] : &gao->matrices[1];
            MATH_CopyMatrix(flashMtx, mtx);
        }
        if ((gao->flags & 0x400000) != 0)
        {
            gaoMtx = gao->basic->mtx;
            if (gaoMtx->refGao2)
            {
                if ((gao->flags & 0x10000) != 0)
                {
                    parentMtx = OBJ_pst_GetAbsoluteMatrix(gaoMtx->refGao2);
                    MATH_InvertMatrix(&invParent, parentMtx);
                    MATH_MulMatrixMatrix(&local, gao->matrices + 1, &invParent);
                    MATH_CopyMatrix(gao->matrices + 1, &local);
                }
            }
        }
    }
}

void OBJ_ComputeGlobalWithLocal(struct sGameObjectMain* gao, Mtx4x4* dest, int withRootAbsolute)
{
    tGameObjectMain* walk; // ebx
    tGameObjectBasic* basic; // eax
    tGaoMatrix* local; // eax
    Mtx4x4* rootMtx; // eax
    tGameObjectMain* parent; // [esp+Ch] [ebp-54h]
    Mtx4x4 acc; // [esp+10h] [ebp-50h] BYREF

    walk = gao;
    if ((gao->flags & 0x400000) != 0 && (basic = gao->basic) != 0 && (local = basic->mtx) != 0 && local->refGao2)
    {
        MATH_SetIdentityMatrix(dest);
        do
        {
            parent = walk->basic->mtx->refGao2;
            if (!parent)
                break;
            MATH_CopyMatrix(&acc, dest);
            MATH_MulMatrixMatrix(dest, &acc, &walk->basic->mtx->mtx);
            walk = parent;
        } while ((parent->flags & 0x400000) != 0);
        if (withRootAbsolute)
        {
            MATH_CopyMatrix(&acc, dest);
            rootMtx = OBJ_pst_GetAbsoluteMatrix(walk);
            MATH_MulMatrixMatrix(dest, &acc, rootMtx);
        }
    }
    else
    {
        MATH_CopyMatrix(dest, OBJ_pst_GetAbsoluteMatrix(gao));
    }
}

void OBJ_ComputeLocalWhenHie(struct sGameObjectMain* gao)
{
    tGaoMatrix* local; // esi
    tGameObjectMain* parent; // eax
    Mtx4x4* parentMtx; // eax
    Mtx4x4* absMtx; // eax
    unsigned int mtxflag; // eax
    Vec3 scale; // [esp+14h] [ebp-ACh] BYREF
    Mtx4x4 invParent; // [esp+20h] [ebp-A0h] BYREF
    Mtx4x4 parentGlobal; // [esp+70h] [ebp-50h] BYREF

    if ((gao->flags & 0x400000) != 0)
    {
        local = gao->basic->mtx;
        parent = local->refGao2;
        if (parent)
        {
            if ((parent->flags & 0x400000) != 0)
            {
                OBJ_ComputeGlobalWithLocal(parent, &parentGlobal, 1);
                MATH_InvertMatrix(&invParent, &parentGlobal);
            }
            else
            {
                parentMtx = OBJ_pst_GetAbsoluteMatrix(local->refGao2);
                MATH_InvertMatrix(&invParent, parentMtx);
            }
            absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
            MATH_MulMatrixMatrix(&local->mtx, absMtx, &invParent);
            if ((local->mtx.mtxflag & 8) != 0)
            {
                MATH_GetScale(&scale, &local->mtx);
                if (MATH_b_EqVectorWithEpsilon(&MATH_gst_IdentityVector, &scale, 0.001f)
                    || MATH_b_EqVectorWithEpsilon(&MATH_gst_NulVector, &scale, 0.001f))
                {
                    mtxflag = local->mtx.mtxflag & ~8u;
                    local->mtx._m14 = 0.0;
                    local->mtx._m24 = 0.0;
                    local->mtx._m34 = 0.0;
                    local->mtx.mtxflag = mtxflag;
                }
            }
            MATH_Orthonormalize(&local->mtx);
        }
    }
}

Vec3* OBJ_pst_GetPos(struct sGameObjectMain* gao)
{
    Mtx4x4* mtx;
    Vec3* result;

    mtx = gao->matrices;
    if ((gao->flags & 0x10000) != 0)
        result = &mtx[1].Pos;
    else
        result = &mtx->Pos;
    return result;
}

struct sDyn* OBJ_pst_GetDynaStruct(struct sGameObjectMain* gao)
{
    tGameObjectBasic* basic;
    tDyn* result;

    basic = gao->basic;
    if (basic)
        result = basic->dyn;
    else
        result = NULL;
    return result;
}

// TODO: ell
int OBJ_b_test_flag(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest)
{
    int flags;

    switch (whatToTest)
    {
    case 0:
        flags = gao->flags;
        if ((flags & flagMustHave) != 0 && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 1:
        flags = gao->designFlags;
        if (!(uint8_t)flags && flagMustHave == -1)
            return 1;
        flags = (uint8_t)flags;
        if ((uint8_t)(flags & flagMustHave) != 0 && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 2:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        flags = gao->extended->fields->designFlags;
        if (!flags && flagMustHave == -1)
            return 1;
        if ((flags & flagMustHave) != 0 && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 3:
        if ((gao->flags & 0x2000) == 0)
            return 0;
        flags = gao->extended->capa;
        if (!(int16_t)flags && flagMustHave == -1)
            return 1;
        flags = (uint16_t)flags;
        if ((flags & flagMustHave) != 0 && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 4:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        flags = gao->extended->fields->designDesFlags;
        if (!flags && flagMustHave == -1)
            return 1;
        if ((flags & flagMustHave) != 0 && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 5:
        flags = gao->flags2;
        if ((flags & flagMustHave) != 0 && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 10:
        flags = gao->flags;
        if ((flagMustHave & flags) == flagMustHave && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 11:
        flags = gao->designFlags;
        if (!(uint8_t)flags && flagMustHave == -1)
            return 1;
        flags = (uint8_t)flags;
        if ((uint8_t)(flagMustHave & flags) == flagMustHave && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 12:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        flags = gao->extended->fields->designFlags;
        if (!flags && flagMustHave == -1)
            return 1;
        if ((flagMustHave & flags) == flagMustHave && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 13:
        if ((gao->flags & 0x2000) == 0)
            return 0;
        flags = gao->extended->capa;
        if (!(int16_t)flags && flagMustHave == -1)
            return 1;
        flags = (uint16_t)flags;
        if ((flagMustHave & flags) == flagMustHave && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 14:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        flags = gao->extended->fields->designDesFlags;
        if (!flags && flagMustHave == -1)
            return 1;
        if ((flagMustHave & flags) == flagMustHave && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    case 15:
        flags = gao->flags2;
        if ((flagMustHave & flags) == flagMustHave && (flags & flagMustNothave) == 0)
            return 1;
        return 0;
    default:
        return 0;
    }
}

void OBJ_putInGroup(tGameObjectMain* gao, tGroup* grp)
{
    tGameObjectExtended* extended;

    extended = gao->extended;
    if (extended)
    {
        if (grp)
        {
            gao->flags |= 0x800000u;
            extended->grp = grp;
            ++grp->groupElemNum;
        }
    }
}

void OBJ_AddInGroup(struct sGroup* grp, struct sGameObjectMain* object)
{
    TAB_PFTable_AddElemToTable(grp->tablePF, object, 0);
}

struct sGameObjectMain* OBJ_pst_GroupGetByRank(struct sGroup* grp, int idx)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;

    if (!grp)
        return 0;
    elem = (tableElemStructPF *)(grp->tablePF->firstElemPtr);
    last = ((tableElemStructPF *)(grp->tablePF->nextElemPtr)) - 1;
    if (elem > last)
        return 0;
    while (idx)
    {
        ++elem;
        --idx;
        if (elem > last)
            return 0;
    }
    return (tGameObjectMain*)elem->objectPtr;
}

BOOL OBJ_b_IsChild(struct sGameObjectMain* gao)
{
    BOOL result;
    tGameObjectBasic* basic;
    tGaoMatrix* local;

    if ((gao->flags & 0x400000) != 0 && (basic = gao->basic) != 0 && (local = basic->mtx) != 0)
        result = local->refGao2 != 0;
    else
        result = 0;
    return result;
}

struct sGameObjectMain* OBJ_GameObject_Create(int flags)
{
    tGameObjectMain* gaomain;
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    tDyn* dyn;
    int bvSize;
    tGameObjectCollMain* coll;
    int flags2;

    gaomain = (tGameObjectMain*)MEM_p_Alloc(sizeof(tGameObjectMain));
    LOA_ul_FileTypeSize_37 += sizeof(tGameObjectMain);
    memset(gaomain, 0, sizeof(tGameObjectMain));
    gaomain->flags = flags;
    gaomain->boundingVolume = 0;
    gaomain->snpIndex = -1;
    gaomain->u3 = 16;
    gaomain->flags2 = 0;
    gaomain->designFlags = 0;

    if ((flags & 0x10000) != 0)
    {
        if ((flags & 0x4000000) != 0)
        {
            gaomain->matrices = MATH_Matrix_AllocArray(3);
            LOA_ul_FileTypeSize_29 += sizeof(Mtx4x4) * 3;
            MATH_SetIdentityMatrix(gaomain->matrices + 1);
            MATH_SetIdentityMatrix(gaomain->matrices + 2);
        } else {
            gaomain->matrices = MATH_Matrix_AllocArray(2);
            LOA_ul_FileTypeSize_29 += sizeof(Mtx4x4) * 2;
            MATH_SetIdentityMatrix(gaomain->matrices + 1);
        }
    } else if ((flags & 0x4000000) != 0)
    {
        gaomain->matrices = MATH_Matrix_AllocArray(2);
        LOA_ul_FileTypeSize_29 += sizeof(Mtx4x4) * 2;
        MATH_SetIdentityMatrix(gaomain->matrices + 1);
    } else {
        gaomain->matrices = MATH_Matrix_AllocArray(1);
        LOA_ul_FileTypeSize_29 += sizeof(Mtx4x4);
    }

    MATH_SetIdentityMatrix(gaomain->matrices);    //  MATH_SetIdentityMatrix
    if ((gaomain->flags & 0x1000) != 0)
    {
        gaomain->basic = (tGameObjectBasic*)MEM_p_Alloc(sizeof(tGameObjectBasic));
        LOA_ul_FileTypeSize_37 += 20;
        basic = gaomain->basic;
        basic->actorObjectOrVisualObject = 0;
        basic->dyn = 0;
        basic->mtx = 0;
        basic->multiPartData = 0;
        basic->extraMaterial = 0;
        if ((gaomain->flags & 0x4000) != 0)
        {
            basic->actorObjectOrVisualObject = (tGameObjectVisualObject*)MEM_p_Alloc(sizeof(tGameObjectVisualObject));
            LOA_ul_FileTypeSize_37 += 20;
            visual = basic->actorObjectOrVisualObject;
            visual->GROGEOOrSomethingElse = 0;
            visual->GROMAT = 0;
            visual->objDrawMask = 0;
            visual->RLIVertexColors = 0;
            visual->renderOnlyWithGFXRender = 0;
            visual->culledFlagFromCullingObject = 0;
            visual->vmilyenDistanceLightAttenuationFactor = 0;
            visual->boneDisplayOrder_zListBufferNum = 0;
            ((tGameObjectVisualObject*)(basic->actorObjectOrVisualObject))->objDrawMask = 0xffffffff;
        }
        if ((gaomain->flags & 2) != 0)
            basic->actorObjectOrVisualObject = ANI_pst_CreateGameObjectAnim();
        if ((gaomain->flags & 4) != 0)
        {
            dyn = DYN_pst_Dyna_Create();
            basic->dyn = dyn;
            DYN_Dyna_Init(dyn, 65, 1.0, 3.402822e38f, 3.402822e38f, &MATH_gst_NulVector, &gaomain->matrices->Pos);
        }
        if ((gaomain->flags & 0x400000) != 0)
        {
            gaomain->basic->mtx = (tGaoMatrix*)MEM_p_AllocAligned(sizeof(tGaoMatrix), 16);
            LOA_ul_FileTypeSize_37 += sizeof(tGaoMatrix);
            gaomain->basic->mtx->refGao2 = 0;
            MATH_SetIdentityMatrix(&gaomain->basic->mtx->mtx);
        }
        if ((gaomain->flags & 0x1000000) != 0)
            gaomain->flags |= 0x200000u;
        if ((gaomain->flags & 0x200000) != 0)
        {
            gaomain->basic->multiPartData = (tGameObjectMultiPartData*)MEM_p_Alloc(sizeof(tGameObjectMultiPartData));
            LOA_ul_FileTypeSize_37 += sizeof(tGameObjectMultiPartData);
            gaomain->basic->multiPartData->elemNum = 0;
            gaomain->basic->multiPartData->multipartGaoObjectPartsOrMatrices = 0;
        }
    }
    if ((gaomain->flags & 0x80000) != 0)
    {
        gaomain->boundingVolume = (tGameObjectBV*)MEM_p_Alloc(72u);
        LOA_ul_FileTypeSize_37 += 72;
        bvSize = 72;
    }
    else
    {
        gaomain->boundingVolume = (tGameObjectBV*)MEM_p_Alloc(48u);
        LOA_ul_FileTypeSize_37 += 48;
        bvSize = 48;
    }
    memset(gaomain->boundingVolume, 0, bvSize);
    if ((gaomain->flags & 0x2000) != 0)
    {
        OBJ_GameObject_CreateExtended(gaomain);
        if ((gaomain->flags & 0x700) != 0)
        {
            gaomain->extended->coll = (tGameObjectCollMain*)MEM_p_Alloc(sizeof(tGameObjectCollMain));
            LOA_ul_FileTypeSize_37 += sizeof(tGameObjectCollMain);
            coll = gaomain->extended->coll;
            coll->detectionList = 0;
            coll->colInstance = 0;
            coll->colMap = 0;
        }
        if ((gaomain->flags & 0x20) != 0)
        {
            flags2 = gaomain->flags2;
            if ((flags2 & 0x20) == 0)
            {
                gaomain->flags2 = flags2 | 0x20;
                COL_AllocDetectionList(gaomain);
            }
        }
        if ((gaomain->flags & 0x8000u) != 0)
        {
            gaomain->extended->messages = (tGameObjectMessages*)MEM_p_Alloc(sizeof(tGameObjectMessages));
            LOA_ul_FileTypeSize_37 += sizeof(tGameObjectMessages);
            memset(gaomain->extended->messages, 0, sizeof(tGameObjectMessages));
        }
        gaomain->extended->AIPriority = 127;
    }
    return gaomain;
}

void OBJ_ReinitMatrixStorageByFlags(tGameObjectMain* gao, Mtx4x4* mtx)
{
    int flags;
    Mtx4x4* matrices;
    Mtx4x4* flashMtx;
    Mtx4x4 saved;

    flags = gao->flags;
    if ((gao->flags & 0x4000000) != 0)
    {
        matrices = gao->matrices;
        if ((flags & 0x10000) != 0)
            flashMtx = matrices + 2;
        else
            flashMtx = matrices + 1;
        MATH_CopyMatrix(flashMtx, mtx);
    }
    else
    {
        flags |= 0x4000000;
        gao->flags = flags;
        MATH_CopyMatrix(&saved, mtx);
        if ((flags & 0x10000) != 0)
        {
            matrices = MATH_Matrix_ReallocArray(gao->matrices, 3);
            flashMtx = matrices + 2;
        }
        else
        {
            matrices = MATH_Matrix_ReallocArray(gao->matrices, 2);
            flashMtx = matrices + 1;
        }
        gao->matrices = matrices;
        MATH_CopyMatrix(flashMtx, &saved);
    }
}

void OBJ_RestoreInitialPos(struct sGameObjectMain* gao)
{
    int flags;
    tGameObjectBasic* basic;
    tGaoMatrix* local;
    Mtx4x4* absMtx;
    Mtx4x4* matrices;
    tGaoMatrix* sourceMtx;
    Mtx4x4* parentMtx;

    flags = gao->flags;
    if ((gao->flags & 0x400000) != 0 && (basic = gao->basic) != 0 && (local = basic->mtx) != 0 && local->refGao)
    {
        if ((flags & 0x10000) != 0)
            MATH_CopyMatrix(&local->mtx, gao->matrices + 1);
        parentMtx = OBJ_pst_GetAbsoluteMatrix(gao->basic->mtx->refGao);
        absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_MulMatrixMatrix(absMtx, (Mtx4x4*)(gao->basic->mtx), parentMtx); //FIXME a1->basic->mtx nem biztos hogy jó, kidebuggolni
    }
    else if ((flags & 0x10000) != 0)
    {
        matrices = gao->matrices;
        MATH_CopyMatrix(matrices, &matrices[1]);
    }
    if ((gao->flags & 2) != 0)
    {
        if ((gao->flags & 0x400000) != 0)
            sourceMtx = OBJ_pst_GetLocalMatrix(gao);
        else
            sourceMtx = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(gao);
        OBJ_ReinitMatrixStorageByFlags(gao, &sourceMtx->mtx);
    }
}

void OBJ_Reinit(struct sGameObjectMain* gao)
{
    tGaoMatrix* local;
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    tGroGeneric* gro;
    tGroGeneric* grm;

    if ((gao->flags & 1) == 0 && (gao->flags & 0x400000) != 0)
    {
        local = gao->basic->mtx;
        if (local->refGao2)
            OBJ_Reinit(local->refGao2);
    }
    OBJ_RestoreInitialPos(gao);
    basic = gao->basic;
    if (basic)
    {
        basic->extraMaterial = 0;
        if ((gao->flags & 0x4000) == 0) {
            MDF_Reinit(gao);
            return;
        }
        visual = gao->basic->actorObjectOrVisualObject;
        if (visual)
            visual->objDrawMask |= 0x60100000u;
    }
    if ((gao->flags & 0x4000) != 0)
    {
        gro = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
        if (gro && gro->intf)
            gro->intf->Reinit(gro);
        grm = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
        if (grm && grm->intf)
            grm->intf->Reinit(grm);
    }

    MDF_Reinit(gao);
}

void OBJ_CheckFlagsAndActivateObject(tGameObjectMain* gao, int newFlags2)
{
    tWorld* world;
    int flags2;

    if (gao)
    {
        world = WOR_gpst_CurrentWorld;
        gao->flags2 &= 0x0000ffff;
        flags2 = newFlags2 & 0xFFFF0000 | gao->flags2;
        gao->flags2 = flags2;
        if ((newFlags2 & 0x8000000) != 0 && (flags2 & 1) == 0)
            WOR_ActivateObjectIfFlagsAllow(gao, world);
        if ((newFlags2 & 0x400000) != 0)
            world->tablesManagerAndEOTNeeded = 1;
        if ((newFlags2 & 0x100000) != 0 && (gao->flags2 & 0x20) == 0)
        {
            COL_AllocDetectionList(gao);
            gao->flags2 |= 0x20u;
        }
        if ((newFlags2 & 0x100000) == 0 && (gao->flags2 & 0x20) != 0 && (gao->flags & 0x20) == 0)
        {
            COL_FreeDetectionList(gao->extended->coll);
            gao->extended->coll->detectionList = 0;
            gao->flags2 &= ~0x20u;
        }
        OBJ_UpdateStatusFlagRTL(gao);
    }
}

void OBJ_SetProcess(tGameObjectMain* gao, char callCount, char mask)
{
    if ((callCount & 1) != 0)
        gao->process &= ~mask;
    else
        gao->process |= mask;
}

void OBJ_FreeGroupGrl(tableDescStruct* table)
{
    if (table)
    {
        TAB_PFtable_Close(table);
        LOA_DeleteAddress((int)table);
        MEM_Free(table);
    }
}

void OBJ_FreeGroup(tGroup* grp)
{
    OBJ_FreeGroupGrl(grp->tablePF);
    MEM_Free(grp);
    LOA_DeleteAddress((int)grp);
}

void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown)
{
    tGroGeneric* gro;
    tGroGeneric* grm;
    tGameObjectVisualObject* visual;
    tGameObjectMultiPartData* multiPart;
    tGroGeneric* light;
    tModifier* mdf;
    tModifier* next;

    GRPSectDelObj(gao);
    if ((gao->flags & 8) == 0)
        MATH_Matrix_Free(gao->matrices);
    if ((gao->flags & 0x1000) != 0)
    {
        if ((gao->flags & 0x4000) != 0)
        {
            gro = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
            if (gro)
            {
                if (refCountDown)
                    gro->intf->AddRef(gro, -1);
                gro->intf->Destroy(gro);
            }
            grm = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
            if (grm)
            {
                if (refCountDown)
                    grm->intf->AddRef(grm, -1);
                grm->intf->Destroy(grm);
            }
            visual = gao->basic->actorObjectOrVisualObject;
            if (visual->RLIVertexColors)
                MEM_Free(visual->RLIVertexColors);
            visual->RLIVertexColors = 0;
            MEM_Free(gao->basic->actorObjectOrVisualObject);
        }
        if ((gao->flags & 2) != 0)
            ANI_Free((tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject);
        if ((gao->flags & 4) != 0)
            DYN_Dyna_Close(gao->basic->dyn);
        if ((gao->flags & 8) == 0 && (gao->flags & 0x400000) != 0)
            MEM_FreeAlign(gao->basic->mtx);
        if ((gao->flags & 0x200000) != 0)
        {
            multiPart = gao->basic->multiPartData;
            if (multiPart->elemNum)
                MEM_FreeAlign(multiPart->multipartGaoObjectPartsOrMatrices);
            MEM_Free(gao->basic->multiPartData);
        }
        MEM_Free(gao->basic);
    }
    if ((gao->flags & 0x2000) != 0)
    {
        if ((gao->flags & 0x800000) != 0)
            GRP_ObjDetachGroup(gao);
        if ((gao->flags & 0x20) != 0)
            AI_FreeInstance(gao->extended->AIInstance);
        if ((gao->flags & 0x2000000) != 0)
            EVE_FreeMainStruct(gao);
        if ((gao->flags & 0x40000) != 0)
            WAY_FreeStruct(gao->extended->way);
        if ((gao->flags & 0x10) != 0)
        {
            light = gao->extended->light;
            if (light)
            {
                light->intf->AddRef(light, -1);
                light->intf->Destroy(light);
            }
        }
        if ((gao->flags & 0x40) != 0)
        {
            MEM_Free(gao->extended->fields);
            gao->extended->fields = 0;
        }
        if ((gao->flags & 0x8000) != 0)
        {
            MEM_Free(gao->extended->messages);
            gao->extended->messages = 0;
        }
        COL_Free(gao);
        mdf = gao->extended->mdf;
        while (mdf)
        {
            next = mdf->nextElem;
            MDF_Modifier_Destroy(mdf);
            mdf = next;
        }

        if ((gao->flags & 0x8000000) != 0)
            SND_FreeMainStruct(gao);
        MEM_Free(gao->extended);
    }
    MEM_Free(gao->boundingVolume);
    LOA_DeleteAddress((int)gao);
    MEM_Free(gao);
}

Mtx4x4* OBJ_pst_GetFlashMatrix(struct sGameObjectMain* gao)
{
    Mtx4x4* matrices;
    Mtx4x4* result;

    if ((gao->flags & 0x4000000) == 0)
        return 0;
    matrices = gao->matrices;
    if ((gao->flags & 0x10000) != 0)
        result = matrices + 2;
    else
        result = matrices + 1;
    return result;
}

void OBJ_ChangeIdentityFlags(tGameObjectMain* gao, unsigned int newFlags, int oldFlags)
{
    tGameObjectBasic* basic;
    Mtx4x4* matrices;
    Mtx4x4* initMtx;
    Mtx4x4* flashMtx;
    tGameObjectVisualObject* visual;
    tGroGeneric* gro;
    tGroGeneric* grm;
    tDyn* dyn;
    tGameObjectMain* parent;
    Mtx4x4* parentMtx;
    tGameObjectMain* walk;
    int walkFlags;
    tGameObjectMultiPartData* multiPart;
    int elemIdx;
    Mtx4x4* partMtx;
    tWay* way;
    tGroGeneric* light;
    tWorld* world;
    tGameObjectMain* walkNext;
    int bvResized;
    Mtx4x4 acc;
    Mtx4x4 tmp;

    bvResized = 0;
    if (!gao)
        return;

    world = WOR_gpst_CurrentWorld;
    if (oldFlags == -1)
        oldFlags = gao->flags;

    if ((newFlags & 0x1000000) != 0)
        newFlags |= 0x200000u;
    if ((newFlags & 0x1604000) != 0 || (newFlags & 6) != 0)
        newFlags |= 0x1000u;
    if ((newFlags & 0x1000) != 0 && (oldFlags & 0x1000) == 0)
    {
        basic = (tGameObjectBasic*)MEM_p_Alloc(sizeof(tGameObjectBasic));
        gao->basic = basic;
        basic->actorObjectOrVisualObject = 0;
        basic->dyn = 0;
        basic->mtx = 0;
        basic->multiPartData = 0;
        basic->extraMaterial = 0;
    }
    if ((newFlags & 0x70) != 0 || (newFlags & 0xA048700) != 0)
        newFlags |= 0x2000u;                      // fixed: char
    if ((newFlags & 0x2000) != 0 && (oldFlags & 0x2000) == 0)
        OBJ_GameObject_CreateExtended(gao);

    if ((newFlags & 0x10000) != 0)
    {
        if ((oldFlags & 0x10000) == 0)
        {
            gao->flags |= 0x10000u;
            if ((newFlags & 0x4000000) != 0)
            {
                matrices = (Mtx4x4*)MATH_Matrix_ReallocArray(gao->matrices, 3);
                gao->matrices = matrices;
                MATH_CopyMatrix(matrices + 2, matrices + 1);
                OBJ_SetInitialAbsoluteMatrix(gao, gao->matrices);
            }
            else
            {
                matrices = (Mtx4x4*)MATH_Matrix_ReallocArray(gao->matrices, 2);
                gao->matrices = matrices;
                OBJ_SetInitialAbsoluteMatrix(gao, matrices);
            }
        }
    }
    else if ((oldFlags & 0x10000) != 0)
    {
        if ((newFlags & 0x4000000) != 0)
        {
            matrices = gao->matrices;
            MATH_CopyMatrix(&matrices[1], &matrices[2]);
            matrices = (Mtx4x4*)MATH_Matrix_ReallocArray(gao->matrices, 2);
        }
        else
        {
            matrices = (Mtx4x4*)MATH_Matrix_ReallocArray(gao->matrices, 1);
        }
        gao->matrices = matrices;
    }

    if ((newFlags & 0x4000) != 0)
    {
        if ((oldFlags & 0x4000) == 0)
        {
            gao->basic->actorObjectOrVisualObject = (tGameObjectVisualObject*)MEM_p_Alloc(sizeof(tGameObjectVisualObject));
            visual = gao->basic->actorObjectOrVisualObject;
            visual->GROGEOOrSomethingElse = 0;
            visual->GROMAT = 0;
            visual->objDrawMask = 0;
            visual->RLIVertexColors = 0;
            visual->renderOnlyWithGFXRender = 0;
            visual->culledFlagFromCullingObject = 0;
            // preserved: these two have no "= 0", so unlike OBJ_GameObject_Create they are left uninitialised
            visual->vmilyenDistanceLightAttenuationFactor;
            visual->boneDisplayOrder_zListBufferNum;
            ((tGameObjectVisualObject *)(gao->basic->actorObjectOrVisualObject))->objDrawMask = 0xffffffff;
        }
    }
    else if ((oldFlags & 0x4000) != 0)
    {
        gro = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
        if (gro)
        {
            gro->intf->AddRef(gro, -1);
            gro->intf->Destroy(gro);
        }
        grm = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
        if (grm)
        {
            grm->intf->AddRef(grm, -1);
            grm->intf->Destroy(grm);
        }
        visual = gao->basic->actorObjectOrVisualObject;
        if (visual->RLIVertexColors)
            MEM_Free(visual->RLIVertexColors);
        MEM_Free(gao->basic->actorObjectOrVisualObject);
        gao->basic->actorObjectOrVisualObject = 0;
    }

    if ((newFlags & 2) != 0)
    {
        if ((oldFlags & 2) == 0)
            gao->basic->actorObjectOrVisualObject = ANI_pst_CreateGameObjectAnim();
    }
    else if ((oldFlags & 2) != 0)
    {
        ANI_Free(gao->basic->actorObjectOrVisualObject);
        gao->basic->actorObjectOrVisualObject = 0;
    }

    if ((newFlags & 4) != 0)
    {
        if ((oldFlags & 4) == 0)
        {
            dyn = DYN_pst_Dyna_Create();
            DYN_Dyna_Init(dyn, 65, 1.0, 3.402822e38f, 3.402822e38f, &MATH_gst_NulVector, &gao->matrices->Pos);
            gao->basic->dyn = dyn;
            OBJ_ResetProcess(gao, WOR_gpst_CurrentWorld->dynCallCount, 64);
        }
    }
    else if ((oldFlags & 4) != 0)
    {
        DYN_Dyna_Close(gao->basic->dyn);
        gao->basic->dyn = 0;
        EOT_UpdateIdentityFlagsOfTable(&world->activatedObjectsTable4PF);
        EOT_RemoveObjectFromTableIfBadFlag(&world->eot.dynamicObjectsTablePF, 4);
    }

    if ((newFlags & 0x400000) != 0)
    {
        if ((oldFlags & 0x400000) == 0)
        {
            gao->basic->mtx = (tGaoMatrix*)MEM_p_AllocAligned(96, 16);
            gao->basic->mtx->refGao = 0;
            gao->basic->mtx->refGao2 = 0;
            MATH_SetIdentityMatrix(&gao->basic->mtx->mtx);
        }
    }
    else if ((oldFlags & 0x400000) != 0)
    {
        if ((newFlags & 0x10000) != 0)
        {
            basic = gao->basic;
            parent = basic->mtx->refGao;
            if (parent && (parent->flags & 0x10000) != 0)
            {
                parentMtx = OBJ_pst_GetAbsoluteMatrix(basic->mtx->refGao);
                MATH_MulMatrixMatrix(&acc, gao->matrices + 1, parentMtx + 1);
                walk = gao->basic->mtx->refGao;
                walkFlags = walk->flags;
                walkNext = walk;
                if ((walk->flags & 0x10000) != 0)
                {
                    while ((walkFlags & 0x400000) != 0)
                    {
                        basic = walk->basic;
                        parent = basic->mtx->refGao;
                        if (!parent)
                            break;
                        if ((parent->flags & 0x10000) == 0)
                            break;
                        parentMtx = OBJ_pst_GetAbsoluteMatrix(basic->mtx->refGao);
                        MATH_MulMatrixMatrix(&tmp, &acc, parentMtx + 1);
                        MATH_CopyMatrix(&acc, &tmp);
                        walkNext = walkNext->basic->mtx->refGao;
                        walkFlags = walkNext->flags;
                        if ((walkNext->flags & 0x10000) == 0)
                            break;
                        walk = walkNext;
                    }
                }
                MATH_CopyMatrix(gao->matrices + 1, &acc);
            }
        }
        MEM_FreeAlign(gao->basic->mtx);
        gao->basic->mtx = 0;
    }

    if ((newFlags & 0x1000000) != 0 && (oldFlags & 0x200000) == 0)
        newFlags |= 0x200000u;
    if ((newFlags & 0x200000) != 0)
    {
        if ((oldFlags & 0x200000) == 0)
        {
            gao->basic->multiPartData = (tGameObjectMultiPartData*)MEM_p_Alloc(sizeof(tGameObjectMultiPartData));
            gao->basic->multiPartData->elemNum = 0;
            gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = 0;
        }
    }
    else if ((oldFlags & 0x200000) != 0)
    {
        basic = gao->basic;
        gao->flags |= 0x200000u;
        multiPart = basic->multiPartData;
        if (multiPart->elemNum)
            MEM_Free(multiPart->multipartGaoObjectPartsOrMatrices);
        MEM_Free(gao->basic->multiPartData);
        gao->basic->multiPartData = 0;
    }

    if ((newFlags & 0x1000000) != 0)
    {
        if ((oldFlags & 0x1000000) == 0)
        {
            multiPart = gao->basic->multiPartData;
            if (multiPart->elemNum)
            {
                MEM_Free(multiPart->multipartGaoObjectPartsOrMatrices);
                gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_Alloc(sizeof(tMultiPartGameObjectLinks) * gao->basic->multiPartData->elemNum);
                // preserved: indexed as an array of pointers, where every other site indexes the array itself
                for (elemIdx = 0; elemIdx < gao->basic->multiPartData->elemNum; ++elemIdx)
                    ((tMultiPartGameObjectLinks**)(gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx]->mtxPtr = 0;
            }
        }
    }
    else if ((oldFlags & 0x1000000) != 0)
    {
        multiPart = gao->basic->multiPartData;
        if (multiPart->elemNum)
        {
            MEM_Free(multiPart->multipartGaoObjectPartsOrMatrices);
            gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(sizeof(Mtx4x4) * gao->basic->multiPartData->elemNum, 16);
            // preserved: indexed as an array of pointers, where every other site indexes the array itself
            for (elemIdx = 0; elemIdx < gao->basic->multiPartData->elemNum; ++elemIdx)
            {
                partMtx = ((Mtx4x4**)gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices)[elemIdx];
                MATH_SetIdentityMatrix(partMtx);
            }
        }
    }

    if ((newFlags & 0x40) != 0)
    {
        if ((oldFlags & 0x40) == 0)
        {
            gao->extended->fields = (tGameObjectDesignFields*)MEM_p_Alloc(0x58u);
            memset(gao->extended->fields, 0, sizeof(tGameObjectDesignFields));
        }
    }
    else if ((oldFlags & 0x40) != 0)
    {
        MEM_Free(gao->extended->fields);
        gao->extended->fields = 0;
    }

    if ((newFlags & 0x8000) != 0)
    {
        if ((oldFlags & 0x8000) == 0)
        {
            gao->extended->messages = (tGameObjectMessages*)MEM_p_Alloc(0x108u);
            memset(gao->extended->messages, 0, sizeof(tGameObjectMessages));
        }
    }
    else if ((oldFlags & 0x8000) != 0)
    {
        MEM_Free(gao->extended->messages);
        gao->extended->messages = 0;
    }

    if ((newFlags & 0x100) != 0 && (oldFlags & 0x100) == 0)
    {
        if ((newFlags & 0x4000) != 0)
        {
            COL_AllocColMap(gao);
            gao->flags2 |= 0x80u;
            INT_SnP_AddObject(gao, world->snp);
            INT_SnP_InsertionSort(world->snp, 0);
        }
        else
        {
            newFlags &= ~0x100u;
            gao->flags2 &= ~0x80u;
        }
    }
    if ((newFlags & 0x100) == 0 && (oldFlags & 0x100) != 0)
        COL_FreeColMap(gao->extended->coll);

    if ((newFlags & 0x200) != 0 && (oldFlags & 0x200) == 0)
        newFlags &= ~0x200u;
    if ((newFlags & 0x400) != 0 && (oldFlags & 0x400) == 0)
        newFlags &= ~0x400u;
    if ((newFlags & 0x200) == 0 && (oldFlags & 0x200) != 0)
    {
        newFlags &= ~0x400u;
        COL_FreeInstance(gao->extended->coll);
    }
    if ((newFlags & 0x400) == 0 && (oldFlags & 0x400) != 0)
    {
        newFlags &= ~0x200;
        COL_FreeInstance(gao->extended->coll);
    }

    if ((newFlags & 0x20) != 0 && (oldFlags & 0x20) == 0)
    {
        COL_AllocDetectionList(gao);
        gao->flags2 |= 0x20u;
    }
    if ((newFlags & 0x20) != 0)
    {
        if ((oldFlags & 0x20) == 0)
            newFlags &= ~0x20u;
    }
    else
    {
        if ((oldFlags & 0x20) != 0 && (gao->flags2 & 0x100000) == 0)
        {
            COL_FreeDetectionList(gao->extended->coll);
            gao->extended->coll->detectionList = 0;
            gao->flags2 &= ~0x20u;
        }
        if ((oldFlags & 0x20) != 0)
        {
            AI_FreeInstance(gao->extended->AIInstance);
            gao->extended->AIInstance = 0;
            EOT_UpdateIdentityFlagsOfTable(&world->activatedObjectsTable4PF);
            EOT_RemoveObjectFromTableIfBadFlag(&world->eot.AICallbacksTablePF, 32);
        }
    }

    if ((newFlags & 0x800000) != 0)
    {
        if ((oldFlags & 0x800000) == 0)
            newFlags &= 0xFF7FFFFF;
    }
    else if ((oldFlags & 0x800000) != 0)
    {
        GRP_ObjDetachGroup(gao);
    }

    if ((newFlags & 0x2000000) != 0)
    {
        if ((oldFlags & 0x2000000) == 0)
            EVE_AllocateMainStruct(gao);
    }
    else if ((oldFlags & 0x2000000) != 0)
    {
        EVE_FreeMainStruct(gao);
        EOT_UpdateIdentityFlagsOfTable(&world->activatedObjectsTable4PF);
        EOT_RemoveObjectFromTableIfBadFlag(&world->eot.eventGameObjectsTablePF, 0x2000000);
    }

    if ((newFlags & 0x8000000) != 0)
    {
        if ((oldFlags & 0x8000000) == 0)
            newFlags &= ~0x8000000u;
    }
    else if ((oldFlags & 0x8000000) != 0)
    {
        gao->flags |= 0x8000000u;
        SND_FreeMainStruct(gao);
        gao->flags &= ~0x8000000u;
    }

    if ((newFlags & 0x40000) != 0)
    {
        if ((oldFlags & 0x40000) == 0)
        {
            gao->extended->way = (tWay*)MEM_p_Alloc(8u);
            way = gao->extended->way;
            way->wayElems = 0;
            way->wayElemNum = 0;
        }
    }
    else if ((oldFlags & 0x40000) != 0)
    {
        WAY_FreeStruct(gao->extended->way);
        gao->extended->way = 0;
    }

    if ((newFlags & 0x10) != 0)
    {
        if ((oldFlags & 0x10) == 0)
        {
            light = (tGroGeneric*)GRO_pst_Struct_Create(2);
            if (TAB_ul_Ptable_GetElemIndexWithPointer(&world->worldGROTable, light) == -1)
            {
                TAB_PTable_AddElemToTable(&world->worldGROTable, light);
                light->intf->AddRef(light, 1);
            }
            light->intf->AddRef(light, 1);
            gao->extended->light = light;
        }
    }
    else if ((oldFlags & 0x10) != 0)
    {
        light = gao->extended->light;
        if (light)
        {
            light->intf->AddRef(light, -1);
            light->intf->Destroy(light);
        }
        gao->extended->light = 0;
    }

    if ((newFlags & 0x80000) != 0)
    {
        if ((oldFlags & 0x80000) == 0)
        {
            gao->boundingVolume = (tGameObjectBV*)MEM_p_Realloc(gao->boundingVolume, 0x48u);
            bvResized = 1;
        }
    }
    else if ((oldFlags & 0x80000) != 0)
    {
        gao->boundingVolume = (tGameObjectBV*)MEM_p_Realloc(gao->boundingVolume, 0x30u);
        bvResized = 1;
    }

    gao->flags = newFlags;
    OBJ_UpdateStatusFlagRTL(gao);
    if (bvResized)
        OBJ_ComputeBV(gao, 0);
}

void OBJ_GameObject_AllocateGizmo(struct sGameObjectMain* gao, int elemNum, int asLinks)
{
    unsigned int newFlags;
    tGameObjectMultiPartData* multiPart;
    int size;

    if (asLinks)
    {
        if (gao)
            newFlags = gao->flags | 0x1200000;
        else
            newFlags = 0x1200000;
    }
    else
    {
        if (gao)
            newFlags = gao->flags;
        else
            newFlags = 0;
        newFlags |= 0x200000;
    }
    if ((gao->flags & 0x200000) != 0)
    {
        multiPart = gao->basic->multiPartData;
        if (multiPart->multipartGaoObjectPartsOrMatrices)
        {
            MEM_FreeAlign(multiPart->multipartGaoObjectPartsOrMatrices);
            MEM_Free(gao->basic->multiPartData);
            gao->flags &= ~0x1200000u;
        }
    }
    OBJ_ChangeIdentityFlags(gao, newFlags, -1);
    size = elemNum * (asLinks != 0 ? sizeof(tMultiPartGameObjectLinks) : sizeof(Mtx4x4));
    gao->basic->multiPartData->elemNum = elemNum;
    gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(size, 16);
    memset(gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices, 0, size);
}

void OBJ_ResolveAIRef(struct sGameObjectMain* gao)
{
    tGameObjectExtended* extended;
    tAI* ai;
    tGameObjectBasic* basic;
    tGameObjectActorObject* actor;
    tActionKit* actionKit;
    tAction** action;
    int elemIdx;
    tAnim* anim;
    int actionIdx;
    tAction** actions;
    tActionElem* actionElems;

    if ((gao->flags & 0x20) != 0)
    {
        extended = gao->extended;
        if (extended)
        {
            ai = extended->AIInstance;
            if (ai)
            {
                AI_ResolveInstanceRef(ai);
                AI_ReinitInstance(gao->extended->AIInstance);
            }
        }
    }
    if ((gao->flags & 0x2000000) != 0)
        EVE_ResolveRefs(gao, 1);
    if ((gao->flags & 2) != 0)
    {
        basic = gao->basic;
        if (basic)
        {
            actor = (tGameObjectActorObject*)basic->actorObjectOrVisualObject;
            if (basic->actorObjectOrVisualObject)
            {
                actionKit = actor->actionKit;
                if (actionKit)
                {
                    actions = (tAction**)(((char*)(actionKit)) + sizeof(tActionKit));
                    for (actionIdx = 0; actionIdx < actionKit->elemNum; ++actionIdx)
                    {
                        action = &actions[actionIdx];
                        if (!*action)
                            continue;
                        actionElems = (tActionElem*)(((char*)*action) + sizeof(tAction));
                        for (elemIdx = 0; elemIdx < (*action)->actionElemNum; ++elemIdx)
                        {
                            if (actionElems[elemIdx].eventLists)
                                EVE_ResolveListTrackRef(actionElems[elemIdx].eventLists);
                        }
                    }
                }
                else
                {
                    anim = actor->animPtr[0];
                    if (anim)
                    {
                        if (anim != (tAnim*)-1)
                        {
                            if (anim->ptr)
                                EVE_ResolveListTrackRef(anim->ptr->eventListStruct);
                        }
                    }
                }
            }
        }
    }
}

struct sGameObjectMain* OBJ_GameObject_Duplicate(struct sWorld* world, struct sGameObjectMain* srcGao, int attach, int dupEvents, Vec3* pos)
{
    int srcFlags; // eax
    tGameObjectMain* gao; // ebp
    int flags; // ecx
    int flags2; // edx
    tGameObjectExtended* srcExtended; // eax
    int* srcColors; // eax
    unsigned int colorBytes; // esi
    int elemNum; // eax
    tGameObjectMultiPartData* srcMultiPart; // eax
    unsigned int partBytes; // eax
    tWorld* destWorld; // esi

    if (srcGao)
        srcFlags = srcGao->flags;
    else
        srcFlags = 0;
    gao = OBJ_GameObject_Create(srcFlags | 0x20000);
    gao->process = srcGao->process & 0xEF;
    gao->designFlags = srcGao->designFlags;
    flags = gao->flags;
    gao->u3 = srcGao->u3;
    gao->flags2 = gao->flags2 & 0xffff00ff; //BYTE1(gao->flags2) = 0;
    flags2 = gao->flags2;
    gao->if_this_not0_wont_render_gao = 0;
    flags2 = srcGao->flags2 & 0xFF00 | flags2;
    gao->flags2 = flags2;
    gao->flags = flags & 0xFFFBFFFF;
    if (dupEvents)
    {
        if ((srcGao->flags2 & 0x40000000) != 0)
            gao->flags2 = flags2 | 0x40000000;
        if ((srcGao->flags2 & 0x20000000) != 0)
            gao->flags2 |= 0x20000000u;
    }
    if ((srcGao->flags2 & 0x1000000) != 0)
        gao->flags2 |= 0x1000000u;
    if ((srcGao->flags2 & 0x2000000) != 0)
        gao->flags2 |= 0x2000000u;
    if ((srcGao->flags2 & 0x80) != 0)
        gao->flags2 |= 0x80u;
    if ((srcGao->flags2 & 0x100000) != 0)
        gao->flags2 |= 0x100000u;
    if ((srcGao->flags2 & 0x800000) != 0)
        gao->flags2 |= 0x800000u;
    srcExtended = srcGao->extended;
    if (srcExtended)
    {
        gao->extended->extraFlag = srcExtended->extraFlag;
        gao->extended->sectorID = -1;
        gao->extended->sectorElemIdx = -1;
    }
    MATH_CopyMatrix(gao->matrices, srcGao->matrices);
    if (pos)
        gao->matrices->Pos = *pos;
    if ((gao->flags & 0x10000) != 0)
    {
        MATH_CopyMatrix(gao->matrices + 1, srcGao->matrices + 1);
        if (pos)
            gao->matrices[1].Pos = *pos;
        if ((gao->flags & 0x4000000) != 0)
        {
            MATH_CopyMatrix(gao->matrices + 2, srcGao->matrices);
            if (pos)
                gao->matrices[2].Pos = *pos;
        }
    }
    else if ((gao->flags & 0x4000000) != 0)
    {
        MATH_CopyMatrix(gao->matrices + 1, srcGao->matrices);
        if (pos)
            gao->matrices[1].Pos = *pos;
    }
    if (gao->extended)
    {
        gao->extended->capaBackup = srcGao->extended->capaBackup;
        gao->extended->AIPriority = srcGao->extended->AIPriority;
        MDF_Modifier_DuplicateList(gao, srcGao->extended->mdf);
    }
    if ((srcGao->flags & 0x4000) != 0)
    {
        memcpy(
            gao->basic->actorObjectOrVisualObject,
            srcGao->basic->actorObjectOrVisualObject,
            sizeof(tGameObjectVisualObject));
        srcColors = ((tGameObjectVisualObject*)(srcGao->basic->actorObjectOrVisualObject))->RLIVertexColors;
        if (srcColors)
        {
            colorBytes = 4 * *srcColors + 4;
            ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors = (int*)MEM_p_Alloc(colorBytes);
            memcpy(
                ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors,
                ((tGameObjectVisualObject*)(srcGao->basic->actorObjectOrVisualObject))->RLIVertexColors,
                colorBytes);
        }
        else
        {
            ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors = 0;
        }
    }
    if ((srcGao->flags & 0x800000) != 0)
        gao->extended->grp = srcGao->extended->grp;
    if ((gao->flags & 0x400000) != 0)
    {
        // preserved: the second copy repeats the first 0x50 bytes the first one has already copied
        memcpy(gao->basic->mtx, srcGao->basic->mtx, sizeof(tGaoMatrix));
        memcpy(gao->basic->mtx, srcGao->basic->mtx, 0x50u);
    }
    if ((gao->flags & 0x200000) != 0)
    {
        elemNum = srcGao->basic->multiPartData->elemNum;
        if (elemNum)
        {
            gao->basic->multiPartData->elemNum = elemNum;
            srcMultiPart = srcGao->basic->multiPartData;
            if ((gao->flags & 0x1000000) != 0)
            {
                gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_Alloc(12 * gao->basic->multiPartData->elemNum);
                partBytes = sizeof(tMultiPartGameObjectLinks) * srcMultiPart->elemNum;
            }
            else
            {
                gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(
                    sizeof(Mtx4x4) * gao->basic->multiPartData->elemNum,
                    16);
                partBytes = 80 * srcMultiPart->elemNum;
            }
            memcpy(
                gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices,
                srcMultiPart->multipartGaoObjectPartsOrMatrices,
                partBytes);
        }
    }
    destWorld = world;
    if ((srcGao->flags & 2) != 0)
        ANI_pst_DuplicateGameObjectAnim(world, gao, srcGao);
    if ((gao->flags & 0x20) != 0)
        AI_DuplicateInstance(srcGao, gao);
    if ((gao->flags & 0x2000000) != 0)
        EVE_Duplicate(srcGao, gao, dupEvents);
    if ((gao->flags & 0x8000000) != 0)
        SND_Duplicate(srcGao, gao);
    if ((gao->flags & 0x10) != 0)
        gao->extended->light = srcGao->extended->light;
    if ((gao->flags & 0x600) != 0)
        COL_DuplicateInstance(gao, srcGao);
    if ((gao->flags & 0x100) != 0)
        COL_DuplicateColMap(gao, srcGao);
    flags2 = gao->flags2;
    if ((flags2 & 0x100000) != 0 && (flags2 & 0x20) == 0)
    {
        COL_AllocDetectionList(gao);
        gao->flags2 |= 0x20u;
    }
    if ((gao->flags & 0x40) != 0)
    {
        gao->extended->fields = (tGameObjectDesignFields*)MEM_p_Alloc(0x58u);
        memcpy(gao->extended->fields, srcGao->extended->fields, sizeof(tGameObjectDesignFields));
        destWorld = world;
    }
    OBJ_DuplicateBV(srcGao, gao);
    if (attach)
        WOR_World_AttachGameObject(destWorld, gao);
    if (destWorld)
    {
        destWorld->tablesManagerAndEOTNeeded = 1;
        destWorld->visibilityCallNeeded = 1;
        destWorld->SnPRefreshNeeded = TRUE;
    }
    return gao;
}

void OBJ_Gizmo_UpdateGizmoPtr(struct sGameObjectMain* gao)
{
    tGameObjectMultiPartData* multiPart;
    int elemIdx;
    tMultiPartGameObjectLinks* link;
    tGameObjectMain* partGao;
    int mtxIdx;
    tGameObjectMultiPartData* partMultiPart;

    if ((gao->flags & 0x200000) != 0 && (gao->flags & 0x1000000) != 0)
    {
        multiPart = gao->basic->multiPartData;
        for (elemIdx = 0; elemIdx < multiPart->elemNum; ++elemIdx)
        {
            link = &((tMultiPartGameObjectLinks*)multiPart->multipartGaoObjectPartsOrMatrices)[elemIdx];
            link->mtxPtr = NULL;
            partGao = link->gao;
            if (!partGao)
                continue;
            mtxIdx = link->mtxIdx;
            if (mtxIdx == -1)
            {
                link->mtxPtr = partGao->matrices;
            }
            else if ((partGao->flags & 0x200000) != 0 && (partGao->flags & 0x1000000) == 0)
            {
                partMultiPart = partGao->basic->multiPartData;
                if (mtxIdx < partMultiPart->elemNum)
                    link->mtxPtr = &(((Mtx4x4*)(partMultiPart->multipartGaoObjectPartsOrMatrices))[mtxIdx]);
            }
        }
    }
}

struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t Offset)
{
    tGameObjectMain* gameObject;
    int fileFlags2;
    tGameObjectExtended* extended;
    int flags2;
    tGameObjectCollMain* coll;
    unsigned int detectionBit;
    char processByte;
    tGameObjectBV* bv;
    int key;
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    char zListBufferNum;
    int colorCount;
    unsigned int colorBytes;
    int* colorPtr;
    tGaoMatrix* gaoMtx;
    tGameObjectActorObject* actor;
    int elemNum;
    int remaining;
    int partBytes;
    int elemIdx;
    tGameObjectMain** linkSlot;
    BOOL hasModifiers;
    int modifierBytes;
    int nameLen;
    char* currRdrPtr;
    int Str1;
    char sstr1[4];
    int fileFlags;
    int fileSize;
    Mtx4x4 mtx;
    char gameObjectName[256];

    currRdrPtr = BIG_pc_ReadFileTmp(Offset, &fileSize);
    LOA_ReadCharArray(&currRdrPtr, sstr1, 4u, 1);
    if (strncmp(sstr1, ".gao", 4u))
        return 0;
    LOA_ReadLong(&currRdrPtr, 0, 2);
    LOA_ReadLong(&currRdrPtr, 0, 1);
    fileFlags = LOA_ReadLong(&currRdrPtr, 0, 1) & ~4u;
    gameObject = OBJ_GameObject_Create(fileFlags);
    OBJ_gpst_ObjectToLoadIn = gameObject;
    gameObject->flags2 = 0x7B9C0080;
    fileFlags2 = LOA_ReadLong(&currRdrPtr, 0, 1);
    extended = gameObject->extended;
    flags2 = fileFlags2 & gameObject->flags2;
    gameObject->flags2 = flags2;
    if (extended && (coll = extended->coll) != 0 && coll->detectionList)
        detectionBit = flags2 | 0x20;
    else
        detectionBit = flags2 & ~0x20u;
    gameObject->flags2 = detectionBit;
    if ((fileFlags2 & 0x20000000) != 0)
        gameObject->flags2 = detectionBit | 0x200000;
    if ((fileFlags2 & 0x40000000) != 0)
        gameObject->flags2 |= 0x400000u;
    gameObject->flags2 |= fileFlags2 & 0xFF00;
    OBJ_UpdateStatusFlagRTL(gameObject);
    flags2 = gameObject->flags2;
    if ((flags2 & 0x20) == 0 && (flags2 & 0x100000) != 0)
    {
        gameObject->flags2 = flags2 | 0x20;
        COL_AllocDetectionList(gameObject);
    }
    LOA_ReadChar(&currRdrPtr, 0, 1);
    gameObject->if_this_not0_wont_render_gao = 0;
    gameObject->u3 = LOA_ReadChar(&currRdrPtr, 0, 1);
    LOA_ReadShort(&currRdrPtr, 0, 2);
    if (!gameObject->u3)
        gameObject->u3 = 16;
    gameObject->distance = LOA_ReadChar(&currRdrPtr, 0, 1);
    gameObject->LODDist = LOA_ReadChar(&currRdrPtr, 0, 1);
    gameObject->designFlags = LOA_ReadChar(&currRdrPtr, 0, 1);
    processByte = LOA_ReadChar(&currRdrPtr, 0, 1);
    gameObject->process = processByte & 0xC | ((processByte & 0xC) >> 2) & 1 | ((processByte & 0xC | ((processByte & 0xC) >> 2) & 1) >> 2) & 2;
    gameObject->if_this_not0_wont_render_gao = 0;
    LOA_ReadMatrix(&currRdrPtr, &mtx, 1);
    MATH_SetCorrectType(&mtx);
    if ((fileFlags & 0x10000) != 0)
        MATH_CopyMatrix(gameObject->matrices + 1, &mtx);
    MATH_CopyMatrix(gameObject->matrices, &mtx);
    if (gameObject->boundingVolume)
    {
        LOA_ReadLong(&currRdrPtr, 0, 2);
        LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMinOriginal, 1);
        bv = gameObject->boundingVolume;
        bv->BVMin.x = bv->BVMinOriginal.x;
        bv->BVMin.y = bv->BVMinOriginal.y;
        bv->BVMin.z = bv->BVMinOriginal.z;
        LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMaxOriginal, 1);
        bv = gameObject->boundingVolume;
        bv->BVMax.x = bv->BVMaxOriginal.x;
        bv->BVMax.y = bv->BVMaxOriginal.y;
        bv->BVMax.z = bv->BVMaxOriginal.z;
        if ((fileFlags & 0x80000) != 0)
        {
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMinLocal, 1);
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMaxLocal, 1);
        }
    }
    if ((fileFlags & 0x1000) != 0)
    {
        if ((fileFlags & 0x4000) != 0)
        {
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key == -1)
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = 0;
            else
                LOA_MakeFileRef(
                    key, &(((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse), GEO_ul_Load_ObjectCallback, 1);
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key == -1)
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROMAT = 0;
            else
                LOA_MakeFileRef(key, &(((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROMAT), GEO_ul_Load_ObjectCallback, 1);
            ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->objDrawMask = LOA_ReadLong(&currRdrPtr, 0, 1);
            ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum = (uint8_t)LOA_ReadLong(&currRdrPtr, 0, 1);// csak az elsõ bájtot veszi figyelembe
            basic = gameObject->basic;
            visual = basic->actorObjectOrVisualObject;
            zListBufferNum = ((tGameObjectVisualObject*)(basic->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum;
            if (zListBufferNum == -1)
            {
                visual->boneDisplayOrder_zListBufferNum = 0;
            }
            else if (zListBufferNum > 10)
            {
                visual->boneDisplayOrder_zListBufferNum = zListBufferNum - 16;
            }
            colorCount = LOA_ReadLong(&currRdrPtr, 0, 1);
            if (colorCount)
            {
                colorBytes = 4 * colorCount + 4;
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors = (int*)MEM_p_Alloc(colorBytes);
                LOA_ul_FileTypeSize_27 += colorBytes;
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors[0] = colorCount; // num
                for (colorPtr = ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors + 1; colorCount; --colorCount)
                    *colorPtr++ = LOA_ReadLong(&currRdrPtr, 0, 1);
            }
            else
            {
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors = 0;
            }
        }
        if ((fileFlags & 0x400000) != 0)
        {
            gaoMtx = gameObject->basic->mtx;
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key == -1)
            {
                gaoMtx->refGao2 = NULL;
                gaoMtx->refGao = NULL;
            }
            else
            {
                LOA_MakeFileRef(key, &gaoMtx->refGao, OBJ_ul_GameObjectCallback, 1);
                LOA_MakeFileRef(Str1, &gaoMtx->refGao2, OBJ_ul_GameObjectCallback, 1);
            }
            LOA_ReadMatrix(&currRdrPtr, &gaoMtx->mtx, 1);
        }
        if ((fileFlags & 2) != 0)
        {
            actor = (tGameObjectActorObject*)gameObject->basic->actorObjectOrVisualObject;
            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            actor->animPtr[0] = (tAnim*)Str1;
            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            actor->shape = (tShape*)Str1;
            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            actor->group = (tGroup*)Str1;
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key != -1)
            {
                if (key)
                    LOA_MakeFileRef(key, &(((tGameObjectActorObject*)(gameObject->basic->actorObjectOrVisualObject))->actionKit), ACT_ul_ActionKitCallback, 1);
            }
        }
        if ((fileFlags & 0x200000) != 0)
        {
            elemNum = LOA_ReadLong(&currRdrPtr, 0, 1);
            remaining = elemNum;
            gameObject->basic->multiPartData->elemNum = elemNum;
            if (elemNum)
            {
                if ((fileFlags & 0x1000000) != 0)
                {
                    partBytes = sizeof(tMultiPartGameObjectLinks) * elemNum;
                    gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_Alloc(sizeof(tMultiPartGameObjectLinks) * elemNum);
                    LOA_ul_FileTypeSize_34 += partBytes;
                    for (elemIdx = 0; remaining > 0; ++elemIdx, --remaining)
                    {
                        ((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx].mtxPtr = NULL;
                        Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
                        ((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx].gao = (tGameObjectMain*)Str1; // ez csak a key egyelore
                        linkSlot = &(((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx].gao);
                        if (*linkSlot == (tGameObjectMain*)-1)
                            *linkSlot = 0;
                        else
                            LOA_MakeFileRef(Str1, linkSlot, OBJ_ul_GameObjectCallback, 1);
                        ((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx].mtxIdx = LOA_ReadLong(&currRdrPtr, 0, 1);
                    }
                }
                else
                {
                    partBytes = sizeof(Mtx4x4) * elemNum;
                    gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(sizeof(Mtx4x4) * elemNum, 16);
                    LOA_ul_FileTypeSize_34 += partBytes;
                    for (elemIdx = 0; remaining > 0; ++elemIdx, --remaining)
                    {
                        LOA_ReadMatrix(&currRdrPtr, &(((Mtx4x4*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[elemIdx]), 1);
                        LOA_ReadString(&currRdrPtr, 0, 64, 2);
                    }
                }
            }
        }
    }
    if ((fileFlags & 0x2000) != 0)
    {
        key = LOA_ReadLong(&currRdrPtr, 0, 1);
        Str1 = key;
        if ((fileFlags & 0x800000) != 0 && key)
            LOA_MakeFileRef(key, &gameObject->extended->grp, GRP_ul_LoadGrpCallback, 1);
        Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
        hasModifiers = Str1 != 0;
        gameObject->extended->aiLod = LOA_ReadFloat(&currRdrPtr, 0, 1);
        gameObject->extended->renderMinReqDistSq = LOA_ReadFloat(&currRdrPtr, 0, 1);
        gameObject->extended->renderMinReqDistSq = 0.0;
        LOA_ReadLong(&currRdrPtr, 0, 2);
        LOA_ReadLong(&currRdrPtr, 0, 2);
        LOA_ReadLong(&currRdrPtr, 0, 2);
        LOA_ReadLong(&currRdrPtr, 0, 2);
        LOA_ReadLong(&currRdrPtr, 0, 2);
        gameObject->extended->capaBackup = LOA_ReadShort(&currRdrPtr, 0, 1);
        gameObject->extended->capa = gameObject->extended->capaBackup;
        LOA_ReadShort(&currRdrPtr, 0, 2);
        gameObject->extended->AIPriority = LOA_ReadChar(&currRdrPtr, 0, 1);
        gameObject->extended->aa = LOA_ReadChar(&currRdrPtr, 0, 1);
        gameObject->extended->extraFlag = LOA_ReadShort(&currRdrPtr, 0, 1);// animationcontrol is
        gameObject->extended->extraFlag &= ~8u; // LOBYTE
        gameObject->extended->sectorID = -1;
        gameObject->extended->sectorElemIdx = -1;
        if ((fileFlags & 0x20) != 0)
        {
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key)
            {
                if (key != -1)
                    LOA_MakeFileRef(key, &(gameObject->extended->AIInstance), AI_ul_CallbackLoadInstance, 1);
            }
        }
        if ((fileFlags & 0x2000000) != 0)
        {
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key)
            {
                if (key != -1)
                {
                    gameObject->extended->eve = (tEventData*)MEM_p_Alloc(sizeof(tEventData));
                    LOA_ul_FileTypeSize_37 += sizeof(tEventData);
                    LOA_MakeFileRef(Str1, &gameObject->extended->eve->eventListStruct, EVE_ul_CallbackLoadListTracks, 1);
                    gameObject->extended->eve->tracks = 0;
                }
            }
        }
        if ((fileFlags & 0x8000000) != 0)
        {
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key)
            {
                if (key != -1)
                    LOA_MakeFileRef(key, &gameObject->extended->snd, SND_ul_CallbackLoad, 1);
            }
        }
        if ((fileFlags & 0x40000) != 0)
        {
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key)
            {
                if (key != -1)
                    LOA_MakeFileRef(key, &gameObject->extended->way, WAY_ul_AllLinkListsCallback, 1);
            }
        }
        if ((fileFlags & 0x10) != 0)
        {
            key = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = key;
            if (key)
            {
                if (key != -1)
                    LOA_MakeFileRef(key, &gameObject->extended->light, GEO_ul_Load_ObjectCallback, 1);
            }
        }
        if ((fileFlags & 0x40) != 0)
        {
            LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields = (tGameObjectDesignFields*)MEM_p_Alloc(88u);
            memset(gameObject->extended->fields, 0, sizeof(tGameObjectDesignFields));
            gameObject->extended->fields->designFlags = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designInt1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designInt2 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designFloat1 = LOA_ReadFloat(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designFloat2 = LOA_ReadFloat(&currRdrPtr, 0, 1);
            LOA_ReadVector(&currRdrPtr, &gameObject->extended->fields->designVec1, 1);
            LOA_ReadVector(&currRdrPtr, &gameObject->extended->fields->designVec2, 1);
            gameObject->extended->fields->designGO1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designGO2 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designNet1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designNet2 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designText1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->u9 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designText2 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->u11 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designDesFlags = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->u13 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designInt3 = LOA_ReadLong(&currRdrPtr, 0, 1);
            gameObject->extended->fields->designDesFlags = gameObject->extended->fields->u13;
        }
        if (hasModifiers)
        {
            modifierBytes = MDF_ul_Modifier_Load(&gameObject->extended->mdf, currRdrPtr, 1, gameObject);
            currRdrPtr += modifierBytes;
            LOA_ReadLong(&currRdrPtr, 0, 1);
        }
    }
    if ((fileFlags & 0x600) != 0)
    {
        key = LOA_ReadLong(&currRdrPtr, 0, 1);
        Str1 = key;
        if (key)
        {
            if (key != -1)
                LOA_MakeFileRef(key, &gameObject->extended->coll->colInstance, COL_ul_CallBackLoadInstance, 1);
        }
    }
    if ((fileFlags & 0x100) != 0)
    {
        key = LOA_ReadLong(&currRdrPtr, 0, 1);
        Str1 = key;
        if (key)
        {
            if (key != -1)
            {
                LOA_MakeFileRef(key, &gameObject->extended->coll->colMap, COL_ul_CallBackLoadColMap, 1);
                gameObject->flags2 |= 0x80u;
            }
        }
    }
    nameLen = LOA_ReadLong(&currRdrPtr, 0, 1);
    if (nameLen) {
        LOA_ReadString(&currRdrPtr, gameObjectName, nameLen, 1); //LOA_ReadString(&currRdrPtr, 0, v46, 1);
        DBG_OutputDebugString("Loading game object: %s (flags: %x, flags2: %x)\n", gameObjectName, gameObject->flags, gameObject->flags2);
    }
    else {
        DBG_OutputDebugString("Loading game object: <unnamed> (flags: %x, flags2: %x)\n", gameObject->flags, gameObject->flags2);
    }
    LOA_ReadLong(&currRdrPtr, 0, 2);
    LOA_ReadLong(&currRdrPtr, 0, 2);
    LOA_ReadLong(&currRdrPtr, 0, 2);
    LOA_ReadLong(&currRdrPtr, 0, 2);
    LOA_ReadLong(&currRdrPtr, 0, 2);
    return gameObject;
}

tableDescStruct* OBJ_ul_World_LoadGolCallback(uint32_t Offset)
{
    tableDescStruct* table;
    char* start;
    tableElemStructPF* elem;
    int key;
    void** slot;
    char* rdr;
    int size;

    table = (tableDescStruct*)MEM_p_Alloc(sizeof(tableDescStruct));
    memset(table, 0, sizeof(tableDescStruct));
    start = BIG_pc_ReadFileTmp(Offset, &size);
    rdr = start;

    TAB_PFtable_Init(table, size, 0.1f);
    while (rdr < &start[size])
    {
        ((tableElemStructPF*)(table->nextElemPtr))->objectPtr = 0;
        ((tableElemStructPF*)(table->nextElemPtr))->objectFlags = 0;
        elem = table->nextElemPtr;
        ++table->actElemNum;
        ++elem;
        slot = &elem[-1].objectPtr;
        table->nextElemPtr = elem;
        key = LOA_ReadLong(&rdr, 0, 1);
        LOA_MakeFileRef(key, slot, OBJ_ul_GameObjectCallback, 1);
        LOA_ReadLong(&rdr, 0, 2);
    }
    return table;
}

void OBJ_ScaleSet(struct sGameObjectMain* gao, Vec3* scale)
{
    tGaoMatrix* local;
    Vec3 parentScale;

    if ((gao->flags & 0x400000) != 0)
    {
        if ((gao->flags & 1) != 0)
        {
            OBJ_ComputeRecursivScale(gao, &parentScale);
            scale->x = scale->x / parentScale.x;
            scale->y = scale->y / parentScale.y;
            scale->z = scale->z / parentScale.z;
        }
        local = OBJ_pst_GetLocalMatrix(gao);
        MATH_SetScale(&local->mtx, scale);
        OBJ_ComputeGlobalWhenHie(gao);
        OBJ_ComputeBV(gao, 0);
    }
    else
    {
        MATH_SetScale(gao->matrices, scale);
        OBJ_ComputeBV(gao, 0);
    }
}

void OBJ_ZoomSet(struct sGameObjectMain* gao, float zoom)
{
    Mtx4x4* absMtx;
    int flags;
    Mtx4x4* matrices;
    Mtx4x4* flashMtx;

    absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_SetZoom(absMtx, zoom);
    flags = gao->flags;
    if ((gao->flags & 2) != 0 && (flags & 0x4000000) != 0)
    {
        matrices = gao->matrices;
        if ((flags & 0x10000) == 0)
            flashMtx = matrices + 1;
        else
            flashMtx = matrices + 2;
        MATH_SetZoom(flashMtx, zoom);
    }
    OBJ_ComputeBV(gao, 0);
}

struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* gao)
{
    tGameObjectBasic* basic;
    tGaoMatrix* local;
    tGameObjectMain* result;

    if (gao && (basic = gao->basic) != 0 && (local = basic->mtx) != 0)
        result = local->refGao2;
    else
        result = 0;
    return result;
}

void OBJ_EyeFollow(struct sGameObjectMain* gao, Vec3* target, float weight, int preset, int clampToSight, int unlimited, int torsoFollowsHead, int torsoFollowsOwn, int useGlobalLimits, int useThirdBone)
{
    tGameObjectActorObject* actor;
    tGroup* skeleton;
    tGameObjectMain* head;
    tGameObjectMain* torso;
    tGameObjectMain* third = NULL;
    tGameObjectMain* rootBone;
    tGameObjectMain* parent;
    tGameObjectBasic* basic;
    Mtx4x4* absMtx;
    Mtx4x4* boneMtx;
    tGaoMatrix* localMtx;
    Vec3* bonePos;
    tAction* action;
    tActionElem* actionElem;
    tAction** actions;
    tActionElem* actionElems;
    tActionElem* actionElems2;
    uint8_t actionItem;
    uint8_t customBits;
    uint8_t ramp;
    int16_t designFlags;
    int rampNeeded;
    int blendFull;
    BOOL torsoUpright;
    BOOL compensateRoot;
    double zTmp;
    double xTmp;
    double deltaTmp;
    double limitTmp;
    float headZMax;
    float headZMin;
    float headXMax;
    double headXMin;
    float headXOffset;
    float headXMaxAbs;
    float headXMinAbs;
    float torsoZMax;
    float torsoZMin;
    float thirdZMax;
    float thirdZMin;
    float headZCurrent;
    float headXCurrent;
    float headZTarget;
    float headXTarget;
    float torsoZCurrent;
    float torsoXCurrent;
    float torsoXFromBlend;
    float torsoZTarget;
    float thirdZCurrent;
    float thirdXCurrent;
    float thirdXFromBlend;
    float thirdZTarget;
    float rootZAngle;
    float headZDelta;
    float headXDelta;
    float torsoZDelta;
    float thirdZDelta;
    float headPosX;
    float headPosY;
    float headPosZ;
    float torsoPosX;
    float torsoPosY;
    float torsoPosZ;
    float thirdPosX;
    float thirdPosY;
    float thirdPosZ;
    Vec3 dest;
    Vec3 gaoSight;
    Vec3 sightTmp;
    Vec3 headHorizon;
    Vec3 thirdHorizon;
    Vec3 torsoHorizon;
    Mtx4x4 globalMtx;
    Vec3 torsoBanking;
    Vec3 torsoScale;
    Vec3 thirdScale;
    Vec3 thirdSight;
    Vec3 headScale;
    Vec3 headSight2;
    Vec3 torsoSight2;
    Vec3 scale;
    Mtx4x4 headGlobalMtx;

    if ((gao->flags & 2) == 0)
        return;
    actor = gao->basic->actorObjectOrVisualObject;
    skeleton = actor->skeletonGroup;
    if (!skeleton)
        return;
    if (!skeleton->tablePF || !actor->actionKit || !actor->actionPtr)
        return;

    head = ANI_pst_GetObjectByAICanal(gao, 0);
    torso = ANI_pst_GetObjectByAICanal(gao, 1u);
    if (useThirdBone)
        third = ANI_pst_GetObjectByAICanal(gao, 12u);
    rootBone = (tGameObjectMain*)(((tableElemStructPF*)(((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->firstElemPtr))->objectPtr);
    if (!head || !torso || !rootBone)
        return;

    absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_GetScale(&scale, absMtx);
    basic = gao->basic;
    actor = basic->actorObjectOrVisualObject;
    actionItem = ((tGameObjectActorObject*)(basic->actorObjectOrVisualObject))->actionItem;
    blendFull = 0;
    if (((tGameObjectActorObject*)(basic->actorObjectOrVisualObject))->flagsEgBlendIw2 == 1)
    {
        action = actor->actionPtr;
        actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));
        customBits = actionElems[actionItem].customBits;
        actionElem = &(actionElems[actionItem]);
        rampNeeded = (customBits & 4) == 0;
    }
    else
    {
        action = actor->actionPtr;
        actionElems = (tActionElem*)(((char*)action) + sizeof(tAction));
        actionElem = &(actionElems[actionItem]);
        actions = (tAction**)(((char*)(actor->actionKit)) + sizeof(tActionKit));
        actionElems2 = (tActionElem*)(((char*)(actions[actor->animPtr[1]->actionForTransition])) + sizeof(tAction));
        rampNeeded = (actionElem->customBits & 4) == 0
            || (actionElems2[actor->animPtr[1]->actionElemForTransition].customBits & 4) == 0;
    }

    if (rampNeeded)
    {
        designFlags = actionElem->designFlags;
        if (designFlags >= 0)
        {
            if ((designFlags & 0x1000) != 0)
                return;
            ramp = actor->unkb >= 6u ? 6 : actor->unkb + 1;
            actor->unkb = ramp;
            weight = 0.40000001f;
            if (ramp >= 6u)
                return;
        }
        else
        {
            if (actor->unkb >= 16u)
                ramp = 16;
            else
                ramp = (uint8_t)(actor->unkb + (uint64_t)(TIM_gf_dt * 60.000004 + 0.5));
            actor->unkb = ramp;
            weight = (float)((double)ramp * 0.0625);
            if (ramp >= 16u)
                return;
        }
    }
    else
    {
        actor->unkb = 0;
    }

    headZMax = 1.5707964f;
    torsoZMax = 0.78539819f;
    if (useThirdBone)
        thirdZMax = 0.78539819f;
    headZMin = -1.5707964f;
    torsoZMin = -0.78539819f;
    if (useThirdBone)
        thirdZMin = -0.78539819f;
    headXMin = -0.78539819;
    headXMax = 0.43633232f;
    switch (preset)
    {
    case 0:
        headXOffset = 0.0;
        headZMax = 2.0943952f;
        headZMin = -2.0943952f;
        torsoZMax = 1.0471976f;
        torsoZMin = -1.0471976f;
        if (useThirdBone)
        {
            thirdZMax = 1.0471976f;
            thirdZMin = -1.0471976f;
        }
        break;
    case 1:
        headXOffset = 0.52359879f;
        break;
    case 2:
        headXOffset = 0.61086524f;
        headXMin = -0.78539819;
        headXMax = 0.12217305f;
        break;
    case 3:
        headXOffset = 0.37088248f;
        break;
    default:
        headXOffset = 0.0;
        break;
    }
    if (useGlobalLimits)
    {
        if (OBJ_gf_EYEFOLLOW_HeadZMax != 1000.0)
            headZMax = OBJ_gf_EYEFOLLOW_HeadZMax;
        if (OBJ_gf_EYEFOLLOW_HeadZMin != 1000.0)
            headZMin = OBJ_gf_EYEFOLLOW_HeadZMin;
        if (OBJ_gf_EYEFOLLOW_HeadXMax != 1000.0)
            headXMax = OBJ_gf_EYEFOLLOW_HeadXMax;
        if (OBJ_gf_EYEFOLLOW_HeadXMin != 1000.0)
            headXMin = OBJ_gf_EYEFOLLOW_HeadXMin;
        if (OBJ_gf_EYEFOLLOW_TorsoZMax != 1000.0)
            torsoZMax = OBJ_gf_EYEFOLLOW_TorsoZMax;
        if (OBJ_gf_EYEFOLLOW_TorsoZMin != 1000.0)
            torsoZMin = OBJ_gf_EYEFOLLOW_TorsoZMin;
        if (OBJ_gf_EYEFOLLOW_HeadXOffset != 1000.0)
            headXOffset = OBJ_gf_EYEFOLLOW_HeadXOffset;
    }
    headXMaxAbs = headXOffset + headXMax;
    headXMinAbs = (float)(headXOffset + headXMin);
    if (unlimited)
    {
        headZMax = 3.1415927f;
        torsoZMax = 3.1415927f;
        headZMin = -3.1415927f;
        torsoZMin = -3.1415927f;
        headXMaxAbs = 3.1415927f;
        headXMinAbs = -3.1415927f;
    }

    OBJ_SightGet(gao, &gaoSight);
    boneMtx = &head->basic->mtx->mtx;
    headPosX = boneMtx->Pos.x;
    headPosY = boneMtx->Pos.y;
    headPosZ = boneMtx->Pos.z;
    MATH_GetScale(&headScale, boneMtx);
    MATH_GetScale(&torsoScale, &torso->basic->mtx->mtx);
    boneMtx = &torso->basic->mtx->mtx;
    torsoPosX = boneMtx->Pos.x;
    torsoPosY = boneMtx->Pos.y;
    torsoPosZ = boneMtx->Pos.z;
    if (useThirdBone)
    {
        MATH_GetScale(&thirdScale, &third->basic->mtx->mtx);
        boneMtx = &third->basic->mtx->mtx;
        thirdPosX = boneMtx->Pos.x;
        thirdPosY = boneMtx->Pos.y;
        thirdPosZ = boneMtx->Pos.z;
    }

    OBJ_SightGet(rootBone, &dest);
    OBJ_ComputeGlobalWithLocal(rootBone, &globalMtx, 1);
    MATH_VecInvert(&sightTmp, &globalMtx.v2);
    rootZAngle = MATH_f_ZAngle(&dest, &sightTmp);
    OBJ_ComputeGlobalWithLocal(head, &globalMtx, 1);
    MATH_VecInvert(&sightTmp, &globalMtx.v2);
    headHorizon = globalMtx.v3;
    OBJ_SightGet(head, &dest);
    headZCurrent = MATH_f_ZAngle(&gaoSight, &dest);
    headXCurrent = MATH_f_XAngle(&gaoSight, &dest);
    if (rampNeeded || target->x == 0.0 && target->y == 0.0 && target->z == 0.0)
    {
        dest = sightTmp;
        headZTarget = MATH_f_ZAngle(&gaoSight, &dest);
        headXTarget = MATH_f_XAngle(&gaoSight, &dest);
        blendFull = 1;
    }
    else
    {
        OBJ_ComputeGlobalWithLocal(head, &headGlobalMtx, 1);
        MATH_SubstractVec(&dest, target, &headGlobalMtx.Pos);
        MATH_NormalizeEqualVector(&dest);
        headZTarget = MATH_f_ZAngle(&gaoSight, &dest);
        headXTarget = MATH_f_XAngle(&gaoSight, &dest);
        if (!clampToSight || headZMax + 0.17453294 >= headZTarget && headZMin - 0.17453294 <= headZTarget)
        {
            headXTarget = (float)(headXTarget + headXOffset);
        }
        else
        {
            headZTarget = MATH_f_ZAngle(&gaoSight, &sightTmp);
            headXTarget = MATH_f_XAngle(&gaoSight, &sightTmp);
            blendFull = 1;
        }
    }

    OBJ_ComputeGlobalWithLocal(torso, &globalMtx, 1);
    MATH_VecInvert(&dest, &globalMtx.v2);
    torsoHorizon = globalMtx.v3;
    OBJ_SightGet(torso, &sightTmp);
    OBJ_BankingGet(torso, &torsoBanking);
    torsoUpright = fabs(torsoBanking.z) < 0.050000001;
    torsoZCurrent = MATH_f_ZAngle(&gaoSight, &sightTmp);
    torsoXCurrent = MATH_f_XAngle(&gaoSight, &sightTmp);
    torsoXFromBlend = MATH_f_XAngle(&gaoSight, &dest);
    if (blendFull)
        torsoZTarget = MATH_f_ZAngle(&gaoSight, &dest);
    else if (torsoFollowsHead)
        torsoZTarget = headZTarget;
    else if (!torsoFollowsOwn)
        torsoZTarget = (float)(headZTarget * 0.33333334);
    else
        torsoZTarget = torsoZCurrent;

    if (useThirdBone)
    {
        OBJ_ComputeGlobalWithLocal(third, &globalMtx, 1);
        MATH_VecInvert(&dest, &globalMtx.v2);
        thirdHorizon = globalMtx.v3;
        OBJ_SightGet(third, &sightTmp);
        thirdZCurrent = MATH_f_ZAngle(&gaoSight, &sightTmp);
        thirdXCurrent = MATH_f_XAngle(&gaoSight, &sightTmp);
        thirdXFromBlend = MATH_f_XAngle(&gaoSight, &dest);
        if (!blendFull)
            zTmp = headZTarget * 0.2;
        else
            zTmp = MATH_f_ZAngle(&gaoSight, &dest);
        thirdZTarget = (float)zTmp;
    }

    compensateRoot = weight != 1.0
        && (rootZAngle == 0.0 || (headZTarget <= (double)headZCurrent || rootZAngle >= 0.0) && (headZTarget >= (double)headZCurrent || rootZAngle <= 0.0));
    if (clampToSight
        || (fabs(headZCurrent - headZMax) >= 0.001 || headZTarget >= (double)headZMin)
        && (fabs(headZCurrent - headZMin) >= 0.001 || headZTarget <= (double)headZMax))
    {
        if ((headZTarget <= 0.0 || headZCurrent >= (double)headZTarget || headZCurrent + rootZAngle <= headZTarget)
            && (headZTarget >= 0.0 || headZCurrent <= (double)headZTarget || headZCurrent + rootZAngle >= headZTarget))
        {
            if (compensateRoot)
                deltaTmp = rootZAngle;
            else
                deltaTmp = 0.0;
            headZDelta = (float)((headZTarget - headZCurrent) * weight + deltaTmp);
            limitTmp = headZDelta + headZCurrent;
            if (limitTmp > headZMax || limitTmp < headZMin)
            {
                if (limitTmp <= headZMax)
                    deltaTmp = headZMin - headZCurrent;
                else
                    deltaTmp = headZMax - headZCurrent;
                headZDelta = (float)deltaTmp;
            }
        }
        else
        {
            headZDelta = headZTarget - headZCurrent;
        }
        xTmp = (headXTarget - headXCurrent) * weight;
        headXDelta = (float)xTmp;
        limitTmp = xTmp + headXCurrent;
        if (limitTmp > headXMaxAbs || limitTmp < headXMinAbs)
        {
            if (limitTmp <= headXMaxAbs)
                deltaTmp = headXMinAbs - headXCurrent;
            else
                deltaTmp = headXMaxAbs - headXCurrent;
            headXDelta = (float)deltaTmp;
        }
        if (compensateRoot)
            deltaTmp = rootZAngle;
        else
            deltaTmp = 0.0;
        torsoZDelta = (torsoZTarget - torsoZCurrent) * weight + (float)deltaTmp;
        limitTmp = torsoZDelta + torsoZCurrent;
        if (limitTmp > torsoZMax || limitTmp < torsoZMin)
        {
            if (limitTmp <= torsoZMax)
                deltaTmp = torsoZMin - torsoZCurrent;
            else
                deltaTmp = torsoZMax - torsoZCurrent;
            torsoZDelta = (float)deltaTmp;
        }
        if (useThirdBone)
        {
            deltaTmp = compensateRoot ? rootZAngle : 0.0;
            thirdZDelta = (float)((thirdZTarget - thirdZCurrent) * weight + deltaTmp);
            limitTmp = thirdZDelta + thirdZCurrent;
            if (limitTmp > thirdZMax || limitTmp < thirdZMin)
            {
                if (limitTmp <= thirdZMax)
                    deltaTmp = thirdZMin - thirdZCurrent;
                else
                    deltaTmp = thirdZMax - thirdZCurrent;
                thirdZDelta = (float)deltaTmp;
            }
        }
    }
    else
    {
        headZDelta = 0.0;
        headXDelta = 0.0;
        torsoZDelta = 0.0;
        thirdZDelta = 0.0;
    }

    if (fabs(headZDelta) >= 0.0000099999997)
        MATH_RotateMatrix_AroundGlobalZAxis(head->matrices, headZDelta);
    if (fabs(headXDelta) >= 0.0000099999997)
        MATH_RotateMatrix_AroundLocalXAxis(head->matrices, headXDelta);
    OBJ_SightGet(head, &headSight2);
    OBJ_SightGeneralSet(head, &headSight2, &headHorizon);
    if (!torsoUpright)
    {
        if (fabs(torsoZDelta) >= 0.0000099999997)
            MATH_RotateMatrix_AroundGlobalZAxis(torso->matrices, torsoZDelta);
        MATH_RotateMatrix_AroundLocalXAxis(torso->matrices, torsoXFromBlend - torsoXCurrent);
        OBJ_SightGet(torso, &torsoSight2);
        OBJ_SightGeneralSet(torso, &torsoSight2, &torsoHorizon);
    }
    if (useThirdBone)
    {
        if (fabs(thirdZDelta) >= 0.0000099999997)
            MATH_RotateMatrix_AroundGlobalZAxis(third->matrices, thirdZDelta);
        MATH_RotateMatrix_AroundLocalXAxis(third->matrices, thirdXFromBlend - thirdXCurrent);
        OBJ_SightGet(third, &thirdSight);
        OBJ_SightGeneralSet(third, &thirdSight, &thirdHorizon);
        // preserved: the getter is null-safe and all three of its results here are dereferenced unchecked
        parent = get_reference_point_gao_2_from_gao_sub_40BEA0(third);
        OBJ_ComputeGlobalWithLocal(parent, parent->matrices, 1);
        OBJ_ComputeLocalWhenHie(third);
        bonePos = &third->basic->mtx->mtx.Pos;
        bonePos->x = thirdPosX;
        bonePos->y = thirdPosY;
        bonePos->z = thirdPosZ;
    }
    parent = get_reference_point_gao_2_from_gao_sub_40BEA0(torso);
    OBJ_ComputeGlobalWithLocal(parent, parent->matrices, 1);
    if (torsoUpright)
        OBJ_ComputeGlobalWhenHie(torso);
    else
        OBJ_ComputeLocalWhenHie(torso);
    bonePos = &torso->basic->mtx->mtx.Pos;
    bonePos->x = torsoPosX;
    bonePos->y = torsoPosY;
    bonePos->z = torsoPosZ;
    parent = get_reference_point_gao_2_from_gao_sub_40BEA0(head);
    OBJ_ComputeGlobalWithLocal(parent, parent->matrices, 1);
    OBJ_ComputeLocalWhenHie(head);
    localMtx = OBJ_pst_GetLocalMatrix(head);
    MATH_SetScale(&localMtx->mtx, &headScale);
    localMtx = OBJ_pst_GetLocalMatrix(torso);
    MATH_SetScale(&localMtx->mtx, &torsoScale);
    if (useThirdBone)
    {
        localMtx = OBJ_pst_GetLocalMatrix(third);
        MATH_SetScale(&localMtx->mtx, &thirdScale);
    }
    bonePos = &head->basic->mtx->mtx.Pos;
    bonePos->x = headPosX;
    bonePos->y = headPosY;
    bonePos->z = headPosZ;
}

void OBJ_FreezeModifier_C(struct sGameObjectMain* gao, int mdfId, int setOrReset)
{
    unsigned int modifierFlags;
    tModifier* mdf;

    mdf = MDF_pst_GetByType(gao, mdfId);
    if (mdf)
    {
        if (setOrReset)
            modifierFlags = mdf->modifierFlags | 0x2000000;
        else
            modifierFlags = mdf->modifierFlags & ~0x2000000u;
        mdf->modifierFlags = modifierFlags;
    }
}

BOOL OBJ_ModifierType_C(struct sGameObjectMain* gao, int mdfId)
{
    return MDF_pst_GetByType(gao, mdfId) != 0;
}

struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* gao)
{
    tGroGeneric* result;
    int id;

    if (!gao
        || (gao->flags & 0x4000) == 0
        || (result = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) == 0
        || (id = result->intf->id, id != 1)
        && (id != 8
            || (result = (tGroGeneric*)GEO_pst_StaticLOD_GetLOD((tGEOSTATICLOD*)result, ((tGEOSTATICLOD*)(result))->currDistance)) == 0
            || result->intf->id != 1))
    {
        result = 0;
    }
    return (tGEOGRO*)result;
}

int OBJ_Frame_CullingSphere(Vec3* center, float radius, struct sDDCamStruct* cam, int width, int height, float* outClipped, float* outScreen)
{
    double dx;
    double dz;
    double dy;
    long double dist;
    double fwdX;
    double fwdY;
    double rightX;
    double rightY;
    double maxYTmp;
    float centerY;
    float* cursor;
    int remaining;
    float clampTmp;
    float halfWidth;
    float halfHeight;
    float negHalfWidth;
    float maxX;
    float negHalfHeight;
    float minX;
    float minY;
    Vec3 fwdPoint;
    float unclampedMinX;
    float unclampedMaxX;
    float unclampedMinY;
    float unclampedMaxY;
    float pointX;
    float pointY;
    float projRadius;
    Vec3 projCenter;
    float centerX;
    Vec3 edgePoint;
    Vec3 projEdge;
    Vec3 projFwd;
    float corners[4][3];

    halfWidth = (float)((double)width * 0.5);
    halfHeight = (float)((double)height * 0.5);
    if (outClipped)
        *outClipped = 0.0;
    if (outScreen)
        *outScreen = 0.0;
    cam->viewMtxPtr_setInGRORender = &cam->cameraMtx;
    dx = cam->objectMtx.Pos.x - center->x;
    dz = cam->objectMtx.Pos.z - center->z;
    dy = cam->objectMtx.Pos.y - center->y;
    dist = sqrt(dz * dz + dy * dy + dx * dx);
    fwdX = cam->objectMtx.v3.x;
    fwdY = cam->objectMtx.v3.y;
    fwdPoint.z = cam->objectMtx.v3.z;
    fwdPoint.x = (float)(fwdX * dist);
    fwdPoint.z = (float)(fwdPoint.z * dist);
    fwdPoint.x = fwdPoint.x + cam->objectMtx.Pos.x;
    fwdPoint.y = (float)(fwdY * dist + cam->objectMtx.Pos.y);
    fwdPoint.z = fwdPoint.z + cam->objectMtx.Pos.z;
    rightX = radius * cam->objectMtx.v1.x;
    rightY = cam->objectMtx.v1.y * radius;
    edgePoint.z = cam->objectMtx.v1.z * radius;
    edgePoint.x = (float)(rightX + fwdPoint.x);
    edgePoint.y = (float)(rightY + fwdPoint.y);
    edgePoint.z = edgePoint.z + fwdPoint.z;
    SOFT_TransformAndProject(&projEdge, &edgePoint, 1, cam);
    SOFT_TransformAndProject(&projFwd, &fwdPoint, 1, cam);
    SOFT_TransformAndProject(&projCenter, center, 1, cam);
    if (projCenter.x == 2147483600.0f || projCenter.y == 2147483600.0f) // TODO FIXME ez jó így?
        return 1;
    centerX = cam->centerX;
    projRadius = (float)sqrt((projEdge.y - projFwd.y) * (projEdge.y - projFwd.y) + (projEdge.x - projFwd.x) * (projEdge.x - projFwd.x));
    if (!(projRadius + halfWidth >= fabs(projCenter.x - centerX)) && !(projRadius + halfHeight >= fabs(projCenter.y - cam->centerY)))
        return 1;
    corners[0][0] = projRadius + projCenter.x;
    corners[0][1] = projRadius + projCenter.y;
    corners[1][0] = corners[0][0];
    corners[1][1] = projCenter.y - projRadius;
    corners[2][0] = projCenter.x - projRadius;
    corners[2][1] = corners[1][1];
    corners[3][0] = corners[2][0];
    corners[3][1] = corners[0][1];
    if (!outClipped && !outScreen)
        return 1;
    negHalfWidth = -halfWidth;
    maxX = negHalfWidth;
    minX = (float)((double)width * 0.5);
    maxYTmp = -halfHeight;
    unclampedMinX = minX;
    negHalfHeight = (float)maxYTmp;
    minY = (float)((double)height * 0.5);
    unclampedMaxX = negHalfWidth;
    unclampedMaxY = negHalfHeight;
    unclampedMinY = minY;
    centerY = cam->centerY;
    cursor = &corners[0][1];
    remaining = 4;
    do
    {
        pointX = *(cursor - 1) - centerX;
        pointY = *cursor - centerY;
        if (pointX < (double)minX)
        {
            clampTmp = *(cursor - 1) - centerX;
            if (pointX <= (double)negHalfWidth)
                clampTmp = -halfWidth;
            minX = clampTmp;
        }
        if (pointX > (double)maxX)
        {
            clampTmp = *(cursor - 1) - centerX;
            if (pointX >= (double)halfWidth)
                clampTmp = (float)((double)width * 0.5);
            maxX = clampTmp;
        }
        if (pointY < (double)minY)
        {
            clampTmp = *cursor - centerY;
            if (pointY <= (double)negHalfHeight)
                clampTmp = negHalfHeight;
            minY = clampTmp;
        }
        if (pointY > maxYTmp)
        {
            if (pointY >= (double)halfHeight)
                maxYTmp = halfHeight;
            else
                maxYTmp = pointY;
        }
        if (pointX < (double)unclampedMinX)
            unclampedMinX = *(cursor - 1) - centerX;
        if (pointX > (double)unclampedMaxX)
            unclampedMaxX = pointX;
        if (pointY < (double)unclampedMinY)
            unclampedMinY = pointY;
        if (pointY > (double)unclampedMaxY)
            unclampedMaxY = pointY;
        cursor += 3;
        --remaining;
    } while (remaining);
    if (outClipped)
        *outClipped = (float)((maxYTmp - minY) * (maxX - minX) / ((unclampedMaxY - unclampedMinY) * (unclampedMaxX - unclampedMinX)));
    if (outScreen)
    {
        //LODWORD(v43) = a5 * a4;
        *outScreen = (float)((unclampedMaxY - unclampedMinY) * (unclampedMaxX - unclampedMinX) / (double)(height * width));
    }
    return 1;
}

int OBJ_Frame_CullingAABBox(Vec3* min, Vec3* max, struct sDDCamStruct* cam, int width, int height, float* outClipped, float* outScreen)
{
    char sideY;
    uint8_t cornerIdx;
    long double dxWide;
    long double absDx;
    double edgeTmp;
    double maxYTmp;
    Vec3* corner;
    int remaining;
    float clampTmp;
    char sideX;
    float halfWidth;
    float halfHeight;
    float dx;
    float pointY;
    float pointX;
    float minX;
    float minY;
    float unclampedMinX;
    float negHalfHeight;
    float unclampedMaxX;
    float negHalfWidth;
    float unclampedMinY;
    float unclampedMaxY;
    float maxX;
    double dy;
    int measure;
    Vec3 corners[8];
    Vec3 projected[8];

    corners[0].x = max->x;
    corners[0].y = max->y;
    corners[0].z = max->z;
    corners[1].x = max->x;
    corners[1].y = max->y;
    corners[1].z = min->z;
    corners[2].x = max->x;
    corners[2].y = min->y;
    corners[2].z = max->z;
    corners[3].x = max->x;
    corners[3].y = min->y;
    corners[3].z = min->z;
    corners[4].x = min->x;
    corners[4].y = max->y;
    corners[4].z = max->z;
    corners[5].x = min->x;
    corners[5].y = max->y;
    corners[5].z = min->z;
    corners[6].x = min->x;
    corners[6].y = min->y;
    corners[6].z = max->z;
    corners[7].x = min->x;
    corners[7].y = min->y;
    corners[7].z = min->z;
    halfWidth = (float)((double)width * 0.5);
    halfHeight = (float)((double)height * 0.5);
    sideX = 0;
    sideY = 0;
    SOFT_TransformAndProject(projected, corners, 8, cam);
    measure = 0;
    for (cornerIdx = 0; cornerIdx < 8u; ++cornerIdx)
    {
        if ((*((uint32_t*)(&(projected[cornerIdx].x)))) == 0x4F000000)
            continue;
        if ((*((uint32_t*)(&(projected[cornerIdx].y)))) == 0x4F000000)
            continue;
        dxWide = projected[cornerIdx].x - cam->centerX;
        dx = (float)dxWide;
        absDx = fabs(dxWide);
        dy = projected[cornerIdx].y - cam->centerY;
        if (absDx < halfWidth && fabs(dy) < halfHeight)
        {
            if (!outClipped && !outScreen)
                return 1;
            measure = 1;
            break;
        }
        if (sideX == 1)
        {
            if (fabs(dy) < halfHeight && !((double)-halfWidth <= dx))
            {
                measure = 1;
                break;
            }
        }
        else if (sideX == 2 && fabs(dy) < halfHeight)
        {
            if (!(dx <= (double)halfWidth))
            {
                measure = 1;
                break;
            }
        }
        if (sideY == 1)
        {
            if (absDx < halfWidth && -halfHeight > dy)
            {
                measure = 1;
                break;
            }
        }
        else if (sideY == 2 && absDx < halfWidth && dy > halfHeight)
        {
            measure = 1;
            break;
        }
        if (!sideX)
        {
            sideX = 1;
            if (dx <= (double)halfWidth)
                sideX = 2;
            sideY = 1;
            if (dy <= halfHeight)
                sideY = 2;
        }
    }
    if (!measure)
    {
        if (outClipped)
            *outClipped = 0.0;
        if (outScreen)
            *outScreen = 0.0;
        return 1;
    }
    negHalfWidth = -halfWidth;
    maxX = negHalfWidth;
    minX = (float)((double)width * 0.5);
    maxYTmp = -halfHeight;
    unclampedMinX = minX;
    negHalfHeight = (float)maxYTmp;
    minY = (float)((double)height * 0.5);
    unclampedMaxX = negHalfWidth;
    unclampedMinY = minY;
    unclampedMaxY = negHalfHeight;
    corner = projected;
    remaining = 8;
    do
    {
        if ( (*((uint32_t *)(&(corner->x)))) != 0x4F000000 && (*((uint32_t*)(&(corner->y)))) != 0x4F000000) // TODO: jó ez így, eredetileg inként volt ellenõizve  0x4F000000 -re
        {
            pointX = corner->x - cam->centerX;
            pointY = corner->y - cam->centerY;
            if (pointX < (double)minX)
            {
                clampTmp = corner->x - cam->centerX;
                if (pointX <= (double)negHalfWidth)
                    clampTmp = -halfWidth;
                minX = clampTmp;
            }
            if (pointX > (double)maxX)
            {
                clampTmp = corner->x - cam->centerX;
                if (pointX >= (double)halfWidth)
                    clampTmp = (float)((double)width * 0.5);
                maxX = clampTmp;
            }
            if (pointY < (double)minY)
            {
                clampTmp = corner->y - cam->centerY;
                if (pointY <= (double)negHalfHeight)
                    clampTmp = negHalfHeight;
                minY = clampTmp;
            }
            if (pointY > maxYTmp)
            {
                if (pointY >= (double)halfHeight)
                    maxYTmp = halfHeight;
                else
                    maxYTmp = pointY;
            }
            if (pointX < (double)unclampedMinX)
                unclampedMinX = corner->x - cam->centerX;
            if (pointX > (double)unclampedMaxX)
                unclampedMaxX = corner->x - cam->centerX;
            if (pointY < (double)unclampedMinY)
                unclampedMinY = corner->y - cam->centerY;
            if (pointY > (double)unclampedMaxY)
                unclampedMaxY = corner->y - cam->centerY;
        }
        ++corner;
        --remaining;
    } while (remaining);
    if (outClipped)
        *outClipped = (float)((maxYTmp - minY) * (maxX - minX) / ((unclampedMaxY - unclampedMinY) * (unclampedMaxX - unclampedMinX)));
    if (!outScreen)
        return 1;
    *outScreen = (float)((maxYTmp - minY) * (maxX - minX) / (double)(height * width));
    return 1;
}

void OBJ_UpdateCullingVars(struct sDDCamStruct* cam)
{
    MATH_TransformVector(&g_stGlobalNormPlaneUp, &cam->objectMtx, &cam->planeUp);
    MATH_TransformVector(&g_stGlobalNormPlaneDown, &cam->objectMtx, &cam->planeDown);
    MATH_TransformVector(&g_stGlobalNormPlaneLeft, &cam->objectMtx, &cam->planeLeft);
    MATH_TransformVector(&g_stGlobalNormPlaneRight, &cam->objectMtx, &cam->planeRight);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneUp, &g_stGlobalNormPlaneUp, &g_stGlobalNormPlaneDown, 0.25);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneDown, &g_stGlobalNormPlaneUp, &g_stGlobalNormPlaneDown, 0.75);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneLeft, &g_stGlobalNormPlaneLeft, &g_stGlobalNormPlaneRight, 0.25);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneRight, &g_stGlobalNormPlaneLeft, &g_stGlobalNormPlaneRight, 0.75);
    MATH_NormalizeEqualVector(&g_stOverlayed_GlobalNormPlaneUp);   // MATH_NormalizeEqualVector x4
    MATH_NormalizeEqualVector(&g_stOverlayed_GlobalNormPlaneDown);
    MATH_NormalizeEqualVector(&g_stOverlayed_GlobalNormPlaneLeft);
    MATH_NormalizeEqualVector(&g_stOverlayed_GlobalNormPlaneRight);
    g_stGlobalCameraPos = cam->objectMtx.Pos;
    g_stGlobalCameraSight.x = cam->objectMtx.v3.x;
    g_stGlobalCameraSight.y = cam->objectMtx.v3.y;
    g_stGlobalCameraSight.z = cam->objectMtx.v3.z;
    if ((cam->flags & 1) != 0)
        g_fDistance = 0.0;
    else
        g_fDistance = -(cam->isoZoom * cam->isoFactor);
}

int OBJ_CullingAABBox(Vec3** box, uint8_t* outFlags)

{
    Vec3* swapped;

    if ((((g_fDistance <=
        g_stGlobalCameraSight.x * box[((*((uint32_t*)&(g_stGlobalCameraSight.x))) >> 0x1f)]->x +
        g_stGlobalCameraSight.y * (box[((*((uint32_t*)&(g_stGlobalCameraSight.y))) >> 0x1f)]->y) +
        g_stGlobalCameraSight.z * (box[((*((uint32_t*)&(g_stGlobalCameraSight.z))) >> 0x1f)]->z)) && //((*((uint32_t*)&(g_stGlobalNormPlaneDown.x))) >> 0x1f)
        (g_fDistance <=
            g_stGlobalNormPlaneLeft.x * box[((*((uint32_t*)&(g_stGlobalNormPlaneLeft.x))) >> 0x1f)]->x +
            g_stGlobalNormPlaneLeft.y * (box[((*((uint32_t*)&(g_stGlobalNormPlaneLeft.y))) >> 0x1f)]->y) +
            g_stGlobalNormPlaneLeft.z * (box[((*((uint32_t*)&(g_stGlobalNormPlaneLeft.z))) >> 0x1f)]->z)))
        && (g_fDistance <=
            g_stGlobalNormPlaneRight.x * box[((*((uint32_t*)&(g_stGlobalNormPlaneRight.x))) >> 0x1f)]->x +
            g_stGlobalNormPlaneRight.y * (box[((*((uint32_t*)&(g_stGlobalNormPlaneRight.y))) >> 0x1f)]->y) +
            g_stGlobalNormPlaneRight.z * (box[((*((uint32_t*)&(g_stGlobalNormPlaneRight.z))) >> 0x1f)]->z))) &&
        ((g_fDistance <=
            g_stGlobalNormPlaneUp.x * box[((*((uint32_t*)&(g_stGlobalNormPlaneUp.x))) >> 0x1f)]->x +
            g_stGlobalNormPlaneUp.y * (box[((*((uint32_t*)&(g_stGlobalNormPlaneUp.y))) >> 0x1f)]->y) +
            g_stGlobalNormPlaneUp.z * (box[((*((uint32_t*)&(g_stGlobalNormPlaneUp.z))) >> 0x1f)]->z) &&
            (g_fDistance <=
                g_stGlobalNormPlaneDown.x * box[((*((uint32_t*)&(g_stGlobalNormPlaneDown.x))) >> 0x1f)]->x +
                g_stGlobalNormPlaneDown.y * (box[((*((uint32_t*)&(g_stGlobalNormPlaneDown.y))) >> 0x1f)]->y) +
                g_stGlobalNormPlaneDown.z * (box[((*((uint32_t*)&(g_stGlobalNormPlaneDown.z))) >> 0x1f)]->z)))))
    {
        if (outFlags != NULL) {
            swapped = box[1];
            box[1] = box[0];
            box[0] = swapped;
            *outFlags = 0;
            if (g_stGlobalCameraSight.x * box[((*((uint32_t*)&(g_stGlobalCameraSight.x))) >> 0x1f)]->x +    //((*((uint32_t*)&(g_stGlobalCameraSight.x))) >> 0x1f)
                g_stGlobalCameraSight.y * (box[((*((uint32_t*)&(g_stGlobalCameraSight.y))) >> 0x1f)]->y) +
                g_stGlobalCameraSight.z * (box[((*((uint32_t*)&(g_stGlobalCameraSight.z))) >> 0x1f)]->z) < g_fDistance) {
                *outFlags = 1;
            }
            if (((g_stOverlayed_GlobalNormPlaneLeft.x * box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneLeft.x))) >> 0x1f)]->x + ////((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.x))) >> 0x1f)
                g_stOverlayed_GlobalNormPlaneLeft.y * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneLeft.y))) >> 0x1f)]->y) +
                g_stOverlayed_GlobalNormPlaneLeft.z * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneLeft.z))) >> 0x1f)]->z) < g_fDistance)
                || (g_stOverlayed_GlobalNormPlaneRight.x * box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneRight.x))) >> 0x1f)]->x +
                    g_stOverlayed_GlobalNormPlaneRight.y * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneRight.y))) >> 0x1f)]->y) +
                    g_stOverlayed_GlobalNormPlaneRight.z * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneRight.z))) >> 0x1f)]->z) < g_fDistance)) ||
                ((g_stOverlayed_GlobalNormPlaneUp.x * box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneUp.x))) >> 0x1f)]->x +
                    g_stOverlayed_GlobalNormPlaneUp.y * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneUp.y))) >> 0x1f)]->y) +
                    g_stOverlayed_GlobalNormPlaneUp.z *  (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneUp.z))) >> 0x1f)]->z) < g_fDistance ||
                    (g_stOverlayed_GlobalNormPlaneDown.x * box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.x))) >> 0x1f)]->x +
                        g_stOverlayed_GlobalNormPlaneDown.y * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.y))) >> 0x1f)]->y) +
                        g_stOverlayed_GlobalNormPlaneDown.z * (box[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.z))) >> 0x1f)]->z) < g_fDistance)))
                ) {
                *outFlags = *outFlags | 2;
            }
        }
        return 0;
    }
    return 1;
}

BOOL OBJ_CullingSphere(Vec3* center, float radius)

{
    float offX;
    float offY;
    float offZ;

    offX = radius;
    if (g_stGlobalNormPlaneLeft.x <= 0.0) {
        offX = -radius;
    }
    offY = radius;
    if (g_stGlobalNormPlaneLeft.y <= 0.0) {
        offY = -radius;
    }
    offZ = radius;
    if (g_stGlobalNormPlaneLeft.z <= 0.0) {
        offZ = -radius;
    }
    if ((g_stGlobalNormPlaneLeft.x * ((offX + center->x) - g_stGlobalCameraPos.x) +
        g_stGlobalNormPlaneLeft.y * ((offY + center->y) - g_stGlobalCameraPos.y) +
        g_stGlobalNormPlaneLeft.z * ((offZ + center->z) - g_stGlobalCameraPos.z) >= 0.0)) {
        offX = radius;
        if (g_stGlobalNormPlaneRight.x <= 0.0) {
            offX = -radius;
        }
        offY = radius;
        if (g_stGlobalNormPlaneRight.y <= 0.0) {
            offY = -radius;
        }
        offZ = radius;
        if (g_stGlobalNormPlaneRight.z <= 0.0) {
            offZ = -radius;
        }
        if ((g_stGlobalNormPlaneRight.x * ((offX + center->x) - g_stGlobalCameraPos.x) +
            g_stGlobalNormPlaneRight.y * ((offY + center->y) - g_stGlobalCameraPos.y) +
            g_stGlobalNormPlaneRight.z * ((offZ + center->z) - g_stGlobalCameraPos.z) >= 0.0)) {
            offX = radius;
            if (g_stGlobalNormPlaneUp.x <= 0.0) {
                offX = -radius;
            }
            offY = radius;
            if (g_stGlobalNormPlaneUp.y <= 0.0) {
                offY = -radius;
            }
            offZ = radius;
            if (g_stGlobalNormPlaneUp.z <= 0.0) {
                offZ = -radius;
            }
            if ((g_stGlobalNormPlaneUp.x * ((offX + center->x) - g_stGlobalCameraPos.x) +
                g_stGlobalNormPlaneUp.y * ((offY + center->y) - g_stGlobalCameraPos.y) +
                g_stGlobalNormPlaneUp.z * ((offZ + center->z) - g_stGlobalCameraPos.z) >= 0.0)) {
                offX = radius;
                if (g_stGlobalNormPlaneDown.x <= 0.0) {
                    offX = -radius;
                }
                offY = radius;
                if (g_stGlobalNormPlaneDown.y <= 0.0) {
                    offY = -radius;
                }
                if (g_stGlobalNormPlaneDown.z <= 0.0) {
                    radius = -radius;
                }
                if (0.0 <= g_stGlobalNormPlaneDown.x * ((offX + center->x) - g_stGlobalCameraPos.x) +
                    g_stGlobalNormPlaneDown.y * ((offY + center->y) - g_stGlobalCameraPos.y) +
                    g_stGlobalNormPlaneDown.z * ((radius + center->z) - g_stGlobalCameraPos.z)) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

BOOL OBJ_CullingObject(struct sGameObjectMain* gao)
{
    tGameObjectBV* bv;
    Mtx4x4* mtx;
    double posX;
    double relX;
    double relY;
    double posZ;
    double maxY;
    int visible;
    tGameObjectBasic* basic;
    tGameObjectActorObject* actor;
    tGameObjectVisualObject* visual;
    Mtx4x4* absMtx;
    Vec3 *box[2];
    float relZ;
    Vec3 boxMax;
    Vec3 boxMin;
    // preserved: OBJ_CullingAABBox writes this only when the box is inside, and it is stored either way
    uint8_t cullFlags;

    bv = gao->boundingVolume;
    if (OBJ_HasNoBV(gao))
        return 0;
    if (OBJ_BV_IsAABBox(bv))
    {
        mtx = gao->matrices;
        posX = mtx->Pos.x;
        mtx = (Mtx4x4*)((char*)mtx + 48);
        relX = posX - g_stGlobalCameraPos.x;
        relY = mtx->v1.y - g_stGlobalCameraPos.y;
        posZ = mtx->v1.z;
        box[0] = &boxMax;
        relZ = (float)(posZ - g_stGlobalCameraPos.z);
        boxMax.x = (float)(relX + bv->BVMax.x);
        maxY = bv->BVMax.y;
        box[1] = &boxMin;
        boxMax.y = (float)(maxY + relY);
        boxMax.z = bv->BVMax.z + relZ;
        boxMin.x = (float)(relX + bv->BVMin.x);
        boxMin.y = (float)(bv->BVMin.y + relY);
        boxMin.z = bv->BVMin.z + relZ;
        visible = OBJ_CullingAABBox(box, &cullFlags);
        if ((gao->flags & 2) != 0)
        {
            basic = gao->basic;
            if (basic)
            {
                actor = (tGameObjectActorObject*)basic->actorObjectOrVisualObject;
                if (actor)
                {
                    actor->culledFlagFromCullingObject = cullFlags;
                    return visible != 0;
                }
            }
        }
        if ((gao->flags & 0x4000) != 0)
        {
            basic = gao->basic;
            if (basic)
            {
                visual = (tGameObjectVisualObject*)basic->actorObjectOrVisualObject;
                if (visual)
                {
                    visual->culledFlagFromCullingObject = cullFlags;
                    return visible != 0;
                }
            }
        }
    }
    else
    {
        absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
        MATH_TransformVertexNoScale(&boxMin, absMtx, &bv->BVMin);
        visible = OBJ_CullingSphere(&boxMin, bv->BVMax.x);
    }
    return visible != 0;
}

BOOL OBJ_CullingPointInside(Vec3* point, Vec3* planeLeft, Vec3* planeUp, Vec3* planeRight, Vec3* planeDown, char flags)
{
    return ((flags & 1) == 0
        || planeLeft->z * (point->z - g_stGlobalCameraPos.z)
        + planeLeft->y * (point->y - g_stGlobalCameraPos.y)
        + (point->x - g_stGlobalCameraPos.x) * planeLeft->x < g_fDistance)
        && ((flags & 4) == 0
            || planeRight->z * (point->z - g_stGlobalCameraPos.z)
            + planeRight->y * (point->y - g_stGlobalCameraPos.y)
            + (point->x - g_stGlobalCameraPos.x) * planeRight->x < g_fDistance)
        && ((flags & 2) == 0
            || planeUp->z * (point->z - g_stGlobalCameraPos.z)
            + planeUp->y * (point->y - g_stGlobalCameraPos.y)
            + (point->x - g_stGlobalCameraPos.x) * planeUp->x < g_fDistance)
        && ((flags & 8) == 0
            || planeDown->z * (point->z - g_stGlobalCameraPos.z)
            + planeDown->y * (point->y - g_stGlobalCameraPos.y)
            + (point->x - g_stGlobalCameraPos.x) * planeDown->x < g_fDistance);
}

void OBJ_HierarchyMainCall(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* last;
    tGameObjectMain* gao;
    tGameObjectBasic* basic;
    int climbed;

    elem = world->eot.objectsWithHiearchyTablePF.firstElemPtr;
    last = ((tableElemStructPF*)(world->eot.objectsWithHiearchyTablePF.nextElemPtr)) - 1;
    for (; elem <= last; ++elem)
    {
        gao = (tGameObjectMain*)(elem->objectPtr);
        if (TAB_TableElemIsInvalid(gao))
            continue;
        if (OBJ_b_HasBeenProcess(gao, world->hierarchyCallCount, 0x80))
            continue;
        basic = gao->basic;
        climbed = 0;
        if (basic)
        {
            while (basic->mtx
                && basic->mtx->refGao2
                && !OBJ_b_HasBeenProcess(basic->mtx->refGao2, world->hierarchyCallCount, 0x80))
            {
                gao = basic->mtx->refGao2;
                basic = basic->mtx->refGao2->basic;
                climbed = 1;
                if (!basic)
                    break;
            }
            if (climbed)
                --elem;
        }
        OBJ_SetProcess(gao, world->hierarchyCallCount, 0x80);
        OBJ_ComputeGlobalWhenHie(gao);
    }
    ++world->hierarchyCallCount;
}

struct sGroGeneric* OBJ_p_GetGro(struct sGameObjectMain* gao)
{
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    tGroGeneric* result;

    if (gao
        && (gao->flags & 0x4000) != 0
        && (basic = gao->basic) != 0
        && (visual = (tGameObjectVisualObject*)basic->actorObjectOrVisualObject) != 0)
    {
        result = visual->GROGEOOrSomethingElse;
    }
    else
    {
        result = 0;
    }
    return result;
}

void OBJMY_ComputeLocalWhenHie(struct sGameObjectMain* gao)
{
    tGaoMatrix* local;
    Mtx4x4* parentMtx;
    Mtx4x4* absMtx;
    Mtx4x4 invParent;

    if ((gao->flags & 0x400000) != 0)
    {
        local = gao->basic->mtx;
        if (local->refGao2)
        {
            parentMtx = OBJ_pst_GetAbsoluteMatrix(local->refGao2);
            MATH_InvertMatrix(&invParent, parentMtx);
            absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
            MATH_MulMatrixMatrix(&local->mtx, absMtx, &invParent);
            MATH_Orthonormalize(&local->mtx);
        }
    }
}

struct sGEOMAT* OBJ_p_GetGrm(struct sGameObjectMain* gao)
{
    tGEOMAT* result;

    if ((gao->flags & 0x4000) != 0)
        result = (tGEOMAT*)(((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT);
    else
        result = 0;
    return result;
}

char OBJ_uc_CalcLODVis(struct sGameObjectMain* gao, struct sViewElem* view)
{
    tGdi_DisplayData* display;
    double dx;
    double dy;
    tGameObjectBV* bv;
    BOOL isSphere;
    double extentX;
    double extentY;
    double extentZ;
    double ratio;
    double lod;
    char result;
    Vec3 bvCenter;
    float distSq;
    float ratioF;
    float rounded;

    display = view->displayData;
    if (!display)
        return -1;
    OBJ_BV_ComputeCenter(gao, &bvCenter);
    dx = bvCenter.x - view->cameraMtx.Pos.x;
    dy = bvCenter.y - view->cameraMtx.Pos.y;
    bv = gao->boundingVolume;
    distSq = (float)((bvCenter.z - view->cameraMtx.Pos.z) * (bvCenter.z - view->cameraMtx.Pos.z) + dy * dy + dx * dx);
    isSphere = OBJ_BV_IsSphere(bv);
    extentX = bv->BVMax.x;
    if (isSphere)
    {
        ratio = extentX * extentX * (display->ct.cam.factorX * display->ct.cam.factorX) / distSq;
    }
    else
    {
        extentY = bv->BVMax.y - bv->BVMin.y;
        extentZ = bv->BVMax.z - bv->BVMin.z;
        ratio = (extentZ * extentZ + extentY * extentY + (extentX - bv->BVMin.x) * (extentX - bv->BVMin.x))
            * (display->ct.cam.factorX
                * display->ct.cam.factorX
                / distSq);
    }
    ratioF = (float)ratio;

    lod = MATH_GetSqrt(ratioF) * display->NormalizedWidthOrHeightMultBy255;

    if (lod >= 255.0)
        lod = 255.0;
    rounded = (float)(lod + 12533760.0);
    result = (*(int *)&rounded) & 0xff; //result = LOBYTE(v16);
    if (!result) //if (!LOBYTE(v16))
        result = 1;
    return result;
}
