#!/bin/sh
# The differential test for platform/d3daftereffects.c.
#
#   ./tests/run_d3daftereffects.sh
#
# tests/run.sh calls this script, so there is no second copy of the
# symbol list; running it on its own is the quick way to work on it.
#
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes on every name either of them defines at file scope -- the
# globals as well as the functions, since both copies define them and
# each side has to read and write its own.
#
# tests/d3daftereffects_sdk.h is forced into both sides with -include. The
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
D3D_Grayscale D3D_Brightness \
D3D_ColorBalanceLessThanZeroPointHalf D3D_ColorBalanceMoreThanZeroPointHalfSD \
D3D_ColorBalanceMoreThanZeroPointHalfHD D3D_ColorBalanceMoreThanZeroPointHalf \
D3D_ColorBalance D3D_WaterBlurHD D3D_WaterBlurSD D3D_WaterBlur \
D3D_DoBlurWaterTexture D3D_ZoomSmooth D3D_ZoomSmooth2 D3D_MotionSmoothHD \
D3D_MotionSmoothSD D3D_MotionSmooth D3D_RemenanceSD D3D_RemenanceHD \
D3D_Remenance D3D_Unknown_AE_FuncDepthBlur CopyRenderTargetTexture D3D_PC_AE \
PC_AE_MASTER_CTRL PC_AE_MASTER_GET_ONOFF PC_AE_MASTER_GET_P1 \
CIN_CutsceneMode \
AE_SetRenderState AE_SetStageState AE_SetSamplerState"

globals="depthBlurNear1 depthBlurType0 depthBlurType1 depthBlurType2 \
bandsVal1 bandsVal2 bandsVal3 bandsVal4 CIN_Bands CIN_NoBands \
CIN_gf_BandHeight gAEOpacity \
colorBalanceParam1 brightnessParam unknownAEFlag4GlobalVar texFactorManual \
remenanceFactor motionBlurFactor zoomSmoothFactor blurFactor \
zoomSmoothFactor2 texFactorProbablyUnunsedType2 blackAndWhiteParam1"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== d3daftereffects ==="
$CC $CFLAGS -include tests/d3daftereffects_sdk.h -c platform/d3daftereffects.c \
    -o "$OUT/d3drender_ae.new.o" $(defs NEW_ $fns $globals)
# The frozen copy still spells three functions and one global the way
# the decompiler named them; each is mapped onto the same per-side
# symbol the rewrite's new name takes, rather than the baseline being
# edited to follow a rename.
$CC $CFLAGS -include tests/d3daftereffects_sdk.h \
    -DPC_AE_MASTER_GET_ONOFF_sub_4A37D0=BASE_PC_AE_MASTER_GET_ONOFF \
    -DD3D_Unkown_AE_Func_Flag_1=BASE_D3D_Grayscale \
    -DD3D_Unkown_AE_Func_2=BASE_D3D_Brightness \
    -DunkAEFunc2Param=BASE_brightnessParam \
    -c tests/baseline_d3daftereffects.c \
    -o "$OUT/d3drender_ae.base.o" $(defs BASE_ $fns $globals)
# The cutscene switch, whether a script may change it and the band
# height are cinebands.c now. Compiled as a second unit of the
# rewrite's side rather than linked plain, because the cases drive
# each side's copy of the switch and the two must stay apart.
$CC $CFLAGS -c cinebands.c -o "$OUT/d3drender_ae.cbd.o" \
    $(defs NEW_ $globals) $(defs NEW_ $fns)
$CC $CFLAGS -c math.c -o "$OUT/d3drender_ae.math.o"
# The blur's arithmetic, which both sides reach: the rewrite calls it
# and the frozen copy carries its own inlined version, so one copy
# linked in serves the rewrite and the frozen copy is unaffected.
$CC $CFLAGS -c depthblur.c -o "$OUT/d3drender_ae.dpb.o"
# watersmooth.c holds the water smoothing's rectangles and steps for
# both renderers. A move out of this file, so the frozen copy still
# has them inline and the water-blur cases compare the two.
$CC $CFLAGS -c watersmooth.c -o "$OUT/d3drender_ae.wsm.o"
# aftereffects/aefade.c holds the fade's timer -- the step, the truncation and the
# finish -- for both renderers. A move out of this file, so the frozen
# copy still steps the opacity inline and the fade cases compare the
# two.
$CC $CFLAGS -c aftereffects/aefade.c -o "$OUT/d3drender_ae.afd.o"
# aftereffects/aehue.c holds the hue sweep's wave and, more to the point, the way
# its three channels are packed into one colour -- which is where a
# depth above one stops being a clamp question. A move out of this
# file, so the frozen copy still packs them inline and the cases
# compare the two.
$CC $CFLAGS -c aftereffects/aehue.c -o "$OUT/d3drender_ae.ahu.o"
# aftereffects/aemotion.c holds the motion smooth's camera-movement test and its
# four taps. The test mixes double and float on purpose and the taps
# are neither symmetric nor sorted, so both are compared against the
# frozen copy rather than reasoned about.
$CC $CFLAGS -c aftereffects/aemotion.c -o "$OUT/d3drender_ae.amo.o"
# aftereffects/aezoom2.c holds the second zoom smooth: the stage split, the walk of
# eight angles it decides, the weight, and the per-sample transform --
# which is written as a rotation and is not one. Both renderers take
# all of that from here.
$CC $CFLAGS -c aftereffects/aezoom2.c -o "$OUT/d3drender_ae.az2.o"
# aftereffects/aeframe.c holds the four sum-of-samples effects' schedules; the
# screen blur's seven reaches moved there out of this file, so the
# cases compare them against the frozen copy.
$CC $CFLAGS -c aftereffects/aeframe.c -o "$OUT/d3drender_ae.aef.o"
$CC $CFLAGS -include tests/d3daftereffects_sdk.h -c tests/d3daftereffects_mocks.c \
    -o "$OUT/d3drender_ae.mocks.o"
$CC $CFLAGS -include tests/d3daftereffects_sdk.h -c tests/test_d3daftereffects.c \
    -o "$OUT/d3drender_ae.drv.o"
$CC "$OUT/d3drender_ae.new.o" "$OUT/d3drender_ae.base.o" "$OUT/d3drender_ae.math.o" \
    "$OUT/d3drender_ae.cbd.o" \
    "$OUT/d3drender_ae.dpb.o" "$OUT/d3drender_ae.wsm.o" "$OUT/d3drender_ae.afd.o" "$OUT/d3drender_ae.ahu.o" "$OUT/d3drender_ae.amo.o" "$OUT/d3drender_ae.az2.o" "$OUT/d3drender_ae.aef.o" "$OUT/d3drender_ae.mocks.o" "$OUT/d3drender_ae.drv.o" \
    -o "$OUT/d3drender_ae" -lm
"$OUT/d3drender_ae"
