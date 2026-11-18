/* A unit test for win/overlay.c.
 *
 * The overlay is new code, so there is no frozen copy to compare it
 * against and this is a plain check of what it does rather than a
 * differential one. It includes overlay.c so the quad batch and the
 * text queue, which are file-local, can be read back after a draw.
 *
 * What is checked:
 *   - the command field's state machine: what each key does, what the
 *     message hook consumes, and that the field cannot be overrun;
 *   - the shapes: every quad inside the box it was asked for, the
 *     centre covered, the outline leaving a hole, and the batch never
 *     written past its end;
 *   - the panel: the five value forms a raster can carry, and that
 *     reading a timing raster clears its accumulator;
 *   - the pass: nothing drawn when neither widget is up, and the
 *     strings issued after the quads rather than between them.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

/* ------------------------------------------------------------- the engine */

/* overlay.c talks to the renderer interface and to nothing else now,
   so this stands in for the interface rather than for win/wingdi.c.
   It used to include that header and define D3DRenderParams, which is
   what the overlay read its viewport out of. */
#include "../gdi.h"
#include "../font.h"

static int ovt_beginCalls;
static int ovt_endCalls;
static int ovt_quadCalls;
static int ovt_trianglesDrawn;
static int ovt_textAfterQuads;   /* strings issued before any quad flush */
static int ovt_ready = 1;
static int ovt_drawWidth = 640;
static int ovt_drawHeight = 480;
static int ovt_sizeCalls;

static int  OVT_b_Ready(void) { return ovt_ready; }
static void OVT_OverlayBegin(void) { ovt_beginCalls++; }
static void OVT_OverlayEnd(void) { ovt_endCalls++; }

static void OVT_GetDrawingSize(int* w, int* h)
{
    ovt_sizeCalls++;
    if (w)
        *w = ovt_drawWidth;
    if (h)
        *h = ovt_drawHeight;
}

/* OVL_Draw empties the batch once it has handed it over, so the mock
   keeps a copy for the placement checks to measure. */
#define OVT_MAXFLUSHED 4096
static float ovt_flushedX[OVT_MAXFLUSHED];
static int   ovt_flushedCount;

static void OVT_OverlayDrawQuads(const void* verts, int triangleCount)
{
    const tGdi_Vertex* v = (const tGdi_Vertex*)verts;
    int n = triangleCount * 3;
    int i;

    ovt_quadCalls++;
    ovt_trianglesDrawn += triangleCount;
    for (i = 0; i < n && ovt_flushedCount < OVT_MAXFLUSHED; i++)
        ovt_flushedX[ovt_flushedCount++] = v[i].pos.x;
}

static tGdi_DisplayData ovt_dd;

#define OVT_MAXSEEN 512
static char ovt_seen[OVT_MAXSEEN][96];
static int  ovt_seenX[OVT_MAXSEEN];
static int  ovt_seenY[OVT_MAXSEEN];

/* The renderer the mock claims to be, which the stamp has to print
   rather than deciding for itself which renderer is drawing. */
static const char* OVT_sz_Name(void)
{
    return "Mock renderer";
}
static int  ovt_seenBig[OVT_MAXSEEN];
static int  ovt_seenCount;

static void ovt_record(const char* text, int x, int y, int big)
{
    if (ovt_quadCalls == 0)
        ovt_textAfterQuads = 0;
    if (ovt_seenCount < OVT_MAXSEEN)
    {
        strncpy(ovt_seen[ovt_seenCount], text, 95);
        ovt_seen[ovt_seenCount][95] = 0;
        ovt_seenX[ovt_seenCount] = x;
        ovt_seenY[ovt_seenCount] = y;
        ovt_seenBig[ovt_seenCount] = big;
        ovt_seenCount++;
    }
}

/* The overlay's own font. It is deliberately **proportional** here,
   with four different glyph widths, so a caret placed by a
   per-character estimate cannot pass the checks below: that is the
   defect this stand-in exists to expose. */
static int ovt_fontHeight = 20;

static int ovt_lastHeightFace = -1;
static int ovt_lastWidthFace = -1;

static int ovt_glyphWidth(char c)
{
    if (c == 'i' || c == 'l' || c == '.')
        return 4;
    if (c == 'm' || c == 'w' || c == 'W')
        return 17;
    if (c == ' ')
        return 6;
    return 10;
}

/* The font module, mocked. The overlay asks it for a height and a
   width and hands it strings to draw; the glyph widths above stand in
   for a real face, so a layout case can state what it expects without
   depending on what Liberation measures. The face the overlay chose
   is what ovt_record's last argument carries. */
/* The first face asked for since the counters were cleared, not the
   last: a case that measures a string itself, to work out what it
   expected, would otherwise overwrite what the overlay asked for and
   the check would pass whatever the overlay did. */
int FNT_i_Height(int face)
{
    if (ovt_lastHeightFace < 0)
        ovt_lastHeightFace = face;
    return ovt_fontHeight;
}

int FNT_i_TextWidth(int face, const char* text)
{
    int w = 0;

    if (ovt_lastWidthFace < 0)
        ovt_lastWidthFace = face;
    while (*text)
        w += ovt_glyphWidth(*text++);
    return w;
}

void FNT_DrawText(int face, const char* text, int x, int y, uint32_t color)
{
    (void)color;
    ovt_record((char*)text, x, y, face == FNT_OVERLAY ? 1 : 0);
}

/* the profiler's own state, which the panel walks */
int PRO_rasterWindowIsOn;
int PRO_rasterDetailPage;
/* The tweak that puts the after-effects list up, and the two
   parameter-driven effects the list reads straight. */
int tweak_ae_view;
int PC_AE_MASTER;
float brightnessParam = 0.5f;
float colorBalanceParam1 = 0.5f;

/* The engine's value getter, as much of it as the list asks for: the
   ids the list carries, and nought for anything else, which is what
   the engine's own default arm gives. The numbers are the harness's,
   so a row printing the wrong effect's value prints a number that
   belongs to another row. */
float PC_AE_MASTER_GET_P1(int effect, int slot)
{
    (void)slot;
    switch (effect)
    {
    case 0:  return 0.125f;      /* depth blur */
    case 1:  return 0.250f;      /* motion blur */
    case 2:  return 0.375f;      /* screen blur */
    case 5:  return 0.500f;      /* zoom smooth */
    case 6:  return 0.625f;      /* zoom smooth 2 */
    case 7:  return 0.750f;      /* remanence */
    case 8:  return brightnessParam;
    case 9:  return colorBalanceParam1;
    case 11: return 0.875f;      /* grayscale */
    case 12: return 0.937f;      /* hue */
    case 14: return 1.125f;      /* depth blur near */
    default: return 0.0f;
    }
}
profilerRasterStructElem* PRO_FirstRasterPtr;
profilerRasterStructElem* pointsToUnkRaster_dword_98F944;
profilerRasterStructElem* PRO_CurrRaster;
float gfDBGRasterSetTo60 = 60.0f;

int _stricmp(const char* a, const char* b)
{
    while (*a && *b)
    {
        int ca = (*a >= 65 && *a <= 90) ? *a + 32 : *a;
        int cb = (*b >= 65 && *b <= 90) ? *b + 32 : *b;
        if (ca != cb)
            return ca - cb;
        a++; b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

/* what the command field hands on when Enter is pressed */
static char ovt_ranCommand[256];
static int  ovt_runCalls;
static int  ovt_acquireCalls;

void pcINO_RunTweakCommand(const char* line)
{
    ovt_runCalls++;
    strncpy(ovt_ranCommand, line ? line : "", sizeof(ovt_ranCommand) - 1);
    ovt_ranCommand[sizeof(ovt_ranCommand) - 1] = 0;
}

void pcINO_dinput_mouse_and_keyb_acquire(void) { ovt_acquireCalls++; }

#include "../win/overlay.c"

/* ---------------------------------------------------------------- scoring */

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

static void resetAll(void)
{
    ovt_beginCalls = 0;
    ovt_endCalls = 0;
    ovt_quadCalls = 0;
    ovt_trianglesDrawn = 0;
    ovt_flushedCount = 0;
    ovt_textAfterQuads = 1;
    ovt_seenCount = 0;
    ovt_runCalls = 0;
    ovt_acquireCalls = 0;
    ovt_ranCommand[0] = 0;
    ovl_vertCount = 0;
    ovl_textCount = 0;
    ovl_cmdActive = 0;
    ovl_cmdLen = 0;
    ovl_cmdText[0] = 0;
    ovl_caretTick = 0;
    PRO_rasterWindowIsOn = 0;
    PRO_rasterDetailPage = 0;
    PRO_FirstRasterPtr = NULL;
    pointsToUnkRaster_dword_98F944 = NULL;
    PRO_CurrRaster = NULL;
    ovt_ready = 1;
    ovt_drawWidth = 640;
    ovt_drawHeight = 480;
    ovt_sizeCalls = 0;

    memset(&ovt_dd, 0, sizeof(ovt_dd));
    ovt_dd.b_Ready = OVT_b_Ready;
    ovt_dd.sz_Name = OVT_sz_Name;
    ovt_dd.GetDrawingSize = OVT_GetDrawingSize;
    ovt_dd.OverlayBegin = OVT_OverlayBegin;
    ovt_dd.OverlayDrawQuads = OVT_OverlayDrawQuads;
    ovt_dd.OverlayEnd = OVT_OverlayEnd;
    gpst_DX_stDD = &ovt_dd;
}

static void type(const char* s)
{
    while (*s)
        OVL_b_CommandChar((unsigned char)*s++);
}

/* --------------------------------------------------------- command field */

static void fieldStateMachine(void)
{
    int i;

    /* nothing is consumed while the field is down */
    resetAll();
    okEq(OVL_b_CommandActive(), 0, "field starts down");
    okEq(OVL_b_CommandChar('a'), 0, "a character is ignored while down");

    /* typing */
    OVL_CommandShow();
    okEq(OVL_b_CommandActive(), 1, "show raises the field");
    okEq(OVL_b_CommandChar('h'), 1, "and consumed while up");
    type("ello world");
    ok(strcmp(ovl_cmdText, "hello world") == 0, "printable characters accumulate");

    /* Nothing else sees the keys while the field has the keyboard, and
       that is not this function's doing: raising the field releases
       the input devices and putting it away takes them back, which is
       the two calls either side of the line being run. There are no
       window messages to swallow any more -- the window is SDL's and
       its events go only where the platform layer routes them. */

    /* backspace, and backspace on an empty line */
    OVL_b_CommandChar('\b');
    ok(strcmp(ovl_cmdText, "hello worl") == 0, "backspace drops a character");
    OVL_CommandShow();
    okEq(OVL_b_CommandChar('\b'), 1, "backspace on empty consumed");
    okEq(ovl_cmdLen, 0, "backspace on empty leaves the line empty");

    /* control characters outside the printable range are dropped */
    OVL_b_CommandChar(7);
    OVL_b_CommandChar(127);
    OVL_b_CommandChar(200);
    okEq(ovl_cmdLen, 0, "non-printables are not appended");

    /* Enter runs the line, puts the field away and takes the devices back */
    type("fps 60");
    OVL_b_CommandChar('\r');
    okEq(ovt_runCalls, 1, "enter runs the line once");
    ok(strcmp(ovt_ranCommand, "fps 60") == 0, "enter hands on what was typed");
    okEq(OVL_b_CommandActive(), 0, "enter puts the field away");
    okEq(ovt_acquireCalls, 1, "enter re-acquires the devices");

    /* escape abandons without running */
    resetAll();
    OVL_CommandShow();
    type("quit");
    OVL_b_CommandChar(27);
    okEq(ovt_runCalls, 0, "escape runs nothing");
    okEq(OVL_b_CommandActive(), 0, "escape puts the field away");
    okEq(ovt_acquireCalls, 1, "escape re-acquires the devices");

    /* show clears whatever was left behind */
    resetAll();
    OVL_CommandShow();
    type("stale");
    OVL_CommandHide();
    OVL_CommandShow();
    okEq(ovl_cmdLen, 0, "show starts from an empty line");

    /* the line cannot be overrun */
    resetAll();
    OVL_CommandShow();
    for (i = 0; i < OVL_CMDMAX * 3; i++)
        OVL_b_CommandChar('x');
    okEq(ovl_cmdLen, OVL_CMDMAX - 1, "the line stops one short of the buffer");
    okEq(ovl_cmdText[OVL_CMDMAX - 1], 0, "the line stays terminated");
    OVL_b_CommandChar('\r');
    okEq((long)strlen(ovt_ranCommand), OVL_CMDMAX - 1, "a full line is handed on whole");

    /* an empty line still goes through, and the field still closes */
    resetAll();
    OVL_CommandShow();
    OVL_b_CommandChar('\n');
    okEq(ovt_runCalls, 1, "a bare newline is still a submit");
    okEq(OVL_b_CommandActive(), 0, "a bare newline closes the field");
}

/* ----------------------------------------------------------------- shapes */

/* every vertex of the batch, against the box it should sit in */
static void boxCheck(float x, float y, float w, float h, const char* what)
{
    int i;
    int outside = 0;

    for (i = 0; i < ovl_vertCount; i++)
    {
        if (ovl_verts[i].pos.x < x - 0.01f || ovl_verts[i].pos.x > x + w + 0.01f
            || ovl_verts[i].pos.y < y - 0.01f || ovl_verts[i].pos.y > y + h + 0.01f)
        {
            outside++;
        }
    }
    okEq(outside, 0, what);
}

/* is the point inside any quad of the batch, and in which colour */
static int colorAt(float px, float py, unsigned int* out)
{
    int i;
    int found = 0;

    for (i = 0; i + 5 < ovl_vertCount; i += 6)
    {
        float left = ovl_verts[i].pos.x;
        float top = ovl_verts[i].pos.y;
        float right = ovl_verts[i + 5].pos.x;
        float bottom = ovl_verts[i + 5].pos.y;

        if (px >= left && px <= right && py >= top && py <= bottom)
        {
            *out = (unsigned int)ovl_verts[i].color;
            found = 1;        /* later quads paint over earlier ones */
        }
    }
    return found;
}

static void shapes(void)
{
    unsigned int c;
    int i;

    /* a plain quad */
    resetAll();
    OVL_l_Quad(0xFF112233u, 10.0f, 20.0f, 30.0f, 40.0f);
    okEq(ovl_vertCount, 6, "a quad is six vertices");
    boxCheck(10.0f, 20.0f, 30.0f, 40.0f, "the quad stays in its box");
    ok(colorAt(25.0f, 40.0f, &c) && c == 0xFF112233u, "the quad carries its colour");
    for (i = 0; i < 6; i++)
        okEq((long)ovl_verts[i].pos.z, 0, "the quad is flat");

    /* a degenerate quad is not emitted at all */
    resetAll();
    OVL_l_Quad(0xFFFFFFFFu, 0.0f, 0.0f, 0.0f, 10.0f);
    OVL_l_Quad(0xFFFFFFFFu, 0.0f, 0.0f, 10.0f, -1.0f);
    okEq(ovl_vertCount, 0, "a zero or negative quad is dropped");

    /* a rounded fill: inside its box, covering the centre, corners clipped */
    resetAll();
    OVL_l_RoundRectFill(0xFF00FF00u, 100.0f, 100.0f, 200.0f, 80.0f, 12.0f);
    ok(ovl_vertCount > 6, "the rounded fill is more than one quad");
    boxCheck(100.0f, 100.0f, 200.0f, 80.0f, "the rounded fill stays in its box");
    ok(colorAt(200.0f, 140.0f, &c), "the rounded fill covers its centre");
    ok(colorAt(101.0f, 140.0f, &c), "the rounded fill reaches its left edge");
    ok(colorAt(200.0f, 101.0f, &c), "the rounded fill reaches its top edge");
    ok(!colorAt(100.5f, 100.5f, &c), "the rounded fill clips its corner");
    ok(!colorAt(299.5f, 179.5f, &c), "the rounded fill clips the far corner");

    /* a radius larger than the box is clamped rather than inverting it */
    resetAll();
    OVL_l_RoundRectFill(0xFF00FF00u, 0.0f, 0.0f, 20.0f, 10.0f, 100.0f);
    boxCheck(0.0f, 0.0f, 20.0f, 10.0f, "an oversized radius is clamped");
    ok(colorAt(10.0f, 5.0f, &c), "an oversized radius still covers the centre");

    /* a radius under a pixel degenerates to one quad */
    resetAll();
    OVL_l_RoundRectFill(0xFF00FF00u, 0.0f, 0.0f, 20.0f, 10.0f, 0.4f);
    okEq(ovl_vertCount, 6, "a sub-pixel radius is a plain quad");

    /* the outline: border colour at the edge, inner colour in the middle */
    resetAll();
    OVL_l_RoundRectOutline(0xFF00FF00u, 0xFF000000u, 50.0f, 50.0f,
                           300.0f, 40.0f, 8.0f, 2.0f);
    boxCheck(50.0f, 50.0f, 300.0f, 40.0f, "the outline stays in its box");
    ok(colorAt(200.0f, 50.5f, &c) && c == 0xFF00FF00u, "the outline is the border colour");
    ok(colorAt(200.0f, 70.0f, &c) && c == 0xFF000000u, "the outline leaves a hole");
    ok(colorAt(51.0f, 70.0f, &c) && c == 0xFF00FF00u, "the left border is drawn");

    /* The batch cannot be written past its end. This fills it exactly
       and then asks for one more, rather than driving it far past the
       end: a missing guard writes over the counter that follows the
       array, and after a few hundred of those the counter can land
       back on a passing value by chance. One step over is decisive. */
    resetAll();
    for (i = 0; i < OVL_MAXQUADS; i++)
        OVL_l_Quad(0xFFFFFFFFu, 0.0f, 0.0f, 1.0f, 1.0f);
    okEq(ovl_vertCount, OVL_MAXQUADS * 6, "the quad batch fills to its end");
    OVL_l_Quad(0xFFFFFFFFu, 0.0f, 0.0f, 1.0f, 1.0f);
    okEq(ovl_vertCount, OVL_MAXQUADS * 6, "the quad batch refuses one more");

    /* and neither can the text queue */
    resetAll();
    for (i = 0; i < OVL_MAXTEXT; i++)
        OVL_l_Text("x", 0, 0, 0);
    okEq(ovl_textCount, OVL_MAXTEXT, "the text queue fills to its end");
    OVL_l_Text("x", 0, 0, 0);
    okEq(ovl_textCount, OVL_MAXTEXT, "the text queue refuses one more");

    /* a string longer than the queue's rows is truncated, not overrun */
    resetAll();
    {
        char longer[OVL_TEXTLEN * 2];
        memset(longer, 'y', sizeof(longer) - 1);
        longer[sizeof(longer) - 1] = 0;
        OVL_l_Text(longer, 0, 0, 0);
        okEq((long)strlen(ovl_text[0].s), OVL_TEXTLEN - 1, "a long string is truncated");
    }
}

/* ------------------------------------------------------------- the panel */

static profilerRasterStructElem rasters[4];

static void makeRaster(profilerRasterStructElem* r, char* group, char* sub,
                       char* module, int kind, float value)
{
    memset(r, 0, sizeof(*r));
    r->group = group;
    r->subgroup = sub;
    r->module = module;
    r->unk1 = kind;
    r->unk2 = value;
    r->unk7 = 1;
}

static int sawText(const char* s)
{
    int i;
    for (i = 0; i < ovt_seenCount; i++)
        if (strcmp(ovt_seen[i], s) == 0)
            return 1;
    return 0;
}

static void panel(void)
{
    int intBits;

    /* the five forms a raster's value can take */
    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 1, 0.0f);
    intBits = 4242;
    memcpy(&rasters[0].unk2, &intBits, sizeof(intBits));
    OVL_l_RasterRow(0, &rasters[0], 1);
    OVL_l_FlushText();
    ok(sawText("4242"), "kind 1 prints the value as an int");
    ok(sawText("sub"), "kind 1 prints the subgroup heading");
    ok(sawText("mod"), "kind 1 prints the module");

    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 2, 0.0f);
    memcpy(&rasters[0].unk2, "hi", 3);
    OVL_l_RasterRow(0, &rasters[0], 0);
    OVL_l_FlushText();
    ok(sawText("hi"), "kind 2 prints the value as a string");

    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 3, 1.0f);
    okEq(OVL_l_RasterRow(0, &rasters[0], 0), OVL_ROWHEIGHT, "kind 3 still advances a row");
    OVL_l_FlushText();
    okEq(ovt_seenCount, 1, "kind 3 prints no value");

    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 4, 0.25f);
    OVL_l_RasterRow(0, &rasters[0], 0);
    OVL_l_FlushText();
    ok(sawText("0.25000"), "kind 4 prints the value as a float");

    /* the default form is a time, and reading it moves the accumulator
       into the carried value so the next frame starts from zero */
    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 0, 0.0f);
    rasters[0].sumTime = 1000;
    TIM_gul_TicksPerSecond = 1000;
    gfDBGRasterSetTo60 = 2.0f;
    OVL_l_RasterRow(0, &rasters[0], 0);
    OVL_l_FlushText();
    ok(sawText("2.000"), "the timing form scales by the raster factor");
    okEq(rasters[0].sumTime, 0, "reading a timing raster clears the accumulator");
    okEq(rasters[0].unk10, 1000, "reading a timing raster carries the ticks over");
    okEq(rasters[0].unk7, 0, "a drawn raster is marked");

    /* with nothing accumulated it reports the carried value again */
    resetAll();
    rasters[0].unk7 = 1;
    OVL_l_RasterRow(0, &rasters[0], 0);
    OVL_l_FlushText();
    ok(sawText("2.000"), "an idle timing raster repeats its last value");
    okEq(rasters[0].unk10, 1000, "an idle timing raster keeps its ticks");
    gfDBGRasterSetTo60 = 60.0f;

    /* a subgroup heading pushes the module onto the next line, but only
       when no single raster has been drilled into */
    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 3, 0.0f);
    okEq(OVL_l_RasterRow(0, &rasters[0], 1), 2 * OVL_ROWHEIGHT,
         "a subgroup heading takes a row of its own");
    resetAll();
    makeRaster(&rasters[0], "g", "sub", "mod", 3, 0.0f);
    PRO_CurrRaster = &rasters[0];
    okEq(OVL_l_RasterRow(0, &rasters[0], 1), OVL_ROWHEIGHT,
         "a drilled-into raster shares the heading's row");

    /* the summary page prints the two figures the window printed */
    resetAll();
    TIM_gf_dt = 0.5f;
    TIM_dtTicks = 0.25f;
    ok(OVL_l_RastersSummary() > 0, "the summary page reserves room");
    OVL_l_FlushText();
    ok(sawText("30.000000"), "the summary prints the scaled frame time");
    ok(sawText("15.000000"), "the summary prints the scaled tick delta");

    /* the detail walk over a group of three, two subgroups */
    resetAll();
    makeRaster(&rasters[0], "g", "a", "m0", 3, 0.0f);
    makeRaster(&rasters[1], "g", "a", "m1", 3, 0.0f);
    makeRaster(&rasters[2], "g", "b", "m2", 3, 0.0f);
    rasters[0].prev = &rasters[1];
    rasters[1].prev = &rasters[2];
    DBG_profiler_first_raster = &rasters[0];
    PRO_FirstRasterPtr = &rasters[0];
    ok(OVL_l_RastersDetail() > 24, "the detail page advances past its heading");
    OVL_l_FlushText();
    ok(sawText("m0") && sawText("m1") && sawText("m2"),
       "the detail page reaches every raster of the group");
    ok(sawText("a") && sawText("b"), "the detail page prints both subgroups");

    /* a group that does not match is skipped */
    resetAll();
    makeRaster(&rasters[0], "other", "a", "m0", 3, 0.0f);
    makeRaster(&rasters[1], "g", "a", "m1", 3, 0.0f);
    rasters[0].prev = &rasters[1];
    rasters[1].prev = NULL;
    DBG_profiler_first_raster = &rasters[0];
    PRO_FirstRasterPtr = &rasters[1];
    OVL_l_RastersDetail();
    OVL_l_FlushText();
    ok(!sawText("m0"), "a raster of another group is not drawn");
    ok(sawText("m1"), "a raster of the walked group is drawn");

    /* with no rasters at all it reports its heading row and nothing else */
    resetAll();
    DBG_profiler_first_raster = NULL;
    PRO_FirstRasterPtr = NULL;
    okEq(OVL_l_RastersDetail(), 24, "an empty detail page is just the heading");
    OVL_l_FlushText();
    okEq(ovt_seenCount, 0, "an empty detail page prints nothing");
}

/* ------------------------------------------------------------ the caret */

/* The caret sits at the end of the line, which means where the text
   actually ends and not where a per-character estimate puts it. With
   a proportional face the two diverge further with every character,
   so these drive several lines of equal length but different widths
   and require the caret to track the measured width exactly. */
static void caret(void)
{
    static const char* lines[] = {
        "", "i", "iiiiiiii", "mmmmmmmm", "map 12", "wwwwwwwwwwww", "il.il.il."
    };
    unsigned k;
    int firstGap = -1;
    int gapsEqual = 1;
    int seenPositions[7];

    /* The command field is monospaced, and both the height it lays
       itself out by and the width it measures the caret with have to
       come from that face. The mock answers either face with the same
       numbers, so nothing else here would notice the wrong one. */
    ovt_lastHeightFace = -1;
    ovt_lastWidthFace = -1;

    for (k = 0; k < sizeof(lines) / sizeof(lines[0]); ++k)
    {
        char want[OVL_CMDMAX + 4];
        int textX;
        int caretLeft;
        int caretRight;
        int caretTop;
        int caretBottom;
        int wantLeft;
        int n;

        resetAll();
        OVL_CommandShow();
        type(lines[k]);
        OVL_l_CommandField();

        /* the caret is the last quad of the batch */
        ok(ovl_vertCount >= 6, "the caret is drawn on the first frame");
        if (ovl_vertCount < 6)
            continue;
        n = ovl_vertCount - 6;
        caretLeft = (int)ovl_verts[n].pos.x;
        caretTop = (int)ovl_verts[n].pos.y;
        caretRight = (int)ovl_verts[n + 5].pos.x;
        caretBottom = (int)ovl_verts[n + 5].pos.y;

        sprintf(want, "> %s", lines[k]);
        textX = (int)OVL_FIELDTOP + 8;
        wantLeft = textX + FNT_i_TextWidth(FNT_OVERLAY, want) + 1;

        okEq(caretLeft, wantLeft, "the caret is at the end of the measured text");
        okEq(caretBottom - caretTop, ovt_fontHeight, "the caret is as tall as the font");
        ok(caretRight - caretLeft > 0 && caretRight - caretLeft <= 4,
           "the caret is a thin bar");

        /* the gap between where the text ends and the caret begins is
           the same for every line, however wide its characters are */
        if (firstGap < 0)
            firstGap = caretLeft - (textX + FNT_i_TextWidth(FNT_OVERLAY, want));
        else if (caretLeft - (textX + FNT_i_TextWidth(FNT_OVERLAY, want)) != firstGap)
            gapsEqual = 0;

        seenPositions[k] = caretLeft;
    }
    ok(gapsEqual, "the gap to the caret does not grow with the line");
    ok(ovt_lastHeightFace == FNT_OVERLAY,
       "the field took its height from the monospaced face");
    ok(ovt_lastWidthFace == FNT_OVERLAY,
       "and measured the caret with it too");

    /* eight narrow characters and eight wide ones are the same length
       and must not put the caret in the same place */
    ok(seenPositions[2] != seenPositions[3],
       "the caret follows the glyphs, not the character count");
    ok(seenPositions[2] < seenPositions[3],
       "a narrow line puts the caret further left than a wide one");

    /* and the line itself goes out in the overlay's own font, not the
       panel's small one */
    resetAll();
    OVL_CommandShow();
    type("hi");
    OVL_Draw();
    {
        int i;
        int bigLines = 0;
        for (i = 0; i < ovt_seenCount; i++)
            if (strcmp(ovt_seen[i], "> hi") == 0 && ovt_seenBig[i])
                bigLines++;
        okEq(bigLines, 1, "the command line uses the overlay font");
    }

    /* the box follows the font: a taller font gives a taller box */
    {
        float smallH;
        float bigH;
        int i;

        resetAll();
        ovt_fontHeight = 14;
        OVL_CommandShow();
        OVL_l_CommandField();
        smallH = 0.0f;
        for (i = 0; i < ovl_vertCount; i++)
            if (ovl_verts[i].pos.y > smallH)
                smallH = ovl_verts[i].pos.y;

        resetAll();
        ovt_fontHeight = 40;
        OVL_CommandShow();
        OVL_l_CommandField();
        bigH = 0.0f;
        for (i = 0; i < ovl_vertCount; i++)
            if (ovl_verts[i].pos.y > bigH)
                bigH = ovl_verts[i].pos.y;

        ok(bigH > smallH + 20.0f, "the field grows with the font");
        ovt_fontHeight = 20;
    }
}

/* ------------------------------------------------- placement and F1 */

/* the right-hand edge of everything the batch holds */
static float rightmost(void)
{
    float r = 0.0f;
    int i;
    for (i = 0; i < ovt_flushedCount; i++)
        if (ovt_flushedX[i] > r)
            r = ovt_flushedX[i];
    return r;
}

static float leftmost(void)
{
    float l = 1.0e9f;
    int i;
    for (i = 0; i < ovt_flushedCount; i++)
        if (ovt_flushedX[i] < l)
            l = ovt_flushedX[i];
    return l;
}

/* ------------------------------------------------ the after-effects

   The tweak is a switch, not a key: set it and the list stays up, clear
   it and it goes. The list reads the engine's one word, so it says the
   same thing whichever renderer is drawing. */
static int ovt_seenAt(const char* s, float* x)
{
    int i;

    for (i = 0; i < ovt_seenCount; i++)
        if (strcmp(ovt_seen[i], s) == 0)
        {
            if (x)
                *x = ovt_seenX[i];
            return 1;
        }
    return 0;
}

static int ovt_seenContaining(const char* sub, float* x, float* y)
{
    int i;

    for (i = 0; i < ovt_seenCount; i++)
        if (strstr(ovt_seen[i], sub))
        {
            if (x)
                *x = (float)ovt_seenX[i];
            if (y)
                *y = (float)ovt_seenY[i];
            return 1;
        }
    return 0;
}

/* The corner stamp: what is running, in the bottom right, for as long
   as the command field is up.
 *
   The alignment is checked by arithmetic rather than against a
   remembered number, because the whole point of measuring the line is
   that none of its three parts has a fixed width: a stamp placed at a
   constant would pass a check written the same way and still be in the
   wrong place at the next version number. */
static void cornerStamp(void)
{
    float x = 0.0f;
    float y = 0.0f;
    char  line[128];
    int   k;
    static const int widths[]  = { 640, 1280, 1920 };
    static const int heights[] = { 480, 720, 1080 };

    /* not up with the field down */
    resetAll();
    PRO_rasterWindowIsOn = 1;
    OVL_Draw();
    ok(!ovt_seenContaining("IRISEng", NULL, NULL),
       "the stamp stays down while the field is");

    /* up with it, saying all three things */
    resetAll();
    OVL_CommandShow();
    OVL_Draw();
    ok(ovt_seenContaining("IRISEng", &x, &y), "the stamp comes up with the field");
    ok(ovt_seenContaining(IRISENG_VERSION_STRING, NULL, NULL),
       "and it names the version");
    ok(ovt_seenContaining(IRISENG_ARCH, NULL, NULL), "and the architecture");
    ok(ovt_seenContaining("Mock renderer", NULL, NULL),
       "and the renderer, as the renderer names itself");

    /* and against the corner, at every size */
    sprintf(line, "IRISEng %s  %s  %s", IRISENG_VERSION_STRING,
            IRISENG_ARCH, "Mock renderer");
    for (k = 0; k < 3; ++k)
    {
        resetAll();
        ovt_drawWidth = widths[k];
        ovt_drawHeight = heights[k];
        OVL_CommandShow();
        OVL_Draw();
        if (!ovt_seenContaining("IRISEng", &x, &y))
        {
            ok(0, "the stamp is drawn at this viewport");
            continue;
        }
        /* the backdrop is what lines up with the field's edge, and the
           text sits one pad inside it */
        okEq((long)(x + (float)FNT_i_TextWidth(FNT_OVERLAY, line)
                      + OVL_STAMPPAD),
             widths[k] - (long)OVL_FIELDTOP,
             "its right edge is one field margin in from the viewport");
        okEq((long)(y + (float)FNT_i_Height(FNT_OVERLAY) + OVL_STAMPPAD),
             heights[k] - (long)OVL_FIELDTOP,
             "and its bottom edge likewise");
    }

    /* a renderer that does not name itself says so rather than lying */
    resetAll();
    ovt_dd.sz_Name = NULL;
    OVL_CommandShow();
    OVL_Draw();
    ok(ovt_seenContaining("unknown renderer", NULL, NULL),
       "an unnamed renderer is named as unknown");
    ovt_dd.sz_Name = OVT_sz_Name;
}

static void afterEffects(void)
{
    float heading = 0.0f;
    float rasters = 0.0f;
    int   onRows;
    int   i;

    printf("-- the after-effects list\n");

    /* nothing up: the tweak is what puts it there */
    resetAll();
    PC_AE_MASTER = 0x8;
    OVL_Draw();
    ok(!sawText("After-effects"), "off, the list is not drawn");
    okEq(ovt_beginCalls, 0, "and nothing is drawn at all");

    /* set, and it is up -- with a row for every effect */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0;
    OVL_Draw();
    ok(sawText("After-effects"), "set, the list is up");
    ok(sawText("depth blur"), "with the effects named");
    ok(sawText("remanence"), "all of them");
    ok(sawText("fade"), "down to the last");
    ok(sawText("brightness"),
       "including the two that have no bit of their own");
    ok(sawText("contrast"), "both of them");

    /* with none of them running, every row reads as off */
    onRows = 0;
    for (i = 0; i < ovt_seenCount; i++)
        if (strcmp(ovt_seen[i], "on") == 0)
            onRows++;
    okEq(onRows, 0, "with the word clear, no row says on");

    /* one bit set, one row on */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0x800;             /* remanence */
    OVL_Draw();
    onRows = 0;
    for (i = 0; i < ovt_seenCount; i++)
        if (strcmp(ovt_seen[i], "on") == 0)
            onRows++;
    okEq(onRows, 1, "one bit set, one row says on");

    /* two bits, two rows -- so the list is reading the word rather
       than answering the same way whatever is in it */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0x800 | 0x8;
    OVL_Draw();
    onRows = 0;
    for (i = 0; i < ovt_seenCount; i++)
        if (strcmp(ovt_seen[i], "on") == 0)
            onRows++;
    okEq(onRows, 2, "two bits set, two rows say on");

    /* the two without a bit: away from a half is running */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0;
    colorBalanceParam1 = 0.25f;
    OVL_Draw();
    onRows = 0;
    for (i = 0; i < ovt_seenCount; i++)
        if (strcmp(ovt_seen[i], "on") == 0)
            onRows++;
    okEq(onRows, 1, "a parameter away from a half counts as running");
    colorBalanceParam1 = 0.5f;

    /* the value beside an effect that is running, and only then */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0x800;                 /* remanence, id 7 */
    OVL_Draw();
    ok(sawText("0.750"), "a running effect shows its value");
    ok(!sawText("0.250"), "and not another effect's");

    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0;
    OVL_Draw();
    ok(!sawText("0.750"), "an effect that is off shows no value");

    /* the value is fetched by the effect's id, which is not the bit's
       position -- two rows whose ids the getter answers differently */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0x1000;                /* depth blur near, id 14 */
    OVL_Draw();
    ok(sawText("1.125"), "the value is fetched by the effect's id");
    ok(!sawText("0.125"), "not by the row's position or its bit");

    /* the rows the getter has no case for print no number at all,
       rather than the nought it answers with */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0x0040;                /* emboss, which has no value */
    OVL_Draw();
    ok(sawText("on"), "an effect with no value still says it is on");
    ok(!sawText("0.000"), "and prints no value for it");

    /* the two without a bit read their own parameter */
    resetAll();
    tweak_ae_view = 1;
    PC_AE_MASTER = 0;
    brightnessParam = 0.375f;
    OVL_Draw();
    ok(sawText("0.375"), "brightness shows the parameter it runs from");
    brightnessParam = 0.5f;

    /* cleared, and it is gone again */
    resetAll();
    tweak_ae_view = 0;
    PC_AE_MASTER = 0x8;
    OVL_Draw();
    ok(!sawText("After-effects"), "cleared, the list goes");

    /* down the left, where the rasters panel is not */
    resetAll();
    tweak_ae_view = 1;
    PRO_rasterWindowIsOn = 1;
    ovt_drawWidth = 1280;
    OVL_Draw();
    ok(ovt_seenAt("After-effects", &heading), "both up together");
    ok(ovt_seenAt("Rasters", &rasters), "the rasters panel as well");
    ok(heading < rasters - 200.0f,
       "and the list is down the left, clear of it");
    tweak_ae_view = 0;
}

static void placement(void)
{
    static const int widths[] = { 640, 800, 1280, 1920 };
    unsigned k;
    float narrowLeft;
    float wideLeft;

    /* the panel hangs off the right edge, at every width */
    for (k = 0; k < sizeof(widths) / sizeof(widths[0]); ++k)
    {
        resetAll();
        ovt_drawWidth = widths[k];
        PRO_rasterWindowIsOn = 1;
        OVL_Draw();

        okEq((long)rightmost(), widths[k] - (long)OVL_PANELMARGIN,
             "the panel's right edge is one margin in from the viewport");
        okEq((long)leftmost(), widths[k] - (long)OVL_PANELW - (long)OVL_PANELMARGIN,
             "the panel is its own width wide");
    }

    /* and it moves when the viewport does */
    resetAll();
    ovt_drawWidth = 640;
    PRO_rasterWindowIsOn = 1;
    OVL_Draw();
    narrowLeft = leftmost();
    resetAll();
    ovt_drawWidth = 1920;
    PRO_rasterWindowIsOn = 1;
    OVL_Draw();
    wideLeft = leftmost();
    ok(wideLeft > narrowLeft + 1000.0f, "the panel follows the right edge");

    /* its text goes with it */
    resetAll();
    ovt_drawWidth = 1280;
    PRO_rasterWindowIsOn = 1;
    OVL_Draw();
    {
        int i;
        int outside = 0;
        for (i = 0; i < ovt_seenCount; i++)
            if (ovt_seenX[i] < 1280 - (int)OVL_PANELW - (int)OVL_PANELMARGIN
                || ovt_seenX[i] > 1280)
            {
                outside++;
            }
        okEq(outside, 0, "the panel's strings move with the panel");
    }

    /* a viewport narrower than the panel keeps it on screen */
    resetAll();
    ovt_drawWidth = 200;
    PRO_rasterWindowIsOn = 1;
    OVL_Draw();
    okEq((long)leftmost(), (long)OVL_PANELMARGIN,
         "a narrow viewport puts the panel at the left edge, not off it");

    /* the command field is still full width and not right-aligned */
    resetAll();
    ovt_drawWidth = 1280;
    OVL_CommandShow();
    OVL_Draw();
    okEq((long)leftmost(), 10, "the field still starts at the left margin");
    okEq((long)rightmost(), 1270, "the field still runs to the right margin");
}

/* F1 steps the panel on, to the detail page, and off again. The old
   cycle alternated between the two pages for ever once it was up. */
static void f1Cycle(void)
{
    profilerRasterStructElem walked;

    resetAll();
    okEq(PRO_rasterWindowIsOn, 0, "the panel starts off");
    okEq(PRO_rasterDetailPage, 0, "the panel starts on no page");

    OVL_RastersCycleView();
    okEq(PRO_rasterWindowIsOn, 1, "one press shows the summary");
    okEq(PRO_rasterDetailPage, 0, "one press is not the detail page");

    OVL_RastersCycleView();
    okEq(PRO_rasterWindowIsOn, 1, "two presses stay up");
    okEq(PRO_rasterDetailPage, 1, "two presses reach the detail page");

    OVL_RastersCycleView();
    okEq(PRO_rasterWindowIsOn, 0, "three presses hide the panel");
    okEq(PRO_rasterDetailPage, 0, "three presses leave no page set");

    OVL_RastersCycleView();
    okEq(PRO_rasterWindowIsOn, 1, "four presses are back at the summary");
    okEq(PRO_rasterDetailPage, 0, "four presses are back at the first page");

    /* and nothing is drawn in the hidden state */
    resetAll();
    OVL_RastersCycleView();
    OVL_RastersCycleView();
    OVL_RastersCycleView();
    OVL_Draw();
    okEq(ovt_beginCalls, 0, "a hidden panel draws nothing");

    /* every step restarts the walk, so a drilled-into raster does not
       survive the change */
    resetAll();
    memset(&walked, 0, sizeof(walked));
    DBG_profiler_first_raster = &walked;
    PRO_CurrRaster = &walked;
    pointsToUnkRaster_dword_98F944 = &walked;
    PRO_FirstRasterPtr = NULL;
    OVL_RastersCycleView();
    ok(PRO_CurrRaster == NULL, "a step clears the drilled-into raster");
    ok(pointsToUnkRaster_dword_98F944 == NULL, "a step clears the subgroup filter");
    ok(PRO_FirstRasterPtr == &walked, "a step restarts the walk");
    DBG_profiler_first_raster = NULL;
}

/* --------------------------------------------------------------- the pass */

static void pass(void)
{
    /* nothing up: no device traffic at all */
    resetAll();
    OVL_Draw();
    okEq(ovt_beginCalls, 0, "an idle overlay does not touch the device");
    okEq(ovt_seenCount, 0, "an idle overlay draws no strings");

    /* no device: nothing, even with a widget up */
    resetAll();
    ovt_ready = 0;
    ovl_cmdActive = 1;
    OVL_Draw();
    okEq(ovt_beginCalls, 0, "the overlay waits for a device");

    /* the field alone */
    resetAll();
    OVL_CommandShow();
    type("map 12");
    OVL_Draw();
    okEq(ovt_beginCalls, 1, "the field's pass is set up once");
    okEq(ovt_endCalls, 1, "the field's pass is torn down once");
    ok(ovt_trianglesDrawn > 0, "the field draws geometry");
    ok(sawText("> map 12"), "the field shows the prompt and the line");
    okEq(ovt_textAfterQuads, 1, "the strings are issued after the quads");
    okEq(ovl_textCount, 0, "the text queue is emptied by the pass");

    /* the summary panel alone */
    resetAll();
    PRO_rasterWindowIsOn = 1;
    OVL_Draw();
    okEq(ovt_beginCalls, 1, "the panel's pass is set up once");
    ok(sawText("Rasters"), "the panel is titled");
    ok(ovt_trianglesDrawn > 0, "the panel draws its backdrop");

    /* both at once, in one pass */
    resetAll();
    PRO_rasterWindowIsOn = 1;
    OVL_CommandShow();
    OVL_Draw();
    okEq(ovt_beginCalls, 1, "both widgets share one pass");
    okEq(ovt_quadCalls, 1, "both widgets share one quad batch");
    ok(sawText("Rasters") && sawText("> "), "both widgets are drawn");

    /* the detail page, which is the walk rather than the two figures */
    resetAll();
    makeRaster(&rasters[0], "g", "a", "m0", 3, 0.0f);
    DBG_profiler_first_raster = &rasters[0];
    PRO_FirstRasterPtr = &rasters[0];
    PRO_rasterDetailPage = 1;
    OVL_Draw();
    ok(sawText("m0"), "the detail page reaches the pass");

    /* a long page cannot grow the backdrop past the viewport */
    resetAll();
    PRO_rasterWindowIsOn = 1;
    ovt_drawHeight = 80;
    OVL_Draw();
    {
        int i;
        int below = 0;
        for (i = 0; i < ovl_vertCount; i++)
            if (ovl_verts[i].pos.y > 80.0f)
                below++;
        okEq(below, 0, "the panel is clipped to the viewport");
    }

    /* the batch is left empty, so the next frame starts clean */
    resetAll();
    OVL_CommandShow();
    OVL_Draw();
    okEq(ovl_vertCount, 0, "the pass leaves the batch empty");

    /* the caret blinks rather than sitting on */
    resetAll();
    OVL_CommandShow();
    {
        int i;
        int on = 0;
        int off = 0;
        for (i = 0; i < 80; i++)
        {
            int before = ovl_vertCount;
            OVL_l_CommandField();
            /* the outline is a fixed number of quads; anything over it
               in a given frame is the caret */
            if (ovl_vertCount - before > 0)
            {
                if (i == 0)
                    on = ovl_vertCount - before;
                else if (ovl_vertCount - before < on)
                    off = 1;
            }
            ovl_vertCount = 0;
            ovl_textCount = 0;
        }
        ok(off != 0, "the caret blinks");
    }
}

int main(void)
{
    printf("=== overlay unit test ===\n");
    fieldStateMachine();
    shapes();
    caret();
    placement();
    cornerStamp();
    afterEffects();
    f1Cycle();
    panel();
    pass();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
