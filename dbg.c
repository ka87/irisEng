#include "dbg.h"
#include "version.h"
/* For the log's timestamp. Asked for by name rather than left to
   arrive through windows.h: the engine has a time.h of its own, and
   that one is the engine's clock, not the C library's. The angle
   brackets are what separates them -- the project's directory is not
   on the compiler's include path, only thirdparty's, so this is the C
   library's header and the engine's is "time.h". */
#include <time.h>
//#include "animation.h" //tmp

int tweak_ae_no;
int tweak_ae_view;
int tweak_d3d_chrome = 1;
int tweak_gl_cull;
int tweak_gl_water_flip;
int tweak_ae_nobands = 0;
float tweak_mipmaplodbias = -0.60000002f;
int tweak_perf;
int tweak_perf_fps;
float tweak_perf_fps_min = 20.0f;
float tweak_perf_fps_target = 60.0f;
int tweak_water_blur_normal = 1;
int tweak_water_clip = 1;
float tweak_water_off;
int tweak_water_smooth = 1;
float tweak_water_smoothx;
float tweak_water_smoothy;
float tweak_water_min_bv;

/* Three of the tweaks are not variables of this module's: they are
   capability flags the renderer works out from the hardware when it
   starts, and the console exists so they can be forced. So these go
   through the drawing interface like everything else that belongs to
   a renderer, which is what keeps this file from naming one.

   They are the table's type 2, the setter kind, which the console
   already knew how to call and nothing had used. A renderer that has
   not been chosen yet simply swallows the write. */
static void DBG_l_TweakCanDiffuse2x(float value)
{
    if (gpst_DX_stDD && gpst_DX_stDD->SetCanDiffuse2x)
        gpst_DX_stDD->SetCanDiffuse2x((int)value);
}

static void DBG_l_TweakWantsNoFlare(float value)
{
    if (gpst_DX_stDD && gpst_DX_stDD->SetWantsNoFlare)
        gpst_DX_stDD->SetWantsNoFlare((int)value);
}

static void DBG_l_TweakNoZonAlpha(float value)
{
    if (gpst_DX_stDD && gpst_DX_stDD->SetNoZonAlpha)
        gpst_DX_stDD->SetNoZonAlpha((int)value);
}

/* Sorted by name: the console finds an entry with bsearch. */
/* No size: the count below is taken from the initialiser, so the
   two cannot drift apart. */
struct sTweakElem tweaks[] = {
    {"ae_d2x", 2, (void*)DBG_l_TweakCanDiffuse2x},
    {"ae_no", 0, &tweak_ae_no},
    {"ae_nobands", 0, &tweak_ae_nobands},
    {"ae_noflare", 2, (void*)DBG_l_TweakWantsNoFlare},
    /* The overlay's list of which after-effects are running: 1 puts it
       up and leaves it up, 0 takes it down. */
    {"ae_view", 0, &tweak_ae_view},
    /* Whether the Direct3D renderer asks the hardware for a unit
       normal while it draws a chrome-mapped material: 1 as it should,
       0 as it was. Here to be turned off against a scene, the way the
       other renderer's chrome question was settled. */
    {"d3d_chrome", 0, &tweak_d3d_chrome},
    /* Which winding the OpenGL renderer culls: 0 as it decides, 1 no
       culling at all, 2 the facing reversed. Kept rather than removed
       with the bug it found -- the mapping it overrides is measured
       rather than derived, so the next card that disagrees wants this
       to hand. */
    {"gl_cull", 0, &tweak_gl_cull},
    /* Which way up the OpenGL renderer samples the water's
       reflection: 0 as it decides, 1 the other way. Here for the same
       reason as the line above -- the answer is a measurement on a
       real scene, not something to be worked out from two APIs'
       conventions. */
    {"gl_water_flip", 0, &tweak_gl_water_flip},
    {"mipmaplodbias", 1, &tweak_mipmaplodbias},
    {"perf", 0, &tweak_perf},
    {"perf_fps", 0, &tweak_perf_fps},    
    {"perf_fps_min", 1, &tweak_perf_fps_min},
    {"perf_fps_target", 1, &tweak_perf_fps_target},
    /* Sorted, and not by taste: the console finds a tweak by
       bsearching this table, so a name out of place is a tweak that
       cannot be set. water_min_bv sat above water_blur_normal and was
       one -- typing it did nothing, with no message to say why.
       tests/tweakcheck.sh is what stops that happening again; the
       console's own differential test cannot see it, because the
       table it drives is a sorted fixture of its own. */
    {"water_blur_normal", 0, &tweak_water_blur_normal},
    {"water_clip", 0, &tweak_water_clip},
    {"water_clip_off", 1, &tweak_water_off}, // water clip offset
    {"water_min_bv", 1, &tweak_water_min_bv},
    {"water_smooth", 0, &tweak_water_smooth},
    {"water_smoothx", 1, &tweak_water_smoothx},
    {"water_smoothy", 1, &tweak_water_smoothy},
    {"zonalpha_no", 2, (void*)DBG_l_TweakNoZonAlpha}
    
};

/* Beside the table, so the two are changed together. */
const int tweaksNum = (int)(sizeof(tweaks) / sizeof(tweaks[0]));

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine, const char* error, const char* extra, int showMsgBox)
{
    char* text;
    char* caption;

    text = (char*)malloc(2048u);
    if (!text)
    {
        exit(-1);
    }
    *text = 0;
    if (error)
    {
        strcat(text, error);
        if (extra)
            strcat(text, " (");
    }
    if (extra)
    {
        strcat(text, extra);
        if (error) {
            text[strlen(text) + 1] = '\0';
            text[strlen(text)] = ')';
        }
    }
    caption = "Assertion failed (Press Shift + OK For A Breakpoint);";
    if (!isError)
        caption = "Error detected";
    if (showMsgBox)
        MessageBoxA(0, text, caption, 0x10u);
    if (text)
        free(text);
    exit(-1);
}

static void DBG_l_Log(const char* text)
{
    static FILE* log;

    if (!log)
        log = fopen("debug.log", "w");
    if (log)
    {
        fputs(text, log);
        fflush(log);
    }
}

/* The timestamp the two loggers below put in front of a message, as
 * "[hh:mm:ss.mmm] ", and how many characters it came to.
 *
 * In front of the *message* and not in front of every write to the
 * log: DBG_DumpVecNoNL exists to write part of a line, and
 * DBG_DumpMatrix writes four rows in one go, so stamping the sink
 * would cut a stamp into the middle of a line and put four on a
 * matrix. Those three go on writing what they wrote.
 *
 * The clock is the time of day, because the date is in the banner at
 * the top of the file and the file holds one run.
 *
 * The room for it is 32 characters, which the two callers reserve on
 * top of what a message may say -- so adding the stamp did not
 * shorten any message.
 */
static int DBG_l_i_Stamp(char* out, int size)
{
    time_t     secs;
    struct tm* local;

    if (!out || size < 2)
        return 0;
    out[0] = 0;

    /* time() and localtime() and nothing else: both are C89, both are
       already used elsewhere in the engine, and neither brings a
       platform header in. The second is as fine as the standard C
       clock goes -- a millisecond would need C11's timespec_get, which
       is not promised by every compiler this has to build with, and
       within a second the order of the lines already says which came
       first. */
    secs = time(NULL);
    local = localtime(&secs);
    if (!local)
        return 0;

    snprintf(out, (size_t)size - 1, "[%02d:%02d:%02d] ",
              local->tm_hour, local->tm_min, local->tm_sec);
    out[size - 1] = 0;
    /* snprintf answers with what it wanted to write, and a negative
       number when it could not, so the length is taken from the string
       that is really there. */
    return (int)strlen(out);
}

/* What the log opens with: the engine, its version, when it was built
 * and what for, between two rules so the start of a run can be found
 * at a glance.
 *
 * The build stamp is this file's compile time -- see version.h -- so
 * it is exact after a full build and older than the run after a
 * partial one.
 */
void DBG_LogBanner(void)
{
    DBG_OutputDebugString("%s\n", IRISENG_BANNER_RULE);
    DBG_OutputDebugString("%s\n", IRISENG_BANNER_LINE);
    DBG_OutputDebugString("%s\n", IRISENG_BANNER_RULE);
}

/* And what it closes with. A log that ends without this line is a run
   that did not reach the end of WinMain. */
void DBG_LogExit(void)
{
    DBG_OutputDebugString("IRISEng exiting\n");
}

void DBG_OutputDebugString(char* Format, ...)
{
    /* The stamp's room is on top of the message's, so what a message
       may say is what it could say before. */
    CHAR OutputString[256 + 32];
    va_list va;
    int at;

    at = DBG_l_i_Stamp(OutputString, 32);
    va_start(va, Format);
    vsnprintf(OutputString + at, sizeof(OutputString) - (size_t)at - 1,
               Format, va);
    OutputString[sizeof(OutputString) - 1] = 0;
    va_end(va);
    OutputDebugStringA(OutputString);
    DBG_l_Log(OutputString);
}

/* The renderer's logger, which is this file's own with one bug: it
   handed vsnprintf the whole buffer, and that does not promise a
   terminator when it truncates, so a message of 256 characters or
   more was passed on unterminated. Reachable -- the screenshot path
   is logged through here. Terminated the same way as the one above
   now.

   The two are otherwise identical, and the D3D_ name is a leftover
   rather than anything renderer-specific. */
void GDI_OutputDebugString(char* Format, ...)
{
    CHAR OutputString[256 + 32];
    va_list ArgList;
    int at;

    at = DBG_l_i_Stamp(OutputString, 32);
    va_start(ArgList, Format);
    vsnprintf(OutputString + at, sizeof(OutputString) - (size_t)at - 1,
               Format, ArgList);
    OutputString[sizeof(OutputString) - 1] = 0;
    va_end(ArgList);
    OutputDebugStringA(OutputString);
    DBG_l_Log(OutputString);
}

void DBG_DumpMatrix(Mtx4x4 *mtx)
{
    CHAR OutputString[256];
    sprintf(OutputString, "%0.4f, %0.4f, %0.4f, %0.4f\n%0.4f, %0.4f, %0.4f, %0.4f\n%0.4f, %0.4f, %0.4f, %0.4f\n%0.4f, %0.4f, %0.4f, %0.4f\nFlags: %x\n",
        mtx->v1.x, mtx->v1.y, mtx->v1.z, mtx->_m14,
        mtx->v2.x, mtx->v2.y, mtx->v2.z, mtx->_m24,
        mtx->v3.x, mtx->v3.y, mtx->v3.z, mtx->_m34,
        mtx->Pos.x, mtx->Pos.y, mtx->Pos.z, mtx->w,
        mtx->mtxflag);
    OutputDebugStringA(OutputString);
    DBG_l_Log(OutputString);
}

void DBG_DumpVec(Vec3* vec)
{
    CHAR OutputString[256];
    sprintf(OutputString, "%0.10f, %0.10f, %0.10f\n", vec->x, vec->y, vec->z);
    OutputDebugStringA(OutputString);
    DBG_l_Log(OutputString);
}

void DBG_DumpVecNoNL(Vec3* vec)
{
    CHAR OutputString[256];
    sprintf(OutputString, "%0.10f, %0.10f, %0.10f", vec->x, vec->y, vec->z);
    OutputDebugStringA(OutputString);
    DBG_l_Log(OutputString);
}

/*
void DBG_DumpSallyHeadPos(char* tag) {
    tGameObjectMain* sally = (tGameObjectMain *)AI_ul_RealResolveOneRefObject(0x71003F5Au);
    if (sally == (tGameObjectMain*)-1) return;
    if (EVE_gpst_OwnerGAO != sally) return;
    tGameObjectMain* sallyhead = ANI_pst_GetObjectByAICanal(sally, 0);
    if (sallyhead == (tGameObjectMain*)-1) return;
    tGameObjectMain* sallyventre = ANI_pst_GetObjectByAICanal(sally, 12);
    if (sallyventre == (tGameObjectMain*)-1) return;
    tGameObjectMain* sallytorse = ANI_pst_GetObjectByAICanal(sally, 1);
    DBG_OutputDebugString("%s \n", tag);
    //DBG_DumpMatrix(&sallytorso->basic->mtx->mtx);
    //DBG_DumpMatrix(&((Mtx4x4*)sally->basic->multiPartData->multipartGaoObjectPartsOrMatrices)[10]); // torse, ez rezeg, ez a rossz
    //DBG_DumpMatrix(&((Mtx4x4*)sally->basic->multiPartData->multipartGaoObjectPartsOrMatrices)[9]); // ventre
    //DBG_DumpVec(&sallyhead->matrices->Pos);
    //DBG_OutputDebugString(" : ");
    //DBG_DumpVec(&sallyhead->basic->mtx->mtx.Pos);
    //DBG_OutputDebugString(" ");
    DBG_DumpMatrix(&sallytorse->basic->mtx->mtx);
}
*/