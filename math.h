#pragma once
#include "common.h"

#define MATH_f_PiBy4			0.7853981633974482f
#define MATH_f_PiBy2			1.570796326794896f
#define MATH_f_Pi				3.141592653589793f
#define MATH_f_2Pi				6.283185307179586f

typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;

typedef struct Mtx4x4
{
    Vec3 v1;
    float _m14;
    Vec3 v2;
    float _m24;
    Vec3 v3;
    float _m34;
    Vec3 Pos;
    float w;
    uint32_t mtxflag;
    int32_t unk1;
    int32_t unk2;
    int32_t unk3;
} Mtx4x4;

typedef struct Mtx4x4WithoutAdditionalData
{
    float f11;
    float f12;
    float f13;
    float f14;
    float f21;
    float f22;
    float f23;
    float f24;
    float f31;
    float f32;
    float f33;
    float f34;
    float f41;
    float f42;
    float f43;
    float f44;
} Mtx4x4WithoutAdditionalData;

typedef struct Quat4
{
    float q1;
    float q2;
    float q3;
    float q4;
} Quat4;

typedef struct CompressedQuat
{
    int16_t s1;
    int16_t s2;
    int16_t s3;
} CompressedQuat;

typedef struct UltraCompressedQuat
{
    int16_t s1;
    int8_t b2;
    int8_t b3;
} UltraCompressedQuat;

typedef struct sMatrixStackStruct
{
    Mtx4x4* elems;
    Mtx4x4* actMatrixElem;
    uint16_t MaxElemNum;
    uint16_t currElemNum;
} tMatrixStackStruct;


float MATH_gf_1024by2Pi;
double MATH_gd_magicDoubleForRound;
Mtx4x4 MATH_gst_IdentityMatrix;
Mtx4x4 MATH_gstUnknownMatrix;
Vec3 MATH_gst_IdentityVector;
Vec3 MATH_gst_BaseVectorI;
Vec3 MATH_gst_BaseVectorJ;
Vec3 MATH_gst_BaseVectorK;
Vec3 MATH_gst_NulVector;
float MATH_gf_TableSin[1024];
int MATH_gl_TableRacine[2048];
int MATH_gl_TableInverse[1024];
int MATH_gl_TableInverseRacine[2048];
//float f_RandomTableLastElem_unk_9A0620[256]; //protex & spg
Quat4 MATH_gst_BlendQuatFrom;
Quat4 MATH_gst_BlendQuatTo;
float MATH_gf_BlendTheta;
float MATH_gf_BlendInvSinTheta;

void MATH_InitModule();
void MATH_SubstractVec(Vec3* dest, Vec3* src1, Vec3* src2);
float MATH_VecLen(Vec3* v);
BOOL MATH_b_NulVector(Vec3* v);
BOOL MATH_b_NulVector(Vec3* v);
BOOL MATH_b_NulVectorWithEpsilon(Vec3* v, float epsilon);
void MATH_NormalizeVector(Vec3* dest, Vec3* src);
void MATH_NormalizeEqualVector(Vec3* v);
void MATH_CrossProduct(Vec3* dest, Vec3* src1, Vec3* src2);
float MATH_f_PointLineDistance(Vec3* point, Vec3* lineFrom, Vec3* lineTo);
void MATH_PointLineProjection(Vec3* destProjection, Vec3* point, Vec3* line1, Vec3* line2);
void MATH_PointSegmentProjection(Vec3* dest, Vec3* point, Vec3* line1, Vec3* line2);
void MATH_PointParabolProjection3D(Vec3* dest, Vec3* tangent, Vec3* point, Vec3* refA, Vec3* refB, Vec3* origin, float time);
void MATH_MatrixBlend(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2, float factor, BOOL dontUseStoredValues);
void MATH_TransformVectorNoScale(Vec3* dest, Mtx4x4* mtx, Vec3* src);
void MATH_BlendVector(Vec3* dest, Vec3* from, Vec3* to, float amount);
void MATH_VecAddVec(Vec3* dest, Vec3* src1, Vec3* src2);
void MATH_RoundVectorWithPrecision(Vec3* vec, float precision);
float MATH_f_VecCos(Vec3* vec1, Vec3* vec2);
float MATH_f_VecSin(Vec3* vec1, Vec3* vec2, Vec3* axis);
float MATH_f_VecAngle(Vec3* vec1, Vec3* vec2, Vec3* axis);
float MATH_LinearInterpolateFloats(float f1, float f2, float factor);
void MATH_MakeRotationMatrix_AxisAngle(Mtx4x4* mtx, Vec3* axis, float angle, Quat4* quatOut, char resetTranslation);
void MATH_TransformVector(Vec3* dest, Mtx4x4* mtx, Vec3* src);
float MATH_VecLenSquared(Vec3* vec);
void MATH_MulMatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2);
void MATH_TransformVertexNoScale(Vec3* dest, Mtx4x4* mtx, Vec3* src);
void MATH_OrientMatrix_UsingSight(Mtx4x4* mtx, Vec3* sight, Vec3* banking);
void MATH_OrientMatrix_UsingBanking(Mtx4x4* mtx, Vec3* sight, Vec3* banking);
void MATH_GetScale(Vec3* scale, Mtx4x4* mtx);
void MATH_SetZoom(Mtx4x4* mtx, float zoom);
void MATH_InvertMatrix(Mtx4x4* dest, Mtx4x4* src);
void MATH_SetIdentityMatrix(Mtx4x4* mtx);
void MATH_CopyMatrix(Mtx4x4* dest, Mtx4x4* src);
BOOL MATH_b_EqVectorWithEpsilon(Vec3* vec1, Vec3* vec2, float epsilon);
void MATH_Orthonormalize(Mtx4x4* mtx);
void MATH_SetNormVector(Vec3* dest, Vec3* src, float norm);
void MATH_SetNullVector(Vec3* vec);
Mtx4x4* MATH_Matrix_AllocArray(int num);
Mtx4x4* MATH_Matrix_ReallocArray(Mtx4x4* oldMtxPtr, int num);
void MATH_Matrix_Free(Mtx4x4* mtxPtr);
void MATH_ConvertQuaternionToMatrix(Mtx4x4* mtx, Quat4* q);
void MATH_ConvertMatrixToQuaternion(Quat4* dest, Mtx4x4* src);
void MATH_QuaternionBlend(Mtx4x4* dest, Quat4* src1, Quat4* src2, float factor, BOOL dontUseDefaultValues);
void MATH_SetCorrectType(Mtx4x4* mtx);
void MATH_TransformVertex(Vec3* dest, Mtx4x4* mtx, Vec3* src);
void MATH_Mul33MatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2, char flagSet);
void MATH_Copy33Matrix(Mtx4x4* dest, Mtx4x4* src);
void MATH_VecAddVecWithoutBuffer(Vec3* dest, Vec3* src);
void MATH_VecMultScalar(Vec3* dest, Vec3* src, float scalar);
void MATH_VecInvert(Vec3* dest, Vec3* src);
Mtx4x4* MATH_Matrix_Alloc();
void MATH_RotateMatrix_AroundLocalXAxis(Mtx4x4* mtx, float angle);
void MATH_RotateMatrix_AroundLocalYAxis(Mtx4x4* mtx, float angle);
void MATH_RotateMatrix_AroundLocalZAxis(Mtx4x4* mtx, float angle);
void MATH_RotateMatrix_AroundGlobalXAxis(Mtx4x4* mtx, float angle);
void MATH_RotateMatrix_AroundGlobalYAxis(Mtx4x4* mtx, float angle);
void MATH_RotateMatrix_AroundGlobalZAxis(Mtx4x4* mtx, float angle);
void MATH_SetScale(Mtx4x4* mtx, Vec3* scale);
float MATH_f_MaxOf3(float a, float b, float c);
float MATH_f_ZAngle(Vec3* vec1, Vec3* vec2);
float MATH_f_XAngle(Vec3* vec1, Vec3* vec2);
void MATH_GetRotationMatrix(Mtx4x4* dest, Mtx4x4* src);
int MATH_l_CalculateCorrectType(Mtx4x4* mtx);
void MATH_Transp33MatrixWithoutBuffer(Mtx4x4* dest, Mtx4x4* src);
void MATH_VectorGlobalToLocal(Vec3* dest, Mtx4x4* mtx, Vec3* src);
BOOL MATH_b_VecColinear(Vec3* vec1, Vec3* vec2);
void MATH_InterpolateBetweenTwoPosWithFactor(Vec3* dest, Vec3* src1, Vec3* src2, float amount);
void MATH_MakeRotationMatrix_UsingBanking(Mtx4x4* mtx, Vec3* sight, Vec3* banking, char onlyRotation);
void MATH_VecSet(Vec3* dest, Vec3* src);
float MATH_f_MinOf3(float a, float b, float c);
void MATH_MakeOGLMatrix(Mtx4x4* dest, Mtx4x4* src);
void MATH_Invert33Matrix(Mtx4x4* dest, Mtx4x4* src);
void MATH_VecRotate(Vec3* dest, Vec3* axis, Vec3* src, float angle);
BOOL MATH_b_TypeIsCorrect(Mtx4x4* mtx);
float MATH_GetSin(float f);
float MATH_GetCos(float f);
float MATH_GetInverseSqrt(float f);
void MATH_AddScaleVector(Vec3* dest, Vec3* src1, Vec3* src2, float scale);
void MATH_VecMultScalarWithoutBuffer(Vec3* vec, float scalar);
float MATH_DistanceOfTwoPoints(Vec3* p1, Vec3* p2);
float MATH_GetSqrt(float f);
void MATH_NormalizeVectorWithNullVectorCheck(Vec3* dest, Vec3* src);
void MATH_TranspEq33Matrix(Mtx4x4* mtx);
void MATH_MakeRotationMatrix_UsingSight(Mtx4x4* dest, Vec3* sight, Vec3* banking, char onlyRotation);
void MATH_CompressQuaternion(CompressedQuat* compressedQuat, Quat4* quat);
void MATH_DecompressQuaternion(Quat4* quat, CompressedQuat* compressedQuat);
void MATH_calc_curr_quat_from_dt_800(Quat4* quat, CompressedQuat* delta);
void MATH_calc_curr_quat_from_dt_1000(Quat4* quat, CompressedQuat* delta);
void MATH_calc_curr_quat_from_dt_2000(Quat4* quat, UltraCompressedQuat* delta);
void MATH_calc_curr_quat_from_dt_4000(Quat4* quat, UltraCompressedQuat* delta);
void MATH_calc_curr_quat_from_dt_8000(Quat4* quat, UltraCompressedQuat* delta);
void MATH_VecMultNormScalar(Vec3* dest, Vec3* src, float scalarToBeNormalized);
float MATH_f_VecCosPlusArcCos(Vec3* vec1, Vec3* vec2);
float MATH_GetInverse(float f);
float MATH_RandFloat();
float MATH_RandFloatRange(float min, float max);
float MATH_DotProduct(Vec3* vec1, Vec3* vec2);

// x86 32 bit hack
inline uint32_t f2iMagic(float f) {

    // Érdekesség: ez miért van?
    // A WATERFFT_Turn_Spectre fügtgvényben volt egy olyan alkalmazása, hogy timeElapseda * src->spectroMap[i].imaginary + magicFloatVal_dbl_B0C590 / 2.0; 
    // (ki volt számolva az osztás, szóval az értéke volt odaírva). mint kiderült ez ugyanaz, mint ha azt írnám, hogy
    // 2 * timeElapseda * src->spectroMap[i].imaginary + magicFloatVal_dbl_B0C590; (zárójelekkel nincs probléma és ez matematikai képtelenség, még is mûködik)

    double d = f + MATH_gd_magicDoubleForRound;
    return *(uint32_t*)&d;
}