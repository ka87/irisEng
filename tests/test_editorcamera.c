/* A unit test for editorcamera.c.
 *
 * New code with no earlier copy, so this is a direct check rather
 * than a differential one. It includes editorcamera.c so the module's
 * own state -- which view it took, where the camera is -- can be read
 * back after driving it.
 *
 * What matters here is not the flying, which is a matrix and a clock.
 * It is what the module does to the engine around it:
 *
 *   - it swaps a view's follow parameter and puts back the same one
 *   - it changes nothing else about the view, so everything that asks
 *     the view what camera it has still gets the game's answer
 *   - it never writes through a view the world has let go of
 *   - it takes the input and gives it back
 *
 * The rotation helpers are implemented for real below rather than
 * stubbed out, because a stub that did nothing would leave every
 * check about which axis the camera moves along passing on code that
 * moved along the wrong one.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../view.h"
#include "../world.h"

/* ------------------------------------------------- the engine around it */

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

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

static void okNear(float got, float want, float slack, const char* what)
{
    checks++;
    if (got < want - slack || got > want + slack)
    {
        printf("  FAIL %s: got %f want %f +/- %f\n", what,
               (double)got, (double)want, (double)slack);
        fails++;
    }
}

float TIM_gf_dt;
Vec3 MATH_gst_NulVector;

static int ecm_setViewPointCalls;
static Mtx4x4 ecm_lastViewPoint;

void WOR_View_SetViewPoint(tViewElem* view, Mtx4x4* mtx, Vec3* v, int i, int onlyPos)
{
    (void)v; (void)i;
    ecm_setViewPointCalls++;
    memcpy(&ecm_lastViewPoint, mtx, sizeof(ecm_lastViewPoint));
    if (view && !onlyPos)
        memcpy(&view->cameraMtx, mtx, sizeof(view->cameraMtx));
}

void MATH_VecInvert(Vec3* dest, Vec3* src)
{
    dest->x = -src->x;
    dest->y = -src->y;
    dest->z = -src->z;
}

/* Rotation, done properly: each of the matrix's three row vectors is
   turned about the axis. */
static void ecm_rotateAbout(Vec3* v, const Vec3* axis, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    float d = axis->x * v->x + axis->y * v->y + axis->z * v->z;
    Vec3 cross;
    Vec3 out;

    cross.x = axis->y * v->z - axis->z * v->y;
    cross.y = axis->z * v->x - axis->x * v->z;
    cross.z = axis->x * v->y - axis->y * v->x;

    out.x = v->x * c + cross.x * s + axis->x * d * (1.0f - c);
    out.y = v->y * c + cross.y * s + axis->y * d * (1.0f - c);
    out.z = v->z * c + cross.z * s + axis->z * d * (1.0f - c);
    *v = out;
}

void MATH_RotateMatrix_AroundGlobalZAxis(Mtx4x4* mtx, float angle)
{
    Vec3 z;

    z.x = 0.0f;
    z.y = 0.0f;
    z.z = 1.0f;
    ecm_rotateAbout(&mtx->v1, &z, angle);
    ecm_rotateAbout(&mtx->v2, &z, angle);
    ecm_rotateAbout(&mtx->v3, &z, angle);
}

void MATH_RotateMatrix_AroundLocalXAxis(Mtx4x4* mtx, float angle)
{
    Vec3 axis = mtx->v1;

    ecm_rotateAbout(&mtx->v2, &axis, angle);
    ecm_rotateAbout(&mtx->v3, &axis, angle);
}

/* The game camera's object matrix, which the sync reads through the
   same call WOR_View_FollowFather uses. */
static Mtx4x4 ecm_gaoMtx;
static int ecm_absMatrixCalls;

void OBJ_MakeAbsoluteMatrix(tGameObjectMain* gao, Mtx4x4* dest)
{
    (void)gao;
    ecm_absMatrixCalls++;
    memcpy(dest, &ecm_gaoMtx, sizeof(*dest));
}

/* the device, below the gate */
char pcINO_keyboardDataBackup[266];
static Vec3 ecm_stick[2];

void pcINO_Joystick_Move(Vec3* move, int stickID)
{
    *move = ecm_stick[stickID & 1];
}

/* the after-effects pass reads this; the suppression it does with
   it is checked in the after-effects harness */

/* the gate, mocked: the real one is checked in the input harness */
static int ecm_suspendCalls;
static int ecm_suspended;

void INO_SuspendInput(int suspend)
{
    ecm_suspendCalls++;
    ecm_suspended = suspend;
}

int INO_b_InputSuspended(void)
{
    return ecm_suspended;
}

#include "../editorcamera.c"

/* ------------------------------------------------------------- a world */

static tWorld ecm_world;
static tViewElem ecm_views[3];
static struct sGdi_DisplayData ecm_display;
static tGameObjectMain ecm_gao;

static int ecm_gameFollowCalls;

static void ecm_gameFollow(tViewElem* view)
{
    (void)view;
    ecm_gameFollowCalls++;
}

/* editorcamera.c does not use it, so it is defined here rather than
   dragging math.c in */
static void MATH_SetIdentityMatrixLocal(Mtx4x4* mtx)
{
    memset(mtx, 0, sizeof(*mtx));
    mtx->v1.x = 1.0f;
    mtx->v2.y = 1.0f;
    mtx->v3.z = 1.0f;
    mtx->w = 1.0f;
}

static void worldReset(void)
{
    /* The module's own state outlives a case, so every case starts
       from off. Without this a case that left the camera on turned it
       *off* with its first toggle and then measured nothing -- which
       is how the strafe check first came to read as a forward one. */
    ECAM_Release();

    memset(&ecm_world, 0, sizeof(ecm_world));
    memset(ecm_views, 0, sizeof(ecm_views));
    memset(&ecm_gao, 0, sizeof(ecm_gao));
    ecm_world.viewPtr = ecm_views;
    ecm_world.viewNum = 3;

    /* view 0 is not being drawn, so it is not the one to take */
    ecm_views[0].displayData = NULL;
    ecm_views[0].viewFunctionPtr = ecm_gameFollow;

    /* view 1 is the one the engine renders from */
    ecm_views[1].displayData = &ecm_display;
    ecm_views[1].viewFunctionPtr = ecm_gameFollow;
    ecm_views[1].assignedGao = &ecm_gao;
    ecm_views[1].focale = 1.25f;
    ecm_views[1].flags = 0x1;
    MATH_SetIdentityMatrixLocal(&ecm_views[1].cameraMtx);
    ecm_views[1].cameraMtx.Pos.x = 10.0f;
    ecm_views[1].cameraMtx.Pos.y = 20.0f;
    ecm_views[1].cameraMtx.Pos.z = 30.0f;

    ecm_views[2].displayData = &ecm_display;
    ecm_views[2].viewFunctionPtr = ecm_gameFollow;

    WOR_gpst_CurrentWorld = &ecm_world;
    TIM_gf_dt = 1.0f / 60.0f;
    memset(pcINO_keyboardDataBackup, 0, sizeof(pcINO_keyboardDataBackup));
    memset(ecm_stick, 0, sizeof(ecm_stick));
    ecm_suspendCalls = 0;
    ecm_suspended = 0;
    ecm_setViewPointCalls = 0;
    ecm_gameFollowCalls = 0;
    ecm_absMatrixCalls = 0;
    MATH_SetIdentityMatrixLocal(&ecm_gaoMtx);
}

/* ------------------------------------------------------------- the checks */

static void takingAndGivingBack(void)
{
    void (*before)(tViewElem*);

    worldReset();
    okEq(ECAM_b_Active(), 0, "it starts off");

    before = ecm_views[1].viewFunctionPtr;
    ECAM_Toggle();
    okEq(ECAM_b_Active(), 1, "one press turns it on");
    ok(ecm_views[1].viewFunctionPtr != before, "it takes the follow parameter");
    okEq(ecm_suspended, 1, "and the input");
    okEq(ECAM_gi_NoAfterEffects, 1, "and asks for the after-effects to be skipped");

    /* and nothing else about the view moved */
    ok(ecm_views[1].assignedGao == &ecm_gao, "the assigned object is untouched");
    okNear(ecm_views[1].focale, 1.25f, 0.0f, "the focale is untouched");
    okEq(ecm_views[1].flags, 0x1, "the flags are untouched");
    ok(ecm_views[1].displayData == &ecm_display, "the display data is untouched");

    /* the view that is not drawn was not taken */
    ok(ecm_views[0].viewFunctionPtr == ecm_gameFollow,
       "a view with no display data is left alone");
    ok(ecm_views[2].viewFunctionPtr == ecm_gameFollow,
       "only one view is taken");

    ECAM_Toggle();
    okEq(ECAM_b_Active(), 0, "a second press turns it off");
    ok(ecm_views[1].viewFunctionPtr == before, "it gives the same follow back");
    okEq(ecm_suspended, 0, "and the input back");
    okEq(ECAM_gi_NoAfterEffects, 0, "and lets the after-effects run again");

    /* The game's own follow runs again afterwards. Tested for before
       being called: giving the view back by *clearing* the pointer
       rather than restoring it is the mistake this guards, and
       ai/ai_viewfunctions.c would dereference it exactly like this,
       so without the guard the failure is a segfault rather than a
       report. */
    ok(ecm_views[1].viewFunctionPtr != NULL,
       "the view is left with a follow function, not with NULL");
    if (ecm_views[1].viewFunctionPtr)
    {
        ecm_gameFollowCalls = 0;
        ecm_views[1].viewFunctionPtr(&ecm_views[1]);
        okEq(ecm_gameFollowCalls, 1, "the game's camera follows again");
    }
}

static void startsWhereTheGameWas(void)
{
    worldReset();
    ECAM_Toggle();
    ECAM_Update();
    ecm_views[1].viewFunctionPtr(&ecm_views[1]);

    okNear(ecm_lastViewPoint.Pos.x, 10.0f, 0.001f, "it starts at the game camera's x");
    okNear(ecm_lastViewPoint.Pos.y, 20.0f, 0.001f, "and y");
    okNear(ecm_lastViewPoint.Pos.z, 30.0f, 0.001f, "and z");
    ECAM_Toggle();
}

/* The AI calls a view's follow function directly when a script sets
   the focale or asks about culling. Those must not pull the camera
   back to the object, and must not move it either. */
static void repeatedFollowIsStable(void)
{
    Mtx4x4 first;

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    ECAM_Update();

    ecm_views[1].viewFunctionPtr(&ecm_views[1]);
    memcpy(&first, &ecm_lastViewPoint, sizeof(first));
    ecm_views[1].viewFunctionPtr(&ecm_views[1]);
    ecm_views[1].viewFunctionPtr(&ecm_views[1]);
    ok(memcmp(&first, &ecm_lastViewPoint, sizeof(first)) == 0,
       "following twice in a frame does not move the camera");
    okEq(ecm_gameFollowCalls, 0, "and never reaches the game's own follow");
    ECAM_Toggle();
}

static void nothingToTake(void)
{
    worldReset();
    WOR_gpst_CurrentWorld = NULL;
    ECAM_Toggle();
    okEq(ECAM_b_Active(), 0, "with no world it does not turn on");
    okEq(ecm_suspendCalls, 0, "and does not take the input");

    worldReset();
    ecm_views[0].displayData = NULL;
    ecm_views[1].displayData = NULL;
    ecm_views[2].displayData = NULL;
    ECAM_Toggle();
    okEq(ECAM_b_Active(), 0, "with no view being drawn it does not turn on");

    /* and a view whose follow is already cleared is not a view */
    worldReset();
    ecm_views[1].viewFunctionPtr = NULL;
    ecm_views[2].viewFunctionPtr = NULL;
    ECAM_Toggle();
    okEq(ECAM_b_Active(), 0, "a removed view is not taken");
}

/* The world can be torn down with the camera up. It must stand down
   rather than write through a view the world has let go of. */
static void survivesTheWorldGoingAway(void)
{
    tWorld other;

    worldReset();
    ECAM_Toggle();
    okEq(ECAM_b_Active(), 1, "it is on");

    memset(&other, 0, sizeof(other));
    other.viewPtr = NULL;
    other.viewNum = 0;
    WOR_gpst_CurrentWorld = &other;

    ECAM_Update();
    okEq(ECAM_b_Active(), 0, "a new world stands the camera down");
    okEq(ecm_suspended, 0, "and gives the input back");
    okEq(ECAM_gi_NoAfterEffects, 0, "and lets the after-effects run again");
    okEq(ecm_setViewPointCalls, 0, "and writes to no view on the way out");

    /* the view it had keeps whatever the engine put there */
    worldReset();
    ECAM_Toggle();
    ecm_views[1].viewFunctionPtr = NULL;      /* the engine closed it */
    ECAM_Update();
    okEq(ECAM_b_Active(), 0, "a closed view stands the camera down");
    okEq(ECAM_gi_NoAfterEffects, 0, "and that path clears the flag as well");
    ok(ecm_views[1].viewFunctionPtr == NULL,
       "and the closed view is left closed, not written to");

    /* Release is safe at any time */
    ECAM_Release();
    ECAM_Release();
    ok(1, "releasing when off is safe");
}

static void inactiveDoesNothing(void)
{
    worldReset();
    ECAM_Update();
    okEq(ecm_setViewPointCalls, 0, "an inactive camera touches no view");
    okEq(ecm_suspendCalls, 0, "and no input");
}

/* Forward is the view matrix's -v2, which is what the camera device
   takes its sight vector from. */
static void movesAlongItsAxes(void)
{
    worldReset();
    ECAM_Toggle();

    /* an identity matrix, so -v2 is -Y */
    ecm_stick[0].y = 1.0f;          /* forward */
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    okNear(ecam_mtx.Pos.y, 20.0f - ECAM_SPEED * 0.5f, 0.01f,
           "forward is the matrix's -v2");
    okNear(ecam_mtx.Pos.x, 10.0f, 0.001f, "and nothing sideways");
    okNear(ecam_mtx.Pos.z, 30.0f, 0.001f, "and nothing vertical");

    /* strafe is v1 */
    worldReset();
    ECAM_Toggle();
    ecm_stick[0].x = 1.0f;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    okNear(ecam_mtx.Pos.x, 10.0f + ECAM_SPEED * 0.5f, 0.01f,
           "strafe is the matrix's v1");
    okNear(ecam_mtx.Pos.y, 20.0f, 0.001f, "and nothing forward");

    /* lift is v3, on a key rather than a stick */
    worldReset();
    ECAM_Toggle();
    pcINO_keyboardDataBackup[ECAM_KEY_E] = (char)0x80;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    okNear(ecam_mtx.Pos.z, 30.0f + ECAM_SPEED * 0.5f, 0.01f,
           "lift is the matrix's v3");
    ECAM_Toggle();
}

static void keysAndSticksAgree(void)
{
    float byStick;
    float byKey;

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    byStick = ecam_mtx.Pos.y;
    ECAM_Toggle();

    worldReset();
    ECAM_Toggle();
    pcINO_keyboardDataBackup[ECAM_KEY_W] = (char)0x80;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    byKey = ecam_mtx.Pos.y;
    ECAM_Toggle();

    okNear(byKey, byStick, 0.001f, "a key moves as far as a full stick");
}

static void speedModifiers(void)
{
    float plain;
    float fast;
    float slow;

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    plain = 20.0f - ecam_mtx.Pos.y;
    ECAM_Toggle();

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    pcINO_keyboardDataBackup[ECAM_KEY_LSHIFT] = (char)0x80;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    fast = 20.0f - ecam_mtx.Pos.y;
    ECAM_Toggle();

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    pcINO_keyboardDataBackup[ECAM_KEY_LCTRL] = (char)0x80;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    slow = 20.0f - ecam_mtx.Pos.y;
    ECAM_Toggle();

    okNear(fast, plain * ECAM_FASTFACTOR, 0.01f, "the modifier makes it faster");
    okNear(slow, plain * ECAM_SLOWFACTOR, 0.01f, "and the other one slower");
}

static void movesWithTheClock(void)
{
    float shortStep;
    float longStep;

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    TIM_gf_dt = 0.1f;
    ECAM_Update();
    shortStep = 20.0f - ecam_mtx.Pos.y;
    ECAM_Toggle();

    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    TIM_gf_dt = 0.4f;
    ECAM_Update();
    longStep = 20.0f - ecam_mtx.Pos.y;
    ECAM_Toggle();

    okNear(longStep, shortStep * 4.0f, 0.01f, "it moves with the clock");

    /* a frame of no time at all moves nothing, rather than dividing
       by it somewhere */
    worldReset();
    ECAM_Toggle();
    ecm_stick[0].y = 1.0f;
    TIM_gf_dt = 0.0f;
    ECAM_Update();
    okNear(ecam_mtx.Pos.y, 20.0f, 0.0001f, "a frame of no time moves nothing");
    ECAM_Toggle();
}

static void turningAndLooking(void)
{
    int i;
    float startY;

    /* yawing a quarter turn should swing forward from -Y to -X or +X */
    worldReset();
    ECAM_Toggle();
    ecm_stick[1].x = 1.0f;
    TIM_gf_dt = (3.14159265f / 2.0f) / ECAM_TURNRATE;
    ECAM_Update();
    okNear(ecam_mtx.v2.y, 0.0f, 0.01f, "a quarter turn swings the forward axis off Y");
    okNear(fabsf(ecam_mtx.v2.x), 1.0f, 0.01f, "and onto X");
    okNear(ecam_mtx.v1.z, 0.0f, 0.01f, "and the horizon stays level");
    ECAM_Toggle();

    /* looking up is limited, however long it is held */
    worldReset();
    ECAM_Toggle();
    ecm_stick[1].y = 1.0f;
    TIM_gf_dt = 0.5f;
    for (i = 0; i < 100; i++)
        ECAM_Update();
    okNear(ecam_pitch, ECAM_PITCHLIMIT, 0.0001f, "looking up stops at the limit");

    /* and down */
    ecm_stick[1].y = -1.0f;
    for (i = 0; i < 100; i++)
        ECAM_Update();
    okNear(ecam_pitch, -ECAM_PITCHLIMIT, 0.0001f, "looking down stops at the limit");

    /* the limit is on the angle, not on the matrix drifting: the
       forward axis must still be a unit vector after all that */
    {
        float len = sqrtf(ecam_mtx.v2.x * ecam_mtx.v2.x
                        + ecam_mtx.v2.y * ecam_mtx.v2.y
                        + ecam_mtx.v2.z * ecam_mtx.v2.z);
        okNear(len, 1.0f, 0.01f, "the matrix does not drift out of shape");
    }

    /* a camera pointing far up still flies where it points */
    startY = ecam_mtx.Pos.y;
    ecm_stick[1].y = 0.0f;
    ecm_stick[0].y = 1.0f;
    TIM_gf_dt = 0.5f;
    ECAM_Update();
    ok(ecam_mtx.Pos.y != startY || ecam_mtx.Pos.z != 30.0f,
       "a camera that has been turned still flies");
    ECAM_Toggle();
}

/* Pitch the identity about its own X axis, the way a camera looking
   up or down is pitched. */
static void ecm_pitchedMatrix(Mtx4x4* mtx, float angle)
{
    Vec3 axis;

    MATH_SetIdentityMatrixLocal(mtx);
    axis.x = 1.0f;
    axis.y = 0.0f;
    axis.z = 0.0f;
    ecm_rotateAbout(&mtx->v2, &axis, angle);
    ecm_rotateAbout(&mtx->v3, &axis, angle);
}

/* The world pitch of whatever the camera is looking along, which is
   what the limit is really about. Read off the matrix rather than off
   the module's own counter, so a counter that disagrees with the
   matrix cannot hide. */
static float ecm_lookPitch(void)
{
    float sine = ecam_mtx.v2.z;

    if (sine > 1.0f)
        sine = 1.0f;
    if (sine < -1.0f)
        sine = -1.0f;
    return asinf(sine);
}

static void syncToGameCamera(void)
{
    /* position, sight and banking in one go */
    worldReset();
    ECAM_Toggle();

    MATH_SetIdentityMatrixLocal(&ecm_gaoMtx);
    ecm_gaoMtx.Pos.x = -4.0f;
    ecm_gaoMtx.Pos.y = 55.0f;
    ecm_gaoMtx.Pos.z = 7.5f;
    /* a banked camera: rolled about its own forward axis, so the
       lateral and vertical rows tilt and the forward row does not */
    {
        Vec3 roll;
        roll.x = 0.0f;
        roll.y = 1.0f;
        roll.z = 0.0f;
        ecm_rotateAbout(&ecm_gaoMtx.v1, &roll, 0.4f);
        ecm_rotateAbout(&ecm_gaoMtx.v3, &roll, 0.4f);
    }

    ECAM_SyncToGameCamera();
    okEq(ecm_absMatrixCalls, 1, "the sync reads the assigned object once");
    okNear(ecam_mtx.Pos.x, -4.0f, 0.0001f, "it takes the game camera's x");
    okNear(ecam_mtx.Pos.y, 55.0f, 0.0001f, "and y");
    okNear(ecam_mtx.Pos.z, 7.5f, 0.0001f, "and z");
    okNear(ecam_mtx.v2.x, ecm_gaoMtx.v2.x, 0.0001f, "and the sight");
    okNear(ecam_mtx.v2.y, ecm_gaoMtx.v2.y, 0.0001f, "all of it");
    okNear(ecam_mtx.v2.z, ecm_gaoMtx.v2.z, 0.0001f, "all of it");
    okNear(ecam_mtx.v1.z, ecm_gaoMtx.v1.z, 0.0001f, "and the banking");
    okNear(ecam_mtx.v3.z, ecm_gaoMtx.v3.z, 0.0001f, "all of that too");
    ok(ecam_mtx.v1.z != 0.0f, "the banked fixture really is banked");

    /* and the view gets it */
    ecm_views[1].viewFunctionPtr(&ecm_views[1]);
    okNear(ecm_lastViewPoint.Pos.y, 55.0f, 0.0001f, "and the view is given it");

    /* once, and not every frame: the object moving afterwards does
       not drag the camera along with it */
    ecm_gaoMtx.Pos.y = 999.0f;
    TIM_gf_dt = 1.0f / 60.0f;
    ECAM_Update();
    ECAM_Update();
    okNear(ecam_mtx.Pos.y, 55.0f, 0.0001f, "the sync does not keep following");
    okEq(ecm_absMatrixCalls, 1, "and does not read the object again");
    ECAM_Toggle();
}

static void syncWhenThereIsNothingToSyncTo(void)
{
    /* Off: nothing happens, and nothing is read.
    
       Note that the !ecam_active half of the guard in
       ECAM_SyncToGameCamera is not what this proves: releasing the
       camera clears ecam_view as well, so the !ecam_view half catches
       this on its own and removing the other one changes nothing
       observable. The module never reaches a state where one is set
       and the other is not, so the term is defence rather than a live
       requirement, and no test here can see it. */
    worldReset();
    ECAM_SyncToGameCamera();
    okEq(ecm_absMatrixCalls, 0, "a sync with the camera off reads nothing");
    okEq(ECAM_b_Active(), 0, "and does not turn it on");

    /* on, but the view has no object: the camera stays where it is */
    worldReset();
    ECAM_Toggle();
    ecm_views[1].assignedGao = NULL;
    ecm_gaoMtx.Pos.y = 123.0f;
    ECAM_SyncToGameCamera();
    okEq(ecm_absMatrixCalls, 0, "a view with no object is not read");
    okNear(ecam_mtx.Pos.y, 20.0f, 0.0001f, "and the camera does not move");
    ECAM_Toggle();
}

/* The look limit is about the world's horizon, so syncing to a camera
   that is already pitched must not buy a further limit's worth of
   pitch on top of it. This measures the matrix, not the module's
   counter, so it holds whichever way round the engine's rotation
   turns. */
static void theLimitSurvivesASync(void)
{
    int i;

    worldReset();
    ECAM_Toggle();
    ecm_pitchedMatrix(&ecm_gaoMtx, 1.0f);      /* well past half the limit */
    ECAM_SyncToGameCamera();
    okNear(ecm_lookPitch(), 1.0f, 0.01f, "the synced camera is pitched as the game's was");

    ecm_stick[1].y = 1.0f;
    TIM_gf_dt = 0.5f;
    for (i = 0; i < 100; i++)
        ECAM_Update();
    okNear(fabsf(ecm_lookPitch()), ECAM_PITCHLIMIT, 0.01f,
           "looking up from a synced pitch still stops at the limit");

    ecm_stick[1].y = -1.0f;
    for (i = 0; i < 100; i++)
        ECAM_Update();
    okNear(fabsf(ecm_lookPitch()), ECAM_PITCHLIMIT, 0.01f,
           "and so does looking down");
    ECAM_Toggle();

    /* the other way round, from a camera pitched the other way */
    worldReset();
    ECAM_Toggle();
    ecm_pitchedMatrix(&ecm_gaoMtx, -1.0f);
    ECAM_SyncToGameCamera();
    ecm_stick[1].y = 1.0f;
    TIM_gf_dt = 0.5f;
    for (i = 0; i < 100; i++)
        ECAM_Update();
    okNear(fabsf(ecm_lookPitch()), ECAM_PITCHLIMIT, 0.01f,
           "a sync the other way round holds the limit too");
    ECAM_Toggle();
}

int main(void)
{
    printf("=== editor camera unit test ===\n");
    takingAndGivingBack();
    startsWhereTheGameWas();
    repeatedFollowIsStable();
    nothingToTake();
    survivesTheWorldGoingAway();
    inactiveDoesNothing();
    movesAlongItsAxes();
    keysAndSticksAgree();
    speedModifiers();
    movesWithTheClock();
    turningAndLooking();
    syncToGameCamera();
    syncWhenThereIsNothingToSyncTo();
    theLimitSurvivesASync();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
