#pragma once

/* The debug keys: the function keys the engine answers while it runs.
 *
 * A screenshot, a world reinit, the compiled-AI switch, the raster
 * display, the free camera, two draw-mask toggles and the profiler's
 * page. None of it is a renderer's business, and it sat in the
 * Direct3D renderer's file -- which is why the screenshot key did
 * nothing under the other renderer until it was made to go through
 * the interface, and why the engine could not be built without
 * Direct3D at all.
 *
 * The keys are named here in the codes the engine's own tables use,
 * the ones PLT_ReadDevices fills a block with, rather than the
 * system's virtual keys. */

#define DBK_F1   0x3B
#define DBK_F2   0x3C
#define DBK_F3   0x3D
#define DBK_F4   0x3E
#define DBK_F5   0x3F
#define DBK_F6   0x40
#define DBK_F7   0x41
#define DBK_F8   0x42
#define DBK_F9   0x43
#define DBK_F10  0x44

/* Called once a frame while the game runs. Each key acts once per
   press: the handler waits for it to come back up. */
void PC_EngineCheatFinal(void);
