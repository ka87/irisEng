/* Differential test for win/winsound.c.
 *
 * Runs the rewrite and the frozen pre-pass copy over identical state, one
 * case at a time, with the mock DirectSound device and the mock allocator
 * rewound between the two runs so that both sides see the same addresses.
 * A case passes only when the two sides produce byte-identical streams:
 * the ordered log of every device, allocator, Win32, big-file and loader
 * call, and the structured dump of the platform layer's own state that
 * each side writes as it goes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "winsound_mocks.h"

int BASE_wsnd_side_cases(void);
void BASE_wsnd_side_run(int c);
int NEW_wsnd_side_cases(void);
void NEW_wsnd_side_run(int c);
void NEW_wsnd_side_guards(void);

void MATH_InitModule(void);

#define SNAP_MAX (8 * 1024 * 1024)
static char snap_base[SNAP_MAX];
static int snap_base_len;

static int first_diff(const char* a, int alen, const char* b, int blen)
{
    int n = alen < blen ? alen : blen;
    int i;
    for (i = 0; i < n; ++i)
        if (a[i] != b[i]) return i;
    if (alen != blen) return n;
    return -1;
}

static void show(const char* who, const char* s, int len, int at)
{
    int start = at - 160;
    int i;
    if (start < 0) start = 0;
    fprintf(stderr, "  %s [%d..]: ", who, start);
    for (i = start; i < len && i < start + 400; ++i)
        fputc(s[i] == '\n' ? '|' : s[i], stderr);
    fputc('\n', stderr);
}

int main(int argc, char** argv)
{
    int cases, c, bad = 0, checked = 0;
    long long bytes = 0;

    /* The engine calls this at startup. Without it the sin/cos and
       reciprocal tables and MATH_gd_magicDoubleForRound are all zero, so
       every table-driven result collapses to the same arm on both sides
       and compares equal. math.c is linked whole here, so one call fixes
       it for both sides. */
    MATH_InitModule();

    if (argc > 1 && strcmp(argv[1], "--guards") == 0)
    {
        NEW_wsnd_side_guards();
        return 0;
    }

    /* --case N prints both sides' streams in full, for reading a
       difference rather than guessing at it. */
    if (argc > 2 && strcmp(argv[1], "--case") == 0)
    {
        int n = atoi(argv[2]);
        wsnd_mock_reset();
        BASE_wsnd_side_run(n);
        fprintf(stderr, "==== BASE ====\n%.*s", wsnd_log_len(), wsnd_log_text());
        wsnd_mock_reset();
        NEW_wsnd_side_run(n);
        fprintf(stderr, "==== NEW ====\n%.*s", wsnd_log_len(), wsnd_log_text());
        return 0;
    }

    /* The engine casts a tSndSoundBufferElem* to int and back at several
       sites, so the comparison is only meaningful while the mock arena
       lives below 4GB -- which is why the binary is linked -no-pie. */
    if ((unsigned long)(size_t)wsnd_heap_base() >= 0x100000000UL)
    {
        fprintf(stderr, "mock arena is above 4GB; the pointer-to-int casts "
                        "under test would not behave as they do on the target\n");
        return 1;
    }

    if (BASE_wsnd_side_cases() != NEW_wsnd_side_cases())
    {
        fprintf(stderr, "case counts differ\n");
        return 1;
    }
    cases = NEW_wsnd_side_cases();

    for (c = 0; c < cases; ++c)
    {
        int nlen, at;

        wsnd_mock_reset();
        BASE_wsnd_side_run(c);
        snap_base_len = wsnd_log_len();
        if (snap_base_len >= SNAP_MAX)
        {
            fprintf(stderr, "case %d: baseline snapshot too large\n", c);
            return 1;
        }
        memcpy(snap_base, wsnd_log_text(), snap_base_len);

        wsnd_mock_reset();
        NEW_wsnd_side_run(c);
        nlen = wsnd_log_len();

        ++checked;
        bytes += nlen;
        at = first_diff(snap_base, snap_base_len, wsnd_log_text(), nlen);
        if (at >= 0)
        {
            if (++bad <= 5)
            {
                fprintf(stderr, "case %d differs at byte %d (base %d bytes, new %d)\n",
                        c, at, snap_base_len, nlen);
                show("base", snap_base, snap_base_len, at);
                show("new ", wsnd_log_text(), nlen, at);
            }
        }
    }

    printf("winsound: %d cases, %lld bytes of state and call log compared, %d differing\n",
           checked, bytes, bad);
    return bad ? 1 : 0;
}
