zlib 1.3.1, from https://github.com/madler/zlib (tag v1.3.1).

Here because libpng needs a deflate, and the only zlib already in the
tree is the inflate-only copy FreeType carries for compressed fonts.

Trimmed to the sources the library needs and their headers: no tests,
no build systems, no contrib, no documentation. `zconf.h` is the one
the project ships, not a generated one, so there is nothing to
configure. LICENSE is zlib's own.
