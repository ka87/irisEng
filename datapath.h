#pragma once
#include <stddef.h>

/* Where the install's data lives.

   Beside the executable, under data\<folder>\, which is also where the
   sideload folder is -- not the working directory. A game started from
   a shortcut has whatever working directory the shortcut says, and the
   data is part of the install.

   One copy of this, because there are two callers now: font.c wants
   data\fonts and glshader.c wants data\shaders, and two copies of a
   path rule drift in opposite directions. */

/* Writes <exe directory>\data\<folder>\<name> into out.

   Returns 0 and leaves nothing usable behind if the executable's own
   path cannot be had, or if the result would not fit -- truncation is
   reported rather than becoming a short path that fails to open for a
   reason nobody can see.

   The separator is whatever separated the executable's own path rather
   than an assumed backslash. Windows accepts either, and taking it
   from what came back is what lets a test drive this on a host that
   has only the other. */
int DPT_b_DataPath(const char* folder, const char* name, char* out, size_t n);
