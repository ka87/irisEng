#pragma once

/*
    Which renderer the game draws through.

    Everything the engine draws with is the interface in
    tGdi_DisplayData: a renderer is a row in a table plus the function
    that writes that interface, and nothing else in the engine learns
    which one it got. Adding a renderer is a row and a fill function.

    The name is what engine.ini asks for under [render] backend.
    A renderer that cannot run on the machine in front of it is not an
    error -- the setting is a preference, and the game falls back to
    the first renderer in the table that can run.
*/

struct sGdi_DisplayData;

typedef struct sGdi_Renderer
{
    const char* name;           /* what the ini asks for */
    const char* description;    /* what the log says */
    void (*FillInterface)(struct sGdi_DisplayData* dd);
    int  (*b_Available)(void);  /* can it run on this machine */
} tGdi_Renderer;

/* The row to use: the one asked for if it can run, else the first in
   the table that can, else NULL when none can. Says in the debug
   output when it could not give what was asked for.

   `asked` may be NULL or empty, which asks for nothing in
   particular. */
const tGdi_Renderer* GDI_pst_PickRenderer(const tGdi_Renderer* table, int num,
                                          const char* asked);
