#pragma once
#include "common.h"
#include "world.h"
#include "gdi.h"
#include "soft.h"

#define CMD_NOSOUND "NOSOUND"
#define CMD_BIN "B"
#define CMD_DBLBF "DBLBF"
#define CMD_SINGLESTREAM "SINGLESTREAM"
#define CMD_WINDOWED "WINDOWED"

typedef struct GLOBAL_MainHandles
{
    HWND hwndSetAfterCreateWindow;
    HWND hwnd;
    struct sWorld* currentWorld;
    struct sGdi_DisplayData* displayData;
} GLOBAL_MainHandles;

GLOBAL_MainHandles MAI_gst_MainHandles;
unsigned int ENG_gul_Loop;

extern int ENG_gb_ActiveSectorization;
extern int ENG_PlatformID;
extern int ENG_gb_NeedToReinit;
extern int ENG_WindowedMode;

void (*ENG_gp_Display)(HWND, tGdi_DisplayData*);
void (*ENG_gp_Input)();
void (*ENG_gp_Engine)();

void ENG_ReinitOneObject(struct sGameObjectMain* a1, int a2);
void ENG_ReinitOneWorld(struct sWorld* a1, int a2);
void ENG_GetStatsText(char* Buffer);
void ENG_CloseApplication();
void ENG_InitApplication();
void ENG_ParseCommandLine(const char* cmd);
void ENG_InitEngine();
void ENG_Loop();
void ENG_CloseEngine();