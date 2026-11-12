#pragma once

typedef struct sADPCMParamStruct
{
    int channelNum;
    int v12;
    int blocksize;
    unsigned int v13;
    int16_t adaptationCoeffs[14];
} tADPCMParamStruct;

int pcSND_ADPCMToPCM_Convert(uint8_t* sndData, int16_t* destPtr, unsigned int fullSize, uint16_t a4, uint16_t a5, uint16_t blockSize);