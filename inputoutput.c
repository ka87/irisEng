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

void INO_Joystick_Init(HWND hwnd)
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
    s_JoystickInit(hwnd);
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
    INO_Keys = (uint8_t*)MEM_p_Alloc(256u);
    INO_Keys_Prev = (uint8_t*)MEM_p_Alloc(256u);
    memset(INO_keybUnkDataProbablyUnused, 0, 100u);
}

void INO_InitModule()
{
    INO_gi_DiscError = 0;
    INO_KeyboardInit();
    INO_SavInitModule();
}

int INO_i_SetCurrentPad(int pad)
{
    return s_SetCurrentPad(pad);
}

int INO_i_GetCurrentPad()
{
    return INO_gi_CurrentPadId;
}

int INO_SimulateReset()
{
    memset(&INO_gst_JoySimulateAsked, 0, sizeof(INO_gst_JoySimulateAsked));
    memset(&INO_gst_JoySimulateCurrent, 0, sizeof(INO_gst_JoySimulateCurrent));
    return 0;
}

void INO_Joystick_Close(void)
{
    s_JoystickClose();
}

void INO_Joystick_Update()
{
    float remaining;
    int i;

    if (!INO_l_Joystick_Enable)
        return;

    memcpy(&INO_gst_JoySimulateCurrent, &INO_gst_JoySimulateAsked, sizeof(INO_gst_JoySimulateCurrent));
    memset(&INO_gst_JoySimulateAsked, 0, sizeof(INO_gst_JoySimulateAsked));
    memcpy(INO_gst_PrevPad, INO_gst_CurrPad, sizeof(INO_gst_PrevPad));

    s_JoystickUpdate(INO_gst_CurrPad);

    if (!Demo_gb_Exit)
    {
        for (i = 0; i < 4; i++)
        {
            if (INO_gst_CurrPad[i].padData[8] && INO_gst_CurrPad[i].padData[9])
            {
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
            for (i = 0; i < 12; i++)
            {
                if (((1 << i) & INO_gst_JoySimulateCurrent.controlsMask) != 0)
                    INO_gst_CurrPad[INO_gi_CurrentPadId].padData[i] =
                        ((1 << i) & INO_gst_JoySimulateCurrent.buttons) != 0 ? 0xFF : 0;
            }
        }
    }

    if (s_IsMouseBeingUsedFunction())
    {
        if (INO_gi_PifDuration != 0.0f)
        {
            remaining = INO_gi_PifDuration - TIM_gf_dt;
            INO_gi_PifDuration = remaining;
            if (remaining <= 0.0f)
            {
                s_JoystickPifPafEnable(0);
                INO_gi_PifDuration = 0.0f;
            }
        }
        if (INO_gi_PafDuration != 0.0f)
        {
            remaining = INO_gi_PafDuration - TIM_gf_dt;
            INO_gi_PafDuration = remaining;
            if (remaining <= 0.0f)
            {
                s_JoystickPafIntensity(0);
                INO_gi_PafDuration = 0.0f;
            }
        }
    }
}

int INO_i_PifPafEnable(int enable)
{
    if (enable != -1)
    {
        if (enable)
        {
            INO_gi_PifPafEnable = 1;
            return 1;
        }
        INO_gi_PifPafEnable = 0;
        INO_gi_PifDuration = 0.0;
        INO_gi_PafDuration = 0.0;
        s_JoystickPifPafEnable(0);
        s_JoystickPafIntensity(0);
    }
    return INO_gi_PifPafEnable;
}

void INO_PifSet(int duration)
{
    if (INO_gi_PifPafEnable)
    {
        INO_gi_PifDuration = (float)((double)(duration < 0 ? -duration : duration) * 0.032000002);
        s_JoystickPifPafEnable(duration != 0);
    }
}

void INO_PafSet(int intensity, int duration)
{
    int clamped;

    if (INO_gi_PifPafEnable)
    {
        INO_gi_PafDuration = (float)((double)(duration < 0 ? -duration : duration) * 0.032000002);
        // preserved: clamped above but not below, so a negative intensity goes through
        clamped = intensity > 255 ? 255 : intensity;
        INO_gi_PafIntensity = clamped;
        s_JoystickPafIntensity(clamped);
    }
}

int INO_b_Joystick_IsButtonDown(int button)
{
    if (INO_gi_CurrentPadId == -1)
        return 0;
    return INO_gst_CurrPad[INO_gi_CurrentPadId].padData[button];
}

BOOL INO_b_Joystick_IsButtonJustDown(int button)
{
    return INO_gi_CurrentPadId != -1
        && INO_gst_CurrPad[INO_gi_CurrentPadId].padData[button]
        && !INO_gst_PrevPad[INO_gi_CurrentPadId].padData[button];
}

BOOL INO_b_Joystick_IsButtonJustUp(int button)
{
    return INO_gi_CurrentPadId != -1
        && !INO_gst_CurrPad[INO_gi_CurrentPadId].padData[button]
        && INO_gst_PrevPad[INO_gi_CurrentPadId].padData[button];
}

static void INO_l_ApplyJoystickDir(Vec3* move)
{
    if ((INO_l_JoystickDir & 1) != 0)
        move->x = -move->x;
    if ((INO_l_JoystickDir & 2) != 0)
        move->y = -move->y;
}

void INO_Joystick_Move(Vec3* move, int stickID)
{
    double amountX;
    double amountY;
    int stickBit;

    if (INO_l_Joystick_Enable && INO_gi_CurrentPadId != -1)
    {
        stickBit = 1 << (stickID + 24);
        if ((stickBit & INO_gst_JoySimulateCurrent.controlsMask) != 0)
        {
            if (stickID)
            {
                amountX = INO_gst_JoySimulateCurrent.amountXRight;
                amountY = INO_gst_JoySimulateCurrent.amountYRight;
            }
            else
            {
                amountX = INO_gst_JoySimulateCurrent.amountXLeft;
                amountY = INO_gst_JoySimulateCurrent.amountYLeft;
            }
            move->x = (float)amountX;
            move->y = (float)amountY;
            move->z = 0.0;
            INO_l_ApplyJoystickDir(move);
            return;
        }
        if ((stickBit & INO_l_JoystickMask) != 0)
        {
            s_joyMoveFunction(move, stickID);
            INO_l_ApplyJoystickDir(move);
            return;
        }
    }
    move->x = 0.0;
    move->y = 0.0;
    move->z = 0.0;
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

void INO_SetControlSubmode(int submode)
{
    pcINO_SetControlSubmode(submode);
}

void INO_SetControlMode(int mode)
{
    pcINO_SetControlMode(mode);
}

/* Mouse sensitivity, 0..1 from the options slider. */
void INO_SetMouseSensitivity(float sensitivity)
{
    pcINO_SetMouseSensitivity(sensitivity);
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
    uint8_t* keys;
    int i;

    keys = INO_Keys_Prev;
    INO_Keys_Prev = INO_Keys;
    memset(keys, 0, 256u);
    INO_Keys = keys;
    if (pcINO_keyboardDataBackup[157])
    {
        for (i = 0; i < 10; ++i)
        {
            if (pcINO_keyboardDataBackup[i + 59])
            {
                keys[164] = 0x80;
                keys[i + 96] = 0x80;
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

BOOL INO_b_LanguageIsPresent(int lang)
{
    return lang < 11
        && lang >= 0
        && (INO_ge_MasterType == 1 || INO_ge_MasterType != 2 || lang <= 1 || lang == 3)
        && INO_gai_PresentLanguage[lang];
}

int INO_i_SavGetCardStatus(int slot, int* savButtons)
{
    if (savButtons)
        memcpy(savButtons, INO_gi_SavButton, sizeof(INO_gi_SavButton));

    if (INO_ge_SavState != 27 && INO_ge_SavState)
        return -1;

    if (INO_ge_SavSetNextNextState != -1 && !pcINO_gi_LastError)
    {
        INO_ge_SavState = INO_ge_SavSetNextNextState;
        INO_ge_SavSetNextNextState = -1;
        return -1;
    }
    return pcINO_gi_LastError;
}

int INO_b_IsKeyPressed(uint8_t key)
{
    return INO_Keys[key] & 0x80;
}

BOOL INO_b_IsKeyJustPressed(uint8_t key)
{
    return (INO_Keys[key] & 0x80) != 0 && (INO_Keys_Prev[key] & 0x80) == 0;
}

BOOL INO_b_IsKeyReleased(uint8_t key)
{
    return (INO_Keys[key] & 0x80) == 0;
}

BOOL INO_b_IsKeyJustReleased(uint8_t key)
{
    return (INO_Keys[key] & 0x80) == 0 && (INO_Keys_Prev[key] & 0x80) != 0;
}

int INO_i_GetDiscError()
{
    return INO_gi_DiscError;
}

int INO_b_IoButtonPressed(int button)
{
    return INO_b_Joystick_IsButtonDown(button) ? 0xFF : 0;
}

void SAV_PhotoFirstInit()
{
    if (!Photo_gi_FirstInit)
    {
        SAV_pc_PhotoValidateBuffer = (char*)MEM_p_Alloc(SAV_PHOTO_BUFFER_SIZE);
        Photo_gi_FirstInit = 1;
    }
}

void SAV_PhotoInit()
{
    char* photoBufPtrBackup;

    if (!Photo_gi_Init)
    {
        // preserved: the saved pointer is put back and then dropped by the alloc below
        photoBufPtrBackup = SAV_gst_Photos.photoBuf;
        memset(&SAV_gst_Photos, 0, sizeof(SAV_gst_Photos));
        SAV_gst_Photos.photoBufSize = SAV_PHOTO_BUFFER_SIZE;
        SAV_gst_Photos.photoBuf = photoBufPtrBackup;
        SAV_gst_Photos.photoBuf = (char*)MEM_p_Alloc(SAV_PHOTO_BUFFER_SIZE);
        SAV_gst_PhotosValidate.photoBufSize = SAV_PHOTO_BUFFER_SIZE;
        SAV_gst_PhotosValidate.photoBuf = SAV_pc_PhotoValidateBuffer;
        Photo_gi_Init = 1;
    }
}

BOOL SAV_b_IsMoreRecent(int slotA, int slotB)
{
    if (!SAV_gst_Data[slotA].superiorAtOrdering)
    {
        if (SAV_gst_Data[slotB].superiorAtOrdering)
            return 0;
        return slotA < slotB;
    }
    if (!SAV_gst_Data[slotB].superiorAtOrdering)
        return 1;

    if (SAV_gst_Data[slotA].year != SAV_gst_Data[slotB].year)
        return SAV_gst_Data[slotA].year > SAV_gst_Data[slotB].year;
    if (SAV_gst_Data[slotA].month != SAV_gst_Data[slotB].month)
        return SAV_gst_Data[slotA].month > SAV_gst_Data[slotB].month;
    if (SAV_gst_Data[slotA].mday != SAV_gst_Data[slotB].mday)
        return SAV_gst_Data[slotA].mday > SAV_gst_Data[slotB].mday;
    if (SAV_gst_Data[slotA].hour != SAV_gst_Data[slotB].hour)
        return SAV_gst_Data[slotA].hour > SAV_gst_Data[slotB].hour;
    if (SAV_gst_Data[slotA].min != SAV_gst_Data[slotB].min)
        return SAV_gst_Data[slotA].min > SAV_gst_Data[slotB].min;
    return SAV_gst_Data[slotA].sec >= SAV_gst_Data[slotB].sec;
}

void SAV_OrderData()
{
    int order[10];
    int i;
    int j;
    int k;

    for (i = 0; i < 10; ++i)
        order[i] = i;
    for (i = 1; i < 10; ++i)
    {
        for (j = 0; j < i && !SAV_b_IsMoreRecent(i, order[j]); ++j);
        for (k = i - 1; k >= j; --k)
            order[k + 1] = order[k];
        order[j] = i;
    }
    for (i = 0; i < 10; ++i)
    {
        SAV_gst_Data[i].s4 = order[i];
        SAV_gst_Data[order[i]].s3 = i;
    }
}

static void INO_l_RunCardOp(int slot)
{
    INO_ge_SavState = INO_gst_MemoryCardManager[INO_ge_SavState + 1](slot);
}

void INO_v_MemoryCardManager(int defaultSlot)
{
    tSavIdxElem* entry;
    struct tm* lt;
    time_t now;

    if (INO_gst_MemoryCardManager[0](defaultSlot))
        return;

    switch (INO_ge_SavState)
    {
    case 0:
    case 27:
        return;

    case 1:
        pcINO_gi_LastError = 0;
        INO_l_RunCardOp(defaultSlot);
        break;

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
        INO_l_RunCardOp(defaultSlot);
        break;

    case 7:
        INO_ge_SavState = 21;
        INO_ge_SavSetNextState = 8;
        return;

    case 8:
        memset(SAV_gst_Data, 0, sizeof(SAV_gst_Data));
    case 11:
        if (!INO_gst_SavFileDesc.fp)
        {
            INO_ge_SavState = 0;
            break;
        }
        INO_gst_SavFileDesc.size = 640;
        INO_gst_SavFileDesc.saveBuffer = (char*)SAV_gst_Data;
        INO_l_RunCardOp(defaultSlot);
        break;

    case 9:
        SAV_OrderData();
        INO_ge_SavState = 22;
        return;

    case 10:
        INO_ge_SavState = 21;
        INO_ge_SavSetNextState = 11;
        return;

    case 12:
        INO_ge_SavState = 21;
        INO_ge_SavSetNextState = 13;
        return;

    case 13:
        if (!INO_gst_SavFileDesc.fp)
        {
            INO_ge_SavState = 0;
            break;
        }
        INO_gst_SavFileDesc.size = AI_gi_SaveBufferSize;
        INO_gst_SavFileDesc.saveBuffer = (char*)MEM_p_Alloc(AI_gi_SaveBufferSize);
        memset(INO_gst_SavFileDesc.saveBuffer, 0, INO_gst_SavFileDesc.size);
        INO_l_RunCardOp(defaultSlot);
        break;

    case 14:
        memcpy(AI_gpc_SaveBuffer, INO_gst_SavFileDesc.saveBuffer, AI_gi_SaveBufferSize);
        MEM_Free(INO_gst_SavFileDesc.saveBuffer);
        SAV_PhotoInit();
        pc_Buffer_304 = SAV_gst_PhotosValidate.photoBuf;
        INO_gst_SavFileDesc.size = 812;
        INO_gst_SavFileDesc.saveBuffer = (char*)&SAV_gst_PhotosValidate;
        INO_l_RunCardOp(defaultSlot);
        return;

    case 15:
        SAV_gst_PhotosValidate.photoBuf = pc_Buffer_304;
        pc_Buffer_304 = 0;
        /* usedSize arrives from the slot file, and the read below
           is a bare fread into a fixed buffer. Clamp it so a
           truncated or foreign file cannot run off the end. */
        if (SAV_gst_PhotosValidate.usedSize < 0
            || SAV_gst_PhotosValidate.usedSize > SAV_PHOTO_BUFFER_SIZE)
        {
            DBG_OutputDebugString(
                "SAV: photo block %d outside 0..%d, clamped\n",
                SAV_gst_PhotosValidate.usedSize, SAV_PHOTO_BUFFER_SIZE);
            SAV_gst_PhotosValidate.usedSize =
                SAV_gst_PhotosValidate.usedSize < 0 ? 0 : SAV_PHOTO_BUFFER_SIZE;
        }
        SAV_gst_PhotosValidate.photoBufSize = SAV_gst_PhotosValidate.usedSize;
        INO_gst_SavFileDesc.size = SAV_gst_PhotosValidate.usedSize;
        INO_gst_SavFileDesc.saveBuffer = SAV_gst_PhotosValidate.photoBuf;
        INO_l_RunCardOp(defaultSlot);
        break;

    case 16:
        INO_ge_SavState = 21;
        INO_ge_SavSetNextState = 17;
        return;

    case 17:
        if (!INO_gst_SavFileDesc.fp)
        {
            INO_ge_SavState = 0;
            break;
        }
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
        INO_ge_SavState = 0;
        break;

    case 28:
        if (SAV_gst_PhotosValidate.usedSize)
        {
            INO_gst_SavFileDesc.saveBuffer = SAV_gst_PhotosValidate.photoBuf;
            INO_gst_SavFileDesc.size = SAV_gst_PhotosValidate.usedSize;
            INO_l_RunCardOp(defaultSlot);
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
            time(&now);
            lt = localtime(&now);
            entry = &SAV_gst_Data[INO_gst_SavSlotDesc.id];
            entry->superiorAtOrdering = 1;
            entry->sec = lt->tm_sec;
            entry->min = lt->tm_min;
            entry->hour = lt->tm_hour;
            entry->mday = lt->tm_mday;
            entry->month = lt->tm_mon;
            entry->year = lt->tm_year + 1900;
            entry->s1 = INO_gst_SavSlotDesc.i2;
            entry->s2 = (int16_t)INO_gst_SavSlotDesc.i3;
            entry->i1 = INO_gst_SavSlotDesc.i4;
            entry->i2 = SAV_idx_Field_i2;
            entry->i3 = SAV_idx_Field_i3;
            entry->i4 = SAV_idx_Field_i4;
            entry->i5 = SAV_idx_Field_i5;
            entry->i6 = SAV_idx_Field_i6;
            entry->i7 = SAV_idx_Field_i7;
            entry->i8 = SAV_idx_Field_i8;
            entry->i9 = SAV_idx_Field_i9;
            strcpy(INO_gsz_SavFileName, "sally.idx");
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
        INO_ge_SavState = 0;
        break;
    }
}

void INO_Update()
{
    INO_Joystick_Update();
    INO_Keyboard_Update();
    INO_v_MemoryCardManager(SAVE_gi_DefaultSlot);
}

void INO_SavSetCardStatus(int slot, int status)
{
    tSavIdxElem* entry;

    if (status < 0 || status >= 41)
    {
        DBG_OutputDebugString(
            "SAV: bad param state, requested %d (slot %d)\n", status, slot);
        DBG_assert(0, __FILE__, __LINE__, "0", "SAVING : Bad Param State", 0, 1);
    }
    if (INO_ge_SavState && INO_ge_SavState != 27)
    {
        /* The machine is mid-operation, so the request is dropped and
           whatever is running carries on. Logged rather than asserted:
           the caller is a script and the race is recoverable, and
           killing the process here takes the running operation with
           it -- an index read abandoned halfway leaves the slot list
           empty and its file handle open.

           Everything needed to place the request: where the machine
           was, what it was doing, and what the last operation said. */
        DBG_OutputDebugString(
            "SAV: request %d dropped, busy: state=%d command=%d "
            "nextState=%d nextNextState=%d lastError=%d\n",
            status, INO_ge_SavState, INO_ge_SavCommand,
            INO_ge_SavSetNextState, INO_ge_SavSetNextNextState,
            pcINO_gi_LastError);
        return;
    }

    if (INO_ge_SavState == status)
        return;

    switch (status)
    {
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
            entry = &SAV_gst_Data[INO_gst_SavSlotDesc.id];
            entry->superiorAtOrdering = 0;
            entry->sec = 0;
            entry->min = 0;
            entry->hour = 0;
            entry->mday = 0;
            entry->month = 0;
            entry->year = 0;
            entry->s1 = 0;
            entry->s2 = 0;
            entry->i1 = 0;
            entry->i2 = 0;
            entry->i3 = 0;
            entry->i4 = 0;
            entry->i5 = 0;
            entry->i6 = 0;
            entry->i7 = 0;
            entry->i8 = 0;
            entry->i9 = 0;
        }
        INO_SavSetCardStatus(slot, 10);
        pcINO_gi_LastError = 0;
        INO_ge_SavSetNextNextState = 34;
        return;

    case 30:
        INO_gi_FreeSpace = 0;
    case 1:
        INO_ge_SavCommand = status;
    default:
        pcINO_gi_LastError = 0;
        INO_ge_SavState = status;
        break;
    }
}

void* SAV_PhotoReinit()
{
    void* result;
    char* photoBuf;

    photoBuf = SAV_gst_Photos.photoBuf;
    result = memset(&SAV_gst_Photos, 0, 812u);
    SAV_gst_Photos.photoBufSize = SAV_PHOTO_BUFFER_SIZE;
    SAV_gst_Photos.photoBuf = photoBuf;
    return result;
}

void SAV_PhotoReinitValidate()
{
    pcINO_SAVPhotosSize = SAV_PHOTO_BUFFER_SIZE;
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

void SAV_PhotoSet(int photo, void* src, size_t size)
{
    int oldSize;
    int nextStart;
    int i;

    SAV_PhotoInit();
    if (photo < 0 || photo >= 100)
        return;

    oldSize = SAV_gst_Photos.elems[photo].photoSize;
    if (size != oldSize)
    {
        if (size - oldSize + SAV_gst_Photos.usedSize > SAV_gst_Photos.photoBufSize)
            return;
        // preserved: elems[photo + 1] reads past the array for photo 99, where it lands on photoBufSize
        nextStart = SAV_gst_Photos.elems[photo + 1].startOffset;
        for (i = 99; i > photo; --i)
            SAV_gst_Photos.elems[i].startOffset += size - oldSize;
        memcpy(&SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[photo + 1].startOffset],
               &SAV_gst_Photos.photoBuf[nextStart],
               SAV_gst_Photos.usedSize - nextStart);
    }
    SAV_gst_Photos.elems[photo].photoSize = size;
    memcpy(&SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[photo].startOffset], src, size);
    SAV_gst_Photos.usedSize += size - oldSize;
}

void SAV_PhotoDel(int photo)
{
    int size;
    int end;
    int index;
    int i;

    SAV_PhotoInit();
    if (photo < 1000)
    {
        if (photo < 0 || photo >= 100)
            return;

        size = SAV_gst_Photos.elems[photo].photoSize;
        if (!size)
            return;

        for (i = photo; i < 99; ++i)
        {
            SAV_gst_Photos.elems[i].photoSize = SAV_gst_Photos.elems[i + 1].photoSize;
            SAV_gst_Photos.elems[i].startOffset = SAV_gst_Photos.elems[i + 1].startOffset - size;
            if (SAV_gst_Photos.elems[i + 1].photoSize)
            {
                memcpy(
                    &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[i].startOffset],
                    &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[i + 1].startOffset],
                    SAV_gst_Photos.elems[i].photoSize);
                SAV_gst_Photos.elems[i + 1].photoSize = 0;
            }
        }
        SAV_gst_Photos.elems[99].photoSize = 0;
        SAV_gst_Photos.elems[99].startOffset = 0;
        SAV_gst_Photos.usedSize -= size;
        return;
    }

    index = photo - 1000;
    if (index < 0 || index >= 100)
        return;

    size = SAV_gst_Photos.elems[index].photoSize;
    if (!size)
        return;

    SAV_gst_Photos.elems[index].photoSize = 0;
    end = size + SAV_gst_Photos.elems[index].startOffset;
    if (SAV_gst_Photos.usedSize != end)
        memcpy(
            &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[index].startOffset],
            &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[index].startOffset + size],
            SAV_gst_Photos.usedSize - end);
    SAV_gst_Photos.usedSize -= size;
    for (i = index + 1; i < 100; ++i)
        SAV_gst_Photos.elems[i].startOffset -= size;
}

void SAV_PhotoDecal(int photo, int count)
{
    int first;
    int dst;
    int end;
    int i;

    SAV_PhotoInit();
    if (photo < 1000)
        return;

    first = photo - 1000;
    // preserved: first and count are bounded apart, so first + count may run past the 100 entries
    if (first < 0 || first >= 100 || count < 0 || count >= 100)
        return;

    dst = first;
    for (i = first; i < count + first; ++i)
    {
        if (SAV_gst_Photos.elems[i].photoSize)
        {
            if (dst != i)
            {
                SAV_gst_Photos.elems[dst].photoSize = SAV_gst_Photos.elems[i].photoSize;
                SAV_gst_Photos.elems[dst].startOffset = SAV_gst_Photos.elems[i].startOffset;
            }
            ++dst;
        }
    }
    if (dst) {
        end = SAV_gst_Photos.elems[dst - 1].photoSize + SAV_gst_Photos.elems[dst - 1].startOffset; // TODO FIXME, ez igy jó?
    }
    else
        end = 0;
    while (dst < count + first)
    {
        SAV_gst_Photos.elems[dst].photoSize = 0;
        SAV_gst_Photos.elems[dst++].startOffset = end;
    }
}

char* SAV_PhotoGet(int photo, int32_t* size)
{
    SAV_PhotoInit();
    if (photo < 0 || photo >= 100)
        return 0;
    *size = SAV_gst_Photos.elems[photo].photoSize;
    return &SAV_gst_Photos.photoBuf[SAV_gst_Photos.elems[photo].startOffset];
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

void SAV_LoadData(int forSaving)
{
    INO_gb_ReadIdxForSaving = forSaving != 0;
    INO_SavSetCardStatus(SAVE_gi_DefaultSlot, 7);
}

int Demo_Geti(int field)
{
    int exitAsked;

    switch (field)
    {
    case 1:
    case 2:
    case 3:
        return 0;
    case 4:
        return 60;
    case 5:
        return 240;
    case 6:
        exitAsked = Demo_gb_Exit;
        Demo_gb_Exit = 0;
        return exitAsked;
    default:
        return -1;
    }
}

int Demo_Seti(int field)
{
    switch (field)
    {
    case 200:
        INO_gi_LaunchDemo = 0;
        break;
    case 201:
        INO_gi_LaunchDemo = 1;
        break;
    case 202:
        INO_gi_LaunchDemo = 2;
        break;
    case 203:
        INO_gi_LaunchDemo = 3;
        break;
    case 6:
    case 666:
        break;
    default:
        return -1;
    }
    ENG_gb_ExitApplication = 1;
    ENG_gb_ForceEndEngine = 1;
    return 0;
}

int INO_i_IsMouseBeingUsed() {
    return pcINO_i_IsMouseBeingUsed();
}
