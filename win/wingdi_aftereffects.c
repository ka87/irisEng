#include "../dbg.h"
#include "wingdi.h"

float depthBlurNear1 = 0.0f;
float depthBlurType0 = 0.0f;
float depthBlurType1 = 0.0f;
float depthBlurType2 = 0.0f; // FIXME: unused?

float bandsVal1 = 0.10416666f;
float bandsVal2 = 0.89583331f;
float bandsVal3 = 0.083333336f;
float bandsVal4 = 0.91666669f;

float gAEOpacity = 0.0f;
float colorBalanceParam1 = 0.5f;
float unkAEFunc2Param = 0.5f;
float unknownAEFlag4GlobalVar = 0.0f;
float texFactorManual = 0.0f;
float remenanceFactor = 0.0f;
float motionBlurFactor = 0.0f;
float zoomSmoothFactor = 0.0f;
float blurFactor = 0.0f;
float zoomSmoothFactor2 = 0.0f;
float texFactorProbablyUnunsedType2 = 0.0f;
float blackAndWhiteParam1 = 0.0f; // FIXME: unused ?


void D3D_SaveAEParams()
{
    double v0;
    double v1;
    double v2;
    double v3;

    if (D3DRenderParams.tweak_zonalpha)
    {
        depthblur_Type = 0;
        goto LABEL_13;
    }
    if ((PC_AE_MASTER & 8) != 0 && (PC_AE_MASTER & 0x1000) != 0)// depthblur, depthblurnear
    {
        depthblur_Type = 3;
        goto LABEL_13;
    }
    if ((PC_AE_MASTER & 8) != 0)
    {
        if ((PC_AE_MASTER & 0x1000) == 0)
        {
            depthblur_Type = 1;
            goto LABEL_13;
        }
        if ((PC_AE_MASTER & 8) != 0)
            goto LABEL_12;
    }
    if ((PC_AE_MASTER & 0x1000) == 0)
        LABEL_12:
    depthblur_Type = D3DRenderParams.sunEffectIsOn != 0 ? 4 : 0;
    else
        depthblur_Type = 2;
LABEL_13:
    if (gpst_DX_stDD->ct.cam.nearPlane <= 1.1754944e-38)
        camUnkf1Save = 1.1754944e-38f;
    else
        camUnkf1Save = gpst_DX_stDD->ct.cam.nearPlane;
    v0 = camUnkf1Save + 0.0099999998;
    if (depthBlurNear1 <= v0)
        deptBlurNear1Save = (float)v0;
    else
        deptBlurNear1Save = depthBlurNear1;
    v1 = deptBlurNear1Save + 0.0099999998;
    if (depthBlurType0 <= v1)
        deptBlurType0Save = (float)v1;
    else
        deptBlurType0Save = depthBlurType0;
    v2 = deptBlurType0Save + 0.0099999998;
    if (depthBlurType1 <= v2)
        deptBlur1TypeSave = (float)v2;
    else
        deptBlur1TypeSave = depthBlurType1;
    v3 = deptBlur1TypeSave + 0.0099999998;
    if (v3 >= gpst_DX_stDD->ct.cam.farPlane)
        depthBlur1TypeOrcamUnkf2Save = (float)v3;
    else
        depthBlur1TypeOrcamUnkf2Save = gpst_DX_stDD->ct.cam.farPlane;
}

void D3D_SetGlobalBlurParams()
{
    memset(&D3DRenderParams.transformationMatrix, 0, sizeof(D3DRenderParams.transformationMatrix));
    switch (depthblur_Type)
    {
    case 0:
        goto LABEL_5;
    case 1:
        D3DRenderParams.textureUVIsProjected = 1;
        D3DRenderParams.transformationMatrix.f41 = deptBlurType0Save;
        D3DRenderParams.transformationMatrix.f33 = 1.0;
        break;
    case 2:
    case 3:
        D3DRenderParams.textureUVIsProjected = 1;
        D3DRenderParams.transformationMatrix.f41 = deptBlurNear1Save;
        D3DRenderParams.transformationMatrix.f33 = 1.0;
        break;
    case 4:
        D3DRenderParams.transformationMatrix.f31 = (float)(1.0 / depthBlur1TypeOrcamUnkf2Save);
    LABEL_5:
        D3DRenderParams.textureUVIsProjected = 0;
        break;
    default:
        return;
    }
}

float D3D_DepthBlurBlurFuncCalc1(int a1)
{
    double result;

    result = 0.0;
    if (depthblur_Type == 4)
        result = (double)a1 * depthBlur1TypeOrcamUnkf2Save * 0.00390625;
    return (float)result;
}

uint32_t  D3D_DepthBlurBlurFuncCalc2(float a1)
{
    double v1;

    v1 = 0.0;
    if (depthblur_Type == 4)
    {
        if (a1 < 0.0)
            return (uint32_t)255.0;
        if (a1 <= (double)depthBlur1TypeOrcamUnkf2Save)
            return (uint32_t)((1.0 - a1 / depthBlur1TypeOrcamUnkf2Save) * 255.0);
        v1 = 0.0;
    }
    return (uint32_t)v1;
}

void D3D_DepthBlurBlurFunc(int* a1)
{
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int i;
    float v9;
    int v10;
    int v11;
    int j;

    switch (depthblur_Type)
    {
    case 0:
        for (j = 0; j < 256; j++) a1[j] = 0xFF000000;
        break;
    case 1:
        v5 = (unsigned int)(deptBlurType0Save / deptBlur1TypeSave * 256.0);
        v6 = 0;
        if (v5 <= 0 || (memset(a1, 0, 4 * v5), v6 = v5, v5 < 256))
        {
            v7 = v6 - v5;
            v11 = v6 - v5;
            do
            {
                a1[v6++] = (unsigned int)((double)v11 * (255.0 / (double)(256 - v5))) << 24;
                v11 = ++v7;
            } while (v6 < 256);
        }
        a1[255] = 0xFF000000;
        break;
    case 2:
        for (j = 0; j < 255; j++) a1[j] = 0xFF000000;
        a1[255] = 0;
        break;
    case 3:
        v1 = (unsigned int)(deptBlurNear1Save / deptBlur1TypeSave * 256.0);
        v2 = (unsigned int)(deptBlurNear1Save / deptBlurType0Save * 256.0);
        v3 = 0;
        if (v1 > 0)
        {
            memset(a1, 0, 4 * v1);
            v3 = v1;
        }
        if (v3 < v2)
        {
            v4 = v3 - v1;
            v10 = v3 - v1;
            do
            {
                a1[v3++] = (unsigned int)((double)v10 * (255.0 / (double)(v2 - v1))) << 24;
                v10 = ++v4;
            } while (v3 < v2);
        }
        if (v3 < 255) {
            for (j = v3; j <= (255 - v3); j++) a1[j] = 0xFF000000;
        }
        a1[255] = 0;
        break;
    case 4:
        for (i = 0; i < 256; ++i)
        {
            v9 = D3D_DepthBlurBlurFuncCalc1(i);
            a1[i] = (uint8_t)D3D_DepthBlurBlurFuncCalc2(v9) << 24;
        }
        a1[255] = 0;
        break;
    default:
        return;
    }
}

int D3D_UpdateBlurTexture()
{
    IDirect3DSurface9* v1;
    D3DLOCKED_RECT v2;

    D3D_SaveAEParams();
    D3DRenderParams.isDepthBlurOn = depthblur_Type != 0;
    D3D_SetGlobalBlurParams();
    D3DRenderParams.pSourceTexture->lpVtbl->GetSurfaceLevel(D3DRenderParams.pSourceTexture, 0, &v1);
    v1->lpVtbl->LockRect(v1, &v2, NULL, 0);
    D3D_DepthBlurBlurFunc((int *)(v2.pBits));
    v1->lpVtbl->UnlockRect(v1);
    D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)(D3DRenderParams.pSourceTexture), (IDirect3DBaseTexture9*)(D3DRenderParams.pDestTexture));
    return v1->lpVtbl->Release(v1);
}

void D3D_CreateBlurTextures()
{
    int v0;
    int v1;

    v0 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        256,
        1,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.pDestTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v0);
    v1 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        256,
        1,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_SYSTEMMEM,
        &D3DRenderParams.pSourceTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v1);
    PC_AE_MASTER &= ~0x1008u;
    depthBlurType0 = 0.0;
    depthBlurType1 = 0.0;
    depthBlurNear1 = 0.0;
    D3D_UpdateBlurTexture();
}

void D3D_CreateTextureAndBlankIt(IDirect3DTexture9 **a3)
{
    int v2;
    IDirect3DSurface9* v3;

    v2 = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
        D3DRenderParams.d3dDev,
        D3DRenderParams.presentParams.BackBufferWidth,
        D3DRenderParams.presentParams.BackBufferHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DRenderParams.presentParams.BackBufferFormat,
        D3DPOOL_DEFAULT,
        a3,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(v2);
    (*a3)->lpVtbl->GetSurfaceLevel(*a3, 0, &v3);
    D3DRenderParams.d3dDev->lpVtbl->ColorFill(D3DRenderParams.d3dDev, v3, NULL, 0);
    v3->lpVtbl->Release(v3);
}

void D3D_AETexturesInit()
{
    gMaxSimultaneousTextures = D3DRenderParams.d3dcaps.MaxSimultaneousTextures;
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.blurRenderTarget);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.blurRenderTarget2);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.unktex1_dword_B31988);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.unktex2_dword_B3198C);
}

void D3D_SetD3DStatesDefaultForAE()
{
    int v0;
    int v1;
    int v2;
    IDirect3DDevice9* v3;
    IDirect3DDevice9* v4;
    IDirect3DDevice9* v5;
    IDirect3DDevice9* v6;
    IDirect3DDevice9* v7;
    IDirect3DDevice9* v8;
    IDirect3DDevice9* v9;
    IDirect3DDevice9* v10;

    v0 = 0;
    if (renderStateConfig.d3d_zEnable)
    {
        renderStateConfig.d3d_zEnable = D3DZB_FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZENABLE, D3DZB_FALSE);
    }
    if (renderStateConfig.d3d_zFunc != D3DCMP_ALWAYS)
    {
        renderStateConfig.d3d_zFunc = D3DCMP_ALWAYS;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZFUNC, D3DCMP_ALWAYS);
    }
    if (renderStateConfig.d3d_zWriteEnable)
    {
        renderStateConfig.d3d_zWriteEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ZWRITEENABLE, FALSE);
    }
    D3DRenderParams.d3dFogModeProbably = 0;
    if (renderStateConfig.d3dFogEnable)
    {
        renderStateConfig.d3dFogEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FOGENABLE, FALSE);
    }
    if (renderStateConfig.d3dFillMode != D3DFILL_SOLID)
    {
        renderStateConfig.d3dFillMode = D3DFILL_SOLID;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_FILLMODE, D3DFILL_SOLID);
    }
    if (renderStateConfig.d3dCullMode != D3DCULL_NONE)
    {
        renderStateConfig.d3dCullMode = D3DCULL_NONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_CULLMODE, D3DCULL_NONE);
    }
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, FALSE);
    }
    if (renderStateConfig.d3dAlphaFunc != D3DCMP_ALWAYS)
    {
        renderStateConfig.d3dAlphaFunc = D3DCMP_ALWAYS;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    if (renderStateConfig.d3d_BlendOp != D3DBLENDOP_ADD)
    {
        renderStateConfig.d3d_BlendOp = D3DBLENDOP_ADD;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
    if (renderStateConfig.d3dSrcBlend != D3DBLEND_ONE)
    {
        renderStateConfig.d3dSrcBlend = D3DBLEND_ONE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_ONE);
    }
    if (renderStateConfig.d3dDestBlend != D3DBLEND_ZERO)
    {
        renderStateConfig.d3dDestBlend = D3DBLEND_ZERO;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_ZERO);
    }
    if (renderStateConfig.d3d_Lighting)
    {
        renderStateConfig.d3d_Lighting = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_LIGHTING, FALSE);
    }
    v1 = 0;
    v2 = 0;
    do
    {
        if (textureStageTypeConfig[v0].d3d_colorOp != D3DTOP_DISABLE)
        {
            v3 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v0].d3d_colorOp = D3DTOP_DISABLE;
            v3->lpVtbl->SetTextureStageState(v3, v1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }
        if (textureStageTypeConfig[v0].d3d_alphaOp != D3DTOP_DISABLE)
        {
            v4 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v0].d3d_alphaOp = D3DTOP_DISABLE;
            v4->lpVtbl->SetTextureStageState(v4, v1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }
        if (textureStageTypeConfig[v0].d3dTexCoordIndex != v1)
        {
            v5 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v0].d3dTexCoordIndex = v1;
            v5->lpVtbl->SetTextureStageState(v5, v1, D3DTSS_TEXCOORDINDEX, v1);
        }
        if (textureStageTypeConfig[v0].d3dTextureTransformFlags)
        {
            v6 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v0].d3dTextureTransformFlags = D3DTTFF_DISABLE;
            v6->lpVtbl->SetTextureStageState(v6, v1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v1, NULL);
        if (samplerConfig[v2].d3d_minfilter != D3DTEXF_POINT)
        {
            v7 = D3DRenderParams.d3dDev;
            samplerConfig[v2].d3d_minfilter = D3DTEXF_POINT;
            v7->lpVtbl->SetSamplerState(v7, v1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
        if (samplerConfig[v2].d3d_magfilter != D3DTEXF_POINT)
        {
            v8 = D3DRenderParams.d3dDev;
            samplerConfig[v2].d3d_magfilter = D3DTEXF_POINT;
            v8->lpVtbl->SetSamplerState(v8, v1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        }
        if (samplerConfig[v2].d3d_addressu != D3DTADDRESS_CLAMP)
        {
            v9 = D3DRenderParams.d3dDev;
            samplerConfig[v2].d3d_addressu = D3DTADDRESS_CLAMP;
            v9->lpVtbl->SetSamplerState(v9, v1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        }
        if (samplerConfig[v2].d3d_addressv != D3DTADDRESS_CLAMP)
        {
            v10 = D3DRenderParams.d3dDev;
            samplerConfig[v2].d3d_addressv = D3DTADDRESS_CLAMP;
            v10->lpVtbl->SetSamplerState(v10, v1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        }
        ++v0;
        ++v1;
        ++v2;
    } while (v0 < 4);
}

void D3D_DrawASurface(void* a1, unsigned int a2)
{
    IDirect3DDevice9* v2;
    int v3;

    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = a2;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    v2 = D3DRenderParams.d3dDev;
    v3 = D3D_CalculateStride(a2);
    v2->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2,  a1, v3);
}

void D3D_DepthBlurSD(IDirect3DTexture9* a5, float a6, float a7)
{
    tVertex_type_204* v3;
    int v4;
    int v5;
    int i;
    IDirect3DDevice9* v7;
    IDirect3DDevice9* v8;
    int v9;
    int v10;
    int v11;
    IDirect3DDevice9* v12;
    IDirect3DDevice9* v13;
    IDirect3DDevice9* v14;
    IDirect3DDevice9* v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    tVertex_type_204 v23[4];

    v16 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v21 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v23[0].uvSet1.v = bandsVal4;
    v23[0].uvSet2.v = bandsVal4;
    v23[0].v1.y = bandsVal4 * v21 - 0.5f;
    v23[1].uvSet1.v = bandsVal3;
    v23[0].uvSet2.u = a6;
    v23[0].uvSet1.u = -a6;
    v23[1].uvSet2.u = a6;
    v23[1].uvSet2.v = bandsVal3;
    v23[2].uvSet1.v = bandsVal4;
    v23[2].uvSet2.v = bandsVal4;
    v22 = bandsVal3 * v21;
    v23[1].v1.y = v22;
    v23[3].v1.y = v22;
    v23[1].uvSet1.u = v23[0].uvSet1.u;
    v23[3].uvSet1.v = bandsVal3;
    v23[0].v1.x = 0.0;
    v23[1].v1.x = 0.0;
    v23[2].v1.x = v16 - 0.5f;
    v23[2].v1.y = v23[0].v1.y;
    v17 = 1.0f - a6;
    v23[2].uvSet1.u = v17;
    v23[3].uvSet1.u = v17;
    v23[3].uvSet2.v = bandsVal3;
    v3 = &v23[0];
    v19 = a6 + 1.0f;
    v23[2].uvSet2.u = v19;
    v23[3].uvSet2.u = v19;
    v23[3].v1.x = v23[2].v1.x;
    v4 = 4;
    do
    {
        v3->v1.z = 0.0;
        v3->hw = 1.0;
        v3++;
        --v4;
    } while (v4);
    v5 = 0;
    for (i = 0; i < 2; ++i)
    {
        if (samplerConfig[i].d3d_minfilter != D3DTEXF_LINEAR)
        {
            v7 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_minfilter = D3DTEXF_LINEAR;
            v7->lpVtbl->SetSamplerState(v7, v5, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        if (samplerConfig[i].d3d_magfilter != D3DTEXF_LINEAR)
        {
            v8 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_magfilter = D3DTEXF_LINEAR;
            v8->lpVtbl->SetSamplerState(v8, v5, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        }
        ++v5;
    }
    if (renderStateConfig.d3dTextureFactor != 0x80FFFFFF)
    {
        renderStateConfig.d3dTextureFactor = 0x80FFFFFF;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, 0x80FFFFFF);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a5);
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_BLENDFACTORALPHA)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_BLENDFACTORALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a5);
    if (textureStageTypeConfig[2].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[2].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[2].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    D3D_DrawASurface(v23, (D3DFVF_TEX2 | D3DFVF_XYZRHW));
    v23[0].uvSet1.v = bandsVal4 - a7;
    v23[0].uvSet1.u = 0.0;
    v23[0].uvSet2.u = 0.0;
    v23[1].uvSet1.u = 0.0;
    v23[1].uvSet2.u = 0.0;
    v23[2].uvSet1.u = 1.0;
    v23[0].uvSet2.v = bandsVal4 + a7;
    v23[2].uvSet2.u = 1.0;
    v23[3].uvSet1.u = 1.0;
    v23[3].uvSet2.u = 1.0;
    v20 = bandsVal3 - a7;
    v23[1].uvSet1.v = v20;
    v23[3].uvSet1.v = v20;
    v18 = bandsVal3 + a7;
    v23[1].uvSet2.v = v18;
    v23[3].uvSet2.v = v18;
    v23[2].uvSet1.v = v23[0].uvSet1.v;
    v23[2].uvSet2.v = v23[0].uvSet2.v;
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a5);
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_BLENDFACTORALPHA)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_BLENDFACTORALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a5);
    if (textureStageTypeConfig[2].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[2].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[2].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
    {
        renderStateConfig.d3dAlphaBlendEnable = TRUE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
    }
    if (renderStateConfig.d3d_BlendOp != D3DBLENDOP_ADD)
    {
        renderStateConfig.d3d_BlendOp = D3DBLENDOP_ADD;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
    if (renderStateConfig.d3dSrcBlend != D3DBLEND_SRCALPHA)
    {
        renderStateConfig.d3dSrcBlend = D3DBLEND_SRCALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    }
    if (renderStateConfig.d3dDestBlend != D3DBLEND_SRCALPHA)
    {
        renderStateConfig.d3dDestBlend = D3DBLEND_SRCALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
    }
    D3D_DrawASurface(v23, (D3DFVF_TEX2 | D3DFVF_XYZRHW));
    v9 = 0;
    v10 = 0;
    v11 = 0;
    do
    {
        if (samplerConfig[v10].d3d_minfilter != D3DTEXF_POINT)
        {
            v12 = D3DRenderParams.d3dDev;
            samplerConfig[v10].d3d_minfilter = D3DTEXF_POINT;
            v12->lpVtbl->SetSamplerState(v12, v9, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
        if (samplerConfig[v10].d3d_magfilter != D3DTEXF_POINT)
        {
            v13 = D3DRenderParams.d3dDev;
            samplerConfig[v10].d3d_magfilter = D3DTEXF_POINT;
            v13->lpVtbl->SetSamplerState(v13, v9, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        }
        if (textureStageTypeConfig[v11].d3d_colorOp != D3DTOP_DISABLE)
        {
            v14 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v11].d3d_colorOp = D3DTOP_DISABLE;
            v14->lpVtbl->SetTextureStageState(v14, v9, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }
        if (textureStageTypeConfig[v11].d3d_alphaOp != D3DTOP_DISABLE)
        {
            v15 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v11].d3d_alphaOp = D3DTOP_DISABLE;
            v15->lpVtbl->SetTextureStageState(v15, v9, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v9, NULL);
        ++v10;
        ++v9;
        ++v11;
    } while (v10 < 2);
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
    }
}

void D3D_DepthBlurHD(IDirect3DTexture9* a5, float a6, float a7)
{
    int v3;
    tVertex_type_404* v4;
    int v5;
    int v6;
    int i;
    IDirect3DDevice9* v8;
    IDirect3DDevice9* v9;
    IDirect3DDevice9* v10;
    IDirect3DDevice9* v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    tVertex_type_404 a1[4];

    if (renderStateConfig.d3dTextureFactor != 0x80FFFFFF)
    {
        renderStateConfig.d3dTextureFactor = 0x80FFFFFF;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 60, 0x80FFFFFF);
    }
    v3 = 0;
    if (samplerConfig[0].d3d_minfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[0].d3d_minfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    if (samplerConfig[0].d3d_magfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[0].d3d_magfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a5);
    if (samplerConfig[1].d3d_minfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[1].d3d_minfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    if (samplerConfig[1].d3d_magfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[1].d3d_magfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_BLENDFACTORALPHA)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_BLENDFACTORALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a5);
    if (samplerConfig[2].d3d_minfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[2].d3d_minfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    if (samplerConfig[2].d3d_magfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[2].d3d_magfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    if (textureStageTypeConfig[2].d3d_colorOp != D3DTOP_BLENDFACTORALPHA)
    {
        textureStageTypeConfig[2].d3d_colorOp = D3DTOP_BLENDFACTORALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    }
    if (textureStageTypeConfig[2].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[2].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[2].d3d_colorarg2 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[2].d3d_colorarg2 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[2].d3d_alphaArg1)
    {
        textureStageTypeConfig[2].d3d_alphaArg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a5);
    if (samplerConfig[3].d3d_minfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[3].d3d_minfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    if (samplerConfig[3].d3d_magfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[3].d3d_magfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    if (textureStageTypeConfig[3].d3d_colorOp != D3DTOP_BLENDFACTORALPHA)
    {
        textureStageTypeConfig[3].d3d_colorOp = D3DTOP_BLENDFACTORALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    }
    if (textureStageTypeConfig[3].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[3].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[3].d3d_colorarg2 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[3].d3d_colorarg2 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[3].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[3].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[3].d3d_alphaArg1)
    {
        textureStageTypeConfig[3].d3d_alphaArg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)a5);
    if (textureStageTypeConfig[4].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[4].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 4, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[4].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[4].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 4, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    v14 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v12 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1[0].uvSet1.v = bandsVal4;
    a1[0].uvSet2.v = bandsVal4;
    a1[0].v1.y = bandsVal4 * v12 - 0.5f;
    a1[1].uvSet1.v = bandsVal3;
    a1[0].uvSet2.u = a6;
    a1[0].uvSet1.u = -a6;
    a1[1].uvSet2.u = a6;
    a1[1].uvSet2.v = bandsVal3;
    a1[2].uvSet1.v = bandsVal4;
    a1[2].uvSet2.v = bandsVal4;
    v17 = bandsVal4 - a7;
    a1[0].v1.x = 0.0;
    a1[0].uvSet3.v = v17;
    a1[2].uvSet3.v = v17;
    a1[0].uvSet3.u = 0.0;
    a1[0].uvSet4.u = 0.0;
    v18 = bandsVal4 + a7;
    a1[1].v1.x = 0.0;
    a1[0].uvSet4.v = v18;
    a1[2].uvSet4.v = v18;
    a1[1].uvSet3.u = 0.0;
    a1[1].uvSet4.u = 0.0;
    a1[2].uvSet3.u = 1.0;
    v13 = bandsVal3 * v12;
    a1[2].uvSet4.u = 1.0;
    a1[1].v1.y = v13;
    a1[3].v1.y = v13;
    a1[1].uvSet1.u = a1[0].uvSet1.u;
    a1[3].uvSet1.v = bandsVal3;
    a1[3].uvSet2.v = bandsVal3;
    a1[3].uvSet3.u = 1.0;
    a1[1].uvSet3.v = bandsVal3 - a7;
    v16 = bandsVal3 + a7;
    a1[1].uvSet4.v = v16;
    a1[2].v1.x = v14 - 0.5f;
    a1[2].v1.y = a1[0].v1.y;
    a1[2].uvSet1.u = 1.0f - a6;
    v15 = a6 + 1.0f;
    a1[2].uvSet2.u = v15;
    a1[3].uvSet2.u = v15;
    a1[3].v1.x = a1[2].v1.x;
    a1[3].uvSet1.u = a1[2].uvSet1.u;
    a1[3].uvSet3.v = a1[1].uvSet3.v;
    a1[3].uvSet4.v = v16;
    a1[3].uvSet4.u = 1.0;
    v4 = &a1[0];
    v5 = 4;
    do
    {
        v4->v1.z = 0.0f;
        v4->hw = 1.0f;        
        v4++;
        --v5;
    } while (v5);
    D3D_DrawASurface(a1, (D3DFVF_TEX4 | D3DFVF_XYZRHW));
    v6 = 0;
    for (i = 0; i < 4; ++i)
    {
        if (samplerConfig[i].d3d_minfilter != D3DTEXF_POINT)
        {
            v8 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_minfilter = D3DTEXF_POINT;
            v8->lpVtbl->SetSamplerState(v8, v6, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
        if (samplerConfig[i].d3d_magfilter != D3DTEXF_POINT)
        {
            v9 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_magfilter = D3DTEXF_POINT;
            v9->lpVtbl->SetSamplerState(v9, v6, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        }
        if (textureStageTypeConfig[v3].d3d_colorOp != D3DTOP_DISABLE)
        {
            v10 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v3].d3d_colorOp = D3DTOP_DISABLE;
            v10->lpVtbl->SetTextureStageState(v10, v6, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }
        if (textureStageTypeConfig[v3].d3d_alphaOp != D3DTOP_DISABLE)
        {
            v11 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v3].d3d_alphaOp = D3DTOP_DISABLE;
            v11->lpVtbl->SetTextureStageState(v11, v6, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v6++, NULL);
        ++v3;
    }
}

void D3D_SetRenderTargetFromTexture(IDirect3DTexture9* a1)
{
    IDirect3DSurface9* surf;
    a1->lpVtbl->GetSurfaceLevel(a1, 0, &surf);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surf);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, NULL);
    surf->lpVtbl->Release(surf);
}

void D3D_DepthBlur(IDirect3DTexture9* texture, IDirect3DTexture9* renderTarget, float blurFactor, float a7)
{
    D3D_SetRenderTargetFromTexture(renderTarget);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_DepthBlurHD(texture, blurFactor, a7);
    else
        D3D_DepthBlurSD(texture, blurFactor, a7);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
}

void D3D_Blur(IDirect3DTexture9* texture, IDirect3DTexture9* renderTarget, float a3)
{
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;
    float blurFactor;
    float blurFactora;
    float blurFactorb;
    float blurFactorc;
    float blurFactord;
    float blurFactore;
    float blurFactorf;
    float a7;
    float a7a;
    float a7b;
    float a7c;
    float a7d;
    float a7e;
    float a7f;
    float v24;

    v3 = a3 * 0.039999999;
    v24 = (float)v3;
    a7 = (float)(v3 * 0.0020833334);
    blurFactor = (float)(v24 * 0.0015625);
    D3D_DepthBlur(texture, renderTarget, blurFactor, a7);
    v4 = v24 + v24;
    a7a = (float)(0.0020833334 * v4);
    blurFactora = (float)(v4 * 0.0015625);
    D3D_DepthBlur(renderTarget, D3DRenderParams.blurRenderTarget, blurFactora, a7a);
    v5 = v24 * 3.0;
    a7b = (float)(0.0020833334 * v5);
    blurFactorb = (float)(v5 * 0.0015625);
    D3D_DepthBlur(D3DRenderParams.blurRenderTarget, renderTarget, blurFactorb, a7b);
    v6 = v24 * 4.0;
    a7c = (float)(0.0020833334 * v6);
    blurFactorc = (float)(v6 * 0.0015625);
    D3D_DepthBlur(renderTarget, D3DRenderParams.blurRenderTarget, blurFactorc, a7c);
    v7 = v24 * 5.0;
    a7d = (float)(0.0020833334 * v7);
    blurFactord = (float)(v7 * 0.0015625);
    D3D_DepthBlur(D3DRenderParams.blurRenderTarget, renderTarget, blurFactord, a7d);
    v8 = v24 * 6.0;
    a7e = (float)(0.0020833334 * v8);
    blurFactore = (float)(v8 * 0.0015625);
    D3D_DepthBlur(renderTarget, D3DRenderParams.blurRenderTarget, blurFactore, a7e);
    v9 = v24 * 7.0;
    a7f = (float)(0.0020833334 * v9);
    blurFactorf = (float)(v9 * 0.0015625);
    D3D_DepthBlur(D3DRenderParams.blurRenderTarget, renderTarget, blurFactorf, a7f);
}

void D3D_Unknown_AE_Func1(IDirect3DTexture9* a1)
{
    int v1;
    tVertex_type_244* v2;
    int v3;
    float v4;
    float v5;
    float v6;
    float v7;
    tVertex_type_244 v8[4];

    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_BLENDCURRENTALPHA)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_BLENDCURRENTALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)D3DRenderParams.blurRenderTarget);
    v7 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v6 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v5 = (float)(bandsVal2 * 480.0);
    v4 = (float)(bandsVal1 * 480.0);
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    v1 = D3D_CalculateStride(D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v8[0].uvSet1.v = v5;
    v8[0].uvSet2.v = v5;
    v8[0].v1.y = v5 - 0.5f;
    v8[2].v1.y = v8[0].v1.y;
    v8[1].v1.y = v4;
    v8[1].uvSet1.v = v4;
    v8[1].uvSet2.v = v4;
    v8[2].uvSet1.v = v5;
    v8[2].uvSet2.v = v5;
    v8[0].color = 0xffffffff;
    v8[1].color = 0xffffffff;
    v8[2].color = 0xffffffff;
    v8[3].color = 0xffffffff;
    v8[3].v1.y = v4;
    v8[3].uvSet1.v = v4;
    v8[3].uvSet2.v = v4;
    v8[0].v1.x = 0.0;
    v8[0].v1.z = 0.0;
    v8[0].hw = 1.0;
    v8[0].uvSet1.u = 0.0;
    v8[0].uvSet2.u = 0.0;
    v8[1].v1.x = 0.0;
    v8[1].v1.z = 0.0;
    v8[1].hw = 1.0;
    v8[1].uvSet1.u = 0.0;
    v8[1].uvSet2.u = 0.0;
    v8[2].v1.x = 639.5;
    v8[2].v1.z = 0.0;
    v8[2].hw = 1.0;
    v8[2].uvSet1.u = 640.0;
    v8[2].uvSet2.u = 640.0;
    v8[3].v1.x = 639.5;
    v8[3].v1.z = 0.0;
    v8[3].hw = 1.0;
    v8[3].uvSet1.u = 640.0;
    v8[3].uvSet2.u = 640.0;
    v2 = &v8[0];
    v3 = 4;
    do
    {
        --v3;
        v2->v1.x = (float)(v7 * v2->v1.x * 0.0015625);
        v2->v1.y = (float)(v7 * v2->v1.y * 0.0020833334);
        v2->uvSet1.u = (float)(v2->uvSet1.u * 0.0015625);
        v2->uvSet1.v = (float)(v2->uvSet1.v * 0.0020833334);
        v2->uvSet2.u = (float)(v2->uvSet2.u * 0.0015625);
        v2->uvSet2.v = (float)(v2->uvSet2.v * 0.0020833334);
        v2++;
    } while (v3);
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, 5, 2, v8, v1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHATESTENABLE, FALSE);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[1].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, NULL);
}

float D3D_CalcTextureFactor(float a1)
{
    double v1;

    v1 = a1;
    if (a1 > 1.0)
        v1 = v1 - 1.0;
    if (v1 > 0.5)
        v1 = 1.0 - v1;
    if (v1 < 0.1666666)
        return 1.0;
    if (v1 <= 0.33333331)
        return (float)(1.0 - (v1 - 0.16666667) * 6.0);
    return 0.0;
}

// TODO FIXME Ez lehet nem jó, mert az IDA-ban rossz a stack, ha nem mûködik megnézni!!!
void D3D_Unkown_AE_Func_Flag_4(IDirect3DTexture9* a2, IDirect3DTexture9* a1, float a4, float texFactorManual)
{
    int v4;
    unsigned int v5; 
    int v6; 
    tVertex_type_104* v7;
    int v8;
    float v9;
    float v10;
    tVertex_type_104 v11[4];
    float a4a;
    float texFactorManuala;

    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SUBTRACT)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SUBTRACT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a2);
    v9 = texFactorManual - 0.33333331f + 1.0f;
    v4 = (unsigned int)((1.0 - D3D_CalcTextureFactor(v9)) * a4 * 0.5 * 510.0);
    v5 = ((((unsigned int)((1.0 - D3D_CalcTextureFactor(texFactorManual)) * a4 * 0.5 * 510.0) | 0xFFFFFF00) << 8) | v4) << 8;
    v10 = texFactorManual - 0.66666663f + 1.0f;
    v6 = (unsigned int)((1.0 - D3D_CalcTextureFactor(v10)) * a4 * 0.5 * 510.0) | v5;
    if (renderStateConfig.d3dTextureFactor != v6)
    {
        renderStateConfig.d3dTextureFactor = v6;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, v6);
    }
    a4a = (float)D3DRenderParams.presentParams.BackBufferWidth;
    texFactorManuala = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v11[0].uvSet1.v = bandsVal2;
    v11[2].uvSet1.v = bandsVal2;
    v11[3].uvSet1.v = bandsVal1;
    v11[0].v1.y = bandsVal2 * texFactorManuala - 0.5f;
    v11[0].v1.x = 0.0;
    v11[0].uvSet1.u = 0.0;
    v11[1].v1.x = 0.0;
    v11[1].uvSet1.u = 0.0;
    v11[1].uvSet1.v = bandsVal1;
    v11[1].v1.y = bandsVal1 * texFactorManuala;
    v11[2].uvSet1.u = 1.0;
    v11[3].uvSet1.u = 1.0;
    v7 = &v11[0];
    v8 = 4;
    v11[2].v1.x = a4a - 0.5f;
    v11[2].v1.y = v11[0].v1.y;
    v11[3].v1.x = v11[2].v1.x;
    v11[3].v1.y = v11[1].v1.y;
    do
    {
        v7->v1.z = 0.0f;
        v7->hw = 1.0f;
        v7++;
        --v8;
    } while (v8);
    D3D_DrawASurface(v11, D3DFVF_TEX1 | D3DFVF_XYZRHW);
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
}

void D3D_Fade(IDirect3DTexture9* a1, IDirect3DTexture9* a2, int opacity)
{
    int v3;
    float v4;
    tVertex_type_204 v5[4];

    D3D_SetRenderTargetFromTexture(a2);
    if (samplerConfig[0].d3d_minfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[0].d3d_minfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    if (samplerConfig[0].d3d_magfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[0].d3d_magfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_MODULATE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_MODULATE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    }
    if (renderStateConfig.d3dTextureFactor != ((opacity << 24) | 0xFFFFFF))
    {
        renderStateConfig.d3dTextureFactor = (opacity << 24) | 0xFFFFFF;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, (opacity << 24) | 0xFFFFFF);
    }
    if (samplerConfig[1].d3d_minfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[1].d3d_minfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    if (samplerConfig[1].d3d_magfilter != D3DTEXF_LINEAR)
    {
        samplerConfig[1].d3d_magfilter = D3DTEXF_LINEAR;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_BLENDFACTORALPHA)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_BLENDFACTORALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)D3DRenderParams.unktex1_dword_B31988);
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = D3DFVF_TEX2 | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    v3 = D3D_CalculateStride(D3DFVF_TEX2 | D3DFVF_XYZRHW);
    v4 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v5[0].v1.y = (float)((double)D3DRenderParams.presentParams.BackBufferHeight - 0.5f);
    v5[0].v1.x = 0.0;
    v5[0].v1.z = 0.0;
    v5[2].v1.x = v4 - 0.5f;
    v5[0].hw = 1.0;
    v5[0].uvSet1.u = 0.0;
    v5[2].v1.y = v5[0].v1.y;
    v5[0].uvSet1.v = 1.0;
    v5[0].uvSet2.u = 0.0;
    v5[3].v1.x = v5[2].v1.x;
    v5[0].uvSet2.v = 1.0;
    v5[1].v1.x = 0.0;
    v5[1].v1.y = 0.0;
    v5[1].v1.z = 0.0;
    v5[1].hw = 1.0;
    v5[1].uvSet1.u = 0.0;
    v5[1].uvSet1.v = 0.0;
    v5[1].uvSet2.u = 0.0;
    v5[1].uvSet2.v = 0.0;
    v5[2].v1.z = 0.0;
    v5[2].hw = 1.0;
    v5[2].uvSet1.u = 1.0;
    v5[2].uvSet1.v = 1.0;
    v5[2].uvSet2.u = 1.0;
    v5[2].uvSet2.v = 1.0;
    v5[3].v1.y = 0.0;
    v5[3].v1.z = 0.0;
    v5[3].hw = 1.0;
    v5[3].uvSet1.u = 1.0;
    v5[3].uvSet1.v = 0.0;
    v5[3].uvSet2.u = 1.0;
    v5[3].uvSet2.v = 0.0;
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2, (void*)v5, v3);
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (samplerConfig[0].d3d_minfilter != D3DTEXF_POINT)
    {
        samplerConfig[0].d3d_minfilter = D3DTEXF_POINT;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    }
    if (samplerConfig[1].d3d_minfilter != D3DTEXF_POINT)
    {
        samplerConfig[1].d3d_minfilter = D3DTEXF_POINT;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    }
    if (samplerConfig[0].d3d_magfilter != D3DTEXF_POINT)
    {
        samplerConfig[0].d3d_magfilter = D3DTEXF_POINT;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }
    if (samplerConfig[1].d3d_magfilter != D3DTEXF_POINT)
    {
        samplerConfig[1].d3d_magfilter = D3DTEXF_POINT;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, NULL);
}

void D3D_StretchRect(IDirect3DTexture9* d3dtex, RECT* rect)
{
    IDirect3DSurface9* surf;
    IDirect3DSurface9* surf2;

    d3dtex->lpVtbl->GetSurfaceLevel(d3dtex, 0, &surf);
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &surf2);
    D3DRenderParams.d3dDev->lpVtbl->StretchRect(D3DRenderParams.d3dDev, surf, rect, surf2, rect, 0);
    surf->lpVtbl->Release(surf);
    surf2->lpVtbl->Release(surf2);
}

void D3D_MotionBlur(IDirect3DTexture9* a1, IDirect3DTexture9* arg4, float a3)
{
    int v3;
    int v4;
    float v5;
    float v6;
    RECT a2; 
    tVertex_type_104 v8[4];

    v6 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v5 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    D3D_SetRenderTargetFromTexture(D3DRenderParams.unktex2_dword_B3198C);
    if (renderStateConfig.d3dAlphaBlendEnable != TRUE)
    {
        renderStateConfig.d3dAlphaBlendEnable = TRUE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, TRUE);
    }
    if (renderStateConfig.d3d_BlendOp != D3DBLENDOP_ADD)
    {
        renderStateConfig.d3d_BlendOp = D3DBLENDOP_ADD;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
    if (renderStateConfig.d3dSrcBlend != D3DBLEND_SRCALPHA)
    {
        renderStateConfig.d3dSrcBlend = D3DBLEND_SRCALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    }
    if (renderStateConfig.d3dDestBlend != D3DBLEND_INVSRCALPHA)
    {
        renderStateConfig.d3dDestBlend = D3DBLEND_INVSRCALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    }
    v3 = ((unsigned int)(255.0 - a3 * 255.0) << 24) | 0xFFFFFF;
    if (renderStateConfig.d3dTextureFactor != v3)
    {
        renderStateConfig.d3dTextureFactor = ((unsigned int)(255.0 - a3 * 255.0) << 24) | 0xFFFFFF;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, v3);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = 0;
    renderStateConfig.fvf = D3DFVF_TEX1 | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    v4 = D3D_CalculateStride(D3DFVF_TEX1 | D3DFVF_XYZRHW);
    v8[0].v1.y = bandsVal2 * v5 - 0.5f;
    v8[1].uvSet1.v = bandsVal1;
    v8[3].uvSet1.v = bandsVal1;
    v8[1].v1.y = bandsVal1 * v5;
    v8[0].v1.x = 0.0;
    v8[0].v1.z = 0.0;
    v8[0].hw = 1.0;
    v8[0].uvSet1.u = 0.0;
    v8[0].uvSet1.v = bandsVal2;
    v8[2].v1.x = v6 - 0.5f;
    v8[1].v1.x = 0.0;
    v8[1].v1.z = 0.0;
    v8[2].v1.y = v8[0].v1.y;
    v8[1].hw = 1.0;
    v8[1].uvSet1.u = 0.0;
    v8[3].v1.x = v8[2].v1.x;
    v8[2].v1.z = 0.0;
    v8[2].hw = 1.0;
    v8[3].v1.y = v8[1].v1.y;
    v8[2].uvSet1.u = 1.0;
    v8[2].uvSet1.v = bandsVal2;
    v8[3].v1.z = 0.0;
    v8[3].hw = 1.0;
    v8[3].uvSet1.u = 1.0;
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2, (void*)v8, v4);
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3D_SetRenderTargetFromTexture(arg4);
    a2.left = 0;
    a2.top = (unsigned int)(bandsVal1 * v5);
    a2.right = (unsigned int)v6;
    a2.bottom = (unsigned int)(bandsVal2 * v5);
    D3D_StretchRect(D3DRenderParams.unktex2_dword_B3198C, &a2);
}
/*
void D3D_AE_CopyToScreen(IDirect3DTexture9* d3dtex)
{
    //double v1;
    //double v2;
    //float v3;
    //float f2;

    RECT rects[2];
    IDirect3DSurface9* surf;

    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, 0, &surf);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surf);
    surf->lpVtbl->Release(surf);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, D3DRenderParams.pDepthStencilSurface);

    if (tweak_ae_nobands)
    {
        rects[1].left = 0;
        rects[1].top = 0;
        rects[1].right = D3DRenderParams.presentParams.BackBufferWidth;
        rects[1].bottom = D3DRenderParams.presentParams.BackBufferHeight;
        D3D_StretchRect(d3dtex, &rects[1]);
    }
    else
    {
        // rendered image
        rects[1].left = 0;
        rects[1].top = (unsigned int)(bandsVal1 * D3DRenderParams.presentParams.BackBufferHeight);
        rects[1].right = D3DRenderParams.presentParams.BackBufferWidth;
        rects[1].bottom = (unsigned int)bandsVal2 * D3DRenderParams.presentParams.BackBufferHeight;
        D3D_StretchRect(d3dtex, &rects[1]);
        
        // top and bottom bands
        rects[0].left = 0;
        rects[0].top = 0;
        rects[0].right = (unsigned int)D3DRenderParams.presentParams.BackBufferWidth;
        rects[0].bottom = (unsigned int)((bandsVal1 * D3DRenderParams.presentParams.BackBufferHeight) + 1.0f);
        rects[1].left = 0;
        rects[1].top = (unsigned int)(D3DRenderParams.presentParams.BackBufferHeight - (bandsVal1 * D3DRenderParams.presentParams.BackBufferHeight) - 1.0f);
        rects[1].right = rects[0].right;
        rects[1].bottom = (unsigned int)D3DRenderParams.presentParams.BackBufferHeight;
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 2, (D3DRECT *)rects, 1, 0xFF000000, 0, 0);        
    }
}
*/

void D3D_AE_CopyToScreen(IDirect3DTexture9* d3dtex)
{
    RECT rects[2];
    IDirect3DSurface9* surf;

    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, 0, &surf);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surf);
    surf->lpVtbl->Release(surf);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, D3DRenderParams.pDepthStencilSurface);

    if (tweak_ae_nobands)
    {
        rects[1].left = 0;
        rects[1].top = 0;
        rects[1].right = (unsigned int)D3DRenderParams.presentParams.BackBufferWidth;
        rects[1].bottom = (unsigned int)D3DRenderParams.presentParams.BackBufferHeight;
        D3D_StretchRect(d3dtex, &rects[1]);
    }
    else
    {
        // rendered image
        rects[1].left = 0;
        rects[1].top = (unsigned int)(bandsVal1 * D3DRenderParams.presentParams.BackBufferHeight);
        rects[1].right = (unsigned int)D3DRenderParams.presentParams.BackBufferWidth;
        rects[1].bottom = (unsigned int)(bandsVal2 * D3DRenderParams.presentParams.BackBufferHeight);
        D3D_StretchRect(d3dtex, &rects[1]);

        // top and bottom bands
        rects[0].left = 0;
        rects[0].top = 0;
        rects[0].right = (unsigned int)D3DRenderParams.presentParams.BackBufferWidth;
        rects[0].bottom = (unsigned int)(bandsVal1 * D3DRenderParams.presentParams.BackBufferHeight + 1.0f);
        rects[1].left = 0;
        rects[1].top = (unsigned int)(D3DRenderParams.presentParams.BackBufferHeight - (bandsVal1 * D3DRenderParams.presentParams.BackBufferHeight) - 1.0f);
        rects[1].right = rects[0].right;
        rects[1].bottom = (unsigned int)D3DRenderParams.presentParams.BackBufferHeight;
        D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, 2, (D3DRECT*)rects, 1, 0xFF000000, 0, 0);
    }
}

void D3D_Unkown_AE_Func_Flag_1(IDirect3DTexture9* a1)
{
    tVertex_type_144* v1;
    int v2;
    int v3;
    int i;
    IDirect3DDevice9* v5;
    IDirect3DDevice9* v6;
    float v7;
    float v8;
    tVertex_type_144 v9[4];

    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_MODULATEALPHA_ADDCOLOR)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_MODULATEALPHA_ADDCOLOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    if (textureStageTypeConfig[1].d3d_colorOp != D3DTOP_DOTPRODUCT3)
    {
        textureStageTypeConfig[1].d3d_colorOp = D3DTOP_DOTPRODUCT3;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != D3DTOP_SELECTARG2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = D3DTOP_SELECTARG2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, D3DTOP_SELECTARG2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1 != D3DTA_CURRENT)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = D3DTA_CURRENT;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
    }
    if (renderStateConfig.d3dTextureFactor != 0xFF9ADA88)
    {
        renderStateConfig.d3dTextureFactor = 0xFF9ADA88;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_TEXTUREFACTOR, 0xFF9ADA88);
    }
    v8 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v7 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v9[0].uvSet1.v = bandsVal2;
    v9[1].uvSet1.v = bandsVal1;
    v9[2].uvSet1.v = bandsVal2;
    v9[0].v1.y = bandsVal2 * v7 - 0.5f;
    v9[3].uvSet1.v = bandsVal1;
    v9[0].v1.x = 0.0;
    v9[0].uvSet1.u = 0.0;
    v9[1].v1.x = 0.0;
    v9[1].uvSet1.u = 0.0;
    v9[1].v1.y = bandsVal1 * v7;
    v9[2].uvSet1.u = 1.0;
    v9[3].uvSet1.u = 1.0;
    v1 = &v9[0];
    v2 = 4;
    v9[2].v1.x = v8 - 0.5f;
    v9[2].v1.y = v9[0].v1.y;
    v9[3].v1.x = v9[2].v1.x;
    v9[3].v1.y = v9[1].v1.y;
    do
    {
        v1->v1.z = 0.0f;
        v1->hw = 1.0f;
        v1->color = 0x7F7F7F7F;
        v1++;
        --v2;
    } while (v2);
    D3D_DrawASurface(v9, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v3 = 0;
    for (i = 0; i < 3; ++i)
    {
        if (textureStageTypeConfig[i].d3d_colorOp != D3DTOP_DISABLE)
        {
            v5 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_colorOp = D3DTOP_DISABLE;
            v5->lpVtbl->SetTextureStageState(v5, v3, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }
        if (textureStageTypeConfig[i].d3d_alphaOp != D3DTOP_DISABLE)
        {
            v6 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_alphaOp = D3DTOP_DISABLE;
            v6->lpVtbl->SetTextureStageState(v6, v3, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v3++, NULL);
    }
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
}

void D3D_Unkown_AE_Func_2(IDirect3DTexture9* a1, float a2)
{
    double v2;
    int v3;
    tVertex_type_104* v4;
    int v5;
    float v6;
    tVertex_type_104 v7[4];
    float v8;

    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (a2 >= 0.5)
    {
        if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_ADD)
        {
            textureStageTypeConfig[0].d3d_colorOp = D3DTOP_ADD;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_ADD);
        }
        v2 = a2 - 0.5;
    }
    else
    {
        if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_SUBTRACT)
        {
            textureStageTypeConfig[0].d3d_colorOp = D3DTOP_SUBTRACT;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
        }
        v2 = 0.5 - a2;
    }
    v3 = (((((unsigned int)(v2 * 255.0) | 0xFF000000) << 8) | (unsigned int)(v2 * 255.0) | 0xFF000000) << 16) | (((unsigned int)(v2 * 255.0) | 0xFF000000) << 8) | (unsigned int)(v2 * 255.0) | 0xFF000000;
    if (renderStateConfig.d3dTextureFactor != v3)
    {
        renderStateConfig.d3dTextureFactor = (((((unsigned int)(v2 * 255.0) | 0xFF000000) << 8) | (unsigned int)(v2 * 255.0) | 0xFF000000) << 16) | (((unsigned int)(v2 * 255.0) | 0xFF000000) << 8) | (unsigned int)(v2 * 255.0) | 0xFF000000;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 60, v3);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2 != D3DTA_TFACTOR)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = D3DTA_TFACTOR;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    v6 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v8 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v7[0].uvSet1.v = bandsVal2;
    v7[2].uvSet1.v = bandsVal2;
    v7[0].v1.y = bandsVal2 * v8 - 0.5f;
    v7[3].uvSet1.v = bandsVal1;
    v7[0].v1.x = 0.0;
    v7[0].uvSet1.u = 0.0;
    v7[1].v1.x = 0.0;
    v7[1].uvSet1.u = 0.0;
    v7[1].v1.y = bandsVal1 * v8;
    v7[1].uvSet1.v = bandsVal1;
    v7[2].uvSet1.u = 1.0;
    v7[3].uvSet1.u = 1.0;
    v4 = &v7[0];
    v5 = 4;
    v7[2].v1.x = v6 - 0.5f;
    v7[2].v1.y = v7[0].v1.y;
    v7[3].v1.x = v7[2].v1.x;
    v7[3].v1.y = v7[1].v1.y;
    do
    {
        v4->v1.z = 0.0f;
        v4->hw = 1.0f;
        v4++;
        --v5;
    } while (v5);
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_ALPHABLENDENABLE, FALSE);
    }
    D3D_DrawASurface(v7, D3DFVF_TEX1 | D3DFVF_XYZRHW);
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
}

void D3D_ColorBalanceLessThanZeroPointHalf(IDirect3DTexture9* a1, float a2)
{
    tVertex_type_144* v2;
    int v3;
    float v4;
    float v5;
    tVertex_type_144 v6[4];

    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_BLENDDIFFUSEALPHA)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_BLENDDIFFUSEALPHA;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != D3DTA_TEXTURE)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = D3DTA_TEXTURE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_SELECTARG1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_SELECTARG1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = D3DTA_DIFFUSE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    v5 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v4 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v6[0].uvSet1.v = bandsVal2;
    v6[2].uvSet1.v = bandsVal2;
    v6[3].uvSet1.v = bandsVal1;
    v6[0].v1.y = bandsVal2 * v4 - 0.5f;
    v6[0].v1.x = 0.0;
    v6[0].uvSet1.u = 0.0;
    v6[1].v1.x = 0.0;
    v6[1].uvSet1.u = 0.0;
    v6[1].uvSet1.v = bandsVal1;
    v6[1].v1.y = bandsVal1 * v4;
    v6[2].uvSet1.u = 1.0;
    v6[3].uvSet1.u = 1.0;
    v2 = &v6[0];
    v3 = 4;
    v6[2].v1.x = v5 - 0.5f;
    v6[2].v1.y = v6[0].v1.y;
    v6[3].v1.x = v6[2].v1.x;
    v6[3].v1.y = v6[1].v1.y;
    do
    {
        v2->v1.z = 0.0f;
        v2->hw = 1.0f;
        v2->color = ((unsigned int)(a2 * 510.0) << 24) | 0x808080;
        v2++;
        --v3;
    } while (v3);
    D3D_DrawASurface(v6, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    if (textureStageTypeConfig[0].d3d_colorOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_colorOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != D3DTOP_DISABLE)
    {
        textureStageTypeConfig[0].d3d_alphaOp = D3DTOP_DISABLE;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, 0);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
}

void D3D_ColorBalanceMoreThanZeroPointHalfSD(int a1, int a2, IDirect3DTexture9* a3)
{
    tVertex_type_144* v4;
    int v5;
    tVertex_type_144* v6;
    int v7;
    tVertex_type_144* v8;
    int v9;
    int v10;
    int i;
    IDirect3DDevice9* v12;
    IDirect3DDevice9* v13;
    IDirect3DDevice9* v14;
    float v15;
    float v16;
    tVertex_type_144 a1a[4];

    v16 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v15 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1a[0].uvSet1.v = bandsVal2;
    a1a[1].uvSet1.v = bandsVal1;
    a1a[2].uvSet1.v = bandsVal2;
    a1a[3].uvSet1.v = bandsVal1;
    a1a[0].v1.y = bandsVal2 * v15 - 0.5f;
    a1a[0].v1.x = 0.0;
    a1a[0].uvSet1.u = 0.0;
    a1a[1].v1.x = 0.0;
    a1a[1].uvSet1.u = 0.0;
    a1a[2].uvSet1.u = 1.0;
    a1a[1].v1.y = bandsVal1 * v15;
    a1a[3].uvSet1.u = 1.0;
    v4 = &a1a[0];
    v5 = 4;
    a1a[2].v1.x = v16 - 0.5f;
    a1a[2].v1.y = a1a[0].v1.y;
    a1a[3].v1.x = a1a[2].v1.x;
    a1a[3].v1.y = a1a[1].v1.y;
    do
    {
        v4->v1.z = 0.0f;
        v4->hw = 1.0f;
        v4++;
        --v5;
    } while (v5);
    
    v6 = &a1a[0];
    v7 = 4;
    do
    {
        v6->color = a2;
        v6++;
        --v7;
    } while (v7);
    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, 0);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[1].d3d_colorOp != 7)
    {
        textureStageTypeConfig[1].d3d_colorOp = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, 7);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAARG1, 0);
    }
    if (textureStageTypeConfig[1].d3dTexCoordIndex)
    {
        textureStageTypeConfig[1].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_TEXCOORDINDEX, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[2].d3d_colorOp != 1)
    {
        textureStageTypeConfig[2].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLOROP, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAOP, 1);
    }
    D3D_DrawASurface(a1a, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v8 = &a1a[0];
    v9 = 4;
    do
    {
        v8->color = a1;
        v8++;
        --v9;
    } while (v9);
    if (textureStageTypeConfig[0].d3d_colorOp != 10)
    {
        textureStageTypeConfig[0].d3d_colorOp = 10;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, 10);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, 0);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[1].d3d_colorOp != 1)
    {
        textureStageTypeConfig[1].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, 1);
    }
    D3D_DrawASurface(a1a, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v10 = 0;
    for (i = 0; i < 2; ++i)
    {
        if (textureStageTypeConfig[i].d3d_colorOp != 1)
        {
            v12 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_colorOp = 1;
            v12->lpVtbl->SetTextureStageState(v12, v10, D3DTSS_COLOROP, 1);
        }
        if (textureStageTypeConfig[i].d3d_alphaOp != 1)
        {
            v13 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_alphaOp = 1;
            v13->lpVtbl->SetTextureStageState(v13, v10, D3DTSS_ALPHAOP, 1);
        }
        if (textureStageTypeConfig[i].d3dTexCoordIndex != v10)
        {
            v14 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3dTexCoordIndex = v10;
            v14->lpVtbl->SetTextureStageState(v14, v10, D3DTSS_TEXCOORDINDEX, v10);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v10++, 0);
    }
}

void D3D_ColorBalanceMoreThanZeroPointHalfHD(int a1, IDirect3DTexture9* a2, int a3)
{
    tVertex_type_144* v4;
    int v5;
    int v6;
    int i;
    IDirect3DDevice9* v8;
    IDirect3DDevice9* v9;
    IDirect3DDevice9* v10;
    float v11;
    float v12;
    tVertex_type_144 a1a[4];

    if (renderStateConfig.d3dTextureFactor != a1)
    {
        renderStateConfig.d3dTextureFactor = a1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 60, a1);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLOROP, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_COLORARG2, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAOP, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_ALPHAARG1, 0);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, D3DTSS_TEXCOORDINDEX, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a2);
    if (textureStageTypeConfig[1].d3d_colorOp != 7)
    {
        textureStageTypeConfig[1].d3d_colorOp = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLOROP, 7);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG1, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_COLORARG2, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAOP, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_ALPHAARG1, 0);
    }
    if (textureStageTypeConfig[1].d3dTexCoordIndex)
    {
        textureStageTypeConfig[1].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, D3DTSS_TEXCOORDINDEX, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a2);
    if (textureStageTypeConfig[2].d3d_colorOp != 10)
    {
        textureStageTypeConfig[2].d3d_colorOp = 10;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLOROP, 10);
    }
    if (textureStageTypeConfig[2].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[2].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLORARG1, 1);
    }
    if (textureStageTypeConfig[2].d3d_colorarg2 != 3)
    {
        textureStageTypeConfig[2].d3d_colorarg2 = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_COLORARG2, 3);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAOP, 2);
    }
    if (textureStageTypeConfig[2].d3d_alphaArg1)
    {
        textureStageTypeConfig[2].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_ALPHAARG1, 0);
    }
    if (textureStageTypeConfig[2].d3dTexCoordIndex)
    {
        textureStageTypeConfig[2].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, D3DTSS_TEXCOORDINDEX, 0);
    }
    if (textureStageTypeConfig[3].d3d_colorOp != 1)
    {
        textureStageTypeConfig[3].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_COLOROP, 1);
    }
    if (textureStageTypeConfig[3].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[3].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, D3DTSS_ALPHAOP, 1);
    }
    v12 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v11 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1a[0].uvSet1.v = bandsVal2;
    a1a[2].uvSet1.v = bandsVal2;
    a1a[3].uvSet1.v = bandsVal1;
    a1a[0].v1.y = bandsVal2 * v11 - 0.5f;
    a1a[0].v1.x = 0.0;
    a1a[0].uvSet1.u = 0.0;
    a1a[1].v1.x = 0.0;
    a1a[1].uvSet1.u = 0.0;
    a1a[1].uvSet1.v = bandsVal1;
    a1a[1].v1.y = bandsVal1 * v11;
    a1a[2].uvSet1.u = 1.0;
    a1a[3].uvSet1.u = 1.0;
    v4 = &a1a[0];
    v5 = 4;
    a1a[2].v1.x = v12 - 0.5f;
    a1a[2].v1.y = a1a[0].v1.y;
    a1a[3].v1.x = a1a[2].v1.x;
    a1a[3].v1.y = a1a[1].v1.y;
    do
    {
        v4->v1.z = 0.0f;
        v4->hw = 1.0f;
        v4->color = a3;
        v4++;
        --v5;
    } while (v5);
    D3D_DrawASurface(a1a, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v6 = 0;
    for (i = 0; i < 3; ++i)
    {
        if (textureStageTypeConfig[i].d3d_colorOp != 1)
        {
            v8 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_colorOp = 1;
            v8->lpVtbl->SetTextureStageState(v8, v6, D3DTSS_COLOROP, 1);
        }
        if (textureStageTypeConfig[i].d3d_alphaOp != 1)
        {
            v9 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_alphaOp = 1;
            v9->lpVtbl->SetTextureStageState(v9, v6, D3DTSS_ALPHAOP, 1);
        }
        if (textureStageTypeConfig[i].d3dTexCoordIndex != v6)
        {
            v10 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3dTexCoordIndex = v6;
            v10->lpVtbl->SetTextureStageState(v10, v6, D3DTSS_TEXCOORDINDEX, v6);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v6++, 0);
    }
}

void D3D_ColorBalanceMoreThanZeroPointHalf(IDirect3DTexture9* a1, float a2)
{
    double v2;
    int v3; 
    int v4;

    v2 = a2 + a2 - 1.0;
    v3 = (((((unsigned int)(255.0 * v2) | 0xFF000000) << 8) | (unsigned int)(255.0 * v2) | 0xFF000000) << 16) | (((unsigned int)(255.0 * v2) | 0xFF000000) << 8) | (unsigned int)(255.0 * v2) | 0xFF000000;
    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
    v4 = (((((unsigned int)(v2 * 128.0) | 0xFF000000) << 8) | (unsigned int)(v2 * 128.0) | 0xFF000000) << 16) | (((unsigned int)(v2 * 128.0) | 0xFF000000) << 8) | (unsigned int)(v2 * 128.0) | 0xFF000000;
    if (gMaxSimultaneousTextures == 2)
        D3D_ColorBalanceMoreThanZeroPointHalfSD(v4, v3, a1);
    else
        D3D_ColorBalanceMoreThanZeroPointHalfHD(v4, a1, v3);
    if (renderStateConfig.d3dColorWriteEnable != (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE))
    {
        renderStateConfig.d3dColorWriteEnable = (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    }
}

void D3D_ColorBalance(IDirect3DTexture9* a1, float a2)
{
    if (a2 < 0.5)
        D3D_ColorBalanceLessThanZeroPointHalf(a1, a2);
    else
        D3D_ColorBalanceMoreThanZeroPointHalf(a1, a2);
}

void D3D_WaterBlurHD(tUnkWaterBlurStruct* a1, IDirect3DTexture9* a2, tUnkWaterBlurStruct* a3, float a4, float a5, float a6)
{
    double v7; // st7
    double v8; // st6
    double v9; // st5
    int v10; // ecx
    double v11; // st4
    double v12; // st5
    tVertex_type_404* v13; // eax
    int v14; // ecx
    int v15; // esi
    int v16; // edi
    int v17; // ebx
    IDirect3DDevice9* v18; // eax
    IDirect3DDevice9* v19; // eax
    IDirect3DDevice9* v20; // eax
    IDirect3DDevice9* v21; // eax
    float v22; // [esp+21Ch] [ebp-E4h]
    float v23; // [esp+21Ch] [ebp-E4h]
    float v24; // [esp+21Ch] [ebp-E4h]
    float v25; // [esp+220h] [ebp-E0h]
    float v26; // [esp+224h] [ebp-DCh]
    int v27; // [esp+228h] [ebp-D8h]
    float v28; // [esp+228h] [ebp-D8h]
    int v29; // [esp+22Ch] [ebp-D4h]
    float v30; // [esp+22Ch] [ebp-D4h]
    int v31; // [esp+230h] [ebp-D0h]
    float v32; // [esp+230h] [ebp-D0h]
    int v33; // [esp+234h] [ebp-CCh]
    float v34; // [esp+234h] [ebp-CCh]
    float v35; // [esp+238h] [ebp-C8h]
    float v36; // [esp+23Ch] [ebp-C4h]
    tVertex_type_404 a1a[4]; // [esp+240h] [ebp-C0h] BYREF

    if (renderStateConfig.d3dTextureFactor != 0x80FFFFFF)
    {
        renderStateConfig.d3dTextureFactor = 0x80FFFFFF;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 60, 0x80FFFFFF);
    }
    if (samplerConfig[0].d3d_minfilter != 2)
    {
        samplerConfig[0].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 6, 2);
    }
    if (samplerConfig[0].d3d_magfilter != 2)
    {
        samplerConfig[0].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != 2)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a2);
    if (samplerConfig[1].d3d_minfilter != 2)
    {
        samplerConfig[1].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, 6, 2);
    }
    if (samplerConfig[1].d3d_magfilter != 2)
    {
        samplerConfig[1].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, 5, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != 14)
    {
        textureStageTypeConfig[1].d3d_colorOp = 14;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 14);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1 != 1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a2);
    if (samplerConfig[2].d3d_minfilter != 2)
    {
        samplerConfig[2].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 2, 6, 2);
    }
    if (samplerConfig[2].d3d_magfilter != 2)
    {
        samplerConfig[2].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 2, 5, 2);
    }
    if (textureStageTypeConfig[2].d3d_colorOp != 14)
    {
        textureStageTypeConfig[2].d3d_colorOp = 14;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 14);
    }
    if (textureStageTypeConfig[2].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[2].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 2, 2);
    }
    if (textureStageTypeConfig[2].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[2].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 3, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 2);
    }
    if (textureStageTypeConfig[2].d3d_alphaArg1 != 1)
    {
        textureStageTypeConfig[2].d3d_alphaArg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 5, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a2);
    if (samplerConfig[3].d3d_minfilter != 2)
    {
        samplerConfig[3].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 3, 6, 2);
    }
    if (samplerConfig[3].d3d_magfilter != 2)
    {
        samplerConfig[3].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 3, 5, 2);
    }
    if (textureStageTypeConfig[3].d3d_colorOp != 14)
    {
        textureStageTypeConfig[3].d3d_colorOp = 14;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 1, 14);
    }
    if (textureStageTypeConfig[3].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[3].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 2, 2);
    }
    if (textureStageTypeConfig[3].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[3].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 3, 1);
    }
    if (textureStageTypeConfig[3].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[3].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 4, 2);
    }
    if (textureStageTypeConfig[3].d3d_alphaArg1 != 1)
    {
        textureStageTypeConfig[3].d3d_alphaArg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 5, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)a2);
    if (textureStageTypeConfig[4].d3d_colorOp != 1)
    {
        textureStageTypeConfig[4].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 4, 1, 1);
    }
    if (textureStageTypeConfig[4].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[4].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 4, 4, 1);
    }
    v22 = (float)(unsigned int)(a1->i5 - 1);
    v7 = (double)a1->i1 / v22;
    v8 = (double)a1->i3 / v22;
    v9 = (double)(unsigned int)(a1->i6 - 1);
    v31 = a3->i3;
    v27 = v31 - a3->i1;
    v29 = a3->i1;
    v10 = a3->i2;
    v33 = a3->i4;
    v25 = (float)((double)a1->i2 / v9);
    v11 = (double)a1->i4;
    a1a[1].uvSet1.v = v25;
    a1a[1].uvSet2.v = v25;
    v23 = (float)(v11 / v9);
    v12 = (a6 - 1.0) * 0.5;
    v28 = (float)((double)v27 * v12);
    v26 = (float)((double)(v33 - v10) * v12);
    a1a[0].v1.x = (float)((double)v29 - v28);
    a1a[0].v1.y = (float)((double)v33 + v26);
    a1a[0].uvSet1.u = (float)v7;
    v34 = (float)(a5 * 0.0 + v23);
    a1a[0].uvSet1.v = v34;
    a1a[0].uvSet2.u = (float)(v7 - a4);
    v30 = v23 + a5;
    a1a[0].uvSet2.v = v30;
    a1a[0].uvSet3.u = (float)(v7 + a4);
    v35 = a5 + a5 + v23;
    a1a[0].uvSet3.v = v35;
    a1a[0].uvSet4.u = (float)v7;
    v24 = (float)(a5 * 3.0 + v23);
    a1a[0].uvSet4.v = v24;
    a1a[1].v1.x = a1a[0].v1.x;
    v36 = (float)((double)v10 - v26);
    a1a[1].v1.y = v36;
    a1a[1].uvSet1.u = (float)v7;
    a1a[1].uvSet2.u = a1a[0].uvSet2.u;
    a1a[1].uvSet3.u = a1a[0].uvSet3.u;
    a1a[1].uvSet4.u = (float)v7;
    a1a[1].uvSet3.v = v25;
    a1a[1].uvSet4.v = v25;
    a1a[2].uvSet1.v = v34;
    v32 = (float)((double)v31 + v28);
    a1a[2].uvSet2.v = v30;
    a1a[2].v1.x = v32;
    a1a[2].uvSet3.v = v35;
    a1a[2].v1.y = a1a[0].v1.y;
    a1a[2].uvSet4.v = v24;
    a1a[2].uvSet1.u = (float)v8;
    a1a[3].v1.x = v32;
    a1a[3].v1.y = v36;
    a1a[2].uvSet2.u = (float)(v8 - a4);
    a1a[3].uvSet1.v = v25;
    a1a[3].uvSet2.v = v25;
    a1a[3].uvSet4.v = v25;
    a1a[3].uvSet3.v = v25;
    v13 = &a1a[0];
    a1a[2].uvSet3.u = (float)(v8 + a4);
    v14 = 4;
    a1a[2].uvSet4.u = (float)v8;
    a1a[3].uvSet1.u = a1a[2].uvSet4.u;
    a1a[3].uvSet2.u = a1a[2].uvSet2.u;
    a1a[3].uvSet3.u = a1a[2].uvSet3.u;
    a1a[3].uvSet4.u = a1a[2].uvSet4.u;
    do
    {
        v13->v1.z = 0.0f;
        v13->hw = 1.0f;
        v13++;
        --v14;
    } while (v14);
    D3D_DrawASurface(a1a, D3DFVF_TEX4 | D3DFVF_XYZRHW);
    v15 = 0;
    v16 = 0;
    v17 = 0;
    do
    {
        if (samplerConfig[v16].d3d_minfilter != 1)
        {
            v18 = D3DRenderParams.d3dDev;
            samplerConfig[v16].d3d_minfilter = 1;
            v18->lpVtbl->SetSamplerState(v18, v15, 6, 1);
        }
        if (samplerConfig[v16].d3d_magfilter != 1)
        {
            v19 = D3DRenderParams.d3dDev;
            samplerConfig[v16].d3d_magfilter = 1;
            v19->lpVtbl->SetSamplerState(v19, v15, 5, 1);
        }
        if (textureStageTypeConfig[v17].d3d_colorOp != 1)
        {
            v20 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v17].d3d_colorOp = 1;
            v20->lpVtbl->SetTextureStageState(v20, v15, 1, 1);
        }
        if (textureStageTypeConfig[v17].d3d_alphaOp != 1)
        {
            v21 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v17].d3d_alphaOp = 1;
            v21->lpVtbl->SetTextureStageState(v21, v15, 4, 1);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v15, NULL);
        ++v16;
        ++v15;
        ++v17;
    } while (v16 < 4);
}

void D3D_WaterBlurSD(IDirect3DTexture9* tex0, tUnkWaterBlurStruct* a6, tUnkWaterBlurStruct* a7, float tex1_2, float tex1)
{
    tVertex_type_204* v5; // eax
    int v6; // ecx
    int v7; // edi
    int i; // esi
    IDirect3DDevice9* v9; // eax
    IDirect3DDevice9* v10; // eax
    int v11; // esi
    int v12; // edi
    int v13; // ebx
    IDirect3DDevice9* v14; // eax
    IDirect3DDevice9* v15; // eax
    IDirect3DDevice9* v16; // eax
    IDirect3DDevice9* v17; // eax
    float v18; // [esp+1ECh] [ebp-8Ch]
    float v19; // [esp+1ECh] [ebp-8Ch]
    float v20; // [esp+1ECh] [ebp-8Ch]
    float v21; // [esp+1F0h] [ebp-88h]
    float v22; // [esp+1F0h] [ebp-88h]
    float v23; // [esp+1F4h] [ebp-84h]
    float v24; // [esp+1F4h] [ebp-84h]
    tVertex_type_204 a1[4]; // [esp+1F8h] [ebp-80h] BYREF

    v18 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v23 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1[0].uvSet1.v = bandsVal4;
    a1[0].uvSet2.v = bandsVal4;
    a1[0].v1.y = bandsVal4 * v23 - 0.5f;
    a1[1].uvSet1.v = bandsVal3;
    a1[0].uvSet2.u = tex1_2;
    a1[0].uvSet1.u = -tex1_2;
    a1[1].uvSet2.u = tex1_2;
    a1[1].uvSet2.v = bandsVal3;
    a1[2].uvSet1.v = bandsVal4;
    a1[2].uvSet2.v = bandsVal4;
    v24 = bandsVal3 * v23;
    a1[1].v1.y = v24;
    a1[3].v1.y = v24;
    a1[1].uvSet1.u = a1[0].uvSet1.u;
    a1[3].uvSet1.v = bandsVal3;
    a1[0].v1.x = 0.0;
    a1[1].v1.x = 0.0;
    a1[2].v1.x = v18 - 0.5f;
    a1[2].v1.y = a1[0].v1.y;
    v19 = 1.0f - tex1_2;
    a1[2].uvSet1.u = v19;
    a1[3].uvSet1.u = v19;
    a1[3].uvSet2.v = bandsVal3;
    v5 = &a1[0];
    v21 = tex1_2 + 1.0f;
    a1[2].uvSet2.u = v21;
    a1[3].uvSet2.u = v21;
    a1[3].v1.x = a1[2].v1.x;
    v6 = 4;
    do
    {
        v5->v1.z = 0.0f;
        v5->hw = 1.0f;
        v5++;
        --v6;
    } while (v6);
    v7 = 0;
    for (i = 0; i < 2; ++i)
    {
        if (samplerConfig[i].d3d_minfilter != 2)
        {
            v9 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_minfilter = 2;
            v9->lpVtbl->SetSamplerState(v9, v7, 6, 2);
        }
        if (samplerConfig[i].d3d_magfilter != 2)
        {
            v10 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_magfilter = 2;
            v10->lpVtbl->SetSamplerState(v10, v7, 5, 2);
        }
        ++v7;
    }
    if (renderStateConfig.d3dTextureFactor != 0x80FFFFFF)
    {
        renderStateConfig.d3dTextureFactor = 0x80FFFFFF;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 60, 0x80FFFFFF);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != 3)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 3);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)tex0);
    if (textureStageTypeConfig[1].d3d_colorOp != 14)
    {
        textureStageTypeConfig[1].d3d_colorOp = 14;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 14);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1 != 3)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 3);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)tex0);
    if (textureStageTypeConfig[2].d3d_colorOp != 1)
    {
        textureStageTypeConfig[2].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 1);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
    D3D_DrawASurface(a1, D3DFVF_TEX2 | D3DFVF_XYZRHW);
    a1[0].uvSet1.v = bandsVal4 - tex1;
    a1[0].uvSet1.u = 0.0;
    a1[0].uvSet2.u = 0.0;
    a1[1].uvSet1.u = 0.0;
    a1[1].uvSet2.u = 0.0;
    a1[2].uvSet1.u = 1.0;
    a1[0].uvSet2.v = bandsVal4 + tex1;
    a1[2].uvSet2.u = 1.0;
    a1[3].uvSet1.u = 1.0;
    a1[3].uvSet2.u = 1.0;
    v22 = bandsVal3 - tex1;
    a1[1].uvSet1.v = v22;
    a1[3].uvSet1.v = v22;
    v20 = bandsVal3 + tex1;
    a1[1].uvSet2.v = v20;
    a1[3].uvSet2.v = v20;
    a1[2].uvSet1.v = a1[0].uvSet1.v;
    a1[2].uvSet2.v = a1[0].uvSet2.v;
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != 3)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 3);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)tex0);
    if (textureStageTypeConfig[1].d3d_colorOp != 14)
    {
        textureStageTypeConfig[1].d3d_colorOp = 14;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 14);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1 != 3)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 3);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)tex0);
    if (textureStageTypeConfig[2].d3d_colorOp != 1)
    {
        textureStageTypeConfig[2].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 1);
    }
    if (renderStateConfig.d3dAlphaBlendEnable != 1)
    {
        renderStateConfig.d3dAlphaBlendEnable = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 1);
    }
    if (renderStateConfig.d3d_BlendOp != 1)
    {
        renderStateConfig.d3d_BlendOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 171, 1);
    }
    if (renderStateConfig.d3dSrcBlend != 5)
    {
        renderStateConfig.d3dSrcBlend = 5;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 5);
    }
    if (renderStateConfig.d3dDestBlend != 5)
    {
        renderStateConfig.d3dDestBlend = 5;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 5);
    }
    D3D_DrawASurface(a1, D3DFVF_TEX2 | D3DFVF_XYZRHW);
    v11 = 0;
    v12 = 0;
    v13 = 0;
    do
    {
        if (samplerConfig[v12].d3d_minfilter != 1)
        {
            v14 = D3DRenderParams.d3dDev;
            samplerConfig[v12].d3d_minfilter = 1;
            v14->lpVtbl->SetSamplerState(v14, v11, 6, 1);
        }
        if (samplerConfig[v12].d3d_magfilter != 1)
        {
            v15 = D3DRenderParams.d3dDev;
            samplerConfig[v12].d3d_magfilter = 1;
            v15->lpVtbl->SetSamplerState(v15, v11, 5, 1);
        }
        if (textureStageTypeConfig[v13].d3d_colorOp != 1)
        {
            v16 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v13].d3d_colorOp = 1;
            v16->lpVtbl->SetTextureStageState(v16, v11, 1, 1);
        }
        if (textureStageTypeConfig[v13].d3d_alphaOp != 1)
        {
            v17 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v13].d3d_alphaOp = 1;
            v17->lpVtbl->SetTextureStageState(v17, v11, 4, 1);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v11, NULL);
        ++v12;
        ++v11;
        ++v13;
    } while (v12 < 2);
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
}

void __cdecl D3D_WaterBlur(IDirect3DTexture9* tex0, tUnkWaterBlurStruct* a5, IDirect3DTexture9* a6, tUnkWaterBlurStruct* a7, float a8, float tex1, float a10)
{
    int v7; // esi
    double v8; // st7
    IDirect3DDevice9* v9; // ecx
    int v10; // edi
    int v11; // ebx
    IDirect3DDevice9* v12; // eax
    IDirect3DDevice9* v13; // eax
    IDirect3DDevice9* v14; // eax
    IDirect3DDevice9* v15; // eax
    IDirect3DDevice9* v16; // eax
    IDirect3DDevice9* v17; // eax
    IDirect3DDevice9* v18; // eax
    IDirect3DDevice9* v19; // eax
    IDirect3DSurface9* a11; // [esp+19Ch] [ebp-28h] BYREF
    IDirect3DSurface9* a12; // [esp+1A0h] [ebp-24h] BYREF
    float tex1_2; // [esp+1A4h] [ebp-20h]
    int v23; // [esp+1A8h] [ebp-1Ch]
    D3DVIEWPORT9 v24;
    int v25; // [esp+1D0h] [ebp+Ch]
    int v26; // [esp+1D8h] [ebp+14h] FORCED
    int a8a;
    IDirect3DSurface9* a8b;

    v7 = 0;
    v23 = a5->i5 - 1;
    a11 = NULL;
    a12 = NULL;
    v8 = a8 / (double)(unsigned int)v23;
    a8a = a5->i6 - 1;
    tex1_2 = (float)v8;
    v9 = D3DRenderParams.d3dDev;
    tex1 = (float)(tex1 / (double)a8a);
    v9->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &a11);
    D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &v24);
    a6->lpVtbl->GetSurfaceLevel(a6, 0, &a8b);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, a8b);
    a8b->lpVtbl->Release(a8b);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &a12);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, NULL);
    v23 = renderStateConfig.d3d_zEnable;
    if (renderStateConfig.d3d_zEnable)
    {
        renderStateConfig.d3d_zEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 7, 0);
    }
    if (renderStateConfig.d3d_zWriteEnable)
    {
        renderStateConfig.d3d_zWriteEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 14, 0);
    }
    v26 = D3DRenderParams.d3dFogModeProbably;
    D3DRenderParams.d3dFogModeProbably = 0;
    v25 = renderStateConfig.d3dFogEnable;
    if (renderStateConfig.d3dFogEnable)
    {
        renderStateConfig.d3dFogEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 28, 0);
    }
    if (renderStateConfig.d3dFillMode != 3)
    {
        renderStateConfig.d3dFillMode = 3;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 8, 3);
    }
    if (renderStateConfig.d3dCullMode != 1)
    {
        renderStateConfig.d3dCullMode = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 22, 1);
    }
    if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 15, 0);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
    if (renderStateConfig.d3d_BlendOp != 1)
    {
        renderStateConfig.d3d_BlendOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 171, 1);
    }
    if (renderStateConfig.d3dSrcBlend != 2)
    {
        renderStateConfig.d3dSrcBlend = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 2);
    }
    if (renderStateConfig.d3dDestBlend != 1)
    {
        renderStateConfig.d3dDestBlend = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 1);
    }
    if (renderStateConfig.d3d_Lighting)
    {
        renderStateConfig.d3d_Lighting = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 137, 0);
    }
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
    v10 = 0;
    v11 = 0;
    do
    {
        if (textureStageTypeConfig[v10].d3d_colorOp != 1)
        {
            v12 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v10].d3d_colorOp = 1;
            v12->lpVtbl->SetTextureStageState(v12, v7, 1, 1);
        }
        if (textureStageTypeConfig[v10].d3d_alphaOp != 1)
        {
            v13 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v10].d3d_alphaOp = 1;
            v13->lpVtbl->SetTextureStageState(v13, v7, 4, 1);
        }
        if (textureStageTypeConfig[v10].d3dTexCoordIndex != v7)
        {
            v14 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v10].d3dTexCoordIndex = v7;
            v14->lpVtbl->SetTextureStageState(v14, v7, 11, v7);
        }
        if (textureStageTypeConfig[v10].d3dTextureTransformFlags)
        {
            v15 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v10].d3dTextureTransformFlags = 0;
            v15->lpVtbl->SetTextureStageState(v15, v7, 24, 0);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v7, NULL);
        if (samplerConfig[v11].d3d_minfilter != 1)
        {
            v16 = D3DRenderParams.d3dDev;
            samplerConfig[v11].d3d_minfilter = 1;
            v16->lpVtbl->SetSamplerState(v16, v7, 6, 1);
        }
        if (samplerConfig[v11].d3d_magfilter != 1)
        {
            v17 = D3DRenderParams.d3dDev;
            samplerConfig[v11].d3d_magfilter = 1;
            v17->lpVtbl->SetSamplerState(v17, v7, 5, 1);
        }
        if (samplerConfig[v11].d3d_addressu != 2)
        {
            v18 = D3DRenderParams.d3dDev;
            samplerConfig[v11].d3d_addressu = 2;
            v18->lpVtbl->SetSamplerState(v18, v7, 1, 2);
        }
        if (samplerConfig[v11].d3d_addressv != 2)
        {
            v19 = D3DRenderParams.d3dDev;
            samplerConfig[v11].d3d_addressv = 2;
            v19->lpVtbl->SetSamplerState(v19, v7, 2, 2);
        }
        ++v10;
        ++v7;
        ++v11;
    } while (v10 < 4);
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_WaterBlurHD(a5, tex0, a7, tex1_2, tex1, a10);
    else
        D3D_WaterBlurSD(tex0, a5, a7, tex1_2, tex1);
    D3DRenderParams.d3dFogModeProbably = v26;
    if (renderStateConfig.d3dFogEnable != v25)
    {
        renderStateConfig.d3dFogEnable = v25;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 28, v25);
    }
    if (renderStateConfig.d3d_zEnable != v23)
    {
        renderStateConfig.d3d_zEnable = v23;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 7, v23);
    }
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, a11);
    D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &v24);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, a12);
    a11->lpVtbl->Release(a11);
    a12->lpVtbl->Release(a12);
}

void D3D_DoBlurWaterTexture()
{
    int v0; // ecx
    int v1; // edx
    IDirect3DSurface9* v2; // [esp+20h] [ebp-58h] BYREF
    IDirect3DSurface9* v3; // [esp+24h] [ebp-54h] BYREF
    tUnkWaterBlurStruct a7; // [esp+28h] [ebp-50h] BYREF
    tUnkWaterBlurStruct a5; // [esp+40h] [ebp-38h] BYREF
    D3DSURFACE_DESC v6; // [esp+58h] [ebp-20h] BYREF

    D3DRenderParams.blurRenderTarget->lpVtbl->GetSurfaceLevel(D3DRenderParams.blurRenderTarget, 0, &v2);
    waterTexture->lpVtbl->GetSurfaceLevel(waterTexture, 0, &v3);
    a5.i3 = 511;
    a5.i4 = 511;
    a5.i1 = 0;
    a5.i2 = 0;
    a5.i5 = 512;
    a5.i6 = 512;
    v2->lpVtbl->GetDesc(v2, &v6);
    if (v6.Width <= 0x200)
    {
        a7.i1 = 0;
        v0 = v6.Width - 1;
    }
    else
    {
        a7.i1 = (v6.Width - 512) >> 1;
        v0 = a7.i1 + 511;
    }
    a7.i3 = v0;
    if (v6.Height <= 512)
    {
        a7.i2 = 0;
        v1 = v6.Height - 1;
    }
    else
    {
        a7.i2 = (v6.Height - 512) >> 1;
        v1 = a7.i2 + 511;
    }
    a7.i5 = v6.Width;
    a7.i6 = v6.Height;
    a7.i4 = v1;
    D3D_WaterBlur(waterTexture, &a5, D3DRenderParams.blurRenderTarget, &a7, tweak_water_smoothx, tweak_water_smoothy, 1.0f);
    D3D_WaterBlur(D3DRenderParams.blurRenderTarget, &a7, waterTexture, &a5, tweak_water_smoothx, tweak_water_smoothy, 1.0f);
    v2->lpVtbl->Release(v2);
    v3->lpVtbl->Release(v3);
}

void D3D_ZoomSmooth(IDirect3DTexture9* a1, IDirect3DTexture9* a2, float a3)
{
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int* v8;
    double v9;
    int v10;
    int v11;
    double v12;
    double v13;
    double v14;
    double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    tVertex_type_444* v20;
    int v21;
    int v22;
    int v23;
    IDirect3DDevice9* v24;
    IDirect3DDevice9* v25;
    float v26;
    float v27;
    int v28;
    float v29;
    float v30;
    float v31;
    int v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    int v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    float v60;
    float v61;
    float v62;
    tVertex_type_444 v63[4];
    float a3a;

    v3 = gMaxSimultaneousTextures;
    if (gMaxSimultaneousTextures >= 4)
        v3 = 4;
    if (v3 >= ENG_ifSingleStreamThenEq2Otherwise8)
    {
        v32 = ENG_ifSingleStreamThenEq2Otherwise8;
    }
    else if (gMaxSimultaneousTextures >= 4)
    {
        v32 = 4;
    }
    else
    {
        v32 = gMaxSimultaneousTextures;
    }
    v49 = 8 / v32;
    v28 = v32 * (8 / v32);
    a3a = (float)(8.0 / (double)v28 * a3);
    D3D_SetRenderTargetFromTexture(a2);
    if (renderStateConfig.d3dColorWriteEnable != 7)
    {
        renderStateConfig.d3dColorWriteEnable = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 7);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    v4 = 1;
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a1);
    if (v32 >= 3)
    {
        v4 = 2;
        if (textureStageTypeConfig[2].d3d_colorOp != 18)
        {
            textureStageTypeConfig[2].d3d_colorOp = 18;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 18);
        }
        if (textureStageTypeConfig[2].d3d_colorarg1 != 1)
        {
            textureStageTypeConfig[2].d3d_colorarg1 = 1;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 2, 1);
        }
        if (textureStageTypeConfig[2].d3d_colorarg2 != 2)
        {
            textureStageTypeConfig[2].d3d_colorarg2 = 2;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 3, 2);
        }
        if (textureStageTypeConfig[2].d3d_alphaOp != 2)
        {
            textureStageTypeConfig[2].d3d_alphaOp = 2;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 2);
        }
        if (textureStageTypeConfig[2].d3d_alphaArg1)
        {
            textureStageTypeConfig[2].d3d_alphaArg1 = 0;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 5, 0);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a1);
        if (v32 >= 4)
        {
            v4 = 3;
            if (textureStageTypeConfig[3].d3d_colorOp != 18)
            {
                textureStageTypeConfig[3].d3d_colorOp = 18;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 1, 18);
            }
            if (textureStageTypeConfig[3].d3d_colorarg1 != 1)
            {
                textureStageTypeConfig[3].d3d_colorarg1 = 1;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 2, 1);
            }
            if (textureStageTypeConfig[3].d3d_colorarg2 != 2)
            {
                textureStageTypeConfig[3].d3d_colorarg2 = 2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 3, 2);
            }
            if (textureStageTypeConfig[3].d3d_alphaOp != 2)
            {
                textureStageTypeConfig[3].d3d_alphaOp = 2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 4, 2);
            }
            if (textureStageTypeConfig[3].d3d_alphaArg1)
            {
                textureStageTypeConfig[3].d3d_alphaArg1 = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 5, 0);
            }
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)a1);
        }
    }
    v5 = v4 + 1;
    v6 = v4 + 1;
    v7 = textureStageTypeConfig[v6].d3d_colorOp;
    v8 = &textureStageTypeConfig[v6].d3d_colorOp;
    if (v7 != 1)
    {
        *v8 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v5, 1, 1);
    }
    if (textureStageTypeConfig[v5].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[v5].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v5, 4, 1);
    }
    v48 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v9 = bandsVal2 * 480.0;
    v10 = 0;
    v55 = (float)((v9 - 1.0) * 0.0020833334);
    v11 = ((((((256 / v28 - 1) << 8) | (256 / v28 - 1)) << 16) | ((256 / v28 - 1) << 8) | (256 / v28 - 1)) << 24) | ((((256 / v28 - 1) << 8) | (256 / v28 - 1)) << 16) | ((256 / v28 - 1) << 8) | (256 / v28 - 1);
    v34 = (float)(a3a * 6400.0 / v9 * 0.0015625);
    v33 = (float)(a3a * 10.0 * 0.0020833334);
    v29 = (float)v32;
    v12 = (double)(v49 - 1) * v29;
    v36 = (float)(v34 * v12);
    v35 = (float)(v12 * v33);
    if (v49 > 0)
    {
        v50 = (float)(v34 * 0.0);
        v52 = v34 + v34;
        v47 = (float)(v34 * 3.0);
        v37 = (float)((double)D3DRenderParams.presentParams.BackBufferWidth - 0.5);
        v13 = v33 * 0.0;
        v46 = (float)v13;
        v61 = (float)(v13 + bandsVal1);
        v53 = v33 + bandsVal1;
        v14 = v33 + v33;
        v51 = (float)v14;
        v59 = (float)(v14 + bandsVal1);
        v15 = v33 * 3.0;
        v62 = (float)(v15 + bandsVal1);
        v60 = v29 * v34;
        v38 = v29 * v33;
        do
        {
            v63[0].v1.x = 0.0;
            v63[1].v1.x = 0.0;
            v63[2].v1.x = v37;
            v63[0].v1.y = (float)(bandsVal2 * v48 - 0.5);
            v63[0].uvSet4.u = v50 + v36;
            v16 = v55 - v35;
            v26 = (float)v16;
            v57 = (float)(v16 - v46);
            v63[0].uvSet4.v = v57;
            v63[0].uvSet3.u = v36 + v34;
            v43 = v26 - v33;
            v63[0].uvSet3.v = v43;
            v30 = v52 + v36;
            v63[0].uvSet2.u = v30;
            v63[1].uvSet2.u = v30;
            v63[2].uvSet4.v = v57;
            v41 = v26 - v51;
            v63[0].uvSet2.v = v41;
            v39 = v47 + v36;
            v63[0].uvSet1.u = v39;
            v63[1].uvSet1.u = v39;
            v63[2].uvSet3.v = v43;
            v45 = (float)v15;
            v54 = v26 - v45;
            v63[0].uvSet1.v = v54;
            v56 = bandsVal1 * v48;
            v63[1].v1.y = v56;
            v63[1].uvSet4.u = v63[0].uvSet4.u;
            v58 = v61 + v35;
            v63[1].uvSet4.v = v58;
            v63[1].uvSet3.u = v63[0].uvSet3.u;
            v17 = v53 + v35;
            v63[1].uvSet3.v = (float)v17;
            v31 = v59 + v35;
            v63[1].uvSet2.v = v31;
            v40 = v62 + v35;
            v63[1].uvSet1.v = v40;
            v18 = v17;
            v63[2].v1.y = v63[0].v1.y;
            v19 = 0.99843752 - v36;
            v27 = (float)v19;
            v63[2].uvSet4.u = (float)(v19 - v50);
            v63[2].uvSet3.u = v27 - v34;
            v44 = v27 - v52;
            v63[2].uvSet1.v = v54;
            v63[2].uvSet2.u = v44;
            v63[2].uvSet2.v = v41;
            v63[3].v1.x = v37;
            v42 = v27 - v47;
            v63[3].uvSet4.v = v58;
            v63[2].uvSet1.u = v42;
            v63[3].v1.y = v56;
            v63[3].uvSet4.u = v63[2].uvSet4.u;
            v63[3].uvSet2.u = v44;
            v63[3].uvSet3.u = v63[2].uvSet3.u;
            v63[3].uvSet1.u = v42;
            v63[3].uvSet3.v = (float)v18;
            v63[3].uvSet2.v = v31;
            v63[3].uvSet1.v = v40;
            if (v10)
            {
                if (renderStateConfig.d3dAlphaBlendEnable != 1)
                {
                    renderStateConfig.d3dAlphaBlendEnable = 1;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 1);
                }
                if (renderStateConfig.d3d_BlendOp != 1)
                {
                    renderStateConfig.d3d_BlendOp = 1;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 171, 1);
                }
                if (renderStateConfig.d3dSrcBlend != 2)
                {
                    renderStateConfig.d3dSrcBlend = 2;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 2);
                }
                if (renderStateConfig.d3dDestBlend != 2)
                {
                    renderStateConfig.d3dDestBlend = 2;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 2);
                }
            }
            else if (renderStateConfig.d3dAlphaBlendEnable)
            {
                renderStateConfig.d3dAlphaBlendEnable = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
            }
            v20 = &v63[0];
            v21 = 4;
            do
            {
                v20->v1.z = 0.0f;
                v20->hw = 1.0f;
                v20->color = v11;
                ++v20;
                --v21;
            } while (v21);
            D3D_DrawASurface(v63, D3DFVF_TEX4 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
            ++v10;
            v36 = v36 - v60;
            v35 = v35 - v38;
        } while (v10 < v49);
    }
    v22 = 0;
    if (v32 > 0)
    {
        v23 = 0;
        do
        {
            if (textureStageTypeConfig[v23].d3d_colorOp != 1)
            {
                v24 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v23].d3d_colorOp = 1;
                v24->lpVtbl->SetTextureStageState(v24, v22, 1, 1);
            }
            if (textureStageTypeConfig[v23].d3d_alphaOp != 1)
            {
                v25 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v23].d3d_alphaOp = 1;
                v25->lpVtbl->SetTextureStageState(v25, v22, 4, 1);
            }
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v22++, NULL);
            ++v23;
        } while (v22 < v32);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
}

void D3D_ZoomSmooth2(IDirect3DTexture9* a1, IDirect3DTexture9* a2, float a3)
{
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int* v8;
    double v9;
    int v10;
    int v11;
    long double v12;
    tVertex_type_444* v13;
    int v14;
    long double v15;
    long double v16;
    long double v17;
    long double v18;
    long double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    double v24;
    tVertex_type_444* v25;
    int v26;
    double v27;
    int v28;
    int v29;
    IDirect3DDevice9* v30;
    IDirect3DDevice9* v31;
    int v32;
    double v33;
    double v34;
    float v35; 
    float v36; 
    float v37; 
    float v38; 
    float v39; 
    float v40; 
    int v41; 
    float v42;
    float v43;
    float v44;
    int v45; 
    float v46;
    float v47;
    float v48;
    float v49;
    double v50;
    double v51;
    double v52;
    tVertex_type_444 v53[4];

    v3 = gMaxSimultaneousTextures;
    if (gMaxSimultaneousTextures >= 4)
        v3 = 4;
    if (v3 >= ENG_ifSingleStreamThenEq2Otherwise8)
    {
        v41 = ENG_ifSingleStreamThenEq2Otherwise8;
    }
    else if (gMaxSimultaneousTextures >= 4)
    {
        v41 = 4;
    }
    else
    {
        v41 = gMaxSimultaneousTextures;
    }
    v45 = 8 / v41;
    D3D_SetRenderTargetFromTexture(a2);
    if (renderStateConfig.d3dColorWriteEnable != 7)
    {
        renderStateConfig.d3dColorWriteEnable = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 7);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    v4 = 1;
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a1);
    if (v41 >= 3)
    {
        v4 = 2;
        if (textureStageTypeConfig[2].d3d_colorOp != 18)
        {
            textureStageTypeConfig[2].d3d_colorOp = 18;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 18);
        }
        if (textureStageTypeConfig[2].d3d_colorarg1 != 1)
        {
            textureStageTypeConfig[2].d3d_colorarg1 = 1;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 2, 1);
        }
        if (textureStageTypeConfig[2].d3d_colorarg2 != 2)
        {
            textureStageTypeConfig[2].d3d_colorarg2 = 2;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 3, 2);
        }
        if (textureStageTypeConfig[2].d3d_alphaOp != 2)
        {
            textureStageTypeConfig[2].d3d_alphaOp = 2;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 2);
        }
        if (textureStageTypeConfig[2].d3d_alphaArg1)
        {
            textureStageTypeConfig[2].d3d_alphaArg1 = 0;
            D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 5, 0);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a1);
        if (v41 >= 4)
        {
            v4 = 3;
            if (textureStageTypeConfig[3].d3d_colorOp != 18)
            {
                textureStageTypeConfig[3].d3d_colorOp = 18;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 1, 18);
            }
            if (textureStageTypeConfig[3].d3d_colorarg1 != 1)
            {
                textureStageTypeConfig[3].d3d_colorarg1 = 1;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 2, 1);
            }
            if (textureStageTypeConfig[3].d3d_colorarg2 != 2)
            {
                textureStageTypeConfig[3].d3d_colorarg2 = 2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 3, 2);
            }
            if (textureStageTypeConfig[3].d3d_alphaOp != 2)
            {
                textureStageTypeConfig[3].d3d_alphaOp = 2;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 4, 2);
            }
            if (textureStageTypeConfig[3].d3d_alphaArg1)
            {
                textureStageTypeConfig[3].d3d_alphaArg1 = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 5, 0);
            }
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)a1);
        }
    }
    v5 = v4 + 1;
    v6 = v4 + 1;
    v7 = textureStageTypeConfig[v6].d3d_colorOp;
    v8 = &textureStageTypeConfig[v6].d3d_colorOp;
    if (v7 != 1)
    {
        *v8 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v4 + 1, 1, 1);
    }
    if (textureStageTypeConfig[v5].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[v5].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, v4 + 1, 4, 1);
    }
    v48 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v47 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v40 = (float)(bandsVal2 * 480.0);
    v39 = (float)(bandsVal1 * 480.0);
    v43 = (float)(a3 * 0.039999999);
    v9 = (double)v41;
    v10 = 0;
    v42 = (float)((double)v45 * v9 * v43 * -0.5);
    v32 = v41 * (8 / v41);
    v11 = ((((((256 / v32 - 1) << 8) | (256 / v32 - 1)) << 16) | ((256 / v32 - 1) << 8) | (256 / v32 - 1)) << 24) | ((((256 / v32 - 1) << 8) | (256 / v32 - 1)) << 16) | ((256 / v32 - 1) << 8) | (256 / v32 - 1);
    if (v45 > 0)
    {
        v44 = (float)(v9 * v43);
        do
        {
            v12 = cos(v42);
            v53[0].v1.y = (float)(v40 - 0.5);
            v53[0].uvSet2.v = v40;
            v53[0].uvSet1.v = v40;
            v53[0].uvSet3.v = v40;
            v53[0].uvSet4.v = v40;
            v53[1].v1.y = v39;
            v53[1].uvSet3.v = v39;
            v53[1].uvSet1.v = v39;
            v53[1].uvSet2.v = v39;
            v53[1].uvSet4.v = v39;
            v53[2].v1.y = v53[0].v1.y;
            v53[2].uvSet2.v = v40;
            v53[2].uvSet1.v = v40;
            v53[2].uvSet4.v = v40;
            v53[2].uvSet3.v = v40;
            v53[3].v1.y = v39;
            v53[3].uvSet2.v = v39;
            v53[3].uvSet3.v = v39;
            v53[0].v1.x = 0.0;
            v53[0].uvSet1.u = 0.0;
            v53[0].uvSet2.u = 0.0;
            v53[0].uvSet3.u = 0.0;
            v53[0].uvSet4.u = 0.0;
            v53[1].v1.x = 0.0;
            v53[1].uvSet1.u = 0.0;
            v53[1].uvSet2.u = 0.0;
            v53[1].uvSet3.u = 0.0;
            v53[1].uvSet4.u = 0.0;
            v53[2].v1.x = 639.5;
            v53[2].uvSet1.u = 640.0;
            v53[2].uvSet2.u = 640.0;
            v53[2].uvSet3.u = 640.0;
            v53[2].uvSet4.u = 640.0;
            v53[3].v1.x = 639.5;
            v53[3].uvSet1.u = 640.0;
            v53[3].uvSet1.v = v39;
            v53[3].uvSet2.u = 640.0;
            v53[3].uvSet3.u = 640.0;
            v53[3].uvSet4.u = 640.0;
            v53[3].uvSet4.v = v39;
            v13 = &v53[0];
            v14 = 4;
            v15 = sin(v42);
            v33 = v42 + v43;
            v16 = cos(v33);
            v17 = sin(v33);
            v46 = v43 + v43;
            v34 = v46 + v42;
            v18 = cos(v34);
            v50 = sin(v34);
            v49 = (float)(v43 * 3.0);
            v19 = v49 + v42;
            v52 = cos(v19);
            v51 = sin(v19);
            do
            {
                v20 = v13->uvSet1.v - 240.0;
                v35 = (float)((v13->uvSet1.u - 320.0) * v12 + v15 * v20);
                v13->uvSet1.u = v35;
                v13->uvSet1.v = (float)(v12 * v20 - v35 * v15);
                v13->uvSet1.u = (float)(v13->uvSet1.u + 320.0);
                v13->uvSet1.v = (float)(v13->uvSet1.v + 240.0);
                v21 = v13->uvSet2.v - 240.0;
                v36 = (float)((v13->uvSet2.u - 320.0) * v16 + v17 * v21);
                v13->uvSet2.u = v36;
                v13->uvSet2.v = (float)(v16 * v21 - v36 * v17);
                v13->uvSet2.u = (float)(v13->uvSet2.u + 320.0);
                v13->uvSet2.v = (float)(v13->uvSet2.v + 240.0);
                v22 = v13->uvSet3.v - 240.0;
                v37 = (float)((v13->uvSet3.u - 320.0) * v18 + v50 * v22);
                v13->uvSet3.u = v37;
                v13->uvSet3.v = (float)(v18 * v22 - v37 * v50);
                v13->uvSet3.u = (float)(v13->uvSet3.u + 320.0);
                v13->uvSet3.v = (float)(v13->uvSet3.v + 240.0);
                v23 = v13->uvSet4.v - 240.0;
                v24 = v13->uvSet4.u - 320.0;
                v38 = (float)(v24 * v52 + v23 * v51);
                v13->uvSet4.u = v38;
                v13->uvSet4.v = (float)(v23 * v52 - v38 * v51);
                v13->uvSet4.u = (float)(v13->uvSet4.u + 320.0);
                v13->uvSet4.v = (float)(v13->uvSet4.v + 240.0);
                ++v13;
                --v14;
            } while (v14);
            v25 = &v53[0];
            v26 = 4;
            do
            {
                v25->v1.z = 0.0f;
                v27 = v48 * v25->v1.x;
                v25->hw = 1.0f;
                v25->color = v11;

                v25->v1.x = (float)(v27 * 0.0015625);
                v25->v1.y = (float)(v47 * v25->v1.y * 0.0020833334);
                v25->uvSet1.u = (float)(v25->uvSet1.u * 0.0015625);
                v25->uvSet1.v = (float)(v25->uvSet1.v * 0.0020833334);
                v25->uvSet2.u = (float)(v25->uvSet2.u * 0.0015625);
                v25->uvSet2.v = (float)(v25->uvSet2.v * 0.0020833334);
                v25->uvSet3.u = (float)(v25->uvSet3.u * 0.0015625);
                v25->uvSet3.v = (float)(v25->uvSet3.v * 0.0020833334);
                v25->uvSet4.u = (float)(v25->uvSet4.u * 0.0015625);
                v25->uvSet4.v = (float)(v25->uvSet4.v * 0.0020833334);

                ++v25;
                --v26;
            } while (v26);
            if (v10)
            {
                if (renderStateConfig.d3dAlphaBlendEnable != 1)
                {
                    renderStateConfig.d3dAlphaBlendEnable = 1;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 1);
                }
                if (renderStateConfig.d3d_BlendOp != 1)
                {
                    renderStateConfig.d3d_BlendOp = 1;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 171, 1);
                }
                if (renderStateConfig.d3dSrcBlend != 2)
                {
                    renderStateConfig.d3dSrcBlend = 2;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 2);
                }
                if (renderStateConfig.d3dDestBlend != 2)
                {
                    renderStateConfig.d3dDestBlend = 2;
                    D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 2);
                }
            }
            else if (renderStateConfig.d3dAlphaBlendEnable)
            {
                renderStateConfig.d3dAlphaBlendEnable = 0;
                D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
            }
            D3D_DrawASurface(v53, D3DFVF_TEX4 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
            ++v10;
            v42 = v44 + v42;
        } while (v10 < v45);
    }
    v28 = 0;
    if (v41 > 0)
    {
        v29 = 0;
        do
        {
            if (textureStageTypeConfig[v29].d3d_colorOp != 1)
            {
                v30 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v29].d3d_colorOp = 1;
                v30->lpVtbl->SetTextureStageState(v30, v28, 1, 1);
            }
            if (textureStageTypeConfig[v29].d3d_alphaOp != 1)
            {
                v31 = D3DRenderParams.d3dDev;
                textureStageTypeConfig[v29].d3d_alphaOp = 1;
                v31->lpVtbl->SetTextureStageState(v31, v28, 4, 1);
            }
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v28++, 0);
            ++v29;
        } while (v28 < v41);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
}

void D3D_MotionSmoothHD(IDirect3DTexture9* a1, float a2, float a3)
{
    int v3;
    double v4;
    tVertex_type_444* v5;
    int v6;
    int v7;
    IDirect3DDevice9* v8;
    IDirect3DDevice9* v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    tVertex_type_444 a1a[4];

    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a1);
    if (textureStageTypeConfig[2].d3d_colorOp != 18)
    {
        textureStageTypeConfig[2].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 18);
    }
    if (textureStageTypeConfig[2].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[2].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 2, 1);
    }
    if (textureStageTypeConfig[2].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[2].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 3, 2);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 2);
    }
    if (textureStageTypeConfig[2].d3d_alphaArg1)
    {
        textureStageTypeConfig[2].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a1);
    if (textureStageTypeConfig[3].d3d_colorOp != 18)
    {
        textureStageTypeConfig[3].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 1, 18);
    }
    if (textureStageTypeConfig[3].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[3].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 2, 1);
    }
    if (textureStageTypeConfig[3].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[3].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 3, 2);
    }
    if (textureStageTypeConfig[3].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[3].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 4, 2);
    }
    v3 = 0;
    if (textureStageTypeConfig[3].d3d_alphaArg1)
    {
        textureStageTypeConfig[3].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)a1);
    if (textureStageTypeConfig[4].d3d_colorOp != 1)
    {
        textureStageTypeConfig[4].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 4, 1, 1);
    }
    if (textureStageTypeConfig[4].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[4].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 4, 4, 1);
    }
    v29 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v14 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1a[0].v1.x = 0.0;
    a1a[1].v1.x = 0.0;
    v10 = (float)(a2 * 0.0015625);
    v11 = (float)(a3 * 0.0020833334);
    a1a[0].v1.y = (float)(bandsVal2 * v14 - 0.5);
    a1a[0].uvSet1.u = (float)(v10 * -3.0);
    v16 = (float)(v11 * 3.0);
    v20 = bandsVal2 - v16;
    a1a[0].uvSet1.v = v20;
    v27 = v10 + v10;
    a1a[0].uvSet2.u = v27;
    a1a[1].uvSet2.u = v27;
    v4 = v11 + v11;
    v12 = (float)v4;
    v22 = (float)(v4 + bandsVal2);
    a1a[0].uvSet2.v = v22;
    a1a[0].uvSet3.u = -v10;
    v25 = bandsVal2 - v11;
    a1a[0].uvSet3.v = v25;
    v28 = (float)(v10 * 0.0);
    a1a[0].uvSet4.u = v28;
    a1a[1].uvSet4.u = v28;
    v18 = (float)(v11 * 0.0);
    v24 = bandsVal2 + v18;
    a1a[0].uvSet4.v = v24;
    v15 = bandsVal1 * v14;
    a1a[1].v1.y = v15;
    a1a[1].uvSet1.u = a1a[0].uvSet1.u;
    v17 = bandsVal1 - v16;
    a1a[1].uvSet1.v = v17;
    v13 = v12 + bandsVal1;
    a1a[1].uvSet2.v = v13;
    a1a[1].uvSet3.u = a1a[0].uvSet3.u;
    a1a[1].uvSet3.v = bandsVal1 - v11;
    v19 = bandsVal1 + v18;
    a1a[1].uvSet4.v = v19;
    a1a[2].v1.x = (float)(v29 - 0.5);
    a1a[2].v1.y = a1a[0].v1.y;
    a1a[2].uvSet1.v = v20;
    a1a[2].uvSet2.v = v22;
    a1a[2].uvSet3.v = v25;
    a1a[2].uvSet1.u = (float)(1.0 - v10 * 3.0);
    a1a[2].uvSet4.v = v24;
    a1a[3].v1.y = v15;
    a1a[3].uvSet1.v = v17;
    a1a[3].uvSet2.v = v13;
    v21 = (float)(v27 + 1.0);
    a1a[2].uvSet2.u = v21;
    a1a[3].uvSet2.u = v21;
    a1a[3].uvSet4.v = v19;
    v23 = (float)(1.0 - v10);
    a1a[2].uvSet3.u = v23;
    a1a[3].uvSet3.u = v23;
    v5 = &a1a[0];
    v26 = (float)(v28 + 1.0);
    a1a[2].uvSet4.u = v26;
    a1a[3].uvSet4.u = v26;
    a1a[3].v1.x = a1a[2].v1.x;
    v6 = 4;
    a1a[3].uvSet1.u = a1a[2].uvSet1.u;
    a1a[3].uvSet3.v = a1a[1].uvSet3.v;
    do
    {
        v5->v1.z = 0.0f;
        v5->hw = 1.0f;
        v5->color = 0x3F3F3F3F;
        ++v5;
        --v6;
    } while (v6);
    D3D_DrawASurface(a1a, D3DFVF_TEX4 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v7 = 0;
    do
    {
        if (textureStageTypeConfig[v3].d3d_colorOp != 1)
        {
            v8 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v3].d3d_colorOp = 1;
            v8->lpVtbl->SetTextureStageState(v8, v7, 1, 1);
        }
        if (textureStageTypeConfig[v3].d3d_alphaOp != 1)
        {
            v9 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v3].d3d_alphaOp = 1;
            v9->lpVtbl->SetTextureStageState(v9, v7, 4, 1);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v7, NULL);
        ++v3;
        ++v7;
    } while (v3 < 4);
}

void D3D_MotionSmoothSD(IDirect3DTexture9* a1, IDirect3DTexture9* a2, IDirect3DTexture9* a3, float a4, float a5)
{
    int v5;
    tVertex_type_244* v6;
    double v7;
    int v8;
    int i;
    IDirect3DDevice9* v10;
    IDirect3DDevice9* v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    tVertex_type_244 a1a[4];

    v26 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v24 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1a[0].v1.x = 0.0;
    a1a[1].v1.x = 0.0;
    v15 = (float)(a4 * 0.0015625);
    v16 = (float)(a5 * 0.0020833334);
    a1a[0].v1.y = (float)(bandsVal2 * v24 - 0.5);
    a1a[0].uvSet1.u = (float)(v15 * -3.0);
    v28 = (float)(v16 * 3.0);
    v19 = bandsVal2 - v28;
    a1a[0].uvSet1.v = v19;
    a1a[2].uvSet1.v = v19;
    v22 = v15 + v15;
    a1a[0].uvSet2.u = v22;
    a1a[1].uvSet2.u = v22;
    v12 = v16 + v16;
    v17 = bandsVal2 + v12;
    a1a[0].uvSet2.v = v17;
    a1a[2].uvSet2.v = v17;
    v25 = bandsVal1 * v24;
    a1a[1].v1.y = v25;
    a1a[3].v1.y = v25;
    a1a[1].uvSet1.u = a1a[0].uvSet1.u;
    a1a[1].uvSet1.v = bandsVal1 - v28;
    v13 = bandsVal1 + v12;
    a1a[1].uvSet2.v = v13;
    a1a[3].uvSet2.v = v13;
    v5 = 4;
    a1a[2].v1.x = (float)(v26 - 0.5);
    a1a[2].v1.y = a1a[0].v1.y;
    a1a[2].uvSet1.u = (float)(1.0 - v15 * 3.0);
    v20 = (float)(v22 + 1.0);
    a1a[2].uvSet2.u = v20;
    a1a[3].uvSet2.u = v20;
    a1a[3].v1.x = a1a[2].v1.x;
    v6 = &a1a[0];
    a1a[3].uvSet1.u = a1a[2].uvSet1.u;
    a1a[3].uvSet1.v = a1a[1].uvSet1.v;
    do
    {
        v6->v1.z = 0.0f;
        v6->hw = 1.0f;
        v6->color = 0x3F3F3F3F;
        ++v6;
        --v5;
    } while (v5);
    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[2].d3d_colorOp != 1)
    {
        textureStageTypeConfig[2].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 1);
    }
    D3D_DrawASurface(a1a, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    a1a[0].uvSet1.u = -v15;
    a1a[0].uvSet1.v = bandsVal2 - v16;
    v18 = (float)(v15 * 0.0);
    a1a[0].uvSet2.u = v18;
    v7 = v16 * 0.0;
    v21 = (float)(bandsVal2 + v7);
    a1a[0].uvSet2.v = v21;
    a1a[2].uvSet2.v = v21;
    a1a[1].uvSet1.u = a1a[0].uvSet1.u;
    v23 = bandsVal1 - v16;
    a1a[1].uvSet1.v = v23;
    a1a[3].uvSet1.v = v23;
    a1a[1].uvSet2.u = v18;
    v14 = (float)(v7 + bandsVal1);
    a1a[3].uvSet2.v = v14;
    a1a[1].uvSet2.v = v14;
    v27 = (float)(1.0 - v15);
    a1a[2].uvSet1.u = v27;
    a1a[3].uvSet1.u = v27;
    a1a[2].uvSet1.v = a1a[0].uvSet1.v;
    a1a[2].uvSet2.u = (float)(v18 + 1.0);
    a1a[3].uvSet2.u = a1a[2].uvSet2.u;
    if (textureStageTypeConfig[0].d3d_colorOp != 4)
    {
        textureStageTypeConfig[0].d3d_colorOp = 4;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 4);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg2)
    {
        textureStageTypeConfig[0].d3d_colorarg2 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 3, 0);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a3);
    if (textureStageTypeConfig[2].d3d_colorOp != 1)
    {
        textureStageTypeConfig[2].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 1);
    }
    if (renderStateConfig.d3dAlphaBlendEnable != 1)
    {
        renderStateConfig.d3dAlphaBlendEnable = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 1);
    }
    if (renderStateConfig.d3d_BlendOp != 1)
    {
        renderStateConfig.d3d_BlendOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 171, 1);
    }
    if (renderStateConfig.d3dSrcBlend != 2)
    {
        renderStateConfig.d3dSrcBlend = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 2);
    }
    if (renderStateConfig.d3dDestBlend != 2)
    {
        renderStateConfig.d3dDestBlend = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 2);
    }
    D3D_DrawASurface(a1a, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v8 = 0;
    for (i = 0; i < 2; ++i)
    {
        if (textureStageTypeConfig[i].d3d_colorOp != 1)
        {
            v10 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_colorOp = 1;
            v10->lpVtbl->SetTextureStageState(v10, v8, 1, 1);
        }
        if (textureStageTypeConfig[i].d3d_alphaOp != 1)
        {
            v11 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[i].d3d_alphaOp = 1;
            v11->lpVtbl->SetTextureStageState(v11, v8, 4, 1);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v8++, NULL);
    }
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
}

void D3D_MotionSmooth(IDirect3DTexture9* a1, IDirect3DTexture9* a2, IDirect3DTexture9* a3, IDirect3DTexture9* a4, float a5, float a6)
{
    D3D_SetRenderTargetFromTexture(a4);
    if (renderStateConfig.d3dColorWriteEnable != 7)
    {
        renderStateConfig.d3dColorWriteEnable = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 7);
    }
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_MotionSmoothHD(a3, a5, a6);
    else
        D3D_MotionSmoothSD(a1, a2, a3, a5, a6);
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
}

void D3D_RemenanceSD(IDirect3DTexture9* a5, float a6)
{
    tVertex_type_244* v2;
    int v3;
    int v4;
    int v5;
    int v6;
    IDirect3DDevice9* v7; 
    IDirect3DDevice9* v8; 
    IDirect3DDevice9* v9; 
    IDirect3DDevice9* v10;
    IDirect3DDevice9* v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    tVertex_type_244 v17[4];

    v14 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v12 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v17[0].uvSet1.v = bandsVal2;
    v17[1].uvSet1.v = bandsVal1;
    v17[2].uvSet1.v = bandsVal2;
    v17[0].v1.y = bandsVal2 * v12 - 0.5f;
    v17[0].v1.x = 0.0;
    v17[0].uvSet1.u = 0.0;
    v17[0].uvSet2.u = 0.0078125;
    v17[1].v1.x = 0.0;
    v17[1].uvSet1.u = 0.0;
    v17[0].uvSet2.v = bandsVal2 - 0.010416667f;
    v17[1].uvSet2.u = 0.0078125;
    v17[2].uvSet1.u = 1.0;
    v17[2].uvSet2.u = 1.0078125;
    v17[3].uvSet1.u = 1.0;
    v17[3].uvSet1.v = bandsVal1;
    v13 = bandsVal1 * v12;
    v17[3].uvSet2.u = 1.0078125;
    v17[1].v1.y = v13;
    v17[3].v1.y = v13;
    v16 = bandsVal1 - 0.010416667f;
    v17[1].uvSet2.v = v16;
    v15 = v14 - 0.5f;
    v17[2].v1.x = v15;
    v17[3].v1.x = v15;
    v17[2].v1.y = v17[0].v1.y;
    v17[3].uvSet2.v = v16;
    v17[2].uvSet2.v = v17[0].uvSet2.v;
    v2 = &v17[0];
    v3 = 4;
    do
    {
        v2->v1.z = 0.0f;
        v2->hw = 1.0f;
        v2->color = ((unsigned int)(a6 * 255.0) << 24) | 0xFFFFFF;
        ++v2;
        --v3;
    } while (v3);
    v4 = 0;
    if (samplerConfig[0].d3d_minfilter != 2)
    {
        samplerConfig[0].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 6, 2);
    }
    if (samplerConfig[0].d3d_magfilter != 2)
    {
        samplerConfig[0].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 11, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a5);
    if (samplerConfig[1].d3d_minfilter != 2)
    {
        samplerConfig[1].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, 6, 2);
    }
    if (samplerConfig[1].d3d_magfilter != 2)
    {
        samplerConfig[1].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, 5, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    if (textureStageTypeConfig[1].d3dTexCoordIndex != 1)
    {
        textureStageTypeConfig[1].d3dTexCoordIndex = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 11, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    if (textureStageTypeConfig[2].d3d_colorOp != 1)
    {
        textureStageTypeConfig[2].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 1);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 1);
    }
    D3D_DrawASurface(v17, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    if (samplerConfig[0].d3d_minfilter != 2)
    {
        samplerConfig[0].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 6, 2);
    }
    if (samplerConfig[0].d3d_magfilter != 2)
    {
        samplerConfig[0].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex != 1)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 11, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    if (textureStageTypeConfig[1].d3d_colorOp != 1)
    {
        textureStageTypeConfig[1].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 1);
    }
    if (renderStateConfig.d3dAlphaBlendEnable != 1)
    {
        renderStateConfig.d3dAlphaBlendEnable = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 1);
    }
    if (renderStateConfig.d3d_BlendOp != 1)
    {
        renderStateConfig.d3d_BlendOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 171, 1);
    }
    if (renderStateConfig.d3dSrcBlend != 5)
    {
        renderStateConfig.d3dSrcBlend = 5;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 19, 5);
    }
    if (renderStateConfig.d3dDestBlend != 2)
    {
        renderStateConfig.d3dDestBlend = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 20, 2);
    }
    D3D_DrawASurface(v17, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v5 = 0;
    v6 = 0;
    do
    {
        if (samplerConfig[v4].d3d_minfilter != 1)
        {
            v7 = D3DRenderParams.d3dDev;
            samplerConfig[v4].d3d_minfilter = 1;
            v7->lpVtbl->SetSamplerState(v7, v5, 6, 1);
        }
        if (samplerConfig[v4].d3d_magfilter != 1)
        {
            v8 = D3DRenderParams.d3dDev;
            samplerConfig[v4].d3d_magfilter = 1;
            v8->lpVtbl->SetSamplerState(v8, v5, 5, 1);
        }
        if (textureStageTypeConfig[v6].d3d_colorOp != 1)
        {
            v9 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v6].d3d_colorOp = 1;
            v9->lpVtbl->SetTextureStageState(v9, v5, 1, 1);
        }
        if (textureStageTypeConfig[v6].d3d_alphaOp != 1)
        {
            v10 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v6].d3d_alphaOp = 1;
            v10->lpVtbl->SetTextureStageState(v10, v5, 4, 1);
        }
        if (textureStageTypeConfig[v6].d3dTexCoordIndex != v5)
        {
            v11 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v6].d3dTexCoordIndex = v5;
            v11->lpVtbl->SetTextureStageState(v11, v5, 11, v5);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v5, NULL);
        ++v4;
        ++v5;
        ++v6;
    } while (v4 < 2);
    if (renderStateConfig.d3dAlphaBlendEnable)
    {
        renderStateConfig.d3dAlphaBlendEnable = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 27, 0);
    }
}

void D3D_RemenanceHD(IDirect3DTexture9* a1, float a2)
{
    int v2;
    tVertex_type_244* v3;
    int v4;
    int v5;
    int i;
    IDirect3DDevice9* v7;
    IDirect3DDevice9* v8;
    IDirect3DDevice9* v9;
    IDirect3DDevice9* v10;
    IDirect3DDevice9* v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    tVertex_type_244 a1a[4];

    v2 = 0;
    if (samplerConfig[0].d3d_minfilter != 2)
    {
        samplerConfig[0].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 6, 2);
    }
    if (samplerConfig[0].d3d_magfilter != 2)
    {
        samplerConfig[0].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 0);
    }
    if (textureStageTypeConfig[0].d3dTexCoordIndex)
    {
        textureStageTypeConfig[0].d3dTexCoordIndex = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 11, 0);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    if (samplerConfig[1].d3d_minfilter != 2)
    {
        samplerConfig[1].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, 6, 2);
    }
    if (samplerConfig[1].d3d_magfilter != 2)
    {
        samplerConfig[1].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 1, 5, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != 18)
    {
        textureStageTypeConfig[1].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 18);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 2);
    }
    if (textureStageTypeConfig[1].d3d_alphaArg1)
    {
        textureStageTypeConfig[1].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 5, 0);
    }
    if (textureStageTypeConfig[1].d3dTexCoordIndex != 1)
    {
        textureStageTypeConfig[1].d3dTexCoordIndex = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 11, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    if (samplerConfig[2].d3d_minfilter != 2)
    {
        samplerConfig[2].d3d_minfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 2, 6, 2);
    }
    if (samplerConfig[2].d3d_magfilter != 2)
    {
        samplerConfig[2].d3d_magfilter = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev, 2, 5, 2);
    }
    if (textureStageTypeConfig[2].d3d_colorOp != 18)
    {
        textureStageTypeConfig[2].d3d_colorOp = 18;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 1, 18);
    }
    if (textureStageTypeConfig[2].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[2].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 2, 1);
    }
    if (textureStageTypeConfig[2].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[2].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 3, 2);
    }
    if (textureStageTypeConfig[2].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[2].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 4, 2);
    }
    if (textureStageTypeConfig[2].d3d_alphaArg1)
    {
        textureStageTypeConfig[2].d3d_alphaArg1 = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 5, 0);
    }
    if (textureStageTypeConfig[2].d3dTexCoordIndex != 1)
    {
        textureStageTypeConfig[2].d3dTexCoordIndex = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 2, 11, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    if (textureStageTypeConfig[3].d3d_colorOp != 1)
    {
        textureStageTypeConfig[3].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 1, 1);
    }
    if (textureStageTypeConfig[3].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[3].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 3, 4, 1);
    }
    v14 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v12 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    a1a[0].uvSet1.v = bandsVal2;
    a1a[1].uvSet1.v = bandsVal1;
    a1a[2].uvSet1.v = bandsVal2;
    a1a[0].v1.y = bandsVal2 * v12 - 0.5f;
    a1a[0].v1.x = 0.0;
    a1a[0].uvSet1.u = 0.0;
    a1a[0].uvSet2.u = 0.0078125f;
    a1a[1].v1.x = 0.0;
    a1a[1].uvSet1.u = 0.0;
    a1a[0].uvSet2.v = bandsVal2 - 0.010416667f;
    a1a[1].uvSet2.u = 0.0078125f;
    a1a[2].uvSet1.u = 1.0;
    a1a[2].uvSet2.u = 1.0078125f;
    a1a[3].uvSet1.u = 1.0;
    a1a[3].uvSet1.v = bandsVal1;
    v13 = bandsVal1 * v12;
    a1a[3].uvSet2.u = 1.0078125f;
    a1a[1].v1.y = v13;
    a1a[3].v1.y = v13;
    v16 = bandsVal1 - 0.010416667f;
    a1a[1].uvSet2.v = v16;
    v15 = v14 - 0.5f;
    a1a[2].v1.x = v15;
    a1a[3].v1.x = v15;
    a1a[2].v1.y = a1a[0].v1.y;
    a1a[3].uvSet2.v = v16;
    a1a[2].uvSet2.v = a1a[0].uvSet2.v;
    v3 = &a1a[0];
    v4 = 4;
    do
    {
        v3->v1.z = 0.0f;
        v3->hw = 1.0f;
        v3->color = ((unsigned int)(a2 * 255.0) << 24) | 0xFFFFFF;
        ++v3;
        --v4;
    } while (v4);
    D3D_DrawASurface(a1a, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v5 = 0;
    for (i = 0; i < 3; ++i)
    {
        if (samplerConfig[i].d3d_minfilter != 1)
        {
            v7 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_minfilter = 1;
            v7->lpVtbl->SetSamplerState(v7, v5, 6, 1);
        }
        if (samplerConfig[i].d3d_magfilter != 1)
        {
            v8 = D3DRenderParams.d3dDev;
            samplerConfig[i].d3d_magfilter = 1;
            v8->lpVtbl->SetSamplerState(v8, v5, 5, 1);
        }
        if (textureStageTypeConfig[v2].d3d_colorOp != 1)
        {
            v9 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v2].d3d_colorOp = 1;
            v9->lpVtbl->SetTextureStageState(v9, v5, 1, 1);
        }
        if (textureStageTypeConfig[v2].d3d_alphaOp != 1)
        {
            v10 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v2].d3d_alphaOp = 1;
            v10->lpVtbl->SetTextureStageState(v10, v5, 4, 1);
        }
        if (textureStageTypeConfig[v2].d3dTexCoordIndex != v5)
        {
            v11 = D3DRenderParams.d3dDev;
            textureStageTypeConfig[v2].d3dTexCoordIndex = v5;
            v11->lpVtbl->SetTextureStageState(v11, v5, 11, v5);
        }
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v5++, NULL);
        ++v2;
    }
}

void D3D_Remenance(IDirect3DTexture9* a3, IDirect3DTexture9* a4, float a5)
{
    D3D_DepthBlur(a3, a4, 0.0015625f, 0.0020833334f);
    D3D_DepthBlur(a4, D3DRenderParams.blurRenderTarget, 0.0046875002f, 0.0062500001f);
    D3D_DepthBlur(D3DRenderParams.blurRenderTarget, a4, 0.0078125f, 0.010416667f);
    D3D_DepthBlur(a4, D3DRenderParams.blurRenderTarget, 0.0109375f, 0.014583333f);
    D3D_SetRenderTargetFromTexture(a4);
    if (renderStateConfig.d3dColorWriteEnable != 7)
    {
        renderStateConfig.d3dColorWriteEnable = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 7);
    }
    if (gMaxSimultaneousTextures == 2)
        D3D_RemenanceSD(a3, a5);
    else
        D3D_RemenanceHD(a3, a5);
        
    if (renderStateConfig.d3dColorWriteEnable != 15)
    {
        renderStateConfig.d3dColorWriteEnable = 15;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 15);
    }
}

void D3D_Unknown_AE_FuncDepthBlur(IDirect3DTexture9* a1)
{
    int v1;
    tVertex_type_244* v2;
    int v3;
    float v4;
    float v5;
    float v6;
    float v7;
    tVertex_type_244 v8[4];

    D3D_SetRenderTargetFromTexture(a1);
    if (renderStateConfig.d3dColorWriteEnable != 7)
    {
        renderStateConfig.d3dColorWriteEnable = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 7);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 2)
    {
        textureStageTypeConfig[0].d3d_colorOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 2);
    }
    if (textureStageTypeConfig[0].d3d_colorarg1 != 2)
    {
        textureStageTypeConfig[0].d3d_colorarg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 2, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 2)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 2);
    }
    if (textureStageTypeConfig[0].d3d_alphaArg1 != 2)
    {
        textureStageTypeConfig[0].d3d_alphaArg1 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 5, 2);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != 16)
    {
        textureStageTypeConfig[1].d3d_colorOp = 16;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 16);
    }
    if (textureStageTypeConfig[1].d3d_colorarg1 != 1)
    {
        textureStageTypeConfig[1].d3d_colorarg1 = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 2, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorarg2 != 2)
    {
        textureStageTypeConfig[1].d3d_colorarg2 = 2;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 3, 2);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    v7 = (float)D3DRenderParams.presentParams.BackBufferWidth;
    v6 = (float)D3DRenderParams.presentParams.BackBufferHeight;
    v5 = bandsVal2 * 480.0f;
    v4 = bandsVal1 * 480.0f;
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    v1 = D3D_CalculateStride(D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v8[0].uvSet1.v = v5;
    v8[0].uvSet2.v = v5;
    v8[0].v1.y = v5 - 0.5f;
    v8[2].v1.y = v8[0].v1.y;
    v8[1].v1.y = v4;
    v8[1].uvSet1.v = v4;
    v8[1].uvSet2.v = v4;
    v8[2].uvSet1.v = v5;
    v8[2].uvSet2.v = v5;
    v8[0].color = 0xffffffff;
    v8[1].color = 0xffffffff;
    v8[2].color = 0xffffffff;
    v8[3].color = 0xffffffff;
    v8[3].v1.y = v4;
    v8[3].uvSet1.v = v4;
    v8[3].uvSet2.v = v4;
    v8[0].v1.x = 0.0;
    v8[0].v1.z = 0.0;
    v8[0].hw = 1.0;
    v8[0].uvSet1.u = 0.0;
    v8[0].uvSet2.u = 0.0;
    v8[1].v1.x = 0.0;
    v8[1].v1.z = 0.0;
    v8[1].hw = 1.0;
    v8[1].uvSet1.u = 0.0;
    v8[1].uvSet2.u = 0.0;
    v8[2].v1.x = 639.5;
    v8[2].v1.z = 0.0;
    v8[2].hw = 1.0;
    v8[2].uvSet1.u = 640.0;
    v8[2].uvSet2.u = 640.0;
    v8[3].v1.x = 639.5;
    v8[3].v1.z = 0.0;
    v8[3].hw = 1.0;
    v8[3].uvSet1.u = 640.0;
    v8[3].uvSet2.u = 640.0;
    v2 = &v8[0];
    v3 = 4;
    do
    {
        v2->v1.x = (float)(v7 * v2->v1.x * 0.0015625);
        v2->v1.y = (float)(v6 * v2->v1.y * 0.0020833334);
        v2->uvSet1.u = (float)(v2->uvSet1.u * 0.0015625);
        v2->uvSet1.v = (float)(v2->uvSet1.v * 0.0020833334);
        v2->uvSet2.u = (float)(v2->uvSet2.u * 0.0015625);
        v2->uvSet2.v = (float)(v2->uvSet2.v * 0.0020833334);
        --v3;
        ++v2;
    } while (v3);

    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, 5, 2, (void*)v8, v1);

    if (renderStateConfig.d3dColorWriteEnable != 7)
    {
        renderStateConfig.d3dColorWriteEnable = 7;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 168, 7);
    }
    if (renderStateConfig.d3dAlphaTest)
    {
        renderStateConfig.d3dAlphaTest = 0;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev, 15, 0);
    }
    if (textureStageTypeConfig[0].d3d_colorOp != 1)
    {
        textureStageTypeConfig[0].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 1, 1);
    }
    if (textureStageTypeConfig[1].d3d_colorOp != 1)
    {
        textureStageTypeConfig[1].d3d_colorOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 1, 1);
    }
    if (textureStageTypeConfig[0].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[0].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 0, 4, 1);
    }
    if (textureStageTypeConfig[1].d3d_alphaOp != 1)
    {
        textureStageTypeConfig[1].d3d_alphaOp = 1;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev, 1, 4, 1);
    }
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, NULL);
}

void D3D_PC_AE(tWorld* param1)
{
    IDirect3DTexture9* v1;
    IDirect3DTexture9* v2;
    IDirect3DTexture9* v3;
    IDirect3DTexture9* v4;
    IDirect3DTexture9* v5;
    double v6;
    BOOL v7;
    IDirect3DTexture9* v8;
    double v9;
    IDirect3DTexture9* v10;
    IDirect3DTexture9* v11;
    IDirect3DTexture9* v12;
    float a5;
    float a6;
    float v15;
    RECT a2;

    D3D_SetD3DStatesDefaultForAE();
    if (tweak_ae_nobands)
    {
        bandsVal1 = 0.0f;
        bandsVal2 = 1.0f; // 480.0f - these broken values were used in the original game. Broke AE
        bandsVal3 = 0.0f;
        bandsVal4 = 1.0f; // 480.0f - || -
    }
    else
    {
        bandsVal1 = 0.10416666f;
        bandsVal2 = 0.89583331f;
        bandsVal3 = 0.083333336f;
        bandsVal4 = 0.91666669f;
    }
    v1 = D3DRenderParams.blurTex;
    v2 = D3DRenderParams.blurRenderTarget2;
    if (tweak_ae_no)
    {
        PC_AE_MASTER = 0;
        colorBalanceParam1 = 0.5f;
        unkAEFunc2Param = 0.5f;
    }
    if ((PC_AE_MASTER & 8) != 0 || (PC_AE_MASTER & 0x1000) != 0)// depthblur
    {
        D3D_DepthBlur(D3DRenderParams.blurTex, D3DRenderParams.blurRenderTarget, 0.0015625f, 0.0020833334f);
        D3D_DepthBlur(D3DRenderParams.blurRenderTarget, v2, 0.003125f, 0.0041666669f);
        D3D_DepthBlur(v2, D3DRenderParams.blurRenderTarget, 0.0046875002f, 0.0062500001f);
        D3D_Unknown_AE_FuncDepthBlur(v1);
    }
    if ((PC_AE_MASTER & 4) != 0)
    {
        v3 = v2;
        D3D_Unkown_AE_Func_Flag_4(v1, v2, unknownAEFlag4GlobalVar, texFactorManual);
        v2 = v1;
        v1 = v3;
    }
    if ((PC_AE_MASTER & 0x800) != 0 && remenanceFactor > 0.0099999998)
    {
        v4 = v2;
        D3D_Remenance(v1, v2, remenanceFactor);
        v2 = v1;
        v1 = v4;
    }
    if ((PC_AE_MASTER & 0x40000) != 0)
    {
        v5 = v2;
        v6 = gAEOpacity * 60.0;
        if (v6 >= 255.0)
            v15 = 1.0;
        else
            v15 = (float)(255.0 / v6);
        if (D3DRenderParams.opacity == 255)
        {
            a2.left = 0;
            a2.top = 0;
            a2.right = (unsigned int)(double)D3DRenderParams.presentParams.BackBufferWidth;
            a2.bottom = (unsigned int)(double)D3DRenderParams.presentParams.BackBufferHeight;
            D3D_SetRenderTargetFromTexture(D3DRenderParams.unktex1_dword_B31988);
            D3D_StretchRect(v1, &a2);
        }
        D3D_Fade(v1, v2, D3DRenderParams.opacity);
        v2 = v1;
        v1 = v5;
        v7 = D3DRenderParams.opacity == 0;
        if (D3DRenderParams.opacity > 0)
        {
            D3DRenderParams.opacity = (int)(D3DRenderParams.opacity - (unsigned int)v15) < 0 ? 0 : D3DRenderParams.opacity - (unsigned int)v15;
            v7 = D3DRenderParams.opacity == 0;
        }
        if (v7)
            PC_AE_MASTER &= ~0x40000u;
    }
    if ((PC_AE_MASTER & 0x10) != 0 && motionBlurFactor != 0.0)
    {
        v8 = v2;
        D3D_MotionBlur(v1, v2, motionBlurFactor);
        v2 = v1;
        v1 = v8;
    }
    if ((PC_AE_MASTER & 0x80u) != 0)
    {
        AE_MS_CurrVec.x = 1.0;
        AE_MS_CurrVec.y = 1.0;
        AE_MS_CurrVec.z = 1.0;
        MATH_TransformVector(&AE_MS_CurrVec, &gpst_DX_stDD->ct.cam.cameraMtx, &AE_MS_CurrVec);
        v9 = AE_MS_CurrVec.x - AE_MS_PrevVec.x;
        if (sqrt(
            v9 * v9
            + (AE_MS_CurrVec.z - AE_MS_PrevVec.z) * (AE_MS_CurrVec.z - AE_MS_PrevVec.z)
            + (AE_MS_CurrVec.y - AE_MS_PrevVec.y) * (AE_MS_CurrVec.y - AE_MS_PrevVec.y)) > 0.0049999999)
        {
            v10 = v2;
            a6 = (float)((AE_MS_CurrVec.y - AE_MS_PrevVec.y) * 50.0);
            a5 = (float)(v9 * 50.0);
            D3D_MotionSmooth(v2, v1, v1, v2, a5, a6);
            v2 = v1;
            v1 = v10;
        }
        AE_MS_PrevVec = AE_MS_CurrVec;
    }
    if ((PC_AE_MASTER & 0x400) != 0 && zoomSmoothFactor != 0.0)
    {
        v11 = v2;
        D3D_ZoomSmooth(v1, v2, zoomSmoothFactor);
        v2 = v1;
        v1 = v11;
    }
    if ((PC_AE_MASTER & 0x20) != 0 && blurFactor != 0.0)
    {
        v12 = v2;
        D3D_Blur(v1, v2, blurFactor);
        v2 = v1;
        v1 = v12;
    }
    if ((PC_AE_MASTER & 0x8000) != 0 && zoomSmoothFactor2 != 0.0)
    {
        D3D_ZoomSmooth2(v1, v2, zoomSmoothFactor2);
        v1 = v2;
    }
    if (unkAEFunc2Param != 0.5)
        D3D_Unkown_AE_Func_2(v1, unkAEFunc2Param);
    if (colorBalanceParam1 != 0.5)
        D3D_ColorBalance(v1, colorBalanceParam1);
    if ((PC_AE_MASTER & 1) != 0)
        D3D_Unkown_AE_Func_Flag_1(v1);
    D3D_AE_CopyToScreen(v1);
    D3D_SetD3DStatesDefaultForAE();
    D3DRenderParams.d3d_defaultZEnable = 1;
}

void PC_AE_MASTER_CTRL(int a1, int a2, int a3, float a4)
{
    int v4;
    double v5;
    unsigned int v6;
    unsigned int v7;
    unsigned int v8;
    unsigned int v9;
    unsigned int v10;
    unsigned int v11;
    unsigned int v12;
    unsigned int v13;
    unsigned int v14;
    int v15;
    unsigned int v16;
    unsigned int v17;

    switch (a1)
    {
    case 0:
        v4 = 0;
        if (a2)
        {
            if ((PC_AE_MASTER & 8) != 0)
                goto LABEL_7;
        }
        else if ((PC_AE_MASTER & 8) == 0)
        {
            goto LABEL_7;
        }
        v4 = 1;
    LABEL_7:
        if (a3)
        {
            if (a3 == 1)
            {
                v5 = depthBlurType1;
            }
            else
            {
                if (a3 != 2)
                    goto LABEL_14;
                v5 = depthBlurType2;
            }
        }
        else
        {
            v5 = depthBlurType0;
        }
        if (v5 != a4)
        {
        LABEL_15:
            v6 = PC_AE_MASTER & ~8u;
            PC_AE_MASTER &= ~8u;
            if (a2)
                PC_AE_MASTER = v6 | 8;
            if (a3)
            {
                if (a3 == 1)
                {
                    depthBlurType1 = a4;
                }
                else if (a3 == 2)
                {
                    depthBlurType2 = a4;
                }
            }
            else
            {
                depthBlurType0 = a4;
            }
            goto LABEL_21;
        }
    LABEL_14:
        if (v4)
            goto LABEL_15;
        return;
    case 1:
        v7 = PC_AE_MASTER & ~0x10u;
        PC_AE_MASTER &= ~0x10u;
        if (a2)
            PC_AE_MASTER = v7 | 0x10;
        motionBlurFactor = a4;
        return;
    case 2:
        v8 = PC_AE_MASTER & ~0x20u;
        PC_AE_MASTER &= ~0x20u;
        if (a2)
            PC_AE_MASTER = v8 | 0x20;
        blurFactor = a4;
        return;
    case 4:                                     // motionsmooth
        v9 = PC_AE_MASTER & ~0x80u;
        PC_AE_MASTER &= ~0x80u;
        if (a2)
            PC_AE_MASTER = v9 | 0x80;
        return;
    case 5:
        v10 = PC_AE_MASTER & ~0x400u;
        PC_AE_MASTER &= ~0x400u;
        if (a2)
            PC_AE_MASTER = v10 | 0x400;
        zoomSmoothFactor = a4;
        return;
    case 6:
        v11 = PC_AE_MASTER & ~0x8000u;
        PC_AE_MASTER &= ~0x8000u;
        if (a2)
            PC_AE_MASTER = v11 | 0x8000;
        zoomSmoothFactor2 = a4;
        return;
    case 7:
        v12 = PC_AE_MASTER & ~0x800u;
        PC_AE_MASTER &= ~0x800u;
        if (a2)
            PC_AE_MASTER = v12 | 0x800;
        remenanceFactor = a4;
        return;
    case 8:
        unkAEFunc2Param = a4;
        return;
    case 9:
        colorBalanceParam1 = a4;
        return;
    case 11:
        v13 = PC_AE_MASTER & ~1u;
        PC_AE_MASTER &= ~1u;
        if (a2)
            PC_AE_MASTER = v13 | 1;
        blackAndWhiteParam1 = a4;
        return;
    case 12:
        if (a3)
        {
            if (a3 == 1)
            {
                texFactorManual = a4;
            }
            else if (a3 == 2)
            {
                texFactorProbablyUnunsedType2 = a4;
            }
        }
        else
        {
            unknownAEFlag4GlobalVar = a4;
        }
        v14 = PC_AE_MASTER & ~4u;
        PC_AE_MASTER &= ~4u;
        if (a2)
            PC_AE_MASTER = v14 | 4;
        return;
    case 14:
        v15 = 0;
        if (a2)
        {
            if ((PC_AE_MASTER & 0x1000) != 0)
                goto LABEL_60;
        }
        else if ((PC_AE_MASTER & 0x1000) == 0)
        {
            goto LABEL_60;
        }
        v15 = 1;
    LABEL_60:
        if (depthBlurNear1 != a4 || v15)
        {
            v16 = PC_AE_MASTER & ~0x1000u;
            PC_AE_MASTER &= ~0x1000u;
            if (a2)
                PC_AE_MASTER = v16 | 0x1000;
            depthBlurNear1 = a4;
        LABEL_21:
            D3D_UpdateBlurTexture();
        }
        return;
    case 15:
        v17 = PC_AE_MASTER & ~0x40000u;
        PC_AE_MASTER &= ~0x40000u;
        if (a2)
        {
            PC_AE_MASTER = v17 | 0x40000;
            if (!D3DRenderParams.opacity)
                D3DRenderParams.opacity = 255;
        }
        gAEOpacity = a4;
        return;
    default:
        return;
    }
}

int PC_AE_MASTER_GET_ONOFF_sub_4A37D0(int a1)
{
    int result;

    switch (a1)
    {
    case 0:
        result = PC_AE_MASTER & 8;                // depthblur
        break;
    case 1:
        result = PC_AE_MASTER & 0x10;             // motionblur
        break;
    case 2:
        result = PC_AE_MASTER & 0x20;             // screenblur
        break;
    case 3:
        result = PC_AE_MASTER & 0x40;             // emboss
        break;
    case 4:
        result = PC_AE_MASTER & 0x80;             // motionsmooth
        break;
    case 5:
        result = PC_AE_MASTER & 0x400;            // zoomsmooth
        break;
    case 6:
        result = PC_AE_MASTER & 0x8000;           // zoomsmooth2
        break;
    case 7:
        result = PC_AE_MASTER & 0x800;            // remanance
        break;
    case 10:
        result = PC_AE_MASTER & 0x2000;           // full-aa-blur
        break;
    case 11:
        result = PC_AE_MASTER & 1;                // colorbalance
        break;
    case 12:
        result = PC_AE_MASTER & 4;
        break;
    case 14:
        result = PC_AE_MASTER & 0x1000;           // depthblurnear
        break;
    case 15:
        result = PC_AE_MASTER & 0x40000;          // fade
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

float PC_AE_MASTER_GET_P1(int a1, int a2)
{
    double result;

    switch (a1)
    {
    case 0:
        if (a2)                                 // 17 warp
        {
            if (a2 == 1)
            {
                result = depthBlurType1;
            }
            else
            {
                if (a2 != 2)
                    goto LABEL_8;
                result = depthBlurType2;
            }
        }
        else
        {
            result = depthBlurType0;
        }
        break;
    case 1:
    LABEL_8:
        result = motionBlurFactor;
        break;
    case 2:
        result = blurFactor;
        break;
    case 5:
        result = zoomSmoothFactor;
        break;
    case 6:
        result = zoomSmoothFactor2;
        break;
    case 7:
        result = remenanceFactor;
        break;
    case 8:
        result = unkAEFunc2Param;
        break;
    case 9:
        result = colorBalanceParam1;
        break;
    case 11:
        result = blackAndWhiteParam1;
        break;
    case 12:
        if (a2)
        {
            if (a2 == 1)
            {
                result = texFactorManual;
            }
            else
            {
                if (a2 != 2)
                    goto LABEL_23;
                result = texFactorProbablyUnunsedType2;
            }
        }
        else
        {
            result = unknownAEFlag4GlobalVar;
        }
        break;
    case 14:
        result = depthBlurNear1;
        break;
    default:
    LABEL_23:
        result = 0.0;
        break;
    }
    return (float)result;
}