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
typedef struct IDirect3DBaseTexture9   IDirect3DBaseTexture9;
typedef struct _D3DVERTEXELEMENT9 {
    WORD Stream, Offset; BYTE Type, Method, Usage, UsageIndex;
} D3DVERTEXELEMENT9;
typedef DWORD D3DFORMAT;
typedef DWORD D3DPOOL;
typedef DWORD D3DTEXTUREFILTERTYPE;
typedef DWORD D3DBACKBUFFER_TYPE;
typedef DWORD D3DCOLOR;
typedef DWORD D3DRENDERSTATETYPE;
typedef DWORD D3DTEXTURESTAGESTATETYPE;

typedef struct _D3DRECT { LONG x1, y1, x2, y2; } D3DRECT;
typedef DWORD D3DRESOURCETYPE;
typedef DWORD D3DMULTISAMPLE_TYPE;
typedef struct _D3DMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;
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
typedef struct _D3DCAPS9 {
    DWORD DeviceType; DWORD Caps2; DWORD RasterCaps; DWORD TextureOpCaps;
    DWORD MaxTextureBlendStages; DWORD MaxSimultaneousTextures;
    DWORD pad[256];
} D3DCAPS9;
#define D3DPOOL_DEFAULT        0
#define D3DPOOL_SYSTEMMEM      2
#define D3DUSAGE_RENDERTARGET              0x00000800
#define D3DCLEAR_TARGET        1
#define D3DLOCK_NOSYSLOCK      0x800
#define D3DBACKBUFFER_TYPE_MONO            0x2005
#define D3DTSS_TEXCOORDINDEX               0x2001
#define D3DTSS_TEXTURETRANSFORMFLAGS       0x2002
#define D3DRS_ZENABLE                      0x2003
#define D3DRS_ZWRITEENABLE                 0x2004
#define D3DTEXF_NONE                       0x2006
#define D3DTEXF_POINT                      0x2007
#define D3DTEXF_LINEAR                     0x2008
#define D3DERR_WASSTILLDRAWING ((HRESULT)0x8876021CL)
/* Just enough of the COM vtables for the calls the engine actually makes.
   Grown per directory as a pass needs more; see docs/cleanup/README.md. */
typedef struct IDirect3DDevice9Vtbl  IDirect3DDevice9Vtbl;
typedef struct IDirect3DSurface9Vtbl IDirect3DSurface9Vtbl;
typedef struct IDirect3DTexture9Vtbl IDirect3DTexture9Vtbl;

typedef struct IDirect3D9Vtbl IDirect3D9Vtbl;
struct IDirect3D9 { IDirect3D9Vtbl* lpVtbl; };
typedef struct _D3DDISPLAYMODE { UINT Width, Height, RefreshRate; D3DFORMAT Format; } D3DDISPLAYMODE;
typedef struct _D3DADAPTER_IDENTIFIER9 {
    char Driver[512], Description[512], DeviceName[32];
    union { struct { DWORD LowPart; long HighPart; }; long long QuadPart; } DriverVersion;
    DWORD VendorId, DeviceId, SubSysId, Revision;
    GUID DeviceIdentifier; DWORD WHQLLevel;
} D3DADAPTER_IDENTIFIER9;
struct IDirect3D9Vtbl
{
    ULONG   (*Release)(IDirect3D9*);
    HRESULT (*CreateDevice)(IDirect3D9*, UINT, DWORD, HWND, DWORD,
                            D3DPRESENT_PARAMETERS*, IDirect3DDevice9**);
    HRESULT (*GetDeviceCaps)(IDirect3D9*, UINT, DWORD, D3DCAPS9*);
    HRESULT (*GetAdapterDisplayMode)(IDirect3D9*, UINT, D3DDISPLAYMODE*);
    HRESULT (*GetAdapterIdentifier)(IDirect3D9*, UINT, DWORD, D3DADAPTER_IDENTIFIER9*);
    UINT    (*GetAdapterModeCount)(IDirect3D9*, UINT, D3DFORMAT);
    HRESULT (*EnumAdapterModes)(IDirect3D9*, UINT, D3DFORMAT, UINT, D3DDISPLAYMODE*);
    HRESULT (*CheckDeviceFormat)(IDirect3D9*, UINT, DWORD, D3DFORMAT, DWORD,
                                 D3DRESOURCETYPE, D3DFORMAT);
    HRESULT (*CheckDepthStencilMatch)(IDirect3D9*, UINT, DWORD, D3DFORMAT,
                                      D3DFORMAT, D3DFORMAT);
};

struct IDirect3DDevice9  { IDirect3DDevice9Vtbl*  lpVtbl; };
struct IDirect3DSurface9 { IDirect3DSurface9Vtbl* lpVtbl; };
struct IDirect3DTexture9 { IDirect3DTexture9Vtbl* lpVtbl; };

struct IDirect3DDevice9Vtbl
{
    HRESULT (*CreateOffscreenPlainSurface)(IDirect3DDevice9*, UINT, UINT, D3DFORMAT,
                                           D3DPOOL, IDirect3DSurface9**, HANDLE*);
    HRESULT (*StretchRect)(IDirect3DDevice9*, IDirect3DSurface9*, const RECT*,
                           IDirect3DSurface9*, const RECT*, D3DTEXTUREFILTERTYPE);
    HRESULT (*GetRenderTargetData)(IDirect3DDevice9*, IDirect3DSurface9*,
                                   IDirect3DSurface9*);
    HRESULT (*GetRenderTarget)(IDirect3DDevice9*, DWORD, IDirect3DSurface9**);
    HRESULT (*GetBackBuffer)(IDirect3DDevice9*, UINT, UINT, D3DBACKBUFFER_TYPE,
                             IDirect3DSurface9**);
    HRESULT (*UpdateSurface)(IDirect3DDevice9*, IDirect3DSurface9*, const RECT*,
                             IDirect3DSurface9*, const POINT*);
    HRESULT (*Clear)(IDirect3DDevice9*, DWORD, const D3DRECT*, DWORD, D3DCOLOR,
                     float, DWORD);
    HRESULT (*CreateTexture)(IDirect3DDevice9*, UINT, UINT, UINT, DWORD, D3DFORMAT,
                             D3DPOOL, IDirect3DTexture9**, HANDLE*);
    HRESULT (*SetRenderState)(IDirect3DDevice9*, D3DRENDERSTATETYPE, DWORD);
    HRESULT (*SetTextureStageState)(IDirect3DDevice9*, DWORD, D3DTEXTURESTAGESTATETYPE,
                                    DWORD);
    HRESULT (*SetTransform)(IDirect3DDevice9*, DWORD, const D3DMATRIX*);
    HRESULT (*UpdateTexture)(IDirect3DDevice9*, IDirect3DBaseTexture9*,
                             IDirect3DBaseTexture9*);
    HRESULT (*SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
    HRESULT (*BeginScene)(IDirect3DDevice9*);
    HRESULT (*EndScene)(IDirect3DDevice9*);
    HRESULT (*Present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const void*);
    HRESULT (*Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    HRESULT (*TestCooperativeLevel)(IDirect3DDevice9*);
    ULONG   (*Release)(IDirect3DDevice9*);
    HRESULT (*ColorFill)(IDirect3DDevice9*, IDirect3DSurface9*, const RECT*, D3DCOLOR);
    HRESULT (*CreateDepthStencilSurface)(IDirect3DDevice9*, UINT, UINT, D3DFORMAT,
                                         D3DMULTISAMPLE_TYPE, DWORD, BOOL,
                                         IDirect3DSurface9**, HANDLE*);
    HRESULT (*CreateIndexBuffer)(IDirect3DDevice9*, UINT, DWORD, D3DFORMAT, D3DPOOL,
                                 IDirect3DIndexBuffer9**, HANDLE*);
    HRESULT (*CreateVertexBuffer)(IDirect3DDevice9*, UINT, DWORD, DWORD, D3DPOOL,
                                  IDirect3DVertexBuffer9**, HANDLE*);
    HRESULT (*CreateVertexDeclaration)(IDirect3DDevice9*, const D3DVERTEXELEMENT9*,
                                       IDirect3DVertexDeclaration9**);
    HRESULT (*DrawIndexedPrimitive)(IDirect3DDevice9*, DWORD, int, UINT, UINT, UINT, UINT);
    HRESULT (*DrawPrimitive)(IDirect3DDevice9*, DWORD, UINT, UINT);
    HRESULT (*DrawPrimitiveUP)(IDirect3DDevice9*, DWORD, UINT, const void*, UINT);
    HRESULT (*GetDepthStencilSurface)(IDirect3DDevice9*, IDirect3DSurface9**);
    HRESULT (*GetViewport)(IDirect3DDevice9*, D3DVIEWPORT9*);
    HRESULT (*SetClipPlane)(IDirect3DDevice9*, DWORD, const float*);
    HRESULT (*SetDepthStencilSurface)(IDirect3DDevice9*, IDirect3DSurface9*);
    HRESULT (*SetDialogBoxMode)(IDirect3DDevice9*, BOOL);
    HRESULT (*SetFVF)(IDirect3DDevice9*, DWORD);
    HRESULT (*SetIndices)(IDirect3DDevice9*, IDirect3DIndexBuffer9*);
    HRESULT (*SetRenderTarget)(IDirect3DDevice9*, DWORD, IDirect3DSurface9*);
    HRESULT (*SetSamplerState)(IDirect3DDevice9*, DWORD, DWORD, DWORD);
    HRESULT (*SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
    HRESULT (*SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
    HRESULT (*SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
    HRESULT (*SetViewport)(IDirect3DDevice9*, const D3DVIEWPORT9*);
};

struct IDirect3DTexture9Vtbl
{
    ULONG   (*Release)(IDirect3DTexture9*);
    HRESULT (*GetSurfaceLevel)(IDirect3DTexture9*, UINT, IDirect3DSurface9**);
    HRESULT (*AddDirtyRect)(IDirect3DTexture9*, const RECT*);
    DWORD   (*GetLevelCount)(IDirect3DTexture9*);
    HRESULT (*GetLevelDesc)(IDirect3DTexture9*, UINT, D3DSURFACE_DESC*);
};

struct IDirect3DSurface9Vtbl
{
    ULONG   (*Release)(IDirect3DSurface9*);
    HRESULT (*GetDesc)(IDirect3DSurface9*, D3DSURFACE_DESC*);
    HRESULT (*LockRect)(IDirect3DSurface9*, D3DLOCKED_RECT*, const RECT*, DWORD);
    HRESULT (*UnlockRect)(IDirect3DSurface9*);
};

typedef struct IDirect3DVertexBuffer9Vtbl      IDirect3DVertexBuffer9Vtbl;
typedef struct IDirect3DIndexBuffer9Vtbl       IDirect3DIndexBuffer9Vtbl;
typedef struct IDirect3DBaseTexture9Vtbl       IDirect3DBaseTexture9Vtbl;
typedef struct IDirect3DVertexDeclaration9Vtbl IDirect3DVertexDeclaration9Vtbl;
typedef struct IDirect3DVertexShader9Vtbl      IDirect3DVertexShader9Vtbl;
typedef struct IDirect3DSwapChain9Vtbl         IDirect3DSwapChain9Vtbl;

struct IDirect3DVertexBuffer9      { IDirect3DVertexBuffer9Vtbl*      lpVtbl; };
struct IDirect3DIndexBuffer9       { IDirect3DIndexBuffer9Vtbl*       lpVtbl; };
struct IDirect3DBaseTexture9       { IDirect3DBaseTexture9Vtbl*       lpVtbl; };
struct IDirect3DVertexDeclaration9 { IDirect3DVertexDeclaration9Vtbl* lpVtbl; };
struct IDirect3DVertexShader9      { IDirect3DVertexShader9Vtbl*      lpVtbl; };
struct IDirect3DSwapChain9         { IDirect3DSwapChain9Vtbl*         lpVtbl; };

struct IDirect3DVertexBuffer9Vtbl
{
    ULONG   (*Release)(IDirect3DVertexBuffer9*);
    HRESULT (*Lock)(IDirect3DVertexBuffer9*, UINT, UINT, void**, DWORD);
    HRESULT (*Unlock)(IDirect3DVertexBuffer9*);
};
struct IDirect3DIndexBuffer9Vtbl
{
    ULONG   (*Release)(IDirect3DIndexBuffer9*);
    HRESULT (*Lock)(IDirect3DIndexBuffer9*, UINT, UINT, void**, DWORD);
    HRESULT (*Unlock)(IDirect3DIndexBuffer9*);
};
struct IDirect3DBaseTexture9Vtbl
{
    ULONG   (*Release)(IDirect3DBaseTexture9*);
    HRESULT (*GetSurfaceLevel)(IDirect3DBaseTexture9*, UINT, IDirect3DSurface9**);
    HRESULT (*GetDesc)(IDirect3DBaseTexture9*, D3DSURFACE_DESC*);
};
struct IDirect3DVertexDeclaration9Vtbl { ULONG (*Release)(IDirect3DVertexDeclaration9*); };
struct IDirect3DVertexShader9Vtbl      { ULONG (*Release)(IDirect3DVertexShader9*); };
struct IDirect3DSwapChain9Vtbl         { ULONG (*Release)(IDirect3DSwapChain9*); };

typedef IDirect3D9*              LPDIRECT3D9;
typedef IDirect3DDevice9*        LPDIRECT3DDEVICE9;
typedef IDirect3DTexture9*       LPDIRECT3DTEXTURE9;
typedef IDirect3DSurface9*       LPDIRECT3DSURFACE9;
typedef IDirect3DVertexBuffer9*  LPDIRECT3DVERTEXBUFFER9;
typedef IDirect3DIndexBuffer9*   LPDIRECT3DINDEXBUFFER9;

/* ---- constants added for the win/ directory ------------------------
   The values below are chosen to be DISTINCT, not copied from the SDK.
   A flag family gets distinct bits so OR-combinations stay telling
   apart; an enum family gets distinct integers. Nothing may read
   meaning from the numbers: both copies in a differential test share
   this header, so all that matters is that two different constants
   never compare equal.

   That property was missing before. Seven names here were defined as
   0 -- among them D3DTSS_TEXCOORDINDEX and
   D3DTSS_TEXTURETRANSFORMFLAGS -- and swapping those two in
   materialanddraw.c scored completely clean against its differential
   test. They are distinct now and that defect is caught. */
/* D3DFVF */
#define D3DFVF_XYZ                         0x00000001
#define D3DFVF_XYZRHW                      0x00000002
#define D3DFVF_NORMAL                      0x00000004
#define D3DFVF_DIFFUSE                     0x00000008
#define D3DFVF_SPECULAR                    0x00000010
#define D3DFVF_TEX1                        0x00000020
#define D3DFVF_TEX2                        0x00000040
#define D3DFVF_TEX4                        0x00000080
/* D3DUSAGE */
#define D3DUSAGE_DEPTHSTENCIL              0x00000001
#define D3DUSAGE_DYNAMIC                   0x00000002
#define D3DUSAGE_WRITEONLY                 0x00000004
#define D3DUSAGE_AUTOGENMIPMAP             0x00000008
/* D3DCLEAR */
#define D3DCLEAR_ZBUFFER                   0x00000001
/* D3DCOLORWRITEENABLE */
#define D3DCOLORWRITEENABLE_RED            0x00000001
#define D3DCOLORWRITEENABLE_GREEN          0x00000002
#define D3DCOLORWRITEENABLE_BLUE           0x00000004
#define D3DCOLORWRITEENABLE_ALPHA          0x00000008
/* D3DCREATE */
#define D3DCREATE_FPU_PRESERVE             0x00000001
#define D3DCREATE_HARDWARE_VERTEXPROCESSING 0x00000002
#define D3DCREATE_PUREDEVICE               0x00000004
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING 0x00000008
/* D3DTA */
#define D3DTA_CURRENT                      0x00000001
#define D3DTA_DIFFUSE                      0x00000002
#define D3DTA_TEXTURE                      0x00000004
#define D3DTA_TFACTOR                      0x00000008
/* D3DTTFF */
#define D3DTTFF_DISABLE                    0x00000001
#define D3DTTFF_COUNT2                     0x00000002
#define D3DTTFF_COUNT3                     0x00000004
#define D3DTTFF_PROJECTED                  0x00000008
/* D3DCAPS2 */
#define D3DCAPS2_CANAUTOGENMIPMAP          0x00000001
/* D3DTEXOPCAPS */
#define D3DTEXOPCAPS_MODULATE2X            0x00000001
/* D3DTSS_TCI */
#define D3DTSS_TCI_CAMERASPACENORMAL       0x00000001
#define D3DTSS_TCI_CAMERASPACEPOSITION     0x00000002
/* D3DLOCK */
#define D3DLOCK_READONLY                   0x00000001
/* D3DRS */
#define D3DRS_ALPHABLENDENABLE             4097
#define D3DRS_ALPHAFUNC                    4098
#define D3DRS_ALPHAREF                     4099
#define D3DRS_ALPHATESTENABLE              4100
#define D3DRS_BLENDOP                      4101
#define D3DRS_CLIPPLANEENABLE              4102
#define D3DRS_COLORWRITEENABLE             4103
#define D3DRS_CULLMODE                     4104
#define D3DRS_DESTBLEND                    4105
#define D3DRS_FILLMODE                     4106
#define D3DRS_FOGCOLOR                     4107
#define D3DRS_FOGDENSITY                   4108
#define D3DRS_FOGENABLE                    4109
#define D3DRS_FOGEND                       4110
#define D3DRS_FOGSTART                     4111
#define D3DRS_FOGTABLEMODE                 4112
#define D3DRS_FOGVERTEXMODE                4113
#define D3DRS_LIGHTING                     4114
#define D3DRS_NORMALIZENORMALS             4115
#define D3DRS_SRCBLEND                     4116
#define D3DRS_TEXTUREFACTOR                4117
#define D3DRS_ZFUNC                        4118
/* D3DTSS */
#define D3DTSS_ALPHAARG0                   4119
#define D3DTSS_ALPHAARG1                   4120
#define D3DTSS_ALPHAARG2                   4121
#define D3DTSS_ALPHAOP                     4122
#define D3DTSS_BUMPENVLOFFSET              4123
#define D3DTSS_BUMPENVLSCALE               4124
#define D3DTSS_BUMPENVMAT00                4125
#define D3DTSS_BUMPENVMAT01                4126
#define D3DTSS_BUMPENVMAT10                4127
#define D3DTSS_BUMPENVMAT11                4128
#define D3DTSS_COLORARG0                   4129
#define D3DTSS_COLORARG1                   4130
#define D3DTSS_COLORARG2                   4131
#define D3DTSS_COLOROP                     4132
#define D3DTSS_CONSTANT                    4133
#define D3DTSS_RESULTARG                   4134
/* D3DTOP */
#define D3DTOP_ADD                         4135
#define D3DTOP_BLENDCURRENTALPHA           4136
#define D3DTOP_BLENDDIFFUSEALPHA           4137
#define D3DTOP_BLENDFACTORALPHA            4138
#define D3DTOP_DISABLE                     4139
#define D3DTOP_DOTPRODUCT3                 4140
#define D3DTOP_MODULATE                    4141
#define D3DTOP_MODULATE2X                  4142
#define D3DTOP_MODULATEALPHA_ADDCOLOR      4143
#define D3DTOP_SELECTARG1                  4144
#define D3DTOP_SELECTARG2                  4145
#define D3DTOP_SUBTRACT                    4146
/* D3DTS */
#define D3DTS_PROJECTION                   4147
#define D3DTS_TEXTURE0                     4148
#define D3DTS_VIEW                         4149
#define D3DTS_WORLD                        4150
/* D3DBLEND */
#define D3DBLEND_DESTALPHA                 4151
#define D3DBLEND_INVDESTALPHA              4152
#define D3DBLEND_INVSRCALPHA               4153
#define D3DBLEND_INVSRCCOLOR               4154
#define D3DBLEND_ONE                       4155
#define D3DBLEND_SRCALPHA                  4156
#define D3DBLEND_ZERO                      4157
/* D3DCMP */
#define D3DCMP_ALWAYS                      4158
#define D3DCMP_GREATER                     4159
#define D3DCMP_LESS                        4160
#define D3DCMP_LESSEQUAL                   4161
/* D3DCULL */
#define D3DCULL_CCW                        4162
#define D3DCULL_CW                         4163
#define D3DCULL_NONE                       4164
/* D3DFILL */
#define D3DFILL_SOLID                      4165
#define D3DFILL_WIREFRAME                  4166
/* D3DFMT */
#define D3DFMT_A8R8G8B8                    4167
#define D3DFMT_D16                         4168
#define D3DFMT_D16_LOCKABLE                4169
#define D3DFMT_D24X8                       4170
#define D3DFMT_D32                         4171
#define D3DFMT_INDEX16                     4172
#define D3DFMT_UNKNOWN                     4173
#define D3DFMT_X8R8G8B8                    4174
/* D3DFOG */
#define D3DFOG_NONE                        4175
/* D3DPT */
#define D3DPT_TRIANGLELIST                 4176
#define D3DPT_TRIANGLESTRIP                4177
/* D3DSAMP */
#define D3DSAMP_ADDRESSU                   4178
#define D3DSAMP_ADDRESSV                   4179
#define D3DSAMP_MAGFILTER                  4180
#define D3DSAMP_MINFILTER                  4181
#define D3DSAMP_MIPFILTER                  4182
#define D3DSAMP_MIPMAPLODBIAS              4183
/* D3DTADDRESS */
#define D3DTADDRESS_CLAMP                  4184
#define D3DTADDRESS_WRAP                   4185
/* D3DSWAPEFFECT */
#define D3DSWAPEFFECT_COPY                 4186
#define D3DSWAPEFFECT_DISCARD              4187
/* D3DZB */
#define D3DZB_FALSE                        4188
#define D3DZB_TRUE                         4189
/* D3DDEVTYPE */
#define D3DDEVTYPE_HAL                     4190
/* D3DRTYPE */
#define D3DRTYPE_SURFACE                   4191
#define D3DRTYPE_TEXTURE                   4192
/* D3DMULTISAMPLE */
#define D3DMULTISAMPLE_NONE                4193
/* D3DBLENDOP */
#define D3DBLENDOP_ADD                     4194
/* D3DX */
#define D3DX_FILTER_POINT                  4195
/* D3DMISC */
#define D3D9b_SDK_VERSION                  4196
#define D3DERR_DEVICELOST                  4197
#define D3DERR_DEVICENOTRESET              4198
#define D3DX_DEFAULT                       0x5001
#define D3DX_FILTER_LINEAR                 0x5002

IDirect3D9* Direct3DCreate9(UINT SDKVersion);
