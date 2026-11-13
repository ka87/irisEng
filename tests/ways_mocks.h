#pragma once
#include <stdint.h>
#include <stddef.h>

/* The mock engine the ways.c differential test runs both copies
   against. Every call is logged in order with its arguments, and the
   allocator is a bump allocator over one arena that the driver rewinds
   between the two sides so the pointers the two copies see are
   identical and the scene can be compared with memcmp. */

#define WMOCK_LOG_MAX 8192
#define WMOCK_ARENA_SIZE (1u << 20)

typedef struct
{
    const char* name;
    long a;
    long b;
    long c;
} tWMockCall;

extern tWMockCall WMOCK_log[WMOCK_LOG_MAX];
extern int WMOCK_logLen;
extern unsigned char* WMOCK_arena;
extern size_t WMOCK_used;

/* The byte stream BIG_pc_ReadFileTmp hands the loader callbacks. */
extern unsigned char WMOCK_stream[4096];
extern int WMOCK_streamSize;
extern int WMOCK_returnNullStream;

void WMOCK_Reset(void);
void WMOCK_Log(const char* name, long a, long b, long c);
long WMOCK_Offset(const void* p);
