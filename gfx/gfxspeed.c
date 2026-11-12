#include "../memory.h"
#include "../time.h"
#include "gfxspeed.h"

int GFX_i_Speed_Render(struct sGfxSpeedData* speed)
{
    Vec3* verts;
    Vec3 prevPos;
    Vec3 curPos;
    Vec3 along;
    Vec3 toCamera;
    Vec3 side;

    speed->timeRemaining = (float)(speed->timeRemaining - TIM_gf_dt);
    if (speed->timeRemaining < 0.0)
        return 0;

    if (*(int32_t*)&speed->v2.x == -1)
    {
        MATH_TransformVertex(&speed->v2, &gpst_DX_stDD->ct.cam.cameraMtx, &speed->v1);
        return 1;
    }
    if (!(speed->v2.z >= 0.0))
        return 0;

    MATH_TransformVertex(&prevPos, &gpst_DX_stDD->ct.cam.objectMtx, &speed->v2);
    curPos = speed->v1;
    MATH_SubstractVec(&along, &curPos, &prevPos);
    MATH_TransformVertex(&speed->v2, &gpst_DX_stDD->ct.cam.cameraMtx, &speed->v1);

    GFX_NeedGeom(4u, 4u, 2u, 1);
    GFX_SetQuadUVs();

    MATH_SubstractVec(&toCamera, &curPos, &gpst_DX_stDD->ct.cam.objectMtx.Pos);
    MATH_CrossProduct(&side, &along, &toCamera);
    MATH_NormalizeEqualVector(&side);
    MATH_VecMultScalarWithoutBuffer(&side, speed->i2 * 0.5f);

    verts = GFX_gpst_Geo->vertices;
    MATH_VecAddVec(&verts[0], &curPos, &side);
    MATH_VecAddVec(&verts[1], &prevPos, &side);
    MATH_SubstractVec(&verts[2], &prevPos, &side);
    MATH_SubstractVec(&verts[3], &curPos, &side);

    GFX_SetQuadIndices();

    GFX_gpst_Geo->vertexColors[1] = speed->i3;
    GFX_gpst_Geo->vertexColors[2] = speed->i3;
    GFX_gpst_Geo->vertexColors[3] = speed->i3;
    GFX_gpst_Geo->vertexColors[4] = speed->i3;
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}

struct sGfxSpeedData* GFX_Speed_Create(void)
{
    tGfxSpeedData* speed;

    speed = (tGfxSpeedData*)MEM_p_Alloc(sizeof(tGfxSpeedData));
    memset(speed, 0, sizeof(tGfxSpeedData));
    speed->timeRemaining = 1.0;
    speed->i2 = 1.0;
    speed->i3 = -1;
    (*(int32_t*)&speed->v2.x) = -1; // the render checks for this
    return speed;
}
