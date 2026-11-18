#pragma once

/*
    Where the game's data files live and how the display is set up,
    read from an ini file beside the executable rather than compiled
    in or kept in the registry.

    The file is optional. When it is not there, or cannot be read,
    nothing is set from it and the game runs on its command line
    alone -- that is not an error and is not reported as one.

    Command line parameters take precedence. The settings here are
    applied before the command line is parsed, so anything given on
    the command line is what ends up in force.

        ; engine.ini
        [data]
        bigfile      = sally_clean.bf
        specialarray = jade.spe
        binarized    = 1

        [display]
        adapter    = 0
        mode       = 0
        zbuffer    = 24
        mipmapping = 1
        windowed   = 0
        width      = 640
        height     = 480

        [video]
        0x3001978F = BGEMakingOf.bik

    Under [video] the key is the id the script passes to the
    fullscreen video function and the value is the file to play. What
    is pushed is a 32-bit resource key rather than a small index, so
    an id may be written in hex with a 0x prefix or in decimal, and
    the whole 32-bit range is matched. An id with no entry plays
    nothing.
*/

#define SET_INIFILE   "engine.ini"

/* Long enough for MAI_gst_BIGFileName, which is the longest thing
   read out of here. */
#define SET_NAMEMAX   288

/* Reads SET_INIFILE from beside the executable. Safe to call when
   there is no such file. */
void SET_InitModule(void);

/* The big file the ini names, or NULL if it named none. */
const char* SET_pc_BigFile(void);

/* The special array's file, which is jade.spe unless the ini says
   otherwise. Never NULL: the default is what the game has always
   opened. */
const char* SET_pc_SpecialArray(void);

/* Non-zero when the ini asked for binarized world files. */
int SET_b_Binarized(void);

/* The renderer the ini asked for, by name, or NULL if it named none.
   What the names are is the renderer's business, not this module's:
   it carries the string across and gdi.c decides what it means. */
const char* SET_pc_Backend(void);

/* The video file for an id, or NULL when the ini has no entry for
   it. The caller decides what to do about that; this module does not
   treat it as an error. */
const char* SET_pc_Video(int id);

/* What [display] says, which is what the registry used to hold.
 *
 * Each of these answers whether the ini said anything at all, and
 * writes the value only when it did -- so a caller keeps whatever
 * default it already had for a setting the player has not set. That
 * is how the registry read behaved: a missing value left the default
 * standing rather than zeroing it.
 *
 * windowed is the engine's flag rather than a property of the window:
 * it is what the renderer reads when it decides whether to take an
 * exclusive display mode.
 */
int SET_b_DisplayAdapter(int* out);
int SET_b_DisplayMode(int* out);
int SET_b_DisplayZBuffer(int* out);
int SET_b_DisplayMipMapping(int* out);
int SET_b_DisplayWindowed(int* out);
int SET_b_DisplayWidth(int* out);
int SET_b_DisplayHeight(int* out);
