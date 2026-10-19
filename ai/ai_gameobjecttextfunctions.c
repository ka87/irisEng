#include "../ai.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../strings.h"
#include "../graphicsobjects/geostr.h"
#include "../textget.h"

char* AI_EvalFunc_TEXTToString_C(struct sTextGet* a1)
{
    unsigned int a1a;
    tTextGroupStruct* v3;

    if (!a1)
        return "";
    if (a1->key == -1)
        return "";
    a1a = BIG_ul_SearchKeyToPos(a1->key);
    v3 = (tTextGroupStruct*)LOA_ul_SearchAddress(a1a);
    if (v3 == (tTextGroupStruct*)-1)
        return "";
    if (a1->idx < v3->num)
        return &v3->texts[v3->ids[a1->idx]];
    return "";
}

char* get_a_string_from_globalstack_sub_4D2040()
{
    char* result;
    tTextGet dest[6];
    tVarStackElem a2;

    AI_GetVarFromStack(dest, &a2);
    if (a2.type == AI_gaw_EnumLink[38])
        result = (char*)dest[0].key;
    else
        result = AI_EvalFunc_TEXTToString_C(dest);
    return result;
}

int AI_EvalFunc_OBJ_TextRequest_C(int a1)
{
    int result;

    if (a1)
        result = 0;
    else
        result = STR_sgl_NumberOfFont;
    return result;
}

tAINode* AI_EvalFunc_OBJ_TextRequest(tAINode* a1)
{
    int v1;
    int v2;

    AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_TextRequest_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextGlobalSet(tAINode* a1)
{
    int v1;
    char* v3;

    v3 = (char*)get_a_string_from_globalstack_sub_4D2040();
    v1 = AI_GetIntFromStack();
    STR_GlobalSet(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextGlobalCharSet(tAINode* a1)
{
    int v1;
    int v3;
    char a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_GlobalSetChar(v1, v3, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextGlobalGet(tAINode* a1)
{
    int v1;
    tStrGlobalStruct* v3[5];

    v1 = AI_GetIntFromStack();
    v3[0] = STR_sz_GlobalGet(v1);
    AI_PutObjectToStack(v3, AI_gaw_EnumLink[38]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextReset(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    STR_Reset(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextSet(tAINode* a1)
{
    int v1;
    char* v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = get_a_string_from_globalstack_sub_4D2040();
    v1 = AI_GetIntFromStack();
    STR_SetString(v4, v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextSetExt(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    char* EndPtr;
    Vec3* a5;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a5 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    EndPtr = get_a_string_from_globalstack_sub_4D2040();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetStringExt(v4, v1, v3, EndPtr, a5);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextInsert(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    char* EndPtr;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    EndPtr = get_a_string_from_globalstack_sub_4D2040();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_InsertString(v4, v1, v3, EndPtr);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFloatSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a3;
    char a4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetFloat(v3, v1, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFloatInsert(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    float a4;
    char precision;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    precision = AI_GetIntFromStack();
    a4 = AI_GetFloatFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_InsertFloat(v4, v1, v3, a4, precision);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextPivotSet_C(struct sGameObjectMain* a1, int idx, Vec3* a3)
{
    tGEOSTRStruct* v3;

    v3 = STR_pst_Get(a1);
    if (v3)
        STR_SetPivot(v3, idx, a3);
}

tAINode* AI_EvalFunc_OBJ_TextPivotSet(tAINode* a1)
{
    int v1;
    Vec3* v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPivotSet_C(v4, v1, v3);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextPivotGet_C(struct sGameObjectMain* a1, int idx, Vec3* a3)
{
    tGEOSTRStruct* v3;

    v3 = STR_pst_Get(a1);
    if (v3)
    {
        *a3 = *STR_pst_GetPivot(v3, idx);
    }
    else
    {
        a3->x = 0.0;
        a3->y = 0.0;
        a3->z = 0.0;
    }
}

tAINode* AI_EvalFunc_OBJ_TextPivotGet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    Vec3 a1a;
    tGameObjectMain* v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v3;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPivotGet_C(v5, v1, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextFlagSet_C(struct sGameObjectMain* a1, int idx, __int16 a3)
{
    tGEOSTRStruct* v3;

    v3 = STR_pst_Get(a1);
    if (v3)
        STR_SetFlag(v3, idx, a3);
}

tAINode* AI_EvalFunc_OBJ_TextFlagSet(tAINode* a1)
{
    int v1;
    __int16 v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextFlagSet_C(v4, v1, v3);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_TextFlagGet_C(struct sGameObjectMain* a1, int idx)
{
    int result;
    tGEOSTRStruct* v3;

    v3 = STR_pst_Get(a1);
    if (v3)
        result = STR_ul_GetFlag(v3, idx);
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_OBJ_TextFlagGet(tAINode* a1)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_TextFlagGet_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextColorSet_C(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5)
{
    tGEOSTRStruct* v5;

    v5 = STR_pst_Get(a1);
    if (v5)
        STR_SetColor(v5, a2, a3, a4, a5);
}

tAINode* AI_EvalFunc_OBJ_TextColorSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a5;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextColorSet_C(v4, v1, v3, a4, a5);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextAlphaSet_C(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5)
{
    tGEOSTRStruct* v5;

    v5 = STR_pst_Get(a1);
    if (v5)
        STR_SetAlpha(v5, a2, a3, a4, a5);
}

tAINode* AI_EvalFunc_OBJ_TextAlphaSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a5;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextAlphaSet_C(v4, v1, v3, a4, a5);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextColorSetExt_C(tGameObjectMain* a1, int a2, int a3, int a4, int a5, int a6)
{
    tGEOSTRStruct* v6;

    v6 = STR_pst_Get(a1);
    if (v6)
        STR_SetColorExt(v6, a2, a3, a4, a5, a6);
}

tAINode* AI_EvalFunc_OBJ_TextColorSetExt(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a5;
    int a6;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextColorSetExt_C(v4, v1, v3, a4, a5, a6);
    return a1 + 1;
}

unsigned int AI_EvalFunc_OBJ_TextColorGet_C(tGameObjectMain* a1, int idx, int a3)
{
    tGEOSTRStruct* v4;

    v4 = STR_pst_Get(a1);
    return STR_GetColor(v4, idx, a3);
}

tAINode* AI_EvalFunc_OBJ_TextColorGet(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_TextColorGet_C(v5, v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextPageSet_C(struct sGameObjectMain* a1, int a2, int a3, int a4, int a5, int a6)
{
    tGEOSTRStruct* v6;

    v6 = STR_pst_Get(a1);
    if (v6)
        STR_ChangePage(v6, a2, a3, a4, a5, a6);
}

tAINode* AI_EvalFunc_OBJ_TextPageSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a6;
    int a5;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a6 = AI_GetIntFromStack();
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPageSet_C(v4, v1, v3, a4, a5, a6);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPosSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;
    Vec3* a5;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a5 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetPos(v4, v1, v3, a4, a5);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextSizeSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;
    Vec3* a5;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a5 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetSize(v4, v1, v3, a4, a5);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextDelete(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_DelChar(v4, v1, v3, a4);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextCharSet_C(struct sGameObjectMain* a1, int a2, int a3, int a4)
{
    tGEOSTRStruct* v4;

    v4 = STR_pst_Get(a1);
    if (v4)
        STR_SetChar(v4, a2, a3, a4);
}

tAINode* AI_EvalFunc_OBJ_TextCharSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextCharSet_C(v4, v1, v3, a4);
    return a1 + 1;
}

unsigned int AI_EvalFunc_OBJ_TextCharGet_C(struct sGameObjectMain* a1, int idx, int a3)
{
    unsigned int result;
    tGEOSTRStruct* v4;

    v4 = STR_pst_Get(a1);
    if (v4)
        result = STR_i_GetChar(v4, idx, a3);
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_OBJ_TextCharGet(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_TextCharGet_C(v5, v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_TextLength_C(struct sGameObjectMain* a1, int a2)
{
    int result;
    tGEOSTRStruct* v3;

    v3 = STR_pst_Get(a1);
    if (v3)
        result = STR_l_GetLength(v3, a2);
    else
        result = -1;
    return result;
}

tAINode* AI_EvalFunc_OBJ_TextLength(tAINode* a1)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_TextLength_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_OBJ_TextSubLength_C(tGameObjectMain* a1, int idx, int a3)
{
    int result;
    tGEOSTRStruct* v4;

    v4 = STR_pst_Get(a1);
    if (v4)
        result = STR_l_GetSubLength(v4, idx, a3);
    else
        result = -1;
    return result;
}

tAINode* AI_EvalFunc_OBJ_TextSubLength(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_OBJ_TextSubLength_C(v5, v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void __cdecl AI_EvalFunc_OBJ_TextPosGetMin_C(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5)
{
    Vec3 a5a;

    *a5 = *STR_pst_GetMin(a1, idx, a3, a4, &a5a);
}

tAINode* AI_EvalFunc_OBJ_TextPosGetMin(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a4;
    int idx;
    Vec3 a1a;
    //int v7;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    //v7 = (int)v2;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    idx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPosGetMin_C(v2, idx, a3, a4, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextPosGetMax_c(struct sGameObjectMain* a1, int idx, int a3, int a4, Vec3* a5)
{
    Vec3 a5a;

    *a5 = *STR_pst_GetMax(a1, idx, a3, a4, &a5a);
}

tAINode* AI_EvalFunc_OBJ_TextPosGetMax(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int idx;
    int a4; 
    Vec3 a1a;
    //int v7;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    //v7 = (int)v2;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    idx = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextPosGetMax_c(v2, idx, a3, a4, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextAlign(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a5;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_Align(v4, v1, v3, a4, a5);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_TextSizeGet_C(struct sGameObjectMain* a1, int a2, int a3, int a4, Vec3* a5)
{
    Vec3 a5a;

    *a5 = *STR_pst_GetSize(a1, a2, a3, a4, &a5a);
}

tAINode* AI_EvalFunc_OBJ_TextSizeGet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;
    int a4;
    Vec3 a1a;
    int v7;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = (int)v2;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_TextSizeGet_C(v2, a2, a3, a4, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFrameSet(tAINode* a1)
{
    int v1;
    char v3;
    tGameObjectMain* v4;
    __int16 a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetFrame(v4, v1, v3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFramePosSet(tAINode* a1)
{
    int v1;
    Vec3* v3;
    tGameObjectMain* v4;
    Vec3* a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a4 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = AI_GetIntFromStack();
    STR_SetFramePos(v4, v1, v3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFrameBorderSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetFrameBorderSize(v3, v1, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextFrameColorSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetFrameColor(v4, v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPartWithLine(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetSubStringWithLine(v4, v1, v3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPartWithMark(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetSubStringWithMark(v4, v1, v3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextPart(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetSubString(v4, v1, v3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCountLine(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;
    int a4;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = STR_i_CountLine(v5, v1, v4, a4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCountMark(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;
    int a5;
    int a4;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = STR_i_CountMark(v5, v1, v4, a4, a5);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_OBJ_Text2Text_C(struct sGameObjectMain* a1, int idx, int a3, int a4, tTextGet* a5)
{
    unsigned int a1a;
    tTextGroupStruct* v6;

    if (a5->key != -1)
    {
        a1a = BIG_ul_SearchKeyToPos(a5->key);
        v6 = (tTextGroupStruct*)LOA_ul_SearchAddress(a1a);
        STR_ToString(a1, idx, a3, a4, &v6->texts[v6->ids[a5->idx]]);
    }
}

tAINode* AI_EvalFunc_OBJ_Text2Text(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    tTextGet* a5;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a5 = (tTextGet*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_OBJ_Text2Text_C(v4, v1, v3, a4, a5);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCharFlagSet(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a5;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_SetLetterFlag(v4, v1, v3, a4, a5);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextCharFlagGet(tAINode* a1)
{
    int v1;
    int v2;
    int v4;
    tGameObjectMain* v5;

    v5 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = STR_GetLetterFlag(v5, v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextToUpper(tAINode* a1)
{
    int v1;
    int v3;
    tGameObjectMain* v4;
    int a4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    STR_ToUpper(v4, v1, v3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectAdd(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a4;
    int a3;
    int a2;
    int a5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = STR_i_EffectAdd(v3, a2, a3, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectDel(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    STR_EffectDel(v2, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectSeti(tAINode* a1)
{
    tGameObjectMain* v2;
    int a4;
    int a3;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    STR_EffectSeti(v2, a2, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectSetf(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;
    float a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetFloatFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    STR_EffectSetf(v2, a2, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_TEXTToString(tAINode* a1)
{
    int a1a[4];

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    //TODO FIXME: ezt is kideríteni és megcsinálni normálisan hogy mi az az enumlink[38] és milyen módon kell neki átadni, lehet ez egy stringptr?
    a1a[3] = (int)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a1a[0] = (int)AI_EvalFunc_TEXTToString_C((tTextGet*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutObjectToStack(a1a, AI_gaw_EnumLink[38]);
    return a1 + 1;
}

BOOL AI_EvalFunc_TEXTIsEmpty_C(int* a1)
{
    BOOL result;

    if (*a1)
        result = *a1 == -1; // TODO FIXME: ez milyen típus??
    else
        result = 1;
    return result;
}

tAINode* AI_EvalFunc_TEXTIsEmpty(tAINode* a1)
{
    int v1;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v1 = AI_EvalFunc_TEXTIsEmpty_C((int*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_TEXTCompare_C(void* a1, void* a2)
{
    return strcmp(a1, a2) == 0;
}

tAINode* AI_EvalFunc_TEXTCompare(tAINode* a1)
{
    int v1;
    char* v3;
    char* v4;

    v4 = get_a_string_from_globalstack_sub_4D2040();
    v3 = get_a_string_from_globalstack_sub_4D2040();
    v1 = AI_EvalFunc_TEXTCompare_C(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

size_t AI_EvalFunc_TEXTLength_C(char* Str)
{
    return strlen(Str);
}

tAINode* AI_EvalFunc_TEXTLength(tAINode* a1)
{
    char* v1;
    int v2;

    v1 = get_a_string_from_globalstack_sub_4D2040();
    v2 = AI_EvalFunc_TEXTLength_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_TEXTSetLang(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    TEXT_ChangeLang(v1);
    return a1 + 1;
}

void AI_EvalFunc_TEXTGet_C(int key, int a2, struct sTextGet* a3)
{
    unsigned int pos;

    a3->key = -1;
    a3->idx = 0;
    pos = BIG_ul_SearchKeyToPos(key);
    if (pos == -1)
    {
        MessageBoxA(0, "TextGet Invalid Key", "AI", 0);
    }
    else if (LOA_ul_SearchAddress(pos) != -1)
    {
        a3->key = key;
        a3->idx = a2;
    }
}

tAINode* AI_EvalFunc_TEXTGet(tAINode* a1)
{
    tTextGet a1a;
    int key;
    tTextGet a3;
    int a2;

    a2 = AI_GetIntFromStack();
    key = AI_GetIntFromStack();
    AI_EvalFunc_TEXTGet_C(key, a2, &a3);
    a1a = a3;
    AI_PutObjectToStack(&a1a, AI_gaw_EnumLink[44]);
    return a1 + 1;
}

int AI_EvalFunc_TEXTCharGet_C(char* a1, int a2)
{
    return a1[a2];
}

tAINode* AI_EvalFunc_TEXTCharGet(tAINode* a1)
{
    char* v1;
    int v2;
    int v4;

    v4 = AI_GetIntFromStack();
    v1 = get_a_string_from_globalstack_sub_4D2040();
    v2 = AI_EvalFunc_TEXTCharGet_C(v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_TextEffectGeti(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a3;
    int a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = STR_EffectGeti(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_3DText(tAINode* a1)
{
    char* str;

    str = get_a_string_from_globalstack_sub_4D2040();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    STR_Init3DString(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], str);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextExt(tAINode* a1)
{
    int a3;
    char* str;

    a3 = AI_GetIntFromStack();
    str = get_a_string_from_globalstack_sub_4D2040();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    STR_Init3DStringExt(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], str, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextLangDependent(tAINode* a1)
{
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    STR_3DStringList_Add(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextCenter(tAINode* a1)
{
    int a3;
    char* str;

    a3 = AI_GetIntFromStack();
    str = get_a_string_from_globalstack_sub_4D2040();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    STR_Init3DStringCenter(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], str, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_OBJ_3DTextFromStrObj(tAINode* a1)
{
    int idx;
    int a4;
    tGameObjectMain* a2;

    AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    idx = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    STR_Init3DStringFromSTR(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2, idx, a4);
    return a1 + 1;
}
