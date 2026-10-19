#include "time.h"
#include "math.h"
#include "common.h"

int ENG_ifSingleStreamThenEq2Otherwise8 = 8;

int RND_JADErand()
{
    float elapsedTime;
    double tmpDbl; 

    elapsedTime = TIM_f_Clock_TrueRead();
    tmpDbl = (double)((*(uint16_t*)(&elapsedTime)) * 0.000015259022 * 32767.0 + MATH_gd_magicDoubleForRound);
    return ((*(uint32_t*)&tmpDbl) & 0xffffffff) ^ rand(); // double 8 bajt, tetejet levagjuk igy egy kis szam lesz
}

float RND_RandFloatRange(float start, float end)
{
    return ((float)rand() * (1.0f / 32767.0f) * (end - start) + start);
}