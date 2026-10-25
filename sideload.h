#pragma once
#include "common.h"

/*
    Sideloading: read a key's file from a directory next to the exe
    instead of from the .bf, and run it through the interpreter even
    where the behaviour has been transcribed into C.

    This is a development aid for checking a recovered script. A .ofc
    lifted back out of the retail exe can be dropped in, run, and
    watched, without rebuilding the archive and without touching
    AI2C_gat_fctdefs.

        <exe dir>\sideload\
            5E006324.ofc        a behaviour, by key
            5E006325.ova        a variable file, by key
            5E006326.omd        a model, by key
            5E006327.oin        an instance, by key
            interpret.txt       keys to interpret from the .bf anyway

    All four AI file types work the same way. A .ova is whichever
    variable file carries that key, so the standard block a model
    points at and the override block an instance points at are both
    replaceable; a .omd replaces the list of parts a model is built
    from, and a .oin the pair of keys an instance names.

    The name before the extension is eight hex digits, optionally
    0x-prefixed; the extension is ignored, so whatever the file is
    called locally works as long as the key is right. A name that is
    not a key is skipped, so notes and spare copies can live in the
    same folder.

    interpret.txt holds one key per line for behaviours that should be
    interpreted from the archive's own nodes rather than run as C,
    without supplying a replacement file. Blank lines and lines
    starting with # or // are ignored.

    Nothing here is reached unless the directory exists, so a normal
    install behaves exactly as before.
*/

extern int SIDE_gb_Enabled;
extern int SIDE_gi_Count;

void SIDE_InitModule(void);
void SIDE_CloseModule(void);

/* TRUE when this key must bypass the compiled-in C: either a file was
   supplied for it or it is listed in interpret.txt. */
BOOL SIDE_b_Overrides(uint32_t key);

/* The sideloaded contents of a key, freshly allocated, or NULL when
   nothing was supplied for it. The caller frees it with SIDE_Free. */
char* SIDE_pc_ReadByKey(uint32_t key, int* sizeRet);

void SIDE_Free(char* buffer);
