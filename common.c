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
    /* Every caller scales the result as n * RND_JADErand() / 0x7FFF, so it
       must stay within 0..0x7FFF -- which MSVC's rand() does, its RAND_MAX
       being 0x7FFF. glibc's goes to 2^31-1: the XOR then gave huge values,
       the scaling overflowed, and MATH_RandInt(0, 20) came back as -43843
       and indexed an AI array 175 KB before its start. */
    return ((*(uint32_t*)&tmpDbl) & 0xffffffff) ^ RND_i_Rand(); // double 8 bajt, tetejet levagjuk igy egy kis szam lesz
}

float RND_RandFloatRange(float start, float end)
{
    return ((float)RND_i_Rand() * (1.0f / 32767.0f) * (end - start) + start);
}