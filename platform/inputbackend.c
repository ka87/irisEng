#include "inputbackend.h"
#include "overlay.h"
#include "../inputoutput.h"
#include "../math.h"
#include "../time.h"
#include "../memory.h"
#include "../dbg.h"
#include "../platform.h"
#include "../datapath.h"
#include "../fileio.h"

int pcINO_SelectedControlMode = 0xffffffff;
int pcINO_SelectedControlSubmode = 0xffffffff;
Vec3 pcINO_joyPositionBeforeControlSubmodeSet = { 1.0f, 0.0f, 0.0f };

char prefixSpaces[12] = "\\P4\\\x8F     \x90\0";
char prefixOneSpace[8] = "\\P4\\\x8F \x90\0";
char prefix_p16_2[7] = "\\p16\\2\0";


// TODO FIXME: ezek mik?? a játék nem 0ázza õket
int pcINO_savPresenceFailure = 0;
int pcINO_savMountFailure = 0;
int pcINO_savCheckFailure = 0;
int pcINO_savFormatFailure = 0;
int pcINO_savFreeSpaceFailure = 0;
int pcINO_savCardTestFailure = 0;

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

uint8_t pcINO_keyMapsCase8[24] = { 0, 0x10, 0xFE, 0x10, 0xFE, 0xFE, 0xFE, 0xFE,
                                   0xFE, 0xFE, 0xFE, 0xFE, 0x0F, 0x0E, 0x0C, 
                                   0x0A, 0x0D, 0x0B, 0xFE, 0xFE, 0xFE, 0xFE, 
                                   0, 0 };

uint8_t pcINO_keyMapsCase100[24] = { 0, 0x01, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 
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

void pcINO_JoystickPositionAlignSetFromKeyboard(int stick, uint8_t* keyboard)
{
    pcINO_joystickPosition[stick].x = 0.0;
    if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
        pcINO_joystickPosition[stick].x = -1.0;
    if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        pcINO_joystickPosition[stick].x = (float)(pcINO_joystickPosition[stick].x + 1.0);
    pcINO_joystickPosition[stick].y = 0.0;
    if (keyboard[pcINO_IntKeybDataLink[20]] || keyboard[pcINO_IntKeybDataLink[21]])
        pcINO_joystickPosition[stick].y = 1.0;
    if (keyboard[pcINO_IntKeybDataLink[22]] || keyboard[pcINO_IntKeybDataLink[23]])
        pcINO_joystickPosition[stick].y = (float)(pcINO_joystickPosition[stick].y - 1.0);
}

/* Control mode 1 -- retail 0x00423220..0x00423563, keymaps 0x00B65768.
   Transcribed from the retail binary. This one diverged a lot from the
   demo: retail inlines the shared state-machine helper and reduces it to
   one input, drives padData[4] from the keyboard rather than from a flag
   bit, and splits the mouse tail on the control submode. */
/* Retail 0x00422A20. Maps a 0..1 slider onto 0.2..2.2:
   t <= 0.5 -> t + 0.2 ; t > 0.5 -> (2t - 1) * 1.5 + 0.7.
   (0x00422A30 test ah,0x41 + jne, so the jump is taken when NOT >.) */
/* The shared control state machine. Retail inlines this identically in
   modes 1 (0x004233CC), 32 (0x00423D74) and 64 (0x00423980) -- same
   7-entry jump table over states 0..0x22, same bodies -- so it is one
   source function. It drives the single global at 0x00B8A08C.

   The demo's pcINO_case1and2UnkFunc took a second input and handled only
   0/16/17/18; retail has one input and three more states. */
int pcINO_case64StateMachineHandler(int pressed, int* state, float* deadline, float delay)
{
    switch (*state)
    {
    case 0:
    case 34:                                   /* index table sends 0x22 here */
        if (pressed)
        {
            *deadline = (float)(TIM_gf_MainClock + delay);
            *state = 17;
        }
        break;
    case 16:
        if (!pressed)
            *state = 0;
        break;
    case 17:
        if (!pressed)
            *state = 18;
        else if (TIM_gf_MainClock > (double)*deadline)
            *state = 16;
        break;
    case 18:
        if (pressed)
            *state = 16;
        break;
    case 32:
        *state = 0;
        break;
    case 33:
        *state = 34;
        break;
    default:
        break;
    }
    return *state;
}

/* Retail 0x004231C0 -- ecx = accumulator, edx = decay deadline,
   stack = limit. Returns the accumulator clamped to +-limit, and once
   the clock passes the deadline bleeds the accumulator 30% toward that
   clamped value and re-arms for another 0.05s. Called but never defined
   before this. */
float pcINO_case4MouseAmountCalc(float* accum, float* deadline, float limit)
{
    float result;

    if (-limit > *accum)            /* 0x004231C6 */
        result = -limit;
    else if (*accum > limit)        /* 0x004231D3 */
        result = limit;
    else
        result = *accum;

    if (TIM_gf_MainClock > (double)*deadline)       /* 0x004231EC */
    {
        *accum = (float)((*accum - result) * 0.69999999f);   /* 0x00B5014C */
        *deadline = (float)(TIM_gf_MainClock + 0.050000001f); /* 0x00B50014 */
    }
    return result;
}

void pcINO_SetMouseSensitivity(float t)
{
    if (t > 0.5f)
        pcINO_mouseSensitivity = (t + t - 1.0f) * 1.5f + 0.69999999f;
    else
        pcINO_mouseSensitivity = t * 1.0f + 0.2f;
}

void pcINO_InputHandlerCase1(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    int forward;
    double y;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[2]] && !pcINO_keyboardDataBackup[pcINO_IntKeybDataLink[2]]
        || keyboard[pcINO_IntKeybDataLink[3]] && !pcINO_keyboardDataBackup[pcINO_IntKeybDataLink[3]])
    {
        pad->padData[2] = -1;
    }
    if (keyboard[pcINO_IntKeybDataLink[16]] || keyboard[pcINO_IntKeybDataLink[17]])
        pad->padData[15] = -1;
    if (keyboard[257])                                  /* wheel down */
        pad->padData[15] = -1;
    if (keyboard[pcINO_IntKeybDataLink[18]] || keyboard[pcINO_IntKeybDataLink[19]])
        pad->padData[13] = -1;
    if (keyboard[256])                                  /* wheel up */
        pad->padData[13] = -1;
    if (keyboard[pcINO_IntKeybDataLink[4]] || keyboard[pcINO_IntKeybDataLink[5]])
        pad->padData[1] = -1;
    if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
        pad->padData[3] = -1;
    if (keyboard[pcINO_IntKeybDataLink[8]] || keyboard[pcINO_IntKeybDataLink[9]])
        pad->padData[5] = -1;
    if (keyboard[pcINO_IntKeybDataLink[10]] || keyboard[pcINO_IntKeybDataLink[11]])
        pad->padData[10] = -1;

    pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);

    if (pcINO_stateMachineFlags == 0x12)                /* 0x00423349 */
    {
        if (pcINO_joystickPosition[0].x == 0.0f && pcINO_joystickPosition[0].y == 0.0f)
        {
            if (TIM_gf_MainClock > pcINO_case1DividedTime)
                pcINO_stateMachineFlags = 0;
        }
        else
        {
            pcINO_case1DividedTime = (float)(TIM_gf_MainClock + 0.30000001f);
        }
    }

    forward = keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]];

    pcINO_case64StateMachineHandler(forward, &pcINO_stateMachineFlags,
                                    &pcINO_case1DividedTime2, 0.2f);

    if ((pcINO_stateMachineFlags & 0x10) != 0)
        pad->padData[7] = -1;
    /* 0x00423449: from the keyboard, not from flags & 0x20. */
    if (keyboard[pcINO_IntKeybDataLink[14]] || keyboard[pcINO_IntKeybDataLink[15]])
        pad->padData[4] = -1;

    if (pcINO_SelectedControlSubmode)                   /* 0x00423466 */
    {
        pcINO_joystickPosition[1].x = (float)(mouse[0] * -0.029999999f);
        y = pcINO_joystickPosition[1].y - mouse[1] * 0.001f;
        if (y < -1.0f)
            pcINO_joystickPosition[1].y = -1.0f;
        else if (y > 1.0f)
            pcINO_joystickPosition[1].y = 1.0f;
        else
            pcINO_joystickPosition[1].y = (float)y;
    }
    else
    {
        /* 0x004234CE: reset X when the mouse reverses, then smooth. */
        if (pcINO_joystickPosition[1].x * mouse[0] < 0.0f)
            pcINO_joystickPosition[1].x = 0.0f;
        pcINO_joystickPosition[1].x =
            (float)((mouse[0] * -0.019999998f - pcINO_joystickPosition[1].x) * 0.60000002f
                    + pcINO_joystickPosition[1].x);
        y = pcINO_joystickPosition[1].y + mouse[1] * 0.001f;
        if (y < -0.2f)
            pcINO_joystickPosition[1].y = -0.2f;
        else if (y > 0.30000001f)
            pcINO_joystickPosition[1].y = 0.30000001f;
        else
            pcINO_joystickPosition[1].y = (float)y;
    }
}

/* Control mode 2. It reads only the shared pcINO_IntKeybDataLink table,
   not a per-mode keymap.
   Retail has no state machine here at all: it force-clears the shared
   flags and drives padData[7] and [4] straight from the keys. It also
   branches three ways on the submode, which the demo did not. */
void pcINO_InputHandlerCase2(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    int wheel;
    int scroll;
    int submode;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[2]] || keyboard[pcINO_IntKeybDataLink[3]])
        pad->padData[1] = -1;
    if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
        pad->padData[3] = -1;
    if (keyboard[pcINO_IntKeybDataLink[8]] || keyboard[pcINO_IntKeybDataLink[9]])
        pad->padData[5] = -1;
    if (keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]])
        pad->padData[7] = -1;
    if (keyboard[pcINO_IntKeybDataLink[14]] || keyboard[pcINO_IntKeybDataLink[15]])
        pad->padData[4] = -1;

    submode = pcINO_SelectedControlSubmode;
    pcINO_stateMachineFlags = 0;                           /* 0x0042366C */

    if (submode == 1 || submode == 2)
    {
        if (submode == 1)
        {
            pcINO_joystickPosition[0].x = (float)(mouse[0] * 0.019999999f);
            pcINO_joystickPosition[0].y = (float)(mouse[1] * 0.0049999999f);
        }
        else
        {
            pcINO_joystickPosition[0].x = (float)(mouse[0] * 0.039999999f);
            pcINO_joystickPosition[0].y = (float)(mouse[1] * -0.012f);
        }
        if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
            pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x + 1.0);
        if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
            pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x - 1.0);
    }
    else
    {
        pcINO_joystickPosition[0].x = (float)(mouse[0] * 0.014f);
        pcINO_joystickPosition[1].x = 0.0f;
        pcINO_joystickPosition[0].y = (float)(mouse[1] * 0.014f);
        if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
            pcINO_joystickPosition[1].x = 0.40000001f;
        if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
            pcINO_joystickPosition[1].x = (float)(pcINO_joystickPosition[1].x - 0.40000001f);
    }

    /* The wheel arrives already divided by the granularity and is an
       int in that slot, not a float, so this accumulates whole notches
       and decays by exactly one per 0.04s. */
    wheel = ((int*)mouse)[2];
    if (wheel)
    {
        pcINO_case2_mouseScroll += wheel;
        pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.039999999f);
    }
    scroll = pcINO_case2_mouseScroll;

    pcINO_joystickPosition[1].y = 0.0f;
    if (keyboard[pcINO_IntKeybDataLink[20]] || keyboard[pcINO_IntKeybDataLink[21]] || scroll > 0)
        pcINO_joystickPosition[1].y = 2.0f;
    if (keyboard[pcINO_IntKeybDataLink[22]] || keyboard[pcINO_IntKeybDataLink[23]] || scroll < 0)
        pcINO_joystickPosition[1].y = (float)(pcINO_joystickPosition[1].y - 2.0);

    if (scroll != 0 && TIM_gf_MainClock > (double)pcINO_case2DividedTime)
    {
        if (scroll > 0)
            pcINO_case2_mouseScroll = scroll - 1;
        else
            pcINO_case2_mouseScroll = scroll + 1;
    }
}

void pcINO_InputHandlerCase4(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    int submode;
    double gain;
    double y;
    double len;
    double inv;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[4]] || keyboard[pcINO_IntKeybDataLink[5]])
        pad->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[32]] || keyboard[pcINO_IntKeybDataLink[33]])
        pad->padData[3] = -1;

    submode = pcINO_SelectedControlSubmode;
    if (!submode)
    {
        gain = 1.0 / pcINO_mouseSensitivity;                 /* 0x00423F6B */
        pcINO_joystickPosition[0].x =
            (float)(gain * mouse[0] * 0.015f + pcINO_joystickPosition[0].x);
        y = pcINO_joystickPosition[0].y - gain * mouse[1] * 0.015f;
        pcINO_joystickPosition[0].y = (float)y;

        len = hypot(pcINO_joystickPosition[0].x, y);
        if (len > 0.1f)                                       /* 0x00423FAE */
        {
            inv = 1.0 / len;
            pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x * inv);
            pcINO_joystickPosition[0].y = (float)(inv * pcINO_joystickPosition[0].y);
        }
        else
        {
            pcINO_joystickPosition[0].x = 1.0f;
            pcINO_joystickPosition[0].y = 0.0f;
        }

        if (keyboard[pcINO_IntKeybDataLink[34]] || keyboard[pcINO_IntKeybDataLink[35]])
            pad->padData[1] = -1;
        if (keyboard[260])                                    /* mouse button 3 */
            pad->padData[1] = -1;
        if (keyboard[67] && keyboard[68])                     /* DIK_F9 + DIK_F10 */
        {
            pad->padData[5] = -1;
            pad->padData[7] = -1;
            if (keyboard[pcINO_IntKeybDataLink[6]] || keyboard[pcINO_IntKeybDataLink[7]])
                pad->padData[1] = -1;
            pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);
        }
        return;
    }

    /* 0x0042406E: zeroes [0].x/.y, then re-seeds them from the keyboard,
       so nothing on these paths can carry a wheel tick over to the next
       frame. Note it is only ever called with index 0 -- [1] is never
       reset here. */
    pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);

    switch (submode)
    {
    case 2:
        if (keyboard[pcINO_IntKeybDataLink[34]] || keyboard[pcINO_IntKeybDataLink[35]])
        {
            pad->padData[5] = -1;
            pad->padData[3] = 0;
            /* retail writes no [1].y on this path */
        }
        else if (keyboard[256])
            pcINO_joystickPosition[1].y = 1.0f;
        else if (keyboard[257])
            pcINO_joystickPosition[1].y = -1.0f;
        else
            pcINO_joystickPosition[1].y = pcINO_joystickPosition[0].x;
        break;

    case 7:
        pcINO_case4_mouseMoveX = (float)(mouse[0] * 0.03f + pcINO_case4_mouseMoveX);
        pcINO_case4_mouseMoveY = (float)(mouse[1] * 0.03f + pcINO_case4_mouseMoveY);
        pcINO_joystickPosition[1].x = pcINO_case4MouseAmountCalc(
            &pcINO_case4_mouseMoveX, &pcINO_case4_mouseMoveXDeadline, 3.0f);
        pcINO_joystickPosition[1].y = pcINO_case4MouseAmountCalc(
            &pcINO_case4_mouseMoveY, &pcINO_case4_mouseMoveYDeadline, 3.0f);
        if (keyboard[256])
            pcINO_joystickPosition[0].y = 1.0f;
        else if (keyboard[257])
            pcINO_joystickPosition[0].y = -1.0f;
        break;

    case 3: case 4: case 5: case 6: case 8: case 9:
        if (keyboard[pcINO_IntKeybDataLink[34]] || keyboard[pcINO_IntKeybDataLink[35]])
            pad->padData[1] = -1;
        if (keyboard[256])
            pcINO_joystickPosition[0].y = 1.0f;
        else if (keyboard[257])
            pcINO_joystickPosition[0].y = -1.0f;
        break;

    case 10:
        if (keyboard[pcINO_IntKeybDataLink[30]] || keyboard[pcINO_IntKeybDataLink[31]])
            pad->padData[1] = -1;
        break;

    default:
        break;
    }
}

void pcINO_InputHandlerCase8(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
    /* 0x00424223: retail has this block, the demo did not. Same keys as
       the padData[1] test below. */
    if (keyboard[pcINO_IntKeybDataLink[32]] || keyboard[pcINO_IntKeybDataLink[33]])
        pad->padData[3] = -1;
    if (keyboard[pcINO_IntKeybDataLink[32]] || keyboard[pcINO_IntKeybDataLink[33]])
        pad->padData[1] = -1;

    pcINO_joystickPosition[0].x = 0.0f;
    pcINO_joystickPosition[0].y = 0.0f;

    if (TIM_gf_MainClock > pcINO_case2DividedTime)
    {
        if (keyboard[pcINO_IntKeybDataLink[20]] || keyboard[pcINO_IntKeybDataLink[21]]
            || keyboard[256])
        {
            pcINO_joystickPosition[0].y = 1.0f;
            pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1f);
        }
        else if (keyboard[pcINO_IntKeybDataLink[22]] || keyboard[pcINO_IntKeybDataLink[23]]
                 || keyboard[257])
        {
            pcINO_joystickPosition[0].y = -1.0f;
            pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1f);
        }
        if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        {
            pcINO_joystickPosition[0].x = 1.0f;
            pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1f);
        }
        else if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
        {
            pcINO_joystickPosition[0].x = -1.0f;
            pcINO_case2DividedTime = (float)(TIM_gf_MainClock + 0.1f);
        }
    }
}

void pcINO_InputHandlerCase32(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    double amount;

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

    /* 0x00423D51: links 12/13 drive the shared state machine; padData[7]
       comes from its flag, not straight from the keys. */
    pcINO_case64StateMachineHandler(
        keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]],
        &pcINO_stateMachineFlags, &pcINO_case1DividedTime2, 0.2f);
    if ((pcINO_stateMachineFlags & 0x10) != 0)
        pad->padData[7] = -1;

    pcINO_JoystickPositionAlignSetFromKeyboard(0, keyboard);   /* 0x00423DF7 */

    /* 0x00B50200 = 0.06f, accumulated onto the keyboard seed. */
    amount = (double)mouse[0] * 0.059999999f + pcINO_joystickPosition[0].x;
    if (amount < -1.0)      pcINO_joystickPosition[0].x = -1.0f;
    else if (amount > 1.0)  pcINO_joystickPosition[0].x = 1.0f;
    else                    pcINO_joystickPosition[0].x = (float)amount;

    amount = pcINO_joystickPosition[0].y - (double)mouse[1] * 0.059999999f;
    if (amount < -1.0)      pcINO_joystickPosition[0].y = -1.0f;
    else if (amount > 1.0)  pcINO_joystickPosition[0].y = 1.0f;
    else                    pcINO_joystickPosition[0].y = (float)amount;

    pcINO_joystickPosition[1].x = 0.0f;
    pcINO_joystickPosition[1].y = 0.0f;
}

/* Control mode 64 -- retail 0x00423850..0x00423C14, keymaps 0x00B65798.
   Retail splits the keyboard align in two: the .x half and the .y = -1
   half run before the submode branch, the .y += 1 half afterwards, and
   two paths return before reaching it. */
void pcINO_InputHandlerHoverCraft(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    int   boost;
    float amount;

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

    boost = keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]];
    pcINO_case64StateMachineHandler(boost, &pcINO_stateMachineFlags,
                                    &pcINO_case1DividedTime2, 0.2f);
    if ((pcINO_stateMachineFlags & 0x10) != 0)
        pad->padData[7] = -1;

    /* first half of the align, inlined (0x00423A0E..0x00423A85) */
    pcINO_joystickPosition[0].x = 0.0f;
    if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
        pcINO_joystickPosition[0].x = -1.0f;
    if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x + 1.0);
    pcINO_joystickPosition[0].y = 0.0f;
    if (keyboard[pcINO_IntKeybDataLink[22]] || keyboard[pcINO_IntKeybDataLink[23]])
        pcINO_joystickPosition[0].y = -1.0f;

    if (pcINO_SelectedControlSubmode == 1)
    {
        pcINO_joystickPosition[0].x = (float)(mouse[0] * 0.014f + pcINO_joystickPosition[0].x);
        pcINO_joystickPosition[0].y = (float)(mouse[1] * 0.014f + pcINO_joystickPosition[0].y);
        if (pcINO_joystickPosition[0].x != 0.0f)
            return;
    }
    else if (pcINO_SelectedControlSubmode == 4)
    {
        pcINO_joystickPosition[0].x = (float)(mouse[0] * 0.019999999f + pcINO_joystickPosition[0].x);
        amount = (float)(mouse[1] * 0.00050000002f + pcINO_joystickPosition[1].y);
        if (amount < 0.0f)              pcINO_joystickPosition[1].y = 0.0f;
        else if (amount > 0.80000001f)  pcINO_joystickPosition[1].y = 0.80000001f;
        else                            pcINO_joystickPosition[1].y = amount;
    }
    else
    {
        /* 0x00423AB2: tests the forward KEYS, not the stick position. */
        if ((pcINO_stateMachineFlags & 0x10) == 0
            && !keyboard[pcINO_IntKeybDataLink[20]]
            && !keyboard[pcINO_IntKeybDataLink[21]])
        {
            pcINO_joystickPosition[1].x = (float)(mouse[0] * -0.029999999f);
        }
        else
        {
            /* no clamp in retail */
            pcINO_joystickPosition[0].x =
                (float)(mouse[0] * 0.039999999f + pcINO_joystickPosition[0].x);
        }

        amount = (float)(mouse[1] * 0.001f + pcINO_joystickPosition[1].y);
        if (amount < -0.40000001f)      pcINO_joystickPosition[1].y = -0.40000001f;
        else if (amount > 0.1f)         pcINO_joystickPosition[1].y = 0.1f;
        else                            pcINO_joystickPosition[1].y = amount;

        if ((pcINO_stateMachineFlags & 0x10) != 0 && pcINO_joystickPosition[0].x != 0.0f)
            return;
    }

    /* deferred second half of the align (0x00423BEC) */
    if (keyboard[pcINO_IntKeybDataLink[20]] || keyboard[pcINO_IntKeybDataLink[21]])
        pcINO_joystickPosition[0].y = (float)(pcINO_joystickPosition[0].y + 1.0);
}

void pcINO_InputHandlerCase100(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    double angle;
    double len;
    double inv;

    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
    if (keyboard[pcINO_IntKeybDataLink[4]] || keyboard[pcINO_IntKeybDataLink[5]])
        pad->padData[0] = -1;                                   /* 0x00424383 */
    if (keyboard[pcINO_IntKeybDataLink[12]] || keyboard[pcINO_IntKeybDataLink[13]])
        pad->padData[0] = -1;                                   /* 0x004243A0 */
    if (keyboard[pcINO_IntKeybDataLink[2]] || keyboard[pcINO_IntKeybDataLink[3]])
        pad->padData[1] = -1;                                   /* 0x004243BD */

    /* No clock gate in retail: the rotation runs every frame, and the
       demo's pcINO_case2DividedTime throttle is not here at all. */
    angle = atan2(pcINO_joystickPosition[0].y, pcINO_joystickPosition[0].x);
    if (keyboard[pcINO_IntKeybDataLink[26]] || keyboard[pcINO_IntKeybDataLink[27]])
        angle = angle - 0.1f;                                   /* 0x00424426 */
    else if (keyboard[pcINO_IntKeybDataLink[24]] || keyboard[pcINO_IntKeybDataLink[25]])
        angle = angle + 0.1f;                                   /* 0x0042441E */
    pcINO_joystickPosition[0].x = (float)cos(angle);
    pcINO_joystickPosition[0].y = (float)sin(angle);

    /* 0x00B50078 = 0.01f, against the already-halved mouse deltas. */
    pcINO_joystickPosition[0].x = (float)(mouse[0] * 0.01f + pcINO_joystickPosition[0].x);
    pcINO_joystickPosition[0].y = (float)(pcINO_joystickPosition[0].y - mouse[1] * 0.01f);

    len = hypot(pcINO_joystickPosition[0].x, pcINO_joystickPosition[0].y);
    if (len > 0.1f)                                            /* 0x0042447C */
    {
        inv = 1.0 / len;
        pcINO_joystickPosition[0].x = (float)(pcINO_joystickPosition[0].x * inv);
        pcINO_joystickPosition[0].y = (float)(inv * pcINO_joystickPosition[0].y);
    }
    else
    {
        pcINO_joystickPosition[0].x = 0.0f;
        pcINO_joystickPosition[0].y = 1.0f;
    }
}

void pcINO_InputHandlerCase101(tInoPad* pad, float* mouse, uint8_t* keyboard)
{
    if (keyboard[pcINO_IntKeybDataLink[0]] || keyboard[pcINO_IntKeybDataLink[1]])
        pad->padData[0] = -1;
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

/* Nothing to do with DirectInput despite the name, which is the one
   the game's own symbols carry: it resets the stick the keyboard
   drives, and the state machine that goes with it. */
void DINPUT_InitVariables(int kind)
{
    switch (kind)
    {
    case 0:
        memset(pcINO_joystickPosition, 0, sizeof(pcINO_joystickPosition));
        pcINO_stateMachineFlags = 0;
        break;
    case 1:
        if (pcINO_stateMachineFlags == 18 || pcINO_stateMachineFlags == 34)
            pcINO_stateMachineFlags = 0;
        break;
    case 2:
        pcINO_stateMachineFlags = 0;
        break;
    case 3:
        if (pcINO_stateMachineFlags == 18)
            pcINO_stateMachineFlags = 0;
        break;
    case 4:
        /* Retail 0x00424827: the fifth entry of the dispatch table at
           0x0042483C, which the demo did not have. */
        if (pcINO_stateMachineFlags == 34)
            pcINO_stateMachineFlags = 0;
        break;
    default:
        break;
    }
}

/* What a key is called, for the controls screen. The name used to be
   asked of the keyboard device by its code; it is asked of the
   keyboard layout now, by the same code. The two markers either side
   of it are the game's own and say how the line is drawn. */
void pcINO_getKeyNames(int keyOffset, char* dest)
{
    char* out;
    const char* src;

    const char* prefix = "\\P4\\\x8F\0";
    const char* postfix = "\x90\0";

    out = dest;
    for (src = prefix; *src; src++)
        *out++ = *src;
    for (src = PLT_sz_KeyName(keyOffset); *src; src++)
        *out++ = *src;
    for (src = postfix; *src; src++)
        *out++ = *src;
    *out = 0;
}

void pcINO_getKeyNamesForEverything()
{
    tInoButtonNames* name;
    int i;

    for (i = 0; i < 36; i += 2)
    {
        name = &buttonNames[i / 2];
        switch (pcINO_IntKeybDataLink[i])
        {
        case 57u:
            memcpy(name, prefixSpaces, sizeof(prefixSpaces));
            break;
        case 256u:
        case 257u:
        case 260u:
            memcpy(name, prefixOneSpace, sizeof(prefixOneSpace));
            break;
        case 258u:
            strcpy(name->str, "\\p16\\0");
            break;
        case 259u:
            strcpy(name->str, "\\p16\\1");
            break;
        default:
            pcINO_getKeyNames(pcINO_IntKeybDataLink[i], name->str);
            break;
        }
    }
}

char* pcINO_GetFourKeyNamesCattedTogether(int action1, int action2, int action3, int action4)
{
    int name1;
    int name2;
    int name3;
    int name4;

    name1 = pcINO_keyMaps[action1];
    name2 = pcINO_keyMaps[action2];
    name3 = pcINO_keyMaps[action3];
    name4 = pcINO_keyMaps[action4];
    if (name1 == -1)
        return prefix_p16_2;
    if (name1 < 0 || name2 < 0 || name3 < 0 || name4 < 0)
        return prefixOneSpace;

    strcpy(pcINO_KeyNamesToRet, buttonNames[name1].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[name2].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[name3].str);
    strcat(pcINO_KeyNamesToRet, buttonNames[name4].str);
    return pcINO_KeyNamesToRet;
}

void pcINO_SetControlSubmode(int submode)
{
    if (submode < 0)
    {
        DINPUT_InitVariables(-submode);
        return;
    }
    if (pcINO_SelectedControlSubmode == submode)
        return;

    if (pcINO_SelectedControlMode == 4)
    {
        if (!pcINO_SelectedControlSubmode)
            pcINO_joyPositionBeforeControlSubmodeSet = pcINO_joystickPosition[0];
        switch (submode)
        {
        case 0:
            pcINO_joystickPosition[0] = pcINO_joyPositionBeforeControlSubmodeSet;
            pcINO_keyMaps = pcINO_InputHandlerSubModeCase0;
            break;
        case 2:
            pcINO_keyMaps = pcINO_InputHandlerSubModeCase2;
            break;
        case 7:
            pcINO_keyMaps = pcINO_InputHandlerSubModeCase7;
            DINPUT_InitVariables(2);
            break;
        case 10:
            pcINO_keyMaps = pcINO_InputHandlerSubModeCase10;
            break;
        default:
            pcINO_keyMaps = pcINO_InputHandlerSubModeCaseDef;
            break;
        }
    }
    else
    {
        /* Retail 0x00424931. Mode 64 picks between two values
           on the submode rather than always using one, and
           mode 1 centres the right stick. The demo had
           neither. */
        if (pcINO_SelectedControlMode == 64)
        {
            if (submode == 4)
                pcINO_joystickPosition[1].y = 0.44999999f;  /* 0x3EE66666 */
            else
                pcINO_joystickPosition[1].y = -0.1f;        /* 0xBDCCCCCD */
        }
        else if (pcINO_SelectedControlMode == 1)
        {
            pcINO_joystickPosition[1].y = 0.0f;
        }
    }
    pcINO_SelectedControlSubmode = submode;
}

void pcINO_SetControlMode(int mode)
{
    if (pcINO_SelectedControlMode == mode)
        return;

    if (pcINO_SelectedControlMode == 4 && !pcINO_SelectedControlSubmode)
        pcINO_joyPositionBeforeControlSubmodeSet = pcINO_joystickPosition[0];
    pcINO_joystickPosition[0].x = 0.0;
    pcINO_joystickPosition[0].y = 0.0;
    switch (mode)
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
    /* Retail numbers these 128 and 129 (jump table at 0x00424764
       over a1-1, entries only for 1/2/4/8/32/64/128/129). The demo
       build these were decompiled from used 100 and 101, so
       INO_SetControlMode(128) -- which the board panel really does
       call -- was falling through to default and leaving whatever
       handler was installed before. */
    case 128:
        sJoystickHandlerFunction = pcINO_InputHandlerCase100;
        pcINO_keyMaps = pcINO_keyMapsCase100;
        /* 0x004246F2: mode 128 starts with the stick fully over,
           set after the common code above has zeroed x and y. */
        pcINO_joystickPosition[0].y = 1.0f;
        break;
    case 129:
        sJoystickHandlerFunction = pcINO_InputHandlerCase101;
        pcINO_keyMaps = pcINO_keyMapsCase101;
        break;
    default:
        break;
    }
    pcINO_SelectedControlMode = mode;
    pcINO_SelectedControlSubmode = 0xffffffff;
    pcINO_SetControlSubmode(0);
}

/* The input. The game came off a console, so a pad is the only input
   it has: everything above this reads sInoPad and the two sticks, and
   the PC build's job is to fill them from the keyboard and the mouse.
   Which keys and which mouse axes do that is the selected control
   mode, and the last two calls here are what pick it.

   So there is no device to open. What used to be opened was a
   DirectInput object, and the only thing it was ever used for was
   making the mouse and keyboard devices; with those gone it opened
   nothing and was released again at close. It is not created any
   more. */
void pcINO_Joystick_Init(HWND hwnd)
{
    uint16_t swappedSecondary;

    pcINO_Hwnd = hwnd;

    /* The keyboard and the mouse come from the window now rather than
       from devices of their own, and the wheel arrives in whole
       notches, so the granularity is one. */
    PLT_InputCapture(1);
    mouseGranularity = 1;
    INO_l_Joystick_Enable = 1;
    INO_gi_CurrentPadId = 0;
    /* The primary button is the primary one, with no asking.
     *
       The device this used to read went underneath the system and
       reported the physical buttons, so a player who had swapped them
       got them the wrong way round unless the engine swapped its own
       codes back -- and the only way to ask whether they were swapped
       was to swap them and put them back, which is what the two calls
       here did.

       Input comes from the window now, and a window is delivered the
       buttons the way the player set them up: the primary button
       arrives as the primary one. Asking again would swap them a
       second time and hand a left-handed player exactly the problem
       the asking was there to prevent. */
    pcINO_IntKeybDataLink[0] = 258;
    swappedSecondary = 259;
    pcINO_IntKeybDataLink[2] = swappedSecondary;
    pcINO_IntKeybDataLink[32] = swappedSecondary;

    pcINO_SetControlMode(4);
    pcINO_SetControlSubmode(8);
    pcINO_getKeyNamesForEverything();
}

void pcINO_Joystick_Close()
{
    PLT_InputCapture(0);
    INO_l_Joystick_Enable = 0;
}

void pcINO_Joystick_Move(Vec3* move, int stickID)
{
    move->x = pcINO_joystickPosition[stickID].x;
    move->y = pcINO_joystickPosition[stickID].y;
    move->z = pcINO_joystickPosition[stickID].z;
}

int pcINO_i_IsMouseBeingUsed()
{
    return 1;
}

int pcINO_i_SetCurrentPad(int pad)
{
    INO_gi_CurrentPadId = 0;
    return -(pad != 0);
}

void pcINO_JoystickPifPafEnable(BOOL enable) {
}

void pcINO_JoystickPafIntensity(int intensity) {
}

/* Taking the input and giving it back. It was two devices held
   exclusively; it is the pointer locked to the window, and the game
   being read the keyboard at all. The names stay because the engine
   calls them from several places. */
void pcINO_mouse_and_keyb_acquire()
{
    PLT_InputCapture(1);
    pcINO_numLockAtAcquire = PLT_b_NumLockOn();
    memset(pcINO_keyboardDataBackup, 0, sizeof(pcINO_keyboardDataBackup));
}

void pcINO_mouse_and_keyb_unacquire()
{
    PLT_InputCapture(0);
}

/* bsearch hands the comparator a void* for the key and a void* for
   the element. The key is the name on its own and the element is a
   whole tweak, whose name is its first field -- which is why this
   used to be strcmp passed straight in, reading the element as if
   it were the string. Spelling it out says that on purpose rather
   than leaving it to the layout. */
static int inp_l_i_TweakNameCompare(const void* key, const void* elem)
{
    return strcmp((const char*)key, ((const tTweakElem*)elem)->elemname);
}

// One command line: a tweak name and a value. Split out of the old
// dialog path so the overlay's command field runs the same thing.
void pcINO_RunTweakCommand(const char* line)
{
    tTweakElem* tweak;
    float value = 0.0;
    char Key[32];

    if (!line || !line[0])
        return;
    Key[0] = 0;
    sscanf(line, "%31s %f", Key, &value);
    if (!Key[0])
        return;
    /* The count comes from beside the table rather than being written
       out here. As a literal it was right only while it matched, and
       the entry it would drop first is the one that sorts last. */
    tweak = (tTweakElem*)bsearch(Key, &tweaks, (size_t)tweaksNum,
                                 sizeof(tTweakElem), inp_l_i_TweakNameCompare);
    if (tweak)
    {
        if (tweak->type == 0)
            *((int32_t*)(tweak->ptr)) = (uint64_t)value;
        else if (tweak->type == 1)
            *((float*)(tweak->ptr)) = value;
        else if (tweak->type == 2)
            ((void(*)(float))tweak->ptr)(value);
    }
}

void pcINO_Joystick_Update(struct sInoPad* padData)
{
    int mouseData[5];
    /* 266, not 264: retail writes mouse buttons 7 and 8 at
       [esp+0x138]/[esp+0x139] (0x00423026, 0x00423035), i.e.
       keyboardData[264] and [265], and the backup copy at the end
       is 266 bytes. The demo decompile put those two in locals. */
    uint8_t keyboardData[266];
    /* The handler takes { lX * sensitivity, lY * sensitivity } as
       floats and the wheel as an INT in the third slot. Slots 0 and 1
       go through fild/fmul/fstp at 0x00422F41 and 0x00422F5B; the
       wheel's idiv quotient is stored with a plain mov at 0x00423047
       and is never converted. The handlers read it with
       ((int*)mouse)[2]. */
    float mouseScaled[3];

    memset(padData->padData, 0, sizeof(padData->padData));

    /* Both devices in one answer, from the window's own events. There
       is nothing here about losing the device and taking it again:
       that was the exclusive device's to lose, and the keyboard a
       window is given is not held that way. */
    PLT_ReadDevices(keyboardData, mouseData);

    keyboardData[0] = 0;
    keyboardData[256] = mouseData[2] <= 0 ? 0 : 0x80;
    keyboardData[257] = mouseData[2] >= 0 ? 0 : 0x80;
    keyboardData[258] = (mouseData[3] & 0xff) == 0 ? 0 : 0x80;
    keyboardData[259] = ((mouseData[3] >> 8) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[260] = ((mouseData[3] >> 16) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[261] = ((mouseData[3] >> 24) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[262] = (mouseData[4] & 0xff) == 0 ? 0 : 0x80;
    keyboardData[263] = ((mouseData[4] >> 8) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[264] = ((mouseData[4] >> 16) & 0xff) == 0 ? 0 : 0x80;
    keyboardData[265] = ((mouseData[4] >> 24) & 0xff) == 0 ? 0 : 0x80;

    /* 0x00422F41 / 0x00422F5B: halve the pointer deltas.
       0x0042303D: idiv by mouseGranularity turns the wheel delta into
       whole notches before any handler sees it. */
    mouseScaled[0] = (float)mouseData[0] * pcINO_mouseSensitivity;
    mouseScaled[1] = (float)mouseData[1] * pcINO_mouseSensitivity;
    ((int*)mouseScaled)[2] = mouseData[2] / (mouseGranularity ? mouseGranularity : 1);

    // What the mouse did this frame, for anything that wants the
    // device rather than a control mode's reading of it.
    pcINO_mouseDeltaX = mouseData[0];
    pcINO_mouseDeltaY = mouseData[1];

    // No control mode runs while the input is taken from the game.
    // Clearing the pad after letting one run would not do instead:
    // the handlers accumulate -- the wheel's notches, case 4's
    // pointer travel -- and the whole running total would arrive in
    // one lump the moment the input was given back. The pad itself
    // needs nothing here; the memset at the top of the function has
    // already cleared it, which is what the game then reads.
    if (!INO_b_InputSuspended())
    {
        sJoystickHandlerFunction(padData, mouseScaled, keyboardData);
        if (keyboardData[pcINO_IntKeybDataLink[30]] || keyboardData[pcINO_IntKeybDataLink[31]])
            padData->padData[9] = -1;
        if (keyboardData[pcINO_IntKeybDataLink[28]] || keyboardData[pcINO_IntKeybDataLink[29]])
            padData->padData[8] = -1;
    }
    /* Alt+F4 used to be caught here and played back to the system as
       four key events, because an exclusively held keyboard never let
       it reach the window. The keyboard is not held that way now: the
       window gets the combination itself and the close arrives as an
       event, so catching it here would ask to close twice. */
    /* 87 is F11, 88 is F12. The field was on F12 and is on F11; the
       statistics the window's caption used to show on a title-bar
       click are on Home. */
    if (keyboardData[87] && !pcINO_keyboardDataBackup[87] && !OVL_b_CommandActive())
    {
        // F11 raises the overlay's command field. The input is given
        // back while the field is up, so the pointer is free and the
        // characters arrive as typed text rather than as game keys;
        // Enter in the field runs the line and takes it again.
        if (!tweakFlagsDisabled)
        {
            pcINO_mouse_and_keyb_unacquire();
            OVL_CommandShow();
        }
    }
    memcpy(pcINO_keyboardDataBackup, keyboardData, sizeof(pcINO_keyboardDataBackup));
}

static int pcINO_l_TakeNextState()
{
    int next;

    next = INO_ge_SavSetNextState;
    INO_ge_SavSetNextState = -1;
    return next == -1 ? 0 : next;
}

static int pcINO_l_SavRead(int nextState)
{
    if (INO_gst_SavFileDesc.fp)
        fread(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return nextState;
}

static int pcINO_l_SavWrite(int nextState)
{
    if (INO_gst_SavFileDesc.fp)
        fwrite(INO_gst_SavFileDesc.saveBuffer, 1u, INO_gst_SavFileDesc.size, INO_gst_SavFileDesc.fp);
    return nextState;
}

int pcINO_SavModuleInit() {
    return 0;
}

int pcINO_SavModuleClose()
{
    int result;

    result = 0;
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

    if (pcINO_savPresenceFailure) // TODO, FIXME: ezt nem kéne initelni?
    {
        pcINO_gi_LastError = 304;
        INO_gi_SavButton[0] = 3;
        INO_gi_SavButton[1] = 6;
        INO_gi_SavButton[2] = 0;
        INO_gi_SavButton[3] = 0;
        INO_gi_SavButton[4] = 24;
        INO_gi_SavButton[5] = 26;
        INO_gi_SavButton[6] = 0;
        INO_gi_SavButton[7] = 0;
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
    if (!pcINO_savMountFailure) // TODO FIXME nem kéne initelni?
        return 4;
    if (pcINO_savMountFailure == 1)
        pcINO_gi_LastError = 5;
    else
        pcINO_gi_LastError = pcINO_savMountFailure != 2 ? 301 : 3;
    INO_gi_SavButton[0] = 3;
    INO_gi_SavButton[1] = 6;
    INO_gi_SavButton[2] = 5;
    INO_gi_SavButton[3] = 0;
    INO_gi_SavButton[4] = 24;
    INO_gi_SavButton[5] = 26;
    INO_gi_SavButton[6] = 5;
    INO_gi_SavButton[7] = 0;
    return 27;
}

int pcINO_i_UnmountCard() {
    return 0;
}

int pcINO_i_CheckCard()
{
    if (!pcINO_savCheckFailure) // TODO FIXME nem kéne initelni?
        return 6;
    pcINO_gi_LastError = 602;
    INO_gi_SavButton[0] = 3;
    INO_gi_SavButton[1] = 0;
    INO_gi_SavButton[2] = 0;
    INO_gi_SavButton[3] = 6;
    INO_gi_SavButton[4] = 24;
    INO_gi_SavButton[5] = 0;
    INO_gi_SavButton[6] = 0;
    INO_gi_SavButton[7] = 26;
    return 27;
}

int pcINO_i_FormatCard()
{
    INO_gi_SavButton[5] = 0;
    INO_gi_SavButton[1] = 0;
    if (pcINO_savFormatFailure) // TODO FIXME nem kéne initelni?
    {
        pcINO_gi_LastError = 705;
        INO_gi_SavButton[0] = 3;
        INO_gi_SavButton[2] = 5;
        INO_gi_SavButton[3] = 6;
        INO_gi_SavButton[4] = 24;
        INO_gi_SavButton[6] = 5;
        INO_gi_SavButton[7] = 26;
    }
    else
    {
        pcINO_gi_LastError = 704;
        INO_gi_SavButton[0] = 4;
        INO_gi_SavButton[2] = 0;
        INO_gi_SavButton[3] = 0;
        INO_gi_SavButton[4] = 1;
        INO_gi_SavButton[6] = 0;
        INO_gi_SavButton[7] = 0;
    }
    return 27;
}

int pcINO_i_TestFreeSpace()
{
    long long free;
    char      dir[512];

    /* Free space where the saves are, clamped to a positive int. The
       player's own directory rather than the current drive, which is
       what this used to ask about and is only the same drive by
       accident; a check against the wrong volume either refuses a
       save that would fit or starts one that cannot finish. A
       directory that cannot be named, or a question the file system
       will not answer, reports nothing free, which makes the caller
       refuse the write. */
    if (!DPT_b_UserPath("", dir, sizeof(dir)))
        dir[0] = 0;

    free = dir[0] ? FIO_i64_FreeSpace(dir) : -1;
    if (free < 0)
        INO_gi_FreeSpace = 0;
    else if (free > 0x7FFFFFFF)
        INO_gi_FreeSpace = 0x7FFFFFFF;
    else
        INO_gi_FreeSpace = (int)free;

    return pcINO_l_TakeNextState();
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
    char path[512];

    /* The saves are the player's, so they go in the player's own
       directory rather than beside the game, which may be installed
       somewhere nobody can write to. Resolved here, at the one place
       the name reaches the filesystem, rather than at each of the
       dozen places the card manager sets it.

       A directory that cannot be named is treated as a file that will
       not open: the save machine already has that path, and it is the
       one that tells the player rather than failing silently. */
    if (!DPT_b_UserPath(INO_gsz_SavFileName, path, sizeof(path)))
        path[0] = 0;

    INO_gst_SavFileDesc.fp = path[0] ? fopen(path, "r+b") : NULL;
    if (!INO_gst_SavFileDesc.fp)
    {
        if (INO_ge_SavCommand == 16 && INO_gi_FreeSpace < SAV_PHOTO_BUFFER_SIZE
            || INO_ge_SavCommand == 10 && INO_gi_FreeSpace < 1024)
        {
            pcINO_gi_LastError = 302;
            INO_gi_SavButton[0] = 3;
            INO_gi_SavButton[1] = 0;
            INO_gi_SavButton[2] = 0;
            INO_gi_SavButton[3] = 6;
            INO_gi_SavButton[4] = 24;
            INO_gi_SavButton[5] = 0;
            INO_gi_SavButton[6] = 0;
            INO_gi_SavButton[7] = 26;
            INO_ge_SavSetNextState = INO_ge_SavCommand;
            return 27;
        }

        INO_gst_SavFileDesc.fp = path[0] ? fopen(path, "w+b") : NULL;
        if (!INO_gst_SavFileDesc.fp)
        {
            if (INO_ge_SavCommand == 10 || INO_ge_SavCommand == 16)
                pcINO_gi_LastError = 308;
            else
                pcINO_gi_LastError = 405;
            INO_gi_SavButton[0] = 3;
            INO_gi_SavButton[1] = 0;
            INO_gi_SavButton[2] = 0;
            INO_gi_SavButton[3] = 6;
            INO_gi_SavButton[4] = 24;
            INO_gi_SavButton[5] = 0;
            INO_gi_SavButton[6] = 0;
            INO_gi_SavButton[7] = 26;
            INO_ge_SavSetNextState = INO_ge_SavCommand;
            return 27;
        }
    }
    return pcINO_l_TakeNextState();
}

int pcINO_i_Close()
{
    if (INO_gst_SavFileDesc.fp)
        fclose(INO_gst_SavFileDesc.fp);
    INO_gst_SavFileDesc.fp = 0;
    return pcINO_l_TakeNextState();
}

int pcINO_i_ReadIndex_1()
{
    return pcINO_l_SavRead(9);
}

int pcINO_i_WriteIndex()
{
    return pcINO_l_SavWrite(22);
}

int pcINO_i_ReadSlot_1()
{
    return pcINO_l_SavRead(14);
}

int pcINO_i_ReadSlot_2()
{
    return pcINO_l_SavRead(15);
}

int pcINO_i_ReadSlot_3()
{
    return pcINO_l_SavRead(22);
}

int pcINO_i_WriteSlot_C()
{
    return pcINO_l_SavWrite(19);
}

int pcINO_i_WriteSlot_E()
{
    return pcINO_l_SavWrite(28);
}

int pcINO_i_CheckFreeSpace()
{
    pcINO_gi_LastError = 0;
    if (!pcINO_savFreeSpaceFailure) // TODO FIXME nem kéne initelni?
        return 0;
    pcINO_gi_LastError = 302;
    INO_gi_SavButton[0] = 3;
    INO_gi_SavButton[1] = 6;
    INO_gi_SavButton[2] = 0;
    INO_gi_SavButton[3] = 0;
    INO_gi_SavButton[4] = 24;
    INO_gi_SavButton[5] = 26;
    INO_gi_SavButton[6] = 0;
    INO_gi_SavButton[7] = 0;
    return 27;
}

int pcINO_i_TestCard()
{
    pcINO_gi_LastError = 0;
    if (!pcINO_savCardTestFailure) // TODO FIXME nem kéne initelni?
        return 0;
    pcINO_gi_LastError = 310;
    memset(INO_gi_SavButton, 0, sizeof(INO_gi_SavButton));
    return 27;
}