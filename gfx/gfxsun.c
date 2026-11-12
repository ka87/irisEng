#include "../memory.h"
#include "../time.h"
#include "../win/wingdi.h"
#include "gfxsun.h"

tD3dStrectRectUnkStructFloats gSunStru_951A40[] = {
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

tD3dStrectRectUnkStruct gSunDestPos[] = {
     {.x = 0, .y = 0 },
     {.x = 1, .y = 0 },
     {.x = 2, .y = 0 },
     {.x = 3, .y = 0 },
     {.x = 4, .y = 0 },
     {.x = 5, .y = 0 },
     {.x = 6, .y = 0 },
     {.x = 7, .y = 0 },
     {.x = 8, .y = 0 }
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

void D3D_InitSunD3DStruct(tGfxSunD3DSpecificData* sun3d, tGfxSunD3DSpecificDataInner* elems,
                          unsigned int bytes, unsigned int count)
{
    sun3d->ptr = elems;
    sun3d->maxElemNum = count;
    sun3d->ptrElemSize = bytes / count;
    sun3d->currPtrInner = NULL;
    sun3d->firstPtrInner = NULL;
}

void D3D_RegisterSunSurface(tGfxSunD3DSpecificData* sun3d)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (!gD3dSunSurfaces[i])
        {
            gD3dSunSurfaces[i] = sun3d;
            return;
        }
    }
}

void D3D_SetSunEffectOnOff(tGfxSunD3DSpecificData* sun3d, int on)
{
    int anyOn;
    int i;

    // gD3dSunSurfaces and gSunEffectOnOffArray are parallel sixteen-entry
    // tables, so both are walked by index
    anyOn = 0;
    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i] == sun3d)
            gSunEffectOnOffArray[i] = on;
        if (gD3dSunSurfaces[i])
            anyOn |= gSunEffectOnOffArray[i];
    }

    if ((anyOn != 0) == (D3DRenderParams.sunEffectIsOn != 0))
        return;

    D3DRenderParams.sunEffectIsOn = anyOn;
    D3D_UpdateBlurTexture();
}

void D3D_CreateSunD3DStuff(tGfxSunD3DSpecificData* sun3d)
{
    int i;

    D3D_InitSunD3DStruct(sun3d, sun3d->elems, sizeof(sun3d->elems), 3u);
    for (i = 0; i < 3; i++)
    {
        sun3d->elems[i].bitMask = 0;
        D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(
            D3DRenderParams.d3dDev, 16, 1, 21, 2, &sun3d->elems[i].d3dppSurface, 0);
    }
    D3D_RegisterSunSurface(sun3d);
    D3D_CreateSunTextures(sun3d);
    D3D_SetSunEffectOnOff(sun3d, 0);
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
    D3D_CreateSunD3DStuff(&sun->d3dStuff);
    sun->alphaThresholdFlags = -1;
    return sun;
}

tGfxSunD3DSpecificDataInner* D3D_GetGFXSunI3Param(tGfxSunD3DSpecificData* sun3d)
{
    return sun3d->currPtrInner;
}

tGfxSunD3DSpecificDataInner* D3D_GetGFXSunI4Param(tGfxSunD3DSpecificData* sun3d)
{
    return sun3d->firstPtrInner;
}

tGfxSunD3DSpecificDataInner* D3D_SelectNextPtrInnerFromPtr(tGfxSunD3DSpecificData* sun3d,
                                                           tGfxSunD3DSpecificDataInner* elem)
{
    unsigned int index;

    index = (unsigned int)((char*)elem - (char*)sun3d->ptr) / sun3d->ptrElemSize;
    return (tGfxSunD3DSpecificDataInner*)
           ((char*)sun3d->ptr + sun3d->ptrElemSize * ((index + 1) % sun3d->maxElemNum));
}

tGfxSunD3DSpecificDataInner* D3D_SelectPrevPtrInnerFromPtr(tGfxSunD3DSpecificData* sun3d,
                                                           tGfxSunD3DSpecificDataInner* elem)
{
    unsigned int index;

    index = (unsigned int)((char*)elem - (char*)sun3d->ptr) / sun3d->ptrElemSize;
    return (tGfxSunD3DSpecificDataInner*)
           ((char*)sun3d->ptr + sun3d->ptrElemSize * ((index - 1) % sun3d->maxElemNum));
}

tGfxSunD3DSpecificDataInner* D3D_SelectAndSetNextPtrInnerFromPtr(tGfxSunD3DSpecificData* sun3d)
{
    tGfxSunD3DSpecificDataInner* next;

    if (!sun3d->currPtrInner)
    {
        sun3d->firstPtrInner = sun3d->ptr;
        sun3d->currPtrInner = sun3d->ptr;
        return sun3d->ptr;
    }

    next = D3D_SelectNextPtrInnerFromPtr(sun3d, sun3d->currPtrInner);
    if (next == sun3d->firstPtrInner)
        return 0;

    sun3d->currPtrInner = next;
    return next;
}

int D3D_StretchRectNoFilter(IDirect3DSurface9* src, int srcX, int srcY,
                            IDirect3DSurface9* dest, int destX, int destY)
{
    RECT srcRect;
    RECT destRect;

    srcRect.left = srcX;
    srcRect.top = srcY;
    srcRect.right = srcX + 1;
    srcRect.bottom = srcY + 1;
    destRect.left = destX;
    destRect.top = destY;
    destRect.right = destX + 1;
    destRect.bottom = destY + 1;
    return D3DRenderParams.d3dDev->lpVtbl->StretchRect(D3DRenderParams.d3dDev, src, &srcRect,
                                                       dest, &destRect, 0);
}

void D3D_MultipleStretchWithArray(IDirect3DSurface9* src, tD3dStrectRectUnkStruct* srcPos,
                                  IDirect3DSurface9* dest, tD3dStrectRectUnkStruct* destPos,
                                  int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        // itt eredetileg distance alapján ment: v6 - srcPos + destPos
        D3D_StretchRectNoFilter(src, srcPos[i].x, srcPos[i].y,
                                dest, destPos[i].x, destPos[i].y);
    }
}

void D3d_GetRenderTargetData(IDirect3DSurface9* rendertarget, IDirect3DSurface9* surface)
{
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev,
                                                        rendertarget, surface);
}

void GFX_SunUnknownFunc(tGfxSunD3DSpecificData* sun3d, tGfxSunD3DSpecificDataInner* slot,
                        tGdi_DisplayData* dd, tGfxSunData* sunData)
{
    tD3dStrectRectUnkStruct srcPos[9];
    IDirect3DSurface9* renderTarget;
    Vec3 screen;
    Vec3 walk;
    float left;
    float right;
    float top;
    float bottom;
    int clip;
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

    slot->bitMask = 0;
    slot->alphaThreshold = (float)(uint8_t)D3D_DepthBlurBlurFuncCalc2(walk.z);

    sampled = 0;
    for (i = 0; i < 9; i++)
    {
        walk.x = gSunStru_951A40[i].x * sunData->multiplier + walk.x;
        walk.y = gSunStru_951A40[i].y * sunData->multiplier + walk.y;
        SOFT_Project(&screen, &walk, 1, &dd->ct);
        if (screen.x < (double)left || screen.x > (double)right
            || screen.y < (double)top || screen.y > (double)bottom)
        {
            slot->bitMask &= ~(1 << i);
        }
        else
        {
            slot->bitMask |= 1 << i;
            srcPos[sampled].x = (unsigned int)screen.x;
            srcPos[sampled].y = (unsigned int)screen.y;
            sampled++;
        }
    }

    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &renderTarget);
    D3D_MultipleStretchWithArray(renderTarget, srcPos, sun3d->d3dRenderTarget,
                                 gSunDestPos, sampled);
    renderTarget->lpVtbl->Release(renderTarget);
    D3d_GetRenderTargetData(sun3d->d3dRenderTarget, slot->d3dppSurface);
}

int D3D_SunGetPixelsFromSurface(IDirect3DSurface9* surface, tD3dStrectRectUnkStruct* pos,
                                int num, int* out)
{
    D3DLOCKED_RECT locked;
    D3DSURFACE_DESC desc;
    uint32_t* pixels;
    int i;

    surface->lpVtbl->GetDesc(surface, &desc);
    if (surface->lpVtbl->LockRect(surface, &locked, 0, 0xC010) == D3DERR_WASSTILLDRAWING)
        return 0;

    pixels = locked.pBits;
    for (i = 0; i < num; i++)
        out[i] = pixels[pos[i].x + desc.Width * pos[i].y];

    surface->lpVtbl->UnlockRect(surface);
    return 1;
}

void D3D_SunSetAlphaThresholdFlagBySurface(tGfxSunData* sun, tGfxSunD3DSpecificDataInner* slot)
{
    int pixels[9];
    int sampled;
    int i;

    D3D_SunGetPixelsFromSurface(slot->d3dppSurface, gSunDestPos, 9, pixels);

    sampled = 0;
    for (i = 0; i < 9; i++)
    {
        if ((slot->bitMask & (1 << i)) == 0)
        {
            sun->alphaThresholdFlags |= 1 << i;
            continue;
        }

        if ((double)((pixels[sampled] >> 24) & 0xff) <= slot->alphaThreshold)
            sun->alphaThresholdFlags &= ~(1 << i);
        else
            sun->alphaThresholdFlags |= 1 << i;
        sampled++;
    }
}

void  D3D_SelectAndSetFirstPtrInnerFromPtr(tGfxSunD3DSpecificData* sun3d,
                                           tGfxSunD3DSpecificDataInner* elem)
{
    if (elem == sun3d->currPtrInner)
    {
        sun3d->currPtrInner = NULL;
        sun3d->firstPtrInner = NULL;
    }
    else
    {
        sun3d->firstPtrInner = D3D_SelectNextPtrInnerFromPtr(sun3d, elem);
    }
}

void D3D_GfxSunDataI21ShortSwap(tGfxSunData* sun)
{
    sun->alphaThresholdFlags = sun->alphaThresholdFlags & 0xFFFF0000
                             | ((int)(sun->alphaThresholdFlags & 0xFFFF0000) >> 16);
}

int D3D_GetDepthBlurType(void)
{
    return depthblur_Type;
}

int D3D_SurfaceTestLockRect(IDirect3DSurface9* surface)
{
    D3DLOCKED_RECT locked;

    if (surface->lpVtbl->LockRect(surface, &locked, 0, 0xC010) == D3DERR_WASSTILLDRAWING)
        return 0;
    surface->lpVtbl->UnlockRect(surface);
    return 1;
}

int calls_D3D_SurfaceTestLockRect(tGfxSunD3DSpecificDataInner* slot)
{
    return D3D_SurfaceTestLockRect(slot->d3dppSurface);
}

static int GFX_l_SunHidden(tGfxSunD3DSpecificData* sun3d, tGfxSunData* sun)
{
    sun->flags |= 0x1000000u;
    sun->alphaThresholdFlags |= 0x1FF;
    D3D_SetSunEffectOnOff(sun3d, 0);
    return ++sunUnkCnt;
}

int GFX_Sun_UpdateRayLum_inner_sub_4A8A40(tGfxSunD3DSpecificData* sun3d, tGdi_DisplayData* dd,
                                          tGfxSunData* sun)
{
    tGfxSunD3DSpecificDataInner* pending;
    tGfxSunD3DSpecificDataInner* ready;
    Vec3 screen;

    if (D3DRenderParams.tweak_ae_noflare)
        return GFX_l_SunHidden(sun3d, sun);

    if (!sun3d->d3dRenderTarget)
    {
        D3D_GfxSunDataI21ShortSwap(sun);
        return ++sunUnkCnt;
    }

    sun->alphaThresholdFlags <<= 16;
    SOFT_TransformAndProject(&screen, &sun->v1, 1, &dd->ct.cam);
    if (screen.z < 0.0 || sun->b1)
        return GFX_l_SunHidden(sun3d, sun);

    D3D_SetSunEffectOnOff(sun3d, 1);
    sun->flags &= ~0x1000000u;

    if (D3D_GetDepthBlurType() == 4)
    {
        pending = D3D_GetGFXSunI3Param(sun3d);
        if (D3D_SelectAndSetNextPtrInnerFromPtr(sun3d))
            GFX_SunUnknownFunc(sun3d, D3D_GetGFXSunI3Param(sun3d), dd, sun);

        // the newest surface the hardware has finished with, walking back
        // from the one just queued
        ready = 0;
        if (pending)
        {
            if (calls_D3D_SurfaceTestLockRect(pending))
            {
                ready = pending;
            }
            else
            {
                while (pending != D3D_GetGFXSunI4Param(sun3d))
                {
                    pending = D3D_SelectPrevPtrInnerFromPtr(sun3d, pending);
                    if (calls_D3D_SurfaceTestLockRect(pending))
                    {
                        ready = pending;
                        break;
                    }
                }
            }
        }

        if (ready)
        {
            D3D_SunSetAlphaThresholdFlagBySurface(sun, ready);
            D3D_SelectAndSetFirstPtrInnerFromPtr(sun3d, ready);
            return ++sunUnkCnt;
        }
    }

    D3D_GfxSunDataI21ShortSwap(sun);
    return ++sunUnkCnt;
}

int GFX_Sun_UpdateRayLum(tGfxSunData* sun)
{
    return GFX_Sun_UpdateRayLum_inner_sub_4A8A40(&sun->d3dStuff, gpst_DX_stDD, sun);
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

void D3D_UnsetSunSurface(tGfxSunD3DSpecificData* sun3d)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gD3dSunSurfaces[i] == sun3d)
        {
            gD3dSunSurfaces[i] = NULL;
            return;
        }
    }
}

void GFX_Sun_Destroy(struct sGfxSunD3DSpecificData* sun3d)
{
    int i;

    D3D_SetSunEffectOnOff(sun3d, 0);
    D3D_ReleaseSunRenderTargetAndTexture(sun3d);
    D3D_UnsetSunSurface(sun3d);
    for (i = 0; i < 3; i++)
        sun3d->elems[i].d3dppSurface->lpVtbl->Release(sun3d->elems[i].d3dppSurface);
}
