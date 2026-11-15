#pragma once

/* A real OpenGL 4.5 core context with no window and no display, for
   the tests that want to see what the shaders actually compute.
   Mesa's software renderer answers for it.
 *
 * Why this exists: every other test of the OpenGL renderer aims the
 * GL function pointers at mocks, which is what makes them fast and
 * what lets them say exactly what was asked for. What a mock cannot
 * say is what came out -- the shader files are loaded and their
 * uniform names checked, and then the arithmetic inside them is
 * nobody's business. Three deliberate breakages of geometry.frag went
 * through that harness untouched, which is the reason for this one.
 *
 * There are no EGL headers on the machines this is built on, so the
 * dozen entry points it needs are declared here and looked up out of
 * libEGL.so.1 -- the same thing win/glload.c does for OpenGL itself,
 * for the same reason.
 *
 * It is allowed to be unavailable. A machine with no libEGL, no
 * software renderer or no 4.5 core profile is an ordinary machine, and
 * a test that cannot run there says so and passes rather than failing
 * for a reason that has nothing to do with the code.
 */

/* Brings up the context and makes it current. Returns 1 on success.
   On failure it returns 0 and leaves a sentence in EGLH_why saying
   which step would not go, which is what the caller prints. */
int EGLH_b_Open(void);

/* Gives the context back. Safe to call when it was never opened. */
void EGLH_Close(void);

/* Why EGLH_b_Open said no. Empty before the first call. */
extern const char* EGLH_why;

/* What the driver answered for, so the test can say which one it is
   talking to. NULL until the context is open. */
extern const char* EGLH_version;
extern const char* EGLH_renderer;
extern const char* EGLH_glsl;

/* One entry point out of the live context, by name, for the test's own
   use -- glReadPixels and the like, which the renderer does not call
   and so are not in glcore.h's list. NULL if the driver has no such
   name. */
void* EGLH_p_Proc(const char* name);
