#include "../world.h"
#include "ai_worldfunctions.h"
#include "../textget.h"
#include "../inputoutput.h"
#include "ai_inofunctions.h"
#include "../dbg.h"

// The copy protection check is gone; the builtin reports whatever the
// script passed it.
int AI_EvalFunc_IoGetProtect_C(int state)
{
    return state;
}

tAINode* AI_EvalFunc_IoGetProtect(tAINode* node)
{
    int state;

    state = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoGetProtect_C(state));
    return node + 1;
}

BOOL AI_EvalFunc_IoKeyPressed_C(char key)
{
    return INO_b_IsKeyPressed(key) != 0;
}

tAINode* AI_EvalFunc_IoKeyPressed(tAINode* node)
{
    char key;

    key = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoKeyPressed_C(key));
    return node + 1;
}

BOOL AI_EvalFunc_IoKeyJustPressed_C(char key)
{
    return INO_b_IsKeyJustPressed(key);
}

tAINode* AI_EvalFunc_IoKeyJustPressed(tAINode* node)
{
    char key;

    key = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoKeyJustPressed_C(key));
    return node + 1;
}

BOOL AI_EvalFunc_IoKeyJustReleased_C(char key)
{
    return INO_b_IsKeyJustReleased(key);
}

tAINode* AI_EvalFunc_IoKeyJustReleased(tAINode* node)
{
    char key;

    key = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoKeyJustReleased_C(key));
    return node + 1;
}

tAINode* AI_EvalFunc_IoPadSet(tAINode* node)
{
    int pad;

    pad = AI_GetIntFromStack();
    AI_PutIntToStack(INO_i_SetCurrentPad(pad));
    return node + 1;
}

tAINode* AI_EvalFunc_IoPadGet(tAINode* node)
{
    AI_PutIntToStack(INO_i_GetCurrentPad());
    return node + 1;
}

tAINode* AI_EvalFunc_IoButtonPressed(tAINode* node)
{
    int button;

    button = AI_GetIntFromStack();
    AI_PutIntToStack(INO_b_IoButtonPressed(button));
    return node + 1;
}

void AI_EvalFunc_IoJoyGetMove_C(Vec3* move)
{
    INO_Joystick_Move(move, 0);
}

tAINode* AI_EvalFunc_IoJoyGetMove(tAINode* node)
{
    Vec3 move;

    AI_EvalFunc_IoJoyGetMove_C(&move);
    AI_PutVecToStack(&move);
    return node + 1;
}

void AI_EvalFunc_IoJoyGetMove1_C(Vec3* move)
{
    INO_Joystick_Move(move, 1);
}

tAINode* AI_EvalFunc_IoJoyGetMove1(tAINode* node)
{
    Vec3 move;

    AI_EvalFunc_IoJoyGetMove1_C(&move);
    AI_PutVecToStack(&move);
    return node + 1;
}

BOOL AI_EvalFunc_IoButtonJustPressed_C(int button)
{
    return INO_b_Joystick_IsButtonJustDown(button);
}

tAINode* AI_EvalFunc_IoButtonJustPressed(tAINode* node)
{
    int button;

    button = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoButtonJustPressed_C(button));
    return node + 1;
}

BOOL AI_EvalFunc_IoButtonJustReleased_C(int button)
{
    return INO_b_Joystick_IsButtonJustUp(button);
}

tAINode* AI_EvalFunc_IoButtonJustReleased(tAINode* node)
{
    int button;

    button = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoButtonJustReleased_C(button));
    return node + 1;
}

// The engine keeps the inverse of what the script asks for, so both the
// argument and the result are inverted. Returns the mask that was in
// force, so a script can put it back.
int AI_EvalFunc_IoJoyFreeze_C(int allowed)
{
    int previous;

    previous = ~INO_l_JoystickMask;
    INO_l_JoystickMask = ~allowed;
    return previous;
}

tAINode* AI_EvalFunc_IoJoyFreeze(tAINode* node)
{
    int allowed;

    allowed = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoJoyFreeze_C(allowed));
    return node + 1;
}

// Returns the direction in force; -1 only reads it.
int AI_EvalFunc_IoJoyDir_C(int dir)
{
    int previous;

    previous = INO_l_JoystickDir;
    if (dir != -1)
        INO_l_JoystickDir = dir;
    return previous;
}

tAINode* AI_EvalFunc_IoJoyDir(tAINode* node)
{
    int dir;

    dir = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoJoyDir_C(dir));
    return node + 1;
}

tAINode* AI_EvalFunc_IoJoySimulate(tAINode* node)
{
    Vec3* move1;
    Vec3* move;
    int buttons;
    int pad;

    // the two vectors are read where the stack keeps them rather than
    // copied out
    move1 = (Vec3*)AI_PopStackSlot();
    move = (Vec3*)AI_PopStackSlot();
    buttons = AI_GetIntFromStack();
    pad = AI_GetIntFromStack();
    INO_JoySimulate(pad, buttons, move, move1);
    return node + 1;
}

tAINode* AI_EvalFunc_IoPifSet(tAINode* node)
{
    int pif;

    pif = AI_GetIntFromStack();
    INO_PifSet(pif);
    return node + 1;
}

tAINode* AI_EvalFunc_IoPafSet(tAINode* node)
{
    int paf;
    int value;

    value = AI_GetIntFromStack();
    paf = AI_GetIntFromStack();
    INO_PafSet(paf, value);
    return node + 1;
}

tAINode* AI_EvalFunc_IoPifPafEnable(tAINode* node)
{
    int enable;

    enable = AI_GetIntFromStack();
    AI_PutIntToStack(INO_i_PifPafEnable(enable));
    return node + 1;
}

tAINode* AI_EvalFunc_IoDiscErrorGet(tAINode* node)
{
    AI_PutIntToStack(INO_i_GetDiscError());
    return node + 1;
}

tAINode* AI_EvalFunc_IoSetControlMode(tAINode* node)
{
    int mode;

    mode = AI_GetIntFromStack();
    INO_SetControlMode(mode);
    return node + 1;
}

tAINode* AI_EvalFunc_IoSetControlSubmode(tAINode* node)
{
    int submode;

    submode = AI_GetIntFromStack();
    INO_SetControlSubmode(submode);
    return node + 1;
}

tAINode* AI_EvalFunc_IoSetMouseSensitivity(tAINode* node)
{
    float sensitivity;

    sensitivity = AI_GetFloatFromStack();
    INO_SetMouseSensitivity(sensitivity);
    return node + 1;
}

tAINode* AI_EvalFunc_IoIsMouseBeingUsed(tAINode* node)
{
    AI_PutIntToStack(INO_i_IsMouseBeingUsed());
    return node + 1;
}

tAINode* AI_EvalFunc_IoDemoGeti(tAINode* node)
{
    int field;

    field = AI_GetIntFromStack();
    AI_PutIntToStack(Demo_Geti(field));
    return node + 1;
}

tAINode* AI_EvalFunc_IoDemoSeti(tAINode* node)
{
    int field;

    field = AI_GetIntFromStack();
    AI_PutIntToStack(Demo_Seti(field));
    return node + 1;
}

// A bit per language that the installed game has text for.
int AI_EvalFunc_IoGameLangGet_C()
{
    int present;
    int lang;

    present = 0;
    for (lang = 0; lang < 11; ++lang)
    {
        if (INO_b_LanguageIsPresent(lang))
            present |= 1 << lang;
    }
    return present;
}

tAINode* AI_EvalFunc_IoGameLangGet(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_IoGameLangGet_C());
    return node + 1;
}

int AI_EvalFunc_IoGameLangSet_C(int lang)
{
    if (!INO_b_LanguageIsPresent(lang))
        return -1;

    TEXT_ChangeLang(lang);
    return 0;
}

tAINode* AI_EvalFunc_IoGameLangSet(tAINode* node)
{
    int lang;

    lang = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoGameLangSet_C(lang));
    return node + 1;
}

// The console language is a console notion; on PC these two report a
// fixed answer and the setter ignores its argument.
int AI_EvalFunc_IoConsLangGet_C()
{
    return 1;
}

tAINode* AI_EvalFunc_IoConsLangGet(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_IoConsLangGet_C());
    return node + 1;
}

int AI_EvalFunc_IoConsLangSet_C()
{
    return 0;
}

tAINode* AI_EvalFunc_IoConsLangSet(tAINode* node)
{
    AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoConsLangSet_C());
    return node + 1;
}

tAINode* AI_EvalFunc_SAVEPhotoDel(tAINode* node)
{
    int photo;

    photo = AI_GetIntFromStack();
    SAV_PhotoDel(photo);
    return node + 1;
}

tAINode* AI_EvalFunc_SAVEPhotoDecal(tAINode* node)
{
    int from;
    int to;

    to = AI_GetIntFromStack();
    from = AI_GetIntFromStack();
    SAV_PhotoDecal(from, to);
    return node + 1;
}

/* NOT REWRITTEN, DELIBERATELY. Renaming this function's locals would
   change what it does.

   Its second branch computes a save code and then returns v3[a2] with
   a2 in 100..115 and v3 declared int[87], so the value handed back is
   whatever sits past the end of that array in the frame. The third loop
   likewise writes v10[15] where v10 is int[11]. Both reach outside
   their arrays, so the result depends on the frame layout, and renaming
   or reordering the locals moves what they land on.

   The shape says the decompiler split one buffer into several locals:
   in the original, the bits built in v10 and the code read out of v3
   were almost certainly the same storage. Reconstructing that is a
   guess, so the function is left exactly as it was, pending a decision.
   Everything around it in this file is rewritten. */
int AI_EvalFunc_SAVEDataGet_C(int a1, int a2)
{
    int result;
    int v3[87];
    int v4;
    int v5;
    int v6;
    int v7[8];
    int v8;
    int v9;
    int v10[11];
    int v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;
    int v17;
    int v18;
    int v19;
    int v20;
    unsigned int v21;
    int i;

    if (a2 < 100)
    {
        v3[86] = a2;
        switch (a2)
        {
        case 0:
            SAV_LoadData(0);
            result = 0;
            break;
        case 1:
            v21 = 0;
            for (i = 0; i < 10; ++i)
            {
                if (SAV_gst_Data[i].superiorAtOrdering)
                    ++v21;
            }
            result = v21;
            break;
        case 2:
            result = SAV_gst_Data[a1].superiorAtOrdering;
            break;
        case 3:
            result = SAV_gst_Data[a1].s1;
            break;
        case 4:
            result = SAV_gst_Data[a1].s2;
            break;
        case 5:
            result = SAV_gst_Data[a1].sec;
            break;
        case 6:
            result = SAV_gst_Data[a1].min;
            break;
        case 7:
            result = SAV_gst_Data[a1].hour;
            break;
        case 8:
            result = SAV_gst_Data[a1].mday;
            break;
        case 9:
            result = SAV_gst_Data[a1].month + 1;
            break;
        case 10:
            result = SAV_gst_Data[a1].year;
            break;
        case 11:
            result = SAV_gst_Data[a1].s3;
            break;
        case 12:
            result = SAV_gst_Data[a1].s4;
            break;
        case 13:
            result = SAV_gst_Data[a1].i1;
            break;
        case 14:
            SAV_LoadData(1);
            result = 0;
            break;
        case 20:
            result = SAV_gst_Data[a1].i2;
            break;
        case 21:
            result = SAV_gst_Data[a1].i3;
            break;
        case 22:
            result = SAV_gst_Data[a1].i4;
            break;
        case 23:
            result = SAV_gst_Data[a1].i5;
            break;
        case 24:
            result = SAV_gst_Data[a1].i6;
            break;
        case 25:
            result = SAV_gst_Data[a1].i7;
            break;
        case 26:
            result = SAV_gst_Data[a1].i8;
            break;
        case 27:
            result = SAV_gst_Data[a1].i9;
            break;
        default:
            result = 0;
            break;
        }
    }
    else
    {
        /* Descending, and the order is load-bearing: the table is
           indexed by v7[v15 & 7], and by v7[i] against v7[7 - i] as
           a mirrored pair, so reversing it changes every code the
           generator produces. */
        v7[0] = 37;
        v7[1] = 31;
        v7[2] = 29;
        v7[3] = 23;
        v7[4] = 19;
        v7[5] = 17;
        v7[6] = 11;
        v7[7] = 7;
        if (a2 <= 115)
        {
            v10[0] = (SAV_gst_Data[a1].i2 & 0x7E0000) >> 17;
            v10[1] = ((SAV_gst_Data[a1].i3 & 0x7000000) >> 24) | (8 * (SAV_gst_Data[a1].i2 & 3)) | ((SAV_gst_Data[a1].i2 & 0x10000) >> 11);
            v10[2] = (SAV_gst_Data[a1].i2 & 0x3F00) >> 8;
            v10[3] = (SAV_gst_Data[a1].i3 & 0xFC0000) >> 18;
            v10[4] = (SAV_gst_Data[a1].i3 & 0x3F000) >> 12;
            v10[5] = (SAV_gst_Data[a1].i3 & 0xFC0) >> 6;
            v10[6] = SAV_gst_Data[a1].i3 & 0x3F;
            v10[7] = (SAV_gst_Data[a1].i9 & 0xFC) >> 2;
            v10[8] = ((SAV_gst_Data[a1].i8 & 0x1E000000) >> 25) | (16 * (SAV_gst_Data[a1].i9 & 3));
            v10[9] = ((SAV_gst_Data[a1].i8 & 0x1F00) >> 8) | ((SAV_gst_Data[a1].i8 & 0x1000000) >> 19);
            v10[10] = ((SAV_gst_Data[a1].i8 & 0xC) >> 2) | ((SAV_gst_Data[a1].i8 & 0x1F0000) >> 14);
            v11 = 16 * (SAV_gst_Data[a1].i8 & 3);
            v15 = SAV_gst_Data[a1].i1 % 60;
            i = (SAV_gst_Data[a1].i1 - v15) / 60;
            v13 = i % 60;
            v14 = (i - i % 60) / 60;
            if (v14 > 63)
                v14 = 63;
            i = SAV_gst_Data[a1].i2;
            i += SAV_gst_Data[a1].i3;
            i += SAV_gst_Data[a1].i8;
            i += SAV_gst_Data[a1].i9;
            i += v13;
            i += v14;
            i += v15;
            v21 = (i & 3) + ((i & 0xFFC) >> 2) + ((i & 0x3FF000) >> 12) + ((i & 0xFFC00000) >> 22);
            v11 |= (int)(((i & 3) + ((i & 0xFFC) >> 2) + ((i & 0x3FF000) >> 12) + ((i & 0xFFC00000) >> 22)) & 0x3C0) >> 6;
            v12 = ((i & 3)
                + (uint8_t)((i & 0xFFC) >> 2)
                + (uint8_t)((i & 0x3FF000) >> 12)
                + (uint8_t)((i & 0xFFC00000) >> 22)) & 0x3F;
            v16 = v7[v15 & 7];
            for (i = 0; i < 40; ++i)
            {
                v17 = v16 * (i + 1) % 90;
                v20 = v17 % 6;
                v19 = (v17 - v17 % 6) / 6;
                v8 = ~((v10[v19] >> (v17 % 6)) & 1);
                v10[(v17 - v20) / 6] = ((v8 & 1) << (v17 % 6)) | ~(1 << (v17 % 6)) & v10[(v17 - v20) / 6];
                v10[v19] &= 0x3Fu;
            }
            v17 = (v15 >> 3) & 7;
            v16 = v7[v17];
            v18 = v7[7 - v17];
            for (i = 0; i < 30; ++i)
            {
                v17 = ((i + 1) * v16 + 45) % 90;
                v20 = v17 % 6;
                v19 = (v17 - v17 % 6) / 6;
                v5 = ((i + 1) * v18 + 45) % 90;
                v4 = v5 % 6;
                v9 = (v5 - v5 % 6) / 6;
                v8 = (v10[v19] >> (v17 % 6)) & 1;
                v6 = (v10[v9] >> (v5 % 6)) & 1;
                v10[(v17 - v20) / 6] = (v6 << (v17 % 6)) | ~(1 << (v17 % 6)) & v10[(v17 - v20) / 6];
                v10[v9] = (v8 << v4) | ~(1 << v4) & v10[v9];
            }
            for (i = 0; i < 6; ++i)
            {
                v17 = 13 * (i + 1);
                v20 = v17 % 6;
                v19 = (v17 - v17 % 6) / 6;
                v4 = i;
                v9 = 15;
                v8 = (v10[v19] >> (v17 % 6)) & 1;
                v6 = (v15 >> i) & 1;
                v10[(v17 - v20) / 6] = (v6 << (v17 % 6)) | ~(1 << (v17 % 6)) & v10[(v17 - v20) / 6];
                v10[v9] = (v8 << v4) | ~(1 << v4) & v10[v9];
            }
            result = v3[a2];
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

tAINode* AI_EvalFunc_SAVEDataGet(tAINode* node)
{
    int v1;
    int v2;
    int v4;

    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_SAVEDataGet_C(v1, v4);
    AI_PutIntToStack(v2);
    return node + 1;
}

// A saved variable's storage is its array-dimension words followed by
// the elements: the top two bits of variableArrayCount are the number
// of dimensions and the low thirty the element count.
static unsigned int AI_l_SaveVarDims(AIVarElem* var)
{
    return var->variableArrayCount >> 30;
}

static int AI_l_SaveVarBytes(AIVarElem* var)
{
    return AI_gast_Types[var->variableType].dataTypeSize
           * (int)(var->variableArrayCount & 0x3FFFFFFF);
}

// Copies the universe instance's saved variables -- the ones flagged
// 0x10 -- into the save buffer, and validates the photos with them.
void AI_EvalFunc_SAVEValidate_C()
{
    tAIVars* vars;
    AIVarElem* var;
    unsigned int dims;
    unsigned int i;
    int offset;
    int at;

    vars = WOR_gst_Universe.univAIInstance->model->vars;
    at = 0;
    for (i = 0; i < vars->varNum; ++i)
    {
        var = &vars->variables[i];
        dims = AI_l_SaveVarDims(var);
        offset = var->varOffset;
        if ((var->flags & 0x10) == 0)
            continue;

        if (dims)
        {
            memcpy(&AI_gpc_SaveBuffer[at],
                   &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[offset],
                   4 * dims);
            at += 4 * dims;
        }
        memcpy(&AI_gpc_SaveBuffer[at],
               &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[4 * dims + offset],
               AI_l_SaveVarBytes(var));
        at += AI_l_SaveVarBytes(var);
    }
    SAV_PhotoValidate();
}

tAINode* AI_EvalFunc_SAVEValidate(tAINode* node)
{
    AI_EvalFunc_SAVEValidate_C();
    return node + 1;
}

// The same for the variables flagged 0x40, which share the buffer with
// the 0x10 ones: a 0x10 variable is skipped over rather than written,
// so both sets land where SAVEValidate would have put them.
void AI_EvalFunc_SAVEValidateAl_C()
{
    tAIVars* vars;
    AIVarElem* var;
    unsigned int dims;
    unsigned int i;
    int offset;
    int at;

    vars = WOR_gst_Universe.univAIInstance->model->vars;
    at = 0;
    for (i = 0; i < vars->varNum; ++i)
    {
        var = &vars->variables[i];
        dims = AI_l_SaveVarDims(var);
        offset = var->varOffset;

        if ((var->flags & 0x40) != 0)
        {
            if (dims)
            {
                memcpy(&AI_gpc_SaveBuffer[at],
                       &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[offset],
                       4 * dims);
                at += 4 * dims;
            }
            memcpy(&AI_gpc_SaveBuffer[at],
                   &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[4 * dims + offset],
                   AI_l_SaveVarBytes(var));
            at += AI_l_SaveVarBytes(var);
        }
        else if ((var->flags & 0x10) != 0)
        {
            if (dims)
                at += 4 * dims;
            at += AI_l_SaveVarBytes(var);
        }
    }
}

tAINode* AI_EvalFunc_SAVEValidateAl(tAINode* node)
{
    AI_EvalFunc_SAVEValidateAl_C();
    return node + 1;
}

// SAVEValidate the other way round: the buffer back into the variables.
void AI_EvalFunc_SAVEUnvalidate_C()
{
    tAIVars* vars;
    AIVarElem* var;
    unsigned int dims;
    unsigned int i;
    int offset;
    int at;

    vars = WOR_gst_Universe.univAIInstance->model->vars;
    at = 0;
    for (i = 0; i < vars->varNum; ++i)
    {
        var = &vars->variables[i];
        dims = AI_l_SaveVarDims(var);
        offset = var->varOffset;
        if ((var->flags & 0x10) == 0)
            continue;

        if (dims)
        {
            memcpy(&WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[offset],
                   &AI_gpc_SaveBuffer[at],
                   4 * dims);
            at += 4 * dims;
        }
        memcpy(&WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[4 * dims + offset],
               &AI_gpc_SaveBuffer[at],
               AI_l_SaveVarBytes(var));
        at += AI_l_SaveVarBytes(var);
    }
    SAV_PhotoUnvalidate();
}

tAINode* AI_EvalFunc_SAVEUnvalidate(tAINode* node)
{
    AI_EvalFunc_SAVEUnvalidate_C();
    return node + 1;
}

// A slot id of zero or more writes the slot descriptor and asks the
// card for a save; a negative one instead records which save-data
// fields the next save takes its three values from.
void AI_EvalFunc_SAVESaveSlotExt_C(int slotOrMode, int value1, int value2, int value3)
{
    if (slotOrMode >= 0)
    {
        INO_gst_SavSlotDesc.id = slotOrMode;
        INO_gst_SavSlotDesc.i2 = value1;
        INO_gst_SavSlotDesc.i3 = value2;
        INO_gst_SavSlotDesc.i4 = value3;
        INO_SavSetCardStatus(SAVE_gi_DefaultSlot, 16);
        return;
    }

    switch (slotOrMode)
    {
    case -1:
        SAV_idx_Field_i2 = value1;
        SAV_idx_Field_i3 = value2;
        SAV_idx_Field_i8 = value3;
        break;
    case -2:
        SAV_idx_Field_i4 = value1;
        SAV_idx_Field_i5 = value2;
        SAV_idx_Field_i6 = value3;
        break;
    case -3:
        SAV_idx_Field_i4 = value1;
        SAV_idx_Field_i9 = value2;
        break;
    }
}

tAINode* AI_EvalFunc_SAVESaveSlotExt(tAINode* node)
{
    int slotOrMode;
    int value1;
    int value2;
    int value3;

    value3 = AI_GetIntFromStack();
    value2 = AI_GetIntFromStack();
    value1 = AI_GetIntFromStack();
    slotOrMode = AI_GetIntFromStack();
    AI_EvalFunc_SAVESaveSlotExt_C(slotOrMode, value1, value2, value3);
    return node + 1;
}

void AI_EvalFunc_SAVESaveSlot_C(int slotOrMode, int value1, int value2)
{
    AI_EvalFunc_SAVESaveSlotExt_C(slotOrMode, value1, value2, 0);
}

tAINode* AI_EvalFunc_SAVESaveSlot(tAINode* node)
{
    int slotOrMode;
    int value1;
    int value2;

    value2 = AI_GetIntFromStack();
    value1 = AI_GetIntFromStack();
    slotOrMode = AI_GetIntFromStack();
    AI_EvalFunc_SAVESaveSlot_C(slotOrMode, value1, value2);
    return node + 1;
}

void AI_EvalFunc_SAVELoadSlot_C(int slot)
{
    INO_gst_SavSlotDesc.id = slot;
    INO_SavSetCardStatus(SAVE_gi_DefaultSlot, 12);
}

tAINode* AI_EvalFunc_SAVELoadSlot(tAINode* node)
{
    int slot;

    slot = AI_GetIntFromStack();
    AI_EvalFunc_SAVELoadSlot_C(slot);
    return node + 1;
}

tAINode* AI_EvalFunc_SAVENewGame(tAINode* node)
{
    SAV_PhotoInit();
    return node + 1;
}

tAINode* AI_EvalFunc_SAVEGetCardStatus(tAINode* node)
{
    char converted[52];
    tVarStackElem var;
    int slot;

    AI_GetVarFromStack(converted, &var);
    slot = AI_GetIntFromStack();
    AI_PutIntToStack(INO_i_SavGetCardStatus(slot, (int*)var.ptr));
    return node + 1;
}

tAINode* AI_EvalFunc_SAVESetCardStatus(tAINode* node)
{
    int slot;
    int status;

    status = AI_GetIntFromStack();
    slot = AI_GetIntFromStack();
    INO_SavSetCardStatus(slot, status);
    return node + 1;
}

BOOL AI_EvalFunc_IoKeyReleased_C(char key)
{
    return INO_b_IsKeyReleased(key) != 0;
}

tAINode* AI_EvalFunc_IoKeyReleased(tAINode* node)
{
    char key;

    key = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_IoKeyReleased_C(key));
    return node + 1;
}
