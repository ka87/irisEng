#pragma once
#include "common.h"
#include "win/wingdi.h"

typedef struct sTweakElem
{
    char elemname[32];
    int type;
    void* ptr;
} tTweakElem;

CHAR dbgDialogBuff[128];
int tweakFlagsDisabled;
extern struct sTweakElem tweaks[17];

extern int tweak_perf;
extern float tweak_perf_fps_min;
extern float tweak_perf_fps_target;
extern float tweak_mipmaplodbias;
extern int tweak_ae_nobands;
extern int tweak_water_blur_normal;
extern float tweak_water_smoothx;
extern float tweak_water_smoothy;
extern int tweak_water_smooth;
extern int tweak_water_clip;
extern float tweak_water_off;
extern float tweak_water_min_bv;
extern int tweak_ae_no;

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine, const char* error, const char* extra, int showMsgBox);
void DBG_OutputDebugString(char* Format, ...);
void D3D_OutputDebugString(char* Format, ...);
void DBG_DumpMatrix(Mtx4x4* mtx);
void DBG_DumpVec(Vec3* vec);
void DBG_DumpVecNoNL(Vec3* vec);

// tmp
//void DBG_DumpSallyHeadPos(char* tag);
