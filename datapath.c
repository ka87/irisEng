#include "datapath.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

int DPT_b_DataPath(const char* folder, const char* name, char* out, size_t n)
{
    char  exe[MAX_PATH];
    char* cut;
    char  sep;
    int   len;

    if (!folder || !name || !out || !n)
        return 0;
    if (!GetModuleFileNameA(NULL, exe, (DWORD)sizeof(exe)))
        return 0;

    cut = strrchr(exe, '\\');
    if (!cut)
        cut = strrchr(exe, '/');
    if (!cut)
        return 0;
    sep = *cut;
    *(cut + 1) = 0;

    /* One bounded call rather than a strcat chain: this project builds
       with the deprecated-call check on, and a single snprintf reports
       truncation instead of quietly producing a short path. */
    len = snprintf(out, n, "%sdata%c%s%c%s", exe, sep, folder, sep, name);
    return len > 0 && (size_t)len < n;
}
