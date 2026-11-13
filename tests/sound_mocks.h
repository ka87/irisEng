/* Mocks for the sound.c differential test.
 *
 * sound.c is the engine-side mixer and event layer over DirectSound. The
 * device half of it cannot be stood up here, but everything that reaches
 * the device does so through the tSndInterfaceStruct function table, so
 * the table is replaced with one that logs every call and answers
 * deterministically. The allocator, the big-file reader and the loader's
 * byte-stream reader are mocked the same way; math.c and basic_array.c
 * are the engine's own, linked rather than mocked.
 *
 * Everything in here is shared by the two sides and rewound between them
 * by snd_mock_reset(), so both runs see identical addresses and an
 * identical call sequence when they behave identically.
 */
#pragma once

#include "../sound.h"

/* ---- the call log --------------------------------------------------- */
void snd_log(const char* fmt, ...);
void snd_mock_reset(void);
const char* snd_log_text(void);
int snd_log_len(void);

/* ---- the bump allocator --------------------------------------------- */
const unsigned char* snd_heap_base(void);
int snd_heap_used(void);

/* ---- the fake big file ---------------------------------------------- */
void snd_bigfile_clear(void);
int snd_bigfile_add(unsigned int key, const void* data, int len);
void snd_stream_set(const void* data, int len);

/* ---- the mock sound interface --------------------------------------- */
extern tSndInterfaceStruct snd_mock_intf;
extern int snd_play_fail_mod;      /* Play() fails on every Nth call; 0 = never */
extern int snd_global_vol;
extern int snd_global_pan;

/* the deterministic 15-bit rand the engine's MSVC build has */
void snd_srand(unsigned int seed);
