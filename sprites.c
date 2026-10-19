#include "math.h"
#include "sprites.h"
#include "gdi.h"
#include "materialanddraw.h"

void SPG_ComputeSpriteFog(Vec3* a1, int* a2, int a3)
{
    int i;
    float v6;

    for (i = a3; i; --i)
    {
        v6 = a1->z * SPG_gs_SpecialFogAxis.z + a1->y * SPG_gs_SpecialFogAxis.y + SPG_gs_SpecialFogAxis.x * a1->x - SPG_gs_SpecialFogDistance;
        *a2 = LIGHT_ul_Interpol2Colors(*a2, SPG_gs_SpecialFogCOLOR, v6);
        ++a2;
        ++a1;
    }
}

void SPG_ComputeSpriteFog_Cull(Vec3* a1, int* a2, int a3)
{
    int i;
    double v6;
    float v7;

    for (i = a3; i; --i)
    {
        v6 = a1->z * SPG_gs_SpecialFogAxis.z + a1->y * SPG_gs_SpecialFogAxis.y + SPG_gs_SpecialFogAxis.x * a1->x - SPG_gs_SpecialFogDistance;
        v7 = (float)v6;
        if (v6 < 1.0)
        {
            if (v7 > 0.0)
                *a2 = LIGHT_ul_Interpol2Colors(*a2, SPG_gs_SpecialFogCOLOR, v7);
        }
        else
        {
            *a1 = SPG_gs_SpecialFogCulledPoint;
        }
        ++a2;
        ++a1;
    }
}

void SPG_Draw(int num)
{
    Vec3* verticesCurrentBackup;

    verticesCurrentBackup = gpst_DX_stDD->verticesCurrent;
    gpst_DX_stDD->verticesCurrent = SPG_gvCurrentStack;
    if (SPG_gs_SpecialFogCOLOR)
    {
        if ((SPG_gs_SpecialFogCOLOR & 0x100) != 0)
            SPG_ComputeSpriteFog_Cull(SPG_gvStack, &SPG_gvCOLORStack[-num], num);
        else
            SPG_ComputeSpriteFog(SPG_gvStack, &SPG_gvCOLORStack[-num], num);
    }
    MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[0]);
    MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[1]);
    MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[2]);
    MAT_DrawIndexedSprites(pst_CurrentObj, pst_CurrentMat, &SPG_stIndxSp[3]);

    gpst_DX_stDD->verticesCurrent = verticesCurrentBackup;
    SPG_gvCOLORStack = SPG_gvCOLORStack - num;
    gvBase = 0;
}

void SPG_PackAndAddData(Vec3* dest, tSpgVertexElem* pos, tSpgVertexElem* addToPos, float* randomData)
{
    pos->colorComponentA += addToPos->colorComponentA;
    pos->colorComponentB += addToPos->colorComponentB;
    pos->colorComponentG += addToPos->colorComponentG;
    pos->colorComponentR += addToPos->colorComponentR;
    pos->v.x = addToPos->v.x + pos->v.x;
    pos->v.y = addToPos->v.y + pos->v.y;
    pos->v.z = addToPos->v.z + pos->v.z;

    dest->x = SPGNoise1.x * randomData[0] + pos->v.x;
    dest->y = SPGNoise1.y * randomData[0] + pos->v.y;
    dest->z = SPGNoise1.z * randomData[0] + pos->v.z;    

    dest->x = SPGNoise2.x * randomData[1] + dest->x;
    dest->y = SPGNoise2.y * randomData[1] + dest->y;
    dest->z = SPGNoise2.z * randomData[1] + dest->z;

    *(SPG_gvCOLORStack++) = (pos->colorComponentR >> 7) | 
                        (2 * (((pos->colorComponentA & 0xFFFFFF80) << 16) | 
                        (pos->colorComponentG | 
                        ((pos->colorComponentB & 0xFFFFFF80) << 8)) & 0xFFFFFF80));
}

void SPG_SerialSpriter(tSpgVertexElem* a1, tSpgVertexElem* a2, int a3)
{
    Vec3* v3;
    Vec3* v4;
    float* v5;
    unsigned int num;
    Vec3* a3a;

    v3 = &SPG_gvStack[gvBase];
    a3a = &v3[a3];
    v4 = a3a;
    v5 = &f_RandomTable[gvBase];
    if (a3a > &SPG_gvStack[256])
    {
        num = ((char*)a3a - (char*)&SPG_gvStack[256] - 1) / (256 * sizeof(Vec3)) + 1;
        a3a -= 256 * num;
        do
        {
            for (; v3 < &SPG_gvStack[256]; ++v5)
                SPG_PackAndAddData(v3++, a1, a2, v5);
            SPG_Draw(256);
            v3 -= 256;
            v5 -= 256;
            --num;
        } while (num);
        v4 = a3a;
    }
    if (v3 < v4)
    {
        do
            SPG_PackAndAddData(v3++, a1, a2, v5++);
        while (v3 < a3a);
    }
    gvBase = v3 - SPG_gvStack;
}

void divide_spg_vertices_andcolorelem_sub_420790(tSpgVertexElem* a1, int a2)
{
    float a1a;
    int tmp;

    a1->colorComponentA >>= a2;
    a1->colorComponentB >>= a2;
    a1->colorComponentG >>= a2;
    a1->colorComponentR >>= a2;
    tmp = 0x3F800000 - (a2 << 23);
    a1a = *(float*)&tmp;
    a1->v.x = a1a * a1->v.x;
    a1->v.y = a1a * a1->v.y;
    a1->v.z = a1a * a1->v.z;
}

void substract_spg_verticesandcolorelem_sub_420730(tSpgVertexElem* a1, tSpgVertexElem* a2, tSpgVertexElem* a3)
{
    a1->colorComponentA = a2->colorComponentA - a3->colorComponentA;
    a1->colorComponentB = a2->colorComponentB - a3->colorComponentB;
    a1->colorComponentG = a2->colorComponentG - a3->colorComponentG;
    a1->colorComponentR = a2->colorComponentR - a3->colorComponentR;
    a1->v.x = a2->v.x - a3->v.x;
    a1->v.y = a2->v.y - a3->v.y;
    a1->v.z = a2->v.z - a3->v.z;
}

float GET_SqrLenght(Vec3* a1)
{
    return a1->x * a1->x + a1->y * a1->y + a1->z * a1->z;
}

int spg_calc_divider_sub_4208F0(int maxDivide, float a2, Vec3* a3, Vec3* a4)
{
    int v4;
    double v5;
    float v7;

    v4 = 0;
    v7 = GET_SqrLenght(a4);
    v5 = GET_SqrLenght(a3);
    if (v5 > v7)
        v5 = v7;
    for (; v5 > a2; ++v4)
    {
        if (v4 >= maxDivide)
            break;
        v5 = v5 * 0.25;
    }
    return v4;
}

void SPG_SetNoise(tSpgVertexElem* a1, Vec3* a2, float a3)
{
    SPGNoise1.x = a1->v.x * a3;
    SPGNoise1.y = a1->v.y * a3;
    SPGNoise1.z = a1->v.z * a3;
    SPGNoise2.x = a2->x * a3;
    SPGNoise2.y = a2->y * a3;
    SPGNoise2.z = a2->z * a3;
}

void SPG_TransformTrisToSprites(Vec3* a1, Vec3* a2, Vec3* a3, unsigned int* colors, struct sSpgTrisToSpritesStruct* a5)
{
    unsigned int v5;
    float v6;
    unsigned int v7;
    float v8;
    unsigned int v9;
    float v10;
    float v11;
    float v12;
    __int16 v13;
    __int16 v14;
    __int16 v15;
    __int16 v16;
    unsigned int v17;
    int v18;
    int v20;
    double v21;
    float v22;
    float v23;
    int v24;
    tSpgVertexElem a3a;
    tSpgVertexElem v26;
    tSpgVertexElem a4a;
    tSpgVertexElem v28;
    int v29;
    tSpgVertexElem a2a;
    tSpgVertexElem v31;
    tSpgVertexElem a1a;
    int tmp;

    v5 = *colors;
    v28.colorComponentA = (*colors >> 17) & 0x7F80;
    v28.colorComponentB = (v5 >> 9) & 0x7F80;
    v28.colorComponentG = (v5 >> 1) & 0x7F80;
    v28.colorComponentR = (uint8_t)v5 << 7;
    v28.v.x = a1->x;
    v6 = a1->z;
    v28.v.y = a1->y;
    v28.v.z = v6;
    v7 = colors[1];
    v31.colorComponentA = (v7 >> 17) & 0x7F80;
    v31.colorComponentB = (v7 >> 9) & 0x7F80;
    v31.colorComponentG = (v7 >> 1) & 0x7F80;
    v31.colorComponentR = (uint8_t)v7 << 7;
    v31.v.x = a2->x;
    v8 = a2->y;
    v31.v.z = a2->z;
    v9 = colors[2];
    v31.v.y = v8;
    a2a.colorComponentA = (v9 >> 17) & 0x7F80;
    a2a.colorComponentB = (v9 >> 9) & 0x7F80;
    a2a.colorComponentR = (uint8_t)v9 << 7;
    v10 = a3->x;
    a2a.colorComponentG = (v9 >> 1) & 0x7F80;
    v11 = a3->y;
    a2a.v.x = v10;
    v12 = a3->z;
    a2a.v.y = v11;
    a2a.v.z = v12;
    v13 = v31.colorComponentA - v28.colorComponentA;
    a3a.colorComponentA = v31.colorComponentA - v28.colorComponentA;
    v14 = v31.colorComponentB - v28.colorComponentB;
    a3a.v.x = v31.v.x - v28.v.x;
    a3a.colorComponentG = v31.colorComponentG - v28.colorComponentG;
    a3a.v.y = v31.v.y - v28.v.y;
    a3a.colorComponentR = v31.colorComponentR - v28.colorComponentR;
    v15 = a2a.colorComponentB - v28.colorComponentB;
    a3a.v.z = v31.v.z - v28.v.z;
    v16 = a2a.colorComponentA - v28.colorComponentA;
    a4a.colorComponentG = a2a.colorComponentG - v28.colorComponentG;
    a4a.v.x = a2a.v.x - v28.v.x;
    a4a.colorComponentR = a2a.colorComponentR - v28.colorComponentR;
    a3a.colorComponentB = v31.colorComponentB - v28.colorComponentB;
    a4a.colorComponentA = a2a.colorComponentA - v28.colorComponentA;
    a4a.colorComponentB = a2a.colorComponentB - v28.colorComponentB;
    a4a.v.y = v11 - v28.v.y;
    a4a.v.z = a2a.v.z - v28.v.z;
    if (a5->f1 == 0.0)
    {
        v18 = a5->maxDivide;
    }
    else
    {
        v17 = spg_calc_divider_sub_4208F0(a5->maxDivide, a5->f1, &a3a.v, &a4a.v);
        v14 = a3a.colorComponentB;
        v18 = v17;
    }
    if (v18)
    {
        a3a.colorComponentG >>= v18;
        a3a.colorComponentR >>= v18;
        tmp = 0x3F800000 - (v18 << 23);
        v23 = *(float*)&tmp;
        a4a.colorComponentG >>= v18;
        a4a.colorComponentR >>= v18;
        a3a.v.x = v23 * a3a.v.x;
        a3a.colorComponentB = v14 >> v18;
        a3a.colorComponentA = v13 >> v18;
        a4a.colorComponentA = v16 >> v18;
        a3a.v.y = a3a.v.y * v23;
        a4a.colorComponentB = v15 >> v18;
        a3a.v.z = a3a.v.z * v23;
        a4a.v.x = v23 * a4a.v.x;
        v22 = a5->noise;
        a4a.v.y = a4a.v.y * v23;
        v20 = (1 << v18) - 1;
        v21 = a4a.v.z * v23;
        v24 = v20;
        a4a.v.z = (float)v21;
        SPG_SetNoise(&a3a, &a4a.v, v22);
        memcpy(&v26, &v28, sizeof(v26));
        if (1 << v18 != 1)
        {
            while (1)
            {
                v26.colorComponentB += a3a.colorComponentB;
                v26.colorComponentG += a3a.colorComponentG;
                v26.colorComponentR += a3a.colorComponentR;
                v26.colorComponentA += a3a.colorComponentA;
                v26.v.x = a3a.v.x + v26.v.x;
                v26.v.y = v26.v.y + a3a.v.y;
                v26.v.z = v26.v.z + a3a.v.z;
                memcpy(&a1a, &v26, sizeof(a1a));
                SPG_SerialSpriter(&a1a, &a4a, v20 - 1);
                v29 = v20 - 1;
                if (!v29)
                    break;
                v20 = v29;
            }
            v20 = (1 << v18) - 1;
        }
        if ((a5->flags & 1) != 0)
        {
            memcpy(&v26, &v28, sizeof(v26));
            SPG_SerialSpriter(&v26, &a3a, v20);
        }
        if ((a5->flags & 2) != 0)
        {
            memcpy(&v26, &v31, sizeof(v26));
            substract_spg_verticesandcolorelem_sub_420730(&a3a, &a2a, &v31);
            divide_spg_vertices_andcolorelem_sub_420790(&a3a, v18);
            SPG_SerialSpriter(&v26, &a3a, v24);
        }
        if ((a5->flags & 4) != 0)
        {
            memcpy(&v26, &v28, sizeof(v26));
            substract_spg_verticesandcolorelem_sub_420730(&a3a, &a2a, &v28);
            divide_spg_vertices_andcolorelem_sub_420790(&a3a, v18);
            SPG_SerialSpriter(&v26, &a3a, v24);
        }
    }
}