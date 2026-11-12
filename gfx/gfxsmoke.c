#include "../memory.h"
#include "../time.h"
#include "gfxsmoke.h"

void GFX_Smoke_Update(tGfxSmokeData* smoke, float dt, int steps)
{
    tGfxSmokeInnerData* puff;
    tGfxSmokeInnerData* end;
    float damping;
    int step;

    puff = smoke->ptr;
    end = &puff[smoke->num];
    while (puff < end)
    {
        puff->i7 = (float)(dt + puff->i7);
        if (!(puff->i7 < smoke->i1))
        {
            smoke->num--;
            memcpy(puff, &smoke->ptr[smoke->num], sizeof(tGfxSmokeInnerData));
            end--;
            continue;
        }

        for (step = 0; step < steps; step++)
        {
            puff->i1 = (float)(puff->i4 * 0.016666668 + puff->i1);
            puff->i2 = (float)(puff->i5 * 0.016666668 + puff->i2);
            puff->i3 = (float)(puff->i6 * 0.016666668) + puff->i3;
            damping = smoke->i2;
            puff->i4 = (float)(damping * puff->i4);
            puff->i5 = (float)(damping * puff->i5);
            puff->i6 = (float)(damping * puff->i6);
        }
        puff++;
    }
}

int GFX_i_Smoke_Render(struct sGfxSmokeData* smoke)
{
    tGfxSmokeInnerData* puff;
    tGfxSmokeInnerData* end;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3* camRight;
    Vec3* camUp;
    Vec3 right;
    Vec3 up;
    int* colors;
    double elapsed;
    unsigned int steps;
    unsigned int color;
    float stepped;
    float invLife;
    float age;
    float size;
    int baseVertex;

    elapsed = TIM_gf_dt + smoke->timeElapsed;
    steps = (unsigned int)(60.0 * elapsed);
    stepped = (float)((double)(int)steps * 0.016666668);
    smoke->timeElapsed = (float)(elapsed - stepped);
    if (steps)
        GFX_Smoke_Update(smoke, stepped, steps);

    if (!GFX_gpst_Current->aiGameObject->distance || !smoke->num)
        return 1;

    GFX_NeedGeom(4 * smoke->num, 4u, 2 * smoke->num, 1);
    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;

    camRight = &gpst_DX_stDD->ct.cam.objectMtx.v1;
    camUp = &gpst_DX_stDD->ct.cam.objectMtx.v2;
    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    verts = GFX_gpst_Geo->vertices;
    colors = GFX_gpst_Geo->vertexColors + 1;
    baseVertex = 0;

    if (smoke->i1 == 0.0)
        invLife = 0.0;
    else
        invLife = (float)(1.0 / smoke->i1);

    puff = smoke->ptr;
    end = &puff[smoke->num];
    while (puff < end)
    {
        age = invLife * puff->i7;
        size = (smoke->i4 - smoke->i3) * age + smoke->i3;
        color = LIGHT_ul_Interpol2Colors(smoke->color1, smoke->color2, age);

        MATH_VecMultScalar(&right, camRight, size);
        MATH_VecMultScalar(&up, camUp, size);

        verts[0].x = (float)(puff->i1 - right.x * 0.5);
        verts[0].y = (float)(puff->i2 - right.y * 0.5);
        verts[0].z = puff->i3 - (float)(right.z * 0.5);
        verts[0].x = (float)(up.x * 0.5 + verts[0].x);
        verts[0].y = (float)(up.y * 0.5 + verts[0].y);
        verts[0].z = (float)(up.z * 0.5) + verts[0].z;

        MATH_VecAddVec(&verts[1], &verts[0], &right);
        MATH_SubstractVec(&verts[2], &verts[1], &up);
        MATH_SubstractVec(&verts[3], &verts[2], &right);

        tris[0].v1 = baseVertex;
        tris[0].v2 = baseVertex + 1;
        tris[0].v3 = baseVertex + 2;
        tris[0].uv1 = 0;
        tris[0].uv2 = 1;
        tris[0].uv3 = 2;
        tris[1].v1 = baseVertex + 2;
        tris[1].v2 = baseVertex + 3;
        tris[1].v3 = baseVertex;
        tris[1].uv1 = 2;
        tris[1].uv2 = 3;
        tris[1].uv3 = 0;

        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;

        tris += 2;
        verts += 4;
        colors += 4;
        baseVertex += 4;
        puff++;
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}

struct sGfxSmokeData* GFX_Smoke_Create(void)
{
    tGfxSmokeData* smoke;

    smoke = (tGfxSmokeData*)MEM_p_Alloc(sizeof(tGfxSmokeData));
    memset(smoke, 0, sizeof(tGfxSmokeData));
    smoke->i1 = 2.0f;
    smoke->i2 = 0.89999998f;
    smoke->i3 = 0.5f;
    smoke->i4 = 2.0f;
    smoke->color1 = 0xFFFFFFFF;
    smoke->color2 = 0;
    return smoke;
}

void GFX_Smoke_Destroy(struct sGfxSmokeData* smoke)
{
    if (!smoke || !smoke->innerDataNum)
        return;

    smoke->innerDataNum = 0;
    MEM_Free(smoke->ptr);
}

void GFX_Smoke_Alloc(struct sGfxSmokeData* smoke, int num)
{
    if (num <= 0)
    {
        if (smoke->innerDataNum)
        {
            smoke->innerDataNum = 0;
            MEM_Free(smoke->ptr);
        }
        return;
    }

    if (num == smoke->innerDataNum)
        return;

    if (smoke->innerDataNum)
        smoke->ptr = (tGfxSmokeInnerData*)
                     MEM_p_Realloc(smoke->ptr, sizeof(tGfxSmokeInnerData) * num);
    else
        smoke->ptr = (tGfxSmokeInnerData*)MEM_p_Alloc(sizeof(tGfxSmokeInnerData) * num);
    smoke->innerDataNum = num;
}

void GFX_Smoke_AddOne(tGfxSmokeData* smoke)
{
    tGfxSmokeInnerData* puff;

    if (smoke->num >= smoke->innerDataNum)
        return;

    puff = &smoke->ptr[smoke->num];
    smoke->num++;
    puff->i7 = 0.0;
    puff->i1 = smoke->innerData1;
    puff->i2 = smoke->innerData2;
    puff->i3 = smoke->innerData3;
    puff->i4 = smoke->innerData4;
    puff->i5 = smoke->innerData5;
    puff->i6 = smoke->innerData6;
}

void GFX_Smoke_Seti(struct sGfxSmokeData* smoke, int id, int num)
{
    if (!smoke)
        return;

    if (id == 6100)
        GFX_Smoke_Alloc(smoke, num);
    else if (id == 6103)
        GFX_Smoke_AddOne(smoke);
}
