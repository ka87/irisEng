#pragma once
#include <stdarg.h>

#define MOCK_LOG_MAX  40000
#define MOCK_LOG_LINE 96

void MOCK_LogReset(void);
int MOCK_LogNum(void);
const char* MOCK_LogLine(int i);
int MOCK_LogOverflowed(void);

void MOCK_HeapReset(void);
const void* MOCK_HeapBase(void);
unsigned int MOCK_HeapUsed(void);
void MOCK_RegisterScene(const void* base, unsigned int size);
void MOCK_FramesReset(void);

void MOCK_FileSet(const unsigned char* bytes, int num);
char* MOCK_FileBase(void);

extern int MOCK_isSpecial;
extern int MOCK_aiStackValue;
