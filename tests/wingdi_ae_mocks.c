/* Mocks for the win/wingdi_aftereffects.c differential test.
 *
 * The file under test talks to one thing: the Direct3D device, through
 * its vtable. So the device, its textures and their surfaces are mock
 * objects whose every entry point appends to one ordered call log, and
 * the log is what the two sides are compared on. DrawPrimitiveUP
 * additionally dumps the vertex buffer it was handed, word by word, so
 * a moved UV or a narrowed coordinate shows up as a differing word
 * rather than as an identical call.
 *
 * Everything outside the device is five engine entry points and a
 * handful of globals; all of them are here, and the ones that return
 * something return it deterministically. */

#include "wingdi_ae_mocks.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tAeMockCall AEMOCK_log[AEMOCK_LOG_MAX];
int AEMOCK_logLen;
int AEMOCK_overflow;

unsigned int AEMOCK_ramp[AEMOCK_RAMP];
unsigned int AEMOCK_descWidth = 640;
unsigned int AEMOCK_descHeight = 480;

/* ---- pointer identity ---- */

#define AEMOCK_PTRS 128
static const void* ptrTable[AEMOCK_PTRS];
static int ptrCount;

static void RegisterPtr(const void* p)
{
    if (ptrCount < AEMOCK_PTRS)
        ptrTable[ptrCount++] = p;
}

int AEMOCK_Index(const void* p)
{
    int i;

    if (!p)
        return -1;
    for (i = 0; i < ptrCount; i++)
        if (ptrTable[i] == p)
            return i;
    return -2;
}

void AEMOCK_Record(const char* fn, int nargs, ...)
{
    tAeMockCall* c;
    va_list ap;
    int i;

    if (AEMOCK_logLen >= AEMOCK_LOG_MAX)
    {
        AEMOCK_overflow = 1;
        return;
    }
    c = &AEMOCK_log[AEMOCK_logLen++];
    c->fn = fn;
    c->nargs = nargs > AEMOCK_ARGS ? AEMOCK_ARGS : nargs;
    va_start(ap, nargs);
    for (i = 0; i < c->nargs; i++)
        c->arg[i] = va_arg(ap, long long);
    va_end(ap);
}

void AEMOCK_Reset(void)
{
    AEMOCK_logLen = 0;
    AEMOCK_overflow = 0;
}

/* ---- the surface ---- */

typedef struct sAeSurface
{
    IDirect3DSurface9Vtbl* lpVtbl;
    int                    owner;      /* texture index, -1 for the back buffer */
    void*                  bits;
} tAeSurface;

static ULONG SurfRelease(IDirect3DSurface9* s)
{
    AEMOCK_Record("Surface::Release", 1, (long long)AEMOCK_Index(s));
    return 0;
}

static HRESULT SurfGetDesc(IDirect3DSurface9* s, D3DSURFACE_DESC* d)
{
    AEMOCK_Record("Surface::GetDesc", 1, (long long)AEMOCK_Index(s));
    memset(d, 0, sizeof(*d));
    d->Width = AEMOCK_descWidth;
    d->Height = AEMOCK_descHeight;
    return 0;
}

static HRESULT SurfLockRect(IDirect3DSurface9* s, D3DLOCKED_RECT* r, const RECT* rc, DWORD f)
{
    tAeSurface* self = (tAeSurface*)s;

    AEMOCK_Record("Surface::LockRect", 3, (long long)AEMOCK_Index(s),
                  (long long)(rc != NULL), (long long)f);
    r->Pitch = AEMOCK_RAMP * 4;
    r->pBits = self->bits;
    return 0;
}

static HRESULT SurfUnlockRect(IDirect3DSurface9* s)
{
    AEMOCK_Record("Surface::UnlockRect", 1, (long long)AEMOCK_Index(s));
    return 0;
}

static IDirect3DSurface9Vtbl surfVtbl = {
    SurfRelease, SurfGetDesc, SurfLockRect, SurfUnlockRect
};

/* ---- the texture ---- */

typedef struct sAeTexture
{
    IDirect3DTexture9Vtbl* lpVtbl;
    tAeSurface             surface;
} tAeTexture;

static ULONG TexRelease(IDirect3DTexture9* t)
{
    AEMOCK_Record("Texture::Release", 1, (long long)AEMOCK_Index(t));
    return 0;
}

static HRESULT TexGetSurfaceLevel(IDirect3DTexture9* t, UINT level, IDirect3DSurface9** out)
{
    tAeTexture* self = (tAeTexture*)t;

    AEMOCK_Record("Texture::GetSurfaceLevel", 2, (long long)AEMOCK_Index(t),
                  (long long)level);
    *out = (IDirect3DSurface9*)&self->surface;
    return 0;
}

static HRESULT TexAddDirtyRect(IDirect3DTexture9* t, const RECT* r) { return 0; }
static DWORD   TexGetLevelCount(IDirect3DTexture9* t) { return 1; }
static HRESULT TexGetLevelDesc(IDirect3DTexture9* t, UINT l, D3DSURFACE_DESC* d) { return 0; }

static IDirect3DTexture9Vtbl texVtbl = {
    TexRelease, TexGetSurfaceLevel, TexAddDirtyRect, TexGetLevelCount, TexGetLevelDesc
};

/* ---- the device ---- */

static HRESULT DevSetRenderState(IDirect3DDevice9* d, D3DRENDERSTATETYPE s, DWORD v)
{
    AEMOCK_Record("SetRenderState", 2, (long long)(unsigned int)s,
                  (long long)(unsigned int)v);
    return 0;
}

static HRESULT DevSetTextureStageState(IDirect3DDevice9* d, DWORD stage,
                                       D3DTEXTURESTAGESTATETYPE s, DWORD v)
{
    AEMOCK_Record("SetTextureStageState", 3, (long long)(unsigned int)stage,
                  (long long)(unsigned int)s, (long long)(unsigned int)v);
    return 0;
}

static HRESULT DevSetSamplerState(IDirect3DDevice9* d, DWORD samp, DWORD s, DWORD v)
{
    AEMOCK_Record("SetSamplerState", 3, (long long)(unsigned int)samp,
                  (long long)(unsigned int)s, (long long)(unsigned int)v);
    return 0;
}

static HRESULT DevSetTexture(IDirect3DDevice9* d, DWORD stage, IDirect3DBaseTexture9* t)
{
    AEMOCK_Record("SetTexture", 2, (long long)(unsigned int)stage,
                  (long long)AEMOCK_Index(t));
    return 0;
}

static HRESULT DevSetVertexShader(IDirect3DDevice9* d, IDirect3DVertexShader9* s)
{
    AEMOCK_Record("SetVertexShader", 1, (long long)AEMOCK_Index(s));
    return 0;
}

static HRESULT DevSetFVF(IDirect3DDevice9* d, DWORD fvf)
{
    AEMOCK_Record("SetFVF", 1, (long long)(unsigned int)fvf);
    return 0;
}

static HRESULT DevDrawPrimitiveUP(IDirect3DDevice9* d, DWORD type, UINT count,
                                  const void* data, UINT stride)
{
    const unsigned int* w = (const unsigned int*)data;
    unsigned int words;
    unsigned int i;

    AEMOCK_Record("DrawPrimitiveUP", 3, (long long)(unsigned int)type,
                  (long long)(unsigned int)count, (long long)(unsigned int)stride);
    /* a triangle strip of `count` primitives is count + 2 vertices */
    words = (stride * (count + 2)) / 4;
    for (i = 0; i < words; i++)
        AEMOCK_Record("vtx", 2, (long long)i, (long long)w[i]);
    return 0;
}

static HRESULT DevSetRenderTarget(IDirect3DDevice9* d, DWORD idx, IDirect3DSurface9* s)
{
    AEMOCK_Record("SetRenderTarget", 2, (long long)idx, (long long)AEMOCK_Index(s));
    return 0;
}

static HRESULT DevSetDepthStencilSurface(IDirect3DDevice9* d, IDirect3DSurface9* s)
{
    AEMOCK_Record("SetDepthStencilSurface", 1, (long long)AEMOCK_Index(s));
    return 0;
}

static tAeSurface backBufferSurface;
static tAeSurface depthStencilSurface;

static HRESULT DevGetRenderTarget(IDirect3DDevice9* d, DWORD idx, IDirect3DSurface9** out)
{
    AEMOCK_Record("GetRenderTarget", 1, (long long)idx);
    *out = (IDirect3DSurface9*)&backBufferSurface;
    return 0;
}

static HRESULT DevGetDepthStencilSurface(IDirect3DDevice9* d, IDirect3DSurface9** out)
{
    AEMOCK_Record("GetDepthStencilSurface", 0);
    *out = (IDirect3DSurface9*)&depthStencilSurface;
    return 0;
}

static HRESULT DevGetBackBuffer(IDirect3DDevice9* d, UINT sw, UINT bb,
                                D3DBACKBUFFER_TYPE t, IDirect3DSurface9** out)
{
    AEMOCK_Record("GetBackBuffer", 3, (long long)sw, (long long)bb, (long long)t);
    *out = (IDirect3DSurface9*)&backBufferSurface;
    return 0;
}

static HRESULT DevStretchRect(IDirect3DDevice9* d, IDirect3DSurface9* src, const RECT* sr,
                              IDirect3DSurface9* dst, const RECT* dr,
                              D3DTEXTUREFILTERTYPE f)
{
    AEMOCK_Record("StretchRect", 3, (long long)AEMOCK_Index(src),
                  (long long)AEMOCK_Index(dst), (long long)f);
    if (sr)
        AEMOCK_Record("rect.src", 4, (long long)sr->left, (long long)sr->top,
                      (long long)sr->right, (long long)sr->bottom);
    if (dr)
        AEMOCK_Record("rect.dst", 4, (long long)dr->left, (long long)dr->top,
                      (long long)dr->right, (long long)dr->bottom);
    return 0;
}

static HRESULT DevClear(IDirect3DDevice9* d, DWORD n, const D3DRECT* rects, DWORD flags,
                        D3DCOLOR col, float z, DWORD stencil)
{
    DWORD i;

    AEMOCK_Record("Clear", 4, (long long)(unsigned int)n, (long long)(unsigned int)flags,
                  (long long)(unsigned int)col, (long long)*(unsigned int*)&z);
    for (i = 0; rects && i < n; i++)
        AEMOCK_Record("clear.rect", 4, (long long)rects[i].x1, (long long)rects[i].y1,
                      (long long)rects[i].x2, (long long)rects[i].y2);
    return 0;
}

static HRESULT DevGetViewport(IDirect3DDevice9* d, D3DVIEWPORT9* vp)
{
    AEMOCK_Record("GetViewport", 0);
    vp->X = 3; vp->Y = 5; vp->Width = 640; vp->Height = 480;
    vp->MinZ = 0.0f; vp->MaxZ = 1.0f;
    return 0;
}

static HRESULT DevSetViewport(IDirect3DDevice9* d, const D3DVIEWPORT9* vp)
{
    AEMOCK_Record("SetViewport", 4, (long long)vp->X, (long long)vp->Y,
                  (long long)vp->Width, (long long)vp->Height);
    return 0;
}

static tAeTexture createdTexture;

static HRESULT DevCreateTexture(IDirect3DDevice9* d, UINT w, UINT h, UINT levels,
                                DWORD usage, D3DFORMAT fmt, D3DPOOL pool,
                                IDirect3DTexture9** out, HANDLE* shared)
{
    AEMOCK_Record("CreateTexture", 6, (long long)(unsigned int)w, (long long)(unsigned int)h,
                  (long long)(unsigned int)levels, (long long)(unsigned int)usage,
                  (long long)(unsigned int)fmt, (long long)(unsigned int)pool);
    *out = (IDirect3DTexture9*)&createdTexture;
    return 0;
}

static HRESULT DevColorFill(IDirect3DDevice9* d, IDirect3DSurface9* s, const RECT* r,
                            D3DCOLOR c)
{
    AEMOCK_Record("ColorFill", 2, (long long)AEMOCK_Index(s), (long long)(unsigned int)c);
    return 0;
}

static HRESULT DevUpdateTexture(IDirect3DDevice9* d, IDirect3DBaseTexture9* src,
                                IDirect3DBaseTexture9* dst)
{
    AEMOCK_Record("UpdateTexture", 2, (long long)AEMOCK_Index(src),
                  (long long)AEMOCK_Index(dst));
    return 0;
}

static IDirect3DDevice9Vtbl devVtbl;
static IDirect3DDevice9 device;

/* ---- the textures the driver hands out ---- */

static tAeTexture textures[AEMOCK_TEXTURES];
IDirect3DTexture9* AEMOCK_tex[AEMOCK_TEXTURES];

/* ---- globals the file under test reads ---- */

int ENG_ifSingleStreamThenEq2Otherwise8 = 8;
int tweak_ae_no;
int tweak_ae_nobands;
float tweak_water_smoothx = 1.25f;
float tweak_water_smoothy = 0.75f;

/* ---- engine entry points ---- */

void D3D_CheckReturnAndRunSettingsApplicationIfProblem(int hr)
{
    AEMOCK_Record("CheckReturn", 1, (long long)hr);
}

void D3D_SetVertexShader(IDirect3DVertexShader9* s)
{
    AEMOCK_Record("D3D_SetVertexShader", 1, (long long)AEMOCK_Index(s));
}

int D3D_CalculateStride(unsigned int fvf)
{
    int stride = 0;

    AEMOCK_Record("D3D_CalculateStride", 1, (long long)fvf);
    if (fvf & D3DFVF_XYZRHW) stride += 16;
    if (fvf & D3DFVF_DIFFUSE) stride += 4;
    if (fvf & D3DFVF_TEX1) stride += 8;
    if (fvf & D3DFVF_TEX2) stride += 16;
    if (fvf & D3DFVF_TEX4) stride += 32;
    return stride;
}

void D3D_GetBandFractions(float* top, float* bottom)
{
    AEMOCK_Record("D3D_GetBandFractions", 0);
    *top = 0.10416666f;
    *bottom = 0.89583331f;
}

void D3D_AspectTrace(const char* where, int viewX, int viewY, int viewW, int viewH)
{
    AEMOCK_Record("D3D_AspectTrace", 4, (long long)viewX, (long long)viewY,
                  (long long)viewW, (long long)viewH);
}

/* ---- the allocator math.c wants; math.c itself is linked whole, so
   MATH_TransformVector is the engine's own ---- */

void* MEM_p_AllocAligned(int size, int align)
{
    return calloc(1, (size_t)size + (size_t)align + 16);
}

void* MEM_p_ReallocAligned(void* p, int size, int align)
{
    return realloc(p, (size_t)size + (size_t)align + 16);
}

void MEM_FreeAlign(void* p)
{
    free(p);
}

/* ---- one-time wiring ---- */

static tGdi_DisplayData displayData;

void AEMOCK_Init(void)
{
    int i;

    devVtbl.SetRenderState = DevSetRenderState;
    devVtbl.SetTextureStageState = DevSetTextureStageState;
    devVtbl.SetSamplerState = DevSetSamplerState;
    devVtbl.SetTexture = DevSetTexture;
    devVtbl.SetVertexShader = DevSetVertexShader;
    devVtbl.SetFVF = DevSetFVF;
    devVtbl.DrawPrimitiveUP = DevDrawPrimitiveUP;
    devVtbl.SetRenderTarget = DevSetRenderTarget;
    devVtbl.SetDepthStencilSurface = DevSetDepthStencilSurface;
    devVtbl.GetRenderTarget = DevGetRenderTarget;
    devVtbl.GetDepthStencilSurface = DevGetDepthStencilSurface;
    devVtbl.GetBackBuffer = DevGetBackBuffer;
    devVtbl.StretchRect = DevStretchRect;
    devVtbl.Clear = DevClear;
    devVtbl.GetViewport = DevGetViewport;
    devVtbl.SetViewport = DevSetViewport;
    devVtbl.CreateTexture = DevCreateTexture;
    devVtbl.ColorFill = DevColorFill;
    devVtbl.UpdateTexture = DevUpdateTexture;
    device.lpVtbl = &devVtbl;

    for (i = 0; i < AEMOCK_TEXTURES; i++)
    {
        textures[i].lpVtbl = &texVtbl;
        textures[i].surface.lpVtbl = &surfVtbl;
        textures[i].surface.owner = i;
        textures[i].surface.bits = AEMOCK_ramp;
        AEMOCK_tex[i] = (IDirect3DTexture9*)&textures[i];
    }
    createdTexture.lpVtbl = &texVtbl;
    createdTexture.surface.lpVtbl = &surfVtbl;
    createdTexture.surface.owner = -2;
    createdTexture.surface.bits = AEMOCK_ramp;
    backBufferSurface.lpVtbl = &surfVtbl;
    backBufferSurface.owner = -1;
    backBufferSurface.bits = AEMOCK_ramp;
    depthStencilSurface.lpVtbl = &surfVtbl;
    depthStencilSurface.owner = -1;
    depthStencilSurface.bits = AEMOCK_ramp;

    /* registered in a fixed order so a logged pointer is the same number
       on both sides */
    RegisterPtr(&device);
    for (i = 0; i < AEMOCK_TEXTURES; i++)
    {
        RegisterPtr(&textures[i]);
        RegisterPtr(&textures[i].surface);
    }
    RegisterPtr(&createdTexture);
    RegisterPtr(&createdTexture.surface);
    RegisterPtr(&backBufferSurface);
    RegisterPtr(&depthStencilSurface);
    RegisterPtr(&displayData);

    D3DRenderParams.d3dDev = &device;
    D3DRenderParams.pDepthStencilSurface = (IDirect3DSurface9*)&depthStencilSurface;
    gpst_DX_stDD = &displayData;
}
