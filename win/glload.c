#include "glload.h"

#include <stdio.h>
#include <string.h>

/* The pointers themselves, one per entry in the list. */
#define GL_DEFINE(ret, name, params, args) ret (*GLFN_##name) params;
GL_FUNCTION_LIST(GL_DEFINE)
#undef GL_DEFINE

void* GL_p_Resolve(const char* name, GL_pfn_GetProc fromContext,
                   GL_pfn_GetProc fromModule)
{
    void* p = NULL;

    if (!name)
        return NULL;

    if (fromContext)
    {
        p = fromContext(name);

        /* The sentinels. Comparing against each rather than treating
           anything small as a failure: a real address could in
           principle be low, and the documented set is this short. */
        if (p == (void*)-1 || p == (void*)(size_t)1 || p == (void*)(size_t)2
            || p == (void*)(size_t)3)
            p = NULL;
    }

    if (!p && fromModule)
        p = fromModule(name);
    return p;
}

int GL_i_LoadFunctions(GL_pfn_GetProc fromContext, GL_pfn_GetProc fromModule,
                       char* missing, size_t missingSize)
{
    int   absent = 0;
    const char* first = NULL;

    if (missing && missingSize)
        missing[0] = 0;

#define GL_LOAD(ret, name, params, args)                                      \
    GLFN_##name = (ret(*) params)GL_p_Resolve(#name, fromContext, fromModule); \
    if (!GLFN_##name)                                                         \
    {                                                                         \
        absent++;                                                             \
        if (!first)                                                           \
            first = #name;                                                    \
    }
    GL_FUNCTION_LIST(GL_LOAD)
#undef GL_LOAD

    if (first && missing && missingSize)
    {
        size_t n = strlen(first);

        if (n >= missingSize)
            n = missingSize - 1;
        memcpy(missing, first, n);
        missing[n] = 0;
    }
    return absent;
}

void GL_ForgetFunctions(void)
{
#define GL_CLEAR(ret, name, params, args) GLFN_##name = NULL;
    GL_FUNCTION_LIST(GL_CLEAR)
#undef GL_CLEAR
}

int GL_b_VersionAtLeast(const char* version, int major, int minor)
{
    int gotMajor = 0;
    int gotMinor = 0;
    const char* p = version;

    if (!p)
        return 0;

    while (*p == ' ')
        p++;

    /* It has to start with a number. That is the contract, and it is
       also what rejects an OpenGL ES string: ES reports
       "OpenGL ES 3.2 <vendor>", which is a different API carrying the
       same version numbers, and it fails here on the first character
       rather than needing a case of its own. A first draft had that
       case, and it could not fire. */
    if (*p < '0' || *p > '9')
        return 0;
    while (*p >= '0' && *p <= '9')
        gotMajor = gotMajor * 10 + (*p++ - '0');
    if (*p != '.')
        return 0;
    p++;
    if (*p < '0' || *p > '9')
        return 0;
    while (*p >= '0' && *p <= '9')
        gotMinor = gotMinor * 10 + (*p++ - '0');

    if (gotMajor != major)
        return gotMajor > major;
    return gotMinor >= minor;
}
