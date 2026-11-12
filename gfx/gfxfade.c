#include "../time.h"
#include "../memory.h"
#include "gfxfade.h"

int GFX_i_Fade_Render(struct sGfxFadeData* fade)
{
    Vec3* verts;
    float left;
    float top;
    float right;
    float bottom;
    float blend;
    unsigned int color;
    int stage;
    int i;

    blend = fade->elapsedTime;
    for (stage = 0; stage < 5; stage++)
    {
        if (blend <= (double)fade->factors[stage])
            break;
        blend = (float)(blend - fade->factors[stage]);
    }
    if (stage == 5)
        return 0;

    switch (stage)
    {
    case 0:
    case 4:
        blend = 0.0;
        break;
    case 1:
        blend = blend / fade->factors[1];
        break;
    case 2:
        blend = 1.0;
        break;
    case 3:
        blend = (float)(1.0 - blend / fade->factors[3]);
        break;
    default:
        break;
    }

    color = LIGHT_ul_Interpol2Colors(fade->color1, fade->color2, blend);
    fade->elapsedTime = TIM_gf_dt + fade->elapsedTime;

    GFX_NeedGeom(4u, 4u, 2u, 1);
    GFX_SetQuadUVs();

    left = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
    top = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
    right = (float)(left + gpst_DX_stDD->ct.cam.width);
    bottom = (float)(top + gpst_DX_stDD->ct.cam.height);

    verts = GFX_gpst_Geo->vertices;
    verts[0].x = left;
    verts[0].y = top;
    verts[0].z = fade->f;
    verts[1].x = right;
    verts[1].y = top;
    verts[1].z = fade->f;
    verts[2].x = right;
    verts[2].y = bottom;
    verts[2].z = fade->f;
    verts[3].x = left;
    verts[3].y = bottom;
    verts[3].z = fade->f;

    for (i = 0; i < 4; i++)
    {
        CAM_AssignCameraToDevice(&gpst_DX_stDD->ddcs2, &gpst_DX_stDD->ct,
                                 gpst_DX_stDD->winWidth, gpst_DX_stDD->winHeight);
        gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
        CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &verts[i], &verts[i]);
    }

    GFX_SetQuadIndices();

    GFX_gpst_Geo->vertexColors[1] = color;
    GFX_gpst_Geo->vertexColors[2] = color;
    GFX_gpst_Geo->vertexColors[3] = color;
    GFX_gpst_Geo->vertexColors[4] = color;
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x9000900u);
    return 1;
}

struct sGfxFadeData* GFX_Fade_Create(void)
{
    tGfxFadeData* fade;

    fade = (tGfxFadeData*)MEM_p_Alloc(sizeof(tGfxFadeData));
    memset(fade, 0, sizeof(tGfxFadeData));
    fade->f = 0.055f;
    return fade;
}
