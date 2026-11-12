#include "../ai.h"

// A field reads a part of the value already on top of the stack: the
// descriptor there is re-pointed at the field's offset, retyped, and
// the value copied down into the value stack beside it. The first field
// of a value sits at offset zero and needs no copy, which is why three
// of these do not make one.

#define AI_FIELD_TOP()  (&AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack])
#define AI_FIELD_SLOT() (&AI_gac_GlobalStack[AI_gul_SizeGlobalStack - 32])


tAINode* AI_EvalField_VecX(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->type = 2;
    return node + 1;
}

tAINode* AI_EvalField_VecY(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 4;
    top->type = 2;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_VecZ(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 8;
    top->type = 2;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_MsgSender(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->type = AI_gaw_EnumLink[40];
    return node + 1;
}

tAINode* AI_EvalField_MsgGAO(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 4;
    top->type = AI_gaw_EnumLink[40];
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_MsgId(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 48;
    top->type = 1;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_MsgVec1(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();
    Vec3* slot;
    Vec3* field;

    top->ptr += 8;
    top->type = 3;
    slot = (Vec3*)AI_FIELD_SLOT();
    field = (Vec3*)top->ptr;
    slot->x = field->x;
    slot->y = field->y;
    slot->z = field->z;
    return node + 1;
}

tAINode* AI_EvalField_MsgVec2(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();
    Vec3* slot;
    Vec3* field;

    top->ptr += 20;
    top->type = 3;
    slot = (Vec3*)AI_FIELD_SLOT();
    field = (Vec3*)top->ptr;
    slot->x = field->x;
    slot->y = field->y;
    slot->z = field->z;
    return node + 1;
}

tAINode* AI_EvalField_MsgInt1(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 40;
    top->type = 1;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_MsgInt2(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 44;
    top->type = 1;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_MsgFloat1(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 32;
    top->type = 2;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

tAINode* AI_EvalField_MsgFloat2(tAINode* node)
{
    tVarStackElem* top = AI_FIELD_TOP();

    top->ptr += 36;
    top->type = 2;
    *(uint32_t*)AI_FIELD_SLOT() = *(uint32_t*)top->ptr;
    return node + 1;
}

// Every design field is read the same way: the object the expression
// names is popped off the stack and resolved to a game object, whose
// field block the accessor then hands back as a variable of the
// field's own type.
static tGameObjectMain* AI_l_PopFieldObject(tVarStackElem* var)
{
    char converted[52];
    tGameObjectMain* gao;

    AI_GetVarFromStack(converted, var);
    AI_EvalType_GetGAO((tGameObjectMain**)var->ptr, &gao);
    return gao;
}

tAINode* AI_EvalField_DesignFlags(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)gao->extended->fields;
    var.type = 1;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignInt1(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designInt1;
    var.type = 1;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignInt2(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designInt2;
    var.type = 1;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignText1(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designText1;
    var.type = AI_gaw_EnumLink[44];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignText2(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designText2;
    var.type = AI_gaw_EnumLink[44];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignFloat1(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designFloat1;
    var.type = 2;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignFloat2(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designFloat2;
    var.type = 2;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignVec1(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designVec1;
    var.type = 3;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignVec2(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designVec2;
    var.type = 3;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignGO1(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designGO1;
    var.type = AI_gaw_EnumLink[40];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignGO2(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designGO2;
    var.type = AI_gaw_EnumLink[40];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignNet1(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designNet1;
    var.type = AI_gaw_EnumLink[43];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignNet2(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designNet2;
    var.type = AI_gaw_EnumLink[43];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignInt3(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designInt3;
    var.type = 1;
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalField_DesignDesFlags(tAINode* node)
{
    tVarStackElem var;
    tGameObjectMain* gao;

    gao = AI_l_PopFieldObject(&var);
    var.ptr = (char*)&gao->extended->fields->designDesFlags;
    var.type = 1;
    AI_EvalVar(&var);
    return node + 1;
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