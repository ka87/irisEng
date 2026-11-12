#include "../memory.h"
#include "../loading.h"
#include "speciallookatmodifier.h"

void GAO_ModifierSpecialLookAt_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tSpecialLookAtMdfStruct* sla;

    sla = (tSpecialLookAtMdfStruct*)MEM_p_Alloc(sizeof(tSpecialLookAtMdfStruct));
    mdf->modifierFlags |= 0x1000000u;
    mdf->mdfPrivateDataPtr = sla;
    if (initData)
    {
        memcpy(sla, initData, sizeof(tSpecialLookAtMdfStruct));
    }
    else
    {
        sla->b1 = 2;
        sla->b2 = 0;
        sla->b3 = 0;
        sla->b4 = 0;
        sla->f1 = 5.0f;
        sla->f2 = 5.0f;
        sla->f3 = 5.0f;
        sla->rank = -1;
        sla->gao = NULL;
    }
}

void GAO_ModifierSpecialLookAt_Destroy(tModifier* mdf)
{
    MEM_Free(mdf->mdfPrivateDataPtr);
}

void GAO_ModifierSpecialLookAt_Apply(struct sModifier* mdf, void* unk1)
{
    tSpecialLookAtMdfStruct* sla;
    tGameObjectMain* gao;
    tGameObjectMain* target;
    Mtx4x4* mtx;
    Mtx4x4* camMtx;
    Mtx4x4* targetMtx;
    Vec3* pos;
    Vec3* bvMin;
    tGEOGRO* geo;
    Vec3 delta;
    Vec3 screen;
    Vec3 depth;
    double angle;
    double scale;
    float dist;
    float height;
    float radius;
    float step;
    int i;

    sla = (tSpecialLookAtMdfStruct*)mdf->mdfPrivateDataPtr;
    gao = gpst_DX_stDD->currTargetGao;
    mtx = OBJ_pst_GetAbsoluteMatrix(gao);
    camMtx = &gpst_DX_stDD->ct.cam.objectMtx;
    pos = &gao->matrices->Pos;

    if (sla->rank != -1)
        sla->gao = OBJ_pst_GroupGetByRank(MDF_Modifier_GetCurGroup(mdf), sla->rank);

    if (!sla->b1)
    {
        MATH_AddScaleVector(pos, &camMtx->Pos, &camMtx->v3, sla->f1);
        mtx->v1 = camMtx->v1;
        MATH_VecInvert(&mtx->v2, &camMtx->v3);
        MATH_VecInvert(&mtx->v3, &camMtx->v2);

        depth.x = 0.0;
        depth.y = 0.0;
        depth.z = sla->f1;
        CAM_2Dto3DCamera2(&gpst_DX_stDD->ct.cam, &screen, &depth);
        bvMin = &gao->boundingVolume->BVMin;
        mtx->mtxflag |= 8u;
        mtx->_m24 = 1.0;
        mtx->_m14 = (float)(screen.x / bvMin->x);
        mtx->_m34 = -(screen.y / bvMin->z);
        return;
    }

    if (sla->b1 == 1)
    {
        target = sla->gao;
        if (!target)
            return;
        targetMtx = target->matrices;
        delta.x = camMtx->Pos.x - targetMtx->Pos.x;
        delta.y = camMtx->Pos.y - targetMtx->Pos.y;
        delta.z = camMtx->Pos.z - targetMtx->Pos.z;
        MATH_AddScaleVector(pos, &targetMtx->Pos, &delta,
                            sla->f1 * sla->f1 / MATH_VecLenSquared(&delta));

        dist = MATH_DistanceOfTwoPoints(pos, &targetMtx->Pos);
        height = MATH_GetSqrt((float)(sla->f1 * sla->f1 - dist * dist));

        MATH_NormalizeVector(&mtx->v2, &delta);
        MATH_VecInvert(&mtx->v2, &mtx->v2);
        MATH_CrossProduct(&mtx->v3, &camMtx->v1, &mtx->v2);
        MATH_NormalizeEqualVector(&mtx->v3);
        MATH_CrossProduct(&mtx->v1, &mtx->v2, &mtx->v3);

        geo = OBJ_p_GetCurrentGeo(gao);
        if (geo)
        {
            radius = (float)((height + sla->f2) / height * 100.0);
            step = (float)(12.566371 / (double)geo->vertexNum);
            angle = 0.0;
            for (i = 0; i < geo->vertexNum; i += 2)
            {
                geo->vertices[i].x = (float)(sin(angle) * radius);
                geo->vertices[i].y = -sla->f3;
                geo->vertices[i].z = (float)(cos(angle) * radius);
                angle = angle + step;
            }
        }

        scale = height * 0.0099999998;
        mtx->mtxflag |= 8u;
        mtx->_m34 = (float)scale;
        mtx->_m24 = 1.0;
        mtx->_m14 = (float)scale;
    }
    else if (sla->b1 == 2)
    {
        target = sla->gao;
        if (!target)
            return;
        targetMtx = target->matrices;
        delta.x = mtx->Pos.x - targetMtx->Pos.x;
        delta.y = mtx->Pos.y - targetMtx->Pos.y;
        delta.z = mtx->Pos.z - targetMtx->Pos.z;
        MATH_NormalizeVector(&mtx->v2, &delta);
        MATH_CrossProduct(&mtx->v3, &camMtx->v1, &mtx->v2);
        MATH_NormalizeEqualVector(&mtx->v3);
        MATH_CrossProduct(&mtx->v1, &mtx->v2, &mtx->v3);
        OBJ_ComputeLocalWhenHie(gao);
    }
}

int GAO_ModifierSpecialLookAt_Load(struct sModifier* mdf, char* data)
{
    char* start;
    tSpecialLookAtMdfStruct* sla;
    int version;
    int key;

    start = data;
    sla = (tSpecialLookAtMdfStruct*)mdf->mdfPrivateDataPtr;
    mdf->modifierFlags |= 0x1000000u;
    version = LOA_ReadLong(&data, 0, 1);
    sla->b1 = LOA_ReadChar(&data, 0, 1);
    sla->b2 = LOA_ReadChar(&data, 0, 1);
    sla->b3 = LOA_ReadChar(&data, 0, 1);
    sla->b4 = LOA_ReadChar(&data, 0, 1);
    sla->f1 = LOA_ReadFloat(&data, 0, 1);
    if (version == 28)
        sla->rank = LOA_ReadLong(&data, 0, 1);
    else
        sla->rank = -1;
    key = LOA_ReadLong(&data, 0, 1);
    if (sla->rank == -1 && key && key != -1)
        LOA_MakeFileRef(key, &sla->gao, OBJ_ul_GameObjectCallback, 1);
    else
        sla->gao = 0;
    if (version == 4)
    {
        sla->f2 = 1000.0f;
        sla->f3 = 1000.0f;
    }
    else
    {
        sla->f2 = LOA_ReadFloat(&data, 0, 1);
        sla->f3 = LOA_ReadFloat(&data, 0, 1);
    }
    return data - start;
}

void GAO_ModifierSpecialLookAt_Unapply(struct sModifier* mdf, void* unk1)
{
}

void GAO_ModifierSpecialLookAt_Reinit(struct sModifier* mdf)
{
}
