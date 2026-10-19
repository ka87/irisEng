#include "../memory.h"
#include "../time.h"
#include "../math.h"
#include "gfxlightning.h"

void GFX_Lightning_SampleZero(tGfxLightningData* a1)
{
    int v1;
    int v2;

    if (a1->pointsNumAllocd)
    {
        v1 = 0;
        if (a1->pointsNum > 0)
        {
            v2 = 0;
            do
            {
                a1->points[v2].x = 0.0;
                a1->points[v2].y = 0.0;
                a1->points[v2++].z = (float)((double)++v1 / (double)(a1->pointsNum + 1));
            } while (v1 < a1->pointsNum);
        }
    }
}

struct sGfxLightningData* GFX_Lightning_Create()
{
    tGfxLightningData* v0;

    v0 = (tGfxLightningData*)MEM_p_Alloc(sizeof(tGfxLightningData));
    v0->v1.x = -10.0f;
    v0->v1.y = -10.0f;
    v0->v1.z = 3.0f;
    v0->v2.z = 3.0f;
    v0->v2.x = 10.0f;
    v0->v2.y = 10.0f;
    v0->v3.z = 1.0;
    v0->v3.x = 0.0;
    v0->v3.y = 0.0;
    v0->f = 0.1f;
    v0->sampleFractalFloat = 2.0f;
    v0->pointsNum = 15;
    v0->pointsNumAllocd = 0;
    v0->vertexColor = -1;
    v0->timeElapsed = 0.0;
    v0->f3 = 0.0;
    v0->timeStart = 3.402822e38f;
    v0->deathTime = 3.402822e38f;
    v0->points = 0;
    GFX_Lightning_SampleZero(v0);
    return v0;
}

void GFX_Lightning_Destroy(struct sGfxLightningData* a1)
{
    if (a1)
    {
        if (a1->pointsNumAllocd)
        {
            a1->pointsNumAllocd = 0;
            MEM_Free(a1->points);
        }
    }
}

void GFX_AllocPattern(tGfxLightningData* a1)
{
    int v2;
    int v3;
    Vec3* v4;
    char v5;
    int v6;
    int v7;
    int v8;
    unsigned int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    Vec3* v15;
    Vec3* v16;
    int v17;
    int v18;
    int v19;
    int v20;
    int v21;
    int v22;
    Vec3* v23;
    Vec3* v24;
    int v25;
    unsigned int a1a;

    v2 = a1->pointsNum;
    v3 = v2;
    if (v2)
    {
        v5 = 0;
        do
        {
            v3 >>= 1;
            ++v5;
        } while (v3);
        v6 = (1 << (v5 - 1)) - 1;
        if ((1 << v5) - 1 - v2 <= v2 - v6)
            a1->pointsNum = (1 << v5) - 1;
        else
            a1->pointsNum = v6;
        v7 = a1->pointsNum;
        v8 = a1->pointsNumAllocd;
        v9 = 12 * v7;
        a1a = 12 * v7;
        if (v8)
        {
            if (v7 >= v8)
            {
                v16 = (Vec3*)MEM_p_Realloc(a1->points, sizeof(Vec3) * v7);
                a1->points = v16;
                memset(v16, 0, v9);
                v17 = a1->pointsNumAllocd;
                v18 = (a1->pointsNum + 1) / (v17 + 1);
                v19 = 0;
                if (v17 > 0)
                {
                    v20 = 0;
                    v21 = v18 - 1;
                    v22 = v18;
                    do
                    {
                        v23 = a1->points;
                        v23[v21].x = v23[v20].x;
                        v23[v21].y = v23[v20].y;
                        v23[v21].z = v23[v20].z;
                        ++v19;
                        ++v20;
                        v21 += v22;
                    } while (v19 < a1->pointsNumAllocd);
                }
                a1->pointsNumAllocd = a1->pointsNum;
            }
            else
            {
                v10 = (v8 + 1) / (v7 + 1);
                v11 = 0;
                if (v7 > 0)
                {
                    v12 = v10 - 1;
                    v13 = 0;
                    v14 = v10;
                    do
                    {
                        v15 = a1->points;
                        v15[v13].x = v15[v12].x;
                        v15[v13].y = v15[v12].y;
                        v15[v13].z = v15[v12].z;
                        ++v11;
                        ++v13;
                        v12 += v14;
                    } while (v11 < a1->pointsNum);
                    v9 = a1a;
                }
                a1->points = (Vec3*)MEM_p_Realloc(a1->points, v9);
                a1->pointsNumAllocd = a1->pointsNum;
            }
        }
        else
        {
            v24 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v7);
            v25 = a1->pointsNum;
            a1->points = v24;
            a1->pointsNumAllocd = v25;
            GFX_Lightning_SampleZero(a1);
        }
    }
    else
    {
        v4 = a1->points;
        a1->pointsNumAllocd = 0;
        MEM_Free(v4);
    }
}

void GFX_Lightning_SampleFractal(struct sGfxLightningData* a1)
{
    int v2; // eax
    int v3; // esi
    int v4; // eax
    char v5; // cl
    int v6; // ebp
    int v7; // edx
    double v8; // st7
    int v9; // edi
    int v10; // esi
    double v11; // st6
    int v12; // eax
    int v13; // ecx
    int v14; // ecx
    int v15; // esi
    Vec3* v16; // eax
    float v17; // edx
    double v19; // st7
    double v20; // st5
    double v22; // st6
    float v23; // [esp+4h] [ebp-48h]
    float v24; // [esp+8h] [ebp-44h]
    float v25; // [esp+Ch] [ebp-40h]
    float v26; // [esp+10h] [ebp-3Ch]
    int v27; // [esp+14h] [ebp-38h]
    int v28; // [esp+14h] [ebp-38h]
    int v29; // [esp+18h] [ebp-34h]
    int v30; // [esp+18h] [ebp-34h]
    float v31; // [esp+1Ch] [ebp-30h]
    float v32; // [esp+20h] [ebp-2Ch]
    float v33; // [esp+24h] [ebp-28h]
    float v34; // [esp+28h] [ebp-24h]
    int v35; // [esp+2Ch] [ebp-20h]
    int v36; // [esp+30h] [ebp-1Ch]
    int v37; // [esp+34h] [ebp-18h]
    int v38; // [esp+38h] [ebp-14h]
    float v39; // [esp+40h] [ebp-Ch]
    float v40; // [esp+44h] [ebp-8h]
    float v41; // [esp+48h] [ebp-4h]
    float a1a; // [esp+50h] [ebp+4h]

    if (a1->pointsNumAllocd)
    {
        v2 = a1->pointsNum;
        v3 = 0;
        v36 = 0;
        if (v2)
        {
            do
            {
                v2 >>= 1;
                ++v3;
            } while (v2);
            v36 = v3;
        }
        v4 = 0;
        v37 = 0;
        if (v3 > 0)
        {
            v5 = v3 - 1;
            v38 = v3 - 1;
            do
            {
                v6 = 1 << v5;
                v7 = 1 << v4;
                v33 = 0.0;
                v34 = 0.0;
                v23 = (float)(1.0 / (double)(1 << v5));
                v8 = 0.0;
                if (1 << v4 > 0)
                {
                    v9 = v6;
                    v10 = -1;
                    v11 = 1.0 / (double)(1 << v4);
                    v12 = v6 - 1;
                    v13 = v6 - 1;
                    v35 = v7;
                    a1a = (float)v11;
                    v31 = (float)(v11 * -6.2831855);
                    v32 = (float)(1.0 - v23);
                    while (1)
                    {
                        v14 = v9 + v13;
                        v15 = v9 + v10;
                        v27 = v6 + v12;
                        v29 = v14;
                        if (v6 + v12 == a1->pointsNum)
                        {
                            v24 = 0.0;
                            v25 = 0.0;
                            v26 = 1.0;
                        }
                        else
                        {
                            v16 = a1->points;
                            v17 = v16[v14].y;
                            v24 = v16[v14].x;
                            v25 = v17;
                            v26 = v16[v14].z;
                        }
                        a1->points[v15].z = (float)((v26 + v8) * 0.5);
                        v40 = (float)((double)rand() * 0.000030518509);
                        v41 = a1a * a1->sampleFractalFloat;
                        v19 = (double)rand();
                        v20 = v24 + v33;
                        v33 = v24;
                        v30 = v9 + v29;
                        v39 = (float)(a1a * 6.2831855 - v31);
                        a1->points[v15].x = (float)((v39 * v40 + v31 + v20 * 0.5) * v23  + v32 * a1->points[v15].x);
                        v22 = v25 + v34;
                        v10 = v9 + v15;
                        v34 = v25;
                        a1->points[v15].y = (float)((v22 * 0.5 + v19 * 0.000030518509 * v41) * v23 + v32 * a1->points[v15].y);
                        v8 = v26;
                        v28 = v6 + v27;
                        if (!--v35)
                            break;
                        v13 = v30;
                        v12 = v28;
                    }
                    v3 = v36;
                    v4 = v37;
                }
                ++v4;
                v5 = v38 - 1;
                v37 = v4;
                --v38;
            } while (v4 < v3);
        }
    }
}

int GFX_i_Lightning_Render(struct sGfxLightningData* data)
{
    double v2;
    Vec3* v4;
    int v5;
    tGdi_DisplayData* v6;
    double v7;
    float v8;
    float v9;
    float v10;
    float v11;
    int v12;
    Vec3* v13;
    int v14;
    int* v15;
    Vec3* v16;
    BOOL v17;
    Vec3* v18;
    double v19;
    int v20;
    int* v21;
    double v22;
    int v23;
    int v27;
    int v28;
    Vec3* v29;
    Vec3* v30;
    double v31;
    double v32;
    Vec3* v33;
    int v34;
    int v35;
    BOOL v36;
    Vec3* v37;
    Vec3* v38;
    double v39;
    double v40;
    Vec3* v41;
    tGEOGRO* v42;
    tGeoTriangleChunkElem* v43;
    int v44;
    int v45;
    uint16_t v46;
    tGeoTriangleChunkElem* v47;
    uint16_t v48;
    uint16_t v49;
    int v50;
    tGeoTriangleChunkElem* v51;
    uint16_t v52;
    int v53;
    float scale;
    float scalea;
    int v56;
    Vec3* v57;
    int v58;
    Vec3 a3;
    Vec3 v60;
    Vec3 a1a;
    Vec3 src2;
    Vec3 dest;
    float dataa;
    Vec3* datab;
    float datac;
    float datad;

    v2 = TIM_gf_dt + data->timeElapsed;
    data->timeElapsed = (float)v2;
    if (v2 > data->deathTime)
        return 0;
    if (data->pointsNumAllocd != data->pointsNum)
        GFX_AllocPattern(data);
    if (!data->pointsNumAllocd || !GFX_gpst_Current->aiGameObject->distance)
        return 1;
    MATH_TransformVertex(&a1a, &gpst_DX_stDD->ct.cam.cameraMtx, &data->v1);
    v4 = &data->v2;
    MATH_TransformVertex(&dest, &gpst_DX_stDD->ct.cam.cameraMtx, &data->v2);
    if (a1a.z < 0.0099999998 && dest.z < 0.0099999998)
    {
        --gpst_DX_stDD->rasters[14].unk2;
        return 1;
    }
    if (gpst_DX_stDD->ct.cam.planeLeft.y * a1a.y
        + gpst_DX_stDD->ct.cam.planeLeft.z * a1a.z
        + a1a.x * gpst_DX_stDD->ct.cam.planeLeft.x < 0.0
        && gpst_DX_stDD->ct.cam.planeLeft.y * dest.y
        + gpst_DX_stDD->ct.cam.planeLeft.z * dest.z
        + dest.x * gpst_DX_stDD->ct.cam.planeLeft.x < 0.0
        || gpst_DX_stDD->ct.cam.planeRight.z * a1a.z
        + gpst_DX_stDD->ct.cam.planeRight.y * a1a.y
        + a1a.x * gpst_DX_stDD->ct.cam.planeRight.x < 0.0
        && gpst_DX_stDD->ct.cam.planeRight.z * dest.z
        + gpst_DX_stDD->ct.cam.planeRight.y * dest.y
        + dest.x * gpst_DX_stDD->ct.cam.planeRight.x < 0.0)
    {
        --gpst_DX_stDD->rasters[14].unk2;
        return 1;
    }
    if (ENG_gb_EngineRunning)
        GFX_Lightning_SampleFractal(data);
    v5 = data->pointsNum;
    if ((data->flags & 2) != 0)
    {
        GFX_NeedGeom(2 * v5 + 4, 2 * v5 + 4, 2 * v5 + 2, 1);
    }
    else
    {
        GFX_NeedGeom(2 * v5 + 4, 4u, 2 * v5 + 2, 1);
        GFX_gpst_Geo->UVs->u = 0.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 0.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 1.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;
    }
    a1a.x = v4->x - data->v1.x;
    a1a.y = data->v2.y - data->v1.y;
    a1a.z = data->v2.z - data->v1.z;
    MATH_CrossProduct(&dest, &data->v3, &a1a);
    MATH_NormalizeEqualVector(&dest);
    v6 = gpst_DX_stDD;
    a3.x = gpst_DX_stDD->ct.cam.objectMtx.Pos.x - data->v1.x;
    a3.y = gpst_DX_stDD->ct.cam.objectMtx.Pos.y - data->v1.y;
    a3.z = gpst_DX_stDD->ct.cam.objectMtx.Pos.z - data->v1.z;
    src2.x = v4->x - data->v1.x;
    src2.y = data->v2.y - data->v1.y;
    v7 = data->v2.z - data->v1.z;
    src2.z = (float)v7;
    dataa = (float)((src2.z * a3.z + src2.y * a3.y + src2.x * a3.x) / (src2.y * src2.y + v7 * v7 + src2.x * src2.x));
    if (dataa >= 0.0)
    {
        if (dataa <= 1.0)
        {
            MATH_AddScaleVector(&a3, &data->v1, &src2, dataa);
        }
        else
        {
            v10 = data->v2.y;
            v11 = data->v2.z;
            a3.x = v4->x;
            a3.y = v10;
            a3.z = v11;
        }
    }
    else
    {
        v8 = data->v1.y;
        v9 = data->v1.z;
        a3.x = data->v1.x;
        a3.y = v8;
        a3.z = v9;
    }
    a3.x = a3.x - v6->ct.cam.objectMtx.Pos.x;
    a3.y = a3.y - v6->ct.cam.objectMtx.Pos.y;
    a3.z = a3.z - v6->ct.cam.objectMtx.Pos.z;
    MATH_CrossProduct(&v60, &a1a, &a3);
    if (MATH_b_NulVector(&v60))
    {
        MATH_VecMultScalar(&v60, &data->v3, data->f);
    }
    else
    {
        MATH_NormalizeEqualVector(&v60);
        MATH_VecMultScalarWithoutBuffer(&v60, data->f);
    }
    v12 = data->pointsNum;
    datab = &a3;
    v13 = GFX_gpst_Geo->vertices;
    v57 = &src2;
    v14 = 0;
    v15 = GFX_gpst_Geo->vertexColors + 1;
    v16 = &data->v1;
    v58 = 0;
    if (v12 >= 0)
    {
        v56 = 0;
        v17 = v12 > 0;
        do
        {
            if (v17)
            {
                MATH_AddScaleVector(datab, &data->v1, &a1a, data->points[v56].z);
                scale = (float)(cos(data->points[v56].x) * data->points[v56].y);
                MATH_AddScaleVector(datab, datab, &dest, scale);
                scalea = (float)(sin(data->points[v56].x) * data->points[v56].y);
                MATH_AddScaleVector(datab, datab, &data->v3, scalea);
                v18 = datab;
                v14 = v58;
            }
            else
            {
                v18 = &data->v2;
            }
            v13 += 2;
            v15 += 2;
            v13[-2].x = v60.x + v16->x;
            v58 = ++v14;
            v13[-2].y = v60.y + v16->y;
            v13[-2].z = v60.z + v16->z;
            v13[-1].x = v16->x - v60.x;
            v13[-1].y = v16->y - v60.y;
            v19 = v16->z;
            v16 = v18;
            datab = v57;
            v57 = v18;
            v13[-1].z = (float)(v19 - v60.z);
            *(v15 - 2) = data->vertexColor;
            *(v15 - 1) = data->vertexColor;
            v20 = data->pointsNum;
            v17 = v14 < v20;
            ++v56;
        } while (v14 <= v20);
    }
    v13->x = v60.x + v16->x;
    v13->y = v60.y + v16->y;
    v13->z = v60.z + v16->z;
    v13[1].x = v16->x - v60.x;
    v13[1].y = v16->y - v60.y;
    v13[1].z = v16->z - v60.z;
    *v15 = 0;
    v15[1] = 0;
    v21 = GFX_gpst_Geo->vertexColors + 1;
    *v21 = 0;
    v21[1] = 0;
    v22 = TIM_gf_dt + data->timeElapsed;
    v23 = 0;
    data->timeElapsed = (float)v22;
    if (v22 < data->f3)
    {
        v27 = data->pointsNum;
        v28 = 0;
        datac = (float)(v22 / data->f3);
        if (v27 <= 0)
            goto LABEL_40;
        v29 = data->points;
        v30 = v29;
        do
        {
            if (v30->z >= (double)datac)
                break;
            ++v28;
            ++v30;
        } while (v28 < data->pointsNum);
        if (v28)
        {
            if (v28 == v27)
                v32 = 1.0 - v29[v28 - 1].z;
            else
                v32 = v29[v28].z - v29[v28 - 1].z;
            v31 = (datac - v29[v28 - 1].z) / v32;
        }
        else
        {
        LABEL_40:
            if (!v27)
            {
            LABEL_47:
                v33 = &GFX_gpst_Geo->vertices[2 * v28];
                MATH_BlendVector(v33 + 2, v33, v33 + 2, datac);
                MATH_BlendVector(v33 + 3, v33 + 1, v33 + 3, datac);
                v23 = 0;
                GFX_gpst_Geo->triangleChunks->triangleNum = 2 * v28 + 2;
                goto LABEL_48;
            }
            v31 = datac / data->points->z;
        }
        datac = (float)v31;
        goto LABEL_47;
    }
LABEL_48:
    if (data->timeElapsed <= (double)data->timeStart)
        goto LABEL_63;
    v34 = data->pointsNum;
    v35 = 0;
    v36 = v34 == 0;
    datad = (data->timeElapsed - data->timeStart) / (data->deathTime - data->timeStart);
    if (v34 > 0)
    {
        v37 = data->points;
        v38 = v37;
        do
        {
            if (v38->z >= (double)datad)
                break;
            ++v35;
            ++v38;
        } while (v35 < data->pointsNum);
        if (v35)
        {
            if (v35 == v34)
                v40 = 1.0 - v37[v35 - 1].z;
            else
                v40 = v37[v35].z - v37[v35 - 1].z;
            v39 = (datad - v37[v35 - 1].z) / v40;
            goto LABEL_61;
        }
        v36 = v34 == 0;
    }
    if (!v36)
    {
        v39 = datad / data->points->z;
    LABEL_61:
        datad = (float)v39;
    }
    v41 = &GFX_gpst_Geo->vertices[2 * v35];
    MATH_BlendVector(v41, v41, v41 + 2, datad);
    MATH_BlendVector(v41 + 1, v41 + 1, v41 + 3, datad);
    v23 = v35;
    GFX_gpst_Geo->triangleChunks->triangleNum += -2 * v35;
LABEL_63:
    v42 = GFX_gpst_Geo;
    v43 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    v44 = data->pointsNum;
    if ((data->flags & 2) != 0)
    {
        if (v23 <= v44)
        {
            do
            {
                if (v23)
                {
                    v45 = 2 * v23;
                    v42->UVs[v45].u = data->points[v23 - 1].z;
                    GFX_gpst_Geo->UVs[v45].v = 0.0;
                    GFX_gpst_Geo->UVs[v45 + 1].u = data->points[v23 - 1].z;
                    GFX_gpst_Geo->UVs[v45 + 1].v = 1.0;
                }
                else
                {
                    v42->UVs->u = 0.0;
                    GFX_gpst_Geo->UVs->v = 0.0;
                    GFX_gpst_Geo->UVs[1].u = 0.0;
                    GFX_gpst_Geo->UVs[1].v = 1.0;
                }
                v43->v1 = 2 * v23;
                v43->uv1 = 2 * v23;
                v46 = 2 * v23 + 3;
                v43->v3 = v46;
                v43->uv3 = v46;
                v47 = v43 + 1;
                v48 = 2 * v23 + 1;
                v47[-1].v2 = v48;
                v47[-1].uv2 = v48;
                v49 = 2 * v23 + 2;
                v47->v1 = v46;
                v47->v2 = v49;
                v47->v3 = 2 * v23;
                v47->uv1 = v46;
                v47->uv2 = v49;
                v47->uv3 = 2 * v23;
                v42 = GFX_gpst_Geo;
                v43 = v47 + 1;
                ++v23;
            } while (v23 <= data->pointsNum);
        }
        v50 = 2 * v23;
        v42->UVs[v50].u = 1.0;
        GFX_gpst_Geo->UVs[v50].v = 0.0;
        GFX_gpst_Geo->UVs[v50 + 1].u = 1.0;
        GFX_gpst_Geo->UVs[v50 + 1].v = 1.0;
        goto LABEL_72;
    }
    if (v23 <= v44)
    {
        do
        {
            v43->v1 = 2 * v23;
            v43->uv2 = 0;
            v43->v2 = 2 * v23 + 1;
            v51 = v43 + 1;
            v52 = 2 * v23 + 3;
            v51[-1].v3 = v52;
            v51[-1].uv1 = 2;
            v51[-1].uv3 = 1;
            v51->v1 = v52;
            v51->v2 = 2 * v23 + 2;
            v51->v3 = 2 * v23;
            v51->uv1 = 1;
            v51->uv2 = 3;
            v51->uv3 = 2;
            v43 = v51 + 1;
            ++v23;
        } while (v23 <= data->pointsNum);
    LABEL_72:
        v42 = GFX_gpst_Geo;
    }
    v42->triangleChunks->materialNum = 0;
    v53 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v53 & ~0x8000900u;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v53;
    return 1;
}