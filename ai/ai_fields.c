#include "../ai.h"

tAINode* AI_EvalField_VecX(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 2;
    return a1 + 1;
}

tAINode* AI_EvalField_VecY(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 2;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 4;
    *(uint32_t *)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;    
    return a1 + 1;
}

tAINode* AI_EvalField_VecZ(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 2;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 8;
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgSender(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = AI_gaw_EnumLink[40];
    return a1 + 1;
}

tAINode* AI_EvalField_MsgGAO(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 4;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = AI_gaw_EnumLink[40];
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgId(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 48;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 1;
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgVec1(tAINode* a1)
{
    Vec3* v2;
    Vec3* v3;

    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 8;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 3;
    v2 = (Vec3*)(&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]));
    v3 = (Vec3*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    *(float*)(&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32])) = v3->x;
    v2->y = v3->y;
    v2->z = v3->z;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgVec2(tAINode* a1)
{
    Vec3* v2;
    Vec3* v3;

    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 20;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 3;
    v2 = (Vec3*)(&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]));
    v3 = (Vec3*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    *(float*)(&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32])) = v3->x;
    v2->y = v3->y;
    v2->z = v3->z;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgInt1(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 40;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 1;
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgInt2(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 44;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 1;
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgFloat1(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 32;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 2;
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_MsgFloat2(tAINode* a1)
{
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr += 36;
    AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].type = 2;
    *(uint32_t*)&(AI_gac_GlobalStack[AI_gul_SizeGlobalVarStack - 32]) = *(uint32_t*)AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack].ptr;
    return a1 + 1;
}

tAINode* AI_EvalField_DesignFlags(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)v4->extended->fields;
    a2.type = 1;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignInt1(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designInt1;
    a2.type = 1;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignInt2(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designInt2;
    a2.type = 1;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignText1(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designText1;
    a2.type = AI_gaw_EnumLink[44];
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignText2(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designText2;
    a2.type = AI_gaw_EnumLink[44];
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignFloat1(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2; 
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designFloat1;
    a2.type = 2;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignFloat2(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designFloat2;
    a2.type = 2;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignVec1(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designVec1;
    a2.type = 3;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignVec2(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designVec2;
    a2.type = 3;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignGO1(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designGO1;
    a2.type = AI_gaw_EnumLink[40];
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignGO2(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designGO2;
    a2.type = AI_gaw_EnumLink[40];
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignNet1(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designNet1;
    a2.type = AI_gaw_EnumLink[43];
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignNet2(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designNet2;
    a2.type = AI_gaw_EnumLink[43];
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignInt3(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designInt3;
    a2.type = 1;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tAINode* AI_EvalField_DesignDesFlags(tAINode* a1)
{
    char dest[52];
    tVarStackElem a2;
    tGameObjectMain* v4;

    AI_GetVarFromStack(dest, &a2);
    AI_EvalType_GetGAO((tGameObjectMain**)a2.ptr, &v4);
    a2.ptr = (char*)&v4->extended->fields->designDesFlags;
    a2.type = 1;
    AI_EvalVar(&a2);
    return a1 + 1;
}

tOfc_scriptengine_functions AI_gast_Fields[27] = {
                                    { 0x200,      0, AI_EvalField_VecX },
                                    { 0x201,      0, AI_EvalField_VecY },
                                    { 0x202,      0, AI_EvalField_VecZ },
                                    { 0x203,      0, AI_EvalField_MsgSender },
                                    { 0x204,      0, AI_EvalField_MsgId },
                                    { 0x205,      0, AI_EvalField_MsgVec1 },
                                    { 0x206,      0, AI_EvalField_MsgVec2 },
                                    { 0x207,      0, AI_EvalField_MsgFloat1 },
                                    { 0x208,      0, AI_EvalField_MsgFloat2 },
                                    { 0x209,      0, AI_EvalField_MsgGAO },
                                    { 0x20A,      0, AI_EvalField_MsgInt1 },
                                    { 0x20B,      0, AI_EvalField_MsgInt2 },
                                    { 0x226,      0, AI_EvalField_DesignFlags },
                                    { 0x227,      0, AI_EvalField_DesignInt1 },
                                    { 0x228,      0, AI_EvalField_DesignInt2 },
                                    { 0x229,      0, AI_EvalField_DesignFloat1 },
                                    { 0x22A,      0, AI_EvalField_DesignFloat2 },
                                    { 0x22B,      0, AI_EvalField_DesignVec1 },
                                    { 0x22C,      0, AI_EvalField_DesignVec2 },
                                    { 0x22D,      0, AI_EvalField_DesignGO1 },
                                    { 0x22E,      0, AI_EvalField_DesignGO2 },
                                    { 0x22F,      0, AI_EvalField_DesignNet1 },
                                    { 0x230,      0, AI_EvalField_DesignNet2 },
                                    { 0x231,      0, AI_EvalField_DesignText1 },
                                    { 0x232,      0, AI_EvalField_DesignText2 },
                                    { 0x233,      0, AI_EvalField_DesignInt3 },
                                    { 0x234,      0, AI_EvalField_DesignDesFlags }
};