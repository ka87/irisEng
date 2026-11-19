# A 32-bit build on a 64-bit Linux machine.
#
#   cmake -S . -B build --toolchain cmake/linux-i386.cmake
#
# The engine is 32-bit only -- it stores pointers in ints in several
# places -- so this is how it is built anywhere that is not Windows.
#
# -m32 on its own is not enough, and the ways it is not enough all
# look like a missing library rather than a misconfigured build:
#
# * find_library does not know the compiler changed. It searches
#   /usr/lib/<arch>, and <arch> comes from CMAKE_LIBRARY_ARCHITECTURE,
#   which is the 64-bit triplet unless it is told otherwise. SDL's
#   ALSA check is the first to hit this: the header is found, because
#   headers are shared between the two architectures, and the library
#   is NOTFOUND with the file in plain sight.
#
# * pkg-config does not know either. PKG_CONFIG_LIBDIR defaults to the
#   64-bit directory, so a backend found that way hands back 64-bit
#   flags that fail at the link instead of at the probe.
#
# * A library that is found in both places is found in the wrong one
#   first, which also fails at the link -- "skipping incompatible".
#
# One more thing this cannot do for you: the development packages have
# to be the i386 ones. The runtime package ships libfoo.so.N and the
# dev package adds the libfoo.so symlink that find_library looks for,
# so libasound2:i386 is not enough and libasound2-dev:i386 is.

set(CMAKE_C_FLAGS_INIT             "-m32")
set(CMAKE_CXX_FLAGS_INIT           "-m32")
set(CMAKE_EXE_LINKER_FLAGS_INIT    "-m32")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-m32")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-m32")

# What find_library appends to /usr/lib and friends.
set(CMAKE_LIBRARY_ARCHITECTURE i386-linux-gnu)

# And searched ahead of the system paths, so the right copy wins even
# where the triplet above has not been taken up. /usr/lib32 is for the
# distributions that use it instead of a multiarch directory.
list(APPEND CMAKE_LIBRARY_PATH /usr/lib/i386-linux-gnu /usr/lib32)

# Nothing 64-bit can go into this binary, so finding one is never
# useful: better a clean "not found" at configure time than a link
# that fails with "skipping incompatible". This covers the package
# config files that live under the same directory as well.
list(APPEND CMAKE_IGNORE_PATH /usr/lib/x86_64-linux-gnu)

set(ENV{PKG_CONFIG_LIBDIR} "/usr/lib/i386-linux-gnu/pkgconfig:/usr/share/pkgconfig")
