#pragma once
#include <d3d9.h>

/* The two D3DX texture calls the renderer used, implemented here.
   Both deal only in 32-bit ARGB surfaces, which is all the engine
   ever handed them: the upload path rejects anything that is not 24
   or 32 bits deep before it gets this far. */

/* Copies a 32-bit image into a surface, row by row, honouring the
   surface's own pitch. This stands in for D3DXLoadSurfaceFromMemory
   at the one place it was called: same dimensions, same 32-bit
   layout, point filter -- which together mean a copy and nothing
   else.

   The destination there is X8R8G8B8 and the source A8R8G8B8. D3DX
   would have dropped the alpha; copying it into the byte that
   X8R8G8B8 ignores comes to the same thing when the surface is
   sampled, and costs nothing. */
int D3D_b_SurfaceLoadFromMemory(IDirect3DSurface9* surface, const void* src,
                                int srcPitch, int width, int height);

/* Fills levels 1 and up of a texture by box-filtering the level
   above each one, which is what D3DXFilterTexture did with a default
   filter. A texture with a single level is left alone, and so is one
   whose format is not 32 bits.

   Each destination pixel is the average of the (up to) 2x2 block
   above it, per channel, rounded rather than truncated. An odd
   dimension halves to the floor and the last row or column of the
   source is averaged with itself rather than being dropped. */
int D3D_b_TextureBuildMipLevels(IDirect3DTexture9* texture);
