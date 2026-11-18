#include "../engine.h"
#include "../cameraengine.h"
#include "../math.h"
#include "../gfx/gfxcarte.h"
#include "../gfx.h"
#include "ai_worldfunctions.h"
#include "ai_gfxfunctions.h"
#include "../sound.h"
#include "../platform/wingdi.h"
#include "../video.h"
#include "../platform/winvideosound.h"
#include "../datafiles.h"
#include "../dbg.h"

// An effect's id says which of the world's two lists holds it: the ids
// below 1024 live in the list the engine draws after the animation
// engine's own effects.
static tGfxElem* AI_l_GfxList(int gfxId)
{
    if (gfxId >= 1024)
        return WOR_gpst_CurrentWorld->gfxCurrentList;
    return WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
}

static tGfxElem** AI_l_GfxListRef(int gfxId)
{
    if (gfxId >= 1024)
        return &WOR_gpst_CurrentWorld->gfxCurrentList;
    return &WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
}

// A stub: the three arguments are taken and nothing is done with them.
tAINode* AI_EvalFunc_GFXDepthBlur(tAINode* node)
{
    AI_GetFloatFromStack();
    AI_GetFloatFromStack();
    AI_GetIntFromStack();
    return node + 1;
}

// Puts the main display device into the orthographic no-clip setup and
// asks for a present. Both arguments are unused and the builtin always
// reports failure.
int AI_EvalFunc_GFXAddWorld_C(int unused1, int unused2)
{
    tGdi_DisplayData* mainDisplay;
    tGdi_DisplayData* previous;

    if (MAI_gst_MainHandles.displayData)
    {
        previous = gpst_DX_stDD;
        gpst_DX_stDD = MAI_gst_MainHandles.displayData;
        previous->SetProjectionMatrix(&CAM_gst_IsoNoClip);
        previous->SetViewMatrix(&MATH_gst_IdentityMatrix);
        gpst_DX_stDD = previous;
        mainDisplay = previous;

        // TODO FIXME: a 23-as requestet hívja, de az nem csinál semmit. Az eredeti kódban itt még van egy kuac float állítgatás amit a requestnek átad. 
        // Én ezeket nem implementáltam itt meg, mert így se csinál semmit

        mainDisplay->presentationFlags |= 0x400000u;
    }
    return -1;
}

tAINode* AI_EvalFunc_GFXAddWorld(tAINode* node)
{
    char arg1;
    int arg2;

    arg2 = AI_GetIntFromStack();
    arg1 = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_GFXAddWorld_C(arg1, arg2));
    return node + 1;
}

int AI_EvalFunc_GFXAdd_C(tGameObjectMain* aiGao, char type)
{
    return GFX_i_Add(&WOR_gpst_CurrentWorld->gfxCurrentList, type, aiGao);
}

int AI_EvalFunc_GFXAdd_C_CURRENT(char type)
{
    return AI_EvalFunc_GFXAdd_C(AI_gpst_CurrentGameObject, type);
}

tAINode* AI_EvalFunc_GFXAdd(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char type;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    type = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_GFXAdd_C(gaoUltra, type));
    return node + 1;
}

int AI_EvalFunc_GFXAddSorted_C(struct sGameObjectMain* aiGao, char type, int16_t idx)
{
    return GFX_i_AddSorted(&WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE, type, idx, aiGao);
}

tAINode* AI_EvalFunc_GFXAddSorted(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char type;
    int16_t idx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    idx = AI_GetIntFromStack();
    type = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_GFXAddSorted_C(gaoUltra, type, idx));
    return node + 1;
}

int AI_EvalFunc_GFXDel_C(tGameObjectMain* gao, int gfxId)
{
    tGfxElem** list;

    list = AI_l_GfxListRef(gfxId);
    return GFX_i_SetDeath(list, GFX_pst_FindById(list, gfxId));
}

int AI_EvalFunc_GFXDel_C_CURRENT(int gfxId)
{
    return AI_EvalFunc_GFXDel_C(AI_gpst_CurrentGameObject, gfxId);
}

tAINode* AI_EvalFunc_GFXDel(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXDel_C(gaoUltra, gfxId);
    return node + 1;
}

void AI_EvalFunc_GFXLifeTimeSet_C(struct sGameObjectMain* gao, int gfxId, float lifeTime)
{
    GFX_pst_FindById(AI_l_GfxListRef(gfxId), gfxId)->lifeTime = lifeTime;
}

tAINode* AI_EvalFunc_GFXLifeTimeSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    float lifeTime;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    lifeTime = AI_GetFloatFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXLifeTimeSet_C(gaoUltra, gfxId, lifeTime);
    return node + 1;
}

// Three of the four arguments are taken and discarded.
tAINode* AI_EvalFunc_GFXRequest(tAINode* node)
{
    int request;

    AI_GetIntFromStack();
    AI_GetIntFromStack();
    AI_GetIntFromStack();
    request = AI_GetIntFromStack();
    AI_PutIntToStack(GFX_i_Request(request));
    return node + 1;
}

float AI_EvalFunc_GFXGetf_C(tGameObjectMain* gao, int gfxId, int paramIdx)
{
    return GFX_f_Getf(AI_l_GfxList(gfxId), gfxId, paramIdx);
}

tAINode* AI_EvalFunc_GFXGetf(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int paramIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    paramIdx = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_PutFloatToStack(AI_EvalFunc_GFXGetf_C(gaoUltra, gfxId, paramIdx));
    return node + 1;
}

void AI_EvalFunc_GFXSetf_C(tGameObjectMain* gao, int gfxId, int paramIdx, float value)
{
    // FIXME: ez a baromság ide nem kell, valószínû azért van mert a getcurrent már __int16-ot vár
    GFX_Setf(AI_l_GfxList(gfxId), gfxId, paramIdx, value);
}

void GFXSetCurGlobal(int gfxId)
{
    GFX_i_GlobalID = gfxId;
}

void AI_EvalFunc_GFXSetf_C_CURGLOB(int paramIdx, float value)
{
    AI_EvalFunc_GFXSetf_C(AI_gpst_CurrentGameObject, GFX_i_GlobalID, paramIdx, value);
}

void AI_EvalFunc_GFXSetf_C_CURRENT(int gfxId, int paramIdx, float value)
{
    AI_EvalFunc_GFXSetf_C(AI_gpst_CurrentGameObject, gfxId, paramIdx, value);
}

tAINode* AI_EvalFunc_GFXSetf(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int paramIdx;
    float value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    value = AI_GetFloatFromStack();
    paramIdx = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXSetf_C(gaoUltra, gfxId, paramIdx, value);
    return node + 1;
}

int AI_EvalFunc_GFXGeti_C(struct sGameObjectMain* gao, int gfxId, int paramIdx)
{
    return GFX_i_Geti(AI_l_GfxList(gfxId), gfxId, paramIdx);
}

tAINode* AI_EvalFunc_GFXGeti(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int paramIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    paramIdx = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_GFXGeti_C(gaoUltra, gfxId, paramIdx));
    return node + 1;
}

void AI_EvalFunc_GFXSeti_C(tGameObjectMain* gao, int gfxId, int paramIdx, int value)
{
    GFX_Seti(AI_l_GfxList(gfxId), gfxId, paramIdx, value);
}

void AI_EvalFunc_GFXSeti_C_CURGLOB(int paramIdx, int value)
{
    AI_EvalFunc_GFXSeti_C(AI_gpst_CurrentGameObject, GFX_i_GlobalID, paramIdx, value);
}

void AI_EvalFunc_GFXSeti_C_CURRENT(int gfxId, int paramIdx, int value)
{
    AI_EvalFunc_GFXSeti_C(AI_gpst_CurrentGameObject, gfxId, paramIdx, value);
}

tAINode* AI_EvalFunc_GFXSeti(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int paramIdx;
    int value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    value = AI_GetIntFromStack();
    paramIdx = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXSeti_C(gaoUltra, gfxId, paramIdx, value);
    return node + 1;
}

void AI_EvalFunc_GFXGetv_C(struct sGameObjectMain* gao, int gfxId, int paramIdx, Vec3* value)
{
    *value = *GFX_pst_Getv(AI_l_GfxList(gfxId), gfxId, paramIdx);
}

tAINode* AI_EvalFunc_GFXGetv(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int paramIdx;
    Vec3 value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    paramIdx = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXGetv_C(gaoUltra, gfxId, paramIdx, &value);
    AI_PutVecToStack(&value);
    return node + 1;
}

void AI_EvalFunc_GFXSetv_C(tGameObjectMain* gao, int gfxId, int paramIdx, Vec3* value)
{
    GFX_Setv(AI_l_GfxList(gfxId), gfxId, paramIdx, value);
}

void AI_EvalFunc_GFXSetv_C_CURRENT(int gfxId, int paramIdx, Vec3* value)
{
    AI_EvalFunc_GFXSetv_C(AI_gpst_CurrentGameObject, gfxId, paramIdx, value);
}

void AI_EvalFunc_GFXSetv_C_CURGLOB(int paramIdx, Vec3* value)
{
    AI_EvalFunc_GFXSetv_C(AI_gpst_CurrentGameObject, GFX_i_GlobalID, paramIdx, value);
}

tAINode* AI_EvalFunc_GFXSetv(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int paramIdx;
    Vec3* value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    // the vector is read where the stack keeps it rather than copied out
    value = (Vec3*)AI_PopStackSlot();
    paramIdx = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXSetv_C(gaoUltra, gfxId, paramIdx, value);
    return node + 1;
}

void AI_EvalFunc_GFXFlagSet_C(tGameObjectMain* gao, int gfxId, int flag, int set)
{
    GFX_FlagSet(AI_l_GfxList(gfxId), gfxId, flag, set);
}

void AI_EvalFunc_GFXFlagSet_C_CURRENT(int gfxId, char flag, int set)
{
    AI_EvalFunc_GFXFlagSet_C(AI_gpst_CurrentGameObject, gfxId, flag, set);
}

tAINode* AI_EvalFunc_GFXFlagSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int gfxId;
    int flag;
    int set;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    set = AI_GetIntFromStack();
    flag = AI_GetIntFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXFlagSet_C(gaoUltra, gfxId, flag, set);
    return node + 1;
}

// Gives the effect the material of another object's visual. A
// multi-material is indexed by materialID, wrapped to its length.
void AI_EvalFunc_GFXMaterialSet_C(tGameObjectMain* gao, int gfxId, tGameObjectMain* source, int materialID)
{
    tGEOMAT* material;

    if (!source || (source->flags & 0x4000) == 0)
        return;

    material = (tGEOMAT*)(((tGameObjectVisualObject*)(source->basic->actorObjectOrVisualObject))->GROMAT);
    if (material && materialID >= 0 && material->intf->id == 4)
    {
        material = material->multiMaterialPtrIfType4ThenDiffuseColor[
            materialID % material->multiMaterialNumIfType4ThenAmbiantColor];
    }
    GFX_SetMaterial(AI_l_GfxList(gfxId), gfxId, material);
}

void AI_EvalFunc_GFXMaterialSet_C_CURRENT(int gfxId, struct sGameObjectMain* source, int materialID)
{
    AI_EvalFunc_GFXMaterialSet_C(AI_gpst_CurrentGameObject, gfxId, source, materialID);
}

tAINode* AI_EvalFunc_GFXMaterialSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* source;
    int gfxId;
    int materialID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    materialID = AI_GetIntFromStack();
    source = (tGameObjectMain*)AI_GetObjectFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXMaterialSet_C(gaoUltra, gfxId, source, materialID);
    return node + 1;
}

void AI_EvalFunc_GFXCarteObject_C(tGameObjectMain* gao, int gfxId, tGameObjectMain* target, int mode, int textElemIndexId)
{
    GFX_Carte_Object(AI_l_GfxList(gfxId), gfxId, target, mode, textElemIndexId);
}

tAINode* AI_EvalFunc_GFXCarteObject(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* target;
    int gfxId;
    int mode;
    int textElemIndexId;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    textElemIndexId = AI_GetIntFromStack();
    mode = AI_GetIntFromStack();
    target = (tGameObjectMain*)AI_GetObjectFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXCarteObject_C(gaoUltra, gfxId, target, mode, textElemIndexId);
    return node + 1;
}

tVideoStream* GFX_gpv_FS_Video = 0;
static tVideoSound* GFX_gpst_FS_VideoSound = 0;
int GFX_gl_FS_VideoPlaying = 0;
int GFX_gl_FS_VideoScaleMode = 0;

// Which of the renderer's fits the scale mode asks for.
static int GFX_l_i_VideoFit(void)
{
    if (GFX_gl_FS_VideoScaleMode & 0x2000000)
        return GDI_VIDEOFIT_SMOOTH;
    if (GFX_gl_FS_VideoScaleMode & 0x1000000)
        return GDI_VIDEOFIT_SHARP;
    return GDI_VIDEOFIT_ACTUALSIZE;
}

static void GFX_FS_VideoDecode(tVideoStream* video)
{
    void* bits;
    int pitch;
    int height;

    if (!VID_b_DecodeFrame(video))
        return;
    // The picture reports its own height, so the frame is clamped to
    // what it is being written into rather than to anything else.
    if (!gpst_DX_stDD->b_VideoFrameLock(&bits, &pitch, &height))
        return;
    VID_CopyToBuffer(video, bits, pitch, height, 1);
    gpst_DX_stDD->VideoFrameUnlock();
}

static void GFX_FS_VideoBlit(tVideoStream* video)
{
    gpst_DX_stDD->VideoFramePresent(VID_i_Width(video), VID_i_Height(video),
                                    GFX_l_i_VideoFit());
}

// The id picks the file out of datafiles.ini. Nothing to play is not
// an error -- it is logged and the video simply does not start, which
// leaves everything as it was.
void GFX_FS_VideoStart(int videoId)
{
    const char* file;

    file = DAT_pc_Video(videoId);
    if (!file)
    {
        DBG_OutputDebugString("GFX_FS_VideoStart: no video for id 0x%08X (%d)"
                              " in %s\n",
                              (unsigned)videoId, videoId, DAT_INIFILE);
        return;
    }

    GFX_gpv_FS_Video = VID_pst_OpenFile(file, 1);
    if (!GFX_gpv_FS_Video)
    {
        DBG_OutputDebugString("GFX_FS_VideoStart: cannot open \"%s\""
                              " for id 0x%08X\n", file, (unsigned)videoId);
        return;
    }
    // The sound is ours to play now: libav decodes it and nothing
    // more. A video with no sound track, or a device that will not
    // give up a buffer, leaves the picture to the wall clock.
    GFX_gpst_FS_VideoSound = WVS_pst_Open(GFX_gpv_FS_Video);

    if (!gpst_DX_stDD->b_VideoDisplayCreate(VID_i_Width(GFX_gpv_FS_Video),
                                            VID_i_Height(GFX_gpv_FS_Video)))
    {
        DBG_OutputDebugString("GFX_FS_VideoStart: the renderer gave no picture"
                              " for \"%s\"\n", file);
        WVS_Close(GFX_gpst_FS_VideoSound);
        GFX_gpst_FS_VideoSound = 0;
        VID_Close(GFX_gpv_FS_Video);
        GFX_gpv_FS_Video = 0;
        return;
    }

    GFX_gl_FS_VideoPlaying = 1;
}

tAINode* AI_EvalFunc_GFX_FS_VideoStart(tAINode* node)
{
    GFX_FS_VideoStart(AI_GetIntFromStack());
    return node + 1;
}

void GFX_FS_VideoStop()
{
    if (GFX_gpv_FS_Video)
    {
        gpst_DX_stDD->VideoDisplayClose();
        WVS_Close(GFX_gpst_FS_VideoSound);
        GFX_gpst_FS_VideoSound = 0;
        VID_Close(GFX_gpv_FS_Video);
        GFX_gpv_FS_Video = 0;
    }
    GFX_gl_FS_VideoPlaying = 0;
}

tAINode* AI_EvalFunc_GFX_FS_VideoStop(tAINode* node)
{
    GFX_FS_VideoStop();
    return node + 1;
}

int GFX_FS_VideoFinish() {
    if (!GFX_gpv_FS_Video)
        return 1;
    return VID_b_Ended(GFX_gpv_FS_Video);
}

tAINode* AI_EvalFunc_GFX_FS_VideoFinish(tAINode* node)
{
    AI_PutIntToStack(GFX_FS_VideoFinish());
    return node + 1;
}

void GFX_FS_VideoUpdate()
{
    if (!GFX_gpv_FS_Video)
        return;

    // Topping the sound buffer up is also what sets the clock the
    // frame below is paced by, so it comes first and happens even on
    // the frames where nothing is decoded.
    WVS_Update(GFX_gpst_FS_VideoSound, GFX_gpv_FS_Video);

    if (VID_b_Waiting(GFX_gpv_FS_Video))
    {
        GFX_FS_VideoBlit(GFX_gpv_FS_Video);
        return;
    }

    GFX_FS_VideoDecode(GFX_gpv_FS_Video);
    GFX_FS_VideoBlit(GFX_gpv_FS_Video);
    VID_NextFrame(GFX_gpv_FS_Video);
}

// The explosion takes its material from the visual of the object the
// script names. arg1 and arg2 are passed straight through to gfx.c,
// whose own parameters are not named yet.
tAINode* AI_EvalFunc_CreateExplosion(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectVisualObject* visual;
    tGameObjectMain* source;
    Vec3* pos;
    float arg1;
    float arg2;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    source = (tGameObjectMain*)AI_GetObjectFromStack();

    // the position is read where the stack keeps it rather than copied out
    pos = (Vec3*)AI_PopStackSlot();
    arg2 = AI_GetFloatFromStack();
    arg1 = AI_GetFloatFromStack();

    if (!source->basic || !source->basic->actorObjectOrVisualObject)
        return node + 1;

    visual = (tGameObjectVisualObject*)(source->basic->actorObjectOrVisualObject);
    if (visual->GROMAT)
        GFX_CreateExplosion(arg1, arg2, pos, gaoUltra, (tGEOMAT*)visual->GROMAT);
    return node + 1;
}