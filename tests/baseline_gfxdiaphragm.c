#include "../memory.h"
#include "../time.h"
#include "gfxdiaphragm.h"

float depth_362 = 0.055f;

struct sGfxDiaphragmData* GFX_Diaphragm_Create()
{
    tGfxDiaphragmData* v0;

    v0 = (tGfxDiaphragmData*)MEM_p_Alloc(sizeof(tGfxDiaphragmData));
    memset(v0, 0, sizeof(tGfxDiaphragmData));
    v0->v1.x = 0.5f;
    v0->v1.y = 0.5f;
    v0->v1.z = 0.0f;
    return v0;
}

int GFX_i_Diaphragm_Render(tGfxDiaphragmData* a1)
{
    float v2;
    float* v3;
    int v4;
    int ret;
    int v12;
    float v14;
    Vec3* v19;
    tGeoTriangleChunkElem* v20;
    float v21;
    float v22;
    float v23;
    Vec3* v24;
    float v25;
    tGeoTriangleChunkElem* v68;
    Vec3* v69;
    Vec3* v70;
    int v71;
    int v72;
    int v73;
    tGeoTriangleChunkElem* v74;
    int v75;
    Vec3* v76;
    int v77;
    int v78;
    int v79;
    tGeoTriangleChunkElem* v80;
    int v81;
    Vec3* v82;
    int v83;
    int v84;
    tGeoTriangleChunkElem* v85;
    int v86;
    Vec3* v87;
    int v88;
    int v89;
    tGeoTriangleChunkElem* v90;
    int v91;
    Vec3* v92;
    int v93;
    int v94;
    tGeoTriangleChunkElem* v95;
    int v96;
    Vec3* v97;
    int v98;
    int v99;
    tGeoTriangleChunkElem* v100;
    int v101;
    Vec3* v102;
    int v103;
    int v104;
    tGeoTriangleChunkElem* v105;
    int v106;
    Vec3* v107;
    int v108; 
    int v109; 
    int v110; 
    Vec3* v111;
    int renderingFlagsBackup;
    float v116;
    float v118;
    int v136;
    float v137; 
    float v140; 
    int v149;
    int v150;
    int v151;
    int v152;
    int v153;
    int v154;
    int v155;

    v2 = TIM_gf_dt + a1->elapsedTime; 
    v3 = a1->f01;
    v4 = 0;
    a1->elapsedTime = v2;
    v140 = v2;
    if (v2 < *v3)
    {
    LABEL_5:
        switch (v4)
        {
        case 0:
            v2 = 0.0;
            goto LABEL_11;
        case 1:
            v2 = v2 / a1->f01[1];
            goto LABEL_11;
        case 2:
            v2 = 1.0;
            goto LABEL_11;
        case 3:
            v2 = 1.0f - v2 / a1->f01[3];
        LABEL_11:
            v140 = v2;
            break;
        default:
            break;
        }

        if (v2 < 0.99000001f)
            GFX_NeedGeom(12, 4, 12, 1);
        else
            GFX_NeedGeom(4, 4, 2, 1);
        v12 = 0;
        GFX_gpst_Geo->UVs[0].u = 0.0;
        GFX_gpst_Geo->UVs[0].v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 0.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;

        v137 = (gpst_DX_stDD->ct.cam.viewPortRealPosX + gpst_DX_stDD->ct.cam.width + gpst_DX_stDD->ct.cam.viewPortRealPosX) * a1->v1.x;
        v14 = (gpst_DX_stDD->ct.cam.viewPortRealPosY + gpst_DX_stDD->ct.cam.height + gpst_DX_stDD->ct.cam.viewPortRealPosY) * a1->v1.y;

        GFX_gpst_Geo->vertices[0].x = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
        GFX_gpst_Geo->vertices[0].y = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
        GFX_gpst_Geo->vertices[0].z = depth_362;
        GFX_gpst_Geo->vertices[1].x = gpst_DX_stDD->ct.cam.viewPortRealPosX + gpst_DX_stDD->ct.cam.width;
        GFX_gpst_Geo->vertices[1].y = (float)gpst_DX_stDD->ct.cam.viewPortRealPosY;
        GFX_gpst_Geo->vertices[1].z = depth_362;
        GFX_gpst_Geo->vertices[2].x = (float)gpst_DX_stDD->ct.cam.viewPortRealPosX;
        GFX_gpst_Geo->vertices[2].y = gpst_DX_stDD->ct.cam.viewPortRealPosY + gpst_DX_stDD->ct.cam.height;
        GFX_gpst_Geo->vertices[2].z = depth_362;
        GFX_gpst_Geo->vertices[3].x = gpst_DX_stDD->ct.cam.viewPortRealPosX + gpst_DX_stDD->ct.cam.width;
        GFX_gpst_Geo->vertices[3].y = gpst_DX_stDD->ct.cam.viewPortRealPosY + gpst_DX_stDD->ct.cam.height;
        GFX_gpst_Geo->vertices[3].z = depth_362;
        v19 = GFX_gpst_Geo->vertices;
        do
        {
            CAM_2Dto3D(&gpst_DX_stDD->ct.cam, v19, v19);
            GFX_gpst_Geo->vertexColors[++v12] = a1->color;
            ++v19;
        } while (v12 < 4);
        if (v140 < 0.99000001f)
        {
            v21 = v137 - gpst_DX_stDD->ct.cam.viewPortRealPosX;
            v116 = gpst_DX_stDD->ct.cam.viewPortRealPosX + gpst_DX_stDD->ct.cam.width - v137;
            if (v21 < v116)
                v21 = v116;
            v22 = v14 - gpst_DX_stDD->ct.cam.viewPortRealPosY;
            v118 = gpst_DX_stDD->ct.cam.viewPortRealPosY + gpst_DX_stDD->ct.cam.height - v14;
            if (v22 < v118)
                v22 = v118;

            v23 = (1.0f - v140) * MATH_GetSqrt(v22 * v22 + v21 * v21);

            v24 = GFX_gpst_Geo->vertices + 4;
            v25 = v140 * 3.1415927f + 0.39269909f;

            v24[0].x = (float)(MATH_GetCos((float)v25) * v23);
            v24[0].z = depth_362;
            v24[0].y = (float)(MATH_GetSin((float)v25) * v23);
            v24[1].x = (float)(MATH_GetCos((float)(v25 + 0.78539819)) * v23);
            v24[1].z = depth_362;
            v24[1].y = (float)(MATH_GetSin((float)(v25 + 0.78539819)) * v23);
            v24[2].x = (float)(MATH_GetCos((float)(v25 + 1.5707964)) * v23);
            v24[2].z = depth_362;
            v24[2].y = (float)(MATH_GetSin((float)(v25 + 1.5707964)) * v23);
            v24[3].x = (float)(MATH_GetCos((float)(v25 + 2.3561945)) * v23);
            v24[3].z = depth_362;
            v24[3].y = (float)(MATH_GetSin((float)(v25 + 2.3561945)) * v23);
            v24[4].x = (float)(MATH_GetCos((float)(v25 + 3.1415927)) * v23);
            v24[4].z = depth_362;
            v24[4].y = (float)(MATH_GetSin((float)(v25 + 3.1415927)) * v23);
            v24[5].x = (float)(MATH_GetCos((float)(v25 + 3.926991)) * v23);
            v24[5].z = depth_362;
            v24[5].y = (float)(MATH_GetSin((float)(v25 + 3.926991)) * v23);
            v24[6].x = (float)(MATH_GetCos((float)(v25 + 4.712389)) * v23);
            v24[6].z = depth_362;
            v24[6].y = (float)(MATH_GetSin((float)(v25 + 4.712389)) * v23);
            v24[7].x = (float)(MATH_GetCos((float)(v25 + 5.4977875)) * v23);
            v24[7].z = depth_362;
            v24[7].y = (float)(MATH_GetSin((float)(v25 + 5.4977875)) * v23);
            v68 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
            v69 = GFX_gpst_Geo->vertices + 11;
            v70 = GFX_gpst_Geo->vertices + 4;
            v136 = 4;
            while (1)
            {
                if (v69->y <= 0.0)
                    v71 = 0;
                else
                    v71 = 2;
                v149 = (v69->x > 0.0) | v71;
                if (v70->y <= 0.0)
                    v72 = 0;
                else
                    v72 = 2;
                v73 = (v70->x > 0.0) | v72;
                if (v149 != v73)
                {
                    v68->v1 = v69 - GFX_gpst_Geo->vertices;
                    v68->v2 = v149;
                    v68->v3 = v73;
                    v68->uv1 = 0;
                    v68->uv2 = 1;
                    v68->uv3 = 2;
                    ++v68;
                }
                v68->v1 = v69 - GFX_gpst_Geo->vertices;
                v68->v2 = v73;
                v68->v3 = v70 - GFX_gpst_Geo->vertices;
                v68->uv1 = 0;
                v68->uv2 = 1;
                v68->uv3 = 2;
                v74 = v68 + 1;
                if (v70->y <= 0.0)
                    v75 = 0;
                else
                    v75 = 2;
                v76 = v70 + 1;
                v77 = (v70->x > 0.0) | v75;
                if (v70[1].y <= 0.0)
                    v78 = 0;
                else
                    v78 = 2;
                v79 = (v70[1].x > 0.0) | v78;
                if (v77 != v79)
                {
                    v74->v1 = v70 - GFX_gpst_Geo->vertices;
                    v74->v2 = v77;
                    v74->v3 = v79;
                    v74->uv1 = 0;
                    v74->uv2 = 1;
                    v74->uv3 = 2;
                    ++v74;
                }
                v74->v1 = v70 - GFX_gpst_Geo->vertices;
                v74->v2 = v79;
                v74->v3 = v70 - GFX_gpst_Geo->vertices + 1;
                v74->uv1 = 0;
                v74->uv2 = 1;
                v74->uv3 = 2;
                v80 = v74 + 1;
                if (v70[1].y <= 0.0)
                    v81 = 0;
                else
                    v81 = 2;
                v82 = v70 + 2;
                v150 = (v76->x > 0.0) | v81;
                if (v70[2].y <= 0.0)
                    v83 = 0;
                else
                    v83 = 2;
                v84 = (v70[2].x > 0.0) | v83;
                if (v150 != v84)
                {
                    v80->v1 = v76 - GFX_gpst_Geo->vertices;
                    v80->v2 = v150;
                    v80->v3 = v84;
                    v80->uv1 = 0;
                    v80->uv2 = 1;
                    v80->uv3 = 2;
                    ++v80;
                }
                v80->v1 = v76 - GFX_gpst_Geo->vertices;
                v80->v2 = v84;
                v80->v3 = v70 - GFX_gpst_Geo->vertices + 2;
                v80->uv1 = 0;
                v80->uv2 = 1;
                v80->uv3 = 2;
                v85 = v80 + 1;
                if (v70[2].y <= 0.0)
                    v86 = 0;
                else
                    v86 = 2;
                v87 = v70 + 3;
                v151 = (v82->x > 0.0) | v86;
                if (v70[3].y <= 0.0)
                    v88 = 0;
                else
                    v88 = 2;
                v89 = (v70[3].x > 0.0) | v88;
                if (v151 != v89)
                {
                    v85->v1 = v82 - GFX_gpst_Geo->vertices;
                    v85->v2 = v151;
                    v85->v3 = v89;
                    v85->uv1 = 0;
                    v85->uv2 = 1;
                    v85->uv3 = 2;
                    ++v85;
                }
                v85->v1 = v82 - GFX_gpst_Geo->vertices;
                v85->v2 = v89;
                v85->v3 = v70 - GFX_gpst_Geo->vertices + 3;
                v85->uv1 = 0;
                v85->uv2 = 1;
                v85->uv3 = 2;
                v90 = v85 + 1;
                if (v70[3].y <= 0.0)
                    v91 = 0;
                else
                    v91 = 2;
                v92 = v70 + 4;
                v152 = (v87->x > 0.0) | v91;
                if (v70[4].y <= 0.0)
                    v93 = 0;
                else
                    v93 = 2;
                v94 = (v70[4].x > 0.0) | v93;
                if (v152 != v94)
                {
                    v90->v1 = v87 - GFX_gpst_Geo->vertices;
                    v90->v2 = v152;
                    v90->v3 = v94;
                    v90->uv1 = 0;
                    v90->uv2 = 1;
                    v90->uv3 = 2;
                    ++v90;
                }
                v90->v1 = v87 - GFX_gpst_Geo->vertices;
                v90->v2 = v94;
                v90->v3 = v70 - GFX_gpst_Geo->vertices + 4;
                v90->uv1 = 0;
                v90->uv2 = 1;
                v90->uv3 = 2;
                v95 = v90 + 1;
                if (v70[4].y <= 0.0)
                    v96 = 0;
                else
                    v96 = 2;
                v97 = v70 + 5;
                v153 = (v92->x > 0.0) | v96;
                if (v70[5].y <= 0.0)
                    v98 = 0;
                else
                    v98 = 2;
                v99 = (v70[5].x > 0.0) | v98;
                if (v153 != v99)
                {
                    v95->v1 = v92 - GFX_gpst_Geo->vertices;
                    v95->v2 = v153;
                    v95->v3 = v99;
                    v95->uv1 = 0;
                    v95->uv2 = 1;
                    v95->uv3 = 2;
                    ++v95;
                }
                v95->v1 = v92 - GFX_gpst_Geo->vertices;
                v95->v2 = v99;
                v95->v3 = v70 - GFX_gpst_Geo->vertices + 5;
                v95->uv1 = 0;
                v95->uv2 = 1;
                v95->uv3 = 2;
                v100 = v95 + 1;
                if (v70[5].y <= 0.0)
                    v101 = 0;
                else
                    v101 = 2;
                v102 = v70 + 6;
                v154 = (v97->x > 0.0) | v101;
                if (v70[6].y <= 0.0)
                    v103 = 0;
                else
                    v103 = 2;
                v104 = (v70[6].x > 0.0) | v103;
                if (v154 != v104)
                {
                    v100->v1 = v97 - GFX_gpst_Geo->vertices;
                    v100->v2 = v154;
                    v100->v3 = v104;
                    v100->uv1 = 0;
                    v100->uv2 = 1;
                    v100->uv3 = 2;
                    ++v100;
                }
                v100->v1 = v97 - GFX_gpst_Geo->vertices;
                v100->v2 = v104;
                v100->v3 = v70 - GFX_gpst_Geo->vertices + 6;
                v100->uv1 = 0;
                v100->uv2 = 1;
                v100->uv3 = 2;
                v105 = v100 + 1;
                v106 = v70[6].y <= 0.0 ? 0 : 2;
                v107 = v70 + 7;
                v155 = (v102->x > 0.0) | v106;
                v108 = v70[7].y <= 0.0 ? 0 : 2;
                v109 = (v70[7].x > 0.0) | v108;
                if (v155 != v109)
                {
                    v105->v1 = v102 - GFX_gpst_Geo->vertices;
                    v105->v2 = v155;
                    v105->v3 = v109;
                    v105->uv1 = 0;
                    v105->uv2 = 1;
                    v105->uv3 = 2;
                    ++v105;
                }
                v105->v1 = v102 - GFX_gpst_Geo->vertices;
                v105->v2 = v109;
                v105->v3 = v70 - GFX_gpst_Geo->vertices + 7;
                v105->uv2 = 0;
                v105->uv2 = 1;
                v105->uv3 = 2;
                v68 = v105 + 1;
                v70 += 8;
                v69 = v107;
                v136 += 8;
                if (v136 >= 12)
                    break;
            }
            v110 = 4;
            v111 = GFX_gpst_Geo->vertices + 4;
            do
            {
                v111->x = v137 + v111->x;
                v111->y = v14 + v111->y;
                CAM_2Dto3D(&gpst_DX_stDD->ct.cam, v111, v111);
                GFX_gpst_Geo->vertexColors[++v110] = a1->color;
                ++v111;
            } while (v110 < 12);
        }
        else
        {
            v20 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
            v20->v1 = 2;
            v20->uv3 = 2;
            v20->v2 = 1;
            v20->uv2 = 1;
            v20->v3 = 0;
            v20->uv1 = 0;
            ++v20;
            v20->v1 = 2;
            v20->v2 = 3;
            v20->v3 = 1;
            v20->uv1 = 2;
            v20->uv2 = 3;
            v20->uv3 = 1;
        }
        renderingFlagsBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = renderingFlagsBackup & ~0x8000900u;
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = renderingFlagsBackup;
        ret = 1;
    }
    else
    {
        while (1)
        {
            v2 = v2 - *v3;
            ++v4;
            ++v3;
            if (v4 == 4)
                break;
            if (v2 <= *v3)
            {
                v140 = v2;
                goto LABEL_5;
            }
        }
        ret = 0;
    }
    return ret;
}