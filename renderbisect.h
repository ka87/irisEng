#pragma once
#include "common.h"

/* A bisect gate for the GAO draw walk.
 *
 * renderbisect_keys.h lists every GAO that world ff00948d places. Each
 * active line in it names one GAO; comment a line out to drop that GAO
 * from the list. RBIS_LISTED_ARE below decides what being on the list
 * means.
 *
 * With RBIS_LISTED_ARE_HIDDEN, everything listed is skipped by the
 * renderer, so a full list draws nothing and commenting a block of
 * lines brings exactly that block back. Halve the commented block
 * until the fault follows one GAO.
 *
 * With RBIS_LISTED_ARE_SHOWN it is the other way round: only what is
 * listed draws. Same bisect, opposite edits.
 *
 * Set RBIS_ENABLED to 0 to compile the whole thing out. */

#define RBIS_ENABLED            1
#define RBIS_LISTED_ARE_HIDDEN  1   /* 0 = listed are the only ones drawn */

struct sGameObjectMain;

#if RBIS_ENABLED
void RBIS_NoteGao(unsigned int key, struct sGameObjectMain* gao);
void RBIS_Report(void);
int  RBIS_IsHidden(struct sGameObjectMain* gao);
#else
#define RBIS_NoteGao(key, gao)  ((void)0)
#define RBIS_Report()           ((void)0)
#define RBIS_IsHidden(gao)      (0)
#endif
