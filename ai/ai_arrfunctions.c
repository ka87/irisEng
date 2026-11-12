#include "ai_arrfunctions.h"
#include "ai_gameobjectfunctions.h"

// A bit array is a run of 32-bit words: the index picks the word and
// its low five bits pick the bit. The original masked the index before
// shifting it -- (bit & 0xFFFFFFE0) >> 5 -- which clears exactly the
// bits the shift discards, so the mask does nothing.
#define AI_BIT_WORD(bit) ((bit) >> 5)
#define AI_BIT_MASK(bit) (1 << ((bit) & 0x1F))

// An array slot holds either nothing or -1 for an object that is gone.
static BOOL AI_l_ArrayObjectAlive(tGameObjectMain* gao)
{
    return gao && gao != (tGameObjectMain*)-1;
}

// The index of the array's object nearest to a point, or 0 when the
// array holds none.
int AI_EvalFunc_ARRObjNearestOfPoint_C(struct sGameObjectMain** objs, int num, Vec3* pos)
{
    tGameObjectMain* gao;
    Vec3 toObj;
    float distance;
    float nearest;
    int nearestIdx;
    int i;

    nearestIdx = 0;
    nearest = FLT_MAX;
    for (i = num - 1; i >= 0; --i)
    {
        gao = objs[i];
        if (!AI_l_ArrayObjectAlive(gao))
            continue;

        MATH_SubstractVec(&toObj, pos, &gao->matrices->Pos);
        distance = MATH_VecLen(&toObj);
        if (distance < nearest)
        {
            nearest = distance;
            nearestIdx = i;
        }
    }
    return nearestIdx;
}

tAINode* AI_EvalFunc_ARRObjNearestOfPoint(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    Vec3* pos;
    int num;

    // the position is read where the stack keeps it rather than copied out
    pos = (Vec3*)AI_PopStackSlot();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRObjNearestOfPoint_C((tGameObjectMain**)var.ptr, num, pos));
    return node + 1;
}

void* AI_EvalFunc_ARRObjCopy_C(void* dst, void* src, int num)
{
    return memcpy(dst, src, 4 * num);
}

tAINode* AI_EvalFunc_ARRObjCopy(tAINode* node)
{
    char srcConverted[52];
    char dstConverted[52];
    tVarStackElem srcVar;
    tVarStackElem dstVar;
    int num;

    num = AI_GetIntFromStack();
    AI_GetVarFromStack(srcConverted, &srcVar);
    AI_GetVarFromStack(dstConverted, &dstVar);
    AI_EvalFunc_ARRObjCopy_C(dstVar.ptr, srcVar.ptr, num);
    return node + 1;
}

// Keeps the objects whose flags match and closes the gaps. Note that
// both of these filter the SOURCE array in place: the destination the
// script passes is popped and never written.
tAINode* AI_EvalFunc_ARRObjCopyIdFlags(tAINode* node)
{
    char srcConverted[52];
    char dstConverted[52];
    tVarStackElem srcVar;
    tVarStackElem dstVar;
    tGameObjectMain* gao;
    int flagMustHave;
    int flagMustNothave;
    int num;
    int kept;
    int i;

    flagMustHave = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(srcConverted, &srcVar);
    AI_GetVarFromStack(dstConverted, &dstVar);

    kept = 0;
    for (i = 0; i < num; ++i)
    {
        gao = *(tGameObjectMain**)&srcVar.ptr[4 * i];
        if (gao && (flagMustHave & gao->flags) != 0 && (flagMustNothave & gao->flags) == 0)
            *(tGameObjectMain**)&srcVar.ptr[4 * kept++] = gao;
    }
    AI_PutIntToStack(kept);
    return node + 1;
}

// The same over the second flag word's upper byte, which is where the
// callback bits live.
tAINode* AI_EvalFunc_ARRObjCopyCB(tAINode* node)
{
    char srcConverted[52];
    char dstConverted[52];
    tVarStackElem srcVar;
    tVarStackElem dstVar;
    tGameObjectMain* gao;
    int flagMustHave;
    int flagMustNothave;
    int num;
    int kept;
    int i;

    flagMustHave = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(srcConverted, &srcVar);
    AI_GetVarFromStack(dstConverted, &dstVar);

    kept = 0;
    for (i = 0; i < num; ++i)
    {
        gao = *(tGameObjectMain**)&srcVar.ptr[4 * i];
        if (gao
            && (flagMustHave & gao->flags2 & 0xFF00) != 0
            && (flagMustNothave & gao->flags2 & 0xFF00) == 0)
        {
            *(tGameObjectMain**)&srcVar.ptr[4 * kept++] = gao;
        }
    }
    AI_PutIntToStack(kept);
    return node + 1;
}

// The index of the object most nearly in the given direction from the
// given point. A non-zero falloff trades angle against distance.
int AI_EvalFunc_ARRObjBestSight_C(struct sGameObjectMain** objs, int num, Vec3* from, Vec3* direction, float falloff)
{
    tGameObjectMain* gao;
    Vec3 sight;
    Vec3 toObj;
    float distance;
    float score;
    float bestScore;
    int bestIdx;
    int i;

    distance = 0.0f;
    MATH_NormalizeVector(&sight, direction);
    bestIdx = num - 1;
    bestScore = -FLT_MAX;
    for (i = num - 1; i >= 0; --i)
    {
        gao = objs[i];
        if (!AI_l_ArrayObjectAlive(gao))
            continue;

        MATH_SubstractVec(&toObj, &gao->matrices->Pos, from);

        // an object at the point itself has no direction to score
        if (fabs(toObj.x) == 0.0 && fabs(toObj.y) == 0.0 && fabs(toObj.z) == 0.0)
            continue;

        if (falloff != 0.0)
            distance = MATH_VecLen(&toObj);
        MATH_NormalizeVector(&toObj, &toObj);
        score = MATH_DotProduct(&sight, &toObj);
        if (falloff != 0.0)
            score = score - distance / falloff;

        if (score > bestScore)
        {
            bestScore = score;
            bestIdx = i;
        }
    }
    return bestIdx;
}

tAINode* AI_EvalFunc_ARRObjBestSight(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    Vec3 from;
    Vec3 direction;
    float falloff;
    int num;

    falloff = AI_GetFloatFromStack();
    AI_GetVecFromStack(&direction);
    AI_GetVecFromStack(&from);
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRObjBestSight_C((tGameObjectMain**)var.ptr, num,
                                                   &from, &direction, falloff));
    return node + 1;
}

int AI_EvalFunc_ARRObjSearch_C(struct sGameObjectMain** objs, int num, struct sGameObjectMain* gao)
{
    int i;

    for (i = 0; i < num; ++i)
    {
        if (objs[i] == gao)
            return i;
    }
    return -1;
}

tAINode* AI_EvalFunc_ARRObjSearch(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    tGameObjectMain* gao;
    int num;

    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRObjSearch_C((tGameObjectMain**)var.ptr, num, gao));
    return node + 1;
}

void AI_EvalFunc_ARRBitSet_C(int* bits, int bit)
{
    bits[AI_BIT_WORD(bit)] |= AI_BIT_MASK(bit);
}

tAINode* AI_EvalFunc_ARRBitSet(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int bit;

    bit = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_EvalFunc_ARRBitSet_C((int*)var.ptr, bit);
    return node + 1;
}

void AI_EvalFunc_ARRBitClear_C(int* bits, int bit)
{
    bits[AI_BIT_WORD(bit)] &= ~AI_BIT_MASK(bit);
}

tAINode* AI_EvalFunc_ARRBitClear(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int bit;

    bit = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_EvalFunc_ARRBitClear_C((int*)var.ptr, bit);
    return node + 1;
}

// Sets every bit from first to last inclusive. Within one word that is
// one mask; across words the two end words are partial and the ones
// between them are filled.
tAINode* AI_EvalFunc_ARRBitSetRange(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int* firstWord;
    int* lastWord;
    int first;
    int last;

    last = AI_GetIntFromStack();
    first = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);

    firstWord = (int*)&var.ptr[4 * AI_BIT_WORD(first)];
    lastWord = (int*)&var.ptr[4 * AI_BIT_WORD(last)];

    if (firstWord == lastWord)
    {
        *firstWord |= (2 << (last & 0x1F)) - AI_BIT_MASK(first);
    }
    else
    {
        *firstWord++ |= -AI_BIT_MASK(first);
        *lastWord |= (2 << (last & 0x1F)) - 1;
        while (firstWord != lastWord)
            *firstWord++ = -1;
    }
    return node + 1;
}

tAINode* AI_EvalFunc_ARRBitClearRange(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int* firstWord;
    int* lastWord;
    int first;
    int last;

    last = AI_GetIntFromStack();
    first = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);

    firstWord = (int*)&var.ptr[4 * AI_BIT_WORD(first)];
    lastWord = (int*)&var.ptr[4 * AI_BIT_WORD(last)];

    if (firstWord == lastWord)
    {
        *firstWord &= ~((2 << (last & 0x1F)) - AI_BIT_MASK(first));
    }
    else
    {
        *firstWord++ &= AI_BIT_MASK(first) - 1;
        *lastWord &= ~((2 << (last & 0x1F)) - 1);
        while (firstWord != lastWord)
            *firstWord++ = 0;
    }
    return node + 1;
}

BOOL AI_EvalFunc_ARRBitTest_C(int* bits, int bit)
{
    return (AI_BIT_MASK(bit) & bits[AI_BIT_WORD(bit)]) != 0;
}

tAINode* AI_EvalFunc_ARRBitTest(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int bit;

    bit = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRBitTest_C((int*)var.ptr, bit));
    return node + 1;
}

void AI_EvalFunc_ARRBitChange_C(int* bits, int bit)
{
    int* word;

    word = &bits[AI_BIT_WORD(bit)];
    if ((AI_BIT_MASK(bit) & *word) != 0)
        *word &= ~AI_BIT_MASK(bit);
    else
        *word |= AI_BIT_MASK(bit);
}

tAINode* AI_EvalFunc_ARRBitChange(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int bit;

    bit = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_EvalFunc_ARRBitChange_C((int*)var.ptr, bit);
    return node + 1;
}

// Whether every bit the script lists is in the state it asks for, and
// at least one was listed. A listed index equal to skip is ignored.
BOOL AI_EvalFunc_ARRBitAnd_C(int* bits, int* bitIdx, int* expected, int num, int skip)
{
    int tested;
    int i;

    tested = 0;
    for (i = 0; i < num; ++i)
    {
        if (bitIdx[i] == skip)
            continue;

        tested = 1;
        if ((AI_BIT_MASK(bitIdx[i]) & bits[AI_BIT_WORD(bitIdx[i])]) != 0)
        {
            if (!expected[i])
                return FALSE;
        }
        else if (expected[i])
        {
            return FALSE;
        }
    }
    return tested != 0;
}

tAINode* AI_EvalFunc_ARRBitAnd(tAINode* node)
{
    char bitsConverted[52];
    char idxConverted[52];
    char expectedConverted[52];
    tVarStackElem bitsVar;
    tVarStackElem idxVar;
    tVarStackElem expectedVar;
    int num;
    int skip;

    skip = AI_GetIntFromStack();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(expectedConverted, &expectedVar);
    AI_GetVarFromStack(idxConverted, &idxVar);
    AI_GetVarFromStack(bitsConverted, &bitsVar);
    AI_PutIntToStack(AI_EvalFunc_ARRBitAnd_C((int*)bitsVar.ptr, (int*)idxVar.ptr,
                                             (int*)expectedVar.ptr, num, skip));
    return node + 1;
}

// Whether any bit the script lists is in the state it asks for.
BOOL AI_EvalFunc_ARRBitOr_C(int* bits, int* bitIdx, int* expected, int num, int skip)
{
    int i;

    for (i = 0; i < num; ++i)
    {
        if (bitIdx[i] == skip)
            continue;

        if ((AI_BIT_MASK(bitIdx[i]) & bits[AI_BIT_WORD(bitIdx[i])]) != 0)
        {
            if (expected[i])
                return TRUE;
        }
        else if (!expected[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

tAINode* AI_EvalFunc_ARRBitOr(tAINode* node)
{
    char bitsConverted[52];
    char idxConverted[52];
    char expectedConverted[52];
    tVarStackElem bitsVar;
    tVarStackElem idxVar;
    tVarStackElem expectedVar;
    int num;
    int skip;

    skip = AI_GetIntFromStack();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(expectedConverted, &expectedVar);
    AI_GetVarFromStack(idxConverted, &idxVar);
    AI_GetVarFromStack(bitsConverted, &bitsVar);
    AI_PutIntToStack(AI_EvalFunc_ARRBitOr_C((int*)bitsVar.ptr, (int*)idxVar.ptr,
                                            (int*)expectedVar.ptr, num, skip));
    return node + 1;
}

// Tests up to four bits in one call. A zero slot is unused, and an
// index above 10000 asks for the bit to be CLEAR, the bit being the
// index less 10000. An index of exactly 10000 passes either way, since
// neither arm's comparison is true for it.
BOOL AI_EvalFunc_ARRBit4Test_C(int* bits, int bit1, int bit2, int bit3, int bit4)
{
    int asked[4];
    int bit;
    int isSet;
    int i;

    asked[0] = bit1;
    asked[1] = bit2;
    asked[2] = bit3;
    asked[3] = bit4;

    for (i = 0; i < 4; ++i)
    {
        if (!asked[i])
            continue;

        bit = asked[i];
        if (bit > 10000)
            bit -= 10000;

        isSet = AI_BIT_MASK(bit) & bits[AI_BIT_WORD(bit)];
        if (isSet && asked[i] > 10000)
            return FALSE;
        if (!isSet && asked[i] < 10000)
            return FALSE;
    }
    return TRUE;
}

tAINode* AI_EvalFunc_ARRBit4Test(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int bit1;
    int bit2;
    int bit3;
    int bit4;

    bit4 = AI_GetIntFromStack();
    bit3 = AI_GetIntFromStack();
    bit2 = AI_GetIntFromStack();
    bit1 = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRBit4Test_C((int*)var.ptr, bit1, bit2, bit3, bit4));
    return node + 1;
}

// ARRObjBestSight again, scored from each object's bounding-volume
// centre rather than its pivot, and normalising the caller's direction
// in place rather than into a local.
int AI_EvalFunc_ARRObjBestSightBVCenter_C(tGameObjectMain** objs, int num, Vec3* from, Vec3* direction, float falloff)
{
    tGameObjectMain* gao;
    Vec3 center;
    Vec3 toObj;
    float distance;
    float score;
    float bestScore;
    int bestIdx;
    int i;

    distance = 0.0f;
    MATH_NormalizeVector(direction, direction);
    bestIdx = num - 1;
    bestScore = -FLT_MAX;
    for (i = num - 1; i >= 0; --i)
    {
        gao = objs[i];
        if (!AI_l_ArrayObjectAlive(gao))
            continue;

        OBJ_BV_ComputeCenter(gao, &center);
        MATH_SubstractVec(&toObj, &center, from);
        if (falloff != 0.0)
            distance = MATH_VecLen(&toObj);
        MATH_NormalizeVector(&toObj, &toObj);
        score = MATH_DotProduct(direction, &toObj);
        if (falloff != 0.0)
            score = score - distance / falloff;

        if (score > bestScore)
        {
            bestScore = score;
            bestIdx = i;
        }
    }
    return bestIdx;
}

tAINode* AI_EvalFunc_ARRObjBestSightBVCenter(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    Vec3 from;
    Vec3 direction;
    float falloff;
    int num;

    falloff = AI_GetFloatFromStack();
    AI_GetVecFromStack(&direction);
    AI_GetVecFromStack(&from);
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRObjBestSightBVCenter_C((tGameObjectMain**)var.ptr, num,
                                                           &from, &direction, falloff));
    return node + 1;
}

void AI_EvalFunc_ARRGlobalIntSet_C(int* bits)
{
    ARR_gpi_Global = bits;
}

tAINode* AI_EvalFunc_ARRGlobalIntSet(tAINode* node)
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    AI_EvalFunc_ARRGlobalIntSet_C((int*)var.ptr);
    return node + 1;
}

void AI_EvalFunc_ARRGlobalIntFlagSet_C(int bit)
{
    AI_EvalFunc_ARRBitSet_C(ARR_gpi_Global, bit);
}

tAINode* AI_EvalFunc_ARRGlobalIntFlagSet(tAINode* node)
{
    int bit;

    bit = AI_GetIntFromStack();
    AI_EvalFunc_ARRGlobalIntFlagSet_C(bit);
    return node + 1;
}

void AI_EvalFunc_ARRGlobalIntFlagClear_C(int bit)
{
    AI_EvalFunc_ARRBitClear_C(ARR_gpi_Global, bit);
}

tAINode* AI_EvalFunc_ARRGlobalIntFlagClear(tAINode* node)
{
    int bit;

    bit = AI_GetIntFromStack();
    AI_EvalFunc_ARRGlobalIntFlagClear_C(bit);
    return node + 1;
}

BOOL AI_EvalFunc_ARRGlobalIntFlagTest_C(int bit)
{
    return AI_EvalFunc_ARRBitTest_C(ARR_gpi_Global, bit);
}

tAINode* AI_EvalFunc_ARRGlobalIntFlagTest(tAINode* node)
{
    int bit;

    bit = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_ARRGlobalIntFlagTest_C(bit));
    return node + 1;
}

// Partitions the array so the objects that pass the flag test come
// first, and returns how many did. A failing object changes places with
// the last one still in range, so nothing is copied twice.
//
// A null slot advances neither the cursor nor the range, so an array
// with a hole in it does not terminate. Preserved as written.
int AI_EvalFunc_ARRSortByFlags_C(struct sGameObjectMain** objs, int num, int flagMustHave, int flagMustNothave, int whatToTest)
{
    tGameObjectMain* swap;
    tGameObjectMain* gao;
    int i;

    i = 0;
    while (i < num)
    {
        gao = objs[i];
        if (!gao)
            continue;

        if (OBJ_b_test_flag(gao, flagMustHave, flagMustNothave, whatToTest))
        {
            ++i;
        }
        else
        {
            --num;
            swap = objs[i];
            objs[i] = objs[num];
            objs[num] = swap;
        }
    }
    return num;
}

tAINode* AI_EvalFunc_ARRSortByFlags(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int num;
    int flagMustHave;
    int flagMustNothave;
    int whatToTest;

    whatToTest = AI_GetIntFromStack();
    flagMustNothave = AI_GetIntFromStack();
    flagMustHave = AI_GetIntFromStack();
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRSortByFlags_C((tGameObjectMain**)var.ptr, num,
                                                  flagMustHave, flagMustNothave, whatToTest));
    return node + 1;
}

// Two passes over the array. The first finds the object closest to the
// sight direction; the second widens that angle by the tolerance and
// returns the NEAREST object inside the widened cone, so a photo
// subject in front wins over one merely better aligned.
int AI_EvalFunc_ARRObjBestSightInfoPhoto_C(struct sGameObjectMain** objs, int num, Vec3* from, Vec3* direction, float toleranceDeg)
{
    tGameObjectMain* gao;
    Vec3 pivot;
    Vec3 toObj;
    double widened;
    float bestDot;
    float coneDot;
    float distance;
    float nearest;
    float dot;
    int nearestIdx;
    int i;

    MATH_NormalizeVector(direction, direction);
    nearestIdx = num - 1;

    bestDot = -FLT_MAX;
    for (i = num - 1; i >= 0; --i)
    {
        gao = objs[i];
        if (!AI_l_ArrayObjectAlive(gao))
            continue;

        AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(gao, &pivot);
        MATH_SubstractVec(&toObj, &pivot, from);
        MATH_NormalizeVector(&toObj, &toObj);
        dot = MATH_DotProduct(direction, &toObj);
        if (dot > bestDot)
            bestDot = dot;
    }

    // no tolerance given means five degrees
    if (toleranceDeg == 0.0)
        widened = (float)acos(bestDot) + 0.087266468; // 5 degrees in radians
    else
        widened = toleranceDeg * 3.1415927 / 180.0 + (float)acos(bestDot);
    coneDot = (float)cos((float)widened);

    nearest = FLT_MAX;
    for (i = num - 1; i >= 0; --i)
    {
        gao = objs[i];
        if (!AI_l_ArrayObjectAlive(gao))
            continue;

        AI_EvalFunc_OBJ_InfoPhotoPivotGet_C(gao, &pivot);
        MATH_SubstractVec(&toObj, &pivot, from);
        distance = MATH_VecLen(&toObj);
        MATH_NormalizeVector(&toObj, &toObj);
        dot = MATH_DotProduct(direction, &toObj);
        if (dot > coneDot && distance < nearest)
        {
            nearest = distance;
            nearestIdx = i;
        }
    }
    return nearestIdx;
}

tAINode* AI_EvalFunc_ARRObjBestSightInfoPhoto(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    Vec3 from;
    Vec3 direction;
    float toleranceDeg;
    int num;

    toleranceDeg = AI_GetFloatFromStack();
    AI_GetVecFromStack(&direction);
    AI_GetVecFromStack(&from);
    num = AI_GetIntFromStack();
    AI_GetVarFromStack(converted, &var);
    AI_PutIntToStack(AI_EvalFunc_ARRObjBestSightInfoPhoto_C((tGameObjectMain**)var.ptr, num,
                                                            &from, &direction, toleranceDeg));
    return node + 1;
}
