#pragma once

#include "common.h"
#include "profiler.h"

/* The performance display, with no renderer in it.
 *
 * `tweak perf` puts a page of bars over the frame: one row per
 * profiler raster, longest first, with a frame-rate row above them and
 * a red band across the top when the frame rate is above its floor.
 * Everything here is what that display *decides* -- the frame rate,
 * the smoothing, the order, which rows are shown, a row's colour and a
 * bar's segments. What a renderer has to supply is a screen-space
 * coloured quad and a line of text.
 *
 * It is a module rather than two copies because the arithmetic is the
 * display: the smoothing constant, the clamp at four units, the
 * shrinking segments and the colour bits are what make the page
 * readable, and a second renderer writing them again from the picture
 * would get a page that looked nearly right.
 */

/* How many rasters the profiler keeps, and so how long every array
   here is. The same 54 as PerfCountertimeSumArray. */
#define PFB_RASTERS 54

/* The rows' geometry, in pixels, as the display has always had them. */
#define PFB_BAND_H    40.0f      /* the red band across the top */
#define PFB_FPS_Y     50.0f      /* the frame-rate row */
#define PFB_FIRST_Y   70.0f      /* the first raster row */
#define PFB_ROW_H     15.0f      /* one row to the next */
#define PFB_BAR_H     10.0f      /* the coloured bar inside a row */

/* A quad for the renderer to draw: screen pixels, and a colour in the
   engine's own 0xAARRGGBB. */
typedef struct sPfb_Quad
{
    unsigned int color;
    float        x;
    float        y;
    float        w;
    float        h;
} tPfb_Quad;

/* The frame rate, from a frame's worth of performance-counter ticks
   and the counter's frequency. Zero ticks answer zero rather than
   dividing by it. */
float PFB_f_Fps(int64_t ticks, int64_t freq);

/* Whether the display is drawn at all this frame.
 *
 * Mode 3 is the odd one: it draws only while the frame rate is *above*
 * the floor, so it shows nothing exactly when the frame is slow. That
 * reads backwards and it is what the display does -- the floor is the
 * rate at which the band appears, not a budget being missed. */
int PFB_b_On(int tweakPerf, float fps, float fpsMin);

/* Whether the red band across the top is drawn. */
int PFB_b_Band(float fps, float fpsMin);

/* Whether the rows are drawn, which every mode but 3 does. */
int PFB_b_Rows(int tweakPerf);

/* What a raster's summed ticks are multiplied by to become a bar's
   length in units, where one unit is a whole segment and the target
   frame time is one. */
double PFB_d_Scale(float fpsTarget, int64_t freq);

/* A raster's value, in those units. */
float PFB_f_Value(int64_t sum, double scale);

/* The smoothing the order is taken on: a twentieth of this frame and
 * the rest of the running average, per raster.
 *
 * It is the average that is sorted and the raw sum that is shown, so a
 * row's position settles while its number still moves. Both are the
 * display's own behaviour. */
void PFB_Smooth(double* avg, const int64_t* sums, int count);

/* The rasters in order, longest average first. Written as the
   selection sort the display has, because a stable sort would order
   equal averages differently and the rows would sit in another
   order. */
void PFB_Order(int* order, const double* avg, int count);

/* How many rows reach the bottom of the screen: the rasters with a
 * kind and a value over a hundredth, counted in order until the next
 * row would fall off.
 *
 * The answer is the index the walk stopped at, so a page that fits
 * entirely answers the raster count and not the number of rows with
 * anything in them -- which is what the display has always passed on,
 * and why mode 1 can ask for more rows than it then draws. */
int PFB_i_Fitting(const int64_t* sums, const tRasterNames* names,
                  const int* order, int count, double scale,
                  float viewHeight);

/* How many rows this mode shows: all of the above for mode 1, and one
   for every other mode. */
int PFB_i_Rows(int tweakPerf, int fitting);

/* A row's colour, from the raster's own index: two greys alternating,
   with green and red added by two more bits of it. */
unsigned int PFB_u_RowColour(int raster);

/* A bar, as the quads it is made of.
 *
 * The bar is a run of segments and each one is a quarter the width of
 * the one before, so the first unit of time takes three quarters of
 * the width and the fourth is barely a mark -- a row at the target
 * frame time fills the bar and one at four times it still fits. The
 * colour is flipped between segments so the units can be counted.
 *
 * The value is clamped at four units. Returns how many quads were
 * written; `max` of 8 is always enough. */
int PFB_i_BarQuads(tPfb_Quad* out, int max, unsigned int color,
                   float value, float y, float width);
