/* A unit test for renderers.c.
 *
 * New code with no earlier copy, so this is a direct check rather
 * than a differential one.
 *
 * The picker is the one piece of the renderer seam with logic in it,
 * and it is driven here through tables of its own rather than the
 * engine's -- so a case can say "this renderer cannot run" without
 * there being a machine that cannot run it, and the real table in
 * gdi.c stays free to be just a list.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

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

/* ------------------------------------------------------- the log, mocked */

static char dmLog[32][256];
static int  dmLogLen;

void DBG_OutputDebugString(char* format, ...)
{
    va_list ap;

    if (dmLogLen >= 32)
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

/* The CRT spelling the engine uses; the stub only declares it. */
int _stricmp(const char* a, const char* b)
{
    return strcasecmp(a, b);
}

#include "../renderers.c"

/* --------------------------------------------------------- the fake table */

static int fillCalls;

static void fillA(struct sGdi_DisplayData* dd) { (void)dd; fillCalls++; }

static int yes(void) { return 1; }
static int no(void)  { return 0; }

/* Set per case, so one table can stand for every combination. */
static int availD3D = 1;
static int availGL = 1;

static int d3dAvail(void) { return availD3D; }
static int glAvail(void)  { return availGL; }

/* gl first, so the table's order is visibly not alphabetical and not
   the order the cases ask in */
static const tGdi_Renderer twoRows[] = {
    { "gl",  "OpenGL 4.5", fillA, glAvail },
    { "d3d", "Direct3D 9", fillA, d3dAvail },
};
#define TWO ((int)(sizeof(twoRows) / sizeof(twoRows[0])))

static const tGdi_Renderer* pick(const char* asked)
{
    dmLogLen = 0;
    return GDI_pst_PickRenderer(twoRows, TWO, asked);
}

static void okPick(const char* asked, const char* wantName, const char* what)
{
    const tGdi_Renderer* r = pick(asked);

    checks++;
    if (!wantName)
    {
        if (r)
        {
            printf("  FAIL %s: got %s want none\n", what, r->name);
            fails++;
        }
        return;
    }
    if (!r || strcmp(r->name, wantName) != 0)
    {
        printf("  FAIL %s: got %s want %s\n", what,
               r ? r->name : "(none)", wantName);
        fails++;
    }
}

/* ------------------------------------------------------------- the checks */

static void nothingAskedFor(void)
{
    availD3D = 1;
    availGL = 1;

    /* the table's order is the preference, and gl is first in it */
    okPick(NULL, "gl", "nothing asked for takes the table's first row");
    okPick("", "gl", "and so does an empty name");

    /* An empty name is not a name nobody knows: `backend =` left
       blank in the ini is an ordinary way to ask for nothing, so it
       must not be reported as a renderer that does not exist. The
       result is the same either way, which is why this is a check on
       the log. */
    ok(dmLogLen == 0, "an empty name says nothing about unknown renderers");
    pick(NULL);
    ok(dmLogLen == 0, "and nor does none at all");
}

static void askedForByName(void)
{
    availD3D = 1;
    availGL = 1;

    okPick("d3d", "d3d", "a name picks its row");
    okPick("gl", "gl", "either way round");

    /* not the table's first row, so this would pass by accident if
       the name were ignored */
    ok(strcmp(twoRows[0].name, "d3d") != 0,
       "d3d is not the table's first row, so that check has teeth");
}

static void namesIgnoreCase(void)
{
    availD3D = 1;
    availGL = 1;

    okPick("D3D", "d3d", "a name ignores case");
    okPick("GL", "gl", "both of them");
    okPick("Direct3D", "d3d", "and so does an alias");
}

static void theAliases(void)
{
    availD3D = 1;
    availGL = 1;

    okPick("d3d9", "d3d", "d3d9 is d3d");
    okPick("direct3d", "d3d", "direct3d is d3d");
    okPick("directx", "d3d", "directx is d3d");
    okPick("opengl", "gl", "opengl is gl");
    okPick("ogl", "gl", "ogl is gl");

    /* An alias must reach only its own row. If the alias table were
       consulted without checking which row it belongs to, every
       alias would match every row and the first row would always
       win -- which is gl here, so asking for d3d9 is the case that
       catches it. */
    ok(pick("d3d9") == &twoRows[1], "an alias reaches only its own row");
}

static void anUnknownNameFallsBack(void)
{
    availD3D = 1;
    availGL = 1;

    okPick("vulkan", "gl", "an unknown name still leaves the game drawing");
    ok(loggedAbout("no renderer called"), "and says it did not know it");
    ok(loggedAbout("vulkan"), "naming what was asked for");
}

static void oneThatCannotRun(void)
{
    /* asked for the one that cannot run: the other one draws */
    availGL = 0;
    availD3D = 1;
    okPick("gl", "d3d", "a renderer that cannot run falls back to one that can");
    ok(loggedAbout("cannot run here"), "and says so");
    ok(loggedAbout("gl"), "naming it");

    /* and the message is about running, not about the name being
       unknown -- two different things to tell someone */
    ok(!loggedAbout("no renderer called"),
       "which is not the same as not knowing the name");

    /* the first row cannot run, so the preference order has to skip
       it even when nothing was asked for */
    availGL = 0;
    availD3D = 1;
    okPick(NULL, "d3d", "the preference order skips a row that cannot run");

    /* asked for the one that can, while the other cannot: no message */
    availGL = 0;
    availD3D = 1;
    okPick("d3d", "d3d", "asking for the one that can run is quiet");
    ok(dmLogLen == 0, "with nothing in the log");
}

static void whenNoneCanRun(void)
{
    availGL = 0;
    availD3D = 0;

    okPick(NULL, NULL, "no renderer that can run picks none");
    okPick("d3d", NULL, "even when one was asked for by name");
    ok(loggedAbout("cannot run here"), "and says why");
}

static void degenerateTables(void)
{
    dmLogLen = 0;
    ok(GDI_pst_PickRenderer(NULL, 1, "d3d") == NULL, "no table picks none");
    ok(GDI_pst_PickRenderer(twoRows, 0, "d3d") == NULL, "an empty table too");
    ok(GDI_pst_PickRenderer(twoRows, -1, "d3d") == NULL,
       "and a nonsense count rather than reading off the front");

    /* A row with no availability function is treated as unable to
       run rather than called through. */
    {
        static const tGdi_Renderer noFunc[] = {
            { "a", "A", fillA, NULL },
            { "b", "B", fillA, yes },
        };

        dmLogLen = 0;
        ok(GDI_pst_PickRenderer(noFunc, 2, "a") == &noFunc[1],
           "a row with no availability test is skipped");
        ok(GDI_pst_PickRenderer(noFunc, 2, NULL) == &noFunc[1],
           "including in the preference order");
    }

    /* and one that is there but says no */
    {
        static const tGdi_Renderer saysNo[] = {
            { "a", "A", fillA, no },
            { "b", "B", fillA, yes },
        };

        ok(GDI_pst_PickRenderer(saysNo, 2, NULL) == &saysNo[1],
           "a row that says no is skipped");
    }
}

/* The picker returns a row; it does not draw through it. Whoever
   calls it does that, which is what keeps this testable without a
   display. */
static void thePickerDoesNotFill(void)
{
    availD3D = 1;
    availGL = 1;
    fillCalls = 0;
    pick("d3d");
    ok(fillCalls == 0, "picking a renderer does not call its fill function");
}

int main(void)
{
    printf("=== renderer picker unit test ===\n");
    nothingAskedFor();
    askedForByName();
    namesIgnoreCase();
    theAliases();
    anUnknownNameFallsBack();
    oneThatCannotRun();
    whenNoneCanRun();
    degenerateTables();
    thePickerDoesNotFill();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
