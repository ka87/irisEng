#include "perfbars.h"

float PFB_f_Fps(int64_t ticks, int64_t freq)
{
    if (ticks == 0)
        return 0.0f;
    return (float)((double)freq / (double)ticks);
}

int PFB_b_On(int tweakPerf, float fps, float fpsMin)
{
    if (!tweakPerf)
        return 0;
    /* Every mode but 3 draws whatever the frame rate is; 3 draws only
       above the floor. See the header. */
    return tweakPerf != 3 || fps > (double)fpsMin;
}

int PFB_b_Band(float fps, float fpsMin)
{
    /* The comparison is made in double, as the display makes it: the
       floor is a float and widening it is not the same as narrowing
       the frame rate. */
    return fps > (double)fpsMin;
}

int PFB_b_Rows(int tweakPerf)
{
    return tweakPerf != 3;
}

double PFB_d_Scale(float fpsTarget, int64_t freq)
{
    if (freq == 0)
        return 0.0;
    return (double)fpsTarget / (double)freq;
}

float PFB_f_Value(int64_t sum, double scale)
{
    return (float)((double)sum * scale);
}

void PFB_Smooth(double* avg, const int64_t* sums, int count)
{
    int i;

    if (!avg || !sums)
        return;
    for (i = 0; i < count; i++)
        avg[i] = (double)sums[i] * 0.05 + avg[i] * 0.95;
}

void PFB_Order(int* order, const double* avg, int count)
{
    int i;
    int j;

    if (!order || !avg)
        return;
    for (i = 0; i < count; i++)
        order[i] = i;
    /* The selection sort the display has, descending, and written out
     * rather than handed to qsort for a sharper reason than taste.
     *
     * It swaps as soon as it finds anything larger rather than finding
     * the largest and swapping once, so it **permutes equal averages**:
     * with three rasters busy and the rest at nought, the tail comes
     * out 3, 4, 5, 6, 7, 8, 9, 0, 11 and not in index order. That is
     * an artefact of the eager swap, and it decides the order of every
     * row that has not run -- the bottom of the page -- so any other
     * sort, stable or not, would draw a different page from the same
     * frame. */
    for (i = 0; i < count - 1; i++)
        for (j = i + 1; j < count; j++)
            if (avg[order[i]] < avg[order[j]])
            {
                int swapped = order[i];

                order[i] = order[j];
                order[j] = swapped;
            }
}

int PFB_i_Fitting(const int64_t* sums, const tRasterNames* names,
                  const int* order, int count, double scale,
                  float viewHeight)
{
    float y;
    int   i;

    if (!sums || !names || !order)
        return 0;

    y = PFB_FIRST_Y;
    for (i = 0; i < count; i++)
    {
        int raster = order[i];

        if (names[raster].unk1
            && (double)sums[raster] * scale > 0.01)
        {
            y = (float)(y + (double)PFB_ROW_H);
            if (y > viewHeight)
                break;
        }
    }
    /* The index the walk stopped at, which is `count` when everything
       fitted -- see the header. */
    return i;
}

int PFB_i_Rows(int tweakPerf, int fitting)
{
    if (tweakPerf == 1)
        return fitting;
    return 1;
}

unsigned int PFB_u_RowColour(int raster)
{
    unsigned char idx = (unsigned char)raster;
    unsigned int  color = 0xF0808020u;

    /* Two greys, alternating by the lowest bit of the index less one,
       and then green and red added by the next two bits of it plus
       one. Three different offsets of the same index, which is what
       makes neighbouring rows differ in more than one channel. */
    if ((unsigned char)(idx - 1) & 1)
        color = 0xF0808060u;
    if ((unsigned char)(idx + 1) & 2)
        color |= 0x4000u;
    if ((unsigned char)(idx + 1) & 4)
        color |= 0x400000u;
    return color;
}

int PFB_i_BarQuads(tPfb_Quad* out, int max, unsigned int color,
                   float value, float y, float width)
{
    float x;
    float segment;
    int   n = 0;

    if (!out || max <= 0)
        return 0;

    if (value > 4.0)
        value = 4.0f;
    x = 70.0f;
    /* The first segment is three quarters and a shade of what is left
       of the width once both margins are taken; each one after it is a
       quarter of the one before. The odd-looking constant is the
       display's own. */
    for (segment = (float)(((double)width - 140.0) * 0.75294119);
         value > 0.0f;
         segment = (float)((double)segment * 0.25))
    {
        double unit = value >= 1.0f ? 1.0 : (double)value;
        float  barWidth = (float)(unit * (double)segment);

        if (n >= max)
            return n;
        out[n].color = color;
        out[n].x = x;
        out[n].y = y;
        out[n].w = barWidth;
        out[n].h = PFB_BAR_H;
        n++;

        color ^= 0xFFFFu;
        x = barWidth + x;
        value = (float)((double)value - 1.0);
    }
    return n;
}
