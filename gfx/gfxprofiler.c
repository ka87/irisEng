#include "../memory.h"
#include "../time.h"
#include "gfxprofiler.h"

struct sGfxProfilerData* GFX_Profiler_Create()
{
    tGfxProfilerData* v1;

    v1 = (tGfxProfilerData*)MEM_p_Alloc(sizeof(tGfxProfilerData));
    memset(v1, 0, sizeof(tGfxProfilerData));
    return v1;
}

void GFX_Profiler_Destroy(struct sGfxProfilerData* a1)
{
    if (a1)
    {
        if (a1->ptr1)
            MEM_Free(a1->ptr1);
        if (a1->ptr4)
            MEM_Free(a1->ptr4);
        if (a1->ptr3)
            MEM_Free(a1->ptr3);
        if (a1->ptr2)
            MEM_Free(a1->ptr2);
    }
}

void GFX_i_Profiler_StartStop(tGfxProfilerData* a1, unsigned int a2)
{
    if (a1->uu)
    {
        a1->ptr3[a1->a1] = a2;
        if (!a2 && ++a1->a1 == a1->num)
            a1->a1 = 0;
    }
}

void GFX_i_Profiler_UpdateMecanic(tGfxProfilerData* a1, float a2)
{
    double Y;
    Vec3 v3;
    Vec3* v4;
    Vec3 a3;
    tGfxProfilerPtr2Data* v6;
    float v7;
    unsigned int* v8;
    tGfxProfilerPtr4Data* v9; 
    float v10;
    float v11;
    float v12;
    unsigned int* v13;
    tGfxProfilerPtr4Data* a1a;

    if (a1->uu)
    {
        v13 = a1->ptr3;
        v8 = &v13[a1->num];
        Y = a2 / 0.016666668;
        v7 = (float)Y;
        v10 = (float)pow(1.0 - a1->v1.x, Y);
        v11 = (float)pow(1.0 - a1->v1.y, v7);
        v12 = (float)pow(1.0 - a1->v1.z, v7);
        a1a = a1->ptr4;
        while (v13 < v8)
        {
            if (*v13)
            {
                v9 = &a1a[a1->PPPNum];
                v6 = a1->ptr2;
                while (a1a < v9)
                {
                    a1a->v2.z = a2 * a1->f0 + a1a->v2.z;
                    v4 = &v6->v4;
                    a3.x = a2 * v6->v4.x;
                    a3.y = v6->v4.y * a2;
                    a3.z = v6->v4.z * a2;
                    MATH_VecAddVec(&a1a->v2, &a1a->v2, &a3);
                    v3.x = a2 * a1a->v2.x;
                    v3.y = a1a->v2.y * a2;
                    v3.z = a1a->v2.z * a2;
                    MATH_VecAddVec(&a1a->v1, &a1a->v1, &v3);
                    if (a1a->v1.z < (double)a1a->fff1)
                        a1a->v1.z = a1a->fff1;
                    a1a->v2.x = v10 * a1a->v2.x;
                    a1a->v2.y = v11 * a1a->v2.y;
                    a1a->v2.z = v12 * a1a->v2.z;
                    if ((a1a->xx & 0xFC000000) != 0)
                        a1a->xx -= 0x2000000;
                    ++a1a;
                    ++v6;
                }
            }
            else
            {
                a1a += a1->PPPNum;
            }
            ++v13;
        }
    }
}

void GFX_i_Profiler_ComputeMeshState(tGfxProfilerData* a1)
{
    unsigned int* v1;
    int v2;
    int pointsNum;
    unsigned int* v4;
    int v5;

    if (a1->uu)
    {
        v4 = a1->ptr3;
        v1 = &v4[a1->num];
        v2 = *(v1 - 1);
        v5 = 0;
        pointsNum = 0;
        while (v4 < v1)
        {
            if (*v4)
            {
                *v4 = (2 * pointsNum) | 1;
                pointsNum += a1->PPPNum;
                if (v2)
                    ++v5;
            }
            v2 = *v4++;
        }
        a1->triangleNum = 2 * a1->LPPNum * v5;
        a1->pointsNum = pointsNum;
        a1->UVNum = (a1->xx + 1) * 2 * a1->LPPNum;
    }
}

void GFX_i_Profiler_Update_With_XYT(tGfxProfilerData* pd)
{
    int v1;
    Vec3 dest;
    Vec3* v4; 
    Vec3* v5; 
    Vec3 v6; 
    Vec3* v7; 
    Vec3 v8; 
    Vec3* v9; 
    Vec3 v10; 
    float v11; 
    float v12; 
    float v13; 
    Vec3* v14; 
    Vec3* v15; 
    Vec3 v16; 
    Vec3* v17; 
    Vec3 v18; 
    Vec3* v19;
    Vec3 a3; 
    float v21;
    float v22;
    float v23;
    tGfxProfilerPtr4Data* v24;
    tGfxProfilerPtr4Data* v25;
    int v26;
    int v27;
    float v28;
    Vec3 v29;
    tGfxProfilerPtr4Data* v30;
    tGfxProfilerPtr4Data* a2; 
    Vec3 v32;
    float v33;
    float v34;
    float v35;
    tGfxProfilerPtr4Data* v36;
    tGfxProfilerPtr2Data* v37;
    tGfxProfilerPtr4Data* a1;
    int v39;

    if (pd->uu && pd->ptr3[pd->a1])
    {
        v39 = pd->a1 + 1;
        if (v39 == pd->num)
            v39 = 0;
        pd->ptr3[v39] = pd->ptr3[pd->a1];
        pd->a1 = v39;
        if (pd->xxxxx)
        {
            if (pd->a1)
                v1 = pd->a1;
            else
                v1 = pd->num;
            pd->a1 = v1 - 1;
        }
        a1 = &pd->ptr4[pd->PPPNum * pd->a1];
        v36 = &a1[pd->PPPNum];
        v37 = pd->ptr2;
        while (a1 < v36)
        {
            a1->v1 = pd->v5;
            v33 = v37->v0.x;
            v34 = v37->v0.y;
            v35 = v37->v0.z;
            v23 = (float)((double)rand() / 32767.0);
            v33 = (float)(((1.0 - 0.0) * v23 + 0.0) * v37->v2.x + v33);
            v22 = (float)((double)rand() / 32767.0);
            v34 = (float)(((1.0 - 0.0) * v22 + 0.0) * v37->v2.y + v34);
            v21 = (float)((double)rand() / 32767.0);
            v35 = (float)(((1.0 - 0.0) * v21 + 0.0) * v37->v2.z + v35);
            v19 = &pd->v2;
            a3.x = v33 * pd->v2.x;
            a3.y = pd->v2.y * v33;
            a3.z = pd->v2.z * v33;
            MATH_VecAddVec(&a1->v1, &a1->v1, &a3);
            v17 = &pd->v3;
            v18.x = v34 * pd->v3.x;
            v18.y = pd->v3.y * v34;
            v18.z = pd->v3.z * v34;
            MATH_VecAddVec(&a1->v1, &a1->v1, &v18);
            v15 = &pd->v4;
            v16.x = v35 * pd->v4.x;
            v16.y = pd->v4.y * v35;
            v16.z = pd->v4.z * v35;
            MATH_VecAddVec(&a1->v1, &a1->v1, &v16);
            v14 = &a1->v2;
            a1->v2.x = 0.0;
            v14->y = 0.0;
            v14->z = 0.0;
            v33 = v37->v1.x;
            v34 = v37->v1.y;
            v35 = v37->v1.z;
            v13 = (float)((double)rand() / 32767.0);
            v33 = (float)(((1.0 - 0.0) * v13 + 0.0) * v37->v3.x + v33);
            v12 = (float)((double)rand() / 32767.0);
            v34 = (float)(((1.0 - 0.0) * v12 + 0.0) * v37->v3.y + v34);
            v11 = (float)((double)rand() / 32767.0);
            v35 = (float)(((1.0 - 0.0) * v11 + 0.0) * v37->v3.z + v35);
            v9 = &pd->v2;
            v10.x = v33 * pd->v2.x;
            v10.y = pd->v2.y * v33;
            v10.z = pd->v2.z * v33;
            MATH_VecAddVec(&a1->v2, &a1->v2, &v10);
            v7 = &pd->v3;
            v8.x = v34 * pd->v3.x;
            v8.y = pd->v3.y * v34;
            v8.z = pd->v3.z * v34;
            MATH_VecAddVec(&a1->v2, &a1->v2, &v8);
            v5 = &pd->v4;
            v6.x = v35 * pd->v4.x;
            v6.y = pd->v4.y * v35;
            v6.z = pd->v4.z * v35;
            MATH_VecAddVec(&a1->v2, &a1->v2, &v6);
            v4 = &a1->v3;
            a1->v3.x = 0.0;
            v4->y = 0.0;
            v4->z = 1.0;
            a1->xx = v37->i2 & ~0x1F000000u;
            a1->fff1 = v37->f3;
            ++a1;
            ++v37;
        }
        pd->xxxxx = 0;
        if (pd->a1 && pd->ptr3[pd->a1 - 1])
        {
            a2 = &pd->ptr4[pd->PPPNum * pd->a1];
            MATH_VecAddVec(&v29, &a2->v1, &a2[pd->PPPNum - 1].v1);
            if (pd->a1)
                v30 = &pd->ptr4[pd->PPPNum * (pd->a1 - 1)];
            else
                v30 = &pd->ptr4[pd->PPPNum * (pd->num - 1)];
            MATH_VecAddVec(&v32, &v30->v1, &v30[pd->PPPNum - 1].v1);
            MATH_SubstractVec(&dest, &v29, &v32);
            v28 = MATH_VecLen(&dest) * 0.5f;
            if (v28 >= (double)pd->f1)
            {
                v26 = pd->a1;
                v27 = 9;
                while (v27--)
                {
                    if (v26 < pd->num - 1)
                        ++v26;
                    else
                        v26 = 0;
                    v25 = &pd->ptr4[pd->PPPNum * v26];
                    v24 = &v25[pd->PPPNum];
                    while (v25 < v24)
                    {
                        if ((v25->xx & 0xE0000000) != 0)
                            v25->xx -= 0x20000000;
                        ++v25;
                    }
                }
            }
            else
            {
                pd->xxxxx = 1;
            }
        }
        v39 = pd->a1 + 1;
        if (v39 == pd->num)
            v39 = 0;
        pd->ptr3[v39] = 0;
    }
}

void GFX_Profiler_Validate(tGfxProfilerData* a1)
{
    if (!a1->uu && a1->PPPNum && a1->LPPNum)
    {
        if (a1->num)
        {
            a1->ptr1 = (tGfxProfilerPtr1Data*)MEM_p_Alloc(sizeof(tGfxProfilerPtr1Data) * a1->LPPNum);
            memset(a1->ptr1, 0, sizeof(tGfxProfilerPtr1Data) * a1->LPPNum);
            a1->ptr2 = (tGfxProfilerPtr2Data*)MEM_p_Alloc(sizeof(tGfxProfilerPtr2Data) * a1->PPPNum);
            memset(a1->ptr2, 0, sizeof(tGfxProfilerPtr2Data) * a1->PPPNum);
            a1->ptr3 = (unsigned int*)MEM_p_Alloc(4 * a1->num);
            memset(a1->ptr3, 0, 4 * a1->num);
            a1->ptr4 = (tGfxProfilerPtr4Data*)MEM_p_Alloc(a1->num * sizeof(tGfxProfilerPtr4Data) * a1->PPPNum);
            memset(a1->ptr4, 0, a1->num * sizeof(tGfxProfilerPtr4Data) * a1->PPPNum);
            a1->uu = 1;
            a1->xx = 4;
        }
    }
}

int GFX_i_Profiler_Render(struct sGfxProfilerData* a1)
{
    unsigned int v3;
    tGeoTriangleChunkElem* v4;
    tGeoTriangleChunkElem* v5;
    unsigned int* v6;
    tGfxProfilerPtr1Data* i;
    int v8;
    unsigned int* v9;
    unsigned int v10;
    tGfxProfilerPtr1Data* v11;
    float v12;
    tGfxProfilerPtr1Data* v13;
    unsigned int v14;
    float v15;
    tGfxProfilerPtr1Data* v16;
    tGeoUVElem* v17;
    unsigned int* v18;
    int* v19;
    tGfxProfilerPtr4Data* v20;
    Vec3* v21;
    unsigned int* v22;
    Vec3* v23;
    tGfxProfilerPtr4Data* v24;
    int v25;

    if (!a1->uu)
        return 1;
    GFX_i_Profiler_UpdateMecanic(a1, TIM_gf_dt);
    GFX_i_Profiler_Update_With_XYT(a1);
    GFX_i_Profiler_ComputeMeshState(a1);
    if (!a1->pointsNum)
        return 1;
    if (!a1->triangleNum)
        return 1;
    GFX_NeedGeom(a1->pointsNum, a1->UVNum, a1->triangleNum, 1);
    v24 = a1->ptr4;
    v19 = GFX_gpst_Geo->vertexColors + 1;
    v21 = GFX_gpst_Geo->vertices;
    v23 = GFX_gpst_Geo->normals;
    v22 = a1->ptr3;
    v18 = &v22[a1->num];
    while (v22 < v18)
    {
        if (*v22)
        {
            v20 = &v24[a1->PPPNum];
            while (v24 < v20)
            {
                v21->x = v24->v1.x;
                v21->y = v24->v1.y;
                v21->z = v24->v1.z;
                ++v21;
                v23->x = v24->v3.x;
                v23->y = v24->v3.y;
                v23->z = v24->v3.z;
                ++v23;
                *v19++ = v24->xx;
                ++v24;
            }
        }
        else
        {
            v24 += a1->PPPNum;
        }
        ++v22;
    }
    v14 = a1->xx + 1;
    v13 = a1->ptr1;
    v16 = &v13[a1->LPPNum];
    v12 = (float)(1.0 / (double)a1->xx);
    v15 = 1.0;
    v17 = GFX_gpst_Geo->UVs;
    while (v14--)
    {
        while (v13 < v16)
        {
            v17->u = v15;
            v17[1].u = v15;
            v17->v = v13->f1;
            v17[1].v = v13->f2;
            v17 += 2;
            ++v13;
        }
        v13 = a1->ptr1;
        v15 = v15 - v12;
    }
    v4 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    v9 = a1->ptr3;
    v6 = &v9[a1->num];
    v10 = *(v6 - 1);
    v11 = &a1->ptr1[a1->LPPNum];
    v8 = 0;
    while (v9 < v6)
    {
        if ((v10 & *v9) != 0)
        {
            for (i = a1->ptr1; i < v11; ++i)
            {
                v3 = *v9 >> 1;
                v4->uv3 = 0;
                v4->uv2 = 0;
                v4->uv1 = 0;
                v4->v1 = i->s1 + v3;
                v4->v2 = i->s2 + v3;
                v4->v3 = i->s2 + (v10 >> 1);
                v4->uv1 = v8 + 2 * a1->LPPNum;
                v4->uv2 = v8 + 2 * a1->LPPNum + 1;
                v4->uv3 = v8 + 1;
                v5 = v4 + 1;
                v5->uv3 = 0;
                v5->uv2 = 0;
                v5->uv1 = 0;
                v5->v1 = i->s2 + (v10 >> 1);
                v5->v2 = i->s1 + (v10 >> 1);
                v5->v3 = i->s1 + v3;
                v5->uv1 = v8 + 1;
                v5->uv2 = v8;
                v5->uv3 = v8 + 2 * a1->LPPNum;
                v4 = v5 + 1;
                v8 += 2;
            }
        }
        else
        {
            v8 += 2 * a1->LPPNum;
        }
        if (v8 == 2 * a1->LPPNum * a1->xx)
            v8 = 0;
        v10 = *v9++;
    }
    v25 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v25 & ~0x900u;
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    if (GFX_gpst_Current->aiGameObject->distance)
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v25;
    return 1;
}

void GFX_SetNumberOfPPP(tGfxProfilerData* a1, int a2)
{
    a1->PPPNum = a2;
    GFX_Profiler_Validate(a1);
}

void GFX_SetNumberOfLPP(tGfxProfilerData* a1, int a2)
{
    a1->LPPNum = a2;
    GFX_Profiler_Validate(a1);
}

void GFX_SetNumberOfP(tGfxProfilerData* a1, unsigned int a2)
{
    a1->num = a2;
    a1->num2 = 255 / a2;
    if (!a1->num2)
        a1->num2 = 1;
    if (a1->num2 >= 127u)
        a1->num2 = 127;
    a1->num2 <<= 24;
    GFX_Profiler_Validate(a1);
}

void GFX_Profiler_Seti(struct sGfxProfilerData* a1, int a2, int a3)
{
    switch (a2)
    {
    case 12100:
        GFX_SetNumberOfPPP(a1, a3);
        GFX_Profiler_Validate(a1);
        break;
    case 12101:
        GFX_SetNumberOfLPP(a1, a3);
        GFX_Profiler_Validate(a1);
        break;
    case 12102:
        GFX_SetNumberOfP(a1, a3);
        GFX_Profiler_Validate(a1);
        break;
    case 12103:
        GFX_i_Profiler_StartStop(a1, a3);
        break;
    case 12104:
        a1->ptr2CurrID = a3;
        break;
    case 12105:
        a1->ptr1CurrID = a3;
        break;
    case 12106:
        a1->ptr1[a1->ptr1CurrID].s1 = a3;
        break;
    case 12107:
        a1->ptr1[a1->ptr1CurrID].s2 = a3;
        break;
    case 12108:
        a1->ptr1[a1->ptr1CurrID].s3 = a3;
        break;
    case 12109:
        a1->ptr2[a1->ptr2CurrID].i2 = a3;
        break;
    case 12110:
        a1->ptr2[a1->ptr2CurrID].i3 = a3;
        break;
    case 12111:
        a1->xx = a3;
        break;
    default:
        return;
    }
}

void GFX_Profiler_Setf(struct sGfxProfilerData* a1, int a2, float a3)
{
    switch (a2)
    {
    case 12000:
        a1->f0 = a3;
        break;
    case 12001:
        a1->ptr1[a1->ptr1CurrID].f1 = a3;
        break;
    case 12002:
        a1->ptr1[a1->ptr1CurrID].f2 = a3;
        break;
    case 12003:
        a1->ptr2[a1->ptr2CurrID].f3 = a3;
        break;
    case 12004:
        a1->f1 = a3;
        break;
    default:
        return;
    }
}

void GFX_Profiler_Setv(struct sGfxProfilerData* a1, int a2, Vec3* a3)
{
    switch (a2)
    {
    case 12200:
        a1->ptr2[a1->ptr2CurrID].v0 = *a3;
        break;
    case 12201:
        a1->ptr2[a1->ptr2CurrID].v1 = *a3;
        break;
    case 12202:
        a1->v1 = *a3;
        break;
    case 12203:
        a1->v2 = *a3;
        break;
    case 12204:
        a1->v3 = *a3;
        break;
    case 12205:
        a1->v4 = *a3;
        break;
    case 12206:
        a1->v5 = *a3;
        break;
    case 12207:
        a1->ptr2[a1->ptr2CurrID].v2 = *a3;
        break;
    case 12208:
        a1->ptr2[a1->ptr2CurrID].v3 = *a3;
        break;
    default:
        return;
    }
}