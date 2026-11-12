#include "../math.h"
#include "../memory.h"
#include "../loading.h"
#include "leglinkmodifier.h"

// csomo inline, megnezni crossproduct, normalizevector
void __cdecl GAO_ModifierLegLink_Compute(Mtx4x4* a1, Mtx4x4* a2, Mtx4x4* a3, float a4, float a5)
{
    double v6; // st4
    double v7; // st3
    double v8; // st4
    double v9; // st3
    long double v10; // st4
    double v11; // rt1
    long double v12; // st6
    double v13; // st7
    double v14; // st6
    double v15; // st5
    long double v16; // st5
    double v17; // st7
    double v18; // st6
    double v19; // st5
    double v20; // st4
    double v21; // st6
    long double v22; // st5
    long double v23; // st7
    double v24; // rt1
    long double v25; // st6
    long double v26; // st6
    double v27; // st5
    long double v28; // st4
    long double v29; // st6
    double v30; // st7
    double v31; // st6
    double v32; // st5
    long double v33; // st4
    double v34; // st7
    long double v35; // st6
    long double v36; // st7
    float v37; // [esp+Ch] [ebp-18h]
    float v38; // [esp+Ch] [ebp-18h]
    float v39; // [esp+Ch] [ebp-18h]
    float v40; // [esp+10h] [ebp-14h]
    float v41; // [esp+10h] [ebp-14h]
    float v42; // [esp+10h] [ebp-14h]
    float v43; // [esp+10h] [ebp-14h]
    float v44; // [esp+14h] [ebp-10h]
    float v45; // [esp+18h] [ebp-Ch]
    float v46; // [esp+18h] [ebp-Ch]
    float v47; // [esp+18h] [ebp-Ch]
    float v48; // [esp+1Ch] [ebp-8h]
    float v49; // [esp+1Ch] [ebp-8h]
    float v50; // [esp+20h] [ebp-4h]
    float v51; // [esp+20h] [ebp-4h]
    float a1a; // [esp+28h] [ebp+4h]
    float a4a; // [esp+34h] [ebp+10h]
    float a4b; // [esp+34h] [ebp+10h]
    float a4c; // [esp+34h] [ebp+10h]
    float a4d; // [esp+34h] [ebp+10h]
    float a4e; // [esp+34h] [ebp+10h]
    float a5a; // [esp+38h] [ebp+14h]
    float a5b; // [esp+38h] [ebp+14h]
    float a5c; // [esp+38h] [ebp+14h]

    v6 = a1->Pos.y - a3->Pos.y;
    v7 = a1->Pos.z - a3->Pos.z;
    v8 = v6 * v6 + v7 * v7;
    v9 = a1->Pos.x - a3->Pos.x;
    v10 = sqrt(v8 + v9 * v9);
    v11 = a4 * a4;
    v12 = (v11 - a5 * a5 + v10 * v10) / (v10 + v10);
    a4a = (float)v12;
    a5a = (float)(v11 - v12 * a4a);
    v13 = a3->Pos.x - a1->Pos.x;
    v14 = a3->Pos.y - a1->Pos.y;
    v15 = a3->Pos.z - a1->Pos.z;
    v50 = (float)v15;

    *((uint32_t*)&a1a) = MATH_gl_TableRacine[((*((int32_t*)(&a5a))) >> 13) & 0x7FF] + (((( (*((uint32_t*)(&a5a))) & 0x7F800000) + 0x3F800000) >> 1) & 0x7F800000);

    v16 = 1.0 / sqrt(v15 * v50 + v14 * v14 + v13 * v13);
    a5b = (float)v16;
    v45 = (float)(v16 * v13);
    v46 = v45 * a4a;
    v48 = (float)(v14 * a5b * a4a);
    v17 = v50 * a5b * a4a;
    v18 = v17 * a1->v3.y - v48 * a1->v3.z;
    v19 = v46 * a1->v3.z - v17 * a1->v3.x;
    v20 = v48 * a1->v3.x - v46 * a1->v3.y;
    v37 = (float)(v48 * v20 - v19 * v17);
    v40 = (float)(v17 * v18 - v20 * v46);
    v21 = v19 * v46 - v48 * v18;
    v22 = 1.0 / sqrt(v37 * v37 + v21 * v21 + v40 * v40);
    v38 = (float)(v37 * v22);
    v41 = (float)(v40 * v22);
    v39 = v38 * a1a;
    v42 = v41 * a1a;
    a2->Pos.x = v46 + a1->Pos.x;
    a2->Pos.y = v48 + a1->Pos.y;
    a2->Pos.z = (float)(v17 + a1->Pos.z);
    a4b = v39 + a2->Pos.x;
    a2->Pos.x = a4b;
    a5c = v42 + a2->Pos.y;
    a2->Pos.y = a5c;
    v23 = v21 * v22 * a1a + a2->Pos.z;
    a2->Pos.z = (float)v23;
    v43 = a5c - a1->Pos.y;
    v24 = a4b - a1->Pos.x;
    v25 = v23 - a1->Pos.z;
    v44 = (float)v25;
    v26 = v25 * a1->v2.y - v43 * a1->v2.z;
    v27 = v24 * a1->v2.z - v44 * a1->v2.x;
    v51 = (float)(v43 * a1->v2.x - v24 * a1->v2.y);
    v28 = 1.0 / sqrt(v26 * v26 + v51 * v51 + v27 * v27);
    a4c = (float)v28;
    a1->v1.x = (float)(v28 * v26);
    a1->v1.y = (float)(v27 * a4c);
    a1->v1.z = v51 * a4c;
    v29 = 1.0 / sqrt(v24 * v24 + v44 * v44 + v43 * v43);
    a4d = (float)v29;
    a1->v3.x = (float)(v29 * v24);
    a1->v3.y = v43 * a4d;
    a1->v3.z = v44 * a4d;
    a1->v2.x = a1->v1.z * a1->v3.y - a1->v1.y * a1->v3.z;
    a1->v2.y = a1->v1.x * a1->v3.z - a1->v1.z * a1->v3.x;
    a1->v2.z = a1->v1.y * a1->v3.x - a1->v1.x * a1->v3.y;
    v30 = a3->Pos.x - a2->Pos.x;
    v31 = a3->Pos.y - a2->Pos.y;
    v32 = a3->Pos.z - a2->Pos.z;
    v33 = (float)(1.0 / sqrt(v30 * v30 + v32 * v32 + v31 * v31));
    a2->v3.x = (float)(v33 * v30);
    a2->v3.y = (float)(v31 * v33);
    a2->v3.z = (float)(v32 * v33);
    v47 = (float)(v32 * a2->v2.y - v31 * a2->v2.z);
    v49 = (float)(v30 * a2->v2.z - v32 * a2->v2.x);
    v34 = v31 * a2->v2.x - v30 * a2->v2.y;
    v35 = 1.0 / sqrt(v47 * v47 + v34 * v34 + v49 * v49);
    a2->v1.x = (float)(v47 * v35);
    a4e = (float)(v49 * v35);
    a2->v1.y = a4e;
    v36 = v34 * v35;
    a2->v1.z = (float)v36;
    a2->v2.x = (float)(v36 * a2->v3.y - a4e * a2->v3.z);
    a2->v2.y = a2->v1.x * a2->v3.z - a2->v1.z * a2->v3.x;
    a2->v2.z = a2->v1.y * a2->v3.x - a2->v1.x * a2->v3.y;
}

void GAO_ModifierLegLink_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tLegLinkMdfStruct* v3;

    v3 = (tLegLinkMdfStruct*)MEM_p_Alloc(sizeof(tLegLinkMdfStruct));
    a2->modifierFlags |= 0x1000000u;
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        memcpy(v3, a3, sizeof(tLegLinkMdfStruct));
    }
    else
    {
        v3->gao0 = 0;
        ((tLegLinkMdfStruct*)(a2->mdfPrivateDataPtr))->gao1 = 0;
        ((tLegLinkMdfStruct*)(a2->mdfPrivateDataPtr))->i3 = 5.0f;
        ((tLegLinkMdfStruct*)(a2->mdfPrivateDataPtr))->i4 = 5.0f;
        ((tLegLinkMdfStruct*)(a2->mdfPrivateDataPtr))->gao2 = 0;
        ((tLegLinkMdfStruct*)(a2->mdfPrivateDataPtr))->flags = 0;
    }
}

void GAO_ModifierLegLink_Destroy(struct sModifier* a1)
{
    MEM_Free(a1->mdfPrivateDataPtr);
}

void GAO_ModifierLegLink_DoIt(struct sLegLinkMdfStruct* a1, struct sGameObjectMain* a2)
{
    tGameObjectMain* v3;
    Vec3* v4;
    Vec3* v5;
    Vec3* v6;
    int v7;
    Vec3* v8;
    long double v9;
    long double v10;
    BOOL v11;
    Vec3* v12;
    Vec3* v13;
    double v14;
    double v15;
    Mtx4x4* v16;
    Mtx4x4* v17;
    float v18;
    Vec3* a3;
    tGameObjectMain* a3a;
    float v21;
    Vec3* v22;
    Vec3 src2;
    Vec3 a2a;
    Vec3 dest;
    float v26;
    Mtx4x4* v27;

    if (a1 && a1->gao0 && a1->gao1 && a2)
    {
        OBJ_ComputeGlobalWhenHie(a1->gao0);
        OBJ_ComputeGlobalWhenHie(a2);
        OBJ_ComputeGlobalWhenHie(a1->gao1);
        v3 = a1->gao0;
        v4 = &a1->gao0->matrices->Pos;
        v5 = &a2->matrices->Pos;
        v6 = &a1->gao1->matrices->Pos;
        if (a1->gao2)
            v3 = a1->gao2;
        v7 = a1->flags;
        if ((v7 & 1) != 0)
        {
            v8 = &OBJ_pst_GetAbsoluteMatrix(v3)->v1;
        }
        else if ((v7 & 2) != 0)
        {
            v8 = &OBJ_pst_GetAbsoluteMatrix(v3)->v2;
        }
        else
        {
            v8 = &OBJ_pst_GetAbsoluteMatrix(v3)->v3;
        }
        a3 = v8;
        v9 = MATH_DistanceOfTwoPoints(v4, v6);
        v10 = (a1->i3 * a1->i3 - a1->i4 * a1->i4 + v9 * v9) / (v9 + v9);
        v26 = (float)v10;
        v18 = (float)(a1->i3 * a1->i3 - v10 * v26);
        v21 = MATH_GetSqrt(v18);
        a2a.x = v6->x - v4->x;
        a2a.y = v6->y - v4->y;
        a2a.z = v6->z - v4->z;
        MATH_NormalizeEqualVector(&a2a);
        v11 = (a1->flags & 8) == 0;
        a2a.x = a2a.x * v26;
        a2a.y = a2a.y * v26;
        a2a.z = a2a.z * v26;
        if (v11)
        {
            v13 = a3;
            v12 = &a2a;
        }
        else
        {
            v12 = a3;
            v13 = &a2a;
        }
        MATH_CrossProduct(&dest, v13, v12);
        MATH_CrossProduct(&src2, &a2a, &dest);
        MATH_NormalizeEqualVector(&src2);
        v14 = src2.x * v21;
        v15 = src2.y * v21;
        src2.z = src2.z * v21;
        v5->x = a2a.x + v4->x;
        v5->y = a2a.y + v4->y;
        v5->z = a2a.z + v4->z;
        v5->x = (float)(v14 + v5->x);
        v5->y = (float)(v15 + v5->y);
        v5->z = src2.z + v5->z;
        a3a = a1->gao0;
        v27 = OBJ_pst_GetAbsoluteMatrix(a1->gao0);
        src2.x = v5->x - v4->x;
        v22 = &v27->v2;
        src2.y = v5->y - v4->y;
        src2.z = v5->z - v4->z;
        MATH_CrossProduct(&a2a, &v27->v2, &src2);
        v16 = v27;
        MATH_NormalizeVector(&v27->v1, &a2a);
        MATH_NormalizeVector(&v27->v3, &src2);
        MATH_CrossProduct(v22, &v27->v3, &v16->v1);
        OBJ_ComputeLocalWhenHie(a3a);
        v17 = OBJ_pst_GetAbsoluteMatrix(a2);
        src2.x = v6->x - v5->x;
        src2.y = v6->y - v5->y;
        src2.z = v6->z - v5->z;
        MATH_NormalizeVector(&v17->v3, &src2);
        MATH_CrossProduct(&a2a, &v17->v2, &src2);
        MATH_NormalizeVector(&v17->v1, &a2a);
        MATH_CrossProduct(&v17->v2, &v17->v3, &v17->v1);
        OBJ_ComputeLocalWhenHie(a2);
        OBJ_ComputeLocalWhenHie(a1->gao1);
    }
}

void GAO_ModifierLegLink_UnApply(struct sModifier* a1, void *unk1) {
}

void GAO_ModifierLegLink_Apply(struct sModifier* a1, void* unk1)
{
    tLegLinkMdfStruct* v1; // esi
    tGroup* v2; // eax
    tGroup* v3; // ebp
    tGameObjectMain* v4; // eax
    int v5; // eax
    tGameObjectMain* v6; // edi
    BOOL v7; // zf
    tGameObjectMain* v8; // ebx
    Vec3* v9; // eax
    Vec3* v10; // eax
    tGameObjectMain* v11; // [esp-14h] [ebp-34h]
    Vec3* v12; // [esp-Ch] [ebp-2Ch]
    Vec3* v13; // [esp-Ch] [ebp-2Ch]
    tGameObjectMain* v14; // [esp+8h] [ebp-18h]
    tGameObjectMain* v15; // [esp+Ch] [ebp-14h]
    tGameObjectMain* v16; // [esp+10h] [ebp-10h]
    Vec3 dest; // [esp+14h] [ebp-Ch] BYREF

    v1 = (tLegLinkMdfStruct*)a1->mdfPrivateDataPtr;
    v14 = v1->gao0;
    v15 = v1->gao1;
    v16 = v1->gao2;
    v2 = MDF_Modifier_GetCurGroup(a1);
    v3 = v2;
    if (v2)
    {
        v1->gao0 = OBJ_pst_GroupGetByRank(v2, (int)v1->gao0);
        v4 = OBJ_pst_GroupGetByRank(v3, (int)v1->gao1);
        v11 = v1->gao2;
        v1->gao1 = v4;
        v1->gao2 = OBJ_pst_GroupGetByRank(v3, (int)v11);
    }
    v5 = v1->flags;
    if (((v5 & 0x10) == 0 || v3) && (v5 & 0x20) != 0 && (v5 & 0x40) == 0)
    {
        v6 = v1->gao0;
        v7 = v1->gao0 == 0;
        v1->flags = v5 | 0x40;
        if (!v7)
        {
            v8 = v1->gao1;
            if (v8)
            {
                if ((v6->flags & 0x10000) != 0
                    && (gpst_DX_stDD->currTargetGao->flags & 0x10000) != 0
                    && (v8->flags & 0x10000) != 0)
                {
                    v12 = OBJ_pst_GetPos(gpst_DX_stDD->currTargetGao);
                    v9 = OBJ_pst_GetPos(v6);
                    MATH_SubstractVec(&dest, v9, v12);
                    v1->i3 = MATH_VecLen(&dest);
                    v13 = OBJ_pst_GetPos(gpst_DX_stDD->currTargetGao);
                    v10 = OBJ_pst_GetPos(v8);
                    MATH_SubstractVec(&dest, v10, v13);
                    v1->i4 = MATH_VecLen(&dest);
                }
            }
        }
    }
    if ((v1->flags & 0x10) == 0 || v3)
        GAO_ModifierLegLink_DoIt((tLegLinkMdfStruct*)a1->mdfPrivateDataPtr, gpst_DX_stDD->currTargetGao);
    v1->gao0 = v14;
    v1->gao1 = v15;
    v1->gao2 = v16;
}

int GAO_ModifierLegLink_Load(struct sModifier* a1, char* a2)
{
    tLegLinkMdfStruct* v2;
    char* v3;
    tGameObjectMain** v4;
    tGameObjectMain** v5;
    int v6;
    tGameObjectMain* v7;
    int v8;
    int v9;
    char* v11;

    v2 = (tLegLinkMdfStruct*)a1->mdfPrivateDataPtr;
    a1->modifierFlags |= 0x1000000u;
    v3 = a2;
    v11 = a2;
    LOA_ReadLong(&v11, 0, 1);
    v2->gao0 = (tGameObjectMain*)LOA_ReadLong(&v11, 0, 1);
    v4 = &v2->gao1;
    v2->gao1 = (tGameObjectMain*)LOA_ReadLong(&v11, 0, 1);
    v2->i3 = LOA_ReadFloat(&v11, 0, 1);
    v2->i4 = LOA_ReadFloat(&v11, 0, 1);
    v5 = &v2->gao2;
    v2->gao2 = (tGameObjectMain*)LOA_ReadLong(&v11, 0, 1);
    v6 = LOA_ReadLong(&v11, 0, 1) & ~0x40u;
    v2->flags = v6;
    if ((v6 & 0x10) == 0)
    {
        v7 = v2->gao0;
        if (!v2->gao0 || v7 == (tGameObjectMain*)-1)
            v2->gao0 = 0;
        else
            LOA_MakeFileRef((int)v7, &v2->gao0, OBJ_ul_GameObjectCallback, 1);
        v8 = (int)*v4;
        if (!*v4 || v8 == -1)
            *v4 = 0;
        else
            LOA_MakeFileRef(v8, &v2->gao1, OBJ_ul_GameObjectCallback, 1);
        v9 = (int)*v5;
        if (*v5 && v9 != -1)
        {
            LOA_MakeFileRef(v9, &v2->gao2, OBJ_ul_GameObjectCallback, 1);
            return v11 - v3;
        }
        *v5 = 0;
    }
    return v11 - v3;
}

void GAO_ModifierLegLink_Reinit(struct sModifier* a1)
{
    ((tLegLinkMdfStruct*)(a1->mdfPrivateDataPtr))->flags &= ~0x40u;
}