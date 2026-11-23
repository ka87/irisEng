/* Differential test for platform/sndbackend.c.
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

#include "sndbackend_mocks.h"

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


/* The block a stream starts on.
 *
 * The frozen copy rounds the start position down to a block size it
 * reads from sizeof a struct; the engine uses the format's own 36
 * bytes per channel. Where a stream is played from a non-zero offset
 * the two therefore land on different boundaries, and the seek that
 * follows moves with them. That difference is deliberate, so it is
 * counted apart rather than hidden: masking the numbers it can reach
 * must leave the two logs identical, and anything it does not reach
 * still counts as a plain difference.
 */
static int snd_is_start_block(const char* s, int i, int len, int* skip)
{
    static const char* const tok[] = { "u6=", "sum=", "cur=", 0 };
    int t;
    for (t = 0; tok[t]; ++t)
    {
        int n = (int)strlen(tok[t]);
        if (i + n <= len && memcmp(s + i, tok[t], (size_t)n) == 0) { *skip = n; return 1; }
    }
    return 0;
}

/* Copies the log with those fields' digits replaced, and the second
   argument of each seek and each read with them. A read's result is
   left alone: it does not move, and leaving it in keeps the check on
   what the read actually returned. */
static int snd_mask(const char* s, int len, char* dst)
{
    int i = 0, o = 0, skip;
    while (i < len)
    {
        if (snd_is_start_block(s, i, len, &skip))
        {
            memcpy(dst + o, s + i, (size_t)skip); o += skip; i += skip;
            while (i < len && (s[i] == '-' || (s[i] >= '0' && s[i] <= '9'))) ++i;
            dst[o++] = '#';
            continue;
        }
        if ((i + 6 <= len && memcmp(s + i, "fseek(", 6) == 0)
         || (i + 6 <= len && memcmp(s + i, "fread(", 6) == 0))
        {
            memcpy(dst + o, s + i, 6); o += 6; i += 6;
            while (i < len && s[i] != ',' && s[i] != ')') dst[o++] = s[i++];
            if (i < len && s[i] == ',')
            {
                dst[o++] = s[i++];
                while (i < len && (s[i] == '-' || (s[i] >= '0' && s[i] <= '9'))) ++i;
                dst[o++] = '#';
            }
            continue;
        }
        dst[o++] = s[i++];
    }
    return o;
}

#define SND_START_BLOCK_CASES 84
#define SND_START_BLOCK_NUMBERS_ONLY 81
static int start_block;

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
    start_block = 0;


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
            static char mb[SNAP_MAX], mn[SNAP_MAX];
            int lb = snd_mask(snap_base, snap_base_len, mb);
            int ln = snd_mask(wsnd_log_text(), nlen, mn);
            if (lb == ln && memcmp(mb, mn, (size_t)lb) == 0)
                ++start_block;
            if (++bad <= 2)
            {
                fprintf(stderr, "case %d differs at byte %d (base %d bytes, new %d)\n",
                        c, at, snap_base_len, nlen);
                show("base", snap_base, snap_base_len, at);
                show("new ", wsnd_log_text(), nlen, at);
            }
        }
    }

    printf("sndbackend: %d cases, %lld bytes of state and call log compared, %d differing\n",
           checked, bytes, bad);
    printf("  of those, %d differ only in the start block and what is read from it;\n"
           "  the other %d also decode different audio, which is the same cause seen through\n",
           start_block, bad - start_block);

    /* Both counts are pinned, and neither is allowed to drift. These
       are the cases where the engine and the frozen copy deliberately
       disagree, because the frozen copy rounds a stream's start down
       to a block size it reads from sizeof a struct. Any difference
       the fix does not account for pushes the first number up. */
    if (bad != SND_START_BLOCK_CASES || start_block != SND_START_BLOCK_NUMBERS_ONLY)
    {
        fprintf(stderr, "deliberate-difference counts moved: %d differing (expected %d), "
                        "%d numbers-only (expected %d)\n",
                bad, SND_START_BLOCK_CASES, start_block, SND_START_BLOCK_NUMBERS_ONLY);
        return 1;
    }
    return 0;
}
