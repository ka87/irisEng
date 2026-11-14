#include "overlay.h"
#include "wingdi.h"
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

#define OVL_FIELDTOP   10.0f
#define OVL_FIELDPAD   6.0f     /* above and below the line of text */
#define OVL_RADIUS     8.0f
#define OVL_BORDER     2.0f
#define OVL_CORNERSEG  5

#define OVL_GREEN      0xFF40FF40
#define OVL_BACKDROP   0xB0000000
#define OVL_TEXT       0xFFFFFFFF
#define OVL_CARET      0xFF40FF40

static tVertexAndColor ovl_verts[OVL_MAXQUADS * 6];
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
    tVertexAndColor* q;
    float right;
    float bottom;
    int i;

    if (ovl_vertCount + 6 > OVL_MAXQUADS * 6 || w <= 0.0f || h <= 0.0f)
        return;

    q = &ovl_verts[ovl_vertCount];
    right = x + w;
    bottom = y + h;

    q[0].v.x = x;      q[0].v.y = y;
    q[1].v.x = right;  q[1].v.y = y;
    q[2].v.x = x;      q[2].v.y = bottom;
    q[3].v.x = x;      q[3].v.y = bottom;
    q[4].v.x = right;  q[4].v.y = y;
    q[5].v.x = right;  q[5].v.y = bottom;
    for (i = 0; i < 6; i++)
    {
        q[i].v.z = 0.0f;
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
            D3D_OverlayDrawText(ovl_text[i].s, ovl_text[i].x, ovl_text[i].y,
                                (D3DCOLOR)ovl_text[i].color);
        else
            D3D_RastersDrawText(ovl_text[i].s, ovl_text[i].x, ovl_text[i].y,
                                (D3DCOLOR)ovl_text[i].color);
    }
    ovl_textCount = 0;
}

// ------------------------------------------------------------ the panel

// The panel sits against the right edge, so its left is a runtime
// value rather than a constant; at a viewport narrower than the panel
// it falls back to the left edge rather than going off-screen.
static float OVL_l_PanelX(void)
{
    float x;

    x = (float)D3DRenderParams.d3dViewPort.Width - OVL_PANELW - OVL_PANELMARGIN;
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
    char line[OVL_CMDMAX + 4];

    viewWidth = (float)D3DRenderParams.d3dViewPort.Width;
    w = viewWidth - 2.0f * OVL_FIELDTOP;
    x = OVL_FIELDTOP;

    // The box is as tall as the line of text it holds, so it follows
    // the font rather than the font having to fit a fixed box.
    fontH = D3D_OverlayFontHeight();
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
    caretX = (int)x + 8 + D3D_OverlayTextWidth(line);

    ovl_caretTick++;
    if (((ovl_caretTick / 20) & 1) == 0)
    {
        OVL_l_Quad(OVL_CARET, (float)caretX + 1.0f, textTop,
                   2.0f, (float)fontH);
    }
}

// ------------------------------------------------------------- the pass

void OVL_Draw(void)
{
    float height;
    int rastersOn;
    int bottom;

    if (!D3DRenderParams.d3dDev)
        return;
    rastersOn = PRO_rasterDetailPage || PRO_rasterWindowIsOn;
    if (!rastersOn && !ovl_cmdActive)
        return;

    ovl_vertCount = 0;
    ovl_textCount = 0;

    D3D_OverlayBegin();

    if (rastersOn)
    {
        // The rows are queued before the backdrop is emitted, so the
        // backdrop can be sized to what the walk actually produced.
        bottom = PRO_rasterDetailPage ? OVL_l_RastersDetail() : OVL_l_RastersSummary();
        OVL_l_PanelText("Rasters", 10, 6, OVL_GREEN);
        height = (float)bottom + 8.0f;
        if (OVL_PANELY + height > (float)D3DRenderParams.d3dViewPort.Height - 8.0f)
            height = (float)D3DRenderParams.d3dViewPort.Height - 8.0f - OVL_PANELY;
        OVL_l_RoundRectFill(OVL_BACKDROP, OVL_l_PanelX(), OVL_PANELY, OVL_PANELW,
                            height, OVL_RADIUS);
    }
    if (ovl_cmdActive)
        OVL_l_CommandField();

    // Shapes as one batch, then the strings on top of them.
    D3D_OverlayDrawQuads(ovl_verts, ovl_vertCount / 3);
    ovl_vertCount = 0;
    OVL_l_FlushText();

    D3D_OverlayEnd();
}
