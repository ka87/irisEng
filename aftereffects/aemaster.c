#include "aemaster.h"

#include "aefade.h"
#include "aehue.h"
#include "aemotion.h"
#include "aeframe.h"
#include "aezoom2.h"

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

void AEM_Control(int effect, int onOff, int slot, float value)
{
    int toggled;
    int changed;
    unsigned int cleared;

    switch (effect)
    {
    case 0:
        toggled = (onOff != 0) != ((PC_AE_MASTER & 8) != 0);
        changed = toggled;
        if (slot == 0)
            changed = depthBlurType0 != value || toggled;
        else if (slot == 1)
            changed = depthBlurType1 != value || toggled;
        else if (slot == 2)
            changed = depthBlurType2 != value || toggled;
        if (!changed)
            return;
        cleared = PC_AE_MASTER & ~8u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 8;
        if (slot == 0)
            depthBlurType0 = value;
        else if (slot == 1)
            depthBlurType1 = value;
        else if (slot == 2)
            depthBlurType2 = value;
        GDI_DepthBlurRampRebuild();
        return;
    case 1:
        cleared = PC_AE_MASTER & ~0x10u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x10;
        motionBlurFactor = value;
        return;
    case 2:
        cleared = PC_AE_MASTER & ~0x20u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x20;
        blurFactor = value;
        return;
    case 4:                                     // motionsmooth
        cleared = (int)(PC_AE_MASTER & ~AMO_BIT);
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | (int)AMO_BIT;
        return;
    case 5:
        cleared = PC_AE_MASTER & ~0x400u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x400;
        zoomSmoothFactor = value;
        return;
    case 6:
        cleared = PC_AE_MASTER & ~0x8000u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x8000;
        zoomSmoothFactor2 = value;
        return;
    case 7:
        cleared = PC_AE_MASTER & ~0x800u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 0x800;
        remenanceFactor = value;
        return;
    case 8:
        brightnessParam = value;
        return;
    case 9:
        colorBalanceParam1 = value;
        return;
    case 11:
        cleared = PC_AE_MASTER & ~1u;
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | 1;
        blackAndWhiteParam1 = value;
        return;
    case 12:
        if (slot == 0)
            unknownAEFlag4GlobalVar = value;
        else if (slot == 1)
            texFactorManual = value;
        else if (slot == 2)
            texFactorProbablyUnunsedType2 = value;
        cleared = (int)(PC_AE_MASTER & ~AHU_BIT);
        PC_AE_MASTER = cleared;
        if (onOff)
            PC_AE_MASTER = cleared | (int)AHU_BIT;
        return;
    case 14:
        toggled = (onOff != 0) != ((PC_AE_MASTER & 0x1000) != 0);
        if (depthBlurNear1 != value || toggled)
        {
            cleared = PC_AE_MASTER & ~0x1000u;
            PC_AE_MASTER = cleared;
            if (onOff)
                PC_AE_MASTER = cleared | 0x1000;
            depthBlurNear1 = value;
            GDI_DepthBlurRampRebuild();
        }
        return;
    case 15:
        cleared = (int)AFD_u_Cleared((unsigned int)PC_AE_MASTER);
        PC_AE_MASTER = cleared;
        if (onOff)
        {
            PC_AE_MASTER = cleared | (int)AFD_BIT;
            /* Only when one is not already running: switching the fade
               on again part way through leaves the timer where it is
               rather than starting over. */
            if (!GDI_i_AeFadeOpacity())
                GDI_AeFadeOpacitySet(AFD_FULL);
        }
        gAEOpacity = value;
        return;
    default:
        return;
    }
}
