#include "dbg.h"
//#include "animation.h" //tmp

int tweak_ae_no;
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

struct sTweakElem tweaks[17] = {
    {"ae_d2x", 0, &D3DRenderParams.tweak_ae_d2x},
    {"ae_no", 0, &tweak_ae_no},
    {"ae_nobands", 0, &tweak_ae_nobands},
    {"ae_noflare", 0, &D3DRenderParams.tweak_ae_noflare},
    {"mipmaplodbias", 1, &tweak_mipmaplodbias},
    {"perf", 0, &tweak_perf},
    {"perf_fps", 0, &tweak_perf_fps},    
    {"perf_fps_min", 1, &tweak_perf_fps_min},
    {"perf_fps_target", 1, &tweak_perf_fps_target},
    {"water_min_bv", 1, &tweak_water_min_bv},
    {"water_blur_normal", 0, &tweak_water_blur_normal},
    {"water_clip", 0, &tweak_water_clip},
    {"water_clip_off", 1, &tweak_water_off}, // water clip offset
    {"water_smooth", 0, &tweak_water_smooth},
    {"water_smoothx", 1, &tweak_water_smoothx},
    {"water_smoothy", 1, &tweak_water_smoothy},
    {"zonalpha_no", 0, &D3DRenderParams.tweak_zonalpha}
    
};

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

void DBG_OutputDebugString(char* Format, ...)
{
    CHAR OutputString[256];
    va_list va;

    va_start(va, Format);
    vsprintf(OutputString, Format, va);
    va_end(va);
    OutputDebugStringA(OutputString);
}

void D3D_OutputDebugString(char* Format, ...)
{
    CHAR OutputString[256];
    va_list ArgList;

    va_start(ArgList, Format);
    _vsnprintf(OutputString, 256, Format, ArgList);
    va_end(ArgList);
    OutputDebugStringA(OutputString);
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
}

void DBG_DumpVec(Vec3* vec)
{
    CHAR OutputString[256];
    sprintf(OutputString, "%0.10f, %0.10f, %0.10f\n", vec->x, vec->y, vec->z);
    OutputDebugStringA(OutputString);
}

void DBG_DumpVecNoNL(Vec3* vec)
{
    CHAR OutputString[256];
    sprintf(OutputString, "%0.10f, %0.10f, %0.10f", vec->x, vec->y, vec->z);
    OutputDebugStringA(OutputString);
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