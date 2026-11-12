#include "../ai.h"
#include "ai_mathfunctions.h"

Mtx4x4 st_M1_459;
Mtx4x4 st_M2_460;

float AI_gf_Epsilon = 0.000099999997f;

int AI_EvalFunc_MATHBitSet_C(int value, char bit)
{
    return (1 << bit) | value;
}

tAINode* AI_EvalFunc_MATHBitSet(tAINode* node)
{
    int value;
    char bit;

    bit = AI_GetIntFromStack();
    value = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATHBitSet_C(value, bit));
    return node + 1;
}

int AI_EvalFunc_MATHBitChange_C(int value, char bit)
{
    if (((1 << bit) & value) != 0)
        return ~(1 << bit) & value;
    return (1 << bit) | value;
}

tAINode* AI_EvalFunc_MATHBitChange(tAINode* node)
{
    int value;
    char bit;

    bit = AI_GetIntFromStack();
    value = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATHBitChange_C(value, bit));
    return node + 1;
}

int AI_EvalFunc_MATHBitReset_C(int value, char bit)
{
    return ~(1 << bit) & value;
}

tAINode* AI_EvalFunc_MATHBitReset(tAINode* node)
{
    int value;
    char bit;

    bit = AI_GetIntFromStack();
    value = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATHBitReset_C(value, bit));
    return node + 1;
}

BOOL AI_EvalFunc_MATHBitTest_C(int value, char bit)
{
    return ((1 << bit) & value) != 0;
}

tAINode* AI_EvalFunc_MATHBitTest(tAINode* node)
{
    int value;
    char bit;

    bit = AI_GetIntFromStack();
    value = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATHBitTest_C(value, bit));
    return node + 1;
}

float AI_EvalFunc_MATHSinus_C(float angle) {
    return sinf(angle);
}

tAINode* AI_EvalFunc_MATHSinus(tAINode* node)
{
    float angle;

    angle = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MATHSinus_C(angle));
    return node + 1;
}

float AI_EvalFunc_MATHCosinus_C(float angle) {
    return cosf(angle);
}

tAINode* AI_EvalFunc_MATHCosinus(tAINode* node)
{
    float angle;

    angle = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MATHCosinus_C(angle));
    return node + 1;
}

// 32767 is the generator's range, so the quotient is the unit interval.
float AI_EvalFunc_MATHRandFloat_C(float min, float max)
{
    float raw;
    float unit;

    if (min == max)
        return min;

    raw = (float)rand();
    unit = (float)(raw / 32767.0);
    return (float)((max - min) * unit + min);
}

tAINode* AI_EvalFunc_MATHRandFloat(tAINode* node)
{
    float min;
    float max;

    max = AI_GetFloatFromStack();
    min = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MATHRandFloat_C(min, max));
    return node + 1;
}

// Half-open: a draw that lands exactly on max is pulled back one.
int AI_EvalFunc_MATHRandInt_C(int min, int max)
{
    int drawn;

    if (min == max)
        return min;

    drawn = min + (max - min) * RND_JADErand() / 0x7FFF;
    if (drawn == max)
        return drawn - 1;
    return drawn;
}

tAINode* AI_EvalFunc_MATHRandInt(tAINode* node)
{
    int min;
    int max;

    max = AI_GetIntFromStack();
    min = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATHRandInt_C(min, max));
    return node + 1;
}

float AI_EvalFunc_MATHAbsFloat_C(float f) {
    return (float)fabs(f);
}

tAINode* AI_EvalFunc_MATHAbsFloat(tAINode* node)
{
    float f;

    f = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MATHAbsFloat_C(f));
    return node + 1;
}

tAINode* AI_EvalFunc_MATHAbsInt(tAINode* node)
{
    int value;

    value = AI_GetIntFromStack();
    if (value < 0)
        AI_PutIntToStack(-value);
    else
        AI_PutIntToStack(value);
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecDistance(tAINode* node)
{
    Vec3* pos2;
    Vec3* pos1;
    Vec3 diff;

    // the vectors are read where the stack keeps them rather than
    // copied out
    pos2 = (Vec3*)AI_PopStackSlot();
    pos1 = (Vec3*)AI_PopStackSlot();
    MATH_SubstractVec(&diff, pos2, pos1);
    AI_PutFloatToStack(MATH_VecLen(&diff));
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecNorm(tAINode* node)
{
    AI_PutFloatToStack(MATH_VecLen((Vec3*)AI_PopStackSlot()));
    return node + 1;
}

void AI_EvalFunc_MATHVecNormalize_C(Vec3* src, Vec3* dest)
{    
    if (MATH_b_NulVector(src))
    {
        dest->x = 1.0f;
        dest->y = 0.0f;
        dest->z = 0.0f;
    }
    else
    {
        MATH_NormalizeVector(dest, src);
    }
}

tAINode* AI_EvalFunc_MATHVecNormalize(tAINode* node)
{
    Vec3 normalized;

    AI_EvalFunc_MATHVecNormalize_C((Vec3*)AI_PopStackSlot(), &normalized);
    AI_PutVecToStack(&normalized);
    return node + 1;
}

void AI_EvalFunc_MATHVecSetNormVector_C(Vec3* vec, float length)
{
    MATH_NormalizeVector(vec, vec);
    MATH_VecMultScalarWithoutBuffer(vec, length);
}

tAINode* AI_EvalFunc_MATHVecSetNormVector(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    float length;

    length = AI_GetFloatFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_EvalFunc_MATHVecSetNormVector_C((Vec3*)var.ptr, length);
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecSetNormalize(tAINode* node)
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    MATH_NormalizeEqualVector((Vec3*)var.ptr);
    return node + 1;
}

void  AI_EvalFunc_MATHVecSetHorzNormalize_C(Vec3* v)
{
    v->z = 0.0f;

    if (MATH_b_NulVector(v))
    {
        v->x = 1.0f;
        v->y = 0.0f;
        v->z = 0.0f;
    }
    else
    {
        MATH_NormalizeEqualVector(v);
    }
}

tAINode* AI_EvalFunc_MATHVecSetHorzNormalize(tAINode* node)
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    AI_EvalFunc_MATHVecSetHorzNormalize_C((Vec3*)var.ptr);
    return node + 1;
}

float AI_EvalFunc_MATHVecDotProduct_C(Vec3* vec1, Vec3* vec2) {
    return MATH_DotProduct(vec1, vec2);
}

tAINode* AI_EvalFunc_MATHVecDotProduct(tAINode* node)
{
    Vec3* vec1;
    Vec3* vec2;

    vec1 = (Vec3*)AI_PopStackSlot();
    vec2 = (Vec3*)AI_PopStackSlot();
    AI_PutFloatToStack(AI_EvalFunc_MATHVecDotProduct_C(vec1, vec2));
    return node + 1;
}

void AI_EvalFunc_MATHVecCrossProduct_C(Vec3* vec1, Vec3* vec2, Vec3* dest)
{
    MATH_CrossProduct(dest, vec1, vec2);
}

tAINode* AI_EvalFunc_MATHVecCrossProduct(tAINode* node)
{
    Vec3* vec2;
    Vec3 cross;

    vec2 = (Vec3*)AI_PopStackSlot();
    AI_EvalFunc_MATHVecCrossProduct_C((Vec3*)AI_PopStackSlot(), vec2, &cross);
    AI_PutVecToStack(&cross);
    return node + 1;
}

tAINode* AI_EvalFunc_MATHDistPointLine(tAINode* node)
{
    Vec3 point;
    Vec3 lineB;
    Vec3 lineA;

    AI_GetVecFromStack(&point);
    AI_GetVecFromStack(&lineB);
    AI_GetVecFromStack(&lineA);
    AI_PutFloatToStack(MATH_f_PointLineDistance(&lineA, &lineB, &point));
    return node + 1;
}

void AI_EvalFunc_MATHProjPointLine_C(Vec3* lineA, Vec3* lineB, Vec3* point, Vec3* dest)
{
    MATH_PointLineProjection(dest, lineA, lineB, point);
}

tAINode* AI_EvalFunc_MATHProjPointLine(tAINode* node)
{
    Vec3 lineA;
    Vec3 lineB;
    Vec3 point;
    Vec3 projected;

    AI_GetVecFromStack(&point);
    AI_GetVecFromStack(&lineB);
    AI_GetVecFromStack(&lineA);
    AI_EvalFunc_MATHProjPointLine_C(&lineA, &lineB, &point, &projected);
    AI_PutVecToStack(&projected);
    return node + 1;
}

void AI_EvalFunc_MATHProjPointSegment_C(Vec3* segA, Vec3* segB, Vec3* point, Vec3* dest)
{
    MATH_PointSegmentProjection(dest, segA, segB, point);
}

tAINode* AI_EvalFunc_MATHProjPointSegment(tAINode* node)
{
    Vec3 segA;
    Vec3 segB;
    Vec3 point;
    Vec3 projected;

    AI_GetVecFromStack(&point);
    AI_GetVecFromStack(&segB);
    AI_GetVecFromStack(&segA);
    AI_EvalFunc_MATHProjPointSegment_C(&segA, &segB, &point, &projected);
    AI_PutVecToStack(&projected);
    return node + 1;
}

// The projection's second output is taken and discarded.
void AI_EvalFunc_MATHProjPointParabol_C(Vec3* from, Vec3* speed, Vec3* gravity, Vec3* point, float time, Vec3* dest)
{
    Vec3 unused;

    MATH_PointParabolProjection3D(dest, &unused, from, speed, gravity, point, time);
}

tAINode* AI_EvalFunc_MATHProjPointParabol(tAINode* node)
{
    Vec3 from;
    Vec3 speed;
    Vec3 gravity;
    Vec3 point;
    Vec3 projected;
    float time;

    time = AI_GetFloatFromStack();
    AI_GetVecFromStack(&point);
    AI_GetVecFromStack(&gravity);
    AI_GetVecFromStack(&speed);
    AI_GetVecFromStack(&from);
    AI_EvalFunc_MATHProjPointParabol_C(&from, &speed, &gravity, &point, time, &projected);
    AI_PutVecToStack(&projected);
    return node + 1;
}

// Blends between two orientations, each given as an up and a forward
// vector, and hands back the blended pair. The two matrices are file
// statics because MATH_MatrixBlend can be asked to carry state between
// calls; rebuild says to fill them from the arguments first, which is
// what the only caller here does.
void AI_EvalFunc_MATHMatrixBlend_C(Vec3* upOut, Vec3* forwardOut, Vec3* up1, Vec3* forward1, Vec3* up2, Vec3* forward2, float amount, char rebuild)
{
    Mtx4x4 blended;

    if (amount < 0.0)
        amount = 0.0;
    if (amount > 1.0)
        amount = 1.0;

    if (rebuild)
    {
        MATH_CrossProduct(&st_M1_459.v1, forward1, up1);
        MATH_CrossProduct(&st_M2_460.v1, forward2, up2);
        st_M2_460.v3.x = forward2->x; // TODO: atnezni megegyszer
        st_M2_460.v3.y = forward2->y;
        st_M2_460.v3.z = forward2->z;

        st_M1_459.v3.x = forward1->x;
        st_M1_459.v3.y = forward1->y;
        st_M1_459.v3.z = forward1->z;

        MATH_VecInvert(&st_M2_460.v2, up2);
        MATH_VecInvert(&st_M1_459.v2, up1);
    }

    MATH_MatrixBlend(&blended, &st_M1_459, &st_M2_460, amount, rebuild);
    MATH_VecInvert(upOut, &blended.v2);
    *forwardOut = blended.v3;
}

tAINode* AI_EvalFunc_MATHMatrixBlend(tAINode* node)
{
    char upConverted[52];
    char forwardConverted[52];
    tVarStackElem upVar;
    tVarStackElem forwardVar;
    Vec3 up1;
    Vec3 forward1;
    Vec3 up2;
    Vec3 forward2;
    float amount;

    amount = AI_GetFloatFromStack();
    AI_GetVecFromStack(&forward2);
    AI_GetVecFromStack(&up2);
    AI_GetVecFromStack(&forward1);
    AI_GetVecFromStack(&up1);
    AI_GetVarFromStack(forwardConverted, &forwardVar);
    AI_GetVarFromStack(upConverted, &upVar);
    AI_EvalFunc_MATHMatrixBlend_C((Vec3*)upVar.ptr, (Vec3*)forwardVar.ptr,
                                  &up1, &forward1, &up2, &forward2, amount, 1);
    return node + 1;
}

// The same orientation convention as MATHMatrixBlend: right is forward
// crossed with up, and the matrix's second axis is the negated up.
void AI_EvalFunc_MATHMatrixVecLocalToGlobal_C(Vec3* dest, Vec3* src, Vec3* up, Vec3* forward)
{
    Mtx4x4 mtx;

    MATH_CrossProduct(&mtx.v1, forward, up);
    mtx.v3 = *forward;
    MATH_VecInvert(&mtx.v2, up);
    MATH_TransformVectorNoScale(dest, &mtx, src);
}

tAINode* AI_EvalFunc_MATHMatrixVecLocalToGlobal(tAINode* node)
{
    Vec3 src;
    Vec3 up;
    Vec3 forward;
    Vec3 global;

    AI_GetVecFromStack(&forward);
    AI_GetVecFromStack(&up);
    AI_GetVecFromStack(&src);
    AI_EvalFunc_MATHMatrixVecLocalToGlobal_C(&global, &src, &up, &forward);
    AI_PutVecToStack(&global);
    return node + 1;
}

float AI_EvalFunc_MATHFloatLimit_C(float num, float min, float max) {

    if (num <= max)
    {
        if (num >= min)
            return num;
        else
            return min;
    }
    else
    {
        return max;
    }
}

tAINode* AI_EvalFunc_MATHFloatLimit(tAINode* node)
{
    float num, min, max;

    max = AI_GetFloatFromStack();
    min = AI_GetFloatFromStack();
    num = AI_GetFloatFromStack();

    AI_PutFloatToStack(AI_EvalFunc_MATHFloatLimit_C(num, min, max));

    return node + 1;
}

float AI_EvalFunc_MATHFloatMin_C(float num1, float num2) {
    if (num1 > num2)
        return num2;
    else
        return num1;
}

tAINode* AI_EvalFunc_MATHFloatMin(tAINode* node)
{
    float num1, num2;

    num1 = AI_GetFloatFromStack();
    num2 = AI_GetFloatFromStack();
    
    AI_PutFloatToStack(AI_EvalFunc_MATHFloatMin_C(num1, num2));

    return node + 1;
}

float AI_EvalFunc_MATHFloatMax_C(float num1, float num2) {
    if (num2 < num1)
        return num1;
    else
        return num2;
}

tAINode* AI_EvalFunc_MATHFloatMax(tAINode* node)
{
    float num1, num2;

    num1 = AI_GetFloatFromStack();
    num2 = AI_GetFloatFromStack();

    AI_PutFloatToStack(AI_EvalFunc_MATHFloatMax_C(num1, num2));

    return node + 1;
}

// Note that this compares the magnitude as a double, where
// MTHFloatNullToler below narrows it to a float first. The two
// therefore disagree on a value right at the boundary; preserved as
// written.
BOOL AI_EvalFunc_MATHFloatNullEps_C(float f)
{
    double magnitude;

    magnitude = fabs(f);
    return magnitude < (double)AI_gf_Epsilon;
}

tAINode* AI_EvalFunc_MATHFloatNullEps(tAINode* node)
{
    float f;

    f = AI_GetFloatFromStack();
    AI_PutIntToStack(AI_EvalFunc_MATHFloatNullEps_C(f));
    return node + 1;
}

void AI_EvalFunc_MATHEpsilonSet_C(float epsilon)
{
    AI_gf_Epsilon = epsilon;
}

tAINode* AI_EvalFunc_MATHEpsilonSet(tAINode* node)
{
    float epsilon;

    epsilon = AI_GetFloatFromStack();
    AI_EvalFunc_MATHEpsilonSet_C(epsilon);
    return node + 1;
}

float AI_EvalFunc_MATHEpsilonGet_C()
{
    return AI_gf_Epsilon;
}

tAINode* AI_EvalFunc_MATHEpsilonGet(tAINode* node)
{
    AI_PutFloatToStack(AI_EvalFunc_MATHEpsilonGet_C());
    return node + 1;
}

void AI_EvalFunc_MATHVecBlend_C(Vec3* vec1, Vec3* vec2, float amount, Vec3* dest)
{
    MATH_BlendVector(dest, vec1, vec2, amount);
}

tAINode* AI_EvalFunc_MATHVecBlend(tAINode* node)
{
    Vec3* vec2;
    Vec3 blended;
    float amount;

    amount = AI_GetFloatFromStack();
    vec2 = (Vec3*)AI_PopStackSlot();
    AI_EvalFunc_MATHVecBlend_C((Vec3*)AI_PopStackSlot(), vec2, amount, &blended);
    AI_PutVecToStack(&blended);
    return node + 1;
}

// Rodrigues rotation about an arbitrary axis: the component along the
// axis is split off, the rest is turned by the angle, and the two are
// added back together.
//
// Note that this normalises the caller's axis in place.
void AI_EvalFunc_MATHVecRotate_C(Vec3* src, Vec3* axis, float angle, Vec3* dest)
{
    Vec3 perpendicular;
    Vec3 parallel;
    Vec3 turned;
    Vec3 crossTerm;
    Vec3 cross;
    float projection;
    float axisLenSquared;
    float cosAngle;
    float sinAngle;

    perpendicular = *src;
    projection = MATH_DotProduct(&perpendicular, axis);
    axisLenSquared = MATH_DotProduct(axis, axis);
    projection = projection / axisLenSquared;

    parallel.x = projection * axis->x;
    parallel.y = axis->y * projection;
    parallel.z = axis->z * projection;

    perpendicular.x = perpendicular.x - parallel.x;
    perpendicular.y = perpendicular.y - parallel.y;
    perpendicular.z = perpendicular.z - parallel.z;

    cosAngle = (float)cos(angle);
    MATH_VecMultScalar(&turned, &perpendicular, cosAngle);

    MATH_NormalizeEqualVector(axis);
    MATH_CrossProduct(&cross, axis, &perpendicular);
    sinAngle = (float)sin(angle);
    MATH_VecMultScalar(&crossTerm, &cross, sinAngle);

    MATH_VecAddVec(&turned, &turned, &crossTerm);
    turned.x = turned.x + parallel.x;
    turned.y = turned.y + parallel.y;
    turned.z = turned.z + parallel.z;
    *dest = turned;
}

tAINode* AI_EvalFunc_MATHVecRotate(tAINode* node)
{
    Vec3 src;
    Vec3 axis;
    Vec3 rotated;
    float angle;

    angle = AI_GetFloatFromStack();
    AI_GetVecFromStack(&axis);
    AI_GetVecFromStack(&src);
    AI_EvalFunc_MATHVecRotate_C(&src, &axis, angle, &rotated);
    AI_PutVecToStack(&rotated);
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecSetNull(tAINode* node)
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    MATH_SetNullVector((Vec3*)var.ptr);
    return node + 1;
}

// Rounds to a multiple of precision. Adding the magic double pins the
// float's exponent so that the integer part lands in the low word of
// its bit pattern, which is the x87 round-to-nearest trick.
float AI_EvalFunc_MATHFloatRound_C(float value, float precision) // TODO: ellenorizni ibztos jo-e
{
    double rounded;
    float steps;

    rounded = 1.0 / precision * value + MATH_gd_magicDoubleForRound;
    steps = (float)*(int32_t *)&rounded;
    return steps * precision;
}

tAINode* AI_EvalFunc_MATHFloatRound(tAINode* node)  // TODO: ellenorizni ibztos jo-e + AI_EvalFunc_MATHFloatRound_C inline
{
    float precision;
    float value;

    precision = AI_GetFloatFromStack();
    value = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MATHFloatRound_C(value, precision));
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecRound(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    float precision;

    precision = AI_GetFloatFromStack();
    AI_GetVarFromStack(converted, &var);
    MATH_RoundVectorWithPrecision((Vec3*)var.ptr, precision);
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecCos(tAINode* node)
{
    Vec3* vec2;

    vec2 = (Vec3*)AI_PopStackSlot();
    AI_PutFloatToStack(MATH_f_VecCos((Vec3*)AI_PopStackSlot(), vec2));
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecSin(tAINode* node)
{
    Vec3* axis;
    Vec3* vec2;

    axis = (Vec3*)AI_PopStackSlot();
    vec2 = (Vec3*)AI_PopStackSlot();
    AI_PutFloatToStack(MATH_f_VecSin((Vec3*)AI_PopStackSlot(), vec2, axis));
    return node + 1;
}

tAINode* AI_EvalFunc_MATHVecAngle(tAINode* node)
{
    Vec3* axis;
    Vec3* vec2;

    axis = (Vec3*)AI_PopStackSlot();
    vec2 = (Vec3*)AI_PopStackSlot();
    AI_PutFloatToStack(MATH_f_VecAngle((Vec3*)AI_PopStackSlot(), vec2, axis));
    return node + 1;
}

// The angle between two vectors, clamped at the ends where acos would
// be out of range. MATH_f_VecAbsAngle, which the original note names,
// does not exist in the tree, so it stays written out here.
tAINode* AI_EvalFunc_MATHVecAbsAngle(tAINode* node)
{
    Vec3* vec2;
    float cosAngle;

    vec2 = (Vec3*)AI_PopStackSlot();
    cosAngle = MATH_f_VecCos((Vec3*)AI_PopStackSlot(), vec2);// MATH_f_VecAbsAngle inline

    if (cosAngle < 1.0)
    {
        if (cosAngle > -1.0)
            AI_PutFloatToStack((float)acos(cosAngle));
        else
            AI_PutFloatToStack(3.1415927f); // pi
    }
    else
    {
        AI_PutFloatToStack(0.0);
    }
    return node + 1;
}

// Turns vec1 towards vec2 by amount, and scales the result between their
// two lengths. Four cases do not rotate at all: either vector being
// null, the two being parallel, and the two being anti-parallel, where
// a base axis stands in for the cross product that vanished.
void AI_EvalFunc_MATHVecBlendRotate_C(Vec3* vec1, Vec3* vec2, float amount, Vec3* dest)
{
    Mtx4x4 rotation;
    Vec3 axis;
    Vec3 baseAxis;
    float len1;
    float len2;
    float length;
    float angle;
    char done;

    if (amount < 0.0)
        amount = 0.0;
    if (amount > 1.0)
        amount = 1.0;
    done = 0;

    if (MATH_b_NulVectorWithEpsilon(vec1, 0.0000001f))
    {
        done = 1;
        *dest = *vec2;
    }

    if (MATH_b_NulVectorWithEpsilon(vec2, 0.0000001f))
    {
        done = 1;
        *dest = *vec1;
    }

    if (!done)
    {
        MATH_CrossProduct(&axis, vec1, vec2);
        if (MATH_b_NulVectorWithEpsilon(&axis, 0.00001f)) // 0.0000099999997 (ps2)
        {
            if (MATH_DotProduct(vec2, vec1) <= 0.0f)
            {
                // anti-parallel: any axis perpendicular to vec1 will do
                baseAxis = MATH_gst_BaseVectorI;
                MATH_CrossProduct(&axis, vec1, &baseAxis);
                if (MATH_b_NulVectorWithEpsilon(&axis, 0.001f))
                {
                    baseAxis = MATH_gst_BaseVectorJ;
                    MATH_CrossProduct(&axis, vec1, &baseAxis);
                }
            }
            else
            {
                // parallel already, so there is nothing to turn
                *dest = *vec1;
                done = 1;
            }
        }

        if (!done)
        {
            MATH_NormalizeEqualVector(&axis);
            angle = MATH_f_VecAngle(vec1, vec2, &axis) * amount;
            MATH_MakeRotationMatrix_AxisAngle(&rotation, &axis, angle, 0, 1);
            MATH_TransformVector(dest, &rotation, vec1);
        }
    }

    len1 = (float)sqrt(MATH_VecLenSquared(vec1));
    len2 = (float)sqrt(MATH_VecLenSquared(vec2));

    if (MATH_b_NulVectorWithEpsilon(dest, 0.0000001f))
    {
        MATH_SetNullVector(dest);
    }
    else
    {
        length = MATH_LinearInterpolateFloats(len1, len2, amount);
        MATH_NormalizeVector(dest, dest);
        MATH_VecMultScalarWithoutBuffer(dest, length);
    }
}

tAINode* AI_EvalFunc_MATHVecBlendRotate(tAINode* node)
{
    Vec3* vec2;
    Vec3 blended;
    float amount;

    amount = AI_GetFloatFromStack();
    vec2 = (Vec3*)AI_PopStackSlot();
    AI_EvalFunc_MATHVecBlendRotate_C((Vec3*)AI_PopStackSlot(), vec2, amount, &blended);
    AI_PutVecToStack(&blended);
    return node + 1;
}

float AI_EvalFunc_MATHFloatBlend_C(float f1, float f2, float factor) {

    if (factor < 0.0f)
        factor = 0.0f;
    if (factor > 1.0f)
        factor = 1.0f;
    return f2 * factor + (1.0f - factor) * f1;
}

tAINode* AI_EvalFunc_MATHFloatBlend(tAINode* node)
{
    float f1;
    float f2;
    float factor;

    factor = AI_GetFloatFromStack();
    f2 = AI_GetFloatFromStack();
    f1 = AI_GetFloatFromStack();

    AI_PutFloatToStack(AI_EvalFunc_MATHFloatBlend_C(f1, f2, factor));
    return node + 1;
}

// A binomial draw quantised to step: one trial per step between min
// and max, each adding a step with probability (value - min) / step
// out of the number of steps. The original spelled the trial's
// threshold as an interpolation from zero.
float AI_EvalFunc_MATHRandBino_C(float value, float step, float max, float min)
{
    float target;
    float raw;
    float unit;
    float total;
    int steps;
    int i;

    target = (value - min) / step;
    steps = (int)((max - min) / step);
    total = 0.0;

    for (i = 0; i < steps; ++i)
    {
        raw = (float)rand();
        unit = (float)(raw / 32767.0);
        if ((float)steps * unit < target)
            total = total + step;
    }
    return total + min;
}

tAINode* AI_EvalFunc_MATHRandBino(tAINode* node)
{
    float value;
    float step;
    float max;
    float min;

    min = AI_GetFloatFromStack();
    max = AI_GetFloatFromStack();
    step = AI_GetFloatFromStack();
    value = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MATHRandBino_C(value, step, max, min));
    return node + 1;
}

// Clamps a vector into a cone about an axis. The flag bits say what
// the caller has already done and what it wants back: bit 0 that the
// axis is normalised, bit 1 that the vector is, and bit 2 that the
// result should be a unit vector rather than the vector's own length.
void AI_EvalFunc_MATHVecInCone_C(Vec3* vec, Vec3* axis, float halfAngle, int flags, Vec3* dest)
{
    Vec3 fallback;
    Vec3 perpendicular;
    Vec3 alongAxis;
    Vec3 clamped;
    float length;
    float cosHalf;
    float sinHalf;
    float dot;
    float invLength;

    length = 1.0;
    if ((flags & 1) == 0)
        MATH_NormalizeEqualVector(axis);

    dot = MATH_DotProduct(vec, axis);
    if ((flags & 2) == 0)
    {
        length = MATH_VecLen(vec);
        dot = dot / length;
    }

    cosHalf = (float)cos(halfAngle);
    if (dot <= (double)cosHalf)
    {
        // outside the cone, so rebuild it on the cone's edge in the
        // plane the vector and the axis share
        if ((float)fabs(dot - -1.0) < 0.0000099999997)
        {
            // anti-parallel, so that plane is undefined: pick one
            if (vec->y == 0.0)
            {
                fallback.x = 0.0;
                fallback.y = 1.0;
            }
            else
            {
                fallback.x = 1.0;
                fallback.y = 0.0;
            }
            fallback.z = 0.0;
            MATH_CrossProduct(&perpendicular, axis, &fallback);
        }
        else
        {
            MATH_CrossProduct(&perpendicular, axis, vec);
        }

        MATH_CrossProduct(&clamped, &perpendicular, axis);
        MATH_NormalizeEqualVector(&clamped);
        sinHalf = (float)sin(halfAngle);
        MATH_VecMultScalarWithoutBuffer(&clamped, sinHalf);
        MATH_VecMultScalar(&alongAxis, axis, cosHalf);
        MATH_VecAddVec(&clamped, &clamped, &alongAxis);

        if ((flags & 6) == 0)
            MATH_VecMultScalarWithoutBuffer(&clamped, length);
        *dest = clamped;
    }
    else
    {
        // inside the cone already
        if ((flags & 6) == 4)
        {
            invLength = (float)(1.0 / length);
            MATH_VecMultScalarWithoutBuffer(vec, invLength);
        }
        *dest = *vec;
    }
}

tAINode* AI_EvalFunc_MATHVecInCone(tAINode* node)
{
    Vec3* axis;
    Vec3 clamped;
    float halfAngle;
    int flags;

    flags = AI_GetIntFromStack();
    halfAngle = AI_GetFloatFromStack();
    axis = (Vec3*)AI_PopStackSlot();
    AI_EvalFunc_MATHVecInCone_C((Vec3*)AI_PopStackSlot(), axis, halfAngle, flags, &clamped);
    AI_PutVecToStack(&clamped);
    return node + 1;
}

void AI_EvalFunc_MATHVecGlobalToLocal_C(struct sGameObjectMain* gao, Vec3* src, Vec3* dest)
{
    MATH_VectorGlobalToLocal(dest, OBJ_pst_GetAbsoluteMatrix(gao), src);
}

tAINode* AI_EvalFunc_MATHVecGlobalToLocal(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 src;
    Vec3 local;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&src);
    AI_EvalFunc_MATHVecGlobalToLocal_C(gaoUltra, &src, &local);
    AI_PutVecToStack(&local);
    return node + 1;
}

void AI_EvalFunc_MATHVecLocalToGlobal_C(struct sGameObjectMain* gao, Vec3* src, Vec3* dest)
{
    MATH_TransformVectorNoScale(dest, OBJ_pst_GetAbsoluteMatrix(gao), src);
}

tAINode* AI_EvalFunc_MATHVecLocalToGlobal(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 src;
    Vec3 global;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetVecFromStack(&src);
    AI_EvalFunc_MATHVecLocalToGlobal_C(gaoUltra, &src, &global);
    AI_PutVecToStack(&global);
    return node + 1;
}

BOOL AI_EvalFunc_MTHVecNull_C(Vec3* v)
{
    return MATH_b_NulVector(v);
}

tAINode* AI_EvalFunc_MTHVecNull(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_MTHVecNull_C((Vec3*)AI_PopStackSlot()));
    return node + 1;
}

BOOL AI_EvalFunc_MTHVecNullEpsilon_C(Vec3* v)
{
    if (fabs(v->x) > AI_gf_Epsilon)
        return FALSE;
    if (fabs(v->y) <= AI_gf_Epsilon)
        return fabs(v->z) <= AI_gf_Epsilon;
    return FALSE;
}

tAINode* AI_EvalFunc_MTHVecNullEpsilon(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_MTHVecNullEpsilon_C((Vec3*)AI_PopStackSlot()));
    return node + 1;
}

BOOL AI_EvalFunc_MTHVecNullToler_C(Vec3* v, float tolerance)
{
    if (fabs(v->x) > tolerance)
        return FALSE;
    if (fabs(v->y) <= tolerance)
        return fabs(v->z) <= tolerance;
    return FALSE;
}

tAINode* AI_EvalFunc_MTHVecNullToler(tAINode* node)
{
    float tolerance;

    tolerance = AI_GetFloatFromStack();
    AI_PutIntToStack(AI_EvalFunc_MTHVecNullToler_C((Vec3*)AI_PopStackSlot(), tolerance));
    return node + 1;
}

// The magnitude is narrowed to a float before the comparison here,
// where MATHFloatNullEps above keeps it a double.
BOOL AI_EvalFunc_MTHFloatNullToler_C(float f, float tolerance)
{
    float magnitude;

    magnitude = (float)fabs(f);
    return magnitude < (double)tolerance;
}

tAINode* AI_EvalFunc_MTHFloatNullToler(tAINode* node)
{
    float f;
    float tolerance;

    tolerance = AI_GetFloatFromStack();
    f = AI_GetFloatFromStack();
    AI_PutIntToStack(AI_EvalFunc_MTHFloatNullToler_C(f, tolerance));
    return node + 1;
}

// Copies the sign bit onto 1.0 rather than branching. Note that zero
// reports +1, negative zero included.
float AI_EvalFunc_MTHFloatSign_C(float f)
{
    uint32_t signed_one;

    if (f == 0.0)
        return 1.0f;

    signed_one = ((*(uint32_t*)&f) & 0x80000000) | 0x3F800000;
    return *(float*)&signed_one;
}

tAINode* AI_EvalFunc_MTHFloatSign(tAINode* node)
{
    float f;

    f = AI_GetFloatFromStack();
    AI_PutFloatToStack(AI_EvalFunc_MTHFloatSign_C(f));
    return node + 1;
}

int AI_EvalFunc_MTHIntSign_C(int value)
{
    if (value > 0)
        return 1;
    if (value < 0)
        return -1;
    return value;
}

tAINode* AI_EvalFunc_MTHIntSign(tAINode* node)
{
    int value;

    value = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MTHIntSign_C(value));
    return node + 1;
}

tAINode* AI_EvalFunc_MTHFloatSqrt(tAINode* node)
{
    float f;

    f = AI_GetFloatFromStack();
    AI_PutFloatToStack(sqrtf(f));
    return node + 1;
}

tAINode* AI_EvalFunc_MTHLn(tAINode* node)
{
    float f;

    f = AI_GetFloatFromStack();
    AI_PutFloatToStack((float)log(f));
    return node + 1;
}

tAINode* AI_EvalFunc_MTHLog10(tAINode* node)
{
    float f;

    f = AI_GetFloatFromStack();
    AI_PutFloatToStack((float)log10(f));
    return node + 1;
}

int AI_EvalFunc_MTHModulo_C(int value, int divisor)
{
    if (!divisor)
        return 0;
    return value % divisor;
}

tAINode* AI_EvalFunc_MTHModulo(tAINode* node)
{
    int value;
    int divisor;

    divisor = AI_GetIntFromStack();
    value = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MTHModulo_C(value, divisor));
    return node + 1;
}
