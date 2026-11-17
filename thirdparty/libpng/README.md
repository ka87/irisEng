libpng 1.6.44, from https://github.com/pnggroup/libpng (tag v1.6.44).

Trimmed to the sources and headers the library needs: no tests, no
build systems, no contrib, no documentation, and none of the ancillary
tools.

`pnglibconf.h` is `scripts/pnglibconf.h.prebuilt` copied into place,
which is what libpng ships for builds that do not run its configure --
the same thing its own Visual Studio and CMake builds do.

Note that libpng has a `pngwrite.c` of its own. The engine's own
screenshot writer is `pngsave.c` at the root of the tree, named apart
from it because Visual Studio puts every object in one directory and
two sources of the same name collide there.

LICENSE is libpng's own; AUTHORS is kept with it.
