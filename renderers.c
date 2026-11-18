#include "renderers.h"
#include "dbg.h"

#include <string.h>

/* The longer spellings people reach for, mapped onto a row's own
   name. A row is found by its name or by any alias of it, so the
   table stays short and the ini stays forgiving. */
static const struct { const char* name; const char* also; } gdi_alias[] = {
    { "d3d", "d3d9" },
    { "d3d", "direct3d" },
    { "d3d", "directx" },
    { "gl",  "opengl" },
    { "gl",  "ogl" },
};

static int GDI_l_b_NameIs(const char* name, const char* asked)
{
    unsigned i;

    if (strcasecmp(name, asked) == 0)
        return 1;
    for (i = 0; i < sizeof(gdi_alias) / sizeof(gdi_alias[0]); i++)
    {
        if (strcasecmp(gdi_alias[i].name, name) == 0
            && strcasecmp(gdi_alias[i].also, asked) == 0)
            return 1;
    }
    return 0;
}

const tGdi_Renderer* GDI_pst_PickRenderer(const tGdi_Renderer* table, int num,
                                          const char* asked)
{
    int i;

    if (!table || num <= 0)
        return NULL;

    /* What was asked for, if it can run. A renderer that cannot run
       here is not a failure: the setting is a preference, so this
       says so and falls through to whatever can. */
    if (asked && *asked)
    {
        for (i = 0; i < num; i++)
        {
            if (!GDI_l_b_NameIs(table[i].name, asked))
                continue;
            if (table[i].b_Available && table[i].b_Available())
                return &table[i];
            DBG_OutputDebugString("GDI: renderer \"%s\" cannot run here\n",
                                  asked);
            asked = NULL;
            break;
        }
        if (asked)
            DBG_OutputDebugString("GDI: no renderer called \"%s\"\n", asked);
    }

    /* Whatever was asked for, the game still has to draw: the first
       renderer in the table that can run. The table's order is the
       preference. */
    for (i = 0; i < num; i++)
    {
        if (table[i].b_Available && table[i].b_Available())
            return &table[i];
    }
    return NULL;
}
