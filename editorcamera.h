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
    turning it off puts the original back. Nothing else about the view
    changes: assignedGao, the focale, the viewport and the display
    data are left exactly as the game set them, so every subsystem
    that asks the view what camera it has -- AI_EvalFunc_VIEWGetObject
    among them -- gets the same answer it would have got.

    Note that the pointer is replaced and not cleared. Clearing it is
    how the engine marks a view as removed, and two callers in
    ai/ai_viewfunctions.c invoke it without testing it first, so a
    script setting the focale or asking whether an object is culled
    would dereference NULL. A view that follows something else is
    still a view that follows.

    The input gate. While the camera has the input, the game's own
    suppression is used to keep it away from everything that reads a
    pad: INO_SuspendInput clears the joystick mask, which is what the
    engine already does for controls it wants ignored. The memory card
    manager is outside that and keeps running, so saving and loading
    are unaffected. The camera itself reads the device below the gate,
    through the platform layer, which is why suppressing the game does
    not suppress the camera.

    The control mode and submode are not touched at all. Whatever the
    game selected stays selected and stays visible to everything that
    reads it.

    Movement is a matrix and a clock, so there is nothing to turn off
    to fly: no collision is consulted, no dynamics run, and no sector
    or portal is tested, because none of them are on the path. The
    rest of the game keeps running while the camera moves.
*/

/* The toggle. Does nothing where there is no world or no view to
   take, which is the state during a load. */
void ECAM_Toggle(void);
int  ECAM_b_Active(void);

/* Once a frame, before the views are updated: reads the device and
   moves the camera. Cheap and safe to call when inactive. */
void ECAM_Update(void);

/* Gives the view back to the game, if it has it. Called when a world
   goes away, so the camera cannot outlive the view it took. */
void ECAM_Release(void);
