/* A unit test for win/glload.c.
 *
 * New code with no earlier copy, so a direct check rather than a
 * differential one.
 *
 * The loader takes its two ways of resolving a name as arguments,
 * which is the whole reason this test can exist: a driver's
 * wglGetProcAddress is not something a test can arrange, but the
 * behaviour that matters -- which of the two lookups answers, what a
 * failure comes back as, what happens when one name is missing -- is
 * all expressible here.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../win/glload.h"

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

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

static void okStr(const char* got, const char* want, const char* what)
{
    checks++;
    if (strcmp(got, want) != 0)
    {
        printf("  FAIL %s: got \"%s\" want \"%s\"\n", what, got, want);
        fails++;
    }
}

/* ------------------------------------------- the two lookups, mocked */

/* Every name the list holds, collected by walking the same list the
   loader walks -- so a function added to glcore.h cannot be missed
   here. */
static const char* glNames[] = {
#define GL_NAME(ret, name, params, args) #name,
    GL_FUNCTION_LIST(GL_NAME)
#undef GL_NAME
};
#define GLNAMECOUNT ((int)(sizeof(glNames) / sizeof(glNames[0])))

/* Distinct fake addresses, so a case can tell which lookup answered
   and whether the right pointer landed in the right variable. */
static void* ctxAddr(int i) { return (void*)(size_t)(0x10000 + i); }
static void* modAddr(int i) { return (void*)(size_t)(0x20000 + i); }

static int ctxCalls;
static int modCalls;

/* what the context lookup should do for each name */
enum { ANSWER, REFUSE_NULL, REFUSE_ONE, REFUSE_TWO, REFUSE_THREE, REFUSE_MINUSONE };
static int ctxMode[64];
static int modAnswers;          /* does the module lookup answer at all */
static int modRefusesIndex;     /* one name the module also refuses, or -1 */

static int nameIndex(const char* name)
{
    int i;

    for (i = 0; i < GLNAMECOUNT; i++)
        if (strcmp(glNames[i], name) == 0)
            return i;
    return -1;
}

static void* fromContext(const char* name)
{
    int i = nameIndex(name);

    ctxCalls++;
    if (i < 0)
        return NULL;
    switch (ctxMode[i])
    {
        case REFUSE_NULL:     return NULL;
        case REFUSE_ONE:      return (void*)(size_t)1;
        case REFUSE_TWO:      return (void*)(size_t)2;
        case REFUSE_THREE:    return (void*)(size_t)3;
        case REFUSE_MINUSONE: return (void*)-1;
        default:              return ctxAddr(i);
    }
}

static void* fromModule(const char* name)
{
    int i = nameIndex(name);

    modCalls++;
    if (!modAnswers || i < 0 || i == modRefusesIndex)
        return NULL;
    return modAddr(i);
}

static void reset(void)
{
    int i;

    for (i = 0; i < 64; i++)
        ctxMode[i] = ANSWER;
    modAnswers = 1;
    modRefusesIndex = -1;
    ctxCalls = 0;
    modCalls = 0;
    GL_ForgetFunctions();
}

/* The loaded pointers, read back through the same list. */
static void* loaded(int i)
{
    void* table[] = {
#define GL_PTR(ret, name, params, args) (void*)GLFN_##name,
        GL_FUNCTION_LIST(GL_PTR)
#undef GL_PTR
    };

    return table[i];
}

/* ---------------------------------------------------------- the cases */

static void caseResolveOrder(void)
{
    printf("-- which lookup answers\n");
    reset();

    /* the context first, and the module not asked at all */
    ok(GL_p_Resolve("glClear", fromContext, fromModule) != NULL,
       "a name the context knows resolves");
    okEq(modCalls, 0, "and the module is not consulted");

    /* and when the context refuses, the module is */
    reset();
    ctxMode[nameIndex("glClear")] = REFUSE_NULL;
    okEq((long)(size_t)GL_p_Resolve("glClear", fromContext, fromModule),
         (long)(size_t)modAddr(nameIndex("glClear")),
         "a name the context refuses comes from the module");
    okEq(modCalls, 1, "which was consulted once");

    /* neither */
    reset();
    modAnswers = 0;
    ctxMode[nameIndex("glClear")] = REFUSE_NULL;
    ok(GL_p_Resolve("glClear", fromContext, fromModule) == NULL,
       "a name neither knows is NULL");

    /* either lookup may be absent */
    reset();
    ok(GL_p_Resolve("glClear", NULL, fromModule) != NULL,
       "no context lookup is survivable");
    ok(GL_p_Resolve("glClear", fromContext, NULL) != NULL,
       "and so is no module lookup");
    ok(GL_p_Resolve(NULL, fromContext, fromModule) == NULL, "no name is NULL");
}

static void caseSentinels(void)
{
    static const struct { int mode; const char* what; } table[] = {
        { REFUSE_ONE,      "1" },
        { REFUSE_TWO,      "2" },
        { REFUSE_THREE,    "3" },
        { REFUSE_MINUSONE, "-1" },
    };
    unsigned k;

    printf("-- the sentinels a failed lookup can come back as\n");

    /* Each of these is a failure that is not NULL. Taken at face
       value it is a function pointer that crashes on the first call,
       a long way from the loader. */
    for (k = 0; k < sizeof(table) / sizeof(table[0]); k++)
    {
        int i;

        reset();
        i = nameIndex("glClear");
        ctxMode[i] = table[k].mode;
        okEq((long)(size_t)GL_p_Resolve("glClear", fromContext, fromModule),
             (long)(size_t)modAddr(i), table[k].what);

        /* and with no module to fall back to, it is NULL and not the
           sentinel */
        reset();
        ctxMode[i] = table[k].mode;
        ok(GL_p_Resolve("glClear", fromContext, NULL) == NULL,
           "the sentinel does not escape as a pointer");
    }
}

static void caseLoadAll(void)
{
    int i;
    int bad = 0;

    printf("-- loading the whole list\n");
    reset();

    ok(GLNAMECOUNT > 5, "the list has entries in it at all");
    okEq(GL_i_LoadFunctions(fromContext, fromModule, NULL, 0), 0,
         "nothing is missing when both lookups answer");

    /* every pointer filled, and filled with its own name's address --
       a loader that assigned them all the same way round but shifted
       by one would otherwise score */
    for (i = 0; i < GLNAMECOUNT; i++)
        if (loaded(i) != ctxAddr(i))
            bad++;
    okEq(bad, 0, "every pointer holds the address of its own name");

    okEq(ctxCalls, GLNAMECOUNT, "the context was asked once per name");

    /* and they are cleared on request, because a pointer belonging to
       a context that has gone is worse than a NULL */
    GL_ForgetFunctions();
    bad = 0;
    for (i = 0; i < GLNAMECOUNT; i++)
        if (loaded(i) != NULL)
            bad++;
    okEq(bad, 0, "forgetting them clears every one");
}

static void caseMissing(void)
{
    char missing[64];
    int  i;

    printf("-- a driver short of an entry point\n");

    /* one name that neither lookup knows */
    reset();
    i = nameIndex("glDebugMessageCallback");
    ctxMode[i] = REFUSE_NULL;
    modRefusesIndex = i;
    okEq(GL_i_LoadFunctions(fromContext, fromModule, missing, sizeof(missing)), 1,
         "one missing is counted as one");
    okStr(missing, "glDebugMessageCallback", "and named");
    ok(loaded(i) == NULL, "its pointer is left NULL");
    ok(loaded(nameIndex("glClear")) != NULL, "the others still loaded");

    /* nothing resolves at all */
    reset();
    modAnswers = 0;
    for (i = 0; i < GLNAMECOUNT; i++)
        ctxMode[i] = REFUSE_NULL;
    okEq(GL_i_LoadFunctions(fromContext, fromModule, missing, sizeof(missing)),
         GLNAMECOUNT, "all of them missing is counted as all of them");
    okStr(missing, glNames[0], "and the first one is the one named");

    /* the name is the first missing, not the last */
    reset();
    ctxMode[nameIndex("glViewport")] = REFUSE_NULL;
    modRefusesIndex = nameIndex("glViewport");
    GL_i_LoadFunctions(fromContext, fromModule, missing, sizeof(missing));
    okStr(missing, "glViewport", "the name reported is the one that failed");

    /* a buffer too small to hold it is truncated, not overrun */
    reset();
    ctxMode[nameIndex("glViewport")] = REFUSE_NULL;
    modRefusesIndex = nameIndex("glViewport");
    {
        char small[5];

        memset(small, 'x', sizeof(small));
        GL_i_LoadFunctions(fromContext, fromModule, small, sizeof(small));
        okStr(small, "glVi", "a short buffer truncates and still terminates");
    }

    /* and no buffer at all is allowed */
    reset();
    ctxMode[nameIndex("glViewport")] = REFUSE_NULL;
    modRefusesIndex = nameIndex("glViewport");
    okEq(GL_i_LoadFunctions(fromContext, fromModule, NULL, 0), 1,
         "no buffer is survivable");
}

static void caseVersion(void)
{
    static const struct
    {
        const char* text;
        int         want;       /* at least 4.5 */
        const char* what;
    } table[] = {
        { "4.5",                            1, "exactly the version asked for" },
        { "4.5.0",                          1, "with a release number" },
        { "4.6.0 NVIDIA 551.23",            1, "a real NVIDIA string" },
        { "4.5 (Core Profile) Mesa 23.2.1", 1, "a real Mesa string" },
        { "4.6.0 Compatibility Profile Context 23.9", 1, "a real AMD string" },
        { "4.4.0",                          0, "one minor version short" },
        { "3.3.0 NVIDIA",                   0, "a major version short" },
        { "4.10",                           1, "a two-digit minor is ten, not one" },
        { "10.0",                           1, "a two-digit major" },
        { "  4.5",                          1, "leading spaces" },
        { "OpenGL ES 4.5",                  0, "ES is a different API" },
        { "OpenGL ES 3.2 Mesa",             0, "and so is a newer ES" },
        { "",                               0, "an empty string" },
        { "x4.5",                           0, "something that is not a number" },
        { "4",                              0, "a major with no minor" },
        { "4.",                             0, "a dot with no minor" },
        { "4x5",                            0, "no dot at all" },
    };
    unsigned k;

    printf("-- the version string\n");

    for (k = 0; k < sizeof(table) / sizeof(table[0]); k++)
        okEq(GL_b_VersionAtLeast(table[k].text, 4, 5), table[k].want,
             table[k].what);

    ok(!GL_b_VersionAtLeast(NULL, 4, 5), "no string is not good enough");

    /* It must *begin* with a number. Without that, ".9" would read as
       0.9 rather than as nonsense -- which no caller asks about,
       since nobody wants to know whether a driver is at least 0.5,
       but it is the contract the parse rests on and it is the line
       that rejects an OpenGL ES string. */
    ok(!GL_b_VersionAtLeast(".9", 0, 5), "a string starting with a dot is not a version");
    ok(!GL_b_VersionAtLeast(" .9", 0, 5), "nor is one after a space");
    ok(!GL_b_VersionAtLeast("OpenGL ES 3.2", 0, 1),
       "and that is what turns an ES string away, at the first character");

    /* the comparison itself, not just at 4.5 */
    ok(GL_b_VersionAtLeast("3.3", 3, 3), "3.3 meets 3.3");
    ok(!GL_b_VersionAtLeast("3.2", 3, 3), "3.2 does not");
    ok(GL_b_VersionAtLeast("4.0", 3, 3), "4.0 does, on the major alone");
    ok(!GL_b_VersionAtLeast("3.9", 4, 0), "and a high minor does not carry it");
}

int main(void)
{
    printf("=== GL loader unit test ===\n");
    caseResolveOrder();
    caseSentinels();
    caseLoadAll();
    caseMissing();
    caseVersion();
    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
