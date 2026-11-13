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

void DYN_CalcSlideVector(Vec3* slideVec, Vec3* normal, Vec3* speed, float scale)
{
    MATH_AddScaleVector(slideVec, speed, normal, -MATH_DotProduct(normal, speed));
    MATH_VecMultScalar(slideVec, slideVec, scale);
}

void DYN_CalcReboundVector(Vec3* reboundVec, Vec3* normal, Vec3* speed, float scale)
{
    MATH_VecMultScalar(reboundVec, normal, -MATH_DotProduct(normal, speed));
    MATH_VecMultScalar(reboundVec, reboundVec, scale);
}

void DYN_SetSpeedVector(struct sDyn* dyn, Vec3* speed)
{
    MATH_VecMultScalar(&dyn->forceVector, speed, (1.0f / dyn->mass));
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

void DYN_ColInit(tDynCol* col, float rebound, float slide, float reboundCos, float slideCos)
{
    if (col)
    {
        col->reboundHorizCosAngle = reboundCos;
        col->slideHorizCosAngle = slideCos;
        col->rebound = rebound;
        col->slide = slide;
    }
}

void DYN_ColReinit(tDynCol* col)
{
    DYN_ColInit(col, 0.0f, 1.0f, 1.0f, 1.0f);
}

void DYN_ColClose(void* col)
{
    MEM_Free(col);
}

tDynConstraint* DYN_pst_ConstraintCreate()
{
    tDynConstraint* constraint;

    constraint = (tDynConstraint*)MEM_p_Alloc(sizeof(tDynConstraint));
    memset(constraint, 0, sizeof(tDynConstraint));
    constraint->tangentVec = MATH_gst_NulVector;
    return constraint;
}

void DYN_ConstraintReinit(tDynConstraint* constraint)
{
    constraint->constraintType = 0;
    constraint->tangentVec = MATH_gst_NulVector;
}

void DYN_ConstraintClose(void* constraint)
{
    MEM_Free(constraint);
}

void DYN_ChangeDynFlags(struct sDyn* dyn, int newFlags, int oldFlags)
{
    int flags;
    tDynForces* forces;
    tDynSolid* solid;
    tDynCol* col;
    tDynConstraint* constraint;
    Vec3* pos;

    if ((newFlags & 2) + (newFlags & 4) + (newFlags & 8))
        flags = newFlags & ~1u;
    else
        flags = newFlags | 1;
    if ((flags & 4) != 0)
    {
        if ((oldFlags & 4) == 0)
        {
            forces = DYN_pst_Forces_Create();
            dyn->forces = forces;
            DYN_Forces_Init(forces, &MATH_gst_NulVector, &MATH_gst_NulVector, &MATH_gst_NulVector, 1.0);
            flags &= ~2u;
        }
    }
    else if ((oldFlags & 4) != 0)
    {
        DYN_Forces_Close(dyn->forces);               // DYN_Forces_Close
        dyn->forces = 0;
    }
    if ((flags & 0x10000) != 0)
    {
        if ((oldFlags & 0x10000) == 0)
        {
            solid = DYN_pst_SolidCreate();
            pos = dyn->vptr;
            dyn->solid = solid;
            DYN_SolidInit(solid, &MATH_gst_NulVector, &MATH_gst_NulVector, pos, 3.402822e38f);
        }
    }
    else if ((oldFlags & 0x10000) != 0)
    {
        DYN_SolidClose(dyn->solid);
        dyn->solid = 0;
    }
    if ((flags & 0x20000) != 0)
    {
        if ((oldFlags & 0x20000) == 0)
        {
            col = DYN_pst_ColCreate();
            dyn->collision = col;
            DYN_ColReinit(col);
        }
    }
    else if ((oldFlags & 0x20000) != 0)
    {
        DYN_ColClose(dyn->collision);
        dyn->collision = 0;
    }
    if ((flags & 0x200000) != 0)
    {
        if ((oldFlags & 0x200000) == 0)
        {
            constraint = DYN_pst_ConstraintCreate();
            dyn->constraint = constraint;
            DYN_ConstraintReinit(constraint);
        }
    }
    else if ((oldFlags & 0x200000) != 0)
    {
        DYN_ConstraintClose(dyn->constraint);
        dyn->constraint = 0;
    }
    dyn->flags = flags;
}

void DYN_Dyna_Init(tDyn* dyn, int flags, float mmass, float speedLimit, float speedLimitVertical, Vec3* speedVec, Vec3* pos)
{
    double mass;
    int oldFlags;

    mass = 1.0 / mmass;
    dyn->mass = (float)mass;
    dyn->forceVector.x = (float)(1.0 / mass * speedVec->x);
    dyn->forceVector.y = (float)(speedVec->y * (1.0 / mass));
    dyn->forceVector.z = (float)(speedVec->z * (1.0 / mass));
    dyn->vptr = pos;
    oldFlags = dyn->flags;
    dyn->speedLimit = speedLimit;
    dyn->speedLimitVertical = speedLimitVertical;
    DYN_ChangeDynFlags(dyn, flags, oldFlags);
    dyn->flags &= ~0xC00000u;
}

float DYN_f_FindFrictionForMaxSpeed(struct sDyn* dyn, float maxSpeed)
{
    tDynForces* forces;

    forces = dyn->forces;
    return MATH_VecLen(&forces->tractionVector) / maxSpeed;
}

void DYN_ApplyHorizontalSpeed(struct sDyn* dyn, float speed)
{
    long double speedLen;
    long double scale;

    speedLen = sqrt(
        dyn->forceVector.x * dyn->forceVector.x
        + dyn->forceVector.y * dyn->forceVector.y
        + dyn->forceVector.z * dyn->forceVector.z);
    if (speedLen != 0.0)
    {
        scale = 1.0 / dyn->mass * speed / speedLen;
        dyn->forceVector.x = (float)(scale * dyn->forceVector.x);
        dyn->forceVector.y = (float)(scale * dyn->forceVector.y);
        dyn->forceVector.z = (float)(scale * dyn->forceVector.z);
    }
}

void DYN_ConstraintToSegment(tDyn* dyn, tDynConstraint* constraint, Vec3* segEnd)
{
    tDynConstraint* dynConstraint;
    long double len;
    Vec3 projected;

    dynConstraint = dyn->constraint;
    MATH_PointSegmentProjection(&projected, dyn->vptr, &constraint->point1, segEnd);
    *dyn->vptr = projected;
    dynConstraint->tangentVec.x = segEnd->x - constraint->point1.x;        // MATH_NormalizeEqualVector inline
    dynConstraint->tangentVec.y = segEnd->y - constraint->point1.y;
    dynConstraint->tangentVec.z = segEnd->z - constraint->point1.z;
    len = sqrt(dynConstraint->tangentVec.x * dynConstraint->tangentVec.x + dynConstraint->tangentVec.y * dynConstraint->tangentVec.y + dynConstraint->tangentVec.z
        * dynConstraint->tangentVec.z);
    dynConstraint->tangentVec.x = (float)(1.0 / len * dynConstraint->tangentVec.x);
    dynConstraint->tangentVec.y = (float)(1.0 / len * dynConstraint->tangentVec.y);
    dynConstraint->tangentVec.z = (float)(1.0 / len * dynConstraint->tangentVec.z);
}

void DYN_ConstraintToParabol(tDyn* dyn, tDynConstraint* constraint, Vec3* refB, Vec3* origin, float time)
{
    Vec3* tangent;
    long double len;
    Vec3 projected;

    tangent = &dyn->constraint->tangentVec;
    MATH_PointParabolProjection3D(&projected, tangent, dyn->vptr, &constraint->point1, refB, origin, time);
    *dyn->vptr = projected;                              // MATH_NormalizeEqualVector inline
    len = sqrt(tangent->x * tangent->x + tangent->y * tangent->y + tangent->z * tangent->z);
    tangent->x = (float)(1.0 / len * tangent->x);
    tangent->y = (float)(1.0 / len * tangent->y);
    tangent->z = (float)(1.0 / len * tangent->z);
}

void DYN_ApplyConstraint(tDyn* dyn)
{
    tDynConstraint* constraint;
    uint8_t type;
    double invMass;
    double along;
    double mass;
    float speedX;
    float speedY;
    float alongY;
    float speedZ;
    float alongZ;

    constraint = dyn->constraint;
    type = constraint->constraintType;
    if (type == 1)
    {
        DYN_ConstraintToSegment(dyn, constraint, &constraint->point2);
    }
    else if (type == 2)
    {
        DYN_ConstraintToParabol(dyn, constraint, &constraint->point2, &constraint->parabolO, constraint->parabolRadius);
    }
    if (constraint->constraintType)
    {
        invMass = dyn->mass;
        speedX = (float)(invMass * dyn->forceVector.x);
        if (fabs(speedX) > 0.0001// MATH_b_NulVectorWithEpsilon inline
            || fabs(dyn->forceVector.y * invMass) > 0.0001
            || fabs(dyn->forceVector.z * invMass) > 0.0001)
        {
            speedY = (float)(dyn->forceVector.y * invMass);
            speedZ = (float)(dyn->forceVector.z * invMass);
            along = constraint->tangentVec.z * speedZ + constraint->tangentVec.y * speedY + speedX * constraint->tangentVec.x;
            alongY = (float)(constraint->tangentVec.y * along);
            alongZ = (float)(constraint->tangentVec.z * along);
            mass = 1.0 / dyn->mass;
            dyn->forceVector.x = (float)(mass * (along * constraint->tangentVec.x));
            dyn->forceVector.y = (float)(alongY * mass);
            dyn->forceVector.z = (float)(alongZ * mass);
        }
    }
}

void DYN_SolidInitWithDefaults(tDynSolid* solid)
{
    DYN_SolidInit(solid, &MATH_gst_NulVector, &MATH_gst_NulVector, &MATH_gst_NulVector, 3.402822e38f);
}

void DYN_CalcRotationSpeed(tDyn* dyn, float dt)
{
    tDynSolid* solid;
    Vec3* torque;
    double scale;
    float addY;
    float addZ;

    solid = dyn->solid;
    torque = &solid->torqueVector;
    scale = dyn->mass * dt;
    addY = (float)(torque->y * scale);
    addZ = (float)(torque->z * scale);
    solid->rotSpeedVector.x = (float)(scale * torque->x + solid->rotSpeedVector.x);
    solid->rotSpeedVector.y = addY + solid->rotSpeedVector.y;
    solid->rotSpeedVector.z = addZ + solid->rotSpeedVector.z;
}

void DYN_ApplyRotationSpeedLimit(tDyn* dyn)
{
    tDynSolid* solid;
    long double speedLen;
    float limit;

    solid = dyn->solid;
    speedLen = sqrt(
        solid->rotSpeedVector.x * solid->rotSpeedVector.x
        + solid->rotSpeedVector.y * solid->rotSpeedVector.y
        + solid->rotSpeedVector.z * solid->rotSpeedVector.z);
    limit = solid->rotSpeedLimit;
    if (speedLen > limit)
        MATH_SetNormVector(&solid->rotSpeedVector, &solid->rotSpeedVector, limit);
}

void DYN_MakeWMatrix(Mtx4x4* mtx, Vec3* rotSpeed, float dt)
{
    double wx;
    double wy;
    double wz;
    int flags;
    double row1x;
    long double len1;
    long double len2;
    long double len3;
    float wzF;
    float negWyF;

    wx = dt * rotSpeed->x;                              // MATH_SetIdentityMatrix
    wy = rotSpeed->y * dt;
    wz = rotSpeed->z * dt;
    mtx->v2.x = 0.0;
    mtx->v2.z = 0.0;
    wzF = (float)wz;
    mtx->v3.x = 0.0;
    mtx->v3.y = 0.0;
    mtx->v1.x = 1.0;
    negWyF = (float)-wy;
    mtx->v1.y = 0.0;
    mtx->v1.z = 0.0;
    mtx->_m14 = 0.0;
    mtx->v2.x = -wzF;
    mtx->_m24 = 0.0;
    mtx->_m34 = 0.0;
    mtx->v2.z = (float)wx;
    mtx->Pos.x = 0.0;
    mtx->Pos.y = 0.0;
    mtx->v3.x = (float)wy;
    mtx->Pos.z = 0.0;
    flags = mtx->mtxflag;
    mtx->v2.y = 1.0;
    mtx->v3.y = (float)-wx;
    mtx->v3.z = 1.0;
    row1x = mtx->v1.x;
    mtx->w = 1.0;
    mtx->mtxflag = flags & 2;
    len1 = sqrt(negWyF * negWyF + row1x * row1x + wzF * wzF);   // MATH_NormalizeEqualVector x3
    mtx->v1.x = (float)(1.0 / len1 * mtx->v1.x);
    mtx->v1.y = (float)(wzF * (1.0 / len1));
    mtx->v1.z = (float)(negWyF * (1.0 / len1));
    len2 = sqrt(mtx->v2.x * mtx->v2.x + mtx->v2.y * mtx->v2.y + mtx->v2.z * mtx->v2.z);
    mtx->v2.x = (float)(1.0 / len2 * mtx->v2.x);
    mtx->v2.y = (float)(1.0 / len2 * mtx->v2.y);
    mtx->v2.z = (float)(1.0 / len2 * mtx->v2.z);
    len3 = sqrt(mtx->v3.x * mtx->v3.x + mtx->v3.y * mtx->v3.y + mtx->v3.z * mtx->v3.z);
    mtx->v3.x = (float)(1.0 / len3 * mtx->v3.x);
    mtx->v3.y = (float)(1.0 / len3 * mtx->v3.y);
    mtx->v3.z = (float)(1.0 / len3 * mtx->v3.z);
}

void DYN_ApplyAngularVelocity(tDyn* dyn, float dt)
{
    Mtx4x4* mtx;
    Mtx4x4 product;
    Mtx4x4 wMtx;

    DYN_MakeWMatrix(&wMtx, &dyn->solid->rotSpeedVector, dt);
    mtx = (Mtx4x4*)&dyn->vptr[-4]; // ez egy rettentõ igénytelen megoldás ohgy egy pozícióbol (mátrix 4. sora) haogyan kapjuk vissza az egész mátrixot, és ez egyben azt is feltételezi
                                 // hogy ne egy egyedüöli vektor lett megadva, hanem a mátrix adott része (&mtx.Pos) A vptr a DYN_Dyna_Init-ben állítódik be ami két helyrõl van hívva, 
                                 // és mindkét helyen egy mátrix adott részét kapja. Ez a szerencse
    MATH_MulMatrixMatrix(&product, &wMtx, mtx);
    MATH_CopyMatrix(mtx, &product);
    MATH_Orthonormalize(mtx);
}

tDyn* DYN_pst_Dyna_Create()
{
    tDyn* dyn;

    dyn = (tDyn*)MEM_p_Alloc(sizeof(tDyn));
    LOA_ul_FileTypeSize_37 += sizeof(tDyn);
    memset(dyn, 0, sizeof(tDyn));
    return dyn;
}

void DYN_Dyna_Close(tDyn* dyn)
{
    if ((dyn->flags & 4) != 0)
        DYN_Forces_Close(dyn->forces);
    if ((dyn->flags & 0x10000) != 0)
        DYN_SolidClose(dyn->solid);
    if ((dyn->flags & 0x20000) != 0)
        DYN_ColClose(dyn->collision);
    if ((dyn->flags & 0x200000) != 0)
        DYN_ConstraintClose(dyn->constraint);
    MEM_Free(dyn);
}

void DYN_SetHorizontalSpeedComponent(struct sDyn* dyn, Vec3* speed)
{
    tDynForces* forces;
    double mass;
    double sz;
    double invMass;
    long double invGravLen;
    long double gxN;
    long double along;
    float gx;
    Vec3 newSpeed;
    float gy;
    float gyN;
    float gz;
    float gzN;
    float sx;
    float sy;
    float alongZ;
    float alongF;
    BOOL projectOut;

    projectOut = FALSE;
    if ((dyn->flags & 0x40004) == 4)
    {
        forces = dyn->forces;
        gx = forces->gravityVector.x;
        gz = forces->gravityVector.z;
        gy = forces->gravityVector.y;
        mass = 1.0 / (1.0 / dyn->mass);
        sx = (float)(mass * dyn->forceVector.x);
        sy = (float)(dyn->forceVector.y * mass);
        sz = (float)(mass * dyn->forceVector.z);
        if (forces->gravityVector.x != 0.0 || gy != 0.0 || gz != 0.0)
            projectOut = TRUE;
    }
    if (projectOut)
    {
        invGravLen = 1.0 / sqrt(gz * gz + gy * gy + gx * gx);
        gyN = (float)(invGravLen * gy);
        gzN = (float)(invGravLen * gz);
        gxN = gx * invGravLen;
        along = sz * gzN + sy * gyN + sx * gxN;
        alongF = (float)along;
        alongZ = alongF * gzN;
        newSpeed.x = (float)(along * gxN + speed->x);
        newSpeed.y = speed->y + alongF * gyN;
        newSpeed.z = speed->z + alongZ;
    }
    else
    {
        newSpeed = *speed;
    }
    invMass = 1.0 / dyn->mass;
    dyn->forceVector.x = (float)(newSpeed.x * invMass);
    dyn->forceVector.y = (float)(newSpeed.y * invMass);
    dyn->forceVector.z = (float)(newSpeed.z * invMass);
}



void DYN_CalculateSpeedWhenFrictionVector(tDyn* dyn, float dt)
{
    int axes;
    tDynForces* forces;
    long double frictionX;
    Vec3* friction;
    double fx;
    double log2DecayX;
    double roundedX;
    double fy;
    double log2DecayY;
    double roundedY;
    double fz;
    double log2DecayZ;
    double roundedZ;
    Vec3 localForce;
    Vec3 localMomentum;

    axes = 0;
    MATH_VectorGlobalToLocal(&localMomentum, (Mtx4x4*)&dyn->vptr[-4], &dyn->momentum); // a vptr[-4] egy gányolás, mivel az egész mátrix kell, lásd másik megjegyzést ezzel kapcs...
    MATH_VectorGlobalToLocal(&localForce, (Mtx4x4*)&dyn->vptr[-4], &dyn->forceVector);
    forces = dyn->forces;
    frictionX = forces->frictionVector.x;
    friction = &forces->frictionVector;
    if (fabs(frictionX) >= 0.0000001)
        axes = 1;
    else
        localForce.x = localMomentum.x * dt + localForce.x;
    if (fabs(friction->y) >= 0.0000001)
        axes += 2;
    else
        localForce.y = localMomentum.y * dt + localForce.y;
    if (fabs(friction->z) >= 0.0000001)
        axes += 4;
    else
        localForce.z = localMomentum.z * dt + localForce.z;
    if (axes)
    {
        /* Each axis decays by 2 to the power of -friction*mass*dt. The
           integer part of that exponent is rounded off and then never
           applied: the original sequence scaled by it with FSCALE, and
           the decompilation kept only the F2XM1 of the fraction. Left
           as written. */
        if ((axes & 1) != 0)
        {
            fx = dyn->mass * friction->x;
            log2DecayX = -(fx * dt) * 1.442695040888963387;
            //_ST4 = log2DecayX;
            //__asm { frndint }
            roundedX = (float)((int)(log2DecayX + 0.5f));
            //localForce.x = 1.0 / fx * localMomentum.x - (__F2XM1__(log2DecayX - _ST4) + 1.0) * (1.0 / fx * localMomentum.x - localForce.x);
            localForce.x = (float)(1.0 / fx * localMomentum.x - ((pow(2, (log2DecayX - roundedX)) - 1.0) + 1.0) * (1.0 / fx * localMomentum.x - localForce.x));
        }
        if ((axes & 2) != 0)
        {
            fy = dyn->mass * friction->y;
            log2DecayY = -(fy * dt) * 1.442695040888963387;
            //_ST4 = log2DecayY;
            //__asm { frndint }
            roundedY = (float)((int)(log2DecayY + 0.5f));
            //localForce.y = 1.0 / fy * localMomentum.y - (__F2XM1__(log2DecayY - _ST4) + 1.0) * (1.0 / fy * localMomentum.y - localForce.y);
            localForce.y = (float)(1.0 / fy * localMomentum.y - ((pow(2, (log2DecayY - roundedY)) - 1.0) + 1.0) * (1.0 / fy * localMomentum.y - localForce.y));
        }
        if ((axes & 4) != 0)
        {
            fz = dyn->mass * friction->z;
            log2DecayZ = -(fz * dt) * 1.442695040888963387;
            //_ST4 = log2DecayZ;
            //__asm { frndint }
            roundedZ = (float)((int)(log2DecayZ + 0.5f));
            //localForce.z = 1.0 / fz * localMomentum.z - (__F2XM1__(log2DecayZ - _ST4) + 1.0) * (1.0 / fz * localMomentum.z - localForce.z);
            localForce.z = (float)(1.0 / fz * localMomentum.z - ((pow(2, (log2DecayZ - roundedZ)) - 1.0) + 1.0) * (1.0 / fz * localMomentum.z - localForce.z));
        }
    }
    MATH_TransformVectorNoScale(&dyn->forceVector, (Mtx4x4*)&dyn->vptr[-4], &localForce);
}

static void DYN_l_ApplyFrictionVector(tDyn* dyn, float dt, int flags)
{
    tDynForces* forces;
    double mass;
    float frictionX;
    Vec3* friction;
    double addX;
    double addY;
    double srcZ;
    double fx;
    double termZ;
    double diffY;
    double log2Decay;
    float rounded;
    long double decay;
    Vec3 accel;
    float deltaX;
    float gyTerm;
    float gzTerm;
    float diffZ;
    float deltaZ;
    float addZ;
    float termX;
    float termY;
    float decayF;

    if ((flags & 4) != 0)
    {
        if ((flags & 0x100000) != 0)
        {
            accel.x = MATH_gst_NulVector.x;
            accel.y = MATH_gst_NulVector.y;
            accel.z = MATH_gst_NulVector.z;
        }
        else
        {
            forces = dyn->forces;
            accel.x = forces->streamVec.x;
            accel.z = forces->streamVec.z;
            accel.y = forces->streamVec.y;
        }
        if ((flags & 0x80000) == 0)
        {
            forces = dyn->forces;
            accel.x = accel.x + forces->tractionVector.x;
            accel.y = accel.y + forces->tractionVector.y;
            accel.z = accel.z + forces->tractionVector.z;
        }
        if ((flags & 0x40000) == 0)
        {
            forces = dyn->forces;
            mass = 1.0 / dyn->mass;
            gyTerm = (float)(forces->gravityVector.y * mass);
            gzTerm = (float)(forces->gravityVector.z * mass);
            accel.x = (float)(accel.x + mass * forces->gravityVector.x);
            accel.y = accel.y + gyTerm;
            accel.z = accel.z + gzTerm;
        }
        dyn->momentum = accel;
        forces = dyn->forces;
        if ((flags & 0x1000) != 0)
        {
            frictionX = forces->frictionVector.x;
            friction = &forces->frictionVector;
            if (fabs(frictionX) > 0.0001// MATH_b_NulVectorWithEpsilon inline
                || fabs(friction->y) > 0.0001
                || fabs(friction->z) > 0.0001)
            {
                DYN_CalculateSpeedWhenFrictionVector(dyn, dt);
                return;
            }
        }
        else if (fabs(forces->frictionVector.x) >= 0.000099999997)
        {
            fx = forces->frictionVector.x * dyn->mass;
            termX = (float)(accel.x * (1.0 / fx));
            termY = (float)(accel.y * (1.0 / fx));
            termZ = 1.0 / fx * accel.z;
            diffY = termY - dyn->forceVector.y;
            diffZ = (float)(termZ - dyn->forceVector.z);
            log2Decay = -(fx * dt) * 1.442695040888963387;
            //_ST2 = log2Decay;
            //__asm { frndint }
            rounded = (float)((int)(log2Decay + 0.5f));
            //decay = __F2XM1__(log2Decay - _ST2) + 1.0;
            decay = (pow(2, (log2Decay - rounded)) - 1.0) + 1.0; // TODO FIXME, jó ez így?
            decayF = (float)decay;
            deltaX = (float)(decay * (termX - dyn->forceVector.x));
            deltaZ = diffZ * decayF;
            dyn->forceVector.x = termX - deltaX;
            dyn->forceVector.y = (float)(termY - decayF * diffY);
            dyn->forceVector.z = (float)(termZ - deltaZ);
            return;
        }
        addX = accel.x * dt;
        addY = accel.y * dt;
        srcZ = accel.z;
    }
    else
    {
        if ((flags & 2) == 0)
            return;
        addX = dt * dyn->momentum.x;
        addY = dyn->momentum.y * dt;
        srcZ = dyn->momentum.z;
    }
    addZ = (float)(srcZ * dt);
    dyn->forceVector.x = (float)(addX + dyn->forceVector.x);
    dyn->forceVector.y = (float)(addY + dyn->forceVector.y);
    dyn->forceVector.z = addZ + dyn->forceVector.z;
}

void DYN_ApplySpeedLimit(struct sDyn* dyn)
{
    double invMass;
    double speedX;
    double speedY;
    tDynForces* forces;
    double forceZ;
    float gy;
    float gz;
    char horizClamped;
    long double invGravLen;
    long double gxN;
    long double gzN;
    double horizZ;
    double sumY;
    double mass;
    long double speedLen;
    float limit;
    float vertLimit;
    Vec3 gravDir;
    Vec3 horiz;
    Vec3 alongVec;
    float speedYF;
    float speedZ;
    float along;
    float alongAbs;
    float massF;
    float clamped;

    limit = dyn->speedLimit;
    vertLimit = dyn->speedLimitVertical;
    invMass = dyn->mass;
    speedX = invMass * dyn->forceVector.x;
    speedY = dyn->forceVector.y * invMass;
    if (limit == vertLimit)
    {
        speedLen = sqrt(dyn->forceVector.z * invMass * (dyn->forceVector.z * invMass) + speedY * speedY + speedX * speedX);
        clamped = (float)speedLen;
        if (speedLen > limit)
            clamped = limit;
        DYN_ApplyHorizontalSpeed(dyn, clamped);
    }
    else
    {
        speedYF = (float)speedY;
        forces = dyn->forces;
        forceZ = dyn->forceVector.z;
        gravDir.x = forces->gravityVector.x;
        gy = forces->gravityVector.y;
        gz = forces->gravityVector.z;
        speedZ = (float)(forceZ * invMass);
        horizClamped = 0;
        invGravLen = 1.0 / sqrt(gz * gz + gy * gy + gravDir.x * gravDir.x);
        gxN = gravDir.x * invGravLen;
        gravDir.y = (float)(gy * invGravLen);
        gzN = gz * invGravLen;
        gravDir.z = (float)gzN;
        along = (float)(gzN * speedZ + gravDir.y * speedYF + gxN * speedX);
        alongVec.x = (float)(gxN * along);
        alongVec.y = gravDir.y * along;
        alongVec.z = gravDir.z * along;
        alongAbs = (float)fabs(along);
        horiz.x = (float)(speedX - alongVec.x);
        horiz.y = speedYF - alongVec.y;
        horizZ = speedZ - alongVec.z;
        horiz.z = (float)horizZ;
        if (sqrt(horizZ * horiz.z + horiz.y * horiz.y + horiz.x * horiz.x) > limit)
        {
            MATH_SetNormVector(&horiz, &horiz, limit);
            horizClamped = 1;
        }
        if (alongAbs <= (double)vertLimit)
        {
            if (!horizClamped)
                return;
        }
        else
        {
            MATH_SetNormVector(&alongVec, &alongVec, vertLimit);
        }
        sumY = horiz.y + alongVec.y;
        speedZ = horiz.z + alongVec.z;
        mass = 1.0 / dyn->mass;
        massF = (float)mass;
        dyn->forceVector.x = (float)(mass * (horiz.x + alongVec.x));
        dyn->forceVector.y = (float)(sumY * massF);
        dyn->forceVector.z = speedZ * massF;
    }
}

static void DYN_l_ApplySpeed(tGameObjectMain* gao, tDyn* dyn, float dt, int flags, Vec3* otherSpeed)
{
    Vec3* pos;
    double invMass;
    double speedX;
    float posZ;
    double forceY;
    double sumX;
    double sumY;
    tDynConstraint* constraint;
    double along;
    Vec3* curPos;
    float maxX;
    tGameObjectBasic* basic;
    tGaoMatrix* gaoMtx;
    tGameObjectMain* refGao;
    Mtx4x4* absMtx;
    tGameObjectBasic* basic2;
    Mtx4x4* flashMtx;
    tGameObjectBasic* basic3;
    tGameObjectActorObject* actor;
    Vec3 delta;
    Vec3 avg;
    float oldX;
    float oldY;
    float oldZ;
    Mtx4x4 invMtx;

    pos = dyn->vptr;
    invMass = dyn->mass;
    speedX = invMass * dyn->forceVector.x;
    oldX = pos->x;
    oldY = pos->y;
    posZ = pos->z;
    delta.x = (float)speedX;
    forceY = dyn->forceVector.y;
    oldZ = posZ;
    delta.y = (float)(forceY * invMass);
    delta.z = (float)(dyn->forceVector.z * invMass);
    if (otherSpeed)
    {
        sumX = delta.x + otherSpeed->x;
        sumY = otherSpeed->y + delta.y;
        avg.z = otherSpeed->z + delta.z;
        avg.x = (float)(sumX * 0.5);
        avg.y = (float)(sumY * 0.5);
        avg.z = (float)(avg.z * 0.5);
    }
    else
    {
        avg = delta;
    }
    if ((flags & 0x10000000) != 0 && dyn->forces)
    {
        delta = avg;
    }
    else
    {
        delta.x = avg.x * dt;
        delta.y = avg.y * dt;
        delta.z = avg.z * dt;
    }
    if ((flags & 0x200000) != 0)
    {
        constraint = dyn->constraint;
        if (fabs(constraint->tangentVec.x) > 0.0001// MATH_b_NulVectorWithEpsilon inline
            || fabs(constraint->tangentVec.y) > 0.0001
            || fabs(constraint->tangentVec.z) > 0.0001)
        {
            along = constraint->tangentVec.z * delta.z + constraint->tangentVec.y * delta.y + delta.x * constraint->tangentVec.x;
            delta.x = (float)(along * constraint->tangentVec.x);
            delta.y = (float)(constraint->tangentVec.y * along);
            delta.z = (float)(constraint->tangentVec.z * along);
        }
        pos->x = delta.x + pos->x;
        pos->y = delta.y + pos->y;
        pos->z = delta.z + pos->z;
        DYN_ApplyConstraint(dyn);
    }
    else
    {
        pos->x = delta.x + pos->x;
        pos->y = delta.y + pos->y;
        pos->z = delta.z + pos->z;
        if ((dyn->flags & 0x4000) != 0)
        {
            curPos = dyn->vptr;
            if (delta.z * (dyn->maxPos.z - curPos->z) + (dyn->maxPos.y - curPos->y) * delta.y + (dyn->maxPos.x - curPos->x) * delta.x < 0.0)
            {
                maxX = dyn->maxPos.x;
                delta.x = maxX - oldX;
                delta.y = dyn->maxPos.y - oldY;
                delta.z = dyn->maxPos.z - oldZ;
                curPos->x = maxX;
                curPos->y = dyn->maxPos.y;
                curPos->z = dyn->maxPos.z;
            }
        }
    }
    if ((gao->flags & 0x400000) != 0)
    {
        basic = gao->basic;
        if (basic && (gaoMtx = basic->mtx) != 0)
            refGao = gaoMtx->refGao2;
        else
            refGao = 0;
        absMtx = OBJ_pst_GetAbsoluteMatrix(refGao);
        MATH_InvertMatrix(&invMtx, absMtx);
        MATH_TransformVector(&delta, &invMtx, &delta);
    }
    if ((gao->flags & 2) != 0)
    {
        basic2 = gao->basic;
        if (basic2)
        {
            if (basic2->actorObjectOrVisualObject)
            {
                if ((gao->flags & 0x4000000) != 0)
                {
                    flashMtx = OBJ_pst_GetFlashMatrix(gao);
                    MATH_VecAddVecWithoutBuffer(&flashMtx->Pos, &delta);
                }
                if (ANI_GaoAnimationIsTransitioning(gao))
                {
                    basic3 = gao->basic;
                    actor = (tGameObjectActorObject*)basic3->actorObjectOrVisualObject;
                    MATH_VecAddVecWithoutBuffer(&actor->animPtr[1]->flashMtx.Pos, &delta);
                    MATH_VecAddVecWithoutBuffer((Vec3*)&actor->animPtr[0]->flashMtx._m14, &delta);
                    MATH_VecAddVecWithoutBuffer(&actor->animPtr[0]->flashMtx.Pos, &delta);
                }
            }
        }
    }
}

static void DYN_l_AutoOrient(tGameObjectMain* gao, tDyn* dyn, int flags, float dt)
{
    Vec3* speed;
    int inertia;
    long double speedLen;
    double inertiaValue;
    double log2Decay;
    double rounded;
    float factor;
    Vec3 up;
    Vec3 speedDir;
    Vec3 target;
    Vec3 sight;

    speed = &dyn->forceVector;
    if (fabs(dyn->forceVector.x) > 0.01// MATH_b_NulVectorWithEpsilon inline
        || fabs(dyn->forceVector.y) > 0.01
        || fabs(dyn->forceVector.z) > 0.01)
    {
        inertia = flags & 0xC00;
        if ((flags & 0xC00) != 0)
        {                                           // MATH_NormalizeVector inline
            speedLen = sqrt(dyn->forceVector.z * dyn->forceVector.z + dyn->forceVector.y * dyn->forceVector.y + speed->x * speed->x);
            speedDir.x = (float)(1.0 / speedLen * speed->x);
            speedDir.y = (float)(1.0 / speedLen * dyn->forceVector.y);
            speedDir.z = (float)(1.0 / speedLen * dyn->forceVector.z);
            OBJ_SightGet(gao, &sight);
            if (inertia == 0x400)
            {
                inertiaValue = DYN_gf_AutoOrientLowInertiaValue;
            }
            else if (inertia == 0x800)
            {
                inertiaValue = DYN_gf_AutoOrientMediumInertiaValue;
            }
            else
            {
                inertiaValue = DYN_gf_AutoOrientHiInertiaValue;
            }
            /* Same missing scale as DYN_CalculateSpeedWhenFrictionVector:
               the integer part of the exponent is rounded off and then
               never applied. The TODO below is the user's own. */
            log2Decay = inertiaValue * dt * 1.442695040888963387;
            rounded = (float)((int)(log2Decay + 0.5f));
            //_ST6 = log2Decay;
            //__asm { frndint }
            //factor = __F2XM1__(log2Decay - _ST6) + 1.0;
            
            factor = (float)((pow(2, (log2Decay - rounded)) - 1.0) + 1.0); // TODO FIXME, jó ez így?
            MATH_InterpolateBetweenTwoPosWithFactor(&target, &sight, &speedDir, factor);
            if ((flags & 0x2000) != 0)
            {
                MATH_NormalizeVector(&up, &dyn->forces->gravityVector);
                MATH_VecInvert(&up, &up);
                if (MATH_b_VecColinear(&up, &target))
                    OBJ_BankingGeneralSet(gao, &MATH_gst_NulVector, &up);
                else
                    OBJ_BankingGeneralSet(gao, &target, &up);
            }
            else
            {
                OBJ_SightSet(gao, &target);
            }
        }
        else
        {
            if ((flags & 0x2000) != 0)
            {
                MATH_NormalizeVector(&up, &dyn->forces->gravityVector);
                MATH_VecInvert(&up, &up);
                if (!MATH_b_VecColinear(&up, speed))
                    OBJ_BankingGeneralSet(gao, speed, &up);
                else
                    OBJ_BankingGeneralSet(gao, &MATH_gst_NulVector, &up);
                return;
            }
            OBJ_SightSet(gao, &dyn->forceVector);
        }
    }
}

void DYN_OneCall(struct sGameObjectMain* gao)
{
    tDyn* dyn;
    int hieFlag;
    double invMass;
    int flags;
    BOOL noGravityFlag;
    tDynForces* forces;
    Vec3 speed;
    int hieFlagSaved;

    dyn = gao->basic->dyn;
    hieFlag = gao->flags & 0x400000;
    hieFlagSaved = hieFlag;
    if ((dyn->flags & 0x4000000) != 0)
        TIM_gf_dt = TIM_gf_dt / TIM_gf_SpeedFactor;
    if (hieFlag)
        OBJ_ComputeGlobalWhenHie(gao);
    invMass = dyn->mass;
    flags = dyn->flags;
    noGravityFlag = (dyn->flags & 1) == 0;
    speed.x = (float)(invMass * dyn->forceVector.x);
    speed.y = (float)(dyn->forceVector.y * invMass);
    speed.z = (float)(dyn->forceVector.z * invMass);
    if (noGravityFlag)
    {
        if ((flags & 0x10000000) != 0 && (forces = dyn->forces) != 0)
        {
            dyn->forceVector = forces->tractionVector;
        }
        else if ((flags & 0x1C0000) != 0x1C0000)
        {
            DYN_l_ApplyFrictionVector(dyn, TIM_gf_dt, flags);
        }
    }
    if ((flags & 0x10000000) == 0 || !dyn->forces)
        DYN_ApplySpeedLimit(dyn);
    if ((dyn->flags & 0x8000000) != 0 || (flags & 0x10000000) != 0 && dyn->forces)
        DYN_l_ApplySpeed(gao, dyn, TIM_gf_dt, flags, 0);
    else
        DYN_l_ApplySpeed(gao, dyn, TIM_gf_dt, flags, &speed);
    if ((flags & 0x200) != 0)
        DYN_l_AutoOrient(gao, dyn, flags, TIM_gf_dt);
    if ((flags & 0x10000) != 0)
    {
        DYN_CalcRotationSpeed(dyn, TIM_gf_dt);
        DYN_ApplyRotationSpeedLimit(dyn);
        DYN_ApplyAngularVelocity(dyn, TIM_gf_dt);
    }
    if (hieFlagSaved)
        OBJ_ComputeLocalWhenHie(gao);
}

int DYN_MainCall(struct sWorld* world)
{
    tableElemStructPF* elem;
    tableElemStructPF* lastElemBound;
    tGameObjectMain* gao;
    tGameObjectExtended* ext;
    tGameObjectCollMain* coll;
    tColInstance* colInst;
    tGameObjectBasic* basic;
    int climbed;
    tGaoMatrix* gaoMtx;
    tGameObjectMain* parent;
    tGameObjectBasic* basic2;
    tDyn* dyn;
    int result;
    float dt;
    tableElemStructPF* lastElem;

    elem = world->eot.dynamicObjectsTablePF.firstElemPtr;
    lastElem = ((tableElemStructPF*)(world->eot.dynamicObjectsTablePF.nextElemPtr)) - 1;
    lastElemBound = lastElem;
    for (dt = TIM_gf_dt; elem <= lastElemBound; ++elem)
    {
        gao = (tGameObjectMain*)elem->objectPtr;
        if (elem->objectPtr && ((uint8_t)gao & 3) != 1)
        {
            TIM_gf_dt = dt;
            if (ENG_gb_FirstFrame)
            {
                if ((gao->flags & 0x600) != 0)
                {
                    ext = gao->extended;
                    if (ext)
                    {
                        coll = ext->coll;
                        if (coll)
                        {
                            colInst = coll->colInstance;
                            if (colInst)
                            {
                                if ((gao->flags & 0x400000) != 0)
                                    MATH_CopyMatrix(colInst->mtx, &gao->basic->mtx->mtx);
                                else
                                    MATH_CopyMatrix(colInst->mtx, gao->matrices);
                            }
                        }
                    }
                }
            }
            if (!OBJ_b_HasBeenProcess(gao, world->dynCallCount, 64))
            {
                basic = gao->basic;
                climbed = 0;
                if (basic)
                {
                    while (1)
                    {
                        gaoMtx = basic->mtx;
                        if (!gaoMtx)
                            break;
                        parent = gaoMtx->refGao2;
                        if (!parent || OBJ_b_HasBeenProcess(parent, world->dynCallCount, 64))
                            break;
                        gao = gao->basic->mtx->refGao2;
                        basic = gao->basic;
                        climbed = 1;
                        if (!basic)
                            break;
                    }
                    /* Having climbed to a parent, the slot is visited
                       again so the child is processed after it. */
                    if (climbed)
                        --elem;
                }
                OBJ_SetProcess(gao, world->dynCallCount, 64);
                basic2 = gao->basic;
                if (basic2)
                {
                    dyn = basic2->dyn;
                    if (dyn)
                    {
                        if ((dyn->flags & 0x20000000) == 0 && (gao->flags2 & 0x4400000) == 0)
                            DYN_OneCall(gao);
                    }
                }
                lastElemBound = lastElem;
            }
        }
    }
    result = world->dynCallCount + 1;
    world->dynCallCount = result;
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

int DYN_b_SpeedReachInTime(struct sDyn* dyn, float* frictionOut, Vec3* tractionOut, Vec3* targetSpeed, float time)
{
    double invMass;
    double speedX;
    double forceY;
    int result;
    long double targetVecLen;
    double scale;
    float invMassF;
    float targetLen;

    invMass = dyn->mass;
    invMassF = dyn->mass;
    speedX = invMass * dyn->forceVector.x;
    forceY = dyn->forceVector.y;
    result = 0;
    targetLen = (float)sqrt(targetSpeed->x * targetSpeed->x + targetSpeed->y * targetSpeed->y + targetSpeed->z * targetSpeed->z);

    /*
      *frictionOut = -(__FYL2X__(
            fabs(
              targetLen
            / (sqrt(dyn->speedVector.z * invMass * (dyn->speedVector.z * invMass) + forceY * invMass * (forceY * invMass) + speedX * speedX) - targetLen)
            * -0.090909094),
            0.69314718055994528623)
        * (1.0
         / (invMassF
          * time)));
    */

    // fyl2x => (ST(1) ? log2 ST(0)) 
    // ln => c: log
    // log2 => c: log2
    // log10 => c: log10

    *frictionOut = (float)(-((log(2) * log2(fabs(targetLen / (sqrt(dyn->forceVector.z * invMass * (dyn->forceVector.z * invMass) + forceY * invMass * (forceY * invMass) + speedX * speedX) - targetLen) * -0.090909094))) * (1.0 / (invMassF * time))));

    targetVecLen = sqrt(targetSpeed->z * targetSpeed->z + targetSpeed->y * targetSpeed->y + targetSpeed->x * targetSpeed->x);
    tractionOut->x = (float)(1.0 / targetVecLen * targetSpeed->x);                    // MATH_NormalizeVector
    tractionOut->y = (float)(1.0 / targetVecLen * targetSpeed->y);
    tractionOut->z = (float)(1.0 / targetVecLen * targetSpeed->z);
    scale = targetLen * *frictionOut * invMassF;
    tractionOut->x = (float)(scale * tractionOut->x);
    tractionOut->y = (float)(scale * tractionOut->y);
    tractionOut->z = (float)(scale * tractionOut->z);
    if ((dyn->flags & 4) != 0)
    {
        dyn->forces->frictionVector.x = *frictionOut;
        dyn->forces->tractionVector = *tractionOut;
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

