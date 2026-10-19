#pragma once
#include "math.h"
#include "common.h"

typedef struct sDynForces
{
    Vec3 gravityVector;
    Vec3 tractionVector;
    Vec3 streamVec;
    Vec3 frictionVector;
    int i3;
} tDynForces;

typedef struct sDynSolid
{
    Vec3 rotSpeedVector;
    Vec3 torqueVector;
    Vec3 oldPos;
    float rotSpeedLimit;
    float elastic;
    int x;
} tDynSolid;

typedef struct sDynCol
{
    float rebound;
    float slide;
    float reboundHorizCosAngle;
    float slideHorizCosAngle;
    float rectFactor;
} tDynCol;

typedef struct sDynConstraint
{
    Vec3 point1;
    Vec3 point2;
    Vec3 parabolO;
    float parabolRadius;
    Vec3 tangentVec;
    byte constraintType;
    byte _pad[2];
} tDynConstraint;


typedef struct sDyn
{
    int flags;
    Vec3* vptr;
    Vec3 forceVector; // force
    Vec3 momentum;
    float mass;
    struct sDynForces* forces;
    float speedLimit;
    struct sDynSolid* solid;
    struct sDynCol* collision;
    float speedLimitVertical;
    struct sDynConstraint* constraint;
    float maxStepSize;
    Vec3 maxPos;
} tDyn;

float DYN_gf_AutoOrientHiInertiaValue;
float DYN_gf_AutoOrientMediumInertiaValue;
float DYN_gf_AutoOrientLowInertiaValue;

tDyn* DYN_pst_Dyna_Create();
void DYN_Dyna_Init(tDyn* dyn, int flags, float mmass, float speedLimit, float speedLimitVertical, Vec3* speedVec, Vec3* pos);
void DYN_Dyna_Close(tDyn* a1);
void DYN_SetHorizontalSpeedComponent(struct sDyn* a1, Vec3* a2);
int DYN_b_SpeedReachInTime(struct sDyn* a1, float* a2, Vec3* a3, Vec3* a4, float a5);
void DYN_SetSpeedVector(struct sDyn* a1, Vec3* a2);
void DYN_SpeedGetVector(struct sDyn* a1, Vec3* a2);
void DYN_ChangeDynFlags(struct sDyn* a1, int a2, int a3);
float DYN_f_FindFrictionForMaxSpeed(struct sDyn* a1, float a2);
void DYN_ApplyHorizontalSpeed(struct sDyn* a1, float a2);
void DYN_ConstraintInit(struct sDynConstraint* a1, char a2, Vec3* a3, Vec3* a4, Vec3* a5, float a6);
void DYN_SetSpeedVector_WithoutChangingGravity(struct sDyn* a1, Vec3* a2);
void DYN_ApplySpeedLimit(struct sDyn* a1);
void DYN_CalcSlideVector(Vec3* a1, Vec3* a2, Vec3* a3, float a4);
void DYN_CalcReboundVector(Vec3* a1, Vec3* a2, Vec3* a3, float a4);
int DYN_MainCall(struct sWorld* a1);
void DYN_OneCall(struct sGameObjectMain* a1);
void DYN_InitModule();
void DYN_CloseModule();
