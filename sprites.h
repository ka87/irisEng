#pragma once
#include "common.h"
#include "math.h"
//#include "graphicsobjects/geo.h"
//#include "graphicsobjects/geomaterial.h"

typedef struct sSpgVertexElem
{
    Vec3 v;
    int16_t unk1;
    int16_t unk2;
    int16_t colorComponentR;
    int16_t colorComponentG;
    int16_t colorComponentB;
    int16_t colorComponentA;
} tSpgVertexElem;

typedef struct sSpgTrisToSpritesStruct
{
    float f1;
    float noise;
    int maxDivide;
    int flags;
} tSpgTrisToSpritesStruct;

typedef union uGeoSpriteElem { // index encoded in a float, there is a special case when idx == 0xC0DE (explode)
    struct st1 {
        uint16_t idx;
        uint16_t dummy; // for alignment
    };
    float size;
} tGeoSpriteElem;

typedef struct sSpriteC0DEData {
    Vec3 v1;
    float f1;
} tSpriteC0DEData;

typedef struct sGeoSprites
{
    uint32_t num;
    int matIdx;
    tGeoSpriteElem* spriteElems;
    int i1;// unused ?
    int i2;// unused ?
    float globalSize;
    float globalRatio;
} tGeoSprites;

Vec3 SPG_gs_SpecialFogAxis;
int SPG_gs_SpecialFogCOLOR;
float SPG_gs_SpecialFogDistance;
Vec3 SPG_gs_SpecialFogCulledPoint;
struct sGEOGRO* pst_CurrentObj;
struct sGEOMAT* pst_CurrentMat;
int* SPG_gvCOLORStack;
Vec3 SPG_gvStack[256];
Vec3* SPG_gvCurrentStack;
struct sGeoSprites SPG_stIndxSp[4];
unsigned int gvBase;
Vec3 SPGNoise1;
Vec3 SPGNoise2;

void SPG_TransformTrisToSprites(Vec3* a1, Vec3* a2, Vec3* a3, unsigned int* colors, struct sSpgTrisToSpritesStruct* a5);
void SPG_ComputeSpriteFog(Vec3* a1, int* a2, int a3);
void SPG_ComputeSpriteFog_Cull(Vec3* a1, int* a2, int a3);