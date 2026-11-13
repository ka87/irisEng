/* math.c, before the rewrite against after.
 *
 * Every function in the file is pure arithmetic over its arguments and
 * the four lookup tables, so this is the one file in the engine where a
 * differential test needs no mocks beyond the three allocator calls.
 *
 * The rewrite was renames plus two structural changes -- the cubic
 * solver's common tail was folded to one exit, and MATH_VecRotate lost
 * two write-only locals and a copy of its cosine -- so every result
 * here is expected to be BIT-IDENTICAL, not merely close. Anything
 * else is a regression.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../math.h"

/* ---- the two copies ---------------------------------------------- */

#define D(ret, name, args) ret NEW_##name args; ret BASE_##name args;

D(void, MATH_InitModule, (void))
D(void, MATH_CopyMatrix, (Mtx4x4*, Mtx4x4*))
D(void, MATH_SetIdentityMatrix, (Mtx4x4*))
D(void, MATH_NormalizeVector, (Vec3*, Vec3*))
D(void, MATH_CrossProduct, (Vec3*, Vec3*, Vec3*))
D(void, MATH_NormalizeEqualVector, (Vec3*))
D(BOOL, MATH_b_NulVector, (Vec3*))
D(BOOL, MATH_b_EqVectorWithEpsilon, (Vec3*, Vec3*, float))
D(float, MATH_f_VecSin, (Vec3*, Vec3*, Vec3*))
D(float, MATH_f_VecCos, (Vec3*, Vec3*))
D(float, MATH_f_VecAngle, (Vec3*, Vec3*, Vec3*))
D(float, MATH_f_ZAngle, (Vec3*, Vec3*))
D(float, MATH_f_XAngle, (Vec3*, Vec3*))
D(float, MATH_DistanceOfTwoPoints, (Vec3*, Vec3*))
D(void, MATH_AddScaleVector, (Vec3*, Vec3*, Vec3*, float))
D(void, MATH_TransformVector, (Vec3*, Mtx4x4*, Vec3*))
D(void, MATH_TransformVertex, (Vec3*, Mtx4x4*, Vec3*))
D(void, MATH_TransformVectorNoScale, (Vec3*, Mtx4x4*, Vec3*))
D(void, MATH_TransformVertexNoScale, (Vec3*, Mtx4x4*, Vec3*))
D(void, MATH_Copy33Matrix, (Mtx4x4*, Mtx4x4*))
D(void, MATH_Mul33MatrixMatrix, (Mtx4x4*, Mtx4x4*, Mtx4x4*, char))
D(void, MATH_Transp33MatrixWithoutBuffer, (Mtx4x4*, Mtx4x4*))
D(void, MATH_InvertMatrix, (Mtx4x4*, Mtx4x4*))
D(void, MATH_VectorGlobalToLocal, (Vec3*, Mtx4x4*, Vec3*))
D(void, MATH_VertexGlobalToLocal, (Vec3*, Mtx4x4*, Vec3*))
D(void, MATH_GetScale, (Vec3*, Mtx4x4*))
D(void, MATH_SetScale, (Mtx4x4*, Vec3*))
D(void, MATH_SetZoom, (Mtx4x4*, float))
D(void, MATH_NormalizeVectorWithNullVectorCheck, (Vec3*, Vec3*))
D(void, MATH_SetNormVector, (Vec3*, Vec3*, float))
D(void, MATH_BlendVector, (Vec3*, Vec3*, Vec3*, float))
D(BOOL, MATH_b_VecColinear, (Vec3*, Vec3*))
D(void, MATH_InterpolateBetweenTwoPosWithFactor, (Vec3*, Vec3*, Vec3*, float))
D(void, MATH_GetRotationMatrix, (Mtx4x4*, Mtx4x4*))
D(void, MATH_PointLineProjection, (Vec3*, Vec3*, Vec3*, Vec3*))
D(void, MATH_PointSegmentProjection, (Vec3*, Vec3*, Vec3*, Vec3*))
D(double, MATH_PowLimitedCE, (float))
D(int, MATH_i_SolveCubicEquation, (float, float, float, Vec3*))
D(void, MATH_PointParabolProjection2D, (Vec3*, Vec3*, float, float))
D(void, MATH_PointParabolProjection3D, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, float))
D(void, MATH_VecRound, (Vec3*))
D(void, MATH_GetZoom, (Vec3*, Mtx4x4*))
D(void, MATH_SetZoomBuffered, (Mtx4x4*, Mtx4x4*, Vec3*))
D(void, MATH_MakeOGLMatrix, (Mtx4x4*, Mtx4x4*))
D(void, MATH_Mul44MatrixMatrix, (Mtx4x4*, Mtx4x4*, Mtx4x4*))
D(void, MATH_MulMatrixMatrix, (Mtx4x4*, Mtx4x4*, Mtx4x4*))
D(void, MATH_MakeRotationMatrix_AxisAngle, (Mtx4x4*, Vec3*, float, Quat4*, char))
D(void, MATH_RotateMatrix_AroundGlobalXAxis, (Mtx4x4*, float))
D(void, MATH_RotateMatrix_AroundGlobalYAxis, (Mtx4x4*, float))
D(void, MATH_RotateMatrix_AroundGlobalZAxis, (Mtx4x4*, float))
D(void, MATH_RotateMatrix_AroundLocalXAxis, (Mtx4x4*, float))
D(void, MATH_RotateMatrix_AroundLocalYAxis, (Mtx4x4*, float))
D(void, MATH_RotateMatrix_AroundLocalZAxis, (Mtx4x4*, float))
D(void, MATH_MakeRotationMatrix_UsingSight, (Mtx4x4*, Vec3*, Vec3*, char))
D(void, MATH_MakeRotationMatrix_UsingBanking, (Mtx4x4*, Vec3*, Vec3*, char))
D(void, MATH_OrientMatrix_UsingSight, (Mtx4x4*, Vec3*, Vec3*))
D(void, MATH_OrientMatrix_UsingBanking, (Mtx4x4*, Vec3*, Vec3*))
D(void, MATH_InvertQuaternion, (Quat4*))
D(void, MATH_NormQuat, (Quat4*, Quat4*))
D(void, MATH_Invert33Matrix, (Mtx4x4*, Mtx4x4*))
D(void, MATH_TranspEq33Matrix, (Mtx4x4*))
D(void, MATH_Orthonormalize, (Mtx4x4*))
D(void, MATH_Slerp, (Quat4*, float, Quat4*, Quat4*, float, float))
D(void, MATH_ConvertQuaternionToMatrix, (Mtx4x4*, Quat4*))
D(void, MATH_InvNormalQuaternion, (Quat4*, Quat4*))
D(void, MATH_ConvertMatrixToQuaternion, (Quat4*, Mtx4x4*))
D(void, MATH_MatrixBlend, (Mtx4x4*, Mtx4x4*, Mtx4x4*, float, BOOL))
D(void, MATH_QuaternionBlend, (Mtx4x4*, Quat4*, Quat4*, float, BOOL))
D(int, MATH_l_CalculateCorrectType, (Mtx4x4*))
D(BOOL, MATH_b_TypeIsCorrect, (Mtx4x4*))
D(void, MATH_SetCorrectType, (Mtx4x4*))
D(void, MATH_RoundVectorWithPrecision, (Vec3*, float))
D(void, MATH_SubstractVec, (Vec3*, Vec3*, Vec3*))
D(float, MATH_VecLen, (Vec3*))
D(float, MATH_f_PointLineDistance, (Vec3*, Vec3*, Vec3*))
D(float, MATH_f_VecCosPlusArcCos, (Vec3*, Vec3*))
D(void, MATH_calc_curr_quat_from_dt_800, (Quat4*, CompressedQuat*))
D(void, MATH_calc_curr_quat_from_dt_1000, (Quat4*, CompressedQuat*))
D(void, MATH_calc_curr_quat_from_dt_2000, (Quat4*, UltraCompressedQuat*))
D(void, MATH_calc_curr_quat_from_dt_4000, (Quat4*, UltraCompressedQuat*))
D(void, MATH_calc_curr_quat_from_dt_8000, (Quat4*, UltraCompressedQuat*))
D(void, MATH_CompressQuaternion, (CompressedQuat*, Quat4*))
D(void, MATH_DecompressQuaternion, (Quat4*, CompressedQuat*))
D(void, MATH_VecSet, (Vec3*, Vec3*))
D(void, MATH_VecAddVec, (Vec3*, Vec3*, Vec3*))
D(void, MATH_VecAddVecWithoutBuffer, (Vec3*, Vec3*))
D(void, MATH_VecMultScalar, (Vec3*, Vec3*, float))
D(void, MATH_VecMultScalarWithoutBuffer, (Vec3*, float))
D(float, MATH_VecLenSquared, (Vec3*))
D(void, MATH_SetNullVector, (Vec3*))
D(void, MATH_VecInvert, (Vec3*, Vec3*))
D(void, MATH_VecMultNormScalar, (Vec3*, Vec3*, float))
D(BOOL, MATH_b_NulVectorWithEpsilon, (Vec3*, float))
D(float, MATH_LinearInterpolateFloats, (float, float, float))
D(float, MATH_GetCos, (float))
D(float, MATH_GetSin, (float))
D(float, MATH_GetSqrt, (float))
D(float, MATH_GetInverseSqrt, (float))
D(float, MATH_GetInverse, (float))
D(float, MATH_RandFloat, (void))
D(float, MATH_RandFloatRange, (float, float))
D(float, MATH_DotProduct, (Vec3*, Vec3*))
D(void, MATH_VecRotate, (Vec3*, Vec3*, Vec3*, float))

float NEW_MATH_f_MaxOf3(float, float, float);
float NEW_MATH_f_MinOf3(float, float, float);
float BASE_float_get_max_of_3vals_sub_48BBB0(float, float, float);
float BASE_float_get_min_of_3vals_sub_48BB80(float, float, float);

/* math.h declares the live copy's tables; the frozen copy's are the
   same declarations compiled with the NEW_ prefix. */
extern float NEW_MATH_gf_TableSin[1024];
extern int NEW_MATH_gl_TableRacine[2048];
extern int NEW_MATH_gl_TableInverse[1024];
extern int NEW_MATH_gl_TableInverseRacine[2048];
extern float BASE_MATH_gf_TableSin[1024];
extern int BASE_MATH_gl_TableRacine[2048];
extern int BASE_MATH_gl_TableInverse[1024];
extern int BASE_MATH_gl_TableInverseRacine[2048];

/* ---- comparison -------------------------------------------------- */

static long cases, diffs;
static const char* site;

static void bad(const char* what, const char* detail)
{
    if (diffs < 25)
        printf("  DIFF %s %s: %s\n", site, what, detail);
    diffs++;
}

static void cmp_mem(const char* what, const void* a, const void* b, size_t n)
{
    cases++;
    if (memcmp(a, b, n) != 0) {
        char msg[256];
        const unsigned char* p = a;
        const unsigned char* q = b;
        size_t i;
        for (i = 0; i < n; i++)
            if (p[i] != q[i]) break;
        snprintf(msg, sizeof msg, "%zu bytes, first differing byte %zu (new %02x base %02x)",
                 n, i, p[i], q[i]);
        bad(what, msg);
    }
}

static void cmp_f(const char* what, float a, float b)
{
    cases++;
    if (memcmp(&a, &b, sizeof a) != 0) {
        char msg[128];
        snprintf(msg, sizeof msg, "new %.9g base %.9g", a, b);
        bad(what, msg);
    }
}

static void cmp_d(const char* what, double a, double b)
{
    cases++;
    if (memcmp(&a, &b, sizeof a) != 0) {
        char msg[128];
        snprintf(msg, sizeof msg, "new %.17g base %.17g", a, b);
        bad(what, msg);
    }
}

static void cmp_i(const char* what, int a, int b)
{
    cases++;
    if (a != b) {
        char msg[64];
        snprintf(msg, sizeof msg, "new %d base %d", a, b);
        bad(what, msg);
    }
}

/* A deterministic generator of its own, so driving the inputs never
   touches the rand() sequence the two copies share. */
static uint32_t seed = 0x1234567u;
static uint32_t nextu(void)
{
    seed = seed * 1664525u + 1013904223u;
    return seed;
}
static float nextf(float lo, float hi)
{
    return lo + (hi - lo) * ((nextu() >> 8) / 16777216.0f);
}
static void fill_vec(Vec3* v, float lo, float hi)
{
    v->x = nextf(lo, hi); v->y = nextf(lo, hi); v->z = nextf(lo, hi);
}
static void fill_mtx(Mtx4x4* m)
{
    fill_vec(&m->v1, -2.0f, 2.0f);
    fill_vec(&m->v2, -2.0f, 2.0f);
    fill_vec(&m->v3, -2.0f, 2.0f);
    fill_vec(&m->Pos, -50.0f, 50.0f);
    m->_m14 = 0.0f; m->_m24 = 0.0f; m->_m34 = 0.0f;
    m->w = 1.0f;
    m->mtxflag = nextu() & 0xF;
}
static void fill_quat(Quat4* q)
{
    q->q1 = nextf(-1.0f, 1.0f); q->q2 = nextf(-1.0f, 1.0f);
    q->q3 = nextf(-1.0f, 1.0f); q->q4 = nextf(-1.0f, 1.0f);
}

#define VV(name, call_new, call_base) do {                                  \
        Vec3 an, ab; site = name;                                           \
        memset(&an, 0, sizeof an); memset(&ab, 0, sizeof ab);               \
        call_new; call_base;                                                \
        cmp_mem("out", &an, &ab, sizeof an);                                \
    } while (0)

#define MM(name, call_new, call_base) do {                                  \
        Mtx4x4 an, ab; site = name;                                         \
        memset(&an, 0, sizeof an); memset(&ab, 0, sizeof ab);               \
        call_new; call_base;                                                \
        cmp_mem("out", &an, &ab, sizeof an);                                \
    } while (0)

#define QQ(name, call_new, call_base) do {                                  \
        Quat4 an, ab; site = name;                                          \
        memset(&an, 0, sizeof an); memset(&ab, 0, sizeof ab);               \
        call_new; call_base;                                                \
        cmp_mem("out", &an, &ab, sizeof an);                                \
    } while (0)

int main(void)
{
    int i, k;

    printf("math.c\n");

    /* 1. the four lookup tables ----------------------------------- */
    site = "InitModule";
    NEW_MATH_InitModule();
    BASE_MATH_InitModule();
    cmp_mem("TableSin", NEW_MATH_gf_TableSin, BASE_MATH_gf_TableSin,
            sizeof NEW_MATH_gf_TableSin);
    cmp_mem("TableRacine", NEW_MATH_gl_TableRacine, BASE_MATH_gl_TableRacine,
            sizeof NEW_MATH_gl_TableRacine);
    cmp_mem("TableInverse", NEW_MATH_gl_TableInverse, BASE_MATH_gl_TableInverse,
            sizeof NEW_MATH_gl_TableInverse);
    cmp_mem("TableInverseRacine", NEW_MATH_gl_TableInverseRacine,
            BASE_MATH_gl_TableInverseRacine, sizeof NEW_MATH_gl_TableInverseRacine);

    /* 2. the table lookups, over a sweep that covers both signs and
          the wrap at each end ------------------------------------- */
    for (i = -2000; i <= 2000; i++) {
        float f = i * 0.01f;
        site = "GetCos";  cmp_f("ret", NEW_MATH_GetCos(f), BASE_MATH_GetCos(f));
        site = "GetSin";  cmp_f("ret", NEW_MATH_GetSin(f), BASE_MATH_GetSin(f));
    }
    for (i = 1; i <= 4000; i++) {
        float f = i * 0.25f;
        site = "GetSqrt";
        cmp_f("ret", NEW_MATH_GetSqrt(f), BASE_MATH_GetSqrt(f));
        site = "GetInverseSqrt";
        cmp_f("ret", NEW_MATH_GetInverseSqrt(f), BASE_MATH_GetInverseSqrt(f));
        site = "GetInverse";
        cmp_f("ret", NEW_MATH_GetInverse(f), BASE_MATH_GetInverse(f));
    }

    /* 3. scalar helpers ------------------------------------------- */
    for (k = 0; k < 400; k++) {
        float a = nextf(-100.0f, 100.0f);
        float b = nextf(-100.0f, 100.0f);
        float c = nextf(-100.0f, 100.0f);
        site = "PowLimitedCE";
        cmp_d("ret", NEW_MATH_PowLimitedCE(a), BASE_MATH_PowLimitedCE(a));
        site = "MaxOf3";
        cmp_f("ret", NEW_MATH_f_MaxOf3(a, b, c),
              BASE_float_get_max_of_3vals_sub_48BBB0(a, b, c));
        site = "MinOf3";
        cmp_f("ret", NEW_MATH_f_MinOf3(a, b, c),
              BASE_float_get_min_of_3vals_sub_48BB80(a, b, c));
        site = "LinearInterpolateFloats";
        cmp_f("ret", NEW_MATH_LinearInterpolateFloats(a, b, c),
              BASE_MATH_LinearInterpolateFloats(a, b, c));
    }
    /* the sign boundaries PowLimitedCE branches on */
    {
        static const float edge[] = { -1.0f, -0.0f, 0.0f, 1.0f, 1e-30f, -1e-30f, 8.0f, -27.0f };
        for (k = 0; k < (int)(sizeof edge / sizeof edge[0]); k++) {
            site = "PowLimitedCE edge";
            cmp_d("ret", NEW_MATH_PowLimitedCE(edge[k]),
                  BASE_MATH_PowLimitedCE(edge[k]));
        }
    }

    /* 4. vector helpers ------------------------------------------- */
    for (k = 0; k < 600; k++) {
        Vec3 a, b, c;
        float s = nextf(-10.0f, 10.0f);
        fill_vec(&a, -40.0f, 40.0f);
        fill_vec(&b, -40.0f, 40.0f);
        fill_vec(&c, -40.0f, 40.0f);

        VV("NormalizeVector", NEW_MATH_NormalizeVector(&an, &a),
                              BASE_MATH_NormalizeVector(&ab, &a));
        VV("NormalizeVectorWithNullVectorCheck",
           NEW_MATH_NormalizeVectorWithNullVectorCheck(&an, &a),
           BASE_MATH_NormalizeVectorWithNullVectorCheck(&ab, &a));
        VV("CrossProduct", NEW_MATH_CrossProduct(&an, &a, &b),
                           BASE_MATH_CrossProduct(&ab, &a, &b));
        VV("SubstractVec", NEW_MATH_SubstractVec(&an, &a, &b),
                           BASE_MATH_SubstractVec(&ab, &a, &b));
        VV("VecAddVec", NEW_MATH_VecAddVec(&an, &a, &b),
                        BASE_MATH_VecAddVec(&ab, &a, &b));
        VV("VecMultScalar", NEW_MATH_VecMultScalar(&an, &a, s),
                            BASE_MATH_VecMultScalar(&ab, &a, s));
        VV("VecMultNormScalar", NEW_MATH_VecMultNormScalar(&an, &a, s),
                                BASE_MATH_VecMultNormScalar(&ab, &a, s));
        VV("VecInvert", NEW_MATH_VecInvert(&an, &a),
                        BASE_MATH_VecInvert(&ab, &a));
        VV("VecSet", NEW_MATH_VecSet(&an, &a), BASE_MATH_VecSet(&ab, &a));
        VV("SetNormVector", NEW_MATH_SetNormVector(&an, &a, s),
                            BASE_MATH_SetNormVector(&ab, &a, s));
        VV("BlendVector", NEW_MATH_BlendVector(&an, &a, &b, s * 0.1f),
                          BASE_MATH_BlendVector(&ab, &a, &b, s * 0.1f));
        VV("AddScaleVector", NEW_MATH_AddScaleVector(&an, &a, &b, s),
                             BASE_MATH_AddScaleVector(&ab, &a, &b, s));
        VV("InterpolateBetweenTwoPosWithFactor",
           NEW_MATH_InterpolateBetweenTwoPosWithFactor(&an, &a, &b, s * 0.1f),
           BASE_MATH_InterpolateBetweenTwoPosWithFactor(&ab, &a, &b, s * 0.1f));
        VV("PointLineProjection", NEW_MATH_PointLineProjection(&an, &a, &b, &c),
                                  BASE_MATH_PointLineProjection(&ab, &a, &b, &c));
        VV("PointSegmentProjection",
           NEW_MATH_PointSegmentProjection(&an, &a, &b, &c),
           BASE_MATH_PointSegmentProjection(&ab, &a, &b, &c));
        /* VecRotate normalises the caller's axis in place, so each copy
           needs its own, and the mutated axis is compared too. */
        {
            Vec3 on, ob, axN = c, axB = c;
            site = "VecRotate";
            memset(&on, 0, sizeof on); memset(&ob, 0, sizeof ob);
            NEW_MATH_VecRotate(&on, &axN, &a, s);
            BASE_MATH_VecRotate(&ob, &axB, &a, s);
            cmp_mem("out", &on, &ob, sizeof on);
            cmp_mem("axis after", &axN, &axB, sizeof axN);
        }

        site = "VecLen";
        cmp_f("ret", NEW_MATH_VecLen(&a), BASE_MATH_VecLen(&a));
        site = "VecLenSquared";
        cmp_f("ret", NEW_MATH_VecLenSquared(&a), BASE_MATH_VecLenSquared(&a));
        site = "DotProduct";
        cmp_f("ret", NEW_MATH_DotProduct(&a, &b), BASE_MATH_DotProduct(&a, &b));
        site = "DistanceOfTwoPoints";
        cmp_f("ret", NEW_MATH_DistanceOfTwoPoints(&a, &b),
              BASE_MATH_DistanceOfTwoPoints(&a, &b));
        site = "f_VecCos";
        cmp_f("ret", NEW_MATH_f_VecCos(&a, &b), BASE_MATH_f_VecCos(&a, &b));
        site = "f_VecSin";
        cmp_f("ret", NEW_MATH_f_VecSin(&a, &b, &c),
              BASE_MATH_f_VecSin(&a, &b, &c));
        site = "f_VecSin null axis";
        cmp_f("ret", NEW_MATH_f_VecSin(&a, &b, NULL),
              BASE_MATH_f_VecSin(&a, &b, NULL));
        site = "f_VecAngle";
        cmp_f("ret", NEW_MATH_f_VecAngle(&a, &b, &c),
              BASE_MATH_f_VecAngle(&a, &b, &c));
        site = "f_ZAngle";
        cmp_f("ret", NEW_MATH_f_ZAngle(&a, &b), BASE_MATH_f_ZAngle(&a, &b));
        site = "f_XAngle";
        cmp_f("ret", NEW_MATH_f_XAngle(&a, &b), BASE_MATH_f_XAngle(&a, &b));
        site = "f_PointLineDistance";
        cmp_f("ret", NEW_MATH_f_PointLineDistance(&a, &b, &c),
              BASE_MATH_f_PointLineDistance(&a, &b, &c));
        site = "f_VecCosPlusArcCos";
        cmp_f("ret", NEW_MATH_f_VecCosPlusArcCos(&a, &b),
              BASE_MATH_f_VecCosPlusArcCos(&a, &b));
        site = "b_NulVector";
        cmp_i("ret", NEW_MATH_b_NulVector(&a), BASE_MATH_b_NulVector(&a));
        site = "b_NulVectorWithEpsilon";
        cmp_i("ret", NEW_MATH_b_NulVectorWithEpsilon(&a, 1.0f),
              BASE_MATH_b_NulVectorWithEpsilon(&a, 1.0f));
        site = "b_EqVectorWithEpsilon";
        cmp_i("ret", NEW_MATH_b_EqVectorWithEpsilon(&a, &b, 50.0f),
              BASE_MATH_b_EqVectorWithEpsilon(&a, &b, 50.0f));
        site = "b_VecColinear";
        cmp_i("ret", NEW_MATH_b_VecColinear(&a, &b),
              BASE_MATH_b_VecColinear(&a, &b));

        /* the in-place ones, each on its own copy */
        {
            Vec3 pn = a, pb = a;
            site = "NormalizeEqualVector";
            NEW_MATH_NormalizeEqualVector(&pn);
            BASE_MATH_NormalizeEqualVector(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = a; pb = a;
            site = "VecRound";
            NEW_MATH_VecRound(&pn); BASE_MATH_VecRound(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = a; pb = a;
            site = "RoundVectorWithPrecision";
            NEW_MATH_RoundVectorWithPrecision(&pn, 0.25f);
            BASE_MATH_RoundVectorWithPrecision(&pb, 0.25f);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = a; pb = a;
            site = "VecMultScalarWithoutBuffer";
            NEW_MATH_VecMultScalarWithoutBuffer(&pn, s);
            BASE_MATH_VecMultScalarWithoutBuffer(&pb, s);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = a; pb = a;
            site = "VecAddVecWithoutBuffer";
            NEW_MATH_VecAddVecWithoutBuffer(&pn, &b);
            BASE_MATH_VecAddVecWithoutBuffer(&pb, &b);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = a; pb = a;
            site = "SetNullVector";
            NEW_MATH_SetNullVector(&pn); BASE_MATH_SetNullVector(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);
        }
    }
    /* the degenerate vector every normalise path guards against */
    {
        Vec3 zero = { 0.0f, 0.0f, 0.0f };
        VV("NormalizeVectorWithNullVectorCheck zero",
           NEW_MATH_NormalizeVectorWithNullVectorCheck(&an, &zero),
           BASE_MATH_NormalizeVectorWithNullVectorCheck(&ab, &zero));
        site = "b_NulVector zero";
        cmp_i("ret", NEW_MATH_b_NulVector(&zero), BASE_MATH_b_NulVector(&zero));
        site = "VecLen zero";
        cmp_f("ret", NEW_MATH_VecLen(&zero), BASE_MATH_VecLen(&zero));
    }

    /* 5. matrices -------------------------------------------------- */
    for (k = 0; k < 400; k++) {
        Mtx4x4 m, m2;
        Vec3 v, sight, banking;
        float s = nextf(0.25f, 4.0f);
        fill_mtx(&m); fill_mtx(&m2);
        fill_vec(&v, -5.0f, 5.0f);
        fill_vec(&sight, -5.0f, 5.0f);
        fill_vec(&banking, -5.0f, 5.0f);

        MM("CopyMatrix", NEW_MATH_CopyMatrix(&an, &m),
                         BASE_MATH_CopyMatrix(&ab, &m));
        MM("Copy33Matrix", NEW_MATH_Copy33Matrix(&an, &m),
                           BASE_MATH_Copy33Matrix(&ab, &m));
        MM("Transp33MatrixWithoutBuffer",
           NEW_MATH_Transp33MatrixWithoutBuffer(&an, &m),
           BASE_MATH_Transp33MatrixWithoutBuffer(&ab, &m));
        MM("InvertMatrix", NEW_MATH_InvertMatrix(&an, &m),
                           BASE_MATH_InvertMatrix(&ab, &m));
        MM("Invert33Matrix", NEW_MATH_Invert33Matrix(&an, &m),
                             BASE_MATH_Invert33Matrix(&ab, &m));
        MM("GetRotationMatrix", NEW_MATH_GetRotationMatrix(&an, &m),
                                BASE_MATH_GetRotationMatrix(&ab, &m));
        MM("MakeOGLMatrix", NEW_MATH_MakeOGLMatrix(&an, &m),
                            BASE_MATH_MakeOGLMatrix(&ab, &m));
        MM("Mul33MatrixMatrix", NEW_MATH_Mul33MatrixMatrix(&an, &m, &m2, 1),
                                BASE_MATH_Mul33MatrixMatrix(&ab, &m, &m2, 1));
        MM("Mul33MatrixMatrix noflag",
           NEW_MATH_Mul33MatrixMatrix(&an, &m, &m2, 0),
           BASE_MATH_Mul33MatrixMatrix(&ab, &m, &m2, 0));
        MM("Mul44MatrixMatrix", NEW_MATH_Mul44MatrixMatrix(&an, &m, &m2),
                                BASE_MATH_Mul44MatrixMatrix(&ab, &m, &m2));
        MM("MulMatrixMatrix", NEW_MATH_MulMatrixMatrix(&an, &m, &m2),
                              BASE_MATH_MulMatrixMatrix(&ab, &m, &m2));
        MM("SetZoomBuffered", NEW_MATH_SetZoomBuffered(&an, &m, &v),
                              BASE_MATH_SetZoomBuffered(&ab, &m, &v));
        MM("MakeRotationMatrix_UsingSight",
           NEW_MATH_MakeRotationMatrix_UsingSight(&an, &sight, &banking, 1),
           BASE_MATH_MakeRotationMatrix_UsingSight(&ab, &sight, &banking, 1));
        MM("MakeRotationMatrix_UsingBanking",
           NEW_MATH_MakeRotationMatrix_UsingBanking(&an, &sight, &banking, 1),
           BASE_MATH_MakeRotationMatrix_UsingBanking(&ab, &sight, &banking, 1));

        VV("TransformVector", NEW_MATH_TransformVector(&an, &m, &v),
                              BASE_MATH_TransformVector(&ab, &m, &v));
        VV("TransformVertex", NEW_MATH_TransformVertex(&an, &m, &v),
                              BASE_MATH_TransformVertex(&ab, &m, &v));
        VV("TransformVectorNoScale",
           NEW_MATH_TransformVectorNoScale(&an, &m, &v),
           BASE_MATH_TransformVectorNoScale(&ab, &m, &v));
        VV("TransformVertexNoScale",
           NEW_MATH_TransformVertexNoScale(&an, &m, &v),
           BASE_MATH_TransformVertexNoScale(&ab, &m, &v));
        VV("VectorGlobalToLocal", NEW_MATH_VectorGlobalToLocal(&an, &m, &v),
                                  BASE_MATH_VectorGlobalToLocal(&ab, &m, &v));
        VV("VertexGlobalToLocal", NEW_MATH_VertexGlobalToLocal(&an, &m, &v),
                                  BASE_MATH_VertexGlobalToLocal(&ab, &m, &v));
        VV("GetScale", NEW_MATH_GetScale(&an, &m), BASE_MATH_GetScale(&ab, &m));
        VV("GetZoom", NEW_MATH_GetZoom(&an, &m), BASE_MATH_GetZoom(&ab, &m));

        site = "l_CalculateCorrectType";
        cmp_i("ret", NEW_MATH_l_CalculateCorrectType(&m),
              BASE_MATH_l_CalculateCorrectType(&m));
        site = "b_TypeIsCorrect";
        cmp_i("ret", NEW_MATH_b_TypeIsCorrect(&m),
              BASE_MATH_b_TypeIsCorrect(&m));

        /* in-place matrix ops */
        {
            Mtx4x4 pn = m, pb = m;
            site = "SetIdentityMatrix";
            NEW_MATH_SetIdentityMatrix(&pn); BASE_MATH_SetIdentityMatrix(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "TranspEq33Matrix";
            NEW_MATH_TranspEq33Matrix(&pn); BASE_MATH_TranspEq33Matrix(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "Orthonormalize";
            NEW_MATH_Orthonormalize(&pn); BASE_MATH_Orthonormalize(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "SetCorrectType";
            NEW_MATH_SetCorrectType(&pn); BASE_MATH_SetCorrectType(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "SetScale";
            NEW_MATH_SetScale(&pn, &v); BASE_MATH_SetScale(&pb, &v);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "SetZoom";
            NEW_MATH_SetZoom(&pn, s); BASE_MATH_SetZoom(&pb, s);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "OrientMatrix_UsingSight";
            NEW_MATH_OrientMatrix_UsingSight(&pn, &sight, &banking);
            BASE_MATH_OrientMatrix_UsingSight(&pb, &sight, &banking);
            cmp_mem("out", &pn, &pb, sizeof pn);

            pn = m; pb = m;
            site = "OrientMatrix_UsingBanking";
            NEW_MATH_OrientMatrix_UsingBanking(&pn, &sight, &banking);
            BASE_MATH_OrientMatrix_UsingBanking(&pb, &sight, &banking);
            cmp_mem("out", &pn, &pb, sizeof pn);
        }
        /* the six axis rotations, each from the same matrix */
        {
            float ang = nextf(-7.0f, 7.0f);
            Mtx4x4 pn, pb;
#define ROT(fn) do {                                                        \
                pn = m; pb = m; site = #fn;                                 \
                NEW_##fn(&pn, ang); BASE_##fn(&pb, ang);                    \
                cmp_mem("out", &pn, &pb, sizeof pn);                        \
            } while (0)
            ROT(MATH_RotateMatrix_AroundGlobalXAxis);
            ROT(MATH_RotateMatrix_AroundGlobalYAxis);
            ROT(MATH_RotateMatrix_AroundGlobalZAxis);
            ROT(MATH_RotateMatrix_AroundLocalXAxis);
            ROT(MATH_RotateMatrix_AroundLocalYAxis);
            ROT(MATH_RotateMatrix_AroundLocalZAxis);
#undef ROT
        }
        /* the banking build with a null sight, which skips a block */
        MM("MakeRotationMatrix_UsingBanking null sight",
           NEW_MATH_MakeRotationMatrix_UsingBanking(&an, NULL, &banking, 0),
           BASE_MATH_MakeRotationMatrix_UsingBanking(&ab, NULL, &banking, 0));
    }

    /* 6. quaternions ---------------------------------------------- */
    for (k = 0; k < 400; k++) {
        Quat4 q, q2;
        Mtx4x4 m;
        Vec3 axis;
        float f = nextf(0.0f, 1.0f);
        float ang = nextf(-7.0f, 7.0f);
        fill_quat(&q); fill_quat(&q2);
        fill_mtx(&m);
        fill_vec(&axis, -3.0f, 3.0f);

        QQ("NormQuat", NEW_MATH_NormQuat(&an, &q), BASE_MATH_NormQuat(&ab, &q));
        QQ("InvNormalQuaternion", NEW_MATH_InvNormalQuaternion(&an, &q),
                                  BASE_MATH_InvNormalQuaternion(&ab, &q));
        QQ("ConvertMatrixToQuaternion",
           NEW_MATH_ConvertMatrixToQuaternion(&an, &m),
           BASE_MATH_ConvertMatrixToQuaternion(&ab, &m));
        MM("ConvertQuaternionToMatrix",
           NEW_MATH_ConvertQuaternionToMatrix(&an, &q),
           BASE_MATH_ConvertQuaternionToMatrix(&ab, &q));

        {
            Quat4 pn = q, pb = q;
            site = "InvertQuaternion";
            NEW_MATH_InvertQuaternion(&pn); BASE_MATH_InvertQuaternion(&pb);
            cmp_mem("out", &pn, &pb, sizeof pn);
        }
        /* Slerp, both the theta == 0 shortcut and the real path */
        {
            Quat4 on, ob;
            site = "Slerp";
            NEW_MATH_Slerp(&on, f, &q, &q2, 0.9f, 1.0f / sinf(0.9f));
            BASE_MATH_Slerp(&ob, f, &q, &q2, 0.9f, 1.0f / sinf(0.9f));
            cmp_mem("out", &on, &ob, sizeof on);
            site = "Slerp theta zero";
            NEW_MATH_Slerp(&on, f, &q, &q2, 0.0f, 0.0f);
            BASE_MATH_Slerp(&ob, f, &q, &q2, 0.0f, 0.0f);
            cmp_mem("out", &on, &ob, sizeof on);
        }
        /* AxisAngle writes a matrix and optionally a quaternion */
        {
            Mtx4x4 mn, mb;
            Quat4 qn, qb;
            memset(&mn, 0, sizeof mn); memset(&mb, 0, sizeof mb);
            memset(&qn, 0, sizeof qn); memset(&qb, 0, sizeof qb);
            site = "MakeRotationMatrix_AxisAngle";
            NEW_MATH_MakeRotationMatrix_AxisAngle(&mn, &axis, ang, &qn, 1);
            BASE_MATH_MakeRotationMatrix_AxisAngle(&mb, &axis, ang, &qb, 1);
            cmp_mem("mtx", &mn, &mb, sizeof mn);
            cmp_mem("quat", &qn, &qb, sizeof qn);
            mn = m; mb = m;
            site = "MakeRotationMatrix_AxisAngle no quat, keep translation";
            NEW_MATH_MakeRotationMatrix_AxisAngle(&mn, &axis, ang, NULL, 0);
            BASE_MATH_MakeRotationMatrix_AxisAngle(&mb, &axis, ang, NULL, 0);
            cmp_mem("mtx", &mn, &mb, sizeof mn);
        }
        /* the compressed-quaternion codec and the five delta variants */
        {
            CompressedQuat c;
            UltraCompressedQuat u;
            Quat4 dn, db;

            site = "CompressQuaternion";
            {
                CompressedQuat cn, cb;
                memset(&cn, 0, sizeof cn); memset(&cb, 0, sizeof cb);
                NEW_MATH_CompressQuaternion(&cn, &q);
                BASE_MATH_CompressQuaternion(&cb, &q);
                cmp_mem("out", &cn, &cb, sizeof cn);
                c = cn;
            }
            site = "DecompressQuaternion";
            NEW_MATH_DecompressQuaternion(&dn, &c);
            BASE_MATH_DecompressQuaternion(&db, &c);
            cmp_mem("out", &dn, &db, sizeof dn);

            u.s1 = (int16_t)nextu();
            u.b2 = (int8_t)nextu();
            u.b3 = (int8_t)nextu();

            dn = q; db = q;
            site = "calc_curr_quat_from_dt_800";
            NEW_MATH_calc_curr_quat_from_dt_800(&dn, &c);
            BASE_MATH_calc_curr_quat_from_dt_800(&db, &c);
            cmp_mem("out", &dn, &db, sizeof dn);

            dn = q; db = q;
            site = "calc_curr_quat_from_dt_1000";
            NEW_MATH_calc_curr_quat_from_dt_1000(&dn, &c);
            BASE_MATH_calc_curr_quat_from_dt_1000(&db, &c);
            cmp_mem("out", &dn, &db, sizeof dn);

            dn = q; db = q;
            site = "calc_curr_quat_from_dt_2000";
            NEW_MATH_calc_curr_quat_from_dt_2000(&dn, &u);
            BASE_MATH_calc_curr_quat_from_dt_2000(&db, &u);
            cmp_mem("out", &dn, &db, sizeof dn);

            dn = q; db = q;
            site = "calc_curr_quat_from_dt_4000";
            NEW_MATH_calc_curr_quat_from_dt_4000(&dn, &u);
            BASE_MATH_calc_curr_quat_from_dt_4000(&db, &u);
            cmp_mem("out", &dn, &db, sizeof dn);

            dn = q; db = q;
            site = "calc_curr_quat_from_dt_8000";
            NEW_MATH_calc_curr_quat_from_dt_8000(&dn, &u);
            BASE_MATH_calc_curr_quat_from_dt_8000(&db, &u);
            cmp_mem("out", &dn, &db, sizeof dn);
        }
        /* the two blends, including the cached-endpoint path. Each copy
           keeps its own cache, so the cached call is driven right after
           the uncached one that filled it. */
        {
            Mtx4x4 m2, mn, mb;
            fill_mtx(&m2);
            site = "MatrixBlend recompute";
            NEW_MATH_MatrixBlend(&mn, &m, &m2, f, 1);
            BASE_MATH_MatrixBlend(&mb, &m, &m2, f, 1);
            cmp_mem("out", &mn, &mb, sizeof mn);
            site = "MatrixBlend cached";
            NEW_MATH_MatrixBlend(&mn, &m, &m2, 1.0f - f, 0);
            BASE_MATH_MatrixBlend(&mb, &m, &m2, 1.0f - f, 0);
            cmp_mem("out", &mn, &mb, sizeof mn);

            /* QuaternionBlend inverts its second argument in place, so
               each copy gets its own */
            {
                Quat4 an2 = q2, ab2 = q2;
                site = "QuaternionBlend recompute";
                NEW_MATH_QuaternionBlend(&mn, &q, &an2, f, 1);
                BASE_MATH_QuaternionBlend(&mb, &q, &ab2, f, 1);
                cmp_mem("out", &mn, &mb, sizeof mn);
                cmp_mem("src2 after", &an2, &ab2, sizeof an2);
                site = "QuaternionBlend defaults";
                NEW_MATH_QuaternionBlend(&mn, &q, &an2, f, 0);
                BASE_MATH_QuaternionBlend(&mb, &q, &ab2, f, 0);
                cmp_mem("out", &mn, &mb, sizeof mn);
            }
        }
    }

    /* 7. the cubic solver and the two parabola projections --------
          driven over all four of the solver's branches ------------ */
    for (k = 0; k < 4000; k++) {
        Vec3 rn, rb;
        float a = nextf(-20.0f, 20.0f);
        float b = nextf(-20.0f, 20.0f);
        float c = nextf(-20.0f, 20.0f);
        int retn, retb;
        site = "i_SolveCubicEquation";
        memset(&rn, 0, sizeof rn); memset(&rb, 0, sizeof rb);
        retn = NEW_MATH_i_SolveCubicEquation(a, b, c, &rn);
        retb = BASE_MATH_i_SolveCubicEquation(a, b, c, &rb);
        cmp_i("ret", retn, retb);
        cmp_mem("roots", &rn, &rb, sizeof rn);
    }
    /* the triple-root and double-root corners, which random inputs miss */
    {
        static const float corner[][3] = {
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, -3.0f, 2.0f },
            { 0.0f, -3.0f, -2.0f },
            { 3.0f, 3.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f },
        };
        for (k = 0; k < (int)(sizeof corner / sizeof corner[0]); k++) {
            Vec3 rn, rb;
            int retn, retb;
            site = "i_SolveCubicEquation corner";
            memset(&rn, 0, sizeof rn); memset(&rb, 0, sizeof rb);
            retn = NEW_MATH_i_SolveCubicEquation(corner[k][0], corner[k][1],
                                                 corner[k][2], &rn);
            retb = BASE_MATH_i_SolveCubicEquation(corner[k][0], corner[k][1],
                                                  corner[k][2], &rb);
            cmp_i("ret", retn, retb);
            cmp_mem("roots", &rn, &rb, sizeof rn);
        }
    }
    for (k = 0; k < 2000; k++) {
        Vec3 p, refA, refB, origin;
        float curv = nextf(-3.0f, 3.0f);
        float h = nextf(-10.0f, 10.0f);
        float t = nextf(0.1f, 5.0f);
        fill_vec(&p, -30.0f, 30.0f);
        fill_vec(&refA, -8.0f, 8.0f);
        fill_vec(&refB, -8.0f, 8.0f);
        fill_vec(&origin, -20.0f, 20.0f);

        VV("PointParabolProjection2D",
           NEW_MATH_PointParabolProjection2D(&an, &p, curv, h),
           BASE_MATH_PointParabolProjection2D(&ab, &p, curv, h));
        /* the degenerate curvature that takes the early exit */
        VV("PointParabolProjection2D flat",
           NEW_MATH_PointParabolProjection2D(&an, &p, 0.0f, h),
           BASE_MATH_PointParabolProjection2D(&ab, &p, 0.0f, h));
        {
            /* the 3D form normalises its refA/refB in place through the
               frame build, so each copy gets its own inputs */
            Vec3 dn, db, tn, tb, aN = refA, aB = refA, bN = refB, bB = refB;
            site = "PointParabolProjection3D";
            memset(&dn, 0, sizeof dn); memset(&db, 0, sizeof db);
            memset(&tn, 0, sizeof tn); memset(&tb, 0, sizeof tb);
            NEW_MATH_PointParabolProjection3D(&dn, &tn, &p, &aN, &bN, &origin, t);
            BASE_MATH_PointParabolProjection3D(&db, &tb, &p, &aB, &bB, &origin, t);
            cmp_mem("dest", &dn, &db, sizeof dn);
            cmp_mem("tangent", &tn, &tb, sizeof tn);
        }
    }

    /* 8. the random helpers, each copy from the same seed ---------- */
    for (k = 0; k < 200; k++) {
        float lo = nextf(-50.0f, 0.0f);
        float hi = nextf(0.0f, 50.0f);
        float rn, rb;
        site = "RandFloat";
        srand(1234 + k); rn = NEW_MATH_RandFloat();
        srand(1234 + k); rb = BASE_MATH_RandFloat();
        cmp_f("ret", rn, rb);
        site = "RandFloatRange";
        srand(4321 + k); rn = NEW_MATH_RandFloatRange(lo, hi);
        srand(4321 + k); rb = BASE_MATH_RandFloatRange(lo, hi);
        cmp_f("ret", rn, rb);
    }

    printf("  %ld comparisons\n", cases);
    if (diffs == 0)
        printf("  every value bit-identical\n");
    else
        printf("  %ld DIFFERING\n", diffs);
    return diffs != 0;
}
