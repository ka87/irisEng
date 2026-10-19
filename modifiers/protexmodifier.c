#include "../math.h"
#include "../gameobject.h"
#include "../modifiers.h"
#include "../memory.h"
#include "../loading.h"
#include "../materialanddraw.h"
#include "../textures.h"
#include "protexmodifier.h"

void PROTEX_Disturb(struct sGameObjectMain* gao, struct sGEOGRO* geogro)
{
    tGameObjectMain* v2;
    tProtexMdfStruct* v3;
    int v4;
    Mtx4x4* v5;
    char v6;
    unsigned int v7;
    double v8;
    double v9;
    double v10;
    int v11;
    long double v12;
    long double v13;
    long double v14;
    long double v15;
    Vec3* v16;
    Vec3* v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    double v24;
    double v25;
    int v26; 
    int v27; 
    int v28; 
    int v29; 
    int v30; 
    int v31; 
    float v32;
    float v33;
    int v34;
    Vec3 v35;
    Vec3 v36;
    float v37;
    int v37a;
    unsigned int v38;
    float v39;
    float v40;
    float v41;
    int v42;
    int v43;
    float v44;
    float v45;
    Vec3 v46;
    Mtx4x4 a1a;

    v2 = gao;
    v3 = p_FirstActiveProtex;
    if (gao)
        v4 = gao->flags;
    else {
        v4 = v4 & ~0xff; //LOBYTE(v4) = 0;
    }
    if ((v4 & 5) != 0 && p_FirstActiveProtex)
    {
        do
        {
            if (v3->gao != v2)
            {
                if (v3->unkPtrFreedInUnapply)
                {
                    v5 = v2->matrices;
                    v6 = v5->mtxflag;
                    v7 = 0;
                    v45 = 0.0;
                    v44 = 0.0;
                    if ((v6 & 8) == 0)
                    {
                        MATH_InvertMatrix(&a1a, v5);
                        MATH_TransformVertex(&v46, &a1a, &v3->v1);
                        a1a.Pos.z = 0.0;
                        a1a.Pos.y = 0.0;
                        a1a.Pos.x = 0.0;
                        MATH_TransformVertex(&v35, &a1a, &v3->v2);
                        MATH_TransformVertex(&v36, &a1a, &v3->v3);
                        v8 = v35.z * v36.y - v35.y * v36.z;
                        v9 = v36.z * v35.x - v35.z * v36.x;
                        v10 = v35.y * v36.x - v36.y * v35.x;
                        v41 = (float)v10;
                        v32 = (float)(1.0 / sqrt(v10 * v41 + v9 * v9 + v8 * v8));
                        v39 = (float)(v8 * v32);
                        v40 = (float)(v9 * v32);
                        v41 = v41 * v32;
                        v11 = geogro->vertexNum;
                        v12 = sqrt(v35.y * v35.y + v35.z * v35.z + v35.x * v35.x);
                        v13 = 1.0 / v12 * (1.0 / v12);
                        v35.x = (float)(v35.x * v13);
                        v35.y = (float)(v35.y * v13);
                        v35.z = (float)(v35.z * v13);
                        v14 = sqrt(v36.z * v36.z + v36.y * v36.y + v36.x * v36.x);
                        v15 = 1.0 / v14 * (1.0 / v14);
                        v36.x = (float)(v36.x * v15);
                        v36.y = (float)(v36.y * v15);
                        v36.z = (float)(v36.z * v15);
                        v16 = gpst_DX_stDD->verticesCurrent;
                        v17 = &v16[v11];
                        if (v16 < v17)
                        {
                            v18 = v44;
                            v19 = v45;
                            do
                            {
                                v20 = v16->x - v46.x;
                                v21 = v16->y - v46.y;
                                v22 = v16->z - v46.z;
                                v23 = v41 * v22 + v21 * v40 + v20 * v39;
                                if (v23 * v23 < 0.02)
                                {
                                    v33 = (float)(v35.z * v22 + v21 * v35.y + v20 * v35.x);
                                    v37 = (float)(v22 * v36.z + v21 * v36.y + v20 * v36.x);
                                    if (v33 > 0.0 && v37 > 0.0 && v33 < 1.0 && v37 < 1.0)
                                    {
                                        ++v7;
                                        v18 = v18 + v33;
                                        v19 = v19 + v37;
                                    }
                                }
                                ++v16;
                            } while (v16 < v17);
                            if (v7)
                            {
                                v38 = v7;
                                v24 = 1.0 / (double)v7;
                                v25 = (double)(unsigned int)v3->unkProbablyDim2;
                                v43 = (uint32_t)(v24 * v18 * (double)(unsigned int)v3->unkProbablyDim1 + 1048576.0);
                                v26 = 3;
                                v38 = (uint32_t)(v24 * v19 * v25 + 1048576.0);
                                v34 = 1;
                                v42 = 3;
                                do
                                {
                                    v27 = 0;
                                    v37a = 0;
                                    if (v26 > 0)
                                    {
                                        do
                                        {
                                            v28 = (v3->unkProbablyDim1 - 1) & (v3->unkProbablyDim2 + v43 + v27 - v34);
                                            v29 = 0;
                                            do
                                            {
                                                v30 = v3->unkProbablyDim1;
                                                v31 = (v30 - 1) & (v3->unkProbablyDim2 + v38 + v29 - v34);
                                                v3->unkPtrFreedInUnapply[v28 + v31 * v30] += 512;
                                                v3->unkPtr2[v28 + v31 * v3->unkProbablyDim1] -= 512;
                                                v26 = v42;
                                                ++v29;
                                            } while (v29 < v42);
                                            ++v37a;
                                            v27 = v37a;
                                        } while (v37a < v42);
                                        v2 = gao;
                                    }
                                    v26 += 2;
                                    ++v34;
                                    v42 = v26;
                                } while (v26 < 11);
                            }
                        }
                    }
                }
            }
            v3 = v3->prevProtex;
        } while (v3);
    }
}

void PROTEX_Modifier_Reinit(struct sModifier* a1)
{
    tProtexMdfStruct* v1;
    uint16_t* v2;
    uint16_t* v3;

    v1 = (tProtexMdfStruct*)a1->mdfPrivateDataPtr;
    if ((v1->flags & 0x40) != 0)
    {
        v2 = v1->unkPtrFreedInUnapply;
        if (v2)
            memset(v2, 0, 2 * v1->unkProbablyDim1 * v1->unkProbablyDim2);
        v3 = v1->unkPtr2;
        if (v3)
            memset(v3, 0, 2 * v1->unkProbablyDim1 * v1->unkProbablyDim2);
    }
}

void PROTEX_Modifier_Destroy(struct sModifier* a1)
{
    tProtexMdfStruct* v1;
    tProtexMdfStruct** v2;
    tProtexMdfStruct* v3; 
    uint16_t* v4;
    char* v5;

    v1 = (tProtexMdfStruct*)a1->mdfPrivateDataPtr;
    v2 = &p_FirstActiveProtex;
    if ((v1->flags & 0x20) != 0 && p_FirstActiveProtex)
    {
        do
        {
            v3 = *v2;
            if (*v2 == v1)
                *v2 = v3->prevProtex;
            else
                v2 = &v3->prevProtex;
        } while (*v2);
    }
    if (v1->ptr3)
    {
        MEM_Free(v1->ptr3);
        v1->ptr3 = 0;
    }
    v4 = v1->unkPtrFreedInUnapply;
    if (v4)
    {
        if (v4 <= v1->unkPtr2)
            MEM_FreeAlign(v1->unkPtrFreedInUnapply);
        else
            MEM_FreeAlign(v1->unkPtr2);
        v1->unkPtrFreedInUnapply = 0;
        v1->unkPtr2 = 0;
    }
    if (v1->i1)
    {
        MEM_Free(v1->ptrCharArray);
        v5 = v1->ptrCharArray2;
        v1->ptrCharArray = 0;
        v1->i1 = 0;
        MEM_Free(v5);
        v1->ptrCharArray2 = 0;
        v1->i2 = 0;
    }
    MEM_Free(v1);
}

void PROTEX_SetStUstVstP(tProtexMdfStruct* a1, Vec3* a2, Vec3* a3, Vec3* a4)
{
    a1->v1 = *a4;
    a1->v2 = *a2;
    a1->v3 = *a3;
}

void PROTEX_Apply2DMatrix(tProtexMdfStruct* a1, tMultiTextureElemStruct* a2)
{
    long double v2;
    long double v3;
    long double v4;
    long double v5;
    long double v6;
    double v7;
    double v8;
    double v9;
    double v10;
    Vec3 v11;
    Vec3 v12;
    float v13;
    float v14;
    tMatDecompressedStruct a2a;

    if (!MAT_VUISIdentity(a2))
    {
        v11 = a1->v2;
        v12 = a1->v3;
        MAT_UVDecompress(a2, &a2a);
        v2 = sqrt(a2a.f3 * a2a.f3 + a2a.f1 * a2a.f1);
        v3 = 1.0 / v2 * (1.0 / v2);
        v4 = a2a.f1 * v3;
        a2a.f3 = (float)(a2a.f3 * v3);
        v5 = sqrt(a2a.f4 * a2a.f4 + a2a.f2 * a2a.f2);
        v6 = 1.0 / v5 * (1.0 / v5);
        a2a.f2 = (float)(a2a.f2 * v6);
        a2a.f4 = (float)(a2a.f4 * v6);
        a1->v2.x = (float)(v11.x * v4);
        a1->v2.y = (float)(v11.y * v4);
        a1->v2.z = (float)(v11.z * v4);
        v13 = v12.z * a2a.f3;
        v7 = v12.y * a2a.f3;
        a1->v2.x = v12.x * a2a.f3 + a1->v2.x;
        a1->v2.y = (float)(a1->v2.y + v7);
        a1->v2.z = a1->v2.z + v13;
        a1->v3.x = v11.x * a2a.f2;
        a1->v3.y = v11.y * a2a.f2;
        a1->v3.z = v11.z * a2a.f2;
        v14 = v12.z * a2a.f4;
        v8 = v12.y * a2a.f4;
        a1->v3.x = v12.x * a2a.f4 + a1->v3.x;
        a1->v3.y = (float)(a1->v3.y + v8);
        a1->v3.z = a1->v3.z + v14;
        v9 = -a2a.speedU;
        v12.y = (float)(a1->v2.y * v9);
        v12.z = (float)(a1->v2.z * v9);
        a1->v1.x = (float)(v9 * a1->v2.x + a1->v1.x);
        a1->v1.y = a1->v1.y + v12.y;
        a1->v1.z = a1->v1.z + v12.z;
        v10 = -a2a.speedV;
        v12.y = (float)(a1->v3.y * v10);
        v12.z = (float)(a1->v3.z * v10);
        a1->v1.x = (float)(v10 * a1->v3.x + a1->v1.x);
        a1->v1.y = a1->v1.y + v12.y;
        a1->v1.z = a1->v1.z + v12.z;
    }
}

void PROTEX_InitModifier(tProtexMdfStruct* a1)
{
    unsigned int v1;
    long double v2;
    uint32_t v3;
    int tmp;

    v1 = 256;
    do
    {
        v2 = fabs(sin((double)--v1 * 0.049087387));
        if (v1 <= 128)
            v3 = 0;
        else
            v3 = (uint32_t)(v2 * v2 * (v2 * v2) * 128.0);
        tmp = v3 | (((unsigned int)v3 | (((unsigned int)v3 | ((unsigned int)(uint32_t)(255.0 * v2) << 8)) << 8)) << 8);
        //f_RandomTableLastElem_unk_9A0620[v1] = (*((float*)(&tmp)));
        f_RandomTable[v1] = (*((float*)(&tmp)));
    } while (v1);
    memset(a1, 0, sizeof(tProtexMdfStruct));
}

int PROTEX_ul_Modifier_Load(tModifier* arg0, char* a1)
{
    char* v2;
    tProtexMdfStruct* v3;
    int v4;
    int v5;
    char* v6;
    int v7;
    char* v8;
    int v10;
    int v11;

    v2 = a1;
    v3 = (tProtexMdfStruct*)arg0->mdfPrivateDataPtr;
    LOA_ReadLong(&a1, 0, 1);
    v4 = LOA_ReadLong(&a1, 0, 1);
    PROTEX_InitModifier(v3);
    v3->gao = arg0->gao;
    if (v4)
    {
        v5 = LOA_ReadLong(&a1, 0, 1);
        v3->i1 = v5;
        v6 = (char*)MEM_p_Alloc(v5);
        v11 = v3->i1;
        v3->ptrCharArray = v6;
        LOA_ReadCharArray(&a1, v6, v11, 1);
        v7 = LOA_ReadLong(&a1, 0, 1);
        v3->i2 = v7;
        v8 = (char*)MEM_p_Alloc(v7);
        v10 = v3->i2;
        v3->ptrCharArray2 = v8;
        LOA_ReadCharArray(&a1, v8, v10, 1);
    }
    return a1 - v2;
}

void PROTEX_Modifier_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tProtexMdfStruct* v3;

    v3 = (tProtexMdfStruct*)MEM_p_Alloc(sizeof(tProtexMdfStruct));
    a2->mdfPrivateDataPtr = v3;
    if (a3)
    {
        memcpy(v3, a3, sizeof(tProtexMdfStruct));
        v3->gao = a1;
    }
    else
    {
        PROTEX_InitModifier(v3);
    }
}

void PROTEX_l_WaterUpdate(tProtexMdfStruct* a1)
{
    int v2;
    unsigned int v3;
    int v4;
    int v5;
    __int16* v6;
    __int16* v7;
    __int16* v8;
    int v9;                         
    int v10;
    __int16* v11;
    __int16* v12;
    uint16_t* v13;
    uint8_t* v14;
    uint16_t* v15;
    int v16;
    uint16_t v17; 
    uint16_t v18; 
    float* v19;
    uint8_t* v20;
    float* i; 
    __int16 v22;
    int v23;
    int v24;
    int v25;
    __int16 v26;
    int a1a;
    int a1b;

    v2 = a1->unkProbablyDim1;
    a1a = a1->unkProbablyDim2;
    v3 = 2 * v2;
    memset(a1->unkPtrFreedInUnapply, 0, 2 * v2);
    memset(a1->unkPtr2, 0, 2 * v2);
    v4 = v2 * (2 * a1a - 2);
    v23 = v2;
    memset((char*)a1->unkPtrFreedInUnapply + v4, 0, 2 * v2);
    memset((char*)a1->unkPtr2 + v4, 0, 2 * v2);
    v5 = a1a;
    v6 = &a1->unkPtrFreedInUnapply[v2 + 1];
    v7 = &a1->unkPtr2[v2 + 1];
    if (a1a != 2)
    {
        v25 = a1a - 2;
        do
        {
            if (v2 != 2)
            {
                v8 = &v7[(v3 / -2) + 1];          // v3/-2 + 1 => ebbõl minusz akármennyi lesz; visszatolja az elejére mert a v3 az v2+1-el el van tolva
                v24 = v2 - 2;
                do
                {
                    v9 = ((v7[1] + *v8 + *(v7 - 1) + *(v8 - 1) + *(v8 - 2) + v7[v3 / 2 + 1] + v7[v3 / 2] + v7[v3 / 2 - 1]) >> 2)
                        - *v6
                        - ((((v7[1] + *v8 + *(v7 - 1) + *(v8 - 1) + *(v8 - 2) + v7[v3 / 2 + 1] + v7[v3 / 2] + v7[v3 / 2 - 1]) >> 2)
                            - *v6) >> 8);
                    if (v9 >= (int)0xFFFFE000)
                    {
                        if (v9 > 0x2000)
                            v9 = (v9 & 0xffff0000) | 0x2000;  //LOWORD(v9) = 0x2000;
                    }
                    else
                    {
                        v9 = (v9 & 0xffff0000) | 0xE000;  //LOWORD(v9) = 0xE000;
                    }
                    *v6++ = v9;
                    ++v7;
                    ++v8;
                    --v24;
                } while (v24);
                v2 = v23;
            }
            v10 = v25;
            *v6 = 0;
            v11 = v6 + 1;
            *v11 = 0;
            *v7 = 0;
            v12 = v7 + 1;
            *v12 = 0;
            v6 = v11 + 1;
            v7 = v12 + 1;
            v25 = v10 - 1;
        } while (v10 != 1);
        v5 = a1a;
    }
    v13 = a1->unkPtr2;
    v14 = a1->unkNumProtexWater;
    a1->unkPtr2 = a1->unkPtrFreedInUnapply;
    a1->unkPtrFreedInUnapply = v13;
    v15 = v13;
    if (v5)
    {
        a1b = v5;
        do
        {
            if (v2)
            {
                v16 = v2;
                do
                {
                    v17 = *v15;
                    v18 = v15[1];
                    ++v15;
                    *v14++ = ((v17 - v18) >> 6) + 0x80;
                    --v16;
                } while (v16);
            }
            --a1b;
        } while (a1b);
    }
    v19 = a1->ptr3;
    v20 = a1->unkNumProtexWater;
    for (i = &v19[a1->unkProbablyDim1 * a1->unkProbablyDim2]; v19 < i; ++v20) {
     //   *v19++ = f_RandomTableLastElem_unk_9A0620[*v20];
        *v19++ = f_RandomTable[*v20];
    }
    v22 = a1->texElemIdx;
    v26 = v22;
    gpst_DX_stDD->Request(37, (void *)a1->ptr3); // does nothing
}

void PROTEX_UpdateWaterEffect(tProtexMdfStruct* a1)
{
    int v1;
    int v2;
    uint8_t* v3;
    uint8_t* v4;
    int v5;
    int v6;
    uint8_t* v7;
    int v8;

    if ((a1->flags & 0x40) != 0)
    {
        if (!a1->unkPtrFreedInUnapply)
        {
            v1 = a1->texElemsI22field;
            v2 = TEX_gst_GlobalList.texElems[v1].normalData.height;
            a1->unkProbablyDim2 = v2;
            v8 = 5 * TEX_gst_GlobalList.texElems[v1].normalData.width * v2;
            a1->unkProbablyDim1 = TEX_gst_GlobalList.texElems[v1].normalData.width;
            v3 = (unsigned __int8*)MEM_p_AllocAligned(v8, 64);
            v4 = v3;
            a1->unkPtrFreedInUnapply = (uint16_t*)v3;
            if (!v3)
                return;
            v5 = a1->unkProbablyDim2;
            v6 = v5 * a1->unkProbablyDim1;
            v7 = &v3[2 * v6];
            a1->unkPtr2 = (uint16_t*)v7;
            a1->unkNumProtexWater = &v7[2 * v6];
            memset(v4, 0, 5 * v5 * a1->unkProbablyDim1);
            a1->ptr3 = (float*)MEM_p_Alloc(4 * a1->unkProbablyDim1 * a1->unkProbablyDim2);
        }
        PROTEX_l_WaterUpdate(a1);
    }
}

void PROTEX_BeforeDraw()
{
    tProtexMdfStruct** v0;
    int v1;

    QueryPerformanceCounter(&PerfCounterStruct[20]);
    v0 = &p_FirstActiveProtex;
    if (p_FirstActiveProtex)
    {
        do
        {
            v1 = (*v0)->flags;
            if ((v1 & 8) != 0)
            {
                PROTEX_UpdateWaterEffect(*v0);
                (*v0)->flags &= ~8u;
                (*v0)->flags |= 0x10u;
                v0 = &(*v0)->prevProtex;
            }
            else
            {
                (*v0)->flags = v1 & ~0x20u;
                *v0 = (*v0)->prevProtex;
            }
        } while (*v0);
    }
    PRO_QPC_RecordTimeDecRef(20);
}

void PROTEX_ComputeStUstVstP(tProtexMdfStruct* a1, tMultiTextureElemStruct* a2, tGameObjectMain* a3)
{
    uint16_t v3;
    int v4;
    int v5;
    Mtx4x4* v6;
    Mtx4x4* v7;
    Mtx4x4* v8;
    Mtx4x4* v9;
    Mtx4x4* v10;
    Mtx4x4* v11;
    Mtx4x4* v12;
    Mtx4x4* v13;
    Mtx4x4* v14;
    Mtx4x4* v15;
    Mtx4x4* v16;
    Mtx4x4* v17;
    Mtx4x4* v18;
    Vec3 a2a;
    Vec3 v20;
    Mtx4x4 a3a;
    Mtx4x4 v22;

    if ((a2->subTextureFlags & 0xF00000) != 0x600000)
        return;
    v3 = a2->phongOrPlanarParam;
    v4 = (v3 >> 4) & 3;
    if ((v3 & 0xC0) == 0)
    {
        if (v4)
        {
            v5 = v4 - 1;
            if (v5)
            {
                if (v5 == 1)
                {
                    v6 = OBJ_pst_GetAbsoluteMatrix(a3);
                    MATH_MakeOGLMatrix(&v22, v6);
                    v7 = OBJ_pst_GetAbsoluteMatrix(a3);
                    PROTEX_SetStUstVstP(a1, &v22.v1, &v22.v2, &v7->Pos);
                    PROTEX_Apply2DMatrix(a1, a2);
                }
                else
                {
                LABEL_14:
                    PROTEX_Apply2DMatrix(a1, a2);
                }
                return;
            }
        }
        else
        {
            v8 = OBJ_pst_GetAbsoluteMatrix(a3);
            MATH_MakeOGLMatrix(&v22, v8);
            v9 = OBJ_pst_GetAbsoluteMatrix(a3);
            PROTEX_SetStUstVstP(a1, &v22.v2, &v22.v3, &v9->Pos);
        }
        v10 = OBJ_pst_GetAbsoluteMatrix(a3);
        MATH_MakeOGLMatrix(&v22, v10);
        v11 = OBJ_pst_GetAbsoluteMatrix(a3);
        PROTEX_SetStUstVstP(a1, &v22.v1, &v22.v3, &v11->Pos);
        PROTEX_Apply2DMatrix(a1, a2);
        return;
    }
    MAT_UV_COMPUTE_GetMAtrix(gpst_DX_stDD, a3, v3, &a3a);
    switch (v4)
    {
    case 0:
        v12 = OBJ_pst_GetAbsoluteMatrix(a3);
        MATH_MulMatrixMatrix(&v22, v12, &a3a);
        v13 = OBJ_pst_GetAbsoluteMatrix(a3);
        PROTEX_SetStUstVstP(a1, &v22.v2, &v22.v3, &v13->Pos);
        goto LABEL_11;
    case 1:
    LABEL_11:
        v14 = OBJ_pst_GetAbsoluteMatrix(a3);
        MATH_MulMatrixMatrix(&v22, v14, &a3a);
        v15 = OBJ_pst_GetAbsoluteMatrix(a3);
        PROTEX_SetStUstVstP(a1, &v22.v1, &v22.v3, &v15->Pos);
        PROTEX_Apply2DMatrix(a1, a2);
        break;
    case 2:
        v16 = OBJ_pst_GetAbsoluteMatrix(a3);
        MATH_MulMatrixMatrix(&v22, v16, &a3a);
        v17 = OBJ_pst_GetAbsoluteMatrix(a3);
        PROTEX_SetStUstVstP(a1, &v22.v1, &v22.v2, &v17->Pos);
        PROTEX_Apply2DMatrix(a1, a2);
        break;
    case 3:
        a2a.x = (float)(a3a.v3.x * 0.0 - a3a.v2.x * 0.70710677 + a3a.v1.x * 0.70710677);
        a2a.y = (float)(a3a.v3.y * 0.0 - a3a.v2.y * 0.70710677 + a3a.v1.y * 0.70710677);
        a2a.z = (float)(a3a.v3.z * 0.0 - a3a.v2.z * 0.70710677 + a3a.v1.z * 0.70710677);
        v20.x = (float)(a3a.v3.x * 0.57735026 - (a3a.v2.x + a3a.v1.x) * 0.57735026);
        v20.y = (float)(a3a.v3.y * 0.57735026 - (a3a.v2.y + a3a.v1.y) * 0.57735026);
        v20.z = (float)(a3a.v3.z * 0.57735026 - (a3a.v2.z + a3a.v1.z) * 0.57735026);
        v18 = OBJ_pst_GetAbsoluteMatrix(a3);
        PROTEX_SetStUstVstP(a1, &a2a, &v20, &v18->Pos);
        goto LABEL_14;
    default:
        goto LABEL_14;
    }
}

void PROTEX_Modifier_Unapply(tModifier* a1, void *unk1)
{
    tGameObjectBasic* v1;
    tGameObjectVisualObject* v2;
    tGEOMAT* v3;
    int v4;
    tMultiTextureElemStruct* v5;
    int v6;
    tMultiTextureElemStruct* i;
    int v8;
    tProtexMdfStruct* v9;
    int v10;
    tProtexMdfStruct* v11;
    tGameObjectMain* a3;

    v9 = (tProtexMdfStruct*)a1->mdfPrivateDataPtr;
    v10 = v9->flags;
    if ((v10 & 0x20) == 0)
    {
        v9->prevProtex = p_FirstActiveProtex;
        v9->flags = v10 | 0x20;
        p_FirstActiveProtex = v9;
    }
    v9->flags |= 8u;
    a3 = a1->gao;
    v11 = v9;
    if (a3)
    {
        v1 = a3->basic; 
        if (v1)
        {
            v2 = (tGameObjectVisualObject *)(v1->actorObjectOrVisualObject);
            if (v2)
            {
                v3 = (tGEOMAT*)(v2->GROMAT);
                if (v3)
                {
                    if (v3->intf->id == 4)
                    {
                        v4 = v3->multiMaterialNumIfType4ThenAmbiantColor - 1;
                        if (v11->maxMaterialID <= v4)
                            v4 = v11->maxMaterialID;
                        v3 = v3->multiMaterialPtrIfType4ThenDiffuseColor[v4];
                    }
                    if (v3->intf->id == 5)
                    {
                        v5 = (tMultiTextureElemStruct*)v3->idxOrPtr;
                        v6 = v11->unkMatCnt;
                        if (v5)
                        {
                            for (i = v5->nextTextureKey; i; --v6)
                            {
                                if (!v6)
                                    break;
                                v5 = i;
                                i = i->nextTextureKey;
                            }
                            PROTEX_ComputeStUstVstP(v11, v5, a3);
                            v8 = v5->texElemIdx;
                            if (v8 != -1)
                            {
                                if ((TEX_gst_GlobalList.texElems[v8].texElemFlag & 0x40) != 0)
                                {
                                    v11->texElemIdx = v8;
                                    v8 = TEX_gst_GlobalList.texElems[v8].proceduralData.paletteID;
                                }
                                if (v8 != -1)
                                {
                                    if ((v11->flags & 0x40) != 0 && v8 != (v11->texElemsI22field & 0xffff) && v11->unkPtrFreedInUnapply)
                                    {
                                        MEM_FreeAlign(v11->unkPtrFreedInUnapply);
                                        v11->unkPtrFreedInUnapply = 0;
                                    }
                                    v11->texElemsI22field = v8;
                                    if (v8 < TEX_gst_GlobalList.actTexElemNum)
                                        v11->flags |= 0x40u;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void PROTEX_Modifier_Apply(tModifier* a1, void* unk1) {
}