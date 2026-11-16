/* The performance display's arithmetic.
 *
 * What is worth pinning here is not the layout constants -- those are
 * read straight off the other renderer's display -- but the four
 * decisions a second implementation would get subtly wrong:
 *
 *   the mode-3 gate, which draws only while the frame rate is *above*
 *   its floor, so it shows nothing exactly when the frame is slow;
 *
 *   the fitting count, which answers the index the walk stopped at and
 *   so answers the raster count when the whole page fits -- more rows
 *   than the page then draws;
 *
 *   the sort, which is a selection sort and not a library one, because
 *   every raster that has not run averages nought and a stable order
 *   is the difference between a readable page and rows that jump;
 *
 *   the bar, whose segments shrink by a quarter each, so the first
 *   unit of time takes three quarters of the width and the fourth is
 *   a mark -- and whose colour flips between them.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../perfbars.c"

static int checks;
static int failures;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond) { failures++; printf("  FAIL %s\n", what); }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        failures++;
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
    }
}

static void okNear(double got, double want, double eps, const char* what)
{
    checks++;
    if (!(fabs(got - want) <= eps))
    {
        failures++;
        printf("  FAIL %s: got %f want %f\n", what, got, want);
    }
}

static void caseFps(void)
{
    printf("-- the frame rate\n");

    okNear(PFB_f_Fps(1000, 60000), 60.0, 1e-6,
           "a sixtieth of the counter is sixty");
    okNear(PFB_f_Fps(60000, 60000), 1.0, 1e-6, "a whole second is one");
    okEq((long)PFB_f_Fps(0, 60000), 0,
         "and no ticks at all is nought rather than a division by it");
}

static void caseGates(void)
{
    printf("-- what is drawn, and the mode that reads backwards\n");

    okEq(PFB_b_On(0, 100.0f, 20.0f), 0, "mode nought draws nothing");
    ok(PFB_b_On(1, 100.0f, 20.0f), "mode one draws");
    ok(PFB_b_On(1, 5.0f, 20.0f),
       "and goes on drawing when the frame is slow");
    ok(PFB_b_On(2, 5.0f, 20.0f), "so does mode two");

    /* Mode 3 is the one that reads backwards: above the floor it
       draws, below it there is nothing on the screen at all. */
    ok(PFB_b_On(3, 100.0f, 20.0f), "mode three draws above the floor");
    okEq(PFB_b_On(3, 5.0f, 20.0f), 0, "and nothing below it");

    okEq(PFB_b_Rows(3), 0, "mode three has no rows");
    ok(PFB_b_Rows(1) && PFB_b_Rows(2), "every other mode has them");

    /* The band is the frame rate being *up*, which is the same
       comparison the gate makes -- so in mode 3 the band is always
       there when anything is. */
    ok(PFB_b_Band(100.0f, 20.0f), "the band is up with the frame rate");
    okEq(PFB_b_Band(5.0f, 20.0f), 0, "and gone when it drops");

    /* Equality is not above: a frame rate exactly at the floor shows
       no band, which is what a comparison written the other way round
       would get wrong and no picture would make obvious. */
    okEq(PFB_b_Band(20.0f, 20.0f), 0,
         "exactly at the floor is not above it");
}

static void caseSmoothing(void)
{
    double   avg[PFB_RASTERS];
    int64_t  sums[PFB_RASTERS];
    int      i;

    printf("-- the running average the order is taken on\n");

    memset(avg, 0, sizeof(avg));
    for (i = 0; i < PFB_RASTERS; i++)
        sums[i] = 100;

    PFB_Smooth(avg, sums, PFB_RASTERS);
    okNear(avg[0], 5.0, 1e-9, "a frame moves it a twentieth of the way");
    PFB_Smooth(avg, sums, PFB_RASTERS);
    okNear(avg[0], 9.75, 1e-9, "and a twentieth of what is left again");

    /* Twenty frames is most of the way but not all: the average is
       deliberately slow, which is what stops the rows swapping every
       frame. */
    for (i = 0; i < 18; i++)
        PFB_Smooth(avg, sums, PFB_RASTERS);
    ok(avg[0] > 60.0 && avg[0] < 100.0,
       "twenty frames is most of the way there and not all of it");
}

static void caseOrder(void)
{
    double avg[PFB_RASTERS];
    int    order[PFB_RASTERS];
    int    i;

    printf("-- longest first, and equals left where they were\n");

    for (i = 0; i < PFB_RASTERS; i++)
        avg[i] = 0.0;
    avg[10] = 5.0;
    avg[20] = 9.0;
    avg[30] = 7.0;

    PFB_Order(order, avg, PFB_RASTERS);
    okEq(order[0], 20, "the longest is first");
    okEq(order[1], 30, "then the next");
    okEq(order[2], 10, "then the last of the three");

    /* Everything else averages nought, and they do **not** keep their
       own order: the sort swaps as soon as it finds anything larger
       rather than finding the largest and swapping once, so equal
       averages get permuted. The nine below are measured from this
       sort, not derived -- they are the fingerprint that says the page
       is ordered by this algorithm and not by a tidier one, and every
       row that has not run is ordered by it. */
    {
        static const int tail[9] = { 3, 4, 5, 6, 7, 8, 9, 0, 11 };
        int bad = -1;

        for (i = 0; i < 9; i++)
            if (order[3 + i] != tail[i])
            {
                bad = i;
                break;
            }
        okEq(bad, -1, "and the rasters at nought are permuted as this"
                      " sort permutes them");
    }

    /* Whatever the order, it is an order: every raster appears once,
       which a sort that swapped the wrong index would break while
       still looking sorted at the top. */
    {
        int seen[PFB_RASTERS];
        int missing = 0;

        memset(seen, 0, sizeof(seen));
        for (i = 0; i < PFB_RASTERS; i++)
            if (order[i] >= 0 && order[i] < PFB_RASTERS)
                seen[order[i]]++;
        for (i = 0; i < PFB_RASTERS; i++)
            if (seen[i] != 1)
                missing++;
        okEq(missing, 0, "and every raster is in the order exactly once");
    }

    /* And it really is descending, read over the whole array rather
       than the three rows the case placed. */
    {
        int wrong = 0;

        for (i = 0; i + 1 < PFB_RASTERS; i++)
            if (avg[order[i]] < avg[order[i + 1]])
                wrong++;
        okEq(wrong, 0, "with no row shorter than the one below it");
    }
}

static void caseFitting(void)
{
    int64_t      sums[PFB_RASTERS];
    tRasterNames names[PFB_RASTERS];
    int          order[PFB_RASTERS];
    double       avg[PFB_RASTERS];
    int          i;
    double       scale = 1.0;

    printf("-- how many rows reach the bottom of the screen\n");

    for (i = 0; i < PFB_RASTERS; i++)
    {
        sums[i] = 1;            /* a value of one, well over a hundredth */
        names[i].unk1 = 2;
        names[i].name[0] = 0;
        avg[i] = (double)(PFB_RASTERS - i);
    }
    PFB_Order(order, avg, PFB_RASTERS);

    /* A tall screen takes the lot, and then the answer is the raster
       count rather than the number of rows -- the walk ran off the end
       instead of stopping. */
    okEq(PFB_i_Fitting(sums, names, order, PFB_RASTERS, scale, 10000.0f),
         PFB_RASTERS, "a tall screen fits every one of them");

    /* A short one stops partway: the first row is at 70 and each is 15
       more, so a 130-pixel screen takes four before the fifth would
       pass the bottom. */
    okEq(PFB_i_Fitting(sums, names, order, PFB_RASTERS, scale, 130.0f), 4,
         "and a short screen stops where the rows run out of room");

    /* A raster with no kind is not a row at all, and nor is one under
       a hundredth. */
    for (i = 0; i < PFB_RASTERS; i++)
        names[i].unk1 = 0;
    names[order[0]].unk1 = 2;
    names[order[1]].unk1 = 2;
    okEq(PFB_i_Fitting(sums, names, order, PFB_RASTERS, scale, 130.0f),
         PFB_RASTERS, "rasters with no kind are skipped, not counted");

    for (i = 0; i < PFB_RASTERS; i++)
        names[i].unk1 = 2;
    okEq(PFB_i_Fitting(sums, names, order, PFB_RASTERS, 0.001, 130.0f),
         PFB_RASTERS, "and so are the ones under a hundredth");
}

static void caseRows(void)
{
    printf("-- and how many this mode shows\n");

    okEq(PFB_i_Rows(1, 9), 9, "mode one shows what fits");
    okEq(PFB_i_Rows(2, 9), 1, "mode two shows one row");
    okEq(PFB_i_Rows(0, 9), 1, "and so does anything else");
}

static void caseRowColour(void)
{
    unsigned int a;
    unsigned int b;

    printf("-- a row's colour, from the raster's own index\n");

    /* Neighbours differ: the three bits that decide the greys, the
       green and the red are taken at different offsets of the index,
       so no two adjacent rows come out the same colour. */
    {
        int i;
        int same = 0;

        for (i = 0; i + 1 < PFB_RASTERS; i++)
            if (PFB_u_RowColour(i) == PFB_u_RowColour(i + 1))
                same++;
        okEq(same, 0, "no two neighbouring rows share a colour");
    }

    /* And every one of them is opaque, or the bars would be a wash. */
    {
        int i;
        int clear = 0;

        for (i = 0; i < PFB_RASTERS; i++)
            if ((PFB_u_RowColour(i) >> 24) < 0xF0u)
                clear++;
        okEq(clear, 0, "and all of them are opaque");
    }

    a = PFB_u_RowColour(0);
    b = PFB_u_RowColour(1);
    ok((a & 0x00FFFFFFu) != (b & 0x00FFFFFFu),
       "the difference is in the colour and not only the alpha");
}

static void caseBar(void)
{
    tPfb_Quad q[8];
    int       n;

    printf("-- the bar's shrinking segments\n");

    /* Half the target frame time: one segment, half of the first
       segment's width, starting at 70. */
    n = PFB_i_BarQuads(q, 8, 0x11223344u, 0.5f, 100.0f, 640.0f);
    okEq(n, 1, "half a unit is one segment");
    okNear(q[0].x, 70.0, 1e-6, "starting at the left margin");
    okNear(q[0].w, (640.0 - 140.0) * 0.75294119 * 0.5, 1e-3,
           "and half the first segment wide");
    okNear(q[0].h, PFB_BAR_H, 1e-6, "ten pixels tall");
    okEq((long)q[0].color, 0x11223344L, "in the colour it was given");

    /* Two units: the first segment whole, then a quarter of it -- and
       the colour flipped between them, which is what makes the units
       countable. */
    n = PFB_i_BarQuads(q, 8, 0x11223344u, 2.0f, 100.0f, 640.0f);
    okEq(n, 2, "two units are two segments");
    okNear(q[1].w, q[0].w * 0.25, 1e-3, "the second is a quarter of it");
    okNear(q[1].x, q[0].x + q[0].w, 1e-3, "and starts where it ended");
    okEq((long)q[1].color, (long)(0x11223344u ^ 0xFFFFu),
         "with the colour flipped between them");

    /* The clamp: four units is as long as a bar gets, so a raster that
       takes eight times the target still fits on the page. */
    {
        int four = PFB_i_BarQuads(q, 8, 0u, 4.0f, 0.0f, 640.0f);
        float end = q[four - 1].x + q[four - 1].w;
        int   lots = PFB_i_BarQuads(q, 8, 0u, 40.0f, 0.0f, 640.0f);
        float lotsEnd = q[lots - 1].x + q[lots - 1].w;

        okEq(lots, four, "forty units draw as four");
        okNear(lotsEnd, end, 1e-3, "and end in the same place");
        ok(end < 640.0f - 50.0f,
           "which is inside the room the number on the right needs");
    }

    /* Nothing at all draws nothing, rather than a zero-width quad for
       a renderer to work out what to do with. */
    okEq(PFB_i_BarQuads(q, 8, 0u, 0.0f, 0.0f, 640.0f), 0,
         "a raster at nought has no bar");

    /* And the room asked for is enough: eight is what both renderers
       pass, and a clamped bar cannot need more than that. */
    {
        int tight = PFB_i_BarQuads(q, 4, 0u, 4.0f, 0.0f, 640.0f);

        ok(tight <= 4, "a small array is not overrun");
    }
}

int main(void)
{
    caseFps();
    caseGates();
    caseSmoothing();
    caseOrder();
    caseFitting();
    caseRows();
    caseRowColour();
    caseBar();

    printf("  %d checks, %d failure(s)\n", checks, failures);
    return failures ? 1 : 0;
}
