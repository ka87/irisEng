#include "../ai.h"

tAINode* AI_EvalCateg_Type(tAINode* node)
{
    AI_PutObjectToStack(node, node->val2);
    return node + 1;
}

tAINode* AI_EvalCateg_Keyword(tAINode* node)
{
    return AI_gast_Keywords[node->val2].funcptr(node);
}

tAINode* AI_EvalCateg_Function(tAINode* node)
{
    return AI_gast_Functions[node->val2].funcptr(node);
}

tAINode* AI_EvalCateg_Field(tAINode* node)
{
    return AI_gast_Fields[node->val2].funcptr(node);
}

tAINode* AI_EvalCateg_LocalVar(tAINode* node)
{
    tVarStackElem var;

    var.type = node->val2;
    var.ptr = &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + node->val];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalCateg_LocalVarArray(tAINode* node)
{
    AI_EvalVarArray(node, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_LocalVarArray2(tAINode* node)
{
    AI_EvalVarArray2(node, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_LocalVarArray3(tAINode* node)
{
    AI_EvalVarArray3(node, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_GlobalVar(tAINode* node)
{
    tVarStackElem var;
    tAI* instanceUltra;

    instanceUltra = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;

    var.type = node->val2;
    var.ptr = &instanceUltra->instanceVarDataCurrent[node->val];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalCateg_GlobalVarArray(tAINode* node)
{
    tAI* instanceUltra;

    instanceUltra = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;

    AI_EvalVarArray(node, &instanceUltra->instanceVarDataCurrent[node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_GlobalVarArray2(tAINode* node)
{
    tAI* instanceUltra;

    instanceUltra = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;

    AI_EvalVarArray2(node, &instanceUltra->instanceVarDataCurrent[node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_GlobalVarArray3(tAINode* node)
{
    tAI* instanceUltra;

    instanceUltra = AI_gpst_CurrentInstanceUltra;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;

    AI_EvalVarArray3(node, &instanceUltra->instanceVarDataCurrent[node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_Text(tAINode* node)
{
    int reference[2];

    reference[0] = node->val;
    reference[1] = node->val2;
    AI_PutObjectToStack(reference, AI_gaw_EnumLink[44]);
    return node + 1;
}

tAINode* AI_EvalCateg_InitLocalVarArray(tAINode* node)
{
    AI_InitVarArray(node, &AI_gast_LocalVarStack[AI_gul_SizeLocalStack + node->val]);
    return node + 1;
}

tAINode* AI_EvalCateg_EventParam(tAINode* node)
{
    AI_PutObjectToStack(&EVE_gpst_CurrentParam->privateData, node->val2);
    return node + 1;
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