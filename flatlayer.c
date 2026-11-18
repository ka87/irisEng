#include "flatlayer.h"

#include "gdi.h"

int FLY_gi_Box = 1;

int FLY_i_Width(int w, int h)
{
    int boxW;

    if (!FLY_gi_Box || !GDI_gi_Widescreen
        || GDI_gf_RefYoverX <= 0.0f || h <= 0)
        return w;
    boxW = (int)((float)h / GDI_gf_RefYoverX);
    return boxW < w ? boxW : w;
}

int FLY_i_OffsetX(int w, int h)
{
    return (w - FLY_i_Width(w, h)) / 2;
}

float FLY_f_ToLayoutX(float windowFraction, int w, int h)
{
    int flat;

    flat = FLY_i_Width(w, h);
    if (flat <= 0)
        return windowFraction;
    return (windowFraction * (float)w - (float)FLY_i_OffsetX(w, h)) / (float)flat;
}

float FLY_f_FromLayoutX(float layoutFraction, int w, int h)
{
    if (w <= 0)
        return layoutFraction;
    return ((float)FLY_i_OffsetX(w, h)
            + layoutFraction * (float)FLY_i_Width(w, h)) / (float)w;
}
