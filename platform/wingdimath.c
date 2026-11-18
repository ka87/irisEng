#include "wingdimath.h"

#include <string.h>

D3DMATRIX* D3D_MatrixMultiply(D3DMATRIX* out, const D3DMATRIX* a, const D3DMATRIX* b)
{
    D3DMATRIX    r;
    const float* fa = (const float*)a;
    const float* fb = (const float*)b;
    float*       fr = (float*)&r;
    int          i, j, k;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            float s = 0.0f;

            for (k = 0; k < 4; k++)
                s += fa[i * 4 + k] * fb[k * 4 + j];
            fr[i * 4 + j] = s;
        }
    memcpy(out, &r, sizeof(r));
    return out;
}

D3DMATRIX* D3D_MatrixTranslation(D3DMATRIX* out, float x, float y, float z)
{
    float* f = (float*)out;
    int    i;

    for (i = 0; i < 16; i++)
        f[i] = (i % 5) == 0 ? 1.0f : 0.0f;
    f[12] = x;
    f[13] = y;
    f[14] = z;
    return out;
}

D3DMATRIX* D3D_MatrixOrthoOffCenterRH(D3DMATRIX* out, float l, float r,
                                      float b, float t, float zn, float zf)
{
    float* f = (float*)out;
    int    i;

    for (i = 0; i < 16; i++)
        f[i] = 0.0f;
    f[0]  = 2.0f / (r - l);
    f[5]  = 2.0f / (t - b);
    f[10] = 1.0f / (zn - zf);
    f[12] = (l + r) / (l - r);
    f[13] = (t + b) / (b - t);
    f[14] = zn / (zn - zf);
    f[15] = 1.0f;
    return out;
}

D3DMATRIX* D3D_p_MatrixInverse(D3DMATRIX* out, float* det, const D3DMATRIX* in)
{
    const float* m = (const float*)in;
    float        a[16];
    float        d;
    float        inv;
    int          i;

    /* The adjugate, as the twelve 2x2 determinants of the first two
       rows and the last two, which is the usual grouping: each
       cofactor below is two of them differenced, and every one is
       used twice. */
    float s0 = m[0] * m[5]  - m[4] * m[1];
    float s1 = m[0] * m[6]  - m[4] * m[2];
    float s2 = m[0] * m[7]  - m[4] * m[3];
    float s3 = m[1] * m[6]  - m[5] * m[2];
    float s4 = m[1] * m[7]  - m[5] * m[3];
    float s5 = m[2] * m[7]  - m[6] * m[3];

    float c5 = m[10] * m[15] - m[14] * m[11];
    float c4 = m[9]  * m[15] - m[13] * m[11];
    float c3 = m[9]  * m[14] - m[13] * m[10];
    float c2 = m[8]  * m[15] - m[12] * m[11];
    float c1 = m[8]  * m[14] - m[12] * m[10];
    float c0 = m[8]  * m[13] - m[12] * m[9];

    d = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (det)
        *det = d;
    if (d == 0.0f)
        return NULL;
    inv = 1.0f / d;

    a[0]  = ( m[5] * c5 - m[6] * c4 + m[7] * c3) * inv;
    a[1]  = (-m[1] * c5 + m[2] * c4 - m[3] * c3) * inv;
    a[2]  = ( m[13] * s5 - m[14] * s4 + m[15] * s3) * inv;
    a[3]  = (-m[9] * s5 + m[10] * s4 - m[11] * s3) * inv;

    a[4]  = (-m[4] * c5 + m[6] * c2 - m[7] * c1) * inv;
    a[5]  = ( m[0] * c5 - m[2] * c2 + m[3] * c1) * inv;
    a[6]  = (-m[12] * s5 + m[14] * s2 - m[15] * s1) * inv;
    a[7]  = ( m[8] * s5 - m[10] * s2 + m[11] * s1) * inv;

    a[8]  = ( m[4] * c4 - m[5] * c2 + m[7] * c0) * inv;
    a[9]  = (-m[0] * c4 + m[1] * c2 - m[3] * c0) * inv;
    a[10] = ( m[12] * s4 - m[13] * s2 + m[15] * s0) * inv;
    a[11] = (-m[8] * s4 + m[9] * s2 - m[11] * s0) * inv;

    a[12] = (-m[4] * c3 + m[5] * c1 - m[6] * c0) * inv;
    a[13] = ( m[0] * c3 - m[1] * c1 + m[2] * c0) * inv;
    a[14] = (-m[12] * s3 + m[13] * s1 - m[14] * s0) * inv;
    a[15] = ( m[8] * s3 - m[9] * s1 + m[10] * s0) * inv;

    for (i = 0; i < 16; i++)
        ((float*)out)[i] = a[i];
    return out;
}
