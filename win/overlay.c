#include "overlay.h"
#include "../font.h"
#include "../gdi.h"
#include "../profiler.h"
#include "../time.h"
#include "../dbg.h"
#include "wininputoutput.h"
#include <stdio.h>
#include <string.h>

// Everything the overlay draws goes through these two: a quad batch of
// its own, so the profiler bars' batch is left alone, and the debug
// font the rasters code already creates.

#define OVL_MAXQUADS   512
#define OVL_CMDMAX     128

#define OVL_ROWHEIGHT  13
#define OVL_PANELMARGIN 8.0f
#define OVL_PANELY     60.0f
#define OVL_PANELW     320.0f
/* The backdrop is drawn in floats, so its width is one. The two
   columns inside it are pixels and ints, which is what the call that
   places text takes -- no cast at the call sites, and nothing for the
   warning sweep to find. */
#define OVL_PANELW_AE  215.0f
#define OVL_AE_STATEX  118
#define OVL_AE_VALUEX  146

#define OVL_FIELDTOP   10.0f
#define OVL_FIELDPAD   6.0f     /* above and below the line of text */
#define OVL_RADIUS     8.0f
#define OVL_BORDER     2.0f
#define OVL_CORNERSEG  5

#define OVL_GREEN      0xFF40FF40
#define OVL_BACKDROP   0xB0000000
#define OVL_TEXT       0xFFFFFFFF
#define OVL_CARET      0xFF40FF40
/* an effect that is not running: there, but not shouting */
#define OVL_DIM        0xFF808080

static tGdi_Vertex ovl_verts[OVL_MAXQUADS * 6];
static int ovl_vertCount = 0;

// The font draws straight through its own sprite, so a string issued
// while quads are still batched would be painted over when the batch
// flushes. The strings are queued here and emitted after it.
#define OVL_MAXTEXT  192
#define OVL_TEXTLEN  96
typedef struct { int x, y; unsigned int color; int big; char s[OVL_TEXTLEN]; } tOvlText;
static tOvlText ovl_text[OVL_MAXTEXT];
static int ovl_textCount = 0;

static int  ovl_cmdActive = 0;
static char ovl_cmdText[OVL_CMDMAX];
static int  ovl_cmdLen = 0;
static int  ovl_caretTick = 0;

// ---------------------------------------------------------------- shapes

static void OVL_l_Quad(unsigned int color, float x, float y, float w, float h)
{
    tGdi_Vertex* q;
    float right;
    float bottom;
    int i;

    if (ovl_vertCount + 6 > OVL_MAXQUADS * 6 || w <= 0.0f || h <= 0.0f)
        return;

    q = &ovl_verts[ovl_vertCount];
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
        q[i].color = (int)color;
    }
    ovl_vertCount += 6;
}

// A rounded rectangle, filled, as a centre block plus the two side
// blocks plus one horizontal strip per corner step. The steps come
// from the circle equation, so there is nothing to load.
static void OVL_l_RoundRectFill(unsigned int color, float x, float y,
                                float w, float h, float r)
{
    float step;
    float dy;
    float inset;
    int i;

    if (r * 2.0f > w) r = w * 0.5f;
    if (r * 2.0f > h) r = h * 0.5f;
    if (r < 1.0f)
    {
        OVL_l_Quad(color, x, y, w, h);
        return;
    }

    OVL_l_Quad(color, x, y + r, w, h - 2.0f * r);

    step = r / (float)OVL_CORNERSEG;
    for (i = 0; i < OVL_CORNERSEG; i++)
    {
        dy = r - ((float)i + 0.5f) * step;
        inset = r - (float)sqrt((double)(r * r - dy * dy));
        OVL_l_Quad(color, x + inset, y + (float)i * step,
                   w - 2.0f * inset, step);
        OVL_l_Quad(color, x + inset, y + h - (float)(i + 1) * step,
                   w - 2.0f * inset, step);
    }
}

// The outline is the fill minus a smaller fill, which keeps the corner
// maths in one place and needs no line primitive.
static void OVL_l_RoundRectOutline(unsigned int color, unsigned int inner,
                                   float x, float y, float w, float h,
                                   float r, float t)
{
    OVL_l_RoundRectFill(color, x, y, w, h, r);
    OVL_l_RoundRectFill(inner, x + t, y + t, w - 2.0f * t, h - 2.0f * t,
                        r - t > 1.0f ? r - t : 1.0f);
}

// The panel keeps the small rasters font, which is what its column
// positions were laid out for; the command line takes the overlay's
// own, which is larger and scaled to the viewport.
static void OVL_l_QueueText(const char* s, int x, int y, unsigned int color, int big)
{
    tOvlText* t;

    if (ovl_textCount >= OVL_MAXTEXT || !s)
        return;
    t = &ovl_text[ovl_textCount++];
    t->x = x;
    t->y = y;
    t->color = color;
    t->big = big;
    strncpy(t->s, s, OVL_TEXTLEN - 1);
    t->s[OVL_TEXTLEN - 1] = 0;
}

static void OVL_l_Text(const char* s, int x, int y, unsigned int color)
{
    OVL_l_QueueText(s, x, y, color, 0);
}

static void OVL_l_BigText(const char* s, int x, int y, unsigned int color)
{
    OVL_l_QueueText(s, x, y, color, 1);
}

static void OVL_l_FlushText(void)
{
    int i;

    for (i = 0; i < ovl_textCount; i++)
    {
        if (ovl_text[i].big)
            FNT_DrawText(FNT_OVERLAY, ovl_text[i].s, ovl_text[i].x,
                         ovl_text[i].y, ovl_text[i].color);
        else
            FNT_DrawText(FNT_RASTERS, ovl_text[i].s, ovl_text[i].x,
                         ovl_text[i].y, ovl_text[i].color);
    }
    ovl_textCount = 0;
}

// ------------------------------------------------------------ the panel

// The panel sits against the right edge, so its left is a runtime
// value rather than a constant; at a viewport narrower than the panel
// it falls back to the left edge rather than going off-screen.
// Which side the panel being drawn sits on. The rasters have always
// been on the right; the after-effects go on the left so the two can
// be up together.
static int ovl_panelLeft = 0;

static float OVL_l_PanelX(void)
{
    float x;
    int   w = 0;
    int   h = 0;

    if (ovl_panelLeft)
        return OVL_PANELMARGIN;
    gpst_DX_stDD->GetDrawingSize(&w, &h);
    x = (float)w - OVL_PANELW - OVL_PANELMARGIN;
    if (x < OVL_PANELMARGIN)
        x = OVL_PANELMARGIN;
    return x;
}

// Panel-local coordinates, so the rows below keep the column positions
// the rasters window used.
static void OVL_l_PanelText(const char* s, int x, int y, unsigned int color)
{
    OVL_l_Text(s, (int)OVL_l_PanelX() + x, (int)OVL_PANELY + y, color);
}

// One raster, drawn the way the window drew it: a subgroup heading when
// this is the first of its subgroup, the module name, and the value in
// whichever of the five forms the raster carries. The default form is a
// time, and reading it clears the accumulator, so this walk is what
// advances the raster from one frame to the next.
static int OVL_l_RasterRow(int y, profilerRasterStructElem* raster, int withSubgroup)
{
    char text[512];
    int ticks;
    int x;
    int lineY;

    x = 10;
    lineY = y;
    if (pointsToUnkRaster_dword_98F944)
    {
        OVL_l_PanelText(raster->module, 10, y, OVL_TEXT);
    }
    else
    {
        if (withSubgroup)
        {
            OVL_l_PanelText(raster->subgroup, 10, y, OVL_GREEN);
            if (!PRO_CurrRaster)
                lineY = y + OVL_ROWHEIGHT;
        }
        x = 20;
        if (PRO_CurrRaster)
            x = 80;
        OVL_l_PanelText(raster->module, x, lineY, OVL_TEXT);
    }

    switch (raster->unk1)
    {
    case 1:
        sprintf(text, "%d", *(int32_t*)&raster->unk2);
        break;
    case 2:
        strncpy(text, (char*)&raster->unk2, sizeof(text) - 1);
        text[sizeof(text) - 1] = 0;
        break;
    case 3:
        return lineY + OVL_ROWHEIGHT;
    case 4:
        sprintf(text, "%.5f", raster->unk2);
        break;
    default:
        ticks = raster->sumTime;
        if (ticks)
        {
            raster->unk10 = ticks;
            raster->sumTime = 0;
        }
        else
        {
            ticks = raster->unk10;
        }
        sprintf(text, "%.3f",
                gfDBGRasterSetTo60 * ((double)ticks / (double)TIM_gul_TicksPerSecond));
        break;
    }
    OVL_l_PanelText(text, x + 150, lineY, OVL_TEXT);
    raster->unk7 = 0;
    return lineY + OVL_ROWHEIGHT;
}

// The detail page: the same group, subgroup and module walk the window
// did, with the rows going to the overlay instead of a device context.
static int OVL_l_RastersDetail(void)
{
    profilerRasterStructElem* start;
    profilerRasterStructElem* head;
    profilerRasterStructElem* scan;
    int displayed;
    int isSubgroupHead;
    int y;

    y = 24;
    if (!PRO_FirstRasterPtr)
        return y;
    displayed = 0;
    for (start = DBG_profiler_first_raster; start; start = start->prev)
    {
        if (start->unk7 && !_stricmp(start->group, PRO_FirstRasterPtr->group))
        {
            for (head = start; head; head = head->prev)
            {
                if (!head->unk7
                    || _stricmp(head->group, PRO_FirstRasterPtr->group)
                    || pointsToUnkRaster_dword_98F944
                       && _stricmp(head->subgroup, pointsToUnkRaster_dword_98F944->subgroup))
                {
                    continue;
                }
                for (scan = head; scan; scan = scan->prev)
                {
                    if (!scan->unk7
                        || _stricmp(scan->group, PRO_FirstRasterPtr->group)
                        || _stricmp(scan->subgroup, head->subgroup))
                    {
                        continue;
                    }
                    if (PRO_CurrRaster && _stricmp(scan->module, PRO_CurrRaster->module))
                        continue;
                    isSubgroupHead = PRO_CurrRaster != NULL || scan == head;
                    y = OVL_l_RasterRow(y, scan, isSubgroupHead);
                    if (PRO_CurrRaster)
                    {
                        displayed = 1;
                        break;
                    }
                }
                if (displayed)
                    break;
            }
            if (displayed)
                return y;
        }
        start->unk7 = 1;
    }
    return y;
}

// The first page: the frame time and the tick delta, both scaled the
// way the window scaled them.
static int OVL_l_RastersSummary(void)
{
    char text[64];

    sprintf(text, "%f", gfDBGRasterSetTo60 * TIM_gf_dt);
    OVL_l_PanelText(text, 10, 24, OVL_TEXT);
    sprintf(text, "%f", TIM_dtTicks * gfDBGRasterSetTo60);
    OVL_l_PanelText(text, 10, 44, OVL_TEXT);
    return 60;
}

// F1 steps the panel through its states and then off again: nothing,
// the summary, the detail page, nothing. It used to alternate between
// the two pages once it was up, with no way back to nothing short of
// restarting. The walk is restarted at every step, so a raster that
// was drilled into does not survive the change.
void OVL_RastersCycleView(void)
{
    if (!PRO_rasterWindowIsOn)
    {
        PRO_rasterWindowIsOn = 1;
        PRO_rasterDetailPage = 0;
    }
    else if (!PRO_rasterDetailPage)
    {
        PRO_rasterDetailPage = 1;
    }
    else
    {
        PRO_rasterWindowIsOn = 0;
        PRO_rasterDetailPage = 0;
    }
    PRO_FirstRasterPtr = DBG_profiler_first_raster;
    pointsToUnkRaster_dword_98F944 = NULL;
    PRO_CurrRaster = NULL;
}

// ------------------------------------------------------- command field

int OVL_b_CommandActive(void)
{
    return ovl_cmdActive;
}

void OVL_CommandShow(void)
{
    ovl_cmdActive = 1;
    ovl_cmdLen = 0;
    ovl_cmdText[0] = 0;
    ovl_caretTick = 0;
}

void OVL_CommandHide(void)
{
    ovl_cmdActive = 0;
    ovl_cmdLen = 0;
    ovl_cmdText[0] = 0;
}

int OVL_b_CommandMessage(unsigned int msg, unsigned int wParam)
{
    if (!ovl_cmdActive)
        return 0;

    if (msg == WM_CHAR)
    {
        if (wParam == '\r' || wParam == '\n')
        {
            // Enter runs the line, puts the field away and hands the
            // keyboard and mouse back to DirectInput.
            pcINO_RunTweakCommand(ovl_cmdText);
            OVL_CommandHide();
            pcINO_dinput_mouse_and_keyb_acquire();
            return 1;
        }
        if (wParam == '\b')
        {
            if (ovl_cmdLen > 0)
                ovl_cmdText[--ovl_cmdLen] = 0;
            return 1;
        }
        if (wParam == 27)                       // escape: abandon the line
        {
            OVL_CommandHide();
            pcINO_dinput_mouse_and_keyb_acquire();
            return 1;
        }
        if (wParam >= 32 && wParam < 127 && ovl_cmdLen < OVL_CMDMAX - 1)
        {
            ovl_cmdText[ovl_cmdLen++] = (char)wParam;
            ovl_cmdText[ovl_cmdLen] = 0;
        }
        return 1;
    }
    // Swallow the key events too, so nothing else in the window sees
    // them while the field has the keyboard.
    return msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_SYSCHAR;
}

static void OVL_l_CommandField(void)
{
    float viewWidth;
    float x;
    float w;
    float fieldH;
    float textTop;
    int fontH;
    int caretX;
    int drawW = 0;
    int drawH = 0;
    char line[OVL_CMDMAX + 4];

    gpst_DX_stDD->GetDrawingSize(&drawW, &drawH);
    viewWidth = (float)drawW;
    w = viewWidth - 2.0f * OVL_FIELDTOP;
    x = OVL_FIELDTOP;

    // The box is as tall as the line of text it holds, so it follows
    // the font rather than the font having to fit a fixed box.
    fontH = FNT_i_Height(FNT_OVERLAY);
    fieldH = (float)fontH + 2.0f * OVL_FIELDPAD;
    textTop = OVL_FIELDTOP + OVL_FIELDPAD;

    OVL_l_RoundRectOutline(OVL_GREEN, OVL_BACKDROP, x, OVL_FIELDTOP, w,
                           fieldH, OVL_RADIUS, OVL_BORDER);

    sprintf(line, "> %s", ovl_cmdText);
    OVL_l_BigText(line, (int)x + 8, (int)textTop, OVL_TEXT);

    // The caret goes where the text actually ends. Measuring the
    // string is the only way to put it there: an estimate per
    // character drifts further from the text with every one typed,
    // and the face is proportional, so no single width is right.
    caretX = (int)x + 8 + FNT_i_TextWidth(FNT_OVERLAY, line);

    ovl_caretTick++;
    if (((ovl_caretTick / 20) & 1) == 0)
    {
        OVL_l_Quad(OVL_CARET, (float)caretX + 1.0f, textTop,
                   2.0f, (float)fontH);
    }
}

// ------------------------------------------------------------- the pass


// ---------------------------------------------------------------- the
// after-effects, and which of them are running
//
// The engine keeps them in one word, PC_AE_MASTER, a bit to an effect,
// and both renderers read that same word -- so this list says what is
// running whichever renderer is drawing it.
//
// The names are what the chain does with each bit rather than what the
// engine's own getter calls it, and the two disagree once: the getter
// names bit 0 "colorbalance", and the bit runs the grayscale pass.
// Colour balance is one of the two effects below that have no bit at
// all -- they run when their parameter is away from a half -- so
// naming this one after the getter would have put the same name on two
// different rows, one of which never runs for that reason.

typedef struct sOvlEffect
{
    const char*  name;
    unsigned int bit;        /* 0 for the two with no bit of their own */
    int          id;         /* what the parameter getters call it */
    int          hasValue;   /* whether the value getter answers for it */
} tOvlEffect;

/* The id is the effect number the engine's getters take, and it is not
   the bit's position: PC_AE_MASTER_GET_ONOFF and PC_AE_MASTER_GET_P1
   are two switches over the same ids, and some ids appear in only one
   of them. So a row's value comes from the value getter by id, and the
   rows that getter has no case for are marked as having none rather
   than printing the zero it returns for anything it does not know.

   The last two have no bit at all: they run when their parameter is
   away from a half, and their ids are in the value getter only. */
static const tOvlEffect ovl_effects[] = {
    { "depth blur",      0x0008u,  0, 1 },
    { "depth blur near", 0x1000u, 14, 1 },
    { "motion blur",     0x0010u,  1, 1 },
    { "screen blur",     0x0020u,  2, 1 },
    { "full aa blur",    0x2000u, 10, 0 },
    { "emboss",          0x0040u,  3, 0 },
    { "motion smooth",   0x0080u,  4, 0 },
    { "zoom smooth",     0x0400u,  5, 1 },
    { "zoom smooth 2",   0x8000u,  6, 1 },
    { "remanence",       0x0800u,  7, 1 },
    { "hue",             0x0004u, 12, 1 },
    { "grayscale",       0x0001u, 11, 1 },
    { "fade",            0x40000u, 15, 0 },
    { "brightness",      0u,       8, 1 },
    { "contrast",        0u,       9, 1 }
};

#define OVL_EFFECTNUM ((int)(sizeof(ovl_effects) / sizeof(ovl_effects[0])))

// A row's state: the bit where it has one, and otherwise its parameter
// being away from the half that means "leave the picture alone", which
// is how the chain itself tests those two.
static int OVL_l_b_EffectOn(const tOvlEffect* e)
{
    if (e->bit != 0u)
        return (PC_AE_MASTER & (int)e->bit) != 0;
    if (e->id == 8)
        return brightnessParam != 0.5f;
    if (e->id == 9)
        return colorBalanceParam1 != 0.5f;
    return 0;
}

static int OVL_l_EffectRows(void)
{
    char value[32];
    int  y = 24;
    int  i;
    int  on;

    for (i = 0; i < OVL_EFFECTNUM; i++)
    {
        const tOvlEffect* e = &ovl_effects[i];

        on = OVL_l_b_EffectOn(e);
        OVL_l_PanelText(e->name, 10, y, on ? OVL_GREEN : OVL_DIM);
        OVL_l_PanelText(on ? "on" : "-", OVL_AE_STATEX, y,
                        on ? OVL_GREEN : OVL_DIM);
        // The value only while it is running: off, the parameter is
        // whatever it was left at and says nothing about the frame.
        if (on && e->hasValue)
        {
            sprintf(value, "%.3f", PC_AE_MASTER_GET_P1(e->id, 0));
            OVL_l_PanelText(value, OVL_AE_VALUEX, y, OVL_GREEN);
        }
        y += OVL_ROWHEIGHT;
    }


    return y;
}

// Down the left, so it does not fight the rasters panel on the right.
static void OVL_l_AfterEffects(void)
{
    int bottom;

    ovl_panelLeft = 1;
    bottom = OVL_l_EffectRows();
    OVL_l_PanelText("After-effects", 10, 6, OVL_GREEN);
    OVL_l_RoundRectFill(OVL_BACKDROP, OVL_l_PanelX(), OVL_PANELY,
                        OVL_PANELW_AE, (float)bottom + 2.0f, OVL_RADIUS);
    ovl_panelLeft = 0;
}

void OVL_Draw(void)
{
    float height;
    int rastersOn;
    int bottom;
    int drawW = 0;
    int drawH = 0;

    if (!gpst_DX_stDD || !gpst_DX_stDD->b_Ready())
        return;
    rastersOn = PRO_rasterDetailPage || PRO_rasterWindowIsOn;
    if (!rastersOn && !ovl_cmdActive && !tweak_ae_view)
        return;

    gpst_DX_stDD->GetDrawingSize(&drawW, &drawH);
    ovl_vertCount = 0;
    ovl_textCount = 0;

    gpst_DX_stDD->OverlayBegin();

    if (rastersOn)
    {
        // The rows are queued before the backdrop is emitted, so the
        // backdrop can be sized to what the walk actually produced.
        bottom = PRO_rasterDetailPage ? OVL_l_RastersDetail() : OVL_l_RastersSummary();
        OVL_l_PanelText("Rasters", 10, 6, OVL_GREEN);
        height = (float)bottom + 8.0f;
        if (OVL_PANELY + height > (float)drawH - 8.0f)
            height = (float)drawH - 8.0f - OVL_PANELY;
        OVL_l_RoundRectFill(OVL_BACKDROP, OVL_l_PanelX(), OVL_PANELY, OVL_PANELW,
                            height, OVL_RADIUS);
    }
    if (tweak_ae_view)
        OVL_l_AfterEffects();
    if (ovl_cmdActive)
        OVL_l_CommandField();

    // Shapes as one batch, then the strings on top of them.
    gpst_DX_stDD->OverlayDrawQuads(ovl_verts, ovl_vertCount / 3);
    ovl_vertCount = 0;
    OVL_l_FlushText();

    gpst_DX_stDD->OverlayEnd();
}
