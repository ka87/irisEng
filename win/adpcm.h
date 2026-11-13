#pragma once
#include <stdint.h>

typedef struct sADPCMParamStruct
{
    int channelNum;
    int i1;
    int blocksize;
    unsigned int u1;
    int16_t adaptationCoeffs[14];
} tADPCMParamStruct;

int pcSND_ADPCMToPCM_Convert(uint8_t* sndData, int16_t* destPtr, unsigned int totalSamples,
                             uint16_t channels, uint16_t blockBytes, uint16_t samplesPerBlock);
