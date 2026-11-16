#include "dbg.h"
//#include "animation.h" //tmp

int tweak_ae_no;
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

void DBG_OutputDebugString(char* Format, ...)
{
    CHAR OutputString[256];
    va_list va;

    va_start(va, Format);
    _vsnprintf(OutputString, sizeof(OutputString) - 1, Format, va);
    OutputString[sizeof(OutputString) - 1] = 0;
    va_end(va);
    OutputDebugStringA(OutputString);
    DBG_l_Log(OutputString);
}

/* The renderer's logger, which is this file's own with one bug: it
   handed _vsnprintf the whole buffer, and that does not promise a
   terminator when it truncates, so a message of 256 characters or
   more was passed on unterminated. Reachable -- the screenshot path
   is logged through here. Terminated the same way as the one above
   now.

   The two are otherwise identical, and the D3D_ name is a leftover
   rather than anything renderer-specific. */
void GDI_OutputDebugString(char* Format, ...)
{
    CHAR OutputString[256];
    va_list ArgList;

    va_start(ArgList, Format);
    _vsnprintf(OutputString, sizeof(OutputString) - 1, Format, ArgList);
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