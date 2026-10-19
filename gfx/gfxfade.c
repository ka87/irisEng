#include "../time.h"
#include "../memory.h"
#include "gfxfade.h"

int GFX_i_Fade_Render(struct sGfxFadeData* a1)
{
    float* v2;
    int v3;
    double v4;
    int result;
    unsigned int v6;
    int v7;
    double v8;
    Vec3* v9;
    int v10;
    double v11;
    double v12;
    double v13;
    double v14;
    double v15;
    double v16;
    Vec3* v17;
    int v18;
    float v19;
    float v20;

    v19 = a1->elapsedTime;
    v2 = a1->factors;
    v3 = 0;
    if (v19 <= (double)a1->factors[0])
    {
    LABEL_4:
        switch (v3)
        {
        case 0:
        case 4:
            v19 = 0.0;
            break;
        case 1:
            v19 = v19 / a1->factors[1];
            break;
        case 2:
            v19 = 1.0;
            break;
        case 3:
            v19 = (float)(1.0 - v19 / a1->factors[3]);
            break;
        default:
            break;
        }
        v6 = LIGHT_ul_Interpol2Colors(a1->color1, a1->color2, v19);
        a1->elapsedTime = TIM_gf_dt + a1->elapsedTime;
        v7 = v6;
        GFX_NeedGeom(4u, 4u, 2u, 1);
        GFX_gpst_Geo->UVs->u = 0.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 0.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;
        v8 = (double)gpst_DX_stDD->ct.cam.viewPortRealPosX;
        v9 = GFX_gpst_Geo->vertices;
        v10 = 4;
        v11 = v8 + gpst_DX_stDD->ct.cam.width;
        v12 = (double)gpst_DX_stDD->ct.cam.viewPortRealPosY;
        v20 = (float)(v12 + gpst_DX_stDD->ct.cam.height);
        v13 = a1->f;
        v9->x = (float)v8;
        v9->y = (float)v12;
        v9->z = (float)v13;
        v14 = a1->f;
        v9[1].x = (float)v11;
        v9[1].y = (float)v12;
        v9[1].z = (float)v14;
        v15 = a1->f;
        v9[2].y = (float)v20;
        v9[2].x = (float)v11;
        v9[2].z = (float)v15;
        v16 = a1->f;
        v9[3].y = v20;
        v17 = v9;
        v9[3].x = (float)v8;
        v9[3].z = (float)v16;
        do
        {
            CAM_AssignCameraToDevice(&gpst_DX_stDD->ddcs2, &gpst_DX_stDD->ct, gpst_DX_stDD->winWidth, gpst_DX_stDD->winHeight);
            gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
            CAM_2Dto3D(&gpst_DX_stDD->ct.cam, v17, v17);
            ++v17;
            --v10;
        } while (v10);
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v1 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v2 = 1;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->v3 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv1 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv2 = 1;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv3 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v1 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v2 = 3;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v3 = 0;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv1 = 2;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv2 = 3;
        GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv3 = 0;
        GFX_gpst_Geo->vertexColors[1] = v7;
        GFX_gpst_Geo->vertexColors[2] = v7;
        GFX_gpst_Geo->vertexColors[3] = v7;
        GFX_gpst_Geo->vertexColors[4] = v7;
        v18 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v18 & ~0x9000900u;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v18;
        result = 1;
    }
    else
    {
        while (1)
        {
            ++v3;
            v4 = v19 - *v2++;
            v19 = (float)v4;
            if (v3 == 5)
                break;
            if (v19 <= (double)*v2)
                goto LABEL_4;
        }
        result = 0;
    }
    return result;
}

struct sGfxFadeData* GFX_Fade_Create()
{
    tGfxFadeData* v0;

    v0 = (tGfxFadeData*)MEM_p_Alloc(sizeof(tGfxFadeData));
    memset(v0, 0, sizeof(tGfxFadeData));
    v0->f = 0.055f;
    return v0;
}