#pragma once
#include "common.h"
#include "view.h"

/*
    A free-flying camera for looking around a running level.

    It is built on the engine's own two mechanisms rather than beside
    them:

    The follow parameter. A view follows its assigned object through
    tViewElem.viewFunctionPtr, which the engine calls once a frame and
    which WOR_View_FollowFather fills from the object's absolute
    matrix. Turning this on swaps that one pointer for a function that
    fills the view from the free camera's own matrix instead, and
    turning it off puts the original back.

    That function is also where the game is kept from setting the
    camera's position, sight and field of view. The engine calls it
    from the view loop just before WOR_SetCam hands the view to the
    device, so what it leaves behind is what is drawn. Position and
    sight reach a view only through WOR_View_SetViewPoint, whose only
    other caller is the follow function this one displaced. The focale
    is a plain field the game writes directly, so it is noted and
    replaced: the camera imposes the one it took, and standing down
    gives back whatever the game last asked for.

    assignedGao, the viewport and the display data are left exactly as
    the game set them, so every subsystem that asks the view what
    camera it has -- AI_EvalFunc_VIEWGetObject among them -- gets the
    same answer it would have got.

    Note that the pointer is replaced and not cleared. Clearing it is
    how the engine marks a view as removed, and two callers in
    ai/ai_viewfunctions.c invoke it without testing it first, so a
    script setting the focale or asking whether an object is culled
    would dereference NULL. A view that follows something else is
    still a view that follows.

    The input gate. While the camera has the input, the game sees
    none of it, and that is stopped in two places because the game
    reads it in two.

    INO_SuspendInput clears the joystick mask, which is what the
    engine already does for controls it wants ignored, and gates the
    keyboard poll; that covers everything reading through INO. In the
    platform layer, pcINO_Joystick_Update stops calling the selected
    control mode's handler at all, so the pad the game reads stays as
    its own memset left it. Clearing the pad after letting a handler
    run would not do instead: two of the handlers accumulate, and the
    running total would arrive in one lump on release.

    The memory card manager is outside both, so saving and loading
    carry on while the camera is up.

    The control mode and submode are neither read nor written. Whatever
    the game selected stays selected and stays visible to everything
    that reads it, and the camera's own controls do not change when it
    changes -- they are the keys and the mouse as the platform layer
    read them, below both gates, which is why suppressing the game does
    not suppress the camera.

    The controls are W A S D to move, Q and E for down and up, and the
    mouse, which looks on its own and moves the camera along the same
    two axes as W A S D while the right button is held.

    Movement is a matrix and a clock, so there is nothing to turn off
    to fly: no collision is consulted, no dynamics run, and no sector
    or portal is tested, because none of them are on the path. The
    rest of the game keeps running while the camera moves.
*/

/* Non-zero while the camera is up, and read by the after-effects
   pass, which skips itself rather than drawing the game's effects
   over a camera the game is not looking through.

   This is a second flag beside the ae_no tweak and not the tweak
   itself: the tweak has to go on holding whatever it was set to, and
   the game has to go on being told the after-effect state it
   actually chose. Only ECAM_Toggle and the stand-down path write
   this. */
extern int ECAM_gi_NoAfterEffects;

/* The toggle. Does nothing where there is no world or no view to
   take, which is the state during a load. */
void ECAM_Toggle(void);
int  ECAM_b_Active(void);

/* Takes the game camera's position, sight and banking, once. Does
   nothing when the camera is off, or when the view it took has no
   object assigned to it. */
void ECAM_SyncToGameCamera(void);

/* Once a frame, before the views are updated: reads the device and
   moves the camera. Cheap and safe to call when inactive. */
void ECAM_Update(void);

/* Gives the view back to the game, if it has it. Called when a world
   goes away, so the camera cannot outlive the view it took. */
void ECAM_Release(void);
