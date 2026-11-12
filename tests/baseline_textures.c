/* The frozen pre-rewrite copies of the textures.c functions the
   differential test drives. Taken verbatim out of the pre-pass file;
   every public name is renamed on the command line so both copies can
   live in one binary. */

#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
#include "../time.h"

int al_QualityCompressionOrder[] = { 0, 1, 0, 2, 0, 1, 3, 0, 1, 2, 0, 1, 0, 2, 0, 1, 3, 0, 1, 2, 4 };
int al_QualityCompressionLimit[] = { 0x10, 0x10, 0x20, 0x40, 0x400, 0 };

void TEX_Compress24_Xo2(char* src, char* dest, int a3, int a4)
{
    unsigned int i;

    for (i = (unsigned int)(a4 * a3) >> 1; i; --i)
    {
        *dest = ((*src >> 1) & 0x7E) + ((src[3] >> 1) & 0x7E);
        dest[1] = ((src[1] >> 1) & 0x7E) + ((src[4] >> 1) & 0x7E);
        dest[2] = ((src[2] >> 1) & 0x7E) + ((src[5] >> 1) & 0x7E);
        src += 6;
        dest += 3;
    }
}

void TEX_Compress24_Yo2(char* a1, char* a2, int a3, unsigned int a4)
{
    unsigned int v4;
    int i;
    int v8;
    char v9;
    char v10;

    v4 = a4 >> 1;
    for (i = 3 * a3; v4; --v4)
    {
        if (i)
        {
            v8 = 3 * a3;
            do
            {
                v9 = (a1[3 * a3] >> 1) & 0x7E;
                v10 = (*a1++ >> 1) & 0x7E;
                *a2++ = v10 + v9;
                --v8;
            } while (v8);
        }
        a1 += i;
    }
}

void TEX_Compress_Xo2(char* a2, char* dest, int a4, int a5)
{
    unsigned int i;

    for (i = (unsigned int)(a5 * a4) >> 1; i; --i)
    {
        *(uint32_t*)dest = ((*(uint32_t*)a2 >> 1) & 0x7E7E7E7E) + ((*((uint32_t*)a2 + 1) >> 1) & 0x7E7E7E7E);
        a2 += 8;
        dest += 4;
    }
}

void TEX_Compress_Yo2(char* a1, char* a2, int a3, unsigned int a4)
{
    int v6;
    int v7;
    unsigned int a4a;

    if (a4 >> 1)
    {
        a4a = a4 >> 1;
        v6 = 4 * a3;
        do
        {
            if (a3)
            {
                v7 = a3;
                do
                {
                    *(uint32_t*)a2 = ((*(uint32_t*)a1 >> 1) & 0x7E7E7E7E) + ((*(uint32_t*)&a1[v6] >> 1) & 0x7E7E7E7E);
                    a1 += 4;
                    a2 += 4;
                    --v7;
                } while (v7);
            }
            a1 += v6;
            --a4a;
        } while (a4a);
    }
}

void TEX_Compress8_Xo2(char* a1, char* a2, int a3, int a4)
{
    unsigned int i;

    for (i = (unsigned int)(a4 * a3) >> 1; i; --i)
    {
        *a2++ = *a1;
        a1 += 2;
    }
}

void TEX_Compress8_Yo2(char* a1, char* a2, int a3, unsigned int a4)
{
    unsigned int v5;
    int v7;

    if (a4 >> 1)
    {
        v5 = a4 >> 1;
        do
        {
            if (a3)
            {
                v7 = a3;
                do
                {
                    *a2++ = *a1++;
                    --v7;
                } while (v7);
            }
            a1 += a3;
            --v5;
        } while (v5);
    }
}

void TEX_Compress4_Xo2(char* a1, char* a2, int a3, int a4)
{
    unsigned int i;

    for (i = (unsigned int)(a4 * a3) >> 2; i; --i)
    {
        *a2++ = *a1 & 0xF0 | (a1[1] >> 4);
        a1 += 2;
    }
}

void TEX_Compress4_Yo2(char* a1, char* a2, unsigned int a3, unsigned int a4)
{
    unsigned int v4;
    unsigned int v6;
    unsigned int v8;

    v4 = a3 >> 1;
    if (a4 >> 1)
    {
        v6 = a4 >> 1;
        do
        {
            if (v4)
            {
                v8 = a3 >> 1;
                do
                {
                    *a2++ = *a1++;
                    --v8;
                } while (v8);
            }
            a1 += v4;
            --v6;
        } while (v6);
    }
}

void TEX_Expand_XBy2(int* a1, int* a2, int a3, int a4)
{
    int* v4; // ecx
    int* i; // eax
    int v6; // esi
    int* v7; // ecx

    v4 = &a2[2 * a4 * a3];
    for (i = &a1[a4 * a3]; i > a1; *v4 = *i)
    {
        v6 = *--i;
        v7 = v4 - 1;
        *v7 = v6;
        v4 = v7 - 1;
    }
}

void TEX_Expand_YBy2(char* a1, char* a2, int a3, int a4)
{
    char* v5;
    char* v6;
    int v7;
    char* v8;
    int v9;
    int v10;
    int a3a;

    v5 = &a2[8 * a4 * a3];
    v6 = &a1[4 * a4 * a3];
    if (a4)
    {
        v7 = 4 * a3;
        a3a = a4;
        do
        {
            if (a3)
            {
                v8 = &v5[-v7];
                v9 = a3;
                do
                {
                    v10 = *((uint32_t*)v6 - 1);
                    v6 -= 4;
                    v5 -= 4;
                    *(uint32_t*)v5 = v10;
                    v8 -= 4;
                    --v9;
                    *(uint32_t*)v8 = *(uint32_t*)v6;
                } while (v9);
            }
            v5 -= v7;
            --a3a;
        } while (a3a);
    }
}

void TEX_Expand8_XBy2(char* a1, char* a2, int a3, int a4)
{
    char* v4;
    char* i;
    char* v6;
    char v7;

    v4 = &a2[2 * a4 * a3];
    for (i = &a1[a4 * a3 - 1]; i > a1; *v4 = v7)
    {
        v6 = v4 - 1;
        *v6 = *i;
        v7 = *i;
        v4 = v6 - 1;
        --i;
    }
}

void TEX_Expand8_YBy2(char* a1, char* a2, int a3, int a4)
{
    int v4;
    char* v5;
    char* i;
    char* v7;
    int v8;
    char v9;

    v4 = a4;
    v5 = &a2[2 * a4 * a3 - 1];
    for (i = &a1[a4 * a3 - 1]; v4; --v4)
    {
        if (a3)
        {
            v7 = &v5[-a3];
            v8 = a3;
            do
            {
                *v5 = *i;
                v9 = *i--;
                --v5;
                *v7-- = v9;
                --v8;
            } while (v8);
        }
        v5 -= a3;
    }
}

void TEX_Expand4_XBy2(char* a1, char* a2, int a3, int a4)
{
    char* v4;
    char* i; 
    char* v6;

    v4 = &a2[a4 * a3];
    for (i = &a1[((unsigned int)(a4 * a3) >> 1) - 1]; i > a1; --i)
    {
        *v4 = (16 * *i) | *i & 0xF;
        v6 = v4 - 1;
        *v6 = ((uint8_t)*i >> 4) | *i & 0xF0;
        v4 = v6 - 1;
    }
}

void TEX_Expand4_YBy2(char* a1, char* a2, unsigned int a3, int a4)
{
    int v4;
    char* v5;
    int v6;
    char* i;
    char* v8;
    unsigned int v9;
    char v10;

    v4 = a4;
    v5 = &a2[a4 * a3 - 1];
    v6 = a3 >> 1;
    for (i = &a1[((a4 * a3) >> 1) - 1]; v4; --v4)
    {
        if (v6)
        {
            v8 = &v5[-v6];
            v9 = a3 >> 1;
            do
            {
                *v5 = *i;
                v10 = *i--;
                --v5;
                *v8-- = v10;
                --v9;
            } while (v9);
        }
        v5 -= v6;
    }
}

void TEX_Expand24_XBy2(char* a1, char* a2, int a3, int a4)
{
    char* v4;
    char* i;
    char* v6;
    char v7;
    char* v8;
    char v9;
    char v10;

    v4 = &a2[6 * a4 * a3];
    for (i = &a1[2 * a4 * a3 - 1 + a4 * a3]; i > a1; *v4 = v10)
    {
        *(v4 - 1) = *i;
        v6 = v4 - 1;
        *(v6 - 1) = *(i - 1);
        v6 -= 2;
        *v6 = *(i - 2);
        v7 = *i;
        --v6;
        v8 = i - 1;
        *v6 = v7;
        v9 = *v8;
        --v6;
        --v8;
        *v6 = v9;
        v10 = *v8;
        v4 = v6 - 1;
        i = v8 - 1;
    }
}

void TEX_Expand24_YBy2(char* a1, char* a2, int a3, int a4)
{
    int v4;
    int v5;
    int v6;
    char* v7;
    char* i;
    char* v9;
    int v10;
    char v11;

    v4 = a4;
    v5 = 3 * a3;
    v6 = a4 * 3 * a3;
    v7 = &a2[2 * v6 - 1];
    for (i = &a1[v6 - 1]; v4; --v4)
    {
        if (v5)
        {
            v9 = &v7[-v5];
            v10 = 3 * a3;
            do
            {
                *v7 = *i;
                v11 = *i--;
                --v7;
                *v9-- = v11;
                --v10;
            } while (v10);
        }
        v7 -= v5;
    }
}

void TEX_Convert_32SwapRB(int* a1, int a2)
{
    int i;

    for (i = a2; i; --i)
    {
        *a1 = *a1 & 0xFF00FF00 | *((uint8_t*)a1 + 2) | ((uint8_t)*a1 << 16);
        ++a1;
    }
}

void TEX_Convert_24To32(char* a1, int a2)
{
    int v2;
    char* v3;
    int* i;
    int v5;
    int v6;

    v2 = a2;
    v3 = &a1[2 * a2 + a2];
    for (i = (int*)&a1[4 * a2]; v2; *i = ((uint8_t)v3[2] + ((v6 + (v5 << 8)) << 8)) | 0xFF000000)
    {
        v5 = (uint8_t)*(v3 - 3);
        v6 = (uint8_t)*(v3 - 2);
        v3 -= 3;
        --i;
        --v2;
    }
}

void TEX_Convert_24To32SwapRB(char* a1, int a2)
{
    int v2;
    char* v3;
    int* i;
    int v5;
    int v6;

    v2 = a2;
    v3 = &a1[2 * a2 + a2];
    for (i = (int*)&a1[4 * a2]; v2; *i = ((uint8_t)*v3 + ((v6 + (v5 << 8)) << 8)) | 0xFF000000)
    {
        v5 = (uint8_t)*(v3 - 1);
        v6 = (uint8_t)*(v3 - 2);
        v3 -= 3;
        --i;
        --v2;
    }
}

void TEX_Convert_4To32(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;
    for (i = num; i; i -= 2)
    {
        *dest = paletteData[*src >> 4];
        dest += 2;
        *(dest - 1) = paletteData[*src++ & 0xF];
    }
}

void TEX_Convert_8To32(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;
 
    for (i = num; i; --i)
        *dest++ = paletteData[*src++];

}

void TEX_Convert_4To32SwapRB(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;

    for (i = num; i; i -= 2)
    {
        dest[0] = paletteData[(uint8_t)*src >> 4] & 0xFF00FF00 | ((uint8_t)paletteData[(uint8_t)*src >> 4] << 16) | (uint8_t)((paletteData[(uint8_t)*src >> 4]) >> 16);
        dest[1] = paletteData[*src & 0xF] & 0xFF00FF00 | ((uint8_t)paletteData[*src & 0xF] << 16) | (uint8_t)((paletteData[*src & 0xF]) >> 16);
        dest += 2;
        ++src;
    }

}

void TEX_Convert_8To32SwapRB(int* dest, unsigned char* src, int* paletteData, int num)
{
    int i;

    if (!paletteData) return;

    for (i = num; i; --i)
    {
        *dest = paletteData[(uint8_t)*src] & 0xFF00FF00 | ((uint8_t)paletteData[(uint8_t)*src] << 16) | (uint8_t)((paletteData[(uint8_t)*src]) >> 16);
        ++src;
        ++dest;
    }
}

unsigned int TEX_ul_GetPower2Up(unsigned int a1)
{
    unsigned int result;
    unsigned int i;

    result = 1;
    for (i = 0; i < 30; ++i)
    {
        if (a1 <= result)
            break;
        result *= 2;
    }
    return result;
}

char TEX_uc_GetMaskShift(unsigned int a1)
{
    unsigned int v1;
    char result;

    v1 = a1;
    result = 0;
    if (a1 && (a1 & 1) == 0)
    {
        do
        {
            v1 >>= 1;
            ++result;
        } while ((v1 & 1) == 0);
    }
    return result;
}

void TEX_GetMaskParams(uint32_t a1, uint8_t* a2, uint8_t* a3)
{
    uint32_t v3;
    uint8_t v4;
    uint8_t v5;

    v3 = a1;
    *a2 = 0;
    *a3 = 0;
    if (a1)
    {
        if ((a1 & 1) == 0)
        {
            v4 = 0;
            do
            {
                v3 >>= 1;
                ++v4;
            } while ((v3 & 1) == 0);
            *a3 = v4;
        }
        v5 = *a2;
        do
        {
            v3 >>= 1;
            ++v5;
        } while ((v3 & 1) != 0);
        *a2 = v5;
    }
}

void TEX_GetColorConvertParams(int a1, unsigned int a2, int* a3, unsigned int* a4)
{
    unsigned int v4;
    int v5;
    unsigned int v6;
    unsigned int* v7;
    uint8_t v8;
    uint8_t v9;
    uint8_t v10;
    uint8_t a1a;

    if (a1 && (v4 = a2) != 0)
    {
        TEX_GetMaskParams(a1, &a1a, &v10);
        TEX_GetMaskParams(v4, &v8, &v9);
        if (a1a >= v8)
        {
            v5 = a1a - v8 - v9 + v10;
            *a3 = v5;
            if (v5 <= 0)
                v6 = v4 >> -(char)v5;
            else
                v6 = v4 << v5;
            *a4 = v6;
        }
    }
    else
    {
        v7 = a4;
        *a3 = 0;
        *v7 = 0;
    }
}

int TEX_l_ConvertPF(struct sEngTexIntermediateStruct* a1, struct sEngTexIntermediateStruct* a2)
{
    tEngTexIntermediateStruct* v2;
    uint8_t v3;
    char* v4;
    char* v5;
    char v6;
    int v7; 
    BOOL v8;
    uint16_t v9;
    int v10;
    int v11;
    int v12;
    unsigned int v13;
    unsigned int v14;
    unsigned int v15;
    int v16;
    unsigned int v17;
    int v18;
    unsigned int v19;
    unsigned int v20;
    int v21;
    int v22;
    char v25;
    int v26;
    int v27;
    int a3;
    unsigned int a4;
    unsigned int v30;
    unsigned int v31;
    unsigned int v32;
    int v33;
    int v34;
    int v35;
    int v36;
    int v37;
    int v38;
    char v39;

    v2 = a1;
    TEX_GetColorConvertParams(a1->redMask, a2->redMask, &a3, &a4);
    TEX_GetColorConvertParams(a1->greenMask, a2->greenMask, &v26, &v30);
    TEX_GetColorConvertParams(a1->blueMask, a2->blueMask, &v27, &v31);
    TEX_GetColorConvertParams(a1->alphaMask, a2->alphaMask, &v35, &v32);
    v3 = a2->colorDepth1;
    v4 = a1->imgDataPtr;
    v5 = a2->imgDataPtr;
    v6 = a1->colorDepth1 >> 3;
    v25 = v6;
    v39 = v3 >> 3;
    if (v3 == 16)
        v7 = 0xFFFF;
    else
        v7 = v3 != 24 ? -1 : 0xFFFFFF;
    v8 = a1->realHeight == 0;
    v33 = v7;
    v36 = ~v7;
    v34 = 0;
    if (!v8)
    {
        v9 = a1->realWidth;
        v10 = a3;
        do
        {
            v11 = 0;
            if (v9)
            {
                v37 = v39;
                v38 = v6;
                do
                {
                    v12 = *(uint32_t*)v4;
                    v13 = a4 & *(uint32_t*)v4;
                    if (v10 < 0)
                        v14 = v13 << -(char)v10;
                    else
                        v14 = v13 >> v10;
                    if (v26 < 0)
                        v15 = (v30 & v12) << -(char)v26;
                    else
                        v15 = (v30 & v12) >> v26;
                    v16 = v15 | v14;
                    if (v27 < 0)
                        v17 = (v31 & v12) << -(char)v27;
                    else
                        v17 = (v31 & v12) >> v27;
                    v18 = v17 | v16;
                    v19 = v32 & v12;
                    if (v35 < 0)
                        v20 = v19 << -(char)v35;
                    else
                        v20 = v19 >> v35;
                    v21 = (v20 | v18) & v33;
                    v22 = v37;
                    v4 += v38;
                    v2 = a1;
                    *(uint32_t*)v5 = v21 | v36 & *(uint32_t*)v5;
                    v10 = a3;
                    v5 += v22;
                    v9 = a1->realWidth;
                    ++v11;
                } while (v11 < v9);
                v6 = v25;
            }
        } while (++v34 < v2->realHeight);
    }
    return 1;
}

int TEX_l_ConvertSize(tEngTexIntermediateStruct* a1, tEngTexIntermediateStruct* a2)
{
    tEngTexIntermediateStruct* v2;
    unsigned int v3;
    unsigned int v4;
    unsigned int* i;
    int v6;
    BOOL v7;
    int v8;
    unsigned int* v9;
    unsigned int v10;
    int v11;
    unsigned int* v12;
    unsigned int j;
    int v14;
    unsigned int v15;
    unsigned int* v16;
    uint8_t v17;
    uint8_t v18;
    unsigned int v19;
    unsigned int v20;
    int* v21;
    int* v22;
    unsigned int v23;
    unsigned int v24;
    unsigned int v25;
    unsigned int v26;
    int v27;
    unsigned int v28;
    char* v29;
    int v30;
    int v31;
    unsigned int v32;
    BOOL v33;
    int* v35;
    uint8_t v36;
    uint8_t v37;
    char v38;
    char v39;
    char v40;
    char v41;
    int v42;
    int v43;
    unsigned int v44;
    unsigned int v45;
    int* k;
    int v47;
    int v48;
    char* v49;
    uint32_t* v50;
    unsigned int v51;
    unsigned int v52;
    unsigned int v53;
    unsigned int v54;
    unsigned int v55;
    uint8_t v56;
    int v57;
    int v58;
    uint32_t* Block;
    int v60;
    unsigned int v61;
    unsigned int v62;
    uint8_t v63;
    uint8_t v64;
    uint8_t v65;
    unsigned int v66;
    int v67;
    unsigned int v68;
    int v69;
    char* v70;
    int v71;
    int v72;
    int v73;
    int v74;
    unsigned int v75;
    int v76;
    int v77;
    int v78;
    int v79;

    v2 = a2;
    Block = malloc(16 * a2->realWidth);
    v50 = malloc(16 * a2->realHeight);
    v3 = 0;
    v4 = 0;
    v44 = 0;
    v42 = (a1->realWidth << 8) / a2->realWidth;
    for (i = Block; v44 < a2->realWidth; ++v44)
    {
        *i = v3;
        v6 = 256 - (uint8_t)v4;
        v4 += v42;
        i[1] = v6;
        v3 = v4 >> 8;
        v7 = v4 >> 8 == *i;
        i[2] = v4 >> 8;
        if (v7)
            i[1] += (uint8_t)v4 - 256;
        else
            i[3] = (uint8_t)v4;
        i += 4;
    }
    v8 = a1->realWidth;
    v9 = i - 4;
    v9[3] = 255;
    v9[2] = v8 - 1;
    v10 = 0;
    v47 = 0;
    v11 = (a1->realHeight << 8) / a2->realHeight;
    v12 = v50;
    if (a2->realHeight)
    {
        for (j = 0; ; j = v10 >> 8)
        {
            *v12 = j;
            v14 = (uint8_t)v10;
            v10 += v11;
            v12[1] = 256 - v14;
            v15 = *v12;
            v12[2] = v10 >> 8;
            if (v10 >> 8 == v15)
                v12[1] += (uint8_t)v10 - 256;
            else
                v12[3] = (uint8_t)v10;
            v12 += 4;
            if (++v47 >= a2->realHeight)
                break;
        }
    }
    v16 = v12 - 4;
    v16[2] = a1->realHeight - 1;
    v16[3] = 255;
    v71 = a1->alphaMask;
    v38 = TEX_uc_GetMaskShift(v71);
    v72 = a1->redMask;
    v39 = TEX_uc_GetMaskShift(v72);
    v73 = a1->greenMask;
    v40 = TEX_uc_GetMaskShift(v73);
    v74 = a1->blueMask;
    v41 = TEX_uc_GetMaskShift(v74);
    v76 = a2->alphaMask;
    v17 = TEX_uc_GetMaskShift(v76);
    v77 = a2->redMask;
    v36 = TEX_uc_GetMaskShift(v77);
    v78 = a2->greenMask;
    v37 = TEX_uc_GetMaskShift(v78);
    v79 = a2->blueMask;
    v18 = TEX_uc_GetMaskShift(v79);
    v19 = v42 * v11;
    v56 = v36;
    v63 = v17;
    v67 = ~(v18 | v37 | v36 | v17);
    v65 = v18;
    v69 = a2->colorDepth1 >> 3;
    v49 = a2->imgDataPtr;
    v70 = a1->imgDataPtr;
    v64 = v37;
    v75 = v19;
    v57 = a1->colorDepth1 >> 3;
    v60 = v57 * a1->realWidth;
    v48 = 0;
    if (a2->realHeight)
    {
        v20 = a2->realWidth;
        v21 = v50 + 1;
        v35 = v50 + 1;
        do
        {
            v45 = 0;
            if (v20)
            {
                v22 = Block + 1;
                for (k = Block + 1; ; v22 = k)
                {
                    v23 = *(v21 - 1);
                    v54 = 0;
                    v53 = 0;
                    v52 = 0;
                    v51 = 0;
                    v24 = v21[1];
                    v25 = v23;
                    v62 = v23;
                    v61 = v23;
                    v68 = v24;
                    if (v23 <= v24)
                    {
                        v55 = *(v22 - 1);
                        v26 = v22[1];
                        v27 = v23 * v60;
                        v66 = v26;
                        v58 = v23 * v60;
                        while (1)
                        {
                            if (v25 == v23)
                            {
                                v43 = *v21;
                            }
                            else if (v25 == v24)
                            {
                                v43 = v21[2];
                            }
                            else
                            {
                                v43 = 256;
                            }
                            v28 = v55;
                            if (v55 <= v26)
                            {
                                v29 = &v70[v27 + v55 * v57];
                                do
                                {
                                    if (v28 == v55)
                                    {
                                        v30 = *k;
                                    }
                                    else if (v28 == v26)
                                    {
                                        v30 = k[2];
                                    }
                                    else
                                    {
                                        v30 = 256;
                                    }
                                    v31 = v43 * v30;
                                    v51 += v31 * (((unsigned int)v71 & *(uint32_t*)v29) >> v38);
                                    v52 += v31 * (((unsigned int)v72 & *(uint32_t*)v29) >> v39);
                                    v53 += v31 * (((unsigned int)v73 & *(uint32_t*)v29) >> v40);
                                    v32 = v31 * (((unsigned int)v74 & *(uint32_t*)v29) >> v41);
                                    v26 = v66;
                                    ++v28;
                                    v29 += v57;
                                    v54 += v32;
                                } while (v28 <= v66);
                                v25 = v61;
                                v19 = v75;
                                v24 = v68;
                                v27 = v58;
                                v21 = v35;
                            }
                            ++v25;
                            v27 += v60;
                            v61 = v25;
                            v58 = v27;
                            if (v25 > v24)
                                break;
                            v23 = v62;
                        }
                    }
                    v21 = v35;
                    *(unsigned int*)v49 = v67 & *(unsigned int*)v49 | v79 & ((v54 / v19) << v65) | v78 & ((v53 / v19) << v64) | v77 & ((v52 / v19) << v56) | v76 & ((v51 / v19) << v63);
                    v49 += v69;
                    v20 = a2->realWidth;
                    ++v45;
                    k += 4;
                    if (v45 >= v20)
                        break;
                }
                v2 = a2;
            }
            v21 += 4;
            v33 = ++v48 < v2->realHeight;
            v35 = v21;
        } while (v33);
    }
    free(Block);
    free(v50);
    return 1;
}

void TEX_l_ConvertSize_24To32(struct sEngTexIntermediateStruct* a1, struct sEngTexIntermediateStruct* a2)
{
    int v2;
    int v3;
    unsigned int* v4;
    double v5;
    unsigned int v6;
    int v7;
    double v8;
    char* v9;
    int i;
    float v11;
    float v12;
    int v13;
    int v14[1024];

    v2 = a2->realWidth;
    v13 = a1->realWidth;
    v3 = 0;
    v4 = (unsigned int*)a2->imgDataPtr;
    v5 = 0.0;
    if (a2->realWidth)
    {
        do
        {
            v6 = (unsigned int)v5;
            v11 = (float)((double)v13 / (double)v2);
            v5 = v5 + v11;
            v14[v3++] = 3 * v6;
        } while (v3 < v2);
    }
    v7 = 0;
    v12 = (float)((double)a1->realHeight / (double)a2->realHeight);
    v8 = 0.0;
    if (a2->realHeight)
    {
        do
        {
            v9 = &a1->imgDataPtr[3 * v13 * (unsigned int)v8];
            for (i = 0; i < v2; ++i)
            {
                *v4 = *(uint32_t*)&v9[v14[i]] & 0xFFFFFF;
                v2 = a2->realWidth;
                ++v4;
            }
            v8 = v8 + v12;
            ++v7;
        } while (v7 < a2->realHeight);
    }
}

void TEX_File_VSwap(char* a1, char* a2, int a3, int a4, int a5)
{
    unsigned int v5;
    char* v6;
    int a3a;

    v5 = a5 * a3;
    v6 = &a2[a5 * a3 * (a4 - 1)];
    if (a4)
    {
        a3a = a4;
        do
        {
            memcpy(a1, v6, v5);
            a1 += v5;
            v6 -= v5;
            --a3a;
        } while (a3a);
    }
}

void TEX_l_File_TgaUncompress(char a1, char* a2, char* a3, int a4)
{
    int v4;
    char v7;
    uint8_t v8;
    int v9;
    int v10;
    char* v11;
    int v12;
    char v13;
    char* v14;
    char* v15;
    uint8_t v16;

    v4 = a4;
    while (v4 > 0)
    {
        v7 = *a3;
        v8 = (*a3++ & 0x7F) + 1;
        v16 = v8;
        if (v7 >= 0)
        {
            if (v8)
            {
                v12 = v8;
                do
                {
                    if (a1 == 24)
                    {
                        *a2 = *a3;
                        v13 = a3[1];
                        v14 = a2 + 1;
                        v15 = a3 + 1;
                        *v14++ = v13;
                        *v14 = v15[1];
                        a2 = v14 + 1;
                        a3 = v15 + 2;
                        v4 -= 3;
                    }
                    else
                    {
                        *(uint32_t*)a2 = *(uint32_t*)a3;
                        a3 += 4;
                        a2 += 4;
                        v4 -= 4;
                    }
                    --v12;
                } while (v12);
            }
        }
        else
        {
            v9 = *(uint32_t*)a3;
            if (a1 == 24)
                a3 += 3;
            else
                a3 += 4;
            if (v16)
            {
                v10 = v16;
                do
                {
                    if (a1 == 24)
                    {
                        *a2 = v9;
                        v11 = a2 + 1;
                        *v11++ = ((v9 >> 8) & 0xff);
                        *v11 = ((v9 >> 16) & 0xff);
                        a2 = v11 + 1;
                        v4 -= 3;
                    }
                    else
                    {
                        *(uint32_t*)a2 = v9;
                        v4 -= 4;
                    }
                    --v10;
                } while (v10);
            }
        }
    }
}

void TEX_File_DevelopParams(tEngTexIntermediateStruct* a1)
{
    uint16_t v1;
    int v2;
    uint8_t v3;
    uint8_t v4;
    uint8_t v5;

    v1 = a1->tp.imageDataHeight;
    a1->realWidth = a1->tp.imageDataWidth;
    v2 = a1->tp.imageDataColorDepth - 16;
    a1->realHeight = v1;
    switch (v2)
    {
    case 0:
        a1->colorDepth1 = 32;
        a1->alphaMask = 0xFF000000;
        goto LABEL_4;
    case 16:
        a1->colorDepth1 = 24;
        a1->alphaMask = 0;
    LABEL_4:
        v3 = a1->colorDepth1;
        a1->redMask = 0xFF0000;
        a1->greenMask = 0xFF00;
        a1->blueMask = 0xFF;
        a1->colorDepth2 = v3;
        return;
    case 32:
        a1->colorDepth1 = 16;
        v4 = a1->colorDepth1;
        a1->alphaMask = 0x8000;
        a1->redMask = 0x7C00;
        a1->greenMask = 0x3E0;
        a1->blueMask = 0x1F;
        a1->colorDepth2 = v4;
        return;
    case 33:
        a1->colorDepth1 = 16;
        v5 = a1->colorDepth1;
        a1->alphaMask = 0xF000;
        a1->redMask = 0xF00;
        a1->greenMask = 0xF0;
        a1->blueMask = 0xF;
        a1->colorDepth2 = v5;
        return;
    case 48:
        a1->colorDepth1 = 8;
        a1->colorDepth2 = a1->colorDepth1;
        return;
    case 64:
        a1->colorDepth1 = 4;
        goto LABEL_9;
    default:
    LABEL_9:
        a1->colorDepth2 = a1->colorDepth1;
        return;
    }
}

int TEX_l_MemoryNeeded(tEngTexIntermediateStruct* texIM)
{    
    int i, tmp, ret;

    ret = (texIM->realHeightResizeToThis * texIM->realWidthResizeToThis * texIM->colorDepth2) / 8;
   
    if ((texIM->tp.flagsUnkAndProceduralUnkb1 & 8) != 0)
    {
        tmp = (texIM->realHeightResizeToThis * texIM->realWidthResizeToThis * texIM->colorDepth2) / 8;
        if (texIM->realWidthResizeToThis < texIM->realHeightResizeToThis)
            texIM->realHeightResizeToThis = texIM->realWidthResizeToThis;
        for (i = texIM->realHeightResizeToThis / 2; i; i /= 2)
        {
            tmp /= 4;
            ret += tmp;
        }

       while(tmp > 4) {
            tmp /= 2;
            ret += tmp;
        }
    }
    return ret;
}

BOOL TEX_l_DimensionIsGood(tEngTexIntermediateStruct* texIM, int minSize, int maxSize, int onlySquareDimensionIsAccepted, tEngTexIntermediateStruct* texIMRet)
{
    uint16_t heightPow2;
    uint16_t widthPow2;

    memcpy(texIMRet, texIM, sizeof(tEngTexIntermediateStruct));
    texIMRet->descriptionPtr = 0;
    texIMRet->imgDataPtr = NULL;

    heightPow2 = TEX_ul_GetPower2Up(texIMRet->realHeight);
    texIMRet->realHeight = heightPow2;
    if (heightPow2 < minSize)
        texIMRet->realHeight = minSize;
    if (texIMRet->realHeight > maxSize)
        texIMRet->realHeight = maxSize;
    widthPow2 = TEX_ul_GetPower2Up(texIMRet->realWidth);
    texIMRet->realWidth = widthPow2;
    if (widthPow2 < minSize)
        texIMRet->realWidth = minSize;
    if (texIMRet->realWidth > maxSize)
        texIMRet->realWidth = maxSize;
    if (!onlySquareDimensionIsAccepted)
    {
        if (texIMRet->realWidth >= texIMRet->realHeight)
            texIMRet->realHeight = texIMRet->realWidth;
        else
            texIMRet->realWidth = texIMRet->realHeight;
    }
    return texIM->realHeight == texIMRet->realHeight && texIM->realWidth == texIMRet->realWidth;
}

void TEX_File_TransformParams(tEngTexIntermediateStructTexParams* a1)
{
    int16_t v1;
    int v2;

    v1 = a1->uuuu;
    a1->unk = *(uint32_t*)&a1->flagsUnkAndProceduralUnkb1; // TODO FIXME? ez itt átmásol 4 bájtot (flagsUnkAndProceduralUnkb1, imageDataColorDepth, type)
    v2 = (unsigned int)a1->uuuu >> 28;
    a1->flagsUnkAndProceduralUnkb1 = v1;
    a1->type = v2;
    a1->imageDataColorDepth = 0;
}

BOOL TEX_b_File_CheckParams(tEngTexIntermediateStructTexParams* a1)
{
    BOOL result;

    if (a1->magicParams.magic3 == 0xC0DEC0DE && a1->magicParams.magic2 == 0x00FF00FF)
        result = a1->magicParams.magic1 == 0xCAD01234;
    else
        result = 0;
    return result;
}

int TEX_l_ConvertSizeFast(tEngTexIntermediateStruct* a1)
{
    unsigned int v1;
    unsigned int v3;
    unsigned int v4;
    unsigned int v5;
    unsigned int v6;
    int v7;
    char* v8;
    char* v9;
    uint8_t v10;

    v1 = a1->realWidth;
    if (TEX_ul_GetPower2Up(v1) != v1)
        return 0;
    v3 = a1->realWidthResizeToThis;
    if (TEX_ul_GetPower2Up(v3) != v3)
        return 0;
    v4 = a1->realHeight;
    if (TEX_ul_GetPower2Up(v4) != v4)
        return 0;
    v5 = a1->realHeightResizeToThis;
    if (TEX_ul_GetPower2Up(v5) != v5)
        return 0;
    v6 = v3 * v5;
    if (v6 <= a1->realWidth * v4)
    {
        v8 = 0;
    }
    else
    {
        v7 = v6 * a1->colorDepth1;
        v8 = a1->imgDataPtr;
        v9 = (char*)MEM_p_AllocAligned(v7, 64);
        a1->imgDataPtr = v9;
        MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetAligned(v9); //MEM_GetBlockSizeBySize(*((void**)v9 - 1));
    }
    v10 = a1->colorDepth1;
    if (v10 != 24)
    {
        switch (v10)
        {
        case 0x20u:
            for (; a1->realWidth > a1->realWidthResizeToThis; a1->realWidth >>= 1)
            {
                if (v8)
                {
                    TEX_Compress_Xo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;                    
                }
                else
                {
                    TEX_Compress_Xo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realHeight > a1->realHeightResizeToThis; a1->realHeight >>= 1)
            {
                if (v8)
                {
                    TEX_Compress_Yo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Compress_Yo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realWidth < a1->realWidthResizeToThis; a1->realWidth *= 2)
            {
                if (v8)
                {
                    TEX_Expand_XBy2((int*)v8, (int *)a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Expand_XBy2((int *)a1->imgDataPtr, (int*)a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            if (a1->realHeight < a1->realHeightResizeToThis)
            {
                do
                {
                    if (v8)
                    {
                        TEX_Expand_YBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                        MEM_FreeAlign(v8);
                        v8 = NULL;
                    }
                    else
                    {
                        TEX_Expand_YBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    }
                    a1->realHeight *= 2;
                } while (a1->realHeight < a1->realHeightResizeToThis);
                return 1;
            }
            return 1;
        case 8u:
            for (; a1->realWidth > a1->realWidthResizeToThis; a1->realWidth >>= 1)
            {
                if (v8)
                {
                    TEX_Compress8_Xo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Compress8_Xo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realHeight > a1->realHeightResizeToThis; a1->realHeight >>= 1)
            {
                if (v8)
                {
                    TEX_Compress8_Yo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Compress8_Yo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realWidth < a1->realWidthResizeToThis; a1->realWidth *= 2)
            {
                if (v8)
                {
                    TEX_Expand8_XBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Expand8_XBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            if (a1->realHeight < a1->realHeightResizeToThis)
            {
                do
                {
                    if (v8)
                    {
                        TEX_Expand8_YBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                        MEM_FreeAlign(v8);
                        v8 = NULL;
                    }
                    else
                    {
                        TEX_Expand8_YBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    }
                    a1->realHeight *= 2;
                } while (a1->realHeight < a1->realHeightResizeToThis);
                return 1;
            }
            return 1;
        case 4u:
            for (; a1->realWidth > a1->realWidthResizeToThis; a1->realWidth >>= 1)
            {
                if (v8)
                {
                    TEX_Compress4_Xo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Compress4_Xo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realHeight > a1->realHeightResizeToThis; a1->realHeight >>= 1)
            {
                if (v8)
                {
                    TEX_Compress4_Yo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Compress4_Yo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realWidth < a1->realWidthResizeToThis; a1->realWidth *= 2)
            {
                if (v8)
                {
                    TEX_Expand4_XBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Expand4_XBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            for (; a1->realHeight < a1->realHeightResizeToThis; a1->realHeight *= 2)
            {
                if (v8)
                {
                    TEX_Expand4_YBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
                    MEM_FreeAlign(v8);
                    v8 = NULL;
                }
                else
                {
                    TEX_Expand4_YBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
                }
            }
            return 1;
        }
        return 0;
    }
    for (; a1->realWidth > a1->realWidthResizeToThis; a1->realWidth >>= 1)
    {
        if (v8)
        {
            TEX_Compress24_Xo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
            MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
            MEM_FreeAlign(v8);
            v8 = NULL;
        }
        else
        {
            TEX_Compress24_Xo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
        }
    }
    for (; a1->realHeight > a1->realHeightResizeToThis; a1->realHeight >>= 1)
    {
        if (v8)
        {
            TEX_Compress24_Yo2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
            MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
            MEM_FreeAlign(v8);
            v8 = NULL;
        }
        else
        {
            TEX_Compress24_Yo2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
        }
    }
    for (; a1->realWidth < a1->realWidthResizeToThis; a1->realWidth *= 2)
    {
        if (v8)
        {
            TEX_Expand24_XBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
            MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
            MEM_FreeAlign(v8);
            v8 = NULL;
        }
        else
        {
            TEX_Expand24_XBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
        }
    }
    if (a1->realHeight >= a1->realHeightResizeToThis)
        return 1;
    do
    {
        if (v8)
        {
            TEX_Expand24_YBy2(v8, a1->imgDataPtr, a1->realWidth, a1->realHeight);
            MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v8);
            MEM_FreeAlign(v8);
            v8 = NULL;
        }
        else
        {
            TEX_Expand24_YBy2(a1->imgDataPtr, a1->imgDataPtr, a1->realWidth, a1->realHeight);
        }
        a1->realHeight *= 2;
    } while (a1->realHeight < a1->realHeightResizeToThis);
    return 1;
}

void TEX_File_LoadPalette(tEngPaletteElem* a1, int a2)
{
    int v2;
    uint8_t v3;
    unsigned int v4;
    uint8_t v5;
    uint8_t* v6;
    uint8_t* i;
    int v8; 
    uint16_t v9;
    uint8_t* v10;
    int v11;
    uint8_t* v12;
    uint8_t* v13;
    char v14;
    int v15; 
    char v16;
    int v17; 
    int v18; 
    char* v19;

    v2 = BIG_ul_SearchKeyToPos(a1->key);
    if (v2 == -1)
        return;
    v19 = BIG_pc_ReadFileTmp(v2, &v18);
    if (v18 == 1024)
    {
        v3 = a1->flags | 2;
        v4 = 1024;
    }
    else
    {
        if (v18 == 768)
        {
            v4 = 1024;
            goto LABEL_12;
        }
        if (v18 == 64)
        {
            v3 = a1->flags | 3;
        }
        else
        {
            v5 = a1->flags;
            if (v18 != 48)
            {
                a1->flags = v5 | 4;
                return;
            }
            v3 = v5 | 1;
        }
        v4 = 64;
    }
    a1->flags = v3;
LABEL_12:
    v6 = (uint8_t*)MEM_p_AllocTmp(v4);
    a1->paletteData = v6;
    MEM_gst_MemoryInfo.usedMemoryByTextures += *((uint32_t*)v6 - 1); // TODO FIXME
    LOA_ReadCharArray(&v19, (char*)a1->paletteData, v18, 1);
    if ((a1->flags & 2) != 0)
    {
        if (a2)
        {
            for (i = &a1->paletteData[4 * (v4 >> 2)]; v18; *(uint32_t*)i = i[2] | ((v8 | (v9 << 8)) << 8))
            {
                v18 -= 4;
                v8 = *(i - 3);
                v9 = (v9 & 0xff) | ((*(i - 1)) << 8); // HIBYTE(v9) = *(i - 1);
                i -= 4;
                v9 = (v9 & 0xff00) | *i; // LOBYTE(v9) = *i;
            }
        }
    }
    else
    {
        v10 = a1->paletteData;
        v11 = v18;
        v12 = &v10[4 * (v4 >> 2)];
        v13 = &v10[v18];
        if (a2)
        {
            for (; v18; v11 = v18)
            {
                v18 = v11 - 3;
                v14 = *(v13 - 3);
                v15 = *(v13 - 2);
                v13 -= 3;
                v12 -= 4;
                *(uint32_t*)v12 = v13[2] | ((v15 | ((*(uint32_t*)&v14 | 0xFFFFFF00) << 8)) << 8);
            }
        }
        else
        {
            for (; v18; v11 = v18)
            {
                v18 = v11 - 3;
                v16 = *(v13 - 1);
                v17 = *(v13 - 2);
                v13 -= 3;
                v12 -= 4;
                *(uint32_t*)v12 = *v13 | ((v17 | ((*(uint32_t*)&v16 | 0xFFFFFF00) << 8)) << 8);
            }
        }
    }
}

void TEX_Manager_ComputeCompression(struct sTexManagerStruct* a1, struct sEngTexIntermediateStruct* tims, int timsIdx, char a4)
{
    tEngTexIntermediateStruct* v4;
    int* v5;
    int* v6;
    int v7;
    tEngTexIntermediateStruct* v8;
    unsigned int v9;
    int v10;
    uint16_t v11;
    uint8_t v12;
    uint16_t v13;
    uint8_t v14;
    int v15;
    int v16;
    tEngTexIntermediateStruct* v17;
    int v18;
    uint16_t v19;
    uint16_t v20;
    uint16_t v21;
    int v22;
    int* v23;
    int v24;
    int* v25;
    int v26;
    unsigned int v27;
    tEngTexIntermediateStruct v28;

    v4 = &tims[timsIdx];
    if (a4)
    {
        v23 = &a1->texMemoryNeededUncompressedCase1;
        v5 = &a1->texMemoryNeededCompressedCase1Backup;
        v6 = &a1->texMemoryNeededCompressedCase1;
    }
    else
    {
        v23 = &a1->texMemoryNeededUncompressedCase2;
        v5 = &a1->texMemoryNeededCompressedCase2Backup;
        v6 = &a1->texMemoryNeededCompressedCase2;
    }
    v25 = v5;
    v7 = a1->flags & 1;
    *v23 = 0;
    if (tims < v4)
    {
        v8 = tims;
        v9 = (sizeof(tEngTexIntermediateStruct) * timsIdx - 1) / sizeof(tEngTexIntermediateStruct) + 1; // ezzel kezdeni vmit, todo
        v27 = v9;
        do
        {
            if ((v8->flags & 0x10) == 0)
            {
                v10 = (v8->tp.flagsUnkAndProceduralUnkb1 & 0x200) != 0 && (a1->flags & 0x200000) != 0;
                if (v10 == a4)
                {
                    v11 = v8->realWidth;
                    v12 = v8->colorDepth1;
                    v8->realHeightResizeToThis = v8->realHeight;
                    v8->realWidthResizeToThis = v11;
                    v8->colorDepth2 = v12;
                    if (!TEX_l_DimensionIsGood(v8, a1->i3, a1->i2, v7, &v28))
                    {
                        v13 = v28.realWidth;
                        v8->realHeightResizeToThis = v28.realHeight;
                        v8->realWidthResizeToThis = v13;
                    }
                    v14 = v8->colorDepth1;
                    if (v14 == 24 && (a1->flags & 0x200) == 0
                        || v14 == 16 && (a1->flags & 0x400) == 0
                        || (v14 == 8 || v14 == 4) && v8->tp.type == 7)
                    {
                        v8->colorDepth2 = 32;
                    }
                    *v23 += TEX_l_MemoryNeeded(v8);
                    v9 = v27;
                }
                v4 = &tims[timsIdx];
            }
            v8++;
            v27 = --v9;
        } while (v9);
    }
    v15 = *v25 - *v23;
    *v6 = v15;
    v24 = 0;
    v26 = v15;
    if (v15 < 0)
    {
        while (1)
        {
            v16 = al_QualityCompressionOrder[v24];
            if (tims < v4)
                break;
        LABEL_40:
            if (++v24 == 21)
            {
                v24 = 0;
                if (*v6 == v26)
                    return;
                v26 = *v6;
            }
            if (*v6 >= 0)
                return;
        }
        v17 = tims;
        while (1)
        {
            if ((v17->flags & 0x10) == 0 && (v17->tp.flagsUnkAndProceduralUnkb1 & 7) == v16)
            {
                v18 = al_QualityCompressionLimit[v16];
                if (v17->realHeightResizeToThis > v18 || v17->realWidthResizeToThis > v18)
                    break;
            }
        LABEL_39:
            v17++;
            if (v17 >= v4)
                goto LABEL_40;
        }
        *v6 += TEX_l_MemoryNeeded(v17);
        v19 = v17->realHeightResizeToThis;
        v20 = v17->realWidthResizeToThis;
        if (v19 >= v17->realWidthResizeToThis)
        {
            if (v19 > v17->realWidthResizeToThis)
            {
                v17->realHeightResizeToThis = v19 >> 1;
                goto LABEL_38;
            }
            v21 = v20 >> 1;
            v17->realHeightResizeToThis = v19 >> 1;
        }
        else
        {
            v21 = v20 >> 1;
        }
        v17->realWidthResizeToThis = v21;
    LABEL_38:
        v22 = *v6 - TEX_l_MemoryNeeded(v17);
        *v6 = v22;
        v4 = &tims[timsIdx];
        if (v22 > 0)
            goto LABEL_40;
        goto LABEL_39;
    }
}

void TEX_Manager_Init(struct sTexManagerStruct* a1)
{
    a1->keyLoadOrderIDs[0] = 3;
    a1->keyLoadOrderIDs[1] = 0;
    a1->keyLoadOrderIDs[2] = -1;
    a1->keyLoadOrderIDs[3] = -1;
    a1->i2 = 512;
    a1->i3 = 8;
    a1->texMemoryNeededCompressedCase2Backup = 0x1000000;
    a1->texMemoryNeededCompressedCase1Backup = 0x80000;
    a1->flags = 0x10d;
}

void TEX_Manager_Reinit(struct sTexManagerStruct* a1)
{
    int v1;

    v1 = a1->texMemoryNeededCompressedCase1Backup;
    a1->texMemoryNeededCompressedCase2 = a1->texMemoryNeededCompressedCase2Backup;
    a1->texMemoryNeededUncompressedCase2 = 0;
    a1->texMemoryNeededCompressedCase1 = v1;
    a1->texMemoryNeededUncompressedCase1 = 0;
    a1->paletteUsedMemory = 0;
    a1->timsIdx = 0;
    a1->elemsFlagsWithoutFlag2 = 0;
    a1->elemsFlagsWithFlag2 = 0;
}

int TEX_l_File_LoadTga(char* a1, tEngTexIntermediateStruct* a2)
{
    uint16_t v2;
    uint16_t v3;
    char v4;
    tEngTexIntermediateStruct* v5;
    int16_t v6;
    char* v7;
    BOOL v8;
    int v9;
    char* v11;
    unsigned int v12;
    char* v13;
    uint8_t v14;
    uint8_t v15;
    int16_t v16;
    char colorDepth;

    LOA_ReadChar(&a1, 0, 1);
    LOA_ReadChar(&a1, 0, 1);
    v14 = LOA_ReadChar(&a1, 0, 1);
    LOA_ReadShort(&a1, 0, 1);
    v2 = LOA_ReadShort(&a1, 0, 1);
    v15 = LOA_ReadChar(&a1, 0, 1);
    LOA_ReadShort(&a1, 0, 1);
    LOA_ReadShort(&a1, 0, 1);
    v3 = LOA_ReadShort(&a1, 0, 1);
    v16 = LOA_ReadShort(&a1, 0, 1);
    v4 = LOA_ReadChar(&a1, 0, 1);
    colorDepth = v4;
    LOA_ReadChar(&a1, 0, 1);
    v5 = a2;
    v6 = a2->flags;
    if ((a2->flags & 1) != 0)
    {
        a2->realWidth = v3;
        v5->realHeight = v16;
        v7 = a1;
        v8 = (v5->tp.flagsUnkAndProceduralUnkb1 & 8) == 0;
        v5->tgaUnkS1 = v2;
        v5->colorDepth2 = v4;
        v5->colorDepth1 = v4;
        v5->tgaUnk1b = v15;
        v5->tgaDataEndPtr = (int)v7;
        v5->tgaDataStartPtr = 0;
        if (!v8)
            v5->bmpUnkb1 |= 0x10u;
        switch (v4)
        {
        case 32:
            v5->alphaMask = 0xFF000000;
        LABEL_6:
            v5->redMask = 0xFF0000;
            v5->greenMask = 0xFF00;
            v5->blueMask = 0xFF;
            break;
        case 24:
            v5->redMask = 0xFF0000;
            v5->greenMask = 0xFF00;
            v5->blueMask = 0xFF;
            v5->alphaMask = 0;
            break;
        case 16:
            v5->alphaMask = 0;
            goto LABEL_6;
        case 8:
            v5->tgaDataStartPtr = (int)v7;
            v5->tgaDataEndPtr = (int)&v7[v2 * (v15 >> 3)];
            break;
        default:
            return 0;
        }
    }
    if ((v6 & 2) != 0)
    {
        v9 = v5->realWidth * v5->realHeight;
        if (!v9)
            return 0;
        LOA_ReadCharArray(&a1, 0, v2 * (v5->tgaUnk1b >> 3), 2);
        v11 = (char*)MEM_p_AllocTmp(v9 * (v5->colorDepth2 >> 3));
        v5->imgDataPtr = v11;
        MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetTmpAddressSpace(v11);
        v12 = v9 * (v5->colorDepth1 >> 3);
        v13 = v5->imgDataPtr;
        if (v14 >= 9u)
        {
            TEX_l_File_TgaUncompress(colorDepth, v13, a1, v9 * (v5->colorDepth1 >> 3));
            LOA_ReadCharArray(&a1, 0, v12, 1);
            return 1;
        }
        LOA_ReadCharArray(&a1, v13, v12, 1);
    }
    return 1;
}

int TEX_l_File_LoadBmp(tTexWeirdBMPHeader* a1, tEngTexIntermediateStruct* a2)
{
    int16_t v2;
    uint8_t v3;
    uint8_t v4;
    int v6;
    uint8_t v7;
    char* v8;

    v2 = a2->flags;
    if ((a2->flags & 1) != 0)
    {
        a2->realWidth = a1->width;
        a2->realHeight = a1->height;
        v3 = a1->colorDepth;
        a2->colorDepth2 = v3;
        a2->colorDepth1 = v3;
        if ((a2->tp.flagsUnkAndProceduralUnkb1 & 8) != 0)
            a2->bmpUnkb1 |= 0x10u;
        v4 = a2->colorDepth1;
        switch (v4)
        {
        case 32u:
            a2->alphaMask = 0xFF000000;
            a2->redMask = 0xFF0000;
            a2->greenMask = 0xFF00;
            a2->blueMask = 0xFF;
            break;
        case 24u:
            a2->redMask = 0xFF0000;
            a2->greenMask = 0xFF00;
            a2->blueMask = 0xFF;
            a2->alphaMask = 0;
            break;
        case 16u:
            a2->redMask = 0x7C00;
            a2->greenMask = 0x3E0;
            a2->blueMask = 0x1F;
            a2->alphaMask = 32768;
            break;
        case 8u:
            break;
        default:
            return 0;
        }
    }
    if ((v2 & 2) == 0)
        return 1;

    v6 = a2->realWidth * a2->realHeight;
    if (!v6)
        return 0;

    v7 = a2->colorDepth1;
    if (v7 != 32 && v7 != 24 && v7 != 16 && v7 != 8)
        return 0;

    v8 = (char*)MEM_p_AllocAligned(v6 * (a2->colorDepth2 >> 3), 64);
    a2->imgDataPtr = v8;
    MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetAligned(v8);
    memcpy(a2->imgDataPtr, a1 + sizeof(tTexWeirdBMPHeader), v6 * (a2->colorDepth1 >> 3));

    return 1;
}

int TEX_l_File_LoadRaw(char* offset, tEngTexIntermediateStruct* a2)
{
    tEngTexIntermediateStruct* v2;
    int result;
    unsigned int v4;
    char* v5;

    v2 = a2;
    if ((a2->flags & 2) != 0)
    {
        result = a2->realWidth * a2->realHeight;
        if (!result)
            return 0;
        v4 = (result * (unsigned int)a2->colorDepth2) >> 3;
        v5 = (char*)MEM_p_AllocAligned(v4, 64);
        v2->imgDataPtr = v5;
        MEM_gst_MemoryInfo.usedMemoryByTextures += MEM_MemBlockSizeGetAligned(v5);
        LOA_ReadCharArray(&offset, v2->imgDataPtr, v4, 1);
    }
    return 1;
}

int TEX_l_File_LoadRawPalette(char* offset, tEngTexIntermediateStruct* a2, unsigned int a3)
{
    int* v3;
    unsigned int v4;

    v3 = (int*)a2->elems;
    memset(a2->elems, 0xFFu, sizeof(a2->elems));
    if (a3 > 80 || (a3 & 3) != 0)
        return 0;
    if ((a3 - 32) >> 2)
    {
        v4 = (a3 - 32) >> 2;
        do
        {
            *v3++ = LOA_ReadLong(&offset, 0, 1);
            --v4;
        } while (v4);
    }
    return 1;
}

int __cdecl TEX_l_File_LoadAnimated(char* offset, tEngTexIntermediateStruct* a2, int a3)
{
    uint8_t v3;
    tEngTexIntermediateStruct* v4;
    int v5;
    tTexAnimatedInnerElem* v6;
    tTexAnimatedInnerElem* v7;
    uint16_t v9;
    tTexAnimatedInnerElem* v10;

    if (a3)
    {
        LOA_ReadLong(&offset, 0, 1);
        v9 = LOA_ReadShort(&offset, 0, 1);
        v3 = LOA_ReadChar(&offset, 0, 1);
        LOA_ReadChar(&offset, 0, 2);
    }
    else
    {
        v9 = 0;
        v3 = 0;
    }
    v4 = a2;
    a2->flags |= 0x20u;
    v10 = 0;
    if (v3)
    {
        v5 = v3;
        v6 = (tTexAnimatedInnerElem*)MEM_p_Alloc(sizeof(tTexAnimatedInnerElem) * v3);
        v10 = v6;
        v7 = v6;
        do
        {
            v7->key = LOA_ReadLong(&offset, 0, 1);
            v7->texTime = LOA_ReadShort(&offset, 0, 1);
            v7->texIdx = -1;
            LOA_ReadShort(&offset, 0, 2);
            ++v7;
            --v5;
        } while (v5);
    }
    v4->tp.animatedParams.aiePtr = v10;
    v4->tp.animatedParams.num = v3;
    v4->tp.animatedParams.flags = v9;
    return 1;
}

struct sEngTexListElemStruct* TEX_pst_List_FindTexture(struct sEngTexListStruct* a1, int key)
{
    int i;

    for (i = 0; i < a1->actTexElemNum; i++) {
        if (a1->texElems[i].texKey == key) return &a1->texElems[i];
    }

    return NULL;
}

struct sEngPaletteElem* TEX_pst_List_FindPalette(tEngTexListStruct* a1, int key)
{
    int i;

    for (i = 0; i < a1->paletteNum; i++) {
        if (a1->palettes[i].key == key) return &a1->palettes[i];
    }

    return NULL;
}

int16_t TEX_w_List_AddTexture(tEngTexListStruct* a1, int key)
{
    int correctedKey;
    int16_t result;
    int v4;
    tEngTexListElemStruct* v5;
    int v6;
    int v7;
    char* v8;
    unsigned int v9;
    tEngTexListElemStruct* v10;
    int v11;
    tEngTexListElemStruct* v12;
    tEngTexListElemStruct* v13;

    correctedKey = key;
    if (key == -1)
        return -1;
    if ((key & 0xFFFFFF0F) == 0 && (key & 0xF0) != 0)
    {
        if (ENG_PlatformID == 2)
        {
            correctedKey = key + 1;
            goto LABEL_10;
        }
        if (ENG_PlatformID == 3)
        {
            correctedKey = key + 2;
            goto LABEL_19;
        }
    }
    if (ENG_PlatformID == 2)
    {
    LABEL_10:
        switch (correctedKey)
        {
        case 0x1F0016B6:
            correctedKey = 0x8F005893;
            break;
        case 0x62000872:
            correctedKey = 0x8F005895;
            break;
        case 0x720006F4:
            correctedKey = 0x8F005897;
            break;
        case 0x62000F6D:
            correctedKey = 0x8F005899;
            break;
        }
        goto LABEL_27;
    }
    if (ENG_PlatformID == 3)
    {
    LABEL_19:
        switch (correctedKey)
        {
        case 0x1F0016B6:
            correctedKey = 0x8F005894;
            break;
        case 0x62000872:
            correctedKey = 0x8F005896;
            break;
        case 0x720006F4:
            correctedKey = 0x8F005898;
            break;
        case 0x62000F6D:
            correctedKey = 0x8F00589A;
            break;
        }
    }
LABEL_27:
    v4 = a1->actTexElemNum;
    if (v4)
    {
        v5 = TEX_pst_List_FindTexture(a1, correctedKey);
        if (v5)
            return v5->textElemIdx;
    }
    if (v4 == a1->maxTexNum)
    {
        v6 = a1->maxTexNum + 256;
        v7 = sizeof(tEngTexListElemStruct) * v6;
        a1->maxTexNum = v6;
        if (v6 == 256)
        {
            v8 = (char*)MEM_p_AllocAligned(v7, 64);
            a1->texElems = (tEngTexListElemStruct*)v8;            
            v9 = MEM_MemBlockSizeGetAligned(v8) + MEM_gst_MemoryInfo.usedMemoryByTextures;
        }
        else
        {
            
            MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAligned(a1->texElems);
            v10 = (tEngTexListElemStruct*)MEM_p_ReallocAligned(a1->texElems, v7, 64);
            a1->texElems = v10;
            v9 = MEM_MemBlockSizeGetAligned(v10) + MEM_gst_MemoryInfo.usedMemoryByTextures;
        }
        MEM_gst_MemoryInfo.usedMemoryByTextures = v9;
    }
    v11 = a1->actTexElemNum;
    v12 = a1->texElems;
    a1->actTexElemNum = v11 + 1;
    v13 = &v12[v11];
    v13->texKey = 0;
    v13->normalData.width = 0;
    v13->normalData.height = 0;
    v13->texElemFlag = 0;
    v13->texKey = correctedKey;
    result = (short)((a1->actTexElemNum & 0xffff) - 1);
    v13->textElemIdx = result;
    return result;
}

int16_t TEX_w_List_AddPalette(tEngTexListStruct* a1, int key)
{
    int16_t result;
    int v3;
    tEngPaletteElem* v4;
    int v5;
    int v6;
    int v7;
    tEngPaletteElem* v8;
    unsigned int v9;
    tEngPaletteElem* v10;
    int v11;
    tEngPaletteElem* v12;
    tEngPaletteElem* v13;

    if (key == -1)
        return -1;
    v3 = a1->paletteNum;
    if (v3)
    {
        v4 = TEX_pst_List_FindPalette(a1, key);
        if (v4)
            return v4->paletteID;
    }
    v5 = a1->unk1;
    if (v3 == v5)
    {
        v6 = v5 + 256;
        v7 = sizeof(tEngPaletteElem) * v6;
        a1->unk1 = v6;
        if (v6 == 256)
        {
            v8 = (tEngPaletteElem*)MEM_p_AllocAligned(v7, 64);
            a1->palettes = v8;
            v9 = MEM_MemBlockSizeGetAligned(v8) + MEM_gst_MemoryInfo.usedMemoryByTextures;
        }
        else
        {
            MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAligned(a1->palettes);
            v10 = (tEngPaletteElem*)MEM_p_ReallocAligned(a1->palettes, v7, 64);
            a1->palettes = v10;
            v9 = MEM_MemBlockSizeGetAligned(v10) + MEM_gst_MemoryInfo.usedMemoryByTextures;
        }
        MEM_gst_MemoryInfo.usedMemoryByTextures = v9;
    }
    v11 = a1->paletteNum;
    v12 = a1->palettes;
    a1->paletteNum = v11 + 1;
    v13 = &v12[v11];
    v13->key = 0;
    v13->paletteData = 0;
    v13->flags = 0;
    v13->b1 = 0;
    v13->paletteID = 0;
    v13->key = key;
    result = (a1->paletteNum & 0xffff) - 1;
    v13->paletteID = result;
    return result;
}

void TEX_List_Init(struct sEngTexListStruct* a1)
{
    memset(a1, 0, sizeof(tEngTexListStruct));
}

void TEX_List_Free(tEngTexListStruct* a1)
{
    tEngTexListElemStruct* v1;
    int v3;
    int v4;
    uint8_t* v5;
    tEngPaletteElem* v7;

    if (a1->maxTexNum)
    {
        v1 = a1->texElems;
        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v1);
        MEM_FreeAlign(a1->texElems);
    }
    if (a1->unk1)
    {
        v3 = 0;
        if (a1->paletteNum > 0)
        {
            v4 = 0;
            do
            {
                v5 = a1->palettes[v4].paletteData;
                if (v5)
                {
                    MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v5);
                    MEM_FreeAlign(a1->palettes[v4].paletteData);
                    a1->palettes[v4].paletteData = 0;
                }
                ++v3;
                ++v4;
            } while (v3 < a1->paletteNum);
        }
        v7 = a1->palettes;
        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v7);
        MEM_FreeAlign(a1->palettes);
    }
    memset(a1, 0, sizeof(tEngTexListStruct));
}

struct sTexAnimatedElem* TEX_pst_Anim_Get(int16_t id)
{
    int i;

    for (i = 0; i < TEX_gi_NbAnimated; i++) {
        if (TEX_gdst_Animated[i].id == id) return &TEX_gdst_Animated[i];
    }

    return NULL;
}

struct sTexAnimatedElem* TEX_pst_Anim_Add(int16_t id, char texNum, int16_t flags, struct sTexAnimatedInnerElem* data)
{
    tTexAnimatedElem* v4;
    tTexAnimatedElem* result;
    unsigned int v6;

    if (TEX_gi_NbAnimated == TEX_gi_NbAnimatedMax)
    {
        TEX_gi_NbAnimatedMax += 16;
        v6 = 16 * TEX_gi_NbAnimatedMax;
        if (TEX_gi_NbAnimatedMax == 16)
            v4 = (tTexAnimatedElem*)MEM_p_Alloc(v6);
        else
            v4 = (tTexAnimatedElem*)MEM_p_Realloc(TEX_gdst_Animated, v6);
        TEX_gdst_Animated = v4;
    }
    result = &TEX_gdst_Animated[TEX_gi_NbAnimated++];
    result->flags = flags;
    result->texNum = texNum;
    result->id = id;
    result->b3 = 0;
    result->animationFlags = 0;
    result->texPtr = data;
    if (data)
    {
        result->currentTex = 0;
        result->remainingTime = data->texTime;
        TEX_gst_GlobalList.texElems[id].animatedData.textureID = data->texIdx;
    }
    else
    {
        result->remainingTime = 0;
        result->currentTex = -1;
        TEX_gst_GlobalList.texElems[id].animatedData.textureID = -1;
    }
    return result;
}

void TEX_Anim_Update()
{
    double v0;
    tTexAnimatedElem* v1;
    int v2;
    int v3;
    uint8_t v4;
    uint8_t v5;
    uint8_t v6;
    int16_t v7;
    tTexAnimatedInnerElem* v8;
    tTexAnimatedInnerElem* v9;

    v0 = TIM_gf_dt + sf_TimeLeft_218;
    v1 = TEX_gdst_Animated;
    v2 = (unsigned int)(60.0f * v0);
    sf_TimeLeft_218 = (float)(v0 - (double)v2 * 0.016666668);

    for (v3 = 0; v3 < TEX_gi_NbAnimated; ++v1)
    {
        v4 = v1->texNum;
        if (v4)
        {
            v5 = v1->animationFlags;
            if ((v5 & 2) == 0 || (v6 = v1->b3, v6 == v1->currentTex))
            {
                if ((v5 & 1) != 0)
                    goto LABEL_10;
                v1->remainingTime -= v2;
                if (v1->remainingTime >= 0)
                    goto LABEL_10;
                v7 = (uint8_t)(v1->currentTex + 1) % (int16_t)v4;
            }
            else
            {
                v7 = (v6 % v4) & 0xff;
                v1->b3 = (uint8_t)v7;
            }
            v8 = v1->texPtr;
            v1->currentTex = (uint8_t)v7;
            v9 = &v8[(uint8_t)v7];
            v1->remainingTime = v9->texTime;
            TEX_gst_GlobalList.texElems[v1->id].animatedData.textureID = v9->texIdx;
        }
    LABEL_10:
        ++v3;
    }
}

void TEX_Anim_Reinit()
{
    int v0;
    int v1;
    tTexAnimatedElem* v2;

    v0 = TEX_gi_NbAnimated;
    if (TEX_gi_NbAnimated)
    {
        v1 = 0;
        if (TEX_gi_NbAnimated > 0)
        {
            v2 = TEX_gdst_Animated;
            do
            {
                if (v2->texNum)
                {
                    MEM_Free(v2->texPtr);
                    v0 = TEX_gi_NbAnimated;
                }
                ++v1;
                ++v2;
            } while (v1 < v0);
        }
        TEX_gi_NbAnimatedMax = 0;
        TEX_gi_NbAnimated = 0;
        MEM_Free(TEX_gdst_Animated);
        TEX_gdst_Animated = 0;
    }
}

int16_t TEX_w_Manager_ChooseGoodInTex(struct sTexManagerStruct* a1, struct sEngTexIntermediateStruct* tims)
{
    int i; 
    int flagsHas0x7000;
    int v4;
    int16_t result;
    tEngTexIntermediateStruct* v6;
    BOOL v7;
    int* paletteID;

    i = 0;
    flagsHas0x7000 = a1->flags & 0x7000;
    tims->texIdx = -1;
    tims->probablyType = 0; // TODO FIXME az biztos jó?
    tims->paletteID = -1;
    tims->textureID = -1;
    do
    {
        v4 = a1->keyLoadOrderIDs[i];
        if (v4 == -1)
            return -1;
        if (tims->elems[v4].paletteID != -1)
            break;
        if (tims->elems[v4].palettedTextureID != -1)
            break;
        if (tims->elems[v4].textureID != -1)
            break;
        ++i;
    } while (i < 4);
    if (i == 4)
        return -1;
    
    v6 = tims;

    if (v6->elems[v4].palettedTextureID == -1
        || (paletteID = &tims->elems[v4].paletteID, *paletteID == -1)
        || v6->elems[v4].textureID != -1 && flagsHas0x7000 != 0x7000)
    {
        if (v6->elems[v4].textureID != -1)
            tims->texIdx = TEX_w_List_AddTexture(&TEX_gst_GlobalList, tims->elems[v4].textureID);
        result = tims->texIdx;
        tims->probablyType = 2;
    }
    else
    {
        tims->paletteID = TEX_w_List_AddPalette(&TEX_gst_GlobalList, *paletteID);
        tims->textureID = TEX_w_List_AddTexture(&TEX_gst_GlobalList, v6->elems[v4].palettedTextureID);
        v7 = (tims->tp.flagsUnkAndProceduralUnkb1 & 0x100) == 0;
        tims->probablyType = (flagsHas0x7000 == 0x7000 ? 0 : 4) | 1;
        if (!v7 && *paletteID != -1)
        {
            TEX_gst_GlobalList.palettes[tims->paletteID].flags |= 0x18; //LOBYTE(ADJ(v8)->unk3InCarteUpdatePalette) |= 0x18u;
        }
        result = tims->textureID;
    }
    return result;
}

void TEX_File_FreeDescription(struct sEngTexIntermediateStruct* a1)
{
    char* v1;

    if (a1->descriptionPtr && a1->tgaUnkS1)
    {
        MEM_Free((void*)a1->descriptionPtr);
        a1->descriptionPtr = 0;
    }
    v1 = a1->imgDataPtr;
    if (v1)
    {
        MEM_gst_MemoryInfo.usedMemoryByTextures -= MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(v1);
        MEM_FreeAlign(a1->imgDataPtr);
        a1->imgDataPtr = 0;
    }
}
