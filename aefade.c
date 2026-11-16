#include "aefade.h"

/* Frames a second, which is what turns the duration a script asks for
   into a number of steps. Not measured and not adjustable: the other
   renderer multiplies by this constant whatever the machine is
   actually running at, so a fade is a count of frames wearing the
   units of seconds. */
#define AFD_FPS         60.0

/* The largest step that can be converted to a whole number, which is
   where the division has to stop rather than hand on something out of
   range. Anything this large takes the whole opacity in one frame
   anyway, so the ceiling costs nothing a fade could notice. */
#define AFD_STEP_MAX    2147483648.0
#define AFD_STEP_MIN    (-2147483649.0)

int AFD_b_On(unsigned int aeMaster)
{
    return (aeMaster & AFD_BIT) != 0u;
}

unsigned int AFD_u_Cleared(unsigned int aeMaster)
{
    return aeMaster & ~AFD_BIT;
}

float AFD_f_Step(float seconds)
{
    double span;
    double step;

    span = (double)seconds * AFD_FPS;

    /* The clamp that keeps a long fade from freezing the screen for
       good -- see the header, it is not a tidy-up.
     *
       Written "no less than" and strict would do just as well: at a
       span of exactly the range the division gives one anyway, so no
       test can tell the two apart and none is missing. What *can* be
       seen is the threshold itself, and the unit test pins it at half
       the range, where the true step is two and a lower clamp would
       flatten it to one. */
    if (span >= (double)AFD_FULL)
        return 1.0f;

    /* Nought cannot be divided by. The test is here and not left to
       the range check below because a division by nought is the one
       case the language does not promise an infinity for -- and so it
       cannot be killed by a test: let the division happen and the
       infinity it gives on this machine fails the range check and
       answers nought all the same. It is here for the machine that
       does something else. */
    if (span == 0.0)
        return 0.0f;

    step = (double)AFD_FULL / span;

    /* Out of the range a whole number can hold, which only a span far
       below one frame reaches -- and, written as a pair of
       comparisons rather than their negation, a span that is not a
       number at all. Nought in both cases, which takes nothing. */
    if (!(step > AFD_STEP_MIN && step < AFD_STEP_MAX))
        return 0.0f;

    return (float)step;
}

unsigned int AFD_u_Taken(float step)
{
    /* Truncated toward nought through a signed whole number and then
       read as unsigned, which is modular below nought: that is what
       makes a negative step come out large and the subtraction in
       AFD_i_Next wrap it into an increase.
     *
       The range test is a guard on a public function and nothing
       reachable drives it: every step AFD_f_Step hands back is either
       in range or exactly nought. It stands in front of the one
       conversion the other renderer leaves undefined, which is why it
       is a test and not a cast -- and the unit test drives it
       directly, since the rest of the code cannot. */
    if (!((double)step > AFD_STEP_MIN && (double)step < AFD_STEP_MAX))
        return 0u;
    return (unsigned int)(int)step;
}

int AFD_i_Next(int opacity, float step)
{
    int next;

    /* Unsigned all the way and then read back as signed, so that an
       amount larger than the opacity shows up as a negative and is
       caught, while an amount that wrapped shows up as the increase
       it is.
     *
       Spelling it the other way round -- subtracting the amount read
       as a signed number -- gives the same answer for every input,
       because an amount that wrapped past the top of the word reads
       back as exactly the small negative it came from. So the choice
       of spelling is not something a test can see; this one is kept
       because it is the shape the other renderer's own subtraction
       has.
     *
       The clamp is "below nought" and "below one" would do as well,
       since nought answers nought either way. What it is really for
       is the short fade whose step is larger than the opacity it
       comes off -- without it the timer wraps to the top of the word
       and the fade runs for billions of frames. */
    next = (int)((unsigned int)opacity - AFD_u_Taken(step));
    if (next < 0)
        return 0;
    return next;
}

int AFD_b_Advance(int* opacity, float step)
{
    if (!opacity)
        return 0;

    /* An opacity of nothing cannot step anywhere, so stepping first
       and asking afterwards gives the same answer as the other
       renderer's test-step-test -- and so does dropping this test
       altogether, which is why no case here kills it. It is kept
       because it is what the other renderer writes. */
    if (*opacity > 0)
        *opacity = AFD_i_Next(*opacity, step);
    return *opacity == 0;
}

int AFD_b_Freeze(int opacity)
{
    return opacity == AFD_FULL;
}

float AFD_f_Blend(int opacity)
{
    return (float)opacity / (float)AFD_FULL;
}

unsigned int AFD_u_TextureFactor(int opacity)
{
    return ((unsigned int)opacity << 24) | 0xFFFFFFu;
}
