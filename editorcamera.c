#include "editorcamera.h"

#include "inputoutput.h"
#include "math.h"
#include "time.h"
#include "world.h"
#include "win/wininputoutput.h"

#include <string.h>

/* How fast, in units a second, and how fast with the modifier held. */
#define ECAM_SPEED       8.0f
#define ECAM_FASTFACTOR  5.0f
#define ECAM_SLOWFACTOR  0.2f

/* Radians a second at full stick, and per second of key held. */
#define ECAM_TURNRATE    1.8f

/* As far as the camera may look up or down, just short of straight,
   because a camera pointing exactly along the world's up axis has no
   lateral axis left to turn around. */
#define ECAM_PITCHLIMIT  1.52f      /* a little under pi/2 */

/* DirectInput scancodes. */
#define ECAM_KEY_W       0x11
#define ECAM_KEY_A       0x1E
#define ECAM_KEY_S       0x1F
#define ECAM_KEY_D       0x20
#define ECAM_KEY_Q       0x10
#define ECAM_KEY_E       0x12
#define ECAM_KEY_LSHIFT  0x2A
#define ECAM_KEY_LCTRL   0x1D
#define ECAM_KEY_UP      0xC8
#define ECAM_KEY_DOWN    0xD0
#define ECAM_KEY_LEFT    0xCB
#define ECAM_KEY_RIGHT   0xCD

static int          ecam_active;
static tViewElem*   ecam_view;
static tWorld*      ecam_world;
static void (*ecam_savedFollow)(tViewElem* view);
static Mtx4x4       ecam_mtx;
static float        ecam_pitch;        /* kept, so the limit can be held */

/* The view's matrix is not laid out the way the camera device's is:
   CAM_SetObjectMatrixFromCam puts the device's sight vector at
   objectMtx.v3 and fills it from the view's -v2. So the view matrix's
   forward is -v2, and the other two rows are the lateral and vertical
   axes. */
static void ECAM_l_Axes(Vec3* forward, Vec3* lateral, Vec3* vertical)
{
    MATH_VecInvert(forward, &ecam_mtx.v2);
    *lateral = ecam_mtx.v1;
    *vertical = ecam_mtx.v3;
}

/* The follow function the view gets while the camera has it. The
   engine calls this once a frame, and ai/ai_viewfunctions.c calls it
   again when a script sets the focale or tests culling; all of them
   get the same matrix, so none of them can pull the camera back. */
static void ECAM_l_Follow(tViewElem* view)
{
    WOR_View_SetViewPoint(view, &ecam_mtx, &MATH_gst_NulVector, 1, 0);
}

static int ECAM_l_b_KeyDown(int scancode)
{
    return pcINO_keyboardDataBackup[scancode] != 0;
}

/* -1, 0 or 1 from a pair of keys. */
static float ECAM_l_Pair(int negative, int positive)
{
    float v = 0.0f;

    if (ECAM_l_b_KeyDown(positive))
        v += 1.0f;
    if (ECAM_l_b_KeyDown(negative))
        v -= 1.0f;
    return v;
}

/* The view the camera takes: the first one the engine is drawing,
   which is the one the frame loop renders from. */
static tViewElem* ECAM_l_pst_FindView(tWorld* world)
{
    int i;

    if (!world)
        return NULL;
    for (i = 0; i < world->viewNum; i++)
    {
        if (world->viewPtr[i].displayData && world->viewPtr[i].viewFunctionPtr)
            return &world->viewPtr[i];
    }
    return NULL;
}

int ECAM_b_Active(void)
{
    return ecam_active;
}

void ECAM_Release(void)
{
    if (!ecam_active)
        return;
    /* The view may already have been closed under us, in which case
       its follow pointer is no longer ours and must not be written. */
    if (ecam_view && ecam_view->viewFunctionPtr == ECAM_l_Follow)
        ecam_view->viewFunctionPtr = ecam_savedFollow;
    ecam_view = NULL;
    ecam_world = NULL;
    ecam_savedFollow = NULL;
    ecam_active = 0;
    INO_SuspendInput(0);
}

void ECAM_Toggle(void)
{
    tViewElem* view;

    if (ecam_active)
    {
        ECAM_Release();
        return;
    }

    view = ECAM_l_pst_FindView(WOR_gpst_CurrentWorld);
    if (!view)
        return;        /* nothing to take, as during a load */

    /* Start exactly where the game's camera is, so switching does not
       move the picture. */
    memcpy(&ecam_mtx, &view->cameraMtx, sizeof(ecam_mtx));
    ecam_pitch = 0.0f;

    ecam_view = view;
    ecam_world = WOR_gpst_CurrentWorld;
    ecam_savedFollow = view->viewFunctionPtr;
    view->viewFunctionPtr = ECAM_l_Follow;
    ecam_active = 1;

    INO_SuspendInput(1);
}

void ECAM_Update(void)
{
    Vec3 forward;
    Vec3 lateral;
    Vec3 vertical;
    Vec3 leftStick;
    Vec3 rightStick;
    float dt;
    float speed;
    float move;
    float strafe;
    float lift;
    float yaw;
    float pitch;

    if (!ecam_active)
        return;

    /* The world can be torn down while the camera is up -- a level
       change closes every view and clears its follow pointer -- and
       then the view this is holding is not ours any more. Giving it
       back here would write through a pointer the engine has already
       let go of, so the camera simply stands down. */
    if (ecam_world != WOR_gpst_CurrentWorld
        || !ecam_view
        || ecam_view->viewFunctionPtr != ECAM_l_Follow)
    {
        ecam_view = NULL;
        ecam_world = NULL;
        ecam_savedFollow = NULL;
        ecam_active = 0;
        INO_SuspendInput(0);
        return;
    }

    dt = TIM_gf_dt;
    if (dt <= 0.0f)
        return;

    /* Read below the gate: these are the platform's own, so the
       suppression that keeps the pad away from the game does not
       keep it away from here. */
    pcINO_Joystick_Move(&leftStick, 0);
    pcINO_Joystick_Move(&rightStick, 1);

    move = leftStick.y + ECAM_l_Pair(ECAM_KEY_S, ECAM_KEY_W);
    strafe = leftStick.x + ECAM_l_Pair(ECAM_KEY_A, ECAM_KEY_D);
    lift = ECAM_l_Pair(ECAM_KEY_Q, ECAM_KEY_E);
    yaw = rightStick.x + ECAM_l_Pair(ECAM_KEY_LEFT, ECAM_KEY_RIGHT);
    pitch = rightStick.y + ECAM_l_Pair(ECAM_KEY_DOWN, ECAM_KEY_UP);

    speed = ECAM_SPEED;
    if (ECAM_l_b_KeyDown(ECAM_KEY_LSHIFT))
        speed *= ECAM_FASTFACTOR;
    if (ECAM_l_b_KeyDown(ECAM_KEY_LCTRL))
        speed *= ECAM_SLOWFACTOR;

    /* Turning is around the world's up axis, so the horizon stays
       level however far the camera has been flown; looking up and
       down is around the camera's own lateral axis. */
    if (yaw != 0.0f)
        MATH_RotateMatrix_AroundGlobalZAxis(&ecam_mtx, -yaw * ECAM_TURNRATE * dt);

    if (pitch != 0.0f)
    {
        float wanted = ecam_pitch + pitch * ECAM_TURNRATE * dt;

        if (wanted > ECAM_PITCHLIMIT)
            wanted = ECAM_PITCHLIMIT;
        if (wanted < -ECAM_PITCHLIMIT)
            wanted = -ECAM_PITCHLIMIT;
        if (wanted != ecam_pitch)
        {
            MATH_RotateMatrix_AroundLocalXAxis(&ecam_mtx, wanted - ecam_pitch);
            ecam_pitch = wanted;
        }
    }

    ECAM_l_Axes(&forward, &lateral, &vertical);

    if (move != 0.0f)
    {
        ecam_mtx.Pos.x += forward.x * move * speed * dt;
        ecam_mtx.Pos.y += forward.y * move * speed * dt;
        ecam_mtx.Pos.z += forward.z * move * speed * dt;
    }
    if (strafe != 0.0f)
    {
        ecam_mtx.Pos.x += lateral.x * strafe * speed * dt;
        ecam_mtx.Pos.y += lateral.y * strafe * speed * dt;
        ecam_mtx.Pos.z += lateral.z * strafe * speed * dt;
    }
    if (lift != 0.0f)
    {
        ecam_mtx.Pos.x += vertical.x * lift * speed * dt;
        ecam_mtx.Pos.y += vertical.y * lift * speed * dt;
        ecam_mtx.Pos.z += vertical.z * lift * speed * dt;
    }

    /* The matrix has been turned and moved, so whatever the engine
       cached about it no longer holds. */
    ecam_mtx.mtxflag |= 0x2;
}
