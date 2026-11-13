/* Mocks for the materialanddraw.c differential test.
 *
 * The real math.c is linked rather than mocked, so the inverse table the
 * two z-sort walks index is the engine's own; this file supplies the
 * three allocator entry points it wants. Everything else here is either
 * a deterministic clock, a logged display-device entry point, or a
 * stand-in that records what it was asked for.
 *
 * profiler.c is linked whole, so the start/end blocks the rewrite calls
 * are the real ones and the frozen copy's inlined arithmetic is measured
 * against them. The GDI entry points below exist only to let it link;
 * nothing in this test reaches the raster window. */

#include "matdraw_mocks.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

tMdMockCall MDMOCK_log[MDMOCK_LOG_MAX];
int MDMOCK_logLen;
int MDMOCK_overflow;

tZListElem MDMOCK_nodes[MDMOCK_NODES];
tZListMeshElem MDMOCK_mesh[MDMOCK_NODES][MDMOCK_NODE_MESH];
int MDMOCK_nodeCount;
int MDMOCK_nodeFailEvery;

unsigned int MDMOCK_ticks;
long long MDMOCK_qpc;

tGEOMAT* MDMOCK_geoMaterials[8];

float MAT_C_f_8Bits = 49152.0f;
tGEOMAT MAT_gst_DefaultSingleMaterial;

/* ---- pointer identity, so a logged pointer compares across the two sides ---- */

#define MDMOCK_PTRS 256
static const void* ptrTable[MDMOCK_PTRS];
static int ptrCount;

void MDMOCK_RegisterPtr(const void* p)
{
    if (ptrCount < MDMOCK_PTRS)
        ptrTable[ptrCount++] = p;
}

int MDMOCK_Index(const void* p)
{
    int i;

    if (!p)
        return -1;
    for (i = 0; i < ptrCount; i++)
        if (ptrTable[i] == p)
            return i;
    return -2;
}

void MDMOCK_Reset(void)
{
    memset(MDMOCK_log, 0, sizeof(MDMOCK_log));
    MDMOCK_logLen = 0;
    MDMOCK_overflow = 0;
    memset(MDMOCK_nodes, 0, sizeof(MDMOCK_nodes));
    memset(MDMOCK_mesh, 0, sizeof(MDMOCK_mesh));
    MDMOCK_nodeCount = 0;
    MDMOCK_nodeFailEvery = 0;
    MDMOCK_ticks = 500;
    MDMOCK_qpc = 90000;
}

void MDMOCK_Record(const char* fn, int nargs, ...)
{
    tMdMockCall* c;
    va_list ap;
    int i;

    if (MDMOCK_logLen >= MDMOCK_LOG_MAX)
    {
        MDMOCK_overflow = 1;
        return;
    }
    c = &MDMOCK_log[MDMOCK_logLen++];
    c->fn = fn;
    c->nargs = nargs > MDMOCK_ARGS ? MDMOCK_ARGS : nargs;
    va_start(ap, nargs);
    for (i = 0; i < c->nargs; i++)
        c->arg[i] = va_arg(ap, long long);
    va_end(ap);
}

/* ---- clocks ---- */

unsigned int TIM_GetTicksAsInt32(void)
{
    unsigned int t = MDMOCK_ticks;
    MDMOCK_ticks += 3;
    return t;
}

BOOL QueryPerformanceCounter(LARGE_INTEGER* li)
{
    li->QuadPart = MDMOCK_qpc;
    MDMOCK_qpc += 17;
    return 1;
}

/* ---- the allocator math.c wants ---- */

void* MEM_p_AllocAligned(int size, int align)
{
    return calloc(1, size + align + 16);
}

void* MEM_p_ReallocAligned(void* p, int size, int align)
{
    return realloc(p, size + align + 16);
}

void MEM_FreeAlign(void* p)
{
    free(p);
}

/* ---- engine stand-ins ---- */

struct sGEOMAT* GEO_GetMaterial(struct sGEOMAT* mat, int idx)
{
    MDMOCK_Record("GEO_GetMaterial", 2, (long long)MDMOCK_Index(mat), (long long)idx);
    if (idx < 0 || idx >= 8)
        return NULL;
    return MDMOCK_geoMaterials[idx];
}

void MAT_ComputeUV_sub_4521A0(tGEOGRO* geogro, tGeoTriangleChunks* chunks,
                              tMultiTextureElemStruct* texElem)
{
    MDMOCK_Record("MAT_ComputeUV", 3, (long long)MDMOCK_Index(geogro),
                  (long long)MDMOCK_Index(chunks), (long long)MDMOCK_Index(texElem));
}

void SOFT_TransformInComputingBuffer(tBigBufferStruct* bb, Vec3* verts, int num,
                                     tDDContainerStruct* ct)
{
    Vec3* out = (Vec3*)bb->data6;
    int i;

    MDMOCK_Record("SOFT_Transform", 2, (long long)MDMOCK_Index(verts), (long long)num);
    /* a deterministic projection: enough spread in z for the sort to
       have something to pick, and negatives so the > 0 guard fires */
    for (i = 0; i < num; i++)
    {
        out[i].x = verts[i].x * 2.0f + 1.0f;
        out[i].y = verts[i].y * 2.0f - 1.0f;
        out[i].z = verts[i].z * 3.0f - 3.0f;   /* z == 0 is reachable */
    }
}

tZListElem* SOFT_pst_ZList_AddNode(float z, int num, int kind)
{
    tZListElem* node;
    int n = MDMOCK_nodeCount++;

    MDMOCK_Record("ZList_AddNode", 3, (long long)*(uint32_t*)&z, (long long)num,
                  (long long)kind);
    if (MDMOCK_nodeFailEvery && (n % MDMOCK_nodeFailEvery) == 0)
        return NULL;
    if (n >= MDMOCK_NODES)
        return NULL;
    node = &MDMOCK_nodes[n];
    node->data = MDMOCK_mesh[n];
    node->currZElemGroup = NULL;
    node->geomat = NULL;
    node->renderingFlags = 0;
    return node;
}

/* ---- display device ---- */

void MDMOCK_TextureInit(int texIdx, int flags, char param)
{
    MDMOCK_Record("TextureInit", 3, (long long)texIdx, (long long)(unsigned int)flags,
                  (long long)param);
}

/* the colour state the device would read at the moment of the draw: the
   per-element arms rewrite it before every call, so only the call log
   sees them -- the end-of-case snapshot carries the last one only */
static long long MDMOCK_ColourState(void)
{
    tBigBufferStruct* bb = gpst_DX_stDD->bigBuffer;
    long long sel = 0;

    if (bb->vertexColorArray == (int*)bb->data3FinalColorWithShadow)
        sel = 1;
    else if (bb->vertexColorArray == (int*)bb->data2SpecularColors)
        sel = 2;
    else if (bb->vertexColorArray == gpst_DX_stDD->RLIPtr)
        sel = 3;
    else if (bb->vertexColorArray)
        sel = 4;
    return sel;
}

int MDMOCK_DrawElementIndexedTriangles(struct sGeoTriangleChunks* chunks, Vec3* verts,
                                       struct sGeoUVElem* uvs, int vertexNum)
{
    tBigBufferStruct* bb = gpst_DX_stDD->bigBuffer;

    MDMOCK_Record("DrawTriangles", 8, (long long)MDMOCK_Index(chunks),
                  (long long)MDMOCK_Index(verts), (long long)MDMOCK_Index(uvs),
                  (long long)vertexNum, (long long)(unsigned int)bb->solidVertexColor,
                  (long long)(unsigned int)bb->colorMask, MDMOCK_ColourState(),
                  (long long)(bb->vertexColorArraySecond != NULL));
    return 1;
}

int MDMOCK_DrawSprites(tGeoSprites* spr, Vec3* verts, int num)
{
    tBigBufferStruct* bb = gpst_DX_stDD->bigBuffer;

    MDMOCK_Record("DrawSprites", 7, (long long)MDMOCK_Index(spr),
                  (long long)MDMOCK_Index(verts), (long long)num,
                  (long long)(unsigned int)bb->solidVertexColor,
                  (long long)(unsigned int)bb->colorMask, MDMOCK_ColourState(),
                  (long long)(bb->vertexColorArraySecond != NULL));
    return 1;
}

/* ---- enough of the GDI for profiler.c to link ---- */

BOOL GetClientRect(HWND h, RECT* r) { return 0; }
DWORD GetSysColor(int i) { return 0; }
COLORREF SetBkColor(HDC d, COLORREF c) { return 0; }
COLORREF SetTextColor(HDC d, COLORREF c) { return 0; }
HBRUSH CreateSolidBrush(COLORREF c) { return 0; }
BOOL ExtTextOutA(HDC d, int x, int y, UINT o, const RECT* r, LPCSTR s, UINT n, const int* dx) { return 0; }
int DrawTextA(HDC d, LPCSTR s, int n, RECT* r, UINT f) { return 0; }
HDC GetDC(HWND h) { return 0; }
int ReleaseDC(HWND h, HDC d) { return 0; }
BOOL ShowWindow(HWND h, int c) { return 0; }
WORD RegisterClassA(const WNDCLASSA* w) { return 0; }
HWND CreateWindowExA(DWORD a, LPCSTR b, LPCSTR c, DWORD d, int e, int f, int g, int h,
                     HWND i, HMENU j, HINSTANCE k, void* l) { return 0; }
HMODULE GetModuleHandleA(LPCSTR n) { return 0; }
LONG DefWindowProcA(HWND h, UINT m, WPARAM w, LPARAM l) { return 0; }
int _stricmp(const char* a, const char* b) { return strcmp(a, b); }
