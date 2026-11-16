/* A unit test for cinebands.c.
 *
 * The cutscene letterbox is two numbers and a gate, and the gate is
 * the part worth testing: it has two halves -- the engine's own switch
 * and the tweak that turns the whole thing off -- and dropping either
 * is invisible until a cutscene runs.
 *
 * The decision is also the one place both renderers are supposed to
 * agree. They do not share the code: the other renderer's copy is
 * spelled out in its after-effects, because calling this would remove
 * a call its differential test records. So the numbers here are the
 * ones that file has to match, and that is written down rather than
 * assumed.
 */

#include "../cinebands.h"

#include <stdio.h>
#include <string.h>

int   CIN_Bands;
int   tweak_ae_nobands;
float D3D_gf_CutsceneBand;

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

static void okF(float got, float want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %.9g, wanted %.9g\n", what, got, want);
        fails++;
    }
}

int main(void)
{
    tCbd_Bands b;

    /* The game's own value, which is what the other renderer's
       after-effects use too. */
    D3D_gf_CutsceneBand = 0.10416666f;

    printf("-- the gate\n");

    CIN_Bands = 0;
    tweak_ae_nobands = 0;
    CBD_Decide(&b);
    ok(!b.b_on, "no letterbox when the engine has not asked for one");
    okF(b.top, 0.0f, "and the strip is the whole screen");
    okF(b.bottom, 1.0f, "top to bottom");
    okF(b.reachTop, 0.0f, "with the reach the same");
    okF(b.reachBottom, 1.0f, "both ends");

    CIN_Bands = 1;
    CBD_Decide(&b);
    ok(b.b_on, "the engine's switch turns it on");

    tweak_ae_nobands = 1;
    CBD_Decide(&b);
    ok(!b.b_on, "and the tweak turns it off again");
    okF(b.top, 0.0f, "giving the whole screen back");
    okF(b.bottom, 1.0f, "both ends");
    tweak_ae_nobands = 0;

    printf("-- the strip\n");

    CBD_Decide(&b);
    okF(b.top, 0.10416666f, "the strip starts one band down");
    okF(b.bottom, 1.0f - 0.10416666f, "and ends one band up");
    ok(b.bottom > b.top, "which leaves something in between");

    /* The reach is wider than the strip at both ends, because a filter
       sampling across the edge needs something to sample. */
    ok(b.reachTop < b.top, "the reach starts above the strip");
    ok(b.reachBottom > b.bottom, "and ends below it");

    printf("-- the reach is clamped to the screen\n");

    /* A band narrower than the reach, which pushes both ends of the
       reach off the screen -- both, because a narrow band puts the top
       of the strip near nought and the bottom near one. Asking about
       the bottom with a *wide* band instead was the first draft's
       mistake: a wide band moves the bottom of the strip towards the
       middle, which is where the reach has the most room. */
    D3D_gf_CutsceneBand = 0.01f;
    CBD_Decide(&b);
    okF(b.reachTop, 0.0f, "the reach stops at the top of the screen");
    okF(b.reachBottom, 1.0f, "and at the bottom");

    /* And a band with room either side is not clamped, so the clamps
       above are the clamps and not the arithmetic. */
    D3D_gf_CutsceneBand = 0.25f;
    CBD_Decide(&b);
    ok(b.reachTop > 0.0f && b.reachBottom < 1.0f,
       "a band with room either side keeps its whole reach");

    /* A band of half the screen leaves no strip at all, which is a
       thing to answer rather than to divide by. */
    D3D_gf_CutsceneBand = 0.5f;
    CBD_Decide(&b);
    ok(b.bottom <= b.top, "half a screen each way leaves no strip");

    printf("-- what it declines to do\n");
    CBD_Decide(NULL);
    ok(1, "no output, no crash");

    printf("\n  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
