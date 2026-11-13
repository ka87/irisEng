/* Mocks for the profiler.c differential test: a deterministic clock, a
   deterministic performance counter, and the GDI entry points the
   raster window draws with, each one logged so the two copies can be
   compared on the ordered sequence of calls they make. */

#include "profiler_mocks.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

tProMockCall PROMOCK_log[PROMOCK_LOG_MAX];
int PROMOCK_logLen;
int PROMOCK_overflow;

unsigned int PROMOCK_ticks;
unsigned int PROMOCK_tickStep;
long long PROMOCK_qpc;
long long PROMOCK_qpcStep;

void PROMOCK_Reset(void)
{
    memset(PROMOCK_log, 0, sizeof(PROMOCK_log));
    PROMOCK_logLen = 0;
    PROMOCK_overflow = 0;
    PROMOCK_ticks = 1000;
    PROMOCK_tickStep = 7;
    PROMOCK_qpc = 100000;
    PROMOCK_qpcStep = 13;
}

void PROMOCK_Record(const char* fn, const char* text, int nargs, ...)
{
    tProMockCall* c;
    va_list ap;
    int i;

    if (PROMOCK_logLen >= PROMOCK_LOG_MAX)
    {
        PROMOCK_overflow = 1;
        return;
    }
    c = &PROMOCK_log[PROMOCK_logLen++];
    c->fn = fn;
    c->nargs = nargs > PROMOCK_ARGS ? PROMOCK_ARGS : nargs;
    va_start(ap, nargs);
    for (i = 0; i < c->nargs; i++)
        c->arg[i] = va_arg(ap, int);
    va_end(ap);
    if (text)
    {
        strncpy(c->text, text, PROMOCK_TEXT - 1);
        c->text[PROMOCK_TEXT - 1] = 0;
    }
}

unsigned int TIM_GetTicksAsInt32(void)
{
    unsigned int t = PROMOCK_ticks;
    PROMOCK_ticks += PROMOCK_tickStep;
    return t;
}

BOOL QueryPerformanceCounter(LARGE_INTEGER* li)
{
    li->QuadPart = PROMOCK_qpc;
    PROMOCK_qpc += PROMOCK_qpcStep;
    return 1;
}

int _stricmp(const char* a, const char* b)
{
    return strcasecmp(a, b);
}

/* --- GDI --- */

static char PROMOCK_window[8];
static char PROMOCK_dc[8];
static char PROMOCK_brush[8];
static char PROMOCK_module[8];

BOOL GetClientRect(HWND hwnd, RECT* rect)
{
    PROMOCK_Record("GetClientRect", NULL, 1, (int)(hwnd != NULL));
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 400;
    return 1;
}

DWORD GetSysColor(int idx)
{
    PROMOCK_Record("GetSysColor", NULL, 1, idx);
    return 0x00C0C0C0;
}

COLORREF SetBkColor(HDC hdc, COLORREF color)
{
    PROMOCK_Record("SetBkColor", NULL, 2, (int)(hdc != NULL), (int)color);
    return 0;
}

COLORREF SetTextColor(HDC hdc, COLORREF color)
{
    PROMOCK_Record("SetTextColor", NULL, 2, (int)(hdc != NULL), (int)color);
    return 0;
}

HBRUSH CreateSolidBrush(COLORREF color)
{
    PROMOCK_Record("CreateSolidBrush", NULL, 1, (int)color);
    return PROMOCK_brush;
}

BOOL ExtTextOutA(HDC hdc, int x, int y, UINT opts, const RECT* rect,
                 LPCSTR str, UINT len, const int* dx)
{
    char buf[PROMOCK_TEXT];
    UINT n = len < PROMOCK_TEXT - 1 ? len : PROMOCK_TEXT - 1;

    memcpy(buf, str, n);
    buf[n] = 0;
    PROMOCK_Record("ExtTextOutA", buf, 4, x, y, (int)opts, (int)len);
    return 1;
}

int DrawTextA(HDC hdc, LPCSTR str, int len, RECT* rect, UINT fmt)
{
    char buf[PROMOCK_TEXT];
    int n = len < PROMOCK_TEXT - 1 ? len : PROMOCK_TEXT - 1;

    if (n < 0)
        n = 0;
    memcpy(buf, str, n);
    buf[n] = 0;
    PROMOCK_Record("DrawTextA", buf, 5, len, rect->left, rect->top,
                   rect->right, rect->bottom);
    return 1;
}

HDC GetDC(HWND hwnd)
{
    PROMOCK_Record("GetDC", NULL, 1, (int)(hwnd != NULL));
    return PROMOCK_dc;
}

int ReleaseDC(HWND hwnd, HDC hdc)
{
    PROMOCK_Record("ReleaseDC", NULL, 2, (int)(hwnd != NULL), (int)(hdc != NULL));
    return 1;
}

BOOL ShowWindow(HWND hwnd, int cmd)
{
    PROMOCK_Record("ShowWindow", NULL, 2, (int)(hwnd != NULL), cmd);
    return 1;
}

WORD RegisterClassA(const WNDCLASSA* wc)
{
    PROMOCK_Record("RegisterClassA", wc->lpszClassName, 2,
                   (int)wc->style, (int)(wc->lpfnWndProc != NULL));
    return 1;
}

HWND CreateWindowExA(DWORD exStyle, LPCSTR cls, LPCSTR name, DWORD style,
                     int x, int y, int w, int h, HWND parent, HMENU menu,
                     HINSTANCE inst, void* param)
{
    PROMOCK_Record("CreateWindowExA", cls, 7, (int)exStyle, (int)style, x, y, w, h,
                   (int)(parent != NULL));
    return PROMOCK_window;
}

HMODULE GetModuleHandleA(LPCSTR name)
{
    PROMOCK_Record("GetModuleHandleA", NULL, 1, (int)(name != NULL));
    return PROMOCK_module;
}

LONG DefWindowProcA(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    return 0;
}
