#include "datapath.h"

#include <stdio.h>
#include <string.h>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>

/* The game's own name, which is the folder the player's saves and
   screenshots go in. One spelling, here, because it is the one thing
   in these paths a person sees. */
#define DPT_GAMENAME "irisEng"
/* SDL wants a publisher as well as a program for its preferences
   path, and uses it as a folder on the systems that have one. */
#define DPT_ORGNAME  "irisEng"

/* Joins a directory that already ends in a separator to the rest of a
   path. Truncation is a failure rather than a short path that fails to
   open for a reason nobody can see. */
static int DPT_l_b_Join(char* out, size_t n, const char* dir, const char* tail)
{
    int len;

    if (!out || !n || !dir || !tail)
        return 0;
    len = snprintf(out, n, "%s%s", dir, tail);
    return len > 0 && (size_t)len < n;
}

int DPT_b_ExePath(const char* name, char* out, size_t n)
{
    /* SDL's base path is the directory the program was run from, with
       a separator already on the end, and it is the same answer on
       every system -- which is what replaced asking Windows for the
       module's file name and cutting the last component off it. */
    const char* base = SDL_GetBasePath();

    if (!base || !name)
        return 0;
    return DPT_l_b_Join(out, n, base, name);
}

int DPT_b_DataPath(const char* folder, const char* name, char* out, size_t n)
{
    char tail[512];
    int  len;

    if (!folder || !name)
        return 0;
    /* Forward slashes throughout: Windows takes either, and every
       other system takes only this one. */
    len = snprintf(tail, sizeof(tail), "data/%s/%s", folder, name);
    if (len <= 0 || (size_t)len >= sizeof(tail))
        return 0;
    return DPT_b_ExePath(tail, out, n);
}

int DPT_b_UserPath(const char* name, char* out, size_t n)
{
    if (!name)
        return 0;

#ifdef _WIN32
    /* Under the player's Documents, which is where a Windows player
       looks for a game's saves. SDL answers with the folder itself,
       so the game's own goes under it. */
    {
        const char* docs = SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS);
        char        here[512];
        int         len;

        if (!docs)
            return 0;
        len = snprintf(here, sizeof(here), "%s%s/", docs, DPT_GAMENAME);
        if (len <= 0 || (size_t)len >= sizeof(here))
            return 0;
        SDL_CreateDirectory(here);
        return DPT_l_b_Join(out, n, here, name);
    }
#else
    /* Everywhere else, where the convention is a data directory under
       the user's home rather than a documents folder. SDL's
       preferences path is exactly that -- $XDG_DATA_HOME/<game>/,
       which is ~/.local/share/<game>/ unless the user says otherwise
       -- and it makes the directory on the way.

       The two locals are declared in here rather than at the top of
       the function because the branch above has no use for them, and
       a local only one side of an #ifdef uses is a warning on the
       other side -- one the warning sweep cannot see, since it only
       ever compiles this side. */
    {
        char* dir = SDL_GetPrefPath(DPT_ORGNAME, DPT_GAMENAME);
        int   ok;

        if (!dir)
            return 0;
        ok = DPT_l_b_Join(out, n, dir, name);
        SDL_free(dir);
        return ok;
    }
#endif
}

int DPT_b_ScreenshotPath(char* out, size_t n)
{
    SDL_Time     now;
    SDL_DateTime at;
    char         dir[512];
    char         tail[64];
    int          len;

    /* The folder first, so a failure to make it is a failure here
       rather than a file that will not open later. DPT_b_UserPath
       makes the player's own directory; this makes the one inside
       it. */
    if (!DPT_b_UserPath("screenshots/", dir, sizeof(dir)))
        return 0;
    SDL_CreateDirectory(dir);

    if (!SDL_GetCurrentTime(&now) || !SDL_TimeToDateTime(now, &at, 1))
        return 0;

    len = snprintf(tail, sizeof(tail), "%04d%02d%02d_%02d%02d%02d_%03d.png",
                   at.year, at.month, at.day, at.hour, at.minute, at.second,
                   at.nanosecond / 1000000);
    if (len <= 0 || (size_t)len >= sizeof(tail))
        return 0;
    return DPT_l_b_Join(out, n, dir, tail);
}
