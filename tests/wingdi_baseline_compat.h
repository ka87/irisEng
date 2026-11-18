#pragma once

/* What the frozen copy of platform/wingdi.c still refers to and the live
 * file does not.
 *
 * Forced in on the baseline's compile line alone, the same way the
 * D3DX header is: if the live file ever needed one of these again, it
 * would not compile, which is the point.
 *
 * PRO_rasterHWND was the handle of the Win32 rasters window. The
 * in-game overlay draws the rasters now, nothing creates that window
 * any more, and so the handle was never anything but null -- the
 * frozen copy's two InvalidateRect calls on it could not fire. The
 * declaration lives here because the engine no longer has the global
 * at all; tests/wingdi_mocks.c defines it so this side links.
 */
extern HWND PRO_rasterHWND;
