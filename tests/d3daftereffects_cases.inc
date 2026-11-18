/* The scenarios, compiled once per side.
 *
 * Included twice from tests/test_d3daftereffects.c with SIDE set to BASE and
 * then to NEW, so X(name) reaches that side's copy of a function or of
 * a file-scope global. Everything the two sides share -- the cached
 * render-state structs, D3DRenderParams, PC_AE_MASTER -- is reset by
 * the driver before each side runs, so both see identical state.
 */

#define XCAT2(a, b) a##_##b
#define XCAT(a, b) XCAT2(a, b)
#define X(name) XCAT(SIDE, name)
#define RUN XCAT(SIDE, Run)

/* ---- per-side state reset ---------------------------------------- */

static void XCAT(SIDE, ResetSide)(void)
{
    X(depthBlurNear1) = 0.0f;
    X(depthBlurType0) = 0.0f;
    X(depthBlurType1) = 0.0f;
    X(depthBlurType2) = 0.0f;
    X(bandsVal1) = 0.10416666f;
    X(bandsVal2) = 0.89583331f;
    X(bandsVal3) = 0.083333336f;
    X(bandsVal4) = 0.91666669f;
    X(CIN_Bands) = 1;
    X(CIN_NoBands) = 0;
    X(gAEOpacity) = 0.0f;
    X(colorBalanceParam1) = 0.5f;
    X(brightnessParam) = 0.5f;
    X(unknownAEFlag4GlobalVar) = 0.0f;
    X(texFactorManual) = 0.0f;
    X(remenanceFactor) = 0.0f;
    X(motionBlurFactor) = 0.0f;
    X(zoomSmoothFactor) = 0.0f;
    X(blurFactor) = 0.0f;
    X(zoomSmoothFactor2) = 0.0f;
    X(texFactorProbablyUnunsedType2) = 0.0f;
    /* All of the grayscale. The frozen copy has no amount -- it is
       the whole of it or nothing -- so the comparison runs at the one
       value where the two agree; the amount's own behaviour is
       checked in tests/test_aecolor.c and the OpenGL overlay test. */
    X(blackAndWhiteParam1) = 1.0f;
}

/* ---- the snapshot that goes into the log at the end of a case ----- */

static void XCAT(SIDE, Snap)(void)
{
    int i;

    AEMOCK_Record("snap.bands", 4, (long long)AE_Bits(X(bandsVal1)),
                  (long long)AE_Bits(X(bandsVal2)), (long long)AE_Bits(X(bandsVal3)),
                  (long long)AE_Bits(X(bandsVal4)));
    AEMOCK_Record("snap.blur", 4, (long long)AE_Bits(X(depthBlurNear1)),
                  (long long)AE_Bits(X(depthBlurType0)),
                  (long long)AE_Bits(X(depthBlurType1)),
                  (long long)AE_Bits(X(depthBlurType2)));
    AEMOCK_Record("snap.param", 6, (long long)AE_Bits(X(gAEOpacity)),
                  (long long)AE_Bits(X(colorBalanceParam1)),
                  (long long)AE_Bits(X(brightnessParam)),
                  (long long)AE_Bits(X(unknownAEFlag4GlobalVar)),
                  (long long)AE_Bits(X(texFactorManual)),
                  (long long)AE_Bits(X(remenanceFactor)));
    AEMOCK_Record("snap.param2", 5, (long long)AE_Bits(X(motionBlurFactor)),
                  (long long)AE_Bits(X(zoomSmoothFactor)),
                  (long long)AE_Bits(X(blurFactor)),
                  (long long)AE_Bits(X(zoomSmoothFactor2)),
                  (long long)AE_Bits(X(blackAndWhiteParam1)));
    AEMOCK_Record("snap.cin", 2, (long long)X(CIN_Bands), (long long)X(CIN_NoBands));
    AEMOCK_Record("snap.master", 4, (long long)PC_AE_MASTER, (long long)depthblur_Type,
                  (long long)D3DRenderParams.opacity,
                  (long long)D3DRenderParams.isDepthBlurOn);
    AEMOCK_Record("snap.save", 5, (long long)AE_Bits(camUnkf1Save),
                  (long long)AE_Bits(deptBlurNear1Save),
                  (long long)AE_Bits(deptBlurType0Save),
                  (long long)AE_Bits(deptBlur1TypeSave),
                  (long long)AE_Bits(depthBlur1TypeOrcamUnkf2Save));
    /* f33 is in here because it was not, and two of the five modes are
       the only ones that set it -- so a change that dropped it was
       invisible to every one of these cases. */
    AEMOCK_Record("snap.proj", 4, (long long)D3DRenderParams.textureUVIsProjected,
                  (long long)AE_Bits(D3DRenderParams.transformationMatrix.f41),
                  (long long)AE_Bits(D3DRenderParams.transformationMatrix.f31),
                  (long long)AE_Bits(D3DRenderParams.transformationMatrix.f33));
    for (i = 0; i < AEMOCK_RAMP; i++)
        if (AEMOCK_ramp[i])
            AEMOCK_Record("snap.ramp", 2, (long long)i, (long long)AEMOCK_ramp[i]);
    for (i = 0; i < 8; i++)
        AEMOCK_Record("snap.stage", 6, (long long)textureStageTypeConfig[i].d3d_colorOp,
                      (long long)textureStageTypeConfig[i].d3d_colorarg1,
                      (long long)textureStageTypeConfig[i].d3d_colorarg2,
                      (long long)textureStageTypeConfig[i].d3d_alphaOp,
                      (long long)textureStageTypeConfig[i].d3d_alphaArg1,
                      (long long)textureStageTypeConfig[i].d3dTexCoordIndex);
    for (i = 0; i < 8; i++)
        AEMOCK_Record("snap.samp", 4, (long long)samplerConfig[i].d3d_minfilter,
                      (long long)samplerConfig[i].d3d_magfilter,
                      (long long)samplerConfig[i].d3d_addressu,
                      (long long)samplerConfig[i].d3d_addressv);
    AEMOCK_Record("snap.rs1", 6, (long long)renderStateConfig.d3d_zEnable,
                  (long long)renderStateConfig.d3d_zFunc,
                  (long long)renderStateConfig.d3d_zWriteEnable,
                  (long long)renderStateConfig.d3dFogEnable,
                  (long long)renderStateConfig.d3dFillMode,
                  (long long)renderStateConfig.d3dCullMode);
    AEMOCK_Record("snap.rs2", 6, (long long)renderStateConfig.d3dColorWriteEnable,
                  (long long)renderStateConfig.d3dAlphaTest,
                  (long long)renderStateConfig.d3dAlphaFunc,
                  (long long)renderStateConfig.d3dAlphaBlendEnable,
                  (long long)renderStateConfig.d3d_BlendOp,
                  (long long)renderStateConfig.d3d_Lighting);
    AEMOCK_Record("snap.rs3", 4, (long long)renderStateConfig.d3dSrcBlend,
                  (long long)renderStateConfig.d3dDestBlend,
                  (long long)(unsigned int)renderStateConfig.d3dTextureFactor,
                  (long long)(unsigned int)renderStateConfig.fvf);
    AEMOCK_Record("snap.fog", 1, (long long)D3DRenderParams.d3dFogModeProbably);
}

/* ---- the scenarios ------------------------------------------------ */

static void RUN(int caseId)
{
    int i, j;
    unsigned int seed = (unsigned)caseId * 2654435761u + 12345u;
    RECT rc;
    tUnkWaterBlurStruct src, dst;

    switch (caseId / AE_CASES_PER_GROUP)
    {
    /* --- group 0: the parameter save and the blur ramp ------------- */
    case 0:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        /* every one of the five depthblur_Type arms is driven on
           purpose rather than left to a seed bit */
        D3DRenderParams.tweak_zonalpha = (k % 16 == 15);
        PC_AE_MASTER = (k & 1) * 8 | ((k >> 1) & 1) * 0x1000;
        D3DRenderParams.sunEffectIsOn = (k >> 2) & 1;
        gpst_DX_stDD->ct.cam.nearPlane = AE_NearPlane(k);
        gpst_DX_stDD->ct.cam.farPlane = 100.0f + (float)(k % 7) * 13.0f;
        X(depthBlurNear1) = (float)(k % 5) * 9.0f;
        X(depthBlurType0) = (float)(k % 3) * 21.0f;
        X(depthBlurType1) = (float)(k % 11) * 7.0f;
        X(depthBlurType2) = (float)(k % 4) * 3.0f;
        X(D3D_SaveAEParams)();
        X(D3D_SetGlobalBlurParams)();
        memset(AEMOCK_ramp, 0, sizeof(AEMOCK_ramp));
        X(D3D_DepthBlurBlurFunc)((int*)AEMOCK_ramp);
        for (i = 0; i < 300; i += 7)
        {
            float f = X(D3D_DepthBlurBlurFuncCalc1)(i);
            AEMOCK_Record("calc1", 2, (long long)i, (long long)AE_Bits(f));
            AEMOCK_Record("calc2", 2, (long long)i,
                          (long long)X(D3D_DepthBlurBlurFuncCalc2)(f));
        }
        for (i = -3; i < 8; i++)
            AEMOCK_Record("calc2b", 2, (long long)i,
                          (long long)X(D3D_DepthBlurBlurFuncCalc2)((float)i * 17.5f));
        X(D3D_UpdateBlurTexture)();
        break;
    }

    /* --- group 1: texture creation and the default state block ----- */
    case 1:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        AE_SeedCaches(&seed);
        if (k & 1)
            X(D3D_CreateBlurTextures)();
        if (k & 2)
            X(D3D_AETexturesInit)();
        if (k & 4)
            X(D3D_CreateTextureAndBlankIt)(&D3DRenderParams.tmpRenderTarget);
        X(D3D_SetD3DStatesDefaultForAE)();
        /* a second run over the state the first one left: every guard
           that fired above now takes its other arm, so nothing between
           these two markers may reach the device */
        AEMOCK_Record("defaults.pass2", 0);
        X(D3D_SetD3DStatesDefaultForAE)();
        AEMOCK_Record("defaults.end", 0);
        D3DRenderParams.blurTex = AEMOCK_tex[0];
        D3DRenderParams.blurRenderTarget = AEMOCK_tex[1];
        D3DRenderParams.blurRenderTarget2 = AEMOCK_tex[2];
        D3DRenderParams.unktex1_dword_B31988 = AEMOCK_tex[3];
        D3DRenderParams.unktex2_dword_B3198C = AEMOCK_tex[4];
        D3DRenderParams.tmpRenderTarget = AEMOCK_tex[5];
        break;
    }

    /* --- group 2: the depth-blur draws ----------------------------- */
    case 2:
    {
        int k = caseId % AE_CASES_PER_GROUP;
        float u = AE_Factor(k, 0) * 0.02f;
        float v = AE_Factor(k, 1) * 0.03f;

        gMaxSimultaneousTextures = AE_Stages(k);
        AE_SeedCaches(&seed);
        X(D3D_DepthBlurSD)(AEMOCK_tex[6], u, v);
        X(D3D_DepthBlurHD)(AEMOCK_tex[6], u, v);
        X(D3D_DepthBlur)(AEMOCK_tex[0], AEMOCK_tex[1], u, v);
        X(D3D_Blur)(AEMOCK_tex[0], AEMOCK_tex[1], AE_Factor(k, 2) * 4.0f);
        X(D3D_SetRenderTargetFromTexture)(AEMOCK_tex[2]);
        X(D3D_DrawASurface)(AEMOCK_ramp, D3DFVF_TEX1 | D3DFVF_XYZRHW);
        break;
    }

    /* --- group 3: the single-pass effects -------------------------- */
    case 3:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        AE_SeedCaches(&seed);
        X(D3D_Unknown_AE_Func1)(AEMOCK_tex[0]);
        X(D3D_Unknown_AE_FuncDepthBlur)(AEMOCK_tex[1]);
        for (i = -2; i < 12; i++)
        {
            float f = (float)i * 0.3f + (float)(k % 5) * 0.07f;
            AEMOCK_Record("ctf", 2, (long long)i,
                          (long long)AE_Bits(X(D3D_CalcTextureFactor)(f)));
        }
        X(D3D_Unkown_AE_Func_Flag_4)(AEMOCK_tex[0], AEMOCK_tex[1],
                                     AE_Factor(k, 0), AE_Factor(k, 1) * 2.0f);
        X(D3D_Fade)(AEMOCK_tex[0], AEMOCK_tex[1], (k * 37) & 0xFF);
        rc.left = 0; rc.top = k; rc.right = 640 - k; rc.bottom = 480;
        X(D3D_StretchRect)(AEMOCK_tex[2], &rc);
        X(D3D_MotionBlur)(AEMOCK_tex[0], AEMOCK_tex[1], AE_Factor(k, 2));
        X(D3D_Grayscale)(AEMOCK_tex[3], AEMOCK_tex[4]);
        X(D3D_Brightness)(AEMOCK_tex[3], AEMOCK_tex[4], AE_Factor(k, 3));
        break;
    }

    /* --- group 4: colour balance, both arms, both stage counts ----- */
    case 4:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        AE_SeedCaches(&seed);
        X(D3D_ColorBalance)(AEMOCK_tex[0], AEMOCK_tex[1], AE_Factor(k, 0));
        X(D3D_ColorBalanceLessThanZeroPointHalf)(AEMOCK_tex[0], AEMOCK_tex[1],
                                                 AE_Factor(k, 1) * 0.5f);
        X(D3D_ColorBalanceMoreThanZeroPointHalf)(AEMOCK_tex[0], AEMOCK_tex[1],
                                                 0.5f + AE_Factor(k, 2) * 0.5f);
        X(D3D_ColorBalanceMoreThanZeroPointHalfSD)((int)(seed & 0xFFFFFF),
                                                   (int)(seed >> 8), AEMOCK_tex[2]);
        X(D3D_ColorBalanceMoreThanZeroPointHalfHD)((int)(seed >> 3), AEMOCK_tex[2],
                                                   (int)(seed * 7u));
        break;
    }

    /* --- group 5: the water blur ----------------------------------- */
    case 5:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        AE_SeedCaches(&seed);
        src.i1 = k % 5; src.i2 = k % 3; src.i3 = 511 - (k % 5);
        src.i4 = 511 - (k % 3); src.i5 = 512; src.i6 = 512;
        dst.i1 = 0; dst.i2 = 0; dst.i3 = 639; dst.i4 = 479;
        dst.i5 = 640; dst.i6 = 480;
        X(D3D_WaterBlurHD)(&src, AEMOCK_tex[6], &dst, AE_Factor(k, 0) * 0.01f,
                           AE_Factor(k, 1) * 0.01f, 1.0f + AE_Factor(k, 2));
        X(D3D_WaterBlurSD)(AEMOCK_tex[6], &src, &dst, AE_Factor(k, 0) * 0.01f,
                           AE_Factor(k, 1) * 0.01f);
        X(D3D_WaterBlur)(AEMOCK_tex[6], &src, AEMOCK_tex[7], &dst,
                         AE_Factor(k, 0), AE_Factor(k, 1), 1.0f + AE_Factor(k, 2));
        /* All three arms of the two size tests inside
           DoBlurWaterTexture. Two were not enough: the tests are
           "no larger than 512", so a target of exactly 512 is the only
           place a strict comparison differs from theirs, and the two
           values here were 1024 and 256. Both strictness mutants
           scored clean.
         *
           Sixteen cases each way, counted rather than assumed. */
        AEMOCK_descWidth = (k % 3u) == 0 ? 256 : ((k % 3u) == 1 ? 512 : 1024);
        AEMOCK_descHeight = ((k >> 2) % 3u) == 0 ? 384
                            : (((k >> 2) % 3u) == 1 ? 512 : 768);
        waterTexture = AEMOCK_tex[8];
        X(D3D_DoBlurWaterTexture)();
        AEMOCK_descWidth = 640;
        AEMOCK_descHeight = 480;
        break;
    }

    /* --- group 6: zoom smooth, every stage count ------------------- */
    case 6:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        ENG_ifSingleStreamThenEq2Otherwise8 = (k & 4) ? 2 : 8;
        AE_SeedCaches(&seed);
        X(D3D_ZoomSmooth)(AEMOCK_tex[0], AEMOCK_tex[1], AE_Factor(k, 0) * 3.0f);
        X(D3D_ZoomSmooth2)(AEMOCK_tex[0], AEMOCK_tex[1], AE_Factor(k, 1) * 3.0f);
        ENG_ifSingleStreamThenEq2Otherwise8 = 8;
        break;
    }

    /* --- group 7: motion smooth and remanence ---------------------- */
    case 7:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        AE_SeedCaches(&seed);
        X(D3D_MotionSmoothHD)(AEMOCK_tex[0], AE_Factor(k, 0) * 9.0f,
                              AE_Factor(k, 1) * 9.0f);
        X(D3D_MotionSmoothSD)(AEMOCK_tex[0], AEMOCK_tex[1], AEMOCK_tex[2],
                              AE_Factor(k, 0) * 9.0f, AE_Factor(k, 1) * 9.0f);
        X(D3D_MotionSmooth)(AEMOCK_tex[0], AEMOCK_tex[1], AEMOCK_tex[2],
                            AEMOCK_tex[3], AE_Factor(k, 0), AE_Factor(k, 1));
        X(D3D_RemenanceSD)(AEMOCK_tex[4], AE_Factor(k, 2));
        X(D3D_RemenanceHD)(AEMOCK_tex[4], AE_Factor(k, 2));
        X(D3D_Remenance)(AEMOCK_tex[4], AEMOCK_tex[5], AE_Factor(k, 3));
        break;
    }

    /* --- group 8: the copy to screen and the frame ----------------- */
    case 8:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        tweak_ae_nobands = (k & 1);
        X(CIN_Bands) = (k & 2) ? 1 : 0;
        X(CIN_NoBands) = (k & 4) ? 1 : 0;
        X(bandsVal1) = AE_BandTop(k);
        X(bandsVal2) = AE_BandBottom(k);
        D3DRenderParams.presentParams.BackBufferWidth = (k & 8) ? 1280 : 640;
        D3DRenderParams.presentParams.BackBufferHeight = (k & 8) ? 1024 : 480;
        X(D3D_AE_CopyToScreen)(AEMOCK_tex[0]);
        X(CIN_CutsceneMode)(k & 1);
        AEMOCK_Record("copy.hr", 1,
                      (long long)X(CopyRenderTargetTexture)(D3DRenderParams.d3dDev,
                                                            AEMOCK_tex[1],
                                                            AEMOCK_tex[2]));
        AEMOCK_Record("copy.null", 1,
                      (long long)X(CopyRenderTargetTexture)(NULL, AEMOCK_tex[1],
                                                            AEMOCK_tex[2]));
        D3DRenderParams.presentParams.BackBufferWidth = 640;
        D3DRenderParams.presentParams.BackBufferHeight = 480;
        tweak_ae_nobands = 0;
        break;
    }

    /* --- group 9: the whole frame ---------------------------------- */
    case 9:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        tweak_ae_no = (k == 0);
        tweak_ae_nobands = (k & 32) ? 1 : 0;
        X(CIN_Bands) = (k & 64) ? 0 : 1;
        PC_AE_MASTER = AE_MasterBits(k);
        D3DRenderParams.opacity = (k * 51) % 256;
        X(gAEOpacity) = AE_Factor(k, 0) * 8.0f;
        X(remenanceFactor) = (k & 1) ? 0.5f : 0.0f;
        X(motionBlurFactor) = (k & 2) ? 0.25f : 0.0f;
        X(zoomSmoothFactor) = (k & 4) ? 1.5f : 0.0f;
        X(blurFactor) = (k & 8) ? 2.0f : 0.0f;
        X(zoomSmoothFactor2) = (k & 16) ? 0.75f : 0.0f;
        X(brightnessParam) = (k & 3) ? 0.25f : 0.5f;
        X(colorBalanceParam1) = (k & 5) ? 0.8f : 0.5f;
        X(unknownAEFlag4GlobalVar) = AE_Factor(k, 1);
        X(texFactorManual) = AE_Factor(k, 2) * 2.0f;
        gpst_DX_stDD->ct.cam.cameraMtx.v1.x = 1.0f + (float)(k % 3) * 0.1f;
        gpst_DX_stDD->ct.cam.cameraMtx.Pos.x = (float)(k % 5) * 0.004f;
        gpst_DX_stDD->ct.cam.cameraMtx.Pos.y = (float)(k % 7) * 0.004f;
        gpst_DX_stDD->ct.cam.cameraMtx.mtxflag = (k & 8) ? 8 : 0;
        AE_SeedCaches(&seed);
        X(D3D_PC_AE)(NULL);
        tweak_ae_no = 0;
        tweak_ae_nobands = 0;
        break;
    }

    /* --- group 10: the fade's own timer, driven on purpose ---------
       The step taken out of the opacity each frame is the whole range
       over the span and is then truncated, which puts four separate
       things out of reach of any seed: a step larger than the opacity,
       where the clamp at nought is all that stops it wrapping; the
       clamp at one, without which a long fade would take nothing at
       all; a span of nought, which cannot be divided by; and a
       negative span, which runs the fade backwards past the top of the
       range. AE_FadeSpan drives all four, and the pairing with the two
       starting opacities is counted rather than hoped for -- two cases
       each way for every span. */
    case 10:
    {
        int k = caseId % AE_CASES_PER_GROUP;
        int frames;
        int capped;

        gMaxSimultaneousTextures = AE_Stages(k);
        PC_AE_MASTER = 0x40000;
        D3DRenderParams.opacity = (k < 24) ? 1 + (k % 5) : 255;
        X(gAEOpacity) = AE_FadeSpan(k);
        AE_SeedCaches(&seed);

        /* Four frames for every span, recorded one by one: that is
           where a wrong step shows up, and it is enough to carry the
           backwards case past the top of the range. */
        for (frames = 0; frames < 4; frames++)
        {
            X(D3D_PC_AE)(NULL);
            AEMOCK_Record("fade.opacity", 2, (long long)frames,
                          (long long)D3DRenderParams.opacity);
        }

        /* And then to the end, for the spans that get there cheaply.
           The cap is the assertion for the three spans that never
           finish: reaching it is recorded, so a change that made one
           of them terminate would show up here rather than nowhere. */
        capped = 1;
        for (frames = 0; frames < 64; frames++)
        {
            if ((PC_AE_MASTER & 0x40000) == 0)
            {
                capped = 0;
                break;
            }
            X(D3D_PC_AE)(NULL);
        }
        AEMOCK_Record("fade.end", 4, (long long)capped, (long long)frames,
                      (long long)D3DRenderParams.opacity,
                      (long long)PC_AE_MASTER);
        break;
    }

    /* --- group 11: the script-facing control and getters ----------- */
    case 11:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        PC_AE_MASTER = AE_MasterBits(k * 3);
        D3DRenderParams.opacity = (k & 1) ? 0 : 128;
        for (i = -1; i <= 17; i++)
            for (j = 0; j <= 3; j++)
            {
                X(PC_AE_MASTER_CTRL)(i, (k + i + j) & 1, j,
                                     (float)((k + i * 3 + j) % 6) * 0.5f);
                AEMOCK_Record("ctrl", 4, (long long)i, (long long)j,
                              (long long)PC_AE_MASTER,
                              (long long)AE_Bits(X(PC_AE_MASTER_GET_P1)(i, j)));
                AEMOCK_Record("onoff", 2, (long long)i,
                              (long long)X(PC_AE_MASTER_GET_ONOFF)(i));
            }
        break;
    }

    /* --- group 12: the hue sweep's own depth and phase -------------
       Its three channel amounts are one wave sampled a third of a
       cycle apart, and they are packed into one colour by OR-ing three
       bytes into a word whose upper bytes start as ones. Above a depth
       of one the bytes overflow, and they do not all overflow the same
       way -- so the packing has to be compared and not just the
       arithmetic. Group 9 leaves the depth and the phase to a seed and
       cannot say it reaches that; here both are driven, with the
       pairing counted. */
    case 12:
    {
        int k = caseId % AE_CASES_PER_GROUP;

        gMaxSimultaneousTextures = AE_Stages(k);
        PC_AE_MASTER = 4;
        X(unknownAEFlag4GlobalVar) = AE_HueDepth(k);
        X(texFactorManual) = AE_HuePhase(k);
        AE_SeedCaches(&seed);
        X(D3D_PC_AE)(NULL);
        AEMOCK_Record("hue.factor", 2,
                      (long long)AE_Bits(X(unknownAEFlag4GlobalVar)),
                      (long long)AE_Bits(X(texFactorManual)));
        /* And the wave on its own at the same phase, which pins the
           three channel amounts separately from the colour they are
           packed into. */
        AEMOCK_Record("hue.wave", 3,
                      (long long)AE_Bits(X(D3D_CalcTextureFactor)(AE_HuePhase(k))),
                      (long long)AE_Bits(X(D3D_CalcTextureFactor)(
                          AE_HuePhase(k) - 0.33333331f + 1.0f)),
                      (long long)AE_Bits(X(D3D_CalcTextureFactor)(
                          AE_HuePhase(k) - 0.66666663f + 1.0f)));
        break;
    }

    /* --- group 13: the motion smooth's movement test ---------------
       The smear is gated on how far one point moved since last frame,
       and three things about that are out of reach of a seed: the
       threshold, which is not a round number; the axis, because the
       test is in three dimensions and the smear is in two, so a camera
       moving towards what it is looking at crosses the threshold and
       then smears by nothing; and the fact that last frame's point is
       replaced whether or not the smear ran, which is what stops a
       slow camera accumulating its way over the line.

       The movement is put in rather than computed: one call makes last
       frame's point equal to this frame's, and then a known amount is
       taken off one axis of it, so the second call sees exactly that
       amount. Eight amounts against three axes, each pair twice over
       the group's 48 cases -- counted. */
    default:
    {
        int   k = caseId % AE_CASES_PER_GROUP;
        float d = AE_MotionDelta(k);
        int   axis = (k / 8) % 3;

        gMaxSimultaneousTextures = AE_Stages(k);
        PC_AE_MASTER = 0x80;
        AE_SeedCaches(&seed);

        /* Settles last frame's point on this frame's, whatever the
           camera matrix happens to put it at. */
        X(D3D_PC_AE)(NULL);
        AEMOCK_Record("motion.settled", 3,
                      (long long)AE_Bits(AE_MS_PrevVec.x),
                      (long long)AE_Bits(AE_MS_PrevVec.y),
                      (long long)AE_Bits(AE_MS_PrevVec.z));

        if (axis == 0)
            AE_MS_PrevVec.x = AE_MS_PrevVec.x - d;
        else if (axis == 1)
            AE_MS_PrevVec.y = AE_MS_PrevVec.y - d;
        else
            AE_MS_PrevVec.z = AE_MS_PrevVec.z - d;

        X(D3D_PC_AE)(NULL);
        AEMOCK_Record("motion.moved", 4, (long long)axis,
                      (long long)AE_Bits(AE_MS_PrevVec.x),
                      (long long)AE_Bits(AE_MS_PrevVec.y),
                      (long long)AE_Bits(AE_MS_PrevVec.z));

        /* And a third frame with nothing moved, which must not smear
           -- the frame after a smear is where an effect that kept its
           own idea of last frame's point would go wrong. */
        X(D3D_PC_AE)(NULL);
        AEMOCK_Record("motion.still", 1, (long long)AE_Bits(AE_MS_PrevVec.x));
        break;
    }
    }
    XCAT(SIDE, Snap)();
}

#undef X
#undef XCAT
#undef XCAT2
#undef RUN
