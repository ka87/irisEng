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

// The effect passes work on a slightly taller slice than is shown,
// so the blur has rows to sample at the band edge.
static const float AE_BAND_MARGIN = 0.020833334f;

int CIN_Bands = 1;
int CIN_NoBands = 0;

void PC_CIN_CutsceneMode(int on)
{
    if (CIN_NoBands)
        CIN_Bands = on ? 1 : 0;
}

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


/* The chain reaches the device through three mirrors of its state --
   renderStateConfig, textureStageTypeConfig and samplerConfig -- and
   every write goes through the same shape: set it only if what the
   mirror holds is not already that. */

static void AE_SetRenderState(int* cached, D3DRENDERSTATETYPE state, int value)
{
    if (*cached != value)
    {
        *cached = value;
        D3DRenderParams.d3dDev->lpVtbl->SetRenderState(D3DRenderParams.d3dDev,
                                                       state, value);
    }
}

static void AE_SetStageState(int stage, int* cached, D3DTEXTURESTAGESTATETYPE state,
                             int value)
{
    if (*cached != value)
    {
        *cached = value;
        D3DRenderParams.d3dDev->lpVtbl->SetTextureStageState(D3DRenderParams.d3dDev,
                                                             stage, state, value);
    }
}

static void AE_SetSamplerState(int sampler, int* cached, DWORD state, int value)
{
    if (*cached != value)
    {
        *cached = value;
        D3DRenderParams.d3dDev->lpVtbl->SetSamplerState(D3DRenderParams.d3dDev,
                                                        sampler, state, value);
    }
}

void D3D_SaveAEParams()
{
    double minNear;
    double minType0;
    double minType1;
    double minFar;

    if (D3DRenderParams.tweak_zonalpha)
        depthblur_Type = 0;
    else if ((PC_AE_MASTER & 8) != 0 && (PC_AE_MASTER & 0x1000) != 0)// depthblur, depthblurnear
        depthblur_Type = 3;
    else if ((PC_AE_MASTER & 8) != 0)
        depthblur_Type = 1;
    else if ((PC_AE_MASTER & 0x1000) != 0)
        depthblur_Type = 2;
    else
        depthblur_Type = D3DRenderParams.sunEffectIsOn != 0 ? 4 : 0;

    if (gpst_DX_stDD->ct.cam.nearPlane <= 1.1754944e-38)
        camUnkf1Save = 1.1754944e-38f;
    else
        camUnkf1Save = gpst_DX_stDD->ct.cam.nearPlane;
    minNear = camUnkf1Save + 0.0099999998;
    if (depthBlurNear1 <= minNear)
        deptBlurNear1Save = (float)minNear;
    else
        deptBlurNear1Save = depthBlurNear1;
    minType0 = deptBlurNear1Save + 0.0099999998;
    if (depthBlurType0 <= minType0)
        deptBlurType0Save = (float)minType0;
    else
        deptBlurType0Save = depthBlurType0;
    minType1 = deptBlurType0Save + 0.0099999998;
    if (depthBlurType1 <= minType1)
        deptBlur1TypeSave = (float)minType1;
    else
        deptBlur1TypeSave = depthBlurType1;
    minFar = deptBlur1TypeSave + 0.0099999998;
    if (minFar >= gpst_DX_stDD->ct.cam.farPlane)
        depthBlur1TypeOrcamUnkf2Save = (float)minFar;
    else
        depthBlur1TypeOrcamUnkf2Save = gpst_DX_stDD->ct.cam.farPlane;
}

void D3D_SetGlobalBlurParams()
{
    memset(&D3DRenderParams.transformationMatrix, 0, sizeof(D3DRenderParams.transformationMatrix));
    switch (depthblur_Type)
    {
    case 0:
        D3DRenderParams.textureUVIsProjected = 0;
        break;
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
        D3DRenderParams.textureUVIsProjected = 0;
        break;
    default:
        return;
    }
}

float D3D_DepthBlurBlurFuncCalc1(int step)
{
    double depth;

    depth = 0.0;
    if (depthblur_Type == 4)
        depth = (double)step * depthBlur1TypeOrcamUnkf2Save * 0.00390625;
    return (float)depth;
}

uint32_t  D3D_DepthBlurBlurFuncCalc2(float depth)
{
    if (depthblur_Type == 4)
    {
        if (depth < 0.0)
            return 255;
        if (depth <= (double)depthBlur1TypeOrcamUnkf2Save)
            return (uint32_t)((1.0 - depth / depthBlur1TypeOrcamUnkf2Save) * 255.0);
    }
    return 0;
}

void D3D_DepthBlurBlurFunc(int* table)
{
    int clearEnd;
    int rampEnd;
    int idx;
    int step;
    int i;
    float depth;

    switch (depthblur_Type)
    {
    case 0:
        for (i = 0; i < 256; i++) table[i] = 0xFF000000;
        break;
    case 1:
        clearEnd = (unsigned int)(deptBlurType0Save / deptBlur1TypeSave * 256.0);
        idx = 0;
        if (clearEnd > 0)
        {
            memset(table, 0, 4 * clearEnd);
            idx = clearEnd;
        }
        if (clearEnd < 256)
        {
            for (step = idx - clearEnd; idx < 256; idx++, step++)
                table[idx] = (unsigned int)((double)step * (255.0 / (double)(256 - clearEnd))) << 24;
        }
        table[255] = 0xFF000000;
        break;
    case 2:
        for (i = 0; i < 255; i++) table[i] = 0xFF000000;
        table[255] = 0;
        break;
    case 3:
        clearEnd = (unsigned int)(deptBlurNear1Save / deptBlur1TypeSave * 256.0);
        rampEnd = (unsigned int)(deptBlurNear1Save / deptBlurType0Save * 256.0);
        idx = 0;
        if (clearEnd > 0)
        {
            memset(table, 0, 4 * clearEnd);
            idx = clearEnd;
        }
        if (idx < rampEnd)
        {
            for (step = idx - clearEnd; idx < rampEnd; idx++, step++)
                table[idx] = (unsigned int)((double)step * (255.0 / (double)(rampEnd - clearEnd))) << 24;
        }
        if (idx < 255) {
            // preserved: a count used where the other arms use an index, so this stops short of 254
            for (i = idx; i <= (255 - idx); i++) table[i] = 0xFF000000;
        }
        table[255] = 0;
        break;
    case 4:
        for (i = 0; i < 256; ++i)
        {
            depth = D3D_DepthBlurBlurFuncCalc1(i);
            table[i] = (uint8_t)D3D_DepthBlurBlurFuncCalc2(depth) << 24;
        }
        table[255] = 0;
        break;
    default:
        return;
    }
}

int D3D_UpdateBlurTexture()
{
    IDirect3DSurface9* surface;
    D3DLOCKED_RECT locked;

    D3D_SaveAEParams();
    D3DRenderParams.isDepthBlurOn = depthblur_Type != 0;
    D3D_SetGlobalBlurParams();
    D3DRenderParams.pSourceTexture->lpVtbl->GetSurfaceLevel(D3DRenderParams.pSourceTexture, 0, &surface);
    surface->lpVtbl->LockRect(surface, &locked, NULL, 0);
    D3D_DepthBlurBlurFunc((int *)(locked.pBits));
    surface->lpVtbl->UnlockRect(surface);
    D3DRenderParams.d3dDev->lpVtbl->UpdateTexture(D3DRenderParams.d3dDev, (IDirect3DBaseTexture9*)(D3DRenderParams.pSourceTexture), (IDirect3DBaseTexture9*)(D3DRenderParams.pDestTexture));
    return surface->lpVtbl->Release(surface);
}

void D3D_CreateBlurTextures()
{
    int hr;

    hr = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        256,
        1,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &D3DRenderParams.pDestTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(hr);
    hr = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(D3DRenderParams.d3dDev,
        256,
        1,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_SYSTEMMEM,
        &D3DRenderParams.pSourceTexture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(hr);
    PC_AE_MASTER &= ~0x1008u;
    depthBlurType0 = 0.0;
    depthBlurType1 = 0.0;
    depthBlurNear1 = 0.0;
    D3D_UpdateBlurTexture();
}

void D3D_CreateTextureAndBlankIt(IDirect3DTexture9 **texture)
{
    int hr;
    IDirect3DSurface9* surface;

    hr = D3DRenderParams.d3dDev->lpVtbl->CreateTexture(
        D3DRenderParams.d3dDev,
        D3DRenderParams.presentParams.BackBufferWidth,
        D3DRenderParams.presentParams.BackBufferHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DRenderParams.presentParams.BackBufferFormat,
        D3DPOOL_DEFAULT,
        texture,
        NULL);
    D3D_CheckReturnAndRunSettingsApplicationIfProblem(hr);
    (*texture)->lpVtbl->GetSurfaceLevel(*texture, 0, &surface);
    D3DRenderParams.d3dDev->lpVtbl->ColorFill(D3DRenderParams.d3dDev, surface, NULL, 0);
    surface->lpVtbl->Release(surface);
}

void D3D_AETexturesInit()
{
    gMaxSimultaneousTextures = D3DRenderParams.d3dcaps.MaxSimultaneousTextures;
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.blurRenderTarget);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.blurRenderTarget2);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.unktex1_dword_B31988);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.unktex2_dword_B3198C);
    D3D_CreateTextureAndBlankIt(&D3DRenderParams.tmpRenderTarget);
}

void D3D_SetD3DStatesDefaultForAE()
{
    int stage;

    AE_SetRenderState(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, D3DZB_FALSE);
    AE_SetRenderState(&renderStateConfig.d3d_zFunc, D3DRS_ZFUNC, D3DCMP_ALWAYS);
    AE_SetRenderState(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, FALSE);
    D3DRenderParams.d3dFogModeProbably = 0;
    AE_SetRenderState(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, D3DFILL_SOLID);
    AE_SetRenderState(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_NONE);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetRenderState(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaFunc, D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ZERO);
    AE_SetRenderState(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, FALSE);
    for (stage = 0; stage < 4; stage++)
    {
        AE_SetStageState(stage, &textureStageTypeConfig[stage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(stage, &textureStageTypeConfig[stage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(stage, &textureStageTypeConfig[stage].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, stage);
        AE_SetStageState(stage, &textureStageTypeConfig[stage].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, stage, NULL);
        AE_SetSamplerState(stage, &samplerConfig[stage].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(stage, &samplerConfig[stage].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(stage, &samplerConfig[stage].d3d_addressu, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        AE_SetSamplerState(stage, &samplerConfig[stage].d3d_addressv, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    }
}

void D3D_DrawASurface(void* vertices, unsigned int fvf)
{
    int stride;

    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = fvf;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    stride = D3D_CalculateStride(fvf);
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2,  vertices, stride);
}

void D3D_DepthBlurSD(IDirect3DTexture9* texture, float uStep, float vStep)
{
    int i;
    float width;
    float uSet1Right;
    float vSet2Top;
    float uSet2Right;
    float vSet1Top;
    float height;
    float topY;
    tVertex_type_204 quad[4];

    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal4;
    quad[0].uvSet2.v = bandsVal4;
    quad[0].v1.y = bandsVal4 * height - 0.5f;
    quad[1].uvSet1.v = bandsVal3;
    quad[0].uvSet2.u = uStep;
    quad[0].uvSet1.u = -uStep;
    quad[1].uvSet2.u = uStep;
    quad[1].uvSet2.v = bandsVal3;
    quad[2].uvSet1.v = bandsVal4;
    quad[2].uvSet2.v = bandsVal4;
    topY = bandsVal3 * height;
    quad[1].v1.y = topY;
    quad[3].v1.y = topY;
    quad[1].uvSet1.u = quad[0].uvSet1.u;
    quad[3].uvSet1.v = bandsVal3;
    quad[0].v1.x = 0.0;
    quad[1].v1.x = 0.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    uSet1Right = 1.0f - uStep;
    quad[2].uvSet1.u = uSet1Right;
    quad[3].uvSet1.u = uSet1Right;
    quad[3].uvSet2.v = bandsVal3;
    uSet2Right = uStep + 1.0f;
    quad[2].uvSet2.u = uSet2Right;
    quad[3].uvSet2.u = uSet2Right;
    quad[3].v1.x = quad[2].v1.x;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0;
        quad[i].hw = 1.0;
    }
    for (i = 0; i < 2; ++i)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0x80FFFFFF);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    D3D_DrawASurface(quad, (D3DFVF_TEX2 | D3DFVF_XYZRHW));
    quad[0].uvSet1.v = bandsVal4 - vStep;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet2.u = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet2.u = 0.0;
    quad[2].uvSet1.u = 1.0;
    quad[0].uvSet2.v = bandsVal4 + vStep;
    quad[2].uvSet2.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[3].uvSet2.u = 1.0;
    vSet1Top = bandsVal3 - vStep;
    quad[1].uvSet1.v = vSet1Top;
    quad[3].uvSet1.v = vSet1Top;
    vSet2Top = bandsVal3 + vStep;
    quad[1].uvSet2.v = vSet2Top;
    quad[3].uvSet2.v = vSet2Top;
    quad[2].uvSet1.v = quad[0].uvSet1.v;
    quad[2].uvSet2.v = quad[0].uvSet2.v;
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
    D3D_DrawASurface(quad, (D3DFVF_TEX2 | D3DFVF_XYZRHW));
    for (i = 0; i < 2; i++)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
}

void D3D_DepthBlurHD(IDirect3DTexture9* texture, float uStep, float vStep)
{
    int i;
    float height;
    float topY;
    float width;
    float uSet2Right;
    float vSet4Top;
    float vSet3Bottom;
    float vSet4Bottom;
    tVertex_type_404 quad[4];

    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0x80FFFFFF);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(2, &samplerConfig[2].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(2, &samplerConfig[2].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(3, &samplerConfig[3].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(3, &samplerConfig[3].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal4;
    quad[0].uvSet2.v = bandsVal4;
    quad[0].v1.y = bandsVal4 * height - 0.5f;
    quad[1].uvSet1.v = bandsVal3;
    quad[0].uvSet2.u = uStep;
    quad[0].uvSet1.u = -uStep;
    quad[1].uvSet2.u = uStep;
    quad[1].uvSet2.v = bandsVal3;
    quad[2].uvSet1.v = bandsVal4;
    quad[2].uvSet2.v = bandsVal4;
    vSet3Bottom = bandsVal4 - vStep;
    quad[0].v1.x = 0.0;
    quad[0].uvSet3.v = vSet3Bottom;
    quad[2].uvSet3.v = vSet3Bottom;
    quad[0].uvSet3.u = 0.0;
    quad[0].uvSet4.u = 0.0;
    vSet4Bottom = bandsVal4 + vStep;
    quad[1].v1.x = 0.0;
    quad[0].uvSet4.v = vSet4Bottom;
    quad[2].uvSet4.v = vSet4Bottom;
    quad[1].uvSet3.u = 0.0;
    quad[1].uvSet4.u = 0.0;
    quad[2].uvSet3.u = 1.0;
    topY = bandsVal3 * height;
    quad[2].uvSet4.u = 1.0;
    quad[1].v1.y = topY;
    quad[3].v1.y = topY;
    quad[1].uvSet1.u = quad[0].uvSet1.u;
    quad[3].uvSet1.v = bandsVal3;
    quad[3].uvSet2.v = bandsVal3;
    quad[3].uvSet3.u = 1.0;
    quad[1].uvSet3.v = bandsVal3 - vStep;
    vSet4Top = bandsVal3 + vStep;
    quad[1].uvSet4.v = vSet4Top;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[2].uvSet1.u = 1.0f - uStep;
    uSet2Right = uStep + 1.0f;
    quad[2].uvSet2.u = uSet2Right;
    quad[3].uvSet2.u = uSet2Right;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].uvSet1.u = quad[2].uvSet1.u;
    quad[3].uvSet3.v = quad[1].uvSet3.v;
    quad[3].uvSet4.v = vSet4Top;
    quad[3].uvSet4.u = 1.0;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
    }
    D3D_DrawASurface(quad, (D3DFVF_TEX4 | D3DFVF_XYZRHW));
    for (i = 0; i < 4; ++i)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
}

void D3D_SetRenderTargetFromTexture(IDirect3DTexture9* texture)
{
    IDirect3DSurface9* surf;
    texture->lpVtbl->GetSurfaceLevel(texture, 0, &surf);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surf);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, NULL);
    surf->lpVtbl->Release(surf);
}

void D3D_DepthBlur(IDirect3DTexture9* texture, IDirect3DTexture9* renderTarget, float uStep, float vStep)
{
    D3D_SetRenderTargetFromTexture(renderTarget);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_DepthBlurHD(texture, uStep, vStep);
    else
        D3D_DepthBlurSD(texture, uStep, vStep);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_Blur(IDirect3DTexture9* texture, IDirect3DTexture9* renderTarget, float strength)
{
    double scaled;
    float step;
    int pass;

    scaled = strength * 0.039999999;
    step = (float)scaled;
    // preserved: the first pass's vertical offset comes from the unnarrowed product, the rest from the narrowed step
    D3D_DepthBlur(texture, renderTarget, (float)(step * 0.0015625),
                  (float)(scaled * 0.0020833334));
    for (pass = 2; pass <= 7; pass++)
    {
        double span = (double)step * pass;
        IDirect3DTexture9* from = (pass & 1) ? D3DRenderParams.blurRenderTarget : renderTarget;
        IDirect3DTexture9* to = (pass & 1) ? renderTarget : D3DRenderParams.blurRenderTarget;

        D3D_DepthBlur(from, to, (float)(span * 0.0015625),
                      (float)(0.0020833334 * span));
    }
}

void D3D_Unknown_AE_Func1(IDirect3DTexture9* texture)
{
    int stride;
    int i;
    float top480;
    float bottom480;
    float height;
    float width;
    tVertex_type_244 quad[4];

    D3D_SetRenderTargetFromTexture(texture);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)D3DRenderParams.blurRenderTarget);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    bottom480 = (float)(bandsVal2 * 480.0);
    top480 = (float)(bandsVal1 * 480.0);
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    stride = D3D_CalculateStride(D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    quad[0].uvSet1.v = bottom480;
    quad[0].uvSet2.v = bottom480;
    quad[0].v1.y = bottom480 - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[1].v1.y = top480;
    quad[1].uvSet1.v = top480;
    quad[1].uvSet2.v = top480;
    quad[2].uvSet1.v = bottom480;
    quad[2].uvSet2.v = bottom480;
    quad[0].color = 0xffffffff;
    quad[1].color = 0xffffffff;
    quad[2].color = 0xffffffff;
    quad[3].color = 0xffffffff;
    quad[3].v1.y = top480;
    quad[3].uvSet1.v = top480;
    quad[3].uvSet2.v = top480;
    quad[0].v1.x = 0.0;
    quad[0].v1.z = 0.0;
    quad[0].hw = 1.0;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet2.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].v1.z = 0.0;
    quad[1].hw = 1.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet2.u = 0.0;
    quad[2].v1.x = 640.0;
    quad[2].v1.z = 0.0;
    quad[2].hw = 1.0;
    quad[2].uvSet1.u = 640.0;
    quad[2].uvSet2.u = 640.0;
    quad[3].v1.x = 640.0;
    quad[3].v1.z = 0.0;
    quad[3].hw = 1.0;
    quad[3].uvSet1.u = 640.0;
    quad[3].uvSet2.u = 640.0;
    for (i = 0; i < 4; i++)
    {
        // down the screen this scaled by the width, which stretched
        // the pass past the bottom of the picture
        quad[i].v1.x = (float)(width * quad[i].v1.x * 0.0015625);
        quad[i].v1.y = (float)(height * quad[i].v1.y * 0.0020833334);
        quad[i].uvSet1.u = (float)(quad[i].uvSet1.u * 0.0015625);
        quad[i].uvSet1.v = (float)(quad[i].uvSet1.v * 0.0020833334);
        quad[i].uvSet2.u = (float)(quad[i].uvSet2.u * 0.0015625);
        quad[i].uvSet2.v = (float)(quad[i].uvSet2.v * 0.0020833334);
    }
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2, quad, stride);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetRenderState(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, FALSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, NULL);
}

float D3D_CalcTextureFactor(float phase)
{
    double wrapped;

    wrapped = phase;
    if (phase > 1.0)
        wrapped = wrapped - 1.0;
    if (wrapped > 0.5)
        wrapped = 1.0 - wrapped;
    if (wrapped < 0.1666666)
        return 1.0;
    if (wrapped <= 0.33333331)
        return (float)(1.0 - (wrapped - 0.16666667) * 6.0);
    return 0.0;
}

// TODO FIXME Ez lehet nem jó, mert az IDA-ban rossz a stack, ha nem mûködik megnézni!!!
void D3D_Unkown_AE_Func_Flag_4(IDirect3DTexture9* src, IDirect3DTexture9* dest, float amount, float phase)
{
    int green;
    unsigned int highChannels;
    int textureFactor;
    int i;
    float greenPhase;
    float bluePhase;
    tVertex_type_104 quad[4];
    float width;
    float height;

    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    greenPhase = phase - 0.33333331f + 1.0f;
    green = (unsigned int)((1.0 - D3D_CalcTextureFactor(greenPhase)) * amount * 0.5 * 510.0);
    highChannels = ((((unsigned int)((1.0 - D3D_CalcTextureFactor(phase)) * amount * 0.5 * 510.0) | 0xFFFFFF00) << 8) | green) << 8;
    bluePhase = phase - 0.66666663f + 1.0f;
    textureFactor = (unsigned int)((1.0 - D3D_CalcTextureFactor(bluePhase)) * amount * 0.5 * 510.0) | highChannels;
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, textureFactor);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[2].uvSet1.v = bandsVal2;
    quad[3].uvSet1.v = bandsVal1;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet1.v = bandsVal1;
    quad[1].v1.y = bandsVal1 * height;
    quad[2].uvSet1.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].v1.y = quad[1].v1.y;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_XYZRHW);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_Fade(IDirect3DTexture9* src, IDirect3DTexture9* dest, int opacity)
{
    int stride;
    float width;
    tVertex_type_204 quad[4];

    D3D_SetRenderTargetFromTexture(dest);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, (opacity << 24) | 0xFFFFFF);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)D3DRenderParams.unktex1_dword_B31988);
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = D3DFVF_TEX2 | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    stride = D3D_CalculateStride(D3DFVF_TEX2 | D3DFVF_XYZRHW);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    quad[0].v1.y = (float)((double)D3DRenderParams.presentParams.BackBufferHeight - 0.5f);
    quad[0].v1.x = 0.0;
    quad[0].v1.z = 0.0;
    quad[2].v1.x = width - 0.5f;
    quad[0].hw = 1.0;
    quad[0].uvSet1.u = 0.0;
    quad[2].v1.y = quad[0].v1.y;
    quad[0].uvSet1.v = 1.0;
    quad[0].uvSet2.u = 0.0;
    quad[3].v1.x = quad[2].v1.x;
    quad[0].uvSet2.v = 1.0;
    quad[1].v1.x = 0.0;
    quad[1].v1.y = 0.0;
    quad[1].v1.z = 0.0;
    quad[1].hw = 1.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet1.v = 0.0;
    quad[1].uvSet2.u = 0.0;
    quad[1].uvSet2.v = 0.0;
    quad[2].v1.z = 0.0;
    quad[2].hw = 1.0;
    quad[2].uvSet1.u = 1.0;
    quad[2].uvSet1.v = 1.0;
    quad[2].uvSet2.u = 1.0;
    quad[2].uvSet2.v = 1.0;
    quad[3].v1.y = 0.0;
    quad[3].v1.z = 0.0;
    quad[3].hw = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[3].uvSet1.v = 0.0;
    quad[3].uvSet2.u = 1.0;
    quad[3].uvSet2.v = 0.0;
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2, (void*)quad, stride);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
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

void D3D_MotionBlur(IDirect3DTexture9* src, IDirect3DTexture9* dest, float factor)
{
    int textureFactor;
    int stride;
    float height;
    float width;
    RECT rect;
    tVertex_type_104 quad[4];

    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    D3D_SetRenderTargetFromTexture(D3DRenderParams.unktex2_dword_B3198C);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    textureFactor = ((unsigned int)(255.0 - factor * 255.0) << 24) | 0xFFFFFF;
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, textureFactor);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = 0;
    renderStateConfig.fvf = D3DFVF_TEX1 | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    stride = D3D_CalculateStride(D3DFVF_TEX1 | D3DFVF_XYZRHW);
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[1].uvSet1.v = bandsVal1;
    quad[3].uvSet1.v = bandsVal1;
    quad[1].v1.y = bandsVal1 * height;
    quad[0].v1.x = 0.0;
    quad[0].v1.z = 0.0;
    quad[0].hw = 1.0;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet1.v = bandsVal2;
    quad[2].v1.x = width - 0.5f;
    quad[1].v1.x = 0.0;
    quad[1].v1.z = 0.0;
    quad[2].v1.y = quad[0].v1.y;
    quad[1].hw = 1.0;
    quad[1].uvSet1.u = 0.0;
    quad[3].v1.x = quad[2].v1.x;
    quad[2].v1.z = 0.0;
    quad[2].hw = 1.0;
    quad[3].v1.y = quad[1].v1.y;
    quad[2].uvSet1.u = 1.0;
    quad[2].uvSet1.v = bandsVal2;
    quad[3].v1.z = 0.0;
    quad[3].hw = 1.0;
    quad[3].uvSet1.u = 1.0;
    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2, (void*)quad, stride);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3D_SetRenderTargetFromTexture(dest);
    rect.left = 0;
    rect.top = (unsigned int)(bandsVal1 * height);
    rect.right = (unsigned int)width;
    rect.bottom = (unsigned int)(bandsVal2 * height);
    D3D_StretchRect(D3DRenderParams.unktex2_dword_B3198C, &rect);
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
    RECT rects[4];
    IDirect3DSurface9* surf;
    int w;
    int h;

    D3DRenderParams.d3dDev->lpVtbl->GetBackBuffer(D3DRenderParams.d3dDev, 0, 0, 0, &surf);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, surf);
    surf->lpVtbl->Release(surf);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, D3DRenderParams.pDepthStencilSurface);

    w = (int)D3DRenderParams.presentParams.BackBufferWidth;
    h = (int)D3DRenderParams.presentParams.BackBufferHeight;

    if (tweak_ae_nobands || !CIN_Bands)
    {
        rects[1].left = 0;
        rects[1].top = 0;
        rects[1].right = w;
        rects[1].bottom = h;
        D3D_StretchRect(d3dtex, &rects[1]);
    }
    else
    {
        int t = (int)(bandsVal1 * h);
        int b = (int)(bandsVal2 * h);
        int n = 0;

        if (t < 0) t = 0;
        if (b > h) b = h;

        // the picture, copied across one to one into the framed area
        rects[0].left = 0;
        rects[0].top = t;
        rects[0].right = w;
        rects[0].bottom = b;
        D3D_StretchRect(d3dtex, &rects[0]);

        // whatever the frame leaves over, in black; the bands overlap
        // the picture by a pixel so no seam shows at the edge
        if (t > 0)
        {
            rects[n].left = 0;
            rects[n].top = 0;
            rects[n].right = w;
            rects[n].bottom = t + 1;
            n++;
        }
        if (b < h)
        {
            rects[n].left = 0;
            rects[n].top = b - 1;
            rects[n].right = w;
            rects[n].bottom = h;
            n++;
        }
        if (n)
            D3DRenderParams.d3dDev->lpVtbl->Clear(D3DRenderParams.d3dDev, n, (D3DRECT*)rects, 1, 0xFF000000, 0, 0);
    }
}

void D3D_Unkown_AE_Func_Flag_1(IDirect3DTexture9* dest, IDirect3DTexture9* src)
{
    int i;
    float height;
    float width;
    tVertex_type_144 quad[4];

    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0xFF9ADA88);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[1].uvSet1.v = bandsVal1;
    quad[2].uvSet1.v = bandsVal2;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[3].uvSet1.v = bandsVal1;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].v1.y = bandsVal1 * height;
    quad[2].uvSet1.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].v1.y = quad[1].v1.y;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = 0x7F7F7F7F;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 3; ++i)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_Unkown_AE_Func_2(IDirect3DTexture9* dest, IDirect3DTexture9* src, float balance)
{
    double offset;
    int textureFactor;
    int i;
    float width;
    tVertex_type_104 quad[4];
    float height;

    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    if (balance >= 0.5)
    {
        AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_ADD);
        offset = balance - 0.5;
    }
    else
    {
        AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
        offset = 0.5 - balance;
    }
    textureFactor = (((((unsigned int)(offset * 255.0) | 0xFF000000) << 8) | (unsigned int)(offset * 255.0) | 0xFF000000) << 16) | (((unsigned int)(offset * 255.0) | 0xFF000000) << 8) | (unsigned int)(offset * 255.0) | 0xFF000000;
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, textureFactor);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[2].uvSet1.v = bandsVal2;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[3].uvSet1.v = bandsVal1;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].v1.y = bandsVal1 * height;
    quad[1].uvSet1.v = bandsVal1;
    quad[2].uvSet1.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].v1.y = quad[1].v1.y;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_XYZRHW);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_ColorBalanceLessThanZeroPointHalf(IDirect3DTexture9* dest, IDirect3DTexture9* src, float balance)
{
    int i;
    float height;
    float width;
    tVertex_type_144 quad[4];

    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[2].uvSet1.v = bandsVal2;
    quad[3].uvSet1.v = bandsVal1;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet1.v = bandsVal1;
    quad[1].v1.y = bandsVal1 * height;
    quad[2].uvSet1.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].v1.y = quad[1].v1.y;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = ((unsigned int)(balance * 510.0) << 24) | 0x808080;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_ColorBalanceMoreThanZeroPointHalfSD(int darkFactor, int brightFactor, IDirect3DTexture9* src)
{
    int i;
    float height;
    float width;
    tVertex_type_144 quad[4];

    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[1].uvSet1.v = bandsVal1;
    quad[2].uvSet1.v = bandsVal2;
    quad[3].uvSet1.v = bandsVal1;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[2].uvSet1.u = 1.0;
    quad[1].v1.y = bandsVal1 * height;
    quad[3].uvSet1.u = 1.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].v1.y = quad[1].v1.y;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
    }
    for (i = 0; i < 4; i++)
        quad[i].color = brightFactor;
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_ADD);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)src);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 4; i++)
        quad[i].color = darkFactor;
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 2; ++i)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, i);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
}

void D3D_ColorBalanceMoreThanZeroPointHalfHD(int darkFactor, IDirect3DTexture9* src, int brightFactor)
{
    int i;
    float height;
    float width;
    tVertex_type_144 quad[4];

    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, darkFactor);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_ADD);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)src);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[2].uvSet1.v = bandsVal2;
    quad[3].uvSet1.v = bandsVal1;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet1.v = bandsVal1;
    quad[1].v1.y = bandsVal1 * height;
    quad[2].uvSet1.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].v1.y = quad[1].v1.y;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = brightFactor;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 3; ++i)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, i);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
}

void D3D_ColorBalanceMoreThanZeroPointHalf(IDirect3DTexture9* dest, IDirect3DTexture9* src, float balance)
{
    double amount;
    int brightFactor;
    int darkFactor;

    amount = balance + balance - 1.0;
    brightFactor = (((((unsigned int)(255.0 * amount) | 0xFF000000) << 8) | (unsigned int)(255.0 * amount) | 0xFF000000) << 16) | (((unsigned int)(255.0 * amount) | 0xFF000000) << 8) | (unsigned int)(255.0 * amount) | 0xFF000000;
    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    darkFactor = (((((unsigned int)(amount * 128.0) | 0xFF000000) << 8) | (unsigned int)(amount * 128.0) | 0xFF000000) << 16) | (((unsigned int)(amount * 128.0) | 0xFF000000) << 8) | (unsigned int)(amount * 128.0) | 0xFF000000;
    if (gMaxSimultaneousTextures == 2)
        D3D_ColorBalanceMoreThanZeroPointHalfSD(darkFactor, brightFactor, src);
    else
        D3D_ColorBalanceMoreThanZeroPointHalfHD(darkFactor, src, brightFactor);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_ColorBalance(IDirect3DTexture9* dest, IDirect3DTexture9* src, float balance)
{
    if (balance < 0.5)
        D3D_ColorBalanceLessThanZeroPointHalf(dest, src, balance);
    else
        D3D_ColorBalanceMoreThanZeroPointHalf(dest, src, balance);
}

void D3D_WaterBlurHD(tUnkWaterBlurStruct* src, IDirect3DTexture9* texture, tUnkWaterBlurStruct* dst, float uStep, float vStep, float overscan)
{
    double uSrcLeft;
    double uSrcRight;
    double vScale;
    int dstTop;
    double srcBottom;
    double halfOverscan;
    int i;
    float uScale;
    float vSrcBottom;
    float vSet4Bottom;
    float vSrcTop;
    float yExpand;
    int dstWidth;
    float xExpand;
    int dstLeft;
    float vSet2Bottom;
    int dstRight;
    float rightX;
    int dstBottom;
    float vSet1Bottom;
    float vSet3Bottom;
    float topY;
    tVertex_type_404 quad[4];

    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0x80FFFFFF);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(2, &samplerConfig[2].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(2, &samplerConfig[2].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(3, &samplerConfig[3].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(3, &samplerConfig[3].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    uScale = (float)(unsigned int)(src->i5 - 1);
    uSrcLeft = (double)src->i1 / uScale;
    uSrcRight = (double)src->i3 / uScale;
    vScale = (double)(unsigned int)(src->i6 - 1);
    dstRight = dst->i3;
    dstWidth = dstRight - dst->i1;
    dstLeft = dst->i1;
    dstTop = dst->i2;
    dstBottom = dst->i4;
    vSrcTop = (float)((double)src->i2 / vScale);
    srcBottom = (double)src->i4;
    quad[1].uvSet1.v = vSrcTop;
    quad[1].uvSet2.v = vSrcTop;
    vSrcBottom = (float)(srcBottom / vScale);
    halfOverscan = (overscan - 1.0) * 0.5;
    xExpand = (float)((double)dstWidth * halfOverscan);
    yExpand = (float)((double)(dstBottom - dstTop) * halfOverscan);
    quad[0].v1.x = (float)((double)dstLeft - xExpand);
    quad[0].v1.y = (float)((double)dstBottom + yExpand);
    quad[0].uvSet1.u = (float)uSrcLeft;
    vSet1Bottom = (float)(vStep * 0.0 + vSrcBottom);
    quad[0].uvSet1.v = vSet1Bottom;
    quad[0].uvSet2.u = (float)(uSrcLeft - uStep);
    vSet2Bottom = vSrcBottom + vStep;
    quad[0].uvSet2.v = vSet2Bottom;
    quad[0].uvSet3.u = (float)(uSrcLeft + uStep);
    vSet3Bottom = vStep + vStep + vSrcBottom;
    quad[0].uvSet3.v = vSet3Bottom;
    quad[0].uvSet4.u = (float)uSrcLeft;
    vSet4Bottom = (float)(vStep * 3.0 + vSrcBottom);
    quad[0].uvSet4.v = vSet4Bottom;
    quad[1].v1.x = quad[0].v1.x;
    topY = (float)((double)dstTop - yExpand);
    quad[1].v1.y = topY;
    quad[1].uvSet1.u = (float)uSrcLeft;
    quad[1].uvSet2.u = quad[0].uvSet2.u;
    quad[1].uvSet3.u = quad[0].uvSet3.u;
    quad[1].uvSet4.u = (float)uSrcLeft;
    quad[1].uvSet3.v = vSrcTop;
    quad[1].uvSet4.v = vSrcTop;
    quad[2].uvSet1.v = vSet1Bottom;
    rightX = (float)((double)dstRight + xExpand);
    quad[2].uvSet2.v = vSet2Bottom;
    quad[2].v1.x = rightX;
    quad[2].uvSet3.v = vSet3Bottom;
    quad[2].v1.y = quad[0].v1.y;
    quad[2].uvSet4.v = vSet4Bottom;
    quad[2].uvSet1.u = (float)uSrcRight;
    quad[3].v1.x = rightX;
    quad[3].v1.y = topY;
    quad[2].uvSet2.u = (float)(uSrcRight - uStep);
    quad[3].uvSet1.v = vSrcTop;
    quad[3].uvSet2.v = vSrcTop;
    quad[3].uvSet4.v = vSrcTop;
    quad[3].uvSet3.v = vSrcTop;
    quad[2].uvSet3.u = (float)(uSrcRight + uStep);
    quad[2].uvSet4.u = (float)uSrcRight;
    quad[3].uvSet1.u = quad[2].uvSet4.u;
    quad[3].uvSet2.u = quad[2].uvSet2.u;
    quad[3].uvSet3.u = quad[2].uvSet3.u;
    quad[3].uvSet4.u = quad[2].uvSet4.u;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX4 | D3DFVF_XYZRHW);
    for (i = 0; i < 4; i++)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
}

void D3D_WaterBlurSD(IDirect3DTexture9* texture, tUnkWaterBlurStruct* src, tUnkWaterBlurStruct* dst, float uStep, float vStep)
{
    int i;
    float width;
    float uSet1Right;
    float vSet2Top;
    float uSet2Right;
    float vSet1Top;
    float height;
    float topY;
    tVertex_type_204 quad[4];

    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal4;
    quad[0].uvSet2.v = bandsVal4;
    quad[0].v1.y = bandsVal4 * height - 0.5f;
    quad[1].uvSet1.v = bandsVal3;
    quad[0].uvSet2.u = uStep;
    quad[0].uvSet1.u = -uStep;
    quad[1].uvSet2.u = uStep;
    quad[1].uvSet2.v = bandsVal3;
    quad[2].uvSet1.v = bandsVal4;
    quad[2].uvSet2.v = bandsVal4;
    topY = bandsVal3 * height;
    quad[1].v1.y = topY;
    quad[3].v1.y = topY;
    quad[1].uvSet1.u = quad[0].uvSet1.u;
    quad[3].uvSet1.v = bandsVal3;
    quad[0].v1.x = 0.0;
    quad[1].v1.x = 0.0;
    quad[2].v1.x = width - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    uSet1Right = 1.0f - uStep;
    quad[2].uvSet1.u = uSet1Right;
    quad[3].uvSet1.u = uSet1Right;
    quad[3].uvSet2.v = bandsVal3;
    uSet2Right = uStep + 1.0f;
    quad[2].uvSet2.u = uSet2Right;
    quad[3].uvSet2.u = uSet2Right;
    quad[3].v1.x = quad[2].v1.x;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
    }
    for (i = 0; i < 2; ++i)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    AE_SetRenderState(&renderStateConfig.d3dTextureFactor, D3DRS_TEXTUREFACTOR, 0x80FFFFFF);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_XYZRHW);
    quad[0].uvSet1.v = bandsVal4 - vStep;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet2.u = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet2.u = 0.0;
    quad[2].uvSet1.u = 1.0;
    quad[0].uvSet2.v = bandsVal4 + vStep;
    quad[2].uvSet2.u = 1.0;
    quad[3].uvSet1.u = 1.0;
    quad[3].uvSet2.u = 1.0;
    vSet1Top = bandsVal3 - vStep;
    quad[1].uvSet1.v = vSet1Top;
    quad[3].uvSet1.v = vSet1Top;
    vSet2Top = bandsVal3 + vStep;
    quad[1].uvSet2.v = vSet2Top;
    quad[3].uvSet2.v = vSet2Top;
    quad[2].uvSet1.v = quad[0].uvSet1.v;
    quad[2].uvSet2.v = quad[0].uvSet2.v;
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_XYZRHW);
    for (i = 0; i < 2; i++)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
}

void D3D_WaterBlur(IDirect3DTexture9* texture, tUnkWaterBlurStruct* src, IDirect3DTexture9* renderTarget, tUnkWaterBlurStruct* dst, float smoothX, float smoothY, float overscan)
{
    int i;
    double uSpread;
    IDirect3DSurface9* savedRenderTarget;
    IDirect3DSurface9* savedDepthStencil;
    IDirect3DSurface9* targetSurface;
    float uStep;
    float vStep;
    int srcLastColumn;
    int srcLastRow;
    int savedZEnable;
    D3DVIEWPORT9 savedViewport;
    int savedFogEnable;
    int savedFogMode;

    srcLastColumn = src->i5 - 1;
    savedRenderTarget = NULL;
    savedDepthStencil = NULL;
    uSpread = smoothX / (double)(unsigned int)srcLastColumn;
    srcLastRow = src->i6 - 1;
    uStep = (float)uSpread;
    vStep = (float)(smoothY / (double)srcLastRow);
    D3DRenderParams.d3dDev->lpVtbl->GetRenderTarget(D3DRenderParams.d3dDev, 0, &savedRenderTarget);
    D3DRenderParams.d3dDev->lpVtbl->GetViewport(D3DRenderParams.d3dDev, &savedViewport);
    renderTarget->lpVtbl->GetSurfaceLevel(renderTarget, 0, &targetSurface);
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, targetSurface);
    targetSurface->lpVtbl->Release(targetSurface);
    D3DRenderParams.d3dDev->lpVtbl->GetDepthStencilSurface(D3DRenderParams.d3dDev, &savedDepthStencil);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, NULL);
    savedZEnable = renderStateConfig.d3d_zEnable;
    AE_SetRenderState(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, D3DZB_FALSE);
    AE_SetRenderState(&renderStateConfig.d3d_zWriteEnable, D3DRS_ZWRITEENABLE, FALSE);
    savedFogMode = D3DRenderParams.d3dFogModeProbably;
    D3DRenderParams.d3dFogModeProbably = 0;
    savedFogEnable = renderStateConfig.d3dFogEnable;
    AE_SetRenderState(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dFillMode, D3DRS_FILLMODE, D3DFILL_SOLID);
    AE_SetRenderState(&renderStateConfig.d3dCullMode, D3DRS_CULLMODE, D3DCULL_NONE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ZERO);
    AE_SetRenderState(&renderStateConfig.d3d_Lighting, D3DRS_LIGHTING, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    for (i = 0; i < 4; i++)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, i);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3dTextureTransformFlags, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_addressu, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_addressv, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
    }
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_WaterBlurHD(src, texture, dst, uStep, vStep, overscan);
    else
        D3D_WaterBlurSD(texture, src, dst, uStep, vStep);
    D3DRenderParams.d3dFogModeProbably = savedFogMode;
    AE_SetRenderState(&renderStateConfig.d3dFogEnable, D3DRS_FOGENABLE, savedFogEnable);
    AE_SetRenderState(&renderStateConfig.d3d_zEnable, D3DRS_ZENABLE, savedZEnable);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    D3DRenderParams.d3dDev->lpVtbl->SetRenderTarget(D3DRenderParams.d3dDev, 0, savedRenderTarget);
    D3DRenderParams.d3dDev->lpVtbl->SetViewport(D3DRenderParams.d3dDev, &savedViewport);
    D3DRenderParams.d3dDev->lpVtbl->SetDepthStencilSurface(D3DRenderParams.d3dDev, savedDepthStencil);
    savedRenderTarget->lpVtbl->Release(savedRenderTarget);
    savedDepthStencil->lpVtbl->Release(savedDepthStencil);
}

void D3D_DoBlurWaterTexture()
{
    int right;
    int bottom;
    IDirect3DSurface9* targetSurface;
    IDirect3DSurface9* waterSurface;
    tUnkWaterBlurStruct target;
    tUnkWaterBlurStruct water;
    D3DSURFACE_DESC desc;

    D3DRenderParams.blurRenderTarget->lpVtbl->GetSurfaceLevel(D3DRenderParams.blurRenderTarget, 0, &targetSurface);
    waterTexture->lpVtbl->GetSurfaceLevel(waterTexture, 0, &waterSurface);
    water.i3 = 511;
    water.i4 = 511;
    water.i1 = 0;
    water.i2 = 0;
    water.i5 = 512;
    water.i6 = 512;
    targetSurface->lpVtbl->GetDesc(targetSurface, &desc);
    if (desc.Width <= 0x200)
    {
        target.i1 = 0;
        right = desc.Width - 1;
    }
    else
    {
        target.i1 = (desc.Width - 512) >> 1;
        right = target.i1 + 511;
    }
    target.i3 = right;
    if (desc.Height <= 512)
    {
        target.i2 = 0;
        bottom = desc.Height - 1;
    }
    else
    {
        target.i2 = (desc.Height - 512) >> 1;
        bottom = target.i2 + 511;
    }
    target.i5 = desc.Width;
    target.i6 = desc.Height;
    target.i4 = bottom;
    D3D_WaterBlur(waterTexture, &water, D3DRenderParams.blurRenderTarget, &target, tweak_water_smoothx, tweak_water_smoothy, 1.0f);
    D3D_WaterBlur(D3DRenderParams.blurRenderTarget, &target, waterTexture, &water, tweak_water_smoothx, tweak_water_smoothy, 1.0f);
    targetSurface->lpVtbl->Release(targetSurface);
    waterSurface->lpVtbl->Release(waterSurface);
}

void D3D_ZoomSmooth(IDirect3DTexture9* src, IDirect3DTexture9* dest, float zoom)
{
    int clampedStages;
    int lastStage;
    int disableStage;
    int i;
    double bottom480;
    int pass;
    int tapColour;
    double startSpan;
    double vTap0d;
    double vTap2d;
    double vTap3d;
    double bottomVd;
    double vTopEdgeTap1;
    double rightUd;
    float bottomV;
    float rightU;
    int samples;
    float stagesF;
    float uLeftTap2;
    float vTopEdgeTap2;
    int stages;
    float vStep;
    float uStep;
    float vOffset;
    float uOffset;
    float rightX;
    float vDecrement;
    float uLeftTap3;
    float vTopEdgeTap3;
    float vBottomTap2;
    float uRightTap3;
    float vBottomTap1;
    float uRightTap2;
    float vTap3;
    float vTap0;
    float uTap3;
    float height;
    int passes;
    float uTap0;
    float vTap2;
    float uTap2;
    float vTopTap1;
    float vBottomTap3;
    float bottomVBase;
    float topY;
    float vBottomTap0;
    float vTopEdgeTap0;
    float vTopTap2;
    float uDecrement;
    float vTopTap0;
    float vTopTap3;
    tVertex_type_444 quad[4];
    float scaledZoom;

    clampedStages = gMaxSimultaneousTextures;
    if (gMaxSimultaneousTextures >= 4)
        clampedStages = 4;
    if (clampedStages >= ENG_ifSingleStreamThenEq2Otherwise8)
    {
        stages = ENG_ifSingleStreamThenEq2Otherwise8;
    }
    else if (gMaxSimultaneousTextures >= 4)
    {
        stages = 4;
    }
    else
    {
        stages = gMaxSimultaneousTextures;
    }
    passes = 8 / stages;
    samples = stages * (8 / stages);
    scaledZoom = (float)(8.0 / (double)samples * zoom);
    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    lastStage = 1;
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)src);
    if (stages >= 3)
    {
        lastStage = 2;
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)src);
        if (stages >= 4)
        {
            lastStage = 3;
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)src);
        }
    }
    disableStage = lastStage + 1;
    AE_SetStageState(disableStage, &textureStageTypeConfig[disableStage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(disableStage, &textureStageTypeConfig[disableStage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    bottom480 = bandsVal2 * 480.0;
    pass = 0;
    bottomVBase = (float)((bottom480 - 1.0) * 0.0020833334);
    tapColour = ((((((256 / samples - 1) << 8) | (256 / samples - 1)) << 16) | ((256 / samples - 1) << 8) | (256 / samples - 1)) << 24) | ((((256 / samples - 1) << 8) | (256 / samples - 1)) << 16) | ((256 / samples - 1) << 8) | (256 / samples - 1);
    uStep = (float)(scaledZoom * 6400.0 / bottom480 * 0.0015625);
    vStep = (float)(scaledZoom * 10.0 * 0.0020833334);
    stagesF = (float)stages;
    startSpan = (double)(passes - 1) * stagesF;
    uOffset = (float)(uStep * startSpan);
    vOffset = (float)(startSpan * vStep);
    if (passes > 0)
    {
        uTap0 = (float)(uStep * 0.0);
        uTap2 = uStep + uStep;
        uTap3 = (float)(uStep * 3.0);
        rightX = (float)((double)D3DRenderParams.presentParams.BackBufferWidth - 0.5);
        vTap0d = vStep * 0.0;
        vTap0 = (float)vTap0d;
        vTopTap0 = (float)(vTap0d + bandsVal1);
        vTopTap1 = vStep + bandsVal1;
        vTap2d = vStep + vStep;
        vTap2 = (float)vTap2d;
        vTopTap2 = (float)(vTap2d + bandsVal1);
        vTap3d = vStep * 3.0;
        vTopTap3 = (float)(vTap3d + bandsVal1);
        uDecrement = stagesF * uStep;
        vDecrement = stagesF * vStep;
        for (pass = 0; pass < passes; pass++)
        {
            quad[0].v1.x = 0.0;
            quad[1].v1.x = 0.0;
            quad[2].v1.x = rightX;
            quad[0].v1.y = (float)(bandsVal2 * height - 0.5);
            quad[0].uvSet4.u = uTap0 + uOffset;
            bottomVd = bottomVBase - vOffset;
            bottomV = (float)bottomVd;
            vBottomTap0 = (float)(bottomVd - vTap0);
            quad[0].uvSet4.v = vBottomTap0;
            quad[0].uvSet3.u = uOffset + uStep;
            vBottomTap1 = bottomV - vStep;
            quad[0].uvSet3.v = vBottomTap1;
            uLeftTap2 = uTap2 + uOffset;
            quad[0].uvSet2.u = uLeftTap2;
            quad[1].uvSet2.u = uLeftTap2;
            quad[2].uvSet4.v = vBottomTap0;
            vBottomTap2 = bottomV - vTap2;
            quad[0].uvSet2.v = vBottomTap2;
            uLeftTap3 = uTap3 + uOffset;
            quad[0].uvSet1.u = uLeftTap3;
            quad[1].uvSet1.u = uLeftTap3;
            quad[2].uvSet3.v = vBottomTap1;
            vTap3 = (float)vTap3d;
            vBottomTap3 = bottomV - vTap3;
            quad[0].uvSet1.v = vBottomTap3;
            topY = bandsVal1 * height;
            quad[1].v1.y = topY;
            quad[1].uvSet4.u = quad[0].uvSet4.u;
            vTopEdgeTap0 = vTopTap0 + vOffset;
            quad[1].uvSet4.v = vTopEdgeTap0;
            quad[1].uvSet3.u = quad[0].uvSet3.u;
            vTopEdgeTap1 = vTopTap1 + vOffset;
            quad[1].uvSet3.v = (float)vTopEdgeTap1;
            vTopEdgeTap2 = vTopTap2 + vOffset;
            quad[1].uvSet2.v = vTopEdgeTap2;
            vTopEdgeTap3 = vTopTap3 + vOffset;
            quad[1].uvSet1.v = vTopEdgeTap3;
            quad[2].v1.y = quad[0].v1.y;
            rightUd = 0.99843752 - uOffset;
            rightU = (float)rightUd;
            quad[2].uvSet4.u = (float)(rightUd - uTap0);
            quad[2].uvSet3.u = rightU - uStep;
            uRightTap2 = rightU - uTap2;
            quad[2].uvSet1.v = vBottomTap3;
            quad[2].uvSet2.u = uRightTap2;
            quad[2].uvSet2.v = vBottomTap2;
            quad[3].v1.x = rightX;
            uRightTap3 = rightU - uTap3;
            quad[3].uvSet4.v = vTopEdgeTap0;
            quad[2].uvSet1.u = uRightTap3;
            quad[3].v1.y = topY;
            quad[3].uvSet4.u = quad[2].uvSet4.u;
            quad[3].uvSet2.u = uRightTap2;
            quad[3].uvSet3.u = quad[2].uvSet3.u;
            quad[3].uvSet1.u = uRightTap3;
            quad[3].uvSet3.v = (float)vTopEdgeTap1;
            quad[3].uvSet2.v = vTopEdgeTap2;
            quad[3].uvSet1.v = vTopEdgeTap3;
            if (pass)
            {
                AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
                AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
                AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
                AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
            }
            else
            {
                AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
            }
            for (i = 0; i < 4; i++)
            {
                quad[i].v1.z = 0.0f;
                quad[i].hw = 1.0f;
                quad[i].color = tapColour;
            }
            D3D_DrawASurface(quad, D3DFVF_TEX4 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
            uOffset = uOffset - uDecrement;
            vOffset = vOffset - vDecrement;
        }
    }
    for (i = 0; i < stages; i++)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_ZoomSmooth2(IDirect3DTexture9* src, IDirect3DTexture9* dest, float zoom)
{
    int clampedStages;
    int lastStage;
    int disableStage;
    int i;
    double stagesD;
    int pass;
    int tapColour;
    long double cos0;
    long double sin0;
    long double cos1;
    long double sin1;
    long double cos2;
    long double angle3;
    double dv0;
    double dv1;
    double dv2;
    double dv3;
    double du3;
    double scaledX;
    int samples;
    double angle1;
    double angle2;
    float u0;
    float u1;
    float u2;
    float u3;
    float top480;
    float bottom480;
    int stages;
    float angle;
    float angleStep;
    float angleAdvance;
    int passes;
    float angleStep2;
    float height;
    float width;
    float angleStep3;
    double sin2;
    double sin3;
    double cos3;
    tVertex_type_444 quad[4];

    clampedStages = gMaxSimultaneousTextures;
    if (gMaxSimultaneousTextures >= 4)
        clampedStages = 4;
    if (clampedStages >= ENG_ifSingleStreamThenEq2Otherwise8)
    {
        stages = ENG_ifSingleStreamThenEq2Otherwise8;
    }
    else if (gMaxSimultaneousTextures >= 4)
    {
        stages = 4;
    }
    else
    {
        stages = gMaxSimultaneousTextures;
    }
    passes = 8 / stages;
    D3D_SetRenderTargetFromTexture(dest);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)src);
    lastStage = 1;
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)src);
    if (stages >= 3)
    {
        lastStage = 2;
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)src);
        if (stages >= 4)
        {
            lastStage = 3;
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)src);
        }
    }
    disableStage = lastStage + 1;
    AE_SetStageState(disableStage, &textureStageTypeConfig[disableStage].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(disableStage, &textureStageTypeConfig[disableStage].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    bottom480 = (float)(bandsVal2 * 480.0);
    top480 = (float)(bandsVal1 * 480.0);
    angleStep = (float)(zoom * 0.039999999);
    stagesD = (double)stages;
    pass = 0;
    angle = (float)((double)passes * stagesD * angleStep * -0.5);
    samples = stages * (8 / stages);
    tapColour = ((((((256 / samples - 1) << 8) | (256 / samples - 1)) << 16) | ((256 / samples - 1) << 8) | (256 / samples - 1)) << 24) | ((((256 / samples - 1) << 8) | (256 / samples - 1)) << 16) | ((256 / samples - 1) << 8) | (256 / samples - 1);
    if (passes > 0)
    {
        angleAdvance = (float)(stagesD * angleStep);
        for (pass = 0; pass < passes; pass++)
        {
            cos0 = cos(angle);
            quad[0].v1.y = (float)(bottom480 - 0.5);
            quad[0].uvSet2.v = bottom480;
            quad[0].uvSet1.v = bottom480;
            quad[0].uvSet3.v = bottom480;
            quad[0].uvSet4.v = bottom480;
            quad[1].v1.y = top480;
            quad[1].uvSet3.v = top480;
            quad[1].uvSet1.v = top480;
            quad[1].uvSet2.v = top480;
            quad[1].uvSet4.v = top480;
            quad[2].v1.y = quad[0].v1.y;
            quad[2].uvSet2.v = bottom480;
            quad[2].uvSet1.v = bottom480;
            quad[2].uvSet4.v = bottom480;
            quad[2].uvSet3.v = bottom480;
            quad[3].v1.y = top480;
            quad[3].uvSet2.v = top480;
            quad[3].uvSet3.v = top480;
            quad[0].v1.x = 0.0;
            quad[0].uvSet1.u = 0.0;
            quad[0].uvSet2.u = 0.0;
            quad[0].uvSet3.u = 0.0;
            quad[0].uvSet4.u = 0.0;
            quad[1].v1.x = 0.0;
            quad[1].uvSet1.u = 0.0;
            quad[1].uvSet2.u = 0.0;
            quad[1].uvSet3.u = 0.0;
            quad[1].uvSet4.u = 0.0;
            quad[2].v1.x = 640.0;
            quad[2].uvSet1.u = 640.0;
            quad[2].uvSet2.u = 640.0;
            quad[2].uvSet3.u = 640.0;
            quad[2].uvSet4.u = 640.0;
            quad[3].v1.x = 640.0;
            quad[3].uvSet1.u = 640.0;
            quad[3].uvSet1.v = top480;
            quad[3].uvSet2.u = 640.0;
            quad[3].uvSet3.u = 640.0;
            quad[3].uvSet4.u = 640.0;
            quad[3].uvSet4.v = top480;
            sin0 = sin(angle);
            angle1 = angle + angleStep;
            cos1 = cos(angle1);
            sin1 = sin(angle1);
            angleStep2 = angleStep + angleStep;
            angle2 = angleStep2 + angle;
            cos2 = cos(angle2);
            sin2 = sin(angle2);
            angleStep3 = (float)(angleStep * 3.0);
            angle3 = angleStep3 + angle;
            cos3 = cos(angle3);
            sin3 = sin(angle3);
            for (i = 0; i < 4; i++)
            {
                dv0 = quad[i].uvSet1.v - 240.0;
                u0 = (float)((quad[i].uvSet1.u - 320.0) * cos0 + sin0 * dv0);
                quad[i].uvSet1.u = u0;
                quad[i].uvSet1.v = (float)(cos0 * dv0 - u0 * sin0);
                quad[i].uvSet1.u = (float)(quad[i].uvSet1.u + 320.0);
                quad[i].uvSet1.v = (float)(quad[i].uvSet1.v + 240.0);
                dv1 = quad[i].uvSet2.v - 240.0;
                u1 = (float)((quad[i].uvSet2.u - 320.0) * cos1 + sin1 * dv1);
                quad[i].uvSet2.u = u1;
                quad[i].uvSet2.v = (float)(cos1 * dv1 - u1 * sin1);
                quad[i].uvSet2.u = (float)(quad[i].uvSet2.u + 320.0);
                quad[i].uvSet2.v = (float)(quad[i].uvSet2.v + 240.0);
                dv2 = quad[i].uvSet3.v - 240.0;
                u2 = (float)((quad[i].uvSet3.u - 320.0) * cos2 + sin2 * dv2);
                quad[i].uvSet3.u = u2;
                quad[i].uvSet3.v = (float)(cos2 * dv2 - u2 * sin2);
                quad[i].uvSet3.u = (float)(quad[i].uvSet3.u + 320.0);
                quad[i].uvSet3.v = (float)(quad[i].uvSet3.v + 240.0);
                dv3 = quad[i].uvSet4.v - 240.0;
                du3 = quad[i].uvSet4.u - 320.0;
                u3 = (float)(du3 * cos3 + dv3 * sin3);
                quad[i].uvSet4.u = u3;
                quad[i].uvSet4.v = (float)(dv3 * cos3 - u3 * sin3);
                quad[i].uvSet4.u = (float)(quad[i].uvSet4.u + 320.0);
                quad[i].uvSet4.v = (float)(quad[i].uvSet4.v + 240.0);
            }
            for (i = 0; i < 4; i++)
            {
                quad[i].v1.z = 0.0f;
                scaledX = width * quad[i].v1.x;
                quad[i].hw = 1.0f;
                quad[i].color = tapColour;

                quad[i].v1.x = (float)(scaledX * 0.0015625);
                quad[i].v1.y = (float)(height * quad[i].v1.y * 0.0020833334);
                quad[i].uvSet1.u = (float)(quad[i].uvSet1.u * 0.0015625);
                quad[i].uvSet1.v = (float)(quad[i].uvSet1.v * 0.0020833334);
                quad[i].uvSet2.u = (float)(quad[i].uvSet2.u * 0.0015625);
                quad[i].uvSet2.v = (float)(quad[i].uvSet2.v * 0.0020833334);
                quad[i].uvSet3.u = (float)(quad[i].uvSet3.u * 0.0015625);
                quad[i].uvSet3.v = (float)(quad[i].uvSet3.v * 0.0020833334);
                quad[i].uvSet4.u = (float)(quad[i].uvSet4.u * 0.0015625);
                quad[i].uvSet4.v = (float)(quad[i].uvSet4.v * 0.0020833334);
            }
            if (pass)
            {
                AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
                AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
                AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
                AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
            }
            else
            {
                AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
            }
            D3D_DrawASurface(quad, D3DFVF_TEX4 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
            angle = angleAdvance + angle;
        }
    }
    for (i = 0; i < stages; i++)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_MotionSmoothHD(IDirect3DTexture9* texture, float dx, float dy)
{
    int i;
    double v2StepD;
    float uStep;
    float vStep;
    float v2Step;
    float vSet2Top;
    float height;
    float topY;
    float v3Step;
    float vSet1Top;
    float v0Step;
    float vSet4Top;
    float vSet1Bottom;
    float uSet2Right;
    float vSet2Bottom;
    float uSet3Right;
    float vSet4Bottom;
    float vSet3Bottom;
    float uSet4Right;
    float u2Step;
    float u0Step;
    float width;
    tVertex_type_444 quad[4];

    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].v1.x = 0.0;
    quad[1].v1.x = 0.0;
    uStep = (float)(dx * 0.0015625);
    vStep = (float)(dy * 0.0020833334);
    quad[0].v1.y = (float)(bandsVal2 * height - 0.5);
    quad[0].uvSet1.u = (float)(uStep * -3.0);
    v3Step = (float)(vStep * 3.0);
    vSet1Bottom = bandsVal2 - v3Step;
    quad[0].uvSet1.v = vSet1Bottom;
    u2Step = uStep + uStep;
    quad[0].uvSet2.u = u2Step;
    quad[1].uvSet2.u = u2Step;
    v2StepD = vStep + vStep;
    v2Step = (float)v2StepD;
    vSet2Bottom = (float)(v2StepD + bandsVal2);
    quad[0].uvSet2.v = vSet2Bottom;
    quad[0].uvSet3.u = -uStep;
    vSet3Bottom = bandsVal2 - vStep;
    quad[0].uvSet3.v = vSet3Bottom;
    u0Step = (float)(uStep * 0.0);
    quad[0].uvSet4.u = u0Step;
    quad[1].uvSet4.u = u0Step;
    v0Step = (float)(vStep * 0.0);
    vSet4Bottom = bandsVal2 + v0Step;
    quad[0].uvSet4.v = vSet4Bottom;
    topY = bandsVal1 * height;
    quad[1].v1.y = topY;
    quad[1].uvSet1.u = quad[0].uvSet1.u;
    vSet1Top = bandsVal1 - v3Step;
    quad[1].uvSet1.v = vSet1Top;
    vSet2Top = v2Step + bandsVal1;
    quad[1].uvSet2.v = vSet2Top;
    quad[1].uvSet3.u = quad[0].uvSet3.u;
    quad[1].uvSet3.v = bandsVal1 - vStep;
    vSet4Top = bandsVal1 + v0Step;
    quad[1].uvSet4.v = vSet4Top;
    quad[2].v1.x = (float)(width - 0.5);
    quad[2].v1.y = quad[0].v1.y;
    quad[2].uvSet1.v = vSet1Bottom;
    quad[2].uvSet2.v = vSet2Bottom;
    quad[2].uvSet3.v = vSet3Bottom;
    quad[2].uvSet1.u = (float)(1.0 - uStep * 3.0);
    quad[2].uvSet4.v = vSet4Bottom;
    quad[3].v1.y = topY;
    quad[3].uvSet1.v = vSet1Top;
    quad[3].uvSet2.v = vSet2Top;
    uSet2Right = (float)(u2Step + 1.0);
    quad[2].uvSet2.u = uSet2Right;
    quad[3].uvSet2.u = uSet2Right;
    quad[3].uvSet4.v = vSet4Top;
    uSet3Right = (float)(1.0 - uStep);
    quad[2].uvSet3.u = uSet3Right;
    quad[3].uvSet3.u = uSet3Right;
    uSet4Right = (float)(u0Step + 1.0);
    quad[2].uvSet4.u = uSet4Right;
    quad[3].uvSet4.u = uSet4Right;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].uvSet1.u = quad[2].uvSet1.u;
    quad[3].uvSet3.v = quad[1].uvSet3.v;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = 0x3F3F3F3F;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX4 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 4; i++)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
}

void D3D_MotionSmoothSD(IDirect3DTexture9* unusedA, IDirect3DTexture9* unusedB, IDirect3DTexture9* texture, float dx, float dy)
{
    int i;
    double v0StepD;
    float v2Step;
    float vSet2Top;
    float vSet2TopB;
    float uStep;
    float vStep;
    float vSet2Bottom;
    float u0Step;
    float vSet1Bottom;
    float uSet2Right;
    float vSet2BottomB;
    float u2Step;
    float vSet1Top;
    float height;
    float topY;
    float width;
    float uSet1Right;
    float v3Step;
    tVertex_type_244 quad[4];

    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].v1.x = 0.0;
    quad[1].v1.x = 0.0;
    uStep = (float)(dx * 0.0015625);
    vStep = (float)(dy * 0.0020833334);
    quad[0].v1.y = (float)(bandsVal2 * height - 0.5);
    quad[0].uvSet1.u = (float)(uStep * -3.0);
    v3Step = (float)(vStep * 3.0);
    vSet1Bottom = bandsVal2 - v3Step;
    quad[0].uvSet1.v = vSet1Bottom;
    quad[2].uvSet1.v = vSet1Bottom;
    u2Step = uStep + uStep;
    quad[0].uvSet2.u = u2Step;
    quad[1].uvSet2.u = u2Step;
    v2Step = vStep + vStep;
    vSet2Bottom = bandsVal2 + v2Step;
    quad[0].uvSet2.v = vSet2Bottom;
    quad[2].uvSet2.v = vSet2Bottom;
    topY = bandsVal1 * height;
    quad[1].v1.y = topY;
    quad[3].v1.y = topY;
    quad[1].uvSet1.u = quad[0].uvSet1.u;
    quad[1].uvSet1.v = bandsVal1 - v3Step;
    vSet2Top = bandsVal1 + v2Step;
    quad[1].uvSet2.v = vSet2Top;
    quad[3].uvSet2.v = vSet2Top;
    quad[2].v1.x = (float)(width - 0.5);
    quad[2].v1.y = quad[0].v1.y;
    quad[2].uvSet1.u = (float)(1.0 - uStep * 3.0);
    uSet2Right = (float)(u2Step + 1.0);
    quad[2].uvSet2.u = uSet2Right;
    quad[3].uvSet2.u = uSet2Right;
    quad[3].v1.x = quad[2].v1.x;
    quad[3].uvSet1.u = quad[2].uvSet1.u;
    quad[3].uvSet1.v = quad[1].uvSet1.v;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = 0x3F3F3F3F;
    }
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    quad[0].uvSet1.u = -uStep;
    quad[0].uvSet1.v = bandsVal2 - vStep;
    u0Step = (float)(uStep * 0.0);
    quad[0].uvSet2.u = u0Step;
    v0StepD = vStep * 0.0;
    vSet2BottomB = (float)(bandsVal2 + v0StepD);
    quad[0].uvSet2.v = vSet2BottomB;
    quad[2].uvSet2.v = vSet2BottomB;
    quad[1].uvSet1.u = quad[0].uvSet1.u;
    vSet1Top = bandsVal1 - vStep;
    quad[1].uvSet1.v = vSet1Top;
    quad[3].uvSet1.v = vSet1Top;
    quad[1].uvSet2.u = u0Step;
    vSet2TopB = (float)(v0StepD + bandsVal1);
    quad[3].uvSet2.v = vSet2TopB;
    quad[1].uvSet2.v = vSet2TopB;
    uSet1Right = (float)(1.0 - uStep);
    quad[2].uvSet1.u = uSet1Right;
    quad[3].uvSet1.u = uSet1Right;
    quad[2].uvSet1.v = quad[0].uvSet1.v;
    quad[2].uvSet2.u = (float)(u0Step + 1.0);
    quad[3].uvSet2.u = quad[2].uvSet2.u;
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)texture);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 2; ++i)
    {
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
}

void D3D_MotionSmooth(IDirect3DTexture9* unusedA, IDirect3DTexture9* unusedB, IDirect3DTexture9* texture, IDirect3DTexture9* renderTarget, float dx, float dy)
{
    D3D_SetRenderTargetFromTexture(renderTarget);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_MotionSmoothHD(texture, dx, dy);
    else
        D3D_MotionSmoothSD(unusedA, unusedB, texture, dx, dy);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_RemenanceSD(IDirect3DTexture9* texture, float opacity)
{
    int i;
    float height;
    float topY;
    float width;
    float rightX;
    float vTopTap1;
    tVertex_type_244 quad[4];

    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[1].uvSet1.v = bandsVal1;
    quad[2].uvSet1.v = bandsVal2;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet2.u = 0.0078125;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[0].uvSet2.v = bandsVal2 - 0.010416667f;
    quad[1].uvSet2.u = 0.0078125;
    quad[2].uvSet1.u = 1.0;
    quad[2].uvSet2.u = 1.0078125;
    quad[3].uvSet1.u = 1.0;
    quad[3].uvSet1.v = bandsVal1;
    topY = bandsVal1 * height;
    quad[3].uvSet2.u = 1.0078125;
    quad[1].v1.y = topY;
    quad[3].v1.y = topY;
    vTopTap1 = bandsVal1 - 0.010416667f;
    quad[1].uvSet2.v = vTopTap1;
    rightX = width - 0.5f;
    quad[2].v1.x = rightX;
    quad[3].v1.x = rightX;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].uvSet2.v = vTopTap1;
    quad[2].uvSet2.v = quad[0].uvSet2.v;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = ((unsigned int)(opacity * 255.0) << 24) | 0xFFFFFF;
    }
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 2; i++)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, i);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
}

void D3D_RemenanceHD(IDirect3DTexture9* texture, float opacity)
{
    int i;
    float height;
    float topY;
    float width;
    float rightX;
    float vTopTap1;
    tVertex_type_244 quad[4];

    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(1, &samplerConfig[1].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    AE_SetSamplerState(2, &samplerConfig[2].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(2, &samplerConfig[2].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 1);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    quad[0].uvSet1.v = bandsVal2;
    quad[1].uvSet1.v = bandsVal1;
    quad[2].uvSet1.v = bandsVal2;
    quad[0].v1.y = bandsVal2 * height - 0.5f;
    quad[0].v1.x = 0.0;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet2.u = 0.0078125f;
    quad[1].v1.x = 0.0;
    quad[1].uvSet1.u = 0.0;
    quad[0].uvSet2.v = bandsVal2 - 0.010416667f;
    quad[1].uvSet2.u = 0.0078125f;
    quad[2].uvSet1.u = 1.0;
    quad[2].uvSet2.u = 1.0078125f;
    quad[3].uvSet1.u = 1.0;
    quad[3].uvSet1.v = bandsVal1;
    topY = bandsVal1 * height;
    quad[3].uvSet2.u = 1.0078125f;
    quad[1].v1.y = topY;
    quad[3].v1.y = topY;
    vTopTap1 = bandsVal1 - 0.010416667f;
    quad[1].uvSet2.v = vTopTap1;
    rightX = width - 0.5f;
    quad[2].v1.x = rightX;
    quad[3].v1.x = rightX;
    quad[2].v1.y = quad[0].v1.y;
    quad[3].uvSet2.v = vTopTap1;
    quad[2].uvSet2.v = quad[0].uvSet2.v;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.z = 0.0f;
        quad[i].hw = 1.0f;
        quad[i].color = ((unsigned int)(opacity * 255.0) << 24) | 0xFFFFFF;
    }
    D3D_DrawASurface(quad, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    for (i = 0; i < 3; ++i)
    {
        AE_SetSamplerState(i, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(i, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(i, &textureStageTypeConfig[i].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, i);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, NULL);
    }
}

void D3D_Remenance(IDirect3DTexture9* texture, IDirect3DTexture9* renderTarget, float opacity)
{
    D3D_DepthBlur(texture, renderTarget, 0.0015625f, 0.0020833334f);
    D3D_DepthBlur(renderTarget, D3DRenderParams.blurRenderTarget, 0.0046875002f, 0.0062500001f);
    D3D_DepthBlur(D3DRenderParams.blurRenderTarget, renderTarget, 0.0078125f, 0.010416667f);
    D3D_DepthBlur(renderTarget, D3DRenderParams.blurRenderTarget, 0.0109375f, 0.014583333f);
    D3D_SetRenderTargetFromTexture(renderTarget);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    if (gMaxSimultaneousTextures == 2)
        D3D_RemenanceSD(texture, opacity);
    else
        D3D_RemenanceHD(texture, opacity);
        
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
}

void D3D_Unknown_AE_FuncDepthBlur(IDirect3DTexture9* texture)
{
    int stride;
    int i;
    float top480;
    float bottom480;
    float height;
    float width;
    tVertex_type_244 quad[4];

    D3D_SetRenderTargetFromTexture(texture);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)texture);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)(D3DRenderParams.blurRenderTarget));
    width = (float)D3DRenderParams.presentParams.BackBufferWidth;
    height = (float)D3DRenderParams.presentParams.BackBufferHeight;
    bottom480 = bandsVal2 * 480.0f;
    top480 = bandsVal1 * 480.0f;
    D3D_SetVertexShader(NULL);
    renderStateConfig.vertexShader = NULL;
    renderStateConfig.fvf = D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW;
    D3DRenderParams.d3dDev->lpVtbl->SetVertexShader(D3DRenderParams.d3dDev, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetFVF(D3DRenderParams.d3dDev, renderStateConfig.fvf);
    stride = D3D_CalculateStride(D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    quad[0].uvSet1.v = bottom480;
    quad[0].uvSet2.v = bottom480;
    quad[0].v1.y = bottom480 - 0.5f;
    quad[2].v1.y = quad[0].v1.y;
    quad[1].v1.y = top480;
    quad[1].uvSet1.v = top480;
    quad[1].uvSet2.v = top480;
    quad[2].uvSet1.v = bottom480;
    quad[2].uvSet2.v = bottom480;
    quad[0].color = 0xffffffff;
    quad[1].color = 0xffffffff;
    quad[2].color = 0xffffffff;
    quad[3].color = 0xffffffff;
    quad[3].v1.y = top480;
    quad[3].uvSet1.v = top480;
    quad[3].uvSet2.v = top480;
    quad[0].v1.x = 0.0;
    quad[0].v1.z = 0.0;
    quad[0].hw = 1.0;
    quad[0].uvSet1.u = 0.0;
    quad[0].uvSet2.u = 0.0;
    quad[1].v1.x = 0.0;
    quad[1].v1.z = 0.0;
    quad[1].hw = 1.0;
    quad[1].uvSet1.u = 0.0;
    quad[1].uvSet2.u = 0.0;
    quad[2].v1.x = 640.0;
    quad[2].v1.z = 0.0;
    quad[2].hw = 1.0;
    quad[2].uvSet1.u = 640.0;
    quad[2].uvSet2.u = 640.0;
    quad[3].v1.x = 640.0;
    quad[3].v1.z = 0.0;
    quad[3].hw = 1.0;
    quad[3].uvSet1.u = 640.0;
    quad[3].uvSet2.u = 640.0;
    for (i = 0; i < 4; i++)
    {
        quad[i].v1.x = (float)(width * quad[i].v1.x * 0.0015625);
        quad[i].v1.y = (float)(height * quad[i].v1.y * 0.0020833334);
        quad[i].uvSet1.u = (float)(quad[i].uvSet1.u * 0.0015625);
        quad[i].uvSet1.v = (float)(quad[i].uvSet1.v * 0.0020833334);
        quad[i].uvSet2.u = (float)(quad[i].uvSet2.u * 0.0015625);
        quad[i].uvSet2.v = (float)(quad[i].uvSet2.v * 0.0020833334);
    }

    D3DRenderParams.d3dDev->lpVtbl->DrawPrimitiveUP(D3DRenderParams.d3dDev, D3DPT_TRIANGLESTRIP, 2, (void*)quad, stride);

    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetRenderState(&renderStateConfig.d3dAlphaTest, D3DRS_ALPHATESTENABLE, FALSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, NULL);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, NULL);
}

HRESULT CopyRenderTargetTexture(LPDIRECT3DDEVICE9 pDevice, IDirect3DTexture9* pSourceTexture, IDirect3DTexture9* pDestTexture)
{
    if (!pDevice || !pSourceTexture || !pDestTexture)
    {
        return E_INVALIDARG;
    }

    IDirect3DSurface9* pSourceSurface = NULL;
    IDirect3DSurface9* pDestSurface = NULL;

    HRESULT hr = pSourceTexture->lpVtbl->GetSurfaceLevel(pSourceTexture, 0, &pSourceSurface);
    if (FAILED(hr)) return hr;

    hr = pDestTexture->lpVtbl->GetSurfaceLevel(pDestTexture, 0, &pDestSurface);
    if (FAILED(hr))
    {
        pSourceSurface->lpVtbl->Release(pSourceSurface);
        return hr;
    }

    hr = pDevice->lpVtbl->StretchRect(pDevice, pSourceSurface, NULL, pDestSurface, NULL, D3DTEXF_NONE);

    pSourceSurface->lpVtbl->Release(pSourceSurface);
    pDestSurface->lpVtbl->Release(pDestSurface);

    return hr;
}

void D3D_PC_AE(tWorld* param1)
{
    IDirect3DTexture9* current;
    IDirect3DTexture9* spare;
    IDirect3DTexture9* swap;
    double fadeSpan;
    BOOL fadeDone;
    double dx;
    float motionX;
    float motionY;
    float fadeStep;
    RECT wholeScreen;

    D3D_SetD3DStatesDefaultForAE();
    if (tweak_ae_nobands || !CIN_Bands)
    {
        bandsVal1 = 0.0f;
        bandsVal2 = 1.0f;
        bandsVal3 = 0.0f;
        bandsVal4 = 1.0f;
    }
    else
    {
        float top;
        float bottom;

        D3D_GetBandFractions(&top, &bottom);
        bandsVal1 = top;
        bandsVal2 = bottom;
        bandsVal3 = top > AE_BAND_MARGIN ? top - AE_BAND_MARGIN : 0.0f;
        bandsVal4 = bottom + AE_BAND_MARGIN < 1.0f ? bottom + AE_BAND_MARGIN : 1.0f;
        D3D_AspectTrace("ae", 0, 0,
                        (int)D3DRenderParams.presentParams.BackBufferWidth,
                        (int)D3DRenderParams.presentParams.BackBufferHeight);
    }
    current = D3DRenderParams.blurTex;
    spare = D3DRenderParams.blurRenderTarget2;
    if (tweak_ae_no)
    {
        PC_AE_MASTER = 0;
        colorBalanceParam1 = 0.5f;
        unkAEFunc2Param = 0.5f;
    }
    if ((PC_AE_MASTER & 8) != 0 || (PC_AE_MASTER & 0x1000) != 0)// depthblur
    {
        D3D_DepthBlur(D3DRenderParams.blurTex, D3DRenderParams.blurRenderTarget, 0.0015625f, 0.0020833334f);
        D3D_DepthBlur(D3DRenderParams.blurRenderTarget, spare, 0.003125f, 0.0041666669f);
        D3D_DepthBlur(spare, D3DRenderParams.blurRenderTarget, 0.0046875002f, 0.0062500001f);
        D3D_Unknown_AE_FuncDepthBlur(current);
    }
    if ((PC_AE_MASTER & 4) != 0)
    {
        swap = spare;
        D3D_Unkown_AE_Func_Flag_4(current, spare, unknownAEFlag4GlobalVar, texFactorManual);
        spare = current;
        current = swap;
    }
    if ((PC_AE_MASTER & 0x800) != 0 && remenanceFactor > 0.0099999998)
    {
        swap = spare;
        D3D_Remenance(current, spare, remenanceFactor);
        spare = current;
        current = swap;
    }
    if ((PC_AE_MASTER & 0x40000) != 0)
    {
        swap = spare;
        fadeSpan = gAEOpacity * 60.0;
        if (fadeSpan >= 255.0)
            fadeStep = 1.0;
        else
            fadeStep = (float)(255.0 / fadeSpan);
        if (D3DRenderParams.opacity == 255)
        {
            wholeScreen.left = 0;
            wholeScreen.top = 0;
            wholeScreen.right = (unsigned int)(double)D3DRenderParams.presentParams.BackBufferWidth;
            wholeScreen.bottom = (unsigned int)(double)D3DRenderParams.presentParams.BackBufferHeight;
            D3D_SetRenderTargetFromTexture(D3DRenderParams.unktex1_dword_B31988);
            D3D_StretchRect(current, &wholeScreen);
        }
        D3D_Fade(current, spare, D3DRenderParams.opacity);
        spare = current;
        current = swap;
        fadeDone = D3DRenderParams.opacity == 0;
        if (D3DRenderParams.opacity > 0)
        {
            D3DRenderParams.opacity = (int)(D3DRenderParams.opacity - (unsigned int)fadeStep) < 0 ? 0 : D3DRenderParams.opacity - (unsigned int)fadeStep;
            fadeDone = D3DRenderParams.opacity == 0;
        }
        if (fadeDone)
            PC_AE_MASTER &= ~0x40000u;
    }
    if ((PC_AE_MASTER & 0x10) != 0 && motionBlurFactor != 0.0)
    {
        swap = spare;
        D3D_MotionBlur(current, spare, motionBlurFactor);
        spare = current;
        current = swap;
    }
    if ((PC_AE_MASTER & 0x80u) != 0)
    {
        AE_MS_CurrVec.x = 1.0;
        AE_MS_CurrVec.y = 1.0;
        AE_MS_CurrVec.z = 1.0;
        MATH_TransformVector(&AE_MS_CurrVec, &gpst_DX_stDD->ct.cam.cameraMtx, &AE_MS_CurrVec);
        dx = AE_MS_CurrVec.x - AE_MS_PrevVec.x;
        if (sqrt(
            dx * dx
            + (AE_MS_CurrVec.z - AE_MS_PrevVec.z) * (AE_MS_CurrVec.z - AE_MS_PrevVec.z)
            + (AE_MS_CurrVec.y - AE_MS_PrevVec.y) * (AE_MS_CurrVec.y - AE_MS_PrevVec.y)) > 0.0049999999)
        {
            swap = spare;
            motionY = (float)((AE_MS_CurrVec.y - AE_MS_PrevVec.y) * 50.0);
            motionX = (float)(dx * 50.0);
            D3D_MotionSmooth(spare, current, current, spare, motionX, motionY);
            spare = current;
            current = swap;
        }
        AE_MS_PrevVec = AE_MS_CurrVec;
    }
    if ((PC_AE_MASTER & 0x400) != 0 && zoomSmoothFactor != 0.0)
    {
        swap = spare;
        D3D_ZoomSmooth(current, spare, zoomSmoothFactor);
        spare = current;
        current = swap;
    }
    if ((PC_AE_MASTER & 0x20) != 0 && blurFactor != 0.0)
    {
        swap = spare;
        D3D_Blur(current, spare, blurFactor);
        spare = current;
        current = swap;
    }
    if ((PC_AE_MASTER & 0x8000) != 0 && zoomSmoothFactor2 != 0.0)
    {
        D3D_ZoomSmooth2(current, spare, zoomSmoothFactor2);
        current = spare;
    }
    if (unkAEFunc2Param != 0.5) {
        D3D_Unkown_AE_Func_2(D3DRenderParams.tmpRenderTarget, current, unkAEFunc2Param);
        CopyRenderTargetTexture(D3DRenderParams.d3dDev, D3DRenderParams.tmpRenderTarget, current);
    }
    if (colorBalanceParam1 != 0.5) {
        D3D_ColorBalance(D3DRenderParams.tmpRenderTarget, current, colorBalanceParam1);
        CopyRenderTargetTexture(D3DRenderParams.d3dDev, D3DRenderParams.tmpRenderTarget, current);
    }
    if ((PC_AE_MASTER & 1) != 0) {
        D3D_Unkown_AE_Func_Flag_1(D3DRenderParams.tmpRenderTarget, current);
        CopyRenderTargetTexture(D3DRenderParams.d3dDev, D3DRenderParams.tmpRenderTarget, current);
    }
    D3D_AE_CopyToScreen(current);
    D3D_SetD3DStatesDefaultForAE();
    D3DRenderParams.d3d_defaultZEnable = 1;
}

void PC_AE_MASTER_CTRL(int effect, int onOff, int slot, float value)
{
    int toggled;
    int changed;
    unsigned int cleared;

    switch (effect)
    {
    case 0:
        toggled = (onOff != 0) != ((PC_AE_MASTER & 8) != 0);
        changed = toggled;
        if (slot == 0)
            changed = depthBlurType0 != value || toggled;
        else if (slot == 1)
            changed = depthBlurType1 != value || toggled;
        else if (slot == 2)
            changed = depthBlurType2 != value || toggled;
        if (!changed)
            return;
        cleared = PC_AE_MASTER & ~8u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 8;
        if (slot == 0)
            depthBlurType0 = value;
        else if (slot == 1)
            depthBlurType1 = value;
        else if (slot == 2)
            depthBlurType2 = value;
        D3D_UpdateBlurTexture();
        return;
    case 1:
        cleared = PC_AE_MASTER & ~0x10u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x10;
        motionBlurFactor = value;
        return;
    case 2:
        cleared = PC_AE_MASTER & ~0x20u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x20;
        blurFactor = value;
        return;
    case 4:                                     // motionsmooth
        cleared = PC_AE_MASTER & ~0x80u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x80;
        return;
    case 5:
        cleared = PC_AE_MASTER & ~0x400u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x400;
        zoomSmoothFactor = value;
        return;
    case 6:
        cleared = PC_AE_MASTER & ~0x8000u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x8000;
        zoomSmoothFactor2 = value;
        return;
    case 7:
        cleared = PC_AE_MASTER & ~0x800u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x800;
        remenanceFactor = value;
        return;
    case 8:
        unkAEFunc2Param = value;
        return;
    case 9:
        colorBalanceParam1 = value;
        return;
    case 11:
        cleared = PC_AE_MASTER & ~1u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 1;
        blackAndWhiteParam1 = value;
        return;
    case 12:
        if (slot == 0)
            unknownAEFlag4GlobalVar = value;
        else if (slot == 1)
            texFactorManual = value;
        else if (slot == 2)
            texFactorProbablyUnunsedType2 = value;
        cleared = PC_AE_MASTER & ~4u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 4;
        return;
    case 14:
        toggled = (onOff != 0) != ((PC_AE_MASTER & 0x1000) != 0);
        if (depthBlurNear1 != value || toggled)
        {
            cleared = PC_AE_MASTER & ~0x1000u;
            PC_AE_MASTER = cleared;
            if (onOff)
                PC_AE_MASTER = cleared | 0x1000;
            depthBlurNear1 = value;
            D3D_UpdateBlurTexture();
        }
        return;
    case 15:
        cleared = PC_AE_MASTER & ~0x40000u;
        PC_AE_MASTER = cleared;
        if (onOff)
        {
            PC_AE_MASTER = cleared | 0x40000;
            if (!D3DRenderParams.opacity)
                D3DRenderParams.opacity = 255;
        }
        gAEOpacity = value;
        return;
    default:
        return;
    }
}

int PC_AE_MASTER_GET_ONOFF(int effect)
{
    int result;

    switch (effect)
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

float PC_AE_MASTER_GET_P1(int effect, int slot)
{
    double result;

    switch (effect)
    {
    case 0:
        if (slot == 0)                          // 17 warp
            result = depthBlurType0;
        else if (slot == 1)
            result = depthBlurType1;
        else if (slot == 2)
            result = depthBlurType2;
        else
            // preserved: every other getter's unknown slot gives zero; this one gives the motion blur
            result = motionBlurFactor;
        break;
    case 1:
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
        if (slot == 0)
            result = unknownAEFlag4GlobalVar;
        else if (slot == 1)
            result = texFactorManual;
        else if (slot == 2)
            result = texFactorProbablyUnunsedType2;
        else
            result = 0.0;
        break;
    case 14:
        result = depthBlurNear1;
        break;
    default:
        result = 0.0;
        break;
    }
    return (float)result;
}