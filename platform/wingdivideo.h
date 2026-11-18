#pragma once
#include "wingdi.h"

/*
    The Direct3D side of the fullscreen video display.

    The engine's half of this knows about files, decoding and the
    clock; it does not know what a surface is. So the picture lives
    here: a movie-sized thing the engine writes decoded frames into,
    which this fits to the screen.

    Reached through tGdi_DisplayData like everything else the engine
    draws with; the names below are what the interface is filled with.
*/

/* A picture of this size to write frames into. Non-zero when there
   is one; the engine plays nothing if there is not. */
int  D3D_b_VideoDisplayCreate(int width, int height);
void D3D_VideoDisplayClose(void);

/* Hands out the picture's memory to write a frame into. The height
   is the picture's own, so whatever fills it cannot be told a size
   that does not belong to it. */
int  D3D_b_VideoFrameLock(void** bits, int* pitch, int* height);
void D3D_VideoFrameUnlock(void);

/* Puts the frame on the screen, fitted as `fit` asks. The movie's
   size is passed rather than remembered, because the engine is what
   knows it. */
void D3D_VideoFramePresent(int width, int height, int fit);
