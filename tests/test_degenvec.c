/* Degenerate vectors through the normalizing math.
 *
 * The math differential cannot cover this: it compares math.c against
 * a frozen copy that predates these guards, and its generator never
 * draws a zero-length vector, so it agrees whether or not the guards
 * are there. These are direct assertions instead.
 *
 * Both directions matter. Some normalizes leave a null vector alone
 * and some are meant to divide regardless, and callers depend on
 * which is which -- a script that tests its vector against null after
 * normalizing it only works because the null survived.
 */

#include <stdio.h>
#include <math.h>
#include "../math.h"

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static int is_nan(float f)
{
    return f != f;
}

static int vec_is_nan(const Vec3* v)
{
    return is_nan(v->x) || is_nan(v->y) || is_nan(v->z);
}

static int mtx33_is_nan(const Mtx4x4* m)
{
    return vec_is_nan(&m->v1) || vec_is_nan(&m->v2) || vec_is_nan(&m->v3);
}

static void set(Vec3* v, float x, float y, float z)
{
    v->x = x; v->y = y; v->z = z;
}

int main(void)
{
    Vec3 v, sight, banking;
    Mtx4x4 m;

    printf("=== degenerate vectors ===\n");

    /* The guarded normalize leaves a null vector exactly as it was. */
    set(&v, 0.0f, 0.0f, 0.0f);
    MATH_NormalizeEqualVector(&v);
    if (vec_is_nan(&v))
        fail("NormalizeEqualVector turned a null vector into NaN");
    else if (!MATH_b_NulVector(&v))
        fail("NormalizeEqualVector did not leave a null vector null");

    /* ...and still normalizes an ordinary one. */
    set(&v, 0.0f, 3.0f, 4.0f);
    MATH_NormalizeEqualVector(&v);
    if (v.y < 0.59f || v.y > 0.61f || v.z < 0.79f || v.z > 0.81f)
        fail("NormalizeEqualVector no longer normalizes");

    /* Orthonormalize normalizes all three axes the guarded way, so a
       matrix with a null row comes back finite rather than NaN. */
    MATH_SetIdentityMatrix(&m);
    set(&m.v1, 0.0f, 0.0f, 0.0f);
    MATH_Orthonormalize(&m);
    if (mtx33_is_nan(&m))
        fail("Orthonormalize produced NaN from a null row");

    /* The banking builder normalizes the sight the guarded way. */
    MATH_SetIdentityMatrix(&m);
    set(&sight, 0.0f, 0.0f, 0.0f);
    set(&banking, 0.0f, 0.0f, 1.0f);
    MATH_MakeRotationMatrix_UsingBanking(&m, &sight, &banking, 1);
    if (mtx33_is_nan(&m))
        fail("MakeRotationMatrix_UsingBanking produced NaN from a null sight");

    /* The sight builder does not: its sight normalize is written out
       in full and divides whatever it is given. Keep it that way --
       this asserts the faithful behaviour, not a bug. */
    MATH_SetIdentityMatrix(&m);
    set(&sight, 0.0f, 0.0f, 0.0f);
    set(&banking, 0.0f, 0.0f, 1.0f);
    MATH_MakeRotationMatrix_UsingSight(&m, &sight, &banking, 1);
    if (!mtx33_is_nan(&m))
        fail("MakeRotationMatrix_UsingSight has acquired a guard on its sight");

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  6 checks, 0 failure(s)\n");
    return 0;
}
