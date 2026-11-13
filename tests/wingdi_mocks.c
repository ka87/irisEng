/* Mocks for the win/wingdi.c differential test.
 *
 * The Direct3D device is a logging vtable: every method records its
 * name and arguments and returns a deterministic result, and Lock()
 * hands out a slice of one scratch buffer so what the renderer writes
 * into a vertex or index buffer can be compared between the two
 * copies.  Pointers go into the log through WGM_Index, so the log is
 * the same whatever the allocator did.
 *
 * The real math.c and profiler.c are linked rather than mocked.
 */
#include "wingdi_mocks.h"
#include "../math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

tWgmCall WGM_log[WGM_LOG_MAX];
int WGM_logLen;
int WGM_overflow;

IDirect3DDevice9 WGM_dev;
IDirect3D9 WGM_d3d;
IDirect3DTexture9 WGM_tex[WGM_TEXTURES];
IDirect3DSurface9 WGM_surf[WGM_SURFACES];
IDirect3DVertexBuffer9 WGM_vb[WGM_VBUFFERS];
IDirect3DIndexBuffer9 WGM_ib[WGM_IBUFFERS];
IDirect3DVertexDeclaration9 WGM_decl[WGM_DECLS];

unsigned char WGM_lockMem[WGM_LOCK_BYTES];
int WGM_lockUsed;
unsigned char WGM_surfMem[WGM_SURF_BYTES];

int WGM_createFailEvery;
int WGM_createCount;
unsigned int WGM_ticks;
long long WGM_qpc;
long long WGM_qpcStep = 7919;
unsigned int WGM_descWidth = 64;
unsigned int WGM_descHeight = 32;
unsigned int WGM_descFormat;
unsigned int WGM_vendorId;
unsigned int WGM_deviceId;
unsigned int WGM_formatMask;
int WGM_formatCall;

/* ---------- pointer identity ---------- */

#define WGM_PTRS 4096
static const void* WGM_ptr[WGM_PTRS];
static int WGM_ptrId[WGM_PTRS];
static int WGM_ptrCount;
static int WGM_ptrBase;
static int WGM_nextId;

void WGM_Register(const void* p, int id)
{
    if (!p || WGM_ptrCount >= WGM_PTRS)
        return;
    WGM_ptr[WGM_ptrCount] = p;
    WGM_ptrId[WGM_ptrCount] = id;
    WGM_ptrCount++;
}

long long WGM_Index(const void* p)
{
    int i;

    if (!p)
        return 0;
    for (i = 0; i < WGM_ptrCount; i++)
        if (WGM_ptr[i] == p)
            return WGM_ptrId[i];
    if (WGM_ptrCount >= WGM_PTRS)
        return -1;
    WGM_ptr[WGM_ptrCount] = p;
    WGM_ptrId[WGM_ptrCount] = ++WGM_nextId + 100000;
    return WGM_ptrId[WGM_ptrCount++];
}

void WGM_Record(const char* fn, int nargs, ...)
{
    va_list ap;
    int i;

    if (WGM_logLen >= WGM_LOG_MAX)
    {
        WGM_overflow = 1;
        return;
    }
    WGM_log[WGM_logLen].fn = fn;
    WGM_log[WGM_logLen].nargs = nargs;
    va_start(ap, nargs);
    for (i = 0; i < nargs && i < WGM_ARGS; i++)
        WGM_log[WGM_logLen].arg[i] = va_arg(ap, long long);
    va_end(ap);
    WGM_logLen++;
}

#define L0(n)          WGM_Record(n, 0)
#define L1(n,a)        WGM_Record(n, 1, (long long)(a))
#define L2(n,a,b)      WGM_Record(n, 2, (long long)(a), (long long)(b))
#define L3(n,a,b,c)    WGM_Record(n, 3, (long long)(a), (long long)(b), (long long)(c))
#define L4(n,a,b,c,d)  WGM_Record(n, 4, (long long)(a), (long long)(b), (long long)(c), (long long)(d))
#define L5(n,a,b,c,d,e) WGM_Record(n, 5, (long long)(a), (long long)(b), (long long)(c), (long long)(d), (long long)(e))
#define L7(n,a,b,c,d,e,f,g) WGM_Record(n, 7, (long long)(a), (long long)(b), (long long)(c), \
    (long long)(d), (long long)(e), (long long)(f), (long long)(g))

/* Floats go into the log as their bit pattern, so a changed rounding
   shows up rather than being lost to a long long conversion. */
static long long fbits(float f)
{
    unsigned int u;
    memcpy(&u, &f, 4);
    return (long long)u;
}

/* ---------- the allocator ---------- */

#define WGM_HEAP (12 << 20)
static unsigned char WGM_heap[WGM_HEAP];
static int WGM_heapUsed;
static int WGM_allocSeq;

/* Twice what is asked for, plus a guard band at each end.  The
   renderer sizes its pointer arrays as 4 bytes an entry, which is
   right on the 32-bit target and half of what a pointer needs here,
   and a frozen copy that writes a little way before its block must
   not kill the process.  Both sides get the same treatment, so the
   comparison is unaffected. */
#define WGM_SLACK 2

void* MEM_p_Alloc(unsigned int size)
{
    void* p;
    int want = (int)size * WGM_SLACK;

    if (WGM_heapUsed + want + 256 > WGM_HEAP)
    {
        fprintf(stderr, "wingdi mock heap exhausted\n");
        exit(2);
    }
    p = WGM_heap + WGM_heapUsed + 128;
    WGM_heapUsed += want + 256;
    WGM_heapUsed = (WGM_heapUsed + 15) & ~15;
    memset(p, 0, (size_t)want);
    WGM_Register(p, 500000 + (++WGM_allocSeq));
    L2("MEM_p_Alloc", size, WGM_allocSeq);
    return p;
}

void* MEM_p_Realloc(void* old, size_t size)
{
    void* p;

    L3("MEM_p_Realloc", WGM_Index(old), size, WGM_allocSeq + 1);
    p = MEM_p_Alloc((unsigned int)size);
    WGM_logLen--;                       /* the inner Alloc is not real */
    if (old)
        memcpy(p, old, size * WGM_SLACK);
    return p;
}

void MEM_Free(void* p)
{
    L1("MEM_Free", WGM_Index(p));
    /* nothing is handed back: the first side's state has to stay
       readable while the second side runs */
}

void* MEM_p_AllocAligned(int size, int align) { (void)align; return MEM_p_Alloc(size); }
void* MEM_p_ReallocAligned(void* p, unsigned int size, int align) { (void)align; return MEM_p_Realloc(p, size); }
void MEM_FreeAlign(void* p) { (void)p; }

/* ---------- clocks ---------- */

unsigned int TIM_GetTicksAsInt32() { return (WGM_ticks += 17); }
float TIM_f_Clock_TrueRead(void) { return (float)(WGM_ticks += 3) * 0.001f; }

BOOL QueryPerformanceCounter(LARGE_INTEGER* p)
{
    WGM_qpc += WGM_qpcStep;
    p->QuadPart = WGM_qpc;
    return 1;
}

BOOL QueryPerformanceFrequency(LARGE_INTEGER* p)
{
    p->QuadPart = 1000000;
    return 1;
}

/* ---------- the device ---------- */

static HRESULT Vt_CreateOffscreenPlainSurface(IDirect3DDevice9* d, UINT w, UINT h,
    D3DFORMAT f, D3DPOOL pool, IDirect3DSurface9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    L5("CreateOffscreenPlainSurface", w, h, f, pool, WGM_createCount);
    *out = &WGM_surf[(WGM_createCount++) % WGM_SURFACES];
    return 0;
}

static HRESULT Vt_StretchRect(IDirect3DDevice9* d, IDirect3DSurface9* a, const RECT* ra,
    IDirect3DSurface9* b, const RECT* rb, D3DTEXTUREFILTERTYPE f)
{
    (void)d; (void)ra; (void)rb;
    L3("StretchRect", WGM_Index(a), WGM_Index(b), f);
    return 0;
}

static HRESULT Vt_GetRenderTargetData(IDirect3DDevice9* d, IDirect3DSurface9* a,
    IDirect3DSurface9* b)
{
    (void)d;
    L2("GetRenderTargetData", WGM_Index(a), WGM_Index(b));
    return 0;
}

static HRESULT Vt_GetRenderTarget(IDirect3DDevice9* d, DWORD i, IDirect3DSurface9** out)
{
    (void)d;
    L1("GetRenderTarget", i);
    *out = &WGM_surf[0];
    return 0;
}

static HRESULT Vt_GetBackBuffer(IDirect3DDevice9* d, UINT a, UINT b,
    D3DBACKBUFFER_TYPE t, IDirect3DSurface9** out)
{
    (void)d;
    L3("GetBackBuffer", a, b, t);
    *out = &WGM_surf[1 + (a & 1)];
    return 0;
}

static HRESULT Vt_UpdateSurface(IDirect3DDevice9* d, IDirect3DSurface9* a, const RECT* ra,
    IDirect3DSurface9* b, const POINT* p)
{
    (void)d; (void)ra; (void)p;
    L2("UpdateSurface", WGM_Index(a), WGM_Index(b));
    return 0;
}

static HRESULT Vt_Clear(IDirect3DDevice9* d, DWORD n, const D3DRECT* r, DWORD flags,
    D3DCOLOR col, float z, DWORD stencil)
{
    (void)d; (void)r;
    L5("Clear", n, flags, col, fbits(z), stencil);
    return 0;
}

static HRESULT Vt_CreateTexture(IDirect3DDevice9* d, UINT w, UINT h, UINT lv, DWORD usage,
    D3DFORMAT f, D3DPOOL pool, IDirect3DTexture9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    L7("CreateTexture", w, h, lv, usage, f, pool, WGM_createCount);
    if (WGM_createFailEvery && (WGM_createCount % WGM_createFailEvery) == 0)
    {
        *out = NULL;
        WGM_createCount++;
        return (HRESULT)0x8007000EL;
    }
    *out = &WGM_tex[(WGM_createCount++) % WGM_TEXTURES];
    return 0;
}

/* DWORD is 32 bits on the target and 64 here, and the renderer passes
   a float's bit pattern through a (DWORD*) cast, so the top half is
   whatever sits beside the float.  Only the 32 bits the real ABI
   carries are compared. */
#define W32(x) ((long long)(unsigned int)(x))

static HRESULT Vt_SetRenderState(IDirect3DDevice9* d, D3DRENDERSTATETYPE s, DWORD v)
{
    (void)d;
    L2("SetRenderState", W32(s), W32(v));
    return 0;
}

static HRESULT Vt_SetTextureStageState(IDirect3DDevice9* d, DWORD stage,
    D3DTEXTURESTAGESTATETYPE s, DWORD v)
{
    (void)d;
    L3("SetTextureStageState", W32(stage), W32(s), W32(v));
    return 0;
}

static HRESULT Vt_SetTransform(IDirect3DDevice9* d, DWORD which, const D3DMATRIX* m)
{
    const float* f = (const float*)m;
    (void)d;
    WGM_Record("SetTransform", 5, (long long)which, fbits(f[0]), fbits(f[5]),
               fbits(f[10]), fbits(f[15]));
    return 0;
}

static HRESULT Vt_UpdateTexture(IDirect3DDevice9* d, IDirect3DBaseTexture9* a,
    IDirect3DBaseTexture9* b)
{
    (void)d;
    L2("UpdateTexture", WGM_Index(a), WGM_Index(b));
    return 0;
}

static HRESULT Vt_SetTexture(IDirect3DDevice9* d, DWORD stage, IDirect3DBaseTexture9* t)
{
    (void)d;
    L2("SetTexture", stage, WGM_Index(t));
    return 0;
}

static HRESULT Vt_BeginScene(IDirect3DDevice9* d) { (void)d; L0("BeginScene"); return 0; }
static HRESULT Vt_EndScene(IDirect3DDevice9* d) { (void)d; L0("EndScene"); return 0; }

static HRESULT Vt_Present(IDirect3DDevice9* d, const RECT* a, const RECT* b, HWND h,
    const void* p)
{
    (void)d; (void)a; (void)b; (void)h; (void)p;
    L0("Present");
    return 0;
}

static HRESULT Vt_Reset(IDirect3DDevice9* d, D3DPRESENT_PARAMETERS* p)
{
    (void)d; (void)p;
    L0("Reset");
    return 0;
}

static HRESULT Vt_TestCooperativeLevel(IDirect3DDevice9* d) { (void)d; L0("TestCooperativeLevel"); return 0; }
static ULONG Vt_DevRelease(IDirect3DDevice9* d) { (void)d; L0("Device::Release"); return 0; }

static HRESULT Vt_ColorFill(IDirect3DDevice9* d, IDirect3DSurface9* s, const RECT* r,
    D3DCOLOR c)
{
    (void)d; (void)r;
    L2("ColorFill", WGM_Index(s), c);
    return 0;
}

static HRESULT Vt_CreateDepthStencilSurface(IDirect3DDevice9* d, UINT w, UINT h,
    D3DFORMAT f, D3DMULTISAMPLE_TYPE ms, DWORD q, BOOL disc,
    IDirect3DSurface9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    L5("CreateDepthStencilSurface", w, h, f, ms, disc);
    *out = &WGM_surf[(WGM_createCount++) % WGM_SURFACES];
    return 0;
}

static HRESULT Vt_CreateIndexBuffer(IDirect3DDevice9* d, UINT len, DWORD usage,
    D3DFORMAT f, D3DPOOL pool, IDirect3DIndexBuffer9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    L5("CreateIndexBuffer", len, usage, f, pool, WGM_createCount);
    *out = &WGM_ib[(WGM_createCount++) % WGM_IBUFFERS];
    return 0;
}

static HRESULT Vt_CreateVertexBuffer(IDirect3DDevice9* d, UINT len, DWORD usage,
    DWORD fvf, D3DPOOL pool, IDirect3DVertexBuffer9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    L5("CreateVertexBuffer", len, usage, fvf, pool, WGM_createCount);
    *out = &WGM_vb[(WGM_createCount++) % WGM_VBUFFERS];
    return 0;
}

static HRESULT Vt_CreateVertexDeclaration(IDirect3DDevice9* d,
    const D3DVERTEXELEMENT9* e, IDirect3DVertexDeclaration9** out)
{
    int i;
    (void)d;
    /* the declaration ends at the Stream 255 marker; what follows it
       is whatever was on the stack and is not passed to the driver */
    for (i = 0; i < 8; i++)
    {
        L5("VertexElement", e[i].Stream, e[i].Offset, e[i].Type, e[i].Usage,
           e[i].UsageIndex);
        if (e[i].Stream == 255)
            break;
    }
    *out = &WGM_decl[(WGM_createCount++) % WGM_DECLS];
    return 0;
}

static HRESULT Vt_DrawIndexedPrimitive(IDirect3DDevice9* d, DWORD t, int base,
    UINT minv, UINT numv, UINT start, UINT prim)
{
    (void)d;
    WGM_Record("DrawIndexedPrimitive", 6, (long long)t, (long long)base,
               (long long)minv, (long long)numv, (long long)start, (long long)prim);
    return 0;
}

static HRESULT Vt_DrawPrimitive(IDirect3DDevice9* d, DWORD t, UINT start, UINT count)
{
    (void)d;
    L3("DrawPrimitive", t, start, count);
    return 0;
}

static HRESULT Vt_DrawPrimitiveUP(IDirect3DDevice9* d, DWORD t, UINT count,
    const void* data, UINT stride)
{
    unsigned int i, h = 2166136261u;
    const unsigned char* p = (const unsigned char*)data;
    (void)d;
    for (i = 0; i < count * 3u * stride && i < 65536u; i++)
        h = (h ^ p[i]) * 16777619u;
    L4("DrawPrimitiveUP", t, count, stride, h);
    return 0;
}

static HRESULT Vt_GetDepthStencilSurface(IDirect3DDevice9* d, IDirect3DSurface9** out)
{
    (void)d;
    L0("GetDepthStencilSurface");
    *out = &WGM_surf[4];
    return 0;
}

static HRESULT Vt_GetViewport(IDirect3DDevice9* d, D3DVIEWPORT9* vp)
{
    (void)d;
    L0("GetViewport");
    vp->X = 3; vp->Y = 5; vp->Width = 640; vp->Height = 480;
    vp->MinZ = 0.0f; vp->MaxZ = 1.0f;
    return 0;
}

static HRESULT Vt_SetClipPlane(IDirect3DDevice9* d, DWORD i, const float* p)
{
    (void)d;
    L5("SetClipPlane", i, fbits(p[0]), fbits(p[1]), fbits(p[2]), fbits(p[3]));
    return 0;
}

static HRESULT Vt_SetDepthStencilSurface(IDirect3DDevice9* d, IDirect3DSurface9* s)
{
    (void)d;
    L1("SetDepthStencilSurface", WGM_Index(s));
    return 0;
}

static HRESULT Vt_SetDialogBoxMode(IDirect3DDevice9* d, BOOL on)
{
    (void)d;
    L1("SetDialogBoxMode", on);
    return 0;
}

static HRESULT Vt_SetFVF(IDirect3DDevice9* d, DWORD fvf) { (void)d; L1("SetFVF", fvf); return 0; }

static HRESULT Vt_SetIndices(IDirect3DDevice9* d, IDirect3DIndexBuffer9* b)
{
    (void)d;
    L1("SetIndices", WGM_Index(b));
    return 0;
}

static HRESULT Vt_SetRenderTarget(IDirect3DDevice9* d, DWORD i, IDirect3DSurface9* s)
{
    (void)d;
    L2("SetRenderTarget", i, WGM_Index(s));
    return 0;
}

static HRESULT Vt_SetSamplerState(IDirect3DDevice9* d, DWORD s, DWORD t, DWORD v)
{
    (void)d;
    L3("SetSamplerState", W32(s), W32(t), W32(v));
    return 0;
}

static HRESULT Vt_SetStreamSource(IDirect3DDevice9* d, UINT stream,
    IDirect3DVertexBuffer9* b, UINT off, UINT stride)
{
    (void)d;
    L4("SetStreamSource", stream, WGM_Index(b), off, stride);
    return 0;
}

static HRESULT Vt_SetVertexDeclaration(IDirect3DDevice9* d, IDirect3DVertexDeclaration9* v)
{
    (void)d;
    L1("SetVertexDeclaration", WGM_Index(v));
    return 0;
}

static HRESULT Vt_SetVertexShader(IDirect3DDevice9* d, IDirect3DVertexShader9* v)
{
    (void)d;
    L1("SetVertexShader", WGM_Index(v));
    return 0;
}

static HRESULT Vt_SetViewport(IDirect3DDevice9* d, const D3DVIEWPORT9* vp)
{
    (void)d;
    L5("SetViewport", vp->X, vp->Y, vp->Width, vp->Height, fbits(vp->MaxZ));
    return 0;
}

static IDirect3DDevice9Vtbl WGM_devVtbl;

/* ---------- textures, surfaces, buffers ---------- */

static ULONG Vt_TexRelease(IDirect3DTexture9* t) { L1("Texture::Release", WGM_Index(t)); return 0; }

static HRESULT Vt_TexGetSurfaceLevel(IDirect3DTexture9* t, UINT lv, IDirect3DSurface9** out)
{
    L2("GetSurfaceLevel", WGM_Index(t), lv);
    *out = &WGM_surf[(int)(WGM_Index(t) % WGM_SURFACES)];
    return 0;
}

static HRESULT Vt_TexAddDirtyRect(IDirect3DTexture9* t, const RECT* r)
{
    (void)r;
    L1("AddDirtyRect", WGM_Index(t));
    return 0;
}

static DWORD Vt_TexGetLevelCount(IDirect3DTexture9* t)
{
    L1("GetLevelCount", WGM_Index(t));
    return 3;
}

static HRESULT Vt_TexGetLevelDesc(IDirect3DTexture9* t, UINT lv, D3DSURFACE_DESC* d)
{
    L2("GetLevelDesc", WGM_Index(t), lv);
    memset(d, 0, sizeof(*d));
    d->Width = WGM_descWidth;
    d->Height = WGM_descHeight;
    d->Format = WGM_descFormat;
    return 0;
}

static IDirect3DTexture9Vtbl WGM_texVtbl;

static ULONG Vt_SurfRelease(IDirect3DSurface9* s) { L1("Surface::Release", WGM_Index(s)); return 0; }

static HRESULT Vt_SurfGetDesc(IDirect3DSurface9* s, D3DSURFACE_DESC* d)
{
    L1("Surface::GetDesc", WGM_Index(s));
    memset(d, 0, sizeof(*d));
    d->Width = WGM_descWidth;
    d->Height = WGM_descHeight;
    d->Format = WGM_descFormat;
    return 0;
}

static HRESULT Vt_SurfLockRect(IDirect3DSurface9* s, D3DLOCKED_RECT* lr, const RECT* r,
    DWORD flags)
{
    L2("Surface::LockRect", WGM_Index(s), flags);
    lr->Pitch = (int)(WGM_descWidth * 4);
    lr->pBits = WGM_surfMem;
    (void)r;
    return 0;
}

static HRESULT Vt_SurfUnlockRect(IDirect3DSurface9* s)
{
    L1("Surface::UnlockRect", WGM_Index(s));
    return 0;
}

static IDirect3DSurface9Vtbl WGM_surfVtbl;

static void* WGM_LockSlice(unsigned int size)
{
    void* p;

    if (size == 0)
        size = 4096;
    if (WGM_lockUsed + (int)size + 64 > WGM_LOCK_BYTES)
        WGM_lockUsed = 0;
    p = WGM_lockMem + WGM_lockUsed;
    WGM_lockUsed += (int)size + 64;
    WGM_lockUsed = (WGM_lockUsed + 15) & ~15;
    return p;
}

static ULONG Vt_VbRelease(IDirect3DVertexBuffer9* b) { L1("VB::Release", WGM_Index(b)); return 0; }

static HRESULT Vt_VbLock(IDirect3DVertexBuffer9* b, UINT off, UINT size, void** out, DWORD f)
{
    L4("VB::Lock", WGM_Index(b), off, size, f);
    *out = WGM_LockSlice(size ? size : (1 << 16));
    return 0;
}

static HRESULT Vt_VbUnlock(IDirect3DVertexBuffer9* b) { L1("VB::Unlock", WGM_Index(b)); return 0; }

static IDirect3DVertexBuffer9Vtbl WGM_vbVtbl;

static ULONG Vt_IbRelease(IDirect3DIndexBuffer9* b) { L1("IB::Release", WGM_Index(b)); return 0; }

static HRESULT Vt_IbLock(IDirect3DIndexBuffer9* b, UINT off, UINT size, void** out, DWORD f)
{
    L4("IB::Lock", WGM_Index(b), off, size, f);
    *out = WGM_LockSlice(size ? size : (1 << 16));
    return 0;
}

static HRESULT Vt_IbUnlock(IDirect3DIndexBuffer9* b) { L1("IB::Unlock", WGM_Index(b)); return 0; }

static IDirect3DIndexBuffer9Vtbl WGM_ibVtbl;

static ULONG Vt_DeclRelease(IDirect3DVertexDeclaration9* v)
{
    L1("Decl::Release", WGM_Index(v));
    return 0;
}

static IDirect3DVertexDeclaration9Vtbl WGM_declVtbl;

/* ---------- IDirect3D9 ---------- */

static HRESULT Vt_CheckDeviceFormat(IDirect3D9* o, UINT ad, DWORD dt, D3DFORMAT af,
    DWORD usage, D3DRESOURCETYPE rt, D3DFORMAT f)
{
    int bit;
    (void)o; (void)ad; (void)dt; (void)af;
    bit = WGM_formatCall++ & 31;
    L4("CheckDeviceFormat", usage, rt, f, (WGM_formatMask >> bit) & 1);
    return ((WGM_formatMask >> bit) & 1) ? 0 : (HRESULT)-1;
}

static HRESULT Vt_CheckDepthStencilMatch(IDirect3D9* o, UINT ad, DWORD dt,
    D3DFORMAT af, D3DFORMAT rt, D3DFORMAT ds)
{
    int bit;
    (void)o; (void)ad; (void)dt; (void)af;
    bit = WGM_formatCall++ & 31;
    L3("CheckDepthStencilMatch", rt, ds, (WGM_formatMask >> bit) & 1);
    return ((WGM_formatMask >> bit) & 1) ? 0 : (HRESULT)-1;
}

static HRESULT Vt_GetAdapterIdentifier(IDirect3D9* o, UINT ad, DWORD flags,
    D3DADAPTER_IDENTIFIER9* id)
{
    (void)o; (void)ad; (void)flags;
    memset(id, 0, sizeof(*id));
    id->VendorId = WGM_vendorId;
    id->DeviceId = WGM_deviceId;
    id->DriverVersion.HighPart = 0x00010002;
    id->DriverVersion.LowPart = 0x00030004;
    L2("GetAdapterIdentifier", WGM_vendorId, WGM_deviceId);
    return 0;
}

static ULONG Vt_D3DRelease(IDirect3D9* o) { (void)o; L0("D3D::Release"); return 0; }

static HRESULT Vt_GetDeviceCaps(IDirect3D9* o, UINT a, DWORD t, D3DCAPS9* c)
{
    (void)o; (void)a; (void)t;
    memset(c, 0, sizeof(*c));
    L0("GetDeviceCaps");
    return 0;
}

static HRESULT Vt_GetAdapterDisplayMode(IDirect3D9* o, UINT a, D3DDISPLAYMODE* m)
{
    (void)o; (void)a;
    memset(m, 0, sizeof(*m));
    L0("GetAdapterDisplayMode");
    return 0;
}

static HRESULT Vt_EnumAdapterModes(IDirect3D9* o, UINT a, D3DFORMAT f, UINT i,
    D3DDISPLAYMODE* m)
{
    (void)o; (void)a; (void)f; (void)i;
    memset(m, 0, sizeof(*m));
    L0("EnumAdapterModes");
    return 0;
}

static HRESULT Vt_CreateDevice(IDirect3D9* o, UINT a, DWORD t, HWND h, DWORD fl,
    D3DPRESENT_PARAMETERS* p, IDirect3DDevice9** out)
{
    (void)o; (void)a; (void)t; (void)h; (void)p;
    L1("CreateDevice", fl);
    *out = &WGM_dev;
    return 0;
}

static IDirect3D9Vtbl WGM_d3dVtbl;

IDirect3D9* Direct3DCreate9(UINT v)
{
    L1("Direct3DCreate9", v);
    return &WGM_d3d;
}

void WGM_InstallDevice(void)
{
    WGM_devVtbl.CreateOffscreenPlainSurface = Vt_CreateOffscreenPlainSurface;
    WGM_devVtbl.StretchRect = Vt_StretchRect;
    WGM_devVtbl.GetRenderTargetData = Vt_GetRenderTargetData;
    WGM_devVtbl.GetRenderTarget = Vt_GetRenderTarget;
    WGM_devVtbl.GetBackBuffer = Vt_GetBackBuffer;
    WGM_devVtbl.UpdateSurface = Vt_UpdateSurface;
    WGM_devVtbl.Clear = Vt_Clear;
    WGM_devVtbl.CreateTexture = Vt_CreateTexture;
    WGM_devVtbl.SetRenderState = Vt_SetRenderState;
    WGM_devVtbl.SetTextureStageState = Vt_SetTextureStageState;
    WGM_devVtbl.SetTransform = Vt_SetTransform;
    WGM_devVtbl.UpdateTexture = Vt_UpdateTexture;
    WGM_devVtbl.SetTexture = Vt_SetTexture;
    WGM_devVtbl.BeginScene = Vt_BeginScene;
    WGM_devVtbl.EndScene = Vt_EndScene;
    WGM_devVtbl.Present = Vt_Present;
    WGM_devVtbl.Reset = Vt_Reset;
    WGM_devVtbl.TestCooperativeLevel = Vt_TestCooperativeLevel;
    WGM_devVtbl.Release = Vt_DevRelease;
    WGM_devVtbl.ColorFill = Vt_ColorFill;
    WGM_devVtbl.CreateDepthStencilSurface = Vt_CreateDepthStencilSurface;
    WGM_devVtbl.CreateIndexBuffer = Vt_CreateIndexBuffer;
    WGM_devVtbl.CreateVertexBuffer = Vt_CreateVertexBuffer;
    WGM_devVtbl.CreateVertexDeclaration = Vt_CreateVertexDeclaration;
    WGM_devVtbl.DrawIndexedPrimitive = Vt_DrawIndexedPrimitive;
    WGM_devVtbl.DrawPrimitive = Vt_DrawPrimitive;
    WGM_devVtbl.DrawPrimitiveUP = Vt_DrawPrimitiveUP;
    WGM_devVtbl.GetDepthStencilSurface = Vt_GetDepthStencilSurface;
    WGM_devVtbl.GetViewport = Vt_GetViewport;
    WGM_devVtbl.SetClipPlane = Vt_SetClipPlane;
    WGM_devVtbl.SetDepthStencilSurface = Vt_SetDepthStencilSurface;
    WGM_devVtbl.SetDialogBoxMode = Vt_SetDialogBoxMode;
    WGM_devVtbl.SetFVF = Vt_SetFVF;
    WGM_devVtbl.SetIndices = Vt_SetIndices;
    WGM_devVtbl.SetRenderTarget = Vt_SetRenderTarget;
    WGM_devVtbl.SetSamplerState = Vt_SetSamplerState;
    WGM_devVtbl.SetStreamSource = Vt_SetStreamSource;
    WGM_devVtbl.SetVertexDeclaration = Vt_SetVertexDeclaration;
    WGM_devVtbl.SetVertexShader = Vt_SetVertexShader;
    WGM_devVtbl.SetViewport = Vt_SetViewport;
    WGM_dev.lpVtbl = &WGM_devVtbl;

    WGM_texVtbl.Release = Vt_TexRelease;
    WGM_texVtbl.GetSurfaceLevel = Vt_TexGetSurfaceLevel;
    WGM_texVtbl.AddDirtyRect = Vt_TexAddDirtyRect;
    WGM_texVtbl.GetLevelCount = Vt_TexGetLevelCount;
    WGM_texVtbl.GetLevelDesc = Vt_TexGetLevelDesc;

    WGM_surfVtbl.Release = Vt_SurfRelease;
    WGM_surfVtbl.GetDesc = Vt_SurfGetDesc;
    WGM_surfVtbl.LockRect = Vt_SurfLockRect;
    WGM_surfVtbl.UnlockRect = Vt_SurfUnlockRect;

    WGM_vbVtbl.Release = Vt_VbRelease;
    WGM_vbVtbl.Lock = Vt_VbLock;
    WGM_vbVtbl.Unlock = Vt_VbUnlock;

    WGM_ibVtbl.Release = Vt_IbRelease;
    WGM_ibVtbl.Lock = Vt_IbLock;
    WGM_ibVtbl.Unlock = Vt_IbUnlock;

    WGM_declVtbl.Release = Vt_DeclRelease;

    WGM_d3dVtbl.CheckDeviceFormat = Vt_CheckDeviceFormat;
    WGM_d3dVtbl.CheckDepthStencilMatch = Vt_CheckDepthStencilMatch;
    WGM_d3dVtbl.GetAdapterIdentifier = Vt_GetAdapterIdentifier;
    WGM_d3dVtbl.Release = Vt_D3DRelease;
    WGM_d3dVtbl.GetDeviceCaps = Vt_GetDeviceCaps;
    WGM_d3dVtbl.GetAdapterDisplayMode = Vt_GetAdapterDisplayMode;
    WGM_d3dVtbl.EnumAdapterModes = Vt_EnumAdapterModes;
    WGM_d3dVtbl.CreateDevice = Vt_CreateDevice;
    WGM_d3d.lpVtbl = &WGM_d3dVtbl;

    {
        int i;
        for (i = 0; i < WGM_TEXTURES; i++) { WGM_tex[i].lpVtbl = &WGM_texVtbl; WGM_Register(&WGM_tex[i], 1000 + i); }
        for (i = 0; i < WGM_SURFACES; i++) { WGM_surf[i].lpVtbl = &WGM_surfVtbl; WGM_Register(&WGM_surf[i], 2000 + i); }
        for (i = 0; i < WGM_VBUFFERS; i++) { WGM_vb[i].lpVtbl = &WGM_vbVtbl; WGM_Register(&WGM_vb[i], 3000 + i); }
        for (i = 0; i < WGM_IBUFFERS; i++) { WGM_ib[i].lpVtbl = &WGM_ibVtbl; WGM_Register(&WGM_ib[i], 4000 + i); }
        for (i = 0; i < WGM_DECLS; i++) { WGM_decl[i].lpVtbl = &WGM_declVtbl; WGM_Register(&WGM_decl[i], 5000 + i); }
        WGM_Register(&WGM_dev, 9000);
        WGM_Register(&WGM_d3d, 9001);
    }
    WGM_ptrBase = WGM_ptrCount;
}

void WGM_Reset(void)
{
    WGM_allocSeq = 0;
    WGM_heapUsed = 0;
    WGM_ptrCount = WGM_ptrBase;
    WGM_nextId = 0;
    WGM_logLen = 0;
    WGM_overflow = 0;
    WGM_createCount = 0;
    WGM_formatCall = 0;
    WGM_lockUsed = 0;
    WGM_ticks = 0;
    WGM_qpc = 0;
    memset(WGM_lockMem, 0, sizeof(WGM_lockMem));
    /* a pattern, not zeroes: a copy loop that reads the wrong stride
       out of a locked surface produces the same bytes as the right one
       when every byte is zero. */
    {
        unsigned int i;
        for (i = 0; i < sizeof(WGM_surfMem); i++)
            WGM_surfMem[i] = (unsigned char)(i * 37u + (i >> 5) + 1u);
    }
}

unsigned long long WGM_HashState(void)
{
    unsigned long long h = 1469598103934665603ULL;
    unsigned int i;

    for (i = 0; i < sizeof(WGM_lockMem); i++)
    {
        h ^= WGM_lockMem[i];
        h *= 1099511628211ULL;
    }
    for (i = 0; i < sizeof(WGM_surfMem); i++)
    {
        h ^= WGM_surfMem[i];
        h *= 1099511628211ULL;
    }
    return h;
}

/* ---------- D3DX ---------- */

D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* out, const D3DXMATRIX* a, const D3DXMATRIX* b)
{
    D3DXMATRIX r;
    const float* fa = (const float*)a;
    const float* fb = (const float*)b;
    float* fr = (float*)&r;
    int i, j, k;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            float s = 0.0f;
            for (k = 0; k < 4; k++)
                s += fa[i * 4 + k] * fb[k * 4 + j];
            fr[i * 4 + j] = s;
        }
    *out = r;
    L0("D3DXMatrixMultiply");
    return out;
}

D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX* out, float x, float y, float z)
{
    float* f = (float*)out;
    int i;
    for (i = 0; i < 16; i++)
        f[i] = (i % 5) == 0 ? 1.0f : 0.0f;
    f[12] = x; f[13] = y; f[14] = z;
    L3("D3DXMatrixTranslation", fbits(x), fbits(y), fbits(z));
    return out;
}

D3DXMATRIX* D3DXMatrixOrthoOffCenterRH(D3DXMATRIX* out, float l, float r, float b, float t,
    float zn, float zf)
{
    float* f = (float*)out;
    int i;
    for (i = 0; i < 16; i++)
        f[i] = 0.0f;
    f[0] = 2.0f / (r - l);
    f[5] = 2.0f / (t - b);
    f[10] = 1.0f / (zn - zf);
    f[12] = (l + r) / (l - r);
    f[13] = (t + b) / (b - t);
    f[14] = zn / (zn - zf);
    f[15] = 1.0f;
    WGM_Record("D3DXMatrixOrthoOffCenterRH", 6, fbits(l), fbits(r), fbits(b),
               fbits(t), fbits(zn), fbits(zf));
    return out;
}

D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX* out, float* det, const D3DXMATRIX* in)
{
    /* enough of an inverse for the test: transpose the 3x3 and negate
       the translation, which is what the renderer's matrices are */
    const float* f = (const float*)in;
    float* o = (float*)out;
    int i, j;

    (void)det;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            o[i * 4 + j] = f[j * 4 + i];
    o[3] = o[7] = o[11] = 0.0f;
    o[12] = -f[12]; o[13] = -f[13]; o[14] = -f[14];
    o[15] = 1.0f;
    L0("D3DXMatrixInverse");
    return out;
}

HRESULT D3DXLoadSurfaceFromMemory(IDirect3DSurface9* s, const PALETTEENTRY* pal, const RECT* dr,
    const void* src, D3DFORMAT f, UINT pitch, const PALETTEENTRY* spal, const RECT* sr,
    DWORD filter, D3DCOLOR key)
{
    (void)pal; (void)dr; (void)src; (void)spal; (void)sr; (void)key;
    L4("D3DXLoadSurfaceFromMemory", WGM_Index(s), f, pitch, filter);
    return 0;
}

HRESULT D3DXFilterTexture(IDirect3DBaseTexture9* t, const PALETTEENTRY* pal, UINT lv, DWORD f)
{
    (void)pal;
    L3("D3DXFilterTexture", WGM_Index(t), lv, f);
    return 0;
}

static int Vt_FontDrawTextA(ID3DXFont* f, void* spr, LPCSTR text, int count,
    RECT* r, DWORD fmt, D3DCOLOR col)
{
    unsigned int h = 2166136261u;
    const unsigned char* p = (const unsigned char*)text;
    (void)f; (void)spr; (void)count; (void)fmt;
    while (p && *p)
    {
        h = (h ^ *p) * 16777619u;
        p++;
    }
    L5("Font::DrawTextA", h, r->left, r->top, col, r->right);
    return 0;
}

static ULONG Vt_FontRelease(ID3DXFont* f) { (void)f; L0("Font::Release"); return 0; }

HRESULT D3DXCreateFontIndirectA(IDirect3DDevice9* d, const D3DXFONT_DESCA* desc,
    LPD3DXFONT* out)
{
    static ID3DXFont font;
    static ID3DXFontVtbl vtbl;
    (void)d;
    L2("D3DXCreateFontIndirectA", desc->Height, desc->Weight);
    vtbl.DrawTextA = Vt_FontDrawTextA;
    vtbl.Release = Vt_FontRelease;
    font.lpVtbl = &vtbl;
    *out = &font;
    WGM_Register(&font, 8000);
    return 0;
}

/* ---------- engine mocks ---------- */

void D3D_OutputDebugString(char* fmt, ...) { (void)fmt; L0("D3D_OutputDebugString"); }

void D3D_AETexturesInit(void) { L0("D3D_AETexturesInit"); }
void D3D_CreateBlurTextures(void) { L0("D3D_CreateBlurTextures"); }
void D3D_DoBlurWaterTexture(void) { L0("D3D_DoBlurWaterTexture"); }

void GEO_UseNormals(tGEOGRO* g) { L1("GEO_UseNormals", WGM_Index(g)); }

BOOL OBJ_BV_IsSphere(tGameObjectBV* bv) { return (BOOL)(WGM_Index(bv) & 1); }

void WTR_WaterBlurNormal(Vec3* v, int a, int b) { L3("WTR_WaterBlurNormal", WGM_Index(v), a, b); }

float GDI_gaf_ScreenRatio[4] = { 0.75f, 0.625f, 0.5625f, 0.8f };

/* MAT_FuncTable: sixteen UV generators.  Each records that it ran and
   touches the display data the way the real ones do. */
static void WGM_MatFunc(tGdi_DisplayData* dd, tGEOGRO* gro, tGeoTriangleChunks* ch, unsigned int p);
void (*MAT_FuncTable[16])(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int);
static int WGM_matFuncIdx;

static void WGM_MatFunc(tGdi_DisplayData* dd, tGEOGRO* gro, tGeoTriangleChunks* ch, unsigned int p)
{
    int i;

    for (i = 0; i < 16; i++)
        if (MAT_FuncTable[i] == WGM_MatFunc)
            break;
    (void)gro; (void)ch;
    L2("MAT_FuncTable", WGM_matFuncIdx, p);
    if (dd && dd->bigBuffer && (WGM_matFuncIdx & 1))
        dd->bigBuffer->UVData = dd->bigBuffer->data7;
    if (WGM_matFuncIdx & 2)
        D3DRenderParams.textureUVMode = 3;
}

void WGM_SetMatFunc(int idx)
{
    int i;
    WGM_matFuncIdx = idx;
    for (i = 0; i < 16; i++)
        MAT_FuncTable[i] = WGM_MatFunc;
}

void MAT_UVDecompress(tMultiTextureElemStruct* e, tMatDecompressedStruct* out)
{
    unsigned int k = (unsigned int)(e->transform1 ^ (e->transform2 * 2654435761u));

    out->f1 = (float)((k >> 3) % 17) * 0.25f;
    out->f2 = (float)((k >> 7) % 13) * 0.5f - 3.0f;
    out->f3 = (float)((k >> 11) % 11) * 0.125f;
    out->f4 = (float)((k >> 15) % 7) - 3.0f;
    out->speedU = (float)((k >> 19) % 5) * 0.75f;
    out->speedV = (float)((k >> 23) % 9) * 0.0625f;
    L1("MAT_UVDecompress", k);
}

/* globals the file reads */
int ENG_WindowedMode;
int CIN_Bands;
int CIN_NoBands;
float tweak_mipmaplodbias;
int tweak_perf;
float tweak_perf_fps_min;
float tweak_perf_fps_target;
int tweak_water_blur_normal;
int tweak_water_clip;
float tweak_water_min_bv;
float tweak_water_off;
int tweak_water_smooth;
int PRO_rasterDetailPage;
int PRO_rasterWindowIsOn;
HWND PRO_rasterHWND;
profilerRasterStructElem* PRO_CurrRaster;
profilerRasterStructElem* PRO_FirstRasterPtr;
profilerRasterStructElem* pointsToUnkRaster_dword_98F944;
int AI_CompiledAIEnabled;

void AI_ReinitUniverse(void) { L0("AI_ReinitUniverse"); }
void SOFT_ZList_Clear(void) { L0("SOFT_ZList_Clear"); }
void ENG_ReinitOneWorld(tWorld* w, int f) { L2("ENG_ReinitOneWorld", WGM_Index(w), f); }
void ENG_GetStatsText(char* s) { strcpy(s, "stats"); }
BOOL TAB_TableElemIsInvalid(void* p) { return (BOOL)(((unsigned long)(size_t)p & 3) == 1); }
void pcINO_dinput_mouse_and_keyb_acquire(void) { L0("acquire"); }
void pcINO_dinput_mouse_and_keyb_unacquire(void) { L0("unacquire"); }
void GDI_DetachDisplay(struct sGdi_DisplayData* d) { L1("GDI_DetachDisplay", WGM_Index(d)); }
void GDI_DestroyDisplayData(struct sGdi_DisplayData* d) { L1("GDI_DestroyDisplayData", WGM_Index(d)); }
void GDI_fnl_InitInterface(struct sGdi_DisplayData* d) { L1("GDI_fnl_InitInterface", WGM_Index(d)); }
BOOL GDI_AttachDisplayAndReadaptDisplay(struct sGdi_DisplayData* d, HWND h) { (void)h; L1("GDI_Attach", WGM_Index(d)); return 1; }
static tGdi_DisplayData WGM_createdDisplay;
static profilerRasterStructElem WGM_createdRasters[64];
static struct sGdi_D3DRenderParams* WGM_DisplayCreateDevice(void)
{
    L0("DisplayData::CreateDevice");
    return NULL;
}
static void WGM_DisplayDestroyDevice(struct sGdi_D3DRenderParams* p)
{
    L1("DisplayData::DestroyDevice", WGM_Index(p));
}
tGdi_DisplayData* GDI_fnpst_CreateDisplayData(void)
{
    L0("GDI_fnpst_CreateDisplayData");
    memset(&WGM_createdDisplay, 0, sizeof(WGM_createdDisplay));
    WGM_createdDisplay.CreateDevice = WGM_DisplayCreateDevice;
    WGM_createdDisplay.DestroyDevice = WGM_DisplayDestroyDevice;
    memset(WGM_createdRasters, 0, sizeof(WGM_createdRasters));
    WGM_createdDisplay.rasters = WGM_createdRasters;
    return &WGM_createdDisplay;
}

/* ---------- Win32 ---------- */

int WGM_asyncKey;

short GetAsyncKeyState(int k) { (void)k; return (short)WGM_asyncKey; }
BOOL PeekMessageA(MSG* m, HWND h, UINT a, UINT b, UINT c)
{ (void)m; (void)h; (void)a; (void)b; (void)c; return 0; }
BOOL TranslateMessage(const MSG* m) { (void)m; return 0; }
LRESULT DispatchMessageA(const MSG* m) { (void)m; return 0; }
LRESULT DefWindowProcA(HWND h, UINT m, WPARAM w, LPARAM l)
{ (void)h; (void)m; (void)w; L1("DefWindowProcA", l); return l; }
HCURSOR SetCursor(HCURSOR c) { (void)c; L0("SetCursor"); return NULL; }
BOOL SetWindowTextA(HWND h, LPCSTR s) { (void)h; (void)s; L0("SetWindowTextA"); return 1; }
UINT RegisterWindowMessageA(LPCSTR s) { (void)s; return 0xC123; }
BOOL InvalidateRect(HWND h, const RECT* r, BOOL e) { (void)h; (void)r; (void)e; return 1; }
LONG RegOpenKeyExA(HKEY k, LPCSTR s, DWORD o, DWORD a, HKEY* out)
{ (void)k; (void)s; (void)o; (void)a; *out = (HKEY)1; return 1; }
LONG RegQueryValueExA(HKEY k, LPCSTR s, DWORD* r, DWORD* t, LPBYTE d, DWORD* c)
{ (void)k; (void)s; (void)r; (void)t; (void)d; (void)c; return 1; }
LONG RegCloseKey(HKEY k) { (void)k; return 0; }
HMODULE LoadLibraryA(LPCSTR s) { (void)s; return NULL; }
FARPROC GetProcAddress(HMODULE m, LPCSTR s) { (void)m; (void)s; return NULL; }
int MessageBoxA(HWND h, LPCSTR a, LPCSTR b, UINT u)
{ (void)h; (void)a; (void)b; (void)u; return 0; }
void OutputDebugStringA(LPCSTR s) { (void)s; }
BOOL CreateProcessA(LPCSTR a, LPSTR b, void* c, void* d, BOOL e, DWORD f, void* g,
    LPCSTR h, STARTUPINFOA* i, PROCESS_INFORMATION* j)
{ (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h; (void)i; (void)j; return 0; }
BOOL DestroyWindow(HWND h) { (void)h; return 1; }
BOOL GetClientRect(HWND h, RECT* r) { (void)h; r->left = 0; r->top = 0; r->right = 800; r->bottom = 600; return 1; }
void GetLocalTime(SYSTEMTIME* t) { memset(t, 0, sizeof(*t)); }
BOOL CreateDirectoryA(LPCSTR p, void* a) { (void)p; (void)a; return 1; }
WORD RegisterClassA(const WNDCLASSA* c) { (void)c; return 1; }
HWND CreateWindowExA(DWORD a, LPCSTR b, LPCSTR c, DWORD d, int e, int f, int g, int h,
    HWND i, HMENU j, HINSTANCE k, void* l)
{ (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h; (void)i;
  (void)j; (void)k; (void)l; return (HWND)0x1234; }
BOOL ShowWindow(HWND h, int n) { (void)h; (void)n; return 1; }
HICON LoadIconA(HINSTANCE i, LPCSTR n) { (void)i; (void)n; return NULL; }
BOOL AdjustWindowRect(RECT* r, DWORD s, BOOL m) { (void)s; (void)m; r->right += 8; r->bottom += 28; return 1; }
DWORD MsgWaitForMultipleObjectsEx(DWORD a, const HANDLE* b, DWORD c, DWORD d, DWORD e)
{ (void)a; (void)b; (void)c; (void)d; (void)e; return 1; }

/* profiler.c's Win32 */
HDC GetDC(HWND h) { (void)h; return NULL; }
int ReleaseDC(HWND h, HDC d) { (void)h; (void)d; return 1; }
HBRUSH CreateSolidBrush(COLORREF c) { (void)c; return NULL; }
BOOL ExtTextOutA(HDC a, int b, int c, UINT d, const RECT* e, LPCSTR f, UINT g, const int* h)
{ (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h; return 1; }
int DrawTextA(HDC a, LPCSTR b, int c, RECT* d, UINT e)
{ (void)a; (void)b; (void)c; (void)d; (void)e; return 0; }
COLORREF SetBkColor(HDC a, COLORREF b) { (void)a; (void)b; return 0; }
COLORREF SetTextColor(HDC a, COLORREF b) { (void)a; (void)b; return 0; }
DWORD GetSysColor(int i) { (void)i; return 0; }
HMODULE GetModuleHandleA(LPCSTR s) { (void)s; return NULL; }

int _stricmp(const char* a, const char* b)
{
    while (*a && *b)
    {
        int ca = (*a >= 65 && *a <= 90) ? *a + 32 : *a;
        int cb = (*b >= 65 && *b <= 90) ? *b + 32 : *b;
        if (ca != cb)
            return ca - cb;
        a++; b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}
