#include "../memory.h"
#include "../loading.h"
#include "speciallookatmodifier.h"

void GAO_ModifierSpecialLookAt_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tSpecialLookAtMdfStruct* v3;

    v3 = (tSpecialLookAtMdfStruct*)MEM_p_Alloc(sizeof(tSpecialLookAtMdfStruct));
    a2->modifierFlags |= 0x1000000u;
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        memcpy(v3, a3, sizeof(tSpecialLookAtMdfStruct));
    }
    else
    {
        v3->b1 = 2;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->b2 = 0;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->b3 = 0;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->b4 = 0;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->f1 = 5.0f;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->f2 = 5.0f;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->f3 = 5.0f;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->rank = -1;
        ((tSpecialLookAtMdfStruct*)(a2->mdfPrivateDataPtr))->gao = NULL;
    }
}

void GAO_ModifierSpecialLookAt_Destroy(tModifier* a1)
{
    MEM_Free(a1->mdfPrivateDataPtr);
}

void GAO_ModifierSpecialLookAt_Apply(struct sModifier* a1, void* unk1)
{
    tSpecialLookAtMdfStruct* v1;
    Mtx4x4* v2;
    Mtx4x4* v3;
    Vec3* v4;
    tGroup* v5;
    byte v6;
    double v7;
    double v8;
    Vec3* v9; 
    double v10;
    tGameObjectMain* v11;
    Mtx4x4* v12;
    double v13;
    double v14;
    double v15;
    double v16;
    double v17;
    long double v18;
    float v19;
    float v20;
    tGEOGRO* v21;
    tSpecialLookAtMdfStruct* v22;
    int v23;
    Vec3* v24;
    unsigned int v25;
    long double v26;
    double v27;
    tGameObjectMain* v28;
    Mtx4x4* v29;
    double v30;
    int a4;
    float a4a;
    float a4b;
    tSpecialLookAtMdfStruct* v34;
    float v35; 
    tGameObjectMain* v36;
    float v37;
    Vec3 a2;
    float v39;
    float v40;
    Vec3 a3;
    float a1a;

    v1 = (tSpecialLookAtMdfStruct*)a1->mdfPrivateDataPtr;
    v34 = v1;
    v36 = gpst_DX_stDD->currTargetGao;
    v2 = OBJ_pst_GetAbsoluteMatrix(v36);
    v3 = &gpst_DX_stDD->ct.cam.objectMtx;
    v4 = &v36->matrices->Pos;
    if (v1->rank != -1)
    {
        a4 = v1->rank;
        v5 = MDF_Modifier_GetCurGroup(a1);
        v1->gao = OBJ_pst_GroupGetByRank(v5, a4);
    }
    v6 = v1->b1;
    if (v1->b1)
    {
        if (v6 == 1)
        {
            v11 = v1->gao;
            if (v11)
            {
                v12 = v11->matrices;
                v13 = v3->Pos.x - v12->Pos.x;
                a2.x = (float)v13;
                a2.y = v3->Pos.y - v12->Pos.y;
                a2.z = v3->Pos.z - v12->Pos.z;
                a4a = v34->f1 * v34->f1 / (a2.z * a2.z + a2.y * a2.y + a2.x * a2.x);
                MATH_AddScaleVector(v4, &v12->Pos, &a2, a4a);
                v14 = v4->z - v12->Pos.z;
                v15 = v4->y - v12->Pos.y;
                v16 = v14 * v14 + v15 * v15;
                v17 = v4->x - v12->Pos.x;
                v18 = sqrt(v16 + v17 * v17);
                a4b = (float)(v34->f1 * v34->f1 - v18 * v18);
                a1a = MATH_GetSqrt(a4b);
                MATH_NormalizeVector(&v2->v2, &a2);
                v19 = v2->v2.y;
                v20 = v2->v2.z;
                v2->v2.x = -v2->v2.x;
                v2->v2.y = -v19;
                v2->v2.z = -v20;
                MATH_CrossProduct(&v2->v3, &v3->v1, &v2->v2);
                MATH_NormalizeEqualVector(&v2->v3);
                MATH_CrossProduct(&v2->v1, &v2->v2, &v2->v3);
                v21 = OBJ_p_GetCurrentGeo(v36);
                if (v21)
                {
                    v22 = v34;
                    v23 = v21->vertexNum;
                    v24 = v21->vertices;
                    v25 = 0;
                    v37 = (float)((a1a + v34->f2) / a1a * 100.0);
                    v26 = 0.0;
                    if (v23 > 0)
                    {
                        do
                        {
                            v25 += 2;
                            v24 += 2;
                            v24[-2].x = (float)(sin(v26) * v37);
                            v24[-2].y = -v22->f3;
                            v24[-2].z = (float)(cos(v26) * v37);
                            v35 = (float)(12.566371 / (double)v23);
                            v26 = v26 + v35;
                        } while (v25 < v21->vertexNum);
                    }
                }
                v27 = a1a * 0.0099999998;
                v2->mtxflag |= 8u;
                v2->_m34 = (float)v27;
                v2->_m24 = 1.0;
                v2->_m14 = (float)v27;
            }
        }
        else if (v6 == 2)
        {
            v28 = v1->gao;
            if (v28)
            {
                v29 = v28->matrices;
                v30 = v2->Pos.x - v29->Pos.x;
                a2.x = (float)v30;
                a2.y = v2->Pos.y - v29->Pos.y;
                a2.z = v2->Pos.z - v29->Pos.z;
                MATH_NormalizeVector(&v2->v2, &a2);
                MATH_CrossProduct(&v2->v3, &v3->v1, &v2->v2);
                MATH_NormalizeEqualVector(&v2->v3);
                MATH_CrossProduct(&v2->v1, &v2->v2, &v2->v3);
                OBJ_ComputeLocalWhenHie(v36);
            }
        }
    }
    else
    {
        v4->x = v3->Pos.x;
        v4->y = v3->Pos.y;
        v4->z = v3->Pos.z;
        v7 = v1->f1;
        v8 = v7 * v3->v3.x;
        v39 = (float)(v3->v3.y * v7);
        v40 = (float)(v3->v3.z * v7);
        v4->x = (float)(v8 + v4->x);
        v4->y = v39 + v4->y;
        v4->z = v40 + v4->z;
        v2->v1.x = v3->v1.x;
        v2->v1.y = v3->v1.y;
        v2->v1.z = v3->v1.z;
        v2->v2.x = -v3->v3.x;
        v2->v2.y = -v3->v3.y;
        v2->v2.z = -v3->v3.z;
        v2->v3.x = -v3->v2.x;
        v2->v3.y = -v3->v2.y;
        v2->v3.z = -v3->v2.z;
        a3.z = v1->f1;
        a3.x = 0.0;
        a3.y = 0.0;
        CAM_2Dto3DCamera2(&gpst_DX_stDD->ct.cam, &a2, &a3);
        v9 = &v36->boundingVolume->BVMin;
        v2->mtxflag |= 8u;
        v10 = a2.x / v9->x;
        v2->_m24 = 1.0;
        v2->_m14 = (float)v10;
        v2->_m34 = -(a2.y / v9->z);
    }
}

int GAO_ModifierSpecialLookAt_Load(struct sModifier* a1, char* a2)
{
    char* v2;
    tSpecialLookAtMdfStruct* v3;
    int v4;
    int v5;
    char* v7;

    v2 = a2;
    v3 = (tSpecialLookAtMdfStruct*)a1->mdfPrivateDataPtr;
    a1->modifierFlags |= 0x1000000u;
    v7 = v2;
    v4 = LOA_ReadLong(&v7, 0, 1);
    v3->b1 = LOA_ReadChar(&v7, 0, 1);
    v3->b2 = LOA_ReadChar(&v7, 0, 1);
    v3->b3 = LOA_ReadChar(&v7, 0, 1);
    v3->b4 = LOA_ReadChar(&v7, 0, 1);
    v3->f1 = LOA_ReadFloat(&v7, 0, 1);
    if (v4 == 28)
        v3->rank = LOA_ReadLong(&v7, 0, 1);
    else
        v3->rank = -1;
    v5 = LOA_ReadLong(&v7, 0, 1);
    if (v3->rank == -1 && v5 && v5 != -1)
        LOA_MakeFileRef(v5, &v3->gao, OBJ_ul_GameObjectCallback, 1);
    else
        v3->gao = 0;
    if (v4 == 4)
    {
        v3->f2 = 1000.0f;
        v3->f3 = 1000.0f;
    }
    else
    {
        v3->f2 = LOA_ReadFloat(&v7, 0, 1);
        v3->f3 = LOA_ReadFloat(&v7, 0, 1);
    }
    return v7 - v2;
}

void GAO_ModifierSpecialLookAt_Unapply(struct sModifier* a1, void* a2) {
}

void GAO_ModifierSpecialLookAt_Reinit(struct sModifier* a1) {
}