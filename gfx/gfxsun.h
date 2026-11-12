#pragma once
#include <d3d9.h>
#include "../gameobject.h"
#include "../gfx.h"

typedef struct sGfxSunD3DSpecificDataInner
{
    IDirect3DSurface9* d3dppSurface;
    int bitMask;
    float alphaThreshold;
} tGfxSunD3DSpecificDataInner;

typedef struct sGfxSunD3DSpecificData
{
    int ptrElemSize;
    int maxElemNum;
    tGfxSunD3DSpecificDataInner* currPtrInner;
    tGfxSunD3DSpecificDataInner* firstPtrInner;
    tGfxSunD3DSpecificDataInner* ptr;
    tGfxSunD3DSpecificDataInner elems[3];
    IDirect3DTexture9* d3dTexture;
    IDirect3DSurface9* d3dRenderTarget;
} tGfxSunD3DSpecificData;

typedef struct sGfxSunDataInner
{
    float i1;
    float i2;
    int i3;
    float i4;
    float i5;
    float colorFactor;
    uint8_t colorID;
    uint8_t colorID2;
    uint8_t _gap[2];
} tGfxSunDataInner;

typedef struct sGfxSunData
{
    Vec3 v1;
    float i4;
    float i5;
    float i6;
    float i7;
    float i8;
    float i9;
    float i10;
    float i11;
    float i12;
    float i13;
    float i14;
    float multiplier;
    float i16;
    float i17;
    uint8_t gap1[4];
    int num;
    int color;
    int flags;
    uint8_t b1;
    uint8_t b2;
    tGfxSunDataInner elems[32];
    uint8_t gap2[2688];
    float rayLums[8];
    int alphaThresholdFlags;
    Vec3 v2;
    tGfxSunD3DSpecificData d3dStuff;
} tGfxSunData;

typedef struct sD3dStrectRectUnkStruct
{
    int x;
    int y;
} tD3dStrectRectUnkStruct;

typedef struct sD3dStrectRectUnkStructFloats
{
    float x;
    float y;
} tD3dStrectRectUnkStructFloats;

int gSunEffectOnOffArray[16];
int sunUnkCnt;

struct sGfxSunData* GFX_Sun_Create(void);
void GFX_Sun_Destroy(struct sGfxSunD3DSpecificData* sun3d);
void GFX_Sun_Render(struct sGfxSunData* sun);
void GFX_Sun_Seti(struct sGfxSunData* sun, int id);