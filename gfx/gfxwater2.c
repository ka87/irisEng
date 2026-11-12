#include "../memory.h"
#include "../time.h"
#include "gfxwater2.h"

struct sGfxWater2Data* GFX_Water2_Create(void)
{
    tGfxWater2Data* water;

    water = (tGfxWater2Data*)MEM_p_Alloc(sizeof(tGfxWater2Data));
    water->numToAlloc = 0;
    water->i2 = 0;
    water->num = 0;
    water->ptr = NULL;
    return water;
}

void GFX_Water2_Destroy(struct sGfxWater2Data* water)
{
    if (!water)
        return;

    if (water->ptr)
        MEM_Free(water->ptr);
    water->ptr = NULL;
    water->num = 0;
}

int GFX_i_Water2_Render(struct sGfxWater2Data* water)
{
    tGfxWater2InnerData* drop;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    int* colors;
    Vec3 center;
    Vec3 axis;
    Vec3 side;
    long double scale;
    float age;
    unsigned int color;
    int baseVertex;
    int alive;
    int i;

    if (!water->num)
        return 1;

    GFX_NeedGeom(3 * water->num, 3u, water->num, 1);

    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    colors = GFX_gpst_Geo->vertexColors + 1;
    verts = GFX_gpst_Geo->vertices;
    baseVertex = 0;
    alive = 0;
    i = 0;
    while (alive < water->num)
    {
        drop = &water->ptr[i];
        age = (float)(drop->ff5 / drop->ff1);
        color = LIGHT_ul_Interpol2Colors(drop->color, 0, age);

        drop->f1 = TIM_gf_dt * drop->v3.x + drop->f1;
        drop->f2 = TIM_gf_dt * drop->v3.y + drop->f2;

        scale = -1.0 / MATH_VecLen(&drop->v3)
              * ((drop->ff3 - drop->ff2) * age + drop->ff2);
        MATH_VecMultScalar(&axis, &drop->v3, (float)scale);
        MATH_CrossProduct(&side, &axis, &MATH_gst_BaseVectorK);
        MATH_VecMultScalarWithoutBuffer(&side, drop->ff4);

        center.x = drop->f1;
        center.y = drop->f2;
        center.z = drop->f3;
        MATH_AddScaleVector(&verts[0], &center, &axis, -0.5f);
        MATH_VecAddVecWithoutBuffer(&verts[0], &side);
        MATH_VecAddVec(&verts[1], &verts[0], &axis);
        MATH_SubstractVec(&verts[2], &verts[1], &side);

        tris[0].v1 = baseVertex;
        tris[0].v2 = baseVertex + 1;
        tris[0].v3 = baseVertex + 2;
        tris[0].uv1 = 0;
        tris[0].uv2 = 1;
        tris[0].uv3 = 2;

        colors[0] = color;
        colors[1] = color;
        colors[2] = color;

        drop->ff5 = (float)(TIM_gf_dt + drop->ff5);

        verts += 3;
        colors += 3;
        baseVertex += 3;
        tris++;

        if (drop->ff5 <= drop->ff1)
        {
            // this one lives; the next iteration takes the entry after it
            alive++;
            i++;
        }
        else
        {
            // this one is done: the last entry moves down into its slot and
            // the next iteration looks at the slot again
            water->num--;
            if (alive < water->num)
                memcpy(drop, &water->ptr[water->num], sizeof(tGfxWater2InnerData));
        }
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x800u);
    return 1;
}

void GFX_Water2_Seti(struct sGfxWater2Data* water, int id, int offset, int value)
{
    if (id == 10102)
    {
        if (water->num < water->numToAlloc)
        {
            if (!water->ptr)
                water->ptr = (tGfxWater2InnerData*)
                             MEM_p_Alloc(sizeof(tGfxWater2InnerData) * water->numToAlloc);
            water->num++;
        }
        water->ptr[water->num - 1].ff5 = 0.0;
        return;
    }

    if (water->num)
        *(int32_t*)((char*)&water->ptr[water->num - 1] + offset) = value;
}

void GFX_Water2_Setf(struct sGfxWater2Data* water, int id, int offset, float value)
{
    if (water->num)
        *(float*)((char*)&water->ptr[water->num - 1] + offset) = value;
}

void GFX_Water2_Setv(struct sGfxWater2Data* water, int id, int offset, Vec3* value)
{
    if (water->num)
        *(Vec3*)((char*)&water->ptr[water->num - 1] + offset) = *value;
}
