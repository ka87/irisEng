#include "../memory.h"
#include "../time.h"
#include "gfxsmoke.h"

void GFX_Smoke_Update(tGfxSmokeData* a1, float a2, int a3)
{
    tGfxSmokeInnerData* v3;
    tGfxSmokeInnerData* v4;
    double v5;
    tGfxSmokeInnerData* v6;
    int v7;
    int i;
    double v9;
    double v10;
    float v11;

    v3 = a1->ptr;
    v4 = &v3[a1->num];
    while (v3 < v4)
    {
        v5 = a2 + v3->i7;
        v3->i7 = (float)v5;
        if (v5 < a1->i1)
        {
            for (i = a3; i; v3->i6 = (float)(v10 * v3->i6))
            {
                --i;
                v9 = v3->i5 * 0.016666668;
                v11 = (float)(v3->i6 * 0.016666668);
                v3->i1 = (float)(v3->i4 * 0.016666668 + v3->i1);
                v3->i2 = (float)(v9 + v3->i2);
                v3->i3 = v11 + v3->i3;
                v10 = a1->i2;
                v3->i4 = (float)(v10 * v3->i4);
                v3->i5 = (float)(v10 * v3->i5);
            }
            ++v3;
        }
        else
        {
            v6 = a1->ptr;
            v7 = a1->num - 1;
            a1->num = v7;
            memcpy(v3, &v6[v7], sizeof(tGfxSmokeInnerData));
            --v4;
        }
    }
}

int GFX_i_Smoke_Render(struct sGfxSmokeData* a1)
{
    double v1;
    unsigned int v2;
    int v3;
    tGEOGRO* v4;
    tGfxSmokeInnerData* v5;
    tGeoTriangleChunkElem* v6;
    int* v7;
    unsigned int v9;
    double v10;
    double v11;
    double v12;
    double v13;
    int* v14;
    double v15;
    int v16;
    int v17;
    float v19;
    float v20;
    Vec3* v21;
    float v22;
    float v23;
    float v24;
    Mtx4x4* v25;
    Vec3* v26;
    tGfxSmokeInnerData* v27;
    float v28;
    float v29;
    float v30;
    float v31;

    v1 = TIM_gf_dt + a1->timeElapsed;
    v2 = (unsigned int)(60.0 * v1);
    v20 = (float)((double)(int)v2 * 0.016666668);
    a1->timeElapsed = (float)(v1 - v20);
    if (v2)
        GFX_Smoke_Update(a1, v20, v2);
    if (GFX_gpst_Current->aiGameObject->distance)
    {
        v3 = a1->num;
        if (v3)
        {
            GFX_NeedGeom(4 * v3, 4u, 2 * v3, 1);
            GFX_gpst_Geo->UVs->u = 0.0;
            GFX_gpst_Geo->UVs->v = 0.0;
            GFX_gpst_Geo->UVs[1].u = 1.0;
            GFX_gpst_Geo->UVs[1].v = 0.0;
            GFX_gpst_Geo->UVs[2].u = 1.0;
            GFX_gpst_Geo->UVs[2].v = 1.0;
            GFX_gpst_Geo->UVs[3].u = 0.0;
            GFX_gpst_Geo->UVs[3].v = 1.0;
            v4 = GFX_gpst_Geo;
            v5 = a1->ptr;
            v25 = &gpst_DX_stDD->ct.cam.objectMtx;
            v6 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
            v26 = &gpst_DX_stDD->ct.cam.objectMtx.v2;
            v21 = GFX_gpst_Geo->vertices;
            v7 = GFX_gpst_Geo->vertexColors + 1;
            v27 = &v5[a1->num];
            if (a1->i1 == 0.0)
                v22 = 0.0;
            else
                v22 = (float)(1.0 / a1->i1);
            if (v5 < &v5[a1->num])
            {
                do
                {
                    v23 = v22 * v5->i7;
                    v19 = (a1->i4 - a1->i3) * v23 + a1->i3;
                    v9 = LIGHT_ul_Interpol2Colors(a1->color1, a1->color2, v23);
                    v10 = v19 * v25->v1.x;
                    v11 = v19 * v25->v1.y;
                    v28 = v19 * v25->v1.z;
                    v12 = v19 * v26->x;
                    v13 = v19 * v26->y;
                    v29 = v19 * v26->z;
                    v30 = (float)(v28 * 0.5);
                    v21->x = (float)(v5->i1 - v10 * 0.5);
                    v21->y = (float)(v5->i2 - v11 * 0.5);
                    v21->z = v5->i3 - v30;
                    v31 = (float)(v29 * 0.5);
                    v24 = (float)(v12 * 0.5 + v21->x);
                    v21->x = v24;
                    v14 = (v7 + 1);
                    v15 = v13 * 0.5 + v21->y;
                    v6 += 2;
                    v21->y = (float)v15;
                    v21->z = v31 + v21->z;
                    v21[1].x = (float)(v24 + v10);
                    v21[1].y = (float)(v11 + v21->y);
                    v21[1].z = v28 + v21->z;
                    v21[2].x = (float)(v21[1].x - v12);
                    v21[2].y = (float)(v21[1].y - v13);
                    v21[2].z = v21[1].z - v29;
                    v21[3].x = (float)(v21[2].x - v10);
                    v21[3].y = (float)(v21[2].y - v11);
                    v21[3].z = v21[2].z - v28;
                    v16 = v21 - GFX_gpst_Geo->vertices;
                    v6[-2].v1 = v16;
                    v6[-2].uv1 = 0;
                    v6[-2].uv2 = 1;
                    v6[-2].uv3 = 2;
                    v21 += 4;
                    v6[-2].v2 = v16 + 1;
                    v6[-2].v3 = v16 + 2;
                    v6[-1].v1 = v16 + 2;
                    v6[-1].v2 = v16 + 3;
                    v6[-1].v3 = v16;
                    v6[-1].uv1 = 2;
                    v6[-1].uv2 = 3;
                    v6[-1].uv3 = 0;
                    *(v14 - 1) = v9;
                    *(v14++) = v9;
                    *(v14++) = v9;
                    *v14 = v9;
                    ++v5;
                    v7 = (v14 + 1);
                } while (v5 < v27);
                v4 = GFX_gpst_Geo;
            }
            v4->triangleChunks->materialNum = 0;
            v17 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent = v17 & ~0x8000900u;
            GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
            gpst_DX_stDD->drawMaskAllowedStuffCurrent = v17;
        }
    }
    return 1;
}

struct sGfxSmokeData* GFX_Smoke_Create()
{
    tGfxSmokeData* v0;

    v0 = (tGfxSmokeData*)MEM_p_Alloc(sizeof(tGfxSmokeData));
    memset(v0, 0, sizeof(tGfxSmokeData));
    v0->i4 = 2.0f;
    v0->i1 = 2.0f;
    v0->i2 = 0.89999998f;
    v0->i3 = 0.5f;
    v0->color1 = 0xFFFFFFFF;
    v0->color2 = 0;
    return v0;
}

void GFX_Smoke_Destroy(struct sGfxSmokeData* a1)
{
    if (a1)
    {
        if (a1->innerDataNum)
        {
            a1->innerDataNum = 0;
            MEM_Free(a1->ptr);
        }
    }
}

void GFX_Smoke_Alloc(struct sGfxSmokeData* a1, int num)
{
    int v2;
    tGfxSmokeInnerData* v3;
    size_t v4;

    if (num > 0)
    {
        v2 = a1->innerDataNum;
        if (num != v2)
        {
            v4 = sizeof(tGfxSmokeInnerData) * num;
            if (v2)
            {
                v3 = (tGfxSmokeInnerData*)MEM_p_Realloc(a1->ptr, v4);
                a1->innerDataNum = num;
                a1->ptr = v3;
            }
            else
            {
                a1->ptr = (tGfxSmokeInnerData*)MEM_p_Alloc(v4);
                a1->innerDataNum = num;
            }
        }
    }
    else if (a1->innerDataNum)
    {
        a1->innerDataNum = 0;
        MEM_Free(a1->ptr);
    }
}

void GFX_Smoke_AddOne(tGfxSmokeData* a1)
{
    int v1;
    tGfxSmokeInnerData* v2;

    v1 = a1->num;
    if (v1 < a1->innerDataNum)
    {
        v2 = &a1->ptr[v1];
        a1->num = v1 + 1;
        v2->i7 = 0.0;
        v2->i1 = a1->innerData1;
        v2->i2 = a1->innerData2;
        v2->i3 = a1->innerData3;
        v2->i4 = a1->innerData4;
        v2->i5 = a1->innerData5;
        v2->i6 = a1->innerData6;
    }
}

void GFX_Smoke_Seti(struct sGfxSmokeData* a1, int a2, int num)
{
    if (a1)
    {
        if (a2 == 6100)
        {
            GFX_Smoke_Alloc(a1, num);
        }
        else if (a2 == 6103)
        {
            GFX_Smoke_AddOne(a1);
        }
    }
}