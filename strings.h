#pragma once
#include "common.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/geostr.h"

int STR_3DString_Number;
tGameObjectMain* STR_3DString_Obj[64];

typedef struct sStrFontAltKey
{
    int key;
    int key2;
} tStrFontAltKey;

typedef struct sStrGlobalStruct
{
    char str[16];
} tStrGlobalStruct;


tFontData* STR_sgpst_Font;
int STR_sgl_NumberOfFont;
int STR_sgl_MaxNumberOfFont;
tStrFontAltKey STR_FontAltKey[16];
int STR_FontAltKeyNumber;
unsigned int sul_SaveDisplayInfo;
unsigned int sul_SaveDrawMask;
tGameObjectMain* STR_sgpst_GO;
tStrGlobalStruct STR_sgaac_Global[24];
int STR_sgac_CharTable[256];
tGEOSTREffectStruct* STR_spst_CurEffect;

void STR_3DStringList_Reinit();
void STR_3DStringList_Clear();
void STR_3DStringList_Add(struct sGameObjectMain* gao);
void STR_AddFont(int key, struct sEngTexIntermediateStruct* tex);
void STR_Close();
void STR_FontAddAlternativeKey(int key, int altKey);
void STR_FontResolveAlternativeKey();
struct sFontData* STR_pst_GetFontDesc(struct sGEOMAT* mat, int matNum);
struct sGEOSTRStruct* STR_pst_Get(struct sGameObjectMain* gao);
int STR_l_GetLength(struct sGEOSTRStruct* str, unsigned int strIdx);
struct sGEOSTRStringData* STR_pst_GetString(struct sGEOSTRStruct* str, int strIdx);
void STR_GetRectShort(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, Vec3* min, Vec3* max);
int STR_l_GetValidIndex(struct sGEOSTRStruct* str, struct sGEOSTRStringData* sd, int firstChar);
int STR_l_GetValidNumber(struct sGEOSTRStruct* str, struct sGEOSTRStringData* sd, int firstChar, int charNum);
void STR_SetDisplay();
void STR_GlobalSet(int globalIdx, char* src);
void STR_GlobalSetChar(int globalIdx, int charIdx, char character);
tStrGlobalStruct* STR_sz_GlobalGet(int globalIdx);
void STR_Reset(tGameObjectMain* gao);
void STR_SetString(struct sGameObjectMain* gao, int strIdx, char* stringToSet);
void STR_SetStringExt(struct sGameObjectMain* gao, int strIdx, int flags, char* src, Vec3* pivot);
void STR_InsertString(struct sGameObjectMain* gao, int strIdx, int firstChar, char* src);
void STR_SetFloat(struct sGameObjectMain* gao, int strIdx, float value, char precision);
void STR_InsertFloat(tGameObjectMain* gao, int strIdx, int firstChar, float value, char precision);
void STR_SetPivot(struct sGEOSTRStruct* str, int strIdx, Vec3* pivot);
Vec3* STR_pst_GetPivot(struct sGEOSTRStruct* str, int strIdx);
void STR_SetFlag(struct sGEOSTRStruct* str, int strIdx, int16_t flags);
int STR_ul_GetFlag(struct sGEOSTRStruct* str, int strIdx);
void STR_SetColor(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int color);
void STR_SetAlpha(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int alpha);
void STR_SetColorExt(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int color, int mask);
unsigned int STR_GetColor(struct sGEOSTRStruct* str, int strIdx, int charIdx);
void STR_ChangePage(struct sGEOSTRStruct* str, int strIdx, int firstChar, int charNum, int page, int matchPage);
void STR_SetPos(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* pos);
void STR_SetSize(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* size);
void STR_DelChar(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum);
void STR_SetChar(struct sGEOSTRStruct* str, int strIdx, int charIdx, int character);
unsigned int STR_i_GetChar(tGEOSTRStruct* str, int strIdx, int charIdx);
int STR_l_GetSubLength(struct sGEOSTRStruct* str, int strIdx, int lineIdx);
Vec3* STR_pst_GetMin(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* min);
Vec3* STR_pst_GetMax(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* max);
void STR_Align(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int align);
Vec3* STR_pst_GetSize(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, Vec3* size);
void STR_SetFrame(struct sGameObjectMain* gao, int strIdx, char frame, int16_t flags);
void STR_EffectDel(struct sGameObjectMain* gao, int effectId);
void STR_SetFramePos(struct sGameObjectMain* gao, int strIdx, Vec3* pos, Vec3* size);
void STR_SetFrameBorderSize(struct sGameObjectMain* gao, int strIdx, float border);
void STR_SetFrameColor(struct sGameObjectMain* gao, int strIdx, int color);
void STR_SetSubStringWithLine(struct sGameObjectMain* gao, int strIdx, int firstLine, int lineNum);
void STR_SetSubStringWithMark(struct sGameObjectMain* gao, int strIdx, int firstMark, int markNum);
void STR_SetSubString(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum);
int STR_i_CountLine(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum);
int STR_i_CountMark(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int mark);
void STR_ToString(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, unsigned char* dest);
void STR_SetLetterFlag(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum, int flags);
int STR_GetLetterFlag(struct sGameObjectMain* gao, int strIdx, int charIdx);
void STR_ToUpper(struct sGameObjectMain* gao, int strIdx, int firstChar, int charNum);
int STR_i_EffectAdd(void* geoStrOrGao, uint32_t effectType, int stringID, int startIndex, int len);
void STR_EffectSeti(struct sGameObjectMain* gao, int effectId, int field, int value);
int STR_EffectGeti(struct sGameObjectMain* gao, int effectId, int field);
void STR_EffectSetf(struct sGameObjectMain* gao, int effectId, int field, float value);
void STR_ReinitAll();
void STR_ParseString(struct sGEOSTRStruct* geostr, int strIdx, struct sGEOSTRLetter* dest, char* src, int len, float width, float height, int color, int firstChar);
int STR_l_GetStringLength(char* string);
void STR_Init3DString(struct sGameObjectMain* gao, char* str);
void STR_Init3DStringCenter(struct sGameObjectMain* gao, char* str, int maxLetters);
void STR_Init3DStringFromSTR(struct sGameObjectMain* gao, struct sGameObjectMain* source, int strIdx, int maxLetters);
void STR_Init3DStringExtExt(struct sGameObjectMain* gao, char* str, int maxLetters);
void STR_Init3DStringExt(struct sGameObjectMain* gao, char* str, int maxLetters);
