#include "../time.h"
#include "../ai.h"

/* The keyword evaluators are the AI language itself: the interpreter
   calls one per node and it returns the node to run next. A few return
   a small integer cast to tAINode* instead, which is a control code the
   interpreter reads rather than a node: 0 yields the frame, 1 stops the
   behaviour, 2 returns from it and 4 returns from the track. */

// Pops one element and hands back its raw value word and type, without
// running the type's own conversion function. The arithmetic and
// comparison keywords work on the raw word, so this is their pop; it
// also hands back the slot the value came from, which the compound
// assignments write their result into.
static int AI_l_PopRawTarget(int16_t* type, char** slot)
{
    int value;

    *type = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack--].type;
    *slot = AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack + 1].ptr;
    value = *(int32_t*)*slot;
    AI_gul_SizeGlobalStack -= 32;
    return value;
}

static int AI_l_PopRawValue(int16_t* type)
{
    char* slot;

    return AI_l_PopRawTarget(type, &slot);
}

// Rounds a raw float word to an int through the x87 magic double, which
// is the same trick AI_GetIntFromStack uses.
static int AI_l_RoundRawToInt(int raw)
{
    double rounded;

    rounded = *(float*)&raw + MATH_gd_magicDoubleForRound;
    return *(int32_t*)&rounded;
}

/* Coerces the FIRST popped operand to the type of the second, and says
   whether the operation is then done in float. The global stack pops in
   reverse, so the second operand popped is the left-hand one in the
   script's source order -- which is why a comparison coerces what it
   popped first, and a compound assignment coerces its right-hand side
   towards its target.

   A type that is neither float (2) nor int (1) is left alone and
   treated as int, which is what the original's fall-through arm came
   to. */
static BOOL AI_l_CoerceToSecondType(int16_t firstType, int16_t secondType, int* firstValue)
{
    if (secondType == firstType)
        return secondType == 2;

    if (secondType == 2)
    {
        *(float*)firstValue = (float)*firstValue;
        return TRUE;
    }
    if (secondType == 1)
        *firstValue = AI_l_RoundRawToInt(*firstValue);
    return FALSE;
}

// A binary operator promotes to float when either side is one, and
// converts the other side in place.
static BOOL AI_l_PromoteToFloat(int16_t leftType, int16_t rightType, int* left, int* right)
{
    if (leftType == rightType)
        return leftType == 2;

    if (leftType == 2)
    {
        *(float*)right = (float)*right;
        return TRUE;
    }
    if (rightType == 2)
    {
        *(float*)left = (float)*left;
        return TRUE;
    }
    return FALSE;
}

tAINode* AI_EvalKeyword_Ultra(tAINode* node)
{
    AI_gpst_CurrentUltra = (tGameObjectMain*)AI_GetObjectFromStack();

    if (AI_gpst_CurrentUltra == (tGameObjectMain*)WOR_gst_Universe.univAIInstance)
    {
        AI_gpst_CurrentInstanceUltra = WOR_gst_Universe.univAIInstance;
        AI_gpst_CurrentUltra = NULL;
    }
    else if (AI_gpst_CurrentUltra->extended)
    {
        AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentUltra->extended->AIInstance;
    }
    else
    {
        AI_gpst_CurrentInstanceUltra = NULL;
    }

    return node + 1;
}

tAINode* AI_EvalKeyword_ObjEqual(tAINode* node)
{
    tGameObjectMain* gao1;
    tGameObjectMain* gao2;

    gao2 = (tGameObjectMain*)AI_GetObjectFromStack();
    gao1 = (tGameObjectMain*)AI_GetObjectFromStack();

    AI_EvalType_GetGAO(&gao2, &gao2);
    AI_EvalType_GetGAO(&gao1, &gao1);

    AI_PutIntToStack(gao2 == gao1);
    return node + 1;
}

tAINode* AI_EvalKeyword_ObjDifferent(tAINode* node)
{
    tGameObjectMain* gao1;
    tGameObjectMain* gao2;

    gao2 = (tGameObjectMain*)AI_GetObjectFromStack();
    gao1 = (tGameObjectMain*)AI_GetObjectFromStack();

    AI_EvalType_GetGAO(&gao2, &gao2);
    AI_EvalType_GetGAO(&gao1, &gao1);

    AI_PutIntToStack(gao2 != gao1);
    return node + 1;
}

tAINode* AI_EvalKeyword_ObjNot(tAINode* node)
{
    tGameObjectMain* gao;

    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalType_GetGAO(&gao, &gao);
    AI_PutIntToStack(gao == NULL);
    return node + 1;
}

tAINode* AI_EvalKeyword_Return(tAINode* node)
{
    *AI_gppst_CurrentJumpNode = NULL;
    return (tAINode*)2;
}

tAINode* AI_EvalKeyword_ReturnTrack(tAINode* node)
{
    *AI_gppst_CurrentJumpNode = NULL;
    return (tAINode*)4;
}

tAINode* AI_EvalKeyword_Stop(tAINode* node)
{
    *AI_gppst_CurrentJumpNode = NULL;
    return (tAINode*)1;
}

tAINode* AI_EvalKeyword_Push(tAINode* node)
{
    int converted[13];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    AI_PushUserStack(converted, var.type);
    return node + 1;
}

tAINode* AI_EvalKeyword_Pop(tAINode* node)
{
    int converted[13];
    tVarStackElem var;

    AI_PopUserStack(converted, &var);
    AI_PutObjectToStack(converted, var.type);
    return node + 1;
}

// Reads the user stack without unwinding it, at the depth the script
// gives.
tAINode* AI_EvalKeyword_PopSP(tAINode* node)
{
    tVarStackElem* elem;
    int converted[13];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    elem = &AI_gast_UserVarStack[AI_gul_SizeUserVarStack - converted[0] - 1];
    var.type = elem->type;
    var.ptr = elem->ptr;
    AI_gast_Types[var.type].funcptr(var.ptr, converted);
    AI_PutObjectToStack(converted, var.type);
    return node + 1;
}

// Drops that many elements off the user stack.
tAINode* AI_EvalKeyword_PopNow(tAINode* node)
{
    int converted[13];
    tVarStackElem var;
    int i;

    AI_GetVarFromStack(converted, &var);
    for (i = converted[0]; i > 0; --i)
        AI_PopUserStack(converted, &var);
    return node + 1;
}

// Yields the frame, with the node to come back to recorded.
tAINode* AI_EvalKeyword_Meta(tAINode* node)
{
    *AI_gppst_CurrentJumpNode = &node[node->val];
    return (tAINode*)0;
}

// A during block counts down the time left on the current track and
// reports whether it is still running.
int AI_EvalKeyword_During_C()
{
    tAI* instanceUltra;

    instanceUltra = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;

    instanceUltra->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] =
        TIM_gf_dt + instanceUltra->instanceCallBackTimeRemaining[AI_gul_CurrentTrack];
    if (instanceUltra->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] < 0.0)
        return 1;

    instanceUltra->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = 0.0;
    return 0;
}

tAINode* AI_EvalKeyword_During(tAINode* node)
{
    AI_PutIntToStack(AI_EvalKeyword_During_C());
    return node + 1;
}

// The time is stored negative and counts up towards zero.
tAINode* AI_EvalKeyword_DuringInit(tAINode* node)
{
    tAI* instanceUltra;
    float seconds;

    instanceUltra = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    seconds = AI_GetFloatFromStack();
    instanceUltra->instanceCallBackTimeRemaining[AI_gul_CurrentTrack] = -seconds;
    return node + 1;
}

/* The four conditional jumps come in two kinds. JumpFalse and JumpTrue
   put the condition back on the stack, so the value of a && or ||
   chain is left behind for whatever reads it; the SP pair consume it. */
tAINode* AI_EvalKeyword_JumpFalse(tAINode* node)
{
    int condition;

    condition = *(int32_t*)AI_PopStackSlot();
    AI_PutIntToStack(condition);
    if (condition)
        return node + 1;
    return &node[node->val];
}

tAINode* AI_EvalKeyword_JumpTrue(tAINode* node)
{
    int condition;

    condition = *(int32_t*)AI_PopStackSlot();
    AI_PutIntToStack(condition);
    if (condition)
        return &node[node->val];
    return node + 1;
}

tAINode* AI_EvalKeyword_JumpFalseSP(tAINode* node)
{
    if (*(int32_t*)AI_PopStackSlot())
        return node + 1;
    return &node[node->val];
}

tAINode* AI_EvalKeyword_JumpTrueSP(tAINode* node)
{
    if (*(int32_t*)AI_PopStackSlot())
        return &node[node->val];
    return node + 1;
}

// Jumps and leaves a false behind, which is the value of a && chain
// that short-circuited.
tAINode* AI_EvalKeyword_JumpFF(tAINode* node)
{
    int result;

    result = 0;
    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return &node[node->val];
}

tAINode* AI_EvalKeyword_Jump(tAINode* node)
{
    return &node[node->val];
}

/* The six comparisons all coerce the operand they popped first towards
   the type of the one they popped second, then compare in that type.
   The second popped is the left-hand operand in source order. */
tAINode* AI_EvalKeyword_Equal(tAINode* node)
{
    int rightData[13];
    int leftData[13];
    tVarStackElem right;
    tVarStackElem left;
    int result;

    AI_GetVarFromStack(rightData, &right);
    AI_GetVarFromStack(leftData, &left);

    if (AI_l_CoerceToSecondType(right.type, left.type, rightData))
        result = *(float*)leftData == *(float*)rightData;
    else
        result = leftData[0] == rightData[0];

    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return node + 1;
}

tAINode* AI_EvalKeyword_Different(tAINode* node)
{
    int rightData[13];
    int leftData[13];
    tVarStackElem right;
    tVarStackElem left;
    int result;

    AI_GetVarFromStack(rightData, &right);
    AI_GetVarFromStack(leftData, &left);

    if (AI_l_CoerceToSecondType(right.type, left.type, rightData))
        result = *(float*)leftData != *(float*)rightData;
    else
        result = leftData[0] != rightData[0];

    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return node + 1;
}

tAINode* AI_EvalKeyword_GreatEq(tAINode* node)
{
    int rightData[13];
    int leftData[13];
    tVarStackElem right;
    tVarStackElem left;
    int result;

    AI_GetVarFromStack(rightData, &right);
    AI_GetVarFromStack(leftData, &left);

    if (AI_l_CoerceToSecondType(right.type, left.type, rightData))
        result = *(float*)leftData >= *(float*)rightData;
    else
        result = leftData[0] >= rightData[0];

    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return node + 1;
}

tAINode* AI_EvalKeyword_Great(tAINode* node)
{
    int rightData[13];
    int leftData[13];
    tVarStackElem right;
    tVarStackElem left;
    int result;

    AI_GetVarFromStack(rightData, &right);
    AI_GetVarFromStack(leftData, &left);

    if (AI_l_CoerceToSecondType(right.type, left.type, rightData))
        result = *(float*)leftData > *(float*)rightData;
    else
        result = leftData[0] > rightData[0];

    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return node + 1;
}

tAINode* AI_EvalKeyword_LessEq(tAINode* node)
{
    int rightData[13];
    int leftData[13];
    tVarStackElem right;
    tVarStackElem left;
    int result;

    AI_GetVarFromStack(rightData, &right);
    AI_GetVarFromStack(leftData, &left);

    if (AI_l_CoerceToSecondType(right.type, left.type, rightData))
        result = *(float*)leftData <= *(float*)rightData;
    else
        result = leftData[0] <= rightData[0];

    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return node + 1;
}

tAINode* AI_EvalKeyword_Less(tAINode* node)
{
    int rightData[13];
    int leftData[13];
    tVarStackElem right;
    tVarStackElem left;
    int result;

    AI_GetVarFromStack(rightData, &right);
    AI_GetVarFromStack(leftData, &left);

    if (AI_l_CoerceToSecondType(right.type, left.type, rightData))
        result = *(float*)leftData < *(float*)rightData;
    else
        result = leftData[0] < rightData[0];

    AI_PutObjectToStack(&result, AI_gaw_EnumLink[32]);
    return node + 1;
}

/* Not of an int yields 0 or 255, not 0 or 1: the original computes
   (value != 0) - 1 truncated to a byte. The float form yields 1.0f or
   0.0f. Preserved as written. */
tAINode* AI_EvalKeyword_Not(tAINode* node)
{
    int converted[13];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    if (var.type == 2)
    {
        if (*(float*)converted == 0.0f)
            converted[0] = 0x3F800000;              // 1.0f
        else
            converted[0] = 0;
    }
    else
    {
        converted[0] = (uint8_t)((converted[0] != 0) - 1);
    }
    AI_PutObjectToStack(converted, var.type);
    return node + 1;
}

tAINode* AI_EvalKeyword_Neg(tAINode* node)
{
    int converted[13];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    if (var.type == 2)
        *(float*)converted = -*(float*)converted;
    else
        converted[0] = -converted[0];
    AI_PutObjectToStack(converted, var.type);
    return node + 1;
}

tAINode* AI_EvalKeyword_Invert(tAINode* node)
{
    AI_PutIntToStack(~AI_GetIntFromStack());
    return node + 1;
}

tAINode* AI_EvalKeyword_VecNeg(tAINode* node)
{
    Vec3 vec;
    tVarStackElem var;

    AI_GetVarFromStack(&vec, &var);
    MATH_VecInvert(&vec, &vec);
    AI_PutVecToStack(&vec);
    return node + 1;
}

tAINode* AI_EvalKeyword_VecEqual(tAINode* node)
{
    Vec3* right;
    Vec3* left;

    right = (Vec3*)AI_PopStackSlot();
    left = (Vec3*)AI_PopStackSlot();
    AI_PutIntToStack(right->x == left->x && right->y == left->y && right->z == left->z);
    return node + 1;
}

tAINode* AI_EvalKeyword_VecDifferent(tAINode* node)
{
    Vec3* right;
    Vec3* left;

    right = (Vec3*)AI_PopStackSlot();
    left = (Vec3*)AI_PopStackSlot();
    AI_PutIntToStack(!(right->x == left->x && right->y == left->y && right->z == left->z));
    return node + 1;
}

// These two write the result back through the descriptor's own type
// function rather than pushing it.
tAINode* AI_EvalKeyword_IncAffect(tAINode* node)
{
    int converted[13];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    if (var.type == 2)
        *(float*)converted = *(float*)converted + 1.0f;
    else
        ++converted[0];
    AI_gast_Types[var.type].funcptr(converted, var.ptr);
    return node + 1;
}

tAINode* AI_EvalKeyword_DecAffect(tAINode* node)
{
    int converted[13];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    if (var.type == 2)
        *(float*)converted = *(float*)converted - 1.0f;
    else
        --converted[0];
    AI_gast_Types[var.type].funcptr(converted, var.ptr);
    return node + 1;
}


/* The four arithmetic operators promote to float when either operand
   is one. The right-hand operand is the one popped first. */
tAINode* AI_EvalKeyword_Plus(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
        AI_PutFloatToStack(*(float*)&left + *(float*)&right);
    else
        AI_PutIntToStack(left + right);
    return node + 1;
}

tAINode* AI_EvalKeyword_Minus(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
        AI_PutFloatToStack(*(float*)&left - *(float*)&right);
    else
        AI_PutIntToStack(left - right);
    return node + 1;
}

tAINode* AI_EvalKeyword_Mul(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
        AI_PutFloatToStack(*(float*)&left * *(float*)&right);
    else
        AI_PutIntToStack(left * right);
    return node + 1;
}

tAINode* AI_EvalKeyword_Div(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
        AI_PutFloatToStack(*(float*)&left / *(float*)&right);
    else
        AI_PutIntToStack(left / right);
    return node + 1;
}


/* The bitwise operators have no float form, so on float operands they
   round both to ints, operate, and convert the result back. */
tAINode* AI_EvalKeyword_OrOp(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
    {
        AI_PutFloatToStack(
            (float)(AI_l_RoundRawToInt(right) | AI_l_RoundRawToInt(left)));
    }
    else
    {
        AI_PutIntToStack(right | left);
    }
    return node + 1;
}

tAINode* AI_EvalKeyword_AndOp(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
    {
        AI_PutFloatToStack(
            (float)(AI_l_RoundRawToInt(right) & AI_l_RoundRawToInt(left)));
    }
    else
    {
        AI_PutIntToStack(right & left);
    }
    return node + 1;
}

tAINode* AI_EvalKeyword_XorOp(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
    {
        AI_PutFloatToStack(
            (float)(AI_l_RoundRawToInt(right) ^ AI_l_RoundRawToInt(left)));
    }
    else
    {
        AI_PutIntToStack(right ^ left);
    }
    return node + 1;
}

/* The float path shifts the RIGHT operand by the LEFT one, the other
   way round from the int path below it. Preserved as written; it looks
   like a transcription slip in the original rather than intent. */
tAINode* AI_EvalKeyword_SLeft(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;
    int rounded;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
    {
        rounded = AI_l_RoundRawToInt(left);
        AI_PutFloatToStack((float)(AI_l_RoundRawToInt(right) << *(int8_t*)&rounded));
    }
    else
    {
        AI_PutIntToStack(left << right);
    }
    return node + 1;
}

/* The float path shifts the RIGHT operand by the LEFT one, the other
   way round from the int path below it. Preserved as written; it looks
   like a transcription slip in the original rather than intent. */
tAINode* AI_EvalKeyword_SRight(tAINode* node)
{
    int16_t leftType;
    int16_t rightType;
    int left;
    int right;
    int rounded;

    right = AI_l_PopRawValue(&rightType);
    left = AI_l_PopRawValue(&leftType);

    if (AI_l_PromoteToFloat(leftType, rightType, &left, &right))
    {
        rounded = AI_l_RoundRawToInt(left);
        AI_PutFloatToStack((float)(AI_l_RoundRawToInt(right) >> *(int8_t*)&rounded));
    }
    else
    {
        AI_PutIntToStack(left >> right);
    }
    return node + 1;
}


/* The compound assignments pop their right-hand side first and their
   target second, coerce the right-hand side towards the target's type,
   and store the result back through the target's own slot. */
// Plain assignment: coerce and store, with no operation.
tAINode* AI_EvalKeyword_Affect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;

    right = AI_l_PopRawValue(&rightType);
    AI_l_PopRawTarget(&targetType, &targetSlot);

    AI_l_CoerceToSecondType(rightType, targetType, &right);
    *(int32_t*)targetSlot = right;
    return node + 1;
}

tAINode* AI_EvalKeyword_PlusAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
        *(float*)&result = *(float*)&target + *(float*)&right;
    else
        result = target + right;

    *(float*)targetSlot = *(float*)&result;
    return node + 1;
}

tAINode* AI_EvalKeyword_MinusAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
        *(float*)&result = *(float*)&target - *(float*)&right;
    else
        result = target - right;

    *(float*)targetSlot = *(float*)&result;
    return node + 1;
}

tAINode* AI_EvalKeyword_MulAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
        *(float*)&result = *(float*)&target * *(float*)&right;
    else
        result = target * right;

    *(float*)targetSlot = *(float*)&result;
    return node + 1;
}

tAINode* AI_EvalKeyword_DivAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
        *(float*)&result = *(float*)&target / *(float*)&right;
    else
        result = target / right;

    *(float*)targetSlot = *(float*)&result;
    return node + 1;
}

tAINode* AI_EvalKeyword_OrAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
    {
        *(float*)&result =
            (float)(AI_l_RoundRawToInt(right) | AI_l_RoundRawToInt(target));
    }
    else
    {
        result = right | target;
    }

    *(int32_t*)targetSlot = result;
    return node + 1;
}

tAINode* AI_EvalKeyword_AndAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
    {
        *(float*)&result =
            (float)(AI_l_RoundRawToInt(right) & AI_l_RoundRawToInt(target));
    }
    else
    {
        result = right & target;
    }

    *(int32_t*)targetSlot = result;
    return node + 1;
}

tAINode* AI_EvalKeyword_XorAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
    {
        *(float*)&result =
            (float)(AI_l_RoundRawToInt(right) ^ AI_l_RoundRawToInt(target));
    }
    else
    {
        result = right ^ target;
    }

    *(int32_t*)targetSlot = result;
    return node + 1;
}

tAINode* AI_EvalKeyword_SLeftAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;
    int rounded;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
    {
        rounded = AI_l_RoundRawToInt(right);
        *(float*)&result =
            (float)(int)(AI_l_RoundRawToInt(target) << *(int8_t*)&rounded);
    }
    else
    {
        // the count is masked here and not in the plain shift keywords
        result = target << (right & 0xff);
    }

    *(int32_t*)targetSlot = result;
    return node + 1;
}

tAINode* AI_EvalKeyword_SRightAffect(tAINode* node)
{
    int16_t rightType;
    int16_t targetType;
    char* targetSlot;
    int right;
    int target;
    int result;
    int rounded;

    right = AI_l_PopRawValue(&rightType);
    target = AI_l_PopRawTarget(&targetType, &targetSlot);

    if (AI_l_CoerceToSecondType(rightType, targetType, &right))
    {
        rounded = AI_l_RoundRawToInt(right);
        *(float*)&result =
            (float)(AI_l_RoundRawToInt(target) >> *(int8_t*)&rounded);
    }
    else
    {
        // the count is masked here and not in the plain shift keywords
        result = target >> (right & 0xff);
    }

    *(int32_t*)targetSlot = result;
    return node + 1;
}

tAINode* AI_EvalKeyword_VecAffect(tAINode* node)
{
    Vec3* source;
    Vec3* target;
    tVarStackElem var;
    int converted[13];

    source = (Vec3*)AI_PopStackSlot();
    AI_GetVarFromStack(converted, &var);

    target = (Vec3*)var.ptr;
    target->x = source->x;
    target->y = source->y;
    target->z = source->z;
    return node + 1;
}

tAINode* AI_EvalKeyword_VecPlusAffect(tAINode* node)
{
    tVarStackElem var;
    Vec3* right;
    Vec3 target;

    right = (Vec3*)AI_PopStackSlot();
    AI_GetVarFromStack(&target, &var);
    MATH_VecAddVec((Vec3*)var.ptr, &target, right);
    return node + 1;
}

tAINode* AI_EvalKeyword_VecMinusAffect(tAINode* node)
{
    tVarStackElem var;
    Vec3* right;
    Vec3 target;

    right = (Vec3*)AI_PopStackSlot();
    AI_GetVarFromStack(&target, &var);
    MATH_SubstractVec((Vec3*)var.ptr, &target, right);
    return node + 1;
}

tAINode* AI_EvalKeyword_VecMulAffect(tAINode* node)
{
    Vec3* target;
    tVarStackElem var;
    float scalar;
    Vec3 value[2]; // TODO 1 is elég cska a biztonság kedvéért 2 (idában votl rossz)

    scalar = AI_GetFloatFromStack();
    AI_GetVarFromStack(value, &var);
    target = (Vec3*)var.ptr;
    target->x = value[0].x * scalar;
    target->y = value[0].y * scalar;
    target->z = value[0].z * scalar;
    return node + 1;
}

// A vector divided by a scalar is a reciprocal multiply, as the
// original has it.
tAINode* AI_EvalKeyword_VecDivAffect(tAINode* node)
{
    Vec3* target;
    tVarStackElem var;
    float divisor;
    float reciprocal;
    Vec3 value[2];

    divisor = AI_GetFloatFromStack();
    AI_GetVarFromStack(value, &var);
    reciprocal = (float)(1.0 / divisor);
    target = (Vec3*)var.ptr;
    target->x = value[0].x * reciprocal;
    target->y = value[0].y * reciprocal;
    target->z = value[0].z * reciprocal;
    return node + 1;
}

// Both of these work in the left operand's own stack slot and push it
// back, so the subtraction and addition are in place.
tAINode* AI_EvalKeyword_VecMinus(tAINode* node)
{
    Vec3* right;
    Vec3* left;

    right = (Vec3*)AI_PopStackSlot();
    left = (Vec3*)AI_PopStackSlot();
    MATH_SubstractVec(left, left, right);
    AI_PutVecToStack(left);
    return node + 1;
}

tAINode* AI_EvalKeyword_VecPlus(tAINode* node)
{
    Vec3* right;
    Vec3* left;

    right = (Vec3*)AI_PopStackSlot();
    left = (Vec3*)AI_PopStackSlot();
    MATH_VecAddVec(left, left, right);
    AI_PutVecToStack(left);
    return node + 1;
}

/* Either operand may be the vector and the other the scalar, so these
   two look at the second operand's type to find out which way round
   they were given. The scalar is converted from int where it needs to
   be. */
tAINode* AI_EvalKeyword_VecMul(tAINode* node)
{
    Vec3 first[4];
    Vec3 second;
    tVarStackElem firstVar;
    tVarStackElem secondVar;

    AI_GetVarFromStack(first, &firstVar);
    AI_GetVarFromStack(&second, &secondVar);

    if (secondVar.type == 3)
    {
        // second is the vector, so first holds the scalar
        if (firstVar.type != 2)
            first[0].x = (float)(*(int32_t*)&(first[0].x));
        second.x = second.x * first[0].x;
        second.y = second.y * first[0].x;
        second.z = second.z * first[0].x;
        AI_PutVecToStack(&second);
    }
    else
    {
        if (secondVar.type != 2)
            second.x = (float)(*(int32_t*)&(second.x));
        first[0].x = first[0].x * second.x;
        first[0].y = first[0].y * second.x;
        first[0].z = first[0].z * second.x;
        AI_PutVecToStack(first);
    }
    return node + 1;
}

tAINode* AI_EvalKeyword_VecDiv(tAINode* node)
{
    Vec3 first[4];
    Vec3 second;
    tVarStackElem firstVar;
    tVarStackElem secondVar;
    float reciprocal;

    AI_GetVarFromStack(first, &firstVar);
    AI_GetVarFromStack(&second, &secondVar);

    if (secondVar.type == 3)
    {
        if (firstVar.type != 2)
            first[0].x = (float)(*(int32_t*)&(first[0].x));
        reciprocal = (float)(1.0 / first[0].x);
        second.x = second.x * reciprocal;
        second.y = second.y * reciprocal;
        second.z = second.z * reciprocal;
        AI_PutVecToStack(&second);
    }
    else
    {
        if (secondVar.type != 2)
            second.x = (float)(*(int32_t*)&(second.x));
        reciprocal = (float)(1.0 / second.x);
        first[0].x = first[0].x * reciprocal;
        first[0].y = first[0].y * reciprocal;
        first[0].z = first[0].z * reciprocal;
        AI_PutVecToStack(first);
    }
    return node + 1;
}

// The one-way form: the vector is always the first operand.
tAINode* AI_EvalKeyword_VecMul2(tAINode* node)
{
    Vec3 vec[4];
    float scalar[13];
    tVarStackElem vecVar;
    tVarStackElem scalarVar;

    AI_GetVarFromStack(vec, &vecVar);
    AI_GetVarFromStack(scalar, &scalarVar);

    if (scalarVar.type != 2)
        scalar[0] = (float)(*(int32_t*)&(scalar[0]));

    vec[0].x = vec[0].x * scalar[0];
    vec[0].y = vec[0].y * scalar[0];
    vec[0].z = vec[0].z * scalar[0];
    AI_PutVecToStack(vec);
    return node + 1;
}

// The target's buffer is overwritten whole, so the value it was given
// is only popped, never read.
tAINode* AI_EvalKeyword_MsgAffect(tAINode* node)
{
    tMessage source;
    tMessage target;
    tVarStackElem sourceVar;
    tVarStackElem targetVar;

    AI_GetVarFromStack(&source, &sourceVar);
    AI_GetVarFromStack(&target, &targetVar);
    memcpy(&target, &source, sizeof(target));
    AI_gast_Types[targetVar.type].funcptr((void*)&target, (void*)targetVar.ptr);
    return node + 1;
}

// A text reference is eight bytes of the fifty-two the pop can write.
tAINode* AI_EvalKeyword_TextAffect(tAINode* node)
{
    char source[52];
    char target[52];
    tVarStackElem sourceVar;
    tVarStackElem targetVar;

    AI_GetVarFromStack(source, &sourceVar);
    AI_GetVarFromStack(target, &targetVar);
    memcpy(target, source, 8u);
    AI_gast_Types[targetVar.type].funcptr((void*)target, (void*)targetVar.ptr);
    return node + 1;
}

tAINode* AI_EvalKeyword_TextPlus(tAINode* node)
{
    int text[13];
    int offset[13];
    tVarStackElem textVar;
    tVarStackElem offsetVar;

    AI_GetVarFromStack(offset, &offsetVar);
    AI_GetVarFromStack(text, &textVar);
    text[1] += offset[0];                            // tTextGet.idx += ..
    AI_PutObjectToStack(text, textVar.type);
    return node + 1;
}

tOfc_scriptengine_functions AI_gast_Keywords[74] = {

                                { 0x800,      0, AI_dum },
                                { 0x801,      0, AI_dum },
                                { 0x802,      0, AI_dum },
                                { 0x803,      0, AI_dum },
                                { 0x804,      0, AI_dum },
                                { 0x805,      0, AI_EvalKeyword_Meta },
                                { 0x806,      0, AI_EvalKeyword_Ultra },
                                { 0x807,      0, AI_EvalKeyword_During },
                                { 0x808,      0, AI_EvalKeyword_DuringInit },
                                { 0x809,      0, AI_dum },
                                { 0x834,      0, AI_EvalKeyword_Return },
                                { 0x835,      0, AI_dum },
                                { 0x836,      0, AI_EvalKeyword_Push },
                                { 0x837,      0, AI_EvalKeyword_Pop },
                                { 0x838,      0, AI_EvalKeyword_PopSP },
                                { 0x839,      0, AI_EvalKeyword_PopNow },
                                { 0x83A,      0, AI_EvalKeyword_Stop },
                                { 0x83B,      0, AI_EvalKeyword_ReturnTrack },
                                { 0x898,      0, AI_EvalKeyword_Affect },
                                { 0x899,      0, AI_EvalKeyword_PlusAffect },
                                { 0x89A,      0, AI_EvalKeyword_MinusAffect },
                                { 0x89B,      0, AI_EvalKeyword_IncAffect },
                                { 0x89C,      0, AI_EvalKeyword_DecAffect },
                                { 0x89D,      0, AI_EvalKeyword_MulAffect },
                                { 0x89E,      0, AI_EvalKeyword_DivAffect },
                                { 0x89F,      0, AI_EvalKeyword_Not },
                                { 0x8A0,      0, AI_EvalKeyword_Neg },
                                { 0x8A1,      0, AI_EvalKeyword_Equal },
                                { 0x8A2,      0, AI_EvalKeyword_GreatEq },
                                { 0x8A3,      0, AI_EvalKeyword_LessEq },
                                { 0x8A4,      0, AI_EvalKeyword_Great },
                                { 0x8A5,      0, AI_EvalKeyword_Less },
                                { 0x8A6,      0, AI_EvalKeyword_Different },
                                { 0x8A7,      0, AI_EvalKeyword_Minus },
                                { 0x8A8,      0, AI_EvalKeyword_Plus },
                                { 0x8A9,      0, AI_EvalKeyword_Mul },
                                { 0x8AA,      0, AI_EvalKeyword_Div },
                                { 0x8AB,      0, AI_EvalKeyword_OrAffect },
                                { 0x8AC,      0, AI_EvalKeyword_AndAffect },
                                { 0x8AD,      0, AI_EvalKeyword_XorAffect },
                                { 0x8AE,      0, AI_EvalKeyword_OrOp },
                                { 0x8AF,      0, AI_EvalKeyword_AndOp },
                                { 0x8B0,      0, AI_EvalKeyword_XorOp },
                                { 0x8B1,      0, AI_EvalKeyword_SLeft },
                                { 0x8B2,      0, AI_EvalKeyword_SRight },
                                { 0x8B3,      0, AI_EvalKeyword_SLeftAffect },
                                { 0x8B4,      0, AI_EvalKeyword_SRightAffect },
                                { 0x8B5,      0, AI_EvalKeyword_Invert },
                                { 0x8B6,      0, AI_EvalKeyword_ObjEqual },
                                { 0x8B7,      0, AI_EvalKeyword_ObjDifferent },
                                { 0x8B8,      0, AI_EvalKeyword_ObjNot },
                                { 0x8FC,      0, AI_EvalKeyword_Jump },
                                { 0x8FD,      0, AI_EvalKeyword_JumpFalse },
                                { 0x8FE,      0, AI_EvalKeyword_JumpTrue },
                                { 0x900,      0, AI_EvalKeyword_JumpFalseSP },
                                { 0x901,      0, AI_EvalKeyword_JumpTrueSP },
                                { 0x902,      0, AI_EvalKeyword_JumpFF },
                                { 0x903,      0, AI_dum },
                                { 0x960,      0, AI_EvalKeyword_VecAffect },
                                { 0x961,      0, AI_EvalKeyword_VecPlusAffect },
                                { 0x962,      0, AI_EvalKeyword_VecMinusAffect },
                                { 0x963,      0, AI_EvalKeyword_VecMulAffect },
                                { 0x964,      0, AI_EvalKeyword_VecDivAffect },
                                { 0x965,      0, AI_EvalKeyword_VecMinus },
                                { 0x966,      0, AI_EvalKeyword_VecPlus },
                                { 0x967,      0, AI_EvalKeyword_VecMul },
                                { 0x968,      0, AI_EvalKeyword_VecDiv },
                                { 0x969,      0, AI_EvalKeyword_VecNeg },
                                { 0x96A,      0, AI_EvalKeyword_VecEqual },
                                { 0x96B,      0, AI_EvalKeyword_VecDifferent },
                                { 0x96C,      0, AI_EvalKeyword_VecMul2 },
                                { 0x9C4,      0, AI_EvalKeyword_MsgAffect },
                                { 0x9F6,      0, AI_EvalKeyword_TextAffect },
                                { 0x9F7,      0, AI_EvalKeyword_TextPlus }
};
