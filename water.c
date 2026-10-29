#include "memory.h"
#include "time.h"
#include "win/wingdi.h"
#include "water.h"
#include "dbg.h"
//

float FFT_FACTORS_C[] = {
        -1.0f, 0.0f, 0.0f, 1.0f, 0.70710677f, 0.70710677f, 0.9238795f,
        0.38268343f, 0.98078525f, 0.19509032f, 0.99518472f, 0.098017141f,
        0.99879545f, 0.049067672f, 0.99969882f, 0.024541229f, 0.99992472f,
        0.012271538f, 0.99998116f, 0.0061358851f, 0.99999529f,
        0.003067957f, 0.99999881f, 0.00153398f, 0.9999997f, 0.00076699001f };

void WTR_Get_A_DifVector(Vec3* a1, Vec3* a2, int a3)
{
    int v3; // eax
    int v4; // esi
    tWtrStruct2* v5; // edi
    float v6; // st7
    float v7; // st7
    float v8; // st6
    float v9; // st5
    int v10; // eax
    float v11; // st6
    tWtrStruct2* v12; // edx
    float v13; // st4
    float v14; // st3
    float v15; // st2
    tWtrStruct2* v16; // edx
    tWtrStruct2* v17; // eax
    float v18; // st5
    BOOL v19; // zf
    float v20; // [esp+10h] [ebp-70h]
    float v21; // [esp+14h] [ebp-6Ch]
    float v22; // [esp+18h] [ebp-68h]
    int v23; // [esp+1Ch] [ebp-64h]
    double v24; // [esp+20h] [ebp-60h]
    float v25; // [esp+2Ch] [ebp-54h]
    double v26; // [esp+30h] [ebp-50h]
    float v27; // [esp+38h] [ebp-48h]
    float v28; // [esp+3Ch] [ebp-44h]
    float v29; // [esp+3Ch] [ebp-44h]
    float v30; // [esp+3Ch] [ebp-44h]
    float v31; // [esp+40h] [ebp-40h]
    float v32; // [esp+40h] [ebp-40h]
    float v33; // [esp+40h] [ebp-40h]
    float v34; // [esp+40h] [ebp-40h]
    Vec3 v35; // [esp+44h] [ebp-3Ch]
    float v36; // [esp+48h] [ebp-38h]
    float v37; // [esp+50h] [ebp-30h]
    float v38; // [esp+54h] [ebp-2Ch]
    float v39; // [esp+58h] [ebp-28h]
    float v40; // [esp+5Ch] [ebp-24h]
    float v41; // [esp+60h] [ebp-20h]
    float v42; // [esp+64h] [ebp-1Ch]
    float v43; // [esp+68h] [ebp-18h]
    float v44; // [esp+6Ch] [ebp-14h]
    float v45; // [esp+70h] [ebp-10h]
    float v46; // [esp+78h] [ebp-8h]
    float v47; // [esp+7Ch] [ebp-4h]

    if (!pst_GlobalsWaterParams || pst_GlobalsWaterParams->waterEngine.useCloudShadingforColors)
    {
        a2->x = 0.0;
        a2->y = 0.0;
        a2->z = -1000000000.0f;
    }
    else
    {
        v36 = a1->y;
        v3 = 1;
        if (a3)
            v3 = 5;
        v4 = 1 << pst_GlobalsWaterParams->waterEngine.borderSizePowerOfTwo;
        v5 = pst_GlobalsWaterParams->concatenatedMap;
        v23 = v3;
        v21 = v4 * pst_GlobalsWaterParams->tiling;
        v6 = 1.0f / pst_GlobalsWaterParams->tiling;
        v22 = v6 * pst_GlobalsWaterParams->choppy;
        v25 = v6 * pst_GlobalsWaterParams->waveHeight;
        v7 = a1->x;
        do
        {
            v8 = (v7 + 40000.0) * v21;
            v9 = (v36 + 40000.0) * v21;
            v26 = v8 - 0.5 + MATH_gd_magicDoubleForRound;
            v24 = v9 - 0.5 + MATH_gd_magicDoubleForRound;
            v20 = v8 - (*(int32_t*)&v26);
            v10 = (((*(uint32_t *)&v24) & (v4 - 1)) << pst_GlobalsWaterParams->waterEngine.borderSizePowerOfTwo)
                + ((*(uint32_t*)&v24) & (v4 - 1))
                + ((*(uint32_t*)&v26) & (v4 - 1));
            v11 = v9 - (double)(*(int32_t*)&v24);
            v12 = &v5[v10];
            v13 = 1.0 - v20;
            v14 = v13 * (1.0 - v11);
            v28 = v12->f2 * v14;
            v31 = v12->f3 * v14;
            v15 = (1.0 - v11) * v20;
            v40 = v15 * v12[1].f1;
            v41 = v12[1].f2 * v15;
            v42 = v12[1].f3 * v15;
            v27 = v40 + v14 * v12->f1;
            v16 = &v5[v10 + 1 + v4];
            v29 = v41 + v28;
            v17 = &v5[v10 + 2 + v4];
            v32 = v42 + v31;
            v46 = v16->f2 * (v13 * v11);
            v47 = v16->f3 * (v13 * v11);
            v30 = v46 + v29;
            v33 = v47 + v32;
            v18 = v11 * v20;
            v43 = v18 * v17->f1;
            v44 = v17->f2 * v18;
            v45 = v17->f3 * v18;
            v34 = v45 + v33;
            v37 = v22 * (v43 + v13 * v11 * v16->f1 + v27);
            v38 = v22 * (v44 + v30);
            v39 = v25 * v34 + pst_GlobalsWaterParams->waterHeight;
            v7 = v7 + (a1->x - (v37 + v7)) * 0.5;
            v19 = v23-- == 1;
            v36 = (a1->y - (v38 + v36)) * 0.5 + v36;
        } while (!v19);
        v35.y = v38 + v36;
        if (a3)
        {
            v35.x = v37 + v7;
            v35.z = a1->z;
            *a2 = v35;
        }
        else
        {
            a2->x = v37;
            a2->y = v38;
        }
        a2->z = v39;
    }
}

void WTR_Get_A_MrmVector(Vec3* a1, Vec3* a2)
{
    tWaterParamsGlobalStruct* v2;
    float v3;
    float v4;
    float v5;
    float v6;
    int v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    Vec3 v15; 
    Vec3 v16; 
    Vec3 v17; 

    a2->x = 0.0;
    a2->y = 0.0;
    a2->z = 1.0;
    v2 = pst_GlobalsWaterParams;
    if (pst_GlobalsWaterParams)
    {
        if (!pst_GlobalsWaterParams->waterEngine.useCloudShadingforColors)
        {
            WTR_Get_A_DifVector(a1, &v16, 1);
            v3 = a1->y;
            v4 = a1->z;
            v15.x = a1->x;
            v5 = a1->x;
            v15.z = v4;
            v6 = a1->z;
            v17.x = v5;
            v17.z = v6;
            v7 = v2->waterEngine.borderSizePowerOfTwo;
            v15.y = v3;
            v17.y = a1->y;
            v8 = 1.0f / (1 << v7) * (1.0f / v2->tiling) * 1.5f;
            v17.y = v17.y + v8;
            v15.x = v8 + v15.x;
            WTR_Get_A_DifVector(&v17, &v17, 1);
            WTR_Get_A_DifVector(&v15, &v15, 1);
            v9 = v16.x - v17.x;
            v10 = v16.y - v17.y;
            v11 = v16.z - v17.z;
            v15.x = v16.x - v15.x;
            v15.y = v16.y - v15.y;
            v12 = v16.z - v15.z;
            v16.x = v15.y * v11 - v12 * v10;
            v16.y = v12 * v9 - v11 * v15.x;
            v13 = v15.x * v10 - v15.y * v9;
            v14 = 1.0f / (float)sqrt(v13 * v13 + v16.y * v16.y + v16.x * v16.x);
            a2->x = v16.x * v14;
            a2->y = v16.y * v14;
            a2->z = v13 * v14;
        }
    }
}

int WTR_Vector_2_Color(Vec3* a1)
{
    unsigned int v1;
    unsigned int v2;
    unsigned int v3;

    v1 = (unsigned int)(a1->x * 255.0);
    v2 = (unsigned int)(a1->y * 255.0);
    v3 = (unsigned int)(a1->z * 255.0);
    if (v1 > 0xFF)
        v1 = 255;
    if (v2 > 0xFF)
        v2 = 255;
    if (v3 > 0xFF)
        v3 = 255;
    return v1 | ((v2 | (v3 << 8)) << 8);
}


void WTR_SetDefault(tWaterParamsGlobalStruct* a1)
{
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;

    memset(a1, 0, sizeof(tWaterParamsGlobalStruct));
    a1->waterEngine.waveMapShift = 6;
    a1->waterEngine.waveMapShiftCHP = 6;
    a1->waterEngine.borderSizePowerOfTwo = 6;
    a1->waterEngine.borderSizeDXDYPowerOfTwo = 6;
    a1->choppy = 0.30000001f;
    a1->disturbance = 0.30000001f;
    a1->speed = 0.30000001f;
    v3.x = 0.30000001;
    a1->waterEngine.wind.x = 60.0f;
    a1->waterEngine.wind.y = 0.0f;
    a1->waterEngine.direction = 1.0f;
    a1->waterEngine.worldSizeInMeters = 10.0f;
    a1->waterEngine.waveProp = 2.0f;
    a1->waveHeight = 0.13f;
    a1->tiling = 0.0099999998f;
    a1->waterHeight = -1.0f;
    a1->waterFog = 50.0f;
    a1->horizontalVertexNum = 50;
    a1->verticallVertexNum = 120;
    a1->waterEngine.useCloudShadingforColors = 0;
    a1->waterEngine.isSymmetryEnabled = 1;
    v1.x = 0.60000002f;
    v1.y = 0.60000002f;
    v1.z = 0.60000002f;
    v3.y = 0.40000001f;
    v3.z = 0.5f;
    v2.x = 0.030000001f;
    v2.y = 0.040000003f;
    v2.z = 0.050000001f;
    a1->waterEngine.skyColor = WTR_Vector_2_Color(&v1);
    a1->waterEngine.bottomColor = WTR_Vector_2_Color(&v2);
    a1->waterEngine.waterColor = WTR_Vector_2_Color(&v3);
}

void* WTR_AllocAlign(int size, int alignment)
{
    void* ptr;

    ptr = MEM_p_AllocAligned(size, alignment);
    memset(ptr, 0, size);
    return ptr;
}

void WTR_AllocSubStruct(tWaterParamsGlobalStruct* a1)
{
    unsigned int* v2;
    unsigned int v3;
    tFFTComplex* v4;
    unsigned int v5;
    tFFTComplex* v6;
    unsigned int v7;
    unsigned int v8;

    a1->vertices = (Vec3*)WTR_AllocAlign(sizeof(Vec3) * a1->horizontalVertexNum * (a1->verticallVertexNum + 1), 16);
    a1->vecsForUVs = (Vec3*)WTR_AllocAlign(sizeof(Vec3) * a1->horizontalVertexNum * (a1->verticallVertexNum + 1), 16);
    a1->verticesForDisplay = (Vec3*)WTR_AllocAlign(sizeof(Vec3) * a1->horizontalVertexNum * (a1->verticallVertexNum + 1), 16);
    v2 = (unsigned int*)WTR_AllocAlign(4 * a1->horizontalVertexNum * (a1->verticallVertexNum + 1), 16);
    v3 = 2 * a1->waterEngine.waveMapShift;
    a1->colors = v2;
    v4 = (tFFTComplex*)WTR_AllocAlign(sizeof(tFFTComplex) * (1 << v3), 16);
    v5 = a1->waterEngine.waveMapShiftCHP;
    a1->spaceMap = v4;
    v6 = (tFFTComplex*)WTR_AllocAlign(sizeof(tFFTComplex) * (1 << (2 * v5)), 16);
    v7 = 2 * a1->waterEngine.waveMapShiftCHP;
    a1->spaceMapDX = v6;
    a1->spaceMapDY = (tFFTComplex*)WTR_AllocAlign(sizeof(tFFTComplex) * (1 << v7), 16);
    v8 = (1 << (2 * a1->waterEngine.waveMapShift)) + (2 << a1->waterEngine.waveMapShift) + 1;
    a1->concatenatedMap = (tWtrStruct2*)WTR_AllocAlign(sizeof(tWtrStruct2) * (v8 + (v8 >> 1)), 32);
}

void WTR_FreeSubStruct(tWaterParamsGlobalStruct* wtr)
{
    if (wtr->vertices)
        MEM_FreeAlign(wtr->vertices);
    if (wtr->vecsForUVs)
        MEM_FreeAlign(wtr->vecsForUVs);
    if (wtr->verticesForDisplay)
        MEM_FreeAlign(wtr->verticesForDisplay);
    wtr->vecsForUVs = NULL;
    wtr->verticesForDisplay = NULL;
    if (wtr->colors)
        MEM_FreeAlign(wtr->colors);
    if (wtr->spaceMap)
        MEM_FreeAlign(wtr->spaceMap);
    if (wtr->spaceMapDX)
        MEM_FreeAlign(wtr->spaceMapDX);
    if (wtr->spaceMapDY)
        MEM_FreeAlign(wtr->spaceMapDY);
    if (wtr->concatenatedMap)
        MEM_FreeAlign(wtr->concatenatedMap);
    if (wtr->waterEngine.spectroMap)
        MEM_FreeAlign(wtr->waterEngine.spectroMap);
    if (wtr->waterEngine.DXDYMap)
        MEM_FreeAlign(wtr->waterEngine.DXDYMap);
    wtr->vertices = NULL;
    wtr->colors = NULL;
    wtr->spaceMap = NULL;
    wtr->spaceMapDX = NULL;
    wtr->spaceMapDY = NULL;
    wtr->concatenatedMap = NULL;
    wtr->waterEngine.spectroMap = NULL;
    wtr->waterEngine.DXDYMap = NULL;
}

void* WATERFFT_Alloc(uint32_t size)
{
    return MEM_p_AllocAligned(size, 32);
}

float WATERFFT_philippe(Vec2* a1, Vec2* a2, float waveProp, float direction)
{
    float vecLen;
    double base;

    vecLen = (float)sqrt(a1->x * a1->x + a1->y * a1->y);
    if (vecLen == 0.0f)
        return 0.0f;

    // The cosine term is exactly -1 where the wavevector opposes the wind, so
    // this base is exactly zero there in exact arithmetic. Rounding leaves a
    // residue either side of zero, and a fractional power of a negative base
    // is a NaN that the later normalisation spreads over the whole spectrum.
    base = (a2->y * a1->y + a1->x * a2->x) * 0.5 / (sqrt(a2->x * a2->x + a2->y * a2->y) * vecLen) + 0.5;
    if (base < 0.0)
        base = 0.0;

    return (float)(pow(base, direction) / pow(vecLen, waveProp));
}

void WATERFFT_Normalize(tFFTComplex* a1, int a2)
{
    double v2; // st7
    unsigned int v3; // ecx
    unsigned int v4; // esi
    tFFTComplex* v5; // eax
    double v6; // st7
    double v7; // st7
    unsigned int i; // eax

    v2 = 0.0;
    v3 = 0;
    if (a2 >= 4)
    {
        v4 = 3;
        v5 = a1 + 2;
        do
        {
            v4 += 4;
            v3 += 4;
            v6 = v2 + v5[-2].real + v5[-1].real + v5->real;
            v5 += 4;
            v2 = v6 + v5[-3].real;
        } while (v4 < a2);
    }
    for (; v3 < a2; ++v3)
        v2 = v2 + a1[v3].real;
    v7 = 1.0 / v2;
    for (i = 0; i < a2; a1[i - 1].real = v7 * a1[i - 1].real)
        ++i;
}

void WATERFFT_SwapXY(tFFTComplex* a1, char a2, int a3, int a4)
{
    tFFTComplex* v4; // ebx
    int v5; // edx
    tFFTComplex* v6; // eax
    tFFTComplex* v7; // ecx
    int v8; // ebx
    float v9; // esi
    float v10; // edi
    tFFTComplex* v11; // eax
    tFFTComplex* v12; // ecx
    int v13; // ebx
    int v14; // ecx
    float v15; // esi
    float v16; // edi

    v4 = a1;
    v5 = 1 << a2;
    v6 = &a1[a3 << a2];
    if (1 << a2)
    {
        v7 = &v6[(a4 << a2) - (a3 << a2)];
        v8 = 1 << a2;
        do
        {
            v9 = v6->real;
            v10 = v6->imaginary;
            *v6 = *v7;
            v7->real = v9;
            v7->imaginary = v10;
            ++v6;
            ++v7;
            --v8;
        } while (v8);
        v4 = a1;
    }
    v11 = &v4[a3];
    v12 = &v4[a4];
    if (v5)
    {
        v13 = 8 * v5;
        v14 = (char*)v12 - (char*)v11;
        do
        {
            v15 = v11->real;
            v16 = v11->imaginary;
            *v11 = *(tFFTComplex*)((char*)v11 + v14);
            *(float*)((char*)&v11->real + v14) = v15;
            *(float*)((char*)&v11->imaginary + v14) = v16;
            v11 = (tFFTComplex*)((char*)v11 + v13);
            --v5;
        } while (v5);
    }
}

void WATERFFT_Pretranspose_Table(tFFTComplex* a1, int a2)
{
    int v2; // esi
    int i; // edi
    int j; // eax

    v2 = 0;
    for (i = 0; i < (1 << a2) - 1; ++i)
    {
        if (i < v2)
            WATERFFT_SwapXY(a1, a2, i, v2);
        for (j = 1 << a2 >> 1; j <= v2; j >>= 1)
            v2 -= j;
        v2 += j;
    }
}

int WATERFFT_Compress_Spectromap(tFFTComplex* a1, int a2)
{
    WATERFFT_Pretranspose_Table(a1, a2);
    return 1 << (2 * a2);
}

void WATERFFT_SWAP(int* a1, int a2, int a3)
{
    int v3; // edx

    v3 = a1[a2];
    a1[a2] = a1[a3];
    a1[a3] = v3;
}

void WATERFFT_Compute_Supershift(tWaterEngineStruct* a1)
{
    int v1; // ecx
    unsigned int v2; // edi
    BOOL v3; // zf
    BOOL v4; // cc
    int v5; // ebp
    int v6; // eax
    int v7; // ebx
    int v8; // esi
    int v9; // edi
    int v10; // ebp
    int j; // eax
    int v12; // esi
    int k; // edi
    int l; // eax
    int v15; // eax
    int v16; // ebx
    int v17; // ebp
    int v18; // edi
    int v19; // ecx
    int v20; // eax
    int v21; // edx
    int i; // [esp+4h] [ebp-808h]
    char v23; // [esp+8h] [ebp-804h]
    int v24[256]; // [esp+Ch] [ebp-800h] BYREF
    int v25[256]; // [esp+40Ch] [ebp-400h] BYREF

    v1 = a1->borderSizePowerOfTwo;
    v2 = a1->borderSizeDXDYPowerOfTwo;
    v3 = v2 == a1->borderSizePowerOfTwo;
    v4 = v2 <= a1->borderSizePowerOfTwo;
    a1->superShift0 = 0;
    a1->superShift1 = 1;
    a1->superShift2 = 1;
    v23 = v2;
    if (!v3 && v4)
    {
        v5 = 1 << v1;
        v6 = 0;
        for (i = 1 << v1; v6 < (1 << v1) - 1; ++v6)
        {
            v25[v6] = (1 << v2 >> 1) - (v5 >> 1) + v6;
            v24[v6] = v6;
        }
        v7 = v5 >> 1;
        v8 = 0;
        v9 = 0;
        v10 = v5 - 1;
        if (v10 > 0)
        {
            do
            {
                if (v9 < v8)
                    WATERFFT_SWAP(v25, v9, v8);
                for (j = v7; j <= v8; j >>= 1)
                    v8 -= j;
                v8 += j;
                ++v9;
            } while (v9 < v10);
        }
        v12 = 0;
        for (k = 0; k < (1 << v23) - 1; ++k)
        {
            if (k < v12)
                WATERFFT_SWAP(v24, k, v12);
            for (l = 1 << v23 >> 1; l <= v12; l >>= 1)
                v12 -= l;
            v12 += l;
        }
        v15 = 0;
        if (i - 1 > 0)
        {
            v16 = v24[2];
            v17 = v24[1];
            v18 = v24[0];
            do
            {
                v19 = v25[v15];
                if (v19 == v18)
                    a1->superShift0 = v15;
                if (v19 == v17)
                    a1->superShift1 = v15;
                if (v19 == v16)
                    a1->superShift2 = v15;
                ++v15;
            } while (v15 < i - 1);
        }
        v20 = a1->superShift1;
        v21 = a1->superShift0;
        a1->superShift2 -= v20;
        a1->superShift1 = v20 - v21;
    }
}

void WATERFFT_Precalc_DXDY_Repartition(tWaterEngineStruct* a1)
{
    int v2; // eax
    int v3; // ebx
    int v4; // edi
    double v5; // st7
    int v6; // edx
    double v7; // st5
    int v8; // ecx
    long double v9; // st4
    long double v10; // st4
    int v11; // [esp+4h] [ebp-4h]
    int a1a; // [esp+Ch] [ebp+4h]

    v2 = a1->borderSizeDXDYPowerOfTwo;
    if (v2)
    {
        if (!a1->DXDYMap)
            a1->DXDYMap = (tFFTComplex*)WATERFFT_Alloc(8 << (2 * v2));
        v3 = 1 << a1->borderSizeDXDYPowerOfTwo;
        v4 = 0;
        if (v3 > 0)
        {
            v11 = -(v3 >> 1);
            do
            {
                v5 = (double)v11;
                v6 = 0;
                a1a = -(v3 >> 1);
                do
                {
                    v7 = (double)a1a;
                    v8 = a1->borderSizeDXDYPowerOfTwo;
                    v9 = v7 * v7 + v5 * v5;
                    if (v9 == 0.0)
                    {
                        a1->DXDYMap[v6 + (v4 << v8)].real = 0.0;
                        a1->DXDYMap[v6 + (v4 << a1->borderSizeDXDYPowerOfTwo)].imaginary = 0.0;
                    }
                    else
                    {
                        v10 = 1.0 / sqrt(v9);
                        a1->DXDYMap[v6 + (v4 << v8)].real = v10 * v7;
                        a1->DXDYMap[v6 + (v4 << a1->borderSizeDXDYPowerOfTwo)].imaginary = v10 * v5;
                    }
                    ++v6;
                    ++a1a;
                } while (v6 < v3);
                ++v4;
                ++v11;
            } while (v4 < v3);
        }
        WATERFFT_Pretranspose_Table(a1->DXDYMap, a1->borderSizeDXDYPowerOfTwo);
        WATERFFT_Compute_Supershift(a1);
    }
}

void WATERFFT_FFT_COMPUTE_COST(int num, tFFTComplexCost* a2)
{
    int v2; // esi
    int currNum; // edi
    float* v5; // edx
    double v6; // st7
    int v7; // ecx
    double v8; // st6
    float v9; // [esp+4h] [ebp-10h]
    float i; // [esp+8h] [ebp-Ch]
    float v11; // [esp+Ch] [ebp-8h]

    v2 = 1;
    if (num > 0)
    {
        currNum = num;
        v5 = FFT_FACTORS_C;
        do
        {
            v6 = 0.0;
            v7 = v2;
            v9 = *v5;
            v2 *= 2;
            v11 = 1.0;
            for (i = v5[1]; v7; v11 = v8)
            {
                a2->imaginary1 = v6;
                a2->real1 = v11;
                a2->imaginary2 = v11;
                a2->real2 = -v6;
                ++a2;
                --v7;
                v8 = v9 * v11 - i * v6;
                v6 = v6 * v9 + i * v11;
            }
            v5 += 2;
            --currNum;
        } while (currNum);
    }
}

void WATERFFT_Init_Spectre(tWaterEngineStruct* a1)
{
    int v2; // eax
    int v3; // ebx
    tFFTComplex* v4; // edi
    float v5; // edx
    double v6; // st7
    double v7; // st6
    double v8; // st7
    double v9; // st5
    int v10; // ecx
    int v11; // edx
    int v12; // eax
    int v13; // edi
    int v14; // ebx
    unsigned int v15; // ebx
    unsigned int v16; // edi
    int v17; // ebp
    double v18; // st7
    long double v19; // st7
    int v20; // eax
    tFFTComplex* v21; // edx
    BOOL v22; // cf
    int v23; // eax
    double v24; // st7
    long double v25; // st7
    float v26; // [esp-10h] [ebp-30h]
    int i; // [esp+4h] [ebp-1Ch]
    unsigned int v28; // [esp+4h] [ebp-1Ch]
    int v29; // [esp+8h] [ebp-18h]
    float v30; // [esp+Ch] [ebp-14h]
    float v31; // [esp+Ch] [ebp-14h]
    float v32; // [esp+10h] [ebp-10h]
    float v33; // [esp+14h] [ebp-Ch]
    Vec2 v34; // [esp+18h] [ebp-8h] BYREF
    int a1a; // [esp+24h] [ebp+4h]
    float a1d; // [esp+24h] [ebp+4h]
    int a1b; // [esp+24h] [ebp+4h]
    float a1e; // [esp+24h] [ebp+4h]
    int a1c; // [esp+24h] [ebp+4h]

    if (!a1->spectroMap)
        a1->spectroMap = (tFFTComplex*)WATERFFT_Alloc(8 << (2 * LOBYTE(a1->borderSizePowerOfTwo)));
    v2 = 0;
    for (i = 0; v2 < 1 << a1->borderSizePowerOfTwo; i = v2)
    {
        v3 = 0;
        a1a = 0;
        v4 = &a1->spectroMap[v2 << a1->borderSizePowerOfTwo];
        if (1 << a1->borderSizePowerOfTwo > 0)
        {
            do
            {
                v5 = a1->waveProp;
                v26 = a1->direction;
                v6 = (double)(1 << a1->borderSizePowerOfTwo) * 0.5;
                v7 = 6.2831855 / a1->worldSizeInMeters;
                v30 = (float)i;
                v34.x = (v30 - v6) * v7;
                v34.y = ((double)a1a - v6) * v7;
                a1d = WATERFFT_philippe(&v34, &a1->wind, v5, v26);
                v4->real = a1d;
                v8 = WATERFFT_philippe(&v34, &a1->wind, a1->waveProp, a1->direction);
                v4->imaginary = v8;
                ++v4;
                v9 = a1d * a1d;
                a1a = ++v3;
                v4[-1].real = sqrt(v8 * v8 + v9);
            } while (v3 < 1 << a1->borderSizePowerOfTwo);
            v2 = i;
        }
        ++v2;
    }
    v10 = a1->borderSizePowerOfTwo;
    if (a1->borderSizePowerOfTwo > 6u)
    {
        v11 = 0;
        v12 = 1 << v10 >> 1;
        if (v10 - 6 > 0)
        {
            do
            {
                a1b = 0;
                if (a1->borderSizePowerOfTwo - 6 > 0)
                {
                    v13 = 1 << v10 >> 1;
                    v14 = v13;
                    do
                    {
                        a1->spectroMap[v12 + v11 + (v13 << a1->borderSizePowerOfTwo)].real = 0.0;
                        a1->spectroMap[v12 + (v13 << a1->borderSizePowerOfTwo) - v11].real = 0.0;
                        a1->spectroMap[v12 + v11 + (v14 << a1->borderSizePowerOfTwo)].real = 0.0;
                        a1->spectroMap[v12 + (v14 << a1->borderSizePowerOfTwo) - v11].real = 0.0;
                        ++v13;
                        --v14;
                        ++a1b;
                    } while (a1b < a1->borderSizePowerOfTwo - 6);
                }
                ++v11;
            } while (v11 < a1->borderSizePowerOfTwo - 6);
        }
    }
    WATERFFT_Normalize(a1->spectroMap, 1 << (2 * LOBYTE(a1->borderSizePowerOfTwo)));
    v15 = 1 << a1->borderSizePowerOfTwo;
    v28 = 0;
    if (v15)
    {
        v29 = 0;
        v31 = (double)v15 * 0.5;
        do
        {
            v16 = 0;
            v17 = v29;
            do
            {
                v18 = 6.2831855 / a1->worldSizeInMeters;// 2PI
                v33 = (double)v28 - v31;
                v19 = sqrt(sqrt(v33 * v18 * (v33 * v18) + ((double)v16 - v31) * v18 * (((double)v16 - v31) * v18)) * 9.8100004);
                a1e = v19;
                v32 = -v19;
                v20 = RND_JADErand();
                v21 = a1->spectroMap;
                //LODWORD(v34.x) = v20;
                ++v16;
                v17 += 8;
                *(float*)((char*)v21 + v17 - 4) = ((double)v20 * 0.000030518509 * (a1e - v32) + v32) * 0.0125 + a1e;
            } while (v16 < v15);
            v22 = ++v28 < v15;
            v29 += 8 * v15;
        } while (v22);
    }
    v23 = 0;
    a1c = 0;
    do
    {
        v24 = (double)a1c;
        v25 = v24 * 0.0061359233;
        COS_TABLE[2 * v23] = cos(v25);
        COS_TABLE[2 * v23 + 1] = sin(v25);
        a1c = ++v23;
    } while (v23 < 1024);
    a1->spectroMapCompressedSize = WATERFFT_Compress_Spectromap(a1->spectroMap, a1->borderSizePowerOfTwo);
    WATERFFT_Precalc_DXDY_Repartition(a1);
    WATERFFT_FFT_COMPUTE_COST(a1->borderSizePowerOfTwo, a1->FFTCostTable);
    WATERFFT_FFT_COMPUTE_COST(a1->borderSizeDXDYPowerOfTwo, a1->FFTCostTableDXDY);
}

void WTR_SetDefaulExp(struct sUserWaterParamsStruct* a1)
{
    int v1; // edx
    int v2; // ecx
    int v3; // edx
    int v4; // ecx
    int v5; // edx
    int v6; // ecx
    float v7; // edx
    float v8; // ecx
    float v9; // edx
    float v10; // ecx
    float v11; // edx
    float v12; // ecx
    float v13; // edx
    int v14; // ecx
    int v15; // edx
    int v16; // ecx
    tWaterParamsGlobalStruct a1a; // [esp+0h] [ebp-3F28h] BYREF

    WTR_SetDefault(&a1a);
    v1 = a1a.verticallVertexNum;
    a1->frustrumMeshSX = a1a.horizontalVertexNum;
    v2 = a1a.waterEngine.waveMapShift;
    a1->frustrumMeshSY = v1;
    v3 = a1a.waterEngine.waveMapShiftCHP;
    a1->waveMapShift = v2;
    v4 = a1a.waterEngine.bottomColor;
    a1->waveMapShiftCHP = v3;
    v5 = a1a.waterEngine.waterColor;
    a1->bottomColor = v4;
    v6 = a1a.waterEngine.skyColor;
    a1->waterColor = v5;
    v7 = a1a.waveHeight;
    a1->skyColor = v6;
    v8 = a1a.choppy;
    a1->waveHeight = v7;
    v9 = a1a.tiling;
    a1->choppy = v8;
    v10 = a1a.speed;
    a1->tiling = v9;
    v11 = a1a.waterHeight;
    a1->speed = v10;
    v12 = a1a.waterEngine.direction;
    a1->waterHeight = v11;
    v13 = a1a.waterEngine.waveProp;
    a1->direction = v12;
    v14 = a1a.waterEngine.useCloudShadingforColors;
    a1->waveProp = v13;
    v15 = a1a.waterEngine.textureRefId;
    a1->cludShadingIsEnabled = v14;
    v16 = a1a.waterEngine.isSymmetryEnabled;
    a1->abadcafe = 0xABADCAFE;
    a1->waterEnabled = 0;
    a1->textureRefID = v15;
    a1->isSymmetryActive = v16;
}

void WATERFFT_Deduct_DXDY_Spectre_Line(tFFTComplex* a1, tFFTComplex* a2, tFFTComplex* a3, tFFTComplex* a4, tFFTComplex* a5, int a6, int a7)
{
    tFFTComplex* v7; // edx
    tFFTComplex* v8; // esi
    tFFTComplex* v9; // ecx
    int v11; // ebx
    double v13; // st7
    double v14; // st7
    double v15; // st7

    v7 = a5;
    v8 = &a5[a6];
    v9 = a1;
    if (a1 < a3)
    {
        v11 = 8 * (a7 + a6);
        do
        {
            v9 += 2;
            v13 = a4->real * v7->imaginary;
            a2 += 2;
            a4 += 2;
            v9[-2].real = -v13;
            v9[-2].imaginary = a4[-2].real * v7->real;
            v9[-1].real = -(v8->imaginary * a4[-1].real);
            v9[-1].imaginary = a4[-1].real * v8->real;
            a2[-2].real = -(a4[-2].imaginary * v7->imaginary);
            v14 = v7->real;
            v7 = (tFFTComplex*)((char*)v7 + v11);
            a2[-2].imaginary = v14 * a4[-2].imaginary;
            a2[-1].real = -(v8->imaginary * a4[-1].imaginary);
            v15 = a4[-1].imaginary * v8->real;
            v8 = (tFFTComplex*)((char*)v8 + v11);
            a2[-1].imaginary = v15;
        } while (v9 < a3);
    }
}

void WATERFFT_CorrectHeight(tFFTComplex* a1, char a2)
{
    unsigned int v3; // edx
    int v4; // edi
    unsigned int v5; // ebx
    tFFTComplex* v6; // eax
    unsigned int v7; // ecx

    v3 = 1 << a2;
    v4 = 2;
    if (1 << a2)
    {
        v5 = v3 >> 1;
        do
        {
            v6 = (tFFTComplex*)((char*)a1 + 4 * v4);
            if (v5)
            {
                v7 = v5;
                do
                {
                    v6->real = -v6->real;
                    v6->imaginary = -v6->imaginary;
                    v6 += 2;
                    --v7;
                } while (v7);
            }
            a1 = (tFFTComplex*)((char*)v6 - 4 * v4);
            v4 ^= 2u;
            --v3;
        } while (v3);
    }
}

void WATERFFT_Deduct_DXDY_Spectre(tWaterEngineStruct* a1, tFFTComplex* a5, tFFTComplex* a3, tFFTComplex* a4)
{
    int v5; // eax
    unsigned int v6; // ebx
    tFFTComplex* v7; // ebp
    tFFTComplex* v8; // edi
    int v9; // eax
    int v10; // edx
    int v11; // ebp
    int v12; // eax
    int v13; // [esp+4h] [ebp-Ch]
    unsigned int v14; // [esp+8h] [ebp-8h]
    tFFTComplex* v15; // [esp+Ch] [ebp-4h]
    tFFTComplex* a4a; // [esp+14h] [ebp+4h]
    tFFTComplex* a5a; // [esp+18h] [ebp+8h]

    v5 = a1->borderSizeDXDYPowerOfTwo;
    if (v5)
    {
        a4a = a1->DXDYMap;
        v6 = 1 << (2 * v5);
        v7 = a4;
        v8 = a3;
        v14 = 0;
        a5a = &a5[a1->superShift0 + (a1->superShift0 << a1->borderSizePowerOfTwo)];
        v13 = 0;
        if (v6)
        {
            v9 = 1 << a1->borderSizeDXDYPowerOfTwo;
            while (1)
            {
                WATERFFT_Deduct_DXDY_Spectre_Line(v8, v7, &v8[v9], a4a, a5a, a1->superShift1, a1->superShift2);
                v10 = 1 << a1->borderSizeDXDYPowerOfTwo;
                v9 = v10;
                v15 = &v7[v10];
                v8 += v10;
                a4a += v10;
                v11 = v13 ? a1->superShift2 : a1->superShift1;
                a5a += v11 << a1->borderSizePowerOfTwo;
                v13 ^= 1u;
                v14 += v10;
                if (v14 >= v6)
                    break;
                v7 = v15;
            }
        }
        v12 = a1->borderSizeDXDYPowerOfTwo;
        if (((LOBYTE(a1->borderSizePowerOfTwo) - (byte)v12) & 1) != 0)
        {
            WATERFFT_CorrectHeight(a3, v12);
            WATERFFT_CorrectHeight(a4, a1->borderSizeDXDYPowerOfTwo);
        }
    }
}

/* Two dimensional inverse FFT of a square complex grid, in place.
 *
 * The grid is (1 << sizeLog2) on a side and arrives in bit reversed order,
 * which WATERFFT_Pretranspose_Table put it in. This is a vector radix
 * transform: each butterfly combines a two by two block rather than a pair,
 * so one pass covers both dimensions at once. Entry (y, x) of a block is
 * combined with (y, x + half), (y + half, x) and (y + half, x + half), each
 * rotated by the twiddle for its offset, and the four sums are written back
 * over the four inputs.
 *
 * The rotations are read from the table WATERFFT_FFT_COMPUTE_COST built, one
 * stage after another, so the cursor advances by the stage's entry count and
 * that count doubles. Afterwards the checkerboard negate recentres the result.
 */
void WATERFFT_FFT2D(tFFTComplex* grid, int sizeLog2, tFFTComplexCost* twiddle)
{
    const int side = 1 << sizeLog2;
    tFFTComplexCost* stage = twiddle;
    int len, blockY, blockX, offsetY, offsetX;

    for (len = 2; len <= side; len <<= 1)
    {
        const int half = len >> 1;

        for (blockY = 0; blockY < side; blockY += len)
        for (blockX = 0; blockX < side; blockX += len)
        for (offsetY = 0; offsetY < half; offsetY++)
        for (offsetX = 0; offsetX < half; offsetX++)
        {
            tFFTComplex* const topLeft = &grid[(blockY + offsetY) * side + (blockX + offsetX)];
            tFFTComplex* const topRight = topLeft + half;
            tFFTComplex* const bottomLeft = topLeft + half * side;
            tFFTComplex* const bottomRight = bottomLeft + half;

            const float acrossReal = stage[offsetX].real1;
            const float acrossImaginary = stage[offsetX].imaginary1;
            const float downReal = stage[offsetY].real1;
            const float downImaginary = stage[offsetY].imaginary1;

            /* Each of the other three turned by the rotation for its offset,
             * the diagonal one by both. */
            const float rightReal = acrossReal * topRight->real - acrossImaginary * topRight->imaginary;
            const float rightImaginary = acrossImaginary * topRight->real + acrossReal * topRight->imaginary;
            const float downwardReal = downReal * bottomLeft->real - downImaginary * bottomLeft->imaginary;
            const float downwardImaginary = downImaginary * bottomLeft->real + downReal * bottomLeft->imaginary;

            const float cornerReal = acrossReal * bottomRight->real - acrossImaginary * bottomRight->imaginary;
            const float cornerImaginary = acrossImaginary * bottomRight->real + acrossReal * bottomRight->imaginary;
            const float diagonalReal = downReal * cornerReal - downImaginary * cornerImaginary;
            const float diagonalImaginary = downImaginary * cornerReal + downReal * cornerImaginary;

            const float baseReal = topLeft->real;
            const float baseImaginary = topLeft->imaginary;

            topLeft->real = baseReal + rightReal + downwardReal + diagonalReal;
            topLeft->imaginary = baseImaginary + rightImaginary + downwardImaginary + diagonalImaginary;
            topRight->real = baseReal - rightReal + downwardReal - diagonalReal;
            topRight->imaginary = baseImaginary - rightImaginary + downwardImaginary - diagonalImaginary;
            bottomLeft->real = baseReal + rightReal - downwardReal - diagonalReal;
            bottomLeft->imaginary = baseImaginary + rightImaginary - downwardImaginary - diagonalImaginary;
            bottomRight->real = baseReal - rightReal - downwardReal + diagonalReal;
            bottomRight->imaginary = baseImaginary - rightImaginary - downwardImaginary + diagonalImaginary;
        }

        stage += half;
    }

    WATERFFT_CorrectHeight(grid, (char)sizeLog2);
}

void WATERFFT_Turn_Spectre(tWaterEngineStruct* src, tFFTComplex* dst, tFFTComplex* DX, tFFTComplex* DY, float timeElapsed)
{
    int i, idx;
    float teFactor = timeElapsed * 162.97466f;

    for (i = 0; i < src->spectroMapCompressedSize; i++)
    {
        idx = 2 * (f2iMagic(2 * teFactor * src->spectroMap[i].imaginary) & 0x3ff);
        dst[i].real = COS_TABLE[idx] * src->spectroMap[i].real;
        dst[i].imaginary = COS_TABLE[idx + 1] * src->spectroMap[i].real;
    }

    if (DX && DY && src->borderSizeDXDYPowerOfTwo)
    {
        WATERFFT_Deduct_DXDY_Spectre(src, dst, DX, DY);
        WATERFFT_FFT2D(DX, src->borderSizeDXDYPowerOfTwo, src->FFTCostTableDXDY);
        WATERFFT_FFT2D(DY, src->borderSizeDXDYPowerOfTwo, src->FFTCostTableDXDY);
        WATERFFT_FFT2D(dst, src->borderSizePowerOfTwo, src->FFTCostTable);
    }
    else
    {
        WATERFFT_FFT2D(dst, src->borderSizePowerOfTwo, src->FFTCostTable);
    }
}

void WTR_Color_2_Vector(Vec3* vec, uint32_t color)
{
    vec->x = (float)((double)((color >> 0)  & 0xff) * 0.0039215689);
    vec->y = (float)((double)((color >> 8)  & 0xff) * 0.0039215689);
    vec->z = (float)((double)((color >> 16) & 0xff) * 0.0039215689);
}

uint32_t WTR_GetFresnelColor(tWaterParamsGlobalStruct* wtr, float cosThetaI)
{
    unsigned int red, blue, green, alpha;
    Vec3 bottomColorVec, waterColorVec, skyColorVec;
    float bottomAndSkyColorFactor, bottomAndWaterColorFactor;
    float alphaNorm;
    long double v2;
    double v3;
    long double v4;
    long double v5;
    long double v6;
    long double v7;
    long double v8;
    long double v9;
    double v10;
    double v11;
    float v19;
    float v20;
    float v21;
    float a4a;

    if (cosThetaI <= 0.0)
    {
        v4 = sqrt(-cosThetaI);
        a4a = (float)v4;
        v5 = acos(v4);
        v19 = (float)v5;
        v6 = asin(sin(v5) * 0.74626863);
        v21 = (float)(v19 + v6);
        v7 = v6 - v19;
        v20 = (float)(sin(v7) / sin(v21));
        v8 = tan(v7);
        v9 = tan(v21);
        v3 = (v8 / v9 * (v8 / v9) + v20 * v20) * 0.5;
        bottomAndSkyColorFactor = (float)v3;
        bottomAndWaterColorFactor = (float)((1.0 - a4a) * (1.0 - a4a) * (1.0 - a4a));
    }
    else
    {
        v2 = sin(acos(cosThetaI)) * 1.34;
        if (v2 < 1.0)
            bottomAndSkyColorFactor = (float)(1.0 - v2);
        else
            bottomAndSkyColorFactor = 0.0;
        bottomAndWaterColorFactor = (float)((1.0 - (1.0 - cosThetaI)) * (1.0 - (1.0 - cosThetaI)));
        v3 = 1.0 - bottomAndSkyColorFactor;
    }
    WTR_Color_2_Vector(&skyColorVec, wtr->waterEngine.skyColor);
    WTR_Color_2_Vector(&bottomColorVec, wtr->waterEngine.bottomColor);
    WTR_Color_2_Vector(&waterColorVec, wtr->waterEngine.waterColor);
    MATH_BlendVector(&bottomColorVec, &bottomColorVec, &waterColorVec, bottomAndWaterColorFactor);
    MATH_BlendVector(&waterColorVec, &bottomColorVec, &skyColorVec, bottomAndSkyColorFactor);
    v10 = 1.0 - (1.0 - (1.0 - v3));
    v11 = 1.0 - v10 * v10 * (v10 * v10);
    alphaNorm = v11 * v11 * (v11 * v11);
    if (alphaNorm >= 0.0)
    {
        if (alphaNorm > 1.0)
            alphaNorm = 1.0;
    }
    else
    {
        alphaNorm = 0.0;
    }
    blue = (unsigned int)(waterColorVec.x * 255.0);
    green = (unsigned int)(waterColorVec.y * 255.0);
    red = (unsigned int)(waterColorVec.z * 255.0);
    alpha = (unsigned int)(alphaNorm * 255.0);
    if (blue > 255)
        blue = 255;
    if (green > 255)
        green = 255;
    if (red > 255)
        red = 255;
    if (alpha > 255)
        alpha = 255;
    return blue | ((green | ((red | (alpha << 8)) << 8)) << 8);
}


void WTR_ComputeFresnel(tWaterParamsGlobalStruct* wtr)
{
    int v2;
    int v3;
    int v5;
    int v6;
    int* v7;
    int* v8;
    int v9;
    double v10;
    unsigned int v11;
    double v12;
    double v13;
    float v14;
    int v15;
    Vec3 v16;
    Vec3 a1a;
    Vec3 v18;
    Vec3 v19;
    Vec3 v20;
    float v21;
    float v22;
    int v23;

    wtr->waterEngine.bottomColor &= 0x00ffffff;
    v2 = wtr->waterEngine.bottomColor | 0xff000000;
    wtr->waterEngine.waterColor &= 0x00ffffff;
    wtr->waterEngine.skyColor &= 0x00ffffff;
    a1a.x = 0.30000001;
    a1a.y = 0.40000001;
    a1a.z = 0.5;
    wtr->waterEngine.bottomColor = v2;
    v15 = v2;
    v3 = WTR_Vector_2_Color(&a1a);
    v5 = v3 | 0xFF000000;
    wtr->waterEngine.waterColor = v5;
    wtr->waterEngine.skyColor = v5;
    if (!wtr->waterEngine.colorTableFastRecomputeDone)
    {
        v6 = 256;
        wtr->waterEngine.bottomColor = v2 & 0xFFFF0000 | 0xFF0000;
        wtr->waterEngine.waterColor = v5 & 0xFF000000 | 0xFF00;
        wtr->waterEngine.skyColor = v5 & 0xFF000000 | 0xFF;
        v23 = 256;
        v7 = wtr->waterEngine.colorTableForFastRecompute;
        do
        {
            v14 = (double)v23 * (2.0 / 512.0);
            *v7++ = WTR_GetFresnelColor(wtr, v14);
            v23 = --v6;
        } while (v6 > -256);
        wtr->waterEngine.bottomColor = v15;
        wtr->waterEngine.waterColor = v5;
        wtr->waterEngine.skyColor = v5;
        wtr->waterEngine.colorTableFastRecomputeDone = 1;
    }
    WTR_Color_2_Vector(&v18, wtr->waterEngine.skyColor);
    WTR_Color_2_Vector(&v20, wtr->waterEngine.bottomColor);
    WTR_Color_2_Vector(&v19, wtr->waterEngine.waterColor);
    v8 = wtr->waterEngine.colorTable;
    v9 = 512;
    do
    {
        v10 = (double)(unsigned __int8)wtr->waterEngine.colorTableForFastRecompute[512 - v9] * 0.00390625;
        v11 = (unsigned int)wtr->waterEngine.colorTableForFastRecompute[512 - v9] >> 8;
        v16.y = v18.y * v10;
        v16.z = v18.z * v10;
        v12 = (double)(unsigned __int8)v11 * 0.00390625;
        v11 >>= 8;
        a1a.x = v19.x * v12;
        a1a.y = v19.y * v12;
        a1a.z = v19.z * v12;
        v16.z = a1a.z + v16.z;
        v13 = (double)(unsigned __int8)v11 * 0.00390625;
        v21 = v20.y * v13;
        v22 = v20.z * v13;
        v16.x = v20.x * v13 + a1a.x + v18.x * v10;
        v16.y = v21 + a1a.y + v16.y;
        v16.z = v22 + v16.z;
        *v8++ = ((v11 & 0xFFFFFF00) << 16) | WTR_Vector_2_Color(&v16);
        --v9;
    } while (v9);
    memcpy(wtr->waterEngine.colorTableBackup, &wtr->waterEngine.colorTable[224], 256u);
    wtr->waterEngine.waterColorMode = 0;
}

void WTR_Compute_MipMap_sub_noSSE(tWtrStruct2* result, tWtrStruct2* a2, tWtrStruct2* a3)
{
    tWtrStruct2* v4; // edx
    tWtrStruct2* v5; // esi
    int v6; // ebx
    double v7; // st7

    v4 = result + 1;
    v5 = a3 + 1;
    v6 = 2;
    do
    {
        ++a2;
        v7 = v5->f1 + v4->f1;
        result += 2;
        a3 += 2;
        v4 += 2;
        v5 += 2;
        --v6;
        a2[-1].f1 = (v7 + result[-2].f1 + a3[-2].f1) * 0.25;
        a2[-1].f2 = (v5[-2].f2 + v4[-2].f2 + result[-2].f2 + a3[-2].f2) * 0.25;
        a2[-1].f3 = (v5[-2].f3 + v4[-2].f3 + result[-2].f3 + a3[-2].f3) * 0.25;
    } while (v6);
}

void WTR_Compute_MipMap(int result, tWtrStruct2* a2, tWtrStruct2* a3)
{
    tWtrStruct2* v3; // edx
    unsigned int offset; // ebp
    tWtrStruct2* ptrEnd; // esi
    tWtrStruct2* ptrStart; // edi
    tWtrStruct2* v8; // edx
    float v9; // ecx
    int v10; // [esp+10h] [ebp-8h]
    tWtrStruct2* i; // [esp+14h] [ebp-4h]

    v3 = a3;
    offset = 16 * result;
    ptrEnd = (tWtrStruct2*)((char*)a3 + result * (16 * result + 16));
    ptrStart = a3;
    if (a3 < ptrEnd)
    {
        v10 = 2 * result;
        do
        {
            for (i = &ptrStart[offset / 0x10]; ptrStart < i; a2 += 4)
            {
                WTR_Compute_MipMap_sub_noSSE(a2, ptrStart, &a2[v10 + 1]);
                ptrStart += 2;
            }
            v8 = ptrStart;
            ptrStart->f1 = ptrStart[offset / 0xFFFFFFF0].f1;
            ptrStart->f2 = ptrStart[offset / 0xFFFFFFF0].f2;
            ptrStart->f3 = ptrStart[offset / 0xFFFFFFF0].f3;
            v9 = ptrStart[offset / 0xFFFFFFF0].f4;
            ++ptrStart;
            a2 = (tWtrStruct2*)((char*)a2 + v10 * 16 + 32);
            v8->f4 = v9;
        } while (ptrStart < ptrEnd);
        v3 = a3;
    }
    memcpy(ptrStart, v3, 16 * ((int)((int)&ptrEnd[1] + offset - (uint32_t)ptrStart) >> 4));
}

void WTR_BackExtendDXDY_sub_unknown(tWtrStruct2* a1, tWtrStruct2* a2, tWtrStruct2* a3, tWtrStruct2* a4)
{
    *a3 = *a2;
    MATH_BlendVector((Vec3*)a2, (Vec3*)&a2[1], (Vec3*)&a3[1], 0.5);
    MATH_BlendVector((Vec3*)a2, (Vec3*)a1, (Vec3*)a4, 0.5);
    MATH_BlendVector((Vec3*)a1, (Vec3*)&a1[1], (Vec3*)&a4[1], 0.5);
    MATH_BlendVector((Vec3*)&a4[1], (Vec3*)&a3[1], (Vec3*)&a4[1], 0.5);
}

void WTR_BackExtendDXDY_0(tWtrStruct2* a1, tWtrStruct2* a2, int a3)
{
    int v3; // ecx
    tWtrStruct2* v4; // ebp
    int v5; // esi
    tWtrStruct2* v6; // edi
    tWtrStruct2* v7; // eax
    int v8; // edx
    int v9; // esi
    int v10; // eax
    tWtrStruct2* v11; // esi
    float v12; // ecx
    int i; // [esp+10h] [ebp-10h]
    tWtrStruct2* v14; // [esp+14h] [ebp-Ch]
    tWtrStruct2* v15; // [esp+18h] [ebp-8h]

    v3 = a3;
    v4 = a2;
    v5 = a3;
    v6 = a1;
    v7 = (tWtrStruct2*)((char*)a1 + a3 * (16 * a3 + 16));
    v15 = v7;
    if (v6 < v7)
    {
        v8 = 32 * a3;
        while (1)
        {
            v14 = &v6[v5];
            if (v6 < &v6[v5])
            {
                v9 = v3 + 1;
                v10 = 16 * (2 * v3 + 1);
                for (i = v10; ; v10 = i)
                {
                    WTR_BackExtendDXDY_sub_unknown(&v6[v9], v6, v4, (tWtrStruct2*)((char*)v4 + v10));
                    ++v6;
                    v4 += 2;
                    if (v6 >= v14)
                        break;
                }
                v7 = v15;
                v8 = 32 * a3;
            }
            v4->f1 = v4[v8 / 0xFFFFFFF0].f1;
            v4->f2 = v4[v8 / 0xFFFFFFF0].f2;
            v4->f3 = v4[v8 / 0xFFFFFFF0].f3;
            v4->f4 = v4[v8 / 0xFFFFFFF0].f4;
            v11 = &v4[v8 / 0x10u + 1];
            v11->f1 = v11[v8 / 0xFFFFFFF0].f1;
            v11->f2 = v11[v8 / 0xFFFFFFF0].f2;
            v11->f3 = v4[1].f3;
            v12 = v4[1].f4;
            ++v6;
            v4 = v11 + 1;
            v11->f4 = v12;
            v3 = a3;
            if (v6 >= v7)
                break;
            v5 = a3;
        }
    }
    memcpy(v4, a2, 4 * ((unsigned int)(16 * ((32 * v3 + 16) >> 4)) >> 2));
}

void WTR_Concate(tWaterParamsGlobalStruct* a1)
{
    tWtrStruct2* v1; // eax
    tFFTComplex* v2; // ebp
    unsigned int v3; // ecx
    tFFTComplex* v4; // edx
    tFFTComplex* v5; // ebx
    tWtrStruct2* v6; // edi
    tWtrStruct2* v7; // edi
    float* v8; // ecx
    tWtrStruct2* i; // edi
    float* v10; // ecx
    tWtrStruct2* v11; // ecx
    unsigned int v12; // edi
    tWtrStruct2* v13; // ecx
    int v14; // ebp
    unsigned int v15; // edi
    tWtrStruct2* v16; // ebp
    unsigned int v17; // ecx
    tWtrStruct2* v18; // eax
    tWtrStruct2* v19; // edi
    tWtrStruct2* v20; // edx
    float* v21; // ecx
    uint32_t* v22; // edx
    float* v23; // ecx
    tWtrStruct2* v24; // ebx
    int v25; // edi
    tWtrStruct2* k; // eax
    int v27; // eax
    tWtrStruct2* a2; // [esp+Ch] [ebp-14h]
    tWtrStruct2* a2a; // [esp+Ch] [ebp-14h]
    tWtrStruct2* a2b; // [esp+Ch] [ebp-14h]
    tFFTComplex* v31; // [esp+10h] [ebp-10h]
    tFFTComplex* v32; // [esp+14h] [ebp-Ch]
    int a3; // [esp+18h] [ebp-8h]
    unsigned int j; // [esp+1Ch] [ebp-4h]

    v1 = a1->concatenatedMap;
    v2 = a1->spaceMapDY;
    v3 = a1->waterEngine.waveMapShift;
    v4 = a1->spaceMap;
    v31 = v2;
    v5 = a1->spaceMapDX;
    v6 = &v1[(1 << v3) + (1 << (2 * v3))];
    a2 = v6;
    v32 = v5;
    if (v3 == a1->waterEngine.waveMapShiftCHP)
    {
        if (v1 < v6)
        {
            while (1)
            {
                v7 = &v1[1 << a1->waterEngine.waveMapShift];
                if (v1 < v7)
                {
                    do
                    {
                        v1->f1 = v5->real;
                        v1->f2 = v2->real;
                        v1->f3 = v4->real;
                        ++v1;
                        ++v5;
                        ++v2;
                        ++v4;
                    } while (v1 < v7);
                    v31 = v2;
                }
                v8 = &v1[-(1 << a1->waterEngine.waveMapShift)].f1;
                v1->f1 = *v8;
                v1->f2 = v8[1];
                v1->f3 = v8[2];
                v1->f4 = v8[3];
                v6 = a2;
                if (++v1 >= a2)
                    break;
                v2 = v31;
            }
            v32 = v5;
        }
    }
    else if (v1 < v6)
    {
        do
        {
            for (i = &v1[1 << a1->waterEngine.waveMapShift]; v1 < i; v4 += 4)
            {
                v1->f3 = v4->real;
                v1[1].f3 = v4[1].real;
                v1[2].f3 = v4[2].real;
                v1[3].f3 = v4[3].real;
                v1 += 4;
            }
            v10 = &v1[-(1 << a1->waterEngine.waveMapShift)].f1;
            v1->f1 = *v10;
            v1->f2 = v10[1];
            v1->f3 = v10[2];
            v1->f4 = v10[3];
            v6 = a2;
            ++v1;
        } while (v1 < a2);
    }
    v11 = a1->concatenatedMap;
    for (a2a = &v6[(1 << a1->waterEngine.waveMapShift) + 1]; v1 < a2a; ++v11)
    {
        v1->f1 = v11->f1;
        v1->f2 = v11->f2;
        v1->f3 = v11->f3;
        v1->f4 = v11->f4;
        ++v1;
    }
    v12 = 1 << a1->waterEngine.waveMapShift;
    v13 = a1->concatenatedMap;
    a2b = v13;
    v14 = v12 * (v12 + 2) + 1;
    v15 = v12 >> 1;
    v16 = &v13[v14];
    a3 = v15;
    if (v15)
    {
        while (1)
        {
            WTR_Compute_MipMap(v15, v13, v16);
            v17 = a1->waterEngine.waveMapShiftCHP;
            if (v15 == 1 << v17 && v17)
            {
                v18 = v16;
                v19 = &v16[(1 << v17) + (1 << (2 * v17))];
                if (v16 < v19)
                {
                    while (1)
                    {
                        v20 = &v18[1 << a1->waterEngine.waveMapShiftCHP];
                        if (v18 < v20)
                        {
                            do
                            {
                                v18->f1 = v5->real;
                                v18->f2 = v31->real;
                                ++v18;
                                ++v5;
                                ++v31;
                            } while (v18 < v20);
                            v32 = v5;
                        }
                        v21 = &v18[-(1 << a1->waterEngine.waveMapShiftCHP)].f1;
                        v22 = (uint32_t*)&v18->f1;
                        v18->f1 = *v21;
                        v18->f2 = v21[1];
                        v18->f3 = v21[2];
                        ++v18;
                        v22[3] = *((uint32_t*)v21 + 3);
                        if (v18 >= v19)
                            break;
                        v5 = v32;
                    }
                }
                v23 = &v16->f1;
                for (j = (unsigned int)&v19[(1 << a1->waterEngine.waveMapShiftCHP) + 1]; (unsigned int)v18 < j; v23 += 4)
                {
                    v18->f1 = *v23;
                    v18->f2 = v23[1];
                    v18->f3 = v23[2];
                    v18->f4 = v23[3];
                    ++v18;
                }
                v24 = a2b;
                v25 = a3;
                for (k = v16; v24 >= a1->concatenatedMap; v24 = (tWtrStruct2*)((char*)v24 - 16 - ((v25 * (v25 + 1)) << 6)))
                {
                    WTR_BackExtendDXDY_0(k, v24, v25);
                    v25 *= 2;
                    k = v24;
                }
                v15 = a3;
                v5 = v32;
            }
            v27 = v15 * (v15 + 2);
            v15 >>= 1;
            a2b = v16;
            v16 += v27 + 1;
            a3 = v15;
            if (!v15)
                break;
            v13 = a2b;
        }
    }
}

void WTR_SetExportParams(tUserWaterParamsStruct* a1)
{
    tWaterParamsGlobalStruct* v1; // eax
    int v2; // ebp
    int v3; // ebx
    unsigned int v4; // edi
    unsigned int v5; // ecx
    unsigned int v6; // edx
    int v7; // [esp+10h] [ebp-8h]
    int colorChanged; // [esp+14h] [ebp-4h]

    v1 = pst_GlobalsWaterParams;
    v2 = 0;
    v3 = 0;
    v7 = 0;
    colorChanged = 0;
    if (pst_GlobalsWaterParams)
    {
        if (!a1->waterEnabled)
        {
            pst_GlobalsWaterParams = 0;
            pst_GlobalsWaterParams_Save = v1;
            return;
        }
    }
    else
    {
        if (!a1->waterEnabled)
            return;
        v1 = pst_GlobalsWaterParams_Save;
        if (pst_GlobalsWaterParams_Save)
        {
            pst_GlobalsWaterParams = pst_GlobalsWaterParams_Save;
            pst_GlobalsWaterParams_Save = 0;
        }
        else
        {
            pst_GlobalsWaterParams = (tWaterParamsGlobalStruct*)MEM_p_AllocAligned(16168, 32);
            WTR_SetDefault(pst_GlobalsWaterParams);
            v1 = pst_GlobalsWaterParams;
            v2 = 1;
            colorChanged = 1;
            v3 = 1;
            v7 = 1;
        }
    }
    if (!a1->frustrumMeshSX)
    {
        a1->frustrumMeshSX = v1->horizontalVertexNum;
        v1 = pst_GlobalsWaterParams;
    }
    if (!a1->frustrumMeshSY)
    {
        a1->frustrumMeshSY = v1->verticallVertexNum;
        v1 = pst_GlobalsWaterParams;
    }
    if (!a1->waveMapShift)
    {
        a1->waveMapShift = v1->waterEngine.waveMapShift;
        v1 = pst_GlobalsWaterParams;
    }
    if (a1->waveMapShift > 9u)
    {
        a1->waveMapShift = 9;
        v1 = pst_GlobalsWaterParams;
    }
    if (a1->waveMapShift < 4u)
    {
        a1->waveMapShift = 4;
        v1 = pst_GlobalsWaterParams;
    }
    if (a1->waveMapShiftCHP > 9u)
    {
        a1->waveMapShiftCHP = 9;
        v1 = pst_GlobalsWaterParams;
    }
    if (a1->waveMapShiftCHP < 4u)
    {
        a1->waveMapShiftCHP = 4;
        v1 = pst_GlobalsWaterParams;
    }
    v4 = a1->waveMapShift;
    if (a1->waveMapShiftCHP > v4)
    {
        a1->waveMapShiftCHP = v4;
        v1 = pst_GlobalsWaterParams;
    }
    if (a1->skyColor != v1->waterEngine.skyColor
        || a1->bottomColor != v1->waterEngine.bottomColor
        || a1->waterColor != v1->waterEngine.waterColor)
    {
        colorChanged = 1;
    }
    v5 = a1->frustrumMeshSX;
    v6 = v1->horizontalVertexNum;
    if (v5 <= v6)
    {
        if (a1->frustrumMeshSY <= (unsigned int)v1->verticallVertexNum
            && v4 <= v1->waterEngine.waveMapShift
            && a1->waveMapShiftCHP <= v1->waterEngine.waveMapShiftCHP)
        {
            v2 = v7;
            goto LABEL_35;
        }
        v2 = v7;
    }
    v3 = 1;
LABEL_35:
    if (v5 != v6
        || a1->frustrumMeshSY != v1->verticallVertexNum
        || v4 != v1->waterEngine.waveMapShift
        || a1->waveMapShiftCHP != v1->waterEngine.waveMapShiftCHP)
    {
        v2 = 1;
    }
    v1->horizontalVertexNum = v5;
    pst_GlobalsWaterParams->verticallVertexNum = a1->frustrumMeshSY;
    pst_GlobalsWaterParams->waterEngine.waveMapShift = a1->waveMapShift;
    pst_GlobalsWaterParams->waterEngine.waveMapShiftCHP = a1->waveMapShiftCHP;
    pst_GlobalsWaterParams->waterEngine.isSymmetryEnabled = a1->isSymmetryActive;
    pst_GlobalsWaterParams->waterEngine.bottomColor = a1->bottomColor;
    pst_GlobalsWaterParams->waterEngine.waterColor = a1->waterColor;
    pst_GlobalsWaterParams->waterEngine.skyColor = a1->skyColor;
    pst_GlobalsWaterParams->waveHeight = a1->waveHeight;
    pst_GlobalsWaterParams->choppy = a1->choppy;
    pst_GlobalsWaterParams->tiling = a1->tiling;
    pst_GlobalsWaterParams->speed = a1->speed;
    pst_GlobalsWaterParams->waterHeight = a1->waterHeight;
    pst_GlobalsWaterParams->waterEngine.useCloudShadingforColors = a1->cludShadingIsEnabled;
    pst_GlobalsWaterParams->waterEngine.borderSizePowerOfTwo = pst_GlobalsWaterParams->waterEngine.waveMapShift;
    pst_GlobalsWaterParams->waterEngine.borderSizeDXDYPowerOfTwo = pst_GlobalsWaterParams->waterEngine.waveMapShiftCHP;
    if (pst_GlobalsWaterParams->waterEngine.waveProp != a1->waveProp
        || pst_GlobalsWaterParams->waterEngine.direction != a1->direction)
    {
        v2 = 1;
    }
    pst_GlobalsWaterParams->waterEngine.waveProp = a1->waveProp;
    pst_GlobalsWaterParams->waterEngine.direction = a1->direction;
    pst_GlobalsWaterParams->waterEngine.textureRefId = a1->textureRefID;
    if (v3)
    {
        WTR_FreeSubStruct(pst_GlobalsWaterParams);
        WTR_AllocSubStruct(pst_GlobalsWaterParams);
    LABEL_46:
        WATERFFT_Init_Spectre(&pst_GlobalsWaterParams->waterEngine);
        WATERFFT_Turn_Spectre(
            &pst_GlobalsWaterParams->waterEngine,
            pst_GlobalsWaterParams->spaceMap,
            pst_GlobalsWaterParams->spaceMapDX,
            pst_GlobalsWaterParams->spaceMapDY,
            pst_GlobalsWaterParams->waterEngine.timeElapsed);
        WTR_Concate(pst_GlobalsWaterParams);
        goto LABEL_47;
    }
    if (v2)
        goto LABEL_46;
LABEL_47:
    if (colorChanged)
        WTR_ComputeFresnel(pst_GlobalsWaterParams);
    waterHeight = pst_GlobalsWaterParams->waterHeight;
}

float WTR_DistToPlane(Vec3* a1, Vec3* a2, float a3)
{
    return a3 - (a1->z * a2->z + a1->y * a2->y + a1->x * a2->x);
}

int WTR_Clip(Vec3* a1, Vec3* a2, Vec3* a3, float a4, int num)
{
    Vec3* v5; // esi nézd meg a full verzió ugyanilyen függvényét, ott jól jeelnik meg -- megcsináltam __spoils
    Vec3* v6; // edi
    Vec3* v7; // edx
    int v8; // ebx
    double v9; // st7
    int v10; // ebp
    char v12; // bp
    double v13; // st7
    float a4a; // [esp+0h] [ebp-18h]
    float a4b; // [esp+0h] [ebp-18h]
    Vec3* v17; // [esp+14h] [ebp-4h]
    float a1a; // [esp+1Ch] [ebp+4h]
    float v19; // [esp+2Ch] [ebp+14h]

    v5 = a1;
    v6 = &a1[num];
    v7 = v6 - 1;
    v17 = v6;
    v8 = 0;
    v9 = WTR_DistToPlane(v6 - 1, a3, a4);
    v19 = v9;
    v10 = v9 < 0.0;
    if (a1 < v6)
    {
        while (1)
        {
            v12 = 2 * v10;
            v13 = WTR_DistToPlane(v5, a3, a4);
            a1a = v13;
            if (v13 < 0.0)
                v12 |= 1u;
            v10 = v12 & 3;
            if (!v10)
                goto LABEL_8;
            if (v10 == 1)
                break;
            if (v10 == 2)
            {
                a4a = -(v19 / (a1a - v19));
                MATH_BlendVector(a2++, v7, v5, a4a);    // ez, lást felsõ komment
                ++v8;
            LABEL_8:
                *a2 = *v5;
            LABEL_9:
                ++a2;
                ++v8;
            }
            v19 = v13;
            v7 = v5++;
            if (v5 >= v17)
                return v8;
        }
        a4b = -(v19 / (a1a - v19));
        MATH_BlendVector(a2, v7, v5, a4b);
        goto LABEL_9;
    }
    return v8;
}

int WTR_Compute4Points(Vec3* points, Vec3* a2, Vec3* a3, Vec3* a4, float a5, float a6, float a7, float a8, float a9, float a10)
{
    double v10; // st7
    double v11; // st6
    double v12; // st5
    long double v13; // st5
    long double v14; // st7
    double v15; // st6
    double v16; // st3
    long double v17; // st2
    long double v18; // st4
    long double v19; // rt0
    long double v20; // st4
    double v22; // rt1
    double v23; // st6
    double v24; // st5
    double v25; // st4
    double v26; // st7
    double v27; // st6
    double v28; // st7
    double v29; // st6
    long double v30; // st5
    long double v31; // st7
    long double v32; // st7
    long double v33; // st7
    long double v34; // st7
    double v35; // st7
    double v36; // st6
    long double v37; // st7
    long double v38; // st7
    long double v39; // st7
    long double v40; // st7
    double v41; // st6
    double v42; // st3
    double v43; // st5
    double v44; // st4
    double v45; // st3
    int v46; // eax
    int v47; // eax
    int v48; // eax
    int v49; // eax
    char v50; // cl
    double v51; // st7
    double v52; // st6
    Vec3* v53; // edx
    float v54; // eax
    float v55; // edx
    Vec3* v56; // edx
    float v57; // eax
    float v58; // edx
    Vec3* v59; // edx
    float v60; // eax
    float v61; // edx
    Vec3* v62; // ecx
    float v63; // edx
    float v64; // eax
    float v65; // ecx
    Vec3 v67; // [esp+10h] [ebp-148h]
    float v68; // [esp+1Ch] [ebp-13Ch]
    float v69; // [esp+1Ch] [ebp-13Ch]
    float v70; // [esp+1Ch] [ebp-13Ch]
    float v71; // [esp+1Ch] [ebp-13Ch]
    float v72; // [esp+1Ch] [ebp-13Ch]
    char v73; // [esp+1Ch] [ebp-13Ch]
    float v74; // [esp+20h] [ebp-138h]
    float v75; // [esp+20h] [ebp-138h]
    float v76; // [esp+20h] [ebp-138h]
    float v77; // [esp+20h] [ebp-138h]
    float v78; // [esp+24h] [ebp-134h]
    float v79; // [esp+24h] [ebp-134h]
    float v80; // [esp+24h] [ebp-134h]
    float v81; // [esp+24h] [ebp-134h]
    float v82; // [esp+28h] [ebp-130h]
    float v83; // [esp+28h] [ebp-130h]
    float v84; // [esp+28h] [ebp-130h]
    float v85; // [esp+28h] [ebp-130h]
    float v86; // [esp+28h] [ebp-130h]
    float v87; // [esp+2Ch] [ebp-12Ch]
    float v88; // [esp+2Ch] [ebp-12Ch]
    float v89; // [esp+30h] [ebp-128h]
    float v90; // [esp+34h] [ebp-124h]
    float v91; // [esp+34h] [ebp-124h]
    float v92; // [esp+34h] [ebp-124h]
    float v93; // [esp+34h] [ebp-124h]
    float v94; // [esp+34h] [ebp-124h]
    float v95; // [esp+34h] [ebp-124h]
    float v96; // [esp+38h] [ebp-120h]
    float v97; // [esp+38h] [ebp-120h]
    float v98; // [esp+38h] [ebp-120h]
    float v99; // [esp+38h] [ebp-120h]
    float v100; // [esp+38h] [ebp-120h]
    Vec3 a3a; // [esp+3Ch] [ebp-11Ch] BYREF
    Vec3 v102; // [esp+48h] [ebp-110h] BYREF
    Vec3 v103; // [esp+54h] [ebp-104h] BYREF
    Vec3 v104; // [esp+60h] [ebp-F8h] BYREF
    Vec3 v105; // [esp+6Ch] [ebp-ECh] BYREF
    float v106; // [esp+78h] [ebp-E0h]
    float v107; // [esp+7Ch] [ebp-DCh]
    float v108; // [esp+80h] [ebp-D8h]
    float a4a; // [esp+84h] [ebp-D4h]
    float v110; // [esp+88h] [ebp-D0h]
    float v111; // [esp+8Ch] [ebp-CCh]
    float v112; // [esp+90h] [ebp-C8h]
    float v113; // [esp+94h] [ebp-C4h]
    Vec3 a2a[8]; // [esp+98h] [ebp-C0h] BYREF
    Vec3 a1[8]; // [esp+F8h] [ebp-60h] BYREF

    v10 = a4->z * 0.0;
    v11 = v10 - a4->y;
    v78 = a4->x - v10;
    v12 = a4->y * 0.0 - a4->x * 0.0;
    v82 = v12;
    v13 = 1.0 / sqrt(v12 * v82 + v78 * v78 + v11 * v11);
    v68 = v13;
    v14 = v13 * v11;
    v15 = v78 * v68;
    v83 = v82 * v68;
    v16 = a4->x;
    v17 = sqrt(v16 * v16 + a4->y * a4->y + a4->z * a4->z);
    v18 = 1.0 / v17 * v16;
    a4->x = v18;
    v87 = 1.0 / v17 * a4->y;
    v19 = v18;
    v20 = 1.0 / v17 * a4->z;
    a4->y = v87;
    v69 = v20;
    a4->z = v69;
    v67.x = v83 * v87 - v15 * v69;
    v67.y = v69 * v14 - v83 * v19;
    v67.z = v15 * v19 - v87 * v14;
    *a2 = v67;
    v106 = v19 * a7;
    v107 = v87 * a7;
    v108 = v69 * a7;
    v22 = v15;
    v74 = v14 * 1.05;
    v23 = v83 * 1.05;
    v67.x = v67.x * a8;
    v67.y = v67.y * a8;
    v67.z = v67.z * a8;
    v24 = v106 - v74;
    points[1].x = v24;
    v25 = v107 - v22 * 1.05;
    points[1].y = v25;
    v70 = v108 - v23;
    points[1].z = v70;
    points[2].x = v24;
    points[2].y = v25;
    points[2].z = v70;
    v75 = v74 * 1.04;
    v79 = v22 * 1.05 * 1.04;
    v84 = v23 * 1.04;
    v26 = v106 + v75;
    points->x = v26;
    v27 = v107 + v79;
    points->y = v27;
    v71 = v108 + v84;
    points->z = v71;
    points[3].z = v71;
    points[3].x = v26;
    points[3].y = v27;
    points->x = v67.x + points->x;
    points->y = v67.y + points->y;
    points->z = v67.z + points->z;
    points[1].x = v67.x + points[1].x;
    points[1].y = points[1].y + v67.y;
    points[1].z = points[1].z + v67.z;
    points[2].x = points[2].x - v67.x;
    points[2].y = points[2].y - v67.y;
    points[2].z = points[2].z - v67.z;
    points[3].x = points[3].x - v67.x;
    points[3].y = points[3].y - v67.y;
    points[3].z = points[3].z - v67.z;
    v28 = points->z * points[1].y - points[1].z * points->y;
    v29 = points[1].z * points->x - points[1].x * points->z;
    a3a.z = points[1].x * points->y - points->x * points[1].y;
    v102.x = points[1].z * points[2].y - points[1].y * points[2].z;
    v102.y = points[1].x * points[2].z - points[1].z * points[2].x;
    v102.z = points[1].y * points[2].x - points[1].x * points[2].y;
    v103.x = points[3].y * points[2].z - points[3].z * points[2].y;
    v103.y = points[3].z * points[2].x - points[2].z * points[3].x;
    v103.z = points[2].y * points[3].x - points[3].y * points[2].x;
    v104.x = points->y * points[3].z - points[3].y * points->z;
    v104.y = points->z * points[3].x - points[3].z * points->x;
    v104.z = points[3].y * points->x - points->y * points[3].x;
    v105.x = a4->x * -1.0;
    v105.y = a4->y * -1.0;
    v105.z = a4->z * -1.0;
    v30 = 1.0 / sqrt(v29 * v29 + v28 * v28 + a3a.z * a3a.z);
    v72 = v30;
    a3a.x = v30 * v28;
    a3a.y = v72 * v29;
    a3a.z = a3a.z * v72;
    v31 = 1.0 / sqrt(v102.z * v102.z + v102.y * v102.y + v102.x * v102.x);
    v102.x = v102.x * v31;
    v102.y = v102.y * v31;
    v102.z = v102.z * v31;
    v32 = 1.0 / sqrt(v103.z * v103.z + v103.y * v103.y + v103.x * v103.x);
    v103.x = v103.x * v32;
    v103.y = v103.y * v32;
    v103.z = v103.z * v32;
    v33 = 1.0 / sqrt(v104.z * v104.z + v104.y * v104.y + v104.x * v104.x);
    v104.x = v104.x * v33;
    v104.y = v104.y * v33;
    v104.z = v104.z * v33;
    v34 = 1.0 / sqrt(v105.z * v105.z + v105.y * v105.y + v105.x * v105.x);
    v105.x = v105.x * v34;
    v105.y = v105.y * v34;
    v105.z = v105.z * v34;
    v35 = a9 * 0.25;
    v36 = a10 * 0.25;
    v110 = sqrt(v102.y * v102.y + v102.x * v102.x) * v36
        + fabs(((v102.y + v102.x) * 0.0 + v102.z) * v35)
        + v102.z * a3->z
        + v102.y * a3->y
        + v102.x * a3->x;
    v112 = sqrt(v104.y * v104.y + v104.x * v104.x) * v36
        + fabs(((v104.y + v104.x) * 0.0 + v104.z) * v35)
        + v104.z * a3->z
        + v104.y * a3->y
        + v104.x * a3->x;
    v67.x = a4->x;
    v67.y = a4->y;
    a4a = sqrt(a3a.y * a3a.y + a3a.x * a3a.x) * v36
        + fabs(((a3a.y + a3a.x) * 0.0 + a3a.z) * v35)
        + a3a.z * a3->z
        + a3a.y * a3->y
        + a3a.x * a3->x;
    v111 = sqrt(v103.y * v103.y + v103.x * v103.x) * v36
        + fabs(v35 * ((v103.y + v103.x) * 0.0 + v103.z))
        + v103.z * a3->z
        + v103.y * a3->y
        + v103.x * a3->x;
    v113 = v105.z * a3->z + v105.x * a3->x + v105.y * a3->y + 2.0;
    v37 = 1.0 / sqrt(v67.x * v67.x + v67.y * v67.y);
    v67.x = v67.x * v37;
    v67.y = v67.y * v37;
    v38 = v37 * 0.0;
    v67.z = v38;
    v39 = v38 * 0.0;
    v106 = v67.y - v39;
    v40 = v39 - v67.x;
    v41 = v67.x * 0.0 - v67.y * 0.0;
    v42 = -a6;
    v90 = v67.y * v42;
    v96 = v42 * v67.z;
    v76 = v67.x * v42 + a3->x;
    v80 = v90 + a3->y;
    v85 = v96 + a5;
    v43 = a6 * 8.0;
    v91 = v40 * v43;
    v97 = v41 * v43;
    a1[2].x = v106 * v43 + v76;
    a1[2].y = v91 + v80;
    a1[2].z = v97 + v85;
    v44 = a6 * -8.0;
    v92 = v40 * v44;
    v98 = v41 * v44;
    a1[3].x = v106 * v44 + v76;
    a1[3].y = v92 + v80;
    a1[3].z = v98 + v85;
    v45 = a6 + a6;
    v93 = v67.y * v45;
    v99 = v45 * v67.z;
    v77 = v67.x * v45 + v76;
    v81 = v93 + v80;
    v86 = v99 + v85;
    v94 = v40 * v44;
    v100 = v41 * v44;
    a1[0].x = v106 * v44 + v77;
    a1[0].y = v94 + v81;
    a1[0].z = v100 + v86;
    v89 = v106 * v43;
    v95 = v40 * v43;
    a1[1].x = v89 + v77;
    a1[1].y = v95 + v81;
    a1[1].z = v41 * v43 + v86;
    v46 = WTR_Clip(a1, a2a, &a3a, a4a, 4);
    v47 = WTR_Clip(a2a, a1, &v102, v110, v46);
    v48 = WTR_Clip(a1, a2a, &v103, v111, v47);
    v49 = WTR_Clip(a2a, a1, &v104, v112, v48);
    v73 = WTR_Clip(a1, a2a, &v105, v113, v49);
    v50 = 0;
    v51 = a2a[0].x * v67.x + a2a[0].y * v67.y + a2a[0].z * v67.z;
    v52 = a2a[1].z * v67.z + a2a[1].x * v67.x + v67.y * a2a[1].y;
    v111 = a2a[2].z * v67.z + a2a[2].x * v67.x + v67.y * a2a[2].y;
    v112 = a2a[3].z * v67.z + a2a[3].x * v67.x + v67.y * a2a[3].y;
    v88 = (v111 + v51 + v112 + v52) * 0.25;
    if (v51 < v88 && v52 > v88)
        v50 = 1;
    if (v52 < v88 && v111 >(double)v88)
        v50 = 2;
    if (v111 < (double)v88 && v112 >(double)v88)
        v50 = 3;
    if (v112 < (double)v88 && v51 > v88)
        v50 = 0;
    v53 = &a2a[v50 & 3];
    points->x = v53->x;
    v54 = v53->y;
    v55 = v53->z;
    points->y = v54;
    points->z = v55;
    v56 = &a2a[(v50 + 1) & 3];
    points[1].x = v56->x;
    v57 = v56->y;
    v58 = v56->z;
    points[1].y = v57;
    points[1].z = v58;
    v59 = &a2a[(v50 - 2) & 3];
    points[2].x = v59->x;
    v60 = v59->y;
    v61 = v59->z;
    points[2].y = v60;
    v62 = &a2a[(v50 - 1) & 3];
    points[2].z = v61;
    v63 = v62->x;
    v64 = v62->y;
    v65 = v62->z;
    points[3].x = v63;
    points[3].y = v64;
    points[3].z = v65;
    return v73 & 4;
}

void WTR_UpdateColorMode(tWaterParamsGlobalStruct* wtr, float wtrHeight)
{
    int* currColor;
    int idx;

    if (wtrHeight <= wtr->waterHeight)
    {
        if (wtr->waterEngine.waterColorMode != 1)
        {
            memcpy(&wtr->waterEngine.colorTable[224], wtr->waterEngine.colorTableBackup, 256u);
            currColor = &wtr->waterEngine.colorTable[256];
            idx = 32;
            do
            {
                *currColor++ = wtr->waterEngine.bottomColor;
                --idx;
            } while (idx);
            wtr->waterEngine.waterColorMode = 1;
        }
    }
    else if (wtr->waterEngine.waterColorMode != 2)
    {
        currColor = &wtr->waterEngine.colorTable[224];
        memcpy(&wtr->waterEngine.colorTable[224], wtr->waterEngine.colorTableBackup, 256u);
        idx = 32;
        do
        {
            *currColor++ = wtr->waterEngine.skyColor;
            --idx;
        } while (idx);
        wtr->waterEngine.waterColorMode = 2;
    }
}

void WTR_Compute_Mesh0_Rasterize_Line(Vec3* a1, tWtrStruct2* a2, int a3, float a4, Vec3* a5, Vec3* a6, int a7, float a8, float a9, float a10)
{
    Vec3* v10; // eax
    int v11; // ebx
    double v12; // st7
    double v13; // st6
    double v14; // st5
    int v15; // edi
    double v16; // st6
    tWtrStruct2* v17; // esi
    double v18; // st4
    double v19; // st3
    double v20; // st2
    tWtrStruct2* v21; // esi
    double v22; // st5
    double v23; // st4
    double v24; // st3
    tWtrStruct2* v25; // esi
    double v26; // st5
    double v27; // rt1
    float v28; // [esp+10h] [ebp-78h]
    float v29; // [esp+14h] [ebp-74h]
    int v30; // [esp+18h] [ebp-70h]
    Vec3* v31; // [esp+1Ch] [ebp-6Ch]
    double v32; // [esp+20h] [ebp-68h]
    double v33; // [esp+28h] [ebp-60h]
    float v34; // [esp+34h] [ebp-54h]
    float v35; // [esp+34h] [ebp-54h]
    float v36; // [esp+34h] [ebp-54h]
    float v37; // [esp+38h] [ebp-50h]
    float v38; // [esp+38h] [ebp-50h]
    float v39; // [esp+38h] [ebp-50h]
    float v40; // [esp+38h] [ebp-50h]
    float v41; // [esp+3Ch] [ebp-4Ch]
    float v42; // [esp+3Ch] [ebp-4Ch]
    float v43; // [esp+3Ch] [ebp-4Ch]
    float v44; // [esp+3Ch] [ebp-4Ch]
    float v45; // [esp+44h] [ebp-44h]
    float v46; // [esp+48h] [ebp-40h]
    float v47; // [esp+4Ch] [ebp-3Ch]
    float v48; // [esp+50h] [ebp-38h]
    float v49; // [esp+54h] [ebp-34h]
    float v50; // [esp+58h] [ebp-30h]
    float v51; // [esp+5Ch] [ebp-2Ch]
    float v52; // [esp+60h] [ebp-28h]
    float v53; // [esp+68h] [ebp-20h]
    float v54; // [esp+6Ch] [ebp-1Ch]
    float v55; // [esp+78h] [ebp-10h]
    float v56; // [esp+84h] [ebp-4h]
    float a4a; // [esp+94h] [ebp+Ch]

    v10 = a1;
    v46 = a5->z;
    v45 = a5->y;
    v11 = 1 << a3;
    v31 = &a1[a7];
    v30 = 1 << a3;
    a4a = (double)(unsigned int)(1 << a3) * a4;
    if (a1 < v31)
    {
        v29 = 1.0 - a10;
        v12 = a5->x;
        do
        {
            v13 = (v12 + 40000.0) * a4a;
            v14 = (v45 + 40000.0) * a4a;
            v33 = v13 - 0.5 + MATH_gd_magicDoubleForRound;
            v32 = v14 - 0.5 + MATH_gd_magicDoubleForRound;
            v28 = v13 - (double)(*(int32_t *)&v33);
            v15 = ((*(uint32_t*)&v33) & (v11 - 1)) + ((*(uint32_t*)&v32) & (v11 - 1)) + (((*(uint32_t*)&v32) & (v11 - 1)) << a3);
            v16 = v14 - (double)(*(int32_t*)&v32);
            v17 = &a2[v15];
            v18 = 1.0 - v28;
            v19 = v18 * (1.0 - v16);
            v37 = v17->f2 * v19;
            v41 = v17->f3 * v19;
            v20 = (1.0 - v16) * v28;
            v47 = v20 * v17[1].f1;
            v48 = v17[1].f2 * v20;
            v49 = v17[1].f3 * v20;
            v34 = v47 + v19 * v17->f1;
            v11 = 1 << a3;
            v38 = v48 + v37;
            v21 = &a2[v15 + 1 + v30];
            v42 = v49 + v41;
            v22 = v18 * v16;
            v23 = v18 * v16 * v21->f1;
            v53 = v21->f2 * v22;
            v24 = v21->f3;
            v25 = &a2[v30 + 2 + v15];
            v54 = v24 * v22;
            v39 = v53 + v38;
            v43 = v54 + v42;
            v26 = v16 * v28;
            v50 = v26 * v25->f1;
            v51 = v25->f2 * v26;
            v52 = v25->f3 * v26;
            v44 = v52 + v43;
            v35 = (v50 + v23 + v34) * a8;
            ++v10;
            v55 = v46 * v29;
            v36 = v29 * v12 + v35;
            v40 = v29 * v45 + (v51 + v39) * a8;
            v56 = a10 * v10[-1].z;
            v27 = v10[-1].y * a10;
            v10[-1].x = a10 * v10[-1].x + v36;
            v10[-1].y = v27 + v40;
            v10[-1].z = v56 + v55 + v44 * a9;
            v12 = v12 + a6->x;
            v45 = v45 + a6->y;
            v46 = v46 + a6->z;
        } while (v10 < v31);
    }
}

void WTR_DirectComputeColors_CloudShading(Vec3* vertices, int* colorOut, int vertexnum, int color1, int color2, float distanceMuliplier, float zDistance)
{
    Vec3* currVertex;
    int* currColor;
    float factor;

    currVertex = vertices;
    if (vertices >= &vertices[vertexnum - 1])
    {
        *colorOut = *(colorOut - 1);
    }
    else
    {
        currColor = colorOut;
        do
        {
            factor = (currVertex->z - zDistance) * distanceMuliplier + 0.5f;
            *currColor = LIGHT_ul_Interpol2Colors(color1, color2, factor);
            ++currVertex;
            ++currColor;
        } while (currVertex < &vertices[vertexnum - 1]);
        *currColor = *(currColor - 1);
    }
}

void WTR_DirectComputeColors(Vec3* a1, Vec3* camerObjectPos, Vec3* vecsForUvs, Vec3* a4, int* colors, int num, int* a7, float a8)
{
    Vec3* v8;
    Vec3* v9;
    int* v10;
    Vec3* v12;
    double v13;
    double v14;
    double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    Vec3* v24;
    long double v25;
    float v26;
    Vec3* v27;
    float v28;
    float v29;
    Vec3* v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    Vec3 v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float a4a;
    float a5a;
    float numa;
    float numb;
    int tmp;

    v8 = a1;
    v9 = &a1[num];
    v27 = camerObjectPos;
    v30 = v9 - 1;
    if (a1 >= &v9[-1])
    {
        *colors = *(colors - 1);
    }
    else
    {
        v10 = colors;
        v12 = v9;
        while (1)
        {
            v34 = v8->x - camerObjectPos->x;
            v35 = v8->y - camerObjectPos->y;
            v36 = v8->z - camerObjectPos->z;
            v13 = v8[1].x - v8->x;
            v14 = v8[1].y - v8->y;
            v15 = v8[1].z - v8->z;
            v42 = v12->z - v8->z;
            v40 = (v12->x - v8->x) * a8;
            v16 = (v12->y - v8->y) * a8;
            v41 = (float)v16;
            v31 = (float)(v16 * v15 - v42 * v14);
            v32 = (float)(v42 * v13 - v15 * v40);
            v17 = v14 * v40 - v13 * v41;
            v33 = (float)v17;
            v18 = v17 * v33;
            v26 = v32 * v32;
            v19 = v31 * v31;
            a4a = (float)v19;
            v20 = v19 + v26 + v18;
            v21 = v36 * v33 + v35 * v32 + v34 * v31;
            if (v21 > 0.0)
                v21 = 0.0;
            numa = v36 * v36 + v35 * v35 + v34 * v34;
            v28 = (float)(numa * v20 * v20);
            tmp = MATH_gl_TableInverseRacine[((*((int32_t*)(&v28))) >> 13) & 0x7FF] + (((int)(0xBD800000 - ((*((uint32_t*)(&v28))) & 0x7F800000)) >> 1) & 0x7F800000);
            a5a = *(float*)&tmp;
            v22 = a5a * v21;
            numb = (float)v22;
            v23 = a5a * v20;
            v37 = (float)(v34 * v23);
            v39 = (float)(v35 * v23);
            v43 = (float)(v22 * -2.0 * v32);
            v44 = (float)(v22 * -2.0 * v33);
            v38.x = (float)(v31 * (v22 * -2.0) + v37);
            v38.y = v43 + v39;
            v38.z = (float)(v44 + v23 * v36);
            v29 = (float)v21;
            tmp = (*((int32_t*)(&v29))) & 0xBF800000 | 0x3F800000;
            *v10 = a7[((unsigned int)(256.0 - numb * numb * v20 * (*(float*)&tmp) * 255.0) & 0x1FF) + 1];
            *a4++ = v38;
            if (fabs(v31) != 0.0 || fabs(v32) != 0.0 || fabs(v33) != 0.0)
            {
                v24 = vecsForUvs;
                v25 = 1.0 / sqrt(v18 + a4a + v26);
                vecsForUvs->x = (float)(v31 * v25);
                vecsForUvs->y = (float)(v32 * v25);
                vecsForUvs->z = (float)(v25 * v33);
            }
            else
            {
                v24 = vecsForUvs;
            }
            vecsForUvs = v24 + 1;
            ++v8;
            ++v12;
            ++v10;
            if (v8 >= v30)
                break;
            camerObjectPos = v27;
        }
        *v10 = *(v10 - 1);
    }
}


int WTR_Compute_Mesh0(tWaterParamsGlobalStruct* a1, Vec3* cameraObjectPos, Vec3* a3, float a7, float a8)
{
    double tiling; // st7
    float waterheight; // edx
    double choppyness; // st7
    int result; // eax
    double v9; // st7
    double v10; // st6
    unsigned int v11; // ebp
    double v12; // st7
    double v13; // st6
    double v14; // st7
    unsigned int v15; // edi
    Vec3* vvecsforuvs; // ecx
    Vec3* v17; // edx
    Vec3* vertices; // esi
    Vec3* v19; // eax
    double v20; // st7
    double v21; // st6
    double v22; // st7
    unsigned int v23; // edx
    tWtrStruct2* v24; // ebp
    double j; // st7
    Vec3* k; // esi
    double v27; // st7
    unsigned int v28; // eax
    Vec3* v29; // edx
    unsigned int* v30; // ebp
    Vec3* v31; // ecx
    int v32; // esi
    Vec3* v33; // ecx
    unsigned int v34; // edx
    float distanceMultiplier; // [esp+0h] [ebp-104h]
    float a6a; // [esp+0h] [ebp-104h]
    float a9; // [esp+4h] [ebp-100h]
    float choppyneshsq; // [esp+8h] [ebp-FCh]
    float v39; // [esp+18h] [ebp-ECh]
    float v40; // [esp+18h] [ebp-ECh]
    Vec3* v41; // [esp+18h] [ebp-ECh]
    int* v42; // [esp+18h] [ebp-ECh]
    float v43; // [esp+1Ch] [ebp-E8h]
    float i; // [esp+1Ch] [ebp-E8h]
    int v45; // [esp+20h] [ebp-E4h]
    int v46; // [esp+20h] [ebp-E4h]
    float v47; // [esp+24h] [ebp-E0h]
    Vec3* vecsforuvs; // [esp+24h] [ebp-E0h]
    int v49; // [esp+28h] [ebp-DCh]
    int v50; // [esp+28h] [ebp-DCh]
    float v51; // [esp+2Ch] [ebp-D8h]
    Vec3 a1a; // [esp+30h] [ebp-D4h] BYREF
    float v53; // [esp+3Ch] [ebp-C8h]
    unsigned int* colors; // [esp+40h] [ebp-C4h]
    int v55; // [esp+44h] [ebp-C0h]
    Vec3* v56; // [esp+44h] [ebp-C0h] FORCED
    float v57; // [esp+48h] [ebp-BCh]
    tWtrStruct2* v58; // [esp+4Ch] [ebp-B8h]
    float v59; // [esp+50h] [ebp-B4h]
    Vec3 a6; // [esp+54h] [ebp-B0h] BYREF
    float v61; // [esp+60h] [ebp-A4h]
    Vec3 dest; // [esp+64h] [ebp-A0h] BYREF
    float v63; // [esp+70h] [ebp-94h]
    float v64; // [esp+74h] [ebp-90h]
    int v65; // [esp+78h] [ebp-8Ch]
    float v66; // [esp+7Ch] [ebp-88h]
    float v67; // [esp+80h] [ebp-84h]
    Vec3 a2a[4]; // [esp+84h] [ebp-80h] BYREF
    int v69[20]; // [esp+B4h] [ebp-50h] BYREF

    tiling = 1.0 / a1->tiling;
    waterheight = a1->waterHeight;
    v51 = tiling * a1->waveHeight;
    choppyness = tiling * a1->choppy;
    v61 = choppyness;
    choppyneshsq = choppyness + choppyness;
    a9 = v51 + v51;
    result = WTR_Compute4Points(a2a, &dest, cameraObjectPos, a3, waterheight, 18000.0, a7, a8, a9, choppyneshsq);
    if (result)
    {
        v9 = a2a[0].x - cameraObjectPos->x;
        v10 = a2a[0].y - cameraObjectPos->y;
        memset(v69, 0, sizeof(v69));
        v39 = v10 * a3->y + v9 * a3->x + (a2a[0].z - cameraObjectPos->z) * a3->z;
        a1a.x = a2a[3].x - cameraObjectPos->x;
        a1a.y = a2a[3].y - cameraObjectPos->y;
        v11 = a1->verticallVertexNum;
        a1a.z = a2a[3].z - cameraObjectPos->z;
        v12 = a1a.y * a3->y + a1a.x * a3->x + a1a.z * a3->z;
        v55 = 1 << a1->waterEngine.waveMapShift;
        v43 = 1.0 / (double)(v11 - 1);
        v53 = 1.0 / v39;
        v13 = (double)v55;
        v40 = 1.0 / (v13 * a1->tiling) * (v43 * v39);
        if (v12 < v40)
            v12 = v40;
        v14 = 1.0 / v12;
        v67 = (v14 - v53) * v43;
        v59 = v53 * 0.0 * v43;
        v64 = v14 * v43;
        MATH_BlendVector(&a1a, a2a, &a2a[1], 0.5);
        MATH_BlendVector(&dest, &a2a[3], &a2a[2], 0.5);
        v47 = v13;
        v63 = 1.0
            / a1->tiling
            / v47
            / sqrt(
                (dest.z - a1a.z) * (dest.z - a1a.z)
                + (dest.y - a1a.y) * (dest.y - a1a.y)
                + (dest.x - a1a.x) * (dest.x - a1a.x));
        v15 = a1->horizontalVertexNum;
        vvecsforuvs = a1->vecsForUVs;
        v17 = a1->verticesForDisplay;
        vertices = a1->vertices;
        colors = a1->colors;
        v57 = 1.0 - (double)v11 * v63;
        vecsforuvs = vvecsforuvs;
        v41 = v17;
        v19 = &vertices[v11 * v15];
        v56 = v19;
        v49 = 0;
        for (i = 1.0 / (double)(v15 - 1); vertices < v56; v41 += v15)
        {
            v20 = v59 / v53;
            if (v57 <= v20)
                v20 = v57;
            v21 = v20;
            if (v20 < 0.0)
                v21 = 0.0;
            if (v21 > 1.0)
                v21 = 1.0;
            a1a.x = a2a[3].x * v21 + (1.0 - v21) * a2a[0].x;
            a1a.y = a2a[0].y * (1.0 - v21) + a2a[3].y * v21;
            a1a.z = a2a[0].z * (1.0 - v21) + a2a[3].z * v21;
            *(float*)&v58 = v20;
            if (v20 >= 0.0)
                v22 = *(float*)&v58;
            else
                v22 = 0.0;
            if (v22 > 1.0)
                v22 = 1.0;
            v23 = a1->waterEngine.waveMapShift;
            v24 = a1->concatenatedMap;
            v58 = v24;
            dest.y = a2a[1].y * (1.0 - v22) + a2a[2].y * v22;
            dest.z = a2a[1].z * (1.0 - v22) + a2a[2].z * v22;
            v59 = v64 + v59;
            v53 = v67 + v53;
            v57 = v57 + v63;
            a6.z = dest.z - a1a.z;
            a6.x = (a2a[1].x * (1.0 - v22) + a2a[2].x * v22 - a1a.x) * i;
            a6.y = (dest.y - a1a.y) * i;
            a6.z = a6.z * i;
            v66 = a6.y * a6.y + a6.x * a6.x;
            v45 = MATH_gl_TableRacine[((*(int32_t *)&v66) >> 13) & 0x7FF]
                + (((((*(uint32_t*)&v66) & 0x7F800000) + 0x3F800000) >> 1) & 0x7F800000);
            v65 = 1 << v23;
            for (j = (double)(1 << v23) * *(float*)&v45 * a1->tiling; j > 2.0; v58 = v24)
            {
                if (v23 < 2)
                    break;
                j = j * 0.5;
                v24 = &v58[(2 << v23) + 1 + (1 << (2 * v23))];
                --v23;
            }
            v69[v23] = v49;
            if (v49)
            {
                WTR_Compute_Mesh0_Rasterize_Line(vertices, v24, v23, a1->tiling, &a1a, &a6, v15, v61, v51, 0.0);
                if (a1->waterEngine.useCloudShadingforColors)
                {
                    distanceMultiplier = 1.0 / v51;
                    WTR_DirectComputeColors_CloudShading(
                        &vertices[-(int)a1->horizontalVertexNum],
                        (int*)&colors[-(int)a1->horizontalVertexNum],
                        a1->horizontalVertexNum,
                        a1->waterEngine.skyColor,
                        a1->waterEngine.bottomColor,
                        distanceMultiplier,
                        a1->waterHeight);
                }
                else
                {
                    WTR_DirectComputeColors(
                        &vertices[-(int)a1->horizontalVertexNum],
                        cameraObjectPos,
                        &vecsforuvs[-(int)a1->horizontalVertexNum],
                        &v41[-(int)a1->horizontalVertexNum],
                        &colors[-(int)a1->horizontalVertexNum],
                        a1->horizontalVertexNum,
                        a1->waterEngine.colorTable,
                        1.0f);
                }
            }
            else
            {
                WTR_Compute_Mesh0_Rasterize_Line(vertices, v24, v23, a1->tiling, &a1a, &a6, v15, 0.0, 0.0, 0.0);
            }
            v15 = a1->horizontalVertexNum;
            ++v49;
            colors += v15;
            vertices += v15;
            v19 = v56;
            vecsforuvs += v15;
        }
        for (k = &vertices[-(int)a1->horizontalVertexNum]; k < v19; k[-1].z = v27 - v51)
        {
            v27 = k->z;
            ++k;
        }
        if (a1->waterEngine.useCloudShadingforColors)
        {
            a6a = 1.0 / v51;
            WTR_DirectComputeColors_CloudShading(
                a1->vertices,
                (int*)a1->colors,
                a1->horizontalVertexNum,
                a1->waterEngine.skyColor,
                a1->waterEngine.bottomColor,
                a6a,
                a1->waterHeight);
        }
        else
        {
            WTR_DirectComputeColors(
                a1->vertices,
                cameraObjectPos,
                a1->vecsForUVs,
                a1->verticesForDisplay,
                a1->colors,
                a1->horizontalVertexNum,
                a1->waterEngine.colorTable,
                1.0f);
        }
        v28 = a1->horizontalVertexNum;
        v29 = a1->vertices;
        v30 = a1->colors;
        v31 = &a1->vertices[v28 * (a1->verticallVertexNum - 1)];
        v42 = &v69[1];
        v32 = 0;
        v46 = (int)v31;
        v50 = 0;
        if (a1->vertices < v31)
        {
            do
            {
                if (*v42 == v32)
                {
                    if (v32)
                    {
                        memcpy(v30, &v30[-(int)v28], 4 * v28);
                        v31 = (Vec3*)v46;
                        v32 = v50;
                    }
                    ++v42;
                }
                v28 = a1->horizontalVertexNum;
                ++v32;
                v29 += v28;
                v50 = v32;
                v30 += v28;
            } while (v29 < v31);
        }
        v33 = &v31[a1->horizontalVertexNum];
        if (v29 < v33)
        {
            v34 = ((char*)v33 - (char*)v29 - 1) / 12u + 1;
            do
            {
                *v30 = v30[-(int)a1->horizontalVertexNum];
                ++v30;
                --v34;
            } while (v34);
        }
        result = 1;
    }
    return result;
}

int WTR_Mesh_Do(tWaterParamsGlobalStruct* a1)
{
    double v1; // st7
    double v2; // st6
    double v3; // st5
    long double v4; // st5
    long double v5; // st7
    double v6; // st6
    double v7; // st6
    Vec3* v8; // edi
    Vec3* v9; // ebx
    int result; // eax
    tFFTComplex* v11; // ecx
    tFFTComplex* v12; // edx
    float v13; // [esp+0h] [ebp-74h]
    float a5a; // [esp+28h] [ebp-4Ch]
    float a5b; // [esp+28h] [ebp-4Ch]
    float a5; // [esp+28h] [ebp-4Ch]
    float a5c; // [esp+28h] [ebp-4Ch]
    float v18; // [esp+2Ch] [ebp-48h]
    float v19; // [esp+30h] [ebp-44h]
    float v20; // [esp+30h] [ebp-44h]
    float v21; // [esp+34h] [ebp-40h]
    float v22; // [esp+34h] [ebp-40h]
    Vec3 a2; // [esp+38h] [ebp-3Ch] BYREF
    Vec3 points[4]; // [esp+44h] [ebp-30h] BYREF

    v1 = gpst_DX_stDD->ct.cam.objectMtx.v3.z * 0.0;
    v2 = v1 - gpst_DX_stDD->ct.cam.objectMtx.v3.y;
    v19 = gpst_DX_stDD->ct.cam.objectMtx.v3.x - v1;
    v3 = gpst_DX_stDD->ct.cam.objectMtx.v3.y * 0.0 - gpst_DX_stDD->ct.cam.objectMtx.v3.x * 0.0;
    v21 = v3;
    v4 = 1.0 / sqrt(v3 * v21 + v19 * v19 + v2 * v2);// crossproduct, normalizevector
    a5a = v4;
    v5 = v4 * v2;
    v6 = v19 * a5a;
    v22 = v21 * a5a;
    a5b = gpst_DX_stDD->ct.cam.objectMtx.v1.z * v22
        + gpst_DX_stDD->ct.cam.objectMtx.v1.y * v6
        + v5 * gpst_DX_stDD->ct.cam.objectMtx.v1.x;
    a2.x = v5 * a5b;
    v18 = a2.x - gpst_DX_stDD->ct.cam.objectMtx.v1.x;
    v20 = v6 * a5b - gpst_DX_stDD->ct.cam.objectMtx.v1.y;
    v7 = v22 * a5b - gpst_DX_stDD->ct.cam.objectMtx.v1.z;
    a5 = 1.0 / ((sqrt(v7 * v7 + v20 * v20 + v18 * v18) + 1.0) * tan(gpst_DX_stDD->ct.cam.focale * 0.5));
    WTR_UpdateColorMode(a1, gpst_DX_stDD->ct.cam.objectMtx.Pos.z);
    v8 = &gpst_DX_stDD->ct.cam.objectMtx.v3;
    v9 = &gpst_DX_stDD->ct.cam.objectMtx.Pos;
    result = WTR_Compute_Mesh0(a1, &gpst_DX_stDD->ct.cam.objectMtx.Pos, &gpst_DX_stDD->ct.cam.objectMtx.v3, a5, 1.0);
    if (result)
    {
        v13 = a1->waterHeight - 100.0;
        WTR_Compute4Points(points, &a2, v9, v8, v13, 18000.0, a5, 1.0, 0.0, 0.0);
        D3D_DrawWater_DrawVertexTable(a1);
        v11 = a1->spaceMapDY;
        v12 = a1->spaceMapDX;
        a5c = TIM_gf_dt * a1->speed + a1->waterEngine.timeElapsed;
        a1->waterEngine.timeElapsed = a5c;
        WATERFFT_Turn_Spectre(&a1->waterEngine, a1->spaceMap, v12, v11, a5c);
        WTR_Concate(a1);
        result = 1;
    }
    return result;
}

void PC_AE_ZListHook(int zListId)
{
    if (zListId == 0)
    {
        QueryPerformanceCounter(&PerfCounterStruct[43]);
        if (pst_GlobalsWaterParams)
            Workkkk_748 = WTR_Mesh_Do(pst_GlobalsWaterParams);
        PRO_QPC_RecordTimeDecRef(43);
    }
}

void WTR_Mesh_Destroy(tWaterParamsGlobalStruct* wtr)
{
    WTR_FreeSubStruct(wtr);
    MEM_FreeAlign(wtr);
}

void WTR_BeginLoadWorldHook()
{
    if (pst_GlobalsWaterParams)
        WTR_Mesh_Destroy(pst_GlobalsWaterParams);
    pst_GlobalsWaterParams = 0;
    WTR_SetDefaulExp(&stExportWaterParrams);
    if (pst_GlobalsWaterParams_Save)
        WTR_Mesh_Destroy(pst_GlobalsWaterParams_Save);
    pst_GlobalsWaterParams_Save = 0;
}


void WTR_WaterBlurNormal(Vec3* a1, int a2, int a3)
{
    Vec3* v3;
    Vec3* i;
    double v8;
    Vec3* v9;
    int v10;
    Vec3* j;
    double v12;

    v3 = &a1[a2 * (a3 - 1)];
    Vec3* _EAX = a1;
    if (a1 < v3)
    {
        //_ESI = 4 * (3 * a2 + 6);
        int _ECX = a2;
        do
        {
            for (i = &_EAX[_ECX - 1]; _EAX < i; _EAX[-1].z = (float)((_EAX[_ECX - 1].z + _EAX->z + _EAX[-1].z) * 0.33333299))
            {
                v8 = _EAX[_ECX].x + _EAX->x;
                ++_EAX;
                _EAX[-1].x = (float)((v8 + _EAX->x) * 0.33333299);
                _EAX[-1].y = (float)((_EAX[_ECX - 1].y + _EAX->y + _EAX[-1].y) * 0.33333299);
            }
            ++_EAX;
        } while (_EAX < v3);
    }
    v9 = a1;
    if (a1 < v3)
    {
        v10 = a2;
        do
        {
            for (j = &v9[v10 - 1]; v9 < j; v9[-1].z = (float)((v9[v10 - 1].z + v9->z + v9[-1].z) * 0.33333299))
            {
                v12 = v9[v10].x;
                ++v9;
                v9[-1].x = (float)((v12 + v9->x + v9[-1].x) * 0.33333299);
                v9[-1].y = (float)((v9[v10 - 1].y + v9->y + v9[-1].y) * 0.33333299);
            }
            ++v9;
        } while (v9 < v3);
    }
}
