/* What the three clamping AI maths functions do with a non-number.
 *
 * Scripts divide without checking the divisor, and more than one of
 * them relies on the clamp immediately afterwards to swallow the
 * result when the divisor was zero. So whether a clamp passes a
 * non-number through or replaces it is not a detail: it decides
 * whether one bad frame stays in one value or spreads into an
 * object's position and stays there.
 *
 * The three do not agree with each other, and that is correct:
 *
 *   max    a comparison that is false either way falls to the second
 *          argument, so the bad value is dropped
 *   min    likewise falls to the second argument
 *   limit  both tests are against the value itself, so a bad one
 *          fails both and is returned unchanged
 *
 * Each is also checked on ordinary numbers, so a change that fixed
 * the non-number case by breaking the arithmetic would not pass.
 */

#include <stdio.h>
#include "../ai.h"
#include "../ai/ai_mathfunctions.h"

/* Only the three clamps are exercised, but the whole file has to link.
   These stand in for the evaluator's stack and the two outside helpers
   it refers to; nothing below is called by any check. */
float AI_GetFloatFromStack(void) { return 0.0f; }
int AI_GetIntFromStack(void) { return 0; }
int AI_PutFloatToStack(float v) { (void)v; return 0; }
int AI_PutIntToStack(int v) { (void)v; return 0; }
Vec3* AI_GetVecFromStack(Vec3* v) { return v; }
void AI_PutVecToStack(Vec3* v) { (void)v; }
void AI_GetVarFromStack(void* dest, tVarStackElem* vse) { (void)dest; (void)vse; }
char* AI_PopStackSlot(void) { return 0; }
Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao) { (void)gao; return 0; }
int RND_JADErand(void) { return 0; }

static int failures;

static void check(const char* what, float got, float want)
{
    /* compared bitwise so that a non-number result is caught rather
       than quietly comparing unequal to everything */
    if (got == want || (got != got && want != want))
        return;
    printf("  FAIL %s: got %f, wanted %f\n", what, (double)got, (double)want);
    ++failures;
}

int main(void)
{
    float zero = 0.0f;
    float nan_ = zero / zero;

    printf("=== AI maths clamps, non-number inputs ===\n");

    if (nan_ == nan_)
    {
        printf("  SKIP: this compiler folded 0/0 into something ordered\n");
        return 0;
    }

    /* max: arguments reach it in reverse, so num2 is the script's
       first argument. A bad first argument must be dropped. */
    check("FloatMax(bad, 0) drops the bad value",
          AI_EvalFunc_MATHFloatMax_C(0.0f, nan_), 0.0f);
    check("FloatMax(0, bad) drops the bad value",
          AI_EvalFunc_MATHFloatMax_C(nan_, 0.0f), nan_);
    check("FloatMax(3, 7)", AI_EvalFunc_MATHFloatMax_C(7.0f, 3.0f), 7.0f);
    check("FloatMax(7, 3)", AI_EvalFunc_MATHFloatMax_C(3.0f, 7.0f), 7.0f);
    check("FloatMax(5, 5)", AI_EvalFunc_MATHFloatMax_C(5.0f, 5.0f), 5.0f);

    /* min: same shape, same direction */
    check("FloatMin(bad, 0) drops the bad value",
          AI_EvalFunc_MATHFloatMin_C(0.0f, nan_), 0.0f);
    check("FloatMin(3, 7)", AI_EvalFunc_MATHFloatMin_C(7.0f, 3.0f), 3.0f);
    check("FloatMin(7, 3)", AI_EvalFunc_MATHFloatMin_C(3.0f, 7.0f), 3.0f);

    /* limit: the odd one out, and deliberately so */
    check("FloatLimit passes a bad value through",
          AI_EvalFunc_MATHFloatLimit_C(nan_, 0.0f, 1.0f), nan_);
    check("FloatLimit(0.5, 0, 1)",
          AI_EvalFunc_MATHFloatLimit_C(0.5f, 0.0f, 1.0f), 0.5f);
    check("FloatLimit(-3, 0, 1)",
          AI_EvalFunc_MATHFloatLimit_C(-3.0f, 0.0f, 1.0f), 0.0f);
    check("FloatLimit(9, 0, 1)",
          AI_EvalFunc_MATHFloatLimit_C(9.0f, 0.0f, 1.0f), 1.0f);

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  12 checks, 0 failure(s)\n");
    return 0;
}
