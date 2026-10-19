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

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* a1)
{
    return a1->matrices;
}

void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* a1)
{
    tGaoMatrix* v1;
    Mtx4x4* v4;
    Mtx4x4* v6;

    if ((a1->flags & 0x400000) != 0)
    {
        v1 = a1->basic->mtx;
        if (v1->refGao2)
        {
            v6 = OBJ_pst_GetAbsoluteMatrix(v1->refGao2);
            v4 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_MulMatrixMatrix(v4, (Mtx4x4*)v1, v6);
        }
    }
}

void OBJ_UpdateStatusFlagRTL(tGameObjectMain* a1)
{
    int v1;
    int v2;
    int v3;

    if (a1)
        v1 = a1->flags;
    else
        v1 = 0; // LOWORD
    if ((v1 & 0x4000) != 0 || (v1 & 2) != 0)
    {
        if ((v1 & 0x26) != 0 || (v2 = a1->flags2, (v2 & 0x40000) != 0))
            v3 = a1->flags2 | 8;
        else
            v3 = v2 & ~8u;
        a1->flags2 = v3;
        if ((v3 & 0x80000) != 0)
            a1->flags2 = v3 & ~8u;
    }
    else
    {
        a1->flags2 &= ~8u;
    }
}

int OBJ_b_HasBeenProcess(tGameObjectMain* a1, char a2, int a3)
{
    int result;

    if ((a2 & 1) != 0)
        result = (a3 & a1->process) == 0;
    else
        result = a3 & a1->process;
    return result;
}

void OBJ_ResetProcess(tGameObjectMain* a1, char a2, char a3)
{
    if ((a2 & 1) != 0)
        a1->process |= a3;
    else
        a1->process &= ~a3;
}

BOOL OBJ_BV_IsSphere(tGameObjectBV* a1)
{
    return (*((uint32_t *)&(a1->BVMax.z))) == 0xffffffff;
}

BOOL OBJ_BV_IsAABBox(tGameObjectBV* a1)
{
    return (*((uint32_t*)&(a1->BVMax.z))) != 0xffffffff;
}

void OBJ_BV_SetSphereFlag(tGameObjectBV* a1)
{
    (*((uint32_t*)&(a1->BVMax.z))) = 0xffffffff;
}

BOOL OBJ_HasNoBV(tGameObjectMain* a1)
{
    tGameObjectBV* v1;
    BOOL v2;
    double v3;

    v1 = a1->boundingVolume;
    v2 = OBJ_BV_IsAABBox(v1);
    v3 = v1->BVMax.x;
    if (v2)
    {
        if (v3 != 0.0 || v1->BVMax.y != 0.0)
            return 0;
        v3 = v1->BVMax.z;
    }
    return v3 == 0.0;
}

void OBJ_BV_InitAABBox(tGameObjectBV* a1, Mtx4x4* a2, Vec3* a3)
{
    float v3;
    float v4;
    float v5;
    Vec3 a1a;

    MATH_TransformVector(&a1a, a2, a3);
    v3 = a1a.z;
    v4 = a1a.x;
    v5 = a1a.y;
    a1->BVMin.z = a1a.z;
    a1->BVMin.x = v4;
    a1->BVMin.y = v5;
    a1->BVMax.z = v3;
    a1->BVMax.x = v4;
    a1->BVMax.y = v5;
}

void OBJ_BV_InitSphereBV(tGameObjectBV* a1, Vec3* a2, float a3, Vec3* a4)
{
    float v4;
    float v5;
    float v6;

    if (a4)
    {
        v5 = a2->x * a4->x;
        v6 = a2->y * a4->y;
        v4 = a2->z * a4->z;
        a1->BVMin.x = v5;
        a1->BVMin.y = v6;
        a1->BVMin.z = v4;
    }
    else
    {
        a1->BVMin = *a2;
    }
    a1->BVMax.x = a3;
    OBJ_BV_SetSphereFlag(a1);
}

void OBJ_BV_AddPointToSphereBV(tGameObjectBV* a1, Vec3* a2, Vec3* a3)
{
    tGameObjectBV* v3;
    float v4;
    float v5;
    float v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float a3a;

    if (a3)
    {
        v10 = a2->y * a3->y;
        v3 = a1;
        v11 = a2->z * a3->z;
        v4 = a2->x * a3->x - a1->BVMin.x;
        v7 = v10 - a1->BVMin.y;
        v5 = v11;
    }
    else
    {
        v3 = a1;
        v4 = a2->x - a1->BVMin.x;
        v7 = a2->y - a1->BVMin.y;
        v5 = a2->z;
    }
    a3a = v3->BVMax.x;
    v8 = v5 - v3->BVMin.z;
    v6 = (float)sqrt(v8 * v8 + v4 * v4 + v7 * v7);
    if (v6 > a3a)
    {
        v12 = (1.0f - a3a / v6) * 0.5f;
        v9 = v8 * v12;
        v3->BVMin.x = v4 * v12 + v3->BVMin.x;
        v3->BVMin.y = v7 * v12 + v3->BVMin.y;
        v3->BVMin.z = v9 + v3->BVMin.z;
        v3->BVMax.x = (a3a + v6) * 0.5f;
    }
}

void OBJ_BV_AddPointToAABBox(tGameObjectBV* a1, Mtx4x4* a2, Vec3* a3, int a4)
{
    Vec3 a1a;

    if (a4)
        MATH_TransformVectorNoScale(&a1a, a2, a3);
    else
        MATH_TransformVector(&a1a, a2, a3);
    if (a1a.x < (double)a1->BVMin.x)
        a1->BVMin.x = a1a.x;
    if (a1a.y < (double)a1->BVMin.y)
        a1->BVMin.y = a1a.y;
    if (a1a.z < (double)a1->BVMin.z)
        a1->BVMin.z = a1a.z;
    if (a1a.x > (double)a1->BVMax.x)
        a1->BVMax.x = a1a.x;
    if (a1a.y > (double)a1->BVMax.y)
        a1->BVMax.y = a1a.y;
    if (a1a.z > (double)a1->BVMax.z)
        a1->BVMax.z = a1a.z;
}

void OBJ_ComputeCameraBV(tGameObjectMain* a1, int a2)
{
    tGameObjectBV* v3;

    v3 = a1->boundingVolume;
    if (a2)
    {
        v3->BVMin.x = 0.0;
        v3->BVMin.y = 0.0;
        v3->BVMin.z = 0.0;
        v3->BVMax.x = 0.5;
        OBJ_BV_SetSphereFlag(a1->boundingVolume);
    }
    else
    {
        v3->BVMin.x = -0.5;
        v3->BVMin.y = -0.5;
        v3->BVMin.z = -0.5;
        v3->BVMax.x = 0.5;
        v3->BVMax.y = 0.5;
        v3->BVMax.z = 0.5;
        a1->boundingVolume->BVMinOriginal = a1->boundingVolume->BVMin;
        a1->boundingVolume->BVMaxOriginal = a1->boundingVolume->BVMax;
    }
    if ((a1->flags & 0x80000) != 0)
    {
        v3->BVMinLocal.x = -0.5;
        v3->BVMinLocal.y = -0.5;
        v3->BVMinLocal.z = -0.5;
        v3->BVMaxLocal.x = 0.5;
        v3->BVMaxLocal.y = 0.5;
        v3->BVMaxLocal.z = 0.5;
    }
}

void OBJ_ComputeWaypointBV(tGameObjectMain* a1, int a2)
{
    tGameObjectBV* v2;

    v2 = a1->boundingVolume;
    if ((a1->flags & 0x80000) != 0)
    {
        v2->BVMinLocal.x = -1.0;
        v2->BVMinLocal.y = -1.0;
        v2->BVMinLocal.z = -1.0;
        v2->BVMaxLocal.x = 1.0;
        v2->BVMaxLocal.y = 1.0;
        v2->BVMaxLocal.z = 1.0;
    }
    else if (a2)
    {
        v2->BVMin.x = 0.0;
        v2->BVMin.y = 0.0;
        v2->BVMin.z = 0.0;
        v2->BVMax.x = 1.0;
        OBJ_BV_SetSphereFlag(a1->boundingVolume);
    }
    else
    {
        v2->BVMin.y = -1.0;
        v2->BVMin.x = -1.0;
        v2->BVMin.z = -1.0;
        v2->BVMax.x = 1.0;
        v2->BVMax.y = 1.0;
        v2->BVMax.z = 1.0;
        v2->BVMinOriginal.x = v2->BVMin.x;
        v2->BVMinOriginal.y = v2->BVMin.y;
        v2->BVMinOriginal.z = v2->BVMin.z;
        v2->BVMaxOriginal.x = v2->BVMax.x;
        v2->BVMaxOriginal.y = v2->BVMax.y;
        v2->BVMaxOriginal.z = v2->BVMax.z;
    }
}

void OBJ_ComputeLightBV(tGameObjectMain* a1)
{
    tGameObjectBV* v1;
    tLIGHT* v2;

    v1 = a1->boundingVolume;
    v2 = (tLIGHT*)(a1->extended->light);
    if ((v2->flags & 7) != 0)
    {
        if ((v2->flags & 7u) <= 3)
        {
            v1->BVMin.x = 0.0;
            v1->BVMin.y = 0.0;
            v1->BVMin.z = 0.0;
            v1->BVMax.x = 0.0;
            OBJ_BV_SetSphereFlag(a1->boundingVolume);
        }
    }
    else
    {
        v1->BVMin.x = 0.0;
        v1->BVMin.y = 0.0;
        v1->BVMin.z = 0.0;
        v1->BVMax.x = v2->ffar;
        OBJ_BV_SetSphereFlag(a1->boundingVolume);
    }
}

void OBJ_ComputeAABBoxFromOBBox(tGameObjectMain* a1)
{
    tGameObjectBV* v1;
    unsigned int i;
    Mtx4x4* v3;
    Vec3 a1a;
    Vec3 a3;

    v1 = a1->boundingVolume;
    v1->BVMin.x = 3.402822e38f;
    v1->BVMin.y = 3.402822e38f;
    v1->BVMin.z = 3.402822e38f;
    v1->BVMax.x = -3.402822e38f;
    v1->BVMax.y = -3.402822e38f;
    v1->BVMax.z = -3.402822e38f;
    for (i = 0; i < 8; ++i)
    {
        if ((i & 1) != 0)
            a3.x = v1->BVMinLocal.x;
        else
            a3.x = v1->BVMaxLocal.x;
        if ((i & 2) != 0)
            a3.y = v1->BVMinLocal.y;
        else
            a3.y = v1->BVMaxLocal.y;
        if ((i & 4) != 0)
            a3.z = v1->BVMinLocal.z;
        else
            a3.z = v1->BVMaxLocal.z;
        v3 = OBJ_pst_GetAbsoluteMatrix(a1);
        MATH_TransformVectorNoScale(&a1a, v3, &a3);
        if (a1a.x < (double)v1->BVMin.x)
            v1->BVMin.x = a1a.x;
        if (a1a.y < (double)v1->BVMin.y)
            v1->BVMin.y = a1a.y;
        if (a1a.z < (double)v1->BVMin.z)
            v1->BVMin.z = a1a.z;
        if (a1a.x > (double)v1->BVMax.x)
            v1->BVMax.x = a1a.x;
        if (a1a.y > (double)v1->BVMax.y)
            v1->BVMax.y = a1a.y;
        if (a1a.z > (double)v1->BVMax.z)
            v1->BVMax.z = a1a.z;
    }
}

void OBJ_DuplicateBV(tGameObjectMain* a1, tGameObjectMain* a2)
{
    tGameObjectBV* v2;

    v2 = a1->boundingVolume;
    if ((a1->flags & 0x80000) != 0)
        memcpy(a2->boundingVolume, v2, sizeof(tGameObjectBV));
    else
        memcpy(a2->boundingVolume, v2, sizeof(tGameObjectBV) - (2 * sizeof(Vec3))); // kivonjuk belõle a végén lévõ két local méretét
}

void OBJ_BV_ComputeCenter(struct sGameObjectMain* a1, Vec3* a2)
{
    tGameObjectBV* v3;
    float v4;
    float v5;
    Mtx4x4* v7;
    float a1a;

    v3 = a1->boundingVolume;
    if (OBJ_BV_IsAABBox(v3))
    {
        a2->x = v3->BVMax.x + v3->BVMin.x;
        a2->y = v3->BVMax.y + v3->BVMin.y;
        a2->z = v3->BVMax.z + v3->BVMin.z;
        v4 = a2->x * 0.5f;
        a2->x = v4;
        v5 = a2->y * 0.5f;
        a2->y = v5;
        a1a = a2->z * 0.5f;
        a2->z = a1a;
        a2->x = v4 + a1->matrices->Pos.x;
        a2->y = v5 + a1->matrices->Pos.y;
        a2->z = a1a + a1->matrices->Pos.z;
    }
    else
    {
        v7 = OBJ_pst_GetAbsoluteMatrix(a1);
        MATH_TransformVertexNoScale(a2, v7, &v3->BVMin);
    }
}

void OBJ_BV_CalcBVForTwoOBJs(tGameObjectMain* a1, tGameObjectMain* a2)
{
    Mtx4x4* v2;
    tGameObjectBV* v3;
    float v4;
    float v5;
    float v6;
    Mtx4x4* v7;
    float v8;
    tGameObjectBV* v9;
    tGameObjectBV* v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v17;
    float v18;
    float v19;
    float v20;

    v2 = a2->matrices;
    v3 = a2->boundingVolume;
    v4 = v3->BVMax.x + v2->Pos.x;
    v5 = v3->BVMax.y + v2->Pos.y;
    v19 = v3->BVMax.z + v2->Pos.z;
    v11 = v3->BVMin.x + v2->Pos.x;
    v13 = v2->Pos.y + v3->BVMin.y;
    v6 = v2->Pos.z + v3->BVMin.z;
    v7 = a1->matrices;
    v15 = v6;
    v17 = v4 - v7->Pos.x;
    v18 = v5 - v7->Pos.y;
    v20 = v19 - v7->Pos.z;
    v12 = v11 - v7->Pos.x;
    v14 = v13 - v7->Pos.y;
    v8 = v15 - v7->Pos.z;
    v9 = a1->boundingVolume;
    v9->BVMax.x = v17;
    v9->BVMax.y = v18;
    v9->BVMax.z = v20;
    v10 = a1->boundingVolume;
    v10->BVMin.x = v12;
    v10->BVMin.y = v14;
    v10->BVMin.z = v8;
}

void __cdecl OBJ_MergeBVs(tGameObjectMain* a1, tGameObjectMain* a2)
{
    tGameObjectBV* v2;
    tGameObjectBV* v3;
    Mtx4x4* v4;
    float v5;
    float v6;
    float v7;
    Mtx4x4* v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;

    v2 = a1->boundingVolume;
    if (OBJ_BV_IsAABBox(v2))
    {
        v3 = a2->boundingVolume;
        if (OBJ_BV_IsAABBox(v3))
        {
            v4 = a2->matrices;
            v5 = v4->Pos.x + v3->BVMax.x;
            v6 = v3->BVMax.y + v4->Pos.y;
            v20 = v3->BVMax.z + v4->Pos.z;
            v14 = v4->Pos.x + v3->BVMin.x;
            v15 = v4->Pos.y + v3->BVMin.y;
            v7 = v4->Pos.z;
            v8 = a1->matrices;
            v17 = v7 + v3->BVMin.z;
            v19 = v5 - v8->Pos.x;
            v9 = v6 - v8->Pos.y;
            v21 = v20 - v8->Pos.z;
            v10 = v14 - v8->Pos.x;
            v16 = v15 - v8->Pos.y;
            v18 = v17 - v8->Pos.z;
            if (v10 >= v2->BVMin.x)
                v10 = v2->BVMin.x;
            v2->BVMin.x = v10;
            if (v16 >= (double)v2->BVMin.y)
                v11 = v2->BVMin.y;
            else
                v11 = v16;
            v2->BVMin.y = v11;
            if (v18 >= (double)v2->BVMin.z)
                v12 = v2->BVMin.z;
            else
                v12 = v18;
            v2->BVMin.z = v12;
            if (v19 <= (double)v2->BVMax.x)
                v13 = v2->BVMax.x;
            else
                v13 = v19;
            v2->BVMax.x = v13;
            if (v9 <= v2->BVMax.y)
                v9 = v2->BVMax.y;
            v2->BVMax.y = v9;
            if (v21 <= (double)v2->BVMax.z)
                v2->BVMax.z = v2->BVMax.z;
            else
                v2->BVMax.z = v21;
        }
    }
}

int GEO_MRM_GetRealNumberOfPoints(tGEOGRO* a1)
{
    tGeoMRM* v1; // eax
    int result; // eax

    if (a1 && a1->intf->id == 1 && (v1 = a1->mrm) != 0)
        result = v1->endNum;
    else
        result = a1->vertexNum;
    return result;
}

void OBJ_ComputeGeometricObjectBV(tGameObjectMain* a1, int a2)
{
    tGameObjectVisualObject* v3;
    tGEOGRO* v4;
    Mtx4x4* v5;
    Vec3* v6;
    Vec3* v7;
    Vec3* v8;
    Mtx4x4* v9;
    Mtx4x4* v10; 
    tColMap* v11;
    int v12;
    tColCollisionObject* v13;
    tCobMeshObjectMesh* v14;
    unsigned int v15;
    int v16;
    Mtx4x4* v17;
    tGameObjectBV* v18;
    double v20;
    double v24;
    double v28;
    Vec3* v31;
    tGEOGRO* v32;
    tColMap* v33;
    Vec3 a1a;
    Mtx4x4* a2a;
    int a2b;

    v3 = a1->basic->actorObjectOrVisualObject;
    v4 = (tGEOGRO*)v3->GROGEOOrSomethingElse;
    v32 = (tGEOGRO*)(v3->GROGEOOrSomethingElse);
    if (!((tGEOGRO*)(v3->GROGEOOrSomethingElse))->vertices)
        return;
    v5 = OBJ_pst_GetAbsoluteMatrix(a1);
    v6 = v4->vertices;
    a2a = v5;
    v7 = v6 + 1;
    v8 = &v6[GEO_MRM_GetRealNumberOfPoints(v4)];
    if (a2 != 1)
    {
        v9 = OBJ_pst_GetAbsoluteMatrix(a1);
        OBJ_BV_InitAABBox(a1->boundingVolume, v9, v6);
        for (; v7 < v8; ++v7)
        {
            v10 = OBJ_pst_GetAbsoluteMatrix(a1);
            OBJ_BV_AddPointToAABBox(a1->boundingVolume, v10, v7, 0);
        }
        if ((a1->flags & 0x100) != 0)
        {
            v11 = a1->extended->coll->colMap;
            v12 = 0;
            v33 = v11;
            a2b = 0;
            if (v11->colObjectID)
            {
                do
                {
                    v13 = v11->collisionObjectPtr[v12];
                    if (v13->objectType == 5)
                    {
                        v14 = (tCobMeshObjectMesh*)v13->objectPtrByType;
                        v15 = 0;
                        if (v14)
                        {
                            v16 = 0;
                            while (v15 < v14->vertexCount)
                            {
                                v31 = &v14->vertices[v16];
                                v17 = OBJ_pst_GetAbsoluteMatrix(a1);
                                OBJ_BV_AddPointToAABBox(a1->boundingVolume, v17, v31, 1);
                                v11 = v33;
                                ++v15;
                                ++v16;
                            }
                            v12 = a2b;
                        }
                    }
                    a2b = ++v12;
                } while (v12 < v11->colObjectID);
            }
        }
        v18 = a1->boundingVolume;
        v20 = v18->BVMin.x;
        if (v18->BVMin.x == v18->BVMax.x)
            v20 = v20 - 0.0099999998;
        v18->BVMin.x = (float)v20;
        v24 = v18->BVMin.y;
        if (v18->BVMin.y == v18->BVMax.y)
            v24 = v24 - 0.0099999998;
        v18->BVMin.y = (float)v24;
        v28 = v18->BVMin.z;
        if (v18->BVMin.z == v18->BVMax.z)
            v28 = v28 - 0.0099999998;
        v18->BVMin.z = (float)v28;
        a1->boundingVolume->BVMinOriginal = a1->boundingVolume->BVMin;
        a1->boundingVolume->BVMaxOriginal = a1->boundingVolume->BVMax;
        goto LABEL_28;
    }
    if ((a2a->mtxflag & 8) != 0)
    {
        MATH_GetScale(&a1a, a2a);
        OBJ_BV_InitSphereBV(a1->boundingVolume, v32->vertices, 0.0, &a1a);
        if (v7 < v8)
        {
            do
                OBJ_BV_AddPointToSphereBV(a1->boundingVolume, v7++, &a1a);
            while (v7 < v8);
            v32->bv = a1->boundingVolume;
            return;
        }
    LABEL_28:
        v32->bv = a1->boundingVolume;
        return;
    }
    OBJ_BV_InitSphereBV(a1->boundingVolume, v6, 0.0, 0);
    if (v7 >= v8)
        goto LABEL_28;
    do
        OBJ_BV_AddPointToSphereBV(a1->boundingVolume, v7++, 0);
    while (v7 < v8);
    v32->bv = a1->boundingVolume;
}

void OBJ_ComputeStaticLODObjectBV(tGameObjectMain* a1, int a2)
{
    tGameObjectVisualObject* v2;
    tGroGeneric* v3;
    tGEOGRO* v4;

    v2 = a1->basic->actorObjectOrVisualObject;
    v3 = v2->GROGEOOrSomethingElse;
    v4 = ((tGEOSTATICLOD*)(v2->GROGEOOrSomethingElse))->geo[0];
    if (v4)
    {
        v2->GROGEOOrSomethingElse = (tGroGeneric*)v4;
        OBJ_ComputeGeometricObjectBV(a1, a2);
        ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = v3;
    }
}

void OBJ_ComputeAnimBV(tGameObjectMain* a1)
{
    tGameObjectBV* v1;
    Vec3* v2;
    tGameObjectBasic* v3;
    tGameObjectActorObject* v4;
    tGroup* v5;
    tableDescStruct* v6;
    tableElemStructPF* v7;
    tableElemStructPF* i;
    tGameObjectMain* v9;

    v1 = a1->boundingVolume;
    v1->BVMin.x = 0.0;
    v1->BVMin.y = 0.0;
    v1->BVMin.z = 0.0;
    v2 = &a1->boundingVolume->BVMax;
    v2->x = 0.0;
    v2->y = 0.0;
    v2->z = 0.0;
    if ((a1->flags & 2) != 0)
    {
        v3 = a1->basic;
        if (v3)
        {
            v4 = (tGameObjectActorObject*)(v3->actorObjectOrVisualObject);
            if (v4)
            {
                v5 = v4->skeletonGroup;
                if (v5)
                {
                    v6 = v5->tablePF;
                    v7 = v6->firstElemPtr;
                    for (i = ((tableElemStructPF*)(v6->nextElemPtr)) - 1; v7 <= i; ++v7)
                    {
                        v9 = (tGameObjectMain*)v7->objectPtr;
                        if ((((struct sGameObjectMain*)v7->objectPtr)->flags & 0x80000) != 0 && (v9->flags2 & 0x2000000) == 0)
                            OBJ_ComputeAABBoxFromOBBox((tGameObjectMain*)(v7->objectPtr));
                        OBJ_ComputeGlobalWhenHie(v9);
                        OBJ_MergeBVs(a1, v9);
                    }
                    a1->boundingVolume->BVMinOriginal = a1->boundingVolume->BVMin;
                    a1->boundingVolume->BVMaxOriginal = a1->boundingVolume->BVMax;
                }
            }
        }
    }
}

void OBJ_ComputeBV(tGameObjectMain* a1, int a2)
{
    tGameObjectExtended* v2;
    tGroGeneric* v3;
    tGroIntf* v4;
    int v5;
    int v6;

    if (a2 == 1 || (a1->flags2 & 0x2000000) == 0)
    {
        v2 = a1->extended;
        if (v2 && v2->light)
        {
            if (OBJ_HasNoBV(a1) || !a2)
                OBJ_ComputeLightBV(a1);
        }
        else if ((a1->flags & 2) != 0)
        {
            if (OBJ_HasNoBV(a1) || !a2)
                OBJ_ComputeAnimBV(a1);
        }
        else
        {
            if ((a1->flags & 0x4000) != 0)
            {
                v3 = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
                if (!v3)
                    return;
                v4 = v3->intf;
                if (!v4)
                    return;
                v5 = v4->id - 1;
                if (v5)
                {
                    v6 = v5 - 5;
                    if (v6)
                    {
                        if (v6 == 2 && (OBJ_HasNoBV(a1) || !a2))
                            OBJ_ComputeStaticLODObjectBV(a1, 0);
                    }
                    else if (OBJ_HasNoBV(a1) || !a2)
                    {
                        OBJ_ComputeCameraBV(a1, 0);
                    }
                }
                else if (OBJ_HasNoBV(a1) || !a2)
                {
                    OBJ_ComputeGeometricObjectBV(a1, 0);
                }
            }
            if ((a1->flags & 0x80) != 0 && (OBJ_HasNoBV(a1) || !a2))
                OBJ_ComputeWaypointBV(a1, 0);
        }
    }
}

void OBJ_GameObject_CreateExtended(tGameObjectMain* a1)
{
    a1->extended = (tGameObjectExtended*)MEM_p_Alloc(sizeof(tGameObjectExtended));
    memset(a1->extended, 0, sizeof(tGameObjectExtended));
    a1->extended->AIPriority = 127;
    a1->extended->sectorID = 0xffff;
    a1->extended->sectorElemIdx = 0xffff;
}

void OBJ_GameObject_CreateExtendedIfNot(struct sGameObjectMain* a1)
{
    if ((a1->flags & 0x2000) == 0)
    {
        OBJ_GameObject_CreateExtended(a1);
        a1->flags |= 0x2000u;
    }
}

tGaoMatrix* OBJ_pst_GetLocalMatrix(tGameObjectMain* a1)
{
    tGaoMatrix* result;

    if ((a1->flags & 0x400000) != 0)
        result = a1->basic->mtx;
    else
        result = 0;
    return result;
}

void OBJ_SightSet(struct sGameObjectMain* a1, Vec3* a2)
{
    MATH_OrientMatrix_UsingSight(a1->matrices, a2, 0);
}

void OBJ_BankingSet(struct sGameObjectMain* a1, Vec3* a3)
{
    MATH_OrientMatrix_UsingBanking(a1->matrices, 0, a3);
}

void OBJ_SightGeneralSet(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    MATH_OrientMatrix_UsingSight(a1->matrices, a2, a3);
}

void OBJ_BankingGeneralSet(struct sGameObjectMain* a1, Vec3* a2, Vec3* a3)
{
    MATH_OrientMatrix_UsingBanking(a1->matrices, a2, a3);
}

void OBJ_ComputeRecursivScale(tGameObjectMain* a1, Vec3* scale)
{
    tGameObjectMain* v2;
    tGameObjectBasic* v3;
    tGaoMatrix* v4;
    tGaoMatrix* v5;
    tGameObjectBasic* v6;
    tGaoMatrix* v7;
    Mtx4x4* v8;
    Vec3 a1a;

    v2 = a1;
    scale->x = 1.0;
    scale->y = 1.0;
    scale->z = 1.0;
    if ((a1->flags & 0x400000) != 0 && (v3 = a1->basic) != 0 && (v4 = v3->mtx) != 0 && v4->refGao2)
    {
        if ((a1->flags & 0x400000) != 0)
        {
            do
            {
                v5 = OBJ_pst_GetLocalMatrix(v2);
                MATH_GetScale(&a1a, &v5->mtx);
                scale->x = a1a.x * scale->x;
                scale->y = a1a.y * scale->y;
                scale->z = a1a.z * scale->z;
                if (v2 && (v6 = v2->basic) != 0 && (v7 = v6->mtx) != 0)
                    v2 = v7->refGao2;
                else
                    v2 = 0;
            } while ((v2->flags & 0x400000) != 0);
        }
        MATH_GetScale(&a1a, v2->matrices);
        scale->x = a1a.x * scale->x;
        scale->y = a1a.y * scale->y;
        scale->z = a1a.z * scale->z;
    }
    else
    {
        v8 = OBJ_pst_GetAbsoluteMatrix(a1);
        MATH_GetScale(scale, v8);
    }
}

void OBJ_SetAbsoluteMatrix(struct sGameObjectMain* a1, Mtx4x4* a2)
{
    Mtx4x4* v2;

    v2 = a1->matrices;
    if (v2 != a2)
        memcpy(v2, a2, sizeof(Mtx4x4));
}

void OBJ_MakeAbsoluteMatrix(tGameObjectMain* a1, Mtx4x4* a2)
{
    Mtx4x4* v3;

    v3 = a1->matrices;
    if (a2 != v3)
        memcpy(a2, v3, sizeof(Mtx4x4));
}

void OBJ_SightGet(tGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2;

    v2 = a1->matrices;
    a2->x = -v2->v2.x;
    a2->y = -v2->v2.y;
    a2->z = -v2->v2.z;
}

void OBJ_SightInitGet(struct sGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2;

    if ((a1->flags & 0x10000) != 0)
    {
        v2 = a1->matrices;
        a2->x = -v2[1].v2.x;
        a2->y = -v2[1].v2.y;
        a2->z = -v2[1].v2.z;
    }
}

void OBJ_BankingGet(tGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2; //MATH_NormalizeVector inline
    double v4;

    v2 = a1->matrices;
    v4 = sqrt(v2->v3.z * v2->v3.z + v2->v3.y * v2->v3.y + v2->v3.x * v2->v3.x);
    a2->x = (float)(1.0 / v4 * v2->v3.x);
    a2->y = (float)(1.0 / v4 * v2->v3.y);
    a2->z = (float)(1.0 / v4 * v2->v3.z);
}

void OBJ_HorizonGet(struct sGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2;
    double v3;

    v2 = a1->matrices;
    v3 = sqrt(v2->v1.z * v2->v1.z + v2->v1.y * v2->v1.y + v2->v1.x * v2->v1.x);
    a2->x = (float)(1.0 / v3 * v2->v1.x);
    a2->y = (float)(1.0 / v3 * v2->v1.y);
    a2->z = (float)(1.0 / v3 * v2->v1.z);
}

void OBJ_SetLocalPosition(struct sGameObjectMain* a1, Vec3* a2)
{
    tGaoMatrix* v2;

    if ((a1->flags & 0x400000) != 0)
    {
        v2 = a1->basic->mtx;
        v2->mtx.Pos = *a2;
        v2->mtx.mtxflag |= 2u;
        OBJ_ComputeGlobalWhenHie(a1);
    }
}

void OBJ_SetAbsolutePosition(struct sGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2;

    v2 = a1->matrices;
    v2->Pos = *a2;
    v2->mtxflag |= 2u;
}

void OBJ_SetInitialAbsolutePosition(struct sGameObjectMain* a1, Vec3* a2)
{
    Mtx4x4* v2;
    tGameObjectBasic* v3;
    Mtx4x4* v4;
    Mtx4x4* v5;
    Mtx4x4 dest;
    Mtx4x4 a1a;

    v2 = a1->matrices;
    if ((a1->flags & 0x10000) != 0)
        ++v2;
    v2->Pos = *a2;
    v2->mtxflag |= 2u;
    if ((a1->flags & 0x400000) != 0)
    {
        v3 = a1->basic;
        if (v3->mtx->refGao2)
        {
            if ((a1->flags & 0x10000) != 0)
            {
                v4 = OBJ_pst_GetAbsoluteMatrix(v3->mtx->refGao2);
                MATH_InvertMatrix(&a1a, v4);
                MATH_MulMatrixMatrix(&dest, a1->matrices + 1, &a1a);
                v5 = a1->matrices + 1;
                if (v5 != &dest)
                    memcpy(v5, &dest, sizeof(Mtx4x4));
            }
        }
    }
}

void OBJ_SetInitialAbsoluteMatrix(tGameObjectMain* a1, Mtx4x4* a2)
{
    Mtx4x4* v2;
    Mtx4x4* v3;
    Mtx4x4* v4;
    tGaoMatrix* v5;
    Mtx4x4* v6;
    Mtx4x4* v7;
    Mtx4x4 a1a;
    Mtx4x4 a3;

    if ((a1->flags & 0x10000) != 0)
    {
        v2 = a1->matrices + 1;
        if (v2 != a2)
            memcpy(v2, a2, sizeof(Mtx4x4));
        if ((a1->flags & 0x4000000) != 0)
        {
            v3 = a1->matrices;
            v4 = (a1->flags & 0x10000) != 0 ? &v3[2] : &v3[1];
            if (v4 != a2)
                memcpy(v4, a2, sizeof(Mtx4x4));
        }
        if ((a1->flags & 0x400000) != 0)
        {
            v5 = a1->basic->mtx;
            if (v5->refGao2)
            {
                if ((a1->flags & 0x10000) != 0)
                {
                    v6 = OBJ_pst_GetAbsoluteMatrix(v5->refGao2);
                    MATH_InvertMatrix(&a3, v6);
                    MATH_MulMatrixMatrix(&a1a, a1->matrices + 1, &a3);
                    v7 = a1->matrices + 1;
                    if (v7 != &a1a)
                        memcpy(v7, &a1a, sizeof(Mtx4x4));
                }
            }
        }
    }
}

void OBJ_ComputeGlobalWithLocal(struct sGameObjectMain* a1, Mtx4x4* a2, int a3)
{
    tGameObjectMain* v3; // ebx
    tGameObjectBasic* v4; // eax
    tGaoMatrix* v5; // eax
    Mtx4x4* v6; // esi
    Mtx4x4* v7; // eax
    Mtx4x4* v8; // eax
    tGameObjectMain* v9; // [esp+Ch] [ebp-54h]
    Mtx4x4 v10; // [esp+10h] [ebp-50h] BYREF

    v3 = a1;
    if ((a1->flags & 0x400000) != 0 && (v4 = a1->basic) != 0 && (v5 = v4->mtx) != 0 && v5->refGao2)
    {
        v6 = a2;
        MATH_SetIdentityMatrix(a2);
        if ((a1->flags & 0x400000) != 0)
        {
            do
            {
                v9 = v3->basic->mtx->refGao2;
                if (!v9)
                    break;
                if (&v10 != v6)
                {
                    memcpy(&v10, v6, sizeof(v10));
                    v6 = a2;
                }
                MATH_MulMatrixMatrix(v6, &v10, &v3->basic->mtx->mtx);
                v3 = v9;
            } while ((v9->flags & 0x400000) != 0);
        }
        if (a3)
        {
            MATH_CopyMatrix(&v10, v6);
            v7 = OBJ_pst_GetAbsoluteMatrix(v3);
            MATH_MulMatrixMatrix(v6, &v10, v7);
        }
    }
    else
    {
        v8 = OBJ_pst_GetAbsoluteMatrix(a1);
        if (a2 != v8)
            memcpy(a2, v8, sizeof(Mtx4x4));
    }
}

void OBJ_ComputeLocalWhenHie(struct sGameObjectMain* a1)
{
    tGaoMatrix* v1; // esi
    tGameObjectMain* v2; // eax
    Mtx4x4* v3; // eax
    Mtx4x4* v4; // eax
    unsigned int v5; // eax
    Vec3 a1a; // [esp+14h] [ebp-ACh] BYREF
    Mtx4x4 a3; // [esp+20h] [ebp-A0h] BYREF
    Mtx4x4 a2; // [esp+70h] [ebp-50h] BYREF

    if ((a1->flags & 0x400000) != 0)
    {
        v1 = a1->basic->mtx;
        v2 = v1->refGao2;
        if (v2)
        {
            if ((v2->flags & 0x400000) != 0)
            {
                OBJ_ComputeGlobalWithLocal(v2, &a2, 1);
                MATH_InvertMatrix(&a3, &a2);
            }
            else
            {
                v3 = OBJ_pst_GetAbsoluteMatrix(v1->refGao2);
                MATH_InvertMatrix(&a3, v3);
            }
            v4 = OBJ_pst_GetAbsoluteMatrix(a1);
            MATH_MulMatrixMatrix(&v1->mtx, v4, &a3);
            if ((v1->mtx.mtxflag & 8) != 0)
            {
                MATH_GetScale(&a1a, &v1->mtx);
                if (MATH_b_EqVectorWithEpsilon(&MATH_gst_IdentityVector, &a1a, 0.001f)
                    || MATH_b_EqVectorWithEpsilon(&MATH_gst_NulVector, &a1a, 0.001f))
                {
                    v5 = v1->mtx.mtxflag & ~8u;
                    v1->mtx._m14 = 0.0;
                    v1->mtx._m24 = 0.0;
                    v1->mtx._m34 = 0.0;
                    v1->mtx.mtxflag = v5;
                }
            }
            MATH_Orthonormalize(&v1->mtx);
        }
    }
}

Vec3* OBJ_pst_GetPos(struct sGameObjectMain* a1)
{
    Mtx4x4* v1;
    Vec3* result;

    v1 = a1->matrices;
    if ((a1->flags & 0x10000) != 0)
        result = &v1[1].Pos;
    else
        result = &v1->Pos;
    return result;
}

struct sDyn* OBJ_pst_GetDynaStruct(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tDyn* result;

    v1 = a1->basic;
    if (v1)
        result = v1->dyn;
    else
        result = NULL;
    return result;
}

// TODO: ell
int OBJ_b_test_flag(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest)
{
    int v4;
    BOOL v5;
    int v7;
    int v8;

    switch (whatToTest)
    {
    case 0:
        v4 = gao->flags;
        v5 = (v4 & flagMustHave) == 0;
        if (!v5)
            if ((v4 & flagMustNothave) == 0)
                return 1;
        return 0;
    case 1:
        v4 = gao->designFlags;
        if (!(byte)v4 && flagMustHave == -1)
            return 1;
        v4 = (uint8_t)v4;
        v5 = (uint8_t)(v4 & flagMustHave) == 0;
        if (!v5)
            if ((v4 & flagMustNothave) == 0)
                return 1;
        return 0;
    case 2:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        v4 = gao->extended->fields->designFlags;
        v7 = flagMustHave;
        if (!v4 && flagMustHave == -1)
            return 1;
        v5 = (v4 & v7) == 0;
        if (!v5)
            if ((v4 & flagMustNothave) == 0)
                return 1;
        return 0;
    case 3:
        if ((gao->flags & 0x2000) == 0)
            return 0;
        v4 = gao->extended->capa;
        v7 = flagMustHave;
        if (!(int16_t)v4 && flagMustHave == -1)
            return 1;
        v4 = (unsigned __int16)v4;
        v5 = (v4 & v7) == 0;
        if (!v5)
            if ((v4 & flagMustNothave) == 0)
                return 1;
        return 0;
    case 4:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        v4 = gao->extended->fields->designDesFlags;
        v7 = flagMustHave;
        if (!v4 && flagMustHave == -1)
            return 1;
        v5 = (v4 & v7) == 0;
        if (!v5)
            if ((v4 & flagMustNothave) == 0)
                return 1;
        return 0;
    case 5:
        v4 = gao->flags2;
        v5 = (v4 & flagMustHave) == 0;
        if (!v5)
            if ((v4 & flagMustNothave) == 0)
                return 1;
        return 0;
    case 10:
        v4 = gao->flags;
        if ((flagMustHave & v4) != flagMustHave)
            return 0;
        if ((v4 & flagMustNothave) == 0)
            return 1;
        return 0;
    case 11:
        v4 = gao->designFlags;
        if (!(byte)v4 && flagMustHave == -1)
            return 1;
        v4 = (uint8_t)v4;
        if ((uint8_t)(flagMustHave & v4) == flagMustHave)
        if ((v4 & flagMustNothave) == 0)
            return 1;    
        return 0;
    case 12:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        v4 = gao->extended->fields->designFlags;
        v8 = flagMustHave;
        if (!v4 && flagMustHave == -1)
            return 1;
        if ((v8 & v4) == v8)
        {
            if ((v4 & flagMustNothave) == 0)
                return 1;
        }
        return 0;
    case 13:
        if ((gao->flags & 0x2000) == 0)
            return 0;
        v4 = gao->extended->capa;
        v8 = flagMustHave;
        if (!(int16_t)v4 && flagMustHave == -1)
            return 1;
        v4 = (unsigned __int16)v4;
        if ((v8 & v4) == v8)
        {
            if ((v4 & flagMustNothave) == 0)
                return 1;
        }
        return 0;
    case 14:
        if ((gao->flags & 0x2000) == 0 || (gao->flags & 0x40) == 0)
            return 0;
        v4 = gao->extended->fields->designDesFlags;
        v8 = flagMustHave;
        if (!v4 && flagMustHave == -1)
            return 1;
        if ((v8 & v4) == v8)
        {
            if ((v4 & flagMustNothave) == 0)
                return 1;
        }
        return 0;
    case 15:
        v4 = gao->flags2;
        if ((flagMustHave & v4) != flagMustHave)
            return 0;
        if ((v4 & flagMustNothave) == 0)
            return 1;
        return 0;
    default:
        return 0;
    }
}

void OBJ_putInGroup(tGameObjectMain* a1, tGroup* a2)
{
    tGameObjectExtended* v2;

    v2 = a1->extended;
    if (v2)
    {
        if (a2)
        {
            a1->flags |= 0x800000u;
            v2->grp = a2;
            ++a2->groupElemNum;
        }
    }
}

void OBJ_AddInGroup(struct sGroup* a1, struct sGameObjectMain* object)
{
    TAB_PFTable_AddElemToTable(a1->tablePF, object, 0);
}

struct sGameObjectMain* OBJ_pst_GroupGetByRank(struct sGroup* a1, int idx)
{
    tableElemStructPF* v2;
    tableElemStructPF* v3;

    if (!a1)
        return 0;
    v2 = (tableElemStructPF *)(a1->tablePF->firstElemPtr);
    v3 = ((tableElemStructPF *)(a1->tablePF->nextElemPtr)) - 1;
    if (v2 > v3)
        return 0;
    while (idx)
    {
        ++v2;
        --idx;
        if (v2 > v3)
            return 0;
    }
    return (tGameObjectMain*)v2->objectPtr;
}

BOOL OBJ_b_IsChild(struct sGameObjectMain* a1)
{
    BOOL result;
    tGameObjectBasic* v2;
    tGaoMatrix* v3;

    if ((a1->flags & 0x400000) != 0 && (v2 = a1->basic) != 0 && (v3 = v2->mtx) != 0)
        result = v3->refGao2 != 0;
    else
        result = 0;
    return result;
}

struct sGameObjectMain* OBJ_GameObject_Create(int flags)
{
    tGameObjectMain* gaomain;
    Mtx4x4* v2;
    int v3;
    tGameObjectBasic* v4;
    tGameObjectVisualObject* v5;
    tDyn* v6;
    int v7;
    int v8;
    tGameObjectCollMain* v9;
    int v10;

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

    v2 = gaomain->matrices;
    v3 = v2->mtxflag & 2;
    v2->v1.x = 1.0;
    v2->v1.y = 0.0;                               //  MATH_SetIdentityMatrix
    v2->v1.z = 0.0;
    v2->_m14 = 0.0;
    v2->v2.x = 0.0;
    v2->v2.y = 1.0;
    v2->v2.z = 0.0;
    v2->_m24 = 0.0;
    v2->v3.x = 0.0;
    v2->v3.y = 0.0;
    v2->v3.z = 1.0;
    v2->_m34 = 0.0;
    v2->Pos.x = 0.0;
    v2->Pos.y = 0.0;
    v2->Pos.z = 0.0;
    v2->w = 1.0;
    v2->mtxflag = v3;
    if ((gaomain->flags & 0x1000) != 0)
    {
        gaomain->basic = (tGameObjectBasic*)MEM_p_Alloc(sizeof(tGameObjectBasic));
        LOA_ul_FileTypeSize_37 += 20;
        v4 = gaomain->basic;
        v4->actorObjectOrVisualObject = 0;
        v4->dyn = 0;
        v4->mtx = 0;
        v4->multiPartData = 0;
        v4->extraMaterial = 0;
        if ((gaomain->flags & 0x4000) != 0)
        {
            v4->actorObjectOrVisualObject = (tGameObjectVisualObject*)MEM_p_Alloc(sizeof(tGameObjectVisualObject));
            LOA_ul_FileTypeSize_37 += 20;
            v5 = v4->actorObjectOrVisualObject;
            v5->GROGEOOrSomethingElse = 0;
            v5->GROMAT = 0;
            v5->objDrawMask = 0;
            v5->RLIVertexColors = 0;
            v5->renderOnlyWithGFXRender = 0;
            v5->culledFlagFromCullingObject = 0;
            v5->vmilyenDistanceLightAttenuationFactor = 0;
            v5->boneDisplayOrder_zListBufferNum = 0;
            ((tGameObjectVisualObject*)(v4->actorObjectOrVisualObject))->objDrawMask = 0xffffffff;
        }
        if ((gaomain->flags & 2) != 0)
            v4->actorObjectOrVisualObject = ANI_pst_CreateGameObjectAnim();
        if ((gaomain->flags & 4) != 0)
        {
            v6 = DYN_pst_Dyna_Create();
            v4->dyn = v6;
            DYN_Dyna_Init(v6, 65, 1.0, 3.402822e38f, 3.402822e38f, &MATH_gst_NulVector, &gaomain->matrices->Pos);
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
        v7 = LOA_ul_FileTypeSize_37 + 72;
        v8 = 18;
    }
    else
    {
        gaomain->boundingVolume = (tGameObjectBV*)MEM_p_Alloc(48u);
        v7 = LOA_ul_FileTypeSize_37 + 48;
        v8 = 12;
    }
    LOA_ul_FileTypeSize_37 = v7;
    memset(gaomain->boundingVolume, 0, 4 * v8);
    if ((gaomain->flags & 0x2000) != 0)
    {
        OBJ_GameObject_CreateExtended(gaomain);
        if ((gaomain->flags & 0x700) != 0)
        {
            gaomain->extended->coll = (tGameObjectCollMain*)MEM_p_Alloc(sizeof(tGameObjectCollMain));
            LOA_ul_FileTypeSize_37 += sizeof(tGameObjectCollMain);
            v9 = gaomain->extended->coll;
            v9->detectionList = 0;
            v9->colInstance = 0;
            v9->colMap = 0;
        }
        if ((gaomain->flags & 0x20) != 0)
        {
            v10 = gaomain->flags2;
            if ((v10 & 0x20) == 0)
            {
                gaomain->flags2 = v10 | 0x20;
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

void OBJ_ReinitMatrixStorageByFlags(tGameObjectMain* a1, Mtx4x4* a2)
{
    int v2;
    Mtx4x4* v3;
    Mtx4x4* v4;
    int v5;
    Mtx4x4* v6;
    Mtx4x4* v7;
    Mtx4x4 v8;

    v2 = a1->flags;
    if ((a1->flags & 0x4000000) != 0)
    {
        v3 = a1->matrices;
        if ((v2 & 0x10000) != 0)
            v4 = v3 + 2;
        else
            v4 = v3 + 1;
        if (v4 != a2)
            memcpy(v4, a2, sizeof(Mtx4x4));
    }
    else
    {
        v5 = v2 | 0x4000000;
        a1->flags = v5;
        if ((v5 & 0x10000) != 0)
        {
            if (&v8 != a2)
                memcpy(&v8, a2, sizeof(v8));
            v6 = MATH_Matrix_ReallocArray(a1->matrices, 3);
            v7 = v6 + 2;
        }
        else
        {
            if (&v8 != a2)
                memcpy(&v8, a2, sizeof(v8));
            v6 = MATH_Matrix_ReallocArray(a1->matrices, 2);
            v7 = v6 + 1;
        }
        a1->matrices = v6;
        if (v7 != &v8)
            memcpy(v7, &v8, sizeof(Mtx4x4));
    }
}

void OBJ_RestoreInitialPos(struct sGameObjectMain* a1)
{
    int v1;
    tGameObjectBasic* v2;
    tGaoMatrix* v3;
    Mtx4x4* v5;
    Mtx4x4* v6;
    tGaoMatrix* v7;
    Mtx4x4* v9;

    v1 = a1->flags;
    if ((a1->flags & 0x400000) != 0 && (v2 = a1->basic) != 0 && (v3 = v2->mtx) != 0 && v3->refGao)
    {
        if ((v1 & 0x10000) != 0)
            MATH_CopyMatrix(&v3->mtx, a1->matrices + 1);
        v9 = OBJ_pst_GetAbsoluteMatrix(a1->basic->mtx->refGao);
        v5 = OBJ_pst_GetAbsoluteMatrix(a1);
        MATH_MulMatrixMatrix(v5, (Mtx4x4*)(a1->basic->mtx), v9); //FIXME a1->basic->mtx nem biztos hogy jó, kidebuggolni
    }
    else if ((v1 & 0x10000) != 0)
    {
        v6 = a1->matrices;
        if (v6 != &v6[1])
            memcpy(v6, &v6[1], sizeof(Mtx4x4));
    }
    if ((a1->flags & 2) != 0)
    {
        if ((a1->flags & 0x400000) != 0)
            v7 = OBJ_pst_GetLocalMatrix(a1);
        else
            v7 = (tGaoMatrix*)OBJ_pst_GetAbsoluteMatrix(a1);
        OBJ_ReinitMatrixStorageByFlags(a1, &v7->mtx);
    }
}

void OBJ_Reinit(struct sGameObjectMain* a1)
{
    tGaoMatrix* v1;
    tGameObjectBasic* v2;
    tGameObjectVisualObject* v3;
    tGameObjectBasic* v5;
    tGroGeneric* v4;
    tGroGeneric* v6;

    if ((a1->flags & 1) == 0 && (a1->flags & 0x400000) != 0)
    {
        v1 = a1->basic->mtx;
        if (v1->refGao2)
            OBJ_Reinit(v1->refGao2);
    }
    OBJ_RestoreInitialPos(a1);
    v2 = a1->basic;
    if (v2)
    {
        v2->extraMaterial = 0;
        if ((a1->flags & 0x4000) == 0) {
            MDF_Reinit(a1);
            return;
        }
        v3 = a1->basic->actorObjectOrVisualObject;
        if (v3)
            v3->objDrawMask |= 0x60100000u;
    }
    if ((a1->flags & 0x4000) != 0)
    {
        v4 = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
        if (v4 && v4->intf)
            v4->intf->Reinit(v4);
        v5 = a1->basic;
        v6 = ((tGameObjectVisualObject*)(v5->actorObjectOrVisualObject))->GROMAT;
        if (v6 && v6->intf)
            v6->intf->Reinit(v6);
    }

    MDF_Reinit(a1);
}

void OBJ_CheckFlagsAndActivateObject(tGameObjectMain* a1, int a2)
{
    tWorld* v2;
    int v3;

    if (a1)
    {
        v2 = WOR_gpst_CurrentWorld;
        a1->flags2 &= 0x0000ffff;
        v3 = a2 & 0xFFFF0000 | a1->flags2;
        a1->flags2 = v3;
        if ((a2 & 0x8000000) != 0 && (v3 & 1) == 0)
            WOR_ActivateObjectIfFlagsAllow(a1, v2);
        if ((a2 & 0x400000) != 0)
            v2->tablesManagerAndEOTNeeded = 1;
        if ((a2 & 0x100000) != 0 && (a1->flags2 & 0x20) == 0)
        {
            COL_AllocDetectionList(a1);
            a1->flags2 |= 0x20u;
        }
        if ((a2 & 0x100000) == 0 && (a1->flags2 & 0x20) != 0 && (a1->flags & 0x20) == 0)
        {
            COL_FreeDetectionList(a1->extended->coll);
            a1->extended->coll->detectionList = 0;
            a1->flags2 &= ~0x20u;
        }
        OBJ_UpdateStatusFlagRTL(a1);
    }
}

void OBJ_SetProcess(tGameObjectMain* a1, char a2, char a3)
{
    if ((a2 & 1) != 0)
        a1->process &= ~a3;
    else
        a1->process |= a3;
}

void OBJ_FreeGroupGrl(tableDescStruct* a1)
{
    if (a1)
    {
        TAB_PFtable_Close(a1);
        LOA_DeleteAddress((int)a1);
        MEM_Free(a1);
    }
}

void OBJ_FreeGroup(tGroup* a1)
{
    OBJ_FreeGroupGrl(a1->tablePF);
    MEM_Free(a1);
    LOA_DeleteAddress((int)a1);
}

void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown)
{
    tGroGeneric* v2;
    tGroGeneric* v3;
    tGameObjectVisualObject* v4;
    tGameObjectMultiPartData* v5;
    tGroGeneric* v6;
    tModifier* v7;
    tModifier* v8;

    GRPSectDelObj(gao);
    if ((gao->flags & 8) == 0)
        MATH_Matrix_Free(gao->matrices);
    if ((gao->flags & 0x1000) != 0)
    {
        if ((gao->flags & 0x4000) != 0)
        {
            v2 = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
            if (v2)
            {
                if (refCountDown)
                    v2->intf->AddRef(v2, -1);
                v2->intf->Destroy(v2);
            }
            v3 = ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->GROMAT;
            if (v3)
            {
                if (refCountDown)
                    v3->intf->AddRef(v3, -1);
                v3->intf->Destroy(v3);
            }
            v4 = gao->basic->actorObjectOrVisualObject;
            if (v4->RLIVertexColors)
                MEM_Free(v4->RLIVertexColors);
            v4->RLIVertexColors = 0;
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
            v5 = gao->basic->multiPartData;
            if (v5->elemNum)
                MEM_FreeAlign(v5->multipartGaoObjectPartsOrMatrices);
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
            v6 = gao->extended->light;
            if (v6)
            {
                v6->intf->AddRef(v6, -1);
                v6->intf->Destroy(v6);
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
        v7 = gao->extended->mdf;
        if (v7)
        {
            do
            {
                v8 = v7->nextElem;
                MDF_Modifier_Destroy(v7);
                v7 = v8;
            } while (v8);
        }

        if ((gao->flags & 0x8000000) != 0)
            SND_FreeMainStruct(gao);
        MEM_Free(gao->extended);
    }
    MEM_Free(gao->boundingVolume);
    LOA_DeleteAddress((int)gao);
    MEM_Free(gao);
}

Mtx4x4* OBJ_pst_GetFlashMatrix(struct sGameObjectMain* a1)
{
    Mtx4x4* v1;
    Mtx4x4* result;

    if ((a1->flags & 0x4000000) == 0)
        return 0;
    v1 = a1->matrices;
    if ((a1->flags & 0x10000) != 0)
        result = v1 + 2;
    else
        result = v1 + 1;
    return result;
}

void OBJ_ChangeIdentityFlags(tGameObjectMain* a1, unsigned int a2, int a3)
{
    int v3;
    unsigned int v4;
    tGameObjectBasic* v5;
    int v6; // fixed: char
    Mtx4x4* v7;
    Mtx4x4* v8;
    Mtx4x4* v9;
    Mtx4x4* v10;
    Mtx4x4* v11;
    Mtx4x4* v12;
    tGameObjectVisualObject* v13;
    tGroGeneric* v14;
    tGroGeneric* v15;
    tGameObjectBasic* v16;
    tDyn* dyn;
    tGameObjectBasic* v18;
    tGameObjectMain* v19;
    Mtx4x4* v20;
    tGameObjectMain* v21;
    int v22;
    tGameObjectBasic* v23;
    tGameObjectMain* v24;
    Mtx4x4* v25;
    unsigned int v26;
    tGameObjectBasic* v27;
    tGameObjectMultiPartData* v28;
    tGameObjectMultiPartData* v29;
    int v30;
    int v31;
    tGameObjectMultiPartData* v32;
    int v33;
    int v34;
    Mtx4x4** v35;
    Mtx4x4* v36;
    unsigned int v37;
    tWay* v38;
    tGroGeneric* v39;
    tGroGeneric* v40;
    tGameObjectBV* v41;
    tWorld* v42;
    tGameObjectMain* v43;
    int v44;
    Mtx4x4 a2a;
    Mtx4x4 v46;

    v44 = 0;
    if (a1)
    {
        v42 = WOR_gpst_CurrentWorld;
        v3 = a3;
        if (a3 == -1)
        {
            a3 = a1->flags;
            v3 = a1->flags;
        }
        v4 = a2;
        if ((a2 & 0x1000000) != 0)
        {
            v4 = a2 | 0x200000;
            a2 |= 0x200000u;
        }
        if ((v4 & 0x1604000) != 0 || (v4 & 6) != 0)
        {
            v4 |= 0x1000u;
            a2 = v4;
        }
        if ((v4 & 0x1000) != 0 && (v3 & 0x1000) == 0)
        {
            v5 = (tGameObjectBasic*)MEM_p_Alloc(sizeof(tGameObjectBasic));
            a1->basic = v5;
            v5->actorObjectOrVisualObject = 0;
            v5->dyn = 0;
            v5->mtx = 0;
            v5->multiPartData = 0;
            v5->extraMaterial = 0;
            v3 = a3;
        }
        v6 = a2;
        if ((a2 & 0x70) != 0 || (a2 & 0xA048700) != 0)
        {
            v6 = a2 | 0x2000u;
            a2 |= 0x2000u;
        }
        if ((v6 & 0x2000) != 0 && (v3 & 0x2000) == 0)
        {
            OBJ_GameObject_CreateExtended(a1);
            v3 = a3;
        }
        if ((a2 & 0x10000) != 0)
        {
            if ((v3 & 0x10000) != 0)
                goto LABEL_33;
            a1->flags |= 0x10000u;
            if ((a2 & 0x4000000) != 0)
            {
                v7 = (Mtx4x4*)MATH_Matrix_ReallocArray(a1->matrices, 3);
                v8 = v7 + 1;
                a1->matrices = v7;
                v9 = v7 + 2;
                if (v9 != v8)
                    memcpy(v9, v8, sizeof(Mtx4x4));
                OBJ_SetInitialAbsoluteMatrix(a1, a1->matrices);
            }
            else
            {
                v10 = (Mtx4x4*)MATH_Matrix_ReallocArray(a1->matrices, 2);
                a1->matrices = v10;
                OBJ_SetInitialAbsoluteMatrix(a1, v10);
            }
        }
        else
        {
            if ((v3 & 0x10000) == 0)
                goto LABEL_33;
            if ((a2 & 0x4000000) != 0)
            {
                v11 = a1->matrices;
                if (&v11[1] != &v11[2])
                    memcpy(&v11[1], &v11[2], sizeof(Mtx4x4));
                v12 = (Mtx4x4*)MATH_Matrix_ReallocArray(a1->matrices, 2);
            }
            else
            {
                v12 = (Mtx4x4*)MATH_Matrix_ReallocArray(a1->matrices, 1);
            }
            a1->matrices = v12;
        }
        v3 = a3;
    LABEL_33:
        if ((a2 & 0x4000) != 0)
        {
            if ((v3 & 0x4000) != 0)
                goto LABEL_45;
            a1->basic->actorObjectOrVisualObject = (tGameObjectVisualObject*)MEM_p_Alloc(sizeof(tGameObjectVisualObject));
            v13 = a1->basic->actorObjectOrVisualObject;
            v13->GROGEOOrSomethingElse = 0;
            v13->GROMAT = 0;
            v13->objDrawMask = 0;
            v13->RLIVertexColors = 0;
            v13->renderOnlyWithGFXRender = 0;
            v13->culledFlagFromCullingObject = 0;
            v13->vmilyenDistanceLightAttenuationFactor;
            v13->boneDisplayOrder_zListBufferNum;
            ((tGameObjectVisualObject *)(a1->basic->actorObjectOrVisualObject))->objDrawMask = 0xffffffff;
        }
        else
        {
            if ((v3 & 0x4000) == 0)
                goto LABEL_45;
            v14 = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse;
            if (v14)
            {
                v14->intf->AddRef(v14, -1);
                v14->intf->Destroy(v14);
            }
            v15 = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            if (v15)
            {
                v15->intf->AddRef(v15, -1);
                v15->intf->Destroy(v15);
            }
            v16 = a1->basic;
            if (((tGameObjectVisualObject*)(v16->actorObjectOrVisualObject))->RLIVertexColors)
                MEM_Free(((tGameObjectVisualObject*)(v16->actorObjectOrVisualObject))->RLIVertexColors);
            MEM_Free(a1->basic->actorObjectOrVisualObject);
            a1->basic->actorObjectOrVisualObject = 0;
        }
        v3 = a3;
    LABEL_45:
        if ((a2 & 2) != 0)
        {
            if ((v3 & 2) != 0)
                goto LABEL_51;
            a1->basic->actorObjectOrVisualObject = ANI_pst_CreateGameObjectAnim();
        }
        else
        {
            if ((v3 & 2) == 0)
                goto LABEL_51;
            ANI_Free(a1->basic->actorObjectOrVisualObject);
            a1->basic->actorObjectOrVisualObject = 0;
        }
        v3 = a3;
    LABEL_51:
        if ((a2 & 4) != 0)
        {
            if ((v3 & 4) != 0)
                goto LABEL_57;
            dyn = DYN_pst_Dyna_Create();
            DYN_Dyna_Init(dyn, 65, 1.0, 3.402822e38f, 3.402822e38f, &MATH_gst_NulVector, &a1->matrices->Pos);
            a1->basic->dyn = dyn;
            OBJ_ResetProcess(a1, WOR_gpst_CurrentWorld->dynCallCount, 64);
        }
        else
        {
            if ((v3 & 4) == 0)
                goto LABEL_57;
            DYN_Dyna_Close(a1->basic->dyn);
            a1->basic->dyn = 0;
            EOT_UpdateIdentityFlagsOfTable(&v42->activatedObjectsTable4PF);
            EOT_RemoveObjectFromTableIfBadFlag(&v42->eot.dynamicObjectsTablePF, 4);
        }
        v3 = a3;
    LABEL_57:
        if ((a2 & 0x400000) != 0)
        {
            if ((v3 & 0x400000) == 0)
            {
                a1->basic->mtx = (tGaoMatrix*)MEM_p_AllocAligned(96, 16);
                a1->basic->mtx->refGao = 0;
                a1->basic->mtx->refGao2 = 0;
                MATH_SetIdentityMatrix(&a1->basic->mtx->mtx);
            LABEL_73:
                v3 = a3;
                goto LABEL_74;
            }
        }
        else if ((v3 & 0x400000) != 0)
        {
            if ((a2 & 0x10000) != 0)
            {
                v18 = a1->basic;
                v19 = v18->mtx->refGao;
                if (v19)
                {
                    if ((v19->flags & 0x10000) != 0)
                    {
                        v20 = OBJ_pst_GetAbsoluteMatrix(v18->mtx->refGao);
                        MATH_MulMatrixMatrix(&a2a, a1->matrices + 1, v20 + 1);
                        v21 = a1->basic->mtx->refGao;
                        v22 = v21->flags;
                        v43 = v21;
                        if ((v21->flags & 0x10000) != 0)
                        {
                            while ((v22 & 0x400000) != 0)
                            {
                                v23 = v21->basic;
                                v24 = v23->mtx->refGao;
                                if (!v24)
                                    break;
                                if ((v24->flags & 0x10000) == 0)
                                    break;
                                v25 = OBJ_pst_GetAbsoluteMatrix(v23->mtx->refGao);
                                MATH_MulMatrixMatrix(&v46, &a2a, v25 + 1);
                                memcpy(&a2a, &v46, sizeof(a2a));
                                v43 = v43->basic->mtx->refGao;
                                v22 = v43->flags;
                                if ((v43->flags & 0x10000) == 0)
                                    break;
                                v21 = v43;
                            }
                        }
                        MATH_CopyMatrix(a1->matrices + 1, &a2a);
                    }
                }
            }
            MEM_FreeAlign(a1->basic->mtx);
            a1->basic->mtx = 0;
            goto LABEL_73;
        }
    LABEL_74:
        v26 = a2;
        if ((a2 & 0x1000000) != 0 && (v3 & 0x200000) == 0)
        {
            v26 = a2 | 0x200000;
            a2 |= 0x200000u;
        }
        if ((v26 & 0x200000) != 0)
        {
            if ((v3 & 0x200000) != 0)
                goto LABEL_85;
            a1->basic->multiPartData = (tGameObjectMultiPartData*)MEM_p_Alloc(sizeof(tGameObjectMultiPartData));
            a1->basic->multiPartData->elemNum = 0;
            a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices = 0;
        }
        else
        {
            if ((v3 & 0x200000) == 0)
                goto LABEL_85;
            v27 = a1->basic;
            a1->flags |= 0x200000u;
            v28 = v27->multiPartData;
            if (v28->elemNum)
                MEM_Free(v28->multipartGaoObjectPartsOrMatrices);
            MEM_Free(a1->basic->multiPartData);
            a1->basic->multiPartData = 0;
        }
        v3 = a3;
    LABEL_85:
        if ((a2 & 0x1000000) != 0)
        {
            if ((v3 & 0x1000000) == 0)
            {
                v29 = a1->basic->multiPartData;
                if (v29->elemNum)
                {
                    MEM_Free(v29->multipartGaoObjectPartsOrMatrices);
                    a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_Alloc(sizeof(tMultiPartGameObjectLinks) * a1->basic->multiPartData->elemNum);
                    v30 = 0;
                    if (a1->basic->multiPartData->elemNum > 0)
                    {
                        v31 = 0;
                        do
                        {
                            ((tMultiPartGameObjectLinks**)(a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v30]->mtxPtr = 0;
                            ++v30;
                        } while (v30 < a1->basic->multiPartData->elemNum);
                    }
                LABEL_97:
                    v3 = a3;
                    goto LABEL_98;
                }
            }
        }
        else if ((v3 & 0x1000000) != 0)
        {
            v32 = a1->basic->multiPartData;
            if (v32->elemNum)
            {
                MEM_Free(v32->multipartGaoObjectPartsOrMatrices);
                a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(sizeof(Mtx4x4) * a1->basic->multiPartData->elemNum, 16);
                v33 = 0;
                if (a1->basic->multiPartData->elemNum > 0)
                {
                    v34 = 0;
                    do
                    {
                        v35 = (Mtx4x4**)a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices;
                        v36 = v35[v33];
                        v36->mtxflag &= 2;
                        v36->v1.x = 1.0;
                        v36->v1.y = 0.0;
                        v36->v1.z = 0.0;
                        v36->_m14 = 0.0;
                        v36->v2.x = 0.0;
                        v36->v2.y = 1.0;
                        v36->v2.z = 0.0;
                        v36->_m24 = 0.0;
                        v36->v3.x = 0.0;
                        v36->v3.y = 0.0;
                        v36->v3.z = 1.0;
                        v36->_m34 = 0.0;
                        v36->Pos.x = 0.0;
                        v36->Pos.y = 0.0;
                        v36->Pos.z = 0.0;
                        v36->w = 1.0;
                        ++v33;
                    } while (v33 < a1->basic->multiPartData->elemNum);
                }
                goto LABEL_97;
            }
        }
    LABEL_98:
        if ((a2 & 0x40) != 0)
        {
            if ((v3 & 0x40) != 0)
                goto LABEL_104;
            a1->extended->fields = (tGameObjectDesignFields*)MEM_p_Alloc(0x58u);
            memset(a1->extended->fields, 0, sizeof(tGameObjectDesignFields));
        }
        else
        {
            if ((v3 & 0x40) == 0)
                goto LABEL_104;
            MEM_Free(a1->extended->fields);
            a1->extended->fields = 0;
        }
        v3 = a3;
    LABEL_104:
        if ((a2 & 0x8000) == 0)
        {
            if ((v3 & 0x8000) != 0)
            {
                MEM_Free(a1->extended->messages);
                a1->extended->messages = 0;
                goto LABEL_109;
            }
        }
        else if ((v3 & 0x8000) == 0)
        {
            a1->extended->messages = (tGameObjectMessages*)MEM_p_Alloc(0x108u);
            memset(a1->extended->messages, 0, sizeof(tGameObjectMessages));
        LABEL_109:
            v3 = a3;
            goto LABEL_110;
        }
    LABEL_110:
        if ((a2 & 0x100) != 0 && (v3 & 0x100) == 0)
        {
            if ((a2 & 0x4000) != 0)
            {
                COL_AllocColMap(a1);
                a1->flags2 |= 0x80u;
                INT_SnP_AddObject(a1, v42->snp);
                INT_SnP_InsertionSort(v42->snp, 0);
                v3 = a3;
            }
            else
            {
                a2 &= ~0x100u;
                a1->flags2 &= ~0x80u;
            }
        }
        if ((a2 & 0x100) == 0 && (v3 & 0x100) != 0)
        {
            COL_FreeColMap(a1->extended->coll);
            v3 = a3;
        }
        v37 = a2;
        if ((a2 & 0x200) != 0 && (v3 & 0x200) == 0)
        {
            v37 = a2 & ~0x200u;
            a2 &= ~0x200u;
        }
        if ((v37 & 0x400) != 0 && (v3 & 0x400) == 0)
        {
            v37 &= ~0x400u;
            a2 = v37;
        }
        if ((v37 & 0x200) == 0 && (v3 & 0x200) != 0)
        {
            a2 = v37 & ~0x400u;
            COL_FreeInstance(a1->extended->coll);
            v3 = a3;
        }
        if ((a2 & 0x400) == 0 && (v3 & 0x400) != 0)
        {
            a2 &= ~0x200;
            COL_FreeInstance(a1->extended->coll);
            v3 = a3;
        }
        if ((a2 & 0x20) != 0 && (v3 & 0x20) == 0)
        {
            COL_AllocDetectionList(a1);
            a1->flags2 |= 0x20u;
            v3 = a3;
        }
        if ((a2 & 0x20) != 0)
        {
            if ((v3 & 0x20) == 0)
                a2 &= ~0x20u;
        }
        else
        {
            if ((v3 & 0x20) != 0 && (a1->flags2 & 0x100000) == 0)
            {
                COL_FreeDetectionList(a1->extended->coll);
                a1->extended->coll->detectionList = 0;
                a1->flags2 &= ~0x20u;
                v3 = a3;
            }
            if ((v3 & 0x20) != 0)
            {
                AI_FreeInstance(a1->extended->AIInstance);
                a1->extended->AIInstance = 0;
                EOT_UpdateIdentityFlagsOfTable(&v42->activatedObjectsTable4PF);
                EOT_RemoveObjectFromTableIfBadFlag(&v42->eot.AICallbacksTablePF, 32);
                v3 = a3;
            }
        }
        if ((a2 & 0x800000) != 0)
        {
            if ((v3 & 0x800000) == 0)
                a2 &= 0xFF7FFFFF;
        }
        else if ((v3 & 0x800000) != 0)
        {
            GRP_ObjDetachGroup(a1);
            v3 = a3;
        }
        if ((a2 & 0x2000000) != 0)
        {
            if ((v3 & 0x2000000) != 0)
                goto LABEL_153;
            EVE_AllocateMainStruct(a1);
        }
        else
        {
            if ((v3 & 0x2000000) == 0)
                goto LABEL_153;
            EVE_FreeMainStruct(a1);
            EOT_UpdateIdentityFlagsOfTable(&v42->activatedObjectsTable4PF);
            EOT_RemoveObjectFromTableIfBadFlag(&v42->eot.eventGameObjectsTablePF, 0x2000000);
        }
        v3 = a3;
    LABEL_153:
        if ((a2 & 0x8000000) != 0)
        {
            if ((v3 & 0x8000000) == 0)
                a2 &= ~0x8000000u;
        }
        else if ((v3 & 0x8000000) != 0)
        {
            a1->flags |= 0x8000000u;
            SND_FreeMainStruct(a1);
            a1->flags &= ~0x8000000u;
            v3 = a3;
        }
        if ((a2 & 0x40000) != 0)
        {
            if ((v3 & 0x40000) != 0)
                goto LABEL_164;
            a1->extended->way = (tWay*)MEM_p_Alloc(8u);
            v38 = a1->extended->way;
            v38->wayElems = 0;
            v38->wayElemNum = 0;
        }
        else
        {
            if ((v3 & 0x40000) == 0)
                goto LABEL_164;
            WAY_FreeStruct(a1->extended->way);
            a1->extended->way = 0;
        }
        v3 = a3;
    LABEL_164:
        if ((a2 & 0x10) != 0)
        {
            if ((v3 & 0x10) == 0)
            {
                v39 = (tGroGeneric*)GRO_pst_Struct_Create(2);
                if (TAB_ul_Ptable_GetElemIndexWithPointer(&v42->worldGROTable, v39) == -1)
                {
                    TAB_PTable_AddElemToTable(&v42->worldGROTable, v39);
                    v39->intf->AddRef(v39, 1);
                }
                v39->intf->AddRef(v39, 1);
                v3 = a3;
                a1->extended->light = v39;
            }
        }
        else if ((v3 & 0x10) != 0)
        {
            v40 = a1->extended->light;
            if (v40)
            {
                v40->intf->AddRef(v40, -1);
                v40->intf->Destroy(v40);
                v3 = a3;
            }
            a1->extended->light = 0;
        }
        if ((a2 & 0x80000) != 0)
        {
            if ((v3 & 0x80000) != 0)
                goto LABEL_179;
            v41 = (tGameObjectBV*)MEM_p_Realloc(a1->boundingVolume, 0x48u);
        }
        else
        {
            if ((v3 & 0x80000) == 0)
                goto LABEL_179;
            v41 = (tGameObjectBV*)MEM_p_Realloc(a1->boundingVolume, 0x30u);
        }
        a1->boundingVolume = v41;
        v44 = 1;
    LABEL_179:
        a1->flags = a2;
        OBJ_UpdateStatusFlagRTL(a1);
        if (v44)
            OBJ_ComputeBV(a1, 0);
    }
}

void OBJ_GameObject_AllocateGizmo(struct sGameObjectMain* a1, int a2, int a3)
{
    int v3;
    int v4;
    unsigned int v5;
    tGameObjectMultiPartData* v6;
    int v7;

    if (a3)
    {
        if (a1)
            v3 = a1->flags | 0x1200000;
        else
            v3 = 0x1200000;
    }
    else
    {
        if (a1)
            v4 = a1->flags;
        else
            v4 = 0;
        v3 = v4 | 0x200000;
    }
    v5 = v3;
    if ((a1->flags & 0x200000) != 0)
    {
        v6 = a1->basic->multiPartData;
        if (v6->multipartGaoObjectPartsOrMatrices)
        {
            MEM_FreeAlign(v6->multipartGaoObjectPartsOrMatrices);
            MEM_Free(a1->basic->multiPartData);
            a1->flags &= ~0x1200000u;
        }
    }
    OBJ_ChangeIdentityFlags(a1, v5, -1);
    v7 = a2 * (a3 != 0 ? sizeof(tMultiPartGameObjectLinks) : sizeof(Mtx4x4));
    a1->basic->multiPartData->elemNum = a2;
    a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(v7, 16);
    memset(a1->basic->multiPartData->multipartGaoObjectPartsOrMatrices, 0, v7);
}

void OBJ_ResolveAIRef(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tAI* v2;
    tGameObjectBasic* v3;
    tGameObjectActorObject* v4;
    tActionKit* v5;
    tAction** v6;
    int v7;
    int v8;
    tAnim* v9;
    tGameObjectEventsList** v10;
    int v11;
    tAction** actions;
    tActionElem* actionElems;

    if ((a1->flags & 0x20) != 0)
    {
        v1 = a1->extended;
        if (v1)
        {
            v2 = v1->AIInstance;
            if (v2)
            {
                AI_ResolveInstanceRef(v2);
                AI_ReinitInstance(a1->extended->AIInstance);
            }
        }
    }
    if ((a1->flags & 0x2000000) != 0)
        EVE_ResolveRefs(a1, 1);
    if ((a1->flags & 2) != 0)
    {
        v3 = a1->basic;
        if (v3)
        {
            v4 = (tGameObjectActorObject*)v3->actorObjectOrVisualObject;
            if (v3->actorObjectOrVisualObject)
            {
                v5 = v4->actionKit;
                if (v5)
                {
                    v11 = 0;
                    if (v5->elemNum)
                    {
                        actions = (tAction**)(((char*)(v5)) + sizeof(tActionKit));
                        v6 = actions;
                        do
                        {
                            if (*v6)
                            {
                                v7 = 0;
                                if ((*v6)->actionElemNum)
                                {
                                    v8 = 0;
                                    do
                                    {
                                        actionElems = (tActionElem*)(((char*)*v6) + sizeof(tAction));
                                        if (actionElems[v8].eventLists)
                                            EVE_ResolveListTrackRef(actionElems[v8].eventLists);
                                        ++v7;
                                        ++v8;
                                    } while (v7 < (*v6)->actionElemNum);
                                }
                            }
                            ++v6;
                            ++v11;
                        } while (v11 < v5->elemNum);
                    }
                }
                else
                {
                    v9 = v4->animPtr[0];
                    if (v9)
                    {
                        if (v9 != (tAnim*)-1)
                        {
                            v10 = &v9->ptr->eventListStruct;
                            if (v10)
                                EVE_ResolveListTrackRef(*v10);
                        }
                    }
                }
            }
        }
    }
}

struct sGameObjectMain* OBJ_GameObject_Duplicate(struct sWorld* a1, struct sGameObjectMain* srcGao, int a3, int a4, Vec3* pos)
{
    int v5; // eax
    tGameObjectMain* gao; // ebp
    int v7; // ecx
    int v8; // edx
    int v9; // edx
    tGameObjectExtended* v10; // eax
    Mtx4x4* v11; // esi
    Mtx4x4* v12; // edi
    Mtx4x4* v13; // esi
    Mtx4x4* v14; // edi
    Mtx4x4* v15; // esi
    Mtx4x4* v16; // edi
    Vec3* v17; // ecx
    Mtx4x4* v18; // esi
    Mtx4x4* v19; // edi
    tGameObjectExtended* v20; // eax
    int* v21; // eax
    unsigned int v22; // esi
    int v23; // eax
    tGameObjectMultiPartData* v24; // eax
    char* v25; // esi
    char* v26; // edi
    unsigned int v27; // eax
    char* v28; // edi
    char* v29; // esi
    char v30; // cl
    tGameObjectMultiPartData* v31; // eax
    char* v32; // esi
    char* v33; // edi
    unsigned int v34; // edx
    tWorld* v35; // esi
    int v36; // eax

    if (srcGao)
        v5 = srcGao->flags;
    else
        v5 = 0;
    gao = OBJ_GameObject_Create(v5 | 0x20000);
    gao->process = srcGao->process & 0xEF;
    gao->designFlags = srcGao->designFlags;
    v7 = gao->flags;
    gao->u3 = srcGao->u3;
    gao->flags2 = gao->flags2 & 0xffff00ff; //BYTE1(gao->flags2) = 0;
    v8 = gao->flags2;
    gao->if_this_not0_wont_render_gao = 0;
    v9 = srcGao->flags2 & 0xFF00 | v8;
    gao->flags2 = v9;
    gao->flags = v7 & 0xFFFBFFFF;
    if (a4)
    {
        if ((srcGao->flags2 & 0x40000000) != 0)
            gao->flags2 = v9 | 0x40000000;
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
    v10 = srcGao->extended;
    if (v10)
    {
        gao->extended->extraFlag = v10->extraFlag;
        gao->extended->sectorID = -1;
        gao->extended->sectorElemIdx = -1;
    }
    v11 = srcGao->matrices;
    v12 = gao->matrices;
    if (v12 != v11)
        memcpy(v12, v11, sizeof(Mtx4x4));
    if (pos)
        gao->matrices->Pos = *pos;
    if ((gao->flags & 0x10000) != 0)
    {
        v13 = srcGao->matrices + 1;
        v14 = gao->matrices + 1;
        if (v14 != v13)
            memcpy(v14, v13, sizeof(Mtx4x4));
        if (pos)
            gao->matrices[1].Pos = *pos;
        if ((gao->flags & 0x4000000) != 0)
        {
            v15 = srcGao->matrices;
            v16 = gao->matrices + 2;
            if (v16 != v15)
                memcpy(v16, v15, sizeof(Mtx4x4));
            if (pos)
            {
                v17 = &gao->matrices[2].Pos;
            LABEL_40:
                *v17 = *pos;
                goto LABEL_41;
            }
        }
    }
    else if ((gao->flags & 0x4000000) != 0)
    {
        v18 = srcGao->matrices;
        v19 = gao->matrices + 1;
        if (v19 != v18)
            memcpy(v19, v18, sizeof(Mtx4x4));
        if (pos)
        {
            v17 = &gao->matrices[1].Pos;
            goto LABEL_40;
        }
    }
LABEL_41:
    v20 = gao->extended;
    if (v20)
    {
        v20->capaBackup = srcGao->extended->capaBackup;
        gao->extended->AIPriority = srcGao->extended->AIPriority;
        MDF_Modifier_DuplicateList(gao, srcGao->extended->mdf);
    }
    if ((srcGao->flags & 0x4000) != 0)
    {
        memcpy(
            gao->basic->actorObjectOrVisualObject,
            srcGao->basic->actorObjectOrVisualObject,
            sizeof(tGameObjectVisualObject));
        v21 = ((tGameObjectVisualObject*)(srcGao->basic->actorObjectOrVisualObject))->RLIVertexColors;
        if (v21)
        {
            v22 = 4 * *v21 + 4;
            ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors = (int*)MEM_p_Alloc(v22);
            memcpy(
                ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->RLIVertexColors,
                ((tGameObjectVisualObject*)(srcGao->basic->actorObjectOrVisualObject))->RLIVertexColors,
                v22);
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
        memcpy(gao->basic->mtx, srcGao->basic->mtx, sizeof(tGaoMatrix));
        memcpy(gao->basic->mtx, srcGao->basic->mtx, 0x50u);
    }
    if ((gao->flags & 0x200000) != 0)
    {
        v23 = srcGao->basic->multiPartData->elemNum;
        if (v23)
        {
            gao->basic->multiPartData->elemNum = v23;
            if ((gao->flags & 0x1000000) != 0)
            {
                gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_Alloc(12 * gao->basic->multiPartData->elemNum);
                v24 = srcGao->basic->multiPartData;
                v25 = (char*)v24->multipartGaoObjectPartsOrMatrices;
                v26 = (char*)gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices;
                v27 = sizeof(tMultiPartGameObjectLinks) * v24->elemNum;
                memcpy(v26, v25, 4 * (v27 >> 2));
                v29 = &v25[4 * (v27 >> 2)];
                v28 = &v26[4 * (v27 >> 2)];
                v30 = v27;
            }
            else
            {
                gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(
                    sizeof(Mtx4x4) * gao->basic->multiPartData->elemNum,
                    16);
                v31 = srcGao->basic->multiPartData;
                v32 = (char*)v31->multipartGaoObjectPartsOrMatrices;
                v33 = (char*)gao->basic->multiPartData->multipartGaoObjectPartsOrMatrices;
                v34 = 80 * v31->elemNum;
                memcpy(v33, v32, 4 * (v34 >> 2));
                v29 = &v32[4 * (v34 >> 2)];
                v28 = &v33[4 * (v34 >> 2)];
                v30 = v34;
            }
            memcpy(v28, v29, v30 & 3);
        }
    }
    v35 = a1;
    if ((srcGao->flags & 2) != 0)
        ANI_pst_DuplicateGameObjectAnim(a1, gao, srcGao);
    if ((gao->flags & 0x20) != 0)
        AI_DuplicateInstance(srcGao, gao);
    if ((gao->flags & 0x2000000) != 0)
        EVE_Duplicate(srcGao, gao, a4);
    if ((gao->flags & 0x8000000) != 0)
        SND_Duplicate(srcGao, gao);
    if ((gao->flags & 0x10) != 0)
        gao->extended->light = srcGao->extended->light;
    if ((gao->flags & 0x600) != 0)
        COL_DuplicateInstance(gao, srcGao);
    if ((gao->flags & 0x100) != 0)
        COL_DuplicateColMap(gao, srcGao);
    v36 = gao->flags2;
    if ((v36 & 0x100000) != 0 && (v36 & 0x20) == 0)
    {
        COL_AllocDetectionList(gao);
        gao->flags2 |= 0x20u;
    }
    if ((gao->flags & 0x40) != 0)
    {
        gao->extended->fields = (tGameObjectDesignFields*)MEM_p_Alloc(0x58u);
        memcpy(gao->extended->fields, srcGao->extended->fields, sizeof(tGameObjectDesignFields));
        v35 = a1;
    }
    OBJ_DuplicateBV(srcGao, gao);
    if (a3)
        WOR_World_AttachGameObject(v35, gao);
    if (v35)
    {
        v35->tablesManagerAndEOTNeeded = 1;
        v35->visibilityCallNeeded = 1;
        v35->SnPRefreshNeeded = TRUE;
    }
    return gao;
}

void OBJ_Gizmo_UpdateGizmoPtr(struct sGameObjectMain* a1)
{
    tGameObjectMultiPartData* v1;
    int v2;
    int v3;
    tMultiPartGameObjectLinks* v4;
    tGameObjectMain* v5;
    tMultiPartGameObjectLinks* v6;
    int v7;
    tGameObjectMultiPartData* v8;

    if ((a1->flags & 0x200000) != 0 && (a1->flags & 0x1000000) != 0)
    {
        v1 = a1->basic->multiPartData;
        v2 = 0;
        if (v1->elemNum > 0)
        {
            v3 = 0;
            do
            {
                ((tMultiPartGameObjectLinks*)v1->multipartGaoObjectPartsOrMatrices)[v3].mtxPtr = NULL;
                v4 = (tMultiPartGameObjectLinks*)v1->multipartGaoObjectPartsOrMatrices;
                v5 = v4[v3].gao;
                v6 = &v4[v3];
                if (v5)
                {
                    v7 = v6->mtxIdx;
                    if (v7 == -1)
                    {
                        v6->mtxPtr = v5->matrices;
                    }
                    else if ((v5->flags & 0x200000) != 0 && (v5->flags & 0x1000000) == 0)
                    {
                        v8 = v5->basic->multiPartData;
                        if (v7 < v8->elemNum)
                            v6->mtxPtr = &(((Mtx4x4*)(v8->multipartGaoObjectPartsOrMatrices))[v7]);
                    }
                }
                ++v2;
                ++v3;
            } while (v2 < v1->elemNum);
        }
    }
}

struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t Offset)
{
    tGameObjectMain* gameObject;
    int v3;
    tGameObjectExtended* v4;
    int v5;
    tGameObjectCollMain* v6;
    unsigned int v7;
    int v8;
    char v9;
    Mtx4x4* v10;
    Mtx4x4* v11;
    tGameObjectBV* v12;
    tGameObjectBV* v13;
    tGameObjectBV* v14;
    tGameObjectBV* v15;
    int v16;
    int v17;
    tGameObjectBasic* v18;
    tGameObjectVisualObject* v19;
    char v20;
    int v21;
    int v22;
    unsigned int v23;
    int* i;
    tGaoMatrix* v25;
    int v26;
    tGameObjectActorObject* v27;
    int v28;
    int v29;
    int v30;
    int v31;
    int v32;
    tGameObjectMain** v33;
    int v34;
    int v35;
    int v36;
    BOOL v37;
    int v38;
    int v39;
    int v40;
    int v41;
    int v42;
    int v43;
    int v44;
    int v45;
    int v46;
    char* currRdrPtr;
    int Str1;
    char sstr1[4];
    int v49;
    int a1;
    Mtx4x4 mtx;
    char gameObjectName[256];

    currRdrPtr = BIG_pc_ReadFileTmp(Offset, &a1);
    LOA_ReadCharArray(&currRdrPtr, sstr1, 4u, 1);
    if (strncmp(sstr1, ".gao", 4u))
        return 0;
    LOA_ReadLong(&currRdrPtr, 0, 2);
    LOA_ReadLong(&currRdrPtr, 0, 1);
    v49 = LOA_ReadLong(&currRdrPtr, 0, 1) & ~4u;
    gameObject = OBJ_GameObject_Create(v49);
    OBJ_gpst_ObjectToLoadIn = gameObject;
    gameObject->flags2 = 0x7B9C0080;
    v3 = LOA_ReadLong(&currRdrPtr, 0, 1);
    v4 = gameObject->extended;
    v5 = v3 & gameObject->flags2;
    gameObject->flags2 = v5;
    if (v4 && (v6 = v4->coll) != 0 && v6->detectionList)
        v7 = v5 | 0x20;
    else
        v7 = v5 & ~0x20u;
    gameObject->flags2 = v7;
    if ((v3 & 0x20000000) != 0)
        gameObject->flags2 = v7 | 0x200000;
    if ((v3 & 0x40000000) != 0)
        gameObject->flags2 |= 0x400000u;
    gameObject->flags2 |= v3 & 0xFF00;
    OBJ_UpdateStatusFlagRTL(gameObject);
    v8 = gameObject->flags2;
    if ((v8 & 0x20) == 0 && (v8 & 0x100000) != 0)
    {
        gameObject->flags2 = v8 | 0x20;
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
    v9 = LOA_ReadChar(&currRdrPtr, 0, 1);
    gameObject->process = v9 & 0xC | ((v9 & 0xC) >> 2) & 1 | ((v9 & 0xC | ((v9 & 0xC) >> 2) & 1) >> 2) & 2;
    gameObject->if_this_not0_wont_render_gao = 0;
    LOA_ReadMatrix(&currRdrPtr, &mtx, 1);
    MATH_SetCorrectType(&mtx);
    if ((v49 & 0x10000) != 0)
    {
        v10 = gameObject->matrices + 1;
        if (v10 != &mtx)
            memcpy(v10, &mtx, sizeof(Mtx4x4));
    }
    v11 = gameObject->matrices;
    if (v11 != &mtx)
        memcpy(v11, &mtx, sizeof(Mtx4x4));
    if (gameObject->boundingVolume)
    {
        LOA_ReadLong(&currRdrPtr, 0, 2);
        if ((v49 & 0x80000) != 0)
        {
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMinOriginal, 1);
            v12 = gameObject->boundingVolume;
            v12->BVMin.x = v12->BVMinOriginal.x;
            v12->BVMin.y = v12->BVMinOriginal.y;
            v12->BVMin.z = v12->BVMinOriginal.z;
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMaxOriginal, 1);
            v13 = gameObject->boundingVolume;
            v13->BVMax.x = v13->BVMaxOriginal.x;
            v13->BVMax.y = v13->BVMaxOriginal.y;
            v13->BVMax.z = v13->BVMaxOriginal.z;
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMinLocal, 1);
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMaxLocal, 1);
        }
        else
        {
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMinOriginal, 1);
            v14 = gameObject->boundingVolume;
            v14->BVMin.x = v14->BVMinOriginal.x;
            v14->BVMin.y = v14->BVMinOriginal.y;
            v14->BVMin.z = v14->BVMinOriginal.z;
            LOA_ReadVector(&currRdrPtr, &gameObject->boundingVolume->BVMaxOriginal, 1);
            v15 = gameObject->boundingVolume;
            v15->BVMax.x = v15->BVMaxOriginal.x;
            v15->BVMax.y = v15->BVMaxOriginal.y;
            v15->BVMax.z = v15->BVMaxOriginal.z;
        }
    }
    if ((v49 & 0x1000) != 0)
    {
        if ((v49 & 0x4000) != 0)
        {
            v16 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v16;
            if (v16 == -1)
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = 0;
            else
                LOA_MakeFileRef(
                    v16, &(((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse), GEO_ul_Load_ObjectCallback, 1);
            v17 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v17;
            if (v17 == -1)
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROMAT = 0;
            else
                LOA_MakeFileRef(v17, &(((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->GROMAT), GEO_ul_Load_ObjectCallback, 1);
            ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->objDrawMask = LOA_ReadLong(&currRdrPtr, 0, 1);
            ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum = (byte)LOA_ReadLong(&currRdrPtr, 0, 1);// csak az elsõ bájtot veszi figyelembe
            v18 = gameObject->basic;
            v19 = v18->actorObjectOrVisualObject;
            v20 = ((tGameObjectVisualObject*)(v18->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum;
            if (v20 == -1)
            {
                v19->boneDisplayOrder_zListBufferNum = 0;
            }
            else if (v20 > 10)
            {
                v19->boneDisplayOrder_zListBufferNum = v20 - 16;
            }
            v21 = LOA_ReadLong(&currRdrPtr, 0, 1);
            v22 = v21;
            if (v21)
            {
                v23 = 4 * v21 + 4;
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors = (int*)MEM_p_Alloc(v23);
                LOA_ul_FileTypeSize_27 += v23;
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors[0] = v22; // num
                for (i = ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors + 1; v22; --v22)
                    *i++ = LOA_ReadLong(&currRdrPtr, 0, 1);
            }
            else
            {
                ((tGameObjectVisualObject*)(gameObject->basic->actorObjectOrVisualObject))->RLIVertexColors = 0;
            }
        }
        if ((v49 & 0x400000) != 0)
        {
            v25 = gameObject->basic->mtx;
            v26 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v26;
            if (v26 == -1)
            {
                v25->refGao2 = NULL;
                v25->refGao = NULL;
            }
            else
            {
                LOA_MakeFileRef(v26, &v25->refGao, OBJ_ul_GameObjectCallback, 1);
                LOA_MakeFileRef(Str1, &v25->refGao2, OBJ_ul_GameObjectCallback, 1);
            }
            LOA_ReadMatrix(&currRdrPtr, &v25->mtx, 1);
        }
        if ((v49 & 2) != 0)
        {
            v27 = (tGameObjectActorObject*)gameObject->basic->actorObjectOrVisualObject;
            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            v27->animPtr[0] = (tAnim*)Str1;
            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            v27->shape = (tShape*)Str1;
            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
            v27->group = (tGroup*)Str1;
            v28 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v28;
            if (v28 != -1)
            {
                if (v28)
                    LOA_MakeFileRef(v28, &(((tGameObjectActorObject*)(gameObject->basic->actorObjectOrVisualObject))->actionKit), ACT_ul_ActionKitCallback, 1);
            }
        }
        if ((v49 & 0x200000) != 0)
        {
            v29 = LOA_ReadLong(&currRdrPtr, 0, 1);
            v30 = v29;
            gameObject->basic->multiPartData->elemNum = v29;
            if (v29)
            {
                if ((v49 & 0x1000000) != 0)
                {
                    v31 = sizeof(tMultiPartGameObjectLinks) * v29;
                    gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_Alloc(sizeof(tMultiPartGameObjectLinks) * v29);
                    LOA_ul_FileTypeSize_34 += v31;
                    if (v30 > 0)
                    {
                        v32 = 0;
                        do
                        {
                            ((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v32].mtxPtr = NULL;
                            Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
                            ((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v32].gao = (tGameObjectMain*)Str1; // ez csak a key egyelore
                            v33 = &(((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v32].gao);
                            if (*v33 == (tGameObjectMain*)-1)
                                *v33 = 0;
                            else
                                LOA_MakeFileRef(Str1, v33, OBJ_ul_GameObjectCallback, 1);
                            ((tMultiPartGameObjectLinks*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v32].mtxIdx = LOA_ReadLong(&currRdrPtr, 0, 1);
                            ++v32;
                            --v30;
                        } while (v30);
                    }
                }
                else
                {
                    v34 = sizeof(Mtx4x4) * v29;
                    gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices = MEM_p_AllocAligned(sizeof(Mtx4x4) * v29, 16);
                    LOA_ul_FileTypeSize_34 += v34;
                    if (v30 > 0)
                    {
                        v35 = 0;
                        do
                        {
                            LOA_ReadMatrix(&currRdrPtr, &(((Mtx4x4*)(gameObject->basic->multiPartData->multipartGaoObjectPartsOrMatrices))[v35]), 1);
                            LOA_ReadString(&currRdrPtr, 0, 64, 2);
                            ++v35;
                            --v30;
                        } while (v30);
                    }
                }
            }
        }
    }
    if ((v49 & 0x2000) != 0)
    {
        v36 = LOA_ReadLong(&currRdrPtr, 0, 1);
        Str1 = v36;
        if ((v49 & 0x800000) != 0 && v36)
            LOA_MakeFileRef(v36, &gameObject->extended->grp, GRP_ul_LoadGrpCallback, 1);
        Str1 = LOA_ReadLong(&currRdrPtr, 0, 1);
        v37 = Str1 != 0;
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
        if ((v49 & 0x20) != 0)
        {
            v38 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v38;
            if (v38)
            {
                if (v38 != -1)
                    LOA_MakeFileRef(v38, &(gameObject->extended->AIInstance), AI_ul_CallbackLoadInstance, 1);
            }
        }
        if ((v49 & 0x2000000) != 0)
        {
            v39 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v39;
            if (v39)
            {
                if (v39 != -1)
                {
                    gameObject->extended->eve = (tEventData*)MEM_p_Alloc(sizeof(tEventData));
                    LOA_ul_FileTypeSize_37 += sizeof(tEventData);
                    LOA_MakeFileRef(Str1, &gameObject->extended->eve->eventListStruct, EVE_ul_CallbackLoadListTracks, 1);
                    gameObject->extended->eve->tracks = 0;
                }
            }
        }
        if ((v49 & 0x8000000) != 0)
        {
            v40 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v40;
            if (v40)
            {
                if (v40 != -1)
                    LOA_MakeFileRef(v40, &gameObject->extended->snd, SND_ul_CallbackLoad, 1);
            }
        }
        if ((v49 & 0x40000) != 0)
        {
            v41 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v41;
            if (v41)
            {
                if (v41 != -1)
                    LOA_MakeFileRef(v41, &gameObject->extended->way, WAY_ul_AllLinkListsCallback, 1);
            }
        }
        if ((v49 & 0x10) != 0)
        {
            v42 = LOA_ReadLong(&currRdrPtr, 0, 1);
            Str1 = v42;
            if (v42)
            {
                if (v42 != -1)
                    LOA_MakeFileRef(v42, &gameObject->extended->light, GEO_ul_Load_ObjectCallback, 1);
            }
        }
        if ((v49 & 0x40) != 0)
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
        if (v37)
        {
            v43 = MDF_ul_Modifier_Load(&gameObject->extended->mdf, currRdrPtr, 1, gameObject);
            currRdrPtr += v43;
            LOA_ReadLong(&currRdrPtr, 0, 1);
        }
    }
    if ((v49 & 0x600) != 0)
    {
        v44 = LOA_ReadLong(&currRdrPtr, 0, 1);
        Str1 = v44;
        if (v44)
        {
            if (v44 != -1)
                LOA_MakeFileRef(v44, &gameObject->extended->coll->colInstance, COL_ul_CallBackLoadInstance, 1);
        }
    }
    if ((v49 & 0x100) != 0)
    {
        v45 = LOA_ReadLong(&currRdrPtr, 0, 1);
        Str1 = v45;
        if (v45)
        {
            if (v45 != -1)
            {
                LOA_MakeFileRef(v45, &gameObject->extended->coll->colMap, COL_ul_CallBackLoadColMap, 1);
                gameObject->flags2 |= 0x80u;
            }
        }
    }
    v46 = LOA_ReadLong(&currRdrPtr, 0, 1);
    a1 = v46;
    if (v46) {
        LOA_ReadString(&currRdrPtr, gameObjectName, v46, 1); //LOA_ReadString(&currRdrPtr, 0, v46, 1);
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
    tableDescStruct* v1;
    char* v2;
    tableElemStructPF* v3;
    int v4;
    void** v6;
    char* a1;
    int size;

    v1 = (tableDescStruct*)MEM_p_Alloc(sizeof(tableDescStruct));
    memset(v1, 0, sizeof(tableDescStruct));
    v2 = BIG_pc_ReadFileTmp(Offset, &size);
    a1 = v2;

    TAB_PFtable_Init(v1, size, 0.1f);
    if (v2 < &v2[size])
    {
        do
        {
            ((tableElemStructPF*)(v1->nextElemPtr))->objectPtr = 0;
            ((tableElemStructPF*)(v1->nextElemPtr))->objectFlags = 0;
            v3 = v1->nextElemPtr;
            ++v1->actElemNum;
            ++v3;
            v6 = &v3[-1].objectPtr;
            v1->nextElemPtr = v3;
            v4 = LOA_ReadLong(&a1, 0, 1);
            LOA_MakeFileRef(v4, v6, OBJ_ul_GameObjectCallback, 1);
            LOA_ReadLong(&a1, 0, 2);
        } while (a1 < &v2[size]);
    }
    return v1;
}

void OBJ_ScaleSet(struct sGameObjectMain* a1, Vec3* a2)
{
    tGaoMatrix* v2;
    Vec3 v3;

    if ((a1->flags & 0x400000) != 0)
    {
        if ((a1->flags & 1) != 0)
        {
            OBJ_ComputeRecursivScale(a1, &v3);
            a2->x = a2->x / v3.x;
            a2->y = a2->y / v3.y;
            a2->z = a2->z / v3.z;
        }
        v2 = OBJ_pst_GetLocalMatrix(a1);
        MATH_SetScale(&v2->mtx, a2);
        OBJ_ComputeGlobalWhenHie(a1);
        OBJ_ComputeBV(a1, 0);
    }
    else
    {
        MATH_SetScale(a1->matrices, a2);
        OBJ_ComputeBV(a1, 0);
    }
}

void OBJ_ZoomSet(struct sGameObjectMain* a1, float zoom)
{
    Mtx4x4* v2;
    int v3;
    BOOL v4;
    Mtx4x4* v5;
    Mtx4x4* v6;

    v2 = OBJ_pst_GetAbsoluteMatrix(a1);
    MATH_SetZoom(v2, zoom);
    v3 = a1->flags;
    if ((a1->flags & 2) != 0 && (v3 & 0x4000000) != 0)
    {
        v4 = (v3 & 0x10000) == 0;
        v5 = a1->matrices;
        if (v4)
            v6 = v5 + 1;
        else
            v6 = v5 + 2;
        MATH_SetZoom(v6, zoom);
    }
    OBJ_ComputeBV(a1, 0);
}

struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGaoMatrix* v2;
    tGameObjectMain* result;

    if (a1 && (v1 = a1->basic) != 0 && (v2 = v1->mtx) != 0)
        result = v2->refGao2;
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
    byte v20; 
    tActionElem* v21;
    __int16 v22;
    byte v23;
    double v24;
    byte v25;
    byte v26;
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
                                        v23 = (byte)(v16->unkb + (uint64_t)(TIM_gf_dt * 60.000004 + 0.5));
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

void OBJ_FreezeModifier_C(struct sGameObjectMain* a1, int a2, int setOrReset)
{
    unsigned int v3;
    tModifier* v4;

    v4 = MDF_pst_GetByType(a1, a2);
    if (v4)
    {
        if (setOrReset)
            v3 = v4->modifierFlags | 0x2000000;
        else
            v3 = v4->modifierFlags & ~0x2000000u;
        v4->modifierFlags = v3;
    }
}

BOOL OBJ_ModifierType_C(struct sGameObjectMain* a1, int mdfId)
{
    return MDF_pst_GetByType(a1, mdfId) != 0;
}

struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* a1)
{
    tGroGeneric* result;
    int v2;

    if (!a1
        || (a1->flags & 0x4000) == 0
        || (result = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) == 0
        || (v2 = result->intf->id, v2 != 1)
        && (v2 != 8
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

int OBJ_CullingAABBox(Vec3** a1, byte* a2)

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
