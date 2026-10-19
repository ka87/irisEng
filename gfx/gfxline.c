#include "../memory.h"
#include "gfxline.h"

struct sGfxLineData* GFX_Line_Create()
{
    tGfxLineData* v0;

    v0 = (tGfxLineData*)MEM_p_Alloc(sizeof(tGfxLineData));
    memset(v0, 0, sizeof(tGfxLineData));
    v0->v1.y = 0.0;
    v0->v1.z = 0.0;
    v0->v1.x = -10.0;
    v0->v2.z = 0.0;
    v0->v2.x = 10.0;
    v0->v2.y = 10.0;
    v0->v3.x = 0.0;
    v0->v3.y = 0.0;
    v0->v3.z = 1.0;
    v0->f = 0.1f;
    v0->vertexColor = 0xFFFFFFFF;
    return v0;
}

void GFX_Line_Render(struct sGfxLineData* a1)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;
    long double v10;
    Vec3* v11;
    Vec3* v12;
    Vec3* v13;
    Vec3* v14;
    int v15;
    int* v16;
    int v17;
    float v18;
    Vec3 v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float a1a;
    float a1b;
    float a1c;

    GFX_NeedGeom(4u, 4u, 2u, 1);
    v2 = a1->v2.x - a1->v1.x;
    v3 = a1->v2.y - a1->v1.y;
    v28 = a1->v2.z - a1->v1.z;
    v18 = (float)v2;
    v22 = (float)v3;
    a1a = (v28 * (gpst_DX_stDD->ct.cam.objectMtx.Pos.z - a1->v1.z)
        + v22 * (gpst_DX_stDD->ct.cam.objectMtx.Pos.y - a1->v1.y)
        + v18 * (gpst_DX_stDD->ct.cam.objectMtx.Pos.x - a1->v1.x))
        / (v22 * v22 + v28 * v28 + v18 * v18);
    if (a1a >= 0.0)
    {
        if (a1a <= 1.0)
        {
            v29 = v28 * a1a;
            v19.x = v18 * a1a + a1->v1.x;
            v19.y = v22 * a1a + a1->v1.y;
            v19.z = v29 + a1->v1.z;
        }
        else
        {
            v19 = a1->v2;
        }
    }
    else
    {
        v19 = a1->v1;
    }
    v4 = v19.x - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
    v5 = v19.y - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
    v6 = v19.z - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
    v20 = (float)(v6 * v3 - v5 * v28);
    v23 = (float)(v28 * v4 - v6 * v2);
    v25 = (float)(v5 * v2 - v3 * v4);
    if (fabs(v20) != 0 || fabs(v28 * v4 - v6 * v2) || fabs(v5 * v2 - v3 * v4))
    {
        a1c = a1->f;
        v10 = 1.0 / sqrt(v25 * v25 + v23 * v23 + v20 * v20);
        v21 = (float)(v20 * v10);
        v24 = (float)(v23 * v10);
        v26 = (float)(v25 * v10);
        v7 = a1c * v21;
        v8 = v24 * a1c;
        v9 = v26 * a1c;
    }
    else
    {
        a1b = a1->f;
        v7 = a1b * a1->v3.x;
        v8 = a1->v3.y * a1b;
        v9 = a1->v3.z * a1b;
    }
    v27 = (float)v9;
    v11 = GFX_gpst_Geo->vertices;
    v11->x = (float)(v7 + a1->v1.x);
    v11->y = (float)(v8 + a1->v1.y);
    v11->z = v27 + a1->v1.z;
    v12 = GFX_gpst_Geo->vertices + 1;
    v12->x = (float)(a1->v1.x - v7);
    v12->y = (float)(a1->v1.y - v8);
    v12->z = a1->v1.z - v27;
    v13 = GFX_gpst_Geo->vertices + 2;
    v13->x = (float)(a1->v2.x - v7);
    v13->y = (float)(a1->v2.y - v8);
    v13->z = a1->v2.z - v27;
    v14 = GFX_gpst_Geo->vertices + 3;
    v14->x = (float)(v7 + a1->v2.x);
    v14->y = (float)(a1->v2.y + v8);
    v14->z = a1->v2.z + v27;
    v15 = a1->vertexColor;
    v16 = GFX_gpst_Geo->vertexColors + 1;
    v16[3] = v15;
    v16[2] = v15;
    v16[1] = v15;
    v16[0] = v15;
    GFX_gpst_Geo->UVs->u = 0.0;
    GFX_gpst_Geo->UVs->v = 0.0;
    GFX_gpst_Geo->UVs[1].u = 0.0;
    GFX_gpst_Geo->UVs[1].v = 1.0;
    GFX_gpst_Geo->UVs[2].u = 1.0;
    GFX_gpst_Geo->UVs[2].v = 1.0;
    GFX_gpst_Geo->UVs[3].u = 1.0;
    GFX_gpst_Geo->UVs[3].v = 0.0;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->v1 = 0;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->v2 = 1;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->v3 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv1 = 0; // TODO nem biztos h jó
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv2 = 1;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv3 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v1 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v2 = 3;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v3 = 0;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv1 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv2 = 3;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv3 = 0;
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    v17 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v17 & ~0x8000900u;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v17;
}