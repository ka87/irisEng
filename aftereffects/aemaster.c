#include "aemaster.h"

int PC_AE_MASTER = 0;

float brightnessParam = 0.5f;
float colorBalanceParam1 = 0.5f;
float blackAndWhiteParam1 = 0.0f;

float motionBlurFactor = 0.0f;
float remenanceFactor = 0.0f;
float zoomSmoothFactor = 0.0f;
float blurFactor = 0.0f;
float zoomSmoothFactor2 = 0.0f;

float depthBlurNear1 = 0.0f;
float depthBlurType0 = 0.0f;
float depthBlurType1 = 0.0f;
float depthBlurType2 = 0.0f;

float gAEOpacity = 0.0f;

float unknownAEFlag4GlobalVar = 0.0f;
float texFactorManual = 0.0f;
float texFactorProbablyUnunsedType2 = 0.0f;

int AEM_i_OnOff(int effect)
{
    int result;

    switch (effect)
    {
    case 0:
        result = PC_AE_MASTER & 8;                // depthblur
        break;
    case 1:
        result = PC_AE_MASTER & 0x10;             // motionblur
        break;
    case 2:
        result = PC_AE_MASTER & 0x20;             // screenblur
        break;
    case 3:
        result = PC_AE_MASTER & 0x40;             // emboss
        break;
    case 4:
        result = PC_AE_MASTER & 0x80;             // motionsmooth
        break;
    case 5:
        result = PC_AE_MASTER & 0x400;            // zoomsmooth
        break;
    case 6:
        result = PC_AE_MASTER & 0x8000;           // zoomsmooth2
        break;
    case 7:
        result = PC_AE_MASTER & 0x800;            // remanance
        break;
    case 10:
        result = PC_AE_MASTER & 0x2000;           // full-aa-blur
        break;
    case 11:
        result = PC_AE_MASTER & 1;                // colorbalance
        break;
    case 12:
        result = PC_AE_MASTER & 4;
        break;
    case 14:
        result = PC_AE_MASTER & 0x1000;           // depthblurnear
        break;
    case 15:
        result = PC_AE_MASTER & 0x40000;          // fade
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

float AEM_f_Param(int effect, int slot)
{
    double result;

    switch (effect)
    {
    case 0:
        if (slot == 0)                          // 17 warp
            result = depthBlurType0;
        else if (slot == 1)
            result = depthBlurType1;
        else if (slot == 2)
            result = depthBlurType2;
        else
            // preserved: every other getter's unknown slot gives zero; this one gives the motion blur
            result = motionBlurFactor;
        break;
    case 1:
        result = motionBlurFactor;
        break;
    case 2:
        result = blurFactor;
        break;
    case 5:
        result = zoomSmoothFactor;
        break;
    case 6:
        result = zoomSmoothFactor2;
        break;
    case 7:
        result = remenanceFactor;
        break;
    case 8:
        result = brightnessParam;
        break;
    case 9:
        result = colorBalanceParam1;
        break;
    case 11:
        result = blackAndWhiteParam1;
        break;
    case 12:
        if (slot == 0)
            result = unknownAEFlag4GlobalVar;
        else if (slot == 1)
            result = texFactorManual;
        else if (slot == 2)
            result = texFactorProbablyUnunsedType2;
        else
            result = 0.0;
        break;
    case 14:
        result = depthBlurNear1;
        break;
    default:
        result = 0.0;
        break;
    }
    return (float)result;
}
