#include "math.h"
#include "sprites.h"
#include "gdi.h"
#include "materialanddraw.h"

void SPG_ComputeSpriteFog(Vec3* verts, int* colors, int num)
{
    int i;
    float dist;

    for (i = num; i; --i)
    {
        dist = verts->z * SPG_gs_SpecialFogAxis.z + verts->y * SPG_gs_SpecialFogAxis.y + SPG_gs_SpecialFogAxis.x * verts->x - SPG_gs_SpecialFogDistance;
        *colors = LIGHT_ul_Interpol2Colors(*colors, SPG_gs_SpecialFogCOLOR, dist);
        ++colors;
        ++verts;
    }
}

void SPG_ComputeSpriteFog_Cull(Vec3* verts, int* colors, int num)
{
    int i;
    double dist;
    float distF;

    for (i = num; i; --i)
    {
        dist = verts->z * SPG_gs_SpecialFogAxis.z + verts->y * SPG_gs_SpecialFogAxis.y + SPG_gs_SpecialFogAxis.x * verts->x - SPG_gs_SpecialFogDistance;
        distF = (float)dist;
        if (dist < 1.0)
        {
            if (distF > 0.0)
                *colors = LIGHT_ul_Interpol2Colors(*colors, SPG_gs_SpecialFogCOLOR, distF);
        }
        else
        {
            *verts = SPG_gs_SpecialFogCulledPoint;
        }
        ++colors;
        ++verts;
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

void SPG_SerialSpriter(tSpgVertexElem* pos, tSpgVertexElem* step, int num)
{
    Vec3* cur;
    Vec3* limit;
    float* rnd;
    unsigned int batches;
    Vec3* end;

    cur = &SPG_gvStack[gvBase];
    end = &cur[num];
    limit = end;
    rnd = &f_RandomTable[gvBase];
    if (end > &SPG_gvStack[256])
    {
        batches = ((char*)end - (char*)&SPG_gvStack[256] - 1) / (256 * sizeof(Vec3)) + 1;
        end -= 256 * batches;
        do
        {
            for (; cur < &SPG_gvStack[256]; ++rnd)
                SPG_PackAndAddData(cur++, pos, step, rnd);
            SPG_Draw(256);
            cur -= 256;
            rnd -= 256;
            --batches;
        } while (batches);
        limit = end;
    }
    if (cur < limit)
    {
        do
            SPG_PackAndAddData(cur++, pos, step, rnd++);
        while (cur < end);
    }
    gvBase = cur - SPG_gvStack;
}

static void SPG_l_DivideVertexAndColor(tSpgVertexElem* elem, int shift)
{
    float scale;
    int tmp;

    elem->colorComponentA >>= shift;
    elem->colorComponentB >>= shift;
    elem->colorComponentG >>= shift;
    elem->colorComponentR >>= shift;
    tmp = 0x3F800000 - (shift << 23);
    scale = *(float*)&tmp;
    elem->v.x = scale * elem->v.x;
    elem->v.y = scale * elem->v.y;
    elem->v.z = scale * elem->v.z;
}

static void SPG_l_SubtractVertexAndColor(tSpgVertexElem* dest, tSpgVertexElem* src1, tSpgVertexElem* src2)
{
    dest->colorComponentA = src1->colorComponentA - src2->colorComponentA;
    dest->colorComponentB = src1->colorComponentB - src2->colorComponentB;
    dest->colorComponentG = src1->colorComponentG - src2->colorComponentG;
    dest->colorComponentR = src1->colorComponentR - src2->colorComponentR;
    dest->v.x = src1->v.x - src2->v.x;
    dest->v.y = src1->v.y - src2->v.y;
    dest->v.z = src1->v.z - src2->v.z;
}

static int SPG_l_CalcDivider(int maxDivide, float threshold, Vec3* edge1, Vec3* edge2)
{
    int divides;
    double len;
    float len2;

    divides = 0;
    len2 = MATH_VecLenSquared(edge2);
    len = MATH_VecLenSquared(edge1);
    if (len > len2)
        len = len2;
    for (; len > threshold; ++divides)
    {
        if (divides >= maxDivide)
            break;
        len = len * 0.25;
    }
    return divides;
}

void SPG_SetNoise(tSpgVertexElem* elem, Vec3* vec, float scale)
{
    SPGNoise1.x = elem->v.x * scale;
    SPGNoise1.y = elem->v.y * scale;
    SPGNoise1.z = elem->v.z * scale;
    SPGNoise2.x = vec->x * scale;
    SPGNoise2.y = vec->y * scale;
    SPGNoise2.z = vec->z * scale;
}

void SPG_TransformTrisToSprites(Vec3* p0, Vec3* p1, Vec3* p2, unsigned int* colors, struct sSpgTrisToSpritesStruct* params)
{
    unsigned int col0;
    float z0;
    unsigned int col1;
    float y1;
    unsigned int col2;
    float x2;
    float y2;
    float z2;
    int16_t dA1;
    int16_t dB1;
    int16_t dB2;
    int16_t dA2;
    unsigned int dividesCalc;
    int divides;
    int count;
    double edge2z;
    float noise;
    float scale;
    int countSaved;
    tSpgVertexElem edge1;
    tSpgVertexElem cur;
    tSpgVertexElem edge2;
    tSpgVertexElem c0;
    int next;
    tSpgVertexElem c2;
    tSpgVertexElem c1;
    tSpgVertexElem rowStart;
    int tmp;

    col0 = *colors;
    c0.colorComponentA = (*colors >> 17) & 0x7F80;
    c0.colorComponentB = (col0 >> 9) & 0x7F80;
    c0.colorComponentG = (col0 >> 1) & 0x7F80;
    c0.colorComponentR = (uint8_t)col0 << 7;
    c0.v.x = p0->x;
    z0 = p0->z;
    c0.v.y = p0->y;
    c0.v.z = z0;
    col1 = colors[1];
    c1.colorComponentA = (col1 >> 17) & 0x7F80;
    c1.colorComponentB = (col1 >> 9) & 0x7F80;
    c1.colorComponentG = (col1 >> 1) & 0x7F80;
    c1.colorComponentR = (uint8_t)col1 << 7;
    c1.v.x = p1->x;
    y1 = p1->y;
    c1.v.z = p1->z;
    col2 = colors[2];
    c1.v.y = y1;
    c2.colorComponentA = (col2 >> 17) & 0x7F80;
    c2.colorComponentB = (col2 >> 9) & 0x7F80;
    c2.colorComponentR = (uint8_t)col2 << 7;
    x2 = p2->x;
    c2.colorComponentG = (col2 >> 1) & 0x7F80;
    y2 = p2->y;
    c2.v.x = x2;
    z2 = p2->z;
    c2.v.y = y2;
    c2.v.z = z2;
    dA1 = c1.colorComponentA - c0.colorComponentA;
    edge1.colorComponentA = c1.colorComponentA - c0.colorComponentA;
    dB1 = c1.colorComponentB - c0.colorComponentB;
    edge1.v.x = c1.v.x - c0.v.x;
    edge1.colorComponentG = c1.colorComponentG - c0.colorComponentG;
    edge1.v.y = c1.v.y - c0.v.y;
    edge1.colorComponentR = c1.colorComponentR - c0.colorComponentR;
    dB2 = c2.colorComponentB - c0.colorComponentB;
    edge1.v.z = c1.v.z - c0.v.z;
    dA2 = c2.colorComponentA - c0.colorComponentA;
    edge2.colorComponentG = c2.colorComponentG - c0.colorComponentG;
    edge2.v.x = c2.v.x - c0.v.x;
    edge2.colorComponentR = c2.colorComponentR - c0.colorComponentR;
    edge1.colorComponentB = c1.colorComponentB - c0.colorComponentB;
    edge2.colorComponentA = c2.colorComponentA - c0.colorComponentA;
    edge2.colorComponentB = c2.colorComponentB - c0.colorComponentB;
    edge2.v.y = y2 - c0.v.y;
    edge2.v.z = c2.v.z - c0.v.z;
    if (params->f1 == 0.0)
    {
        divides = params->maxDivide;
    }
    else
    {
        dividesCalc = SPG_l_CalcDivider(params->maxDivide, params->f1, &edge1.v, &edge2.v);
        dB1 = edge1.colorComponentB;
        divides = dividesCalc;
    }
    if (divides)
    {
        edge1.colorComponentG >>= divides;
        edge1.colorComponentR >>= divides;
        tmp = 0x3F800000 - (divides << 23);
        scale = *(float*)&tmp;
        edge2.colorComponentG >>= divides;
        edge2.colorComponentR >>= divides;
        edge1.v.x = scale * edge1.v.x;
        edge1.colorComponentB = dB1 >> divides;
        edge1.colorComponentA = dA1 >> divides;
        edge2.colorComponentA = dA2 >> divides;
        edge1.v.y = edge1.v.y * scale;
        edge2.colorComponentB = dB2 >> divides;
        edge1.v.z = edge1.v.z * scale;
        edge2.v.x = scale * edge2.v.x;
        noise = params->noise;
        edge2.v.y = edge2.v.y * scale;
        count = (1 << divides) - 1;
        edge2z = edge2.v.z * scale;
        countSaved = count;
        edge2.v.z = (float)edge2z;
        SPG_SetNoise(&edge1, &edge2.v, noise);
        memcpy(&cur, &c0, sizeof(cur));
        if (1 << divides != 1)
        {
            while (1)
            {
                cur.colorComponentB += edge1.colorComponentB;
                cur.colorComponentG += edge1.colorComponentG;
                cur.colorComponentR += edge1.colorComponentR;
                cur.colorComponentA += edge1.colorComponentA;
                cur.v.x = edge1.v.x + cur.v.x;
                cur.v.y = cur.v.y + edge1.v.y;
                cur.v.z = cur.v.z + edge1.v.z;
                memcpy(&rowStart, &cur, sizeof(rowStart));
                SPG_SerialSpriter(&rowStart, &edge2, count - 1);
                next = count - 1;
                if (!next)
                    break;
                count = next;
            }
            count = (1 << divides) - 1;
        }
        if ((params->flags & 1) != 0)
        {
            memcpy(&cur, &c0, sizeof(cur));
            SPG_SerialSpriter(&cur, &edge1, count);
        }
        if ((params->flags & 2) != 0)
        {
            memcpy(&cur, &c1, sizeof(cur));
            SPG_l_SubtractVertexAndColor(&edge1, &c2, &c1);
            SPG_l_DivideVertexAndColor(&edge1, divides);
            SPG_SerialSpriter(&cur, &edge1, countSaved);
        }
        if ((params->flags & 4) != 0)
        {
            memcpy(&cur, &c0, sizeof(cur));
            SPG_l_SubtractVertexAndColor(&edge1, &c2, &c0);
            SPG_l_DivideVertexAndColor(&edge1, divides);
            SPG_SerialSpriter(&cur, &edge1, countSaved);
        }
    }
}