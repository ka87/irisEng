#pragma once
#include "gro_common.h"
#include "../common.h"

typedef struct sFontGlypthElem
{
    float x;
    float y;
    float w;
    float h;
} tFontGlypthElem;

typedef struct sFontData
{
    int texKey;
    int texKeyAlternative;
    struct sGEOMAT* mat;
    tFontGlypthElem fge[224];
    __int16 width;
    __int16 height;
} tFontData;

typedef struct sGEOSTRStringData
{
    int16_t flags;
    byte gap1[2];
    uint16_t index;
    uint16_t number;
    uint16_t index2;
    uint16_t number2;
    Vec3 pivotPoint;
    unsigned int frame;
    int16_t framePosX1;
    int16_t framePosX2;
    int16_t framePosY1;
    int16_t framePosY2;
    int16_t frameBorderSize1;
    int16_t frameBorderSize2;
    int frameColor;
    int i2;
} tGEOSTRStringData;

typedef struct sGEOSTREffectStruct
{
    int flags;
    int effectID;
    int stringid;
    int index;
    int number;
    float f1;         // ezek kb barmilyen paramek lehetnek tupustol fuggoen
    float timeElapsed; // -- 
    float color1; // --
    float color2; // --
    float f3; // --
} tGEOSTREffectStruct;

typedef struct sGEOSTREffectStructType20
{
    int flags;
    int effectIidx;
    int stringid;
    int index;
    int number;
    int unki1;
    byte flagsAndChar; // -- 
    byte unkb1; // -- 
    byte _gap[2];
    float color1; // --
    float color2; // --
    float f3; // --
} tGEOSTREffectStructType20;




typedef struct sGEOSTRLetter
{
    uint32_t flagsAndChar;
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    uint32_t textColor;
} tGEOSTRLetter;


typedef struct sGEOSTRStruct
{
    struct sGroIntf* intf;
    int usageCount;
    int unkNum;
    byte stringNum;
    byte maxEffectsNum;
    byte effectsNum;
    byte gapb;
    int16_t fontMaterialID;
    byte gapc[2];
    int renderCnt;
    int16_t width;
    int16_t height;
    tGEOSTRLetter* letters;
    tGEOSTRStringData* stringData;
    tGEOSTREffectStruct effect[32];
    struct sFontData* fontDesc;
    int* specularColorsPtr;
    byte gap[8];
    __int16 shorts[24];
} tGEOSTRStruct;

typedef struct sUnkGeoStrStruct
{
    int triangleNum[64];
    struct sFontData* fontData[64];
    int triangleChunkID[64];
    int materialID[64];
} tUnkGeoStrStruct;


struct sGameObjectMain* STR_sgpst_GO;

int16_t STR_w_XFramePivot;
int16_t STR_w_YFramePivot;
int16_t STR_w_XPivot;
int16_t STR_w_YPivot;
int16_t STR_w_LetterDX; // unused ?
int16_t STR_w_LetterDY; // unused ?

void STR_Reinit(struct sGroGeneric* a1);
void STR_Init();