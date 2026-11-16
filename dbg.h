#pragma once
#include "common.h"
#include "gdi.h"

typedef struct sTweakElem
{
    char elemname[32];
    int type;
    void* ptr;
} tTweakElem;

CHAR dbgDialogBuff[128];
int tweakFlagsDisabled;
/* Declared without a size: the table grows, and a size written here
   as well is a second place to forget. How many there are is
   tweaksNum, below. */
extern struct sTweakElem tweaks[];
/* How many of them there are, defined beside the table itself.
 *
   The console's bsearch used to carry the count as a literal, which
   worked only while the literal and the table agreed: adding an entry
   silently pushed the last one -- zonalpha_no, which sorts last -- out
   of the searched range, and nothing would have said so. The count
   travels with the table now, and the test harness defines its own
   beside its own table. */
extern const int tweaksNum;

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
/* Which winding the OpenGL renderer culls: 0 as it decides, 1 none at
   all, 2 the facing reversed. See the table in dbg.c. */
extern int tweak_gl_cull;
extern int tweak_gl_water_flip;

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine, const char* error, const char* extra, int showMsgBox);
void DBG_OutputDebugString(char* Format, ...);
/* The log's first items and its last: the engine, its version, when it
   was built and what for, between two rules; and one line at the end,
   whose absence says the run did not reach the end of WinMain. */
void DBG_LogBanner(void);
void DBG_LogExit(void);
void GDI_OutputDebugString(char* Format, ...);
void DBG_DumpMatrix(Mtx4x4* mtx);
void DBG_DumpVec(Vec3* vec);
void DBG_DumpVecNoNL(Vec3* vec);

// tmp
//void DBG_DumpSallyHeadPos(char* tag);
