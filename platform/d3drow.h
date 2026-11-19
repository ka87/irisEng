#pragma once

/* The Direct3D renderer's row in the table, named without naming
 * Direct3D.
 *
 * gdi.c holds the table of renderers and fills none of them itself,
 * except that for a long time it did fill this one: the row's function
 * sat there and assigned sixty-odd D3D_* entries, so gdi.c -- the one
 * file that is supposed to know both renderers and neither renderer's
 * types -- included the Direct3D headers and could not be compiled on
 * a machine without them.
 *
 * The row lives in d3drow.c now, which is built only where Direct3D
 * is. These two declarations name nothing but the interface, so gdi.c
 * can see them wherever it is built, and the table carries the row
 * only when there is one.
 */

struct sGdi_DisplayData;

/* Whether this build has the Direct3D renderer at all. It is a value
   rather than a defined/undefined, so a wrong spelling is a build
   error instead of a silently absent renderer. */
#if defined(_WIN32) || __has_include(<d3d9.h>)
#define GDI_HAVE_D3D 1
#else
#define GDI_HAVE_D3D 0
#endif

#if GDI_HAVE_D3D

/* Writes every entry of the interface. The engine's side of the
   renderer seam; what the entries do is the renderer's business. */
void D3D_FillRenderer(struct sGdi_DisplayData* dd);

/* Whether it can run on the machine in front of it. It always can:
   this is the renderer the game shipped on, and a Windows build has
   it by definition. */
int D3D_b_Available(void);

#endif
