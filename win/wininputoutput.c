#include "wininputoutput.h"
#include "../inputoutput.h"
#include "../math.h"
#include "../time.h"
#include "../memory.h"
#include "../dbg.h"

int pcINO_SelectedControlMode = 0xffffffff;
int pcINO_SelectedControlSubmode = 0xffffffff;
Vec3 pcINO_joyPositionBeforeControlSubmodeSet = { 1.0f, 0.0f, 0.0f };

char prefixSpaces[12] = "\\P4\\\x8F     \x90\0";
char prefixOneSpace[8] = "\\P4\\\x8F \x90\0";
char prefix_p16_2[7] = "\\p16\\2\0";

unsigned char commandWindowDlgTemplate[88] = {
    0x80, 0x02, 0x00, 0x90, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xC8, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x81, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xBA, 0x00, 0x0C, 0x00, 0x01, 0x00, 0xFF, 0xFF, 0x81, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x00, 0x00, 0x00, 0x00,
    0xBC, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x01, 0x00, 0xFF, 0xFF,
    0x80, 0x00, 0x4F, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};


// TODO FIXME: ezek mik?? a játék nem 0ázza õket
int dword_990164 = 0;
int dword_990168 = 0;
int dword_99016C = 0;
int dword_990170 = 0;
int dword_990174 = 0;
int dword_990178 = 0;

/*
IDirectInput8A* pDI;
HRESULT hr;


int test()
{
    hr = DirectInput8Create(GetModuleHandleA(0), DIRECTINPUT_VERSION, &IID_IDirectInput8A, (void**)&pDI, NULL);
    if (FAILED(hr))
    {        
        return 0;
    }
}
*/

uint16_t pcINO_IntKeybDataLink[36] = { 258,  0, 259, 0, 16, 211, 18, 207,
                                        42, 82, 46, 76, 57, 157, 29, 221,
                                         3, 201, 4, 209, 17, 200, 31, 208,
                                        30, 203, 32, 205, 15, 14, 1, 197,
                                       259, 1, 57, 260 };

uint8_t pcINO_keyMapsCase1[24] = { 0, 2, 1, 3, 7, 7, 4, 6, 8, 0xFE,
                                   9, 0xFE, 0x0F, 0x0E, 0x0C, 0x0A, 
                                   0x0D, 0x0B, 0xFF, 0xFF, 0xFF, 0xFF, 
                                   0, 0 };

uint8_t pcINO_keyMapsCase2[24] = { 0, 1, 0xFE, 3, 0xFE, 0xFE, 4, 6, 0xFE, 
                                   0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0xFF, 0xFF, 
                                   0xFF, 0xFF, 0x0C, 0x0A, 0x0D, 0x0B, 
                                   0, 0 };

uint8_t pcINO_keyMapsCase64[24] = { 0, 2, 1, 3, 0xFE, 0xFE, 4, 6, 8, 0xFE,
                                    9, 0xFE, 0x0F, 0x0E, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0x0C, 0x0A, 0x0D, 0x0B, 0, 0 };

uint8_t pcINO_keyMapsCase32[24] = { 0, 2, 1, 3, 0xFE, 0xFE, 4, 6, 8, 0xFE,
                                    9, 0xFE, 0x0F, 0x0E, 0x0C, 0x0A, 0x0D, 
                                    0x0B, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 };

uint8_t pcINO_keyMapsCase8[24] = { 0, 0x10, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
                                   0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0x0C, 
                                   0x0A, 0x0D, 0x0B, 0xFE, 0xFE, 0xFE, 0xFE, 
                                   0, 0 };

uint8_t pcINO_keyMapsCase100[24] = { 0, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 
                                     0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 
                                     0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 
                                     0xFE, 0, 0 };

uint8_t pcINO_keyMapsCase101[24] = { 0, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
                                     0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 
                                     0xFF, 0xFF, 0xFF, 0xFF,  0xFE, 0xFE, 0xFE, 
                                     0xFE, 0, 0 };

uint8_t pcINO_InputHandlerSubModeCaseDef[24] = { 0, 0x11, 0x0FE, 0x10, 0xFE, 0xFE, 0xFE,
                                                 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E,
                                                 0x0C, 0x0A, 0x0D, 0x0B, 0xFE, 0xFE, 0xFE,
                                                 0xFE, 0, 0 };

uint8_t pcINO_InputHandlerSubModeCase0[24] = { 0, 0x11, 0xFE, 0x10, 0xFE, 0xFE, 0xFE, 0xFE,
                                               0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0xFF,0xFF,
                                               0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE, 0, 0 };

uint8_t pcINO_InputHandlerSubModeCase2[24] = { 0, 0xFE, 0xFE, 0x10, 0xFE, 0xFE, 0x11, 0xFE, 
                                               0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0x0C, 0x0A, 
                                               0x0D, 0x0B, 0x0FE, 0x0C, 0xFE, 0x0D, 0, 0 };

uint8_t pcINO_InputHandlerSubModeCase7[24] = { 0, 0xFE, 0xFE, 0x10, 0xFE, 0xFE, 0xFE, 0xFE, 
                                               0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0x0C, 0x0A,
                                               0x0D, 0x0B, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 };
uint8_t pcINO_InputHandlerSubModeCase10[24] = { 0, 0x0F, 0xFE, 0x10, 0xFE, 0xFE, 0xFE, 0xFE,
                                                0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0x0C, 0x0A,
                                                0x0D, 0x0B, 0xFE, 0xFE, 0xFE, 0xFE, 0, 0 };

int pcINO_case1and2UnkFunc(int a1, int* a2, float* a3, int a4, float a5)
{
    int result;
    BOOL v6;

    switch (*a2)
    {
    case 0:
        if (!a1)
            goto LABEL_13;
        goto LABEL_17;
    case 16:
        v6 = a1 == 0;
        goto LABEL_20;
    case 17:
        if (!a1)
        {
            *a2 = 18;
            return *a2;
        }
        if (TIM_gf_MainClock <= (double)*a3)
            goto LABEL_22;
    LABEL_7:
        *a2 = 16;
        result = *a2;
        break;
    case 18:
        if (a1)
            goto LABEL_7;
    LABEL_13:
        if (!a4)
            goto LABEL_22;
        *a2 = 33;
        *a3 = TIM_gf_MainClock + a5;
        return *a2;
    case 32:
        v6 = a4 == 0;
    LABEL_20:
        if (v6)
            *a2 = 0;
        goto LABEL_22;
    case 33:
        if (a4)
        {
            if (TIM_gf_MainClock <= (double)*a3)
            {
            LABEL_22:
                result = *a2;
            }
            else
            {
            LABEL_11:
                *a2 = 32;
                result = *a2;
            }
        }
        else
        {
            *a2 = 34;
            result = *a2;
        }
        break;
    case 34:
        if (a4)
            goto LABEL_11;
        if (!a1)
            goto LABEL_22;
    LABEL_17:
        *a2 = 17;
        *a3 = TIM_gf_MainClock + a5;
        return *a2;
    default:
        goto LABEL_22;
    }
    return result;
}

void pcINO_JoystickPositionAlignSetFromKeyboard(int zero, byte* keybData)
{
    int v2;
    int v3;
    int v4;

    v2 = pcINO_IntKeybDataLink[24];
    v3 = zero;
    pcINO_joystickPosition[v3].x = 0.0;
    if (keybData[v2] || keybData[pcINO_IntKeybDataLink[25]])
        pcINO_joystickPosition[v3].x = -1.0;
    if (keybData[pcINO_IntKeybDataLink[26]] || keybData[pcINO_IntKeybDataLink[27]])
        pcINO_joystickPosition[v3].x = (float)(pcINO_joystickPosition[v3].x + 1.0);
    v4 = pcINO_IntKeybDataLink[20];
    pcINO_joystickPosition[v3].y = 0.0;
    if (keybData[v4] || keybData[pcINO_IntKeybDataLink[21]])
        pcINO_joystickPosition[v3].y = 1.0;
    if (keybData[pcINO_IntKeybDataLink[22]] || keybData[pcINO_IntKeybDataLink[23]])
        pcINO_joystickPosition[v3].y = (float)(pcINO_joystickPosition[v3].y - 1.0);
}

void pcINO_InputHandlerCase1(struct sInoPad* a1, int* mouseData, byte* keybData)
{
    int v4;
    int v5;
    double v6;

    if (keybData[pcINO_IntKeybDataLink[0]] || keybData[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    if (keybData[pcINO_IntKeybDataLink[2]] && !pcINO_keyboardDataBackup[pcINO_IntKeybDataLink[2]]
        || keybData[pcINO_IntKeybDataLink[3]] && !pcINO_keyboardDataBackup[pcINO_IntKeybDataLink[3]])
    {
        a1->padData[2] = -1;
    }
    if (keybData[pcINO_IntKeybDataLink[16]] || keybData[pcINO_IntKeybDataLink[17]])
        a1->padData[15] = -1;
    if (keybData[257])
        a1->padData[15] = -1;
    if (keybData[pcINO_IntKeybDataLink[18]] || keybData[pcINO_IntKeybDataLink[19]])
        a1->padData[13] = -1;
    if (keybData[256])
        a1->padData[13] = -1;
    if (keybData[pcINO_IntKeybDataLink[4]] || keybData[pcINO_IntKeybDataLink[5]])
        a1->padData[1] = -1;
    if (keybData[pcINO_IntKeybDataLink[6]] || keybData[pcINO_IntKeybDataLink[7]])
        a1->padData[3] = -1;
    if (keybData[pcINO_IntKeybDataLink[8]] || keybData[pcINO_IntKeybDataLink[9]])
        a1->padData[5] = -1;
    if (keybData[pcINO_IntKeybDataLink[10]] || keybData[pcINO_IntKeybDataLink[11]])
        a1->padData[10] = -1;
    pcINO_JoystickPositionAlignSetFromKeyboard(0, keybData);
    if (pcINO_stateMachineFlags == 18)
    {
        if (pcINO_joystickPosition[0].x == 0.0 && pcINO_joystickPosition[0].y == 0.0)
        {
            if (TIM_gf_MainClock > (double)pcINO_case1DividedTime)
                pcINO_stateMachineFlags = 0;
        }
        else
        {
            pcINO_case1DividedTime = (float)(TIM_gf_MainClock + 0.30000001);
        }
    }
    v4 = keybData[pcINO_IntKeybDataLink[14]] || keybData[pcINO_IntKeybDataLink[15]];
    v5 = keybData[pcINO_IntKeybDataLink[12]] || keybData[pcINO_IntKeybDataLink[13]];
    pcINO_case1and2UnkFunc(v5, &pcINO_stateMachineFlags, &pcINO_case1DividedTime2, v4, 0.2f);
    if ((pcINO_stateMachineFlags & 0x10) != 0)
        a1->padData[7] = -1;
    if ((pcINO_stateMachineFlags & 0x20) != 0)
        a1->padData[4] = -1;
    pcINO_joystickPosition[1].x = (float)((double)*mouseData * -0.029999999);
    v6 = (double)mouseData[1] * 0.001 + pcINO_joystickPosition[1].y;
    if (v6 >= -0.2)
    {
        if (v6 <= 0.30000001)
            pcINO_joystickPosition[1].y = (float)v6;
        else
            pcINO_joystickPosition[1].y = 0.30000001f;
    }
    else
    {
        pcINO_joystickPosition[1].y = -0.2f;
    }
}

void pcINO_InputHandlerCase2(tInoPad* a1, int* mouse, byte* keyboard)
{
    int v3;
    int v4;
    int v5;
    int v6;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[2]] || keyboard[pcINO_IntKeybDataLink[3]])
        a1->padData[1] = -1;
    if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
        a1->padData[3] = -1;
    if (keyboard[pcINO_IntKeybDataLink[8]] || keyboard[pcINO_IntKeybDataLink[9]])
        a1->padData[5] = -1;
    v3 = keyboard[pcINO_IntKeybDataLink[14]] || keyboard[pcINO_IntKeybDataLink[15]];
    v4 = keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]];
    pcINO_case1and2UnkFunc(v4, &pcINO_stateMachineFlags, &pcINO_case1DividedTime2, v3, 0.2f);
    if ((pcINO_stateMachineFlags & 0x10) != 0)
        a1->padData[7] = -1;
    if ((pcINO_stateMachineFlags & 0x20) != 0)
        a1->padData[4] = -1;
    v5 = pcINO_case2_mouseScroll;
    pcINO_joystickPosition[0].x = (float)((double)*mouse * 0.017999999);
    pcINO_joystickPosition[0].y = (float)((double)mouse[1] * 0.017999999);
    v6 = mouse[2];
    if (v6)
    {
        v5 = v6 + pcINO_case2_mouseScroll;
        pcINO_case2_mouseScroll += v6;
        pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.039999999);
    }
    pcINO_joystickPosition[1].x = 0.0;
    pcINO_joystickPosition[1].y = 0.0;
    if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        pcINO_joystickPosition[1].x = 1.0;
    if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
        pcINO_joystickPosition[1].x = (float)(pcINO_joystickPosition[1].x - 1.0);
    if (keyboard[pcINO_IntKeybDataLink[20]] || keyboard[pcINO_IntKeybDataLink[21]] || v5 > 0)
        pcINO_joystickPosition[1].y = 3.4000001f;
    if (keyboard[pcINO_IntKeybDataLink[22]] || keyboard[pcINO_IntKeybDataLink[23]] || v5 < 0)
        pcINO_joystickPosition[1].y = pcINO_joystickPosition[1].y - 3.4000001f;
    if (v5 && TIM_gf_MainClock > (double)pcINO_case2DividedTime)
    {
        if (v5 <= 0)
            pcINO_case2_mouseScroll = mouseGranularity + v5;
        else
            pcINO_case2_mouseScroll = v5 - mouseGranularity;
    }
}

float pcINO_case4MouseAmountCalc(float* a1, float* a2, float a3)
{
    double result;

    result = -a3;
    if (result <= *a2)
    {
        if (*a2 <= (double)a3)
            result = *a2;
        else
            result = a3;
    }
    if (TIM_gf_MainClock > (double)*a1)
    {
        *a2 = (float)((*a2 - result) * 0.69999999);
        *a1 = (float)(TIM_gf_MainClock + 0.050000001);
    }
    return (float)result;
}

void pcINO_InputHandlerCase4(tInoPad* a1, int* mouse, byte* keyboard)
{
    int v3;
    double Y;
    double v5;
    double v6;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[4]] || keyboard[pcINO_IntKeybDataLink[5]])
        a1->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[32]] || keyboard[pcINO_IntKeybDataLink[33]])
        a1->padData[3] = -1;
    v3 = pcINO_SelectedControlSubmode;
    if (pcINO_SelectedControlSubmode)
    {
        pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);
        switch (v3)
        {
        case 2:
            if (keyboard[pcINO_IntKeybDataLink[34]] || keyboard[pcINO_IntKeybDataLink[35]])
            {
                a1->padData[5] = -1;
                a1->padData[3] = 0;
            }
            else if (keyboard[256])
            {
                pcINO_joystickPosition[1].y = 1.0;
            }
            else if (keyboard[257])
            {
                pcINO_joystickPosition[1].y = -1.0;
            }
            else
            {
                pcINO_joystickPosition[1].y = pcINO_joystickPosition[0].x;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
            if (keyboard[pcINO_IntKeybDataLink[34]] || keyboard[pcINO_IntKeybDataLink[35]])
                a1->padData[1] = -1;
            if (!keyboard[256])
                goto LABEL_35;
            goto LABEL_41;
        case 7:
            pcINO_case4_mouseMoveX = (float)((double)*mouse * 0.029999999 + pcINO_case4_mouseMoveX);
            pcINO_case4_mouseMoveY = (float)((double)mouse[1] * 0.029999999 + pcINO_case4_mouseMoveY);
            pcINO_joystickPosition[1].x = pcINO_case4MouseAmountCalc(&pcINO_case4_probablyUnunsedUnk1, &pcINO_case4_mouseMoveX, 3.0f);
            pcINO_joystickPosition[1].y = pcINO_case4MouseAmountCalc((float*)&MEM_gst_MemoryInfo.allocatedMemoryEnd, &pcINO_case4_mouseMoveY, 3.0f);
            if (keyboard[256])
            {
            LABEL_41:
                pcINO_joystickPosition[0].y = 1.0;
            }
            else
            {
            LABEL_35:
                if (keyboard[257])
                    pcINO_joystickPosition[0].y = -1.0;
            }
            break;
        case 10:
            if (keyboard[pcINO_IntKeybDataLink[30]] || keyboard[pcINO_IntKeybDataLink[31]])
                a1->padData[1] = -1;
            break;
        default:
            return;
        }
    }
    else
    {
        pcINO_joystickPosition[0].x = (float)((double)*mouse * 0.02 + pcINO_joystickPosition[0].x);
        Y = pcINO_joystickPosition[0].y - (double)mouse[1] * 0.02;
        pcINO_joystickPosition[0].y = (float)Y;
        v5 = _hypot(pcINO_joystickPosition[0].x, Y); // átfogó
        if (v5 <= 0.1)
        {
            pcINO_joystickPosition[0].x = 1.0;
            pcINO_joystickPosition[0].y = 0.0;
        }
        else
        {
            v6 = 1.0 / v5;
            pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x * v6);
            pcINO_joystickPosition[0].y = (float)(v6 * pcINO_joystickPosition[0].y);
        }
        if (keyboard[pcINO_IntKeybDataLink[34]] || keyboard[pcINO_IntKeybDataLink[35]])
            a1->padData[1] = -1;
        if (keyboard[260])
            a1->padData[1] = -1;
        if (keyboard[67] && keyboard[68])
        {
            a1->padData[5] = -1;
            a1->padData[7] = -1;
            if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
                a1->padData[1] = -1;
            pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);
        }
    }
}

void pcINO_InputHandlerCase8(tInoPad* a1, int *mouse, byte* keyboard)
{
    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[32]] || keyboard[pcINO_IntKeybDataLink[33]])
        a1->padData[1] = -1;
    pcINO_joystickPosition[0].x = 0.0;
    pcINO_joystickPosition[0].y = 0.0;
    if (TIM_gf_MainClock > (double)pcINO_case2DividedTime)
    {
        if (keyboard[pcINO_IntKeybDataLink[20]] || keyboard[pcINO_IntKeybDataLink[21]] || keyboard[256])
        {
            pcINO_joystickPosition[0].y = 1.0;
        }
        else
        {
            if (!keyboard[pcINO_IntKeybDataLink[22]] && !keyboard[pcINO_IntKeybDataLink[23]] && !keyboard[257])
            {
            LABEL_17:
                if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
                {
                    pcINO_joystickPosition[0].x = 1.0;
                    pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1);
                }
                else if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
                {
                    pcINO_joystickPosition[0].x = -1.0;
                    pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1);
                }
                return;
            }
            pcINO_joystickPosition[0].y = -1.0;
        }
        pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1);
        goto LABEL_17;
    }
}

void pcINO_InputHandlerCase32(tInoPad* a1, int* mouse, byte* keyboard)
{
    double v3;
    double v4;
    double v5;
    double v6;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[2]] || keyboard[pcINO_IntKeybDataLink[3]])
        a1->padData[2] = -1;
    if (keyboard[pcINO_IntKeybDataLink[16]] || keyboard[pcINO_IntKeybDataLink[17]])
        a1->padData[15] = -1;
    if (keyboard[257])
        a1->padData[15] = -1;
    if (keyboard[pcINO_IntKeybDataLink[18]] || keyboard[pcINO_IntKeybDataLink[19]])
        a1->padData[13] = -1;
    if (keyboard[256])
        a1->padData[13] = -1;
    if (keyboard[pcINO_IntKeybDataLink[4]] || keyboard[pcINO_IntKeybDataLink[5]])
        a1->padData[1] = -1;
    if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
        a1->padData[3] = -1;
    if (keyboard[pcINO_IntKeybDataLink[8]] || keyboard[pcINO_IntKeybDataLink[9]])
        a1->padData[5] = -1;
    if (keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]])
        a1->padData[7] = -1;
    if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
        v3 = -1.0;
    else
        v3 = 0.0;
    if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        v4 = 1.0;
    else
        v4 = 0.0;
    v5 = v3 + (double)*mouse * 0.050000001 + v4;
    if (v5 >= -1.0)
    {
        if (v5 <= 1.0)
            pcINO_joystickPosition[0].x = (float)v5;
        else
            pcINO_joystickPosition[0].x = 1.0;
    }
    else
    {
        pcINO_joystickPosition[0].x = -1.0;
    }
    v6 = (double)mouse[1] * 0.050000001;
    if (v6 >= -1.0)
    {
        if (v6 <= 1.0)
            pcINO_joystickPosition[0].y = (float)v6;
        else
            pcINO_joystickPosition[0].y = 1.0;
    }
    else
    {
        pcINO_joystickPosition[0].y = -1.0;
    }
    pcINO_joystickPosition[1].x = 0.0;
    pcINO_joystickPosition[1].y = 0.0;
}

int pcINO_case64StateMachineHandler(int a1, int* a2, float* a3, float a4)
{
    int result; // eax

    switch (*a2)
    {
    case 0:
        if (!a1)
            goto LABEL_12;
        *a2 = 17;
        *a3 = TIM_gf_MainClock + a4;
        return *a2;
    case 16:
        if (!a1)
            *a2 = 0;
        goto LABEL_12;
    case 17:
        if (a1)
        {
            if (TIM_gf_MainClock <= (double)*a3)
            {
            LABEL_12:
                result = *a2;
            }
            else
            {
                *a2 = 16;
                result = *a2;
            }
        }
        else
        {
            *a2 = 18;
            result = *a2;
        }
        break;
    case 18:
        if (!a1)
            goto LABEL_12;
        *a2 = 16;
        result = *a2;
        break;
    default:
        goto LABEL_12;
    }
    return result;
}

void pcINO_InputHandlerHoverCraft(tInoPad* pad, int* mouse, byte* keyboard)
{
    int tmp;
    float tmpX;
    float tmpY;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[2]] || keyboard[pcINO_IntKeybDataLink[3]])
        pad->padData[2] = -1;
    if (keyboard[pcINO_IntKeybDataLink[16]] || keyboard[pcINO_IntKeybDataLink[17]])
        pad->padData[15] = -1;
    if (keyboard[257])
        pad->padData[15] = -1;
    if (keyboard[pcINO_IntKeybDataLink[18]] || keyboard[pcINO_IntKeybDataLink[19]])
        pad->padData[13] = -1;
    if (keyboard[256])
        pad->padData[13] = -1;
    if (keyboard[pcINO_IntKeybDataLink[4]] || keyboard[pcINO_IntKeybDataLink[5]])
        pad->padData[1] = -1;
    if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
        pad->padData[3] = -1;
    if (keyboard[pcINO_IntKeybDataLink[8]] || keyboard[pcINO_IntKeybDataLink[9]])
        pad->padData[5] = -1;
    if (keyboard[pcINO_IntKeybDataLink[10]] || keyboard[pcINO_IntKeybDataLink[11]])
        pad->padData[10] = -1;
    
    tmp = keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]];    
    pcINO_case64StateMachineHandler(tmp, &pcINO_case64StateMachineFlags, &pcINO_case64ElapsedTime, 0.2f);
    
    if ((pcINO_case64StateMachineFlags & 0x10) != 0)
        pad->padData[7] = -1;
    
    pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);
    
    if (pcINO_SelectedControlSubmode == 1)
    {
        pcINO_joystickPosition[0].x = (float)mouse[0] * 0.017999999f;
        pcINO_joystickPosition[0].y = (float)mouse[1] * 0.017999999f;
    }
    else
    {
        if ((pcINO_case64StateMachineFlags & 0x10) == 0
            && pcINO_joystickPosition[0].x == 0.0f
            && pcINO_joystickPosition[0].y == 0.0f)
        {
            pcINO_joystickPosition[1].x = (float)mouse[0] * -0.029999999f;
        }
        else
        {
            tmpX = (float)mouse[0] * 0.039999999f + pcINO_joystickPosition[0].x;
            if (tmpX >= -1.0f)
            {
                if (tmpX <= 1.0f)
                    pcINO_joystickPosition[0].x = tmpX;
                else
                    pcINO_joystickPosition[0].x = 1.0f;
            }
            else
            {
                pcINO_joystickPosition[0].x = -1.0f;
            }
        }
        tmpY = (float)mouse[1] * 0.001f + pcINO_joystickPosition[1].y;
        if (tmpY >= -0.40000001f)
        {
            if (tmpY <= 0.1f)
                pcINO_joystickPosition[1].y = tmpY;
            else
                pcINO_joystickPosition[1].y = 0.1f;
        }
        else
        {
            pcINO_joystickPosition[1].y = -0.40000001f;
        }
    }
}

void pcINO_InputHandlerCase100(tInoPad* a1, int* mouse, byte* keyboard)
{
    long double v3;
    double Y;
    double v5;
    double v6;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    if (TIM_gf_MainClock > (double)pcINO_case2DividedTime)
    {
        v3 = atan2(pcINO_joystickPosition[0].y, pcINO_joystickPosition[0].x);
        if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        {
            v3 = v3 - 0.40000001;
        }
        else
        {
            if (!keyboard[pcINO_IntKeybDataLink[24]] && !keyboard[pcINO_IntKeybDataLink[25]])
                goto LABEL_12;
            v3 = v3 + 0.40000001;
        }
        pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1);
    LABEL_12:
        pcINO_joystickPosition[0].x = (float)cos(v3);
        pcINO_joystickPosition[0].y = (float)sin(v3);
    }
    pcINO_joystickPosition[0].x = (float)((double)*mouse * 0.02 + pcINO_joystickPosition[0].x);
    Y = pcINO_joystickPosition[0].y - (double)mouse[1] * 0.02;
    pcINO_joystickPosition[0].y = (float)Y;
    v5 = _hypot(pcINO_joystickPosition[0].x, Y);
    if (v5 <= 0.1)
    {
        pcINO_joystickPosition[0].x = 0.0;
        pcINO_joystickPosition[0].y = 1.0;
    }
    else
    {
        v6 = 1.0 / v5;
        pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x * v6);
        pcINO_joystickPosition[0].y = (float)(v6 * pcINO_joystickPosition[0].y);
    }
}

void pcINO_InputHandlerCase101(tInoPad* a1, int *mouse, byte* keyboard)
{
    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        a1->padData[0] = -1;
    pcINO_joystickPosition[0].x = 0.0;
    if (keyboard[30])
        pcINO_joystickPosition[0].x = -1.0;
    if (keyboard[32])
        pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x + 1.0);
    pcINO_joystickPosition[0].y = 0.0;
    if (keyboard[17])
        pcINO_joystickPosition[0].y = 1.0;
    if (keyboard[31])
        pcINO_joystickPosition[0].y = (float)(pcINO_joystickPosition[0].y - 1.0);
    pcINO_joystickPosition[1].x = 0.0;
    if (keyboard[203])
        pcINO_joystickPosition[1].x = -1.0;
    if (keyboard[205])
        pcINO_joystickPosition[1].x = (float)(pcINO_joystickPosition[1].x + 1.0);
    pcINO_joystickPosition[1].y = 0.0;
    if (keyboard[200])
        pcINO_joystickPosition[1].y = 1.0;
    if (keyboard[208])
        pcINO_joystickPosition[1].y = (float)(pcINO_joystickPosition[1].y - 1.0);
}

int DINPUT_GetProperty(IDirectInputDevice8A* a1, int obj, int how, const GUID* propertyGuid, int* dataRet)
{
    int result;
    DIPROPDWORD diprop;

    diprop.diph.dwHow = how;
    diprop.diph.dwObj = obj;
    diprop.diph.dwSize = sizeof(DIPROPDWORD);
    diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprop.dwData = 0;
    
    result = a1->lpVtbl->GetProperty(a1, propertyGuid, (LPDIPROPHEADER)&diprop);
    *dataRet = diprop.dwData;

    return result;
}

void DINPUT_InitVariables(int a1)
{
    BOOL v1;

    switch (a1)
    {
    case 0:
        memset(pcINO_joystickPosition, 0, sizeof(pcINO_joystickPosition));
        pcINO_case64StateMachineFlags = 0;
        pcINO_stateMachineFlags = 0;
        break;
    case 1:
        if (pcINO_stateMachineFlags == 18)
            goto LABEL_8;
        v1 = pcINO_stateMachineFlags == 34;
        goto LABEL_7;
    case 2:
        pcINO_case64StateMachineFlags = 0;
        pcINO_stateMachineFlags = 0;
        break;
    case 3:
        v1 = pcINO_stateMachineFlags == 18;
    LABEL_7:
        if (v1)
            LABEL_8 :
            pcINO_stateMachineFlags = 0;
        break;
    default:
        return;
    }
}

void pcINO_getKeyNames(int dwhow, char* a2)
{
    char* v2;
    char v3;
    char *v4;
    DIDEVICEOBJECTINSTANCEA pdidoi;
    
    char* prefix = "\\P4\\\x8F\0";
    char* postfix = "\x90\0";

    memset(&pdidoi, 0, sizeof(DIDEVICEOBJECTINSTANCEA));
    pdidoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEA);
    keyb_if_dword_98F238->lpVtbl->GetObjectInfo(keyb_if_dword_98F238, &pdidoi, dwhow, DIPH_BYOFFSET);
    v2 = a2;
    v3 = *prefix;
    
    while (v3)
    {
        *v2 = v3;
        v3 = *(++prefix);
        v2++;
    }

    v4 = pdidoi.tszName;
    if (pdidoi.tszName[0])
    {
        do
        {
            *v2 = *v4;
            v4++;
            v2++;
        } while (*v4);
    }

    v3 = *postfix;

    while (v3)
    {
        *v2 = v3;
        v3 = *(++postfix);
        v2++;
    }

    *v2 = 0;
}

void pcINO_getKeyNamesForEverything()
{
    tInoButtonNames* v1;
    uint16_t* v2;
    
    v1 = buttonNames;
    v2 = pcINO_IntKeybDataLink;
    do
    {
        switch (*v2)
        {
        case 57u:
            memcpy(v1, prefixSpaces, sizeof(prefixSpaces));
            break;
        case 256u:
        case 257u:
        case 260u:
            memcpy(v1, prefixOneSpace, sizeof(prefixOneSpace));
            break;
        case 258u:
            strcpy(v1->str, "\\p16\\0");
            break;
        case 259u:
            strcpy(v1->str, "\\p16\\1");
            break;
        default:
            pcINO_getKeyNames(*v2, v1->str);
            break;
        }
        v2 += 2;
        ++v1;
    } while ((int)v2 <= (int)&(pcINO_IntKeybDataLink[35]));
}

char* pcINO_GetFourKeyNamesCattedTogether(int a1, int a2, int a3, int a4)
{
    int v4;
    int v5;
    int v6;
    int v7;    

    v4 = pcINO_keyMaps[a1];
    v5 = pcINO_keyMaps[a2];
    v6 = pcINO_keyMaps[a3];
    v7 = pcINO_keyMaps[a4];
    if (v4 == -1)
        return prefix_p16_2;
    if (v4 < 0 || v5 < 0 || v6 < 0 || v7 < 0)
        return prefixOneSpace;

    strcpy(pcINO_KeyNamesToRet, buttonNames[v4].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[v5].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[v6].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[v7].str);
    return pcINO_KeyNamesToRet;
}

void INO_SetControlSubmode_inner_sub_48A650(int a1)
{
    if (a1 >= 0)
    {
        if (pcINO_SelectedControlSubmode != a1)
        {
            if (pcINO_SelectedControlMode == 4)
            {
                if (!pcINO_SelectedControlSubmode)
                    pcINO_joyPositionBeforeControlSubmodeSet = pcINO_joystickPosition[0];
                switch (a1)
                {
                case 0:
                    pcINO_joystickPosition[0] = pcINO_joyPositionBeforeControlSubmodeSet;
                    pcINO_keyMaps = pcINO_InputHandlerSubModeCase0;
                    pcINO_SelectedControlSubmode = a1;
                    break;
                case 2:
                    pcINO_keyMaps = pcINO_InputHandlerSubModeCase2;
                    pcINO_SelectedControlSubmode = a1;
                    break;
                case 7:
                    pcINO_keyMaps = pcINO_InputHandlerSubModeCase7;
                    DINPUT_InitVariables(2);
                    pcINO_SelectedControlSubmode = a1;
                    break;
                case 10:
                    pcINO_keyMaps = pcINO_InputHandlerSubModeCase10;
                    pcINO_SelectedControlSubmode = a1;
                    break;
                default:
                    pcINO_keyMaps = pcINO_InputHandlerSubModeCaseDef;
                    pcINO_SelectedControlSubmode = a1;
                    break;
                }
            }
            else
            {
                if (pcINO_SelectedControlMode == 64)
                    pcINO_joystickPosition[1].y = -0.30000001f;
                pcINO_SelectedControlSubmode = a1;
            }
        }
    }
    else
    {
        DINPUT_InitVariables(-a1);
    }
}

void INO_SetControlMode_inner_sub_48ACB0(int a1)
{
    if (pcINO_SelectedControlMode != a1)
    {
        if (pcINO_SelectedControlMode == 4 && !pcINO_SelectedControlSubmode)
            pcINO_joyPositionBeforeControlSubmodeSet = pcINO_joystickPosition[0];
        pcINO_joystickPosition[0].x = 0.0;
        pcINO_joystickPosition[0].y = 0.0;
        switch (a1)
        {
        case 1:
            sJoystickHandlerFunction = pcINO_InputHandlerCase1;
            pcINO_keyMaps = pcINO_keyMapsCase1;
            DINPUT_InitVariables(3);
            break;
        case 2:
            sJoystickHandlerFunction = pcINO_InputHandlerCase2;
            pcINO_keyMaps = pcINO_keyMapsCase2;
            break;
        case 4:
            pcINO_joystickPosition[0] = pcINO_joyPositionBeforeControlSubmodeSet;
            sJoystickHandlerFunction = pcINO_InputHandlerCase4;
            break;
        case 8:
            sJoystickHandlerFunction = pcINO_InputHandlerCase8;
            pcINO_keyMaps = pcINO_keyMapsCase8;
            break;
        case 32:
            sJoystickHandlerFunction = pcINO_InputHandlerCase32;
            pcINO_keyMaps = pcINO_keyMapsCase32;
            break;
        case 64:
            sJoystickHandlerFunction = pcINO_InputHandlerHoverCraft;
            pcINO_keyMaps = pcINO_keyMapsCase64;
            break;
        case 100:
            sJoystickHandlerFunction = pcINO_InputHandlerCase100;
            pcINO_keyMaps = pcINO_keyMapsCase100;
            break;
        case 101:
            sJoystickHandlerFunction = pcINO_InputHandlerCase101;
            pcINO_keyMaps = pcINO_keyMapsCase101;
            break;
        default:
            break;
        }
        pcINO_SelectedControlMode = a1;
        pcINO_SelectedControlSubmode = 0xffffffff;
        INO_SetControlSubmode_inner_sub_48A650(0);
    }
}

void pcINO_Joystick_Init(HWND hwnd)
{
    pcINO_Hwnd = hwnd;

    if (DirectInput8Create(GetModuleHandleA(0), DIRECTINPUT_VERSION, &IID_IDirectInput8A, (LPVOID*)&dinput_if, 0) >= 0)
    {
        if (dinput_if->lpVtbl->CreateDevice(dinput_if, &GUID_SysMouse, &mouse_if_dword_98F248, 0) >= 0)
        {
            if (mouse_if_dword_98F248->lpVtbl->SetDataFormat(mouse_if_dword_98F248, &c_dfDIMouse2) < 0
                || mouse_if_dword_98F248->lpVtbl->SetCooperativeLevel(mouse_if_dword_98F248, hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE) < 0)
            {
                mouse_if_dword_98F248->lpVtbl->Release(mouse_if_dword_98F248);
                mouse_if_dword_98F248 = 0;
            }
            if (DINPUT_GetProperty(mouse_if_dword_98F248, 8, 1, (const GUID*)3, &mouseGranularity) < 0)// DIPROP_GRANULARITY
                mouseGranularity = 1;
        }
        if (dinput_if->lpVtbl->CreateDevice(dinput_if, &GUID_SysKeyboard, &keyb_if_dword_98F238, 0) >= 0
            && (keyb_if_dword_98F238->lpVtbl->SetDataFormat(keyb_if_dword_98F238, &c_dfDIKeyboard) < 0
                || keyb_if_dword_98F238->lpVtbl->SetCooperativeLevel(keyb_if_dword_98F238, hwnd, DISCL_NOWINKEY | DISCL_FOREGROUND | DISCL_EXCLUSIVE) < 0))
        {
            keyb_if_dword_98F238->lpVtbl->Release(keyb_if_dword_98F238);
            keyb_if_dword_98F238 = 0;
        }
        INO_l_Joystick_Enable = 1;
        INO_gi_CurrentPadId = 0;
        if (SwapMouseButton(0))                   // ha elozoleg swappelve volt, akkor  nem 0-val tér vissza
        {
            SwapMouseButton(1);
            pcINO_IntKeybDataLink[0] = 259;
            pcINO_IntKeybDataLink[2] = 258;
        }

        INO_SetControlMode_inner_sub_48ACB0(4);
        INO_SetControlSubmode_inner_sub_48A650(8);
        pcINO_getKeyNamesForEverything();
    }
}

void pcINO_Joystick_Close()
{
    if (mouse_if_dword_98F248)
    {
        mouse_if_dword_98F248->lpVtbl->Release(mouse_if_dword_98F248);
        mouse_if_dword_98F248 = 0;
    }
    if (keyb_if_dword_98F238)
    {
        keyb_if_dword_98F238->lpVtbl->Release(keyb_if_dword_98F238);
        keyb_if_dword_98F238 = 0;
    }
    if (dinput_if)
    {
        dinput_if->lpVtbl->Release(dinput_if);
        dinput_if = 0;
    }
    INO_l_Joystick_Enable = 0;
}

void pcINO_Joystick_Move(Vec3* a1, int stickID)
{
    a1->x = pcINO_joystickPosition[stickID].x;
    a1->y = pcINO_joystickPosition[stickID].y;
    a1->z = pcINO_joystickPosition[stickID].z;
}

int pcINO_i_IsMouseBeingUsed()
{
    return 1;
}

int pcINO_i_SetCurrentPad(int a1)
{
    INO_gi_CurrentPadId = 0;
    return -(a1 != 0);
}

void pcINO_JoystickPifPafEnable(BOOL b) {
}

void pcINO_JoystickPafIntensity(int i1) {
}

INT_PTR WINAPI DbgDialogFunc(HWND hDlg, UINT a2, WPARAM hWnd, LPARAM a4)
{
    if (a2 == 272)
    {
        SetFocus((HWND)hWnd);
        SetWindowTextA((HWND)hWnd, dbgDialogBuff);
        SendMessageA((HWND)hWnd, 0xB1u, 0, -1);
        return 0;
    }
    if (a2 != 273)
        return 0;
    if (hWnd == 1)
    {
        GetDlgItemTextA(hDlg, 1, dbgDialogBuff, 128);
    }
    else if (hWnd != 2)
    {
        return 0;
    }
    EndDialog(hDlg, hWnd);
    return 1;
}

void pcINO_dinput_mouse_and_keyb_acquire()
{
    if (mouse_if_dword_98F248)
        mouse_if_dword_98F248->lpVtbl->Acquire(mouse_if_dword_98F248);
    if (keyb_if_dword_98F238)
        keyb_if_dword_98F238->lpVtbl->Acquire(keyb_if_dword_98F238);
    pcINO_unkKeyStateTest = GetKeyState(144) != 0;
    memset(pcINO_keyboardDataBackup, 0, sizeof(pcINO_keyboardDataBackup));
    pcINO_mouseTwoBytesDataBackup[0] = 0;
    pcINO_mouseTwoBytesDataBackup[1] = 0;
}

void pcINO_dinput_mouse_and_keyb_unacquire()
{
    if (mouse_if_dword_98F248)
        mouse_if_dword_98F248->lpVtbl->Unacquire(mouse_if_dword_98F248);
    if (keyb_if_dword_98F238)
        keyb_if_dword_98F238->lpVtbl->Unacquire(keyb_if_dword_98F238);
}

void pcINO_HandleTweakFlags(HWND hWndParent)
{
    tTweakElem* v1;
    tTweakElem* v2;
    int v3;
    int v4;
    float v5 = 0.0; // game doesn't initialize it, but it helps with sscanf fail.
    char Key[32];

    if (!tweakFlagsDisabled)
    {
        // TODO FIXME: megcsinalni azt a dialogot rendesen, nem csak egy karaktertömböt átadni!!!
        if (DialogBoxIndirectParamA(0, (LPCDLGTEMPLATEA)commandWindowDlgTemplate, hWndParent, DbgDialogFunc, 0) == 1)
        {
            sscanf(dbgDialogBuff, "%s %f", Key, &v5);
            v1 = (tTweakElem*)bsearch(Key, &tweaks, 17u, sizeof(tTweakElem), strcmp);
            v2 = v1;
            if (v1)
            {
                v3 = v1->type;
                if (v3)
                {
                    v4 = v3 - 1;
                    if (v4)
                    {
                        if (v4 == 1)
                            ((void(*)(float))v2->ptr)(v5);
                    }
                    else
                    {
                        *((float*)(v2->ptr)) = v5;
                    }
                }
                else
                {
                    *((int32_t*)(v2->ptr)) = (uint64_t)v5;
                }
            }
        }
        pcINO_dinput_mouse_and_keyb_acquire();
    }
}

void pcINO_Joystick_Update(struct sInoPad* padData)
{
    int v1;
    int16_t v2;
    int v3;
    int mouseData[5];
    byte keyboardData[264];
    byte v6a;
    byte v6b;

    memset(padData->padData, 0, sizeof(padData->padData));

    if (!mouse_if_dword_98F248)
        goto LABEL_7;
    v1 = mouse_if_dword_98F248->lpVtbl->GetDeviceState(mouse_if_dword_98F248, 20, mouseData);
    if (v1 == 0x8007001E)
    {
        v1 = mouse_if_dword_98F248->lpVtbl->Acquire(mouse_if_dword_98F248);
        if (v1 < 0)
        {
            DBG_OutputDebugString("Mouse input lost but acquire fails\n");
        }
        else
        {
            DBG_OutputDebugString("Mouse input lost & re-acquired\n");
            v1 = mouse_if_dword_98F248->lpVtbl->GetDeviceState(mouse_if_dword_98F248, 20, mouseData);
        }
    }
    if (v1 >= 0)
    {
        v2 = mouseData[4] & 0xffff;
    }
    else
    {
    LABEL_7:
        v2 = 0;
        mouseData[4] = 0;
        mouseData[0] = 0;
        mouseData[1] = 0;
        mouseData[2] = 0;
        mouseData[3] = 0;
    }
    if (!keyb_if_dword_98F238)
        goto LABEL_15;
    v3 = keyb_if_dword_98F238->lpVtbl->GetDeviceState(keyb_if_dword_98F238, 256, keyboardData);
    if (v3 == 0x8007001E)
    {
        v3 = keyb_if_dword_98F238->lpVtbl->Acquire(keyb_if_dword_98F238);
        if (v3 < 0)
        {
            DBG_OutputDebugString("Keyboard input lost but acquire fails\n");
        }
        else
        {
            DBG_OutputDebugString("Keyboard input lost & re-acquired\n");
            v3 = keyb_if_dword_98F238->lpVtbl->GetDeviceState(keyb_if_dword_98F238, 266, keyboardData);
        }
    }
    v2 = mouseData[4] & 0xffff;
    if (v3 < 0)
    {
    LABEL_15:
        memset(keyboardData, 0, sizeof(keyboardData));
        v6a = 0;
        v6b = 0;
    }
    keyboardData[0] = 0;
    keyboardData[256] = mouseData[2] <= 0 ? 0 : 0x80;
    keyboardData[257] = mouseData[2] >= 0 ? 0 : 0x80;
    keyboardData[258] = (mouseData[3] & 0xff) == 0 ? 0 : 0x80;
    keyboardData[259] = ((mouseData[3] >> 8) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[260] = ((mouseData[3] >> 16) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[261] = ((mouseData[3] >> 24) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[262] = (byte)(v2 & 0xff) == 0 ? 0 : 0x80;
    keyboardData[263] = (((v2) >> 8) & 0xff) == 0 ? 0 : 0x80;
    v6a = ((mouseData[4] >> 16) & 0xff) == 0 ? 0 : 0x80;
    v6b = ((mouseData[4] >> 24) & 0xff) == 0 ? 0 : 0x80;
    sJoystickHandlerFunction(padData, mouseData, keyboardData);
    if (keyboardData[pcINO_IntKeybDataLink[30]] || keyboardData[pcINO_IntKeybDataLink[31]])
        padData->padData[9] = -1;
    if (keyboardData[pcINO_IntKeybDataLink[28]] || keyboardData[pcINO_IntKeybDataLink[29]])
        padData->padData[8] = -1;
    if ((keyboardData[56] || keyboardData[184]) && keyboardData[62])
    {
        keyb_if_dword_98F238->lpVtbl->Unacquire(keyb_if_dword_98F238);
        keybd_event(0xA4u, 0, 1u, 0);
        keybd_event(0x73u, 0, 1u, 0);
        keybd_event(0x73u, 0, 3u, 0);
        keybd_event(0xA4u, 0, 3u, 0);
    }
    if (keyboardData[88] && !pcINO_keyboardDataBackup[88])
    {
        keyb_if_dword_98F238->lpVtbl->Unacquire(keyb_if_dword_98F238);
        pcINO_HandleTweakFlags(pcINO_Hwnd);
        keyb_if_dword_98F238->lpVtbl->Acquire(keyb_if_dword_98F238);
    }
    memcpy(pcINO_keyboardDataBackup, keyboardData, sizeof(pcINO_keyboardDataBackup));
    pcINO_mouseTwoBytesDataBackup[0] = v6a;
    pcINO_mouseTwoBytesDataBackup[1] = v6b;
}

int pcINO_SavModuleInit() {
    return 0;
}

int pcINO_SavModuleClose()
{
    int result;

    result = (int)INO_gst_SavFileDesc.fp;
    if (INO_gst_SavFileDesc.fp)
        result = fclose(INO_gst_SavFileDesc.fp);
    INO_gst_SavFileDesc.fp = 0;
    return result;
}

int pcINO_b_AsyncInProgress()
{
    return 0;
}

int pcINO_i_TestPresence()
{
    int result; // eax

    if (dword_990164) // TODO, FIXME: ezt nem kéne initelni?
    {
        INO_gi_SavButton[2] = 0;
        INO_gi_SavButton[6] = 0;
        INO_gi_SavButton[3] = 0;
        INO_gi_SavButton[7] = 0;
        pcINO_gi_LastError = 304;
        INO_gi_SavButton[0] = 3;
        INO_gi_SavButton[4] = 24;
        INO_gi_SavButton[1] = 6;
        INO_gi_SavButton[5] = 26;
        result = 27;
    }
    else
    {
        pcINO_gi_LastError = 0;
        result = 2;
    }
    return result;
}

int pcINO_i_MountCard()
{
    if (!dword_990168) // TODO FIXME nem kéne initelni?
        return 4;
    if (dword_990168 == 1)
        pcINO_gi_LastError = 5;
    else
        pcINO_gi_LastError = dword_990168 != 2 ? 301 : 3;
    INO_gi_SavButton[0] = 3;
    INO_gi_SavButton[4] = 24;
    INO_gi_SavButton[1] = 6;
    INO_gi_SavButton[5] = 26;
    INO_gi_SavButton[2] = 5;
    INO_gi_SavButton[6] = 5;
    INO_gi_SavButton[3] = 0;
    INO_gi_SavButton[7] = 0;
    return 27;
}

int pcINO_i_UnmountCard() {
    return 0;
}

int pcINO_i_CheckCard()
{
    if (!dword_99016C) // TODO FIXME nem kéne initelni?
        return 6;
    INO_gi_SavButton[2] = 0;
    INO_gi_SavButton[6] = 0;
    INO_gi_SavButton[3] = 0;
    INO_gi_SavButton[7] = 0;
    pcINO_gi_LastError = 602;
    INO_gi_SavButton[0] = 3;
    INO_gi_SavButton[4] = 24;
    INO_gi_SavButton[1] = 6;
    INO_gi_SavButton[5] = 26;
    return 27;
}

int pcINO_i_FormatCard()
{
    INO_gi_SavButton[7] = 0;
    INO_gi_SavButton[3] = 0;
    if (dword_990170) // TODO FIXME nem kéne initelni?
    {
        pcINO_gi_LastError = 705;
        INO_gi_SavButton[0] = 3;
        INO_gi_SavButton[4] = 24;
        INO_gi_SavButton[1] = 6;
        INO_gi_SavButton[5] = 26;
        INO_gi_SavButton[2] = 5;
        INO_gi_SavButton[6] = 5;
    }
    else
    {
        INO_gi_SavButton[1] = 0;
        INO_gi_SavButton[5] = 0;
        INO_gi_SavButton[2] = 0;
        INO_gi_SavButton[6] = 0;
        pcINO_gi_LastError = 704;
        INO_gi_SavButton[0] = 4;
        INO_gi_SavButton[4] = 1;
    }
    return 27;
}

int pcINO_i_TestFreeSpace()
{
    int v0;
    BOOL v1;

    v0 = INO_ge_SavSetNextState;
    v1 = INO_ge_SavSetNextState == -1;
    INO_gi_FreeSpace = 0x100000;
    INO_ge_SavSetNextState = -1;
    return v1 ? 0 : v0;
}

int pcINO_i_Retry()
{
    return INO_ge_SavCommand;
}

int pcINO_i_Continue() {
    return 0;
}

int pcINO_i_ActiveConsole() {
    return 0;
}

int pcINO_i_Open()
{
    int result;
    int v1;
    BOOL v2;

    INO_gst_SavFileDesc.fp = fopen(INO_gsz_SavFileName, "r+b");
    if (INO_gst_SavFileDesc.fp)
        goto LABEL_13;
    if (INO_ge_SavCommand == 16)
    {
        if (INO_gi_FreeSpace < 409600)
        {
            INO_gi_SavButton[2] = 0;
            INO_gi_SavButton[6] = 0;
            INO_gi_SavButton[3] = 0;
            INO_gi_SavButton[7] = 0;
            pcINO_gi_LastError = 302;
            INO_gi_SavButton[0] = 3;
            INO_gi_SavButton[4] = 24;
            INO_gi_SavButton[1] = 6;
            INO_gi_SavButton[5] = 26;
            INO_ge_SavSetNextState = 16;
            return 27;
        }
    }
    else if (INO_ge_SavCommand == 10 && INO_gi_FreeSpace < 1024)
    {
        INO_gi_SavButton[2] = 0;
        INO_gi_SavButton[6] = 0;
        INO_gi_SavButton[3] = 0;
        INO_gi_SavButton[7] = 0;
        pcINO_gi_LastError = 302;
        INO_gi_SavButton[0] = 3;
        INO_gi_SavButton[4] = 24;
        INO_gi_SavButton[1] = 6;
        INO_gi_SavButton[5] = 26;
        INO_ge_SavSetNextState = 10;
        return 27;
    }
    INO_gst_SavFileDesc.fp = fopen(INO_gsz_SavFileName, "w+b");
    if (INO_gst_SavFileDesc.fp)
    {
    LABEL_13:
        v1 = INO_ge_SavSetNextState;
        v2 = INO_ge_SavSetNextState == -1;
        INO_ge_SavSetNextState = -1;
        result = v2 ? 0 : v1;
    }
    else
    {
        if (INO_ge_SavCommand == 10 || (pcINO_gi_LastError = 405, INO_ge_SavCommand == 16))
            pcINO_gi_LastError = 308;
        INO_gi_SavButton[2] = 0;
        INO_gi_SavButton[6] = 0;
        INO_gi_SavButton[3] = 0;
        INO_gi_SavButton[7] = 0;
        INO_ge_SavSetNextState = INO_ge_SavCommand;
        INO_gi_SavButton[0] = 3;
        INO_gi_SavButton[4] = 24;
        INO_gi_SavButton[1] = 6;
        INO_gi_SavButton[5] = 26;
        result = 27;
    }
    return result;
}

int pcINO_i_Close()
{
    int v0;
    BOOL v1;

    if (INO_gst_SavFileDesc.fp)
        fclose(INO_gst_SavFileDesc.fp);
    v0 = INO_ge_SavSetNextState;
    v1 = INO_ge_SavSetNextState == -1;
    INO_gst_SavFileDesc.fp = 0;
    INO_ge_SavSetNextState = -1;
    return v1 ? 0 : v0;
}

int pcINO_i_ReadIndex_1()
{
    if (INO_gst_SavFileDesc.fp)
        fread(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 9;
}

int pcINO_i_WriteIndex()
{
    if (INO_gst_SavFileDesc.fp)
        fwrite(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 22;
}

int pcINO_i_ReadSlot_1()
{
    if (INO_gst_SavFileDesc.fp)
        fread(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 14;
}

int pcINO_i_ReadSlot_2()
{
    if (INO_gst_SavFileDesc.fp)
        fread(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 15;
}

int pcINO_i_ReadSlot_3()
{
    if (INO_gst_SavFileDesc.fp)
        fread(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 22;
}

int pcINO_i_WriteSlot_C()
{
    if (INO_gst_SavFileDesc.fp)
        fwrite(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 19;
}

int pcINO_i_WriteSlot_E()
{
    if (INO_gst_SavFileDesc.fp)
        fwrite(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return 28;
}

int pcINO_i_CheckFreeSpace()
{
    pcINO_gi_LastError = 0;
    if (!dword_990174) // TODO FIXME nem kéne initelni?
        return 0;
    INO_gi_SavButton[2] = 0;
    INO_gi_SavButton[6] = 0;
    INO_gi_SavButton[3] = 0;
    INO_gi_SavButton[7] = 0;
    pcINO_gi_LastError = 302;
    INO_gi_SavButton[0] = 3;
    INO_gi_SavButton[4] = 24;
    INO_gi_SavButton[1] = 6;
    INO_gi_SavButton[5] = 26;
    return 27;
}

int pcINO_i_TestCard()
{
    pcINO_gi_LastError = 0;
    if (!dword_990178) // TODO FIXME nem kéne initelni?
        return 0;
    INO_gi_SavButton[0] = 0;
    INO_gi_SavButton[4] = 0;
    INO_gi_SavButton[1] = 0;
    INO_gi_SavButton[5] = 0;
    INO_gi_SavButton[2] = 0;
    INO_gi_SavButton[6] = 0;
    INO_gi_SavButton[3] = 0;
    INO_gi_SavButton[7] = 0;
    pcINO_gi_LastError = 310;
    return 27;
}