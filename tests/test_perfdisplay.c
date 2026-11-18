/* A unit test for platform/perfdisplay.c -- the page `tweak perf` puts over
 * the frame.
 *
 * The display is new code on the OpenGL side and has no frozen copy to
 * compare against, so this is a plain check of what it draws. What
 * perfbars.c decides has its own harness; what is read here is the
 * things only the drawer can get wrong:
 *
 *   - the gate: nothing at all with the tweak off, and nothing drawn
 *     while the renderer is not ready;
 *   - **the profiler's bookkeeping**, which is not drawing at all and
 *     is the half that was missing. The sums are a frame's worth only
 *     because the display clears them once it has shown them: nothing
 *     else in the engine does, so a renderer drawing the page without
 *     this would show bars that only ever grew. The clear has to
 *     happen whether or not anything was drawn, which is the case
 *     mode 3 gets wrong if the clear is put inside the drawing;
 *   - the page: the band at the top, the backdrop sized to the rows,
 *     one row per raster with its name and its number, and the frame
 *     rate's own row;
 *   - that the strings go out with the quads rather than instead of
 *     them.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "../gdi.h"
#include "../font.h"
#include "../profiler.h"

/* --------------------------------------------------------- the engine */

static int pdt_ready = 1;
static int pdt_beginCalls;
static int pdt_endCalls;
static int pdt_quadCalls;
static int pdt_drawWidth = 640;
static int pdt_drawHeight = 480;

static int  PDT_b_Ready(void) { return pdt_ready; }
static void PDT_OverlayBegin(void) { pdt_beginCalls++; }
static void PDT_OverlayEnd(void) { pdt_endCalls++; }
static void PDT_GetDrawingSize(int* w, int* h)
{
    if (w) *w = pdt_drawWidth;
    if (h) *h = pdt_drawHeight;
}

/* The batch is emptied once it is handed over, so the mock keeps the
   quads for the cases to measure: one entry a quad, taken from the
   six vertices it is made of. */
#define PDT_MAXQ 2048
typedef struct { float x, y, w, h; unsigned int color; } tPdtQuad;
static tPdtQuad pdt_q[PDT_MAXQ];
static int      pdt_qCount;

static void PDT_OverlayDrawQuads(const void* verts, int triangleCount)
{
    const tGdi_Vertex* v = (const tGdi_Vertex*)verts;
    int quads = triangleCount / 2;
    int i;

    pdt_quadCalls++;
    for (i = 0; i < quads && pdt_qCount < PDT_MAXQ; i++)
    {
        const tGdi_Vertex* q = &v[i * 6];

        pdt_q[pdt_qCount].x = q[0].pos.x;
        pdt_q[pdt_qCount].y = q[0].pos.y;
        pdt_q[pdt_qCount].w = q[1].pos.x - q[0].pos.x;
        pdt_q[pdt_qCount].h = q[2].pos.y - q[0].pos.y;
        pdt_q[pdt_qCount].color = q[0].color;
        pdt_qCount++;
    }
}

static tGdi_DisplayData pdt_dd;
tGdi_DisplayData* gpst_DX_stDD;

/* The font, mocked: what matters is which strings went out and where,
   not how wide they are. */
#define PDT_MAXTEXT 256
static char pdt_text[PDT_MAXTEXT][64];
static int  pdt_textY[PDT_MAXTEXT];
static int  pdt_textX[PDT_MAXTEXT];
static int  pdt_textCount;

void FNT_DrawText(int face, const char* text, int x, int y,
                  uint32_t color)
{
    (void)face; (void)color;
    if (pdt_textCount < PDT_MAXTEXT)
    {
        strncpy(pdt_text[pdt_textCount], text ? text : "", 63);
        pdt_text[pdt_textCount][63] = 0;
        pdt_textX[pdt_textCount] = x;
        pdt_textY[pdt_textCount] = y;
        pdt_textCount++;
    }
}

int  FNT_i_Height(int face) { (void)face; return 13; }
int  FNT_i_TextWidth(int face, const char* s)
{
    (void)face;
    return s ? (int)strlen(s) * 8 : 0;
}

/* The profiler, mocked down to the four calls the display makes and
   the two arrays it reads. The counter advances by a fixed step per
   read so a case can state a frame time. */
/* The three tweaks that drive the display. Their home is dbg.c, which
   a harness for one display has no reason to link. */
int   tweak_perf;
float tweak_perf_fps_min = 20.0f;
float tweak_perf_fps_target = 60.0f;

/* The names table the display reads. Its home is profiler.h and its
   definition is beside the Direct3D display, so a harness that does
   not link that renderer defines its own. */
tRasterNames rasterNames[54];

static int64_t pdt_qpc;
static int64_t pdt_qpcStep = 1000;
static int     pdt_recorded[64];
static int     pdt_started[64];
static int     pdt_resets;
static int     pdt_order[8];
static int     pdt_orderCount;

void PRO_QPC_RecordTimeDecRef(int id)
{
    if (id >= 0 && id < 64)
        pdt_recorded[id]++;
    if (pdt_orderCount < 8)
        pdt_order[pdt_orderCount++] = 100 + id;
}

void PRO_QPC_StartTime(int id)
{
    if (id >= 0 && id < 64)
        pdt_started[id]++;
    if (pdt_orderCount < 8)
        pdt_order[pdt_orderCount++] = 200 + id;
}

void PRO_QPC_ResetSums(void)
{
    pdt_resets++;
    memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
}

int64_t PRO_i64_QPC(void)
{
    int64_t now = pdt_qpc;

    pdt_qpc += pdt_qpcStep;
    return now;
}

int64_t PRO_i64_QPCFrequency(void) { return 1000000; }

#include "../platform/perfdisplay.c"

/* --------------------------------------------------------- the harness */

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

static void reset(void)
{
    int i;

    pdt_beginCalls = 0;
    pdt_endCalls = 0;
    pdt_quadCalls = 0;
    pdt_qCount = 0;
    pdt_textCount = 0;
    pdt_resets = 0;
    pdt_orderCount = 0;
    pdt_ready = 1;
    memset(pdt_recorded, 0, sizeof(pdt_recorded));
    memset(pdt_started, 0, sizeof(pdt_started));

    pdt_dd.b_Ready = PDT_b_Ready;
    pdt_dd.OverlayBegin = PDT_OverlayBegin;
    pdt_dd.OverlayEnd = PDT_OverlayEnd;
    pdt_dd.OverlayDrawQuads = PDT_OverlayDrawQuads;
    pdt_dd.GetDrawingSize = PDT_GetDrawingSize;
    gpst_DX_stDD = &pdt_dd;

    /* A frame already behind us: the counter is a frame's worth ahead
       of the display's last reading, so the first draw has a rate to
       show. A fixture starting both at nought measures no frame at
       all, which is the real first frame and has a case of its own
       below. */
    pdt_qpc = 16667;
    pdt_qpcStep = 16667;

    tweak_perf = 0;
    tweak_perf_fps_min = 20.0f;
    tweak_perf_fps_target = 60.0f;

    /* Three rasters with something in them, the rest idle. */
    memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
    for (i = 0; i < PFB_RASTERS; i++)
    {
        rasterNames[i].unk1 = 2;
        sprintf(rasterNames[i].name, "R%02d", i);
    }
    /* The busiest raster is deliberately the one with the **highest**
       index: with it at the lowest, the sorted page and the page in
       declaration order are the same, and a display that never sorted
       at all would pass every case here. */
    PerfCountertimeSumArray[40] = 16667;     /* about a frame's worth */
    PerfCountertimeSumArray[7] = 8000;
    PerfCountertimeSumArray[9] = 4000;
    memset(pfd_avg, 0, sizeof(pfd_avg));
    pfd_lastTick = 0;
}

/* How many quads of a given colour came out. */
static int pdt_i_OfColour(unsigned int color)
{
    int i;
    int n = 0;

    for (i = 0; i < pdt_qCount; i++)
        if (pdt_q[i].color == color)
            n++;
    return n;
}

static int pdt_b_Text(const char* s)
{
    int i;

    for (i = 0; i < pdt_textCount; i++)
        if (strcmp(pdt_text[i], s) == 0)
            return 1;
    return 0;
}

static void caseGate(void)
{
    printf("-- nothing at all with the tweak off\n");

    reset();
    PFD_Draw();
    okEq(pdt_beginCalls, 0, "the renderer is not asked to begin");
    okEq(pdt_qCount, 0, "nothing is drawn");
    okEq(pdt_textCount, 0, "nothing is written");
    okEq(pdt_resets, 0, "and the profiler's sums are left alone");
    okEq(pdt_recorded[53] + pdt_started[53], 0,
         "the display does not even time itself");

    printf("-- nor while the renderer is not ready\n");
    reset();
    tweak_perf = 1;
    pdt_ready = 0;
    PFD_Draw();
    okEq(pdt_beginCalls, 0, "nothing is drawn");
    okEq(pdt_resets, 0, "and nothing is cleared");
}

static void caseBookkeeping(void)
{
    printf("-- the profiler's sums are a frame's worth\n");

    reset();
    tweak_perf = 1;
    PFD_Draw();
    okEq(pdt_resets, 1, "the sums are cleared once the page has shown"
                        " them");
    okEq((long)PerfCountertimeSumArray[40], 0, "so the next frame starts"
                                               " from nothing");

    /* The display's own cost: two rasters closed on the way in and
       reopened on the way out, and a third bracketing the page. */
    okEq(pdt_recorded[53], 1, "the display closes its own raster");
    okEq(pdt_recorded[19], 1, "and the one beside it");
    okEq(pdt_started[47], 1, "it opens the one that times the page");
    okEq(pdt_recorded[47], 1, "and closes it again");
    okEq(pdt_started[19], 1, "then reopens the two it closed");
    okEq(pdt_started[53], 1, "both of them");

    /* The order matters: closing 47 before the page is drawn would
       time nothing. */
    ok(pdt_orderCount >= 4, "the four calls are in the log");
    okEq(pdt_order[0], 100 + 53, "53 is closed first");
    okEq(pdt_order[1], 100 + 19, "then 19");
    okEq(pdt_order[2], 200 + 47, "then 47 is opened");
    okEq(pdt_order[3], 100 + 47, "and closed after the page");

    /* **Mode 3 below the floor draws nothing and must still clear.**
       Putting the clear inside the drawing is the mistake this case
       exists for: the sums would grow without bound in exactly the
       mode meant for watching a slow frame. */
    printf("-- and cleared even on a frame that drew nothing\n");
    reset();
    tweak_perf = 3;
    tweak_perf_fps_min = 2000.0f;      /* above the mocked frame rate */
    PFD_Draw();
    okEq(pdt_beginCalls, 0, "mode three below the floor draws nothing");
    okEq(pdt_resets, 1, "and still clears the sums");
    okEq((long)PerfCountertimeSumArray[40], 0, "so they do not pile up");
}

static void casePage(void)
{
    printf("-- the page: a band, a backdrop, the rows and the rate\n");

    reset();
    tweak_perf = 1;
    PFD_Draw();

    okEq(pdt_beginCalls, 1, "the renderer is asked to begin once");
    okEq(pdt_endCalls, 1, "and to end once");
    okEq(pdt_quadCalls, 1, "the whole page is one batch");

    /* The band across the top, in the red it has always been. */
    okEq(pdt_i_OfColour(0xFFFF0000u), 1, "the band is drawn");
    {
        int i;
        int found = -1;

        for (i = 0; i < pdt_qCount; i++)
            if (pdt_q[i].color == 0xFFFF0000u)
                found = i;
        if (found >= 0)
        {
            okNear(pdt_q[found].y, 0.0, 1e-6, "at the top of the frame");
            okNear(pdt_q[found].w, (double)pdt_drawWidth, 1e-6,
                   "across the whole width");
            okNear(pdt_q[found].h, 40.0, 1e-6, "forty pixels tall");
        }
    }

    /* The backdrop behind the rows, translucent black, and as tall as
       the rows it is behind -- a fixed height would cover the wrong
       part of the frame as the page grew. */
    okEq(pdt_i_OfColour(0x80000000u), 1, "the backdrop is drawn");
    {
        int i;
        int found = -1;
        int rows = 0;

        for (i = 0; i < pdt_qCount; i++)
            if (pdt_q[i].color == 0x80000000u)
                found = i;
        rows = pdt_textCount / 2;
        if (found >= 0)
        {
            okNear(pdt_q[found].y, 50.0, 1e-6, "from the frame-rate row");
            okNear(pdt_q[found].w, (double)pdt_drawWidth, 1e-6,
                   "across the whole width");
            /* **Fifty-four rows tall, on a screen that holds twenty.**
               The height is the row count the fitting walk answered,
               and that walk answers the raster count whenever the page
               fits -- not the number of rows with anything in them. So
               the backdrop runs off the bottom of the screen, which is
               what the other renderer's display does and why this is an
               equality rather than a lower bound. */
            okNear(pdt_q[found].h, 54.0 * 15.0, 1e-6,
                   "and as tall as the row count the walk answered");
            ok(pdt_q[found].h > (double)pdt_drawHeight,
               "which is taller than the screen, as it has always been");
            ok(rows < 54, "while far fewer rows are really drawn");
        }
    }

    /* The three rasters with something in them are named, and so is
       the frame rate. */
    ok(pdt_b_Text("R40"), "the busiest raster is named");
    ok(pdt_b_Text("R07"), "and the next");
    ok(pdt_b_Text("Frame Rate"), "and the frame rate has its own row");

    /* Each row writes two strings -- the name and the number -- so a
       row that lost its value would show as an odd count. */
    okEq(pdt_textCount % 2, 0, "every row wrote a name and a number");

    /* The rows are in order down the screen, the frame rate above
       them. */
    {
        int i;
        int rateY = -1;
        int firstRowY = 10000;

        for (i = 0; i < pdt_textCount; i++)
        {
            if (strcmp(pdt_text[i], "Frame Rate") == 0)
                rateY = pdt_textY[i];
            else if (pdt_text[i][0] == 'R' && pdt_textY[i] < firstRowY)
                firstRowY = pdt_textY[i];
        }
        okEq(rateY, 50, "the frame rate sits at fifty");
        okEq(firstRowY, 70, "and the first raster row below it");
    }

    /* And the bars themselves: more quads than the band and the
       backdrop, which is what says the rows were drawn and not just
       labelled. */
    ok(pdt_qCount > 2, "the rows have bars as well as names");

    /* The name on the left and the number on the right, which is the
       difference between a readable row and two numbers in a heap. */
    {
        int i;
        int nameX = -1;
        int numberX = -1;

        for (i = 0; i + 1 < pdt_textCount; i++)
            if (strcmp(pdt_text[i], "R40") == 0)
            {
                nameX = pdt_textX[i];
                numberX = pdt_textX[i + 1];
                break;
            }
        okEq(nameX, 20, "the name is at the left margin");
        okEq(numberX, pdt_drawWidth - 50,
             "and its number against the right edge");
    }

    /* The busiest raster is at the top. That is what the running
       average and the order are for, and without them the rows come
       out in whatever order the rasters are declared in. */
    {
        int i;
        int yBusiest = -1;
        int yNext = -1;
        int yThird = -1;

        for (i = 0; i < pdt_textCount; i++)
        {
            if (strcmp(pdt_text[i], "R40") == 0)
                yBusiest = pdt_textY[i];
            else if (strcmp(pdt_text[i], "R07") == 0)
                yNext = pdt_textY[i];
            else if (strcmp(pdt_text[i], "R09") == 0)
                yThird = pdt_textY[i];
        }
        ok(yBusiest >= 0 && yNext > yBusiest,
           "the busiest raster's row is above the next");
        ok(yThird > yNext, "and that one above the third");
    }

    /* A raster with no kind is not a row at all, however much time it
       has in it. */
    {
        reset();
        tweak_perf = 1;
        rasterNames[40].unk1 = 0;
        PFD_Draw();
        okEq(pdt_b_Text("R40"), 0, "a raster with no kind is not drawn");
        ok(pdt_b_Text("R07"), "while the one beside it still is");
    }
}

static void caseModes(void)
{
    int oneRow;
    int manyRows;

    printf("-- mode one shows the page, mode two shows one row\n");

    reset();
    tweak_perf = 1;
    PFD_Draw();
    manyRows = pdt_textCount;

    reset();
    tweak_perf = 2;
    PFD_Draw();
    oneRow = pdt_textCount;

    ok(manyRows > oneRow, "mode one writes more rows than mode two");
    okEq(pdt_i_OfColour(0xFFFF0000u), 1, "mode two still draws the band");
    ok(pdt_b_Text("Frame Rate"), "and still shows the frame rate");

    printf("-- and mode three shows the band alone\n");
    reset();
    tweak_perf = 3;
    PFD_Draw();
    okEq(pdt_i_OfColour(0xFFFF0000u), 1, "the band is there");
    okEq(pdt_i_OfColour(0x80000000u), 0, "the backdrop is not");
    okEq(pdt_textCount, 0, "and nothing is written at all");
}

static void caseFirstFrame(void)
{
    printf("-- the first frame has no rate, so no band\n");

    /* Nothing has been measured yet, so the rate is nought and the
       band -- which means the rate is *up* -- stays off. The other
       renderer's display does the same from the same cause: its own
       last reading starts at nought too. */
    reset();
    tweak_perf = 1;
    pdt_qpc = 0;
    pfd_lastTick = 0;
    PFD_Draw();
    okEq(pdt_i_OfColour(0xFFFF0000u), 0, "no band on the first frame");
    ok(pdt_beginCalls == 1, "but the page is drawn");

    /* And the frame after it has one. */
    pdt_qCount = 0;
    pdt_textCount = 0;
    PFD_Draw();
    okEq(pdt_i_OfColour(0xFFFF0000u), 1, "the next frame has the band");
}

static void caseFrameRate(void)
{
    printf("-- the rate shown is the frame's own\n");

    /* A frame of 16667 ticks at a million a second is sixty a second.
       The display measures between its own calls, so the second draw
       is the one with a frame behind it. */
    reset();
    tweak_perf = 1;
    pdt_qpcStep = 16667;
    PFD_Draw();
    /* The clock is left where it is, so the second draw has a frame
       behind it; only what the mocks recorded is cleared. */
    pdt_textCount = 0;
    pdt_qCount = 0;
    PFD_Draw();
    {
        int i;
        int found = 0;

        for (i = 0; i + 1 < pdt_textCount; i++)
            if (strcmp(pdt_text[i], "Frame Rate") == 0)
            {
                double shown = atof(pdt_text[i + 1]);

                /* The row's number is the rate itself, clamped by the
                   bar but printed whole. */
                okNear(shown, 60.0, 0.5, "sixty a second is read back");
                found = 1;
            }
        ok(found, "the frame rate's row carries a number");
    }
}

int main(void)
{
    caseGate();
    caseBookkeeping();
    caseFirstFrame();
    casePage();
    caseModes();
    caseFrameRate();

    printf("  %d checks, %d failure(s)\n", checks, failures);
    return failures ? 1 : 0;
}
