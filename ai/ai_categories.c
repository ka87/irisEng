#include "../ai.h"

tAINode* AI_EvalCateg_Type(tAINode* a1)
{
    AI_PutObjectToStack(a1, a1->val2);
    return a1 + 1;
}

tAINode* AI_EvalCateg_Keyword(tAINode* a1)
{
    return AI_gast_Keywords[a1->val2].funcptr(a1);
}

tAINode* AI_EvalCateg_Function(tAINode* a1)
{
    return AI_gast_Functions[a1->val2].funcptr(a1);
}

tAINode* AI_EvalCateg_Field(tAINode* a1)
{
    return AI_gast_Fields[a1->val2].funcptr(a1);
}

tAINode* AI_EvalCateg_LocalVar(tAINode* a1)
{
    char* v2;
    tVarStackElem a1a;

    v2 = &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + a1->val];
    a1a.type = a1->val2;
    a1a.ptr = v2;
    AI_EvalVar(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalCateg_LocalVarArray(tAINode* a1)
{
    AI_EvalVarArray(a1, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_LocalVarArray2(tAINode* a1)
{
    AI_EvalVarArray2(a1, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_LocalVarArray3(tAINode* a1)
{
    AI_EvalVarArray3(a1, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_GlobalVar(tAINode* a1)
{
    char* v2;
    tVarStackElem a1a;
    tAI* v4;
    tAI* v5;

    v4 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    v5 = v4;
    v2 = &v4->instanceVarDataCurrent[a1->val];
    a1a.type = a1->val2;
    a1a.ptr = v2;
    AI_EvalVar(&a1a);
    return a1 + 1;
}

tAINode* AI_EvalCateg_GlobalVarArray(tAINode* a1)
{
    tAI* v2;

    v2 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_EvalVarArray(a1, &v2->instanceVarDataCurrent[a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_GlobalVarArray2(tAINode* a1)
{
    tAI* v2;

    v2 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_EvalVarArray2(a1, &v2->instanceVarDataCurrent[a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_GlobalVarArray3(tAINode* a1)
{
    tAI* v2;

    v2 = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_EvalVarArray3(a1, &v2->instanceVarDataCurrent[a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_Text(tAINode* a1)
{
    int a1a[2];

    a1a[0] = a1->val;
    a1a[1] = a1->val2;
    AI_PutObjectToStack(a1a, AI_gaw_EnumLink[44]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_InitLocalVarArray(tAINode* a1)
{
    AI_InitVarArray(a1, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + a1->val]);
    return a1 + 1;
}

tAINode* AI_EvalCateg_EventParam(tAINode* a1)
{
    AI_PutObjectToStack(&EVE_gpst_CurrentParam->privateData, a1->val2);
    return a1 + 1;
}

tOfc_scriptengine_functions AI_gast_Categs[16] = {
    { 0x1, 0, AI_EvalCateg_Type },
    { 0x2, 0, AI_EvalCateg_Keyword },
    { 0x3, 0, AI_EvalCateg_Function },
    { 0x4, 0, AI_EvalCateg_Field },
    { 0x5, 0, AI_EvalCateg_LocalVar },
    { 0x6, 0, AI_EvalCateg_LocalVarArray },
    { 0x7, 0, AI_EvalCateg_LocalVarArray2 },
    { 0x8, 0, AI_EvalCateg_LocalVarArray3 },
    { 0x14, 0, AI_EvalCateg_GlobalVar },
    { 0x15, 0, AI_EvalCateg_GlobalVarArray },
    { 0x16, 0, AI_EvalCateg_GlobalVarArray2 },
    { 0x17, 0, AI_EvalCateg_GlobalVarArray3 },
    { 0x18, 0, AI_EvalCateg_Text },
    { 0x19, 0, AI_EvalCateg_InitLocalVarArray },
    { 0x1A, 0, AI_EvalCateg_EventParam },
    { 0x1F, 0, AI_dum }
};