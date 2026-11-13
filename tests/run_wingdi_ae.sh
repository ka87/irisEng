#!/bin/sh
# The differential test for win/wingdi_aftereffects.c.
#
#   ./tests/run_wingdi_ae.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes on every name either of them defines at file scope -- the
# globals as well as the functions, since both copies define them and
# each side has to read and write its own.
#
# tests/wingdi_ae_sdk.h is forced into both sides with -include. The
# stub's D3D constants carry deliberately arbitrary values; the file
# under test writes the SDK's own numbers at 150-odd sites and the
# rewrite spells them with the names, so the two only compare equal if
# the names carry the SDK's numbers. See the head of that file.
#
# The real math.c is linked rather than mocked, and the driver calls
# MATH_InitModule before the first case.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

fns="PC_CIN_CutsceneMode D3D_SaveAEParams D3D_SetGlobalBlurParams \
D3D_DepthBlurBlurFuncCalc1 D3D_DepthBlurBlurFuncCalc2 D3D_DepthBlurBlurFunc \
D3D_UpdateBlurTexture D3D_CreateBlurTextures D3D_CreateTextureAndBlankIt \
D3D_AETexturesInit D3D_SetD3DStatesDefaultForAE D3D_DrawASurface \
D3D_DepthBlurSD D3D_DepthBlurHD D3D_SetRenderTargetFromTexture D3D_DepthBlur \
D3D_Blur D3D_Unknown_AE_Func1 D3D_CalcTextureFactor D3D_Unkown_AE_Func_Flag_4 \
D3D_Fade D3D_StretchRect D3D_MotionBlur D3D_AE_CopyToScreen \
D3D_Unkown_AE_Func_Flag_1 D3D_Unkown_AE_Func_2 \
D3D_ColorBalanceLessThanZeroPointHalf D3D_ColorBalanceMoreThanZeroPointHalfSD \
D3D_ColorBalanceMoreThanZeroPointHalfHD D3D_ColorBalanceMoreThanZeroPointHalf \
D3D_ColorBalance D3D_WaterBlurHD D3D_WaterBlurSD D3D_WaterBlur \
D3D_DoBlurWaterTexture D3D_ZoomSmooth D3D_ZoomSmooth2 D3D_MotionSmoothHD \
D3D_MotionSmoothSD D3D_MotionSmooth D3D_RemenanceSD D3D_RemenanceHD \
D3D_Remenance D3D_Unknown_AE_FuncDepthBlur CopyRenderTargetTexture D3D_PC_AE \
PC_AE_MASTER_CTRL PC_AE_MASTER_GET_ONOFF_sub_4A37D0 PC_AE_MASTER_GET_P1 \
AE_SetRenderState AE_SetStageState AE_SetSamplerState"

globals="depthBlurNear1 depthBlurType0 depthBlurType1 depthBlurType2 \
bandsVal1 bandsVal2 bandsVal3 bandsVal4 CIN_Bands CIN_NoBands gAEOpacity \
colorBalanceParam1 unkAEFunc2Param unknownAEFlag4GlobalVar texFactorManual \
remenanceFactor motionBlurFactor zoomSmoothFactor blurFactor \
zoomSmoothFactor2 texFactorProbablyUnunsedType2 blackAndWhiteParam1"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== wingdi_aftereffects ==="
$CC $CFLAGS -include tests/wingdi_ae_sdk.h -c win/wingdi_aftereffects.c \
    -o "$OUT/wingdi_ae.new.o" $(defs NEW_ $fns $globals)
$CC $CFLAGS -include tests/wingdi_ae_sdk.h -c tests/baseline_wingdi_ae.c \
    -o "$OUT/wingdi_ae.base.o" $(defs BASE_ $fns $globals)
$CC $CFLAGS -c math.c -o "$OUT/wingdi_ae.math.o"
$CC $CFLAGS -include tests/wingdi_ae_sdk.h -c tests/wingdi_ae_mocks.c \
    -o "$OUT/wingdi_ae.mocks.o"
$CC $CFLAGS -include tests/wingdi_ae_sdk.h -c tests/test_wingdi_ae.c \
    -o "$OUT/wingdi_ae.drv.o"
$CC "$OUT/wingdi_ae.new.o" "$OUT/wingdi_ae.base.o" "$OUT/wingdi_ae.math.o" \
    "$OUT/wingdi_ae.mocks.o" "$OUT/wingdi_ae.drv.o" -o "$OUT/wingdi_ae" -lm
"$OUT/wingdi_ae"
