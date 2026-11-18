/* What the frozen copy of win/wininputoutput.c still needs and the
   rewrite no longer has.

   The keyboard and the mouse were two DirectInput devices held
   exclusively, and the file kept a pointer to each. They are the
   platform layer's now, so the two pointers left the engine header
   -- but the frozen copy is not edited to follow a rewrite, so they
   live here instead and the runner puts this header in front of it.

   -fcommon, as everywhere else in this harness: these are the same
   tentative definitions the engine header used to carry, so the
   frozen copy, the mocks and the driver all land on one object. */
#pragma once

#include "stub/dinput.h"

IDirectInputDevice8A* pcINO_mouseDevice;
IDirectInputDevice8A* pcINO_keybDevice;
