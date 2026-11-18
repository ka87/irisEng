/* A unit test for settings.c.
 *
 * New code with no earlier copy, so this is a direct check rather
 * than a differential one. It includes settings.c so the reader can
 * be pointed at a file written here, rather than at the one name the
 * module uses in earnest.
 *
 * What is NOT covered here, and is worth knowing: the precedence of
 * the command line over the ini. That lives in WinMain -- the ini is
 * applied first and ENG_ParseCommandLine runs over the top of it --
 * and no harness owns main.c. What this file covers is the reader.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okStr(const char* got, const char* want, const char* what)
{
    checks++;
    if (!got && !want)
        return;
    if (!got || !want || strcmp(got, want) != 0)
    {
        printf("  FAIL %s: got %s want %s\n", what,
               got ? got : "(none)", want ? want : "(none)");
        fails++;
    }
}

static void okInt(int got, int want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %d want %d\n", what, got, want);
        fails++;
    }
}

/* ------------------------------------------------------- the log, mocked */

static char dmLog[64][256];
static int  dmLogLen;

void DBG_OutputDebugString(char* format, ...)
{
    va_list ap;

    if (dmLogLen >= 64)
        return;
    va_start(ap, format);
    vsnprintf(dmLog[dmLogLen], sizeof(dmLog[0]), format, ap);
    va_end(ap);
    dmLogLen++;
}

static int loggedAbout(const char* needle)
{
    int i;

    for (i = 0; i < dmLogLen; i++)
        if (strstr(dmLog[i], needle))
            return 1;
    return 0;
}

/* Where SET_InitModule looks. The real one answers with the
   executable's own directory; this one answers with whatever the case
   below has put a file in, which is how the "beside the executable"
   rule is checked without an executable to be beside. */
static const char* dmExeDir = "";
int DPT_b_ExePath(const char* name, char* out, size_t n)
{
    int len;

    if (!name || !out || !n)
        return 0;
    len = snprintf(out, (size_t)n, "%s%s", dmExeDir, name);
    return len > 0 && (size_t)len < n;
}

#include "../settings.c"

/* ------------------------------------------------------------ the driver */

#define TMP "tests/build/settings_case.ini"

/* Writes an ini and reads it back through the module. */
static void load(const char* text)
{
    FILE* f = fopen(TMP, "wb");

    if (!f)
    {
        printf("  FAIL cannot write %s\n", TMP);
        fails++;
        return;
    }
    fwrite(text, 1, strlen(text), f);
    fclose(f);

    dmLogLen = 0;
    SET_l_Read(TMP);
}

static void missingFileIsNotAProblem(void)
{
    load("[data]\nbigfile = kept.bf\n");
    okStr(SET_pc_BigFile(), "kept.bf", "a file that is there is read");

    dmLogLen = 0;
    SET_l_Read("tests/build/settings_no_such_file.ini");
    okStr(SET_pc_BigFile(), NULL, "a missing file leaves the big file unset");
    okInt(SET_b_Binarized(), 0, "and binarized off");
    okStr(SET_pc_Video(0), NULL, "and no videos");
    okInt(dmLogLen, 0, "and says nothing about it");
}

static void theDisplaySettings(void)
{
    int v;

    printf("-- the display settings, which the registry used to hold\n");
    load("[display]\n"
         "adapter = 1\n"
         "mode = 7\n"
         "zbuffer = 24\n"
         "mipmapping = 0\n"
         "windowed = 1\n"
         "width = 1280\n"
         "height = 720\n");

    v = -1; okInt(SET_b_DisplayAdapter(&v) && v == 1, 1, "the adapter");
    v = -1; okInt(SET_b_DisplayMode(&v) && v == 7, 1, "the mode");
    v = -1; okInt(SET_b_DisplayZBuffer(&v) && v == 24, 1, "the depth buffer");
    v = -1; okInt(SET_b_DisplayMipMapping(&v) && v == 0, 1, "mip mapping off, which is not the same as unset");
    v = -1; okInt(SET_b_DisplayWindowed(&v) && v == 1, 1, "windowed");
    v = -1; okInt(SET_b_DisplayWidth(&v) && v == 1280, 1, "the width");
    v = -1; okInt(SET_b_DisplayHeight(&v) && v == 720, 1, "the height");
    okInt(dmLogLen, 0, "and nothing to complain about");
}

static void theSpecialArray(void)
{
    printf("-- the special array's file\n");

    /* Named, and not named: the second is what every build that does
       not mention it gets, and it is what the game has always
       opened. */
    load("[data]\nspecialarray = other.spe\n");
    okStr(SET_pc_SpecialArray(), "other.spe", "the ini can name it");
}

static void theFileBesideTheExecutable(void)
{
    char path[512];
    FILE* f;

    printf("-- the ini is read from beside the executable\n");

    /* Written where the stubbed path module says the executable is,
       under a name the working directory does not have, so finding it
       means the rule was followed rather than the old one. */
    dmExeDir = "tests/build/";
    snprintf(path, sizeof(path), "tests/build/%s", SET_INIFILE);
    f = fopen(path, "wb");
    if (f)
    {
        fputs("[data]\nbigfile = beside_the_exe.bf\n", f);
        fclose(f);
    }
    dmLogLen = 0;
    SET_InitModule();
    okStr(SET_pc_BigFile(), "beside_the_exe.bf",
          "SET_InitModule reads it from there, not from the working directory");
    remove(path);
    dmExeDir = "";
}

static void theShippedFile(void)
{
    /* The file in the repository root, read as it ships, so a typo in
       it is a failure here rather than a surprise in game. */
    dmLogLen = 0;
    SET_l_Read("engine.ini");
    okStr(SET_pc_BigFile(), "sally_clean.bf",
          "the shipped ini names the big file");
    okInt(SET_b_Binarized(), 1, "and asks for binarized world files");
    okStr(SET_pc_Backend(), "d3d", "and names the renderer the game shipped on");
    okStr(SET_pc_Video((int)0x490294C0u), "BGEMakingOf.bik",
          "and has the video under the id the trailer launcher asks for");
    okInt(dmLogLen, 0, "and the reader finds nothing to complain about");
}

static void layout(void)
{
    load("; a comment\n"
         "# another\n"
         "\n"
         "   [ DaTa ]   \n"
         "\tBIGFILE\t=\tspaced.bf\t\n"
         "binarized=yes\n"
         "\n"
         "[VIDEO]\n"
         "  7  =  seven.bik  \n");
    okStr(SET_pc_BigFile(), "spaced.bf", "whitespace is trimmed off a value");
    okInt(SET_b_Binarized(), 1, "the section and key names ignore case");
    okStr(SET_pc_Video(7), "seven.bik", "and so does the video section");
    okInt(dmLogLen, 0, "none of that is worth a word");
}

static void quoting(void)
{
    load("[data]\nbigfile = \"two words.bf\"\n"
         "[video]\n0 = \"a video.bik\"\n");
    okStr(SET_pc_BigFile(), "two words.bf", "quotes come off a name with spaces");
    okStr(SET_pc_Video(0), "a video.bik", "and off a video's name");

    load("[data]\nbigfile = \"trailing \"\n");
    okStr(SET_pc_BigFile(), "trailing ", "quoting is how a trailing space survives");

    load("[data]\nbigfile = mid\"dle.bf\n");
    okStr(SET_pc_BigFile(), "mid\"dle.bf", "a quote inside a name is left alone");

    /* Both ends or neither: an unbalanced quote is part of the name,
       not an invitation to chop the other end off it. */
    load("[data]\nbigfile = \"unbalanced.bf\n");
    okStr(SET_pc_BigFile(), "\"unbalanced.bf", "a leading quote alone is kept");
    load("[data]\nbigfile = unbalanced.bf\"\n");
    okStr(SET_pc_BigFile(), "unbalanced.bf\"", "and so is a trailing one");
}

static void theFlag(void)
{
    static const struct { const char* v; int want; } cases[] = {
        { "1", 1 }, { "yes", 1 }, { "YES", 1 }, { "true", 1 }, { "on", 1 },
        { "2", 1 }, { "-1", 1 },
        { "0", 0 }, { "no", 0 }, { "false", 0 }, { "off", 0 },
        { "", 0 }, { "maybe", 0 },
    };
    unsigned i;
    char buf[64];

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        snprintf(buf, sizeof(buf), "[data]\nbinarized = %s\n", cases[i].v);
        load(buf);
        checks++;
        if (SET_b_Binarized() != cases[i].want)
        {
            printf("  FAIL binarized = \"%s\": got %d want %d\n",
                   cases[i].v, SET_b_Binarized(), cases[i].want);
            fails++;
        }
    }
}

static void videoIds(void)
{
    load("[video]\n0 = zero.bik\n3 = three.bik\n-2 = minus.bik\n");
    okStr(SET_pc_Video(0), "zero.bik", "id 0");
    okStr(SET_pc_Video(3), "three.bik", "a gap in the ids is no trouble");
    okStr(SET_pc_Video(-2), "minus.bik", "and a negative id works");
    okStr(SET_pc_Video(1), NULL, "an id with no entry has no file");
    okStr(SET_pc_Video(99), NULL, "and nor does one past the end");

    /* a repeated id: the last one wins, and the table does not grow */
    load("[video]\n4 = first.bik\n4 = second.bik\n");
    okStr(SET_pc_Video(4), "second.bik", "a repeated id takes the last value");
    okInt(set_videoNum, 1, "and does not take a second slot");
}

/* What the scripts push is a 32-bit resource key, not a small index,
   so the whole range has to be reachable and hex has to be
   writable -- a key with the top bit set does not fit a positive
   int and nobody writes one in decimal. */
static void idsAreThirtyTwoBitKeys(void)
{
    load("[video]\n0x3001978F = a.bik\n0x610127be = b.bik\n0XFF = c.bik\n");
    okStr(SET_pc_Video((int)0x3001978Fu), "a.bik", "a hex id");
    okStr(SET_pc_Video((int)0x610127BEu), "b.bik", "in lower case too");
    okStr(SET_pc_Video(255), "c.bik", "and with an upper-case prefix");
    okInt(dmLogLen, 0, "none of which is worth a word");

    /* the top bit set: the id arrives as a negative int, and has to
       match the key as it is written in the file */
    load("[video]\n0x9E006771 = top.bik\n");
    okStr(SET_pc_Video((int)0x9E006771u), "top.bik",
          "a key with the top bit set matches");
    ok((int)0x9E006771u < 0, "and that key really is negative as an int");

    /* the same key written as a decimal is the same call */
    load("[video]\n2650826609 = dec.bik\n");
    okStr(SET_pc_Video((int)0x9E006771u), "dec.bik",
          "the same key in decimal matches the same call");

    /* and as a negative decimal, which is how it would be written if
       it were read back out of an int */
    load("[video]\n-1644140687 = neg.bik\n");
    okStr(SET_pc_Video((int)0x9E006771u), "neg.bik",
          "and as the negative decimal of the same bits");

    load("[video]\n-1 = minusone.bik\n");
    okStr(SET_pc_Video(-1), "minusone.bik", "-1 is still -1");
    okStr(SET_pc_Video((int)0xFFFFFFFFu), "minusone.bik", "which is 0xFFFFFFFF");

    /* past the range, and malformed, are reported rather than folded
       into something that happens to parse */
    load("[video]\n4294967296 = over.bik\n");
    ok(loggedAbout("is not an id"), "a decimal past 32 bits is reported");

    load("[video]\n0x100000000 = over.bik\n");
    ok(loggedAbout("is not an id"), "and so is a hex one");

    load("[video]\n0x = empty.bik\n");
    ok(loggedAbout("is not an id"), "a bare 0x is not an id");

    load("[video]\n-0x10 = neghex.bik\n");
    ok(loggedAbout("is not an id"), "nor is a negated hex id");

    load("[video]\n0x12zz = junk.bik\n");
    ok(loggedAbout("is not an id"), "nor hex with something after it");

    load("[video]\n-2147483649 = under.bik\n");
    ok(loggedAbout("is not an id"), "nor a decimal past the negative end");
}

/* [render] carries the renderer's name across. Which names mean
   what is gdi.c's business, so this module only has to not lose the
   string and not invent one. */
static void theRenderer(void)
{
    load("[render]\nbackend = gl\n");
    okStr(SET_pc_Backend(), "gl", "the renderer's name is carried across");

    load("[render]\nBACKEND = D3D9\n");
    okStr(SET_pc_Backend(), "D3D9",
          "the key ignores case but the value is passed as written");

    load("[data]\nbigfile = x.bf\n");
    okStr(SET_pc_Backend(), NULL, "no [render] means no renderer asked for");

    load("[render]\nbackend =\n");
    okStr(SET_pc_Backend(), NULL, "and nor does an empty one");

    load("[render]\nbakend = gl\n");
    okStr(SET_pc_Backend(), NULL, "a misspelt setting asks for nothing");
    ok(loggedAbout("bakend"), "and is named in the log");

    /* it must not leak across a re-read, like everything else */
    load("[render]\nbackend = gl\n");
    load("[data]\nbigfile = y.bf\n");
    okStr(SET_pc_Backend(), NULL, "a second read clears it");
}

static void mistakesAreReported(void)
{
    load("[data]\nbigfle = typo.bf\n");
    okStr(SET_pc_BigFile(), NULL, "a misspelt setting sets nothing");
    ok(loggedAbout("bigfle"), "and is named in the log");

    load("[nonsuch]\nbigfile = ignored.bf\n");
    okStr(SET_pc_BigFile(), NULL, "a setting in an unknown section is ignored");
    ok(loggedAbout("nonsuch"), "and the section is named");

    load("bigfile = loose.bf\n");
    okStr(SET_pc_BigFile(), NULL, "a setting before any section is ignored");
    ok(loggedAbout("outside any section"), "and said to be");

    load("[data]\nthis line has no equals sign\n");
    ok(loggedAbout("not a setting"), "a line that is not a setting is reported");

    load("[data\nbigfile = x.bf\n");
    ok(loggedAbout("unclosed"), "an unclosed section is reported");
    okStr(SET_pc_BigFile(), NULL, "and what follows it is not taken as [data]");

    /* A key that is not a number must not quietly become id 0, which
       is what atoi on its own would do. */
    load("[video]\nmaking_of = BGEMakingOf.bik\n");
    okStr(SET_pc_Video(0), NULL, "a non-numeric video key does not become id 0");
    ok(loggedAbout("is not an id"), "and is reported");

    load("[video]\n12x = x.bik\n");
    okStr(SET_pc_Video(12), NULL, "nor does a key that only starts with digits");
    ok(loggedAbout("is not an id"), "and that is reported too");
}

static void theLimits(void)
{
    char buf[8192];
    char line[64];
    int i;
    size_t n = 0;

    /* one past the table, which is reported and dropped while every
       earlier entry stays */
    n += (size_t)snprintf(buf + n, sizeof(buf) - n, "[video]\n");
    for (i = 0; i < SET_MAXVIDEOS + 1; i++)
    {
        snprintf(line, sizeof(line), "%d = v%d.bik\n", i, i);
        n += (size_t)snprintf(buf + n, sizeof(buf) - n, "%s", line);
    }
    load(buf);
    okInt(set_videoNum, SET_MAXVIDEOS, "the table stops at its size");
    okStr(SET_pc_Video(0), "v0.bik", "the first entry is still there");
    okStr(SET_pc_Video(SET_MAXVIDEOS - 1), "v31.bik", "and so is the last that fit");
    okStr(SET_pc_Video(SET_MAXVIDEOS), NULL, "the one that did not fit is absent");
    ok(loggedAbout("dropped"), "and is reported");

    /* a name longer than the buffer is truncated, not written past */
    {
        char longName[SET_NAMEMAX + 64];

        memset(longName, 'a', sizeof(longName) - 1);
        longName[sizeof(longName) - 1] = '\0';
        snprintf(buf, sizeof(buf), "[data]\nbigfile = %s\n", longName);
        load(buf);
        okInt((int)strlen(SET_pc_BigFile()), SET_NAMEMAX - 1,
              "an over-long name is truncated to the buffer");
    }

    /* A line longer than the read buffer: the remainder must not come
       back round as a setting of its own.

       The padding is sized so that the cut falls exactly where a real
       setting begins, because that is the only arrangement the check
       can see. Anything shorter leaves the tail starting mid-padding,
       which is rejected as an unknown key whether the remainder was
       skipped or not -- so the case would pass either way. */
    {
        char line2[SET_LINEMAX];

        /* one comment line of exactly as much as fgets will take */
        line2[0] = ';';
        memset(line2 + 1, 'b', SET_LINEMAX - 2);
        line2[SET_LINEMAX - 1] = '\0';

        snprintf(buf, sizeof(buf),
                 "[data]\n%sbinarized = 1\nbigfile = after.bf\n", line2);
        load(buf);
        okInt(SET_b_Binarized(), 0,
              "the tail of an over-long line is not read as a setting");
        okStr(SET_pc_BigFile(), "after.bf", "and the next line still is");
    }
}

static void readingAgainStartsClean(void)
{
    load("[data]\nbigfile = first.bf\nbinarized = 1\n[video]\n5 = five.bik\n");
    okStr(SET_pc_BigFile(), "first.bf", "the first read takes effect");

    load("[data]\nbigfile = second.bf\n");
    okStr(SET_pc_BigFile(), "second.bf", "a second read replaces it");
    okInt(SET_b_Binarized(), 0, "and clears what the first read set");
    okStr(SET_pc_Video(5), NULL, "and the videos with it");
}

static void emptyValues(void)
{
    load("[data]\nbigfile =\n");
    okStr(SET_pc_BigFile(), NULL,
          "a setting with no value is the same as not setting it");

    load("[video]\n0 =\n");
    okStr(SET_pc_Video(0), NULL, "and so is a video with no file");
}

int main(void)
{
    printf("=== settings unit test ===\n");
    missingFileIsNotAProblem();
    theDisplaySettings();
    theSpecialArray();
    theFileBesideTheExecutable();
    theShippedFile();
    layout();
    quoting();
    theFlag();
    videoIds();
    idsAreThirtyTwoBitKeys();
    theRenderer();
    mistakesAreReported();
    theLimits();
    readingAgainStartsClean();
    emptyValues();

    remove(TMP);

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
