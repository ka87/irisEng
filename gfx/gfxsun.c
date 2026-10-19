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

void GFX_Sun_ComputeRay(tGfxSunData* a1)
{
    int v2;
    tGfxSunDataInner* v3;
    tGfxSunDataInner* v4;
    tGfxSunDataInner* v5;
    double v6;
    signed __int8 v7;
    tGfxSunDataInner* v10;
    tGfxSunDataInner* v11;
    tGfxSunDataInner* v12;
    double v13;
    signed __int8 v14;
    tGfxSunDataInner* v17;
    tGfxSunDataInner* v18;
    tGfxSunDataInner* v19;
    tGfxSunDataInner* v20;
    int v21;
    int v22;
    tGfxSunDataInner* v23;
    tGfxSunDataInner* v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    tGfxSunDataInner* a1a;
    tGfxSunDataInner* a1b;

    v2 = a1->num;
    if ((v2 & 1) != 0)
        a1->num = v2 & ~1u;
    if (a1->num > 128)
        a1->num = 128;
    v21 = 0;
    v25 = 0.0;
    v42 = (float)(6.2831855 / (double)(a1->num >> 1));
    if (a1->num > 0)
    {
        v23 = a1->elems;
        v19 = a1->elems;
        v17 = a1->elems;
        a1a = a1->elems;
        v3 = a1->elems;
        v4 = a1->elems;
        v5 = a1->elems;
        do
        {
            v36 = a1->i4;
            v26 = -v36;
            v6 = (double)rand() * 0.000030518509 * (v36 - v26) + v26 + v25;
            if (v6 < 0.0)
                v6 = v6 + 6.2831855;
            v7 = (unsigned __int64)(1.2732395 * v6) & 7;
            v5->colorID = v7;
            v4->colorID2 = (v7 + 1) & 7;
            v3->colorFactor = (float)(1.2732395 * v6 - (double)v7);
            a1a->i1 = MATH_GetCos((float)v6);
            v17->i2 = MATH_GetSin((float)v6);
            v37 = a1->i6;
            v28 = a1->i5;
            v19->i4 = (float)((double)rand() * 0.000030518509 * (v37 - v28) + v28);
            v38 = a1->i8;
            v29 = a1->i7;
            v22 = v21 + 1;
            a1b = a1a + 1;
            v18 = v17 + 1;
            v10 = v5 + 1;
            v23->i5 = (float)((double)rand() * 0.000030518509 * (v38 - v29) + v29);
            v11 = v4 + 1;
            v12 = v3 + 1;
            v27 = -a1->i9;
            v20 = v19 + 1;
            v24 = v23 + 1;
            v39 = a1->i9;
            v13 = (double)rand() * 0.000030518509 * (v39 - v27) + v27 + v25;
            if (v13 < 0.0)
                v13 = v13 + 6.2831855;
            v14 = (unsigned int)(1.2732395 * v13) & 7;
            v10->colorID = v14;
            v11->colorID2 = (v14 + 1) & 7;
            v12->colorFactor = (float)(1.2732395 * v13 - (double)v14);
            a1b->i1 = MATH_GetCos((float)v13);
            v18->i2 = MATH_GetSin((float)v13);
            v40 = a1->i11;
            v30 = a1->i10;
            v20->i4 = (float)((double)rand() * 0.000030518509 * (v40 - v30) + v30);
            v41 = a1->i13;
            v31 = a1->i12;
            a1a = a1b + 1;
            v17 = v18 + 1;
            v23 = v24 + 1;
            v5 = v10 + 1;
            v4 = v11 + 1;
            v23[-1].i5 = (float)((double)rand() * 0.000030518509 * (v41 - v31) + v31);
            v3 = v12 + 1;
            v21 = v22 + 1;
            v25 = v25 + v42;
            v19 = v20 + 1;
        } while (v21 < a1->num);
    }
}

void GFX_Sun_InitRayLum(tGfxSunData* a1)
{
    char v2;
    float a1a;
    int i;

    v2 = a1->flags;
    a1a = 0.0;
    if ((v2 & 8) == 0)
        a1a = 1.0;
    for (i = 0; i < 8; i++) {
        a1->rayLums[i] = a1a;
    }
}

void D3D_InitSunD3DStruct(tGfxSunD3DSpecificData* a1, tGfxSunD3DSpecificDataInner* a2, unsigned int a3, unsigned int a4)
{
    a1->ptr = a2;
    a1->maxElemNum = a4;
    a1->ptrElemSize = a3 / a4;
    a1->currPtrInner = NULL;
    a1->firstPtrInner = NULL;
}

void D3D_RegisterSunSurface(tGfxSunD3DSpecificData* a1)
{
    int v1;

    v1 = 0;
    while (gD3dSunSurfaces[v1])
    {
        if (++v1 >= 16)
            return;
    }
    gD3dSunSurfaces[v1] = a1;
}

void D3D_SetSunEffectOnOff(tGfxSunD3DSpecificData* a1, int a2)
{
    int v2;
    int* v3;
    tGfxSunD3DSpecificData* v4;

    v2 = 0;
    v3 = gSunEffectOnOffArray;
    v4 = gD3dSunSurfaces[0];
    do
    {
        if (v4 == a1)
            *v3 = a2;
        if (v4)
            v2 |= *v3;
        ++v3;
        ++v4;
    } while ((int)v3 <= (int)&gSunEffectOnOffArray[15]);
    if (v2)
    {
        if (D3DRenderParams.sunEffectIsOn)
            return;
    LABEL_11:
        D3DRenderParams.sunEffectIsOn = v2;
        D3D_UpdateBlurTexture();
        return;
    }
    if (D3DRenderParams.sunEffectIsOn)
        goto LABEL_11;
}

void D3D_CreateSunD3DStuff(tGfxSunD3DSpecificData* a1)
{
    tGfxSunD3DSpecificDataInner* v1;
    int v2;

    v1 = a1->elems;
    D3D_InitSunD3DStruct(a1, a1->elems, sizeof(a1->elems), 3u);
    v2 = 3;
    do
    {
        v1->bitMask = 0;
        D3DRenderParams.d3dDev->lpVtbl->CreateOffscreenPlainSurface(D3DRenderParams.d3dDev, 16, 1, 21, 2, &(v1->d3dppSurface), 0);
        v1++;
        --v2;
    } while (v2);
    D3D_RegisterSunSurface(a1);
    D3D_CreateSunTextures(a1);
    D3D_SetSunEffectOnOff(a1, 0);
}

struct sGfxSunData* GFX_Sun_Create()
{
    tGfxSunData* v0;

    v0 = (tGfxSunData*)MEM_p_Alloc(sizeof(tGfxSunData));
    memset(v0, 0, sizeof(tGfxSunData));
    v0->v1.x = 0.0f;
    v0->v1.y = 0.0f;
    v0->v1.z = 0.0f;
    v0->v2.x = 0.0f;
    v0->v2.y = 0.0f;
    v0->v2.z = 0.0f;
    v0->i4 = 0.2f;
    v0->i9 = 0.2f;
    v0->i6 = 15.0f;
    v0->i5 = 10.0;
    v0->i8 = 2.0f;
    v0->i7 = 1.0f;
    v0->i11 = 4.0f;
    v0->i10 = 2.0f;
    v0->i13 = 3.0f;
    v0->i12 = 2.0f;
    v0->i14 = 3.0f;
    v0->i16 = 0.1f;
    v0->multiplier = 1.0f;
    v0->i17 = 10.0f;
    v0->num = 32;
    v0->flags = 12;
    v0->alphaThresholdFlags = 0;
    v0->b1 = 0;
    v0->b2 = 1;
    GFX_Sun_ComputeRay(v0);
    GFX_Sun_InitRayLum(v0);
    D3D_CreateSunD3DStuff(&v0->d3dStuff);
    v0->alphaThresholdFlags = -1;
    return v0;
}

tGfxSunD3DSpecificDataInner* D3D_GetGFXSunI3Param(tGfxSunD3DSpecificData* a1)
{
    return a1->currPtrInner;
}

tGfxSunD3DSpecificDataInner* D3D_GetGFXSunI4Param(tGfxSunD3DSpecificData* a1)
{
    return a1->firstPtrInner;
}

tGfxSunD3DSpecificDataInner* D3D_SelectNextPtrInnerFromPtr(tGfxSunD3DSpecificData* a1, tGfxSunD3DSpecificDataInner* a2)
{
    return (tGfxSunD3DSpecificDataInner*)((char*)a1->ptr + a1->ptrElemSize * (((unsigned int)((char*)a2 - (char*)a1->ptr) / a1->ptrElemSize + 1) % a1->maxElemNum));
}

tGfxSunD3DSpecificDataInner* D3D_SelectPrevPtrInnerFromPtr(tGfxSunD3DSpecificData* a1, tGfxSunD3DSpecificDataInner* a2)
{
    return (tGfxSunD3DSpecificDataInner*)((char*)a1->ptr + a1->ptrElemSize * (((unsigned int)((char*)a2 - (char*)a1->ptr) / a1->ptrElemSize - 1)  % a1->maxElemNum));
}

tGfxSunD3DSpecificDataInner* D3D_SelectAndSetNextPtrInnerFromPtr(tGfxSunD3DSpecificData* a1)
{
    tGfxSunD3DSpecificDataInner* result;

    if (!a1->currPtrInner)
    {
        result = a1->ptr;
        a1->firstPtrInner = result;
    LABEL_3:
        a1->currPtrInner = result;
        return result;
    }
    result = D3D_SelectNextPtrInnerFromPtr(a1, a1->currPtrInner);
    if (result != a1->firstPtrInner)
        goto LABEL_3;
    return 0;
}

int D3D_StretchRectNoFilter(IDirect3DSurface9* src, int srcX, int srcY, IDirect3DSurface9* dest, int destX, int destY)
{
    RECT v7;
    RECT v8;

    v8.left = srcX;
    v8.top = srcY;
    v8.right = srcX + 1;
    v8.bottom = srcY + 1;
    v7.left = destX;
    v7.top = destY;
    v7.right = destX + 1;
    v7.bottom = destY + 1;
    return D3DRenderParams.d3dDev->lpVtbl->StretchRect(D3DRenderParams.d3dDev, src, &v8, dest, &v7, 0);
}

void D3D_MultipleStretchWithArray(IDirect3DSurface9* a1, tD3dStrectRectUnkStruct* srcPos, IDirect3DSurface9* a3, tD3dStrectRectUnkStruct* destPos, int num)
{
    int v5;
    tD3dStrectRectUnkStruct* v6;
    int i;

    v5 = num;
    if (num > 0)
    {
        i = 0;
        v6 = srcPos;
        do
        {
            // itt eredetileg distance alapján ment: v6 - srcPos + destPos
            D3D_StretchRectNoFilter(a1, v6->x, v6->y, a3, destPos[i].x, destPos[i].y);
            ++i;
            ++v6;
            --v5;
        } while (v5);
    }
}

void D3d_GetRenderTargetData(IDirect3DSurface9* rendertarget, IDirect3DSurface9* surface)
{
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTargetData(D3DRenderParams.d3dDev, rendertarget, surface);
}

void GFX_SunUnknownFunc(tGfxSunD3DSpecificData* a1, tGfxSunD3DSpecificDataInner* a2, tGdi_DisplayData* a3, tGfxSunData* sunData)
{
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    double v10;
    unsigned int v11;
    double v12;
    float v13;
    int v14;
    tDDContainerStruct* a4a; 
    float v16;
    float v17;
    float v18;
    float v19;
    Vec3 a1a;
    Vec3 a2a;
    tD3dStrectRectUnkStruct srcPos[9];
    IDirect3DSurface9* v14a;

    v4 = a3->winWidth;
    v5 = a3->ct.cam.viewPortRealPosX;
    v14 = v5 < 0 ? 0 : v5;
    v16 = (float)v14;
    v14 = v5 + (unsigned int)a3->ct.cam.width;
    if (v14 > v4)
        v14 = v4;
    v6 = a3->ct.cam.viewPortRealPosY;
    v17 = (float)v14;
    v14 = v6 <= 0 ? 0 : v6;
    v18 = (float)v14;
    v7 = v6 + (unsigned int)a3->ct.cam.height;
    v14 = a3->winHeight;
    if (v14 > v7)
        v14 = v7;
    v19 = (float)v14;
    a4a = &a3->ct;
    SOFT_TransformAndProject(&a1a, &sunData->v1, 1, &a3->ct.cam);
    MATH_TransformVertex(&a2a, a3->ct.cam.viewMtxPtr_setInGRORender, &sunData->v1);
    v8 = 0;
    v13 = a2a.z;
    v9 = 0;
    a2->bitMask = 0;
    v14 = (uint8_t)D3D_DepthBlurBlurFuncCalc2(v13);
    a2->alphaThreshold = (float)v14;
    do
    {
        a2a.x = gSunStru_951A40[v8].x * sunData->multiplier + a2a.x;
        a2a.y = gSunStru_951A40[v8].y * sunData->multiplier + a2a.y;
        SOFT_Project(&a1a, &a2a, 1, a4a);
        if (a1a.x < (double)v16 || a1a.x >(double)v17 || a1a.y < (double)v18 || a1a.y >(double)v19)
        {
            a2->bitMask &= ~(1 << v8);
        }
        else
        {
            v10 = a1a.x;
            a2->bitMask |= 1 << v8;
            v11 = (unsigned int)v10;
            v12 = a1a.y;
            srcPos[v9].x = v11;
            srcPos[v9++].y = (unsigned int)v12;
        }
        ++v8;
    } while (v8 < 9);
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &v14a);
    D3D_MultipleStretchWithArray(v14a, srcPos, a1->d3dRenderTarget, gSunDestPos, v9);
    v14a->lpVtbl->Release(v14a);
    D3d_GetRenderTargetData(a1->d3dRenderTarget, a2->d3dppSurface);
}

int D3D_SunGetPixelsFromSurface(IDirect3DSurface9* a1, tD3dStrectRectUnkStruct* a2, int a3, int* a4)
{
    IDirect3DSurface9* v4;
    int v6;
    uint32_t* v7;
    UINT v8;
    int v9;
    D3DLOCKED_RECT v10;
    D3DSURFACE_DESC v11;

    v4 = a1;
    a1->lpVtbl->GetDesc(a1, &v11);
    if (v4->lpVtbl->LockRect(v4, &v10, 0, 0xC010) == D3DERR_WASSTILLDRAWING)
        return 0;
    v6 = 0;
    if (a3 > 0)
    {
        v7 = v10.pBits;
        v8 = v11.Width;
        do
        {
            v9 = a2[v6].x + v8 * a2[v6].y;
            a4[v6++] = v7[v9];
        } while (v6 < a3);
        v4 = a1;
    }
    v4->lpVtbl->UnlockRect(v4);
    return 1;
}

void D3D_SunSetAlphaThresholdFlagBySurface(tGfxSunData* a1, tGfxSunD3DSpecificDataInner* a2)
{
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9; 
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;
    int v17;
    int v18;
    int v19;
    int v20[9];

    D3D_SunGetPixelsFromSurface(a2->d3dppSurface, gSunDestPos, 9, v20);
    v2 = 0;
    if ((a2->bitMask & 1) != 0)
    {
        v3 = a1->alphaThresholdFlags;
        if ((double)((v20[0] >> 24) & 0xff) <= a2->alphaThreshold)
            v4 = v3 & ~1u;
        else
            v4 = v3 | 1;
        v2 = 1;
    }
    else
    {
        v4 = a1->alphaThresholdFlags | 1;
    }
    a1->alphaThresholdFlags = v4;
    if ((a2->bitMask & 2) != 0)
    {
        v5 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v6 = v5 & ~2u;
        else
            v6 = v5 | 2;
        ++v2;
    }
    else
    {
        v6 = a1->alphaThresholdFlags | 2;
    }
    a1->alphaThresholdFlags = v6;
    if ((a2->bitMask & 4) != 0)
    {
        v7 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v8 = v7 & ~4u;
        else
            v8 = v7 | 4;
        ++v2;
    }
    else
    {
        v8 = a1->alphaThresholdFlags | 4;
    }
    a1->alphaThresholdFlags = v8;
    if ((a2->bitMask & 8) != 0)
    {
        v9 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v10 = v9 & ~8u;
        else
            v10 = v9 | 8;
        ++v2;
    }
    else
    {
        v10 = a1->alphaThresholdFlags | 8;
    }
    a1->alphaThresholdFlags = v10;
    if ((a2->bitMask & 0x10) != 0)
    {
        v11 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v12 = v11 & ~0x10u;
        else
            v12 = v11 | 0x10;
        ++v2;
    }
    else
    {
        v12 = a1->alphaThresholdFlags | 0x10;
    }
    a1->alphaThresholdFlags = v12;
    if ((a2->bitMask & 0x20) != 0)
    {
        v13 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v14 = v13 & ~0x20u;
        else
            v14 = v13 | 0x20;
        ++v2;
    }
    else
    {
        v14 = a1->alphaThresholdFlags | 0x20;
    }
    a1->alphaThresholdFlags = v14;
    if ((a2->bitMask & 0x40) != 0)
    {
        v15 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v16 = v15 & ~0x40u;
        else
            v16 = v15 | 0x40;
        ++v2;
    }
    else
    {
        v16 = a1->alphaThresholdFlags | 0x40;
    }
    a1->alphaThresholdFlags = v16;
    if ((a2->bitMask & 0x80) == 0)
    {
        v18 = a1->alphaThresholdFlags | 0x80;
    }
    else
    {
        v17 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
            v18 = v17 & ~0x80u;
        else
            v18 = v17 | 0x80;
        ++v2;
    }
    a1->alphaThresholdFlags = v18;
    if ((a2->bitMask & 0x100) != 0)
    {
        v19 = a1->alphaThresholdFlags;
        if ((double)((v20[v2] >> 24) & 0xff) <= a2->alphaThreshold)
        {
            a1->alphaThresholdFlags = v19 & ~0x100u;
            return;
        }
    }
    else
    {
        v19 = a1->alphaThresholdFlags;
    }
    a1->alphaThresholdFlags = v19 | 0x100;
}

void  D3D_SelectAndSetFirstPtrInnerFromPtr(tGfxSunD3DSpecificData* a1, tGfxSunD3DSpecificDataInner* a2)
{
    if (a2 == a1->currPtrInner)
    {
        a1->currPtrInner = NULL;
        a1->firstPtrInner = NULL;
    }
    else
    {
        a1->firstPtrInner = D3D_SelectNextPtrInnerFromPtr(a1, a2);
    }
}

void D3D_GfxSunDataI21ShortSwap(tGfxSunData* a1)
{
    a1->alphaThresholdFlags = a1->alphaThresholdFlags & 0xFFFF0000 | ((int)(a1->alphaThresholdFlags & 0xFFFF0000) >> 16);
}

int D3D_GetDepthBlurType()
{
    return depthblur_Type;
}

int D3D_SurfaceTestLockRect(IDirect3DSurface9* a1)
{
    D3DLOCKED_RECT v2;

    if (a1->lpVtbl->LockRect(a1, &v2, 0, 0xC010) == D3DERR_WASSTILLDRAWING)
        return 0;
    a1->lpVtbl->UnlockRect(a1);
    return 1;
}

int calls_D3D_SurfaceTestLockRect(tGfxSunD3DSpecificDataInner* a1)
{
    return D3D_SurfaceTestLockRect(a1->d3dppSurface);
}

int GFX_Sun_UpdateRayLum_inner_sub_4A8A40(tGfxSunD3DSpecificData* a1, tGdi_DisplayData* a2, tGfxSunData* a3)
{
    int v3;
    tGfxSunD3DSpecificDataInner* v5;
    tGfxSunD3DSpecificDataInner* v6;
    int v7;
    Vec3 a1a;

    if (D3DRenderParams.tweak_ae_noflare)
    {
        v3 = a3->alphaThresholdFlags | 0x1FF;
        a3->flags |= 0x1000000u;
        a3->alphaThresholdFlags = v3;
        D3D_SetSunEffectOnOff(a1, 0);
        return ++sunUnkCnt;
    }
    if (a1->d3dRenderTarget)
    {
        a3->alphaThresholdFlags <<= 16;
        SOFT_TransformAndProject(&a1a, &a3->v1, 1, &a2->ct.cam);
        if (a1a.z < 0.0 || a3->b1)
        {
            v7 = a3->alphaThresholdFlags | 0x1FF;
            a3->flags |= 0x1000000u;
            a3->alphaThresholdFlags = v7;
            D3D_SetSunEffectOnOff(a1, 0);
            return ++sunUnkCnt;
        }
        D3D_SetSunEffectOnOff(a1, 1);
        a3->flags &= ~0x1000000u;
        if (D3D_GetDepthBlurType() == 4)
        {
            v5 = D3D_GetGFXSunI3Param(a1);
            if (D3D_SelectAndSetNextPtrInnerFromPtr(a1))
            {
                v6 = D3D_GetGFXSunI3Param(a1);
                GFX_SunUnknownFunc(a1, v6, a2, a3);
            }
            if (v5)
            {
                if (calls_D3D_SurfaceTestLockRect(v5))
                {
                LABEL_13:
                    D3D_SunSetAlphaThresholdFlagBySurface(a3, v5);
                    D3D_SelectAndSetFirstPtrInnerFromPtr(a1, v5);
                    return ++sunUnkCnt;
                }
                while (v5 != D3D_GetGFXSunI4Param(a1))
                {
                    v5 = D3D_SelectPrevPtrInnerFromPtr(a1, v5);
                    if (calls_D3D_SurfaceTestLockRect(v5))
                        goto LABEL_13;
                }
            }
        }
        D3D_GfxSunDataI21ShortSwap(a3);
    }
    else
    {
        D3D_GfxSunDataI21ShortSwap(a3);
    }
    return ++sunUnkCnt;
}

int GFX_Sun_UpdateRayLum(tGfxSunData* a1)
{
    return GFX_Sun_UpdateRayLum_inner_sub_4A8A40(&a1->d3dStuff, gpst_DX_stDD, a1);
}

void GFX_Sun_Render(struct sGfxSunData* a1)
{
    int v1;
    int v2;
    double v3;
    double v4;
    long double v5;
    double v6;
    int v7;
    tGeoTriangleChunkElem* v8;
    int* vertexColors;
    double v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    double v24;
    double v25;
    double v26;
    double v27;
    double v28;
    double v29;
    int v30; 
    double v31;
    double v32;
    double v33;
    int v34; 
    double v35;
    double v36;
    double v37;
    double v38;
    double v39;
    double v40;
    double v41;
    double v42;
    double v43;
    double v44;
    float v45; 
    double v46;
    double v47;
    double v48;
    double v49;
    float* v50;
    unsigned int v51;
    int v52;
    Vec3* v53;
    tGfxSunDataInner *v54;
    unsigned int v55;
    int* v56;
    uint16_t v57;
    double v58;
    double v59;
    double v60;
    double v61;
    double v62;
    double v63;
    double v64;
    double v65;
    int v66; 
    int v67; 
    float v68;
    float v69;
    float v70;
    float v71;
    int v72;
    float v73;
    float v74;
    float v75;
    float v76;
    float v77;
    float v78;
    float v79;
    float v80;
    int v81; 
    float v82;
    int v83; 
    float v84;
    float v85;
    float v86;
    float v87;
    float v88;
    float v89;
    float v90;
    float v91;
    float v92;
    float v93;
    float v94;
    float v95;
    float v96;
    Vec3* v97;
    float v98;
    float v99;
    float v100;
    float v101;
    float v102;
    float v103;
    float v104;
    float v105;
    float v106;
    float v107;
    float v108;
    float v109;
    float v110;
    float v111;
    float v112;
    float v113;
    float v114[8];
    int v115[8];

    v1 = 0;
    if (a1->b1)
    {
        a1->alphaThresholdFlags = (a1->alphaThresholdFlags << 16) | 0x1FF;
    }
    else if ((a1->flags & 8) != 0)
    {
        GFX_Sun_UpdateRayLum(a1);
    }
    else
    {
        a1->alphaThresholdFlags = 0;
    }
    v2 = a1->flags;
    if ((v2 & 0x1000000) == 0)
    {
        if ((v2 & 4) != 0)
        {
            v76 = a1->v1.x;
            v78 = a1->v1.y;
            v80 = a1->v1.z;
        }
        else
        {
            v3 = a1->v1.x - gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
            v4 = a1->v1.y - gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
            v79 = a1->v1.z - gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
            v5 = 1.0 / sqrt(v4 * v4 + v3 * v3 + v79 * v79);
            v71 = (float)v5;
            v74 = (float)(v5 * v3);
            v6 = a1->i17;
            v75 = (float)(v74 * v6);
            v77 = (float)(v4 * v71 * v6);
            v76 = v75 + gpst_DX_stDD->ct.cam.objectMtx.Pos.x;
            v78 = v77 + gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
            v80 = (float)(v6 * (v79 * v71) + gpst_DX_stDD->ct.cam.objectMtx.Pos.z);
        }
        GFX_NeedGeom(3 * a1->num, 3u, a1->num, 1);
        GFX_gpst_Geo->UVs->u = 0.5;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 0.0;
        GFX_gpst_Geo->UVs[1].v = 1.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        v7 = a1->alphaThresholdFlags;
        v8 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        vertexColors = GFX_gpst_Geo->vertexColors + 1;
        v97 = GFX_gpst_Geo->vertices;
        if ((v7 & 0x100) != 0)
            v10 = 0.5;
        else
            v10 = 1.0;
        v67 = a1->alphaThresholdFlags & 1;
        if ((v7 & 1) != 0)
            v11 = 0.0;
        else
            v11 = 2.0;
        v81 = a1->alphaThresholdFlags & 0x80;
        if ((v7 & 0x80) != 0)
            v12 = 0.0;
        else
            v12 = 1.0;
        v13 = v11 + v12;
        if ((v7 & 2) != 0)
            v14 = 0.0;
        else
            v14 = 1.0;
        v114[0] = (float)((v13 + v14) * 0.25 * v10);
        if ((v7 & 2) != 0)
            v15 = 0.0;
        else
            v15 = 2.0;
        if ((v7 & 1) != 0)
            v16 = 0.0;
        else
            v16 = 1.0;
        v17 = v15 + v16;
        if ((v7 & 4) != 0)
            v18 = 0.0;
        else
            v18 = 1.0;
        v114[1] = (float)((v17 + v18) * 0.25 * v10);
        if ((v7 & 4) != 0)
            v19 = 0.0;
        else
            v19 = 2.0;
        if ((v7 & 2) != 0)
            v20 = 0.0;
        else
            v20 = 1.0;
        v21 = v19 + v20;
        if ((v7 & 8) != 0)
            v22 = 0.0;
        else
            v22 = 1.0;
        v114[2] = (float)((v21 + v22) * 0.25 * v10);
        if ((v7 & 8) != 0)
            v23 = 0.0;
        else
            v23 = 2.0;
        if ((v7 & 4) != 0)
            v24 = 0.0;
        else
            v24 = 1.0;
        v25 = v23 + v24;
        if ((v7 & 0x10) != 0)
            v26 = 0.0;
        else
            v26 = 1.0;
        v114[3] = (float)((v25 + v26) * 0.25 * v10);
        if ((v7 & 0x10) != 0)
            v27 = 0.0;
        else
            v27 = 2.0;
        if ((v7 & 8) != 0)
            v28 = 0.0;
        else
            v28 = 1.0;
        v29 = v27 + v28;
        v30 = v7 & 0x20;
        if ((v7 & 0x20) != 0)
            v31 = 0.0;
        else
            v31 = 1.0;
        v114[4] = (float)((v29 + v31) * 0.25 * v10);
        if ((v7 & 0x20) != 0)
            v32 = 0.0;
        else
            v32 = 2.0;
        if ((v7 & 0x10) != 0)
            v33 = 0.0;
        else
            v33 = 1.0;
        v34 = v7 & 0x40;
        v35 = v32 + v33;
        if (v34)
            v36 = 0.0;
        else
            v36 = 1.0;
        v114[5] = (float)((v35 + v36) * 0.25 * v10);
        if (v34)
            v37 = 0.0;
        else
            v37 = 2.0;
        if (v30)
            v38 = 0.0;
        else
            v38 = 1.0;
        v39 = v37 + v38;
        if (v81)
            v40 = 0.0;
        else
            v40 = 1.0;
        v114[6] = (float)((v39 + v40) * 0.25 * v10);
        if (v81)
            v41 = 0.0;
        else
            v41 = 2.0;
        if (v34)
            v42 = 0.0;
        else
            v42 = 1.0;
        v43 = v41 + v42;
        if (v67)
            v44 = 0.0;
        else
            v44 = 1.0;
        v45 = a1->v1.x;
        v114[7] = (float)((v43 + v44) * 0.25 * v10);
        v46 = a1->v2.y - a1->v1.y;
        v47 = a1->v2.z - a1->v1.z;
        v48 = a1->v2.x - a1->v1.x;
        a1->v2.x = v45;
        a1->v2.y = a1->v1.y;
        a1->v2.z = a1->v1.z;
        v68 = (float)(sqrt(v48 * v48 + v47 * v47 + v46 * v46));
        if (a1->b2)
        {
            v49 = 1.0;
            a1->b2 = 0;
        }
        else if (v68 == 0.0 || !(uint8_t)a1->alphaThresholdFlags || (uint8_t)a1->alphaThresholdFlags == 255)
        {
            v49 = a1->i16 / TIM_gf_dt;
        }
        else
        {
            v49 = a1->multiplier / v68;
        }
        v72 = 0;
        v50 = a1->rayLums;
        do
        {
            v82 = (float)(1.0 / v49);
            v69 = (v114[v1] - *v50) * v82 + *v50;
            *v50 = v69;
            if (v69 <= 1.0)
            {
                if (v69 < 0.0)
                    *v50 = 0.0;
            }
            else
            {
                *v50 = 1.0;
            }
            v51 = LIGHT_ul_Interpol2Colors(0, a1->color, *v50);
            v115[v1] = v51;
            v52 = v51 | v72;
            ++v1;
            ++v50;
            v72 |= v51;
        } while (v1 < 8);
        if (v52)
        {
            v83 = 0;
            if (a1->num > 0)
            {
                v53 = v97;
                v54 = a1->elems;
                do
                {
                    v55 = LIGHT_ul_Interpol2Colors(v115[v54->colorID], v115[v54->colorID2], v54->colorFactor);
                    if (v55)
                    {
                        *vertexColors = v55;
                        vertexColors[1] = v55;
                        v56 = vertexColors + 1;
                        v56[1] = v55;
                        
                        v57 = ((char*)v97 - ((char*)(GFX_gpst_Geo->vertices))) / sizeof(Vec3);
                        v8->v1 = v57;
                        v8->uv1 = 0;
                        v8->uv2 = 1;
                        v8->uv3 = 2;
                        vertexColors = v56 + 2;
                        v8->v2 = v57 + 1;
                        v8->v3 = v8->v1 + 2;
                        v58 = v54->i1;
                        v73 = v54->i4;
                        ++v8;
                        v102 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v1.y * v58);
                        v104 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v1.z * v58);
                        v59 = v54->i2;
                        v91 = (float)(v59 * gpst_DX_stDD->ct.cam.objectMtx.v2.x);
                        v93 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v2.y * v59);
                        v95 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v2.z * v59);
                        v100 = v95 + v104;
                        v98 = (float)((v91 + v58 * gpst_DX_stDD->ct.cam.objectMtx.v1.x) * v73);
                        v60 = (v93 + v102) * v73;
                        v61 = v100 * v73;
                        v106 = v76 - v98;
                        v107 = (float)(v78 - v60);
                        v108 = (float)(v80 - v61);
                        v62 = a1->i14;
                        v112 = (float)(v98 * v62);
                        v113 = (float)(v60 * v62);
                        v99 = v113 + v78;
                        v63 = v112 + v76;
                        v101 = (float)(v61 * v62 + v80);
                        v64 = v54->i2;
                        v103 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v1.y * v64);
                        v105 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v1.z * v64);
                        v65 = -v54->i1;
                        v92 = (float)(v65 * gpst_DX_stDD->ct.cam.objectMtx.v2.x);
                        v94 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v2.y * v65);
                        v70 = v54->i5;
                        v96 = (float)(gpst_DX_stDD->ct.cam.objectMtx.v2.z * v65);
                        v88 = v96 + v105;
                        v84 = (float)((v92 + v64 * gpst_DX_stDD->ct.cam.objectMtx.v1.x) * v70);
                        v97->x = v106;
                        v86 = (v94 + v103) * v70;
                        v53->y = v107;
                        v53->z = v108;
                        v89 = v88 * v70;
                        v109 = (float)(v63 - v84);
                        v53[1].x = v109;
                        v110 = v99 - v86;
                        v53[1].y = v110;
                        v111 = v101 - v89;
                        v53[1].z = v111;
                        v85 = (float)(v84 + v63);
                        v53[2].x = v85;
                        v87 = v86 + v99;
                        v53[2].y = v87;
                        v90 = v89 + v101;
                        v53[2].z = v90;
                        v97 += 3;
                        v53 += 3;
                    }
                    else
                    {
                        --GFX_gpst_Geo->triangleChunks->triangleNum;
                    }
                    ++v54;
                    ++v83;
                } while (v83 < a1->num);
            }
            v66 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent = v66 & ~0x9000900u;
            p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists;
            GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
            gpst_DX_stDD->drawMaskAllowedStuffCurrent = v66;
        }
    }
}

void GFX_Sun_Seti(struct sGfxSunData* a1, int a2)
{
    if (a2 == 18100)
        GFX_Sun_ComputeRay(a1);
}

void D3D_UnsetSunSurface(tGfxSunD3DSpecificData* a1)
{
    int v1;

    v1 = 0;
    while (gD3dSunSurfaces[v1] != a1)
    {
        if (++v1 >= 16)
            return;
    }
    gD3dSunSurfaces[v1] = NULL;
}

void GFX_Sun_Destroy(struct sGfxSunD3DSpecificData* a1)
{
    tGfxSunD3DSpecificDataInner* v1;
    int v2;

    D3D_SetSunEffectOnOff(a1, 0);
    D3D_ReleaseSunRenderTargetAndTexture(a1);
    D3D_UnsetSunSurface(a1);
    v1 = a1->elems;
    v2 = 3;
    do
    {
        v1->d3dppSurface->lpVtbl->Release(v1->d3dppSurface);
        ++v1;
        --v2;
    } while (v2);
}