/* A unit test for platform/d3dvideo.c.
 *
 * New code with no earlier copy, so this is a direct check rather
 * than a differential one. The device is mocked and records what it
 * was asked for, in order, so the checks are on the sequence as much
 * as on the result.
 *
 * Two things here are worth more than the rest:
 *
 *   - the picture reports its own height. The engine clamps a decoded
 *     frame to what it is told, and it used to be told the screen's
 *     height while writing into a movie-sized picture -- so a window
 *     shorter than the movie left the bottom of every frame unwritten.
 *     The case for it makes the two sizes differ on purpose.
 *
 *   - every path that fails part-way gives back what it had already
 *     made. The cases count creations against releases rather than
 *     trusting the code to look tidy.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../platform/d3dvideo.h"
#include "../gdi.h"

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

/* ------------------------------------------------------------- the device */

#define LOGMAX 64

static char vlog[LOGMAX][128];
static int  vlogLen;

static void Log(const char* fmt, ...)
{
    va_list ap;

    if (vlogLen >= LOGMAX)
        return;
    va_start(ap, fmt);
    vsnprintf(vlog[vlogLen], sizeof(vlog[0]), fmt, ap);
    va_end(ap);
    vlogLen++;
}

static int LoggedAt(int i, const char* needle)
{
    return i < vlogLen && strstr(vlog[i], needle) != NULL;
}

static int Logged(const char* needle)
{
    int i;

    for (i = 0; i < vlogLen; i++)
        if (strstr(vlog[i], needle))
            return 1;
    return 0;
}

/* The expectations below are built from the SDK's own names rather
   than from numbers. tests/stub/d3d9.h gives the D3D constants
   deliberately arbitrary values, so a check written with a number
   copied out of a run would pass whatever constant the code actually
   used -- which is the one thing worth catching here. */
static int LoggedFmt(const char* fmt, ...)
{
    char want[160];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(want, sizeof(want), fmt, ap);
    va_end(ap);
    return Logged(want);
}

static void dumpLog(const char* title)
{
    int i;

    printf("    [%s]\n", title);
    for (i = 0; i < vlogLen; i++)
        printf("      %s\n", vlog[i]);
}

static int LoggedCount(const char* needle)
{
    int i;
    int n = 0;

    for (i = 0; i < vlogLen; i++)
        if (strstr(vlog[i], needle))
            n++;
    return n;
}

/* What the mock should refuse, so every failure path can be driven. */
static struct {
    int failBackBuffer;
    int failPlainSurface;
    int failTexture;
    int failSurfaceLevel;
    int failLock;
    int screenW;
    int screenH;
    D3DFORMAT screenFormat;
} fake;

static int liveObjects;

static IDirect3DDevice9     dev;
static IDirect3DDevice9Vtbl devVtbl;

static IDirect3DSurface9     backSurf, picSurf, stageSurf;
static IDirect3DSurface9Vtbl surfVtbl;
static IDirect3DTexture9     stageTex;
static IDirect3DTexture9Vtbl texVtbl;

/* The picture's memory, with a pitch that matches the size the
   picture was asked for -- a mock that reported a pitch its own
   buffer could not back would be the one overrunning, not the code. */
static uint8_t pictureBytes[4 * 1024 * 1024];
static int     pictureLocks;
static int     picturePitch;

static const char* Name(void* p)
{
    if (p == &backSurf)  return "back";
    if (p == &picSurf)   return "picture";
    if (p == &stageSurf) return "stage";
    if (p == &stageTex)  return "stagetex";
    return "?";
}

static ULONG Vt_SurfRelease(IDirect3DSurface9* s)
{
    Log("Release(%s)", Name(s));
    liveObjects--;
    return 0;
}

static ULONG Vt_TexRelease(IDirect3DTexture9* t)
{
    Log("Release(%s)", Name(t));
    liveObjects--;
    return 0;
}

static HRESULT Vt_GetDesc(IDirect3DSurface9* s, D3DSURFACE_DESC* d)
{
    Log("GetDesc(%s)", Name(s));
    memset(d, 0, sizeof(*d));
    d->Width = fake.screenW;
    d->Height = fake.screenH;
    d->Format = fake.screenFormat;
    return 0;
}

static HRESULT Vt_LockRect(IDirect3DSurface9* s, D3DLOCKED_RECT* lr,
                           const RECT* r, DWORD flags)
{
    (void)r;
    Log("LockRect(%s,%lu)", Name(s), (unsigned long)flags);
    if (fake.failLock)
        return -1;
    pictureLocks++;
    lr->pBits = pictureBytes;
    lr->Pitch = picturePitch;
    return 0;
}

static HRESULT Vt_UnlockRect(IDirect3DSurface9* s)
{
    Log("UnlockRect(%s)", Name(s));
    pictureLocks--;
    return 0;
}

static HRESULT Vt_GetBackBuffer(IDirect3DDevice9* d, UINT a, UINT b,
                                D3DBACKBUFFER_TYPE t, IDirect3DSurface9** out)
{
    (void)d; (void)a; (void)b; (void)t;
    Log("GetBackBuffer");
    if (fake.failBackBuffer)
        return -1;
    *out = &backSurf;
    liveObjects++;
    return 0;
}

static HRESULT Vt_CreateOffscreenPlainSurface(IDirect3DDevice9* d, UINT w, UINT h,
                                              D3DFORMAT f, D3DPOOL pool,
                                              IDirect3DSurface9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    Log("CreateOffscreenPlainSurface(%u,%u,%d,%d)", w, h, (int)f, (int)pool);
    if (fake.failPlainSurface)
        return -1;
    picturePitch = (int)w * 4;
    if ((size_t)picturePitch * h > sizeof(pictureBytes))
    {
        printf("  FAIL the mock's buffer is too small for %ux%u\n", w, h);
        fails++;
        return -1;
    }
    *out = &picSurf;
    liveObjects++;
    return 0;
}

static HRESULT Vt_CreateTexture(IDirect3DDevice9* d, UINT w, UINT h, UINT levels,
                                DWORD usage, D3DFORMAT f, D3DPOOL pool,
                                IDirect3DTexture9** out, HANDLE* sh)
{
    (void)d; (void)sh;
    Log("CreateTexture(%u,%u,%u,%lu,%d,%d)", w, h, levels,
        (unsigned long)usage, (int)f, (int)pool);
    if (fake.failTexture)
        return -1;
    *out = &stageTex;
    liveObjects++;
    return 0;
}

static HRESULT Vt_GetSurfaceLevel(IDirect3DTexture9* t, UINT level,
                                  IDirect3DSurface9** out)
{
    (void)t;
    Log("GetSurfaceLevel(%u)", level);
    if (fake.failSurfaceLevel)
        return -1;
    *out = &stageSurf;
    liveObjects++;
    return 0;
}

static HRESULT Vt_UpdateSurface(IDirect3DDevice9* d, IDirect3DSurface9* src,
                                const RECT* sr, IDirect3DSurface9* dst,
                                const POINT* dp)
{
    (void)d; (void)sr; (void)dp;
    Log("UpdateSurface(%s->%s)", Name(src), Name(dst));
    return 0;
}

static HRESULT Vt_Clear(IDirect3DDevice9* d, DWORD count, const D3DRECT* rects,
                        DWORD flags, D3DCOLOR colour, float z, DWORD stencil)
{
    (void)d; (void)count; (void)rects; (void)z; (void)stencil;
    Log("Clear(%lu,%lu)", (unsigned long)flags, (unsigned long)colour);
    return 0;
}

static HRESULT Vt_StretchRect(IDirect3DDevice9* d, IDirect3DSurface9* src,
                              const RECT* sr, IDirect3DSurface9* dst,
                              const RECT* dr, D3DTEXTUREFILTERTYPE filter)
{
    (void)d;
    Log("StretchRect(%s->%s,src=%ld,%ld,%ld,%ld,dst=%ld,%ld,%ld,%ld,f=%d)",
        Name(src), Name(dst),
        sr->left, sr->top, sr->right, sr->bottom,
        dr->left, dr->top, dr->right, dr->bottom, (int)filter);
    return 0;
}

static void Reset(int withDevice)
{
    vlogLen = 0;
    liveObjects = 0;
    pictureLocks = 0;
    picturePitch = 0;

    memset(&fake, 0, sizeof(fake));
    fake.screenW = 1024;
    fake.screenH = 768;
    fake.screenFormat = (D3DFORMAT)21;

    surfVtbl.Release = Vt_SurfRelease;
    surfVtbl.GetDesc = Vt_GetDesc;
    surfVtbl.LockRect = Vt_LockRect;
    surfVtbl.UnlockRect = Vt_UnlockRect;
    backSurf.lpVtbl = &surfVtbl;
    picSurf.lpVtbl = &surfVtbl;
    stageSurf.lpVtbl = &surfVtbl;

    texVtbl.Release = Vt_TexRelease;
    texVtbl.GetSurfaceLevel = Vt_GetSurfaceLevel;
    stageTex.lpVtbl = &texVtbl;

    devVtbl.GetBackBuffer = Vt_GetBackBuffer;
    devVtbl.CreateOffscreenPlainSurface = Vt_CreateOffscreenPlainSurface;
    devVtbl.CreateTexture = Vt_CreateTexture;
    devVtbl.UpdateSurface = Vt_UpdateSurface;
    devVtbl.Clear = Vt_Clear;
    devVtbl.StretchRect = Vt_StretchRect;
    dev.lpVtbl = &devVtbl;

    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    D3DRenderParams.d3dDev = withDevice ? &dev : NULL;
}

/* ------------------------------------------------------------- the checks */

static void makingTheDisplay(void)
{
    Reset(1);
    ok(D3D_b_VideoDisplayCreate(320, 240) == 1, "a display is made");

    /* the back buffer decides the format and the fit, so it is asked
       first and given back rather than held */
    ok(LoggedAt(0, "GetBackBuffer"), "the back buffer is asked for first");
    ok(LoggedAt(1, "GetDesc(back)"), "and described");
    ok(Logged("Release(back)"), "and released rather than held");

    ok(LoggedFmt("CreateOffscreenPlainSurface(320,240,%d,%d)",
                 (int)fake.screenFormat, (int)D3DPOOL_SYSTEMMEM),
       "the picture is the movie's size, the screen's format, in system memory");
    ok(LoggedFmt("CreateTexture(320,240,1,%lu,%d,%d)",
                 (unsigned long)D3DUSAGE_RENDERTARGET, (int)fake.screenFormat,
                 (int)D3DPOOL_DEFAULT),
       "and the stage is a render-target texture of the same size");
    ok(Logged("GetSurfaceLevel(0)"), "whose top surface is taken");

    /* black to begin with, so a frame that does not cover the picture
       shows black and not whatever the memory held */
    ok(Logged("LockRect(picture"), "the picture is cleared");
    ok(Logged("UnlockRect(picture)"), "and unlocked again");
    okEq(pictureLocks, 0, "leaving no lock open");

    D3D_VideoDisplayClose();
    okEq(liveObjects, 0, "and closing gives everything back");
}

static void refusingToMakeOne(void)
{
    Reset(0);
    vlogLen = 0;
    okEq(D3D_b_VideoDisplayCreate(320, 240), 0, "no device makes no display");
    okEq(vlogLen, 0, "and asks the device nothing");

    Reset(1);
    okEq(D3D_b_VideoDisplayCreate(0, 240), 0, "nor does a zero width");
    okEq(D3D_b_VideoDisplayCreate(320, -4), 0, "nor a negative height");
    okEq(vlogLen, 0, "neither of which reaches the device");
}

/* Every part-way failure has to give back what it had already made,
   which is counted rather than eyeballed. */
static void failingPartWay(void)
{
    Reset(1);
    fake.failBackBuffer = 1;
    okEq(D3D_b_VideoDisplayCreate(320, 240), 0, "no back buffer, no display");
    okEq(liveObjects, 0, "and nothing left over");

    Reset(1);
    fake.failPlainSurface = 1;
    okEq(D3D_b_VideoDisplayCreate(320, 240), 0, "no picture, no display");
    okEq(liveObjects, 0, "and the back buffer was still given back");

    Reset(1);
    fake.failTexture = 1;
    okEq(D3D_b_VideoDisplayCreate(320, 240), 0, "no stage texture, no display");
    okEq(liveObjects, 0, "and the picture was given back");
    ok(Logged("Release(picture)"), "by name");

    Reset(1);
    fake.failSurfaceLevel = 1;
    okEq(D3D_b_VideoDisplayCreate(320, 240), 0, "no stage surface, no display");
    okEq(liveObjects, 0, "and the texture and picture were both given back");
    ok(Logged("Release(stagetex)"), "the texture");
    ok(Logged("Release(picture)"), "and the picture");

    /* and after a failure there is nothing to lock or present */
    okEq(D3D_b_VideoFrameLock(NULL, NULL, NULL), 0,
         "a failed display has no frame to lock");
}

static void makingTwoInARow(void)
{
    Reset(1);
    D3D_b_VideoDisplayCreate(320, 240);
    okEq(liveObjects, 3, "one display holds three things");

    vlogLen = 0;
    ok(D3D_b_VideoDisplayCreate(640, 480) == 1, "a second display is made");
    okEq(liveObjects, 3, "and the first one was closed, not leaked");
    ok(Logged("CreateOffscreenPlainSurface(640,480"), "at the new size");

    D3D_VideoDisplayClose();
    okEq(liveObjects, 0, "all given back");
}

/* The one that matters. The engine clamps a decoded frame to the
   height it is handed, writing into a picture that is the movie's
   size -- so the height handed over has to be the picture's. It used
   to be the screen's, which only looked right while the window was
   taller than the movie. */
static void thePictureReportsItsOwnHeight(void)
{
    void* bits;
    int pitch;
    int height;

    Reset(1);
    fake.screenW = 640;
    fake.screenH = 480;            /* deliberately not the movie's */
    D3D_b_VideoDisplayCreate(320, 720);

    bits = NULL;
    pitch = 0;
    height = 0;
    ok(D3D_b_VideoFrameLock(&bits, &pitch, &height) == 1, "a frame locks");
    ok(bits != NULL, "with somewhere to write");
    okEq(pitch, 320 * 4, "and the picture's pitch");
    okEq(height, 720, "and the picture's own height, not the screen's");
    ok(height != fake.screenH, "which differ here, so that check has teeth");

    D3D_VideoFrameUnlock();
    okEq(pictureLocks, 0, "and unlocks");

    D3D_VideoDisplayClose();
}

static void lockingWhenThereIsNothingTo(void)
{
    void* bits = (void*)1;
    int pitch = 1;
    int height = 1;

    Reset(1);
    vlogLen = 0;
    okEq(D3D_b_VideoFrameLock(&bits, &pitch, &height), 0,
         "no display, no frame");
    okEq(vlogLen, 0, "and nothing asked of the device");

    /* unlocking without a display must not reach the device either */
    D3D_VideoFrameUnlock();
    okEq(vlogLen, 0, "and nor does unlocking");

    /* a device that refuses the lock */
    Reset(1);
    D3D_b_VideoDisplayCreate(320, 240);
    fake.failLock = 1;
    vlogLen = 0;
    okEq(D3D_b_VideoFrameLock(&bits, &pitch, &height), 0,
         "a refused lock is no frame");
    ok(!Logged("UnlockRect"), "and is not unlocked");
    D3D_VideoDisplayClose();
}

static void presentingAFrame(void)
{
    Reset(1);
    fake.screenW = 1000;
    fake.screenH = 600;
    D3D_b_VideoDisplayCreate(500, 300);

    /* actual size: no scaling, no filter, centred */
    vlogLen = 0;
    D3D_VideoFramePresent(500, 300, GDI_VIDEOFIT_ACTUALSIZE);
    ok(Logged("UpdateSurface(picture->stage)"),
       "the picture is copied to the stage");
    ok(LoggedFmt("Clear(%lu,0)", (unsigned long)D3DCLEAR_TARGET),
       "the screen is cleared to black");
    ok(LoggedFmt("StretchRect(stage->back,src=0,0,500,300,"
                 "dst=250,150,750,450,f=%d)", (int)D3DTEXF_NONE),
       "and blitted centred, unscaled and unfiltered");
    ok(Logged("Release(back)"), "and the back buffer given back");
    okEq(liveObjects, 3, "leaving the display's own three");

    /* the clear comes before the blit, or the frame is cleared away */
    {
        int clearAt = -1;
        int blitAt = -1;
        int i;

        for (i = 0; i < vlogLen; i++)
        {
            if (clearAt < 0 && strstr(vlog[i], "Clear("))
                clearAt = i;
            if (blitAt < 0 && strstr(vlog[i], "StretchRect("))
                blitAt = i;
        }
        ok(clearAt >= 0 && blitAt >= 0 && clearAt < blitAt,
           "the clear comes before the blit");
    }

    /* sharp: scaled to the screen's width, unsmoothed */
    vlogLen = 0;
    D3D_VideoFramePresent(500, 300, GDI_VIDEOFIT_SHARP);
    ok(LoggedFmt("dst=0,0,1000,600,f=%d", (int)D3DTEXF_POINT),
       "sharp scales to the screen, point filter");

    /* smooth: the same fit, smoothed */
    vlogLen = 0;
    D3D_VideoFramePresent(500, 300, GDI_VIDEOFIT_SMOOTH);
    ok(LoggedFmt("dst=0,0,1000,600,f=%d", (int)D3DTEXF_LINEAR),
       "smooth scales the same way, linear filter");

    /* an unknown fit is the safe one rather than undefined */
    vlogLen = 0;
    D3D_VideoFramePresent(500, 300, 99);
    ok(LoggedFmt("f=%d", (int)D3DTEXF_NONE),
       "an unknown fit is treated as actual size");

    D3D_VideoDisplayClose();
}

static void presentingWhenThereIsNothingTo(void)
{
    Reset(1);
    vlogLen = 0;
    D3D_VideoFramePresent(320, 240, GDI_VIDEOFIT_SMOOTH);
    okEq(vlogLen, 0, "no display, nothing presented");

    Reset(1);
    D3D_b_VideoDisplayCreate(320, 240);
    vlogLen = 0;
    D3D_VideoFramePresent(0, 240, GDI_VIDEOFIT_SMOOTH);
    okEq(vlogLen, 0, "a zero-width frame is not presented");
    D3D_VideoFramePresent(320, 0, GDI_VIDEOFIT_SMOOTH);
    okEq(vlogLen, 0, "nor a zero-height one");

    /* and a back buffer that cannot be had is not a crash */
    fake.failBackBuffer = 1;
    vlogLen = 0;
    D3D_VideoFramePresent(320, 240, GDI_VIDEOFIT_SMOOTH);
    ok(!Logged("StretchRect"), "no back buffer, no blit");
    okEq(liveObjects, 3, "and nothing released that was not taken");

    D3D_VideoDisplayClose();
}

static void closingTwice(void)
{
    Reset(1);
    D3D_b_VideoDisplayCreate(320, 240);
    D3D_VideoDisplayClose();
    okEq(liveObjects, 0, "closed");

    vlogLen = 0;
    D3D_VideoDisplayClose();
    okEq(vlogLen, 0, "closing again releases nothing a second time");
    okEq(LoggedCount("Release"), 0, "nothing at all");
}

int main(void)
{
    printf("=== fullscreen video display unit test ===\n");
    makingTheDisplay();
    refusingToMakeOne();
    failingPartWay();
    makingTwoInARow();
    thePictureReportsItsOwnHeight();
    lockingWhenThereIsNothingTo();
    presentingAFrame();
    presentingWhenThereIsNothingTo();
    closingTwice();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    if (fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
