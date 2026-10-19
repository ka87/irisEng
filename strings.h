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
void STR_3DStringList_Add(struct sGameObjectMain* a1);
void STR_AddFont(int key, struct sEngTexIntermediateStruct* a2);
void STR_Close();
void STR_FontAddAlternativeKey(int a1, int a2);
void STR_FontResolveAlternativeKey();
struct sFontData* STR_pst_GetFontDesc(struct sGEOMAT* a1, int matNum);
struct sGEOSTRStruct* STR_pst_Get(struct sGameObjectMain* a1);
int STR_l_GetLength(struct sGEOSTRStruct* a1, unsigned int a2);
struct sGEOSTRStringData* STR_pst_GetString(struct sGEOSTRStruct* a1, int idx);
void STR_GetRectShort(struct sGEOSTRStruct* a1, int idx, int a3, int a4, Vec3* min, Vec3* max);
int STR_l_GetValidIndex(struct sGEOSTRStruct* a1, struct sGEOSTRStringData* a2, int a3);
int STR_l_GetValidNumber(struct sGEOSTRStruct* a1, struct sGEOSTRStringData* a2, int a3, int a4);
void STR_SetDisplay();
void STR_GlobalSet(int a1, char* a2);
void STR_GlobalSetChar(int a1, int a2, char a3);
tStrGlobalStruct* STR_sz_GlobalGet(int a1);
void STR_Reset(tGameObjectMain* a1);
void STR_SetString(struct sGameObjectMain* a1, int idx, char* stringToSet);
void STR_SetStringExt(struct sGameObjectMain* a1, int a2, int a3, char* EndPtr, Vec3* a5);
void STR_InsertString(struct sGameObjectMain* a1, int idx, int a3, char* EndPtr);
void STR_SetFloat(struct sGameObjectMain* a1, int idx, float a3, char a4);
void STR_InsertFloat(tGameObjectMain* a1, int idx, int a3, float a4, char precision);
void STR_SetPivot(struct sGEOSTRStruct* a1, int idx, Vec3* a3);
Vec3* STR_pst_GetPivot(struct sGEOSTRStruct* a1, int idx);
void STR_SetFlag(struct sGEOSTRStruct* a1, int idx, __int16 a3);
int STR_ul_GetFlag(struct sGEOSTRStruct* a1, int idx);
void STR_SetColor(struct sGEOSTRStruct* a1, int idx, int a3, int a4, int a5);
void STR_SetAlpha(struct sGEOSTRStruct* a1, int idx, int a3, int a4, int a5);
void STR_SetColorExt(struct sGEOSTRStruct* a1, int idx, int a3, int a4, int a5, int a6);
unsigned int STR_GetColor(struct sGEOSTRStruct* a1, int idx, int a3);
void STR_ChangePage(struct sGEOSTRStruct* a1, int a2, int a3, int a4, int a5, int a6);
void STR_SetPos(struct sGameObjectMain* a1, int a2, int a3, int a4, Vec3* a5);
void STR_SetSize(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5);
void STR_DelChar(struct sGameObjectMain* a1, int a2, int a3, int a4);
void STR_SetChar(struct sGEOSTRStruct* a1, int a2, int a3, int a4);
unsigned int STR_i_GetChar(tGEOSTRStruct* a1, int idx, int a3);
int STR_l_GetSubLength(struct sGEOSTRStruct* a1, int idx, int a3);
Vec3* STR_pst_GetMin(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5);
Vec3* STR_pst_GetMax(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5);
void STR_Align(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5);
Vec3* STR_pst_GetSize(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5);
void STR_SetFrame(struct sGameObjectMain* a1, int a2, char a3, __int16 a4);
void STR_EffectDel(struct sGameObjectMain* a1, int a2);
void STR_SetFramePos(struct sGameObjectMain* a1, int idx, Vec3* a3, Vec3* a4);
void STR_SetFrameBorderSize(struct sGameObjectMain* a1, int a2, float a3);
void STR_SetFrameColor(struct sGameObjectMain* a1, int a2, int a3);
void STR_SetSubStringWithLine(struct sGameObjectMain* a1, int a2, int a3, int a4);
void STR_SetSubStringWithMark(struct sGameObjectMain* a1, int a2, int a3, int a4);
void STR_SetSubString(struct sGameObjectMain* a1, int a2, int a3, int a4);
int STR_i_CountLine(struct sGameObjectMain* a1, int idx, int a3, int a4);
int STR_i_CountMark(struct sGameObjectMain* a1, int idx, int a3, int a4, int a5);
void STR_ToString(struct sGameObjectMain* a1, int idx, int a3, int a4, unsigned char* a5);
void STR_SetLetterFlag(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5);
int STR_GetLetterFlag(struct sGameObjectMain* a1, int idx, int a3);
void STR_ToUpper(struct sGameObjectMain* a1, int a2, int a3, int a4);
int STR_i_EffectAdd(void* geoStrOrGao, uint32_t effectType, int stringID, int startIndex, int len);
void STR_EffectSeti(struct sGameObjectMain* a1, int a2, int a3, int a4);
int STR_EffectGeti(struct sGameObjectMain* a1, int a2, int a3);
void STR_EffectSetf(struct sGameObjectMain* a1, int a2, int a3, float a4);
void STR_ReinitAll();
void STR_ParseString(struct sGEOSTRStruct* geostr, int idx, struct sGEOSTRLetter* dest, char* EndPtr, int len, float width, float height, int color, int a9);
int STR_l_GetStringLength(char* string);
void STR_Init3DString(struct sGameObjectMain* a1, char* str);
void STR_3DStringList_Add(struct sGameObjectMain* a1);
void STR_Init3DStringCenter(struct sGameObjectMain* a1, char* EndPtr, int a3);
void STR_Init3DStringFromSTR(struct sGameObjectMain* a1, struct sGameObjectMain* a2, int idx, int a4);
void STR_Init3DStringExtExt(struct sGameObjectMain* a1, char* str, int a3);
void STR_Init3DStringExt(struct sGameObjectMain* a1, char* EndPtr, int a3);