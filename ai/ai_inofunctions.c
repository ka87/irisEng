#include "../world.h"
#include "ai_worldfunctions.h"
#include "../textget.h"
#include "../inputoutput.h"
#include "ai_inofunctions.h"
#include "../dbg.h"

int AI_EvalFunc_IoGetProtect_C(int a1)
{
    return a1;
}

tAINode* AI_EvalFunc_IoGetProtect(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoGetProtect_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_IoKeyPressed_C(char a1)
{
    return INO_b_IsKeyPressed(a1) != 0;
}

tAINode* AI_EvalFunc_IoKeyPressed(tAINode* a1)
{
    char v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoKeyPressed_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_IoKeyJustPressed_C(char a1)
{
    return INO_b_IsKeyJustPressed(a1);
}

tAINode* AI_EvalFunc_IoKeyJustPressed(tAINode* a1)
{
    char v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoKeyJustPressed_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_IoKeyJustReleased_C(char a1)
{
    return INO_b_IsKeyJustReleased(a1);
}

tAINode* AI_EvalFunc_IoKeyJustReleased(tAINode* a1)
{
    char v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoKeyJustReleased_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoPadSet(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = INO_i_SetCurrentPad(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoPadGet(tAINode* a1)
{
    int v1;

    v1 = INO_i_GetCurrentPad();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoButtonPressed(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = INO_b_IoButtonPressed(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_IoJoyGetMove_C(Vec3* a1)
{
    INO_Joystick_Move(a1, 0);
}

tAINode* AI_EvalFunc_IoJoyGetMove(tAINode* a1)
{
    Vec3 a1a;

    AI_EvalFunc_IoJoyGetMove_C(&a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_IoJoyGetMove1_C(Vec3* a1)
{
    INO_Joystick_Move(a1, 1);
}

tAINode* AI_EvalFunc_IoJoyGetMove1(tAINode* a1)
{
    Vec3 a1a;

    AI_EvalFunc_IoJoyGetMove1_C(&a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

BOOL AI_EvalFunc_IoButtonJustPressed_C(int a1)
{
    return INO_b_Joystick_IsButtonJustDown(a1);
}

tAINode* AI_EvalFunc_IoButtonJustPressed(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoButtonJustPressed_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_IoButtonJustReleased_C(int a1)
{
    return INO_b_Joystick_IsButtonJustUp(a1);
}

tAINode* AI_EvalFunc_IoButtonJustReleased(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoButtonJustReleased_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_IoJoyFreeze_C(int a1)
{
    int v2;

    v2 = ~INO_l_JoystickMask;
    INO_l_JoystickMask = ~a1;
    return v2;
}

tAINode* AI_EvalFunc_IoJoyFreeze(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoJoyFreeze_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_IoJoyDir_C(int a1)
{
    int v2;

    v2 = INO_l_JoystickDir;
    if (a1 != -1)
        INO_l_JoystickDir = a1;
    return v2;
}

tAINode* AI_EvalFunc_IoJoyDir(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoJoyDir_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoJoySimulate(tAINode* a1)
{
    int v1;
    int v3;
    Vec3* a4;
    Vec3* a3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a4 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    INO_JoySimulate(v1, v3, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoPifSet(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    INO_PifSet(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoPafSet(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    INO_PafSet(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoPifPafEnable(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = INO_i_PifPafEnable(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoDiscErrorGet(tAINode* a1)
{
    int v1;

    v1 = INO_i_GetDiscError();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoSetControlMode(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    INO_SetControlMode(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoSetControlSubmode(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    INO_SetControlSubmode(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoSetMouseSensitivity(tAINode* a1)
{
    float v1;

    v1 = AI_GetFloatFromStack();
    INO_SetMouseSensitivity(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoIsMouseBeingUsed(tAINode* a1)
{
    int v1;

    v1 = INO_i_IsMouseBeingUsed();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoDemoGeti(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = Demo_Geti(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_IoDemoSeti(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = Demo_Seti(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_IoGameLangGet_C()
{
    int v1;
    int v2;

    v2 = 0;
    v1 = 0;
    while (v2 < 11)
    {
        if (INO_b_LanguageIsPresent(v2))
            v1 |= 1 << v2;
        ++v2;
    }
    return v1;
}

tAINode* AI_EvalFunc_IoGameLangGet(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_IoGameLangGet_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_IoGameLangSet_C(int a1)
{
    if (!INO_b_LanguageIsPresent(a1))
        return -1;

    TEXT_ChangeLang(a1);
    return 0;
}

tAINode* AI_EvalFunc_IoGameLangSet(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoGameLangSet_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_IoConsLangGet_C()
{
    return 1;
}

tAINode* AI_EvalFunc_IoConsLangGet(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_IoConsLangGet_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_IoConsLangSet_C()
{
    return 0;
}

tAINode* AI_EvalFunc_IoConsLangSet(tAINode* a1)
{
    int v1;

    AI_GetIntFromStack();
    v1 = AI_EvalFunc_IoConsLangSet_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SAVEPhotoDel(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    SAV_PhotoDel(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SAVEPhotoDecal(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SAV_PhotoDecal(v1, v3);
    return a1 + 1;
}

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
        v7[0] = 7;
        v7[1] = 11;
        v7[2] = 17;
        v7[3] = 19;
        v7[4] = 23;
        v7[5] = 29;
        v7[6] = 31;
        v7[7] = 37;
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
                + (unsigned __int8)((i & 0xFFC) >> 2)
                + (unsigned __int8)((i & 0x3FF000) >> 12)
                + (unsigned __int8)((i & 0xFFC00000) >> 22)) & 0x3F;
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

tAINode* AI_EvalFunc_SAVEDataGet(tAINode* a1)
{
    int v1;
    int v2;
    int v4;

    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_SAVEDataGet_C(v1, v4);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_SAVEValidate_C()
{
    int v1;
    unsigned int v2;
    unsigned int v3;
    tAIVars* v4;
    int v5;
    unsigned int i;
    int v7;

    v4 = WOR_gst_Universe.univAIInstance->model->vars;
    v7 = 0;
    for (i = 0; i < v4->varNum; ++i)
    {
        v1 = AI_gast_Types[v4->variables[i].variableType].dataTypeSize;
        v5 = v4->variables[i].varOffset;
        v3 = v4->variables[i].variableArrayCount;
        v2 = v3 >> 30;
        if ((v4->variables[i].flags & 0x10) != 0)
        {
            if (v2)
            {
                memcpy(&AI_gpc_SaveBuffer[v7], &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[v5], 4 * v2);
                v7 += 4 * v2;
            }
            memcpy(&AI_gpc_SaveBuffer[v7], &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[4 * v2 + v5], v1 * (v3 & 0x3FFFFFFF));
            v7 += v1 * (v3 & 0x3FFFFFFF);
        }
    }
    SAV_PhotoValidate();
}

tAINode* AI_EvalFunc_SAVEValidate(tAINode* a1)
{
    AI_EvalFunc_SAVEValidate_C();
    return a1 + 1;
}

void AI_EvalFunc_SAVEValidateAl_C()
{
    int v0;
    unsigned int v1;
    unsigned int v2;
    tAIVars* v3;
    int v4;
    unsigned int i;
    int v6;

    v3 = WOR_gst_Universe.univAIInstance->model->vars;
    v6 = 0;
    for (i = 0; i < v3->varNum; ++i)
    {
        v0 = AI_gast_Types[v3->variables[i].variableType].dataTypeSize;
        v4 = v3->variables[i].varOffset;
        v2 = v3->variables[i].variableArrayCount;
        v1 = v2 >> 30;
        if ((v3->variables[i].flags & 0x40) != 0)
        {
            if (v1)
            {
                memcpy(&AI_gpc_SaveBuffer[v6], &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[v4], 4 * v1);
                v6 += 4 * v1;
            }
            memcpy(
                &AI_gpc_SaveBuffer[v6],
                &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[4 * v1 + v4],
                v0 * (v2 & 0x3FFFFFFF));
            v6 += v0 * (v2 & 0x3FFFFFFF);
        }
        else if ((v3->variables[i].flags & 0x10) != 0)
        {
            if (v1)
                v6 += 4 * v1;
            v6 += v0 * (v2 & 0x3FFFFFFF);
        }
    }
}

tAINode* AI_EvalFunc_SAVEValidateAl(tAINode* a1)
{
    AI_EvalFunc_SAVEValidateAl_C();
    return a1 + 1;
}

void AI_EvalFunc_SAVEUnvalidate_C()
{
    int v0;
    unsigned int v1;
    unsigned int v2;
    tAIVars* v3;
    int v4;
    unsigned int i;
    int v6;

    v3 = WOR_gst_Universe.univAIInstance->model->vars;
    v6 = 0;
    for (i = 0; i < v3->varNum; ++i)
    {
        v0 = AI_gast_Types[v3->variables[i].variableType].dataTypeSize;
        v4 = v3->variables[i].varOffset;
        v2 = v3->variables[i].variableArrayCount;
        v1 = v2 >> 30;
        if ((v3->variables[i].flags & 0x10) != 0)
        {
            if (v1)
            {
                memcpy(&WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[v4], &AI_gpc_SaveBuffer[v6], 4 * v1);
                v6 += 4 * v1;
            }
            memcpy(
                &WOR_gst_Universe.univAIInstance->instanceVarDataCurrent[4 * v1 + v4],
                &AI_gpc_SaveBuffer[v6],
                v0 * (v2 & 0x3FFFFFFF));
            v6 += v0 * (v2 & 0x3FFFFFFF);
        }
    }
    SAV_PhotoUnvalidate();
}

tAINode* AI_EvalFunc_SAVEUnvalidate(tAINode* a1)
{
    AI_EvalFunc_SAVEUnvalidate_C();
    return a1 + 1;
}

void AI_EvalFunc_SAVESaveSlotExt_C(int a1, int a2, int a3, int a4)
{

    if (a1 >= 0)
    {
        INO_gst_SavSlotDesc.id = a1;
        INO_gst_SavSlotDesc.i2 = a2;
        INO_gst_SavSlotDesc.i3 = a3;
        INO_gst_SavSlotDesc.i4 = a4;
        INO_SavSetCardStatus(SAVE_gi_DefaultSlot, 16);
    }
    else
    {
        switch (a1)
        {
        case -1:
            SAV_idx_Field_i2 = a2;
            SAV_idx_Field_i3 = a3;
            SAV_idx_Field_i8 = a4;
            break;
        case -2:
            SAV_idx_Field_i4 = a2;
            SAV_idx_Field_i5 = a3;
            SAV_idx_Field_i6 = a4;
            break;
        case -3:
            SAV_idx_Field_i4 = a2;
            SAV_idx_Field_i9 = a3;
            break;
        }
    }
}

tAINode* AI_EvalFunc_SAVESaveSlotExt(tAINode* a1)
{
    int v2;
    int v3;
    int v4;
    int v5;

    v5 = AI_GetIntFromStack();
    v2 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    AI_EvalFunc_SAVESaveSlotExt_C(v4, v3, v2, v5);
    return a1 + 1;
}

void AI_EvalFunc_SAVESaveSlot_C(int a1, int a2, int a3)
{
    AI_EvalFunc_SAVESaveSlotExt_C(a1, a2, a3, 0);
}

tAINode* AI_EvalFunc_SAVESaveSlot(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_SAVESaveSlot_C(v1, v4, v3);
    return a1 + 1;
}

void AI_EvalFunc_SAVELoadSlot_C(int a1)
{
    INO_gst_SavSlotDesc.id = a1;
    INO_SavSetCardStatus(SAVE_gi_DefaultSlot, 12);
}

tAINode* AI_EvalFunc_SAVELoadSlot(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    AI_EvalFunc_SAVELoadSlot_C(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SAVENewGame(tAINode* a1)
{
    SAV_PhotoInit();
    return a1 + 1;
}

tAINode* AI_EvalFunc_SAVEGetCardStatus(tAINode* a1)
{
    int v1;
    char dest[52];
    tVarStackElem a2;
    int v5;

    AI_GetVarFromStack(dest, &a2);
    v5 = AI_GetIntFromStack();
    v1 = INO_i_SavGetCardStatus(v5, (int*)a2.ptr);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SAVESetCardStatus(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    INO_SavSetCardStatus(v1, v3);
    return a1 + 1;
}

BOOL AI_EvalFunc_IoKeyReleased_C(char a1)
{
    return INO_b_IsKeyReleased(a1) != 0;
}

tAINode* AI_EvalFunc_IoKeyReleased(tAINode* a1)
{
    char v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_IoKeyReleased_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}