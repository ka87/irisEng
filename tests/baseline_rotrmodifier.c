#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "rotrmodifier.h"

void GAO_ModifierROTR_UpdatePointers(tModifier* a1)
{
    tRotrMdfStruct* v1;
    tGameObjectMain* v2;
    tGameObjectBasic* v3;
    tGaoMatrix* v4;
    tGameObjectMain* v5;
    tGaoMatrix* v6;

    v1 = (tRotrMdfStruct*)a1->mdfPrivateDataPtr;
    v1->gao = 0;
    v2 = a1->gao;
    if (v2)
    {
        v3 = v2->basic;
        if (v3)
        {
            v4 = v3->mtx;
            if (v4)
            {
                v5 = v4->refGao2;
                if (v5)
                {
                    v1->gao = v5;
                    v6 = a1->gao->basic->mtx;
                    v1->v1.x = v6->mtx.v3.x;
                    v1->v1.y = v6->mtx.v3.y;
                    v1->v1.z = v6->mtx.v3.z;
                }
            }
        }
    }
}

void GAO_ModifierROTR_Create(struct sGameObjectMain* a1, struct sModifier* a2,  void* a3)
{
    tRotrMdfStruct* v3;
    tRotrMdfStruct* v4;

    v3 = (tRotrMdfStruct*)MEM_p_Alloc(sizeof(tRotrMdfStruct));
    v4 = v3;
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        a2->modifierFlags = 0x1000000;
        memcpy(v3, a3, sizeof(tRotrMdfStruct));
        v3->gao = 0;
    }
    else
    {
        memset(v3, 0, sizeof(tRotrMdfStruct));
        a2->modifierFlags = 0x1000000;
        GAO_ModifierROTR_UpdatePointers(a2);
        v4->gao = 0;
    }
}

void GAO_ModifierROTR_Destroy(struct sModifier* a1)
{
    if (a1->mdfPrivateDataPtr)
        MEM_Free(a1->mdfPrivateDataPtr);
}

void GAO_ModifierROTR_Apply(struct sModifier* a1, void *unk1)
{
    tRotrMdfStruct* v1;
    tGameObjectMain* v2;
    Mtx4x4* v3;
    Mtx4x4* v4;
    double v5;
    double v6;
    double v7;
    float v8; 
    float v9; 
    double v10;
    double v11;
    tGaoMatrix* v12;
    float v13;
    float v14;
    tGaoMatrix* v15;
    double v16;
    long double v17;
    long double v18;
    long double v19;
    long double v20;
    double v21;
    long double v22;
    Vec3 a1a;
    Vec3 a2; 
    float v25;
    float v26;
    float v27;
    Mtx4x4 v28;

    v1 = (tRotrMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1->gao)
    {
        v2 = a1->gao;
        v3 = v2->matrices;
        if (&v28 != v3)
            memcpy(&v28, v3, sizeof(v28));
        OBJ_ComputeGlobalWithLocal(v2, v3, 1);
        v4 = a1->gao->matrices;
        v5 = v28.Pos.x - v4->Pos.x;
        a1a.x = (float)v5;
        a1a.y = v28.Pos.y - v4->Pos.y;
        v6 = v28.Pos.z - v4->Pos.z;
        a1a.x = v28.v3.x + a1a.x;
        a1a.y = v28.v3.y + a1a.y;
        a1a.z = (float)(v28.v3.z + v6);
        MATH_SetNormVector(&a1a, &a1a, 1.0);
        MATH_VectorGlobalToLocal(&a1a, v1->gao->matrices, &a1a);
        v7 = TIM_gf_dt * v1->ff1;
        v8 = v1->v1.y;
        v9 = v1->v1.z;
        v25 = v1->v1.x;
        v26 = v8;
        v27 = v9;
        if (v7 >= 0.0)
        {
            if (v7 > 1.0)
                v7 = 1.0;
        }
        else
        {
            v7 = 0.0;
        }
        a2.x = (float)((1.0 - v7) * a1a.x + v7 * v25);
        v10 = TIM_gf_dt * v1->timeFactor1;
        if (v10 >= 0.0)
        {
            if (v10 > 1.0)
                v10 = 1.0;
        }
        else
        {
            v10 = 0.0;
        }
        a2.y = (float)((1.0 - v10) * a1a.y + v10 * v26);
        v11 = TIM_gf_dt * v1->timeFactor2;
        if (v11 >= 0.0)
        {
            if (v11 > 1.0)
                v11 = 1.0;
        }
        else
        {
            v11 = 0.0;
        }
        a2.z = (float)((1.0 - v11) * a1a.z + v11 * v27);
        MATH_SetNormVector(&a2, &a2, 1.0);
        v12 = a1->gao->basic->mtx;
        v13 = a2.y;
        v12->mtx.v3.x = a2.x;
        v14 = a2.z;
        v12->mtx.v3.y = v13;
        v12->mtx.v3.z = v14;
        v15 = a1->gao->basic->mtx;
        v15->mtx.v2.x = v15->mtx.v3.y * v15->mtx.v1.z - v15->mtx.v3.z * v15->mtx.v1.y;
        v15->mtx.v2.y = v15->mtx.v1.x * v15->mtx.v3.z - v15->mtx.v1.z * v15->mtx.v3.x;
        v16 = v15->mtx.v3.x * v15->mtx.v1.y - v15->mtx.v3.y * v15->mtx.v1.x;
        v15->mtx.v2.z = (float)v16;
        v17 = sqrt(v15->mtx.v2.x * v15->mtx.v2.x + v15->mtx.v2.y * v15->mtx.v2.y + v16 * v16);
        v15->mtx.v2.x = (float)(1.0 / v17 * v15->mtx.v2.x);
        v18 = 1.0 / v17 * v15->mtx.v2.y;
        v15->mtx.v2.y = (float)v18;
        v19 = v18;
        v20 = 1.0 / v17 * v15->mtx.v2.z;
        v15->mtx.v2.z = (float)v20;
        v15->mtx.v1.x = (float)(v19 * v15->mtx.v3.z - v20 * v15->mtx.v3.y);
        v15->mtx.v1.y = v15->mtx.v3.x * v15->mtx.v2.z - v15->mtx.v2.x * v15->mtx.v3.z;
        v21 = v15->mtx.v3.y * v15->mtx.v2.x - v15->mtx.v3.x * v15->mtx.v2.y;
        v15->mtx.v1.z = (float)v21;
        v22 = sqrt(v21 * v21 + v15->mtx.v1.x * v15->mtx.v1.x + v15->mtx.v1.y * v15->mtx.v1.y);
        v15->mtx.v1.x = (float)(1.0 / v22 * v15->mtx.v1.x);
        v15->mtx.v1.y = (float)(1.0 / v22 * v15->mtx.v1.y);
        v15->mtx.v1.z = (float)(1.0 / v22 * v15->mtx.v1.z);
    }
}

int GAO_ModifierROTR_Load(struct sModifier* arg0, char* a1)
{
    tRotrMdfStruct* v2;
    char* v3;

    v2 = (tRotrMdfStruct*)arg0->mdfPrivateDataPtr;
    v3 = a1;
    LOA_ReadLong(&a1, 0, 2);
    v2->ii = LOA_ReadLong(&a1, 0, 1);
    v2->ff1 = LOA_ReadFloat(&a1, 0, 1);
    v2->timeFactor1 = LOA_ReadFloat(&a1, 0, 1);
    v2->timeFactor2 = LOA_ReadFloat(&a1, 0, 1);
    v2->ff4 = LOA_ReadFloat(&a1, 0, 1);
    v2->gao = 0;
    return a1 - v3;
}

void GAO_ModifierROTR_Reinit(struct sModifier* a1)
{
    GAO_ModifierROTR_UpdatePointers(a1);
}

void GAO_ModifierROTR_Unapply(struct sModifier* a1, void* unk1) {
}