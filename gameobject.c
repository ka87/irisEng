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

void __cdecl OBJ_MergeBVs(tGameObjectMain* dest, tGameObjectMain* src)
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

void OBJ_EyeFollow(struct sGameObjectMain* a1, Vec3* a2, float a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    tGameObjectActorObject* v10;
    tGroup* v11;
    tGameObjectMain* v12;
    tGameObjectMain* v13;
    Mtx4x4* v14;
    tGameObjectBasic* v15;
    tGameObjectActorObject* v16;
    int v17;
    tAction* v19;
    uint8_t v20;
    tActionElem* v21;
    int16_t v22;
    uint8_t v23;
    double v24;
    uint8_t v25;
    uint8_t v26;
    Mtx4x4* v27;
    float v28;
    Mtx4x4* v29;
    float v30;
    float v31;
    tGameObjectMain* v32;
    Mtx4x4* v33;
    float v34;
    float v35;
    double v36;
    double v37;
    double v38;
    double v39;
    BOOL v40;
    double v41;
    double v42;
    double v43;
    double v44;
    double v45;
    double v46;
    double v47;
    double v48;
    double v49;
    double v50;
    double v51;
    double v52;
    tGameObjectMain* v53;
    float v54;
    Vec3* v55;
    float v56;
    tGameObjectMain* v57;
    float v58;
    Vec3* v59;
    float v60;
    tGameObjectMain* v61;
    tGaoMatrix* v62;
    tGaoMatrix* v63;
    tGaoMatrix* v64;
    float v65;
    Vec3* v66;
    float v67;
    Vec3* v68;
    float v69;
    float v70;
    float v71;
    float angle;
    float anglea;
    float v74;
    float v75;
    float v76;
    float v77;
    float v78;
    float v79;
    float v80;
    float v81;
    float v82;
    BOOL v83;
    float v84;
    tGameObjectMain* v85;
    BOOL v86;
    tGameObjectMain* v87 = NULL;
    float v88;
    float v89;
    Vec3 dest;
    float v91;
    float v92;
    float v93;
    Vec3 v1;
    float v95;
    Vec3 v2;
    float v97;
    float v98;
    float v99;
    float v100;
    float v101;
    float v102;
    float v103;
    float v104;
    float v105;
    float v106;
    float v107;
    float v108;
    float v109;
    float v110;
    float v111;
    Vec3 a3a;
    Vec3 v113;
    Vec3 v114;
    Mtx4x4 v115;
    Vec3 v116;
    Vec3 v117;
    Vec3 v118;
    Vec3 v119;
    Vec3 a2a;
    Vec3 v121;
    Vec3 v122;
    Vec3 scale;
    Mtx4x4 v124;
    tAction** actions;
    tActionElem* actionElems;
    tActionElem* actionElems2;
    float v85a;

    if ((a1->flags & 2) != 0)
    {
        v10 = a1->basic->actorObjectOrVisualObject;
        v11 = v10->skeletonGroup;
        if (v11)
        {
            if (v11->tablePF && v10->actionKit && v10->actionPtr)
            {
                v12 = ANI_pst_GetObjectByAICanal(a1, 0);
                v13 = ANI_pst_GetObjectByAICanal(a1, 1u);
                if (a10)
                    v87 = ANI_pst_GetObjectByAICanal(a1, 12u);
                v85 = (tGameObjectMain*)(((tableElemStructPF*)(((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup->tablePF->firstElemPtr))->objectPtr);
                if (v12)
                {
                    if (v13 && v85)
                    {
                        v14 = OBJ_pst_GetAbsoluteMatrix(a1);
                        MATH_GetScale(&scale, v14);
                        v15 = a1->basic;
                        v16 = v15->actorObjectOrVisualObject;
                        v17 = ((tGameObjectActorObject*)(v15->actorObjectOrVisualObject))->actionItem;
                        v89 = 0.0;
                        if (((tGameObjectActorObject*)(v15->actorObjectOrVisualObject))->flagsEgBlendIw2 == 1)
                        {
                            v19 = v16->actionPtr;
                            actionElems = (tActionElem*)(((char*)v19) + sizeof(tAction));
                            v20 = actionElems[v17].customBits;
                            v21 = &(actionElems[v17]);
                            v83 = (v20 & 4) == 0;
                            if ((v20 & 4) == 0)
                                goto LABEL_13;
                        }
                        else
                        {
                            v19 = v16->actionPtr;
                            actionElems = (tActionElem*)(((char*)v19) + sizeof(tAction));
                            v21 = &(actionElems[v17]);
                            actions = (tAction**)(((char*)(v16->actionKit)) + sizeof(tActionKit));
                            actionElems2 = (tActionElem*)(((char*)(actions[v16->animPtr[1]->actionForTransition])) + sizeof(tAction));
                            if ((v21->customBits & 4) == 0
                                || (actionElems2[v16->animPtr[1]->actionElemForTransition].customBits & 4) == 0)
                            {
                                v83 = 1;
                            LABEL_13:
                                v22 = v21->designFlags;
                                if (v22 >= 0)
                                {
                                    if ((v22 & 0x1000) != 0)
                                        return;
                                    v25 = v16->unkb;
                                    v26 = v25 >= 6u ? 6 : v25 + 1;
                                    v16->unkb = v26;
                                    a3 = 0.40000001f;
                                    if (v26 >= 6u)
                                        return;
                                }
                                else
                                {
                                    if (v16->unkb >= 16u)
                                        v23 = 16;
                                    else
                                        v23 = (uint8_t)(v16->unkb + (uint64_t)(TIM_gf_dt * 60.000004 + 0.5));
                                    v16->unkb = v23;
                                    a3 = (float)((double)v23 * 0.0625);
                                    if (v23 >= 16u)
                                        return;
                                }
                            LABEL_20:
                                v81 = 1.5707964f;
                                v80 = 0.78539819f;
                                if (a10)
                                    v95 = 0.78539819f;
                                v74 = -1.5707964f;
                                v92 = -0.78539819f;
                                if (a10)
                                    v98 = -0.78539819f;
                                v24 = -0.78539819;
                                v76 = 0.43633232f;
                                switch (a4)
                                {
                                case 0:
                                    angle = 0.0;
                                    v81 = 2.0943952f;
                                    v74 = -2.0943952f;
                                    v80 = 1.0471976f;
                                    v92 = -1.0471976f;
                                    if (a10)
                                    {
                                        v95 = 1.0471976f;
                                        v98 = -1.0471976f;
                                    }
                                    break;
                                case 1:
                                    angle = 0.52359879f;
                                    break;
                                case 2:
                                    angle = 0.61086524f;
                                    v24 = -0.78539819;
                                    v76 = 0.12217305f;
                                    break;
                                case 3:
                                    angle = 0.37088248f;
                                    break;
                                default:
                                    angle = 0.0;
                                    break;
                                }
                                if (a9)
                                {
                                    if (OBJ_gf_EYEFOLLOW_HeadZMax != 1000.0)
                                        v81 = OBJ_gf_EYEFOLLOW_HeadZMax;
                                    if (OBJ_gf_EYEFOLLOW_HeadZMin != 1000.0)
                                        v74 = OBJ_gf_EYEFOLLOW_HeadZMin;
                                    if (OBJ_gf_EYEFOLLOW_HeadXMax != 1000.0)
                                        v76 = OBJ_gf_EYEFOLLOW_HeadXMax;
                                    if (OBJ_gf_EYEFOLLOW_HeadXMin != 1000.0)
                                        v24 = OBJ_gf_EYEFOLLOW_HeadXMin;
                                    if (OBJ_gf_EYEFOLLOW_TorsoZMax != 1000.0)
                                        v80 = OBJ_gf_EYEFOLLOW_TorsoZMax;
                                    if (OBJ_gf_EYEFOLLOW_TorsoZMin != 1000.0)
                                        v92 = OBJ_gf_EYEFOLLOW_TorsoZMin;
                                    if (OBJ_gf_EYEFOLLOW_HeadXOffset != 1000.0)
                                        angle = OBJ_gf_EYEFOLLOW_HeadXOffset;
                                }
                                v77 = angle + v76;
                                v97 = (float)(angle + v24);
                                if (a6)
                                {
                                    v81 = 3.1415927f;
                                    v80 = 3.1415927f;
                                    v74 = -3.1415927f;
                                    v92 = -3.1415927f;
                                    v77 = 3.1415927f;
                                    v97 = -3.1415927f;
                                }
                                OBJ_SightGet(a1, &v1);
                                v27 = &v12->basic->mtx->mtx;
                                v28 = v27->Pos.x;
                                v104 = v27->Pos.y;
                                v103 = v28;
                                v105 = v27->Pos.z;
                                MATH_GetScale(&a2a, v27);
                                MATH_GetScale(&v117, &v13->basic->mtx->mtx);
                                v29 = &v13->basic->mtx->mtx;
                                v30 = v29->Pos.x;
                                v31 = v29->Pos.y;
                                v32 = v87;
                                v102 = v29->Pos.z;
                                v100 = v30;
                                v101 = v31;
                                if (a10)
                                {
                                    MATH_GetScale(&v118, &v87->basic->mtx->mtx);
                                    v33 = &v87->basic->mtx->mtx;
                                    v34 = v33->Pos.y;
                                    v109 = v33->Pos.x;
                                    v35 = v33->Pos.z;
                                    v110 = v34;
                                    v111 = v35;
                                }
                                OBJ_SightGet(v85, &dest);
                                OBJ_ComputeGlobalWithLocal(v85, &v115, 1);
                                MATH_VecInvert(&v2, &v115.v2);
                                v82 = MATH_f_ZAngle(&dest, &v2);
                                OBJ_ComputeGlobalWithLocal(v12, &v115, 1);
                                MATH_VecInvert(&v2, &v115.v2);
                                a3a = v115.v3;
                                OBJ_SightGet(v12, &dest);
                                v79 = MATH_f_ZAngle(&v1, &dest);
                                v88 = MATH_f_XAngle(&v1, &dest);
                                if (v83 || a2->x == 0.0 && a2->y == 0.0 && a2->z == 0.0)
                                {
                                    dest = v2;
                                    v36 = MATH_f_ZAngle(&v1, &dest);
                                    v68 = &dest;
                                }
                                else
                                {
                                    OBJ_ComputeGlobalWithLocal(v12, &v124, 1);
                                    MATH_SubstractVec(&dest, a2, &v124.Pos);
                                    MATH_NormalizeEqualVector(&dest);
                                    v71 = MATH_f_ZAngle(&v1, &dest);
                                    v93 = MATH_f_XAngle(&v1, &dest);
                                    if (!a5 || v81 + 0.17453294 >= v71 && v74 - 0.17453294 <= v71)
                                    {
                                        v37 = v93 + angle;
                                        goto LABEL_71;
                                    }
                                    v36 = MATH_f_ZAngle(&v1, &v2);
                                    v68 = &v2;
                                }
                                v71 = (float)v36;
                                v37 = MATH_f_XAngle(&v1, v68);
                                v89 = 1;
                            LABEL_71:
                                v93 = (float)v37;
                                OBJ_ComputeGlobalWithLocal(v13, &v115, 1);
                                MATH_VecInvert(&dest, &v115.v2);
                                v114 = v115.v3;
                                OBJ_SightGet(v13, &v2);
                                OBJ_BankingGet(v13, &v116);
                                v86 = fabs(v116.z) < 0.050000001;
                                v91 = MATH_f_ZAngle(&v1, &v2);
                                v99 = MATH_f_XAngle(&v1, &v2);
                                v108 = MATH_f_XAngle(&v1, &dest);
                                if (v89 != 0.0)
                                {
                                    v38 = MATH_f_ZAngle(&v1, &dest);
                                LABEL_78:
                                    v84 = (float)v38;
                                    goto LABEL_79;
                                }
                                if (a7)
                                {
                                    v84 = v71;
                                }
                                else
                                {
                                    if (!a8)
                                    {
                                        v38 = v71 * 0.33333334;
                                        goto LABEL_78;
                                    }
                                    v84 = v91;
                                }
                            LABEL_79:
                                if (a10)
                                {
                                    OBJ_ComputeGlobalWithLocal(v32, &v115, 1);
                                    MATH_VecInvert(&dest, &v115.v2);
                                    v113 = v115.v3;
                                    OBJ_SightGet(v32, &v2);
                                    v85a = MATH_f_ZAngle(&v1, &v2);
                                    v107 = MATH_f_XAngle(&v1, &v2);
                                    v106 = MATH_f_XAngle(&v1, &dest);
                                    if (v89 == 0.0)
                                        v39 = v71 * 0.2;
                                    else
                                        v39 = MATH_f_ZAngle(&v1, &dest);
                                    v89 = (float)v39;
                                }
                                v40 = a3 != 1.0
                                    && (v82 == 0.0 || (v71 <= (double)v79 || v82 >= 0.0) && (v71 >= (double)v79 || v82 <= 0.0));
                                if (a5
                                    || (fabs(v79 - v81) >= 0.001 || v71 >= (double)v74)
                                    && (fabs(v79 - v74) >= 0.001 || v71 <= (double)v81))
                                {
                                    if ((v71 <= 0.0 || v79 >= (double)v71 || v79 + v82 <= v71)
                                        && (v71 >= 0.0 || v79 <= (double)v71 || v79 + v82 >= v71))
                                    {
                                        if (v40)
                                            v41 = v82;
                                        else
                                            v41 = 0.0;
                                        anglea = (float)((v71 - v79) * a3 + v41);
                                        v42 = anglea + v79;
                                        if (v42 > v81 || v42 < v74)
                                        {
                                            if (v42 <= v81)
                                                v43 = v74 - v79;
                                            else
                                                v43 = v81 - v79;
                                            anglea = (float)v43;
                                        }
                                    }
                                    else
                                    {
                                        anglea = v71 - v79;
                                    }
                                    v44 = (v93 - v88) * a3;
                                    v75 = (float)v44;
                                    v45 = v44 + v88;
                                    if (v45 > v77 || v45 < v97)
                                    {
                                        if (v45 <= v77)
                                            v46 = v97 - v88;
                                        else
                                            v46 = v77 - v88;
                                        v75 = (float)v46;
                                    }
                                    if (v40)
                                        v47 = v82;
                                    else
                                        v47 = 0.0;
                                    v78 = (v84 - v91) * a3 + (float)v47;
                                    v48 = v78 + v91;
                                    if (v48 > v80 || v48 < v92)
                                    {
                                        if (v48 <= v80)
                                            v49 = v92 - v91;
                                        else
                                            v49 = v80 - v91;
                                        v78 = (float)v49;
                                    }
                                    if (a10)
                                    {
                                        v50 = v40 ? v82 : 0.0;
                                        v80 = (float)((v89 - v85a) * a3 + v50);
                                        v51 = v80 + v85a;
                                        if (v51 > v95 || v51 < v98)
                                        {
                                            if (v51 <= v95)
                                                v52 = v98 - v85a;
                                            else
                                                v52 = v95 - v85a;
                                            v80 = (float)v52;
                                        }
                                    }
                                }
                                else
                                {
                                    anglea = 0.0;
                                    v75 = 0.0;
                                    v78 = 0.0;
                                    v80 = 0.0;
                                }
                                if (fabs(anglea) >= 0.0000099999997)
                                    MATH_RotateMatrix_AroundGlobalZAxis(v12->matrices, anglea);
                                if (fabs(v75) >= 0.0000099999997)
                                    MATH_RotateMatrix_AroundLocalXAxis(v12->matrices, v75);
                                OBJ_SightGet(v12, &v121);
                                OBJ_SightGeneralSet(v12, &v121, &a3a);
                                if (!v86)
                                {
                                    if (fabs(v78) >= 0.0000099999997)
                                        MATH_RotateMatrix_AroundGlobalZAxis(v13->matrices, v78);
                                    v69 = v108 - v99;
                                    MATH_RotateMatrix_AroundLocalXAxis(v13->matrices, v69);
                                    OBJ_SightGet(v13, &v122);
                                    OBJ_SightGeneralSet(v13, &v122, &v114);
                                }
                                if (a10)
                                {
                                    if (fabs(v80) >= 0.0000099999997)
                                        MATH_RotateMatrix_AroundGlobalZAxis(v32->matrices, v80);
                                    v70 = v106 - v107;
                                    MATH_RotateMatrix_AroundLocalXAxis(v32->matrices, v70);
                                    OBJ_SightGet(v32, &v119);
                                    OBJ_SightGeneralSet(v32, &v119, &v113);
                                    v53 = get_reference_point_gao_2_from_gao_sub_40BEA0(v32);
                                    OBJ_ComputeGlobalWithLocal(v53, v53->matrices, 1);
                                    OBJ_ComputeLocalWhenHie(v32);
                                    v54 = v110;
                                    v55 = &v32->basic->mtx->mtx.Pos;
                                    v55->x = v109;
                                    v56 = v111;
                                    v55->y = v54;
                                    v55->z = v56;
                                }
                                v57 = get_reference_point_gao_2_from_gao_sub_40BEA0(v13);
                                OBJ_ComputeGlobalWithLocal(v57, v57->matrices, 1);
                                if (v86)
                                    OBJ_ComputeGlobalWhenHie(v13);
                                else
                                    OBJ_ComputeLocalWhenHie(v13);
                                v58 = v101;
                                v59 = &v13->basic->mtx->mtx.Pos;
                                v59->x = v100;
                                v60 = v102;
                                v59->y = v58;
                                v59->z = v60;
                                v61 = get_reference_point_gao_2_from_gao_sub_40BEA0(v12);
                                OBJ_ComputeGlobalWithLocal(v61, v61->matrices, 1);
                                OBJ_ComputeLocalWhenHie(v12);
                                v62 = OBJ_pst_GetLocalMatrix(v12);
                                MATH_SetScale(&v62->mtx, &a2a);
                                v63 = OBJ_pst_GetLocalMatrix(v13);
                                MATH_SetScale(&v63->mtx, &v117);
                                if (a10)
                                {
                                    v64 = OBJ_pst_GetLocalMatrix(v32);
                                    MATH_SetScale(&v64->mtx, &v118);
                                }
                                v65 = v104;
                                v66 = &v12->basic->mtx->mtx.Pos;
                                v66->x = v103;
                                v67 = v105;
                                v66->y = v65;
                                v66->z = v67;
                                return;
                            }
                            v83 = 0;
                        }
                        v16->unkb = 0;
                        goto LABEL_20;
                    }
                }
            }
        }
    }
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

int OBJ_Frame_CullingSphere(Vec3* a1, float a2, struct sDDCamStruct* a3, int a4, int a5, float* a6, float* a7)
{
    double v7;
    double v8;
    double v9;
    long double v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    float v16;
    float* v17;
    int v18;
    float v19;
    float v20;
    float v21;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    Vec3 v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    Vec3 v44;
    float v45;
    Vec3 a2a;
    Vec3 a1a;
    Vec3 v48;
    float v49[12];

    v27 = (float)((double)a4 * 0.5);
    v28 = (float)((double)a5 * 0.5);
    if (a6)
        *a6 = 0.0;
    if (a7)
        *a7 = 0.0;
    a3->viewMtxPtr_setInGRORender = &a3->cameraMtx;
    v7 = a3->objectMtx.Pos.x - a1->x;
    v8 = a3->objectMtx.Pos.z - a1->z;
    v9 = a3->objectMtx.Pos.y - a1->y;
    v10 = sqrt(v8 * v8 + v9 * v9 + v7 * v7);
    v11 = a3->objectMtx.v3.x;
    v12 = a3->objectMtx.v3.y;
    v34.z = a3->objectMtx.v3.z;
    v34.x = (float)(v11 * v10);
    v34.z = (float)(v34.z * v10);
    v34.x = v34.x + a3->objectMtx.Pos.x;
    v34.y = (float)(v12 * v10 + a3->objectMtx.Pos.y);
    v34.z = v34.z + a3->objectMtx.Pos.z;
    v13 = a2 * a3->objectMtx.v1.x;
    v14 = a3->objectMtx.v1.y * a2;
    a2a.z = a3->objectMtx.v1.z * a2;
    a2a.x = (float)(v13 + v34.x);
    a2a.y = (float)(v14 + v34.y);
    a2a.z = a2a.z + v34.z;
    SOFT_TransformAndProject(&a1a, &a2a, 1, a3);
    SOFT_TransformAndProject(&v48, &v34, 1, a3);
    SOFT_TransformAndProject(&v44, a1, 1, a3);
    if (v44.x != 2147483600.0f && v44.y != 2147483600.0f) // TODO FIXME ez jó így?
    {
        v25 = a3->centerX;
        v23 = (float)sqrt((a1a.y - v48.y) * (a1a.y - v48.y) + (a1a.x - v48.x) * (a1a.x - v48.x));
        if (v23 + v27 >= fabs(v44.x - v25) || v23 + v28 >= fabs(v44.y - a3->centerY))
        {
            v49[0] = v23 + v44.x;
            v49[1] = v23 + v44.y;
            v49[3] = v49[0];
            v49[4] = v44.y - v23;
            v49[6] = v44.x - v23;
            v49[7] = v49[4];
            v49[9] = v49[6];
            v49[10] = v49[1];
            if (a6 || a7)
            {
                v29 = -v27;
                v30 = v29;
                v32 = (float)((double)a4 * 0.5);
                v15 = -v28;
                v39 = v32;
                v31 = (float)v15;
                v33 = (float)((double)a5 * 0.5);
                v36 = v29;
                v40 = v33;
                v42 = v31;
                v35 = v32;
                v41 = v29;
                v38 = v33;
                v16 = a3->centerY;
                v37 = v31;
                v45 = v25;
                v43 = v16;
                v17 = &v49[1];
                v18 = 4;
                do
                {
                    v26 = *(v17 - 1) - v45;
                    v24 = *v17 - v43;
                    if (v26 < (double)v32)
                    {
                        v19 = *(v17 - 1) - v45;
                        if (v26 <= (double)v29)
                            v19 = -v27;
                        v32 = v19;
                    }
                    if (v26 > (double)v30)
                    {
                        v20 = *(v17 - 1) - v45;
                        if (v26 >= (double)v27)
                            v20 = (float)((double)a4 * 0.5);
                        v30 = v20;
                    }
                    if (v24 < (double)v33)
                    {
                        v21 = *v17 - v43;
                        if (v24 <= (double)v31)
                            v21 = v31;
                        v33 = v21;
                    }
                    if (v24 > v15)
                    {
                        if (v24 >= (double)v28)
                            v15 = v28;
                        else
                            v15 = v24;
                    }
                    if (v26 < (double)v39)
                        v39 = *(v17 - 1) - v45;
                    if (v26 > (double)v36)
                        v36 = v26;
                    if (v24 < (double)v40)
                        v40 = v24;
                    if (v24 > (double)v42)
                        v42 = v24;
                    if (v26 < (double)v35)
                        v35 = v26;
                    if (v26 > (double)v41)
                        v41 = v26;
                    if (v24 < (double)v38)
                        v38 = v24;
                    if (v24 > (double)v37)
                        v37 = v24;
                    v17 += 3;
                    --v18;
                } while (v18);
                if (a6)
                    *a6 = (float)((v15 - v33) * (v30 - v32) / ((v37 - v38) * (v41 - v35)));
                if (a7)
                {
                    //LODWORD(v43) = a5 * a4;
                    *a7 = (float)((v42 - v40) * (v36 - v39) / (double)(a5 * a4));
                }
            }
        }
    }
    return 1;
}

int OBJ_Frame_CullingAABBox(Vec3* a1, Vec3* a2, struct sDDCamStruct* a3, int a4, int a5, float* a6, float* a7)
{
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    char v12;
    uint8_t v13;
    int v14;
    float* v15;
    long double v16;
    long double v17;
    double v18;
    BOOL v19;
    BOOL v20;
    int result;
    double v22;
    Vec3* v23;
    int v24;
    float v25;
    float v26;
    float v27;
    char v28;
    uint8_t v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    Vec3 a2a[8];
    Vec3 a1a[8];

    v7 = a2->x;
    a2a[0].y = a2->y;
    a2a[1].x = a2->x;
    a2a[0].x = v7;
    a2a[0].z = a2->z;
    a2a[1].y = a2->y;
    a2a[1].z = a1->z;
    a2a[2].x = a2->x;
    a2a[2].y = a1->y;
    a2a[2].z = a2->z;
    a2a[3].x = a2->x;
    a2a[3].y = a1->y;
    a2a[3].z = a1->z;
    a2a[4].x = a1->x;
    a2a[4].y = a2->y;
    a2a[4].z = a2->z;
    a2a[5].x = a1->x;
    v8 = a2->z;
    a2a[5].y = a2->y;
    a2a[5].z = a1->z;
    a2a[6].x = a1->x;
    v30 = (float)((double)a4 * 0.5);
    a2a[6].y = a1->y;
    v9 = a1->x;
    a2a[6].z = v8;
    v10 = a1->y;
    v11 = a1->z;
    a2a[7].x = v9;
    a2a[7].y = v10;
    v31 = (float)((double)a5 * 0.5);
    a2a[7].z = v11;
    v28 = 0;
    v12 = 0;
    SOFT_TransformAndProject(a1a, a2a, 8, a3);
    v13 = 0;
    v29 = 0;
    while (1)
    {
        v14 = v13;
        if ((*((uint32_t*)(&(a1a[v14].x)))) == 0x4F000000)
            goto LABEL_25;
        v15 = &a1a[v14].y;
        if ((*((uint32_t*)(&(a1a[v14].y)))) == 0x4F000000)
            goto LABEL_25;
        v16 = a1a[v14].x - a3->centerX;
        v32 = (float)v16;
        v17 = fabs(v16);
        if (v17 < v30 && fabs(*v15 - a3->centerY) < v31)
            break;
        if (v28 == 1)
        {
            if (fabs(*v15 - a3->centerY) < v31)
            {
                v18 = -v30;
                v19 = v18 < v32;
                v20 = v18 == v32;
                goto LABEL_12;
            }
        }
        else if (v28 == 2 && fabs(*v15 - a3->centerY) < v31)
        {
            v19 = v32 < (double)v30;
            v20 = v32 == v30;
        LABEL_12:
            if (!v19 && !v20)
                goto LABEL_31;
            goto LABEL_13;
        }
    LABEL_13:
        if (v12 == 1)
        {
            if (v17 < v30 && -v31 > * v15 - a3->centerY)
                goto LABEL_31;
        }
        else if (v12 == 2 && v17 < v30 && *v15 - a3->centerY > v31)
        {
            goto LABEL_31;
        }
        if (!v28)
        {
            v28 = 1;
            if (v32 <= (double)v30)
                v28 = 2;
            v12 = 1;
            if (*v15 - a3->centerY <= v31)
                v12 = 2;
        }
    LABEL_25:
        v13 = ++v29;
        if (v29 >= 8u)
        {
            if (a6)
                *a6 = 0.0;
            if (a7)
                *a7 = 0.0;
            return 1;
        }
    }
    if (!a6 && !a7)
        return 1;
LABEL_31:
    v41 = -v30;
    v36 = v41;
    v33 = (float)((double)a4 * 0.5);
    v22 = -v31;
    v38 = v33;
    v39 = (float)v22;
    v37 = (float)((double)a5 * 0.5);
    v40 = v41;
    v42 = v37;
    v43 = v39;
    v23 = a1a;
    v24 = 8;
    do
    {
        if ( (*((uint32_t *)(&(v23->x)))) != 0x4F000000 && (*((uint32_t*)(&(v23->y)))) != 0x4F000000) // TODO: jó ez így, eredetileg inként volt ellenõizve  0x4F000000 -re
        {
            v35 = v23->x - a3->centerX;
            v34 = v23->y - a3->centerY;
            if (v35 < (double)v33)
            {
                v25 = v23->x - a3->centerX;
                if (v35 <= (double)v41)
                    v25 = -v30;
                v33 = v25;
            }
            if (v35 > (double)v36)
            {
                v26 = v23->x - a3->centerX;
                if (v35 >= (double)v30)
                    v26 = (float)((double)a4 * 0.5);
                v36 = v26;
            }
            if (v34 < (double)v37)
            {
                v27 = v23->y - a3->centerY;
                if (v34 <= (double)v39)
                    v27 = v39;
                v37 = v27;
            }
            if (v34 > v22)
            {
                if (v34 >= (double)v31)
                    v22 = v31;
                else
                    v22 = v34;
            }
            if (v35 < (double)v38)
                v38 = v23->x - a3->centerX;
            if (v35 > (double)v40)
                v40 = v23->x - a3->centerX;
            if (v34 < (double)v42)
                v42 = v23->y - a3->centerY;
            if (v34 > (double)v43)
                v43 = v23->y - a3->centerY;
        }
        ++v23;
        --v24;
    } while (v24);
    if (a6)
        *a6 = (float)((v22 - v37) * (v36 - v33) / ((v43 - v42) * (v40 - v38)));
    if (!a7)
        return 1;
    result = 1;
    *a7 = (float)((v22 - v37) * (v36 - v33) / (double)(a5 * a4));
    return result;
}

void OBJ_UpdateCullingVars(struct sDDCamStruct* a1)
{
    long double v1;
    long double v2;
    long double v3;
    long double v4;

    MATH_TransformVector(&g_stGlobalNormPlaneUp, &a1->objectMtx, &a1->planeUp);
    MATH_TransformVector(&g_stGlobalNormPlaneDown, &a1->objectMtx, &a1->planeDown);
    MATH_TransformVector(&g_stGlobalNormPlaneLeft, &a1->objectMtx, &a1->planeLeft);
    MATH_TransformVector(&g_stGlobalNormPlaneRight, &a1->objectMtx, &a1->planeRight);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneUp, &g_stGlobalNormPlaneUp, &g_stGlobalNormPlaneDown, 0.25);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneDown, &g_stGlobalNormPlaneUp, &g_stGlobalNormPlaneDown, 0.75);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneLeft, &g_stGlobalNormPlaneLeft, &g_stGlobalNormPlaneRight, 0.25);
    MATH_BlendVector(&g_stOverlayed_GlobalNormPlaneRight, &g_stGlobalNormPlaneLeft, &g_stGlobalNormPlaneRight, 0.75);
    v1 = 1.0                                      // MATH_NormalizeEqualVector x4
        / sqrt(
            g_stOverlayed_GlobalNormPlaneUp.z * g_stOverlayed_GlobalNormPlaneUp.z
            + g_stOverlayed_GlobalNormPlaneUp.y * g_stOverlayed_GlobalNormPlaneUp.y
            + g_stOverlayed_GlobalNormPlaneUp.x * g_stOverlayed_GlobalNormPlaneUp.x);
    g_stOverlayed_GlobalNormPlaneUp.x = (float)(g_stOverlayed_GlobalNormPlaneUp.x * v1);
    g_stOverlayed_GlobalNormPlaneUp.y = (float)(g_stOverlayed_GlobalNormPlaneUp.y * v1);
    g_stOverlayed_GlobalNormPlaneUp.z = (float)(g_stOverlayed_GlobalNormPlaneUp.z * v1);
    v2 = 1.0
        / sqrt(
            g_stOverlayed_GlobalNormPlaneDown.z * g_stOverlayed_GlobalNormPlaneDown.z
            + g_stOverlayed_GlobalNormPlaneDown.y * g_stOverlayed_GlobalNormPlaneDown.y
            + g_stOverlayed_GlobalNormPlaneDown.x * g_stOverlayed_GlobalNormPlaneDown.x);
    g_stOverlayed_GlobalNormPlaneDown.x = (float)(g_stOverlayed_GlobalNormPlaneDown.x * v2);
    g_stOverlayed_GlobalNormPlaneDown.y = (float)(g_stOverlayed_GlobalNormPlaneDown.y * v2);
    g_stOverlayed_GlobalNormPlaneDown.z = (float)(g_stOverlayed_GlobalNormPlaneDown.z * v2);
    v3 = 1.0
        / sqrt(
            g_stOverlayed_GlobalNormPlaneLeft.z * g_stOverlayed_GlobalNormPlaneLeft.z
            + g_stOverlayed_GlobalNormPlaneLeft.y * g_stOverlayed_GlobalNormPlaneLeft.y
            + g_stOverlayed_GlobalNormPlaneLeft.x * g_stOverlayed_GlobalNormPlaneLeft.x);
    g_stOverlayed_GlobalNormPlaneLeft.x = (float)(g_stOverlayed_GlobalNormPlaneLeft.x * v3);
    g_stOverlayed_GlobalNormPlaneLeft.y = (float)(g_stOverlayed_GlobalNormPlaneLeft.y * v3);
    g_stOverlayed_GlobalNormPlaneLeft.z = (float)(g_stOverlayed_GlobalNormPlaneLeft.z * v3);
    v4 = 1.0
        / sqrt(
            g_stOverlayed_GlobalNormPlaneRight.z * g_stOverlayed_GlobalNormPlaneRight.z
            + g_stOverlayed_GlobalNormPlaneRight.y * g_stOverlayed_GlobalNormPlaneRight.y
            + g_stOverlayed_GlobalNormPlaneRight.x * g_stOverlayed_GlobalNormPlaneRight.x);
    g_stOverlayed_GlobalNormPlaneRight.x = (float)(g_stOverlayed_GlobalNormPlaneRight.x * v4);
    g_stOverlayed_GlobalNormPlaneRight.y = (float)(g_stOverlayed_GlobalNormPlaneRight.y * v4);
    g_stOverlayed_GlobalNormPlaneRight.z = (float)(g_stOverlayed_GlobalNormPlaneRight.z * v4);
    g_stGlobalCameraPos = a1->objectMtx.Pos;
    g_stGlobalCameraSight.x = a1->objectMtx.v3.x;
    g_stGlobalCameraSight.y = a1->objectMtx.v3.y;
    g_stGlobalCameraSight.z = a1->objectMtx.v3.z;
    if ((a1->flags & 1) != 0)
        g_fDistance = 0.0;
    else
        g_fDistance = -(a1->isoZoom * a1->isoFactor);
}

int OBJ_CullingAABBox(Vec3** a1, uint8_t* a2)

{
    Vec3* uVar1;

    if ((((g_fDistance <=
        g_stGlobalCameraSight.x * a1[((*((uint32_t*)&(g_stGlobalCameraSight.x))) >> 0x1f)]->x +
        g_stGlobalCameraSight.y * (a1[((*((uint32_t*)&(g_stGlobalCameraSight.y))) >> 0x1f)]->y) +
        g_stGlobalCameraSight.z * (a1[((*((uint32_t*)&(g_stGlobalCameraSight.z))) >> 0x1f)]->z)) && //((*((uint32_t*)&(g_stGlobalNormPlaneDown.x))) >> 0x1f)
        (g_fDistance <=
            g_stGlobalNormPlaneLeft.x * a1[((*((uint32_t*)&(g_stGlobalNormPlaneLeft.x))) >> 0x1f)]->x +
            g_stGlobalNormPlaneLeft.y * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneLeft.y))) >> 0x1f)]->y) +
            g_stGlobalNormPlaneLeft.z * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneLeft.z))) >> 0x1f)]->z)))
        && (g_fDistance <=
            g_stGlobalNormPlaneRight.x * a1[((*((uint32_t*)&(g_stGlobalNormPlaneRight.x))) >> 0x1f)]->x +
            g_stGlobalNormPlaneRight.y * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneRight.y))) >> 0x1f)]->y) +
            g_stGlobalNormPlaneRight.z * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneRight.z))) >> 0x1f)]->z))) &&
        ((g_fDistance <=
            g_stGlobalNormPlaneUp.x * a1[((*((uint32_t*)&(g_stGlobalNormPlaneUp.x))) >> 0x1f)]->x +
            g_stGlobalNormPlaneUp.y * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneUp.y))) >> 0x1f)]->y) +
            g_stGlobalNormPlaneUp.z * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneUp.z))) >> 0x1f)]->z) &&
            (g_fDistance <=
                g_stGlobalNormPlaneDown.x * a1[((*((uint32_t*)&(g_stGlobalNormPlaneDown.x))) >> 0x1f)]->x +
                g_stGlobalNormPlaneDown.y * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneDown.y))) >> 0x1f)]->y) +
                g_stGlobalNormPlaneDown.z * (a1[((*((uint32_t*)&(g_stGlobalNormPlaneDown.z))) >> 0x1f)]->z)))))
    {
        if (a2 != NULL) {
            uVar1 = a1[1];
            a1[1] = a1[0];
            a1[0] = uVar1;
            *a2 = 0;
            if (g_stGlobalCameraSight.x * a1[((*((uint32_t*)&(g_stGlobalCameraSight.x))) >> 0x1f)]->x +    //((*((uint32_t*)&(g_stGlobalCameraSight.x))) >> 0x1f)
                g_stGlobalCameraSight.y * (a1[((*((uint32_t*)&(g_stGlobalCameraSight.y))) >> 0x1f)]->y) +
                g_stGlobalCameraSight.z * (a1[((*((uint32_t*)&(g_stGlobalCameraSight.z))) >> 0x1f)]->z) < g_fDistance) {
                *a2 = 1;
            }
            if (((g_stOverlayed_GlobalNormPlaneLeft.x * a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneLeft.x))) >> 0x1f)]->x + ////((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.x))) >> 0x1f)
                g_stOverlayed_GlobalNormPlaneLeft.y * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneLeft.y))) >> 0x1f)]->y) +
                g_stOverlayed_GlobalNormPlaneLeft.z * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneLeft.z))) >> 0x1f)]->z) < g_fDistance)
                || (g_stOverlayed_GlobalNormPlaneRight.x * a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneRight.x))) >> 0x1f)]->x +
                    g_stOverlayed_GlobalNormPlaneRight.y * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneRight.y))) >> 0x1f)]->y) +
                    g_stOverlayed_GlobalNormPlaneRight.z * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneRight.z))) >> 0x1f)]->z) < g_fDistance)) ||
                ((g_stOverlayed_GlobalNormPlaneUp.x * a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneUp.x))) >> 0x1f)]->x +
                    g_stOverlayed_GlobalNormPlaneUp.y * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneUp.y))) >> 0x1f)]->y) +
                    g_stOverlayed_GlobalNormPlaneUp.z *  (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneUp.z))) >> 0x1f)]->z) < g_fDistance ||
                    (g_stOverlayed_GlobalNormPlaneDown.x * a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.x))) >> 0x1f)]->x +
                        g_stOverlayed_GlobalNormPlaneDown.y * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.y))) >> 0x1f)]->y) +
                        g_stOverlayed_GlobalNormPlaneDown.z * (a1[((*((uint32_t*)&(g_stOverlayed_GlobalNormPlaneDown.z))) >> 0x1f)]->z) < g_fDistance)))
                ) {
                *a2 = *a2 | 2;
            }
        }
        return 0;
    }
    return 1;
}

BOOL OBJ_CullingSphere(Vec3* param_1, float param_2)

{
    float fVar1;
    float fVar2;
    float fVar3;

    fVar1 = param_2;
    if (g_stGlobalNormPlaneLeft.x <= 0.0) {
        fVar1 = -param_2;
    }
    fVar2 = param_2;
    if (g_stGlobalNormPlaneLeft.y <= 0.0) {
        fVar2 = -param_2;
    }
    fVar3 = param_2;
    if (g_stGlobalNormPlaneLeft.z <= 0.0) {
        fVar3 = -param_2;
    }
    if ((g_stGlobalNormPlaneLeft.x * ((fVar1 + param_1->x) - g_stGlobalCameraPos.x) +
        g_stGlobalNormPlaneLeft.y * ((fVar2 + param_1->y) - g_stGlobalCameraPos.y) +
        g_stGlobalNormPlaneLeft.z * ((fVar3 + param_1->z) - g_stGlobalCameraPos.z) >= 0.0)) {
        fVar1 = param_2;
        if (g_stGlobalNormPlaneRight.x <= 0.0) {
            fVar1 = -param_2;
        }
        fVar2 = param_2;
        if (g_stGlobalNormPlaneRight.y <= 0.0) {
            fVar2 = -param_2;
        }
        fVar3 = param_2;
        if (g_stGlobalNormPlaneRight.z <= 0.0) {
            fVar3 = -param_2;
        }
        if ((g_stGlobalNormPlaneRight.x * ((fVar1 + param_1->x) - g_stGlobalCameraPos.x) +
            g_stGlobalNormPlaneRight.y * ((fVar2 + param_1->y) - g_stGlobalCameraPos.y) +
            g_stGlobalNormPlaneRight.z * ((fVar3 + param_1->z) - g_stGlobalCameraPos.z) >= 0.0)) {
            fVar1 = param_2;
            if (g_stGlobalNormPlaneUp.x <= 0.0) {
                fVar1 = -param_2;
            }
            fVar2 = param_2;
            if (g_stGlobalNormPlaneUp.y <= 0.0) {
                fVar2 = -param_2;
            }
            fVar3 = param_2;
            if (g_stGlobalNormPlaneUp.z <= 0.0) {
                fVar3 = -param_2;
            }
            if ((g_stGlobalNormPlaneUp.x * ((fVar1 + param_1->x) - g_stGlobalCameraPos.x) +
                g_stGlobalNormPlaneUp.y * ((fVar2 + param_1->y) - g_stGlobalCameraPos.y) +
                g_stGlobalNormPlaneUp.z * ((fVar3 + param_1->z) - g_stGlobalCameraPos.z) >= 0.0)) {
                fVar1 = param_2;
                if (g_stGlobalNormPlaneDown.x <= 0.0) {
                    fVar1 = -param_2;
                }
                fVar2 = param_2;
                if (g_stGlobalNormPlaneDown.y <= 0.0) {
                    fVar2 = -param_2;
                }
                if (g_stGlobalNormPlaneDown.z <= 0.0) {
                    param_2 = -param_2;
                }
                if (0.0 <= g_stGlobalNormPlaneDown.x * ((fVar1 + param_1->x) - g_stGlobalCameraPos.x) +
                    g_stGlobalNormPlaneDown.y * ((fVar2 + param_1->y) - g_stGlobalCameraPos.y) +
                    g_stGlobalNormPlaneDown.z * ((param_2 + param_1->z) - g_stGlobalCameraPos.z)) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

BOOL OBJ_CullingObject(struct sGameObjectMain* a1)
{
    tGameObjectMain* v1;
    tGameObjectBV* v2;
    Mtx4x4* v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;
    int v10;
    tGameObjectBasic* v11;
    tGameObjectActorObject* v12;
    tGameObjectBasic* v13;
    tGameObjectVisualObject* v14;
    Mtx4x4* v15;
    Vec3 *v16[2];
    float v17;
    Vec3 v18;
    Vec3 dest;
    uint8_t a1a;

    v1 = a1;
    v2 = a1->boundingVolume;
    if (OBJ_HasNoBV(a1))
        return 0;
    if (OBJ_BV_IsAABBox(v2))
    {
        v4 = v1->matrices;
        v5 = v4->Pos.x;
        v4 = (Mtx4x4*)((char*)v4 + 48);
        v6 = v5 - g_stGlobalCameraPos.x;
        v7 = v4->v1.y - g_stGlobalCameraPos.y;
        v8 = v4->v1.z;
        v16[0] = &v18;
        v17 = (float)(v8 - g_stGlobalCameraPos.z);
        v18.x = (float)(v6 + v2->BVMax.x);
        v9 = v2->BVMax.y;
        v16[1] = &dest;
        v18.y = (float)(v9 + v7);
        v18.z = v2->BVMax.z + v17;
        dest.x = (float)(v6 + v2->BVMin.x);
        dest.y = (float)(v2->BVMin.y + v7);
        dest.z = v2->BVMin.z + v17;
        v10 = OBJ_CullingAABBox(v16, &a1a);
        if ((v1->flags & 2) != 0)
        {
            v11 = v1->basic;
            if (v11)
            {
                v12 = (tGameObjectActorObject*)v11->actorObjectOrVisualObject;
                if (v12)
                {
                    v12->culledFlagFromCullingObject = a1a;
                    return v10 != 0;
                }
            }
        }
        if ((v1->flags & 0x4000) != 0)
        {
            v13 = v1->basic;
            if (v13)
            {
                v14 = (tGameObjectVisualObject*)v13->actorObjectOrVisualObject;
                if (v14)
                {
                    v14->culledFlagFromCullingObject = a1a;
                    return v10 != 0;
                }
            }
        }
    }
    else
    {
        v15 = OBJ_pst_GetAbsoluteMatrix(v1);
        MATH_TransformVertexNoScale(&dest, v15, &v2->BVMin);
        v10 = OBJ_CullingSphere(&dest, v2->BVMax.x);
    }
    return v10 != 0;
}

BOOL OBJ_CullingPointInside(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, char flags)
{
    return ((flags & 1) == 0
        || a2->z * (a1->z - g_stGlobalCameraPos.z)
        + a2->y * (a1->y - g_stGlobalCameraPos.y)
        + (a1->x - g_stGlobalCameraPos.x) * a2->x < g_fDistance)
        && ((flags & 4) == 0
            || a4->z * (a1->z - g_stGlobalCameraPos.z)
            + a4->y * (a1->y - g_stGlobalCameraPos.y)
            + (a1->x - g_stGlobalCameraPos.x) * a4->x < g_fDistance)
        && ((flags & 2) == 0
            || a3->z * (a1->z - g_stGlobalCameraPos.z)
            + a3->y * (a1->y - g_stGlobalCameraPos.y)
            + (a1->x - g_stGlobalCameraPos.x) * a3->x < g_fDistance)
        && ((flags & 8) == 0
            || a5->z * (a1->z - g_stGlobalCameraPos.z)
            + a5->y * (a1->y - g_stGlobalCameraPos.y)
            + (a1->x - g_stGlobalCameraPos.x) * a5->x < g_fDistance);
}

void OBJ_HierarchyMainCall(struct sWorld* world)
{
    tWorld* v1;
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    tGameObjectMain* v4;
    tGameObjectBasic* v6;
    int v7;
    tableElemStructPF* i;

    v1 = world;
    v2 = world->eot.objectsWithHiearchyTablePF.firstElemPtr;
    v3 = ((tableElemStructPF*)(world->eot.objectsWithHiearchyTablePF.nextElemPtr)) - 1;
    for (i = v3; v2 <= v3; ++v2)
    {
        v4 = (tGameObjectMain*)(v2->objectPtr);
        if (!TAB_TableElemIsInvalid(v4))
        {
            if (!OBJ_b_HasBeenProcess(v4, v1->hierarchyCallCount, 0x80))
            {
                v6 = v4->basic;
                v7 = 0;
                if (v6)
                {
                    while (1)
                    {
                        if (!v6->mtx)
                            break;
                        if (!v6->mtx->refGao2 || OBJ_b_HasBeenProcess(v6->mtx->refGao2, v1->hierarchyCallCount, 0x80))
                            break;
                        v4 = v6->mtx->refGao2;
                        v6 = v6->mtx->refGao2->basic;
                        v7 = 1;
                        if (!v6)
                            goto LABEL_12;
                    }
                    if (!v7)
                        goto LABEL_13;
                LABEL_12:
                    --v2;
                }
            LABEL_13:
                OBJ_SetProcess(v4, v1->hierarchyCallCount, 0x80);
                OBJ_ComputeGlobalWhenHie(v4);
                v3 = i;
                v1 = world;
            }

        }
    }
    ++v1->hierarchyCallCount;
}

struct sGroGeneric* OBJ_p_GetGro(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGameObjectVisualObject* v2;
    tGroGeneric* result;

    if (a1
        && (a1->flags & 0x4000) != 0
        && (v1 = a1->basic) != 0
        && (v2 = (tGameObjectVisualObject*)v1->actorObjectOrVisualObject) != 0)
    {
        result = v2->GROGEOOrSomethingElse;
    }
    else
    {
        result = 0;
    }
    return result;
}

void OBJMY_ComputeLocalWhenHie(struct sGameObjectMain* a1)
{
    tGaoMatrix* v1;
    Mtx4x4* v2;
    Mtx4x4* v3;
    Mtx4x4 a3;

    if ((a1->flags & 0x400000) != 0)
    {
        v1 = a1->basic->mtx;
        if (v1->refGao2)
        {
            v2 = OBJ_pst_GetAbsoluteMatrix(v1->refGao2);
            MATH_InvertMatrix(&a3, v2);
            v3 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_MulMatrixMatrix(&v1->mtx, v3, &a3);
            MATH_Orthonormalize(&v1->mtx);
        }
    }
}

struct sGEOMAT* OBJ_p_GetGrm(struct sGameObjectMain* a1)
{
    tGEOMAT* result;

    if ((a1->flags & 0x4000) != 0)
        result = (tGEOMAT*)(((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT);
    else
        result = 0;
    return result;
}

char OBJ_uc_CalcLODVis(struct sGameObjectMain* a1, struct sViewElem* a2)
{
    tGdi_DisplayData* v2;
    double v3;
    double v4;
    tGameObjectBV* v5;
    BOOL v6;
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    char result;
    Vec3 v13;
    float v14;
    float v15;
    float v16;
    int tmp;

    v2 = a2->displayData;
    if (!v2)
        return -1;
    OBJ_BV_ComputeCenter(a1, &v13);
    v3 = v13.x - a2->cameraMtx.Pos.x;
    v4 = v13.y - a2->cameraMtx.Pos.y;
    v5 = a1->boundingVolume;
    v14 = (float)((v13.z - a2->cameraMtx.Pos.z) * (v13.z - a2->cameraMtx.Pos.z) + v4 * v4 + v3 * v3);
    v6 = OBJ_BV_IsSphere(v5);
    v7 = v5->BVMax.x;
    if (v6)
    {
        v8 = v7 * v7 * (v2->ct.cam.factorX * v2->ct.cam.factorX) / v14;
    }
    else
    {
        v9 = v5->BVMax.y - v5->BVMin.y;
        v10 = v5->BVMax.z - v5->BVMin.z;
        v8 = (v10 * v10 + v9 * v9 + (v7 - v5->BVMin.x) * (v7 - v5->BVMin.x))
            * (v2->ct.cam.factorX
                * v2->ct.cam.factorX
                / v14);
    }
    v15 = (float)v8;

    tmp = (MATH_gl_TableRacine[((*((int32_t*)(&v15))) >> 13) & 0x7FF] + (((((*((uint32_t*)(&v15))) & 0x7F800000) + 0x3F800000) >> 1) & 0x7F800000));
    v11 = (*(float *)&tmp) * v2->NormalizedWidthOrHeightMultBy255;

    if (v11 >= 255.0)
        v11 = 255.0;
    v16 = (float)(v11 + 12533760.0);
    result = (*(int *)&v16) & 0xff; //result = LOBYTE(v16);
    if (!result) //if (!LOBYTE(v16))
        result = 1;
    return result;
}
