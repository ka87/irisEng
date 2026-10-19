#include "../memory.h"
#include "../loading.h"
#include "../cameraengine.h"
#include "camera.h"

tGEOCAM* CAM_pst_Create()
{
    tGEOCAM* cam = (tGEOCAM*)MEM_p_Alloc(sizeof(tGEOCAM));

    memset(cam, 0, sizeof(tGEOCAM));
    GRO_Struct_Init((tGroGeneric*)cam, 6);
    
    return cam;
}

struct sGroGeneric* CAM_p_CreateFromBuffer(struct sGroGeneric* gro, char** readptr, struct sWorld* world)
{
    tGEOCAM* cam = CAM_pst_Create();

    cam->flags = LOA_ReadLong(readptr, 0, 1);
    cam->nearPlane = LOA_ReadFloat(readptr, 0, 1);
    cam->farPlane = LOA_ReadFloat(readptr, 0, 1);
    cam->fov = LOA_ReadFloat(readptr, 0, 1);
    return (tGroGeneric*)cam;
}

int CAM_l_HasSomethingToRender(struct sGameObjectVisualObject* gaovo, struct sGroGeneric** gro)
{
    return gpst_DX_stDD->presentationFlags & 0x4;
}

void CAM_Free(struct sGroGeneric* gro)
{
    if (gro && gro->intf->id == 6 && gro->usageCount <= 0)
    {
        LOA_DeleteAddress((int)gro);
        MEM_Free(gro);
    }
}

void CAM_Init()
{
    GRO_gast_Interface[6].CreateFromBuffer = CAM_p_CreateFromBuffer;
    GRO_gast_Interface[6].Destroy = CAM_Free;
    GRO_gast_Interface[6].HasSomethingToRender = CAM_l_HasSomethingToRender;
    CAM_gst_IsoNoClip.flags = 0x2;
    CAM_gst_IsoNoClip.nearPlane = 1.0f;
    CAM_gst_IsoNoClip.farPlane = 0.0f;
    CAM_gst_IsoNoClip.YoverX = 1.0f;
    CAM_gst_IsoNoClip.isoFactor = 80.0f;
}