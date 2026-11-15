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

events_syms="EVE_InitData EVE_pst_DuplicateData EVE_Duplicate \
EVE_b_Track_UseFlash EVE_Event_AIFunction_Play EVE_i_SearchLabel \
EVE_i_SearchTime EVE_SetAllTimesTo EVE_RunningTracksAtStart \
EVE_SetRunningTrack EVE_SetRunningTracks EVE_SetPauseTrack \
EVE_SetPauseTracks EVE_FlashMatrix EVE_GetTracksTime EVE_SetTracksTime \
EVE_SynchronizeTracks EVE_FreeTrack EVE_DeleteListTracks \
EVE_FreeMainStruct EVE_p_GetPdata EVE_w_Event_InterpolationKey_GetType \
EVE_pst_Event_InterpolationKey_GetQuaternion \
EVE_Anim_InterpolationKey_CopyMatrix \
EVE_pst_Event_InterpolationKey_GetNextValue \
EVE_pst_Event_InterpolationKey_GetRotation \
EVE_Event_InterpolationKey_CopyMatrix \
EVE_pf_Event_InterpolationKey_GetTime \
EVE_f_Event_InterpolationKey_InterpolateTime EVE_Event_AIFunction_Resolve \
EVE_Event_AIFunction_Load EVE_Anim_ComputeInterpoledMatrix \
EVE_Event_ComputeInterpoledMatrix EVE_pst_Event_InterpolationKey_GetPos \
EVE_Anim_TranslationKey_Play EVE_Anim_QuaternionKey_Play \
EVE_RunAnimTrack EVE_c_UpdateLOD EVE_Event_InterpolationKey_Play \
EVE_RunOneTrack EVE_b_RunAnimation EVE_b_RunAllTracks \
EVE_AllocateMainStruct EVE_ResolveTrackRef EVE_ResolveListTrackRef \
EVE_ReinitTrack EVE_ReinitData EVE_ResolveRefs \
EVE_Event_InterpolationKey_Load EVE_LoadListEvents \
EVE_ul_CallbackLoadListTracks EVE_MainCall EVE_gb_CanFlash"


textures_syms="TEX_Compress24_Xo2 TEX_Compress24_Yo2 TEX_Compress_Xo2 TEX_Compress_Yo2 \
TEX_Compress8_Xo2 TEX_Compress8_Yo2 TEX_Compress4_Xo2 TEX_Compress4_Yo2 \
TEX_Expand_XBy2 TEX_Expand_YBy2 TEX_Expand8_XBy2 TEX_Expand8_YBy2 \
TEX_Expand4_XBy2 TEX_Expand4_YBy2 TEX_Expand24_XBy2 TEX_Expand24_YBy2 \
TEX_Convert_32SwapRB TEX_Convert_24To32 TEX_Convert_24To32SwapRB \
TEX_Convert_4To32 TEX_Convert_8To32 TEX_Convert_4To32SwapRB TEX_Convert_8To32SwapRB \
TEX_ul_GetPower2Up TEX_uc_GetMaskShift TEX_GetMaskParams TEX_GetColorConvertParams \
TEX_l_ConvertPF TEX_l_ConvertSize TEX_l_ConvertSize_24To32 TEX_File_VSwap \
TEX_l_File_TgaUncompress TEX_File_DevelopParams TEX_l_MemoryNeeded TEX_l_DimensionIsGood \
TEX_File_TransformParams TEX_b_File_CheckParams TEX_l_ConvertSizeFast TEX_File_LoadPalette \
TEX_Manager_ComputeCompression TEX_Manager_Init TEX_Manager_Reinit \
TEX_l_File_LoadTga TEX_l_File_LoadBmp TEX_l_File_LoadRaw TEX_l_File_LoadRawPalette \
TEX_l_File_LoadAnimated TEX_pst_List_FindTexture TEX_pst_List_FindPalette \
TEX_w_List_AddTexture TEX_w_List_AddPalette TEX_List_Init TEX_List_Free \
TEX_pst_Anim_Get TEX_pst_Anim_Add TEX_Anim_Update TEX_Anim_Reinit \
TEX_w_Manager_ChooseGoodInTex TEX_File_FreeDescription \
al_QualityCompressionOrder al_QualityCompressionLimit \
TEX_gst_GlobalList TEX_gdst_Animated TEX_gi_NbAnimated TEX_gi_NbAnimatedMax \
sf_TimeLeft_218"


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

run_textures() {
    name=textures
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) return 0 ;; esac
    printf '=== %s\n' "$name"
    # shellcheck disable=SC2086
    $CC $CFLAGS -c textures.c -o "$OUT/textures.new.o" $(defs NEW_ $textures_syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -c tests/baseline_textures.c -o "$OUT/textures.base.o" $(defs BASE_ $textures_syms)
    $CC $CFLAGS -c tests/textures_mocks.c -o "$OUT/textures.mocks.o"
    $CC $CFLAGS -c tests/test_textures.c -o "$OUT/textures.drv.o"
    $CC "$OUT/textures.new.o" "$OUT/textures.base.o" "$OUT/textures.mocks.o" \
        "$OUT/textures.drv.o" -o "$OUT/textures" -lm
    "$OUT/textures" || fails=$((fails + 1))
    echo
}

run_textures

run_events() {
    name=events
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) return 0 ;; esac
    printf '=== %s\n' "$name"
    # shellcheck disable=SC2086
    $CC $CFLAGS -c events.c -o "$OUT/events.new.o" $(defs NEW_ $events_syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -c tests/baseline_events.c -o "$OUT/events.base.o" $(defs BASE_ $events_syms)
    $CC $CFLAGS -c math.c -o "$OUT/events.math.o"
    $CC $CFLAGS -c tests/events_mocks.c -o "$OUT/events.mocks.o"
    $CC $CFLAGS -c tests/test_events.c -o "$OUT/events.drv.o"
    $CC "$OUT/events.new.o" "$OUT/events.base.o" "$OUT/events.math.o" \
        "$OUT/events.mocks.o" "$OUT/events.drv.o" -o "$OUT/events" -lm
    "$OUT/events" || fails=$((fails + 1))
    echo
}

run_events

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

# animation.c: the file sits at the repo root, so the frozen copy needs
# the root on the *quoted* include path only -- -I. would also put the
# engine's own strings.h in front of the system <strings.h>.
animation_syms="ANI_pst_CreateGameObjectAnim ANI_LimitFrequencyBetween0And255 \
ANI_ClearBlendingFlag ANI_DestroyAnimation ANI_DestroySkeleton ANI_Free \
ANI_AllocAnim ANI_ApplyHierarchyOnFlash ANI_GaoAnimationIsTransitioning \
ANI_RecomputeFlash ANI_UpdateFlash ANI_uc_GetEngineCanalByAICanal \
ANI_pst_GetObjectByAICanal ANI_ChangeBoneShape ANI_ApplyShape ANI_ChangeShape \
ANI_SetTrackList ANI_ChangeAction ANI_UpdateFlashAfterMagicBoxTranslation \
ANI_GetEndSpeedOfMB ANI_InterpolateMagicBoxPositionAfterEndOfAnim \
ANI_b_ComputeMagicBoxPositionForBlendAnim ANI_b_BlendMagicBox \
ANI_PlayLeadAnimation ANI_PlayGameObjectAnimation ANI_OneCall ANI_InitSkeleton \
ANI_pst_DuplicateGameObjectAnim ANI_ul_ShapeCallback \
ANI_AttachSkeletonModelToWorld ANI_pst_Load ANI_ResolveRefs ANI_ReinitAnimation \
ANI_Reinit ANI_RemoveHierarchyOnFlash ANI_pst_GetReference \
ANI_PlayBlendAnimationWithoutMagicBox ANI_BeforeRender ANI_BeforeRendering \
ANI_MainCall ANI_pst_GetReferenceInit ANI_ComputeBoneMatrix ANI_Render \
ANI_ChangeDefaultSkeleton ANI_RestoreDefaultSkeleton ANI_DestroyShape \
ANI_CloneSet ANI_c_DefaultBitFieldLOD"

run_animation() {
    [ -n "$FILTER" ] && case "animation" in *"$FILTER"*) ;; *) return 0 ;; esac
    printf '=== %s\n' animation
    # shellcheck disable=SC2086
    $CC $CFLAGS -c animation.c -o "$OUT/animation.new.o" $(defs NEW_ $animation_syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -iquote . -c tests/baseline_animation.c -o "$OUT/animation.base.o" \
        $(defs BASE_ $animation_syms)
    $CC $CFLAGS -c math.c -o "$OUT/animation.math.o"
    $CC $CFLAGS -c tests/animation_mocks.c -o "$OUT/animation.mocks.o"
    $CC $CFLAGS -c tests/test_animation.c -o "$OUT/animation.drv.o"
    $CC "$OUT/animation.new.o" "$OUT/animation.base.o" "$OUT/animation.math.o" \
        "$OUT/animation.mocks.o" "$OUT/animation.drv.o" -o "$OUT/animation" -lm
    "$OUT/animation" || fails=$((fails + 1))
    echo
}

run_animation

# strings.c: like animation.c it sits at the repo root, and the frozen
# copy includes its headers through "../" for the same reason -- -I.
# would put the engine's own strings.h in front of the system one,
# which glibc's <string.h> pulls in. Both copies' file-scope globals
# are renamed as well as their functions: the initialised ones collide
# at link time otherwise, and the rest would be silently shared.
strings_syms="STR_3DStringList_Reinit STR_3DStringList_Clear STR_3DStringList_Add \
STR_pst_GetFont STR_FontResolveAlternativeKey STR_FontAddAlternativeKey \
STR_b_LoadFontDescriptor STR_AddFont STR_SetDisplay STR_Close STR_pst_GetFontDesc \
STR_pst_Get STR_l_GetLength STR_pst_GetString STR_l_GetMarkIndex STR_l_GetValidIndex \
STR_l_GetValidNumber STR_GetRectShort STR_GlobalSet STR_GlobalSetChar STR_sz_GlobalGet \
STR_Reset STR_GetKeyName STR_sz_ControlsReadFormat_pcINO STR_l_ReadFormat_Int \
STR_l_ReadFormat_Hexa STR_l_ReadFormat_Float STR_l_ReadFormat STR_SetCurEffect \
STR_i_EffectAdd STR_ParseString STR_ChangeStringLength STR_RecomputePos \
STR_i_GlobalLength STR_l_GetStringLength STR_EffectDelAllForAString STR_SetString \
STR_SetSize STR_pst_GetSize STR_Align STR_SetStringExt STR_InsertString STR_SetFloat \
STR_InsertFloat STR_SetPivot STR_pst_GetPivot STR_SetFlag STR_ul_GetFlag STR_SetColor \
STR_SetAlpha STR_SetColorExt STR_GetColor STR_ChangePage STR_SetPos STR_EffectCheck \
STR_DelChar STR_SetChar STR_i_GetChar STR_l_GetSubLength STR_pst_GetMin STR_pst_GetMax \
STR_SetFrame STR_EffectDelAll STR_EffectDel STR_SetFramePos STR_SetFrameBorderSize \
STR_SetFrameColor STR_SetSubStringWithLine STR_SetSubStringWithMark STR_SetSubString \
STR_i_CountLine STR_i_CountMark STR_ToString STR_SetLetterFlag STR_GetLetterFlag \
STR_ToUpper STR_EffectSeti STR_i_EffectGetId STR_EffectGeti STR_EffectSetf \
STR_ReinitAll STR_Init3DStringExtExt STR_Init3DString STR_Init3DStringExt \
STR_Init3DStringCenter STR_Init3DStringFromSTR \
pcINO_ControlsReadFormatBufferSize pcINO_ControlsReadFormatBuffer STRDataPtrStart \
STR_sgst_Format STR_si_Id STR_f_DefaultLetterDepth \
STRU1Offset STRV1Offset STRU2Offset STRV2Offset \
STRU3Offset STRV3Offset STRU4Offset STRV4Offset"

run_strings() {
    [ -n "$FILTER" ] && case "strings" in *"$FILTER"*) ;; *) return 0 ;; esac
    printf '=== %s\n' strings
    # shellcheck disable=SC2086
    $CC $CFLAGS -c strings.c -o "$OUT/strings.new.o" $(defs NEW_ $strings_syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -c tests/baseline_strings.c -o "$OUT/strings.base.o" \
        $(defs BASE_ $strings_syms)
    $CC $CFLAGS -c tests/strings_mocks.c -o "$OUT/strings.mocks.o"
    # shellcheck disable=SC2086
    $CC $CFLAGS -c tests/strings_runner.c -o "$OUT/strings.rb.o" \
        -DRUNNER=STRCASE_RunBase $(defs BASE_ $strings_syms)
    # shellcheck disable=SC2086
    $CC $CFLAGS -c tests/strings_runner.c -o "$OUT/strings.rn.o" \
        -DRUNNER=STRCASE_RunNew $(defs NEW_ $strings_syms)
    $CC $CFLAGS -c tests/test_strings.c -o "$OUT/strings.drv.o"
    $CC "$OUT/strings.new.o" "$OUT/strings.base.o" "$OUT/strings.mocks.o" \
        "$OUT/strings.rb.o" "$OUT/strings.rn.o" "$OUT/strings.drv.o" \
        -o "$OUT/strings" -lm
    "$OUT/strings" || fails=$((fails + 1))
    echo
}

run_strings

# The rest each came with a standalone runner of its own; rather than
# inline their symbol lists -- collision's is 161 names long and is
# derived with nm so it cannot drift -- run.sh calls them. They each
# build and run exactly as the blocks above do, each sets -e, and each
# ends with its test binary, so a difference leaves the script's status
# non-zero and the loop below counts it.
for one in tests/runworld.sh \
           tests/run_collision_a.sh \
           tests/run_collision_b.sh \
           tests/run_collision_c.sh \
           tests/runsound.sh \
           tests/run_gfx.sh \
           tests/run_ok3.sh \
           tests/run_particle.sh \
           tests/run_photo.sh \
           tests/run_proctex.sh \
           tests/rungameobject.sh \
           tests/runsoundmod.sh \
           tests/run_io.sh \
           tests/run_ways.sh \
           tests/run_matdraw.sh \
           tests/run_profiler.sh \
           tests/run_snp.sh \
           tests/run_actions.sh \
           tests/run_tables.sh \
           tests/run_bigfiles.sh \
           tests/run_adpcm.sh \
           tests/run_winio.sh \
           tests/run_winsound.sh \
           tests/run_wingdi_ae.sh \
           tests/run_wingdi.sh \
           tests/run_overlay.sh \
           tests/run_video.sh \
           tests/run_videosound.sh \
           tests/run_editorcamera.sh
do
    name=$(basename "$one" .sh)
    [ -n "$FILTER" ] && case "$name" in *"$FILTER"*) ;; *) continue ;; esac
    "$one" || fails=$((fails + 1))
    echo
done

if [ "$fails" -ne 0 ]; then
    echo "$fails test binary/binaries reported differences"
    exit 1
fi
echo "no differences"
