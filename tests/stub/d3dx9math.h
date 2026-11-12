#pragma once
#include <d3d9.h>

typedef struct D3DXVECTOR2 { float x, y; } D3DXVECTOR2;
typedef struct D3DXVECTOR3 { float x, y, z; } D3DXVECTOR3;
typedef struct D3DXVECTOR4 { float x, y, z, w; } D3DXVECTOR4;
typedef struct D3DXQUATERNION { float x, y, z, w; } D3DXQUATERNION;
typedef struct D3DXPLANE { float a, b, c, d; } D3DXPLANE;
typedef struct D3DXMATRIX { float m[4][4]; } D3DXMATRIX;
typedef D3DXMATRIX* LPD3DXMATRIX;
typedef D3DXVECTOR3* LPD3DXVECTOR3;
typedef struct ID3DXFont ID3DXFont;
typedef struct ID3DXSprite ID3DXSprite;
typedef struct ID3DXBuffer ID3DXBuffer;
typedef ID3DXFont* LPD3DXFONT;
typedef ID3DXSprite* LPD3DXSPRITE;
