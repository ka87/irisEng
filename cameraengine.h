#pragma once
#include "math.h"
#include "common.h"

typedef struct sDDCamStruct
{
    int flags;
    float nearPlane;
    float farPlane;
    float focale; // FOV
    float YoverX;
    float factorX;
    float factorY;
    float centerX;
    float centerY;
    float width;
    float height;
    float viewPortPosX;
    float viewPortPosY;
    float viewPortSizeWidth;
    float viewPortSizeHeight;
    int viewPortRealPosX;
    int viewPortRealPosY;
    float isoFactor;
    float isoZoom;
    Vec3 planeUp;
    Vec3 planeDown;
    Vec3 planeRight;
    Vec3 planeLeft;
    float plains15;
    float plains16;
    float plainsf17;
    float plainsf18;
    float plainsf19;
    Mtx4x4 objectMtx;
    Mtx4x4 cameraMtx;
    Mtx4x4 projectionMtx;
    Mtx4x4* viewMtxPtr_setInGRORender;
    uint8_t gapC81b[12];
} tDDCamStruct;

typedef struct sDDContainerStructForIDA
{
    tDDCamStruct cam;
    tDDCamStruct camBackup;
    uint8_t gapC81c[12];
} tDDContainerStruct;

typedef struct sDDContainerStruct2ForIDA
{
    int32_t flags2;
    float pixelYoverX;
    float manualScreenRatio;
    uint8_t gapB4_unused[16];
    int32_t selectedScreenRatio;
    uint8_t gapC8_unused[8];
} tDDContainerStruct2ForIDA;

Vec3 g_stGlobalNormPlaneUp;
Vec3 g_stGlobalNormPlaneDown;
Vec3 g_stGlobalNormPlaneLeft;
Vec3 g_stGlobalNormPlaneRight;
Vec3 g_stOverlayed_GlobalNormPlaneUp;
Vec3 g_stOverlayed_GlobalNormPlaneDown;
Vec3 g_stOverlayed_GlobalNormPlaneLeft;
Vec3 g_stOverlayed_GlobalNormPlaneRight;
Vec3 g_stGlobalCameraPos;
Vec3 g_stGlobalCameraSight;
float g_fDistance;
tDDCamStruct CAM_gst_IsoNoClip;


void CAM_AssignCameraToDevice(tDDContainerStruct2ForIDA* srcdd, tDDContainerStruct* dstdd, int winWidth, int winHeight);
void CAM_2Dto3D(struct sDDCamStruct* cam, Vec3* dest, Vec3* src);
void CAM_2Dto3DCamera(struct sDDCamStruct* cam, Vec3* cam3D, Vec3* cam2D);
void CAM_2Dto3DCamera2(struct sDDCamStruct* cam, Vec3* cam3D, Vec3* cam2D);
void CAM_Engine_ComputePlans(tDDContainerStruct* dd);
int CAM_SetCameraMatrix(tDDContainerStruct* ddc, Mtx4x4* mtx);
void CAM_Inverse(tDDContainerStruct* ddc);
void CAM_Engine_Init(tDDContainerStruct* dd);