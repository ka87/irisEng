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
    // preserved: the first pass takes its vertical offset from the unnarrowed
    // product, every later one from the narrowed step
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
    quad[2].v1.x = 639.5;
    quad[2].v1.z = 0.0;
    quad[2].hw = 1.0;
    quad[2].uvSet1.u = 640.0;
    quad[2].uvSet2.u = 640.0;
    quad[3].v1.x = 639.5;
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
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, 0);
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
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, 0);
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
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, i, 0);
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
    double uSrcLeft; // st7
    double uSrcRight; // st6
    double vScale; // st5
    int dstTop; // ecx
    double srcBottom; // st4
    double halfOverscan; // st5
    int i;
    float uScale; // [esp+21Ch] [ebp-E4h]
    float vSrcBottom; // [esp+21Ch] [ebp-E4h]
    float vSet4Bottom; // [esp+21Ch] [ebp-E4h]
    float vSrcTop; // [esp+220h] [ebp-E0h]
    float yExpand; // [esp+224h] [ebp-DCh]
    int dstWidth; // [esp+228h] [ebp-D8h]
    float xExpand; // [esp+228h] [ebp-D8h]
    int dstLeft; // [esp+22Ch] [ebp-D4h]
    float vSet2Bottom; // [esp+22Ch] [ebp-D4h]
    int dstRight; // [esp+230h] [ebp-D0h]
    float rightX; // [esp+230h] [ebp-D0h]
    int dstBottom; // [esp+234h] [ebp-CCh]
    float vSet1Bottom; // [esp+234h] [ebp-CCh]
    float vSet3Bottom; // [esp+238h] [ebp-C8h]
    float topY; // [esp+23Ch] [ebp-C4h]
    tVertex_type_404 quad[4]; // [esp+240h] [ebp-C0h] BYREF

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
    float width; // [esp+1ECh] [ebp-8Ch]
    float uSet1Right; // [esp+1ECh] [ebp-8Ch]
    float vSet2Top; // [esp+1ECh] [ebp-8Ch]
    float uSet2Right; // [esp+1F0h] [ebp-88h]
    float vSet1Top; // [esp+1F0h] [ebp-88h]
    float height; // [esp+1F4h] [ebp-84h]
    float topY; // [esp+1F4h] [ebp-84h]
    tVertex_type_204 quad[4]; // [esp+1F8h] [ebp-80h] BYREF

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

void __cdecl D3D_WaterBlur(IDirect3DTexture9* texture, tUnkWaterBlurStruct* src, IDirect3DTexture9* renderTarget, tUnkWaterBlurStruct* dst, float smoothX, float smoothY, float overscan)
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
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    v4 = 1;
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a1);
    if (v32 >= 3)
    {
        v4 = 2;
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a1);
        if (v32 >= 4)
        {
            v4 = 3;
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
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
    AE_SetStageState(v5, &textureStageTypeConfig[v5].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
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
                AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
                AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
                AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
                AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
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
            AE_SetStageState(v22, &textureStageTypeConfig[v23].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
            AE_SetStageState(v22, &textureStageTypeConfig[v23].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v22++, NULL);
            ++v23;
        } while (v22 < v32);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
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
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    v4 = 1;
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a1);
    if (v41 >= 3)
    {
        v4 = 2;
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a1);
        if (v41 >= 4)
        {
            v4 = 3;
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
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
    AE_SetStageState(v4 + 1, &textureStageTypeConfig[v5].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
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
                AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
                AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
                AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
                AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
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
            AE_SetStageState(v28, &textureStageTypeConfig[v29].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
            AE_SetStageState(v28, &textureStageTypeConfig[v29].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v28++, 0);
            ++v29;
        } while (v28 < v41);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
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

    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 2, (IDirect3DBaseTexture9*)a1);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    v3 = 0;
    AE_SetStageState(3, &textureStageTypeConfig[3].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 3, (IDirect3DBaseTexture9*)a1);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(4, &textureStageTypeConfig[4].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
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
        AE_SetStageState(v7, &textureStageTypeConfig[v3].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(v7, &textureStageTypeConfig[v3].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
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
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a3);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a3);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
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
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a3);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 1, (IDirect3DBaseTexture9*)a3);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
    AE_SetStageState(2, &textureStageTypeConfig[2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, TRUE);
    AE_SetRenderState(&renderStateConfig.d3d_BlendOp, D3DRS_BLENDOP, D3DBLENDOP_ADD);
    AE_SetRenderState(&renderStateConfig.d3dSrcBlend, D3DRS_SRCBLEND, D3DBLEND_ONE);
    AE_SetRenderState(&renderStateConfig.d3dDestBlend, D3DRS_DESTBLEND, D3DBLEND_ONE);
    D3D_DrawASurface(a1a, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v8 = 0;
    for (i = 0; i < 2; ++i)
    {
        AE_SetStageState(v8, &textureStageTypeConfig[i].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(v8, &textureStageTypeConfig[i].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v8++, NULL);
    }
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
}

void D3D_MotionSmooth(IDirect3DTexture9* a1, IDirect3DTexture9* a2, IDirect3DTexture9* a3, IDirect3DTexture9* a4, float a5, float a6)
{
    D3D_SetRenderTargetFromTexture(a4);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    if (gMaxSimultaneousTextures < 2 || gMaxSimultaneousTextures > 3)
        D3D_MotionSmoothHD(a3, a5, a6);
    else
        D3D_MotionSmoothSD(a1, a2, a3, a5, a6);
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
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
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a5);
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
    D3D_DrawASurface(v17, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
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
    D3D_DrawASurface(v17, D3DFVF_TEX2 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    v5 = 0;
    v6 = 0;
    do
    {
        AE_SetSamplerState(v5, &samplerConfig[v4].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(v5, &samplerConfig[v4].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(v5, &textureStageTypeConfig[v6].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(v5, &textureStageTypeConfig[v6].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(v5, &textureStageTypeConfig[v6].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, v5);
        D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, v5, NULL);
        ++v4;
        ++v5;
        ++v6;
    } while (v4 < 2);
    AE_SetRenderState(&renderStateConfig.d3dAlphaBlendEnable, D3DRS_ALPHABLENDENABLE, FALSE);
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
    AE_SetSamplerState(0, &samplerConfig[0].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    AE_SetSamplerState(0, &samplerConfig[0].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, 0);
    D3DRenderParams.d3dDev->lpVtbl->SetTexture(D3DRenderParams.d3dDev, 0, (IDirect3DBaseTexture9*)a1);
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
        AE_SetSamplerState(v5, &samplerConfig[i].d3d_minfilter, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        AE_SetSamplerState(v5, &samplerConfig[i].d3d_magfilter, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        AE_SetStageState(v5, &textureStageTypeConfig[v2].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_DISABLE);
        AE_SetStageState(v5, &textureStageTypeConfig[v2].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        AE_SetStageState(v5, &textureStageTypeConfig[v2].d3dTexCoordIndex, D3DTSS_TEXCOORDINDEX, v5);
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
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    if (gMaxSimultaneousTextures == 2)
        D3D_RemenanceSD(a3, a5);
    else
        D3D_RemenanceHD(a3, a5);
        
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
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
    AE_SetRenderState(&renderStateConfig.d3dColorWriteEnable, D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaOp, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    AE_SetStageState(0, &textureStageTypeConfig[0].d3d_alphaArg1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorOp, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    AE_SetStageState(1, &textureStageTypeConfig[1].d3d_colorarg2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
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
    if (unkAEFunc2Param != 0.5) {
        D3D_Unkown_AE_Func_2(D3DRenderParams.tmpRenderTarget, v1, unkAEFunc2Param);
        CopyRenderTargetTexture(D3DRenderParams.d3dDev, D3DRenderParams.tmpRenderTarget, v1);
    }
    if (colorBalanceParam1 != 0.5) {
        D3D_ColorBalance(D3DRenderParams.tmpRenderTarget, v1, colorBalanceParam1);
        CopyRenderTargetTexture(D3DRenderParams.d3dDev, D3DRenderParams.tmpRenderTarget, v1);
    }
    if ((PC_AE_MASTER & 1) != 0) {
        D3D_Unkown_AE_Func_Flag_1(D3DRenderParams.tmpRenderTarget, v1);
        CopyRenderTargetTexture(D3DRenderParams.d3dDev, D3DRenderParams.tmpRenderTarget, v1);
    }
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