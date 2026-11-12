#include "../memory.h"
#include "../time.h"
#include "gfxwater2.h"

struct sGfxWater2Data* GFX_Water2_Create()
{
    tGfxWater2Data* result;

    result = (tGfxWater2Data*)MEM_p_Alloc(sizeof(tGfxWater2Data));
    result->numToAlloc = 0;
    result->i2 = 0;
    result->num = 0;
    result->ptr = NULL;
    return result;
}

void GFX_Water2_Destroy(struct sGfxWater2Data* a1)
{
    if (a1)
    {
        if (a1->ptr)
            MEM_Free(a1->ptr);
        a1->ptr = NULL;
        a1->num = 0;
    }
}

int GFX_i_Water2_Render(struct sGfxWater2Data* a1)
{
    tGfxWater2Data* v1;
    int v2;
    tGeoTriangleChunkElem* v3;
    int* v4;
    Vec3* v5;
    tGfxWater2InnerData* v6;
    double v7;
    tGfxWater2InnerData* v8;
    unsigned int v9;
    long double v10;
    long double v11;
    double v12;
    double v14;
    tGfxWater2Data* v15;
    int v16;
    int v17;
    int v19;
    float v20;
    int v21;
    float v22;
    Vec3* v23;
    int v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;

    v1 = a1;
    v2 = a1->num;
    if (v2)
    {
        GFX_NeedGeom(3 * v2, 3u, v2, 1);
        GFX_gpst_Geo->UVs->u = 0.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        v3 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        v4 = GFX_gpst_Geo->vertexColors + 1;
        v23 = GFX_gpst_Geo->vertices;
        v24 = 0;
        v19 = 0;
        if (a1->num > 0)
        {
            v21 = 0;
            v5 = GFX_gpst_Geo->vertices;
            while (1)
            {
                v6 = v1->ptr;
                v7 = v6[v21].ff5;
                v8 = &v6[v21];
                v22 = (float)(v7 / v8->ff1);
                v9 = LIGHT_ul_Interpol2Colors(v8->color, 0, v22);
                v8->f1 = TIM_gf_dt * v8->v3.x + v8->f1;
                v8->f2 = TIM_gf_dt * v8->v3.y + v8->f2;
                v10 = -1.0
                    / sqrt(v8->v3.x * v8->v3.x + v8->v3.y * v8->v3.y + v8->v3.z * v8->v3.z)
                    * ((v8->ff3 - v8->ff2) * v22 + v8->ff2);
                v11 = v10 * v8->v3.x;
                v29 = (float)(v8->v3.y * v10);
                v30 = (float)(v8->v3.z * v10);
                v20 = v8->ff4;
                v27 = (float)(MATH_gst_BaseVectorK.y * v11 - MATH_gst_BaseVectorK.x * v29);
                v25 = (MATH_gst_BaseVectorK.z * v29 - MATH_gst_BaseVectorK.y * v30) * v20;
                v26 = (float)((MATH_gst_BaseVectorK.x * v30 - MATH_gst_BaseVectorK.z * v11) * v20);
                v28 = v27 * v20;
                v31 = (float)(v30 * 0.5);
                v23[0].x = (float)(v8->f1 - 0.5 * v11);
                v5[0].y = (float)(v8->f2 - v29 * 0.5);
                v5[0].z = v8->f3 - v31;
                v12 = v25 + v23[0].x;
                v23[0].x = (float)v12;
                v23 += 3;
                v5[0].y = v26 + v5[0].y;
                v5[0].z = v28 + v5[0].z;
                v5[1].x = (float)(v12 + v11);
                v5[1].y = v29 + v5[0].y;
                v5[1].z = v30 + v5[0].z;
                v5[2].x = v5[1].x - v25;
                v5[2].y = v5[1].y - v26;
                v5[2].z = v5[1].z - v28;
                v5 += 3;
                v3->v1 = v24;
                v3->v2 = v24 + 1;
                v3->uv1 = 0;
                v3->uv2 = 1;
                v3->uv3 = 2;
                v3->v3 = v24 + 2;
                v4[0] = v9;
                v4[1] = v9;
                v4[2] = v9;
                v4 += 3;
                v14 = TIM_gf_dt + v8->ff5;
                v8->ff5 = (float)v14;
                ++v3;
                v24 += 3;
                if (v14 <= v8->ff1)
                {
                    ++v19;
                    ++v21;
                }
                else
                {
                    v15 = a1;
                    v16 = a1->num - 1;
                    a1->num = v16;
                    if (v19 >= v16)
                        goto LABEL_10;
                    memcpy(v8, &a1->ptr[v16], sizeof(tGfxWater2InnerData));
                }
                v15 = a1;
            LABEL_10:
                if (v19 >= v15->num)
                    break;
                v1 = a1;
            }
        }
        v17 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v17 & ~0x800u;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v17;
    }
    return 1;
}

void GFX_Water2_Seti(struct sGfxWater2Data* a1, int a2, int offset, int a4)
{
    int v4;

    if (a2 == 10102)
    {
        if (a1->num < a1->numToAlloc)
        {
            if (!a1->ptr)
                a1->ptr = (tGfxWater2InnerData*)MEM_p_Alloc(48 * a1->numToAlloc);
            ++a1->num;
        }
        a1->ptr[a1->num - 1].ff5 = 0.0;
    }
    else
    {
        v4 = a1->num;
        if (v4)
            *(int32_t*)((char*)&a1->ptr[v4 - 1] + offset) = a4;
    }
}

void GFX_Water2_Setf(struct sGfxWater2Data* a1, int a2, int a3, float a4)
{
    int v4;

    v4 = a1->num;
    if (v4)
        *(float*)((char*)&a1->ptr[v4 - 1] + a3) = a4;
}

void GFX_Water2_Setv(struct sGfxWater2Data* a1, int a2, int a3, Vec3* a4)
{
    int v4;

    v4 = a1->num;
    if (v4)
        *(Vec3*)((char*)&a1->ptr[v4 - 1] + a3) = *a4;
}