#include "../memory.h"
#include "../time.h"
#include "gfxspeed.h"

int GFX_i_Speed_Render(struct sGfxSpeedData* a1)
{
    double v1;
    int result;
    float v3;
    float v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;
    Vec3* v10;
    double v11;
    Vec3* v12;
    Vec3* v13;
    Vec3* v14;
    int v15;
    Vec3 v16;
    float v17;
    float v18;
    float v19;
    Vec3 a1a;
    float v21;
    float v22;
    float v23;

    v1 = a1->timeRemaining - TIM_gf_dt;
    a1->timeRemaining = (float)v1;
    if (v1 < 0.0)
        return 0;
    if (*(int32_t *)&(a1->v2.x) == -1)
    {
        MATH_TransformVertex(&a1->v2, &gpst_DX_stDD->ct.cam.cameraMtx, &a1->v1);
        result = 1;
    }
    else if (a1->v2.z >= 0.0)
    {
        MATH_TransformVertex(&a1a, &gpst_DX_stDD->ct.cam.objectMtx, &a1->v2);
        v3 = a1->v1.y;
        v4 = a1->v1.z;
        v17 = a1->v1.x;
        v18 = v3;
        v19 = v4;
        v21 = v17 - a1a.x;
        v22 = v3 - a1a.y;
        v23 = v4 - a1a.z;
        MATH_TransformVertex(&a1->v2, &gpst_DX_stDD->ct.cam.cameraMtx, &a1->v1);
        GFX_NeedGeom(4u, 4u, 2u, 1);
        v5 = v17;
        GFX_gpst_Geo->UVs->u = 0.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 0.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;
        v6 = v5 - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
        v7 = v18 - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
        v8 = v19 - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
        v16.x = (float)(v22 * v8 - v7 * v23);
        v16.y = (float)(v23 * v6 - v8 * v21);
        v16.z = (float)(v7 * v21 - v22 * v6);
        MATH_NormalizeEqualVector(&v16);
        v9 = a1->i2 * 0.5;
        v10 = GFX_gpst_Geo->vertices;
        v11 = v16.x * v9;
        v16.y = (float)(v16.y * v9);
        v16.z = (float)(v16.z * v9);
        v10->x = (float)(v17 + v11);
        v10->y = v16.y + v18;
        v10->z = v16.z + v19;
        v12 = GFX_gpst_Geo->vertices + 1;
        v12->x = (float)(a1a.x + v11);
        v12->y = v16.y + a1a.y;
        v12->z = v16.z + a1a.z;
        v13 = GFX_gpst_Geo->vertices + 2;
        v13->x = (float)(a1a.x - v11);
        v13->y = a1a.y - v16.y;
        v13->z = a1a.z - v16.z;
        v14 = GFX_gpst_Geo->vertices + 3;
        v14->x = (float)(v17 - v11);
        v14->y = v18 - v16.y;
        v14->z = v19 - v16.z;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v1 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v2 = 1;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v3 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv1 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv2 = 1;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv3 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v1 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v2 = 3;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v3 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv1 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv2 = 3;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv3 = 0;
        GFX_gpst_Geo->vertexColors[1] = a1->i3;
        GFX_gpst_Geo->vertexColors[2] = a1->i3;
        GFX_gpst_Geo->vertexColors[3] = a1->i3;
        GFX_gpst_Geo->vertexColors[4] = a1->i3;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        v15 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v15 & ~0x8000900u;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v15;
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

struct sGfxSpeedData* GFX_Speed_Create()
{
    tGfxSpeedData* v0;

    v0 = (tGfxSpeedData*)MEM_p_Alloc(sizeof(tGfxSpeedData));
    memset(v0, 0, sizeof(tGfxSpeedData));
    v0->timeRemaining = 1.0;
    v0->i2 = 1.0;
    v0->i3 = -1;
    (*(int32_t *)&(v0->v2.x)) = -1; // erre ellenõriz a renderben
    return v0;
}