#pragma once

/* The Windows calls platform/glrender.c refers to, stood in for.
 *
 * Both of the OpenGL renderer's harnesses need these and neither
 * calls the parts of the renderer that would use them: one mocks
 * OpenGL outright, the other gets its context from EGL. What they
 * have in common is that no window is ever made, so every one of
 * these answers "nothing" or "it worked" and nothing looks at the
 * result.
 *
 * It is a header full of definitions rather than declarations, which
 * is deliberate but only works because each harness is one
 * translation unit -- platform/glrender.c is included rather than linked,
 * because its state is static to it. Including this twice in one
 * binary would be two definitions of each.
 *
 * It exists as one file because the two copies of it drifted. That is
 * the failure mode this repository has been bitten by more than once:
 * two copies of the same thing, edited in opposite directions, and
 * the one that was not edited is the one that gets built.
 *
 * GetModuleFileNameA is not here. Every harness wants its own answer
 * from it -- it is how datapath.c is pointed at the repository -- so
 * each defines that one itself.
 */

HDC      GetDC(HWND h) { (void)h; return NULL; }
int      ReleaseDC(HWND h, HDC d) { (void)h; (void)d; return 1; }
BOOL     DestroyWindow(HWND h) { (void)h; return 1; }
HMODULE  GetModuleHandleA(LPCSTR n) { (void)n; return NULL; }
HMODULE  LoadLibraryA(LPCSTR n) { (void)n; return NULL; }
FARPROC  GetProcAddress(HMODULE m, LPCSTR n) { (void)m; (void)n; return NULL; }
BOOL     UnregisterClassA(LPCSTR n, HINSTANCE i) { (void)n; (void)i; return 1; }
WORD     RegisterClassA(const WNDCLASSA* c) { (void)c; return 0; }
HWND     CreateWindowExA(DWORD a, LPCSTR b, LPCSTR c, DWORD d, int e, int f,
                         int g, int h, HWND i, HMENU j, HINSTANCE k, LPVOID l)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    (void)g; (void)h; (void)i; (void)j; (void)k; (void)l;
    return NULL;
}
LRESULT  DefWindowProcA(HWND a, UINT b, WPARAM c, LPARAM d)
{
    (void)a; (void)b; (void)c; (void)d;
    return 0;
}
BOOL     GetClientRect(HWND h, RECT* r) { (void)h; (void)r; return 0; }
int      ChoosePixelFormat(HDC d, const PIXELFORMATDESCRIPTOR* p)
{
    (void)d; (void)p; return 0;
}
BOOL     SetPixelFormat(HDC d, int f, const PIXELFORMATDESCRIPTOR* p)
{
    (void)d; (void)f; (void)p; return 0;
}
BOOL     SwapBuffers(HDC d) { (void)d; return 1; }
HGLRC    wglCreateContext(HDC d) { (void)d; return NULL; }
BOOL     wglMakeCurrent(HDC d, HGLRC c) { (void)d; (void)c; return 1; }
BOOL     wglDeleteContext(HGLRC c) { (void)c; return 1; }
void*    wglGetProcAddress(LPCSTR n) { (void)n; return NULL; }
