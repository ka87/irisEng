#include "../memory.h"
#include "../time.h"
#include "gfxspark.h"

struct sGfxSparkData* GFX_Spark_Create(void)
{
    tGfxSparkData* spark;

    spark = (tGfxSparkData*)MEM_p_Alloc(sizeof(tGfxSparkData));
    memset(spark, 0, sizeof(tGfxSparkData));
    spark->v1.x = 0.0;
    spark->v1.y = 0.0;
    spark->v1.z = 1.0;
    spark->i4 = 0.0;
    spark->i5 = 0.0;
    spark->i6 = -2.0f;
    spark->i10 = 0.89999998f;
    spark->ii1 = 2.0f;
    spark->ii4 = 1.0;
    spark->ii5 = 0.1f;
    spark->rand2Max = 2.0f;
    spark->rand2Min = 4.0f;
    spark->rand3Max = 1.0;
    spark->rand3Min = 2.0f;
    spark->iii1 = -1;
    spark->flags = 0x20;
    return spark;
}

void GFX_Spark_Destroy(struct sGfxSparkData* spark)
{
    if (!spark || !spark->allocdNum)
        return;

    spark->allocdNum = 0;
    MEM_Free(spark->ptr);
}

void GFX_Spark_Generate(struct sGfxSparkData* spark)
{
    tGfxSparkElemData* elem;
    Vec3 side;
    Vec3 up;
    float coneAngle;
    float azimuth;
    double speed;
    double acrossSide;
    double acrossUp;
    double alongAxis;

    spark->timeRemaining = (float)(spark->timeRemaining - TIM_gf_dt);
    if (!(spark->timeRemaining < 0.0))
        return;

    do
    {
        if (spark->actNum >= spark->num)
            break;
        spark->timeRemaining = spark->ii5 + spark->timeRemaining;

        // an orthonormal pair across the emission axis, from the world up
        // unless the axis is parallel to it
        MATH_CrossProduct(&side, &spark->v1, &MATH_gst_BaseVectorK);
        if (MATH_b_NulVector(&side))
            MATH_CrossProduct(&side, &spark->v1, &MATH_gst_BaseVectorJ);
        MATH_CrossProduct(&up, &side, &spark->v1);
        MATH_NormalizeEqualVector(&side);
        MATH_NormalizeEqualVector(&up);

        elem = &spark->ptr[spark->actNum];
        spark->actNum++;
        elem->v1 = spark->v0;

        coneAngle = MATH_RandFloatRange(-spark->i7, spark->i7);
        azimuth = (float)(MATH_RandFloat() * 6.2831855);
        speed = MATH_RandFloatRange(spark->rand2Max, spark->rand2Min);
        acrossSide = MATH_GetCos(azimuth) * MATH_GetSin(coneAngle) * speed;
        acrossUp = MATH_GetSin(azimuth) * MATH_GetSin(coneAngle) * speed;
        alongAxis = speed * MATH_GetCos(coneAngle);

        elem->i5 = (float)(side.x * acrossSide);
        elem->i6 = (float)(side.y * acrossSide);
        elem->i7 = (float)(side.z * acrossSide);

        elem->i5 = (float)(up.x * acrossUp + elem->i5);
        elem->i6 = (float)(up.y * acrossUp + elem->i6);
        elem->i7 = (float)(up.z * acrossUp + elem->i7);

        elem->i5 = (float)(spark->v1.x * alongAxis + elem->i5);
        elem->i6 = (float)(spark->v1.y * alongAxis + elem->i6);
        elem->i7 = (float)(spark->v1.z * alongAxis + elem->i7);

        elem->i9 = 0.0;
        elem->i1 = 0;
        elem->i8 = MATH_RandFloatRange(spark->rand3Max, spark->rand3Min);
    } while (spark->timeRemaining < 0.0);
}

static void GFX_l_SparkStep(tGfxSparkData* spark, tGfxSparkElemData* elem, float dt)
{
    elem->i5 = (float)(spark->i4 * dt + elem->i5);
    elem->i6 = (float)(spark->i5 * dt + elem->i6);
    elem->i7 = (float)(spark->i6 * dt + elem->i7);
    elem->i5 = (float)(spark->i10 * elem->i5);
    elem->i6 = (float)(spark->i10 * elem->i6);
    elem->i7 = (float)(spark->i10 * elem->i7);
}

static void GFX_l_SparkDrop(tGfxSparkData* spark, tGfxSparkElemData* elem)
{
    spark->actNum--;
    memcpy(elem, &spark->ptr[spark->actNum], sizeof(tGfxSparkElemData));
}

// head[0] is the spark's leading point and head[2] its trailing one, so
// head[1] is a spare vertex slot between them; the pass stores the strip
// width in its x and the strip colour, as an int, in its y. The render
// reads both back out of there.
void GFX_Spark_Update(tGfxSparkData* spark, Vec3* head, Vec3* tail, int stride)
{
    tGfxSparkElemData* elem;
    Vec3 along;
    Vec3 middle;
    Vec3 half;
    double subStep;
    double lengthSq;
    double shift;
    float maxLengthSq;
    float extraLife;
    int color;
    int mode;
    int processed;

    if (spark->actNum <= 0)
        return;

    maxLengthSq = (float)(spark->ii1 * spark->ii1);
    elem = spark->ptr;
    processed = 0;
    do
    {
        elem->i8 = (float)(elem->i8 - TIM_gf_dt);
        if (!(elem->i8 >= 0.0))
        {
            if ((spark->flags & 0x10) == 0 || (elem->i1 & 1) != 0)
            {
                GFX_l_SparkDrop(spark, elem);
                continue;
            }

            extraLife = MATH_RandFloatRange(spark->randMin, spark->randMax);
            if (!(extraLife > 0.0))
            {
                GFX_l_SparkDrop(spark, elem);
                continue;
            }

            // a second life, this one fading out over its own span
            elem->i8 = (float)(extraLife + elem->i8);
            elem->i9 = MATH_GetInverse(extraLife);
            elem->i1 |= 1;
        }

        for (subStep = TIM_gf_dt; subStep >= 0.033333335; subStep = subStep - 0.016666668)
        {
            elem->v1.x = (float)(elem->i5 * 0.016666668 + elem->v1.x);
            elem->v1.y = (float)(elem->i6 * 0.016666668 + elem->v1.y);
            elem->v1.z = (float)(elem->i7 * 0.016666668) + elem->v1.z;
            GFX_l_SparkStep(spark, elem, 0.016666668f);
        }

        *head = elem->v1;
        elem->v1.x = (float)(subStep * elem->i5 + elem->v1.x);
        elem->v1.y = (float)(subStep * elem->i6 + elem->v1.y);
        elem->v1.z = (float)(subStep * elem->i7) + elem->v1.z;
        *tail = elem->v1;
        GFX_l_SparkStep(spark, elem, (float)subStep);

        MATH_SubstractVec(&along, head, tail);
        lengthSq = along.z * along.z + along.y * along.y + along.x * along.x;

        mode = spark->flags & 3;
        if (mode == 0)
        {
            MATH_AddScaleVector(tail, tail, &along, (float)(1.0 - spark->ii4));
        }
        else if (mode == 1)
        {
            shift = (spark->ii4 - 1.0) * 0.5;
            MATH_AddScaleVector(head, head, &along, (float)shift);
            MATH_AddScaleVector(tail, tail, &along, (float)-shift);
        }
        else if (mode == 2)
        {
            MATH_AddScaleVector(head, head, &along, (float)(spark->ii4 - 1.0));
        }
        if (mode != 3)
            lengthSq = lengthSq * spark->ii4;

        if (lengthSq >= maxLengthSq)
            head[1].x = spark->ii1;
        else
            head[1].x = MATH_GetSqrt((float)lengthSq);
        head[1].x = head[1].x * 4.0f;

        // mirror the pair about its own midpoint, which doubles its length
        MATH_SubstractVec(&half, head, tail);
        MATH_VecAddVec(&middle, head, tail);
        MATH_VecMultScalarWithoutBuffer(&middle, 0.5f);
        MATH_VecAddVec(tail, &middle, &half);
        MATH_SubstractVec(head, &middle, &half);

        LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
        if ((elem->i1 & 1) != 0)
            color = LIGHT_ul_MulColor(spark->iii1, elem->i9 * elem->i8);
        else
            color = spark->iii1;
        *(uint32_t*)&head[1].y = color;

        processed++;
        GFX_l_SparkStep(spark, elem, TIM_gf_dt);
        head += stride;
        tail += stride;
        elem++;
    } while (processed < spark->actNum);
}

void GFX_AllocSpark(tGfxSparkData* spark)
{
    if (!spark->num)
    {
        spark->allocdNum = 0;
        MEM_Free(spark->ptr);
        return;
    }

    if (spark->allocdNum)
        spark->ptr = (tGfxSparkElemData*)
                     MEM_p_Realloc(spark->ptr, sizeof(tGfxSparkElemData) * spark->num);
    else
        spark->ptr = (tGfxSparkElemData*)
                     MEM_p_Alloc(sizeof(tGfxSparkElemData) * spark->num);
    spark->allocdNum = spark->num;
    if (spark->actNum > spark->num)
        spark->actNum = spark->num;
}

int GFX_i_Spark_Render(struct sGfxSparkData* spark)
{
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3 outward;
    Vec3 toCamera;
    Vec3 along;
    int* colors;
    double halfWidth;
    int color;
    int savedMask;
    int stride;
    int count;
    int i;

    if (!spark)
        return 1;

    spark->flags |= 0x20u;
    if (spark->allocdNum != spark->num)
        GFX_AllocSpark(spark);
    if (!spark->allocdNum)
        return 1;

    // 0x20 has just been set, so the stride is three and the four-vertex
    // paths below are unreachable as the file stands
    stride = 4 - ((spark->flags & 0x20) != 0);

    if ((spark->flags & 4) != 0)
    {
        GFX_NeedGeom(stride * spark->actNum, 4u, spark->actNum * (stride - 2), 1);
        GFX_Spark_Update(spark, GFX_gpst_Geo->vertices, GFX_gpst_Geo->vertices + 2, stride);
        if (!spark->actNum)
            return 0;
    }
    else
    {
        GFX_Spark_Generate(spark);
        if (spark->actNum >= spark->num && (spark->flags & 8) != 0)
            spark->flags |= 4;
        GFX_NeedGeom(stride * spark->actNum, 4u, spark->actNum * (stride - 2), 1);
        GFX_Spark_Update(spark, GFX_gpst_Geo->vertices, GFX_gpst_Geo->vertices + 2, stride);
        if (!spark->actNum)
            return 1;
    }

    if (!GFX_gpst_Current->aiGameObject->distance)
        return 1;

    count = spark->actNum;
    if ((spark->flags & 0x20) != 0)
    {
        GFX_NeedGeom(3 * count, 3u, count, 1);
        uvs = GFX_gpst_Geo->UVs;
        uvs[0].u = 1.0;
        uvs[0].v = 0.0;
        uvs[1].u = 0.0;
        uvs[1].v = 0.0;
        uvs[2].u = 0.5;
        uvs[2].v = 1.0;
    }
    else
    {
        GFX_NeedGeom(4 * count, 4u, 2 * count, 1);
        GFX_SetQuadUVs();
    }

    verts = GFX_gpst_Geo->vertices;
    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    colors = GFX_gpst_Geo->vertexColors + 1;

    if ((spark->flags & 0x20) != 0)
    {
        for (i = 0; i < count; i++)
        {
            color = *(uint32_t*)&verts[1].y;
            colors[0] = color;
            colors[1] = color;
            colors[2] = color;

            MATH_SubstractVec(&toCamera, &verts[2], &gpst_DX_stDD->ct.cam.objectMtx.Pos);
            MATH_SubstractVec(&along, &verts[2], &verts[0]);
            MATH_CrossProduct(&outward, &along, &toCamera);
            MATH_NormalizeEqualVector(&outward);
            halfWidth = verts[1].x * 0.5;
            MATH_VecMultScalarWithoutBuffer(&outward, (float)halfWidth);

            MATH_SubstractVec(&verts[1], &verts[0], &outward);
            MATH_VecAddVecWithoutBuffer(&verts[0], &outward);

            tris[0].v1 = 3 * i;
            tris[0].v2 = 3 * i + 1;
            tris[0].v3 = 3 * i + 2;
            tris[0].uv1 = 0;
            tris[0].uv2 = 1;
            tris[0].uv3 = 2;

            colors += 3;
            verts += 3;
            tris++;
        }
    }
    else
    {
        for (i = 0; i < count; i++)
        {
            color = *(uint32_t*)&verts[1].y;
            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;

            MATH_SubstractVec(&toCamera, &verts[2], &gpst_DX_stDD->ct.cam.objectMtx.Pos);
            MATH_SubstractVec(&along, &verts[2], &verts[0]);
            MATH_CrossProduct(&outward, &along, &toCamera);
            MATH_NormalizeEqualVector(&outward);
            halfWidth = verts[1].x * 0.5;
            MATH_VecMultScalarWithoutBuffer(&outward, (float)halfWidth);

            MATH_SubstractVec(&verts[3], &verts[0], &outward);
            MATH_VecAddVec(&verts[1], &verts[2], &outward);
            MATH_SubstractVec(&verts[2], &verts[2], &outward);
            MATH_VecAddVecWithoutBuffer(&verts[0], &outward);

            tris[0].v1 = 4 * i;
            tris[0].v2 = 4 * i + 1;
            tris[0].v3 = 4 * i + 2;
            tris[0].uv1 = 0;
            tris[0].uv2 = 1;
            tris[0].uv3 = 2;
            tris[1].v1 = 4 * i + 2;
            tris[1].v2 = 4 * i + 3;
            tris[1].v3 = 4 * i;
            tris[1].uv1 = 2;
            tris[1].uv2 = 3;
            tris[1].uv3 = 0;

            colors += 4;
            verts += 4;
            tris += 2;
        }
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    savedMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask & ~0x8000900u;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask;
    return 1;
}
