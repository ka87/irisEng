#include "../memory.h"
#include "../time.h"
#include "gfxtable.h"

int GFX_i_Table_Render(struct sGfxTableData* a1)
{
    int v2;
    int v3;
    tGdi_DisplayData* v4;
    tGEOGRO* v5;
    Vec3* v6;
    tGeoTriangleChunkElem* v7;
    int* vertexcolorPtr;
    double v9;
    int v10;
    double v11;
    double v12;
    double v13;
    int v14;
    int v15;
    tGeoTriangleChunkElem* v16;
    Vec3* v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    Vec3* v23; 
    Vec3* v24; 
    int v25; 
    float* v27;
    float* v28;
    char* v29;
    int v30; 
    float v31;
    float v32;
    int v33;
    Vec3 a2;
    float v35;
    int v36;

    v2 = a1->num2 * a1->num1;
    if (v2)
    {
        v3 = 4 * v2;
        v33 = 4 * v2;
        GFX_NeedGeom(4 * v2, 4u, 2 * v2, 1);
        GFX_gpst_Geo->UVs->u = 0.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 0.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;
        v4 = gpst_DX_stDD;
        v35 = gpst_DX_stDD->ct.cam.height;
        v31 = gpst_DX_stDD->ct.cam.width;
        v5 = GFX_gpst_Geo;
        v6 = GFX_gpst_Geo->vertices;
        v7 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        vertexcolorPtr = GFX_gpst_Geo->vertexColors + 1;
        v9 = a1->ff2 * -v35 + v35;
        v30 = 0;
        if (a1->num1 > 0)
        {
            v10 = a1->num2;
            v11 = -v35;
            v29 = a1->vertexColorIndexes;
            v28 = a1->f;
            do
            {
                v32 = (float)v9;
                v36 = 0;
                v9 = v9 + v11 * *v28;
                v12 = v31 * a1->ff1;
                if (v10 > 0)
                {
                    v27 = a1->f2;
                    do
                    {
                        v13 = v12;
                        v14 = a1->colors[v29[v36]];
                        v12 = v12 + v31 * *v27;
                        v15 = (char*)v6 - (char*)v5->vertices;
                        v7->uv1 = 0;
                        v7->uv2 = 1;
                        v7->uv3 = 2;
                        v15 = (int16_t)v15 / sizeof(Vec3);
                        v7->v1 = v15;
                        v7->v2 = v15 + 1;
                        v7->v3 = v15 + 2;
                        v7[1].v1 = v15 + 2;
                        v7[1].v3 = v15;
                        v7[1].uv1 = 2;
                        v7[1].uv2 = 3;
                        v7[1].uv3 = 0;
                        v16 = v7 + 1;
                        v16->v2 = v15 + 3;
                        v17 = v6 + 1;
                        v18 = a1->ff3;
                        v17[-1].y = v32;
                        v17[-1].x = (float)v13;
                        ++v17;
                        v17[-2].z = (float)v18;
                        ++v17;
                        v19 = a1->ff3;
                        v17[-2].y = v32;
                        v17[-2].x = (float)v12;
                        v17[-2].z = (float)v19;
                        v20 = a1->ff3;
                        ++v27;
                        v17[-1].x = (float)v12;
                        v7 = v16 + 1;
                        v6 = v17 + 1;
                        v6[-2].y = (float)v9;
                        v6[-2].z = (float)v20;
                        ++v36;
                        v21 = v13;
                        v22 = a1->ff3;
                        v6[-1].x = (float)v21;
                        v6[-1].y = (float)v9;
                        v6[-1].z = (float)v22;
                        *(vertexcolorPtr - 0) = v14;
                        *(vertexcolorPtr - 1) = v14;
                        *(vertexcolorPtr - 2) = v14;
                        *(vertexcolorPtr - 3) = v14;
                        vertexcolorPtr += 4;
                        v10 = a1->num2;
                        v5 = GFX_gpst_Geo;
                    } while (v36 < v10);
                }
                ++v30;
                ++v28;
                v29 += 10;
            } while (v30 < a1->num1);
            v4 = gpst_DX_stDD;
            v3 = v33;
        }
        v23 = v5->vertices;
        if (v3 > 0)
        {
            v24 = v23;
            do
            {
                CAM_2Dto3DCamera2(&v4->ct.cam, &a2, v24);
                MATH_TransformVertex(v24, &gpst_DX_stDD->ct.cam.objectMtx, &a2);
                v4 = gpst_DX_stDD;
                ++v24;
                --v3;
            } while (v3);
        }
        v25 = v4->drawMaskAllowedStuffCurrent;
        v4->drawMaskAllowedStuffCurrent = v25 & ~0x8000900u;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v25;
    }
    return 1;
}

void GFX_Table_Setv(struct sGfxTableData* a1, int a2, Vec3* a3)
{
    unsigned int v3;
    int v4;
    unsigned int v5;
    int v6;
    unsigned int v7;
    char* v8;
    unsigned int v9;
    unsigned int v10;

    switch (a2)
    {
    case 7201:
        v10 = (unsigned int)a3->x;
        if ((int)v10 == -1)
        {
            a1->f[0] = a3->y;
            a1->f[1] = a3->y;
            a1->f[2] = a3->y;
            a1->f[3] = a3->y;
            a1->f[4] = a3->y;
            a1->f[5] = a3->y;
            a1->f[6] = a3->y;
            a1->f[7] = a3->y;
            a1->f[8] = a3->y;
            a1->f[9] = a3->y;
        }
        else if ((v10 & 0x80000000) == 0i64 && (int)v10 <= 10)
        {
            a1->f[v10] = a3->y;
        }
        break;
    case 7202:
        v9 = (unsigned int)a3->x;
        if ((int)v9 == -1)
        {
            a1->f2[0] = a3->y;
            a1->f2[1] = a3->y;
            a1->f2[2] = a3->y;
            a1->f2[3] = a3->y;
            a1->f2[4] = a3->y;
            a1->f2[5] = a3->y;
            a1->f2[6] = a3->y;
            a1->f2[7] = a3->y;
            a1->f2[8] = a3->y;
            a1->f2[9] = a3->y;
            *(float*)a1->vertexColorIndexes = a3->y; // TODO FIXME: ez jó így??
        }
        else if ((v9 & 0x80000000) == 0i64 && (int)v9 <= 10)
        {
            a1->f2[v9] = a3->y;
        }
        break;
    case 7203:
        v3 = (unsigned int)a3->x;
        v4 = v3;
        if ((v3 & 0x80000000) == 0 && (int)v3 <= 10)
        {
            v5 = (unsigned int)a3->y;
            v6 = v5;
            if ((v5 & 0x80000000) == 0 && (int)v5 <= 10)
            {
                v7 = (unsigned int)a3->z;
                v8 = &a1->vertexColorIndexes[10 * v4 + v6];
                *v8 = v7;
                if ((v7 & 0x80u) != 0)
                    *v8 = 0;
                if (*v8 >= 10)
                    *v8 = 9;
            }
        }
        break;
    }
}

struct sGfxTableData* GFX_Table_Create()
{
    tGfxTableData* v0;
    int v1;
    float* v2;
    float* v2a;
    int* v2b;

    v0 = (tGfxTableData*)MEM_p_Alloc(sizeof(tGfxTableData));
    memset(v0, 0, sizeof(tGfxTableData));
    v1 = 10;
    v0->num2 = 10;
    v0->num1 = 10;

    v2b = v0->colors;
    v2 = v0->f;
    v2a = v0->f2;

    do
    {
        *v2b = 0xffffffff;
        *v2 = 0.1f;
        *v2a = 0.1f;
        ++v2;
        --v1;
    } while (v1);
    return v0;
}