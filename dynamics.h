#pragma once
#include "math.h"
#include "common.h"

struct sWorld;

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
    uint8_t constraintType;
    uint8_t _pad[2];
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
void DYN_Dyna_Close(tDyn* dyn);
void DYN_SetHorizontalSpeedComponent(struct sDyn* dyn, Vec3* speed);
int DYN_b_SpeedReachInTime(struct sDyn* dyn, float* frictionOut, Vec3* tractionOut, Vec3* targetSpeed, float time);
void DYN_SetSpeedVector(struct sDyn* dyn, Vec3* speed);
void DYN_SpeedGetVector(struct sDyn* dyn, Vec3* v);
void DYN_ChangeDynFlags(struct sDyn* dyn, int newFlags, int oldFlags);
float DYN_f_FindFrictionForMaxSpeed(struct sDyn* dyn, float maxSpeed);
void DYN_ApplyHorizontalSpeed(struct sDyn* dyn, float speed);
void DYN_ConstraintInit(struct sDynConstraint* constraint, char type, Vec3* point1, Vec3* point2, Vec3* parabolO, float parabolRadius);
void DYN_SetSpeedVector_WithoutChangingGravity(struct sDyn* dyn, Vec3* speedVec);
void DYN_ApplySpeedLimit(struct sDyn* dyn);
void DYN_CalcSlideVector(Vec3* slideVec, Vec3* normal, Vec3* speed, float scale);
void DYN_CalcReboundVector(Vec3* reboundVec, Vec3* normal, Vec3* speed, float scale);
int DYN_MainCall(struct sWorld* world);
void DYN_OneCall(struct sGameObjectMain* gao);
void DYN_InitModule();
void DYN_CloseModule();
