/* Mocks for the win/winsound.c differential test.
 *
 * winsound.c is the DirectSound platform layer under sound.c. Everything
 * it does to the device goes through COM vtables, so the device, the
 * 3D buffer, the property set and the notify interface are replaced by
 * objects whose methods log their arguments and answer deterministically.
 * The allocator, the big-file reader, the loader's byte-stream reader and
 * the Win32 calls are mocked the same way; basic_array.c, math.c and
 * win/adpcm.c are the engine's own, linked rather than mocked.
 *
 * Everything in here is shared by the two sides and rewound between them
 * by wsnd_mock_reset(), so both runs see identical addresses and an
 * identical call sequence when they behave identically.
 *
 * The engine-side callbacks that winsound.c calls -- SND_l_GetAttFromVol,
 * SND_AutoPlay and the rest -- are NOT here. They take pointers into the
 * caller's own globals, which are per-side, so they live in
 * winsound_side.c and are renamed along with everything else.
 */
#pragma once

#include <dsound.h>
#include "../win/eax/eax.h"

/* ---- the call log --------------------------------------------------- */
void wsnd_log(const char* fmt, ...);
void wsnd_mock_reset(void);
const char* wsnd_log_text(void);
int wsnd_log_len(void);

/* ---- the bump allocator --------------------------------------------- */
const unsigned char* wsnd_heap_base(void);
int wsnd_heap_used(void);
long wsnd_heap_off(const void* p);

/* ---- the mock device ------------------------------------------------ */
IDirectSound8* wsnd_device(void);
/* id of a mock COM object, or -1 for NULL and -2 for anything else */
int wsnd_obj_id(const void* p);

/* CreateSoundBuffer fails on every Nth call; 0 = never */
extern int wsnd_create_fail_mod;
/* Lock fails on every Nth call; 0 = never */
extern int wsnd_lock_fail_mod;
/* what GetStatus reports */
extern int wsnd_status;
/* what GetCurrentPosition reports */
extern int wsnd_play_pos;
/* second half of a Lock: 0 for none */
extern int wsnd_lock_tail;
/* QuerySupport answer for the EAX listener property set */
extern int wsnd_eax_support;
/* QueryInterface refuses the Nth interface asked for; 0 = never */
extern int wsnd_qi_fail_mod;

/* ---- the fake file and the registry --------------------------------- */
extern int wsnd_reg_fail;      /* bit 0: the FX value, bit 1: the GUID */
extern int wsnd_reg_fx_value;
extern int wsnd_read_bytes;    /* how much ReadFile hands back */
extern int wsnd_loa_speedmode; /* drives isSpeedModeAndBinFile() */

/* ---- the scripted thread -------------------------------------------- */
/* WaitForMultipleObjects walks this list and then returns 2, which is the
   thread's own exit case; ExitThread longjmps back out of the loop. */
void wsnd_thread_script(const int* indices, int count);
void wsnd_run_thread(int (*body)(void*));
