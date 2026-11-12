#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "snapmodifier.h"

void GEO_ModifierSnap_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a)
{
    tSnapMdfStruct* v3;
    tSnapMdfStruct* v4;

    int v5;
    tSnapMdfStruct* a3 = (tSnapMdfStruct*)a3a;


    v3 = (tSnapMdfStruct*)MEM_p_Alloc(sizeof(tSnapMdfStruct));
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        *v3 = *a3;
        v4 = (tSnapMdfStruct*)a2->mdfPrivateDataPtr;
        v5 = v4->num;
        if (v5)
            v4->ptr = (tSnapMdfInnerStruct*)MEM_p_Alloc(sizeof(tSnapMdfInnerStruct) * v5);
        else
            v4->ptr = 0;
    }
    else
    {
        v3->gao = 0;
        v3->num = 0;
        v3->ptr = 0;
    }
}

void GEO_ModifierSnap_Destroy(struct sModifier* a1)
{
    tSnapMdfStruct* v1;

    v1 = (tSnapMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1->num)
        MEM_Free(v1->ptr);
    MEM_Free(v1);
}

void GEO_ModifierSnap_Apply(struct sModifier* a1, struct sGEOGRO* a2)
{
    tSnapMdfStruct* v2;
    Mtx4x4* v3;
    tGEOGRO* v4;
    tSnapMdfInnerStruct* v5;
    tSnapMdfInnerStruct* v6;
    tSnapMdfInnerStruct* v7;
    Vec3* v8;
    Vec3* v9;
    tGEOGRO* v10;
    Mtx4x4* a2a;
    Vec3 a3;
    Mtx4x4 a1a;

    v2 = (tSnapMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = OBJ_pst_GetAbsoluteMatrix(gpst_DX_stDD->currTargetGao);
    MATH_InvertMatrix(&a1a, v3);
    a2a = OBJ_pst_GetAbsoluteMatrix(v2->gao);
    v4 = (tGEOGRO*)OBJ_p_GetGro(v2->gao);
    v5 = v2->ptr;
    v10 = v4;
    v6 = &v5[v2->num];
    if ((gpst_DX_stDD->flags & 0x400) != 0)
        v9 = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
    else
        v9 = a2->vertices;
    if (v5 < v6)
    {
        v7 = v5;
        do
        {
            v8 = &a2->vertices[v5->vec1Idx];
            v7->v = *v8;
            MATH_TransformVertex(&a3, a2a, &v10->vertices[v7->vec2Idx]);
            MATH_TransformVertex(&v9[v5->vec1Idx], &a1a, &a3);
            ++v5;
            ++v7;
        } while (v5 < v6);
    }
}

void GEO_ModifierSnap_Unapply(struct sModifier* a1, struct sGEOGRO* a2)
{
    tSnapMdfStruct* v2;
    tSnapMdfInnerStruct* v3;
    tSnapMdfInnerStruct* v4;
    Vec3* v5;
    tSnapMdfInnerStruct* v6;
    Vec3* v7;
    float v8;

    v2 = (tSnapMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = v2->ptr;
    v4 = &v3[v2->num];
    v5 = a2->vertices;
    if (v3 < v4)
    {
        v6 = v3;
        do
        {
            v7 = &v5[v3->vec1Idx];
            v7->x = v6->v.x;
            v7->y = v6->v.y;
            v8 = v6->v.z;
            ++v3;
            ++v6;
            v7->z = v8;
        } while (v3 < v4);
    }
}

int GEO_ul_ModifierSnap_Load(struct sModifier* a1, char* a2)
{
    char* v2;
    tSnapMdfStruct* v3;
    int v4;
    int v5;
    int v6;
    tSnapMdfInnerStruct* v7;
    tSnapMdfInnerStruct* i;
    char* v10;

    v2 = a2;
    v3 = (tSnapMdfStruct*)a1->mdfPrivateDataPtr;
    v10 = a2;
    LOA_ReadLong(&v10, 0, 2);
    v4 = LOA_ReadLong(&v10, 0, 1);
    v5 = v4;
    if (v4 != -1)
    {
        if (BIG_ul_SearchKeyToPos(v4) == -1)
            v3->gao = 0;
        else
            LOA_MakeFileRef(v5, &v3->gao, OBJ_ul_GameObjectCallback, 1);
    }
    v6 = LOA_ReadLong(&v10, 0, 1);
    v3->num = v6;
    if (v6)
        v3->ptr = (tSnapMdfInnerStruct*)MEM_p_Alloc(sizeof(tSnapMdfInnerStruct) * v6);
    else
        v3->ptr = 0;
    v7 = v3->ptr;
    for (i = &v7[v3->num]; v7 < i; ++v7)
    {
        v7->vec1Idx = LOA_ReadLong(&v10, 0, 1);
        v7->vec2Idx = LOA_ReadLong(&v10, 0, 1);
    }
    return v10 - v2;
}