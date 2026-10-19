#include "common.h"
#include "memory.h"
#include "math.h"

void MATH_CreateTrigTables()
{
    int i;

    for (i = 0; i < 1024; i++) {
        MATH_gf_TableSin[i] = (float)sin((i + 1) * (MATH_f_2Pi / 1024.0f));
    }
}

void MATH_CreateSqrtTable()
{
    int i;
    float sqr, sqOf2, sqrsqrOf2;

    sqOf2 = (float)sqrt(2.0f);

    for (i = 0; i < 1024; i++) {
        sqr = (float)sqrt(i * (1 / 1024.0f) + 1.0f);
        sqrsqrOf2 = sqOf2 * sqr;
        MATH_gl_TableRacine[i + 1024] = (*(uint32_t*)&sqr) & 0x7fffff;
        MATH_gl_TableRacine[i] = (*(uint32_t*)&sqrsqrOf2) & 0x7fffff;
    }
}

void MATH_CreateInvTable()
{
    int i;
    float inv;

    MATH_gl_TableInverse[0] = 0x800000;

    for (i = 1; i < 1024; i++) {
        inv = (1.0f / (i * (1 / 1024.0f) + 1.0f));
        MATH_gl_TableInverse[i] = (*(uint32_t*)&inv) & 0x7fffff;
    }
}

void MATH_CreateInvSqrtTable()
{
    int i;
    float sqrOf2, sqr, inv1, inv2;

    sqrOf2 = (float)sqrt(2.0f);

    for (i = 0; i < 1024; i++) {
        sqr = (float)sqrt(i * (1 / 1024.0f) + 1.0f);
        inv1 = (2.0f / sqr);
        inv2 = (sqrOf2 / sqr);

        MATH_gl_TableInverseRacine[i + 1024] = (*(uint32_t*)&inv1) & 0x7fffff;
        MATH_gl_TableInverseRacine[i] = ((*(uint32_t*)&inv2) & 0x7fffff) + 0x800000;
    }

    MATH_gl_TableInverseRacine[1024] = 0x800000;
}

void MATH_InitModule()
{
    srand(1);
    MATH_gf_1024by2Pi = 162.97466f;
    MATH_gd_magicDoubleForRound = pow(2.0, 51.0) * 3.0;
    MATH_CreateInvSqrtTable();
    MATH_CreateInvTable();
    MATH_CreateSqrtTable();
    MATH_CreateTrigTables();
    MATH_gst_IdentityMatrix.v1.y = 0.0f;
    MATH_gst_IdentityMatrix.v1.z = 0.0f;
    MATH_gst_IdentityMatrix._m14 = 0.0f;
    MATH_gst_IdentityMatrix.v2.x = 0.0f;
    MATH_gst_IdentityMatrix.v2.z = 0.0f;
    MATH_gst_IdentityMatrix._m24 = 0.0f;
    MATH_gst_IdentityMatrix.v3.x = 0.0f;
    MATH_gst_IdentityMatrix.v3.y = 0.0f;
    MATH_gst_IdentityMatrix._m34 = 0.0f;
    MATH_gst_IdentityMatrix.Pos.x = 0.0f;
    MATH_gst_IdentityMatrix.Pos.y = 0.0f;
    MATH_gst_IdentityMatrix.Pos.z = 0.0f;
    MATH_gstUnknownMatrix.v1.y = 0.0f;
    MATH_gstUnknownMatrix.v1.z = 0.0f;
    MATH_gstUnknownMatrix._m14 = 0.0f;
    MATH_gstUnknownMatrix.v2.x = 0.0f;
    MATH_gstUnknownMatrix.v2.z = 0.0f;
    MATH_gstUnknownMatrix._m24 = 0.0f;
    MATH_gstUnknownMatrix.v3.x = 0.0f;
    MATH_gstUnknownMatrix.v3.y = 0.0;
    MATH_gstUnknownMatrix._m34 = 0.0f;
    MATH_gstUnknownMatrix.Pos.x = 0.0f;
    MATH_gstUnknownMatrix.Pos.y = 0.0f;
    MATH_gstUnknownMatrix.Pos.z = 0.0f;
    MATH_gst_IdentityMatrix.mtxflag = MATH_gst_IdentityMatrix.mtxflag & 2;
    MATH_gst_IdentityMatrix.v1.x = 1.0f;
    MATH_gst_IdentityMatrix.v2.y = 1.0f;
    MATH_gst_IdentityMatrix.v3.z = 1.0f;
    MATH_gst_IdentityMatrix.w = 1.0f;
    MATH_gstUnknownMatrix.v1.x = 1.0f;
    MATH_gstUnknownMatrix.v2.y = 1.0f;
    MATH_gstUnknownMatrix.v3.z = 1.0f;
    MATH_gstUnknownMatrix.w = 1.0f;
    MATH_gstUnknownMatrix.mtxflag = MATH_gstUnknownMatrix.mtxflag & 2;
    MATH_gst_IdentityVector.x = 1.0f;
    MATH_gst_IdentityVector.y = 1.0f;
    MATH_gst_IdentityVector.z = 1.0f;
    MATH_gst_BaseVectorI.x = 1.0f;
    MATH_gst_BaseVectorI.y = 0.0f;
    MATH_gst_BaseVectorI.z = 0.0f;
    MATH_gst_BaseVectorJ.x = 0.0f;
    MATH_gst_BaseVectorJ.y = 1.0f;
    MATH_gst_BaseVectorJ.z = 0.0f;
    MATH_gst_BaseVectorK.x = 0.0f;
    MATH_gst_BaseVectorK.y = 0.0f;
    MATH_gst_BaseVectorK.z = 1.0f;
    MATH_gst_NulVector.x = 0.0f;
    MATH_gst_NulVector.y = 0.0f;
    MATH_gst_NulVector.z = 0.0f;
    MATH_CreateTrigTables();
}

void MATH_CopyMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    if (dest != src)
        memcpy(dest, src, sizeof(Mtx4x4));
}

void MATH_SetIdentityMatrix(Mtx4x4* mtx)
{
    mtx->v1.x = 1.0f;
    mtx->v1.y = 0.0f;
    mtx->v1.z = 0.0f;
    mtx->_m14 = 0.0f;
    mtx->v2.x = 0.0f;
    mtx->v2.y = 1.0f;
    mtx->v2.z = 0.0f;
    mtx->_m24 = 0.0f;
    mtx->v3.x = 0.0f;
    mtx->v3.y = 0.0f;
    mtx->v3.z = 1.0f;
    mtx->_m34 = 0.0f;
    mtx->Pos.x = 0.0f;
    mtx->Pos.y = 0.0f;
    mtx->Pos.z = 0.0f;
    mtx->w = 1.0f;
    mtx->mtxflag = mtx->mtxflag & 0x2;
}

void MATH_NormalizeVector(Vec3* dest, Vec3* src)
{
    float normLen;

    normLen = 1.0f / (float)sqrt(src->z * src->z + src->y * src->y + src->x * src->x);
    dest->x = normLen * src->x;
    dest->y = normLen * src->y;
    dest->z = normLen * src->z;
}

void MATH_CrossProduct(Vec3* dest, Vec3* src1, Vec3* src2)
{
    dest->x = src1->y * src2->z - src1->z * src2->y;
    dest->y = src1->z * src2->x - src1->x * src2->z;
    dest->z = src1->x * src2->y - src1->y * src2->x; // fixed: dest->z = src1->x * src2->y - src1->x * src2->y volt
}

void MATH_NormalizeEqualVector(Vec3* v)
{
    long double len;

    len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x = (float)(1.0 / len * v->x);
    v->y = (float)(1.0 / len * v->y);
    v->z = (float)(1.0 / len * v->z);
}

BOOL MATH_b_NulVector(Vec3* v)
{
    if (fabs(v->x) != 0.0f || fabs(v->y) != 0.0f || fabs(v->z) != 0.0f) return FALSE;
    return TRUE;
}

BOOL MATH_b_EqVectorWithEpsilon(Vec3* vec1, Vec3* vec2, float epsilon)
{
    if (fabs(vec1->x - vec2->x) > epsilon)
        return FALSE;
    if (fabs(vec1->y - vec2->y) > epsilon)
        return FALSE;
    if (fabs(vec1->z - vec2->z) > epsilon)
        return FALSE;
    
    return TRUE;
}

float MATH_f_VecSin(Vec3* v1, Vec3* v2, Vec3* v3)
{    
    Vec3 tmpv1, tmpv2, cp;
    float len;

    MATH_NormalizeVector(&tmpv1, v1);
    MATH_NormalizeVector(&tmpv2, v2);
    MATH_CrossProduct(&cp, &tmpv1, &tmpv2);
    
    len = MATH_VecLen(&cp);

    if (v3 && cp.x * v3->x + v3->y * cp.y + v3->z * cp.z < 0.0f)
        return -len;
    else
        return len;
}

float MATH_f_VecCos(Vec3* v1, Vec3* v2)
{
    Vec3 tmpv1, tmpv2;

    MATH_NormalizeVector(&tmpv1, v1);
    MATH_NormalizeVector(&tmpv2, v2);

    return tmpv1.x * tmpv2.x + tmpv1.y * tmpv2.y + tmpv1.z * tmpv2.z;
}

float MATH_f_VecAngle(Vec3* v1, Vec3* v2, Vec3* v3)
{
    float vc, vs, ret;

    vc = MATH_f_VecCos(v1, v2);

    if (vc >= 1.0f)
        return 0.0f;

    if (vc > -1.0f)
        ret = (float)acos(vc);
    else
        ret = MATH_f_Pi;

    vs = MATH_f_VecSin(v1, v2, v3);

   if (vs < 0.0f)
       ret = -ret;

    return ret;
}

float MATH_f_ZAngle(Vec3* v1, Vec3* v2)
{
    float ret;

    Vec3 tmpVec1 = { v1->x, v1->y, 0.0f };
    Vec3 tmpVec2 = { v2->x, v2->y, 0.0f };

    MATH_NormalizeEqualVector(&tmpVec1);
    MATH_NormalizeEqualVector(&tmpVec2);

    ret = MATH_f_VecAngle(&tmpVec1, &tmpVec2, &MATH_gst_BaseVectorK);
    if (ret > MATH_f_Pi)
        ret -= MATH_f_2Pi;
    if (ret < -MATH_f_Pi)
        ret += MATH_f_2Pi;
    return (float)ret;
}

float MATH_f_XAngle(Vec3* v1, Vec3* v2)
{
    float v1AngleAbs, v2AngleAbs, ret;
    Vec3 tmpVec1 = { v1->x, v1->y, 0.0f };
    Vec3 tmpVec2 = { v2->x, v2->y, 0.0f };

    MATH_NormalizeEqualVector(&tmpVec1);
    MATH_NormalizeEqualVector(&tmpVec2);

    v1AngleAbs = (float)fabs(MATH_f_VecAngle(&tmpVec1, v1, &MATH_gst_BaseVectorI));
    
    if (v1->z < 0.0f)
        v1AngleAbs = -v1AngleAbs;

    v2AngleAbs = (float)fabs(MATH_f_VecAngle(&tmpVec2, v2, &MATH_gst_BaseVectorI));
    
    if (v2->z < 0.0f)
        v2AngleAbs = -v2AngleAbs;
    
    ret = v1AngleAbs - v2AngleAbs;
    
    if (ret > MATH_f_Pi)
        ret -= MATH_f_2Pi;
    if (ret < -MATH_f_Pi)
        ret += MATH_f_2Pi;
    if (ret > MATH_f_PiBy2)
        ret = MATH_f_Pi - ret;
    if (ret < -MATH_f_PiBy2)
        ret = -MATH_f_Pi - ret;
    return ret;
}

float MATH_DistanceOfTwoPoints(Vec3* v1, Vec3* v2)
{
    double xDif = v1->x - v2->x;
    double yDif = v1->y - v2->y;
    double zDif = v1->z - v2->z;

    return (float)sqrt(xDif * xDif + yDif * yDif + zDif * zDif);
}

void MATH_AddScaleVector(Vec3* dest, Vec3* src1, Vec3* src2, float scale)
{
    dest->x = src2->x * scale + src1->x;
    dest->y = src2->y * scale + src1->y;
    dest->z = src2->z * scale + src1->z;
}

void MATH_TransformVector(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    float srcX;
    float srcY;

    if (dest == src)
    {
        srcX = src->x;
        srcY = src->y;
        if ((mtx->mtxflag & 8) != 0)
        {
            src->x = mtx->v3.x * mtx->_m34 * src->z + mtx->v2.x * mtx->_m24 * src->y + mtx->_m14 * mtx->v1.x * src->x;
            src->y = mtx->v1.y * mtx->_m14 * srcX + mtx->v3.y * mtx->_m34 * src->z + mtx->v2.y * mtx->_m24 * src->y;
            src->z = mtx->v2.z * mtx->_m24 * srcY + mtx->v1.z * mtx->_m14 * srcX + mtx->v3.z * mtx->_m34 * src->z;
        }
        else
        {
            src->x = mtx->v2.x * src->y + mtx->v3.x * src->z + mtx->v1.x * src->x;
            src->y = mtx->v2.y * src->y + mtx->v1.y * srcX + mtx->v3.y * src->z;
            src->z = mtx->v2.z * srcY + mtx->v1.z * srcX + mtx->v3.z * src->z;
        }
    }
    else
    {
        if ((mtx->mtxflag & 8) != 0)
        {
            dest->x = mtx->v3.x * mtx->_m34 * src->z + mtx->v2.x * mtx->_m24 * src->y + mtx->_m14 * mtx->v1.x * src->x;
            dest->y = mtx->v1.y * mtx->_m14 * src->x + mtx->v3.y * mtx->_m34 * src->z + mtx->v2.y * mtx->_m24 * src->y;
            dest->z = mtx->v1.z * mtx->_m14 * src->x + mtx->v3.z * mtx->_m34 * src->z + mtx->v2.z * mtx->_m24 * src->y;
        }
        else
        {
            dest->x = mtx->v2.x * src->y + mtx->v3.x * src->z + mtx->v1.x * src->x;
            dest->y = mtx->v2.y * src->y + mtx->v1.y * src->x + mtx->v3.y * src->z;
            dest->z = mtx->v2.z * src->y + mtx->v1.z * src->x + mtx->v3.z * src->z;
        }
    }
}

void MATH_TransformVertex(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    MATH_TransformVector(dest, mtx, src);
    dest->x = mtx->Pos.x + dest->x;
    dest->y = mtx->Pos.y + dest->y;
    dest->z = mtx->Pos.z + dest->z;
}

void MATH_TransformVectorNoScale(Vec3 *dest, Mtx4x4 *mtx, Vec3 *src)
{
  float srcX;
  float srcY;

  if ( dest == src )
  {
    srcX = src->x;
    srcY = src->y;
    src->x = mtx->v2.x * src->y + mtx->v3.x * src->z + mtx->v1.x * src->x;
    src->y = mtx->v2.y * src->y + mtx->v1.y * srcX + mtx->v3.y * src->z;
    src->z = mtx->v2.z * srcY + mtx->v1.z * srcX + mtx->v3.z * src->z;
  }
  else
  {
    dest->x = mtx->v2.x * src->y + mtx->v3.x * src->z + mtx->v1.x * src->x;
    dest->y = mtx->v2.y * src->y + mtx->v1.y * src->x + mtx->v3.y * src->z;
    dest->z = mtx->v2.z * src->y + mtx->v1.z * src->x + mtx->v3.z * src->z;
  }
}

void MATH_TransformVertexNoScale(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    MATH_TransformVectorNoScale(dest, mtx, src);
    dest->x = mtx->Pos.x + dest->x;
    dest->y = mtx->Pos.y + dest->y;
    dest->z = mtx->Pos.z + dest->z;
}

void MATH_Copy33Matrix(Mtx4x4* dest, Mtx4x4* src)
{
    dest->v1 = src->v1;
    dest->v2 = src->v2;
    dest->v3 = src->v3;
    if ((src->mtxflag & 0x4) != 0)
        dest->mtxflag |= 0x4;
}

void MATH_Mul33MatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2, char flagSet)
{
    if ((src1->mtxflag & 0x3c) != 0)
    {
        if ((src2->mtxflag & 0x3c) != 0)
        {
            dest->v1.x = src1->v1.x * src2->v1.x + src1->v1.y * src2->v2.x + src2->v3.x * src1->v1.z;
            dest->v1.y = src2->v3.y * src1->v1.z + src2->v1.y * src1->v1.x + src2->v2.y * src1->v1.y;
            dest->v1.z = src1->v1.x * src2->v1.z + src1->v1.y * src2->v2.z + src1->v1.z * src2->v3.z;
            dest->v2.x = src1->v2.z * src2->v3.x + src1->v2.y * src2->v2.x + src1->v2.x * src2->v1.x;
            dest->v2.y = src1->v2.y * src2->v2.y + src1->v2.x * src2->v1.y + src1->v2.z * src2->v3.y;
            dest->v2.z = src1->v2.y * src2->v2.z + src1->v2.x * src2->v1.z + src1->v2.z * src2->v3.z;
            dest->v3.x = src2->v2.x * src1->v3.y + src2->v1.x * src1->v3.x + src2->v3.x * src1->v3.z;
            dest->v3.y = src2->v2.y * src1->v3.y + src2->v1.y * src1->v3.x + src2->v3.y * src1->v3.z;
            dest->v3.z = src1->v3.z * src2->v3.z + src1->v3.y * src2->v2.z + src1->v3.x * src2->v1.z;
            if (flagSet)
            {
                if ((src1->mtxflag & 0x4) != 0 || (src2->mtxflag & 0x4) != 0)
                    dest->mtxflag |= 0x4;
            }
        }
        else
        {
            MATH_Copy33Matrix(dest, src1);
        }
    }
    else
    {
        MATH_Copy33Matrix(dest, src2);
    }
}

void MATH_Transp33MatrixWithoutBuffer(Mtx4x4* dest, Mtx4x4* src)
{
    dest->v1.x = src->v1.x;
    dest->v2.x = src->v1.y;
    dest->v3.x = src->v1.z;
    dest->v1.y = src->v2.x;
    dest->v2.y = src->v2.y;
    dest->v3.y = src->v2.z;
    dest->v3.z = src->v3.z;
    dest->v1.z = src->v3.x;
    dest->v2.z = src->v3.y;
}

void MATH_InvertMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    MATH_Transp33MatrixWithoutBuffer(dest, src);
    dest->mtxflag = src->mtxflag;
    if ((src->mtxflag & 0x8) != 0)
    {
        dest->_m14 = 1.0f / src->_m14;
        dest->_m24 = 1.0f / src->_m24;
        dest->_m34 = 1.0f / src->_m34;
    }
    dest->Pos.x = -src->Pos.x;
    dest->Pos.y = -src->Pos.y;
    dest->Pos.z = -src->Pos.z;
    MATH_TransformVector(&dest->Pos, dest, &dest->Pos);
}

void MATH_VectorGlobalToLocal(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    Mtx4x4 invMtx;

    MATH_InvertMatrix(&invMtx, mtx);
    MATH_TransformVectorNoScale(dest, &invMtx, src);
}

void MATH_VertexGlobalToLocal(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    Mtx4x4 invMtx;

    MATH_InvertMatrix(&invMtx, mtx);
    MATH_TransformVertex(dest, &invMtx, src);
}

void MATH_GetScale(Vec3* scale, Mtx4x4* mtx)
{
    if ((mtx->mtxflag & 0x8) != 0)
    {
        scale->x = mtx->_m14;
        scale->y = mtx->_m24;
        scale->z = mtx->_m34;
    }
    else
    {
        scale->x = 1.0f;
        scale->y = 1.0f;
        scale->z = 1.0f;
    }
}

void MATH_SetScale(Mtx4x4* mtx, Vec3* scale)
{
    if (MATH_b_EqVectorWithEpsilon(&MATH_gst_IdentityVector, scale, 0.000099999997f)
        || MATH_b_EqVectorWithEpsilon(&MATH_gst_NulVector, scale, 0.000099999997f))
    {
        mtx->_m14 = 0.0;
        mtx->_m24 = 0.0;
        mtx->_m34 = 0.0;
        mtx->mtxflag &= ~0x8;
    }
    else
    {
        mtx->_m14 = scale->x;
        mtx->_m24 = scale->y;
        mtx->_m34 = scale->z;
        mtx->mtxflag |= 0x8;
    }
}

void MATH_SetZoom(Mtx4x4* mtx, float zoom)
{
    if (fabs(1.0f - zoom) < 0.000099999997f)
    {
        mtx->_m14 = 0.0;
        mtx->_m24 = 0.0;
        mtx->_m34 = 0.0;
        mtx->mtxflag &= ~8u;
    }
    else
    {
        mtx->_m14 = zoom;
        mtx->_m24 = zoom;
        mtx->_m34 = zoom;
        mtx->mtxflag |= 8u;
    }
}

void MATH_NormalizeVectorWithNullVectorCheck(Vec3* dest, Vec3* src)
{    
    if (!MATH_b_NulVector(src))
    {
        MATH_NormalizeVector(dest, src);
    }
    else
    {
        dest->x = 0.0f;
        dest->y = 0.0f;
        dest->z = 0.0f;
    }
}

void MATH_SetNormVector(Vec3* dest, Vec3* src, float norm)
{
    MATH_NormalizeVector(dest, src);
    MATH_VecMultScalarWithoutBuffer(dest, norm);
}

void MATH_BlendVector(Vec3* dest, Vec3* v1, Vec3* v2, float amount)
{
    if (amount < 0.0f)
        amount = 0.0f;
    if (amount > 1.0f)
        amount = 1.0f;
    dest->x = (1.0f - amount) * v1->x + amount * v2->x;
    dest->y = (1.0f - amount) * v1->y + amount * v2->y;
    dest->z = (1.0f - amount) * v1->z + amount * v2->z;
}

BOOL MATH_b_VecColinear(Vec3* v1, Vec3* v2)
{
    Vec3 cp;
    
    MATH_CrossProduct(&cp, v1, v2);
    
    if (fabs(cp.x) > 0.0000001f)
        return FALSE;
    if (fabs(cp.y) > 0.0000001f)
        return FALSE;
    return fabs(cp.z) <= 0.0000001f;
}

void MATH_InterpolateBetweenTwoPosWithFactor(Vec3* dest, Vec3* src1, Vec3* src2, float amount)
{
    MATH_SubstractVec(dest, src1, src2);
    MATH_AddScaleVector(dest, src2, dest, amount);
}

void MATH_GetRotationMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    dest->v1 = src->v1;
    dest->v2 = src->v2;
    dest->v3 = src->v3;
    if ((src->mtxflag & 0x4) != 0)
        dest->mtxflag |= 0x4;
    dest->_m14 = 0.0f;
    dest->_m24 = 0.0f;
    dest->_m34 = 0.0f;
    dest->mtxflag = dest->mtxflag & ~0x8;
    dest->Pos.x = 0.0f;
    dest->Pos.y = 0.0f;
    dest->Pos.z = 0.0f;
    dest->mtxflag = dest->mtxflag & ~0x2 | 0x4;
}

void MATH_PointLineProjection(Vec3* destProjection, Vec3* point, Vec3* line1, Vec3* line2)
{
    float dotProduct;
    Vec3 tmp32, tmp12;

    MATH_SubstractVec(&tmp32, line2, line1);
    MATH_SubstractVec(&tmp12, point, line1);
    MATH_NormalizeEqualVector(&tmp32);

    dotProduct = tmp12.z * tmp32.z + tmp12.y * tmp32.y + tmp12.x * tmp32.x; // dotproduct

    MATH_AddScaleVector(destProjection, line1, &tmp32, dotProduct);
}

void MATH_PointSegmentProjection(Vec3* dest, Vec3* point, Vec3* line1, Vec3* line2)
{
    MATH_PointLineProjection(dest, point, line1, line2);
    if ((line2->z - dest->z) * (dest->z - line1->z)
        + (line2->y - dest->y) * (dest->y - line1->y)
        + (line2->x - dest->x) * (dest->x - line1->x) < 0.0)
    {
        if ((line2->x - line1->x) * (dest->x - line1->x) +
            (line2->y - line1->y) * (dest->y - line1->y) +
            (line2->z - line1->z) * (dest->z - line1->z) < 0.0)
        {
            dest->x = line1->x;
            dest->y = line1->y;
            dest->z = line1->z;
        }
        else
        {
            dest->x = line2->x;
            dest->y = line2->y;
            dest->z = line2->z;
        }
    }
}

double MATH_PowLimitedCE(float a1)
{
    if (a1 > 0.0)
        return pow(a1, 0.3333333432674408);
    if (a1 >= 0.0)
        return 0.0;
    return -pow(-a1, 0.3333333432674408);
}

int MATH_i_SolveCubicEquation(float a1, float a2, float a3, Vec3* dest)
{
    double v4;
    long double v5;
    double v7;
    int ret;
    double v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    double v16;
    float v20;
    float v21;
    float v22;
    float v23;

    v4 = a1 * a1;
    v20 = (float)((a2 - 0.33333334 * v4) * 0.33333334);
    v23 = (float)((v4 * a1 * 0.074074075 - a1 * a2 * 0.33333334 + a3) * 0.5);
    v21 = v20 * v20 * v20;
    v5 = v23 * v23 + v21;
    if (fabs(v5) >= 0.0000099999997)
    {
        if (v5 >= 0.0)
        {
            v13 = sqrt(v5);
            v22 = (float)MATH_PowLimitedCE((float)(v13 - v23));
            v14 = (float)MATH_PowLimitedCE((float)(v13 + v23));
            v15 = v22 - v14;
            dest->x = (float)v15;
            dest->y = (float)v15;
            dest->z = (float)v15;
            ret = 1;
            v16 = a1 * 0.33333334;
            dest->x = (float)(dest->x - v16);
            dest->y = (float)(dest->y - v16);
            dest->z = (float)(dest->z - v16);
            return ret;
        }
        v10 = acos(-(v23 / sqrt(-v21))) * 0.33333334;
        ret = 3;
        v11 = sqrt(-v20);
        v12 = v11 + v11;
        dest->x = (float)(cos(v10) * v12);
        dest->y = (float)(-(cos(v10 + 1.0471976) * v12));
        dest->z = (float)(-(cos(v10 - 1.0471976) * v12));
    }
    else
    {
        if (fabs(v23) < 0.0000099999997)
        {
            dest->x = 0.0;
            dest->y = 0.0;
            dest->z = 0.0;
            ret = 1;
            v16 = a1 * 0.33333334;
            dest->x = (float)(dest->x - v16);
            dest->y = (float)(dest->y - v16);
            dest->z = (float)(dest->z - v16);
            return ret;
        }
        v7 = MATH_PowLimitedCE(-v23);
        ret = 2;
        dest->x = (float)(v7 + v7);
        dest->y = (float)(-v7);
        dest->z = (float)(-v7);
    }
    v16 = a1 * 0.33333334;
    dest->x = (float)(dest->x - v16);
    dest->y = (float)(dest->y - v16);
    dest->z = (float)(dest->z - v16);
    return ret;
}

void MATH_PointParabolProjection2D(Vec3* a1, Vec3* a2, float a3, float a4)
{
    double v4;
    int v5;
    double v6;
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    double v13;
    float a2a;
    float a3a;
    float v16;
    float v17;
    Vec3 a4a;

    if (fabs(a3) >= 0.00000000099999997)
    {
        v4 = 1.0 / a3;
        a3a = (float)(-(0.5 * v4 * a2->x * v4));
        a2a = (float)((0.5 * v4 + a4 - a2->y) * v4);
        v5 = MATH_i_SolveCubicEquation(0.0, a2a, a3a, &a4a);
        v6 = a4a.x;
        if (v5 > 1)
        {
            v7 = v6 * a4a.x * a3 + a4 - a2->y;
            v8 = a4a.x - a2->x;
            v9 = v7 * v7 + v8 * v8;
            v10 = a4a.y * a4a.y * a3 + a4 - a2->y;
            v11 = a4a.y - a2->x;
            v16 = (float)(v10 * v10 + v11 * v11);
            v12 = a4a.z - a2->x;
            v13 = a4a.z * a4a.z * a3 + a4 - a2->y;
            v17 = (float)(v13 * v13 + v12 * v12);
            if (v9 >= v16)
            {
                if (v16 < (double)v17)
                {
                    a1->x = a4a.y;
                    a1->z = 0.0;
                    a1->y = (float)(v6 * v6 * a3 + a4);
                    return;
                }
            }
            else if (v9 < v17)
            {
                a1->x = a4a.x;
                a1->z = 0.0;
                a1->y = (float)(v6 * v6 * a3 + a4);
                return;
            }
            v6 = a4a.z;
        }
        a1->x = (float)v6;
        a1->z = 0.0;
        a1->y = (float)(v6 * v6 * a3 + a4);
        return;
    }
    a1->y = a4;
    a1->x = a2->x;
    a1->z = 0.0;
}

void MATH_PointParabolProjection3D(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5, Vec3* a6, float a7)
{
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    float a4a;
    float v19;
    float v20;
    Vec3 src;
    Vec3 a3a;
    Mtx4x4 a2a;
    Vec3 a1a;
    Vec3 v25;
    Vec3 v26;
    Vec3 v27;
    Vec3 dest;

    v7 = a4->x - a6->x;
    v8 = a4->y - a6->y;
    a3a.z = a4->z - a6->z;
    src.x = a5->x - a6->x;
    src.y = a5->y - a6->y;
    src.z = a5->z - a6->z;
    v9 = 1.0 / sqrt(v8 * v8 + v7 * v7 + a3a.z * a3a.z);
    a3a.x = (float)(v9 * v7);
    v10 = v8 * v9;
    v11 = v9 * a3a.z;
    v12 = 1.0 / sqrt(src.z * src.z + src.y * src.y + src.x * src.x);
    src.x = (float)(src.x * v12);
    src.y = (float)(src.y * v12);
    src.z = (float)(src.z * v12);
    v13 = src.x - a3a.x;
    v14 = src.y - v10;
    a2a.v1.z = (float)(src.z - v11);
    a2a.v2.x = src.x + a3a.x;
    a2a.v2.y = (float)(src.y + v10);
    a2a.v2.z = (float)(src.z + v11);
    v19 = (float)(1.0 / sqrt(a2a.v1.z * a2a.v1.z + v14 * v14 + v13 * v13));
    a2a.v1.x = (float)(v13 * v19);
    a2a.v1.y = (float)(v14 * v19);
    a2a.v1.z = a2a.v1.z * v19;
    v15 = 1.0 / sqrt(a2a.v2.z * a2a.v2.z + a2a.v2.y * a2a.v2.y + a2a.v2.x * a2a.v2.x);
    a2a.v2.x = (float)(a2a.v2.x * v15);
    a2a.v2.y = (float)(a2a.v2.y * v15);
    a2a.v2.z = (float)(a2a.v2.z * v15);
    a2a.v3.x = a2a.v2.z * a2a.v1.y - a2a.v2.y * a2a.v1.z;
    a2a.Pos.x = a6->x;
    a2a.Pos.y = a6->y;
    a2a.Pos.z = a6->z;
    a2a.v3.y = a2a.v2.x * a2a.v1.z - a2a.v2.z * a2a.v1.x;
    a2a._m14 = 0.0;
    a2a._m24 = 0.0;
    a2a._m34 = 0.0;
    a2a.mtxflag = (a2a.mtxflag | 2) & ~8u;
    a2a.v3.z = a2a.v2.y * a2a.v1.x - a2a.v2.x * a2a.v1.y;
    a3a.x = a3a.x * a7;
    a3a.y = (float)(v10 * a7);
    src.y = src.y * a7;
    src.z = src.z * a7;
    a3a.x = a3a.x + a6->x;
    a3a.y = a3a.y + a6->y;
    a3a.z = (float)(v11 * a7 + a6->z);
    src.x = src.x * a7 + a6->x;
    src.y = src.y + a6->y;
    src.z = src.z + a6->z;
    MATH_VertexGlobalToLocal(&a1a, &a2a, &a3a);
    MATH_VertexGlobalToLocal(&dest, &a2a, &src);
    MATH_VertexGlobalToLocal(&v27, &a2a, a3);
    v20 = a1a.y / (a1a.x * a1a.x + a1a.x * a1a.x);
    a4a = a1a.y * 0.5f;
    MATH_PointParabolProjection2D(&v26, &v27, v20, a4a);
    MATH_TransformVertex(a1, &a2a, &v26);
    v25.y = v26.x * v20 + v26.x * v20;
    v25.x = 1.0;
    v25.z = 0.0;
    MATH_TransformVectorNoScale(a2, &a2a, &v25);
}

void MATH_VecRound(Vec3* v)
{
    double x;
    double y;
    double z;

    x = v->x * 10000.0 + MATH_gd_magicDoubleForRound;
    y = v->y * 10000.0 + MATH_gd_magicDoubleForRound;
    z = v->z * 10000.0 + MATH_gd_magicDoubleForRound;
    v->x = (float)((*(int32_t*)&x) * 0.000099999997);
    v->y = (float)((*(int32_t*)&y) * 0.000099999997);
    v->z = (float)((*(int32_t*)&z) * 0.000099999997);
}

void MATH_GetZoom(Vec3* zoom, Mtx4x4* mtx)
{
    zoom->x = (float)(sqrt(mtx->v1.x * mtx->v1.x + mtx->v1.y * mtx->v1.y + mtx->v1.z * mtx->v1.z));
    zoom->y = (float)(sqrt(mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y + mtx->v2.z * mtx->v2.z));
    zoom->z = (float)(sqrt(mtx->v3.x * mtx->v3.x + mtx->v3.y * mtx->v3.y + mtx->v3.z * mtx->v3.z));
    MATH_VecRound(zoom);
}

void MATH_SetZoomBuffered(Mtx4x4* dest, Mtx4x4* mtx, Vec3* zoom)
{
    dest->v1.x = mtx->v1.x * zoom->x;
    dest->v1.y = mtx->v1.y * zoom->x;
    dest->v1.z = mtx->v1.z * zoom->x;
    dest->v2.x = mtx->v2.x * zoom->y;
    dest->v2.y = mtx->v2.y * zoom->y;
    dest->v2.z = mtx->v2.z * zoom->y;
    dest->v3.x = mtx->v3.x * zoom->z;
    dest->v3.y = mtx->v3.y * zoom->z;
    dest->v3.z = mtx->v3.z * zoom->z;
}

void MATH_MakeOGLMatrix(Mtx4x4* dest, Mtx4x4* src)
{
    if ((src->mtxflag & 8) != 0)
    {       
        dest->v1.x = src->_m14 * src->v1.x;
        dest->v1.y = src->v1.y * src->_m14;
        dest->v1.z = src->v1.z * src->_m14;
        dest->v2.x = src->v2.x * src->_m24;
        dest->v2.y = src->v2.y * src->_m24;
        dest->v2.z = src->v2.z * src->_m24;
        dest->v3.x = src->v3.x * src->_m34;
        dest->v3.y = src->v3.y * src->_m34;
        dest->v3.z = src->v3.z * src->_m34;

        dest->Pos = src->Pos;
        dest->_m14 = 0.0;
        dest->_m24 = 0.0;
        dest->_m34 = 0.0;
        dest->mtxflag &= ~8u;
        dest->w = 1.0;
    }
    else
    {
        if (dest != src)
            memcpy(dest, src, sizeof(Mtx4x4));
        dest->w = 1.0;
    }
}

void MATH_Mul44MatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2)
{
    dest->v1.x = src1->v1.x * src2->v1.x + src1->v1.y * src2->v2.x + src2->v3.x * src1->v1.z;
    dest->v1.y = src1->v1.y * src2->v2.y + src2->v3.y * src1->v1.z + src1->v1.x * src2->v1.y;
    dest->v1.z = src2->v3.z * src1->v1.z + src1->v1.y * src2->v2.z + src2->v1.z * src1->v1.x;
    dest->v2.x = src2->v2.x * src1->v2.y + src1->v2.z * src2->v3.x + src2->v1.x * src1->v2.x;
    dest->v2.y = src1->v2.z * src2->v3.y + src1->v2.y * src2->v2.y + src1->v2.x * src2->v1.y;
    dest->v2.z = src2->v2.z * src1->v2.y + src2->v1.z * src1->v2.x + src2->v3.z * src1->v2.z;
    dest->v3.x = src1->v3.z * src2->v3.x + src2->v2.x * src1->v3.y + src1->v3.x * src2->v1.x;
    dest->v3.y = src1->v3.y * src2->v2.y + src1->v3.x * src2->v1.y + src1->v3.z * src2->v3.y;
    dest->v3.z = src1->v3.y * src2->v2.z + src1->v3.x * src2->v1.z + src1->v3.z * src2->v3.z;
    dest->Pos.x = src2->v2.x * src1->Pos.y + src1->Pos.z * src2->v3.x + src2->v1.x * src1->Pos.x + src2->Pos.x;
    dest->Pos.y = src1->Pos.z * src2->v3.y + src1->Pos.y * src2->v2.y + src1->Pos.x * src2->v1.y + src2->Pos.y;
    dest->Pos.z = src2->v2.z * src1->Pos.y + src2->v1.z * src1->Pos.x + src2->v3.z * src1->Pos.z + src2->Pos.z;
}

void MATH_MulMatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2)
{
    Vec3 zoom;
    Mtx4x4 oglsrc2;
    Mtx4x4 oglsrc1;

    dest->mtxflag = 0;
    if ((src1->mtxflag & 8) != 0 || (src2->mtxflag & 8) != 0)
    {
        MATH_MakeOGLMatrix(&oglsrc1, src1);
        MATH_MakeOGLMatrix(&oglsrc2, src2);
        MATH_Mul44MatrixMatrix(dest, &oglsrc1, &oglsrc2);
        MATH_GetZoom(&zoom, dest);
        MATH_SetScale(dest, &zoom);
        if ((dest->mtxflag & 8) != 0)
        {
            zoom.x = (float)(1.0f / zoom.x);
            zoom.y = (float)(1.0f / zoom.y);
            zoom.z = (float)(1.0f / zoom.z);
            MATH_SetZoomBuffered(dest, dest, &zoom);
        }
    }
    else
    {
        MATH_Mul44MatrixMatrix(dest, src1, src2);
        dest->_m34 = 0.0;
        dest->_m24 = 0.0;
        dest->_m14 = 0.0;
        dest->w = 1.0;
    }
    if ((src1->mtxflag & 4) != 0 || (src2->mtxflag & 4) != 0)
        dest->mtxflag |= 4u;
    if (fabs(dest->Pos.x) != 0 || fabs(dest->Pos.y) != 0 || fabs(dest->Pos.z) != 0)
    {
        dest->mtxflag |= 2u;
    }
    else
    {
        dest->mtxflag &= ~2u;
    }
}

void MATH_MakeRotationMatrix_AxisAngle(Mtx4x4* a1, Vec3* a2, float a3, Quat4* a4, char a5)
{
    Quat4* v5;
    float v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    unsigned int v14;
    unsigned int v15;
    float v16;
    float v17;
    float v18;
    Quat4 v19;
    float a2a;
    float a3a;
    float a4a;
    float a4b;

    v5 = a4;
    v6 = a3 * 0.5f;
    v7 = (float)sin(v6);
    if (!a4)
        v5 = &v19;
    v5->q1 = v7 * a2->x;
    v5->q2 = a2->y * v7;
    v5->q3 = a2->z * v7;
    v8 = (float)cos(v6);
    v5->q4 = v8;
    v9 = v5->q1 + v5->q1;
    a4a = v5->q2 + v5->q2;
    a3a = v5->q3 + v5->q3;
    v17 = v8 * v9;
    v16 = v8 * a4a;
    a2a = v8 * a3a;
    v10 = v9 * v5->q1;
    v11 = a4a * v5->q1;
    v12 = a3a * v5->q1;
    v18 = a4a * v5->q2;
    a4b = a3a * v5->q2;
    v13 = a3a * v5->q3;
    a1->v1.x = 1.0f - (v18 + v13);
    a1->v1.y = v11 + a2a;
    a1->v1.z = v12 - v16;
    a1->v2.x = v11 - a2a;
    v14 = a1->mtxflag;
    a1->v2.y = 1.0f - (v13 + v10);
    a1->v2.z = a4b + v17;
    a1->v3.x = v12 + v16;
    a1->v3.y = a4b - v17;
    a1->v3.z = 1.0f - (v18 + v10);
    if (a5)
    {
        a1->_m14 = 0.0;
        a1->_m24 = 0.0;
        a1->_m34 = 0.0;
        a1->mtxflag = v14 & ~8u;
        a1->Pos.x = 0.0;
        a1->Pos.y = 0.0;
        a1->Pos.z = 0.0;
        v15 = a1->mtxflag & ~2u;
        a1->mtxflag = v15;
        v14 = v15;
    }
    a1->mtxflag = v14 | 4;
}

void MATH_RotateMatrix_AroundGlobalXAxis(Mtx4x4* mtx, float angle)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;

    v2 = cos(angle);
    mtx->mtxflag |= 4u;
    v3 = sin(angle);
    v4 = mtx->v1.y;
    v5 = mtx->v1.z;
    mtx->v1.y = (float)(v4 * v2 - v5 * v3);
    mtx->v1.z = (float)(v4 * v3 + v5 * v2);
    v6 = mtx->v2.y;
    v7 = mtx->v2.z;
    mtx->v2.y = (float)(v6 * v2 - v7 * v3);
    mtx->v2.z = (float)(v6 * v3 + v7 * v2);
    v8 = mtx->v3.y;
    v9 = mtx->v3.z;
    mtx->v3.y = (float)(v8 * v2 - v9 * v3);
    mtx->v3.z = (float)(v8 * v3 + v9 * v2);
}

void MATH_RotateMatrix_AroundGlobalYAxis(Mtx4x4* mtx, float angle)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;

    v2 = cos(angle);
    mtx->mtxflag |= 4u;
    v3 = sin(angle);
    v4 = mtx->v1.x;
    v5 = mtx->v1.z;
    mtx->v1.x = (float)(v5 * v3 + v4 * v2);
    mtx->v1.z = (float)(v5 * v2 - v4 * v3);
    v6 = mtx->v2.x;
    v7 = mtx->v2.z;
    mtx->v2.x = (float)(v7 * v3 + v6 * v2);
    mtx->v2.z = (float)(v7 * v2 - v6 * v3);
    v8 = mtx->v3.x;
    v9 = mtx->v3.z;
    mtx->v3.x = (float)(v9 * v3 + v8 * v2);
    mtx->v3.z = (float)(v9 * v2 - v8 * v3);
}

void MATH_RotateMatrix_AroundGlobalZAxis(Mtx4x4* mtx, float angle)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;

    v2 = cos(angle);
    mtx->mtxflag |= 4u;
    v3 = sin(angle);
    v4 = mtx->v1.x;
    v5 = mtx->v1.y;
    mtx->v1.x = (float)(v4 * v2 - v5 * v3);
    mtx->v1.y = (float)(v4 * v3 + v5 * v2);
    v6 = mtx->v2.x;
    v7 = mtx->v2.y;
    mtx->v2.x = (float)(v6 * v2 - v7 * v3);
    mtx->v2.y = (float)(v6 * v3 + v7 * v2);
    v8 = mtx->v3.x;
    v9 = mtx->v3.y;
    mtx->v3.x = (float)(v8 * v2 - v9 * v3);
    mtx->v3.y = (float)(v8 * v3 + v9 * v2);
}

void MATH_RotateMatrix_AroundLocalXAxis(Mtx4x4* mtx, float angle)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;

    v2 = cos(angle);
    mtx->mtxflag |= 4u;
    v3 = sin(angle);
    v4 = mtx->v2.x;
    v5 = mtx->v3.x;
    mtx->v2.x = (float)(v5 * v3 + v4 * v2);
    mtx->v3.x = (float)(v5 * v2 - v4 * v3);
    v6 = mtx->v2.y;
    v7 = mtx->v3.y;
    mtx->v2.y = (float)(v7 * v3 + v6 * v2);
    mtx->v3.y = (float)(v7 * v2 - v6 * v3);
    v8 = mtx->v2.z;
    v9 = mtx->v3.z;
    mtx->v2.z = (float)(v9 * v3 + v8 * v2);
    mtx->v3.z = (float)(v9 * v2 - v8 * v3);
}

void MATH_RotateMatrix_AroundLocalYAxis(Mtx4x4* mtx, float angle)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;

    v2 = cos(angle);
    mtx->mtxflag |= 4u;
    v3 = sin(angle);
    v4 = mtx->v1.x;
    v5 = mtx->v3.x;
    mtx->v1.x = (float)(v4 * v2 - v5 * v3);
    mtx->v3.x = (float)(v4 * v3 + v5 * v2);
    v6 = mtx->v1.y;
    v7 = mtx->v3.y;
    mtx->v1.y = (float)(v6 * v2 - v7 * v3);
    mtx->v3.y = (float)(v6 * v3 + v7 * v2);
    v8 = mtx->v1.z;
    v9 = mtx->v3.z;
    mtx->v1.z = (float)(v8 * v2 - v9 * v3);
    mtx->v3.z = (float)(v8 * v3 + v9 * v2);
}

void MATH_RotateMatrix_AroundLocalZAxis(Mtx4x4* mtx, float angle)
{
    double v2;
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;
    double v9;

    v2 = cos(angle);
    mtx->mtxflag |= 4u;
    v3 = sin(angle);
    v4 = mtx->v1.x;
    v5 = mtx->v2.x;
    mtx->v1.x = (float)(v5 * v3 + v4 * v2);
    mtx->v2.x = (float)(v5 * v2 - v4 * v3);
    v6 = mtx->v1.y;
    v7 = mtx->v2.y;
    mtx->v1.y = (float)(v7 * v3 + v6 * v2);
    mtx->v2.y = (float)(v7 * v2 - v6 * v3);
    v8 = mtx->v1.z;
    v9 = mtx->v2.z;
    mtx->v1.z = (float)(v9 * v3 + v8 * v2);
    mtx->v2.z = (float)(v9 * v2 - v8 * v3);
}

void MATH_MakeRotationMatrix_UsingSight(Mtx4x4* dest, Vec3* sight, Vec3* banking, char onlyRotation)
{
    double v4;
    double v7;
    double v12;
    Vec3 v16;

    v4 = sqrt(sight->z * sight->z + sight->y * sight->y + sight->x * sight->x);
    dest->v2.x = (float)(1.0 / v4 * sight->x);
    dest->v2.y = (float)(1.0 / v4 * sight->y);
    dest->v2.z = (float)(1.0 / v4 * sight->z);
    dest->v2.y = -dest->v2.y;
    dest->v2.z = -dest->v2.z;
    dest->v2.x = -dest->v2.x;

    if (banking)
    {
        v7 = sqrt(banking->z * banking->z + banking->y * banking->y + banking->x * banking->x);
        v16.x = (float)(1.0 / v7 * banking->x);
        v16.y = (float)(1.0 / v7 * banking->y);
        v16.z = (float)(1.0 / v7 * banking->z);
        dest->v1.x = v16.z * sight->y - v16.y * sight->z;
        dest->v1.y = v16.x * sight->z - v16.z * sight->x;
        dest->v1.z = v16.y * sight->x - v16.x * sight->y;
        if (fabs(dest->v1.x) <= 1e-09 && fabs(dest->v1.y) <= 1e-09 && fabs(dest->v1.z) <= 1e-09)
        {
            v16.x = (float)(v16.x + 0.000099999997);
            dest->v1.x = v16.z * sight->y - v16.y * sight->z;
            dest->v1.y = v16.x * sight->z - v16.z * sight->x;
            dest->v1.z = v16.y * sight->x - v16.x * sight->y;
        }
        dest->v3 = v16;
        dest->v1.x = -dest->v1.x;
        dest->v1.y = -dest->v1.y;
        dest->v1.z = -dest->v1.z;
    }
    else
    {
        dest->v1.x = dest->v2.y * dest->v3.z - dest->v2.z * dest->v3.y;
        dest->v1.y = dest->v3.x * dest->v2.z - dest->v3.z * dest->v2.x;
        dest->v1.z = dest->v2.x * dest->v3.y - dest->v2.y * dest->v3.x;
    }
    v12 = sqrt(dest->v1.x * dest->v1.x + dest->v1.y * dest->v1.y + dest->v1.z * dest->v1.z);
    dest->v1.x = (float)(1.0 / v12 * dest->v1.x);
    dest->v1.y = (float)(1.0 / v12 * dest->v1.y);
    dest->v1.z = (float)(1.0 / v12 * dest->v1.z);
    dest->v3.x = dest->v1.y * dest->v2.z - dest->v1.z * dest->v2.y;
    dest->v3.y = dest->v2.x * dest->v1.z - dest->v2.z * dest->v1.x;
    dest->v3.z = dest->v2.y * dest->v1.x - dest->v1.y * dest->v2.x;
    if (onlyRotation)
    {
        dest->mtxflag &= ~0x8;
        dest->_m14 = 0.0;
        dest->_m24 = 0.0;
        dest->_m34 = 0.0;
        dest->Pos.x = 0.0;
        dest->Pos.y = 0.0;
        dest->Pos.z = 0.0;
        dest->mtxflag &= ~0x2;
    }
    dest->mtxflag |= 0x4;
}

void MATH_MakeRotationMatrix_UsingBanking(Mtx4x4* mtx, Vec3* v1, Vec3* v2, char onlyRotation)
{
    double v4; // normalize equalvector croosproduct nullveectorwithepsilon inlineok
    double v5;
    double v7;

    v4 = sqrt(v2->z * v2->z + v2->y * v2->y + v2->x * v2->x);
    mtx->v3.x = (float)(1.0 / v4 * v2->x);
    mtx->v3.y = (float)(1.0 / v4 * v2->y);
    mtx->v3.z = (float)(1.0 / v4 * v2->z);
    if (v1)
    {
        mtx->v2.x = -v1->x;
        mtx->v2.y = -v1->y;
        mtx->v2.z = -v1->z;
        v5 = sqrt(mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y + mtx->v2.z * mtx->v2.z);
        mtx->v2.x = (float)(1.0 / v5 * mtx->v2.x);
        mtx->v2.y = (float)(1.0 / v5 * mtx->v2.y);
        mtx->v2.z = (float)(1.0 / v5 * mtx->v2.z);
    }
    mtx->v1.x = mtx->v2.y * mtx->v3.z - mtx->v2.z * mtx->v3.y;
    mtx->v1.y = mtx->v2.z * mtx->v3.x - mtx->v3.z * mtx->v2.x;
    mtx->v1.z = mtx->v3.y * mtx->v2.x - mtx->v2.y * mtx->v3.x;
    if (fabs(mtx->v1.x) <= 1e-07 && fabs(mtx->v1.y) <= 1e-07  && fabs(mtx->v1.z) <= 1e-07)
    {
        mtx->v2.x = MATH_gst_BaseVectorI.x + mtx->v2.x;
        mtx->v2.y = MATH_gst_BaseVectorI.y + mtx->v2.y;
        mtx->v2.z = MATH_gst_BaseVectorI.z + mtx->v2.z;
        mtx->v1.x = mtx->v2.y * mtx->v3.z - mtx->v2.z * mtx->v3.y;
        mtx->v1.y = mtx->v2.z * mtx->v3.x - mtx->v3.z * mtx->v2.x;
        mtx->v1.z = mtx->v3.y * mtx->v2.x - mtx->v2.y * mtx->v3.x;
    }
    v7 = sqrt(mtx->v1.x * mtx->v1.x + mtx->v1.y * mtx->v1.y + mtx->v1.z * mtx->v1.z);
    mtx->v1.x = (float)(1.0 / v7 * mtx->v1.x);
    mtx->v1.y = (float)(1.0 / v7 * mtx->v1.y);
    mtx->v1.z = (float)(1.0 / v7 * mtx->v1.z);
    mtx->v2.x = mtx->v1.z * mtx->v3.y - mtx->v1.y * mtx->v3.z;
    mtx->v2.y = mtx->v3.z * mtx->v1.x - mtx->v1.z * mtx->v3.x;
    mtx->v2.z = mtx->v3.x * mtx->v1.y - mtx->v1.x * mtx->v3.y;
    if (onlyRotation)
    {
        mtx->_m14 = 0.0;
        mtx->_m24 = 0.0;
        mtx->_m34 = 0.0;
        mtx->mtxflag = mtx->mtxflag & ~8u;
        mtx->Pos.x = 0.0;
        mtx->Pos.y = 0.0;
        mtx->Pos.z = 0.0;
        mtx->mtxflag &= ~2u;
    }
    mtx->mtxflag |= 4u;
}

void MATH_OrientMatrix_UsingSight(Mtx4x4* mtx, Vec3* v1, Vec3* v2)
{
    MATH_MakeRotationMatrix_UsingSight(mtx, v1, v2, 0);
}

void MATH_OrientMatrix_UsingBanking(Mtx4x4* mtx, Vec3* v1, Vec3* v2)
{
    MATH_MakeRotationMatrix_UsingBanking(mtx, v1, v2, 0);
}

void MATH_InvertQuaternion(Quat4* a1)
{
    a1->q1 = -a1->q1;
    a1->q2 = -a1->q2;
    a1->q3 = -a1->q3;
    a1->q4 = -a1->q4;
}

void MATH_NormQuat(Quat4* dest, Quat4* src)
{
    float len = (float)sqrt(src->q4 * src->q4 + src->q3 * src->q3 + src->q2 * src->q2 + src->q1 * src->q1);

    dest->q1 = 1.0f / len * src->q1;
    dest->q2 = 1.0f / len * src->q2;
    dest->q3 = 1.0f / len * src->q3;
    dest->q4 = 1.0f / len * src->q4;
}

void MATH_Invert33Matrix(Mtx4x4* dest, Mtx4x4* src)
{
    double v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v8;

    v3 = src->v3.z * src->v2.y - src->v3.y * src->v2.z;
    dest->v1.x = (float)v3;
    v4 = v3 * src->v1.x;
    v5 = src->v1.z * src->v3.y - src->v1.y * src->v3.z;
    dest->v1.y = (float)v5;
    v6 = v4 + v5 * src->v2.x;
    v7 = src->v2.z * src->v1.y - src->v1.z * src->v2.y;
    dest->v1.z = (float)v7;
    v8 = 1.0 / (v6 + v7 * src->v3.x);
    dest->v2.x = (float)((src->v3.x * src->v2.z - src->v3.z * src->v2.x) * v8);
    dest->v2.y = (float)((src->v1.x * src->v3.z - src->v1.z * src->v3.x) * v8);
    dest->v2.z = (float)((src->v1.z * src->v2.x - src->v2.z * src->v1.x) * v8);
    dest->v3.x = (float)((src->v3.y * src->v2.x - src->v3.x * src->v2.y) * v8);
    dest->v3.y = (float)((src->v3.x * src->v1.y - src->v3.y * src->v1.x) * v8);
    dest->v3.z = (float)((src->v1.x * src->v2.y - src->v1.y * src->v2.x) * v8);
    dest->v1.x = (float)(v8 * dest->v1.x);
    dest->v1.y = (float)(v8 * dest->v1.y);
    dest->v1.z = (float)(v8 * dest->v1.z);
}

void MATH_TranspEq33Matrix(Mtx4x4* mtx)
{
    float tmp;       

    tmp = mtx->v2.x;
    mtx->v2.x = mtx->v1.y;
    mtx->v1.y = tmp;
    tmp = mtx->v3.x;
    mtx->v3.x = mtx->v1.z;
    mtx->v1.z = tmp;
    tmp = mtx->v3.y;
    mtx->v3.y = mtx->v2.z;
    mtx->v2.z = tmp;
}

void MATH_Orthonormalize(Mtx4x4* mtx)
{    
    double v3;
    double v8; 
    double v13;

    v3 = sqrt(mtx->v1.x * mtx->v1.x + mtx->v1.y * mtx->v1.y + mtx->v1.z * mtx->v1.z);
    mtx->v1.x = (float)(1.0 / v3 * mtx->v1.x);
    mtx->v1.y = (float)(1.0 / v3 * mtx->v1.y);
    mtx->v1.z = (float)(1.0 / v3 * mtx->v1.z);
    mtx->v2.x = mtx->v1.z * mtx->v3.y - mtx->v1.y * mtx->v3.z;
    mtx->v2.y = mtx->v1.x * mtx->v3.z - mtx->v1.z * mtx->v3.x;
    mtx->v2.z = mtx->v3.x * mtx->v1.y - mtx->v3.y * mtx->v1.x;
    v8 = sqrt(mtx->v2.z * mtx->v2.z + mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y);
    mtx->v2.x = (float)(1.0 / v8 * mtx->v2.x);
    mtx->v2.y = (float)(1.0 / v8 * mtx->v2.y);
    mtx->v2.z = (float)(1.0 / v8 * mtx->v2.z);
    mtx->v3.x = mtx->v2.z * mtx->v1.y - mtx->v2.y * mtx->v1.z;
    mtx->v3.y = mtx->v1.z * mtx->v2.x - mtx->v2.z * mtx->v1.x;
    mtx->v3.z = mtx->v1.x * mtx->v2.y - mtx->v2.x * mtx->v1.y;
    v13 = sqrt(mtx->v3.z * mtx->v3.z + mtx->v3.x * mtx->v3.x + mtx->v3.y * mtx->v3.y);
    mtx->v3.x = (float)(1.0 / v13 * mtx->v3.x);
    mtx->v3.y = (float)(1.0 / v13 * mtx->v3.y);
    mtx->v3.z = (float)(1.0 / v13 * mtx->v3.z);
}

void MATH_Slerp(Quat4* dest, float a2, Quat4* src1, Quat4* src2, float a5, float a6)
{
    double v6;
    double v7;
    float v8;
    float v9;
    float v10;
    float v11;

    if (a5 == 0.0)
    {
        *dest = *src1;
    }
    else
    {
        v6 = sin((1.0 - a2) * a5) * a6;
        v7 = sin((double)a2 * a5) * a6;
        v8 = (float)(src1->q1 * v6);
        v9 = (float)(src1->q2 * v6);
        v10 = (float)(src1->q3 * v6);
        v11 = (float)(src1->q4 * v6);
        dest->q1 = (float)(src2->q1 * v7);
        dest->q2 = (float)(src2->q2 * v7);
        dest->q3 = (float)(src2->q3 * v7);
        dest->q4 = (float)(src2->q4 * v7);
        dest->q1 = v8 + dest->q1;
        dest->q2 = v9 + dest->q2;
        dest->q3 = v10 + dest->q3;
        dest->q4 = v11 + dest->q4;
        MATH_NormQuat(dest, dest);
    }
}

void MATH_ConvertQuaternionToMatrix(Mtx4x4* mtx, Quat4* q)
{
    double v3;
    double v4;
    double v5;
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
    double v17;
    float v18; 
    float v19; 
    float a2a; 

    v3 = q->q1 * q->q1;
    v4 = 0.5 - (v3 + v3);
    v5 = q->q2 * q->q1;
    v6 = v5 + v5;
    v7 = q->q3 * q->q1;
    v8 = v7 + v7;
    v9 = q->q4 * q->q1;
    v18 = (float)(v9 + v9);
    v10 = q->q2 * q->q2;
    v19 = (float)(0.5 - (v10 + v10));
    v11 = q->q3 * q->q2;
    a2a = (float)(v11 + v11);
    v12 = q->q4 * q->q2;
    v13 = v12 + v12;
    v14 = q->q3 * q->q3;
    v15 = 0.5 - (v14 + v14);
    v16 = q->q4 * q->q3;
    mtx->_m14 = 0.0;
    mtx->_m24 = 0.0;
    v17 = v16 + v16;
    mtx->_m34 = 0.0;
    mtx->mtxflag = mtx->mtxflag & ~8u | 4;
    mtx->v1.x = (float)(v15 + v19);
    mtx->v2.x = (float)(v6 - v17);
    mtx->v3.x = (float)(v13 + v8);
    mtx->v1.y = (float)(v17 + v6);
    mtx->v2.y = (float)(v15 + v4);
    mtx->v3.y = a2a - v18;
    mtx->v1.z = (float)(v8 - v13);
    mtx->v2.z = a2a + v18;
    mtx->v3.z = (float)(v19 + v4);
}

void MATH_InvNormalQuaternion(Quat4* dest, Quat4* src) {

    double len = sqrt(src->q4 * src->q4 + src->q3 * src->q3 + src->q2 * src->q2 + src->q1 * src->q1);
    dest->q1 = (float)(1.0 / len * src->q1);
    dest->q2 = (float)(1.0 / len * src->q2);
    dest->q3 = (float)(1.0 / len * src->q3);
    dest->q4 = (float)(1.0 / len * src->q4);
}

void MATH_ConvertMatrixToQuaternion(Quat4 *dest, Mtx4x4 *src) {
    float v2;
    double v3;
    double v4;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;

    v2 = src->v1.x + src->v3.z + src->v2.y;
    if (v2 <= 0.0) {
        if (src->v1.x < src->v2.y) {
            if (src->v2.y >= src->v3.z) {
                v11 = sqrt(1.0 - src->v1.x + src->v2.y - src->v3.z);
                dest->q2 = (float)(0.5 * v11);
                v9 = 0.5 / v11;
                dest->q3 = (float)((src->v3.y + src->v2.z) * v9);
                dest->q1 = (float)((src->v2.x + src->v1.y) * v9);
                v10 = src->v3.x - src->v1.z;
            }
            else {
                v8 = sqrt(1.0 - src->v1.x - src->v2.y + src->v3.z); // same
                dest->q3 = (float)(0.5 * v8);
                v9 = 0.5 / v8;
                dest->q1 = (float)((src->v3.x + src->v1.z) * v9);
                dest->q2 = (float)((src->v3.y + src->v2.z) * v9);
                v10 = src->v1.y - src->v2.x;
            }
        }
        else if (src->v1.x >= src->v3.z) {
            v12 = sqrt(src->v1.x + 1.0 - src->v2.y - src->v3.z);
            dest->q1 = (float)(0.5 * v12);
            v9 = 0.5 / v12;
            dest->q2 = (float)((src->v2.x + src->v1.y) * v9);
            dest->q3 = (float)((src->v3.x + src->v1.z) * v9);
            v10 = src->v2.z - src->v3.y;
        }
        else {
            v8 = sqrt(1.0 - src->v1.x - src->v2.y + src->v3.z); // same
            dest->q3 = (float)(0.5 * v8);
            v9 = 0.5 / v8;
            dest->q1 = (float)((src->v3.x + src->v1.z) * v9);
            dest->q2 = (float)((src->v3.y + src->v2.z) * v9);
            v10 = src->v1.y - src->v2.x;
        }

        dest->q4 = (float)(v10 * v9);
        MATH_InvNormalQuaternion(dest, dest);
        return;
    }
    v3 = sqrt(v2 + 1.0);
    dest->q4 = (float)(0.5 * v3);
    v4 = 0.5 / v3;
    dest->q1 = (float)((src->v2.z - src->v3.y) * v4);
    dest->q2 = (float)((src->v3.x - src->v1.z) * v4);
    dest->q3 = (float)((src->v1.y - src->v2.x) * v4);

    MATH_InvNormalQuaternion(dest, dest);
}

void MATH_MatrixBlend(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2, float factor, BOOL dontUseStoredValues)
{
    Quat4 tmpQ;
    float a5a;

    if (dontUseStoredValues)
    {
        MATH_ConvertMatrixToQuaternion(&Q0_137, src1);
        MATH_ConvertMatrixToQuaternion(&Q1_138, src2);
        a5a = (float)(Q1_138.q4 * Q0_137.q4 + Q1_138.q3 * Q0_137.q3 + Q1_138.q2 * Q0_137.q2 + Q1_138.q1 * Q0_137.q1);
        if (a5a < 0.0)
        {
            a5a = -a5a;
            MATH_InvertQuaternion(&Q1_138);
        }
        if (fabs(a5a) >= 0.99999988)
        {
            fTheta_139 = 0.0;
            fInvSinTheta_140 = 0.0;
        }
        else
        {
            fTheta_139 = (float)acos(a5a);
            fInvSinTheta_140 = (float)(1.0 / sqrt(1.0 - a5a * a5a));
        }
    }
    dest->mtxflag |= 4u;
    MATH_Slerp(&tmpQ, factor, &Q0_137, &Q1_138, fTheta_139, fInvSinTheta_140);
    MATH_ConvertQuaternionToMatrix(dest, &tmpQ);
}

void MATH_QuaternionBlend(Mtx4x4* dest, Quat4* src1, Quat4* src2, float factor, BOOL dontUseDefaultValues)
{
    float invSinTheta;
    float theta;
    Quat4 tmpQ;
    float v12;

    theta = 1.5700001f;
    invSinTheta = 1.0f;
    if (dontUseDefaultValues)
    {
        v12 = src1->q3 * src2->q3 + src1->q2 * src2->q2 + src1->q4 * src2->q4 + src1->q1 * src2->q1;
        if (v12 <= 1.0)
        {
            if (v12 < 0.0)
            {
                v12 = -v12;
                MATH_InvertQuaternion(src2);
            }
        }
        else
        {
            v12 = 1.0;
        }
        if (fabs(v12 - 1.0f) <= 0.0000001f)
        {
            theta = 0.0;
            invSinTheta = 0.0;
        }
        else
        {
            theta = (float)acos(v12);
            invSinTheta = (float)(1.0 / sqrt(1.0 - v12 * v12));
        }
    }
    dest->mtxflag |= 4u;
    MATH_Slerp(&tmpQ, factor, src1, src2, theta, invSinTheta);
    MATH_ConvertQuaternionToMatrix(dest, &tmpQ);
}

int MATH_l_CalculateCorrectType(Mtx4x4* mtx)
{
    int flagBackup;
    int ret;
    Vec3 scale;
    Mtx4x4 dest;

    flagBackup = mtx->mtxflag;
    mtx->mtxflag = 8;
    MATH_GetScale(&scale, mtx);
    if (MATH_b_EqVectorWithEpsilon(&scale, &MATH_gst_IdentityVector, 0.000099999997f))
        mtx->mtxflag &= ~8u;
    if (MATH_b_EqVectorWithEpsilon(&scale, &MATH_gst_NulVector, 0.000099999997f))
        mtx->mtxflag &= ~8u;
    MATH_GetRotationMatrix(&dest, mtx);
    if (MATH_b_EqVectorWithEpsilon(&dest.v1, &MATH_gst_IdentityMatrix.v1, 0.000099999997f)
        && MATH_b_EqVectorWithEpsilon(&dest.v2, &MATH_gst_IdentityMatrix.v2, 0.000099999997f)
        && MATH_b_EqVectorWithEpsilon(&dest.v3, &MATH_gst_IdentityMatrix.v3, 0.000099999997f))
    {
        mtx->v1.y = 0.0;
        mtx->v1.z = 0.0;
        mtx->v2.x = 0.0;
        mtx->v2.z = 0.0;
        mtx->v3.x = 0.0;
        mtx->v3.y = 0.0;
        mtx->v1.x = 1.0;
        mtx->v2.y = 1.0;
        mtx->v3.z = 1.0;
        mtx->mtxflag = mtx->mtxflag & ~4u;
    }
    else
    {
        MATH_Orthonormalize(mtx);
        mtx->mtxflag = mtx->mtxflag | 4;
    }
    if (!MATH_b_EqVectorWithEpsilon(&mtx->Pos, &MATH_gst_NulVector, 9.9999997e-10f))
        mtx->mtxflag |= 2u;
    ret = mtx->mtxflag;
    mtx->mtxflag = flagBackup;
    return ret;
}

BOOL MATH_b_TypeIsCorrect(Mtx4x4* mtx)
{
    return MATH_l_CalculateCorrectType(mtx) == mtx->mtxflag;
}

void MATH_SetCorrectType(Mtx4x4* mtx)
{
    mtx->mtxflag = MATH_l_CalculateCorrectType(mtx);
    if ((mtx->mtxflag & 8) == 0)
    {
        mtx->_m14 = 0.0;
        mtx->_m24 = 0.0;
        mtx->_m34 = 0.0;
    }
}

void MATH_RoundVectorWithPrecision(Vec3* vec, float precision)
{
    float v2;
    float v3;
    float v4;
    double v5;
    double v6;
    double v7;
    float v8;

    v8 = (float)(1.0 / precision);
    v7 = v8 * vec->x + MATH_gd_magicDoubleForRound;
    v6 = v8 * vec->y + MATH_gd_magicDoubleForRound;
    v5 = v8 * vec->z + MATH_gd_magicDoubleForRound;
    v4 = (float)*(int32_t *)&v7;
    vec->x = v4 * precision;
    v3 = (float)*(int32_t*)&v6;
    vec->y = v3 * precision;
    v2 = (float)*(int32_t*)&v5;
    vec->z = v2 * precision;
}

void MATH_SubstractVec(Vec3* dest, Vec3* v1, Vec3* v2)
{
    dest->x = v1->x - v2->x;
    dest->y = v1->y - v2->y;
    dest->z = v1->z - v2->z;
}

float MATH_VecLen(Vec3* v)
{
    return (float)sqrt(MATH_VecLenSquared(v));
}

float MATH_f_PointLineDistance(Vec3* v1, Vec3* v2, Vec3* v3)
{
    Vec3 src1;
    Vec3 src2;
    Vec3 dest;

    MATH_SubstractVec(&src1, v3, v2);
    MATH_SubstractVec(&src2, v1, v2);

    MATH_NormalizeEqualVector(&src1);
    MATH_CrossProduct(&dest, &src1, &src2);
    return MATH_VecLen(&dest);
}

float MATH_f_VecCosPlusArcCos(Vec3* a1, Vec3* a2)
{
    float vecCos;

    vecCos = MATH_f_VecCos(a1, a2);
    if (vecCos >= 1.0f)
        return 0.0f;
    if (vecCos <= -1.0f)
        return 3.1415927f;
    return (float)acos(vecCos);
}

void MATH_calc_curr_quat_from_dt_800(Quat4* a1, CompressedQuat* a2)
{
    float v2;
    float v3;
    float v4;

    v4 = (float)a2->s1;
    a1->q1 = (float)(v4 / 1024.0);
    v3 = (float)a2->s2;
    a1->q2 = (float)(v3 / 1024.0);
    v2 = (float)a2->s3;
    a1->q3 = (float)(v2 / 1024.0);
}

void MATH_calc_curr_quat_from_dt_1000(Quat4* a1, CompressedQuat* a2)
{
    float v2;
    float v3;
    float v4;

    v4 = (float)a2->s1;
    a1->q1 = (float)(v4 / 1024.0 + a1->q1);
    v3 = (float)a2->s2;
    a1->q2 = (float)(v3 / 1024.0 + a1->q2);
    v2 = (float)a2->s3;
    a1->q3 = (float)(v2 / 1024.0 + a1->q3);
}

void MATH_calc_curr_quat_from_dt_2000(Quat4* a1, UltraCompressedQuat* a2)
{
    float v2;
    float v3;
    float v4;

    v4 = (float)a2->s1;
    a1->q1 = (float)(v4 / 1024.0 + a1->q1);
    v3 = (float)a2->b2;
    a1->q2 = (float)(v3 / 1024.0 + a1->q2);
    v2 = (float)a2->b3;
    a1->q3 = (float)(v2 / 1024.0 + a1->q3);
}

void MATH_calc_curr_quat_from_dt_4000(Quat4* a1, UltraCompressedQuat* a2)
{
    float v2;
    float v3;
    float v4;

    v4 = (float)a2->b2;
    a1->q1 = (float)(v4 / 1024.0 + a1->q1);
    v3 = (float)a2->s1;
    a1->q2 = (float)(v3 / 1024.0 + a1->q2);
    v2 = (float)a2->b3;
    a1->q3 = (float)(v2 / 1024.0 + a1->q3);
}

void MATH_calc_curr_quat_from_dt_8000(Quat4* a1, UltraCompressedQuat* a2)
{
    float v2;
    float v3;
    float v4;

    v4 = (float)a2->b2;
    a1->q1 = (float)(v4 / 1024.0 + a1->q1);
    v3 = (float)a2->b3;
    a1->q2 = (float)(v3 / 1024.0 + a1->q2);
    v2 = (float)a2->s1;
    a1->q3 = (float)(v2 / 1024.0 + a1->q3);
}

void MATH_CompressQuaternion(CompressedQuat* compressedQuat, Quat4* quat)
{
    Quat4 tmpQuat;

    tmpQuat = *quat;
    if (quat->q4 < 0.0f)
        MATH_InvertQuaternion(&tmpQuat);
    compressedQuat->s1 = (uint16_t)(tmpQuat.q1 * 32767.0f + 0.5f);
    compressedQuat->s2 = (uint16_t)(tmpQuat.q2 * 32767.0f + 0.5f);
    compressedQuat->s3 = (uint16_t)(tmpQuat.q3 * 32767.0f + 0.5f);
}

void MATH_DecompressQuaternion(Quat4* quat, CompressedQuat* compressedQuat)
{
    float sum;

    quat->q1 = compressedQuat->s1 / 32767.0f;
    quat->q2 = compressedQuat->s2 / 32767.0f;
    quat->q3 = compressedQuat->s3 / 32767.0f;
    sum = quat->q1 * quat->q1 + quat->q2 * quat->q2 + quat->q3 * quat->q3;
    if (sum >= 1.0f)
    {
        quat->q4 = 0.0f;
    }
    else
    {
        quat->q4 = (float)sqrt(1.0f - sum);
    }
}

void MATH_VecSet(Vec3* dest, Vec3* src)
{
    *dest = *src;
}

void MATH_VecAddVec(Vec3* dest, Vec3* src1, Vec3* src2)
{
    dest->x = src1->x + src2->x;
    dest->y = src1->y + src2->y;
    dest->z = src1->z + src2->z;
}

void MATH_VecAddVecWithoutBuffer(Vec3* a1, Vec3* a2)
{
    a1->x = a1->x + a2->x;
    a1->y = a2->y + a1->y;
    a1->z = a2->z + a1->z;
}

void MATH_VecMultScalar(Vec3* dest, Vec3* src, float scalar)
{
    dest->x = src->x * scalar;
    dest->y = src->y * scalar;
    dest->z = src->z * scalar;
}

void MATH_VecMultScalarWithoutBuffer(Vec3* vec, float scalar)
{
    vec->x = vec->x * scalar;
    vec->y = vec->y * scalar;
    vec->z = vec->z * scalar;
}

float MATH_VecLenSquared(Vec3* vec)
{
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

void MATH_VecInvert(Vec3* dest, Vec3* src)
{
   dest->x = -src->x;
   dest->y = -src->y;
   dest->z = -src->z;
}

void MATH_VecMultNormScalar(Vec3* dest, Vec3* src, float scalarToBeNormalized)
{
    float normScalar;

    normScalar = (1.0f / scalarToBeNormalized);
    dest->x = src->x * normScalar;
    dest->y = src->y * normScalar;
    dest->z = src->z * normScalar;
}

// ilyen nincs a játékban

BOOL MATH_b_NulVectorWithEpsilon(Vec3* v, float epsilon) {

    if (fabs(v->x) <= epsilon)
    {
        if (fabs(v->y) <= epsilon)
            return fabs(v->z) <= epsilon;
        else
            return FALSE;
    }
    else
    {
        return FALSE;
    }
}

float MATH_LinearInterpolateFloats(float f1, float f2, float factor)
{
    float tmpFactor = factor;

    if (factor >= 0.0f)
    {
        if (tmpFactor > 1.0f)
            tmpFactor = 1.0f;
    }
    else
    {
        tmpFactor = 0.0f;
    }
    return (1.0f - tmpFactor) * f1 + f2 * tmpFactor;
}

Mtx4x4* MATH_Matrix_AllocArray(int num)
{
    return (Mtx4x4*)MEM_p_AllocAligned(sizeof(Mtx4x4) * num, 16);
}

Mtx4x4* MATH_Matrix_ReallocArray(Mtx4x4* oldMtxPtr, int num)
{
    return MEM_p_ReallocAligned(oldMtxPtr, sizeof(Mtx4x4) * num, 16);
}

void MATH_Matrix_Free(Mtx4x4* mtxPtr)
{
    MEM_FreeAlign(mtxPtr);
}

Mtx4x4* MATH_Matrix_Alloc()
{
    return (Mtx4x4*)MEM_p_AllocAligned(sizeof(Mtx4x4), 16);
}

float float_get_max_of_3vals_sub_48BBB0(float a1, float a2, float a3)
{
    double result; 

    result = a1;
    if (a2 > (double)a1)
        result = a2;
    if (a3 > result)
        result = a3;
    return (float)result;
}

float float_get_min_of_3vals_sub_48BB80(float a1, float a2, float a3)
{
    double result; // st7

    result = a1;
    if (a2 < (double)a1)
        result = a2;
    if (a3 < result)
        result = a3;
    return (float)result;
}

void MATH_VecRotate(Vec3* a1, Vec3* a2, Vec3* a3, float a4)
{
    float v4;
    float v5;
    Vec3 v6;
    float v7;
    Vec3* v8;
    Vec3* v9;
    Vec3 a3a;
    Vec3 dest;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;

    v8 = &a3a;
    v9 = a3;
    a3a = *a3;
    v12 = a3a.z * a2->z + a3a.y * a2->y + a3a.x * a2->x;
    v13 = a2->z * a2->z + a2->y * a2->y + a2->x * a2->x;
    v12 = v12 / v13;
    v14 = v12 * a2->x;
    v15 = a2->y * v12;
    v16 = a2->z * v12;
    a3a.x = a3a.x - v14;
    a3a.y = a3a.y - v15;
    a3a.z = a3a.z - v16;
    v5 = (float)cos(a4);
    v7 = v5;
    a1->x = a3a.x * v5;
    a1->y = a3a.y * v7;
    a1->z = a3a.z * v7;
    MATH_NormalizeEqualVector(a2);
    MATH_CrossProduct(&dest, a2, &a3a);
    v4 = (float)sin(a4);
    v6.x = dest.x * v4;
    v6.y = dest.y * v4;
    v6.z = dest.z * v4;
    MATH_VecAddVec(a1, a1, &v6);
    a1->x = v14 + a1->x;
    a1->y = v15 + a1->y;
    a1->z = v16 + a1->z;
}

float MATH_GetCos(float f)
{
    int v1;
    double v3;

    v3 = MATH_gf_1024by2Pi * f + MATH_gd_magicDoubleForRound;
    v1 = (*((int32_t *)(&v3))) + 256;
    if ((*((int32_t*)(&v3))) + 256 < 0) {
        v1 |= ~0x3FF; // make sure it's not greather than -1023 (0x3ff) (negative numbers are like 0xffffffffe formats two's complement)
        return MATH_gf_TableSin[1024 + v1 - 1]; // TODO nem biztos ohgy kell a -1
    }
    if (v1 > 1023)
        v1 &= 0x3FFu;
    return MATH_gf_TableSin[v1 + 1];
}

float MATH_GetSin(float f)
{
    int v1;
    double v3;

    v3 = MATH_gf_1024by2Pi * f + MATH_gd_magicDoubleForRound;
    v1 = (*((uint32_t*)(&v3)));
    if ((*((int32_t*)(&v3))) < 0) {
        v1 |= ~0x3FF; // make sure it's not greather than -1023 (0x3ff) (negative numbers are like 0xffffffffe formats two's complement)
        return MATH_gf_TableSin[1024 + v1 - 1]; // TODO nem biztos ohgy kell a -1
    }
    if ((*((int32_t*)(&v3))) > 1023)
        v1 = (*((uint32_t*)(&v3))) & 0x3FF;
    return MATH_gf_TableSin[v1 + 1];
}

float MATH_GetSqrt(float f)
{
    int ret = MATH_gl_TableRacine[ ((*((int32_t*)&f)) >> 13) & 0x7FF] + (((((*((uint32_t*)&f)) & 0x7F800000) + 0x3F800000) >> 1) & 0x7F800000);
    return *((float *)&ret);
}

float MATH_GetInverseSqrt(float f)
{
    int ret =  MATH_gl_TableInverseRacine[((*((int32_t*)&f)) >> 13) & 0x7FF]+ (((int)(0xBD800000 - ((*((int32_t*)&f)) & 0x7F800000)) >> 1) & 0x7F800000);
        
    return *((float*)&ret);
}

float MATH_GetInverse(float f)
{
    int ret = MATH_gl_TableInverse[((*((int32_t*)(&f))) >> 13) & 0x3FF] - ((*((uint32_t*)&f)) & 0xFF800000) + 0x7E800000;

    return *((float*)&ret);
}

float MATH_RandFloat() {
    int r = rand();
    return r * (1.0f / SHRT_MAX);
}

float MATH_RandFloatRange(float min, float max) {
    return MATH_RandFloat() * (max - min) + min;
}

float MATH_DotProduct(Vec3* v1, Vec3* v2)
{
    return v1->z * v2->z + v1->y * v2->y + v1->x * v2->x;
}

