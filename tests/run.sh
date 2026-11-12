#!/bin/sh
# Differential tests: the pre-refactor copy of a file against the
# rewritten one, in a single binary, over identical state.
#
#   ./tests/run.sh            run everything
#   ./tests/run.sh snake      run the tests whose name matches
#
# Needs nothing but gcc. tests/stub holds just enough of windows.h,
# d3d9.h and dsound.h for the real engine headers to parse; it is not
# the real toolchain, so a batch should still be built under MSVC.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every public name in a file under test, renamed so the two copies can
# live in one binary.
snake_syms="GAO_ModifierSNAKE_Apply GAO_ModifierSNAKE_UpdatePointers \
GAO_ModifierSNAKE_Create GAO_ModifierSNAKE_Destroy GAO_ModifierSNAKE_Load \
GAO_ModifierSNAKE_Reinit GAO_ModifierSNAKE_Unapply"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

protex_syms="PROTEX_Disturb PROTEX_Modifier_Reinit PROTEX_Modifier_Destroy \
PROTEX_SetStUstVstP PROTEX_Apply2DMatrix PROTEX_InitModifier PROTEX_ul_Modifier_Load \
PROTEX_Modifier_Create PROTEX_UpdateWaterEffect PROTEX_BeforeDraw \
PROTEX_ComputeStUstVstP PROTEX_Modifier_Unapply PROTEX_Modifier_Apply"

lazy_syms="GAO_ModifierLazy_Compute_SemiMecanical_Model GAO_ModifierLazy_Compute \
GAO_ModifierLazy_Inherit GAO_ModifierLazy_Apply Lazy_FirstInit GAO_ModifierLazy_Reinit \
GAO_ModifierLazy_Create GAO_ModifierLazy_Destroy GAO_ul_ModifierLazy_Load \
GAO_ModifierLazy_Unapply GAO_ModifierLazy_UnApply_Local GAO_ModifierLazy_UnapplyAll"

xmen_syms="XMEN_FirstInit GAO_ModifierXMEN_Create GAO_ModifierXMEN_Destroy \
GAO_ModifierXMEN_Apply GAO_ModifierXMEN_Load GAO_ModifierXMEN_Reinit \
GAO_ModifierXMEN_GetBonePosFromNum GAO_ModifierXMEN_SMOOTH_BUFFER_BendVectors_sub_4235A0 \
GAO_ModifierXMEN_SMOOTH_BUFFER GAO_ModifierXMEN_DRAW GAO_ModifierXMEN_DRAW_ALL \
GAO_ModifierXMEN_Unapply XMEN_OnOff XMEN_SetSize \
bFirst_286 XMEN_STACK_PTR xmenSmoothBuffer_flt_9684BC"

fails=0
run_one() {
    name=$1; live=$2; base=$3; incdir=$4; syms=$5
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) return 0 ;; esac

    printf '=== %s\n' "$name"
    # shellcheck disable=SC2086
    $CC $CFLAGS -c "$live" -o "$OUT/$name.new.o" $(defs NEW_ $syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -I"$incdir" -c "$base" -o "$OUT/$name.base.o" $(defs BASE_ $syms)
    $CC $CFLAGS -c tests/engine_mocks.c -o "$OUT/$name.mocks.o"
    $CC $CFLAGS -c "tests/test_$name.c" -o "$OUT/$name.drv.o"
    $CC "$OUT/$name.new.o" "$OUT/$name.base.o" "$OUT/$name.mocks.o" \
        "$OUT/$name.drv.o" -o "$OUT/$name" -lm
    "$OUT/$name" || fails=$((fails + 1))
    echo
}

# Several files in one binary. Each remaining argument is
# "live.c:baseline.c:sym sym sym", so a driver can drive a group of
# effects that share one set of mocks.
run_group() {
    name=$1; incdir=$2
    shift 2
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) return 0 ;; esac

    printf '=== %s\n' "$name"
    objs=""
    n=0
    for spec in "$@"; do
        live=${spec%%:*}
        rest=${spec#*:}
        base=${rest%%:*}
        syms=${rest#*:}
        n=$((n + 1))
        # shellcheck disable=SC2086
        $CC $CFLAGS -c "$live" -o "$OUT/$name.$n.new.o" $(defs NEW_ $syms)
        # shellcheck disable=SC2086
        $CC $CFLAGS -I"$incdir" -c "$base" -o "$OUT/$name.$n.base.o" $(defs BASE_ $syms)
        objs="$objs $OUT/$name.$n.new.o $OUT/$name.$n.base.o"
    done
    $CC $CFLAGS -c math.c -o "$OUT/$name.math.o"
    $CC $CFLAGS -c tests/gfx_mocks.c -o "$OUT/$name.mocks.o"
    $CC $CFLAGS -c "tests/test_$name.c" -o "$OUT/$name.drv.o"
    # shellcheck disable=SC2086
    $CC $objs "$OUT/$name.math.o" "$OUT/$name.mocks.o" "$OUT/$name.drv.o" \
        -o "$OUT/$name" -lm
    "$OUT/$name" || fails=$((fails + 1))
    echo
}

water2_syms="GFX_Water2_Create GFX_Water2_Destroy GFX_i_Water2_Render \
GFX_Water2_Seti GFX_Water2_Setf GFX_Water2_Setv"

ripple_syms="GFX_Ripple_Create GFX_Ripple_Generate GFX_Ripple_Update \
GFX_Ripple_Alloc GFX_Ripple_Seti GFX_Ripple_Setf GFX_Ripple_Destroy \
GFX_i_Ripple_Render"

smoke_syms="GFX_Smoke_Update GFX_i_Smoke_Render GFX_Smoke_Create \
GFX_Smoke_Destroy GFX_Smoke_Alloc GFX_Smoke_AddOne GFX_Smoke_Seti"

lightning_syms="GFX_Lightning_SampleZero GFX_Lightning_Create \
GFX_Lightning_Destroy GFX_AllocPattern GFX_Lightning_SampleFractal \
GFX_i_Lightning_Render"

spark_syms="GFX_Spark_Create GFX_Spark_Destroy GFX_Spark_Generate \
GFX_Spark_Update GFX_AllocSpark GFX_i_Spark_Render"

profiler_syms="GFX_Profiler_Create GFX_Profiler_Destroy \
GFX_i_Profiler_StartStop GFX_i_Profiler_UpdateMecanic \
GFX_i_Profiler_ComputeMeshState GFX_i_Profiler_Update_With_XYT \
GFX_Profiler_Validate GFX_i_Profiler_Render GFX_SetNumberOfPPP \
GFX_SetNumberOfLPP GFX_SetNumberOfP GFX_Profiler_Seti GFX_Profiler_Setf \
GFX_Profiler_Setv"

diaphragm_syms="GFX_Diaphragm_Create GFX_i_Diaphragm_Render depth_362"
fade_syms="GFX_i_Fade_Render GFX_Fade_Create"
line_syms="GFX_Line_Create GFX_Line_Render"
table_syms="GFX_i_Table_Render GFX_Table_Setv GFX_Table_Create"
speed_syms="GFX_i_Speed_Render GFX_Speed_Create"
flare_syms="GFX_Flare_Create GFX_i_Flare_TestDepth GFX_Flare_Test \
GFX_Flare_Render GFXF_f_depth f_9Factor_296"

reveal_syms="STR_l_Effect_Reveal"

math_syms="MATH_CreateTrigTables MATH_CreateSqrtTable MATH_CreateInvTable \
MATH_CreateInvSqrtTable MATH_InitModule MATH_CopyMatrix \
MATH_SetIdentityMatrix MATH_NormalizeVector MATH_CrossProduct \
MATH_NormalizeEqualVector MATH_b_NulVector MATH_b_EqVectorWithEpsilon \
MATH_f_VecSin MATH_f_VecCos MATH_f_VecAngle MATH_f_ZAngle MATH_f_XAngle \
MATH_DistanceOfTwoPoints MATH_AddScaleVector MATH_TransformVector \
MATH_TransformVertex MATH_TransformVectorNoScale \
MATH_TransformVertexNoScale MATH_Copy33Matrix MATH_Mul33MatrixMatrix \
MATH_Transp33MatrixWithoutBuffer MATH_InvertMatrix \
MATH_VectorGlobalToLocal MATH_VertexGlobalToLocal MATH_GetScale \
MATH_SetScale MATH_SetZoom MATH_NormalizeVectorWithNullVectorCheck \
MATH_SetNormVector MATH_BlendVector MATH_b_VecColinear \
MATH_InterpolateBetweenTwoPosWithFactor MATH_GetRotationMatrix \
MATH_PointLineProjection MATH_PointSegmentProjection MATH_PowLimitedCE \
MATH_i_SolveCubicEquation MATH_PointParabolProjection2D \
MATH_PointParabolProjection3D MATH_VecRound MATH_GetZoom \
MATH_SetZoomBuffered MATH_MakeOGLMatrix MATH_Mul44MatrixMatrix \
MATH_MulMatrixMatrix MATH_MakeRotationMatrix_AxisAngle \
MATH_RotateMatrix_AroundGlobalXAxis MATH_RotateMatrix_AroundGlobalYAxis \
MATH_RotateMatrix_AroundGlobalZAxis MATH_RotateMatrix_AroundLocalXAxis \
MATH_RotateMatrix_AroundLocalYAxis MATH_RotateMatrix_AroundLocalZAxis \
MATH_MakeRotationMatrix_UsingSight MATH_MakeRotationMatrix_UsingBanking \
MATH_OrientMatrix_UsingSight MATH_OrientMatrix_UsingBanking \
MATH_InvertQuaternion MATH_NormQuat MATH_Invert33Matrix \
MATH_TranspEq33Matrix MATH_Orthonormalize MATH_Slerp \
MATH_ConvertQuaternionToMatrix MATH_InvNormalQuaternion \
MATH_ConvertMatrixToQuaternion MATH_MatrixBlend MATH_QuaternionBlend \
MATH_l_CalculateCorrectType MATH_b_TypeIsCorrect MATH_SetCorrectType \
MATH_RoundVectorWithPrecision MATH_SubstractVec MATH_VecLen \
MATH_f_PointLineDistance MATH_f_VecCosPlusArcCos \
MATH_calc_curr_quat_from_dt_800 MATH_calc_curr_quat_from_dt_1000 \
MATH_calc_curr_quat_from_dt_2000 MATH_calc_curr_quat_from_dt_4000 \
MATH_calc_curr_quat_from_dt_8000 MATH_CompressQuaternion \
MATH_DecompressQuaternion MATH_VecSet MATH_VecAddVec \
MATH_VecAddVecWithoutBuffer MATH_VecMultScalar \
MATH_VecMultScalarWithoutBuffer MATH_VecLenSquared MATH_SetNullVector \
MATH_VecInvert MATH_VecMultNormScalar MATH_b_NulVectorWithEpsilon \
MATH_LinearInterpolateFloats MATH_Matrix_AllocArray \
MATH_Matrix_ReallocArray MATH_Matrix_Free MATH_Matrix_Alloc MATH_f_MaxOf3 \
MATH_f_MinOf3 MATH_VecRotate MATH_GetCos MATH_GetSin MATH_GetSqrt \
MATH_GetInverseSqrt MATH_GetInverse MATH_RandFloat MATH_RandFloatRange \
MATH_DotProduct MATH_gf_1024by2Pi MATH_gd_magicDoubleForRound \
MATH_gst_IdentityMatrix MATH_gstUnknownMatrix MATH_gst_IdentityVector \
MATH_gst_BaseVectorI MATH_gst_BaseVectorJ MATH_gst_BaseVectorK \
MATH_gst_NulVector MATH_gf_TableSin MATH_gl_TableRacine \
MATH_gl_TableInverse MATH_gl_TableInverseRacine MATH_gst_BlendQuatFrom \
MATH_gst_BlendQuatTo MATH_gf_BlendTheta MATH_gf_BlendInvSinTheta \
float_get_max_of_3vals_sub_48BBB0 float_get_min_of_3vals_sub_48BB80 Q0_137 \
Q1_138 fTheta_139 fInvSinTheta_140"


FILTER=$1

run_math() {
    name=math
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) return 0 ;; esac
    printf '=== %s\n' "$name"
    # shellcheck disable=SC2086
    $CC $CFLAGS -c math.c -o "$OUT/math.new.o" $(defs NEW_ $math_syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -c tests/baseline_math.c -o "$OUT/math.base.o" $(defs BASE_ $math_syms)
    $CC $CFLAGS -c tests/math_mocks.c -o "$OUT/math.mocks.o"
    $CC $CFLAGS -c tests/test_math.c -o "$OUT/math.drv.o"
    $CC "$OUT/math.new.o" "$OUT/math.base.o" "$OUT/math.mocks.o" \
        "$OUT/math.drv.o" -o "$OUT/math" -lm
    "$OUT/math" || fails=$((fails + 1))
    echo
}

run_math

run_one snakemodifier modifiers/snakemodifier.c \
        tests/baseline_snakemodifier.c modifiers "$snake_syms"

run_one protexmodifier modifiers/protexmodifier.c \
        tests/baseline_protexmodifier.c modifiers "$protex_syms"

run_one lazymodifier modifiers/lazymodifier.c \
        tests/baseline_lazymodifier.c modifiers "$lazy_syms"

run_one xmenmodifier modifiers/xmenmodifier.c \
        tests/baseline_xmenmodifier.c modifiers "$xmen_syms"

run_group gfxgrid gfx \
    "gfx/gfxwater2.c:tests/baseline_gfxwater2.c:$water2_syms" \
    "gfx/gfxripple.c:tests/baseline_gfxripple.c:$ripple_syms" \
    "gfx/gfxsmoke.c:tests/baseline_gfxsmoke.c:$smoke_syms"

run_group gfxlightning gfx \
    "gfx/gfxlightning.c:tests/baseline_gfxlightning.c:$lightning_syms"

run_group gfxspark gfx \
    "gfx/gfxspark.c:tests/baseline_gfxspark.c:$spark_syms"

run_group gfxprofiler gfx \
    "gfx/gfxprofiler.c:tests/baseline_gfxprofiler.c:$profiler_syms"

run_group gfxdraw gfx \
    "gfx/gfxdiaphragm.c:tests/baseline_gfxdiaphragm.c:$diaphragm_syms" \
    "gfx/gfxfade.c:tests/baseline_gfxfade.c:$fade_syms" \
    "gfx/gfxline.c:tests/baseline_gfxline.c:$line_syms" \
    "gfx/gfxtable.c:tests/baseline_gfxtable.c:$table_syms" \
    "gfx/gfxspeed.c:tests/baseline_gfxspeed.c:$speed_syms" \
    "gfx/gfxflare.c:tests/baseline_gfxflare.c:$flare_syms"

# The string reveal effect: the live copy comes out of geostr.c, which
# is compiled on its own because only the reveal and the geometry it
# touches are needed.
run_reveal() {
    [ -n "$FILTER" ] && case "streveal" in *"$FILTER"*) ;; *) return 0 ;; esac
    printf '=== %s\n' streveal
    $CC $CFLAGS -c tests/baseline_streveal.c -o "$OUT/streveal.base.o" \
        -DSTR_l_Effect_Reveal=BASE_STR_l_Effect_Reveal
    $CC $CFLAGS -c graphicsobjects/geostr.c -o "$OUT/streveal.new.o" \
        -DSTR_l_Effect_Reveal=NEW_STR_l_Effect_Reveal
    $CC $CFLAGS -c tests/test_streveal.c -o "$OUT/streveal.drv.o"
    $CC $CFLAGS -c tests/streveal_mocks.c -o "$OUT/streveal.mocks.o"
    $CC "$OUT/streveal.base.o" "$OUT/streveal.new.o" "$OUT/streveal.drv.o" \
        "$OUT/streveal.mocks.o" -o "$OUT/streveal" -lm
    "$OUT/streveal" || fails=$((fails + 1))
    echo
}

run_reveal

if [ "$fails" -ne 0 ]; then
    echo "$fails test binary/binaries reported differences"
    exit 1
fi
echo "no differences"
