#include "../engine.h"
#include "../cameraengine.h"
#include "../math.h"
#include "../gfx/gfxcarte.h"
#include "../gfx.h"
#include "ai_worldfunctions.h"
#include "ai_gfxfunctions.h"

tAINode* AI_EvalFunc_GFXDepthBlur(tAINode* a1)
{
    AI_GetFloatFromStack();
    AI_GetFloatFromStack();
    AI_GetIntFromStack();
    return a1 + 1;
}

int AI_EvalFunc_GFXAddWorld_C(int a1, int a2)
{
    tGdi_DisplayData* v5;

    if (MAI_gst_MainHandles.displayData)
    {
        v5 = gpst_DX_stDD;
        gpst_DX_stDD = MAI_gst_MainHandles.displayData;
        v5->SetProjectionMatrix(&CAM_gst_IsoNoClip);
        v5->SetViewMatrix(&MATH_gst_IdentityMatrix);
        gpst_DX_stDD = v5;
       
        // TODO FIXME: a 23-as requestet hívja, de az nem csinál semmit. Az eredeti kódban itt még van egy kuac float állítgatás amit a requestnek átad. 
        // Én ezeket nem implementáltam itt meg, mert úgy se csinál semmit

        v5->presentationFlags |= 0x400000u;
    }
    return -1;
}

tAINode* AI_EvalFunc_GFXAddWorld(tAINode* a1)
{
    int v1;
    char v3;
    int v4;

    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_GFXAddWorld_C(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GFXAdd_C(tGameObjectMain* aiGao, char a2)
{
    return GFX_i_Add(&WOR_gpst_CurrentWorld->gfxCurrentList, a2, aiGao);
}

int AI_EvalFunc_GFXAdd_C_CURRENT(char a1)
{
    return GFX_i_Add(&WOR_gpst_CurrentWorld->gfxCurrentList, a1, AI_gpst_CurrentGameObject);
}

tAINode* AI_EvalFunc_GFXAdd(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_GFXAdd_C(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GFXAddSorted_C(struct sGameObjectMain* a4, char a2, __int16 a3)
{
    return GFX_i_AddSorted(&WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE, a2, a3, a4);
}

tAINode* AI_EvalFunc_GFXAddSorted(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char v4;
    __int16 a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_GFXAddSorted_C(v3, v4, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_GFXDel_C(tGameObjectMain* a1, int a2)
{
    tGfxElem* v2;
    tGfxElem** v4;

    if (a2 >= 1024)
        v4 = &WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = &WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    v2 = GFX_pst_FindById(v4, a2);
    return GFX_i_SetDeath(v4, v2);
}

int AI_EvalFunc_GFXDel_C_CURRENT(int a1)
{
    tGfxElem* v1;
    tGfxElem** v3;

    if (a1 >= 1024)
        v3 = &WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v3 = &WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    v1 = GFX_pst_FindById(v3, a1);
    return GFX_i_SetDeath(v3, v1);
}

tAINode* AI_EvalFunc_GFXDel(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    AI_EvalFunc_GFXDel_C(v2, v3);
    return a1 + 1;
}

void AI_EvalFunc_GFXLifeTimeSet_C(struct sGameObjectMain* a1, int gfxId, float a3)
{
    tGfxElem** v3;

    if (gfxId >= 1024)
        v3 = &WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v3 = &WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_pst_FindById(v3, gfxId)->lifeTime = a3;
}

tAINode* AI_EvalFunc_GFXLifeTimeSet(tAINode* a1)
{
    tGameObjectMain* v2;
    float a3;
    int gfxId;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    gfxId = AI_GetIntFromStack();
    AI_EvalFunc_GFXLifeTimeSet_C(v2, gfxId, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_GFXRequest(tAINode* a1)
{
    int v1;
    int v2;

    AI_GetIntFromStack();
    AI_GetIntFromStack();
    AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v2 = GFX_i_Request(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_GFXGetf_C(tGameObjectMain* a1, int a2, int a3)
{
    tGfxElem* v4;

    if (a2 >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    return GFX_f_Getf(v4, a2, a3);
}

tAINode* AI_EvalFunc_GFXGetf(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;
    int a3;
    int a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_GFXGetf_C(v3, a2, a3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_GFXSetf_C(tGameObjectMain* a1, int id, int a2, float a3)
{
    int v4;
    tGfxElem* v5;

    if (id >= 1024)
        v5 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v5 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    // FIXME: ez a baromság ide nem kell, valószínû azért van mert a getcurrent már __int16-ot vár
    //HIWORD(v4) = HIWORD(v5);
    //LOWORD(v4) = id;
    v4 = id;
    GFX_Setf(v5, v4, a2, a3);
}

void GFXSetCurGlobal(int a1)
{
    GFX_i_GlobalID = a1;
}

void AI_EvalFunc_GFXSetf_C_CURGLOB(int a2, float a3)
{
    int v2;
    tGfxElem* v3;

    if (GFX_i_GlobalID >= 1024)
        v3 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v3 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    // FIXME: ez a baromság ide nem kell, valószínû azért van mert a getcurrent már __int16-ot vár
    //HIWORD(v2) = HIWORD(v3);
    //LOWORD(v2) = GFX_i_GlobalID;
    v2 = GFX_i_GlobalID;
    GFX_Setf(v3, v2, a2, a3);
}

void AI_EvalFunc_GFXSetf_C_CURRENT(int id, int a2, float a3)
{
    int v3;
    tGfxElem* v4;

    if (id >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    // FIXME: ez a baromság ide nem kell, valószínû azért van mert a getcurrent már __int16-ot vár
    //HIWORD(v3) = HIWORD(v4);
    //LOWORD(v3) = id;
    v3 = id;
    GFX_Setf(v4, v3, a2, a3);
}

tAINode* AI_EvalFunc_GFXSetf(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;
    int v4;
    float v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = AI_GetFloatFromStack();
    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    AI_EvalFunc_GFXSetf_C(v2, v4, v3, v5);
    return a1 + 1;
}

int AI_EvalFunc_GFXGeti_C(struct sGameObjectMain* a1, int a2, int a3)
{
    tGfxElem* v4;

    if (a2 >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    return GFX_i_Geti(v4, a2, a3);
}

tAINode* AI_EvalFunc_GFXGeti(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int v4;
    int v5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_GFXGeti_C(v3, v5, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_GFXSeti_C(tGameObjectMain* a1, int a2, int a3, int a4)
{
    tGfxElem* v4;

    if (a2 >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Seti(v4, a2, a3, a4);
}

void AI_EvalFunc_GFXSeti_C_CURGLOB(int a1, int a2)
{
    tGfxElem* v2;

    if (GFX_i_GlobalID >= 1024)
        v2 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v2 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Seti(v2, GFX_i_GlobalID, a1, a2);
}

void AI_EvalFunc_GFXSeti_C_CURRENT(int a1, int a2, int a3)
{
    tGfxElem* v3;

    if (a1 >= 1024)
        v3 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v3 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Seti(v3, a1, a2, a3);
}

tAINode* AI_EvalFunc_GFXSeti(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;
    int v4;
    int v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    AI_EvalFunc_GFXSeti_C(v2, v4, v3, v5);
    return a1 + 1;
}

void AI_EvalFunc_GFXGetv_C(struct sGameObjectMain* a1, int a2, int a3, Vec3* a4)
{
    tGfxElem* v4;
    Vec3 *ret;

    if (a2 >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    ret = GFX_pst_Getv(v4, a2, a3);
    *a4 = *ret;
}

tAINode* AI_EvalFunc_GFXGetv(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;
    Vec3 a1a;
    tGameObjectMain* v6;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v6 = v2;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GFXGetv_C(v2, a2, a3, &a1a);
    AI_PutVecToStack(&a1a);
    return a1 + 1;
}

void AI_EvalFunc_GFXSetv_C(tGameObjectMain* a1, int a2, int a3, Vec3* a4)
{
    tGfxElem* v4;

    if (a2 >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Setv(v4, a2, a3, a4);
}

void AI_EvalFunc_GFXSetv_C_CURRENT(int a1, int a2, Vec3* a3)
{
    tGfxElem* v3;

    if (a1 >= 1024)
        v3 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v3 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Setv(v3, a1, a2, a3);
}

void AI_EvalFunc_GFXSetv_C_CURGLOB(int a1, Vec3* a2)
{
    tGfxElem* v2;

    if (GFX_i_GlobalID >= 1024)
        v2 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v2 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Setv(v2, GFX_i_GlobalID, a1, a2);
}

tAINode* AI_EvalFunc_GFXSetv(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;
    Vec3* a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a4 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GFXSetv_C(v2, a2, a3, a4);
    return a1 + 1;
}

void AI_EvalFunc_GFXFlagSet_C(tGameObjectMain* a1, int a2, int a3, int a4)
{
    tGfxElem* v4;

    if (a2 >= 1024)
        v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_FlagSet(v4, a2, a3, a4);
}

void AI_EvalFunc_GFXFlagSet_C_CURRENT(int a1, char a2, int a3)
{
    tGfxElem* v3;

    if (a1 >= 1024)
        v3 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v3 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_FlagSet(v3, a1, a2, a3);
}

tAINode* AI_EvalFunc_GFXFlagSet(tAINode* a1)
{
    tGameObjectMain* v2; 
    int v3;
    int v4;
    int v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    AI_EvalFunc_GFXFlagSet_C(v2, v4, v5, v3);
    return a1 + 1;
}

void AI_EvalFunc_GFXMaterialSet_C(tGameObjectMain* a1, int a2, tGameObjectMain* a3, int materialID)
{
    tGfxElem* v4;
    tGEOMAT* v5;

    if (a3 && (a3->flags & 0x4000) != 0)
    {
        if (a2 >= 1024)
            v4 = WOR_gpst_CurrentWorld->gfxCurrentList;
        else
            v4 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
        v5 = (tGEOMAT*)(((tGameObjectVisualObject*)(a3->basic->actorObjectOrVisualObject))->GROMAT);
        if (v5 && materialID >= 0 && v5->intf->id == 4)
            v5 = v5->multiMaterialPtrIfType4ThenDiffuseColor[materialID % v5->multiMaterialNumIfType4ThenAmbiantColor];
        GFX_SetMaterial(v4, a2, v5);
    }
}

void AI_EvalFunc_GFXMaterialSet_C_CURRENT(int a1, struct sGameObjectMain* a2, int a3)
{
    AI_EvalFunc_GFXMaterialSet_C(AI_gpst_CurrentGameObject, a1, a2, a3);
}

tAINode* AI_EvalFunc_GFXMaterialSet(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    tGameObjectMain* a3;
    int materialID;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    materialID = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_GFXMaterialSet_C(v3, v1, a3, materialID);
    return a1 + 1;
}

void AI_EvalFunc_GFXCarteObject_C(tGameObjectMain* a1, int a2, tGameObjectMain* a3, int a4, int a5)
{
    tGfxElem* v5;

    if (a2 >= 1024)
        v5 = WOR_gpst_CurrentWorld->gfxCurrentList;
    else
        v5 = WOR_gpst_CurrentWorld->gfxCurrentListSortedAfterAE;
    GFX_Carte_Object(v5, a2, a3, a4, a5);
}

tAINode* AI_EvalFunc_GFXCarteObject(tAINode* a1)
{
    tGameObjectMain* v2;
    tGameObjectMain* a3;
    int a5;
    int a2;
    int a4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GFXCarteObject_C(v2, a2, a3, a4, a5);
    return a1 + 1;
}

/* Fullscreen Bink playback. binkw32 is not linked here, so the Bink
 * calls sit behind IRISENG_HAVE_BINK; without it nothing plays and
 * VideoFinish says so at once, which is what the scripts cope with.
 * How these were recovered: ai/notes/gfx-fs-video.md
 */
void* GFX_gpv_FS_Video = 0;
int GFX_gl_FS_VideoPlaying = 0;

void GFX_FS_VideoStart(int a1)
{
    /* Read off the AI stack and discarded: the file name is not a
     * parameter. Kept so the signature matches. */
    (void)a1;
#ifdef IRISENG_HAVE_BINK
    BinkSetSoundSystem(BinkOpenDirectSound, GFX_FS_VideoSoundDevice());
    GFX_gpv_FS_Video = BinkOpen("BGEMakingOf.bik", 0);
    if (!GFX_gpv_FS_Video)
        return;
    GFX_gl_FS_VideoPlaying = 1;
#else
    GFX_gpv_FS_Video = 0;
    GFX_gl_FS_VideoPlaying = 0;
#endif
}

tAINode* AI_EvalFunc_GFX_FS_VideoStart(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    GFX_FS_VideoStart(v1);
    return a1 + 1;
}

void GFX_FS_VideoStop()
{
#ifdef IRISENG_HAVE_BINK
    if (GFX_gpv_FS_Video)
    {
        BinkClose(GFX_gpv_FS_Video);
        GFX_gpv_FS_Video = 0;
    }
#endif
    GFX_gl_FS_VideoPlaying = 0;
}

tAINode* AI_EvalFunc_GFX_FS_VideoStop(tAINode* a1)
{
    GFX_FS_VideoStop();
    return a1 + 1;
}

/* 1 when there is no handle, otherwise the last frame has been
 * reached. With no Bink handle there never is one, so this stays 1. */
int GFX_FS_VideoFinish() {
    return 1;
}

tAINode* AI_EvalFunc_GFX_FS_VideoFinish(tAINode* a1)
{
    int v1;

    v1 = GFX_FS_VideoFinish();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_CreateExplosion(tAINode* arg0)
{
    tGameObjectMain* v2;
    tGameObjectMain* v3;
    Vec3* a3;
    float a2;
    float a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    a3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    a2 = AI_GetFloatFromStack();
    a1 = AI_GetFloatFromStack();
    if (v3->basic && v3->basic->actorObjectOrVisualObject && ((tGameObjectVisualObject*)(v3->basic->actorObjectOrVisualObject))->GROMAT)
        GFX_CreateExplosion(a1, a2, a3, v2, (tGEOMAT*)((tGameObjectVisualObject*)(v3->basic->actorObjectOrVisualObject))->GROMAT);
    return arg0 + 1;
}