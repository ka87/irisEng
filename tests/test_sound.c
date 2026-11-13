/* Differential test for sound.c.
 *
 * Runs the rewrite and the frozen pre-pass copy over identical state, one
 * case at a time, with the mock device and the mock allocator rewound
 * between the two runs so that both sides see the same addresses. A case
 * passes only when the two sides produce byte-identical streams: the
 * ordered log of every device, allocator, big-file and loader call, and
 * the structured dump of the engine's own state that each side writes at
 * the end of the case.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound_mocks.h"

int BASE_snd_side_cases(void);
void BASE_snd_side_run(int c);
int NEW_snd_side_cases(void);
void NEW_snd_side_run(int c);

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

int main(void)
{
    int cases, c, bad = 0, checked = 0;
    long long bytes = 0;

    if (BASE_snd_side_cases() != NEW_snd_side_cases())
    {
        fprintf(stderr, "case counts differ\n");
        return 1;
    }
    cases = NEW_snd_side_cases();

    for (c = 0; c < cases; ++c)
    {
        int nlen, at;

        snd_mock_reset();
        BASE_snd_side_run(c);
        snap_base_len = snd_log_len();
        if (snap_base_len >= SNAP_MAX)
        {
            fprintf(stderr, "case %d: baseline snapshot too large\n", c);
            return 1;
        }
        memcpy(snap_base, snd_log_text(), snap_base_len);

        snd_mock_reset();
        NEW_snd_side_run(c);
        nlen = snd_log_len();

        ++checked;
        bytes += nlen;
        at = first_diff(snap_base, snap_base_len, snd_log_text(), nlen);
        if (at >= 0)
        {
            if (++bad <= 5)
            {
                fprintf(stderr, "case %d differs at byte %d (base %d bytes, new %d)\n",
                        c, at, snap_base_len, nlen);
                show("base", snap_base, snap_base_len, at);
                show("new ", snd_log_text(), nlen, at);
            }
        }
    }

    printf("sound: %d cases, %lld bytes of state and call log compared, %d differing\n",
           checked, bytes, bad);
    return bad ? 1 : 0;
}
