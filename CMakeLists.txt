# irisEng, built with CMake.
#
# The engine builds and runs as a 32-bit program. The check below
# refuses a 64-bit configure, and its own comment says exactly what
# that refusal does and does not rest on -- the short version is that
# the pointer casts are nearly gone and the file formats are the
# reason, not them.
#
# What this links today is the Windows game. Off Windows it builds
# every engine source but the Direct3D renderer's six, which are left
# out at the foot of this file, and draws through OpenGL instead. The
# window, the input, the paths, the file reads and the clocks had all
# left Windows already; what remained was that gdi.c filled the
# renderer table with Direct3D entries itself, and that is in
# platform/d3drow.c now.
#
#   cmake -S . -B build -A Win32            Visual Studio, 32-bit
#   cmake --build build --config Release
#
cmake_minimum_required(VERSION 3.21)
project(irisEng C)

# ----------------------------------------------------------- debug info
#
# An empty CMAKE_BUILD_TYPE on a single-config generator means no -g and
# no optimisation flags at all, which is the worst of both: slow and
# impossible to debug. Default to Debug instead. Pass
# -DCMAKE_BUILD_TYPE=RelWithDebInfo (or Release) to override.
get_property(IRIS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT IRIS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Debug, Release, RelWithDebInfo or MinSizeRel" FORCE)
    message(STATUS "irisEng: no build type given, defaulting to Debug")
endif()

# ---------------------------------------------------------------- 32-bit
#
# This refuses a 64-bit configure, and it is worth being exact about
# why, because the reason has changed and the old one is no longer the
# obstacle.
#
# What a 64-bit port has established, and the audit that says so:
#
#   - all 181 engine sources compile at -m64, none failing;
#   - the 181 objects relocatably link together, with nothing
#     engine-internal unresolved (tests/run_wholelink.sh);
#   - 12 pointer/int cast sites are left in those sources, and every
#     one of them is an internal 32-bit slot that wants a wider
#     declaration. None needs an address-translation layer. Six of
#     the twelve are a convention of the generated compiled-script C
#     rather than the engine's, two are in the Direct3D renderer that
#     is not compiled off Windows, and three are unreachable from
#     anything the project file builds.
#
# What it has not established, and this is the refusal:
#
#   - 230 of the 409 structs with members change size between 32 and
#     64 bits, and 65 places in 14 files advance a load cursor by the
#     sizeof of a struct. Those strides are file formats. A struct
#     that gains a pointer silently changes how a world file is read,
#     and the four-byte slots inside those files -- a variable block's
#     refs, a node's operand -- are fixed on disk and cannot widen
#     with the pointer. That is the work, and it is not a compiler
#     flag.
#   - nothing has been linked into a running program at 64 bits, let
#     alone run.
#
# So: the casts are no longer the reason, the formats are. Until a
# 64-bit build has been run, this refuses rather than letting it be
# discovered at run time.
#
# IRIS_CONFIGURE_ONLY_64 configures 64-bit anyway. It is for looking at
# this file's own workings where there is no 32-bit toolchain to hand
# -- it was how the source list and the library targets below were
# checked -- and it does not make a 64-bit build work.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(IRIS_CONFIGURE_ONLY_64)
        message(WARNING
            "Configuring 64-bit because IRIS_CONFIGURE_ONLY_64 is set. "
            "The sources compile at this width, but the on-disk formats "
            "are still read by striding over structs that change size, "
            "so anything this builds must not be run. This is for "
            "inspecting the configuration and nothing else.")
    else()
        message(FATAL_ERROR
            "irisEng builds 32-bit. The sources compile at 64 bits, but "
            "the world-file formats are read by striding over structs "
            "that change size there, and no 64-bit build has been run. "
            "This file's 32-bit section says what that rests on.\n"
            "  Visual Studio:  cmake -S . -B build -A Win32\n"
            "  Linux:          cmake -S . -B build --toolchain "
            "cmake/linux-i386.cmake\n"
            "\n"
            "To configure 64-bit anyway, for inspection and not for "
            "running, pass -DIRIS_CONFIGURE_ONLY_64=ON.\n"
            "\n"
            "Passing -m32 by hand instead moves the compiler and leaves "
            "CMake's library search and pkg-config on the 64-bit paths, "
            "which shows up as a library that cannot be found with the "
            "file in plain sight. The toolchain file above sets all "
            "three together; its header says what each one is for and "
            "which packages have to be the i386 ones.")
    endif()
endif()

# ------------------------------------------------------- what to compile
#
# The file list comes out of irisEng.vcxproj rather than being written
# again here. That is deliberate: the project file is already the one
# the tests check themselves against -- tests/run.sh fails if a source
# the tests build is missing from it -- so making it the source of
# truth for this build too means the two cannot drift apart. A source
# added to the project is in this build the moment it is added.

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/irisEng.vcxproj" IRIS_VCXPROJ)
string(REGEX MATCHALL "ClCompile Include=\"[^\"]+\\.c\"" IRIS_ENTRIES "${IRIS_VCXPROJ}")

set(IRIS_ENGINE_SOURCES "")
set(IRIS_FREETYPE_SOURCES "")
set(IRIS_VENDOR_SOURCES "")
foreach(entry IN LISTS IRIS_ENTRIES)
    string(REGEX REPLACE "ClCompile Include=\"([^\"]+)\"" "\\1" one "${entry}")
    string(REPLACE "\\" "/" one "${one}")
    if(one MATCHES "^thirdparty/freetype/")
        list(APPEND IRIS_FREETYPE_SOURCES "${one}")
    elseif(one MATCHES "^thirdparty/")
        list(APPEND IRIS_VENDOR_SOURCES "${one}")
    else()
        list(APPEND IRIS_ENGINE_SOURCES "${one}")
    endif()
endforeach()

# The Direct3D renderer, which only a machine with Direct3D can build.
# Everything else in the engine is portable now, including gdi.c, which
# holds the table these fill a row of: the row itself is in
# platform/d3drow.c and goes with them. What is left draws through the
# OpenGL renderer.
if(NOT WIN32)
    set(IRIS_D3D_SOURCES
        "platform/d3drender.c"
        "platform/d3daftereffects.c"
        "platform/d3dsun.c"
        "platform/d3dtex.c"
        "platform/d3dvideo.c"
        "platform/d3drow.c")
    foreach(one IN LISTS IRIS_D3D_SOURCES)
        list(FIND IRIS_ENGINE_SOURCES "${one}" IRIS_FOUND_AT)
        if(IRIS_FOUND_AT EQUAL -1)
            message(FATAL_ERROR
                "${one} is not in irisEng.vcxproj, so this build cannot "
                "be leaving it out -- the list above has gone stale")
        endif()
        list(REMOVE_ITEM IRIS_ENGINE_SOURCES "${one}")
    endforeach()
endif()

list(LENGTH IRIS_ENGINE_SOURCES IRIS_ENGINE_COUNT)
if(IRIS_ENGINE_COUNT LESS 100)
    message(FATAL_ERROR
        "only ${IRIS_ENGINE_COUNT} engine sources were read out of "
        "irisEng.vcxproj, which cannot be right -- the project file has "
        "moved on and the regular expression above has not")
endif()
message(STATUS "irisEng: ${IRIS_ENGINE_COUNT} engine sources from irisEng.vcxproj")

foreach(one IN LISTS IRIS_ENGINE_SOURCES IRIS_FREETYPE_SOURCES IRIS_VENDOR_SOURCES)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${one}")
        message(FATAL_ERROR "irisEng.vcxproj names ${one}, which is not there")
    endif()
endforeach()

# ------------------------------------------------------------------ SDL3
#
# Vendored and built from source, so the game and the engine agree on
# which SDL they are talking about. Static, because the game ships as
# one executable.

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/SDL3 EXCLUDE_FROM_ALL)

# -------------------------------------------------------------- FreeType
#
# The thirteen amalgamations the project compiles, each of which pulls
# in its own siblings. They need two defines of their own and none of
# the engine's headers, which is why they are a target rather than part
# of the engine's.

add_library(irisEngFreeType STATIC ${IRIS_FREETYPE_SOURCES})
target_compile_definitions(irisEngFreeType PRIVATE
    FT2_BUILD_LIBRARY
    "FT_CONFIG_MODULES_H=<ftmodule.h>")
target_include_directories(irisEngFreeType PRIVATE
    thirdparty/freetype/iriseng
    thirdparty/freetype/include)
if(MSVC)
    target_compile_options(irisEngFreeType PRIVATE /w)
else()
    target_compile_options(irisEngFreeType PRIVATE -w)
endif()

# --------------------------------------------- libpng, zlib and jpeg-6b
#
# Upstream source, built with warnings off for the same reason: it is
# not ours to tidy and its warnings would bury the engine's.

# One of these is not upstream: thirdparty/jpeg-6b/jmemjade.c is
# jpeg's memory manager written against this engine's allocator, and
# it reaches back up into common.h for it. It builds with the rest
# because that is where the project keeps it, and it is the only file
# here that the warnings-off below is hiding engine code from.
add_library(irisEngVendor STATIC ${IRIS_VENDOR_SOURCES})
target_include_directories(irisEngVendor PRIVATE
    thirdparty/zlib thirdparty/libpng thirdparty/jpeg-6b)
if(MSVC)
    target_compile_options(irisEngVendor PRIVATE /w)
else()
    target_compile_options(irisEngVendor PRIVATE -w)
endif()

# ---------------------------------------------------------------- ffmpeg
#
# Two sources, because there is only one of them to be had on each
# side. The Windows libraries are in the tree, prebuilt, with the
# headers they belong to; off Windows the distribution's ffmpeg is
# used, headers and all.
#
# The two are not the same version and the headers have to travel with
# the libraries, so the vendored include directory is Windows' alone.
# Mixing them -- this tree's headers against a distribution's
# libraries -- would compile and then misbehave at run time, which is
# the worst of the three outcomes.
#
# What that costs is that video.c has to build against whatever
# version is installed. Its calls are the AVChannelLayout generation
# of the API and nothing newer, so the floor below is where it sits;
# a version under it fails here rather than at the first unresolved
# symbol.

add_library(irisEngFFmpeg INTERFACE)

if(WIN32)
    target_include_directories(irisEngFFmpeg INTERFACE thirdparty/ffmpeg/include)
    # These libraries are not built by this project and not built by
    # MSVC, and a good deal of them is hand-written assembly, so their
    # objects carry no safe exception handler table. On x86 the linker
    # marks the image as having one and then refuses any object that
    # cannot contribute to it:
    #
    #   swresample.lib(...) : error LNK2026: module unsafe for SAFESEH image
    #
    # The table is image-wide, so the only answer is to not ask for one.
    # It goes here, on the libraries that need it, rather than on the
    # engine, and only where it means anything: the option is x86's.
    #
    # It only ever showed up in Release because the project file's
    # Release|Win32 link turns on /OPT:REF and /OPT:ICF, which rule out
    # an incremental link -- and an incremental link, which is what
    # Debug gets, builds no table to fail in the first place. So this
    # goes on every configuration rather than one.
    if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_link_options(irisEngFFmpeg INTERFACE /SAFESEH:NO)
    endif()
    foreach(lib avcodec avformat avutil swscale swresample)
        if(MSVC)
            target_link_libraries(irisEngFFmpeg INTERFACE
                "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ffmpeg/lib/${lib}.lib")
        else()
            target_link_libraries(irisEngFFmpeg INTERFACE
                "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ffmpeg/lib/lib${lib}.dll.a")
        endif()
    endforeach()
else()
    find_package(PkgConfig REQUIRED)
    # libavutil 57.28 is ffmpeg 5.1, which is where AVChannelLayout and
    # swr_alloc_set_opts2 arrive. video.c uses both.
    pkg_check_modules(IRIS_FFMPEG IMPORTED_TARGET
        libavcodec libavformat "libavutil>=57.28" libswscale libswresample)
    if(NOT IRIS_FFMPEG_FOUND)
        message(FATAL_ERROR
            "ffmpeg's development files were not found for this "
            "architecture. On a 32-bit build of a 64-bit Debian or "
            "Ubuntu they are the i386 ones, and the runtime packages "
            "are not enough -- the .pc files and the headers come "
            "with -dev:\n"
            "  sudo apt install libavcodec-dev:i386 "
            "libavformat-dev:i386 libavutil-dev:i386 "
            "libswscale-dev:i386 libswresample-dev:i386\n"
            "\n"
            "Build with --toolchain cmake/linux-i386.cmake, which "
            "points pkg-config at the i386 directory. Without it the "
            "64-bit ones are found here and the link fails later. "
            "ffmpeg 5.1 or newer is needed, for AVChannelLayout.")
    endif()
    target_link_libraries(irisEngFFmpeg INTERFACE PkgConfig::IRIS_FFMPEG)
    message(STATUS "ffmpeg: the distribution's, libavutil ${IRIS_FFMPEG_libavutil_VERSION}")
endif()

# ------------------------------------------------------------ the engine

add_executable(irisEng WIN32 ${IRIS_ENGINE_SOURCES})

# The source directory is deliberately NOT here, and the project file
# does not have it either. The engine keeps a header called strings.h,
# which is a name POSIX owns: with the source directory on the include
# path, the C library's own string.h -- which includes <strings.h> --
# reaches the engine's instead, and the engine's reaches back into
# common.h while common.h is still being read. Every engine include is
# quoted and relative, so none of them needs this.
# ffmpeg is not here: its headers come with its libraries, through
# irisEngFFmpeg, because which ones they are depends on the platform.
target_include_directories(irisEng PRIVATE
    thirdparty/SDL3/include
    thirdparty/freetype/include
    thirdparty/zlib
    thirdparty/libpng
    thirdparty/jpeg-6b)

target_link_libraries(irisEng PRIVATE
    SDL3::SDL3-static
    irisEngFreeType
    irisEngVendor
    irisEngFFmpeg)

if(WIN32)
    # The two renderers and the sound: Direct3D 9, OpenGL and the
    # multimedia timer. SDL brings its own.
    target_link_libraries(irisEng PRIVATE winmm dxguid d3d9 opengl32)
    target_compile_definitions(irisEng PRIVATE
        _CRT_SECURE_NO_WARNINGS WIN32 _CONSOLE)
else()
    # The big file is bigger than two gigabytes, and this is a 32-bit
    # build: without this, off_t is 32 bits wide and the wide seek in
    # fileio.c would reach the first two gigabytes of the file and
    # fail past them. fileio.c refuses to compile without it rather
    # than let that happen.
    target_compile_definitions(irisEng PRIVATE _FILE_OFFSET_BITS=64)

    # OpenGL needs no library named here: every entry point this
    # renderer uses is resolved at run time through SDL, so there is
    # no -lGL and no -ldl to add.

    message(STATUS
        "This is not a Windows build: the Direct3D renderer is left "
        "out and the game draws through OpenGL. Every other engine "
        "source is built, and nothing in them refers to the renderer "
        "that was left out -- its row in the table is in "
        "platform/d3drow.c, which goes with it.")
endif()

if(MSVC)
    # /W3 is what the warning sweep in tests/ is written against.
    target_compile_options(irisEng PRIVATE /W3)
else()
    target_compile_options(irisEng PRIVATE -fms-extensions)
endif()

# Symbols and frame pointers in every configuration, so a crash in a
# Release build still produces a usable backtrace. -g does not change
# the generated code, only what is kept beside it.
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(irisEng PRIVATE -g -fno-omit-frame-pointer)
elseif(MSVC)
    target_compile_options(irisEng PRIVATE /Zi)
    target_link_options(irisEng PRIVATE /DEBUG)
endif()

# The memory debugging build. The engine carves everything out of one
# pool, where AddressSanitizer sees a single 40 MB block and nothing in
# it; with this on, memory.c hands every allocation to malloc instead
# (memdebug.inc), so an overrun or a use after free is reported where it
# happens. -fsanitize-recover lets a run report everything rather than
# stop at the first. Use its own build directory: the binary it writes
# is the same ./irisEng.
option(IRIS_MEM_DEBUG "Replace the pool allocator with malloc under AddressSanitizer" OFF)
if(IRIS_MEM_DEBUG)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "IRIS_MEM_DEBUG needs GCC or Clang's AddressSanitizer")
    endif()
    target_compile_definitions(irisEng PRIVATE IRIS_MEM_DEBUG)
    target_compile_options(irisEng PRIVATE
        -fsanitize=address -fsanitize-recover=address)
    target_link_options(irisEng PRIVATE -fsanitize=address)
    message(STATUS "irisEng: memory debugging build (pool replaced, AddressSanitizer on)")
endif()

# The engine declares its globals in headers without extern. In C that
# is a tentative definition, so every file that includes one gets
# storage of its own and the linker is expected to merge them -- which
# is what it did until GCC 10 and Clang 11 made -fno-common the
# default. Today it is 545 symbols and 46,543 definitions of them;
# engine.h alone puts 157 copies of each of its globals into the
# build, one per file that includes it.
#
# -fcommon asks for the old behaviour so the engine links. It is a
# crutch: the fix is extern in the header and one definition in a
# file, which is what aemaster.h and platform.h already do for the
# ones that have been through here. The numbers above are what the
# work should be measured against.
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(irisEng PRIVATE -fcommon)
endif()

set_target_properties(irisEng PROPERTIES
    C_STANDARD 11
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
