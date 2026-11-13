#pragma once
#include <stdarg.h>

#define COLB_LOG_MAX 4096

extern char COLB_log[COLB_LOG_MAX][64];
extern int COLB_logLen;
extern int COLB_logOverflow;

void COLB_LogReset(void);
void COLB_Log(const char* fmt, ...);
