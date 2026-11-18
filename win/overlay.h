#pragma once

#include "../common.h"

// The in-game overlay: a screen-space layer drawn in D3D on top of
// everything else, and the few widgets built on it. No assets and no
// Win32 windows; every shape is quads and every string goes through
// the debug font.

// Drawn once per frame from D3D_Flip, after the profiler bars.
void OVL_Draw(void);

// F1: nothing -> the summary -> the detail page -> nothing.
void OVL_RastersCycleView(void);

// The command field at the top of the screen. While it is up,
// DirectInput is released and the keyboard arrives as window
// messages instead.
int  OVL_b_CommandActive(void);
void OVL_CommandShow(void);
void OVL_CommandHide(void);

// Window-message entry points, live only while the field is up.
// OVL_b_CommandChar returns non-zero when the field consumed the
// character, which it does whenever it is up.
int  OVL_b_CommandChar(int ch);
