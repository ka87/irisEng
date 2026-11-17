libjpeg 6b (the Independent JPEG Group's release of 27 March 1998).

It encodes the savegame photo and decodes the JPEGs the game loads;
`textures.c` and `ai/ai_viewfunctions.c` are its two callers. The
folder keeps its upstream name so that a file in it can be matched
against the release it came from.

Unlike zlib and libpng beside it, this is the **whole upstream
distribution**, untrimmed: the 1998 build systems, the `cjpeg`,
`djpeg`, `jpegtran`, `rdjpgcom` and `wrjpgcom` command-line tools, the
documentation and the test images are all still here. Only the 46
sources the project compiles are in `irisEng.vcxproj`; the rest is
carried, not built.

Two files are not upstream's:

- **`jmemjade.c`**, an added back end for the library's
  system-dependent memory manager, and the one of the `jmem*.c` set
  the project compiles. It is upstream's `jmemansi.c` with three
  changes: `malloc` and `free` become the engine's `MEM_p_Alloc` and
  `MEM_Free`, `jpeg_mem_available` returns whatever is asked for, and
  the backing-store routines are commented out -- the second being
  what makes the third safe, since a library that is never short of
  memory never opens a temporary file.
- **`jconfig.h`**, which is upstream's `jconfig.vc` -- its own header
  for Visual C++ -- to the line, with `_CRT_SECURE_NO_WARNINGS` taken
  out, because
  the project file defines it for every configuration and a second
  definition is a redefinition in each of the 46 sources that include
  this. The comment at the top of the file says so.

`README` and `libjpeg.doc` are the library's own, and the conditions of
distribution and use are in `README`.
