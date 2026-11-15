#pragma once

/*
    Where the game's data files live, read from an ini file in the
    working directory rather than compiled in.

    The file is optional. When it is not there, or cannot be read,
    nothing is set from it and the game runs on its command line
    alone -- that is not an error and is not reported as one.

    Command line parameters take precedence. The settings here are
    applied before the command line is parsed, so anything given on
    the command line is what ends up in force.

        ; datafiles.ini
        [data]
        bigfile   = sally_clean.bf
        binarized = 1

        [video]
        0 = BGEMakingOf.bik

    Under [video] the key is the id the script passes to the
    fullscreen video function and the value is the file to play. An id
    with no entry plays nothing.
*/

#define DAT_INIFILE   "datafiles.ini"

/* Long enough for MAI_gst_BIGFileName, which is the longest thing
   read out of here. */
#define DAT_NAMEMAX   288

/* Reads DAT_INIFILE from the working directory. Safe to call when
   there is no such file. */
void DAT_InitModule(void);

/* The big file the ini names, or NULL if it named none. */
const char* DAT_pc_BigFile(void);

/* Non-zero when the ini asked for binarized world files. */
int DAT_b_Binarized(void);

/* The video file for an id, or NULL when the ini has no entry for
   it. The caller decides what to do about that; this module does not
   treat it as an error. */
const char* DAT_pc_Video(int id);
