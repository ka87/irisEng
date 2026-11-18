#pragma once

/* The after-effects the scripts asked for, and what they asked of
 * them, with no renderer in it.
 *
 * This is the word of bits saying which effects are running, the
 * parameters saying how hard each one runs, and the two functions that
 * read them back. All of it is state the AI scripts move and both
 * renderers read, so it belongs to neither of them.
 *
 * It used to be declared here -- in the shared header -- and *defined*
 * in the Direct3D renderer's after-effects file, which had two
 * consequences worth naming because this file exists to end both.
 *
 * **The declarations were tentative definitions in a header.** Every
 * file that included it got storage of its own, merged with everyone
 * else's by a linker willing to treat them as common. That is an
 * extension both compilers happen to grant and neither promises; a
 * build that does not grant it -- which is now the default for one of
 * them -- gets one definition per translation unit and refuses to
 * link. Here they are declarations, and the storage is in one file.
 *
 * **A tentative definition cannot carry an initialiser**, so the two
 * parameters whose neutral value is a half took theirs from the
 * initialised copies beside the renderer, and were right only for as
 * long as that renderer was linked. Nought is not neutral for either:
 * it is a frame flattened to mid grey on the contrast and darkened by
 * half on the brightness. They are initialised here, in a file that is
 * linked whichever renderer is in force, so that hazard is gone rather
 * than documented.
 *
 * The scripts' way *in* is here too, and it is the one function of the
 * three that touches a renderer at all: a depth-blur setting moving
 * means a renderer may have something to rebuild, and starting a fade
 * sets a timer that one renderer happens to store. Both go out through
 * the engine's side of the interface rather than to a renderer by
 * name, which is what let the call leave the Direct3D file.
 *
 * The parameter names are the ones the scripts have always had, kept
 * as they are because they are the vocabulary six headers already
 * explain the effects in. Several say nothing about what they hold and
 * one says something untrue; each is answered where it is declared
 * below.
 */

/* Which effects are running, one bit each. The ids below are the
   scripts', not the bits: the getters map one to the other, and the
   bit for an effect bears no relation to its id. The depth blur's
   decision is built from this and the three below it -- see
   depthblur.h. */
extern int PC_AE_MASTER;

/* The brightness after-effect's parameter: a flat grey added to the
   picture above a half and subtracted below it, which is what makes
   the effect brightness and not something else. A half leaves the
   picture alone and is what being off looks like -- this effect has
   no bit in the word above. Its id is 8 for the parameter getter.

   The frozen copies in tests/ still spell it unkAEFunc2Param; the
   differential maps that spelling onto this one for its side, which
   is how the two functions renamed before it are handled. */
extern float brightnessParam;
/* The contrast's, id 9, and neutral at a half for the same reason.
   The colour effects built from these two are in aecolor.h; the
   engine puts both back to a half itself when they are switched
   off. */
extern float colorBalanceParam1;
/* How much of the grayscale, 0 to 1, with 0 meaning none of it. */
extern float blackAndWhiteParam1;

/* The four that scale the effects building a frame out of several
   samples of one. Neutral for all four is nought. The effects built
   from them are in aeframe.h.

   The last one is the odd one: it scales how far the blur *reaches*
   rather than how much of it is mixed in, so a value under a pixel
   spreads nothing -- and still costs brightness, because the taps are
   interpolated. aeframe.h has what that comes to. */
extern float motionBlurFactor;
extern float remenanceFactor;
extern float zoomSmoothFactor;
extern float blurFactor;
/* And the second zoom smooth's, which is a fifth of that family rather
   than a member of it: the effect it scales is a *rotational* smear
   about the middle of the picture, not a sum of scaled samples, and it
   is built in aezoom2.h. Neutral is nought here too, and a negative
   value is legal -- it turns the samples the other way. */
extern float zoomSmoothFactor2;

/* The depth blur's three settings, as a script asks for them. */
extern float depthBlurNear1;
extern float depthBlurType0;
extern float depthBlurType1;
/* The third type, which the getter hands back and nothing reads. */
extern float depthBlurType2;

/* The fade's duration, in seconds, as a script asks for it. Neutral is
   nought, and nought is also the one value a fade cannot be given --
   see aefade.h, where what it comes to is written down. */
extern float gAEOpacity;

/* The hue sweep's two, and the third the scripts can set and nothing
 * reads.
 *
 * The names are the ones they have always had and neither says what it
 * is: the first is how deep the tint goes and the second is where in
 * the cycle it sits -- a phase, not a texture factor. Neutral for the
 * depth is nought, which leaves a draw that takes nothing off, because
 * the effect is gated by its bit alone. See aehue.h.
 *
 * The third is set and read back by the scripts' own calls and is
 * never looked at by anything that draws. It is here so that reading
 * it back gives what was written. */
extern float unknownAEFlag4GlobalVar;
extern float texFactorManual;
extern float texFactorProbablyUnunsedType2;

/* Whether an effect is running, by the script's id for it. Nought is
   off; what comes back when it is on is the effect's bit and not one,
   which is what the scripts have always been given. */
int AEM_i_OnOff(int effect);

/* What an effect's first parameter currently is, by the same ids.
   Three of the effects have more than one and take a slot; the rest
   ignore it. An unknown effect gives nought, and so does an unknown
   slot -- with one exception, marked where it is. */
float AEM_f_Param(int effect, int slot);

/* The scripts' way in: switch an effect on or off and set one of its
   parameters, both in the one call, by the same ids again. An effect
   whose id this does not know is ignored.

   Two of the arms do more than write the state. A depth-blur setting
   that actually moves asks for the ramp to be rebuilt, and only then
   -- the arms test the value first, so setting a parameter to what it
   already holds costs nothing. Switching the fade on starts its timer,
   and only if one is not already running: switching it on again part
   way through leaves the timer where it is rather than starting
   over. */
void AEM_Control(int effect, int onOff, int slot, float value);

/* The engine's side of those two, which this module is told rather
   than reaching for itself. Both are in gdi.c -- the one file that
   knows both renderers -- and are declared in gdi.h as well; here
   because this file includes no engine header, which is what keeps it
   a leaf and keeps the shared renderer header's include of it from
   being a cycle. */
void GDI_DepthBlurRampRebuild(void);
int  GDI_i_AeFadeOpacity(void);
void GDI_AeFadeOpacitySet(int opacity);
