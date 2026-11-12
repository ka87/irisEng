#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "snapmodifier.h"

void GEO_ModifierSnap_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tSnapMdfStruct* snap;
    tSnapMdfStruct* src = (tSnapMdfStruct*)initData;

    snap = (tSnapMdfStruct*)MEM_p_Alloc(sizeof(tSnapMdfStruct));
    mdf->mdfPrivateDataPtr = snap;
    if (src)
    {
        *snap = *src;
        if (snap->num)
            snap->ptr = (tSnapMdfInnerStruct*)MEM_p_Alloc(sizeof(tSnapMdfInnerStruct) * snap->num);
        else
            snap->ptr = 0;
    }
    else
    {
        snap->gao = 0;
        snap->num = 0;
        snap->ptr = 0;
    }
}

void GEO_ModifierSnap_Destroy(struct sModifier* mdf)
{
    tSnapMdfStruct* snap;

    snap = (tSnapMdfStruct*)mdf->mdfPrivateDataPtr;
    if (snap->num)
        MEM_Free(snap->ptr);
    MEM_Free(snap);
}

void GEO_ModifierSnap_Apply(struct sModifier* mdf, struct sGEOGRO* geo)
{
    tSnapMdfStruct* snap;
    tSnapMdfInnerStruct* link;
    tGEOGRO* snapGeo;
    Mtx4x4* snapMtx;
    Mtx4x4 targetInv;
    Vec3* dst;
    Vec3 snapped;
    int i;

    snap = (tSnapMdfStruct*)mdf->mdfPrivateDataPtr;
    MATH_InvertMatrix(&targetInv, OBJ_pst_GetAbsoluteMatrix(gpst_DX_stDD->currTargetGao));
    snapMtx = OBJ_pst_GetAbsoluteMatrix(snap->gao);
    snapGeo = (tGEOGRO*)OBJ_p_GetGro(snap->gao);
    if ((gpst_DX_stDD->flags & 0x400) != 0)
        dst = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
    else
        dst = geo->vertices;
    for (i = 0; i < snap->num; i++)
    {
        link = &snap->ptr[i];
        link->v = geo->vertices[link->vec1Idx];
        MATH_TransformVertex(&snapped, snapMtx, &snapGeo->vertices[link->vec2Idx]);
        MATH_TransformVertex(&dst[link->vec1Idx], &targetInv, &snapped);
    }
}

void GEO_ModifierSnap_Unapply(struct sModifier* mdf, struct sGEOGRO* geo)
{
    tSnapMdfStruct* snap;
    tSnapMdfInnerStruct* link;
    int i;

    snap = (tSnapMdfStruct*)mdf->mdfPrivateDataPtr;
    for (i = 0; i < snap->num; i++)
    {
        link = &snap->ptr[i];
        geo->vertices[link->vec1Idx] = link->v;
    }
}

int GEO_ul_ModifierSnap_Load(struct sModifier* mdf, char* data)
{
    char* start;
    tSnapMdfStruct* snap;
    int key;
    int count;
    int i;

    start = data;
    snap = (tSnapMdfStruct*)mdf->mdfPrivateDataPtr;
    LOA_ReadLong(&data, 0, 2);
    key = LOA_ReadLong(&data, 0, 1);
    if (key != -1)
    {
        if (BIG_ul_SearchKeyToPos(key) == -1)
            snap->gao = 0;
        else
            LOA_MakeFileRef(key, &snap->gao, OBJ_ul_GameObjectCallback, 1);
    }
    count = LOA_ReadLong(&data, 0, 1);
    snap->num = count;
    if (count)
        snap->ptr = (tSnapMdfInnerStruct*)MEM_p_Alloc(sizeof(tSnapMdfInnerStruct) * count);
    else
        snap->ptr = 0;
    for (i = 0; i < snap->num; i++)
    {
        snap->ptr[i].vec1Idx = LOA_ReadLong(&data, 0, 1);
        snap->ptr[i].vec2Idx = LOA_ReadLong(&data, 0, 1);
    }
    return data - start;
}
