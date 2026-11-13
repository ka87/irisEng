#pragma once
#include <stdarg.h>

#define STRMOCK_LOGMAX 4096

extern int STRMOCK_logLen;
extern char STRMOCK_log[STRMOCK_LOGMAX][96];
extern struct sGroGeneric* STRMOCK_gro;
extern int STRMOCK_fontSize;
extern int STRMOCK_fontKey;

void STRMOCK_Reset(void);
void STRMOCK_BuildFont(int glyphs);

/* The world the cases run against. Owned by test_strings.c and shared
   by both copies, which is why the driver restores it between runs. */
extern struct sGEOSTRStruct* STRW_str;
extern struct sGameObjectMain* STRW_gao;
extern struct sGEOMAT* STRW_mat;
extern struct sEngTexIntermediateStruct* STRW_tex;
extern struct sFontData STRW_fontBuf;
extern int g_matVariant;

extern int STRCASE_ri[8];
extern float STRCASE_rf[8];
extern char STRCASE_rs[1024];
