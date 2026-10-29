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

/* Surface normal of the water under a world position.
 *
 * The surface is sampled three times: at the position itself and at two
 * neighbours a cell and a half away along x and along y. The two differences
 * from the centre span the surface there, so their cross product is the
 * normal, and it is returned normalised.
 *
 * Straight up is returned when there is no water, and also when cloud shading
 * is on, which lights the surface from the colour ramp instead of the normal. */
void WTR_Get_A_MrmVector(Vec3* position, Vec3* normalOut)
{
    tWaterParamsGlobalStruct* water = pst_GlobalsWaterParams;
    float sampleOffset;
    Vec3 centre;
    Vec3 alongX;
    Vec3 alongY;
    Vec3 toX;
    Vec3 toY;
    float inverseLength;

    normalOut->x = 0.0f;
    normalOut->y = 0.0f;
    normalOut->z = 1.0f;

    if (!water || water->waterEngine.useCloudShadingforColors)
        return;

    sampleOffset = 1.0f / (1 << water->waterEngine.borderSizePowerOfTwo) * (1.0f / water->tiling) * 1.5f;

    WTR_Get_A_DifVector(position, &centre, 1);

    alongX = *position;
    alongX.x += sampleOffset;
    WTR_Get_A_DifVector(&alongX, &alongX, 1);

    alongY = *position;
    alongY.y += sampleOffset;
    WTR_Get_A_DifVector(&alongY, &alongY, 1);

    toX.x = centre.x - alongX.x;
    toX.y = centre.y - alongX.y;
    toX.z = centre.z - alongX.z;

    toY.x = centre.x - alongY.x;
    toY.y = centre.y - alongY.y;
    toY.z = centre.z - alongY.z;

    normalOut->x = toX.y * toY.z - toX.z * toY.y;
    normalOut->y = toX.z * toY.x - toX.x * toY.z;
    normalOut->z = toX.x * toY.y - toX.y * toY.x;

    inverseLength = 1.0f / (float)sqrt(normalOut->z * normalOut->z
                                       + normalOut->y * normalOut->y
                                       + normalOut->x * normalOut->x);

    normalOut->x *= inverseLength;
    normalOut->y *= inverseLength;
    normalOut->z *= inverseLength;
}

/* Pack a colour held as three components in zero to one into one word, eight
 * bits each, laid out as blue in the high byte then green then red. Components
 * above one are clamped; the conversion to unsigned makes a negative component
 * wrap to a very large value, which the same clamp catches. */
int WTR_Vector_2_Color(Vec3* colour)
{
    unsigned int red = (unsigned int)(colour->x * 255.0);
    unsigned int green = (unsigned int)(colour->y * 255.0);
    unsigned int blue = (unsigned int)(colour->z * 255.0);

    if (red > 0xFF)
        red = 255;
    if (green > 0xFF)
        green = 255;
    if (blue > 0xFF)
        blue = 255;

    return red | (green << 8) | (blue << 16);
}


/* Reset the water to its built in settings.
 *
 * These are what a world gets before any of its own values arrive through
 * WTR_SetExportParams, and they have to be sane on their own: the engine reads
 * worldSizeInMeters and wind while building the spectrum and they are never
 * written from outside. */
void WTR_SetDefault(tWaterParamsGlobalStruct* water)
{
    Vec3 sky;
    Vec3 bottom;
    Vec3 surface;

    memset(water, 0, sizeof(tWaterParamsGlobalStruct));

    /* 64 cells a side for both the height grid and the choppy grid. */
    water->waterEngine.waveMapShift = 6;
    water->waterEngine.waveMapShiftCHP = 6;
    water->waterEngine.borderSizePowerOfTwo = 6;
    water->waterEngine.borderSizeDXDYPowerOfTwo = 6;

    water->waterEngine.wind.x = 60.0f;
    water->waterEngine.wind.y = 0.0f;
    water->waterEngine.direction = 1.0f;
    water->waterEngine.worldSizeInMeters = 10.0f;
    water->waterEngine.waveProp = 2.0f;

    water->waveHeight = 0.13f;
    water->choppy = 0.30000001f;
    water->disturbance = 0.30000001f;
    water->speed = 0.30000001f;
    water->tiling = 0.0099999998f;
    water->waterFog = 50.0f;

    /* A negative height keeps the surface out of sight until a world sets one. */
    water->waterHeight = -1.0f;

    water->horizontalVertexNum = 50;
    water->verticallVertexNum = 120;

    water->waterEngine.useCloudShadingforColors = 0;
    water->waterEngine.isSymmetryEnabled = 1;

    sky.x = 0.60000002f;
    sky.y = 0.60000002f;
    sky.z = 0.60000002f;

    bottom.x = 0.030000001f;
    bottom.y = 0.040000003f;
    bottom.z = 0.050000001f;

    surface.x = 0.30000001f;
    surface.y = 0.40000001f;
    surface.z = 0.5f;

    water->waterEngine.skyColor = WTR_Vector_2_Color(&sky);
    water->waterEngine.bottomColor = WTR_Vector_2_Color(&bottom);
    water->waterEngine.waterColor = WTR_Vector_2_Color(&surface);
}

/* Allocate aligned and clear it, since the water reads some of these buffers
 * before anything has written them. */
void* WTR_AllocAlign(int size, int alignment)
{
    void* block = MEM_p_AllocAligned(size, alignment);

    memset(block, 0, size);
    return block;
}

/* Allocate every buffer a water block needs for its current grid sizes.
 *
 * The three vertex arrays and the colour array carry one row more than the
 * grid, which is the spare row the mesh walk reads ahead into. The spectra are
 * square grids of their own side, and the concatenated map holds the base
 * level plus its reductions, so it is sized at one and a half times the base.
 *
 * Everything is 16 byte aligned apart from the concatenated map, which is
 * aligned to 32. */
void WTR_AllocSubStruct(tWaterParamsGlobalStruct* water)
{
    const unsigned int vertexCount = water->horizontalVertexNum * (water->verticallVertexNum + 1);
    const unsigned int heightCells = 1u << (2 * water->waterEngine.waveMapShift);
    const unsigned int choppyCells = 1u << (2 * water->waterEngine.waveMapShiftCHP);
    const unsigned int baseLevel = heightCells + (2u << water->waterEngine.waveMapShift) + 1;

    water->vertices = (Vec3*)WTR_AllocAlign(sizeof(Vec3) * vertexCount, 16);
    water->vecsForUVs = (Vec3*)WTR_AllocAlign(sizeof(Vec3) * vertexCount, 16);
    water->verticesForDisplay = (Vec3*)WTR_AllocAlign(sizeof(Vec3) * vertexCount, 16);
    water->colors = (unsigned int*)WTR_AllocAlign(sizeof(unsigned int) * vertexCount, 16);

    water->spaceMap = (tFFTComplex*)WTR_AllocAlign(sizeof(tFFTComplex) * heightCells, 16);
    water->spaceMapDX = (tFFTComplex*)WTR_AllocAlign(sizeof(tFFTComplex) * choppyCells, 16);
    water->spaceMapDY = (tFFTComplex*)WTR_AllocAlign(sizeof(tFFTComplex) * choppyCells, 16);

    water->concatenatedMap = (tWtrStruct2*)WTR_AllocAlign(sizeof(tWtrStruct2) * (baseLevel + (baseLevel >> 1)), 32);
}

/* Release every buffer hanging off a water block and clear the pointers, so
 * the block can be filled again or freed. */
void WTR_FreeSubStruct(tWaterParamsGlobalStruct* water)
{
    void* buffers[10];
    int i;

    buffers[0] = water->vertices;
    buffers[1] = water->vecsForUVs;
    buffers[2] = water->verticesForDisplay;
    buffers[3] = water->colors;
    buffers[4] = water->spaceMap;
    buffers[5] = water->spaceMapDX;
    buffers[6] = water->spaceMapDY;
    buffers[7] = water->concatenatedMap;
    buffers[8] = water->waterEngine.spectroMap;
    buffers[9] = water->waterEngine.DXDYMap;

    for (i = 0; i < 10; i++)
        if (buffers[i])
            MEM_FreeAlign(buffers[i]);

    water->vertices = NULL;
    water->vecsForUVs = NULL;
    water->verticesForDisplay = NULL;
    water->colors = NULL;
    water->spaceMap = NULL;
    water->spaceMapDX = NULL;
    water->spaceMapDY = NULL;
    water->concatenatedMap = NULL;
    water->waterEngine.spectroMap = NULL;
    water->waterEngine.DXDYMap = NULL;
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

/* Scale every real component so they sum to one. The imaginary components are
 * left alone; at this point they still hold the raw directional term and are
 * overwritten with the dispersion frequency later.
 *
 * Note that this divides by the sum of the whole grid, so a single cell that is
 * not finite makes every cell not finite. */
void WATERFFT_Normalize(tFFTComplex* spectrum, int count)
{
    double sum = 0.0;
    double scale;
    int i;

    for (i = 0; i < count; i++)
        sum += spectrum[i].real;

    scale = 1.0 / sum;

    for (i = 0; i < count; i++)
        spectrum[i].real = (float)(scale * spectrum[i].real);
}

/* Exchange rows `first` and `second` of the square complex grid, and then the
 * columns with the same two indices. The grid is (1 << sizeLog2) on a side and
 * stored row major, so this is one symmetric permutation step: applying it for
 * every index paired with its bit reversal transposes the grid into the order
 * the butterfly passes expect. */
void WATERFFT_SwapXY(tFFTComplex* grid, char sizeLog2, int first, int second)
{
    const int side = 1 << sizeLog2;
    tFFTComplex* firstRow = &grid[first * side];
    tFFTComplex* secondRow = &grid[second * side];
    tFFTComplex swap;
    int i;

    for (i = 0; i < side; i++)
    {
        swap = firstRow[i];
        firstRow[i] = secondRow[i];
        secondRow[i] = swap;
    }

    for (i = 0; i < side; i++)
    {
        swap = grid[i * side + first];
        grid[i * side + first] = grid[i * side + second];
        grid[i * side + second] = swap;
    }
}

/* Reorder the square complex grid into bit reversed index order, which is what
 * the in place FFT passes consume. Each index is paired with its reversal once,
 * guarded by `index < reversed` so no pair is exchanged twice, and the pairing
 * is applied to rows and columns together by WATERFFT_SwapXY. */
void WATERFFT_Pretranspose_Table(tFFTComplex* grid, int sizeLog2)
{
    const int side = 1 << sizeLog2;
    int reversed = 0;
    int index;

    for (index = 0; index < side - 1; index++)
    {
        if (index < reversed)
            WATERFFT_SwapXY(grid, (char)sizeLog2, index, reversed);

        /* Increment `reversed` as if its bits ran the other way: carry down from
         * the high bit instead of up from the low one. */
        {
            int bit = side >> 1;
            while (bit <= reversed)
            {
                reversed -= bit;
                bit >>= 1;
            }
            reversed += bit;
        }
    }
}

/* Put the spectrum into the order the transform reads it and report how many
 * entries that is, which is every cell of the square grid. */
int WATERFFT_Compress_Spectromap(tFFTComplex* spectrum, int sizeLog2)
{
    WATERFFT_Pretranspose_Table(spectrum, sizeLog2);
    return 1 << (2 * sizeLog2);
}

/* Exchange two entries of an int array. */
void WATERFFT_SWAP(int* values, int first, int second)
{
    const int swap = values[first];

    values[first] = values[second];
    values[second] = swap;
}

/* Work out where the displacement grid lands inside the height grid.
 *
 * When the two grids are the same size the displacement spectrum lines up with
 * the height spectrum and nothing is needed, so the three shifts stay at their
 * neutral 0, 1, 1. When the displacement grid is the smaller of the two its
 * cells have to be scattered into the larger one, and because both grids are
 * read in bit reversed order that scatter is not a simple stride.
 *
 * So build the two orderings explicitly - the centred position each height
 * cell maps to, and the identity for the displacement cells - reverse each by
 * its own size, then look up where the first three displacement cells ended up.
 * The three results are stored as a start and two gaps.
 *
 * Note that both tables are filled one short of the grid side, and the search
 * respects that, as the last position is never needed. */
void WATERFFT_Compute_Supershift(tWaterEngineStruct* engine)
{
    const int heightSizeLog2 = engine->borderSizePowerOfTwo;
    const int displacementSizeLog2 = engine->borderSizeDXDYPowerOfTwo;
    const int heightSide = 1 << heightSizeLog2;
    const int displacementSide = 1 << displacementSizeLog2;
    int heightPosition[256];
    int displacementPosition[256];
    int reversed, index, bit;
    int firstCell, secondCell, thirdCell;

    engine->superShift0 = 0;
    engine->superShift1 = 1;
    engine->superShift2 = 1;

    if (displacementSizeLog2 >= heightSizeLog2)
        return;

    for (index = 0; index < heightSide - 1; index++)
    {
        heightPosition[index] = (displacementSide >> 1) - (heightSide >> 1) + index;
        displacementPosition[index] = index;
    }

    reversed = 0;
    for (index = 0; index < heightSide - 1; index++)
    {
        if (reversed > index)
            WATERFFT_SWAP(heightPosition, index, reversed);

        bit = heightSide >> 1;
        while (bit <= reversed)
        {
            reversed -= bit;
            bit >>= 1;
        }
        reversed += bit;
    }

    reversed = 0;
    for (index = 0; index < displacementSide - 1; index++)
    {
        if (index < reversed)
            WATERFFT_SWAP(displacementPosition, index, reversed);

        bit = displacementSide >> 1;
        while (bit <= reversed)
        {
            reversed -= bit;
            bit >>= 1;
        }
        reversed += bit;
    }

    firstCell = displacementPosition[0];
    secondCell = displacementPosition[1];
    thirdCell = displacementPosition[2];

    for (index = 0; index < heightSide - 1; index++)
    {
        if (heightPosition[index] == firstCell)
            engine->superShift0 = index;
        if (heightPosition[index] == secondCell)
            engine->superShift1 = index;
        if (heightPosition[index] == thirdCell)
            engine->superShift2 = index;
    }

    /* Keep the first as a start and turn the other two into gaps. */
    engine->superShift2 -= engine->superShift1;
    engine->superShift1 -= engine->superShift0;
}

/* Fill the direction map with the unit wavevector of every cell.
 *
 * WATERFFT_Deduct_DXDY_Spectre_Line multiplies the height spectrum by these to
 * get the two horizontal displacement spectra, so each entry holds the x part
 * of the unit vector in `real` and the y part in `imaginary`. The grid is
 * centred, so the coordinates run from -side/2 up to side/2 - 1 and the centre
 * cell has no direction at all; it is stored as zero rather than divided by a
 * zero length.
 *
 * The map is then put into bit reversed order to match the spectrum it will be
 * multiplied against. */
void WATERFFT_Precalc_DXDY_Repartition(tWaterEngineStruct* engine)
{
    const int sizeLog2 = engine->borderSizeDXDYPowerOfTwo;
    const int side = 1 << sizeLog2;
    const int half = side >> 1;
    int row, column;

    if (!sizeLog2)
        return;

    if (!engine->DXDYMap)
        engine->DXDYMap = (tFFTComplex*)WATERFFT_Alloc(sizeof(tFFTComplex) << (2 * sizeLog2));

    for (row = 0; row < side; row++)
    {
        const double y = (double)(row - half);

        for (column = 0; column < side; column++)
        {
            const double x = (double)(column - half);
            const double lengthSquared = x * x + y * y;
            tFFTComplex* entry = &engine->DXDYMap[column + (row << sizeLog2)];

            if (lengthSquared == 0.0)
            {
                entry->real = 0.0f;
                entry->imaginary = 0.0f;
            }
            else
            {
                const double inverseLength = 1.0 / sqrt(lengthSquared);

                entry->real = (float)(inverseLength * x);
                entry->imaginary = (float)(inverseLength * y);
            }
        }
    }

    WATERFFT_Pretranspose_Table(engine->DXDYMap, sizeLog2);
    WATERFFT_Compute_Supershift(engine);
}

/* Fill the butterfly twiddle table for an FFT of `stageCount` stages.
 *
 * Stage n needs 2^n rotations of the unit circle, so the table holds
 * 1 + 2 + 4 + ... = 2^stageCount - 1 entries laid out stage by stage. Each
 * entry keeps the rotation twice over, once as (cos, sin) and once turned a
 * quarter turn as (-sin, cos), which is what the butterfly multiplies by.
 *
 * Rather than evaluate a trigonometric function per entry, each stage starts at
 * one and steps round by repeatedly multiplying by that stage's fixed rotation,
 * whose cosine and sine sit as a pair in FFT_FACTORS_C. */
void WATERFFT_FFT_COMPUTE_COST(int stageCount, tFFTComplexCost* table)
{
    const float* factor = FFT_FACTORS_C;
    int entriesThisStage = 1;
    int stage;

    for (stage = 0; stage < stageCount; stage++)
    {
        const float stepCos = factor[0];
        const float stepSin = factor[1];
        /* The running cosine is kept at single precision and the sine at double,
         * as the rotation was originally carried, so the table rounds the same. */
        float angleCos = 1.0f;
        double angleSin = 0.0;
        int i;

        for (i = 0; i < entriesThisStage; i++)
        {
            double turnedCos;

            table->real1 = angleCos;
            table->imaginary1 = (float)angleSin;
            table->real2 = (float)-angleSin;
            table->imaginary2 = angleCos;
            table++;

            turnedCos = stepCos * angleCos - stepSin * angleSin;
            angleSin = angleSin * stepCos + stepSin * angleCos;
            angleCos = (float)turnedCos;
        }

        entriesThisStage *= 2;
        factor += 2;
    }
}

/* Build the wave spectrum for the current wind and sea settings.
 *
 * Every cell of the square grid stands for one wavevector k, measured in
 * radians per metre and centred so that k runs from -side/2 to side/2 - 1
 * steps of 2*pi over the world size. For each one:
 *
 *   real       the wave amplitude, from the Phillips distribution, which
 *              favours long waves travelling with the wind
 *   imaginary  the frequency at which that wave advances, from the deep water
 *              dispersion relation sqrt(g * |k|), jittered by a little over one
 *              percent so neighbouring waves drift apart instead of beating
 *
 * The amplitudes are normalised to sum to one, so the overall wave height is
 * set by the caller rather than by the grid size. Finally the cosine table and
 * the two butterfly tables are built, and the spectrum is put into the order
 * the transform reads it. */
void WATERFFT_Init_Spectre(tWaterEngineStruct* engine)
{
    const int sizeLog2 = engine->borderSizePowerOfTwo;
    const int side = 1 << sizeLog2;
    const int cellCount = 1 << (2 * sizeLog2);
    const double half = (double)side * 0.5;
    const double radiansPerCell = 6.2831855 / engine->worldSizeInMeters;
    int row, column;

    if (!engine->spectroMap)
        engine->spectroMap = (tFFTComplex*)WATERFFT_Alloc(sizeof(tFFTComplex) << (2 * sizeLog2));

    /* Amplitudes. The directional term is the same for both components, so the
     * distribution is evaluated once and the stored amplitude is the length of
     * the pair it forms with itself. */
    for (row = 0; row < side; row++)
    {
        tFFTComplex* cell = &engine->spectroMap[row << sizeLog2];

        for (column = 0; column < side; column++, cell++)
        {
            Vec2 waveVector;
            float amplitude;

            waveVector.x = (float)(((float)row - half) * radiansPerCell);
            waveVector.y = (float)(((double)column - half) * radiansPerCell);

            amplitude = WATERFFT_philippe(&waveVector, &engine->wind, engine->waveProp, engine->direction);

            cell->imaginary = amplitude;
            cell->real = (float)sqrt((double)amplitude * amplitude + (double)amplitude * amplitude);
        }
    }

    /* On grids finer than 64 cells a side, silence a wedge of the highest
     * frequencies around the centre column: they carry almost no energy and
     * would otherwise show up as shimmer. */
    if (sizeLog2 > 6)
    {
        const int extra = sizeLog2 - 6;
        const int centre = side >> 1;
        int offset;

        for (offset = 0; offset < extra; offset++)
        {
            int above = centre;
            int below = centre;
            int step;

            for (step = 0; step < extra; step++, above++, below--)
            {
                engine->spectroMap[centre + offset + (above << sizeLog2)].real = 0.0f;
                engine->spectroMap[centre - offset + (above << sizeLog2)].real = 0.0f;
                engine->spectroMap[centre + offset + (below << sizeLog2)].real = 0.0f;
                engine->spectroMap[centre - offset + (below << sizeLog2)].real = 0.0f;
            }
        }
    }

    WATERFFT_Normalize(engine->spectroMap, cellCount);

    /* Frequencies. Deep water dispersion, with a jitter of about one and a
     * quarter percent either way so the waves do not stay in step. */
    for (row = 0; row < side; row++)
    {
        for (column = 0; column < side; column++)
        {
            const double x = ((double)row - half) * radiansPerCell;
            const double y = ((double)column - half) * radiansPerCell;
            const double frequency = sqrt(sqrt(x * x + y * y) * 9.8100004);
            const int noise = RND_JADErand();

            engine->spectroMap[column + (row << sizeLog2)].imaginary =
                (float)(((double)noise * 0.000030518509 * (frequency + frequency) - frequency) * 0.0125 + frequency);
        }
    }

    /* One full turn of cosine and sine pairs, which WATERFFT_Turn_Spectre
     * indexes instead of calling a trigonometric function per cell. */
    for (row = 0; row < 1024; row++)
    {
        const double angle = (double)row * 0.0061359233;

        COS_TABLE[2 * row] = (float)cos(angle);
        COS_TABLE[2 * row + 1] = (float)sin(angle);
    }

    engine->spectroMapCompressedSize = WATERFFT_Compress_Spectromap(engine->spectroMap, sizeLog2);
    WATERFFT_Precalc_DXDY_Repartition(engine);
    WATERFFT_FFT_COMPUTE_COST(engine->borderSizePowerOfTwo, engine->FFTCostTable);
    WATERFFT_FFT_COMPUTE_COST(engine->borderSizeDXDYPowerOfTwo, engine->FFTCostTableDXDY);
}

/* Fill an export block with the built in settings, so a caller has somewhere
 * sensible to start before overriding the values it cares about.
 *
 * The defaults live on a full water block, so one is built on the stack and the
 * fields the export block carries are copied across. The water starts disabled,
 * and the leading marker is what identifies a block of this shape. */
void WTR_SetDefaulExp(struct sUserWaterParamsStruct* exported)
{
    tWaterParamsGlobalStruct defaults;

    WTR_SetDefault(&defaults);

    exported->abadcafe = 0xABADCAFE;
    exported->waterEnabled = 0;

    exported->frustrumMeshSX = defaults.horizontalVertexNum;
    exported->frustrumMeshSY = defaults.verticallVertexNum;
    exported->waveMapShift = defaults.waterEngine.waveMapShift;
    exported->waveMapShiftCHP = defaults.waterEngine.waveMapShiftCHP;

    exported->skyColor = defaults.waterEngine.skyColor;
    exported->bottomColor = defaults.waterEngine.bottomColor;
    exported->waterColor = defaults.waterEngine.waterColor;

    exported->waveHeight = defaults.waveHeight;
    exported->choppy = defaults.choppy;
    exported->tiling = defaults.tiling;
    exported->speed = defaults.speed;
    exported->waterHeight = defaults.waterHeight;
    exported->direction = defaults.waterEngine.direction;
    exported->waveProp = defaults.waterEngine.waveProp;

    exported->cludShadingIsEnabled = defaults.waterEngine.useCloudShadingforColors;
    exported->textureRefID = defaults.waterEngine.textureRefId;
    exported->isSymmetryActive = defaults.waterEngine.isSymmetryEnabled;
}

/* Build one line of the two horizontal displacement spectra from one line of
 * the height spectrum.
 *
 * The horizontal displacement of a wave is its height spectrum turned a quarter
 * turn and scaled by the unit wavevector, so each output is the input component
 * multiplied by i times the direction entry: (-dir.imaginary, dir.real). The
 * real part of the height spectrum feeds the first output and the imaginary
 * part feeds the second.
 *
 * Two entries are produced per step, taking their directions from two rows of
 * the direction map that sit `rowLength` apart, and both direction cursors
 * advance by `rowLength + rowStride` entries each step. */
void WATERFFT_Deduct_DXDY_Spectre_Line(tFFTComplex* outFromReal, tFFTComplex* outFromImaginary, tFFTComplex* outEnd,
                                       tFFTComplex* spectrum, tFFTComplex* direction, int rowLength, int rowStride)
{
    const int directionStep = rowLength + rowStride;
    tFFTComplex* lowDirection = direction;
    tFFTComplex* highDirection = &direction[rowLength];

    while (outFromReal < outEnd)
    {
        outFromReal[0].real = -(spectrum[0].real * lowDirection->imaginary);
        outFromReal[0].imaginary = spectrum[0].real * lowDirection->real;
        outFromReal[1].real = -(spectrum[1].real * highDirection->imaginary);
        outFromReal[1].imaginary = spectrum[1].real * highDirection->real;

        outFromImaginary[0].real = -(spectrum[0].imaginary * lowDirection->imaginary);
        outFromImaginary[0].imaginary = spectrum[0].imaginary * lowDirection->real;
        outFromImaginary[1].real = -(spectrum[1].imaginary * highDirection->imaginary);
        outFromImaginary[1].imaginary = spectrum[1].imaginary * highDirection->real;

        outFromReal += 2;
        outFromImaginary += 2;
        spectrum += 2;
        lowDirection += directionStep;
        highDirection += directionStep;
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

void WATERFFT_FFT2D(tFFTComplex* a1, int num, tFFTComplexCost* a3)
{
    int v4; // ebx
    unsigned int v5; // edi
    unsigned int v6; // eax
    tFFTComplex* v7; // ebp
    unsigned int v8; // esi
    int v9; // ebx
    double v10; // st7
    double v11; // st6
    tFFTComplex* v12; // eax
    double v13; // st7
    double v14; // st6
    double v15; // st5
    double v16; // st6
    double v17; // st5
    double v18; // st7
    double v19; // st5
    double v20; // st7
    tFFTComplex* v21; // eax
    double v22; // st7
    double v23; // st6
    double v24; // st7
    double v25; // st6
    double v26; // st5
    double v27; // st6
    double v28; // st5
    double v29; // st7
    double v30; // st5
    double v31; // st7
    tFFTComplex* v32; // eax
    double v33; // st7
    double v34; // st6
    double v35; // st7
    double v36; // st6
    double v37; // st5
    double v38; // st6
    double v39; // st5
    double v40; // st7
    double v41; // st5
    double v42; // st7
    tFFTComplex* v43; // eax
    double v44; // st7
    double v45; // st6
    double v46; // st7
    double v47; // st6
    double v48; // st5
    double v49; // st6
    double v50; // st5
    double v51; // st7
    double v52; // st5
    double v53; // st7
    int v54; // ebx
    double v55; // st7
    double v56; // st6
    tFFTComplex* v57; // eax
    double v58; // st7
    double v59; // st6
    double v60; // st5
    double v61; // st6
    double v62; // st5
    double v63; // st7
    double v64; // st5
    double v65; // st7
    int v66; // esi
    unsigned int v67; // ebp
    int v68; // eax
    int v69; // ebx
    tFFTComplexCost* v70; // edi
    int v71; // edx
    int v72; // ebp
    tFFTComplex* v73; // eax
    tFFTComplex* v74; // edi
    float* v75; // ecx
    int v76; // ebp
    double v77; // st7
    float* v78; // ebp
    double v79; // st6
    tFFTComplex* v80; // edx
    tFFTComplex* v81; // esi
    double v82; // st7
    double v83; // st6
    double v84; // st5
    double v85; // st6
    double v86; // st5
    double v87; // st7
    double v88; // st5
    double v89; // st7
    double v90; // st7
    double v91; // st6
    double v92; // st7
    double v93; // st6
    double v94; // st5
    double v95; // st6
    double v96; // st5
    double v97; // st7
    double v98; // st5
    double v99; // st7
    tFFTComplex* v100; // esi
    tFFTComplex* v101; // edx
    double v102; // st7
    double v103; // st6
    double v104; // st7
    double v105; // st6
    double v106; // st5
    double v107; // st6
    double v108; // st5
    double v109; // st7
    double v110; // st5
    double v111; // st7
    tFFTComplex* v112; // edi
    tFFTComplex* v113; // eax
    double v114; // st7
    double v115; // st6
    double v116; // st7
    double v117; // st6
    double v118; // st5
    double v119; // st6
    double v120; // st5
    double v121; // st7
    double v122; // st5
    double v123; // st4
    BOOL v124; // cf
    tFFTComplexCost* v125; // ebx
    int v127; // [esp+10h] [ebp-60h]
    float v128; // [esp+14h] [ebp-5Ch]
    float v129; // [esp+14h] [ebp-5Ch]
    float v130; // [esp+14h] [ebp-5Ch]
    float v131; // [esp+14h] [ebp-5Ch]
    float v132; // [esp+14h] [ebp-5Ch]
    float v133; // [esp+18h] [ebp-58h]
    float v134; // [esp+18h] [ebp-58h]
    float v135; // [esp+18h] [ebp-58h]
    float v136; // [esp+18h] [ebp-58h]
    float v137; // [esp+18h] [ebp-58h]
    float* v138; // [esp+18h] [ebp-58h]
    float v139; // [esp+1Ch] [ebp-54h]
    float v140; // [esp+1Ch] [ebp-54h]
    float v141; // [esp+1Ch] [ebp-54h]
    float v142; // [esp+1Ch] [ebp-54h]
    float v143; // [esp+1Ch] [ebp-54h]
    float v144; // [esp+1Ch] [ebp-54h]
    float v145; // [esp+1Ch] [ebp-54h]
    float v146; // [esp+1Ch] [ebp-54h]
    float v147; // [esp+1Ch] [ebp-54h]
    float v148; // [esp+20h] [ebp-50h]
    float v149; // [esp+20h] [ebp-50h]
    float v150; // [esp+20h] [ebp-50h]
    float v151; // [esp+20h] [ebp-50h]
    float v152; // [esp+20h] [ebp-50h]
    float v153; // [esp+20h] [ebp-50h]
    float v154; // [esp+20h] [ebp-50h]
    float v155; // [esp+20h] [ebp-50h]
    float v156; // [esp+20h] [ebp-50h]
    float v157; // [esp+20h] [ebp-50h]
    float v158; // [esp+20h] [ebp-50h]
    float v159; // [esp+20h] [ebp-50h]
    float v160; // [esp+20h] [ebp-50h]
    float v161; // [esp+20h] [ebp-50h]
    float v162; // [esp+24h] [ebp-4Ch]
    float v163; // [esp+24h] [ebp-4Ch]
    float v164; // [esp+24h] [ebp-4Ch]
    float v165; // [esp+24h] [ebp-4Ch]
    float v166; // [esp+24h] [ebp-4Ch]
    float v167; // [esp+24h] [ebp-4Ch]
    float* v168; // [esp+28h] [ebp-48h]
    float* v169; // [esp+28h] [ebp-48h]
    float* v170; // [esp+28h] [ebp-48h]
    float* v171; // [esp+28h] [ebp-48h]
    float v172; // [esp+2Ch] [ebp-44h]
    float v173; // [esp+34h] [ebp-3Ch]
    float v174; // [esp+3Ch] [ebp-34h]
    int v175; // [esp+40h] [ebp-30h]
    unsigned int v176; // [esp+44h] [ebp-2Ch]
    float* v177; // [esp+44h] [ebp-2Ch]
    int v178; // [esp+48h] [ebp-28h]
    float v179; // [esp+4Ch] [ebp-24h]
    float v180; // [esp+4Ch] [ebp-24h]
    float v181; // [esp+4Ch] [ebp-24h]
    float v182; // [esp+4Ch] [ebp-24h]
    float v183; // [esp+4Ch] [ebp-24h]
    float v184; // [esp+50h] [ebp-20h]
    float v185; // [esp+50h] [ebp-20h]
    float v186; // [esp+50h] [ebp-20h]
    float v187; // [esp+50h] [ebp-20h]
    float v188; // [esp+50h] [ebp-20h]
    float v189; // [esp+50h] [ebp-20h]
    float v190; // [esp+50h] [ebp-20h]
    int v191; // [esp+54h] [ebp-1Ch]
    unsigned int v192; // [esp+58h] [ebp-18h]
    int numsubst1; // [esp+5Ch] [ebp-14h]
    tFFTComplex* v194; // [esp+60h] [ebp-10h]
    int v195; // [esp+64h] [ebp-Ch]
    tFFTComplexCost* v196; // [esp+7Ch] [ebp+Ch]

    v4 = 1 << num;
    v5 = 1 << num >> 1;                           // =32
    v6 = 0;
    v127 = 1 << num;
    v176 = 0;
    if (v5)
    {
        do
        {
            v7 = &a1[(2 * v6) << num];
            v8 = 0;
            if ((int)v5 >= 4)
            {
                v9 = 2;
                do
                {
                    v10 = a3->real1;
                    v11 = a3->imaginary1;
                    v12 = &v7[2 * v8];
                    v128 = a3->imaginary1;
                    v133 = v10 * v12[1].real - v11 * v12[1].imaginary;
                    v139 = v11 * v12[1].real + v10 * v12[1].imaginary;
                    v148 = v10 * v12[v127 + 1].real - v11 * v12[v127 + 1].imaginary;
                    v13 = v10 * v12[v127 + 1].imaginary + v11 * v12[v127 + 1].real;
                    v14 = v12[v127].real - v148;
                    v168 = &v12[v127].imaginary;
                    v15 = *v168 - v13;
                    v162 = v14 * a3->real1 - v15 * v128;
                    v16 = v15 * a3->real1 + v14 * v128;
                    v17 = v13;
                    v18 = v148 + v12[v127].real;
                    v19 = v17 + *v168;
                    v149 = v18 * a3->real1 - v19 * v128;
                    v20 = v19 * a3->real1 + v18 * v128;
                    v12[v127 + 1].real = v149 + v12->real + v133;
                    v12[v127 + 1].imaginary = v139 + v20 + v12->imaginary;
                    v12[v127].real = v12->real - v133 + v162;
                    *v168 = v12->imaginary - v139 + v16;
                    v12[1].real = v133 + v12->real - v149;
                    v12[1].imaginary = v139 + v12->imaginary - v20;
                    v12->real = v12->real - v133 - v162;
                    v12->imaginary = v12->imaginary - v139 - v16;
                    v21 = &v7[v9 + v8];
                    v22 = a3->real1;
                    v23 = a3->imaginary1;
                    v129 = a3->imaginary1;
                    v134 = v22 * v7[v9 + 1 + v8].real - v23 * v7[v9 + 1 + v8].imaginary;
                    v140 = v23 * v7[v9 + 1 + v8].real + v22 * v7[v9 + 1 + v8].imaginary;
                    v150 = v22 * v21[v127 + 1].real - v23 * v21[v127 + 1].imaginary;
                    v24 = v22 * v21[v127 + 1].imaginary + v23 * v21[v127 + 1].real;
                    v25 = v21[v127].real - v150;
                    v169 = &v21[v127].imaginary;
                    v26 = *v169 - v24;
                    v163 = v25 * a3->real1 - v26 * v129;
                    v27 = v26 * a3->real1 + v25 * v129;
                    v28 = v24;
                    v29 = v150 + v21[v127].real;
                    v30 = v28 + *v169;
                    v151 = v29 * a3->real1 - v30 * v129;
                    v31 = v30 * a3->real1 + v29 * v129;
                    v21[v127 + 1].real = v151 + v21->real + v134;
                    v21[v127 + 1].imaginary = v140 + v31 + v7[v9 + v8].imaginary;
                    v21[v127].real = v21->real - v134 + v163;
                    *v169 = v7[v9 + v8].imaginary - v140 + v27;
                    v21[1].real = v134 + v21->real - v151;
                    v21[1].imaginary = v140 + v7[v9 + v8].imaginary - v31;
                    v21->real = v21->real - v134 - v163;
                    v21->imaginary = v7[v9 + v8].imaginary - v140 - v27;
                    v32 = &v7[v9 + 2 + v8];
                    v33 = a3->real1;
                    v34 = a3->imaginary1;
                    v130 = a3->imaginary1;
                    v135 = v33 * v7[v9 + 3 + v8].real - v34 * v7[v9 + 3 + v8].imaginary;
                    v141 = v34 * v7[v9 + 3 + v8].real + v33 * v7[v9 + 3 + v8].imaginary;
                    v152 = v33 * v32[v127 + 1].real - v34 * v32[v127 + 1].imaginary;
                    v35 = v33 * v32[v127 + 1].imaginary + v34 * v32[v127 + 1].real;
                    v36 = v32[v127].real - v152;
                    v170 = &v32[v127].imaginary;
                    v37 = *v170 - v35;
                    v164 = v36 * a3->real1 - v37 * v130;
                    v38 = v37 * a3->real1 + v36 * v130;
                    v39 = v35;
                    v40 = v152 + v32[v127].real;
                    v41 = v39 + *v170;
                    v153 = v40 * a3->real1 - v41 * v130;
                    v42 = v41 * a3->real1 + v40 * v130;
                    v32[v127 + 1].real = v153 + v32->real + v135;
                    v32[v127 + 1].imaginary = v141 + v42 + v7[v9 + 2 + v8].imaginary;
                    v32[v127].real = v32->real - v135 + v164;
                    *v170 = v7[v9 + 2 + v8].imaginary - v141 + v38;
                    v32[1].real = v135 + v32->real - v153;
                    v32[1].imaginary = v141 + v7[v9 + 2 + v8].imaginary - v42;
                    v32->real = v32->real - v135 - v164;
                    v32->imaginary = v7[v9 + 2 + v8].imaginary - v141 - v38;
                    v43 = &v7[v9 + 4 + v8];
                    v44 = a3->real1;
                    v45 = a3->imaginary1;
                    v131 = a3->imaginary1;
                    v136 = v44 * v7[v9 + 5 + v8].real - v45 * v7[v9 + 5 + v8].imaginary;
                    v142 = v45 * v7[v9 + 5 + v8].real + v44 * v7[v9 + 5 + v8].imaginary;
                    v154 = v44 * v43[v127 + 1].real - v45 * v43[v127 + 1].imaginary;
                    v46 = v44 * v43[v127 + 1].imaginary + v45 * v43[v127 + 1].real;
                    v47 = v43[v127].real - v154;
                    v171 = &v43[v127].imaginary;
                    v48 = *v171 - v46;
                    v165 = v47 * a3->real1 - v48 * v131;
                    v49 = v48 * a3->real1 + v47 * v131;
                    v50 = v46;
                    v51 = v154 + v43[v127].real;
                    v9 += 4;
                    v52 = v50 + *v171;
                    v8 += 4;
                    v155 = v51 * a3->real1 - v52 * v131;
                    v53 = v52 * a3->real1 + v51 * v131;
                    v43[v127 + 1].real = v155 + v43->real + v136;
                    v43[v127 + 1].imaginary = v142 + v53 + v43->imaginary;
                    v43[v127].real = v43->real - v136 + v165;
                    *v171 = v43->imaginary - v142 + v49;
                    v43[1].real = v136 + v43->real - v155;
                    v43[1].imaginary = v142 + v43->imaginary - v53;
                    v43->real = v43->real - v136 - v165;
                    v43->imaginary = v43->imaginary - v142 - v49;
                } while (v9 + 1 < v5);
            }
            if (v8 < v5)
            {
                v54 = v127;
                do
                {
                    v55 = a3->real1;
                    v56 = a3->imaginary1;
                    v57 = &v7[2 * v8];
                    v132 = a3->imaginary1;
                    v137 = v55 * v57[1].real - v56 * v57[1].imaginary;
                    v143 = v56 * v57[1].real + v55 * v57[1].imaginary;
                    v156 = v55 * v57[v54 + 1].real - v56 * v57[v54 + 1].imaginary;
                    v58 = v55 * v57[v54 + 1].imaginary + v56 * v57[v54 + 1].real;
                    v59 = v57[v54].real - v156;
                    v60 = v57[v54].imaginary - v58;
                    v166 = v59 * a3->real1 - v60 * v132;
                    v61 = v60 * a3->real1 + v59 * v132;
                    v62 = v58;
                    v63 = v156 + v57[v54].real;
                    v64 = v62 + v57[v54].imaginary;
                    v157 = v63 * a3->real1 - v64 * v132;
                    v65 = v64 * a3->real1 + v63 * v132;
                    v57[v54 + 1].real = v157 + v57->real + v137;
                    v57[v54 + 1].imaginary = v143 + v65 + v57->imaginary;
                    v57[v54].real = v57->real - v137 + v166;
                    v57[v54].imaginary = v57->imaginary - v143 + v61;
                    v57[1].real = v137 + v57->real - v157;
                    ++v8;
                    v57[1].imaginary = v143 + v57->imaginary - v65;
                    v57->real = v57->real - v137 - v166;
                    v57->imaginary = v57->imaginary - v143 - v61;
                } while (v8 < v5);
            }
            v6 = ++v176;
        } while (v176 < v5);
    }
    v66 = 2;
    v196 = a3 + 1;
    v175 = 2;
    if (num > 1)
    {
        numsubst1 = num - 1;
        while (1)
        {
            v67 = 0;
            v68 = ~(v66 - 1);
            v178 = v68;
            v195 = v66 << num;
            v192 = 0;
            if (v5)
            {
                v69 = v4;
                while (1)
                {
                    v138 = &v196[(v66 - 1) & v67].real1;
                    v70 = &v196[(v66 - 1) & (v67 + 1)];
                    v71 = (v67 + (v68 & v67)) << num;
                    v72 = 0;
                    v177 = &v70->real1;
                    v194 = &a1[v71];
                    v191 = 0;
                    while (1)
                    {
                        v73 = &v194[v72 + (v68 & v72)];
                        v74 = &v73[v195];
                        v75 = &v196[(v66 - 1) & (v72 + 1)].real1;
                        v76 = (v66 - 1) & v72;
                        v77 = v196[v76].real1;
                        v78 = &v196[v76].real1;
                        v79 = v78[1];
                        v80 = &v74[v66];
                        v81 = &v73[v66];
                        v167 = v138[1];
                        v158 = v77 * v81->real - v79 * v81->imaginary;
                        v144 = v77 * v81->imaginary + v79 * v81->real;
                        v179 = v77 * v80->real - v79 * v80->imaginary;
                        v82 = v77 * v80->imaginary + v79 * v80->real;
                        v83 = v74->real - v179;
                        v84 = v74->imaginary - v82;
                        v184 = v83 * *v138 - v84 * v167;
                        v85 = v84 * *v138 + v83 * v167;
                        v86 = v82;
                        v87 = v179 + v74->real;
                        v88 = v86 + v74->imaginary;
                        v180 = v87 * *v138 - v88 * v167;
                        v89 = v88 * *v138 + v87 * v167;
                        v80->real = v180 + v73->real + v158;
                        v80->imaginary = v89 + v73->imaginary + v144;
                        v74->real = v73->real - v158 + v184;
                        v74->imaginary = v73->imaginary - v144 + v85;
                        v81->real = v158 + v73->real - v180;
                        v81->imaginary = v144 + v73->imaginary - v89;
                        v73->real = v73->real - v158 - v184;
                        v73->imaginary = v73->imaginary - v144 - v85;
                        v90 = *v75;
                        v91 = v75[1];
                        v172 = v138[1];
                        v159 = v90 * v81[1].real - v91 * v81[1].imaginary;
                        v145 = v90 * v81[1].imaginary + v91 * v81[1].real;
                        v185 = v90 * v80[1].real - v91 * v80[1].imaginary;
                        v92 = v91 * v80[1].real + v90 * v80[1].imaginary;
                        v93 = v74[1].real - v185;
                        v94 = v74[1].imaginary - v92;
                        v181 = *v138 * v93 - v172 * v94;
                        v95 = *v138 * v94 + v172 * v93;
                        v96 = v92;
                        v97 = v185 + v74[1].real;
                        v98 = v96 + v74[1].imaginary;
                        v186 = *v138 * v97 - v172 * v98;
                        v99 = *v138 * v98 + v172 * v97;
                        v80[1].real = v186 + v73[1].real + v159;
                        v80[1].imaginary = v99 + v73[1].imaginary + v145;
                        v74[1].real = v73[1].real - v159 + v181;
                        v74[1].imaginary = v73[1].imaginary - v145 + v95;
                        v81[1].real = v159 + v73[1].real - v186;
                        v81[1].imaginary = v145 + v73[1].imaginary - v99;
                        v100 = &v81[v69];
                        v101 = &v80[v69];
                        v73[1].real = v73[1].real - v159 - v181;
                        v73[1].imaginary = v73[1].imaginary - v145 - v95;
                        v102 = *v78;
                        v103 = v78[1];
                        v173 = v177[1];
                        v160 = v102 * v100->real - v103 * v100->imaginary;
                        v146 = v102 * v100->imaginary + v103 * v100->real;
                        v187 = v102 * v101->real - v103 * v101->imaginary;
                        v104 = v102 * v101->imaginary + v103 * v101->real;
                        v105 = v74[v69].real - v187;
                        v106 = v74[v69].imaginary - v104;
                        v182 = *v177 * v105 - v173 * v106;
                        v107 = *v177 * v106 + v173 * v105;
                        v108 = v104;
                        v109 = v187 + v74[v69].real;
                        v110 = v108 + v74[v69].imaginary;
                        v188 = *v177 * v109 - v173 * v110;
                        v111 = *v177 * v110 + v173 * v109;
                        v101->real = v188 + v73[v69].real + v160;
                        v101->imaginary = v111 + v73[v69].imaginary + v146;
                        v74[v69].real = v73[v69].real - v160 + v182;
                        v74[v69].imaginary = v73[v69].imaginary - v146 + v107;
                        v112 = &v74[v69 + 1];
                        v100->real = v160 + v73[v69].real - v188;
                        v100->imaginary = v146 + v73[v69].imaginary - v111;
                        v73[v69].real = v73[v69].real - v160 - v182;
                        v73[v69].imaginary = v73[v69].imaginary - v146 - v107;
                        v113 = &v73[v69 + 1];
                        v114 = *v75;
                        v115 = v75[1];
                        v174 = v177[1];
                        v161 = v114 * v100[1].real - v115 * v100[1].imaginary;
                        v147 = v115 * v100[1].real + v114 * v100[1].imaginary;
                        v189 = v114 * v101[1].real - v115 * v101[1].imaginary;
                        v116 = v115 * v101[1].real + v114 * v101[1].imaginary;
                        v117 = v112->real - v189;
                        v118 = v112->imaginary - v116;
                        v183 = *v177 * v117 - v174 * v118;
                        v119 = *v177 * v118 + v174 * v117;
                        v120 = v116;
                        v121 = v189 + v112->real;
                        v122 = v120 + v112->imaginary;
                        v190 = *v177 * v121 - v174 * v122;
                        v123 = *v177 * v122 + v174 * v121;
                        v101[1].real = v190 + v161 + v113->real;
                        v101[1].imaginary = v123 + v113->imaginary + v147;
                        v112->real = v113->real - v161 + v183;
                        v112->imaginary = v113->imaginary - v147 + v119;
                        v100[1].real = v161 + v113->real - v190;
                        v5 = 1 << num >> 1;
                        v72 = v191 + 2;
                        v124 = v191 + 2 < v5;
                        v100[1].imaginary = v147 + v113->imaginary - v123;
                        v66 = v175;
                        v191 += 2;
                        v113->real = v113->real - v161 - v183;
                        v113->imaginary = v113->imaginary - v147 - v119;
                        if (!v124)
                            break;
                        v68 = v178;
                    }
                    v67 = v192 + 2;
                    v192 += 2;
                    if (v192 >= v5)
                        break;
                    v68 = v178;
                }
            }
            v125 = &v196[v66];
            v66 *= 2;
            v196 = v125;
            v175 = v66;
            if (!--numsubst1)
                break;
        }
    }
    WATERFFT_CorrectHeight(a1, num);
}

/* Advance the spectrum to `timeElapsed` and transform it into the space maps.
 *
 * Each spectral cell carries its amplitude in `real` and its dispersion
 * frequency in `imaginary`. Rotating a cell by its own frequency times the
 * elapsed time gives its phase at this instant, which is read out of COS_TABLE
 * rather than computed: the table holds 1024 cosine and sine pairs over a full
 * turn, and the magic rounding in f2iMagic converts the angle into an index.
 *
 * The height field and, when the choppy maps are present, the two horizontal
 * displacement fields are then brought into world space by the inverse FFT. */
void WATERFFT_Turn_Spectre(tWaterEngineStruct* engine, tFFTComplex* heightOut,
                          tFFTComplex* displacementXOut, tFFTComplex* displacementYOut, float timeElapsed)
{
    /* 1024 table entries over one turn, so this converts radians to an index. */
    const float angleToIndex = timeElapsed * 162.97466f;
    int i;

    for (i = 0; i < engine->spectroMapCompressedSize; i++)
    {
        const int entry = 2 * (f2iMagic(2 * angleToIndex * engine->spectroMap[i].imaginary) & 0x3ff);

        heightOut[i].real = COS_TABLE[entry] * engine->spectroMap[i].real;
        heightOut[i].imaginary = COS_TABLE[entry + 1] * engine->spectroMap[i].real;
    }

    if (displacementXOut && displacementYOut && engine->borderSizeDXDYPowerOfTwo)
    {
        WATERFFT_Deduct_DXDY_Spectre(engine, heightOut, displacementXOut, displacementYOut);
        WATERFFT_FFT2D(displacementXOut, engine->borderSizeDXDYPowerOfTwo, engine->FFTCostTableDXDY);
        WATERFFT_FFT2D(displacementYOut, engine->borderSizeDXDYPowerOfTwo, engine->FFTCostTableDXDY);
    }

    WATERFFT_FFT2D(heightOut, engine->borderSizePowerOfTwo, engine->FFTCostTable);
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


/* Rebuild the surface colour ramp.
 *
 * The ramp has 512 steps covering viewing angles from straight down through
 * grazing, and each step mixes three colours: the sky reflected off the
 * surface, the water itself, and the bottom seen through it. How much of each
 * applies is pure geometry, so it is worked out once and cached.
 *
 * The trick for caching it is to ask WTR_GetFresnelColor for the mix while the
 * three colours are temporarily set to pure red, green and blue. What comes
 * back is not a colour but the three weights, one per byte, and that is what
 * colorTableForFastRecompute holds. Every later rebuild just weights the real
 * colours by those bytes.
 *
 * Note the three components are summed in three different orders, which is
 * kept as it was so the packed bytes round the same way. */
void WTR_ComputeFresnel(tWaterParamsGlobalStruct* water)
{
    Vec3 fixedSurface;
    Vec3 sky, surface, bottom;
    Vec3 blended;
    int opaqueBottom;
    int opaqueSurface;
    int i;

    water->waterEngine.bottomColor &= 0x00ffffff;
    water->waterEngine.waterColor &= 0x00ffffff;
    water->waterEngine.skyColor &= 0x00ffffff;

    opaqueBottom = water->waterEngine.bottomColor | 0xff000000;
    water->waterEngine.bottomColor = opaqueBottom;

    /* The surface and the sky reflection are forced to one fixed colour. */
    fixedSurface.x = 0.30000001f;
    fixedSurface.y = 0.40000001f;
    fixedSurface.z = 0.5f;
    opaqueSurface = WTR_Vector_2_Color(&fixedSurface) | 0xFF000000;
    water->waterEngine.waterColor = opaqueSurface;
    water->waterEngine.skyColor = opaqueSurface;

    if (!water->waterEngine.colorTableFastRecomputeDone)
    {
        /* One pure channel each, so the returned mix is three weights. */
        water->waterEngine.bottomColor = (opaqueBottom & 0xFFFF0000) | 0xFF0000;
        water->waterEngine.waterColor = (opaqueSurface & 0xFF000000) | 0xFF00;
        water->waterEngine.skyColor = (opaqueSurface & 0xFF000000) | 0xFF;

        for (i = 0; i < 512; i++)
        {
            const float cosine = (float)((double)(256 - i) * (2.0 / 512.0));

            water->waterEngine.colorTableForFastRecompute[i] = WTR_GetFresnelColor(water, cosine);
        }

        water->waterEngine.bottomColor = opaqueBottom;
        water->waterEngine.waterColor = opaqueSurface;
        water->waterEngine.skyColor = opaqueSurface;
        water->waterEngine.colorTableFastRecomputeDone = 1;
    }

    WTR_Color_2_Vector(&sky, water->waterEngine.skyColor);
    WTR_Color_2_Vector(&bottom, water->waterEngine.bottomColor);
    WTR_Color_2_Vector(&surface, water->waterEngine.waterColor);

    for (i = 0; i < 512; i++)
    {
        const int packed = water->waterEngine.colorTableForFastRecompute[i];
        unsigned int remaining = (unsigned int)packed;
        double skyWeight, surfaceWeight, bottomWeight;
        Vec3 surfaceTerm;
        float bottomY, bottomZ;

        skyWeight = (double)(unsigned char)remaining * 0.00390625;
        remaining >>= 8;
        surfaceWeight = (double)(unsigned char)remaining * 0.00390625;
        remaining >>= 8;
        bottomWeight = (double)(unsigned char)remaining * 0.00390625;

        blended.y = (float)(sky.y * skyWeight);
        blended.z = (float)(sky.z * skyWeight);

        surfaceTerm.x = (float)(surface.x * surfaceWeight);
        surfaceTerm.y = (float)(surface.y * surfaceWeight);
        surfaceTerm.z = (float)(surface.z * surfaceWeight);

        blended.z = surfaceTerm.z + blended.z;

        bottomY = (float)(bottom.y * bottomWeight);
        bottomZ = (float)(bottom.z * bottomWeight);

        blended.x = (float)(bottom.x * bottomWeight + surfaceTerm.x + sky.x * skyWeight);
        blended.y = bottomY + surfaceTerm.y + blended.y;
        blended.z = bottomZ + blended.z;

        /* Carry the top byte of the weights through as the alpha. */
        water->waterEngine.colorTable[i] = (int)((remaining & 0xFFFFFF00) << 16) | WTR_Vector_2_Color(&blended);
    }

    memcpy(water->waterEngine.colorTableBackup, &water->waterEngine.colorTable[224], 256u);
    water->waterEngine.waterColorMode = 0;
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
            /* Repeat the first entry of the row as its wrap entry. The index
             * is a signed divide of the byte offset by -16, which is minus
             * the row length in entries. */
            ptrStart->f1 = ptrStart[-result].f1;
            ptrStart->f2 = ptrStart[-result].f2;
            ptrStart->f3 = ptrStart[-result].f3;
            v9 = ptrStart[-result].f4;
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
            /* Same wrap: minus the row length in entries, which is a signed
             * divide of the byte offset by -16. */
            v4->f1 = v4[-(2 * a3)].f1;
            v4->f2 = v4[-(2 * a3)].f2;
            v4->f3 = v4[-(2 * a3)].f3;
            v4->f4 = v4[-(2 * a3)].f4;
            v11 = &v4[v8 / 0x10u + 1];
            v11->f1 = v11[-(2 * a3)].f1;
            v11->f2 = v11[-(2 * a3)].f2;
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

/* Signed distance from a point to a plane, positive on the side the normal
 * points away from. The plane is the set of points whose projection onto
 * `planeNormal` equals `planeOffset`. */
float WTR_DistToPlane(Vec3* point, Vec3* planeNormal, float planeOffset)
{
    return planeOffset - (point->x * planeNormal->x + point->y * planeNormal->y + point->z * planeNormal->z);
}

/* Clip a polygon against one plane, Sutherland and Hodgman.
 *
 * Walks the edges of `input` and writes the part on the near side of the plane
 * into `output`: a vertex inside is kept, and an edge that crosses the plane
 * contributes the point where it crosses. The result can hold one vertex more
 * than the input, since a single crossing edge turns one vertex into two.
 *
 * Returns how many vertices were written. */
int WTR_Clip(Vec3* input, Vec3* output, Vec3* planeNormal, float planeOffset, int vertexCount)
{
    Vec3* const inputEnd = &input[vertexCount];
    Vec3* current = input;
    Vec3* previous = inputEnd - 1;
    float previousDistance;
    int written = 0;

    if (vertexCount <= 0)
        return 0;

    previousDistance = (float)WTR_DistToPlane(previous, planeNormal, planeOffset);

    while (current < inputEnd)
    {
        const float currentDistance = (float)WTR_DistToPlane(current, planeNormal, planeOffset);
        const int previousOutside = previousDistance < 0.0f;
        const int currentOutside = currentDistance < 0.0f;

        if (previousOutside != currentOutside)
        {
            /* The edge crosses the plane, so emit the crossing point. */
            const float t = -(previousDistance / (currentDistance - previousDistance));

            MATH_BlendVector(output, previous, current, t);
            output++;
            written++;
        }

        if (!currentOutside)
        {
            *output = *current;
            output++;
            written++;
        }

        previousDistance = currentDistance;
        previous = current;
        current++;
    }

    return written;
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

/* Switch the surface colour ramp between looking down at the water and looking
 * up through it.
 *
 * The ramp always starts from the same stored copy; what changes is which end
 * of it is flattened. Seen from above, the 32 entries from 224 become the sky
 * colour; seen from below, the 32 from 256 become the bottom colour. The mode
 * is remembered so the ramp is only rebuilt when the camera crosses. */
void WTR_UpdateColorMode(tWaterParamsGlobalStruct* water, float cameraHeight)
{
    const int wanted = (cameraHeight <= water->waterHeight) ? 1 : 2;
    int* entry;
    int i;

    if (water->waterEngine.waterColorMode == wanted)
        return;

    /* 256 bytes, so 64 entries from 224: only the top half of the stored copy
     * is restored, not all 128 of it. */
    memcpy(&water->waterEngine.colorTable[224], water->waterEngine.colorTableBackup, 256u);

    entry = &water->waterEngine.colorTable[wanted == 1 ? 256 : 224];

    for (i = 0; i < 32; i++)
        entry[i] = (wanted == 1) ? water->waterEngine.bottomColor : water->waterEngine.skyColor;

    water->waterEngine.waterColorMode = wanted;
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

/* Shade one row of the surface by height alone, blending between two colours
 * as the vertex rises above or falls below the reference height.
 *
 * The last entry of the row is not shaded but copied from the one before it,
 * because the row carries one vertex more than it has spans. */
void WTR_DirectComputeColors_CloudShading(Vec3* vertices, int* colorOut, int vertexnum,
                                          int lowColor, int highColor, float distanceMuliplier, float zDistance)
{
    const int shadedCount = vertexnum - 1;
    int i;

    for (i = 0; i < shadedCount; i++)
    {
        const float blend = (vertices[i].z - zDistance) * distanceMuliplier + 0.5f;

        colorOut[i] = LIGHT_ul_Interpol2Colors(lowColor, highColor, blend);
    }

    colorOut[shadedCount] = colorOut[shadedCount - 1];
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

/* Draw the water surface, called once per pass over the sorted draw lists.
 *
 * Only the pass numbered zero draws it, so the surface lands between the
 * opaque geometry and whatever sorts after it. */
void PC_AE_ZListHook(int zListId)
{
    if (zListId != 0)
        return;

    QueryPerformanceCounter(&PerfCounterStruct[43]);

    if (pst_GlobalsWaterParams)
        Workkkk_748 = WTR_Mesh_Do(pst_GlobalsWaterParams);

    PRO_QPC_RecordTimeDecRef(43);
}

/* Release a water block and everything hanging off it. */
void WTR_Mesh_Destroy(tWaterParamsGlobalStruct* water)
{
    WTR_FreeSubStruct(water);
    MEM_FreeAlign(water);
}

/* Drop the water before a world loads.
 *
 * Both the live block and the one parked while the water is switched off are
 * released, and the export block goes back to its built in settings, so the
 * next world starts from a clean state rather than inheriting this one\'s. */
void WTR_BeginLoadWorldHook()
{
    if (pst_GlobalsWaterParams)
        WTR_Mesh_Destroy(pst_GlobalsWaterParams);
    pst_GlobalsWaterParams = NULL;

    WTR_SetDefaulExp(&stExportWaterParrams);

    if (pst_GlobalsWaterParams_Save)
        WTR_Mesh_Destroy(pst_GlobalsWaterParams_Save);
    pst_GlobalsWaterParams_Save = NULL;
}


/* Smooth the surface normals along each row, averaging each with the entry
 * before it and the matching entry on the next row. The last row is left as
 * it is, as it has no next row to read.
 *
 * The body still carries its original pointer walk and wants rewriting. */
void WTR_WaterBlurNormal(Vec3* normals, int rowLength, int rowCount)
{
    Vec3* v3;
    Vec3* i;
    double v8;
    Vec3* v9;
    int v10;
    Vec3* j;
    double v12;

    v3 = &normals[rowLength * (rowCount - 1)];
    Vec3* _EAX = normals;
    if (normals < v3)
    {
        //_ESI = 4 * (3 * rowLength + 6);
        int _ECX = rowLength;
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
    v9 = normals;
    if (normals < v3)
    {
        v10 = rowLength;
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
