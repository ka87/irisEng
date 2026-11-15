#pragma once

#include "math.h"

struct sDDCamStruct;

/* Fitting a camera to a drawing surface: the viewport it occupies and
   the projection matrix that goes with it.

   This was inside the Direct3D renderer, and almost none of it is
   Direct3D's. It is arithmetic on the camera and on the shape of the
   surface -- the aspect ratio the game was authored for, the
   letterboxing rules, the three kinds of projection the engine uses.
   A second renderer needs exactly the same answers, and a second copy
   of a hundred and twenty lines of this would drift.

   So the computation is here and pure: it reads a camera and a
   description of the surface, and writes a matrix and a rectangle.
   What a renderer then does with them -- hand the matrix to a device,
   set a viewport -- is the renderer's own. */

/* What the computation needs to know about the surface and the
   display settings. A renderer fills this in from whatever it keeps. */
typedef struct sPrj_Input
{
    int   surfaceWidth;          /* the drawing rectangle, in pixels */
    int   surfaceHeight;
    int   selectedScreenRatio;   /* an index into GDI_gaf_ScreenRatio */
    float manualScreenRatio;     /* used when that index is out of range */
    float pixelYoverX;
    int   flags2;                /* the display settings' fit flags */
    int   widescreen;
    float realYoverX;            /* the surface's own aspect, measured */
    float refYoverX;             /* the aspect the game was authored at */
} tPrj_Input;

typedef struct sPrj_Output
{
    /* The projection to hand to the device.

       The camera's own matrix is written as a side effect, because
       that is what the renderer did and something reads it back: two
       of the three projections are built there, and the third leaves
       an identity behind that is not the projection. MATH_SetIdentityMatrix
       masks the matrix's flag word rather than clearing it, so which
       matrix it is given is not a detail either. */
    Mtx4x4 matrix;

    /* Whether the viewport below should be set. The 2D projection
       leaves the viewport alone -- it is used for drawing into a
       target whose viewport was already chosen. */
    int    setViewport;

    int    viewX;
    int    viewY;
    int    viewWidth;
    int    viewHeight;
} tPrj_Output;

/* Computes both. Writes the camera's viewPortRealPos fields, and
   fills in its viewport size and position if they were never set,
   which is what the renderer did. */
void PRJ_Compute(struct sDDCamStruct* cam, const tPrj_Input* in,
                 tPrj_Output* out);

/* The off-centre projection the isometric cameras use, and the one
   the renderer still exposes under its own name for the engine to
   call directly.

   Note where the offsets land: the fourth column, not the fourth row,
   which is the transpose of the convention the perspective projection
   is written in. And the depth scale is -4/(far - near) rather than
   the -2 a textbook orthographic projection has. Both are as the game
   shipped; neither is tidied here. */
void PRJ_OffCentre(Mtx4x4* out, float left, float right, float bottom,
                   float top, float nearZ, float farZ);
