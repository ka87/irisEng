#include "adpcm.h"

//https://github.com/videolan/vlc/blob/master/modules/codec/adpcm.c
//https://wiki.multimedia.cx/index.php/Microsoft_ADPCM

static const int pcSND_adaptationTable[16] = {
    230, 230, 230, 230, 307, 409, 512, 614,
    768, 614, 512, 409, 307, 230, 230, 230
};

static const int16_t pcSND_adaptationCoeffs[14] = {
    256, 0, 512, -256, 0, 0, 192, 64, 240, 0, 460, -208, 392, -232
};

static int16_t pcSND_l_ReadLE16(uint8_t** cursor)
{
    uint8_t* at;

    at = *cursor;
    *cursor = at + 2;
    return (int16_t)((at[1] << 8) | at[0]);
}

int16_t pcINO_ApplyADPCMAaptation(uint8_t nibble, int16_t* coeffs, int16_t* state)
{
    int coeff1;
    int coeff2;
    int sample1;
    int delta;
    int predicted;
    int signedNibble;
    int sample;
    int newDelta;

    coeff2 = coeffs[1];
    coeff1 = coeffs[0];
    sample1 = state[2];
    delta = (uint16_t)state[1];

    predicted = (coeff1 * sample1 + coeff2 * state[3]) / 256;
    signedNibble = (nibble & 8) != 0 ? nibble - 16 : nibble;
    sample = delta * signedNibble + predicted;

    if (sample < -32768)
        sample = 0x8000;    // preserved: not -32768, but every use of it narrows to int16_t
    else if (sample > 0x7FFF)
        sample = 0x7FFF;

    newDelta = delta * pcSND_adaptationTable[nibble] / 256;
    if (newDelta < 16)
        newDelta = 16;

    state[3] = sample1;
    state[1] = newDelta;
    state[2] = sample;
    return sample;
}

int pcSND_ADPCMToPCM_ConvertOneBlock(int16_t* dest, tADPCMParamStruct* params, uint8_t* src)
{
    int channels;
    int outBytes;
    int pairs;
    int i;
    int16_t* coeffsEven;
    int16_t* coeffsOdd;
    int16_t* state[2];
    int16_t stateEven[4];
    int16_t stateOdd[4];

    channels = params->channelNum;
    outBytes = 2 * params->channelNum * params->blocksize;
    state[0] = stateEven;
    state[1] = params->channelNum == 2 ? stateOdd : stateEven;

    for (i = 0; i < channels; i++)
    {
        state[i][0] = 0;
        *(uint8_t*)state[i] = *src++;
    }
    for (i = 0; i < channels; i++)
        state[i][1] = pcSND_l_ReadLE16(&src);
    for (i = 0; i < channels; i++)
        state[i][2] = pcSND_l_ReadLE16(&src);
    for (i = 0; i < channels; i++)
        state[i][3] = pcSND_l_ReadLE16(&src);

    coeffsEven = &params->adaptationCoeffs[2 * *(uint8_t*)state[0]];
    coeffsOdd = &params->adaptationCoeffs[2 * *(uint8_t*)state[1]];

    for (i = 0; i < channels; i++)
        *dest++ = state[i][3];
    for (i = 0; i < channels; i++)
        *dest++ = state[i][2];

    pairs = (params->channelNum * (params->blocksize - 2) + 1) / 2;
    for (i = 0; i < pairs; i++)
    {
        dest[0] = pcINO_ApplyADPCMAaptation(*src >> 4, coeffsEven, state[0]);
        dest[1] = pcINO_ApplyADPCMAaptation(*src & 0xF, coeffsOdd, state[1]);
        dest += 2;
        src++;
    }
    return outBytes;
}

int pcSND_ADPCMToPCM_Convert(uint8_t* sndData, int16_t* destPtr, unsigned int totalSamples,
                             uint16_t channels, uint16_t blockBytes, uint16_t samplesPerBlock)
{
    unsigned int wholeBlocks;
    unsigned int tail;
    unsigned int i;
    int outBytes;
    tADPCMParamStruct params;

    params.channelNum = channels;
    for (i = 0; i < 14; i++)
        params.adaptationCoeffs[i] = pcSND_adaptationCoeffs[i];
    params.blocksize = samplesPerBlock;

    wholeBlocks = totalSamples / samplesPerBlock;
    tail = totalSamples % samplesPerBlock;

    for (i = 0; i < wholeBlocks; i++)
    {
        outBytes = pcSND_ADPCMToPCM_ConvertOneBlock(destPtr, &params, sndData);
        sndData += blockBytes;
        destPtr += outBytes / 2;
    }
    if (tail)
    {
        params.blocksize = tail;
        pcSND_ADPCMToPCM_ConvertOneBlock(destPtr, &params, sndData);
    }
    return 0;
}
