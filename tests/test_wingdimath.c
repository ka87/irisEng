/* A unit test for win/wingdimath.c.
 *
 * New code replacing four D3DX calls, so this is a direct check and
 * not a differential one: there is no earlier copy of these to
 * compare against, and the thing worth checking is the arithmetic
 * rather than a call sequence.
 *
 * The reference inverses and the reference product below were
 * computed outside this file, in exact rational arithmetic, and
 * pasted in. That matters: an expected value worked out with the same
 * formulas being tested would only prove the formulas agree with
 * themselves. Each inverse is also checked the other way, by
 * multiplying it back and asking for the identity, which is
 * independent of both.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../win/wingdimath.h"

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

/* Matrices are compared with a tolerance, because the reference is
   exact and the implementation is single-precision. 1e-5 is far
   tighter than any arithmetic slip and far looser than rounding. */
static void okMatrix(const D3DMATRIX* got, const float* want, const char* what)
{
    const float* g = (const float*)got;
    int          i;

    for (i = 0; i < 16; i++)
    {
        checks++;
        if (fabs((double)(g[i] - want[i])) > 1e-5)
        {
            printf("  FAIL %s: element %d is %.9g, want %.9g\n",
                   what, i, (double)g[i], (double)want[i]);
            fails++;
            return;
        }
    }
}

static void okNear(double got, double want, const char* what)
{
    checks++;
    if (fabs(got - want) > 1e-5)
    {
        printf("  FAIL %s: got %.9g want %.9g\n", what, got, want);
        fails++;
    }
}

/* v * M, Direct3D's way round: a row vector on the left. */
static void transform(float* out, const float* v, const D3DMATRIX* m)
{
    const float* f = (const float*)m;
    int          j, k;

    for (j = 0; j < 4; j++)
    {
        float s = 0.0f;

        for (k = 0; k < 4; k++)
            s += v[k] * f[k * 4 + j];
        out[j] = s;
    }
}

static const float identity[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};
/* ------------------------------------------- the reference values */

static const float mulA[16] = {
    1.0f, 2.0f, 3.0f, 4.0f,
    5.0f, 6.0f, 7.0f, 8.0f,
    9.0f, 10.0f, 11.0f, 12.0f,
    13.0f, 14.0f, 15.0f, 16.0f
};
static const float mulB[16] = {
    17.0f, 18.0f, 19.0f, 20.0f,
    21.0f, 22.0f, 23.0f, 24.0f,
    25.0f, 26.0f, 27.0f, 28.0f,
    29.0f, 30.0f, 31.0f, 32.0f
};
/* computed outside this file */
static const float mulAB[16] = {
    250.0f, 260.0f, 270.0f, 280.0f,
    618.0f, 644.0f, 670.0f, 696.0f,
    986.0f, 1028.0f, 1070.0f, 1112.0f,
    1354.0f, 1412.0f, 1470.0f, 1528.0f
};

/* a view matrix: rotation about Y by 30 degrees, then a translation */
static const float rigid[16] = {
    0.8660254f, 0.0f, -0.5f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.8660254f, 0.0f,
    10.0f, 20.0f, 30.0f, 1.0f
};
/* computed outside this file, in exact arithmetic */
static const float rigidInv[16] = {
    0.866025406f, 0.0f, 0.500000003f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    -0.500000003f, 0.0f, 0.866025406f, 0.0f,
    6.33974604f, -20.0f, -30.9807622f, 1.0f
};

/* the same carrying a non-uniform scale -- which is what the flag
   that selects this path means, and the reason the engine's own rigid
   inverse will not do */
static const float scaled[16] = {
    1.7320508f, 0.0f, -1.0f, 0.0f,
    0.0f, 3.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.7320508f, 0.0f,
    10.0f, 20.0f, 30.0f, 1.0f
};
/* computed outside this file, in exact arithmetic */
static const float scaledInv[16] = {
    0.433012703f, 0.0f, 0.250000002f, 0.0f,
    0.0f, 0.333333333f, 0.0f, 0.0f,
    -0.250000002f, 0.0f, 0.433012703f, 0.0f,
    3.16987302f, -6.66666667f, -15.4903811f, 1.0f
};

/* sheared, where no shortcut applies at all */
static const float sheared[16] = {
    1.0f, 2.0f, 3.0f, 0.0f,
    0.0f, 1.0f, 4.0f, 0.0f,
    5.0f, 6.0f, 0.0f, 0.0f,
    1.0f, 2.0f, 3.0f, 1.0f
};
/* computed outside this file, in exact arithmetic */
static const float shearedInv[16] = {
    -24.0f, 18.0f, 5.0f, 0.0f,
    20.0f, -15.0f, -4.0f, 0.0f,
    -5.0f, 4.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, 0.0f, 1.0f
};

/* a fourth column that is not (0,0,0,1) */
static const float projective[16] = {
    2.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 2.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, -1.0f, 0.0f
};
/* computed outside this file, in exact arithmetic */
static const float projectiveInv[16] = {
    0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

/* Every entry non-zero, so that every one of the twelve 2x2
   determinants and every cofactor term actually contributes. Four
   matrices with zeros in them missed a sign error on one term,
   because the term was multiplied by zero in all of them. */
static const float dense[16] = {
    4.0f, 1.0f, 2.0f, 3.0f,
    1.0f, 5.0f, 1.0f, 2.0f,
    2.0f, 1.0f, 6.0f, 1.0f,
    3.0f, 2.0f, 1.0f, 7.0f
};
/* computed outside this file, in exact arithmetic */
static const float denseInv[16] = {
    0.43627451f, 0.00490196078f, -0.117647059f, -0.171568627f,
    0.00490196078f, 0.230392157f, -0.0294117647f, -0.0637254902f,
    -0.117647059f, -0.0294117647f, 0.205882353f, 0.0294117647f,
    -0.171568627f, -0.0637254902f, 0.0294117647f, 0.230392157f
};

/* ------------------------------------------------------- the cases */

static void caseMultiply(void)
{
    D3DMATRIX r;

    printf("-- multiply\n");

    D3D_MatrixMultiply(&r, (const D3DMATRIX*)mulA, (const D3DMATRIX*)mulB);
    okMatrix(&r, mulAB, "a known product");

    D3D_MatrixMultiply(&r, (const D3DMATRIX*)identity, (const D3DMATRIX*)mulB);
    okMatrix(&r, mulB, "identity on the left leaves the other alone");

    D3D_MatrixMultiply(&r, (const D3DMATRIX*)mulA, (const D3DMATRIX*)identity);
    okMatrix(&r, mulA, "and so does identity on the right");

    /* It is not commutative, so getting the operands the wrong way
       round has to be visible. */
    D3D_MatrixMultiply(&r, (const D3DMATRIX*)mulB, (const D3DMATRIX*)mulA);
    checks++;
    if (memcmp(&r, mulAB, sizeof(r)) == 0)
    {
        printf("  FAIL the product does not depend on the order\n");
        fails++;
    }

    /* The renderer passes the destination as an operand, so that has
       to work: the result is formed in a temporary first. */
    memcpy(&r, mulA, sizeof(r));
    D3D_MatrixMultiply(&r, &r, (const D3DMATRIX*)mulB);
    okMatrix(&r, mulAB, "the destination may be the left operand");

    memcpy(&r, mulB, sizeof(r));
    D3D_MatrixMultiply(&r, (const D3DMATRIX*)mulA, &r);
    okMatrix(&r, mulAB, "or the right one");

    ok(D3D_MatrixMultiply(&r, (const D3DMATRIX*)mulA, (const D3DMATRIX*)mulB) == &r,
       "it gives back what it was told to write into");
}

static void caseTranslation(void)
{
    D3DMATRIX r;
    float     v[4] = { 1.0f, 2.0f, 3.0f, 1.0f };
    float     p[4];
    float     want[16];

    printf("-- translation\n");

    memcpy(want, identity, sizeof(want));
    want[12] = 7.0f;
    want[13] = -8.0f;
    want[14] = 9.5f;
    D3D_MatrixTranslation(&r, 7.0f, -8.0f, 9.5f);
    okMatrix(&r, want, "identity with the translation in the fourth row");

    /* which is the row Direct3D's row vectors pick up */
    transform(p, v, &r);
    okNear(p[0], 8.0, "a point moves in x");
    okNear(p[1], -6.0, "and in y");
    okNear(p[2], 12.5, "and in z");
    okNear(p[3], 1.0, "and keeps its w");
}

static void caseOrtho(void)
{
    D3DMATRIX r;
    float     p[4];
    float     corner[4];

    printf("-- orthographic projection\n");

    /* An off-centre box, deliberately asymmetric in every axis so
       that a dropped offset or a swapped pair shows up. */
    D3D_MatrixOrthoOffCenterRH(&r, 10.0f, 50.0f, 20.0f, 100.0f, 1.0f, 9.0f);

    /* The near-bottom-left corner goes to the corner of clip space,
       and the far-top-right to the opposite one. Checking where the
       box lands is the whole meaning of the matrix; checking sixteen
       numbers against the same formula would not be.

       Note the sign on z. This is the right-handed projection, so the
       camera looks down negative z and a point at distance zn sits at
       view z = -zn. The first draft of this case used +zn, and the
       four depth expectations failed against correct code -- which is
       the trap worth leaving written down, because the near and far
       arguments are given as positive distances while the geometry
       they describe is at negative z. */
    corner[0] = 10.0f; corner[1] = 20.0f; corner[2] = -1.0f; corner[3] = 1.0f;
    transform(p, corner, &r);
    okNear(p[0], -1.0, "left goes to -1");
    okNear(p[1], -1.0, "bottom goes to -1");
    okNear(p[2], 0.0, "and near goes to 0, which is Direct3D's range");

    corner[0] = 50.0f; corner[1] = 100.0f; corner[2] = -9.0f; corner[3] = 1.0f;
    transform(p, corner, &r);
    okNear(p[0], 1.0, "right goes to +1");
    okNear(p[1], 1.0, "top goes to +1");
    okNear(p[2], 1.0, "and far goes to 1");

    /* the centre of the box lands in the middle */
    corner[0] = 30.0f; corner[1] = 60.0f; corner[2] = -5.0f; corner[3] = 1.0f;
    transform(p, corner, &r);
    okNear(p[0], 0.0, "the middle of the box is the middle of x");
    okNear(p[1], 0.0, "and of y");
    okNear(p[2], 0.5, "and halfway in depth");
    okNear(p[3], 1.0, "with w left alone, so it does not divide");

    /* Beyond the far plane goes past 1 rather than back towards 0. */
    corner[0] = 30.0f; corner[1] = 60.0f; corner[2] = -17.0f; corner[3] = 1.0f;
    transform(p, corner, &r);
    ok(p[2] > 1.0f, "past the far plane is past 1");

    /* And the handedness itself: a point in front of the camera, at
       positive view z, is behind it as far as this matrix is
       concerned, so its depth comes out below the near plane. That is
       what tells this from its left-handed twin, which the engine
       does not want and which scores identically on x and y. */
    corner[0] = 30.0f; corner[1] = 60.0f; corner[2] = 5.0f; corner[3] = 1.0f;
    transform(p, corner, &r);
    ok(p[2] < 0.0f, "positive view z is behind the near plane");

    /* the overlay's own call: y flipped, so screen y grows downwards */
    D3D_MatrixOrthoOffCenterRH(&r, 0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);
    corner[0] = 0.0f; corner[1] = 0.0f; corner[2] = 0.0f; corner[3] = 1.0f;
    transform(p, corner, &r);
    okNear(p[0], -1.0, "the top left of the screen is the left of clip space");
    okNear(p[1], 1.0, "and the top of it");
    corner[0] = 640.0f; corner[1] = 480.0f;
    transform(p, corner, &r);
    okNear(p[0], 1.0, "the bottom right is the right");
    okNear(p[1], -1.0, "and the bottom");
}

static void caseInverse(void)
{
    static const struct
    {
        const char*  name;
        const float* m;
        const float* want;
    } table[] = {
        { "rigid", rigid, rigidInv },
        { "scaled", scaled, scaledInv },
        { "sheared", sheared, shearedInv },
        { "projective", projective, projectiveInv },
        { "dense", dense, denseInv },
    };
    unsigned  i;
    D3DMATRIX r;
    D3DMATRIX back;
    float     det;

    printf("-- inverse\n");

    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
    {
        ok(D3D_p_MatrixInverse(&r, NULL, (const D3DMATRIX*)table[i].m) == &r,
           "it gives back what it was told to write into");
        okMatrix(&r, table[i].want, table[i].name);

        /* and the other way, which depends on neither the reference
           nor the formula under test */
        D3D_MatrixMultiply(&back, (const D3DMATRIX*)table[i].m, &r);
        okMatrix(&back, identity, "the matrix times its inverse");
        D3D_MatrixMultiply(&back, &r, (const D3DMATRIX*)table[i].m);
        okMatrix(&back, identity, "and the inverse times the matrix");
    }

    /* the determinant, when it is asked for */
    det = 0.0f;
    D3D_p_MatrixInverse(&r, &det, (const D3DMATRIX*)sheared);
    okNear((double)det, 1.0, "the determinant of the sheared one");
    det = 0.0f;
    D3D_p_MatrixInverse(&r, &det, (const D3DMATRIX*)scaled);
    okNear((double)det, 12.0, "and of the scaled one");
    det = 0.0f;
    D3D_p_MatrixInverse(&r, &det, (const D3DMATRIX*)identity);
    okNear((double)det, 1.0, "and of the identity");

    /* A singular matrix has no inverse: say so, write nothing, and
       report a zero determinant -- as D3DX did, because a caller
       that ignores the result must not be handed noise. */
    {
        float     singular[16];
        D3DMATRIX before;

        memcpy(singular, sheared, sizeof(singular));
        singular[4] = singular[0];   /* two identical rows */
        singular[5] = singular[1];
        singular[6] = singular[2];
        singular[7] = singular[3];

        memcpy(&r, mulA, sizeof(r));
        memcpy(&before, mulA, sizeof(before));
        det = 123.0f;
        ok(D3D_p_MatrixInverse(&r, &det, (const D3DMATRIX*)singular) == NULL,
           "a singular matrix gives nothing back");
        okNear((double)det, 0.0, "with a zero determinant");
        checks++;
        if (memcmp(&r, &before, sizeof(r)) != 0)
        {
            printf("  FAIL the destination was written anyway\n");
            fails++;
        }
    }

    /* aliasing again: this call site does not use it, but the one
       next door does and the two are easy to mix up */
    memcpy(&r, scaled, sizeof(r));
    D3D_p_MatrixInverse(&r, NULL, &r);
    okMatrix(&r, scaledInv, "the destination may be the source");
}

int main(void)
{
    printf("=== renderer matrix unit test ===\n");
    caseMultiply();
    caseTranslation();
    caseOrtho();
    caseInverse();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
