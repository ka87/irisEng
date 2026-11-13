#include "../ai.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../strings.h"
#include "../graphicsobjects/geostr.h"
#include "../textget.h"

// A text reference is a big-file key plus an index into that group's
// string table.
char* AI_EvalFunc_TEXTToString_C(struct sTextGet* textRef)
{
    tTextGroupStruct* group;
    unsigned int pos;

    if (!textRef)
        return "";
    if (textRef->key == -1)
        return "";

    pos = BIG_ul_SearchKeyToPos(textRef->key);
    group = (tTextGroupStruct*)LOA_ul_SearchAddress(pos);
    if (group == (tTextGroupStruct*)-1)
        return "";
    if (textRef->idx < group->num)
        return &group->texts[group->ids[textRef->idx]];
    return "";
}

// A string argument reaches a builtin either as a pointer already or as
// a text reference to resolve. The original's convert buffer was six
// tTextGet, which is 48 bytes and ample for the 8 a text value writes;
// it is the directory's standard 52 now, which is what the widest type
// the pop can write would need if a script ever passed one.
char* AI_GetStringFromStack()
{
    char converted[52];
    tVarStackElem var;

    AI_GetVarFromStack(converted, &var);
    if (var.type == AI_gaw_EnumLink[38])
        return (char*)((tTextGet*)converted)->key;
    return AI_EvalFunc_TEXTToString_C((tTextGet*)converted);
}

// Reports how many fonts are loaded, and nothing at all for a non-zero
// argument.
int AI_EvalFunc_OBJ_TextRequest_C(int which)
{
    if (which)
        return 0;
    return STR_sgl_NumberOfFont;
}

tAINode* AI_EvalFunc_OBJ_TextRequest(tAINode* node)
{
    int which;

    // the builtin takes two arguments and uses only the second
    AI_GetIntFromStack();
    which = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_TextRequest_C(which));
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextGlobalSet(tAINode* node)
{
    char* str;
    int globalIdx;

    str = AI_GetStringFromStack();
    globalIdx = AI_GetIntFromStack();
    STR_GlobalSet(globalIdx, str);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextGlobalCharSet(tAINode* node)
{
    int globalIdx;
    int charIdx;
    char character;

    character = AI_GetIntFromStack();
    charIdx = AI_GetIntFromStack();
    globalIdx = AI_GetIntFromStack();
    STR_GlobalSetChar(globalIdx, charIdx, character);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextGlobalGet(tAINode* node)
{
    tStrGlobalStruct* reference[5];
    int globalIdx;

    globalIdx = AI_GetIntFromStack();
    reference[0] = STR_sz_GlobalGet(globalIdx);
    AI_PutObjectToStack(reference, AI_gaw_EnumLink[38]);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextReset(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    STR_Reset(gaoUltra);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char* str;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    str = AI_GetStringFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetString(gaoUltra, strIdx, str);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextSetExt(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char* str;
    Vec3* pivot;
    int strIdx;
    int flags;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the pivot is read where the stack keeps it rather than copied out
    pivot = (Vec3*)AI_PopStackSlot();
    str = AI_GetStringFromStack();
    flags = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetStringExt(gaoUltra, strIdx, flags, str, pivot);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextInsert(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char* str;
    int strIdx;
    int firstChar;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    str = AI_GetStringFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_InsertString(gaoUltra, strIdx, firstChar, str);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFloatSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float value;
    char precision;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    precision = AI_GetIntFromStack();
    value = AI_GetFloatFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetFloat(gaoUltra, strIdx, value, precision);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFloatInsert(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float value;
    char precision;
    int strIdx;
    int firstChar;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    precision = AI_GetIntFromStack();
    value = AI_GetFloatFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_InsertFloat(gaoUltra, strIdx, firstChar, value, precision);
    return node + 1;
}

void AI_EvalFunc_OBJ_TextPivotSet_C(struct sGameObjectMain* gao, int strIdx, Vec3* pivot)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_SetPivot(str, strIdx, pivot);
}

tAINode* AI_EvalFunc_OBJ_TextPivotSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3* pivot;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the pivot is read where the stack keeps it rather than copied out
    pivot = (Vec3*)AI_PopStackSlot();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPivotSet_C(gaoUltra, strIdx, pivot);
    return node + 1;
}

void AI_EvalFunc_OBJ_TextPivotGet_C(struct sGameObjectMain* gao, int strIdx, Vec3* pivot)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
    {
        *pivot = *STR_pst_GetPivot(str, strIdx);
    }
    else
    {
        pivot->x = 0.0;
        pivot->y = 0.0;
        pivot->z = 0.0;
    }
}

tAINode* AI_EvalFunc_OBJ_TextPivotGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 pivot;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPivotGet_C(gaoUltra, strIdx, &pivot);
    AI_PutVecToStack(&pivot);
    return node + 1;
}

void AI_EvalFunc_OBJ_TextFlagSet_C(struct sGameObjectMain* gao, int strIdx, int16_t flags)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_SetFlag(str, strIdx, flags);
}

tAINode* AI_EvalFunc_OBJ_TextFlagSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int16_t flags;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    flags = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextFlagSet_C(gaoUltra, strIdx, flags);
    return node + 1;
}

int AI_EvalFunc_OBJ_TextFlagGet_C(struct sGameObjectMain* gao, int strIdx)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (!str)
        return 0;
    return STR_ul_GetFlag(str, strIdx);
}

tAINode* AI_EvalFunc_OBJ_TextFlagGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_TextFlagGet_C(gaoUltra, strIdx));
    return node + 1;
}

// The colour and page builtins all address a run of characters: a
// string index, the first character of the run, and how many
// characters it covers.
void AI_EvalFunc_OBJ_TextColorSet_C(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int color)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_SetColor(str, strIdx, firstChar, charNum, color);
}

tAINode* AI_EvalFunc_OBJ_TextColorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int color;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    color = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextColorSet_C(gaoUltra, strIdx, firstChar, charNum, color);
    return node + 1;
}

void AI_EvalFunc_OBJ_TextAlphaSet_C(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int alpha)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_SetAlpha(str, strIdx, firstChar, charNum, alpha);
}

tAINode* AI_EvalFunc_OBJ_TextAlphaSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int alpha;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    alpha = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextAlphaSet_C(gaoUltra, strIdx, firstChar, charNum, alpha);
    return node + 1;
}

// The same, with a mask saying which bits of the colour to take.
void AI_EvalFunc_OBJ_TextColorSetExt_C(tGameObjectMain* gao, int strIdx, int firstChar, int charNum, int color, int mask)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_SetColorExt(str, strIdx, firstChar, charNum, color, mask);
}

tAINode* AI_EvalFunc_OBJ_TextColorSetExt(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int color;
    int mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mask = AI_GetIntFromStack();
    color = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextColorSetExt_C(gaoUltra, strIdx, firstChar, charNum, color, mask);
    return node + 1;
}

// Alone among these, this one passes the string on without checking
// that the object has one; preserved as written.
unsigned int AI_EvalFunc_OBJ_TextColorGet_C(tGameObjectMain* gao, int strIdx, int charIdx)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    return STR_GetColor(str, strIdx, charIdx);
}

tAINode* AI_EvalFunc_OBJ_TextColorGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int charIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the builtin takes three arguments and uses the last two
    AI_GetIntFromStack();
    charIdx = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_TextColorGet_C(gaoUltra, strIdx, charIdx));
    return node + 1;
}

// Moves a run of characters onto another page. matchPage picks which
// characters move: negative means all of them, 0 to 255 only those on
// that page, and 256 to 511 only those NOT on page matchPage - 256.
void AI_EvalFunc_OBJ_TextPageSet_C(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int page, int matchPage)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_ChangePage(str, strIdx, firstChar, charNum, page, matchPage);
}

tAINode* AI_EvalFunc_OBJ_TextPageSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int page;
    int matchPage;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    matchPage = AI_GetIntFromStack();
    page = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPageSet_C(gaoUltra, strIdx, firstChar, charNum, page, matchPage);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3* pos;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the position is read where the stack keeps it rather than copied out
    pos = (Vec3*)AI_PopStackSlot();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetPos(gaoUltra, strIdx, firstChar, charNum, pos);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextSizeSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3* size;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the size is read where the stack keeps it rather than copied out
    size = (Vec3*)AI_PopStackSlot();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetSize(gaoUltra, strIdx, firstChar, charNum, size);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextDelete(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_DelChar(gaoUltra, strIdx, firstChar, charNum);
    return node + 1;
}

// The character value carries its own flags in the bits above it, so
// the engine keeps only the low eighteen.
void AI_EvalFunc_OBJ_TextCharSet_C(struct sGameObjectMain* gao, int strIdx, int charIdx, int character)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (str)
        STR_SetChar(str, strIdx, charIdx, character);
}

tAINode* AI_EvalFunc_OBJ_TextCharSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int charIdx;
    int character;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    character = AI_GetIntFromStack();
    charIdx = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextCharSet_C(gaoUltra, strIdx, charIdx, character);
    return node + 1;
}

unsigned int AI_EvalFunc_OBJ_TextCharGet_C(struct sGameObjectMain* gao, int strIdx, int charIdx)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (!str)
        return 0;
    return STR_i_GetChar(str, strIdx, charIdx);
}

tAINode* AI_EvalFunc_OBJ_TextCharGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int charIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charIdx = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_TextCharGet_C(gaoUltra, strIdx, charIdx));
    return node + 1;
}

int AI_EvalFunc_OBJ_TextLength_C(struct sGameObjectMain* gao, int strIdx)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (!str)
        return -1;
    return STR_l_GetLength(str, strIdx);
}

tAINode* AI_EvalFunc_OBJ_TextLength(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_TextLength_C(gaoUltra, strIdx));
    return node + 1;
}

// The length of one line, or of the longest line for a negative index.
int AI_EvalFunc_OBJ_TextSubLength_C(tGameObjectMain* gao, int strIdx, int lineIdx)
{
    tGEOSTRStruct* str;

    str = STR_pst_Get(gao);
    if (!str)
        return -1;
    return STR_l_GetSubLength(str, strIdx, lineIdx);
}

tAINode* AI_EvalFunc_OBJ_TextSubLength(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int lineIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    lineIdx = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_OBJ_TextSubLength_C(gaoUltra, strIdx, lineIdx));
    return node + 1;
}

// STR_pst_GetMin fills the caller's vector and hands it back, so the
// local it is given here is written and then copied over itself.
void __cdecl AI_EvalFunc_OBJ_TextPosGetMin_C(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* min)
{
    Vec3 scratch;

    *min = *STR_pst_GetMin(gao, strIdx, firstChar, charNum, &scratch);
}

tAINode* AI_EvalFunc_OBJ_TextPosGetMin(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 min;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPosGetMin_C(gaoUltra, strIdx, firstChar, charNum, &min);
    AI_PutVecToStack(&min);
    return node + 1;
}

void AI_EvalFunc_OBJ_TextPosGetMax_c(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* max)
{
    Vec3 scratch;

    *max = *STR_pst_GetMax(gao, strIdx, firstChar, charNum, &scratch);
}

tAINode* AI_EvalFunc_OBJ_TextPosGetMax(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 max;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPosGetMax_c(gaoUltra, strIdx, firstChar, charNum, &max);
    AI_PutVecToStack(&max);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextAlign(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int align;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    align = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_Align(gaoUltra, strIdx, firstChar, charNum, align);
    return node + 1;
}

void AI_EvalFunc_OBJ_TextSizeGet_C(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* size)
{
    Vec3 scratch;

    *size = *STR_pst_GetSize(gao, strIdx, firstChar, charNum, &scratch);
}

tAINode* AI_EvalFunc_OBJ_TextSizeGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3 size;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextSizeGet_C(gaoUltra, strIdx, firstChar, charNum, &size);
    AI_PutVecToStack(&size);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFrameSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    char frame;
    int16_t flags;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    flags = AI_GetIntFromStack();
    frame = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetFrame(gaoUltra, strIdx, frame, flags);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFramePosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    Vec3* size;
    Vec3* pos;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // both vectors are read where the stack keeps them rather than
    // copied out
    size = (Vec3*)AI_PopStackSlot();
    pos = (Vec3*)AI_PopStackSlot();
    strIdx = AI_GetIntFromStack();
    STR_SetFramePos(gaoUltra, strIdx, pos, size);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFrameBorderSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float border;
    int strIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    border = AI_GetFloatFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetFrameBorderSize(gaoUltra, strIdx, border);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFrameColorSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int color;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    color = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetFrameColor(gaoUltra, strIdx, color);
    return node + 1;
}

// The three "part" builtins narrow the string to a run, counted in
// lines, in marks, or in characters.
tAINode* AI_EvalFunc_OBJ_TextPartWithLine(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstLine;
    int lineNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    lineNum = AI_GetIntFromStack();
    firstLine = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetSubStringWithLine(gaoUltra, strIdx, firstLine, lineNum);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPartWithMark(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstMark;
    int markNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    markNum = AI_GetIntFromStack();
    firstMark = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetSubStringWithMark(gaoUltra, strIdx, firstMark, markNum);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPart(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetSubString(gaoUltra, strIdx, firstChar, charNum);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCountLine(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(STR_i_CountLine(gaoUltra, strIdx, firstChar, charNum));
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCountMark(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int mark;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    mark = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(STR_i_CountMark(gaoUltra, strIdx, firstChar, charNum, mark));
    return node + 1;
}

// Writes a text reference's string into a run of the object's own.
void AI_EvalFunc_OBJ_Text2Text_C(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, tTextGet* textRef)
{
    tTextGroupStruct* group;
    unsigned int pos;

    if (textRef->key == -1)
        return;

    pos = BIG_ul_SearchKeyToPos(textRef->key);
    group = (tTextGroupStruct*)LOA_ul_SearchAddress(pos);
    STR_ToString(gao, strIdx, firstChar, charNum, &group->texts[group->ids[textRef->idx]]);
}

tAINode* AI_EvalFunc_OBJ_Text2Text(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tTextGet* textRef;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the text reference is read where the stack keeps it rather than
    // copied out
    textRef = (tTextGet*)AI_PopStackSlot();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_Text2Text_C(gaoUltra, strIdx, firstChar, charNum, textRef);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCharFlagSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;
    int flags;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    flags = AI_GetIntFromStack();
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_SetLetterFlag(gaoUltra, strIdx, firstChar, charNum, flags);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCharFlagGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int charIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charIdx = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    AI_PutIntToStack(STR_GetLetterFlag(gaoUltra, strIdx, charIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextToUpper(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int strIdx;
    int firstChar;
    int charNum;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    charNum = AI_GetIntFromStack();
    firstChar = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    STR_ToUpper(gaoUltra, strIdx, firstChar, charNum);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectAdd(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    uint32_t effectType;
    int strIdx;
    int startIndex;
    int len;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    len = AI_GetIntFromStack();
    startIndex = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    effectType = AI_GetIntFromStack();
    AI_PutIntToStack(STR_i_EffectAdd(gaoUltra, effectType, strIdx, startIndex, len));
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectDel(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int effectId;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    effectId = AI_GetIntFromStack();
    STR_EffectDel(gaoUltra, effectId);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectSeti(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int effectId;
    int field;
    int value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    value = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    effectId = AI_GetIntFromStack();
    STR_EffectSeti(gaoUltra, effectId, field, value);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectSetf(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int effectId;
    int field;
    float value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    value = AI_GetFloatFromStack();
    field = AI_GetIntFromStack();
    effectId = AI_GetIntFromStack();
    STR_EffectSetf(gaoUltra, effectId, field, value);
    return node + 1;
}

tAINode* AI_EvalFunc_TEXTToString(tAINode* node)
{
    int reference[4];
    char* slot;

    slot = AI_PopStackSlot();
    //TODO FIXME: ezt is kideríteni és megcsinálni normálisan hogy mi az az enumlink[38] és milyen módon kell neki átadni, lehet ez egy stringptr?
    reference[3] = (int)slot;
    reference[0] = (int)AI_EvalFunc_TEXTToString_C((tTextGet*)slot);
    AI_PutObjectToStack(reference, AI_gaw_EnumLink[38]);
    return node + 1;
}

BOOL AI_EvalFunc_TEXTIsEmpty_C(int* textRef)
{
    if (!*textRef)
        return TRUE;
    return *textRef == -1; // TODO FIXME: ez milyen típus??
}

tAINode* AI_EvalFunc_TEXTIsEmpty(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_TEXTIsEmpty_C((int*)AI_PopStackSlot()));
    return node + 1;
}

BOOL AI_EvalFunc_TEXTCompare_C(void* left, void* right)
{
    return strcmp(left, right) == 0;
}

tAINode* AI_EvalFunc_TEXTCompare(tAINode* node)
{
    char* right;
    char* left;

    right = AI_GetStringFromStack();
    left = AI_GetStringFromStack();
    AI_PutIntToStack(AI_EvalFunc_TEXTCompare_C(left, right));
    return node + 1;
}

size_t AI_EvalFunc_TEXTLength_C(char* Str)
{
    return strlen(Str);
}

tAINode* AI_EvalFunc_TEXTLength(tAINode* node)
{
    char* str;

    str = AI_GetStringFromStack();
    AI_PutIntToStack(AI_EvalFunc_TEXTLength_C(str));
    return node + 1;
}

tAINode* AI_EvalFunc_TEXTSetLang(tAINode* node)
{
    int lang;

    lang = AI_GetIntFromStack();
    TEXT_ChangeLang(lang);
    return node + 1;
}

// Makes a text reference, leaving it empty when the key is not in the
// big files or its group is not loaded.
void AI_EvalFunc_TEXTGet_C(int key, int idx, struct sTextGet* textRef)
{
    unsigned int pos;

    textRef->key = -1;
    textRef->idx = 0;

    pos = BIG_ul_SearchKeyToPos(key);
    if (pos == -1)
    {
        MessageBoxA(0, "TextGet Invalid Key", "AI", 0);
    }
    else if (LOA_ul_SearchAddress(pos) != -1)
    {
        textRef->key = key;
        textRef->idx = idx;
    }
}

tAINode* AI_EvalFunc_TEXTGet(tAINode* node)
{
    tTextGet textRef;
    int key;
    int idx;

    idx = AI_GetIntFromStack();
    key = AI_GetIntFromStack();
    AI_EvalFunc_TEXTGet_C(key, idx, &textRef);
    AI_PutObjectToStack(&textRef, AI_gaw_EnumLink[44]);
    return node + 1;
}

int AI_EvalFunc_TEXTCharGet_C(char* str, int charIdx)
{
    return str[charIdx];
}

tAINode* AI_EvalFunc_TEXTCharGet(tAINode* node)
{
    char* str;
    int charIdx;

    charIdx = AI_GetIntFromStack();
    str = AI_GetStringFromStack();
    AI_PutIntToStack(AI_EvalFunc_TEXTCharGet_C(str, charIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectGeti(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int effectId;
    int field;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    field = AI_GetIntFromStack();
    effectId = AI_GetIntFromStack();
    AI_PutIntToStack(STR_EffectGeti(gaoUltra, effectId, field));
    return node + 1;
}

// The 3D text builtins take their object off the stack rather than
// working on the current one.
tAINode* AI_EvalFunc_OBJ_3DText(tAINode* node)
{
    char* str;

    str = AI_GetStringFromStack();
    STR_Init3DString((tGameObjectMain*)AI_GetObjectFromStack(), str);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextExt(tAINode* node)
{
    char* str;
    int maxLetters;

    maxLetters = AI_GetIntFromStack();
    str = AI_GetStringFromStack();
    STR_Init3DStringExt((tGameObjectMain*)AI_GetObjectFromStack(), str, maxLetters);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextLangDependent(tAINode* node)
{
    STR_3DStringList_Add((tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextCenter(tAINode* node)
{
    char* str;
    int maxLetters;

    maxLetters = AI_GetIntFromStack();
    str = AI_GetStringFromStack();
    STR_Init3DStringCenter((tGameObjectMain*)AI_GetObjectFromStack(), str, maxLetters);
    return node + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextFromStrObj(tAINode* node)
{
    tGameObjectMain* source;
    int strIdx;
    int maxLetters;

    // the builtin takes five arguments and drops the last one pushed
    AI_GetIntFromStack();
    maxLetters = AI_GetIntFromStack();
    strIdx = AI_GetIntFromStack();
    source = (tGameObjectMain*)AI_GetObjectFromStack();
    STR_Init3DStringFromSTR((tGameObjectMain*)AI_GetObjectFromStack(), source, strIdx, maxLetters);
    return node + 1;
}
