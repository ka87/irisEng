#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "../ai/ai_mathfunctions.h"
#include "snakemodifier.h"

void GAO_ModifierSNAKE_Apply(struct sModifier* a1, void *unk1)
{
    tSnakeMdfStruct* v2; 
    tGameObjectMain* v3;
    int v4;
    tGameObjectMain** v5;
    float* v5a;
    Mtx4x4* v6;
    float v7;
    Vec3* v8;
    Mtx4x4* v9;
    float v10; 
    float v11; 
    Vec3* v12; 
    Mtx4x4* v13;
    float v14;
    Mtx4x4* v15;
    Mtx4x4* v16;
    double v17; 
    double v18; 
    Mtx4x4* v19;
    Vec3* v20; 
    double v21; 
    Mtx4x4* v22;
    float v23; 
    Vec3* v24; 
    float v25; 
    double v26;
    double v27;
    long double v28;
    long double v29;
    char v30; 
    Mtx4x4* v31; 
    double v32;
    float a4;
    float a4a;
    float norm;
    float v36; 
    float v37; 
    Vec3 src; 
    Vec3 a5;
    Vec3 a2;
    Vec3 a3;
    Vec3 v42; 
    Vec3 a1a; 
    float v44;

    v36 = TIM_gf_dt;
    if (TIM_gf_dt == 0.0)
        v36 = 0.02f;
    v2 = (tSnakeMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = v2->gao;
    if (v3)
    {
        v4 = 1;
        OBJ_ComputeGlobalWithLocal(v3, v3->matrices, 1);
        v44 = 0.0;
        if (v2->num > 1)
        {
            v5 = v2->gaos;
            v5a = v2->distances;
            do
            {
                if (!*v5 || !v5[1])
                    break;
                v6 = (*v5)->matrices;
                if ((a1->modifierFlags & 1) != 0)
                {
                    v7 = v6->v3.x;
                    v8 = &v6->v3;
                    a3.x = v7;
                    a3.y = v8->y;
                    a3.z = v8->z;
                    v9 = v5[1]->matrices;
                    v10 = v9->v3.x;
                    a2.x = v10;
                    a2.y = v9->v3.y;
                    a2.z = v9->v3.z;
                }
                else
                {
                    v11 = v6->v2.x;
                    v12 = &v6->v2;
                    a3.x = -v11;
                    a3.y = -v12->y;
                    a3.z = -v12->z;
                    v13 = v5[1]->matrices;
                    v14 = v13->v2.x;
                    a2.x = -v14;
                    a2.y = -v13->v2.y;
                    a2.z = -v13->v2.z;
                }
                a4 = v36 * v2->inertia;
                AI_EvalFunc_MATHVecBlendRotate_C(&a2, &a3, a4, &a5);
                a3 = a5;
                v15 = v5[1]->matrices;
                v16 = (*v5)->matrices;
                v17 = v15->Pos.x - v16->Pos.x;
                src.x = (float)v17;
                src.y = v15->Pos.y - v16->Pos.y;
                v18 = v15->Pos.z - v16->Pos.z;
                src.z = (float)v18;
                v37 = (float)(sqrt(v18 * src.z + src.y * src.y + src.x * src.x));
                if (v4 < 2)
                {
                    v22 = (*v5)->matrices;
                    if ((a1->modifierFlags & 1) != 0)
                    {
                        v23 = v22->v2.x;
                        v24 = &v22->v2;
                        a2.x = v23;
                    }
                    else
                    {
                        v25 = v22->v3.x;
                        v24 = &v22->v3;
                        a2.x = v25;
                    }
                    a2.y = v24->y;
                    a2.z = v24->z;
                }
                else
                {
                    v19 = (*v5)->matrices;
                    v20 = &(*(v5 - 1))->matrices->Pos;
                    v21 = v19->Pos.x - v20->x;
                    a2.x = (float)v21;
                    a2.y = v19->Pos.y - v20->y;
                    a2.z = v19->Pos.z - v20->z;
                }
                MATH_SetNormVector(&a5, &a2, *v5a);
                a2 = a5;
                a4a = (v44 + v2->inertia) * v36;
                AI_EvalFunc_MATHVecBlendRotate_C(&src, &a2, a4a, &a5);
                src = a5;
                v26 = v44 + v2->f3;
                v44 = (float)v26;
                if (v26 + v2->inertia < 0.0)
                    v44 = -v2->inertia;
                v27 = v36 * v2->f2;
                if (v27 >= 0.0)
                {
                    if (v27 > 1.0)
                        v27 = 1.0;
                }
                else
                {
                    v27 = 0.0;
                }
                norm = (float)((1.0 - v27) * *v5a + v27 * v37);
                MATH_SetNormVector(&a5, &src, norm);
                src = a5;
                v28 = 1.0 / sqrt(a5.z * a5.z + a5.y * a5.y + a5.x * a5.x);
                a5.x = (float)(a5.x * v28);
                a5.y = (float)(a5.y * v28);
                a5.z = (float)(a5.z * v28);
                v29 = 1.0 / sqrt(a3.z * a3.z + a3.y * a3.y + a3.x * a3.x);
                v42.x = (float)(a3.x * v29);
                v30 = a1->modifierFlags;
                v42.y = (float)(a3.y * v29);
                v42.z = (float)(a3.z * v29);
                if ((v30 & 1) != 0)
                {
                    a5.x = -a5.x;
                    a5.y = -a5.y;
                    a5.z = -a5.z;
                    OBJ_SightGeneralSet(v5[1], &a5, &v42);
                }
                else
                {
                    OBJ_BankingGeneralSet(v5[1], &v42, &a5);
                }
                v31 = (*v5)->matrices;
                v32 = src.x + v31->Pos.x;
                src.x = (float)v32;
                src.y = src.y + v31->Pos.y;
                src.z = src.z + v31->Pos.z;
                v5[1]->matrices->Pos = src;
                MATH_GetScale(&a1a, &v5[1]->basic->mtx->mtx);
                OBJMY_ComputeLocalWhenHie(v5[1]);
                MATH_SetScale(&v5[1]->basic->mtx->mtx, &a1a);
                ++v4;
                ++v5;
                ++v5a;
            } while (v4 < v2->num);
        }
    }
}

void GAO_ModifierSNAKE_UpdatePointers(tModifier* a1)
{
    tSnakeMdfStruct* snake;
    tGroup* grp;
    tGameObjectMain* gao;
    Vec3* currPos, *nextPos;
    float diffX, diffY, diffZ;
    int i, j;

    snake = (tSnakeMdfStruct*)a1->mdfPrivateDataPtr;
    snake->gao = NULL;
    if (snake->num >= 2)
    {
        grp = MDF_Modifier_GetCurGroup(a1);
        if (grp)
        {
            j = 0;
            if (snake->num <= 0)
            {
            LABEL_6:
                snake->gao = snake->gaos[0];

                for (i = 1; i < snake->num; i++) {
                    currPos = &snake->gaos[i - 1]->matrices->Pos;
                    nextPos = &snake->gaos[i]->matrices->Pos;
                    diffX = currPos->x - nextPos->x;
                    diffY = currPos->y - nextPos->y;
                    diffZ = currPos->z - nextPos->z;
                    snake->distances[i - 1] = (float)sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
                }
            }
            else
            {
                while (1)
                {
                    gao = OBJ_pst_GroupGetByRank(grp, snake->ranks[j]);
                    snake->gaos[j] = gao;
                    if (!gao)
                        break;
                    OBJ_RestoreInitialPos(gao);
                    OBJ_ComputeGlobalWithLocal(snake->gaos[j], snake->gaos[j]->matrices, 1);
                    if (++j >= snake->num)
                        goto LABEL_6;
                }
            }
        }
    }
}

void GAO_ModifierSNAKE_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tSnakeMdfStruct* v3;

    v3 = (tSnakeMdfStruct*)MEM_p_Alloc(sizeof(tSnakeMdfStruct));
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        a2->modifierFlags = 0x1000000;
        if ((MDF_gpst_GlobalCreate->modifierFlags & 1) != 0)
            a2->modifierFlags = 0x1000001;
        memcpy(v3, a3, sizeof(tSnakeMdfStruct));
    }
    else
    {
        memset(v3, 0, sizeof(tSnakeMdfStruct));
        a2->modifierFlags = 0x1000000;
        GAO_ModifierSNAKE_UpdatePointers(a2);
    }
}

void GAO_ModifierSNAKE_Destroy(struct sModifier* a1)
{
    if (a1->mdfPrivateDataPtr)
        MEM_Free(a1->mdfPrivateDataPtr);
}

int GAO_ModifierSNAKE_Load(struct sModifier* arg0, char* a1)
{
    char* v2;
    tSnakeMdfStruct* v3;
    int* v4;
    int v5;

    v2 = a1;
    v3 = (tSnakeMdfStruct*)arg0->mdfPrivateDataPtr;
    LOA_ReadLong(&a1, 0, 2);
    v3->num = LOA_ReadLong(&a1, 0, 1);
    v3->inertia = LOA_ReadFloat(&a1, 0, 1);
    v3->f2 = LOA_ReadFloat(&a1, 0, 1);
    v3->f3 = LOA_ReadFloat(&a1, 0, 1);
    v4 = v3->ranks;
    v5 = 20;
    do
    {
        *v4++ = LOA_ReadLong(&a1, 0, 1);
        --v5;
    } while (v5);
    return a1 - v2;
}

void GAO_ModifierSNAKE_Reinit(struct sModifier* a1)
{
    GAO_ModifierSNAKE_UpdatePointers(a1);
}

void GAO_ModifierSNAKE_Unapply(struct sModifier* a1, void* unk1) { 
}