#pragma once
#include "math.h"
#include "gdi.h" // lights.h-val nwem fordul le
#include "common.h"

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

typedef struct sFFTComplex
{
    float real;
    float imaginary;
} tFFTComplex;


typedef struct sFFTComplexCost
{
    float real1;
    float imaginary1;
    float real2;
    float imaginary2;
} tFFTComplexCost;


typedef struct sWaterEngineStruct
{
    int borderSizePowerOfTwo;
    int borderSizeDXDYPowerOfTwo;
    Vec2 wind;
    float direction;
    float worldSizeInMeters;
    float waveProp;
    tFFTComplex* spectroMap;
    int spectroMapCompressedSize;
    tFFTComplex* DXDYMap;
    int DXDYMapCompressedSize;
    int superShift0;
    int superShift1;
    int superShift2;
    tFFTComplexCost FFTCostTable[356];
    tFFTComplexCost FFTCostTableDXDY[356];
    unsigned int waveMapShift;
    unsigned int waveMapShiftCHP;
    float timeElapsed;
    int colorTable[512];
    int colorTableFastRecomputeDone;
    int colorTableForFastRecompute[512];
    int colorTableBackup[128];
    int waterColorMode;
    int skyColor;
    int bottomColor;
    int waterColor;
    int textureRefId;
    int useCloudShadingforColors;
    int isSymmetryEnabled;
} tWaterEngineStruct;

typedef struct sWtrStruct2
{
    float f1; // az f1, f2, f3 egy vec3 elvileg a WTR_BackExtendDXDY_sub_unknown szerint
    float f2;
    float f3;
    float f4;
} tWtrStruct2;

typedef struct sWaterParamsGlobalStruct
{
    Vec3* vertices;
    Vec3* vecsForUVs;
    Vec3* verticesForDisplay;
    unsigned int* colors;
    tFFTComplex* spaceMap;
    tFFTComplex* spaceMapDX;
    tFFTComplex* spaceMapDY;
    tWtrStruct2* concatenatedMap;
    unsigned int horizontalVertexNum;
    int verticallVertexNum;
    float waveHeight;
    float choppy;
    float tiling;
    float disturbance;
    float speed;
    float waterHeight;
    float waterFog;
    tWaterEngineStruct waterEngine;
} tWaterParamsGlobalStruct;

typedef struct sUserWaterParamsStruct
{
    int abadcafe;
    int waterEnabled;
    int skyColor;
    int bottomColor;
    int waterColor;
    int frustrumMeshSX;
    int frustrumMeshSY;
    int waveMapShift;
    int waveMapShiftCHP;
    int cludShadingIsEnabled;
    int textureRefID;
    int isSymmetryActive;
    float waveHeight;
    float choppy;
    float tiling;
    float speed;
    float waterHeight;
    float direction;
    float waveProp;
} tUserWaterParamsStruct;


int wtr_primitiveCount;
int wtr_NumA;
int wtr_NumB;

tWaterParamsGlobalStruct* pst_GlobalsWaterParams;
tWaterParamsGlobalStruct* pst_GlobalsWaterParams_Save;
tUserWaterParamsStruct stExportWaterParrams;

float COS_TABLE[2048]; //[cos, sin] [cos, sin] [...]
float waterHeight;
int Workkkk_748;

void WTR_Get_A_DifVector(Vec3* a1, Vec3* a2, int a3);
void WTR_Get_A_MrmVector(Vec3* a1, Vec3* a2);
void WTR_SetExportParams(tUserWaterParamsStruct* a1);
void PC_AE_ZListHook(int zListId);
void WTR_BeginLoadWorldHook();
void WTR_SetDefaulExp(struct sUserWaterParamsStruct* exported);
void WTR_WaterBlurNormal(Vec3* normals, int rowLength, int rowCount);

