#pragma once
/* Same include order as win/wingdi.c's own: the engine headers are
   order sensitive. */
#include "../profiler.h"
#include "../time.h"
#include "../memory.h"
#include "../textures.h"
#include "../dbg.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/staticlod.h"
#include "../water.h"
#include "../engine.h"
#include "../win/wininputoutput.h"
#include "../resource.h"
#include "../win/wingdi.h"

#define WGM_LOG_MAX 262144
#define WGM_ARGS 8

typedef struct sWgmCall
{
    const char* fn;
    int nargs;
    long long arg[WGM_ARGS];
} tWgmCall;

extern tWgmCall WGM_log[WGM_LOG_MAX];
extern int WGM_logLen;
extern int WGM_overflow;

void WGM_Reset(void);
void WGM_Record(const char* fn, int nargs, ...);
/* A stable small integer for a pointer, so the log does not depend on
   where the allocator happened to put things. */
long long WGM_Index(const void* p);
void WGM_Register(const void* p, int id);

/* ---- the mock Direct3D objects ---- */

#define WGM_TEXTURES 48
#define WGM_SURFACES 48
#define WGM_VBUFFERS 64
#define WGM_IBUFFERS 16
#define WGM_DECLS 8

extern IDirect3DDevice9 WGM_dev;
extern IDirect3D9 WGM_d3d;
extern IDirect3DTexture9 WGM_tex[WGM_TEXTURES];
extern IDirect3DSurface9 WGM_surf[WGM_SURFACES];
extern IDirect3DVertexBuffer9 WGM_vb[WGM_VBUFFERS];
extern IDirect3DIndexBuffer9 WGM_ib[WGM_IBUFFERS];
extern IDirect3DVertexDeclaration9 WGM_decl[WGM_DECLS];

/* Every Lock() hands out a slice of this, so what a function writes
   into a vertex or index buffer is comparable between the two sides. */
#define WGM_LOCK_BYTES (1 << 20)
extern unsigned char WGM_lockMem[WGM_LOCK_BYTES];
extern int WGM_lockUsed;

/* Surface LockRect pitch and backing store. */
#define WGM_SURF_BYTES (1 << 18)
extern unsigned char WGM_surfMem[WGM_SURF_BYTES];

/* What CreateTexture/CreateVertexBuffer etc. hand back next, and what
   they return.  A negative result drives the out-of-video-memory arm. */
extern int WGM_createFailEvery;
extern int WGM_createCount;

/* Deterministic clocks. */
extern unsigned int WGM_ticks;
extern long long WGM_qpc;
extern long long WGM_qpcStep;

/* What the GetDesc of a surface reports. */
extern unsigned int WGM_descWidth;
extern unsigned int WGM_descHeight;
extern unsigned int WGM_descFormat;

/* The adapter identifier D3D_ApplyRenderingPreesets... reads. */
extern unsigned int WGM_vendorId;
extern unsigned int WGM_deviceId;

/* What CheckDeviceFormat / CheckDepthStencilMatch answer, as a bit per
   call index, so D3D_SelectSupportedDeviceFormat takes every arm. */
extern unsigned int WGM_formatMask;
extern int WGM_formatCall;

void WGM_InstallDevice(void);
unsigned long long WGM_HashState(void);
