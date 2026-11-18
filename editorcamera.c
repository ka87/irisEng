#include "editorcamera.h"

#include "gameobject.h"
#include "inputoutput.h"
#include "math.h"
#include "time.h"
#include "world.h"
#include "platform/inputbackend.h"

#include <string.h>

/* How fast, in units a second, and how fast with the modifier held. */
#define ECAM_SPEED       8.0f
#define ECAM_FASTFACTOR  5.0f
#define ECAM_SLOWFACTOR  0.2f

/* Units of travel per unit of mouse movement, for panning with the
   right button held. Like looking, this is not scaled by the frame
   time. */
#define ECAM_PANSCALE    0.04f

/* Radians per unit of mouse travel. The mouse reports counts rather
   than a rate, so this is not scaled by the frame time: a given sweep
   of the hand turns the camera by the same amount however fast the
   frame rate is. */
#define ECAM_LOOKSCALE   0.0035f

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

/* The mouse buttons live above the keys in the same platform array;
   the right one is the look button. */
#define ECAM_MOUSE_RIGHT 259

/* Set only here, and read by the after-effects pass so it can be
   skipped while the camera is up. It is a second flag rather than the
   tweak's own ae_no, which has to go on holding whatever the tweak
   was set to. */
int ECAM_gi_NoAfterEffects;

static int          ecam_active;
static tViewElem*   ecam_view;
static tWorld*      ecam_world;
static void (*ecam_savedFollow)(tViewElem* view);
static Mtx4x4       ecam_mtx;
static float        ecam_pitch;        /* kept, so the limit can be held */
static float        ecam_focale;       /* the field of view we impose */
static float        ecam_gameFocale;   /* the last one the game asked for */

/* The view's matrix is not laid out the way the camera device's is.
   CAM_SetObjectMatrixFromCam fills the device's rows from the view's
   as (v1, v2, v3) = (-view.v1, -view.v3, -view.v2), and the device's
   v3 is the sight. So all three of the camera's axes are the view
   rows negated: forward is -v2, right is -v1 and up is -v3. All three
   signs are read out of that mapping; none of them is a convention
   picked here. */
static void ECAM_l_Axes(Vec3* forward, Vec3* lateral, Vec3* vertical)
{
    MATH_VecInvert(forward, &ecam_mtx.v2);
    MATH_VecInvert(lateral, &ecam_mtx.v1);
    MATH_VecInvert(vertical, &ecam_mtx.v3);
}

/* The follow function the view gets while the camera has it, and the
   one place that decides what the view holds. The engine calls it
   once a frame from the view loop, before WOR_SetCam hands the view
   to the device, and ai/ai_viewfunctions.c calls it again when a
   script sets the focale or tests culling. All of them get the same
   answer, so none of them can pull the camera back.

   Position and sight reach the view only through
   WOR_View_SetViewPoint, and its only other caller is the follow
   function this one displaced, so imposing the matrix here is the
   whole of it.

   The focale -- the field of view -- is different: it is a plain
   field that the game writes directly, and
   AI_EvalFunc_VIEWFocaleSet_C writes it and then calls this very
   function. So whatever it asked for is noted, to be given back when
   the camera stands down, and ours is put in its place. */
static void ECAM_l_Follow(tViewElem* view)
{
    if (view->focale != ecam_focale)
        ecam_gameFocale = view->focale;
    view->focale = ecam_focale;

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

/* Everything about standing down except the view, which the two
   callers disagree about: one gives it back, the other must not touch
   it. Shared so that neither can forget a part of it -- the
   after-effects flag above being the easiest to leave set. */
static void ECAM_l_Forget(void)
{
    ecam_view = NULL;
    ecam_world = NULL;
    ecam_savedFollow = NULL;
    ecam_active = 0;
    ECAM_gi_NoAfterEffects = 0;
    INO_SuspendInput(0);
}

void ECAM_Release(void)
{
    if (!ecam_active)
        return;
    /* The view may already have been closed under us, in which case
       its follow pointer is no longer ours and must not be written. */
    if (ecam_view && ecam_view->viewFunctionPtr == ECAM_l_Follow)
    {
        ecam_view->viewFunctionPtr = ecam_savedFollow;
        /* the field of view the game last asked for and was not
           given while the camera held the view */
        ecam_view->focale = ecam_gameFocale;
    }
    ECAM_l_Forget();
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
    ecam_focale = view->focale;
    ecam_gameFocale = view->focale;

    ecam_view = view;
    ecam_world = WOR_gpst_CurrentWorld;
    ecam_savedFollow = view->viewFunctionPtr;
    view->viewFunctionPtr = ECAM_l_Follow;
    ecam_active = 1;
    ECAM_gi_NoAfterEffects = 1;

    INO_SuspendInput(1);
}

/* Takes the game camera's position and orientation once, so the free
   camera carries on from where the game is looking -- its sight and
   its banking included, which is why the whole matrix is adopted
   rather than an angle pair read out of it.
   
   The matrix comes from the assigned object, which is the same place
   WOR_View_FollowFather reads it from, so what is adopted is exactly
   what the game's camera would be this frame. Calling the saved
   follow function instead would also work and would be shorter, but
   it writes the view and its previous viewpoint on the way past, and
   this has to be a read. */
void ECAM_SyncToGameCamera(void)
{
    float sine;

    if (!ecam_active || !ecam_view || !ecam_view->assignedGao)
        return;

    OBJ_MakeAbsoluteMatrix(ecam_view->assignedGao, &ecam_mtx);

    /* The look limit is measured from the world's horizon, so the
       pitch is read back out of the matrix just adopted rather than
       reset to zero. Reset, a sync to an already-pitched camera could
       then be pitched a further whole limit on top of it.
       
       Forward is -v2 and the up axis is the world's Z, so the sine of
       the pitch is v2.z. */
    sine = ecam_mtx.v2.z;
    if (sine > 1.0f)
        sine = 1.0f;
    if (sine < -1.0f)
        sine = -1.0f;
    ecam_pitch = (float)asin((double)sine);

    ecam_mtx.mtxflag |= 0x2;
}

void ECAM_Update(void)
{
    Vec3 forward;
    Vec3 lateral;
    Vec3 vertical;
    float dt;
    float speed;
    float move;
    float strafe;
    float lift;
    float yaw;
    float pitch;
    float panX;
    float panY;
    float modifier;

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
        ECAM_l_Forget();
        return;
    }

    dt = TIM_gf_dt;
    if (dt <= 0.0f)
        return;

    /* The device as the platform layer read it, below the gate that
       keeps it away from the game: the keys and the mouse, and not a
       control mode's reading of either. Nothing here asks what mode
       the game has selected, so nothing here changes when it
       changes. */
    move = ECAM_l_Pair(ECAM_KEY_S, ECAM_KEY_W);
    strafe = ECAM_l_Pair(ECAM_KEY_A, ECAM_KEY_D);
    lift = ECAM_l_Pair(ECAM_KEY_Q, ECAM_KEY_E);

    /* The mouse does one of two things. On its own it looks; with the
       right button held it moves the camera instead, along the same
       two axes W A S D move it along -- across with the pointer's x
       and forward with the pointer going away from you. */
    yaw = 0.0f;
    pitch = 0.0f;
    panX = 0.0f;
    panY = 0.0f;
    if (ECAM_l_b_KeyDown(ECAM_MOUSE_RIGHT))
    {
        panX = (float)pcINO_mouseDeltaX;
        panY = -(float)pcINO_mouseDeltaY;
    }
    else
    {
        yaw = (float)pcINO_mouseDeltaX;
        pitch = (float)pcINO_mouseDeltaY;
    }

    /* The modifiers scale both ways of moving. A key is a rate, so it
       is scaled by the frame time as well; a mouse movement is a
       displacement and is not. */
    modifier = 1.0f;
    if (ECAM_l_b_KeyDown(ECAM_KEY_LSHIFT))
        modifier *= ECAM_FASTFACTOR;
    if (ECAM_l_b_KeyDown(ECAM_KEY_LCTRL))
        modifier *= ECAM_SLOWFACTOR;
    speed = ECAM_SPEED * modifier;

    /* so the two sources add up into one step along each axis */
    move = move * speed * dt + panY * ECAM_PANSCALE * modifier;
    strafe = strafe * speed * dt + panX * ECAM_PANSCALE * modifier;
    lift = lift * speed * dt;

    /* Turning is around the world's up axis, so the horizon stays
       level however far the camera has been flown; looking up and
       down is around the camera's own lateral axis.

       Both signs follow from math.c rather than from taste. The
       global-Z rotation is counter-clockwise about +Z, so turning
       right -- which is what the mouse going right should do -- is a
       negative angle. The local-X rotation puts sin(angle) in v2.z,
       and the pitch counter below is asin of the same component, so a
       positive angle both looks down and raises the counter; the
       mouse going down is a positive delta, which is the
       non-inverted sense. */
    if (yaw != 0.0f)
        MATH_RotateMatrix_AroundGlobalZAxis(&ecam_mtx, -yaw * ECAM_LOOKSCALE);

    if (pitch != 0.0f)
    {
        float wanted = ecam_pitch + pitch * ECAM_LOOKSCALE;

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
        ecam_mtx.Pos.x += forward.x * move;
        ecam_mtx.Pos.y += forward.y * move;
        ecam_mtx.Pos.z += forward.z * move;
    }
    if (strafe != 0.0f)
    {
        ecam_mtx.Pos.x += lateral.x * strafe;
        ecam_mtx.Pos.y += lateral.y * strafe;
        ecam_mtx.Pos.z += lateral.z * strafe;
    }
    if (lift != 0.0f)
    {
        ecam_mtx.Pos.x += vertical.x * lift;
        ecam_mtx.Pos.y += vertical.y * lift;
        ecam_mtx.Pos.z += vertical.z * lift;
    }

    /* The matrix has been turned and moved, so whatever the engine
       cached about it no longer holds. */
    ecam_mtx.mtxflag |= 0x2;
}
