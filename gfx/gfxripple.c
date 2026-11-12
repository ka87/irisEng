#include "../memory.h"
#include "../time.h"
#include "gfxripple.h"

struct sGfxRippleData* GFX_Ripple_Create(struct sGameObjectMain* gao)
{
    tGfxRippleData* ripple;
    Mtx4x4* absMtx;

    ripple = (tGfxRippleData*)MEM_p_Alloc(sizeof(tGfxRippleData));
    memset(ripple, 0, sizeof(tGfxRippleData));

    ripple->pos = gao->matrices->Pos;
    absMtx = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_VecInvert(&ripple->v1, &absMtx->v2);
    ripple->v2 = absMtx->v3;
    ripple->f1 = 5.0f;
    ripple->f2 = 1.0f;
    ripple->f3 = 0.2f;
    ripple->f4 = 1.0f;
    ripple->color1 = 0;
    ripple->color2 = 0xFFFFFFFF;
    return ripple;
}

void GFX_Ripple_Generate(tGfxRippleData* ripple, float dt)
{
    tGfxRippleInnerData* ring;

    if (ripple->f3 < 0.0 || (ripple->flags & 1) != 0)
        return;

    ripple->fff = (float)(dt + ripple->fff);
    while (ripple->fff > (double)ripple->f3)
    {
        if (ripple->rippleMaxElemID == ripple->num)
            break;

        ring = &ripple->ptr[ripple->rippleMaxElemID];
        ripple->numUnk++;
        ripple->rippleMaxElemID++;

        ring->f1 = ripple->f1;
        ring->pos = ripple->pos;
        MATH_VecInvert(&ring->v1Inv, &ripple->v1);
        ring->v2 = ripple->v2;

        if (ripple->f3 == 0.0)
        {
            ripple->f3 = -1.0;
            break;
        }
        ripple->fff = ripple->fff - ripple->f3;
    }

    if (ripple->numUnk == ripple->num && (ripple->flags & 2) != 0)
        ripple->flags |= 1;
}

void GFX_Ripple_Update(tGfxRippleData* ripple, float dt)
{
    tGfxRippleInnerData* ring;
    tGfxRippleInnerData* end;

    ring = ripple->ptr;
    end = &ring[ripple->rippleMaxElemID];
    while (ring < end)
    {
        ring->f1 = (float)(ring->f1 - dt);
        if (!(ring->f1 >= 0.0))
        {
            ripple->rippleMaxElemID--;
            memcpy(ring, &ripple->ptr[ripple->rippleMaxElemID],
                   sizeof(tGfxRippleInnerData));
            end--;
            continue;
        }

        MATH_AddScaleVector(&ring->pos, &ring->pos, &ring->v1Inv,
                            (float)(dt * ripple->f4));
        ring++;
    }
}

void GFX_Ripple_Alloc(tGfxRippleData* ripple, int num)
{
    if (num <= 0)
    {
        if (ripple->num)
        {
            ripple->num = 0;
            MEM_Free(ripple->ptr);
        }
        return;
    }

    if (num == ripple->num)
        return;

    if (ripple->num)
        ripple->ptr = (tGfxRippleInnerData*)
                      MEM_p_Realloc(ripple->ptr, sizeof(tGfxRippleInnerData) * num);
    else
        ripple->ptr = (tGfxRippleInnerData*)
                      MEM_p_Alloc(sizeof(tGfxRippleInnerData) * num);
    ripple->num = num;
}

void GFX_Ripple_Seti(struct sGfxRippleData* ripple, int id, int value)
{
    if (!ripple || id != 20100)
        return;

    if (value == -1)
        ripple->flags |= 1u;
    else if (value == -2)
        ripple->flags |= 2u;
    else
        GFX_Ripple_Alloc(ripple, value);
}

void GFX_Ripple_Setf(struct sGfxRippleData* ripple, int id, float value)
{
    if (!ripple || id != 20002)
        return;

    ripple->f3 = value;
    ripple->fff = value;
}

void GFX_Ripple_Destroy(struct sGfxRippleData* ripple)
{
    if (!ripple || !ripple->num)
        return;

    ripple->num = 0;
    MEM_Free(ripple->ptr);
}

BOOL GFX_i_Ripple_Render(struct sGfxRippleData* ripple)
{
    tGfxRippleInnerData* ring;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3 right;
    Vec3 up;
    int* colors;
    double amplitude;
    unsigned int color;
    float invLife;
    int baseVertex;
    int count;
    int i;

    GFX_Ripple_Generate(ripple, TIM_gf_dt);
    GFX_Ripple_Update(ripple, TIM_gf_dt);

    count = ripple->rippleMaxElemID;
    if (!count)
        return (ripple->flags & 1) == 0;

    if ((ripple->flags & 2) == 0 && !GFX_gpst_Current->aiGameObject->distance)
        return 1;

    GFX_NeedGeom(4 * count, 4u, 2 * count, 1);
    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    verts = GFX_gpst_Geo->vertices;
    colors = GFX_gpst_Geo->vertexColors + 1;
    baseVertex = 0;

    if (ripple->f1 == 0.0)
        invLife = 0.0;
    else
        invLife = MATH_GetInverse(ripple->f1);

    for (i = 0; i < count; i++)
    {
        ring = &ripple->ptr[i];
        amplitude = (ripple->f1 - ring->f1) * ripple->f2 + ripple->f555;

        MATH_VecMultScalar(&right, &ring->v2, (float)amplitude);
        MATH_CrossProduct(&up, &ring->v1Inv, &right);

        verts[0].x = (float)(ring->pos.x - right.x * 0.5);
        verts[0].y = (float)(ring->pos.y - right.y * 0.5);
        verts[0].z = ring->pos.z - (float)(right.z * 0.5);
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

        color = LIGHT_ul_Interpol2Colors(ripple->color1, ripple->color2,
                                         invLife * ring->f1);
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;

        tris += 2;
        verts += 4;
        colors += 4;
        baseVertex += 4;
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}
