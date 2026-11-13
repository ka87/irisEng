#include "common.h"
#include "memory.h"
#include "math.h"

void MATH_CreateTrigTables()
{
    int i;

    for (i = 0; i < 1024; i++) {
        MATH_gf_TableSin[i] = (float)sin(i * (MATH_f_2Pi / 1024.0f));
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

float MATH_f_VecSin(Vec3* vec1, Vec3* vec2, Vec3* axis)
{    
    Vec3 tmpv1, tmpv2, cp;
    float len;

    MATH_NormalizeVector(&tmpv1, vec1);
    MATH_NormalizeVector(&tmpv2, vec2);
    MATH_CrossProduct(&cp, &tmpv1, &tmpv2);
    
    len = MATH_VecLen(&cp);

    if (axis && cp.x * axis->x + axis->y * cp.y + axis->z * cp.z < 0.0f)
        return -len;
    else
        return len;
}

float MATH_f_VecCos(Vec3* vec1, Vec3* vec2)
{
    Vec3 tmpv1, tmpv2;

    MATH_NormalizeVector(&tmpv1, vec1);
    MATH_NormalizeVector(&tmpv2, vec2);

    return tmpv1.x * tmpv2.x + tmpv1.y * tmpv2.y + tmpv1.z * tmpv2.z;
}

float MATH_f_VecAngle(Vec3* vec1, Vec3* vec2, Vec3* axis)
{
    float vc, vs, ret;

    vc = MATH_f_VecCos(vec1, vec2);

    if (vc >= 1.0f)
        return 0.0f;

    if (vc > -1.0f)
        ret = (float)acos(vc);
    else
        ret = MATH_f_Pi;

    vs = MATH_f_VecSin(vec1, vec2, axis);

   if (vs < 0.0f)
       ret = -ret;

    return ret;
}

float MATH_f_ZAngle(Vec3* vec1, Vec3* vec2)
{
    float ret;

    Vec3 tmpVec1 = { vec1->x, vec1->y, 0.0f };
    Vec3 tmpVec2 = { vec2->x, vec2->y, 0.0f };

    MATH_NormalizeEqualVector(&tmpVec1);
    MATH_NormalizeEqualVector(&tmpVec2);

    ret = MATH_f_VecAngle(&tmpVec1, &tmpVec2, &MATH_gst_BaseVectorK);
    if (ret > MATH_f_Pi)
        ret -= MATH_f_2Pi;
    if (ret < -MATH_f_Pi)
        ret += MATH_f_2Pi;
    return (float)ret;
}

float MATH_f_XAngle(Vec3* vec1, Vec3* vec2)
{
    float v1AngleAbs, v2AngleAbs, ret;
    Vec3 tmpVec1 = { vec1->x, vec1->y, 0.0f };
    Vec3 tmpVec2 = { vec2->x, vec2->y, 0.0f };

    MATH_NormalizeEqualVector(&tmpVec1);
    MATH_NormalizeEqualVector(&tmpVec2);

    v1AngleAbs = (float)fabs(MATH_f_VecAngle(&tmpVec1, vec1, &MATH_gst_BaseVectorI));
    
    if (vec1->z < 0.0f)
        v1AngleAbs = -v1AngleAbs;

    v2AngleAbs = (float)fabs(MATH_f_VecAngle(&tmpVec2, vec2, &MATH_gst_BaseVectorI));
    
    if (vec2->z < 0.0f)
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

float MATH_DistanceOfTwoPoints(Vec3* p1, Vec3* p2)
{
    double xDif = p1->x - p2->x;
    double yDif = p1->y - p2->y;
    double zDif = p1->z - p2->z;

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

void MATH_BlendVector(Vec3* dest, Vec3* from, Vec3* to, float amount)
{
    if (amount < 0.0f)
        amount = 0.0f;
    if (amount > 1.0f)
        amount = 1.0f;
    dest->x = (1.0f - amount) * from->x + amount * to->x;
    dest->y = (1.0f - amount) * from->y + amount * to->y;
    dest->z = (1.0f - amount) * from->z + amount * to->z;
}

BOOL MATH_b_VecColinear(Vec3* vec1, Vec3* vec2)
{
    Vec3 cp;
    
    MATH_CrossProduct(&cp, vec1, vec2);
    
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

double MATH_PowLimitedCE(float x)
{
    if (x > 0.0)
        return pow(x, 0.3333333432674408);
    if (x >= 0.0)
        return 0.0;
    return -pow(-x, 0.3333333432674408);
}

int MATH_i_SolveCubicEquation(float a, float b, float c, Vec3* dest)
{
    double aSq;
    long double disc;
    double sqrtDisc;
    double w;
    double root;
    double phi;
    double amp;
    double amp2;
    double cbrtRoot;
    double shift;
    float pThird;
    float pThirdCubed;
    float u;
    float qHalf;
    int ret;

    aSq = a * a;
    pThird = (float)((b - 0.33333334 * aSq) * 0.33333334);
    qHalf = (float)((aSq * a * 0.074074075 - a * b * 0.33333334 + c) * 0.5);
    pThirdCubed = pThird * pThird * pThird;
    disc = qHalf * qHalf + pThirdCubed;

    if (fabs(disc) >= 0.0000099999997)
    {
        if (disc >= 0.0)
        {
            sqrtDisc = sqrt(disc);
            u = (float)MATH_PowLimitedCE((float)(sqrtDisc - qHalf));
            w = (float)MATH_PowLimitedCE((float)(sqrtDisc + qHalf));
            root = u - w;
            dest->x = (float)root;
            dest->y = (float)root;
            dest->z = (float)root;
            ret = 1;
        }
        else
        {
            phi = acos(-(qHalf / sqrt(-pThirdCubed))) * 0.33333334;
            amp = sqrt(-pThird);
            amp2 = amp + amp;
            dest->x = (float)(cos(phi) * amp2);
            dest->y = (float)(-(cos(phi + 1.0471976) * amp2));
            dest->z = (float)(-(cos(phi - 1.0471976) * amp2));
            ret = 3;
        }
    }
    else
    {
        if (fabs(qHalf) < 0.0000099999997)
        {
            dest->x = 0.0;
            dest->y = 0.0;
            dest->z = 0.0;
            ret = 1;
        }
        else
        {
            cbrtRoot = MATH_PowLimitedCE(-qHalf);
            dest->x = (float)(cbrtRoot + cbrtRoot);
            dest->y = (float)(-cbrtRoot);
            dest->z = (float)(-cbrtRoot);
            ret = 2;
        }
    }

    shift = a * 0.33333334;
    dest->x = (float)(dest->x - shift);
    dest->y = (float)(dest->y - shift);
    dest->z = (float)(dest->z - shift);
    return ret;
}

void MATH_PointParabolProjection2D(Vec3* dest, Vec3* point, float curvature, float height)
{
    double invCurv;
    int numRoots;
    double chosen;
    double dyX;
    double dxX;
    double distX;
    double dyY;
    double dxY;
    double dxZ;
    double dyZ;
    float coefB;
    float coefC;
    float distY;
    float distZ;
    Vec3 roots;

    if (fabs(curvature) >= 0.00000000099999997)
    {
        invCurv = 1.0 / curvature;
        coefC = (float)(-(0.5 * invCurv * point->x * invCurv));
        coefB = (float)((0.5 * invCurv + height - point->y) * invCurv);
        numRoots = MATH_i_SolveCubicEquation(0.0, coefB, coefC, &roots);
        chosen = roots.x;
        if (numRoots > 1)
        {
            dyX = chosen * roots.x * curvature + height - point->y;
            dxX = roots.x - point->x;
            distX = dyX * dyX + dxX * dxX;
            dyY = roots.y * roots.y * curvature + height - point->y;
            dxY = roots.y - point->x;
            distY = (float)(dyY * dyY + dxY * dxY);
            dxZ = roots.z - point->x;
            dyZ = roots.z * roots.z * curvature + height - point->y;
            distZ = (float)(dyZ * dyZ + dxZ * dxZ);
            if (distX >= distY)
            {
                if (distY < (double)distZ)
                {
                    // takes its x from roots.y and its height from roots.x
                    dest->x = roots.y;
                    dest->z = 0.0;
                    dest->y = (float)(chosen * chosen * curvature + height);
                    return;
                }
            }
            else if (distX < distZ)
            {
                dest->x = roots.x;
                dest->z = 0.0;
                dest->y = (float)(chosen * chosen * curvature + height);
                return;
            }
            chosen = roots.z;
        }
        dest->x = (float)chosen;
        dest->z = 0.0;
        dest->y = (float)(chosen * chosen * curvature + height);
        return;
    }
    dest->y = height;
    dest->x = point->x;
    dest->z = 0.0;
}

void MATH_PointParabolProjection3D(Vec3* dest, Vec3* tangent, Vec3* point, Vec3* refA, Vec3* refB, Vec3* origin, float time)
{
    double dxA;
    double dyA;
    double invLenA;
    double ayNorm;
    double azNorm;
    double invLenB;
    double axisX;
    double axisY;
    double invBisLen;
    float height;
    float invAxisLen;
    float curvature;
    Vec3 src;
    Vec3 ptA;
    Mtx4x4 frame;
    Vec3 localA;
    Vec3 localTangent;
    Vec3 localProj;
    Vec3 localPoint;
    Vec3 localB;

    dxA = refA->x - origin->x;
    dyA = refA->y - origin->y;
    ptA.z = refA->z - origin->z;
    src.x = refB->x - origin->x;
    src.y = refB->y - origin->y;
    src.z = refB->z - origin->z;
    invLenA = 1.0 / sqrt(dyA * dyA + dxA * dxA + ptA.z * ptA.z);
    ptA.x = (float)(invLenA * dxA);
    ayNorm = dyA * invLenA;
    azNorm = invLenA * ptA.z;
    invLenB = 1.0 / sqrt(src.z * src.z + src.y * src.y + src.x * src.x);
    src.x = (float)(src.x * invLenB);
    src.y = (float)(src.y * invLenB);
    src.z = (float)(src.z * invLenB);
    axisX = src.x - ptA.x;
    axisY = src.y - ayNorm;
    frame.v1.z = (float)(src.z - azNorm);
    frame.v2.x = src.x + ptA.x;
    frame.v2.y = (float)(src.y + ayNorm);
    frame.v2.z = (float)(src.z + azNorm);
    invAxisLen = (float)(1.0 / sqrt(frame.v1.z * frame.v1.z + axisY * axisY + axisX * axisX));
    frame.v1.x = (float)(axisX * invAxisLen);
    frame.v1.y = (float)(axisY * invAxisLen);
    frame.v1.z = frame.v1.z * invAxisLen;
    invBisLen = 1.0 / sqrt(frame.v2.z * frame.v2.z + frame.v2.y * frame.v2.y + frame.v2.x * frame.v2.x);
    frame.v2.x = (float)(frame.v2.x * invBisLen);
    frame.v2.y = (float)(frame.v2.y * invBisLen);
    frame.v2.z = (float)(frame.v2.z * invBisLen);
    frame.v3.x = frame.v2.z * frame.v1.y - frame.v2.y * frame.v1.z;
    frame.Pos.x = origin->x;
    frame.Pos.y = origin->y;
    frame.Pos.z = origin->z;
    frame.v3.y = frame.v2.x * frame.v1.z - frame.v2.z * frame.v1.x;
    frame._m14 = 0.0;
    frame._m24 = 0.0;
    frame._m34 = 0.0;
    frame.mtxflag = (frame.mtxflag | 2) & ~8u;
    frame.v3.z = frame.v2.y * frame.v1.x - frame.v2.x * frame.v1.y;
    ptA.x = ptA.x * time;
    ptA.y = (float)(ayNorm * time);
    src.y = src.y * time;
    src.z = src.z * time;
    ptA.x = ptA.x + origin->x;
    ptA.y = ptA.y + origin->y;
    ptA.z = (float)(azNorm * time + origin->z);
    src.x = src.x * time + origin->x;
    src.y = src.y + origin->y;
    src.z = src.z + origin->z;
    MATH_VertexGlobalToLocal(&localA, &frame, &ptA);
    // localB is computed and never read; the parabola is built from localA alone
    MATH_VertexGlobalToLocal(&localB, &frame, &src);
    MATH_VertexGlobalToLocal(&localPoint, &frame, point);
    curvature = localA.y / (localA.x * localA.x + localA.x * localA.x);
    height = localA.y * 0.5f;
    MATH_PointParabolProjection2D(&localProj, &localPoint, curvature, height);
    MATH_TransformVertex(dest, &frame, &localProj);
    localTangent.y = localProj.x * curvature + localProj.x * curvature;
    localTangent.x = 1.0;
    localTangent.z = 0.0;
    MATH_TransformVectorNoScale(tangent, &frame, &localTangent);
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

void MATH_MakeRotationMatrix_AxisAngle(Mtx4x4* mtx, Vec3* axis, float angle, Quat4* quatOut, char resetTranslation)
{
    Quat4* q;
    float half;
    float s;
    float c;
    float x2;
    float xx;
    float xy;
    float xz;
    float zz;
    unsigned int flags;
    unsigned int flagsNoScale;
    float wy;
    float wx;
    float yy;
    Quat4 scratch;
    float wz;
    float z2;
    float y2;
    float yz;

    q = quatOut;
    half = angle * 0.5f;
    s = (float)sin(half);
    if (!quatOut)
        q = &scratch;
    q->q1 = s * axis->x;
    q->q2 = axis->y * s;
    q->q3 = axis->z * s;
    c = (float)cos(half);
    q->q4 = c;
    x2 = q->q1 + q->q1;
    y2 = q->q2 + q->q2;
    z2 = q->q3 + q->q3;
    wx = c * x2;
    wy = c * y2;
    wz = c * z2;
    xx = x2 * q->q1;
    xy = y2 * q->q1;
    xz = z2 * q->q1;
    yy = y2 * q->q2;
    yz = z2 * q->q2;
    zz = z2 * q->q3;
    mtx->v1.x = 1.0f - (yy + zz);
    mtx->v1.y = xy + wz;
    mtx->v1.z = xz - wy;
    mtx->v2.x = xy - wz;
    flags = mtx->mtxflag;
    mtx->v2.y = 1.0f - (zz + xx);
    mtx->v2.z = yz + wx;
    mtx->v3.x = xz + wy;
    mtx->v3.y = yz - wx;
    mtx->v3.z = 1.0f - (yy + xx);
    if (resetTranslation)
    {
        mtx->_m14 = 0.0;
        mtx->_m24 = 0.0;
        mtx->_m34 = 0.0;
        mtx->mtxflag = flags & ~8u;
        mtx->Pos.x = 0.0;
        mtx->Pos.y = 0.0;
        mtx->Pos.z = 0.0;
        flagsNoScale = mtx->mtxflag & ~2u;
        mtx->mtxflag = flagsNoScale;
        flags = flagsNoScale;
    }
    mtx->mtxflag = flags | 4;
}

void MATH_RotateMatrix_AroundGlobalXAxis(Mtx4x4* mtx, float angle)
{
    double cosA;
    double sinA;
    double u1;
    double w1;
    double u2;
    double w2;
    double u3;
    double w3;

    cosA = cos(angle);
    mtx->mtxflag |= 4u;
    sinA = sin(angle);
    u1 = mtx->v1.y;
    w1 = mtx->v1.z;
    mtx->v1.y = (float)(u1 * cosA - w1 * sinA);
    mtx->v1.z = (float)(u1 * sinA + w1 * cosA);
    u2 = mtx->v2.y;
    w2 = mtx->v2.z;
    mtx->v2.y = (float)(u2 * cosA - w2 * sinA);
    mtx->v2.z = (float)(u2 * sinA + w2 * cosA);
    u3 = mtx->v3.y;
    w3 = mtx->v3.z;
    mtx->v3.y = (float)(u3 * cosA - w3 * sinA);
    mtx->v3.z = (float)(u3 * sinA + w3 * cosA);
}

void MATH_RotateMatrix_AroundGlobalYAxis(Mtx4x4* mtx, float angle)
{
    double cosA;
    double sinA;
    double u1;
    double w1;
    double u2;
    double w2;
    double u3;
    double w3;

    cosA = cos(angle);
    mtx->mtxflag |= 4u;
    sinA = sin(angle);
    u1 = mtx->v1.x;
    w1 = mtx->v1.z;
    mtx->v1.x = (float)(w1 * sinA + u1 * cosA);
    mtx->v1.z = (float)(w1 * cosA - u1 * sinA);
    u2 = mtx->v2.x;
    w2 = mtx->v2.z;
    mtx->v2.x = (float)(w2 * sinA + u2 * cosA);
    mtx->v2.z = (float)(w2 * cosA - u2 * sinA);
    u3 = mtx->v3.x;
    w3 = mtx->v3.z;
    mtx->v3.x = (float)(w3 * sinA + u3 * cosA);
    mtx->v3.z = (float)(w3 * cosA - u3 * sinA);
}

void MATH_RotateMatrix_AroundGlobalZAxis(Mtx4x4* mtx, float angle)
{
    double cosA;
    double sinA;
    double u1;
    double w1;
    double u2;
    double w2;
    double u3;
    double w3;

    cosA = cos(angle);
    mtx->mtxflag |= 4u;
    sinA = sin(angle);
    u1 = mtx->v1.x;
    w1 = mtx->v1.y;
    mtx->v1.x = (float)(u1 * cosA - w1 * sinA);
    mtx->v1.y = (float)(u1 * sinA + w1 * cosA);
    u2 = mtx->v2.x;
    w2 = mtx->v2.y;
    mtx->v2.x = (float)(u2 * cosA - w2 * sinA);
    mtx->v2.y = (float)(u2 * sinA + w2 * cosA);
    u3 = mtx->v3.x;
    w3 = mtx->v3.y;
    mtx->v3.x = (float)(u3 * cosA - w3 * sinA);
    mtx->v3.y = (float)(u3 * sinA + w3 * cosA);
}

void MATH_RotateMatrix_AroundLocalXAxis(Mtx4x4* mtx, float angle)
{
    double cosA;
    double sinA;
    double u1;
    double w1;
    double u2;
    double w2;
    double u3;
    double w3;

    cosA = cos(angle);
    mtx->mtxflag |= 4u;
    sinA = sin(angle);
    u1 = mtx->v2.x;
    w1 = mtx->v3.x;
    mtx->v2.x = (float)(w1 * sinA + u1 * cosA);
    mtx->v3.x = (float)(w1 * cosA - u1 * sinA);
    u2 = mtx->v2.y;
    w2 = mtx->v3.y;
    mtx->v2.y = (float)(w2 * sinA + u2 * cosA);
    mtx->v3.y = (float)(w2 * cosA - u2 * sinA);
    u3 = mtx->v2.z;
    w3 = mtx->v3.z;
    mtx->v2.z = (float)(w3 * sinA + u3 * cosA);
    mtx->v3.z = (float)(w3 * cosA - u3 * sinA);
}

void MATH_RotateMatrix_AroundLocalYAxis(Mtx4x4* mtx, float angle)
{
    double cosA;
    double sinA;
    double u1;
    double w1;
    double u2;
    double w2;
    double u3;
    double w3;

    cosA = cos(angle);
    mtx->mtxflag |= 4u;
    sinA = sin(angle);
    u1 = mtx->v1.x;
    w1 = mtx->v3.x;
    mtx->v1.x = (float)(u1 * cosA - w1 * sinA);
    mtx->v3.x = (float)(u1 * sinA + w1 * cosA);
    u2 = mtx->v1.y;
    w2 = mtx->v3.y;
    mtx->v1.y = (float)(u2 * cosA - w2 * sinA);
    mtx->v3.y = (float)(u2 * sinA + w2 * cosA);
    u3 = mtx->v1.z;
    w3 = mtx->v3.z;
    mtx->v1.z = (float)(u3 * cosA - w3 * sinA);
    mtx->v3.z = (float)(u3 * sinA + w3 * cosA);
}

void MATH_RotateMatrix_AroundLocalZAxis(Mtx4x4* mtx, float angle)
{
    double cosA;
    double sinA;
    double u1;
    double w1;
    double u2;
    double w2;
    double u3;
    double w3;

    cosA = cos(angle);
    mtx->mtxflag |= 4u;
    sinA = sin(angle);
    u1 = mtx->v1.x;
    w1 = mtx->v2.x;
    mtx->v1.x = (float)(w1 * sinA + u1 * cosA);
    mtx->v2.x = (float)(w1 * cosA - u1 * sinA);
    u2 = mtx->v1.y;
    w2 = mtx->v2.y;
    mtx->v1.y = (float)(w2 * sinA + u2 * cosA);
    mtx->v2.y = (float)(w2 * cosA - u2 * sinA);
    u3 = mtx->v1.z;
    w3 = mtx->v2.z;
    mtx->v1.z = (float)(w3 * sinA + u3 * cosA);
    mtx->v2.z = (float)(w3 * cosA - u3 * sinA);
}

void MATH_MakeRotationMatrix_UsingSight(Mtx4x4* dest, Vec3* sight, Vec3* banking, char onlyRotation)
{
    double sightLen;
    double bankLen;
    double rightLen;
    Vec3 bank;

    sightLen = sqrt(sight->z * sight->z + sight->y * sight->y + sight->x * sight->x);
    dest->v2.x = (float)(1.0 / sightLen * sight->x);
    dest->v2.y = (float)(1.0 / sightLen * sight->y);
    dest->v2.z = (float)(1.0 / sightLen * sight->z);
    dest->v2.y = -dest->v2.y;
    dest->v2.z = -dest->v2.z;
    dest->v2.x = -dest->v2.x;

    if (banking)
    {
        bankLen = sqrt(banking->z * banking->z + banking->y * banking->y + banking->x * banking->x);
        bank.x = (float)(1.0 / bankLen * banking->x);
        bank.y = (float)(1.0 / bankLen * banking->y);
        bank.z = (float)(1.0 / bankLen * banking->z);
        dest->v1.x = bank.z * sight->y - bank.y * sight->z;
        dest->v1.y = bank.x * sight->z - bank.z * sight->x;
        dest->v1.z = bank.y * sight->x - bank.x * sight->y;
        if (fabs(dest->v1.x) <= 1e-09 && fabs(dest->v1.y) <= 1e-09 && fabs(dest->v1.z) <= 1e-09)
        {
            bank.x = (float)(bank.x + 0.000099999997);
            dest->v1.x = bank.z * sight->y - bank.y * sight->z;
            dest->v1.y = bank.x * sight->z - bank.z * sight->x;
            dest->v1.z = bank.y * sight->x - bank.x * sight->y;
        }
        dest->v3 = bank;
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
    rightLen = sqrt(dest->v1.x * dest->v1.x + dest->v1.y * dest->v1.y + dest->v1.z * dest->v1.z);
    dest->v1.x = (float)(1.0 / rightLen * dest->v1.x);
    dest->v1.y = (float)(1.0 / rightLen * dest->v1.y);
    dest->v1.z = (float)(1.0 / rightLen * dest->v1.z);
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

void MATH_MakeRotationMatrix_UsingBanking(Mtx4x4* mtx, Vec3* sight, Vec3* banking, char onlyRotation)
{
    double bankLen; // normalize equalvector croosproduct nullveectorwithepsilon inlineok
    double sightLen;
    double rightLen;

    bankLen = sqrt(banking->z * banking->z + banking->y * banking->y + banking->x * banking->x);
    mtx->v3.x = (float)(1.0 / bankLen * banking->x);
    mtx->v3.y = (float)(1.0 / bankLen * banking->y);
    mtx->v3.z = (float)(1.0 / bankLen * banking->z);
    if (sight)
    {
        mtx->v2.x = -sight->x;
        mtx->v2.y = -sight->y;
        mtx->v2.z = -sight->z;
        sightLen = sqrt(mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y + mtx->v2.z * mtx->v2.z);
        mtx->v2.x = (float)(1.0 / sightLen * mtx->v2.x);
        mtx->v2.y = (float)(1.0 / sightLen * mtx->v2.y);
        mtx->v2.z = (float)(1.0 / sightLen * mtx->v2.z);
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
    rightLen = sqrt(mtx->v1.x * mtx->v1.x + mtx->v1.y * mtx->v1.y + mtx->v1.z * mtx->v1.z);
    mtx->v1.x = (float)(1.0 / rightLen * mtx->v1.x);
    mtx->v1.y = (float)(1.0 / rightLen * mtx->v1.y);
    mtx->v1.z = (float)(1.0 / rightLen * mtx->v1.z);
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

void MATH_OrientMatrix_UsingSight(Mtx4x4* mtx, Vec3* sight, Vec3* banking)
{
    MATH_MakeRotationMatrix_UsingSight(mtx, sight, banking, 0);
}

void MATH_OrientMatrix_UsingBanking(Mtx4x4* mtx, Vec3* sight, Vec3* banking)
{
    MATH_MakeRotationMatrix_UsingBanking(mtx, sight, banking, 0);
}

void MATH_InvertQuaternion(Quat4* q)
{
    q->q1 = -q->q1;
    q->q2 = -q->q2;
    q->q3 = -q->q3;
    q->q4 = -q->q4;
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
    double cof00;
    double det1;
    double cof01;
    double det2;
    double cof02;
    double invDet;

    cof00 = src->v3.z * src->v2.y - src->v3.y * src->v2.z;
    dest->v1.x = (float)cof00;
    det1 = cof00 * src->v1.x;
    cof01 = src->v1.z * src->v3.y - src->v1.y * src->v3.z;
    dest->v1.y = (float)cof01;
    det2 = det1 + cof01 * src->v2.x;
    cof02 = src->v2.z * src->v1.y - src->v1.z * src->v2.y;
    dest->v1.z = (float)cof02;
    invDet = 1.0 / (det2 + cof02 * src->v3.x);
    dest->v2.x = (float)((src->v3.x * src->v2.z - src->v3.z * src->v2.x) * invDet);
    dest->v2.y = (float)((src->v1.x * src->v3.z - src->v1.z * src->v3.x) * invDet);
    dest->v2.z = (float)((src->v1.z * src->v2.x - src->v2.z * src->v1.x) * invDet);
    dest->v3.x = (float)((src->v3.y * src->v2.x - src->v3.x * src->v2.y) * invDet);
    dest->v3.y = (float)((src->v3.x * src->v1.y - src->v3.y * src->v1.x) * invDet);
    dest->v3.z = (float)((src->v1.x * src->v2.y - src->v1.y * src->v2.x) * invDet);
    // the first row scales the float already stored, not the double cofactor
    dest->v1.x = (float)(invDet * dest->v1.x);
    dest->v1.y = (float)(invDet * dest->v1.y);
    dest->v1.z = (float)(invDet * dest->v1.z);
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
    double len1;
    double len2; 
    double len3;

    len1 = sqrt(mtx->v1.x * mtx->v1.x + mtx->v1.y * mtx->v1.y + mtx->v1.z * mtx->v1.z);
    mtx->v1.x = (float)(1.0 / len1 * mtx->v1.x);
    mtx->v1.y = (float)(1.0 / len1 * mtx->v1.y);
    mtx->v1.z = (float)(1.0 / len1 * mtx->v1.z);
    mtx->v2.x = mtx->v1.z * mtx->v3.y - mtx->v1.y * mtx->v3.z;
    mtx->v2.y = mtx->v1.x * mtx->v3.z - mtx->v1.z * mtx->v3.x;
    mtx->v2.z = mtx->v3.x * mtx->v1.y - mtx->v3.y * mtx->v1.x;
    len2 = sqrt(mtx->v2.z * mtx->v2.z + mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y);
    mtx->v2.x = (float)(1.0 / len2 * mtx->v2.x);
    mtx->v2.y = (float)(1.0 / len2 * mtx->v2.y);
    mtx->v2.z = (float)(1.0 / len2 * mtx->v2.z);
    mtx->v3.x = mtx->v2.z * mtx->v1.y - mtx->v2.y * mtx->v1.z;
    mtx->v3.y = mtx->v1.z * mtx->v2.x - mtx->v2.z * mtx->v1.x;
    mtx->v3.z = mtx->v1.x * mtx->v2.y - mtx->v2.x * mtx->v1.y;
    len3 = sqrt(mtx->v3.z * mtx->v3.z + mtx->v3.x * mtx->v3.x + mtx->v3.y * mtx->v3.y);
    mtx->v3.x = (float)(1.0 / len3 * mtx->v3.x);
    mtx->v3.y = (float)(1.0 / len3 * mtx->v3.y);
    mtx->v3.z = (float)(1.0 / len3 * mtx->v3.z);
}

void MATH_Slerp(Quat4* dest, float factor, Quat4* src1, Quat4* src2, float theta, float invSinTheta)
{
    double wFrom;
    double wTo;
    float s1;
    float s2;
    float s3;
    float s4;

    if (theta == 0.0)
    {
        *dest = *src1;
    }
    else
    {
        wFrom = sin((1.0 - factor) * theta) * invSinTheta;
        wTo = sin((double)factor * theta) * invSinTheta;
        s1 = (float)(src1->q1 * wFrom);
        s2 = (float)(src1->q2 * wFrom);
        s3 = (float)(src1->q3 * wFrom);
        s4 = (float)(src1->q4 * wFrom);
        dest->q1 = (float)(src2->q1 * wTo);
        dest->q2 = (float)(src2->q2 * wTo);
        dest->q3 = (float)(src2->q3 * wTo);
        dest->q4 = (float)(src2->q4 * wTo);
        dest->q1 = s1 + dest->q1;
        dest->q2 = s2 + dest->q2;
        dest->q3 = s3 + dest->q3;
        dest->q4 = s4 + dest->q4;
        MATH_NormQuat(dest, dest);
    }
}

void MATH_ConvertQuaternionToMatrix(Mtx4x4* mtx, Quat4* q)
{
    double xx;
    double halfMinusXX;
    double xy;
    double xy2;
    double xz;
    double xz2;
    double wx;
    double yy;
    double yz;
    double wy;
    double wy2;
    double zz;
    double halfMinusZZ;
    double wz;
    double wz2;
    float wx2; 
    float halfMinusYY; 
    float yz2; 

    xx = q->q1 * q->q1;
    halfMinusXX = 0.5 - (xx + xx);
    xy = q->q2 * q->q1;
    xy2 = xy + xy;
    xz = q->q3 * q->q1;
    xz2 = xz + xz;
    wx = q->q4 * q->q1;
    wx2 = (float)(wx + wx);
    yy = q->q2 * q->q2;
    halfMinusYY = (float)(0.5 - (yy + yy));
    yz = q->q3 * q->q2;
    yz2 = (float)(yz + yz);
    wy = q->q4 * q->q2;
    wy2 = wy + wy;
    zz = q->q3 * q->q3;
    halfMinusZZ = 0.5 - (zz + zz);
    wz = q->q4 * q->q3;
    mtx->_m14 = 0.0;
    mtx->_m24 = 0.0;
    wz2 = wz + wz;
    mtx->_m34 = 0.0;
    mtx->mtxflag = mtx->mtxflag & ~8u | 4;
    mtx->v1.x = (float)(halfMinusZZ + halfMinusYY);
    mtx->v2.x = (float)(xy2 - wz2);
    mtx->v3.x = (float)(wy2 + xz2);
    mtx->v1.y = (float)(wz2 + xy2);
    mtx->v2.y = (float)(halfMinusZZ + halfMinusXX);
    mtx->v3.y = yz2 - wx2;
    mtx->v1.z = (float)(xz2 - wy2);
    mtx->v2.z = yz2 + wx2;
    mtx->v3.z = (float)(halfMinusYY + halfMinusXX);
}

void MATH_InvNormalQuaternion(Quat4* dest, Quat4* src) {

    double len = sqrt(src->q4 * src->q4 + src->q3 * src->q3 + src->q2 * src->q2 + src->q1 * src->q1);
    dest->q1 = (float)(1.0 / len * src->q1);
    dest->q2 = (float)(1.0 / len * src->q2);
    dest->q3 = (float)(1.0 / len * src->q3);
    dest->q4 = (float)(1.0 / len * src->q4);
}

void MATH_ConvertMatrixToQuaternion(Quat4 *dest, Mtx4x4 *src) {
    float trace;
    double rW;
    double scaleW;
    double rZ;
    double scale;
    double wNum;
    double rY;
    double rX;

    trace = src->v1.x + src->v3.z + src->v2.y;
    if (trace <= 0.0) {
        if (src->v1.x < src->v2.y) {
            if (src->v2.y >= src->v3.z) {
                rY = sqrt(1.0 - src->v1.x + src->v2.y - src->v3.z);
                dest->q2 = (float)(0.5 * rY);
                scale = 0.5 / rY;
                dest->q3 = (float)((src->v3.y + src->v2.z) * scale);
                dest->q1 = (float)((src->v2.x + src->v1.y) * scale);
                wNum = src->v3.x - src->v1.z;
            }
            else {
                rZ = sqrt(1.0 - src->v1.x - src->v2.y + src->v3.z); // same
                dest->q3 = (float)(0.5 * rZ);
                scale = 0.5 / rZ;
                dest->q1 = (float)((src->v3.x + src->v1.z) * scale);
                dest->q2 = (float)((src->v3.y + src->v2.z) * scale);
                wNum = src->v1.y - src->v2.x;
            }
        }
        else if (src->v1.x >= src->v3.z) {
            rX = sqrt(src->v1.x + 1.0 - src->v2.y - src->v3.z);
            dest->q1 = (float)(0.5 * rX);
            scale = 0.5 / rX;
            dest->q2 = (float)((src->v2.x + src->v1.y) * scale);
            dest->q3 = (float)((src->v3.x + src->v1.z) * scale);
            wNum = src->v2.z - src->v3.y;
        }
        else {
            rZ = sqrt(1.0 - src->v1.x - src->v2.y + src->v3.z); // same
            dest->q3 = (float)(0.5 * rZ);
            scale = 0.5 / rZ;
            dest->q1 = (float)((src->v3.x + src->v1.z) * scale);
            dest->q2 = (float)((src->v3.y + src->v2.z) * scale);
            wNum = src->v1.y - src->v2.x;
        }

        dest->q4 = (float)(wNum * scale);
        MATH_InvNormalQuaternion(dest, dest);
        return;
    }
    rW = sqrt(trace + 1.0);
    dest->q4 = (float)(0.5 * rW);
    scaleW = 0.5 / rW;
    dest->q1 = (float)((src->v2.z - src->v3.y) * scaleW);
    dest->q2 = (float)((src->v3.x - src->v1.z) * scaleW);
    dest->q3 = (float)((src->v1.y - src->v2.x) * scaleW);

    MATH_InvNormalQuaternion(dest, dest);
}

void MATH_MatrixBlend(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2, float factor, BOOL dontUseStoredValues)
{
    Quat4 tmpQ;
    float cosTheta;

    if (dontUseStoredValues)
    {
        MATH_ConvertMatrixToQuaternion(&MATH_gst_BlendQuatFrom, src1);
        MATH_ConvertMatrixToQuaternion(&MATH_gst_BlendQuatTo, src2);
        cosTheta = (float)(MATH_gst_BlendQuatTo.q4 * MATH_gst_BlendQuatFrom.q4 + MATH_gst_BlendQuatTo.q3 * MATH_gst_BlendQuatFrom.q3 + MATH_gst_BlendQuatTo.q2 * MATH_gst_BlendQuatFrom.q2 + MATH_gst_BlendQuatTo.q1 * MATH_gst_BlendQuatFrom.q1);
        if (cosTheta < 0.0)
        {
            cosTheta = -cosTheta;
            MATH_InvertQuaternion(&MATH_gst_BlendQuatTo);
        }
        if (fabs(cosTheta) >= 0.99999988)
        {
            MATH_gf_BlendTheta = 0.0;
            MATH_gf_BlendInvSinTheta = 0.0;
        }
        else
        {
            MATH_gf_BlendTheta = (float)acos(cosTheta);
            MATH_gf_BlendInvSinTheta = (float)(1.0 / sqrt(1.0 - cosTheta * cosTheta));
        }
    }
    dest->mtxflag |= 4u;
    MATH_Slerp(&tmpQ, factor, &MATH_gst_BlendQuatFrom, &MATH_gst_BlendQuatTo, MATH_gf_BlendTheta, MATH_gf_BlendInvSinTheta);
    MATH_ConvertQuaternionToMatrix(dest, &tmpQ);
}

void MATH_QuaternionBlend(Mtx4x4* dest, Quat4* src1, Quat4* src2, float factor, BOOL dontUseDefaultValues)
{
    float invSinTheta;
    float theta;
    Quat4 tmpQ;
    float cosTheta;

    theta = 1.5700001f;
    invSinTheta = 1.0f;
    if (dontUseDefaultValues)
    {
        cosTheta = src1->q3 * src2->q3 + src1->q2 * src2->q2 + src1->q4 * src2->q4 + src1->q1 * src2->q1;
        if (cosTheta <= 1.0)
        {
            if (cosTheta < 0.0)
            {
                cosTheta = -cosTheta;
                // inverts the caller's quaternion in place
                MATH_InvertQuaternion(src2);
            }
        }
        else
        {
            cosTheta = 1.0;
        }
        if (fabs(cosTheta - 1.0f) <= 0.0000001f)
        {
            theta = 0.0;
            invSinTheta = 0.0;
        }
        else
        {
            theta = (float)acos(cosTheta);
            invSinTheta = (float)(1.0 / sqrt(1.0 - cosTheta * cosTheta));
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
    float rz;
    float ry;
    float rx;
    double bz;
    double by;
    double bx;
    float invPrec;

    invPrec = (float)(1.0 / precision);
    bx = invPrec * vec->x + MATH_gd_magicDoubleForRound;
    by = invPrec * vec->y + MATH_gd_magicDoubleForRound;
    bz = invPrec * vec->z + MATH_gd_magicDoubleForRound;
    rx = (float)*(int32_t *)&bx;
    vec->x = rx * precision;
    ry = (float)*(int32_t*)&by;
    vec->y = ry * precision;
    rz = (float)*(int32_t*)&bz;
    vec->z = rz * precision;
}

void MATH_SubstractVec(Vec3* dest, Vec3* src1, Vec3* src2)
{
    dest->x = src1->x - src2->x;
    dest->y = src1->y - src2->y;
    dest->z = src1->z - src2->z;
}

float MATH_VecLen(Vec3* v)
{
    return (float)sqrt(MATH_VecLenSquared(v));
}

float MATH_f_PointLineDistance(Vec3* point, Vec3* lineFrom, Vec3* lineTo)
{
    Vec3 src1;
    Vec3 src2;
    Vec3 dest;

    MATH_SubstractVec(&src1, lineTo, lineFrom);
    MATH_SubstractVec(&src2, point, lineFrom);

    MATH_NormalizeEqualVector(&src1);
    MATH_CrossProduct(&dest, &src1, &src2);
    return MATH_VecLen(&dest);
}

float MATH_f_VecCosPlusArcCos(Vec3* vec1, Vec3* vec2)
{
    float vecCos;

    vecCos = MATH_f_VecCos(vec1, vec2);
    if (vecCos >= 1.0f)
        return 0.0f;
    if (vecCos <= -1.0f)
        return 3.1415927f;
    return (float)acos(vecCos);
}

void MATH_calc_curr_quat_from_dt_800(Quat4* quat, CompressedQuat* delta)
{
    float dz;
    float dy;
    float dx;

    dx = (float)delta->s1;
    quat->q1 = (float)(dx / 1024.0);
    dy = (float)delta->s2;
    quat->q2 = (float)(dy / 1024.0);
    dz = (float)delta->s3;
    quat->q3 = (float)(dz / 1024.0);
}

void MATH_calc_curr_quat_from_dt_1000(Quat4* quat, CompressedQuat* delta)
{
    float dz;
    float dy;
    float dx;

    dx = (float)delta->s1;
    quat->q1 = (float)(dx / 1024.0 + quat->q1);
    dy = (float)delta->s2;
    quat->q2 = (float)(dy / 1024.0 + quat->q2);
    dz = (float)delta->s3;
    quat->q3 = (float)(dz / 1024.0 + quat->q3);
}

void MATH_calc_curr_quat_from_dt_2000(Quat4* quat, UltraCompressedQuat* delta)
{
    float dz;
    float dy;
    float dx;

    dx = (float)delta->s1;
    quat->q1 = (float)(dx / 1024.0 + quat->q1);
    dy = (float)delta->b2;
    quat->q2 = (float)(dy / 1024.0 + quat->q2);
    dz = (float)delta->b3;
    quat->q3 = (float)(dz / 1024.0 + quat->q3);
}

void MATH_calc_curr_quat_from_dt_4000(Quat4* quat, UltraCompressedQuat* delta)
{
    float dz;
    float dy;
    float dx;

    dx = (float)delta->b2;
    quat->q1 = (float)(dx / 1024.0 + quat->q1);
    dy = (float)delta->s1;
    quat->q2 = (float)(dy / 1024.0 + quat->q2);
    dz = (float)delta->b3;
    quat->q3 = (float)(dz / 1024.0 + quat->q3);
}

void MATH_calc_curr_quat_from_dt_8000(Quat4* quat, UltraCompressedQuat* delta)
{
    float dz;
    float dy;
    float dx;

    dx = (float)delta->b2;
    quat->q1 = (float)(dx / 1024.0 + quat->q1);
    dy = (float)delta->b3;
    quat->q2 = (float)(dy / 1024.0 + quat->q2);
    dz = (float)delta->s1;
    quat->q3 = (float)(dz / 1024.0 + quat->q3);
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

void MATH_VecAddVecWithoutBuffer(Vec3* dest, Vec3* src)
{
    dest->x = dest->x + src->x;
    dest->y = src->y + dest->y;
    dest->z = src->z + dest->z;
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

void MATH_SetNullVector(Vec3* vec)
{
    vec->x = 0.0;
    vec->y = 0.0;
    vec->z = 0.0;
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

float MATH_f_MaxOf3(float a, float b, float c)
{
    double result;

    result = a;
    if (b > (double)a)
        result = b;
    if (c > result)
        result = c;
    return (float)result;
}

float MATH_f_MinOf3(float a, float b, float c)
{
    double result;

    result = a;
    if (b < (double)a)
        result = b;
    if (c < result)
        result = c;
    return (float)result;
}

void MATH_VecRotate(Vec3* dest, Vec3* axis, Vec3* src, float angle)
{
    float sinA;
    float cosA;
    Vec3 sinTerm;
    Vec3 perp;
    Vec3 perpCross;
    float proj;
    float axisLenSq;
    float parX;
    float parY;
    float parZ;

    perp = *src;
    proj = perp.z * axis->z + perp.y * axis->y + perp.x * axis->x;
    axisLenSq = axis->z * axis->z + axis->y * axis->y + axis->x * axis->x;
    proj = proj / axisLenSq;
    parX = proj * axis->x;
    parY = axis->y * proj;
    parZ = axis->z * proj;
    perp.x = perp.x - parX;
    perp.y = perp.y - parY;
    perp.z = perp.z - parZ;
    cosA = (float)cos(angle);
    dest->x = perp.x * cosA;
    dest->y = perp.y * cosA;
    dest->z = perp.z * cosA;
    // normalizes the caller's axis in place
    MATH_NormalizeEqualVector(axis);
    MATH_CrossProduct(&perpCross, axis, &perp);
    sinA = (float)sin(angle);
    sinTerm.x = perpCross.x * sinA;
    sinTerm.y = perpCross.y * sinA;
    sinTerm.z = perpCross.z * sinA;
    MATH_VecAddVec(dest, dest, &sinTerm);
    dest->x = parX + dest->x;
    dest->y = parY + dest->y;
    dest->z = parZ + dest->z;
}


float MATH_GetCos(float f)
{
    int idx;
    double biased;

    biased = MATH_gf_1024by2Pi * f + MATH_gd_magicDoubleForRound;
    idx = (*((int32_t *)(&biased))) + 256;
    if ((*((int32_t*)(&biased))) + 256 < 0) {
        idx |= ~0x3FF; // make sure it's not greather than -1023 (0x3ff) (negative numbers are like 0xffffffffe formats two's complement)
        return MATH_gf_TableSin[1024 + idx];
    }
    if (idx > 1023)
        idx &= 0x3FFu;
    return MATH_gf_TableSin[idx];
}

float MATH_GetSin(float f)
{
    int idx;
    double biased;

    biased = MATH_gf_1024by2Pi * f + MATH_gd_magicDoubleForRound;
    idx = (*((uint32_t*)(&biased)));
    if ((*((int32_t*)(&biased))) < 0) {
        idx |= ~0x3FF; // make sure it's not greather than -1023 (0x3ff) (negative numbers are like 0xffffffffe formats two's complement)
        return MATH_gf_TableSin[1024 + idx];
    }
    if ((*((int32_t*)(&biased))) > 1023)
        idx = (*((uint32_t*)(&biased))) & 0x3FF;
    return MATH_gf_TableSin[idx];
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

float MATH_DotProduct(Vec3* vec1, Vec3* vec2)
{
    return vec1->z * vec2->z + vec1->y * vec2->y + vec1->x * vec2->x;
}

