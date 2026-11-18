#pragma once
#include "glcore.h"

/* Filling in the function pointers in glcore.h, and deciding whether
   the context that handed them over is good enough to draw with.

   Nothing here calls Windows. The two ways of resolving a name are
   passed in, which is what lets the awkward parts -- the sentinel
   values a failed lookup comes back as, the two-stage lookup, the
   shape of a version string -- be driven directly rather than
   inferred from a machine that happens to be in front of us. */

/* How a name is looked up. Returns NULL when it cannot be found. */
typedef void* (*GL_pfn_GetProc)(const char* name);

/* Resolves one name, through the context first and the module
   second.

   Both are needed and neither is enough. A driver's
   wglGetProcAddress answers for everything its context added past
   OpenGL 1.1 and is entitled to refuse the 1.1 entry points, which
   opengl32.dll exports itself; asking the module alone gets those and
   nothing newer.

   It also screens the sentinels. wglGetProcAddress reports failure as
   NULL on most drivers but is documented to be allowed 1, 2, 3 and
   -1 as well, and those have been seen in the wild. Taken at face
   value they are a function pointer that crashes on the first call,
   somewhere far from here. */
void* GL_p_Resolve(const char* name, GL_pfn_GetProc fromContext,
                   GL_pfn_GetProc fromModule);

/* Fills every pointer in glcore.h's list.

   Returns the number that could not be resolved, so zero is success,
   and writes the first missing name into `missing` when that is not
   NULL -- one name is what a log line wants, and a driver short of
   one entry point is usually short of a family. */
int GL_i_LoadFunctions(GL_pfn_GetProc fromContext, GL_pfn_GetProc fromModule,
                       char* missing, size_t missingSize);

/* Clears every pointer. Not for tidiness: a context that went away
   leaves pointers that belong to it, and calling one of those is
   worse than finding a NULL. */
void GL_ForgetFunctions(void);

/* Whether a GL_VERSION string is at least the version asked for.

   The string starts with "major.minor" and may carry a release
   number and a vendor's own text after it -- "4.6.0 NVIDIA 551.23",
   "4.5 (Core Profile) Mesa 23.2.1". An OpenGL ES string is reported
   as not good enough whatever its number, because it says
   "OpenGL ES " first and is not the same API. */
int GL_b_VersionAtLeast(const char* version, int major, int minor);
