/* Mocks for the soundmodifier.c differential test.
 *
 * soundmodifier.c sits between the game's sound modifier records and
 * sound.c's instance and sound tables. Everything below it -- the
 * allocator, the big-file reader, the loader's byte stream, the debug
 * assert -- is replaced here with a copy that logs every call and
 * answers deterministically. sound.c itself is not linked: the twenty-odd
 * entry points soundmodifier.c reaches it through are stood up in
 * soundmod_side.c instead, which is compiled once per side so that each
 * side drives its own copy of SND_gst_Params.
 *
 * Everything in here is shared by the two sides and rewound between them
 * by sm_reset(), so both runs see identical addresses and an identical
 * call sequence when they behave identically.
 */
#pragma once

#include "../sound.h"

/* ---- the call log --------------------------------------------------- */
void sm_log(const char* fmt, ...);
void sm_reset(void);
const char* sm_log_text(void);
int sm_log_len(void);

/* ---- the bump allocator --------------------------------------------- */
const unsigned char* sm_heap_base(void);
long sm_heap_off(const void* p);

/* ---- the fake big file ---------------------------------------------- */
void sm_bigfile_clear(void);
int sm_bigfile_add(unsigned int key, const void* data, int len);

/* ---- the fake loader stream ----------------------------------------- */
void sm_stream_set(const void* data, int len);
char** sm_stream_cursor(void);

/* ---- what isSpeedModeAndBinFile() answers --------------------------- */
extern int sm_speed_mode;
