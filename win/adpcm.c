#include "adpcm.h"

//https://github.com/videolan/vlc/blob/master/modules/codec/adpcm.c
//https://wiki.multimedia.cx/index.php/Microsoft_ADPCM

__int16 pcINO_ApplyADPCMAaptation(unsigned __int8 a1, __int16* a2, __int16* a3)
{
    int v4;
    int v5;
    int v6;
    __int16 v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    int adaptationTable[16];

    adaptationTable[0] = 230;
    adaptationTable[1] = 230;
    adaptationTable[2] = 230;
    adaptationTable[3] = 230;
    adaptationTable[4] = 307;
    adaptationTable[5] = 409;
    adaptationTable[6] = 512;
    adaptationTable[7] = 614;
    adaptationTable[8] = 768;
    adaptationTable[9] = 614;
    adaptationTable[10] = 512;
    adaptationTable[11] = 409;
    adaptationTable[12] = 307;
    adaptationTable[13] = 230;
    adaptationTable[14] = 230;
    adaptationTable[15] = 230;

    v4 = a2[1];
    v5 = *a2;

    v6 = (__int16)a3[3] * v4;
    v7 = a3[2];
    v8 = (unsigned __int16)a3[1];
    v9 = v5 * v7 + v6;
    v10 = a1;
    v11 = (a1 & 8) == 0;
    v12 = a1 - 16;
    if (v11)
        v12 = v10;
    v13 = v8 * v12 + v9 / 256;
    if (v13 >= -32768)
    {
        if (v13 > 0x7FFF)
            v13 = 0x7FFF;
    }
    else
    {
        v13 = 0x8000;
    }
    v14 = v8 * adaptationTable[v10] / 256;
    if (v14 < 16)
        v14 = 16;
    a3[3] = v7;
    a3[1] = v14;
    a3[2] = v13;
    return v13;
}

int pcSND_ADPCMToPCM_ConvertOneBlock(__int16* dest, tADPCMParamStruct* a2, unsigned __int8* src)
{
    int channelNum;
    int isStereo;
    __int16* v7;
    int o;
    __int16* v9;
    char v10;
    int i;
    __int16* v12;
    __int16 v13;
    int j;
    __int16* v15;
    __int16 v16;
    int k;
    __int16* v18;
    __int16 v19;
    int l;
    int m;
    int v22;
    unsigned int v23;
    __int16 v24;
    __int16* v25;
    __int16* v26;
    int v28;
    tADPCMParamStruct* v29;
    __int16* coeff1;
    __int16* coeff2;
    __int16* v32[2];
    __int16 v33[4];
    __int16 v34[4];

    channelNum = a2->channelNum;
    isStereo = a2->channelNum == 2;
    v28 = 2 * a2->channelNum * a2->blocksize;
    v32[0] = v33;
    v7 = v34;
    if (!isStereo)
        v7 = v33;
    o = 0;
    for (v32[1] = v7; o < channelNum; a2 = v29)
    {
        v9 = v32[o];
        v29 = a2;
        v10 = *src++;
        ++o;
        *v9 = 0;
        *(unsigned __int8*)v9 = v10;
    }
    for (i = 0; i < channelNum; i++)
    {
        v12 = v32[i];
        v13 = 0;
        v13 |= ((src[1] << 8) & 0xff00);
        v13 |= (src[0]) & 0xff;
        v12[1] = v13;
        src += 2;
    }
    for (j = 0; j < channelNum; j++)
    {
        v15 = v32[j];
        v16 = 0;
        v16 |= ((src[1] << 8) & 0xff00);
        v16 |= (src[0]) & 0xff;
        v15[2] = v16;
        src += 2;
    }
    for (k = 0; k < channelNum; k++)
    {
        v18 = v32[k];
        v19 = 0;
        v19 |= ((src[1] << 8) & 0xff00);
        v19 |= (src[0]) & 0xff;
        v18[3] = v19;
        src += 2;
    }
    coeff1 = &a2->adaptationCoeffs[2 * (v33[0] & 0xff)];
    coeff2 = &a2->adaptationCoeffs[2 * *(unsigned __int8*)v32[1]];
    for (l = 0; l < channelNum; ++l)
        *dest++ = v32[l][3];
    for (m = 0; m < channelNum; ++m)
        *dest++ = v32[m][2];
    v22 = a2->channelNum * (a2->blocksize - 2);
    if (v22 > 0)
    {
        v23 = ((unsigned int)(v22 - 1) >> 1) + 1;
        do
        {
            v24 = pcINO_ApplyADPCMAaptation(*src >> 4, coeff1, v33);
            v25 = v32[1];
            *dest = v24;
            v26 = dest + 1;
            *v26 = pcINO_ApplyADPCMAaptation(*src & 0xF, coeff2, v25);
            dest = v26 + 1;
            ++src;
            --v23;
        } while (v23);
    }
    return v28;
}

int pcSND_ADPCMToPCM_Convert(unsigned __int8* sndData, __int16* destPtr, unsigned int fullSize, unsigned __int16 a4, unsigned __int16 a5, unsigned __int16 blockSize)
{
    int v6;
    unsigned int v9;
    int v10;
    tADPCMParamStruct a2;

    a2.channelNum = a4;
    v6 = fullSize % blockSize;
    a2.adaptationCoeffs[0] = 256;
    a2.adaptationCoeffs[1] = 0;
    a2.adaptationCoeffs[2] = 512;
    a2.adaptationCoeffs[3] = -256;
    a2.adaptationCoeffs[4] = 0;
    a2.adaptationCoeffs[5] = 0;
    a2.adaptationCoeffs[6] = 192;
    a2.adaptationCoeffs[7] = 64;
    a2.adaptationCoeffs[8] = 240;
    a2.adaptationCoeffs[9] = 0;
    a2.adaptationCoeffs[10] = 460;
    a2.adaptationCoeffs[11] = -208;
    a2.adaptationCoeffs[12] = 392;
    a2.adaptationCoeffs[13] = -232;
    a2.blocksize = blockSize;
    if (fullSize / blockSize)
    {
        v9 = fullSize / blockSize;
        do
        {
            v10 = pcSND_ADPCMToPCM_ConvertOneBlock(destPtr, &a2, sndData);
            sndData += a5;
            --v9;
            destPtr += v10 / 2;
        } while (v9);
        v6 = fullSize % blockSize;
    }
    if (v6)
    {
        a2.blocksize = v6;
        pcSND_ADPCMToPCM_ConvertOneBlock(destPtr, &a2, sndData);
    }
    return 0;
}