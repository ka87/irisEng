#include "../memory.h"
#include "../time.h"
#include "../platform/wingdi.h"
#include "gfxsun.h"

tGfxSunRingOffset gSunStru_951A40[] = {
    {.x = 1.0f, .y = 0.0f },
    {.x = -0.30000001f, .y = 0.69999999f },
    {.x = -0.69999999f, .y = 0.30000001f  },
    {.x = -0.69999999f, .y = -0.30000001f  },
    {.x = -0.30000001f, .y = -0.69999999f  },
    {.x = 0.30000001f, .y = -0.69999999f  },
    {.x = 0.69999999f, .y = -0.30000001f  },
    {.x = 0.69999999f, .y = 0.30000001f  },
    {.x = 0.0f, .y = 0.0f }
};

// one ray: a direction, a length and a half-width, plus the two colour
// octants its angle falls between
static void GFX_l_SunMakeRay(tGfxSunDataInner* ray, double angle, float lengthMin,
                             float lengthMax, float widthMin, float widthMax)
{
    unsigned int octant;

    if (angle < 0.0)
        angle = angle + 6.2831855;

    octant = (unsigned int)(1.2732395 * angle) & 7;
    ray->colorID = octant;
    ray->colorID2 = (octant + 1) & 7;
    ray->colorFactor = (float)(1.2732395 * angle - (double)octant);
    ray->i1 = MATH_GetCos((float)angle);
    ray->i2 = MATH_GetSin((float)angle);
    ray->i4 = MATH_RandFloatRange(lengthMin, lengthMax);
    ray->i5 = MATH_RandFloatRange(widthMin, widthMax);
}

void GFX_Sun_ComputeRay(tGfxSunData* sun)
{
    float angleStep;
    float angle;
    int i;

    if ((sun->num & 1) != 0)
        sun->num &= ~1u;
    if (sun->num > 128)
        sun->num = 128;

    angleStep = (float)(6.2831855 / (double)(sun->num >> 1));
    angle = 0.0;
    for (i = 0; i < sun->num; i += 2)
    {
        GFX_l_SunMakeRay(&sun->elems[i],
                         MATH_RandFloatRange(-sun->i4, sun->i4) + angle,
                         sun->i5, sun->i6, sun->i7, sun->i8);
        GFX_l_SunMakeRay(&sun->elems[i + 1],
                         MATH_RandFloatRange(-sun->i9, sun->i9) + angle,
                         sun->i10, sun->i11, sun->i12, sun->i13);
        angle = angle + angleStep;
    }
}

void GFX_Sun_InitRayLum(tGfxSunData* sun)
{
    float lum;
    int i;

    lum = 0.0;
    if ((sun->flags & 8) == 0)
        lum = 1.0;
    for (i = 0; i < 8; i++)
        sun->rayLums[i] = lum;
}

struct sGfxSunData* GFX_Sun_Create(void)
{
    tGfxSunData* sun;

    sun = (tGfxSunData*)MEM_p_Alloc(sizeof(tGfxSunData));
    memset(sun, 0, sizeof(tGfxSunData));
    MATH_SetNullVector(&sun->v1);
    MATH_SetNullVector(&sun->v2);
    sun->i4 = 0.2f;
    sun->i5 = 10.0;
    sun->i6 = 15.0f;
    sun->i7 = 1.0f;
    sun->i8 = 2.0f;
    sun->i9 = 0.2f;
    sun->i10 = 2.0f;
    sun->i11 = 4.0f;
    sun->i12 = 2.0f;
    sun->i13 = 3.0f;
    sun->i14 = 3.0f;
    sun->i16 = 0.1f;
    sun->i17 = 10.0f;
    sun->multiplier = 1.0f;
    sun->num = 32;
    sun->flags = 12;
    sun->alphaThresholdFlags = 0;
    sun->b1 = 0;
    sun->b2 = 1;
    GFX_Sun_ComputeRay(sun);
    GFX_Sun_InitRayLum(sun);
    sun->sampler = gpst_DX_stDD->p_SunSamplerCreate();
    sun->alphaThresholdFlags = -1;
    return sun;
}

/* Where on the screen to look for the sun, and which of the nine
   positions are on it at all. The positions walk out from the sun in
   a ring; the renderer is given the ones inside the viewport, packed,
   and told by mask which of the nine they were. */
static void GFX_l_SunQueueSample(void* sampler, tGdi_DisplayData* dd,
                                 tGfxSunData* sunData)
{
    int srcX[9];
    int srcY[9];
    Vec3 screen;
    Vec3 walk;
    float left;
    float right;
    float top;
    float bottom;
    int clip;
    int pointMask;
    int sampled;
    int i;

    clip = dd->ct.cam.viewPortRealPosX;
    left = (float)(clip < 0 ? 0 : clip);
    clip = dd->ct.cam.viewPortRealPosX + (unsigned int)dd->ct.cam.width;
    if (clip > dd->winWidth)
        clip = dd->winWidth;
    right = (float)clip;

    clip = dd->ct.cam.viewPortRealPosY;
    top = (float)(clip <= 0 ? 0 : clip);
    clip = dd->winHeight;
    if (clip > (int)(dd->ct.cam.viewPortRealPosY + (unsigned int)dd->ct.cam.height))
        clip = dd->ct.cam.viewPortRealPosY + (unsigned int)dd->ct.cam.height;
    bottom = (float)clip;

    SOFT_TransformAndProject(&screen, &sunData->v1, 1, &dd->ct.cam);
    MATH_TransformVertex(&walk, dd->ct.cam.viewMtxPtr_setInGRORender, &sunData->v1);

    pointMask = 0;
    sampled = 0;
    for (i = 0; i < 9; i++)
    {
        walk.x = gSunStru_951A40[i].x * sunData->multiplier + walk.x;
        walk.y = gSunStru_951A40[i].y * sunData->multiplier + walk.y;
        SOFT_Project(&screen, &walk, 1, &dd->ct);
        if (screen.x < (double)left || screen.x > (double)right
            || screen.y < (double)top || screen.y > (double)bottom)
            continue;

        pointMask |= 1 << i;
        srcX[sampled] = (unsigned int)screen.x;
        srcY[sampled] = (unsigned int)screen.y;
        sampled++;
    }

    /* walk.z is the sun's depth in camera space, which is what the
       renderer needs to know what counts as occluded. */
    dd->SunSampleQueue(sampler, srcX, srcY, sampled, pointMask, walk.z);
}

/* No fresh answer this frame, so last frame's is used again: the
   high half holds it and is copied back down. */
static void GFX_l_SunFlagsReuse(tGfxSunData* sun)
{
    sun->alphaThresholdFlags = sun->alphaThresholdFlags & 0xFFFF0000
                             | ((int)(sun->alphaThresholdFlags & 0xFFFF0000) >> 16);
}

static int GFX_l_SunHidden(void* sampler, tGdi_DisplayData* dd, tGfxSunData* sun)
{
    sun->flags |= 0x1000000u;
    sun->alphaThresholdFlags |= 0x1FF;
    dd->SunEffectOnOff(sampler, 0);
    return ++sunUnkCnt;
}

int GFX_Sun_UpdateRayLum_inner_sub_4A8A40(void* sampler, tGdi_DisplayData* dd,
                                          tGfxSunData* sun)
{
    Vec3 screen;
    int passed;

    // A renderer with too few texture units draws no flare, and the
    // tweak console can force the same. Either way it is the
    // renderer that answers.
    if (dd->b_WantsNoFlare())
        return GFX_l_SunHidden(sampler, dd, sun);

    if (!dd->b_SunSamplerReady(sampler))
    {
        GFX_l_SunFlagsReuse(sun);
        return ++sunUnkCnt;
    }

    // this frame's nine bits are made from scratch, with last
    // frame's kept in the high half in case none can be
    sun->alphaThresholdFlags <<= 16;
    SOFT_TransformAndProject(&screen, &sun->v1, 1, &dd->ct.cam);
    if (screen.z < 0.0 || sun->b1)
        return GFX_l_SunHidden(sampler, dd, sun);

    dd->SunEffectOnOff(sampler, 1);
    sun->flags &= ~0x1000000u;

    if (dd->b_SunSamplingNow(sampler))
    {
        GFX_l_SunQueueSample(sampler, dd, sun);
        if (dd->b_SunSampleRead(sampler, &passed))
        {
            sun->alphaThresholdFlags = (sun->alphaThresholdFlags & ~0x1FF)
                                     | (passed & 0x1FF);
            return ++sunUnkCnt;
        }
    }

    GFX_l_SunFlagsReuse(sun);
    return ++sunUnkCnt;
}

int GFX_Sun_UpdateRayLum(tGfxSunData* sun)
{
    return GFX_Sun_UpdateRayLum_inner_sub_4A8A40(sun->sampler, gpst_DX_stDD, sun);
}

void GFX_Sun_Render(struct sGfxSunData* sun)
{
    tGfxSunDataInner* ray;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3 toSun;
    Vec3 sunPos;
    Vec3 wide;
    Vec3 across;
    Vec3 farCentre;
    Vec3 moved;
    int* colors;
    double invDist;
    double centreScale;
    double self;
    double prev;
    double next;
    double rate;
    float targetLum[8];
    int rayColors[8];
    float movedLen;
    unsigned int color;
    int anyColor;
    int baseVertex;
    int flags;
    int k;

    if (sun->b1)
        sun->alphaThresholdFlags = (sun->alphaThresholdFlags << 16) | 0x1FF;
    else if ((sun->flags & 8) != 0)
        GFX_Sun_UpdateRayLum(sun);
    else
        sun->alphaThresholdFlags = 0;

    if ((sun->flags & 0x1000000) != 0)
        return;

    if ((sun->flags & 4) != 0)
    {
        sunPos = sun->v1;
    }
    else
    {
        MATH_SubstractVec(&toSun, &sun->v1, &gpst_DX_stDD->ct.cam.objectMtx.Pos);
        invDist = 1.0 / MATH_VecLen(&toSun);
        MATH_AddScaleVector(&sunPos, &gpst_DX_stDD->ct.cam.objectMtx.Pos, &toSun,
                            (float)(invDist * sun->i17));
    }

    GFX_NeedGeom(3 * sun->num, 3u, sun->num, 1);
    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.5;
    uvs[0].v = 0.0;
    uvs[1].u = 0.0;
    uvs[1].v = 1.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    colors = GFX_gpst_Geo->vertexColors + 1;
    verts = GFX_gpst_Geo->vertices;
    baseVertex = 0;

    // each octant is lit by its own sample at double weight and by its two
    // neighbours at single, and the centre sample halves the lot
    flags = sun->alphaThresholdFlags;
    centreScale = ((flags & 0x100) != 0) ? 0.5 : 1.0;
    for (k = 0; k < 8; k++)
    {
        self = ((flags & (1 << k)) != 0) ? 0.0 : 2.0;
        prev = ((flags & (1 << ((k + 7) & 7))) != 0) ? 0.0 : 1.0;
        next = ((flags & (1 << ((k + 1) & 7))) != 0) ? 0.0 : 1.0;
        targetLum[k] = (float)((self + prev + next) * 0.25 * centreScale);
    }

    MATH_SubstractVec(&moved, &sun->v2, &sun->v1);
    sun->v2 = sun->v1;
    movedLen = (float)sqrt(moved.x * moved.x + moved.z * moved.z + moved.y * moved.y);

    if (sun->b2)
    {
        rate = 1.0;
        sun->b2 = 0;
    }
    else if (movedLen == 0.0 || !(uint8_t)sun->alphaThresholdFlags
             || (uint8_t)sun->alphaThresholdFlags == 255)
    {
        rate = sun->i16 / TIM_gf_dt;
    }
    else
    {
        rate = sun->multiplier / movedLen;
    }

    anyColor = 0;
    for (k = 0; k < 8; k++)
    {
        sun->rayLums[k] = (targetLum[k] - sun->rayLums[k]) * (float)(1.0 / rate)
                        + sun->rayLums[k];
        if (sun->rayLums[k] > 1.0)
            sun->rayLums[k] = 1.0;
        else if (sun->rayLums[k] < 0.0)
            sun->rayLums[k] = 0.0;
        rayColors[k] = LIGHT_ul_Interpol2Colors(0, sun->color, sun->rayLums[k]);
        anyColor |= rayColors[k];
    }

    if (!anyColor)
        return;

    for (k = 0; k < sun->num; k++)
    {
        ray = &sun->elems[k];
        color = LIGHT_ul_Interpol2Colors(rayColors[ray->colorID], rayColors[ray->colorID2],
                                         ray->colorFactor);
        if (!color)
        {
            --GFX_gpst_Geo->triangleChunks->triangleNum;
            continue;
        }

        colors[0] = color;
        colors[1] = color;
        colors[2] = color;

        tris->v1 = baseVertex;
        tris->v2 = baseVertex + 1;
        tris->v3 = baseVertex + 2;
        tris->uv1 = 0;
        tris->uv2 = 1;
        tris->uv3 = 2;

        MATH_VecMultScalar(&wide, &gpst_DX_stDD->ct.cam.objectMtx.v1, ray->i1);
        MATH_AddScaleVector(&wide, &wide, &gpst_DX_stDD->ct.cam.objectMtx.v2, ray->i2);
        MATH_VecMultScalarWithoutBuffer(&wide, ray->i4);

        MATH_VecMultScalar(&across, &gpst_DX_stDD->ct.cam.objectMtx.v1, ray->i2);
        MATH_AddScaleVector(&across, &across, &gpst_DX_stDD->ct.cam.objectMtx.v2, -ray->i1);
        MATH_VecMultScalarWithoutBuffer(&across, ray->i5);

        MATH_SubstractVec(&verts[0], &sunPos, &wide);
        MATH_AddScaleVector(&farCentre, &sunPos, &wide, sun->i14);
        MATH_SubstractVec(&verts[1], &farCentre, &across);
        MATH_VecAddVec(&verts[2], &farCentre, &across);

        colors += 3;
        verts += 3;
        baseVertex += 3;
        tris++;
    }

    p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists;
    GFX_RenderGeom(0x9000900u);
}

void GFX_Sun_Seti(struct sGfxSunData* sun, int id)
{
    if (id == 18100)
        GFX_Sun_ComputeRay(sun);
}

void GFX_Sun_Destroy(struct sGfxSunData* sun)
{
    // A sun that has already been destroyed has nothing to hand
    // back. Worth the guard: before the renderer owned the surfaces,
    // a second destroy released them a second time.
    if (!sun || !sun->sampler)
        return;
    gpst_DX_stDD->SunSamplerDestroy(sun->sampler);
    sun->sampler = NULL;
}
