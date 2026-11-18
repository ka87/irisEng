#pragma once

/* The system calls platform/glrender.c refers to, stood in for.
 *
 * Both of the OpenGL renderer's harnesses need these and neither
 * calls the parts of the renderer that would use them: one mocks
 * OpenGL outright, the other gets its context from EGL. What they
 * have in common is that no window is ever made, so every one of
 * these answers "nothing" or "it worked" and nothing looks at the
 * result.
 *
 * It was a list of Windows calls -- a window class, a throwaway
 * window, a device context, a pixel format and the four WGL entry
 * points -- because that is what making a context on Windows takes.
 * The renderer asks SDL now, which is a shorter list. Each of the
 * eleven that went was taken out on its own and the two harnesses
 * relinked, so what is left is what is still referred to rather than
 * what looked unused.
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

#include <SDL3/SDL_video.h>

/* The window and the context, which the renderer asks SDL for now
   rather than Windows. Neither harness makes one: one mocks OpenGL
   outright and the other gets its context from EGL, so these answer
   "nothing" or "it worked" exactly as the Windows pair above do, and
   the renderer's own entry points that would use them are not
   reached.

   The real headers are included by the renderer, so a prototype that
   drifts from SDL's is a compile error here rather than a stub that
   quietly stands in for a different function. */
SDL_Window* SDL_CreateWindow(const char* title, int w, int h,
                             SDL_WindowFlags flags)
{
    (void)title; (void)w; (void)h; (void)flags;
    return NULL;
}
void     SDL_DestroyWindow(SDL_Window* w) { (void)w; }
bool     SDL_GetWindowSizeInPixels(SDL_Window* w, int* x, int* y)
{
    (void)w; (void)x; (void)y; return false;
}
bool     SDL_GL_SetAttribute(SDL_GLAttr attr, int value)
{
    (void)attr; (void)value; return true;
}
SDL_GLContext SDL_GL_CreateContext(SDL_Window* w) { (void)w; return NULL; }
bool     SDL_GL_MakeCurrent(SDL_Window* w, SDL_GLContext c)
{
    (void)w; (void)c; return true;
}
bool     SDL_GL_DestroyContext(SDL_GLContext c) { (void)c; return true; }
bool     SDL_GL_SwapWindow(SDL_Window* w) { (void)w; return true; }
SDL_FunctionPointer SDL_GL_GetProcAddress(const char* name)
{
    (void)name; return NULL;
}
const char* SDL_GetError(void) { return "no error, this is a test"; }

/* The window the renderer asks the platform layer for. Null, which is
   what makes GL_Init decline -- and neither harness calls it. */
void*    PLT_p_Window(void) { return NULL; }
