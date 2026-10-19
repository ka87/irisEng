#include "memory.h"
#include "loading.h"
#include "time.h"
#include "animation.h"
#include "dynamics.h"

void DYN_InitModule()
{
    DYN_gf_AutoOrientLowInertiaValue = -10.0f;
    DYN_gf_AutoOrientMediumInertiaValue = -3.0f;
    DYN_gf_AutoOrientHiInertiaValue = -1.0f;
}

void DYN_CloseModule()
{
}

void DYN_SpeedGetVector(struct sDyn* dyn, Vec3* v)
{
    MATH_VecMultScalar(v, &dyn->forceVector, dyn->mass);
}

void DYN_CalcSlideVector(Vec3* slideVec, Vec3* v1, Vec3* v2, float scale)
{
    MATH_AddScaleVector(slideVec, v2, v1, -MATH_DotProduct(v1, v2));
    MATH_VecMultScalar(slideVec, slideVec, scale);
}

void DYN_CalcReboundVector(Vec3* reboundVec, Vec3* v1, Vec3* v2, float scale)
{
    MATH_VecMultScalar(reboundVec, v1, -MATH_DotProduct(v1, v2));
    MATH_VecMultScalar(reboundVec, reboundVec, scale);
}

void DYN_SetSpeedVector(struct sDyn* a1, Vec3* a2)
{
    MATH_VecMultScalar(&a1->forceVector, a2, (1.0f / a1->mass));
}

tDynForces* DYN_pst_Forces_Create()
{
    tDynForces* forces = (tDynForces*)MEM_p_Alloc(sizeof(tDynForces));
    
    memset(forces, 0, sizeof(tDynForces));

    return forces;
}

void DYN_Forces_Init(tDynForces* dyn, Vec3* gravity, Vec3* traction, Vec3* stream, float frictionX)
{
    if (gravity)
        dyn->gravityVector = *gravity;
    if (traction)
        dyn->tractionVector = *traction;
    if (stream)
        dyn->streamVec = *stream;
    
    dyn->frictionVector.x = frictionX;
    dyn->frictionVector.y = 0.0f;
    dyn->frictionVector.z = 0.0f;
    dyn->i3 = 0;
}

void DYN_Forces_Close(void* forces)
{
    MEM_Free(forces);
}

tDynSolid* DYN_pst_SolidCreate()
{
    return (tDynSolid*)MEM_p_Alloc(sizeof(tDynSolid));
}

void DYN_SolidInit(tDynSolid* solid, Vec3* rotSpeedVec, Vec3* torqueVec, Vec3* pos, float speedLimit)
{
    solid->rotSpeedLimit = speedLimit;
    solid->elastic = 0.5f;
    solid->oldPos = *pos;
    solid->rotSpeedVector = *rotSpeedVec;
    solid->torqueVector = *torqueVec;
}

void DYN_SolidClose(void* solid)
{
    MEM_Free(solid);
}

tDynCol* DYN_pst_ColCreate()
{
    return (tDynCol*)MEM_p_Alloc(sizeof(tDynCol));
}

void DYN_ColInit(tDynCol* a1, float a2, float a3, float a4, float a5)
{
    if (a1)
    {
        a1->reboundHorizCosAngle = a4;
        a1->slideHorizCosAngle = a5;
        a1->rebound = a2;
        a1->slide = a3;
    }
}

void DYN_ColReinit(tDynCol* a1)
{
    DYN_ColInit(a1, 0.0f, 1.0f, 1.0f, 1.0f);
}

void DYN_ColClose(void* a1)
{
    MEM_Free(a1);
}

tDynConstraint* DYN_pst_ConstraintCreate()
{
    tDynConstraint* v0; // edx

    v0 = (tDynConstraint*)MEM_p_Alloc(sizeof(tDynConstraint));
    memset(v0, 0, sizeof(tDynConstraint));
    v0->tangentVec = MATH_gst_NulVector;
    return v0;
}

void DYN_ConstraintReinit(tDynConstraint* a1)
{
    a1->constraintType = 0;
    a1->tangentVec = MATH_gst_NulVector;
}

void DYN_ConstraintClose(void* a1)
{
    MEM_Free(a1);
}

void DYN_ChangeDynFlags(struct sDyn* a1, int a2, int a3)
{
    int v3;
    tDynForces* v4;
    tDynSolid* v5;
    tDynCol* v6;
    tDynConstraint* v7;
    Vec3* v8;

    if ((a2 & 2) + (a2 & 4) + (a2 & 8))
        v3 = a2 & ~1u;
    else
        v3 = a2 | 1;
    if ((v3 & 4) != 0)
    {
        if ((a3 & 4) == 0)
        {
            v4 = DYN_pst_Forces_Create();
            a1->forces = v4;
            DYN_Forces_Init(v4, &MATH_gst_NulVector, &MATH_gst_NulVector, &MATH_gst_NulVector, 1.0);
            v3 &= ~2u;
        }
    }
    else if ((a3 & 4) != 0)
    {
        DYN_Forces_Close(a1->forces);               // DYN_Forces_Close
        a1->forces = 0;
    }
    if ((v3 & 0x10000) != 0)
    {
        if ((a3 & 0x10000) == 0)
        {
            v5 = DYN_pst_SolidCreate();
            v8 = a1->vptr;
            a1->solid = v5;
            DYN_SolidInit(v5, &MATH_gst_NulVector, &MATH_gst_NulVector, v8, 3.402822e38f);
        }
    }
    else if ((a3 & 0x10000) != 0)
    {
        DYN_SolidClose(a1->solid);
        a1->solid = 0;
    }
    if ((v3 & 0x20000) != 0)
    {
        if ((a3 & 0x20000) == 0)
        {
            v6 = DYN_pst_ColCreate();
            a1->collision = v6;
            DYN_ColReinit(v6);
        }
    }
    else if ((a3 & 0x20000) != 0)
    {
        DYN_ColClose(a1->collision);
        a1->collision = 0;
    }
    if ((v3 & 0x200000) != 0)
    {
        if ((a3 & 0x200000) == 0)
        {
            v7 = DYN_pst_ConstraintCreate();
            a1->constraint = v7;
            DYN_ConstraintReinit(v7);
            a1->flags = v3;
            return;
        }
    }
    else if ((a3 & 0x200000) != 0)
    {
        DYN_ConstraintClose(a1->constraint);
        a1->constraint = 0;
    }
    a1->flags = v3;
}

void DYN_Dyna_Init(tDyn* dyn, int flags, float mmass, float speedLimit, float speedLimitVertical, Vec3* speedVec, Vec3* pos)
{
    double mass;
    int v8;

    mass = 1.0 / mmass;
    dyn->mass = (float)mass;
    dyn->forceVector.x = (float)(1.0 / mass * speedVec->x);
    dyn->forceVector.y = (float)(speedVec->y * (1.0 / mass));
    dyn->forceVector.z = (float)(speedVec->z * (1.0 / mass));
    dyn->vptr = pos;
    v8 = dyn->flags;
    dyn->speedLimit = speedLimit;
    dyn->speedLimitVertical = speedLimitVertical;
    DYN_ChangeDynFlags(dyn, flags, v8);
    dyn->flags &= ~0xC00000u;
}

float DYN_f_FindFrictionForMaxSpeed(struct sDyn* a1, float a2)
{
    tDynForces* v2;

    v2 = a1->forces;
    return (float)sqrt(
        v2->tractionVector.x * v2->tractionVector.x
        + v2->tractionVector.y * v2->tractionVector.y
        + v2->tractionVector.z * v2->tractionVector.z)
        / a2;
}

void DYN_ApplyHorizontalSpeed(struct sDyn* a1, float a2)
{
    long double v2; // st4
    long double v3; // st7

    v2 = sqrt(
        a1->forceVector.x * a1->forceVector.x
        + a1->forceVector.y * a1->forceVector.y
        + a1->forceVector.z * a1->forceVector.z);
    if (v2 != 0.0)
    {
        v3 = 1.0 / a1->mass * a2 / v2;
        a1->forceVector.x = (float)(v3 * a1->forceVector.x);
        a1->forceVector.y = (float)(v3 * a1->forceVector.y);
        a1->forceVector.z = (float)(v3 * a1->forceVector.z);
    }
}

void DYN_ConstraintToSegment(tDyn* a1, tDynConstraint* a2, Vec3* a3)
{
    tDynConstraint* v3;
    long double v4;
    Vec3 v5;

    v3 = a1->constraint;
    MATH_PointSegmentProjection(&v5, a1->vptr, &a2->point1, a3);
    *a1->vptr = v5;
    v3->tangentVec.x = a3->x - a2->point1.x;        // MATH_NormalizeEqualVector inline
    v3->tangentVec.y = a3->y - a2->point1.y;
    v3->tangentVec.z = a3->z - a2->point1.z;
    v4 = sqrt(v3->tangentVec.x * v3->tangentVec.x + v3->tangentVec.y * v3->tangentVec.y + v3->tangentVec.z
        * v3->tangentVec.z);
    v3->tangentVec.x = (float)(1.0 / v4 * v3->tangentVec.x);
    v3->tangentVec.y = (float)(1.0 / v4 * v3->tangentVec.y);
    v3->tangentVec.z = (float)(1.0 / v4 * v3->tangentVec.z);
}

void DYN_ConstraintToParabol(tDyn* a1, tDynConstraint* a2, Vec3* a3, Vec3* a4, float a5)
{
    Vec3* v5;
    long double v6;
    Vec3 a1a;

    v5 = &a1->constraint->tangentVec;
    MATH_PointParabolProjection3D(&a1a, v5, a1->vptr, &a2->point1, a3, a4, a5);
    *a1->vptr = a1a;                              // MATH_NormalizeEqualVector inline
    v6 = sqrt(v5->x * v5->x + v5->y * v5->y + v5->z * v5->z);
    v5->x = (float)(1.0 / v6 * v5->x);
    v5->y = (float)(1.0 / v6 * v5->y);
    v5->z = (float)(1.0 / v6 * v5->z);
}

void DYN_ApplyConstraint(tDyn* a1)
{
    tDynConstraint* v2;
    byte v3;
    double v4;
    double v5;
    double v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;

    v2 = a1->constraint;
    v3 = v2->constraintType;
    if (v3 == 1)
    {
        DYN_ConstraintToSegment(a1, v2, &v2->point2);
    }
    else if (v3 == 2)
    {
        DYN_ConstraintToParabol(a1, v2, &v2->point2, &v2->parabolO, v2->parabolRadius);
    }
    if (v2->constraintType)
    {
        v4 = a1->mass;
        v7 = (float)(v4 * a1->forceVector.x);
        if (fabs(v7) > 0.0001// MATH_b_NulVectorWithEpsilon inline
            || fabs(a1->forceVector.y * v4) > 0.0001
            || fabs(a1->forceVector.z * v4) > 0.0001)
        {
            v8 = (float)(a1->forceVector.y * v4);
            v10 = (float)(a1->forceVector.z * v4);
            v5 = v2->tangentVec.z * v10 + v2->tangentVec.y * v8 + v7 * v2->tangentVec.x;
            v9 = (float)(v2->tangentVec.y * v5);
            v11 = (float)(v2->tangentVec.z * v5);
            v6 = 1.0 / a1->mass;
            a1->forceVector.x = (float)(v6 * (v5 * v2->tangentVec.x));
            a1->forceVector.y = (float)(v9 * v6);
            a1->forceVector.z = (float)(v11 * v6);
        }
    }
}

void DYN_SolidInitWithDefaults(tDynSolid* a1)
{
    DYN_SolidInit(a1, &MATH_gst_NulVector, &MATH_gst_NulVector, &MATH_gst_NulVector, 3.402822e38f);
}

void DYN_CalcRotationSpeed(tDyn* a1, float a2)
{
    tDynSolid* v2;
    Vec3* v3;
    double v4;
    float v5;
    float v6;

    v2 = a1->solid;
    if (v2)
        v3 = &v2->torqueVector;
    else
        v3 = 0;
    v4 = a1->mass * a2;
    v5 = (float)(v3->y * v4);
    v6 = (float)(v3->z * v4);
    v2->rotSpeedVector.x = (float)(v4 * v3->x + v2->rotSpeedVector.x);
    v2->rotSpeedVector.y = v5 + v2->rotSpeedVector.y;
    v2->rotSpeedVector.z = v6 + v2->rotSpeedVector.z;
}

void DYN_ApplyRotationSpeedLimit(tDyn* a1)
{
    tDynSolid* v1;
    long double v2;
    float v3;

    v1 = a1->solid;
    v2 = sqrt(
        v1->rotSpeedVector.x * v1->rotSpeedVector.x
        + v1->rotSpeedVector.y * v1->rotSpeedVector.y
        + v1->rotSpeedVector.z * v1->rotSpeedVector.z);
    if (v1)
        v3 = v1->rotSpeedLimit;
    else
        v3 = 0.0;
    if (v2 > v3)
        MATH_SetNormVector(&v1->rotSpeedVector, &v1->rotSpeedVector, v3);
}

void DYN_MakeWMatrix(Mtx4x4* a1, Vec3* a2, float a3)
{
    double v3;
    double v4;
    double v6;
    int v7;
    double v8;
    long double v9;
    long double v10;
    long double v11;
    float v12;
    float a3a;

    v3 = a3 * a2->x;                              // MATH_SetIdentityMatrix
    v4 = a2->y * a3;
    v6 = a2->z * a3;
    a1->v2.x = 0.0;
    a1->v2.z = 0.0;
    v12 = (float)v6;
    a1->v3.x = 0.0;
    a1->v3.y = 0.0;
    a1->v1.x = 1.0;
    a3a = (float)-v4;
    a1->v1.y = 0.0;
    a1->v1.z = 0.0;
    a1->_m14 = 0.0;
    a1->v2.x = -v12;
    a1->_m24 = 0.0;
    a1->_m34 = 0.0;
    a1->v2.z = (float)v3;
    a1->Pos.x = 0.0;
    a1->Pos.y = 0.0;
    a1->v3.x = (float)v4;
    a1->Pos.z = 0.0;
    v7 = a1->mtxflag;
    a1->v2.y = 1.0;
    a1->v3.y = (float)-v3;
    a1->v3.z = 1.0;
    v8 = a1->v1.x;
    a1->w = 1.0;
    a1->mtxflag = v7 & 2;
    v9 = sqrt(a3a * a3a + v8 * v8 + v12 * v12);   // MATH_NormalizeEqualVector x3
    a1->v1.x = (float)(1.0 / v9 * a1->v1.x);
    a1->v1.y = (float)(v12 * (1.0 / v9));
    a1->v1.z = (float)(a3a * (1.0 / v9));
    v10 = sqrt(a1->v2.x * a1->v2.x + a1->v2.y * a1->v2.y + a1->v2.z * a1->v2.z);
    a1->v2.x = (float)(1.0 / v10 * a1->v2.x);
    a1->v2.y = (float)(1.0 / v10 * a1->v2.y);
    a1->v2.z = (float)(1.0 / v10 * a1->v2.z);
    v11 = sqrt(a1->v3.x * a1->v3.x + a1->v3.y * a1->v3.y + a1->v3.z * a1->v3.z);
    a1->v3.x = (float)(1.0 / v11 * a1->v3.x);
    a1->v3.y = (float)(1.0 / v11 * a1->v3.y);
    a1->v3.z = (float)(1.0 / v11 * a1->v3.z);
}

void DYN_ApplyAngularVelocity(tDyn* a1, float a3)
{
    Mtx4x4* v2;
    Mtx4x4 a1a;
    Mtx4x4 a2a;

    DYN_MakeWMatrix(&a2a, &a1->solid->rotSpeedVector, a3);
    v2 = (Mtx4x4*)&a1->vptr[-4]; // ez egy rettentõ igénytelen megoldás ohgy egy pozícióbol (mátrix 4. sora) haogyan kapjuk vissza az egész mátrixot, és ez egyben azt is feltételezi
                                 // hogy ne egy egyedüöli vektor lett megadva, hanem a mátrix adott része (&mtx.Pos) A vptr a DYN_Dyna_Init-ben állítódik be ami két helyrõl van hívva, 
                                 // és mindkét helyen egy mátrix adott részét kapja. Ez a szerencse
    MATH_MulMatrixMatrix(&a1a, &a2a, v2);
    if (v2 != &a1a)
        memcpy(v2, &a1a, sizeof(Mtx4x4));
    MATH_Orthonormalize(v2);
}

tDyn* DYN_pst_Dyna_Create()
{
    tDyn* dyn;

    dyn = (tDyn*)MEM_p_Alloc(sizeof(tDyn));
    LOA_ul_FileTypeSize_37 += sizeof(tDyn);
    memset(dyn, 0, sizeof(tDyn));
    return dyn;
}

void DYN_Dyna_Close(tDyn* a1)
{
    if ((a1->flags & 4) != 0)
        DYN_Forces_Close(a1->forces);
    if ((a1->flags & 0x10000) != 0)
        DYN_SolidClose(a1->solid);
    if ((a1->flags & 0x20000) != 0)
        DYN_ColClose(a1->collision);
    if ((a1->flags & 0x200000) != 0)
        DYN_ConstraintClose(a1->constraint);
    MEM_Free(a1);
}

void DYN_SetHorizontalSpeedComponent(struct sDyn* a1, Vec3* a2)
{
    tDynForces* v3; // eax
    double v4; // st7
    double v5; // st7
    double v6; // st7
    long double v7; // st6
    long double v8; // rt1
    long double v9; // st6
    float v10; // [esp+0h] [ebp-18h]
    Vec3 v11; // [esp+0h] [ebp-18h]
    float v12; // [esp+4h] [ebp-14h]
    float v13; // [esp+4h] [ebp-14h]
    float v14; // [esp+8h] [ebp-10h]
    float v15; // [esp+8h] [ebp-10h]
    float v16; // [esp+Ch] [ebp-Ch]
    float v17; // [esp+10h] [ebp-8h]
    float v18; // [esp+14h] [ebp-4h]
    float a1a; // [esp+1Ch] [ebp+4h]

    if ((a1->flags & 0x40004) != 4)
        goto LABEL_5;
    v3 = a1->forces;
    v10 = v3->gravityVector.x;
    v14 = v3->gravityVector.z;
    v12 = v3->gravityVector.y;
    v4 = 1.0 / (1.0 / a1->mass);
    v16 = (float)(v4 * a1->forceVector.x);
    v17 = (float)(a1->forceVector.y * v4);
    v5 = (float)(v4 * a1->forceVector.z);
    if (v3->gravityVector.x != 0.0)
        goto LABEL_7;
    if (v12 == 0.0 && v14 == 0.0)
    {
    LABEL_5:
        v11 = *a2;
    }
    else
    {
    LABEL_7:
        v7 = 1.0 / sqrt(v14 * v14 + v12 * v12 + v10 * v10);
        v13 = (float)(v7 * v12);
        v15 = (float)(v7 * v14);
        v8 = v10 * v7;
        v9 = v5 * v15 + v17 * v13 + v16 * v8;
        a1a = (float)v9;
        v18 = a1a * v15;
        v11.x = (float)(v9 * v8 + a2->x);
        v11.y = a2->y + a1a * v13;
        v11.z = a2->z + v18;
    }
    v6 = 1.0 / a1->mass;
    a1->forceVector.x = (float)(v11.x * v6);
    a1->forceVector.y = (float)(v11.y * v6);
    a1->forceVector.z = (float)(v11.z * v6);
}


void DYN_CalculateSpeedWhenFrictionVector(tDyn* a1, float a2)
{
    int v2;
    tDynForces* v3;
    long double v4;
    Vec3* v5;
    double v6;
    double v7;
    double v7a;
    double v10;
    double v11;
    double v11a;
    double v14;
    double v15;
    double v15a;
    Vec3 v18;
    Vec3 v19;

    v2 = 0;
    MATH_VectorGlobalToLocal(&v19, (Mtx4x4*)&a1->vptr[-4], &a1->momentum); // a vptr[-4] egy gányolás, mivel az egész mátrix kell, lásd másik megjegyzést ezzel kapcs...
    MATH_VectorGlobalToLocal(&v18, (Mtx4x4*)&a1->vptr[-4], &a1->forceVector);
    v3 = a1->forces;
    v4 = v3->frictionVector.x;
    v5 = &v3->frictionVector;
    if (fabs(v4) >= 0.0000001)
        v2 = 1;
    else
        v18.x = v19.x * a2 + v18.x;
    if (fabs(v5->y) >= 0.0000001)
        v2 += 2;
    else
        v18.y = v19.y * a2 + v18.y;
    if (fabs(v5->z) >= 0.0000001)
        v2 += 4;
    else
        v18.z = v19.z * a2 + v18.z;
    if (v2)
    {
        if ((v2 & 1) != 0)
        {
            v6 = a1->mass * v5->x;
            v7 = -(v6 * a2) * 1.442695040888963387;
            //_ST4 = v7;
            //__asm { frndint }
            v7a = (float)((int)(v7 + 0.5f));
            //v18.x = 1.0 / v6 * v19.x - (__F2XM1__(v7 - _ST4) + 1.0) * (1.0 / v6 * v19.x - v18.x);
            v18.x = (float)(1.0 / v6 * v19.x - ((pow(2, (v7 - v7a)) - 1.0) + 1.0) * (1.0 / v6 * v19.x - v18.x));
        }
        if ((v2 & 2) != 0)
        {
            v10 = a1->mass * v5->y;
            v11 = -(v10 * a2) * 1.442695040888963387;
            //_ST4 = v11;
            //__asm { frndint }
            v11a = (float)((int)(v11 + 0.5f));
            //v18.y = 1.0 / v10 * v19.y - (__F2XM1__(v11 - _ST4) + 1.0) * (1.0 / v10 * v19.y - v18.y);
            v18.y = (float)(1.0 / v10 * v19.y - ((pow(2, (v11 - v11a)) - 1.0) + 1.0) * (1.0 / v10 * v19.y - v18.y));
        }
        if ((v2 & 4) != 0)
        {
            v14 = a1->mass * v5->z;
            v15 = -(v14 * a2) * 1.442695040888963387;
            //_ST4 = v15;
            //__asm { frndint }
            v15a = (float)((int)(v15 + 0.5f));
            //v18.z = 1.0 / v14 * v19.z - (__F2XM1__(v15 - _ST4) + 1.0) * (1.0 / v14 * v19.z - v18.z);
            v18.z = (float)(1.0 / v14 * v19.z - ((pow(2, (v15 - v15a)) - 1.0) + 1.0) * (1.0 / v14 * v19.z - v18.z));
        }
    }
    MATH_TransformVectorNoScale(&a1->forceVector, (Mtx4x4*)&a1->vptr[-4], &v18);
}

void dyn_frictionvector_subfunc_sub_427630(tDyn* a1, float a2, int a3)
{
    tDynForces* v4;
    tDynForces* v8;
    tDynForces* v10;
    double v11;
    tDynForces* v12;
    float v13;
    Vec3* v14;
    double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    float v22;
    long double v24;
    Vec3 v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;

    if ((a3 & 4) != 0)
    {
        if ((a3 & 0x100000) != 0)
        {
            v25.x = MATH_gst_NulVector.x;
            v25.y = MATH_gst_NulVector.y;
            v25.z = MATH_gst_NulVector.z;
        }
        else
        {
            v4 = a1->forces;
            v25.x = v4->streamVec.x;
            v25.z = v4->streamVec.z;
            v25.y = v4->streamVec.y;
        }
        if ((a3 & 0x80000) == 0)
        {
            v8 = a1->forces;
            v25.x = v25.x + v8->tractionVector.x;
            v25.y = v25.y + v8->tractionVector.y;
            v25.z = v25.z + v8->tractionVector.z;
        }
        if ((a3 & 0x40000) == 0)
        {
            v10 = a1->forces;
            v11 = 1.0 / a1->mass;
            v27 = (float)(v10->gravityVector.y * v11);
            v28 = (float)(v10->gravityVector.z * v11);
            v25.x = (float)(v25.x + v11 * v10->gravityVector.x);
            v25.y = v25.y + v27;
            v25.z = v25.z + v28;
        }
        a1->momentum = v25;
        v12 = a1->forces;
        if ((a3 & 0x1000) != 0)
        {
            v13 = v12->frictionVector.x;
            v14 = &v12->frictionVector;
            if (fabs(v13) > 0.0001// MATH_b_NulVectorWithEpsilon inline
                || fabs(v14->y) > 0.0001
                || fabs(v14->z) > 0.0001)
            {
                DYN_CalculateSpeedWhenFrictionVector(a1, a2);
                return;
            }
        }
        else if (fabs(v12->frictionVector.x) >= 0.000099999997)
        {
            v18 = v12->frictionVector.x * a1->mass;
            v32 = (float)(v25.x * (1.0 / v18));
            v33 = (float)(v25.y * (1.0 / v18));
            v19 = 1.0 / v18 * v25.z;
            v20 = v33 - a1->forceVector.y;
            v29 = (float)(v19 - a1->forceVector.z);
            v21 = -(v18 * a2) * 1.442695040888963387;
            //_ST2 = v21;
            //__asm { frndint }
            v22 = (float)((int)(v21 + 0.5f));
            //v24 = __F2XM1__(v21 - _ST2) + 1.0;
            v24 = (pow(2, (v21 - v22)) - 1.0) + 1.0; // TODO FIXME, jó ez így?
            v34 = (float)v24;
            v26 = (float)(v24 * (v32 - a1->forceVector.x));
            v30 = v29 * v34;
            a1->forceVector.x = v32 - v26;
            a1->forceVector.y = (float)(v33 - v34 * v20);
            a1->forceVector.z = (float)(v19 - v30);
            return;
        }
        v15 = v25.x * a2;
        v16 = v25.y * a2;
        v17 = v25.z;
    }
    else
    {
        if ((a3 & 2) == 0)
            return;
        v15 = a2 * a1->momentum.x;
        v16 = a1->momentum.y * a2;
        v17 = a1->momentum.z;
    }
    v31 = (float)(v17 * a2);
    a1->forceVector.x = (float)(v15 + a1->forceVector.x);
    a1->forceVector.y = (float)(v16 + a1->forceVector.y);
    a1->forceVector.z = v31 + a1->forceVector.z;
}

void DYN_ApplySpeedLimit(struct sDyn* a1)
{
    double v2;
    double v3;
    double v5;
    tDynForces* v8;
    double v9;
    float v10;
    float v11;
    char v12;
    long double v13;
    long double v14;
    long double v15;
    double v16;
    double v17;
    double v18;
    long double v19;
    float a3;
    float norm;
    Vec3 a1a;
    Vec3 a2;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;

    a3 = a1->speedLimit;
    norm = a1->speedLimitVertical;
    v2 = a1->mass;
    v3 = v2 * a1->forceVector.x;
    v5 = a1->forceVector.y * v2;
    if (a3 == norm)
    {
        v19 = sqrt(a1->forceVector.z * v2 * (a1->forceVector.z * v2) + v5 * v5 + v3 * v3);
        v29 = (float)v19;
        if (v19 > a3)
            v29 = a3;
        DYN_ApplyHorizontalSpeed(a1, v29);
    }
    else
    {
        v24 = (float)v5;
        v8 = a1->forces;
        v9 = a1->forceVector.z;
        a1a.x = v8->gravityVector.x;
        v10 = v8->gravityVector.y;
        v11 = v8->gravityVector.z;
        v25 = (float)(v9 * v2);
        v12 = 0;
        v13 = 1.0 / sqrt(v11 * v11 + v10 * v10 + a1a.x * a1a.x);
        v14 = a1a.x * v13;
        a1a.y = (float)(v10 * v13);
        v15 = v11 * v13;
        a1a.z = (float)v15;
        v26 = (float)(v15 * v25 + a1a.y * v24 + v14 * v3);
        a2.x = (float)(v14 * v26);
        a2.y = a1a.y * v26;
        a2.z = a1a.z * v26;
        v27 = (float)fabs(v26);
        a1a.x = (float)(v3 - a2.x);
        a1a.y = v24 - a2.y;
        v16 = v25 - a2.z;
        a1a.z = (float)v16;
        if (sqrt(v16 * a1a.z + a1a.y * a1a.y + a1a.x * a1a.x) > a3)
        {
            MATH_SetNormVector(&a1a, &a1a, a3);
            v12 = 1;
        }
        if (v27 <= (double)norm)
        {
            if (!v12)
                return;
        }
        else
        {
            MATH_SetNormVector(&a2, &a2, norm);
        }
        v17 = a1a.y + a2.y;
        v25 = a1a.z + a2.z;
        v18 = 1.0 / a1->mass;
        v28 = (float)v18;
        a1->forceVector.x = (float)(v18 * (a1a.x + a2.x));
        a1->forceVector.y = (float)(v17 * v28);
        a1->forceVector.z = v25 * v28;
    }
}

void DYN_ApplySpeed_sub_427870(tGameObjectMain* a1, tDyn* a2, float a3, int a4, Vec3* a5)
{
    Vec3* v5;
    double v6;
    double v7;
    float v8;
    double v9;
    double v10;
    double v11;
    tDynConstraint* v12;
    double v13;
    Vec3* v14;
    float v15;
    tGameObjectBasic* v16;
    tGaoMatrix* v17;
    tGameObjectMain* v18;
    Mtx4x4* v19;
    tGameObjectBasic* v20;
    Mtx4x4* v21;
    tGameObjectBasic* v22;
    tGameObjectActorObject* v23;
    Vec3 a3a;
    Vec3 v25;
    float v26;
    float v27;
    float v28;
    Mtx4x4 a1a;

    v5 = a2->vptr;
    v6 = a2->mass;
    v7 = v6 * a2->forceVector.x;
    v26 = v5->x;
    v27 = v5->y;
    v8 = v5->z;
    a3a.x = (float)v7;
    v9 = a2->forceVector.y;
    v28 = v8;
    a3a.y = (float)(v9 * v6);
    a3a.z = (float)(a2->forceVector.z * v6);
    if (a5)
    {
        v10 = a3a.x + a5->x;
        v11 = a5->y + a3a.y;
        v25.z = a5->z + a3a.z;
        v25.x = (float)(v10 * 0.5);
        v25.y = (float)(v11 * 0.5);
        v25.z = (float)(v25.z * 0.5);
    }
    else
    {
        v25 = a3a;
    }
    if ((a4 & 0x10000000) != 0 && a2->forces)
    {
        a3a = v25;
    }
    else
    {
        a3a.x = v25.x * a3;
        a3a.y = v25.y * a3;
        a3a.z = v25.z * a3;
    }
    if ((a4 & 0x200000) != 0)
    {
        v12 = a2->constraint;
        if (fabs(v12->tangentVec.x) > 0.0001// MATH_b_NulVectorWithEpsilon inline
            || fabs(v12->tangentVec.y) > 0.0001
            || fabs(v12->tangentVec.z) > 0.0001)
        {
            v13 = v12->tangentVec.z * a3a.z + v12->tangentVec.y * a3a.y + a3a.x * v12->tangentVec.x;
            a3a.x = (float)(v13 * v12->tangentVec.x);
            a3a.y = (float)(v12->tangentVec.y * v13);
            a3a.z = (float)(v12->tangentVec.z * v13);
        }
        v5->x = a3a.x + v5->x;
        v5->y = a3a.y + v5->y;
        v5->z = a3a.z + v5->z;
        DYN_ApplyConstraint(a2);
    }
    else
    {
        v5->x = a3a.x + v5->x;
        v5->y = a3a.y + v5->y;
        v5->z = a3a.z + v5->z;
        if ((a2->flags & 0x4000) != 0)
        {
            v14 = a2->vptr;
            if (a3a.z * (a2->maxPos.z - v14->z) + (a2->maxPos.y - v14->y) * a3a.y + (a2->maxPos.x - v14->x) * a3a.x < 0.0)
            {
                v15 = a2->maxPos.x;
                a3a.x = v15 - v26;
                a3a.y = a2->maxPos.y - v27;
                a3a.z = a2->maxPos.z - v28;
                v14->x = v15;
                v14->y = a2->maxPos.y;
                v14->z = a2->maxPos.z;
            }
        }
    }
    if ((a1->flags & 0x400000) != 0)
    {
        v16 = a1->basic;
        if (v16 && (v17 = v16->mtx) != 0)
            v18 = v17->refGao2;
        else
            v18 = 0;
        v19 = OBJ_pst_GetAbsoluteMatrix(v18);
        MATH_InvertMatrix(&a1a, v19);
        MATH_TransformVector(&a3a, &a1a, &a3a);
    }
    if ((a1->flags & 2) != 0)
    {
        v20 = a1->basic;
        if (v20)
        {
            if (v20->actorObjectOrVisualObject)
            {
                if ((a1->flags & 0x4000000) != 0)
                {
                    v21 = OBJ_pst_GetFlashMatrix(a1);
                    MATH_VecAddVecWithoutBuffer(&v21->Pos, &a3a);
                }
                if (ANI_GaoAnimationIsTransitioning(a1))
                {
                    v22 = a1->basic;
                    v23 = (tGameObjectActorObject*)v22->actorObjectOrVisualObject;
                    MATH_VecAddVecWithoutBuffer(&v23->animPtr[1]->flashMtx.Pos, &a3a);
                    MATH_VecAddVecWithoutBuffer((Vec3*)&v23->animPtr[0]->flashMtx._m14, &a3a);
                    MATH_VecAddVecWithoutBuffer(&v23->animPtr[0]->flashMtx.Pos, &a3a);
                }
            }
        }
    }
}

void DYN_AutoOrient_sub_4273F0(tGameObjectMain* a1, tDyn* a2, int a3, float a4)
{
    Vec3* v4; // esi
    int v5; // edi
    long double v6; // st4
    double v7; // st7
    double v8; // st7
    double v8a;
    float a4a; // [esp+0h] [ebp-48h]
    Vec3 a1a; // [esp+18h] [ebp-30h] BYREF
    Vec3 a3a; // [esp+24h] [ebp-24h] BYREF
    Vec3 v14; // [esp+30h] [ebp-18h] BYREF
    Vec3 a2a; // [esp+3Ch] [ebp-Ch] BYREF

    v4 = &a2->forceVector;
    if (fabs(a2->forceVector.x) > 0.01// MATH_b_NulVectorWithEpsilon inline
        || fabs(a2->forceVector.y) > 0.01
        || fabs(a2->forceVector.z) > 0.01)
    {
        v5 = a3 & 0xC00;
        if ((a3 & 0xC00) != 0)
        {                                           // MATH_NormalizeVector inline
            v6 = sqrt(a2->forceVector.z * a2->forceVector.z + a2->forceVector.y * a2->forceVector.y + v4->x * v4->x);
            a3a.x = (float)(1.0 / v6 * v4->x);
            a3a.y = (float)(1.0 / v6 * a2->forceVector.y);
            a3a.z = (float)(1.0 / v6 * a2->forceVector.z);
            OBJ_SightGet(a1, &a2a);
            if (v5 == 0x400)
            {
                v7 = DYN_gf_AutoOrientLowInertiaValue;
            }
            else if (v5 == 0x800)
            {
                v7 = DYN_gf_AutoOrientMediumInertiaValue;
            }
            else
            {
                v7 = DYN_gf_AutoOrientHiInertiaValue;
            }
            v8 = v7 * a4 * 1.442695040888963387;
            v8a = (float)((int)(v8 + 0.5f));
            //_ST6 = v8;
            //__asm { frndint }
            //a4a = __F2XM1__(v8 - _ST6) + 1.0;
            
            a4a = (float)((pow(2, (v8 - v8a)) - 1.0) + 1.0); // TODO FIXME, jó ez így?
            MATH_InterpolateBetweenTwoPosWithFactor(&v14, &a2a, &a3a, a4a);
            if ((a3 & 0x2000) != 0)
            {
                MATH_NormalizeVector(&a1a, &a2->forces->gravityVector);
                a1a.y = -a1a.y;
                a1a.x = -a1a.x;
                a1a.z = -a1a.z;
                if (MATH_b_VecColinear(&a1a, &v14))
                {
                LABEL_8:
                    OBJ_BankingGeneralSet(a1, &MATH_gst_NulVector, &a1a);
                    return;
                }
                OBJ_BankingGeneralSet(a1, &v14, &a1a);
            }
            else
            {
                OBJ_SightSet(a1, &v14);
            }
        }
        else
        {
            if ((a3 & 0x2000) != 0)
            {
                MATH_NormalizeVector(&a1a, &a2->forces->gravityVector);
                a1a.y = -a1a.y;
                a1a.x = -a1a.x;
                a1a.z = -a1a.z;
                if (!MATH_b_VecColinear(&a1a, v4))
                {
                    OBJ_BankingGeneralSet(a1, v4, &a1a);
                    return;
                }
                goto LABEL_8;
            }
            OBJ_SightSet(a1, &a2->forceVector);
        }
    }
}

void DYN_OneCall(struct sGameObjectMain* a1)
{
    tDyn* v2;
    int v3;
    double v4;
    int v5;
    BOOL v6;
    tDynForces* v7;
    Vec3 v8;
    int v9;

    v2 = a1->basic->dyn;
    v3 = a1->flags & 0x400000;
    v9 = v3;
    if ((v2->flags & 0x4000000) != 0)
        TIM_gf_dt = TIM_gf_dt / TIM_gf_SpeedFactor;
    if (v3)
        OBJ_ComputeGlobalWhenHie(a1);
    v4 = v2->mass;
    v5 = v2->flags;
    v6 = (v2->flags & 1) == 0;
    v8.x = (float)(v4 * v2->forceVector.x);
    v8.y = (float)(v2->forceVector.y * v4);
    v8.z = (float)(v2->forceVector.z * v4);
    if (v6)
    {
        if ((v5 & 0x10000000) != 0 && (v7 = v2->forces) != 0)
        {
            v2->forceVector = v7->tractionVector;
        }
        else if ((v5 & 0x1C0000) != 0x1C0000)
        {
            dyn_frictionvector_subfunc_sub_427630(v2, TIM_gf_dt, v5);
        }
    }
    if ((v5 & 0x10000000) == 0 || !v2->forces)
        DYN_ApplySpeedLimit(v2);
    if ((v2->flags & 0x8000000) != 0 || (v5 & 0x10000000) != 0 && v2->forces)
        DYN_ApplySpeed_sub_427870(a1, v2, TIM_gf_dt, v5, 0);
    else
        DYN_ApplySpeed_sub_427870(a1, v2, TIM_gf_dt, v5, &v8);
    if ((v5 & 0x200) != 0)
        DYN_AutoOrient_sub_4273F0(a1, v2, v5, TIM_gf_dt);
    if ((v5 & 0x10000) != 0)
    {
        DYN_CalcRotationSpeed(v2, TIM_gf_dt);
        DYN_ApplyRotationSpeedLimit(v2);
        DYN_ApplyAngularVelocity(v2, TIM_gf_dt);
    }
    if (v9)
        OBJ_ComputeLocalWhenHie(a1);
}

int DYN_MainCall(struct sWorld* a1)
{
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    tGameObjectMain* v4;
    tGameObjectExtended* v5;
    tGameObjectCollMain* v6;
    tColInstance* v7;
    tGameObjectBasic* v8;
    int v9;
    tGaoMatrix* v10;
    tGameObjectMain* v11;
    tGameObjectBasic* v12;
    tDyn* v13;
    int result;
    float i;
    tableElemStructPF* v16;

    v2 = a1->eot.dynamicObjectsTablePF.firstElemPtr;
    v16 = ((tableElemStructPF*)(a1->eot.dynamicObjectsTablePF.nextElemPtr)) - 1;
    v3 = v16;
    for (i = TIM_gf_dt; v2 <= v3; ++v2)
    {
        v4 = (tGameObjectMain*)v2->objectPtr;
        if (v2->objectPtr && ((uint8_t)v4 & 3) != 1)
        {
            TIM_gf_dt = i;
            if (ENG_gb_FirstFrame)
            {
                if ((v4->flags & 0x600) != 0)
                {
                    v5 = v4->extended;
                    if (v5)
                    {
                        v6 = v5->coll;
                        if (v6)
                        {
                            v7 = v6->colInstance;
                            if (v7)
                            {
                                if ((v4->flags & 0x400000) != 0)
                                    MATH_CopyMatrix(v7->mtx, &v4->basic->mtx->mtx);
                                else
                                    MATH_CopyMatrix(v7->mtx, v4->matrices);
                            }
                        }
                    }
                }
            }
            if (!OBJ_b_HasBeenProcess(v4, a1->dynCallCount, 64))
            {
                v8 = v4->basic;
                v9 = 0;
                if (v8)
                {
                    while (1)
                    {
                        v10 = v8->mtx;
                        if (!v10)
                            break;
                        v11 = v10->refGao2;
                        if (!v11 || OBJ_b_HasBeenProcess(v11, a1->dynCallCount, 64))
                            break;
                        v4 = v4->basic->mtx->refGao2;
                        v8 = v4->basic;
                        v9 = 1;
                        if (!v8)
                            goto LABEL_20;
                    }
                    if (!v9)
                        goto LABEL_21;
                LABEL_20:
                    --v2;
                }
            LABEL_21:
                OBJ_SetProcess(v4, a1->dynCallCount, 64);
                v12 = v4->basic;
                if (v12)
                {
                    v13 = v12->dyn;
                    if (v13)
                    {
                        if ((v13->flags & 0x20000000) == 0 && (v4->flags2 & 0x4400000) == 0)
                            DYN_OneCall(v4);
                    }
                }
                v3 = v16;
            }
        }
    }
    result = a1->dynCallCount + 1;
    a1->dynCallCount = result;
    return result;
}

void DYN_SetSpeedVector_WithoutChangingGravity(struct sDyn* dyn, Vec3* speedVec)
{
    float dp;
    Vec3 tmp, gravNorm;

    if ((dyn->flags & 0x40004) == 0x4 && !MATH_b_NulVectorWithEpsilon(&dyn->forces->gravityVector, 0.0001f))
    {
        MATH_NormalizeVector(&gravNorm, &dyn->forces->gravityVector);

        dp = MATH_DotProduct(&gravNorm, speedVec);
        tmp.x = speedVec->x - gravNorm.x * dp;
        tmp.y = speedVec->y - gravNorm.y * dp;
        tmp.z = speedVec->z - gravNorm.z * dp;
    }
    else
    {
        tmp = *speedVec;
    }
    
    MATH_VecMultNormScalar(&dyn->forceVector, &tmp, dyn->mass);
}

void DYN_InitWithDefaults(tDyn* dyn)
{
    dyn->mass = 1.0f;
    dyn->forceVector = MATH_gst_NulVector;
    dyn->speedLimit = FLT_MAX; 
    dyn->speedLimitVertical = FLT_MAX;
    dyn->maxStepSize = -1.0f;
    dyn->flags &= ~0x1200;
    if ((dyn->flags & 0x4) != 0)
        DYN_Forces_Init(dyn->forces, &MATH_gst_NulVector, &MATH_gst_NulVector, &MATH_gst_NulVector, 1.0f);
    if ((dyn->flags & 0x10000) != 0)
        DYN_SolidInitWithDefaults(dyn->solid);
    if ((dyn->flags & 0x20000) != 0)
        DYN_ColReinit(dyn->collision);
    if ((dyn->flags & 0x200000) != 0)
        DYN_ConstraintReinit(dyn->constraint);
    dyn->flags &= ~0xc00000;
}

int DYN_b_SpeedReachInTime(struct sDyn* a1, float* a2, Vec3* a3, Vec3* a4, float a5)
{
    double v6; // st7
    double v7; // st6
    double v8; // st5
    int result; // eax
    long double v10; // st4
    double v11; // st7
    float v12; // [esp+8h] [ebp-4h]
    float v13; // [esp+10h] [ebp+4h]

    v6 = a1->mass;
    v12 = a1->mass;
    v7 = v6 * a1->forceVector.x;
    v8 = a1->forceVector.y;
    result = 0;
    v13 = (float)sqrt(a4->x * a4->x + a4->y * a4->y + a4->z * a4->z);

    /*
      *a2 = -(__FYL2X__(
            fabs(
              v13
            / (sqrt(a1->speedVector.z * v6 * (a1->speedVector.z * v6) + v8 * v6 * (v8 * v6) + v7 * v7) - v13)
            * -0.090909094),
            0.69314718055994528623)
        * (1.0
         / (v12
          * a5)));
    */

    // fyl2x => (ST(1) ? log2 ST(0)) 
    // ln => c: log
    // log2 => c: log2
    // log10 => c: log10

    *a2 = (float)(-((log(2) * log2(fabs(v13 / (sqrt(a1->forceVector.z * v6 * (a1->forceVector.z * v6) + v8 * v6 * (v8 * v6) + v7 * v7) - v13) * -0.090909094))) * (1.0 / (v12 * a5))));

    v10 = sqrt(a4->z * a4->z + a4->y * a4->y + a4->x * a4->x);
    a3->x = (float)(1.0 / v10 * a4->x);                    // MATH_NormalizeVector
    a3->y = (float)(1.0 / v10 * a4->y);
    a3->z = (float)(1.0 / v10 * a4->z);
    v11 = v13 * *a2 * v12;
    a3->x = (float)(v11 * a3->x);
    a3->y = (float)(v11 * a3->y);
    a3->z = (float)(v11 * a3->z);
    if ((a1->flags & 4) != 0)
    {
        a1->forces->frictionVector.x = *a2;
        a1->forces->tractionVector = *a3;
        result = 1;
    }
    return result;
}

void DYN_ConstraintInit(struct sDynConstraint* constraint, char type, Vec3* point1, Vec3* point2, Vec3* parabolO, float parabolRadius)
{
    constraint->constraintType = type;
    constraint->point1 = *point1;
    constraint->point2 = *point2;
    constraint->parabolO = *parabolO;
    constraint->parabolRadius = parabolRadius;
}

