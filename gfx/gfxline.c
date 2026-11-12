#include "../memory.h"
#include "gfxline.h"

struct sGfxLineData* GFX_Line_Create(void)
{
    tGfxLineData* line;

    line = (tGfxLineData*)MEM_p_Alloc(sizeof(tGfxLineData));
    memset(line, 0, sizeof(tGfxLineData));
    line->v1.x = -10.0;
    line->v1.y = 0.0;
    line->v1.z = 0.0;
    line->v2.x = 10.0;
    line->v2.y = 10.0;
    line->v2.z = 0.0;
    line->v3.x = 0.0;
    line->v3.y = 0.0;
    line->v3.z = 1.0;
    line->f = 0.1f;
    line->vertexColor = 0xFFFFFFFF;
    return line;
}

void GFX_Line_Render(struct sGfxLineData* line)
{
    Vec3* verts;
    Vec3 dir;
    Vec3 toStart;
    Vec3 closest;
    Vec3 toCamera;
    Vec3 normal;
    Vec3 offset;
    tGeoUVElem* uvs;
    float along;

    GFX_NeedGeom(4u, 4u, 2u, 1);

    MATH_SubstractVec(&dir, &line->v2, &line->v1);
    MATH_SubstractVec(&toStart, &gpst_DX_stDD->ct.cam.objectMtx.Pos, &line->v1);
    along = MATH_DotProduct(&dir, &toStart) / MATH_VecLenSquared(&dir);

    if (!(along >= 0.0))
        closest = line->v1;
    else if (along > 1.0)
        closest = line->v2;
    else
        MATH_AddScaleVector(&closest, &line->v1, &dir, along);

    MATH_SubstractVec(&toCamera, &closest, &gpst_DX_stDD->ct.cam.objectMtx.Pos);
    MATH_CrossProduct(&normal, &dir, &toCamera);
    if (!MATH_b_NulVector(&normal))
    {
        MATH_NormalizeVector(&offset, &normal);
        MATH_VecMultScalarWithoutBuffer(&offset, line->f);
    }
    else
    {
        MATH_VecMultScalar(&offset, &line->v3, line->f);
    }

    verts = GFX_gpst_Geo->vertices;
    MATH_VecAddVec(&verts[0], &line->v1, &offset);
    MATH_SubstractVec(&verts[1], &line->v1, &offset);
    MATH_SubstractVec(&verts[2], &line->v2, &offset);
    MATH_VecAddVec(&verts[3], &line->v2, &offset);

    GFX_gpst_Geo->vertexColors[1] = line->vertexColor;
    GFX_gpst_Geo->vertexColors[2] = line->vertexColor;
    GFX_gpst_Geo->vertexColors[3] = line->vertexColor;
    GFX_gpst_Geo->vertexColors[4] = line->vertexColor;

    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 0.0;
    uvs[1].v = 1.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 1.0;
    uvs[3].v = 0.0;

    GFX_SetQuadIndices();
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
}
