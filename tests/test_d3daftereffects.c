/* Differential test for platform/d3daftereffects.c.
 *
 * The frozen pre-rewrite copy and the rewrite go into one binary with
 * their file-scope names prefixed BASE_ and NEW_. Each case runs over
 * identical shared state -- the cached render-state structs,
 * D3DRenderParams, PC_AE_MASTER -- and the two sides are compared on
 * the ordered sequence of Direct3D calls, with the vertex buffer of
 * every DrawPrimitiveUP dumped word by word, and on a snapshot of
 * every global the file owns.
 *
 * The real math.c is linked rather than mocked, so MATH_TransformVector
 * is the engine's own; MATH_InitModule is called before the first case.
 *
 * tests/d3daftereffects_sdk.h is forced into both sides so the D3D constants
 * carry the values the shipping SDK gives them -- see the comment at
 * the top of that file for why.
 */

#include "d3daftereffects_mocks.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

void MATH_InitModule(void);

extern int tweak_ae_no;
extern int tweak_ae_nobands;
extern int ECAM_gi_NoAfterEffects;

#define AE_GROUPS           14
#define AE_CASES_PER_GROUP  48
#define AE_CASES            (AE_GROUPS * AE_CASES_PER_GROUP)

/* ---- declarations of both sides ---------------------------------- */

/* The cutscene switch, and the one function that sets it, are in
   cinebands.c now: a script asks for cutscene mode, the switch says
   whether it may have it, and neither is a renderer's business. The
   rewrite's side of them is that module, which this harness compiles
   as a second unit of this side.

   The frozen copy still spells the function the way it was spelled
   when it lived in the after-effects, so the table's name for it is
   mapped onto that for the frozen side alone. */
#define BASE_CIN_CutsceneMode BASE_PC_CIN_CutsceneMode

#define AE_DECLS(P) \
    extern float P##_depthBlurNear1, P##_depthBlurType0, P##_depthBlurType1, \
                 P##_depthBlurType2, P##_bandsVal1, P##_bandsVal2, \
                 P##_bandsVal3, P##_bandsVal4, P##_gAEOpacity, \
                 P##_colorBalanceParam1, P##_brightnessParam, \
                 P##_unknownAEFlag4GlobalVar, P##_texFactorManual, \
                 P##_remenanceFactor, P##_motionBlurFactor, \
                 P##_zoomSmoothFactor, P##_blurFactor, P##_zoomSmoothFactor2, \
                 P##_texFactorProbablyUnunsedType2, P##_blackAndWhiteParam1; \
    extern int P##_CIN_Bands, P##_CIN_NoBands; \
    void  P##_CIN_CutsceneMode(int on); \
    void  P##_D3D_SaveAEParams(void); \
    void  P##_D3D_SetGlobalBlurParams(void); \
    float P##_D3D_DepthBlurBlurFuncCalc1(int step); \
    uint32_t P##_D3D_DepthBlurBlurFuncCalc2(float depth); \
    void  P##_D3D_DepthBlurBlurFunc(int* table); \
    int   P##_D3D_UpdateBlurTexture(void); \
    void  P##_D3D_CreateBlurTextures(void); \
    void  P##_D3D_CreateTextureAndBlankIt(IDirect3DTexture9** out); \
    void  P##_D3D_AETexturesInit(void); \
    void  P##_D3D_SetD3DStatesDefaultForAE(void); \
    void  P##_D3D_DrawASurface(void* verts, unsigned int fvf); \
    void  P##_D3D_DepthBlurSD(IDirect3DTexture9* t, float u, float v); \
    void  P##_D3D_DepthBlurHD(IDirect3DTexture9* t, float u, float v); \
    void  P##_D3D_SetRenderTargetFromTexture(IDirect3DTexture9* t); \
    void  P##_D3D_DepthBlur(IDirect3DTexture9* t, IDirect3DTexture9* rt, float u, float v); \
    void  P##_D3D_Blur(IDirect3DTexture9* t, IDirect3DTexture9* rt, float f); \
    void  P##_D3D_Unknown_AE_Func1(IDirect3DTexture9* t); \
    float P##_D3D_CalcTextureFactor(float f); \
    void  P##_D3D_Unkown_AE_Func_Flag_4(IDirect3DTexture9* s, IDirect3DTexture9* d, \
                                        float a, float b); \
    void  P##_D3D_Fade(IDirect3DTexture9* s, IDirect3DTexture9* d, int opacity); \
    void  P##_D3D_StretchRect(IDirect3DTexture9* t, RECT* r); \
    void  P##_D3D_MotionBlur(IDirect3DTexture9* s, IDirect3DTexture9* d, float f); \
    void  P##_D3D_AE_CopyToScreen(IDirect3DTexture9* t); \
    void  P##_D3D_Grayscale(IDirect3DTexture9* d, IDirect3DTexture9* s); \
    void  P##_D3D_Brightness(IDirect3DTexture9* d, IDirect3DTexture9* s, float f); \
    void  P##_D3D_ColorBalanceLessThanZeroPointHalf(IDirect3DTexture9* d, \
                                                    IDirect3DTexture9* s, float f); \
    void  P##_D3D_ColorBalanceMoreThanZeroPointHalfSD(int a, int b, IDirect3DTexture9* s); \
    void  P##_D3D_ColorBalanceMoreThanZeroPointHalfHD(int a, IDirect3DTexture9* s, int b); \
    void  P##_D3D_ColorBalanceMoreThanZeroPointHalf(IDirect3DTexture9* d, \
                                                    IDirect3DTexture9* s, float f); \
    void  P##_D3D_ColorBalance(IDirect3DTexture9* d, IDirect3DTexture9* s, float f); \
    void  P##_D3D_WaterBlurHD(tUnkWaterBlurStruct* a, IDirect3DTexture9* t, \
                              tUnkWaterBlurStruct* b, float c, float e, float g); \
    void  P##_D3D_WaterBlurSD(IDirect3DTexture9* t, tUnkWaterBlurStruct* a, \
                              tUnkWaterBlurStruct* b, float c, float e); \
    void  P##_D3D_WaterBlur(IDirect3DTexture9* t, tUnkWaterBlurStruct* a, \
                            IDirect3DTexture9* t2, tUnkWaterBlurStruct* b, \
                            float c, float e, float g); \
    void  P##_D3D_DoBlurWaterTexture(void); \
    void  P##_D3D_ZoomSmooth(IDirect3DTexture9* s, IDirect3DTexture9* d, float f); \
    void  P##_D3D_ZoomSmooth2(IDirect3DTexture9* s, IDirect3DTexture9* d, float f); \
    void  P##_D3D_MotionSmoothHD(IDirect3DTexture9* t, float a, float b); \
    void  P##_D3D_MotionSmoothSD(IDirect3DTexture9* a, IDirect3DTexture9* b, \
                                 IDirect3DTexture9* c, float d, float e); \
    void  P##_D3D_MotionSmooth(IDirect3DTexture9* a, IDirect3DTexture9* b, \
                               IDirect3DTexture9* c, IDirect3DTexture9* d, \
                               float e, float f); \
    void  P##_D3D_RemenanceSD(IDirect3DTexture9* t, float f); \
    void  P##_D3D_RemenanceHD(IDirect3DTexture9* t, float f); \
    void  P##_D3D_Remenance(IDirect3DTexture9* a, IDirect3DTexture9* b, float f); \
    void  P##_D3D_Unknown_AE_FuncDepthBlur(IDirect3DTexture9* t); \
    HRESULT P##_CopyRenderTargetTexture(LPDIRECT3DDEVICE9 dev, IDirect3DTexture9* s, \
                                        IDirect3DTexture9* d); \
    void  P##_D3D_PC_AE(tWorld* world); \
    void  P##_PC_AE_MASTER_CTRL(int effect, int onOff, int slot, float value); \
    int   P##_PC_AE_MASTER_GET_ONOFF(int effect); \
    float P##_PC_AE_MASTER_GET_P1(int effect, int slot)

AE_DECLS(BASE);
AE_DECLS(NEW);

/* ---- helpers the scenarios use ----------------------------------- */

static unsigned int AE_Bits(float f)
{
    unsigned int u;
    memcpy(&u, &f, 4);
    return u;
}

static unsigned int AE_Rand(unsigned int* s)
{
    *s = *s * 1103515245u + 12345u;
    return (*s >> 16) & 0x7FFF;
}

static float AE_Factor(int k, int which)
{
    static const float table[8] = {
        0.0f, 0.125f, 0.33333334f, 0.5f, 0.66666669f, 0.875f, 1.0f, 1.5f
    };
    return table[(k * 3 + which * 5) & 7];
}

static int AE_Stages(int k)
{
    static const int table[4] = { 1, 2, 3, 8 };
    return table[(k >> 2) & 3];
}

static float AE_NearPlane(int k)
{
    static const float table[4] = { 0.0f, 1.1754944e-38f, 0.1f, 2.5f };
    return table[(k >> 4) & 3];
}

/* includes a band past the edge of the back buffer, so the two clamps
   in D3D_AE_CopyToScreen are both driven */
static float AE_BandTop(int k)
{
    static const float table[4] = { 0.0f, -0.05f, 0.125f, 0.25f };
    return table[(k >> 4) & 3];
}

static float AE_BandBottom(int k)
{
    static const float table[4] = { 1.0f, 1.2f, 0.875f, 0.75f };
    return table[(k >> 4) & 3];
}

/* The fade's duration, in the seconds a script asks for. Twelve
   values, so that k % 12 pairs each one with both starting opacities
   twice over the group's 48 cases -- counted, not assumed.

   The four corners of the step arithmetic are entries 0 and 1 (a span
   of nought, and one so small the step will not fit in a whole
   number), 9 and 10 (the clamp at one, at its boundary and past it)
   and 11 (a negative span, which runs the fade backwards). The rest
   are ordinary durations whose steps do and do not divide the range
   exactly. */
static float AE_FadeSpan(int k)
{
    static const float table[12] = {
        0.0f, 1.0e-30f, 0.03f, 0.09f, 0.25f, 0.5f,
        1.0f, 2.0f, 4.0f, 4.25f, 8.0f, -1.0f
    };
    return table[k % 12];
}

/* The hue sweep's depth and phase, driven rather than seeded.
 *
 * Eight depths and six phases, so k % 8 and (k / 8) % 6 pair every
 * depth with every phase exactly once over the group's 48 cases --
 * counted, not hoped for.
 *
 * The depths above one are the point: that is where a channel's amount
 * passes the byte it is packed into. The phases are spread across the
 * wave's flat top, its ramp and its nothing, and two of them are
 * outside the cycle -- where the single wrap does not bring them back
 * in and the three channels disagree about what that means. */
static float AE_HueDepth(int k)
{
    static const float table[8] = {
        0.0f, 0.25f, 0.5f, 1.0f, 1.0009f, 1.5f, 2.0f, 3.0f
    };
    return table[k % 8];
}

static float AE_HuePhase(int k)
{
    static const float table[6] = {
        0.0f, 0.2f, 0.3f, 0.5f, 1.5f, -0.25f
    };
    return table[(k / 8) % 6];
}

/* How far the watched point moves between two frames, for the motion
   smooth's group.
 *
   Eight amounts, straddling the threshold on both sides and at it:
   the test is strictly greater than, so the amount equal to the
   threshold must *not* smear and the one a hair above must. The two
   large ones are there because the smear they ask for is far wider
   than the picture, which is a perfectly ordinary thing for a camera
   cut to produce. */
static float AE_MotionDelta(int k)
{
    static const float table[8] = {
        0.0f, 0.001f, 0.0049f, 0.0049999999f, 0.005f, 0.0050000001f,
        0.02f, 1.0f
    };
    return table[k % 8];
}

static int AE_MasterBits(int k)
{
    static const int bits[11] = {
        1, 4, 8, 0x10, 0x20, 0x80, 0x400, 0x800, 0x1000, 0x8000, 0x40000
    };
    int i, m = 0;

    for (i = 0; i < 11; i++)
        if ((k >> (i % 6)) & 1)
            m |= bits[i];
    if (k % 7 == 0)
        m = 0x7FFFF;
    return m;
}

/* Seed the cached render-state mirrors. Three modes, so both arms of
   every "set it only if it changed" guard are driven: a spread of
   plausible values, all-zero, and the values the effects themselves
   leave behind. */
static int AE_seedModeSeen[3];

static void AE_SeedCaches(unsigned int* s)
{
    int mode = (int)(AE_Rand(s) % 3);
    int i;

    AE_seedModeSeen[mode]++;

    memset(&renderStateConfig, 0, sizeof(renderStateConfig));
    memset(textureStageTypeConfig, 0, sizeof(textureStageTypeConfig));
    memset(samplerConfig, 0, sizeof(samplerConfig));
    if (mode == 1)
        return;
    if (mode == 2)
    {
        for (i = 0; i < 8; i++)
        {
            textureStageTypeConfig[i].d3d_colorOp = D3DTOP_DISABLE;
            textureStageTypeConfig[i].d3d_alphaOp = D3DTOP_DISABLE;
            textureStageTypeConfig[i].d3d_colorarg1 = D3DTA_TEXTURE;
            textureStageTypeConfig[i].d3d_colorarg2 = D3DTA_CURRENT;
            textureStageTypeConfig[i].d3d_alphaArg1 = D3DTA_TFACTOR;
            textureStageTypeConfig[i].d3dTexCoordIndex = i;
            samplerConfig[i].d3d_minfilter = D3DTEXF_POINT;
            samplerConfig[i].d3d_magfilter = D3DTEXF_POINT;
            samplerConfig[i].d3d_addressu = D3DTADDRESS_CLAMP;
            samplerConfig[i].d3d_addressv = D3DTADDRESS_CLAMP;
        }
        renderStateConfig.d3dColorWriteEnable =
            D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED |
            D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE;
        renderStateConfig.d3dAlphaBlendEnable = FALSE;
        renderStateConfig.d3dSrcBlend = D3DBLEND_ONE;
        renderStateConfig.d3dDestBlend = D3DBLEND_ZERO;
        renderStateConfig.d3d_BlendOp = D3DBLENDOP_ADD;
        renderStateConfig.d3dTextureFactor = 0x80FFFFFF;
        renderStateConfig.d3d_zFunc = D3DCMP_ALWAYS;
        renderStateConfig.d3dAlphaFunc = D3DCMP_ALWAYS;
        renderStateConfig.d3dFillMode = D3DFILL_SOLID;
        renderStateConfig.d3dCullMode = D3DCULL_NONE;
        return;
    }
    for (i = 0; i < 8; i++)
    {
        textureStageTypeConfig[i].d3d_colorOp = 1 + (int)(AE_Rand(s) % 26);
        textureStageTypeConfig[i].d3d_alphaOp = 1 + (int)(AE_Rand(s) % 26);
        textureStageTypeConfig[i].d3d_colorarg1 = (int)(AE_Rand(s) % 5);
        textureStageTypeConfig[i].d3d_colorarg2 = (int)(AE_Rand(s) % 5);
        textureStageTypeConfig[i].d3d_alphaArg1 = (int)(AE_Rand(s) % 5);
        textureStageTypeConfig[i].d3dTexCoordIndex = (int)(AE_Rand(s) % 8);
        textureStageTypeConfig[i].d3dTextureTransformFlags = (int)(AE_Rand(s) % 4);
        samplerConfig[i].d3d_minfilter = (int)(AE_Rand(s) % 3);
        samplerConfig[i].d3d_magfilter = (int)(AE_Rand(s) % 3);
        samplerConfig[i].d3d_addressu = 1 + (int)(AE_Rand(s) % 3);
        samplerConfig[i].d3d_addressv = 1 + (int)(AE_Rand(s) % 3);
    }
    renderStateConfig.d3d_zEnable = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3d_zFunc = (int)(AE_Rand(s) % 9);
    renderStateConfig.d3d_zWriteEnable = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3dFogEnable = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3dFillMode = 2 + (int)(AE_Rand(s) % 2);
    renderStateConfig.d3dCullMode = 1 + (int)(AE_Rand(s) % 3);
    renderStateConfig.d3dColorWriteEnable = (int)(AE_Rand(s) % 16);
    renderStateConfig.d3dAlphaTest = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3dAlphaFunc = (int)(AE_Rand(s) % 9);
    renderStateConfig.d3dAlphaBlendEnable = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3d_BlendOp = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3dSrcBlend = 1 + (int)(AE_Rand(s) % 8);
    renderStateConfig.d3dDestBlend = 1 + (int)(AE_Rand(s) % 8);
    renderStateConfig.d3d_Lighting = (int)(AE_Rand(s) % 2);
    renderStateConfig.d3dTextureFactor = (int)AE_Rand(s);
}

/* ---- the two sides ----------------------------------------------- */

#define SIDE BASE
#include "d3daftereffects_cases.inc"
#undef SIDE

#define SIDE NEW
#include "d3daftereffects_cases.inc"
#undef SIDE

/* ---- shared state, identical before each side runs ---------------- */

static void AE_ResetShared(void)
{
    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    memset(&renderStateConfig, 0, sizeof(renderStateConfig));
    memset(textureStageTypeConfig, 0, sizeof(textureStageTypeConfig));
    memset(samplerConfig, 0, sizeof(samplerConfig));
    memset(AEMOCK_ramp, 0, sizeof(AEMOCK_ramp));
    AEMOCK_Init();
    memset(gpst_DX_stDD, 0, sizeof(*gpst_DX_stDD));
    D3DRenderParams.presentParams.BackBufferWidth = 640;
    D3DRenderParams.presentParams.BackBufferHeight = 480;
    D3DRenderParams.presentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
    D3DRenderParams.d3dcaps.MaxSimultaneousTextures = 4;
    D3DRenderParams.blurTex = AEMOCK_tex[0];
    D3DRenderParams.blurRenderTarget = AEMOCK_tex[1];
    D3DRenderParams.blurRenderTarget2 = AEMOCK_tex[2];
    D3DRenderParams.unktex1_dword_B31988 = AEMOCK_tex[3];
    D3DRenderParams.unktex2_dword_B3198C = AEMOCK_tex[4];
    D3DRenderParams.tmpRenderTarget = AEMOCK_tex[5];
    D3DRenderParams.pSourceTexture = AEMOCK_tex[9];
    D3DRenderParams.pDestTexture = AEMOCK_tex[10];
    D3DRenderParams.opacity = 255;
    waterTexture = AEMOCK_tex[8];
    gpst_DX_stDD->ct.cam.nearPlane = 0.1f;
    gpst_DX_stDD->ct.cam.farPlane = 1000.0f;
    gpst_DX_stDD->ct.cam.cameraMtx.v1.x = 1.0f;
    gpst_DX_stDD->ct.cam.cameraMtx.v2.y = 1.0f;
    gpst_DX_stDD->ct.cam.cameraMtx.v3.z = 1.0f;
    gpst_DX_stDD->ct.cam.cameraMtx._m14 = 1.0f;
    gpst_DX_stDD->ct.cam.cameraMtx._m24 = 1.0f;
    gpst_DX_stDD->ct.cam.cameraMtx._m34 = 1.0f;
    gpst_DX_stDD->ct.cam.cameraMtx.w = 1.0f;
    gMaxSimultaneousTextures = 4;
    ENG_ifSingleStreamThenEq2Otherwise8 = 8;
    tweak_ae_no = 0;
    tweak_ae_nobands = 0;
    ECAM_gi_NoAfterEffects = 0;
    PC_AE_MASTER = 0;
    depthblur_Type = 0;
    camUnkf1Save = 0.0f;
    deptBlurNear1Save = 0.0f;
    deptBlurType0Save = 0.0f;
    deptBlur1TypeSave = 0.0f;
    depthBlur1TypeOrcamUnkf2Save = 0.0f;
    AE_MS_CurrVec.x = AE_MS_CurrVec.y = AE_MS_CurrVec.z = 0.0f;
    AE_MS_PrevVec.x = AE_MS_PrevVec.y = AE_MS_PrevVec.z = 0.0f;
}

/* ---- comparison --------------------------------------------------- */

static tAeMockCall baseLog[AEMOCK_LOG_MAX];
static int baseLen;

static int IsSetCall(const tAeMockCall* c)
{
    return strcmp(c->fn, "SetRenderState") == 0 ||
           strcmp(c->fn, "SetTextureStageState") == 0 ||
           strcmp(c->fn, "SetSamplerState") == 0;
}

/* The free camera's after-effects flag exists only on the rewrite's
   side, so it is driven directly rather than compared. The two things
   it has to do are opposite in kind: skip the effects, and leave the
   state the game reads alone.
   
   That second one is the point of the flag being separate from the
   ae_no tweak. The tweak clears PC_AE_MASTER for good, which is what
   a tweak is for; the camera must not, because the game goes on
   reading it back through PC_AE_MASTER_GET_ONOFF and would be told
   its effects had been turned off behind its back. */
static int editorCameraSuppressionCases(void)
{
    const int master = 0x8 | 0x4 | 0x10 | 0x80;   /* several effects on */
    int failures = 0;
    int plainLen;
    int suppressedLen;

    /* the pass as it runs normally */
    AE_ResetShared();
    AEMOCK_Reset();
    PC_AE_MASTER = master;
    NEW_D3D_PC_AE(NULL);
    plainLen = AEMOCK_logLen;
    if (PC_AE_MASTER != master)
    {
        printf("FAIL: ae: the pass changed PC_AE_MASTER with nothing set"
               " (%d, wanted %d)\n", PC_AE_MASTER, master);
        failures++;
    }

    /* with the camera's flag: fewer device writes, and the state the
       game reads comes back exactly as it was */
    AE_ResetShared();
    AEMOCK_Reset();
    PC_AE_MASTER = master;
    ECAM_gi_NoAfterEffects = 1;
    NEW_D3D_PC_AE(NULL);
    suppressedLen = AEMOCK_logLen;
    if (PC_AE_MASTER != master)
    {
        printf("FAIL: ae: the free camera left PC_AE_MASTER at %d,"
               " wanted %d\n", PC_AE_MASTER, master);
        failures++;
    }
    if (suppressedLen >= plainLen)
    {
        printf("FAIL: ae: the free camera drew as much as ever"
               " (%d writes against %d)\n", suppressedLen, plainLen);
        failures++;
    }
    ECAM_gi_NoAfterEffects = 0;

    /* the tweak on its own keeps the behaviour it always had: the
       state is cleared and stays cleared */
    AE_ResetShared();
    AEMOCK_Reset();
    PC_AE_MASTER = master;
    tweak_ae_no = 1;
    NEW_D3D_PC_AE(NULL);
    if (PC_AE_MASTER != 0)
    {
        printf("FAIL: ae: the tweak no longer clears PC_AE_MASTER (%d)\n",
               PC_AE_MASTER);
        failures++;
    }

    /* and with both, the tweak decides, because it is the one that
       means it for good */
    AE_ResetShared();
    AEMOCK_Reset();
    PC_AE_MASTER = master;
    tweak_ae_no = 1;
    ECAM_gi_NoAfterEffects = 1;
    NEW_D3D_PC_AE(NULL);
    if (PC_AE_MASTER != 0)
    {
        printf("FAIL: ae: the camera put back what the tweak cleared (%d)\n",
               PC_AE_MASTER);
        failures++;
    }
    tweak_ae_no = 0;
    ECAM_gi_NoAfterEffects = 0;

    printf("  free camera: %d device writes suppressed to %d\n",
           plainLen, suppressedLen);
    return failures;
}

/* ---------------------------- the after-effects with no device
 *
 * The engine calls two of these directly rather than through the
 * interface in tGdi_DisplayData: the per-frame pass from the world's
 * render, and the blur table whenever an AI script changes a
 * depth-blur setting. Both therefore run whichever renderer is in
 * force, so under another renderer they run here with no device, and
 * both have to do nothing rather than reach through a null pointer.
 *
 * The per-frame one is the one that matters: a renderer that is not
 * this one could not survive a frame without it.
 */
static int NoDeviceCase(void)
{
    IDirect3DDevice9* saved = D3DRenderParams.d3dDev;
    int bad = 0;

    AE_ResetShared();
    AEMOCK_Reset();
    NEW_ResetSide();

    /* As this renderer is when another one was chosen. */
    D3DRenderParams.d3dDev = NULL;
    AEMOCK_logLen = 0;

    NEW_D3D_PC_AE(NULL);
    NEW_D3D_SetD3DStatesDefaultForAE();
    NEW_D3D_UpdateBlurTexture();

    /* The AI scripts' way in, which is what reaches the blur table.
       Effect nought is the depth blur, and the map manager drives it
       in ordinary play. */
    NEW_PC_AE_MASTER_CTRL(0, 1, 0, 0.25f);
    NEW_PC_AE_MASTER_CTRL(0, 1, 0, 0.75f);

    if (AEMOCK_logLen != 0)
    {
        int k;

        printf("FAIL no device: %d call(s) made with no device\n",
               AEMOCK_logLen);
        for (k = 0; k < AEMOCK_logLen && k < 8; k++)
            printf("       %s\n", AEMOCK_log[k].fn);
        bad = 1;
    }
    else
    {
        printf("  the after-effects touch nothing with no device\n");
    }

    D3DRenderParams.d3dDev = saved;
    return bad;
}

int main(void)
{
    int caseId, i, j;
    int failures = 0;
    long long totalEntries = 0;
    long long totalSetCalls = 0;
    long long drawCalls = 0;
    long long rightEdgeFixed = 0;
    int groupSet[AE_GROUPS];
    int groupMinSet[AE_GROUPS];
    int groupMaxSet[AE_GROUPS];
    long long groupEntries[AE_GROUPS];
    int blurTypeSeen[5];
    int stagesSeen[4];
    static const int stageValues[4] = { 1, 2, 3, 8 };
    int defaultsSecondPassSets = -1;

    MATH_InitModule();
    for (i = 0; i < AE_GROUPS; i++)
    {
        groupSet[i] = 0;
        groupMinSet[i] = 1 << 30;
        groupMaxSet[i] = -1;
        groupEntries[i] = 0;
    }
    memset(blurTypeSeen, 0, sizeof(blurTypeSeen));
    memset(stagesSeen, 0, sizeof(stagesSeen));

    for (caseId = 0; caseId < AE_CASES; caseId++)
    {
        int group = caseId / AE_CASES_PER_GROUP;
        int setCalls = 0;
        int mark1 = -1, mark2 = -1;

        AE_ResetShared();
        AEMOCK_Reset();
        BASE_ResetSide();
        BASE_Run(caseId);
        if (AEMOCK_overflow)
        {
            printf("FAIL case %d: call log overflowed\n", caseId);
            return 1;
        }
        baseLen = AEMOCK_logLen;
        memcpy(baseLog, AEMOCK_log, (size_t)baseLen * sizeof(tAeMockCall));

        AE_ResetShared();
        AEMOCK_Reset();
        NEW_ResetSide();
        NEW_Run(caseId);
        if (AEMOCK_overflow)
        {
            printf("FAIL case %d: call log overflowed on the rewrite\n", caseId);
            return 1;
        }

        for (i = 0; i < baseLen; i++)
        {
            if (IsSetCall(&baseLog[i]))
                setCalls++;
            if (strcmp(baseLog[i].fn, "DrawPrimitiveUP") == 0)
                drawCalls++;
            if (strcmp(baseLog[i].fn, "defaults.pass2") == 0)
                mark1 = i;
            if (strcmp(baseLog[i].fn, "defaults.end") == 0)
                mark2 = i;
        }
        if (mark1 >= 0 && mark2 > mark1)
        {
            int n = 0;
            for (i = mark1 + 1; i < mark2; i++)
                if (IsSetCall(&baseLog[i]))
                    n++;
            if (defaultsSecondPassSets < 0 || n > defaultsSecondPassSets)
                defaultsSecondPassSets = n;
        }
        for (i = 0; i < baseLen; i++)
            if (strcmp(baseLog[i].fn, "snap.master") == 0 &&
                baseLog[i].arg[1] >= 0 && baseLog[i].arg[1] < 5)
                blurTypeSeen[baseLog[i].arg[1]]++;
        for (i = 0; i < 4; i++)
            if (gMaxSimultaneousTextures == stageValues[i])
                stagesSeen[i]++;
        groupEntries[group] += baseLen;
        totalEntries += baseLen;
        totalSetCalls += setCalls;
        groupSet[group] += setCalls;
        if (setCalls < groupMinSet[group]) groupMinSet[group] = setCalls;
        if (setCalls > groupMaxSet[group]) groupMaxSet[group] = setCalls;

        if (AEMOCK_logLen != baseLen)
        {
            printf("FAIL case %d: %d calls from the frozen copy, %d from the rewrite\n",
                   caseId, baseLen, AEMOCK_logLen);
            for (i = 0; i < baseLen && i < AEMOCK_logLen; i++)
                if (strcmp(baseLog[i].fn, AEMOCK_log[i].fn) != 0)
                {
                    printf("       first differing entry %d: %s vs %s\n", i,
                           baseLog[i].fn, AEMOCK_log[i].fn);
                    break;
                }
            if (++failures > 8)
                return 1;
            continue;
        }
        for (i = 0; i < baseLen; i++)
        {
            int differs = strcmp(baseLog[i].fn, AEMOCK_log[i].fn) != 0 ||
                          baseLog[i].nargs != AEMOCK_log[i].nargs;

            for (j = 0; !differs && j < baseLog[i].nargs; j++)
                if (baseLog[i].arg[j] != AEMOCK_log[i].arg[j])
                {
                    /* The one expected difference: the frozen copy puts
                       the full-screen quad's right edge at 639.5 where
                       the retail binary stores 640.0, so the vertices
                       carrying that x differ by exactly that pair and by
                       nothing else. Counted rather than compared, and
                       the run fails below if the count is zero, so a
                       revert to 639.5 cannot pass quietly. */
                    if (baseLog[i].arg[j] == 0x441FE000LL &&
                        AEMOCK_log[i].arg[j] == 0x44200000LL)
                    {
                        rightEdgeFixed++;
                        continue;
                    }
                    differs = 1;
                }
            if (differs)
            {
                printf("FAIL case %d entry %d: %s(", caseId, i, baseLog[i].fn);
                for (j = 0; j < baseLog[i].nargs; j++)
                    printf("%s%lld", j ? ", " : "", baseLog[i].arg[j]);
                printf(")  vs  %s(", AEMOCK_log[i].fn);
                for (j = 0; j < AEMOCK_log[i].nargs; j++)
                    printf("%s%lld", j ? ", " : "", AEMOCK_log[i].arg[j]);
                printf(")\n");
                if (++failures > 8)
                    return 1;
                break;
            }
        }
    }

    if (rightEdgeFixed == 0)
    {
        printf("FAIL the quad right-edge correction never appeared, so this run "
               "proves nothing about it\n");
        failures++;
    }
    printf("  quad right edge 639.5 -> 640.0 %lld vertices\n", rightEdgeFixed);
    printf("d3daftereffects: %d cases, %lld logged entries compared, "
           "%lld differing\n", AE_CASES, totalEntries, (long long)failures);
    printf("  DrawPrimitiveUP calls         %lld\n", drawCalls);
    printf("  cached-state writes reaching the device %lld\n", totalSetCalls);
    printf("  per group (min..max per case):\n");
    for (i = 0; i < AE_GROUPS; i++)
        printf("    group %2d  total %6d   %d..%d\n", i, groupSet[i],
               groupMinSet[i], groupMaxSet[i]);
    printf("  the second SetD3DStatesDefaultForAE over state the first one"
           " left made %d device writes\n", defaultsSecondPassSets);
    if (defaultsSecondPassSets != 0)
    {
        printf("FAIL: the skip arm of the cached-state guards is not being"
               " exercised\n");
        failures++;
    }
    printf("  cached-state seedings:");
    for (i = 0; i < 3; i++)
        printf(" mode%d=%d", i, AE_seedModeSeen[i]);
    for (i = 0; i < 3; i++)
        if (!AE_seedModeSeen[i])
        {
            printf("\nFAIL: cached-state seeding mode %d never used\n", i);
            failures++;
        }
    printf("\n  depthblur_Type seen:");
    for (i = 0; i < 5; i++)
        printf(" %d=%d", i, blurTypeSeen[i]);
    printf("\n  stage counts seen:");
    for (i = 0; i < 4; i++)
        printf(" %d=%d", stageValues[i], stagesSeen[i]);
    printf("\n");
    for (i = 0; i < 5; i++)
        if (!blurTypeSeen[i])
        {
            printf("FAIL: depthblur_Type %d never reached\n", i);
            failures++;
        }
    for (i = 0; i < 4; i++)
        if (!stagesSeen[i])
        {
            printf("FAIL: gMaxSimultaneousTextures %d never reached\n",
                   stageValues[i]);
            failures++;
        }
    for (i = 0; i < AE_GROUPS; i++)
        if (groupEntries[i] == 0)
        {
            printf("FAIL: group %d logged nothing\n", i);
            failures++;
        }
    /* the groups that drive the cached-state guards must show both the
       arm that writes and the arm that skips, so the per-case counts
       cannot all be equal */
    for (i = 1; i <= 7; i++)
        if (groupMinSet[i] == groupMaxSet[i])
        {
            printf("FAIL: group %d made the same %d device writes in every"
                   " case, so the guards are not being varied\n",
                   i, groupMinSet[i]);
            failures++;
        }
    failures += editorCameraSuppressionCases();

    failures += NoDeviceCase();

    if (failures)
    {
        printf("FAILED\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
