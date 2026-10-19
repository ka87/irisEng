#pragma once
#include "../math.h"
#include "../gfx.h"

typedef struct sGfxFlareDataBasic
{
    Vec3 v1;
    Vec3 v2;
    float unkFactor;
    int flags;
    float colorFactor;
    float timeColorFactorDivider;
    float i11;
    float i12;
    int i13;
    int i14;
    int i15;
    Vec3 v3;
    int materialNum2;
    int color2;
    int color1;
} tGfxFlareDataBasic;

typedef struct sGfxFlareData
{
    tGfxFlareDataBasic basic;
    float i22;
    float i23;
    float i24;
    float i25;
    float i26;
    float i27;
    float i28;
    float i29;
    float i30;
    float i31;
    float i32;
    float i33;
    float i34;
    float i35;
    float i36;
    float i37;
    int materialNum1;
    int i39;
    int i40;
    int i41;
    int i42;
    int i43;
    int i44;
    int i45;
    int i46;
    int color3;
    int i48;
    int i49;
    int i50;
    int i51;
    int i52;
    int i53;
    int i54;
    int i55;
} tGfxFlareData;

void GFX_Flare_Test(tGfxFlareData* a1);
struct sGfxFlareData* GFX_Flare_Create(int a1);
void GFX_Flare_Render(struct sGfxFlareData* a1);