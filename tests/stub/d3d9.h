#pragma once
#include <windows.h>
typedef struct IDirect3D9              IDirect3D9;
typedef struct IDirect3DDevice9        IDirect3DDevice9;
typedef struct IDirect3DTexture9       IDirect3DTexture9;
typedef struct IDirect3DSurface9       IDirect3DSurface9;
typedef struct IDirect3DVertexBuffer9  IDirect3DVertexBuffer9;
typedef struct IDirect3DIndexBuffer9   IDirect3DIndexBuffer9;
typedef struct IDirect3DVertexShader9  IDirect3DVertexShader9;
typedef struct IDirect3DVertexDeclaration9 IDirect3DVertexDeclaration9;
typedef struct IDirect3DSwapChain9     IDirect3DSwapChain9;
typedef DWORD D3DFORMAT;
typedef DWORD D3DPOOL;
typedef DWORD D3DTEXTUREFILTERTYPE;
typedef DWORD D3DBACKBUFFER_TYPE;
typedef DWORD D3DCOLOR;
typedef DWORD D3DRESOURCETYPE;
typedef DWORD D3DMULTISAMPLE_TYPE;
typedef struct _D3DMATRIX { float m[4][4]; } D3DMATRIX;
typedef struct _D3DVIEWPORT9 { DWORD X, Y, Width, Height; float MinZ, MaxZ; } D3DVIEWPORT9;
typedef struct _D3DSURFACE_DESC {
    D3DFORMAT Format; D3DRESOURCETYPE Type; DWORD Usage; D3DPOOL Pool;
    D3DMULTISAMPLE_TYPE MultiSampleType; DWORD MultiSampleQuality;
    UINT Width, Height;
} D3DSURFACE_DESC;
typedef struct _D3DLOCKED_RECT { int Pitch; void* pBits; } D3DLOCKED_RECT;
typedef struct _D3DPRESENT_PARAMETERS_ {
    UINT BackBufferWidth, BackBufferHeight; D3DFORMAT BackBufferFormat;
    UINT BackBufferCount; D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD MultiSampleQuality, SwapEffect; HWND hDeviceWindow;
    BOOL Windowed, EnableAutoDepthStencil; D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags, FullScreen_RefreshRateInHz, PresentationInterval;
} D3DPRESENT_PARAMETERS;
typedef struct _D3DCAPS9 { DWORD DeviceType; DWORD pad[256]; } D3DCAPS9;
#define D3DPOOL_DEFAULT        0
#define D3DPOOL_SYSTEMMEM      2
#define D3DUSAGE_RENDERTARGET  1
#define D3DCLEAR_TARGET        1
#define D3DLOCK_NOSYSLOCK      0x800
#define D3DBACKBUFFER_TYPE_MONO 0
#define D3DTEXF_NONE   0
#define D3DTEXF_POINT  1
#define D3DTEXF_LINEAR 2
typedef IDirect3D9*              LPDIRECT3D9;
typedef IDirect3DDevice9*        LPDIRECT3DDEVICE9;
typedef IDirect3DTexture9*       LPDIRECT3DTEXTURE9;
typedef IDirect3DSurface9*       LPDIRECT3DSURFACE9;
typedef IDirect3DVertexBuffer9*  LPDIRECT3DVERTEXBUFFER9;
typedef IDirect3DIndexBuffer9*   LPDIRECT3DINDEXBUFFER9;
