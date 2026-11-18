#pragma once
/* The path module's seam, shared by the harnesses that build
 * datapath.c.
 *
 * datapath.c asks SDL where the executable is and where the player's
 * own directory is. A test wants to answer both itself -- to point the
 * install at a fixture directory, and to say "there is no such
 * directory" without arranging for one -- so the three SDL entry
 * points it uses are defined here instead of linked.
 *
 * The real headers are included, so a prototype that drifts is a
 * compile error rather than a silently different function.
 *
 * A case sets dpt_basePath to the directory the executable sits in,
 * separator and all, or leaves it empty for "SDL cannot say". There
 * used to be a copy of this in each of the four harnesses, keyed on
 * the Windows call the module made before SDL; one copy means a case
 * added to one of them reads the same as the same case in the next.
 */

#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>

/* Where the executable is, as SDL would report it: a directory with a
   separator already on the end. Empty means SDL has no answer, which
   is the whole of "nowhere to look" now -- the Windows call this
   replaced left its buffer unspecified on failure, so a harness had to
   plant a plausible path in it to make an ignored failure visible. A
   caller that ignores this one gets NULL, and the sanitizer says so. */
char dpt_basePath[512];
/* How many times the module asked, and how many times it was told.
   Counted so a case can assert the module does not ask once per file,
   and that a decline happened in the module rather than here. */
int  dpt_baseCalls;
int  dpt_baseOk;
/* The player's own directory, for the one path rule that writes.
   Empty means SDL has no answer, as above. */
char dpt_prefPath[512];

const char* SDL_GetBasePath(void)
{
    dpt_baseCalls++;
    if (!dpt_basePath[0])
        return NULL;
    dpt_baseOk++;
    return dpt_basePath;
}

/* The real one allocates, and the caller frees it; so does this, or a
   leak checker would have nothing to say about the caller. */
char* SDL_GetPrefPath(const char* org, const char* app)
{
    char*  out;
    size_t len;

    (void)org;
    (void)app;
    if (!dpt_prefPath[0])
        return NULL;
    len = strlen(dpt_prefPath);
    out = (char*)malloc(len + 1);
    if (out)
        memcpy(out, dpt_prefPath, len + 1);
    return out;
}

void SDL_free(void* mem)
{
    free(mem);
}

/* Nothing is made on disk: a harness that wanted a real directory
   would be testing the operating system. The path is kept so a case
   can say which one was asked for, and the count so a case can say it
   was asked for once rather than once per file. */
char dpt_madeDir[512];
int  dpt_madeDirs;

bool SDL_CreateDirectory(const char* path)
{
    dpt_madeDirs++;
    if (path)
    {
        size_t len = strlen(path);

        if (len < sizeof(dpt_madeDir))
            memcpy(dpt_madeDir, path, len + 1);
    }
    return true;
}

/* A fixed moment, so a name built from the clock is the same on every
   run. A case that wants another one sets it. */
SDL_Time dpt_now = 1000000000LL * 1700000000LL;

bool SDL_GetCurrentTime(SDL_Time* ticks)
{
    if (!ticks)
        return false;
    *ticks = dpt_now;
    return true;
}

/* Not the real calendar: the fields are derived from the clock simply
   enough that a case can predict them, which is all a name built from
   them needs. */
bool SDL_TimeToDateTime(SDL_Time ticks, SDL_DateTime* dt, bool localTime)
{
    long long secs = (long long)(ticks / 1000000000LL);

    (void)localTime;
    if (!dt)
        return false;
    memset(dt, 0, sizeof(*dt));
    dt->year = 2023;
    dt->month = 11;
    dt->day = 14;
    dt->hour = (int)((secs / 3600) % 24);
    dt->minute = (int)((secs / 60) % 60);
    dt->second = (int)(secs % 60);
    dt->nanosecond = (int)(ticks % 1000000000LL);
    return true;
}
