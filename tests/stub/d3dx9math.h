#pragma once
#include <d3d9.h>

typedef struct tagPALETTEENTRY { BYTE peRed, peGreen, peBlue, peFlags; } PALETTEENTRY;
typedef struct D3DXVECTOR2 { float x, y; } D3DXVECTOR2;
typedef struct D3DXVECTOR3 { float x, y, z; } D3DXVECTOR3;
typedef struct D3DXVECTOR4 { float x, y, z, w; } D3DXVECTOR4;
typedef struct D3DXQUATERNION { float x, y, z, w; } D3DXQUATERNION;
typedef struct D3DXPLANE { float a, b, c, d; } D3DXPLANE;
typedef struct D3DXMATRIX { float m[4][4]; } D3DXMATRIX;
typedef D3DXMATRIX* LPD3DXMATRIX;
typedef D3DXVECTOR3* LPD3DXVECTOR3;
typedef struct ID3DXFontVtbl ID3DXFontVtbl;
typedef struct ID3DXFont { ID3DXFontVtbl* lpVtbl; } ID3DXFont;
struct ID3DXFontVtbl {
    ULONG (*Release)(ID3DXFont*);
    int   (*DrawTextA)(ID3DXFont*, void*, LPCSTR, int, RECT*, DWORD, D3DCOLOR);
    HRESULT (*OnLostDevice)(ID3DXFont*);
    HRESULT (*OnResetDevice)(ID3DXFont*);
};
typedef struct ID3DXSprite ID3DXSprite;
typedef struct ID3DXBuffer ID3DXBuffer;
typedef ID3DXFont* LPD3DXFONT;
typedef ID3DXSprite* LPD3DXSPRITE;

/* The D3DX surface the win/ directory uses. Declarations only; the
   values of the filter flags live in d3d9.h with the rest. */
typedef struct _D3DXFONT_DESCA {
    int Height, Width; UINT Weight, MipLevels; BOOL Italic;
    BYTE CharSet, OutputPrecision, Quality, PitchAndFamily;
    CHAR FaceName[32];
} D3DXFONT_DESCA;

HRESULT D3DXCreateFontIndirectA(IDirect3DDevice9*, const D3DXFONT_DESCA*, LPD3DXFONT*);
HRESULT D3DXFilterTexture(IDirect3DBaseTexture9*, const PALETTEENTRY*, UINT, DWORD);
HRESULT D3DXLoadSurfaceFromMemory(IDirect3DSurface9*, const PALETTEENTRY*, const RECT*,
                                  const void*, D3DFORMAT, UINT, const PALETTEENTRY*,
                                  const RECT*, DWORD, D3DCOLOR);
D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX*, float*, const D3DXMATRIX*);
D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX*, const D3DXMATRIX*, const D3DXMATRIX*);
D3DXMATRIX* D3DXMatrixOrthoOffCenterRH(D3DXMATRIX*, float, float, float, float, float, float);
D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX*, float, float, float);
