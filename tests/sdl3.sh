#!/bin/sh
# Builds the vendored SDL3 once, for the test harnesses to link against.
#
#   . ./tests/sdl3.sh        sets SDL3_INC and SDL3_LIB
#
# Sourced rather than run, so it must not cd anywhere: $0 is the calling
# shell when a script is sourced, and working the root out from it lands
# in "/". Every runner cds to the repository root before sourcing this,
# so the root is simply where we already are -- checked below rather
# than assumed, because the failure is otherwise a confusing one.
#
# The library is not committed, only its source, so the first run in a
# fresh container builds it and later runs find it. It takes a couple of
# minutes, and says so rather than appearing to hang.
#
# The build is headless on purpose: no X11, no Wayland, no ALSA, no
# PulseAudio. What is left is SDL's own dummy audio driver and dummy
# video driver, which is what a test wants -- a real SDL, with real
# streams and real events, writing to nothing. SDL's own error message
# names the switch that allows a build with no window system
# (SDL_UNIX_CONSOLE_BUILD), and this is the case it is for.

if [ ! -d thirdparty/SDL3/include ]; then
    echo "tests/sdl3.sh: run from the repository root" >&2
    return 1 2>/dev/null || exit 1
fi

SDL3_INC="$PWD/thirdparty/SDL3/include"
SDL3_LIB="$PWD/tests/build/sdl3/libSDL3.a"

if [ ! -f "$SDL3_LIB" ]; then
    echo "  building the vendored SDL3 once into tests/build/sdl3" \
         "(a couple of minutes)"
    mkdir -p tests/build
    cmake -S "$PWD/thirdparty/SDL3" -B "$PWD/tests/build/sdl3" -G Ninja \
          -DCMAKE_BUILD_TYPE=Release \
          -DSDL_SHARED=OFF -DSDL_STATIC=ON \
          -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF \
          -DSDL_UNIX_CONSOLE_BUILD=ON \
          -DSDL_X11=OFF -DSDL_WAYLAND=OFF \
          -DSDL_ALSA=OFF -DSDL_PULSEAUDIO=OFF -DSDL_PIPEWIRE=OFF \
          -DSDL_JACK=OFF -DSDL_SNDIO=OFF -DSDL_OSS=OFF \
          -DSDL_VULKAN=OFF -DSDL_RPATH=OFF \
          > tests/build/sdl3.cmake.log 2>&1 \
      && cmake --build "$PWD/tests/build/sdl3" -j"$(nproc)" \
          >> tests/build/sdl3.cmake.log 2>&1
    if [ ! -f "$SDL3_LIB" ]; then
        echo "  SDL3 did not build -- see tests/build/sdl3.cmake.log" >&2
        return 1 2>/dev/null || exit 1
    fi
fi

export SDL3_INC SDL3_LIB
