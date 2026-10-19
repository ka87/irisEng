#include "../memory.h"
#include "../time.h"
#include "gfxspark.h"

struct sGfxSparkData* GFX_Spark_Create()
{
    tGfxSparkData* v0;

    v0 = (tGfxSparkData*)MEM_p_Alloc(sizeof(tGfxSparkData));
    memset(v0, 0, sizeof(tGfxSparkData));
    v0->i10 = 0.89999998f;
    v0->v1.x = 0.0;
    v0->v1.y = 0.0;
    v0->v1.z = 1.0;
    v0->i4 = 0.0;
    v0->i5 = 0.0;
    v0->i6 = -2.0f;
    v0->ii1 = 2.0f;
    v0->rand2Max = 2.0f;
    v0->rand3Min = 2.0f;
    v0->rand2Min = 4.0f;
    v0->iii1 = -1;
    v0->rand3Max = 1.0;
    v0->ii4 = 1.0;
    v0->ii5 = 0.1f;
    v0->flags = 0x20;
    return v0;
}

void GFX_Spark_Destroy(struct sGfxSparkData* a1)
{
    if (a1)
    {
        if (a1->allocdNum)
        {
            a1->allocdNum = 0;
            MEM_Free(a1->ptr);
        }
    }
}

void GFX_Spark_Generate(struct sGfxSparkData* sd)
{
    double v1;
    int v2;
    tGfxSparkElemData* v3;
    double v4;
    double v5;
    double v6;
    double v7;
    long double v8; 
    long double v9;
    int v10;
    double v11;
    double v15;
    double v19;
    double v21;
    float v22; 
    float v23; 
    float v24; 
    float v25; 
    float v26; 
    float v27; 
    float v28; 
    int v34; 
    float v35; 
    float v36; 
    float v37; 
    float v38; 
    float v39; 
    float v40; 
    float v41; 
    float v42; 
    float v43; 
    float v44; 
    float v45; 
    float v46; 
    float v47; 
    float v48; 
    float v49; 
    float v50; 

    v1 = sd->timeRemaining - TIM_gf_dt;
    sd->timeRemaining = (float)v1;
    if (v1 < 0.0)
    {                                             // CrossProduct
                                                  // NulVector
                                                  // CrossProduct
                                                  // CrossProduct
                                                  // NormalizeEqualVector
                                                  // NormalizeEqualVector
        do
        {
            v2 = sd->actNum;
            if (v2 >= sd->num)
                break;
            sd->timeRemaining = sd->ii5 + sd->timeRemaining;
            v37 = MATH_gst_BaseVectorK.z * sd->v1.y - MATH_gst_BaseVectorK.y * sd->v1.z;
            v39 = MATH_gst_BaseVectorK.x * sd->v1.z - MATH_gst_BaseVectorK.z * sd->v1.x;
            v41 = MATH_gst_BaseVectorK.y * sd->v1.x - MATH_gst_BaseVectorK.x * sd->v1.y;
            if (fabs(v37) == 0
                && !fabs(MATH_gst_BaseVectorK.x * sd->v1.z - MATH_gst_BaseVectorK.z * sd->v1.x)
                && !fabs(MATH_gst_BaseVectorK.y * sd->v1.x - MATH_gst_BaseVectorK.x * sd->v1.y))
            {
                v37 = MATH_gst_BaseVectorJ.z * sd->v1.y - MATH_gst_BaseVectorJ.y * sd->v1.z;
                v39 = MATH_gst_BaseVectorJ.x * sd->v1.z - MATH_gst_BaseVectorJ.z * sd->v1.x;
                v41 = MATH_gst_BaseVectorJ.y * sd->v1.x - MATH_gst_BaseVectorJ.x * sd->v1.y;
            }
            v3 = &sd->ptr[v2];
            v4 = v39 * sd->v1.z - v41 * sd->v1.y;
            v5 = v41 * sd->v1.x - v37 * sd->v1.z;
            v6 = v37 * sd->v1.y;
            v7 = v39 * sd->v1.x;
            sd->actNum = v2 + 1;
            v45 = (float)(v6 - v7);
            v8 = 1.0 / sqrt(v41 * v41 + v39 * v39 + v37 * v37);
            v38 = (float)(v37 * v8);
            v40 = (float)(v39 * v8);
            v42 = (float)(v41 * v8);
            v9 = 1.0 / sqrt(v45 * v45 + v5 * v5 + v4 * v4);
            v22 = (float)v9;
            v43 = (float)(v9 * v4);
            v44 = (float)(v5 * v22);
            v46 = v45 * v22;
            v3->v1.x = sd->v0.x;
            v3->v1.y = sd->v0.y;
            v3->v1.z = sd->v0.z;
            v23 = -sd->i7;
            v28 = sd->i7;
            v24 = (float)((double)rand() * 0.000030518509 * (v28 - v23) + v23);
            v10 = rand();
            v35 = sd->rand2Min;
            v25 = sd->rand2Max;
            v26 = (float)((double)v10 * 0.000030518509 * 6.2831855);
            v11 = (double)rand() * 0.000030518509 * (v35 - v25) + v25;

            v15 = MATH_GetCos(v26) * MATH_GetSin(v24) * v11;

            v3->i5 = (float)(v38 * v15);
            v3->i6 = (float)(v40 * v15);
            v3->i7 = (float)(v42 * v15);

            v19 = MATH_GetSin(v26) * MATH_GetSin(v24) * v11;

            v47 = (float)(v44 * v19);
            v48 = (float)(v46 * v19);
            v3->i5 = (float)(v43 * v19 + v3->i5);              // addScaleVector
            v3->i6 = v3->i6 + v47;
            v3->i7 = v3->i7 + v48;

            v21 = v11 *  MATH_GetCos(v24);

            v49 = (float)(sd->v1.y * v21);                     // addScaleVector
            v50 = (float)(sd->v1.z * v21);
            v3->i5 = (float)(v21 * sd->v1.x + v3->i5);
            v3->i6 = v3->i6 + v49;
            v3->i7 = v3->i7 + v50;
            v36 = sd->rand3Min;
            v27 = sd->rand3Max;
            v34 = rand();
            v3->i9 = 0.0;
            v3->i1 = 0;
            v3->i8 = (float)((double)v34 * 0.000030518509 * (v36 - v27) + v27);
        } while (sd->timeRemaining < 0.0);
    }
}

void GFX_Spark_Update(tGfxSparkData* sd, Vec3* a2, Vec3* a3, int a4)
{
    int v5;
    tGfxSparkElemData* v6;
    double v7; 
    tGfxSparkElemData* v8;
    double v9;
    double v10;
    int v11; 
    int v12; 
    int v13; 
    double i;
    double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    double v20;
    int v21;
    double v22;
    int v23; 
    double v24;
    tGfxSparkElemData* v25;
    int v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    double v33;
    int v34; 
    double v35;
    double v36;
    double v37;
    float v38; 
    int v39; 
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    float v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    float v60;
    float v61;
    float v62;
    float v63;
    tGfxSparkElemData* sda;

    v5 = sd->actNum;
    v6 = sd->ptr;
    v7 = sd->ii1 * sd->ii1;
    sda = v6;
    v39 = 0;
    v43 = (float)v7;
    if (v5 > 0)
    {
        v8 = v6; // v6->i6
        do
        {
            v9 = v8->i8 - TIM_gf_dt;
            v8->i8 = (float)v9;
            if (v9 >= 0.0)
                goto LABEL_9;
            if ((sd->flags & 0x10) == 0 || (v6->i1 & 1) != 0)
            {
                v25 = sd->ptr;
                v26 = sd->actNum - 1;
                sd->actNum = v26;
                memcpy(v6, &v25[v26], sizeof(tGfxSparkElemData));
            }
            else
            {
                v42 = sd->randMax;
                v40 = sd->randMin;
                v10 = (double)rand() * 0.000030518509 * (v42 - v40) + v40;
                if (v10 > 0.0)
                {
                    v41 = (float)v10;
                    v12 = MATH_gl_TableInverse[((*(int32_t *)&v41) >> 13) & 0x3FF];
                    v8->i8 = (float)(v10 + v8->i8);
                    v13 = sda->i1 | 1;
                    *(uint32_t *)(&(v8->i9)) = v12 - ((*(uint32_t *)&v41) & 0xFF800000) + 0x7E800000;
                    sda->i1 = v13;
                    v6 = sda;
                LABEL_9:
                    for (i = TIM_gf_dt; i >= 0.033333335; i = i - 0.016666668)
                    {
                        v15 = v8->i6 * 0.016666668;
                        v60 = (float)(v8->i7 * 0.016666668);
                        v8->v1.x = (float)(v8->i5 * 0.016666668 + v8->v1.x);
                        v8->v1.y = (float)(v8->v1.y + v15);
                        v8->v1.z = v8->v1.z + v60;
                        v16 = sd->i5 * 0.016666668;
                        v61 = (float)(sd->i6 * 0.016666668);
                        v8->i5 = (float)(sd->i4 * 0.016666668 + v8->i5);
                        v8->i6 = (float)(v8->i6 + v16);
                        v8->i7 = v8->i7 + v61;
                        v17 = sd->i10;
                        v8->i5 = (float)(v17 * v8->i5);
                        v8->i6 = (float)(v17 * v8->i6);
                        v8->i7 = (float)(v17 * v8->i7);
                    }
                    *a2 = v8->v1;
                    v18 = i * v8->i6;
                    v62 = (float)(i * v8->i7);
                    v8->v1.x = (float)(i * v8->i5 + v8->v1.x);
                    v8->v1.y = (float)(v18 + v8->v1.y);
                    v8->v1.z = v62 + v8->v1.z;
                    *a3 = v8->v1;
                    v58 = (float)(sd->i5 * i);
                    v59 = (float)(sd->i6 * i);
                    v8->i5 = (float)(i * sd->i4 + v8->i5);
                    v8->i6 = v58 + v8->i6;
                    v8->i7 = v59 + v8->i7;
                    v19 = sd->i10;
                    v8->i5 = (float)(v19 * v8->i5);
                    v8->i6 = (float)(v19 * v8->i6);
                    v8->i7 = (float)(v19 * v8->i7);
                    v45 = a2->x - a3->x;
                    v46 = a2->y - a3->y;
                    v20 = a2->z - a3->z;
                    v50 = (float)v20;
                    v21 = sd->flags & 3;
                    v22 = v20 * v50 + v46 * v46 + v45 * v45;
                    if (v21)
                    {
                        v23 = v21 - 1;
                        if (v23)
                        {
                            if (v23 == 1)
                            {
                                v24 = sd->ii4 - 1.0;
                                v47 = (float)(v46 * v24);
                                v51 = (float)(v50 * v24);
                                a2->x = (float)(v45 * v24 + a2->x);
                                a2->y = v47 + a2->y;
                                a2->z = v51 + a2->z;
                                goto LABEL_19;
                            }
                            goto LABEL_20;
                        }
                        v27 = (sd->ii4 - 1.0) * 0.5;
                        v28 = v45 * v27;
                        v48 = (float)(v46 * v27);
                        v52 = (float)(v50 * v27);
                        a2->x = (float)(v28 + a2->x);
                        a2->y = v48 + a2->y;
                        a2->z = v52 + a2->z;
                        a3->x = (float)(a3->x - v28);
                        a3->y = a3->y - v48;
                        v29 = a3->z - v52;
                    }
                    else
                    {
                        v30 = 1.0 - sd->ii4;
                        v49 = (float)(v46 * v30);
                        v53 = (float)(v50 * v30);
                        a3->x = (float)(v45 * v30 + a3->x);
                        a3->y = v49 + a3->y;
                        v29 = v53 + a3->z;
                    }
                    a3->z = (float)v29;
                LABEL_19:
                    v22 = v22 * sd->ii4;
                LABEL_20:
                    if (v22 >= v43)
                    {
                        a2[1].x = sd->ii1;
                    }
                    else
                    {
                        v44 = (float)v22;
                        *(uint32_t *)&(a2[1].x) = MATH_gl_TableRacine[((*(int32_t *)&v44) >> 13) & 0x7FF] + (((((*(uint32_t *)&v44) & 0x7F800000) + 0x3F800000) >> 1) & 0x7F800000);
                    }
                    a2[1].x = a2[1].x * 4.0f;
                    v31 = a2->x - a3->x;
                    v32 = a2->y - a3->y;
                    v54 = a2->z - a3->z;
                    v33 = a2->y + a3->y;
                    v56 = a2->z + a3->z;
                    v55 = (a2->x + a3->x) * 0.5f;
                    v57 = v56 * 0.5f;
                    a3->x = (float)(v55 + v31);
                    a3->y = (float)(v33 * 0.5 + v32);
                    a3->z = v57 + v54;
                    a2->x = (float)(v55 - v31);
                    a2->y = (float)(v33 * 0.5 - v32);
                    a2->z = v57 - v54;
                    LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
                    if ((v6->i1 & 1) != 0)
                    {
                        v38 = v8->i9 * v8->i8;
                        v34 = LIGHT_ul_MulColor(sd->iii1, v38);
                        v6 = sda;
                    }
                    else
                    {
                        v34 = sd->iii1;
                    }
                    *(uint32_t *)&(a2[1].y) = v34;
                    ++v39;
                    v63 = sd->i6 * TIM_gf_dt;
                    v35 = sd->i5 * TIM_gf_dt;
                    v36 = TIM_gf_dt * sd->i4 + v8->i5;
                    ++v6;
                    v8->i5 = (float)v36;
                    sda = v6;
                    v8->i6 = (float)(v35 + v8->i6);
                    a3 += a4;
                    a2 += a4;
                    v8->i7 = v63 + v8->i7;
                    v37 = sd->i10;
                    v8->i5 = (float)(v37 * v8->i5);
                    v8->i6 = (float)(v37 * v8->i6);
                    v8->i7 = (float)(v37 * v8->i7);
                    v8++;
                    continue;
                }
                v11 = sd->actNum - 1;
                sd->actNum = v11;
                v6 = sda;
                memcpy(sda, &sd->ptr[v11], sizeof(tGfxSparkElemData));
            }
        } while (v39 < sd->actNum);
    }
}

void GFX_AllocSpark(tGfxSparkData* a1)
{
    int v1;
    tGfxSparkElemData* v2;
    int v3;
    int v4;
    size_t v5;

    v1 = a1->num;
    if (v1)
    {
        v5 = sizeof(tGfxSparkElemData) * v1;
        if (a1->allocdNum)
            v2 = (tGfxSparkElemData*)MEM_p_Realloc(a1->ptr, v5);
        else
            v2 = (tGfxSparkElemData*)MEM_p_Alloc(v5);
        v3 = a1->actNum;
        a1->ptr = v2;
        v4 = a1->num;
        a1->allocdNum = v4;
        if (v3 > v4)
            a1->actNum = v4;
    }
    else
    {
        a1->allocdNum = 0;
        MEM_Free(a1->ptr);
    }
}

int GFX_i_Spark_Render(struct sGfxSparkData* a1)
{
    int v1;
    int v2;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    tGEOGRO* v9;
    Vec3* v10;
    tGeoTriangleChunkElem* v11;
    int* v12;
    int v13;
    Vec3* v14;
    int v15;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    Vec3* v23; 
    int v24; 
    double v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    unsigned __int16 v33;
    unsigned __int16 v34;
    int v35; 
    int v36; 
    Vec3 a1a;
    float v38; 
    float v39; 

    if (!a1)
        return 1;
    v1 = a1->allocdNum;
    v2 = a1->num;
    a1->flags |= 0x20u;
    if (v1 != v2)
        GFX_AllocSpark(a1);
    if (!a1->allocdNum)
        return 1;
    v5 = 4 - ((a1->flags & 0x20) != 0);
    if ((a1->flags & 4) != 0)
    {
        v4 = 4 - ((a1->flags & 0x20) != 0);
        GFX_NeedGeom(v4 * a1->actNum, 4u, a1->actNum * (v4 - 2), 1);
        GFX_Spark_Update(a1, GFX_gpst_Geo->vertices, GFX_gpst_Geo->vertices + 2, v5);
        v8 = a1->actNum;
        if (!v8)
            return 0;
    }
    else
    {
        GFX_Spark_Generate(a1);
        v6 = a1->actNum;
        if (v6 >= a1->num)
        {
            v7 = a1->flags;
            if ((v7 & 8) != 0)
                a1->flags = v7 | 4;
        }
        GFX_NeedGeom(v5 * v6, 4u, v6 * (v5 - 2), 1);
        GFX_Spark_Update(a1, GFX_gpst_Geo->vertices, GFX_gpst_Geo->vertices + 2, v5);
        v8 = a1->actNum;
        if (!v8)
            return 1;
    }
    if (GFX_gpst_Current->aiGameObject->distance)
    {
        if ((a1->flags & 0x20) != 0)
        {
            GFX_NeedGeom(3 * v8, 3u, v8, 1);
            GFX_gpst_Geo->UVs[0].u = 1.0;
            GFX_gpst_Geo->UVs[0].v = 0.0;
            GFX_gpst_Geo->UVs[1].u = 0.0;
            GFX_gpst_Geo->UVs[1].v = 0.0;
            GFX_gpst_Geo->UVs[2].u = 0.5;
            GFX_gpst_Geo->UVs[2].v = 1.0;
        }
        else
        {
            GFX_NeedGeom(4 * v8, 4u, 2 * v8, 1);
            GFX_gpst_Geo->UVs[0].u = 0.0;
            GFX_gpst_Geo->UVs[0].v = 0.0;
            GFX_gpst_Geo->UVs[1].u = 1.0;
            GFX_gpst_Geo->UVs[1].v = 0.0;
            GFX_gpst_Geo->UVs[2].u = 1.0;
            GFX_gpst_Geo->UVs[2].v = 1.0;
            GFX_gpst_Geo->UVs[3].u = 0.0;
            GFX_gpst_Geo->UVs[3].v = 1.0;
        }
        v9 = GFX_gpst_Geo;
        v10 = GFX_gpst_Geo->vertices;
        v11 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        v12 = GFX_gpst_Geo->vertexColors + 1;
        v13 = a1->actNum;
        v36 = 0;
        if ((a1->flags & 0x20) != 0)
        {
            if (v13 > 0)
            {
                v14 = v10;
                do
                {
                    v15 = *(uint32_t *)&(v14[1].y); // a Generate ebbe generál bele egy színt
                    v12[0] = v15;
                    v12[1] = v15;
                    v12[2] = v15;
                    v17 = v14[2].x - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
                    v12 += 3;
                    v18 = v14[2].y - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
                    v19 = v14[2].z - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
                    v38 = v14[2].x - v10->x;
                    v39 = v14[2].y - v14[0].y;
                    v20 = v14[2].z - v14[0].z;
                    a1a.x = (float)(v39 * v19 - v20 * v18);
                    a1a.y = (float)(v20 * v17 - v19 * v38);
                    a1a.z = (float)(v18 * v38 - v39 * v17);
                    MATH_NormalizeEqualVector(&a1a);
                    v21 = v14[1].x * 0.5;
                    v22 = a1a.x * v21;
                    a1a.y = (float)(a1a.y * v21);
                    a1a.z = (float)(a1a.z * v21);
                    v14[1].x = (float)(v10->x - v22);
                    v14[1].y = v14[0].y - a1a.y;
                    v14[1].z = v14[0].z - a1a.z;
                    v10->x = (float)(v22 + v10->x);
                    v14[0].y = a1a.y + v14[0].y;
                    v14[0].z = a1a.z + v14[0].z;
                    ++v11;
                    v10 += 3;
                    v14 += 3;
                    v11[-1].v1 = 3 * v36;
                    v11[-1].v3 = 3 * v36 + 2;
                    v11[-1].v2 = 3 * v36 + 1;
                    v11[-1].uv1 = 0;
                    v11[-1].uv2 = 1;
                    v11[-1].uv3 = 2;
                    ++v36;
                } while (v36 < a1->actNum);
            LABEL_25:
                v9 = GFX_gpst_Geo;
                goto LABEL_26;
            }
        }
        else if (v13 > 0)
        {
            v23 = v10; // (int)&v10[2].y;
            do
            {
                v24 = *(uint32_t *)&(v23[1].y); // a Generate ebbe generál bele egy színt
                v12[0] = v24;
                v12[1] = v24;
                v12[2] = v24;
                v12[3] = v24;
                v26 = v23[2].x - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
                v27 = v23[2].y - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
                v28 = v23[2].z - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
                v12 += 4;
                v38 = v23[2].x - v10->x;
                v39 = v23[2].y - v23[0].y;
                v29 = v23[2].z - v23[0].z;
                a1a.x = (float)(v39 * v28 - v29 * v27);
                a1a.y = (float)(v29 * v26 - v28 * v38);
                a1a.z = (float)(v27 * v38 - v39 * v26);
                MATH_NormalizeEqualVector(&a1a);
                v30 = v23[1].x * 0.5; // a Generate ebbe is generál vmit
                v31 = a1a.x * v30;
                a1a.y = (float)(a1a.y * v30);
                a1a.z = (float)(a1a.z * v30);
                v23[3].x = (float)(v10->x - v31);
                v23[3].y = v23[0].y - a1a.y;
                v23[3].z = v23[0].z - a1a.z;
                v32 = v31 + v10->x;
                v33 = 4 * v36;
                v34 = 4 * v36 + 1;
                v10[0].x = (float)v32;
                v10 += 4;
                ++v36;
                v23[0].y = a1a.y + v23[0].y;
                v23[0].z = a1a.z + v23[0].z;
                v23[1].x = (float)(v31 + v23[2].x);
                v23[1].y = a1a.y + v23[2].y;
                v23[1].z = a1a.z + v23[2].z;
                v23[2].x = (float)(v23[2].x - v31);
                v23[2].y = v23[2].y - a1a.y;
                v23[2].z = v23[2].z - a1a.z;
                v23 += 4;
                v11[0].v2 = v34;
                v11[0].v3 = v33 + 2;
                v11[1].v1 = v33 + 2;
                v11[0].v1 = v33;
                v11[1].v3 = v33;
                v11[0].uv1 = 0;
                v11[0].uv2 = 1;
                v11[0].uv3 = 2;
                v11[1].v2 = v33 + 3;
                v11[1].uv1 = 2;
                v11[1].uv2 = 3;
                v11[1].uv3 = 0;
                v11 += 2;
            } while (v36 < a1->actNum);
            goto LABEL_25;
        }
    LABEL_26:
        v9->triangleChunks->materialNum = 0;
        v35 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v35 & ~0x8000900u;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v35;
    }
    return 1;
}