/* The log's timestamp, its banner and its closing line.
 *
 * Three things are worth asserting here and only one of them is the
 * stamp's shape.
 *
 * The first is **where** the stamp goes. It belongs to the two
 * formatted loggers, DBG_OutputDebugString and GDI_OutputDebugString,
 * and not to the sink they share with the dump helpers:
 * DBG_DumpVecNoNL exists to write part of a line and DBG_DumpMatrix
 * writes four rows in one call, so a stamp in the sink would cut one
 * into the middle of a line and put four on a matrix. Those two facts
 * are what the cases below pin.
 *
 * The second is that the stamp is **on top of** the message's room
 * rather than taken out of it: a message that fitted before still
 * fits.
 *
 * The third is the banner, which is read by a person looking for the
 * start of a run, so its shape matters: a rule, one line carrying the
 * version, the build stamp and the architecture, and a rule.
 *
 * The log is read back from the file the engine really writes, not
 * from a captured string, so the sink and the flush are in the path
 * too. Where that file is comes from the path module, which this
 * harness answers through tests/sdlpath.h: the player's own directory
 * is pointed at one the runner makes under tests/build, so the cases
 * read the file through the same rule the game writes it through
 * rather than through a copy of the rule.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "sdlpath.h"

/* What the engine gets from the platform and this harness has to
   supply. OutputDebugStringA is where a debugger would see the same
   line; the log file is what the cases read, so it is enough to count
   the calls and prove the two sinks get the same string. */
static int  mockDbgStrings;
static char mockLastDbgString[512];

void OutputDebugStringA(const char* text)
{
    mockDbgStrings++;
    strncpy(mockLastDbgString, text ? text : "",
            sizeof(mockLastDbgString) - 1);
    mockLastDbgString[sizeof(mockLastDbgString) - 1] = 0;
}

int MessageBoxA(void* h, const char* text, const char* caption,
                unsigned int type)
{
    (void)h; (void)text; (void)caption; (void)type;
    return 0;
}

int _snprintf(char* buf, size_t count, const char* format, ...)
{
    va_list va;
    int     n;

    va_start(va, format);
    n = vsnprintf(buf, count, format, va);
    va_end(va);
    return n;
}

int _vsnprintf(char* buf, size_t count, const char* format, va_list args)
{
    return vsnprintf(buf, count, format, args);
}

#include "../dbg.c"

static int checks;
static int failures;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        failures++;
        printf("  FAIL %s\n", what);
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        failures++;
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
    }
}

/* What is in the log so far, as one string. */
static char  logText[65536];
static int   logLen;

/* Where the engine put it, asked the same way the engine asks. */
static const char* logPath(void)
{
    static char path[512];

    if (!DPT_b_UserPath("debug.log", path, sizeof(path)))
        path[0] = 0;
    return path;
}

static void readLog(void)
{
    FILE* f = fopen(logPath(), "rb");
    size_t n = 0;

    logText[0] = 0;
    logLen = 0;
    if (!f)
        return;
    n = fread(logText, 1, sizeof(logText) - 1, f);
    logText[n] = 0;
    logLen = (int)n;
    fclose(f);
}

/* The log's lines, split. */
#define MAXLINES 64
static char* lines[MAXLINES];
static int   lineNum;

static void splitLog(void)
{
    char* p = logText;

    readLog();
    lineNum = 0;
    p = logText;
    while (*p && lineNum < MAXLINES)
    {
        char* nl = strchr(p, '\n');

        lines[lineNum++] = p;
        if (!nl)
            break;
        *nl = 0;
        p = nl + 1;
    }
}

/* "[hh:mm:ss] " and nothing else -- the digits are not compared
   against the clock, only their shape, because a test that read the
   clock twice could straddle a second. */
static int isStamp(const char* s)
{
    int i;
    static const char* shape = "[dd:dd:dd] ";

    for (i = 0; shape[i]; i++)
    {
        if (!s[i])
            return 0;
        if (shape[i] == 'd')
        {
            if (s[i] < '0' || s[i] > '9')
                return 0;
        }
        else if (s[i] != shape[i])
        {
            return 0;
        }
    }
    return 1;
}

static int countStamps(const char* s)
{
    int n = 0;

    while (*s)
    {
        if (*s == '[' && isStamp(s))
            n++;
        s++;
    }
    return n;
}

static void caseStampShape(void)
{
    printf("-- a logged line opens with the time of day\n");

    DBG_OutputDebugString("hello %d\n", 7);
    splitLog();

    ok(lineNum >= 1, "something was logged");
    if (lineNum >= 1)
    {
        ok(isStamp(lines[0]), "the line opens with a stamp");
        okEq((long)strcmp(lines[0] + 11, "hello 7"), 0,
             "and the message follows it");
        okEq(countStamps(lines[0]), 1, "with one stamp and no more");
    }
}

static void caseBothLoggers(void)
{
    int at;

    printf("-- and so does one from the renderer's logger\n");

    at = lineNum;
    GDI_OutputDebugString("from the renderer %s\n", "here");
    splitLog();

    ok(lineNum > at, "it logged");
    if (lineNum > at)
    {
        ok(isStamp(lines[at]), "that line opens with a stamp too");
        okEq((long)strcmp(lines[at] + 11, "from the renderer here"), 0,
             "and carries its message");
    }
}

static void caseDumpsAreNotStamped(void)
{
    int  at;
    Vec3 v;

    printf("-- the dump helpers are left alone\n");

    v.x = 1.0f;
    v.y = 2.0f;
    v.z = 3.0f;

    /* A vector written without a newline, then the rest of the line
       through the stamped logger. Were the stamp in the sink, it would
       land in the middle of this line -- and four times over on the
       matrix below. */
    at = lineNum;
    DBG_DumpVecNoNL(&v);
    DBG_OutputDebugString(" <- that one\n");
    splitLog();

    ok(lineNum > at, "the pieces reached the log");
    if (lineNum > at)
    {
        ok(!isStamp(lines[at]),
           "a line a dump helper started has no stamp");
        okEq(countStamps(lines[at]), 1,
             "and the one from the logger that finished it is the only"
             " stamp on it");
        ok(strstr(lines[at], "1.000") != NULL,
           "the vector itself is there");
    }

    /* And a matrix, which is four rows and a flags line from one
       call. */
    at = lineNum;
    {
        Mtx4x4 m;
        int    i;

        memset(&m, 0, sizeof(m));
        DBG_DumpMatrix(&m);
        splitLog();
        ok(lineNum >= at + 5, "the matrix wrote its five lines");
        for (i = at; i < lineNum && i < at + 5; i++)
            ok(!isStamp(lines[i]), "none of them is stamped");
    }
}

static void caseMessageRoomIsUnchanged(void)
{
    char  big[400];
    int   at;
    int   i;

    printf("-- the stamp's room is on top of the message's\n");

    /* 250 characters and a newline: a message that fitted in the old
       256-byte buffer. It has to survive whole, which is what says the
       stamp was added to the buffer rather than taken out of the
       message. */
    for (i = 0; i < 250; i++)
        big[i] = 'x';
    big[250] = 0;

    at = lineNum;
    DBG_OutputDebugString("%s\n", big);
    splitLog();

    ok(lineNum > at, "the long message was logged");
    if (lineNum > at)
    {
        ok(isStamp(lines[at]), "stamped as well");
        okEq((long)strlen(lines[at] + 11), 250,
             "and all 250 characters of it are there");
    }
}

static void caseBanner(void)
{
    int at;

    printf("-- the banner says what this is, when it was built and"
           " what for\n");

    at = lineNum;
    DBG_LogBanner();
    splitLog();

    ok(lineNum >= at + 3, "three lines");
    if (lineNum >= at + 3)
    {
        ok(isStamp(lines[at]), "the first is stamped");
        ok(strncmp(lines[at] + 11, "====", 4) == 0,
           "and is a rule");
        ok(strncmp(lines[at + 2] + 11, "====", 4) == 0,
           "so is the third, so the middle one is between two rules");

        /* The middle line's four parts. */
        ok(strncmp(lines[at + 1] + 11, "IRISEng v. ", 11) == 0,
           "the middle line names the engine");
        ok(strstr(lines[at + 1], IRISENG_VERSION_STRING) != NULL,
           "and its version");
        ok(strstr(lines[at + 1], IRISENG_ARCH) != NULL,
           "and what it was built for");
        ok(strstr(lines[at + 1], ", built ") != NULL,
           "and when it was built");

        /* The version string is the three numbers and not a second
           spelling of them: a header that let them drift would show
           up here. */
        {
            char want[32];

            sprintf(want, "%d.%d.%d", IRISENG_VERSION_MAJOR,
                    IRISENG_VERSION_MINOR, IRISENG_VERSION_PATCH);
            okEq((long)strcmp(IRISENG_VERSION_STRING, want), 0,
                 "and the version string is those three numbers");
        }
        /* An unrecognised architecture is reported as unknown rather
           than guessed, and this harness is built on one the header
           knows, so the honest-unknown branch is not what is being
           read above. */
        okEq((long)strcmp(IRISENG_ARCH, "unknown arch") != 0, 1,
             "the architecture was recognised");
    }
}

static void caseExit(void)
{
    int at;

    printf("-- and one line at the end\n");

    at = lineNum;
    DBG_LogExit();
    splitLog();

    ok(lineNum > at, "the exit line was logged");
    if (lineNum > at)
    {
        ok(isStamp(lines[at]), "stamped like the rest");
        ok(strstr(lines[at], "exiting") != NULL, "and says it is exiting");
    }
}

int main(void)
{
    /* The log is written into the player's own directory, so the
       harness points that at one of its own under tests/build: the
       engine's name for the file is fixed and a test has no business
       writing it into the tree. */
    const char* dir = getenv("DBGLOG_DIR");

    if (!dir)
    {
        printf("  FAIL no scratch directory was named\n");
        return 1;
    }
    snprintf(dpt_prefPath, sizeof(dpt_prefPath), "%s/", dir);
    remove(logPath());

    /* The log goes where the saves go, which is what the rest of the
       cases then read it from. Asserted rather than assumed: a path
       module that answered with the working directory would leave
       every case below passing against a file in the tree. */
    if (strncmp(logPath(), dpt_prefPath, strlen(dpt_prefPath)) != 0)
    {
        printf("  FAIL the log is not in the player's directory: %s\n",
               logPath());
        return 1;
    }

    caseStampShape();
    caseBothLoggers();
    caseDumpsAreNotStamped();
    caseMessageRoomIsUnchanged();
    caseBanner();
    caseExit();

    printf("  %d checks, %d failure(s)\n", checks, failures);
    return failures ? 1 : 0;
}
