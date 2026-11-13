/* Mocks for the win/wingdi_aftereffects.c differential test. */
#pragma once
#include "../win/wingdi.h"

#define AEMOCK_LOG_MAX   60000
#define AEMOCK_ARGS      6

typedef struct sAeMockCall
{
    const char* fn;
    int         nargs;
    long long   arg[AEMOCK_ARGS];
} tAeMockCall;

extern tAeMockCall AEMOCK_log[AEMOCK_LOG_MAX];
extern int         AEMOCK_logLen;
extern int         AEMOCK_overflow;

/* the 256-entry depth-blur ramp the locked surface hands out */
#define AEMOCK_RAMP 256
extern unsigned int AEMOCK_ramp[AEMOCK_RAMP];

/* textures the driver hands the code under test */
#define AEMOCK_TEXTURES 12
extern IDirect3DTexture9* AEMOCK_tex[AEMOCK_TEXTURES];

/* what GetDesc reports for a surface of a mock texture */
extern unsigned int AEMOCK_descWidth;
extern unsigned int AEMOCK_descHeight;

void AEMOCK_Reset(void);
void AEMOCK_Record(const char* fn, int nargs, ...);
int  AEMOCK_Index(const void* p);
void AEMOCK_Init(void);
