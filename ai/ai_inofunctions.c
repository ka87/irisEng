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
//
// Through the game's own mask rather than the live one: while the
// input is suspended the live mask is not the game's to read or
// write, and a script that saved it would put the suspension back
// later as though it had chosen it.
int AI_EvalFunc_IoJoyFreeze_C(int allowed)
{
    int previous;

    previous = ~INO_i_GameJoystickMask();
    INO_GameJoystickMaskSet(~allowed);
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

/* The save slot accessor the menus read every field through.

   Selectors below 100 are one field of the slot's index record each.
   Selectors 100..115 are the sixteen characters of the slot's code,
   which is not stored anywhere: it is derived from four of the
   record's words and the play time, folded, and then scrambled. */
int AI_EvalFunc_SAVEDataGet_C(int slot, int field)
{
    /* The scramble keys. Which one a pass uses is chosen by the
       seconds, so a slot saved a second later gets a different code. */
    static const int savKeys[8] = { 37, 31, 29, 23, 19, 17, 11, 7 };

    int code[16];
    int result;
    unsigned int used;
    int i1, i2, i3, i8, i9;
    int totalMinutes;
    int total;
    unsigned int fold;
    int key, keyBack;
    int pos, bit, word;
    int posBack, bitBack, wordBack;
    int bitValue, bitValueBack;
    int i;

    if (field < 100)
    {
        switch (field)
        {
        case 0:
            SAV_LoadData(0);
            result = 0;
            break;
        case 1:
            used = 0;
            for (i = 0; i < 10; ++i)
            {
                if (SAV_gst_Data[i].superiorAtOrdering)
                    ++used;
            }
            result = (int)used;
            break;
        case 2:
            result = SAV_gst_Data[slot].superiorAtOrdering;
            break;
        case 3:
            result = SAV_gst_Data[slot].s1;
            break;
        case 4:
            result = SAV_gst_Data[slot].s2;
            break;
        case 5:
            result = SAV_gst_Data[slot].sec;
            break;
        case 6:
            result = SAV_gst_Data[slot].min;
            break;
        case 7:
            result = SAV_gst_Data[slot].hour;
            break;
        case 8:
            result = SAV_gst_Data[slot].mday;
            break;
        case 9:
            result = SAV_gst_Data[slot].month + 1;
            break;
        case 10:
            result = SAV_gst_Data[slot].year;
            break;
        case 11:
            result = SAV_gst_Data[slot].s3;
            break;
        case 12:
            result = SAV_gst_Data[slot].s4;
            break;
        case 13:
            result = SAV_gst_Data[slot].i1;
            break;
        case 14:
            SAV_LoadData(1);
            result = 0;
            break;
        case 20:
            result = SAV_gst_Data[slot].i2;
            break;
        case 21:
            result = SAV_gst_Data[slot].i3;
            break;
        case 22:
            result = SAV_gst_Data[slot].i4;
            break;
        case 23:
            result = SAV_gst_Data[slot].i5;
            break;
        case 24:
            result = SAV_gst_Data[slot].i6;
            break;
        case 25:
            result = SAV_gst_Data[slot].i7;
            break;
        case 26:
            result = SAV_gst_Data[slot].i8;
            break;
        case 27:
            result = SAV_gst_Data[slot].i9;
            break;
        default:
            result = 0;
            break;
        }
        return result;
    }

    if (field > 115)
        return 0;

    i1 = SAV_gst_Data[slot].i1;
    i2 = SAV_gst_Data[slot].i2;
    i3 = SAV_gst_Data[slot].i3;
    i8 = SAV_gst_Data[slot].i8;
    i9 = SAV_gst_Data[slot].i9;

    /* Thirteen six-bit groups out of four of the record's words. Three
       of them are stitched out of two or three separate runs of bits. */
    code[0]  = (i2 & 0x7E0000) >> 17;
    code[1]  = ((i2 & 0x10000) >> 11) | ((i2 & 0x3) << 3)
             | ((i3 & 0x7000000) >> 24);
    code[2]  = (i2 & 0x3F00) >> 8;
    code[3]  = (i3 & 0xFC0000) >> 18;
    code[4]  = (i3 & 0x3F000) >> 12;
    code[5]  = (i3 & 0xFC0) >> 6;
    code[6]  = i3 & 0x3F;
    code[7]  = (i9 & 0xFC) >> 2;
    code[8]  = ((i9 & 0x3) << 4) | ((i8 & 0x1E000000) >> 25);
    code[9]  = ((i8 & 0x1000000) >> 19) | ((i8 & 0x1F00) >> 8);
    code[10] = ((i8 & 0x1F0000) >> 14) | ((i8 & 0xC) >> 2);
    code[11] = (i8 & 0x3) << 4;                 /* the fold completes it */

    /* Three more are the play time. The hours saturate rather than
       carry, so a slot past 63 hours stops counting them. */
    code[15] = i1 % 60;
    totalMinutes = (i1 - code[15]) / 60;
    code[13] = totalMinutes % 60;
    code[14] = (totalMinutes - code[13]) / 60;
    if (code[14] > 63)
        code[14] = 63;

    /* A ten-bit fold of everything that went in, so that a code with a
       character altered does not read back as a valid one. */
    total = i2 + i3 + i8 + i9 + code[13] + code[14] + code[15];
    fold = ((unsigned int)(total & 0xFFC00000) >> 22)
         + ((total & 0x3FF000) >> 12)
         + ((total & 0xFFC) >> 2)
         + (total & 0x3);
    code[11] |= (fold & 0x3C0) >> 6;
    code[12] = fold & 0x3F;

    /* The scramble treats code[0..14] as one ninety-bit field, six bits
       to the entry. code[15] is left out of it and used as the key, so
       it is the one group that still reads as the seconds afterwards. */
    key = savKeys[code[15] & 0x7];
    for (i = 0; i < 40; ++i)
    {
        pos = ((i + 1) * key) % 90;
        bit = pos % 6;
        word = (pos - bit) / 6;
        bitValue = ~((code[word] >> bit) & 1);
        code[word] = (code[word] & ~(1 << bit)) | ((bitValue & 1) << bit);
        code[word] &= 0x3F;
    }

    key = savKeys[(code[15] >> 3) & 0x7];
    keyBack = savKeys[7 - ((code[15] >> 3) & 0x7)];
    for (i = 0; i < 30; ++i)
    {
        pos = (key * (i + 1) + 45) % 90;
        bit = pos % 6;
        word = (pos - bit) / 6;
        posBack = (keyBack * (i + 1) + 45) % 90;
        bitBack = posBack % 6;
        wordBack = (posBack - bitBack) / 6;
        bitValue = (code[word] >> bit) & 1;
        bitValueBack = (code[wordBack] >> bitBack) & 1;
        code[word] = (code[word] & ~(1 << bit)) | (bitValueBack << bit);
        code[wordBack] = (code[wordBack] & ~(1 << bitBack))
                       | (bitValue << bitBack);
    }

    /* And six bits traded with the key group itself. */
    for (i = 0; i < 6; ++i)
    {
        pos = 13 * (i + 1);
        bit = pos % 6;
        word = (pos - bit) / 6;
        bitValue = (code[word] >> bit) & 1;
        bitValueBack = (code[15] >> i) & 1;
        code[word] = (code[word] & ~(1 << bit)) | (bitValueBack << bit);
        code[15] = (code[15] & ~(1 << i)) | (bitValue << i);
    }

    return code[field - 100];
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
