#include "../time.h"
#include "../memory.h"
#include "../loading.h"
#include "lazymodifier.h"

void GAO_ModifierLazy_Compute_SemiMecanical_Model(tLazyMdfStruct* a1, Mtx4x4* a2, Mtx4x4* a3)
{
    Vec3* v3;
    float v4;
    float v5;
    double v6;
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    double v16;
    Vec3* v17; 
    double v18;
    long double v19;
    long double v20;
    long double v21;
    double v22;
    Mtx4x4* v23;
    double v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    Vec3 v40;
    float v41;
    Vec3* v42;
    Quat4 v43;
    float v44;
    float v45;
    float v46;
    Mtx4x4 v47;
    Mtx4x4 a2a;
    Mtx4x4 a3a;

    if (a3)
    {
        v3 = &a1->mtx2.Pos;
        v4 = a1->mtx2.Pos.y;
        v5 = a1->mtx2.Pos.z;
        v44 = a1->mtx2.Pos.x;
        v45 = v4;
        v46 = v5;
        MATH_MulMatrixMatrix(&v47, &a1->mtx1, a3);
        v6 = v47.Pos.x - a1->mtx2.Pos.x;
        v7 = a1->mtx2.Pos.y;
        v25 = a1->f2;
        v40.z = v47.Pos.z - a1->mtx2.Pos.z;
        v43.q1 = (float)(v6 * v25);
        v8 = (v47.Pos.y - v7) * v25;
        v9 = v40.z * v25;
        a1->vv.x = v43.q1 + a1->vv.x;
        a1->vv.y = (float)(a1->vv.y + v8);
        a1->vv.z = (float)(a1->vv.z + v9);
        a1->vv.z = a1->fff1 + a1->vv.z;
        v10 = a1->fff2;
        a1->vv.x = (float)(v10 * a1->vv.x);
        a1->vv.y = (float)(v10 * a1->vv.y);
        a1->vv.z = (float)(v10 * a1->vv.z);
        v11 = TIM_gf_dt * a1->vv.x;
        v12 = a1->vv.y * TIM_gf_dt;
        v43.q3 = a1->vv.z * TIM_gf_dt;
        a1->mtx2.Pos.x = (float)(v11 + a1->mtx2.Pos.x);
        a1->mtx2.Pos.y = (float)(a1->mtx2.Pos.y + v12);
        a1->mtx2.Pos.z = a1->mtx2.Pos.z + v43.q3;
        if ((a1->upperFlag & 0x400) != 0)
        {
            v13 = v3->x - v47.Pos.x;
            v14 = a1->mtx2.Pos.y - v47.Pos.y;
            v15 = a1->mtx2.Pos.z - v47.Pos.z;
            v41 = (float)(v47.v1.z * v15 + v47.v1.y * v14 + v47.v1.x * v13);
            v26 = (float)(v47.v2.z * v15 + v47.v2.y * v14 + v47.v2.x * v13);
            v16 = v13 * v47.v3.x + v47.v3.z * v15 + v47.v3.y * v14;
            if (v41 < (double)a1->ff1)
                v41 = a1->ff1;
            if (v26 < (double)a1->ff2)
                v26 = a1->ff2;
            if (v16 < a1->ff3)
                v16 = a1->ff3;
            if (v41 > (double)a1->upperMax)
                v41 = a1->upperMax;
            if (v26 > (double)a1->ff5)
                v26 = a1->ff5;
            if (v16 > a1->ff6)
                v16 = a1->ff6;
            v35 = v47.v1.z * v41;
            v43.q2 = v47.v2.y * v26;
            v43.q3 = v47.v2.z * v26;
            v28 = v47.v2.x * v26 + v47.v1.x * v41;
            v31 = v43.q2 + v47.v1.y * v41;
            v36 = v43.q3 + v35;
            v43.q1 = (float)(v47.v3.x * v16);
            v43.q2 = (float)(v47.v3.y * v16);
            v32 = v43.q2 + v31;
            v37 = (float)(v16 * v47.v3.z + v36);
            v3->x = v43.q1 + v28 + v47.Pos.x;
            a1->mtx2.Pos.y = v32 + v47.Pos.y;
            a1->mtx2.Pos.z = v37 + v47.Pos.z;
        }
        v17 = &a2->Pos;
        v18 = a2->Pos.x - a3->Pos.x;
        v42 = &a2->Pos;
        v29 = (float)v18;
        v33 = a2->Pos.y - a3->Pos.y;
        v38 = a2->Pos.z - a3->Pos.z;
        v40.x = v3->x - a3->Pos.x;
        v40.y = a1->mtx2.Pos.y - a3->Pos.y;
        v40.z = a1->mtx2.Pos.z - a3->Pos.z;
        v27 = (float)sqrt(v33 * v33 + v29 * v29 + v38 * v38);
        v19 = sqrt(v40.z * v40.z + v40.y * v40.y + v40.x * v40.x);
        if (v19 != 0.0)
        {
            v20 = 1.0 / v19;
            v21 = v27 * v20;
            v43.q2 = (float)(v40.y * v21);
            v43.q3 = (float)(v40.z * v21);
            v3->x = (float)(v40.x * v21 + a3->Pos.x);
            a1->mtx2.Pos.y = a3->Pos.y + v43.q2;
            a1->mtx2.Pos.z = a3->Pos.z + v43.q3;
            if (v27 != 0.0)
            {
                v22 = 1.0 / v27;
                v30 = (float)(v29 * v22);
                v34 = (float)(v33 * v22);
                v39 = (float)(v38 * v22);
                v40.y = (float)(v40.y * v20);
                v40.z = (float)(v40.z * v20);
                v40.x = (float)(v30 + v40.x * v20);
                v40.y = v34 + v40.y;
                v40.z = v39 + v40.z;
                MATH_NormalizeVector(&v40, &v40);
                v43.q1 = v34 * v40.z - v39 * v40.y;
                v43.q2 = v39 * v40.x - v40.z * v30;
                v43.q3 = v40.y * v30 - v34 * v40.x;
                v43.q4 = v39 * v40.z + v34 * v40.y + v30 * v40.x;
                MATH_ConvertQuaternionToMatrix(&a3a, &v43);
                v23 = a2;
                if (&a2a != a2)
                {
                    memcpy(&a2a, a2, sizeof(a2a));
                    v23 = a2;
                }
                MATH_Mul33MatrixMatrix(v23, &a2a, &a3a, 1);
                v17 = v42;
            }
        }
        a1->vv.x = a1->mtx2.Pos.x - v44;
        a1->vv.y = a1->mtx2.Pos.y - v45;
        a1->vv.z = a1->mtx2.Pos.z - v46;
        v24 = 1.0 / TIM_gf_dt;
        a1->vv.x = (float)(v24 * a1->vv.x);
        a1->vv.y = (float)(v24 * a1->vv.y);
        a1->vv.z = (float)(v24 * a1->vv.z);
        *v17 = a1->mtx2.Pos;
    }
}

void GAO_ModifierLazy_Compute(tLazyMdfStruct* a1, Mtx4x4* a2, Mtx4x4* a3)
{
    int v3;
    double v4;
    double v5;
    Mtx4x4* v6;
    double v7;
    double v8;
    Vec3* v9; 
    double v10;
    double v11;
    double v12;
    double v13;
    Vec3* v14;
    long double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    double v24;
    double v25;
    double v26;
    double v27;
    BOOL v28;
    char v29;
    double v30;
    double v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    Vec3 src1;
    float v40;
    float scale;
    Vec3 src2;
    float a4;
    int v44;
    Quat4 dest;
    Mtx4x4 a3a;
    Mtx4x4 a2a;

    v3 = a1->upperFlag;
    if ((v3 & 0x1000) != 0)
    {
        GAO_ModifierLazy_Compute_SemiMecanical_Model(a1, a2, a3);
    }
    else
    {
        v4 = 1.0;
        v5 = 1.0 - a1->f2;
        if (v5 < 0.0)
            v5 = 0.0;
        v6 = a3;
        v44 = a1->upperFlag;
        scale = (float)(1.0 - v5 * v5);
        v7 = 0.0;
        if (!a3)
        {
            v3 &= ~8u;
            v44 = v3;
        }
        if ((v3 & 8) != 0)
        {
            v3 &= ~2u;
            v44 = v3;
        }
        if (TIM_gf_dt > 0.0)
        {
            do
            {
                v4 = v4 * scale;
                v7 = v7 + 0.0033333334;
            } while (v7 < TIM_gf_dt);
        }
        v40 = (float)(1.0 - v4);
        if ((v3 & 9) != 0)
        {
            v8 = a2->Pos.x - a1->mtx2.Pos.x;
            v9 = &a1->mtx2.Pos;
            v10 = a2->Pos.y - a1->mtx2.Pos.y;
            src2.z = a2->Pos.z - a1->mtx2.Pos.z;
            src1.x = (float)(v8 * v40);
            v11 = v10 * v40;
            v12 = src2.z * v40;
            a1->mtx2.Pos.x = src1.x + a1->mtx2.Pos.x;
            a1->mtx2.Pos.y = (float)(a1->mtx2.Pos.y + v11);
            a1->mtx2.Pos.z = (float)(a1->mtx2.Pos.z + v12);
            if ((v3 & 8) != 0)
            {
                a1->mtx2.Pos.z = v40 * a1->fff1 + a1->mtx2.Pos.z;
                if ((v3 & 0x400) != 0)
                {
                    MATH_MulMatrixMatrix(&a3a, &a1->mtx1, a3);
                    src1.x = v9->x - a3a.Pos.x;
                    src1.y = a1->mtx2.Pos.y - a3a.Pos.y;
                    src1.z = a1->mtx2.Pos.z - a3a.Pos.z;
                    v13 = a3a.v1.z * src1.z + a3a.v1.y * src1.y + a3a.v1.x * src1.x;
                    a4 = a3a.v2.x * src1.x + a3a.v2.z * src1.z + a3a.v2.y * src1.y;
                    scale = a3a.v3.z * src1.z + a3a.v3.y * src1.y + a3a.v3.x * src1.x;
                    if (v13 < a1->ff1)
                        v13 = a1->ff1;
                    if (a4 < (double)a1->ff2)
                        a4 = a1->ff2;
                    if (scale < (double)a1->ff3)
                        scale = a1->ff3;
                    if (v13 > a1->upperMax)
                        v13 = a1->upperMax;
                    if (a4 > (double)a1->ff5)
                        a4 = a1->ff5;
                    if (scale > (double)a1->ff6)
                        scale = a1->ff6;
                    src1.x = (float)(a3a.v1.x * v13);
                    src1.y = (float)(a3a.v1.y * v13);
                    src1.z = (float)(a3a.v1.z * v13);
                    MATH_AddScaleVector(&src1, &src1, &a3a.v2, a4);
                    MATH_AddScaleVector(&src1, &src1, &a3a.v3, scale);
                    v6 = a3;
                    v3 = (v3 & ~0xff) | (v44 & 0xff); //LOBYTE(v3) = v44;
                    v9->x = src1.x + a3a.Pos.x;
                    a1->mtx2.Pos.y = src1.y + a3a.Pos.y;
                    a1->mtx2.Pos.z = src1.z + a3a.Pos.z;
                }
                v14 = &v6->Pos;
                src1.x = a2->Pos.x - v14->x;
                src1.y = a2->Pos.y - v14->y;
                src1.z = a2->Pos.z - v14->z;
                src2.x = v9->x - v14->x;
                src2.y = a1->mtx2.Pos.y - v14->y;
                src2.z = a1->mtx2.Pos.z - v14->z;
                scale = (float)sqrt(src1.x * src1.x + src1.z * src1.z + src1.y * src1.y);
                v15 = sqrt(src2.z * src2.z + src2.y * src2.y + src2.x * src2.x);
                if (v15 != 0.0)
                {
                    a4 = (float)(1.0 / v15);
                    v37 = a4 * scale;
                    MATH_AddScaleVector(&a1->mtx2.Pos, v14, &src2, v37);
                    if (scale != 0.0)
                    {
                        v38 = (float)(1.0 / scale);
                        MATH_VecMultScalarWithoutBuffer(&src1, v38);
                        MATH_VecMultScalarWithoutBuffer(&src2, a4);
                        src2.x = src1.x + src2.x;
                        src2.y = src1.y + src2.y;
                        src2.z = src1.z + src2.z;
                        MATH_NormalizeVector(&src2, &src2);
                        MATH_CrossProduct((Vec3*)&dest, &src1, &src2);
                        dest.q4 = src1.z * src2.z + src1.y * src2.y + src1.x * src2.x;
                        MATH_ConvertQuaternionToMatrix(&a3a, &dest);
                        MATH_CopyMatrix(&a2a, a2);
                        MATH_Mul33MatrixMatrix(a2, &a2a, &a3a, 1);
                    }
                    v3 = (v3 & ~0xff) | (v44 & 0xff); //LOBYTE(v3) = v44;
                }
            }
            a2->Pos.x = v9->x;
            a2->Pos.y = a1->mtx2.Pos.y;
            a2->Pos.z = a1->mtx2.Pos.z;
        }
        if ((v3 & 2) != 0)
        {
            v16 = a2->v1.x - a1->mtx2.v1.x;
            v17 = a2->v1.y - a1->mtx2.v1.y;
            src1.z = a2->v1.z - a1->mtx2.v1.z;
            src2.x = (float)(v16 * v40);
            v18 = v17 * v40;
            v19 = src1.z * v40;
            a1->mtx2.v1.x = src2.x + a1->mtx2.v1.x;
            a1->mtx2.v1.y = (float)(a1->mtx2.v1.y + v18);
            a1->mtx2.v1.z = (float)(a1->mtx2.v1.z + v19);
            v20 = a2->v2.x - a1->mtx2.v2.x;
            v21 = a2->v2.y - a1->mtx2.v2.y;
            src1.z = a2->v2.z - a1->mtx2.v2.z;
            src2.x = (float)(v20 * v40);
            v22 = v21 * v40;
            v23 = src1.z * v40;
            a1->mtx2.v2.x = src2.x + a1->mtx2.v2.x;
            a1->mtx2.v2.y = (float)(a1->mtx2.v2.y + v22);
            a1->mtx2.v2.z = (float)(a1->mtx2.v2.z + v23);
            v24 = a2->v3.x - a1->mtx2.v3.x;
            v25 = a2->v3.y - a1->mtx2.v3.y;
            src1.z = a2->v3.z - a1->mtx2.v3.z;
            src2.x = (float)(v24 * v40);
            v26 = v25 * v40;
            v27 = src1.z * v40;
            a1->mtx2.v3.x = src2.x + a1->mtx2.v3.x;
            a1->mtx2.v3.y = (float)(a1->mtx2.v3.y + v26);
            a1->mtx2.v3.z = (float)(a1->mtx2.v3.z + v27);
            MATH_Orthonormalize(&a1->mtx2);
            a2->v1.x = a1->mtx2.v1.x;
            a2->v1.y = a1->mtx2.v1.y;
            a2->v1.z = a1->mtx2.v1.z;
            a2->v2 = a1->mtx2.v2;
            a2->v3.x = a1->mtx2.v3.x;
            a2->v3.y = a1->mtx2.v3.y;
            v3 = (v3 & ~0xff) | (v44 & 0xff); //LOBYTE(v3) = v44;
            a2->v3.z = a1->mtx2.v3.z;
        }
        if ((v3 & 4) != 0)
        {
            v28 = (a2->mtxflag & 8) == 0;
            src1.z = 1.0;
            src1.y = 1.0;
            src1.x = 1.0;
            if (!v28)
            {
                v29 = a1->mtx2.mtxflag;
                v30 = a2->_m14;
                v31 = a2->_m24;
                dest.q3 = a2->_m34;
                if ((v29 & 8) != 0)
                {
                    v32 = a1->mtx2._m24;
                    v33 = a1->mtx2._m34;
                    src1.x = a1->mtx2._m14;
                    src1.y = v32;
                    src1.z = v33;
                }
                else
                {
                    a1->mtx2.mtxflag |= 8u;
                }
                src2.x = (float)(v30 - src1.x);
                src2.y = (float)(v31 - src1.y);
                src2.z = dest.q3 - src1.z;
                MATH_AddScaleVector(&src1, &src1, &src2, v40);
                v34 = src1.x;
                v35 = src1.y;
                a2->_m14 = src1.x;
                a1->mtx2._m14 = v34;
                v36 = src1.z;
                a2->_m24 = v35;
                a1->mtx2._m24 = v35;
                a2->_m34 = v36;
                a1->mtx2._m34 = v36;
            }
        }
    }
}

void GAO_ModifierLazy_Inherit(tLazyMdfStruct* a1, tGameObjectMain* a2, tGameObjectMain* a3)
{
    tGaoMatrix* v3;
    tGaoMatrix* v4;
    Mtx4x4* v5;

    if ((a2->flags & 0x400000) != 0)
    {
        v3 = a2->basic->mtx;
        v4 = a3->basic->mtx;
        if ((a1->upperFlag & 0x10) != 0)
        {
            v4->mtx.Pos.x = v3->mtx.Pos.x;
            v4->mtx.Pos.y = v3->mtx.Pos.y;
            v4->mtx.Pos.z = v3->mtx.Pos.z;
        }
        if ((a1->upperFlag & 0x20) != 0)
        {
            v4->mtx.v1.x = v3->mtx.v1.x;
            v4->mtx.v1.y = v3->mtx.v1.y;
            v4->mtx.v1.z = v3->mtx.v1.z;
            v4->mtx.v2.x = v3->mtx.v2.x;
            v4->mtx.v2.y = v3->mtx.v2.y;
            v4->mtx.v2.z = v3->mtx.v2.z;
            v4->mtx.v3.x = v3->mtx.v3.x;
            v4->mtx.v3.y = v3->mtx.v3.y;
            v4->mtx.v3.z = v3->mtx.v3.z;
        }
        if ((a1->upperFlag & 0x40) != 0)
        {
            if ((v3->mtx.mtxflag & 8) != 0)
            {
                v4->mtx.mtxflag |= 8u;
                v4->mtx._m14 = v3->mtx._m14;
                v4->mtx._m24 = v3->mtx._m24;
                v4->mtx._m34 = v3->mtx._m34;
                return;
            }
        LABEL_14:
            v4->mtx.mtxflag &= ~8u;
            return;
        }
    }
    else
    {
        v4 = a3->basic->mtx;
        if ((a1->upperFlag & 0x20) != 0)
        {
            v5 = a2->matrices;
            v4->mtx.v1.x = v5->v1.x;
            v4->mtx.v1.y = v5->v1.y;
            v4->mtx.v1.z = v5->v1.z;
            v4->mtx.v2 = a2->matrices->v2;
            v4->mtx.v3 = a2->matrices->v3;
        }
        if ((a1->upperFlag & 0x40) != 0)
        {
            if ((a2->matrices->mtxflag & 8) != 0)
            {
                v4->mtx.mtxflag |= 8u;
                v4->mtx._m14 = a2->matrices->_m14;
                v4->mtx._m24 = a2->matrices->_m24;
                v4->mtx._m34 = a2->matrices->_m34;
                return;
            }
            goto LABEL_14;
        }
    }
}

void GAO_ModifierLazy_Apply(tModifier* a1, void *unk1)
{
    tLazyMdfStruct* v1;
    float v2;
    float v3;
    int v4;
    float v5;
    float v6;
    float v7;
    float v8;
    tModifier* v9;
    tGameObjectMain* v10;
    tGaoMatrix* v11;
    tGameObjectMain* v12;
    tGameObjectExtended* v13;
    tModifier* v14;
    int v15;
    int v16;
    Mtx4x4* v17;
    Mtx4x4* v18;
    tLazyMdfStruct* v19;
    tGameObjectMain* a1a;
    tGaoMatrix* v21;
    Mtx4x4 a3;

    v1 = (tLazyMdfStruct*)a1->mdfPrivateDataPtr;
    v2 = v1->fff1;
    UpperFriction = v1->fff2;
    v1->upperFriction = UpperFriction;
    v3 = v1->f2;
    v1->ffffdf = v2;
    UpperGravity = v2;
    UpperFactor = v3;
    v1->ffff2 = v3;
    UpperFlag = v1->upperFlag;
    v4 = UpperFlag;
    v1->upperFlagBak = UpperFlag;
    UpperMax = v1->upperMax;
    v5 = v1->ff6;
    LazyFF5_dword_98C72C = v1->ff5;
    LazyFF6_dword_98C730 = v5;
    v1->aaf1 = v1->upperMax;
    v6 = v1->ff6;
    v1->aaf2 = v1->ff5;
    v1->aaf3 = v6;
    UpperMin = v1->ff1;
    v7 = v1->ff2;
    LazyFF3_dword_98C73C = v1->ff3;
    LazyFF2_dword_98C738 = v7;
    v1->af1 = v1->ff1;
    v8 = v1->ff3;
    v1->af2 = v1->ff2;
    v1->af3 = v8;
    if ((v4 & 0x10000000) == 0)
    {
        v9 = a1;
        v10 = a1->gao;
        if ((v10->flags & 0x400000) != 0)
        {
            v11 = v10->basic->mtx;
            v12 = v11->refGao2;
            v21 = v11;
            a1a = v12;
            if (v12)
            {
                if ((v12->flags & 0x2000) != 0)
                {
                    v13 = v12->extended;
                    if (v13)
                    {
                        v14 = v13->mdf;
                        if (v14)
                        {
                            do
                            {
                                if (v14->modifierIntf->mdfId == 24)// lazy
                                {
                                    v15 = v14->modifierFlags;
                                    if ((v15 & 0xA000000) == 0 && (v15 & 0x1000000) != 0)
                                    {
                                        GAO_ModifierLazy_Apply(v14, NULL);
                                        v12 = a1a;
                                    }
                                }
                                v14 = v14->nextElem;
                            } while (v14);
                            v9 = a1;
                        }
                    }
                }
                if ((v1->upperFlag & 0x80) != 0)
                    v1->upperFlag = UpperFlag;
                v16 = v1->upperFlag;
                if ((v16 & 0x100) != 0)
                {
                    v1->f2 = UpperFactor;
                    v1->fff1 = UpperGravity;
                    v1->fff2 = UpperFriction;
                }
                if ((v16 & 0x800) != 0)
                {
                    v1->upperMax = UpperMax;
                    v1->ff5 = LazyFF5_dword_98C72C;
                    v1->ff6 = LazyFF6_dword_98C730;
                    v1->ff1 = UpperMin;
                    v1->ff2 = LazyFF2_dword_98C738;
                    v1->ff3 = LazyFF3_dword_98C73C;
                }
                if ((v16 & 0x70) != 0)
                    GAO_ModifierLazy_Inherit(v1, v12, v9->gao);
                OBJ_ComputeGlobalWhenHie(a1->gao);
                if (&v1->mtx1 != (Mtx4x4*)v21)
                    memcpy(&v1->mtx1, v21, sizeof(v1->mtx1));
                GAO_ModifierLazy_Compute(v1, a1->gao->matrices, a1a->matrices);
                v17 = OBJ_pst_GetAbsoluteMatrix(a1a);
                MATH_InvertMatrix(&a3, v17);
                MATH_MulMatrixMatrix(&v21->mtx, a1->gao->matrices, &a3);
            }
        }
        else
        {
            v18 = v10->matrices;
            if (&v1->mtx1 != v18)
                memcpy(&v1->mtx1, v18, sizeof(v1->mtx1));
            GAO_ModifierLazy_Compute(v1, a1->gao->matrices, 0);
        }
        v19 = p_FirsToUnapply;
        v1->upperFlag |= 0x10000000u;
        v1->gao = a1->gao;
        v1->firstToUnapply = v19;
        p_FirsToUnapply = v1;
    }
}

int Lazy_FirstInit()
{
    int result;

    result = bFirst_286_1;
    if (!bFirst_286_1)
        bFirst_286_1 = 1;
    return result;
}

void GAO_ModifierLazy_Reinit(tModifier* a1)
{
    tGameObjectMain* v1;
    tLazyMdfStruct* v2;
    Mtx4x4* v3;
    Mtx4x4* v4;
    Mtx4x4* v5;
    Mtx4x4* v6;

    v1 = a1->gao;
    v2 = (tLazyMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1)
    {
        if ((v1->flags & 0x400000) != 0)
        {
            if (v1->basic->mtx->refGao2)
            {
                OBJ_ComputeGlobalWhenHie(a1->gao);
                v3 = a1->gao->matrices;
                v4 = &v2->mtx2;
                if (v4 != v3)
                    memcpy(v4, v3, sizeof(Mtx4x4));
            }
        }
        else
        {
            v5 = v1->matrices;
            v6 = &v2->mtx2;
            if (v6 != v5)
                memcpy(v6, v5, sizeof(Mtx4x4));
        }
    }
}

void GAO_ModifierLazy_Create(struct sGameObjectMain* a1, tModifier* a2, void* a3)
{
    tLazyMdfStruct* v3;

    Lazy_FirstInit();
    v3 = (tLazyMdfStruct*)MEM_p_AllocAligned(sizeof(tLazyMdfStruct), 16);
    a2->mdfPrivateDataPtr = (void *)v3;
    a2->modifierFlags = 0x1000000;
    if (a3)
    {
        memcpy(v3, a3, sizeof(tLazyMdfStruct));
    }
    else
    {
        memset(v3, 0, sizeof(tLazyMdfStruct));
        v3->upperFlag = 0;
        v3->f2 = 0.89999998f;
        v3->ff1 = -0.1f;
        v3->ff2 = -0.1f;
        v3->ff3 = -0.1f;
        v3->upperMax = 0.1f;
        v3->ff5 = 0.1f;
        v3->ff6 = 0.1f;
    }
    GAO_ModifierLazy_Reinit(a2);
}

void GAO_ModifierLazy_Destroy(tModifier* a1)
{
    if (a1->mdfPrivateDataPtr)
        MEM_FreeAlign(a1->mdfPrivateDataPtr);
}

int GAO_ul_ModifierLazy_Load(tModifier* arg0, char* a1)
{
    char* v2;
    tLazyMdfStruct* v3;
    unsigned int v4;

    v2 = a1;
    v3 = (tLazyMdfStruct*)arg0->mdfPrivateDataPtr;
    LOA_ReadLong(&a1, 0, 2);
    v4 = LOA_ReadLong(&a1, 0, 1);
    v3->f2 = LOA_ReadFloat(&a1, 0, 1);
    v3->upperFlag = LOA_ReadLong(&a1, 0, 1);
    if (v4)
    {
        v3->ff1 = LOA_ReadFloat(&a1, 0, 1);
        v3->ff2 = LOA_ReadFloat(&a1, 0, 1);
        v3->ff3 = LOA_ReadFloat(&a1, 0, 1);
        v3->upperMax = LOA_ReadFloat(&a1, 0, 1);
        v3->ff5 = LOA_ReadFloat(&a1, 0, 1);
        v3->ff6 = LOA_ReadFloat(&a1, 0, 1);
        if (v4 > 1)
        {
            v3->fff1 = LOA_ReadFloat(&a1, 0, 1);
            if (v4 > 2)
            {
                v3->fff2 = LOA_ReadFloat(&a1, 0, 1);
                return a1 - v2;
            }
        }
    }
    else
    {
        v3->ff1 = -1.0;
        v3->ff2 = -1.0;
        v3->ff3 = -1.0;
        v3->upperMax = 1.0;
        v3->ff5 = 1.0;
        v3->ff6 = 1.0;
    }
    return a1 - v2;
}

void GAO_ModifierLazy_Unapply(struct sModifier* a1, void* unk1) {
}

void GAO_ModifierLazy_UnApply_Local(tLazyMdfStruct* a1)
{
    float v1;
    float v2;
    float v3;
    float v4;
    tGameObjectMain* v5;
    Mtx4x4* v6;
    tGaoMatrix* v7;

    v1 = a1->upperFriction;
    a1->fff1 = a1->ffffdf;
    v2 = a1->ffff2;
    a1->fff2 = v1;
    a1->f2 = v2;
    a1->upperMax = a1->aaf1;
    v3 = a1->aaf3;
    a1->ff5 = a1->aaf2;
    a1->ff6 = v3;
    a1->ff1 = a1->af1;
    v4 = a1->af3;
    a1->ff2 = a1->af2;
    a1->ff3 = v4;
    v5 = a1->gao;
    a1->upperFlag = a1->upperFlagBak & ~0x10000000u;
    v6 = &a1->mtx1;
    if ((v5->flags & 0x400000) != 0)
        v7 = v5->basic->mtx;
    else
        v7 = (tGaoMatrix*)v5->matrices;
    if (v7 != (tGaoMatrix*)v6)
        memcpy(v7, v6, sizeof(Mtx4x4));
}

void GAO_ModifierLazy_UnapplyAll()
{
    tLazyMdfStruct* i;

    gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    CAM_Inverse(&gpst_DX_stDD->ct);
    gpst_DX_stDD->SetViewMatrix(&gpst_DX_stDD->ct.cam.cameraMtx);
    for (i = p_FirsToUnapply; i; i = i->firstToUnapply)
        GAO_ModifierLazy_UnApply_Local(i);
    p_FirsToUnapply = NULL;
}