#pragma once

typedef struct sADPCMParamStruct
{
    int channelNum;
    int v12;
    int blocksize;
    unsigned int v13;
    __int16 adaptationCoeffs[14];
} tADPCMParamStruct;

int pcSND_ADPCMToPCM_Convert(unsigned __int8* sndData, __int16* destPtr, unsigned int fullSize, unsigned __int16 a4, unsigned __int16 a5, unsigned __int16 blockSize);