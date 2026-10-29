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

/* Find the water surface under a world position.
 *
 * The surface is not a height field: the choppy term moves each point
 * sideways as well as up, so the point of the surface that ends up above a
 * given x and y is not the one sampled at that x and y. The lookup therefore
 * solves backwards, sampling, seeing how far the sample drifted, and stepping
 * halfway back. Five passes when an absolute position is wanted, one when only
 * the displacement is.
 *
 * Sampling is bilinear over the base level of the concatenated map, whose rows
 * are one entry longer than the grid is wide so the fourth corner of the last
 * cell exists. The 40000 added before scaling keeps the indices positive for
 * any position in a world.
 *
 * With no water, or with cloud shading on, the surface is reported as
 * unreachably far below. */
void WTR_Get_A_DifVector(Vec3* position, Vec3* result, int wantAbsolute)
{
    tWaterParamsGlobalStruct* water = pst_GlobalsWaterParams;
    tWtrStruct2* map;
    float cellsPerMetre, choppyScale, heightScale;
    float inverseTiling;
    int side, mask, rowStride;
    int pass, passes;
    float x, y;
    float displacementX = 0.0f, displacementY = 0.0f, height = 0.0f;

    if (!water || water->waterEngine.useCloudShadingforColors)
    {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = -1000000000.0f;
        return;
    }

    passes = wantAbsolute ? 5 : 1;
    side = 1 << water->waterEngine.borderSizePowerOfTwo;
    mask = side - 1;
    rowStride = side + 1;
    map = water->concatenatedMap;

    cellsPerMetre = side * water->tiling;
    inverseTiling = 1.0f / water->tiling;
    choppyScale = inverseTiling * water->choppy;
    heightScale = inverseTiling * water->waveHeight;

    x = position->x;
    y = position->y;

    for (pass = 0; pass < passes; pass++)
    {
        const double u = (x + 40000.0) * cellsPerMetre;
        const double v = (y + 40000.0) * cellsPerMetre;

        /* Round down to a cell by the magic double trick: adding the magic
         * constant leaves the integer in the low word of the double. The
         * values here are far too large to survive a trip through float, so
         * this is done in double rather than through f2iMagic. */
        double roundedU = u - 0.5 + MATH_gd_magicDoubleForRound;
        double roundedV = v - 0.5 + MATH_gd_magicDoubleForRound;
        const int cellX = *(int32_t*)&roundedU;
        const int cellY = *(int32_t*)&roundedV;

        const double fractionX = u - (double)cellX;
        const double fractionY = v - (double)cellY;

        const int corner = (cellY & mask) * rowStride + (cellX & mask);
        tWtrStruct2* const bottomLeft = &map[corner];
        tWtrStruct2* const bottomRight = &map[corner + 1];
        tWtrStruct2* const topLeft = &map[corner + rowStride];
        tWtrStruct2* const topRight = &map[corner + rowStride + 1];

        const double oneMinusX = 1.0 - fractionX;
        const double weightBottomLeft = oneMinusX * (1.0 - fractionY);
        const double weightBottomRight = (1.0 - fractionY) * fractionX;
        const double weightTopLeft = oneMinusX * fractionY;
        const double weightTopRight = fractionY * fractionX;

        const float sumX = (float)(weightBottomRight * bottomRight->displacementX + weightBottomLeft * bottomLeft->displacementX);
        const float sumY = (float)(bottomRight->displacementY * weightBottomRight + bottomLeft->displacementY * weightBottomLeft);
        const float sumZ = (float)(bottomRight->height * weightBottomRight + bottomLeft->height * weightBottomLeft);

        const float withTopY = (float)(topLeft->displacementY * weightTopLeft + sumY);
        const float withTopZ = (float)(topLeft->height * weightTopLeft + sumZ);
        const float allZ = (float)(weightTopRight * topRight->height + withTopZ);

        displacementX = (float)(choppyScale * (weightTopRight * topRight->displacementX + weightTopLeft * topLeft->displacementX + sumX));
        displacementY = (float)(choppyScale * (topRight->displacementY * weightTopRight + withTopY));
        height = (float)(heightScale * allZ + water->waterHeight);

        /* Step halfway back towards the position asked for. The halving is in
         * double, as it was, so the step rounds the same. */
        x = (float)(x + (position->x - (displacementX + x)) * 0.5);
        y = (float)(y + (position->y - (displacementY + y)) * 0.5);
    }

    if (wantAbsolute)
    {
        result->x = displacementX + x;
        result->y = displacementY + y;
    }
    else
    {
        result->x = displacementX;
        result->y = displacementY;
    }

    result->z = height;
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

/* Phillips distribution: how much energy a wave of this wavevector carries.
 * Long waves dominate, by the inverse power of the length, and waves running
 * with the wind are favoured over waves running across it. */
float WATERFFT_philippe(Vec2* waveVector, Vec2* wind, float waveProp, float direction)
{
    float vecLen;
    double base;

    vecLen = (float)sqrt(waveVector->x * waveVector->x + waveVector->y * waveVector->y);
    if (vecLen == 0.0f)
        return 0.0f;

    // The cosine term is exactly -1 where the wavevector opposes the wind, so
    // this base is exactly zero there in exact arithmetic. Rounding leaves a
    // residue either side of zero, and a fractional power of a negative base
    // is a NaN that the later normalisation spreads over the whole spectrum.
    base = (wind->y * waveVector->y + waveVector->x * wind->x) * 0.5 / (sqrt(wind->x * wind->x + wind->y * wind->y) * vecLen) + 0.5;
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

/* Flip the sign of every other cell, in a checkerboard: the transform
 * leaves the grid with alternating cells negated, and this puts them
 * back. Each row starts one cell further along than the last, which is
 * what staggers the pattern. */
void WATERFFT_CorrectHeight(tFFTComplex* grid, char sizeLog2)
{
    const unsigned int side = 1u << sizeLog2;
    const unsigned int perRow = side >> 1;
    unsigned int rowsLeft = side;
    int firstCell = 1;

    if (!side)
        return;

    do
    {
        tFFTComplex* cell = &grid[firstCell];
        unsigned int remaining = perRow;

        while (remaining)
        {
            cell->real = -cell->real;
            cell->imaginary = -cell->imaginary;
            cell += 2;
            --remaining;
        }

        grid = cell - firstCell;
        firstCell ^= 1;
        --rowsLeft;
    } while (rowsLeft);
}

/* Derive the two horizontal displacement spectra from the height one.
 *
 * The displacement maps may be coarser than the height map, so the
 * source is read through the precalculated direction table and walked
 * with the two super shifts, which alternate from row to row to spread
 * the coarser sampling evenly. Each pass fills one row of both output
 * spectra.
 *
 * When the two maps differ by an odd number of levels the transform
 * lands on the opposite checkerboard phase, so both outputs are
 * corrected at the end. */
void WATERFFT_Deduct_DXDY_Spectre(tWaterEngineStruct* engine, tFFTComplex* heightSpectrum,
                                  tFFTComplex* displacementX, tFFTComplex* displacementY)
{
    const int sizeLog2 = engine->borderSizeDXDYPowerOfTwo;

    if (sizeLog2)
    {
        const unsigned int totalCells = 1u << (2 * sizeLog2);
        tFFTComplex* direction = engine->DXDYMap;
        tFFTComplex* outX = displacementX;
        tFFTComplex* outY = displacementY;
        tFFTComplex* source = &heightSpectrum[engine->superShift0
                              + (engine->superShift0 << engine->borderSizePowerOfTwo)];
        unsigned int filled = 0;
        int useSecondShift = 0;
        int side = 1 << sizeLog2;

        while (filled < totalCells)
        {
            tFFTComplex* const nextY = &outY[side];

            WATERFFT_Deduct_DXDY_Spectre_Line(outX, outY, &outX[side], direction, source,
                                              engine->superShift1, engine->superShift2);

            side = 1 << engine->borderSizeDXDYPowerOfTwo;
            outX += side;
            direction += side;
            source += (useSecondShift ? engine->superShift2 : engine->superShift1)
                      << engine->borderSizePowerOfTwo;
            useSecondShift ^= 1;
            filled += side;

            if (filled >= totalCells)
                break;

            outY = nextY;
        }

        /* an odd difference in level count inverts the checkerboard */
        if (((engine->borderSizePowerOfTwo - (unsigned char)engine->borderSizeDXDYPowerOfTwo) & 1) != 0)
        {
            WATERFFT_CorrectHeight(displacementX, (char)engine->borderSizeDXDYPowerOfTwo);
            WATERFFT_CorrectHeight(displacementY, (char)engine->borderSizeDXDYPowerOfTwo);
        }
    }
}

/* In-place two dimensional inverse transform of a square complex grid,
   vector radix 2 by 2: one butterfly combines a quad of cells spanning
   both axes at once rather than running all the rows and then all the
   columns.

   gridPowerOfTwo is log2 of the side, so the grid is
   (1 << gridPowerOfTwo) cells square. twiddleTable holds the stages
   laid end to end, each stage contributing half its span, so the
   cursor walks forward by half a span per stage and never rewinds.

   Two details are load bearing and neither is the textbook spelling:

   the row rotation is applied to the sum and the difference of the
   down and diagonal cells rather than to each of them, which is the
   same value in exact arithmetic, two multiplies cheaper, and rounds
   differently;

   and the four results go to the diagonally opposite corners of the
   quad they came from. The all-plus combination lands on the diagonal
   cell and the all-minus one on the base. Storing each result where it
   was read from yields a transform that is just as self consistent and
   is not this one, and the water is built around this one. */
void WATERFFT_FFT2D(tFFTComplex* grid, int gridPowerOfTwo, tFFTComplexCost* twiddleTable)
{
    const int side = 1 << gridPowerOfTwo;
    tFFTComplexCost* stageTwiddle = twiddleTable;
    int span;

    for (span = 2; span <= side; span <<= 1)
    {
        const int half = span >> 1;
        int blockRow, blockColumn, rowInBlock, columnInBlock;

        for (blockRow = 0; blockRow < side; blockRow += span)
        for (blockColumn = 0; blockColumn < side; blockColumn += span)
        for (rowInBlock = 0; rowInBlock < half; rowInBlock++)
        for (columnInBlock = 0; columnInBlock < half; columnInBlock++)
        {
            /* the quad: the base cell, its neighbour half a span
               across, the one half a span down, and the diagonal */
            tFFTComplex* const base     = &grid[(blockRow + rowInBlock) * side
                                                + blockColumn + columnInBlock];
            tFFTComplex* const across   = base + half;
            tFFTComplex* const down     = base + half * side;
            tFFTComplex* const diagonal = down + half;

            const float columnTwiddleReal = stageTwiddle[columnInBlock].real1;
            const float columnTwiddleImaginary = stageTwiddle[columnInBlock].imaginary1;
            const float rowTwiddleReal = stageTwiddle[rowInBlock].real1;
            const float rowTwiddleImaginary = stageTwiddle[rowInBlock].imaginary1;

            /* rotate the two right hand cells by the column twiddle */
            const float acrossReal = columnTwiddleReal * across->real
                                   - columnTwiddleImaginary * across->imaginary;
            const float acrossImaginary = columnTwiddleImaginary * across->real
                                        + columnTwiddleReal * across->imaginary;
            const float diagonalReal = columnTwiddleReal * diagonal->real
                                     - columnTwiddleImaginary * diagonal->imaginary;
            const float diagonalImaginary = columnTwiddleImaginary * diagonal->real
                                          + columnTwiddleReal * diagonal->imaginary;

            /* combine each with the down cell, then rotate the two
               combinations by the row twiddle */
            const float sumReal = down->real + diagonalReal;
            const float sumImaginary = down->imaginary + diagonalImaginary;
            const float differenceReal = down->real - diagonalReal;
            const float differenceImaginary = down->imaginary - diagonalImaginary;

            const float rotatedSumReal = rowTwiddleReal * sumReal
                                       - rowTwiddleImaginary * sumImaginary;
            const float rotatedSumImaginary = rowTwiddleImaginary * sumReal
                                            + rowTwiddleReal * sumImaginary;
            const float rotatedDifferenceReal = rowTwiddleReal * differenceReal
                                              - rowTwiddleImaginary * differenceImaginary;
            const float rotatedDifferenceImaginary = rowTwiddleImaginary * differenceReal
                                                   + rowTwiddleReal * differenceImaginary;

            const float baseReal = base->real;
            const float baseImaginary = base->imaginary;

            /* every input is in a local by now, so the crossed over
               stores below may overwrite in any order */
            diagonal->real = baseReal + acrossReal + rotatedSumReal;
            diagonal->imaginary = baseImaginary + acrossImaginary + rotatedSumImaginary;

            down->real = baseReal - acrossReal + rotatedDifferenceReal;
            down->imaginary = baseImaginary - acrossImaginary + rotatedDifferenceImaginary;

            across->real = baseReal + acrossReal - rotatedSumReal;
            across->imaginary = baseImaginary + acrossImaginary - rotatedSumImaginary;

            base->real = baseReal - acrossReal - rotatedDifferenceReal;
            base->imaginary = baseImaginary - acrossImaginary - rotatedDifferenceImaginary;
        }

        stageTwiddle += half;
    }

    WATERFFT_CorrectHeight(grid, (char)gridPowerOfTwo);
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

/* Unpack a packed colour into three components in zero to one, red from the
 * low byte then green then blue. */
void WTR_Color_2_Vector(Vec3* colourOut, uint32_t packed)
{
    colourOut->x = (float)((double)((packed >> 0)  & 0xff) * 0.0039215689);
    colourOut->y = (float)((double)((packed >> 8)  & 0xff) * 0.0039215689);
    colourOut->z = (float)((double)((packed >> 16) & 0xff) * 0.0039215689);
}

/* Mix the sky, water and bottom colours for one viewing angle and pack
 * the result, with the Fresnel reflectance as the alpha.
 *
 * cosThetaI is the cosine of the angle between the view ray and the
 * surface normal. Looking into the water from above it is negative,
 * and the full Fresnel term is evaluated: Snell's law through a
 * refractive index of 1.34 gives the refracted angle, and the
 * perpendicular and parallel reflectances are averaged for unpolarised
 * light. Looking the other way the approximation is cheaper, and past
 * the critical angle the sky contributes nothing at all.
 *
 * The nested one-minus terms below are left exactly as they were. They
 * do not cancel in floating point, and the alpha curve is a visible
 * gradient across the surface.
 *
 * WTR_Color_2_Vector unpacks to blue, green, red in x, y, z, which is
 * the order the packing at the end puts back. */
uint32_t WTR_GetFresnelColor(tWaterParamsGlobalStruct* wtr, float cosThetaI)
{
    unsigned int red, blue, green, alpha;
    Vec3 bottomColorVec, waterColorVec, skyColorVec;
    float bottomAndSkyColorFactor, bottomAndWaterColorFactor;
    float alphaNorm;
    double reflectance;
    double alphaOuter, alphaInner;

    if (cosThetaI <= 0.0)
    {
        const long double cosineRoot = sqrt(-cosThetaI);
        const float cosineRootAsFloat = (float)cosineRoot;
        const long double incidentAngle = acos((double)cosineRoot);
        const float incidentAngleAsFloat = (float)incidentAngle;
        /* Snell's law: 0.74626863 is one over the refractive index */
        const long double refractedAngle = asin(sin((double)incidentAngle) * 0.74626863);
        const float angleSum = (float)(incidentAngleAsFloat + refractedAngle);
        const long double angleDifference = refractedAngle - incidentAngleAsFloat;
        const float perpendicular = (float)(sin((double)angleDifference) / sin(angleSum));
        const long double tangentDifference = tan((double)angleDifference);
        const long double tangentSum = tan(angleSum);
        const long double parallel = tangentDifference / tangentSum;

        /* unpolarised light: the mean of the two squared reflectances */
        reflectance = (double)((parallel * parallel + perpendicular * perpendicular) * 0.5);
        bottomAndSkyColorFactor = (float)reflectance;
        bottomAndWaterColorFactor = (float)((1.0 - cosineRootAsFloat)
                                          * (1.0 - cosineRootAsFloat)
                                          * (1.0 - cosineRootAsFloat));
    }
    else
    {
        const long double refractedSine = sin(acos(cosThetaI)) * 1.34;

        /* at or past the critical angle nothing of the sky gets through */
        if (refractedSine < 1.0)
            bottomAndSkyColorFactor = (float)(1.0 - refractedSine);
        else
            bottomAndSkyColorFactor = 0.0;

        bottomAndWaterColorFactor = (float)((1.0 - (1.0 - cosThetaI))
                                          * (1.0 - (1.0 - cosThetaI)));
        reflectance = 1.0 - bottomAndSkyColorFactor;
    }

    WTR_Color_2_Vector(&skyColorVec, wtr->waterEngine.skyColor);
    WTR_Color_2_Vector(&bottomColorVec, wtr->waterEngine.bottomColor);
    WTR_Color_2_Vector(&waterColorVec, wtr->waterEngine.waterColor);

    /* bottom towards water by the depth term, then towards sky by the
       reflectance; the second blend leaves the result in waterColorVec */
    MATH_BlendVector(&bottomColorVec, &bottomColorVec, &waterColorVec, bottomAndWaterColorFactor);
    MATH_BlendVector(&waterColorVec, &bottomColorVec, &skyColorVec, bottomAndSkyColorFactor);

    /* the alpha curve: a fourth power of a fourth power, which keeps
       the surface clear at grazing angles and opaque looking down */
    alphaOuter = 1.0 - (1.0 - (1.0 - reflectance));
    alphaInner = 1.0 - alphaOuter * alphaOuter * (alphaOuter * alphaOuter);
    alphaNorm = (float)(alphaInner * alphaInner * (alphaInner * alphaInner));

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

/* Reduce two by two blocks of two source rows into two output entries.
 *
 * Averages the four corners of each block for the two displacement fields and
 * the height. The fourth field is not touched; the caller carries it. */
void WTR_Compute_MipMap_sub_noSSE(tWtrStruct2* topRow, tWtrStruct2* out, tWtrStruct2* bottomRow)
{
    int i;

    for (i = 0; i < 2; i++)
    {
        const int left = 2 * i;
        const int right = left + 1;

        out[i].displacementX = (float)((bottomRow[right].displacementX + topRow[right].displacementX + topRow[left].displacementX + bottomRow[left].displacementX) * 0.25);
        out[i].displacementY = (float)((bottomRow[right].displacementY + topRow[right].displacementY + topRow[left].displacementY + bottomRow[left].displacementY) * 0.25);
        out[i].height = (float)((bottomRow[right].height + topRow[right].height + topRow[left].height + bottomRow[left].height) * 0.25);
    }
}

/* Reduce one level of the concatenated map into the next.
 *
 * outputSide is the side of the level being written; the level being
 * read is twice that. Each output cell averages a two by two block
 * across two source rows, and each output row is followed by a wrap
 * cell repeating the row's first - the same shape the finest level
 * has, so that a level of side S occupies (S + 1) squared entries.
 * The wrap row at the end is a copy of the first row.
 *
 * The averaging leaves the fourth component of a cell alone; only the
 * displacement pair and the height are reduced. */
void WTR_Compute_MipMap(int outputSide, tWtrStruct2* source, tWtrStruct2* destination)
{
    const int sourceStride = 2 * outputSide + 1;
    const int sourceRowPair = 2 * outputSide + 2;
    tWtrStruct2* const bodyEnd = &destination[outputSide * (outputSide + 1)];
    tWtrStruct2* out = destination;

    while (out < bodyEnd)
    {
        tWtrStruct2* const rowEnd = &out[outputSide];

        while (out < rowEnd)
        {
            WTR_Compute_MipMap_sub_noSSE(source, out, &source[sourceStride]);
            out += 2;
            source += 4;
        }

        /* wrap cell: repeat the first cell of the row just written */
        {
            const float spare = out[-outputSide].spare;
            out->displacementX = out[-outputSide].displacementX;
            out->displacementY = out[-outputSide].displacementY;
            out->height = out[-outputSide].height;
            out->spare = spare;
            ++out;
        }

        source += sourceRowPair;
    }

    /* wrap row: whatever of the level is still unwritten, taken from
       its first row. The row loop steps two cells at a time, so for a
       level only one cell wide it overshoots and less is left here. */
    memcpy(out, destination,
           16 * (int)((((char*)&bodyEnd[1] + 16 * outputSide) - (char*)out) >> 4));
}

/* Expand one two by two block of the finer level from the coarser one:
 * the coarse cell is copied to the top left and the other three are
 * interpolated from it and its neighbours across, below and diagonally.
 *
 * NOTE: the four blends below write their result to the argument the
 * surrounding code treats as a source. As they stand, the top right
 * and bottom left of the destination block are never written and two
 * cells of the source level are overwritten instead. Reaching this at
 * all needs a choppy map coarser than the height map, which the
 * defaults do not set up, so nothing exercises it today. Left as it
 * was rather than changed blind. */
void WTR_BackExtendDXDY_sub_unknown(tWtrStruct2* sourceBelow, tWtrStruct2* source,
                                    tWtrStruct2* destination, tWtrStruct2* destinationBelow)
{
    *destination = *source;
    MATH_BlendVector((Vec3*)source, (Vec3*)&source[1], (Vec3*)&destination[1], 0.5);
    MATH_BlendVector((Vec3*)source, (Vec3*)sourceBelow, (Vec3*)destinationBelow, 0.5);
    MATH_BlendVector((Vec3*)sourceBelow, (Vec3*)&sourceBelow[1], (Vec3*)&destinationBelow[1], 0.5);
    MATH_BlendVector((Vec3*)&destinationBelow[1], (Vec3*)&destination[1], (Vec3*)&destinationBelow[1], 0.5);
}

/* Carry the horizontal displacement from a coarse level of the
 * concatenated map down into the next finer one, which has twice the
 * side. Each coarse cell expands into a two by two block, so one pass
 * of the inner loop fills a pair of finer rows; each of those rows is
 * then closed with the wrap cell repeating its own first cell, and the
 * finer level's wrap row is copied from its first row.
 *
 * sourceSide is the side of the coarse level. */
void WTR_BackExtendDXDY_0(tWtrStruct2* source, tWtrStruct2* destination, int sourceSide)
{
    const int sourceStride = sourceSide + 1;
    const int destinationStride = 2 * sourceSide + 1;
    tWtrStruct2* const sourceEnd = &source[sourceSide * (sourceSide + 1)];
    tWtrStruct2* in = source;
    tWtrStruct2* out = destination;

    while (in < sourceEnd)
    {
        tWtrStruct2* const rowEnd = &in[sourceSide];
        tWtrStruct2* lower;

        while (in < rowEnd)
        {
            WTR_BackExtendDXDY_sub_unknown(&in[sourceStride], in, out, &out[destinationStride]);
            ++in;
            out += 2;
        }

        /* `out` is now the wrap cell of the upper row of the pair, and
           out[1] the first cell of the lower row, so each wrap cell is
           its own row start reached by stepping back one row of cells */
        out->displacementX = out[-(2 * sourceSide)].displacementX;
        out->displacementY = out[-(2 * sourceSide)].displacementY;
        out->height = out[-(2 * sourceSide)].height;
        out->spare = out[-(2 * sourceSide)].spare;

        lower = &out[destinationStride];
        {
            const float spare = lower[-(2 * sourceSide)].spare;
            lower->displacementX = lower[-(2 * sourceSide)].displacementX;
            lower->displacementY = lower[-(2 * sourceSide)].displacementY;
            lower->height = lower[-(2 * sourceSide)].height;
            lower->spare = spare;
        }

        out = lower + 1;
        ++in;
    }

    /* the wrap row of the finer level: a copy of its first row */
    memcpy(out, destination, 16 * destinationStride);
}

/* Build the concatenated wave map: the finest level filled from the
   space maps, then a pyramid of successively halved copies above it,
   all in one buffer laid end to end.

   A level whose side is S occupies (S + 1) squared entries, not S
   squared: every row carries one extra cell repeating the first of
   that row, and the level carries one extra row repeating the first.
   That wrap margin is what lets the sampler interpolate across the
   seam without a modulo, and it is why the stride from one level to
   the next is (S + 1) squared rather than S squared.

   Horizontal displacement is only available at the resolution of the
   choppy maps, which may be coarser than the height map. So the
   finest level gets height alone unless the two resolutions match,
   and the displacement is filled in later at whichever level does
   match, then pushed back up the pyramid by WTR_BackExtendDXDY_0. */
void WTR_Concate(tWaterParamsGlobalStruct* water)
{
    const unsigned int heightShift = water->waterEngine.waveMapShift;
    const unsigned int choppyShift = water->waterEngine.waveMapShiftCHP;
    const int heightSide = 1 << heightShift;

    tWtrStruct2* cell = water->concatenatedMap;
    tWtrStruct2* const finestEnd = &cell[heightSide + (heightSide * heightSide)];
    const tFFTComplex* height = water->spaceMap;
    const tFFTComplex* displacementX = water->spaceMapDX;
    const tFFTComplex* displacementY = water->spaceMapDY;

    tWtrStruct2* levelSource;
    tWtrStruct2* levelDestination;
    tWtrStruct2* previousLevel;
    unsigned int levelSide;
    int finestEntries;

    /* ---- the finest level, one row at a time plus its wrap cell ---- */
    while (cell < finestEnd)
    {
        tWtrStruct2* const rowEnd = &cell[heightSide];

        if (heightShift == choppyShift)
        {
            /* displacement is available at this resolution */
            while (cell < rowEnd)
            {
                cell->displacementX = displacementX->real;
                cell->displacementY = displacementY->real;
                cell->height = height->real;
                ++cell;
                ++displacementX;
                ++displacementY;
                ++height;
            }
        }
        else
        {
            /* height only; displacement arrives at a coarser level */
            while (cell < rowEnd)
            {
                cell->height = height->real;
                ++cell;
                ++height;
            }
        }

        /* wrap cell: repeat the first cell of the row just written */
        {
            const float* const rowStart = &cell[-heightSide].displacementX;
            cell->displacementX = rowStart[0];
            cell->displacementY = rowStart[1];
            cell->height = rowStart[2];
            cell->spare = rowStart[3];
            ++cell;
        }
    }

    /* wrap row: repeat the first row, its own wrap cell included */
    {
        const tWtrStruct2* firstRow = water->concatenatedMap;
        tWtrStruct2* const wrapRowEnd = &finestEnd[heightSide + 1];

        while (cell < wrapRowEnd)
        {
            cell->displacementX = firstRow->displacementX;
            cell->displacementY = firstRow->displacementY;
            cell->height = firstRow->height;
            cell->spare = firstRow->spare;
            ++cell;
            ++firstRow;
        }
    }

    /* ---- the pyramid above it ---- */
    finestEntries = heightSide * (heightSide + 2) + 1; /* (S + 1) squared */
    levelSource = water->concatenatedMap;
    previousLevel = levelSource;
    levelDestination = &levelSource[finestEntries];
    levelSide = (unsigned int)heightSide >> 1;

    while (levelSide)
    {
        int levelEntries;

        WTR_Compute_MipMap(levelSide, levelSource, levelDestination);

        /* if this is the level the choppy maps are sized for, fill the
           displacement here and push it back down to the finer levels */
        if (levelSide == (1u << choppyShift) && choppyShift)
        {
            const int choppySide = 1 << choppyShift;
            tWtrStruct2* choppyCell = levelDestination;
            tWtrStruct2* const choppyEnd =
                &levelDestination[choppySide + (choppySide * choppySide)];

            while (choppyCell < choppyEnd)
            {
                tWtrStruct2* const rowEnd = &choppyCell[choppySide];

                while (choppyCell < rowEnd)
                {
                    choppyCell->displacementX = displacementX->real;
                    choppyCell->displacementY = displacementY->real;
                    ++choppyCell;
                    ++displacementX;
                    ++displacementY;
                }

                {
                    const float* const rowStart = &choppyCell[-choppySide].displacementX;
                    choppyCell->displacementX = rowStart[0];
                    choppyCell->displacementY = rowStart[1];
                    choppyCell->height = rowStart[2];
                    choppyCell->spare = rowStart[3];
                    ++choppyCell;
                }
            }

            /* this level's wrap row */
            {
                const tWtrStruct2* firstRow = levelDestination;
                tWtrStruct2* const wrapRowEnd = &choppyEnd[choppySide + 1];

                while (choppyCell < wrapRowEnd)
                {
                    choppyCell->displacementX = firstRow->displacementX;
                    choppyCell->displacementY = firstRow->displacementY;
                    choppyCell->height = firstRow->height;
                    choppyCell->spare = firstRow->spare;
                    ++choppyCell;
                    ++firstRow;
                }
            }

            /* walk back down to the finest level, doubling the side at
               each step, extending the displacement into each one */
            {
                tWtrStruct2* coarser = levelDestination;
                tWtrStruct2* finer = previousLevel;
                int coarserSide = (int)levelSide;

                while (finer >= water->concatenatedMap)
                {
                    WTR_BackExtendDXDY_0(coarser, finer, coarserSide);

                    /* step down one level: `finer` becomes the coarse
                       side of the next pair, so the distance back is
                       the size of the level below it, not of this one */
                    coarser = finer;
                    coarserSide *= 2;
                    finer -= (2 * coarserSide + 1) * (2 * coarserSide + 1);
                }
            }
        }

        levelEntries = (int)(levelSide * (levelSide + 2)) + 1;
        previousLevel = levelDestination;
        levelSource = levelDestination;
        levelDestination += levelEntries;
        levelSide >>= 1;
    }
}

/* Take the parameters a level exports and apply them to the live water.
 *
 * Disabling the water parks the globals in the save slot rather than
 * freeing them, so re-enabling it later gets the same block back with
 * its buffers intact; only the very first enable allocates.
 *
 * Any field left at zero means "keep what is already there", and the
 * two map shifts are clamped to the four to nine the allocator can
 * serve, with the choppy map never finer than the height map.
 *
 * Three things can follow, and they are independent:
 *   - buffers are reallocated when any requested size grew, since the
 *     existing ones would be too small;
 *   - the spectrum is rebuilt when any size or wave parameter changed
 *     at all, in either direction;
 *   - the Fresnel table is recomputed when a colour changed. */
void WTR_SetExportParams(tUserWaterParamsStruct* exported)
{
    tWaterParamsGlobalStruct* water = pst_GlobalsWaterParams;
    int needsRealloc = 0;
    int needsRebuild = 0;
    int colorChanged = 0;
    unsigned int requestedShift;
    unsigned int requestedWidth, currentWidth;

    if (water)
    {
        if (!exported->waterEnabled)
        {
            /* park it; the buffers are kept for the next enable */
            pst_GlobalsWaterParams = 0;
            pst_GlobalsWaterParams_Save = water;
            return;
        }
    }
    else
    {
        if (!exported->waterEnabled)
            return;

        if (pst_GlobalsWaterParams_Save)
        {
            pst_GlobalsWaterParams = pst_GlobalsWaterParams_Save;
            pst_GlobalsWaterParams_Save = 0;
            water = pst_GlobalsWaterParams;
        }
        else
        {
            pst_GlobalsWaterParams =
                (tWaterParamsGlobalStruct*)MEM_p_AllocAligned(16168, 32);
            WTR_SetDefault(pst_GlobalsWaterParams);
            water = pst_GlobalsWaterParams;
            needsRealloc = 1;
            needsRebuild = 1;
            colorChanged = 1;
        }
    }

    /* zero means keep the current value */
    if (!exported->frustrumMeshSX)
        exported->frustrumMeshSX = water->horizontalVertexNum;
    if (!exported->frustrumMeshSY)
        exported->frustrumMeshSY = water->verticallVertexNum;
    if (!exported->waveMapShift)
        exported->waveMapShift = water->waterEngine.waveMapShift;

    if (exported->waveMapShift > 9u)
        exported->waveMapShift = 9;
    if (exported->waveMapShift < 4u)
        exported->waveMapShift = 4;
    if (exported->waveMapShiftCHP > 9u)
        exported->waveMapShiftCHP = 9;
    if (exported->waveMapShiftCHP < 4u)
        exported->waveMapShiftCHP = 4;

    requestedShift = exported->waveMapShift;
    if ((unsigned int)exported->waveMapShiftCHP > requestedShift)
        exported->waveMapShiftCHP = requestedShift;

    if (exported->skyColor != water->waterEngine.skyColor
        || exported->bottomColor != water->waterEngine.bottomColor
        || exported->waterColor != water->waterEngine.waterColor)
    {
        colorChanged = 1;
    }

    requestedWidth = exported->frustrumMeshSX;
    currentWidth = water->horizontalVertexNum;

    /* anything that grew needs bigger buffers */
    if (!(requestedWidth <= currentWidth
          && (unsigned int)exported->frustrumMeshSY <= (unsigned int)water->verticallVertexNum
          && requestedShift <= water->waterEngine.waveMapShift
          && (unsigned int)exported->waveMapShiftCHP <= water->waterEngine.waveMapShiftCHP))
    {
        needsRealloc = 1;
    }

    /* anything that changed at all needs the spectrum rebuilt */
    if (requestedWidth != currentWidth
        || exported->frustrumMeshSY != water->verticallVertexNum
        || requestedShift != water->waterEngine.waveMapShift
        || (unsigned int)exported->waveMapShiftCHP != water->waterEngine.waveMapShiftCHP)
    {
        needsRebuild = 1;
    }

    water->horizontalVertexNum = requestedWidth;
    water->verticallVertexNum = exported->frustrumMeshSY;
    water->waterEngine.waveMapShift = exported->waveMapShift;
    water->waterEngine.waveMapShiftCHP = exported->waveMapShiftCHP;
    water->waterEngine.isSymmetryEnabled = exported->isSymmetryActive;
    water->waterEngine.bottomColor = exported->bottomColor;
    water->waterEngine.waterColor = exported->waterColor;
    water->waterEngine.skyColor = exported->skyColor;
    water->waveHeight = exported->waveHeight;
    water->choppy = exported->choppy;
    water->tiling = exported->tiling;
    water->speed = exported->speed;
    water->waterHeight = exported->waterHeight;
    water->waterEngine.useCloudShadingforColors = exported->cludShadingIsEnabled;
    water->waterEngine.borderSizePowerOfTwo = water->waterEngine.waveMapShift;
    water->waterEngine.borderSizeDXDYPowerOfTwo = water->waterEngine.waveMapShiftCHP;

    if (water->waterEngine.waveProp != exported->waveProp
        || water->waterEngine.direction != exported->direction)
    {
        needsRebuild = 1;
    }

    water->waterEngine.waveProp = exported->waveProp;
    water->waterEngine.direction = exported->direction;
    water->waterEngine.textureRefId = exported->textureRefID;

    if (needsRealloc)
    {
        WTR_FreeSubStruct(water);
        WTR_AllocSubStruct(water);
    }

    if (needsRealloc || needsRebuild)
    {
        WATERFFT_Init_Spectre(&water->waterEngine);
        WATERFFT_Turn_Spectre(&water->waterEngine, water->spaceMap,
                              water->spaceMapDX, water->spaceMapDY,
                              water->waterEngine.timeElapsed);
        WTR_Concate(water);
    }

    if (colorChanged)
        WTR_ComputeFresnel(water);

    waterHeight = water->waterHeight;
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

/* Normalise a plane normal in place. */
static void WTR_NormalisePlane(Vec3* plane)
{
    const double reciprocal = 1.0 / sqrt(plane->z * plane->z
                                       + plane->y * plane->y
                                       + plane->x * plane->x);
    plane->x = (float)(plane->x * reciprocal);
    plane->y = (float)(plane->y * reciprocal);
    plane->z = (float)(plane->z * reciprocal);
}

/* Distance of the camera from a plane through the origin, with the
   plane pushed outwards by the choppy displacement in the horizontal
   and the wave amplitude in the vertical, so that water lying flat
   inside the view but displaced outwards still gets meshed. */
static float WTR_PlaneOffsetForCamera(const Vec3* plane, const Vec3* cameraPosition,
                                      double choppyQuarter, double waveQuarter)
{
    return (float)(sqrt(plane->y * plane->y + plane->x * plane->x) * choppyQuarter
        + fabs(((plane->y + plane->x) * 0.0 + plane->z) * waveQuarter)
        + plane->z * cameraPosition->z
        + plane->y * cameraPosition->y
        + plane->x * cameraPosition->x);
}

/* Find the quad of the water plane that the camera can see.
 *
 * A frame is built from the view direction: sideAxis is the horizontal
 * vector perpendicular to it, upAxis completes the pair. Four corner
 * directions are spread from those two, and each consecutive pair of
 * them spans one side plane of the view volume; the reversed view
 * direction gives the back plane. A deliberately oversized quad is
 * laid on the water plane and clipped against all five, and the four
 * corners that survive are written out, the pair nearest the camera
 * first.
 *
 * viewDirection is normalised in place, which both callers rely on.
 * axisOut receives upAxis before it is scaled; neither caller reads it.
 *
 * planeExtent sizes the quad laid on the water plane, forwardScale and
 * acrossScale the view volume, and waveExpansion and choppyExpansion
 * how far the side planes are pushed out. Returns 0 when the clip
 * leaves nothing to mesh.
 *
 * Several terms below multiply by zero rather than being folded away.
 * They are kept because they carry a non-finite view direction through
 * to the result instead of quietly producing a finite one. */
int WTR_Compute4Points(Vec3* corners, Vec3* axisOut, Vec3* cameraPosition,
                       Vec3* viewDirection, float planeHeight, float planeExtent,
                       float forwardScale, float acrossScale,
                       float waveExpansion, float choppyExpansion)
{
    Vec3 upAxis, groundDirection;
    Vec3 plane01, plane12, plane32, plane03, backPlane;
    Vec3 clipA[8];
    Vec3 clipB[8];

    /* the horizontal vector perpendicular to the view direction */
    const double zeroFromZ = viewDirection->z * 0.0;
    const double sideRawY = zeroFromZ - viewDirection->y;
    const float sideRawX = (float)(viewDirection->x - zeroFromZ);
    const float sideRawZ = (float)(viewDirection->y * 0.0 - viewDirection->x * 0.0);
    const double sideReciprocal = 1.0 / sqrt(sideRawZ * sideRawZ
                                           + sideRawX * sideRawX
                                           + sideRawY * sideRawY);
    /* the first component keeps the double reciprocal, the other two
       take it rounded to float, exactly as the original does */
    const float sideReciprocalAsFloat = (float)sideReciprocal;
    const double sideAxisX = sideReciprocal * sideRawY;
    const double sideAxisY = sideRawX * sideReciprocalAsFloat;
    const float sideAxisZ = sideRawZ * sideReciprocalAsFloat;

    /* normalise the view direction in place */
    /* the first term is widened before squaring and the other two are
       not, which on a target that rounds every float product shows up
       in the last bits of the normalised direction */
    const double viewX = viewDirection->x;
    const double viewLength = sqrt(viewX * viewX
                                 + viewDirection->y * viewDirection->y
                                 + viewDirection->z * viewDirection->z);
    const double forwardX = 1.0 / viewLength * viewX;
    const float forwardY = (float)(1.0 / viewLength * viewDirection->y);
    const float forwardZ = (float)(1.0 / viewLength * viewDirection->z);

    const double waveQuarter = waveExpansion * 0.25;
    const double choppyQuarter = choppyExpansion * 0.25;

    float offset01, offset12, offset32, offset03, backOffset;
    char survivors;
    int firstCorner;
    const Vec3* pick;

    viewDirection->x = (float)forwardX;
    viewDirection->y = forwardY;
    viewDirection->z = forwardZ;

    /* upAxis = viewDirection crossed with sideAxis */
    upAxis.x = (float)(sideAxisZ * forwardY - sideAxisY * forwardZ);
    upAxis.y = (float)(forwardZ * sideAxisX - sideAxisZ * forwardX);
    upAxis.z = (float)(sideAxisY * forwardX - forwardY * sideAxisX);
    *axisOut = upAxis;

    /* ---- the four corner directions of the view volume ---- */
    {
        const float forwardScaledX = (float)(forwardX * forwardScale);
        const float forwardScaledY = (float)(forwardY * forwardScale);
        const float forwardScaledZ = (float)(forwardZ * forwardScale);

        /* the volume is not symmetric about the view direction: one
           side is spread a further four per cent */
        const float sideNarrowX = (float)(sideAxisX * 1.05);
        const double sideNarrowZ = sideAxisZ * 1.05;

        const double bottomX = forwardScaledX - sideNarrowX;
        const double bottomY = forwardScaledY - sideAxisY * 1.05;
        const float bottomZ = (float)(forwardScaledZ - sideNarrowZ);

        const float sideWideX = (float)(sideNarrowX * 1.04);
        const float sideWideY = (float)(sideAxisY * 1.05 * 1.04);
        const float sideWideZ = (float)(sideNarrowZ * 1.04);

        const double topX = forwardScaledX + sideWideX;
        const double topY = forwardScaledY + sideWideY;
        const float topZ = (float)(forwardScaledZ + sideWideZ);

        corners[1].x = (float)bottomX;
        corners[1].y = (float)bottomY;
        corners[1].z = bottomZ;
        corners[2].x = (float)bottomX;
        corners[2].y = (float)bottomY;
        corners[2].z = bottomZ;

        corners->x = (float)topX;
        corners->y = (float)topY;
        corners->z = topZ;
        corners[3].x = (float)topX;
        corners[3].y = (float)topY;
        corners[3].z = topZ;

        upAxis.x = (float)(upAxis.x * acrossScale);
        upAxis.y = (float)(upAxis.y * acrossScale);
        upAxis.z = (float)(upAxis.z * acrossScale);

        corners->x = upAxis.x + corners->x;
        corners->y = upAxis.y + corners->y;
        corners->z = upAxis.z + corners->z;
        corners[1].x = upAxis.x + corners[1].x;
        corners[1].y = corners[1].y + upAxis.y;
        corners[1].z = corners[1].z + upAxis.z;
        corners[2].x = corners[2].x - upAxis.x;
        corners[2].y = corners[2].y - upAxis.y;
        corners[2].z = corners[2].z - upAxis.z;
        corners[3].x = corners[3].x - upAxis.x;
        corners[3].y = corners[3].y - upAxis.y;
        corners[3].z = corners[3].z - upAxis.z;
    }

    /* ---- each consecutive pair of corners spans a side plane ---- */
    {
        const double normalX = corners->z * corners[1].y - corners[1].z * corners->y;
        const double normalY = corners[1].z * corners->x - corners[1].x * corners->z;
        double reciprocal;

        plane01.z = (float)(corners[1].x * corners->y - corners->x * corners[1].y);

        plane12.x = (float)(corners[1].z * corners[2].y - corners[1].y * corners[2].z);
        plane12.y = (float)(corners[1].x * corners[2].z - corners[1].z * corners[2].x);
        plane12.z = (float)(corners[1].y * corners[2].x - corners[1].x * corners[2].y);

        plane32.x = (float)(corners[3].y * corners[2].z - corners[3].z * corners[2].y);
        plane32.y = (float)(corners[3].z * corners[2].x - corners[2].z * corners[3].x);
        plane32.z = (float)(corners[2].y * corners[3].x - corners[3].y * corners[2].x);

        plane03.x = (float)(corners->y * corners[3].z - corners[3].y * corners->z);
        plane03.y = (float)(corners->z * corners[3].x - corners[3].z * corners->x);
        plane03.z = (float)(corners[3].y * corners->x - corners->y * corners[3].x);

        backPlane.x = (float)(viewDirection->x * -1.0);
        backPlane.y = (float)(viewDirection->y * -1.0);
        backPlane.z = (float)(viewDirection->z * -1.0);

        reciprocal = 1.0 / sqrt(normalY * normalY + normalX * normalX
                              + plane01.z * plane01.z);
        plane01.x = (float)(reciprocal * normalX);
        plane01.y = (float)((float)reciprocal * normalY);
        plane01.z = (float)(plane01.z * (float)reciprocal);
    }

    WTR_NormalisePlane(&plane12);
    WTR_NormalisePlane(&plane32);
    WTR_NormalisePlane(&plane03);
    WTR_NormalisePlane(&backPlane);

    offset12 = WTR_PlaneOffsetForCamera(&plane12, cameraPosition, choppyQuarter, waveQuarter);
    offset03 = WTR_PlaneOffsetForCamera(&plane03, cameraPosition, choppyQuarter, waveQuarter);

    groundDirection.x = viewDirection->x;
    groundDirection.y = viewDirection->y;

    offset01 = WTR_PlaneOffsetForCamera(&plane01, cameraPosition, choppyQuarter, waveQuarter);
    offset32 = WTR_PlaneOffsetForCamera(&plane32, cameraPosition, choppyQuarter, waveQuarter);

    backOffset = (float)(backPlane.z * cameraPosition->z + backPlane.x * cameraPosition->x
                       + backPlane.y * cameraPosition->y + 2.0);

    /* ---- an oversized quad on the water plane ---- */
    {
        /* Whether an intermediate is rounded to float before being
           added to the base differs between x, y and z below, and
           between the corners. It is kept exactly as it was: these are
           coordinates in the tens of thousands, so a rounding that
           moves one corner moves which triangles survive the clip. */
        const double groundReciprocal = 1.0 / sqrt(groundDirection.x * groundDirection.x
                                                 + groundDirection.y * groundDirection.y);
        double zeroFromReciprocal, zeroFromZero, perpX, perpZ;
        float perpY;
        float baseX, baseY, baseZ, shiftedX, shiftedY, shiftedZ;
        float towardsY, towardsZ;
        double behind, wide, narrow, twice;

        groundDirection.x = (float)(groundDirection.x * groundReciprocal);
        groundDirection.y = (float)(groundDirection.y * groundReciprocal);
        zeroFromReciprocal = groundReciprocal * 0.0;
        groundDirection.z = (float)zeroFromReciprocal;
        zeroFromZero = zeroFromReciprocal * 0.0;

        /* the ground direction turned a quarter turn */
        perpY = (float)(groundDirection.y - zeroFromZero);
        perpX = zeroFromZero - groundDirection.x;
        perpZ = groundDirection.x * 0.0 - groundDirection.y * 0.0;

        /* start well behind the camera so the quad straddles it */
        behind = -planeExtent;
        towardsY = (float)(groundDirection.y * behind);
        towardsZ = (float)(behind * groundDirection.z);
        baseX = (float)(groundDirection.x * behind + cameraPosition->x);
        baseY = (float)(towardsY + cameraPosition->y);
        baseZ = (float)(towardsZ + planeHeight);

        wide = planeExtent * 8.0;
        narrow = planeExtent * -8.0;
        twice = planeExtent + planeExtent;

        clipA[2].x = (float)(perpY * wide + baseX);
        clipA[2].y = (float)((float)(perpX * wide) + baseY);
        clipA[2].z = (float)((float)(perpZ * wide) + baseZ);

        clipA[3].x = (float)(perpY * narrow + baseX);
        clipA[3].y = (float)((float)(perpX * narrow) + baseY);
        clipA[3].z = (float)((float)(perpZ * narrow) + baseZ);

        /* the far pair, shifted two extents along the ground direction */
        shiftedX = (float)(groundDirection.x * twice + baseX);
        shiftedY = (float)((float)(groundDirection.y * twice) + baseY);
        shiftedZ = (float)((float)(twice * groundDirection.z) + baseZ);

        clipA[0].x = (float)(perpY * narrow + shiftedX);
        clipA[0].y = (float)((float)(perpX * narrow) + shiftedY);
        clipA[0].z = (float)((float)(perpZ * narrow) + shiftedZ);

        clipA[1].x = (float)((float)(perpY * wide) + shiftedX);
        clipA[1].y = (float)((float)(perpX * wide) + shiftedY);
        clipA[1].z = (float)(perpZ * wide + shiftedZ);
    }

    /* ---- clip against the five planes, alternating buffers ---- */
    {
        int kept = WTR_Clip(clipA, clipB, &plane01, offset01, 4);
        kept = WTR_Clip(clipB, clipA, &plane12, offset12, kept);
        kept = WTR_Clip(clipA, clipB, &plane32, offset32, kept);
        kept = WTR_Clip(clipB, clipA, &plane03, offset03, kept);
        survivors = (char)WTR_Clip(clipA, clipB, &backPlane, backOffset, kept);
    }

    /* ---- order the survivors, nearest pair first ---- */
    {
        const float projection0 = (float)(clipB[0].x * groundDirection.x
            + clipB[0].y * groundDirection.y + clipB[0].z * groundDirection.z);
        const float projection1 = (float)(clipB[1].z * groundDirection.z
            + clipB[1].x * groundDirection.x + groundDirection.y * clipB[1].y);
        const float projection2 = (float)(clipB[2].z * groundDirection.z
            + clipB[2].x * groundDirection.x + groundDirection.y * clipB[2].y);
        const float projection3 = (float)(clipB[3].z * groundDirection.z
            + clipB[3].x * groundDirection.x + groundDirection.y * clipB[3].y);
        const float projectionMean =
            (float)((projection2 + projection0 + projection3 + projection1) * 0.25);

        firstCorner = 0;
        if (projection0 < projectionMean && projection1 > projectionMean)
            firstCorner = 1;
        if (projection1 < projectionMean && projection2 > (double)projectionMean)
            firstCorner = 2;
        if (projection2 < (double)projectionMean && projection3 > (double)projectionMean)
            firstCorner = 3;
        if (projection3 < (double)projectionMean && projection0 > projectionMean)
            firstCorner = 0;
    }

    pick = &clipB[firstCorner & 3];
    corners->x = pick->x;
    corners->y = pick->y;
    corners->z = pick->z;

    pick = &clipB[(firstCorner + 1) & 3];
    corners[1].x = pick->x;
    corners[1].y = pick->y;
    corners[1].z = pick->z;

    pick = &clipB[(firstCorner - 2) & 3];
    corners[2].x = pick->x;
    corners[2].y = pick->y;
    corners[2].z = pick->z;

    pick = &clipB[(firstCorner - 1) & 3];
    corners[3].x = pick->x;
    corners[3].y = pick->y;
    corners[3].z = pick->z;

    return survivors & 4;
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

/* Walk a line across the water and write the surface along it.
 *
 * Starting at `start` and stepping by `step`, each vertex samples the grid
 * bilinearly at the position reached so far, the same four corner read the
 * lookup uses, and the sampled displacement and height are added to that
 * position. `keepPrevious` blends the result against whatever the vertex
 * already held, so zero replaces it outright.
 *
 * The 40000 added before scaling keeps the grid indices positive for any
 * position in a world. */
void WTR_Compute_Mesh0_Rasterize_Line(Vec3* verticesOut, tWtrStruct2* map, int sizeLog2, float tiling,
                                      Vec3* start, Vec3* step, int count,
                                      float choppyScale, float heightScale, float keepPrevious)
{
    const int side = 1 << sizeLog2;
    const int mask = side - 1;
    const int rowStride = side + 1;
    const double cellsPerMetre = (double)(unsigned int)side * tiling;
    const double oneMinusKeep = 1.0 - keepPrevious;

    float walkX = start->x;
    float walkY = start->y;
    float walkZ = start->z;
    int i;

    for (i = 0; i < count; i++)
    {
        const double u = (walkX + 40000.0) * cellsPerMetre;
        const double v = (walkY + 40000.0) * cellsPerMetre;

        /* Cell index through the magic double, in double because the values
         * are far too large to survive a trip through float. */
        double roundedU = u - 0.5 + MATH_gd_magicDoubleForRound;
        double roundedV = v - 0.5 + MATH_gd_magicDoubleForRound;
        const int cellX = *(int32_t*)&roundedU;
        const int cellY = *(int32_t*)&roundedV;

        const double fractionX = u - (double)cellX;
        const double fractionY = v - (double)cellY;

        const int corner = (cellX & mask) + (cellY & mask) + ((cellY & mask) << sizeLog2);
        tWtrStruct2* const bottomLeft = &map[corner];
        tWtrStruct2* const bottomRight = &map[corner + 1];
        tWtrStruct2* const topLeft = &map[corner + rowStride];
        tWtrStruct2* const topRight = &map[corner + rowStride + 1];

        const double oneMinusX = 1.0 - fractionX;
        const double weightBottomLeft = oneMinusX * (1.0 - fractionY);
        const double weightBottomRight = (1.0 - fractionY) * fractionX;
        const double weightTopLeft = oneMinusX * fractionY;
        const double weightTopRight = fractionY * fractionX;

        const float sumX = (float)(weightBottomRight * bottomRight->displacementX + weightBottomLeft * bottomLeft->displacementX);
        const float sumY = (float)(bottomRight->displacementY * weightBottomRight + bottomLeft->displacementY * weightBottomLeft);
        const float sumZ = (float)(bottomRight->height * weightBottomRight + bottomLeft->height * weightBottomLeft);

        const float topLeftX = (float)(oneMinusX * fractionY * topLeft->displacementX);
        const float withTopY = (float)(topLeft->displacementY * weightTopLeft + sumY);
        const float withTopZ = (float)(topLeft->height * weightTopLeft + sumZ);

        const float topRightX = (float)(weightTopRight * topRight->displacementX);
        const float topRightY = (float)(topRight->displacementY * weightTopRight);
        const float allZ = (float)(topRight->height * weightTopRight + withTopZ);

        const float displacementX = (float)((topRightX + topLeftX + sumX) * choppyScale);
        const float movedX = (float)(oneMinusKeep * walkX + displacementX);
        const float movedY = (float)(oneMinusKeep * walkY + (topRightY + withTopY) * choppyScale);
        const float keptZ = (float)(walkZ * oneMinusKeep);

        const float previousZ = (float)(keepPrevious * verticesOut[i].z);
        const float previousY = (float)(verticesOut[i].y * keepPrevious);

        verticesOut[i].x = (float)(keepPrevious * verticesOut[i].x + movedX);
        verticesOut[i].y = previousY + movedY;
        verticesOut[i].z = (float)(previousZ + keptZ + allZ * heightScale);

        walkX = walkX + step->x;
        walkY = walkY + step->y;
        walkZ = walkZ + step->z;
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

/* Shade one row of the surface, and produce its normals and reflection
 * vectors.
 *
 * For each vertex the surface normal is the cross product of the edge to the
 * next vertex along the row and the edge to the vertex on the next row, which
 * is why `vertices` is indexed a whole row ahead as well as by one. From that:
 *
 *   colorOut       a step along the ramp, chosen by how square on the surface
 *                  is to the eye, so it darkens towards grazing angles
 *   reflectionOut  the view direction reflected about the normal
 *   normalsOut     the normal itself, normalised, left alone where the cross
 *                  product came out as nothing
 *
 * The last vertex of the row gets the previous one's colour, since it has no
 * next vertex to form an edge with.
 *
 * Two oddities are kept as they were, because they are what the surface is
 * built from today: the across edge has its x and y scaled by `acrossScale`
 * while its z is not, and the normal's z term is formed from the single
 * precision copy of the scaled y while the x term uses the double. */
void WTR_DirectComputeColors(Vec3* vertices, Vec3* cameraPosition, Vec3* normalsOut, Vec3* reflectionOut,
                             int* colorOut, int rowLength, int* colorRamp, float acrossScale)
{
    const int shadedCount = rowLength - 1;
    int i;

    if (shadedCount <= 0)
    {
        colorOut[0] = colorOut[-1];
        return;
    }

    for (i = 0; i < shadedCount; i++)
    {
        Vec3* const vertex = &vertices[i];
        Vec3* const nextAlong = &vertices[i + 1];
        Vec3* const nextAcross = &vertices[i + rowLength];

        const float toCameraX = vertex->x - cameraPosition->x;
        const float toCameraY = vertex->y - cameraPosition->y;
        const float toCameraZ = vertex->z - cameraPosition->z;

        const float alongX = nextAlong->x - vertex->x;
        const float alongY = nextAlong->y - vertex->y;
        const float alongZ = nextAlong->z - vertex->z;

        const float acrossZ = nextAcross->z - vertex->z;
        const float scaledAcrossX = (nextAcross->x - vertex->x) * acrossScale;
        const double scaledAcrossY = (nextAcross->y - vertex->y) * acrossScale;
        const float scaledAcrossYSingle = (float)scaledAcrossY;

        const float normalX = (float)(scaledAcrossY * alongZ - acrossZ * alongY);
        const float normalY = (float)(acrossZ * alongX - alongZ * scaledAcrossX);
        const double normalZWide = alongY * scaledAcrossX - alongX * scaledAcrossYSingle;
        const float normalZ = (float)normalZWide;

        const double normalZSquared = normalZWide * normalZ;
        const double normalYSquared = normalY * normalY;
        const double normalXSquared = normalX * normalX;
        const float normalXSquaredSingle = (float)normalXSquared;
        const double normalLengthSquared = normalXSquared + normalYSquared + normalZSquared;

        const double cameraDistanceSquared = toCameraZ * toCameraZ + toCameraY * toCameraY + toCameraX * toCameraX;

        double facing = toCameraZ * normalZ + toCameraY * normalY + toCameraX * normalX;
        float scaleInput;
        float inverseScale;
        double facingScaled, distanceScale;
        float facingSingle, facingSign;
        Vec3 reflection;
        int tmp;

        /* Only the side of the surface turned towards the eye is shaded. */
        if (facing > 0.0)
            facing = 0.0;

        /* One over the square root of the product, from the table rather than
         * a square root, which gives one over the camera distance times the
         * normal length squared. */
        scaleInput = (float)(cameraDistanceSquared * normalLengthSquared * normalLengthSquared);
        tmp = MATH_gl_TableInverseRacine[((*((int32_t*)(&scaleInput))) >> 13) & 0x7FF]
            + (((int)(0xBD800000 - ((*((uint32_t*)(&scaleInput))) & 0x7F800000)) >> 1) & 0x7F800000);
        inverseScale = *(float*)&tmp;

        facingScaled = inverseScale * facing;
        facingSingle = (float)facingScaled;
        distanceScale = inverseScale * normalLengthSquared;

        reflection.x = (float)(normalX * (facingScaled * -2.0) + (float)(toCameraX * distanceScale));
        reflection.y = (float)(facingScaled * -2.0 * normalY) + (float)(toCameraY * distanceScale);
        reflection.z = (float)((float)(facingScaled * -2.0 * normalZ) + distanceScale * toCameraZ);

        /* Plus or minus one, carrying only the sign of the facing term. */
        {
            const float facingAsFloat = (float)facing;

            tmp = ((*((int32_t*)(&facingAsFloat))) & 0xBF800000) | 0x3F800000;
            facingSign = *(float*)&tmp;
        }

        colorOut[i] = colorRamp[((unsigned int)(256.0 - facingSingle * facingSingle * normalLengthSquared
                                                * facingSign * 255.0) & 0x1FF) + 1];
        reflectionOut[i] = reflection;

        if (fabs(normalX) != 0.0 || fabs(normalY) != 0.0 || fabs(normalZ) != 0.0)
        {
            const double inverseLength = 1.0 / sqrt(normalZSquared + normalXSquaredSingle + normalYSquared);

            normalsOut[i].x = (float)(normalX * inverseLength);
            normalsOut[i].y = (float)(normalY * inverseLength);
            normalsOut[i].z = (float)(inverseLength * normalZ);
        }
    }

    colorOut[shadedCount] = colorOut[shadedCount - 1];
}


/* Build the water mesh for this frame and shade it.
 *
 * WTR_Compute4Points gives the quad of the water plane the camera can
 * see: corners 0 and 3 are the near edge, 1 and 2 the far one. The
 * mesh spans that quad with verticallVertexNum rows by
 * horizontalVertexNum columns, the rows spaced by even steps in
 * inverse depth rather than in distance, so they stay evenly spaced
 * on screen as the quad recedes.
 *
 * Each row picks its own level of the concatenated map from how far
 * apart its vertices land in map space: once a row's step spans more
 * than two texels the next coarser level is used, which keeps the
 * sampling near one texel per vertex all the way to the horizon. The
 * row at which each level was last used is kept in lastRowUsingLevel
 * so that the colour pass can repeat a row wherever the level changed.
 *
 * forwardScale and acrossScale size the visible quad along the view
 * direction and across it. Returns 0 when there is no visible water. */
int WTR_Compute_Mesh0(tWaterParamsGlobalStruct* water, Vec3* cameraPosition,
                      Vec3* viewDirection, float forwardScale, float acrossScale)
{
    const double inverseTiling = 1.0 / water->tiling;
    const float waterHeight = water->waterHeight;
    const float scaledWaveHeight = (float)(inverseTiling * water->waveHeight);
    const float scaledChoppy = (float)(inverseTiling * water->choppy);

    Vec3 corners[4];
    Vec3 acrossVector;
    int lastRowUsingLevel[20];

    if (!WTR_Compute4Points(corners, &acrossVector, cameraPosition, viewDirection,
                            waterHeight, 18000.0, forwardScale, acrossScale,
                            scaledWaveHeight + scaledWaveHeight,
                            scaledChoppy + scaledChoppy))
        return 0;

    {
        const unsigned int rowCount = water->verticallVertexNum;
        const int mapSide = 1 << water->waterEngine.waveMapShift;
        const double mapSideAsDouble = (double)mapSide;
        /* the same value kept as a float, which is what the texel step
           below is divided by; the tile clamp uses the double */
        const float mapSideAsFloat = (float)mapSideAsDouble;
        const float inverseRowSpan = (float)(1.0 / (double)(rowCount - 1));

        /* depth along the view direction at the near and far edges */
        const float nearDepth = (float)((corners[0].x - cameraPosition->x) * viewDirection->x
                               + (corners[0].y - cameraPosition->y) * viewDirection->y
                               + (corners[0].z - cameraPosition->z) * viewDirection->z);

        double farDepth = (corners[3].x - cameraPosition->x) * viewDirection->x
                        + (corners[3].y - cameraPosition->y) * viewDirection->y
                        + (corners[3].z - cameraPosition->z) * viewDirection->z;

        /* never let the far edge come closer than one map tile */
        const float minimumFarDepth = (float)(
            1.0 / (mapSideAsDouble * water->tiling) * (inverseRowSpan * nearDepth));

        float inverseDepth;
        float inverseDepthStep;
        float rowNumerator;
        float rowNumeratorStep;
        float texelLimit;
        float texelStep;

        Vec3 nearMidpoint, farMidpoint;
        Vec3* vertices;
        Vec3* uvVectors;
        Vec3* displayVertices;
        unsigned int* colors;
        Vec3* const verticesEnd =
            &water->vertices[rowCount * water->horizontalVertexNum];
        unsigned int columnCount;
        float inverseColumnSpan;
        int rowIndex;

        memset(lastRowUsingLevel, 0, sizeof(lastRowUsingLevel));

        if (farDepth < minimumFarDepth)
            farDepth = minimumFarDepth;

        inverseDepth = (float)(1.0 / nearDepth);
        inverseDepthStep = (float)((1.0 / farDepth - inverseDepth) * inverseRowSpan);
        rowNumeratorStep = (float)(1.0 / farDepth * inverseRowSpan);
        /* zero, but reached through the running inverse depth, so a
           degenerate near edge stays degenerate rather than becoming 0 */
        rowNumerator = (float)(inverseDepth * 0.0 * inverseRowSpan);

        /* one texel step measured across the middle of the quad */
        MATH_BlendVector(&nearMidpoint, corners, &corners[1], 0.5);
        MATH_BlendVector(&farMidpoint, &corners[3], &corners[2], 0.5);
        texelStep = (float)(1.0
            / water->tiling
            / mapSideAsFloat
            / sqrt((farMidpoint.x - nearMidpoint.x) * (farMidpoint.x - nearMidpoint.x)
                 + (farMidpoint.y - nearMidpoint.y) * (farMidpoint.y - nearMidpoint.y)
                 + (farMidpoint.z - nearMidpoint.z) * (farMidpoint.z - nearMidpoint.z)));
        texelLimit = (float)(1.0 - (double)rowCount * texelStep);

        columnCount = water->horizontalVertexNum;
        inverseColumnSpan = (float)(1.0 / (double)(columnCount - 1));
        vertices = water->vertices;
        uvVectors = water->vecsForUVs;
        displayVertices = water->verticesForDisplay;
        colors = water->colors;
        rowIndex = 0;

        while (vertices < verticesEnd)
        {
            /* where this row sits along the quad, perspective correct */
            double alongQuad = rowNumerator / inverseDepth;
            double nearBlend, farBlend;
            Vec3 columnStep;
            double stepLengthSquared;
            unsigned int level;
            tWtrStruct2* levelMap;
            double texelsPerStep;

            if (texelLimit <= alongQuad)
                alongQuad = texelLimit;

            nearBlend = alongQuad;
            if (alongQuad < 0.0)
                nearBlend = 0.0;
            if (nearBlend > 1.0)
                nearBlend = 1.0;

            /* the far edge uses the same value rounded to float first,
               which is what the original stores and reloads */
            farBlend = (float)alongQuad;
            if (!(alongQuad >= 0.0))
                farBlend = 0.0;
            if (farBlend > 1.0)
                farBlend = 1.0;

            nearMidpoint.x = (float)(corners[3].x * nearBlend + (1.0 - nearBlend) * corners[0].x);
            nearMidpoint.y = (float)(corners[0].y * (1.0 - nearBlend) + corners[3].y * nearBlend);
            nearMidpoint.z = (float)(corners[0].z * (1.0 - nearBlend) + corners[3].z * nearBlend);

            farMidpoint.y = (float)(corners[1].y * (1.0 - farBlend) + corners[2].y * farBlend);
            farMidpoint.z = (float)(corners[1].z * (1.0 - farBlend) + corners[2].z * farBlend);

            level = water->waterEngine.waveMapShift;
            levelMap = water->concatenatedMap;

            rowNumerator += rowNumeratorStep;
            inverseDepth += inverseDepthStep;
            texelLimit += texelStep;

            columnStep.x = (float)((corners[1].x * (1.0 - farBlend) + corners[2].x * farBlend
                            - nearMidpoint.x) * inverseColumnSpan);
            columnStep.y = (farMidpoint.y - nearMidpoint.y) * inverseColumnSpan;
            columnStep.z = (farMidpoint.z - nearMidpoint.z) * inverseColumnSpan;

            /* horizontal length of one column step, in map texels */
            stepLengthSquared = columnStep.y * columnStep.y + columnStep.x * columnStep.x;
            texelsPerStep = (double)(1 << level)
                          * MATH_GetSqrt((float)stepLengthSquared)
                          * water->tiling;

            /* drop a level for every doubling past two texels a step */
            while (texelsPerStep > 2.0)
            {
                if (level < 2)
                    break;
                texelsPerStep = texelsPerStep * 0.5;
                levelMap = &levelMap[(2 << level) + 1 + (1 << (2 * level))];
                --level;
            }

            lastRowUsingLevel[level] = rowIndex;

            if (rowIndex)
            {
                WTR_Compute_Mesh0_Rasterize_Line(vertices, levelMap, level, water->tiling,
                                                 &nearMidpoint, &columnStep, columnCount,
                                                 scaledChoppy, scaledWaveHeight, 0.0);

                /* shade the row completed on the previous pass */
                if (water->waterEngine.useCloudShadingforColors)
                {
                    WTR_DirectComputeColors_CloudShading(
                        &vertices[-(int)water->horizontalVertexNum],
                        (int*)&colors[-(int)water->horizontalVertexNum],
                        water->horizontalVertexNum,
                        water->waterEngine.skyColor,
                        water->waterEngine.bottomColor,
                        (float)(1.0 / scaledWaveHeight),
                        water->waterHeight);
                }
                else
                {
                    WTR_DirectComputeColors(
                        &vertices[-(int)water->horizontalVertexNum],
                        cameraPosition,
                        &uvVectors[-(int)water->horizontalVertexNum],
                        &displayVertices[-(int)water->horizontalVertexNum],
                        &colors[-(int)water->horizontalVertexNum],
                        water->horizontalVertexNum,
                        water->waterEngine.colorTable,
                        1.0f);
                }
            }
            else
            {
                /* the first row carries no displacement of its own */
                WTR_Compute_Mesh0_Rasterize_Line(vertices, levelMap, level, water->tiling,
                                                 &nearMidpoint, &columnStep, columnCount,
                                                 0.0, 0.0, 0.0);
            }

            columnCount = water->horizontalVertexNum;
            ++rowIndex;
            vertices += columnCount;
            colors += columnCount;
            uvVectors += columnCount;
            displayVertices += columnCount;
        }

        /* drop the last row by the wave amplitude, so the furthest
           edge sits below the surface and meets the horizon flat */
        {
            Vec3* vertex = &vertices[-(int)water->horizontalVertexNum];
            while (vertex < verticesEnd)
            {
                const double height = vertex->z;
                ++vertex;
                vertex[-1].z = (float)(height - scaledWaveHeight);
            }
        }

        /* shade the first row, left over by the one row lag above */
        if (water->waterEngine.useCloudShadingforColors)
        {
            WTR_DirectComputeColors_CloudShading(
                water->vertices,
                (int*)water->colors,
                water->horizontalVertexNum,
                water->waterEngine.skyColor,
                water->waterEngine.bottomColor,
                (float)(1.0 / scaledWaveHeight),
                water->waterHeight);
        }
        else
        {
            WTR_DirectComputeColors(
                water->vertices,
                cameraPosition,
                water->vecsForUVs,
                water->verticesForDisplay,
                water->colors,
                water->horizontalVertexNum,
                water->waterEngine.colorTable,
                1.0f);
        }

        /* wherever the level changed the row below was never shaded, so
           repeat the colours of the row before it */
        {
            Vec3* rowVertices = water->vertices;
            unsigned int* rowColors = water->colors;
            Vec3* const lastRow = &water->vertices[water->horizontalVertexNum
                                   * (water->verticallVertexNum - 1)];
            const int* levelChange = &lastRowUsingLevel[1];
            int row = 0;

            while (rowVertices < lastRow)
            {
                if (*levelChange == row)
                {
                    if (row)
                        memcpy(rowColors,
                               &rowColors[-(int)water->horizontalVertexNum],
                               4 * water->horizontalVertexNum);
                    ++levelChange;
                }
                ++row;
                rowVertices += water->horizontalVertexNum;
                rowColors += water->horizontalVertexNum;
            }

            /* and the final row, which has no successor to shade it */
            {
                Vec3* const pastLastRow = &lastRow[water->horizontalVertexNum];
                while (rowVertices < pastLastRow)
                {
                    *rowColors = rowColors[-(int)water->horizontalVertexNum];
                    ++rowColors;
                    ++rowVertices;
                }
            }
        }
    }

    return 1;
}

/* Build and draw the water surface for this frame.
 *
 * The projection scale handed to the mesh builder shrinks as the camera rolls
 * away from level: take the horizontal vector across the line of sight, see
 * how much of the camera's own right vector lies out of that direction, and
 * widen the field of view by it. A level camera leaves the scale at one over
 * the tangent of the half angle.
 *
 * Nothing is drawn when the builder reports no surface in view. When there is
 * one, the surface is drawn and then advanced: the elapsed time moves on by
 * the frame time scaled by the wave speed, the spectrum is transformed at the
 * new time, and the result is concatenated ready for the next frame. */
int WTR_Mesh_Do(tWaterParamsGlobalStruct* water)
{
    Vec3* const sight = &gpst_DX_stDD->ct.cam.objectMtx.v3;
    Vec3* const cameraPosition = &gpst_DX_stDD->ct.cam.objectMtx.Pos;
    Vec3* const cameraRight = &gpst_DX_stDD->ct.cam.objectMtx.v1;

    /* The horizontal direction across the line of sight, which is the line of
     * sight crossed with straight up, with the zero terms folded out. */
    const double acrossX = -sight->y;
    const double acrossY = sight->x;
    const double acrossZ = 0.0;
    const double inverseLength = 1.0 / sqrt(acrossZ * acrossZ + acrossY * acrossY + acrossX * acrossX);

    const double unitAcrossX = inverseLength * acrossX;
    const double unitAcrossY = acrossY * inverseLength;
    const double unitAcrossZ = acrossZ * inverseLength;

    /* How much of the camera's right vector lies along that direction, and
     * what is left over once it is taken away. */
    const double alignment = cameraRight->z * unitAcrossZ + cameraRight->y * unitAcrossY + unitAcrossX * cameraRight->x;

    /* The x term goes through single precision on its way out, as it did, so
     * the scale rounds the same; the other two stay wide. */
    const float alongAcrossX = (float)(unitAcrossX * alignment);
    const double leftOverX = alongAcrossX - cameraRight->x;
    const double leftOverY = unitAcrossY * alignment - cameraRight->y;
    const double leftOverZ = unitAcrossZ * alignment - cameraRight->z;

    const float projectionScale = (float)(1.0 /
        ((sqrt(leftOverZ * leftOverZ + leftOverY * leftOverY + leftOverX * leftOverX) + 1.0)
         * tan(gpst_DX_stDD->ct.cam.focale * 0.5)));

    Vec3 corners[4];
    Vec3 centre;
    int built;

    centre.x = alongAcrossX;

    WTR_UpdateColorMode(water, gpst_DX_stDD->ct.cam.objectMtx.Pos.z);

    built = WTR_Compute_Mesh0(water, cameraPosition, sight, projectionScale, 1.0);

    if (!built)
        return built;

    WTR_Compute4Points(corners, &centre, cameraPosition, sight,
                       (float)(water->waterHeight - 100.0), 18000.0, projectionScale, 1.0, 0.0, 0.0);

    D3D_DrawWater_DrawVertexTable(water);

    water->waterEngine.timeElapsed = TIM_gf_dt * water->speed + water->waterEngine.timeElapsed;
    WATERFFT_Turn_Spectre(&water->waterEngine, water->spaceMap, water->spaceMapDX, water->spaceMapDY,
                          water->waterEngine.timeElapsed);
    WTR_Concate(water);

    return 1;
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


/* Smooth the surface normals.
 *
 * Two passes, each averaging every entry with the next one along its row and
 * the one directly below it on the next row. The last entry of each row and
 * the last row are left alone, as they have no neighbour to average with.
 *
 * The averaging is in place, so an entry is blended with neighbours that have
 * already been blended themselves on this pass; walking order is therefore
 * part of the result and is kept as it was. So are the two summation orders,
 * which differ between the passes for the x component only. */
void WTR_WaterBlurNormal(Vec3* normals, int rowLength, int rowCount)
{
    Vec3* const end = &normals[rowLength * (rowCount - 1)];
    Vec3* cursor;

    for (cursor = normals; cursor < end; cursor++)
    {
        Vec3* const rowEnd = &cursor[rowLength - 1];

        for (; cursor < rowEnd; cursor++)
        {
            const double belowPlusCurrent = cursor[rowLength].x + cursor->x;

            cursor->x = (float)((belowPlusCurrent + cursor[1].x) * 0.33333299);
            cursor->y = (float)((cursor[rowLength].y + cursor[1].y + cursor->y) * 0.33333299);
            cursor->z = (float)((cursor[rowLength].z + cursor[1].z + cursor->z) * 0.33333299);
        }
    }

    for (cursor = normals; cursor < end; cursor++)
    {
        Vec3* const rowEnd = &cursor[rowLength - 1];

        for (; cursor < rowEnd; cursor++)
        {
            const float below = cursor[rowLength].x;

            cursor->x = (float)((below + cursor[1].x + cursor->x) * 0.33333299);
            cursor->y = (float)((cursor[rowLength].y + cursor[1].y + cursor->y) * 0.33333299);
            cursor->z = (float)((cursor[rowLength].z + cursor[1].z + cursor->z) * 0.33333299);
        }
    }
}
