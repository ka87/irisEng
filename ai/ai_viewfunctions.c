#include "../gameobject.h"
#include "../gdi.h"
#include "../textures.h"
#include "../memory.h"
#include "../inputoutput.h"
#include "../photo.h"
#include "../jpeg-6b/jpeglib.h"
#include "ai_viewfunctions.h"
#include "../dbg.h"

int VIEWSNAP_Slot = -1;

void AI_EvalFunc_VIEWSnapshot_DoIt()
{
    tJpegLibIntf jpgIntf;
    tD3dRequestSnapshot rs;
    int snapSlotBackup, quality;
    void* srcData;

    if (VIEWSNAP_Slot != -1)
    {
        snapSlotBackup = VIEWSNAP_Slot;
        VIEWSNAP_Slot = -1;
        AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
        AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

        if (!WOR_gpst_CurrentWorld || !WOR_gpst_CurrentWorld->viewPtr->displayData) return;

        MEM_ResetTmpMemory();

        MEM_gb_EnableTmp = TRUE;
        gpst_DX_stDD = WOR_gpst_CurrentWorld->viewPtr->displayData;
        rs.flags = 0;

        if (WOR_gpst_CurrentWorld->viewPtr->displayData->winWidth >= 512)
        {
            rs.cutSizeFromTopAndBottom = (WOR_gpst_CurrentWorld->viewPtr->displayData->winWidth - 512) / 2;
            rs.width = 512;
        }
        else
        {
            rs.cutSizeFromTopAndBottom = 0;
            rs.width = WOR_gpst_CurrentWorld->viewPtr->displayData->winWidth - WOR_gpst_CurrentWorld->viewPtr->displayData->winWidth % 4;
        }

        if (WOR_gpst_CurrentWorld->viewPtr->displayData->winHeight >= 256)
        {
            rs.cutSizeFromSides = (WOR_gpst_CurrentWorld->viewPtr->displayData->winHeight - 256) / 2;
            rs.height = 256;
        }
        else
        {
            rs.cutSizeFromSides = 0;
            rs.height = WOR_gpst_CurrentWorld->viewPtr->displayData->winHeight;
        }

        rs.srcData = (char*)MEM_p_AllocTmp(3 * rs.height * rs.width);

        WOR_gpst_CurrentWorld->viewPtr->displayData->Request(24, &rs);

        if (rs.width == 512 && rs.height == 256)
        {
            TEX_Compress24_Xo2(rs.srcData, rs.srcData, 512, 256);
            TEX_Compress24_Yo2(rs.srcData, rs.srcData, 256, 256);
            rs.width = 256;
            rs.height = 128;
        }
        
        /*
        FILE* fp1 = fopen("dbg.ppm", "wb");
        if (fp1) {
            fprintf(fp1, "P6\n%d %d\n255\n", rs.width, rs.height);
            fwrite(rs.srcData, 1, rs.width * rs.height * 3, fp1);
            fclose(fp1);
        }
        */

        srcData = MEM_p_AllocTmp(3 * rs.height * rs.width);
        memcpy(srcData, rs.srcData, 3 * rs.width * rs.height);

        quality = 70;

        do
        {
            memset(&jpgIntf, 0, sizeof(jpgIntf));
            jpgIntf.width = rs.width;
            jpgIntf.height = rs.height;
            jpgIntf.indata = srcData;
            jpgIntf.quality = quality;
            jpgIntf.outdata = rs.srcData;
            
            lJADE_JPEG_INPOUT_FILE_LENGTH = 0;
            JpegWrite(&jpgIntf);
            quality -= 5;

        } while ((int)lJADE_JPEG_INPOUT_FILE_LENGTH > 4096);
      
        /*
        FILE* fp2 = fopen("dbg.jpg", "wb");
        if (fp2) {
            fwrite(rs.srcData, 1, lJADE_JPEG_INPOUT_FILE_LENGTH, fp2);
            fclose(fp2);
        }
        */

        SAV_PhotoSet(snapSlotBackup, rs.srcData, lJADE_JPEG_INPOUT_FILE_LENGTH);
        MEM_Free(srcData);
        MEM_Free(rs.srcData);
        MEM_gb_EnableTmp = FALSE;
    }
}

void AI_EvalFunc_VIEWSnapshot_C(int slotID)
{
    VIEWSNAP_Slot = slotID;
}

tAINode* AI_EvalFunc_VIEWSnapshot(tAINode* a1)
{
    int slotID = AI_GetIntFromStack();

    AI_EvalFunc_VIEWSnapshot_C(slotID);
    return a1 + 1;
}

int AI_EvalFunc_VIEWAnalyser_C(struct sGameObjectMain* a1, int mission)
{
    if (!WOR_gpst_CurrentWorld)
        return 0;

    if (mission > 0)
        return IMG_Analyser(WOR_gpst_CurrentWorld, mission);

    return IMG_Analyser_Bis(WOR_gpst_CurrentWorld, mission);
}

tAINode* AI_EvalFunc_VIEWAnalyser(tAINode* aiNode)
{
    int ret;
    tGameObjectMain* ultraBak;
    int mission;

    mission = AI_GetIntFromStack();
    ultraBak = AI_gpst_CurrentUltra;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    ret = AI_EvalFunc_VIEWAnalyser_C(ultraBak, mission);

    AI_PutIntToStack(ret);

    return aiNode + 1;
}

struct sGameObjectMain* AI_EvalFunc_VIEWAssignObject_C(struct sGameObjectMain* gao, int viewIdx)
{
    tGameObjectMain* prevAssignedGao;

    if (!WOR_gpst_CurrentWorld)
        return NULL;

    prevAssignedGao = WOR_gpst_CurrentWorld->viewPtr[viewIdx].assignedGao;
    WOR_gpst_CurrentWorld->viewPtr[viewIdx].assignedGao = gao;

    return prevAssignedGao;
}

tAINode* AI_EvalFunc_VIEWAssignObject(tAINode* aiNode)
{
    int tmp[6];
    tGameObjectMain* ultraBak;
    int viewIdx;

    ultraBak = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    viewIdx = AI_GetIntFromStack();
    tmp[0] = (int)AI_EvalFunc_VIEWAssignObject_C(ultraBak, viewIdx);

    AI_PutObjectToStack(tmp, AI_gaw_EnumLink[40]);

    return aiNode + 1;
}

tAINode* AI_EvalFunc_VIEWCreate(tAINode* aiNode)
{
    int viewIdx;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_GetIntFromStack(); // TODO ez jó így?
    AI_GetIntFromStack();
    AI_GetIntFromStack();
    AI_GetIntFromStack();
    AI_GetIntFromStack();

    if (WOR_gpst_CurrentWorld)
    {
        viewIdx = WOR_View_Create(WOR_gpst_CurrentWorld);
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData = WOR_gpst_CurrentWorld->viewPtr[0].displayData;

        AI_PutIntToStack(viewIdx);
    }
    return aiNode + 1;
}

tAINode* AI_EvalFunc_VIEWDestroy(tAINode* a1)
{
    int viewIdx;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    viewIdx = AI_GetIntFromStack();

    if (WOR_gpst_CurrentWorld)
    {
        WOR_View_Close(&WOR_gpst_CurrentWorld->viewPtr[viewIdx]); // nem kell ez ide
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].flags &= (uint32_t)~1;
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewFunctionPtr = NULL;
    }
    return a1 + 1;
}

struct sGameObjectMain* AI_EvalFunc_VIEWGetObject_C(int viewIdx)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    return WOR_gpst_CurrentWorld->viewPtr[viewIdx].assignedGao;
}

tAINode* AI_EvalFunc_VIEWGetObject(tAINode* aiNode)
{
    int viewIdx;
    int tmp[5];

    viewIdx = AI_GetIntFromStack();
    tmp[0] = (int)AI_EvalFunc_VIEWGetObject_C(viewIdx);

    AI_PutObjectToStack(tmp, AI_gaw_EnumLink[40]);

    return aiNode + 1;
}

void AI_EvalFunc_VIEWFocaleSet_C(int viewIdx, float fov)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (WOR_gpst_CurrentWorld && WOR_gpst_CurrentWorld->viewPtr[viewIdx].focale != fov)
    {
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].focale = fov;
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewFunctionPtr(&WOR_gpst_CurrentWorld->viewPtr[viewIdx]);

        if (WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData)
        {
            WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.cam.focale = fov;
            
            CAM_AssignCameraToDevice(&WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ddcs2, &WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct, 
                WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->winWidth, WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->winHeight);
            
            gpst_DX_stDD = WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData;

            WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->SetProjectionMatrix(&WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.cam);
        }
    }
}

tAINode* AI_EvalFunc_VIEWFocaleSet(tAINode* aiNode)
{
    int viewIdx;
    float fov;

    fov = AI_GetFloatFromStack();
    viewIdx = AI_GetIntFromStack();
    AI_EvalFunc_VIEWFocaleSet_C(viewIdx, fov);

    return aiNode + 1;
}

float AI_EvalFunc_VIEWFocaleGet_C(int viewIdx)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    
    if (WOR_gpst_CurrentWorld)
        return WOR_gpst_CurrentWorld->viewPtr[viewIdx].focale;
    else
        return 0.0f;
}

tAINode* AI_EvalFunc_VIEWFocaleGet(tAINode* aiNode)
{
    int viewIdx;
    float ret;

    viewIdx = AI_GetIntFromStack();
    ret = AI_EvalFunc_VIEWFocaleGet_C(viewIdx);
    
    AI_PutFloatToStack(ret);
    return aiNode + 1;
}

void AI_EvalFunc_VIEWViewportPosSet_C(int viewIdx, Vec3* pos)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (!WOR_gpst_CurrentWorld) return;

    WOR_View_SetViewport(&WOR_gpst_CurrentWorld->viewPtr[viewIdx], pos->x, pos->y,
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortSizeWidth,
        WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortSizeHeight);
}

tAINode* AI_EvalFunc_VIEWViewportPosSet(tAINode* aiNode)
{
    int viewIdx;
    Vec3* pos;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    pos = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    viewIdx = AI_GetIntFromStack();

    AI_EvalFunc_VIEWViewportPosSet_C(viewIdx, pos);
    return aiNode + 1;
}

void AI_EvalFunc_VIEWViewportPosGet_C(int viewIdx, Vec3* pos)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (WOR_gpst_CurrentWorld)
    {
        pos->x = WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortPosX;
        pos->y = WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortPosY;
    }
}

tAINode* AI_EvalFunc_VIEWViewportPosGet(tAINode* aiNode)
{
    int viewIdx;
    Vec3 pos;

    viewIdx = AI_GetIntFromStack();
    
    AI_EvalFunc_VIEWViewportPosGet_C(viewIdx, &pos);
    AI_PutVecToStack(&pos);

    return aiNode + 1;
}

void AI_EvalFunc_VIEWViewportSizeSet_C(int viewIdx, Vec3* size)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    if (!WOR_gpst_CurrentWorld)
        return;

    WOR_View_SetViewport(&WOR_gpst_CurrentWorld->viewPtr[viewIdx], WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortPosX, WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortPosY,
        size->x, size->y);
}

tAINode* AI_EvalFunc_VIEWViewportSizeSet(tAINode* aiNode)
{
    int viewIdx;
    Vec3* size;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    size = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    viewIdx = AI_GetIntFromStack();

    AI_EvalFunc_VIEWViewportSizeSet_C(viewIdx, size);
    return aiNode + 1;
}

void AI_EvalFunc_VIEWViewportSizeGet_C(int viewIdx, Vec3* size)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (WOR_gpst_CurrentWorld)
    {
        size->x = WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortSizeWidth;
        size->y = WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewPortSizeHeight;
    }
}

tAINode* AI_EvalFunc_VIEWViewportSizeGet(tAINode* aiNode)
{
    int viewIdx;
    Vec3 size;

    viewIdx = AI_GetIntFromStack();
    AI_EvalFunc_VIEWViewportSizeGet_C(viewIdx, &size);
    AI_PutVecToStack(&size);
    return aiNode + 1;
}

void AI_EvalFunc_VIEW3dWorldTo2d_C(int viewIdx, Vec3 *vec3d, Vec3 *vec2d)
{
  tDDCamStruct *cam;

  AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
  AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

  if (WOR_gpst_CurrentWorld && WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData)
  {
      if ((WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->flags & 0x800) != 0)
          cam = &WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.camBackup;
      else
          cam = &WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.cam;

      cam->viewMtxPtr_setInGRORender = &cam->cameraMtx;
      
      SOFT_TransformAndProject(vec2d, vec3d, 1, cam);
      
      vec2d->x = (vec2d->x - cam->centerX) / cam->width + 0.5f;
      vec2d->y = (cam->centerY - vec2d->y) / cam->height + 0.5f;
  }
}

tAINode* AI_EvalFunc_VIEW3dWorldTo2d(tAINode* aiNode)
{
    int viewIdx;
    Vec3 vec3d, vec2d;

    AI_GetVecFromStack(&vec3d);
    viewIdx = AI_GetIntFromStack();

    AI_EvalFunc_VIEW3dWorldTo2d_C(viewIdx, &vec3d, &vec2d);
    
    AI_PutVecToStack(&vec2d);
    return aiNode + 1;
}

void AI_EvalFunc_VIEW2dTo3d_C(int viewIdx, Vec3* vec2d, Vec3* vec3d)
{
    tDDCamStruct* cam;
    Vec3 cam2d, cam3D;

    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    cam2d = *vec2d;

    if (!WOR_gpst_CurrentWorld || !WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData) return;

    if ((WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->flags & 0x800) != 0)
        cam = &WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.camBackup;
    else
        cam = &WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.cam;

    cam2d.x = cam->centerX + (cam2d.x - 0.5f) * cam->width;
    cam2d.y = cam->centerY - (cam2d.y - 0.5f) * cam->height;

    CAM_2Dto3DCamera(cam, &cam3D, &cam2d);
    MATH_TransformVertex(vec3d, &cam->objectMtx, &cam3D);
}

tAINode* AI_EvalFunc_VIEW2dTo3d(tAINode* aiNode)
{
    int viewIdx;
    Vec3 vec2d, vec3d;

    AI_GetVecFromStack(&vec2d);
    viewIdx = AI_GetIntFromStack();

    AI_EvalFunc_VIEW2dTo3d_C(viewIdx, &vec2d, &vec3d);
    AI_PutVecToStack(&vec3d);
    
    return aiNode + 1;
}

int AI_EvalFunc_VIEWWidth_C(int viewIdx)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    
    if (!WOR_gpst_CurrentWorld) return 0;

    if (WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData)
        return WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->winWidth;
    else
        return 0;
}

tAINode* AI_EvalFunc_VIEWWidth(tAINode* aiNode)
{
    int viewIdx, width;

    viewIdx = AI_GetIntFromStack();
    width = AI_EvalFunc_VIEWWidth_C(viewIdx);

    AI_PutIntToStack(width);

    return aiNode + 1;
}

int AI_EvalFunc_VIEWHeight_C(int viewIdx)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (!WOR_gpst_CurrentWorld) return 0;

    if (WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData)
        return WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->winHeight;
    else
        return 0;
}

tAINode* AI_EvalFunc_VIEWHeight(tAINode* aiNode)
{
    int viewIdx, height;

    viewIdx = AI_GetIntFromStack();
    height = AI_EvalFunc_VIEWHeight_C(viewIdx);

    AI_PutIntToStack(height);

    return aiNode + 1;
}

BOOL AI_EvalFunc_VIEWCulled_C(struct sGameObjectMain* gao, int viewIdx)
{
    if (!WOR_gpst_CurrentWorld) return FALSE;

    WOR_gpst_CurrentWorld->viewPtr[viewIdx].viewFunctionPtr(&WOR_gpst_CurrentWorld->viewPtr[viewIdx]);

    if (!WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData) return FALSE;

    CAM_SetCameraMatrix(&WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct, &WOR_gpst_CurrentWorld->viewPtr[viewIdx].cameraMtx);
    CAM_Engine_ComputePlans(&WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct);
    OBJ_UpdateCullingVars(&WOR_gpst_CurrentWorld->viewPtr[viewIdx].displayData->ct.cam);

    return OBJ_CullingObject(gao);
}

tAINode* AI_EvalFunc_VIEWCulled(tAINode* aiNode)
{
    int viewIdx, ret;
    tGameObjectMain* ultraBak;

    ultraBak = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    viewIdx = AI_GetIntFromStack();
    ret = AI_EvalFunc_VIEWCulled_C(ultraBak, viewIdx);

    AI_PutIntToStack(ret);

    return aiNode + 1;
}

void AI_EvalFunc_VIEWDrawMaskSet_C(int maskSet, int maskReset)
{
    if (gpst_DX_stDD)
    {
        gpst_DX_stDD->drawMaskAllowedStuff |= maskSet;
        gpst_DX_stDD->drawMaskAllowedStuff &= ~maskReset;
    }
}

tAINode* AI_EvalFunc_VIEWDrawMaskSet(tAINode* aiNode)
{
    int maskSet, maskReset;

    maskReset = AI_GetIntFromStack();
    maskSet = AI_GetIntFromStack();

    AI_EvalFunc_VIEWDrawMaskSet_C(maskSet, maskReset);

    return aiNode + 1;
}

void AI_EvalFunc_VIEWForceNoFog_C(int onOff)
{
    if (gpst_DX_stDD)
    {
        if (onOff)
            gpst_DX_stDD->fog.flags = gpst_DX_stDD->fog.flags | 4;
        else
            gpst_DX_stDD->fog.flags = gpst_DX_stDD->fog.flags & ~4;
    }
}

tAINode* AI_EvalFunc_VIEWForceNoFog(tAINode* aiNode)
{
    int onOff = AI_GetIntFromStack();
    
    AI_EvalFunc_VIEWForceNoFog_C(onOff);

    return aiNode + 1;
}