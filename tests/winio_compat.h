/* What the frozen copy of platform/wininputoutput.c still needs and the
   rewrite no longer has.

   The keyboard and the mouse were two DirectInput devices held
   exclusively, and the file kept a pointer to each as well as to the
   DirectInput object that made them. They are the platform layer's
   now and nothing in the engine opens DirectInput at all -- but the
   frozen copy is not edited to follow a rewrite, so the three
   pointers live here instead and the runner puts this header in
   front of it.

   -fcommon, as everywhere else in this harness: these are the same
   tentative definitions the engine header used to carry, so the
   frozen copy, the mocks and the driver all land on one object. */
#pragma once

/* The version macro went with the include: the engine header defined
   it so that <dinput.h> would give the 8 interfaces, and nothing in
   the engine asks for them any more. */
#define DIRECTINPUT_VERSION  0x0800

#include "stub/dinput.h"

IDirectInputDevice8A* pcINO_mouseDevice;
IDirectInputDevice8A* pcINO_keybDevice;

/* And the object that made them. The only thing the engine ever used
   it for was creating those two devices, so it went when they did;
   the frozen copy still opens it and releases it. */
IDirectInput8A* dinput_if;
