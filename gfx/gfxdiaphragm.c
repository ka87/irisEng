#include "../memory.h"
#include "../time.h"
#include "gfxdiaphragm.h"

float depth_362 = 0.055f;

static const double GFX_l_DiaphragmBladeAngles[8] =
{
    0.0, 0.78539819, 1.5707964, 2.3561945, 3.1415927, 3.926991, 4.712389, 5.4977875
};

struct sGfxDiaphragmData* GFX_Diaphragm_Create(void)
{
    tGfxDiaphragmData* iris;

    iris = (tGfxDiaphragmData*)MEM_p_Alloc(sizeof(tGfxDiaphragmData));
    memset(iris, 0, sizeof(tGfxDiaphragmData));
    iris->v1.x = 0.5f;
    iris->v1.y = 0.5f;
    iris->v1.z = 0.0f;
    return iris;
}

static tGeoTriangleChunkElem* GFX_l_DiaphragmTriangle(tGeoTriangleChunkElem* tris,
                                                      int corner1, int corner2, int corner3)
{
    tris->v1 = corner1;
    tris->v2 = corner2;
    tris->v3 = corner3;
    tris->uv1 = 0;
    tris->uv2 = 1;
    tris->uv3 = 2;
    return tris + 1;
}

// which screen corner a blade tip sits against: bit 0 for the right half,
// bit 1 for the lower, which is the index of that corner in vertices[0..3]
static int GFX_l_DiaphragmCorner(Vec3* point)
{
    return (point->x > 0.0) | (point->y <= 0.0 ? 0 : 2);
}

int GFX_i_Diaphragm_Render(tGfxDiaphragmData* iris)
{
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3* prev;
    Vec3* cur;
    float left;
    float top;
    float right;
    float bottom;
    float centreX;
    float centreY;
    float spanX;
    float spanY;
    float radius;
    float baseAngle;
    float angle;
    float blend;
    int prevCorner;
    int curCorner;
    int stage;
    int i;

    blend = TIM_gf_dt + iris->elapsedTime;
    iris->elapsedTime = blend;

    stage = 0;
    if (!(blend < iris->f01[0]))
    {
        do
        {
            blend = blend - iris->f01[stage];
            stage++;
            if (stage == 4)
                return 0;
        } while (!(blend <= iris->f01[stage]));
    }

    switch (stage)
    {
    case 0:
        blend = 0.0;
        break;
    case 1:
        blend = blend / iris->f01[1];
        break;
    case 2:
        blend = 1.0;
        break;
    case 3:
        blend = 1.0f - blend / iris->f01[3];
        break;
    default:
        break;
    }

    if (blend < 0.99000001f)
        GFX_NeedGeom(12, 4, 12, 1);
    else
        GFX_NeedGeom(4, 4, 2, 1);

    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;

    left = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
    top = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
    right = gpst_DX_stDD->ct.cam.viewPortRealPosX + gpst_DX_stDD->ct.cam.width;
    bottom = gpst_DX_stDD->ct.cam.viewPortRealPosY + gpst_DX_stDD->ct.cam.height;
    centreX = (left + right) * iris->v1.x;
    centreY = (top + bottom) * iris->v1.y;

    verts = GFX_gpst_Geo->vertices;
    verts[0].x = left;
    verts[0].y = top;
    verts[0].z = depth_362;
    verts[1].x = right;
    verts[1].y = top;
    verts[1].z = depth_362;
    verts[2].x = left;
    verts[2].y = bottom;
    verts[2].z = depth_362;
    verts[3].x = right;
    verts[3].y = bottom;
    verts[3].z = depth_362;
    for (i = 0; i < 4; i++)
    {
        CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &verts[i], &verts[i]);
        GFX_gpst_Geo->vertexColors[i + 1] = iris->color;
    }

    if (!(blend < 0.99000001f))
    {
        tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        tris[0].v1 = 2;
        tris[0].v2 = 1;
        tris[0].v3 = 0;
        tris[0].uv1 = 0;
        tris[0].uv2 = 1;
        tris[0].uv3 = 2;
        tris[1].v1 = 2;
        tris[1].v2 = 3;
        tris[1].v3 = 1;
        tris[1].uv1 = 2;
        tris[1].uv2 = 3;
        tris[1].uv3 = 1;

        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_RenderGeom(0x8000900u);
        return 1;
    }

    spanX = centreX - left;
    if (spanX < right - centreX)
        spanX = right - centreX;
    spanY = centreY - top;
    if (spanY < bottom - centreY)
        spanY = bottom - centreY;
    radius = (1.0f - blend) * MATH_GetSqrt(spanY * spanY + spanX * spanX);

    baseAngle = blend * 3.1415927f + 0.39269909f;
    for (i = 0; i < 8; i++)
    {
        angle = (float)(baseAngle + GFX_l_DiaphragmBladeAngles[i]);
        verts[4 + i].x = (float)(MATH_GetCos(angle) * radius);
        verts[4 + i].y = (float)(MATH_GetSin(angle) * radius);
        verts[4 + i].z = depth_362;
    }

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    prev = &verts[11];
    for (i = 0; i < 8; i++)
    {
        cur = &verts[4 + i];
        prevCorner = GFX_l_DiaphragmCorner(prev);
        curCorner = GFX_l_DiaphragmCorner(cur);
        if (prevCorner != curCorner)
            tris = GFX_l_DiaphragmTriangle(tris, prev - verts, prevCorner, curCorner);
        tris = GFX_l_DiaphragmTriangle(tris, prev - verts, curCorner, cur - verts);
        prev = cur;
    }

    for (i = 0; i < 8; i++)
    {
        verts[4 + i].x = centreX + verts[4 + i].x;
        verts[4 + i].y = centreY + verts[4 + i].y;
        CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &verts[4 + i], &verts[4 + i]);
        GFX_gpst_Geo->vertexColors[4 + i + 1] = iris->color;
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}
