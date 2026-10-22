#include "memory.h"
#include "time.h"
#include "ai.h"
#include "dbg.h"
#include "win/wininputoutput.h"
#include "inputoutput.h"

int INO_ge_SavSetNextState = 0xffffffff;
int INO_ge_SavSetNextNextState = 0xffffffff;
int INO_ge_SavCommand = 0xffffffff;
int INO_ge_SavState;
int INO_gi_LaunchDemo = 0xffffffff;
int Photo_gi_FirstInit = 0; // TODO FIXME: nem állítja a játék
int Photo_gi_Init = 0; // TODO FIXME: nem állítja a játék

//int INO_gai_PresentLanguage[12] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 }; // demo
int INO_gai_PresentLanguage[12] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 3 };
int INO_ge_MasterType = 0; // TODO FIXME: nem állítja a játék

void INO_Joystick_Init(HWND a1)
{
    memset(&INO_gst_JoySimulateAsked, 0, sizeof(INO_gst_JoySimulateAsked));
    memset(&INO_gst_JoySimulateCurrent, 0, sizeof(INO_gst_JoySimulateCurrent));
    memset(INO_gst_CurrPad, 0, sizeof(INO_gst_CurrPad));
    memset(INO_gst_PrevPad, 0, sizeof(INO_gst_CurrPad));
    s_JoystickInit = pcINO_Joystick_Init;
    s_JoystickClose = pcINO_Joystick_Close;
    s_JoystickUpdate = pcINO_Joystick_Update;
    s_JoystickPifPafEnable = pcINO_JoystickPifPafEnable;// Rumble1 ?
    s_JoystickPafIntensity = pcINO_JoystickPafIntensity;// Rumble2 ?
    s_joyMoveFunction = pcINO_Joystick_Move;
    s_IsMouseBeingUsedFunction = pcINO_i_IsMouseBeingUsed;
    s_SetCurrentPad = pcINO_i_SetCurrentPad;
    INO_l_JoystickMask = 0xffffffff;
    INO_l_JoystickDir = 0;
    INO_l_Joystick_Enable = 0;
    INO_gi_PifDuration = 0.0;
    INO_gi_PafDuration = 0.0;
    INO_gi_PafIntensity = 0;
    INO_gi_PifPafEnable = 1;
    INO_gi_CurrentPadId = -1;
    s_JoystickInit(a1);
}

void INO_SavInitModule() {
    memset(INO_gst_MemoryCardManager, 0, sizeof(INO_gst_MemoryCardManager));
    INO_gst_MemoryCardManager[41] = pcINO_SavModuleInit;
    INO_gst_MemoryCardManager[42] = pcINO_SavModuleClose;
    INO_gst_MemoryCardManager[0] = pcINO_b_AsyncInProgress;
    INO_gst_MemoryCardManager[2] = pcINO_i_TestPresence;
    INO_gst_MemoryCardManager[3] = pcINO_i_MountCard;
    INO_gst_MemoryCardManager[4] = pcINO_i_UnmountCard;
    INO_gst_MemoryCardManager[5] = pcINO_i_CheckCard;
    INO_gst_MemoryCardManager[6] = pcINO_i_FormatCard;
    INO_gst_MemoryCardManager[7] = pcINO_i_TestFreeSpace;
    INO_gst_MemoryCardManager[25] = pcINO_i_Retry;
    INO_gst_MemoryCardManager[26] = pcINO_i_Continue;
    INO_gst_MemoryCardManager[24] = pcINO_i_ActiveConsole;
    INO_gst_MemoryCardManager[22] = pcINO_i_Open;
    INO_gst_MemoryCardManager[23] = pcINO_i_Close;
    INO_gst_MemoryCardManager[9] = pcINO_i_ReadIndex_1;
    INO_gst_MemoryCardManager[12] = pcINO_i_WriteIndex;
    INO_gst_MemoryCardManager[14] = pcINO_i_ReadSlot_1;
    INO_gst_MemoryCardManager[15] = pcINO_i_ReadSlot_2;
    INO_gst_MemoryCardManager[16] = pcINO_i_ReadSlot_3;
    INO_gst_MemoryCardManager[19] = pcINO_i_WriteSlot_C;
    INO_gst_MemoryCardManager[21] = pcINO_i_WriteSlot_E;
    INO_gst_MemoryCardManager[29] = pcINO_i_WriteIndex;
    INO_gst_MemoryCardManager[32] = pcINO_i_CheckFreeSpace;
    INO_gst_MemoryCardManager[33] = pcINO_i_TestCard;
    pcINO_SavModuleInit();
}

void INO_KeyboardInit()
{
    INO_Keys = (byte*)MEM_p_Alloc(256u);
    INO_Keys_Prev = (byte*)MEM_p_Alloc(256u);
    memset(INO_keybUnkDataProbablyUnused, 0, 100u);
}

void INO_InitModule()
{
    INO_gi_DiscError = 0;
    INO_KeyboardInit();
    INO_SavInitModule();
}

int INO_i_SetCurrentPad(int a1)
{
    return s_SetCurrentPad(a1);
}

int INO_i_GetCurrentPad()
{
    return INO_gi_CurrentPadId;
}

int INO_SimulateReset()
{
    int result;

    result = 0;
    memset(&INO_gst_JoySimulateAsked, 0, sizeof(INO_gst_JoySimulateAsked));
    memset(&INO_gst_JoySimulateCurrent, 0, sizeof(INO_gst_JoySimulateCurrent));
    return result;
}

void INO_Joystick_Close(void)
{
    s_JoystickClose();
}

void INO_Joystick_Update()
{
    int i; 
    float pifDurRemaining, pafDurRemaining;
    byte controlsMask[16];
    byte buttons[16];

    if (INO_l_Joystick_Enable)
    {
        memcpy(&INO_gst_JoySimulateCurrent, &INO_gst_JoySimulateAsked, sizeof(INO_gst_JoySimulateCurrent));
        memset(&INO_gst_JoySimulateAsked, 0, sizeof(INO_gst_JoySimulateAsked));
        memcpy(INO_gst_PrevPad, INO_gst_CurrPad, sizeof(INO_gst_PrevPad));

        s_JoystickUpdate(INO_gst_CurrPad);

        if (!Demo_gb_Exit)
        {
            for (i = 0; i < 4; i++) {
                if (INO_gst_CurrPad[i].padData[8] && INO_gst_CurrPad[i].padData[9]) {
                    Demo_gb_Exit = 1;
                    break;
                }
            }          
        }

        if (INO_gi_CurrentPadId != -1)
        {
            for (i = 0; i < 12; i++)
            {
                if (((1 << i) & INO_l_JoystickMask) == 0)
                    INO_gst_CurrPad[INO_gi_CurrentPadId].padData[i] = 0;
            }
            if (INO_gst_JoySimulateCurrent.controlsMask)
            {
                memset(controlsMask, 0, sizeof(controlsMask));
                memset(buttons, 0, sizeof(buttons));
                for (i = 0; i < 12; i++)
                {
                    if (((1 << i) & INO_gst_JoySimulateCurrent.controlsMask) != 0)
                        controlsMask[i] = -1;
                    if (((1 << i) & INO_gst_JoySimulateCurrent.buttons) != 0)
                        buttons[i] = -1;
                    INO_gst_CurrPad[INO_gi_CurrentPadId].padData[i] = INO_gst_CurrPad[INO_gi_CurrentPadId].padData[i] & ~controlsMask[i] | controlsMask[i] & buttons[i];
                }
            }
        }
        if (s_IsMouseBeingUsedFunction())
        {
            if (INO_gi_PifDuration != 0.0f)
            {
                pifDurRemaining = INO_gi_PifDuration - TIM_gf_dt;
                INO_gi_PifDuration = (float)pifDurRemaining;
                if (pifDurRemaining <= 0.0f)
                {
                    s_JoystickPifPafEnable(0);
                    INO_gi_PifDuration = 0.0f;
                }
            }
            if (INO_gi_PafDuration != 0.0f)
            {
                pafDurRemaining = INO_gi_PafDuration - TIM_gf_dt;
                INO_gi_PafDuration = (float)pafDurRemaining;
                if (pafDurRemaining <= 0.0f)
                {
                    s_JoystickPafIntensity(0);
                    INO_gi_PafDuration = 0.0f;
                }
            }
        }
    }
}

int INO_i_PifPafEnable(int a1)
{
    int result;

    if (a1 != -1)
    {
        if (a1)
        {
            result = 1;
            INO_gi_PifPafEnable = 1;
            return result;
        }
        INO_gi_PifPafEnable = 0;
        INO_gi_PifDuration = 0.0;
        INO_gi_PafDuration = 0.0;
        s_JoystickPifPafEnable(0);
        s_JoystickPafIntensity(0);
    }
    return INO_gi_PifPafEnable;
}

void INO_PifSet(int a1)
{
    int v1;

    if (INO_gi_PifPafEnable)
    {
        if (a1 < 0)
            v1 = -a1;
        else
            v1 = a1;
        INO_gi_PifDuration = (float)((double)v1 * 0.032000002);
        s_JoystickPifPafEnable(a1 != 0);
    }
}

void INO_PafSet(int a1, int a2)
{
    int v2;
    double v3;
    int v4;

    if (INO_gi_PifPafEnable)
    {
        v2 = a2;
        if (a2 < 0)
            v2 = -a2;
        v3 = (double)v2;
        v4 = a1;
        INO_gi_PafDuration = (float)(v3 * 0.032000002);
        if (a1 > 255)
            v4 = 255;
        INO_gi_PafIntensity = v4;
        s_JoystickPafIntensity(v4);
    }
}

int INO_b_Joystick_IsButtonDown(int a1)
{
    int result;

    if (INO_gi_CurrentPadId == -1)
        result = 0;
    else
        result = INO_gst_CurrPad[INO_gi_CurrentPadId].padData[a1];
    return result;
}

BOOL INO_b_Joystick_IsButtonJustDown(int a1)
{
    return INO_gi_CurrentPadId != -1
        && INO_gst_CurrPad[INO_gi_CurrentPadId].padData[a1]
        && !INO_gst_PrevPad[INO_gi_CurrentPadId].padData[a1];
}

BOOL INO_b_Joystick_IsButtonJustUp(int a1)
{
    return INO_gi_CurrentPadId != -1
        && !INO_gst_CurrPad[INO_gi_CurrentPadId].padData[a1]
        && INO_gst_PrevPad[INO_gi_CurrentPadId].padData[a1];
}

void INO_Joystick_Move(Vec3* a1, int a2)
{
    int v2;
    double v3;
    double v4;

    if (!INO_l_Joystick_Enable || INO_gi_CurrentPadId == -1)
        goto LABEL_16;
    v2 = 1 << (a2 + 24);
    if ((v2 & INO_gst_JoySimulateCurrent.controlsMask) != 0)
    {
        if (a2)
        {
            v3 = INO_gst_JoySimulateCurrent.amountXRight;
            v4 = INO_gst_JoySimulateCurrent.amountYRight;
        }
        else
        {
            v3 = INO_gst_JoySimulateCurrent.amountXLeft;
            v4 = INO_gst_JoySimulateCurrent.amountYLeft;
        }
        a1->x = (float)v3;
        a1->z = 0.0;
        a1->y = (float)v4;
        if ((INO_l_JoystickDir & 1) != 0)
            a1->x = (float)(a1->x * -1.0);
        if ((INO_l_JoystickDir & 2) != 0)
            a1->y = (float)(a1->y * -1.0);
        return;
    }
    if ((v2 & INO_l_JoystickMask) == 0)
    {
    LABEL_16:
        a1->x = 0.0;
        a1->y = 0.0;
        a1->z = 0.0;
        return;
    }
    s_joyMoveFunction(a1, a2);
    if ((INO_l_JoystickDir & 1) != 0)
        a1->x = (float)(a1->x * -1.0);
    if ((INO_l_JoystickDir & 2) != 0)
        a1->y = (float)(a1->y * -1.0);
}

void INO_JoySimulate(int controlMask, int buttons, Vec3* leftStick, Vec3* rightStick)
{
    INO_gst_JoySimulateAsked.controlsMask = controlMask;
    INO_gst_JoySimulateAsked.buttons = buttons;
    if (leftStick)
    {
        INO_gst_JoySimulateAsked.amountXLeft = leftStick->x;
        INO_gst_JoySimulateAsked.amountYLeft = leftStick->y;
    }
    if (rightStick)
    {
        INO_gst_JoySimulateAsked.amountXRight = rightStick->x;
        INO_gst_JoySimulateAsked.amountYRight = rightStick->y;
    }
}

void INO_SetControlSubmode(int a1)
{
    INO_SetControlSubmode_inner_sub_48A650(a1);
}

void INO_SetControlMode(int a1)
{
    INO_SetControlMode_inner_sub_48ACB0(a1);
}

void INO_KeyboardClose()
{
    MEM_Free(INO_Keys);
    MEM_Free(INO_Keys_Prev);
}

int INO_SavCloseModule(void)
{
    return INO_gst_MemoryCardManager[42]();
}

void INO_CloseModule()
{
    INO_KeyboardClose();
    INO_SavCloseModule();
}

void INO_Keyboard_Update()
{
    byte* v0;
    int i;

    v0 = INO_Keys_Prev;
    INO_Keys_Prev = INO_Keys;
    memset(v0, 0, 256u);
    INO_Keys = v0;
    if (pcINO_keyboardDataBackup[157])
    {
        for (i = 0; i < 10; ++i)
        {
            if (pcINO_keyboardDataBackup[i + 59])
            {
                v0[164] = 0x80;
                v0[i + 96] = 0x80;
            }
        }
    }
}

void INO_Reinit()
{
    INO_gi_DiscError = 0;
    INO_l_JoystickMask = -1;
    INO_l_JoystickDir = 0;
    INO_SimulateReset();
    INO_PifSet(0);
    INO_PafSet(0, 0);
    // INO_SavReinit(); // nullsub
}

BOOL INO_b_LanguageIsPresent(int a1)
{
    return a1 < 11
        && a1 >= 0
        && (INO_ge_MasterType == 1 || INO_ge_MasterType != 2 || a1 <= 1 || a1 == 3)
        && INO_gai_PresentLanguage[a1];
}

int INO_i_SavGetCardStatus(int a1, int* a2)
{
    int result;

    if (a2)
        memcpy(a2, INO_gi_SavButton, 32u);
    if (INO_ge_SavState != 27 && INO_ge_SavState)
        goto LABEL_8;
    result = pcINO_gi_LastError;
    if (INO_ge_SavSetNextNextState != -1 && !pcINO_gi_LastError)
    {
        INO_ge_SavState = INO_ge_SavSetNextNextState;
        INO_ge_SavSetNextNextState = -1;
    LABEL_8:
        result = -1;
    }
    return result;
}

int INO_b_IsKeyPressed(uint8_t a1)
{
    return INO_Keys[a1] & 0x80;
}

BOOL INO_b_IsKeyJustPressed(uint8_t a1)
{
    return (INO_Keys[a1] & 0x80) != 0 && (INO_Keys_Prev[a1] & 0x80) == 0;
}

BOOL INO_b_IsKeyReleased(uint8_t a1)
{
    return (INO_Keys[a1] & 0x80) == 0;
}

BOOL INO_b_IsKeyJustReleased(uint8_t a1)
{
    return (INO_Keys[a1] & 0x80) == 0 && (INO_Keys_Prev[a1] & 0x80) != 0;
}

int INO_i_GetDiscError()
{
    return INO_gi_DiscError;
}

int INO_b_IoButtonPressed(int a1)
{
    return (uint8_t)-(INO_b_Joystick_IsButtonDown(a1) != 0);
}

void SAV_PhotoFirstInit()
{
    if (!Photo_gi_FirstInit)
    {
        SAV_pc_PhotoValidateBuffer = (char*)MEM_p_Alloc(0x64000);
        Photo_gi_FirstInit = 1;
    }
}

void SAV_PhotoInit()
{
    char* photoBufPtrBackup;

    if (!Photo_gi_Init)
    {
        photoBufPtrBackup = SAV_gst_Photos.photoBuf;
        memset(&SAV_gst_Photos, 0, sizeof(SAV_gst_Photos));
        SAV_gst_Photos.photoBufSize = 409600;
        SAV_gst_Photos.photoBuf = photoBufPtrBackup;
        SAV_gst_Photos.photoBuf = (char*)MEM_p_Alloc(409600);
        SAV_gst_PhotosValidate.photoBufSize = 409600;
        SAV_gst_PhotosValidate.photoBuf = SAV_pc_PhotoValidateBuffer;
        Photo_gi_Init = 1;
    }
}

BOOL SAV_b_IsMoreRecent(int a1, int a2)
{
    BOOL result;

    if (SAV_gst_Data[a1].superiorAtOrdering)
    {
        if (SAV_gst_Data[a2].superiorAtOrdering)
        {
            if (SAV_gst_Data[a1].year == SAV_gst_Data[a2].year)
            {
                if (SAV_gst_Data[a1].month == SAV_gst_Data[a2].month)
                {
                    if (SAV_gst_Data[a1].mday == SAV_gst_Data[a2].mday)
                    {
                        if (SAV_gst_Data[a1].hour == SAV_gst_Data[a2].hour)
                        {
                            if (SAV_gst_Data[a1].min == SAV_gst_Data[a2].min)
                                result = SAV_gst_Data[a1].sec >= SAV_gst_Data[a2].sec;
                            else
                                result = SAV_gst_Data[a1].min > SAV_gst_Data[a2].min;
                        }
                        else
                        {
                            result = SAV_gst_Data[a1].hour > SAV_gst_Data[a2].hour;
                        }
                    }
                    else
                    {
                        result = SAV_gst_Data[a1].mday > SAV_gst_Data[a2].mday;
                    }
                }
                else
                {
                    result = SAV_gst_Data[a1].month > SAV_gst_Data[a2].month;
                }
            }
            else
            {
                result = SAV_gst_Data[a1].year > SAV_gst_Data[a2].year;
            }
        }
        else
        {
            result = 1;
        }
    }
    else if (SAV_gst_Data[a2].superiorAtOrdering)
    {
        result = 0;
    }
    else
    {
        result = a1 < a2;
    }
    return result;
}

void SAV_OrderData()
{
    int j;
    int v1[10];
    int k;
    int i;

    for (i = 0; i < 10; ++i)
        v1[i] = i;
    for (i = 1; i < 10; ++i)
    {
        for (j = 0; j < i && !SAV_b_IsMoreRecent(i, v1[j]); ++j);
        for (k = i - 1; k >= j; --k)
            v1[k + 1] = v1[k];
        v1[j] = i;
    }
    for (i = 0; i < 10; ++i)
    {
        SAV_gst_Data[i].s4 = v1[i];
        SAV_gst_Data[v1[i]].s3 = i;
    }
}

void INO_v_MemoryCardManager(int defaultSlot)
{
    struct tm* lt;
    __int16 v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9; 
    int v10;
    int v11;
    int v12;
    time_t Time;

    if (!INO_gst_MemoryCardManager[0](defaultSlot))
    {
        switch (INO_ge_SavState)
        {
        case 0:
        case 27:
            return;
        case 1:
            pcINO_gi_LastError = 0;
            goto LABEL_7;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 18:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 31:
        case 32:
            goto LABEL_7;
        case 7:
            INO_ge_SavState = 21;
            INO_ge_SavSetNextState = 8;
            return;
        case 8:
            memset(SAV_gst_Data, 0, sizeof(SAV_gst_Data));
            goto LABEL_10;
        case 9:
            SAV_OrderData();
            INO_ge_SavState = 22;
            return;
        case 10:
            goto LABEL_28;
        case 11:
        LABEL_10:
            if (!INO_gst_SavFileDesc.fp)
                goto LABEL_4;
            INO_gst_SavFileDesc.size = 640;
            INO_gst_SavFileDesc.saveBuffer = (char*)SAV_gst_Data;
        LABEL_7:
            INO_ge_SavState = INO_gst_MemoryCardManager[INO_ge_SavState + 1](defaultSlot);
            break;
        case 12:
            INO_ge_SavState = 21;
            INO_ge_SavSetNextState = 13;
            return;
        case 13:
            if (!INO_gst_SavFileDesc.fp)
                goto LABEL_4;
            INO_gst_SavFileDesc.size = AI_gi_SaveBufferSize;
            INO_gst_SavFileDesc.saveBuffer = (char*)MEM_p_Alloc(AI_gi_SaveBufferSize);
            memset(INO_gst_SavFileDesc.saveBuffer, 0, INO_gst_SavFileDesc.size);
            INO_ge_SavState = INO_gst_MemoryCardManager[INO_ge_SavState + 1](defaultSlot);
            break;
        case 14:
            memcpy(AI_gpc_SaveBuffer, INO_gst_SavFileDesc.saveBuffer, AI_gi_SaveBufferSize);
            MEM_Free(INO_gst_SavFileDesc.saveBuffer);
            SAV_PhotoInit();
            pc_Buffer_304 = SAV_gst_PhotosValidate.photoBuf;
            INO_gst_SavFileDesc.size = 812;
            INO_gst_SavFileDesc.saveBuffer = (char*)&SAV_gst_PhotosValidate;
            INO_ge_SavState = INO_gst_MemoryCardManager[INO_ge_SavState + 1](defaultSlot);
            return;
        case 15:
            SAV_gst_PhotosValidate.photoBuf = pc_Buffer_304;
            pc_Buffer_304 = 0;
            SAV_gst_PhotosValidate.photoBufSize = SAV_gst_PhotosValidate.usedSize;
            INO_gst_SavFileDesc.size = SAV_gst_PhotosValidate.usedSize;
            INO_gst_SavFileDesc.saveBuffer = SAV_gst_PhotosValidate.photoBuf;
            goto LABEL_7;
        case 16:
            INO_ge_SavState = 21;
            INO_ge_SavSetNextState = 17;
            return;
        case 17:
            if (!INO_gst_SavFileDesc.fp)
                goto LABEL_4;
            INO_gst_SavFileDesc.size = AI_gi_SaveBufferSize;
            INO_gst_SavFileDesc.saveBuffer = (char*)MEM_p_Alloc(AI_gi_SaveBufferSize);
            memset(INO_gst_SavFileDesc.saveBuffer, 0, INO_gst_SavFileDesc.size);
            memcpy(INO_gst_SavFileDesc.saveBuffer, AI_gpc_SaveBuffer, INO_gst_SavFileDesc.size);
            INO_ge_SavState = 18;
            break;
        case 19:
            MEM_Free(INO_gst_SavFileDesc.saveBuffer);
            SAV_PhotoInit();
            INO_gst_SavFileDesc.saveBuffer = (char*)&SAV_gst_PhotosValidate;
            INO_gst_SavFileDesc.size = 812;
            INO_ge_SavState = 20;
            return;
        case 26:
            pcINO_gi_LastError = 1;
            INO_ge_SavSetNextNextState = -1;
            INO_ge_SavSetNextState = -1;
            INO_ge_SavCommand = -1;
            goto LABEL_4;
        case 28:
            if (SAV_gst_PhotosValidate.usedSize)
            {
                INO_gst_SavFileDesc.saveBuffer = SAV_gst_PhotosValidate.photoBuf;
                INO_gst_SavFileDesc.size = SAV_gst_PhotosValidate.usedSize;
                INO_ge_SavState = INO_gst_MemoryCardManager[INO_ge_SavState + 1](defaultSlot);
                INO_ge_SavSetNextState = 29;
            }
            else
            {
                INO_ge_SavSetNextState = 29;
                INO_ge_SavState = 22;
            }
            return;
        case 29:
            if (INO_gst_SavSlotDesc.id < 10)
            {
                time(&Time);
                lt = localtime(&Time);
                SAV_gst_Data[INO_gst_SavSlotDesc.id].superiorAtOrdering = 1;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].sec = lt->tm_sec;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].min = lt->tm_min;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].hour = lt->tm_hour;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].mday = lt->tm_mday;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].month = lt->tm_mon;
                v3 = INO_gst_SavSlotDesc.i2;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].year = lt->tm_year + 1900;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].s1 = v3;
                v4 = INO_gst_SavSlotDesc.i4;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].s2 = (__int16)INO_gst_SavSlotDesc.i3;
                v5 = SAV_idx_Field_i2;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i1 = v4;
                v6 = SAV_idx_Field_i3;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i2 = v5;
                v7 = SAV_idx_Field_i4;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i3 = v6;
                v8 = SAV_idx_Field_i5;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i4 = v7;
                v9 = SAV_idx_Field_i6;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i5= v8;
                v10 = SAV_idx_Field_i7;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i6 = v9;
                v11 = SAV_idx_Field_i8;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i7 = v10;
                v12 = SAV_idx_Field_i9;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i8 = v11;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i9 = v12;
                strcpy(INO_gsz_SavFileName, "sally.idx");
            LABEL_28:
                INO_ge_SavState = 21;
                INO_ge_SavSetNextState = 11;
            }
            return;
        case 30:
            INO_gi_FreeSpace = 0;
            INO_ge_SavState = 1;
            INO_ge_SavSetNextState = 31;
            return;
        case 34:
            sprintf(INO_gsz_SavFileName, "slot%d.sav", INO_gst_SavSlotDesc.id);
            INO_ge_SavState = 1;
            INO_ge_SavSetNextState = 16;
            INO_ge_SavCommand = 16;
            return;
        default:
        LABEL_4:
            INO_ge_SavState = 0;
            break;
        }
    }
}

void INO_Update()
{
    INO_Joystick_Update();
    INO_Keyboard_Update();
    INO_v_MemoryCardManager(SAVE_gi_DefaultSlot);
}

void INO_SavSetCardStatus(int a1, int a2)
{    
    if (a2 < 0 || a2 >= 40)
        DBG_assert(0, __FILE__, __LINE__, "0", "SAVING : Bad Param State", 0, 1);
    if (INO_ge_SavState && INO_ge_SavState != 27)
        DBG_assert(0, __FILE__, __LINE__, "0", "SAVING : Bad Current State", 0, 1);

    if (INO_ge_SavState != a2)
    {
        switch (a2)
        {
        case 1:
            goto LABEL_9;
        case 7:
            pcINO_gi_LastError = 0;
            strcpy(INO_gsz_SavFileName, "sally.idx");
            INO_ge_SavState = 1;
            INO_ge_SavSetNextState = 7;
            INO_ge_SavCommand = 7;
            return;
        case 10:
            pcINO_gi_LastError = 0;
            strcpy(INO_gsz_SavFileName, "sally.idx");
            INO_ge_SavState = 1;
            INO_ge_SavSetNextState = 10;
            INO_ge_SavCommand = 10;
            return;
        case 12:
            sprintf(INO_gsz_SavFileName, "slot%d.sav", INO_gst_SavSlotDesc.id);
            pcINO_gi_LastError = 0;
            INO_ge_SavState = 1;
            INO_ge_SavSetNextState = 12;
            INO_ge_SavCommand = 12;
            return;
        case 16:
            if (INO_gst_SavSlotDesc.id < 10)
            {
                SAV_gst_Data[INO_gst_SavSlotDesc.id].superiorAtOrdering = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].sec = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].min = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].hour = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].mday = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].month = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].year = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].s1 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].s2 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i1 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i2 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i3 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i4 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i5 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i6 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i7 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i8 = 0;
                SAV_gst_Data[INO_gst_SavSlotDesc.id].i9 = 0;
            }
            INO_SavSetCardStatus(a1, 10);
            pcINO_gi_LastError = 0;
            INO_ge_SavSetNextNextState = 34;
            return;
        case 30:
            INO_gi_FreeSpace = 0;
        LABEL_9:
            INO_ge_SavCommand = a2;
            goto LABEL_10;
        default:
        LABEL_10:
            pcINO_gi_LastError = 0;
            INO_ge_SavState = a2;
            break;
        }
    }
}

void* SAV_PhotoReinit()
{
    void* result;
    char* v1;

    v1 = SAV_gst_Photos.photoBuf;
    result = memset(&SAV_gst_Photos, 0, 812u);
    SAV_gst_Photos.photoBufSize = 409600;
    SAV_gst_Photos.photoBuf = v1;
    return result;
}

void SAV_PhotoReinitValidate()
{
    pcINO_SAVPhotosSize = 409600;
    SAV_gst_PhotosValidate.photoBuf = SAV_pc_PhotoValidateBuffer;
}

void SAV_PhotoClose()
{
    if (Photo_gi_Init)
    {
        Photo_gi_Init = 0;
        SAV_PhotoReinit();
        SAV_PhotoReinitValidate();
        MEM_Free(SAV_gst_Photos.photoBuf);
        SAV_gst_Photos.photoBuf = 0;
    }
}

void SAV_PhotoLastClose()
{
    if (Photo_gi_FirstInit)
    {
        SAV_PhotoClose();
        SAV_PhotoReinit();
        MEM_Free(SAV_pc_PhotoValidateBuffer);
        Photo_gi_FirstInit = 0;
    }
}

void SAV_PhotoSet(int a1, void* Src, size_t Size)
{
    int v3;
    int i;
    int v5;

    SAV_PhotoInit();
    if (a1 >= 0 && a1 < 100)
    {
        v5 = SAV_gst_Photos.elems[a1].photoSize;
        if (Size != v5)
        {
            if (Size - v5 + SAV_gst_Photos.usedSize > SAV_gst_Photos.photoBufSize)
                return;
            v3 = SAV_gst_Photos.elems[a1 + 1].startOffset;
            for (i = 99; i > a1; --i)
                SAV_gst_Photos.elems[i].startOffset += Size - v5;
            memcpy(&SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[a1 + 1].startOffset], &SAV_gst_Photos.photoBuf[v3], SAV_gst_Photos.usedSize - v3);
        }
        SAV_gst_Photos.elems[a1].photoSize = Size;
        memcpy(&SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[a1].startOffset], Src, Size);
        SAV_gst_Photos.usedSize += Size - v5;
    }
}

void SAV_PhotoDel(int a1)
{
    int v1;
    int v2;
    int v3;
    signed int Size;
    int Sizea;
    int v6;

    SAV_PhotoInit();
    if (a1 < 1000)
    {
        if (a1 >= 0 && a1 < 100)
        {
            v3 = SAV_gst_Photos.elems[a1].photoSize;
            if (v3)
            {
                for (Sizea = a1; Sizea < 99; ++Sizea)
                {
                    SAV_gst_Photos.elems[Sizea].photoSize = SAV_gst_Photos.elems[Sizea + 1].photoSize;
                    SAV_gst_Photos.elems[Sizea].startOffset = SAV_gst_Photos.elems[Sizea + 1].startOffset - v3;
                    if (SAV_gst_Photos.elems[Sizea + 1].photoSize)
                    {
                        memcpy(
                            &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[Sizea].startOffset],
                            &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[Sizea + 1].startOffset],
                            SAV_gst_Photos.elems[Sizea].photoSize);
                        SAV_gst_Photos.elems[Sizea + 1].photoSize = 0;
                    }
                }
                SAV_gst_Photos.elems[99].photoSize = 0;
                SAV_gst_Photos.elems[99].startOffset = 0;
                SAV_gst_Photos.usedSize -= v3;
            }
        }
    }
    else
    {
        v6 = a1 - 1000;
        if (v6 >= 0 && v6 < 100)
        {
            v2 = SAV_gst_Photos.elems[v6].photoSize;
            if (v2)
            {
                SAV_gst_Photos.elems[v6].photoSize = 0;
                v1 = v2 + SAV_gst_Photos.elems[v6].startOffset;
                if (SAV_gst_Photos.usedSize != v1)
                    memcpy(
                        &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[v6].startOffset],
                        &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[v6].startOffset + v2],
                        SAV_gst_Photos.usedSize - v1);
                SAV_gst_Photos.usedSize -= v2;
                for (Size = v6 + 1; Size < 100; ++Size)
                    SAV_gst_Photos.elems[Size].startOffset -= v2;
            }
        }
    }
}

void SAV_PhotoDecal(int a1, int a2)
{
    int v2;
    int v3;
    int i;
    int v5;

    SAV_PhotoInit();
    if (a1 >= 1000)
    {
        v5 = a1 - 1000;
        if (v5 >= 0 && v5 < 100 && a2 >= 0 && a2 < 100)
        {
            v3 = v5;
            for (i = v5; i < a2 + v5; ++i)
            {
                if (SAV_gst_Photos.elems[i].photoSize)
                {
                    if (v3 != i)
                    {
                        SAV_gst_Photos.elems[v3].photoSize = SAV_gst_Photos.elems[i].photoSize;
                        SAV_gst_Photos.elems[v3].startOffset = SAV_gst_Photos.elems[i].startOffset;
                    }
                    ++v3;
                }
            }
            if (v3) {
                v2 = SAV_gst_Photos.elems[v3 - 1].photoSize + SAV_gst_Photos.elems[v3 - 1].startOffset; // TODO FIXME, ez igy jó?
            }
            else
                v2 = 0;
            while (v3 < a2 + v5)
            {
                SAV_gst_Photos.elems[v3].photoSize = 0;
                SAV_gst_Photos.elems[v3++].startOffset = v2;
            }
        }
    }
}

char* SAV_PhotoGet(int a1, int32_t* a2)
{
    SAV_PhotoInit();
    if (a1 < 0 || a1 >= 100)
        return 0;
    *a2 = SAV_gst_Photos.elems[a1].photoSize;
    return &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[a1].startOffset];
}

void SAV_PhotoValidate()
{
    SAV_PhotoInit();
    SAV_PhotoReinitValidate();
    memcpy(&SAV_gst_PhotosValidate.elems, &SAV_gst_Photos.elems, sizeof(SAV_gst_Photos.elems));
    SAV_gst_PhotosValidate.usedSize = SAV_gst_Photos.usedSize;
    if (SAV_gst_Photos.usedSize)
        memcpy(SAV_gst_PhotosValidate.photoBuf, SAV_gst_Photos.photoBuf, SAV_gst_PhotosValidate.usedSize);
}

void SAV_PhotoUnvalidate()
{
    SAV_PhotoInit();
    SAV_PhotoReinit();
    memcpy(&SAV_gst_Photos, &SAV_gst_PhotosValidate, sizeof(SAV_gst_PhotosValidate.elems));
    SAV_gst_Photos.usedSize = SAV_gst_PhotosValidate.usedSize;
    if (SAV_gst_PhotosValidate.usedSize)
        memcpy(SAV_gst_Photos.photoBuf, SAV_gst_PhotosValidate.photoBuf, SAV_gst_PhotosValidate.usedSize);
}

void SAV_LoadData(int a1)
{
    INO_gb_ReadIdxForSaving = a1 != 0;
    INO_SavSetCardStatus(SAVE_gi_DefaultSlot, 7);
}

int Demo_Geti(int a1)
{
    int result;
    int v2;

    switch (a1)
    {
    case 1:
        result = 0;
        break;
    case 2:
        result = 0;
        break;
    case 3:
        result = 0;
        break;
    case 4:
        result = 60;
        break;
    case 5:
        result = 240;
        break;
    case 6:
        v2 = Demo_gb_Exit;
        Demo_gb_Exit = 0;
        result = v2;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

int Demo_Seti(int a1)
{
    if (a1 <= 202)
    {
        if (a1 == 202)
        {
            INO_gi_LaunchDemo = 2;
            ENG_gb_ExitApplication = 1;
            ENG_gb_ForceEndEngine = 1;
            return 0;
        }
        if (a1 != 6)
        {
            if (a1 == 200)
            {
                INO_gi_LaunchDemo = 0;
                ENG_gb_ExitApplication = 1;
                ENG_gb_ForceEndEngine = 1;
                return 0;
            }
            if (a1 == 201)
            {
                INO_gi_LaunchDemo = 1;
                ENG_gb_ExitApplication = 1;
                ENG_gb_ForceEndEngine = 1;
                return 0;
            }
            return -1;
        }
    LABEL_9:
        ENG_gb_ExitApplication = 1;
        ENG_gb_ForceEndEngine = 1;
        return 0;
    }
    if (a1 == 203)
    {
        INO_gi_LaunchDemo = 3;
        ENG_gb_ExitApplication = 1;
        ENG_gb_ForceEndEngine = 1;
        return 0;
    }
    if (a1 == 666)
        goto LABEL_9;
    return -1;
}

int INO_i_IsMouseBeingUsed() {
    return pcINO_i_IsMouseBeingUsed();
}
