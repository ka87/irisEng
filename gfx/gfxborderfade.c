#include "../time.h"
#include "../memory.h"
#include "gfxborderfade.h"

float bf_depth = 0.1f;
uint16_t ai_Index[] = { 0, 0, 8, 0, 0x0B, 0, 3, 0, 8, 0, 9, 0, 5, 0, 4, 0, 9, 0, 1, 0, 2, 0, 0x0A, 0, 7, 0, 6, 0, 0x0A, 0, 0x0B, 0 };
uint16_t ai_IndexLine[] = { 0, 0, 1, 0, 3, 0, 1, 0, 2, 0, 3, 0, 0, 0, 3, 0, 5, 0, 3, 0, 4, 0, 5, 0 };


int GFX_i_BorderFade_Render(struct sGfxBorderFadeData* bf)
{
    int i, j;   
    float timeRatio;
    int drawMaskBackup;    
    float f1, f2, f3, f4;
    float camEndHeight;
    float camEndWidth; 
    BOOL timeRatioHasBeenSet = FALSE;
    
    f1 = bf->f1;
    f2 = bf->f2;
    f3 = bf->f3;
    f4 = bf->f4;
    camEndWidth = (float)(gpst_DX_stDD->ct.cam.viewPortRealPosX + gpst_DX_stDD->ct.cam.width);
    camEndHeight = (float)(gpst_DX_stDD->ct.cam.viewPortRealPosY + gpst_DX_stDD->ct.cam.height);

    if ((bf->flags & 0x3) != 0)
    {
        if ((bf->flags & 0x2) != 0)
        {
            f1 = (float)((camEndWidth - gpst_DX_stDD->ct.cam.viewPortRealPosX) * bf->f1 + gpst_DX_stDD->ct.cam.viewPortRealPosX);
            f3 = (float)((camEndWidth - gpst_DX_stDD->ct.cam.viewPortRealPosX) * bf->f3 + gpst_DX_stDD->ct.cam.viewPortRealPosX);
            f2 = (float)(camEndHeight - gpst_DX_stDD->ct.cam.viewPortRealPosY * bf->f2 + gpst_DX_stDD->ct.cam.viewPortRealPosY);
            f4 = (float)(camEndHeight - gpst_DX_stDD->ct.cam.viewPortRealPosY * bf->f4 + gpst_DX_stDD->ct.cam.viewPortRealPosY);
        }
        if ((bf->flags & 0x1) != 0)
        {
            f3 = (float)((camEndWidth + gpst_DX_stDD->ct.cam.viewPortRealPosX + f1) * 0.5);
            f1 = (float)((camEndWidth + gpst_DX_stDD->ct.cam.viewPortRealPosX - f1) * 0.5);
            f4 = (float)((camEndHeight + gpst_DX_stDD->ct.cam.viewPortRealPosY + f2) * 0.5);
            f2 = (float)((camEndHeight + gpst_DX_stDD->ct.cam.viewPortRealPosY - f2) * 0.5);
        }
        bf->f1 = f1;
        bf->f3 = f3;
        bf->f2 = f2;
        bf->f4 = f4;
        bf->flags &= ~0x3;
    }
    if ((bf->flags & 0x200) != 0)
    {
        bf->elapsedTime = TIM_gf_dt + bf->elapsedTime;
        
        if (bf->totalTime <= bf->elapsedTime)
            return 0;

        timeRatio = 1.0f - bf->elapsedTime / bf->totalTime;
        timeRatioHasBeenSet = TRUE;
    } else if ((bf->flags & 0x100) != 0)
    {
        bf->elapsedTime = TIM_gf_dt + bf->elapsedTime;
     
        if (bf->elapsedTime < bf->startTime)
            return 1;
        
        if (bf->finishTime + bf->startTime > bf->elapsedTime)
        {
            timeRatio = (bf->elapsedTime - bf->startTime) / bf->finishTime;
            timeRatioHasBeenSet = TRUE;        
        }
        else
        {
            bf->flags -= 0x100;
        }
    }
    bf->elapsedTime = 0.0f;

    if (timeRatioHasBeenSet && timeRatio != 1.0f)
    {
        f1 = (float)((f1 - (float)gpst_DX_stDD->ct.cam.viewPortRealPosX) * timeRatio + (float)gpst_DX_stDD->ct.cam.viewPortRealPosX);
        f2 = (float)((f2 - (float)gpst_DX_stDD->ct.cam.viewPortRealPosY) * timeRatio + (float)gpst_DX_stDD->ct.cam.viewPortRealPosY);
        f3 = (float)((f3 - camEndWidth) * timeRatio + camEndWidth);
        f4 = (float)((f4 - camEndHeight) * timeRatio + camEndHeight);
    }
    
    GFX_NeedGeom(12, 4, 8, 1);
    
    GFX_gpst_Geo->UVs[0].u = 0.0f;
    GFX_gpst_Geo->UVs[0].v = 0.0f;
    GFX_gpst_Geo->UVs[1].u = 1.0f;
    GFX_gpst_Geo->UVs[1].v = 0.0f;
    GFX_gpst_Geo->UVs[2].u = 1.0f;
    GFX_gpst_Geo->UVs[2].v = 1.0f;
    GFX_gpst_Geo->UVs[3].u = 0.0f;
    GFX_gpst_Geo->UVs[3].v = 1.0f;

    GFX_gpst_Geo->vertices[0].x = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
    GFX_gpst_Geo->vertices[0].y = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
    GFX_gpst_Geo->vertices[0].z = bf_depth;
    GFX_gpst_Geo->vertices[1].x = camEndWidth;
    GFX_gpst_Geo->vertices[1].y = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
    GFX_gpst_Geo->vertices[1].z = bf_depth;
    GFX_gpst_Geo->vertices[2].x = camEndWidth;
    GFX_gpst_Geo->vertices[2].y = camEndHeight;
    GFX_gpst_Geo->vertices[2].z = bf_depth;
    GFX_gpst_Geo->vertices[3].x = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
    GFX_gpst_Geo->vertices[3].y = camEndHeight;
    GFX_gpst_Geo->vertices[3].z = bf_depth;
    GFX_gpst_Geo->vertices[4].x = f1;
    GFX_gpst_Geo->vertices[4].y = f2;
    GFX_gpst_Geo->vertices[4].z = bf_depth;
    GFX_gpst_Geo->vertices[5].x = f3;
    GFX_gpst_Geo->vertices[5].y = f2;
    GFX_gpst_Geo->vertices[5].z = bf_depth;
    GFX_gpst_Geo->vertices[6].x = f3;
    GFX_gpst_Geo->vertices[6].y = f4;
    GFX_gpst_Geo->vertices[6].z = bf_depth;
    GFX_gpst_Geo->vertices[7].x = f1;
    GFX_gpst_Geo->vertices[7].y = f4;
    GFX_gpst_Geo->vertices[7].z = bf_depth;
    GFX_gpst_Geo->vertices[8].x = f1;
    GFX_gpst_Geo->vertices[8].y = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
    GFX_gpst_Geo->vertices[8].z = bf_depth;
    GFX_gpst_Geo->vertices[9].x = f3;
    GFX_gpst_Geo->vertices[9].y = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
    GFX_gpst_Geo->vertices[9].z = bf_depth;
    GFX_gpst_Geo->vertices[10].x = f3;
    GFX_gpst_Geo->vertices[10].y = camEndHeight;
    GFX_gpst_Geo->vertices[10].z = bf_depth;
    GFX_gpst_Geo->vertices[11].x = f1;
    GFX_gpst_Geo->vertices[11].y = camEndHeight;
    GFX_gpst_Geo->vertices[11].z = bf_depth;

    for (i = 1; i <= 12; i++)
    {
        CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &GFX_gpst_Geo->vertices[i - 1], &GFX_gpst_Geo->vertices[i - 1]);
        GFX_gpst_Geo->vertexColors[i] = bf->color2;
    }

    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v1 = ai_Index[0];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v2 = ai_Index[2];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].v3 = ai_Index[4];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv1 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv2 = 1;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[0].uv3 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v1 = ai_Index[4];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v2 = ai_Index[6];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].v3 = ai_Index[0];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv1 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv2 = 3;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[1].uv3 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].v1 = ai_Index[8];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].v2 = ai_Index[10];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].v3 = ai_Index[12];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].uv1 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].uv2 = 1;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[2].uv3 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].v1 = ai_Index[12];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].v2 = ai_Index[14];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].v3 = ai_Index[8];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].uv1 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].uv2 = 3;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[3].uv3 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[4].v1 = ai_Index[16];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[4].v2 = ai_Index[18];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[4].v3 = ai_Index[20];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[4].uv1 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[4].uv2 = 1;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[4].uv3 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[5].v1 = ai_Index[20];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[5].v2 = ai_Index[22];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[5].v3 = ai_Index[16];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[5].uv1 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[5].uv2 = 3;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[5].uv3 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[6].v1 = ai_Index[24];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[6].v2 = ai_Index[26];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[6].v3 = ai_Index[28];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[6].uv1 = 0;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[6].uv2 = 1;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[6].uv3 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[7].v1 = ai_Index[28];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[7].v2 = ai_Index[30];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[7].v3 = ai_Index[24];
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[7].uv1 = 2;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[7].uv2 = 3;
    GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[7].uv3 = 0;
    
    drawMaskBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup & ~0x8000900;
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup;

    if ((bf->flags & 0x10) != 0)
    {
        GFX_NeedGeom(24, 4, 16, 1);

        GFX_gpst_Geo->vertices[0].x = f1 + bf->vertUnk1;
        GFX_gpst_Geo->vertices[0].y = f2 + bf->vertUnk2;
        GFX_gpst_Geo->vertices[0].z = bf_depth;        
        GFX_gpst_Geo->vertices[1].x = f1 + bf->vertUnk1 + bf->vertUnk3;
        GFX_gpst_Geo->vertices[1].y = f2 + bf->vertUnk2;
        GFX_gpst_Geo->vertices[1].z = bf_depth;
        GFX_gpst_Geo->vertices[2].x = f1 + bf->vertUnk1 + bf->vertUnk3;
        GFX_gpst_Geo->vertices[2].y = f2 + bf->vertUnk2 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[2].z = bf_depth;
        GFX_gpst_Geo->vertices[3].x = f1 + bf->vertUnk1 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[3].y = f2 + bf->vertUnk2 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[3].z = bf_depth;
        GFX_gpst_Geo->vertices[4].x = f1 + bf->vertUnk1 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[4].y = f2 + bf->vertUnk2 + bf->vertUnk4;
        GFX_gpst_Geo->vertices[4].z = bf_depth;
        GFX_gpst_Geo->vertices[5].x = f1 + bf->vertUnk1;
        GFX_gpst_Geo->vertices[5].y = f2 + bf->vertUnk2 + bf->vertUnk4;
        GFX_gpst_Geo->vertices[5].z = bf_depth;
        GFX_gpst_Geo->vertices[6].x = f3 - bf->vertUnk1;
        GFX_gpst_Geo->vertices[6].y = f2 + bf->vertUnk2;
        GFX_gpst_Geo->vertices[6].z = bf_depth;
        GFX_gpst_Geo->vertices[7].x = f3 - bf->vertUnk1 - bf->vertUnk3;
        GFX_gpst_Geo->vertices[7].y = f2 + bf->vertUnk2;
        GFX_gpst_Geo->vertices[7].z = bf_depth;
        GFX_gpst_Geo->vertices[8].x = f3 - bf->vertUnk1 - bf->vertUnk3;
        GFX_gpst_Geo->vertices[8].y = f2 + bf->vertUnk2 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[8].z = bf_depth;
        GFX_gpst_Geo->vertices[9].x = f3 - bf->vertUnk1 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[9].y = f2 + bf->vertUnk2 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[9].z = bf_depth;
        GFX_gpst_Geo->vertices[10].x = f3 - bf->vertUnk1 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[10].y = f2 + bf->vertUnk2 + bf->vertUnk4;
        GFX_gpst_Geo->vertices[10].z = bf_depth;
        GFX_gpst_Geo->vertices[11].x = f3 - bf->vertUnk1;
        GFX_gpst_Geo->vertices[11].y = f2 + bf->vertUnk2 + bf->vertUnk4;
        GFX_gpst_Geo->vertices[11].z = bf_depth;
        GFX_gpst_Geo->vertices[12].x = f3 - bf->vertUnk1;
        GFX_gpst_Geo->vertices[12].y = f4 - bf->vertUnk2;
        GFX_gpst_Geo->vertices[12].z = bf_depth;
        GFX_gpst_Geo->vertices[13].x = f3 - bf->vertUnk1 - bf->vertUnk3;
        GFX_gpst_Geo->vertices[13].y = f4 - bf->vertUnk2;
        GFX_gpst_Geo->vertices[13].z = bf_depth;
        GFX_gpst_Geo->vertices[14].x = f3 - bf->vertUnk1 - bf->vertUnk3;
        GFX_gpst_Geo->vertices[14].y = f4 - bf->vertUnk2 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[14].z = bf_depth;
        GFX_gpst_Geo->vertices[15].x = f3 - bf->vertUnk1 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[15].y = f4 - bf->vertUnk2 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[15].z = bf_depth;
        GFX_gpst_Geo->vertices[16].x = f3 - bf->vertUnk1 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[16].y = f4 - bf->vertUnk2 - bf->vertUnk4;
        GFX_gpst_Geo->vertices[16].z = bf_depth;
        GFX_gpst_Geo->vertices[17].x = f3 - bf->vertUnk1;
        GFX_gpst_Geo->vertices[17].y = f4 - bf->vertUnk2 - bf->vertUnk4;
        GFX_gpst_Geo->vertices[17].z = bf_depth;
        GFX_gpst_Geo->vertices[18].x = f1 + bf->vertUnk1;
        GFX_gpst_Geo->vertices[18].y = f4 - bf->vertUnk2;
        GFX_gpst_Geo->vertices[18].z = bf_depth;
        GFX_gpst_Geo->vertices[19].x = f1 + bf->vertUnk1 + bf->vertUnk3;
        GFX_gpst_Geo->vertices[19].y = f4 - bf->vertUnk2;
        GFX_gpst_Geo->vertices[19].z = bf_depth;
        GFX_gpst_Geo->vertices[20].x = f1 + bf->vertUnk1 + bf->vertUnk3;
        GFX_gpst_Geo->vertices[20].y = f4 - bf->vertUnk2 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[20].z = bf_depth;
        GFX_gpst_Geo->vertices[21].x = f1 + bf->vertUnk1 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[21].y = f4 - bf->vertUnk2 - bf->vertUnk5;
        GFX_gpst_Geo->vertices[21].z = bf_depth;
        GFX_gpst_Geo->vertices[22].x = f1 + bf->vertUnk1 + bf->vertUnk5;
        GFX_gpst_Geo->vertices[22].y = f4 - bf->vertUnk2 - bf->vertUnk4;
        GFX_gpst_Geo->vertices[22].z = bf_depth;
        GFX_gpst_Geo->vertices[23].x = f1 + bf->vertUnk1;
        GFX_gpst_Geo->vertices[23].y = f4 - bf->vertUnk2 - bf->vertUnk4;
        GFX_gpst_Geo->vertices[23].z = bf_depth;

        for (i = 1; i <= 24; i++)
        {
            CAM_2Dto3D(&gpst_DX_stDD->ct.cam, &GFX_gpst_Geo->vertices[i - 1], &GFX_gpst_Geo->vertices[i - 1]);
            GFX_gpst_Geo->vertexColors[i] = bf->color;
        }
       
        for (i = 0, j = 0; i < 4; i++, j += 6)
        {
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 0].v1 = j + ai_IndexLine[0];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 0].v2 = j + ai_IndexLine[2];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 0].v3 = j + ai_IndexLine[4];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].v1 = j + ai_IndexLine[6];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].v2 = j + ai_IndexLine[8];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 1].v3 = j + ai_IndexLine[10];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 2].v1 = j + ai_IndexLine[12];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 2].v2 = j + ai_IndexLine[14];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 2].v3 = j + ai_IndexLine[16];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 3].v1 = j + ai_IndexLine[18];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 3].v2 = j + ai_IndexLine[20];
            GFX_gpst_Geo->triangleChunks[0].triangleChunkElems[j + 3].v3 = j + ai_IndexLine[22];
        }

        drawMaskBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup & ~0x8000900;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup;
    }
    return 1;
}

struct sGfxBorderFadeData* GFX_BorderFade_Create()
{
    tGfxBorderFadeData* bf = (tGfxBorderFadeData*)MEM_p_Alloc(sizeof(tGfxBorderFadeData));

    memset(bf, 0, sizeof(tGfxBorderFadeData));

    return bf;
}