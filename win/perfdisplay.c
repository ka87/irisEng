#include "perfdisplay.h"

#include "../perfbars.h"
#include "../profiler.h"
#include "../gdi.h"
#include "../font.h"
#include "../dbg.h"

#include <stdio.h>
#include <string.h>

/* Room for the bars: a quad is six vertices, and a frame draws the
   band, the backdrop, and up to eight segments for each of the rows
   that fit. */
#define PFD_MAXQUADS 512

static tGdi_Vertex pfd_verts[PFD_MAXQUADS * 6];
static int         pfd_vertCount;

/* The running averages the order is taken on, and the counter reading
   the frame rate is measured from. Both belong to the display rather
   than to the profiler: they are what it shows, not what it times. */
static double  pfd_avg[PFB_RASTERS];
static int64_t pfd_lastTick;

static void PFD_l_Quad(unsigned int color, float x, float y,
                       float w, float h)
{
    tGdi_Vertex* q;
    float        right;
    float        bottom;
    int          i;

    if (pfd_vertCount + 6 > PFD_MAXQUADS * 6 || w <= 0.0f || h <= 0.0f)
        return;

    q = &pfd_verts[pfd_vertCount];
    right = x + w;
    bottom = y + h;

    q[0].pos.x = x;      q[0].pos.y = y;
    q[1].pos.x = right;  q[1].pos.y = y;
    q[2].pos.x = x;      q[2].pos.y = bottom;
    q[3].pos.x = x;      q[3].pos.y = bottom;
    q[4].pos.x = right;  q[4].pos.y = y;
    q[5].pos.x = right;  q[5].pos.y = bottom;
    for (i = 0; i < 6; i++)
    {
        q[i].pos.z = 0.0f;
        q[i].color = color;
    }
    pfd_vertCount += 6;
}

/* One row: its name on the left, its number on the right, and the bar
   between them. The bar's segments come from perfbars.c, which is
   where the other renderer's display gets them too. */
static void PFD_l_Row(unsigned int color, const char* name, float value,
                      float y, float width)
{
    tPfb_Quad quads[8];
    char      text[32];
    int       n;
    int       i;

    sprintf(text, "%5.3f", value);
    FNT_DrawText(FNT_RASTERS, name, 20, (unsigned int)y, 0xFFFFFFFF);
    FNT_DrawText(FNT_RASTERS, text, (unsigned int)(width - 50.0f),
                 (unsigned int)y, 0xFFFFFFFF);

    n = PFB_i_BarQuads(quads, 8, color, value, y, width);
    for (i = 0; i < n; i++)
        PFD_l_Quad(quads[i].color, quads[i].x, quads[i].y,
                   quads[i].w, quads[i].h);
}

void PFD_Draw(void)
{
    int     order[PFB_RASTERS];
    int64_t now;
    int64_t freq;
    double  scale;
    float   fps;
    float   width;
    float   height;
    int     shown;
    int     drawW = 0;
    int     drawH = 0;
    int     row;

    if (!tweak_perf)
        return;
    if (!gpst_DX_stDD || !gpst_DX_stDD->b_Ready())
        return;

    /* The display's own cost is timed like anything else: two rasters
       are closed here and reopened at the end, and a third brackets
       the drawing itself. */
    PRO_QPC_RecordTimeDecRef(53);
    PRO_QPC_RecordTimeDecRef(19);
    PRO_QPC_StartTime(47);

    now = PRO_i64_QPC();
    freq = PRO_i64_QPCFrequency();
    fps = PFB_f_Fps(now - pfd_lastTick, freq);
    pfd_lastTick = now;

    if (PFB_b_On(tweak_perf, fps, tweak_perf_fps_min))
    {
        gpst_DX_stDD->GetDrawingSize(&drawW, &drawH);
        width = (float)drawW;
        height = (float)drawH;
        pfd_vertCount = 0;

        gpst_DX_stDD->OverlayBegin();

        if (PFB_b_Band(fps, tweak_perf_fps_min))
            PFD_l_Quad(0xFFFF0000u, 0.0f, 0.0f, width, PFB_BAND_H);

        if (PFB_b_Rows(tweak_perf))
        {
            scale = PFB_d_Scale(tweak_perf_fps_target, freq);
            PFB_Smooth(pfd_avg, PerfCountertimeSumArray, PFB_RASTERS);
            PFB_Order(order, pfd_avg, PFB_RASTERS);
            shown = PFB_i_Rows(tweak_perf,
                               PFB_i_Fitting(PerfCountertimeSumArray,
                                             rasterNames, order,
                                             PFB_RASTERS, scale, height));

            /* The backdrop first, so the rows sit on it. The other
               renderer emits it in the same place and flushes its
               batch there; here the whole page is one batch, which is
               the overlay's way and leaves the picture the same. */
            PFD_l_Quad(0x80000000u, 0.0f, PFB_FPS_Y, width,
                       (float)((double)shown * (double)PFB_ROW_H));

            {
                float y = PFB_FIRST_Y;

                for (row = 0; row < shown; row++)
                {
                    int   raster = order[row];
                    int   kind = rasterNames[raster].unk1;
                    float value = PFB_f_Value(
                        PerfCountertimeSumArray[raster], scale);

                    if (kind && value > 0.01f)
                    {
                        PFD_l_Row(PFB_u_RowColour(raster),
                                  rasterNames[raster].name, value, y,
                                  width);
                        y = (float)((double)y + (double)PFB_ROW_H);
                        if (y > height)
                            break;
                    }
                }
            }
            PFD_l_Row(0xFFFFFFu, "Frame Rate", fps, PFB_FPS_Y, width);
        }

        /* The shapes as one batch, then the strings over them -- the
           font draws as it is called, so the rows' text has already
           gone out by here and lands on top of the quads either way.
           Kept in this order so the page reads like the overlay's. */
        gpst_DX_stDD->OverlayDrawQuads(pfd_verts, pfd_vertCount / 3);
        pfd_vertCount = 0;
        gpst_DX_stDD->OverlayEnd();
    }

    /* The sums are a frame's worth because this clears them once it has
       shown them. Nothing else does, which is why a renderer without
       this display showed bars that only ever grew. */
    PRO_QPC_ResetSums();
    PRO_QPC_RecordTimeDecRef(47);
    PRO_QPC_StartTime(19);
    PRO_QPC_StartTime(53);
}
